#include "parser.hpp"
#include <cctype>
#include <stdexcept>
#include <sstream>

void Parser::skipWhitespace() {
    while (pos_ < input_.size() && std::isspace(input_[pos_])) {
        ++pos_;
    }
}

char Parser::peek() const {
    return (pos_ < input_.size()) ? input_[pos_] : '\0';
}

char Parser::get() {
    return (pos_ < input_.size()) ? input_[pos_++] : '\0';
}

Expression<long double> Parser::parseExpression() {
    Expression<long double> expr = parseTerm();
    skipWhitespace();
    while (peek() == '+' || peek() == '-') {
        char op = get();
        skipWhitespace();
        Expression<long double> term = parseTerm();
        expr = (op == '+') ? (expr + term) : (expr - term);
        skipWhitespace();
    }
    return expr;
}

Expression<long double> Parser::parseTerm() {
    Expression<long double> expr = parseFactor();
    skipWhitespace();
    while (peek() == '*' || peek() == '/') {
        char op = get();
        skipWhitespace();
        Expression<long double> factor = parseFactor();
        expr = (op == '*') ? (expr * factor) : (expr / factor);
        skipWhitespace();
    }
    return expr;
}

Expression<long double> Parser::parseFactor() {
    Expression<long double> expr = parsePrimary();
    skipWhitespace();
    while (peek() == '^') {
        get();
        skipWhitespace();
        Expression<long double> exponent = parseFactor();
        expr = expr ^ exponent;
        skipWhitespace();
    }
    return expr;
}

Expression<long double> Parser::parsePrimary() {
    skipWhitespace();
    char c = peek();
    if (c == '(') {
        get();
        Expression<long double> expr = parseExpression();
        skipWhitespace();
        if (get() != ')')
            throw std::runtime_error("Expected ')'");
        return expr;
    } else if (std::isdigit(c) || c == '.') {
        return parseNumber();
    } else if (std::isalpha(c)) {
        std::string id = parseIdentifier();
        skipWhitespace();
        // Если после идентификатора идёт скобка – это функция.
        if (peek() == '(') {
            get();
            Expression<long double> arg = parseExpression();
            skipWhitespace();
            if (get() != ')')
                throw std::runtime_error("Expected ')' after function argument");
            if (id == "sin")
                return ::sin(arg);
            else if (id == "cos")
                return ::cos(arg);
            else if (id == "ln")
                return ::ln(arg);
            else if (id == "exp")
                return ::exp(arg);
            else
                throw std::runtime_error("Unknown function: " + id);
        } else {
            // Иначе – переменная.
            return Expression<long double>(id);
        }
    } else if (c == '-') {
        get();
        return Expression<long double>(-1.0L) * parsePrimary();
    }
    throw std::runtime_error("Unexpected character in input");
}

Expression<long double> Parser::parseNumber() {
    skipWhitespace();
    size_t start = pos_;
    while (pos_ < input_.size() && (std::isdigit(input_[pos_]) || input_[pos_] == '.'))
        pos_++;
    std::string numStr = input_.substr(start, pos_ - start);
    std::istringstream iss(numStr);
    long double value;
    iss >> value;
    return Expression<long double>(value);
}

std::string Parser::parseIdentifier() {
    skipWhitespace();
    size_t start = pos_;
    while (pos_ < input_.size() && std::isalpha(input_[pos_]))
        pos_++;
    return input_.substr(start, pos_ - start);
}

Expression<long double> parseExpression(const std::string &str) {
    Parser parser(str);
    return parser.parseExpression();
}
