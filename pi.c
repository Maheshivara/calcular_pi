#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// Montando uma estrutura para passar informações para a thread
typedef struct {
  unsigned long long int start;
  unsigned long long int end;
} ThreadData;

// Declarando pi como global para facilitar acesso pelas threads
double pi = 0;

// Iniciando um mutex (no mesmo escopo) para proteger pi quando necessário
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* leibniz(void* arg) {
  // Convertendo o argumento recebido pela thread em uma struct
  ThreadData* data = (ThreadData*)arg;

  double parcial = 0;
  // Usando a série de Leibniz
  for (unsigned long long int i = data->start; i < data->end; ++i) {
    double term = 1.0 / (2 * i + 1);
    if (i % 2 == 0)
      parcial = parcial + term;
    else
      parcial = parcial - term;
  }

  // Bloqueando as outras threads de fazer alteração no valor
  // enquanto essa estiver alterando
  pthread_mutex_lock(&mutex);
  pi += parcial;
  // Desbloqueando o valor para as outras threads
  pthread_mutex_unlock(&mutex);
  // Fechando a thread
  pthread_exit(NULL);
}

int main() {
  int num_threads, remainder;
  unsigned long long int total_terms, divided_terms_per_thread;
  struct timespec start, end;
  double time_spend;

  // Solicitando o número de threads a ser usado
  printf("Digite o numero de threads: ");
  scanf("%d", &num_threads);

  if (num_threads <= 0) {
    printf("Numero de threads deve ser maior ou igual a 1\n");
    return 1;
  }

  // Pedindo a quantidade total de termos para serem processados
  printf("Digite a quantidade total de termos: ");
  scanf("%llu", &total_terms);

  if (total_terms < 10) {
    printf("Quantidade de termos deve ser maior ou igual a 10\n");
    return 1;
  }

  // Obtendo a quantidade de termos que cada thread tem de processar
  divided_terms_per_thread = total_terms / num_threads;
  remainder = total_terms % num_threads;

  // Criando um vetor de threads e de informações para as threads
  pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
  ThreadData* thread_info = malloc(num_threads * sizeof(ThreadData));

  // Armazenando o tempo inicial das threads
  clock_gettime(CLOCK_MONOTONIC, &start);

  // Configurando todas as threads para rodarem a função da série de Leibniz com as informações de cada thread
  unsigned long long int current = 0;
  for (long i = 0; i < num_threads; ++i) {
    thread_info[i].start = current;
    // Dividindo os termos que sobraram entre as threads
    if (remainder > 0) {
      thread_info[i].end = divided_terms_per_thread + current + 1;
      current = current + divided_terms_per_thread + 1;
      remainder--;
    }
    else {
      thread_info[i].end = divided_terms_per_thread + current;
      current = current + divided_terms_per_thread;
    }

    pthread_create(&threads[i], NULL, leibniz, &thread_info[i]);
  }

  // Espera pela finalização da execução das threads
  for (int i = 0; i < num_threads; ++i) {
    pthread_join(threads[i], NULL);
  }

  // Armazenando o tempo final e realizando o calculo do tempo gasto nas threads
  clock_gettime(CLOCK_MONOTONIC, &end);
  time_spend = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1000000000.0);

  // Liberando os vetores e destruindo o mutex
  free(thread_info);
  free(threads);
  pthread_mutex_destroy(&mutex);

  // Multiplicando o resultado por quatro para obter o valor de pi
  pi = pi * 4.0;

  // Imprimindo valor e tempo gasto nas threads
  printf("Valor estimado para pi: %f\n", pi);
  printf("Tempo nas threads: %f segundos\n", time_spend);

  return 0;
}
