# MAC0431-Projeto
Projeto de MAC0431 (Introdução à Computação Paralela e Distribuída)

Alunos:
Fabio B. Muller - NUSP: 8536127
Leonardo Daneu Lopes - NUSP: 8516816
Lucas S. J. Hong - NUSP: 8124329

Para compilar:

    make

Para executar:

    ./paralelo <nome_arq_entrada> <nome_arq_saida> <numero_iteracoes> <numero_threads>

Onde o arquivo de entrada e o de saída são arquivos `.ppm` no formato **P3** (Colorido ASCII).


## OpenMP

Foi utilizada a configuração `static` do OpenMP sem especificar tamanho de pedaço, o que divide igualmente (se possível) o número de iterações pelas threads.

Assim, cada thread executa um bloco de linhas e não há problema com as dependências porque threads diferentes estarão executando linhas que não são seguidas, ou seja, uma não influencia o cálculo da atualização dos pixels da outra.


# Testes

Computadores testados:

* Computadores da rede Linux
  * 8 processadores Intel(R) Xeon(R) CPU, 2.66GHz
  * Mem: 8004 MB
* Netbook HP
  * 2 processadores Intel(R) Atom(TM) CPU, 1.66GHz
  * Mem: 2035 MB


## Arquivos de testes

Observação: As imagens usadas nos testes estão disponíveis no repositório do GitHub do projeto: https://github.com/fabiobm/MAC0431-Projeto

Número de iterações: 25

### Imagem: zwei.ppm

*Dimensão:* 500x500 pixels

*Tamanho:* 2.5 MB

Computador: rede Linux

| time | 1 thread | 8 threads | 20 threads |
|:-----|:---------|:----------|:-----------|
| real | 0m6.113s | 0m1.830s  | 0m1.489s   |
| user | 0m5.812s | 0m7.408s  | 0m6.096s   |
| sys  | 0m0.020s | 0m0.020s  | 0m0.024s   |

Computador: Netbook HP

| time | 1 thread  | 8 threads | 20 threads |
|:-----|:----------|:----------|:-----------|
| real | 0m23.878s | 0m16.203s | 0m15.729s  |
| user | 0m23.740s | 0m28.900s | 0m28.996s  |
| sys  | 0m0.064s  | 0m0.120s  | 0m0.124s   |


### Imagem: minecraft.ppm

*Dimensão:* 1500x1500 pixels

*Tamanho:* 21.4 MB

Computador: rede Linux

| time | 1 thread  | 8 threads | 20 threads |
|:-----|:----------|:----------|:-----------|
| real | 0m52.537s | 0m13.242s | 0m11.052s  |
| user | 0m50.124s | 0m52.224s | 0m50.984s  |
| sys  | 0m0.100s  | 0m0.132s  | 0m0.156s   |

Computador: Netbook HP

| time | 1 thread  | 8 threads | 20 threads |
|:-----|:----------|:----------|:-----------|
| real | 3m28.295s | 2m17.913s | 2m18.108s  |
| user | 3m27.324s | 4m14.812s | 4m15.056s  |
| sys  | 0m0.524s  | 0m0.584s  | 0m0.620s   |
