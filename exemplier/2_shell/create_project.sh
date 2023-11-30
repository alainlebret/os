#!/bin/bash

#
# ENSICAEN
# 6 Boulevard Maréchal Juin
# F-14050 Caen Cedex
#
# Unix System Programming Examples / Exemplier de programmation système Unix
# "Shell bash" / "Interpréteur de commandes bash"
#
# This script automates the setup and management of C programming projects.
# It provides functions for creating, opening, compiling, and archiving projects.
#
# Authors: Alain Lebret (alain.lebret@ensicaen.fr)
# Dependencies: bash, gcc, make, wget
# Usage: Run the script and follow the on-screen menu options.
#
# This script is licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# GLOBALS
#
DEFAULT_PROJECT_PATH=""
AUTHOR1="Jean Saigne"
AUTHOR2="Jacques Horde"
DEFAULT_AUTHORS="$AUTHOR1 & $AUTHOR2"
DEFAULT_C_HEADER="/*
 * ENSICAEN
 * 6 Boulevard Marechal Juin
 * F-14050 Caen Cedex
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */"

DEFAULT_COMPILER="gcc"
DEFAULT_CFLAGS="-Wall -Wextra -ansi -pedantic -g"
DEFAULT_HEADER_PATH="include"
DEFAULT_DOC_PATH="doc"
DEFAULT_LIBS="-lm"
DEFAULT_EXEC="prog.exe"

# Example of a directory configuration array
# Used by initialize_rep()
DEFAULT_DIR_STRUCTURE=("bin" "bin/tests" ${DEFAULT_DOC_PATH} "etc" ${DEFAULT_HEADER_PATH} "lib" "src" "tests")

# All errors are logged 
exec 2>>/tmp/create_project.log


# Error handling function.
# Displays an error message and exits the script.
# Arguments:
#   $1 - Error message to display.
#   $2 - Name of the function where the error occurred.
function error() { 
    echo "Error in function $2: $1!" >&2 
    exit 1 
} 

# Displays the help message for the script usage.
# Arguments:
#   None
# Usage:
#   help
function help() { 
    echo "$(basename "$0") est un script shell pour créer des projets C."
    exit 0
} 

# Initializes the project repository.
# Creates a new directory structure based on the selected project template.
# Globals:
#   DEFAULT_PROJECT_PATH
# Arguments:
#   None
# Returns:
#   0 if successful, 1 if the project already exists or fails to create directories.
function initialize_rep() {
    if [ ! -e "$DEFAULT_PROJECT_PATH" ]; then
       if ! mkdir "$DEFAULT_PROJECT_PATH"; then
		  error "La création du dossier pour le projet a échoué."
          return 1
       fi
       for dir in "${DEFAULT_DIR_STRUCTURE[@]}"; do
          mkdir -p "$DEFAULT_PROJECT_PATH/$dir"
		  touch "$DEFAULT_PROJECT_PATH/$dir/.gitkeep"
       done
       return 0
    else
       error "Le projet existe déjà"
       return 1
 fi
}


# Creates a README.md file in the project directory.
# Globals:
#   DEFAULT_PROJECT_PATH - The path of the project directory.
# Arguments:
#   None
# Usage:
#   create_README
function create_README() {
   cat > "$DEFAULT_PROJECT_PATH/README.md"<<EOF
# Project "$DEFAULT_PROJECT_PATH"
## Description

A complete description of the project...

## Installation

A complete description of its installation...

```
make ...
```

## Usage 

A description of its usage...

## Dependancies

List of dependances... 

## Authors

"$DEFAULT_AUTHORS"
$(date)
EOF
}


# Customizes the Makefile parameters for the project.
# Prompts the user to input custom values for the compiler (CC) and 
# compiler flags (CFLAGS). If the user does not provide any input, 
# default values are used. This function allows the user to tailor 
# the build process to specific requirements or preferences.
#
# Globals:
#   DEFAULT_CC - The compiler to be used for the project (default: gcc).
#   DEFAULT_CFLAGS - The compiler flags to be used (default: -Wall -Wextra -ansi -pedantic -g).
#
# Arguments:
#   None
#
# Usage:
#   customize_makefile
function customize_makefile() {
    echo "Spécifiez le compilateur (par défaut : gcc) :"
    read -r input
    DEFAULT_CC=${input:-gcc}

    echo "Spécifiez les options du compilateur (par défaut : -Wall -Wextra -ansi -pedantic -g) :"
    read -r input
    DEFAULT_CFLAGS=${input:--Wall -Wextra -ansi -pedantic -g}

    echo "Spécifiez le dossier contenant les entêtes (par défaut : include) :"
    read -r input
    DEFAULT_HEADER_PATH=${input:-include}

    echo "Spécifiez les bibliothèques utilisées (par défaut : lm) :"
    read -r input
    DEFAULT_LIBS=${input:--lm}

    echo "Spécifiez le nom du programme exécutable (par défaut : prog.exe) :"
    read -r input
    DEFAULT_EXEC=${input:-prog.exe}
}

