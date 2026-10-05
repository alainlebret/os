/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2026 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <gtk/gtk.h>

/**
 * @file color_displayer_without_synchro.c
 *
 * This GTK application is designed to display colors that are updated 
 * periodically. The color data is intended to be read from a shared 
 * memory segment and displayed in a GTK window.
 *
 * WARNING: synchronization is deliberately missing (see
 * color_writer_without_synchro.c). Compare with color_displayer.c.
 */

#define SHM_NAME "/color_memory"
#define SHM_SIZE 1024

typedef struct {
    GtkCssProvider *css;       /* Background color of the window */
    const char *shared_memory; /* Color data written by the writer */
} shared_data;

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * Updates colors from the shared memory. Called every second by the GTK
 * main loop (GSourceFunc: a single gpointer argument, returns a gboolean).
 * No synchronization: the string may be read while it is being written.
 */
gboolean update_colors(gpointer user_data) {
    shared_data *data = user_data;
    char css[80];
    int red;
    int green;
    int blue;

    /* The shared memory contains color data in "R,G,B" format */
    if (sscanf(data->shared_memory, "%d,%d,%d", &red, &green, &blue) == 3) {
        snprintf(css, sizeof(css), "* { background-color: rgb(%d,%d,%d); }",
                 red, green, blue);
        gtk_css_provider_load_from_data(data->css, css, -1, NULL);
    }

    return G_SOURCE_CONTINUE;   /* Keep the timer */
}

int main(int argc, char *argv[]) {
    int shm_fd;
    char *shared_memory;
    shared_data data;
    GtkWidget *window;
    GtkWidget *colorDisplay;

    /* Initialize GTK */
    gtk_init(&argc, &argv);

    /* Create a GTK window whose background color is set by a CSS provider */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    data.css = gtk_css_provider_new();
    gtk_style_context_add_provider(gtk_widget_get_style_context(window),
                                   GTK_STYLE_PROVIDER(data.css),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    /* Create other GTK widgets to display colors */
    colorDisplay = gtk_label_new(NULL);
    gtk_container_add(GTK_CONTAINER(window), colorDisplay);

    /* Open the shared memory */
    shm_fd = shm_open(SHM_NAME, O_RDONLY, S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        handle_fatal_error("Error [shm_open()]");
    }

    /* Map the shared memory */
    shared_memory = (char *) mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        handle_fatal_error("Error [mmap()]");
    }
    data.shared_memory = shared_memory;

    /* Create a timer to update colors periodically */
    g_timeout_add(1000, update_colors, &data);

    /* Show the window and start the GTK main loop */
    gtk_widget_show_all(window);
    gtk_main();

    /* Clean up */
    munmap(shared_memory, SHM_SIZE);
    close(shm_fd);
    g_object_unref(data.css);

    return EXIT_SUCCESS;
}
