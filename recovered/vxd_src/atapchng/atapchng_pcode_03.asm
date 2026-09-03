; atapchng_pcode_03.asm -- ATAPCHNG object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        db  005h, 0B0h, 005h, 0A2h, 000h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 56h], dl
        push   ebx
        mov    esi, dword ptr [ebx + 0ch]
        test   dword ptr [esi + 20h], 6000h
        jne    NEAR PTR loc_3_01F7
        cmp    dword ptr [esi + 0a6h], 61656469h
        jne    SHORT loc_3_0033
        cmp    dword ptr [esi + 0aah], 69706174h
        je     SHORT loc_3_003D
loc_3_0033:
        test   byte ptr [esi + 24h], 2
        je     NEAR PTR loc_3_01F7
loc_3_003D:
        cmp    byte ptr [esi + 74h], 1
        jne    NEAR PTR loc_3_01F7
        cmp    byte ptr [esi + 40h], 5
        jne    NEAR PTR loc_3_01F7
        cmp    byte ptr [esi + 7dh], 0
        jne    NEAR PTR loc_3_01F7
        xor    ecx, ecx
        call   sub_3_0447
        or     byte ptr [ebx + 6dh], 1
        mov    dword ptr [edi + 30h], 2a005ah
        mov    dword ptr [edi + 34h], 0
        mov    ecx, 1ch
        mov    dword ptr [edi + 38h], ecx
        mov    dword ptr [edi + 0ch], 248h
        call   sub_3_05A6
        jb     NEAR PTR loc_3_0202
loc_3_008E:
        xor    edx, edx
        mov    dword ptr [ebx + 10h], 0
        call   sub_3_056C
        mov    edx, dword ptr [ebx + 90h]
        cmp    byte ptr [edi + 3], 1
        jne    NEAR PTR loc_3_0209
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [eax + 8], 2ah
        jne    NEAR PTR loc_3_01FB
        movzx  ecx, byte ptr [eax + 0eh]
        mov    ch, cl
        and    cl, 0c0h
        cmp    cl, 80h
        jne    NEAR PTR loc_3_01FB
        and    ch, 20h
        mov    cl, byte ptr [eax + 0fh]
        or     cl, ch
        mov    dword ptr [edi + 30h], 0bdh
        mov    word ptr [edi + 38h], 800h
        mov    dword ptr [ebx + 7ch], 8
        mov    dword ptr [ebx + 10h], 0
        mov    dword ptr [edi + 10h], 8
        push   ebx
        db  0FFh, 015h, 040h, 005h, 000h, 000h
        add    esp, 4
        xor    edx, edx
        call   sub_3_056C
        cmp    byte ptr [edi + 3], 1
        jne    NEAR PTR loc_3_01FB
        mov    edx, dword ptr [ebx + 80h]
        mov    ch, byte ptr [edx + 5]
        call   sub_3_0217
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 1ch]
        mov    ebx, dword ptr [esp]
        sub    esp, 18h
        mov    edi, esp
        mov    word ptr [edi], 5
        mov    dword ptr [edi + 12h], eax
        mov    dword ptr [edi + 4], esi
        db  0C7h, 047h, 008h, 0D8h, 000h, 000h, 000h
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [edi + 16h], al
        push   ecx
        movzx  ecx, ch
        lea    eax, [ecx + ecx*4 + 21h]
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        movzx  ecx, byte ptr [esp + 1]
        or     eax, eax
        lea    ebx, [ecx + eax + 19h]
        mov    dword ptr [eax + 14h], ebx
        mov    dword ptr [eax + 10h], 0ffh
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax + 8], esi
        pop    ecx
        movzx  edx, cl
        and    edx, 4
        movzx  ecx, ch
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [edi + 10h], 0
        push   ecx
        push   edi
        lea    edi, [eax + 18h]
        mov    al, 0ffh
        rep stosb
        db  02Eh, 0FFh, 015h, 02Ch, 005h, 000h, 000h
        pop    ecx
        pop    ecx
        add    esp, 18h
        mov    eax, ecx
        dec    ecx
