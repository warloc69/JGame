
using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

using UnityEngine;
using UnityEditor;

public class JavaClient : MonoBehaviour 
{
	public static byte[] sessionKey = null;

	// TODO: read from properties
	private static int readPort = 6669;
	private static int sendPort = 6670;
	private static string serverIPAddress = "127.0.0.1";

	private static UdpClient m_client;
	private static Thread m_thread;
	private static Queue m_queue;
	private static bool m_active;

	void Start () 
	{
		m_active = true;
		m_queue = new Queue();
		m_thread = new Thread(new ThreadStart(readPackets));
		m_thread.IsBackground = true;
		m_thread.Start();
	}

	void Update () 
	{
		Packet pkt = null;
		while(m_queue != null && m_queue.pop (ref pkt))
			PacketHandler.handle(pkt);
	}
	
	void OnDestroy()
	{
		if (Player.isActive) {
			sendPacket (PacketBuilder.disconnectPacket ());
		}
		stop();
	}

	public static void stop()
	{
		m_active = false;

		if(m_thread != null && m_thread.IsAlive)
			m_thread.Interrupt();

		if(m_client != null)
			m_client.Close();
	}

	private static void readPackets()
	{
		m_client = new UdpClient(readPort);
		m_client.Client.Blocking = true;

		while(m_active)
		{
			try {
				IPEndPoint anyIP = new IPEndPoint(IPAddress.Any, 0);
				byte[] arr = m_client.Receive(ref anyIP);
				// validate 1st data size byte and last '\n' byte
				if(arr[arr[0] + 3] != '\n')
				{
					Debug.LogWarning("Broken packet, incorrect EOF");
					continue;
				}

				Packet pkt = new Packet(arr);
				m_queue.push(pkt);
			} 
			catch(Exception e) 
			{
				Debug.LogError(e.Message);
			}
		}
	}
	
	public static void sendPacket(Packet p)
	{
		UdpClient server = new UdpClient();
		IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Parse(serverIPAddress), sendPort);

		Logger.log ("Sending packet to Java server :: " + p.toString());
		server.Send(p.getBuffer(), (int) p.size(), ipEndPoint);
	}
}
