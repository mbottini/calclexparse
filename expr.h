#ifndef EXPR_H
#define EXPR_H

#include "tokens.h"
#include <unordered_map>
#include <unordered_set>
#include <utility>

#define INDENT_LEVEL 4

enum EVAL_RESULT {
    FAILURE,
    SUCCESS,
    ASSIGN,
    CYCLE
};

class Expr;
typedef std::shared_ptr<Expr> Expr_ptr;
typedef std::unordered_map<std::string, Expr_ptr> expr_map;
typedef std::vector<std::unique_ptr<Token>> token_vec;
typedef std::pair<Expr_ptr, size_t> parse_result;
typedef std::pair<EVAL_RESULT, int> eval_pair;
typedef std::unordered_set<std::string> var_set;

class Expr {
    public:
        virtual eval_pair 
            eval(expr_map& map) = 0;
        virtual std::ostream& print(std::ostream& os, int indent = 0) const = 0;
        virtual bool has_cycle(expr_map& map, var_set& set) const = 0;
        ~Expr() {}
};

class Const : public Expr {
    private:
        int _data;
    public:
        Const(int i) {
            _data = i;
        }
        eval_pair eval(expr_map& map);
        std::ostream& print(std::ostream& os, int indent) const;
        bool has_cycle(expr_map& map, var_set& set) const { return false; }
};

class Var : public Expr {
    private:
        std::string _data;
    public:
        Var(const std::string& s) {
            _data = s;
        }
        eval_pair eval(expr_map& map);
        std::ostream& print(std::ostream& os, int indent) const;
        bool has_cycle(expr_map& map, var_set& set) const {
            if(set.find(_data) != set.end()) {
                return true;
            }

            set.insert(_data);

            auto it = map.find(_data);
            if(it != map.end()) {
                return it->second->has_cycle(map, set);
            }

            return false;
        }
};

class Plus : public Expr {
    private:
        Expr_ptr _e1, _e2;
    public:
        Plus(Expr_ptr &e1, Expr_ptr &e2) {
            _e1 = e1;
            _e2 = e2;
        }
        Plus(Expr_ptr &&e1, Expr_ptr &&e2) {
            _e1 = e1;
            _e2 = e2;
        }
        eval_pair eval(expr_map& map);
        std::ostream& print(std::ostream& os, int indent) const;
        bool has_cycle(expr_map& map, var_set& set) const {
            return _e1->has_cycle(map, set) || _e2->has_cycle(map, set);
        }
};

class Minus : public Expr {
    private:
        Expr_ptr _e1, _e2;
    public:
        Minus(Expr_ptr &e1, Expr_ptr &e2) {
            _e1 = e1;
            _e2 = e2;
        }
        Minus(Expr_ptr &&e1, Expr_ptr &&e2) {
            _e1 = e1;
            _e2 = e2;
        }
        eval_pair eval(expr_map& map);
        std::ostream& print(std::ostream& os, int indent) const;
        bool has_cycle(expr_map& map, var_set& set) const {
            return _e1->has_cycle(map, set) || _e2->has_cycle(map, set);
        }
};

class Times : public Expr {
    private:
        Expr_ptr _e1, _e2;
    public:
        Times(Expr_ptr &e1, Expr_ptr &e2) {
            _e1 = e1;
            _e2 = e2;
        }
        Times(Expr_ptr &&e1, Expr_ptr &&e2) {
            _e1 = e1;
            _e2 = e2;
        }
        eval_pair eval(expr_map& map);
        std::ostream& print(std::ostream& os, int indent) const;
        bool has_cycle(expr_map& map, var_set& set) const {
            return _e1->has_cycle(map, set) || _e2->has_cycle(map, set);
        }
};

class Div : public Expr {
    private:
        Expr_ptr _e1, _e2;
    public:
        Div(Expr_ptr &e1, Expr_ptr &e2) {
            _e1 = e1;
            _e2 = e2;
        }

        Div(Expr_ptr &&e1, Expr_ptr &&e2) {
            _e1 = e1;
            _e2 = e2;
        }
        eval_pair eval(expr_map& map);
        std::ostream& print(std::ostream& os, int indent) const;
        bool has_cycle(expr_map& map, var_set& set) const {
            return _e1->has_cycle(map, set) || _e2->has_cycle(map, set);
        }
};

class Exp : public Expr {
    private:
        Expr_ptr _e1, _e2;
    public:
        Exp(Expr_ptr &e1, Expr_ptr &e2) {
            _e1 = e1;
            _e2 = e2;
        }
        eval_pair eval(expr_map& map);
        std::ostream& print(std::ostream& os, int indent) const;
        bool has_cycle(expr_map& map, var_set& set) const {
            return _e1->has_cycle(map, set) || _e2->has_cycle(map, set);
        }
};

class UMinus : public Expr {
    private:
        Expr_ptr _e;
    public:
        UMinus(Expr_ptr &e) {
            _e = e;
        }
        eval_pair eval(expr_map& map);
        std::ostream& print(std::ostream& os, int indent) const;
        bool has_cycle(expr_map& map, var_set& set) const {
            return _e->has_cycle(map, set);
        }
};

class UPlus : public Expr {
    private:
        Expr_ptr _e;
    public:
        UPlus(Expr_ptr &e) {
            _e = e;
        }
        eval_pair eval(expr_map& map);
        std::ostream& print(std::ostream& os, int indent) const;
        bool has_cycle(expr_map& map, var_set& set) const {
            return _e->has_cycle(map, set);
        }
};

class Assign : public Expr {
    private:
        std::string _var;
        Expr_ptr _e;
    public:
        Assign(Token *t, Expr_ptr &e) {
            if(t->is_var()) {
                _var = static_cast<TVar*>(t)->get_val();
                _e = e;
            }
            else {
                throw "MASSIVE FAILURE - non-var assigned to!";
            }
        }
        eval_pair eval(expr_map& map);
        std::ostream& print(std::ostream& os, int indent) const;
        bool has_cycle(expr_map& map, var_set& set) const {
            if(set.find(_var) != set.end()) {
                return true;
            }
            set.insert(_var);
            return _e->has_cycle(map, set);
        }
};

std::vector<Expr_ptr> parseTokens(const token_vec& t_vec);

/*
A is Assignment.
A -> Var = S | S

S is the Start of an Expression.
Note that we've removed all left-recursive aspects of the grammar.

S -> T S2
S2 -> + T S2 | - T S2 | epsilon
T -> U T2
T2 -> * U T2 | / U T2 | epsilon
U -> W ^ U | W
W -> -W | +W | X
X -> (S) -> Term
*/

parse_result parseA(const token_vec& t_vec, size_t current_index);

parse_result parseS(const token_vec& t_vec, size_t current_index);

parse_result parseS2(const token_vec& t_vec, size_t current_index, Expr_ptr&& e);

parse_result parseT(const token_vec& t_vec, size_t current_index);

parse_result parseT2(const token_vec& t_vec, size_t current_index, Expr_ptr&& e);

parse_result parseU(const token_vec& t_vec, size_t current_index);

parse_result parseW(const token_vec& t_vec, size_t current_index);

parse_result parseX(const token_vec& t_vec, size_t current_index);


// Printing helper.

std::ostream& print_spaces(std::ostream& os, int n);
std::ostream& operator <<(std::ostream& os, const eval_pair &e);

#endif