# Creates a Makefile in the project directory.
# Globals:
#   DEFAULT_PROJECT_PATH - The path of the project directory.
#   DEFAULT_HEADER_PATH - The path of the header directory
#   DEFAULT_CFLAGS - The flags for the C compiler
#   DEFAULT_LIBS - the libraries to link with
#   DEFAULT_EXEC - The executable name
# Arguments:
#   None
# Usage:
#   create_Makefile
function create_Makefile() {
   echo "CC       = ${DEFAULT_CC}
RM       = rm
DOC      = doxygen
CPPFLAGS = -I ${DEFAULT_HEADER_PATH}
CFLAGS   = ${DEFAULT_CFLAGS}
LDFLAGS  = -L ./lib ${DEFAULT_LIBS}

.PHONY: all doc clean distclean

all: ./bin/${DEFAULT_EXEC}

./bin/${DEFAULT_EXEC}: ./src/main.o # And others .o
	\$(CC) \$< -o \$@ \$(LDFLAGS)

./src/%.o: ./src/%.c
	\$(CC) \$(CPPFLAGS) \$(${DEFAULT_CFLAGS}) \$< -o \$@ -c

doc:
	-@\$(DOC) ${DEFAULT_DOC_PATH}/Doxyfile

clean:
	-@\$(RM) ./src/*.o

distclean: clean
	-@\$(RM) ./bin/${DEFAULT_EXEC}" > "$DEFAULT_PROJECT_PATH"/Makefile
}

# Creates the main C source file in the project's source directory.
# Globals:
#   DEFAULT_PROJECT_PATH - The path of the project directory.
#   DEFAULT_C_HEADER - C file header content.
# Arguments:
#   None
# Usage:
#   create_main
function create_main() {
   cat > "$DEFAULT_PROJECT_PATH/src/main.c"<<EOF
$DEFAULT_C_HEADER 

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

# Creates a Doxygen configuration file in the project's documentation directory.
# Globals:
#   DEFAULT_PROJECT_PATH - The path of the project directory.
#   DEFAULT_HEADER_PATH - The path of the headers directory.
#   DEFAULT_DOC_PATH - The path of the doxygen dcumentation/
# Arguments:
#   None
# Usage:
#   create_Doxyfile
function create_Doxyfile() {
   cat > "$DEFAULT_PROJECT_PATH/doc/Doxyfile"<<EOF
PROJECT_NAME = "$DEFAULT_PROJECT_PATH"
OUTPUT_DIRECTORY = $DEFAULT_DOC_PATH
FILE_PATTERNS = $DEFAULT_HEADER_PATH/*.h src/*.c
GENERATE_HTML = YES
GENERATE_LATEX = NO
GENERATE_RTF = NO
EOF
}

# Downloads the MinUnit testing framework files into the project.
# Globals:
#   DEFAULT_PROJECT_PATH - The path of the project directory.
# Arguments:
#   None
# Usage:
#   copy_minunit
function copy_minunit() {
	wget -P "${DEFAULT_PROJECT_PATH}/${DEFAULT_HEADER_PATH}" https://raw.githubusercontent.com/siu/minunit/master/minunit.h
	wget -P "${DEFAULT_PROJECT_PATH}/etc" https://raw.githubusercontent.com/siu/minunit/master/minunit_example.c
}

# Validates the project path input.
# Checks if the input is not empty and does not contain invalid characters.
# This function is used to ensure that the provided project path is suitable 
# for directory creation and does not cause any issues in the filesystem.
#
# Arguments:
#   $1 - The project path to validate.
#
# Returns:
#   0 if the project path is valid.
#   1 if the project path is invalid (either empty or contains special characters).
#
# Usage:
#   validate_project_path "example_project"
function validate_project_path() {
    if [[ -z "$1" ]]; then
        error "Entrée invalide : le nom du projet ne peut être vide."
        return 1
    elif [[ $1 =~ [^a-zA-Z0-9_-] ]]; then
        error "Entrée invalide : le nom du projet contient des caractères spéciaux."
        return 1
    fi
    return 0
}

# Interactive function to create a new C project.
# Prompts the user for the project name and initializes the project structure.
# Globals:
#   DEFAULT_PROJECT_PATH - The path of the project directory.
# Arguments:
#   None
# Usage:
#   create_project
function create_project() {
   echo "Entrer le nom du nouveau projet :"
   read -r DEFAULT_PROJECT_PATH
   if ! validate_project_path "$DEFAULT_PROJECT_PATH"; then
       return 1
   fi
   initialize_rep
   if [ "$?" == 0 ] 
   then
      create_README
      create_main
      customize_makefile
      create_Makefile
	  create_Doxyfile
	  copy_minunit
      echo "Projet $(basename "$DEFAULT_PROJECT_PATH") créé."
   fi
}

# Opens an existing project or creates it if it doesn't exist.
# Prompts the user for the project name.
# Globals:
#   DEFAULT_PROJECT_PATH - The path of the project directory.
# Arguments:
#   None
# Usage:
#   open_project
function open_project() {
   echo "Entrer le nom du projet à ouvrir :"
   read -r DEFAULT_PROJECT_PATH
   if [ ! -e "$DEFAULT_PROJECT_PATH" ] 
   then
      echo "Projet inexistant. Il va être créé."
      initialize_rep
      if [ "$?" == 0 ] 
      then
         create_README
         create_main
         customize_makefile
         create_Makefile
   	     create_Doxyfile
   	     copy_minunit
      fi
      echo "Projet $(basename "$DEFAULT_PROJECT_PATH") créé."
      return 0
   else 
      if [ ! -d "$DEFAULT_PROJECT_PATH" ] 
      then 
         echo "Un fichier porte déjà ce nom."
         DEFAULT_PROJECT_PATH=""
         return 10
      else
         echo "Projet ouvert."	      
      fi
fi
}

# Creates a compressed archive of the current project.
# The archive name includes authors' names and current date.
# Globals:
#   DEFAULT_PROJECT_PATH, AUTHOR1, AUTHOR2
# Arguments:
#   None
# Usage:
#   create_archive
function create_archive() {
   author1=$( echo "$AUTHOR1" | cut -f2 -d' ' )
   author2=$( echo "$AUTHOR2" | cut -f2 -d' ' )
   currentdate=$( date +%d−%m−%Y-%H-%M )
   
   if [ -n "$DEFAULT_PROJECT_PATH" ]; then
      cd "${DEFAULT_PROJECT_PATH}" || return ; make distclean; cd .. ;   
      tar czf "$DEFAULT_PROJECT_PATH-$author1-$author2-$currentdate.tgz" "$(basename "$DEFAULT_PROJECT_PATH")"
   else
      error "Projet courant inexistant !" "create_archive()"
   fi
   echo "Archive du projet courant créée."
}

# Deletes the current project directory and its contents.
# Globals:
#   DEFAULT_PROJECT_PATH - The path of the project directory.
# Arguments:
#   None
# Usage:
#   suppress_project
function suppress_project() {
   rm -R -f "$DEFAULT_PROJECT_PATH" ;  
   echo "Projet courant supprimé."
}

# Compiles the current C project using the Makefile.
# Globals:
#   DEFAULT_PROJECT_PATH - The path of the project directory.
# Arguments:
#   None
# Usage:
#   compile_project
function compile_project() {
   if [ -z "$DEFAULT_PROJECT_PATH" ]
   then
      echo "Aucun projet ouvert."
      return 12
   fi
   cd "${DEFAULT_PROJECT_PATH}" || return ; make ; cd .. ; 
   echo "Projet compilé."
}

# Main menu function.
# Displays the menu options and processes user input.
# The user can select options to create, open, archive, or delete projects.
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

# Checks dependancies on the system.
# Arguments:
#   None
# Usage:
#   check_dependencies
function check_dependencies() {
    local dependencies=(${DEFAULT_COMPILER} "doxygen" "make" "tar" "wget")
    local missing_deps=()

	for dep in "${dependencies[@]}"; do
	    if ! command -v "$dep" &> /dev/null; then
	        echo "Checking $dep: Not Found"
	        missing_deps+=("$dep")
	    else
	        echo "Checking $dep: Found"
	    fi
	done

    if [ ${#missing_deps[@]} -ne 0 ]; then
        error "Dépendances manquantes : ${missing_deps[*]}. Installez ces dépendances pour continuer."
    fi
}

#
# Beginning of the script
#
if [ "$#" -gt 1 ]; then
    error "Usage incorrect. Seule l'option '-h' est acceptée."
    exit 1
fi

if [ "$1" = "-h" ]; then
    help
fi

#check_dependencies
menu

exit 0

