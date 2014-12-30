#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include "statement_value.h"
#include "raw_data.h"
#include "implementations.h"
#include "utilities.h"

namespace rt
{
	namespace sqlite
	{
		class RT_SQLITE_API bc_statement
		{
		private:
			const int m_size;
		protected:
			class RT_SQLITE_API step_functor sealed
			{
			public:
				auto operator()(::rt::sqlite::implementations::pr_stmt_impl)
					-> bool;
			};

			::rt::sqlite::implementations::pu_stmt_impl mpu_stmt_impl;

			void bind_at(int, const bool&);
			void bind_at(int, bool&&);

			void bind_at(int, const ::std::int32_t&);
			void bind_at(int, ::std::int32_t&&);

			void bind_at(int, const ::std::int64_t&);
			void bind_at(int, ::std::int64_t&&);

			void bind_at(int, const double&);
			void bind_at(int, double&&);

			void bind_at(int, const ::std::string&);
			void bind_at(int, ::std::string&&);

			void bind_at(int, const ::std::wstring&);
			void bind_at(int, ::std::wstring&&);

			void bind_at(int, const ::std::vector<unsigned char>&);
			void bind_at(int, ::std::vector<unsigned char>&&);

			void bind_at(int, const uchar_raw_data&);
			void bind_at(int, uchar_raw_data&&);

			void bind_at(int, const wchar_raw_data&);
			void bind_at(int, wchar_raw_data&&);

			void bind_at(int, const char*);
			void bind_at(int, const wchar_t*);

			void bind_at(int, nullptr_t);

			template<class TArg>
			void bindImpl(int index, TArg&& arg, ::std::false_type)
			{
				bind_at(index, ::std::forward<TArg>(arg));
			}

			template<class TArg>
			void bindImpl(int index, const TArg* arg, ::std::true_type)
			{
				if (nullptr == arg)
				{
					bind_at(index, nullptr);
				}
				else
				{
					bind_at(index, *arg);
				}
			}
			
			template<class ...TArgs>
			void bind(TArgs&&... args);

			template<class TArg>
			inline void bind(TArg&& arg)
			{
				bindImpl(m_size, ::std::forward<TArg>(arg), ::std::is_pointer<::std::remove_reference_t<TArg>>());
			}

			template<class TArg, class ...TArgs>
			inline void bind(TArg&& arg, TArgs... args)
			{
				bindImpl(m_size - sizeof...(TArgs), ::std::forward<TArg>(arg), ::std::is_pointer<::std::remove_reference_t<TArg>>());
				bind(::std::forward<TArgs>(args)...);
			}

		public:
			bc_statement(::rt::sqlite::implementations::pu_stmt_impl&& p_stmt_impl, const int& size) throw()
				: mpu_stmt_impl(::std::move(p_stmt_impl))
				, m_size(size)
			{}
			bc_statement(bc_statement&& source) throw()
				: mpu_stmt_impl(::std::move(source.mpu_stmt_impl))
				, m_size(source.m_size)
			{}

			bc_statement& operator=(bc_statement&& source) throw()
			{
				if (this != &source)
				{
					mpu_stmt_impl = ::std::move(source.mpu_stmt_impl);
				}
				return *this;
			}

			bc_statement(const bc_statement&) = delete;
			bc_statement& operator=(const bc_statement&) = delete;

			void execute()
			{
				step_functor()(mpu_stmt_impl.get());
			}

			virtual void reset();

			virtual ~bc_statement() throw() {}
		};
	}
}