# Calculando Pi Usando Threads

## Sobre

Atividade apresentada na disciplina de Sistemas Operacionais durante o 3º período de Sistemas de Informação no Instituto Federal de Alagoas Campus Arapiraca para fins avaliativos.

### Equipe

- [Augusto Lumes](https://github.com/augustolumes)
- Humberto Augusto
- Lucas Silva
- [Luis Gabriel](https://github.com/Maheshivara)
- [Wallace Souza](https://github.com/RochaSWallace)

## Como Funciona

O código se utiliza da biblioteca pthreads para dividir a carga de trabalho em calcular os termos da série de Leibniz entre diferentes threads, agilizando consideravelmente a operação.
Usamos também a biblioteca time para calcular o tempo gasto dentro das threads por meio da função clock_gettime() e o MONOTONIC_CLOCK (o que só está disponível em sistemas baseados em UNIX, em sua maioria).

## Como Rodar

Usando um sistema linux (ou WSL) use o gcc para compilar o arquivo com a flag -pthread tal como:

```bash
gcc ./pi.c -pthread -o ./pi
```

Em seguida basta executar o binário gerado com:

```bash
./pi
```

O programa irá então perguntar a quantidade de threads que você deseja usar e quantos termos ele deve calcular, após o cálculo apresentará em tela o valor estimado de pi por meio da série e o tempo gasto nas threads.
