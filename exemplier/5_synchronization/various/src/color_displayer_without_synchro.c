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
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <gtk/gtk.h>

/**
 * @file color_displayer_without_synchro.c
 *
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2023-09-07
 */

#define SHM_NAME "/color_memory"
#define SHM_SIZE 1024

void update_colors(GtkWidget* widget, char* color_data) 
{
    GdkRGBA color;
    char* token;
    int red;
	int green;
	int blue;

    /* Split the color_data string into R, G, and B components */
    token = strtok(color_data, ",");
    if (token != NULL) {
        red = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) {
            green = atoi(token);
            token = strtok(NULL, ",");
            if (token != NULL) {
                blue = atoi(token);

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
        }
    }
}

int main(int argc, char* argv[]) 
{
    /* Initialize GTK */
    gtk_init(&argc, &argv);

    /* Create a GTK window */
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    /* Create other GTK widgets to display colors */
    GtkWidget* colorDisplay = gtk_label_new(NULL);
    gtk_container_add(GTK_CONTAINER(window), colorDisplay);

    /* Create a timer to update colors periodically */
    g_timeout_add(1000, (GSourceFunc)update_colors, colorDisplay);

    /* Show the window and start the GTK main loop */
    gtk_widget_show_all(window);
    gtk_main();

    return EXIT_SUCCESS;
}
