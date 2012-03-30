#include "Matrix.h"
#include <iostream>

using namespace std;

int main()
{
	int** k = new int*[3];
	
	for(int i = 0; i < 3; i++)
	{
		k[i] = new int[3];
	}
	
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			k[i][j] = rand() % 10+1;
		}
	}
	
	
	int* e = new int[3];
	for(int i = 0; i < 3; i++)
	{
		e[i] = rand() % 10+1;
	}
	
	Matrix test(3, k, e);
	
	
	return 0;
}

