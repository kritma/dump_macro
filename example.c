#include "dump.h"

#define Zozin_fields                                                           \
  FIELD_INFO(uint32_t, subscribers)                                            \
  FIELD_INFO(float, salary)                                                    \
  FIELD_INFO(const char *, name)

typedef struct {
#define FIELD_INFO(type, name) DECLARE_FIELD(type, name);
  Zozin_fields
#define FIELD_INFO(type, name) PRINT_FIELD(type, name)
} Zozin;

int main() {
  Zozin var = {160000, 0, "Alexey Kutepov"};
  dump(var, Zozin_fields);

  return 0;
}
