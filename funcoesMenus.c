#include <stdio.h>
#include <ctype.h>

#include "funcoesMenus.h"
#include "funcoesAuxiliares.h"


// -------------- FUNÇÕES DE MENU ----------------


int menuAlterarEquipamento (void)
{
    int opc;

    printf("\n\n\nIndique qual o campo que deseja alterar:\n");
    printf("\t1 - Data do Inventario\n");
    printf("\t2 - Descricao\n");
    printf("\t3 - Numero de serie\n");
    printf("\t4 - Custo\n");
    printf("\n\t0 - Voltar atras\n");

    opc  = lerInteiro("\n\t\tOPCAO: ", 0, 4);

    return opc;
}


int menuAlterarLaboratorio (void)
{
    int opc;

    printf("\n\n\nIndique qual o campo que deseja alterar:\n");
    printf("\t1 - Localizacao\n");
    printf("\t2 - Numero de postos de trabalho\n");
    printf("\n\t0 - Voltar atras\n");

    opc  = lerInteiro("\n\t\tOPCAO: ", 0, 2);

    return opc;
}

int subMenuAvarias(void)
{
    int opc;

    printf("\n\n\n--------------- MENU AVARIAS ----------------------------------------------\n\n");
    printf("\t1 - Registar Avaria\n");
    printf("\t2 - Eliminar Avaria\n");
    printf("\t3 - Alterar Estado da Avaria\n");

    printf("\n\t4 - Listar Avarias\n");

    printf("\n\t5 - Consultar Avarias - pelo nr. da avaria\n");
    printf("\t6 - Consultar Avarias - pelo nr. de inventario do equipamento\n");
    printf("\t7 - Consultar Avarias - por data\n");

    printf("\n\t0 - Voltar atras\n");
    opc = lerInteiro("\n\t\tOPCAO --> ", 0, 7);

    return opc;
}


int subMenuEquipamentos(void)
{
    int opc;

    printf("\n\n\n------------ MENU EQUIPAMENTOS ------------------------------------------\n\n");
    printf("\t1 - Inserir Equipamento (Inventariacao)\n");
    printf("\t2 - Instalar Equipamento\n");
    printf("\t3 - Abate do Equipamento\n");
    printf("\t4 - Alterar Equipamento\n");

    printf("\n\t5 - Listar Equipamentos\n");
    printf("\t6 - Listar Equipamentos por Laboratorio\n");
    printf("\t7 - Listar Equipamentos Avariados\n");

    printf("\n\t8 - Consultar Equipamento - por nr. de inventario\n");
    printf("\n\t0 - Voltar atras\n");
    opc = lerInteiro("\n\t\tOPCAO --> ", 0, 8);

    return opc;
}



int subMenuLaboratorios (void)
{
    int opc;

    printf("\n\n\n------------- MENU LABORATORIOS ------------------------------------------\n\n");
    printf("\t1 - Inserir laboratorio\n");
    printf("\t2 - Alterar laboratorio\n");
    printf("\t3 - Listar laboratorios\n");
    printf("\n\t4 - Consultar laboratorio - Pela designacao\n");

    printf("\n\t0 - Voltar atras\n");
    opc = lerInteiro("\n\t\tOPCAO --> ", 0, 4);

    return opc;
}

char menu (int quantLaboratorios, int quantPostosTrabalho, int quantEquipamentos, float percEquipAvariados)
{
    char opc;

    printf("\n\n\n\n-------------------------------------------------------------------------\n");
    printf("# Laboratorios: %d\t\t# Postos de Trabalho: %d\n", quantLaboratorios, quantPostosTrabalho);
    printf("# Equipamentos: %d\t\tPercentagem Equipamentos Avariados: %.2f%%\n", quantEquipamentos, percEquipAvariados);
    printf("\n\n------------ MENU PRINCIPAL ---------------------------------------------\n");
    printf("\n\tG - Gerir Laboratorios\n");
    printf("\tE - Gerir Equipamentos\n");
    printf("\tA - Gerir Avarias\n");

    printf("\n\tT - Estatisticas\n");
    printf("\n\tF - Guardar no Ficheiro\n");
    printf("\tR - Ler do Ficheiro\n");
    printf("\n\tS - Sair do Programa\n");
    do
    {

        printf("\n\t\tOPCAO --> ");
        scanf(" %c", &opc);
        opc = toupper(opc);

        if (opc != 'G' && opc != 'E' && opc != 'A' && opc != 'T' && opc != 'S' && opc != 'F' && opc != 'R')
        {
            printf("\nERRO: opcao desconhecida. Tente novamente.\n");
        }
    }
    while (opc != 'G' && opc != 'E' && opc != 'A' && opc != 'T' && opc != 'S' && opc != 'F' && opc != 'R');

    return opc;
}

