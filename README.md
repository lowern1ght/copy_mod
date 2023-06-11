# copy_mod

- **Company** : ```Energosfera``` 👾
- **Author :** ```lowern1ght``` 🥸
- **License :** ```MIT```

## Getting started

<p>
install release <strong>binary file</strong> 
<a href="https://github.com/lowern1ght/copy_mod/releases/tag/binary">release</a> or git clone and build rep.
</p>

### Require for build

- **CMake**
- **conan** (package manager)

<p>
<code lang="bash">conan install .</code> and <code>cmake build .</code> in <strong>src</strong> directory
</p>


## Description

---

A program for copying files and folders to a specific location with logging.

- Specifications
    - ~~32~~ or 64-bit _system_
    - Uses ``openssl`` to hash files

---

## Getting Started

Arguments for start use program:

> ? - the symbol means that the argument is optional

- **--c="path/to/file/or/directory"** (**copy**) - Name entity for copy (_folder_ or _file_)
- **--p="path/to/directory"** (**point_to**) - End point to copy **entity** (_folder_)
- ?**--lg="log.txt"** - you can point out name logger file and then the logs will be written to a file with the same
  name in the current program startup folder
- ? **--lg (**logger_on**)** - don't specify a path that logger file create in the current work folder
- ? **--chck** **(check_on)** - hash check parameter after copying is completed
- ? **--rt=2gb** **(rotation_file)** - option set max size log file, clearing if file larger this value (examples):
  --rt=2gb, --rt=1mb --rt=201kb.
  additions (GB, MB, KB)

---
