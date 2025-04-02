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
			_isBrackets(symbolBefore))
			return true;
	}
	return false;
}
static inline const char * _shearchNotPriorityOperator( \
	const char * const expression, bool &isTwoOperand, unsigned char &lenOperator, \
	ArifmeticAction &action \
) {
	action = none, lenOperator = 0, isTwoOperand = true;
	size_t len{strlen(expression)}, levelBrakets = 0UL;
	const char * finaly = 0L;

	for (const char *ptrExpression = expression + len; \
		ptrExpression-- != expression;)
	{
		if (*ptrExpression == ')') {levelBrakets++; continue;}
		if (*ptrExpression == '(') {levelBrakets--; continue;}
		if (levelBrakets) continue;
		if (*ptrExpression == '+') {
			lenOperator = 1, action = addition; return ptrExpression;
		}
		if (*ptrExpression == '-' && _isOperatorSub(ptrExpression, expression)) {
			lenOperator = 1, action = subtraction; return ptrExpression;
		}
		if (*ptrExpression == '*' && finaly == 0) 
			lenOperator = 1, action = multiplication, finaly = ptrExpression;
		if (*ptrExpression == '/' && finaly == 0)
			lenOperator = 1, action = division, finaly = ptrExpression;
	}
	const char *temp;
	if ((temp = strrstr(expression, "mod")) > finaly) 
		lenOperator = 3, action = remainderFromDivision, finaly = temp;
	std::cout << (long)finaly << std::endl;
	if (finaly) return finaly;
	isTwoOperand = false;
	if ((temp = strrstr(expression, "sin")) > finaly)
		finaly = temp, action = sin, lenOperator = 3;
	if ((temp = strrstr(expression, "cos")) > finaly) 
		finaly = temp, action = cos, lenOperator = 3;
	if ((temp = strrstr(expression, "tan")) > finaly) 
		finaly = temp, action = tan, lenOperator = 3;
	if ((temp = strrstr(expression, "cot")) > finaly) 
		finaly = temp, action = cot, lenOperator = 3;
	if ((temp = strrstr(expression, "sec")) > finaly) 
		finaly = temp, action = sec, lenOperator = 3;
	if ((temp = strrstr(expression, "csc")) > finaly) 
		finaly = temp, action = csc, lenOperator = 3;
	if ((temp = strrstr(expression, "asin")) > finaly) 
		finaly = temp, action = asin, lenOperator = 4;
	if ((temp = strrstr(expression, "acos")) > finaly) 
		finaly = temp, action = acos, lenOperator = 4;
	if ((temp = strrstr(expression, "atan")) > finaly) 
		finaly = temp, action = atan, lenOperator = 4;
	if ((temp = strrstr(expression, "acot")) > finaly) 
		finaly = temp, action = acot, lenOperator = 4;
	if ((temp = strrstr(expression, "asec")) > finaly) 
		finaly = temp, action = asec, lenOperator = 4;
	if ((temp = strrstr(expression, "acsc")) > finaly) 
		finaly = temp, action = acsc, lenOperator = 4;
	if ((temp = strrstr(expression, "sgn")) > finaly) 
		finaly = temp, action = sgn, lenOperator = 3;
	puts("klsdsdsd");
	return finaly;

}


union Data {mpfr_t number; ArifmeticAction action;};
class Expression {
private:
	Expression *_parent, *_operand1, *_operand2;
	Data _data;
	bool _isNumber;


