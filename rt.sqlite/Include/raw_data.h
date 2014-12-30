#pragma once
#include "implementations.h"

namespace rt
{
	namespace sqlite
	{
		template<class T>
		class raw_data;

		template<class T>
		class raw_data < const T* >
		{
		private:
			const T* mp_data;
			size_t m_size;
		public:
			raw_data(const T* p_value, size_t&& size) throw()
				: mp_data(p_value)
				, m_size(::std::move(size)){}
			raw_data(const raw_data&) = default;
			raw_data(raw_data&& source) throw()
				: mp_data(::std::move(source.mp_data))
				, m_size(::std::move(source.m_size)){}

			raw_data& operator=(const raw_data&) = default;
			raw_data& operator=(raw_data&& source) throw()
			{
				if (this != &source)
				{
					mp_data = ::std::move(source.mp_data);
					m_size = ::std::move(source.m_size);
				}
				return *this;
			}

			const T* data() const throw()
			{
				return mp_data;
			}
			const size_t& size() const
			{
				return m_size;
			}
		};

		using uchar_raw_data = raw_data < const unsigned char* >;
		using wchar_raw_data = raw_data < const wchar_t* >;

		template class RT_SQLITE_API raw_data < const unsigned char* >;
		template class RT_SQLITE_API raw_data < const wchar_t* >;
	}
}