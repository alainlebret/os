///
/// Unix System Programming Examples / Exemplier de programmation système Unix
///
/// Copyright (C) 1995-2026 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
use nix::fcntl::{open, OFlag};
use nix::sys::stat::Mode;
use nix::unistd::{close, read, write};
use std::process::exit;

///
/// Copies the text typed on the keyboard to a file named file.out.
/// Reads input from stdin in 80-byte chunks until EOF (Ctrl+D).
///

const SIZE: usize = 80;

fn main() {
    let fd = open(
        "file.out",
        OFlag::O_CREAT | OFlag::O_WRONLY | OFlag::O_TRUNC,
        Mode::from_bits_truncate(0o644),
    )
    .unwrap_or_else(|e| {
        eprintln!("Error opening file.out: {}", e);
        exit(1);
    });

    write(1, b"Type your input (Ctrl+D to end):\n").unwrap_or_else(|e| {
        eprintln!("Error writing prompt: {}", e);
        exit(1);
    });

    let mut buffer = [0u8; SIZE];
    loop {
        match read(0, &mut buffer) {
            Ok(0) => break,
            Ok(n) => {
                write(fd, &buffer[..n]).unwrap_or_else(|e| {
                    eprintln!("Error writing to file: {}", e);
                    close(fd).ok();
                    exit(1);
                });
            }
            Err(e) => {
                eprintln!("Error reading from stdin: {}", e);
                close(fd).ok();
                exit(1);
            }
        }
    }

    close(fd).unwrap_or_else(|e| {
        eprintln!("Error closing file: {}", e);
        exit(1);
    });

    exit(0);
}
