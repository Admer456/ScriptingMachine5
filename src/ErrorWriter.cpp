// SPDX-FileCopyrightText: 2025 Admer Šuko
// SPDX-License-Identifier: MIT

#include "Host.hpp"
#include "ErrorWriter.hpp"
#include <iostream>

using namespace smv;

void ErrorWriter::Register( fxrSetErrorWriter_f* setErrorWriter )
{
	return;
	
	if ( reinterpret_cast<int>( setErrorWriter ) > 0x10000 )
	{
		try
		{
			(*setErrorWriter)( &ErrorWriter::ErrorWriteFunction );
			std::cout << "ErrorWriter::Register: success" << std::endl;
		}
		catch ( std::exception ex )
		{
			std::cout << "ErrorWriter::Register: setErrorWriter triggers DEP" << std::endl;
		}
	}
	else
	{
		std::cout << "ErrorWriter::Register: setErrorWriter is nullptr" << std::endl;
	}
}

void HOSTFXR_CALLTYPE ErrorWriter::ErrorWriteFunction( const char_t* message )
{
	std::wcout << L"hostfxr: < " << message << L" >" << std::endl;
}
