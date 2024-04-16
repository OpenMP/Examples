# Contributing

The usual process for adding new examples, making changes or adding corrections 
is to submit an issue for discussion and initial evaluation of changes or example additions. 
When there is a consensus at a meeting about the contribution, 
the issue will be brought forward for voting at the OpenMP Language
Committee meetings and you will be asked to submit a pull request.

Of course, if your contribution is an obvious correction, clarification, or note, you
may want to submit a pull request directly.

-----------------------------------------------------------

## The OpenMP Examples document

The OpenMP Examples document is in LaTeX format.
Please see the main LaTeX file, `openmp-examples.tex`, for more information.

## Maintainer

[OpenMP Examples Subcommittee](http://twiki.openmp.org/twiki/bin/view/OpenMPLang/OpenMPExamplesSubCommittee)
For a brief revision history, see `Changes.log` in the repo.

## Git procedure

 * Fork your own branch of the OpenMP [examples-internal repo](https://github.com/OpenMP/examples-internal)
 * Clone your fork locally
 * If you are working on generic or old-version updates, create a branch off main.
 * If you are working on an example for a release candidate for version #.#, create a branch off work_#.#.
   1) `git clone --branch <main|work_#.#> https://github.com/<my_account>/examples-internal`
   2) `git checkout -b <branch_name>`
   3) ...  `add`, `commit`
   4) `git push -u origin <branch_name>`
   5) `make` or `make diff` will create a full-document pdf or just a pdf with differences (do this at any point).
 * `git status` and `git branch -a` are your friends
 * Submit an issue for your work (usually with a diff pdf), and then you will be asked to submit a pull request
   * Create an issue by selecting the (issue tab)[https://github.com/OpenMP/examples-internal/issues] and clicking on `new issue`.
   * Use this MarkDown Cheatsheet for (issue formatting)[https://wordpress.com/support/markdown-quick-reference/]
   * More MarkDown details are available (here)[https://markdown-it.github.io]
   * You can cut and paste markdown formatted text in a (reader)[https://dillinger.io] to see formatting effects.
   * Forced spaces are available in Markdown.  On a Mac it is "option+space".
   * Polling is available.  Go to (gh-poll)[https://app.gh-polls.com/].  Type an option on each line, then click `copy markdown`, and paste the contents into the issue.  (Use preview to check your poll, and then submit it.)
 * Create a pull request


## Processing source code

   * Prepare source code (C/C++ and Fortran) and a text description (use similar styles found in recent examples)
   * Determine the *example* name `<ename>`, *sequence* identifier `<seq-id>` and *compiler* suffix `<csuffix>` for the example
      * The syntax is:   `<ename>.<seq-id>.<csuffix>`   (e.g. `affinity_display.1.f90`)
      * The example name may be a Section name (e.g. affinity), or a Subsection name (affinity_display)
      * If you are creating a new Chapter, it may be the chapter name.
   * New examples are usually added at the end of a Section or Subsection. Number it as the next number in the sequence numbers for examples in that Section or Subsection.
   * The compiler suffix `<csuffix>` is `c`, `cpp`, `f`, and `f90` for C, C++ and Fortran (fixed/free form) codes.
   * Insert the code in the sources directory for each chapter, and include the following metadata:
   * Metadata Tags for example sources:
     ```
       @@name:        <ename>.<seq-no>
       @@type:        C|C++|F-fixed|F-free
       @@operation:   view|compile|link|run
       @@expect:      success|ct-error|rt-error|unspecified
       @@version:     [pre_]omp_<verno>
       @@env:         <environment_variables>
       @@depend:      <source_code_name>
     ```
     * **name**
       - is the name of an example
     * **type**
       - is the source code type, which can be translated into or from proper file extension (C:c,C++:cpp,F-fixed:f,F-free:f90)
     * **operation**
       - indicates how the source code is treated. Possible values are:
         - `view`     - code for illustration only, not compilable;
         - `compile`  - incomplete program, such as function or subroutine;
         - `link`     - complete program, but no verification value;
         - `run`      - complete program with verification value.
     * **expect**
       - indicates some expected result for testing purpose. 
         - `success` means no issue;
         - `ct-error` applies to the result of code compilation;
         - `rt-error` is for a case where compilation may be successful, but the code
       contains potential runtime issues (including race condition); 
         - `unspecified` could result from a non-conforming code or is for code
       that is viewable only.
     * **version**
       - indicates that the example uses features in a specific OpenMP version, such as "`omp_5.0`".
       The prefix `pre_` indicates that the example uses features prior to a specific version, such as "`pre_omp_3.0`".
     * **env**
       - specifies any environment variables needed to run the code.
       This tag is optional and can be repeated.
     * **depend**
       - specifies a source code file on which the current code depends.
       This tag is optional and can be repeated.
     * For **env** and **depend**, make sure to specify
       a proper skipping number `<s>` in the LaTeX macros described below
       to match with the number of `env` and `depend` tags.


## Process for text
   * Create or update the description text in a Section/Subsection file under each chapter directory, usually `<chap_directory>/<ename>.tex`
   * If adding a new Subsection, just include it in the appropriate subsection file (`<subsection>.tex`)
   * If adding a new Section, create an `<section>.tex` file and add an entry in the corresponding chapter file, such as `Chap_affinity.tex`
   * If adding a new Chapter, create a `Chap_<chap_name>.tex` file with introductory text, and add a new `<section>.tex` file with text and links to the code. Update `Makefile` and `openmp-examples.tex` to include the new chapter file.
   * Commit your changes into your fork of examples-internal
   * Summit your issue at [OpenMP Examples internal repo]( https://github.com/openmp/examples-internal/issues), and include a PDF when ready.
   * Examples subcommittee members can view [meeting schedule and notes](http://twiki.openmp.org/twiki/bin/view/OpenMPLang/ExamplesSchedules)
   * Shepherd your issue to acceptance (discussed at weekly Examples meeting and in issue comments)
   * When it is in a ready state, you should then submit a pull request.
   * It will be reviewed and voted on, and changes will be requested.
   * Once the last changes are made, it will be verified and merged into an appropriate branch (either the `main` branch or a working branch).




## LaTeX macros for examples

The following describes LaTeX macros defined specifically for examples.
* Source code with language h-rules
* Source code without language h-rules
* Language h-rules
* Macros for keywords in text description
* Other macros
* See `openmp.sty` for more information

### Source code with language h-rules
```
   \cexample[<verno>]{<ename>}{<seq-no>}[<s>]     % for C/C++ examples
   \cppexample[<verno>]{<ename>}{<seq-no>}[<s>]   % for C++ examples
   \fexample[<verno>]{<ename>}{<seq-no>}[<s>]     % for fixed-form Fortran examples
   \ffreeexample[<verno>]{<ename>}{<seq-no>}[<s>] % for free-form Fortran examples
```

### Source code without language h-rules
```
   \cnexample[<verno>]{<ename>}{<seq-no>}[<s>]
   \cppnexample[<verno>]{<ename>}{<seq-no>}[<s>]
   \fnexample[<verno>]{<ename>}{<seq-no>}[<s>]
   \ffreenexample[<verno>]{<ename>}{<seq-no>}[<s>]
   \srcnexample[<verno>]{<ename>}{<seq-no>}{<ext>}[<s>]
```

   Optional `<verno>` can be supplied in a macro to include a specific OpenMP
   version in the example header.  This option also suggests one additional
   tag (`@@version`) line is included in the corresponding source code. 
   If this is not the case (i.e., no `@@version` tag line), one needs to 
   prefix `<verno>` with an underscore '\_' symbol in the macro.

   The exception is macro `\srcnexample`, for which the corresponding
   source code might not contain any `@@` metadata tags. The `ext` argument
   to this macro is the file extension (such as `h`, `hpp`, `inc`).

   The `<s>` option to each macro allows finer-control of any additional lines
   to be skipped due to addition of new `@@` tags, such as `@@env`.
   The default value for `<s>` is 0.

### Language h-rules
```
   \cspecificstart, \cspecificend
   \cppspecificstart, \cppspecificend
   \ccppspecificstart, \ccppspecificend
   \fortranspecificstart, \fortranspecificend
   \topmarker{Lang}
```

   The macro `\topmarker` puts a dashed blue line floater at top of a page for 
   "Lang (cont.)" where `Lang` can be `C/C++`, `C++`, `Fortran`.

### Macros for keywords in text description
A partial list:
- `\kcode{}` - for OpenMP keywords, such as directives, clauses, environment variables, API routines. Support direct use of '_' (underscore) and ' ' (space)
- `\scode{}` - OpenMP specifier with special chars, such as '`$`' in "`!$omp`"
- `\bcode{}` - base language keywords (such as `ASSOCIATE` in Fortran)
- `\vcode{}` - values of a keyword, such as `TRUE`, `FALSE`, `VERBOSE`
- `\plc{}` - OpenMP concept, such ICV names; `\splc{}` - escape '_' (underscore)
- `\example{}` - example names, such as `\example{taskloop_reduction.1}`
- `\docref{}` - chapter or section name of a document, such as the spec
- `\ucode{}` - program variables, procedure names, or expression in examples codes. Support direct use of '_' (underscore) and ' ' (space).
- `\pout{}` - program outputs

Examples:
- `\kcode{declare reduction}` for **declare reduction**
- `\scode{!$omp}` sentinel, however, `\kcode{\#pragma omp}`
- `\kcode{map(iterator(\ucode{i=0:n}), tofrom: \ucode{p[i]})}` for **map(iterator(**_i=0:n_**), tofrom:** _p[i]_**)**
- Fortran `\bcode{IMPLICIT NONE}` statement
- The `\vcode{VERBOSE}` value for `\kcode{OMP_DISPLAY_ENV}`
- OpenMP `\plc{directives}`, the `\plc{num-threads}` ICV
- This is an example name `\example{taskloop_reduction.1}`
- `(\ucode{x,y,z})` argument for procedure `\ucode{a_proc_name}`
- structure constructor `\ucode{point($\ldots$)}`
- This is a code output `"\pout{x = 1}"`

### Other macros
```
   \cchapter{<Chapter Name>}{<chap_directory>}
   \hexentry[ext1]{<example_name>}[ext2]{<earlier_tag>}
   \hexmentry[ext1]{<example_name>}[ext2]{<earlier_tag>}{<prior_name>}
   \examplesref{<verno>}
   \examplesblob{<verno/file>}
```

The `\cchapter` macro is used for starting a chapter with proper page spacing.
`<Chapter Name>` is the name of a chapter and `<chap_directory>` is the name 
of the chapter directory.  All section and subsection files for the chapter 
should be placed under `<chap_directory>`. The corresponding example sources 
should be placed under the `sources` directory inside `<chap_directory>`.

A previously-defined macro `\sinput{<section_file>}` to import a section
file from `<chap_directory>` is no longer supported.  Please use
`\input{<chap_directory>/<section_file>}` explicitly.

The two macros `\hexentry` and `\hexmentry` are defined for simplifying
entries in the feature deprecation and update tables. Option `[ext1]` is
the file extension with a default value of `c` and option `[ext2]` is 
the file extension for the associated second file if present.
`<earlier_tag>` is the version tag of the corresponding example
in the earlier version. `\hexentry` assumes no name change for an example
in different versions; `\hexmentry` can be used to specify a prior name
if it is different.

The two macros `\examplesref` and `\examplesblob` are for referencing
a specific version of or a file in the github Examples repository.

## License

For copyright information, please see [omp_copyright.txt](omp_copyright.txt).
