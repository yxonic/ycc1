all: notes.pdf clean

notes.pdf: notes.tex ref.bib
	xelatex -halt-on-error notes.tex
	bibtex notes
	xelatex -halt-on-error notes.tex
	xelatex -halt-on-error notes.tex

clean:
	rm -f *.aux *.log *.out *.bbl *.blg *.toc
