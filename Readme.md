# Overview
Implement some function of GNU bash(v3.2).

# How to use
### Install GNL readline
```shell
brew install readline
```
### Install minishell
```shell
git clone https://github.com/minishellakirawchen/minishell_rev1.git
cd minishell_rev1
make
./minishell
```

# Specification
### Mandatory Part
* Display a prompt when waiting for a new command.
* Have a working history.
* Search and launch the right executable (based on the `PATH` variable or using a relative or an absolute path).
* Not interpret unclosed quotes or special characters which are not required by the subject such as `\` (backslash).
* Handle `’` (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
* Handle `"` (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for `$` (dollar sign).
* Implement redirections:
  - `<` should redirect input
  - `>` should redirect output
  - `<<` should be given a delimiter as here-doc
  - `>>` should redirect output in append mode
* Implement operators.
  - pipe `|` : connect to the input of the next command.
* Handle environment variables (`$` followed by a sequence of characters) which should expand to their values.
* Handle `$?` which should expand to the exit status of the most recently executed foreground pipeline.
* Handle `ctrl-C`, `ctrl-D` and `ctrl-\` which should behave like in bash in interactive mode:
  - `ctrl-C` displays a new prompt on a new line.
  - `ctrl-D` exits the shell.
  - `ctrl-\` does nothing.
* Implement the following builtins:
  - `echo` with option `-n`
  - `cd` with only a relative or absolute path
  - `pwd` with no options
  - `export` with no options
  - `unset` with no options
  - `env` with no options or arguments
  - `exit` with no options

### Bonus Part
* Implement operators.
    - and `&&`
    - or `||`
    - subshell `(`, `)`
* Wildcards `*` should work for the current working directory.

### Plus alpha
* Implement operators.
    - semicolon `;`
    
    
# Release
* Feb/12th
