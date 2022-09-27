#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-

import re
import os
import glob
import sys
import argparse

###################################
# Handling Jyputer Notebook cells #
###################################
## Language sets for Jupyter Notebook
### Native Kernel is used for executing C\C++ code
### in this case, code highlighting is ignored
LanguageSet = ' ],\n'                              + \
              ' "metadata": {\n'                   + \
              '  "kernelspec": {\n'                + \
              '   "display_name": "Native",\n'     + \
              '   "language": "native",\n'         + \
              '   "name": "native"\n'              + \
              '  },\n'                             + \
              '  "language_info": {\n'             + \
              '   "file_extension": ".c",\n'       + \
              '   "mimetype": "text/plain",\n'     + \
              '   "name": "none"\n'                + \
              '  }\n'                              + \
              ' },\n'                              + \
              ' "nbformat": 4,\n'                  + \
              ' "nbformat_minor": 2\n'             + \
              '}'
## Cell sets for jupyter notebook
### Markdown cell begins
MB = '  {\n'                         + \
     '   "cell_type": "markdown",\n' + \
     '   "metadata": {},\n'          + \
     '   "source": [\n'              + \
     '    "'
### Code cell begins
CB = '  {\n'                         + \
     '   "cell_type": "code",\n'     + \
     '   "execution_count": null,\n' + \
     '   "metadata": {},\n'          + \
     '   "outputs": [],\n'           + \
     '   "source": [\n'              + \
     '    "'
### Cell ends
E = '"\n   ]\n  },\n'
### Last cell ends
E2 = '"\n   ]\n  }\n'

########################
# Handling LaTeX marks #
########################
## \code
def rep_code(match):
    val = match.group()
    l = len(val)
    out = '`' + val[6 :(l - 1)] + '`'
    return out
def replace_code(Str):
    Str = re.sub(r'\\code\{[^\{\}]*\}', rep_code, Str)
    Str = re.sub(r'\\code', '', Str)
    return Str

## \scode
def rep_scode(match):
    val = match.group()
    l = len(val)
    out = '`' + val[7 :(l - 1)] + '`'
    return out
def replace_scode(Str):
    Str = re.sub(r'\\scode\{[^\{\}]*\}', rep_scode, Str)
    Str = re.sub(r'\\scode', '', Str)
    return Str

## \plc
def rep_plc(match):
    val = match.group()
    l = len(val)
    out = ' _' + val[5 :(l - 1)] + '_ '
    return out
def replace_plc(Str):
    Str = re.sub(r'\\plc\{[^\{\}]*\}', rep_plc, Str)
    return Str
    
## \splc
def rep_splc(match):
    val = match.group()
    l = len(val)
    out = ' _' + val[6 :(l - 1)] + '_ '
    return out
def replace_splc(Str):
    Str = re.sub(r'\\splc\{[^\{\}]*\}', rep_splc, Str)
    return Str

## \texttt
def rep_texttt(match):
    val = match.group()
    l = len(val)
    out = ' **' + val[8 :(l - 1)] + '** '
    return out
def replace_texttt(Str):
    Str = re.sub(r'textsc', 'texttt', Str)
    Str = re.sub(r'texttt', 'texttt', Str)
    Str = re.sub(r'textbf', 'texttt', Str)
    Str = re.sub(r'texit', 'texttt', Str)
    Str = re.sub(r'\\texttt\{[^\{\}]*\}', rep_texttt, Str)
    return Str

## \_
def replace_us(Str):
    Str = re.sub('\\\_', '_', Str)
    Str = re.sub('\\\#', '#', Str)
    Str = re.sub('\\\&', '&', Str)
    return Str

## quotes in text contents
def replace_quote(Str):
    Str = re.sub('``', '\"', Str)
    Str = re.sub('\"', '\\\"', Str)
    return Str

## \emph
def rep_emph(match):
    val = match.group()
    l = len(val)
    out = '**' + val[6 :(l - 1)] + '**'
    return out
def replace_emph(Str):
    Str = re.sub(r'\\emph\{[^\{\}]*\}', rep_emph, Str)
    return Str

## \item
def replace_item(Str):
    Str = re.sub(r'\\item', '\n* ', Str)
    return Str
    
## \index
def rep_index(match):
    val = match.group()
    l = len(val)
    out = '**' + val[7 :(l - 1)] + '**'
    return out
def replace_index(Str):
    Str = re.sub(r'\\index\{[^\{\}]*\}', rep_index, Str)
    return Str

