/*
 * Copyright (c) 2012-2023 Daniele Bartolini et al.
 * SPDX-License-Identifier: MIT
 */

#include "core/error/callstack.h"
#include "core/error/error.h"
#include "core/memory/temp_allocator.inl"
#include "core/strings/string_stream.inl"
#include "device/log.h"
#include <stdarg.h>
#include <stdlib.h> // exit
#include <stb_sprintf.h>

LOG_SYSTEM(ERROR, "error")

namespace crown
{
namespace error
{
	static void abort(const char *format, va_list args)
	{
		char buf[1024];
		stbsp_vsnprintf(buf, sizeof(buf), format, args);

		TempAllocator4096 ta;
		StringStream ss(ta);
		ss << buf;
		ss << "Stacktrace:\n";
		callstack(ss);

		loge(ERROR, string_stream::c_str(ss));
		exit(EXIT_FAILURE);
	}

	void abort(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		abort(format, args);
		va_end(args);
	}

} // namespace error

} // namespace crown
