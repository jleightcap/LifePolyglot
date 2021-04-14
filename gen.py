#!/usr/bin/env python
import argparse
import sys

# Z80 asm to generate random initial state
# Source:
# https://wikiti.brandonw.net/index.php?title=Z80_Routines:Math:Random
rand_asm = [
    "    LD      IX, (tempSwapArea)\n",
    "    LD      BC, $18\n",
    "rand_loop:\n",
    "    LD      A, R\n",
    "    LD      D, A\n",
    "    LD      E, (HL)\n",
    "    ADD     HL, DE\n",
    "    LD      E, A\n",
    "    ADD     HL, DE\n",
    "    LD      A, H\n",
    "    XOR     L\n",
    "    AND     $80\n",
    "    LD      (IX+0), A\n",
    "    INC     IX\n",
    "    DJNZ    rand_loop\n",
    "    DEC     C\n",
    "    JR      NZ, rand_loop\n"
]

# Z80 asm for generated initial constants
gen_const = [
    "    LD      IX, (tempSwapArea)\n",
    "    LD      DE, XMAX\n",
    "    LD      A, $80\n"
]

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate state.asm from given initial state or a random state")
    parser.add_argument("--state", help="Plaintext initial state")
    parser.add_argument("--rand", help="Generate random initial state", action="store_true")
    args = parser.parse_args()
    fp = open("state.asm", "w")

    if args.rand:
        fp.writelines(rand_asm)
        sys.stdout.write("Generated assmembly for random state. Run `make` to assemble .8xp machine code.\n")
        fp.close()
        exit()

    state_dat = open(args.state, "r")
    lines = state_dat.readlines()
    fp.writelines(gen_const)    # Set constants
    last_row = 0                # Last row with an alive cell
    for row in range(len(lines)):
        row_has_alive = 0       # Does this row contain an alive cell?
        for col in range(len(lines[row])):
            if lines[row][col] == "1":
                # First alive cell in row, add row offset to IX
                if row_has_alive == 0:
                    fp.write((row-last_row)*"    ADD     IX, DE\n")
                    last_row = row
                fp.write("    LD      (IX+" + str(col) + "), A\n")
                row_has_alive = 1
    sys.stdout.write("Generated assembly for given state.dat. Run `make` to assemble .8xp machine code.\n")

    state_dat.close()
    fp.close()
