#include "expr.h"

/* Alright, this looks flagrantly terrible. Let's talk about some things.
   Our goal here is to change a big vector of Tokens into an Expression.
   To do this, we have a whole bunch of parsing functions that call each
   other in accordance with a context-free grammar.
   Each function returns a pair: the expression that it built, and
   the index of the next remaining token in the token vector.

   We have to be very careful with move semantics because everything
   is a unique_ptr. We could also do this with shared_ptrs, but that's
   pussying out. Go big or go home, baby.
*/

parse_result parseA(const token_vec& t_vec, size_t current_index) {
    // If the first two indices are a Var and an Equals, it's an assignment.
    if(t_vec.size() - current_index >= 2 &&
       t_vec[current_index]->is_var() && t_vec[current_index+1]->is_equals()) {
        auto pair = parseS(t_vec, current_index+2);
        return parse_result(Expr_ptr(
                             new Assign(t_vec[current_index].get(), pair.first)),
                             pair.second);
    }

    // Otherwise, we pass through to ParseS.
    return parseS(t_vec, current_index);
}

parse_result parseS(const token_vec& t_vec, size_t current_index) {
    // Note that S2 consumes a T.
    auto pair = parseT(t_vec, current_index);
    return parseS2(t_vec, pair.second, std::move(pair.first));
}

parse_result parseS2(const token_vec& t_vec, 
                     size_t current_index, 
                     Expr_ptr &&e) {
    if(t_vec.size() - current_index >= 1) {
        if(t_vec[current_index]->is_plus()) {
            auto pair = parseT(t_vec, current_index+1);
            return parseS2(t_vec, pair.second, Expr_ptr(new Plus(e, pair.first)));
        }

        if(t_vec[current_index]->is_minus()) {
            auto pair = parseT(t_vec, current_index+1);
            return parseS2(t_vec, pair.second, Expr_ptr(new Minus(e, pair.first)));
        }
    }

    return parse_result(e, current_index);
}

parse_result parseT(const token_vec& t_vec, size_t current_index) {
    // Note that T2 consumes a U.
    auto pair = parseU(t_vec, current_index);
    return parseT2(t_vec, pair.second, std::move(pair.first));
}

parse_result parseT2(const token_vec& t_vec,
                                    size_t current_index,
                                    Expr_ptr &&e) {
    if(t_vec.size() - current_index >= 1) {
        if(t_vec[current_index]->is_times()) {
            auto pair = parseU(t_vec, current_index+1);
            return parseT2(t_vec, pair.second, Expr_ptr(new Times(e, pair.first)));
        }

        if(t_vec[current_index]->is_div()) {
            auto pair = parseU(t_vec, current_index+1);
            return parseT2(t_vec, pair.second, Expr_ptr(new Div(e, pair.first)));
        }
    }

    return parse_result(e, current_index);
}

parse_result parseU(const token_vec& t_vec, size_t current_index) {
    auto pair = parseW(t_vec, current_index);
    if(t_vec.size() - pair.second >= 1 && t_vec[pair.second]->is_exp()) {
        auto pair2 = parseU(t_vec, pair.second + 1);
        return parse_result(Expr_ptr(new Exp(pair.first, pair2.first)), pair2.second);
    }
    return pair;
}

parse_result parseW(const token_vec& t_vec, size_t current_index) {
    if(t_vec.size() - current_index >= 1) {
        if(t_vec[current_index]->is_plus()) {
            auto pair = parseW(t_vec, current_index+1);
            return parse_result(Expr_ptr(new UPlus(pair.first)), pair.second);
        }

        if(t_vec[current_index]->is_minus()) {
            auto pair = parseW(t_vec, current_index+1);
            return parse_result(Expr_ptr(new UPlus(pair.first)), pair.second);
        }
    }

    return parseX(t_vec, current_index);
}

parse_result parseX(const token_vec& t_vec, size_t current_index) {
    if(t_vec.size() - current_index <= 0) {
        throw std::pair<size_t, std::string>(current_index, "Missing operand!");
    }

    if(t_vec[current_index]->is_lparen()) {
        auto pair = parseS(t_vec, current_index+1);
        if(t_vec.size() - pair.second <= 0 || 
           !t_vec[pair.second]->is_rparen()) {
            throw std::pair<size_t, std::string>(pair.second, "Mismatched parens!");
        }
        return parse_result(pair.first, pair.second+1);
    }

    if(t_vec[current_index]->is_var()) {
        return parse_result(
          Expr_ptr(
          new Var(static_cast<TVar*>(t_vec[current_index].get())->get_val())),
          current_index+1);
    }

    if(t_vec[current_index]->is_int()) {
        return parse_result(
          Expr_ptr(
          new Const(static_cast<TInt*>(t_vec[current_index].get())->get_val())),
          current_index+1);
    }

    throw std::pair<size_t, std::string>(current_index, "Invalid token!");
}
            
std::vector<Expr_ptr> parseTokens(const token_vec& t_vec) {
    size_t current_index = 0;
    std::vector<Expr_ptr> result;

    try {
        while(current_index < t_vec.size()) {
            auto pair = parseA(t_vec, current_index);
            if(pair.second >= t_vec.size() ||
               t_vec[pair.second]->is_semicolon()) {
                current_index = pair.second + 1;
                result.push_back(pair.first);
            }
            else {
                throw std::pair<size_t, std::string>(
                    current_index, 
                    "Missing semicolon!");
            }
        }
    }
    catch(std::pair<size_t, std::string> pair) {
        std::cout << "Error at token " << pair.first << ": " << pair.second
                  << "\n";
        return std::vector<Expr_ptr>();
    }

    return result;
}

// Printing expressions!

std::ostream& print_spaces(std::ostream& os, int n) {
    for(int i = 0; i < n; i++) {
        os << " ";
    }
    return os;
}

