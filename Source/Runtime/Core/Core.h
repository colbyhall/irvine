// Copyright Colby Hall. All Rights Reserved.

#pragma once

// Determine platform
#if defined(PLATFORM_BLUE)
	// Correct define already defined, this overrides everything else
#elif defined(_WIN32) || defined(_WIN64)
	#include <winapifamily.h>
	#if WINAPI_FAMILY == WINAPI_FAMILY_APP
		#define PLATFORM_WINDOWS_UWP
	#endif
	#define PLATFORM_WINDOWS
#elif defined(__ANDROID__) // Android is linux too. That's why we check it first
	#define PLATFORM_ANDROID
#elif defined(__linux__)
	#define PLATFORM_LINUX
#elif defined(__APPLE__)
	#include <TargetConditionals.h>
	#if defined(TARGET_OS_IPHONE) && !TARGET_OS_IPHONE
		#define PLATFORM_MACOS
	#else
		#define PLATFORM_IOS
	#endif
#elif defined(__EMSCRIPTEN__)
	#define PLATFORM_WASM
#endif

// Determine compiler
#if defined(__clang__)
	#define COMPILER_CLANG
#elif defined(__GNUC__)
	#define COMPILER_GCC
#elif defined(_MSC_VER)
	#define COMPILER_MSVC
#endif

#if defined(__MINGW64__) || defined (__MINGW32__)
	#define COMPILER_MINGW
#endif

// Detect CPU architecture
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	// X86 CPU architecture
	#define CPU_X86
	#if defined(__x86_64__) || defined(_M_X64)
		#define CPU_ADDRESS_BITS 64
	#else
		#define CPU_ADDRESS_BITS 32
	#endif
#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm__) || defined(_M_ARM)
	// ARM CPU architecture
	#define CPU_ARM
	#if defined(__aarch64__) || defined(_M_ARM64)
		#define CPU_ADDRESS_BITS 64
	#else
		#define CPU_ADDRESS_BITS 32
	#endif
#elif defined(PLATFORM_WASM)
	// WebAssembly CPU architecture
	#define CPU_WASM
	#define CPU_ADDRESS_BITS 32
#else
	#error Unsupported CPU architecture
#endif

// Pragmas to store / restore the warning state and to disable individual warnings
#ifdef COMPILER_CLANG
	#define PRAGMA(x)					_Pragma(#x)
	#define SUPPRESS_WARNING_PUSH		PRAGMA(clang diagnostic push)
	#define SUPPRESS_WARNING_POP		PRAGMA(clang diagnostic pop)
	#define CLANG_SUPPRESS_WARNING(w)	PRAGMA(clang diagnostic ignored w)
#else
	#define CLANG_SUPPRESS_WARNING(w)
#endif

#ifdef COMPILER_GCC
	#define PRAGMA(x)					_Pragma(#x)
	#define SUPPRESS_WARNING_PUSH		PRAGMA(GCC diagnostic push)
	#define SUPPRESS_WARNING_POP		PRAGMA(GCC diagnostic pop)
	#define GCC_SUPPRESS_WARNING(w)		PRAGMA(GCC diagnostic ignored w)
#else
	#define GCC_SUPPRESS_WARNING(w)
#endif

#ifdef COMPILER_MSVC
	#define PRAGMA(x)					__pragma(x)
	#define SUPPRESS_WARNING_PUSH		PRAGMA(warning (push))
	#define SUPPRESS_WARNING_POP		PRAGMA(warning (pop))
	#define MSVC_SUPPRESS_WARNING(w)	PRAGMA(warning (disable : w))
#else
	#define MSVC_SUPPRESS_WARNING(w)
#endif

// Define inline macro
#if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
	#define INLINE __inline__ __attribute__((always_inline))
#elif defined(COMPILER_MSVC)
	#define INLINE __forceinline
#else
	#error Undefined
#endif

// Cache line size (used for aligning to cache line)
#ifndef CACHE_LINE_SIZE
	#define CACHE_LINE_SIZE 64
#endif

