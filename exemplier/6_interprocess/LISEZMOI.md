------------------------------------------------------------------------------
#                         Communication interprocessus
------------------------------------------------------------------------------
La communication entre processus peut être réalisée par plusieurs mécanismes, 
entre autres :

* les signaux (voir section processus) ;
* les tubes ;
* les segments de mémoire partagée ;
* les files de messages.

------------------------------------------------------------------------------
##                                     Tubes
------------------------------------------------------------------------------
Que ce soient des tubes anonymes (entre processus avec filiation) ou nommés
(entre processus quelconques), ces mécanismes permettent l'échange asynchrone
et unidirectionnel de données entre processus. 

Les tubes sous Ms-Windows permettent davantage de fonctionnalités que les
tubes sous UNIX.

Fichiers : `anonymous_pipe_01.c`, `anonymous_pipe_02.c`, 
`mkfifo_server.c`, `mkfifo_client.c`, `mkfifo_producer.c`, 
`mkfifo_consumer.c.c`, `win32_pipe_parent.c`, `win32_pipe_child.c`

------------------------------------------------------------------------------
##                               Mémoires partagées
------------------------------------------------------------------------------
Ce mécanisme permet à plusieurs processus de partager des segments mémoire. 
Chaque segment mémoire est identifié, au niveau du système, par une *clé* à 
laquelle correspond un identifiant. 
Lorsqu'un segment est attaché à un processus, les données qu'il contient 
sont accessibles en mémoire par l'intermédiaire d'un pointeur.

Fichiers : `mmap_buffer_01.c`, `mmap_buffer_02.c`, `mmap_buffer_03.c`,
`mmap_buffer_04.c`, `posix_shm_simple_1.c`, `posix_shm_simple_2.c`, 
`posix_shm_server.c`, `posix_shm_client.c`, `unix_shm.c`, `unix_shm_producer.c`,
`unix_shm_consumer.c`


------------------------------------------------------------------------------
##                              Files de messages
------------------------------------------------------------------------------
Ce mécanisme permet l'échange de messages entre des processus. Chaque message 
possède un corps de longueur variable, et un type (entier strictement positif)
qui peut servir à préciser la nature des informations contenues dans le corps.

Au moment de la réception, il est possible de choisir de ne sélectionner que
les messages d'un type donné.

Fichiers : `posix_msg_sender.c`, `posix_msg_receiver.c`
