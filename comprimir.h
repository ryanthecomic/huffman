#include "structs_huffman.h"

struct arvore
{
    void *byte;
    long frequencia;
    Arvore *next, *esquerda, *direita;
};

/**
 * @brief Esta função é usada para criar um novo nó da árvore de Huffman, alocar memória para armazenar o byte de 
 * dados, definir a frequência do nó e inicializar os ponteiros para os nós filhos como nulos.
 * 
 * @param i             Um ponteiro para void que vai pegar o byte para traferir para o nó da árvore.
 * @param frequencia    Um long para para a frequencia do byte.
 * @return              Um ponteiro para o novo nó criado.
 */
Arvore* novo_no_arvore(void *i, long frequencia)
{
    Arvore *novo_no = (Arvore*)malloc(sizeof(Arvore));
    if(novo_no == NULL)
    {
        printf("\nERRO AO ALOCAR NÓ DA ÁRVORE\n");
        exit(1);
    }
    novo_no->byte = (void*)malloc(1);
    if(novo_no->byte == NULL)
    {
        printf("\nERRO AO ALOCAR BYTE\n");
        exit(1);
    }
    memcpy(novo_no->byte, i, 1);
    novo_no->frequencia = frequencia;
    novo_no->esquerda = NULL;
    novo_no->next = NULL;
    novo_no->direita = NULL;

    return novo_no;
}

/**
 * @brief Essa função é usada para manter a fila de prioridades ordenada durante o processo de construção da árvore 
 * de Huffman, garantindo que os nós com frequências mais baixas estejam na frente da fila.
 * 
 * @param no_arvore     Um ponteiro para um ponteiro para a raiz da fila de prioridades.
 * @param no            Um ponteiro para um nó da árvore de Huffman que deve ser enfileirado.
 */
void enfileirar(Arvore **no_arvore, Arvore *no)
{
    Arvore *novo_no_fila = no;

    if((*no_arvore) == NULL || (*no_arvore)->frequencia >= novo_no_fila->frequencia)
    {
        novo_no_fila->next = *no_arvore;
        *no_arvore = novo_no_fila;
    }
    else
    {
        Arvore *aux = *no_arvore;
        while(aux->next != NULL)
        {
            if(aux->next->frequencia >= novo_no_fila->frequencia)
            {
                break;
            }
            aux = aux->next;
        }   
        novo_no_fila->next = aux->next;
        aux->next = novo_no_fila;
    }
    return;
}

/**
 * @brief Essa função é usada para manter a fila de prioridades atualizada durante o processo de construção da 
 * árvore de Huffman, garantindo que o nó com a menor frequência seja sempre removido da frente da fila.
 * 
 * @param fila      Um ponteiro para um ponteiro para a fila de prioridades.
 * @return          Um ponteiro para o nó "desenfileirado".
 */
Arvore* desenfileirar(Arvore **fila)
{
    if(fila == NULL)
    {
        return NULL;
    }
    Arvore *desenfileirado = *fila;
    *fila = desenfileirado->next;
    desenfileirado->next = NULL;
    return desenfileirado;
}

/**
 * @brief Essa função é usada durante a construção da árvore de Huffman para criar um novo nó raiz que combina dois 
 * nós filhos em uma árvore maior.
 * 
 * @param esquerda      Nó filho à esquerda que será a raiz de uma subárvore para montar nossa árvore de huffman.
 * @param direita       Nó filho à direita que será a raiz de uma subárvore para montar nossa árvore de huffman.
 * @param byte          O byte associado ao nó raiz que está sendo criado.
 * @return              Um ponteiro para o novo nó raiz.
 */
Arvore* novo_no_arvore_huffman(Arvore *esquerda, Arvore *direita, void *byte)
{
    Arvore *raiz = (Arvore*)malloc(sizeof(Arvore));
    if(raiz == NULL)
    {
        printf("\nERRO AO ALOCAR RAIZ\n");
        exit(1);
    }
    raiz->byte = (void*)malloc(1);
    if(raiz->byte == NULL)
    {
        printf("\nERRO AO ALOCAR BYTE PARA RAIZ\n");
        exit(1);
    }
    memcpy(raiz->byte, byte, 1);
    raiz->frequencia = (esquerda->frequencia) + (direita->frequencia);
    raiz->esquerda = esquerda;
    raiz->direita = direita;

    return raiz;
}

