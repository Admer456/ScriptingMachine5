// SPDX-FileCopyrightText: 2022-2025 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

#include <string>

namespace smv
{
	class Assembly
	{
	public:
		Assembly( std::string_view path );

		const char* GetPath() const { return fullPath.c_str(); }

		void* GetMethod( std::string_view namespaceName, std::string_view typeName, std::string_view methodName );

		template <typename TFunc>
		constexpr TFunc* GetMethod( std::string_view namespaceName, std::string_view typeName, std::string_view methodName )
		{
			return static_cast<TFunc*>(GetMethod( namespaceName, typeName, methodName ));
		}

	private:
		std::string fullPath; // path/to/library
	};
}
