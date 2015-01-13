# Makefile for the OpenMP Examples document in LaTex format. 
# For more information, see the master document, openmp-examples.tex.

version=4.0.1ltx
default: openmp-examples.pdf


CHAPTERS=Title_Page.tex \
	Introduction_Chapt.tex \
	Examples_Chapt.tex \
	Examples_ploop.tex \
	Examples_mem_model.tex \
	Examples_cond_comp.tex \
	Examples_icv.tex \
	Examples_parallel.tex \
	Examples_nthrs_nesting.tex \
	Examples_nthrs_dynamic.tex \
	Examples_affinity.tex \
	Examples_fort_do.tex \
	Examples_fort_loopvar.tex \
	Examples_nowait.tex \
	Examples_collapse.tex \
	Examples_psections.tex \
	Examples_fpriv_sections.tex \
	Examples_single.tex \
	Examples_tasking.tex \
	Examples_taskgroup.tex \
	Examples_taskyield.tex \
	Examples_workshare.tex \
	Examples_master.tex \
	Examples_critical.tex \
	Examples_worksharing_critical.tex \
	Examples_barrier_regions.tex \
	Examples_atomic.tex \
	Examples_atomic_restrict.tex \
	Examples_flush_nolist.tex \
	Examples_standalone.tex \
	Examples_ordered.tex \
	Examples_cancellation.tex \
	Examples_threadprivate.tex \
	Examples_pra_iterator.tex \
	Examples_fort_sp_common.tex \
	Examples_default_none.tex \
	Examples_fort_race.tex \
	Examples_private.tex \
	Examples_fort_sa_private.tex \
	Examples_carrays_fpriv.tex \
	Examples_lastprivate.tex \
	Examples_reduction.tex \
	Examples_copyin.tex \
	Examples_copyprivate.tex \
	Examples_nested_loop.tex \
	Examples_nesting_restrict.tex \
	Examples_set_dynamic_nthrs.tex \
	Examples_get_nthrs.tex \
	Examples_init_lock.tex \
	Examples_lock_owner.tex \
	Examples_simple_lock.tex \
	Examples_nestable_lock.tex \
	Examples_target.tex \
	Examples_target_data.tex \
	Examples_target_update.tex \
	Examples_declare_target.tex \
	Examples_teams.tex \
	Examples_async_target.tex \
	Examples_array_sections.tex \
	Examples_device.tex \
	Examples_associate.tex

INTERMEDIATE_FILES=openmp-examples.pdf \
		openmp-examples.toc \
		openmp-examples.idx \
		openmp-examples.aux \
		openmp-examples.ilg \
		openmp-examples.ind \
		openmp-examples.out \
		openmp-examples.log

openmp-examples.pdf: $(CHAPTERS) openmp.sty openmp-examples.tex openmp-logo.png
	rm -f $(INTERMEDIATE_FILES)
	pdflatex -interaction=batchmode -file-line-error openmp-examples.tex
	pdflatex -interaction=batchmode -file-line-error openmp-examples.tex
	pdflatex -interaction=batchmode -file-line-error openmp-examples.tex
	cp openmp-examples.pdf openmp-examples-${version}.pdf

clean:
	rm -f $(INTERMEDIATE_FILES)

