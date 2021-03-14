.SILENT: notebook
default:
	python tex2notebook.py

notebook:
	cp -r ../../notebook tmp
	rm -rf tmp/contents
	rm tmp/_toc.yml
	python tex2notebook.py -i ../../ -o tmp
	jb build tmp 2>&1 | grep WARNING > /dev/null
	if [ $$? == 1 ]; then \
		rm -rf ../../notebook/contents ../../notebook/_toc.yml;\
		cp -r tmp/contents ../../notebook/;\
		cp tmp/_toc.yml ../../notebook/;\
		echo "Jupyter Notebooks and toc file are generated successfully.";\
	else \
		echo "Warnings are found while generating Jupyter Notebooks. Please manually run the script to review the warnings.";\
	fi
	rm -rf tmp

clean:
	rm -rf ../../notebook/contents ../../notebook/_toc.yml