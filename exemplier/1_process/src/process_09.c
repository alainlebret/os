/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2023 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/wait.h>

/**
 * @file process_09.c
 *
 * Another simple program that uses execvp() to executes different applications with arguments.
 *
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.1
 * @date 2023-09-19
 */

int main(void)
{
	int pid1;
	int pid2;
	int pid3;
	int pid4;	
	int status;

	char *firefox_args1[] = { "firefox", "-url", "https://foad.ensicaen.fr", 
							"-new-tab", "-url", "https://gitlab.ecole.ensicaen.fr", NULL };
	char *firefox_args2[] = { "firefox", "--search", "chatgpt", NULL };
	char *gedit_args[] = { "gedit", "pointeurs_et_cie1.c", NULL };
	char *vlc_args[] = { "vlc", "http://web.ecole.ensicaen.fr/~alebret/ressources/mister_trololo.mp4", NULL };
	
	pid1 = fork();
	if (pid1 == 0) { /* child 1 executes firefox with args 1 */
		execvp("/usr/bin/firefox", firefox_args1);
	} else { /* le père attend */
		pid2 = fork();
		if (pid2 == 0) { /* child 2 executes firefox with args 2 */
			execvp("/usr/bin/firefox", firefox_args2);
		} else {
			pid3 = fork();
			if (pid3 == 0) { /* child 3 executes gedit with args */
				execvp("/usr/bin/gedit", gedit_args);
			} else {
				pid4 = fork();
				if (pid4 == 0) {
					execvp("/usr/bin/vlc", vlc_args);
				} else {
					wait(NULL);
					wait(NULL);
					wait(NULL);
					wait(NULL);					
				}
			}
		}
	}

	return EXIT_SUCCESS;
}     
