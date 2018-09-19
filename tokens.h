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
        virtual bool is_int() { return false; }
        virtual bool is_var() { return false; }
        virtual bool is_plus() { return false; }
        virtual bool is_minus() { return false; }
        virtual bool is_times() { return false; }
        virtual bool is_div() { return false; }
        virtual bool is_exp() { return false; }
        virtual bool is_lparen() { return false; }
        virtual bool is_rparen() { return false; }
        virtual bool is_equals() { return false; }
        virtual bool is_semicolon() { return false; }
};

class TInt : public Token {
    public:
        TInt(int val) { _val = val; }
        std::ostream& print(std::ostream& os);
        int get_val() const { return _val; }
        bool is_int() { return true; }
    private:
        int _val;
};

class TVar : public Token {
    public:
        TVar(const std::string& s) { _val = s; }
        std::ostream& print(std::ostream& os);
        const std::string& get_val() const { return _val; }
        bool is_var() { return true; }
    private:
        std::string _val;
};

class TPlus : public Token {
    public:
        std::ostream& print(std::ostream& os);
        bool is_plus() { return true; }
};

class TMinus : public Token {
    public:
        std::ostream& print(std::ostream& os);
        bool is_minus() { return true; }
};

class TTimes : public Token {
    public:
        std::ostream& print(std::ostream& os);
        bool is_times() { return true; }
};

class TDiv : public Token {
    public:
        std::ostream& print(std::ostream& os);
        bool is_div() { return true; }
};

class TExp : public Token {
    public:
        std::ostream& print(std::ostream& os);
        bool is_exp() { return true; }
};

class TLParen : public Token {
    public:
        std::ostream& print(std::ostream& os);
        bool is_lparen() { return true; }
};

class TRParen : public Token {
    public:
        std::ostream& print(std::ostream& os);
        bool is_rparen() { return true; }
};

class TEquals : public Token {
    public:
        std::ostream& print(std::ostream& os);
        bool is_equals() { return true; }
};

class TSemicolon : public Token {
    public:
        std::ostream& print(std::ostream& os);
        bool is_semicolon() { return true; }
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
