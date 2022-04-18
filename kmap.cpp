// main.cpp
#include "Petrick_Method.h"
#include "Quine_McCluskey.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
using namespace std;

void loadinginput(ifstream &ifs, Quine_McCluskey &QM);
void kmap(int varible_num, vector<int> m, vector<int> d);

int main()
{
	ifstream fin("input.txt");
	// ofstream fout("output.txt");

	// cout << "\n***** Quine-McCluskey Algorithm *****" << endl;
	Quine_McCluskey QM;
	loadinginput(fin, QM);
	kmap(QM.Get_Vars(), QM.Get_On_Set_digit(), QM.Get_DC_Set());
	QM.Solve();
	QM.Print_Prime_Impli();
	QM.Print_Essential_Prime_Impli();

	Petrick_Method PM(QM.Get_Prime_Impli(), QM.Get_On_Set());
	PM.Solve();
	PM.output_SOP(QM.Get_Vars());

	fin.close();
	// fout.close();
	cout << "Successfully. Please check the output.txt" << endl;
	return 0;
}

void loadinginput(ifstream &ifs, Quine_McCluskey &QM)
{

	string str;
	char last; //判斷上一個str是哪個(v,m,d)
	while (ifs >> str)
	{
		istringstream f(str);
		const string delimiter = ",";
		size_t pos;
		switch (last)
		{
		case 'v':
			while (getline(f, str, ','))
				QM.Set_Num_Var(stoi(str));
			break;
		case 'm':
			while (getline(f, str, ','))
				QM.Add_On_Set(stoi(str));
			break;
		case 'd':
			while (getline(f, str, ','))
				QM.Add_DC_Set(stoi(str));
			break;
		}
		last = str[0];
	}
}

void kmap(int varible_num, vector<int> m, vector<int> d)
{
	ofstream ofs;
	ofs.open("output.txt");
	string show[20] = {"0"};
	for (int i = 0; i < 20; i++)
		show[i] = "0";

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < m.size(); j++)
			if (m[j] == i)
				show[i] = "1";
		for (int j = 0; j < d.size(); j++)
			if (d[j] == i)
				show[i] = "x";
	}
	/*
		switch (varible_num)
		{
		case 2:
			cout << "  \\ A|       |" << endl;
			cout << " B \\ |  0   1|" << endl;
			cout << "-----+---+---+ " << endl;
			cout << "    0|  " << show[0] << "|  " << show[2] << "|" << endl;
			cout << "-----+---+---+ " << endl;
			cout << "    1|  " << show[1] << "|  " << show[3] << "|" << endl;
			cout << "-----+---+---+ " << endl;
			break;
		case 3:
			cout << "  \\AB|               |" << endl;
			cout << " C \\ | 00  01  11  10|" << endl;
			cout << "-----+---+---+---+---+ " << endl;
			cout << "    0|  " << show[0] << "|  " << show[2] << "|  " << show[6] << "|  " << show[4] << "|" << endl;
			cout << "-----+---+---+---+---+ " << endl;
			cout << "    1|  " << show[1] << "|  " << show[3] << "|  " << show[7] << "|  " << show[5] << "|" << endl;
			cout << "-----+---+---+---+---+ " << endl;

			break;
		case 4:
			cout << "  \\AB|               |" << endl;
			cout << "CD \\ | 00  01  11  10|" << endl;
			cout << "-----+---+---+---+---+ " << endl;
			cout << "   00|  " << show[0] << "|  " << show[4] << "|  " << show[12] << "|  " << show[8] << "|" << endl;
			cout << "-----+---+---+---+---+ " << endl;
			cout << "   01|  " << show[1] << "|  " << show[5] << "|  " << show[13] << "|  " << show[9] << "|" << endl;
			cout << "-----+---+---+---+---+ " << endl;
			cout << "   11|  " << show[3] << "|  " << show[7] << "|  " << show[15] << "|  " << show[11] << "|" << endl;
			cout << "-----+---+---+---+---+ " << endl;
			cout << "   10|  " << show[2] << "|  " << show[6] << "|  " << show[14] << "|  " << show[10] << "|" << endl;
			cout << "-----+---+---+---+---+ " << endl;
			break;
		}
	*/

	switch (varible_num)
	{
	case 2:
		ofs << "  \\ A|       |" << endl;
		ofs << " B \\ |  0   1|" << endl;
		ofs << "-----+---+---+ " << endl;
		ofs << "    0|  " << show[0] << "|  " << show[2] << "|" << endl;
		ofs << "-----+---+---+ " << endl;
		ofs << "    1|  " << show[1] << "|  " << show[3] << "|" << endl;
		ofs << "-----+---+---+ " << endl;
		break;
	case 3:
		ofs << "  \\AB|               |" << endl;
		ofs << " C \\ | 00  01  11  10|" << endl;
		ofs << "-----+---+---+---+---+ " << endl;
		ofs << "    0|  " << show[0] << "|  " << show[2] << "|  " << show[6] << "|  " << show[4] << "|" << endl;
		ofs << "-----+---+---+---+---+ " << endl;
		ofs << "    1|  " << show[1] << "|  " << show[3] << "|  " << show[7] << "|  " << show[5] << "|" << endl;
		ofs << "-----+---+---+---+---+ " << endl;

		break;
	case 4:
		ofs << "  \\AB|               |" << endl;
		ofs << "CD \\ | 00  01  11  10|" << endl;
		ofs << "-----+---+---+---+---+ " << endl;
		ofs << "   00|  " << show[0] << "|  " << show[4] << "|  " << show[12] << "|  " << show[8] << "|" << endl;
		ofs << "-----+---+---+---+---+ " << endl;
		ofs << "   01|  " << show[1] << "|  " << show[5] << "|  " << show[13] << "|  " << show[9] << "|" << endl;
		ofs << "-----+---+---+---+---+ " << endl;
		ofs << "   11|  " << show[3] << "|  " << show[7] << "|  " << show[15] << "|  " << show[11] << "|" << endl;
		ofs << "-----+---+---+---+---+ " << endl;
		ofs << "   10|  " << show[2] << "|  " << show[6] << "|  " << show[14] << "|  " << show[10] << "|" << endl;
		ofs << "-----+---+---+---+---+ " << endl;
		break;
	}
	ofs.close();
}