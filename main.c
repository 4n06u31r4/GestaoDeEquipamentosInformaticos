#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

#include "funcoesAuxiliares.h"
#include "funcoesMenus.h"

#define LOCAL_ARMAZEM "ARMAZEM-ABATES"
#define LOCAL_INVENTARIO "ARMAZEM-INVENTARIO"
#define LOCAL_AVARIA "EM REPARACAO"


#define MAX_LABORATORIOS 15
#define MAX_EQUIPAMENTOS 200

#define NAO_EXISTE -1    // Valor devolvido pelas funções procurar(...) quando o laboratório, equipamento ou avaria não existem no respetivo vetor

#define ESTADO_DISPONIVEL 1     // O equipamento está funcional
#define ESTADO_INDISPONIVEL -1  // o equipamento está avariado
#define ESTADO_ABATIDO 0        // O equipamento está abatido -> em Armazem-abates

#define ANO_MIN 2010
#define ANO_MAX 2500

#define POSTO_TRABALHO_MIN 0
#define POSTO_TRABALHO_MAX 30

#define CUSTO_MAX 9999999.999

#define POR_REPARAR "Por Reparar"
#define REPARADO "Reparado"
#define EQUIPAMENTO_ABATIDO "Equipamento Abatido"

typedef struct
{
    int dia, mes, ano;
} tipoData;


typedef struct
{
    char designacao[TEXTO_BREVE];   // valor único
    char localizacao [TEXTO_BREVE];
    int numPostosTrabalho;
    int quantEquipamentosInstalados;
    int quantDisponiveis; //novo campo
} tipoLaboratorio;

typedef struct
{
    int numInventario;   // valor único
    tipoData dataInventario;
    char descricao [TEXTO_LONGO];
    char numSerie [TEXTO_BREVE];
    char local [TEXTO_BREVE];
    int estado;   // (1)-ESTADO_DISPONIVEL  (-1)-ESTADO_INDISPONIVEL (0)-ESTADO_ABATIDO
    // em alternativa o estado poderia ser um char estado ou uma string   char estado[TEXTO_BREVE]
    float custo;
} tipoEquipamento;



typedef struct
{
    int numeroAvaria; //unico
    tipoData dataAvaria;
    int numInventario;
    char descricao [TEXTO_LONGO];
    char estado [TEXTO_BREVE]; //por reparar,  reparado
    float custo;
} tipoAvaria;

typedef struct
{
    float menorPercEquipDisp;
    char designacaoAux[TEXTO_BREVE];

} tipoAux;



// ----------- DECLARAÇÃO DAS FUNÇÕES -------------
tipoData lerData (void);
void escreveData (tipoData data);

int procuraLaboratorio (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios, char designacao[TEXTO_BREVE]);
void listaLaboratorios (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios);
void inserirLaboratorio (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int *quantLaboratorios);
void alterarLaboratorio (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios, tipoEquipamento vEquip[MAX_EQUIPAMENTOS], int quantEquip);
void consultaLaboratorioDesignacao (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios);
void escreveDadosLaboratorio (tipoLaboratorio laboratorio, int cabecalho);


int procuraEquipamento (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos, int numInventario);
void inserirEquipamento (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int *quantEquipamentos);
void alterarEquipamento (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos);
void listaEquipamentos (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos);
void consultaEquipamentoNrInventario (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos);
void escreveDadosEquipamento (tipoEquipamento equipamento, int cabecalho);


void guardarFicheiro (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios, tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos);
void lerFicheiro (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int *quantLaboratorios, tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int *quantEquipamentos);

int calculos (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios);

tipoAvaria * acrescentaAvaria(tipoAvaria *pAvaria, int *quantAvarias, tipoEquipamento vEquip[], int quantEquipamentos);
void mostrarDadosAvarias(tipoAvaria *pAvarias, int quantAvarias);
int procuraAvaria(tipoAvaria *pAvarias, int quantAvarias, int numeroAvariaAux);
tipoAvaria * eliminarAvaria(tipoAvaria *pAvarias, int *quantAvarias, tipoEquipamento vEquip[], int quantEquip);
//tipoAvaria * alterarEstadoAvaria(tipoAvaria *pAvarias, int quantAvarias);
void gravarFicheiroBinario_Avarias(tipoAvaria *pAvarias, int quantAvarias);
tipoAvaria * lerFicheiroBinario_avarias(tipoAvaria *pAvarias, int *quantAvarias);
void registarInstalacao(tipoLaboratorio vLab[], int numeroLab, tipoEquipamento vEquip [], int nEquip);
void registarAbate(tipoAvaria *pAvarias, int quantAvarias,tipoEquipamento vEquip [], int nEquip, tipoLaboratorio vLabs[], int qtLabs);

int comparaDesignacao(const void *v1, const void *v2) ;

float percEquipAvariados(tipoAvaria *pAvarias,int quantAvarias, int quantEquipamentos);

void listaEquipamentosporLaboratorio(tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos, tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS],int quantLaboratorios);
int comparaQuantidadeDisponivel(const void *v1, const void *v2);
void registarReparacao(tipoAvaria *pAvarias, int quantAvaria, tipoEquipamento vEquip[MAX_EQUIPAMENTOS], int quantEquip, tipoLaboratorio vLab[MAX_LABORATORIOS], int quantLab);
void listaEquipamentosAvariados(tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos, int quantAvarias);




void consultaAvariaPorNumero (tipoAvaria *pAvaria, int quantAvarias);
void escreveDadosAvaria (tipoAvaria *pAvaria, int cabecalho, int posicao);
void logEquipamentosReparados(tipoAvaria *pAvarias, int pos);
void consultaAvariaPorNumeroInv (tipoAvaria *pAvaria, int quantAvarias);
int procuraAvariaNrInv(tipoAvaria *pAvarias, int quantAvarias, int numeroInvAux);
void consultarAvariaPorData (tipoAvaria *pAvarias, int quantAvarias);





//ESTATISTICA
void custoMedioEquip (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos);
void percEquipAbatidos(tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos);
void quantEquipAvariadosDada (tipoAvaria *pAvarias, int quantAvarias, int quantEquipamentos);
void dadosReaparacoesMaiorCusto(tipoAvaria *pAvarias, int quantAvarias);
int comparaMaiorCusto(const void *v1, const void *v2);
void DesignacaolabMenorPercEquipDisp(tipoLaboratorio vLab[MAX_LABORATORIOS], tipoAux vAux[MAX_LABORATORIOS], int quantLab, int quantLabAux);
int comparaMenorPerc(const void *v1, const void *v2);

