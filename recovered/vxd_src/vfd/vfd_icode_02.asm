; vfd_icode_02.asm -- VFD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 036h, 000h
        jb     NEAR PTR loc_2_00CA
        db  0CDh, 020h, 004h, 000h, 036h, 000h
        jb     NEAR PTR loc_2_00CA
        mov    eax, 13h
        db  0BEh, 04Ch, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, 2
        db  0BEh, 0D0h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        db  0A3h, 0C4h, 001h, 000h, 000h
        push   ebx
        push   0
        push   2
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_2_005A
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_005A:
        db  0A3h, 0BCh, 001h, 000h, 000h
        pop    ebx
        or     eax, 0ffffffffh
        xor    esi, esi
        db  0BFh, 0CCh, 001h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A3h, 0C0h, 001h, 000h, 000h
        mov    eax, 6
        xor    ebx, ebx
        db  0CDh, 020h, 010h, 000h, 003h, 000h
        or     eax, 0ffffffffh
        xor    esi, esi
        db  0BFh, 0DCh, 001h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        test   eax, eax
        je     SHORT loc_2_00C9
        cmp    dword ptr [0fffe8h], 4f433330h
        jne    SHORT loc_2_00C9
        cmp    dword ptr [0fffech], 5141504dh
        jne    SHORT loc_2_00C9
        in     al, 86h
        mov    ah, al
        and    ah, 0c0h
        cmp    ah, 0c0h
        jne    SHORT loc_2_00C9
        and    al, 3fh
        or     al, 40h
        jmp    SHORT loc_2_00C0
loc_2_00C0:
        out    86h, al
        db  0C6h, 005h, 0C8h, 001h, 000h, 000h, 0FFh
loc_2_00C9:
        clc
loc_2_00CA:
        ret
        db  020h
_ITEXT ends
        end
