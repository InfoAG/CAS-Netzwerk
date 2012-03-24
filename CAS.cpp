#include "ArlecksCAS.h"

void Fehler(string meldung) {
	cout << meldung << endl;
	system("PAUSE");
	exit(0);
}

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
    // Skip delimiters at beginning.
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
    }
}

ArithmeticExpression *ArithmeticExpression::sqrt_func() {
	return new SquareRoot(this);
}

ArithmeticExpression *ArithmeticExpression::log_func() {
	return new Logarithm(this);
}

ArithmeticExpression *ArithmeticExpression::ln_func() {
	return new NaturalLogarithm(this);
}

ArithmeticExpression *ArithmeticExpression::sin_func() {
	return new Sinus(this);
}

ArithmeticExpression *ArithmeticExpression::cos_func() {
	return new Cosinus(this);
}

ArithmeticExpression *ArithmeticExpression::tan_func() {
	return new Tangent(this);
}

ArithmeticExpression *NumericalValue::sqrt_func() {
	return new NumericalValue(sqrt(value));
}

ArithmeticExpression *NumericalValue::log_func() {
	return new NumericalValue(log10(value));
}

ArithmeticExpression *NumericalValue::ln_func() {
	return new NumericalValue(log(value));
}

ArithmeticExpression *NumericalValue::sin_func() {
	return new NumericalValue(sin(value));
}

ArithmeticExpression *NumericalValue::cos_func() {
	return new NumericalValue(cos(value));
}

ArithmeticExpression *NumericalValue::tan_func() {
	return new NumericalValue(tan(value));
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
			if (*i == '(')  ebenencount--;
			else if (*i == ')') ebenencount++;
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
		}else if (pos_divi) {
			return new Division(ArithmeticExpression::create(strin.substr(0, pos_divi)), ArithmeticExpression::create(strin.substr(pos_divi + 1, strin.length() - pos_divi - 1)));
		} else if (pos_potenz) {
			return new Exponentiation(ArithmeticExpression::create(strin.substr(0, pos_potenz)), ArithmeticExpression::create(strin.substr(pos_potenz + 1, strin.length() - pos_potenz - 1)));
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
			Tokenize(strin.substr(10, strin.length() - 11), tokens, ","); //ALEX FUNCKOMMAS!!!!!!!
			if (tokens.size() != 5 || ! IdentifierRec::runWith(tokens.at(1)) || ! DecimalRec::runWith(tokens.at(2)) || ! DecimalRec::runWith(tokens.at(3)) || ! DecimalRec::runWith(tokens.at(4))) throw "Ungueltiger (Teil-)Term"; //UND HIER AUCH
			return new Integral(ArithmeticExpression::create(tokens.at(0)), new VariableExpression(tokens.at(1)), strtod(tokens.at(2).c_str(), NULL), strtod(tokens.at(3).c_str(), NULL), strtod(tokens.at(4).c_str(), NULL));
		} else if (strin.front() == '$' && DecimalRec::runWith(strin.substr(1, strin.length() - 1))) { //!!!!!!ALEX GUCK HIER
			return new CommandExpression(atoi(strin.substr(1, strin.length() - 1).c_str()));		//!!!!!!!!! decimal, atoi
		} else {
			size_t pos_leftparenth = strin.find_first_of('(');
			string funcname = strin.substr(0, pos_leftparenth);
			if (pos_leftparenth == string::npos || strin.back() != ')' || ! IdentifierRec::runWith(funcname)) throw "Ungueltiger (Teil-)Term";
			string funcargs = strin.substr(pos_leftparenth + 1, strin.length() - pos_leftparenth - 2);
			int pos_before = -1, pos_now = 0;
			vector<ArithmeticExpression*> argvec;
			while ((pos_now = funcargs.find(',', pos_before + 1)) != string::npos) {
				argvec.push_back(ArithmeticExpression::create(funcargs.substr(pos_before + 1, pos_now - pos_before - 1)));
				pos_before = pos_now;
			}
			argvec.push_back(ArithmeticExpression::create(funcargs.substr(pos_before + 1, funcargs.length() - pos_before - 1)));
			return new FunctionExpression(funcname, argvec);
		}
	}
}

