#include <vector>
#include "decimal.h"

enum ArifmeticAction {
	addition, subtraction, \
	remainderFromDivision, \
	multiplication, division, \
	cos, sin, tan, sec, csc, cot, sgn \
	acos, asin, atan, asec, acsc, acot
}
union Data {dec::decimal<30> number, ArifmeticAction action};
class Expression {
private:
	Expression *_parent, *_operand1, *_operand2;
	Data _data;
	bool _isNumber;
public:
	Expression(dec::decimal<30> number, \
		Expression *parent = nullptr) 
		: isNumber(true), _parent(parent), \
		_data(.number = number) {}
	Expression(ArifmeticAction action, \
		Expression *parent = nullptr)
		: _isNumber(false), _parent(parent), \
		_data(.action = action) {}
	static dec::decimal<30> calculate(Expression *expression) {
		if (expression->_isNumber) {
			dec::decimal<30> number{expression->_data.number};
			delete expression;
			return number;
		}
		dec::decimal<30> result;
		switch (expression->_data.action) {
			case addition:
				result = Expression::calculate(expression->_operand1) + \
						 Expression::calculate(expression->_operand2);
				break;
			case subtraction:
				result = Expression::calculate(expression->_operand1) - \
						 Expression::calculate(expression->_operand2);
				break;
			case remainderFromDivision:
				result = Expression::calculate(expression->_operand1) % \
						 Expression::calculate(expression->_operand2);
				break;
			case multiplication:
				result = Expression::calculate(expression->_operand1) * \
						 Expression::calculate(expression->_operand2);
				break;
			case division:
				result = Expression::calculate(expression->_operand1) / \
						 Expression::calculate(expression->_operand2);
				break;
			case sin:
				result = Expression::calculate(expression->_operand1).sin();
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
