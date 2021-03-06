#include "CAS.h"

string dtostr(double d) {
	stringstream ss;
	ss << d;
	return ss.str();
}

bool compareOperands(const list<ArithmeticExpression*>& first, const list<ArithmeticExpression*>& second) {
	bool deleted;
	if (second.size() == first.size()) {
		list<ArithmeticExpression*> lo(second);
		for (list<ArithmeticExpression*>::const_iterator it = first.begin(); it != first.end(); ++it){
			deleted = false;
			list<ArithmeticExpression*>::iterator it_c;
			for (it_c = lo.begin(); it_c != lo.end(); ++it_c) {
				if ((*it)->isEqual(*it_c)) {
					lo.erase(it_c);
					deleted = true;
					break;
				}
			}
			if (! deleted) return false;
		}
		return true;
	} else return false;
}

//template<typename T, typename P>
//T remove_if(T beg, T end, P pred)
//{
//    T dest = beg;
//    for (T itr = beg;itr != end; ++itr)
//        if (!pred(*itr))
//            *(dest++) = *itr;
//    return dest;
//}

bool polysort(ArithmeticExpression *left, ArithmeticExpression *right) {
	return (dynamic_cast<Exponentiation*>(dynamic_cast<Multiplication*>(left)->operands.back())->right < dynamic_cast<Exponentiation*>(dynamic_cast<Multiplication*>(right)->operands.back())->right);
}

void Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " "){
    /*// Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }*/
	int ebenencount = 0;
	string::const_iterator lastPos = str.begin();
	for (string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (*it == '(' || *it == '[' || *it == '{')  ebenencount--;
		else if (*it == ')' || *it == ']' || *it == '}') ebenencount++;
		else if (ebenencount == 0 && delimiters.find_first_of(*it) != string::npos) {
			tokens.push_back(str.substr(lastPos - str.begin(), it - lastPos));
			lastPos = it + 1;
		}
	}
	if (lastPos != str.begin()) tokens.push_back(str.substr(lastPos - str.begin(), str.length() - (lastPos - str.begin())));
	else tokens.push_back(str);
}

