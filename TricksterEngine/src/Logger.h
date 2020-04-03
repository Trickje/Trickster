#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
namespace Trickster {
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

	static void LOG_CLEAR()
	{
		system("cls");
	}


	static void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}
	static bool GLLogCall(const char* function, const char* file, int line)
	{
		while (GLenum error = glGetError())
		{
			LOG_ERROR("[OpenGL Error] (" + std::to_string(error) + ")" + ": " + function + " " + file + ": " + std::to_string(line));
			return false;
		}
		return true;
	}

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
}