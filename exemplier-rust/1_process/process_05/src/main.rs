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
use nix::sys::wait::{wait, WaitStatus};
use nix::unistd::{fork, getpid, getppid, sleep, ForkResult};
use std::process;
use std::process::exit;

///
/// A simple program that clones a process using the fork() primitive. The
/// parent is waiting for his child to finish.
///

///
/// Manages the child process. The child process is blocked during "duration"
/// seconds.
///
fn manage_child(duration: u32) {
    println!("Child process (PID n° {})", process::id());
    println!("Child will be blocked during {} seconds...", duration);
    sleep(duration);
    println!("Child has finished to sleep.");
    println!("The PID of my parent is {}.", getppid());
}

///
/// Manages the parent process. The parent is waiting for his child to exit.
///
fn manage_parent() {
    println!(
        "Parent process (PID n° {}) is waiting for the child to finish.",
        process::id()
    );
}

fn main() {
    let duration = 20;

    match unsafe { fork() } {
        Ok(ForkResult::Child) => {
            manage_child(duration);
        }

        Ok(ForkResult::Parent { child: _ }) => {
            manage_parent();
            match wait() {
                Ok(WaitStatus::Exited(child_pid, code)) => {
                    println!(
                        "{} : child {} has finished his work (code: {})",
                        getpid(),
                        child_pid,
                        code
                    );
                }
                Ok(_) => {}
                Err(err) => panic!("Error [wait()]: {}", err),
            }
        }

        Err(err) => {
            panic!("Error [fork()]: {}", err);
        }
    };

    exit(0);
}
