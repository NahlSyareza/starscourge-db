#include <stdio.h>

typedef struct glintstone_schema_t
{
    char *schema_description;
    char *storage;
} glintstone_schema_t;

// void glintstone_consume(char *dest, void *generic, unsigned int *start_index, char *pattern);
char *glintstone_store(char *patterns, ...);