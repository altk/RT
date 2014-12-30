#pragma once
#include <cstdint>
#include <iterator>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <typeinfo>
#include "raw_data.h"
#include "implementations.h"
#include "utilities.h"

namespace rt
{
	namespace sqlite
	{
#pragma region statement_value

		template<class T>
		using remove_const_pointer_t = ::std::remove_const_t < ::std::remove_pointer_t<T> > ;

		template<class... TResults>
		class statement_value;

		template<>
		class RT_SQLITE_API statement_value < >
		{
		protected:
			::rt::sqlite::implementations::pr_stmt_impl mpr_stmt_impl;
			int m_size;

			bool is_null_at(int) const;

			template<class TArg>
			TArg get_at(int index) const
			{
				static_assert(false, "Method 'get_at' not implemented for this type of argument");
			}

			template<>
			bool get_at(int) const;
			template<>
			::std::int32_t get_at(int) const;
			template<>
			::std::int64_t get_at(int) const;
			template<>
			double get_at(int) const;
			template<>
			const unsigned char* get_at(int) const;
			template<>
			uchar_raw_data get_at(int) const;
			template<>
			const wchar_t* get_at(int) const;
			template<>
			wchar_raw_data get_at(int) const;
			template<>
			::std::string get_at(int) const;
			template<>
			::std::wstring get_at(int) const;
			template<>
			::std::vector<unsigned char> get_at(int) const;

			template<class TArg>
			::std::unique_ptr<remove_const_pointer_t<TArg>> try_get_at(int index) const
			{
				if (is_null_at(index))
				{
					return nullptr;
				}
				else
				{
					return ::std::make_unique<remove_const_pointer_t<TArg>>(get_at<remove_const_pointer_t<TArg>>(index));
				}
			}

			statement_value(int&& size)
				: m_size(::std::move(size)){}
			statement_value(::rt::sqlite::implementations::pr_stmt_impl p_stmt_impl, int&& size)
				: mpr_stmt_impl(p_stmt_impl)
				, m_size(::std::move(size)){}
		public:
			statement_value(const statement_value&) = default;
			statement_value(statement_value&& source) throw()
				: mpr_stmt_impl(::std::move(source.mpr_stmt_impl))
				, m_size(::std::move(source.m_size)){}

			statement_value& operator=(const statement_value&) = default;
			statement_value& operator=(statement_value&& source) throw()
			{
				if (this != &source)
				{
					mpr_stmt_impl = ::std::move(source.mpr_stmt_impl);
					m_size = ::std::move(m_size);
				}
				return *this;
			}

			virtual ~statement_value(){}
		};

		template<class TResult, class... TResults>
		class statement_value <TResult, TResults... >
			: public statement_value < TResults... >
		{
		protected:
			statement_value(int&& size) throw()
				: statement_value<TResults...>::statement_value(::std::move(size)){}
			statement_value(::rt::sqlite::implementations::pr_stmt_impl p_stmt_impl, int&& size) throw()
				: statement_value<TResults...>::statement_value(p_stmt_impl, ::std::move(size)){}
		public:
			statement_value() throw()
				: statement_value<TResults...>::statement_value(sizeof...(TResults)){}
			statement_value(const statement_value&) = default;
			statement_value(statement_value&& source) throw()
				: statement_value<TResults...>::statement_value(::std::move(source)){}
			statement_value(::rt::sqlite::implementations::pr_stmt_impl p_stmt_impl) throw()
				: statement_value<TResults...>::statement_value(p_stmt_impl, sizeof...(TResults)){}

			statement_value& operator=(const statement_value&) = default;
			statement_value& operator=(statement_value&& source) throw()
			{
				if (this != &source)
				{
					statement_value<TResults...>::operator=(::std::move(source.mpr_stmt_impl));
				}
				return *this;
			}

			inline TResult get() const
			{
				return statement_value<TResults...>::get_at<TResult>(statement_value<TResults...>::m_size - sizeof...(TResults));
			}
			inline ::std::unique_ptr<remove_const_pointer_t<TResult>> try_get() const
			{
				return statement_value<TResults...>::try_get_at<TResult>(statement_value<TResults...>::m_size - sizeof...(TResults));
			}

			virtual ~statement_value() override {}
		};

#pragma endregion

#pragma region statement_value_size

		template<class T>
		class statement_value_size;

		template<class ...T>
		class statement_value_size<statement_value<T...>>
			: public ::std::integral_constant < ::std::size_t, sizeof...(T) > {};

