#include <pthread.h>
#include <stdint.h>
#define DYLD_INTERPOSE(_replacment,_replacee) \
  __attribute__((used)) static struct{ const void* replacement; const void* replacee; } _interpose_##_replacee \
  __attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacment, (const void*)(unsigned long)&_replacee };

static int my_pthread_set_qos_class_self_np(int __qos_class, int __relative_priority) {
  (void)__qos_class; (void)__relative_priority; return 0;
}
DYLD_INTERPOSE(my_pthread_set_qos_class_self_np, pthread_set_qos_class_self_np);
