#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glintstone.h>

int main()
{
    int a = 10;
    int b = 20;
    char *c = "Starscourge Radahn";

    char *store = glintstone_store("$int $int $string", &a, &b, c);

    printf("%s\n", store + 8);

    int x;
    int y;
    char z[255];

    x = *((int *)store);
    y = *((int *)(store + 4));
    strcpy(z, (store + 8));

    printf("%d %d %s\n", x, y, z);
}