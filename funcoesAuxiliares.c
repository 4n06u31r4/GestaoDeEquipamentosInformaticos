#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcoesAuxiliares.h"


// ----------------------- FUNÇÕES AUXILIARES ------------------------------------------


void lerString(char msg[TEXTO_LONGO], char str[TEXTO_LONGO], int tamanho)
{
    int tamTexto;

    do
    {
        printf("%s", msg);
        fgets(str, tamanho, stdin);

        tamTexto = strlen(str);

        if (tamTexto <= 1)
        {
            printf("\n\nERRO: tem de inserir um valor.\n");
        }
        else
        {
            if (str[tamTexto-1] == '\n')
            {
                str[tamTexto-1] = '\0';
            }
            else
            {
                limparBuffer();
            }
        }
    }
    while (tamTexto <= 1);
}


int lerInteiro (char msg[TEXTO_LONGO], int limMin, int limMax)
{
    int num, res;


    do
    {
        printf("%s", msg);
        res = scanf("%d", &num);
        limparBuffer();

        if (res != 1)
        {
            printf("\n\nERRO: o valor inserido tem que ser um numero inteiro.\n");
        }
        else
        {
            if (num < limMin || num > limMax)
            {
                printf("\n\nERRO: o numero tem de pertencer ao intervalo [%d, %d].\n", limMin, limMax);
            }
        }

    }
    while (num < limMin || num > limMax || res != 1);

    return num;
}


float lerFloat (char msg[TEXTO_LONGO], float limMin, float limMax)
{
    float num;
    int res;


    do
    {
        printf("%s", msg);
        res = scanf("%f", &num);
        limparBuffer();

        if (res != 1)
        {
            printf("\n\nERRO: o valor inserido tem que ser um numero inteiro.\n");
        }
        else
        {
            if (num < limMin || num > limMax)
            {
                printf("\n\nERRO: o numero tem de pertencer ao intervalo [%.2f, %.2f].\n", limMin, limMax);
            }
        }

    }
    while (num < limMin || num > limMax || res != 1);

    return num;
}


void limparBuffer (void)
{
    char lixo;

    do
    {
        lixo = getchar();

    }
    while (lixo != '\n' && lixo != EOF);
}


