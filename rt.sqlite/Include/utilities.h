#pragma once

#ifdef DLL_EXPORT
#define RT_SQLITE_API __declspec(dllexport) 
#else
#define RT_SQLITE_API __declspec(dllimport) 
#endif

namespace rt
{
	namespace sqlite
	{
		template<class T>
		class type_display;
	}
}