ArithmeticExpression *ArithmeticExpression::create(string strin) {
	int ebenencount;
	bool geloescht;
	do {
		if (strin.empty()) throw "Ungueltiger (Teil-)Term";
		geloescht = false;
		if (strin.at(0) == '(' && strin.at(strin.length() - 1) == ')') {
			ebenencount = 0;
			for (string::iterator i = strin.begin() + 1; i != strin.end() - 1; ++i) {
				if (*i == '(') ebenencount++;
				else if (*i == ')' && --ebenencount == -1) break;
			}
			if (ebenencount != -1) {
				strin.erase(0, 1);
				strin.erase(strin.length() - 1, 1);
				geloescht = true;
			}
		}
	} while (geloescht);

	if (DecimalRec::runWith(strin)) {
		return new NumericalValue(strin);
	} else if (strin == "e") {
		return new NumericalValue(2.71828182845905);
	} else if (strin == "pi") {
		return new NumericalValue(3.14159265358979);
	} else if (IdentifierRec::runWith(strin)) {
		return new VariableExpression(strin);
	} else {
		ebenencount = 0;
		int pos_multi = 0, pos_divi = 0, pos_potenz = 0;

		for (string::reverse_iterator i = strin.rbegin(); i != strin.rend(); ++i) {
			if (*i == '(' || *i == '[' || *i == '{')  ebenencount--;
			else if (*i == ')' || *i == ']' || *i == '}') ebenencount++;
			else if (ebenencount == 0) {
				switch (*i) {
				case '+':
					if ((strin.rend() - i) == 1 || (i[1] != '-' && i[1] != '+' && i[1] != '*' && i[1] != '/' && i[1] != '^')) {
						return new Addition(ArithmeticExpression::create(strin.substr(0, strin.rend() - i - 1)), ArithmeticExpression::create(strin.substr(strin.rend() - i, i - strin.rbegin())));
					}
					break;
				case '-':
					if ((strin.rend() - i) != 1 && i[1] != '-' && i[1] != '+' && i[1] != '*' && i[1] != '/' && i[1] != '^') {
						return new Subtraction(ArithmeticExpression::create(strin.substr(0, strin.rend() - i - 1)), ArithmeticExpression::create(strin.substr(strin.rend() - i, i - strin.rbegin())));
					}
					break;
				case '*':
					if (! pos_multi && ! pos_divi) pos_multi = strin.rend() - i - 1;
					break;
				case '/':
					if (! pos_multi && ! pos_divi) pos_divi = strin.rend() - i - 1;
					break;
				case '^':
					if (! pos_potenz) pos_potenz = strin.rend() - i - 1;
					break;
				}
			}
		}

		if (pos_multi) {
			return new Multiplication(ArithmeticExpression::create(strin.substr(0, pos_multi)), ArithmeticExpression::create(strin.substr(pos_multi + 1, strin.length() - pos_multi - 1)));
		} else if (pos_divi) {
			return new Division(ArithmeticExpression::create(strin.substr(0, pos_divi)), ArithmeticExpression::create(strin.substr(pos_divi + 1, strin.length() - pos_divi - 1)));
		} else if (pos_potenz) {
			return new Exponentiation(ArithmeticExpression::create(strin.substr(0, pos_potenz)), ArithmeticExpression::create(strin.substr(pos_potenz + 1, strin.length() - pos_potenz - 1)));
		} else if (strin.at(strin.length() - 1) == '}') {
			int select_ebenencount = 0;
			string::reverse_iterator itselect;
			for (itselect = strin.rbegin(); itselect != strin.rend(); ++itselect) {
				if (*itselect == '(' || *itselect == '[')  select_ebenencount--;
				else if (*itselect == ')' || *itselect == ']' || *itselect == '}') select_ebenencount++;
				else if (*itselect == '{') {
					if (select_ebenencount == 1) break;
					else select_ebenencount--;
				}
			}
			vector<string> tokens;
			Tokenize(strin.substr(strin.rend() - itselect, strin.length() - (strin.rend() - itselect) - 1), tokens, ",");
			if (tokens.size() != 2) throw "Ungueltige Argumentenanzahl fuer Auswahl";
			else {
				return new Selection(ArithmeticExpression::create(strin.substr(0, strin.rend() - itselect - 1)), ArithmeticExpression::create(tokens.at(1)), ArithmeticExpression::create(tokens.at(0)));
			}
		} else if (strin.substr(0, 5) == "sqrt(") {
			return new SquareRoot(ArithmeticExpression::create(strin.substr(5, strin.length() - 6)));
		} else if (strin.substr(0, 4) == "log(") {
			return new Logarithm(ArithmeticExpression::create(strin.substr(4, strin.length() - 5)));
		} else if (strin.substr(0, 3) == "ln(") {
			return new NaturalLogarithm(ArithmeticExpression::create(strin.substr(3, strin.length() - 4)));
		} else if (strin.substr(0, 4) == "sin(") {
			return new Sinus(ArithmeticExpression::create(strin.substr(4, strin.length() - 5)));
		} else if (strin.substr(0, 4) == "cos(") {
			return new Cosinus(ArithmeticExpression::create(strin.substr(4, strin.length() - 5)));
		} else if (strin.substr(0, 4) == "tan(") {
			return new Tangent(ArithmeticExpression::create(strin.substr(4, strin.length() - 5)));
		} else if (strin.substr(0, 10) == "integrate(") {
			vector<string> tokens;
            Tokenize(strin.substr(10, strin.length() - 11), tokens, ","); //ALEX FUNCKOMMAS!!!!!!! NACHHER CHEKC?
			if (tokens.size() != 5 || ! IdentifierRec::runWith(tokens.at(1)) || ! DecimalRec::runWith(tokens.at(2)) || ! DecimalRec::runWith(tokens.at(3)) || ! DecimalRec::runWith(tokens.at(4))) throw "Ungueltiger (Teil-)Term"; //UND HIER AUCH
			return new Integral(ArithmeticExpression::create(tokens.at(0)), new VariableExpression(tokens.at(1)), strtod(tokens.at(2).c_str(), NULL), strtod(tokens.at(3).c_str(), NULL), strtod(tokens.at(4).c_str(), NULL));
		} else if (strin.at(0) == '$' && DecimalRec::runWith(strin.substr(1, strin.length() - 1))) { //!!!!!!ALEX GUCK HIER
			return new CommandExpression(atoi(strin.substr(1, strin.length() - 1).c_str()));		//!!!!!!!!! decimal, atoi
		} else if (strin.at(0) == '[' && strin.at(strin.length() - 1) == ']') {
			vector<string> tokens;
			Tokenize(strin.substr(1, strin.length() -2), tokens, "|");
			ArithmeticExpression ***c = new ArithmeticExpression **[tokens.size()];
			vector<string> line_tokens;
			int size_x = 0;
			for (int i = 0; i < tokens.size(); i++) {
				Tokenize(tokens.at(i), line_tokens, ",");

				if (i == 0) size_x = line_tokens.size();
				else if (line_tokens.size() != size_x) throw "Ungueltiger (Teil-)Term";

				c[i] = new ArithmeticExpression *[size_x];
				for (int j = 0; j < size_x; j++) {
					c[i][j] = ArithmeticExpression::create(line_tokens.at(j));
				}
				line_tokens.clear();
			}
			return new Matrix(c, size_x, tokens.size());
		} else if (strin.substr(0, 6) == "solve(") {
			return new Solver(ArithmeticExpression::create(strin.substr(6, strin.length() - 7)));
        } else if (strin.substr(0, 10) == "transform(") {
            vector<string> tokens;
            Tokenize(strin.substr(10, strin.length() - 11), tokens, ",");
            return new Transformation(ArithmeticExpression::create(tokens.at(0)), ArithmeticExpression::create(tokens.at(1)), ArithmeticExpression::create(tokens.at(2)));
		} else {
			size_t pos_leftparenth = strin.find_first_of('(');
			string funcname = strin.substr(0, pos_leftparenth);
			if (pos_leftparenth == string::npos || strin.at(strin.length() - 1) != ')' || ! IdentifierRec::runWith(funcname)) throw "Ungueltiger (Teil-)Term";
			/*string funcargs = strin.substr(pos_leftparenth + 1, strin.length() - pos_leftparenth - 2);
			int pos_before = -1, pos_now = 0;
			vector<ArithmeticExpression*> argvec;
			while ((pos_now = funcargs.find(',', pos_before + 1)) != string::npos) {
				argvec.push_back(ArithmeticExpression::create(funcargs.substr(pos_before + 1, pos_now - pos_before - 1)));
				pos_before = pos_now;
			}
			argvec.push_back(ArithmeticExpression::create(funcargs.substr(pos_before + 1, funcargs.length() - pos_before - 1)));*/
			vector<string> tokens;
			Tokenize(strin.substr(pos_leftparenth + 1, strin.length() - pos_leftparenth - 2), tokens, ",");
			vector<ArithmeticExpression*> argvec;
			for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) argvec.push_back(ArithmeticExpression::create(*it));
			return new FunctionExpression(funcname, argvec);
		}
	}
}

ArithmeticExpression *ArithmeticExpression::sqrt_func() { return new SquareRoot(this); };
ArithmeticExpression *ArithmeticExpression::log_func() { return new Logarithm(this); };
ArithmeticExpression *ArithmeticExpression::ln_func() { return new NaturalLogarithm(this); };
ArithmeticExpression *ArithmeticExpression::sin_func() { return new Sinus(this); };
ArithmeticExpression *ArithmeticExpression::cos_func() { return new Cosinus(this); };
ArithmeticExpression *ArithmeticExpression::tan_func() { return new Tangent(this); }; 

