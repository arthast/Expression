#ifndef PARSER_HPP
#define PARSER_HPP

#include "expression.hpp"

class Parser {
public:
    explicit Parser(const std::string &input) : input_(input), pos_(0) {}

    Expression<long double> parseExpression();

private:
    std::string input_;
    size_t pos_;

    // Парсит сумму и разность.
    Expression<long double> parseTerm();

    // Парсит произведение и деление.
    Expression<long double> parseFactor();

    // Парсит элемент: число, переменная, функция, скобочное выражение.
    Expression<long double> parsePrimary();

    // Парсит число.
    Expression<long double> parseNumber();

    // Парсит идентификатор (имя переменной или имя функции).
    std::string parseIdentifier();

    // Пропуск пробельных символов.
    void skipWhitespace();

    // Возврат текущего символа.
    char peek() const;

    // Чтение текущего символа и переход к следующему.
    char get();
};


Expression<long double> parseExpression(const std::string &str);

#endif
