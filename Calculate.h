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
	remainderFromDivision, power, \
	multiplication, division, \
	log, sin, cos, tan, cot, sec, csc, \
	asin, acos, atan, acot, asec, acsc, \
	sqrt, qrt, cbrt, unaryMinus, _abs, \
	sgn, log2, lg, ln \
};
static const char *ARIFMETIC_STR_ACTION[] { \
	"none", "+", "-", "mod", "^", "*", "/", "log", \
	"sin", "cos", "tan", "cot", "sec", "csc", \
	"asin", "acos", "atan", "acot", "asec", "acsc", \
	"sqrt", "qrt", "cbrt", "-", "abs", "sgn", \
	"log2", "lg", "ln" \
};
enum Diff { \
	numberDi, variableDi, actionVaribleDi \
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
		if ((temp = strrstr(copyExpression, "lg")) < ptr && !ptr)
			lenOperator = 2, action = lg, ptr = temp;
		if ((temp = strrstr(copyExpression, "ln")) < ptr && !ptr)
			lenOperator = 2, action = ln, ptr = temp;
		if ((temp = strrstr(copyExpression, "abs")) < ptr && !ptr)
			lenOperator = 3, action = _abs, ptr = temp;
		if ((temp = strrstr(copyExpression, "sgn")) < ptr && !ptr)
			lenOperator = 3, action = sgn, ptr = temp;
		if ((temp = strrstr(copyExpression, "log")) < ptr && !ptr)
			lenOperator = 3, action = log, ptr = temp, \
				isTwoOperand = true;
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

enum TypeData { \
	numberTD, variableTD, actionTD \
};
union Data { \
	const char * variable; \
	mpfr_t number; \
	ArifmeticAction action; \
};
class Expression {
private:
	Expression *_parent, *_operand1, *_operand2;
	Data _data;
	TypeData _typeData;

