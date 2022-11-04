#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct pp_arg {
  int32_t count;
  sem_t sem_ping;
  sem_t sem_pong;
  // AUX
  pthread_barrier_t barr;
  pthread_barrierattr_t barr_attr;
} pp_arg_t;
//////////////////////////////////////////////////////////////

static void* ping(void *arg) {
  pp_arg_t *pp_arg = (pp_arg_t*)arg;
  pthread_barrier_wait(&pp_arg->barr);
  for (int32_t i = 0; i < pp_arg->count; ++i) {
    sem_wait(&pp_arg->sem_pong);
    printf("ping %d\n", i);
    sem_post(&pp_arg->sem_ping);
  }
  return NULL;
}
//////////////////////////////////////////////////////////////

static void* pong(void *arg) {
  pp_arg_t *pp_arg = (pp_arg_t*)arg;
  pthread_barrier_wait(&pp_arg->barr);
  for (int32_t i = 0; i < pp_arg->count; ++i) {
    sem_post(&pp_arg->sem_pong);
    sem_wait(&pp_arg->sem_ping);
    printf("pong %d\n", i);
    fflush(stdout);
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

  if (sem_init(&arg.sem_ping, 0, 0)) {
    printf("failed to init ping semaphore\n");
    return 2;
  }

  if (sem_init(&arg.sem_pong, 0, 0)) {
    printf("failed to init pong semaphore\n");
    return 3;
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
  sem_destroy(&arg.sem_ping);
  sem_destroy(&arg.sem_pong);

  printf("program finished\n");
  return 0;
}
////////////////////////////////////////////////////////////
