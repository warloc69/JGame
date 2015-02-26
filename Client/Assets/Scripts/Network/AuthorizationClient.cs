
using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

using UnityEngine;
using UnityEditor;

public class AuthorizationClient : AsyncUdpClient 
{
	public static byte[] sessionKey = null;

    public AuthorizationClient(int port, string ip) : base(port, ip)
    {
    }
}
