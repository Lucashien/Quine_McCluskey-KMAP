// Quine_McCluskey.h

#ifndef QUINE_MCCLUSKEY
#define QUINE_MCCLUSKEY

#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Quine_McCluskey
{
	friend bool sort_by_num_of_1(const string, const string);
	friend bool sort_by_num_of_literals(const string, const string);

private:
	int Num_Var;
	vector<int> On_Set;
	vector<int> DC_Set;
	vector<string> s_On_Set;
	vector<string> Impli;
	vector<string> Prime_Impli;
	vector<string> Prime_dontcare;
	vector<string> Dontcare_Impli;
	vector<vector<int>> Prime_Impli_pair;
	vector<bool> essential_flag;


	void Init_Impli();
	string Dec_to_Bin(int);
	void Generate_Prime_Impli();
	void Generate_min_dontcare();
	void Erase_min_dontcare();
	string Merge(string, string);
	bool isGreyCode(string, string);

public:
	Quine_McCluskey();
	~Quine_McCluskey();

	// ----方便設定用----
	void Set_Num_Var(int);
	void Add_On_Set(int);
	void Add_DC_Set(int);
	//--------------------

	vector<string> Solve(); // return prime implicants

	int &Get_Vars();
	vector<string> &Get_On_Set();
	vector<string> &Get_Prime_Impli();
	vector<int> &Get_DC_Set();
	vector<int> &Get_On_Set_digit();

	void Print_Impli();
	void Print_Prime_Impli();
	void Find_Prime_Impli_Pair();
	void Gernerate_essential();
	void Print_Essential_Prime_Impli();
	string binaryToAlphabet(string a);
};

#endif