## $ $
def rep_math(match):
    val = match.group()
    l = len(val)
    out = ' ' + val[1 :(l - 1)] + ' '
    return out
def replace_math(Str):
    Str = re.sub(r'\$\\times\$', ' x ', Str)
    Str = re.sub(r'\$\\ldots\$', ' ... ', Str)
    Str = re.sub(r'\$[\w\W]\_[\w\W\d]\$', rep_math, Str)
    Str = re.sub(r'\$[\w\W]\$', rep_math, Str)
    return Str

## link
def rep_link(match):
    val = match.group()
    l = len(val)
    out = ' ' + val[13 :(l - 1)] + ' '
    return out
def replace_link(Str):
    Str = re.sub(r'\\specref\{sec:[\s\w_-]+\}', rep_link, Str)
    return Str

def rep_link2(match):
    val = match.group()
    l = len(val)
    out = ' ' + val[16 :(l - 1)] + ' '
    return out
def replace_link2(Str):
    Str = re.sub(r'\\specref\{subsec:[\s\w_-]+\}', rep_link2, Str)
    return Str

## \pageref
def rep_pageref(match):
    val = match.group()
    l = len(val)
    out = ' **' + val[14 :(l - 1)] + '** '
    return out
def replace_pageref(Str):
    Str = re.sub(r'\\pageref\{chap:[\w]+_*[\w]+\}', rep_pageref, Str)
    return Str

## \href
def rep_href(match):
    val = match.group()
    l = len(val)
    out = '[' + val[6 :(l - 1)] + ']'
    return out
def replace_href(Str):
    Str = re.sub(r'\\href\{[^\{\}]*\}', rep_href, Str)
    return Str
    
def rep_web(match):
    val = match.group()
    l = len(val)
    out = '](' + val[2 :(l - 1)] + ')'
    return out
def replace_web(Str):
    Str = re.sub(r'\]\{http[^\{\}]*\}', rep_web, Str)
    return Str


## marks never be used, such as \pagebreak, label{.*}, etc.
def delete_marks(Str):
    Str = re.sub(r'\t', ' ', Str)
    Str = re.sub(r'\\pagebreak', '', Str)
    Str = re.sub(r'\\label\{[^\{\}]*\}', '', Str)
    Str = re.sub(r'\\fortranspecificstart', '', Str)
    Str = re.sub(r'\\fortranspecificend', '', Str)
    Str = re.sub(r'\\cppspecificstart', '', Str)
    Str = re.sub(r'\\cppspecificend', '', Str)
    Str = re.sub(r'\\ccppspecificstart', '', Str)
    Str = re.sub(r'\\ccppspecificend', '', Str)
    Str = re.sub(r'\\clearpage', '', Str)
    Str = re.sub(r'\\bigskip', '\n\n', Str)
    Str = re.sub(r'\\smallskip', '\n', Str)
    Str = re.sub(r'\\begin\{[\w]*\}', '', Str)
    Str = re.sub(r'\\end\{[\w]*\}', '', Str)
    Str = re.sub(r'\\\$', '$', Str)
    Str = re.sub(r'\\\{', '{', Str)
    Str = re.sub(r'\\\}', '}', Str)
    Str = re.sub(r'\\ifpdf.*\\fi\s*', ' ', Str)
    Str = re.sub(r'\\item\[\]\\hspace\*\{5mm\}', '', Str)
    Str = re.sub(r'\~', ' ', Str)
    Str = re.sub(r'\s\\noteheader\{--\}\s', '\n', Str)
    Str = re.sub(r'\\small\{\}', '', Str)
    Str = re.sub(r'\\small', '', Str)
    Str = re.sub(r'\\;', '', Str)
    Str = re.sub(r'\\linebreak', '\n', Str)
    Str = re.sub(r'\\VER\{\}', '5.0.1', Str)
    Str = re.sub(r'\\VER', '5.0.1', Str)
    
    return Str

## inputs
def rep_input(match):
    val = match.group()
    l = len(val)
    out = '\n' + "../../" + val[7 :(l - 1)] + ".tex"
    #out = "../../" + out + ".tex"
    return out
    
def replace_input2(Str):
    Str = re.sub(r'\\input\{[a-zA-Z0-9/_]*\}', rep_input, Str)
    #Str = "../../" + Str + ".tex"
    #print(Str)
    return Str
    
## sections
### \chapter
def rep_chapter(match):
    val = match.group()
    l = len(val)
    out = '\n# ' + val[9 :(l - 1)] + '\n'
    return out
