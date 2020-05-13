; Relevant pieces of ti83plus.inc
; ===============================
#define b_call(xxxx)     RST  28h \ .DW xxxx
#define skClear          $0F        ; Clear key code
#define tAsmCmp          $6D        ; Assember directive
#define t2ByteTok        $BB        ; Assember directive
#define _GetCSC          $4018      ; Get keypresses
#define _RunIndicOff     $4570
#define _GrBufCpy        $486A      ; Copy buffer to screen
#define _ForceFullScreen $508F
#define tempSwapArea     $82A5
#define plotSScreen      $9340
#define tempMem          $9820
#define ProgStart        $9D93

; Constants
; =========
#define XMAX             96
#define YMAX             64
#define PIXELS           XMAX*YMAX
#define PIXELS_LOOP      PIXELS/256
#define PADDING          XMAX+1


    .ORG    ProgStart
    .DB     t2ByteTok, tAsmCmp      ; Magic constant of TI-83+ Z80 assembly

; Load temporary memory
    LD      HL, (tempMem)
    LD      BC, PADDING
    ADD     HL, BC
    LD      (tempSwapArea), HL

; Setup the screen
    b_call(_ForceFullScreen)
    b_call(_RunIndicOff)

; State initialization
#include "state.asm"


main:

; Counting neighbors
    LD      HL, (tempSwapArea)
    LD      DE, XMAX
    LD      BC, PIXELS_LOOP
count_loop:
    LD      A, (HL)
    INC     HL
    AND     $80
    JR      Z, count_done
    INC     (HL)                ; Right neighbor
    SBC     HL, DE
    INC     (HL)                ; Top-Right neighbor
    DEC     HL
    INC     (HL)                ; Top neighbor
    DEC     HL
    INC     (HL)                ; Top-Left neighbor
    ADD     HL, DE
    INC     (HL)                ; Left neighbor
    ADD     HL, DE
    INC     (HL)                ; Bottom-Left neighbor
    INC     HL
    INC     (HL)                ; Bottom neighbor
    INC     HL
    INC     (HL)                ; Bottom-Right neighbor
    SBC     HL, DE
count_done:
    DJNZ    count_loop
    DEC     C
    JR      NZ, count_loop


; Generation
    LD      HL, (tempSwapArea)
    LD      C, PIXELS_LOOP
generation_loop:
    LD      A, (HL)
    RLCA
    SUB     %101
    CP      %111-%101+%001      ; :0 magic
    LD      A, D
    RRA
    LD      (HL), A
    INC     HL
    DJNZ    generation_loop
    DEC     C
    JR      NZ, generation_loop


; Display game state
    LD      HL, (tempSwapArea)
    LD      DE, plotSScreen
    LD      C, PIXELS_LOOP/8
display_loop:
    LD      A, (HL)             ; A.0 = *(tempSwapArea+8ii)
    INC     HL
    RLCA
    OR      (HL)                ; A.1 |= *(tempSwapArea+8ii+1)
    INC     HL
    RLCA
    OR      (HL)                ; A.2 |= *(tmepSwapArea+8ii+2)
    INC     HL
    RLCA
    OR      (HL)                ; ...
    INC     HL
    RLCA
    OR      (HL)
    INC     HL
    RLCA
    OR      (HL)
    INC     HL
    RLCA
    OR      (HL)
    INC     HL
    RLCA
    OR      (HL)
    INC     HL
    RLCA
    LD      (DE), A             ; *(plotSScreen+ii) = A
    INC     DE
    DJNZ    display_loop
    DEC     C
    JR      NZ, display_loop
    b_call(_GrBufCpy)


; Return to main if CLEAR not pressed
    b_call(_GetCSC)
    CP   skClear
    JR   NZ, main

    RET

    .END
