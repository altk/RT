#pragma once
#include <memory>
#include <vector>
#include <cstdint>
#include "raw_data.h"
#include "sqlite3.h"
#include "implementations.h"
#include "exception.h"

namespace rt
{
	namespace sqlite
	{
		namespace implementations
		{
			class sqlite3_stmt_deleter
			{
			public:
				inline void operator()(sqlite3_stmt* p_sqlite3_stmt) const
				{
					sqlite3_finalize(p_sqlite3_stmt);
				}
			};
			class statement_impl sealed
			{
			private:
				::std::shared_ptr<sqlite3> mp_sqlite;
				using pu_sqlite_stmt = ::std::unique_ptr < sqlite3_stmt, sqlite3_stmt_deleter > ;
				pu_sqlite_stmt mp_stmt;

				static pu_sqlite_stmt create_statement(sqlite3* sqlite, const char* str_stmt, int size)
				{
					sqlite3_stmt* p_stmt;
					if (SQLITE_OK == sqlite3_prepare_v2(sqlite, str_stmt, size, &p_stmt, nullptr))
						return pu_sqlite_stmt(p_stmt);
					throw create_statement_exception(sqlite3_errmsg(sqlite));
				}
				static pu_sqlite_stmt create_statement(sqlite3* sqlite, const wchar_t* str_stmt, int size)
				{
					sqlite3_stmt* p_stmt;
					if (SQLITE_OK == sqlite3_prepare16_v2(sqlite, str_stmt, size*sizeof(wchar_t), &p_stmt, nullptr))
						return pu_sqlite_stmt(p_stmt);
					throw create_statement_exception(sqlite3_errmsg(sqlite));
				}
				static void test_bind(sqlite3* p_sqlite, int&& rc)
				{
					if (SQLITE_OK != rc)
						throw bind_parameter_exception(sqlite3_errmsg(p_sqlite));
				}

			public:
				statement_impl()
					: mp_sqlite()
					, mp_stmt(nullptr){}
				statement_impl(::std::shared_ptr<sqlite3> p_sqlite, const char* str_stmt, int size)
					: mp_sqlite(p_sqlite)
					, mp_stmt(::std::move(create_statement(p_sqlite.get(), str_stmt, size))){}
				statement_impl(::std::shared_ptr<sqlite3> p_sqlite, const wchar_t* str_stmt, int size)
					: mp_sqlite(p_sqlite)
					, mp_stmt(::std::move(create_statement(p_sqlite.get(), str_stmt, size))){}

				statement_impl(statement_impl&& source) throw()
					: mp_sqlite(::std::move(source.mp_sqlite))
					, mp_stmt(::std::move(source.mp_stmt))
				{}
				statement_impl& operator=(statement_impl&& source) throw()
				{
					if (this != &source)
					{
						this->mp_sqlite = ::std::move(source.mp_sqlite);
						this->mp_stmt = ::std::move(source.mp_stmt);
					}
					return *this;
				}

				statement_impl(const statement_impl&) = delete;
				statement_impl& operator=(const statement_impl&) = delete;

				bool step() const
				{
					switch (sqlite3_step(mp_stmt.get()))
					{
					case SQLITE_BUSY:
						throw busy_exception();
					case SQLITE_LOCKED:
						throw locked_exception();
					case SQLITE_LOCKED_SHAREDCACHE:
						throw locked_sharedcache_exception();
					case SQLITE_ROW:
						return true;
					case SQLITE_OK:
					case SQLITE_DONE:
						return false;
					case SQLITE_ERROR:
					case SQLITE_MISUSE:
					default:
						throw step_statement_exception(sqlite3_errmsg(mp_sqlite.get()));
					}
				}
				void reset()
				{
					if (SQLITE_OK == sqlite3_reset(mp_stmt.get()))return;
					throw reset_statement_exception(sqlite3_errmsg(mp_sqlite.get()));
				}

				bool is_null_at(int index)
				{
					return SQLITE_NULL == sqlite3_column_type(mp_stmt.get(), index);
				}

				void bind_at(int index, const bool& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_int(mp_stmt.get(), index, value));
				}
				void bind_at(int index, bool&& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_int(mp_stmt.get(), index, value));
				}

