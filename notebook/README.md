## tex2notebook.py script generates Jupyter Notebooks from the tex file

#### run

Convert .tex to .ipynb

read .tex -> change .tex(template) -> write .ipynb

```
$ python tex2notebook.py
```

#### notes
`%load` is supported by python, then kernel of jupuyter should be set to python or python3.

`%load` recognizes both absolute path and related path. Now the default one is `../sources/Example_*`.
