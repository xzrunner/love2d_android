#ifndef LOVE_LOG_H
#define LOVE_LOG_H

#include "config.h"

#ifdef LOVE_WINDOWS
#	include <iostream>
#	include <stdarg.h>
#elif defined LOVE_ANDROID_JNI
#	include <android/log.h>
#endif

inline void log(const char* pMessage, ...)
{
#ifdef LOVE_WINDOWS
	va_list args;
	int size_buffer = 256, size_out;
	char* buffer;
	while (true)
	{
		buffer = new char[size_buffer];
		memset(buffer, 0, size_buffer);

		va_start(args, pMessage);
		size_out = vsnprintf(buffer, size_buffer, pMessage, args);
		va_end(args);

		if (size_out == size_buffer || size_out == -1 || size_out == size_buffer-1)
			size_buffer *= 2;
		else if (size_out > size_buffer)
			size_buffer = size_out + 2; // to avoid the ambiguous case
		else
			break;

		delete[] buffer;
	}
	std::cout << buffer << std::endl;
	delete[] buffer;
#else
	va_list lVarArgs;
	va_start(lVarArgs, pMessage);
	__android_log_vprint(ANDROID_LOG_ERROR, "love", pMessage, lVarArgs);
	__android_log_print(ANDROID_LOG_ERROR, "love", "\n");
	va_end(lVarArgs);
#endif
}

#endif // LOVE_LOG_H