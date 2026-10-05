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

//! Uses statvfs() to display the filesystem statistics (block size, total,
//! free and available blocks) for the path given as a command-line argument.
//!
//! The block counts are expressed in units of the fragment size (f_frsize),
//! which may differ from the block size (f_bsize): on macOS, f_bsize is 1 MiB
//! and f_frsize 4 KiB.

use nix::sys::statvfs::statvfs;
use std::env;
use std::process::exit;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        eprintln!(
            "Usage: {} <path to any directory or file on the target filesystem>",
            args[0]
        );
        exit(1);
    }

    let stat = statvfs(args[1].as_str()).unwrap_or_else(|err| {
        eprintln!("Error [statvfs()]: {}", err);
        exit(1);
    });

    let unit = stat.fragment_size() as u64; // unit of the counts below
    let gib = |blocks: u64| blocks * unit / (1024 * 1024 * 1024);

    println!(
        "Filesystem block size (f_bsize): {} bytes",
        stat.block_size()
    );
    println!(
        "Fragment size (f_frsize, unit of the counts): {} bytes",
        unit
    );
    println!(
        "Total blocks: {} ({} GiB)",
        stat.blocks(),
        gib(stat.blocks() as u64)
    );
    println!(
        "Free blocks: {} ({} GiB)",
        stat.blocks_free(),
        gib(stat.blocks_free() as u64)
    );
    println!(
        "Available blocks (non-superuser): {} ({} GiB)",
        stat.blocks_available(),
        gib(stat.blocks_available() as u64)
    );
}
