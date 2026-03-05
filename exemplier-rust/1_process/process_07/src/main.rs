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
use libc::getpgrp;
use nix::sys::wait::{wait, WaitStatus};
use nix::unistd::{fork, getpid, ForkResult};
use std::process;
use std::process::exit;

///
/// A simple program about a process and its group.
///

///
/// Manages the child process by displaying its PID and group ID.
///
fn manage_child() {
    unsafe {
        println!(
            "Child process: PID={}, Group ID={}",
            process::id(),
            getpgrp()
        );
    }
    exit(0);
}

///
/// Manages the parent process. The parent displays its group ID then waits
/// for the child to exit.
///
fn manage_parent() {
    unsafe {
        println!(
            "Parent process: PID={}, Group ID={}",
            process::id(),
            getpgrp()
        );
    }
}

fn main() {
    match unsafe { fork() } {
        Ok(ForkResult::Child) => {
            manage_child();
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
