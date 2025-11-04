#include <sys/resource.h>
#include <stdint.h>

// dyld interpose helper
#define DYLD_INTERPOSE(_replacment,_replacee) \
  __attribute__((used)) static struct{ const void* replacement; const void* replacee; } _interpose_##_replacee \
  __attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacment, (const void*)(unsigned long)&_replacee };

static int my_setpriority(int which, id_t who, int prio) {
  // Ignore attempts to raise priority (negative nice) that would require root.
  // Pretend success so libraries that treat EPERM as fatal can proceed.
  (void)which; (void)who; (void)prio;
  return 0;
}

DYLD_INTERPOSE(my_setpriority, setpriority);
