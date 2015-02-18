
using System;
using UnityEngine;

public class PacketBuilder
{
	public static Packet moveAndRotate(int objectID, Vector3 pos, Vector4 rot)
	{
		Packet p = new Packet();

		p.write<byte>(30);		// 1-packet size
		p.write<short>((short)Packets.PACKET_MOVE_GAME_OBJECT);		// 2-packet id
		p.write<int>(objectID);		// 4-object id
		p.write<float>(pos.x);// 4-pos x
		p.write<float>(pos.y);// 4-pos y
		p.write<float>(pos.z);// 4-pos z
		p.write<float>(rot.x);	// 4-rot x
		p.write<float>(rot.y);	// 4-rot y
		p.write<float>(rot.z);	// 4-rot z
		p.finalize();

		return p;
	}
}
