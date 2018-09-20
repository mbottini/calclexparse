# Calc Lexparser

True to its name, this is a lexer and parser for the "calc" programming
language. The calc programming language supports the following features:

* Arbitary evaluation of arithmetic expressions.

* Assignment of variables to said arithmetic expressions.

* Printing of the evaluation of the expressions.

# Constants

Constants evaluate to themselves. For example:

    > 1
    1
    > 1 + 2
    3

# Assignment

Variables can be assigned to expressions. Note that variables can be assigned to
other variables. If a variable's value changes, expressions that depend on the
value will change.

**TODO:** Change CFG to allow assignment inside expressions. It should be
possible to do

    x = y = 1

Note that circular assignment is not allowed. Doing the following will result in
a `CYCLE` evaluation.

    > x = 1
    ASSIGN 1
    > y = x
    ASSIGN 1
    > x = y
    CYCLE

