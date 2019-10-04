#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-

import re
import os

# ipynb sets
with open('NBSets/LanguageSet.txt', 'r') as f:
    LanguageSet = f.read()

with open('NBSets/markdown.txt', 'r') as f:
    MB = f.read()

with open('NBSets/code.txt', 'r') as f:
    CB = f.read()

E = '"\n   ]\n  },\n'
E1 = ' \n   ]\n  },\n'
E2 = '"\n   ]\n  }\n'

# Do some changes
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

    return '%load ~/Documents/Examples/sources/Example_' + Str

# get file list
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
#print mylists

# check grammar
LIST = ['\\pa', '\\ch', '\\se', '\\su', '\\la', '\\ce', '\\cn', '\\cp', '\\fe', '\\fn', '\\ff']

CodeList = ['ce', 'cn', 'cp', 'fe', 'fn', 'ff']

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
