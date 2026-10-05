// Unix System Programming Examples / Exemplier de programmation système Unix
//
// Copyright (C) 1995-2026 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//! Copies the text typed on the keyboard to a file named file.out.
//! Reads input from stdin in 80-byte chunks until EOF (Ctrl+D).

use nix::fcntl::{open, OFlag};
use nix::sys::stat::Mode;
use nix::unistd::{read, write};
use std::io;
use std::os::fd::{AsFd, BorrowedFd};
use std::process::exit;

const SIZE: usize = 80;

/// Writes the whole buffer: write() may write fewer bytes than asked (partial
/// write), so it is called again on the rest (`ecrire_n()` in the C course).
fn write_all(fd: BorrowedFd, mut buffer: &[u8]) -> nix::Result<()> {
    while !buffer.is_empty() {
        let written = write(fd, buffer)?;
        buffer = &buffer[written..];
    }
    Ok(())
}

fn main() {
    // The OwnedFd returned by open() closes the file when it goes out of scope
    let file = open(
        "file.out",
        OFlag::O_CREAT | OFlag::O_WRONLY | OFlag::O_TRUNC,
        Mode::from_bits_truncate(0o644),
    )
    .unwrap_or_else(|err| {
        eprintln!("Error [open()] file.out: {}", err);
        exit(1);
    });

    let stdin = io::stdin();
    let stdout = io::stdout();
    if let Err(err) = write_all(stdout.as_fd(), b"Type your input (Ctrl+D to end):\n") {
        eprintln!("Error [write()] prompt: {}", err);
        exit(1);
    }

    let mut buffer = [0u8; SIZE];
    loop {
        match read(stdin.as_fd(), &mut buffer) {
            Ok(0) => break, // EOF
            Ok(n) => {
                if let Err(err) = write_all(file.as_fd(), &buffer[..n]) {
                    eprintln!("Error [write()] file.out: {}", err);
                    exit(1);
                }
            }
            Err(err) => {
                eprintln!("Error [read()] stdin: {}", err);
                exit(1);
            }
        }
    }
    // `file` is closed here (drop of the OwnedFd)
}
