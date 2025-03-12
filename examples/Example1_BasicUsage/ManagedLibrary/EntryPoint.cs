// SPDX-FileCopyrightText: 2025 Admer Šuko
// SPDX-License-Identifier: MIT

using System.Runtime.InteropServices;

namespace ManagedLibrary;

public static class EntryPoint
{
	[UnmanagedCallersOnly]
	public static void ExampleMethod()
	{
		Console.WriteLine( "Hello from .NET land!" );
	}
}
