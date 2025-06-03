# derive(Debug) in C
powered by magic

```c
#include "dump.h"
#include "sds/sds.h"
#include <stdio.h>

#define PERSON_FIELDS(X)                                                       \
  X(uint32_t, age)                                                             \
  X(const char *, name)

DECLARE_STRUCT(Person, PERSON_FIELDS);

#define STUDENT_FIELDS(X)                                                      \
  X(Person, person)                                                            \
  X(float, avg)

DECLARE_STRUCT(Student, STUDENT_FIELDS);

int main() {
  Student var = {18, "Egorchik"};
  sds s = to_string_Student(&var);
  printf("%s\n", s);
  sdsfree(s);
  return 0;
}

```

Actually uses dlsym, works on macos, maybe on linux too.
