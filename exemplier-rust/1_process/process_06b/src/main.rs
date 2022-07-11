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
use libc::{WEXITSTATUS, WIFEXITED};
use nix::sys::wait::wait;
use nix::unistd::{fork, getpid, getppid, sleep, ForkResult};
use std::os::unix::process::CommandExt;
use std::process;
use std::process::{exit, Command};

///
/// Another program that clones a process using the fork() primitive. The
/// child is overlayed by a new program using the exec() functions.
///

///
/// Manages the child process. The child process is calling the exec()
/// function to execute the "gnuplot" command.
///
fn manage_child() {
    println!("Child process (PID n° {})", process::id());
    println!("Child is going to be replaced by Gnuplot program.");
    Command::new("gnuplot")
        .args(["-persist", "command.gp"])
        .exec();
    exit(0);
}

///
/// Manages the parent process. The parent is waiting for his child to exit.
///
fn manage_parent() {
    println!("Parent process (PID n° {})", process::id());
}

fn main() {
    let status = 0;

    match unsafe { fork() } {
        Ok(ForkResult::Child) => {
            manage_child();
        }

        Ok(ForkResult::Parent { child }) => {
            manage_parent();
            wait().expect("Unable to wait for my child to end!");
            if (WIFEXITED(status)) {
                println!(
                    "{} : child {} has finished his work (code: {} )",
                    getpid(),
                    child,
                    WEXITSTATUS(status)
                );
            }
            exit(0);
        }

        Err(err) => {
            panic!("Error [fork()]: {}", err);
        }
    };

    exit(0);
}
