#include "Time.h"

void Time::TickOneFrame(float Time)
{
	DeltaTime = Time - ApplicationTime;

	ApplicationTime = Time;

	Framerate = 1 / DeltaTime;
}
