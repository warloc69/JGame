
using System;
using UnityEngine;

public class PacketBuilder
{
	public static Packet gameObjectMove(int objectID, Vector3 pos, Vector4 rot)
	{
		Packet p = new Packet();

		p.write<byte>(32);		// 1-packet size
		p.write<short>((short)Packets.CE_PKT_GO_MOVE);		// 2-packet id
		p.write<int>(objectID);		// 4-object id
		p.write<float>(pos.x);// 4-pos x
		p.write<float>(pos.y);// 4-pos y
		p.write<float>(pos.z);// 4-pos z
		p.write<float>(rot.x);	// 4-rot x
		p.write<float>(rot.y);	// 4-rot y
		p.write<float>(rot.z);	// 4-rot z
		p.write<float>(rot.w);	// 4-rot w
		p.finalize();

		return p;
	}

	public static Packet gameObjectFire(int objectID, short armsID, Vector3 direction)
	{
		Packet p = new Packet ();

		p.write<byte> (18);
		p.write<short>((short)Packets.CE_PKT_GO_FIRE);
		p.write<int> (objectID);
		p.write<short> (armsID);
		p.write<float> (direction.x);
		p.write<float> (direction.y);
		p.write<float> (direction.z);
		p.finalize ();

		return p;
	}

	public static Packet authRequestPacket(string clientName, string password, byte authRequest)
	{
		Packet p = new Packet ();

		p.write<byte> (41);
		p.write<short> ((short)Packets.C_PKT_AUTH_REQUEST);
		p.write(clientName, 20);
		p.write(password, 20);
		p.write<byte> (authRequest);
		p.finalize ();

		return p;
	}
}