std::ostream& Const::print(std::ostream& os, int indent) const {
    print_spaces(os, INDENT_LEVEL * indent);
    os << "(Const " << _data << ")";
    return os;
}

std::ostream& Var::print(std::ostream& os, int indent) const {
    print_spaces(os, INDENT_LEVEL * indent);
    os << "(Var " << _data << ")";
    return os;
}

std::ostream& Plus::print(std::ostream& os, int indent) const {
    print_spaces(os, INDENT_LEVEL * indent);
    os << "(Plus\n";
    _e1->print(os, indent+1);
    os << "\n";
    _e2->print(os, indent+1);
    os << ")";
    return os;
}

std::ostream& Minus::print(std::ostream& os, int indent) const {
    print_spaces(os, INDENT_LEVEL * indent);
    os << "(Minus\n";
    _e1->print(os, indent+1);
    os << "\n";
    _e2->print(os, indent+1);
    os << ")";
    return os;
}

std::ostream& Times::print(std::ostream& os, int indent) const {
    print_spaces(os, INDENT_LEVEL * indent);
    os << "(Times\n";
    _e1->print(os, indent+1);
    os << "\n";
    _e2->print(os, indent+1);
    os << ")";
    return os;
}

std::ostream& Div::print(std::ostream& os, int indent) const {
    print_spaces(os, INDENT_LEVEL * indent);
    os << "(Div\n";
    _e1->print(os, indent+1);
    os << "\n";
    _e2->print(os, indent+1);
    os << ")";
    return os;
}

std::ostream& Exp::print(std::ostream& os, int indent) const {
    print_spaces(os, INDENT_LEVEL * indent);
    os << "(Exp\n";
    _e1->print(os, indent+1);
    os << "\n";
    _e2->print(os, indent+1);
    os << ")";
    return os;
}

std::ostream& UMinus::print(std::ostream& os, int indent) const {
    print_spaces(os, INDENT_LEVEL * indent);
    os << "(UMinus\n";
    _e->print(os, indent+1);
    os << ")";
    return os;
}

std::ostream& UPlus::print(std::ostream& os, int indent) const {
    print_spaces(os, INDENT_LEVEL * indent);
    os << "(UPlus\n";
    _e->print(os, indent+1);
    os << ")";
    return os;
}

std::ostream& Assign::print(std::ostream& os, int indent) const {
    print_spaces(os, INDENT_LEVEL * indent);
    os << "(Assign " << _var << ":\n";
    _e->print(os, indent+1);
    os << ")";
    return os;
}

// Eval functions.

eval_pair Const::eval(expr_map& map) {
    return eval_pair(SUCCESS, _data);
}

eval_pair Var::eval(expr_map& map) {
    if(map.find(_data) != map.end()) {
        return eval_pair(SUCCESS, map[_data]->eval(map).second);
    }
    return eval_pair(FAILURE, 0);
}

eval_pair Plus::eval(expr_map& map) { 
    auto p1 = _e1->eval(map);
    auto p2 = _e2->eval(map);
    if(p1.first != FAILURE && p2.first != FAILURE) {
        return eval_pair(SUCCESS, p1.second + p2.second);
    }
    return eval_pair(FAILURE, 0);
}

eval_pair Minus::eval(expr_map& map) {
    auto p1 = _e1->eval(map);
    auto p2 = _e2->eval(map);
    if(p1.first != FAILURE && p2.first != FAILURE) {
        return eval_pair(SUCCESS, p1.second - p2.second);
    }
    return eval_pair(FAILURE, 0);
}

eval_pair Times::eval(expr_map& map) {
    auto p1 = _e1->eval(map);
    auto p2 = _e2->eval(map);
    if(p1.first != FAILURE && p2.first != FAILURE) {
        return eval_pair(SUCCESS, p1.second * p2.second);
    }
    return eval_pair(FAILURE, 0);
}

eval_pair Div::eval(expr_map& map) {
    auto p1 = _e1->eval(map);
    auto p2 = _e2->eval(map);
    if(p1.first != FAILURE && p2.first != FAILURE) {
        return eval_pair(SUCCESS, p1.second / p2.second);
    }
    return eval_pair(FAILURE, 0);
}

eval_pair Exp::eval(expr_map& map) {
    auto p1 = _e1->eval(map);
    auto p2 = _e2->eval(map);
    if(p1.first != FAILURE && p2.first != FAILURE) {
        int result = 1;
        // TODO: Negative exponents will come when we do floats.
        for(int i = 0; i < p2.second; i++) {
            result *= p1.second;
        }
        return eval_pair(SUCCESS, result);
    }
    return eval_pair(FAILURE, 0);
}

eval_pair UMinus::eval(expr_map& map) {
    auto p = _e->eval(map);
    if(p.first != FAILURE) {
        return eval_pair(SUCCESS, -(p.second));
    }
    return eval_pair(FAILURE, 0);
}

eval_pair UPlus::eval(expr_map& map) {
    auto p = _e->eval(map);
    if(p.first != FAILURE) {
        return eval_pair(SUCCESS, p.second);
    }
    return eval_pair(FAILURE, 0);
}

eval_pair Assign::eval(expr_map& map) {
    auto p = _e->eval(map);
    if(p.first != FAILURE) {
        map.emplace(_var, _e);
        return eval_pair(ASSIGN, p.second);
    }
    return eval_pair(FAILURE, 0);
}

std::ostream& operator <<(std::ostream& os, const eval_pair &e) {
    switch(e.first) {
        case FAILURE:
            os << "FAILURE";
            break;
        case ASSIGN:
            os << "ASSIGN " << e.second;
            break;
        case SUCCESS:
            os << e.second;
            break;
    }
    return os;
}


