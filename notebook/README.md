## tex2notebook.py script generates Jupyter Notebooks from the tex file

#### run

Convert .tex to .ipynb

read .tex -> change .tex(template) -> write .ipynb

```
$ python ReadWrite.py
```

#### notes
`%load` is supported by python, then kernel of jupuyter should be set to python or python3.

`%load` recognizes absolute path. Now the default one is `~/Documents/Examples/sources/Example_*`