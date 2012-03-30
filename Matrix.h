#ifndef _MATRIX__H
#define _MATRIX__H


class Matrix
{
private:
	int n;
	int** coeff;
	int* res;
	
public:
	Matrix(int g, int** c, int* r) : n(g),coeff(c),res(r) {};
	int calcDet();
	
	
	
};











#endif