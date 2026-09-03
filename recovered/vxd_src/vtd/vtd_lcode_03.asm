; vtd_lcode_03.asm -- VTD object 3 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_3_0000:
        cmp    eax, 0
        je     SHORT loc_3_0010
        cmp    eax, 1
        je     NEAR PTR loc_3_0183
        clc
        ret
loc_3_0010:
        db  089h, 01Dh, 0FCh, 004h, 000h, 000h
        push   0
        push   18h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_3_017D
        db  0A3h, 0F4h, 004h, 000h, 000h
        db  0E8h, 0CCh, 0FFh, 0FDh, 0FFh
        mov    al, 34h
        out    43h, al
        jmp    SHORT loc_3_003A
loc_3_003A:
        jmp    SHORT loc_3_003C
loc_3_003C:
        xor    eax, eax
        out    40h, al
        jmp    SHORT loc_3_0042
loc_3_0042:
        jmp    SHORT loc_3_0044
loc_3_0044:
        out    40h, al
        xor    eax, eax
        jmp    SHORT loc_3_004A
loc_3_004A:
        jmp    SHORT loc_3_004C
loc_3_004C:
        out    43h, al
        jmp    SHORT loc_3_0050
loc_3_0050:
        jmp    SHORT loc_3_0052
loc_3_0052:
        in     al, 40h
        mov    ecx, eax
        jmp    SHORT loc_3_0058
loc_3_0058:
        jmp    SHORT loc_3_005A
loc_3_005A:
        in     al, 40h
        mov    ch, al
loc_3_005E:
        xor    eax, eax
        jmp    SHORT loc_3_0062
loc_3_0062:
        jmp    SHORT loc_3_0064
loc_3_0064:
        out    43h, al
        jmp    SHORT loc_3_0068
loc_3_0068:
        jmp    SHORT loc_3_006A
loc_3_006A:
        in     al, 40h
        mov    ah, al
        jmp    SHORT loc_3_0070
loc_3_0070:
        jmp    SHORT loc_3_0072
loc_3_0072:
        in     al, 40h
        xchg   al, ah
        cmp    eax, ecx
        je     SHORT loc_3_005E
        cmp    ah, 0ffh
        jne    SHORT loc_3_005E
        mov    ecx, eax
        nop
        nop
        nop
        xor    eax, eax
        out    43h, al
        jmp    SHORT loc_3_008A
loc_3_008A:
        jmp    SHORT loc_3_008C
loc_3_008C:
        in     al, 40h
        jmp    SHORT loc_3_0090
loc_3_0090:
        jmp    SHORT loc_3_0092
loc_3_0092:
        mov    ah, al
        in     al, 40h
        xchg   al, ah
        sub    cx, ax
        add    ecx, ecx
        cmp    ecx, 14h
        jbe    SHORT loc_3_00A7
        mov    ecx, 14h
loc_3_00A7:
        db  089h, 00Dh, 01Ch, 005h, 000h, 000h
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        test   ebx, 40000h
        je     SHORT loc_3_00CF
        db  0C7h, 005h, 0D8h, 002h, 000h, 000h, 0C8h, 002h, 000h, 000h
        in     al, 61h
        or     al, 80h
        jmp    SHORT loc_3_00CB
loc_3_00CB:
        jmp    SHORT loc_3_00CD
loc_3_00CD:
        out    61h, al
loc_3_00CF:
        or     eax, 0ffffffffh
        xor    esi, esi
        db  0BFh, 0F4h, 002h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0BFh, 0D4h, 002h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_3_00EB
        mov    dword ptr [edi + 14h], eax
loc_3_00EB:
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     NEAR PTR loc_3_017D
        db  0A3h, 0F8h, 004h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        mov    ax, 0c3f8h
        db  066h, 087h, 005h, 04Bh, 003h, 000h, 000h
        db  066h, 0A3h, 032h, 003h, 000h, 000h
        or     eax, 0ffffffffh
        xor    esi, esi
        db  0BFh, 001h, 003h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        inc    eax
        db  0A2h, 036h, 005h, 000h, 000h
        mov    edx, 43h
        db  0BEh, 00Ch, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 40h
        db  0BEh, 078h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    eax, 1ah
        db  0BEh, 064h, 002h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, dword ptr [46ch]
        db  0A3h, 030h, 005h, 000h, 000h
        cmp    byte ptr [470h], 0
        je     SHORT loc_3_017B
        db  0A1h, 0C8h, 004h, 000h, 000h
        mov    byte ptr [470h], 0
        inc    word ptr [eax]
