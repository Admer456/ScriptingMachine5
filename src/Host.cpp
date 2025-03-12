// SPDX-FileCopyrightText: 2025 Admer Šuko
// SPDX-License-Identifier: MIT

#include "Host.hpp"
#include "Assembly.hpp"
#include "Utility.hpp"
#include "ErrorWriter.hpp"
#include "Library.hpp"

#include <array>
#include <filesystem>

#if ADM_PLATFORM == PLATFORM_WINDOWS
#include <ShlObj_core.h>
#else
#include <dlfcn.h>
#endif

using namespace sm5;

bool Host::Init( std::string_view runtimeConfigPath )
{
	// Load hostfxr
	if ( !LoadHostFxr( runtimeConfigPath ) )
	{
		return false;
	}

	// Register the error writer
	ErrorWriter::Register( fxrSetErrorWriter );

	return true;
}

void Host::Shutdown()
{
	fxrClose( clrContext );
}

Assembly* Host::LoadAssembly( std::string_view assemblyPath )
{
	std::string absolutePath = absolute( std::filesystem::path( assemblyPath ) ).string();
	std::wstring fullPathW;
	StringToWString( absolutePath, fullPathW );

	// Load the assembly
	int resultCode = clrLoadAssembly(
		fullPathW.c_str(), // path/to/library.dll
		nullptr, nullptr
	);

	if ( resultCode )
	{
		return nullptr;
	}

	return new Assembly( assemblyPath );
}

void* Host::GetMethod( std::string_view namespaceName, std::string_view typeName, std::string_view methodName )
{
	void* result{ nullptr };
	std::wstring namespaceNameW;
	std::wstring typeNameW;
	std::wstring methodNameW;

	StringToWString( namespaceName, namespaceNameW );
	StringToWString( typeName, typeNameW );
	StringToWString( methodName, methodNameW );

	// .NET needs this. Sigh
	typeNameW = namespaceNameW + L"." + typeNameW + L", " + namespaceNameW;

	int resultCode = clrGetFunctionPointer(
		typeNameW.c_str(),
		methodNameW.c_str(),
		UNMANAGEDCALLERSONLY_METHOD,
		nullptr,
		nullptr,
		&result );

	if ( resultCode )
	{
		return nullptr;
	}

	return result;
}

#if ADM_PLATFORM == PLATFORM_WINDOWS
constexpr const char* gHostFxrFilename = "hostfxr.dll";
#else
constexpr const char* gHostFxrFilename = "libhostfxr.so";
#endif

// Adapted from Coral:
// https://github.com/StudioCherno/Coral/blob/84669efe5e705461b11dd9b76c05b38c6b662796/Coral.Native/Source/Coral/HostInstance.cpp#L129
static std::optional<std::string> FindHostFxr()
{
#if ADM_PLATFORM == PLATFORM_WINDOWS
	std::filesystem::path basePath = "";

	// Find the Program Files folder
	TCHAR pf[MAX_PATH];
	SHGetSpecialFolderPath(
		nullptr,
		pf,
		CSIDL_PROGRAM_FILES,
		FALSE );

	basePath = pf;
	basePath /= "dotnet/host/fxr/";

	auto searchPaths = std::array
	{
		basePath
	};
#elif ADM_PLATFORM == PLATFORM_LINUX
	auto searchPaths = std::array
	{
		std::filesystem::path( "/usr/lib/dotnet/host/fxr/" ),
		std::filesystem::path( "/usr/share/dotnet/host/fxr/" )
	};
#endif

	for ( const std::filesystem::path& path : searchPaths )
	{
		if ( !exists( path ) )
			continue;

		for ( const auto& dir : std::filesystem::recursive_directory_iterator( path ) )
		{
			if ( !dir.is_directory() )
				continue;

			std::string dirPath = dir.path().string();

			// Deviation from original Coral behaviour:
			// In x86, Program Files has an x86 next to it, which breaks
			// this algorithm and makes it use the first HostFxr it can find,
			// in my case from .NET Core 2.1. So only look for versions after dotnet/
			size_t dotnetOffset = dirPath.find( "dotnet" );

			if ( dirPath.find( '8', dotnetOffset ) == std::string::npos )
				continue;

			std::filesystem::path res = dir / std::filesystem::path( gHostFxrFilename );
			return res.string();
		}
	}

	return {};
}

bool Host::LoadHostFxr( std::string_view runtimeConfigPath )
{
	// Get the path to hostfxr
	std::optional<std::string> hostFxrPath = FindHostFxr();
	// We failed to get teh path
	if ( !hostFxrPath )
	{
		return false;
	}

	// Load the hostfxr library
	adm::Library lib = adm::Library( hostFxrPath.value().c_str() );
	if ( !lib )
	{
		return false;
	}

	// Get the function pointers
	fxrInit = lib.FindFunction<fxrInit_f>( "hostfxr_initialize_for_runtime_config" );
	fxrGetDelegate = lib.FindFunction<fxrGetDelegate_f>( "hostfxr_get_runtime_delegate" );
	fxrClose = lib.FindFunction<fxrClose_f>( "hostfxr_close" );
	fxrSetErrorWriter = lib.FindFunction<fxrSetErrorWriter_f*>( "hostfxr_set_error_writer" );

	std::string runtimeConfigPathStr = std::string( runtimeConfigPath );
	std::wstring runtimeConfigPathWStr;
	StringToWString( runtimeConfigPathStr, runtimeConfigPathWStr );

	if ( fxrInit == nullptr || fxrGetDelegate == nullptr || fxrClose == nullptr )
	{
		return nullptr;
	}

	// Load .NET Core, set up the context
	int resultCode = fxrInit( runtimeConfigPathWStr.c_str(), nullptr, &clrContext );
	if ( resultCode || nullptr == clrContext )
	{
		Shutdown();
		return false;
	}

	// Get the "load assembly" function pointer
	resultCode = fxrGetDelegate( clrContext,
	                             hdt_load_assembly,
	                             reinterpret_cast<void**>(&clrLoadAssembly) );
	if ( resultCode || nullptr == clrLoadAssembly )
	{
		Shutdown();
		return false;
	}

	// Try to get "get function pointer" too
	resultCode = fxrGetDelegate( clrContext,
	                             hdt_get_function_pointer,
	                             reinterpret_cast<void**>(&clrGetFunctionPointer) );
	if ( resultCode || nullptr == clrGetFunctionPointer )
	{
		Shutdown();
		return false;
	}

	return true;
}
