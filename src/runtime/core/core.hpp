// Copyright Colby Hall. All Rights Reserved.

#pragma once

// Determine platform
#if defined(PLATFORM_BLUE)
	// Correct define already defined, this overrides everything else
	#error Not Supported
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
	#define FORCE_INLINE __inline__ __attribute__((always_inline))
#elif defined(COMPILER_MSVC)
	#define FORCE_INLINE __forceinline
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

// OS-specific includes
#if defined(PLATFORM_WINDOWS)
	#define BREAKPOINT		__debugbreak()
#elif defined(PLATFORM_BLUE)
	#error Not Supported
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_ANDROID) || defined(PLATFORM_MACOS) || defined(PLATFORM_IOS)
	#if defined(_CPU_X86)
		#define BREAKPOINT	__asm volatile ("int $0x3")
	#elif defined(_CPU_ARM)
		#define BREAKPOINT	__builtin_trap()
	#endif
#elif defined(PLATFORM_WASM)
	#define BREAKPOINT	do { } while (false) // Not supported
#else
	#error Unknown platform
#endif

#ifdef _DEBUG
	#define BUILD_DEBUG
#endif

// Crashes the application
#define CRASH																				\
	SUPPRESS_WARNING_PUSH																	\
	MSVC_SUPPRESS_WARNING(6011)																\
	do { int* _ptr = nullptr; *_ptr = 0; } while (false)									\
	SUPPRESS_WARNING_POP

// Suppress warnings generated by the standard template library
#define SUPPRESS_WARNINGS_STD_BEGIN															\
	SUPPRESS_WARNING_PUSH																	\
	MSVC_SUPPRESS_WARNING(4619)																\
	MSVC_SUPPRESS_WARNING(4710)																\
	MSVC_SUPPRESS_WARNING(4711)																\
	MSVC_SUPPRESS_WARNING(4820)																\
	MSVC_SUPPRESS_WARNING(4514)																\
	MSVC_SUPPRESS_WARNING(5262) /* implicit fall-through occurs here; are you missing a break statement? Use [[fallthrough]] when a break statement is intentionally omitted between cases */ \
	MSVC_SUPPRESS_WARNING(5264)																\
	MSVC_SUPPRESS_WARNING(4365)																\
	MSVC_SUPPRESS_WARNING(5219)																\
																							\
	GCC_SUPPRESS_WARNING("-Wstringop-overflow=")

#define SUPPRESS_WARNINGS_STD_END															\
	SUPPRESS_WARNING_POP

// Disable common warnings triggered by core when compiling with -Wall
#define SUPPRESS_WARNINGS																	\
	CLANG_SUPPRESS_WARNING("-Wc++98-compat")												\
	CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")										\
	CLANG_SUPPRESS_WARNING("-Wfloat-equal")													\
	CLANG_SUPPRESS_WARNING("-Wsign-conversion")												\
	CLANG_SUPPRESS_WARNING("-Wold-style-cast")												\
	CLANG_SUPPRESS_WARNING("-Wgnu-anonymous-struct")										\
	CLANG_SUPPRESS_WARNING("-Wnested-anon-types")											\
	CLANG_SUPPRESS_WARNING("-Wglobal-constructors")											\
	CLANG_SUPPRESS_WARNING("-Wexit-time-destructors")										\
	CLANG_SUPPRESS_WARNING("-Wnonportable-system-include-path")								\
	CLANG_SUPPRESS_WARNING("-Wlanguage-extension-token")									\
	CLANG_SUPPRESS_WARNING("-Wunused-parameter")											\
	CLANG_SUPPRESS_WARNING("-Wformat-nonliteral")											\
	CLANG_SUPPRESS_WARNING("-Wcovered-switch-default")										\
	CLANG_SUPPRESS_WARNING("-Wcast-align")													\
	CLANG_SUPPRESS_WARNING("-Winvalid-offsetof")											\
	CLANG_SUPPRESS_WARNING("-Wgnu-zero-variadic-macro-arguments")							\
	CLANG_SUPPRESS_WARNING("-Wdocumentation-unknown-command")								\
	CLANG_SUPPRESS_WARNING("-Wctad-maybe-unsupported")										\
	CLANG_SUPPRESS_WARNING("-Wdeprecated-copy")												\
	CLANG_SUPPRESS_WARNING("-Wimplicit-int-float-conversion")								\
																							\
	GCC_SUPPRESS_WARNING("-Wcomment")														\
	GCC_SUPPRESS_WARNING("-Winvalid-offsetof")												\
	GCC_SUPPRESS_WARNING("-Wclass-memaccess")												\
																							\
	MSVC_SUPPRESS_WARNING(4619) /* #pragma warning: there is no warning number 'XXXX' */	\
	MSVC_SUPPRESS_WARNING(4514) /* 'X' : unreferenced inline function has been removed */	\
	MSVC_SUPPRESS_WARNING(4710) /* 'X' : function not inlined */							\
	MSVC_SUPPRESS_WARNING(4711) /* function 'X' selected for automatic inline expansion */	\
	MSVC_SUPPRESS_WARNING(4820) /* 'X': 'Y' bytes padding added after data member 'Z' */	\
	MSVC_SUPPRESS_WARNING(4100) /* 'X' : unreferenced formal parameter */					\
	MSVC_SUPPRESS_WARNING(4626) /* 'X' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted */ \
	MSVC_SUPPRESS_WARNING(5027) /* 'X' : move assignment operator was implicitly defined as deleted because a base class move assignment operator is inaccessible or deleted */ \
	MSVC_SUPPRESS_WARNING(4365) /* 'argument' : conversion from 'X' to 'Y', signed / unsigned mismatch */ \
	MSVC_SUPPRESS_WARNING(4324) /* 'X' : structure was padded due to alignment specifier */ \
	MSVC_SUPPRESS_WARNING(4625) /* 'X' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted */ \
	MSVC_SUPPRESS_WARNING(5026) /* 'X': move constructor was implicitly defined as deleted because a base class move constructor is inaccessible or deleted */ \
	MSVC_SUPPRESS_WARNING(4623) /* 'X' : default constructor was implicitly defined as deleted */ \
	MSVC_SUPPRESS_WARNING(4201) /* nonstandard extension used: nameless struct/union */		\
	MSVC_SUPPRESS_WARNING(4371) /* 'X': layout of class may have changed from a previous version of the compiler due to better packing of member 'Y' */ \
	MSVC_SUPPRESS_WARNING(5045) /* Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified */ \
	MSVC_SUPPRESS_WARNING(4583) /* 'X': destructor is not implicitly called */				\
	MSVC_SUPPRESS_WARNING(4582) /* 'X': constructor is not implicitly called */				\
	MSVC_SUPPRESS_WARNING(5219) /* implicit conversion from 'X' to 'Y', possible loss of data  */ \
	MSVC_SUPPRESS_WARNING(4826) /* Conversion from 'X *' to 'JPH::uint64' is sign-extended. This may cause unexpected runtime behavior. (32-bit) */ \
	MSVC_SUPPRESS_WARNING(5264) /* 'X': 'const' variable is not used */

