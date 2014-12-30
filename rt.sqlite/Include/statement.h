#pragma once
#include <memory>
#include "bc_statement.h"
#include "statement_value.h"
#include "implementations.h"
#include "utilities.h"

namespace rt
{
	namespace sqlite
	{
		template<class ...TArgs>
		class statement
			: public bc_statement
		{
		public:
			statement(::rt::sqlite::implementations::pu_stmt_impl&& p_stmt) throw()
				: bc_statement(::std::move(p_stmt), sizeof...(TArgs))
			{}
			statement(statement&& source)  throw()
				: bc_statement(::std::move(source))
			{}

			statement& operator=(statement&& source) throw()
			{
				if (this != &source)
				{
					bc_statement::operator=(::std::move(source));
				}
				return *this;
			}

			statement(const statement&) = delete;
			statement& operator=(const statement&) = delete;

			void bind(TArgs... args)
			{
				bc_statement::bind(::std::forward<TArgs>(args)...);
			}

			virtual ~statement() throw() override {}

#pragma region statement_data
			template<class TResult, class... TResults>
			class statement_data sealed
				: public statement
			{
			public:
#pragma region iterator
				class iterator sealed : public ::std::iterator <
					::std::input_iterator_tag,
					::std::shared_ptr<::rt::sqlite::statement_value<TResult, TResults...>>,
					::std::ptrdiff_t,
					::std::shared_ptr<::rt::sqlite::statement_value<TResult, TResults...>>*,
					::std::shared_ptr<::rt::sqlite::statement_value<TResult, TResults...>>& >
				{
				private:
					::rt::sqlite::implementations::pr_stmt_impl mpr_stmt_impl;
					::std::shared_ptr<::rt::sqlite::statement_value<TResult, TResults...>> mps_stmt_value;
					bool m_isDone;
				public:
					iterator(const iterator&) = default;
					iterator(::rt::sqlite::implementations::pr_stmt_impl p_stmt_impl, bool isDone) throw()
						: mpr_stmt_impl(p_stmt_impl)
						, mps_stmt_value(::std::make_shared<::rt::sqlite::statement_value<TResult, TResults...>>(p_stmt_impl))
						, m_isDone(isDone)
					{}

					iterator& operator=(const iterator&) = default;

					::std::shared_ptr<::rt::sqlite::statement_value<TResult, TResults...>>& operator*() throw()
					{
						return mps_stmt_value;
					}
					::std::shared_ptr<::rt::sqlite::statement_value<TResult, TResults...>>* operator->() throw()
					{
						return &mps_stmt_value;
					}

					iterator& operator++()
					{
						if (!m_isDone)
						{
							m_isDone = !step_functor()(mpr_stmt_impl);
						}
						return *this;
					}
					iterator& operator++(int)
					{
						if (!m_isDone)
						{
							m_isDone = !statement_holder::step_functor()(mpr_stmt_impl);
						}
						return *this;
					}

					friend bool operator==(const iterator& lhs, const iterator& rhs)
					{
						return &lhs == &rhs || (lhs.mpr_stmt_impl == rhs.mpr_stmt_impl && lhs.m_isDone == rhs.m_isDone);
					}
					friend bool operator!=(const iterator& lhs, const iterator& rhs)
					{
						return !(lhs == rhs);
					}
				};
#pragma endregion
			protected:
				::std::unique_ptr<iterator> mpu_begin_iterator;
				::std::unique_ptr<iterator> mpu_end_iterator;
			public:
				statement_data(::rt::sqlite::implementations::pu_stmt_impl&& p_stmt) throw()
					: statement(::std::move(p_stmt))
					, mpu_end_iterator(::std::make_unique<iterator>(mpu_stmt_impl.get(), true))
				{}
				statement_data(statement_data&& source) throw()
					: statement(::std::move(source))
					, mpu_end_iterator(::std::make_unique<iterator>(mpu_stmt_impl.get(), false))
				{}

				statement_data& operator=(statement_data&& source) throw()
				{
					if (this != &source)
					{
						statement::operator=(::std::move(source));
						mpu_begin_iterator = ::std::move(source.mpu_begin_iterator);
						mpu_end_iterator = ::std::move(source.mpu_end_iterator);
					}
					return *this;
				}

				statement_data(const statement_data&) = delete;
				statement_data& operator=(const statement_data&) = delete;

				iterator& begin()
				{
					if (nullptr == mpu_begin_iterator)
					{
						mpu_begin_iterator = ::std::make_unique<iterator>(
							mpu_stmt_impl.get(),
							!step_functor()(mpu_stmt_impl.get())
							);
					}
					return *mpu_begin_iterator;
				}
				iterator& end()
				{
					return *mpu_end_iterator;
				}

				virtual void reset() override
				{
					statement::reset();
					mpu_begin_iterator = nullptr;
				}

				virtual ~statement_data() throw() override {}
			};
#pragma endregion

		};
	}
}