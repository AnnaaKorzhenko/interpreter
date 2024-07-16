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

class Parser {
    TokenStream tokenStream;
    Token currentToken;

    void getToken(TokenType type) {
        if (currentToken.type == type) {
            currentToken = tokenStream.getNextToken();
        }
        else throw runtime_error("Unexpected token");
    }

    double numOrParenths() {
        double result;
        if (currentToken.type == TokenType::Number) {
            result = stod(currentToken.value);
            getToken(TokenType::Number);
        } else if (currentToken.type == TokenType::LeftParenth) {
            getToken(TokenType::LeftParenth);
            result = plusOrMinus();
            getToken(TokenType::RightParenth);
        } else throw runtime_error("Unexpected token in factor");
        return result;
    }

    double multiplyOrDivide() {
        double result = numOrParenths();
        while (currentToken.type == TokenType::Multiply || currentToken.type == TokenType::Divide) {
            TokenType operation = currentToken.type;
            getToken(operation);
            if (operation == TokenType::Multiply) result *= numOrParenths();
            else result /= numOrParenths();
        }
        return result;
    }

    double plusOrMinus() {
        double result = multiplyOrDivide();
        while (currentToken.type == TokenType::Plus || currentToken.type == TokenType::Minus) {
            TokenType operation = currentToken.type;
            getToken(operation);
            if (operation == TokenType::Plus) result += multiplyOrDivide();
            else result -= multiplyOrDivide();
        }
        return result;
    }

public:
    Parser(const string& input) : tokenStream(input), currentToken(tokenStream.getNextToken()) {}

    double parse() {
        return plusOrMinus();
    }
};

int main() {
    string input = "3 + 5 * 10 / 2";
    Parser parser(input);

    try {
        double result = parser.parse();
        cout << "Result: " << result << endl;
    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}


