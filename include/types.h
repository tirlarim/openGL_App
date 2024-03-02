#pragma once

#include <string>
#include <glm/vec3.hpp>
#include "colors.h"

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
// Vectors
typedef struct v2_s { f32 x, y; } v2;
typedef struct v2i8_s { i8 x, y; } v2i8;
typedef struct v2i16_s { i16 x, y; } v2i16;
typedef struct v2i_s { i32 x, y; } v2i;
typedef struct v2u8_s { u8 x, y; } v2u8;
typedef struct v2u16_s { u16 x, y; } v2u16;
typedef struct v2u_s { u32 x, y; } v2u;
typedef struct v3_s { f32 x, y, z; } v3;
typedef struct v3i8_s { i8 x, y, z; } v3i8;
typedef struct v3i16_s { i16 x, y, z; } v3i16;
typedef struct v3i_s { i32 x, y, z; } v3i;
typedef struct v3u8_s { u8 x, y; } v3u8;
typedef struct v3u16_s { u16 x, y; } v3u16;
typedef struct v3u_s { u32 x, y, z; } v3u;
// App types
typedef struct Settings_s {
  f32 transparency = 0.2f;
  bool pause = false;
}Settings;
typedef struct Material_s {
  std::string name;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  f32 shininess;
}Material;
typedef struct Light_s { // default light settings
  glm::vec3 ambient = glm::vec3(0.1f);
  glm::vec3 diffuse = glm::vec3(0.5f);
  glm::vec3 specular = glm::vec3(1.0f);
  f32 constant = 1.0f;
  f32 linear = 0.09f;
  f32 quadratic = 0.032f;
}Light;
// Custom types
typedef union bit2char_u {
  char character;
  struct bit_s {
    u8 b8 : 1;
    u8 b7 : 1;
    u8 b6 : 1;
    u8 b5 : 1;
    u8 b4 : 1;
    u8 b3 : 1;
    u8 b2 : 1;
    u8 b1 : 1;
  }bit;
}bit2char;
