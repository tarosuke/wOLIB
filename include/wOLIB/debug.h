#pragma once


#ifndef NDEBUG

namespace wO{

	class Debug{
		Debug();
		Debug(const Debug&);
	public:
		static void Printf(const char*, ...);
		static void Puts(const char*);
	};

}

#define DPRINTF(f, ...) wO::Debug::Printf((f), __VA_ARGS__)
#define DPUTS(s) wO::Debug::Puts(s)

#else

#define ABOUT(s)
#define DPRINTF(...)
#define DPUTS(s)

#endif
