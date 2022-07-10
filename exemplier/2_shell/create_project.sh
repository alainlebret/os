#!/bin/bash

#
# Unix System Programming Examples / Exemplier de programmation système Unix
#
# Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

PROJECT_PATH=""
AUTHOR1="Jean Saigne"
AUTHOR2="Jacques Horde"
AUTHORS="$AUTHOR1 & $AUTHOR2"
ENTETE_C="/*
 * ENSICAEN
 * 6 Boulevard Marechal Juin
 * F-14050 Caen Cedex
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */"
 
#
# 
#
function error() { 
    echo "Erreur : $1!" >&2 
    exit 1 
} 

#
# 
#
function help() { 
    echo "$(basename "$0") est un script shell pour créer des projets C."
    exit 0
} 

#
# 
#
function initialize_rep() {
   if [ ! -e "$PROJECT_PATH" ]; then
      mkdir "$PROJECT_PATH"
      for repertory in bin doc etc include lib object src
         do
            mkdir "$PROJECT_PATH/$repertory"
         done
      return 0
   else
      echo "Le projet existe déjà"
      return 1
   fi
}

#
# 
#
function create_README() {
   cat > "$PROJECT_PATH/README.txt"<<EOF
This program 
bla bla bla
bli bli bli
EOF
}

#
# 
#
function create_INSTALL() {
   cat > "$PROJECT_PATH/INSTALL.txt"<<EOF
# This program 
bla bla bla
bli bli bli
EOF
}

#
# 
#
function create_AUTHORS() {
   cat > "$PROJECT_PATH/AUTHORS.txt"<<EOF
$AUTHORS
EOF
}

#
# 
#
function create_Makefile() {
   printf "CC=gcc
RM=rm
CPPFLAGS=-I./include
CFLAGS=-Wall -Wextra -pedantic -O2
LDFLAGS=-L./lib

.PHONY: all clean distclean

all: ./bin/main.exe

./bin/main.exe: ./object/main.o
\t\$(CC) \$< -o \$@ \$(LDFLAGS)

./object/%.o: ./src/%.c
\t\$(CC) \$(CCFLAGS) \$(CPPFLAGS) \$< -o \$@ -c

clean:
\t-@\$(RM) ./src/*~ ./include/*~
\t-@\$(RM) ./object/*.o

distclean: clean
\t-@\$(RM) ./bin/main.exe" > "$PROJECT_PATH/Makefile"
}

#
# 
#
function create_main() {
   cat > "$PROJECT_PATH/src/main.c"<<EOF
$ENTETE_C 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
   #define USAGE "Usage : %s text_file\n"

   if ((argc > 1) && (!strcasecmp("-h", argv[1]))) {
      fprintf(stderr, USAGE, argv[0]);
      return EXIT_FAILURE;
   }   
   
   /* Your code here ... */
   return EXIT_SUCCESS;
}
EOF
}

#
# 
#
function create_project() {
   echo "Entrer le nom du nouveau projet :"
   read -r PROJECT_PATH
   initialize_rep
   if [ "$?" == 0 ] 
   then
      create_README
      create_INSTALL
      create_AUTHORS
      create_main
      create_Makefile
      echo "Projet $(basename $PROJECT_PATH) créé."
   fi
}

#
# 
#
function open_project() {
   echo "Entrer le nom du projet à ouvrir :"
   read PROJECT_PATH
   if [ ! -e "$PROJECT_PATH" ]
   then
      echo "Projet inexistant. Il va être créé."
      initialize_rep
      if [ "$?" == 0 ] 
      then
         create_README
         create_INSTALL
         create_AUTHORS
         create_main
         create_Makefile
      fi
      echo "Projet $(basename "$PROJECT_PATH") créé."
      return 0
   else 
      if [ ! -d "$PROJECT_PATH" ]
      then 
         echo "Un fichier porte déjà ce nom."
	 PROJECT_PATH=""
         return 10
      else
         echo "Projet ouvert."	      
      fi
fi
}

#
# Creates a .TGZ compressed archive
# The archive filename indicates both authors and the current date
# using the format:
# PROJECT_DIRECTORY-NAME1-NAME2-day-month-year-hour:min.tgz
#
function create_archive() {
   author1=$( echo "$AUTHOR1" | cut -f2 -d' ' )
   author2=$( echo "$AUTHOR2" | cut -f2 -d' ' )
   currentdate=$( date +%d−%m−%Y-%H-%M )
   
   if [ -n "$PROJECT_PATH" ]; then
      cd "${PROJECT_PATH}" || exit 1; make distclean; cd ..;
      tar czf "${PROJECT_PATH}-${author1}-${author2}-${currentdate}.tgz" "$( basename $PROJECT_PATH )"
   else
      error "Projet courant inexistant !" 
   fi
   echo "Archive du projet courant créée."
}

#
# Suppress the current C project
#
function suppress_project() {
   rm -R -f "$PROJECT_PATH"
   echo "Projet courant supprimé."
}

#
# Compiles the current C project
#
function compile_project() {
   if [ -z "$PROJECT_PATH" ]
   then
      echo "Aucun projet ouvert."
      return 12
   fi
   cd "${PROJECT_PATH}" ; make ; cd .. ; 
   echo "Projet compilé."
}

#
# The main menu
#
function menu() {
   clear ; loop=y
   while [ $loop = y ]
   do
      echo "Générateur de squelette d'un projet C"  
      echo "====================================="
      echo "N|n : créer un nouveau projet"
      echo "O|o : ouvrir un projet"
      echo "S|s : supprimer le projet courant"
      echo "C|c : compiler le projet courant"
      echo "A|a : archiver le projet courant"
      echo "Q|q : quitter"
      echo
      read -s choix # silent mode with no echo
      case $choix in
         N|n) create_project ;;
         O|o) open_project ;;
         S|s) suppress_project ;;
         C|c) compile_project ;;
         A|a) create_archive ;;
         Q|q) loop=n ;;
           *) echo "Choix invalide !" ;;
      esac
      echo
   done 
}

# Début du script
if [ $1 = "-h" ] 
then
   help
fi

menu
