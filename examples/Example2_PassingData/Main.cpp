// SPDX-FileCopyrightText: 2025 Admer Šuko
// SPDX-License-Identifier: MIT

#include "Host.hpp"
#include "Assembly.hpp"

#include <iostream>

using namespace sm5;

struct Vector3
{
	float x, y, z;
};

void Example( Assembly* assembly )
{
	using intMethod_f = void SM5_CALL( int );
	using floatMethod_f = void SM5_CALL( float );
	using vector3Method_f = void SM5_CALL( Vector3* );
	using complexMethod_f = int SM5_CALL( int* );

	auto intMethod = assembly->GetMethod<intMethod_f>( "ManagedLibrary", "EntryPoint", "IntMethod" );
	auto floatMethod = assembly->GetMethod<floatMethod_f>( "ManagedLibrary", "EntryPoint", "FloatMethod" );
	auto vector3Method = assembly->GetMethod<vector3Method_f>( "ManagedLibrary", "EntryPoint", "Vector3Method" );
	auto complexMethod = assembly->GetMethod<complexMethod_f>( "ManagedLibrary", "EntryPoint", "ComplexMethod" );

	if ( intMethod != nullptr )
	{
		std::cout << "[C++] Sending 25..." << std::endl;
		intMethod( 25 );
	}

	if ( floatMethod != nullptr )
	{
		std::cout << "[C++] Sending 1.125..." << std::endl;
		floatMethod( 1.125 );
	}

	if ( vector3Method != nullptr )
	{
		std::cout << "[C++] Sending (1, 2, 3)..." << std::endl;
		Vector3 vec = Vector3{ 1.0f, 2.0f, 3.0f };
		vector3Method( &vec );
	}

	if ( complexMethod != nullptr )
	{
		int variable = 10;
		std::cout << "[C++] Sending 10..." << std::endl;
		int result = complexMethod( &variable );
		std::cout << "[C++] Return value is " << result << "!" << std::endl;
		std::cout << "[C++] Original variable became " << variable << "!" << std::endl;
	}
}

int main()
{
	if ( !Host::Init( "example.runtimeconfig.json" ) )
	{
		std::cout << "Failed to initialize host." << std::endl;
		return 1;
	}

	Assembly* library = Host::LoadAssembly( "ManagedLibrary.dll" );
	if ( library == nullptr )
	{
		std::cout << "Failed to load ManagedLibrary.dll" << std::endl;
		return 2;
	}

	Example( library );

	Host::Shutdown();
	return 0;
}
