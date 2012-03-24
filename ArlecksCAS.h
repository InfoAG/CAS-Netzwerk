#include <string>
#include "lexer.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <list>
#include <algorithm>

using namespace std;

void Fehler(string);

//enum knotentyp { ADDITION, SUBTRAKTION, MULTIPLIKATION, DIVISION, POTENZ, WURZEL, LOG, LN, SINUS, COSINUS, TANGENS, NUMBER, VAR, FUNCTION };

class Exponentiation;
struct Variable;
struct Function;
struct Command;
struct CASInformation;
struct ExpansionInformation;
class CAS;
class Multiplication;

class ArithmeticExpression {
public:

	//ArithmeticExpression();
	static ArithmeticExpression *create(string);
	virtual ArithmeticExpression *copy() const = 0;

	virtual ArithmeticExpression *expand(const ExpansionInformation&) const = 0;
	virtual ArithmeticExpression *formPolynom() const;
	virtual Multiplication *formMonom() const;
	virtual string getString() const = 0;
	virtual bool isEqual(ArithmeticExpression *) const = 0;
	//virtual double getVal() { return 0; };
	//virtual bool containsVariable(string) const = 0;
	//virtual bool containsFunction(string) const = 0;

	//virtual ArithmeticExpression *add(ArithmeticExpression*);
	//virtual ArithmeticExpression *subtract(ArithmeticExpression *r);
	//virtual ArithmeticExpression *multiply(ArithmeticExpression *r);
	//virtual ArithmeticExpression *divide(ArithmeticExpression *r);
	//virtual ArithmeticExpression *power(ArithmeticExpression *r);
	virtual ArithmeticExpression *sqrt_func();
	virtual ArithmeticExpression *log_func();
	virtual ArithmeticExpression *ln_func();
	virtual ArithmeticExpression *sin_func();
	virtual ArithmeticExpression *cos_func();
	virtual ArithmeticExpression *tan_func();

	friend ostream& operator << (ostream&, const ArithmeticExpression&);
};

//class ArithmeticExpression {
//public:
//
//	ArithmeticExpression(string);
//
//	ArithmeticExpression expand(const ExpansionInformation&) const;
//	string getString() const;
//
//	friend ostream& operator << (ostream&, const ArithmeticExpression&);
//};

struct UnaryOperation : public ArithmeticExpression {
	ArithmeticExpression *operand;

	UnaryOperation(ArithmeticExpression *o) : operand(o) {};
	virtual ArithmeticExpression *copy() const = 0;
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const = 0;
	//virtual ArithmeticExpression *formPolynom() const = 0;
	virtual string getString() const = 0;
	virtual bool isEqual(ArithmeticExpression *) const;
	//virtual inline bool containsVariable(string id) const { return operand->containsVariable(id); };
	//virtual inline bool containsFunction(string id) const { return operand->containsFunction(id); };
};

struct BinaryOperation : public ArithmeticExpression {
	ArithmeticExpression *left;
	ArithmeticExpression *right;

	BinaryOperation(ArithmeticExpression *l, ArithmeticExpression *r) : left(l), right(r) {};
	virtual ArithmeticExpression *copy() const = 0;
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const = 0;
	//virtual ArithmeticExpression *formPolynom() const = 0;
	virtual string getString() const = 0;
	virtual bool isEqual(ArithmeticExpression *) const;
	//virtual inline bool containsVariable(string id) const { return (left->containsVariable(id) || right->containsVariable(id)); };
	//virtual inline bool containsFunction(string id) const { return (left->containsFunction(id) || right->containsFunction(id)); };
};

struct LevelingOperation : public ArithmeticExpression {
	list<ArithmeticExpression*> operands;