### \cchapter
def rep_cchapter(match):
    val = match.group()
    l = len(val)
    out = '\n# ' + val[10 :(l - 1)] + '\n'
    return out
def replace_chapter(Str):
    Str = re.sub('\\\\chapter\*', '\\\\chapter', Str)
    Str = re.sub(r'\\chapter\{[^\{\}]*\}', rep_chapter, Str)
    Str = re.sub(r'\\cchapter\{[^\{\}]*\}', rep_cchapter, Str)
    return Str
### \section
def rep_section(match):
    val = match.group()
    l = len(val)
    out = '\n## ' + val[9 :(l - 1)] + '\n'
    return out
def replace_section(Str):
    Str = re.sub(r'\\section\{[^\{\}]*\}', rep_section, Str)
    return Str
### \subsection
def rep_subsec(match):
    val = match.group()
    l = len(val)
    out = '\n### ' + val[12 :(l - 1)] + '\n'
    return out
def replace_subsec(Str):
    Str = re.sub(r'\\subsection\{[^\{\}]*\}', rep_subsec, Str)
    return Str

def replace_sections(Str):
    Str = replace_chapter(Str)
    Str = replace_section(Str)
    Str = replace_subsec(Str)
### \ keep in line show be removed
    Str = re.sub(r'\\\\', '', Str)
    return Str

## examples
def rep_cexample(match):
    val = match.group()
    l = len(val)
    out = '\n' + val + '\n'
    return out
def replace_cexample(Str):
    Str = re.sub(r'\\cexample[\[\d\.\d\]]*\{[^\{\}]*\}\{[^\{\}]*\}', rep_cexample, Str)
    return Str

def rep_fexample(match):
    val = match.group()
    l = len(val)
    out = '\n' + val + '\n'
    return out
def replace_fexample(Str):
    Str = re.sub(r'\\fexample[\[\d\.\d\]]*\{[^\{\}]*\}\{[^\{\}]*\}', rep_fexample, Str)
    return Str

def rep_cppexample(match):
    val = match.group()
    l = len(val)
    out = '\n' + val + '\n'
    return out
def replace_cppexample(Str):
    Str = re.sub(r'\\cppexample[\[\d\.\d\]]*\{[^\{\}]*\}\{[^\{\}]*\}', rep_cppexample, Str)
    return Str

def rep_cnexample(match):
    val = match.group()
    l = len(val)
    out = '\n' + val + '\n'
    return out
def replace_cnexample(Str):
    Str = re.sub(r'\\cnexample[\[\d\.\d\]]*\{[^\{\}]*\}\{[^\{\}]*\}', rep_cnexample, Str)
    return Str

def rep_cppnexample(match):
    val = match.group()
    l = len(val)
    out = '\n' + val + '\n'
    return out
def replace_cppnexample(Str):
    Str = re.sub(r'\\cppnexample[\[\d\.\d\]]*\{[^\{\}]*\}\{[^\{\}]*\}', rep_cppnexample, Str)
    return Str

def rep_fnexample(match):
    val = match.group()
    l = len(val)
    out = '\n' + val + '\n'
    return out
def replace_fnexample(Str):
    Str = re.sub(r'\\fnexample[\[\d\.\d\]]*\{[^\{\}]*\}\{[^\{\}]*\}', rep_fnexample, Str)
    return Str

def rep_cfreeexample(match):
    val = match.group()
    l = len(val)
    out = '\n' + val + '\n'
    return out
def replace_cfreeexample(Str):
    Str = re.sub(r'\\cfreeexample[\[\d\.\d\]]*\{[^\{\}]*\}\{[^\{\}]*\}', rep_cfreeexample, Str)
    return Str

def rep_ffreeexample(match):
    val = match.group()
    l = len(val)
    out = '\n' + val + '\n'
    return out
def replace_ffreeexample(Str):
    Str = re.sub(r'\\ffreeexample[\[\d\.\d\]]*\{[^\{\}]*\}\{[^\{\}]*\}', rep_ffreeexample, Str)
    return Str
    
def rep_ffreenexample(match):
    val = match.group()
    l = len(val)
    out = '\n' + val + '\n'
    return out
def replace_ffreenexample(Str):
    Str = re.sub(r'\\ffreenexample[\[\d\.\d\]]*\{[^\{\}]*\}\{[^\{\}]*\}', rep_ffreenexample, Str)
    return Str