				void bind_at(int index, const ::std::int32_t& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_int(mp_stmt.get(), index, value));
				}
				void bind_at(int index, ::std::int32_t&& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_int(mp_stmt.get(), index, value));
				}

				void bind_at(int index, const ::std::int64_t& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_int64(mp_stmt.get(), index, value));
				}
				void bind_at(int index, ::std::int64_t&& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_int64(mp_stmt.get(), index, value));
				}

				void bind_at(int index, const double& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_double(mp_stmt.get(), index, value));
				}
				void bind_at(int index, double&& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_double(mp_stmt.get(), index, value));
				}

				void bind_at(int index, const std::string& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_text(mp_stmt.get(), index, value.c_str(), static_cast<int>(value.size()), SQLITE_STATIC));
				}
				void bind_at(int index, std::string&& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_text(mp_stmt.get(), index, value.c_str(), static_cast<int>(value.size()), SQLITE_STATIC));
				}

				void bind_at(int index, const std::wstring& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_text16(mp_stmt.get(), index, value.c_str(), static_cast<int>(value.size()*sizeof(wchar_t)), SQLITE_STATIC));
				}
				void bind_at(int index, std::wstring&& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_text16(mp_stmt.get(), index, value.c_str(), static_cast<int>(value.size()*sizeof(wchar_t)), SQLITE_STATIC));
				}

				void bind_at(int index, const std::vector<unsigned char>& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_blob(mp_stmt.get(), index, value.data(), static_cast<int>(value.size()), SQLITE_STATIC));
				}
				void bind_at(int index, std::vector<unsigned char>&& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_blob(mp_stmt.get(), index, value.data(), static_cast<int>(value.size()), SQLITE_STATIC));
				}

				void bind_at(int index, const uchar_raw_data& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_blob(mp_stmt.get(), index, value.data(), value.size(), SQLITE_STATIC));
				}
				void bind_at(int index, uchar_raw_data&& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_blob(mp_stmt.get(), index, value.data(), value.size(), SQLITE_STATIC));
				}

				void bind_at(int index, const wchar_raw_data& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_text16(mp_stmt.get(), index, value.data(), value.size(), SQLITE_STATIC));
				}
				void bind_at(int index, wchar_raw_data&& value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_text16(mp_stmt.get(), index, value.data(), value.size(), SQLITE_STATIC));
				}

				void bind_at(int index, const char* value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_blob(mp_stmt.get(), index, value, -1, SQLITE_STATIC));
				}
				void bind_at(int index, const wchar_t* value)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_text16(mp_stmt.get(), index, value, -1, SQLITE_STATIC));
				}
				
				void bind_at(int index, nullptr_t)
				{
					test_bind(mp_sqlite.get(), sqlite3_bind_null(mp_stmt.get(), index));
				}

				template<class TResult>
				TResult get_at(int)
				{
					static_assert(false, "methor for type not implemented");
				}

				template<>
				bool get_at(int index)
				{
					return sqlite3_column_int(mp_stmt.get(), index);
				}

				template<>
				::std::int32_t get_at(int index)
				{
					return sqlite3_column_int(mp_stmt.get(), index);
				}

				template<>
				::std::int64_t get_at(int index)
				{
					return sqlite3_column_int64(mp_stmt.get(), index);
				}

				template<>
				double get_at(int index)
				{
					return sqlite3_column_double(mp_stmt.get(), index);
				}

				template<>
				const unsigned char* get_at(int index)
				{
					return reinterpret_cast<const unsigned char*>(sqlite3_column_blob(mp_stmt.get(), index));
				}

				template<>
				uchar_raw_data get_at(int index)
				{
					return uchar_raw_data(reinterpret_cast<const unsigned char*>(sqlite3_column_blob(mp_stmt.get(), index)), sqlite3_column_bytes(mp_stmt.get(), index));
				}

				template<>
				const wchar_t* get_at(int index)
				{
					return reinterpret_cast<const wchar_t*>(sqlite3_column_text16(mp_stmt.get(), index));
				}

				template<>
				wchar_raw_data get_at(int index)
				{
					return wchar_raw_data(reinterpret_cast<const wchar_t*>(sqlite3_column_text16(mp_stmt.get(), index)), sqlite3_column_bytes16(mp_stmt.get(), index) / sizeof(wchar_t));
				}

				template<>
				std::string get_at(int index)
				{
					return std::string(reinterpret_cast<const char*>(sqlite3_column_text(mp_stmt.get(), index)));
				}

				template<>
				std::wstring get_at(int index)
				{
					return std::wstring(reinterpret_cast<const wchar_t*>(sqlite3_column_text16(mp_stmt.get(), index)));
				}

				template<>
				std::vector<unsigned char> get_at(int index)
				{
					auto data = reinterpret_cast<const unsigned char*>(sqlite3_column_blob(mp_stmt.get(), index));
					return std::vector<unsigned char>(data, data + sqlite3_column_bytes(mp_stmt.get(), index));
				}

				~statement_impl() throw(){}
			};
		}
	}
}

