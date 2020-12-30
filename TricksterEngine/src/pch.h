#pragma once
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
//External header files
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <glm/trigonometric.hpp>

#include <stb_image.h>
#include <TinyObjLoader.h>
//FMOD includes
#include <fmod/fmod.h>
#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>
#include <fmod/fmod_codec.h>
#include <fmod/fmod_common.h>
#include <fmod/fmod_dsp.h>
#include <fmod/fmod_dsp_effects.h>
#include <fmod/fmod_output.h>


//My header files
#include <Core/Timer.h>
#include <Core/Logger.h>
#include <Core/Profiler.h>
#include <Events/EventManager.h>
#include <Events/Event.h>
#include <Core/Engine.h>
#include <Renderer.h>
#include <Core/AudioPlayer.h>
#include <Core/Application.h>
