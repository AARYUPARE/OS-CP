#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iomanip>
#include"Loader.h"
#include"MachineResources.h"
#include"Executor.h"
using namespace std;



class VirtualMachine
{
	Resources rs;
	Loader loader;
	Executor ex;

	fstream fin;

public:

	VirtualMachine(int sizeR = 4, int sizeIR = 4, int valIC = 0, bool valC = false, int valSI = 0, int sizeMR = 300, int sizeMC = 4)
	{
		rs.R.resize(sizeR);
		rs.IR.resize(sizeIR);
		rs.IC = valIC;
		rs.C = valC;
		rs.SI = valSI;
		rs.PTR = 0;
		rs.PI = 0;
		rs.M.resize(sizeMR, vector<char>(sizeMC, ' '));

		fin.open("Input.txt", ios::in);
	}

	void Init()
	{
		cout << "Started" << endl;

		string buffer;
		int id, TTL, TLL;

		while (getline(fin, buffer))
		{
			if (buffer.substr(0, 4) == "$AMJ")
			{
				if (buffer.size() < 16)
				{
					cout << "Incorrect AMJ card" << endl;
					return;
				}

				string s = { buffer.begin() + 4, buffer.begin() + 8 };
				id = stoi(s);

				s = { buffer.begin() + 8, buffer.begin() + 12 };
				TTL = stoi(s);
				
				s = { buffer.begin() + 12, buffer.begin() + 16 };
				TLL = stoi(s);

				loader.Load(fin, rs);
			}
			else if (buffer.substr(0, 4) == "$DTA")
			{
				ex.executeProgram(fin, rs, id, TTL, TLL);
			}
			else if (buffer.substr(0, 4) == "$END")
			{
				cout << endl <<"Job with id: " << id << " is terminated\n\n" << endl;
			}
		}
	}

	

	~VirtualMachine()
	{
		fin.close();
	}
};
