#ifndef KMAP_H
#define KMAP_H
#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include "Implicant.h"
using namespace std;

ostream& operator<<(ostream&, vector<Implicant>);

class Kmap {
	friend ostream& operator<<(ostream&, Kmap&);

private:
	vector<Implicant> care_item;	// original care implicants
	vector<Implicant> dont_care;	// origin don't care implicants
	vector<Implicant> prime;	// selected implicants
	int var;	// amount of variables
	vector<bool> care_value;
	vector<bool> dontCare_value;
	string blockName(vector<pair<int, int>>);
	vector<Implicant> bestPrime(const vector<Implicant>&,vector<Implicant>, vector<bool>,  int);
public:
	Kmap(int);
	void append(Implicant, bool);
	void simplify();
	vector<bool> getCareValue();
	vector<bool> getDontCareValue();
	vector<Implicant> getPrime();
	char getChar(int);
};


#endif
