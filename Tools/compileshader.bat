@echo off
echo Welcome to the shader compile batch file
echo This will compile the raw GLSL shaders into compiled shaders
echo:
set FetchDir=../TricksterEngine/NotCompiledShaders/
set OutputDir=../Resources/Shaders/
set /A "count=0"
echo The input directory is %FetchDir%
echo The output directory is %OutputDir%
echo The Vulkan SDK directory is %VULKAN_SDK%
echo:
echo:
for /r %FetchDir% %%i in (*.frag) do (%VULKAN_SDK%\Bin\glslangValidator.exe %%i -o %OutputDir%%%~nifrag.spv
echo Compiled %%~ni.frag to %%~nifrag.spv
set /A "count=count+1") 
for /r %FetchDir% %%i in (*.vert) do (%VULKAN_SDK%\Bin\glslangValidator.exe %%i -o %OutputDir%%%~nivert.spv
echo Compiled %%~ni.vert to %%~nivert.spv
set /A "count=count+1")
echo:
echo Compiled %count% shaders!