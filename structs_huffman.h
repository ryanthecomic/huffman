#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <arpa/inet.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define Max_table 256

typedef struct arvore Arvore;
typedef struct arvore_descomprimida Arvore_D;
