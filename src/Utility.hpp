// SPDX-FileCopyrightText: 2025 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <string_view>
#pragma warning( disable : 4244 )

namespace sm5
{
	inline void WStringToString( const std::wstring& in, std::string& out )
	{
		out = "";
		out.reserve( in.length() );

		for ( auto& character : in )
		{
			out += character;
		}
	}

	inline void StringToWString( std::string_view in, std::wstring& out )
	{
		out = L"";
		out.reserve( in.length() );

		for ( auto& character : in )
		{
			out += character;
		}
	}

	inline void StringToWString( const std::string& in, std::wstring& out )
	{
		StringToWString( std::string_view( in ), out );
	}
}
