#pragma once
#include <memory>
#include <string>
#include <ppltasks.h>
#include "implementations.h"
#include "database_open_mode.h"
#include "utilities.h"

namespace rt
{
	namespace sqlite
	{
		class RT_SQLITE_API database sealed
		{
		private:
			::rt::sqlite::implementations::ps_db_impl mp_impl;

			using task_ps_stmt_impl = concurrency::task < ::rt::sqlite::implementations::ps_stmt_impl > ;

			static ::rt::sqlite::implementations::pu_stmt_impl create_statement_p_impl(::rt::sqlite::implementations::ps_db_impl, const char*, int&&);
			static ::rt::sqlite::implementations::pu_stmt_impl create_statement_p_impl(::rt::sqlite::implementations::ps_db_impl, const wchar_t*, int&&);

			template<class TString, int TSize>
			inline static ::rt::sqlite::implementations::pu_stmt_impl create_statement_p_impl(::rt::sqlite::implementations::ps_db_impl p_db_impl, const TString(&str_stmt)[TSize])
			{
				return create_statement_p_impl(p_db_impl, str_stmt, TSize - 1);
			}

			template<class TString>
			inline static ::rt::sqlite::implementations::pu_stmt_impl create_statement_p_impl(::rt::sqlite::implementations::ps_db_impl p_db_impl, TString&& str_stmt)
			{
				return create_statement_p_impl(p_db_impl, str_stmt.data(), str_stmt.size());
			}

			static void wait_unlock(::rt::sqlite::implementations::ps_db_impl);
		public:
			explicit database(const char*, database_open_mode);
			explicit database(const wchar_t*, database_open_mode);
			explicit database(const ::std::string&, database_open_mode);
			explicit database(const ::std::wstring&, database_open_mode);
			database(const database&);
			database(database&&) throw();

			database& operator=(const database&);
			database& operator=(database&&) throw();

			template<class TString>
			inline ::rt::sqlite::implementations::pu_stmt_impl create_statement(TString&& str_stmt) const
			{
				return create_statement_p_impl(mp_impl, std::forward<TString>(str_stmt));
			}

			::rt::sqlite::implementations::pu_transaction_impl begin_transaction() const;
			::rt::sqlite::implementations::pu_transaction_impl begin_immediate_transaction() const;

			~database() throw(){}
		};
	}
}