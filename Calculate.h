#include <vector>
//#include "decimal.h"
#include <gmp.h>

enum ArifmeticAction { \
	addition, subtraction, \
	remainderFromDivision, \
	multiplication, division, \
	cos, sin, tan, sec, csc, cot, sgn, \
	acos, asin, atan, asec, acsc, acot \
}
union Data {mpf_t number; ArifmeticAction action};
class Expression {
private:
	Expression *_parent, *_operand1, *_operand2;
	Data _data;
	bool _isNumber;
public:
	Expression(mpf_t number, \
		Expression *parent = nullptr) 
		: isNumber(true), _parent(parent), \
		_data{.number = number} {}
	Expression(ArifmeticAction action, \
		Expression *parent = nullptr)
		: _isNumber(false), _parent(parent), \
		_data{.action = action} {}
	static mpf_t calculate(Expression *expression) {
		if (expression->_isNumber) {
			mpf_t number{expression->_data.number};
			delete expression;
			return number;
		}
		mpf_t result;
		switch (expression->_data.action) {
			case addition:
				mpf_add(result, Expression::calculate(expression->_operand1), \
						Expression::calculate(expression->_operand2));
				break;
			case subtraction:
				mpf_sub(result, Expression::calculate(expression->_operand1), \
						Expression::calculate(expression->_operand2));
				break;
			case remainderFromDivision:
				mpf_ result = Expression::calculate(expression->_operand1) % \
						Expression::calculate(expression->_operand2);
				break;
			case multiplication:
				mpf_mul(result, Expression::calculate(expression->_operand1), \
						Expression::calculate(expression->_operand2));
				break;
			case division:
				mpf_div(result, Expression::calculate(expression->_operand1), \
						Expression::calculate(expression->_operand2));
				break;
			case sin:
				mpf_sinresult = Expression::calculate(expression->_operand1).sin();
				break;
			case cos:
				result = Expression::calculate(expression->_operand1).cos();
				break;
			case tan:
				result = Expression::calculate(expression->_operand1).tan();
				break;
			case sec:
				result = Expression::calculate(expression->_operand1).sec();
				break;
			case csc:
				result = Expression::calculate(expression->_operand1).csc();
				break;
			case cot:
				result = Expression::calculate(expression->_operand1).cot();
				break;
			case asin:
				result = Expression::calculate(expression->_operand1).asin();
				break;
			case acos:
				result = Expression::calculate(expression->_operand1).acos();
				break;
			case atan:
				result = Expression::calculate(expression->_operand1).atan();
				break;
			case asec:
				result = Expression::calculate(expression->_operand1).asec();
				break;
			case acsc:
				result = Expression::calculate(expression->_operand1).acsc();
				break;
			case acot:
				result = Expression::calculate(expression->_operand1).acot();
				break;
			case sgn:
				result = Expression::calculate(expression->_operand1).sgn();
		}
		delete expression;
		return result;
	}
};
class CalculateBasic {
	public:
