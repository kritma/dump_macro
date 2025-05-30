#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PRINT_FIELD(type, name)                                                \
  printf(_Generic((var.name), \
        uint32_t: "%s: %d\n", \
        float: "%s: %f\n", \
        const char*: "%s: %s\n",\
        default: "well fuck"\
    ), #name, var.name);

#define DECLARE_FIELD(type, name) type name;

#define dump(v, FIELDS)                                                        \
  {                                                                            \
    __auto_type *_var_ptr = &(v);                                              \
    __auto_type var = *_var_ptr;                                               \
    FIELDS                                                                     \
  }