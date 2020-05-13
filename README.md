# Life Polyglot
Implementations of Conway's Game of Life on various systems / in different
languages.

## C (terminal)
Focuses on the memory management aspects of C.
8 cells are stored to a byte, leading to a lot of bit manipulation.

## TI83+
Small implementation in Z80 assembly.
Includes Python script that generates Z80 assembly to initialize memory for:
- An initial random state. The randomness is derived from the calculator itself
  (the R register).
- A given state in `state.dat`.

![](TI83/tilife.gif)

