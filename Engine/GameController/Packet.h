
#pragma once

#include "baseTypes.h"
#include <vector>

#include <assert.h>

class Packet
{
	public:
		Packet()
		{
			m_read_pos = 0;
			m_write_pos = 0;
			m_finalized = false;
		}

		Packet(uint8 N)
		{
			m_read_pos = 0;
			m_write_pos = 0;
			m_finalized = false;
			m_buffer.resize(N);
		}

		uint8 readByte()
		{
			return read<uint8>();
		}

		template <typename T, size_t N> Packet& writeArray(T (&v)[N], uint8 start, uint8 size)
		{
			assert(size <= N);
			for(int i = start; i < size; i++)
				append(v[i]);
			return *this;
		}

		void writeString(std::string v)
		{
			for(size_t i = 0; i < v.length(); i++) 
				append(v[i]);
		}

		std::vector<uint8> readArray(uint8 size)
		{
			std::vector<uint8> arr = std::vector<uint8>(size);
			for(int i = 0; i < size; i++)
				arr[i] = read<uint8>();
			return arr;
		}

		template <typename T, size_t N> Packet& operator<<(T (&v)[N])
		{
			for(int i = 0; i < N; i++)
				append(v[i]);
			return *this;
		}

		template <typename T> Packet& operator<<(std::vector<T> v)
		{
			for(T k : v) 
				append(k);
			return *this;
		}

		template <typename T> Packet& operator<<(T v)
		{
			append(v);
			return *this;
		}

		template <typename T> Packet& operator>>(T& v)
		{
			v = read<T>();
			return *this;
		}

		void resetRead()
		{
			m_read_pos = 0;
		}

		void finalize()
		{
			append<uint8>('\n');
			m_finalized = true;
		}

		uint8 size()
		{
			return m_finalized ? m_write_pos : m_buffer.size();
		}

		uint8* getBuffer()
		{
			return m_buffer.data();
		}

	protected:
		std::vector<uint8> m_buffer;
		uint8 m_read_pos;
		uint8 m_write_pos;
		bool m_finalized;

	private:
		template <typename T>
		void append(const T v)
		{
			if(m_finalized)
				return;

			if(m_write_pos + sizeof(T) > m_buffer.size())
				m_buffer.resize(m_write_pos + sizeof(T));

			memcpy(&m_buffer[m_write_pos],&v,sizeof(T));
			m_write_pos += sizeof(T);
		}

		template <typename T>
		T read()
		{
			assert(m_read_pos + sizeof(T) <= m_buffer.size());

			T v;
			memcpy(&v,&m_buffer[m_read_pos],sizeof(T));
			m_read_pos += sizeof(T);
			return v;
		}
};