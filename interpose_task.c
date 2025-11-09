#include <mach/mach.h>
#include <stdint.h>
#define DYLD_INTERPOSE(_replacment,_replacee) \
  __attribute__((used)) static struct{ const void* replacement; const void* replacee; } _interpose_##_replacee \
  __attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacment, (const void*)(unsigned long)&_replacee };

static kern_return_t my_task_policy_set(task_t task, task_policy_flavor_t flavor, task_policy_t policy_info, mach_msg_type_number_t count) {
  (void)task; (void)flavor; (void)policy_info; (void)count; return KERN_SUCCESS;
}
DYLD_INTERPOSE(my_task_policy_set, task_policy_set);

static kern_return_t my_task_policy_get(task_t task, task_policy_flavor_t flavor, task_policy_t policy_info, mach_msg_type_number_t *count, boolean_t *get_default) {
  (void)task; (void)flavor; if (count) *count=0; if (get_default) *get_default=1; return KERN_SUCCESS;
}
DYLD_INTERPOSE(my_task_policy_get, task_policy_get);
