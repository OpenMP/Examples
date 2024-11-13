### OpenMP Examples Coding Style Guide

Must Dos:
- Indents and Braces
  - Code: Follow common base language practices.
    - Where indents are normally used, use 2 spaces instead of tabs.
  - Comments: Follow the indent of the base language for which the comment applies.
  - OpenMP directives should be indented as if it's base language code where possible.
  - Braces `{}` around structured blocks following directives must be on a new line and must follow base language indent.
  - For C/C++ examples, for code blocks with multiple lines, the if-else statements must follow the following format:
      ```
      if {
      } else {
      }
      ```
- All Section and sub-section headings must be in Title case. For example: " This is a Useful Example of X Directive ".

- Comments
  - Comments go on a new line before the relevant code/code block.
    - Expected results may go on the same line.
  - Keep comments terse; detailed explanations go in the text.

- Output
  - If there is a deterministic output, provide it.
  - It can be done in one of the following ways:
    - Specify the correct value in a comment.
    - Code prints out "expected" and "run" values.
    - Test for the correctness of a value in a conditional.
  - If the test is expected to execute, return values must be used to indicate success or failure.
    - For tests that produce incorrect results, use:
      - `return(1)` for C/C++
      - `stop 1` for Fortran (do not exit)
    - For tests that need to discontinue execution, use:
      - `exit(1)` for C/C++
      - `error stop` for Fortran
    - Validation messages such as "Pass" / "Fail" are not mandatory.
  - A single "pass" or "fail" is sufficient for a multi-case test.

- To Verify Metadata:
  - A tool in the repository at the top level, "make check", scans all sources for version tags and ensures line length is 75 characters max.
  - Inside `utils`, there is `chk_tags` (see different options) that can accept 1 file and scan for all specified values.

Don’ts:
- Unless required by the feature, use free-format Fortran for new examples.
- Do not use all-caps for emphasis in the document.

