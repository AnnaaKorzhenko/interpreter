#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

enum class TokenType {
    Number, Plus, Minus, Multiply, Divide,
    LeftParenth, RightParenth, EndOfFile
};

struct Token {
    TokenType type;
    string value;
    Token(TokenType type, string value = "") : type(type), value(value) {}
};

class TokenStream {
    string input;
    size_t position;
    char currentChar;

    void moveNext() {
        position++;
        if (position < input.length()) {
            currentChar = input[position];
        } else {
            currentChar = '\0';
        }
    }

    void skipWhitespace() {
        while (currentChar != '\0' && isspace(currentChar)) {
            moveNext();
        }
    }

    Token number() {
        string result;
        while (isdigit(currentChar) || currentChar == '.') {
            result += currentChar;
            moveNext();
        }
        return Token(TokenType::Number, result);
    }

public:
    TokenStream(const string& input) : input(input), position(0), currentChar(input[0]) {}

    Token getNextToken() {
        while (currentChar != '\0') {
            if (isspace(currentChar)) {
                skipWhitespace();
                continue;
            }
            if (isdigit(currentChar) || currentChar == '.') return number();
            if (currentChar == '+') { moveNext(); return Token(TokenType::Plus); }
            if (currentChar == '-') { moveNext(); return Token(TokenType::Minus); }
            if (currentChar == '*') { moveNext(); return Token(TokenType::Multiply); }
            if (currentChar == '/') { moveNext(); return Token(TokenType::Divide); }
            if (currentChar == '(') { moveNext(); return Token(TokenType::LeftParenth); }
            if (currentChar == ')') { moveNext(); return Token(TokenType::RightParenth); }
            throw runtime_error("Unknown character");
        }
        return Token(TokenType::EndOfFile);
    }
};

int main() {
    return 0;
}


