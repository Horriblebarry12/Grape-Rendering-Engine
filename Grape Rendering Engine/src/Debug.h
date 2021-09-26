#pragma once
#include <string>
#include "spdlog/spdlog.h"

// Debug
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) Debug::GLClearError();\
	x;\
ASSERT(Debug::GLLogCall(#x,__FILE__, __LINE__));

// Loging
#define LogInfo(msg, ...) spdlog::info(msg, __VA_ARGS__)
#define LogWarning(msg, ...) spdlog::warn(msg, __VA_ARGS__)
#define LogError(msg, ...) spdlog::error(msg, __VA_ARGS__)
#define LogCritical(msg, ...) spdlog::critical(msg, __VA_ARGS__)
#if _DEBUG
#define LogDebug(msg, ...) spdlog::debug(msg, __VA_ARGS__)
#elif
#define LogDebug(msg)
#endif
class Debug
{
public:
	static void GLClearError();

	static bool GLLogCall(const char* funtion, const char* file, int line);

};