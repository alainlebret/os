#!/bin/bash

#
# ENSICAEN
# 6 Boulevard Maréchal Juin
# F-14050 Caen Cedex
#
# Unix System Programming Examples / Exemplier de programmation système Unix
# "Shell bash" / "Interpréteur de commandes bash"
#
# Copyright (C) 1995-2023 Alain Lebret (alain.lebret@ensicaen.fr)
#
# Licensed under the Apache License, Version 2.0 (the "License");
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

# A script to demonstrate a more complex function in Bash

less_simple () {
	local JUST_A_SECOND=$1  # Sleep duration as a parameter
	local counter=0
	local REPETITIONS=$2  # Number of repetitions as a parameter

	echo "And now we run..."
	while [ $counter -lt $REPETITIONS ]; do 
		echo "<------- FUNCTIONS are useful (${counter}) ------>"
 		sleep $JUST_A_SECOND
		counter=$((counter + 1))  # Increment counter
	done
}

# Call the function with parameters: sleep duration and number of repetitions
less_simple 1 20
