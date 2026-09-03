; vmvid_pcode_04.asm -- VMVID object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  060h, 066h, 08Bh, 015h, 000h, 000h, 000h, 000h, 066h, 0A1h, 000h, 000h, 000h, 000h, 066h, 08Bh
        db  00Dh, 000h, 000h, 000h, 000h, 0E8h, 000h, 000h, 000h, 000h, 00Fh, 082h, 0B2h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h, 00Fh, 082h, 0A7h, 000h, 000h, 000h, 066h, 0A3h, 000h, 000h, 000h
        db  000h, 0E8h, 000h, 000h, 000h, 000h, 00Fh, 082h, 096h, 000h, 000h, 000h, 066h, 0A3h, 000h, 000h
        db  000h, 000h, 066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h, 066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h, 0CDh, 020h, 001h, 000h, 003h, 000h, 072h, 075h, 0A3h, 000h, 000h
        db  000h, 000h, 066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h, 066h, 083h, 0F9h, 000h, 074h, 019h, 066h
        db  089h, 00Dh, 000h, 000h, 000h, 000h, 0BFh, 000h, 000h, 000h, 000h, 0CDh, 020h, 001h, 000h, 003h
        db  000h, 072h, 04Fh, 0A3h, 000h, 000h, 000h, 000h, 066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 008h
        db  000h, 0B9h, 008h, 000h, 000h, 000h, 0BEh, 000h, 000h, 000h, 000h, 056h, 0BFh, 000h, 000h, 000h
        db  000h, 083h, 0C6h, 002h, 033h, 0C9h, 066h, 08Bh, 014h, 04Dh, 000h, 000h, 000h, 000h, 066h, 003h
        db  015h, 000h, 000h, 000h, 000h, 066h, 089h, 016h, 083h, 0C6h, 006h, 041h, 081h, 0F9h, 008h, 000h
        db  000h, 000h, 072h, 0E2h, 05Fh, 0CDh, 020h, 095h, 000h, 001h, 000h, 072h, 003h, 061h, 0F8h, 0C3h
        db  0EBh, 0FBh, 061h, 0F9h, 0C3h
sub_4_00D5:
        push   ebp
        mov    ebp, esp
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_00E4
        jmp    SHORT loc_4_00EB
loc_4_00E4:
        mov    eax, 0
        jmp    SHORT loc_4_00F0
loc_4_00EB:
        mov    eax, 0ffffffffh
loc_4_00F0:
        pop    edi
        pop    ebp
        ret
        db  090h, 0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_4_00FC:
        pushad
        pushfd
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_4_0151
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_011E
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        mov    eax, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_011E:
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0135
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_012B:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0135:
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0151
        mov    eax, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0147:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0151:
        popfd
        popad
        db  0FFh, 025h, 000h, 000h, 000h, 000h
        db  02Eh, 08Bh, 0C0h
loc_4_015C:
        db  0CDh, 020h, 012h, 080h, 004h, 000h
        cmp    eax, 40h
        jle    SHORT loc_4_016C
        mov    eax, 40h
loc_4_016C:
        add    eax, 3
        shr    eax, 2
loc_4_0172:
        mov    ebx, eax
        dec    eax
        bsr    cx, ax
        inc    cl
        mov    eax, 1
        shl    eax, cl
        dec    eax
        mov    ecx, ebx
        push   ecx
        sub    esp, 4
        mov    ebx, esp
        push   0eh
        push   ebx
        push   0fffh
        push   0
        push   eax
        push   0
        push   1
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        pop    ebx
        pop    ecx
        or     eax, eax
        jne    SHORT loc_4_01B4
        mov    eax, ecx
        shr    eax, 1
        jne    SHORT loc_4_0172
        xor    ecx, ecx
        stc
        ret
loc_4_01B4:
        shl    ecx, 0ch
        clc
        ret
sub_4_01B9:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp + 2ch]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp + 0ch]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp + 14h]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp + 18h]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp + 10h]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp + 1ch]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp + 20h]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp + 24h]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_4_03E7
        jb     SHORT loc_4_0268
        movzx  eax, word ptr [ebp + 20h]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        jb     SHORT loc_4_0268
        movzx  eax, word ptr [ebp + 24h]
        cmp    ax, word ptr [ebp + 20h]
        je     SHORT loc_4_025A
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        jb     SHORT loc_4_0268
loc_4_025A:
        mov    dword ptr [esp + 1ch], 0ffffffffh
