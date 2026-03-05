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
use nix::unistd::{fork, getpid, ForkResult};
use std::os::unix::process::CommandExt;
use std::process;
use std::process::{exit, Command};

///
/// Another program that clones a process using the fork() primitive. The
/// child is replaced by a new program using the exec() function.
///

///
/// Manages the child process. The child process is calling the exec()
/// function to execute the "gnuplot" command.
///
fn manage_child() {
    println!("Child process (PID n° {})", process::id());
    println!("Child is going to be replaced by Gnuplot program.");
    let err = Command::new("gnuplot")
        .args(["-persist", "resources/command.gp"])
        .exec();
    eprintln!("Failed to run Gnuplot: {}", err);
    exit(1);
}

///
/// Manages the parent process. The parent is waiting for his child to exit.
///
fn manage_parent() {
    println!(
        "Parent process (PID n° {}) waiting for the child.",
        process::id()
    );
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
