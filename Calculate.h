#include <vector>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mpfr.h>

// Add this function definition
const char* strrstr(const char* haystack, const char* needle) {
    const char* last = nullptr;
    const char* tmp = haystack;
    
    while ((tmp = strstr(tmp, needle)) != nullptr) {
        last = tmp;
        tmp += strlen(needle);
    }
    
    return last;
}

void mpfr_asec(mpfr_t& result, const mpfr_t x, mpfr_rnd_t rnd) {
    mpfr_t temp;
    mpfr_init2(temp, mpfr_get_prec(result));

    // temp = 1 / x
    mpfr_ui_div(temp, 1, x, rnd);

    // result = asin(temp)
    mpfr_acos(result, temp, rnd);

    mpfr_clear(temp);
}

void mpfr_acsc(mpfr_t& result, const mpfr_t x, mpfr_rnd_t rnd) {
    mpfr_t temp;
    mpfr_init2(temp, mpfr_get_prec(result));

    // temp = 1 / x
    mpfr_ui_div(temp, 1, x, rnd);

    // result = asin(temp)
    mpfr_asin(result, temp, rnd);

    mpfr_clear(temp);
}

void mpfr_acot(mpfr_t& result, const mpfr_t x, mpfr_rnd_t rnd) {
    mpfr_t temp;
    mpfr_init2(temp, mpfr_get_prec(result));

    // temp = 1 / x
    mpfr_ui_div(temp, 1, x, rnd);

    // result = atan(temp)
    mpfr_atan(result, temp, rnd);

    // Коррекция для отрицательных x
    if (mpfr_sgn(x) < 0) {
        mpfr_const_pi(temp, rnd);
        mpfr_add(result, result, temp, rnd);
    }

    mpfr_clear(temp);
}
enum ArifmeticAction { \
	none, addition, subtraction, \
	remainderFromDivision, \
	multiplication, division, \
	sin, cos, tan, sec, csc, cot, sgn, \
	acos, asin, atan, asec, acsc, acot \
};
static inline bool _isBrackets(char symbol) {
	return symbol == '(' || symbol == ')';
}
static inline bool _isOperatorSub(const char * const symbol, const char * const start) {
	if (start < symbol) {
		const char symbolBefore = *(symbol - 1UL);
		if (isalpha(symbolBefore) || isdigit(symbolBefore) || \
			_isBrackets(symbolBefore) || symbolBefore == '.')
			return true;
	}
	return false;
}
static inline size_t _shearchMaxLevelBrackets(const char * expression) {
	size_t maxLevelBrackets {0UL}, levelBrackets {0UL};
	const char * const expressionEnd {expression + strlen(expression)};
	for (; expression < expressionEnd; expression++) {
		if (*expression == '(')
			if (maxLevelBrackets < ++levelBrackets)
				maxLevelBrackets = levelBrackets;
			else;
		else if (*expression == ')')
			levelBrackets--;
	}
	return maxLevelBrackets;
}
static inline void _replaceOnX(char *expression, const size_t level = 0UL) {
	char * expressionEnd {expression + strlen(expression)}, temp;
	size_t levelBrackets {0UL};
	for (; expression < expressionEnd; expression++)
	{
		temp = *expression;
		if (temp == '(') levelBrackets++;
		if (levelBrackets != level) *expression = 'x';
		if (temp == ')') levelBrackets--;
	}
}
static inline const char * _shearchNotPriorityOperator( \
	const char * const expression, bool &isTwoOperand, unsigned char &lenOperator, \
	ArifmeticAction &action \
) {
	action = none, lenOperator = 0, isTwoOperand = true;
	size_t len{strlen(expression)};
	puts(expression);

	char * const copyExpression {new char[len+1UL]()};
	const size_t maxLevelBrackets {_shearchMaxLevelBrackets(expression)+1UL};
	//std::cout << "maxLevelBrackets " << maxLevelBrackets << std::endl;
	const char * ptr {0L}, *temp;
	for (size_t levelBrakets{0UL}; ptr == 0L && levelBrakets < maxLevelBrackets; levelBrakets++) {
		strcpy(copyExpression, expression);
		_replaceOnX(copyExpression, levelBrakets);
		//puts(copyExpression);
		isTwoOperand = true;
		if (ptr = strrstr(copyExpression, "+")) {
			lenOperator = 1, action = addition; break;
		}
		if (ptr = strrstr(copyExpression, "-")) {
			lenOperator = 1, action = subtraction; break;
		}
		if (ptr = strrstr(copyExpression, "*")) {
			lenOperator = 1, action = multiplication; break;
		}
		if (ptr = strrstr(copyExpression, "/")) {
			lenOperator = 1, action = division; break;
		}
		if (ptr = strrstr(copyExpression, "mod")) {
			lenOperator = 3, action = remainderFromDivision; break;
		}
		isTwoOperand = false;
		if ((temp = strrstr(copyExpression, "sin")) < ptr != !ptr)
			lenOperator = 3, action = sin, ptr = temp; 
		if ((temp = strrstr(copyExpression, "cos")) < ptr != !ptr)
			lenOperator = 3, action = cos, ptr = temp; 
		if ((temp = strrstr(copyExpression, "tan")) < ptr && !ptr)
			lenOperator = 3, action = tan, ptr = temp; 
		if ((temp = strrstr(copyExpression, "cot")) < ptr && !ptr)
			lenOperator = 3, action = cot, ptr = temp; 
		if ((temp = strrstr(copyExpression, "sec")) < ptr && !ptr)
			lenOperator = 3, action = sec, ptr = temp; 
		if ((temp = strrstr(copyExpression, "csc")) < ptr && !ptr)
			lenOperator = 3, action = csc, ptr = temp; 
		if ((temp = strrstr(copyExpression, "asin")) < ptr && !ptr)
			lenOperator = 4, action = asin, ptr = temp; 
		if ((temp = strrstr(copyExpression, "acos")) < ptr && !ptr)
			lenOperator = 4, action = acos, ptr = temp; 
		if ((temp = strrstr(copyExpression, "atan")) < ptr && !ptr)
			lenOperator = 4, action = atan, ptr = temp; 
		if ((temp = strrstr(copyExpression, "acot")) < ptr && !ptr)
			lenOperator = 4, action = acot, ptr = temp; 
		if ((temp = strrstr(copyExpression, "asec")) < ptr && !ptr)
			lenOperator = 4, action = asec, ptr = temp; 
		if ((temp = strrstr(copyExpression, "acsc")) < ptr && !ptr)
			lenOperator = 4, action = acsc, ptr = temp;
		//puts("rfjvkfkml");
	}
	//std::cout << ptr << std::endl;
	//putchar('A');
	//putchar(*ptr);
	delete [] copyExpression;
	//puts("klsdsdsd");
	if (ptr) return expression + (ptr - copyExpression);
	return 0L;
}

