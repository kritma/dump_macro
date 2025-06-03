# derive(Debug) in C
powered by magic

```c
#include "dump.h"

#define Vec2_FIELDS(X)                                                         \
  X(uint32_t, x)                                                               \
  X(uint32_t, y)
DECLARE_STRUCT(Vec2);

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} Color;

#define Square_FIELDS(X)                                                       \
  X(Vec2 *, position)                                                          \
  X(Color, color)                                                              \
  X(float, rotation)                                                           \
  X(const float, size)
DECLARE_STRUCT(Square);

int main() {
  Vec2 pos = {69, 420};
  Square square = {&pos, {}, 3.14f / 2, 20};
  DUMP(square, Square);
  /*
    example.c:25
    square(Square): {
      position(Vec2 *): {
        x(uint32_t): 69
        y(uint32_t): 420
      }
      color(Color): <unknown>
      rotation(float): 1.570000
      size(const float): 20.000000
    }
 */
  return 0;
}

```

Actually uses dlsym, works on macos, maybe on linux too.
