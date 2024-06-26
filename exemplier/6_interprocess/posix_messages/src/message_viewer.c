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
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include "macosx/mqueue.h"
#elif __linux__
#include <mqueue.h>
#endif

/**
 * @file message_viewer.c
 * @brief Receive messages throught a message queue.
 */

#define QUEUE_NAME "/myqueue"

typedef struct {
    char sentence[1024];
    int priority;
} Message;

GtkWidget *window;
GtkWidget *grid;

/* An array to keep track of message labels for each priority */
GtkWidget *message_labels[10] = {NULL};

void display_message(Message msg) {
    if (msg.priority < 0 || msg.priority >= 10) {
        fprintf(stderr, "Invalid message priority: %d\n", msg.priority);
        return;
    }

    /* Remove the old message label for the same priority */
    if (message_labels[msg.priority] != NULL) {
        gtk_widget_destroy(message_labels[msg.priority]);
    }

    GtkWidget *label = gtk_label_new(msg.sentence);
    gtk_widget_set_halign(label, GTK_ALIGN_START);  // Horizontal alignment
    gtk_widget_set_valign(label, GTK_ALIGN_START);  // Vertical alignment

    gtk_grid_attach(GTK_GRID(grid), label, 0, msg.priority, 1, 1);

    /* Store the new label for the same priority */
    message_labels[msg.priority] = label;

    gtk_widget_show_all(window);
}

void read_queue() {
    mqd_t mq;
    struct mq_attr attr;
    Message msg;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Message);
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_RDONLY, 0600, &attr);
    if (mq == (mqd_t) - 1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if (mq_receive(mq, (char *) &msg, sizeof(Message), NULL) == -1) {
            perror("mq_receive");
            continue;
        }

        display_message(msg);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Message Queue Consumer");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    /* Start reading messages from the memory queue */
    g_thread_new(NULL, (GThreadFunc) read_queue, NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
