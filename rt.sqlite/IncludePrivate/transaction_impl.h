#pragma once
#include <memory>
#include "sqlite3.h"
#include "exception.h"

namespace rt
{
	namespace sqlite
	{
		namespace implementations
		{
			class transaction_impl
			{
			private:
				::std::shared_ptr<sqlite3> mps_sqlite;

				mutable bool is_started;

				void execute_statement(const char* str_stmt)const
				{
					char* error_message;
					try
					{
						switch (sqlite3_exec(mps_sqlite.get(), str_stmt, nullptr, nullptr, &error_message))
						{
						case SQLITE_OK:
							return;
						case SQLITE_BUSY:
							throw busy_exception();
						case SQLITE_LOCKED:
							throw locked_exception();
						case SQLITE_LOCKED_SHAREDCACHE:
							throw locked_sharedcache_exception();
						default:
							throw execute_statement_exception(error_message);
						}
					}
					catch (const std::exception& exception)
					{
						sqlite3_free(error_message);
						throw;
					}
				}
			protected:
				transaction_impl(::std::shared_ptr<sqlite3> p_sqlite, const char* begin_stmt)
					: mps_sqlite(p_sqlite)
				{
					execute_statement(begin_stmt);
					is_started = true;
				}
			public:
				transaction_impl(transaction_impl&& source) throw()
					: mps_sqlite(::std::move(source.mps_sqlite))
					, is_started(::std::move(source.is_started))
				{
					source.is_started = false;
				}

				transaction_impl& operator=(transaction_impl&& source)  throw()
				{
					if (this != &source)
					{
						mps_sqlite = ::std::move(source.mps_sqlite);
						is_started = ::std::move(source.is_started);
						source.is_started = false;
					}
					return *this;
				}

				transaction_impl(const transaction_impl&) = delete;
				transaction_impl& operator=(const transaction_impl&) = delete;

				~transaction_impl() throw()
				{
					try
					{
						if (!is_started)return;
						execute_statement("COMMIT TRANSACTION");
					}
					catch (const ::std::exception&)
					{
						try
						{
							execute_statement("ROLLBACK TRANSACTION");
						}
						catch (const ::std::exception&)
						{
						}
					}
				}
			};

			class deferred_transaction_impl sealed
				: public transaction_impl
			{
			public:
				deferred_transaction_impl(::std::shared_ptr<sqlite3> p_sqlite)
					: transaction_impl(p_sqlite, &"BEGIN DEFERRED TRANSACTION"[0]){}
				deferred_transaction_impl(deferred_transaction_impl&& source) throw()
					: transaction_impl(::std::move(source)){}

				deferred_transaction_impl& operator=(deferred_transaction_impl&& source)
				{
					if (this != &source)
					{
						transaction_impl::operator=(::std::move(source));
					}
					return *this;
				}

				deferred_transaction_impl(const deferred_transaction_impl&) = delete;
				deferred_transaction_impl& operator=(const deferred_transaction_impl&) = delete;
			};

			class immediate_transaction_impl sealed
				: public transaction_impl
			{
			public:
				immediate_transaction_impl(::std::shared_ptr<sqlite3> p_sqlite)
					: transaction_impl(p_sqlite, &"BEGIN IMMEDIATE TRANSACTION"[0]){}
				immediate_transaction_impl(immediate_transaction_impl&& source) throw()
					: transaction_impl(::std::move(source)){}

				immediate_transaction_impl& operator=(immediate_transaction_impl&& source)
				{
					if (this != &source)
					{
						transaction_impl::operator=(::std::move(source));
					}
					return *this;
				}

				immediate_transaction_impl(const immediate_transaction_impl&) = delete;
				immediate_transaction_impl& operator=(const immediate_transaction_impl&) = delete;
			};
		}
	}
}