int main(void)
{
    tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS];
    tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS];
    tipoAux vetorAUX[MAX_LABORATORIOS];
    tipoAvaria *vetorAvaria = NULL;
    int quantLaboratorios=0, quantEquipamentos=0, quantPostosTrabalho=0, subOpc=0,qtAvarias = 0, qtLabAux = 0;
    char opc;
    float percEquipAvariadosAUX=0.0;


    do
    {
        quantPostosTrabalho = calculos(vetorLaboratorios, quantLaboratorios);
        percEquipAvariadosAUX = percEquipAvariados(vetorAvaria, qtAvarias, quantEquipamentos);
        opc = menu(quantLaboratorios, quantPostosTrabalho, quantEquipamentos, percEquipAvariadosAUX);
        switch (opc)
        {
        case 'G': // gerir Laboratórios
            do
            {
                subOpc = subMenuLaboratorios();
                switch (subOpc)
                {
                case 1:  // Inserir um novo laboratorio
                    inserirLaboratorio(vetorLaboratorios, &quantLaboratorios);
                    break;
                case 2:  // Alterar os campos alteráveis do laboratório
                    alterarLaboratorio (vetorLaboratorios, quantLaboratorios, vetorEquipamentos, quantEquipamentos);

                    break;
                case 3:  // Listar TODOS os laboratórios
                    listaLaboratorios(vetorLaboratorios, quantLaboratorios);
                    break;
                case 4:  // Consulta UM laboratorio pela sua designação
                    consultaLaboratorioDesignacao(vetorLaboratorios, quantLaboratorios);
                    break;
                }
            }
            while (subOpc != 0);
            break;

        case 'E':  // gerir Equipamentos
            do
            {
                subOpc = subMenuEquipamentos();
                switch (subOpc)
                {
                case 1:  // Inserir/Inventariar Equipamento
                    listaEquipamentos(vetorEquipamentos, quantEquipamentos);
                    inserirEquipamento(vetorEquipamentos, &quantEquipamentos);
                    break;
                case 2:  // Instalar o equipamento no laboratorio
                    listaEquipamentos(vetorEquipamentos, quantEquipamentos);
                    listaLaboratorios(vetorLaboratorios, quantLaboratorios);
                    registarInstalacao(vetorLaboratorios, quantLaboratorios, vetorEquipamentos, quantEquipamentos);
                    break;
                case 3:  // Abate do equipamento;
                    listaEquipamentos(vetorEquipamentos,quantEquipamentos);
                    registarAbate(vetorAvaria, qtAvarias, vetorEquipamentos, quantEquipamentos, vetorLaboratorios, quantLaboratorios);
                    break;
                case 4:   // Alterar Equipamento
                    alterarEquipamento(vetorEquipamentos, quantEquipamentos);
                    break;
                case 5:  // Listar TODOS os equipamentos
                    listaEquipamentos(vetorEquipamentos, quantEquipamentos);
                    break;
                case 6:  // Listar Equipamentos por LAboratorio
                    listaEquipamentosporLaboratorio(vetorEquipamentos,quantEquipamentos,vetorLaboratorios,quantLaboratorios);
                    break;
                case 7:  // Listar Equipamentos Avariados
                    listaEquipamentosAvariados(vetorEquipamentos,quantEquipamentos,qtAvarias);
                    break;
                case 8:  // Consultar Equipamento pelo numero de inventário
                    consultaEquipamentoNrInventario(vetorEquipamentos, quantEquipamentos);
                    break;
                }
            }
            while (subOpc != 0);
            break;

        case 'A':  // gerir Avarias
            do
            {
                subOpc = subMenuAvarias();
                switch (subOpc)
                {
                case 1: // Registar Avaria
                    vetorAvaria = acrescentaAvaria(vetorAvaria, &qtAvarias, vetorEquipamentos, quantEquipamentos);
                    break;
                case 2:  // Eliminar avaria
                    eliminarAvaria(vetorAvaria, &qtAvarias, vetorEquipamentos, quantEquipamentos);
                    break;
                case 3:   // Alterar o estado da avaria
                    registarReparacao(vetorAvaria,qtAvarias,vetorEquipamentos,quantEquipamentos,vetorLaboratorios,quantLaboratorios);
                    break;
                case 4:   // Listar avarias
                    mostrarDadosAvarias(vetorAvaria, qtAvarias);
                    break;
                case 5:  // Consultar avarias - pelo nr da avaria
                    consultaAvariaPorNumero (vetorAvaria, qtAvarias);
                    break;
                case 6:   // Consultar avarias - pelo nr. de inventário do equipamento
                    consultaAvariaPorNumeroInv (vetorAvaria, qtAvarias);
                    break;
                case 7:  // Consultar avarias - por data
                    consultarAvariaPorData (vetorAvaria, qtAvarias);
                    break;
                }
            }
            while (subOpc != 0);
            break;

        case 'T':  // ESTATISTICAS
            printf("\n ESTAT.1 - CUSTO MEDIO DE CADA EQUIPAMENTO -\n");
            custoMedioEquip(vetorEquipamentos,quantEquipamentos);
            printf("\n ESTAT.2 - PERCENTAGEM EQUIPAMENTOS ABATIDOS -\n");
            percEquipAbatidos(vetorEquipamentos,quantEquipamentos);
            printf("\n ESTAT.3 - QUANTIDADE DE EQUIPAMENTOS AVARIADOS NUMA DETERMINADA DATA -\n");
            quantEquipAvariadosDada(vetorAvaria,qtAvarias,quantEquipamentos);
            printf("\n ESTAT.4 - DADOS DAS REPARACOES COM MAIOR CUSTO -\n");
            dadosReaparacoesMaiorCusto(vetorAvaria,qtAvarias);
            printf("\n ESTAT.5 - DESIGNACAO LABORATORIO COM MENOR PERCENTAGEM DE EQUIPAMENTOS DISPONIVEIS -\n");
            DesignacaolabMenorPercEquipDisp(vetorLaboratorios,vetorAUX, quantLaboratorios, qtLabAux);
            break;
        case 'F':
            guardarFicheiro(vetorLaboratorios, quantLaboratorios, vetorEquipamentos, quantEquipamentos);
            gravarFicheiroBinario_Avarias(vetorAvaria, qtAvarias);
            break;
        case 'R':
            lerFicheiro(vetorLaboratorios, &quantLaboratorios, vetorEquipamentos, &quantEquipamentos);
            vetorAvaria = lerFicheiroBinario_avarias(vetorAvaria, &qtAvarias);
            break;
        case 'S':
            break;
        }
    }
    while (opc != 'S');
    free(vetorAvaria);
    return 0;
}


// Função para calcular o total de postos de trabalho
int calculos (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios)
{
    int i, quantPostosTrabalho;

    quantPostosTrabalho = 0;

    for (i=0; i < quantLaboratorios; i++)
    {
        (quantPostosTrabalho) += vetorLaboratorios[i].numPostosTrabalho;
    }

    return quantPostosTrabalho;
}


// -------------------------------- FUNÇÕES GERIR EQUIPAMENTOS ---------------------------------

// Função que permite inserir/inventariar um equipamento no vetor
void inserirEquipamento (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int *quantEquipamentos)
{
    int posicao;

    if (*quantEquipamentos == MAX_EQUIPAMENTOS)
    {
        printf("\n\nATENCAO: Impossivel inserir um novo Equipamento (MAXIMO atingido).\n");
    }
    else
    {
        do
        {
            printf("\n\n----------- INSERIR/INVENTARIAR EQUIPAMENTO -----------  \n");
            vetorEquipamentos[*quantEquipamentos].numInventario = lerInteiro("\nNumero de inventario: ", 1, INT_MAX);

            posicao = procuraEquipamento(vetorEquipamentos, *quantEquipamentos, vetorEquipamentos[*quantEquipamentos].numInventario);

            if (posicao == NAO_EXISTE)  // Equipamento não existe no vetor
            {
                printf("\nData de Inventariacao: ");
                vetorEquipamentos[*quantEquipamentos].dataInventario = lerData();
                lerString("\nDescricao do Equipamento: ", vetorEquipamentos[*quantEquipamentos].descricao, TEXTO_LONGO);
                lerString("\nNumero de Serie: ", vetorEquipamentos[*quantEquipamentos].numSerie, TEXTO_BREVE);
                strcpy(vetorEquipamentos[*quantEquipamentos].local, LOCAL_INVENTARIO);
                vetorEquipamentos[*quantEquipamentos].estado = ESTADO_DISPONIVEL;
                vetorEquipamentos[*quantEquipamentos].custo = lerFloat("\nCusto do Equipamento: ", 0.0, CUSTO_MAX);
                (*quantEquipamentos)++;
            }
            else
            {
                printf("\n\nATENCAO: O equipamento nr. %d ja existe. Escolha outro numero para o inventario.\n", vetorEquipamentos[*quantEquipamentos].numInventario);
            }
        }
        while (posicao != NAO_EXISTE);
    }
}


