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
//#include <SDKDDKVer.h>
//https://curl.se/windows/
#include "Build/WarningsDisable.h"
#pragma warning (push)
#pragma warning (disable: 6319)
#pragma warning (disable: 26812)
#pragma warning (disable: 6255)
#pragma warning (disable: 6258)
#pragma warning (disable: 6001)
#pragma warning (disable: 26495)
#pragma warning (disable: 6386)
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
	#include <cstdint>
//External header files
#ifdef TRICKSTER_WINDOWS
	#if defined(TRICKSTER_OPENGL)
		#if defined(TRICKSTER_DEBUG)
		//DEBUG
		#include <GL/glew.h>
		#include <GLFW/glfw3.h>
		#include <glm/vec3.hpp>
		#include <glm/mat4x4.hpp>
		#include <glm/gtc/matrix_transform.hpp>
		#include <glm/matrix.hpp>
		#include <glm/vec4.hpp>
		#include <glm/vec2.hpp>
		#include <glm/trigonometric.hpp>
		#else
		//RELEASE
		#include <GL/glew.h>
		#include <GLFW/glfw3.h>
		#include <glm/vec3.hpp>
		#include <glm/mat4x4.hpp>
		#include <glm/gtc/matrix_transform.hpp>
		#include <glm/matrix.hpp>
		#include <glm/vec4.hpp>
		#include <glm/vec2.hpp>
		#include <glm/trigonometric.hpp>
		#endif
	#elif defined(TRICKSTER_VULKAN)
		#if defined(TRICKSTER_DEBUG)
		//DEBUG
		#else
		//RELEASE
		#endif
	#endif
#endif


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

//Freetype includes
	#include <ft2build.h>
	#include FT_FREETYPE_H
//Asio include
	#include <asio.hpp>

#pragma warning(pop)
//My header libraries
	#include <Core/Logger.h>
	#include <Events/Event.h>
	#include <Core/Macros.h>