ArithmeticExpression *Addition::expand(const ExpansionInformation& ei) const {
	list<ArithmeticExpression*> tmpvec;
	Addition *nv;
	ArithmeticExpression *ax;
	for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != operands.end(); ++it) {
		ax = (*it)->expand(ei);
		if (nv = dynamic_cast<Addition*>(ax)) tmpvec.insert(tmpvec.begin(), nv->operands.begin(), nv->operands.end());
		else tmpvec.push_back(ax);
	}

	NumericalValue *np;
	Matrix *mp;
	double dbuf = 0;
	vector<Matrix*> matrvec;
	for (list<ArithmeticExpression*>::iterator it = tmpvec.begin(); it != tmpvec.end();) {
		if (np = dynamic_cast<NumericalValue*>(*it)) {
			it = tmpvec.erase(it);
			dbuf += np->value;
		} else if (mp = dynamic_cast<Matrix*>(*it)) {
			vector<Matrix*>::iterator it_mat;
			for (it_mat = matrvec.begin(); it_mat != matrvec.end(); ++it_mat) {
				if ((*it_mat)->size_x == mp->size_x && (*it_mat)->size_y == mp->size_y) {
					for (int i = 0; i < mp->size_y; i++)
						for (int j = 0; j < mp->size_x; j++)
							(*it_mat)->components[i][j] = new Addition((*it_mat)->components[i][j], mp->components[i][j]); 
					break;
				}
			}
			if (it_mat == matrvec.end()) matrvec.push_back(mp);
			it = tmpvec.erase(it);
		} else ++it;
	}

	for (vector<Matrix*>::iterator it_mat = matrvec.begin(); it_mat != matrvec.end(); ++it_mat)
		tmpvec.push_back((*it_mat)->expand(ei)); //hier expand?

	if (tmpvec.size() == 0) return new NumericalValue(dbuf);
	else if (dbuf == 0) {
		if (tmpvec.size() == 1) return tmpvec.front();
	} else {
		tmpvec.push_front(new NumericalValue(dbuf));
	}
	list<Multiplication*> monlist;
	for (list<ArithmeticExpression*>::iterator it = tmpvec.begin(); it != tmpvec.end(); ++it)
		monlist.push_back((*it)->formMonom());
	tmpvec.clear();
	for (list<Multiplication*>::iterator it = monlist.begin(); it != monlist.end(); ++it) {
		for (list<Multiplication*>::iterator it_mult = ++(list<Multiplication*>::iterator(it)); it_mult != monlist.end() && monlist.size() > 1;) {
			if (compareOperands(list<ArithmeticExpression*>(++((*it)->operands.begin()), (*it)->operands.end()), list<ArithmeticExpression*>(++((*it_mult)->operands.begin()), (*it_mult)->operands.end()))) {
				(*it)->operands.front() = Addition((*it)->operands.front(), (*it_mult)->operands.front()).expand(ei);
				it_mult = monlist.erase(it_mult);
			} else ++it_mult;
		}
	}
	for (list<Multiplication*>::iterator it = monlist.begin(); it != monlist.end(); ++it)
		tmpvec.push_back((*it)->expand(ExpansionInformation(ei.variables, ei.functions, ei.commands)));
	return new Addition(tmpvec);
}

ArithmeticExpression *Subtraction::expand(const ExpansionInformation& ei) const {
    return Addition(left, new Multiplication(new NumericalValue(-1), right)).expand(ei);
}

ArithmeticExpression *Subtraction::transformExpression(VariableExpression *var_to, ArithmeticExpression *before) const
{
    if (left->containsVar(var_to)) return left->transformExpression(var_to, new Addition(before, right));
    else return right->transformExpression(var_to, new Multiplication(new NumericalValue(-1), new Subtraction(before, left)));
}

void Multiplication::addexp(ArithmeticExpression *base, ArithmeticExpression *potency, vector<Exponentiation*>& exvec) const {
	vector<Exponentiation*>::iterator it;
	for (it = exvec.begin(); it != exvec.end(); ++it) {
		if ((*it)->left->isEqual(base)) {
			(*it)->right = new Addition((*it)->right, potency);
			break;
		}
	}
	if (it == exvec.end()) exvec.push_back(new Exponentiation(base, potency));
}

ArithmeticExpression *Multiplication::expand(const ExpansionInformation& ei) const {
	list<ArithmeticExpression*> tmpvec;
	Multiplication *nv;
	ArithmeticExpression *ax;
	for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != operands.end(); ++it) {
		ax = (*it)->expand(ei);
		if (nv = dynamic_cast<Multiplication*>(ax)) {
			tmpvec.insert(tmpvec.begin(), nv->operands.begin(), nv->operands.end());
		}
		else tmpvec.push_back(ax);
	}
	Division *dp;
	Addition *ap;
	list<ArithmeticExpression*> mtmp;
	for (list<ArithmeticExpression*>::iterator it = tmpvec.begin(); it != tmpvec.end(); ++it) {
		if (dp = dynamic_cast<Division*>(*it)) {
			tmpvec.erase(it);
			if (tmpvec.size() > 1) return Division(new Multiplication(new Multiplication(tmpvec), dp->left), dp->right).expand(ei);
			else return Division(new Multiplication(tmpvec.front(), dp->left), dp->right).expand(ei);
		} else if (ap = dynamic_cast<Addition*>(*it)) {
			tmpvec.erase(it);
			for (list<ArithmeticExpression*>::iterator ait = ap->operands.begin(); ait != ap->operands.end(); ++ait) {
				tmpvec.push_front(*ait);
				mtmp.push_back(new Multiplication(tmpvec));
				tmpvec.erase(tmpvec.begin());
			}
			return Addition(mtmp).expand(ei);
		}
	}
	Exponentiation *exp;
	vector<Exponentiation*> exvec;
	for (list<ArithmeticExpression*>::iterator it = tmpvec.begin(); it != tmpvec.end(); ++it) {
		exp = dynamic_cast<Exponentiation*>(*it);
		if (exp) addexp(exp->left, exp->right, exvec);
		else addexp(*it, new NumericalValue(1), exvec);
	} 
	tmpvec.clear();
	for (vector<Exponentiation*>::iterator it = exvec.begin(); it != exvec.end(); ++it)
		tmpvec.push_back((*it)->expand(ei)); //nicht expand?? f�r hoch1, oder exponent-expand �ndern (aber endergebnis)
	NumericalValue *np;
	double dbuf = 1;
	for (list<ArithmeticExpression*>::iterator it = tmpvec.begin(); it != tmpvec.end();) { //hier matrix
		if (np = dynamic_cast<NumericalValue*>(*it)) {
			it = tmpvec.erase(it);
			dbuf *= np->value;
		} else ++it;
	}
	Matrix *mp;
	for (list<ArithmeticExpression*>::iterator it = tmpvec.begin(); it != tmpvec.end();) { //hier matrix
		if (mp = dynamic_cast<Matrix*>(*it)) {
			for (int i = 0; i < mp->size_y; i++)
				for (int j = 0; j < mp->size_x; j++)
					mp->components[i][j] = Multiplication(mp->components[i][j], new NumericalValue(dbuf)).expand(ei);
			dbuf = 1;
			break;
		} else ++it;
	}
	/*NumericalValue *np;
	Matrix *mp;
	map<int, Matrix*> matrvec;
	map<int, Matrix*>::iterator it_vec;
	for (list<ArithmeticExpression*>::iterator it = tmpvec.begin(); it != tmpvec.end();) { //hier matrix
		if (np = dynamic_cast<NumericalValue*>(*it)) {
			it_vec = matrvec.find(1);
			ArithmeticExpression ***c = new ArithmeticExpression**[1];
			if (it_vec != matrvec.end()) {
				c[1] = new ArithmeticExpression*[it_vec->second->size_x];
				for (int i = 0; i < 
			} else {
				c[1] = new ArithmeticExpression*[1];
				c[1][1] = np;
				matrvec.insert(pair<int, Matrix*>(1, new Matrix(c, 1, 1)));
			}
			it = tmpvec.erase(it);
		} else if (mp = dynamic_cast<Matrix*>(*it)) {
			
		} else ++it;
	}*/
	if (dbuf == 0) return new NumericalValue(0);
	else if (tmpvec.size() == 0) return new NumericalValue(dbuf);
	else if (dbuf == 1) {
		if (tmpvec.size() == 1) return tmpvec.front();
		else return new Multiplication(tmpvec);
	} else {
		tmpvec.push_front(new NumericalValue(dbuf));
		return new Multiplication(tmpvec);
	}
}

