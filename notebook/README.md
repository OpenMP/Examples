## tex2notebook.py for OpenMP examples document 
The tex2notebook.py script generates Jupyter Notebooks from the tex files of the OpenMP examples document. Each tex file is processed by the script which generates a single Jupyter notebook file. The textual content of the tex file is converted to markdown-formatted content enclosed in a notebook cell. A reference to a program source file (example source code in the example document) that appears in the tex file is converted to a cell that uses Jupyter `%load` magic, e.g. `%load ../sources/Example_parallel.1.c` such that running the cell causes loading the content of the file and then replacing the cell with the content. `%load` recognizes both absolute path and related path. Now the default one is `../sources/Example_*`.

Since all the OpenMP example source files are either C/C++ and Fortran. The correct setting for the notebook kernel
should be a kernel that can recognize C/C++ and Fortran. As of now, C/C++ and Fortran are not 
officially supported by Jupyter notebook. Thus the script sets Python as the notebook kernel (Python3 works as well). 
By doing that, users won't need to to set it manually when open a notebook. Of course, the C/C++/Fortran code is not compiliable or executable. User can always select a kernel from notebook interface if she has C/C++/Fortran kernel. 

### To convert

```
$ python tex2notebook.py
```

All the notebooks will be generated under current folders. The title page and TOC page are removed since they are not properly processed. 


### Notebook Copyright
The generated notebooks have the same copyright as the orginal tex file and the official OpenMP document. 

**Copyright © 1997-2016 OpenMP Architecture Review Board.
Permission to copy without fee all or part of this material is granted,
provided the OpenMP Architecture Review Board copyright notice and
the title of this document appear. Notice is given that copying is by
permission of OpenMP Architecture Review Board.**

### Acknowledgement
[Kewei Yan](https://github.com/ambipomyan) from PASSlab (https://passlab.github.io) of 
UNC Charlotte is the main developer of this script. The script is licensed with 3-clause BSD License, 
and Copyrighted @ 2019 by PASSlab (https://passlab.github.io) 
from University of North Carolina at Charlotte. All rights reserved. 
To contact, reach Kewei or Yonghong Yan (yanyh15@github or gmail). 

Funding for this development has been provided by the National Science Foundation 
under award number [CISE CCF 1833332](https://www.nsf.gov/awardsearch/showAward?AWD_ID=1833332). Please also
acknowledge OpenMP cOMPunity, one of the OpenMP ARB members. 
