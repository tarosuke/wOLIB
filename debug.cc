#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>

#include <wOLIB/debug.h>

#ifndef NDEBUG

namespace wO{

	void Debug::Printf(const char* format, ...){
		if(!format){ return; };
		va_list ap;
		va_start(ap, format);

		vsyslog(LOG_ERR, format, ap);
		va_end(ap);
	}

	void Debug::Puts(const char* str){
		if(!str){ return; }

		syslog(LOG_ERR, "%s", str);
	}

}

#endif
