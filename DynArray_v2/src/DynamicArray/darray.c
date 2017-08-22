#include <DynamicArray/darray.h>
#include <DynamicArray/debug.h>


DArray DArray_create(size_t initial_max)
{
    check(initial_max > 0, "Initial max must be > 0.");

    Item** items = calloc(initial_max, sizeof(Item*));
    check_mem(items);
    DArray array = {.end = 0, .max = initial_max,
        .expand_rate = DEFAULT_EXPAND_RATE, .contents = items};

    return array;

error:
    if(items)
        free(items);
    return (DArray){0};
}

DArray* DArray_create_p(size_t initial_max)
{
    check(initial_max > 0, "Initial max must be > 0.");

    DArray* array = malloc(sizeof(DArray*));
    check_mem(array);
    array->max = initial_max;
    array->contents = calloc(initial_max, sizeof(Item*));
    array->end = 0;
    array->expand_rate = DEFAULT_EXPAND_RATE;

    return array;

error:
    if(array)
        free(array);
    return NULL;
}

static inline int DArray_resize(DArray* array, size_t newsize)
{
    check(newsize > 0, "The newsize must be > 0.");
    array->max = newsize;

    Item** contents = realloc(array->contents, array->max * sizeof(Item*));

    // check contents, assume realloc doesn't harm the original on error
    check_mem(contents);
    array->contents = contents;

    return 0;
error:
    return -1;
}

int DArray_expand(DArray* array)
{
    size_t old_max = array->max;
    check(DArray_resize(array, array->max + array->expand_rate) == 0,
        "Failed to expand array to new size. %d",
        array->max + (int)array->expand_rate);

    memset(array->contents + old_max, 0, array->expand_rate + 1);
    return 0;

error:
    return 1;
}

int DArray_contract(DArray* array)
{
    int new_size = array->end < (int)array->expand_rate ?
        (int)array->expand_rate : array->end;

    return DArray_resize(array, new_size + 1);
}

void DArray_destroy(DArray* array)
{
    if(array){
        if(array->contents){
            free(array->contents);
        }
        free(array);
    }
}

void DArray_clear(DArray* array)
{
    int i = 0;
    for(i = 0; i < array->max; ++i){
        if(array->contents[i] != NULL){
            free(array->contents[i]);
        }
    }
}

void DArray_clear_destroy(DArray* array)
{
    DArray_clear(array);
    DArray_destroy(array);
}

int DArray_push(DArray* array, Item* item)
{
    array->contents[array->end] = item;
    ++array->end;

    if(DArray_end(array) >= DArray_max(array)){
        return DArray_expand(array);
    }else{
        return 0;
    }
}

int DArray_push_(DArray* array, Item item)
{
    Item* elem = malloc(sizeof(Item*));
    check_mem(elem);
    *elem = item;
    array->contents[array->end] = elem;
    ++array->end;
    
    if(DArray_end(array) >= DArray_max(array)){
        return DArray_expand(array);
    }else{
        return 0;
    }

error:
    return 1;
}

Item* DArray_pop(DArray* array)
{
    check(array->end - 1 >= 0, "Attempt to pop from empty array.");
    Item* item = DArray_remove(array, array->end - 1);
    --array->end;

    if(DArray_end(array) > (int)array->expand_rate &&
        DArray_end(array) % array->expand_rate){
        DArray_contract(array);
    }
    return item;
error:
    return NULL;
}