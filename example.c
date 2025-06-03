#include "dump.h"
#include <stdint.h>
#include <stdio.h>

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
  X(Vec2, position)                                                            \
  X(Color, color)                                                              \
  X(float, rotation)                                                           \
  X(float, size)
DECLARE_STRUCT(Square);

int main() {
  Square square = {{69, 420}, {}, 3.14f / 2, 20};
  DUMP(square, Square);
  /*
    example.c:26
    square: {
      position: {
        x: 69
        y: 420
      }
      color: <object>
      rotation: 1.570000
      size: 20.000000
    }
 */
  return 0;
}
