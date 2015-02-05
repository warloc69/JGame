
#pragma once

#include <boost/thread.hpp>
using namespace boost;

//!Custom queue with fixed size [N]
//Data is kept in array. After each push/pop RW-counters are incremented.
//
//TODO: performance/threadsafe testing
template <typename T, size_t N> struct Queue
{
	public:
		Queue()
		{
			m_reader = 0;
			m_writer = 0;
			m_size = 0;
			memset(&m_list[0], 0, sizeof(m_list));
		}

		//!Adds element (T v) into queue. Returns false in case when queue is full.
		void push(T v)
		{
			m_mutex.lock();

			if(m_size == N)
			{
				m_mutex.unlock();
				return;
			}

			m_list[m_writer] = v;
			m_writer = m_writer == N - 1 ? 0 : m_writer + 1;
			m_size++;

			m_mutex.unlock();
		}

		//!Reads element (T& v) from queue. Return false in case of empty queue.
		bool pop(T& v)
		{
			m_mutex.lock();

			if(!m_size)
			{
				m_mutex.unlock();
				return false;
			}

			v = m_list[m_reader];
			m_reader = m_reader == N - 1 ? 0 : m_reader + 1;
			m_size--;

			m_mutex.unlock();

			return true;
		}

	private:
		mutex m_mutex;

		T m_list[N];
		int m_reader;
		int m_writer;
		int m_size;
};