// Função que permite alterar alguns campos do equipamento
void alterarEquipamento (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos)
{
    int posicao, opc, nrInventario;

    if (quantEquipamentos == 0)
    {
        printf("\n\nATENCAO: nao existem equipamentos inseridos.\n");
    }
    else
    {
        listaEquipamentos(vetorEquipamentos, quantEquipamentos);
        printf("\n\n-------------- ALTERAR EQUIPAMENTO --------------  \n");
        nrInventario =  lerInteiro("\nNr. de Inventario do equipamento que pertende alterar (digite 0 para cancelar): ", 0, INT_MAX);

        if (nrInventario == 0)
        {
            printf("\n\nATENCAO: operacao de alteracao cancelada.\n\n");
        }
        else
        {
            posicao = procuraEquipamento(vetorEquipamentos, quantEquipamentos, nrInventario);

            if (posicao == NAO_EXISTE)  // Equipamento não existe no vetor
            {
                printf("\n\nATENCAO: O equipamento com nr. de inventario %d nao existe.\n", nrInventario);
            }
            else  // O Laboratorio foi encontrado
            {
                opc = menuAlterarEquipamento();
                switch (opc)
                {
                case 1: // alterar Data do Inventário
                    printf("\nNova data do inventario: ");
                    vetorEquipamentos[posicao].dataInventario = lerData();
                    break;

                case 2:  // alterar Descrição
                    lerString("\nNova Descricao: ", vetorEquipamentos[posicao].descricao, TEXTO_LONGO);
                    break;

                case 3:   // alterar numero de serie
                    lerString("\nNovo Nr. de Serie: ", vetorEquipamentos[posicao].numSerie, TEXTO_BREVE);
                    break;

                case 4:   // alterar o custo
                    vetorEquipamentos[posicao].custo = lerFloat("\nNovo custo: ", 0, CUSTO_MAX);
                    break;
                }
            }
        }
    }
}


//Função que procura, através do numero do inventário, se o equipamento já existe no vetor
int procuraEquipamento (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos, int numInventario)
{
    int posicao, i;

    posicao=NAO_EXISTE;

    for (i=0; i < quantEquipamentos; i++)
    {
        if (vetorEquipamentos[i].numInventario == numInventario)
        {
            posicao = i;
            i = quantEquipamentos;
        }
    }

    return posicao;
}

// Função que lista todos os equipamentos registados no vetor
void listaEquipamentos (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos)
{
    int i;

    if (quantEquipamentos == 0)
    {
        printf("\n\nATENCAO: nao existem equipamentos inseridos.\n");
    }
    else
    {
        printf("\n\n----------------------------------------- LISTAGEM DE EQUIPAMENTOS ----------------------------------------- \n");

        for (i=0; i < quantEquipamentos; i++)
        {
            escreveDadosEquipamento(vetorEquipamentos[i], i);
        }
    }
}

// Função que escreve no ecra os dados de um equipamento fornecido. O parâmetro cabeçalho permite a função escrever ou não escrever os títulos para a listagem
void escreveDadosEquipamento (tipoEquipamento equipamento, int cabecalho)
{
    if (cabecalho == 0)
    {
        printf("\nNr.Inv.\tData \t\tDescricao  \tNr. de Serie  \tLocal  \t\t\tEstado\t\t\tCusto \n");
        printf("___________________________________________________________________________________________________________\n");
    }

    printf("%6d\t", equipamento.numInventario);
    escreveData(equipamento.dataInventario);
    printf("\t%12s\t%13s\t%18s", equipamento.descricao, equipamento.numSerie, equipamento.local);
    switch (equipamento.estado)
    {
    case ESTADO_DISPONIVEL:
        printf("\tDisponivel");
        break;
    case ESTADO_INDISPONIVEL:
        printf("\tIndisponivel");
        break;
    case ESTADO_ABATIDO:
        printf("\tAbatido");
    }
    printf("\t%9.2f\n", equipamento.custo);
}


// Função que escreve no ecra os dados de um equipamento selecionado pelo utlizador através do campo nr. de inventário
void consultaEquipamentoNrInventario (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos)
{
    int posicao, numInventario;

    if (quantEquipamentos == 0)
    {
        printf("\n\nATENCAO: nao existem Equipamentos inseridos.\n");
    }
    else
    {
        printf("\n\n----------- CONSULTA EQUIPAMENTO -----------  \n");
        numInventario = lerInteiro("\nNr. do inventario: ", 1, INT_MAX);

        posicao = procuraEquipamento(vetorEquipamentos, quantEquipamentos, numInventario);

        if (posicao == NAO_EXISTE)  // Equipamento não existe no vetor
        {
            printf("\n\nATENCAO: O Equipamento com o nr %d nao existe.\n", numInventario);
        }
        else
        {
            escreveDadosEquipamento(vetorEquipamentos[posicao], 0);
        }
    }
}


// -------------------------------- FUNÇÕES GERIR LABORATORIO ---------------------------------

// Função que permite inserir/registar um novo laboratório
void inserirLaboratorio (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int *quantLaboratorios)
{
    int posicao;


    if (*quantLaboratorios == MAX_LABORATORIOS)
    {
        printf("\n\nATENCAO: Impossivel inserir um novo laboratorio (MAXIMO atingido).\n");
    }
    else
    {
        printf("\n\n-------------- INSERIR LABORATORIO --------------  \n");
        lerString("\nDesignacao: ", vetorLaboratorios[*quantLaboratorios].designacao, TEXTO_BREVE);

        posicao = procuraLaboratorio(vetorLaboratorios, *quantLaboratorios, vetorLaboratorios[*quantLaboratorios].designacao);
        if (posicao == NAO_EXISTE)  // Laboratório não existe no vetor
        {
            lerString("Localizacao: ", vetorLaboratorios[*quantLaboratorios].localizacao, TEXTO_BREVE);
            vetorLaboratorios[*quantLaboratorios].numPostosTrabalho = lerInteiro("Nr. Postos de trabalho: ", POSTO_TRABALHO_MIN, POSTO_TRABALHO_MAX);
            vetorLaboratorios[*quantLaboratorios].quantEquipamentosInstalados = 0;
            vetorLaboratorios[*quantLaboratorios].quantDisponiveis = 0;
            (*quantLaboratorios)++;
        }
        else
        {
            escreveDadosLaboratorio(vetorLaboratorios[posicao], 0);
            printf("\n\nATENCAO: O laboratorio %s ja existe.\n", vetorLaboratorios[*quantLaboratorios].designacao);
        }
    }
}

// função que procura um laboratório tendo em conta a sua designação
int procuraLaboratorio (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios, char designacao[TEXTO_BREVE])
{
    int posicao, i;

    posicao=NAO_EXISTE;

    for (i=0; i < quantLaboratorios; i++)
    {
        if (strcmp(vetorLaboratorios[i].designacao, designacao) == 0)
        {
            posicao = i;
            i = quantLaboratorios;
        }
    }

    return posicao;
}


