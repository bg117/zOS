bits 16

section _ENTRY class=CODE

global _start
extern _cstart_

_start: xor     dh, dh
        push    dx          ; push drive number to stack
        push    sp          ; push address of drive number to stack

        call    _cstart_    ; pass the address to main
