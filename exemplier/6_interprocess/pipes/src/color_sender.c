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
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * @file color_sender.c
 *
 * @brief A program to send color values to a named pipe.
 *
 * This program, color_sender.c, is a C application that reads color values 
 * from the user and sends them to a named pipe. It is part of a system where
 * another program (color_changer.c) reads these color values from the pipe
 * and changes its background color accordingly.
 *
 * To use the Color Sender program, run it from the command line. It will 
 * prompt the user to enter color values. Once a color value is entered, it 
 * will be sent to the named pipe for the Color Changer program to read and 
 * act upon. The word "FIN" will send a termination order to the Color Changer
 * program and will exit.
 *
 * Example usage:
 * ./color_sender
 * Entrez une couleur en anglais (ex. : red, green, blue) : red
 * Entrez une couleur en anglais (ex. : red, green, blue) : green
 * Entrez une couleur en anglais (ex. : red, green, blue) : FIN
 */

#define COLORPIPE "colorpipe"
#define COLOR_NAME_SIZE 50

void trim_newline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main(void) {
    int pipe_fd = open(COLORPIPE, O_WRONLY);
    if (pipe_fd == -1) {
        perror("Error opening pipe for writing");
        exit(EXIT_FAILURE);
    }

    char color_name[COLOR_NAME_SIZE];
    ssize_t bytes_written;

    while(1) {
        printf("Enter a color name (e.g., red, green, blue): ");
        fgets(color_name, sizeof(color_name), stdin);
        trim_newline(color_name); /* Suppression du caractère de fin de ligne */

        /* Ecriture de la couleur vers le tube */
        bytes_written = write(pipe_fd, color_name, strlen(color_name) + 1);
        if (bytes_written == -1) {
            perror("Error writing to pipe");
            close(pipe_fd);
            exit(EXIT_FAILURE);
        }

        /* Teste si l'entrée est le mot-clé "FIN" de manière à quitter */
        if (strcmp(color_name, "FIN") == 0) {
            break;
        }
    }

    close(pipe_fd);

    return EXIT_SUCCESS;
}
