#pragma once
#define TRICKSTER_VERSION_MAJOR 0
#define TRICKSTER_VERSION_MINOR 1
#define TRICKSTER_VERSION_PATCH 0
#define TRICKSTER_VERSION_BUILD 17

#define stringify(a) stringify_(a)
#define stringify_(a) #a
#define TRICKSTER_VERSION_STRING std::string(std::to_string(TRICKSTER_VERSION_MAJOR) + "." + std::to_string(TRICKSTER_VERSION_MINOR) + "." + std::to_string(TRICKSTER_VERSION_PATCH) + "." + std::to_string(TRICKSTER_VERSION_BUILD))
