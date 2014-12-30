#include "pch.h"
#include "statement_value.h"
#include "statement_impl.h"

using namespace rt::sqlite;
using namespace rt::sqlite::implementations;

bool statement_value<>::is_null_at(int index) const
{
	return mpr_stmt_impl->is_null_at(index);
}

template<>
bool statement_value<>::get_at(int index) const
{
	return mpr_stmt_impl->get_at<bool>(index);
}

template<>
::std::int32_t statement_value<>::get_at(int index) const
{
	return mpr_stmt_impl->get_at<::std::int32_t>(index);
}

template<>
::std::int64_t statement_value<>::get_at(int index) const
{
	return mpr_stmt_impl->get_at<::std::int64_t>(index);
}

template<>
double statement_value<>::get_at(int index) const
{
	return mpr_stmt_impl->get_at<double>(index);
}

template<>
const unsigned char* statement_value<>::get_at(int index) const
{
	return mpr_stmt_impl->get_at<const unsigned char*>(index);
}

template<>
uchar_raw_data statement_value<>::get_at(int index) const
{
	return mpr_stmt_impl->get_at<uchar_raw_data>(index);
}

template<>
const wchar_t* statement_value<>::get_at(int index) const
{
	return mpr_stmt_impl->get_at<const wchar_t*>(index);
}

template<>
wchar_raw_data statement_value<>::get_at(int index) const
{
	return mpr_stmt_impl->get_at<wchar_raw_data>(index);
}

template<>
std::string statement_value<>::get_at(int index) const
{
	return mpr_stmt_impl->get_at<std::string>(index);
}

template<>
std::wstring statement_value<>::get_at(int index) const
{
	return mpr_stmt_impl->get_at<std::wstring>(index);
}

template<>
std::vector<unsigned char> statement_value<>::get_at(int index) const
{
	return mpr_stmt_impl->get_at<std::vector<unsigned char>>(index);
}