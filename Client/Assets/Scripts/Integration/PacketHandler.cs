
using System;
using UnityEngine;

public class PacketHandler : MonoBehaviour
{
	public static void handle(Packet pkt)
	{
		pkt.resetRead();
		pkt.read<byte>();//read size byte
		short id = pkt.read<short>();

		switch(id)
		{
			case (short)Packets.E_PKT_GO_MOVE:
				handleGameObjectMovePacket(pkt);
				break;

			case (short)Packets.E_PKT_GO_SPAWN:
				handleGameObjectSpawnPacket(pkt);
				break;

			case (short)Packets.S_PKT_AUTH_RESPONSE:
				handleAuthResponsePacket(pkt);
				break;

			default:
				Debug.Log ("Unknown packet id=" + id);
				break;
		}
	}

	private static void handleGameObjectMovePacket(Packet p)
	{
		Debug.Log("PacketHandler::handleGameObjectMovePacket start");

		// parse packet
		int objectID = p.read<int>();
		float posX = p.read<float>();
		float posY = p.read<float>();
		float posZ = p.read<float>();
		float rotX = p.read<float>();
		float rotY = p.read<float>();
		float rotZ = p.read<float>();
		float rotW = p.read<float>();

		Debug.Log ("objID=" + objectID + ",posX=" + posX + ",posY=" + posY + ",posZ=" + posZ + 
		           ",rotX=" + rotX + ",rotY=" + rotY + ",rotZ=" + rotZ);

		var gameObject = GameObject.Find(""+objectID);
		gameObject.transform.position.Set(posX, posY, posZ);
		gameObject.transform.rotation.Set(rotX, rotY, rotZ, rotW);

		Debug.Log("PacketHandler::handleGameObjectMovePacket end");
	}

	private static void handleGameObjectSpawnPacket(Packet p)
	{
		Debug.Log("PacketHandler::handleGameObjectSpawnPacket start");
		
		// parse packet
		int objectID = p.read<int>();
		short resourceID = p.read<short>();
		float posX = p.read<float>();
		float posY = p.read<float>();
		float posZ = p.read<float>();
		float rotX = p.read<float>();
		float rotY = p.read<float>();
		float rotZ = p.read<float>();
		float rotW = p.read<float>();
		float velX = p.read<float>();
		float velY = p.read<float>();
		float velZ = p.read<float>();

		Debug.Log ("objID=" + objectID + ",posX=" + posX + ",posY=" + posY + ",posZ=" + posZ + 
		           ",rotX=" + rotX + ",rotY=" + rotY + ",rotZ=" + rotZ +
				   ",velX=" + velX + ",velY=" + velY + ",velZ=" + rotZ);

		// TODO: find prefab by resource id
		if(resourceID == 1)
		{
			GameObject obj = (GameObject) Instantiate(Resources.Load ("Player", typeof(GameObject)));
			obj.name = "" + objectID;
			obj.transform.position.Set(posX, posY, posZ);
			obj.transform.rotation.Set(rotX, rotY, rotZ, rotW);

			/// set camera
			Camera.target = obj.transform;

		}
		else if(resourceID == 2)
		{
			DynamicObject obj = (DynamicObject) Instantiate(Resources.Load ("Dynamic", typeof(DynamicObject)));
			obj.name = "d:" + objectID;
			obj.transform.position.Set(posX, posY, posZ);
			obj.transform.rotation.Set(rotX, rotY, rotZ, rotW);
			if(obj.transform.rigidbody != null)
				obj.transform.rigidbody.velocity.Set(velX, velY, velZ);
		}

		Debug.Log("PacketHandler::handleGameObjectSpawnPacket end");
	}

	private static void handleAuthResponsePacket(Packet p)
	{
		//Debug.Log("PacketHandler::handleAuthResponsePacket start");
		
		// parse packet
		byte authResponse = p.read<byte> ();
		switch(authResponse)
		{
			case (byte)AuthorizationResults.AUTH_ALREADY_CONNECTED:
				InputFieldUI.showWarningPopup("Already connected");
				break;
			case (byte)AuthorizationResults.AUTH_BANNED:
				InputFieldUI.showWarningPopup("Banned");
				break;
			case (byte)AuthorizationResults.AUTH_DISCONNECTED:
				InputFieldUI.showWarningPopup("Disconnected");
				JavaClient.sessionKey = null;
				GameObject.Find ("Canvas").SetActive (true);
				Player.isActive = false;
				break;
			case (byte)AuthorizationResults.AUTH_NOT_REGISTERED:
				InputFieldUI.showWarningPopup("Not registered");
				break;
			case (byte)AuthorizationResults.AUTH_REGISTRATION_SUCCESS:
				InputFieldUI.showWarningPopup("Registered");
				break;
			case (byte)AuthorizationResults.AUTH_ALREADY_EXIST:
				InputFieldUI.showWarningPopup("Login already exists");
				break;
			case (byte)AuthorizationResults.AUTH_CONNECTION_SUCCESS:
				InputFieldUI.showWarningPopup("Connected");
				JavaClient.sessionKey = p.read(32);

				/// spawn object request
				JavaClient.sendPacket(PacketBuilder.gameObjectSpawn(1, new Vector3(0,10,0), new Vector4(0,0,0,1), new Vector3()));
				
			    GameObject.Find ("Canvas").SetActive (false);
				Player.isActive = true;
				break;
		}


		//Debug.Log("PacketHandler::handleAuthResponsePacket end");
	}

}
