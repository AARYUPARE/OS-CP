#pragma once
#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include"MachineResources.h"
#include"PagingManager.h"
using namespace std;

class MOS
{
	string OUTPUT_FILE;
	PagingManager pm;
	int LLC;

	void GD(fstream& fin, Resources& rs, int address)
	{
		string data;
		getline(fin, data);		

		int RA = pm.GetRealAddress(rs, address);

		if (rs.PI == 3)
		{
			int frame = pm.GetRandomFrame(rs, address);
			RA = pm.GetRealAddress(rs, address);
			rs.PI = 0;
		}

		int i = 0;
		int k = 0;

		for (char ch : data)
		{
			rs.M[RA + k][i] = ch;

			i++;

			if (i == 4)
			{
				k++;
				i = 0;
			}

			if (k == 10)
			{
				address += 10;
				int frame = pm.GetRandomFrame(rs, address);
				RA = frame * 10;
				k = 0;
			}
		}
	}

	void PD(Resources& rs, int address, int TLL)
	{
		LLC++;
		if (LLC > TLL)
		{
			cout << "Total line limit exceeded" << endl;
			exit(0);
		}

		fstream fout;
		fout.open(OUTPUT_FILE, ios::app);

		int RA = pm.GetRealAddress(rs, address);

		if (rs.PI == 3)
		{
			cout << "Program terminated due to runtime exception: Page is not present" << endl;
			rs.PI = 0;
			exit(0);
		}

		for(int j = RA; j < RA + 10; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				if (!rs.M[j][i]) continue;

				fout << rs.M[j][i];
			}
		}

		fout << "\n";

		fout.close();
	}

	void H(Resources & rs)
	{
		fstream fout;
		fout.open(OUTPUT_FILE, ios::app);

		fout << "\n\n";

		fout.close();
		cout << "Memory :" << endl;
		PrintMemory(rs);
	}
public:

	MOS(string OUTPUT_FILE = "Output.txt")
	{
		this->OUTPUT_FILE = OUTPUT_FILE;
		LLC = 0;
	}

	void Clean()
	{
		LLC = 0;
	}

	void mos(fstream& fin, Resources& rs, int address, int TLL, string opcode = "", int VA = -1)
	{
		if (rs.PI == 1)
		{
			cout << "Incorrect Opcode" << endl;
			exit(0);
		}
		else if (rs.PI == 2)
		{
			cout << "Incorrect Operand" << endl;
			exit(0);
		}
		else if (rs.PI == 3)
		{
			if (opcode == "SR")
			{
				int frame = pm.GetRandomFrame(rs, VA);
				rs.PI = 0;
				return;  
			}

			cout << "Program terminated due to runtime exception: Page is not present" << endl;
			exit(0);
		}
		else if (rs.TI == 2)
		{
			cout << "Total time limit exceeded" << endl;
			exit(0);
		}
		
		if (rs.SI == 1)
		{
			GD(fin, rs, address);
		}
		else if (rs.SI == 2)
		{
			PD(rs, address, TLL);
		}
		else if (rs.SI == 3)
		{
			H(rs);
		}

		rs.SI = 0;
	}

	void PrintMemory(Resources& rs)
	{
		cout << setw(10) << "  Location  " << setw(20) << "    Memory Content    " << endl;
		cout << setw(10) << " ---------- " << setw(20) << " -------------------  " << endl;

		for (int i = 0; i < 300; i++)
		{
			cout << setw(10) << i << "  ";

			for (int j = 0; j < 4; j++)
			{
				cout << "  " << (rs.M[i][j] ? rs.M[i][j] : ' ') << "  ";
			}

			cout << endl;
		}
	}
};

