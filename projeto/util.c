#include <stdio.h>
#include "util.h"

void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void copiar_string(char *dest, const char *src, int n)
{
    int i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
}

int comparar_strings(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

int tamanho_string(const char *s)
{
    int len = 0;
    while (s[len] != '\0')
        len++;
    return len;
}

void concatenar_string(char *dest, const char *src)
{
    int len = tamanho_string(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[len + i] = src[i];
        i++;
    }
    dest[len + i] = '\0';
}