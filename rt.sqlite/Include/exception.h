#pragma once
#include <exception>
#include <string>
#include "utilities.h"

namespace rt
{
	namespace sqlite
	{
		class RT_SQLITE_API sqlite_exception : public std::exception
		{
		public:
			sqlite_exception() throw(){}

			explicit sqlite_exception(const char* message) throw()
				: std::exception(message)
			{
				OutputDebugStringA(message);
				OutputDebugStringA("\n");
			}
		};

		class RT_SQLITE_API open_database_exception sealed : public sqlite_exception
		{
		public:
			explicit open_database_exception(const char*  message) throw()
				: sqlite_exception(message){}
		};

		class RT_SQLITE_API bind_parameter_exception sealed : public sqlite_exception
		{
		public:
			explicit bind_parameter_exception(const char*  message) throw()
				: sqlite_exception(message){}
		};

		class RT_SQLITE_API create_statement_exception sealed : public sqlite_exception
		{
		public:
			explicit create_statement_exception(const char*  message) throw()
				: sqlite_exception(message){}
		};

		class RT_SQLITE_API execute_statement_exception sealed : public sqlite_exception
		{
		public:
			explicit execute_statement_exception(const char*  message) throw()
				: sqlite_exception(message){}
		};

		class RT_SQLITE_API busy_exception sealed : public sqlite_exception
		{
		public:
			busy_exception() throw(){}
		};

		class RT_SQLITE_API step_statement_exception sealed : public sqlite_exception
		{
		public:
			explicit step_statement_exception(const char*  message) throw()
				: sqlite_exception(message){}
		};

		class RT_SQLITE_API reset_statement_exception sealed : public sqlite_exception
		{
		public:
			explicit reset_statement_exception(const char*  message) throw()
				: sqlite_exception(message){}
		};

		class RT_SQLITE_API locked_exception sealed : public sqlite_exception
		{
		public:
			explicit locked_exception() throw(){}
		};

		class RT_SQLITE_API locked_sharedcache_exception sealed : public sqlite_exception
		{
		public:
			explicit locked_sharedcache_exception() throw(){}
		};
	}
}