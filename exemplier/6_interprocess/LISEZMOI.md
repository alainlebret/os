------------------------------------------------------------------------------
#                         Communication interprocessus
------------------------------------------------------------------------------
La communication entre processus peut être réalisée par plusieurs objets, 
entre autres :

* les tubes ;
* les segments de mémoire partagée ;
* les files de messages.

------------------------------------------------------------------------------
##                                  Tubes nommés
------------------------------------------------------------------------------
cf. chapitre fichiers
 
Fichiers : `tube_nomme_producteur.c`, `tube_nomme_consommateur.c`

------------------------------------------------------------------------------
##                               Mémoires partagées
------------------------------------------------------------------------------
Ce mécanisme permet à plusieurs processus de partager des segments mémoire. 
Chaque segment mémoire est identifié, au niveau du système, par une *clé* à 
laquelle correspond un identifiant. 
Lorsqu'un segment est attaché à un processus, les données qu'il contient 
sont accessibles en mémoire par l'intermédiaire d'un pointeur.

Fichiers : `shm_producteur.c`, `shm_consommateur.c`

------------------------------------------------------------------------------
##                              Files de messages
------------------------------------------------------------------------------
Ce mécanisme permet l'échange de messages entre des processus. Chaque message 
possède un corps de longueur variable, et un type (entier strictement positif)
qui peut servir à préciser la nature des informations contenues dans le corps.

Au moment de la réception, il est possible de choisir de ne sélectionner que
les messages d'un type donné.

Fichiers : `file_producteur.c`, `file_consommateur.c`
