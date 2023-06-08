# copy_mod 

- **Company** : ```Energosfera``` 👾
- **Author :** ```lowern1ght``` 🥸
- **License :** ```MIT```

## Description

---

A program for copying files and folders to a specific location with logging.

- Specifications
  - 32 or 64-bit _system_
  - Uses ``openssl`` to hash files

---
## Getting Started

Arguments for start use program:

> ? - the symbol means that the argument is optional

- **--c="path/to/file/or/directory"** (**copy**) - Name entity for copy (_folder_ or _file_)
- **--p="path/to/directory"** (**point_to**) - End point to copy **entity** (_folder_)
- ?**--lg="log.txt"** - you can point out name logger file and then the logs will be written to a file with the same name in the current program startup folder
- ? **--lg (**logger_on**)** - don't specify a path that logger file create in the current work folder
- ? **--chck** **(check_on)** - hash check parameter after copying is completed

---