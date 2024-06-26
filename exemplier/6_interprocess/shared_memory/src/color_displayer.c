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
#include <gtk/gtk.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <gtk/gtk.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

/**
 * @file color_displayer.c
 *
 * This program displays color data from a shared memory segment using GTK+ 3.
 * It continuously updates the displayed colors based on the shared memory content.
 */

typedef struct {
    int *ptr;        /*<! Pointer to the shared memory segment */
    int matrix_size; /*<! Size of the color matrix */
} SharedData;

/** 
 * Redraws window every 500 ms.
 */
gboolean timeout_callback(gpointer data) {
    GtkWidget *darea = (GtkWidget *) data;
    gtk_widget_queue_draw(darea);
    return TRUE; /* Continue calling this function */
}

/**
 * Handles the 'draw' event to render the color data.
 */
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    SharedData *data;
    int i;
    int j;
    double red;
    double green;
    double blue;

    data = (SharedData *) user_data;
    for (i = 0; i < data->matrix_size; i++) {
        for (j = 0; j < data->matrix_size; j++) {
            red = data->ptr[(i * data->matrix_size + j) * 3] / 255.0;
            green = data->ptr[(i * data->matrix_size + j) * 3 + 1] / 255.0;
            blue = data->ptr[(i * data->matrix_size + j) * 3 + 2] / 255.0;
            cairo_set_source_rgb(cr, red, green, blue);
            cairo_rectangle(cr, j, i, 1, 1);
            cairo_fill(cr);
        }
    }
    return FALSE; /* Event propagation stops here */
}

int main(int argc, char *argv[]) {
    int shm_fd;
    GtkWidget *window;
    GtkWidget *darea;
    SharedData data;

    gtk_init(&argc, &argv);

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <matrix_size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    data.matrix_size = atoi(argv[1]);
    shm_fd = shm_open("/matrix", O_RDONLY, S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    data.ptr = mmap(0, data.matrix_size * data.matrix_size * 3 * sizeof(int),
                    PROT_READ, MAP_SHARED, shm_fd, 0);
    if (data.ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return EXIT_FAILURE;
    }

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), data.matrix_size, data.matrix_size);
    gtk_window_set_title(GTK_WINDOW(window), "POSIX Shared Memory Color Display");

    darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), darea);

    g_timeout_add(500, timeout_callback, darea);
    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), &data);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    if (munmap(data.ptr, data.matrix_size * data.matrix_size * 3 * sizeof(int)) == -1) {
        perror("munmap");
    }
    close(shm_fd);

    return EXIT_SUCCESS;
}
