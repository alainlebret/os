use libc::{WEXITSTATUS, WIFEXITED};
use nix::sys::wait::wait;
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
    println!("Parent process (PID n° {})", process::id());
}

fn main() {
    let duration = 20;
    let status = 0;

    match unsafe { fork() } {
        Ok(ForkResult::Child) => {
            manage_child(duration);
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
