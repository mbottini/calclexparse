#include "tokens.h"

std::ostream& TInt::print(std::ostream& os) {
    os << "(TInt " << _val << ")";
    return os;
}

std::ostream& TVar::print(std::ostream& os) {
    os << "(TVar \"" << _val << "\")";
    return os;
}

std::ostream& TPlus::print(std::ostream& os) {
    os << "(TPlus)";
    return os;
}

std::ostream& TMinus::print(std::ostream& os) {
    os << "(TMinus)";
    return os;
}

std::ostream& TTimes::print(std::ostream& os) {
    os << "(TTimes)";
    return os;
}

std::ostream& TDiv::print(std::ostream& os) {
    os << "(TDiv)";
    return os;
}

std::ostream& TExp::print(std::ostream& os) {
    os << "(TExp)";
    return os;
}

std::ostream& TLParen::print(std::ostream& os) {
    os << "(TLParen)";
    return os;
}

std::ostream& TRParen::print(std::ostream& os) {
    os << "(TRParen)";
    return os;
}

std::ostream& TEquals::print(std::ostream& os) {
    os << "(TEquals)";
    return os;
}

std::ostream& TSemicolon::print(std::ostream& os) {
    os << "(TSemicolon)";
    return os;
}

std::vector<std::unique_ptr<Token>> lex_str(std::istream& is) {
    std::string line;
    std::vector<std::unique_ptr<Token>> t_vec;
    size_t current_index = 0;
    while(std::getline(is, line)) {
        current_index = 0;
        while(current_index < line.length()) {
            // Currently a character?
            if(std::isalpha(line[current_index])) {
                t_vec.emplace_back(new TVar(lex_str(line, current_index)));
            }

            else if(std::isdigit(line[current_index])) {
                t_vec.emplace_back(new TInt(lex_int(line, current_index)));
            }

            // Remainder are single-character.
            else {
                switch(line[current_index]) {
                    case '+':
                        t_vec.emplace_back(new TPlus());
                        break;
                    case '-':
                        t_vec.emplace_back(new TMinus());
                        break;
                    case '*':
                        t_vec.emplace_back(new TTimes());
                        break;
                    case '/':
                        t_vec.emplace_back(new TDiv());
                        break;
                    case '^':
                        t_vec.emplace_back(new TExp());
                        break;
                    case '(':
                        t_vec.emplace_back(new TLParen());
                        break;
                    case ')':
                        t_vec.emplace_back(new TRParen());
                        break;
                    case '=':
                        t_vec.emplace_back(new TEquals());
                        break;
                    case ';':
                        t_vec.emplace_back(new TSemicolon());
                        break;
                    default:
                        break;
                }

                // All other characters are ignored.
                current_index++;
            }
        }
    }

    return t_vec;
}

std::ostream& operator <<(std::ostream& os,
                          const std::vector<std::unique_ptr<Token>>& t_vec) {
    os << "[";
    for(const auto& ptr : t_vec) {
        ptr->print(os);
        if(&ptr != &(t_vec.back())) {
            os << " ";
        }
    }
    os << "]";
    return os;
}

int lex_int(const std::string& s, size_t& current) {
    size_t start = current;
    for(; current < s.length() && isdigit(s[current]); current++) {}
    return std::stoi(s.substr(start, current - start));
}

std::string lex_str(const std::string& s, size_t& current) {
    size_t start = current;
    for(; current < s.length() && isalpha(s[current]); current++) {}
    return s.substr(start, current - start);
}
