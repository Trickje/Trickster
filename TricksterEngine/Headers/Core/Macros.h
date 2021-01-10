#pragma once
//EXPORTING
#ifdef TRICKSTER_EXPORT
	#if defined(TRICKSTER_C)
		#define TRICKSTER_API extern "C" __declspec(dllexport)
		#define TRICKSTER_FUNCTION(returnType) extern "C" __declspec(dllexport) returnType __cdecl
	#else
		#define TRICKSTER_API __declspec(dllexport)
		#define TRICKSTER_FUNCTION(returnType) __declspec(dllexport) returnType __cdecl
	#endif
#else
	#if defined(TRICKSTER_C)
		//IMPORTING
		#define TRICKSTER_API extern "C" __declspec(dllimport)
		#define TRICKSTER_FUNCTION(returnType) extern "C" __declspec(dllimport) returnType __cdecl
	#else
		#define TRICKSTER_API __declspec(dllimport)
		#define TRICKSTER_FUNCTION(returnType) __declspec(dllimport) returnType __cdecl
	#endif
#endif

#ifdef TRICKSTER_WINDOWS
	#if defined(TRICKSTER_OPENGL)
		#if defined(TRICKSTER_DEBUG)
		//DEBUG
		#else
		//RELEASE
		#endif
	#elif defined(TRICKSTER_VULKAN)
		#if defined(TRICKSTER_DEBUG)
		//DEBUG
		#else
		//RELEASE
		#endif
	#endif
#endif