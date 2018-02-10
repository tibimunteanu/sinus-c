#ifndef DEFINES_H
#define DEFINES_H 

#include<stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t b32;
typedef float r32;
typedef double r64;
typedef size_t memory_index;
typedef wchar_t wchar;
typedef ptrdiff_t ptr_diff;
typedef intptr_t imm;
typedef uintptr_t umm;

#define internal static
#define global static

#define Kilobytes(value) ((value) * 1024LL)
#define Megabytes(value) (Kilobytes(value) * 1024LL)
#define Gigabytes(value) (Megabytes(value) * 1024LL)
#define TeraBytes(value) (Gigabytes(value) * 1024LL)
#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))
#define Min(A, B) ((A < B) ? (A) : (B))
#define Max(A, B) ((A > B) ? (A) : (B))
#define InvalidCodePath Assert(!"Invalid code path")
#define InvalidDefaultCase default: { InvalidCodePath; } break
#define AsciiCode(a, b, c, d) (((u32)(a) << 0) | ((u32)(b) << 8) | ((u32)(c) << 16) | ((u32)(d) << 24))

#if SINUS_DEBUG
#define Assert(expression) if(!(expression)) { *(int*)0 = 0; }
#else
#define Assert(expression)
#endif

#define InvalidCodePath Assert(!"Invalid code path")
#define InvalidDefaultcase default: {Assert(!"Invalid default case")} break
#define NotImplemented Assert(!"Not implemented")

#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif
    
#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif

#if !COMPILER_MSVC && !COMPILER_LLVM
#if _MSC_VER
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#else
#undef COMPILER_LLVM
#define COMPILER_LLVM 1
#endif
#endif

#if COMPILER_MSVC
#include <intrin.h>
#elif COMPILER_LLVM
#include <x86intrin.h>
#else
#error SSE/NEON optimizations are not available for this compiler yet!!!!
#endif

global b32 SSE2;
global b32 SSE3;
global b32 SSSE3;
global b32 SSE41;
global b32 SSE42;
global b32 AVX;

internal void DetectInstructionSets()
{
    i32 info[4];
    __cpuid(info, 1);
    i32 info3 = info[3];
    SSE2 = ((info3 >> 26) & 1) != 0;
    SSE3 = ((info3 >> 0) & 1) != 0;
    SSSE3 = ((info3 >> 9) & 1) != 0;
    SSE41 = ((info3 >> 19) & 1) != 0;
    SSE42 = ((info3 >> 20) & 1) != 0;
    AVX = ((info3 >> 28) & 1) != 0;
}

#endif /* DEFINES_H */