ArithmeticExpression *Division::expand(const ExpansionInformation& ei) const {
	ArithmeticExpression *al = left->expand(ei);
	ArithmeticExpression *ar = right->expand(ei);
	Division *dp;
	Addition *ap;
	Multiplication *mp;
	NumericalValue *nl = dynamic_cast<NumericalValue*>(al), *nr = dynamic_cast<NumericalValue*>(ar);
	if (nl && nr) return new NumericalValue(nl->value / nr->value);
	else if (dp = dynamic_cast<Division*>(al)) return Division(dp->left, new Multiplication(dp->right, ar)).expand(ei);
	else if (dp = dynamic_cast<Division*>(ar)) return Division(new Multiplication(al, dp->right), dp->left).expand(ei);
	else if (ap = dynamic_cast<Addition*>(al)) {
		list<ArithmeticExpression*> res;
		for (list<ArithmeticExpression*>::iterator it = ap->operands.begin(); it != ap->operands.end(); ++it)
			res.push_back(Division(*it, ar).expand(ei));
		return new Addition(res);
	} else if (mp = dynamic_cast<Multiplication*>(al)) {
		list<ArithmeticExpression*> res(mp->operands);
		res.front() = Division(res.front(), ar).expand(ei);
		return new Multiplication(res);
	} else {
		return new Division(al, ar);
		/*list<ArithmeticExpression*> explist;
		list<ArithmeticExpression*> reslist;
		Addition *adl = dynamic_cast<Addition*>(al->formPolynom()), *adr = dynamic_cast<Addition*>(ar->formPolynom()), *nah = new Addition, *nal = new Addition, *adp;
		reslist.push_front(new Division(nah, nal));
		Multiplication *mp, *mexp;
		Division *dp;
		Exponentiation *fac_ep, *ep;
		NumericalValue *nvp;
		for (list<ArithmeticExpression*>::iterator it_divisor = adr->operands.begin(); it_divisor != adr->operands.end(); ++it_divisor) {
			mp = dynamic_cast<Multiplication*>(*it_divisor);
			for (list<ArithmeticExpression*>::iterator it_divisor_fac = mp->operands.begin(); it_divisor_fac != mp->operands.end(); ++it_divisor_fac) {
				if (nvp = dynamic_cast<NumericalValue*>(*it_divisor_fac)) nal->operands.push_back(nvp);
				else {
					fac_ep = dynamic_cast<Exponentiation*>(*it_divisor_fac);
					list<ArithmeticExpression*>::iterator it_explist;
					for (it_explist = explist.begin(); it_explist != explist.end(); ++it_explist) {
						adp = dynamic_cast<Addition*>(dynamic_cast<Division*>(*it_explist)->right);
						if (dynamic_cast<Exponentiation*>(dynamic_cast<Multiplication*>(adp->operands.front())->operands.back())->left->isEqual(fac_ep->left)) {
							list<ArithmeticExpression*>::iterator it_expad;
							for (it_expad = adp->operands.begin(); it_expad != adp->operands.end(); ++it_expad) {
								mexp = dynamic_cast<Multiplication*>(*it_expad);
								if (dynamic_cast<Exponentiation*>(mexp->operands.back())->right->isEqual(fac_ep->right)) ++(mexp->operands.front());
							}
							if (it_expad == adp->operands.end()) adp->operands.push_back(new Multiplication(new NumericalValue(1), fac_ep));
							break;
						}
					}
					if (it_explist == explist.end()) {
						list<ArithmeticExpression*> tmplist;
						tmplist.push_back(new Multiplication(new NumericalValue(1), fac_ep));
						explist.push_back(new Division(new Addition, new Addition(tmplist)));
					}
				}
			}
		}

		for (list<ArithmeticExpression*>::iterator it_dividend = adl->operands.begin(); it_dividend != adl->operands.end(); ++it_dividend) {
			mp = dynamic_cast<Multiplication*>(*it_dividend);
			for (list<ArithmeticExpression*>::iterator it_dividend_fac = mp->operands.begin(); it_dividend_fac != mp->operands.end(); ++it_dividend_fac) {
				if (nvp = dynamic_cast<NumericalValue*>(*it_dividend_fac)) nah->operands.push_back(nvp);
				else {
					fac_ep = dynamic_cast<Exponentiation*>(*it_dividend_fac);
					list<ArithmeticExpression*>::iterator it_explist;
					for (it_explist = explist.begin(); it_explist != explist.end(); ++it_explist) {
						adp = dynamic_cast<Addition*>(dynamic_cast<Division*>(*it_explist)->left);
						if (dynamic_cast<Exponentiation*>(dynamic_cast<Multiplication*>(dynamic_cast<Addition*>(dynamic_cast<Division*>(*it_explist)->right)->operands.front())->operands.back())->left->isEqual(fac_ep->left)) {
							list<ArithmeticExpression*>::iterator it_expad;
							for (it_expad = adp->operands.begin(); it_expad != adp->operands.end(); ++it_expad) {
								mexp = dynamic_cast<Multiplication*>(*it_expad);
								if (dynamic_cast<Exponentiation*>(mexp->operands.back())->right->isEqual(fac_ep->right)) ++(mexp->operands.front());
							}
							if (it_expad == adp->operands.end()) adp->operands.push_back(new Multiplication(new NumericalValue(1), fac_ep));
							break;
						}
					}
					if (it_explist == explist.end()) {
						list<ArithmeticExpression*> tmplist;
						tmplist.push_back(new Multiplication(new NumericalValue(1), fac_ep));
						explist.push_back(new Division(new Addition(tmplist), new Addition));
					}
				}
			}
		}
		Multiplication *mlp, *mrp;
		Exponentiation *elp, *erp;
		for (list<ArithmeticExpression*>::iterator it = explist.begin(); it != explist.end(); ++it) {
			dp = dynamic_cast<Division*>(*it);
			adr = dynamic_cast<Addition*>(dp->right);
			adr->operands.sort(polysort);
			adl = dynamic_cast<Addition*>(dp->left);
			adl->operands.sort(polysort);
			mrp = dynamic_cast<Multiplication*>(adr->operands.front());
			erp = dynamic_cast<Exponentiation*>(mrp->operands.back());
			for (;;) {
				if (adl->operands.size() == 0) {
					reslist.push_back(dp);
					break;
				}
				mlp = dynamic_cast<Multiplication*>(adl->operands.front());
				elp = dynamic_cast<Exponentiation*>(mlp->operands.back());
				if (elp->right 
				reslist.push_back(new Multiplication(new Division(mlp->operands.front(), mrp->operands.front()), new Exponentiation(elp->left, new Subtraction(elp->right, erp->right))));
				adl = dynamic_cast<Addition*>(Subtraction(adl, new Multiplication(reslist.back(), adr)).expand(ei)->formPolynom());
			}
		}
		return Multiplication(reslist).expand(ei);*/
    }
}

