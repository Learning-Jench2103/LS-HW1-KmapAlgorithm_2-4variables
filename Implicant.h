#ifndef IMPLICANT_H
#define IMPLICANT_H

#include <iostream>
#include <string>
#include <set>

using namespace std;

class Implicant {
	friend ostream& operator<<(ostream&, Implicant&);
private:
	int boolean[4];
	int variables;
	set<int> decimalNum;
	void computeDecimal();
public:
	Implicant(int, int);
	Implicant(string, int);
	set<int> getDecimal() const;
	int care() const;
	bool operator==(Implicant&);
	int getVariables();
	bool all();
};

#endif
