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
#include <string.h>
#include <gtk/gtk.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * @file color_changer.c
 * @brief A program that changes its background color based on input from a named pipe.
 *
 * This program, color_changer.c, is a GTK-based application that creates a window and 
 * continuously reads color values from a named pipe. It changes its background color 
 * to match the received color values, providing a dynamic visual display of colors.
 * When receiving "FIN", it unlinks the pipe and terminates.
 *
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2023-09-23
 */

#define COLORPIPE "colorpipe"

typedef struct {
    GtkWidget *window;
    int pipe_fd;
} ThreadData;

void set_window_color(gpointer user_data) 
{
    ThreadData *thread_data = (ThreadData *)user_data;
    GtkWidget *window = thread_data->window;
    int pipe_fd = thread_data->pipe_fd;

    char color_name[50];
    ssize_t bytes_read;

    while ((bytes_read = read(pipe_fd, color_name, sizeof(color_name))) > 0) {
        color_name[bytes_read - 1] = '\0'; /* Enlève le caractère de fin de passage à la ligne */

		/* Teste si le mot reçu est le mot-clé "FIN" de manière à quitter */
        if (strcmp(color_name, "FIN") == 0) {
            break;
        }
		
        GdkRGBA color;
        gdk_rgba_parse(&color, color_name);

        /* Met à jour la couleur pour le thread principal */
        gdk_threads_enter();
        gtk_widget_override_background_color(window, GTK_STATE_NORMAL, &color);
        gtk_widget_queue_draw(window);
        gdk_threads_leave();
    }
	
	/* Ferme et détruit le tube nommé */
    close(pipe_fd);
    unlink(COLORPIPE);

    /* Sort de la boucle principale de GTK */
    gdk_threads_enter();
    gtk_main_quit();
    gdk_threads_leave();
}

gpointer thread_func(gpointer user_data) 
{
    set_window_color(user_data);
    return NULL;
}

int main(int argc, char *argv[]) 
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Color Changer");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    int pipe_fd = open(COLORPIPE, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Error opening the pipe for reading");
        exit(EXIT_FAILURE);
    }

    ThreadData thread_data;
    thread_data.window = window;
    thread_data.pipe_fd = pipe_fd;

    /* Initialise le support des threads pour GTK */
    if (!g_thread_supported()) {
        g_thread_init(NULL);
        gdk_threads_init();
    }

    gdk_threads_enter();

    /* 
	 * Crée un thread pour la lecture de la couleur depuis le tube. Ceci est
	 * nécessaire afin de libérer le thread principal de cette tâche.
	 */
    GThread *pipe_thread = g_thread_new("PipeThread", thread_func, &thread_data);

    gtk_widget_show_all(window);

    gdk_threads_leave();

    gtk_main();

    g_thread_unref(pipe_thread);

    return EXIT_SUCCESS;
}
