#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <glintstone.h>

typedef struct glintstone_n
{
    struct glintstone_n *next;
    void *item;
} glintstone_n;

typedef struct glintstone_ll
{
    glintstone_n *head;
    glintstone_n *tail;
    size_t size;
} glintstone_ll;

glintstone_ll *create_glintstone_ll()
{
    glintstone_ll *ll = (glintstone_ll *)malloc(sizeof(glintstone_ll));
    ll->head = NULL;
    ll->tail = NULL;
    ll->size = 0;

    return ll;
}

typedef struct glintstone_pebble_t
{
    size_t length;
    void *item;
} glintstone_pebble_t;

// Massive pointer freeing issue here
void push_glintstone_ll(glintstone_ll *ll, void *item)
{
    glintstone_n *n = (glintstone_n *)malloc(sizeof(glintstone_n));
    n->next = NULL;
    n->item = item;

    if (!ll->size)
    {
        ll->head = n;
        ll->tail = n;
    }
    else
    {
        ll->tail->next = n;
        ll->tail = n;
    }

    ll->size++;
}

/**
 * See the input and outputs it's length
 * $int = 4 bytes
 * $string = until a null-terminator is found
 */
size_t glintstone_comparator(char *input, void *generic)
{
    if (!strcmp(input, "$int"))
    {
        return 4;
    }
    else if (!strcmp(input, "$string"))
    {
        return strlen((char *)generic) + 1;
    }

    return 0;
}

/**
 * Copies the exact amount of bytes into the destination via the pattern
 * $int = 4 bytes
 * $string = until a null-terminator is found
 *
 * PROBABLY deprecated
 */
void glintstone_consume(char *dest, void *generic, unsigned int *start_index, char *pattern)
{
    printf("Offset: %u\n", *start_index);
    if (!strcmp(pattern, "$int"))
    {
        printf("INT DETECTED: %d\n", *((int *)generic));
        for (int i = 0; i < 4; i++)
        {
            dest[*start_index] = *((char *)generic + i);
            (*start_index)++;
        }
    }
    else if (!strcmp(pattern, "$string"))
    {
        printf("STRING DETECTED: %s\n", (char *)generic);

        strcpy((dest + *start_index), (char *)generic);

        *start_index += strlen((char *)generic) + 1;
    }
}

void cuckoo_glintstone(char *dest, char *generic, unsigned int *start_index, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        dest[*start_index + i] = *((char *)(generic + i));
    }

    *start_index += len;
}

/**
 * Retrieves the pattern from a string
 * "$int $string $int" => "$int" "$string" "$int"
 */
char *glintstone_hopper(char **input)
{
    size_t len = strlen(*input);
    size_t internal_len = 0;
    char buff[64] = {0};

    for (int i = 0; i < len; i++)
    {
        if (*(*input + i) == ' ')
        {
            break;
        }

        // glintstone_append(buff, *(*input + i));
        strncat(buff, *input + i, 1);
        internal_len++;
    }

    if (internal_len < len)
    {
        *input = *input + internal_len + 1;
    }
    else
    {
        *input = *input + internal_len;
    }

    char *r = (char *)malloc(sizeof(char) * (internal_len + 1));
    strcpy(r, buff);
    r[internal_len] = '\0';

    return r;
}

/**
 * Real magic shit right here, but nah mostly it's for storing stuff
 */
char *glintstone_store(char *patterns, ...)
{
    va_list args;
    va_start(args, patterns);

    char *copy = (char *)malloc(sizeof(char) * (strlen(patterns) + 1));
    // char copy[strlen(patterns) + 1];
    char *copy_ptr = copy;

    strcpy(copy, patterns);
    copy[strlen(patterns)] = '\0';

    size_t final_length = 0;

    char pattern[16] = {1, 0};

    glintstone_ll *ll = create_glintstone_ll();

    while (*pattern)
    {
        // printf("Pattern before: %s\n", pattern);
        char *c = glintstone_hopper(&copy);

        memset(pattern, 0, 16);

        strcpy(pattern, c);

        // printf("Pattern after: %s\n", pattern);

        free(c);

        c = NULL;

        if (!strcmp(pattern, "$int"))
        {
            int *value = va_arg(args, int *);
            size_t value_length = glintstone_comparator(pattern, NULL);
            final_length += value_length;
            glintstone_pebble_t *pebble = (glintstone_pebble_t *)malloc(sizeof(glintstone_pebble_t));
            pebble->item = value;
            pebble->length = value_length;
            push_glintstone_ll(ll, pebble);
        }
        else if (!strcmp(pattern, "$string"))
        {
            char *value = va_arg(args, char *);
            size_t value_length = glintstone_comparator(pattern, value);
            final_length += value_length;
            glintstone_pebble_t *pebble = (glintstone_pebble_t *)malloc(sizeof(glintstone_pebble_t));
            pebble->item = value;
            pebble->length = value_length;
            push_glintstone_ll(ll, pebble);
        }
    }

    printf("Final length: %lu\n", final_length);

    char *storage = (char *)calloc(final_length, sizeof(char));

    unsigned int index = 0;
    glintstone_n *current = ll->head;
    while (current)
    {
        glintstone_pebble_t *pebble = (glintstone_pebble_t *)current->item;
        cuckoo_glintstone(storage, pebble->item, &index, pebble->length);
        free(pebble);
        current = current->next;
    }

    // copy = copy_ptr;

    free(copy_ptr);

    return storage;
}