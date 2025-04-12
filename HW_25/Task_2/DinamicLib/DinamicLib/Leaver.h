#pragma once

#ifdef LIB_DYNAMIC_EXPORTS
#define LIB_DYNAMIC_API __declspec(dllexport)
#else
#define LIB_DYNAMIC_API __declspec(dllimport)
#endif

class Leaver
{
public:
	LIB_DYNAMIC_API std::string leave(std::string str);
};