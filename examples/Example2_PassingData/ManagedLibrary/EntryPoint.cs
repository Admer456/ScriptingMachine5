// SPDX-FileCopyrightText: 2025 Admer Šuko
// SPDX-License-Identifier: MIT

using System.Numerics;
using System.Runtime.InteropServices;

namespace ManagedLibrary;

public static class EntryPoint
{
	[StructLayout( LayoutKind.Sequential )]
	public struct NativeVector3
	{
		public float X;
		public float Y;
		public float Z;
	}
	
	[UnmanagedCallersOnly]
	public static void IntMethod( int value )
	{
		Console.WriteLine( $"[C#][IntMethod] Received: {value}" );
	}

	[UnmanagedCallersOnly]
	public static void FloatMethod( float value )
	{
		Console.WriteLine( $"[C#][FloatMethod] Received: {value}" );
	}

	[UnmanagedCallersOnly]
	public static unsafe void Vector3Method( Vector3* ptr )
	{
		Console.WriteLine( $"[C#][Vector3Method] Received: ({ptr->X}, {ptr->Y}, {ptr->Z})" );
	}

	[UnmanagedCallersOnly]
	public static unsafe int ComplexMethod( int* outValue )
	{
		Console.WriteLine( $"[C#][ComplexMethod] Received: {*outValue}, changing it to 256..." );
		*outValue = 256;
		Console.WriteLine( $"[C#][ComplexMethod] Returning 1024" );
		return 1024;
	}

}
