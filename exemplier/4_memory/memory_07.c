/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret (alain.lebret [at] ensicaen [dot] fr)
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

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2019-01-21
 */

/**
 * @file memory_07.c
 */

int f(void)
{
    int val;

    val = 1;
    printf("location of stack: %p\n", (void *)&val);

    return val;
}

int main(int argc, char *argv[])
{
    printf("location of main code: %p\n", main);
    printf("location of f() code: %p\n", f);
    int val = f();
    printf("location of stack: %p\n", (void *)&val);
    printf("location of heap: %p\n", malloc(100e8));

    return 0;
}