ArithmeticExpression *Addition::expand(const ExpansionInformation& ei) const {
	//ArithmeticExpression *al = left->expand(ei);
	//ArithmeticExpression *ar = right->expand(ei);
	//NumericalValue *nl = dynamic_cast<NumericalValue*>(al);
	//NumericalValue *nr = dynamic_cast<NumericalValue*>(ar);
	//if (nl && nr) return new NumericalValue(nl->value + nr->value);
	//else return new Addition(al, ar);
	list<ArithmeticExpression*> tmpvec;
	Addition *nv;
	ArithmeticExpression *ax;
	for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != operands.end(); ++it) {
		ax = (*it)->expand(ei);
		if (nv = dynamic_cast<Addition*>(ax)) tmpvec.insert(tmpvec.begin(), nv->operands.begin(), nv->operands.end());
		else tmpvec.push_back(ax);
	}
	//Multiplication *mult, *mult_in;
	//for (list<ArithmeticExpression*>::iterator it = tmpvec.begin(); it != tmpvec.end(); ++it) {
	//	for (list<ArithmeticExpression*>::iterator it_in = tmpvec.begin(); it_in != tmpvec.end();) {
	//		if (it != it_in) {
	//			mult = dynamic_cast<Multiplication*>(*it);
	//			mult_in = dynamic_cast<Multiplication*>(*it_in);
	//			if (mult->left == mult_in->left) {
	//				mult->right = new Addition(mult->right, mult_in->right);
	//				it_in = tmpvec.erase(it_in);
	//			} else ++it_in;
	//		} else ++it_in;
	//	}
	//}
	NumericalValue *np;
	double dbuf = 0;
	for (list<ArithmeticExpression*>::iterator it = tmpvec.begin(); it != tmpvec.end();) {
		if (np = dynamic_cast<NumericalValue*>(*it)) {
			it = tmpvec.erase(it);
			dbuf += np->value;
		} else ++it;
	}

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
		tmpvec.push_back((*it)->expand(ei));
	return new Addition(tmpvec);
}

ArithmeticExpression *Subtraction::expand(const ExpansionInformation& ei) const {
	//ArithmeticExpression *al = left->expand(ei);
	//ArithmeticExpression *ar = right->expand(ei);
	//NumericalValue *nl = dynamic_cast<NumericalValue*>(al);
	//NumericalValue *nr = dynamic_cast<NumericalValue*>(ar);
	//if (nl && nr) return new NumericalValue(nl->value - nr->value);
	//else return new Subtraction(al, ar);
	return Addition(left, new Multiplication(new NumericalValue(-1), right)).expand(ei);
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
	//ArithmeticExpression *al = left->expand(ei);
	//ArithmeticExpression *ar = right->expand(ei);
	//NumericalValue *nl = dynamic_cast<NumericalValue*>(al);
	//NumericalValue *nr = dynamic_cast<NumericalValue*>(ar);
	//if (nl && nr) return new NumericalValue(nl->value * nr->value);
	//else return new Multiplication(al, ar);
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
		tmpvec.push_back((*it)->expand(ei)); //nicht expand?? für hoch1, oder exponent-expand ändern (aber endergebnis)
	NumericalValue *np;
	double dbuf = 1;
	for (list<ArithmeticExpression*>::iterator it = tmpvec.begin(); it != tmpvec.end();) {
		if (np = dynamic_cast<NumericalValue*>(*it)) {
			it = tmpvec.erase(it);
			dbuf *= np->value;
		} else ++it;
	}
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
	//NumericalValue *nl = dynamic_cast<NumericalValue*>(al);
	//NumericalValue *nr = dynamic_cast<NumericalValue*>(ar);
	//if (nl && nr) return new NumericalValue(nl->value / nr->value);
	//else return new Division(al, ar);
	Division *dp;
	NumericalValue *nl = dynamic_cast<NumericalValue*>(al), *nr = dynamic_cast<NumericalValue*>(ar);
	if (nl && nr) return new NumericalValue(nl->value / nr->value);
	else if (dp = dynamic_cast<Division*>(al)) return Division(dp->left->expand(ei), new Multiplication(dp->right->expand(ei), ar)).expand(ei);
	else if (dp = dynamic_cast<Division*>(ar)) return Division(new Multiplication(al, dp->right->expand(ei)), dp->left->expand(ei)).expand(ei);
	else {
		list<ArithmeticExpression*> explist;
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
			for (;;) {
				if (adl->operands.size() == 1) {
					nvp = dynamic_cast<NumericalValue*>(adl->operands.front());
					if (nvp && nvp->value == 0) break;
				}
				mlp = dynamic_cast<Multiplication*>(adl->operands.front());
				mrp = dynamic_cast<Multiplication*>(adr->operands.front());
				elp = dynamic_cast<Exponentiation*>(mlp->operands.back());
				erp = dynamic_cast<Exponentiation*>(mrp->operands.back());
				reslist.push_back(new Multiplication(new Division(mlp->operands.front(), mrp->operands.front()), new Exponentiation(elp->left, new Subtraction(elp->right, erp->right))));
				adl = dynamic_cast<Addition*>(Subtraction(adl, new Multiplication(reslist.back(), adr)).expand(ei));
			}
		}
		return Multiplication(reslist).expand(ei);
	} //return new Division(al, ar); //poly else expo form
}