loc_4_0262:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_0268:
        call   sub_4_0277
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_0262
sub_4_0277:
        pushad
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_0287
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
loc_4_0287:
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_0296
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
loc_4_0296:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_02BE
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_02BE:
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 017h, 001h, 001h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 017h, 001h, 001h, 000h
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_02F8
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 017h, 001h, 001h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_02F8
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_4_02F8:
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_0307
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_4_0307:
        popad
        ret
sub_4_0309:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [esp + 1ch], 13h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 2ch]
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_01B9
        add    esp, 28h
        or     eax, eax
        je     SHORT loc_4_0370
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        mov    dword ptr [esp + 1ch], 0
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0370:
        popad
        add    esp, 4
        pop    ebp
        ret
        db  0C7h, 044h, 024h, 01Ch, 024h, 000h, 000h, 000h, 0EBh, 0F0h
sub_4_0380:
        push   ebp
        mov    ebp, esp
        push   edi
        pushfd
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 0FFh
        jne    SHORT loc_4_03B2
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_03A8
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        mov    eax, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03A8:
        call   sub_4_0277
        mov    eax, 0ffffffffh
loc_4_03B2:
        popfd
        pop    edi
        pop    ebp
        ret
loc_4_03B6:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03CD:
        add    esp, 14h
        leave
        clc
        ret
loc_4_03D3:
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
        stc
        ret
sub_4_03E7:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    ax, 70h
        db  066h, 08Bh, 01Dh, 000h, 000h, 000h, 000h
        and    bx, 30h
        shr    bx, 4
        or     bx, ax
        mov    dx, 201h
        mov    ax, 0afh
        out    dx, al
        jmp    SHORT loc_4_040E
loc_4_040E:
        mov    ax, 50h
        out    dx, al
        jmp    SHORT loc_4_0415
loc_4_0415:
        mov    ax, bx
        out    dx, al
        call   0
        jb     NEAR PTR loc_4_055F
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0429:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0438
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0438:
        db  00Fh, 0B7h, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, ecx
        sub    eax, 1
        push   0fh
        db  068h, 000h, 000h, 000h, 000h
        push   0fffh
        push   110h
        push   eax
        push   0
        push   1
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    ecx, eax
        or     ecx, edx
        je     SHORT loc_4_04B1
        db  0A3h, 000h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 000h, 000h, 000h, 000h
        shl    ecx, 0ch
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        sub    ecx, 1
        push   0
        push   0
        push   0f2h
        push   ecx
        push   edx
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_04B1
loc_4_04B1:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        and    ax, 0f0h
        cmp    ax, 10h
        je     SHORT loc_4_04EA
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    al, 7
        jle    SHORT loc_4_04D2
        mov    al, 0ffh
        jmp    NEAR PTR loc_4_055F
loc_4_04D2:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    ax, 3
        jle    SHORT loc_4_04E2
        mov    al, 0ffh
        jmp    SHORT loc_4_055F
loc_4_04E2:
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_055D
loc_4_04EA:
        xor    ebx, ebx
        db  066h, 08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  08Ah, 083h, 000h, 000h, 000h, 000h
        mov    cx, 4
        shl    al, cl
        xor    ebx, ebx
        db  066h, 08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  00Ah, 083h, 000h, 000h, 000h, 000h
        mov    bx, ax
        push   eax
        xor    ebx, ebx
        db  066h, 08Bh, 01Dh, 000h, 000h, 000h, 000h
        and    bx, 7
        db  08Ah, 083h, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        db  066h, 08Bh, 01Dh, 000h, 000h, 000h, 000h
        and    bx, 7
        db  08Ah, 09Bh, 000h, 000h, 000h, 000h
        mov    cx, 4
        shl    ax, cl
        or     bl, al
        mov    al, 0fbh
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_054F:
        mov    ax, bx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0557:
        pop    eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_055D:
        clc
        ret
loc_4_055F:
        stc
        ret
_PTEXT ends
        end
