#include "Kmap.h"
#include <algorithm>

string Kmap::blockName(vector<pair<int, int>> a)
{
	// create tables about the boolean equation of each position //
	vector<vector<vector<int>>> var4(4, vector<vector<int>>(4, vector<int>(4)));
	var4[0][0] = { 0,0,0,0 }; var4[0][1] = { 0,1,0,0 }; var4[0][2] = { 1,1,0,0 }; var4[0][3] = { 1,0,0,0 };
	var4[1][0] = { 0,0,0,1 }; var4[1][1] = { 0,1,0,1 }; var4[1][2] = { 1,1,0,1 }; var4[1][3] = { 1,0,0,1 };
	var4[2][0] = { 0,0,1,1 }; var4[2][1] = { 0,1,1,1 }; var4[2][2] = { 1,1,1,1 }; var4[2][3] = { 1,0,1,1 };
	var4[3][0] = { 0,0,1,0 }; var4[3][1] = { 0,1,1,0 }; var4[3][2] = { 1,1,1,0 }; var4[3][3] = { 1,0,1,0 };
	vector<vector<vector<int>>> var3(2, vector<vector<int>>(4, vector<int>(3)));
	var3[0][0] = { 0,0,0 }; var3[0][1] = { 0,1,0 }; var3[0][2] = { 1,1,0 }; var3[0][3] = { 1,0,0 };
	var3[1][0] = { 0,0,1 }; var3[1][1] = { 0,1,1 }; var3[1][2] = { 1,1,1 }; var3[1][3] = { 1,0,1 };
	vector<vector<vector<int>>> var2(2, vector<vector<int>>(2, vector<int>(2)));
	var2[0][0] = { 0,0 }; var2[0][1] = { 1,0 }; var2[1][0] = { 0,1 }; var2[1][1] = { 1,1 };

	vector<int> curr;
	switch (var) {
	case 4:
		curr = var4[a.at(0).first][a.at(0).second];
		for (int i = 1; i < a.size(); i++) {
			for (int j = 0; j < var; j++) {
				if (var4[a.at(i).first][a.at(i).second].at(j) != curr.at(j) && curr.at(j) != -1) {
					curr.at(j) = -1;
				}
			}
		}

		break;
	case 3:
		curr = var3[a.at(0).first][a.at(0).second];
		for (int i = 1; i < a.size(); i++) {
			for (int j = 0; j < var; j++) {
				if (var3[a.at(i).first][a.at(i).second].at(j) != curr.at(j) && curr.at(j) != -1) {
					curr.at(j) = -1;
				}
			}
		}

		break;
	case 2:
		curr = var2[a.at(0).first][a.at(0).second];
		for (int i = 1; i < a.size(); i++) {
			for (int j = 0; j < var; j++) {
				if (var2[a.at(i).first][a.at(i).second].at(j) != curr.at(j) && curr.at(j) != -1) {
					curr.at(j) = -1;
				}
			}
		}

		break;
	}

	string output;
	for (int i = 0; i < curr.size(); i++) {
		if (curr.at(i) == 1) {
			output += (char)('a' + i);
		}
		else if (curr.at(i) == 0) {
			output += (char)('a' + i);
			output += '\'';
		}
	}

	return output;


}