	static size_t size;
	static Expression *ZERO, *ONE, *TEN;
	inline explicit Expression (
		const char * const number, \
		Expression * parent = nullptr, \
		bool isDelete = true \
	) : _parent{parent} {
		//puts("jkk");
		//puts(expression);
		if (isdigit(*number) || *number == '-') {
			_typeData = numberTD;
			mpfr_init2(_data.number, size);
			mpfr_set_str(_data.number, number, 10, MPFR_RNDN);
			puts("ndh");
			puts(number);
		} else {
			puts("variableTD");
			puts(number);
			_typeData = variableTD;
			char * variable = new char[strlen(number)+1UL];
			strcpy(variable, number);
			_data.variable = variable;
		}
		if (isDelete) delete [] number;
	}
	inline explicit Expression(ArifmeticAction action, \
		Expression *parent = nullptr)
		: _typeData{actionTD}, _parent{parent}, \
		_data{.action = action} {}
	inline explicit Expression(void) {}
	mpfr_t* calculate(void) {
		mpfr_t* result {new mpfr_t[1]};
		mpfr_init2(*result, size);
		if (_typeData == variableTD) {
			mpfr_set_str(*result, "6", 10, MPFR_RNDN);
			return result;
		}
		if (_typeData == numberTD) {
			mpfr_set(*result, _data.number, MPFR_RNDN);
			return result;
		}
		mpfr_t* operand1 = _operand1->calculate(), *operand2;
		ArifmeticAction arifmeticAction = _data.action;
		bool isTwoOperandBool {isTwoOperand()};
		if (isTwoOperandBool)
			operand2 = _operand2->calculate();
		switch (arifmeticAction) {
			case addition:
				mpfr_add(*result, *operand1, *operand2, MPFR_RNDN);
				puts("add");
				break;
			case subtraction:
				mpfr_sub(*result, *operand1, *operand2, MPFR_RNDN);
				puts("sub");
				break;
			case remainderFromDivision:
				mpfr_fmod(*result, *operand1, *operand2, MPFR_RNDN);
				puts("fmod");
				break;
			case multiplication:
				mpfr_mul(*result, *operand1, *operand2, MPFR_RNDN);
				puts("mul");
				break;
			case division:
				mpfr_div(*result, *operand1, *operand2, MPFR_RNDN);
				puts("div");
				break;
			case unaryMinus:
				mpfr_sub(*result, ZERO->_data.number, *operand1, MPFR_RNDN);
				puts("unary minus");
				break;
			case sin:
				mpfr_sin(*result, *operand1, MPFR_RNDN);
				puts("sin");
				break;
			case cos:
				mpfr_cos(*result, *operand1, MPFR_RNDN);
				puts("cos");
				break;
			case tan:
				mpfr_tan(*result, *operand1, MPFR_RNDN);
				puts("tan");
				break;
			case sec:
				mpfr_sec(*result, *operand1, MPFR_RNDN);
				puts("sec");
				break;
			case csc:
				mpfr_csc(*result, *operand1, MPFR_RNDN);
				puts("csc");
				break;
			case cot:
				mpfr_cot(*result, *operand1, MPFR_RNDN);
				puts("cot");
				break;
			case asin:
				mpfr_asin(*result, *operand1, MPFR_RNDN);
				puts("asin");
				break;
			case acos:
				mpfr_acos(*result, *operand1, MPFR_RNDN);
				puts("acos");
				break;
			case atan:
				mpfr_atan(*result, *operand1, MPFR_RNDN);
				puts("atan");
				break;
			case asec:
				mpfr_asec(*result, *operand1, MPFR_RNDN);
				puts("asec");
				break;
			case acsc:
				mpfr_acsc(*result, *operand1, MPFR_RNDN);
				puts("acsc");
				break;
			case acot:
				mpfr_acot(*result, *operand1, MPFR_RNDN);
				puts("acot");
				break;
			//case sgn:
				//result = mpfr_sgn(operand1);
				//puts("sgn");
				//mpfr_printf("Результат: %.50Rf\n", result);
		}
		mpfr_printf("1 %Rf\nr %Rf\n", *operand1, *result);
		mpfr_clear(*operand1);
		delete [] operand1;
		if (isTwoOperandBool) {
			mpfr_clear(*operand2);
			delete [] operand2;
		}
		//puts("skddk");
		return result;
	}
public:
	const char *calc(void) {
		char *resStr = new char[100];
		mpfr_t *result = calculate();
		mpfr_sprintf(resStr, "%.77Rf", *result);
		mpfr_clear(*result);
		delete [] result;
		std::cout << (void*)resStr << std::endl;
		return resStr;
	}
	static void init(void) {
		if (ZERO) delete ZERO;
		if (ONE) delete ONE;
		if (TEN) delete TEN;
		ZERO = new Expression{"0", nullptr, false}, \
		ONE = new Expression{"1", nullptr, false}, \
		TEN = new Expression{"10", nullptr, false};
	}
	inline bool isTwoOperand(void) const {
		return _data.action < sin;
	}
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
	inline const char *print(void) {
		char * expression;
		switch (_typeData){
			case numberTD:
				puts("numberTD");
				expression = new char[12];
				mpfr_sprintf(expression, "%05.5Rf", _data.number);
				return expression;
			case variableTD:
				puts("variableTD");
				expression = new char[strlen(_data.variable) + 1UL];
				strcpy(expression, _data.variable);
				return expression;
		}
		puts("actionTD");
		puts(ARIFMETIC_STR_ACTION[_data.action]);
		//if (isTwoOperand()) puts(_operand2->print());
		const char *operand1 {_operand1->print()}, \
			*action {ARIFMETIC_STR_ACTION[_data.action]};
		if (isTwoOperand()) {
			const char *operand2 {_operand2->print()};
			expression = new char[strlen(operand1) + \
				strlen(action) + strlen(operand2) + 1UL];
			strcpy(expression, operand1);
			strcat(expression, action);
			strcat(expression, operand2);
			delete [] operand2;
		} else {
			expression = new char[strlen(action) + strlen(operand1) + 1UL];
			strcpy(expression, action);
			strcat(expression, operand1);
		}
		delete [] operand1;
		return expression;
	}

