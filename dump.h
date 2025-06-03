#include "sds/sds.h"
#include <dlfcn.h>

typedef struct {
  sds name;
  sds (*to_string)(const void *);
} ToStingsFn;

size_t funs_count = 0;

sds int_to_sds(const void *v) { return sdsfromlonglong(*(uint32_t *)v); }
sds float_to_string(const void *v) {
  return sdscatfmt(sdsempty(), "%s: %c\n", *(float *)v);
}
sds string_to_sds(const void *v) { return sdsnew(*(char **)v); }

sds cant_dump(void *v) { return sdsnew("<object>"); }

sds (*get_fun(char *name))(void *v) {
  sds fname = sdscat(sdsnew("to_string_"), name);
  sds (*fn)(void *v) = dlsym(RTLD_DEFAULT, fname);
  sdsfree(fname);

  if (!fn) {
    return cant_dump;
  }

  return fn;
}

#define FIELD_TO_STRING(type, name)                                            \
  temp = _Generic((v->name), int                                           \
                       : sdscatfmt(sdsempty(), "%s: %i\n", #name, v->name), long             \
                       : sdscatfmt(sdsempty(), "%s: %I\n", #name, v->name), unsigned int         \
                       : sdscatfmt(sdsempty(), "%s: %u\n", #name, v->name), unsigned long             \
                       : sdscatfmt(sdsempty(), "%s: %U\n", #name, v->name), char            \
                       : sdscatfmt(sdsempty(), "%s: %%\n", #name, v->name), char *           \
                       : sdscatfmt(sdsempty(), "%s: %s\n", #name, v->name), const char *     \
                       : sdscatfmt(sdsempty(), "%s: %s\n", #name, v->name), float            \
                       : sdscatprintf(sdsempty(), "%s: %f\n", #name, v->name), double           \
                       : sdscatprintf(sdsempty(), "%s: %lf\n", #name, v->name), default         \
                       : sdscatfmt(sdsempty(), "%s: {\n%S}\n", #name, get_fun(#type)(&v->name)));   \
  out = sdscat(out, temp);                                                     \
  sdsfree(temp);

#define DECLARE_FIELD(type, name) type name;

#define DECLARE_STRUCT(NAME, FIELDS)                                           \
  typedef struct {                                                             \
    FIELDS(DECLARE_FIELD)                                                      \
  }(NAME);                                                                     \
  sds to_string_##NAME(NAME *v) {                                              \
    sds out = sdsempty();                                                      \
    sds temp = sdsempty();                                                     \
    FIELDS(FIELD_TO_STRING);                                                   \
    return out;                                                                \
  }
