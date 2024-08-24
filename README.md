#Assembler Project
Final project for the course "Laboratory in C - 20465 (2024B)" at The Open University

Overview
This C-based assembler converts assembly-like language into octal machine code. It operates in three stages: macro processing, first pass for symbol detection and initial encoding, and second pass for final encoding.

Features
Macro Processing: Expands macros in the source code.
Two-Pass Assembly: Resolves symbols and addresses over two passes.
Output Files: Generates .am, .ob, .ent, and .ext files.
Usage
Compile: gcc -o assembler assembler.c
Run: ./assembler filename.as
Output Files
filename.am: Source code after macro expansion.
filename.ob: Octal machine code with memory addresses.
filename.ent: Addresses of .entry symbols.
filename.ext: Addresses of .extern symbols.
Environment
OS: Linux (Ubuntu)
Compiler: GCC
