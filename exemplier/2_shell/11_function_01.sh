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

#!/bin/bash

# A script to demonstrate the use of functions in Bash

# Function definition
display_message () {
  local message=$1  # Using a parameter within the function
  echo "We are in the display_message function."
  echo "Message: $message"
  echo "Now we exit the function."
  return 0  # Indicate successful execution
} 

# Function call with a parameter
echo "Calling the display_message function..."
display_message "Hello from the function!"

# Check the return status of the function
if [ $? -eq 0 ]; then
  echo "Function executed successfully."
else
  echo "Function execution failed."
fi
