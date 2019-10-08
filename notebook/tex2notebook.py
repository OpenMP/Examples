#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-

import re
import os

# ipynb sets
# previously these settings are from .txt, now they are included in this file
# languageSet is the code blocks in the end of the .ipynb file to show the setting of kernels
# in this situaiton, the kernel is called Native, which is a C kernel.
LanguageSet = ' ],\n' + \
              ' "metadata": {\n' + \
              '  "kernelspec": {\n' + \
              '   "display_name": "C",\n' + \
              '   "language": "c",\n' + \
              '   "name": "c"\n' + \
              '  },\n' + \
              '  "language_info": {\n' + \
              '   "file_extension": ".c",\n' + \
              '   "mimetype": "text/plain",\n' + \
              '   "name": "c"\n' + \
              '  }\n' + \
              ' },\n' + \
              ' "nbformat": 4,\n' + \
              ' "nbformat_minor": 2\n' + \
              '}'
# Also, markdown and code cells are declarated with certain start mark and end mark
# This is start mark for Markdown cells
MB = '  {\n' + \
     '   "cell_type": "markdown",\n' + \
     '   "metadata": {},\n' + \
     '   "source": [\n' + \
     '    "'
# this is start mark for code cells
CB = '  {\n' + \
     '   "cell_type": "code",\n' + \
     '   "execution_count": null,\n' + \
     '   "metadata": {},\n' + \
     '   "outputs": [],\n' + \
     '   "source": [\n' + \
     '    "'

# plus, the end marks may vary, for if it is the last one in the cells block or not
# There is a "," if the cell is not the last one
E = '"\n   ]\n  },\n'
E2 = '"\n   ]\n  }\n'

# Do some changes to the .tex files
# mainly changing some .tex symbols to .ipynb symbols, or simple get rid of them

# ilegal symbols
def replace_underscore(Str):
    Str = re.sub('"', '\'', Str)
    Str = re.sub('\\\\_', '_', Str)
    Str = re.sub('\\ifpdf', '', Str)
    Str = re.sub('\\item ', '(item)', Str)
    Str = re.sub('\\fi', '', Str)
    Str = re.sub('\\fortranspecificstart', '', Str)
    Str = re.sub('\\fortranspecificend', '', Str)
    return Str

# dash, "\\\\\"
def replace_dash(Str):
    Str = re.sub('\\\\', '', Str)
    return Str

# pattern
def replce_pattern(Str):
    if '[htbp]' in Str:
        Str = ''
    
    if ' {figs or' in Str:
        Str = ''

    return Str

# \pagebreak
def replace_page(Str):
    Str = re.sub(r'\\pagebreak', '', Str)
    return Str

# \label
def replace_label(Str):
    Str = re.sub(r'\\label\{[^\{\}]*\}', '', Str)
    Str = re.sub(r'\\specref\{[^\{\}]*\}', '', Str)
    return Str

# \chapter
def rep_chapter(match):
    val = match.group()
    l = len(val)
    out = ' ## ' + val[9 :(l - 1)]
    return out

def replace_chapter(Str):
    Str = re.sub(r'\\chapter\{[^\{\}]*\}', rep_chapter, Str)
    return Str

# \section
def rep_section(match):
    val = match.group()
    l = len(val)
    out = ' ### ' + val[9 :(l - 1)]
    return out

def replace_section(Str):
    Str = re.sub(r'\\section\{[^\{\}]*\}', rep_section, Str)
    return Str

# \subsection
def rep_subsec(match):
    val = match.group()
    l = len(val)
    out = ' #### ' + val[12 :(l - 1)]
    return out

def replace_subsec(Str):
    Str = re.sub(r'\\subsection\{[^\{\}]*\}', rep_subsec, Str)
    return Str

# %
def replace_perc(Str):
    Str = re.sub(r'^[][%]', '', Str)
    Str = re.sub(r'[%]', '\\\\n",\n"', Str)
    Str = re.sub(r'begin\{[\w]*\}', '', Str)
    Str = re.sub(r'end\{[\w]*\}', '', Str)
    return Str

# (item)
def replace_item(Str):
    Str = re.sub(r'\(item\)', '\\\\n",\n"* ', Str)
    return Str

# \code
def rep_code(match):
    val = match.group()
    l = len(val)
    out = ' `' + val[6 :(l - 1)] + '` '
    return out

def replace_code(Str):
    Str = re.sub(r'\\code\{[^\{\}]*\}', rep_code, Str)
    if '\\code{' in Str:
        Str = ' `' + Str[7 :(l - 1)] + '` '
    return Str

# \plc
def rep_plc(match):
    val = match.group()
    l = len(val)
    out = ' _' + val[5 :(l - 1)] + '_ '
    return out

def replace_plc(Str):
    Str = re.sub(r'\\plc\{[^\{\}]*\}', rep_plc, Str)
    return Str

# \texttt
def rep_texttt(match):
    val = match.group()
    l = len(val)
    out = ' ' + val[8 :(l - 1)] + ' '
    return out

def replace_texttt(Str):
    Str = re.sub(r'\\texttt\{[\w\W\s]\}', rep_texttt, Str)
    return Str

# \href
def rep_href(match):
    val = match.group()
    l = len(val)
    out = ' ' + val[6 :(l - 1)] + ' '
    return out