// Função que permite alterar os campos do laboratório
void alterarLaboratorio (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios, tipoEquipamento vEquip[MAX_EQUIPAMENTOS], int quantEquip)
{
    int posicao, opc, i;
    char designacao[TEXTO_BREVE], novoTexto[TEXTO_BREVE];

    if (quantLaboratorios == 0)
    {
        printf("\n\nATENCAO: nao existem laboratorios inseridos.\n");
    }
    else
    {
        listaLaboratorios(vetorLaboratorios, quantLaboratorios);
        printf("\n\n-------------- ALTERAR LABORATORIO --------------  \n");
        lerString("\nDesignacao do laboratorio que pertende alterar (digite 0 para cancelar): ", designacao, TEXTO_BREVE);
        if (strcmp(designacao, "0")==0)  // O utilizador cancelou a operação de alteraçãoA
        {
            printf("\n\nATENCAO: operacao cancelada\n\n");
        }
        else
        {
            posicao = procuraLaboratorio(vetorLaboratorios, quantLaboratorios, designacao);

            if (posicao == NAO_EXISTE)  // Laboratório não existe no vetor
            {
                printf("\n\nATENCAO: O laboratorio %s nao existe.\n", designacao);
            }
            else  // O Laboratorio foi encontrado
            {
                for(i=0; i<quantEquip; i++)
                {
                    if(strcmp(vetorLaboratorios[posicao].designacao,vEquip[i].local)==0)
                    {
                        opc = menuAlterarLaboratorio();
                        switch (opc)
                        {

                        case 1:  // alterar Localização
                            lerString("\nNova Localizacao: ", novoTexto, TEXTO_BREVE);
                            strcpy(vetorLaboratorios[posicao].localizacao, novoTexto);
                            strcpy(vEquip[i].local, novoTexto);
                            break;
                        case 2:   // alterar numero de postos de trabalho
                            vetorLaboratorios[posicao].numPostosTrabalho = lerInteiro("\nNova quantidade de postos de trabalho: ", POSTO_TRABALHO_MIN, POSTO_TRABALHO_MAX);
                            break;
                        }
                    }
                    else
                    {
                        opc = menuAlterarLaboratorio();
                        switch (opc)
                        {

                        case 1:  // alterar Localização
                            lerString("\nNova Localizacao: ", novoTexto, TEXTO_BREVE);
                            strcpy(vetorLaboratorios[posicao].localizacao, novoTexto);
                            break;
                        case 2:   // alterar numero de postos de trabalho
                            vetorLaboratorios[posicao].numPostosTrabalho = lerInteiro("\nNova quantidade de postos de trabalho: ", POSTO_TRABALHO_MIN, POSTO_TRABALHO_MAX);
                            break;
                        }
                    }


                }
            }
        }
    }
}



// Função que escreve no ecra os dados do laboratório passado por parâmetro. O parâmetro cabeçalho é uma flag que indica se os títulos da listagem são ou não escritos no ecra
void escreveDadosLaboratorio (tipoLaboratorio laboratorio, int cabecalho)
{
    if (cabecalho == 0)
    {
        printf("\n  Designacao \t    Localizacao \tPostos Trabalho\t  Equip.Instalados\n");
        printf("____________________________________________________________________________\n");
    }

    printf("\n%10s\t%15s\t%15d\t %15d", laboratorio.designacao, laboratorio.localizacao, laboratorio.numPostosTrabalho, laboratorio.quantEquipamentosInstalados);
}


// Função que lista no ecra todos os laboratórios registados
void listaLaboratorios (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios)
{
    int i;

    if (quantLaboratorios == 0)
    {
        printf("\n\nATENCAO: nao existem laboratorios inseridos.\n");
    }
    else
    {
        printf("\n\n------------------------- LISTAGEM DE LABORATORIOS -------------------------  \n");

        for (i=0; i < quantLaboratorios; i++)
        {
            escreveDadosLaboratorio(vetorLaboratorios[i], i);
        }
    }
}


// Função que escreve no ecra os dados de um laboratório selecionado pelo utlizador através do campo designacao
void consultaLaboratorioDesignacao (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios)
{
    int posicao;
    char designacao[TEXTO_BREVE];

    if (quantLaboratorios == 0)
    {
        printf("\n\nATENCAO: nao existem laboratorios inseridos.\n");
    }
    else
    {
        listaLaboratorios(vetorLaboratorios, quantLaboratorios);
        printf("\n\n----------- CONSULTA LABORATORIO -----------  \n");
        lerString("\nDesignacao do laboratorio: ", designacao, TEXTO_BREVE);

        posicao = procuraLaboratorio(vetorLaboratorios, quantLaboratorios, designacao);

        if (posicao == NAO_EXISTE)  // Laboratório não existe no vetor
        {
            printf("\n\nATENCAO: O laboratorio %s nao existe.\n", designacao);
        }
        else
        {
            escreveDadosLaboratorio(vetorLaboratorios[posicao], 0);
        }
    }
}



// -------------- FUNÇÕES LER e ESCREVER FICHEIRO ----------------

// Função que guarda os dados dos vetores dos laboratórios e dos equipamentos num ficheiro
void guardarFicheiro (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int quantLaboratorios, tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos)
{
    FILE *ficheiro;
    int elementosGuardados;

    ficheiro = fopen("dadoBin.dat", "wb");

    if (ficheiro == NULL)
    {
        printf("\nERRO: falha na abertura do ficheiro.\n");
    }
    else
    {
        elementosGuardados = fwrite(&quantLaboratorios, sizeof(int), 1, ficheiro);
        if (elementosGuardados != 1)
        {
            printf("\nERRO: falha na gravacao do numero de laboratorios inseridos.\n");
        }
        else
        {
            elementosGuardados = fwrite(vetorLaboratorios, sizeof (tipoLaboratorio), quantLaboratorios, ficheiro);
            if (elementosGuardados != quantLaboratorios)
            {
                printf("\nERRO: falha na gravacao dos dados dos laboratorios inseridos.\n");
            }
            else
            {
                elementosGuardados = fwrite(&quantEquipamentos, sizeof(int), 1, ficheiro);
                if (elementosGuardados != 1)
                {
                    printf("\nERRO: falha na gravacao do numero de Equipamentos inseridos.\n");
                }
                else
                {
                    elementosGuardados = fwrite(vetorEquipamentos, sizeof(tipoEquipamento), quantEquipamentos, ficheiro);
                    if (elementosGuardados != quantEquipamentos)
                    {
                        printf("\nERRO: falha na gravacao dos dados dos equipamentos inseridos.\n");
                    }
                }
            }
        }
        printf("\n\tFicheiro guardado com sucesso!");
    }
    fclose(ficheiro);
}

// Função que lê do ficheiro os dados dos laboratórios e dos equipamentos e os guarda nos repetivos vetores
void lerFicheiro (tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS], int *quantLaboratorios, tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int *quantEquipamentos)
{
    FILE *ficheiro;
    int elementosGuardados;

    *quantEquipamentos = 0;
    *quantLaboratorios = 0;

    ficheiro = fopen("dadoBin.dat", "rb");

    if (ficheiro == NULL)
    {
        printf("\nERRO: falha na abertura do ficheiro.\n");
    }
    else
    {
        elementosGuardados = fread(quantLaboratorios, sizeof(int), 1, ficheiro);
        if (elementosGuardados != 1)
        {
            printf("\nERRO: falha na leitura do numero de laboratorios inseridos.\n");
        }
        else
        {
            elementosGuardados = fread(vetorLaboratorios, sizeof (tipoLaboratorio), *quantLaboratorios, ficheiro);
            if (elementosGuardados != *quantLaboratorios)
            {
                printf("\nERRO: falha na leitura dos dados dos laboratorios inseridos.\n");
                *quantLaboratorios = 0;
            }
            else
            {
                elementosGuardados = fread(quantEquipamentos, sizeof(int), 1, ficheiro);
                if (elementosGuardados != 1)
                {
                    printf("\nERRO: falha na leitura do numero de Equipamentos inseridos.\n");
                }
                else
                {
                    elementosGuardados = fread(vetorEquipamentos, sizeof(tipoEquipamento), *quantEquipamentos, ficheiro);
                    if (elementosGuardados != *quantEquipamentos)
                    {
                        printf("\nERRO: falha na leitura dos dados dos equipamentos inseridos.\n");
                        *quantEquipamentos = 0;
                    }
                }
            }
        }
        printf("\n\tFicheiro lido com sucesso!");
    }
    fclose(ficheiro);
}


