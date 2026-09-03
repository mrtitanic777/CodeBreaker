; mgamil_pcode_03.asm -- MGAMIL object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        pushad
        mov    eax, dword ptr [ebp + 18h]
        cmp    eax, 0
        je     NEAR PTR loc_3_0261
        cmp    eax, 0ac53962bh
        je     SHORT loc_3_0036
        cmp    eax, 0ac53962ch
        jne    SHORT loc_3_0025
        db  0E8h, 050h, 006h, 0FDh, 0FFh
        jmp    NEAR PTR loc_3_0287
loc_3_0025:
        cmp    eax, 0ac53962dh
        jne    SHORT loc_3_0036
        db  0E8h, 012h, 007h, 0FDh, 0FFh
        jmp    NEAR PTR loc_3_0287
loc_3_0036:
        movzx  eax, word ptr [ebp + 40h]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        je     NEAR PTR loc_3_0279
        dec    eax
        movzx  esi, word ptr [ebp + 4]
        add    esi, eax
        db  0A1h, 010h, 008h, 000h, 000h
        or     eax, eax
        jne    NEAR PTR loc_3_012F
        mov    ax, word ptr [ebp + 3ch]
        shl    eax, 10h
        mov    ax, word ptr [ebp]
        db  0A3h, 01Ch, 008h, 000h, 000h
        mov    ax, word ptr [esi + 10h]
        cmp    ax, 40h
        jle    SHORT loc_3_0080
        mov    ax, 40h
loc_3_0080:
        db  066h, 0A3h, 056h, 008h, 000h, 000h
        mov    al, byte ptr [esi + 12h]
        or     al, al
        jne    SHORT loc_3_0097
        db  081h, 00Dh, 00Ch, 008h, 000h, 000h, 000h, 040h, 000h, 000h
loc_3_0097:
        push   esi
        add    esi, 16h
        mov    al, byte ptr [esi + 4ch]
        and    eax, 0ffh
        db  009h, 005h, 00Ch, 008h, 000h, 000h
        pop    esi
        mov    eax, dword ptr [esi + 6]
        db  0A3h, 014h, 008h, 000h, 000h
        db  066h, 083h, 03Dh, 018h, 008h, 000h, 000h, 000h
        jne    SHORT loc_3_00FD
        push   edx
        mov    dx, 0cf8h
        mov    al, 80h
        out    dx, al
        jmp    SHORT loc_3_00C6
loc_3_00C6:
        mov    dx, 0bf00h
loc_3_00CA:
        add    dx, 100h
        cmp    dx, 0d000h
        je     SHORT loc_3_00F3
        in     al, dx
        jmp    SHORT loc_3_00D9
loc_3_00D9:
        cmp    al, 2bh
        jne    SHORT loc_3_00CA
        inc    dx
        in     al, dx
        jmp    SHORT loc_3_00E2
loc_3_00E2:
        dec    dx
        cmp    al, 10h
        jne    SHORT loc_3_00CA
        add    dx, 41h
        db  066h, 089h, 015h, 018h, 008h, 000h, 000h
loc_3_00F3:
        mov    dx, 0cf8h
        mov    al, 0
        out    dx, al
        jmp    SHORT loc_3_00FC
loc_3_00FC:
        pop    edx
loc_3_00FD:
        mov    eax, dword ptr [esi + 2]
        cmp    eax, 0ac000h
        jne    SHORT loc_3_0111
        db  081h, 00Dh, 00Ch, 008h, 000h, 000h, 000h, 001h, 000h, 000h
loc_3_0111:
        push   0
        push   4000h
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        je     NEAR PTR loc_3_0279
        dec    eax
        db  0A3h, 010h, 008h, 000h, 000h
loc_3_012F:
        mov    eax, dword ptr [esi + 1ch]
        db  03Bh, 005h, 038h, 008h, 000h, 000h
        jne    SHORT loc_3_016A
        mov    eax, dword ptr [ebp + 14h]
        db  03Bh, 005h, 034h, 008h, 000h, 000h
        jne    SHORT loc_3_016A
        db  0A1h, 03Ch, 008h, 000h, 000h
        mov    dword ptr [ebp + 14h], eax
        db  0A1h, 040h, 008h, 000h, 000h
        mov    dword ptr [ebp + 18h], eax
        db  0A1h, 044h, 008h, 000h, 000h
        mov    dword ptr [ebp + 4], eax
        db  0A1h, 048h, 008h, 000h, 000h
        mov    dword ptr [ebp], eax
        jmp    NEAR PTR loc_3_0284
loc_3_016A:
        push   esi
        mov    edx, dword ptr [esi + 2]
        push   0
        push   1000h
        push   edx
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        db  0A3h, 048h, 008h, 000h, 000h
        push   0
        push   80h
        push   92h
        push   1
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        db  0A3h, 044h, 008h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        pop    esi
        mov    eax, dword ptr [ebp + 14h]
        db  0A3h, 034h, 008h, 000h, 000h
        mov    eax, dword ptr [esi + 1ch]
        db  0A3h, 038h, 008h, 000h, 000h
        mov    edx, dword ptr [esi + 6]
        mov    eax, dword ptr [esi + 1ch]
        db  083h, 03Dh, 034h, 008h, 000h, 000h, 010h
        jne    SHORT loc_3_01D9
        shl    eax, 1
loc_3_01D9:
        db  083h, 03Dh, 034h, 008h, 000h, 000h, 018h
        jne    SHORT loc_3_01E5
        lea    eax, [eax + eax*2]
loc_3_01E5:
        db  083h, 03Dh, 034h, 008h, 000h, 000h, 020h
        jne    SHORT loc_3_01F1
        shl    eax, 2
loc_3_01F1:
        add    edx, eax
        push   0
        push   800000h
        push   edx
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        db  0A3h, 040h, 008h, 000h, 000h
        push   0
        push   80h
        push   92h
        push   800h
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        db  0A3h, 03Ch, 008h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        db  0A1h, 03Ch, 008h, 000h, 000h
        mov    dword ptr [ebp + 14h], eax
        db  0A1h, 040h, 008h, 000h, 000h
        mov    dword ptr [ebp + 18h], eax
        db  0A1h, 044h, 008h, 000h, 000h
        mov    dword ptr [ebp + 4], eax
        db  0A1h, 048h, 008h, 000h, 000h
        mov    dword ptr [ebp], eax
        jmp    SHORT loc_3_0284
loc_3_0261:
        push   0
        push   4000h
        push   0ac000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        mov    dword ptr [ebp + 14h], eax
loc_3_0279:
        clc
        db  0C7h, 005h, 010h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_0284:
        stc
        jmp    SHORT loc_3_0287
loc_3_0287:
        popad
        ret
        db  000h, 000h, 000h
_PTEXT ends
        end