static inline const char * normalize( \
	const char * const expression, bool isDelete = true \
) {
	char *number {new char[strlen(expression) + 1UL]};
	strcpy(number, expression);
	if (isDelete)
		delete [] expression;
	char * deleteChar;
	size_t len {strlen(number) + 1UL};
	//std::cout << len << std::endl;
	while (deleteChar = strchr(number, '('))
		memmove(deleteChar, deleteChar + 1L, len - (number - deleteChar));
	while (deleteChar = strchr(number, ')'))
		memmove(deleteChar, deleteChar + 1L, len - (number - deleteChar));
	return number;
}


union Data {mpfr_t number; ArifmeticAction action;};
class Expression {
private:
	Expression *_parent, *_operand1, *_operand2;
	Data _data;
	bool _isNumber;


	static size_t size;
	inline explicit Expression (
		const char * const number, \
		Expression * parent, \
		bool isDelete = true \
	) : _isNumber{true}, _parent{parent} {
		//puts("jkk");
		//puts(expression);
		mpfr_init2(_data.number, size);
		mpfr_set_str(_data.number, number, 10, MPFR_RNDN);
		//puts("ndh");
		if (isDelete) delete [] number;
	}
	inline explicit Expression(ArifmeticAction action, \
		Expression *parent = nullptr)
		: _isNumber{false}, _parent{parent}, \
		_data{.action = action} {}
public:
	static Expression *buildExpressionTree( \
		const char *expression, \
		Expression * parent = 0L, \
		bool isDelete = true \
	) {
		//puts(expression);
		unsigned char lenOperator;
		bool isTwoOperand;
		ArifmeticAction action;
		const char * ptrOperator = \
			_shearchNotPriorityOperator(expression, \
				isTwoOperand, lenOperator, action);
		//std::cout << action << std::endl;
		//if (ptrOperator)
			//putchar(*ptrOperator);
		//puts("dfsfgv");
		if (!ptrOperator) {
			//puts("kl");
			//if (isDelete) delete [] expression;
			//puts("kl");

			const char * const number = normalize(expression, isDelete);
			//std::cout << (number == expression) << std::endl;
			//puts("dfkfkdl");
			return new Expression{number, parent};
		}
		putchar(*ptrOperator);
		putchar('\n');
		//puts("nm");
		Expression * result = new Expression{action, parent};
		//puts("nm");
		if (isTwoOperand) {
			size_t lenExpression{(size_t)(ptrOperator - expression)};
			char * newExpression = new char[lenExpression+1]();
			strncpy(newExpression, expression, lenExpression);
			//puts("firstOperandStart");
			result->setFirstOperand(buildExpressionTree(newExpression, result));
			//puts("firstOperandEnd");
			//puts("klkdd");
			lenExpression = strlen(expression) - (ptrOperator - expression) - lenOperator;
			//std::cout << "lenTwoOperand " << lenExpression << std::endl;
			newExpression = new char[lenExpression+1]();
			//std:: cout << strlen(newExpression) << std::endl;
			strncpy(newExpression, ptrOperator + lenOperator, lenExpression);
			//puts("twoOperand");
			//puts(newExpression);
			result->setSecondOperand(buildExpressionTree(newExpression, result));
		} else {
			size_t lenExpression{ptrOperator - expression + lenOperator + 1UL};
			char * const newExpression = new char[lenExpression + 1]();
			strncpy(newExpression, ptrOperator + lenOperator, lenExpression);
			result->setFirstOperand(buildExpressionTree(newExpression, result));
			//puts("dj");
		}
		if (isDelete) delete [] expression;
		return result;
	}

