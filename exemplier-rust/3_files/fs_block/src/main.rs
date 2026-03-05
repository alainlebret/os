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
use nix::sys::statvfs::statvfs;
use std::env;
use std::process::exit;

///
/// Uses statvfs() to display the filesystem statistics (block size, total,
/// free and available blocks) for the path given as a command-line argument.
///

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        eprintln!(
            "Usage: {} <path to any directory or file on the target filesystem>",
            args[0]
        );
        exit(1);
    }

    let stat = statvfs(args[1].as_str()).unwrap_or_else(|e| {
        eprintln!("Error retrieving filesystem statistics: {}", e);
        exit(1);
    });

    println!("Filesystem block size: {} bytes", stat.block_size());
    println!("Total blocks: {}", stat.blocks());
    println!("Free blocks: {}", stat.blocks_free());
    println!("Available blocks (non-superuser): {}", stat.blocks_available());

    exit(0);
}
