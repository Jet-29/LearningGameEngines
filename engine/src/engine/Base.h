#pragma once

#include "EnginePch.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#define ENGINE_API
#endif

#ifdef ENGINE_DEBUG
#if defined(ENGINE_PLATFORM_WINDOWS)
#define ENGINE_DEBUG_BREAK() __debugbreak()
#elif defined(ENGINE_PLATFORM_LINUX)
#include <signal.h>
#define ENGINE_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define ENGINE_ENABLE_ASSERTS
#else
#define ENGINE_DEBUGBREAK()
#endif

#define BIT(x) (1 << x)

#define ENGINE_EXPAND_MACRO(x) x
#define ENGINE_STRINGIFY_MACRO(x) #x

#define ENGINE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include "Assert.h"
