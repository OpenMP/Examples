# Makefile for the OpenMP Examples document in LaTex format. 
# For more information, see the main document, openmp-examples.tex.

include versioninfo

default: openmp-examples.pdf
diff: openmp-diff-abridged.pdf

book: BOOK_BUILD="\\\\def\\\\bookbuild{1}"
book: VERSIONSTR="$(version_date)"
book: clean openmp-examples.pdf
	mv openmp-examples-${version}.pdf openmp-examples-${version}-book.pdf
release: VERSIONSTR="$(version_date)"
release: clean openmp-examples.pdf

CHAPTERS=Title_Page.tex \
	Foreword_Chapt.tex \
	Chap_*.tex \
	Deprecated_Features.tex \
	History.tex \
	*/*.tex

SOURCES=*/sources/*.c \
	*/sources/*.cpp \
	*/sources/*.f90 \
	*/sources/*.f 

INTERMEDIATE_FILES=openmp-examples.pdf \
		openmp-examples.toc \
		openmp-examples.lof \
		openmp-examples.lot \
		openmp-examples.idx \
		openmp-examples.aux \
		openmp-examples.ilg \
		openmp-examples.ind \
		openmp-examples.out \
		openmp-examples.log

LATEXCMD=pdflatex -interaction=batchmode -file-line-error
LATEXDCMD=$(LATEXCMD) -draftmode

# check for branches names with "name_XXX"
DIFF_TICKET_ID=$(shell git rev-parse --abbrev-ref HEAD)
GITREV=$(shell git rev-parse --short HEAD)
VERSIONSTR="GIT rev $(GITREV)"

openmp-examples.pdf: $(CHAPTERS) $(SOURCES) openmp.sty openmp-examples.tex openmp-logo.png generated-include.tex
	rm -f $(INTERMEDIATE_FILES)
	touch generated-include.tex
	$(LATEXDCMD) openmp-examples.tex
	makeindex -s openmp-index.ist openmp-examples.idx
	$(LATEXDCMD) openmp-examples.tex
	$(LATEXCMD) openmp-examples.tex
	cp openmp-examples.pdf openmp-examples-${version}.pdf

check:
	sources/check_tags

clean:
	rm -f $(INTERMEDIATE_FILES)
	rm -f generated-include.tex
	rm -f openmp-diff-full.pdf openmp-diff-abridged.pdf
	rm -rf *.tmpdir
	cd util; make clean
	rm -f chk_tags.log sources/*.log

realclean: clean
	rm -f openmp-examples-${version}.pdf openmp-examples-${version}-book.pdf

ifdef DIFF_TO
    VC_DIFF_TO := -r ${DIFF_TO}
else
    VC_DIFF_TO :=
endif
ifdef DIFF_FROM
    VC_DIFF_FROM := -r ${DIFF_FROM}
else
    VC_DIFF_FROM := -r main
endif

DIFF_TO:=HEAD
DIFF_FROM:=main
DIFF_TYPE:=UNDERLINE

COMMON_DIFF_OPTS:=--math-markup=whole  \
                  --append-safecmd=plc,code,hcode,scode,pcode,splc \
                  --append-textcmd=subsubsubsection

VC_DIFF_OPTS:=${COMMON_DIFF_OPTS} --force -c latexdiff.cfg --flatten --type="${DIFF_TYPE}" --git --pdf  ${VC_DIFF_FROM} ${VC_DIFF_TO}  --subtype=ZLABEL --graphics-markup=none

VC_DIFF_MINIMAL_OPTS:= --only-changes --force

generated-include.tex:
	echo "$(BOOK_BUILD)"
	echo "$(BOOK_BUILD)" > $@
	echo "\def\VER{${version}}" >> $@
	echo "\def\PVER{${version_spec}}" >> $@
	echo "\def\VERDATE{${VERSIONSTR}}" >> $@
	util/list_tags -vtag */sources/* >> $@

%.tmpdir: $(wildcard *.sty) $(wildcard *.png) $(wildcard *.aux) openmp-examples.pdf
	mkdir -p $@/sources
	for i in affinity devices loop_transformations parallel_execution SIMD tasking \
		 data_environment memory_model program_control synchronization \
		 directives ompt_interface;  do \
	  mkdir -p $@/$$i; ln -sf "$$PWD"/$$i/sources $@/$$i/sources; done
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

.PHONY: diff default book clean realclean
