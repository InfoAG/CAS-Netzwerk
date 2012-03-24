#include "CAS.h"

using namespace std;

int main(int argc, char *argv[]) {
	cout << "      ::::::::::: ::::    ::: :::::::::: ::::::::      :::      ::::::::" << endl;
	cout << "         :+:     :+:+:   :+: :+:       :+:    :+:   :+: :+:   :+:    :+:" << endl;
	cout << "        +:+     :+:+:+  +:+ +:+       +:+    +:+  +:+   +:+  +:+" << endl;
	cout << "       +#+     +#+ +:+ +#+ :#::+::#  +#+    +:+ +#++:++#++: :#:" << endl;
	cout << "      +#+     +#+  +#+#+# +#+       +#+    +#+ +#+     +#+ +#+   +#+#" << endl;
	cout << "     #+#     #+#   #+#+# #+#       #+#    #+# #+#     #+# #+#    #+#" << endl;
	cout << "########### ###    #### ###        ########  ###     ###  ########" << endl;
	cout << endl << endl;
	cout << "\"help\" fuer Hilfe" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << endl;

	string instr;
	CAS mycas;
	for (;;) {
		cout << "->";
		getline(cin, instr);
		if (instr == "help") {
			cout << endl;
			cout << "Grundbefehle:" << endl;
			cout << "->print variables" << endl;
			cout << "->print functions" << endl;
			cout << "->print commands" << endl;
			cout << endl;
			cout << "Definitionen:" << endl;
			cout << "->f(x,y):=x^sin(x)" << endl;
			cout << "->var:=3.5" << endl;
			cout << endl;
			cout << "Loeschen:" << endl;
			cout << "->delete variable $identifier" << endl;
			cout << "->delete function $identifier" << endl;
			cout << "->clear variables" << endl;
			cout << "->clear functions" << endl;
			cout << endl;
			cout << "Ausdruecke auswerten:" << endl;
			cout << "->integrate(x^2,x,0,1,100) (Funktion, Variable, untere Grenze, obere Grenze, Teilintervalle)" << endl;
			cout << "->Grundrechenarten, sqrt(), log(), ln(), sin(), cos(), tan(), funktion(3,4)" << endl;
			cout << "->$n steht fuer den Ausdruck des n-ten Kommandos" << endl;
			cout << endl;
			cout << endl;
		} else if (instr == "quit") exit(0);
		else if (instr == "print variables") {
			vector<Variable> varvec = mycas.getVariables();
			for (vector<Variable>::iterator it = varvec.begin(); it != varvec.end(); ++it)
				cout << "[" << it - varvec.begin() + 1 << "]\t" << *it << endl;
		} else if (instr == "print functions") {
			vector<Function> funcvec = mycas.getFunctions();
			for (vector<Function>::iterator it = funcvec.begin(); it != funcvec.end(); ++it)
				cout << "[" << it - funcvec.begin() + 1 << "]\t" << *it << endl;
		} else if (instr == "print commands") {
			vector<Command> comvec = mycas.getCommands();
			for (vector<Command>::iterator it = comvec.begin(); it != comvec.end(); ++it)
				cout << "[" << it - comvec.begin() + 1 << "]\t" << *it << endl;
		} else if (instr.substr(0, 15) == "delete variable") {
			mycas.deleteVariable(instr.substr(16, instr.length() - 16));
		} else if (instr.substr(0, 15) == "delete function") {
			mycas.deleteFunction(instr.substr(16, instr.length() - 16));
		} else if (instr.substr(0, 15) == "clear functions") {
			mycas.clearFunctions();
		} else if (instr.substr(0, 15) == "clear variables") {
			mycas.clearVariables();
		} else if (instr.substr(0, 5) == "reset") {
			mycas.reset();
		} else {
			try {
				cout << mycas.process(instr) << endl;
			} catch (const char *s) {
				cout << s << endl;
			}
		}
	}
	return 0;
}