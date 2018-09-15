#ifndef EXPR_H
#define EXPR_H

#include "tokens.h"
#include <unordered_map>
#include <utility>

#define INDENT_LEVEL 4

typedef std::unordered_map<std::string, Expr> expr_map;

enum EVAL_RESULT {
    SUCCESS,
    ASSIGN,
    FAILURE
};

class Expr {
    public:
        virtual std::pair<EVAL_RESULT, int> 
            eval(expr_map& map = expr_map()) const = 0;
        virtual std::ostream& print(std::ostream& os, indent = 0) const = 0;
        ~Expr() {}
};

class Plus : public Expr {
    private:
        std::unique_ptr<Expr> _e1, _e2;
    public:
        Plus(std::unique_ptr<Expr> e1, e2) {
            _e1 = std::move(e1);
            _e2 = std::move(e2);
        }
        std::pair<EVAL_RESULT, int> eval(expr_map& map) const;
        std::ostream& print(std::ostream& os, indent) const;
};

class Minus : public Expr {
    private:
        std::unique_ptr<Expr> _e1, _e2;
    public:
        Minus(std::unique_ptr<Expr> &e1, &e2) {
            _e1 = std::move(e1);
            _e2 = std::move(e2);
        }
        std::pair<EVAL_RESULT, int> eval(expr_map& map) const;
        std::ostream& print(std::ostream& os, indent) const;
};

class Times : public Expr {
    private:
        std::unique_ptr<Expr> _e1, _e2;
    public:
        Times(std::unique_ptr<Expr> &e1, &e2) {
            _e1 = std::move(e1);
            _e2 = std::move(e2);
        }
        std::pair<EVAL_RESULT, int> eval(expr_map& map) const;
        std::ostream& print(std::ostream& os, indent) const;
};

class Div : public Expr {
    private:
        std::unique_ptr<Expr> _e1, _e2;
    public:
        Div(std::unique_ptr<Expr> &e1, &e2) {
            _e1 = std::move(e1);
            _e2 = std::move(e2);
        }
        std::pair<EVAL_RESULT, int> eval(expr_map& map) const;
        std::ostream& print(std::ostream& os, indent) const;
};

class Exp : public Expr {
    private:
        std::unique_ptr<Expr> _e1, _e2;
    public:
        Exp(std::unique_ptr<Expr> &e1, &e2) {
            _e1 = std::move(e1);
            _e2 = std::move(e2);
        }
        std::pair<EVAL_RESULT, int> eval(expr_map& map) const;
        std::ostream& print(std::ostream& os, indent) const;
};

class UMinus : public Expr {
    private:
        std::unique_ptr<Expr> _e;
    public:
        UMinus(std::unique_ptr<Expr> &e) {
            _e = std::move(e);
        }
        std::pair<EVAL_RESULT, int> eval(expr_map& map) const;
        std::ostream& print(std::ostream& os, indent) const;
};

class UPlus : public Expr {
    private:
        std::unique_ptr<Expr> _e;
    public:
        UPlus(std::unique_ptr<Expr> &e) {
            _e = std::move(e);
        }
        std::pair<EVAL_RESULT, int> eval(expr_map& map) const;
        std::ostream& print(std::ostream& os, indent) const;
};

class Assign : public Expr {
    private:
        std::string _var;
        std::unique_ptr<Expr> _e;
    public:
        Assign(const std::string& s, std::unique_ptr<Expr> &e) {
            _var = s;
            _e = std::move(e);
        }
        std::pair<EVAL_RESULT, int> eval(expr_map& map) const;
        std::ostream& print(std::ostream& os, indent) const;
};

std::vector<std::unique_ptr<Expr>> 
parseTokens(const std::vector<std::unique_ptr<Token>>& t_vec);

/*
Starting generator is E.
E -> S | A

A is Assignment.
A -> Var = S

S is the Start of an Expression.
Note that we've removed all left-recursive aspects of the grammar.

S -> TS'
S' -> + T S' | - T S' | epsilon
T -> UT'
T' -> * U T' | / U T' | epsilon
U -> W ^ U | W
W -> -W | +W | X
X -> (S) -> Term
*/



#endif
