using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Net;
using System.Net.Sockets;


public class TestClient : MonoBehaviour {
    public string serverIP = "127.0.0.1";    
    public int  serverPort = 6669;
    public String ReadMsg;    
    public String ReadMsg2;
    private StreamReader sr;
    private NetworkStream ns;
    private bool streamIsReady = false;

    // Use this for initialization
    void Start () {
        var mainPlayer = GameObject.Find("MainPlayer");
        var mainPos = mainPlayer.transform.position;
        Debug.Log("GUIIII I am alive! oooo" + mainPos.ToString());
        try{
            TcpClient tc = new TcpClient(serverIP,serverPort);// in the place of server, enter your java server's hostname or Ip
            Debug.Log("Server invoked");
            ns = tc.GetStream();
            StreamWriter sw = new StreamWriter(ns);
            sw.WriteLine("Server Hello my little bitch! " + mainPos.ToString());
            sw.Flush();            
            sr = new StreamReader(ns);
            streamIsReady = true;
            
        } catch(Exception e){Console.WriteLine(e);}
        
    }
    // Update is called once per frame 
    void Update () {
        if (streamIsReady)
        {
            if (ns.DataAvailable)
            {
                Debug.Log(sr.ReadLine());
            }
        }
    }
}
