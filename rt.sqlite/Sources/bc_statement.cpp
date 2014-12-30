#include "pch.h"
#include "bc_statement.h"
#include "statement_impl.h"

using namespace rt::sqlite;

#pragma region BIND PARAMETER METHODS

void bc_statement::bind_at(int index, const bool& value)
{
	mpu_stmt_impl->bind_at(index, value);
}
void bc_statement::bind_at(int index, bool&& value)
{
	mpu_stmt_impl->bind_at(index, std::move(value));
}

void bc_statement::bind_at(int index, const ::std::int32_t& value)
{
	mpu_stmt_impl->bind_at(index, value);
}
void bc_statement::bind_at(int index, ::std::int32_t&& value)
{
	mpu_stmt_impl->bind_at(index, std::move(value));
}

void bc_statement::bind_at(int index, const ::std::int64_t& value)
{
	mpu_stmt_impl->bind_at(index, value);
}
void bc_statement::bind_at(int index, ::std::int64_t&& value)
{
	mpu_stmt_impl->bind_at(index, std::move(value));
}

void bc_statement::bind_at(int index, const double& value)
{
	mpu_stmt_impl->bind_at(index, value);
}
void bc_statement::bind_at(int index, double&& value)
{
	mpu_stmt_impl->bind_at(index, std::move(value));
}

void bc_statement::bind_at(int index, const std::string& value)
{
	mpu_stmt_impl->bind_at(index, value);
}
void bc_statement::bind_at(int index, std::string&& value)
{
	mpu_stmt_impl->bind_at(index, std::move(value));
}

void bc_statement::bind_at(int index, const std::wstring& value)
{
	mpu_stmt_impl->bind_at(index, value);
}
void bc_statement::bind_at(int index, std::wstring&& value)
{
	mpu_stmt_impl->bind_at(index, std::move(value));
}

void bc_statement::bind_at(int index, const std::vector<unsigned char>& value)
{
	mpu_stmt_impl->bind_at(index, value);
}
void bc_statement::bind_at(int index, std::vector<unsigned char>&& value)
{
	mpu_stmt_impl->bind_at(index, std::move(value));
}

void bc_statement::bind_at(int index, const uchar_raw_data& value)
{
	mpu_stmt_impl->bind_at(index, value);
}
void bc_statement::bind_at(int index, uchar_raw_data&& value)
{
	mpu_stmt_impl->bind_at(index, ::std::move(value));
}

void bc_statement::bind_at(int index, const wchar_raw_data& value)
{
	mpu_stmt_impl->bind_at(index, value);
}
void bc_statement::bind_at(int index, wchar_raw_data&& value)
{
	mpu_stmt_impl->bind_at(index, ::std::move(value));
}

void bc_statement::bind_at(int index, const char* value)
{
	mpu_stmt_impl->bind_at(index, value);
}
void bc_statement::bind_at(int index, const wchar_t* value)
{
	mpu_stmt_impl->bind_at(index, value);
}

void bc_statement::bind_at(int index, nullptr_t value)
{
	mpu_stmt_impl->bind_at(index, value);
}

#pragma endregion

void bc_statement::reset()
{
	mpu_stmt_impl->reset();
}

auto bc_statement::step_functor::operator()(::rt::sqlite::implementations::pr_stmt_impl p_stmt_impl)
-> bool
{
	return p_stmt_impl->step();
}