////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_CPUPROFILER_H__
#define __CORE_CPUPROFILER_H__


#include <NsCore/Noesis.h>


// Critical paths in Noesis code contain markers for code instrumentation
// These markers can be redirected to any profiler tool by setting NS_PROFILER_ENABLED to 1
// The following macros must be defined when profiler is enabled:
//  - NS_PROFILE_CPU_INIT (used at init time)
//  - NS_PROFILER_CPU_FRAME (used at the beginning of each frame)
//  - NS_PROFILE_CPU (used for each marker)
//  - NS_PROFILE_CPU_SHUTDOWN (used at shutdown time)
// To minimize overhead, CPU Profiler is disabled by default

#ifndef NS_PROFILER_ENABLED
    #define NS_PROFILER_ENABLED 0
#endif

#if NS_PROFILER_ENABLED

    // https://github.com/bombomby/brofiler
    #ifdef NS_BROFILER
        #include "Brofiler.h"
        #pragma comment(lib, "ProfilerCore64.lib")
        #ifndef NS_PROFILER_CPU_FRAME
            #define NS_PROFILER_CPU_FRAME BROFILER_FRAME("Frame")
        #endif
        #ifndef NS_PROFILE_CPU
            #define NS_PROFILE_CPU(name) BROFILER_CATEGORY(name, Profiler::Color::Blue)
        #endif
    #endif

    #ifdef NS_PLATFORM_ANDROID
        #include <NsCore/KernelApi.h>
        #include <dlfcn.h> 

        // https://developer.android.com/studio/profile/systrace
        // Remember to enable debuggable in the manifest and custom events in systrace (-a).
        //  systrace.py -a com.noesis.Samples.HelloWorld perf sched gfx view

        extern NS_CORE_KERNEL_API void (*ATrace_beginSection_)(const char*);
        extern NS_CORE_KERNEL_API void (*ATrace_endSection_)();

        #ifndef NS_PROFILE_CPU_INIT
            #define NS_PROFILE_CPU_INIT_DECLARE \
                void (*ATrace_beginSection_)(const char*); \
                void (*ATrace_endSection_)(); \
                static void ATraceInit() \
                { \
                    void* android = dlopen("libandroid.so", RTLD_NOW | RTLD_LOCAL); \
                    if (android != nullptr) \
                    { \
                        typedef void (*ATrace_beginSectionT)(const char*); \
                        ATrace_beginSection_ = (ATrace_beginSectionT)dlsym(android, "ATrace_beginSection"); \
                        typedef void (*ATrace_endSectionT)(void); \
                        ATrace_endSection_ = (ATrace_endSectionT)dlsym(android, "ATrace_endSection"); \
                    } \
                }
            #define NS_PROFILE_CPU_INIT ATraceInit()
        #endif

        struct AScopedTrace
        {
            inline AScopedTrace(const char *name) { ATrace_beginSection_(name); }
            inline ~AScopedTrace() { ATrace_endSection_(); }
        };

        #ifndef NS_PROFILER_CPU_FRAME
            #define NS_PROFILER_CPU_FRAME NS_PROFILE_CPU("Frame")
        #endif
        #ifndef NS_PROFILE_CPU
            #define NS_PROFILE_CPU(name) AScopedTrace ___tracer(name)
        #endif
    #endif

    #ifdef NS_PLATFORM_XBOX_ONE
        #define USE_PIX
        #include <NsCore/StringUtils.h>
        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN 1
        #endif
        #include <windows.h>
        #include <pix.h>
        #pragma comment(lib, "pixEvt")
        #ifndef NS_PROFILER_CPU_FRAME
            #define NS_PROFILER_CPU_FRAME NS_PROFILE_CPU("Frame")
        #endif
        #ifndef NS_PROFILE_CPU
            #define NS_PROFILE_CPU(name) PIXScopedEvent(0xff000000 | StrHash(name), name)
        #endif
    #endif

    #ifdef NS_PLATFORM_SCE
        #include <razorcpu.h>
        #pragma comment(lib, "SceRazorCPU_stub_weak")
        #ifndef NS_PROFILER_CPU_FRAME
            #define NS_PROFILER_CPU_FRAME NS_PROFILE_CPU("Frame")
        #endif
        #ifndef NS_PROFILE_CPU
            #define NS_PROFILE_CPU(name) SCE_RAZOR_CPU_SCOPED_MARKER(name)
        #endif
    #endif

    #ifdef NS_PLATFORM_NX
        // Remember to add 'libnn_profiler.a' to 'Additional Dependencies'
        #include <nn/profiler.h>
        #ifndef NS_PROFILER_CPU_FRAME
            #define NS_PROFILER_CPU_FRAME nn::profiler::RecordHeartbeat(nn::profiler::Heartbeats_Main)
        #endif
        #ifndef NS_PROFILE_CPU
            #define NS_PROFILE_CPU(name) nn::profiler::ScopedCodeBlock(name)
        #endif
    #endif

#endif

#ifndef NS_PROFILE_CPU_INIT_DECLARE
    #define NS_PROFILE_CPU_INIT_DECLARE
#endif

#ifndef NS_PROFILE_CPU_INIT
    #define NS_PROFILE_CPU_INIT NS_NOOP
#endif

#ifndef NS_PROFILER_CPU_FRAME
    #define NS_PROFILER_CPU_FRAME NS_NOOP
#endif

#ifndef NS_PROFILE_CPU
    #define NS_PROFILE_CPU(...) NS_NOOP
#endif

#ifndef NS_PROFILE_CPU_SHUTDOWN
    #define NS_PROFILE_CPU_SHUTDOWN NS_NOOP
#endif

#endif
