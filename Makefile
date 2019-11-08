# Makefile for the OpenMP Examples document in LaTex format. 
# For more information, see the master document, openmp-examples.tex.

version=5.0.0
default: openmp-examples.pdf


CHAPTERS=Title_Page.tex \
	Foreword_Chapt.tex \
	Introduction_Chapt.tex \
	Examples_*.tex \
	History.tex

SOURCES=sources/*.c \
	sources/*.cpp \
	sources/*.f90 \
	sources/*.f 

INTERMEDIATE_FILES=openmp-examples.pdf \
		openmp-examples.toc \
		openmp-examples.idx \
		openmp-examples.aux \
		openmp-examples.ilg \
		openmp-examples.ind \
		openmp-examples.out \
		openmp-examples.log

openmp-examples.pdf: $(CHAPTERS) $(SOURCES) openmp.sty openmp-examples.tex openmp-logo.png
	rm -f $(INTERMEDIATE_FILES)
	pdflatex -interaction=batchmode -file-line-error openmp-examples.tex
	pdflatex -interaction=batchmode -file-line-error openmp-examples.tex
	pdflatex -interaction=batchmode -file-line-error openmp-examples.tex
	cp openmp-examples.pdf openmp-examples-${version}.pdf

clean:
	rm -f $(INTERMEDIATE_FILES)

