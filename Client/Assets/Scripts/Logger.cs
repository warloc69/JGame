using UnityEngine;
using System.Collections; 
using System;
using System.IO;

public class Logger : MonoBehaviour{ 
	  
	// Use this for initialization 
    public static void log(string info) {
		System.IO.StreamWriter logFile = new System.IO.StreamWriter(@"log.txt",true); 
        logFile.WriteLine(info); 
        logFile.Flush();
		logFile.Dispose ();
		Debug.Log(info);
	}
}
