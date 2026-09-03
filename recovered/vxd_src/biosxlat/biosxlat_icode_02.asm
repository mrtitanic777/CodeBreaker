; biosxlat_icode_02.asm -- BIOSXLAT object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0BEh, 022h, 000h, 000h, 000h
        mov    edx, 10h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     NEAR PTR loc_2_00F4
        xchg   edx, eax
        mov    ecx, edx
        shr    ecx, 10h
        movzx  edx, dx
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        db  0BEh, 008h, 000h, 000h, 000h
        mov    edx, 13h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     NEAR PTR loc_2_00F4
        xchg   edx, eax
        mov    ecx, edx
        shr    ecx, 10h
        movzx  edx, dx
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        db  0BEh, 0ECh, 002h, 000h, 000h
        mov    edx, 15h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     NEAR PTR loc_2_00F4
        xchg   edx, eax
        mov    ecx, edx
        shr    ecx, 10h
        movzx  edx, dx
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        db  0BEh, 00Bh, 000h, 000h, 000h
        mov    edx, 1ch
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     SHORT loc_2_00F4
        xchg   edx, eax
        mov    ecx, edx
        shr    ecx, 10h
        movzx  edx, dx
        db  066h, 089h, 00Dh, 098h, 000h, 000h, 000h
        db  066h, 089h, 015h, 09Ah, 000h, 000h, 000h
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        db  0BEh, 011h, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, dword ptr [78h]
        movzx  ecx, ax
        shr    eax, 10h
        shl    eax, 4
        add    eax, ecx
        push   0
        push   0
        push   92h
        push   100h
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        or     edx, eax
        je     SHORT loc_2_00F4
        movzx  ecx, ax
        xor    edx, edx
        mov    eax, 1eh
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        clc
        ret
loc_2_00F4:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
        add    byte ptr [eax], al
_ITEXT ends
        end
