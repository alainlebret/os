///
/// Unix System Programming Examples / Exemplier de programmation système Unix
///
/// Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///

use std::fs;
use std::process::exit;

///
/// Copy from keyboard to a file.
///

fn main() {
    let mut buffer: String;

    buffer = String::new();
    std::io::stdin()
        .read_line(&mut buffer)
        .expect("Unable to read from standard input!");
    fs::write("file.out", buffer).expect("Unable to open file!");

    exit(0);
}
