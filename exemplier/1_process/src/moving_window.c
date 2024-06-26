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
#include <signal.h>
#include <math.h>
#include <stdlib.h>

/**
 * @file moving_window.c
 *
 * @brief This program creates a GTK window that changes its background 
 * color in response to SIGUSR1 and SIGUSR2 signals. It demonstrates 
 * inter-process communication using UNIX signals and GTK for GUI.
 *
 * The program is a GTK-based application that displays a window with 
 * a specified initial color and position. It sets up signal handlers for
 * SIGUSR1 and SIGUSR2. When these signals are received, the background 
 * color of the window changes to predefined colors (color1 and color2).
 * Additionally, the program demonstrates the use of random color generation
 * close to the initial color, signal handling in a GUI application, and
 * basic GTK window creation and manipulation. This example is useful for
 * understanding how to integrate system-level signal handling with
 * graphical user interfaces in C.
 */

GdkRGBA initial_color;
GdkRGBA color;
GtkWidget *window;

volatile sig_atomic_t sigusr1_received = 0;
volatile sig_atomic_t sigusr2_received = 0;
double color1[4] = {0.28, 0.26, 0.12, 1.0};
double color2[4] = {0.01, 0.19, 0.19, 1.0};

void change_color(double new_color[]) {
    color.red = new_color[0];
    color.green = new_color[1];
    color.blue = new_color[2];
    color.alpha = new_color[3];

    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &color);
    gtk_widget_queue_draw(window);
}

GdkRGBA generate_random_color_close_to_initial() {
    GdkRGBA new_color;
    double delta = 0.8;  /* Adjust this value to control the "speed" of the transition */

    new_color.red = initial_color.red + ((double) rand() / RAND_MAX - 0.5) * delta;
    new_color.green = initial_color.green + ((double) rand() / RAND_MAX - 0.5) * delta;
    new_color.blue = initial_color.blue + ((double) rand() / RAND_MAX - 0.5) * delta;
    new_color.alpha = 1.0;

    /* Clamp the values between 0 and 1 */
    new_color.red = (new_color.red > 1.0) ? 1.0 : (new_color.red < 0.0) ? 0.0 : new_color.red;
    new_color.green = (new_color.green > 1.0) ? 1.0 : (new_color.green < 0.0) ? 0.0 : new_color.green;
    new_color.blue = (new_color.blue > 1.0) ? 1.0 : (new_color.blue < 0.0) ? 0.0 : new_color.blue;

    return new_color;
}

static gboolean check_sigusr1_received(gpointer data) {
    if (sigusr1_received) {
        /* Reset the flag */
        sigusr1_received = 0;

        change_color(color1);

        color = generate_random_color_close_to_initial();
        gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &color);
        gtk_widget_queue_draw(window);
    }

    return TRUE;  /* Continue checking */
}

static gboolean check_sigusr2_received(gpointer data) {
    if (sigusr2_received) {
        /* Reset the flag */
        sigusr2_received = 0;

        change_color(color2);

        color = generate_random_color_close_to_initial();
        gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &color);
        gtk_widget_queue_draw(window);

    }

    return TRUE;  /* Continue checking */
}

static void handle_sigusr1(int signum) {
    sigusr1_received = 1;
}

static void handle_sigusr2(int signum) {
    sigusr2_received = 1;
}

int main(int argc, char *argv[]) {
    GtkWidget *label;
    char pid_text[100];

    /* Set up SIGUSR1 and SIGUSR2 signal handlers using sigaction */
    struct sigaction sa1;
    struct sigaction sa2;

    sa1.sa_handler = handle_sigusr1; /* Pointer to the signal handler function 1 */
    sigemptyset(&sa1.sa_mask); /* Clear any blocked signals during the execution of the signal handler */

    if (sigaction(SIGUSR1, &sa1, NULL) == -1) {
        perror("Error setting up sigaction for SIGUSR1");
        return EXIT_FAILURE;
    }

    sa2.sa_handler = handle_sigusr2; /* Pointer to the signal handler function 2 */
    sigemptyset(&sa2.sa_mask); /* Clear any blocked signals during the execution of the signal handler */

    if (sigaction(SIGUSR2, &sa2, NULL) == -1) {
        perror("Error setting up sigaction for SIGUSR2");
        return EXIT_FAILURE;
    }

    /* Initialize GTK */
    gtk_init(&argc, &argv);

    /* Check if we have enough arguments */
	if (argc != 4) {
	    g_print("Usage: %s <x-pos> <y-pos> <color>\nExample color formats: '#RRGGBB', 'rgb(r,g,b)', 'rgba(r,g,b,a)'\n", argv[0]);
	    return EXIT_FAILURE;
	}

    int x_pos = atoi(argv[1]);
    int y_pos = atoi(argv[2]);
    const char *color_name = argv[3];

    /* Initialize random seed */
    srand(time(NULL));

    /* Set up a timeout function to check the signal_received flag every 100ms */
    g_timeout_add(100, check_sigusr1_received, NULL);
    g_timeout_add(100, check_sigusr2_received, NULL);

    /* Create a new window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Position and Color");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_window_move(GTK_WINDOW(window), x_pos, y_pos);

    /* Set the background color */
    if (gdk_rgba_parse(&initial_color, color_name)) {
        color = initial_color;
        gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &color);
    } else {
        g_print("Failed to parse color: %s\n", color_name);
        return EXIT_FAILURE;
    }

    /* Create a label with the PID and add it to the window */
    snprintf(pid_text, sizeof(pid_text), "%d", getpid());
    label = gtk_label_new(pid_text);
    gtk_container_add(GTK_CONTAINER(window), label);

    /* Connect the close event */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Show the window */
    gtk_widget_show_all(window);

    /* Main GTK loop */
    gtk_main();

    return EXIT_SUCCESS;
}
