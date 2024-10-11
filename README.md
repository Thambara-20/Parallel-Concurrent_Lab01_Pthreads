# Lab01 - Concurrent Programming

## Overview

This project demonstrates linked list operations using three approaches:
1. **Serial Execution** (sequential).
2. **Parallel with Mutex** (mutual exclusion).
3. **Parallel with Read-Write Locks** (concurrent reads).

## Structure

Each folder contains:
- `CMakeLists.txt` for configuration.
- `main.c` for source code.

## Build & Run

1. Navigate to a module:
   ```bash
   cd serial  # Or parallel_mutex, parallel_read_write_lock
   mkdir build && cd build
   cmake ..
   make
   ./executable_name  # Replace with actual name ex: ./serial
2. Parameters
   N, M: Node count and total operations.
   M_MEMBER, M_INSERT, M_DELETE: Operation fractions.
   NUMBER_OF_THREADS: Thread count for parallel runs.


