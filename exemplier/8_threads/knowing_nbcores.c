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

/**
 * @author Alain Lebret
 * @version	1.0
 * @date 2023-10-10
 */

/**
 * @file knowing_cores.c
 *
 * A simple program used to know how much cores on your Linux system.
 *
 */
int main(void)
{
	int cores;
	
	FILE *f = popen("grep processor /proc/cpuinfo | wc -l", "r");
	if (!f) { 
		perror("popen"); 
		exit(EXIT_FAILURE); 
	};
	fscanf(f, "%d", &cores);
	pclose(f);
	
	printf("Number of cores: %d\n", cores);
}
