#include "pch.h"
#include "exception.h"
#include "database.h"
#include "database_impl.h"
#include "statement_impl.h"
#include "transaction_impl.h"

using namespace rt::sqlite;
using namespace rt::sqlite::implementations;

#pragma region CONSTRUCTORS

database::database(const char* path, database_open_mode open_mode) 
	: mp_impl(std::make_shared<database_impl>(path, open_mode))
{}
database::database(const wchar_t* path, database_open_mode open_mode)
	: mp_impl(std::make_shared<database_impl>(path, open_mode))
{}
database::database(const std::string & path, database_open_mode open_mode) 
	: mp_impl(std::make_shared<database_impl>(path, open_mode))
{}
database::database(const std::wstring & path, database_open_mode open_mode) 
	: mp_impl(std::make_shared<database_impl>(path, open_mode))
{}
database::database(const database& source) 
	: mp_impl(std::make_shared<database_impl>(*source.mp_impl))
{}
database::database(database&& source) throw()
	: mp_impl(std::move(source.mp_impl))
{}

#pragma endregion

database& database::operator=(const database& source)
{
	if (this != &source)
	{
		mp_impl = source.mp_impl;
	}
	return *this;
}
database& database::operator=(database&& source) throw()
{
	if (this != &source)
		mp_impl = std::move(source.mp_impl);
	return *this;
}

#pragma region CREATE PIML METHODS

pu_stmt_impl database::create_statement_p_impl(ps_db_impl p_impl, const char* str_stmt, int&& size)
{
	return pu_stmt_impl(new statement_impl(p_impl->create_statement(str_stmt, size)));
}
pu_stmt_impl database::create_statement_p_impl(ps_db_impl p_impl, const wchar_t* str_stmt, int&& size)
{
	return pu_stmt_impl(new statement_impl(p_impl->create_statement(str_stmt, size)));
}

#pragma endregion

void database::wait_unlock(ps_db_impl p_impl)
{
	p_impl->wait_unlock_notify();
}

pu_transaction_impl database::begin_transaction() const
{
	return pu_transaction_impl(new ::rt::sqlite::implementations::deferred_transaction_impl(mp_impl->begin_transaction()));
}
pu_transaction_impl database::begin_immediate_transaction() const
{
	return pu_transaction_impl(new ::rt::sqlite::implementations::immediate_transaction_impl(mp_impl->begin_immediate_transaction()));
}