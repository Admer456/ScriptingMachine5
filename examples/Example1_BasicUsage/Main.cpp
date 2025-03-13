// SPDX-FileCopyrightText: 2025 Admer Šuko
// SPDX-License-Identifier: MIT

#include "Host.hpp"
#include "Assembly.hpp"

#include <iostream>

using namespace smv;

int main()
{
	if ( !Host::Init( "example.runtimeconfig.json" ) )
	{
		std::cout << "Failed to initialize host. Make sure to copy:" << std::endl;
		std::cout << "* examples/Example1_BasicUsage/example.runtimeconfig.json" << std::endl;
		std::cout << "to:" << std::endl;
		std::cout << "* examples/Example1_BasicUsage/ManagedLibrary/bin/Debug/net8.0" << std::endl;
		std::cout << "or" << std::endl;
		std::cout << "* examples/Example1_BasicUsage/ManagedLibrary/bin/Release/net8.0" << std::endl;
		return 1;
	}

	Assembly* library = Host::LoadAssembly( "ManagedLibrary.dll" );
	if ( library == nullptr )
	{
		std::cout << "Failed to load ManagedLibrary.dll. Make sure to set the working directory to:" << std::endl;
		std::cout << "* examples/Example1_BasicUsage/ManagedLibrary/bin/Debug/net8.0" << std::endl;
		std::cout << "or" << std::endl;
		std::cout << "* examples/Example1_BasicUsage/ManagedLibrary/bin/Release/net8.0" << std::endl;
		return 2;
	}

	using methodType = void CORECLR_DELEGATE_CALLTYPE ();
	methodType* method = library->GetMethod<methodType>( "ManagedLibrary", "EntryPoint", "ExampleMethod" );
	if ( method == nullptr )
	{
		std::cout << "Failed to obtain ExampleMethod method" << std::endl;
		return 3;
	}

	method();
	
	Host::Shutdown();
	return 0;
}