loc_3_017B:
        clc
        ret
loc_3_017D:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_3_0183:
        push   0
        push   2
        push   edx
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_3_019B
        db  0A3h, 0C8h, 004h, 000h, 000h
loc_3_019B:
        mov    eax, 1ch
        db  0BEh, 0B0h, 003h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 160eh
        mov    byte ptr [ebp + 10h], 2
        xor    eax, eax
        mov    al, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_3_01D4:
        mov    eax, dword ptr [20h]
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A3h, 013h, 003h, 000h, 000h
        mov    eax, dword ptr [70h]
        db  0A3h, 004h, 000h, 000h, 000h
        db  0A3h, 02Eh, 003h, 000h, 000h
        mov    word ptr [ebp + 1ch], 160eh
        mov    byte ptr [ebp + 10h], 1
        xor    eax, eax
        mov    al, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        push   104h
        push   22h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_3_017D
        db  0A3h, 028h, 005h, 000h, 000h
        db  0BEh, 010h, 003h, 000h, 000h
        mov    ecx, 22h
        mov    edi, eax
        cld
        rep movsb
        shr    eax, 4
        shl    eax, 10h
        mov    ecx, eax
        mov    cl, 1dh
loc_3_0253:
        mov    dword ptr [20h], eax
        mov    dword ptr [70h], ecx
        db  066h, 0A1h, 032h, 003h, 000h, 000h
        db  066h, 0A3h, 04Bh, 003h, 000h, 000h
        db  0BEh, 006h, 004h, 000h, 000h
        db  0CDh, 020h, 02Ah, 000h, 001h, 000h
        mov    eax, 15000eh
        db  0BEh, 06Bh, 001h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, 8
        db  0CDh, 020h, 00Bh, 000h, 003h, 000h
        jb     SHORT loc_3_02B2
        test   ch, 1
        jne    SHORT loc_3_02A0
        test   ch, 2
        jne    SHORT loc_3_02A7
        or     ebx, 0ffffffffh
        jmp    SHORT loc_3_02A7
loc_3_02A0:
        xor    ebx, ebx
        or     eax, 20000h
loc_3_02A7:
        or     eax, 10000h
        db  0CDh, 020h, 010h, 000h, 003h, 000h
loc_3_02B2:
        db  0CDh, 020h, 000h, 000h, 026h, 000h
        cmp    ah, 4
        jb     SHORT loc_3_02CF
        push   0e0000000h
        db  068h, 0ACh, 001h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 026h, 000h
        pop    ecx
        pop    ecx
loc_3_02CF:
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 0D4h, 002h, 000h, 000h, 04Bh, 003h, 000h, 000h, 088h
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 08Eh, 003h, 000h, 000h, 0F4h, 001h, 000h, 000h, 0ECh
        db  004h, 000h, 000h, 054h, 069h, 06Dh, 065h, 072h, 041h, 075h, 074h
loc_3_02FC:
        outsd
        inc    ebp
        dec    edi
        dec    ecx
        add    byte ptr [ecx + 75h], al
        je     SHORT loc_3_0374
        inc    esp
        popad
        je     SHORT loc_3_036E
        push   ebp
        jo     SHORT loc_3_0370
        popad
        je     SHORT loc_3_0374
        add    bl, ch
        add    dl, ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        mov    eax, cs
        cmp    dword ptr cs:[esi], eax
        adc    eax, 9c067400h
        lcall  20b0h, 0
        out    20h, al
        pop    eax
        iretd
        db  0EAh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
loc_3_036E:
        add    byte ptr [eax], al
loc_3_0370:
        add    byte ptr [eax], al
loc_3_0372:
        add    byte ptr [eax], al
loc_3_0374:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0392:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0396:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03BE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03D2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03D6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0404:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0418:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
