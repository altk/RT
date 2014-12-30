#pragma once
#include "utilities.h"

namespace rt
{
	namespace sqlite
	{
		enum class RT_SQLITE_API database_open_mode
		{
			readonly,
			read_write,
			read_write_create
		};
	}
}