vector<Implicant> Kmap::bestPrime(const vector<Implicant>& imp, vector<Implicant> chosen, vector<bool> careList, int index)
{
	// this combination can't cover all care-number //
	if (index == imp.size()) {
		chosen.clear();
		return chosen;
	}

	// this term can't cover any care-number which is uncovered //
	set<int> curr_set = imp.at(index).getDecimal();
	bool coverd = false;
	for (set<int>::iterator it = curr_set.begin(); it != curr_set.end(); it++) {
		if (care_value.at(*it) && !careList.at(*it)) {
			coverd = true;
			careList.at(*it) = true;
		}
	}
	if (!coverd) {
		chosen.clear();
		return chosen;
	}

	chosen.push_back(imp.at(index));

	// check if all care-number are coverd with this term //
	bool uncovered = false;
	for (int i = 0; i < pow(2, var); i++) {
		if (care_value.at(i) && !careList.at(i)) {
			uncovered = true;
			break;
		}
	}
	if (!uncovered) {
		return chosen;
	}
	if (index == imp.size() - 1 && uncovered) {
		chosen.clear();
		return chosen;
	}

	vector<Implicant> shortest;
	vector<Implicant> reply;

	for (int i = index + 1; i < imp.size(); i++) {
		reply = bestPrime(imp, chosen, careList, i);
		if (reply.size() != 0 && shortest.size() == 0) {
			shortest = reply;
		}
		else if (reply.size() != 0 && reply.size() < shortest.size()) {
			shortest = reply;
		}
	}

	if (shortest.size() != 0) {
		return shortest;
	}
	else {
		chosen.clear();
		return chosen;
	}

}

Kmap::Kmap(int v)
	:var(v)
{
	care_value.resize(pow(2, var), false);
	dontCare_value.resize(pow(2, var), false);
}

void Kmap::append(Implicant a, bool care)
{
	if (care) {
		care_item.push_back(a);
		set<int> dec = a.getDecimal();
		for (set<int>::iterator it = dec.begin(); it != dec.end(); it++) {
			care_value.at(*it) = true;
		}
	}
	else {
		dont_care.push_back(a);
		set<int> dec = a.getDecimal();
		for (set<int>::iterator it = dec.begin(); it != dec.end(); it++) {
			dontCare_value.at(*it) = true;
		}
	}
}

