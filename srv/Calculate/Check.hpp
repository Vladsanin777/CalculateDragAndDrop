#pragma once

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