loc_3_01A7:
        push   eax
        sub    esp, 0ch
        mov    ebx, esp
        mov    word ptr [ebx], 1
        mov    word ptr [ebx + 4], 4a8h
        push   ebx
        db  0FFh, 015h, 02Ch, 005h, 000h, 000h
        mov    ebx, dword ptr [ebx + 6]
        add    esp, 10h
        push   ecx
        mov    ecx, 12ah
        cli
        push   esi
        push   dword ptr [ebx + 0ch]
        push   dword ptr [ebx + 70h]
        push   edi
        mov    edi, ebx
        rep movsd
        pop    edi
        pop    dword ptr [ebx + 70h]
        pop    dword ptr [ebx + 0ch]
        pop    esi
        mov    dword ptr [esi + 6ch], ebx
        sti
        pop    ecx
        mov    eax, dword ptr [esp]
        sub    eax, ecx
        mov    byte ptr [ebx + 7dh], al
        mov    dword ptr [ebx], ebx
        db  0E8h, 098h, 000h, 000h, 000h
        pop    eax
        loop   loc_3_01A7
loc_3_01F7:
        pop    ebx
        pop    esi
        pop    ebp
        ret
loc_3_01FB:
        call   sub_3_0217
        jmp    SHORT loc_3_01F7
loc_3_0202:
        call   sub_3_0234
        jmp    SHORT loc_3_01F7
loc_3_0209:
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_3_008E
        jmp    SHORT loc_3_01FB
sub_3_0217:
        mov    edx, dword ptr [ebx + 80h]
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], edx
        mov    word ptr [eax], 4
        push   eax
        db  0FFh, 015h, 02Ch, 005h, 000h, 000h
        add    esp, 0ch
sub_3_0234:
        db  0E8h, 094h, 002h, 0FDh, 0FFh
        ret
loc_3_023A:
        mov    esi, dword ptr [ebx + 0ch]
        cmp    byte ptr [esi + 74h], 1
        jne    SHORT loc_3_025B
        cmp    byte ptr [esi + 40h], 5
        jne    SHORT loc_3_025B
        mov    al, byte ptr [esi + 84h]
        and    al, 7
        cmp    al, 2
        jae    SHORT loc_3_025B
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
loc_3_025B:
        ret
loc_3_025C:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_026B
        mov    word ptr [ebx + 2], 0ffffh
loc_3_026B:
        ret
loc_3_026C:
        push   ebx
        inc    ebx
        push   ebx
        dec    ecx
        pop    esp
        db  000h
loc_3_0272:
        push   ebp
        db  073h, 065h
        db  072h, 044h
        jb     SHORT loc_3_02E2
        jbe    SHORT loc_3_02E0
        dec    esp
        je     NEAR PTR loc_3_02F3
        db  065h, 072h, 041h
        db  073h, 073h
        imul   esp, dword ptr [edi + 6eh], 746e656dh
        add    byte ptr [ebx - 729ff314h], al
        inc    ebx
        db  070h, 06Ah
        add    bh, bh
        jbe    SHORT loc_3_0308
        push   eax
        db  0CDh, 020h, 003h, 000h, 033h, 000h
        add    esp, 0ch
        lea    eax, [esp + 28h]
        push   0
        push   dword ptr [ebx + 70h]
        push   eax
        db  0CDh, 020h, 006h, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_3_0353
        mov    eax, dword ptr [esp + 28h]
        add    eax, 22h
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_0353
        push   eax
        db  08Dh, 035h, 06Ch, 002h, 000h, 000h
        mov    ecx, 5
loc_3_02E0:
        mov    edi, eax
loc_3_02E2:
        rep movsb
        lea    esi, [ebx + 8ah]
        mov    ecx, 19h
        rep movsb
        mov    al, 5ch
loc_3_02F3:
        stosb
        mov    eax, dword ptr [esp + 2ch]
        push   0
        push   eax
        push   edi
        push   dword ptr [ebx + 70h]
        db  0CDh, 020h, 007h, 000h, 033h, 000h
        add    esp, 10h
loc_3_0308:
        pop    esi
        xor    ecx, ecx
        or     eax, eax
        jne    SHORT loc_3_0353
        lea    edi, [esi + 1fh]
        mov    al, 5ch
        mov    ecx, dword ptr [esp + 28h]
        mov    edx, ecx
loc_3_031A:
        repne scasb
        jecxz  loc_3_0324
        mov    byte ptr [edi - 1], 26h
        jmp    SHORT loc_3_031A