ArithmeticExpression *Exponentiation::expand(const ExpansionInformation& ei) const {
	ArithmeticExpression *al = left->expand(ei);
	ArithmeticExpression *ar = right->expand(ei);
	NumericalValue *nl = dynamic_cast<NumericalValue*>(al);
	NumericalValue *nr = dynamic_cast<NumericalValue*>(ar);
	if (nl && nr) return new NumericalValue(pow(nl->value,  nr->value));
	else if (nr) {
		if (nr->value == 0) return new NumericalValue(1);
		else if (nr->value == 1) return al;
	}
	return new Exponentiation(al, ar);
}

//ArithmeticExpression *Subtraction::expand(const ExpansionInformation& ei) const {
//	return left->expand(ei)->subtract(right->expand(ei));
//}
//
//ArithmeticExpression *Multiplication::expand(const ExpansionInformation& ei) const {
//	return left->expand(ei)->multiply(right->expand(ei));
//}
//
//ArithmeticExpression *Division::expand(const ExpansionInformation& ei) const {
//	return left->expand(ei)->divide(right->expand(ei));
//}
//
//ArithmeticExpression *Exponentiation::expand(const ExpansionInformation& ei) const {
//	return left->expand(ei)->power(right->expand(ei));
//}

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
	return "integrate(" + aexp->getString() + "," + var->getString() + "," + dtostr(lower_bound) + "," + dtostr(upper_bound);
}

