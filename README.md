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

Note that circular assignment is not allowed. Doing the following will result in
a `CYCLE` evaluation.

    > x = 1
    ASSIGN 1
    > y = x
    ASSIGN 1
    > x = y
    CYCLE

# Main Program

The lexer is built to take lines of input from an `istream`. With the way the
`main` function is currently set up, `std::cin` is that `istream`. This means
that you need to terminate your input with a newline and an EOF (Ctrl+D on
Linux, Ctrl+Z on Windows). Example:

    x = 5+4;
    y = x^2;
    z = 3*(y-4) + 2^(25 - x);
    z;
    ^D

    // list of lexed tokens, used for debugging...

    (Assign x:
        (Plus
            (Const 5)
            (Const 4)))
    Expression 1: ASSIGN 9
    (Assign y:
        (Exp
            (Var x)
            (Const 2)))
    Expression 2: ASSIGN 81
    (Assign z:
        (Plus
            (Times
                (Const 3)
                (Minus
                    (Var y)
                    (Const 4)))
            (Exp
                (Const 2)
                (Minus
                    (Const 25)
                    (Var x)))))
    Expression 3: ASSIGN 65767
    (Var z)
    Expression 4: 65767
    
