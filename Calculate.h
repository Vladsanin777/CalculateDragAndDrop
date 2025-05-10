#include <stack>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mpfr.h>
#include <unordered_map>

using Error = std::pair<size_t, const char *>;

static const std::unordered_map<char, char> \
	bracketsIsValid {
	{')', '('},
	{'}', '{'},
	{']', '['}
};

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
	OpNone, OpAddition, OpSubtraction, \
	OpRemainderFromDivision, OpPower, \
	OpMultiplication, OpDivision, \
	OpLog, OpSin, OpCos, OpTan, OpCot, OpSec, OpCsc, \
	OpAsin, OpAcos, OpAtan, OpAcot, OpAsec, OpAcsc, \
	OpSqrt, OpSqr, OpCbrt, OpUnaryMinus, OpAbs, \
	OpSgn, OpLg, OpLn, OpExp \
};
static const char *ARIFMETIC_STR_ACTION[] { \
	"none", "+", "-", "mod", "^", "*", "/", "log", \
	"sin", "cos", "tan", "cot", "sec", "csc", \
	"asin", "acos", "atan", "acot", "asec", "acsc", \
	"sqrt", "sqr", "cbrt", "-", "abs", "sgn", \
	"lg", "ln", "exp" \
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
	const char * const expression, unsigned char &lenOperator, \
	ArifmeticAction &action \
) {
	action = OpNone, lenOperator = 0;
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
		if (ptr = strrstr(copyExpression, "+")) {
			lenOperator = 1, action = OpAddition; break;
		}
		if (ptr = strrstr(copyExpression, "-")) {
			lenOperator = 1;
			if (_isOperatorSub(ptr, expression)) 
				action = OpSubtraction;
			else action = OpUnaryMinus;
			break;
		}
		if (ptr = strrstr(copyExpression, "*")) {
			lenOperator = 1, action = OpMultiplication; break;
		}
		if (ptr = strrstr(copyExpression, "/")) {
			lenOperator = 1, action = OpDivision; break;
		}
		if (ptr = strrstr(copyExpression, "mod")) {
			lenOperator = 3, action = OpRemainderFromDivision; break;
		}
		if ((temp = strrstr(copyExpression, "sin")) < ptr != !ptr)
			lenOperator = 3, action = OpSin, ptr = temp; 
		if ((temp = strrstr(copyExpression, "cos")) < ptr != !ptr)
			lenOperator = 3, action = OpCos, ptr = temp; 
		if ((temp = strrstr(copyExpression, "tan")) < ptr && !ptr)
			lenOperator = 3, action = OpTan, ptr = temp; 
		if ((temp = strrstr(copyExpression, "cot")) < ptr && !ptr)
			lenOperator = 3, action = OpCot, ptr = temp; 
		if ((temp = strrstr(copyExpression, "sec")) < ptr && !ptr)
			lenOperator = 3, action = OpSec, ptr = temp; 
		if ((temp = strrstr(copyExpression, "csc")) < ptr && !ptr)
			lenOperator = 3, action = OpCsc, ptr = temp; 
		if ((temp = strrstr(copyExpression, "asin")) < ptr && !ptr)
			lenOperator = 4, action = OpAsin, ptr = temp; 
		if ((temp = strrstr(copyExpression, "acos")) < ptr && !ptr)
			lenOperator = 4, action = OpAcos, ptr = temp; 
		if ((temp = strrstr(copyExpression, "atan")) < ptr && !ptr)
			lenOperator = 4, action = OpAtan, ptr = temp; 
		if ((temp = strrstr(copyExpression, "acot")) < ptr && !ptr)
			lenOperator = 4, action = OpAcot, ptr = temp; 
		if ((temp = strrstr(copyExpression, "asec")) < ptr && !ptr)
			lenOperator = 4, action = OpAsec, ptr = temp; 
		if ((temp = strrstr(copyExpression, "acsc")) < ptr && !ptr)
			lenOperator = 4, action = OpAcsc, ptr = temp;
		if ((temp = strrstr(copyExpression, "lg")) < ptr && !ptr)
			lenOperator = 2, action = OpLg, ptr = temp;
		if ((temp = strrstr(copyExpression, "ln")) < ptr && !ptr)
			lenOperator = 2, action = OpLn, ptr = temp;
		if ((temp = strrstr(copyExpression, "abs")) < ptr && !ptr)
			lenOperator = 3, action = OpAbs, ptr = temp;
		if ((temp = strrstr(copyExpression, "sgn")) < ptr && !ptr)
			lenOperator = 3, action = OpSgn, ptr = temp;
		if ((temp = strrstr(copyExpression, "log")) < ptr && !ptr)
			lenOperator = 3, action = OpLog, ptr = temp;
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
	static Expression *ZERO, *ONE, *TWO, *TEN, *VAR_X, *MINUS_ONE;
	inline explicit Expression (
		const char * const number, \
		Expression * parent = nullptr, \
		bool isDelete = true \
	) : _parent{parent}, _operand1{nullptr}, \
		_operand2{nullptr} {
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
		_data{.action = action}, _operand1{nullptr}, \
		_operand2{nullptr} {}
	inline explicit Expression(void) : _parent{nullptr}, \
	_operand1{nullptr}, _operand2{nullptr} {}
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
		puts(ARIFMETIC_STR_ACTION[arifmeticAction]);
		switch (arifmeticAction) {
			case OpAddition:
				mpfr_add(*result, *operand1, *operand2, MPFR_RNDN);
				break;
			case OpSubtraction:
				mpfr_sub(*result, *operand1, *operand2, MPFR_RNDN);
				break;
			case OpRemainderFromDivision:
				mpfr_fmod(*result, *operand1, *operand2, MPFR_RNDN);
				break;
			case OpMultiplication:
				mpfr_mul(*result, *operand1, *operand2, MPFR_RNDN);
				break;
			case OpDivision:
				mpfr_div(*result, *operand1, *operand2, MPFR_RNDN);
				break;
			case OpUnaryMinus:
				mpfr_sub(*result, ZERO->_data.number, *operand1, MPFR_RNDN);
				break;
			case OpSin:
				mpfr_sin(*result, *operand1, MPFR_RNDN);
				break;
			case OpCos:
				mpfr_cos(*result, *operand1, MPFR_RNDN);
				break;
			case OpTan:
				mpfr_tan(*result, *operand1, MPFR_RNDN);
				break;
			case OpSec:
				mpfr_sec(*result, *operand1, MPFR_RNDN);
				break;
			case OpCsc:
				mpfr_csc(*result, *operand1, MPFR_RNDN);
				break;
			case OpCot:
				mpfr_cot(*result, *operand1, MPFR_RNDN);
				break;
			case OpAsin:
				mpfr_asin(*result, *operand1, MPFR_RNDN);
				break;
			case OpAcos:
				mpfr_acos(*result, *operand1, MPFR_RNDN);
				break;
			case OpAtan:
				mpfr_atan(*result, *operand1, MPFR_RNDN);
				break;
			case OpAsec:
				mpfr_asec(*result, *operand1, MPFR_RNDN);
				break;
			case OpAcsc:
				mpfr_acsc(*result, *operand1, MPFR_RNDN);
				break;
			case OpAcot:
				mpfr_acot(*result, *operand1, MPFR_RNDN);
				break;
			case OpSqr:
				mpfr_sqr(*result, *operand1, MPFR_RNDN);
				break;
			case OpSqrt:
				mpfr_sqrt(*result, *operand1, MPFR_RNDN);
				break;
			case OpCbrt:
				mpfr_cbrt(*result, *operand1, MPFR_RNDN);
				break;
			case OpExp:
				mpfr_exp(*result, *operand1, MPFR_RNDN);
				break;
			case OpLog:
				mpfr_log(*operand1, *operand1, MPFR_RNDN);
				mpfr_log(*operand2, *operand2, MPFR_RNDN);
				mpfr_div(*result, *operand1, *operand2, MPFR_RNDN);
				break;
			case OpLn:
				mpfr_log(*result, *operand1, MPFR_RNDN);
				break;
			case OpLg:
				mpfr_log10(*result, *operand1, MPFR_RNDN);
				break;
			//case sgn:
				//result = mpfr_sgn(operand1);
				//puts("sgn");
				//mpfr_printf("Результат: %.50Rf\n", result);
		}
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
		char * const resStr{new char[100]{0}};
		mpfr_t * const result{calculate()};
		mpfr_sprintf(resStr, "%.77Rf", *result);
		mpfr_clear(*result);
		delete [] result;
		char * ptr{resStr + strlen(resStr) - 1UL};
		for (; ptr != resStr; ptr--)
			if (*ptr != '0') break;
		if (*ptr == '.') *ptr = '\0';
		else if (strchr(resStr, '.')) *++ptr = '\0';
		return (const char *)resStr;
	}
	static void init(void) {
		if (ZERO) delete ZERO;
		if (ONE) delete ONE;
		if (TWO) delete TWO;
		if (TEN) delete TEN;
		if (MINUS_ONE) delete MINUS_ONE;
		ZERO = new Expression{"0", nullptr, false}, \
		ONE = new Expression{"1", nullptr, false}, \
		TWO = new Expression{"2", nullptr, false}, \
		TEN = new Expression{"10", nullptr, false}, \
		MINUS_ONE = new Expression{"-1", nullptr, false};
		if (!VAR_X) VAR_X = new Expression{"x", nullptr, false};
	}
	inline bool isTwoOperand(void) const {
		return _data.action < OpSin;
	}
	static Error *isValid(const char * const expression) {
		std::stack<std::pair<char, const char *>> balanceBrackets;
		for (const char * ptr {expression}, \
			* const ptrEnd {expression + strlen(expression)}; \
			ptr != ptrEnd; ptr++) {
			const char symbol {*ptr};
			switch (symbol) {
				case '(':
				case '[':
				case '{':
					balanceBrackets.push({symbol, ptr});
					continue;
				case ')':
				case ']':
				case '}':
					if (balanceBrackets.empty() || \
						balanceBrackets.top().first != \
						bracketsIsValid.at(symbol)) {
						char * exstraBracket {new char[30]{'\0'}};
						size_t index {(size_t)(ptr - expression)};
						sprintf(exstraBracket, \
							"%lu Exstra Bracket '%c'", index, symbol);
						return new Error{index, exstraBracket};
					} else {
						balanceBrackets.pop();
					}
			}
		}
		if (!balanceBrackets.empty()) {
			char *exstraBracket {new char[30]{'\0'}};
			size_t index {(size_t)(balanceBrackets.top().second - expression)};
			sprintf(exstraBracket, "%lu Exstra Bracket '%c'", \
				index, balanceBrackets.top().first);
			return new Error{index, exstraBracket};
		}
		return nullptr;
	}
	static Expression *buildExpressionTree( \
		const char *expression, \
		Expression * parent = 0L, \
		bool isDelete = true \
	) {
		try {
			//puts(expression);
			unsigned char lenOperator;
			bool isTwoOperand;
			ArifmeticAction action;
			const char * ptrOperator = \
				_shearchNotPriorityOperator(expression, \
					lenOperator, action);
			puts(ARIFMETIC_STR_ACTION[action]);
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
			if (action < OpSin) {
				puts("two operator");
				puts(ARIFMETIC_STR_ACTION[action]);
				std::cout << action << std::endl;

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
				puts("one operator");
				size_t lenExpression{ptrOperator - expression + strlen(expression) + lenOperator + 1UL};
				char * const newExpression = new char[lenExpression + 1]();
				strncpy(newExpression, ptrOperator + lenOperator, lenExpression);
				result->setFirstOperand(buildExpressionTree(newExpression, result));
				//puts("dj");
			}
			if (isDelete) delete [] expression;
			return result;
		} catch (...) {
			return 0;
		}
	}
	inline bool operator==(const Expression * operand2) const {
		if (_typeData != operand2->_typeData) return false;
		switch (_typeData) {
			case numberTD:
				{
					const mpfr_t &operand1Num {_data.number}, \
						&operand2Num {operand2->_data.number};
					return mpfr_equal_p(operand1Num, operand2Num);
				}
			case variableTD:
				return true;
			case actionTD:
				if (_data.action != _data.action) return false;
				{
					bool equalOperand1 \
						{_operand1->operator==(operand2->_operand1)};
					if (isTwoOperand()){
						return equalOperand1 == \
							_operand2->operator==(operand2->_operand2);
					}
					return equalOperand1;
				}
		}
		return false;
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
				strlen(action) + strlen(operand2) + 3UL];
			strcpy(expression, "(");
			strcat(expression, operand1);
			strcat(expression, action);
			strcat(expression, operand2);
			strcat(expression, ")");
			delete [] operand2;
		} else {
			expression = new char[strlen(action) + strlen(operand1) + 1UL];
			strcpy(expression, "(");
			strcat(expression, action);
			strcat(expression, operand1);
			strcat(expression, ")");
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
	inline Diff heandlerDifferentiate(void) const {
		switch (_typeData) {
			case numberTD:
				//puts("svfjsjk");
				return numberDi;
			case variableTD:
				return variableDi;
		}
		Diff diff {_operand1->heandlerDifferentiate()};
		if (diff != numberDi) return actionVaribleDi;
		if (isTwoOperand())
			if ((diff = _operand2->heandlerDifferentiate()) == variableDi) 
				return actionVaribleDi;
		return diff;
	}
	inline Expression * differentiate(Expression * parent = 0L) const {
		switch (heandlerDifferentiate()) {
			case numberDi:
				//puts("numberDi");
				return ZERO->copy(parent);
			case variableDi:
				//puts("variableDi");
				return ONE->copy(parent);
		}
		Expression * result {new Expression{OpNone, parent}};
		Diff operand1Di {_operand1->heandlerDifferentiate()}, operand2Di;
		if (_operand2) operand2Di = _operand2->heandlerDifferentiate();
		const ArifmeticAction &action {_data.action};
		ArifmeticAction &resAction{result->_data.action};
		Expression * &resOperand1 {result->_operand1}, \
			* &resOperand2 {result->_operand2};
		Expression * resOperand1Operand1, \
			* resOperand1Operand2, \
			* resOperand2Operand2, \
			* resOperand2Operand1;
        switch (action) {
			case OpAddition:
			case OpSubtraction:
				//puts("subAdd");
				if (operand1Di == numberDi) {
					//puts("asd");
					delete result;
					//puts("asd");
					result = _operand2->differentiate(parent);
				} else if (operand2Di == numberDi) {
					//puts("asd");
					delete result;
					//puts("asd");
					result = _operand1->differentiate(parent);
				} else {
					resAction = action;
					resOperand1 = _operand1->differentiate(result);
					resOperand2 = _operand2->differentiate(result);
				}
				return result;
			case OpMultiplication:
				//std::cout << operand1Di << ' ' << operand2Di << std::endl;
				//puts("sfjkvjk");
				if (operand1Di == numberDi || operand2Di == numberDi) {
					delete result;
					result = ZERO->copy(parent);
				} else if (operand1Di == variableDi) {
					delete result;
					result = _operand2->differentiate(parent);
				} else if (operand2Di == variableDi) {
					//puts("operand2");
					delete result;
					result = _operand1->differentiate(parent);
				} else {
					resAction = OpAddition;
					resOperand1 = new Expression{OpMultiplication, result};
					resOperand1->_operand1 = _operand1->differentiate(resOperand1);
					resOperand1->_operand2 = _operand2->copy(resOperand1);
					resOperand2 = new Expression{OpMultiplication, result};
					resOperand2->_operand1 = _operand1->copy(resOperand2);
					resOperand2->_operand2 = _operand2->differentiate(resOperand2);
				}
				return result;
			case OpDivision:
				resAction = OpDivision;
				resOperand1 = new Expression{OpAddition, result};
				resOperand1Operand1 = \
					new Expression {OpMultiplication, resOperand1}, \
				resOperand1Operand2 = \
					new Expression {OpMultiplication, resOperand1};
				resOperand1Operand1->_operand1 = \
					_operand1->differentiate(resOperand1Operand1);
				resOperand1Operand1->_operand2 = \
					_operand2->copy(resOperand1Operand1);
				resOperand1->_operand1 = resOperand1Operand1;
				resOperand1Operand2->_operand1 = 
					_operand1->copy(resOperand1Operand2);
				resOperand1Operand2->_operand2 = 
					_operand2->differentiate(resOperand1Operand2);
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand2 = new Expression{OpSqr, result};
				resOperand2->_operand1 = _operand2->copy(resOperand2);
				return result;
			case OpPower:
				resAction = OpMultiplication;
				resOperand1 = _operand2->copy(resOperand1);
				resOperand2 = new Expression{OpPower, result};
				resOperand2->_operand1 = \
					_operand1->copy(resOperand2);
				resOperand2Operand2 = \
					new Expression{OpSubtraction, resOperand2};
				resOperand2Operand2->_operand1 = \
					_operand2->copy(resOperand2Operand2);
				resOperand2Operand2->_operand2 = \
					ONE->copy(resOperand2Operand2);
				break;
			case OpLog:
				resAction = OpDivision;
				resOperand1 = ONE->copy(result);
				resOperand2 = new Expression{OpMultiplication, result};
				resOperand2->_operand1 = _operand1->copy(resOperand2);
				resOperand2Operand2 = new Expression{OpLn, resOperand2};
				resOperand2->_operand2 = resOperand2Operand2;
				resOperand2Operand2->_operand1 = \
					_operand2->copy(resOperand2Operand2);
				break;
			case OpLn:
				resAction = OpDivision;
				resOperand1 = ONE->copy(result);
				resOperand2 = _operand1->copy(result);
				break;
			case OpLg:
				resAction = OpDivision;
				resOperand1 = ONE->copy(result);
				resOperand2 = new Expression{OpMultiplication, result};
				resOperand2->_operand1 = _operand1->copy(resOperand2);
				resOperand2Operand2 = new Expression{OpLn, resOperand2};
				resOperand2->_operand2 = resOperand2Operand2;
				resOperand2Operand2->_operand1 = \
					TEN->copy(resOperand2Operand2);
				break;
			case OpExp:
				resAction = OpExp;
				resOperand1 = _operand1->copy();
				break;
			case OpSin:
				resAction = OpCos;
				resOperand1 = _operand1->copy();
				if (operand1Di == variableDi) return result;
				break;
			case OpCos:
				resAction = OpUnaryMinus;
				resOperand1 = new Expression{OpSin, result};
				resOperand1->_operand1 = _operand1->copy();
				if (operand1Di == variableDi) return result;
				break;
			case OpTan:
				resAction = OpPower;
				resOperand1 = new Expression{OpSec, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				if (operand1Di == variableDi) return result;
				break;
			case OpCot:
				resAction = OpPower;
				resOperand1 = new Expression{OpUnaryMinus, result};
				resOperand1Operand1 = new Expression{OpCsc, resOperand1};
				resOperand1Operand1->_operand1 = _operand1->copy(resOperand1Operand1);
				resOperand1->_operand1 = resOperand1Operand1;
				if (operand1Di == variableDi) return result;
				break;
			case OpSec:
				resAction = OpMultiplication;
				resOperand1 = new Expression{OpSec, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				resOperand2 = new Expression{OpTan, result};
				resOperand2->_operand1 = _operand1->copy(resOperand2);
				if (operand1Di == variableDi) return result;
				break;
			case OpCsc:
				resAction = OpMultiplication;
				resOperand1 = new Expression{OpUnaryMinus, result};
				resOperand1Operand1 = new Expression{OpCsc, resOperand1};
				resOperand1Operand1->_operand1 = _operand1->copy(resOperand1Operand1);
				resOperand2 = new Expression{OpCot, result};
				resOperand2->_operand1 = _operand1->copy(resOperand2);
				if (operand1Di == variableDi) return result;
				break;
			case OpAbs:
				resAction = OpSgn;
				resOperand1 = _operand1->copy();
				if (operand1Di == variableDi) return result;
				break;
			case OpUnaryMinus:
				resAction = OpUnaryMinus;
				resOperand1 = _operand1->differentiate();
				return result;
			case OpAsin:
				resAction = OpDivision;
				resOperand1 = ONE->copy();
				resOperand2 = new Expression{OpSqrt, result};
				resOperand2Operand1 = \
					new Expression{OpSubtraction, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{OpSqr, resOperand2Operand1};
				resOperand2Operand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
			case OpAcos:
				resAction = OpUnaryMinus;
				resOperand1 = new Expression{OpDivision, result};
				resOperand1->_operand1 = ONE->copy(resOperand1);
				resOperand1Operand2 = new Expression{OpSqrt, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand2Operand1 = \
					new Expression{OpSubtraction, resOperand1Operand2};
				resOperand1Operand2->_operand1 = \
					resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{OpSqr, resOperand2Operand1};
				resOperand2Operand1->_operand2 = \
					resOperand1Operand2;
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
			case OpAtan:
				resAction = OpDivision;
				resOperand1 = ONE->copy();
				resOperand2 = new Expression{OpSqrt, result};
				resOperand2Operand1 = \
					new Expression{OpAddition, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{OpSqr, resOperand2Operand1};
				resOperand2Operand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
				
			case OpAcot:
				resAction = OpUnaryMinus;
				resOperand1 = new Expression{OpDivision, result};
				resOperand1->_operand1 = ONE->copy(resOperand1);
				resOperand1Operand2 = new Expression{OpSqrt, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand2Operand1 = \
					new Expression{OpAddition, resOperand1Operand2};
				resOperand1Operand2->_operand1 = \
					resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{OpSqr, resOperand2Operand1};
				resOperand2Operand1->_operand2 = \
					resOperand1Operand2;
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
			case OpAsec:
				resAction = OpDivision;
				resOperand1 = ONE->copy(result);
				resOperand2 = new Expression{OpMultiplication, result};
				resOperand2Operand1 = \
					new Expression{OpAbs, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					_operand1->copy(resOperand2Operand1);
				resOperand2Operand2 = \
					new Expression{OpSqrt, resOperand2};
				resOperand2->_operand2 = resOperand2Operand2;
				resOperand2Operand1 = \
					new Expression{OpSubtraction, resOperand2Operand2};
				resOperand2Operand2->_operand1 = \
					resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{OpSqr, resOperand2Operand1};
				resOperand2Operand1->_operand2 = \
					resOperand1Operand2;
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
			case OpAcsc:
				resAction = OpUnaryMinus;
				resOperand1 = new Expression{OpDivision, result};
				resOperand1->_operand1 = ONE->copy(result);
				resOperand1Operand2 = new Expression{OpMultiplication, result};
				resOperand2Operand1 = new Expression{OpAbs, resOperand2};
				resOperand1Operand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					_operand1->copy(resOperand2Operand1);
				resOperand2Operand2 = \
					new Expression{OpSqrt, resOperand1Operand2};
				resOperand1Operand2->_operand2 = resOperand2Operand2;
				resOperand2Operand1 = \
					new Expression{OpSubtraction, resOperand2Operand2};
				resOperand2Operand2->_operand1 = \
					resOperand2Operand1;
				resOperand2Operand1->_operand1 = \
					ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = \
					new Expression{OpSqr, resOperand2Operand1};
				resOperand2Operand1->_operand2 = \
					resOperand1Operand2;
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				if (operand1Di == variableDi) return result;
				break;
    
			case OpSgn:
				result->_typeData = numberTD;
				result = ZERO->copy(parent);
				return result;
		}
		Expression *mulOnDiffResult {new Expression {OpMultiplication, parent}};
		mulOnDiffResult->_operand1 = result;
		result->_parent = mulOnDiffResult;
		mulOnDiffResult->_operand2 = _operand1->differentiate(mulOnDiffResult);
		return mulOnDiffResult;
	}
	inline Expression *integrate(Expression * parent = 0L) const {
		Expression *result {new Expression{OpNone, parent}};
		const ArifmeticAction &action {_data.action};
		ArifmeticAction &resAction{result->_data.action};
		Expression * &resOperand1 {result->_operand1}, \
			* &resOperand2 {result->_operand2};
		switch (heandlerDifferentiate()) {
			case numberDi:
				resAction = OpMultiplication;
				resOperand1 = this->copy(result);
				resOperand2 = VAR_X;
				return result;
		}
		Diff operand1Di {_operand1->heandlerDifferentiate()}, operand2Di;
		if (_operand2) operand2Di = _operand2->heandlerDifferentiate();
		Expression * resOperand1Operand1, \
			* resOperand1Operand2, \
			* resOperand2Operand2, \
			* resOperand2Operand1;
		switch (action) {
			case OpAddition:
			case OpSubtraction:
				resAction = action;
				resOperand1 = _operand1->integrate(result);
				resOperand2 = _operand2->integrate(result);
				return result;
			case OpMultiplication:
				resAction = OpMultiplication;
				// Попробовать упростить (если один из операндов — константа)
				if (operand1Di == numberDi) {
					resOperand1 = _operand1->copy(result);  // Константа выносится за знак интеграла
					resOperand2 = _operand2->integrate(result);
				}
				else if (operand2Di == numberDi) {
					resOperand1 = _operand1->integrate(result);
					resOperand2 = _operand2->copy(result);
				}
				// Иначе применить интегрирование по частям: ∫u dv = uv - ∫v du
				else {
					resAction = OpSubtraction;
					resOperand1 = new Expression{OpMultiplication, result};
					resOperand1->_operand1 = _operand1->copy(resOperand1);
					resOperand1->_operand2 = _operand2->integrate(resOperand1);
					resOperand2 = new Expression{OpMultiplication, result};
					resOperand2->_operand1 = _operand2->integrate(resOperand2);
					resOperand2->_operand2 = _operand1->differentiate(resOperand2);
					resOperand2Operand2 = resOperand2->integrate(result);  // Вторая часть: ∫v du
					delete resOperand2;
					resOperand2 = resOperand2Operand2;

				}
				return result;

			
			case OpDivision:  // Деление: ∫(u / v) dx → зависит от формы u и v
				resAction = OpDivision;
				// Если знаменатель — константа
				if (operand2Di == numberDi) {
					resOperand1 = _operand1->integrate(result);
					resOperand2 = _operand2->copy(result);
					return result;
				}
				// Случай 2: Числитель — производная знаменателя → ∫(f'/f) dx = ln|f| + C
				{
					Expression *operand2Diff = _operand2->differentiate();
					if (_operand1->operator==(_operand2->differentiate(result))) {
						resAction = OpLn;  // ln(f)
						resOperand1 = _operand2->copy(result);   // аргумент логарифма
						delete operand2Diff;
						return result;
					}
					delete operand2Diff;
				}
				/*
				// Случай 3: Рациональная дробь (многочлен / многочлен) → разложение на простые дроби
				if (num.isPolynomial() && denom.isPolynomial()) {
					// Проверяем степень числителя и знаменателя
					if (num.degree() >= denom.degree()) {
						// Делим многочлены (например, (x^2+1)/(x+1) → x - 1 + 2/(x+1))
						Expression quotient = num.polynomialDivide(denom);
						Expression remainder = num.polynomialRemainder(denom);
						// ∫(quotient + remainder/denom) dx = ∫quotient dx + ∫(remainder/denom) dx
						resAction = addition;
						resOperand1 = quotient.integrate(parent);
						resOperand2 = (remainder / denom).integrate(parent);
						return result;
					} else {
						// Разложение знаменателя на множители и метод неопределённых коэффициентов
						Expression partialFractions = denom.partialFractionDecomposition();
						return partialFractions.integrate(parent);
					}
				}

				// Случай 4: Тригонометрические подстановки (например, ∫1/(x^2 + a^2) dx)
				if (denom.isQuadraticForm()) {
					Expression a = denom.extractCoefficient(); // например, для x^2 + a^2
					resAction = arctangent;                    // (1/a) arctan(x/a)
					resOperand1 = num / a;                     // коэффициент
					resOperand2 = denom.symbol() / a;          // аргумент arctan
					return result;
				}

				// Случай 5: Подстановка (u-подстановка)
				Expression substitution = denom.findSubstitution();
				if (substitution.isValid()) {
					Expression u = substitution;
					Expression du = u.differentiate(parent);
					// Замена: ∫(num / denom) dx = ∫(num / u) * (dx/du) du
					resAction = substitution;
					resOperand1 = (num / u).substitute(u, du).integrate(parent);
					return result;
				}
				*/
			case OpPower:
				if (_operand2->heandlerDifferentiate() != numberDi)
					throw "Not Constant Two Operand Power Integrate";
				if (_operand2->operator==(MINUS_ONE)) {
					resAction = OpLn;
					resOperand1 = _operand1->copy();
					return result;
				}
				resAction = OpDivision;
				resOperand2 = new Expression{OpAddition, result};
				resOperand2->_operand1 = _operand2->copy(resOperand2);
				resOperand2->_operand2 = ONE->copy(resOperand2);
				resOperand1 = new Expression{OpPower, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				resOperand1->_operand2 = _operand2->copy(resOperand2);
				return result;
			case OpAbs:
				resAction = OpDivision;
				resOperand2 = TWO->copy(result);
				resOperand1 = new Expression{OpMultiplication, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				resOperand1Operand2 = new Expression{OpAbs, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				return result;
			case OpLog:
				resAction = OpSubtraction;
				resOperand1 = new Expression{OpMultiplication, result};
				resOperand1->_operand1 = _operand2->copy(resOperand1);
				resOperand1Operand2 = new Expression{OpLog, result};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				resOperand1Operand2->_operand2 = \
					_operand2->copy(resOperand1Operand2);
				resOperand2 = new Expression{OpDivision, result};
				resOperand2->_operand1 = _operand2->copy(resOperand2);
				resOperand2Operand2 = new Expression{OpLn, resOperand2};
				resOperand2->_operand2 = resOperand2Operand2;
				resOperand2Operand2->_operand1 = \
					_operand1->copy(resOperand2Operand2);
				return result;
			case OpLn:
				resAction = OpSubtraction;
				resOperand2 = _operand1->copy(result);
				resOperand1 = new Expression{OpMultiplication, result};
				resOperand1->_operand1 = _operand1->copy(result);
				resOperand1Operand2 = new Expression{OpLn, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				return result;
			case OpLg:
				resAction = OpSubtraction;
				resOperand1 = new Expression{OpMultiplication, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				resOperand1Operand2 = new Expression{OpLg, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				resOperand2 = new Expression{OpDivision, result};
				resOperand2->_operand1 = _operand1->copy(resOperand2);
				resOperand2Operand2 = new Expression{OpLg, resOperand2};
				resOperand2->_operand2 = resOperand2Operand2;
				resOperand2Operand2->_operand1 = _operand1->copy(resOperand2Operand2);
				return result;
			case OpUnaryMinus:
				resAction = OpUnaryMinus;
				resOperand1 = _operand1->integrate();
				return result;
			case OpExp:
				throw std::runtime_error("exp integrate");
				return result;
			case OpSin:
				resAction = OpUnaryMinus;
				resOperand1 = new Expression{OpCos, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				return result;
			case OpCos:
				resAction = OpSin;
				resOperand1 = _operand1->copy(result);
				return result;
			case OpTan:
				resAction = OpLn;
				resOperand1 = new Expression{OpAbs, result};
				resOperand1Operand1 = new Expression{OpSec, resOperand1};
				resOperand1->_operand1 = resOperand1Operand1;
				resOperand1Operand1->_operand1 = _operand1->copy(resOperand1Operand1);
				return result;
			case OpCot:
				resAction = OpLn;
				resOperand1 = new Expression{OpAbs, result};
				resOperand1Operand1 = new Expression{OpSin, resOperand1};
				resOperand1->_operand1 = resOperand1Operand1;
				resOperand1Operand1->_operand1 = _operand1->copy(resOperand1Operand1);
				return result;
			case OpSec:
				resAction = OpLn;
				resOperand1 = new Expression{OpAbs, result};
				resOperand1Operand1 = new Expression{OpAddition, resOperand1};
				resOperand1->_operand1 = resOperand1Operand1;
				resOperand2Operand1 = new Expression{OpSec, resOperand1Operand1};
				resOperand1Operand1->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = _operand1->copy(resOperand2Operand1);
				resOperand1Operand2 = new Expression{OpTan, resOperand1Operand1};
				resOperand1Operand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				return result;
			case OpCsc:
				resAction = OpLn;
				resOperand1 = new Expression{OpAbs, result};
				resOperand1Operand1 = new Expression{OpTan, resOperand1};
				resOperand1->_operand1 = resOperand1Operand1;
				resOperand2Operand1 = new Expression{OpDivision, resOperand1Operand1};
				resOperand1Operand1->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = _operand1->copy(resOperand2Operand1);
				resOperand2Operand1->_operand2 = TWO->copy(resOperand2Operand1);
				return result;
			case OpAsin:
				resAction = OpAddition;
				resOperand1 = new Expression{OpMultiplication, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				resOperand1Operand2 = new Expression{OpAsin, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				resOperand2 = new Expression{OpSqrt, result};
				resOperand2Operand1 = new Expression{OpSubtraction, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = new Expression{OpSqr};
				resOperand2Operand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				return result;
			case OpAcos:
				resAction = OpSubtraction;
				resOperand1 = new Expression{OpMultiplication, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				resOperand1Operand2 = new Expression{OpAcos, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				resOperand2 = new Expression{OpSqrt, result};
				resOperand2Operand1 = new Expression{OpSubtraction, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = ONE->copy(resOperand2Operand1);
				resOperand1Operand2 = new Expression{OpSqr};
				resOperand2Operand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				return result;
			case OpAtan:
				resAction = OpSubtraction;
				resOperand1 = new Expression{OpMultiplication, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				resOperand1Operand2 = new Expression{OpAtan, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				resOperand2 = new Expression{OpDivision, result};
				resOperand2->_operand2 = TWO->copy(resOperand2);
				resOperand2Operand1 = new Expression{OpLn, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand1Operand1 = new Expression{OpAddition, resOperand2Operand1};
				resOperand2Operand1->_operand1 = resOperand1Operand1;
				resOperand1Operand1->_operand1 = ONE->copy(resOperand1Operand1);
				resOperand1Operand2 = new Expression{OpSqr, resOperand1Operand1};
				resOperand1Operand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				return result;
			case OpAcot:
				resAction = OpAddition;
				resOperand1 = new Expression{OpMultiplication, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				resOperand1Operand2 = new Expression{OpAcot, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				resOperand2 = new Expression{OpDivision, result};
				resOperand2->_operand2 = TWO->copy(resOperand2);
				resOperand2Operand1 = new Expression{OpLn, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand1Operand1 = new Expression{OpAddition, resOperand2Operand1};
				resOperand2Operand1->_operand1 = resOperand1Operand1;
				resOperand1Operand1->_operand1 = ONE->copy(resOperand1Operand1);
				resOperand1Operand2 = new Expression{OpSqr, resOperand1Operand1};
				resOperand1Operand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				return result;			
			case OpAsec:
				resAction = OpSubtraction;
				resOperand1 = new Expression{OpMultiplication, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				resOperand1Operand2 = new Expression{OpAsec, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				resOperand2 = new Expression{OpMultiplication, result};
				resOperand2Operand1 = new Expression{OpSgn, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = _operand1->copy(resOperand2Operand1);
				resOperand2Operand2 = new Expression{OpLn, resOperand2};
				resOperand2->_operand2 = resOperand2Operand2;
				resOperand2Operand1 = new Expression{OpAddition, resOperand2Operand2};
				resOperand2Operand2->_operand1 = resOperand2Operand1;
				resOperand1Operand1 = new Expression{OpAbs, resOperand2Operand1};
				resOperand2Operand1->_operand1 = resOperand1Operand1;
				resOperand1Operand1->_operand1 = _operand1->copy(resOperand1Operand1);
				resOperand1Operand2 = new Expression{OpSqrt, resOperand2Operand1};
				resOperand2Operand1->_operand2 = resOperand1Operand2;
				resOperand2Operand1 = new Expression{OpSubtraction, resOperand1Operand2};
				resOperand1Operand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand2 = ONE->copy(resOperand2Operand1);
				resOperand1Operand1 = new Expression{OpSqr, resOperand2Operand1};
				resOperand2Operand1->_operand1 = resOperand1Operand1;
				resOperand1Operand1->_operand1 = _operand1->copy(resOperand1Operand1);
				return result;

			case OpAcsc:
				resAction = OpAddition;
				resOperand1 = new Expression{OpMultiplication, result};
				resOperand1->_operand1 = _operand1->copy(resOperand1);
				resOperand1Operand2 = new Expression{OpAsec, resOperand1};
				resOperand1->_operand2 = resOperand1Operand2;
				resOperand1Operand2->_operand1 = _operand1->copy(resOperand1Operand2);
				resOperand2 = new Expression{OpMultiplication, result};
				resOperand2Operand1 = new Expression{OpSgn, resOperand2};
				resOperand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand1 = _operand1->copy(resOperand2Operand1);
				resOperand2Operand2 = new Expression{OpLn, resOperand2};
				resOperand2->_operand2 = resOperand2Operand2;
				resOperand2Operand1 = new Expression{OpAddition, resOperand2Operand2};
				resOperand2Operand2->_operand1 = resOperand2Operand1;
				resOperand1Operand1 = new Expression{OpAbs, resOperand2Operand1};
				resOperand2Operand1->_operand1 = resOperand1Operand1;
				resOperand1Operand1->_operand1 = _operand1->copy(resOperand1Operand1);
				resOperand1Operand2 = new Expression{OpSqrt, resOperand2Operand1};
				resOperand2Operand1->_operand2 = resOperand1Operand2;
				resOperand2Operand1 = new Expression{OpSubtraction, resOperand1Operand2};
				resOperand1Operand2->_operand1 = resOperand2Operand1;
				resOperand2Operand1->_operand2 = ONE->copy(resOperand2Operand1);
				resOperand1Operand1 = new Expression{OpSqr, resOperand2Operand1};
				resOperand2Operand1->_operand1 = resOperand1Operand1;
				resOperand1Operand1->_operand1 = _operand1->copy(resOperand1Operand1);
				return result;
		}
		return 0L;
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
Expression * Expression::ZERO {0L}, \
	* Expression::ONE {0L}, \
	* Expression::TWO {0L}, \
	* Expression::TEN {0L}, \
	* Expression::MINUS_ONE {0L}, \
	* Expression::VAR_X {0L};