		template<class T>
		class statement_value_size<const T>
			: public ::std::integral_constant < ::std::size_t, statement_value_size<T>::value > {};

		template<class T>
		class statement_value_size<volatile T>
			: public ::std::integral_constant < ::std::size_t, statement_value_size<T>::value > {};

		template<class T>
		class statement_value_size<const volatile T>
			: public ::std::integral_constant < ::std::size_t, statement_value_size<T>::value > {};

#pragma endregion

#pragma region statement_value_getter

		template<size_t TIndex, class TResult, class ...TResults>
		struct statement_value_getter
		{
			static inline auto get(const ::rt::sqlite::statement_value<TResult, TResults...>& stmt_value)
				-> decltype(::rt::sqlite::statement_value_getter<TIndex - 1, TResults...>::get(stmt_value))
			{
				return ::rt::sqlite::statement_value_getter<TIndex - 1, TResults...>::get(stmt_value);
			}

			static inline auto try_get(const ::rt::sqlite::statement_value<TResult, TResults...>& stmt_value)
				-> decltype(::rt::sqlite::statement_value_getter<TIndex - 1, TResults...>::try_get(stmt_value))
			{
				return ::rt::sqlite::statement_value_getter<TIndex - 1, TResults...>::try_get(stmt_value);
			}
		};

		template<class TResult, class ...TResults>
		struct statement_value_getter < 1, TResult, TResults... >
		{
			static inline TResult get(const ::rt::sqlite::statement_value<TResult, TResults...>& stmt_value)
			{
				return stmt_value.get();
			}

			static inline auto try_get(const ::rt::sqlite::statement_value<TResult, TResults...>& stmt_value)
				-> decltype(stmt_value.try_get())
			{
				return stmt_value.try_get();
			}
		};

		template<size_t TIndex, class TResult, class ...TResults>
		inline auto get(const ::rt::sqlite::statement_value<TResult, TResults...>& stmt_value)
			->decltype(::rt::sqlite::statement_value_getter<TIndex, TResult, TResults...>::get(stmt_value))
		{
			return ::rt::sqlite::statement_value_getter<TIndex, TResult, TResults...>::get(stmt_value);
		}

		template<size_t TIndex, class TResult, class ...TResults>
		inline auto try_get(const ::rt::sqlite::statement_value<TResult, TResults...>& stmt_value)
			->decltype(::rt::sqlite::statement_value_getter<TIndex, TResult, TResults...>::try_get(stmt_value))
		{
			return ::rt::sqlite::statement_value_getter<TIndex, TResult, TResults...>::try_get(stmt_value);
		}

#pragma endregion

#pragma region statement_value_applicator

		template<size_t TIndex>
		struct statement_value_applicator
		{
			template<class TFunc, class TValue, class... TSources>
			static inline auto apply(TFunc&& func, const TValue& stmt_value, TSources&&... args)
				-> decltype(::rt::sqlite::statement_value_applicator<TIndex - 1>::apply(::std::forward<TFunc>(func), stmt_value, ::rt::sqlite::get<TIndex>(stmt_value), ::std::forward<TSources>(args)...))
			{
				return ::rt::sqlite::statement_value_applicator<TIndex - 1>::apply(::std::forward<TFunc>(func), stmt_value, ::rt::sqlite::get<TIndex>(stmt_value), ::std::forward<TSources>(args)...);
			}
		};

		template<>
		struct statement_value_applicator < 0 >
		{
			template<class TFunc, class TValue, class... TSources>
			static inline auto apply(TFunc&& func, const TValue&, TSources&&... args)
				-> decltype(::std::forward<TFunc>(func)(::std::forward<TSources>(args)...))
			{
				return ::std::forward<TFunc>(func)(::std::forward<TSources>(args)...);
			}
		};

		template<class TFunc, class TValue>
		inline auto apply(TFunc&& func, const TValue& stmt_value)
			-> decltype(::rt::sqlite::statement_value_applicator< ::rt::sqlite::statement_value_size<typename ::std::decay<TValue>::type>::value>::apply(::std::forward<TFunc>(func), stmt_value))
		{
			return ::rt::sqlite::statement_value_applicator< ::rt::sqlite::statement_value_size<typename ::std::decay<TValue>::type>::value>::apply(::std::forward<TFunc>(func), stmt_value);
		}

#pragma endregion
	}
}