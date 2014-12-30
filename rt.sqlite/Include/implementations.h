#pragma once
#include <memory>
#include "utilities.h"

namespace rt
{
	namespace sqlite
	{
		namespace implementations
		{
			class database_impl;

			using ps_db_impl = ::std::shared_ptr < database_impl > ;

			class statement_impl;
			using pr_stmt_impl = statement_impl*;

			class RT_SQLITE_API statement_impl_deleter
			{
			public:
				void operator()(pr_stmt_impl) const;
			};

			using ps_stmt_impl = ::std::shared_ptr < statement_impl > ;
			using pu_stmt_impl = ::std::unique_ptr < statement_impl, statement_impl_deleter > ;

			class transaction_impl;
			using pr_transaction_impl = transaction_impl*;

			class RT_SQLITE_API transaction_impl_deleter
			{
			public:
				void operator()(pr_transaction_impl) const;
			};

			using pu_transaction_impl = ::std::unique_ptr < transaction_impl, transaction_impl_deleter > ;
		}
	}
}