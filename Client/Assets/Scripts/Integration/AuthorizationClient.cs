
using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

using UnityEngine;
using UnityEditor;

/// Определяет сетевую прослойку через порт авторизации на сервер.
/// Обмен пакетами происходит асинхронно. Все входящие пакеты от сервера складываются в очередь,
/// параллельно читаются и обрабатываются в основном треде.
public class AuthorizationClient : MonoBehaviour 
{
	public static byte[] sessionKey = null;

	// TODO: read from properties
	private static short m_port = 6669;
	private static string m_serverIPAddress = "127.0.0.1";
    //

    private static IPEndPoint m_receiveEndPoint;
    private static UdpClient m_receiveClient;
    private static UdpClient m_sendClient;
    private static Queue m_queue;

	void Start () 
	{
        m_queue = new Queue();

        /// if server is on local machine => client will use +1 authorization port
        short port = m_serverIPAddress == "127.0.0.1" ? (short)(m_port + 1) : m_port;
        m_receiveEndPoint = new IPEndPoint(IPAddress.Parse(m_serverIPAddress), port);
        m_receiveClient = new UdpClient(m_receiveEndPoint);
        m_sendClient = new UdpClient();
	}

	void Update() 
	{
        Packet pkt = null;
        while (m_queue != null && m_queue.pop(ref pkt))
            PacketHandler.handle(pkt);
	}
	
	void OnDestroy()
	{
        disconnect();
	}

    private static void disconnect()
    {
        if (m_receiveClient != null)
        {
            try { m_receiveClient.Close(); }
            catch (Exception e) { Debug.Log(e.Message); }
        }

        if (m_sendClient != null)
        {
            try { m_sendClient.Close(); }
            catch (Exception e) { Debug.Log(e.Message); }
        }
    }

    /// Асинхронно принимает пакеты сервера
    private static void receive()
    {
        Debug.Log("Establishing connection to " + m_serverIPAddress + ":" + m_receiveEndPoint.Port + "..");
        m_receiveClient.BeginReceive(new AsyncCallback(on_receive), null);
    }

    /// Срабатывает по приёму сообщения от сервера, парсит, валидирует массив, обворачивает его в пакет и закидывает в очередь
    private static void on_receive(IAsyncResult r)
    {
        Debug.Log("Connected to " + m_serverIPAddress + "..");

        byte[] arr = m_receiveClient.EndReceive(r, ref m_receiveEndPoint);

        /// parse packet size
        byte data_size = arr[0];

        /// validate packet
        if(data_size != (arr.Length - 4) || arr[arr.Length - 1] != (byte)'\n')
        {
            Debug.LogWarning("Broken packet, incorrect size or EOF\n");
            return;
        }

        /// wrap packet
        byte[] data = new byte[data_size + 2];
        Array.Copy(arr, 1, data, 0, data_size + 2);
        Packet pkt = new Packet(data);

        /// save packet to queue
        m_queue.push(pkt);
    }

    /// Асинхронно отправляет сгенерированный пакет на сервер
    public static void sendPacket(Packet p)
	{
        /// if server is on remote machine => client will use receive authorization port
        short port = m_serverIPAddress != "127.0.0.1" ? (short)(m_port + 1) : m_port;
        IPEndPoint sendEndPoint = new IPEndPoint(IPAddress.Parse(m_serverIPAddress), port);

        Debug.Log("Sending packet to " + m_serverIPAddress + ":" + sendEndPoint.Port + " packet=" + p.toString());
        m_sendClient.BeginSend(p.getBuffer(), (int)p.size(), sendEndPoint, new AsyncCallback(on_send), null);
	}

    /// Срабатывает по отправке пакета серверу, и возвращается к чтению следующих пакетов
    private static void on_send(IAsyncResult r)
    {
        m_sendClient.EndSend(r);

        receive();
    }
}
