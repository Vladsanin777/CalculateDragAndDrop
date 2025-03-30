#include <vector>
//#include "decimal.h"
#include <mpfr.h>


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
	addition, subtraction, \
	remainderFromDivision, \
	multiplication, division, \
	sin, cos, tan, sec, csc, cot, sgn, \
	acos, asin, atan, asec, acsc, acot \
};
union Data {mpfr_t number; ArifmeticAction action;};
class Expression {
private:
	Expression *_parent, *_operand1, *_operand2;
	Data _data;
	bool _isNumber;
public:
	static size_t size;
	Expression(const char *number, bool isDelete = true, \
		Expression *parent = nullptr) 
		: _isNumber(true), _parent(parent), \
	{
		mpfr_t numberDecimal;
		mpfr_init2(numberDecimal, size);
		mpfr_set_str(numberDecimal, number, 10, MPFR_RNDN);
		_data.number = numberDecimal;
		if (isDelete) delete [] number;
	}
	Expression(ArifmeticAction action, \
		Expression *parent = nullptr)
		: _isNumber(false), _parent(parent), \
		_data{.action = action} {}
	void setParent(Expression *parent) {
		_parent = parent;
	}
	void setFirstOperand(Expression *operand1) {
		_operand1 = operand1;
	}
	void setSecondOperand(Expression *operand2) {
		_operand2 = operand2;
	}
	static void calculate(Expression *expression, mpfr_t &result) {
		mpfr_init2(result, size);
		if (expression->_isNumber) {
			const char *str = expression->_data.number;
			mpfr_set_str(result, str, 10, MPFR_RNDN);
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
			case sgn:
				result = mpfr_sgn(operand1);
				mpfr_printf("Результат: %.50Rf\n", result);
		}
		mpfr_clear(operand1);
		mpfr_clear(operand2);
		delete expression;
		return;
	}
	~Expression() {
		if (_isNumber)
			delete [] _data.number;
	}
};

static size_t size = 256;

class ConstructerExpression {
public:
	ConstructerExpression ( \
		const char *expression, \
		bool isDelete = true \
	) {
		_buildExpressionTree(expression);
		if (isDelete) delete [] expression;
	}
private:
	inline bool _isBrackets(char symbol) {
		return symbol == '(' || symbol ')';
	}
	bool _isOperator(const char * const symbol, const char * const start) {
		if (start < symbol) {
			const char symbolBefore = *(symbol - 1UL);
			if (isalpha(symbolBefore) || isdigit(symbolBefore) || \
				_isBrackets(symbolBefore))
				return true;
		}
		return false;
	}
	const char * _shearchNotPriorityOperator(const char * const expression) {
		size_t len = strlen(expression);
		size_t levelBrakets = 0UL;

		for (const char *ptrExpression = expression + len; \
			ptrExpression-- != expression;)
		{
			if (*ptrExpression == ')') levelBrakets++, continue;
			if (*ptrExpression == '(') levelBrakets--, continue;
			if (levelBrakets) continue;
			if (*ptrExpression == '+' || (*ptrExpression == '-' && \
				_isOperator(ptrExpression)))
				return ptrExpression;
			if ()
		}
	}
	Expression *_buildExpressionTree(const char *expression, bool isDelete = true)
	{
		
		if (isDelete) delete [] expression;
	}
};
/*
class CalculateBasic {
	public:
*/
