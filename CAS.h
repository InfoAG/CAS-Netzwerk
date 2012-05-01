#include <string>
#include "lexer.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <list>
#include <algorithm>

using namespace std;

struct ArithmeticExpression;
struct UnaryOperation;
struct BinaryOperation;
struct LevelingOperation;
struct Addition;
struct Subtraction;
struct Multiplication;
struct Division;
struct Exponentiation;
struct SquareRoot;
struct Logarithm;
struct NaturalLogarithm;
struct Sinus;
struct Cosinus;
struct Tangent;
struct NumericalValue;
struct VariableExpression;
struct CommandExpression;
struct FunctionExpression;
struct Integral;
struct Variable;
struct Function;
struct Command;
struct CASInformation;
struct ExpansionInformation;

struct ArithmeticExpression {
	static ArithmeticExpression *create(string);
	virtual ArithmeticExpression *copy() const = 0;

	virtual ArithmeticExpression *expand(const ExpansionInformation&) const = 0;
	virtual ArithmeticExpression *formPolynom() const;
	virtual Multiplication *formMonom() const;
	virtual string getString() const = 0;
	virtual bool isEqual(ArithmeticExpression *) const = 0;

	virtual ArithmeticExpression *sqrt_func();
	virtual ArithmeticExpression *log_func();
	virtual ArithmeticExpression *ln_func();
	virtual ArithmeticExpression *sin_func();
	virtual ArithmeticExpression *cos_func();
	virtual ArithmeticExpression *tan_func(); 

	friend ostream& operator << (ostream&, const ArithmeticExpression&);
};

struct UnaryOperation : public ArithmeticExpression {
	ArithmeticExpression *operand;

	UnaryOperation(ArithmeticExpression *o) : operand(o) {};
	virtual ArithmeticExpression *copy() const = 0;
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const = 0;
	virtual string getString() const = 0;
	virtual bool isEqual(ArithmeticExpression *) const;
};

struct BinaryOperation : public ArithmeticExpression {
	ArithmeticExpression *left;
	ArithmeticExpression *right;

