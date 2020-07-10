#ifndef _STATIC_QUEUE_HS_
#define _STATIC_QUEUE_HS_
#include "debugging.h"
#include <cstddef>
#include <iterator>
#include <array>
#include <exception>
#include <stdexcept>

//TODO: 
//		[Finished] Check cbegin() and cend() because I (maybe) think it can change underlying
//	value 
//		[Finished] Modify constructor and interator to make it not possible to change value (of
//	CyclicIterator. StaticQueue does not provide to write base on index or
//	interator. Only Push and Pop allow
//		[Partial] Check behaviour of StaticQueue when N is 1 and 2
//			--> When N is 1, behaviour of cbegin and cend is not consist base on
//			implementation
namespace saitou
{


template<typename T, size_t N>
class static_queue
{
	static_assert(N > 0);
	public:
	class CyclicIterator
	{
		public:
		//using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::input_iterator_tag;
		using self_type = CyclicIterator;
		public:
		CyclicIterator(T* current,T* start, T* end): m_ptr(current), m_start(start), m_end(end) {}
		public:
		reference operator*()
		{
			return *m_ptr;
		}
		self_type& operator++()
		{
			if(++m_ptr == m_end)
				m_ptr = m_start;
			return *this;

		}
		self_type operator++(int)
		{
			auto tmp = *this;
			if(++m_ptr == m_end)
				m_ptr = m_start;
			return tmp;
		}
		bool operator==(const CyclicIterator& it)
		{
			return it.m_ptr == m_ptr;
		}
		bool operator!=(const CyclicIterator& it)
		{
			return !operator==(it);
		}
		bool operator->()
		{
			return m_ptr;
		}
	private:
		pointer m_ptr = nullptr;
		pointer const m_start = nullptr;
		pointer const m_end = nullptr;
};
	class ConstCyclicIterator
	{
		public:
		//using iterator_category = std::random_access_iterator_tag;
		using value_type = const T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = std::input_iterator_tag;
		using self_type = ConstCyclicIterator;
		public:
		ConstCyclicIterator(pointer current, pointer start, pointer end): m_ptr(current), m_start(start), m_end(end) {}
		public:
		reference operator*()
		{
			return *m_ptr;
		}
		self_type& operator++()
		{
			if(++m_ptr == m_end)
				m_ptr = m_start;
			return *this;

		}
		self_type operator++(int)
		{
			auto tmp = *this;
			if(++m_ptr == m_end)
				m_ptr = m_start;
			return tmp;
		}
		bool operator==(const ConstCyclicIterator& it)
		{
			return it.m_ptr == m_ptr;
		}
		bool operator!=(const ConstCyclicIterator& it)
		{
			return !operator==(it);
		}
		bool operator->()
		{
			return m_ptr;
		}
	private:
		pointer m_ptr = nullptr;
		pointer m_start = nullptr;
		pointer m_end = nullptr;
};
	private:
		std::array<T, N + 1> data = {};
		CyclicIterator front_it;
		CyclicIterator back_it;
	public:
		static_queue()
			: 
			front_it(CyclicIterator(data.begin(), data.begin(), data.end())),
			back_it(CyclicIterator(data.begin(), data.begin(), data.end()))
		{}
	public:
		void Push(T t) noexcept
		{
			*back_it = t;
			back_it++;
			if(back_it == front_it)
				front_it++;
		}
		T Pop()
		{
			if(back_it == front_it)
				throw std::underflow_error("Queue Empty");
			return *front_it++;
		}
		bool empty() noexcept
		{
			return front_it == back_it;
		}
		ConstCyclicIterator cbegin() noexcept
		{
			return ConstCyclicIterator(&*front_it, data.cbegin(), data.cend());
		}
		ConstCyclicIterator cend() noexcept
		{
			return ConstCyclicIterator(&*back_it, data.cbegin(), data.cend());
		}
};
template<typename T>
class static_queue<T, 1>
{
	public:
	class ConstCyclicIterator
	{
		public:
		//using iterator_category = std::random_access_iterator_tag;
		using value_type = const T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = std::input_iterator_tag;
		using self_type = ConstCyclicIterator;
		public:
		ConstCyclicIterator(pointer current, pointer start, pointer end): m_ptr(current), m_start(start), m_end(end) {}
		public:
		reference operator*()
		{
			return *m_ptr;
		}
		self_type& operator++()
		{
			if(++m_ptr == m_end)
				m_ptr = m_start;
			return *this;

		}
		self_type operator++(int)
		{
			auto tmp = *this;
			if(++m_ptr == m_end)
				m_ptr = m_start;
			return tmp;
		}
		bool operator==(const ConstCyclicIterator& it)
		{
			return it.m_ptr == m_ptr;
		}
		bool operator!=(const ConstCyclicIterator& it)
		{
			return !operator==(it);
		}
		bool operator->()
		{
			return m_ptr;
		}
	private:
		pointer m_ptr = nullptr;
		pointer m_start = nullptr;
		pointer m_end = nullptr;
};
	private:
		bool is_set = false;
		T data;
	public:
		void Push(T t) noexcept
		{
			is_set = true;
			data = t;
		}

		T Pop()
		{
			if(is_set == false)
				throw std::underflow_error("Queue empty");
			is_set = false;
			return data;
		}

		bool empty() noexcept
		{
			return !is_set;
		}

		ConstCyclicIterator cbegin() noexcept
		{
			return ConstCyclicIterator(&data, &data, &data + 1);
		}
		ConstCyclicIterator cend() noexcept
		{
			return ConstCyclicIterator(&data + 1, &data, &data +1);
		}
};
}


#endif
