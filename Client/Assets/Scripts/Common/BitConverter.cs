
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

public class BitConverter<T>
{
	public static Func<T, byte[]> GetBytes = x => new byte[] {};
	public static Func<byte[], T> ConvertToGeneric;

	static BitConverter()
	{
		BitConverter<byte>.GetBytes   = x => new byte[] { x };
		BitConverter<bool>.GetBytes   = x => BitConverter.GetBytes(x);
		BitConverter<char>.GetBytes   = x => BitConverter.GetBytes(x);
		BitConverter<Int16>.GetBytes  = x => BitConverter.GetBytes(x);
		BitConverter<Int32>.GetBytes  = x => BitConverter.GetBytes(x);
		BitConverter<Int64>.GetBytes  = x => BitConverter.GetBytes(x);
		BitConverter<UInt16>.GetBytes = x => BitConverter.GetBytes(x);
		BitConverter<UInt32>.GetBytes = x => BitConverter.GetBytes(x);
		BitConverter<UInt64>.GetBytes = x => BitConverter.GetBytes(x);
		BitConverter<float>.GetBytes  = x => BitConverter.GetBytes(x);
		BitConverter<double>.GetBytes = x => BitConverter.GetBytes(x);
		BitConverter<string>.GetBytes = x => Encoding.ASCII.GetBytes(x);

		BitConverter<byte>.ConvertToGeneric   = (arr) => arr[0];
		BitConverter<bool>.ConvertToGeneric   = (arr) => BitConverter.ToBoolean(arr, 0);
		BitConverter<char>.ConvertToGeneric   = (arr) => BitConverter.ToChar(arr, 0);
		BitConverter<Int16>.ConvertToGeneric  = (arr) => BitConverter.ToInt16(arr, 0);
		BitConverter<Int32>.ConvertToGeneric  = (arr) => BitConverter.ToInt32(arr, 0);
		BitConverter<Int64>.ConvertToGeneric  = (arr) => BitConverter.ToInt64(arr, 0);
		BitConverter<UInt16>.ConvertToGeneric = (arr) => BitConverter.ToUInt16(arr, 0);
		BitConverter<UInt32>.ConvertToGeneric = (arr) => BitConverter.ToUInt32(arr, 0);
		BitConverter<UInt64>.ConvertToGeneric = (arr) => BitConverter.ToUInt64(arr, 0);
		BitConverter<float>.ConvertToGeneric  = (arr) => BitConverter.ToSingle(arr, 0);
		BitConverter<double>.ConvertToGeneric = (arr) => BitConverter.ToDouble(arr, 0);
		BitConverter<string>.ConvertToGeneric = (arr) => Encoding.ASCII.GetString(arr);
	}
}