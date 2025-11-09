#include <sys/resource.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdint.h>

#define DYLD_INTERPOSE(_replacment,_replacee) \
  __attribute__((used)) static struct{ const void* replacement; const void* replacee; } _interpose_##_replacee \
  __attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacment, (const void*)(unsigned long)&_replacee };

static int my_setpriority(int which, id_t who, int prio) { (void)which; (void)who; (void)prio; return 0; }
DYLD_INTERPOSE(my_setpriority, setpriority);

static int my_pthread_setschedparam(pthread_t t, int pol, const struct sched_param* p) { (void)t; (void)pol; (void)p; return 0; }
DYLD_INTERPOSE(my_pthread_setschedparam, pthread_setschedparam);

static int my_setrlimit(int resource, const struct rlimit *rlp) { (void)resource; (void)rlp; return 0; }
DYLD_INTERPOSE(my_setrlimit, setrlimit);

static int my_mlockall(int flags) { (void)flags; return 0; }
DYLD_INTERPOSE(my_mlockall, mlockall);
