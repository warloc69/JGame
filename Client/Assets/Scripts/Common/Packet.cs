using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

/// Хранит массив байтов, осуществляет удобные чтение/запись пакета
public class Packet
{
    /// Максимальный размер пакета
	private const byte PACKET_SIZE = 0xFF;

    /// Массив данных пакета
	private byte[] m_buffer;

    /// Позиция чтения в пакете
	private byte m_read_pos;

    /// Позиция записи в пакете
	private byte m_write_pos;

    /// Флаг финализации пакета. Это значит, что в пакет записать уже ничего нельзя и он готов к отправке на сервер
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

    /// Базовый метод чтения данных из пакета. Работает с большинством типов переменных
    public T read<T>()
    {
        int size = Marshal.SizeOf(typeof(T));
        Debug.Assert(m_read_pos + size <= PACKET_SIZE);

        byte[] dest = new byte[size];
        Buffer.BlockCopy(m_buffer, m_read_pos, dest, 0, size);
        m_read_pos += (byte)size;

        T v = BitConverter<T>.ConvertToGeneric(dest);
        return v;
    }

    /// Читает определенное количество байт пакета
    public byte[] read(int size)
    {
        byte[] arr = new byte[size];
        Buffer.BlockCopy(m_buffer, m_read_pos, arr, 0, size);
        m_read_pos += (byte)size;
        return arr;
    }

    /// Базовый метод записи данных в пакет. Работает с большинством типов переменных
    public void write<T>(T v)
    {
        if (m_finalized)
            return;

        int size = Marshal.SizeOf(typeof(T));
        Debug.Assert(m_write_pos + size <= PACKET_SIZE);

        byte[] src = BitConverter<T>.GetBytes(v);

        if (!BitConverter.IsLittleEndian)
            Array.Reverse(src);

        Buffer.BlockCopy(src, 0, m_buffer, m_write_pos, size);
        m_write_pos += (byte)size;
    }

    /// Записывает массив популярных переменных в пакет
    public void write<T>(T[] v)
    {
        for (int i = 0; i < v.Length; i++)
            write(v[i]);
    }

    /// Записывает определенное количество байт строки
	public void write(string v, int size)
	{
		byte[] arr = BitConverter<string>.GetBytes(v);
		Array.Resize(ref arr, size);
		write (arr);
	}

    /// Сбрасывает счетчик чтения пакета
	public void resetRead()
	{
		m_read_pos = 0;
	}
	
    /// Возвращает буфер пакета
	public byte[] getBuffer()
	{
		return m_buffer;
	}
	
    /// Финализирует пакет. Добавляет финальный байт в конце. Изменяет размер пакета, готового для отправки серверу.
	public void finalize()
	{
		write<byte>((byte)'\n');
		m_finalized = true;
		Array.Resize(ref m_buffer, m_write_pos);
	}
	
    /// Возвращает размер пакета
	public byte size()
	{
		return m_finalized ? m_write_pos : PACKET_SIZE;
	}

	/// Преобразовывает в строку данные пакета
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