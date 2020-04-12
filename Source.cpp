#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Implicant.h"
#include "Kmap.h"

using namespace std;

const bool debug = false;

bool inputValid(string a);
int variables(string input);
void genKmap(Kmap& a, string input, const int var);

int main() {
	fstream file("input.txt", ios::in);
	if (file) {
		string input;
		getline(file, input);
		file.close();

		if (!inputValid(input)) {
			cout << "input.txt內資料錯誤!!" << endl << endl;
			system("pause");
			return 0;
		}

		// determine how many variables in the equation //
		const int var = variables(input);

		Kmap KarnaughMap(var);
		genKmap(KarnaughMap, input, var);

		if (debug) {
			cout << KarnaughMap << endl;
			cout << KarnaughMap.getPrime() << endl << endl;
		}
		else {
			file.open("output.txt", ios::out);
			file << KarnaughMap << endl;
			file << KarnaughMap.getPrime() << endl << endl;
			file.close();

			cout << "輸出完成!!!" << endl << endl;
		}
	}
	else {
		cout << "input.txt未找到!!" << endl << endl;
	}

	system("pause");
	return 0;
}

bool inputValid(string a) {
	if (a.length() == 0) {
		return false;
	}
	if (a.find('b', 0) == -1) {
		return false;
	}
	for (int i = 0; i < a.length(); i++) {
		if (!((a[i] <= 'd' && a[i] >= 'a') || a[i] == '(' || a[i] == ')' || a[i] == '\'' || a[i] == ' ' || a[i] == '+')) {
			return false;
		}
	}
	return true;
}

int variables(string input) {
	if (find(input.begin(), input.end(), 'd') != input.end()) {
		return 4;
	}
	else if (find(input.begin(), input.end(), 'c') != input.end()) {
		return 3;
	}
	else if (find(input.begin(), input.end(), 'b') != input.end()) {
		return 2;
	}
	else if (find(input.begin(), input.end(), 'a') != input.end()) {
		return 1;
	}
	else {
		cout << "input file error!" << endl;
		exit(1);
	}
}

void genKmap(Kmap& a, string input, const int var) {
	int last = 0;
	int now;

	while (input.find('+', last) != -1) {	// if there is a '+' after last finding
		now = input.find('+', last);
		if (input.at(last) == '(') {	// if the character after last '+' is '(', then this term must be don't care
			a.append(Implicant(string(input.begin() + last + 1, input.begin() + now - 1), var), false);
		}
		else {	// this term is a care term
			a.append(Implicant(string(input.begin() + last, input.begin() + now), var), true);
		}

		last = now + 1;
	}

	// check the last term in input //
	if (input.at(last) == '(') {
		a.append(Implicant(string(input.begin() + last + 1, input.end() - 1), var), false);
	}
	else {
		a.append(Implicant(string(input.begin() + last, input.end()), var), true);
	}

	// simplify the boolean equation of Kmap a //
	a.simplify();
}
