; shell_pcode_13.asm -- SHELL object 13 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

SHELL_GetVMInfo:
        push   ebx
        db  003h, 01Dh, 000h, 00Ah, 000h, 000h
        mov    eax, dword ptr [ebx + 0ch]
        and    eax, 4007ffe4h
        or     edx, 0ffffffffh
        mov    edi, edx
        mov    dx, word ptr [ebx + 36h]
        mov    esi, edi
        pop    ebx
        ret
loc_13_001C:
        sub    eax, eax
        xchg   dword ptr [ebp + 18h], eax
        and    byte ptr [ebp + 2ch], 0feh
        db  08Bh, 035h, 020h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_13_0084
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        or     eax, eax
        jne    SHORT loc_13_0049
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_13_0084
        jmp    SHORT loc_13_005A
loc_13_0049:
        mov    edx, dword ptr [eax]
loc_13_004B:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_13_0084
        call   sub_13_00DD
        je     SHORT loc_13_004B
loc_13_005A:
        xchg   ebx, eax
        mov    dword ptr [ebp + 18h], ebx
        push   edi
        lea    edi, [edi + 14h]
        sub    eax, eax
        mov    byte ptr [edi], al
        mov    esi, dword ptr [ebx + 4]
        or     esi, esi
        je     SHORT loc_13_0073
loc_13_006D:
        lodsb
        stosb
        or     al, al
        jne    SHORT loc_13_006D
loc_13_0073:
        pop    edi
        mov    esi, dword ptr [ebx]
        mov    dword ptr [edi], esi
        cmp    esi, 0bh
        jb     SHORT loc_13_0084
        stosd
loc_13_007E:
        lodsb
        stosb
        or     al, al
        jne    SHORT loc_13_007E
loc_13_0084:
        ret
loc_13_0085:
        db  08Bh, 035h, 020h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_13_00D8
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_13_00D8
loc_13_00A3:
        mov    edx, dword ptr [edi]
        and    edx, 0ffh
        jne    SHORT loc_13_00B0
        lea    edx, [edi + 4]
loc_13_00B0:
        call   sub_13_00DD
        jne    SHORT loc_13_00CB
        mov    ebx, dword ptr [ebp + 10h]
        xchg   edx, eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edx, eax
        push   eax
        call   dword ptr [eax + 8]
        pop    eax
loc_13_00CB:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_13_00A3
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_13_00D8:
        or     byte ptr [ebp + 2ch], 1
        ret
sub_13_00DD:
        or     edx, edx
        je     SHORT loc_13_011F
        cmp    dword ptr [eax], edx
        je     SHORT loc_13_011F
        cmp    edx, 0bh
        jb     SHORT loc_13_011F
        cmp    dword ptr [eax], 0bh
        jb     SHORT loc_13_011F
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    ebx, eax
        sub    eax, eax
        mov    ecx, 10h
        mov    edi, edx
        repne scasb
        not    ecx
        add    ecx, 10h
        cmp    ecx, 10h
        jb     SHORT loc_13_010F
        mov    ecx, 0fh
loc_13_010F:
        mov    edi, edx
        mov    esi, dword ptr [ebx]
        repe cmpsb
        jne    SHORT loc_13_0119
        cmp    byte ptr [esi], al
loc_13_0119:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
loc_13_011F:
        ret
loc_13_0120:
        db  08Bh, 03Dh, 000h, 00Ah, 000h, 000h
        push   edi
        lea    edi, [edi + ebx + 0ch]
        push   edi
        mov    ecx, 5eh
        cld
        rep movsb
        db  0CDh, 020h, 032h, 000h, 001h, 000h
        pop    esi
        pop    edi
        mov    ecx, dword ptr [esi]
        xor    ecx, eax
        and    ecx, 3
        xor    eax, ecx
        xor    ecx, ecx
        inc    ecx
        mov    edx, ecx
        db  0CDh, 020h, 033h, 000h, 001h, 000h
        jb     NEAR PTR loc_13_0232
        push   esi
        movzx  edi, word ptr [esi + 1ch]
        movzx  esi, word ptr [esi + 1ah]
        cmp    di, 280h
        jne    SHORT loc_13_016A
        or     di, 0ffffh
loc_13_016A:
        cmp    si, 280h
        jne    SHORT loc_13_0175
        or     si, 0ffffh