/**
 * @brief No geral, esta função monta a árvore de Huffman combinando gradualmente os nós da fila, até que reste 
 * apenas a árvore completa na fila. A árvore resultante é atribuída ao ponteiro arvore_huffman passado como argumento.
 * 
 * @param fila              Um ponteiro para uma fila de prioridades.
 * @param arvore_huffman    Um ponteiro para um ponteiro para a árvore de Huffman.
 */
void criar_arvore_huffman(Arvore **fila, Arvore **arvore_huffman)
{
    uint8_t byte = '*';
    //montando a arvore de huffman
    while((*fila)->next != NULL)
    {
        Arvore *esquerda = desenfileirar(&*fila), *direita = desenfileirar(&*fila), *raiz;
        raiz = novo_no_arvore_huffman(esquerda, direita, &byte);
        enfileirar(&*fila, raiz);
    }

    *arvore_huffman = desenfileirar(&*fila);
    return;
}

/**
 * @brief Essa função é usada para calcular a altura de uma árvore binária, representada por um nó.
 * 
 * @param no_arvore     Um ponteiro para um nó.
 * @return              A altura da árvore.
 */
long altura_arvore(Arvore *no_arvore)
{
    int esquerda, direita;
    if(no_arvore == NULL)
    {
        return -1;
    }
    else
    {
        esquerda = altura_arvore(no_arvore->esquerda)+1;
        direita = altura_arvore(no_arvore->direita)+1;
        if(esquerda > direita)
        {
            return esquerda;
        }
        else
        {
            return direita;
        }
    }
}

/**
 * @brief Essa função é usada para calcular o número de nós em uma árvore.
 * 
 * @param no_arvore     Um ponteiro para a raiz da nossa árvore.
 * @return              O número total de nós na árvore.
 */
long tamanho_arvore(Arvore *no_arvore)
{
    if(no_arvore == NULL)
    {
        return 0;
    }
    return 1 + (no_arvore->esquerda == NULL &&
                no_arvore->direita == NULL &&
                (*(unsigned char*)no_arvore->byte == '*' ||
                *(unsigned char*)no_arvore->byte == '\\'))
                +
                tamanho_arvore(no_arvore->esquerda)
                +
                tamanho_arvore(no_arvore->direita);
}

//cria o dicionario
/**
 * @brief Essa função é usada para preparar uma estrutura de dados que armazenará os códigos de Huffman 
 * associados a cada símbolo na árvore de Huffman.
 * 
 * @param tamanho_arvore    O tamanho da nossa árvore de Huffman.
 * @return                  Um ponteiro para um array de ponteiros de onde cada elemento desse array será 
 *                          uma sequência de bytes que representa o código para um símbolo.
 */
uint8_t** criar_dicionario(int tamanho_arvore)
{
    uint8_t **dicionario = (uint8_t**)malloc(sizeof(uint8_t*) * Max_table);
    if(dicionario == NULL)
    {
        printf("\nERRO AO ALOCAR DICIONÁRIO\n");
        exit(1);
    }
    for(int i = 0; i < Max_table; i++)
    {
        dicionario[i] = (uint8_t*)malloc(sizeof(uint8_t) * tamanho_arvore);
        if(dicionario[i] == NULL)
        {
            printf("\nERRO AO ALOCAR RESTO DO DICIONÁRIO\n");
            exit(1);
        }
        dicionario[i][0] = '\0';
    }

    return dicionario;
}

/**
 * @brief Essa função é usada para percorrer a árvore de Huffman a partir da raiz até as folhas, 
 * construindo os códigos de Huffman associados a cada símbolo e armazenando-os no dicionário para 
 * uso posterior na codificação e decodificação de dados.
 * 
 * @param dicionario        Um array de ponteiros representando o dicionário de códigos de Huffman.
 * @param raiz              O nó raiz da árvore de Huffman.
 * @param profundidade      A profundidade atual na árvore durante a recursão.
 * @param aux               Um array usado temporariamente para construir os códigos de Huffman.
 */
