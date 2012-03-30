#include "Matrix.h"
#include <iostream>
#include <ctime>

using namespace std;

int main()
{
	int groesse;
	cout << "Wie viele Gleichungen wollen Sie eingeben?: ";
	cin >> groesse;
	
	double* m_ergebnisse = new double[groesse];
	double **m_array = new double*[groesse];	
	
	for(int i = 0; i < groesse; i++)
	{
		m_array[i] = new double[groesse];
	}
	
	
	for(int i = 0; i < groesse; i++)
	{
		cout << "Geben Sie die Koeffizienten der " << i+1 << ". Gleichung ein: " << endl;
		
		for(int j = 0; j < groesse; j++)
		{
			cout << "a" << groesse-j << ": ";
			cin >> m_array[i][j];
		}
		
		cout << endl;
	}
	
	for(int i = 0; i < groesse; i++)
	{
	    cout << "Geben Sie die Loesung der " << i+1 << ". Gleichung ein: ";
	    cin >> m_ergebnisse[i];
	}
	
	cout << endl;
	
	Matrix test(groesse, m_array, m_ergebnisse);
	
	cout << test << endl;
	
	test.setResults();
	
	double* ergebnisse = test.getResults();
	
	for(int i = 0; i < groesse; i++)
	{
		cout << (char)(i+97) << " = " << ergebnisse[i] << endl;
	}
	
	
	return 0;
}

