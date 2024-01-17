#ifndef TYPES_H
#define TYPES_H

// Unsigned types
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
// Signed types
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
// Floating point types
typedef float     f32;
typedef double   f64;
// Platform-dependent types
typedef size_t   usize;
typedef ssize_t  isize;
// Points
typedef struct v2_s { f32 x, y; } v2;
typedef struct v2i_s { i32 x, y; } v2i;

#endif // TYPES_H