	void setParent(Expression *parent) {
		_parent = parent;
	}
	void setFirstOperand(Expression *operand1) {
		_operand1 = operand1;
	}
	void setSecondOperand(Expression *operand2) {
		_operand2 = operand2;
	}
	void getNumber(char * const str) {

	}
	static void calculate(Expression *expression, mpfr_t &result) {
		mpfr_init2(result, size);
		if (expression->_isNumber) {
			mpfr_set(result, expression->_data.number, MPFR_RNDN);
			delete expression;
			return;
		}
		mpfr_t operand1, operand2;
		Expression::calculate(expression->_operand1, operand1);
		ArifmeticAction arifmeticAction = expression->_data.action;
		if (arifmeticAction < sin)
			Expression::calculate(expression->_operand2, operand2);
		switch (arifmeticAction) {
			case addition:
				mpfr_add(result, operand1, operand2, MPFR_RNDN);
				puts("add");
				break;
			case subtraction:
				mpfr_sub(result, operand1, operand2, MPFR_RNDN);
				puts("sub");
				break;
			case remainderFromDivision:
				mpfr_fmod(result, operand1, operand2, MPFR_RNDN);
				puts("fmod");
				break;
			case multiplication:
				mpfr_mul(result, operand1, operand2, MPFR_RNDN);
				puts("mul");
				break;
			case division:
				mpfr_div(result, operand1, operand2, MPFR_RNDN);
				puts("div");
				break;
			case sin:
				mpfr_sin(result, operand1, MPFR_RNDN);
				puts("sin");
				break;
			case cos:
				mpfr_cos(result, operand1, MPFR_RNDN);
				puts("cos");
				break;
			case tan:
				mpfr_tan(result, operand1, MPFR_RNDN);
				puts("tan");
				break;
			case sec:
				mpfr_sec(result, operand1, MPFR_RNDN);
				puts("sec");
				break;
			case csc:
				mpfr_csc(result, operand1, MPFR_RNDN);
				puts("csc");
				break;
			case cot:
				mpfr_cot(result, operand1, MPFR_RNDN);
				puts("cot");
				break;
			case asin:
				mpfr_asin(result, operand1, MPFR_RNDN);
				puts("asin");
				break;
			case acos:
				mpfr_acos(result, operand1, MPFR_RNDN);
				puts("acos");
				break;
			case atan:
				mpfr_atan(result, operand1, MPFR_RNDN);
				puts("atan");
				break;
			case asec:
				mpfr_asec(result, operand1, MPFR_RNDN);
				puts("asec");
				break;
			case acsc:
				mpfr_acsc(result, operand1, MPFR_RNDN);
				puts("acsc");
				break;
			case acot:
				mpfr_acot(result, operand1, MPFR_RNDN);
				puts("acot");
				break;
			//case sgn:
				//result = mpfr_sgn(operand1);
				//puts("sgn");
				//mpfr_printf("Результат: %.50Rf\n", result);
		}
		mpfr_printf("1 %Rf\nr %Rf\n", operand1, result);
		mpfr_clear(operand1);
		if (arifmeticAction < sin) mpfr_clear(operand2);
		//puts("skddk");
		delete expression;
		return;
	}
	~Expression() {
		//puts("delete Expression");
		if (_isNumber) {
			//mpfr_printf("%Rf\n", _data.number);
			mpfr_clear(_data.number);
		}
	}
};

size_t Expression::size = 256;
