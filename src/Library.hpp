// SPDX-FileCopyrightText: 2021-2025 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>

// Platform identification code copied from adm-utils
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1

#ifdef WIN32

#define ADM_PLATFORM PLATFORM_WINDOWS

#elif __linux__

#define ADM_PLATFORM PLATFORM_LINUX

#else

static_assert( false, "Unsupported platform" );

#endif

namespace adm
{
	// ============================
	// Library
	//
	// A utility for loading and using dynamic modules
	// Usage:
	// 
	// Library myLib( "bin/mylib" );
	// if ( myLib )
	//     myLib.FindFunction<
	// ============================
	class Library final
	{
	public:
		Library( std::string_view libraryPath );
		Library( const char* libraryPath );
		Library( void* handle );
		Library( Library&& library ) noexcept;
		Library( const Library& library ) = delete;
		~Library();

		// Easy way to determine a return type
		// https://stackoverflow.com/questions/53673442/simplest-way-to-determine-return-type-of-function
		template<typename functionType>
		using FunctionReturnType = typename decltype(std::function{ std::declval<functionType>() })::result_type;

		// Parameterless version
		template<typename functionType,
			typename returnType = FunctionReturnType<functionType>>
		std::optional<returnType> TryExecuteFunction( std::string_view functionName )
		{
			auto func = FindFunction<functionType>( functionName );
			if ( nullptr == func )
			{
				return {};
			}

			return (*func)();
		}

		template<typename functionType, 
			typename structArgumentType, 
			typename returnType = FunctionReturnType<functionType>>
		std::optional<returnType> TryExecuteFunction( std::string_view functionName, const structArgumentType& argument )
		{
			auto func = FindFunction<functionType>( functionName );
			if ( nullptr == func )
			{
				return {};
			}

			return (*func)( argument );
		}

		template<typename functionType>
		functionType FindFunction( std::string_view functionName ) const
		{
			return static_cast<functionType>(GetFunctionInternal( functionName ));
		}

		void Dispose();

		operator bool() const;

	private:
		void* GetFunctionInternal( std::string_view functionName ) const;

		void* libraryHandle{ nullptr };
	};
}
