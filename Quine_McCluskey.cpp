// Quine_McCluskey.cpp

#include "Quine_McCluskey.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <set>

using namespace std;

/* -------------------- FRIEND -------------------- */
// 可存取放在private中的function跟varible

// 以 1 的個數分類
bool sort_by_num_of_1(const string a, const string b)
{
	int count_a = 0;
	int count_b = 0;

	// 計算有多少個1
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i] == '1')
			count_a++;
		if (b[i] == '1')
			count_b++;
	}
	return (count_a < count_b);
}

// 以 "-" 的個數分類
bool sort_by_num_of_literal(const string a, const string b)
{
	int count_a = 0, count_b = 0, size = a.length();
	for (int i = 0; i < size; i++)
	{
		if (a[i] != '-')
			count_a++;
		if (b[i] != '-')
			count_b++;
	}
	return (count_a < count_b);
}

/* -------------------- PUBLIC -------------------- */
Quine_McCluskey::Quine_McCluskey() {}

Quine_McCluskey::~Quine_McCluskey() {}

// 設定Varible的數量
void Quine_McCluskey::Set_Num_Var(int n) { Num_Var = n; }

void Quine_McCluskey::Add_On_Set(int n) { On_Set.push_back(n); }

void Quine_McCluskey::Add_DC_Set(int n) { DC_Set.push_back(n); }

// Retrun a Prime implicant
vector<string> Quine_McCluskey::Solve()
{
	Init_Impli();
	sort(Impli.begin(), Impli.end());
	sort(Impli.begin(), Impli.end(), sort_by_num_of_1);

	int round = 0;
	while (!Dontcare_Impli.empty()) //產生最簡的dontcare項，以免也把他算進prime implicants
		Generate_min_dontcare();

	while (!Impli.empty())
		Generate_Prime_Impli();

	Erase_min_dontcare();
	Gernerate_essential();
	return Prime_Impli;
}

int &Quine_McCluskey::Get_Vars() { return Num_Var; }

// Check on_set
vector<string> &Quine_McCluskey::Get_On_Set() { return s_On_Set; }
vector<int> &Quine_McCluskey::Get_DC_Set() { return DC_Set; }
vector<int> &Quine_McCluskey::Get_On_Set_digit() { return On_Set; }

// Check prime implicants
vector<string> &Quine_McCluskey::Get_Prime_Impli() { return Prime_Impli; }

/* -------------------- PRIVATE -------------------- */
void Quine_McCluskey::Generate_Prime_Impli()
{
	vector<bool> flag;
	for (int i = 0; i < Impli.size(); i++)
		flag.push_back(false);

	string s;
	vector<string> temp;

	// 比對兩 Implicant 是否有 greycode
	for (int i = 0; i < Impli.size(); i++)
	{
		for (int j = i + 1; j < Impli.size(); j++)
		{
			if (isGreyCode(Impli[i], Impli[j]))
			{
				string s = Merge(Impli[i], Impli[j]);
				if (find(temp.begin(), temp.end(), s) == temp.end())
					temp.push_back(s);

				// 標記 implicant 是否已經過融合
				flag[i] = true;
				flag[j] = true;
			}
		}
		// 將剩餘的 implicants 加進 prime implicant
		if (flag[i] == false)
		{
			// cout << "left:" << Impli[i] << endl;
			Prime_Impli.push_back(Impli[i]);
		}
	}

	// 清空(融合的融合，剩下的也加進prime implicants內了，此implicant內部只剩下flag = true的東西)
	Impli.clear();

	// 只要 Impli 不是 empty 的, 此 function 就會持續執行
	for (int i = 0; i < temp.size(); i++)
		Impli.push_back(temp[i]);
}

void Quine_McCluskey::Generate_min_dontcare()
{
	vector<bool> flag;
	for (int i = 0; i < Dontcare_Impli.size(); i++)
		flag.push_back(false);

	string s;
	vector<string> temp;

	// 比對兩 Implicant 是否有 greycode
	for (int i = 0; i < Dontcare_Impli.size(); i++)
	{
		for (int j = i + 1; j < Dontcare_Impli.size(); j++)
		{
			if (isGreyCode(Dontcare_Impli[i], Dontcare_Impli[j]))
			{
				/*cout << "check:" << endl;
				cout << Dontcare_Impli[i] << endl
					 << Dontcare_Impli[j] << endl;*/
				string s = Merge(Dontcare_Impli[i], Dontcare_Impli[j]);
				if (find(temp.begin(), temp.end(), s) == temp.end())
					temp.push_back(s);
				// cout << s << endl;
				// 標記 implicant 是否已經過融合
				flag[i] = true;
				flag[j] = true;
			}
		}

		// 將剩餘的 implicants 加進 prime implicant
		if (flag[i] == false)
		{
			// cout << "Dontcare_Impli:" << Dontcare_Impli[i] << endl;
			Prime_dontcare.push_back(Dontcare_Impli[i]);
		}
	}

	// 清空(融合的融合，剩下的也加進prime implicants內了，此implicant內部只剩下flag = true的東西)
	Dontcare_Impli.clear();

	// 只要 Impli 不是 empty 的, 此 function 就會持續執行
	for (int i = 0; i < temp.size(); i++)
		Dontcare_Impli.push_back(temp[i]);
}

void Quine_McCluskey::Erase_min_dontcare()
{
	for (int i = 0; i < Prime_Impli.size(); i++)
		for (int k = 0; k < Prime_dontcare.size(); k++)
		{
			if (Prime_Impli[i].compare(Prime_dontcare[k]) == 0)
				auto iter = Prime_Impli.erase(Prime_Impli.begin() + i);
		}
}
// 經 greycode 比較後使用次函式
// 結合兩 implicants
string Quine_McCluskey::Merge(string a, string b)
{
	for (int i = 0; i < a.length(); i++)
	{
		// a != b 回傳 "-"
		if (a[i] != b[i])
		{
			a[i] = '-';
			return a;
		}
	}
	return EXIT_SUCCESS;
}

