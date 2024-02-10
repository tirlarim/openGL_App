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
typedef struct v2u_s { u32 x, y; } v2u;
typedef struct v3_s { f32 x, y, z; } v3;
typedef struct v3i_s { i32 x, y, z; } v3i;
typedef struct v3u_s { u32 x, y, z; } v3u;

typedef struct Settings {
  f32 transparency = 0.2f;
}SETTINGS;
typedef struct Texture {
  u32 id = 0;
  std::string path;
  bool isRGBA = false;
}TEXTURE;
typedef struct Camera_s {
  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
} Camera;

#endif // TYPES_H
