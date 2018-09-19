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

**BUG:** Right now, circular assignment is not detected. Doing the following is
syntactically valid, and evaluating it will be Very Bad. do not do that please
thank you

    > x = 1; y = x; x = y; x
    (Segmentation fault ayyy lmao)

Assignments evaluate to the expression that the variable is assigned to.
