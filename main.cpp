#include "tokens.h"
#include "expr.h"

int main() {
    std::vector<std::unique_ptr<Token>> t_vec = lex_str(std::cin);
    expr_map e_map;
    std::cout << t_vec << "\n";
    std::vector<Expr_ptr> e_vec = parseTokens(t_vec);
    for(auto e : e_vec) {
        e->print(std::cout);
        std::cout << "\n";
        std::cout << e->eval(e_map) << "\n";
    }
    return 0;
}
