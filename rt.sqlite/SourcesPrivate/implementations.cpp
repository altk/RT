#include "pch.h"
#include "implementations.h"
#include "statement_impl.h"
#include "transaction_impl.h"

using namespace std;
using namespace rt::sqlite::implementations;

void transaction_impl_deleter::operator()(pr_transaction_impl p_transaction_impl) const
{
	p_transaction_impl->~transaction_impl();
}

void statement_impl_deleter::operator()(pr_stmt_impl p_stmt_impl) const
{
	p_stmt_impl->~statement_impl();
}