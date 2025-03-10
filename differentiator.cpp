#include <iostream>
#include <string>
#include <map>
#include "src/parser.hpp"
#include "src/expression.hpp"

std::pair<std::string, long double> parseAssignment(const std::string &s) {
    size_t pos = s.find('=');
    if (pos == std::string::npos)
        throw std::runtime_error("Invalid assignment: " + s);
    std::string var = s.substr(0, pos);
    long double value = std::stold(s.substr(pos + 1));
    return {var, value};
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage:\n"
                  << "  differentiator --eval \"expression\" var=value ...\n"
                  << "  differentiator --diff \"expression\" --by variable\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string exprStr = argv[2];

    try {
        Expression<long double> expr = parseExpression(exprStr);

        if (mode == "--eval") {
            std::map<std::string, long double> context;
            for (int i = 3; i < argc; ++i) {
                auto assign = parseAssignment(argv[i]);
                context[assign.first] = assign.second;
            }
            long double result = expr.eval(context);
            std::cout << result << std::endl;
        } else if (mode == "--diff") {
            std::string diffVar;

            for (int i = 3; i < argc; ++i) {
                std::string arg = argv[i];
                if (arg == "--by" && i + 1 < argc) {
                    diffVar = argv[i + 1];
                    break;
                }
            }
            if (diffVar.empty()) {
                std::cerr << "Missing --by option for differentiation\n";
                return 1;
            }
            Expression<long double> deriv = expr.differentiate(diffVar);
            std::cout << deriv.to_string() << std::endl;
        } else {
            std::cerr << "Unknown mode: " << mode << std::endl;
            return 1;
        }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
