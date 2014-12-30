#pragma once
#include <memory>
#include <string>
#include <ppltasks.h>
#include "sqlite3.h"
#include "implementations.h"
#include "database_open_mode.h"
#include "statement_impl.h"
#include "transaction_impl.h"

namespace rt
{
	namespace sqlite
	{
		namespace implementations
		{
			class database_impl sealed
			{
			private:
				::std::string m_path;
				database_open_mode m_open_mode;

				static void unlock_notify_cb(void** args, int args_count)
				{
					reinterpret_cast<concurrency::task_completion_event<void>*>(args[0])->set();
				}
				static ::std::unique_ptr<char[]> to_char(const wchar_t* p_str)
				{
					auto bufferSize = std::wcslen(p_str) + 1;
					auto result = std::make_unique<char[]>(bufferSize);
					if (0 == WideCharToMultiByte(CP_UTF8, 0, p_str, -1, result.get(), bufferSize, NULL, NULL))
						throw std::exception("Can't convert wide string to normal string");
					return result;
				}
				static ::std::unique_ptr<char[]> to_char(const wchar_t* p_str, int size)
				{
					auto bufferSize = size*sizeof(wchar_t) + 1;
					auto result = std::make_unique<char[]>(bufferSize);
					if (0 == WideCharToMultiByte(CP_UTF8, 0, p_str, -1, result.get(), bufferSize, NULL, NULL))
						throw std::exception("Can't convert wide string to normal string");
					return result;
				}

				static ::std::shared_ptr<sqlite3> open_database(const char* path, database_open_mode open_mode)
				{
					sqlite3* p_sqlite;
					int flags = SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE;
					switch (open_mode)
					{
					case rt::sqlite::database_open_mode::readonly:
						flags |= SQLITE_OPEN_READONLY;
						break;
					case rt::sqlite::database_open_mode::read_write:
						flags |= SQLITE_OPEN_READWRITE;
						break;
					case rt::sqlite::database_open_mode::read_write_create:
						flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
						break;
					}
					if (SQLITE_OK != sqlite3_open_v2(path, &p_sqlite, flags, nullptr))
						throw open_database_exception(sqlite3_errmsg(p_sqlite));
					return std::shared_ptr<sqlite3>(p_sqlite, sqlite3_close_v2);
				}

				::std::shared_ptr<sqlite3> mp_sqlite;
			public:
				explicit database_impl(const char* path, database_open_mode open_mode)
					: mp_sqlite(open_database(path, open_mode))
					, m_path(path)
					, m_open_mode(open_mode)
				{
					sqlite3_exec(mp_sqlite.get(), "PRAGMA read_uncommitted=ON", nullptr, nullptr, nullptr);
				}
				explicit database_impl(const wchar_t* path, database_open_mode open_mode)
					: database_impl(to_char(path).get(), open_mode){}
				explicit database_impl(const std::string& path, database_open_mode open_mode)
					: database_impl(path.data(), open_mode){}
				explicit database_impl(std::string&& path, database_open_mode open_mode)
					: database_impl(path.data(), open_mode){}
				explicit database_impl(const std::wstring& path, database_open_mode open_mode)
					: database_impl(to_char(path.data(), path.size()).get(), open_mode){}
				explicit database_impl(std::wstring&& path, database_open_mode open_mode)
					: database_impl(to_char(path.data(), path.size()).get(), open_mode){}
				database_impl(const database_impl& source)
					: database_impl(source.m_path, source.m_open_mode){}
				database_impl(database_impl&& source) throw()
					: mp_sqlite(std::move(source.mp_sqlite))
					, m_path(std::move(source.m_path))
					, m_open_mode(std::move(source.m_open_mode)){}

				database_impl& operator=(const database_impl& source)
				{
					if (this != &source)
					{
						mp_sqlite = open_database(source.m_path.data(), source.m_open_mode);
						m_path = source.m_path;
						m_open_mode = source.m_open_mode;
					}
					return *this;
				}
				database_impl& operator=(database_impl&& source) throw()
				{
					if (this != &source)
					{
						mp_sqlite = std::move(source.mp_sqlite);
						m_path = std::move(source.m_path);
						m_open_mode = std::move(source.m_open_mode);
					}
					return *this;
				}

				inline statement_impl create_statement(const char* str_stmt, int size)const
				{
					return statement_impl(mp_sqlite, str_stmt, size);
				}
				inline statement_impl create_statement(const wchar_t* str_stmt, int size)const
				{
					return statement_impl(mp_sqlite, str_stmt, size);
				}

				inline deferred_transaction_impl begin_transaction() const
				{
					return deferred_transaction_impl(mp_sqlite);
				}
				inline immediate_transaction_impl begin_immediate_transaction() const
				{
					return immediate_transaction_impl(mp_sqlite);
				}

				void wait_unlock_notify()const
				{
					concurrency::task_completion_event<void> tce;
					if (SQLITE_LOCKED == sqlite3_unlock_notify(mp_sqlite.get(), unlock_notify_cb, &tce))
					{
						throw locked_exception();
					}
					concurrency::task<void>(tce).wait();
					sqlite3_unlock_notify(mp_sqlite.get(), nullptr, nullptr);
				}
			};
		}
	}
}

