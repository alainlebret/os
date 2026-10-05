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

//! A simple program that clones a process using the fork() primitive, and
//! where the parent process dies before its child. The child process becomes
//! an orphan: its new parent is init (or a subreaper), see `getppid()`.

use nix::unistd::{fork, getppid, sleep, ForkResult};
use std::process::{self, exit};

/// Manages the child process: it is blocked during `duration` seconds.
fn manage_child(duration: u32) {
    println!(
        "Child process (PID n° {}), parent {}",
        process::id(),
        getppid()
    );
    println!("Child will be blocked during {} seconds...", duration);
    sleep(duration);
    println!(
        "Child has finished to sleep; its parent is now {}.",
        getppid()
    );
}

/// Manages the parent process: it does not wait for its child and dies.
fn manage_parent() {
    println!("Parent process (PID n° {})", process::id());
    println!("Dying...");
}

fn main() {
    let duration = 20;

    // SAFETY: fork() is unsafe in Rust because, in a multi-threaded program,
    // the child only gets a copy of the calling thread (a lock held by another
    // thread would stay locked forever). This program has a single thread.
    match unsafe { fork() } {
        Ok(ForkResult::Child) => manage_child(duration),
        Ok(ForkResult::Parent { child: _ }) => manage_parent(),
        Err(err) => {
            eprintln!("Error [fork()]: {}", err);
            exit(1);
        }
    }
}