loc_13_0175:
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        shl    eax, 2
        mov    ecx, eax
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        shl    eax, 2
        cmp    si, -1
        jne    SHORT loc_13_0194
        mov    esi, ecx
        jmp    SHORT loc_13_0196
loc_13_0194:
        add    esi, eax
loc_13_0196:
        cmp    di, -1
        jne    SHORT loc_13_01A0
        mov    edi, ecx
        jmp    SHORT loc_13_01A2
loc_13_01A0:
        add    edi, eax
loc_13_01A2:
        xor    ecx, ecx
        pop    eax
        push   eax
        test   byte ptr [eax + 2], 4
        je     SHORT loc_13_01AF
        or     ecx, 1
loc_13_01AF:
        db  0CDh, 020h, 001h, 000h, 006h, 000h
        pop    esi
        jb     SHORT loc_13_0231
        mov    dword ptr [esi + 1ah], eax
        mov    ecx, 1
        test   byte ptr [esi + 1], 20h
        jne    SHORT loc_13_01CB
        mov    ecx, 2
loc_13_01CB:
        test   byte ptr [esi + 1], 80h
        je     SHORT loc_13_01D4
        or     ecx, 8
loc_13_01D4:
        test   byte ptr [esi + 2], 1
        je     SHORT loc_13_01DD
        or     ecx, 4
loc_13_01DD:
        movzx  eax, word ptr [esi + 24h]
        movzx  edx, word ptr [esi + 22h]
        cmp    dx, -1
        jne    SHORT loc_13_01F0
        mov    edx, 0ffffffffh
loc_13_01F0:
        movzx  edi, word ptr [esi + 26h]
        cmp    di, -1
        jne    SHORT loc_13_01FF
        mov    edi, 0ffffffffh
loc_13_01FF:
        push   esi
        movzx  esi, word ptr [esi + 28h]
        db  0CDh, 020h, 002h, 000h, 006h, 000h
        pop    esi
        jb     SHORT loc_13_0231
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 0eh]
        push   ebx
        db  0CDh, 020h, 001h, 000h, 015h, 000h
        add    esp, 10h
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    esi, ebx
        pop    ebx
        db  0CDh, 020h, 002h, 000h, 015h, 000h
        clc
loc_13_0231:
        ret
loc_13_0232:
        mov    eax, 20008h
        db  08Dh, 015h, 0BCh, 009h, 000h, 000h
        xor    ebx, ebx
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        stc
        ret
loc_13_024A:
        db  08Bh, 00Dh, 000h, 00Ah, 000h, 000h
        mov    ax, word ptr [edx + 0ah]
        mov    word ptr [ecx + ebx + 2], ax
        ret
loc_13_025A:
        db  08Bh, 035h, 000h, 00Ah, 000h, 000h
        lea    esi, [esi + ebx + 0ch]
        mov    eax, dword ptr [esi]
        and    eax, 8
        db  0CDh, 020h, 004h, 000h, 00Ah, 000h
        mov    eax, dword ptr [esi + 4]
        and    eax, 0ffffh
        shr    eax, 0
        db  0CDh, 020h, 001h, 000h, 00Ah, 000h
        db  0CDh, 020h, 000h, 000h, 018h, 000h
        jb     SHORT loc_13_0295
        test   byte ptr [esi + 1], 10h
        setne  al
        db  0CDh, 020h, 001h, 000h, 018h, 000h
loc_13_0295:
        test   byte ptr [esi + 2], 2
        sete   al
        db  0CDh, 020h, 009h, 000h, 00Dh, 000h
        push   ebx
        push   esi
        movzx  ecx, word ptr [esi - 0ah]
        call   sub_13_02C5
        pop    esi
        pop    ebx
        push   0
        db  068h, 07Bh, 001h, 000h, 000h
        push   dword ptr [esi + 1ah]
        push   ebx
        db  0CDh, 020h, 003h, 000h, 015h, 000h
        add    esp, 10h
        clc
        ret
sub_13_02C5:
        movzx  edx, byte ptr [484h]
        inc    edx
        cmp    edx, ecx
        je     SHORT loc_13_0322
        mov    ax, 1202h
        cmp    ecx, 19h
        je     SHORT loc_13_02E5
        cmp    ecx, 32h
        je     SHORT loc_13_02E5
        cmp    ecx, 2bh
        jne    SHORT loc_13_0322
        dec    eax
