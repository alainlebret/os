/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

use nix::sys::ptrace;
use nix::sys::signal::Signal;
use nix::sys::wait::{WaitStatus, wait};
use nix::unistd::{fork, ForkResult, Pid};

use std::collections::HashMap;
use std::ffi::c_void;
use std::os::unix::process::CommandExt;
use std::process::{Command, exit};

extern crate linux_personality;
use linux_personality::personality;


fn set_breakpoint(pid: Pid, addr: u64) -> i64 {
    // Read 8 bytes from the process memory
    let value = ptrace::read(pid, addr as *mut c_void).unwrap();

    // Insert breakpoint by write new values
    let bp = (value & (i64::MAX ^ 0xFF)) | 0xCC;

    unsafe {
        ptrace::write(pid, addr as *mut c_void, bp as *mut c_void).unwrap();
    }

    // Return original bytecode
    value
}

fn restore_breakpoint(pid: Pid, addr: u64, orig_value: i64) {
    unsafe {
        // Restore original bytecode
        ptrace::write(pid, addr as *mut c_void, orig_value as *mut c_void).unwrap();
    }
}

fn handle_sigstop(pid: Pid, saved_values: &HashMap<u64, i64>) {
    let mut regs = ptrace::getregs(pid).unwrap();
    println!("Hit breakpoint at 0x{:x}", regs.rip - 1);

    match saved_values.get(&(regs.rip - 1)) {
        Some(orig) => {
            restore_breakpoint(pid, regs.rip - 1, *orig);

            // rewind rip
            regs.rip -= 1;
            ptrace::setregs(pid, regs).expect("Error rewinding RIP");

        }
        _ => print!("Nothing saved here"),
    }

    ptrace::cont(pid, None).expect("Restoring breakpoint failed");

}

// Code that runs only for child
fn manage_child() {
    // Allows process to be traced
    ptrace::traceme().unwrap();

    // Disable ASLR for this process
    personality(linux_personality::ADDR_NO_RANDOMIZE).unwrap();

    // Execute binary replacing
    Command::new("/home/carstein/sample").exec();

    exit(0);
}

// Code that runs only for parent
fn manage_parent(pid: Pid, breakpoints: &[u64]) {
    let mut saved_values = HashMap::new();

    // Placing breakpoints
    wait().unwrap();
    for addr in breakpoints.iter() {
        let orig = set_breakpoint(pid, *addr);
        saved_values.insert(*addr, orig);
    }
    ptrace::cont(pid, None).expect("Failed continue process");

    loop {
        match wait() {
            Ok(WaitStatus::Stopped(pid_t, sig_num)) => {
                match sig_num {
                    Signal::SIGTRAP => {
                        handle_sigstop(pid_t, &saved_values);
                    }

                    Signal::SIGSEGV => {
                        let regs = ptrace::getregs(pid_t).unwrap();
                        println!("Segmentation fault at 0x{:x}", regs.rip);
                        break
                    }
                    _ => {
                        println!("Some other signal - {}", sig_num);
                        break
                    }
                }
            },

            Ok(WaitStatus::Exited(pid, exit_status)) => {
                println!("Process with pid: {} exited with status {}", pid, exit_status);
                break;
            },

            Ok(status) =>  {
                println!("Received status: {:?}", status);
                ptrace::cont(pid, None).expect("Failed to deliver signal");
            },

            Err(err) => {
                println!("Some kind of error - {:?}",err);

            },
        }
    }
}

fn main() {
    // set breakpoints hash map
    let breakpoints: [u64; 1] = [0x8048451];

    match unsafe{fork()} {

        Ok(ForkResult::Child) => {
            manage_child();
        }

        Ok(ForkResult::Parent {child}) => {
            manage_parent(child, &breakpoints);
        }

        Err(err) => {
            panic!("[main] fork() failed: {}", err);
        }
    };
}