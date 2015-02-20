using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

/**
 * Represents packet of data
 */
public class Packet
{
	private const byte PACKET_SIZE = 0xFF;
	private byte[] m_buffer;
	private byte m_read_pos;
	private byte m_write_pos;
	private bool m_finalized;

	public Packet()
	{
		m_buffer = new byte[PACKET_SIZE];
		m_read_pos = 0;
		m_write_pos = 0;
		m_finalized = false;
	}

	public Packet(byte[] arr)
	{
		m_buffer = new byte[PACKET_SIZE];
		m_read_pos = 0;
		m_write_pos = 0;
		m_finalized = false;
		write(arr);
	}

	public void write(string v, int size)
	{
		byte[] arr = BitConverter<string>.GetBytes(v);
		Array.Resize(ref arr, size);
		write (arr);
	}

	public void write<T>(T v)
	{
		if(m_finalized)
			return;

		int size = Marshal.SizeOf (typeof(T));
		Debug.Assert(m_write_pos + size <= PACKET_SIZE);

		byte[] src = BitConverter<T>.GetBytes(v);
		
		if(!BitConverter.IsLittleEndian)
			Array.Reverse(src);

		Buffer.BlockCopy(src, 0, m_buffer, m_write_pos, size);
		m_write_pos += (byte) size;
	}

	public byte[] read(int size)
	{
		byte[] arr = new byte[size];
		Buffer.BlockCopy(m_buffer, m_read_pos, arr, 0, size);
		m_read_pos += (byte) size;
		return arr;
	}

	public T read<T>()
	{
		int size = Marshal.SizeOf (typeof(T));
		Debug.Assert(m_read_pos + size <= PACKET_SIZE);
		
		byte[] dest = new byte[size];
		Buffer.BlockCopy(m_buffer, m_read_pos, dest, 0, size);
		m_read_pos += (byte) size;

		T v = BitConverter<T>.ConvertToGeneric(dest);
		return v;
	}

	public void write<T>(T[] v)
	{
		for(int i = 0; i < v.Length; i++)
			write(v[i]);
	}
	
	public void resetRead()
	{
		m_read_pos = 0;
	}
	
	public byte[] getBuffer()
	{
		return m_buffer;
	}
	
	public void finalize()
	{
		write<byte>((byte)'\n');
		m_finalized = true;
		Array.Resize(ref m_buffer, m_write_pos);
	}
	
	public byte size()
	{
		return m_finalized ? m_write_pos : PACKET_SIZE;
	}

	// temp
	public string toString()
	{
		string str = "";
		for(int i = 0; i < size (); i++)
		{
			str += m_buffer[i].ToString() + ",";
		}
		return str;
	}
}