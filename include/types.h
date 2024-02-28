#ifndef TYPES_H
#define TYPES_H

#include <string>

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
typedef struct v2u_s { u32 x, y; } v2u;
typedef struct v3_s { f32 x, y, z; } v3;
typedef struct v3i_s { i32 x, y, z; } v3i;
typedef struct v3u_s { u32 x, y, z; } v3u;

typedef struct Settings_s {
  f32 transparency = 0.2f;
  bool pause = false;
}Settings;
typedef struct Texture_s {
  u32 id = 0;
  std::string path;
}Texture;
typedef struct Material_s {
  std::string name;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  f32 shininess;
}Material;

#endif // TYPES_H