loc_13_02E5:
        mov    bl, 30h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    al, 12h
        jne    SHORT loc_13_0322
        xor    eax, eax
        mov    al, 3
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    ecx, 19h
        je     SHORT loc_13_0322
        mov    ax, 1112h
        xor    ebx, ebx
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    ax, 100h
        mov    cx, 607h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_13_0322:
        mov    ah, 40h
        xor    ebx, ebx
        inc    ebx
        db  0BAh, 034h, 000h, 000h, 000h
        lea    ecx, [ebx + 1]
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    ah, 8
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        xchg   ebx, eax
        mov    dl, byte ptr [44ah]
        dec    edx
        mov    dh, byte ptr [484h]
        mov    ax, 600h
        xor    ecx, ecx
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        ret
        db  000h, 000h
loc_13_0360:
        db  08Bh, 00Dh, 06Ch, 00Ah, 000h, 000h
        db  0CDh, 020h, 0D0h, 000h, 001h, 000h
        test   eax, eax
        jne    SHORT loc_13_037B
        test   byte ptr [edx + 2], 4
        jne    SHORT loc_13_037B
        or     byte ptr [ecx + ebx + 10h], 8
loc_13_037B:
        and    byte ptr [ecx + ebx + 11h], 0feh
        test   byte ptr [edx + 1], 4
        je     SHORT loc_13_038B
        or     byte ptr [ecx + ebx + 11h], 1
loc_13_038B:
        ret
loc_13_038C:
        db  0E8h, 07Eh, 0FCh, 0F2h, 0FFh
        movzx  ecx, word ptr [edx]
        xor    ecx, dword ptr [eax]
        and    cx, 0fe0h
        xor    word ptr [eax], cx
        jmp    NEAR PTR sub_13_045C
loc_13_03A3:
        db  0E8h, 067h, 0FCh, 0F2h, 0FFh
        xchg   edi, eax
        db  08Bh, 03Dh, 06Ch, 00Ah, 000h, 000h
        push   201h
        push   1c8h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_13_03F1
        mov    dword ptr [edi + ebx + 0ch], eax
        xor    eax, eax
        mov    al, 0eh
        push   54h
        pop    ecx
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_13_03F1
        mov    dword ptr [edi + ebx + 4], esi
        mov    edi, esp
        db  0E8h, 024h, 002h, 0F3h, 0FFh
        jb     SHORT loc_13_03F1
        mov    byte ptr [eax + 2], 1
        db  0E8h, 051h, 003h, 0F3h, 0FFh
loc_13_03EF:
        clc
        ret
loc_13_03F1:
        db  0E8h, 0D6h, 0FEh, 0F2h, 0FFh
        stc
        ret
loc_13_03F8:
        db  0E8h, 012h, 0FCh, 0F2h, 0FFh
        test   byte ptr [eax], 2
        je     SHORT loc_13_0426
        push   ebx
        db  0CDh, 020h, 030h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_13_0425
        push   5
        pop    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  0E8h, 040h, 0FFh, 0F2h, 0FFh
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_13_0425:
        pop    ebx
loc_13_0426:
        xor    eax, eax
        lea    ecx, [eax + 3]
        db  0BEh, 038h, 004h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        clc
        ret
loc_13_0438:
        mov    ecx, 426h
        mov    eax, 200000h
        db  0BEh, 0EFh, 003h, 000h, 000h
        db  0CDh, 020h, 002h, 080h, 017h, 000h
loc_13_044D:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_13_045A
        call   sub_13_045C
loc_13_045A:
        clc
        ret
sub_13_045C:
        push   esi
        db  0E8h, 0ADh, 0FBh, 0F2h, 0FFh
        mov    edx, dword ptr [eax]
        xor    edx, 80000000h
        and    edx, 0fffffffeh
        db  0BEh, 08Ch, 00Ah, 000h, 000h
        lodsd
        xchg   ecx, eax
loc_13_0474:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_13_048D
        test   dword ptr [esi], edx
        je     SHORT loc_13_0487
        db  0CDh, 020h, 004h, 000h, 00Dh, 000h
        jmp    SHORT loc_13_048D
