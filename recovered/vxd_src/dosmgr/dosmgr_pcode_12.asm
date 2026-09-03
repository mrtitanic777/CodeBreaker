; dosmgr_pcode_12.asm -- DOSMGR object 12 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_12_0000:
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    dword ptr es:[eax], eax
        db  000h, 026h
        add    dword ptr [eax], eax
        db  000h, 030h
        add    dword ptr [eax], eax
        db  000h, 030h
        add    dword ptr [eax], eax
        db  000h, 010h
        add    eax, dword ptr [eax]
        db  000h, 010h
        add    eax, dword ptr [eax]
        db  000h, 010h
        add    eax, dword ptr [eax]
        db  000h, 010h
        add    eax, dword ptr [eax]
        db  000h, 010h
        add    eax, dword ptr [eax]
        db  000h, 010h
        add    eax, dword ptr [eax]
        db  000h, 018h
        add    dword ptr [eax], eax
        db  000h, 02Bh
        add    dword ptr [eax], eax
        db  000h, 010h
        add    eax, dword ptr [eax]
        db  000h, 010h
        add    eax, dword ptr [eax]
        db  000h, 010h
        add    eax, dword ptr [eax]
        db  000h, 010h
        add    eax, dword ptr [eax]
        add    byte ptr [edi], cl
        mov    dh, 55h
        sbb    al, 83h
        cli
        adc    dword ptr [edi + 0dh], esi
        db  08Bh, 014h, 095h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_12_005E:
        db  0BAh, 010h, 003h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + 53h], dl
        mov    eax, dword ptr [ebp + 14h]
        test   word ptr [ebx], 180h
        jne    SHORT loc_12_007B
        movzx  eax, ax
loc_12_007B:
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 38h], eax
        mov    ax, word ptr [ebp + 3ch]
        mov    word ptr [ebx + 36h], ax
        xor    eax, eax
        mov    dword ptr [ebx + 3ch], eax
        pop    ebx
        pop    eax
        ret
loc_12_0094:
        push   eax
        push   ebx
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        movzx  eax, word ptr [ebx + 36h]
        verw   ax
        je     SHORT loc_12_00AE
        xor    eax, eax
        mov    word ptr [ebx + 36h], ax
        mov    dword ptr [ebx + 38h], eax
loc_12_00AE:
        mov    word ptr [ebp + 38h], ax
        mov    eax, dword ptr [ebx + 38h]
        mov    word ptr [ebp + 10h], ax
        pop    ebx
        test   word ptr [ebx], 180h
        je     SHORT loc_12_00C4
        mov    dword ptr [ebp + 10h], eax
loc_12_00C4:
        pop    eax
        ret
sub_12_00C6:
        mov    ecx, 25h
        cmp    byte ptr fs:[esi], 0ffh
        je     SHORT loc_12_00D2
        ret
loc_12_00D2:
        add    ecx, 7
        ret
loc_12_00D6:
        push   fs
        pushad
        mov    eax, 3c14h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        call   sub_12_00C6
        jmp    SHORT loc_12_00F3
loc_12_00EB:
        push   fs
        pushad
        mov    ecx, 2bh
loc_12_00F3:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    fs, word ptr [eax + ebx + 36h]
        mov    esi, dword ptr [eax + ebx + 38h]
        stc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        cmp    dword ptr [eax + ebx + 3ch], edi
        je     SHORT loc_12_0138
        mov    dword ptr [eax + ebx + 3ch], edi
        push   dword ptr [ebp + 60h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 14h]
        mov    word ptr [ebp + 14h], di
        shr    edi, 10h
        mov    word ptr [ebp + 60h], di
        mov    byte ptr [ebp + 1dh], 1ah
        call   sub_12_04B0
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 1ch]
        pop    dword ptr [ebp + 60h]
loc_12_0138:
        cmp    byte ptr [ebp + 1dh], 12h
        ja     SHORT loc_12_014B
        db  0BAh, 025h, 003h, 000h, 000h
        db  0CDh, 020h, 006h, 000h, 006h, 000h
        jmp    SHORT loc_12_0150
loc_12_014B:
        call   sub_12_04B0
loc_12_0150:
        stc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        popad
        pop    fs
        ret
loc_12_015B:
        pushad
        movzx  eax, byte ptr [ebp + 1ch]
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        mov    word ptr [ebp + 38h], cx
        test   word ptr [ebx], 180h
        jne    SHORT loc_12_0177
        mov    word ptr [ebp + 10h], dx
        popad
        ret
