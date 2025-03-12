// SPDX-FileCopyrightText: 2025 Admer Šuko
// SPDX-License-Identifier: MIT

#include "Host.hpp"
#include "Assembly.hpp"
#include "Utility.hpp"

using namespace smv;

Assembly::Assembly( std::string_view path )
	: fullPath( path )
{
	
}

void* Assembly::GetMethod( std::string_view namespaceName, std::string_view typeName, std::string_view methodName )
{
	return Host::GetMethod( namespaceName, typeName, methodName );
}
