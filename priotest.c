#include <stdio.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>
int main(){
  if (setpriority(PRIO_PROCESS, 0, -20) != 0) {
    printf("setpriority failed: %s\n", strerror(errno));
  } else {
    printf("setpriority ok\n");
  }
  return 0;
}