	BinaryOperation(ArithmeticExpression *l, ArithmeticExpression *r) : left(l), right(r) {};
	virtual ArithmeticExpression *copy() const = 0;
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const = 0;
	virtual string getString() const = 0;
	virtual bool isEqual(ArithmeticExpression *) const;
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

struct Addition : public LevelingOperation {
	Addition() {};
	virtual ArithmeticExpression *copy() const { return new Addition(*this); };
	Addition(const list<ArithmeticExpression*>& va) : LevelingOperation(va) {}; 
	Addition(ArithmeticExpression *l, ArithmeticExpression *r) : LevelingOperation(l, r) {};
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};
struct Subtraction : public BinaryOperation {
	Subtraction(ArithmeticExpression *l, ArithmeticExpression *r) : BinaryOperation(l, r) {};
	virtual ArithmeticExpression *copy() const { return new Subtraction(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual string getString() const;
};
struct Multiplication : public LevelingOperation {
	void addexp(ArithmeticExpression*, ArithmeticExpression*, vector<Exponentiation*>&) const;
	Multiplication(const list<ArithmeticExpression*>& va) : LevelingOperation(va) {};
	virtual ArithmeticExpression *copy() const { return new Multiplication(*this); };
	Multiplication(ArithmeticExpression *l, ArithmeticExpression *r) : LevelingOperation(l, r) {};
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual ArithmeticExpression *formPolynom() const;
	virtual Multiplication *formMonom() const;
	virtual string getString() const;
};
struct Division : public BinaryOperation {

	Division(ArithmeticExpression *l, ArithmeticExpression *r) : BinaryOperation(l, r) {};
	virtual ArithmeticExpression *copy() const { return new Division(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual string getString() const;
};
struct Exponentiation : public BinaryOperation {

	Exponentiation(ArithmeticExpression *l, ArithmeticExpression *r) : BinaryOperation(l, r) {};
	virtual ArithmeticExpression *copy() const { return new Exponentiation(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual ArithmeticExpression *formPolynom() const;
	virtual string getString() const;
};
struct SquareRoot : public UnaryOperation {

	SquareRoot(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new SquareRoot(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual string getString() const;
};
struct Logarithm : public UnaryOperation {

	Logarithm(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new Logarithm(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual string getString() const;
};
struct NaturalLogarithm : public UnaryOperation {

	NaturalLogarithm(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new NaturalLogarithm(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual string getString() const;
};
struct Sinus : public UnaryOperation {

	Sinus(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new Sinus(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual string getString() const;
};
struct Cosinus : public UnaryOperation {

	Cosinus(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new Cosinus(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual string getString() const;
};
struct Tangent : public UnaryOperation {

	Tangent(ArithmeticExpression *o) : UnaryOperation(o) {};
	virtual ArithmeticExpression *copy() const { return new Tangent(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual string getString() const;
};

struct NumericalValue : public ArithmeticExpression {
	double value;
	
	NumericalValue(double d) : value(d) {};
	virtual ArithmeticExpression *copy() const { return new NumericalValue(*this); };
	NumericalValue(string str) { value = strtod(str.c_str(), NULL); return; };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual ArithmeticExpression* formPolynom() const;
	virtual string getString() const;
	virtual bool isEqual(ArithmeticExpression *) const;

	virtual ArithmeticExpression *sqrt_func() { return new NumericalValue(sqrt(value)); }
	virtual ArithmeticExpression *log_func() { return new NumericalValue(sqrt(value)); }
	virtual ArithmeticExpression *ln_func() { return new NumericalValue(sqrt(value)); }
	virtual ArithmeticExpression *sin_func() { return new NumericalValue(sqrt(value)); }
	virtual ArithmeticExpression *cos_func() { return new NumericalValue(sqrt(value)); }
	virtual ArithmeticExpression *tan_func() { return new NumericalValue(sqrt(value)); }
};

struct VariableExpression : public ArithmeticExpression {
	string identifier;

	VariableExpression(const string& i) : identifier(i) {};
	virtual ArithmeticExpression *copy() const { return new VariableExpression(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual string getString() const;
	virtual bool isEqual(ArithmeticExpression *) const;
};

struct CommandExpression : public ArithmeticExpression {

	int id;

	CommandExpression(int i) : id(i) {};
	virtual ArithmeticExpression *copy() const { return new CommandExpression(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const ;
	virtual string getString() const;
	virtual bool isEqual(ArithmeticExpression *) const;
};

struct FunctionExpression : public ArithmeticExpression {
	string identifier;
	vector<ArithmeticExpression*> argvec;

	FunctionExpression(const string& i, const vector<ArithmeticExpression*>& av) : identifier(i), argvec(av) {};
	virtual ArithmeticExpression *copy() const { return new FunctionExpression(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual string getString() const;
	virtual bool isEqual(ArithmeticExpression *) const;
};

struct Integral : public ArithmeticExpression {
	ArithmeticExpression *aexp;
	VariableExpression *var;
	double lower_bound;
	double upper_bound;
	int intervals;

	Integral(ArithmeticExpression *ax, VariableExpression *v, double lb, double ub, int iv) : aexp(ax), var(v), lower_bound(lb), upper_bound(ub), intervals(iv) {};
	virtual ArithmeticExpression *copy() const { return new Integral(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const;
	virtual string getString() const;
	virtual bool isEqual(ArithmeticExpression *) const;
};

struct Vector : public ArithmeticExpression {
	ArithmeticExpression *coordinates[3];

	Vector(ArithmeticExpression *c[]) { for (int i = 0; i < 3; i++) coordinates[i]=c[i]; };
	virtual ArithmeticExpression *copy() const { return new Vector(*this); };
	virtual ArithmeticExpression *expand(const ExpansionInformation&) const { return copy(); };
	virtual string getString() const { return string("[") + coordinates[0]->getString() + string("|") + coordinates[1]->getString() + string("|") + coordinates[2]->getString() + string("]"); };
	virtual bool isEqual(ArithmeticExpression *) const;
};

struct Matrix : public ArithmeticExpression {
	ArithmeticExpression ***components;
	int size_x, size_y;

	Matrix(ArithmeticExpression ***, int, int);
	virtual ArithmeticExpression *copy() const;
	virtual ArithmeticExpression *expand();
	virtual string getString() const;
	virtual bool isEqual(ArithmeticExpression*) const;
};

struct Variable {
	string identifier;
	ArithmeticExpression *aexp;
	bool expanded;

	Variable(const string& n, ArithmeticExpression *ax) : identifier(n), aexp(ax), expanded(false) {};
	string getString() const;

	friend ostream& operator<<(ostream&, const Variable&);
};

struct Function {
	string identifier;
	vector<string> func_args;
	ArithmeticExpression *aexp;
	bool expanded;

	Function(const string& i, vector<string>& fargs, ArithmeticExpression *ax) : identifier(i), func_args(fargs), aexp(ax), expanded(false) {};
	string getString() const;

	friend ostream& operator<<(ostream&, const Function&);
};

struct Command {
	ArithmeticExpression *aexp;
	bool expanded;

	Command(ArithmeticExpression *ax) : aexp(ax), expanded(false) {};
	string getString() const;

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
	bool FunctionsModified;
	bool VariablesModified;
	bool CommandsModified;

public:
	CAS() : FunctionsModified(false), VariablesModified(false), CommandsModified(false) {};
	string process(string);

	const vector<Variable>& getVariables() const { return casinfo.variables; };
	const vector<Function>& getFunctions() const { return casinfo.functions; };
	const vector<Command>& getCommands() const { return casinfo.commands; };
	bool functionsModified() const { return FunctionsModified; };
	bool variablesModified() const { return VariablesModified; };
	bool commandsModified() const { return CommandsModified; };

	void deleteVariable(string);
	void deleteFunction(string);
	void clearVariables();
	void clearFunctions();
	void reset();
};