loc_12_0177:
        mov    dword ptr [ebp + 10h], edx
        popad
        ret
loc_12_017C:
        movzx  eax, byte ptr [ebp + 1ch]
        movzx  ecx, word ptr [ebp + 3ch]
        mov    edx, dword ptr [ebp + 14h]
        test   word ptr [ebx], 180h
        jne    SHORT loc_12_0191
        movzx  edx, dx
loc_12_0191:
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        cmp    al, 24h
        jne    SHORT loc_12_01D6
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_12_01D6
        db  083h, 03Dh, 0F4h, 001h, 000h, 000h, 000h
        je     SHORT loc_12_01D6
        push   ecx
        xor    ecx, ecx
        db  066h, 087h, 00Dh, 03Ch, 001h, 000h, 000h
        jecxz  loc_12_01D5
        db  089h, 015h, 03Eh, 001h, 000h, 000h
        mov    edx, dword ptr [esp]
        db  066h, 089h, 015h, 042h, 001h, 000h, 000h
        db  00Fh, 0B7h, 015h, 03Ah, 001h, 000h, 000h
        db  0CDh, 020h, 045h, 000h, 001h, 000h
loc_12_01D5:
        pop    ecx
loc_12_01D6:
        ret
loc_12_01D7:
        push   fs
        pushad
        mov    ax, 3c04h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        mov    ecx, 14h
        stc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        mov    eax, edi
        mov    word ptr [ebp + 4], ax
        shr    eax, 10h
        xchg   word ptr [ebp + 60h], ax
        call   sub_12_04B0
        stc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        mov    word ptr [ebp + 60h], ax
        movzx  ecx, word ptr [ebp + 4]
        sub    cx, di
        add    esi, ecx
        mov    dword ptr [ebp + 4], esi
        popad
        pop    fs
        ret
loc_12_021D:
        db  0BAh, 010h, 003h, 000h, 000h
        cmp    byte ptr [ebp + 1ch], 0bh
        ja     SHORT loc_12_0233
        movzx  edx, byte ptr [ebp + 1ch]
        db  08Bh, 014h, 095h, 090h, 004h, 000h, 000h
loc_12_0233:
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_12_0239:
        db  0BAh, 010h, 003h, 000h, 000h
        cmp    byte ptr [ebp + 1ch], 3
        ja     SHORT loc_12_024F
        movzx  edx, byte ptr [ebp + 1ch]
        db  08Bh, 014h, 095h, 0C0h, 004h, 000h, 000h
loc_12_024F:
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_12_0255:
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        cmp    al, 4
        db  0BAh, 05Ah, 004h, 000h, 000h
        je     SHORT loc_12_0270
        sub    edx, 0ch
        sub    al, 2
        cmp    al, 3
        jbe    SHORT loc_12_0270
        db  0BAh, 010h, 003h, 000h, 000h
loc_12_0270:
        pop    eax
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_12_0277:
        pushad
        mov    eax, dword ptr [ebp + 10h]
        xor    edx, edx
        db  0E8h, 0E6h, 002h, 0F4h, 0FFh
        jae    SHORT loc_12_02A4
        shr    eax, 4
        xchg   dword ptr [ebp + 10h], eax
        call   sub_12_04B0
        mov    dword ptr [ebp + 10h], eax
        mov    word ptr [edi + 2ah], ax
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ebx + 2eh], 0
        popad
        ret
loc_12_02A4:
        or     byte ptr [ebp + 2ch], 1
        popad
        ret
loc_12_02AA:
        pushad
        movzx  eax, word ptr [edi + 2ah]
        mov    esi, eax
        xor    edx, edx
        test   eax, eax
        je     SHORT loc_12_02BC
        db  0E8h, 0ACh, 002h, 0F4h, 0FFh
loc_12_02BC:
        mov    ecx, eax
        shr    ecx, 4
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        cmp    cx, word ptr [ebp + 10h]
        jne    SHORT loc_12_02E8
        mov    word ptr [ebp + 10h], si
loc_12_02E2:
        and    byte ptr [ebp + 2ch], 0feh
        popad
        ret
loc_12_02E8:
        movzx  eax, word ptr [ebp + 10h]
        shl    eax, 4
        mov    ecx, 0ffh
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        mov    word ptr [edi + 2ah], cx
        mov    word ptr [ebp + 10h], cx
        jmp    SHORT loc_12_02E2
