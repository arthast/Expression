#include <iostream>
#include <string>
#include <map>
#include <complex>
#include "../src/parser.hpp"
#include "../src/expression.hpp"

void testEvaluation() {
    try {
        auto expr = parseExpression("x * y");
        std::map<std::string, long double> context = {{"x", 10}, {"y", 12}};
        long double res = expr.eval(context);
        if (res == 120)
            std::cout << "testEvaluation: OK\n";
        else
            std::cout << "testEvaluation: FAIL (expected 120, got " << res << ")\n";
    } catch (const std::exception &ex) {
        std::cout << "testEvaluation: FAIL (" << ex.what() << ")\n";
    }
}

void testDifferentiation() {
    try {
        auto expr = parseExpression("x * sin(x)");
        auto deriv = expr.differentiate("x");
        // Для проверки вычисляем производную при x = 1.
        std::map<std::string, long double> context = {{"x", 1}};
        long double approx = deriv.eval(context);
        long double expected = 1 * std::cos(1) + std::sin(1); // x*cos(x) + sin(x)
        if (std::abs(approx - expected) < 1e-9)
            std::cout << "testDifferentiation: OK\n";
        else
            std::cout << "testDifferentiation: FAIL (expected " << expected << ", got " << approx << ")\n";
    } catch (const std::exception &ex) {
        std::cout << "testDifferentiation: FAIL (" << ex.what() << ")\n";
    }
}

void testSubstitution() {
    try {
        // Выражение: x + y
        Expression<long double> expr("x");
        expr += Expression<long double>("y");
        // Подставляем y = 5
        auto substituted = expr.substitute("y", Expression(5.0L));
        std::map<std::string, long double> context = {{"x", 3}};
        long double res = substituted.eval(context);
        if (res == 8)
            std::cout << "testSubstitution: OK\n";
        else
            std::cout << "testSubstitution: FAIL (expected 8, got " << res << ")\n";
    } catch (const std::exception &ex) {
        std::cout << "testSubstitution: FAIL (" << ex.what() << ")\n";
    }
}

void testParsing() {
    try {
        auto expr = parseExpression("3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3");
        std::string s = expr.to_string();
        if (!s.empty())
            std::cout << "testParsing: OK\n";
        else
            std::cout << "testParsing: FAIL (empty string)\n";
    } catch (const std::exception &ex) {
        std::cout << "testParsing: FAIL (" << ex.what() << ")\n";
    }
}

void testComplexUsage() {
    try {
        // Пример: (3+4i) + 1 должно давать (4+4i)
        Expression expr(std::complex<long double>(3, 4));
        expr = expr + Expression(std::complex<long double>(1, 0));
        // Для комплексных чисел контекст не используется.
        auto res = expr.eval({});
        if (res == std::complex<long double>(4, 4))
            std::cout << "testComplexUsage: OK\n";
        else
            std::cout << "testComplexUsage: FAIL (expected (4,4), got ("
                    << res.real() << "," << res.imag() << "))\n";
    } catch (const std::exception &ex) {
        std::cout << "testComplexUsage: FAIL (" << ex.what() << ")\n";
    }
}


int main() {
    testEvaluation();
    testDifferentiation();
    testSubstitution();
    testParsing();
    testComplexUsage();
    return 0;
}