loc_3_0324:
        lea    eax, [esi + 1fh]
        add    eax, edx
        mov    cx, word ptr [ebx + 7ch]
        add    cx, 3030h
        mov    word ptr [eax], cx
        lea    edi, [esp + 24h]
        push   0
        push   dword ptr [ebx + 70h]
        push   esi
        push   edi
        db  0CDh, 020h, 00Bh, 000h, 033h, 000h
        add    esp, 10h
        xor    ecx, ecx
        or     eax, eax
        jne    SHORT loc_3_0353
        mov    ecx, dword ptr [esp + 24h]
loc_3_0353:
        mov    dword ptr [ebx + 70h], ecx
        mov    word ptr [ebx + 48h], 0ffffh
        lea    ecx, [esp + 20h]
        sub    esp, 18h
        mov    eax, esp
        mov    word ptr [eax], 11h
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 0ch], 1
        mov    dword ptr [eax + 10h], 3
        mov    dword ptr [eax + 14h], ecx
        db  0C7h, 040h, 008h, 072h, 002h, 000h, 000h
        push   eax
        db  0FFh, 015h, 02Ch, 005h, 000h, 000h
        movzx  eax, word ptr [eax + 2]
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_3_03A4
        mov    ax, word ptr [esp + 20h]
        sub    ax, 4141h
        mov    word ptr [ebx + 48h], ax
loc_3_03A4:
        popad
        db  0E8h, 029h, 000h, 000h, 000h
        jae    SHORT loc_3_03B8
        call   sub_3_0412
        mov    esi, ebx
        call   sub_3_0412
loc_3_03B8:
        add    esp, 0ch
        ret
loc_3_03BC:
        inc    ecx
        jne    SHORT loc_3_0433
        outsd
        dec    ecx
        outsb
        jae    SHORT loc_3_0429
        db  072h, 074h
        dec    esi
        outsd
        je     SHORT loc_3_0433
        imul   sp, word ptr [ebx + 61h], 6974h
        outsd
        outsb
        add    byte ptr [ecx - 7dh], dl
        in     al, dx
        sbb    al, 8bh
        int3
        mov    word ptr [ecx], 11h
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 0ch], 3
        mov    dword ptr [ecx + 10h], 1
        lea    edx, [ecx + 18h]
        mov    dword ptr [ecx + 14h], edx
        db  0C7h, 041h, 008h, 0BCh, 003h, 000h, 000h
        push   ecx
        db  0FFh, 015h, 02Ch, 005h, 000h, 000h
        movzx  ecx, word ptr [ecx + 2]
        movzx  edx, byte ptr [edx]
        add    esp, 20h
        jecxz  loc_3_0410
        stc
loc_3_0410:
        pop    ecx
        ret
sub_3_0412:
        enter  0ch, 0
        pushad
        mov    dword ptr [ebp - 0ch], 1
        mov    edx, dword ptr [esi + 70h]
        lea    eax, [ebp - 8]
        mov    byte ptr [eax], 1
        push   0
loc_3_0429:
        push   1
        push   eax
        push   3
        db  068h, 0BCh, 003h, 000h, 000h
loc_3_0433:
        push   0
        push   edx
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        popad
        leave
        ret
        db  000h, 000h
loc_3_0444:
        push   ecx
        jmp    SHORT loc_3_044D
sub_3_0447:
        push   ebx
        jecxz  loc_3_0444
        push   dword ptr [ebx + 10h]
loc_3_044D:
        push   ecx
        call   sub_3_0482
        mov    eax, ecx
        pop    ecx
        or     edi, edi
        je     SHORT loc_3_047E
        mov    dword ptr [edi + 0a4h], edi
        pop    dword ptr [edi + 10h]
        mov    word ptr [edi + 68h], 12h
        mov    dword ptr [edi + 8], esi
        mov    dword ptr [edi + 4], esi
        mov    ebx, edi
        call   sub_3_0506
        pop    dword ptr [ebx + 90h]
        clc
        ret
loc_3_047E:
        pop    ebx
        pop    ebx
        stc
        ret
