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
 * @author Alain Lebret
 * @version	1.1
 * @date 2023-11-27
 */

#define SHM_NAME "/color_memory"
#define SHM_SIZE 1024

void update_colors(GtkWidget* widget, gpointer data) 
{
    GdkRGBA color;
    char* color_data = (char*)data;  /* Cast shared memory pointer to char* */
    int red, green, blue;

    /* Assuming the shared memory contains color data in "R,G,B" format */
    sscanf(color_data, "%d,%d,%d", &red, &green, &blue);

    /* Set the RGB color values */
    color.red = (double)red / 255.0;
    color.green = (double)green / 255.0;
    color.blue = (double)blue / 255.0;
    color.alpha = 1.0; /* Alpha channel (opacity) */

    /* Set the background color of the widget */
    gtk_widget_override_background_color(widget, GTK_STATE_NORMAL, &color);

    /* Force a redraw of the widget */
    gtk_widget_queue_draw(widget);
}

int main(int argc, char* argv[]) 
{
	int shm_fd;
	char *shared_memory;
	GtkWidget *window;
	GtkWidget *colorDisplay;

    /* Initialize GTK */
    gtk_init(&argc, &argv);

    /* Create a GTK window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    /* Create other GTK widgets to display colors */
    colorDisplay = gtk_label_new(NULL);
    gtk_container_add(GTK_CONTAINER(window), colorDisplay);

    /* Open the shared memory */
    shm_fd = shm_open(SHM_NAME, O_RDONLY, 0644);
    if (shm_fd == -1) {
        handle_fatal_error("Error [shm_open()]: ");
    }

    /* Map the shared memory */
    shared_memory = (char*)mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        handle_fatal_error("Error [mmap()]: ");
    }

    /* Create a timer to update colors periodically */
    g_timeout_add(1000, (GSourceFunc)update_colors, shared_memory);

    /* Show the window and start the GTK main loop */
    gtk_widget_show_all(window);
    gtk_main();

    /* Clean up */
    munmap(shared_memory, SHM_SIZE);
    close(shm_fd);

    return EXIT_SUCCESS;
}
