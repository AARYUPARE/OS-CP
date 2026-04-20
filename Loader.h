#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include"MachineResources.h"
#include"PagingManager.h"
using namespace std;


class Loader {

	PagingManager pm;

public:
	void Load(fstream& fin, Resources& rs)
	{
		rs.Clear();
		rs.PTR = pm.InitPTR();

		string instr;

		int z = 0;
		int frame = pm.GetRandomFrame(rs, z);
		z += 10;

		int k = 0;
		int pos = 0;

		while (getline(fin, instr))
		{
			if (instr == "$AMJ")
			{
				continue;
			}
			else if (instr.size() >= 4 && instr.substr(0, 4) == "$DTA")
			{
				fin.seekg(pos);
				return;
			}

			if (k == 10)
			{
				frame = pm.GetRandomFrame(rs, z++);
				z += 10;
				k = 0;
			}

			for (int i = 0; i < instr.size(); )
			{
				if (instr[i] == 'H')
				{
					rs.M[frame * 10 + k][0] = instr[i];
					i++;
				}
				else
				{
					for (int j = i, x = 0; j < i + 4 && j < instr.size() && x < 4; j++, x++)
					{
						rs.M[frame * 10 + k][x] = instr[j];
					}
					i += 4;
				}

				k++;

				if (k == 10 && i < instr.size())
				{
					frame = pm.GetRandomFrame(rs, z++);
					z += 10;
					k = 0;
				}
			}

			pos = fin.tellg();
		}
	}
};
