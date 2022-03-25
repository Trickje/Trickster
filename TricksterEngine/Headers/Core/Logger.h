#pragma once
/*
================================================================================
		Copyright 2020 Rick Pijpers

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
#include <Windows.h>
#include <iostream>
#include <string>
#if defined(_DEBUG) && defined(TRICKSTER_OPENGL)
#include <GL/glew.h>
#endif

#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef _DEBUG
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#define LOG(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);\
			std::cout << x << std::endl;\
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); 
#define LOG_WARNING(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);\
			std::cout << x << std::endl;\
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#define LOG_ERROR(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);\
			std::cout << x << std::endl;\
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#define LOG_USELESS(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);\
			std::cout << x << std::endl;\
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#endif
#ifndef _DEBUG
#define GLCall(x) x;
#define LOG(x) 
#define LOG_WARNING(x) 
#define LOG_ERROR(x)
#define LOG_USELESS(x)
#endif
namespace TE {
	/*
	static void LOG(std::string a_Str)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 2);
		std::cout << a_Str.c_str() << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
	};

	static void LOG(int a_Int)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 2);
		std::cout << std::to_string(a_Int).c_str() << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
	};

	static void LOG(char* a_CStr)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 2);
		std::cout << a_CStr << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
	};
	
	static void LOG_ERROR(std::string a_Str)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << a_Str.c_str() << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
	};

	static void LOG_ERROR(int a_Int)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << std::to_string(a_Int).c_str() << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
	};

	static void LOG_ERROR(char* a_CStr)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << a_CStr << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
	};
	*/
	static void LOG_CLEAR()
	{
#ifdef _DEBUG
		system("cls");
#endif
	}

	
	static void GLClearError()
	{
#ifdef _DEBUG
		while (glGetError() != GL_NO_ERROR);
#endif
	}
	static bool GLLogCall(const char* function, const char* file, int line)
	{
#ifdef _DEBUG
		while (GLenum error = glGetError())
		{
			LOG_ERROR("[OpenGL Error] (" + std::to_string(error) + ")" + ": " + function + " " + file + ": " + std::to_string(line));
			return false;
		}
#endif

		return true;
	}

#if defined(_DEBUG) && defined(TRICKSTER_OPENGL)
	static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		std::string typeString;
		std::string severityString;

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: typeString = "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeString = "DEPRICATED BEHAVIOUR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeString = "UNDEFINED BEHAVIOUR";
		case GL_DEBUG_TYPE_PERFORMANCE: typeString = "PERFORMANCE";
		case GL_DEBUG_TYPE_PORTABILITY: typeString = "PORTABILITY";
		default: typeString = "OTHER";
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH: severityString = "HIGH";
		case GL_DEBUG_SEVERITY_MEDIUM: severityString = "MEDIUM";
		case GL_DEBUG_SEVERITY_LOW: severityString = "LOW";
		case GL_DEBUG_SEVERITY_NOTIFICATION: severityString = "NOTIFICATION";
		default: severityString = "OTHER";
		}

		fprintf(stderr, "OGL DEBUG MESSAGE:\n type: %s\n severity: %s\n message: %s\n\n", typeString.c_str(), severityString.c_str(), message);

	}
#endif
	}