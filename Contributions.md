# Contributing

The usual process for adding new examples, making changes or adding corrections 
is to submit an issue for discussion and initial evaluation of changes or example additions. 
When there is a consensus at a meeting about the contribution, 
you will be asked to submit a pull request.

Of course, if your contribution is an obvious correction, clarification, or note, you
may want to submit a pull request directly.

-----------------------------------------------------------

## The OpenMP Examples document

The OpenMP Examples document is in LaTeX format.
Please see the master LaTeX file, `openmp-examples.tex`, for more information.

## Maintainer

[OpenMP Examples Subcommittee](http://twiki.openmp.org/twiki/bin/view/OpenMPLang/OpenMPExamplesSubCommittee)
For a brief revision history, see `Changes.log` in the repo.

## Git procedure

 * Fork your own branch of the OpenMP [examples-internal repo](https:/github.com/openmp/examples-internal)
 * Clone your fork locally
 * If you are working on generic or old-version updates, create a branch off master.
 * If you are working on an example for a release candidate for version #.#, create a branch off work_#.#.
   1.) `git clone --branch <master|work_#.#> https://github.com/<my_account>/examples-internal`
   2.) `git checkout  -b <branch_name>`
   3.) ...  `add`, `commit`
   4.) `git push -u origin <branch_name>`
   5.) `make` or `make diff` will create a full-document pdf or just a pdf with differences (do this at any point).
 * `git status` and `git branch -a` are your friends
 * Submit an issue for your work (usually with a diff pdf), and then you will be asked to submit a pull request
   * Create an issue by selecting the (issue tab)[https://github.com/openmp/examples-internal/issues] and clicking on `new issue`.
   * Use this MarkDown Cheatsheet for (issue formatting)[https://wordpress.com/support/markdown-quick-reference/]
   * More MarkDown details are available (here)[https://markdown-it.github.io]
   * You can cut and paste markdown formatted text in a (reader)[https://dillinger.io] to see formatting effects.
   * Forced spaces are available in Markdown.  On a Mac is is "option+space".
   * Polling is available.  Go to (gh-poll)[https://app.gh-polls.com/].  Type an option on each line, then click `copy markdown`, and paste the contents into the issue.  (Use preview to check your poll, and then submit it.)
 * Create a pull request


## Processing source code

   * Prepare source code (C/C++ and Fortran) and a text description (use similar styles found in recent examples)
   * Determine the *example* name `<ename>`, *sequence* number `<seq-no>` and *compiler* suffix `<csuffix>` for the example
      * The syntax is:   `<ename>.<seq-no>.<csuffix>`   (e.g. `affinity_display.1.f90`)
      * The example name may be a Section name (e.g. affinity), or a Subsection name (affinity_display)
      * If you are creating a new Chapter, it may be the chapter name.
   * New examples are usually added at the end of a Section or Subsection. Number it as the next number in the sequence numbers for examples in that Section or Subsection.
   * The compiler suffix `<csuffix>` is `c`, `cpp`, `f`, and `f90` for C, C++ and Fortran codes.
   * Insert the code in the sources directory for each chapter, and include the following metadata:
   * Metadata Tags for example sources:
     ```
       @@name:        <ename>.<seq-no>
       @@type:        C|C++|F-fixed|F-free
       @@requires:    preprocessing
       @@compilable:  yes|no|maybe
       @@linkable:    yes|no|maybe
       @@expect:      success|compile-time-error|runtime-error|undefined-behavior
       @@version:     omp_<verno>
     ```
      * **name**
       is the name of an example
      * **type**
       is the source code type, which can be translated into or from proper file extension (C:c,C++:cpp,F-fixed:f,F-free:f90)
      * **requires**
       any additional requirements, currently `preprocessing` for requiring preprocessing
      * **compilable**
       indicates whether the source code is compilable
      * **linkable**
       indicates whether the source code is linkable
      * **expect**
       indicates some expected result for testing purpose "`success|compile-time-error|ct-error`" applies 
       to the result of code compilation; "`runtime-error|rt-error`" is for a case where compilation may be
       successful, but the code contains potential runtime issues (such as race condition); `undefined-behavior` could result from a non-conforming code.
       Alternative would be to just use "`conforming`" or "`non-conforming`".
      * **version**
       indicates features for a specific OpenMP version, such as "`omp_5.0`"


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
   * Once the last changes are made, it will be verified and merged into an appropriate branch (either the `master` branch or a working branch).




## LaTeX macros for examples

The following describes LaTeX macros defined specifically for examples.
* Source code with language h-rules
* Source code without language h-rules
* Language h-rules
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
   source code should not contain any `@@` metadata tags. The `ext` argument
   to this macro is the file extension (such as `h`, `hpp`, `inc`).

   The `<s>` option to each macro allows finer-control of any additional lines
   to be skipped due to addition of new `@@` tags, such as `@@requires`.
   The default value for `<s>` is 0.

### Language h-rules
```
   \cspecificstart, \cspecificend
   \cppspecificstart, \cppspecificend
   \ccppspecificstart, \ccppspecificend
   \fortranspecificstart, \fortranspecificend
```

### Other macros
```
   \cchapter{<Chapter Name>}{<chap_directory>}
   \hexentry[ext1]{<example_name>}[ext2]{<earlier_tag>}
   \hexmentry[ext1]{<example_name>}[ext2]{<earlier_tag>}{<prior_name>}
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

## License

For copyright information, please see `omp_copyright.txt`.
