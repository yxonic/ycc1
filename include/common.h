#pragma once

#define nul             0x1
#define ident           0x2      
#define number          0x4      
#define plus            0x8      
#define minus           0x10     
#define times           0x20     
#define divide          0x40     
#define mod             0x80     
#define eql             0x100    
#define neq             0x200    
#define lss             0x400    
#define leq             0x800    
#define gtr             0x1000   
#define geq             0x2000   
#define lparen          0x4000   
#define rparen          0x8000   
#define comma           0x10000  
#define semicolon       0x20000  
#define assign          0x40000  
#define lbracket        0x80000  
#define rbracket        0x100000 
#define lbrace          0x200000 
#define rbrace          0x400000 
#define oddsym          0x800000 
#define constsym        0x1000000
#define intsym          0x2000000
#define ifsym           0x4000000
#define elsesym         0x8000000               
#define whilesym        0x10000000

int yylval;
unsigned long sym;      // last symbol read

int nsym;               // number of symbols
char symbols[256][32];  // maximum 256 identifiers, each with max
                        // length 32
