; msmouse_icode_02.asm -- MSMOUSE object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0CDh, 020h, 000h, 000h, 00Ch, 000h
        cmp    eax, 400h
        jb     NEAR PTR loc_2_0131
        db  089h, 01Dh, 0C4h, 004h, 000h, 000h
        db  0BEh, 075h, 002h, 000h, 000h
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        jb     SHORT loc_2_002C
        db  0FFh, 005h, 07Dh, 002h, 000h, 000h
        jmp    SHORT loc_2_0039
loc_2_002C:
        db  0BEh, 060h, 002h, 000h, 000h
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        jb     SHORT loc_2_004E
loc_2_0039:
        mov    esi, edx
        db  0BFh, 0D0h, 001h, 000h, 000h
        movzx  ecx, byte ptr [esi]
        rep movsb
        mov    eax, 0
        mov    dword ptr [edi], eax
        jmp    SHORT loc_2_0077
loc_2_004E:
        db  0BEh, 066h, 002h, 000h, 000h
        db  0BFh, 06Ch, 002h, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        or     edx, edx
        je     SHORT loc_2_0077
        mov    esi, edx
        db  0BFh, 0D0h, 001h, 000h, 000h
        movzx  ecx, byte ptr [esi]
        rep movsb
        mov    eax, 0
        mov    dword ptr [edi], eax
loc_2_0077:
        push   0
        push   1ch
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    SHORT loc_2_008D
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_008D:
        db  0A3h, 0D0h, 004h, 000h, 000h
        push   100h
        push   1e4h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    SHORT loc_2_00AE
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_00AE:
        db  0A3h, 0D4h, 004h, 000h, 000h
        mov    eax, 33h
        db  0BEh, 020h, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 0C8h, 004h, 000h, 000h
        db  089h, 015h, 0CCh, 004h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 10h
        mov    eax, 33h
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        mov    eax, 0c0008h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0D8h, 004h, 000h, 000h
        mov    eax, 0ch
        db  0BEh, 09Ch, 000h, 000h, 000h
        db  0CDh, 020h, 092h, 000h, 001h, 000h
        db  089h, 035h, 0DCh, 004h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 00Ch, 000h
        or     cx, cx
        je     SHORT loc_2_0130
        db  0FEh, 005h, 000h, 000h, 000h, 000h
loc_2_0130:
        clc
loc_2_0131:
        ret
        db  000h, 000h
_ITEXT ends
        end
