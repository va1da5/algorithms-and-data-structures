#define ASSERT(c, m)                                                           \
  do {                                                                         \
    if (!(c)) {                                                                \
      fprintf(stderr, __FILE__ ":%d: assertion %s failed: %s\n", __LINE__, #c, \
              m);                                                              \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)
