#include "Implicant.h"
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

void Implicant::computeDecimal()
{
	int careNum = 0;
	vector<int> dontCare;

	switch (variables) {
	case 4:
		// compute care variables //
		for (int i = 0; i < 4; i++) {
			if (boolean[i] == -1) {
				dontCare.push_back(i);
				continue;
			}
			if (boolean[i] == 1) {
				careNum += pow(2, 3 - i);
			}
		}
		decimalNum.insert(careNum);

		if (dontCare.size() == 1) {
			// 1 combination of 1 //
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)));
		}
		else if (dontCare.size() == 2) {
			// 2 combinations of 1 //
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(1)));
			// 2 combination of 2 //
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)) + pow(2, 3 - dontCare.at(1)));
		}
		else if (dontCare.size() == 3) {
			// 3 combinations of 1 //
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(1)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(2)));
			// 3 combinations of 2 //
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)) + pow(2, 3 - dontCare.at(1)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)) + pow(2, 3 - dontCare.at(2)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(1)) + pow(2, 3 - dontCare.at(2)));
			// 3 combination of 3 //
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)) + pow(2, 3 - dontCare.at(1)) + pow(2, 3 - dontCare.at(2)));
		}
		else if (dontCare.size() == 4) {
			for (int i = 0; i < 16; i++) {
				decimalNum.insert(i);
			}
		}

		break;

	case 3:
		// compute care variables //
		for (int i = 0; i < 3; i++) {
			if (boolean[i] == -1) {
				dontCare.push_back(i);
				continue;
			}
			if (boolean[i] == 1) {
				careNum += pow(2, 2 - i);
			}
		}
		decimalNum.insert(careNum);

		if (dontCare.size() == 1) {
			// 1 combination of 1 //
			decimalNum.insert(careNum + pow(2, 2 - dontCare.at(0)));
		}
		else if (dontCare.size() == 2) {
			// 2 combinations of 1 //
			decimalNum.insert(careNum + pow(2, 2 - dontCare.at(0)));
			decimalNum.insert(careNum + pow(2, 2 - dontCare.at(1)));
			// 2 combination of 2 //
			decimalNum.insert(careNum + pow(2, 2 - dontCare.at(0)) + pow(2, 2 - dontCare.at(1)));
		}
		else if (dontCare.size() == 3) {
			for (int i = 0; i < 8; i++) {
				decimalNum.insert(i);
			}
		}

		break;

	case 2:
		// compute care variables //
		for (int i = 0; i < 2; i++) {
			if (boolean[i] == -1) {
				dontCare.push_back(i);
				continue;
			}
			if (boolean[i] == 1) {
				careNum += pow(2, 1 - i);
			}
		}
		decimalNum.insert(careNum);

		if (dontCare.size() == 1) {
			// 1 combination of 1 //
			decimalNum.insert(careNum + pow(2, 1 - dontCare.at(0)));
		}
		else if (dontCare.size() == 2) {
			for (int i = 0; i < 4; i++) {
				decimalNum.insert(i);
			}
		}

		break;
	}
}

Implicant::Implicant(int value, int v) :variables(v)
{
	switch (variables) {
	case 4:	// turn the value into 4 bits boolean term
		if (value >= 8) {
			boolean[0] = 1;
			value -= 8;
		}
		else {
			boolean[0] = 0;
		}
		if (value >= 4) {
			boolean[1] = 1;
			value -= 4;
		}
		else {
			boolean[1] = 0;
		}
		if (value >= 2) {
			boolean[2] = 1;
			value -= 2;
		}
		else {
			boolean[2] = 0;
		}
		if (value == 1) {
			boolean[3] = 1;
		}
		else {
			boolean[3] = 0;
		}

		break;
	case 3:	// turn the value into 3 bits boolean term
		if (value >= 4) {
			boolean[0] = 1;
			value -= 4;
		}
		else {
			boolean[0] = 0;
		}
		if (value >= 2) {
			boolean[1] = 1;
			value -= 2;
		}
		else {
			boolean[1] = 0;
		}
		if (value == 1) {
			boolean[2] = 1;
		}
		else {
			boolean[2] = 0;
		}
		boolean[3] = -1;

		break;
	case 2:	// turn the value into 2 bits boolean term
		if (value >= 2) {
			boolean[0] = 1;
			value -= 2;
		}
		else {
			boolean[0] = 0;
		}
		if (value == 1) {
			boolean[1] = 1;
		}
		else {
			boolean[1] = 0;
		}
		boolean[2] = -1;
		boolean[3] = -1;

		break;
	}

	computeDecimal();
}

Implicant::Implicant(string input, int v)
	:variables(v)
{
	const int length = input.length();
	int char_now;

	// a //
	char_now = input.find('a', 0);
	if (char_now != -1) {
		if (char_now + 1 < length) {
			if (input.at(char_now + 1) == '\'') {
				boolean[0] = 0;
			}
			else {
				boolean[0] = 1;
			}
		}
		else {
			boolean[0] = 1;
		}
	}
	else {
		boolean[0] = -1;
	}

	// b //
	char_now = input.find('b', 0);
	if (char_now != -1) {
		if (char_now + 1 < length) {
			if (input.at(char_now + 1) == '\'') {
				boolean[1] = 0;
			}
			else {
				boolean[1] = 1;
			}
		}
		else {
			boolean[1] = 1;
		}
	}
	else {
		boolean[1] = -1;
	}

	// c //
	char_now = input.find('c', 0);
	if (char_now != -1) {
		if (char_now + 1 < length) {
			if (input.at(char_now + 1) == '\'') {
				boolean[2] = 0;
			}
			else {
				boolean[2] = 1;
			}
		}
		else {
			boolean[2] = 1;
		}
	}
	else {
		boolean[2] = -1;
	}

	// d //
	char_now = input.find('d', 0);
	if (char_now != -1) {
		if (char_now + 1 < length) {
			if (input.at(char_now + 1) == '\'') {
				boolean[3] = 0;
			}
			else {
				boolean[3] = 1;
			}
		}
		else {
			boolean[3] = 1;
		}
	}
	else {
		boolean[3] = -1;
	}

	computeDecimal();
}

set<int> Implicant::getDecimal() const
{
	return decimalNum;
}

int Implicant::care() const
{
	// count the amount of care variables //
	int count = 0;
	for (int i = 0; i < 4; i++) {
		if (boolean[i] == -1) {
			count++;
		}
	}
	return 4 - count;
}

bool Implicant::operator==(Implicant& a)
{
	for (int i = 0; i < 4; i++) {
		if (boolean[i] != a.boolean[i]) {
			return false;
		}
	}
	if (variables != a.variables) {
		return false;
	}
	if (decimalNum != a.decimalNum) {
		return false;
	}
	return true;
}

int Implicant::getVariables()
{
	return variables;
}

bool Implicant::all()
{
	for (int i = 0; i < 4; i++) {
		if (boolean[i] != -1) {
			return false;
		}
	}
	return true;
}

bool Implicant::contain(Implicant& a)
{
	set<int> dec = a.decimalNum;
	for (set<int>::iterator it = dec.begin(); it != dec.end(); it++) {
		if (decimalNum.find(*it) == decimalNum.end()) {
			return false;
		}
	}
	return true;
}

ostream& operator<<(ostream& output, Implicant& a)
{
	for (int i = 0; i < 4; i++) {
		switch (a.boolean[i]) {
		case 1:
			output << (char)('a' + i);
			break;
		case 0:
			output << (char)('a' + i) << '\'';
			break;
		}
	}
	return output;
}
