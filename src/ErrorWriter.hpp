// SPDX-FileCopyrightText: 2025 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

namespace smv
{
	class ErrorWriter final
	{
	public:
		static void Register( fxrSetErrorWriter_f* setErrorWriter );
		static void HOSTFXR_CALLTYPE ErrorWriteFunction( const char_t* message );
	};
}
