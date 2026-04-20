#pragma once
#include<iostream>
#include <random>
#include"MachineResources.h"
using namespace std;

class PagingManager
{
	random_device rd;
	mt19937 gen;
	uniform_int_distribution<> dist;

	int GetPTEInt(int i, Resources& rs)
	{
		if (rs.M[i][3] == ' ') return -1;

		int pte = 0;
		int mul = 1;

		for (int j = 3; j >= 0; j--)
		{
			if (rs.M[i][j] == ' ') continue;

			pte += (rs.M[i][j] - '0') * mul;
			mul *= 10;
		}

		return pte;
	}

	bool IsFrameExist(int frame, Resources& rs)
	{
		for (int i = rs.PTR; i < rs.PTR + 10; i++)
		{
			if (GetPTEInt(i, rs) == frame) return true;
		}

		return false;
	}

	void PutPTE(int newFrame, Resources& rs, int VA)
	{
		int page = VA / 10;
		int pteIndex = rs.PTR + page;

		for (int j = 0; j < 4; j++)
			rs.M[pteIndex][j] = ' ';

		int temp = newFrame;
		int j = 3;

		while (temp && j >= 0)
		{
			rs.M[pteIndex][j] = temp % 10 + '0';
			temp /= 10;
			j--;
		}
	}

public:

	PagingManager() : gen(rd()), dist(0, 29) {}

	int InitPTR()
	{
		return dist(gen) * 10;
	}

	int GetRandomFrame(Resources& rs, int VA)
	{
		int frame = dist(gen);

		while (frame * 10 == rs.PTR || IsFrameExist(frame, rs))
		{
			frame = dist(gen);
		}

		PutPTE(frame, rs, VA);

		return frame;
	}

	int GetRealAddress(Resources& rs, int VA)
	{
		int pte = rs.PTR + VA / 10;
		int frame = GetPTEInt(pte, rs);

		if (frame == -1)
		{
			rs.PI = 3;
			return -1;
		}

		int RA =  frame * 10 + VA % 10;


		return RA;
	}
};