ArithmeticExpression *Division::transformExpression(VariableExpression *var_to, ArithmeticExpression *before) const
{
    if (left->containsVar(var_to)) return left->transformExpression(var_to, new Multiplication(before, right));
    else return right->transformExpression(var_to, new Multiplication(new Division(new NumericalValue(1), before), left));
}

ArithmeticExpression *Exponentiation::expand(const ExpansionInformation& ei) const {
	ArithmeticExpression *al = left->expand(ei);
	ArithmeticExpression *ar = right->expand(ei);
	NumericalValue *nl = dynamic_cast<NumericalValue*>(al);
	NumericalValue *nr = dynamic_cast<NumericalValue*>(ar);
	if (nl && nr) return new NumericalValue(pow(nl->value,  nr->value));
	else if (nl && nl->value == 1) return al;
	else if (nr) {
		if (nr->value == 0) return new NumericalValue(1);
		else if (nr->value == 1) return al;
/*		else {
			list<ArithmeticExpression*> multis;
			for (int i = 0; i < nr->value; i++) multis.push_back(al);
			return Multiplication(multis).expand(ei);
		}*/
	}
	return new Exponentiation(al, ar);
	
}

ArithmeticExpression *SquareRoot::expand(const ExpansionInformation& ei) const {
	return operand->expand(ei)->sqrt_func();
}

ArithmeticExpression *Logarithm::expand(const ExpansionInformation& ei) const {
	return operand->expand(ei)->log_func();
}

ArithmeticExpression *NaturalLogarithm::expand(const ExpansionInformation& ei) const {
	return operand->expand(ei)->ln_func();
}

ArithmeticExpression *Sinus::expand(const ExpansionInformation& ei) const {
	return operand->expand(ei)->sin_func();
}

ArithmeticExpression *Cosinus::expand(const ExpansionInformation& ei) const {
	return operand->expand(ei)->cos_func();
}

ArithmeticExpression *Tangent::expand(const ExpansionInformation& ei) const {
	return operand->expand(ei)->tan_func();
}

ArithmeticExpression *NumericalValue::expand(const ExpansionInformation& ei) const {
	return new NumericalValue(*this);
}

ArithmeticExpression *Integral::expand(const ExpansionInformation& ei) const {
	ArithmeticExpression *sum = new NumericalValue(0), *func1, *func2;
	NumericalValue *h = new NumericalValue((upper_bound - lower_bound) / intervals);
	vector<Variable> cvars(ei.variables);
	for (int i = 1; i <= intervals; i++) {
		cvars.push_back(Variable(var->getString(), new Addition(new NumericalValue(lower_bound), new Multiplication(new NumericalValue(i - 1), h))));
		func1 = aexp->expand(ExpansionInformation(cvars, ei.functions, ei.commands));
		cvars.pop_back();
		cvars.push_back(Variable(var->getString(), new Addition(new NumericalValue(lower_bound), new Multiplication(new NumericalValue(i), h))));
		func2 = aexp->expand(ExpansionInformation(cvars, ei.functions, ei.commands));
		cvars.pop_back();
		sum = new Addition(sum, new Multiplication(new Division(h, new NumericalValue(6)), new Addition(func1, new Addition(new Division(new Multiplication(new NumericalValue(4), new Addition(func2, func1)), new NumericalValue(2)), func2))));
	}
	return sum->expand(ExpansionInformation(ei.variables, ei.functions, ei.commands));
}


string Integral::getString() const {
	return "integrate(" + aexp->getString() + "," + var->getString() + "," + dtostr(lower_bound) + "," + dtostr(upper_bound) + ")";
}

ArithmeticExpression *VariableExpression::expand(const ExpansionInformation& ei) const {
	for (vector<Variable>::const_iterator it = ei.funcvars.begin(); it != ei.funcvars.end(); ++it) {
			if (it->identifier == identifier) {
				if (it->expanded) throw "Fehler: rekursive Variable"; //?? m�glich?
				return it->aexp->expand(ExpansionInformation(ei.variables, ei.functions, ei.commands));
			}
	}
	
	for (vector<Variable>::const_iterator it = ei.variables.begin(); it != ei.variables.end(); ++it) {
			if (it->identifier == identifier) {
				if (it->expanded) throw "Fehler: rekursive Variable";
				vector<Variable> exvars(ei.variables);
				exvars.at(it - ei.variables.begin()).expanded = true;
				ExpansionInformation exptmp(exvars, ei.functions, ei.commands);
				return it->aexp->expand(exptmp);
			}
	}

    return new VariableExpression(identifier);
}

ArithmeticExpression *VariableExpression::transformExpression(VariableExpression *var_to, ArithmeticExpression *before) const
{
    if (var_to->identifier == identifier) return before;
    else throw "Transform-Fehler";
}

ArithmeticExpression *CommandExpression::expand(const ExpansionInformation& ei) const {
	if (ei.commands.size() < id) throw "Fehler: Kommando nicht gelistet";
	if (ei.commands.at(id - 1).expanded) throw "Fehler: rekursive Eingabe";
	vector<Command> cc(ei.commands);
	cc.at(id - 1).expanded = true;
	return ei.commands.at(id - 1).aexp->expand(ExpansionInformation(ei.variables, ei.functions, cc));
}

ArithmeticExpression* ArithmeticExpression::formPolynom() const {
	list<ArithmeticExpression*> tmpvec;
	tmpvec.push_back(new Multiplication(new NumericalValue(1), new Exponentiation(this->copy(), new NumericalValue(1))));
	return new Addition(tmpvec);
}

ArithmeticExpression* Addition::formPolynom() const {
	ArithmeticExpression *ax;
	Multiplication *mp;
	list<ArithmeticExpression*> newlist;
	for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != operands.end(); ++it) {
		ax = (*it)->formPolynom();
		if (mp = dynamic_cast<Multiplication*>(ax)) newlist.push_back(ax);
		else newlist.push_back(Multiplication(new NumericalValue(1), ax).formPolynom());
	}
    return new Addition(newlist);
}