ArithmeticExpression *VariableExpression::expand(const ExpansionInformation& ei) const {
	for (vector<Variable>::const_iterator it = ei.funcvars.begin(); it != ei.funcvars.end(); ++it) {
			if (it->identifier == identifier) {
				if (it->expanded) throw "Fehler: rekursive Variable"; //?? möglich?
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

//ArithmeticExpression* Subtraction::formPolynom() const {
//	return new Subtraction(*this);
//}

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

//ArithmeticExpression* Division::formPolynom() const {
//	return new Division(*this);
//}

ArithmeticExpression* Exponentiation::formPolynom() const {
	list<ArithmeticExpression*> tmpvec;
	tmpvec.push_back(new Multiplication(new NumericalValue(1), new Exponentiation(*this)));
	return new Addition(tmpvec);
}

//ArithmeticExpression* SquareRoot::formPolynom() const {
//	list<ArithmeticExpression*> tmpvec;
//	tmpvec.push_back(new Multiplication(new NumericalValue(1), new Exponentiation(this->operand, new NumericalValue(0.5))));
//	return new Addition(tmpvec);
//}

//ArithmeticExpression* Logarithm::formPolynom() const {
//	return new Logarithm(*this);
//}
//
//ArithmeticExpression* NaturalLogarithm::formPolynom() const {
//	return new NaturalLogarithm(*this);
//}
//
//ArithmeticExpression* Sinus::formPolynom() const {
//	return new Sinus(*this);
//}
//
//ArithmeticExpression* Cosinus::formPolynom() const {
//	return new Cosinus(*this);
//}
//
//ArithmeticExpression* Tangent::formPolynom() const {
//	return new Tangent(*this);
//}
//ArithmeticExpression* NumericalValue::formPolynom() const {
//	return new NumericalValue(*this);
//}
//
//ArithmeticExpression* VariableExpression::formPolynom() const {
//	return new VariableExpression(*this);
//}
//
//ArithmeticExpression* FunctionExpression::formPolynom() const {
//	return new FunctionExpression(*this);
//}
//
//ArithmeticExpression* CommandExpression::formPolynom() const {
//	return new CommandExpression(*this);
//}
//
//ArithmeticExpression* Integral::formPolynom() const {
//	return new Integral(*this);
//}

Multiplication *ArithmeticExpression::formMonom() const {
	return new Multiplication(new NumericalValue(1), this->copy());
}

Multiplication *Multiplication::formMonom() const {
	return new Multiplication(this->operands);
}

string CommandExpression::getString() const {
	return "$" + id;
}
//virtual inline bool containsVariable(string id) const;
//virtual inline bool containsFunction(string id) const; 

string Addition::getString() const {
	string str;
	//if (operands.size() > 1) {
	//	list<ArithmeticExpression*>::const_iterator cit;
	//	for (list<ArithmeticExpression*>::const_iterator it = operands.begin();; ++it) {
	//		cit = it;
	//		if (++cit == operands.end()) { break; }
	//		else str += (*it)->getString() + '+';
	//	}
	//}
	for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != --(operands.end()); ++it)
			str += (*it)->getString() + '+';
	str += operands.back()->getString();
	return str;
}

string Subtraction::getString() const {
	return left->getString() + "-" + right->getString();
}

string Multiplication::getString() const {
	string str;
		/*list<ArithmeticExpression*>::const_iterator cit;
		for (list<ArithmeticExpression*>::const_iterator it = operands.begin();; ++it) {
			cit = it;
			if (++cit == operands.end()) { break; }
			else str += (*it)->getString() + '*';
		}*/
	for (list<ArithmeticExpression*>::const_iterator it = operands.begin(); it != --(operands.end()); ++it)
			str += (*it)->getString() + '*';
	str += operands.back()->getString();
	return str;
}

string Division::getString() const {
	return left->getString() + "/" + right->getString();
}

string Exponentiation::getString() const {
	return left->getString() + "^" + right->getString();
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

bool CommandExpression::isEqual(ArithmeticExpression *other) const {
	CommandExpression *cp = dynamic_cast<CommandExpression*>(other);
	return (cp->id == id);
}


ostream& operator << (ostream& os, const ArithmeticExpression& kt) {
	os << kt.getString();
	return os;
}

//ArithmeticExpression::ArithmeticExpression(string strin) {
//	ursprung = new ArithmeticExpression(strin);
//}
//
//ArithmeticExpression ArithmeticExpression::expand(const ExpansionInformation& ei) const {
//	return ursprung->expand(ei);
//}
//
//string ArithmeticExpression::getString() const {
//	return ursprung->getString();
//}
//
//ostream& operator << (ostream& os, const ArithmeticExpression& aexp) {
//	os << *(aexp.ursprung);
//	return os;
//}

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

ostream& operator<<(ostream& os, const Variable& var) {
	os << var.identifier << "=" << *(var.aexp);
	return os;
};

ostream& operator<<(ostream& os, const Function& func) {
	os << func.identifier << "(";
	for (vector<string>::const_iterator it = func.func_args.begin(); it != func.func_args.end(); ++it) {
		if (it != func.func_args.begin()) os << ",";
		os << (*it);
	}
	os << ")=" << *(func.aexp);
	return os;
};

ostream& operator<<(ostream& os, const Command& com) {
	os << *(com.aexp);
	return os;
};

string CAS::process(string strin) {
	strin.erase(remove_if(strin.begin(), strin.end(), isspace), strin.end());
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
			return aexp->getString();
		}
	} else {
		ArithmeticExpression *tmpex = ArithmeticExpression::create(strin);;
		casinfo.commands.push_back(tmpex);
		return tmpex->expand(casinfo)->getString();
	}
};

void CAS::deleteVariable(string i) {
	for (vector<Variable>::iterator it = casinfo.variables.begin(); it != casinfo.variables.end(); ++it) {
		if (it->identifier == i) {
			casinfo.variables.erase(it);
			return;
		}
	}
}

void CAS::deleteFunction(string i) {
	for (vector<Function>::iterator it = casinfo.functions.begin(); it != casinfo.functions.end(); ++it) {
		if (it->identifier == i) {
			casinfo.functions.erase(it);
			return;
		}
	}
}

void CAS::clearVariables() {
	casinfo.variables.clear();
}

void CAS::clearFunctions() {
	casinfo.functions.clear();
}