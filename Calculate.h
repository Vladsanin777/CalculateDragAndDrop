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
	sin, cos, tan, sec, csc, cot, sgn, \
	acos, asin, atan, asec, acsc, acot, \
	sqrt, qrt, cbrt, unaryMinus \
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

union NumOrVar {mpfr_t number; const char * variable}
union Data {NumOrVar operand; ArifmeticAction action;};
class Expression {
private:
	Expression *_parent, *_operand1, *_operand2;
	Data _data;
	bool _isOperand;
	bool _isVariable;

	static size_t size;
	static mpfr_t ZERO, ONE;
	inline explicit Expression (
		const char * const number, \
		Expression * parent, \
		bool isDelete = true \
	) : _isOperand{true}, _parent{parent} {
		//puts("jkk");
		//puts(expression);
		if (isdigit(*number) || *number == '-') {
			_isVariable = true;
			mpfr_init2(_data.operand.number, size);
			mpfr_set_str(_data.operand.number, number, 10, MPFR_RNDN);
			//puts("ndh");
		} else {
			_isVariable = false;
			char * variable = new char[strlen(number)+1UL];
			strcpy(variable, number);
			_data.operand.variable = variable;
		}
		if (isDelete) delete [] number;
	}
	inline explicit Expression(ArifmeticAction action, \
		Expression *parent = nullptr)
		: _isNumber{false}, _parent{parent}, \
		_data{.action = action} {}
	inline explicit Expression(void) {}
public:
	static void init(void) {
		mpfr_init2(ZERO, size);
		mpfr_set_str(ZERO, "0", 8, MPFR_RNDN);
		mpfr_init2(ONE, size);
		mpfr_set_str(ONE, "1", 8, MPFR_RNDN);
		return;
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
		if (_isOperand) {
			//mpfr_printf("%Rf\n", _data.number);
			mpfr_clear(_data.number);
		}
	}
	inline one(Expression * parent) {
		this = new Expression {"1", parent};
	}
	inline Expression * copy(Expression * parent = 0L) {
		Expression *result {new Expression{}};
		*result = *this;
		result->_parent = parent;
		return result;
	}
	inline Expression * diff(Expression * parent = 0L) const {
		Expression * result = Expression{};
		result->_parent = parent;
		if (_isOperand) {
			result->_isOperand = true;
			result->_isVariable = false;
			mpfr_t &number = result._data.operand.number;
			mpfr_init2(number, size);
			if (expression->_isVariable){
				mpfr_set(number, ONE, MPFR_RNDN);
				return result;
			}
			mpfr_set(number, ZERO, MPFR_RNDN);
			return result;
		}
		result->_isOperand = false;
		ArifmeticAction &action {_data.action}, \
			&resAction{result->_data.action};
		Expression * &resOperand1 {result->_operand1}, \
			* &resOperand2 {result->_operand2};
        switch (action) {
			case addition:
			case subtraction:
				resAction = action;
				resOperand1 = _operand1->diff(result);
				resOperand2 = _operand2->diff(result);
				return result;
			case multiplication:
				resAction = addition;
				resOperand1 = new Expression{multiplication, result};
				resOperand1->_operand1 = _operand1->diff(resOperand1);
				resOperand1->_operand2 = _operand2->copy(resOperand1);
				resOperand2 = new Expression{multiplication, result};
				resOperand2->_operand1 = _operand1->copy;
				resOperand2->_operand2 = _operand2->diff(resOperand2);
				return result;
			case division:
				resAction = division;
				resOperand1 = new Expression{addition, result};
				Expression * resOperand1Operand1 { \
					new Expression {multiplication, resOperand1}}, \
				* resOperand1Operand2 { \
					new Expression {multiplication, resOperand1}};
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
				resOperand1 = new Expression{multiplication, result};
				resOperand2 = _operand1->diff(result);
				resOperand1->_operand1 = _operand2->copy(resOperand1);
				Expression * resOperand1Operand2 { \
					new Expression{power, resOperand1}};
				resOperand1Operand2->_operand1 = \
					_operand1->copy(resOperand1Operand2);
				Expression * resOperand1Operand2Operand2 { \
					new Expression{subtraction, resOperand1Operand2}};
				resOperand1Operand2Operand2->_operand1 = \
					_operand2->copy(resOperand1Operand2Operand2);
				resOperand1Operand2Operand2->_operand2->one();
				resOperand1->_operand2 = resOperand1Operand2;

				return result;
			case sin:
				resAction = cos;
				resOperand1 = _

            case 1:
                print(expression)
                if len(expression[0]) == 1:
                    expression = '0' if expression[0][0] in "0123456789" else '1'
                else: 
                    expression = '0' if expression[0][1] in "0123456789" else '1'
            case 2:
                if expression[1][0] in "0123456789":
                    expression = '0'
                else:
                    expression_0: str
                    expression_1: Union[str, list] = expression[1]
                    complex_expression: bool = isinstance(expression_1, list)
                    is_minus: bool = expression[0][0] == '-'
                    match expression_0[-6:]:
                        case 'arcsin':
                            expression = [
                                '1', '/', ['sqrt', ['1', '-',
                                [expression_1, '^', '2']]]]
                        case 'arccos':
                            expression = [
                                '-1', '/', ['sqrt', ['1', '-', 
                                [expression_1, '^', '2']]]]
                        case 'arctan':
                            expression = [
                                '1', '/', ['sqrt', ['1', '+',
                                [expression_1, '^', '2']]]]
                        case 'arccot':
                            expression = [
                                '-1', '/', ['sqrt', ['1', '+', 
                                [expression_1, '^', '2']]]]
                        case 'arcsec':
                            expression = [
                                '1', '/', [['abs', expression_1],
                                '*', ['sqrt', ['1', '-',
                                [expression_1, '^', '2']]]]]
                        case 'arccsc':
                            expression = [
                                '-1', '/', [['abs', expression_1],
                                '*', ['sqrt', ['1', '-',
                                [expression_1, '^', '2']]]]]
                        case _:
                            match expression_0[-3:]:
                                case 'sin':
                                    expression = [
                                        '-cos' if is_minus else 'cos', 
                                        expression_1
                                    ]
                                case 'cos':
                                    expression = [
                                        'sin' if is_minus else '-sin',
                                        expression_1
                                    ]
                                case 'tan':
                                    expression = [
                                        [
                                            '-sec' if is_minus else 'sec',
                                            expression_1
                                        ], '^', '2']
                                case 'cot':
                                    expression = [
                                        [
                                            'csc' if is_minus else '-csc',
                                            expression_1
                                        ], '^', '2']
                                case 'sec': 
                                    expression = [
                                        [
                                            '-sec' if is_minus else 'sec',
                                            expression_1
                                        ],
                                        '*',
                                        ['tan', expression_1]
                                    ]
                                case 'csc':
                                    expression = [
                                        [
                                            'csc' if is_minus else '-csc',
                                            expression_1
                                        ],
                                        '*',
                                        ['cot', expression_1]
                                    ]
                                case 'abs':
                                    expression = ['sgn', expression_1]
                                case 'sgn':
                                    expression = '0'
                                case 'log':
                                    if len(expression_1) > 1 and expression_1[1] == '|':
                                        expression = ['1', '/', [expression_1[2], 
                                            '*', ['ln', expression_1[0]]]]
                                        expression = expression_1[2]
                                    else:
                                        expression = ['1', '/', expression_1]
                                case _:
                                    match expression_0[-2:]:
                                        case 'ln':
                                            expression = ['1', '/', expression_1]
                                        case 'lg':
                                            expression = ['1', '/', [expression_1, '*', ['ln', '10']]]
                    expression = [expression, '*', self.ordinar_derivate(expression_1)]

            case 3:
                expression_0: Union[str, list] = expression[0]
                expression_2: Union[str, list] = expression[2]
                match expression[1]:
                    
                    case "^":
                        expression = [
                            [
                                expression_2,
                                '*',
                                [
                                    expression_0,
                                    '^',
                                    [expression_2, '-', '1']
                                ]
                            ],
                            '*',
                            self.ordinar_derivate(expression_0)
                        ]
                    case "|":
                        ...
                        

        return expression
	}
};

size_t Expression::size = 256;