ArithmeticExpression *Addition::transformExpression(VariableExpression *var_to, ArithmeticExpression *before) const {
    ArithmeticExpression *others = before;
    ArithmeticExpression *varexp = NULL;
    for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != operands.end(); ++it) {
        if ((*it)->containsVar(var_to)) {
            varexp = *it;
        } else others = new Subtraction(others, *it);
    }
    return varexp->transformExpression(var_to, others);
}

ArithmeticExpression* Multiplication::formPolynom() const {
	Exponentiation *ep;
	NumericalValue *np;
	list<ArithmeticExpression*> newlist;
	for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != operands.end(); ++it) {
		ep = dynamic_cast<Exponentiation*>(*it);
		np = dynamic_cast<NumericalValue*>(*it);
		if (np || ep) newlist.push_back(*it);
		else newlist.push_back(new Exponentiation(*it, new NumericalValue(1)));
	}
	list<ArithmeticExpression*> adlist;
	adlist.push_back(new Multiplication(newlist));
	return new Addition(adlist);
}

ArithmeticExpression* Exponentiation::formPolynom() const {
	list<ArithmeticExpression*> tmpvec;
	tmpvec.push_back(new Multiplication(new NumericalValue(1), new Exponentiation(*this)));
	return new Addition(tmpvec);
}

ArithmeticExpression* NumericalValue::formPolynom() const {
	list<ArithmeticExpression*> tmpvec;
	tmpvec.push_back(formMonom());
	return new Addition(tmpvec);
}

Multiplication *ArithmeticExpression::formMonom() const {
	return new Multiplication(new NumericalValue(1), copy());
}

Multiplication *Multiplication::formMonom() const {
	if (! dynamic_cast<NumericalValue*>(operands.front())) {
		list<ArithmeticExpression*> clist(operands);
		clist.push_front(new NumericalValue(1));
		return new Multiplication(clist);
    } else return new Multiplication(operands);
}

ArithmeticExpression *Multiplication::transformExpression(VariableExpression *var_to, ArithmeticExpression *before) const
{
    list<ArithmeticExpression*> others;
    ArithmeticExpression *varexp;
    for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != operands.end(); ++it) {
        if ((*it)->containsVar(var_to)) {
            varexp = *it;
        } else others.push_back((*it)->copy());
    }
    return varexp->transformExpression(var_to, new Division(before, new Multiplication(others)));
}

string CommandExpression::getString() const {
	return string("$") + dtostr(id);
}

string Addition::getString() const {
	string str;
	for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != --(operands.end()); ++it)
			str += (*it)->getString() + '+';
	str += operands.back()->getString();
	return str;
}

string Subtraction::getString() const {
	return left->getString() + "-" + right->getString();
}

string Multiplication::getString() const {
	string result;
	Addition *ap;
	for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != operands.end(); ++it) {
		if (ap = dynamic_cast<Addition*>(*it)) result += "(" + (*it)->getString() + ")";
		else result += (*it)->getString();
		if (it != --(operands.end())) result += "*";
	}
	return result;
}

string Division::getString() const {
	string result;
	LevelingOperation *lp = dynamic_cast<LevelingOperation*>(left), *rp = dynamic_cast<LevelingOperation*>(right);
	if (lp) result += "(" + left->getString() + ")";
	else result += left->getString();
	result += "/";
	if (rp) result += "(" + right->getString() + ")";
	else result += right->getString();
	return result;
}

string Exponentiation::getString() const {
	string result;
	LevelingOperation *lp = dynamic_cast<LevelingOperation*>(left), *rp = dynamic_cast<LevelingOperation*>(right);
	if (lp) result += "(" + left->getString() + ")";
	else result += left->getString();
	result += "^";
	if (rp) result += "(" + right->getString() + ")";
	else result += right->getString();
	return result;
}

string SquareRoot::getString() const {
	return "sqrt(" + operand->getString() + ")";
}

string Logarithm::getString() const {
	return "log(" + operand->getString() + ")";
}

string NaturalLogarithm::getString() const {
	return "ln(" + operand->getString() + ")";
}

string Sinus::getString() const {
	return "sin(" + operand->getString() + ")";
}

string Cosinus::getString() const {
	return "cos(" + operand->getString() + ")";
}

string Tangent::getString() const {
	return "tan(" + operand->getString() + ")";
}

string NumericalValue::getString() const {
	return dtostr(value);
}

string VariableExpression::getString() const {
	return identifier;
}

string FunctionExpression::getString() const {
	string tmp = identifier + "(";
	for (vector<ArithmeticExpression*>::const_iterator it = argvec.begin(); it != argvec.end(); ++it) {
		if (it != argvec.begin()) tmp += ",";
		tmp += (*it)->getString();
	}
	tmp += ")";
	return tmp;	
}

bool LevelingOperation::isEqual(ArithmeticExpression *other) const {
	LevelingOperation *ap = dynamic_cast<LevelingOperation*>(other);
	if (ap) return compareOperands(operands, ap->operands);
    else return false;
}

bool LevelingOperation::containsVar(VariableExpression *var) const
{
    for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != operands.end(); ++it)
        if ((*it)->containsVar(var)) return true;
    return false;
}

bool BinaryOperation::isEqual(ArithmeticExpression *other) const {
	BinaryOperation *bp = dynamic_cast<BinaryOperation*>(other);
	return (bp && bp->left->isEqual(left) && bp->right->isEqual(right));
}

bool UnaryOperation::isEqual(ArithmeticExpression *other) const {
	UnaryOperation *up = dynamic_cast<UnaryOperation*>(other);
	return (up && up->operand->isEqual(operand));
}

bool NumericalValue::isEqual(ArithmeticExpression *other) const {
	NumericalValue *np = dynamic_cast<NumericalValue*>(other);
	return (np && np->value == value);
}

bool FunctionExpression::isEqual(ArithmeticExpression *other) const {
	FunctionExpression *fp = dynamic_cast<FunctionExpression*>(other);
	if (fp && fp->identifier == identifier && fp->argvec.size() == argvec.size()) {
		for (vector<ArithmeticExpression*>::const_iterator it = argvec.begin(); it != argvec.end(); ++it)
			if (! (*it)->isEqual(fp->argvec.at(it - argvec.begin()))) return false;
		return true;
	} else return false;
}

bool VariableExpression::isEqual(ArithmeticExpression *other) const {
	VariableExpression *vp = dynamic_cast<VariableExpression*>(other);
	return (vp && vp->identifier == identifier);
}

