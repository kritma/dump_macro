#include "dump.h"
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
  Student var = {18, "Egorchik", 8};
  sds s = to_string_Student(&var);
  printf("%s\n", s);
  /*
   person: {
   age: 18
   name: Egorchik
   }
   avg: 8.000000
 */
  sdsfree(s);
  return 0;
}
