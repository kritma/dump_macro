#include "sds/sds.h"
#include <dlfcn.h>
#include <stdio.h>

sds cant_dump(void *v /*unused*/, sds /*unused*/ p) {
  return sdsnew("<object>");
}

sds (*get_fun(char *name))(void *v, sds spit) {
  sds fname = sdscat(sdsnew(name), "_to_string");
  sds (*fn)(void *v, sds padding) = dlsym(RTLD_DEFAULT, fname);
  sdsfree(fname);

  if (!fn) {
    return cant_dump;
  }

  return fn;
}

#define FIELD_TO_STRING(type, name)                                            \
  temp = _Generic((v->name), int                                           \
                       : sdscatfmt(padd, "%s: %i", #name, v->name), long             \
                       : sdscatfmt(padd, "%s: %I", #name, v->name), unsigned int         \
                       : sdscatfmt(padd, "%s: %u", #name, v->name), unsigned long             \
                       : sdscatfmt(padd, "%s: %U", #name, v->name), char            \
                       : sdscatfmt(padd, "%s: %%", #name, v->name), char *           \
                       : sdscatfmt(padd, "%s: %s", #name, v->name), const char *     \
                       : sdscatfmt(padd, "%s: %s", #name, v->name), float            \
                       : sdscatprintf(padd, "%s: %f", #name, v->name), double           \
                       : sdscatprintf(padd, "%s: %lf", #name, v->name), default         \
                       : sdscatfmt(padd, "%s: %S", #name, trash = get_fun(#type)(&v->name, sdscat(sdsdup(padding), "  "))));   \
  out = sdscat(out, temp);                                                     \
  sdsfree(temp);                                                               \
  sdsfree(trash);                                                              \
  trash = NULL;                                                                \
  padd = sdscat(sdsnew("\n  "), padding);

#define DECLARE_FIELD(type, name) type name;

#define DECLARE_STRUCT(TYPE_NAME)                                              \
  typedef struct {                                                             \
    TYPE_NAME##_FIELDS(DECLARE_FIELD)                                          \
  }(TYPE_NAME);                                                                \
  sds TYPE_NAME##_to_string(TYPE_NAME *v, sds padding) {                       \
    int count = 0;                                                             \
    sds out = sdsnew("{\n");                                                   \
    sds temp = sdsempty();                                                     \
    sds padd = sdscat(sdsnew("  "), padding);                                  \
    sds trash = sdsempty();                                                    \
    _Pragma("clang diagnostic push");                                          \
    _Pragma("clang diagnostic ignored \"-Wformat\"");                          \
    TYPE_NAME##_FIELDS(FIELD_TO_STRING);                                       \
    _Pragma("clang diagnostic pop");                                           \
    out = sdscatfmt(out, "\n%S}", padding);                                    \
    return out;                                                                \
  }

#define DUMP(var, TYPE_NAME)                                                   \
  {                                                                            \
    sds str = TYPE_NAME##_to_string(&(var), sdsempty());                       \
    printf("%s:%d\n%s: %s\n", __FILE__, __LINE__, #var, str);                  \
    sdsfree(str);                                                              \
  }