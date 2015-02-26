
using System;
using System.Threading;
using UnityEngine;

public class Queue
{
	private const int QUEUE_SIZE = 1000;
	private static Mutex m_mutex = new Mutex();
	private Packet[] m_list = new Packet[QUEUE_SIZE];
	private int m_reader = 0;
	private int m_writer = 0;
	private int m_size = 0;

	public void push(Packet v)
	{
		m_mutex.WaitOne();
		
		if(m_size == QUEUE_SIZE)
		{
			m_mutex.ReleaseMutex();
			Debug.LogWarning("No free space in queue. All packets are ignored.");
			return;
		}
		
		m_list[m_writer] = v;
		m_writer = m_writer == QUEUE_SIZE - 1 ? 0 : m_writer + 1;
		m_size++;
		
		m_mutex.ReleaseMutex();
	}
	
	public bool pop(ref Packet v)
	{
		m_mutex.WaitOne();
		
		if(m_size == 0)
		{
			m_mutex.ReleaseMutex();
			return false;
		}
		
		v = m_list[m_reader];
		m_reader = m_reader == QUEUE_SIZE - 1 ? 0 : m_reader + 1;
		m_size--;
		
		m_mutex.ReleaseMutex();
		
		return true;
	}
}