// -------------- FUNÇÕES GESTAO da DATA  ----------------

void escreveData (tipoData data)
{
    printf("%02d-%02d-%4d", data.dia, data.mes, data.ano);
}

tipoData lerData (void)
{
    tipoData data;
    int diaMax;

    data.ano = lerInteiro("\nAno: ", ANO_MIN, ANO_MAX);
    data.mes = lerInteiro("Mes: ", 1, 12);

    switch (data.mes)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        diaMax = 31;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        diaMax = 30;
        break;
    case 2:
        if (data.ano % 400 == 0 || (data.ano % 4 == 0 && data.ano % 100 != 0))
        {

            diaMax = 29;
        }
        else
        {
            diaMax = 28;
        }
    }

    data.dia = lerInteiro("Dia: ", 1, diaMax);

    return data;
}

// 2 PARTE

//ACRESCENTA AVARIA
tipoAvaria * acrescentaAvaria(tipoAvaria *pAvaria, int *quantAvarias, tipoEquipamento vEquip[MAX_EQUIPAMENTOS], int quantEquip)
{
    tipoAvaria *pAux;
    tipoData data;
    int i, pos, numeroEquipamento,controlo;
    char descricaoAUX[TEXTO_LONGO];
    pAux = pAvaria;
    tipoData dataA, dataB,diferenca;

    if(quantEquip == 0)
    {
        printf("\n\tAinda nao existem equipamentos!\n");
    }
    else
    {
        pAux = realloc(pAux,(*quantAvarias+1)*sizeof(tipoAvaria));
        if(pAux == NULL)
        {
            printf("\n\tImpossivel reserva memoria!\n");
        }
        else
        {
            if(*quantAvarias == 0)
            {
                pAux[*quantAvarias].numeroAvaria = 1;
            }
            else
            {
                pAux[*quantAvarias].numeroAvaria = pAux[*quantAvarias-1].numeroAvaria+1; //atribui numero
            }
            //pAux[*quantAvarias].numInventario = lerInteiro("\n Numero de equipamentos para reparar?", 0, quantEquip);
            for (i=0; i < quantEquip; i++)
            {
                escreveDadosEquipamento(vEquip[i], i);
            }

            do
            {
                numeroEquipamento = lerInteiro("\n Numero de inventario do equipamento?", 1, 200);
                pos = procuraEquipamento(vEquip, quantEquip, numeroEquipamento);
                if(pos == -1)
                {
                    printf("\n\tEsse numero de equipamento nao existe");
                }
                else
                {
                    do
                    {
                        pAux[*quantAvarias].numInventario = vEquip[pos].numInventario;
                        controlo = 0;
                        printf(" Data da avaria:");
                        data = lerData();
                        pAux[*quantAvarias].dataAvaria = data;
                        dataA = data;
                        dataB = vEquip[pos].dataInventario;


                        if(dataA.dia>=dataB.dia)
                            diferenca.dia = dataA.dia-dataB.dia;
                        else
                        {
                            dataA.dia+=30;
                            dataA.mes-=1;
                            diferenca.dia = dataA.dia-dataB.dia;
                        }
                        if(dataA.mes>=dataB.mes)
                            diferenca.mes = dataA.mes-dataB.mes;
                        else
                        {
                            dataA.mes+=12;
                            dataA.ano-=1;
                            diferenca.mes = dataA.dia-dataB.mes;
                        }
                        diferenca.ano = dataA.ano-dataB.ano;


                        if(diferenca.ano < 0)
                        {
                            printf("\n\t Data invalida! \n");
                            controlo = 1;
                        }
                    }
                    while(controlo != 0);
                    pAux[*quantAvarias].dataAvaria = data;
                    lerString("\n Descricao da avaria: ", descricaoAUX, TEXTO_LONGO);
                    strcpy(pAux[*quantAvarias].descricao, descricaoAUX);
                    strcpy(pAux[*quantAvarias].estado, POR_REPARAR);
                    pAux[*quantAvarias].custo = 0.0;
                    vEquip[pos].estado = ESTADO_INDISPONIVEL;
            }
        }
        while(pos == -1);
    }
    (*quantAvarias) ++;
}
return pAux;
}

//MOSTRAR DADOS AVARIAS
void mostrarDadosAvarias(tipoAvaria *pAvarias, int quantAvarias)
{
    int i;

    if(quantAvarias == 0)
    {
        printf("\n\tAinda nao existem avarias inseridas!");
    }
    else
    {
        printf("\nDADOS DAS AVARIAS: \n");
        for(i = 0; i < quantAvarias; i ++)
        {
            printf("\n\n Numero da avaria: %d", pAvarias[i].numeroAvaria);
            printf("\n Numero de inventario: %d",pAvarias[i].numInventario);
            printf("\n Data: %.2d - %.2d - %.4d", pAvarias[i].dataAvaria.dia, pAvarias[i].dataAvaria.mes, pAvarias[i].dataAvaria.ano);
            printf("\n Descricao: %s", pAvarias[i].descricao);
            printf("\n Estado: %s", pAvarias[i].estado);
            printf("\n Custo da reparacao: %.2f", pAvarias[i].custo);
        }
    }
}

//PROCURA AVARIA
int procuraAvaria(tipoAvaria *pAvarias, int quantAvarias, int numeroAvariaAux)
{
    int i, posicao;
    posicao = -1;
    //numeroAvariaAux--;
    for (i=0; i<quantAvarias; i++)
    {
        if (pAvarias[i].numeroAvaria == numeroAvariaAux) // Elemento encontrado
        {
            posicao=i;         // Armazena posicao no vetor
            i=quantAvarias; // para ao continuar no for..
        }
    }
    return posicao;
}

//ELIMINA AVARIA INCORRETAMENTE COLOCADA
tipoAvaria * eliminarAvaria(tipoAvaria *pAvarias, int *quantAvarias, tipoEquipamento vEquip[], int quantEquip)
{
    int numeroAvariaEliminar, pos, i, j;
    tipoAvaria * pAux;
    pAux = pAvarias;

    if (*quantAvarias != 0)
    {
        if(quantEquip==0)
        {
            printf("\n\tNao foram inseridos equipamentos!\n");
        }
        else
        {
            numeroAvariaEliminar = lerInteiro("\n Numero de avaria a eliminar: ", 1, 100);
            pos = procuraAvaria(pAvarias, *quantAvarias, numeroAvariaEliminar);
            if(pos == -1)
            {
                printf("\n\tNumero da avaria nao existe!");
            }
            else
            {
                for(j=0; j < quantEquip; j++)
                {
                    if(pAvarias[pos].numInventario == vEquip[j].numInventario)
                    {
                        //strcpy(vEquip[j].local, " ARMAZEM-INVENTARIO");
                        vEquip[j].estado = ESTADO_DISPONIVEL;
                    }
                }

                for(i = pos; i < *quantAvarias-1; i ++)
                {
                    pAvarias[i] = pAvarias[i+1]; //pAux[i] = pAux[i-1];
                }
                pAux = realloc(pAux, (*quantAvarias-1)*sizeof(tipoAvaria));
                if(pAux == NULL && (*quantAvarias-1) != 0)
                {
                    printf("\n\tERRO no realloc ao tentar reservar a memoria alocada!");
                    pAvarias = pAux;
                }
                (*quantAvarias) --;
            }
        }
    }
    else
    {
        printf("\n\tNao existem avarias para eliminar!\n");
    }
    return pAvarias;
}

