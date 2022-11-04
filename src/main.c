#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct pp_arg {
  int32_t count;
  // AUX
  pthread_barrier_t barr;
  pthread_barrierattr_t barr_attr;
} pp_arg_t;
//////////////////////////////////////////////////////////////

static void* ping(void *arg) {
  pp_arg_t *pp_arg = (pp_arg_t*)arg;
  pthread_barrier_wait(&pp_arg->barr);
  for (int32_t i = 0; i < pp_arg->count; ++i) {
    printf("ping %d\n", i);
  }
  return NULL;
}
//////////////////////////////////////////////////////////////

static void* pong(void *arg) {
  pp_arg_t *pp_arg = (pp_arg_t*)arg;
  pthread_barrier_wait(&pp_arg->barr);
  for (int32_t i = 0; i < pp_arg->count; ++i) {
    printf("pong %d\n", i);
  }
  return NULL;
}
//////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  pp_arg_t arg = {.count = 20};
  if (pthread_barrier_init(&arg.barr, &arg.barr_attr, 2)) {
    printf("failed to init barrier\n");
    return 1;
  }

  pthread_t t_ping, t_pong;
  if (pthread_create(&t_ping, NULL, ping, (void*) &arg)) {
    printf("failed to create ping thread\n");
    return 4;
  }

  if (pthread_create(&t_pong, NULL, pong, (void*) &arg)) {
    printf("failed to create pong thread\n");
    return 5;
  }

  pthread_join(t_ping, NULL);
  pthread_join(t_pong, NULL);
  pthread_barrier_destroy(&arg.barr);
  printf("program finished\n");
  return 0;
}
////////////////////////////////////////////////////////////
