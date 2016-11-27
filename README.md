# MAC0431-Projeto
Projeto de MAC0431 (Introdução à Computação Paralela e Distribuída)

Para compilar a versão sequencial:
`gcc fisica_alt.c -o fisica_alt -lm`

Para compilar a versão paralela:
`gcc -fopenmp fisica_alt_openmp.c -o fisica_alt_openmp -lm`

Para rodar a versão sequencial:
`./fisica_alt <nome_arquivo_entrada> <nome_arquivo_saida> <numero_de_iteracoes>`

Para rodar a versão paralela:
`./fisica_alt_openmp <nome_arquivo_entrada> <nome_arquivo_saida> <numero_de_iteracoes>`

Onde o arquivo de entrada e o de saída são arquivos .ppm no formato P3 (Colorido ASCII).

Na versão paralela, o número de threads é definido pela variável de ambiente `OMP_NUM_THREADS` (o valor default *aparentemente* é o número de processadores na máquina, mas melhor modificar a variável de ambiente pra ter certeza).

Pra modificar a variável de ambiente:
`export OMP_NUM_THREADS=<num_threads>`
