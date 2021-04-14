    LD      IX, (tempSwapArea)
    LD      BC, $18
rand_loop:
    LD      A, R
    LD      D, A
    LD      E, (HL)
    ADD     HL, DE
    LD      E, A
    ADD     HL, DE
    LD      A, H
    XOR     L
    AND     $80
    LD      (IX+0), A
    INC     IX
    DJNZ    rand_loop
    DEC     C
    JR      NZ, rand_loop
