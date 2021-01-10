#pragma once
/*
================================================================================
		Copyright 2021 Rick Pijpers

		Licensed under the Apache License, Version 2.0 (the "License");
		you may not use this file except in compliance with the License.
		You may obtain a copy of the License at

			http://www.apache.org/licenses/LICENSE-2.0

		Unless required by applicable law or agreed to in writing, software
		distributed under the License is distributed on an "AS IS" BASIS,
		WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		See the License for the specific language governing permissions and
		limitations under the License.
=================================================================================
 */
#ifndef TRICKSTER_STATIC
//DLL
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
#else
//Static library
#define TRICKSTER_API
#define TRICKSTER_FUNCTION(returnType) returnType __cdecl
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