	inline Expression * copy( \
		Expression * parent = 0L \
	) const {
		Expression *result {new Expression{}};
		result->_parent = parent;
		char * temp = 0L;
		switch (_typeData) {
			case numberTD:
				mpfr_init2(result->_data.number, size);
				mpfr_set(result->_data.number, _data.number, MPFR_RNDN);
				result->_typeData = numberTD;
				return result;
			case variableTD:
				//std::cout << "fvhfdnk " << _data.variable << ' ' << strlen(_data.variable) + 1UL << std::endl;
				temp = new char[strlen(_data.variable) + 1UL];
				strcpy(temp, _data.variable);
				result->_data.variable = temp;
				result->_typeData = variableTD;
				return result;
			case actionTD:
				result->_data.action = _data.action;
				result->_operand1 = _operand1->copy(result);
				if (isTwoOperand())
					result->_operand2 = _operand2->copy(result);
				result->_typeData = actionTD;
		}
		return result;
	}
	inline Diff heandlerDiff(void) const {
		switch (_typeData) {
			case numberTD:
				//puts("svfjsjk");
				return numberDi;
			case variableTD:
				return variableDi;
		}
		Diff diff {_operand1->heandlerDiff()};
		if (diff != numberDi) return actionVaribleDi;
		if (isTwoOperand())
			if ((diff = _operand2->heandlerDiff()) == variableDi) 
				return actionVaribleDi;
		return diff;
	}
	inline Expression * diff(Expression * parent = 0L) const {
		switch (heandlerDiff()) {
			case numberDi:
				//puts("numberDi");
				return ZERO->copy(parent);
			case variableDi:
				//puts("variableDi");
				return ONE->copy(parent);
		}
		Expression * result {new Expression{none, parent}};
		result->_operand1 = nullptr;
		result->_operand2 = nullptr;
		Diff operand1Di {_operand1->heandlerDiff()}, operand2Di;
		if (_operand2) operand2Di = _operand2->heandlerDiff();
		const ArifmeticAction &action {_data.action};
		ArifmeticAction &resAction{result->_data.action};
		Expression * &resOperand1 {result->_operand1}, \
			* &resOperand2 {result->_operand2};
		Expression * resOperand1Operand1, \
			* resOperand1Operand2, \
			* resOperand2Operand2, \
			* resOperand2Operand1;
        switch (action) {
			case addition:
			case subtraction:
				//puts("subAdd");
				if (operand1Di == numberDi) {
					//puts("asd");
					delete result;
					//puts("asd");
					result = _operand2->diff(parent);
				} else if (operand2Di == numberDi) {
					//puts("asd");
					delete result;
					//puts("asd");
					result = _operand1->diff(parent);
				} else {
					resAction = action;
					resOperand1 = _operand1->diff(result);
					resOperand2 = _operand2->diff(result);
				}
				return result;
			case multiplication:
				std::cout << operand1Di << ' ' << operand2Di << std::endl;
				puts("sfjkvjk");
				if (operand1Di == numberDi || operand2Di == numberDi) {
					delete result;
					result = ZERO->copy(parent);
				} else if (operand1Di == variableDi) {
					delete result;
					result = _operand2->diff(parent);
				} else if (operand2Di == variableDi) {
					//puts("operand2");
					delete result;
					result = _operand1->diff(parent);
				} else {
					resAction = addition;
					resOperand1 = new Expression{multiplication, result};
					resOperand1->_operand1 = _operand1->diff(resOperand1);
					resOperand1->_operand2 = _operand2->copy(resOperand1);
					resOperand2 = new Expression{multiplication, result};
					resOperand2->_operand1 = _operand1->copy(resOperand2);
					resOperand2->_operand2 = _operand2->diff(resOperand2);
				}
				return result;
			case division:
				resAction = division;
				resOperand1 = new Expression{addition, result};
				resOperand1Operand1 = \
					new Expression {multiplication, resOperand1}, \
				resOperand1Operand2 = \
					new Expression {multiplication, resOperand1};
				resOperand1Operand1->_operand1 = \
					_operand1->diff(resOperand1Operand1);
				resOperand1Operand1->_operand2 = \
					_operand2->copy(resOperand1Operand1);
				resOperand1->_operand1 = resOperand1Operand1;
				resOperand1Operand2->_operand1 = 
					_operand1->copy(resOperand1Operand2);
				resOperand1Operand2->_operand2 = 
					_operand2->diff(resOperand1Operand2);
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand2 = new Expression{qrt, result};
				resOperand2->_operand1 = _operand2->copy(resOperand2);
				return result;
			case power:
				resAction = multiplication;
				resOperand1 = _operand2->copy(resOperand1);
				resOperand2 = new Expression{power, result};
				resOperand2->_operand1 = \
					_operand1->copy(resOperand2);
				resOperand2Operand2 = \
					new Expression{subtraction, resOperand2};
				resOperand2Operand2->_operand1 = \
					_operand2->copy(resOperand2Operand2);
				resOperand2Operand2->_operand2 = \
					ONE->copy(resOperand2Operand2);
				break;
			case log:
				resAction = division;
				resOperand1 = ONE->copy(result);
				resOperand2 = new Expression{multiplication, result};
				resOperand2->_operand1 = _operand1->copy(resOperand2);
				resOperand2Operand2 = new Expression{ln, resOperand2};
				resOperand2->_operand2 = resOperand2Operand2;
				resOperand2Operand2->_operand1 = \
					_operand2->copy(resOperand2Operand2);
				break;
			case ln:
				resAction = division;
				resOperand1 = ONE->copy(result);
				resOperand2 = _operand1->copy(result);
				break;
			case lg:
				resAction = division;
				resOperand1 = ONE->copy(result);
				resOperand2 = new Expression{multiplication, result};
				resOperand2->_operand1 = _operand1->copy(resOperand2);
				resOperand2Operand2 = new Expression{ln, resOperand2};
				resOperand2->_operand2 = resOperand2Operand2;
				resOperand2Operand2->_operand1 = \
					TEN->copy(resOperand2Operand2);
				break;
			case sin:
				resAction = cos;
				resOperand1 = _operand1->copy();
				if (operand1Di == variableDi) return result;
				break;
			case cos:
				resAction = unaryMinus;
				resOperand1 = new Expression{sin, result};
				resOperand1->_operand1 = _operand1->copy();
				if (operand1Di == variableDi) return result;
				break;
			case tan:
				resAction = power;
				resOperand1 = new Expression{sec, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				if (operand1Di == variableDi) return result;
				break;
			case cot:
				resAction = power;
				resOperand1 = new Expression{unaryMinus, result};
				resOperand1Operand1 = new Expression{csc, resOperand1};
				resOperand1Operand1->_operand1 = _operand1->copy(resOperand1Operand1);
				resOperand1->_operand1 = resOperand1Operand1;
				if (operand1Di == variableDi) return result;
				break;
			case sec:
				resAction = multiplication;
				resOperand1 = new Expression{sec, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				resOperand2 = new Expression{tan, result};
				resOperand2->_operand1 = _operand1->copy(resOperand2);
				if (operand1Di == variableDi) return result;
				break;
			case csc:
				resAction = multiplication;
				resOperand1 = new Expression{unaryMinus, result};
				resOperand1Operand1 = new Expression{csc, resOperand1};
				resOperand1Operand1->_operand1 = _operand1->copy(resOperand1Operand1);
				resOperand2 = new Expression{cot, result};
				resOperand2->_operand1 = _operand1->copy(resOperand2);
				if (operand1Di == variableDi) return result;
				break;
			case _abs:
				resAction = sgn;
				resOperand1 = _operand1->copy();
				if (operand1Di == variableDi) return result;
				break;

			case asin:
				resAction = division;
				resOperand1 = ONE->copy();
				resOperand2 = new Expression{sqrt, result};
				resOperand2Operand1 = \
					new Expression{subtraction, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{qrt, resOperand2Operand1};
				resOperand2Operand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
			case acos:
				resAction = unaryMinus;
				resOperand1 = new Expression{division, result};
				resOperand1->_operand1 = ONE->copy(resOperand1);
				resOperand1Operand2 = new Expression{sqrt, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand2Operand1 = \
					new Expression{subtraction, resOperand1Operand2};
				resOperand1Operand2->_operand1 = \
					resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{qrt, resOperand2Operand1};
				resOperand2Operand1->_operand2 = \
					resOperand1Operand2;
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
			case atan:
				resAction = division;
				resOperand1 = ONE->copy();
				resOperand2 = new Expression{sqrt, result};
				resOperand2Operand1 = \
					new Expression{addition, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{qrt, resOperand2Operand1};
				resOperand2Operand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
				
			case acot:
				resAction = unaryMinus;
				resOperand1 = new Expression{division, result};
				resOperand1->_operand1 = ONE->copy(resOperand1);
				resOperand1Operand2 = new Expression{sqrt, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand2Operand1 = \
					new Expression{addition, resOperand1Operand2};
				resOperand1Operand2->_operand1 = \
					resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{qrt, resOperand2Operand1};
				resOperand2Operand1->_operand2 = \
					resOperand1Operand2;
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
			case asec:
				resAction = division;
				resOperand1 = ONE->copy(result);
				resOperand2 = new Expression{multiplication, result};
				resOperand2Operand1 = \
					new Expression{_abs, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					_operand1->copy(resOperand2Operand1);
				resOperand2Operand2 = \
					new Expression{sqrt, resOperand2};
				resOperand2->_operand2 = resOperand2Operand2;
				resOperand2Operand1 = \
					new Expression{subtraction, resOperand2Operand2};
				resOperand2Operand2->_operand1 = \
					resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{qrt, resOperand2Operand1};
				resOperand2Operand1->_operand2 = \
					resOperand1Operand2;
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
			case acsc:
				resAction = unaryMinus;
				resOperand1 = new Expression{division, result};
				resOperand1->_operand1 = ONE->copy(result);
				resOperand1Operand2 = new Expression{multiplication, result};
				resOperand2Operand1 = new Expression{_abs, resOperand2};
				resOperand1Operand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					_operand1->copy(resOperand2Operand1);
				resOperand2Operand2 = \
					new Expression{sqrt, resOperand1Operand2};
				resOperand1Operand2->_operand2 = resOperand2Operand2;
				resOperand2Operand1 = \
					new Expression{subtraction, resOperand2Operand2};
				resOperand2Operand2->_operand1 = \
					resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{qrt, resOperand2Operand1};
				resOperand2Operand1->_operand2 = \
					resOperand1Operand2;
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
    
			case sgn:
				result->_typeData = numberTD;
				result = ZERO->copy(parent);
				return result;
		}
		Expression *mulOnDiffResult {new Expression {multiplication, parent}};
		mulOnDiffResult->_operand1 = result;
		result->_parent = mulOnDiffResult;
		mulOnDiffResult->_operand2 = _operand1->diff(mulOnDiffResult);
		return mulOnDiffResult;
	}
	~Expression() {
		//puts("delete Expression");
		switch (_typeData) {
			case numberTD:
				mpfr_clear(_data.number);
				return;
			case variableTD:
				delete [] _data.variable;
				return;
			case actionTD:
				if (_operand1) delete _operand1;
				if (isTwoOperand()) {
					if (_operand2) delete _operand2;
				}
				return;
		}
	}
};

size_t Expression::size = 256;
Expression * Expression::ZERO, \
	* Expression::ONE, \
	* Expression::TEN;

