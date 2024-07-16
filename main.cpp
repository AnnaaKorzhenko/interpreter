#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <utility>


using namespace std;

enum class TokenType {
    Number, Plus, Minus, Multiply, Divide,
    LeftParenth, RightParenth, EndOfFile,
    Pow, Abs, Min, Max, Comma
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

    Token function() {
        string result;
        while (isalpha(currentChar)) {
            result += currentChar;
            moveNext();
        }
        if (result == "pow") return Token(TokenType::Pow);
        if (result == "abs") return Token(TokenType::Abs);
        if (result == "min") return Token(TokenType::Min);
        if (result == "max") return Token(TokenType::Max);
        throw runtime_error("Unknown function: " + result);
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
            if (isalpha(currentChar)) return function();
            if (currentChar == ',') { moveNext(); return Token(TokenType::Comma); }
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

    double numberParenthsFunction() {
        double result;
        if (currentToken.type == TokenType::Number) {
            result = stod(currentToken.value);
            getToken(TokenType::Number);
        } else if (currentToken.type == TokenType::LeftParenth) {
            getToken(TokenType::LeftParenth);
            result = calculate();
            getToken(TokenType::RightParenth);
        } else if (currentToken.type == TokenType::Pow) {
            getToken(TokenType::Pow);
            getToken(TokenType::LeftParenth);
            double base = calculate();
            getToken(TokenType::Comma);
            double exponent = calculate();
            getToken(TokenType::RightParenth);
            result = pow(base, exponent);
        } else if (currentToken.type == TokenType::Abs) {
            getToken(TokenType::Abs);
            getToken(TokenType::LeftParenth);
            result = abs(calculate());
            getToken(TokenType::RightParenth);
        } else if (currentToken.type == TokenType::Min) {
            getToken(TokenType::Min);
            getToken(TokenType::LeftParenth);
            double arg1 = calculate();
            getToken(TokenType::Comma);
            double arg2 = calculate();
            getToken(TokenType::RightParenth);
            result = min(arg1, arg2);
        } else if (currentToken.type == TokenType::Max) {
            getToken(TokenType::Max);
            getToken(TokenType::LeftParenth);
            double arg1 = calculate();
            getToken(TokenType::Comma);
            double arg2 = calculate();
            getToken(TokenType::RightParenth);
            result = max(arg1, arg2);
        } else throw runtime_error("Unexpected token in factor");
        return result;
    }

    double calculate() {
        double result = numberParenthsFunction();
        while (currentToken.type == TokenType::Multiply || currentToken.type == TokenType::Divide) {
            TokenType operation = currentToken.type;
            getToken(operation);
            if (operation == TokenType::Multiply) result *= numberParenthsFunction();
            else result /= numberParenthsFunction();
        }
        while (currentToken.type == TokenType::Plus || currentToken.type == TokenType::Minus) {
            TokenType operation = currentToken.type;
            getToken(operation);
            if (operation == TokenType::Plus) result += numberParenthsFunction();
            else result -= numberParenthsFunction();
        }
        return result;
    }

public:
    Parser(const string& input) : tokenStream(input), currentToken(tokenStream.getNextToken()) {}

    double parse() {
        return calculate();
    }
};

int main() {
    string input1 = "3 + 5.5 * (10 + 2)";
    string input2 = "3 + pow(2,3) * 10 / 2 + 5 + min(4,6) - max(7,3)";
    string input3 = "pow(2, 3) + 10";
    Parser parser(input3);

    try {
        double result = parser.parse();
        cout << "Result: " << result << endl;
    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}