// Define macro to get current function name
#if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
	#define FUNCTION_NAME	__PRETTY_FUNCTION__
#elif defined(COMPILER_MSVC)
	#define FUNCTION_NAME	__FUNCTION__
#else
	#error Undefined
#endif

#if defined(COMPILER_MSVC)
	#if _MSC_VER < 1300
		#define DEBUG_TRAP __asm int 3
	#else
		#define DEBUG_TRAP __debugbreak()
	#endif
#else
	#define DEBUG_TRAP __builtin_trap()
#endif

// Suppress warnings generated by the standard template library
#define SUPPRESS_WARNINGS_STD_BEGIN															\
	SUPPRESS_WARNING_PUSH																	\
	MSVC_SUPPRESS_WARNING(4619)																\
	MSVC_SUPPRESS_WARNING(4710)																\
	MSVC_SUPPRESS_WARNING(4711)																\
	MSVC_SUPPRESS_WARNING(4820)																\
	MSVC_SUPPRESS_WARNING(4514)																\
	MSVC_SUPPRESS_WARNING(5262)																\
	MSVC_SUPPRESS_WARNING(5264)																\
																							\
	GCC_SUPPRESS_WARNING("-Wstringop-overflow=")

#define SUPPRESS_WARNINGS_STD_END															\
	SUPPRESS_WARNING_POP

SUPPRESS_WARNING_PUSH
MSVC_SUPPRESS_WARNING(5264)

using u8 = unsigned char;
constexpr u8 U8_MIN = 0;
constexpr u8 U8_MAX = 0xff;

using u16 = unsigned short;
constexpr u16 U16_MIN = 0;
constexpr u16 U16_MAX = 0xffff;

using u32 = unsigned int;
constexpr u32 U32_MIN = 0;
constexpr u32 U32_MAX = 0xffffffff;

using u64 = unsigned long long;
constexpr u64 U64_MIN = 0;
constexpr u64 U64_MAX = 0xffffffffffffffff;

using usize = u64;
constexpr usize USIZE_MIN = U64_MIN;
constexpr usize USIZE_MAX = U64_MAX;

static_assert(sizeof(u8) == 1, "u8 should only be 1 byte");
static_assert(sizeof(u16) == 2, "u16 should only be 2 bytes");
static_assert(sizeof(u32) == 4, "u32 should only be 4 bytes");
static_assert(sizeof(u64) == 8, "u64 should only be 8 bytes");
static_assert(sizeof(usize) == 8, "usize should only be 8 bytes");

using i8 = signed char;
constexpr i8 I8_MIN = (-127 - 1);
constexpr i8 I8_MAX = 127;

using i16 = short;
constexpr i16 I16_MIN = (-32767 - 1);
constexpr i16 I16_MAX = 32767;

using i32 = int;
constexpr i32 I32_MIN = (-2147483647 - 1);
constexpr i32 I32_MAX = 2147483647;

using i64 = long long;
constexpr i64 I64_MIN = (-9223372036854775807 - 1);
constexpr i64 I64_MAX = 9223372036854775807;

using isize = i64;
constexpr isize ISIZE_MIN = I64_MIN;
constexpr isize ISIZE_MAX = I64_MAX;

static_assert(sizeof(i8) == 1, "i8 should only be 1 byte");
static_assert(sizeof(i16) == 2, "i16 should only be 2 bytes");
static_assert(sizeof(i32) == 4, "i32 should only be 4 bytes");
static_assert(sizeof(i64) == 8, "i64 should only be 8 bytes");
static_assert(sizeof(isize) == 8, "isize should only be 8 bytes");

using f32 = float;
constexpr f32 F32_MIN = 1.17549435e-38f;
constexpr f32 F32_MAX = 3.40282347e+38f;

using f64 = double;

static_assert(sizeof(f32) == 4, "f32 should only be 4 bytes");
static_assert(sizeof(f64) == 8, "f64 should only be 8 bytes");

using NullPtr = decltype(nullptr);

SUPPRESS_WARNING_POP
