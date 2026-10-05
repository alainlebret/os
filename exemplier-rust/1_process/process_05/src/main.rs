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

//! A simple program that clones a process using the fork() primitive. The
//! parent waits for its child to finish and decodes its status.

use nix::sys::wait::{waitpid, WaitStatus};
use nix::unistd::{fork, getpid, getppid, sleep, ForkResult};
use std::process::{self, exit};

/// Manages the child process: it is blocked during `duration` seconds.
fn manage_child(duration: u32) {
    println!("Child process (PID n° {})", process::id());
    println!("Child will be blocked during {} seconds...", duration);
    sleep(duration);
    println!("Child has finished to sleep.");
    println!("The PID of my parent is {}.", getppid());
}

/// Manages the parent process before it waits for its child.
fn manage_parent() {
    println!(
        "Parent process (PID n° {}) is waiting for the child to finish.",
        process::id()
    );
}

/// Displays how the child ended: exit code (WIFEXITED) or signal (WIFSIGNALED).
fn report(status: WaitStatus) {
    match status {
        WaitStatus::Exited(child, code) => {
            println!(
                "{} : child {} has finished (exit code: {})",
                getpid(),
                child,
                code
            );
        }
        WaitStatus::Signaled(child, signal, _core_dumped) => {
            println!(
                "{} : child {} was killed by the signal {:?}",
                getpid(),
                child,
                signal
            );
        }
        other => println!("{} : other status {:?}", getpid(), other),
    }
}

fn main() {
    let duration = 20;

    // SAFETY: fork() is unsafe in Rust because, in a multi-threaded program,
    // the child only gets a copy of the calling thread (a lock held by another
    // thread would stay locked forever). This program has a single thread.
    match unsafe { fork() } {
        Ok(ForkResult::Child) => manage_child(duration),
        Ok(ForkResult::Parent { child }) => {
            manage_parent();
            match waitpid(child, None) {
                Ok(status) => report(status),
                Err(err) => {
                    eprintln!("Error [waitpid()]: {}", err);
                    exit(1);
                }
            }
        }
        Err(err) => {
            eprintln!("Error [fork()]: {}", err);
            exit(1);
        }
    }
}
