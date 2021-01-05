#pragma once
//#include <SDKDDKVer.h>
//https://curl.se/windows/
//STL
	#include <iostream>
	#include <memory>
	#include <functional>
	#include <algorithm>
	#include <utility>
	#include <string>
	#include <vector>
	#include <unordered_map>
	#include <type_traits>
	#include <sstream>
	#include <cstdio>
	#include <fstream>
	#include <mutex>
	#include <shared_mutex>
	#include <list>
	#include <thread>
	#include <queue>
	#include <ctime>
	#include <future>
	#include <condition_variable>
	#include <atomic>
	#include <map>
//External header files
//GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
	#include <GL/glew.h>
//GLFW
	#include <GLFW/glfw3.h>

//GLM
	#include <glm/vec3.hpp>
	#include <glm/mat4x4.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/matrix.hpp>
	#include <glm/vec4.hpp>
	#include <glm/vec2.hpp>
	#include <glm/trigonometric.hpp>

//Single-header libraries

//FMOD includes
	#include <fmod/fmod.h>
	#include <fmod/fmod.hpp>
	#include <fmod/fmod_errors.h>
	#include <fmod/fmod_codec.h>
	#include <fmod/fmod_common.h>
	#include <fmod/fmod_dsp.h>
	#include <fmod/fmod_dsp_effects.h>
	#include <fmod/fmod_output.h>

//My header libraries
	#include <Core/Logger.h>
	#include <Events/Event.h>
#ifdef TRICKSTER_EXPORT
#define TRICKSTER_API __declspec(dllexport)
#else
#define TRICKSTER_API  __declspec(dllimport)
#endif