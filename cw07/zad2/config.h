#ifndef CONFIG_H
#define CONFIG_H

#define COOKER_COUNT 3
#define SUPPLIER_COUNT 4

#define MAX_PIZZA 5


typedef enum {CREATED, PACKED, SENT} pizza_status;

typedef struct pizza_str {
    pizza_status status;
    int value;
} pizza_str;

typedef struct memory_str {
    int furnace_index;
    int furnace_size;
    int table_size;
    int table_index;
    pizza_str pizzas_furnace[MAX_PIZZA];
    pizza_str pizzas_table[MAX_PIZZA];
} memory_str;

#endif