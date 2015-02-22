
using System;
using System.Security.Cryptography;
using System.Text;
using UnityEngine;

public class PacketBuilder
{
	public static Packet gameObjectMove(Vector3 pos, Quaternion rot)
	{
		Packet p = new Packet();

		p.write<byte>(28+32);		// 1-packet size
		p.write<short>((short)Packets.C_PKT_GO_MOVE);		// 2-packet id
		p.write (JavaClient.sessionKey);
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

	public static Packet gameObjectFire(short armsID, Vector3 direction)
	{
		Packet p = new Packet ();

		p.write<byte> (14+32);
		p.write<short>((short)Packets.C_PKT_GO_FIRE);
		p.write (JavaClient.sessionKey);
		p.write<short> (armsID);
		p.write<float> (direction.x);
		p.write<float> (direction.y);
		p.write<float> (direction.z);
		p.finalize ();

		return p;
	}

	public static Packet gameObjectSpawn(short resourceID, Vector3 position, Vector4 rotation, Vector3 velocity)
	{
		Packet p = new Packet ();
		
		p.write<byte> (42+32);
		p.write<short>((short)Packets.C_PKT_GO_SPAWN);
		p.write (JavaClient.sessionKey);
		p.write<short> (resourceID);
		p.write<float> (position.x);
		p.write<float> (position.y);
		p.write<float> (position.z);
		p.write<float> (rotation.x);
		p.write<float> (rotation.y);
		p.write<float> (rotation.z);
		p.write<float> (rotation.w);
		p.write<float> (velocity.x);
		p.write<float> (velocity.y);
		p.write<float> (velocity.z);
		p.finalize ();
		
		return p;
	}

	public static Packet authRequestPacket(string clientName, string password, byte authRequest)
	{
		/// convert password
		MD5 md5Hash = MD5.Create ();
		byte[] data = md5Hash.ComputeHash(Encoding.ASCII.GetBytes(password.Trim()));
		StringBuilder sb = new StringBuilder();
		for (int i = 0; i < data.Length; i++)
			sb.Append(data[i].ToString("x2"));
		///

		Packet p = new Packet ();

		p.write<byte> (53);
		p.write<short> ((short)Packets.C_PKT_AUTH_REQUEST);
		p.write(clientName, 20);
		p.write(sb.ToString(), 32);
		p.write<byte> (authRequest);
		p.finalize ();

		return p;
	}

	public static Packet disconnectPacket()
	{   
		Packet p = new Packet ();
		p.write<byte> (0 + 32);
		p.write<short> ((short)Packets.C_PKT_DISCONNECT);
		p.write (JavaClient.sessionKey);
		p.finalize ();
		return p;
	}
}
