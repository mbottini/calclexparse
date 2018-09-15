#ifndef TOKENS_H
#define TOKENS_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cctype>

class Token {
    public:
        virtual std::ostream& print(std::ostream& os) = 0;
        virtual ~Token() {}
};

class TInt : public Token {
    public:
        TInt(int val) { _val = val; }
        std::ostream& print(std::ostream& os);
    private:
        int _val;
};

class TVar : public Token {
    public:
        TVar(const std::string& s) { _val = s; }
        std::ostream& print(std::ostream& os);
    private:
        std::string _val;
};

class TPlus : public Token {
    public:
        std::ostream& print(std::ostream& os);
};

class TMinus : public Token {
    public:
        std::ostream& print(std::ostream& os);
};

class TTimes : public Token {
    public:
        std::ostream& print(std::ostream& os);
};

class TDiv : public Token {
    public:
        std::ostream& print(std::ostream& os);
};

class TExp : public Token {
    public:
        std::ostream& print(std::ostream& os);
};

class TLParen : public Token {
    public:
        std::ostream& print(std::ostream& os);
};

class TRParen : public Token {
    public:
        std::ostream& print(std::ostream& os);
};

class TEquals : public Token {
    public:
        std::ostream& print(std::ostream& os);
};

class TSemicolon : public Token {
    public:
        std::ostream& print(std::ostream& os);
};

// Lex a stream into tokens

std::vector<std::unique_ptr<Token>> lex_str(std::istream& is);

// Conveniently print a vector of tokens.

std::ostream& operator <<(std::ostream& os,
                          const std::vector<std::unique_ptr<Token>>& t_vec);

// Utility functions for lexing.

int lex_int(const std::string& s, size_t& current);
std::string lex_str(const std::string& s, size_t& current_index);
#endif
