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
extern crate nix;

use nix::unistd::{fork, ForkResult};
use std::process;
use std::process::exit;

///
/// A simple program that clones a process using the fork() primitive.
///

///
/// Manages the child process.
///
fn manage_child() {
    println!("Child process (PID n° {})", process::id());
    println!("Instructions of child process...");
}

///
/// Manages the parent process.
///
fn manage_parent() {
    println!("Parent process (PID n° {})", process::id());
    println!("Instructions of parent process...");
}

fn main() {
    match unsafe { fork() } {
        Ok(ForkResult::Child) => {
            manage_child();
        }

        Ok(ForkResult::Parent { child }) => {
            manage_parent();
        }

        Err(err) => {
            panic!("Error [fork()]: {}", err);
        }
    };
    exit(0);
}