void gerar_codigos(uint8_t **dicionario, Arvore *raiz, long profundidade, uint8_t *aux)
{
    if(raiz->esquerda == NULL && raiz->direita == NULL)
    {
        uint8_t byte = *(uint8_t*)raiz->byte;
        for(int i = 0; i < profundidade; i++)
        {
            dicionario[byte][i] = aux[i];
        }
        dicionario[byte][profundidade] = '\0';
        return;
    }

    aux[profundidade] = '0';
    gerar_codigos(dicionario, raiz->esquerda, profundidade + 1, aux);

    aux[profundidade] = '1';
    gerar_codigos(dicionario, raiz->direita, profundidade + 1, aux);
}

/**
 * @brief Responsável por calcular o número de bits "desperdiçados" ou economizados após a codificação de Huffman. 
 * Ela faz isso comparando o número de bits necessários para representar os dados originais com o número de bits 
 * necessários após a compressão de Huffman.
 * 
 * @param dicionario        Um array de ponteiros representando o dicionário de códigos de Huffman.
 * @param frequencia        Um array de longs representando as frequências dos símbolos.
 * @return                  Determina o número de bits economizados (ou desperdiçados) que indica 
 *                          quantos bits não se ajustam completamente em um byte.
 */
int lixo(uint8_t **dicionario, long *frequencia){
    long bits_antes = 0;
    long bits_depois = 0;
    for(int i = 0; i < Max_table; i++)
    {
        if (frequencia[i] != 0)
        {
            bits_antes += frequencia[i] * 8;
            bits_depois += frequencia[i] * strlen(dicionario[i]);
        }
    }

    printf("\nbits antes: %ld || bits depois: %ld\n", bits_antes, bits_depois);

    return (bits_antes - bits_depois) % 8;
}

/**
 * @brief   Esta função é responsável por escrever a estrutura da árvore de Huffman no cabeçalho de um arquivo 
 *          comprimido. Isso é necessário para que o arquivo descomprimido saiba como reconstruir a árvore de Huffman 
 *          durante o processo de descompressão.
 * 
 * @param arquivo_comprimido    Um ponteiro para o arquivo comprimido onde a estrutura da árvore de Huffman será 
 *                              escrita no cabeçalho.
 * @param arvore                O nó raiz da árvore de Huffman.
 */
void escrever_arvore_no_cabecalho(FILE *arquivo_comprimido, Arvore *arvore)
{
    if(arvore == NULL)
    {
        return;
    }
    if(arvore->esquerda == NULL && arvore->direita == NULL && (*(uint8_t*)arvore->byte == '*' || *(uint8_t*)arvore->byte == '\\'))
    {
        uint8_t scape = (uint8_t)'\\';
        fwrite(&scape, sizeof(uint8_t), 1, arquivo_comprimido);
    }
    uint8_t byte = *(uint8_t*)arvore->byte;
    fwrite(&byte, sizeof(uint8_t), 1, arquivo_comprimido);
    escrever_arvore_no_cabecalho(arquivo_comprimido, arvore->esquerda);
    escrever_arvore_no_cabecalho(arquivo_comprimido, arvore->direita);
}

/**
 * @brief   Essa função permite que o arquivo descomprimido saiba quantos bits de lixo ignorar e como reconstruir 
 *          a árvore de Huffman para decodificar os dados comprimidos.
 * 
 * @param arquivo_comprimido    Um ponteiro para o arquivo comprimido onde o cabeçalho será escrito.
 * @param bits_de_lixo          O número de bits de "lixo" (bits extras) que podem ser ignorados na descompressão.
 * @param tamanho_arvore        O tamanho da árvore de Huffman (em bytes).
 * @param arvore                O nó raiz da árvore de Huffman.
 */
void escrever_cabecalho_no_arquivo(FILE *arquivo_comprimido, int bits_de_lixo,
 int tamanho_arvore, Arvore *arvore)
{
    //setando os bits
    uint16_t tamanho_lixo_e_arvore = 0, mascara;
    //guarda o lixo no vetor para ser gravado no arquivo
    tamanho_lixo_e_arvore |= bits_de_lixo << 13;
    //gravando o tamanho da arvore no vetor
    for(int i = 0; i < 13; i++)
    {
        mascara = 1 << i;
        if(tamanho_arvore & mascara)
        {
            tamanho_lixo_e_arvore |= 1 << i;
        }
        else
        {
            tamanho_lixo_e_arvore |= 0 << i;
        }
    }
    //Como a ordem em que a fwrite escreve depende da arquitetura do processador, ou seja, 
    //little-endian ou big-endian precisamos garantir que
    //os dois bytes para lixo e para tamanho da arvore sejam escritos corretamente,
    //para isso usamos a funcao htons da biblioteca arpa/inet.h
    tamanho_lixo_e_arvore = htons(tamanho_lixo_e_arvore);
    //gravando o lixo e o tamanho da arvore
    fwrite(&tamanho_lixo_e_arvore, 1, 2, arquivo_comprimido);
    //gravando a arvore
    escrever_arvore_no_cabecalho(arquivo_comprimido, arvore);
}

