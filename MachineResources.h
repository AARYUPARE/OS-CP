#pragma once
#include<vector>
using namespace std;

class Resources
{
public:
	vector<char> R;
	vector<char> IR;
	int IC;
	bool C;
	int SI;
	int PTR;
	int PI;
	int TI;
	vector<vector<char>> M;

	void Clear()
	{
		for (char& ch : R)
		{
			ch = ' ';
		}
		
		for (char& ch : IR)
		{
			ch = ' ';
		}

		IC = 0;
		C = false;
		SI = 0;
		PTR = -1;
		PI = 0;
		TI = 0;

		for (vector<char>& v : M)
		{
			for (char& ch : v)
			{
				ch = ' ';
			}
		}
	}
};