	static size_t size;
	Expression (
		const char *expression, \
		Expression * parent, \
		bool isDelete = true \
	) : _isNumber{true}, _parent{parent} {
		puts("jkk");
		puts(expression);
		mpfr_init2(_data.number, size);
		mpfr_set_str(_data.number, expression, 10, MPFR_RNDN);
		puts("ndh");
		if (isDelete) delete [] expression;
	}
	Expression(ArifmeticAction action, \
		Expression *parent = nullptr)
		: _isNumber{false}, _parent{parent}, \
		_data{.action = action} {}
public:
	static Expression *buildExpressionTree( \
		const char *expression, \
		Expression * parent = 0L, \
		bool isDelete = true \
	) {
		puts(expression);
		unsigned char lenOperator;
		bool isTwoOperand;
		ArifmeticAction action;
		const char * ptrOperator = \
			_shearchNotPriorityOperator(expression, \
				isTwoOperand, lenOperator, action);

		if (ptrOperator)
			putchar(*ptrOperator);
		puts("dfsfgv");
		if (!ptrOperator) {
			puts("kl");
			//if (isDelete) delete [] expression;
			puts("kl");
			return new Expression{expression, parent, isDelete};
		}
		puts("nm");
		Expression * result = new Expression{action, parent};
		puts("nm");
		if (isTwoOperand) {
			size_t lenExpression{(size_t)(ptrOperator - expression)};
			char * newExpression = new char[lenExpression + 1];
			strncpy(newExpression, expression, lenExpression);
			result->setFirstOperand(buildExpressionTree(newExpression, result, false));
			puts("klkdd");
			lenExpression = ptrOperator - expression + lenOperator;
			newExpression = (char *)realloc(newExpression, lenExpression + 1);
			strncpy(newExpression, ptrOperator + lenOperator, lenExpression);
			result->setSecondOperand(buildExpressionTree(newExpression, result));
		} else {
			size_t lenExpression{(size_t)(ptrOperator - expression + lenOperator)};
			char * const newExpression = new char[lenExpression + 1];
			strncpy(newExpression, ptrOperator + lenOperator, lenExpression);
			result->setFirstOperand(buildExpressionTree(newExpression, result));
			puts("dj");
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
		switch (expression->_data.action) {
			case addition:
				mpfr_add(result, operand1, operand2, MPFR_RNDN);
				break;
			case subtraction:
				mpfr_sub(result, operand1, operand2, MPFR_RNDN);
				break;
			case remainderFromDivision:
				mpfr_fmod(result, operand1, operand2, MPFR_RNDN);
				break;
			case multiplication:
				mpfr_mul(result, operand1, operand2, MPFR_RNDN);
				break;
			case division:
				mpfr_div(result, operand1, operand2, MPFR_RNDN);
				break;
			case sin:
				mpfr_sin(result, operand1, MPFR_RNDN);
				break;
			case cos:
				mpfr_cos(result, operand1, MPFR_RNDN);
				break;
			case tan:
				mpfr_tan(result, operand1, MPFR_RNDN);
				break;
			case sec:
				mpfr_sec(result, operand1, MPFR_RNDN);
				break;
			case csc:
				mpfr_csc(result, operand1, MPFR_RNDN);
				break;
			case cot:
				mpfr_cot(result, operand1, MPFR_RNDN);
				break;
			case asin:
				mpfr_asin(result, operand1, MPFR_RNDN);
				break;
			case acos:
				mpfr_acos(result, operand1, MPFR_RNDN);
				break;
			case atan:
				mpfr_atan(result, operand1, MPFR_RNDN);
				break;
			case asec:
				mpfr_asec(result, operand1, MPFR_RNDN);
				break;
			case acsc:
				mpfr_acsc(result, operand1, MPFR_RNDN);
				break;
			case acot:
				mpfr_acot(result, operand1, MPFR_RNDN);
				break;
			//case sgn:
				//result = mpfr_sgn(operand1);
				//mpfr_printf("Результат: %.50Rf\n", result);
		}
		mpfr_clear(operand1);
		mpfr_clear(operand2);
		puts("skddk");
		delete expression;
		return;
	}
	~Expression() {
		puts("delete Expression");
		if (_isNumber) {
			mpfr_printf("%Rf\n", _data.number);
			mpfr_clear(_data.number);
		}
	}
};

size_t Expression::size = 256;