def replace_examples(Str):
    Str = replace_cexample(Str)
    Str = replace_fexample(Str)
    Str = replace_cppexample(Str)
    Str = replace_cnexample(Str)
    Str = replace_cppnexample(Str)
    Str = replace_fnexample(Str)
    Str = replace_cfreeexample(Str)
    Str = replace_ffreeexample(Str)
    Str = replace_ffreenexample(Str)
    return Str

def gen_markdown_cell(Str):
    Str = Str.strip()
    return Str

#####################################
# Get Full Text, Handling \input{ } #
#####################################
def get_folder_name(Str):
    #print("Hello!" + Str)
    folder = Str.partition('_')[2]
    folder = folder.partition('.')[0]
    print(folder)
    return folder
    
def insert_dot_tex_file(Str, file):
    folder = ''
    if ('.tex' in Str) and (len(Str) <= 50): # handleling the length of file_names
        #Str = 'please insert LaTex file here!\n'
        # clean spaces and \n
        Str = re.sub(r'\n', '', Str)
        Str = re.sub(r' ', '', Str)
        #print(Str)
        f1 = open(Str)
        line = f1.readline()
        while line:
            file.write(line)
            line = f1.readline()
    
    return 0
    
def gen_full_text(input):
# prepare input/tmp/output
    output = input[0:-4] + '_tmp.tex'
    tmp    = input[0:-4] + '_tmp.txt'
# read input by line, save to tmp: .txt files
    f1 = open(input)
    f2 = open(tmp, 'w')
    line = f1.readline()
    while line:
        f2.write(line)
        line = f1.readline()
        line = replace_input2(line)
    f1.close()
    f2.close()
# read tmp by line, save back to output: .tex files
    f1 = open(tmp)
    f2 = open(output, 'w')
    line = f1.readline()
    while line:
        f2.write(line)
        line = f1.readline()
        status = insert_dot_tex_file(line, f2)
    f1.close()
    f2.close()
    
    return output

###################################
# Handling Markdown text contents #
###################################
def gen_single_line(Str):
## indents
    Str = re.sub(r'\n', ' ', Str)
    if (Str.strip() == ''):
        Str = '\n'
## delete comments
    Str = re.sub(r'\\\%', '', Str)
    Str = re.sub(r'%.*', '', Str)
## delete other unused marks
    Str = re.sub(r'\\addcontentsline.*', '', Str)
    Str = re.sub(r'\\linewitharrows.*', '', Str)
    Str = re.sub(r'\[t\!\].*', '', Str)
    return Str

def gen_cells(Str):
## get input{} in Latex
    Str = replace_input2(Str);
## delete marks
    Str = delete_marks(Str)
## \plc
    Str = replace_plc(Str)
## \splc
    Str = replace_splc(Str)
## \code
    Str = replace_code(Str)
## \scode
    Str = replace_scode(Str)
## \texttt
    Str = replace_texttt(Str)
## \_
    Str = replace_us(Str)
## \emph
    Str = replace_emph(Str)
## \item
    Str = replace_item(Str)
## \index
    Str = replace_index(Str)
## $ $
    Str = replace_math(Str)
## link
    Str = replace_link(Str)
    Str = replace_link2(Str)
## \pageref
    Str = replace_pageref(Str)
## \href
    Str = replace_href(Str)
    Str = replace_web(Str)
## replace sections
    Str = replace_sections(Str)
## replace examples
    Str = replace_examples(Str)
## quote
    Str = replace_quote(Str)
    return Str

######################################
# Handling C/C++ or Fortran examples #
######################################
## grab text of code
def get_text(Str):
    codes = ''
    lan = 'C'
    if '.f' in Str:
        lan = 'Fortran'
    f = open(Str)
    #f = open("tests.txt")
    line = f.readline()
    while line:
## rewrite '\', '\n', '\t', '"' and, '@'
        line = re.sub(r'\\', '\\\\\\\\', line)
        line = re.sub(r'\"', '\\\"', line)
        line = re.sub(r'@', '', line)
        line = re.sub(r'[\t]', ' ', line)
        if ('linkable:' in line):
            line = ''
        if ('compilable:' in line):
            line = ''
        if ('expect:' in line):
            line = ''
        
        if (line != ''):
            codes = codes + '    "' + line.rstrip() + '\\n",\n'
        else:
            codes = codes + line
            
        line = f.readline()
    f.close()

## add command line args (C/C++ code only)
    if lan == 'C':
        com = '//%compiler: clang\\n",\n "//%cflags: -fopenmp\\n",\n "\\n",\n'
    else:
        com = '\\n",\n'
    
    return com + \
           codes   + \
           '    "\\n'
