#pragma once

#ifdef _WIN32
#ifdef GP_EXPORT
#define GP_API __declspec(dllexport)
#else
#define GP_API __declspec(dllimport)
#endif
#else
#error Grape only supports windows as of now!
#endif