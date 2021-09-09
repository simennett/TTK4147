#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "array.h"


// Construction / Destruction

int main() {
    printf("start\n");
    struct Array myArray = array_new(2);

    for (long some_data = 0; some_data < 4000000000; some_data++) {
        array_insertBack(&myArray, some_data);
    }

    return 0;
}

Array array_new(long capacity){
    assert(capacity > 0);
    return (Array){malloc(sizeof(long)*capacity), 0, 0, capacity};
}

void array_destroy(Array a){
    free(a.data);
}

// Primitives

long array_empty(Array a){
    return a.back <= a.front;
}

long array_front(Array a){
    return a.data[a.front];
}

long array_back(Array a){
    return a.data[a.back - 1];
}

void array_popFront(Array* a){
    a->front++;
}

void array_popBack(Array* a){
    a->back--;
}

Array array_save(Array a){
    return (Array){a.data, a.front, a.back, a.capacity};
}


// Iteration

void array_foreach(Array a, void fn(long)){
    for(Array b = array_save(a); !array_empty(b); array_popFront(&b)){
        fn(array_front(b));
    }
}

void array_foreachReverse(Array a, void fn(long)){
    for(Array b = array_save(a); !array_empty(b); array_popBack(&b)){
        fn(array_back(b));
    }
}

static void _array_printSingleLongHelper(long i){
    printf(", %ld", i);
}

void array_print(Array a){
    printf("Array:{");
    if(!array_empty(a)){
        printf("%ld", array_front(a));
        array_popFront(&a);
    }
    array_foreach(a, _array_printSingleLongHelper);
    printf("}\n");
}


// Capacity

long array_length(Array a){
    return a.back - a.front;
}

void array_reserve(Array* a, long capacity){
    Array Array_new = array_new(capacity);

    // Check whether the array is empty
    while(!array_empty(*a)) {
        array_insertBack(&Array_new, array_front(*a));
        array_popFront(a); //go through the array and move data
    }
    array_destroy(*a); //Remove old array allocation
    *a = Array_new; //Replace pointer
}


// Modifiers

void array_insertBack(Array* a, long stuff){
    printf("capacity: %li\nback: %li\n", a->capacity, a->back);

    if (a->back == a->capacity) {
        array_reserve(a, a->capacity + 1);
    }

    a->data[a->back] = stuff;
    a->back++;
}