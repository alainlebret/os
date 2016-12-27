#!/bin/sh

#
# ENSICAEN
# 6 Boulevard Maréchal Juin
# F-14050 Caen Cedex
#
# Unix System Programming Examples / Exemplier de programmation système Unix
# "Shell bash" / "Interpréteur de commandes bash"
#
# Copyright (C) 1995-2016 Alain Lebret (alain.lebret@ensicaen.fr)
#
# This work is licensed under a Creative Commons Attribution-ShareAlike 4.0
# International License. You should have received a copy of the License.
# If not, see https://creativecommons.org/licenses/by-sa/4.0/.
#

# A simple script to show how to use functions. A more complex function
less_simple () {
	JUST_A_SECOND=1
	let counter=0
	REPETITIONS=20

	echo "And now we run..."
	while [ ${counter} -lt ${REPETITIONS} ]
	do 
		echo "<------- FUNCTIONS are useful (${counter}) ------>"
 		sleep ${JUST_A_SECOND}
		let counter+=1
	done
}

less_simple