## There are 9 different kinds of examples
## These lines are marked with specific words
## such as "cexample"
## This part is to generate the file name of source code
## For instance, the flag in .tex file is
## "\cexample{xxxx}{1}"
## The source code name should be
## "~/Example_xxxx.1.c"
def gen_code_cell(Str, flg, folder_name):
## \ is not allowed in .ipynb
    Str = re.sub('\\\\', '', Str)
## version number [0-9.0-9]
    Str = re.sub(r'\[\d\.\d\]', '', Str)
## dot
    Str = re.sub('}{', '.', Str)
## lquote
    Str = re.sub('{', '', Str)
## rquote
    if ('cexample' in Str):
        Str = re.sub('cexample', '', Str)
        Str = re.sub('}', '.c', Str)
    if ('fexample' in Str):
        Str = re.sub('fexample', '', Str)
        Str = re.sub('}', '.f', Str)
    if ('cppexample' in Str):
        Str = re.sub('cppexample', '', Str)
        Str = re.sub('}', '.cpp', Str)
    if ('cnexample' in Str):
        Str = re.sub('cnexample', '', Str)
        Str = re.sub('}', '.c', Str)
    if ('cppnexample' in Str):
        Str = re.sub('cppnexample', '', Str)
        Str = re.sub('}', '.cpp', Str)
    if ('fnexample' in Str):
        Str = re.sub('fnexample', '', Str)
        Str = re.sub('}', '.f', Str)
    if ('cfreeexample' in Str):
        Str = re.sub('cfreeexample', '', Str)
        Str = re.sub('}', '.c', Str)
    if ('ffreeexample' in Str):
        Str = re.sub('ffreeexample', '', Str)
        Str = re.sub('}', '.f90', Str)
    if ('ffreenexample' in Str):
        Str = re.sub('ffreenexample', '', Str)
        Str = re.sub('}', '.f90', Str)

## newline
    Str = re.sub(r'\n', '', Str)
## //%load or show code
    if (flg == 1):
        #return '//%load ../../sources/Example_' + Str
        return '//%load ../../' + folder_name + '/sources/' + Str
    else:
        #return get_text('../../sources/Example_' + Str)
        return get_text('../../' + folder_name + '/sources/' + Str)

################
# Handling TOC #
################
## get rid of all the comments
def replace_comments(Str):
    if ('%' in Str):
        return ''
    else:
        return Str

## get rid of the lines with .sty files
### it is also in \include{}
### while it is no use for building Notebooks
def replace_sty(Str):
    if ('.sty' in Str):
        return ''
    else:
        return Str

## get rid of title page
## for it is remake by Jupyter Book
def replace_title_page(Str):
    if ('Title_Page' in Str):
        return ''
    else:
        return Str
        
def check_content1(Str):
    if ('Chap' in Str):
        return Str
    else:
        return ""


## only the lines start with \input{} would be chosen
def check_content(Str):
    #s = '\input'
    #t = 'Chap'
    #result = s in Str
    #result2 = t in Str
    if('\input{Chap_' in Str):
        return Str
    else:
        return ''

## at mean time, the \input{} should be replaced by -file
def replace_input(Str):
    Str = re.sub(r'\ \ \ \ \\input\{', '- file: contents/', Str)
    Str = re.sub(r'\}', '', Str)
    #l = len(Str)
    #Str = '\n' + Str[0 :(l - 1)] + ".ipynb"
    #Str = Str + ".ipynb"
    #print(Str)
    return Str

## inserting "sections:" to toc
def insert_sections(Str, pre, post):
    sec = ''
    tmp = pre - post - 1
    if (tmp > 0):
        if (post > 0):
            tmp = tmp + 1
        for i in range(tmp):
            sec = ' ' + sec
        Str = sec + 'sections:\n' + Str
    return Str
        
def gen_toc_file(Str):
    Str = replace_comments(Str)
    Str = check_content1(Str)
    Str = replace_sty(Str)
    Str = replace_title_page(Str)
    Str = check_content(Str)
    Str = replace_input(Str)
    return Str

########
# main #
########
##################
# Receive inputs #
##################
## default inputs, path to .tex files
path = '../../'
dir = os.listdir(path)
## default outputs, path to .ipynb contents
contents_folder = '../../notebook'
## arg parser
parser = argparse.ArgumentParser()
parser.add_argument("-useLoad", action='store_true', help = "Use //%load magic of Native kernel")
parser.add_argument("-i", help = "Path for .tex input files, if not specified, default path will be used")
parser.add_argument("-o", help = "path for .ipynb output files, if not specified, default path will be used")
args = parser.parse_args()

