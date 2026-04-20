#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<iomanip>
#include"MachineResources.h"
#include"MOS.h"
#include"PagingManager.h"
using namespace std;

class Executor
{	
	MOS mos;
	PagingManager pm;
	
	void LoadIR(vector<char>& IR, vector<vector<char>>& M, int RA)
	{
		if (RA < 0)
		{
			return;
		}

		for (int i = 0; i < 4; i++)
		{
			IR[i] = M[RA][i];
		}
	}

	void LR(fstream& fin, Resources& rs, int address)
	{
		int RA = pm.GetRealAddress(rs, address);

		if (rs.PI == 3)
		{
			return;
		}

		for (int i = 0; i < 4; i++)
		{
			rs.R[i] = rs.M[RA][i];
		}
	}


	void SR(Resources& rs, int address)
	{
		int RA = pm.GetRealAddress(rs, address);

		if (rs.PI == 3)
		{
			return;
		}

		for (int i = 0; i < 4; i++)
		{
			rs.M[RA][i] = rs.R[i];
		}
	}

	void CR(Resources& rs, int address)
	{
		int RA = pm.GetRealAddress(rs, address);

		if (rs.PI == 3)
		{
			return;
		}

		rs.C = true;
		for (int i = 0; i < 4 && rs.C; i++)
		{
			rs.C = rs.R[i] == rs.M[RA][i];
		}
	}

	void BT(Resources& rs, int address)
	{
		rs.IC = rs.C ? address : rs.IC + 1;
	}

public:

	Executor()
	{
	}

	string ExtractOpcode(vector<char>& IR, Resources& rs)
	{
		string opcode;

		if (IR[0] != ' ')
			opcode += IR[0];
		if (IR[1] != ' ')
			opcode += IR[1];

		return opcode;
	}


	int ExtractAddress(vector<char>& IR, Resources& rs, string& opcode)
	{
		if (opcode == "H")
		{
			return -1;
		}

		char dig1 = IR[2];
		char dig2 = IR[3];

		if (dig1 < '0' || dig1 > '9' || dig2 < '0' || dig2 > '9')
		{
			rs.PI = 2;
			return -1;
		}

		return  (dig1 - '0') * 10 + (dig2 - '0');
	}	

	void executeProgram(fstream& fin, Resources& rs, int id, int TTL, int TLL)
	{
		mos.Clean();
		int& IC = rs.IC;
		vector<char>& IR = rs.IR;

		int TTC = 0;
		bool isHaulted = false;

		while (!isHaulted)
		{
			int RA = pm.GetRealAddress(rs, IC);

			if (rs.PI != 0)
			{
				mos.mos(fin, rs, IC, TLL);
			}

			LoadIR(IR, rs.M, RA);
			TTC++;
			
			string opcode = ExtractOpcode(IR, rs);	
			int address = ExtractAddress(IR, rs, opcode);

			if (rs.PI != 0)
			{
				mos.mos(fin, rs, address, TLL);
			}

			cout << "IC: " << setw(2) << IC  <<" IR: " << opcode << IR[2] << IR[3] << endl;

			if (TTC == TTL)
			{
				rs.TI = 2;
			}

			if (opcode == "GD")
			{
				rs.SI = 1;			
			}
			else if (opcode == "PD")
			{
				rs.SI = 2;			
			}
			else if (opcode == "H")
			{
				rs.SI = 3;
				isHaulted = true;
			}
			else if (opcode == "LR")
			{
				LR(fin, rs, address);
			}
			else if (opcode == "SR")
			{
				SR(rs, address);
			}
			else if (opcode == "CR")
			{
				CR(rs, address);
			}
			else if (opcode == "BT")
			{
				BT(rs, address);
			}
			else
			{
				rs.PI = 1;
			}

			if (!(rs.PI == 3) && opcode != "BT")
			{
				IC++;
			}

			if (rs.PI || rs.SI || rs.TI)
			{
				mos.mos(fin, rs, address, TLL, opcode, address);
			}

			IR.assign(4, ' ');
		}
	}
};