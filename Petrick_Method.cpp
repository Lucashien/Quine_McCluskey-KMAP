// Petrick_Method.cpp
#include "Petrick_Method.h"
#include "Quine_McCluskey.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

set<string>::iterator iter;

/* -------------------- FRIEND -------------------- */
bool sort_by_SOP(const set<string> a, const set<string> b)
{
	return (a.size() < b.size());
}

/* -------------------- PUBLIC -------------------- */

// 傳入 prime implicants 與 onset, 檢查 prime implicant 是否有包含 onset
Petrick_Method::Petrick_Method(vector<string> &Prime_Impli, vector<string> &On_Set)
{
	for (int i = 0; i < On_Set.size(); i++)
	{
		vector<string> sum;
		for (int j = 0; j < Prime_Impli.size(); j++)
		{
			if (is_On_in_Prime_Impli(On_Set[i], Prime_Impli[j]))
				sum.push_back(Prime_Impli[j]);
		}
		POS.push_back(sum);
	}
}

Petrick_Method::~Petrick_Method() {}

set<string> Petrick_Method::Solve()
{
	POS2SOP();
	sort(SOP.begin(), SOP.end(), sort_by_SOP);
	// CountAllCost();
	return SOP[0];
}

/* -------------------- PRIVATE -------------------- */
void Petrick_Method::POS2SOP()
{
	set<string> product;
	Expansion(product, 0, POS.size());
}

// 展開 SOP
void Petrick_Method::Expansion(set<string> &product, int i_POS, int maxi)
{
	if (i_POS == maxi)
	{
		string s = set2string(product);
		
		// 以遞迴的方式查找end of SOP
		// 接著以 set 型態的 s_SOP 處理 X + X = X 的情況
		if (s_SOP.find(s) == s_SOP.end())
		{
			s_SOP.insert(s);
			SOP.push_back(product);
		}
	}

	// POS[i_POS] is a Product e.g., a'bc or b'cd
	// 將傳入的 product 進行比對
	// 用遞迴的方式展開此式
	for (int i = 0; i < POS[i_POS].size(); i++)
	{
		if (product.find(POS[i_POS][i]) == product.end())
		{
			product.insert(POS[i_POS][i]);
			Expansion(product, i_POS + 1, maxi);
			product.erase(POS[i_POS][i]);
		}
		else
			Expansion(product, i_POS + 1, maxi);
	}
}

// 將 set 轉成 string
string Petrick_Method::set2string(set<string> &temp)
{
	stringstream ss;
	for (iter = temp.begin(); iter != temp.end(); iter++)
		ss << *iter;
	string s;
	ss >> s;
	return s;
}

// 檢查 Prime implicant 是否涵蓋其他 Inplicant
// 涵蓋：true
bool Petrick_Method::is_On_in_Prime_Impli(string On, string Prime_Impli)
{
	for (int i = 0; i < On.size(); i++)
	{
		if (Prime_Impli[i] != '-' && Prime_Impli[i] != On[i])
			return false;
	}
	return true;
}

void Petrick_Method::CountAllCost()
{
	int min_cover = SOP[0].size();
	for (int i = 0; SOP[i].size() == min_cover; i++)
	{
		int cost = 0;
		for (iter = SOP[i].begin(); iter != SOP[i].end(); iter++)
			cost += CountCost(*iter);
		SOP_cost.push_back(cost);
	}
}

// 計算 "-" 數量
int Petrick_Method::CountCost(string impli)
{
	int size = impli.size();
	int cost = 0;
	for (int i = 0; i < size; i++)
		if (impli[i] != '-')
			cost++;

	return cost;
}

/* -------------------- PUBLIC/Print -------------------- */
string Petrick_Method::binaryToAlphabet(string a, int varible_num)
{
	vector<string> vars;
	string letters[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "str", "t", "u", "v", "w", "x", "y", "z"};
	for (int i = 0; i < varible_num; i++)
		vars.push_back(letters[i]);

	string temp = "";
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i] != '-')
		{
			if (a[i] == '0')
				temp = temp + vars[i] + "\'";
			else
				temp = temp + vars[i];
		}
	}
	return temp;
}

void Petrick_Method::Print_POS()
{
	cout << "Product of Sum = ";
	int size1 = POS.size();
	for (int i = 0; i < size1; i++)
	{
		cout << "( " << POS[i][0];
		int size2 = POS[i].size();
		for (int j = 1; j < size2; j++)
			cout << " + " << POS[i][j];
		cout << " )";
	}
	cout << endl;
}

void Petrick_Method::Print_SOP()
{
	cout << "Sum of Product = " << endl;
	int size = SOP.size();
	for (int i = 0; i < size; i++)
	{
		iter = SOP[i].begin();
		cout << "*(" << *iter << ")";
		for (iter++; iter != SOP[i].end(); iter++)
			cout << "(" << *iter << ")";
		cout << endl;
	}
	cout << endl;
}

void Petrick_Method::output_SOP(int varible_num)
{
	int i;
	int min_cover = SOP[0].size();
	string letters[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
	ofstream ofs;
	ofs.open("output.txt", std::ios_base::app);

	
	ofs << "The reduced SOP form:" << endl;
	for (i = 0; SOP[i].size() == min_cover; i++)
	{

		ofs << "Answer " << i + 1 << " : " << endl;

		// 生成 F(A,B,C,D)
		vector<string> vars;
		for (int i = 0; i < varible_num; i++)
		{
			vars.push_back(letters[i]);
			vars.push_back(",");
		}
		vars.pop_back();
		ofs << "F(";
		for (int i = 0; i < vars.size(); i++)
			ofs << vars[i];
		ofs << ") = ";

		vector<string> output;
		iter = SOP[i].begin();
		for (iter; iter != SOP[i].end(); iter++)
		{
			output.push_back(binaryToAlphabet(*iter, varible_num));
			output.push_back("+");
		}
		output.pop_back();
		for (int j = 0; j < output.size(); j++)
			ofs << output[j];
		ofs << endl;
	}

	// output到檔案

}

void Petrick_Method::Print_FewestTerm_P()
{
	int min_cover = SOP[0].size();
	for (int i = 0; SOP[i].size() == min_cover; i++)
	{
		cout << "Answer " << i + 1 << " : " << endl;
		iter = SOP[i].begin();
		cout << *iter;
		for (iter++; iter != SOP[i].end(); iter++)
			cout << "\n"
				 << *iter;
		cout << endl;
		// cout << "cost = " << SOP_cost[i] << endl;
	}
}