void Kmap::simplify()
{
	vector<Implicant> term_list;
	vector<vector<int>> m;
	vector<pair<int, int>> posi;
	int count_true;

	bool all = true;
	for (int i = 0; i < pow(2, var); i++) {
		if (!care_value.at(i) && !dontCare_value.at(i)) {
			all = false;
			break;
		}
	}
	if (all) {
		prime.push_back(Implicant(string(" "), var));
		return;
	}

	switch (var) {
	case 4:
		m.resize(4);
		for (int i = 0; i < 4; i++) {
			m.at(i).resize(4);
		}
		if (care_value.at(0)) m[0][0] = 1; else if (dontCare_value.at(0)) m[0][0] = -1; else m[0][0] = 0;	// (0,0) = 0
		if (care_value.at(4)) m[0][1] = 1; else if (dontCare_value.at(4)) m[0][1] = -1; else m[0][1] = 0;	// (0,1) = 4
		if (care_value.at(12)) m[0][2] = 1; else if (dontCare_value.at(12)) m[0][2] = -1; else m[0][2] = 0;	// (0,2) = 12
		if (care_value.at(8)) m[0][3] = 1; else if (dontCare_value.at(8)) m[0][3] = -1; else m[0][3] = 0;	// (0,3) = 8
		if (care_value.at(1)) m[1][0] = 1; else if (dontCare_value.at(1)) m[1][0] = -1; else m[1][0] = 0;	// (1,0) = 1
		if (care_value.at(5)) m[1][1] = 1; else if (dontCare_value.at(5)) m[1][1] = -1; else m[1][1] = 0;	// (1,1) = 5
		if (care_value.at(13)) m[1][2] = 1; else if (dontCare_value.at(13)) m[1][2] = -1; else m[1][2] = 0;	// (1,2) = 13
		if (care_value.at(9)) m[1][3] = 1; else if (dontCare_value.at(9)) m[1][3] = -1; else m[1][3] = 0;	// (1,3) = 9
		if (care_value.at(3)) m[2][0] = 1; else if (dontCare_value.at(3)) m[2][0] = -1; else m[2][0] = 0;	// (2,0) = 3
		if (care_value.at(7)) m[2][1] = 1; else if (dontCare_value.at(7)) m[2][1] = -1; else m[2][1] = 0;	// (2,1) = 7
		if (care_value.at(15)) m[2][2] = 1; else if (dontCare_value.at(15)) m[2][2] = -1; else m[2][2] = 0;	// (2,2) = 15
		if (care_value.at(11)) m[2][3] = 1; else if (dontCare_value.at(11)) m[2][3] = -1; else m[2][3] = 0;	// (2,3) = 11
		if (care_value.at(2)) m[3][0] = 1; else if (dontCare_value.at(2)) m[3][0] = -1; else m[3][0] = 0;	// (3,0) = 2
		if (care_value.at(6)) m[3][1] = 1; else if (dontCare_value.at(6)) m[3][1] = -1; else m[3][1] = 0;	// (3,1) = 6
		if (care_value.at(14)) m[3][2] = 1; else if (dontCare_value.at(14)) m[3][2] = -1; else m[3][2] = 0;	// (3,2) = 14
		if (care_value.at(10)) m[3][3] = 1; else if (dontCare_value.at(10)) m[3][3] = -1; else m[3][3] = 0;	// (3,3) = 10

		// scan 2x4 //
		for (int r = 0; r < 4; r++) {
			count_true = 0;
			posi.clear();
			if (r < 3) {	// r = 0 , 1, 2
				for (int i = r; i < r + 2; i++) {
					for (int j = 0; j < 4; j++) {
						if (m.at(i).at(j) != 0) {
							count_true++;
							posi.push_back(pair<int, int>(i, j));
						}
					}
				}
				if (posi.size() == 8) term_list.push_back(Implicant(blockName(posi), var));
			}
			else {	// r = 3
				for (int j = 0; j < 4; j++) {
					if (m.at(0).at(j) != 0) {
						count_true++;
						posi.push_back(pair<int, int>(0, j));
					}
				}
				for (int j = 0; j < 4; j++) {
					if (m.at(3).at(j) != 0) {
						count_true++;
						posi.push_back(pair<int, int>(3, j));
					}
				}
				if (posi.size() == 8) term_list.push_back(Implicant(blockName(posi), var));
			}
		}

		// scan 4x2 //
		for (int c = 0; c < 4; c++) {
			count_true = 0;
			posi.clear();
			if (c < 3) {	// r = 0 , 1, 2
				for (int i = 0; i < 4; i++) {
					for (int j = c; j < c + 2; j++) {
						if (m.at(i).at(j) != 0) {
							count_true++;
							posi.push_back(pair<int, int>(i, j));
						}
					}
				}
				if (posi.size() == 8) term_list.push_back(Implicant(blockName(posi), var));
			}
			else {	// r = 3
				for (int j = 0; j < 4; j++) {
					if (m.at(j).at(0) != 0) {
						count_true++;
						posi.push_back(pair<int, int>(j, 0));
					}
				}
				for (int j = 0; j < 4; j++) {
					if (m.at(j).at(3) != 0) {
						count_true++;
						posi.push_back(pair<int, int>(j, 3));
					}
				}
				if (posi.size() == 8) term_list.push_back(Implicant(blockName(posi), var));
			}
		}

		// scan 1x4 //
		for (int i = 0; i < 4; i++) {
			count_true = 0;
			posi.clear();
			for (int j = 0; j < 4; j++) {
				if (m.at(i).at(j) != 0) {
					count_true++;
					posi.push_back(pair<int, int>(i, j));
				}
			}
			if (posi.size() == 4) term_list.push_back(Implicant(blockName(posi), var));
		}

		// scan 4x1 //
		for (int j = 0; j < 4; j++) {
			count_true = 0;
			posi.clear();
			for (int i = 0; i < 4; i++) {
				if (m.at(i).at(j) != 0) {
					count_true++;
					posi.push_back(pair<int, int>(i, j));
				}
			}
			if (posi.size() == 4) term_list.push_back(Implicant(blockName(posi), var));
		}

		// scan 2x2 //
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				posi.clear();
				for (int i = 0; i < 2 && i >= 0; i++) {
					if (r == 3 && i == 1) {
						i = -3;
					}
					for (int j = 0; j < 2 && j >= 0; j++) {
						if (c == 3 && j == 1) {
							j = -3;
						}
						if (m.at(r + i).at(c + j) != 0) {
							posi.push_back(pair<int, int>(r + i, c + j));
						}

					}
				}
				if (posi.size() == 4) term_list.push_back(Implicant(blockName(posi), var));
			}
		}

		// scan 1x2 //
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				posi.clear();
				for (int j = 0; j < 2 && j >= 0; j++) {
					if (c == 3 && j == 1) {
						j = -3;
					}
					if (m.at(r).at(c + j) != 0) {
						posi.push_back(pair<int, int>(r, c + j));
					}
				}
				if (posi.size() == 2) term_list.push_back(Implicant(blockName(posi), var));
			}
		}

		// scan 2x1 //
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				posi.clear();
				for (int i = 0; i < 2 && i >= 0; i++) {
					if (r == 3 && i == 1) {
						i = -3;
					}
					if (m.at(r + i).at(c) != 0) {
						posi.push_back(pair<int, int>(r + i, c));
					}
				}
				if (posi.size() == 2) term_list.push_back(Implicant(blockName(posi), var));
			}
		}

		break;
	case 3:
		m.resize(2);
		for (int i = 0; i < 2; i++) {
			m.at(i).resize(4);
		}
		if (care_value.at(0)) m[0][0] = 1; else if (dontCare_value.at(0)) m[0][0] = -1; else m[0][0] = 0;	// (0,0) = 0
		if (care_value.at(2)) m[0][1] = 1; else if (dontCare_value.at(2)) m[0][1] = -1; else m[0][1] = 0;	// (0,1) = 2
		if (care_value.at(6)) m[0][2] = 1; else if (dontCare_value.at(6)) m[0][2] = -1; else m[0][2] = 0;	// (0,2) = 6
		if (care_value.at(4)) m[0][3] = 1; else if (dontCare_value.at(4)) m[0][3] = -1; else m[0][3] = 0;	// (0,3) = 4
		if (care_value.at(1)) m[1][0] = 1; else if (dontCare_value.at(1)) m[1][0] = -1; else m[1][0] = 0;	// (1,0) = 1
		if (care_value.at(3)) m[1][1] = 1; else if (dontCare_value.at(3)) m[1][1] = -1; else m[1][1] = 0;	// (1,1) = 3
		if (care_value.at(7)) m[1][2] = 1; else if (dontCare_value.at(7)) m[1][2] = -1; else m[1][2] = 0;	// (1,2) = 7
		if (care_value.at(5)) m[1][3] = 1; else if (dontCare_value.at(5)) m[1][3] = -1; else m[1][3] = 0;	// (1,3) = 5

		// scan 1x4 //
		for (int i = 0; i < 2; i++) {
			count_true = 0;
			posi.clear();
			for (int j = 0; j < 4; j++) {
				if (m.at(i).at(j) != 0) {
					count_true++;
					posi.push_back(pair<int, int>(i, j));
				}
			}
			if (posi.size() == 4) term_list.push_back(Implicant(blockName(posi), var));
		}

		// scan 2x2 //
		for (int c = 0; c < 4; c++) {
			posi.clear();
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2 && j >= 0; j++) {
					if (c == 3 && j == 1) {
						j = -3;
					}
					if (m.at(i).at(c + j) != 0) {
						posi.push_back(pair<int, int>(i, c + j));
					}
				}
			}
			if (posi.size() == 4) term_list.push_back(Implicant(blockName(posi), var));
		}

		// scan 1x2 //
		for (int r = 0; r < 2; r++) {
			for (int c = 0; c < 4; c++) {
				posi.clear();
				for (int j = 0; j < 2 && j >= 0; j++) {
					if (c == 3 && j == 1) {
						j = -3;
					}
					if (m.at(r).at(c + j) != 0) {
						posi.push_back(pair<int, int>(r, c + j));
					}
				}
				if (posi.size() == 2) term_list.push_back(Implicant(blockName(posi), var));
			}
		}

		// scan 2x1 //
		for (int c = 0; c < 4; c++) {
			posi.clear();
			for (int i = 0; i < 2; i++) {
				if (m.at(i).at(c) != 0) {
					posi.push_back(pair<int, int>(i, c));
				}
			}
			if (posi.size() == 2) term_list.push_back(Implicant(blockName(posi), var));
		}

		break;

	case 2:
		m.resize(2);
		for (int i = 0; i < 2; i++) {
			m.at(i).resize(2);
		}
		if (care_value.at(0)) m[0][0] = 1; else if (dontCare_value.at(0)) m[0][0] = -1; else m[0][0] = 0;	// (0,0) = 0
		if (care_value.at(2)) m[0][1] = 1; else if (dontCare_value.at(2)) m[0][1] = -1; else m[0][1] = 0;	// (0,1) = 2
		if (care_value.at(1)) m[1][0] = 1; else if (dontCare_value.at(1)) m[1][0] = -1; else m[1][0] = 0;	// (1,0) = 1
		if (care_value.at(3)) m[1][1] = 1; else if (dontCare_value.at(3)) m[1][1] = -1; else m[1][1] = 0;	// (1,1) = 3

		// scan 1x2 //
		for (int r = 0; r < 2; r++) {
			posi.clear();
			for (int j = 0; j < 2; j++) {
				if (m.at(r).at(j) != 0) {
					posi.push_back(pair<int, int>(r, j));
				}
			}
			if (posi.size() == 2) term_list.push_back(Implicant(blockName(posi), var));
		}

		// scan 2x1 //
		for (int c = 0; c < 2; c++) {
			posi.clear();
			for (int i = 0; i < 2; i++) {
				if (m.at(i).at(c) != 0) {
					posi.push_back(pair<int, int>(i, c));
				}
			}
			if (posi.size() == 2) term_list.push_back(Implicant(blockName(posi), var));
		}

		break;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	vector<set<int>> Map(pow(2, var));	// remember the index of terms in vector<Implicant> term_list which contains the number
	vector<bool> put(pow(2, var), false);	// care_value
	set<int> temp_dec;
	vector<bool> put_implicant(term_list.size(), false);	// implicant in term_list

	// initialize Map //
	for (int index = 0; index < term_list.size(); index++) {
		temp_dec = term_list.at(index).getDecimal();

		for (set<int>::iterator it = temp_dec.begin(); it != temp_dec.end(); it++) {
			Map.at(*it).insert(index);
		}

	}

	// find essential prime implicants //
	bool found;
	for (int num = 0; num < pow(2, var); num++) {
		if (!care_value.at(num)) {
			continue;
		}

		if (Map.at(num).size() == 1) {
			found = false;
			for (int i = 0; i < prime.size(); i++) {
				if (prime.at(i) == term_list.at(*(Map.at(num).begin()))) {
					found = true;
					break;
				}
			}
			if (!found) {
				prime.push_back(term_list.at(*(Map.at(num).begin())));
				put_implicant.at(*(Map.at(num).begin())) = true;

				temp_dec = term_list.at(*(Map.at(num).begin())).getDecimal();

				for (set<int>::iterator it = temp_dec.begin(); it != temp_dec.end(); it++) {
					put.at(*it) = true;
				}
			}
			put.at(num) = true;

		}
	}

	// the number that no implicant covered //
	for (int i = 0; i < pow(2, var); i++) {
		if (!care_value.at(i)) {
			continue;
		}
		if (Map.at(i).size() == 0) {
			prime.push_back(Implicant(i, var));
			put.at(i) = true;
		}
	}

	// deal with prime but not essential implicants //
	vector<Implicant> shortestChosen;
	vector<Implicant> result;
	vector<Implicant> emp;

	for (int i = 0; i < term_list.size(); i++) {
		result = bestPrime(term_list, emp, put, i);
		if (shortestChosen.size() == 0 && result.size() != 0) {
			shortestChosen = result;
		}
		else if (result.size() != 0 && result.size() < shortestChosen.size()) {
			shortestChosen = result;
		}
	}

	for (int i = 0; i < shortestChosen.size(); i++) {
		prime.push_back(shortestChosen.at(i));
	}

}