//REGISTA INSTALAÇÃO
void registarInstalacao(tipoLaboratorio vLab[], int numeroLab, tipoEquipamento vEquip [], int nEquip)
{
    int posicaoLAB,posicaoEQUIP,i;
    char designacaoAUX [TEXTO_LONGO];
    int numInventarioAUX;

    if(nEquip == 0)
    {
        printf("\n\tNao foram inseridos equipamentos!\n");
    }
    else
    {
        if(numeroLab == 0)
        {
            printf("\n\tNao foram inseridos laboratorios!\n");
        }
        else
        {
            numInventarioAUX = lerInteiro("\n Introduza o numero de inventario do equipamento a registar: ", 0, 200);
            posicaoEQUIP = procuraEquipamento(vEquip, nEquip, numInventarioAUX);
            if(posicaoEQUIP == -1)
            {
                printf("\n\t\t Equipamento não encontrado! \n");
            }
            else
            {
                lerString("\n Designacao do laboratorio onde pretende registar o equipamento: ", designacaoAUX, TEXTO_BREVE);
                posicaoLAB = procuraLaboratorio(vLab, numeroLab, designacaoAUX);
                if(posicaoLAB == -1)
                {
                    printf("\n\t\t Laboratorio com a designacao '%s' ainda nao foi registado! \n", designacaoAUX);
                }
                else
                {
                    if(strcmp(vEquip[posicaoEQUIP].local, vLab[posicaoLAB].designacao) == 0)        //-------------------------------------------------------------------------
                    {
                        printf("\n\nEquipamento ja registado neste laboratorio!");
                    }
                    else
                    {
                        if(strcmp(vEquip[posicaoEQUIP].local, LOCAL_INVENTARIO)==0)
                        {
                            strcpy(vEquip[posicaoEQUIP].local, vLab[posicaoLAB].designacao);
                            vLab[posicaoLAB].quantEquipamentosInstalados ++;
                            vLab[posicaoLAB].quantDisponiveis++;
                            printf("\n\nEquipamento registado com sucesso!");
                        }
                        else
                        {
                            for(i=0; i<numeroLab; i++)
                            {
                                if(strcmp(vEquip[posicaoEQUIP].local,vLab[i].designacao)==0)
                                {
                                    strcpy(vEquip[posicaoEQUIP].local, vLab[posicaoLAB].designacao);
                                    vLab[i].quantEquipamentosInstalados --;
                                    vLab[i].quantDisponiveis --;
                                    vLab[posicaoLAB].quantEquipamentosInstalados ++;
                                    vLab[posicaoLAB].quantDisponiveis++;
                                    i=numeroLab;
                                }
                            }
                            printf("\n\nEquipamento registado com sucesso!");
                        }

                    }
                }
            }
        }
    }
}

//REGISTA ABATE
void registarAbate(tipoAvaria *pAvarias, int quantAvarias,tipoEquipamento vEquip [], int nEquip, tipoLaboratorio vLabs[], int qtLabs)
{
    int numeroDoEquip, pos, i, j, x,k;

    if(nEquip==0)
    {
        printf("\n\tNao foram inseridos equipamentos!\n");
    }
    else
    {
        numeroDoEquip = lerInteiro("Insira o numero do equipamento que pretende abater: ", 0, 200);
        pos = procuraEquipamento(vEquip, nEquip, numeroDoEquip);
        if(pos == -1)
        {
            printf("\n\tEsse numero de equipamento nao existe!");
        }
        else
        {
            if(vEquip[pos].estado==0)
            {
                printf("\n\tEquipamento ja se encontra abatido!");
            }
            else
            {
                for(i=0; i< quantAvarias; i++)
                {
                    if(vEquip[pos].numInventario == pAvarias[i].numInventario)
                    {
                        if(vEquip[pos].numInventario == pAvarias[i].numInventario && strcmp(pAvarias[i].estado, POR_REPARAR) == 0)
                        {
                            strcpy(pAvarias[i].estado, EQUIPAMENTO_ABATIDO);
                            for(j=0; j<qtLabs; j++)
                            {
                                if(strcmp(vEquip[pos].local, vLabs[j].designacao) == 0)
                                {
                                    vLabs[j].quantEquipamentosInstalados--;
                                    j=qtLabs;
                                }
                            }
                        }
                        else
                        {
                            for(x=0; x<qtLabs; x++)
                            {
                                if(strcmp(pAvarias[i].estado, REPARADO) == 0)
                                {
                                    vLabs[x].quantEquipamentosInstalados--;
                                    vLabs[x].quantDisponiveis--;
                                    x=qtLabs;
                                    i=quantAvarias;
                                }
                            }
                        }
                    }
                }

                for(k=0; k<qtLabs; k++)
                {
                    if(strcmp(vEquip[pos].local, vLabs[k].designacao) == 0 && vEquip[pos].estado == 1)
                    {
                        vLabs[k].quantEquipamentosInstalados--;
                        vLabs[k].quantDisponiveis--;
                        k=qtLabs;
                    }
                }
                vEquip[pos].estado = ESTADO_ABATIDO;
                strcpy(vEquip[pos].local, "ARMAZEM-ABATE");
            }
        }
    }
}


//REGISTA REPARAÇÃO
void registarReparacao(tipoAvaria *pAvarias, int quantAvaria, tipoEquipamento vEquip[MAX_EQUIPAMENTOS], int quantEquip, tipoLaboratorio vLab[MAX_LABORATORIOS], int quantLab)
{
    int pos, numeroAvaria, i, j;

    if(quantAvaria == 0)
    {
        printf("\n\tNao existem avarias registadas! ");
    }
    else
    {
        if(quantEquip==0)
        {
            printf("\n\tNao foram introduzidos equipamentos!\n");
        }
        else
        {
            mostrarDadosAvarias(pAvarias, quantAvaria);
            numeroAvaria = lerInteiro("\n Insira o numero de avaria a registar: ", 1, quantAvaria);
            pos = procuraAvaria(pAvarias, quantAvaria, numeroAvaria);
            if(pos == -1)
            {
                printf("\n\t Avaria nao encontrada! ");
            }
            else
            {
                if(strcmp(pAvarias[pos].estado, REPARADO)==0)
                {
                    printf("\n\t Equipamento ja se encontra reparado! ");
                }
                else
                {

                    strcpy(pAvarias[pos].estado, REPARADO);
                    for(i=0; i<quantEquip; i++)
                    {
                        if(pAvarias[pos].numInventario == vEquip[i].numInventario)
                        {
                            vEquip[i].estado = ESTADO_DISPONIVEL;
                            for(j=0; j< quantLab; j++)
                            {
                                if(strcmp(vEquip[i].local, vLab[j].designacao)==0)
                                {
                                    vLab[j].quantDisponiveis++;

                                }
                            }
                            pAvarias[pos].custo = lerFloat(" Insira o custo da reparacao: ", 0.0, 9999999.0);
                            logEquipamentosReparados(pAvarias, pos);
                        }
                    }
                }
            }
        }
    }
}


//GRAVA EM FICHEIRO BINARIO AS AVARIAS
void gravarFicheiroBinario_Avarias(tipoAvaria *pAvarias, int quantAvarias)
{
    FILE *f;
    int quantEscritos;

    f = fopen("avarias.dat", "wb");

    if(f == NULL)
    {
        printf("\n Impossivel criar ficheiro!");
    }
    else
    {
        quantEscritos = fwrite(&quantAvarias, sizeof(int), 1, f);
        if(quantEscritos != 1)
        {
            printf("\n ERRO ao escrever a quantidade de avarias!");
        }
        quantEscritos = fwrite(pAvarias, sizeof(tipoAvaria), quantAvarias, f);
        if(quantEscritos != quantAvarias)
        {
            printf("\n ERRO ao escrever dados das avarias no ficheiro!");
        }
        fclose(f);
    }
}

