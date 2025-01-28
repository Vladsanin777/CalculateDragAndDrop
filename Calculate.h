#include <iostream>
#include <vector>
#include <cstring>
#include <cctype>

class oldSimpleExpression {
public:
    explicit SimpleExpression(const char* expression) {
        // Разбиваем выражение на токены
        tokenize_expression(expression);
        
        // Создаем дерево выражения
        result = build_expression_tree(tokens);
        
        if (result.empty()) {
            std::cerr << "Ошибка: дерево выражения пусто." << std::endl;
        }
    }

    void printResult() const {
        if (!result.empty()) {
            printTree(result);
            std::cout << std::endl;
        }
    }

private:
    std::vector<const char*> tokens;
    std::vector<std::vector<const char*>> result;

    void tokenize_expression(const char* expression) {
        const char* delimiters = " \t\n()+-*/^|";
        char* expr_copy = strdup(expression);
        char* token = strtok(expr_copy, delimiters);

        while (token != nullptr) {
            tokens.push_back(token);
            token = strtok(nullptr, delimiters);
        }

        free(expr_copy);
    }

    std::vector<std::vector<const char*>> build_expression_tree(const std::vector<const char*>& tokens) {
        std::vector<const char*> stack;
        std::vector<const char*> operators;

        auto apply_operator = [&]() {
            if (stack.size() < 2) {
                std::cerr << "Ошибка: недостаточно операндов для оператора." << std::endl;
                return;
            }

            const char* op = operators.back();
            operators.pop_back();

            const char* right = stack.back();
            stack.pop_back();

            const char* left = stack.back();
            stack.pop_back();

            // Формируем подвыражение и помещаем в стек
            char* expression = new char[strlen(left) + strlen(op) + strlen(right) + 3];
            sprintf(expression, "(%s %s %s)", left, op, right);

            stack.push_back(expression);
        };

        const char* precedence = "+-*/^";

        for (const char* token : tokens) {
            if (isdigit(token[0])) {
                stack.push_back(token);
            } else if (strchr(precedence, token[0])) {
                while (!operators.empty() && strchr(precedence, operators.back()[0]) >= strchr(precedence, token[0])) {
                    apply_operator();
                }
                operators.push_back(token);
            } else {
                std::cerr << "Неизвестный токен: " << token << std::endl;
            }
        }

        while (!operators.empty()) {
            apply_operator();
        }

        std::vector<std::vector<const char*>> tree;
        for (const char* expr : stack) {
            tree.push_back({expr});
        }

        return tree;
    }

    void printTree(const std::vector<std::vector<const char*>>& tree) const {
        for (const auto& node : tree) {
            for (const char* element : node) {
                std::cout << element << " ";
            }
        }
    }
};



class Calculate {
	explicit Calculate(char *expression) {
		while (strchr(expression, '(')) {
			char *substring = extract_substring(expression);
			calculateSimpleExpression(substring);
			result
		}
	}
	void extract_substring(char *str) {

		// Find the closing parenthesis ')'
		const char *end = strchr(str, ')');

		// Find the matching opening parenthesis '(' from the last closing parenthesis
		const char *start = nullptr;
		for (const char *p = end; p >= str; --p) {
			if (*p == '(') {
				start = p;
				break;
			}
		}

		// Calculate the length of the substring inside the parentheses
		size_t len = size_t(end - start - 1);

		// Allocate memory for the substring
		char *substring = new(nothrow) char[len + 1];

		// Copy the substring from input (inside parentheses)
		strncpy(substring, start + 1, len);
		substring[len] = '\0';  // Null terminate the substring

		// Clean up memory
		return substring;
	}

};



#include <vector>
class LongArifmetic {
private:
	vector<unsigned long long> _digit;
	bool _isNegative;
	unsigned long long _exponent;
	void _removeZeros() {
		size_t n = max((long long) 1, _exponent);

		while (_digit.size() > n && _digit[_digit.size() - 1] == 0)
			_digit.erase(_digit.end() - 1);

		while (_digit.size() > 1 && _digit[0] == 0) {
			_digit.erase(_digit.begin());
			exponent--;
		}

		while (_digit.size() > 1 && _digit[_digit.size() - 1] == 0)
			_digit.erase(_digit.end() - 1);

		if (isZero()) exponent = 1, sign = 1;

		_normalize();
	}
	void _normalize() {
		size_t start = max(exponent, (long) 0);
		size_t realDigits = digits.size() - start;

		if (realDigits >= divDigits) {
			size_t count = 0;
			size_t maxCount = 0;

			size_t i = start;

			while(i < _digit.size()) {
				count = 0;

				while (i < digits.size() && digits[i] == 9) 
					count++, i++;
				
				if (count > maxCount)
					maxCount = count;

				i++;
			}
			
			if (maxCount > divDigits * 4 / 5) {
				i = digits.size() - 1;

				do {
					count = 0;

					while (i > 0 && digits[i] != 9)
						i--;

					while (i > 0 && digits[i] == 9) {
						count++;
						i--;
					}
				} while (count != maxCount);

				digits.erase(digits.begin() + i + 1, digits.end());
				digits[i]++;
			}
		}
public:

