
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
			case (short)Packets.CE_PKT_GO_MOVE:
				handleGameObjectMovePacket(pkt);
				break;

			case (short)Packets.E_PKT_GO_SPAWN:
				handleGameObjectSpawnPacket(pkt);
				break;

			case (short)Packets.S_PKT_GEN_SESSION_KEY:
				handleGenerateSessionKeyPacket(pkt);
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

		Debug.Log ("objID=" + objectID + ",posX=" + posX + ",posY=" + posY + ",posZ=" + posZ + 
		           ",rotX=" + rotX + ",rotY=" + rotY + ",rotZ=" + rotZ);

		// TODO: find prefab by resource id
		GameObject obj = (GameObject) Instantiate(Resources.Load ("Player", typeof(GameObject)));
		obj.name = "" + objectID;
		obj.transform.position.Set(posX, posY, posZ);
		obj.transform.rotation.Set(rotX, rotY, rotZ, rotW);

		Debug.Log("PacketHandler::handleGameObjectSpawnPacket end");
	}

	private static void handleGenerateSessionKeyPacket(Packet p)
	{
		Debug.Log("PacketHandler::handleGenerateSessionKeyPacket start");
		
		// parse packet
		byte[] sessionKey = p.read(20);
		string key = BitConverter<string>.ConvertToGeneric (sessionKey);
		
		Debug.Log("sessionKey=" + key);
		GameObject.Find ("Canvas").SetActive (false);
		Player.isActive = true;

		Debug.Log("PacketHandler::handleGenerateSessionKeyPacket end");
	}
}