bool Integral::isEqual(ArithmeticExpression *other) const {
	Integral *ip = dynamic_cast<Integral*>(other);
	return (ip && ip->lower_bound == lower_bound && ip->upper_bound == upper_bound && ip->intervals == intervals && ip->var->isEqual(var) && ip->aexp->isEqual(aexp));
}

bool Vector::isEqual(ArithmeticExpression *other) const {
	Vector *vp = dynamic_cast<Vector*>(other);
	return (vp && coordinates[0]->isEqual(vp->coordinates[0]) && coordinates[1]->isEqual(vp->coordinates[1]) && coordinates[2]->isEqual(vp->coordinates[2]));
}

bool CommandExpression::isEqual(ArithmeticExpression *other) const {
	CommandExpression *cp = dynamic_cast<CommandExpression*>(other);
	return (cp && cp->id == id);
}


ostream& operator << (ostream& os, const ArithmeticExpression& kt) {
	os << kt.getString();
	return os;
}

ArithmeticExpression *FunctionExpression::expand(const ExpansionInformation& ei) const {
	vector<Function>::const_iterator it_func = ei.functions.begin();
	for (; it_func != ei.functions.end(); ++it_func)
		if (identifier == it_func->identifier) { break; }
	if (it_func == ei.functions.end()) {
		vector<ArithmeticExpression*> copyargs;
		for (vector<ArithmeticExpression*>::const_iterator it = argvec.begin(); it != argvec.end(); ++it)
			copyargs.push_back((*it)->expand(ei));
		return new FunctionExpression(identifier, copyargs);
	}
	else if (it_func->func_args.size() != argvec.size()) throw "Fehler: inkompatible Funktionsdefinitionen";
	else if (it_func->expanded) throw "Fehler: rekursive Funktion";
	vector<Function> cfunc(ei.functions);
	cfunc.at(it_func - ei.functions.begin()).expanded = true;
	
	//ArithmeticExpression *expex = it_func->aexp->expand(ExpansionInformation(ei.variables, cfunc, ei.commands));
	vector<Variable> copyvars;
	for (vector<string>::const_iterator it = it_func->func_args.begin(); it != it_func->func_args.end(); ++it) {
		ArithmeticExpression *tmpex = argvec.at(it - it_func->func_args.begin())->expand(ei); //andere fwerterekur????
		copyvars.push_back(Variable(*it, tmpex)); //FunctionVar mit id
	}
	return it_func->aexp->expand(ExpansionInformation(ei.variables, cfunc, ei.commands, copyvars));
}

string Variable::getString() const {
	return identifier + "=" + aexp->getString();
}

ArithmeticExpression *Matrix::expand(const ExpansionInformation &ei) const {
	ArithmeticExpression ***res = new ArithmeticExpression **[size_y];
	for (int i = 0; i < size_y; i++) {
		res[i] = new ArithmeticExpression*[size_x];
		for (int j = 0; j < size_x; j++) {
			res[i][j] = components[i][j]->expand(ei);
		}
	}
	return new Matrix(res, size_x, size_y);
}

string Function::getString() const {
	string out = identifier + "(";
	for (vector<string>::const_iterator it = func_args.begin(); it != func_args.end(); ++it) {
		if (it != func_args.begin()) out += ",";
		out += (*it);
	}
	return out	+ ")" // linebreak to prevent unhappy smiley =)
				+ "=" + aexp->getString();
}

Matrix::Matrix(ArithmeticExpression ***c, int x, int y) {
	components = new ArithmeticExpression **[y];
	for (int i = 0; i < y; i++) {
		components[i] = new ArithmeticExpression*[x];
		for (int j = 0; j < x; j++) {
			components[i][j] = c[i][j];
		}
	}
	size_x = x;
	size_y = y;
}

string Matrix::getString() const {
	string result = "[";
	for (int i = 0; i < size_y; i++) {
		for (int j = 0; j < size_x; j++) {
			result += components[i][j]->getString();
			if (j < size_x - 1) result += ",";
		}
		if (i < size_y - 1) result += "|";
	}
	return result + "]";
}

bool Matrix::isEqual(ArithmeticExpression *other) const {
	Matrix *mp = dynamic_cast<Matrix*>(other);
	if (! mp || size_x != mp->size_x || size_y != mp->size_y) return false;
	for (int i = 0; i < size_y; i++) {
		for (int j = 0; j < size_x; j++) {
			if (! components[i][j]->isEqual(mp->components[i][j])) return false;
		}
	}
	return true;
}

ArithmeticExpression *Solver::expand(const ExpansionInformation &ei) const {
	Matrix *mp = dynamic_cast<Matrix*>(matrix->expand(ei));
	if (! mp) throw "Ungueltiges Argument: solve() mag Matrizen";
	else if (mp->size_x < 2) throw "Zu wenig Zeilenwerte fuer LGS";
	//TODO: linear abh�ngige zeilen abfangen, zu viele zeilen f�r unbekannte usw.
	ArithmeticExpression *diff;
	for (int i = 0; i < mp->size_y; i++) {
		for (int j = i + 1; j < mp->size_y; j++) {
			diff = new Division(mp->components[j][i], mp->components[i][i]);
			for (int k = 0; k < mp->size_x; k++) {
				mp->components[j][k] = new Subtraction(mp->components[j][k], new Multiplication(diff, mp->components[i][k]));
			}
		}
	}

	ArithmeticExpression ***resmatrix = new ArithmeticExpression**[mp->size_x];
	ArithmeticExpression **ptrptr;
	for (int i = mp->size_y - 1; i >= 0; i--) {
		resmatrix[i] = &(mp->components[i][mp->size_x - 1]);
		for (int j = i + 1; j < mp->size_y; j++) {
			ptrptr = new ArithmeticExpression*;
			*ptrptr = new Subtraction(*(resmatrix[i]), new Multiplication(*(resmatrix[j]), mp->components[i][j]));
			resmatrix[i] = ptrptr;
		}
		ptrptr = new ArithmeticExpression*;
		*ptrptr = new Division(*(resmatrix[i]), mp->components[i][i]);
		resmatrix[i] = ptrptr;
	}
	return Matrix(resmatrix, 1, mp->size_y).expand(ei);
}

bool Solver::isEqual(ArithmeticExpression *other) const {
	Solver *sp = dynamic_cast<Solver*>(other);
	return sp && sp->matrix->isEqual(matrix);
}