// Begin the core namespace
#define CORE_NAMESPACE_BEGIN																\
	SUPPRESS_WARNING_PUSH																	\
	SUPPRESS_WARNINGS																		\
	namespace core {

// End the core namespace
#define CORE_NAMESPACE_END																	\
	}																						\
	SUPPRESS_WARNING_POP

// Declare primitive types under core namespace 
CORE_NAMESPACE_BEGIN

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

using wchar = wchar_t;

CORE_NAMESPACE_END


// Export primitive types out of core namespace 
using core::u8;
using core::u16;
using core::u32;
using core::u64;
using core::usize;
using core::i8;
using core::i16;
using core::i32;
using core::i64;
using core::isize;
using core::f32;
using core::f64;
using core::NullPtr;
using core::wchar;

#include <core/templates/remove.hpp>

// Roll our own move semantics functions
CORE_NAMESPACE_BEGIN

template <typename T>
constexpr T&& forward(typename RemoveReference<T>::Type& t) {
	return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& forward(typename RemoveReference<T>::Type&& t) {
	return static_cast<T&&>(t);
}

template <typename T>
constexpr typename RemoveReference<T>::Type&& move(T&& t) {
	return static_cast<typename RemoveReference<T>::Type&&>(t);
}

CORE_NAMESPACE_END

// Turn on asserts if compiled in debug
#if defined(_DEBUG)
	#define ENABLE_ASSERTS
#endif

SUPPRESS_WARNING_PUSH
MSVC_SUPPRESS_WARNING(4514)

bool _assert_failed(bool must_crash, const char* expression, const char* message, const char* file, u32 line);

#ifdef ENABLE_ASSERTS
	struct AssertLastParam { };
	inline bool _assert_failed_helper(bool must_crash, const char* expression, const char* file, u32 line, AssertLastParam) { return _assert_failed(must_crash, expression, nullptr, file, line); }
	inline bool _assert_failed_helper(bool must_crash, const char* expression, const char* file, u32 line, const char* message, AssertLastParam) { return _assert_failed(must_crash, expression, message, file, line); }

	// Crashes application if expression evaluates to false. Usage: ASSERT(condition) or ASSERT(condition, message)
	#define ASSERT(expression, ...) do { if (!(expression) && _assert_failed_helper(true, #expression, __FILE__, u32(__LINE__), ##__VA_ARGS__, AssertLastParam())) CRASH; } while (false)

	// Halts application if expression evaluates to false but can resume. Usage: ENSURE(condition) or ENSURE(condition, message)
	#define ENSURE(expression, ...) do { if (!(expression) && _assert_failed_helper(false, #expression, __FILE__, u32(__LINE__), ##__VA_ARGS__, AssertLastParam())) BREAKPOINT; } while (false)
#else
	#define ASSERT(...) ((void)0)
	#define ENSURE(...) ((void)0)
#endif // ENABLE_ASSERTS

SUPPRESS_WARNING_POP

#define BITFLAG(name) using name = int; enum