/**
 * @brief   Em resumo, essa função lê os dados originais, mapeia cada byte para seu código Huffman correspondente 
 *          e escreve os bits compactados no arquivo comprimido, garantindo que os bytes sejam escritos corretamente 
 *          no arquivo, mesmo quando eles não formam múltiplos de 8 bits.
 * 
 * @param arquivo_comprimido    Um ponteiro para o arquivo no qual os bits compactados serão escritos.
 * @param dados                 Um ponteiro para um array de bytes contendo os dados originais que serão compactados.
 * @param dicionario            Um array de strings (códigos de Huffman) que mapeiam cada byte para sua representação 
 *                              compactada.
 * @param tamanho_arquivo       O tamanho do array de dados (número de bytes) que serão compactados e escritos no 
 *                              arquivo.
 */
void escrever_bits_compactados(FILE *arquivo_comprimido, uint8_t *dados, uint8_t **dicionario, long tamanho_arquivo)
{
    long indice_bit = 7;
    uint8_t byte = 0;
    for(int i = 0; i < tamanho_arquivo; i++)
    {
        for(int j = 0; dicionario[dados[i]][j] != '\0'; j++)
        {
            if(dicionario[dados[i]][j] == '1')
            {
                byte |= 1 << indice_bit;
            }
            indice_bit--;
            if(indice_bit < 0)
            {
                fwrite(&byte, sizeof(unsigned char), 1, arquivo_comprimido);
                indice_bit = 7;
                byte = 0;
            }
        }
    }
    if(indice_bit != 7)
    {
        fwrite(&byte, sizeof(unsigned char), 1, arquivo_comprimido);
    }

    return;
}

/**
 * @brief É responsável por liberar a memória alocada dinamicamente para o dicionário de códigos de Huffman. 
 * 
 * @param dicionario    Um ponteiro para um array de ponteiros para strings que representa o dicionário de códigos de 
 *                      Huffman.
 */
void free_dicionario(uint8_t **dicionario)
{
    for(int i = 0; i < 256; i++)
    {
        free(dicionario[i]);
    }
    free(dicionario);
    return;
}

/**
 * @brief É responsável por liberar a memória alocada dinamicamente para a nossa árvore de Huffman. 
 * 
 * @param raiz  Um ponteiro para a raiz da nossa árvore de Huffman.
 */
void free_arvore_huffman(Arvore *raiz)
{
    if(raiz == NULL)
    {
        return;
    }
    free_arvore_huffman(raiz->esquerda);
    free_arvore_huffman(raiz->direita);
    free(raiz->byte);
    free(raiz);
    return;
}

/**
 * @brief   Essa função comprime um arquivo de entrada usando o algoritmo de Huffman, cria um arquivo comprimido com 
 *          a extensão ".huff" e armazena a árvore de Huffman e os bits compactados no cabeçalho do arquivo.
 * 
 * @param nome_arquivo  Uma string contendo o nome do arquivo que será comprimido.
 */