vector<bool> Kmap::getCareValue()
{
	return care_value;
}

vector<bool> Kmap::getDontCareValue()
{
	return dontCare_value;
}

vector<Implicant> Kmap::getPrime()
{
	return prime;
}

char Kmap::getChar(int value)
{
	if (care_value.at(value)) {
		return '1';
	}
	else if (dontCare_value.at(value)) {
		return 'x';
	}
	else {
		return '0';
	}
}

ostream& operator<<(ostream& output, Kmap& a)
{
	switch (a.var) {
	case 4:
		output << "=========Kmap=========" << endl;
		output << "  \\AB|               |" << endl;
		output << "CD \\ | 00  01  11  10|" << endl;
		output << "-----+---+---+---+---+" << endl;
		output << "  00 | " << a.getChar(0) << " | " << a.getChar(4) << " | " << a.getChar(12) << " | " << a.getChar(8) << " |" << endl;
		output << "  01 | " << a.getChar(1) << " | " << a.getChar(5) << " | " << a.getChar(13) << " | " << a.getChar(9) << " |" << endl;
		output << "  11 | " << a.getChar(3) << " | " << a.getChar(7) << " | " << a.getChar(15) << " | " << a.getChar(11) << " |" << endl;
		output << "  10 | " << a.getChar(2) << " | " << a.getChar(6) << " | " << a.getChar(14) << " | " << a.getChar(10) << " |" << endl;
		output << "-----+---+---+---+---+" << endl;

		break;

	case 3:
		output << "=========Kmap=========" << endl;
		output << "  \\AB|               |" << endl;
		output << " C \\ | 00  01  11  10|" << endl;
		output << "-----+---+---+---+---+" << endl;
		output << "   0 | " << a.getChar(0) << " | " << a.getChar(2) << " | " << a.getChar(6) << " | " << a.getChar(4) << " |" << endl;
		output << "   1 | " << a.getChar(1) << " | " << a.getChar(3) << " | " << a.getChar(7) << " | " << a.getChar(5) << " |" << endl;
		output << "-----+---+---+---+---+" << endl;

		break;

	case 2:
		output << "=====Kmap=====" << endl;
		output << "  \\ A|       |" << endl;
		output << " B \\ | 0   1 |" << endl;
		output << "-----+---+---+" << endl;
		output << "   0 | " << a.getChar(0) << " | " << a.getChar(2) << " |" << endl;
		output << "   1 | " << a.getChar(1) << " | " << a.getChar(3) << " |" << endl;
		output << "-----+---+---+" << endl;

		break;
	}

	return output;
}

ostream& operator<<(ostream& output, vector<Implicant> a)
{
	set<int> temp;
	for (int i = 0; i < a.size(); i++) {
		temp = a.at(i).getDecimal();
		output << "group " << i + 1 << ": " << *(temp.begin());
		for (set<int>::iterator it = ++temp.begin(); it != temp.end(); it++) {
			output << ", " << *it;
		}
		output << endl;

		output << "simplification of group " << i + 1 << " -> ";
		if (a.size() == 1 && a.at(0).all()) {
			output << '1' << endl;
		}
		else {
			output << a.at(i) << endl;
		}

	}

	output << endl;

	switch (a.at(0).getVariables()) {
	case 4:
		output << "F(A,B,C,D) = ";
		break;
	case 3:
		output << "F(A,B,C) = ";
		break;
	case 2:
		output << "F(A,B) = ";
		break;
	}

	if (a.size() == 1 && a.at(0).all()) {
		output << '1';
	}
	else {
		for (int i = 0; i < a.size() - 1; i++) {
			output << a.at(i) << '+';
		}
		output << a.at(a.size() - 1);
	}
	return output;
}