def replace_href(Str):
    Str = re.sub(r'\\href\{[^\{\}]*\}', rep_href, Str)
    Str = re.sub(r'\{https:[^\{\}]*\}.', '', Str)
    return Str

# examples
# There are 7 different kinds of examples that marked with 7 flags, such as "cexample"
# This part is to get the file name of source code for them
# For instance, the flag in .tex file is {cexample}{xxxx1}
# The source code name should be "Example_xxxx.1.c"
def replace_example(Str):
    Str = re.sub('{', '', Str)
    Str = re.sub('}', '', Str)
    l = len(Str)
    if 'cexample' in Str:
        if Str[l-2] not in ['1', '2', '3', '4']:
            Str = Str[8 : l-1] + '.' + Str[l-1] + '.c'
        else:
            Str = Str[8 : l-2] + '.' + Str[l-2 :] + '.c'

    elif 'fexample' in Str:
        # Str = Str[8 : ] + '.f'
        if Str[l-2] not in ['1', '2', '3', '4']:
            Str = Str[8 : l-1] + '.' + Str[l-1] + '.f'
        else:
            Str = Str[8 : l-2] + '.' + Str[l-2 :] + '.f'

    elif 'cppexample' in Str:
        # Str = Str[10 : ] + '.cpp'
        if Str[l-2] not in ['1', '2', '3', '4']:
            Str = Str[10 : l-1] + '.' + Str[l-1] + '.cpp'
        else:
            Str = Str[10 : l-2] + '.' + Str[l-2 :] + '.cpp'

    elif 'cnexample' in Str:
        # Str = Str[9 : ] + '.c'
        if Str[l-2] not in ['1', '2', '3', '4']:
            Str = Str[9 : l-1] + '.' + Str[l-1] + '.c'
        else:
            Str = Str[9 : l-2] + '.' + Str[l-2 :] + '.c'

    elif 'fnexample' in Str:
        # Str = Str[9 : ] + '.f'
        if Str[l-2] not in ['1', '2', '3', '4']:
            Str = Str[9 : l-1] + '.' + Str[l-1] + '.f'
        else:
            Str = Str[9 : l-2] + '.' + Str[l-2 :] + '.f'

    elif 'cfreeexample' in Str:
        # Str = Str[12 : ] + '.c'
        if Str[l-2] not in ['1', '2', '3', '4']:
            Str = Str[12 : l-1] + '.' + Str[l-1] + '.c'
        else:
            Str = Str[12 : l-2] + '.' + Str[l-2 :] + '.c'

    elif 'ffreeexample' in Str:
        # Str = Str[12 : ] + '.f90'
        if Str[l-2] not in ['1', '2', '3', '4']:
            Str = Str[12 : l-1] + '.' + Str[l-1] + '.f90'
        else:
            Str = Str[12 : l-2] + '.' + Str[l-2 :] + '.f90'

    return '%load ../sources/Example_' + Str

# get file list
# The file list is a temprary .txt file that records the names of .tex files to be processed
# This list will be read line by line in terms of getting access to the .tex files.
# The .tex files are located in the higher level of folder
path = '../'
dir = os.listdir(path)

with open('MyList.txt', 'w') as f:
    for i in dir:
        if os.path.splitext(i)[1] == '.tex':
            f.write(i)
            f.write('\n')

with open('MyList.txt', 'r') as f:
     mylist = f.read()

mylist = mylist[: -1]
mylists = re.split('\n', mylist)
# print mylists

# check grammar
LIST = ['\\pa', '\\ch', '\\se', '\\su', '\\la', '\\ce', '\\cn', '\\cp', '\\fe', '\\fn', '\\ff']

CodeList = ['ce', 'cn', 'cp', 'fe', 'fn', 'ff']

# test files are created, which are intermediate files that awaiting for futher translation
for FileName in mylists:
    print(FileName)
    FileLen = len(FileName)
    input = '../' + FileName
    output = FileName[:(FileLen - 4)] + '.ipynb'
    testfile = 'TEST' + FileName

    with open(input, 'r') as f:
        s = f.read()

    lines = re.split('\n', s)

    with open(testfile, 'w') as f:
        for line in lines:
            l = len(line)
            if line[ : 3] in LIST:
                f.write(line)
                f.write('\n')
            elif l == 0:
                f.write('\n')
            else :
                f.write(' ')
                f.write(line)

# read and write
# read the test files and write the results
    with open(testfile, 'r') as f:
        s = f.read()

    lines = re.split('\n', s)

    with open(output, 'w') as f:
        
        f.write('{\n "cells": [\n')
        
        for line in lines:
            line = replace_underscore(line)
            line = replace_code(line)
            line = replace_plc(line)
            line = replace_texttt(line)
            line = replace_href(line)
            
            line = replce_pattern(line)
            
            line = replace_page(line)
            line = replace_chapter(line)
            line = replace_section(line)
            line = replace_subsec(line)
            line = replace_label(line)
            
            line = replace_dash(line)
            if len(line) == 0:
                f.write('')
            elif line[ : 2] in CodeList:
                line = replace_example(line)
                f.write(CB)
                f.write(line)
                f.write(E)
            else :
                line = replace_perc(line)
                line = replace_item(line)
                f.write(MB)
                f.write(line)
                f.write(E)

        f.write(MB)
        f.write('---end---')
        f.write(E2)

        f.write(LanguageSet)
# get rid of the test files
        os.remove(testfile)

# finally get rid of the file list
os.remove('MyList.txt')

