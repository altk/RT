#pragma once
#include <memory>
#include "implementations.h"

namespace rt
{
	namespace sqlite
	{
		class RT_SQLITE_API transaction sealed
		{
		private:
			::rt::sqlite::implementations::pu_transaction_impl mpu_transaction_impl;
		public:
			transaction(::rt::sqlite::implementations::pu_transaction_impl&& p_transaction_impl) throw()
				: mpu_transaction_impl(::std::move(p_transaction_impl)){}
			transaction(transaction&& source) throw()
				: mpu_transaction_impl(::std::move(source.mpu_transaction_impl)){}

			transaction& operator=(transaction&& source) throw()
			{
				if (this != &source)
				{
					mpu_transaction_impl = ::std::move(source.mpu_transaction_impl);
				}
				return *this;
			}

			transaction(const transaction& source) = delete;
			transaction& operator=(const transaction& source) = delete;
		};
	}
}