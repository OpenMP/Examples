# Makefile for the OpenMP Examples document in LaTex format. 
# For more information, see the master document, openmp-examples.tex.

version=5.0.1
default: openmp-examples.pdf
diff: openmp-diff-abridged.pdf


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

# check for branches names with "name_XXX"
DIFF_TICKET_ID=$(shell git rev-parse --abbrev-ref HEAD)

openmp-examples.pdf: $(CHAPTERS) $(SOURCES) openmp.sty openmp-examples.tex openmp-logo.png
	rm -f $(INTERMEDIATE_FILES)
	pdflatex -interaction=batchmode -file-line-error openmp-examples.tex
	pdflatex -interaction=batchmode -file-line-error openmp-examples.tex
	pdflatex -interaction=batchmode -file-line-error openmp-examples.tex
	cp openmp-examples.pdf openmp-examples-${version}.pdf

clean:
	rm -f $(INTERMEDIATE_FILES)
	rm -f openmp-diff-full.pdf openmp-diff-abridged.pdf
	rm -rf *.tmpdir

ifdef DIFF_TO
    VC_DIFF_TO := -r ${DIFF_TO}
else
    VC_DIFF_TO :=
endif
ifdef DIFF_FROM
    VC_DIFF_FROM := -r ${DIFF_FROM}
else
    VC_DIFF_FROM := -r master
endif

DIFF_TO:=HEAD
DIFF_FROM:=master
DIFF_TYPE:=UNDERLINE

COMMON_DIFF_OPTS:=--math-markup=whole  \
                  --append-safecmd=plc,code,hcode,scode,pcode,splc \
                  --append-textcmd=subsubsubsection

VC_DIFF_OPTS:=${COMMON_DIFF_OPTS} --force -c latexdiff.cfg --flatten --type="${DIFF_TYPE}" --git --pdf  ${VC_DIFF_FROM} ${VC_DIFF_TO}  --subtype=ZLABEL --graphics-markup=none

VC_DIFF_MINIMAL_OPTS:= --only-changes --force

%.tmpdir: $(wildcard *.sty) $(wildcard *.png) $(wildcard *.aux) openmp-examples.pdf
	mkdir -p $@/sources
	mkdir -p $@/figs
	cp -f $^ "$@/"
	cp -f sources/* "$@/sources"
	cp -f figs/* "$@/figs"

openmp-diff-abridged.pdf: diff-fast-minimal.tmpdir openmp-examples.pdf
	env PATH="$(shell pwd)/util/latexdiff:$(PATH)" latexdiff-vc ${VC_DIFF_MINIMAL_OPTS} --fast -d $< ${VC_DIFF_OPTS} openmp-examples.tex
	cp $</openmp-examples.pdf $@
	if [ "x$(DIFF_TICKET_ID)" != "x" ]; then cp $@ ${@:.pdf=-$(DIFF_TICKET_ID).pdf}; fi

# Slow but portable diffs
openmp-diff-minimal.pdf: diffs-slow-minimal.tmpdir
	env PATH="$(shell pwd)/util/latexdiff:$(PATH)" latexdiff-vc ${VC_DIFF_MINIMAL_OPTS} -d $< ${VC_DIFF_OPTS} openmp-examples.tex
	cp $</openmp-examples.pdf $@
	if [ "x$(DIFF_TICKET_ID)" != "x" ]; then cp $@ ${@:.pdf=-$(DIFF_TICKET_ID).pdf}; fi
