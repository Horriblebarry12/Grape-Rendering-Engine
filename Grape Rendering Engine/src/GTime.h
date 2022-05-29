#pragma once
#include "GrapePCH.h"

class GP_API GTime
{
public:

	GTime();

	static void Init(double time)
	{
		AppTime = time;
	}

	static void Tick(double time)
	{
		DeltaTime = time - AppTime;
		AppTime = time;
	}

	static double AppTime;
	static double DeltaTime;
private:
};