ArithmeticExpression *Selection::expand(const ExpansionInformation &ei) const {
	Matrix *mp = dynamic_cast<Matrix*>(expression->expand(ei));
	NumericalValue *np_x = dynamic_cast<NumericalValue*>(select_x), *np_y = dynamic_cast<NumericalValue*>(select_y);
	if (! mp) throw "Es koennen nur Matrixkomponenten ausgewaehlt werden";
	else if (! (np_x && np_y)) throw "Argumente fuer Auswahl muessen ganze Zahlen sein"; //ganze Zahl vs DecimalRec
	else if (mp->size_x < np_x->value || mp->size_y < np_y->value) throw "Komponente kann nicht aus Matrix gewaehlt werden";
	else return mp->components[(int)np_y->value - 1][(int)np_x->value - 1]->expand(ei);

}

bool Selection::isEqual(ArithmeticExpression *other) const {
	Selection *sp = dynamic_cast<Selection*>(other);
	return (sp && sp->expression->isEqual(expression) && sp->select_x->isEqual(select_x) && sp->select_y->isEqual(select_y));
}

string Command::getString() const {
	return aexp->getString();
}

ostream& operator<<(ostream& os, const Variable& var) {
	os << var.getString();
	return os;
}

ostream& operator<<(ostream& os, const Function& func) {
	os << func.getString();
	return os;
}

ostream& operator<<(ostream& os, const Command& com) {
	os << com.getString();
	return os;
}

string CAS::process(string strin) {
#if defined (_WIN32)
	strin.erase(remove_if(strin.begin(), strin.end(), isspace), strin.end());
#endif
	size_t pos_assign = 0;
	if ((pos_assign = strin.find(":=")) != string::npos) {
		if (IdentifierRec::runWith(strin.substr(0, pos_assign))) {
			ArithmeticExpression *aexp = ArithmeticExpression::create(strin.substr(pos_assign + 2, strin.length() - pos_assign - 2));
			for (vector<Variable>::iterator it = casinfo.variables.begin(); it != casinfo.variables.end(); ++it) {
				if (it->identifier == strin.substr(0, pos_assign)) {
					casinfo.variables.erase(it);
					break;
				}
			}
			casinfo.variables.push_back(Variable(strin.substr(0, pos_assign), aexp));
			casinfo.commands.push_back(aexp);
			VariablesModified = true;
			FunctionsModified = false;
			CommandsModified = true;
			return aexp->getString();
		} else {
			size_t pos_leftparenth = strin.find_first_of('(');
			size_t pos_rightparenth = strin.find_first_of(')', pos_leftparenth);
			string funcname = strin.substr(0, pos_leftparenth);
			if (pos_leftparenth == string::npos || pos_rightparenth == string::npos || ! IdentifierRec::runWith(funcname)) throw "Ungueltige Eingabe";
			string funcargs = strin.substr(pos_leftparenth + 1, pos_rightparenth - pos_leftparenth - 1);
			int pos_before = -1, pos_now = 0;
			vector<string> argvec;
			string tmpstr;
			while ((pos_now = funcargs.find(',', pos_before + 1)) != string::npos) {
				tmpstr = funcargs.substr(pos_before + 1, pos_now - pos_before - 1);
				if (IdentifierRec::runWith(tmpstr)) argvec.push_back(tmpstr);
				else throw "Ungueltige Funtionsargumente";
				pos_before = pos_now;
			}
			tmpstr = funcargs.substr(pos_before + 1, funcargs.length() - pos_before - 1);
			if (IdentifierRec::runWith(tmpstr)) argvec.push_back(tmpstr);
			else throw "Ungueltige Funtionsargumente";
			ArithmeticExpression *aexp = ArithmeticExpression::create(strin.substr(pos_assign + 2, strin.length() - pos_assign - 2));
			for (vector<Function>::iterator it = casinfo.functions.begin(); it != casinfo.functions.end(); ++it) {
				if (it->identifier == funcname) {
					casinfo.functions.erase(it);
					break;
				}
			}
			casinfo.functions.push_back(Function(funcname, argvec, aexp));
			casinfo.commands.push_back(aexp);
			VariablesModified = false;
			FunctionsModified = true;
			CommandsModified = true;
			return aexp->getString();
		}
	} else {
		VariablesModified = false;
		FunctionsModified = false;
		CommandsModified = false;
		ArithmeticExpression *tmpex = ArithmeticExpression::create(strin);;
		casinfo.commands.push_back(tmpex);
		CommandsModified = true;
		return tmpex->expand(casinfo)->getString();
	}
}

void CAS::deleteVariable(string i) {
	FunctionsModified = false;
	CommandsModified = false;
	for (vector<Variable>::iterator it = casinfo.variables.begin(); it != casinfo.variables.end(); ++it) {
		if (it->identifier == i) {
			casinfo.variables.erase(it);
			VariablesModified = true;
			return;
		}
	}
	VariablesModified = false;
}

void CAS::deleteFunction(string i) {
	VariablesModified = false;
	CommandsModified = false;
	for (vector<Function>::iterator it = casinfo.functions.begin(); it != casinfo.functions.end(); ++it) {
		if (it->identifier == i) {
			casinfo.functions.erase(it);
			FunctionsModified = true;
			return;
		}
	}
	FunctionsModified = false;
}

void CAS::clearVariables() {
	FunctionsModified = false;
	VariablesModified = true;
	CommandsModified = false;
	casinfo.variables.clear();
}

void CAS::clearFunctions() {
	FunctionsModified = true;
	VariablesModified = false;
	CommandsModified = false;
	casinfo.functions.clear();
}

void CAS::reset() {
	casinfo.functions.clear();
	casinfo.variables.clear();
	casinfo.commands.clear();
	VariablesModified = true;
	FunctionsModified = true;
    CommandsModified = true;
}

ArithmeticExpression *Transformation::expand(const ExpansionInformation &ei) const
{
    /*FunctionExpression *fp = dynamic_cast<FunctionExpression*>(function);
    if (! fp) throw "Erstes Argument zu transform() muss Funktion sein.";*/
    VariableExpression *vp_to = dynamic_cast<VariableExpression*>(var_to);
    if (! vp_to) throw "Zweites Argument zu transform() muss Variable sein.";
    VariableExpression *vp_fid = dynamic_cast<VariableExpression*>(var_fid);
    if (! vp_fid) throw "Drittes Argument zu transform() muss Variable sein.";

    //Path p = exp->getVarPath(vp_to->identifier);

    return exp->transformExpression(vp_to, vp_fid)->expand(ei);
}

bool Transformation::isEqual(ArithmeticExpression *other) const
{
    Transformation *tp = dynamic_cast<Transformation*>(other);
    return (tp && tp->exp->isEqual(exp) && tp->var_to->isEqual(var_to) && tp->var_fid->isEqual(var_fid));
}
