## Exemplier (version Rust)

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

Ce sous-dossier contient la version en Rust des exemples utilisés dans le cours de systèmes d'exploitation.

Les exemples forment un *workspace* Cargo (crate `nix` 0.30) : `cargo build` à la racine de
`exemplier-rust/` les construit tous ; `cargo run -p process_05` lance l'un d'eux (ou `cargo run`
dans son dossier). Chacun correspond au fichier C de même nom de `../exemplier/`.

| Programme | Équivalent C | Description |
|---|---|---|
| `1_process/process_01` | `process_01.c` | `fork()` : parent et enfant exécutent chacun leur partie, le parent attend avec `waitpid()` |
| `1_process/process_02` | `process_02.c` | Après `fork()`, chacun modifie sa propre copie d'une variable |
| `1_process/process_03` | `process_03.c` | Parent et enfant bloqués par `sleep()` |
| `1_process/process_04a` | `process_04a.c` | Le parent n'attend jamais : l'enfant devient zombie (état `Z` dans `ps`) ; le parent dort avec `pause()` |
| `1_process/process_04b` | `process_04b.c` | Le parent meurt avant l'enfant : l'enfant devient orphelin (`getppid()` change) |
| `1_process/process_05` | `process_05.c` | Le parent attend l'enfant et décode son état : code de sortie ou signal (`WaitStatus::Exited` / `Signaled`) |
| `1_process/process_06a` | `process_06a.c` | L'enfant est remplacé par `ls -al` (`CommandExt::exec()`) |
| `1_process/process_06b` | `process_06b.c` | L'enfant est remplacé par `gnuplot`, qui trace `command.gp` (lancer dans ce dossier) |
| `1_process/process_07` | `process_07.c` | Groupes de processus : `setpgid(0, 0)` dans l'enfant et le parent, puis `kill(-pgid, SIGTERM)` |
| `3_files/file_copy` | `file_copy.c` | Copie le clavier dans `file.out` avec `read()`/`write()`, écritures partielles traitées |
| `3_files/fs_block` | `fs_block.c` | `statvfs()` : taille de bloc et nombre de blocs, comptés en `f_frsize` |

L'utilisation de `fork()` est considérée comme dangereuse en Rust. Chaque programme inclut un commentaire (`// SAFETY:`) expliquant pourquoi l'appel est sûr dans ce contexte.

Ces exemples de code sont fournis sous la licence [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0).

----

This repository contains system programming examples written in Rust for use in the course.

The examples are organized as a Cargo workspace (`nix` 0.30). Run `cargo build` to build all examples. Use `cargo run -p <name>` (for example, `cargo run -p process_05`) to run a specific example. Each example corresponds to a C file with the same name in `../exemplier/`.

These code examples are provided under the [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0) License.

