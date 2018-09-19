#include "tokens.h"
#include "expr.h"

#define VERBOSE_FLAG true

int main() {
    std::string user_input;
    expr_map e_map;
    do {
        std::vector<std::unique_ptr<Token>> t_vec = lex_str(std::cin);
        std::cin.clear();
        if(t_vec.empty()) {
            break;
        }
        if(VERBOSE_FLAG) {
            std::cout << t_vec << "\n";
        }

        std::vector<Expr_ptr> e_vec = parseTokens(t_vec);
    
        int counter = 1;
        for(auto e : e_vec) {
            if(VERBOSE_FLAG) {
                e->print(std::cout);
                std::cout << "\n";
            }
            std::cout << "Expression " << counter << ": "
                      << e->eval(e_map) << "\n";
            counter++;
        }
    } while(true);

    return 0;
}
