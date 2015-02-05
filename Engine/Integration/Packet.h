
#pragma once

#include "baseTypes.h"
#include <vector>

#include <assert.h>

template <typename T>
T SWAP_BYTES(T v) 
{
	T retVal;
	char *pVal = (char*)&v;
	char *pRetVal = (char*)&retVal;
	int size = sizeof(T);
	for(int i = 0; i < size; i++)
		pRetVal[size-1-i] = pVal[i];
	return retVal;
}

//TODO: find a way for use dynamic memory allocation in order to use in queue
class Packet
{
	public:
		static const uint8 PACKET_SIZE = 0xFF;

		Packet()
		{
			m_read_pos = 0;
			m_write_pos = 0;
			m_finalized = false;
			memset(&m_buffer[0], 0, sizeof(m_buffer));
		}

		// TODO: string read/write
		/*Packet& operator<<(std::wstring v)
		{
			uint8 freeSpace = PACKET_SIZE - m_write_pos;
			if(v.size() > freeSpace)
				throw Exception(L"Attempted to write to Packet :: wstring size=" + toWString(v.size() + L", must be <= " + freeSpace));

			memcpy(&m_buffer[m_write_pos],v.c_str(),v.size());
			m_write_pos += v.size();

			return *this;
		}

		Packet& operator>>(std::wstring& v)
		{
			if(m_read_pos + v.size() > PACKET_SIZE)
				throw Exception(L"Attempted to read Packet :: size=" + toWString(PACKET_SIZE));

			wchar_t vv[MAX_TEXT_LENGTH];
			memcpy(&vv,&m_buffer[m_read_pos],MAX_TEXT_LENGTH);
			m_read_pos += MAX_TEXT_LENGTH;

			v = vv;
			return *this;
		}*/

		template <typename T, size_t N>
		Packet& operator<<(T (&v)[N])
		{
			for(int i = 0; i < N; i++)
				append(v[i]);
			return *this;
		}

		template <typename T>
		Packet& operator<<(std::vector<T> v)
		{
			for(T k : v) 
				append(k);
			return *this;
		}

		template <typename T>
		Packet& operator<<(T v)
		{
			append(v);
			return *this;
		}

		template <typename T>
		Packet& operator>>(T& v)
		{
			v = read<T>();
			return *this;
		}

		void resetRead()
		{
			m_read_pos = 0;
		}

		uint8* getBuffer()
		{
			return m_buffer;
		}

		void finalize()
		{
			m_finalized = true;
		}

		uint8 size()
		{
			return m_finalized ? m_write_pos : PACKET_SIZE;
		}

	protected:
		uint8 m_buffer[PACKET_SIZE];
		uint8 m_read_pos;
		uint8 m_write_pos;
		bool m_finalized;

	private:
		template <typename T>
		void append(const T v)
		{
			if(m_finalized)
				return;

			assert(m_write_pos + sizeof(T) <= PACKET_SIZE);

			memcpy(&m_buffer[m_write_pos],&v,sizeof(T));
			m_write_pos += sizeof(T);
		}

		template <typename T>
		T read()
		{
			assert(m_read_pos + sizeof(T) <= PACKET_SIZE);

			T v;
			memcpy(&v,&m_buffer[m_read_pos],sizeof(T));
			m_read_pos += sizeof(T);
			return SWAP_BYTES(v);
		}
};