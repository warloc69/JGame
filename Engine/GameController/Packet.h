
#pragma once

#include "baseTypes.h"
#include <string>
#include <vector>
#include "Exception.h"
#include "Logger.h"

class Packet
{
	public:
		static const uint16 DEFAULT_SIZE = 0xFFFF;

		Packet()
		{
			m_buffer.resize(DEFAULT_SIZE);
			m_read_pos = 0;
			m_write_pos = 0;
		}

		Packet(uint32 size)
		{
			m_buffer.resize(size);
			m_read_pos = 0;
			m_write_pos = 0;
		}

		Packet& operator<<(std::wstring v)
		{
			if(v.size() > MAX_TEXT_LENGTH)
				throw Exception(L"Attempted to write to Packet :: wstring size=" + toWString(v.size() + L", must be <= " + MAX_TEXT_LENGTH));

			if(m_buffer.size() < m_write_pos + MAX_TEXT_LENGTH)
				m_buffer.resize(m_buffer.size() + MAX_TEXT_LENGTH);

			memcpy(&m_buffer[m_write_pos],v.c_str(),MAX_TEXT_LENGTH);
			m_write_pos += MAX_TEXT_LENGTH;

			return *this;
		}

		Packet& operator>>(std::wstring& v)
		{
			if(m_read_pos + MAX_TEXT_LENGTH > m_buffer.size())
				throw Exception(L"Attempted to read Packet :: size=" + toWString(m_buffer.size()));

			wchar_t vv[MAX_TEXT_LENGTH];
			memcpy(&vv,&m_buffer[m_read_pos],MAX_TEXT_LENGTH);
			m_read_pos += MAX_TEXT_LENGTH;

			v = vv;
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

		void resetWrite()
		{
			m_write_pos = 0;
		}

	protected:
		std::vector<uint8> m_buffer;
		uint32 m_read_pos;
		uint32 m_write_pos;

		static const uint32 MAX_TEXT_LENGTH = 256;

	private:
		template <typename T>
		void append(const T v)
		{
			if(m_buffer.size() < m_write_pos + sizeof(T))
				m_buffer.resize(m_buffer.size() + sizeof(T));

			memcpy(&m_buffer[m_write_pos],&v,sizeof(T));
			m_write_pos += sizeof(T);
		}

		template <typename T> T read()
		{
			if(m_read_pos + sizeof(T) > m_buffer.size())
				throw Exception(L"Attempted to read Packet :: size=" + toWString(m_buffer.size()));

			T v;
			memcpy(&v,&m_buffer[m_read_pos],sizeof(T));
			m_read_pos += sizeof(T);
			return v;
		}
};