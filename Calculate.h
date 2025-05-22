#include <cctype>
#include <cstring>
#include <iostream>
#include <mpfr.h>
#include <stack>
#include <unordered_map>
#include <memory>

namespace Check {
	struct Error {
		const size_t errorPos;
		const char* const message;
		const char errorChar;
	};
	struct Op {
		const char * op;
		size_t length;
	};
	const size_t countRightUnaryOps {21};
	const Op rightUnaryOps[countRightUnaryOps] {
		{"sqrt", 4}, {"cbrt", 4}, {"asin", 4}, {"acos", 4}, \
		{"atan", 4}, {"acot", 4}, {"asec", 4}, {"acsc", 4}, \
		{"sgn", 3}, {"abs", 3}, {"sin", 3}, {"cos", 3}, \
		{"tan", 3}, {"cot", 3}, {"sec", 3}, {"csc", 3}, \
		{"ln", 2}, {"lg", 2}, {"sqr", 3}, {"exp", 3}, {"-", 1} \
	};
	const size_t countLeftUnaryOps {2};
	const Op leftUnaryOps[countLeftUnaryOps] {
		{"!", 1}, {"%", 1} \
	};
	const size_t countBinaryUnaryOps {8};
	const Op binaryUnaryOps[countBinaryUnaryOps] {
		{"mod", 3}, {"log", 3}, {"+", 1}, {"-", 1}, \
		{"*", 1}, {"/", 1}, {"^", 1}, {":", 1} \
	};
	static const std::unordered_map<char, char> \
		bracketsMap {
		{')', '('},
		{'}', '{'},
		{']', '['}
	};
	bool isBinaryDigit(const char &ch) {
		switch (ch) {
			case '0':
			case '1':
				return true;
			default:
				return false;
		}
	}
	bool isOctalDigit(const char &ch) { 
		switch (ch) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
				return true;
			default:
				return false;
		}
	}
	bool isHexDigit(const char &ch) { 
		switch (toupper(ch)) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				return true;
			default:
				return false;
		}
	}
	bool isDecimalDigit(const char &ch) {
		switch (ch) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return true;
			default:
				return false;
		}
	}
	bool isPoint(const char &ch) {
		switch (ch) {
			case '.':
			case ',':
				return true;
			default:
				return false;
		}
	}
	bool isOpenBrackets(const char &ch) {
		switch (ch) {
			case '(':
			case '[':
			case '{':
				return true;
			default:
				return false;
		}
	}
	bool isCloseBrackets(const char &ch) {
		switch (ch) {
			case ')':
			case ']':
			case '}':
				return true;
			default:
				return false;
		}
	}
	void checkUnaryOps(const char* &str, const Op *oper, \
		const size_t &countOps, bool &expectOperand) {
		bool match {false};
		const char *strPtr {nullptr}, \
			*operPtr {nullptr}, *operPtrEnd {nullptr};
		size_t lenOper {0UL};
		for (const Op *operEnd{oper+countOps}; \
			oper < operEnd; oper++) {
			const Op& op = *oper;
			match = true;
			lenOper = op.length;
			for (strPtr = str, operPtr = op.op, \
				operPtrEnd = operPtr + lenOper; \
				operPtr < operPtrEnd; strPtr++ , \
				operPtr++) {
				if (*strPtr != *operPtr) {
					match = false;
					break;
				}
			}
			if (match) { expectOperand = true, \
				str = strPtr; return; }
		}
		expectOperand = false; return;
	}
	inline Error *isNumber(const char * &ptr, \
		bool (*func)(const char &), \
		const char * const &exprStart) {
		puts(ptr);
		char symbol {*ptr};
		if (symbol == '-') symbol = *++ptr;
		if (!func(symbol)) 
			return new Error{(size_t)(ptr - exprStart), \
				"Invalid digit", symbol};
		if (symbol == '0' && !isPoint(ptr[1]))
			return new Error{(size_t)(ptr - exprStart), \
				"No comma after zero", symbol};
		while (func(*ptr)) ptr++;
		if (!isPoint(*ptr)) return nullptr;
		ptr++;
		while (func(*ptr)) ptr++;
		//puts("isNumber double point");
		if (isPoint(symbol = *ptr))
			return new Error{(size_t)(ptr - exprStart), \
				"Double point", symbol};
		return nullptr;
	}
	inline Error *isBinaryNumber(const char * &ptr, \
		const char * const &exprStart) {
		return isNumber(ptr, isBinaryDigit, exprStart);
	}
	inline Error *isOctalNumber(const char * &ptr, \
		const char * const &exprStart) {
		return isNumber(ptr, isOctalDigit, exprStart);
	}
	inline Error *isHexNumber(const char * &ptr, \
		const char * const &exprStart) {
		return isNumber(ptr, isHexDigit, exprStart);
	}
	inline Error *isDecimalNumber(const char * &ptr, \
		const char * const &exprStart) {
		return isNumber(ptr, isDecimalDigit, exprStart);
	}
	inline Error *isDecimalZeroNumber(const char * &ptr, \
		const char * const &exprStart) {
		char symbol {*ptr};
		if (symbol == '-') symbol = *++ptr;
		while (isDecimalDigit(*ptr)) ptr++;
		//puts("Decimal Zero");
		if (isPoint(symbol = *ptr))
			return new Error{(size_t)(ptr - exprStart), \
				"Double point", symbol};
		return nullptr;
	}

	inline Error *parseNumber(const char* &ptr, bool &expectOperand, \
		const char * const &exprStart) {
		char symbol {*ptr};
		if (symbol == '-') symbol = *++ptr;
		bool hasPoint {false};
		if (symbol == '0') {
			Error* (*func)(const char * &, const char * const&) {nullptr};
			switch (symbol = *++ptr) {
				case 'b':
				case 'B':
					func = isBinaryNumber;
					break;
				case 'o':
				case 'O':
					func = isOctalNumber;
					break;
				case 'x':
				case 'X':
					func = isHexNumber;
					break;
				case ',':
				case '.':
					func = isDecimalZeroNumber;
					break;
				default:
					if (isDecimalDigit(symbol))
						return new Error{(size_t)(ptr - exprStart), \
							"No comma after zero", symbol};
					expectOperand = false;
					return nullptr;
			}
			expectOperand = false;
			return func(++ptr, exprStart);
		} else if (isDecimalDigit(symbol)) {
			expectOperand = false;
			return isDecimalNumber(ptr, exprStart);
		} else return nullptr;
	}
	inline Error* parseX(const char* &ptr, bool &expectOperand, \
		const char * const &exprStart) {
		switch (*ptr) {
			case 'x':
			case 'X':
				expectOperand = false, ptr++;
		}
		return nullptr;
	}
	Error* isBalancedParentheses(const char* const expression) {
		std::stack<std::pair<char, const char*>> balanceBrackets;
		char symbol {'\0'};
		for (const char* ptr = expression; *ptr; ptr++) {
			if (isOpenBrackets(symbol = *ptr))
				balanceBrackets.push({symbol, ptr});
			else if (isCloseBrackets(symbol)) {
				if (balanceBrackets.empty() || \
					balanceBrackets.top().first != bracketsMap.at(symbol))
					return new Error{(size_t)(ptr - expression), \
						"Extra closing bracket", symbol};
				balanceBrackets.pop();
			}
		}

		if (!balanceBrackets.empty()) {
			std::pair<char, const char *>& top = balanceBrackets.top();
			return new Error{(size_t)(top.second - expression), \
				"Unclosed opening bracket", top.first};
		}
		return nullptr;
	}
	Error* validateExpression(const char * const expression, \
		const bool withX) {
		Error* error {isBalancedParentheses(expression)};
		if (error) return error;

		bool expectOperand {true}, is {false};
		const char* ptr {expression};
		char symbol {'\0'};

		while (symbol = *ptr) {
			if (expectOperand) {
				// Check for right unary
				checkUnaryOps(ptr, rightUnaryOps, \
					countRightUnaryOps, is = false);
				if (is) continue;
				
				// Check for number
				error = parseNumber(ptr, expectOperand, expression);
				if (error) return error;
				if (!expectOperand) continue;

				if (withX) {
					error = parseX(ptr, expectOperand, expression);
					if (error) return error;
					if (!expectOperand) continue;
				}

				// Check for opening bracket
				if (isOpenBrackets(symbol)) {
					ptr++; continue;
				}
				return new Error{(size_t)(ptr - expression), \
					"Expected operand", symbol};
			} else {
				// Check for binary operator
				checkUnaryOps(ptr, binaryUnaryOps, \
					countBinaryUnaryOps, expectOperand);
				std::cout << expectOperand << std::endl;
				if (expectOperand) continue;

				// Check for left unary
				checkUnaryOps(ptr, leftUnaryOps, \
					countLeftUnaryOps, expectOperand);
				if (expectOperand) continue;

				// Check for closing bracket
				if (isCloseBrackets(symbol)) {
					ptr++;
					expectOperand = false;
					continue;
				}

				return new Error{(size_t)(ptr - expression), \
					"Expected operator", *ptr};
			}
		}

		if (expectOperand)
			return new Error{(size_t)((ptr -= 1UL) - expression), \
				"Unexpected end of expression", *ptr};

		return nullptr;
	}
}


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
	noneDi, numberDi, variableDi, actionVaribleDi \
};
static inline bool _isBrackets(char symbol) {
	return symbol == '(' || symbol == ')';
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
		if (levelBrackets != level) *expression = '\\';
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
	for (size_t levelBrakets{0UL}; ptr == nullptr && \
		levelBrakets < maxLevelBrackets; levelBrakets++) {
		strcpy(copyExpression, expression);
		_replaceOnX(copyExpression, levelBrakets);
		//puts(copyExpression);
		if (ptr = strrstr(copyExpression, "+")) {
			lenOperator = 1, action = OpAddition; break;
		}
		if (ptr = strrstr(copyExpression, "-")) {
			lenOperator = 1;
			//if (_isOperatorSub(ptr, expression)) 
			if (ptr == copyExpression || isalpha(*(ptr-1)) && \
				*(ptr-1) != 'x' || *(ptr-1) == '(') {
				action = OpUnaryMinus, ptr = nullptr;
					puts("minus-unsryminus");
					std::cout << action << ARIFMETIC_STR_ACTION[action] \
						<< std::endl;
			}
			else {
				action = OpSubtraction; break;
			}
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
		if (((temp = strrstr(copyExpression, "sin")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpSin, ptr = temp; 
		if (((temp = strrstr(copyExpression, "cos")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpCos, ptr = temp; 
		if (((temp = strrstr(copyExpression, "tan")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpTan, ptr = temp; 
		if (((temp = strrstr(copyExpression, "cot")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpCot, ptr = temp; 
		if (((temp = strrstr(copyExpression, "sec")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpSec, ptr = temp; 
		if (((temp = strrstr(copyExpression, "csc")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpCsc, ptr = temp; 
		if (((temp = strrstr(copyExpression, "asin")) < ptr || !ptr) && temp)
			lenOperator = 4, action = OpAsin, ptr = temp; 
		if (((temp = strrstr(copyExpression, "acos")) < ptr || !ptr) && temp)
			lenOperator = 4, action = OpAcos, ptr = temp; 
		if (((temp = strrstr(copyExpression, "atan")) < ptr || !ptr) && temp)
			lenOperator = 4, action = OpAtan, ptr = temp; 
		if (((temp = strrstr(copyExpression, "acot")) < ptr || !ptr) && temp)
			lenOperator = 4, action = OpAcot, ptr = temp; 
		if (((temp = strrstr(copyExpression, "asec")) < ptr || !ptr) && temp)
			lenOperator = 4, action = OpAsec, ptr = temp; 
		if (((temp = strrstr(copyExpression, "acsc")) < ptr || !ptr) && temp)
			lenOperator = 4, action = OpAcsc, ptr = temp;
		if (((temp = strrstr(copyExpression, "lg")) < ptr || !ptr) && temp)
			lenOperator = 2, action = OpLg, ptr = temp;
		if (((temp = strrstr(copyExpression, "ln")) < ptr || !ptr) && temp)
			lenOperator = 2, action = OpLn, ptr = temp;
		if (((temp = strrstr(copyExpression, "abs")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpAbs, ptr = temp;
		if (((temp = strrstr(copyExpression, "sgn")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpSgn, ptr = temp;
		if (((temp = strrstr(copyExpression, "log")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpLog, ptr = temp;
		if (((temp = strrstr(copyExpression, "cbrt")) < ptr || !ptr) && temp)
			lenOperator = 4, action = OpCbrt, ptr = temp;
		if (((temp = strrstr(copyExpression, "sqrt")) < ptr || !ptr) && temp)
			lenOperator = 4, action = OpSqrt, ptr = temp;
		if (((temp = strrstr(copyExpression, "sqr")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpSqr, ptr = temp;
		if (((temp = strrstr(copyExpression, "exp")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpExp, ptr = temp;
		if (((temp = strrchr(copyExpression, '^')) < ptr || !ptr) && temp)
			lenOperator = 1, action = OpPower, ptr = temp;
		if (((temp = strrchr(copyExpression, '-')) < ptr || !ptr) && temp)
			lenOperator = 1, action = OpUnaryMinus, ptr = temp;
		puts(copyExpression);
		std::cout << (void*)ptr << "fj" << \
			(action == OpUnaryMinus) << "gf" << action  << \
			ARIFMETIC_STR_ACTION[action] << "io" << \
			(void *)strstr(copyExpression, "sqr") << std::endl;
		//puts("rfjvkfkml");
	}
	//std::cout << ptr << std::endl;
	//putchar('A');
	//putchar(*ptr);
	delete [] copyExpression;
	//puts("klsdsdsd");
	if (ptr) ptr = expression + (ptr - copyExpression);
	puts("klhj");
	std::cout << (void*)ptr << std::endl;
	return ptr;
}

enum TypeData { \
	nullTD, numberTD, variableTD, actionTD \
};
union Data { \
	const char * variable; \
	mpfr_t number; \
	ArifmeticAction action; \
};
class Expression;
using Expr = std::shared_ptr<Expression>;
inline Expr operator+(Expr operand1, Expr operand2 \
) noexcept;
inline Expr operator-(Expr operand1, Expr operand2 \
) noexcept;
inline Expr operator*(Expr operand1, Expr operand2 \
) noexcept;
inline Expr operator/(Expr operand1, Expr operand2 \
) noexcept;
class Expression : public std::enable_shared_from_this<Expression> {
private:

	friend inline Expr operator+(Expr operand1, Expr operand2 \
	) noexcept;
	friend inline Expr operator-(Expr operand1, Expr operand2 \
	) noexcept;
	friend inline Expr operator*(Expr operand1, Expr operand2 \
	) noexcept;
	friend inline Expr operator/(Expr operand1, Expr operand2 \
	) noexcept;

	Expr _operand1 {nullptr}, _operand2 {nullptr};
	Data _data;
	TypeData _typeData;

	
	static size_t size, count;
	static char *strPrint;
	static Expr ZERO, ONE, TWO, THREE, \
		FOUR, TEN, VAR_X, MINUS_ONE;
	inline explicit Expression (
		const char * const number \
	) : _typeData{nullTD}, _operand1{nullptr}, \
		_operand2{nullptr} {
		if (isdigit(*number) || *number == '-') {
			_typeData = numberTD;
			mpfr_init2(_data.number, size);
			mpfr_set_str(_data.number, number, 10, MPFR_RNDN);
			puts(number);
		} else {
			_typeData = variableTD;
			_data.variable = "x";
		}
	}
	inline explicit Expression(ArifmeticAction action, \
		Expr operand1 = nullptr, Expr operand2 = nullptr)
		: _typeData{actionTD}, _data{.action = action}, \
		_operand1{operand1}, _operand2{operand2} {}
	

	inline explicit Expression(void) noexcept : \
		_operand1{nullptr}, _operand2{nullptr}, _typeData{nullTD} {}
	

	inline explicit Expression(mpfr_t *number) noexcept : \
		_typeData{numberTD}, _operand1{nullptr}, _operand2{nullptr} { \
		mpfr_init(_data.number);
		mpfr_set(_data.number, *number, MPFR_RNDN);
	}

	mpfr_t* calculate(Expr xNumber = nullptr) {
		mpfr_t* result {new mpfr_t[1]};
		mpfr_init2(*result, size);
		switch (_typeData) {
			case variableTD:
				mpfr_set(*result, xNumber->_data.number, MPFR_RNDN);
				return result;
			case numberTD:
				mpfr_set(*result, _data.number, MPFR_RNDN);
				return result;
		}
		mpfr_t* operand1 = _operand1->calculate(xNumber), *operand2;
		ArifmeticAction arifmeticAction = _data.action;
		bool isTwoOperandBool {isTwoOperand()};
		if (isTwoOperandBool)
			operand2 = _operand2->calculate(xNumber);
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
				mpfr_neg(*result, *operand1, MPFR_RNDN);
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
			case OpPower:
				mpfr_pow(*result, *operand1, *operand2, MPFR_RNDN);
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
			case OpAbs:
				mpfr_abs(*result, *operand1, MPFR_RNDN);
				break;
			case OpSgn:
				mpfr_set_si(*result, mpfr_sgn(*operand1), MPFR_RNDN);
		}
		mpfr_clear(*operand1);
		delete [] operand1;
		if (isTwoOperandBool) {
			mpfr_clear(*operand2);
			delete [] operand2;
		}
		//puts("skddk");
		//mpfr_printf("%Rf\n", *result);
		return result;
	}
	static void deleteZeros(char * const str) {
		char * ptr {str + strlen(str) - 1UL};
		for (; ptr != str; ptr--)
			if (*ptr != '0') break;
		if (*ptr == '.') *ptr = '\0';
		else if (strchr(str, '.')) *++ptr = '\0';
		return;
	}
public:
	static Expr create(const char* str) {
        return std::shared_ptr<Expression>{new Expression{str}};
    }

	static Expr create(void) {
		return std::shared_ptr<Expression>{new Expression{}};
	}

    static Expr create( \
		ArifmeticAction action, Expr a = nullptr, Expr b = nullptr) {
        return std::shared_ptr<Expression>{new Expression{action, a, b}};
    }
    static Expr create(mpfr_t * number) {
        return std::shared_ptr<Expression>{new Expression{number}};
    }
	const char *calc(Expr xNumber = nullptr) {
		char * str {nullptr};
		mpfr_t * const result {calculate(xNumber)};
		mpfr_asprintf(&str, Expression::strPrint, *result);
		mpfr_clear(*result);
		delete [] result;
		deleteZeros(str);
		return (const char *)str;
	}
	Expr calcExpr(Expr xNumber = nullptr) {
		mpfr_t * const result {calculate(xNumber)};
		Expr expr {Expression::create(result)};
		mpfr_clear(*result);
		delete [] result;
		return expr;
	}
	static void init(size_t bits) {
		Expression::size = bits;
		Expression::count = bits * 10 / 34;
		if (Expression::strPrint)
			delete [] Expression::strPrint;
		asprintf(&Expression::strPrint, "%%.%luRf", count);
		ZERO = Expression::create("0"), \
		ONE = Expression::create("1"), \
		TWO = Expression::create("2"), \
		THREE = Expression::create("3"), \
		FOUR = Expression::create("4"), \
		TEN = Expression::create("10"), \
		MINUS_ONE = Expression::create("-1");
		if (!VAR_X) VAR_X = Expression::create("x");
	}
	static inline bool isTwoOperand( \
		const ArifmeticAction &action) {
		return action < OpSin;
	}
	inline bool isTwoOperand(void) const {
		return _data.action < OpSin;
	}
	
	static Expr buildExpressionTree( \
		const char * constExpression) {
		unsigned char lenOperator;
		bool isTwoOperand;
		ArifmeticAction action {OpNone};
		char *expression {(char*)strdup(constExpression)};
		const char * ptrOperator { \
			_shearchNotPriorityOperator(expression, \
				lenOperator, action)};
		char * newExpression {nullptr};
		size_t lenExpression {0UL};
		if (!ptrOperator) {
			puts("Number");
			puts(expression);
			lenExpression = strlen(expression);
			if (*(newExpression = expression + \
				(size_t)(lenExpression - 1UL)) == ')')
				*newExpression = '\0'; 
			if (*expression == '(')
				memmove(expression, expression + 1UL, 
					(size_t)(lenExpression));

			puts(expression);
			Expr expr {Expression::create(expression)};
			delete [] expression;
			return expr;
		}
		Expr result {Expression::create(action)};
		Expr *operand {nullptr};
		if (Expression::isTwoOperand(action)) {
			lenExpression = (size_t)(ptrOperator - expression);
			newExpression = new char[lenExpression+1UL]{'\0'};
			strncpy(newExpression, expression, lenExpression);
			result->_operand1 = Expression::buildExpressionTree(newExpression);
			delete [] newExpression;
			lenExpression = (size_t)(strlen(expression) - \
				(ptrOperator - expression) - lenOperator);
			operand = &result->_operand2;
		} else {
			lenExpression = (size_t)(ptrOperator - expression + \
				strlen(expression) + lenOperator);
			operand = &result->_operand1;
		}
		newExpression = new char[lenExpression+1UL]{'\0'};
		puts(ptrOperator);
		//std::cout << (void*)(ptrOperator - expression) << std::endl \
			<< ;
		strncpy(newExpression, ptrOperator + lenOperator, lenExpression);
		puts("newExpression");
		puts(newExpression);
		*operand = Expression::buildExpressionTree(newExpression);
		delete []expression;
		return result;
	}
	inline const char *print(void) {
		char * expression {nullptr};
		switch (_typeData) {
			case numberTD:
				puts("numberTD");
				mpfr_asprintf(&expression, Expression::strPrint, \
					_data.number);
				deleteZeros(expression);
				return expression;
			case variableTD:
				puts("variableTD");
				return strdup("x");
		}
		const char *operand1 {_operand1->print()}, \
			*action {ARIFMETIC_STR_ACTION[_data.action]};
		if (isTwoOperand()) {
			const char *operand2 {_operand2->print()};
			expression = new char[strlen(operand1) + \
				strlen(action) + strlen(operand2) + 5UL];
			asprintf(&expression, "(%s)%s(%s)", operand1, action, operand2);
			delete [] operand2;
		} else {
			expression = new char[strlen(action) + \
				strlen(operand1) + 3UL];
			puts("operand1");
			asprintf(&expression, "%s(%s)", action, operand1);
		}
		delete [] operand1;
		return expression;
	}
	
	inline Diff heandlerDifferentiate(void) const {
		switch (_typeData) {
			case numberTD:
				//puts("svfjsjk");
				return numberDi;
			case variableTD:
				return variableDi;
		}
		//std::cout << _typeData << std::endl;
		puts(ARIFMETIC_STR_ACTION[_data.action]);
		Diff diff {_operand1->heandlerDifferentiate()};
		if (diff != numberDi) return actionVaribleDi;
		if (isTwoOperand())
			if ((diff = _operand2->heandlerDifferentiate()) == variableDi) 
				return actionVaribleDi;
		return diff;
	}

	inline Expr mod(Expr operand2 \
	) noexcept {
		return Expression::create( \
			OpRemainderFromDivision, shared_from_this(), operand2);
	}
	inline Expr pow(Expr operand2 \
	) noexcept {
		return Expression::create( \
			OpPower, shared_from_this(), operand2);
	}

	inline Expr log(Expr operand2 \
	) noexcept {
		return Expression::create( \
			OpLog, shared_from_this(), operand2);
	}
	inline Expr sin(void) noexcept {
		return Expression::create(OpSin, shared_from_this());
	}
	inline Expr cos(void) noexcept {
		return Expression::create(OpCos, shared_from_this());
	}
	inline Expr tan(void) noexcept {
		return Expression::create(OpTan, shared_from_this());
	}
	inline Expr cot(void) noexcept {
		return Expression::create(OpCot, shared_from_this());
	}
	inline Expr sec(void) noexcept {
		return Expression::create(OpSec, shared_from_this());
	}
	inline Expr csc(void) noexcept {
		return Expression::create(OpCsc, shared_from_this());
	}
	inline Expr asin(void) noexcept {
		return Expression::create(OpAsin, shared_from_this());
	}
	inline Expr acos(void) noexcept {
		return Expression::create(OpAcos, shared_from_this()); 
	}
	inline Expr atan(void) noexcept {
		return Expression::create(OpAtan, shared_from_this());
	}
	inline Expr acot(void) noexcept {
		return Expression::create(OpAcot, shared_from_this());
	}
	inline Expr asec(void) noexcept {
		return Expression::create(OpAsec, shared_from_this());
	}
	inline Expr acsc(void) noexcept {
		return Expression::create(OpAcsc, shared_from_this());
	}
	inline Expr sqrt(void) noexcept {
		return Expression::create(OpSqrt, shared_from_this());
	}
	inline Expr sqr(void) noexcept {
		return Expression::create(OpSqr, shared_from_this());
	}
	inline Expr cbrt(void) noexcept {
		return Expression::create(OpCbrt, shared_from_this());
	}
	inline Expr unaryMinus(void) noexcept {
		return Expression::create(OpUnaryMinus, shared_from_this());
	}
	inline Expr abs(void) noexcept {
		return Expression::create(OpAbs, shared_from_this());
	}
	inline Expr sgn(void) noexcept {
		return Expression::create(OpSgn, shared_from_this());
	}
	inline Expr lg(void) noexcept {
		return Expression::create(OpLg, shared_from_this());
	}
	inline Expr ln(void) noexcept {
		return Expression::create(OpLn, shared_from_this());
	}
	inline Expr exp(void) noexcept {
		return Expression::create(OpExp, shared_from_this());
	}
	inline Expr diff(void) {
		switch (heandlerDifferentiate()) {
			case numberDi: return ZERO;
			case variableDi: return ONE;
		}
		Expr &operand1 {_operand1}, &operand2 {_operand2}, \
			result {nullptr};
		Diff operand1Di {operand1->heandlerDifferentiate()}, \
			operand2Di {noneDi};
		if (operand2) operand2Di = operand2->heandlerDifferentiate();

		const ArifmeticAction &action {_data.action};

        switch (action) {
			case OpAddition:
				if (operand1Di == numberDi)
					result = operand2->diff();
				else if (operand2Di == numberDi)
					result = operand1->diff();
				else 
					result = operand1->diff() + operand2->diff();
				return result;
			case OpSubtraction:
				if (operand1Di == numberDi)
					result = operand2->diff()->unaryMinus();
				else if (operand2Di == numberDi)
					result = operand1->diff();
				else 
					result = operand1->diff() - operand2->diff();
				return result;
			case OpMultiplication:
				if (operand1Di == numberDi && \
					operand2Di == numberDi)
					result = ZERO;
				else if (operand1Di == variableDi && \
					operand2Di == numberDi)
					result = operand2;
				else if (operand2Di == variableDi && \
					operand1Di == numberDi)
					result = operand1;
				else if (operand2Di == variableDi && \
					operand1Di == variableDi)
					result = TWO * operand1;
				else if (operand1Di == numberDi)
					result = operand1 * operand2->diff();
				else if (operand2Di == numberDi)
					result = operand2 * operand1->diff();
				else if (operand1Di == variableDi)
					result = operand2 + operand1 * operand2->diff();
				else if (operand2Di == variableDi)
					result = operand1 + operand2 * operand1->diff();
				else
					result = operand1->diff() * operand2 + \
						operand1 * operand2->diff();
				return result;
			case OpDivision:
				result = (operand1 * operand2)->diff() / operand2->sqr();
				return result;
			case OpPower:
				switch (operand2Di) {
					case numberDi:
						result = operand2 * operand1->pow(operand2 - ONE);
						break;
					case variableDi:
					case actionVaribleDi:
						result = shared_from_this() * (operand2->diff() * \
							operand1->ln() + operand2 * operand1->diff() / operand1);
				}
				return result;
			case OpSqr:
				switch (operand1Di) {
					case variableDi:
						result = TWO * operand1;
						break;
					case numberDi:
						result = ZERO;
						break;
					case actionVaribleDi:
						result = TWO * (operand1->diff() * operand1);
				}
				return result;
			case OpSqrt:
				switch (operand1Di) {
					case variableDi:
						return ONE / (TWO * shared_from_this());
					case numberDi:
						return ZERO;
					case actionVaribleDi:
						return operand1->diff() / (TWO * shared_from_this());
				}
				return nullptr;
			case OpCbrt:
				switch (operand1Di) {
					case variableDi:
						return ONE / (THREE * operand1->sqr()->cbrt());
					case numberDi:
						return ZERO;
					case actionVaribleDi:
						return operand1->diff() / (THREE * operand1->sqr()->cbrt());
				}
				return nullptr;
			case OpLog:
				result = ONE / operand1 * operand2->ln();
				break;
			case OpLn:
				result = ONE / operand1;
				break;
			case OpLg:
				result = ONE / operand1 * TEN->ln();
				break;
			case OpExp:
				result = operand1->exp();
				break;
			case OpSin:
				result = operand1->cos();
				break;
			case OpCos:
				result = operand1->sin()->unaryMinus();
				break;
			case OpTan:
				result = operand1->sec()->sqr();
				break;
			case OpCot:
				result = operand1->csc()->unaryMinus()->sqr();
				break;
			case OpSec:
				result = operand1->sec() * operand1->tan();
				break;
			case OpCsc:
				result = operand1->csc()->unaryMinus() * operand1->cot();
				break;
			case OpAbs:
				result = operand1->sgn();
				break;
			case OpUnaryMinus:
				puts("diff unary minus");
				return operand1->diff()->unaryMinus();
			case OpRemainderFromDivision:
				return operand1->diff() - operand2->diff() * \
					(operand1 / operand2)->abs();
			case OpAsin:
				result = ONE / (ONE - operand1->sqr())->sqrt();
				break;
			case OpAcos:
				result = (ONE / (ONE - operand1->sqr())->sqrt())->unaryMinus();
				break;
			case OpAtan:
				result = ONE / (ONE + operand1->sqr())->sqrt();
				break;
			case OpAcot:
				result = (ONE / (ONE + operand1->sqr())->sqrt())->unaryMinus();
				break;
			case OpAsec:
				result = ONE / (operand1->abs() * \
					(ONE - operand1->sqr())->sqrt());
				break;
			case OpAcsc:
				result = ONE / (operand1->abs() * \
					(ONE - operand1->sqr())->sqrt());
				break;
    
			case OpSgn:
				return ZERO;
		}
		if (operand1Di == variableDi)
			return result;
		return result * operand1->diff();
	}
	inline Expr integrate(Expression * parent = 0L) {
		switch (heandlerDifferentiate()) {
			case numberDi:
				return shared_from_this() * VAR_X;
			case variableDi:
				return shared_from_this()->sqr() / TWO;
		}
		const ArifmeticAction &action {_data.action};
		Expr &operand1 {_operand1}, &operand2 {_operand2};
		Diff operand1Di {_operand1->heandlerDifferentiate()}, \
			operand2Di {noneDi};
		if (_operand2) operand2Di = _operand2->heandlerDifferentiate();
		switch (action) {
			case OpAddition:
				if (operand1Di == numberDi)
					return operand2->integrate();
				else if (operand2Di == numberDi)
					return operand1->integrate();
				else 
					return operand1->integrate() + operand2->integrate();
			case OpSubtraction:
				if (operand1Di == numberDi)
					return operand2->integrate()->unaryMinus();
				else if (operand2Di == numberDi)
					return operand1->integrate();
				else 
					return operand1->integrate() - operand2->integrate();
			case OpMultiplication:
				// Попробовать упростить (если один из операндов — константа)
				if (operand1Di == numberDi)
					return operand1 * operand2->integrate();
				else if (operand2Di == numberDi)
					return operand1->integrate() * operand2;
				// Иначе применить интегрирование по частям: ∫u dv = uv - ∫v du
				else return operand1 * operand2->integrate() - \
					(operand2->integrate() * operand1->diff())->integrate();
			case OpDivision:  // Деление: ∫(u / v) dx → зависит от формы u и v
				return (ONE / operand1 * operand2)->integrate();
			case OpPower:
				return (operand2 * operand1->ln())->exp();
			case OpAbs:
				return operand1 * operand1->abs() / TWO;
			case OpLog:
				return operand2 * operand1->log(operand2) - \
					operand2 / operand1->ln();
			case OpLn:
				return operand1 * operand1->ln() - operand1;
			case OpLg:
				return operand1 * operand1->lg() - operand1 / operand1->lg();
			case OpUnaryMinus:
				return operand1->integrate()->unaryMinus(); 
			case OpExp:
				return (operand1->exp() * (ONE / operand1->diff()))->integrate();
			case OpRemainderFromDivision:
				{
					Expr div {operand1 / operand2}, \
						op2Integrate {operand2->integrate()};
					return operand1->integrate() - div * op2Integrate + \
						(op2Integrate * div->diff())->integrate();
				}
			case OpCbrt:
				return THREE / FOUR * operand1->pow(FOUR / THREE);
			case OpSqrt:
				return TWO / THREE * operand1->pow(THREE / TWO);
			case OpSqr:
				return operand1->pow(THREE) / THREE;
			case OpSin:
				return operand1->cos()->unaryMinus();
			case OpCos:
				return operand1->sin();
			case OpTan:
				return operand1->sec()->abs()->ln();
			case OpCot:
				return operand1->sin()->abs()->ln();
			case OpSec:
				return (operand1->tan() + operand1->sec())->abs();
			case OpCsc:
				return (operand1 / TWO)->tan()->abs();
			case OpAsin:
				return operand1 * operand1->asin() + (ONE - \
					operand1->sqr())->sqrt();
			case OpAcos:
				return operand1 * operand1->acos() - \
					(ONE - operand1->sqr())->sqr();
			case OpAtan:
				return operand1 * operand1->atan() - \
					(ONE + operand1->sqr())->ln() / TWO;
			case OpAcot:
				return operand1 * operand1->acos() + \
					(ONE + operand1->sqr())->ln() / TWO;
			case OpAsec:
				return operand1 * operand1->asec() - \
					(operand1->sgn() * (operand1->abs() \
					+ (operand1->sqr() - ONE)->sqrt())->ln());
			case OpAcsc:
				return operand1 * operand1->acsc() - \
					(operand1->sgn() * (operand1->abs() \
					+ (operand1->sqr() - ONE)->sqrt())->ln());
			case OpSgn:
				return operand1->abs();
		}
		return nullptr;
	}
	~Expression() {
		if (_typeData == numberTD)
			mpfr_clear(_data.number);
	}
};
inline Expr operator+(Expr operand1, Expr operand2 \
) noexcept {
	return Expression::create( \
		OpAddition, operand1, operand2);
}
inline Expr operator-(Expr operand1, Expr operand2 \
) noexcept {
	return Expression::create( \
		OpSubtraction, operand1, operand2);
}
inline Expr operator*(Expr operand1, Expr operand2 \
) noexcept {
	return Expression::create( \
		OpMultiplication, operand1, operand2);
}
inline Expr operator/(Expr operand1, Expr operand2 \
) noexcept {
	return Expression::create( \
		OpDivision, operand1, operand2);
}
size_t Expression::size {0UL}, Expression::count {0UL};
char * Expression::strPrint {nullptr};
Expr Expression::ZERO {nullptr}, Expression::ONE {nullptr}, \
	Expression::TWO {nullptr}, Expression::THREE {nullptr}, \
	Expression::FOUR {nullptr}, Expression::TEN {nullptr}, \
	Expression::MINUS_ONE {nullptr}, Expression::VAR_X {nullptr};
