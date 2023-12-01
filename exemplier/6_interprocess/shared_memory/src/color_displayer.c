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
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

/**
 * @file color_displayer.c
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2023-09-30
 */

int *ptr;
int MATRIX_SIZE;

/* Cette fonction sera appelée toutes les 500 ms pour redessiner la fenêtre. */
gboolean timeout_callback(gpointer data) 
{
    GtkWidget *darea = (GtkWidget *)data;
    gtk_widget_queue_draw(darea);
    return TRUE;
}

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) 
{
    int i;
	int j;

    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            double r = ptr[(i * MATRIX_SIZE + j) * 3] / 255.0;
            double g = ptr[(i * MATRIX_SIZE + j) * 3 + 1] / 255.0;
            double b = ptr[(i * MATRIX_SIZE + j) * 3 + 2] / 255.0;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, j, i, 1, 1);
            cairo_fill(cr);
        }
    }
    return FALSE;
}

int main(int argc, char *argv[]) 
{
    int shm_fd;
    GtkWidget *fenetre;
    GtkWidget *darea;

	/* Copie les arguments de la ligne de commande */
	int my_argc = argc;
	char **my_argv = g_strdupv(argv);

	/* Initialisation de GTK+ */
	gtk_init(&argc, &argv);

	/* Vérifie si la taille de la matrice est passée en argument */
	if (my_argc < 2) {
		fprintf(stderr, "Usage: %s <taille_de_la_matrice>\n", my_argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Convertit l'argument en un entier */
	MATRIX_SIZE = atoi(my_argv[1]);

	/* Libère la copie des arguments de la ligne de commande */
	g_strfreev(my_argv);
		
    shm_fd = shm_open("/matrix", O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    /* Mappe le segment de mémoire partagée dans l'espace d'adressage */
    ptr = mmap(0, MATRIX_SIZE * MATRIX_SIZE * 3 * sizeof(int), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    /* Crée une nouvelle fenêtre */
    fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(fenetre), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(fenetre), MATRIX_SIZE, MATRIX_SIZE);
    gtk_window_set_title(GTK_WINDOW(fenetre), "Démonstration de mémoire partagée POSIX");

    /* Crée une nouvelle zone de dessin */
    darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(fenetre), darea);

    /* Ajoute un timer qui appelle timeout_callback toutes les 500 ms */
    g_timeout_add(500, timeout_callback, darea);
	
    /* Connecte le signal "draw" à la fonction de rappel */
    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(G_OBJECT(fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Affiche la fenêtre */
    gtk_widget_show_all(fenetre);
    gtk_main();

    /* Détache le segment de mémoire partagée */
    if (munmap(ptr, MATRIX_SIZE * MATRIX_SIZE * 3 * sizeof(int)) == -1) {
        perror("munmap");
        return EXIT_FAILURE;
    }

    /* Ferme le descripteur de fichier de mémoire partagée */
    close(shm_fd);

    return EXIT_SUCCESS;
}