loc_13_0487:
        db  0CDh, 020h, 003h, 000h, 00Dh, 000h
loc_13_048D:
        add    esi, 14h
        loop   loc_13_0474
        pop    esi
        ret
loc_13_0494:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 1ch], 400h
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_13_04A9:
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        inc    esi
        je     NEAR PTR loc_13_05BE
        dec    esi
        xor    edi, edi
        movzx  eax, word ptr [esi + 0ch]
        push   edi
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        add    dword ptr [esi + 8], eax
        movzx  eax, word ptr [esi + 12h]
        push   edi
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        add    dword ptr [esi + 0eh], eax
        movzx  eax, word ptr [esi + 18h]
        push   edi
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        add    dword ptr [esi + 14h], eax
        mov    ax, 3cffh
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    word ptr [eax + 10h], 5043h
        je     SHORT loc_13_050D
        and    dword ptr [esi + 2ah], 0
loc_13_050D:
        mov    eax, dword ptr [esi + 4]
        and    eax, 0ffffh
        mov    ecx, eax
        xor    ecx, 1eh
        and    ecx, 7eh
        xor    eax, ecx
        db  0CDh, 020h, 001h, 000h, 00Ah, 000h
        mov    edi, ebx
        xor    eax, eax
        mov    al, 7
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        jb     SHORT loc_13_05B0
        push   esi
        push   ebx
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  0CDh, 020h, 066h, 001h, 001h, 000h
        mov    ebx, edi
        mov    eax, ecx
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        xor    edx, edx
        db  0CDh, 020h, 063h, 001h, 001h, 000h
        pop    ebx
        pop    esi
        db  0E8h, 0B5h, 0FAh, 0F2h, 0FFh
        db  003h, 03Dh, 004h, 00Ah, 000h, 000h
        mov    dword ptr [eax - 6], edi
        mov    dword ptr [edi], ebx
        mov    dword ptr [ebp + 1ch], ebx
        cmp    dword ptr [eax + 2ah], 0
        je     SHORT loc_13_05AB
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   1c8h
        push   eax
        push   dword ptr [esi + ebx + 0ch]
        db  0CDh, 020h, 07Dh, 001h, 001h, 000h
        add    esp, 0ch
        cmp    byte ptr [eax + 0c0h], 1
        jb     SHORT loc_13_05AB
        jne    SHORT loc_13_05A2
        dec    byte ptr [eax + 0c0h]
loc_13_05A2:
        inc    byte ptr [esi + ebx + 18h]
        or     byte ptr [esi + ebx + 11h], 2
loc_13_05AB:
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_13_05B0:
        xor    ecx, ecx
        xor    ebx, ebx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        test   eax, eax
        jne    SHORT loc_13_05C5
loc_13_05BE:
        xor    edx, edx
        mov    eax, 20000h
loc_13_05C5:
        mov    dword ptr [ebp + 1ch], eax
        mov    dword ptr [ebp + 14h], edx
        or     byte ptr [ebp + 2ch], 1
        ret
loc_13_05D0:
        or     byte ptr [ebp + 2ch], 1
        db  080h, 03Dh, 0E4h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_13_05E9
        db  0E8h, 097h, 006h, 0F3h, 0FFh
        db  0C6h, 005h, 0E4h, 000h, 000h, 000h, 0FFh
loc_13_05E9:
        db  00Fh, 0B7h, 005h, 0E0h, 000h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_13_062D
        db  0CDh, 020h, 06Dh, 000h, 001h, 000h
        mov    esi, eax
        push   0
        push   0
        push   eax
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        mov    ecx, edx
        or     ecx, eax
        je     SHORT loc_13_0645
        or     edx, 200h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_13_0645
        db  066h, 0A3h, 0E0h, 000h, 000h, 000h
loc_13_062D:
        mov    dword ptr [ebp + 14h], eax
        db  0A1h, 0D8h, 000h, 000h, 000h
        mov    dword ptr [ebp + 10h], eax
        db  0A1h, 0DCh, 000h, 000h, 000h
        mov    dword ptr [ebp + 18h], eax
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_13_0645:
        mov    eax, esi
        jmp    SHORT loc_13_062D
        db  000h, 000h, 000h
_PTEXT ends
        end