// 檢查兩 implicants 是否可以合併
// 兩 implicants 是否只有一個 bit 不同，即是否為 grey code
// 0110
// 0100 即為 greycode
bool Quine_McCluskey::isGreyCode(string a, string b)
{
	int flag = 0;
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i] != b[i])
			flag++;
	}
	return (flag == 1);
}

// 初始化 implicant
void Quine_McCluskey::Init_Impli()
{
	// on set
	for (int i = 0; i < On_Set.size(); i++)
	{
		// 將轉換過的 on_set 加入 implicant
		Impli.push_back(Dec_to_Bin(On_Set[i]));
		s_On_Set.push_back(Impli.back());
	}

	// don't care set
	for (int i = 0; i < DC_Set.size(); i++)
	{
		Impli.push_back(Dec_to_Bin(DC_Set[i]));
		Dontcare_Impli.push_back(Dec_to_Bin(DC_Set[i]));
	}
}

// 10進位轉2進位
// e.g., 5 -> 101
//       8 -> 1000
string Quine_McCluskey::Dec_to_Bin(int n)
{
	string s;
	while (n != 0)
	{
		s += '0' + n % 2;
		n /= 2;
	}
	int pad = Num_Var - s.length();
	while (pad--)
		s += '0';
	reverse(s.begin(), s.end());
	return s;
}

void Quine_McCluskey::Find_Prime_Impli_Pair()
{

	for (int i = 0; i < Prime_Impli.size(); i++)
	{
		int Mask = 0;
		int Varification = 0;
		vector<int> temp;
		reverse(Prime_Impli[i].begin(), Prime_Impli[i].end());

		// 取得 Prime implicants[i] 的 Mask 跟 Varification
		for (int j = 0; j < Prime_Impli[i].size(); j++)
		{
			if (Prime_Impli[i][j] != '-')
				Mask += pow(2, j);

			if (Prime_Impli[i][j] == '1')
				Varification += pow(2, j);
		}
		for (unsigned int k = 0; k < pow(2, Num_Var); k++)
			if ((k & Mask) == Varification)
				temp.push_back(k);
		Prime_Impli_pair.push_back(temp);
		reverse(Prime_Impli[i].begin(), Prime_Impli[i].end());
	}
}

void Quine_McCluskey::Gernerate_essential()
{
	Find_Prime_Impli_Pair();
	vector<int> temp;			   //裝重複項，跟 re 做操作
	vector<vector<int>> copy_pair; //不直接對Prime Implicant 內的 element 做操作
	set<int> re;				   // 裝重複的minterm
	set<int>::iterator iter = re.begin();

	copy_pair.assign(Prime_Impli_pair.begin(), Prime_Impli_pair.end());

	for (int i = 0; i < copy_pair.size(); i++) // 有幾組prime implicants minterm
		for (int j = 0; j < copy_pair[i].size(); j++)
			temp.push_back(copy_pair[i][j]);
	sort(temp.begin(), temp.end());

	// 取得重複元素
	for (int i = 0; i < temp.size(); i++)
		if (temp[i] == temp[i + 1])
			re.insert(temp[i]);

	for (int i = 0; i < copy_pair.size(); i++) // 有幾組prime implicants minterm
		for (int j = 0; j < copy_pair[i].size(); j++)
		{
			// 跟重複向比對
			for (iter = re.begin(); iter != re.end(); iter++)
				if (copy_pair[i][j] == *iter)
					copy_pair[i].erase(copy_pair[i].begin() + j);

			// 跟dontcare項比對
			for (int k = 0; k < DC_Set.size(); k++)
				if (copy_pair[i][j] == DC_Set[k])
					copy_pair[i].erase(copy_pair[i].begin() + j);
		}

	// 標記 essential
	for (int i = 0; i < copy_pair.size(); i++)
	{
		if (!copy_pair[i].empty())
			essential_flag.push_back(true);
		else
			essential_flag.push_back(false);
	}
}

/* -------------------- PUBLIC/Print -------------------- */
void Quine_McCluskey::Print_Impli()
{
	int size = Impli.size();
	for (int i = 0; i < size; i++)
		cout << Impli[i] << endl;
}

void Quine_McCluskey::Print_Prime_Impli()
{
	ofstream ofs;
	ofs.open("output.txt", std::ios_base::app);

	int size = Prime_Impli.size();
	ofs << "Prime Implicant = ";
	vector<string> temp;
	for (int i = 0; i < size; i++)
	{
		temp.push_back(binaryToAlphabet(Prime_Impli[i]));
		temp.push_back(",");
	}

	temp.pop_back();
	for (int i = 0; i < temp.size(); i++)
		ofs << temp[i];
	ofs << endl;
}

void Quine_McCluskey::Print_Essential_Prime_Impli()
{
	ofstream ofs;
	ofs.open("output.txt", std::ios_base::app);

	int size = Prime_Impli.size();
	ofs << "Essential Prime Implicant = ";
	vector<string> temp;
	for (int i = 0; i < size; i++)
	{
		if (essential_flag[i])
		{
			temp.push_back(binaryToAlphabet(Prime_Impli[i]));
			temp.push_back(",");
		}
	}

	temp.pop_back();
	for (int i = 0; i < temp.size(); i++)
		ofs << temp[i];
	ofs << endl;
}

string Quine_McCluskey::binaryToAlphabet(string a)
{
	vector<string> vars;
	string letters[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "str", "t", "u", "v", "w", "x", "y", "z"};
	for (int i = 0; i < Num_Var; i++)
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