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

//! A simple program that clones a process using the fork() primitive and
//! shows the evolution of a variable in parent and child processes: each one
//! modifies its own copy.

use nix::sys::wait::waitpid;
use nix::unistd::{fork, ForkResult};
use std::process::{self, exit};

/// Manages the child process.
fn manage_child(parameter: &mut i32) {
    println!("Child process (PID n° {})", process::id());
    println!("Child modifies its own variable...");
    *parameter = 20;
}

/// Manages the parent process.
fn manage_parent(parameter: &mut i32) {
    println!("Parent process (PID n° {})", process::id());
    println!("Parent modifies its own variable...");
    *parameter = 10;
}

fn main() {
    let mut own_variable = 0;

    // SAFETY: fork() is unsafe in Rust because, in a multi-threaded program,
    // the child only gets a copy of the calling thread (a lock held by another
    // thread would stay locked forever). This program has a single thread.
    match unsafe { fork() } {
        Ok(ForkResult::Child) => {
            manage_child(&mut own_variable);
            println!(
                "PID n° {} has its own variable equal to: {}",
                process::id(),
                own_variable
            );
        }
        Ok(ForkResult::Parent { child }) => {
            manage_parent(&mut own_variable);
            println!(
                "PID n° {} has its own variable equal to: {}",
                process::id(),
                own_variable
            );
            if let Err(err) = waitpid(child, None) {
                eprintln!("Error [waitpid()]: {}", err);
                exit(1);
            }
        }
        Err(err) => {
            eprintln!("Error [fork()]: {}", err);
            exit(1);
        }
    }
}
