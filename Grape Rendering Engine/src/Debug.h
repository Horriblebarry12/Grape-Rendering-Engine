#pragma once
#include <string>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) Debug::GLClearError();\
	x;\
ASSERT(Debug::GLLogCall(#x,__FILE__, __LINE__));


class Debug
{
public:
	static void GLClearError();

	static bool GLLogCall(const char* funtion, const char* file, int line);

	//TODO: implment logging api
	static void LogInfo(std::string msg);
};