if args.useLoad:
    flg = 1
else:
    flg = 0

if args.i:
    path = args.i
    
if args.o:
    contents_folder = args.o

yml_folder = contents_folder + '/'
contents_folder = contents_folder + '/contents/'
#print(path)
#print(contents_folder)
#print(yml_folder)

##############################
# Write to Jupyter Notebooks #
##############################
## file lists
mylists = []
for i in dir:
    if os.path.splitext(i)[1] == '.tex':
        mylists.append(i)

## marks for code cells
CodeList = ['cexa', 'cnex', 'cppe', 'cppn', 'fexa', 'fnex', 'ffre', 'cfre']
## create intermediate test files for futher translation
for FileName in mylists:
    #print(FileName)
    FileLen = len(FileName)
    input = path + FileName
    if not os.path.exists(contents_folder):
        os.makedirs(contents_folder)
    output = contents_folder + FileName[:(FileLen - 4)] + '.ipynb'
    testfile = 'TEST' + FileName
    tmp_file = "tmp.txt"
    # gen_full_texts
    input2 = gen_full_text(input)
    # get folder name
    folder_name = get_folder_name(input)
    
    # check
    print("input:       " + input)
    print("tmp:         " + input2)
    print("folder_name: " + folder_name)

## turn the text contents to a single line
    f1 = open(input2)
    f2 = open(tmp_file, 'w')
    line = f1.readline()
## write to raw text contents file from inputs
    while line:
        line = gen_single_line(line)
        f2.write(line)
        line = f1.readline()
    f1.close()
    f2.close()
    
    f1 = open(tmp_file)
    f2 = open(testfile, 'w')
    s = f1.read()
    s = gen_cells(s)
    f2.write(s)
    #print(s)
    f1.close()
    f2.close()
    
## write to results from raw text contents
    f1 = open(testfile)
    f2 = open(output, 'w')
    line = f1.readline()
    f2.write('{\n "cells": [\n')
    while line:
        if line[1:5] in CodeList:
            line = gen_code_cell(line, flg, folder_name)
            f2.write(CB)
            f2.write(line)
            f2.write(E)
        else :
            line = gen_markdown_cell(line)
            f2.write(MB)
            f2.write(line)
            f2.write(E)
        line = f1.readline()
    f2.write(MB)
    f2.write('')
    f2.write(E2)
    f2.write(LanguageSet)
    os.remove(tmp_file)
    os.remove(testfile)

    f1.close()
    f2.close()
#os.remove(contents_folder + "Title_Page.ipynb")
#os.remove(contents_folder + "openmp-example.ipynb")
#os.remove(contents_folder + "openmp-examples.ipynb")

# remove temprary files
for filename in glob.glob("../../*_tmp.tex"):
    os.remove(filename)
for filename in glob.glob("../../*_tmp.txt"):
    os.remove(filename)

#####################
# Generate _toc.yml #
#####################
## _toc.yml file begins
begin = 'format: jb-article\n'
begin += 'root: intro\n'
begin += 'sections:\n'
## TOC
fileName = path + 'openmp-examples.tex'
## open main.tex file to get target lines
f = open(fileName)
g = open("tmp.txt", 'w')
g.write(begin)
line = f.readline()
while line:
## pick up all the comments, which can be at the end of code line
    line = re.sub('%', '\\n%', line)
    g.write(line)
    line = f.readline()

f.close()
g.close()

## open main.tex file to get target lines
f = open("tmp.txt")
g = open(yml_folder +"_toc_inter.yml", 'w')
g.write(begin)
line = f.readline()
while line:
    line = gen_toc_file(line)
    g.write(line)
    line = f.readline()

f.close()
g.close()

## insert "sections:" to toc file
pre = 0
post = 0
f = open(yml_folder + "_toc_inter.yml")
g = open(yml_folder + "_toc.yml", 'w')
line = f.readline()
while line:
    pre = len(line) - len(line.lstrip(' '))
    #print(str(pre) + ' ' + str(post))
    line = insert_sections(line, pre, post)
    g.write(line)
    
    line = f.readline()
    post = line.count(' ')
    
    swap = pre
    pre = post
    post = swap

f.close()
g.close()
os.remove(yml_folder + "_toc_inter.yml")
os.remove("tmp.txt")