loc_12_0304:
        pushad
        mov    eax, dword ptr [ebp + 14h]
        xor    edx, edx
        db  0E8h, 059h, 002h, 0F4h, 0FFh
        jae    SHORT loc_12_0321
        shr    eax, 4
        xchg   dword ptr [ebp + 14h], eax
        call   sub_12_04B0
        mov    dword ptr [ebp + 14h], eax
        popad
        ret
loc_12_0321:
        popad
        ret
loc_12_0323:
        pushad
        mov    eax, dword ptr [ebp + 14h]
        xor    edx, edx
        db  0E8h, 03Ah, 002h, 0F4h, 0FFh
        jae    SHORT loc_12_0350
        shr    eax, 4
        xchg   dword ptr [ebp + 14h], eax
        call   sub_12_04B0
        mov    dword ptr [ebp + 14h], eax
        mov    word ptr [edi + 2ah], ax
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ebx + 2eh], 0
        popad
        ret
loc_12_0350:
        popad
        ret
loc_12_0352:
        pushad
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_12_0375
        lea    esi, [edi + 33h]
        cld
        lodsb
        mov    byte ptr [ebp + 19h], al
        lodsw
        mov    word ptr [ebp + 1ch], ax
        lodsw
        mov    word ptr [ebp + 10h], ax
        and    byte ptr [ebp + 2ch], 0feh
        jmp    SHORT loc_12_037A
loc_12_0375:
        call   sub_12_04B0
loc_12_037A:
        popad
        ret
loc_12_037C:
        push   eax
        push   esi
        push   fs
        mov    ecx, 1
        mov    eax, 3c14h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
loc_12_0390:
        cmp    byte ptr fs:[esi], 0
        je     SHORT loc_12_039A
        inc    ecx
        inc    esi
        jmp    SHORT loc_12_0390
loc_12_039A:
        add    ecx, 0dh
        pop    fs
        pop    esi
        pop    eax
        ret
loc_12_03A2:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    byte ptr [ebp + 1ch], 0
        je     SHORT loc_12_03BA
        mov    word ptr [edx + ebx + 2eh], 0
        jmp    NEAR PTR sub_12_04B0
loc_12_03BA:
        push   eax
        mov    ax, word ptr [edi + 2ah]
        mov    word ptr [edx + ebx + 34h], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [edx + ebx + 2eh], ax
        mov    eax, dword ptr [ebp + 18h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 14h]
        mov    dword ptr [edx + ebx + 30h], eax
        pop    eax
        jmp    NEAR PTR sub_12_04B0
loc_12_03E0:
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    esi, ebx
        movzx  eax, word ptr [edi + 2ah]
        push   fs
        mov    fs, ax
        mov    ax, word ptr [ebp + 28h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 24h]
        xor    edx, edx
        mov    dword ptr fs:[edx + 0ah], eax
        mov    ax, word ptr fs:[edx + 16h]
        db  0E8h, 046h, 001h, 0F4h, 0FFh
        jae    SHORT loc_12_045D
        movzx  ecx, word ptr fs:[edx + 32h]
loc_12_0429:
        dec    ecx
        mov    byte ptr [ebp + 1dh], 3eh
        mov    word ptr [ebp + 10h], cx
        push   fs
        pushad
        db  0E8h, 078h, 0FDh, 0F3h, 0FFh
        popad
        pop    fs
        test   ecx, ecx
        jne    SHORT loc_12_0429
        mov    ecx, eax
        shr    eax, 4
        mov    word ptr fs:[edx + 16h], ax
        mov    word ptr [edi + 2ah], ax
        mov    eax, dword ptr [ebp + 32h]
        mov    ax, word ptr [ebp + 30h]
        sub    ax, 18h
        mov    dword ptr [ecx + 2eh], eax
loc_12_045D:
        db  0A1h, 040h, 000h, 000h, 000h
        mov    dword ptr fs:[edx + 0eh], eax
        db  0A1h, 044h, 000h, 000h, 000h
        mov    dword ptr fs:[edx + 12h], eax
        pop    fs
        mov    byte ptr [ebp + 1dh], 4ch
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0A1h, 040h, 000h, 000h, 000h
        mov    dword ptr [edx + 8ch], eax
        db  0A1h, 044h, 000h, 000h, 000h
        mov    dword ptr [edx + 90h], eax
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
        popad
        and    byte ptr [ebp + 2ch], 0feh
        ret
        db  02Eh, 08Bh, 0C0h
sub_12_04B0:
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        push   eax
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 086h, 080h, 001h, 000h
_PTEXT ends
        end