sub_3_0482:
        push   esi
        mov    esi, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        add    ecx, 64h
        sub    esp, 10h
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    cx, 106h
        add    cx, word ptr [esi + 80h]
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0eh], 2
        mov    word ptr [edi], 2
        push   edi
        db  0FFh, 015h, 02Ch, 005h, 000h, 000h
        mov    edi, dword ptr [edi + 0ah]
        add    esp, 14h
        mov    eax, edi
        or     edi, edi
        je     SHORT loc_3_0504
        add    edi, dword ptr [esi + 4]
        xor    eax, eax
        mov    word ptr [edi + 0ch], 0fh
        mov    word ptr [edi + 0eh], 0fh
        mov    dword ptr [edi + 64h], eax
        mov    dword ptr [edi + 0a8h], eax
        mov    dword ptr [edi + 7ch], eax
        mov    dword ptr [edi + 6ch], 8c09h
        pop    esi
        mov    al, byte ptr [esi + 14h]
        mov    byte ptr [edi + 0b1h], al
        mov    eax, edi
        add    eax, 0bch
        mov    dword ptr [edi + 0ach], eax
        lea    ecx, [edi + 0cch]
        mov    dword ptr [edi + 20h], ecx
        ret
loc_3_0504:
        pop    esi
        ret
sub_3_0506:
        push   edx
        mov    edi, eax
        mov    dword ptr [edi + 40h], ebx
        mov    dword ptr [edi], 40h
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 3], 0
        xor    eax, eax
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [edi + 44h], eax
        mov    dword ptr [edi + 20h], eax
        mov    byte ptr [edi + 4ch], al
        mov    byte ptr [edi + 8], al
        mov    byte ptr [edi + 9], al
        mov    dword ptr [edi + 18h], eax
        mov    dword ptr [edi + 14h], 0fh
        mov    dword ptr [edi + 0ch], 0
        mov    dword ptr [edi + 10h], eax
        mov    byte ptr [edi + 0ah], 0ch
        mov    al, byte ptr [esi + 75h]
        mov    byte ptr [edi + 5], al
        mov    al, byte ptr [esi + 7ch]
        mov    byte ptr [edi + 6], al
        mov    edx, edi
        add    edx, 50h
        mov    dword ptr [edi + 28h], edx
        movzx  eax, word ptr [esi + 80h]
        add    edx, eax
        mov    dword ptr [edi + 1ch], edx
        mov    byte ptr [edi + 0bh], 0eh
        pop    edx
        ret
sub_3_056C:
        push   esi
        push   edi
        push   ebx
        mov    ecx, dword ptr [ebx + 10h]
        jecxz  loc_3_057D
        mov    esi, dword ptr [ecx + 8]
        mov    esi, dword ptr [esi + 8]
        mov    dword ptr [ebx + 4], esi
loc_3_057D:
        mov    byte ptr [edi + 3], 0
        mov    eax, dword ptr [ebx + 80h]
        mov    dword ptr [edi + 18h], eax
        mov    cx, word ptr [ebx + 0eh]
        mov    word ptr [ebx + 0ch], cx
        xchg   esi, edi
        xchg   edi, ebx
        mov    dword ptr [edi + 0a4h], edi
        db  0FFh, 015h, 038h, 005h, 000h, 000h
        pop    ebx
        pop    edi
        pop    esi
        ret
sub_3_05A6:
        sub    esp, 10h
        mov    edx, esp
        mov    word ptr [edx + 4], cx
        mov    word ptr [edx + 0ah], 9
        mov    word ptr [edx], 3
        mov    byte ptr [edx + 0ch], 8
        push   edx
        db  0FFh, 015h, 02Ch, 005h, 000h, 000h
        mov    eax, dword ptr [edx + 6]
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_3_05F8
        mov    dword ptr [ebx + 80h], eax
        mov    dword ptr [ebx + 7ch], ecx
        mov    dword ptr [edi + 10h], ecx
        and    byte ptr [ebx + 6ch], 0fdh
        or     byte ptr [ebx + 6dh], 2
        push   eax
        push   ebx
        db  0FFh, 015h, 040h, 005h, 000h, 000h
        add    esp, 4
        or     al, al
        pop    eax
        je     SHORT loc_3_05F7
        or     dword ptr [ebx + 6ch], 40h
loc_3_05F7:
        ret
loc_3_05F8:
        stc
        ret
        db  026h, 041h
_PTEXT ends
        end
