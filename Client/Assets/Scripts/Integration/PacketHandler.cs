
using System;
using UnityEngine;

public class PacketHandler : MonoBehaviour
{
	public static void handle(Packet pkt)
	{
		pkt.resetRead ();
		short id = pkt.read<short>();

		switch(id)
		{
			case (short)Packets.PACKET_MOVE_GAME_OBJECT:
				handleMoveAndRotatePacket(pkt);
				break;

			case (short)Packets.PACKET_SPAWN_GAME_OBJECT:
				handleSpawnObjectPacket(pkt);
				break;

			default:
				Debug.Log ("Unknown packet");
				break;
		}
	}

	private static void handleMoveAndRotatePacket(Packet p)
	{
		Debug.Log("PacketHandler::handleMoveAndRotatePacket start");

		// parse packet
		int objectID = p.read<int>();
		float posX = p.read<float>();
		float posY = p.read<float>();
		float posZ = p.read<float>();
		float rotX = p.read<float>();
		float rotY = p.read<float>();
		float rotZ = p.read<float>();

		Debug.Log ("objID=" + objectID + ",posX=" + posX + ",posY=" + posY + ",posZ=" + posZ + 
		           ",rotX=" + rotX + ",rotY=" + rotY + ",rotZ=" + rotZ);

		// TODO: search by object id
		var gameObject = GameObject.Find("MainPlayer");
		gameObject.transform.position.Set(posX, posY, posZ);
		gameObject.transform.rotation.Set(rotX, rotY, rotZ, 0);

		Debug.Log("PacketHandler::handleMoveAndRotatePacket end");
	}

	private static void handleSpawnObjectPacket(Packet p)
	{
		Debug.Log("PacketHandler::handleSpawnObjectPacket start");
		
		// parse packet
		int objectID = p.read<int>();
		short type = p.read<short>();
		float posX = p.read<float>();
		float posY = p.read<float>();
		float posZ = p.read<float>();
		float rotX = p.read<float>();
		float rotY = p.read<float>();
		float rotZ = p.read<float>();

		Debug.Log ("objID=" + objectID + ",posX=" + posX + ",posY=" + posY + ",posZ=" + posZ + 
		           ",rotX=" + rotX + ",rotY=" + rotY + ",rotZ=" + rotZ);
		
		GameObject obj = (GameObject) Instantiate(Resources.Load ("Cubez", typeof(GameObject)));
		obj.name = "C++ object_" + objectID;
		obj.transform.position.Set(posX, posY, posZ);
		obj.transform.rotation.Set(rotX, rotY, rotZ, 0);

		Packet answer = PacketBuilder.moveAndRotate(objectID, new Vector3(12,13,14), new Vector4(1,2,3,4));
		JavaClient.sendPacket(answer);

		Debug.Log("PacketHandler::handleSpawnObjectPacket end");
	}
}