//LE DO FICHEIRO AS AVARIAS
tipoAvaria * lerFicheiroBinario_avarias(tipoAvaria *pAvarias, int *quantAvarias)
{
    FILE *f;
    int quantEscritos, erro = 0;

    f = fopen("avarias.dat", "rb");

    if(f == NULL)
    {
        printf("\n Impossivel abrir ficheiro!");
        erro = 1;
    }
    else
    {
        quantEscritos = fread(&(*quantAvarias), sizeof(int), 1, f);
        if(quantEscritos != 1)
        {
            printf("\n ERRO ao ler a quantidade de avarias!");
            erro = 1;
        }
        pAvarias = realloc(pAvarias, (*quantAvarias)*sizeof(tipoAvaria));
        if(pAvarias == NULL && *quantAvarias != 0)
        {
            printf("\n ERRO ao reservar memoria!");
            erro = 1;
        }
        else
        {
            quantEscritos = fread(pAvarias, sizeof(tipoAvaria), *quantAvarias, f);
            if(quantEscritos != *quantAvarias)
            {
                printf("\n ERRO ao ler dados das avarias no ficheiro!");
                erro=1;
            }
        }
        fclose(f);
    }
    if(erro == 1)
    {
        *quantAvarias = 0;
        free(pAvarias);
        pAvarias = NULL;
    }

    return pAvarias;
}

//LISTA EQUIPAMENTOS DISPONIVEIS POR LABORATORIO POR ORDEM CRESCENTE DO NUMERO DE EQUIPAMENTOS DISPONIVEIS
void listaEquipamentosporLaboratorio(tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos, tipoLaboratorio vetorLaboratorios[MAX_LABORATORIOS],int quantLaboratorios)
{
    int i,j;
    if (quantEquipamentos == 0)
    {
        printf("\n\nATENCAO: nao existem equipamentos inseridos.\n");
    }
    else
    {
        if (quantLaboratorios == 0)
        {
            printf("\n\nATENCAO: nao existem laboratorios inseridos.\n");
        }
        else
        {
            qsort(vetorLaboratorios, quantLaboratorios, sizeof(tipoLaboratorio), comparaQuantidadeDisponivel);
            for(i=0; i<quantLaboratorios; i++)
            {
                for(j=0; j<quantEquipamentos; j++)
                {
                    if(vetorEquipamentos[j].estado==1 && strcmp(vetorEquipamentos[j].local,vetorLaboratorios[i].designacao)==0)
                    {
                        escreveDadosEquipamento(vetorEquipamentos[j],j);
                    }
                    else
                    {
                        printf("\n\tNao existem equipamentos disponiveis!");
                    }
                }
            }

        }
    }
}

//FUNÇAO PARA ORDENAR vetorLaboratorios POR Nº EQUIPAMENTOS DISPONIVEIS
int comparaQuantidadeDisponivel(const void *v1, const void *v2)
{
    tipoLaboratorio *valor1, *valor2;
    int comp;
    valor1 = (tipoLaboratorio *) v1;
    valor2 = (tipoLaboratorio *) v2;
    comp = valor2->quantDisponiveis - valor1->quantDisponiveis; /* Compara valores */
    return comp;
}

//CUSTO MEDIO DE CADA EQUIPAMENTOS --1 ESTATISTICA--
void custoMedioEquip (tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos)
{
    int i;
    float soma=0.0,media=0.0;
    if (quantEquipamentos == 0)
    {
        printf("\n\tNao foram inseridos equipamentos!\n");
    }
    else
    {
        for(i=0; i<quantEquipamentos; i++)
        {
            soma += vetorEquipamentos[i].custo;
        }
        media = soma/quantEquipamentos;
        printf("\n\t CUSTO MEDIO DE CADA EQUIPAMENTO: %0.2f\n",media);
    }
}

//PERCENTAGEM EQUIPAMENTOS ABATIDOS --2 ESTATISTICA--
void percEquipAbatidos(tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos)
{
    int i, numeroabatidos=0;
    float perc=0.0;
    if(quantEquipamentos==0)
    {
        printf("\n\tNao foram inseridos equipamentos!\n");
    }
    else
    {
        for(i=0; i<quantEquipamentos; i++)
        {
            if(vetorEquipamentos[i].estado == ESTADO_ABATIDO)
            {
                numeroabatidos++;
            }
        }
        perc = ((float)numeroabatidos*100)/quantEquipamentos;
        printf("\n\t PERCENTAGEM EQUIPAMENTOS ABATIDOS: %0.2f%%\n",perc);
    }
}

// QUANTIDADE EQUIPAMENTOS AVARIADOS NUMA DETERMINADA DATA --3 ESTATISTICA--
void quantEquipAvariadosDada (tipoAvaria *pAvarias, int quantAvarias, int quantEquipamentos)
{
    tipoData data;
    int quantAvariaData =0, i;
    if(quantEquipamentos==0)
    {
        printf("\n\t Nao foram inseridos equipamentos! \n");
    }
    else
    {
        if(quantAvarias == 0)
        {
            printf("\n\t Nao foram inseridas avarias! \n");
        }
        else
        {
            printf("\n\tInsira a data que pretende procurar:");
            data = lerData();
            for (i=0; i<quantAvarias; i++)
            {
                if(pAvarias[i].dataAvaria.ano == data.ano && pAvarias[i].dataAvaria.mes == data.mes && pAvarias[i].dataAvaria.dia == data.dia)
                {
                    quantAvariaData ++;
                }
            }
            if(quantAvariaData != 0)
            {
                printf("\n\t Numero de equipamentos avariados em %d/%d/%d: %d \n",data.dia,data.mes,data.ano,quantAvariaData);
            }
            else
            {
                printf("\n\t Nao existem avarias registadas nessa data! \n");
            }
        }

    }

}

//DADOS REPARAÇOES COM MAIOR CUSTO --4 ESTATISTICA--
void dadosReaparacoesMaiorCusto(tipoAvaria *pAvarias, int quantAvarias)
{
    int i;
    if (quantAvarias==0)
    {
        printf("\n\tAinda nao foram introduzidas avarias!\n");
    }
    else
    {
        qsort(pAvarias, quantAvarias, sizeof(tipoAvaria), comparaMaiorCusto);
        for(i=0; i<quantAvarias; i++)
        {
            if(pAvarias[0].custo == pAvarias[i].custo && strcmp(pAvarias[0].estado,REPARADO)==0)
            {
                printf("\t\n Numero da avaria: %d", pAvarias[i].numeroAvaria);
                printf("\t\n Data: %.2d - %.2d - %.4d", pAvarias[i].dataAvaria.dia, pAvarias[i].dataAvaria.mes, pAvarias[i].dataAvaria.ano);
                printf("\t\n Descricao: %s", pAvarias[i].descricao);
                printf("\t\n Estado: %s", pAvarias[i].estado);
                printf("\t\n Custo da reparacao: %.2f\n", pAvarias[i].custo);
            }
        }
    }

}


//FUNÇAO PARA ORDENAR pAvarias PELA QUANTIDADE DE AVARIAS --4 ESTATISTICA--
int comparaMaiorCusto(const void *v1, const void *v2)
{
    tipoAvaria *valor1, *valor2;
    float comp;
    valor1 = (tipoAvaria *) v1;
    valor2 = (tipoAvaria *) v2;
    comp = valor1->custo - valor2->custo; /* Compara valores */
    return comp;
}

