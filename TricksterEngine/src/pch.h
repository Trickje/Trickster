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
#include <sstream>
#include <cstdio>
#include <fstream>
#include <mutex>
#include <shared_mutex>
#include <list>
#include <thread>
#include <ctime>
#include <future>

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


//My header files
#include <Timer.h>
#include <Logger.h>
#include <Events/EventManager.h>
#include <Events/Event.h>
#include <Engine.h>
#include <Renderer.h>
#include <Application.h>
