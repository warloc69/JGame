
using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

using UnityEngine;

public class JavaClient : MonoBehaviour 
{
	// TODO: read from properties
	private static string host = "127.0.0.1";    
	private static int port = 6669;

	private static TcpClient client;
	private static Thread thread;

	private static Queue m_queue;

	public static bool isConnected()
	{
		return client != null && client.Connected;
	}

	private static void disconnect()
	{
		if(client != null)
			client.Close ();
		Debug.Log ("Disconnected from Java server");

		if(thread != null)
			thread.Interrupt();
	}

	private static void readPacketsFromServer()
	{
		NetworkStream stream = client.GetStream();
		while(isConnected() && stream.CanRead)
		{
			int len = stream.ReadByte();
			if(len == -1)
				break;
			if(len <= 0)
				continue;

			byte[] arr = new byte[len];
			stream.Read(arr, 0, len);
			if(stream.ReadByte() != '\n')
			{
				Debug.LogError("Broken packet, incorrect EOF");
				break;
			}
			
			Packet pkt = new Packet(arr);
			m_queue.push(pkt);
		}

		disconnect ();
	}
	
	public static void sendPacket(Packet p)
	{
		if(!isConnected())
			return;
		
		Logger.log ("Sending packet to Java server :: " + p.toString());
		NetworkStream stream = client.GetStream();
		stream.Write(p.getBuffer(), 0, (int) p.size());
		stream.Flush();
	}

	void Start () 
	{
		m_queue = new Queue();
		try
		{
			Debug.Log("Connecting to Java server......");
			client = new TcpClient(host, port);
			Debug.Log("Connected to Java server");

			thread = new Thread (new ThreadStart (readPacketsFromServer));
			thread.Start ();
		} 
		catch(Exception e)
		{
			Debug.LogError(e.Message);
			disconnect ();
		}
	}
	
	// Update is called once per frame 
	void Update () 
	{
		Packet pkt = null;
		while(m_queue.pop(ref pkt))
		{
			PacketHandler.handle(pkt);
		}
	}

	void OnDisable()
	{
		disconnect ();
	}
}
