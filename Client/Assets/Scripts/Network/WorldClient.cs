
using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

using UnityEngine;
using UnityEditor;

public class WorldClient : AsyncUdpClient
{
    public WorldClient(int port, string ip) : base(port, ip)
    {
    }
}
