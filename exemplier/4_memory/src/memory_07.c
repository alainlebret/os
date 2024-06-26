/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * @file memory_07.c
 *
 * This program demonstrates the memory layout of a process by printing the
 * addresses of the main function, another function, a local variable (stack),
 * and a dynamically allocated memory block (heap).
 */

void print_stack_location() {
    int stack_variable;
    printf("Location of stack variable inside function: %p\n", (void *)&stack_variable);
}

int main(void) {
    void *heap_memory;

    printf("Location of main() function code: %p\n", (void *)main);
    printf("Location of print_stack_location() function code: %p\n", (void *)print_stack_location);
    
    print_stack_location(); /* Function call to demonstrate stack location */

    heap_memory = malloc(100e6); /* Reduced the size to a more reasonable amount for demonstration */
    if (heap_memory == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return EXIT_FAILURE;
    }

    printf("Location of heap allocated memory: %p\n", heap_memory);

    free(heap_memory); /* Free the allocated memory */
    return EXIT_SUCCESS;
}
