// SPDX-FileCopyrightText: 2021-2025 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

// Useful stuff:
// https://docs.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting
// https://github.com/dotnet/runtime/blob/main/docs/design/features/native-hosting.md

// hostfxr.h are at first confusing to find. You can find them in the example code from the .NET Core hosting tutorial, or:
// Simply download some packages via commandline NuGet:
// nuget.exe install runtime.win-x64.Microsoft.NETCore.DotNetAppHost
// There are also win-x86 and linux-x64. Doesn't look like there's an x86 Linux one, so oof
// It might be possible to build an x86 one though: https://github.com/dotnet/runtime/tree/main/src/native/corehost
#include "DotNet/coreclr_delegates.h"
#include "DotNet/hostfxr.h"

#include <string_view>

#define SM5_CALL CORECLR_DELEGATE_CALLTYPE

namespace smv
{
	// Shorten these names
	// Used by Host to load function pointers for assemblies
	using fxrInit_f = hostfxr_initialize_for_runtime_config_fn;
	using fxrGetDelegate_f = hostfxr_get_runtime_delegate_fn;
	using fxrClose_f = hostfxr_close_fn;
	using fxrSetErrorWriter_f = hostfxr_set_error_writer_fn;
	//using fxrSetErrorWriter_f = hostfxr_error_writer_fn __stdcall(hostfxr_error_writer_fn);
	//typedef hostfxr_error_writer_fn(__stdcall *fxrSetErrorWriter_f)(hostfxr_error_writer_fn error_writer);
	
	// Used by Assembly to get function pointers to C# methods
	using clrLoadAssembly_f = load_assembly_fn;
	using clrGetFunctionPointer_f = get_function_pointer_fn;

	class Assembly;

	class Host final
	{
	public:
		static bool Init( std::string_view runtimeConfigPath );
		static void Shutdown();

		// Loads the managed assembly.
		// @param assemblyPath: Must not contain the extension!
		// @return nullptr if it cannot be found, otherwise a proper assembly reference
		static Assembly* LoadAssembly( std::string_view assemblyPath );

		// Finds a method and gets a function pointer to it. This is best called from Assembly.
		static void* GetMethod( std::string_view namespaceName, std::string_view typeName, std::string_view methodName );

	private:
		// Hostfxr loading utility
		static bool LoadHostFxr( std::string_view runtimeConfigPath );

		// HostFxr API
		// Initialises a HostFxr context with a given runtime configuration
		static inline fxrInit_f fxrInit{ nullptr };
		// Obtains functions from NetHost, kinda like GetProcAddress or dlopen
		static inline fxrGetDelegate_f fxrGetDelegate{ nullptr };
		// Closes an fxr context
		static inline fxrClose_f fxrClose{ nullptr };
		// Sets the logger for whatever error messages we get from .NET
		static inline fxrSetErrorWriter_f* fxrSetErrorWriter{ nullptr };

		// .NET runtime context
		static inline hostfxr_handle clrContext{ nullptr };
		// Loads a C# DLL file
		static inline clrLoadAssembly_f clrLoadAssembly{ nullptr };
		// Obtains a method from a C# DLL
		static inline clrGetFunctionPointer_f clrGetFunctionPointer{ nullptr };
	};
}
