#include "tokens.h"

int main() {
    std::vector<std::unique_ptr<Token>> t_vec = lex_str(std::cin);

    std::cout << t_vec << "\n";
    return 0;
}
