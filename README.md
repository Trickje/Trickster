# Trickster Engine
## Made by Trickje

### About this engine
#### Target audience
This engine is not public yet, and the source files will never be public.
But I will publish the header files and binaries until I want it only for myself.
Which I can decide at any point.


## Current Version
#### Version (0.0.0)
1. Entry point handled by engine
1. Window
1. 2D textures
1. Delta Time
1. Logging system
1. OpenGL wrapper, sort of
1. Event system
1. Model loading
1. 3D models
1. Input
1. Camera
1. Multithreading
1. Pause option

## Future versions:
### Single-Header Files
1. Seeded random number generator that takes position into account, so the same seed will load the same world everytime
1. Noise generator (for heightmaps)
1. Bounding volume hierarchy (including node traversal)
1. File traverser (header file who finds all the files of x extension, in folder x)
1. Image maker (used for raytracing or procedural textures)
1. Timer, like SFML
1. File logger (to debug code with)
1. Improve profiler to profile function calls and data usage, and seperate it in a single-header
1. Random name generator
1. X solver (don't know if it's possible but why not)



### Networking Update               (Version 0.1.0)
1. ~~Sound~~
1. Packet listening / sending
1. Database requesting and handling
1. ~~Versioning~~
1. Automatic updating (launcher?)
1. Create a server project
1. Logging in / account system per game. But don't make it mandatory, use a way of choosing to enable it

### UI Update                       (Version 0.1.1)
1. Text rendering (OpenGL)
1. ImGUI OR my own GUI classes :))
1. GUI docking
1. Chat

### Dynamic linking Update          (Version 0.1.2)
1. Modding support
1. Scripting support
1. File saving
1. File loading
1. Encrypting those files
1. Configuration of settings
1. Having a header file per category that you can include, like glm does
1. ~~Have easy switching between building the dll and a static library~~

### Graphical front-end Update      (Version 0.1.3)
1. Ray maths
1. Lights
1. Shaders and having a system to choose the shader based on variables, no burden with the game programmer
1. PBR Set up
1. Batch rendering based on materials
1. Render queue
1. Different camera's active at the same time (minimap)

### Graphical back-end Update       (Version 0.2.0)
1. Abstracting the graphics away from platform specific, this will change the layout of  the project quite a bit
1. Having the update functionality run asynchronous from the graphic funcitonality, (graphic would be the main thread)
1. Vulkan (maybe)
1. Debug log in-game (like in CounterStrike)
1. Improve the Model system (And actually give the ModelManager a use)
1. Model animations
1. Sprite animations

### Editor Update                   (Version 0.2.1)
1. Scene viewer
1. Mouse picking
1. Entity hierarchy (without ECS)
1. Saving and loading levels

### System back-end Update          (Version 0.2.2)
1. Seperating the worker pool so that some tasks are only acessible to certain workers but those workers can also access the main pool
1. Loading screen
1. Support backwards compatibility, people can join servers if they have a version of the game that is within the same subversion, but not a graphical update


