#include "ArlecksCAS.h"
#include <conio.h>

using namespace std;
/*
ToDo:
Vereifachen
Klammersetzung
Kovertierung binary, hex, okt, Einheiten?
*/

int main(int argc, char *argv[]) {
	cout << "8\"\"\"\"8 8\"\"\"8  8     8\"\"\"\" 8\"\"\"\"8 8   8  8\"\"\"\"8       8\"\"\"\"8 8\"\"\"\"8 8\"\"\"\"8" << endl;
	cout << "8    8 8   8  8     8     8    \" 8   8  8            8    \" 8    8 8" << endl;
	cout << "8eeee8 8eee8e 8e    8eeee 8e     8eee8e 8eeeee       8e     8eeee8 8eeeee" << endl;
	cout << "88   8 88   8 88    88    88     88   8     88       88     88   8     88" << endl;
	cout << "88   8 88   8 88    88    88   e 88   8 e   88       88   e 88   8 e   88" << endl;
	cout << "88   8 88   8 88eee 88eee 88eee8 88   8 8eee88       88eee8 88   8 8eee88" << endl;
	cout << endl << endl;
	//cout << "\"help\" fuer Hilfe :D" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	//cout << "->help";

	string instr;
	CAS mycas;
	//int deletes = 0;
	//char input;
	//while ((input = _getch()) != '\r' && input != '\n') {
	//	if (input == '\b') {
	//		if (deletes < 4) {
	//			cout.put('\b');
	//			cout.put(' ');
	//			cout.put('\b');
	//			instr.erase(--(instr.end()));
	//			deletes++;
	//		}
	//	} else {
	//		instr += input;
	//		deletes--;
	//		cout.put(input);
	//	}
	//}
	cout << endl;
	for (;;) {
		cout << "->";
		getline(cin, instr);
		if (instr == "help") {
			cout << endl;
			cout << "Grundbefehle:" << endl;
			cout << "->print ei.variables" << endl;
			cout << "->print ei.functions" << endl;
			cout << "->print ei.commands" << endl;
			cout << endl;
			cout << "Definitionen:" << endl;
			cout << "->f(x,y):=x^sin(x)" << endl;
			cout << "->var:=3.5" << endl;
			cout << endl;
			cout << "Loeschen:" << endl;
			cout << "->delete variable $identifier" << endl;
			cout << "->delete function $identifier" << endl;
			cout << "->clear ei.variables" << endl;
			cout << "->clear ei.functions" << endl;
			cout << endl;
			cout << "Ausdruecke auswerten:" << endl;
			cout << "->integrate(x^2,x,0,1,100) (Funktion, Variable, untere Grenze, obere Grenze, Teilintervalle)" << endl;
			cout << "->Grundrechenarten, sqrt(), log(), ln(), sin(), cos(), tan(), funktion(3,4)" << endl;
			cout << "->$n steht fuer den Ausdruck des n-ten Kommandos" << endl;
			cout << endl << endl;
			cout << "TODO: vereinfachen, Klammersetzung (wird richtig interpretiert, aber nicht angezeigt), FEHLER FIXEN WENN EIKE NOERGELT" << endl;
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