//DESIGNACAO LAB COM MENOR PERCENTAGEM DE EQUIPAMENTOS DISPONIVEIS --5 ESTATISTICA--
void DesignacaolabMenorPercEquipDisp(tipoLaboratorio vLab[MAX_LABORATORIOS], tipoAux vAux[MAX_LABORATORIOS], int quantLab, int quantLabAux)
{
    int i,quantidadeDisponiveis=0,quantidadeInstalados=0;
    if(quantLab==0)
    {
        printf("\n\tNao foram inseridos laboratorios!\n");
    }
    else
    {
        quantLabAux = quantLab;
        for(i=0; i<quantLabAux; i++)
        {
            if(vLab[i].quantEquipamentosInstalados!=0)
            {
                quantidadeDisponiveis = vLab[i].quantDisponiveis;
                quantidadeInstalados = vLab[i].quantEquipamentosInstalados;
                vAux[i].menorPercEquipDisp = (float)(quantidadeDisponiveis * 100.0 / quantidadeInstalados);
                strcpy(vAux[i].designacaoAux, vLab[i].designacao);
            }


        }
        qsort(vAux, quantLabAux, sizeof(tipoAux), comparaMenorPerc);
        for(i=1; i<quantLabAux; i++)
        {
            if(vAux[0].menorPercEquipDisp == vAux[i].menorPercEquipDisp)
            {
                printf("\n\tDesignacao: %s",vAux[i].designacaoAux);
            }
            else
            {
                printf("\n\tDesignacao: %s",vAux[0].designacaoAux);
                i=quantLabAux;
            }

        }
    }
}
//COMPARA TOTAL DE EQUIPAMENTOS --5 ESTATISTICA--
int comparaMenorPerc(const void *v1, const void *v2)
{
    tipoAux *valor1, *valor2;
    int comp;
    valor1 = (tipoAux *) v1;
    valor2 = (tipoAux *) v2;
    comp = valor1->menorPercEquipDisp - valor2->menorPercEquipDisp;
    return comp;
}

//PERCENTAGEM EQUIPAMENTOS AVARIADOS --MENU PRINCIPAL--
float percEquipAvariados(tipoAvaria *pAvarias,int quantAvarias, int quantEquipamentos)
{
    int i, numeroavariados=0;
    float perc=0.0;
    if(quantEquipamentos!=0)
    {
        if(quantAvarias != 0)
        {
            for(i=0; i<quantEquipamentos; i++)
            {
                if(strcmp(pAvarias[i].estado, POR_REPARAR)==0)
                {
                    numeroavariados++;
                }
            }
            perc = ((float)numeroavariados*100)/quantEquipamentos;
        }
    }
    return perc;
}




void consultaAvariaPorNumero (tipoAvaria *pAvaria, int quantAvarias)
{
    int posicao, numAvaria;

    if (quantAvarias == 0)
    {
        printf("\n\nATENCAO: nao existem Avarias registadas.\n");
    }
    else
    {
        printf("\n\n----------- CONSULTA AVARIA -----------  \n");
        numAvaria = lerInteiro("\nNr. da avaria: ", 1, INT_MAX);

        posicao = procuraAvaria(pAvaria, quantAvarias, numAvaria);

        if (posicao == NAO_EXISTE)  // Equipamento não existe no vetor
        {
            printf("\n\nATENCAO: A Avaria com o nr %d nao existe.\n", numAvaria);
        }
        else
        {
            escreveDadosAvaria(pAvaria, 0, posicao);
        }
    }
}

void escreveDadosAvaria (tipoAvaria *pAvaria, int cabecalho, int posicao)
{
    if (cabecalho == 0)
    {
        printf("\n  Numero Avaria \t    Data    \t Descricao \t Estado \t Custo\n");
        printf("____________________________________________________________________________\n");
    }

    printf("\n%d\t\t%2d-%2d-%4d \t%15s\t%15s %15f", pAvaria[posicao].numeroAvaria, pAvaria[posicao].dataAvaria.dia, pAvaria[posicao].dataAvaria.mes, pAvaria[posicao].dataAvaria.ano, pAvaria[posicao].descricao, pAvaria[posicao].estado, pAvaria[posicao].custo);
}


int procuraAvariaNrInv(tipoAvaria *pAvarias, int quantAvarias, int numeroInvAux)
{
    int i, posicao;
    posicao = -1;
    for (i=0; i<quantAvarias; i++)
    {
        if (pAvarias[i].numInventario == numeroInvAux) // Elemento encontrado
        {
            posicao=i;         // Armazena posicao no vetor
            i=quantAvarias; // para ao continuar no for..
        }
    }
    return posicao;
}

void consultaAvariaPorNumeroInv (tipoAvaria *pAvaria, int quantAvarias)
{
    int posicao, numeroInvAux;

    if (quantAvarias == 0)
    {
        printf("\n\nATENCAO: nao existem Avarias registadas.\n");
    }
    else
    {
        printf("\n\n----------- CONSULTA AVARIA -----------  \n");
        numeroInvAux = lerInteiro("\nNr. do inventario: ", 1, MAX_EQUIPAMENTOS);

        posicao = procuraAvariaNrInv(pAvaria, quantAvarias, numeroInvAux);

        if (posicao == NAO_EXISTE)  // Equipamento não existe no vetor
        {
            printf("\n\nATENCAO: A Avaria com o nr de inventario %d nao existe.\n", numeroInvAux);
        }
        else
        {
            escreveDadosAvaria(pAvaria, 0, posicao);
        }
    }
}

void logEquipamentosReparados(tipoAvaria *pAvarias, int pos)
{

    FILE *fich;
    fich = fopen("log_equipamentos_reparados.txt","a");

    if(fich == NULL)
    {
        printf("Erro ao abrir ficheiro!");
    }
    else
    {
        fprintf(fich, "pos: %d", pos);
        fprintf(fich, "\n Numero da avaria: %d", pAvarias[pos].numeroAvaria);
        fprintf(fich, "\n Data: %.2d - %.2d - %.4d", pAvarias[pos].dataAvaria.dia, pAvarias[pos].dataAvaria.mes, pAvarias[pos].dataAvaria.ano);
        fprintf(fich, "\n Numero Inventario do equipamento: %d", pAvarias[pos].numInventario);
        fprintf(fich, "\n Descricao: %s", pAvarias[pos].descricao);
        fprintf(fich, "\n Custo da reparacao: %f", pAvarias[pos].custo);
    }
    fclose(fich);
}

void consultarAvariaPorData (tipoAvaria *pAvarias, int quantAvarias)
{
    tipoData data;
    int i, controlo = 0;

    if(quantAvarias == 0)
    {
        printf("\n\tNao foram inseridas avarias!\n");
    }
    else
    {
        printf("\n\tInsira a data que pretende procurar:");
        data = lerData();
        for (i=0; i<quantAvarias; i++)
        {
            if(pAvarias[i].dataAvaria.ano == data.ano && pAvarias[i].dataAvaria.mes == data.mes && pAvarias[i].dataAvaria.dia == data.dia)
            {
                escreveDadosAvaria(pAvarias, 0, i);
                controlo ++;
            }
        }
        if(controlo == 0)
        {
            printf("\n\t Nao existem avarias registadas nessa data! \n");
        }
    }
}

void listaEquipamentosAvariados(tipoEquipamento vetorEquipamentos[MAX_EQUIPAMENTOS], int quantEquipamentos, int quantAvarias)
{
    int i;
    if(quantEquipamentos==0)
    {
        printf("\n\tNao foram inseridos equipamentos");
    }
    else
    {
        if(quantAvarias==0)
        {
            printf("\n\tNao foram inseridas avarias!");
        }
        else
        {
            for(i=0; i<quantEquipamentos; i++)
            {
                if(vetorEquipamentos[i].estado==-1)
                {
                    escreveDadosEquipamento(vetorEquipamentos[i],i);
                }
            }
        }

    }
}