	LevelingOperation() {};
	virtual ArithmeticExpression *copy() const = 0;
	LevelingOperation(const list<ArithmeticExpression*>& va) : operands(va) {};
	LevelingOperation(ArithmeticExpression *l, ArithmeticExpression *r) { list<ArithmeticExpression*> tmpvec; tmpvec.push_back(l); tmpvec.push_back(r); operands.assign(tmpvec.begin(), tmpvec.end()); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const = 0;
	virtual ArithmeticExpression *formPolynom() const = 0;
	virtual string getString() const = 0;
	virtual bool isEqual(ArithmeticExpression *) const;
};

class Addition : public LevelingOperation {
public:
	Addition() {};
	virtual ArithmeticExpression *copy() const { return new Addition(*this); };
	Addition(const list<ArithmeticExpression*>& va) : LevelingOperation(va) {}; 
	Addition(ArithmeticExpression *l, ArithmeticExpression *r) : LevelingOperation(l, r) {};
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};
class Subtraction : public BinaryOperation {
public:
	Subtraction(ArithmeticExpression *l, ArithmeticExpression *r) : BinaryOperation(l, r) {};
	virtual ArithmeticExpression *copy() const { return new Subtraction(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};
class Multiplication : public LevelingOperation {
private:
	void addexp(ArithmeticExpression*, ArithmeticExpression*, vector<Exponentiation*>&) const;
public:
	Multiplication(const list<ArithmeticExpression*>& va) : LevelingOperation(va) {};
	virtual ArithmeticExpression *copy() const { return new Multiplication(*this); };
	Multiplication(ArithmeticExpression *l, ArithmeticExpression *r) : LevelingOperation(l, r) {};
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual ArithmeticExpression *formPolynom() const;
	virtual Multiplication *formMonom() const;
	virtual string getString() const;
};
class Division : public BinaryOperation {
public:
	Division(ArithmeticExpression *l, ArithmeticExpression *r) : BinaryOperation(l, r) {};
	virtual ArithmeticExpression *copy() const { return new Division(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};
class Exponentiation : public BinaryOperation {
public:
	Exponentiation(ArithmeticExpression *l, ArithmeticExpression *r) : BinaryOperation(l, r) {};
	virtual ArithmeticExpression *copy() const { return new Exponentiation(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};
class SquareRoot : public UnaryOperation {
public:
	SquareRoot(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new SquareRoot(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};
class Logarithm : public UnaryOperation {
public:
	Logarithm(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new Logarithm(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};
class NaturalLogarithm : public UnaryOperation {
public:
	NaturalLogarithm(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new NaturalLogarithm(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};
class Sinus : public UnaryOperation {
public:
	Sinus(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new Sinus(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};
class Cosinus : public UnaryOperation {
public:
	Cosinus(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new Cosinus(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};
class Tangent : public UnaryOperation {
public:
	Tangent(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new Tangent(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};

class NumericalValue : public ArithmeticExpression {
public:
	double value;
	NumericalValue(double d) : value(d) {};
	virtual ArithmeticExpression *copy() const { return new NumericalValue(*this); };
	NumericalValue(string str) { value = strtod(str.c_str(), NULL); return; };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
	virtual bool isEqual(ArithmeticExpression *) const;
	//virtual inline bool containsVariable(string id) const { return false; };
	//virtual inline bool containsFunction(string id) const { return false; };

	//virtual ArithmeticExpression *add(ArithmeticExpression*);
	//virtual ArithmeticExpression *subtract(ArithmeticExpression *r);
	//virtual ArithmeticExpression *multiply(ArithmeticExpression *r);
	//virtual ArithmeticExpression *divide(ArithmeticExpression *r);
	//virtual ArithmeticExpression *power(ArithmeticExpression *r);
	virtual ArithmeticExpression *sqrt_func();
	virtual ArithmeticExpression *log_func();
	virtual ArithmeticExpression *ln_func();
	virtual ArithmeticExpression *sin_func();
	virtual ArithmeticExpression *cos_func();
	virtual ArithmeticExpression *tan_func();
};

class VariableExpression : public ArithmeticExpression {
private:
	string identifier;
public:
	VariableExpression(const string& i) : identifier(i) {};
	virtual ArithmeticExpression *copy() const { return new VariableExpression(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
	virtual bool isEqual(ArithmeticExpression *) const;
	//virtual inline bool containsVariable(string id) const { return (identifier == id); };
	//virtual inline bool containsFunction(string id) const { return false; };
};

class CommandExpression : public ArithmeticExpression {
private:
	int id;
public:
	CommandExpression(int i) : id(i) {};
	virtual ArithmeticExpression *copy() const { return new CommandExpression(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const ;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
	virtual bool isEqual(ArithmeticExpression *) const;
	//virtual inline bool containsVariable(string id) const;
	//virtual inline bool containsFunction(string id) const;
};

class FunctionExpression : public ArithmeticExpression {
private:
	string identifier;
	vector<ArithmeticExpression*> argvec;
public:
	FunctionExpression(const string& i, const vector<ArithmeticExpression*>& av) : identifier(i), argvec(av) {};
	virtual ArithmeticExpression *copy() const { return new FunctionExpression(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
	virtual bool isEqual(ArithmeticExpression *) const;
	//virtual bool containsVariable(string id) const;
	//virtual bool containsFunction(string id) const { return (identifier == id); };
};

class Integral : public ArithmeticExpression {
public:
	ArithmeticExpression *aexp;
	VariableExpression *var;
	double lower_bound;
	double upper_bound;
	int intervals;

	Integral(ArithmeticExpression *ax, VariableExpression *v, double lb, double ub, int iv) : aexp(ax), var(v), lower_bound(lb), upper_bound(ub), intervals(iv) {};
	virtual ArithmeticExpression *copy() const { return new Integral(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	//virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
	virtual bool isEqual(ArithmeticExpression *) const;
};

struct Variable {
	string identifier;
	ArithmeticExpression *aexp;
	bool expanded;

	Variable(const string& n, ArithmeticExpression *ax) : identifier(n), aexp(ax), expanded(false) {};

	friend ostream& operator<<(ostream&, const Variable&);
};

struct Function {
	string identifier;
	vector<string> func_args;
	ArithmeticExpression *aexp;
	bool expanded;

	Function(const string& i, vector<string>& fargs, ArithmeticExpression *ax) : identifier(i), func_args(fargs), aexp(ax), expanded(false) {};
	friend ostream& operator<<(ostream&, const Function&);
};

struct Command {
	ArithmeticExpression *aexp;
	bool expanded;

	Command(ArithmeticExpression *ax) : aexp(ax), expanded(false) {};
	friend ostream& operator<<(ostream&, const Command&);
};

struct CASInformation {
	vector<Variable> variables;
	vector<Function> functions;
	vector<Command> commands;

	CASInformation() {};
	CASInformation(const vector<Variable>& v, const vector<Function>& f, const vector<Command>& c) : variables(v), functions(f), commands(c) {};
};

struct ExpansionInformation : CASInformation{
	vector<Variable> funcvars;

	ExpansionInformation() {};
	ExpansionInformation(const CASInformation& casinfo) : CASInformation(casinfo) {};
	ExpansionInformation(const vector<Variable>& v, const vector<Function>& f, const vector<Command>& c) : CASInformation(v, f, c) {};
	ExpansionInformation(const vector<Variable>& v, const vector<Function>& f, const vector<Command>& c, const vector<Variable>& fv) : CASInformation(v, f, c), funcvars(fv) {};
};

class CAS {
private:
	CASInformation casinfo;
public:
	string process(string);

	const vector<Variable>& getVariables() const { return casinfo.variables; };
	const vector<Function>& getFunctions() const { return casinfo.functions; };
	const vector<Command>& getCommands() const { return casinfo.commands; };

	void deleteVariable(string);
	void deleteFunction(string);
	void clearVariables();
	void clearFunctions();
	void reset() { casinfo.commands.clear(); casinfo.functions.clear(); casinfo.variables.clear(); }
};