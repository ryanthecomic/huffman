#include "structs_huffman.h"

//struct para arvore de descompactacao
struct arvore_descomprimida
{
    void *byte;
    Arvore_D *esquerda, *direita;
};

/**
 * @brief 
 * 
 * @param c     É um valor de 8 bits (byte) em que você deseja verificar se um bit específico está definido ou não.
 * @param i     É o índice do bit que você deseja verificar, variando de 0 a 7, onde 0 representa o bit menos 
 *              significativo e 7 representa o bit mais significativo.
 * @return      O resultado da operação de AND como um valor 0 ou 1, indicando se o bit estava definido ou não.
 */
int esta_setado(uint8_t c, int i)
{
    unsigned char mascara = 1 << i;
    return mascara & c;
}

/**
 * @brief   Em resumo, esta função extrai os bits de lixo e o tamanho da árvore de Huffman do cabeçalho de um arquivo 
 *          comprimido e armazena essas informações nas variáveis *lixo e *tamanho_arvore para uso posterior no 
 *          processo de descompactação.
 * 
 * @param lixo              Um ponteiro para um inteiro que será usado para armazenar o número de bits de lixo no final 
 *                          do arquivo.
 * @param tamanho_arvore    Um ponteiro para um long que será usado para armazenar o tamanho da árvore de Huffman.
 * @param dados             Um ponteiro para um array de bytes que contém o cabeçalho do arquivo comprimido.
 */
void bits_de_lixo_e_tamanho_da_arvore(int *lixo, long *tamanho_arvore, uint8_t *dados)
{
    int i, j, k, bit;
    unsigned char mascara;
    //primeiro pegamos os bits referente ao lixo
    for(i = 7, j = 0; i >= 5; i--, j++)
    {
        bit = esta_setado(dados[0], i); 
        if(bit)
        {
            *lixo |= 1 << j;
        }
    }
    
    //pegando os bits do tamanho da arvore
    for(i = 4, j = 12, k = 0; j >= 0; j--)
    {
        bit = esta_setado(dados[k], i); 
        if(bit)
        {
            *tamanho_arvore |= 1 << j;
        }
        i--;
        if(i < 0)
        {
            i = 7;
            k++;
        }
    }

    return;
}

/**
 * @brief   Essa função é responsável por criar um novo nó para nossa árvore descompactada.
 * 
 * @param i     O byte que deve ser inserido no nosso novo nó.
 * @return      O nosso novo nó.
 */
Arvore_D* novo_no_arvore_D(void *i)
{
    Arvore_D *novo_no = (Arvore_D*)malloc(sizeof(Arvore_D));
    if(novo_no == NULL)
    {
        printf("\nErro ao alocar nó da árvore\n");
        exit(1);
    }
    novo_no->byte = (void*)malloc(1);
    if(novo_no->byte == NULL)
    {
        printf("\nErro ao alocar byte\n");
        exit(1);
    }
    memcpy(novo_no->byte, i, 1);
    novo_no->esquerda = NULL;
    novo_no->direita = NULL;

    return novo_no;
}

/**
 * @brief   esta função é responsável por reconstruir a árvore de Huffman a partir dos dados do cabeçalho de um 
 *          arquivo comprimido. A árvore resultante é usada na descompactação para mapear os códigos binários de 
 *          volta para os caracteres originais.
 * 
 * @param raiz              Um ponteiro para um nó da árvore de Huffman que representa a raiz da árvore.
 * @param dados             Um ponteiro para um array de bytes que contém os dados do cabeçalho do arquivo comprimido.
 * @param i                 Um ponteiro para um inteiro que representa a posição atual nos dados do cabeçalho. 
 * @param tamanho_arvore    Um longo que representa o tamanho da árvore de Huffman nos dados do cabeçalho.
 * @return                  A nossa árvore de Huffman usada para a descompactação.
 */
Arvore_D* montar_arvore_huffman_D(Arvore_D *raiz, uint8_t *dados, int *i, long tamanho_arvore)
{
    if(*i == tamanho_arvore)
    {
        return NULL;
    }
    else
    {
        if(dados[*i] == '\\')
        {
            (*i)++;
            raiz = novo_no_arvore_D(&dados[*i]);
            (*i)++;
            return raiz;
        }
        else if(dados[*i] == '*')
        {
            raiz = novo_no_arvore_D(&dados[*i]);
            (*i)++;
            raiz->esquerda = montar_arvore_huffman_D(raiz->esquerda, dados, i, tamanho_arvore);
            raiz->direita = montar_arvore_huffman_D(raiz->direita, dados, i, tamanho_arvore);
        }
        else
        {
            raiz = novo_no_arvore_D(&dados[*i]);
            (*i)++;
            return raiz;
        }
    }

    return raiz;
}

