#!/bin/bash

# A simple script to show how to use functions. A more complex function

less_simple () {
	JUST_A_SECOND=1
	let counter=0
	REPETITIONS=20

	echo "And now we run..."
	while [ "${counter}" -lt "${REPETITIONS}" ]
	do 
		echo "<------- FUNCTIONS are useful (${counter}) ------>"
 		sleep ${JUST_A_SECOND}
		let counter+=1
	done
}

less_simple
