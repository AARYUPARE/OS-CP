#pragma once

enum Error
{
	None = 0,
	Opcode,
	Oprand,
	PageFault,
	TLE, // Time limit Exceeded
	LLE // Line limit Exceeded
};