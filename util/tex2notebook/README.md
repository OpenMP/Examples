## Python Script for OpenMP Application Programming Interface Examples-version 5.0.1-June 2020
The tex2notebook.py script generates Jupyter Notebooks from the tex files of the OpenMP document and thus build a Jupyter Book. 

### Pre-requests
```
python3.6/3.8
Jupyter-notebook/Jupyter-lab
Jupyter-Book
Web browser
```

### Build Jupyter Book
```
$ cd examples-internal-main
$ cd notebook
$ mkdir contents
```

There are 2 choices for users, considering whether the users want the content of the source code file to be included in Jupyter notebook cells or use the `%load` magic.

```
$ python tex2notebook.py
```

This is default choice, and the content of code are copied to the cells directly.

```
$ python tex2notebook.py -use-load
```
The `-use-load` flag is for using `%load` magic with the file name in the code cell so users can click to load the file content into the cell. 
Plus, for both choices, filename, code language version, and OpenMP version is shown in the comments of corresponding code pieces.

```
$ jb build --path-output ../docs .
```
This is used for building Jupyter Book

```
$ rm -rf ./contents ./_build _toc.yml
```
This is used to clean all the generated materials.

### File Structure
```
-examples-internal-main/
-- notebook/
   -- _build/
      -- *.html      <= HTML index
   -- contents/
      -- *.ipynb     <= Jupyter notebooks
   -- _config.yml
   -- _toc.yml
   -- requirement.txt
   -- intro.md
   -- logo.jpg
   -- PythonScript
   -- Makefile
   -- README.md
-- source/           <= C/C++ and Fortran codes
   -- *.c
   -- *.cpp
   -- *.f
   -- *.f90
-- *.tex             <= TeX files 
```

### Other Details 
All the notebooks will be generated under the `contents` folder. The title page is replaced by the welcome page and TOC page are transformed to `_toc.yml` for Jyputer Book. The TOC file is mainly come from `openmp-examples.tex` which is the backbone of the whole book.

Each tex file is processed by the script which generates a single Jupyter notebook file. The textual content of the tex file is converted to markdown-formatted content enclosed in a notebook cell. A reference to a program source file (example source code in the example document) that appears in the tex file is converted to a cell that uses Jupyter `%load` magic, e.g. `%load ../sources/Example_parallel.1.c` such that running the cell causes loading the content of the file and then replacing the cell with the content. `%load` recognizes both absolute path and related path. Now the default one is `../sources/Example_*`.

Since all the OpenMP example source files are either C/C++ and Fortran. The correct setting for the notebook kernel
should be a kernel that can recognize C/C++ and Fortran.

### Notebook Copyright
The generated notebooks have the same copyright as the orginal tex file and the official OpenMP document. 

**Copyright © 1997-2016 OpenMP Architecture Review Board.
Permission to copy without fee all or part of this material is granted,
provided the OpenMP Architecture Review Board copyright notice and
the title of this document appear. Notice is given that copying is by
permission of OpenMP Architecture Review Board.**

### Acknowledgement
[Kewei Yan](https://github.com/ambipomyan) from HPCAS lab (https://passlab.github.io) of 
UNC Charlotte is the main developer of this script. When this becomes a question, 
the script is licensed with 3-clause BSD License, 
and Copyrighted @ 2019 by HPCAS (https://passlab.github.io) 
from University of North Carolina at Charlotte. All rights reserved. 
To contact, reach Kewei or Yonghong Yan (yanyh15@github or gmail). 

Funding for this development has been provided by the National Science Foundation 
under award number [CISE CCF 1833332](https://www.nsf.gov/awardsearch/showAward?AWD_ID=1833332). Please also
acknowledge OpenMP cOMPunity, one of the OpenMP ARB members. 
