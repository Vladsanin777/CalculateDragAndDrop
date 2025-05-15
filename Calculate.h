#include <cctype>
#include <cstring>
#include <iostream>
#include <mpfr.h>
#include <stack>
#include <unordered_map>
#include <shared_ptr>

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
	const size_t countRightUnaryOps {20};
	const Op rightUnaryOps[countRightUnaryOps] {
		{"sqrt", 4}, {"cbrt", 4}, {"asin", 4}, {"acos", 4}, \
		{"atan", 4}, {"acot", 4}, {"asec", 4}, {"acsc", 4}, \
		{"sgn", 3}, {"abs", 3}, {"sin", 3}, {"cos", 3}, \
		{"tan", 3}, {"cot", 3}, {"sec", 3}, {"csc", 3}, \
		{"ln", 2}, {"lg", 2}, {"sqr", 3}, {"exp", 3} \
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
		while (func(*ptr)) ptr++;
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
				// Check for number
				error = parseNumber(ptr, expectOperand, expression);
				if (error) return error;
				if (!expectOperand) continue;

				if (withX) {
					error = parseX(ptr, expectOperand, expression);
					if (error) return error;
					if (!expectOperand) continue;
				}

				// Check for right unary
				checkUnaryOps(ptr, rightUnaryOps, \
					countRightUnaryOps, is = false);
				if (is) continue;

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
/*
в C++ проверить строку являетсяли она математическим выражением
Напиши поддержку операторов c двумя опеоандами + ^ / : * mod log
с левым операндом ! %
с правым операндом sqr sqrt ln lg sgn abs exp cbrt asin acos atan asec acsc sin cos tan asec acsc
У лево сторонних операндов может и не быть скобок и поэтому это "sqrt25" валидно
При этом числа могут быть двоичные 0b101010 восьмеричные 0o173 шестнадцатиричные 0xA8F1 и дестичные 789823
Сделай так чтобы код обределял в каком символе от начала строки ошибка Так же говорил в чём конкретно ошибка и символ из за которого происходит ошибка
Не используй библиотеки string за место неё используй char * также избавься от unordered_set в пользу обычного массива с константой для хранения длинны
добавь поддержку скобок () [] {}
вот структура для возврата ошибки
struct Error {
    size_t errorPos;
    char* message;
    char errorChar;
};
на до всегда возращать указатель на эту структуру
и если ошибки нет возращать nullptr
пример с функцией для проверки баланса скобок тебе её писать не надо а посто сделать так чтобы она органично вписалась в твой код
Error *isBalancedParentheses(const char * const expression) {
	std::stack<std::pair<char, const char *>> balanceBrackets;
	char symbol {'\0'};
	for (const char * ptr {expression}, \
		* const ptrEnd {expression + strlen(expression)}; \
		ptr != ptrEnd; ptr++) {
		switch (symbol = *ptr) {
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
					bracketsIsValid.at(symbol))
					return new Error{(size_t)(ptr - expression), "Exstra Bracket", symbol};
				else
					balanceBrackets.pop();
		}
	}
	if (!balanceBrackets.empty()) {
		char *exstraBracket {new char[30]{'\0'}};
		size_t index {};
		sprintf(exstraBracket, "%lu  '%c'", \
			index, balanceBrackets.top().first);
		return new Error{(size_t)(balanceBrackets.top().second - expression), "Exstra Bracket", symbol};
	}
	return nullptr;
}
*/


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
		if (((temp = strstr(copyExpression, "sqr")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpSqr, ptr = temp;
		if (((temp = strstr(copyExpression, "exp")) < ptr || !ptr) && temp)
			lenOperator = 3, action = OpExp, ptr = temp;
		if (((temp = strchr(copyExpression, '^')) < ptr || !ptr) && temp)
			lenOperator = 1, action = OpPower, ptr = temp;
		if (action == OpUnaryMinus && !ptr) ptr = strchr(copyExpression, '-');
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
class Expression;
using Expr = std::shared_ptr<Expression>;
class Expression : public std::enable_shared_from_this<Expression> {
private:
	Expr _operand1 {nullptr}, _operand2 {nullptr};
	Data _data;
	TypeData _typeData;

	static size_t size;
	static Expr ZERO, ONE, \
		TWO, TEN, VAR_X, MINUS_ONE;
	inline explicit Expression (
		const char * const number, \
	) : _operand1{nullptr}, _operand2{nullptr} {
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
	}
	inline explicit Expression(ArifmeticAction action, \
		Expr operand1 = nullptr, Expr operand2 = nullptr)
		: _typeData{actionTD}, _data{.action = action}, \
		_operand1{operand1}, _operand2{operand2} {}
	inline explicit Expression(ArifmeticAction action, \
		Expr operand1 = nullptr, Expr operand2 = nullptr)
		: _typeData{actionTD}, _data{.action = action}, \
		_operand1{operand1.shared_from_this}, _operand2{operand2} {}
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
		mpfr_printf("%Rf\n", *result);
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
		ZERO = std::make_shared<Expression>("0"), \
		ONE = std::make_shared<Expression>("1"), \
		TWO = std::make_shared<Expression>("2"), \
		TEN = std::make_shared<Expression>("10"), \
		MINUS_ONE = std::make_shared<Expression>("-1");
		if (!VAR_X) VAR_X = std::make_shared<Expression>{"x"};
	}
	static inline bool isTwoOperand( \
		const ArifmeticAction &action) const {
		return action < OpSin;
	}
	inline bool isTwoOperand(void) const {
		return _data.action < OpSin;
	}
	
	static Expr buildExpressionTree( \
		const char *expression) {
		unsigned char lenOperator;
		bool isTwoOperand;
		ArifmeticAction action {none};
		const char * ptrOperator { \
			_shearchNotPriorityOperator(expression, \
				lenOperator, action)}, * newExpression {nullptr};
		if (!ptrOperator)
			return std::make_shared<Expression>(expression);
		Expr result {std::make_shared<Expression>(action)};
		size_t lenExpression {0UL};
		if (isTwoOperand(action)) {
			lenExpression = (size_t)(ptrOperator - expression);
			newExpression = new char[lenExpression+1UL]{'\0'};
			strncpy(newExpression, expression, lenExpression);
			result->setFirstOperand(buildExpressionTree(newExpression));
			delete [] newExpression;
			lenExpression = (size_t)(strlen(expression) - \
				(ptrOperator - expression) - lenOperator);
		} else
			lenExpression = (size_t)(ptrOperator - expression + \
				strlen(expression) + lenOperator + 1UL);
		newExpression = new char[lenExpression+1UL]{'\0'};
		strncpy(newExpression, ptrOperator + lenOperator, lenExpression);
		result->setSecondOperand(buildExpressionTree(newExpression));
		delete [] newExpression;
		return result;
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
	inline const char *print(void) {
		char * expression {nullptr};
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
	/*
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
	*/
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
	inline Expr operator+(Expression * operand2 \
	) noexcept {
		return std::make_shared<Expression>( \
			OpAddition, shared_from_this(), operand2);
	}
	inline Expr operator-(Expression * operand2 \
	) noexcept {
		return std::make_shared<Expression>( \
			OpSubtraction, shared_from_this() operand2);
	}
	inline Expr mod(Expression * operand2 \
	) noexcept {
		return std::make_shared<Expression>( \
			OpRemainderFromDivision, shared_from_this(), operand2);
	}
	inline Expr pow(Expression * operand2 \
	) noexcept {
		return std::make_shared<Expression>( \
			OpPower, shared_from_this(), operand2);
	}
	inline Expr operator*(Expression * operand2 \
	) noexcept {
		return std::make_shared<Expression>( \
			OpMultiplication, shared_from_this(), operand2);
	}
	inline Expr operator/(Expression * operand2 \
	) noexcept {
		return std::make_shared<Expression>( \
			OpPower, shared_from_this(), operand2);
	}
	inline Expr log(Expression * operand2 \
	) noexcept {
		return std::make_shared<Expression>( \
			OpLog, shared_from_this(), operand2);
	}
	inline Expr sin(void) noexcept {
		return std::make_shared<Expression>(OpSin, shared_from_this());
	}
	inline Expr cos(void) noexcept {
		return std::make_shared<Expression>(OpCos, shared_from_this());
	}
	inline Expr tan(void) noexcept {
		return std::make_shared<Expression>(OpTan, shared_from_this());
	}
	inline Expr cot(void) noexcept {
		return std::make_shared<Expression>(OpCot, shared_from_this());
	}
	inline Expr sec(void) noexcept {
		return std::make_shared<Expression>(OpSec, shared_from_this());
	}
	inline Expr csc(void) noexcept {
		return std::make_shared<Expression>(OpCsc, shared_from_this());
	}
	inline Expr asin(void) noexcept {
		return std::make_shared<Expression>(OpAsin, shared_from_this());
	}
	inline Expr acos(void) noexcept {
		return std::make_shared<Expression>(OpAcos, shared_from_this()); 
	}
	inline Expr atan(void) noexcept {
		return std::make_shared<Expression>(OpAtan, shared_from_this());
	}
	inline Expr acot(void) noexcept {
		return std::make_shared<Expression>(OpAcot, shared_from_this());
	}
	inline Expr asec(void) noexcept {
		return std::make_shared<Expression>(OpAsec, shared_from_this());
	}
	inline Expr acsc(void) noexcept {
		return std::make_shared<Expression>(OpAcsc, shared_from_this());
	}
	inline Expr sqrt(void) noexcept {
		return std::make_shared<Expression>(OpSqrt, shared_from_this());
	}
	inline Expr sqrt(void) noexcept {
		return std::make_shared<Expression>(OpSqrt, shared_from_this());
	}
	inline Expr sqr(void) noexcept {
		return std::make_shared<Expression>(OpSqr, shared_from_this());
	}
	inline Expr cbrt(void) noexcept {
		return std::make_shared<Expression>(OpCbrt, shared_from_this());
	}
	inline Expr unaryMinus(void) noexcept {
		return std::make_shared<Expression>(OpUnaryMinus, shared_from_this());
	}
	inline Expr abs(void) noexcept {
		return std::make_shared<Expression>(OpAbs, shared_from_this());
	}
	inline Expr sgn(void) noexcept {
		return std::make_shared<Expression>(OpSgn, shared_from_this());
	}
	inline Expr lg(void) noexcept {
		return std::make_shared<Expression>(OpLg, shared_from_this());
	}
	inline Expr ln(void) noexcept {
		return std::make_shared<Expression>(OpLn, shared_from_this());
	}
	inline Expr exp(void) noexcept {
		return std::make_shared<Expression>(OpExp, shared_from_this());
	}
	inline Expr diff(void) const {
		switch (heandlerDifferentiate()) {
			case numberDi:
				return ZERO->copy();
			case variableDi:
				return ONE->copy();
		}
		Expr &operand1 {_operand1}, &operand2 {_operand2}, \
			result {nullptr};
		Diff operand1Di {operand1->heandlerDifferentiate()}, \
			operand2Di {nullptr};
		if (operand2) operand2Di = operand2->heandlerDifferentiate();

		const ArifmeticAction &action {_data.action};

        switch (action) {
			case OpAddition:
			case OpSubtraction:
				if (operand1Di == numberDi)
					result = _operand2->diff();
				else if (operand2Di == numberDi)
					result = _operand1->diff();
				else 
					result = std::make_shared<Expression>(action, \
						operand1->diff(), operand2->diff());
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
				else
					result = operand1->diff() * operand2 + \
						operand1 * operand2->diff();
				return result;
			case OpDivision:
				result = (operand1 * operand2)->diff() / operand2->sqr();
				return result;
			case OpPower:
				result = operand2 * operand1->pow(operand2 - ONE);
				break;
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
				if (operand1Di == variableDi) return result;
				break;
			case OpCos:
				result = operand1->sin()->unaryMinus();
				if (operand1Di == variableDi) return result;
				break;
			case OpTan:
				result = operand1->sec()->qsr();
				if (operand1Di == variableDi) return result;
				break;
			case OpCot:
				result = operand1->csc()->unaryMinus()->sqr();
				if (operand1Di == variableDi) return result;
				break;
			case OpSec:
				result = operand1->sec() * operand1->tan();
				if (operand1Di == variableDi) return result;
				break;
			case OpCsc:
				result = operand1->csc()->unaryMinus() * operand1->cot();
				if (operand1Di == variableDi) return result;
				break;
			case OpAbs:
				result = operand1->sgn();
				if (operand1Di == variableDi) return result;
				break;
			case OpUnaryMinus:
				return operand1->diff()->unaryMinus();
			case OpRemainderFromDivision:
				return operand1->diff() - operand2->diff() * \
					(operand1 / operand2)->abs();
			case OpAsin:
				result = ONE / (ONE - operand1->sqr())->sqrt();
				if (operand1Di == variableDi) return result;
				break;
			case OpAcos:
				result = (ONE / (ONE - operand1->sqr())->sqrt())->unaryMinus();
				if (operand1Di == variableDi) return result;
				break;
			case OpAtan:
				result = ONE / (ONE + operand1->sqr())->sqrt();
				if (operand1Di == variableDi) return result;
				break;
			case OpAcot:
				result = (ONE / (ONE + operand1->sqr())->sqrt())->unaryMinus();
				if (operand1Di == variableDi) return result;
				break;
			case OpAsec:
				result = ONE / (operand1->abs() * \
					(ONE - operand1->sqr())->sqrt())
				if (operand1Di == variableDi) return result;
				break;
			case OpAcsc:
				result = ONE / (operand1->abs() * \
					(ONE - operand1->sqr())->sqrt());
				if (operand1Di == variableDi) return result;
				break;
    
			case OpSgn:
				return ZERO;
		}
		return std::make_shared<Expression>( \
			OpMultiplication, result, operand1->diff());
	}
	inline Expression *integrate(Expression * parent = 0L) const {
		switch (heandlerDifferentiate()) {
			case numberDi:
				return shared_from_this() * VAR_X;
			case variableDi:
				return shared_from_this()->sqr() / TWO;
		}
		Expr result {nullptr};
		const ArifmeticAction &action {_data.action};
		Diff operand1Di {_operand1->heandlerDifferentiate()}, \
			operand2Di {noneDi};
		if (_operand2) operand2Di = _operand2->heandlerDifferentiate();
		switch (action) {
			case OpAddition:
			case OpSubtraction:
				return std::make_shared<Expression>(action, \
					operand1->integrate(), operand2->integrate());
			case OpMultiplication:
				// Попробовать упростить (если один из операндов — константа)
				if (operand1Di == numberDi)
					result = operand1 * operand2->integrate();
				else if (operand2Di == numberDi)
					result = operand1->integrate() * operand2;
				// Иначе применить интегрирование по частям: ∫u dv = uv - ∫v du
				else result = operand1 * operand2->integrate() - \
					(operand2->integrate() * operand1->diff())->integrate();
				return result;
			case OpDivision:  // Деление: ∫(u / v) dx → зависит от формы u и v
				return (ONE / operand1 * operand2)->integrate();
			case OpPower:
				return (operand2 * operand1->ln())->exp();
			case OpAbs:
				return operand1 * operand1->abs() / TWO;
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
Expression Expression::ZERO, Expression::ONE, \
	Expression::TWO, Expression::TEN, \
	Expression::MINUS_ONE, Expression::VAR_X;