/**
 * @brief   Esta função é responsável por descompactar os dados compactados usando a árvore de Huffman e escrever os 
 *          dados descompactados no arquivo de saída.
 * 
 * @param arquivo_descomprimido     Um ponteiro para o arquivo de saída onde os dados descompactados serão escritos.
 * @param dados                     Um ponteiro para um array de bytes que contém os dados compactados a serem 
 *                                  descompactados.
 * @param tamanho_arquivo           O tamanho total do arquivo compactado em bytes.
 * @param i                         Um índice que representa a posição atual nos dados compactados.
 * @param arvore                    A raiz da árvore de Huffman que foi previamente montada a partir do cabeçalho do 
 *                                  arquivo compactado.
 * @param lixo                      O número de bits de lixo no final do arquivo compactado.
 */
void escrever_arquivo(FILE* arquivo_descomprimido, uint8_t* dados, long tamanho_arquivo, long i, Arvore_D* arvore, int lixo)
{
    Arvore_D* aux = arvore;

    while(i < tamanho_arquivo)
    {
        uint8_t byte = dados[i];

        for(int j = 7; j >= 0; j--)
        {
            if(i == tamanho_arquivo - 1 && j < lixo) return;

            if (esta_setado(byte, j))
            {
                aux = aux->direita;
            }
            else
            {
                aux = aux->esquerda;
            }

            if(aux->direita == NULL && aux->esquerda == NULL)
            {
                fwrite(&(*(uint8_t *)aux->byte), sizeof(uint8_t), 1, arquivo_descomprimido);
                aux = arvore;
            }
        }
        i++;
    }
}

/**
 * @brief   Esta função é usada para liberar a memória alocada para a árvore de Huffman após o processo de 
 *          descompactação ser concluído.
 * 
 * @param raiz  Um ponteiro para a raiz da árvore de Huffman a ser liberada.
 */
void free_arvore_huffman_D(Arvore_D *raiz)
{
    if(raiz == NULL)
    {
        return;
    }
    free_arvore_huffman_D(raiz->esquerda);
    free_arvore_huffman_D(raiz->direita);
    free(raiz->byte);
    free(raiz);

    return;
}

/**
 * @brief   Essa função descomprime um arquivo no formato Huffman, criando a árvore de Huffman a partir dos dados 
 *          e escrevendo o arquivo descompactado no diretório do nosso programa.
 * 
 * @param nome_arquivo  Uma string contendo o nome do arquivo que será descomprimido.
 */
void descomprimir(char *nome_arquivo)
{
    FILE *arquivo_comprimido, *arquivo_descomprimido;
    Arvore_D *arvore_huffman_descomprimida = NULL;
    uint8_t *dados;
    int bits_de_lixo = 0, i; 
    long tamanho_arvore = 0;
    arquivo_comprimido = fopen(nome_arquivo, "rb");
    if(arquivo_comprimido == NULL)
    {
        printf("\nNão foi possível encontrar o arquivo\n");
        exit(1);
    }
    //buscando o final do arquivo
    fseek(arquivo_comprimido, 0, SEEK_END);
    //pegando o tamanho do arquivo
    long tamanho_arquivo = ftell(arquivo_comprimido);
    //voltando para o inicio do arquivo
    fseek(arquivo_comprimido, 0, SEEK_SET);
    //alocando memoria para a variavel dados
    dados = malloc(tamanho_arquivo);
    if(dados == NULL)
    {
        printf("\nNão foi possível alocar memoria para vetor do arquivo\n");
        exit(1);
    }
    //setando 0 em todas as posicos de dados
    memset(dados, 0, tamanho_arquivo);
    //pegando todos os bytes do arquivo
    fread(dados, 1, tamanho_arquivo, arquivo_comprimido);
    //fechando o arquivo
    fclose(arquivo_comprimido);

    //pegando a quantidade de bits de lixo que temos e o tamanho da arvore
    bits_de_lixo_e_tamanho_da_arvore(&bits_de_lixo, &tamanho_arvore, dados);
    //montando a arvore de huffman
    i = 2;
    arvore_huffman_descomprimida = montar_arvore_huffman_D(arvore_huffman_descomprimida, dados, &i, tamanho_arvore + 2);

    //escrevendo arquivo descompactado
    int tamanho_nome_arquivo = strlen(nome_arquivo);
    nome_arquivo[tamanho_nome_arquivo - 5] = '\0';
    arquivo_descomprimido = fopen(nome_arquivo, "wb");
    if(arquivo_descomprimido == NULL)
    {
        printf("\nNão foi possível criar um arquivo de saída\n");
        exit(1);
    }
    escrever_arquivo(arquivo_descomprimido, dados, tamanho_arquivo, i, arvore_huffman_descomprimida, bits_de_lixo);

    //liberando o espaço
    free(dados);
    free_arvore_huffman_D(arvore_huffman_descomprimida);
    arvore_huffman_descomprimida = NULL;
    dados = NULL;
}
