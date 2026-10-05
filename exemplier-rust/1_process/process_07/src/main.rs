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

//! A simple program about a process and its group.
//!
//! The child leaves the group of its parent with setpgid(0, 0): it becomes the
//! leader of a new group whose PGID is its PID. The parent makes the same call
//! (setpgid(pid, pid)), so that the group exists whoever runs first, then sends
//! SIGTERM to the whole group with kill(-pgid, SIGTERM).
//!
//! Course "Operating Systems", chapter « Signaux », slide « Groupes de processus ».

use nix::sys::signal::{kill, Signal};
use nix::sys::wait::{waitpid, WaitStatus};
use nix::unistd::{fork, getpgrp, getpid, pause, setpgid, sleep, ForkResult, Pid};
use std::io::{self, Write};
use std::process::{self, exit};

/// Manages the child process: it creates its own group, then waits.
fn manage_child() -> ! {
    println!(
        "Child process: PID={}, Group ID={}",
        process::id(),
        getpgrp()
    );

    // New group, led by the child: PGID = PID of the child
    if let Err(err) = setpgid(Pid::from_raw(0), Pid::from_raw(0)) {
        eprintln!("Error [setpgid()] (child): {}", err);
        exit(1);
    }
    println!(
        "Child process: PID={}, new Group ID={}",
        process::id(),
        getpgrp()
    );
    io::stdout().flush().ok(); // killed by SIGTERM: nothing must stay in a buffer

    loop {
        pause(); // waits for the SIGTERM of its parent
    }
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

/// Manages the parent process: displays its group, sends SIGTERM to the group
/// of the child, then waits for it.
fn manage_parent(child: Pid) {
    println!(
        "Parent process: PID={}, Group ID={}",
        process::id(),
        getpgrp()
    );

    // Same call as in the child: no race on who runs first
    if let Err(err) = setpgid(child, child) {
        eprintln!("Error [setpgid()] (parent): {}", err);
    }
    sleep(1); // let the child display its new group

    println!("Parent: SIGTERM to the group {}", child);
    // -pgid: the whole group (killpg(pgid, SIGTERM) does the same)
    if let Err(err) = kill(Pid::from_raw(-child.as_raw()), Signal::SIGTERM) {
        eprintln!("Error [kill()]: {}", err);
        exit(1);
    }

    match waitpid(child, None) {
        Ok(status) => report(status),
        Err(err) => {
            eprintln!("Error [waitpid()]: {}", err);
            exit(1);
        }
    }
}

fn main() {
    // SAFETY: fork() is unsafe in Rust because, in a multi-threaded program,
    // the child only gets a copy of the calling thread (a lock held by another
    // thread would stay locked forever). This program has a single thread.
    match unsafe { fork() } {
        Ok(ForkResult::Child) => manage_child(),
        Ok(ForkResult::Parent { child }) => manage_parent(child),
        Err(err) => {
            eprintln!("Error [fork()]: {}", err);
            exit(1);
        }
    }
}
