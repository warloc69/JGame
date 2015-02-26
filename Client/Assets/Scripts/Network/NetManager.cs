using UnityEngine;
using System.Collections;

public class NetManager : MonoBehaviour 
{
    public int authorizationPort = 6669;
    public int worldPort = 6671;
    public string serverIp = "127.0.0.1";

    private static AsyncUdpClient authClient;
    private static AsyncUdpClient worldClient;

	void Start () 
    {
        authClient = new AuthorizationClient(authorizationPort, serverIp);
        worldClient = new WorldClient(worldPort, serverIp);
	}
	
	void Update () 
    {
        authClient.Update();
        worldClient.Update();
	}

    void OnDestroy()
    {
        authClient.disconnect();
        worldClient.disconnect();
    }

    public static void sendWorldPacket(Packet p)
    {
        worldClient.sendPacket(p);
    }

    public static void sendAuthorizationPacket(Packet p)
    {
        authClient.sendPacket(p);
    }
}
