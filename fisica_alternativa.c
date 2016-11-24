#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct imagem {
    int altura;
    int largura;
    int maxVal;
    double ***matriz;
};

typedef struct imagem Imagem;


void alocaMatriz(Imagem *imagem)
{
    int i, j;

    double ***matriz = malloc(imagem->altura * sizeof(double **));

    if (matriz == NULL) {
        printf("Malloc nao conseguiu alocar!!!\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < imagem->altura; i++) {
        matriz[i] = malloc(imagem->largura * sizeof(double *));
        if (matriz[i] == NULL) {
            printf("Malloc i nao conseguiu alocar!!!\n");
            exit(EXIT_FAILURE);
        }

        for (j = 0; j < imagem->largura; j++) {
            matriz[i][j] = malloc(3 * sizeof(double));
            if (matriz[i][j] == NULL) {
                printf("Malloc i j nao conseguiu alocar!!!\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    imagem->matriz = matriz;
}


void liberaMatriz(Imagem *imagem)
{
    int i, j;

    for (i = 0; i < imagem->altura; i++) {
        for (j = 0; j < imagem->largura; j++) {
            free(imagem->matriz[i][j]);
            imagem->matriz[i][j] = NULL;
        }

        free(imagem->matriz[i]);
        imagem->matriz[i] = NULL;
    }


    free(imagem->matriz);
    imagem->matriz = NULL;
}


void imprimeMatriz(Imagem imagem)
{
    int i, j;

    for (i = 0; i < imagem.altura; i++) {
        for (j = 0; j < imagem.largura; j++) {
            printf("(%.2f", imagem.matriz[i][j][0]);
            printf(" %.2f", imagem.matriz[i][j][1]);
            printf(" %.2f) ", imagem.matriz[i][j][2]);
        }
        printf("\n");
    }
}


void lePPM(char nomeArquivo[], Imagem *imagem)
{
    FILE *fp;

    int i, j;
    int r, g, b;

    char magicNumber[3];
    magicNumber[2] = '\0';

    fp = fopen(nomeArquivo, "r");
    if (fp == NULL) {
        printf("Nao foi possivel abrir o arquivo '%s'\n", nomeArquivo);
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%c %c", &magicNumber[0], &magicNumber[1]);
    if (strcmp(magicNumber, "P3") != 0) {
        printf("Numero magico errado!\nEsperado: P3\n Encontrado: %s\n", magicNumber);
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d %d %d", &(imagem->altura), &(imagem->largura), &(imagem->maxVal));

    alocaMatriz(imagem);

    for (i = 0; i < imagem->altura; i++) {
        for (j = 0; j < imagem->largura; j++) {
            fscanf(fp, "%d %d %d", &r, &g, &b);

            imagem->matriz[i][j][0] = (double) r / imagem->maxVal;
            imagem->matriz[i][j][1] = (double) g / imagem->maxVal;
            imagem->matriz[i][j][2] = (double) b / imagem->maxVal;
        }
    }

    fclose(fp);
}


void escrevePPM(char nomeArquivo[], Imagem imagem)
{
    FILE *fp;

    int i, j;
    double r, g, b;

    fp = fopen(nomeArquivo, "w");
    if (fp == NULL) {
        printf("Nao foi possivel abrir o arquivo '%s'\n", nomeArquivo);
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "P3\n%d %d\n%d\n", imagem.altura, imagem.largura, imagem.maxVal);

    for (i = 0; i < imagem.altura; i++) {
        for (j = 0; j < imagem.largura; j++) {
            r = round(imagem.matriz[i][j][0] * imagem.maxVal);
            g = round(imagem.matriz[i][j][1] * imagem.maxVal);
            b = round(imagem.matriz[i][j][2] * imagem.maxVal);

            fprintf(fp, "%d %d %d", (int) r, (int) g, (int) b);
            if (j != (imagem.largura - 1))
                fprintf(fp, " ");
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}


void calculaComponentesRB(double pixel[], double componentes[])
{
    double constanteX = sin(2 * M_PI * pixel[1]);
    double constanteY = cos(2 * M_PI * pixel[1]);

    componentes[0] =  pixel[0] * constanteX; // R_x =  |R| sin(2pi G)
    componentes[1] =  pixel[0] * constanteY; // R_y =  |R| cos(2pi G)
    componentes[2] = -pixel[2] * constanteX; // B_x = -|B| sin(2pi G)
    componentes[3] = -pixel[2] * constanteY; // B_y = -|B| cos(2pi G)
}


double calculaDeltaAtualizacaoRB(double componente, char identificador, double pixelVizinho[])
{
    double delta;

    /* O identificador indica qual cor esta sendo considerada. */
    /* R = vermelho, B = azul                                  */

    if (identificador == 'R')
        delta = ((1 - pixelVizinho[0]) * fabs(componente)) / 4;
    else if (identificador == 'B')
        delta = ((1 - pixelVizinho[2]) * fabs(componente)) / 4;
    else {
        printf("Identificador '%c' invalido!\n", identificador);
        exit(EXIT_FAILURE);
    }

    return delta;
}


double calculaCorAtualizada(char identificador[], double delta, double componentes[])
{
    double componenteAtualizado, corAtualizada;

    /* O identificador indica qual componente de qual cor esta sendo */
    /* considerado.                                                  */
    /*                                                               */
    /* R_x = x da cor vermelha, R_y = y da cor vermelha              */
    /* B_x = x da cor azul,     B_y = y da cor azul                  */

    if (strcmp(identificador, "R_x") == 0) {
        componenteAtualizado = componentes[0] + delta;
        corAtualizada = sqrt(pow(componenteAtualizado, 2) + pow(componentes[1], 2));
    }

    else if (strcmp(identificador, "R_y") == 0) {
        componenteAtualizado = componentes[1] + delta;
        corAtualizada = sqrt(pow(componenteAtualizado, 2) + pow(componentes[0], 2));
    }

    else if (strcmp(identificador, "B_x") == 0) {
        componenteAtualizado = componentes[2] + delta;
        corAtualizada = sqrt(pow(componenteAtualizado, 2) + pow(componentes[3], 2));
    }

    else if (strcmp(identificador, "B_y") == 0) {
        componenteAtualizado = componentes[3] + delta;
        corAtualizada = sqrt(pow(componenteAtualizado, 2) + pow(componentes[2], 2));
    }

    else {
        printf("Identificador '%s' invalido!\n", identificador);
        exit(EXIT_FAILURE);
    }

    return corAtualizada;
}


double min(double a, double b) { return (a < b) ? a : b; }


void corrigeValoresAtualizaVerde(Imagem *imagem)
{
    int i, j;

    double excedente, menor;

    double componentes[4], angulo, thetaG;

    for (i = 1; i < imagem->altura - 1; i++) {
        for (j = 1; j < imagem->largura - 1; j++) {

            if (imagem->matriz[i][j][0] > 1) {
                excedente = 1 - imagem->matriz[i][j][0];

                if (i > 1) {
                    menor = min(1, imagem->matriz[i - 1][j][0] + excedente/4);
                    imagem->matriz[i - 1][j][0] = menor;
                }

                if (i < imagem->altura - 2) {
                    menor = min(1, imagem->matriz[i + 1][j][0] + excedente/4);
                    imagem->matriz[i + 1][j][0] = menor;
                }

                if (j > 1) {
                    menor = min(1, imagem->matriz[i][j - 1][0] + excedente/4);
                    imagem->matriz[i][j - 1][0] = menor;
                }

                if (j < imagem->largura - 2) {
                    menor = min(1, imagem->matriz[i][j + 1][0] + excedente/4);
                    imagem->matriz[i][j + 1][0] = menor;
                }
            }

            if (imagem->matriz[i][j][0] < 0)
                imagem->matriz[i][j][0] = 0;

            if (imagem->matriz[i][j][2] > 1) {
                excedente = 1 - imagem->matriz[i][j][2];

                if (i > 1) {
                    menor = min(1, imagem->matriz[i - 1][j][2] + excedente/4);
                    imagem->matriz[i - 1][j][2] = menor;
                }

                if (i < imagem->altura - 2) {
                    menor = min(1, imagem->matriz[i + 1][j][2] + excedente/4);
                    imagem->matriz[i + 1][j][2] = menor;
                }

                if (j > 1) {
                    menor = min(1, imagem->matriz[i][j - 1][2] + excedente/4);
                    imagem->matriz[i][j - 1][2] = menor;
                }

                if (j < imagem->largura - 2) {
                    menor = min(1, imagem->matriz[i][j + 1][2] + excedente/4);
                    imagem->matriz[i][j + 1][2] = menor;
                }
            }

            if (imagem->matriz[i][j][2] < 0)
                imagem->matriz[i][j][2] = 0;

            if (imagem->matriz[i][j][0] == 0)
                angulo = 0;
            else {
                calculaComponentesRB(imagem->matriz[i][j], componentes);
                // angulo = asin(componentes[0] / imagem->matriz[i][j][0]);
                angulo = atan(imagem->matriz[i][j][0] / imagem->matriz[i][j][2]);
            }

            thetaG = imagem->matriz[i][j][1] * 2 * M_PI;
            thetaG += angulo;
            imagem->matriz[i][j][1] = thetaG / (2 * M_PI);
            if (imagem->matriz[i][j][1] > 1)
                imagem->matriz[i][j][1] -= (int) imagem->matriz[i][j][1];
        }
    }
}


void atualizaImagem(Imagem *imagem)
{
    int i, j;

    double componentes[4], componentesVizinho[4], delta;

    for (i = 1; i < imagem->altura - 1; i++) {
        for (j = 1; j < imagem->largura - 1; j++) {

            calculaComponentesRB(imagem->matriz[i][j], componentes);
            
            if (componentes[0] > 0 && i != imagem->altura - 2) {
                calculaComponentesRB(imagem->matriz[i + 1][j], componentesVizinho);
                delta = calculaDeltaAtualizacaoRB(componentes[0], 'R', imagem->matriz[i + 1][j]);
                imagem->matriz[i + 1][j][0] = calculaCorAtualizada("R_x", delta, componentesVizinho);
            }

            else if (componentes[0] < 0 && i != 1) {
                calculaComponentesRB(imagem->matriz[i - 1][j], componentesVizinho);
                delta = calculaDeltaAtualizacaoRB(componentes[0], 'R', imagem->matriz[i - 1][j]);
                imagem->matriz[i - 1][j][0] = calculaCorAtualizada("R_x", delta, componentesVizinho);
            }

            if (componentes[1] > 0 && j != imagem->largura - 2) {
                calculaComponentesRB(imagem->matriz[i][j + 1], componentesVizinho);                
                delta = calculaDeltaAtualizacaoRB(componentes[1], 'R', imagem->matriz[i][j + 1]);
                imagem->matriz[i][j + 1][0] = calculaCorAtualizada("R_y", delta, componentesVizinho);
            }

            else if (componentes[1] < 0 && j != 1) {
                calculaComponentesRB(imagem->matriz[i][j - 1], componentesVizinho);                
                delta = calculaDeltaAtualizacaoRB(componentes[1], 'R', imagem->matriz[i][j - 1]);
                imagem->matriz[i][j - 1][0] = calculaCorAtualizada("R_y", delta, componentesVizinho);
            }

            if (componentes[2] > 0 && i != imagem->altura - 2) {
                calculaComponentesRB(imagem->matriz[i + 1][j], componentesVizinho);                
                delta = calculaDeltaAtualizacaoRB(componentes[2], 'B', imagem->matriz[i + 1][j]);
                imagem->matriz[i + 1][j][2] = calculaCorAtualizada("B_x", delta, componentesVizinho);
            }

            else if (componentes[2] < 0 && i != 1) {
                calculaComponentesRB(imagem->matriz[i - 1][j], componentesVizinho);                
                delta = calculaDeltaAtualizacaoRB(componentes[2], 'B', imagem->matriz[i - 1][j]);
                imagem->matriz[i - 1][j][2] = calculaCorAtualizada("B_x", delta, componentesVizinho);
            }

            if (componentes[3] > 0 && j != imagem->largura - 2) {
                calculaComponentesRB(imagem->matriz[i][j + 1], componentesVizinho);                
                delta = calculaDeltaAtualizacaoRB(componentes[3], 'B', imagem->matriz[i][j + 1]);
                imagem->matriz[i][j + 1][2] = calculaCorAtualizada("B_y", delta, componentesVizinho);
            }

            else if (componentes[3] < 0 && j != 1) {
                calculaComponentesRB(imagem->matriz[i][j - 1], componentesVizinho);                
                delta = calculaDeltaAtualizacaoRB(componentes[3], 'B', imagem->matriz[i][j - 1]);
                imagem->matriz[i][j - 1][2] = calculaCorAtualizada("B_y", delta, componentesVizinho);
            }
        }
    }
    corrigeValoresAtualizaVerde(imagem);
}


void atualiza(Imagem *imagem, int nVezes, char nomeArquivo[], int salvaSempre, int log)
{
    int i;

    for (i = 0; i < nVezes; i++) {
        atualizaImagem(imagem);

        if (salvaSempre)
            escrevePPM(nomeArquivo, *imagem);

        if (log)
            printf("Iteracao %d\n", i + 1);
    }

    escrevePPM(nomeArquivo, *imagem);
}


int main(int argc, char *argv[])
{
    Imagem imagem;

    if (argc < 4) {
        printf("Faltam argumentos!!! Só tem %d\n", argc - 1);
        return 1;
    }

    lePPM(argv[1], &imagem);

    atualiza(&imagem, atoi(argv[3]), argv[2], 1, 1);

    liberaMatriz(&imagem);

    return 0;
}
