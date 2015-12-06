//===-- SmartPointerChecker.cpp------------------------------------*- C++ -*--//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Defines a checker for checking whether there are shared_ptr's that can be 
// transformed to unique_ptr's
//
//===----------------------------------------------------------------------===//

#include "ClangSACheckers.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

using namespace clang;
using namespace ento;

class RefState {
    enum Kind {
        SS_NotShared,
        SS_Shared
    };
    const Stmt *S;
    Kind K;
    RefState(Kind k, const Stmt *s) : S(s), K(k) { }
    
public:
    const Stmt *getStmt() const { return S; }
    bool isShared() const { return K == SS_Shared; }
    static RefState getUnsharedRef(const Stmt *s) { return RefState(SS_NotShared, s); }

    bool operator==(const RefState &X) const {
        return K == X.K && S == X.S;
    }

    void Profile(llvm::FoldingSetNodeID &ID) const {
        ID.AddInteger(K);
        ID.AddPointer(S);
    }    
};

namespace {
    class SmartPointerChecker : public Checker<check::PreStmt<CXXConstructExpr>,
                                               check::PostStmt<CallExpr>,
                                               check::DeadSymbols> {
    public:
        SmartPointerChecker() : II_make_shared(nullptr) { }
        void checkPreStmt(const CXXConstructExpr *CE, CheckerContext &C) const;
        void checkPostStmt(const CallExpr *CE, CheckerContext &C) const;
        void checkDeadSymbols(SymbolReaper &SymReaper, CheckerContext &C) const;

    private:
        mutable IdentifierInfo *II_make_shared;
        mutable std::unique_ptr<BuiltinBug> BT_abused_shared;
        mutable bool Shared = false;
    
        void initIdentifierInfo(ASTContext &C) const;
        void MakeAux(CheckerContext &C,
                     const CallExpr *CE) const;
        void CopyAux(CheckerContext &C,
                     const CXXConstructExpr *E) const;
        void ReportAbusedShared(CheckerContext &C,
                                SourceRange Range,
                                ProgramStateRef State) const;
    };
} // end anonymous namespace

REGISTER_MAP_WITH_PROGRAMSTATE(ShareState, SymbolRef, RefState)

void SmartPointerChecker::initIdentifierInfo(ASTContext &C) const {
    if (II_make_shared)
        return;
    II_make_shared = &C.Idents.get("make_shared");
}

void SmartPointerChecker::checkPreStmt(const CXXConstructExpr *CE, CheckerContext &C) const {
    if (CE->getType().getAsString().find("shared_ptr") != 0)
        return;
    const CXXConstructorDecl *CD = CE->getConstructor();
    if (!CD || !CD->isCopyConstructor())
        return;
    CopyAux(C, CE);
}

void SmartPointerChecker::checkPostStmt(const CallExpr *CE, CheckerContext &C) const {    
    const FunctionDecl *FD = C.getCalleeDecl(CE);
    if (!FD)
        return;
    
    if (FD->getKind() == Decl::Function) {
        initIdentifierInfo(C.getASTContext());
        IdentifierInfo *FunI = FD->getIdentifier();
        if (FunI != II_make_shared)
            return;
        MakeAux(C, CE);
    }
}

void SmartPointerChecker::checkDeadSymbols(SymbolReaper &SymReaper, CheckerContext &C) const {
    if (Shared)
        return;
    for (SymbolReaper::dead_iterator I = SymReaper.dead_begin(),
             E = SymReaper.dead_end(); I != E; ++I) {
        SymbolRef Sym = *I;
        ProgramStateRef State = C.getState();
        const RefState *RS = State->get<ShareState>(Sym);
        if (!RS)
            continue;
        llvm::errs() << "FUCK";
        ReportAbusedShared(C, RS->getStmt()->getSourceRange(), State);
    }
}

void SmartPointerChecker::MakeAux(CheckerContext &C,
                                  const CallExpr *CE) const {
    ProgramStateRef state = C.getState();
    SValBuilder &svalBuilder = C.getSValBuilder();
    const LocationContext *LCtx = C.getPredecessor()->getLocationContext();
    DefinedSVal RetVal = svalBuilder.conjureSymbolVal(nullptr, CE, LCtx,
                                                      C.blockCount())
        .castAs<DefinedSVal>();
    if (SymbolRef Sym = RetVal.getAsSymbol()) {
        state->set<ShareState>(Sym, RefState::getUnsharedRef(CE));
        C.addTransition(state);
    }
}

void SmartPointerChecker::CopyAux(CheckerContext &C, const CXXConstructExpr *E) const {
    Shared = true;
    ProgramStateRef State = C.getState();
    SVal EVal = State->getSVal(E, C.getLocationContext());
    SymbolRef Sym = EVal.getAsSymbol();
    // FIXME: Arguments have different SVal settings, which makes it
    // very difficult to count usage.
    if (!Sym)
        return;
}

void SmartPointerChecker::ReportAbusedShared(CheckerContext &C,
                                             SourceRange Range,
                                             ProgramStateRef State) const {
    ExplodedNode *N = C.generateSink();
    if (!BT_abused_shared)
        BT_abused_shared.reset(
            new BuiltinBug(this, "Abused shared_ptr",
                           "The use of the shared_ptr could be replaced by a unique_ptr."));
    BugReport *R = new BugReport(*BT_abused_shared,
                                 BT_abused_shared->getDescription(), N);
    R->addRange(Range);
    C.emitReport(R);
}

void ento::registerSmartPointerChecker(CheckerManager &mgr) {
    mgr.registerChecker<SmartPointerChecker>();
}
