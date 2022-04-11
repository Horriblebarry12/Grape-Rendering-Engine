#pragma once
class Time
{
public:

	static void Init(float time)
	{
		ApplicationTime = time;
		AppStartTime = time;
	}

	static void TickOneFrame(float Time);

	static float DeltaTime;
	static float ApplicationTime;
	static float AppStartTime;
	static float Framerate;


};

