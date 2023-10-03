/**
* código de huffman
* UFAL
*
* @author  PLÁCIDO AUGUSTUS DE OLIVEIRA CORDEIRO, RIAN ANTONIO DA SILVA GAIÃO, NELSON ALVES SOUZA MOREIRA
* @version 1.0
* @since   01/10/2023
*/


#include "comprimir.h"
#include "descomprimir.h"


void main()
{
    int opcao = -1;

    do
    {
        printf("\n\n\t < ESCOLHA UMA AÇÃO A SER REALIZADA >\n\n[1] COMPRIMIR ARQUIVO\n[2] DESCOMPRIMIR ARQUIVO\n[0] ENCERRAR PROGRAMA\n");
        scanf("%d", &opcao);
        char nome_arquivo[106];
        switch (opcao)
        {
        case 1:
            printf("\nEscreva o nome do arquivo (incluindo a extensão dele): \n");
            scanf("%s", nome_arquivo);
            printf("\nIniciando compressão do arquivo...\n");
            comprimir(nome_arquivo);
            break;
        case 2:
            printf("\nEscreva o nome do arquivo (incluindo a extensão dele): \n");
            scanf("%s", nome_arquivo);
            printf("\nIniciando descompressão do arquivo...\n");
            descomprimir(nome_arquivo);
            break;
        case 0:
            printf("\nEncerrando programa...\n");
            break;
        default:
            printf("\nOpção inválida, escolha novamente!\n");
            break;
        }
    } while(opcao != 0);
}