void comprimir(char *nome_arquivo)
{
    //cria a fila de frequências
    Arvore *fila = NULL;
    //criando arvore de huffman
    Arvore *arvore_huffman = NULL;
    //criando a variavel do dicionario
    uint8_t **dicionario; 
    //criando a variavel auxiliar para preencher o dicionario
    uint8_t *aux;

    //abrindo o arquivo
    FILE *arquivo = fopen(nome_arquivo, "rb"), *arquivo_comprimido;
    if(arquivo == NULL)
    {
        printf("\nArquivo não encontrado!\n");
        exit(1);
    }

    //criando a variavel que vai manter todos os bytes
    uint8_t *dados;
    //criando a tabela de frequencia
    long frequencia[Max_table], i;

    //lendo os bytes do arquivo
    //procura o fim do arquivo
    fseek(arquivo, 0, SEEK_END);
    //pega o tamanho do arquivo
    long tamanho_arquivo = ftell(arquivo);
    //volta o ponteiro para o inicio do arquivo
    fseek(arquivo, 0, SEEK_SET);
    //aloca memoria para a variavel dados que usaremos para pegar todos os bytes do arquivo
    dados = malloc(tamanho_arquivo);
    if(dados == NULL)
    {
        printf("\nNão foi possível alocar memória para o vetor do arquivo\n");
        exit(1);
    }
    //setando os bytes para 0
    memset(dados, 0, tamanho_arquivo);
    //pegando todos os bytes do arquivo
    fread(dados, 1, tamanho_arquivo, arquivo);
    //fechando o arquivo
    fclose(arquivo);

    //iniciando as frequencias como 0
    memset(frequencia, 0, Max_table*sizeof(long));

    //obtendo frequencias dos bytes
    for(i = 0; i < tamanho_arquivo; i++)
    {
        //pegando a frequencia de cada byte
        frequencia[dados[i]]++;
    }
    
    //montando a lista de frequência
    for(i = 0; i < Max_table; i++)
    {
        //avaliando se a frequencia e igual a 0 para nao pegarmos o byte que nao tem no arquivo
        if(frequencia[i] != 0)
        {
            //fazendo o cast da variavel i para do tipo unsigned char
            (uint8_t)i;
            //insere na fila de frequencia de maneira organizada
            enfileirar(&fila, novo_no_arvore(&i, frequencia[i]));
        }
    }

    //criando a arvore de huffman
    criar_arvore_huffman(&fila, &arvore_huffman);

    //pegando a altura da arvore
    long altura_da_arvore = altura_arvore(arvore_huffman);
    //pegando o tamanho da arvore
    long tamanho_da_arvore = tamanho_arvore(arvore_huffman);

    
    //criando o dicionário
    dicionario = criar_dicionario(altura_da_arvore + 1);

    //criando o vetor auxiliar para preencher o dicionario
    aux = (uint8_t*)malloc(sizeof(uint8_t) * (altura_da_arvore + 1));
    if(aux == NULL)
    {
        printf("\nNão foi possível alocar memória para o array auxiliar\n");
        exit(1);
    }
    
    //preenchendo o dicionario
    gerar_codigos(dicionario, arvore_huffman, 0, aux);
    
    //calculo do lixo de bits
    int bits_de_lixo = lixo(dicionario, frequencia);
    //mudanca do nome, exemplo: arquivo.txt vira arquivo.txt.huff
    for(i = 0; i != 106; i++)
    {
        if(nome_arquivo[i] == '\0' && i + 6 <= 106)
        {
            nome_arquivo[i] = '.';
            nome_arquivo[i + 1] = 'h';
            nome_arquivo[i + 2] = 'u';
            nome_arquivo[i + 3] = 'f';
            nome_arquivo[i + 4] = 'f';
            nome_arquivo[i + 5] = '\0';
            break;
        }
    }

    //escrevendo o arquivo comprimido
    arquivo_comprimido = fopen(nome_arquivo,"wb");
    if(arquivo_comprimido == NULL)
    {
        printf("\nNão foi possível abrir o arquivo de saída\n");
        exit(1);
    }
    //escrevendo o cabecalho
    escrever_cabecalho_no_arquivo(arquivo_comprimido, bits_de_lixo, tamanho_da_arvore, arvore_huffman);
    //escrevendo os bytes compactados
    escrever_bits_compactados(arquivo_comprimido, dados, dicionario, tamanho_arquivo);
    //fechando o arquivo
    fclose(arquivo_comprimido);

    printf("\nArquivo comprimido com sucesso!!!\n");

    //libera a memoria do dicionario
    free_dicionario(dicionario);
    //libera a memoria da arvore
    free_arvore_huffman(arvore_huffman);
    //libera a memoria do vetor auxiliar
    free(aux);
    //libera a memoria alocada para o vetor dados
    free(dados);
    aux = NULL;
    dicionario = NULL;
    arvore_huffman = NULL;
    dados = NULL;
    
    return;
}
