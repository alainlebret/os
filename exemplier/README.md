# Exemplier -- Unix system programming examples

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

Ce dépôt contient une collection d'exemples de programmation système Unix en C, utilisés dans mes cours à l'Université Paris-Est, France (2011-2014), et à l'ENSICAEN, France (depuis 2016). Ces exemples couvrent divers aspects de la programmation système tels que la gestion des processus, la communication inter-processus, les sockets, et plus encore.

## Table des matières
- [1_process](1_process/README.md)
- [2_shell](2_shell/README.md)
- [3_files](3_files/README.md)
- [4_memory](4_memory/README.md)
- [5_synchronization](5_synchronization/README.md)
- [6_interprocess](6_interprocess/README.md)
- [7_sockets](7_sockets/README.md)
- [8_multiplexing](8_multiplexing/README.md)
- [9_threads](9_threads/README.md)
- [10_various](10_various/README.md)

## Utilisation

Pour utiliser ces exemples, assurez-vous de disposer des outils et environnements nécessaires. La plupart des exemples nécessitent un compilateur C, comme GCC, et un environnement Unix-like. Suivez ces étapes pour exécuter les exemples :

- Clonez le dépôt sur votre machine locale.
- Pour produire l'ensemble des exécutables, lancez la commande `make` à la racine du dépôt. Vous pouvez aussi naviguer vers le sous-répertoire de l'exemple que vous souhaitez exécuter et lancer `make`.
- Les programmes graphiques (`moving_window`, `color_*`, `message_viewer`) demandent GTK 3 (`pkg-config gtk+-3.0`) : sans GTK, `make` les ignore en le signalant et construit tous les autres.
- Quelques exemples sont propres à Linux (`timer_create()`, `sem_init()`…) : ils le disent dans leur en-tête et dans le README de leur dossier.

## Licence

Ces exemples de code sont fournis sous licence [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0).

----

This repository contains a collection of system programming examples in C, used in courses at Paris-Est University, France (2011-2014), and ENSICAEN, France (since 2016). These examples cover various aspects of system programming such as process management, interprocess communication, sockets, and more.

## Table of Contents
- [1_process](1_process/README.md)
- [2_shell](2_shell/README.md)
- [3_files](3_files/README.md)
- [4_memory](4_memory/README.md)
- [5_synchronization](5_synchronization/README.md)
- [6_interprocess](6_interprocess/README.md)
- [7_sockets](7_sockets/README.md)
- [8_multiplexing](8_multiplexing/README.md)
- [9_threads](9_threads/README.md)
- [10_various](10_various/README.md)

## Usage

To use these examples, ensure you have the necessary tools and environments. Most examples require a C compiler, like GCC, and a Unix-like environment. Follow these steps to run the examples:

- Clone the repository to your local machine.
- Navigate to the subdirectory of the example you want to run.
- Compile the source code using `make`.
- The graphical programs (`moving_window`, `color_*`, `message_viewer`) need GTK 3 (`pkg-config gtk+-3.0`): without GTK, `make` skips them with a message and builds all the others.
- A few examples are Linux only (`timer_create()`, `sem_init()`…): their header and the README of their directory say so.

## License

These codes are provided under the [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0) License.

