; nwlink_lcode_01_02.asm -- NWLINK object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4EC4:
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 01Dh, 00Eh, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 048h, 000h
        or     eax, eax
        je     SHORT loc_1_4EFD
        push   0
        push   ebx
        push   ecx
        push   0
        push   0
        push   esp
        db  0CDh, 020h, 001h, 000h, 048h, 000h
        add    esp, 18h
loc_1_4EFD:
        db  0A3h, 04Ah, 05Ah, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_4F07
        ret
loc_1_4F07:
        db  0BBh, 06Ah, 059h, 000h, 000h
        mov    ecx, 8
loc_1_4F11:
        push   ecx
        push   ebx
        push   dword ptr [ebx + 18h]
        mov    edx, dword ptr [ebx + 8]
        db  00Fh, 0B7h, 004h, 055h, 00Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        mov    edx, dword ptr [ebx + 0ch]
        db  00Fh, 0B7h, 004h, 055h, 00Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        mov    edx, dword ptr [ebx]
        db  00Fh, 0B7h, 004h, 055h, 00Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        mov    edx, dword ptr [ebx + 4]
        db  00Fh, 0B7h, 004h, 055h, 00Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   dword ptr [ebx + 14h]
        push   0
        push   esp
        db  0FFh, 035h, 04Ah, 05Ah, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        pop    ebx
        pop    ecx
        mov    dword ptr [ebx + 10h], eax
        add    ebx, 1ch
        dec    cx
        jecxz  loc_1_4F80
        jmp    SHORT loc_1_4F11
loc_1_4F80:
        ret
loc_1_4F81:
        xor    eax, eax
        ret
sub_1_4F84:
        db  083h, 03Dh, 0C8h, 058h, 000h, 000h, 000h
        je     SHORT loc_1_4F94
        db  08Bh, 03Dh, 0ECh, 058h, 000h, 000h
        ret
loc_1_4F94:
        mov    eax, 4
        stc
        ret
sub_1_4F9B:
        push   ebp
        push   edx
        mov    ebp, esp
        push   0
        push   0
        push   0
        push   ebp
        call   sub_1_02E0
        add    esp, 10h
        pop    edx
        pop    ebp
        ret
loc_1_4FB1:
        push   edx
        mov    ebx, eax
        db  003h, 005h, 020h, 058h, 000h, 000h
        mov    eax, dword ptr [eax + 8]
        movzx  eax, word ptr [eax]
        mov    ecx, dword ptr [ebx + 0ch]
        add    eax, ecx
        shl    ecx, 10h
        add    eax, ecx
        mov    ecx, esp
        push   0
        push   eax
        push   ebx
        push   ecx
        call   sub_1_02E0
        add    esp, 10h
        pop    edx
        ret
loc_1_4FDB:
        push   0
        push   edx
        call   sub_1_041B
        add    esp, 8
        ret
loc_1_4FE7:
        push   esi
        push   edi
        mov    ebx, edi
        call   sub_1_4F84
        jb     SHORT loc_1_5028
        push   ebp
        push   0
        mov    ebp, esp
        push   edi
        push   ebp
        push   esi
        push   ebx
        call   sub_1_61D0
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_501D
        test   dword ptr [edi + 8], 2
        je     SHORT loc_1_5024
        mov    esi, ebx
        mov    ebx, edi
        call   sub_1_2365
        xor    eax, eax
        jmp    SHORT loc_1_5024
loc_1_501D:
        db  066h, 0FFh, 005h, 03Ch, 059h, 000h, 000h
loc_1_5024:
        pop    ecx
        xchg   cl, ch
        pop    ebp
loc_1_5028:
        pop    edi
        pop    esi
        ret
loc_1_502B:
        push   edi
        call   sub_1_410B
        pop    edi
        ret
loc_1_5033:
        push   esi
        call   sub_1_3FF7
        pop    esi
        ret
loc_1_503B:
        mov    byte ptr [esi + 9], 0f0h
        mov    byte ptr [esi + 8], 0
        ret
loc_1_5044:
        mov    byte ptr [esi + 9], 1
        mov    byte ptr [esi + 8], 0
        ret
loc_1_504D:
        mov    byte ptr [esi + 9], 0ffh
        mov    byte ptr [esi + 8], 0
        ret
loc_1_5056:
        or     dword ptr [esi + 0ch], 40h
        cmp    byte ptr [esi + 8], 0
        jne    SHORT loc_1_50BB
loc_1_5060:
        mov    ebx, eax
        push   dword ptr [esi + 0ah]
        call   sub_1_42A4
        pop    edx
        jb     SHORT loc_1_504D
        cmp    dword ptr [eax], ebx
        jne    SHORT loc_1_504D
        mov    edi, eax
        push   esi
        call   sub_1_4488
        pop    edx
        or     eax, eax
        je     SHORT loc_1_5044
        lea    edi, [edi + 0ch]
        lea    edx, [eax + 58h]
loc_1_5084:
        mov    ecx, edx
        xchg   dword ptr [edi], ecx
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], edi
        mov    byte ptr [esi + 8], 0fdh
        mov    edi, eax
loc_1_5096:
        movzx  eax, word ptr [ebp + 1ch]
        or     eax, eax
        je     SHORT loc_1_50B8
        imul   eax, eax, 37h
loc_1_50A1:
        mov    edx, edi
        db  0BEh, 0D9h, 048h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        mov    dword ptr [edi + 54h], esi
        db  0FFh, 005h, 02Ah, 059h, 000h, 000h
        ret
loc_1_50B8:
        inc    eax
        jmp    SHORT loc_1_50A1
loc_1_50BB:
        push   eax
        push   ecx
        push   dword ptr [esi + 0ah]
        call   sub_1_42A4
        pop    edi
        mov    edi, eax
        pop    ecx
        pop    eax
        jb     SHORT loc_1_504D
        lea    edx, [edi + 0ch]
        mov    ebx, edx
loc_1_50D1:
        mov    ebx, dword ptr [ebx]
        cmp    edx, ebx
        je     SHORT loc_1_5060
        cmp    dword ptr [ebx - 14h], esi
        je     SHORT loc_1_50DE
        jmp    SHORT loc_1_50D1
loc_1_50DE:
        lea    edi, [ebx - 58h]
        mov    esi, dword ptr [edi + 54h]
        mov    ebx, eax
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        jmp    SHORT loc_1_5096
loc_1_50EE:
        or     dword ptr [esi + 0ch], 840h
        cmp    byte ptr [esi + 8], 0
        jne    SHORT loc_1_511C
loc_1_50FB:
        mov    ebx, eax
        push   esi
        call   sub_1_4488
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_1_5044
        mov    dword ptr [eax + 40h], ebx
        db  0BFh, 09Ch, 058h, 000h, 000h
        lea    edx, [eax + 58h]
        jmp    NEAR PTR loc_1_5084
loc_1_511C:
        db  0BAh, 09Ch, 058h, 000h, 000h
        mov    ebx, edx
loc_1_5123:
        mov    ebx, dword ptr [ebx]
        cmp    edx, ebx
        je     SHORT loc_1_50FB
        cmp    dword ptr [ebx - 14h], esi
        je     SHORT loc_1_50DE
        jmp    SHORT loc_1_5123
sub_1_5130:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        sub    ebx, 20h
        xor    esi, esi
        xchg   dword ptr [ebx + 1ch], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        xor    esi, esi
        xchg   dword ptr [ebx + 0ch], esi
        mov    byte ptr [esi + 9], 0fch
        mov    byte ptr [esi + 8], 0
        mov    edx, dword ptr [esi]
        call   sub_1_48D9
        pop    esi
        pop    ebx
        ret
loc_1_5165:
        db  0B8h, 094h, 058h, 000h, 000h
        mov    ecx, eax
loc_1_516C:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, eax
        je     SHORT loc_1_5187
        cmp    dword ptr [ecx - 8], esi
        je     SHORT loc_1_5179
        jmp    SHORT loc_1_516C
loc_1_5179:
        push   0
        push   0
        push   esi
        call   sub_1_0562
        add    esp, 0ch
        ret
loc_1_5187:
        mov    eax, 0ffh
        ret
sub_1_518D:
        push   ebx
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        xor    edx, edx
        mov    ebx, 37h
        div    ebx
        pop    ebx
        ret
sub_1_519F:
        push   esi
        push   edi
        call   sub_1_4F84
        jb     SHORT loc_1_51BC
        mov    edx, edi
        mov    edi, esi
        lea    esi, [edx + 18h]
        movsd
        movsd
        movsw
        movzx  ecx, word ptr [edx + 24h]
        pop    edi
        pop    esi
        xor    eax, eax
        ret
loc_1_51BC:
        xor    eax, eax
        mov    edi, esi
        stosd
        stosd
        stosw
        pop    edi
        pop    esi
        inc    eax
        ret
loc_1_51C8:
        xor    eax, eax
        ret
sub_1_51CB:
        push   edi
        call   sub_1_4F84
        jb     SHORT loc_1_51E6
        mov    edi, dword ptr [edi + 48h]
        mov    eax, dword ptr [edi + 1ch]
        sub    eax, 40h
        mov    edx, 0ah
        mov    ecx, dword ptr [edi + 20h]
        pop    edi
        ret
loc_1_51E6:
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        pop    edi
        ret
loc_1_51EE:
        push   edi
        call   sub_1_4F84
        jb     SHORT loc_1_51E6
        mov    eax, dword ptr [edi + 0ch]
        mov    ecx, 0ah
        pop    edi
        ret
loc_1_5200:
        call   sub_1_51CB
        ret
sub_1_5206:
        push   esi
        mov    edx, dword ptr [esp + 8]
        movzx  edx, word ptr [edx]
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 10h]
        call   sub_1_5224
        mov    esi, dword ptr [esp + 8]
        mov    word ptr [esi], dx
        pop    esi
        ret
sub_1_5224:
        push   eax
        call   sub_1_4F9B
        or     eax, eax
        jne    SHORT loc_1_5250
        push   edx
        call   sub_1_42A4
        add    esp, 4
        or     dword ptr [eax + 18h], 8
        mov    dword ptr [eax + 30h], esi
        pop    dword ptr [eax + 34h]
        mov    eax, dword ptr [eax + 8]
        db  003h, 005h, 064h, 058h, 000h, 000h
        or     byte ptr [eax], 2
        xor    eax, eax
        ret
loc_1_5250:
        pop    edx
        ret
loc_1_5252:
        xor    eax, eax
        db  039h, 005h, 0C8h, 058h, 000h, 000h
        je     SHORT loc_1_5282
        mov    al, 0ffh
        mov    word ptr [ebp + 10h], 30ah
        jmp    SHORT loc_1_5276
loc_1_5266:
        xor    eax, eax
        db  039h, 005h, 0C8h, 058h, 000h, 000h
        je     SHORT loc_1_5282
        mov    al, 0ffh
        mov    bx, 30ah
loc_1_5276:
        db  08Bh, 00Dh, 028h, 058h, 000h, 000h
        db  08Bh, 015h, 058h, 058h, 000h, 000h
loc_1_5282:
        ret
loc_1_5283:
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 8], 0
        ret
loc_1_528B:
        call   sub_1_4F84
        jb     SHORT loc_1_5283
        mov    dword ptr [esi + 10h], edi
        mov    edx, dword ptr [esi + 24h]
        movzx  edi, dx
        test   dword ptr [esi + 0ch], 2
        je     SHORT loc_1_52B0
        xor    dh, dh
        shr    edx, 0ch
        add    edx, dword ptr [eax + 4]
        add    edi, edx
        jmp    SHORT loc_1_52C2
loc_1_52B0:
        shr    edx, 10h
        push   0
        push   edx
        push   eax
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        add    edi, eax
loc_1_52C2:
        mov    eax, dword ptr [ebp + 1ch]
loc_1_52C5:
        push   0
        mov    edx, esp
        push   edi
        movzx  ecx, ah
        movzx  eax, al
        push   edx
        push   ecx
        push   eax
        push   esi
        call   sub_1_0652
        add    esp, 14h
        pop    edx
        ret
loc_1_52DE:
        push   eax
        call   sub_1_4F84
        pop    eax
        jb     SHORT loc_1_5283
        mov    dword ptr [esi + 10h], edi
        mov    edi, dword ptr [esi + 24h]
        jmp    SHORT loc_1_52C5
loc_1_52EF:
        mov    eax, dword ptr [ebp + 1ch]
loc_1_52F2:
        or     dword ptr [esi + 0ch], 500h
        movzx  ecx, ah
        movzx  eax, al
        push   ecx
        push   eax
        push   esi
        call   sub_1_091C
        add    esp, 0ch
        ret
loc_1_530B:
        or     dword ptr [esi + 0ch], 80h
        push   edx
        call   sub_1_42D3
        pop    edx
        jb     SHORT loc_1_532C
        test   dword ptr [eax + 4], 40000h
        jne    SHORT loc_1_532C
        call   sub_1_38AA
        xor    eax, eax
        ret
loc_1_532C:
        mov    byte ptr [esi + 9], 0eeh
        mov    byte ptr [esi + 8], 0
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_1_5343
        mov    eax, esi
        mov    ecx, dword ptr [eax]
        mov    dl, 0ffh
        call   dword ptr [eax + 18h]
loc_1_5343:
        xor    eax, eax
        ret
sub_1_5346:
        db  066h, 0FFh, 005h, 056h, 059h, 000h, 000h
        push   edx
        call   sub_1_42D3
        pop    edx
        jb     SHORT loc_1_5369
        or     dword ptr [eax + 4], 400000h
        push   1
        push   eax
        call   sub_1_0BD0
        add    esp, 8
        ret
loc_1_5369:
        push   edx
        call   sub_1_42D3
        pop    edx
        jb     SHORT loc_1_53B7
        mov    ebx, eax
        mov    edi, esi
        mov    edx, esi
        lea    esi, [ebx]
        movsd
        lea    esi, [ebx + 60h]
        movsd
        movsd
        mov    eax, dword ptr [ebx + 68h]
        stosd
        mov    ax, word ptr [ebx + 5ah]
        stosw
        lea    esi, [ebx + 38h]
        movsd
        movsw
        lea    esi, [ebx + 44h]
        movsd
        movsd
        movsd
        mov    eax, dword ptr [ebx + 30h]
        xchg   ah, al
        stosw
        mov    eax, dword ptr [ebx + 1ch]
        xchg   ah, al
        stosw
        mov    ax, word ptr [ebx + 6eh]
        xchg   ah, al
        shl    eax, 10h
        mov    ax, word ptr [ebx + 6ch]
        xchg   ah, al
        stosd
        xor    eax, eax
        ret
loc_1_53B7:
        mov    eax, 0eeh
        ret
loc_1_53BD:
        or     dword ptr [esi + 0ch], 80h
        call   sub_1_3FF7
        ret
loc_1_53CA:
        cmp    dword ptr [esp + 0ch], -1
        jne    SHORT loc_1_53F0
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax + 1ch], 111dh
        jne    SHORT loc_1_53F0
        mov    edx, dword ptr [esp + 4]
        mov    edx, dword ptr [edx + 8]
        push   ebx
        mov    ebx, 4
        call   sub_1_53F6
        pop    ebx
loc_1_53F0:
        db  0FFh, 025h, 084h, 058h, 000h, 000h
sub_1_53F6:
        push   esi
        push   edi
        db  08Bh, 035h, 060h, 058h, 000h, 000h
        db  08Bh, 03Dh, 040h, 058h, 000h, 000h
        mov    edi, dword ptr [edi]
        inc    edi
loc_1_5407:
        dec    edi
        je     SHORT loc_1_5449
        lodsd
        or     edx, edx
        jne    SHORT loc_1_541A
        test   dword ptr [eax + 18h], 1
        jne    SHORT loc_1_5435
        jmp    SHORT loc_1_541F
loc_1_541A:
        cmp    dword ptr [ebx + eax], edx
        jne    SHORT loc_1_5407
loc_1_541F:
        mov    cx, word ptr [eax + 14h]
        push   edx
        push   0
        push   ecx
        call   sub_1_041B
        add    esp, 8
        pop    edx
        sub    esi, 4
        jmp    SHORT loc_1_5407
loc_1_5435:
        push   edx
        mov    cx, word ptr [eax + 14h]
        push   ecx
        call   sub_1_4300
        add    esp, 4
        pop    edx
        sub    esi, 4
        jmp    SHORT loc_1_5407
loc_1_5449:
        pop    edi
        pop    esi
        ret
sub_1_544C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 10h], eax
        mov    esi, dword ptr [ebp + 0ch]
        mov    ax, word ptr [esi]
        mov    dword ptr [ebx + 14h], eax
        db  08Bh, 00Dh, 014h, 059h, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 14h]
        mov    word ptr [esi], ax
        movzx  eax, word ptr [ebx + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_5508:
        xor    al, 5eh
        add    byte ptr [eax], al
        push   ecx
        pop    esi
        add    byte ptr [eax], al
        jno    SHORT loc_1_5570
        add    byte ptr [eax], al
        xchg   ecx, eax
        pop    esi
        add    byte ptr [eax], al
        pop    edi
        add    dword ptr [eax], eax
        db  000h, 0B9h, 05Fh, 000h, 000h, 002h
        pushad
        add    byte ptr [eax], al
        sbb    dword ptr [eax], esp
        add    byte ptr [edi], cl
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 054h, 055h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 075h, 055h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xchg   esi, eax
        push   ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    bh, 55h
        add    byte ptr [eax], al
loc_1_5554:
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
loc_1_5570:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5596:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_55D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_55DC:
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
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
        db  000h, 070h, 056h
        add    byte ptr [eax], al
        je     SHORT loc_1_56D4
        add    byte ptr [eax], al
loc_1_5680:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5684:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5688:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_568C:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_5690:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5694:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5698:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_569C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_56F0:
        mov    cl, 4fh
        add    byte ptr [eax], al
        fisttp dword ptr [edi]
        db  000h, 0E7h
        dec    edi
        add    byte ptr [eax], al
        or     eax, dword ptr [ecx]
        db  000h, 0F7h
        aas
        add    byte ptr [eax], al
        push   esi
        push   eax
        add    byte ptr [eax], al
        push   ecx
        add    byte ptr [eax], al
        out    dx, al
        push   eax
        add    byte ptr [eax], al
        lea    edx, [ecx]
        db  000h, 09Fh, 051h, 000h, 000h, 098h
        dec    edx
        add    byte ptr [eax], al
        enter  51h, 0
        db  081h, 04Fh, 000h, 000h, 000h, 052h, 000h
        db  000h, 081h, 04Fh, 000h, 000h, 00Bh
        inc    ecx
        add    byte ptr [eax], al
        push   edx
        push   edx
        add    byte ptr [eax], al
        mov    edx, dword ptr [edx]
        db  000h, 0EFh
        push   edx
        add    byte ptr [eax], al
        or     edx, dword ptr [ebx]
        db  000h, 046h, 053h
        add    byte ptr [eax], al
        db  069h, 053h, 000h, 000h, 01Bh, 039h, 000h
        db  000h, 0BDh, 053h, 000h, 000h, 081h
        dec    edi
        add    byte ptr [eax], al
        db  081h, 04Fh, 000h, 000h, 070h, 036h, 000h
        db  000h, 081h, 04Fh, 000h, 000h, 081h
        dec    edi
        add    byte ptr [eax], al
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h
        db  000h, 081h, 04Fh, 000h, 000h, 081h
        dec    edi
        add    byte ptr [eax], al
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h
        add    byte ptr [esp + eax], al
        add    dword ptr [ebx], eax
        pop    es
        add    eax, dword ptr [4010403h]
        add    dword ptr [eax], eax
        add    al, 4
        add    eax, dword ptr [eax]
        add    eax, dword ptr [ebx]
        add    eax, dword ptr [eax]
        add    dword ptr [ebx], eax
        add    eax, dword ptr [eax]
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 040h, 03Ch, 04Bh, 000h, 000h, 00Bh, 04Eh, 000h, 000h
        db  058h, 030h, 000h, 000h, 052h, 04Bh, 000h, 000h, 0CAh, 04Bh, 000h, 000h, 074h, 030h, 000h, 000h
        db  000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 087h, 004h, 001h, 000h, 000h, 000h, 04Eh, 057h, 04Ch, 049h
        db  04Eh, 04Bh, 020h, 020h, 0F6h, 05Fh, 000h, 054h, 008h, 04Bh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A8h, 057h, 000h, 000h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_5824:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5828:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_582C:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5830:
        inc    eax
        add    byte ptr [eax], al
        db  000h
loc_1_5834:
        add    al, 0
        add    byte ptr [eax], al
loc_1_5838:
        add    al, 0
        add    byte ptr [eax], al
loc_1_583C:
        fadd   qword ptr [eax]
        add    byte ptr [eax], al
loc_1_5840:
        push   eax
        pop    eax
        add    byte ptr [eax], al
loc_1_5844:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5848:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_584C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5850:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5854:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5858:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_585C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5860:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5864:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5868:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_586C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5870:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5874:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5878:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_587C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5884:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5888:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_588C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5890:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5894:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_589C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5900:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5904:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5908:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_590C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5910:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5914:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5918:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_591C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5920:
        add    byte ptr [eax], al
loc_1_5922:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5926:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_592A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5930:
        add    byte ptr [eax], al
loc_1_5932:
        add    byte ptr [eax], al
loc_1_5934:
        add    byte ptr [eax], al
loc_1_5936:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_593A:
        add    byte ptr [eax], al
loc_1_593C:
        add    byte ptr [eax], al
loc_1_593E:
        add    byte ptr [eax], al
loc_1_5940:
        add    byte ptr [eax], al
loc_1_5942:
        add    byte ptr [eax], al
loc_1_5944:
        add    byte ptr [eax], al
loc_1_5946:
        add    byte ptr [eax], al
loc_1_5948:
        add    byte ptr [eax], al
loc_1_594A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_594E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5952:
        add    byte ptr [eax], al
loc_1_5954:
        add    byte ptr [eax], al
loc_1_5956:
        add    byte ptr [eax], al
loc_1_5958:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_595C:
        add    byte ptr [eax], al
loc_1_595E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5962:
        add    byte ptr [eax], al
loc_1_5964:
        add    byte ptr [eax], al
loc_1_5966:
        add    byte ptr [eax], al
loc_1_5968:
        add    byte ptr [eax], al
loc_1_596A:
        push   es
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        sbb    al, 59h
        add    byte ptr [eax], al
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        and    bl, byte ptr [ecx]
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        or     eax, 0e000000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        pop    ecx
        add    byte ptr [eax], al
        sldt   word ptr [eax]
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 050h, 058h
        add    byte ptr [eax], al
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        dec    edx
        pop    ecx
        add    byte ptr [eax], al
        adc    eax, 16000000h
        add    byte ptr [eax], al
        add    byte ptr [edi], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        pop    esi
        pop    ecx
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        sbb    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    eax, 1b000056h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
        sbb    eax, 3000000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0C8h
        push   esi
        add    byte ptr [eax], al
loc_1_5A4A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  09Bh, 04Fh, 000h, 000h, 0DBh, 04Fh, 000h, 000h, 0E7h, 04Fh, 000h, 000h, 02Bh, 050h, 000h, 000h
        db  033h, 050h, 000h, 000h, 03Bh, 050h, 000h, 000h, 065h, 051h, 000h, 000h, 03Bh, 050h, 000h, 000h
        db  08Dh, 051h, 000h, 000h, 09Fh, 051h, 000h, 000h, 098h, 04Ah, 000h, 000h, 0C8h, 051h, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 0CBh, 051h, 000h, 000h, 024h, 052h, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  066h, 052h, 000h, 000h, 0DEh, 052h, 000h, 000h, 0F2h, 052h, 000h, 000h, 00Bh, 053h, 000h, 000h
        db  046h, 053h, 000h, 000h, 069h, 053h, 000h, 000h, 01Bh, 039h, 000h, 000h, 0BDh, 053h, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 0EEh, 051h, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  0DFh, 04Dh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h
        db  081h, 04Fh, 000h, 000h, 081h, 04Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_5B70:
        db  08Bh, 00Dh, 040h, 07Ch, 000h, 000h
        db  0A1h, 044h, 07Ch, 000h, 000h
        mov    word ptr [ecx + 0ah], ax
        db  0A1h, 040h, 07Ch, 000h, 000h
        mov    dword ptr [eax + 0ch], 100h
        db  0A1h, 040h, 07Ch, 000h, 000h
        push   1
        mov    dword ptr [eax + 4], 1
        db  0A1h, 040h, 07Ch, 000h, 000h
        push   0
        db  0C7h, 040h, 018h, 072h, 000h, 000h, 000h
        db  0A1h, 040h, 07Ch, 000h, 000h
        mov    word ptr [eax + 22h], 0
        db  0FFh, 035h, 040h, 07Ch, 000h, 000h
        call   sub_1_091C
        db  0A1h, 040h, 07Ch, 000h, 000h
        add    esp, 0ch
        cmp    byte ptr [eax + 8], 0
        je     SHORT loc_1_5BD2
        db  083h, 00Dh, 048h, 07Ch, 000h, 000h, 002h
loc_1_5BD2:
        ret
sub_1_5BD3:
        sub    esp, 4
        push   ebx
        push   esi
        mov    ebx, 1
        db  0F6h, 005h, 048h, 07Ch, 000h, 000h, 006h
        jne    SHORT loc_1_5BEB
        call   sub_1_5B70
loc_1_5BEB:
        push   -1
        push   0
        lea    eax, [esp + 10h]
        push   58h
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 8], 0
        je     NEAR PTR loc_1_5CD3
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 8]
        add    esi, 2ah
        mov    word ptr [eax + 0ah], 5604h
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esp + 8]
        db  0C7h, 040h, 018h, 0B8h, 002h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        push   6
        mov    word ptr [eax + 22h], 1
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [eax + 28h], 24h
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 24h], esi
        mov    eax, dword ptr [esp + 1ch]
        add    eax, 1ch
        push   eax
        mov    eax, dword ptr [esp + 10h]
        add    eax, 1ch
        push   eax
        call   sub_1_4AF3
        add    esp, 0ch
        mov    eax, dword ptr [esp + 14h]
        push   0ch
        add    eax, 12h
        push   eax
        lea    eax, [esi + 6]
        push   eax
        call   sub_1_4AF3
        add    esp, 0ch
        mov    byte ptr [esi + 5], 0
        mov    byte ptr [esi + 1eh], 1
        mov    byte ptr [esi + 1fh], 1
        db  0A1h, 044h, 07Ch, 000h, 000h
        sub    ecx, ecx
        mov    word ptr [esi + 20h], ax
        mov    byte ptr [esi + 23h], 0
        sub    edx, edx
loc_1_5CA0:
        mov    eax, ecx
        inc    ecx
        db  039h, 014h, 085h, 0C8h, 054h, 000h, 000h
        je     SHORT loc_1_5CB1
        mov    byte ptr [esi + ebx + 23h], cl
        inc    ebx
loc_1_5CB1:
        cmp    ecx, 10h
        jb     SHORT loc_1_5CA0
        mov    byte ptr [esi + 22h], bl
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 10h]
        dec    bx
        add    word ptr [eax + 28h], bx
        push   dword ptr [esp + 0ch]
        call   sub_1_4092
        add    esp, 8
loc_1_5CD3:
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_5CD9:
        db  083h, 00Dh, 048h, 07Ch, 000h, 000h, 004h
        db  083h, 025h, 048h, 07Ch, 000h, 000h, 0FDh
        push   -1
        push   0
        push   2f8h
        db  068h, 03Ch, 07Ch, 000h, 000h
        call   sub_1_2468
        db  083h, 03Dh, 03Ch, 07Ch, 000h, 000h, 000h
        jne    SHORT loc_1_5D0F
        db  083h, 025h, 048h, 07Ch, 000h, 000h, 0FBh
        jmp    NEAR PTR loc_1_5DC6
loc_1_5D0F:
        db  0A1h, 03Ch, 07Ch, 000h, 000h
        mov    dword ptr [eax], 2
        db  08Bh, 00Dh, 03Ch, 07Ch, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 03Ch, 07Ch, 000h, 000h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax + 2d4h], 0
        db  08Bh, 015h, 03Ch, 07Ch, 000h, 000h
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 8], eax
        db  0A1h, 03Ch, 07Ch, 000h, 000h
        shr    dword ptr [eax + 8], 10h
        db  08Bh, 015h, 03Ch, 07Ch, 000h, 000h
        db  0A1h, 044h, 07Ch, 000h, 000h
        mov    word ptr [edx + 1ah], ax
        db  0A1h, 03Ch, 07Ch, 000h, 000h
        mov    byte ptr [eax + 18h], 0
        db  0A1h, 03Ch, 07Ch, 000h, 000h
        mov    dword ptr [eax + 14h], 0
        db  0A1h, 03Ch, 07Ch, 000h, 000h
        mov    word ptr [eax + 32h], 1
        db  0A1h, 03Ch, 07Ch, 000h, 000h
        db  08Bh, 015h, 03Ch, 07Ch, 000h, 000h
        add    eax, 3ah
        mov    dword ptr [edx + 34h], eax
        db  0C7h, 041h, 018h, 07Dh, 000h, 000h, 000h
        mov    word ptr [ecx + 22h], 1
        mov    word ptr [ecx + 28h], 70h
        db  0A1h, 03Ch, 07Ch, 000h, 000h
        add    eax, 264h
        mov    dword ptr [ecx + 24h], eax
        mov    dword ptr [ecx + 0ch], 80h
        mov    dword ptr [ecx + 14h], 0
        push   ecx
        call   sub_1_3FB4
        add    esp, 4
loc_1_5DC6:
        ret
sub_1_5DC7:
        push   esi
        db  08Bh, 035h, 03Ch, 07Ch, 000h, 000h
        cmp    byte ptr [esi + 18h], 0
        je     SHORT loc_1_5DDF
        push   dword ptr [esi + 8]
        call   sub_1_014E
        add    esp, 4
loc_1_5DDF:
        mov    eax, dword ptr [esi]
        test   al, 4
        je     SHORT loc_1_5DEC
        or     eax, 30h
        mov    dword ptr [esi], eax
        jmp    SHORT loc_1_5E32
loc_1_5DEC:
        test   al, 8
        je     SHORT loc_1_5E0A
        push   0
        db  066h, 0A1h, 0C0h, 054h, 000h, 000h
        push   eax
        call   sub_1_041B
        add    esp, 8
        db  066h, 0C7h, 005h, 0C0h, 054h, 000h, 000h, 000h, 000h
loc_1_5E0A:
        push   0
        push   2f8h
        db  0FFh, 035h, 03Ch, 07Ch, 000h, 000h
        call   sub_1_246E
        db  0C7h, 005h, 03Ch, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 025h, 048h, 07Ch, 000h, 000h, 0FBh
        call   sub_1_5B70
loc_1_5E32:
        pop    esi
        ret
loc_1_5E34:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax], 0
        mov    byte ptr [eax + 5], 3
        mov    byte ptr [eax + 6], 0ah
        mov    byte ptr [eax + 7], 3
        mov    byte ptr [eax + 8], 0ah
        mov    eax, 9
        ret
loc_1_5E51:
        mov    eax, dword ptr [esp + 8]
        push   22h
        mov    byte ptr [eax], 0
        db  068h, 01Ch, 059h, 000h, 000h
        add    eax, 5
        push   eax
        call   sub_1_4AF3
        add    esp, 0ch
        mov    eax, 27h
        ret
loc_1_5E71:
        mov    eax, dword ptr [esp + 8]
        push   2ch
        mov    byte ptr [eax], 0
        db  068h, 03Eh, 059h, 000h, 000h
        add    eax, 5
        push   eax
        call   sub_1_4AF3
        add    esp, 0ch
        mov    eax, 31h
        ret
loc_1_5E91:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 18h]
        movzx  eax, word ptr [edi + 14h]
        or     eax, eax
        mov    dword ptr [esi + 2d8h], eax
        jne    SHORT loc_1_5EC5
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [eax], 0
        mov    word ptr [eax + 5], 0
        mov    eax, 7
        jmp    NEAR PTR loc_1_5FB1
loc_1_5EC5:
        push   -1
        push   0
        lea    eax, [esp + 18h]
        push   22eh
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_5EF0
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [eax], 0ffh
        mov    eax, 7
        jmp    NEAR PTR loc_1_5FB1
loc_1_5EF0:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], esi
        mov    ebx, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 8], 1
        mov    dword ptr [ebx + 10h], 0
        db  0C7h, 043h, 01Ch, 084h, 001h, 000h, 000h
        add    ebx, 4
        db  0A1h, 044h, 07Ch, 000h, 000h
        mov    word ptr [ebx + 0ah], ax
        mov    word ptr [ebx + 22h], 1
        lea    ebp, [ebx + 2ah]
        mov    dword ptr [ebx + 24h], ebp
        mov    ax, word ptr [edi + 18h]
        mov    word ptr [ebx + 28h], ax
        push   0ch
        lea    eax, [edi + 2]
        push   eax
        lea    eax, [ebp + 6]
        push   eax
        call   sub_1_4AF3
        add    esp, 0ch
        push   6
        lea    eax, [edi + 0eh]
        push   eax
        lea    eax, [ebx + 1ch]
        push   eax
        call   sub_1_4AF3
        add    esp, 0ch
        mov    byte ptr [ebp + 5], 0
        or     dword ptr [esi], 4
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 2d4h], eax
        movzx  eax, byte ptr [edi + 17h]
        mov    dword ptr [esi + 2e0h], eax
        movzx  eax, byte ptr [edi + 17h]
        mov    dword ptr [esi + 2dch], eax
        movzx  eax, word ptr [edi + 18h]
        mov    dword ptr [esi + 2e4h], eax
        movsx  eax, word ptr [edi + 1ah]
        mov    dword ptr [esi + 2e8h], eax
        push   dword ptr [esi + 4]
        movzx  eax, byte ptr [edi + 16h]
        imul   eax, eax, 37h
        mov    dword ptr [esi + 2ech], eax
        mov    dword ptr [esi + 2f0h], 0
        push   ebx
        call   sub_1_4092
        add    esp, 8
        sub    eax, eax
loc_1_5FB1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_5FB9:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    byte ptr [esi], 0
        db  0C7h, 005h, 0C4h, 054h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0F6h, 005h, 048h, 07Ch, 000h, 000h, 008h
        jne    SHORT loc_1_5FF1
        push   1
        push   0
        db  068h, 0AFh, 002h, 000h, 000h
        db  068h, 0C0h, 054h, 000h, 000h
        call   sub_1_5206
        add    esp, 10h
        db  083h, 00Dh, 048h, 07Ch, 000h, 000h, 008h
loc_1_5FF1:
        db  066h, 0A1h, 0C0h, 054h, 000h, 000h
        mov    word ptr [esi + 5], ax
        mov    eax, 7
        pop    esi
        ret
loc_1_6002:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax], 0
        db  08Bh, 00Dh, 0C4h, 054h, 000h, 000h
        mov    word ptr [eax + 5], cx
        mov    eax, 7
        ret
loc_1_6019:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax], 0ffh
        mov    eax, 5
        ret
loc_1_6026:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        movzx  eax, byte ptr [eax + 1]
        or     eax, eax
        je     SHORT loc_1_6047
        cmp    eax, 1
        je     SHORT loc_1_6098
        mov    edx, dword ptr [esp + 10h]
        mov    eax, 5
        mov    byte ptr [edx], 0ffh
        jmp    SHORT loc_1_60AD
loc_1_6047:
        mov    edx, dword ptr [esp + 10h]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 38h
        rep stosd
        stosw
        db  08Bh, 035h, 0ECh, 058h, 000h, 000h
        lea    edi, [edx + 9]
        add    esi, 1ch
        movsd
        movsw
        db  0A1h, 0ECh, 058h, 000h, 000h
        db  0BEh, 04Ch, 07Ch, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        sub    ax, 40h
        mov    word ptr [edx + 11h], ax
        db  0A1h, 0ECh, 058h, 000h, 000h
        lea    edi, [edx + 26h]
        mov    ax, word ptr [eax + 24h]
        mov    word ptr [edx + 15h], ax
        movsd
        movsd
        mov    eax, 0e2h
        movsd
        movsd
        movsb
        jmp    SHORT loc_1_60AD
loc_1_6098:
        mov    edi, dword ptr [esp + 10h]
        sub    eax, eax
        mov    ecx, 0ah
        rep stosd
        stosw
        stosb
        mov    eax, 2bh
loc_1_60AD:
        pop    edi
        pop    esi
        ret
sub_1_60B0:
        sub    esp, 4
        push   esi
        mov    word ptr [esp + 6], 5604h
        push   -1
        push   0
        push   133h
        db  068h, 040h, 07Ch, 000h, 000h
        call   sub_1_2468
        db  083h, 03Dh, 040h, 07Ch, 000h, 000h, 000h
        je     NEAR PTR loc_1_61C0
        db  08Bh, 035h, 040h, 07Ch, 000h, 000h
        mov    dword ptr [esi + 2eh], 1
        db  0C7h, 046h, 042h, 02Ch, 000h, 000h, 000h
        mov    word ptr [esi + 34h], 5604h
        mov    word ptr [esi + 4ch], 1
        mov    word ptr [esi + 52h], 0dfh
        lea    eax, [esi + 54h]
        mov    dword ptr [esi + 4eh], eax
        add    esi, 2ah
        push   22h
        push   0
        db  068h, 01Ch, 059h, 000h, 000h
        call   sub_1_4AE2
        add    esp, 0ch
        push   2ch
        push   0
        db  068h, 03Eh, 059h, 000h, 000h
        call   sub_1_4AE2
        db  0A1h, 024h, 058h, 000h, 000h
        db  066h, 0A3h, 030h, 059h, 000h, 000h
        db  0A1h, 028h, 058h, 000h, 000h
        db  066h, 0A3h, 03Eh, 059h, 000h, 000h
        add    esp, 0ch
        push   1
        push   0
        lea    eax, [esp + 0eh]
        push   0
        push   eax
        call   sub_1_02E0
        add    esp, 10h
        or     eax, eax
        push   0
        je     SHORT loc_1_6168
        push   109h
        push   esi
        call   sub_1_246E
        jmp    SHORT loc_1_61C0
loc_1_6168:
        push   0
        push   0
        db  068h, 044h, 07Ch, 000h, 000h
        call   sub_1_02E0
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_619A
        push   0
        push   109h
        push   esi
        call   sub_1_246E
        push   0
        push   dword ptr [esp + 0ah]
        call   sub_1_041B
        add    esp, 8
        jmp    SHORT loc_1_61C0
loc_1_619A:
        db  0C7h, 005h, 048h, 07Ch, 000h, 000h, 001h, 000h, 000h, 000h
        mov    dword ptr [esi + 0ch], 0
        mov    dword ptr [esi + 14h], 0
        push   esi
        call   sub_1_3FB4
        add    esp, 4
        call   sub_1_02C4
loc_1_61C0:
        pop    esi
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_61D0:
        sub    esp, 2ch
        mov    ecx, dword ptr [esp + 3ch]
        mov    eax, 0fah
        test   byte ptr [ecx + 8], 10h
        jne    SHORT loc_1_6217
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [esp + 0ch], eax
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [esp + 10h], eax
        lea    eax, [esp]
        mov    dword ptr [esp + 4], ecx
        mov    byte ptr [esp], 1
        push   eax
        call   sub_1_6265
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 3ch]
        add    esp, 4
        mov    dword ptr [eax], ecx
        movzx  eax, byte ptr [esp + 1]
loc_1_6217:
        add    esp, 2ch
        ret
sub_1_621B:
        sub    esp, 2ch
        push   esi
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 34h]
        mov    esi, dword ptr [esp + 40h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 4]
        mov    byte ptr [esp + 4], 1
        push   eax
        call   sub_1_6265
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 40h]
        add    esp, 4
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [esi], eax
        movzx  eax, byte ptr [esp + 5]
        pop    esi
        add    esp, 2ch
        ret
sub_1_6265:
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 64h]
        mov    edi, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [edi + 18h], eax
        je     NEAR PTR loc_1_632B
        or     eax, eax
        je     NEAR PTR loc_1_632B
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 010h
        je     SHORT loc_1_62AD
        db  083h, 03Dh, 094h, 056h, 000h, 000h, 000h
        je     SHORT loc_1_62AD
        push   esi
        call   sub_1_2F8E
        add    esp, 4
        cmp    byte ptr [esi + 1], 0fah
        jne    NEAR PTR loc_1_6359
loc_1_62AD:
        lea    eax, [esp + 0ch]
        mov    dword ptr [esi + 18h], eax
        push   edi
        push   esi
        call   sub_1_6549
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_62CB
        mov    byte ptr [esi + 1], 1
        jmp    NEAR PTR loc_1_6359
loc_1_62CB:
        mov    ebx, dword ptr [esi + 18h]
        test   byte ptr [esi], 80h
        jne    SHORT loc_1_62DD
loc_1_62D3:
        call   sub_1_4A98
        test   byte ptr [esi], 80h
        je     SHORT loc_1_62D3
loc_1_62DD:
        cmp    byte ptr [ebx + 8], 0
        je     SHORT loc_1_62EC
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 44h], 0
loc_1_62EC:
        mov    dword ptr [esi + 28h], edi
        cmp    byte ptr [esi + 1], 0
        je     SHORT loc_1_6359
        cmp    dword ptr [edi + 18h], 0
        jne    SHORT loc_1_6359
        push   6
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 4
        push   eax
        push   dword ptr [esi + 10h]
        call   sub_1_4AF3
        add    esp, 0ch
        mov    ax, word ptr [edi + 24h]
        mov    byte ptr [esi + 1], 0
        movzx  ecx, ax
        shr    ax, 8
        shl    ecx, 8
        movzx  eax, ax
        or     ecx, eax
        mov    dword ptr [esi + 14h], ecx
        jmp    SHORT loc_1_6359
loc_1_632B:
        push   6
        add    ecx, 4
        push   ecx
        push   dword ptr [esi + 10h]
        call   sub_1_4AF3
        add    esp, 0ch
        mov    ax, word ptr [edi + 24h]
        mov    byte ptr [esi + 1], 0
        movzx  ecx, ax
        shr    ax, 8
        shl    ecx, 8
        movzx  eax, ax
        or     ecx, eax
        mov    dword ptr [esi + 14h], ecx
        mov    dword ptr [esi + 28h], edi
loc_1_6359:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
sub_1_6360:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        mov    esi, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 44h]
        lea    ebx, [edi + 44h]
        cmp    dword ptr [esi + 18h], eax
        jne    SHORT loc_1_638A
        mov    eax, dword ptr [esp + 18h]
        movzx  ecx, word ptr [esi + 24h]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        jmp    NEAR PTR loc_1_6444
loc_1_638A:
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 010h
        je     SHORT loc_1_63D0
        db  083h, 03Dh, 094h, 056h, 000h, 000h, 000h
        je     SHORT loc_1_63D0
        push   eax
        call   sub_1_2D02
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_63B3
        mov    eax, 1
        jmp    NEAR PTR loc_1_6444
loc_1_63B3:
        mov    ax, word ptr [eax + 16h]
        movzx  ecx, ax
        shr    ax, 8
        shl    ecx, 8
        movzx  eax, ax
        or     ecx, eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        jmp    SHORT loc_1_6444
loc_1_63D0:
        push   -1
        push   0
        lea    eax, [esp + 18h]
        push   8eh
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 10h], 0
        mov    eax, 1
        je     SHORT loc_1_6444
        mov    ebp, dword ptr [esp + 10h]
        lea    eax, [ebp + 3ch]
        add    ebp, 2ch
        mov    ecx, dword ptr [esp + 10h]
        push   0ah
        mov    dword ptr [ecx + 18h], eax
        push   ebx
        push   ebp
        call   sub_1_4AF3
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    dword ptr [eax + 0ch], ebp
        mov    eax, dword ptr [esp + 10h]
        add    ebp, 0ah
        mov    dword ptr [eax + 10h], ebp
        mov    eax, dword ptr [esp + 10h]
        push   esi
        mov    dword ptr [eax + 4], esi
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 8], edi
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax], 2
        push   dword ptr [esp + 14h]
        call   sub_1_6549
        add    esp, 8
        mov    eax, 0ffh
loc_1_6444:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_644C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ebx + 24h]
        lea    edi, [ebx + 24h]
        cmp    dword ptr [esi + 18h], eax
        jne    SHORT loc_1_648B
        mov    ax, word ptr [esi + 24h]
        mov    word ptr [ebx + 0ah], ax
        add    esi, 1ch
        lea    edi, [ebx + 1ch]
        movsd
        movsw
        mov    byte ptr [ebx + 9], 0
        push   ebx
        call   sub_1_26BF
        add    esp, 4
        sub    eax, eax
        jmp    NEAR PTR loc_1_6541
loc_1_648B:
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 010h
        je     SHORT loc_1_64CC
        db  083h, 03Dh, 094h, 056h, 000h, 000h, 000h
        je     SHORT loc_1_64CC
        push   eax
        call   sub_1_2D02
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_64CC
        mov    cx, word ptr [eax + 16h]
        mov    word ptr [ebx + 0ah], cx
        lea    esi, [eax + 0ch]
        lea    edi, [ebx + 1ch]
        movsd
        movsw
        mov    byte ptr [ebx + 9], 0
        push   ebx
        call   sub_1_26BF
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_6541
loc_1_64CC:
        push   -1
        push   0
        lea    eax, [esp + 18h]
        push   8eh
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 10h], 0
        mov    eax, 1
        je     SHORT loc_1_6541
        mov    ebp, dword ptr [esp + 10h]
        lea    eax, [ebp + 3ch]
        add    ebp, 2ch
        mov    ecx, dword ptr [esp + 10h]
        push   0ah
        mov    dword ptr [ecx + 18h], eax
        push   dword ptr [edi]
        push   ebp
        call   sub_1_4AF3
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    dword ptr [eax + 0ch], ebp
        mov    eax, dword ptr [esp + 10h]
        add    ebp, 0ah
        mov    dword ptr [eax + 10h], ebp
        mov    eax, dword ptr [esp + 10h]
        push   esi
        mov    dword ptr [eax + 4], esi
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 8], ebx
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax], 4
        push   dword ptr [esp + 14h]
        call   sub_1_6549
        add    esp, 8
        mov    eax, 0ffh
loc_1_6541:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_6549:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    dword ptr [edi + 20h], 14h
        mov    esi, dword ptr [edi + 18h]
        db  066h, 083h, 03Dh, 0F4h, 07Ch, 000h, 000h, 000h
        lea    ebx, [esi + 2ah]
        jne    SHORT loc_1_658B
        push   1
        push   0
        db  068h, 0DFh, 024h, 000h, 000h
        db  068h, 0F4h, 07Ch, 000h, 000h
        call   sub_1_5206
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_658B
        mov    eax, 1
        jmp    NEAR PTR loc_1_6639
loc_1_658B:
        mov    dword ptr [esi + 4], 0
        db  066h, 0A1h, 0F4h, 07Ch, 000h, 000h
        mov    word ptr [esi + 0ah], ax
        mov    dword ptr [esi + 0ch], 0
        lea    eax, [esi + 1ch]
        push   6
        push   -1
        push   eax
        call   sub_1_4AE2
        add    esp, 0ch
        mov    word ptr [esi + 22h], 1
        mov    dword ptr [esi + 24h], ebx
        mov    word ptr [esi + 28h], 28h
        mov    byte ptr [ebx + 5], 1
        lea    eax, [ebx + 0ah]
        mov    dword ptr [ebx + 6], 0
        push   6
        push   -1
        push   eax
        call   sub_1_4AE2
        add    esp, 0ch
        mov    word ptr [ebx + 10h], 5304h
        add    ebx, 1eh
        mov    word ptr [ebx], 100h
        mov    eax, dword ptr [edi + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 2], eax
        mov    word ptr [ebx + 6], 0ffffh
        mov    word ptr [ebx + 8], 0ffffh
        db  0A1h, 080h, 056h, 000h, 000h
        mov    dword ptr [edi + 24h], eax
        db  089h, 03Dh, 080h, 056h, 000h, 000h
        push   edi
        db  068h, 0AFh, 025h, 000h, 000h
        push   3e8h
        call   sub_1_4AAB
        add    esp, 0ch
        mov    dword ptr [edi + 1ch], eax
        mov    dword ptr [edi + 14h], 5
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_4092
        add    esp, 8
        sub    eax, eax
loc_1_6639:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h
sub_1_6640:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 0FCh, 07Ch, 000h, 000h
        db  039h, 005h, 010h, 07Dh, 000h, 000h
        ja     SHORT loc_1_665E
        mov    eax, 1
        jmp    NEAR PTR loc_1_67C5
loc_1_665E:
        db  083h, 03Dh, 0B8h, 07Ch, 000h, 000h, 001h
        jne    SHORT loc_1_6671
        mov    eax, 1
        jmp    NEAR PTR loc_1_67C5
loc_1_6671:
        db  0C7h, 005h, 0B8h, 07Ch, 000h, 000h, 001h, 000h, 000h, 000h
        db  0A1h, 0A8h, 07Ch, 000h, 000h
        lea    ebp, [eax*2]
        db  08Bh, 01Dh, 0FCh, 07Ch, 000h, 000h
        imul   ebx, ebx, 28h
        push   -1
        push   0
        lea    eax, [esp + 18h]
        push   ebp
        add    ebx, 8
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_66BD
        db  0C7h, 005h, 0B8h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_67C5
loc_1_66BD:
        push   -1
        push   0
        lea    eax, [esp + 1ch]
        push   ebx
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_66F3
        push   0
        push   ebx
        push   dword ptr [esp + 18h]
        call   sub_1_246E
        db  0C7h, 005h, 0B8h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_67C5
loc_1_66F3:
        db  0A1h, 0FCh, 07Ch, 000h, 000h
        lea    ecx, [eax*4]
        mov    edi, dword ptr [esp + 10h]
        db  08Bh, 035h, 0B0h, 056h, 000h, 000h
        shr    ecx, 2
        rep movsd
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 0B0h, 056h, 000h, 000h
        db  08Bh, 015h, 0FCh, 07Ch, 000h, 000h
        shl    edx, 3
        add    edx, dword ptr [esp + 10h]
        db  0A1h, 0FCh, 07Ch, 000h, 000h
        lea    ecx, [eax*4]
        mov    edi, edx
        db  08Bh, 035h, 0B4h, 056h, 000h, 000h
        shr    ecx, 2
        rep movsd
        db  089h, 015h, 0B4h, 056h, 000h, 000h
        push   0
        db  0FFh, 035h, 0A8h, 07Ch, 000h, 000h
        db  0FFh, 035h, 018h, 07Dh, 000h, 000h
        call   sub_1_246E
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 018h, 07Dh, 000h, 000h
        db  089h, 02Dh, 0A8h, 07Ch, 000h, 000h
        mov    ecx, dword ptr [esp + 14h]
        db  0A1h, 0F8h, 07Ch, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 14h]
        db  0A3h, 0F8h, 07Ch, 000h, 000h
        mov    dword ptr [eax + 4], ebx
        sub    eax, eax
        mov    ecx, dword ptr [esp + 14h]
        add    ecx, 8
        db  039h, 005h, 0FCh, 07Ch, 000h, 000h
        jbe    SHORT loc_1_67A8
loc_1_678D:
        db  08Bh, 015h, 020h, 07Dh, 000h, 000h
        mov    dword ptr [ecx + 24h], edx
        db  089h, 00Dh, 020h, 07Dh, 000h, 000h
        add    ecx, 28h
        inc    eax
        db  03Bh, 005h, 0FCh, 07Ch, 000h, 000h
        jb     SHORT loc_1_678D
loc_1_67A8:
        db  0A1h, 0FCh, 07Ch, 000h, 000h
        db  001h, 005h, 008h, 07Dh, 000h, 000h
        db  0D1h, 025h, 0FCh, 07Ch, 000h, 000h
        db  0C7h, 005h, 0B8h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        sub    eax, eax
loc_1_67C5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_67CD:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 014h, 07Dh, 000h, 000h
        db  039h, 005h, 000h, 07Dh, 000h, 000h
        jb     SHORT loc_1_67EB
        mov    eax, 1
        jmp    NEAR PTR loc_1_69C9
loc_1_67EB:
        db  083h, 03Dh, 0BCh, 07Ch, 000h, 000h, 001h
        jne    SHORT loc_1_67FE
        mov    eax, 1
        jmp    NEAR PTR loc_1_69C9
loc_1_67FE:
        db  0C7h, 005h, 0BCh, 07Ch, 000h, 000h, 001h, 000h, 000h, 000h
        db  0A1h, 0B0h, 07Ch, 000h, 000h
        add    eax, eax
        mov    dword ptr [esp + 18h], eax
        db  08Bh, 01Dh, 000h, 07Dh, 000h, 000h
        imul   ebx, ebx, 6ch
        push   -1
        push   0
        add    ebx, 8
        lea    eax, [esp + 18h]
        push   dword ptr [esp + 20h]
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_684C
        db  0C7h, 005h, 0BCh, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_69C9
loc_1_684C:
        push   -1
        push   0
        lea    eax, [esp + 1ch]
        push   ebx
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_6882
        push   0
        push   ebx
        push   dword ptr [esp + 18h]
        call   sub_1_246E
        db  0C7h, 005h, 0BCh, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_69C9
loc_1_6882:
        db  0A1h, 000h, 07Dh, 000h, 000h
        lea    ecx, [eax*4]
        mov    edi, dword ptr [esp + 10h]
        db  08Bh, 035h, 0C0h, 056h, 000h, 000h
        shr    ecx, 2
        rep movsd
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 0C0h, 056h, 000h, 000h
        db  08Bh, 015h, 000h, 07Dh, 000h, 000h
        shl    edx, 3
        add    edx, dword ptr [esp + 10h]
        db  0A1h, 000h, 07Dh, 000h, 000h
        lea    ecx, [eax*4]
        mov    edi, edx
        db  08Bh, 035h, 0C4h, 056h, 000h, 000h
        shr    ecx, 2
        rep movsd
        db  089h, 015h, 0C4h, 056h, 000h, 000h
        db  0A1h, 000h, 07Dh, 000h, 000h
        lea    ebp, [edx + eax*8]
        add    eax, eax
        mov    edi, ebp
        db  08Bh, 035h, 0D0h, 056h, 000h, 000h
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  089h, 02Dh, 0D0h, 056h, 000h, 000h
        db  0A1h, 000h, 07Dh, 000h, 000h
        lea    ebp, [ebp + eax*4]
        lea    ecx, [eax*4]
        mov    edi, ebp
        db  08Bh, 035h, 0D4h, 056h, 000h, 000h
        shr    ecx, 2
        rep movsd
        db  089h, 02Dh, 0D4h, 056h, 000h, 000h
        sub    eax, eax
        db  039h, 005h, 0C8h, 056h, 000h, 000h
        jbe    SHORT loc_1_6936
loc_1_6925:
        mov    ecx, dword ptr [edx]
        inc    eax
        mov    dword ptr [ecx + 4], edx
        add    edx, 4
        db  039h, 005h, 0C8h, 056h, 000h, 000h
        ja     SHORT loc_1_6925
loc_1_6936:
        push   0
        db  0FFh, 035h, 0B0h, 07Ch, 000h, 000h
        db  0FFh, 035h, 004h, 07Dh, 000h, 000h
        call   sub_1_246E
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 004h, 07Dh, 000h, 000h
        mov    eax, dword ptr [esp + 18h]
        db  0A3h, 0B0h, 07Ch, 000h, 000h
        db  0A1h, 0ECh, 07Ch, 000h, 000h
        mov    ecx, dword ptr [esp + 14h]
        sub    esi, esi
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 14h]
        db  0A3h, 0ECh, 07Ch, 000h, 000h
        mov    dword ptr [eax + 4], ebx
        mov    edi, dword ptr [esp + 14h]
        add    edi, 8
        db  039h, 035h, 000h, 07Dh, 000h, 000h
        jbe    SHORT loc_1_69AC
loc_1_6983:
        push   30h
        push   0
        lea    eax, [edi + 16h]
        push   eax
        call   sub_1_4AE2
        add    esp, 0ch
        db  0A1h, 024h, 07Dh, 000h, 000h
        mov    dword ptr [edi], eax
        db  089h, 03Dh, 024h, 07Dh, 000h, 000h
        add    edi, 6ch
        inc    esi
        db  039h, 035h, 000h, 07Dh, 000h, 000h
        ja     SHORT loc_1_6983
loc_1_69AC:
        db  0A1h, 000h, 07Dh, 000h, 000h
        db  001h, 005h, 00Ch, 07Dh, 000h, 000h
        db  0D1h, 025h, 000h, 07Dh, 000h, 000h
        db  0C7h, 005h, 0BCh, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        sub    eax, eax
loc_1_69C9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_69D1:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        push   6
        lea    ebx, [esi + 42h]
        lea    edi, [esi + 18h]
        lea    ebp, [ebx + 1eh]
        mov    dword ptr [edi + 4], 1
        db  0C7h, 047h, 018h, 03Eh, 02Fh, 000h, 000h
        mov    dword ptr [edi + 0ch], 0
        lea    eax, [esi + 0ch]
        push   eax
        lea    eax, [edi + 1ch]
        push   eax
        call   sub_1_4AF3
        add    esp, 0ch
        mov    word ptr [edi + 22h], 1
        mov    dword ptr [edi + 24h], ebx
        mov    word ptr [ebp], 200h
        cmp    byte ptr [esi + 14h], 0
        jne    SHORT loc_1_6A48
        mov    word ptr [edi + 0ah], 5304h
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        add    ebp, 2
        push   ebp
        call   sub_1_2EC4
        add    esp, 0ch
        lea    ecx, [eax*8 + 20h]
        mov    word ptr [edi + 28h], cx
        mov    byte ptr [ebx + 5], 1
        jmp    SHORT loc_1_6A6D
loc_1_6A48:
        mov    word ptr [edi + 0ah], 5204h
        push   esi
        add    ebp, 2
        push   ebp
        call   sub_1_35CA
        add    esp, 8
        mov    ecx, eax
        shl    cx, 6
        add    cx, 20h
        mov    word ptr [edi + 28h], cx
        mov    byte ptr [ebx + 5], 4
loc_1_6A6D:
        or     eax, eax
        je     SHORT loc_1_6AAF
        mov    eax, dword ptr [esi + 4]
        push   8
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebx + 6], eax
        lea    eax, [esi + 0ch]
        push   eax
        add    ebx, 0ah
        push   ebx
        call   sub_1_4AF3
        add    esp, 0ch
        cmp    byte ptr [esi + 14h], 0
        je     SHORT loc_1_6A9F
        test   byte ptr [esi + 15h], 1
        je     SHORT loc_1_6A9F
        mov    eax, dword ptr [esi + 4]
        or     dword ptr [eax + 8], 20h
loc_1_6A9F:
        push   dword ptr [esi + 4]
        push   edi
        call   sub_1_4092
        add    esp, 8
        sub    eax, eax
        jmp    SHORT loc_1_6AB4
loc_1_6AAF:
        mov    eax, 1
loc_1_6AB4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_6AB9:
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 0B4h, 07Ch, 000h, 000h, 000h
        je     NEAR PTR loc_1_6B65
        db  08Bh, 035h, 01Ch, 07Dh, 000h, 000h
        db  0FFh, 00Dh, 0B4h, 07Ch, 000h, 000h
        mov    eax, dword ptr [esi + 38h]
        db  0A3h, 01Ch, 07Dh, 000h, 000h
        mov    edi, dword ptr [esi + 48h]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], 0
        mov    bl, byte ptr [esp + 18h]
        mov    byte ptr [edi + 14h], bl
        mov    ax, word ptr [esp + 1ch]
        mov    word ptr [edi + 16h], ax
        mov    eax, dword ptr [esp + 14h]
        or     eax, eax
        jne    SHORT loc_1_6B30
        push   6
        push   -1
        lea    eax, [edi + 0ch]
        push   eax
        call   sub_1_4AE2
        add    esp, 0ch
        mov    byte ptr [edi + 15h], 1
        or     bl, bl
        jne    SHORT loc_1_6B28
        mov    word ptr [edi + 12h], 5304h
        jmp    SHORT loc_1_6B43
loc_1_6B28:
        mov    word ptr [edi + 12h], 5204h
        jmp    SHORT loc_1_6B43
loc_1_6B30:
        mov    byte ptr [edi + 15h], 0
        push   8
        push   eax
        lea    eax, [edi + 0ch]
        push   eax
        call   sub_1_4AF3
        add    esp, 0ch
loc_1_6B43:
        push   edi
        call   sub_1_69D1
        add    esp, 4
        cmp    eax, 1
        jne    SHORT loc_1_6B65
        db  0A1h, 01Ch, 07Dh, 000h, 000h
        mov    dword ptr [esi + 38h], eax
        db  089h, 035h, 01Ch, 07Dh, 000h, 000h
        db  0FFh, 005h, 0B4h, 07Ch, 000h, 000h
loc_1_6B65:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_6B69:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        db  039h, 035h, 0C8h, 058h, 000h, 000h
        jbe    SHORT loc_1_6BAC
        db  0BFh, 0DCh, 058h, 000h, 000h
        mov    ebx, 2
loc_1_6B80:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ecx + 48h]
        test   dword ptr [eax + 4], ebx
        jne    SHORT loc_1_6BA0
        push   0ffffh
        db  0A0h, 0C0h, 07Ch, 000h, 000h
        push   eax
        push   0
        push   ecx
        call   sub_1_6AB9
        add    esp, 10h
loc_1_6BA0:
        add    edi, 4
        inc    esi
        db  039h, 035h, 0C8h, 058h, 000h, 000h
        ja     SHORT loc_1_6B80
loc_1_6BAC:
        db  080h, 03Dh, 0C0h, 07Ch, 000h, 000h, 001h
        sbb    al, al
        neg    al
        db  0A2h, 0C0h, 07Ch, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_6BC0:
        push   esi
        db  083h, 025h, 0F8h, 058h, 000h, 000h, 0AFh
        db  0FFh, 035h, 090h, 056h, 000h, 000h
        call   sub_1_4AD5
        add    esp, 4
        db  0C7h, 005h, 090h, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        push   5304h
        call   sub_1_041B
        add    esp, 8
        db  0F6h, 005h, 0F9h, 058h, 000h, 000h, 010h
        jne    SHORT loc_1_6C07
        push   0
        push   5204h
        call   sub_1_041B
        add    esp, 8
loc_1_6C07:
        db  083h, 03Dh, 0A8h, 07Ch, 000h, 000h, 000h
        je     SHORT loc_1_6C23
        push   0
        db  0FFh, 035h, 0A8h, 07Ch, 000h, 000h
        db  0FFh, 035h, 018h, 07Dh, 000h, 000h
        call   sub_1_246E
loc_1_6C23:
        db  0C7h, 005h, 0A8h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 0F8h, 07Ch, 000h, 000h, 000h
        je     SHORT loc_1_6C52
loc_1_6C36:
        db  0A1h, 0F8h, 07Ch, 000h, 000h
        push   0
        mov    esi, dword ptr [eax]
        push   dword ptr [eax + 4]
        push   eax
        call   sub_1_246E
        db  089h, 035h, 0F8h, 07Ch, 000h, 000h
        or     esi, esi
        jne    SHORT loc_1_6C36
loc_1_6C52:
        db  083h, 03Dh, 0B0h, 07Ch, 000h, 000h, 000h
        je     SHORT loc_1_6C6E
        push   0
        db  0FFh, 035h, 0B0h, 07Ch, 000h, 000h
        db  0FFh, 035h, 004h, 07Dh, 000h, 000h
        call   sub_1_246E
loc_1_6C6E:
        db  0C7h, 005h, 0B0h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 0ECh, 07Ch, 000h, 000h, 000h
        je     SHORT loc_1_6C9D
loc_1_6C81:
        db  0A1h, 0ECh, 07Ch, 000h, 000h
        push   0
        mov    esi, dword ptr [eax]
        push   dword ptr [eax + 4]
        push   eax
        call   sub_1_246E
        db  089h, 035h, 0ECh, 07Ch, 000h, 000h
        or     esi, esi
        jne    SHORT loc_1_6C81
loc_1_6C9D:
        db  083h, 03Dh, 0BCh, 056h, 000h, 000h, 000h
        je     SHORT loc_1_6CAB
        call   sub_1_2B80
loc_1_6CAB:
        db  083h, 03Dh, 0CCh, 056h, 000h, 000h, 000h
        je     SHORT loc_1_6CB9
        call   sub_1_328A
loc_1_6CB9:
        pop    esi
        ret
sub_1_6CBB:
        push   esi
        push   edi
        sub    edi, edi
        db  039h, 03Dh, 0B8h, 056h, 000h, 000h
        jbe    SHORT loc_1_6CEC
        mov    esi, dword ptr [esp + 0ch]
loc_1_6CCB:
        db  0A1h, 0B4h, 056h, 000h, 000h
        mov    eax, dword ptr [eax + edi*4]
        cmp    dword ptr [eax + 20h], esi
        jne    SHORT loc_1_6CE3
        push   edi
        push   eax
        call   sub_1_2D3B
        add    esp, 8
        dec    edi
loc_1_6CE3:
        inc    edi
        db  03Bh, 03Dh, 0B8h, 056h, 000h, 000h
        jb     SHORT loc_1_6CCB
loc_1_6CEC:
        pop    edi
        pop    esi
        ret
sub_1_6CEF:
        sub    esp, 58h
        push   esi
        push   edi
        lea    eax, [esp + 2ah]
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 38h], 0
        mov    di, word ptr [esp + 68h]
        mov    word ptr [esp + 3eh], di
        mov    dword ptr [esp + 40h], 0
        push   6
        lea    eax, [esp + 54h]
        push   -1
        push   eax
        call   sub_1_4AE2
        mov    word ptr [esp + 62h], 1
        lea    eax, [esp + 18h]
        mov    dword ptr [esp + 64h], eax
        mov    word ptr [esp + 68h], 28h
        mov    esi, dword ptr [esp + 70h]
        add    esp, 0ch
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [esp + 12h], eax
        push   6
        lea    eax, [esp + 1ah]
        push   -1
        push   eax
        call   sub_1_4AE2
        mov    word ptr [esp + 28h], di
        add    esp, 0ch
        cmp    di, 5304h
        jne    SHORT loc_1_6D94
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [esp + 11h], 1
        mov    word ptr [eax], 100h
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 2], 0ffffffffh
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 8], 0
        jmp    SHORT loc_1_6DAC
loc_1_6D94:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [esp + 11h], 4
        mov    word ptr [eax], 100h
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 2], 0ffffh
loc_1_6DAC:
        db  08Bh, 03Dh, 0ECh, 058h, 000h, 000h
        db  089h, 035h, 0ECh, 058h, 000h, 000h
        lea    eax, [esp + 34h]
        push   esi
        push   eax
        call   sub_1_4092
        add    esp, 8
        db  089h, 03Dh, 0ECh, 058h, 000h, 000h
        lea    eax, [esp + 34h]
        push   eax
        call   sub_1_0D40
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 58h
        ret
sub_1_6DDF:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [esp + 0eh], 5204h
        mov    esi, dword ptr [esp + 14h]
        mov    eax, esi
        imul   eax, eax, 0eh
        db  0A3h, 0B0h, 07Ch, 000h, 000h
        push   -1
        push   0
        push   eax
        db  068h, 004h, 07Dh, 000h, 000h
        call   sub_1_2468
        db  083h, 03Dh, 004h, 07Dh, 000h, 000h, 000h
        jne    SHORT loc_1_6E26
        db  0C7h, 005h, 0B0h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_6F5F
loc_1_6E26:
        mov    edi, esi
        imul   edi, edi, 6ch
        add    edi, 8
        push   -1
        push   0
        push   edi
        db  068h, 0ECh, 07Ch, 000h, 000h
        call   sub_1_2468
        db  083h, 03Dh, 0ECh, 07Ch, 000h, 000h, 000h
        jne    SHORT loc_1_6E6D
        push   0
        db  0FFh, 035h, 0B0h, 07Ch, 000h, 000h
        db  0FFh, 035h, 004h, 07Dh, 000h, 000h
        call   sub_1_246E
        db  0C7h, 005h, 0B0h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_6F5F
loc_1_6E6D:
        db  089h, 035h, 000h, 07Dh, 000h, 000h
        mov    eax, dword ptr [esp + 18h]
        db  0A3h, 014h, 07Dh, 000h, 000h
        db  0A1h, 004h, 07Dh, 000h, 000h
        db  0A3h, 0C0h, 056h, 000h, 000h
        lea    eax, [esi*4]
        db  08Bh, 00Dh, 0C0h, 056h, 000h, 000h
        add    ecx, eax
        db  089h, 00Dh, 0C4h, 056h, 000h, 000h
        add    eax, ecx
        db  0A3h, 0D0h, 056h, 000h, 000h
        lea    eax, [esi*2]
        db  003h, 005h, 0D0h, 056h, 000h, 000h
        db  0A3h, 0D4h, 056h, 000h, 000h
        db  08Bh, 01Dh, 0ECh, 07Ch, 000h, 000h
        db  0A1h, 0ECh, 07Ch, 000h, 000h
        add    ebx, 8
        mov    dword ptr [eax], 0
        db  0A1h, 0ECh, 07Ch, 000h, 000h
        or     esi, esi
        mov    dword ptr [eax + 4], edi
        db  0C7h, 005h, 024h, 07Dh, 000h, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_6F03
        mov    edi, esi
loc_1_6EE0:
        push   30h
        push   0
        lea    eax, [ebx + 16h]
        push   eax
        call   sub_1_4AE2
        add    esp, 0ch
        db  0A1h, 024h, 07Dh, 000h, 000h
        mov    dword ptr [ebx], eax
        db  089h, 01Dh, 024h, 07Dh, 000h, 000h
        add    ebx, 6ch
        dec    edi
        jne    SHORT loc_1_6EE0
loc_1_6F03:
        db  089h, 035h, 00Ch, 07Dh, 000h, 000h
        db  0C7h, 005h, 0C8h, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D8h, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        push   1
        sub    edi, edi
        push   0
        lea    eax, [esp + 16h]
        db  068h, 0AFh, 030h, 000h, 000h
        push   eax
        call   sub_1_5206
        add    esp, 10h
        db  039h, 03Dh, 0C8h, 058h, 000h, 000h
        jbe    SHORT loc_1_6F5D
        db  0BEh, 0DCh, 058h, 000h, 000h
loc_1_6F42:
        push   5204h
        push   dword ptr [esi]
        call   sub_1_6CEF
        add    esp, 8
        add    esi, 4
        inc    edi
        db  039h, 03Dh, 0C8h, 058h, 000h, 000h
        ja     SHORT loc_1_6F42
loc_1_6F5D:
        sub    eax, eax
loc_1_6F5F:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_6F66:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], 5304h
        db  08Bh, 035h, 0ACh, 07Ch, 000h, 000h
        add    esi, 2ah
        mov    ebx, dword ptr [esp + 18h]
        lea    eax, [ebx*8]
        db  0A3h, 0A8h, 07Ch, 000h, 000h
        mov    eax, esi
        db  00Fh, 0AFh, 005h, 0B4h, 07Ch, 000h, 000h
        mov    ecx, ebx
        imul   ecx, ecx, 28h
        lea    ebp, [eax + ecx + 8]
        push   -1
        push   0
        db  0FFh, 035h, 0A8h, 07Ch, 000h, 000h
        db  068h, 018h, 07Dh, 000h, 000h
        call   sub_1_2468
        db  083h, 03Dh, 018h, 07Dh, 000h, 000h, 000h
        jne    SHORT loc_1_6FC1
        jmp    NEAR PTR loc_1_7214
loc_1_6FC1:
        push   -1
        push   0
        push   ebp
        db  068h, 0F8h, 07Ch, 000h, 000h
        call   sub_1_2468
        db  083h, 03Dh, 0F8h, 07Ch, 000h, 000h, 000h
        jne    SHORT loc_1_6FF1
        push   0
        db  0FFh, 035h, 0A8h, 07Ch, 000h, 000h
        db  0FFh, 035h, 018h, 07Dh, 000h, 000h
        call   sub_1_246E
        jmp    NEAR PTR loc_1_7214
loc_1_6FF1:
        db  089h, 01Dh, 0FCh, 07Ch, 000h, 000h
        mov    eax, dword ptr [esp + 1ch]
        db  0A3h, 010h, 07Dh, 000h, 000h
        db  0A1h, 018h, 07Dh, 000h, 000h
        db  0A3h, 0B0h, 056h, 000h, 000h
        lea    eax, [ebx*4]
        db  003h, 005h, 0B0h, 056h, 000h, 000h
        db  0A3h, 0B4h, 056h, 000h, 000h
        db  08Bh, 03Dh, 0F8h, 07Ch, 000h, 000h
        db  0A1h, 0F8h, 07Ch, 000h, 000h
        add    edi, 8
        mov    dword ptr [eax], 0
        db  0A1h, 0F8h, 07Ch, 000h, 000h
        or     ebx, ebx
        mov    dword ptr [eax + 4], ebp
        db  0C7h, 005h, 020h, 07Dh, 000h, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_705D
        mov    eax, ebx
loc_1_7048:
        db  08Bh, 00Dh, 020h, 07Dh, 000h, 000h
        mov    dword ptr [edi + 24h], ecx
        db  089h, 03Dh, 020h, 07Dh, 000h, 000h
        add    edi, 28h
        dec    eax
        jne    SHORT loc_1_7048
loc_1_705D:
        db  089h, 01Dh, 008h, 07Dh, 000h, 000h
        db  0C7h, 005h, 0F4h, 058h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0B8h, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 02Ch, 058h, 000h, 000h
        db  039h, 005h, 0B4h, 07Ch, 000h, 000h
        jbe    SHORT loc_1_708E
        db  0A1h, 0B4h, 07Ch, 000h, 000h
        db  0A3h, 02Ch, 058h, 000h, 000h
loc_1_708E:
        db  0A1h, 0B4h, 07Ch, 000h, 000h
        add    eax, eax
        db  03Bh, 005h, 030h, 058h, 000h, 000h
        jbe    SHORT loc_1_70A2
        db  0A3h, 030h, 058h, 000h, 000h
loc_1_70A2:
        call   sub_1_0F2E
        or     eax, eax
        jne    NEAR PTR loc_1_7214
        call   sub_1_1050
        or     eax, eax
        jne    NEAR PTR loc_1_7214
        sub    ecx, ecx
        db  039h, 00Dh, 0B4h, 07Ch, 000h, 000h
        jbe    SHORT loc_1_7121
        mov    ebx, 0fffffffeh
loc_1_70CB:
        db  08Bh, 015h, 0B4h, 058h, 000h, 000h
        db  0FFh, 00Dh, 048h, 058h, 000h, 000h
        mov    eax, dword ptr [edx + 38h]
        db  0A3h, 0B4h, 058h, 000h, 000h
        mov    dword ptr [edx + 48h], edi
        db  0A1h, 0B8h, 058h, 000h, 000h
        mov    ebp, dword ptr [eax]
        mov    ebp, dword ptr [ebp]
        db  089h, 02Dh, 0B8h, 058h, 000h, 000h
        mov    ebp, dword ptr [eax]
        mov    dword ptr [ebp], 0
        add    edi, esi
        db  001h, 01Dh, 04Ch, 058h, 000h, 000h
        mov    dword ptr [edx + 44h], eax
        mov    byte ptr [edx + 3ch], 10h
        db  0A1h, 01Ch, 07Dh, 000h, 000h
        inc    ecx
        mov    dword ptr [edx + 38h], eax
        db  089h, 015h, 01Ch, 07Dh, 000h, 000h
        db  03Bh, 00Dh, 0B4h, 07Ch, 000h, 000h
        jb     SHORT loc_1_70CB
loc_1_7121:
        db  083h, 00Dh, 0F8h, 058h, 000h, 000h, 010h
        sub    esi, esi
        db  039h, 035h, 0C8h, 058h, 000h, 000h
        jbe    SHORT loc_1_715D
        db  0BFh, 0DCh, 058h, 000h, 000h
        sub    ebx, ebx
loc_1_7139:
        mov    eax, dword ptr [edi]
        cmp    dword ptr [eax + 18h], ebx
        je     SHORT loc_1_7151
        db  039h, 01Dh, 008h, 07Dh, 000h, 000h
        jbe    SHORT loc_1_715D
        push   eax
        call   sub_1_7221
        add    esp, 4
loc_1_7151:
        add    edi, 4
        inc    esi
        db  039h, 035h, 0C8h, 058h, 000h, 000h
        ja     SHORT loc_1_7139
loc_1_715D:
        mov    eax, dword ptr [esp + 20h]
        or     eax, eax
        je     SHORT loc_1_7185
        push   dword ptr [esp + 24h]
        push   eax
        call   sub_1_6DDF
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_7185
        push   0
        call   sub_1_6BC0
        add    esp, 4
        jmp    NEAR PTR loc_1_7214
loc_1_7185:
        push   1
        push   0
        lea    eax, [esp + 1ah]
        db  068h, 0EDh, 02Ah, 000h, 000h
        sub    esi, esi
        push   eax
        call   sub_1_5206
        add    esp, 10h
        db  039h, 035h, 0C8h, 058h, 000h, 000h
        jbe    SHORT loc_1_71C5
        db  0BFh, 0DCh, 058h, 000h, 000h
loc_1_71AA:
        push   5304h
        push   dword ptr [edi]
        call   sub_1_6CEF
        add    esp, 8
        add    edi, 4
        inc    esi
        db  039h, 035h, 0C8h, 058h, 000h, 000h
        ja     SHORT loc_1_71AA
loc_1_71C5:
        db  0A1h, 0ECh, 058h, 000h, 000h
        cmp    dword ptr [eax + 18h], 0
        jne    SHORT loc_1_71DD
        push   0bb8h
        call   sub_1_302C
        add    esp, 4
loc_1_71DD:
        db  0C7h, 005h, 08Ch, 056h, 000h, 000h, 006h, 000h, 000h, 000h
        db  0F6h, 005h, 0F9h, 058h, 000h, 000h, 010h
        jne    SHORT loc_1_7210
        push   0
        db  068h, 0DFh, 02Dh, 000h, 000h
        push   6eh
        call   sub_1_4AAB
        add    esp, 0ch
        db  0A3h, 090h, 056h, 000h, 000h
        db  0C7h, 005h, 088h, 056h, 000h, 000h, 010h, 001h, 000h, 000h
loc_1_7210:
        sub    eax, eax
        jmp    SHORT loc_1_7219
loc_1_7214:
        mov    eax, 1
loc_1_7219:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_7221:
        push   esi
        push   edi
        db  08Bh, 035h, 020h, 07Dh, 000h, 000h
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 24h]
        db  0A3h, 020h, 07Dh, 000h, 000h
        db  0FFh, 00Dh, 008h, 07Dh, 000h, 000h
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [esi], eax
        lea    eax, [esi + 0ch]
        push   6
        push   0
        push   eax
        call   sub_1_4AE2
        add    esp, 0ch
        mov    word ptr [esi + 14h], 0
        mov    ax, word ptr [edi + 24h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi + 16h], cx
        mov    dword ptr [esi + 18h], 0ffffffffh
        mov    dword ptr [esi + 20h], edi
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 1ch], 0
        db  0A1h, 0B8h, 056h, 000h, 000h
        db  039h, 005h, 0F4h, 058h, 000h, 000h
        jae    SHORT loc_1_72C0
        db  08Bh, 00Dh, 0B4h, 056h, 000h, 000h
        db  0A1h, 0F4h, 058h, 000h, 000h
        mov    eax, dword ptr [ecx + eax*4]
        db  08Bh, 015h, 0B4h, 056h, 000h, 000h
        db  08Bh, 00Dh, 0B8h, 056h, 000h, 000h
        mov    dword ptr [edx + ecx*4], eax
        db  08Bh, 00Dh, 0B0h, 056h, 000h, 000h
        db  0A1h, 0F4h, 058h, 000h, 000h
        mov    eax, dword ptr [ecx + eax*4]
        db  08Bh, 015h, 0B0h, 056h, 000h, 000h
        db  08Bh, 00Dh, 0B8h, 056h, 000h, 000h
        mov    dword ptr [edx + ecx*4], eax
loc_1_72C0:
        db  0A1h, 0B4h, 056h, 000h, 000h
        db  08Bh, 00Dh, 0F4h, 058h, 000h, 000h
        mov    dword ptr [eax + ecx*4], esi
        mov    eax, dword ptr [esi]
        db  08Bh, 015h, 0B0h, 056h, 000h, 000h
        db  08Bh, 00Dh, 0F4h, 058h, 000h, 000h
        mov    dword ptr [edx + ecx*4], eax
        db  0FFh, 005h, 0B8h, 056h, 000h, 000h
        db  0FFh, 005h, 0F4h, 058h, 000h, 000h
        pop    edi
        pop    esi
        ret
        db  000h, 000h
sub_1_72F0:
        sub    esp, 4
        mov    word ptr [esp + 2], 5204h
        db  0A1h, 0F8h, 058h, 000h, 000h
        test   al, 10h
        jne    SHORT loc_1_733E
        push   1
        push   0
        lea    eax, [esp + 0ah]
        db  068h, 0AFh, 030h, 000h, 000h
        push   eax
        call   sub_1_5206
        add    esp, 10h
        push   0
        db  068h, 0DFh, 02Dh, 000h, 000h
        push   6eh
        call   sub_1_4AAB
        add    esp, 0ch
        db  0A3h, 090h, 056h, 000h, 000h
        db  0C7h, 005h, 088h, 056h, 000h, 000h, 010h, 001h, 000h, 000h
        db  0A1h, 0F8h, 058h, 000h, 000h
loc_1_733E:
        or     eax, 1000h
        db  0A3h, 0F8h, 058h, 000h, 000h
        add    esp, 4
        ret
sub_1_734C:
        db  08Bh, 00Dh, 0F8h, 058h, 000h, 000h
        test   cl, 10h
        jne    SHORT loc_1_7384
        push   0
        push   5204h
        call   sub_1_041B
        add    esp, 8
        db  0FFh, 035h, 090h, 056h, 000h, 000h
        call   sub_1_4AD5
        add    esp, 4
        db  0C7h, 005h, 090h, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 0F8h, 058h, 000h, 000h
loc_1_7384:
        db  0A1h, 0D8h, 07Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_73BF
        push   0
        push   eax
        db  0FFh, 035h, 0DCh, 07Ch, 000h, 000h
        call   sub_1_246E
        db  0C7h, 005h, 0DCh, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D8h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D4h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 0F8h, 058h, 000h, 000h
loc_1_73BF:
        and    ecx, 0ffffefffh
        db  089h, 00Dh, 0F8h, 058h, 000h, 000h
        ret
sub_1_73CC:
        push   ebx
        push   esi
        db  0A1h, 0C8h, 07Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_7419
        mov    edx, dword ptr [esp + 0ch]
loc_1_73DB:
        mov    cx, word ptr [edx]
        cmp    word ptr [eax + 4], cx
        jne    SHORT loc_1_7413
        lea    ecx, [eax + 6]
        lea    esi, [edx + 2]
loc_1_73EA:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_1_740A
        or     bl, bl
        je     SHORT loc_1_7406
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_1_740A
        add    ecx, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_1_73EA
loc_1_7406:
        sub    ecx, ecx
        jmp    SHORT loc_1_740F
loc_1_740A:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_1_740F:
        or     ecx, ecx
        je     SHORT loc_1_741B
loc_1_7413:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_73DB
loc_1_7419:
        sub    eax, eax
loc_1_741B:
        pop    esi
        pop    ebx
        ret
sub_1_741E:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, dword ptr [edx + 24h]
        mov    dword ptr [edx + 4], 1
        lea    ebx, [ecx + 1eh]
        db  0C7h, 042h, 018h, 0A5h, 079h, 000h, 000h
        mov    word ptr [edx + 0ah], 5204h
        mov    dword ptr [edx + 0ch], 0
        mov    word ptr [edx + 22h], 1
        lea    edi, [edx + 1ch]
        mov    eax, 0ffffffffh
        stosd
        stosw
        mov    byte ptr [ecx + 5], 4
        mov    dword ptr [ecx + 6], 0
        lea    edi, [ecx + 0ah]
        stosd
        stosw
        cmp    dword ptr [esp + 1ch], 0
        mov    word ptr [ecx + 10h], 5204h
        mov    word ptr [ebx], 200h
        je     SHORT loc_1_74A8
        add    ebx, 2
        mov    eax, dword ptr [esp + 18h]
        sub    ebp, ebp
        cmp    eax, ebp
        je     SHORT loc_1_74C0
loc_1_7488:
        db  039h, 02Dh, 0D4h, 07Ch, 000h, 000h
        jbe    SHORT loc_1_74C0
        lea    esi, [eax + 4]
        mov    edi, ebx
        mov    ecx, 10h
        rep movsd
        add    ebx, 40h
        mov    eax, dword ptr [eax]
        inc    ebp
        or     eax, eax
        jne    SHORT loc_1_7488
        jmp    SHORT loc_1_74C0
loc_1_74A8:
        mov    ebp, 1
        mov    esi, dword ptr [esp + 18h]
        add    esi, 4
        lea    edi, [ebx + 2]
        mov    ecx, 10h
        rep movsd
        sub    eax, eax
loc_1_74C0:
        shl    bp, 6
        add    bp, 20h
        mov    word ptr [edx + 28h], bp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_74D1:
        sub    esp, 4
        push   esi
        db  081h, 025h, 0F8h, 058h, 000h, 000h, 0FFh, 0BFh, 0FFh, 0FFh
        db  0A1h, 0C4h, 07Ch, 000h, 000h
        db  039h, 005h, 0D4h, 07Ch, 000h, 000h
        jae    NEAR PTR loc_1_75AE
        db  083h, 03Dh, 0D4h, 07Ch, 000h, 000h, 007h
        jae    NEAR PTR loc_1_75AE
        db  083h, 03Dh, 0DCh, 07Ch, 000h, 000h, 000h
        je     SHORT loc_1_750F
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        mov    byte ptr [eax + 8], 1
loc_1_750F:
        db  08Bh, 035h, 0C4h, 07Ch, 000h, 000h
        shl    esi, 6
        add    esi, 4ah
        push   -1
        push   0
        lea    eax, [esp + 0ch]
        push   esi
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 4], 0
        jne    SHORT loc_1_754F
        db  081h, 00Dh, 0F8h, 058h, 000h, 000h, 000h, 040h, 000h, 000h
        db  083h, 03Dh, 0DCh, 07Ch, 000h, 000h, 000h
        je     SHORT loc_1_75AE
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        mov    byte ptr [eax + 8], 0
        jmp    SHORT loc_1_75AE
loc_1_754F:
        db  0A1h, 0C4h, 07Ch, 000h, 000h
        cmp    eax, 7
        jb     SHORT loc_1_755E
        mov    eax, 7
loc_1_755E:
        db  0A3h, 0D4h, 07Ch, 000h, 000h
        db  083h, 03Dh, 0D8h, 07Ch, 000h, 000h, 000h
        je     SHORT loc_1_757F
        push   0
        db  0FFh, 035h, 0D8h, 07Ch, 000h, 000h
        db  0FFh, 035h, 0DCh, 07Ch, 000h, 000h
        call   sub_1_246E
loc_1_757F:
        db  089h, 035h, 0D8h, 07Ch, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        db  0A3h, 0DCh, 07Ch, 000h, 000h
        mov    byte ptr [eax + 8], 0
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        mov    word ptr [eax + 22h], 1
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        db  08Bh, 00Dh, 0DCh, 07Ch, 000h, 000h
        add    eax, 2ah
        mov    dword ptr [ecx + 24h], eax
loc_1_75AE:
        pop    esi
        add    esp, 4
        ret
sub_1_75B3:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0F6h, 005h, 0F9h, 058h, 000h, 000h, 010h
        jne    SHORT loc_1_75C7
        call   sub_1_72F0
loc_1_75C7:
        mov    ebx, dword ptr [esp + 18h]
        push   ebx
        call   sub_1_73CC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_75E2
        mov    eax, 4dah
        jmp    NEAR PTR loc_1_76E2
loc_1_75E2:
        db  0FFh, 005h, 0C4h, 07Ch, 000h, 000h
        db  083h, 03Dh, 0DCh, 07Ch, 000h, 000h, 000h
        je     SHORT loc_1_7608
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        cmp    byte ptr [eax + 8], 0
        je     SHORT loc_1_7608
        db  081h, 00Dh, 0F8h, 058h, 000h, 000h, 000h, 040h, 000h, 000h
        jmp    SHORT loc_1_760D
loc_1_7608:
        call   sub_1_74D1
loc_1_760D:
        push   -1
        push   0
        lea    eax, [esp + 14h]
        push   44h
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_762E
        mov    eax, 8
        jmp    NEAR PTR loc_1_76E2
loc_1_762E:
        mov    edi, dword ptr [esp + 0ch]
        add    edi, 4
        mov    esi, ebx
        mov    ecx, 0ch
        rep movsd
        movsw
        db  08Bh, 035h, 0ECh, 058h, 000h, 000h
        mov    edi, dword ptr [esp + 0ch]
        add    edi, 36h
        add    esi, 18h
        movsd
        movsd
        movsw
        mov    ecx, dword ptr [esp + 0ch]
        mov    ax, word ptr [ebx + 3ch]
        mov    word ptr [ecx + 40h], ax
        mov    eax, dword ptr [esp + 0ch]
        push   -1
        mov    word ptr [eax + 42h], 100h
        mov    ecx, dword ptr [esp + 10h]
        db  0A1h, 0C8h, 07Ch, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 0C8h, 07Ch, 000h, 000h
        push   0
        lea    eax, [esp + 18h]
        push   8ah
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_76E0
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 10h]
        add    eax, 2ah
        push   0
        mov    dword ptr [ecx + 24h], eax
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 18h]
        call   sub_1_741E
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esp + 10h]
        db  0C7h, 040h, 018h, 060h, 036h, 000h, 000h
        db  0FFh, 035h, 0ECh, 058h, 000h, 000h
        push   dword ptr [esp + 14h]
        call   sub_1_4092
        add    esp, 8
loc_1_76E0:
        sub    eax, eax
loc_1_76E2:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_76E9:
        push   esi
        push   edi
        db  0A1h, 0C8h, 07Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_770A
loc_1_76F4:
        db  08Bh, 035h, 0ECh, 058h, 000h, 000h
        lea    edi, [eax + 36h]
        add    esi, 18h
        movsd
        movsd
        movsw
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_76F4
loc_1_770A:
        pop    edi
        pop    esi
        ret
sub_1_770D:
        sub    esp, 4
        push   esi
        db  0F6h, 005h, 0F9h, 058h, 000h, 000h, 010h
        jne    SHORT loc_1_7724
        mov    eax, 4dbh
        jmp    NEAR PTR loc_1_77E9
loc_1_7724:
        push   dword ptr [esp + 0ch]
        call   sub_1_73CC
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_7740
        mov    eax, 4dbh
        jmp    NEAR PTR loc_1_77E9
loc_1_7740:
        db  0A1h, 0CCh, 07Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_775B
loc_1_7749:
        cmp    dword ptr [eax + 10h], esi
        jne    SHORT loc_1_7755
        mov    dword ptr [eax + 10h], 0
loc_1_7755:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_7749
loc_1_775B:
        push   -1
        push   0
        lea    eax, [esp + 0ch]
        push   8ah
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_77C0
        mov    word ptr [esi + 42h], 1000h
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 4]
        add    eax, 2ah
        push   0
        push   esi
        mov    dword ptr [ecx + 24h], eax
        push   dword ptr [esp + 0ch]
        call   sub_1_741E
        mov    eax, dword ptr [esp + 10h]
        add    esp, 0ch
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esp + 4]
        db  0C7h, 040h, 018h, 060h, 036h, 000h, 000h
        db  0FFh, 035h, 0ECh, 058h, 000h, 000h
        push   dword ptr [esp + 8]
        call   sub_1_4092
        add    esp, 8
loc_1_77C0:
        push   0
        push   esi
        db  068h, 0C8h, 07Ch, 000h, 000h
        call   sub_1_46EA
        add    esp, 0ch
        push   0
        push   44h
        push   esi
        call   sub_1_246E
        db  0FFh, 00Dh, 0C4h, 07Ch, 000h, 000h
        jne    SHORT loc_1_77E7
        call   sub_1_734C
loc_1_77E7:
        sub    eax, eax
loc_1_77E9:
        pop    esi
        add    esp, 4
        ret
loc_1_77EE:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        cmp    byte ptr [eax + 8], 0
        je     SHORT loc_1_7818
        push   0
        db  068h, 0EEh, 077h, 000h, 000h
        push   37h
        call   sub_1_4AAB
        add    esp, 0ch
        db  0A3h, 0D0h, 07Ch, 000h, 000h
        jmp    NEAR PTR loc_1_78EE
loc_1_7818:
        sub    ebp, ebp
loc_1_781A:
        db  08Bh, 01Dh, 0CCh, 07Ch, 000h, 000h
        cmp    ebx, ebp
        je     NEAR PTR loc_1_78E4
        mov    eax, dword ptr [ebx]
        db  0A3h, 0CCh, 07Ch, 000h, 000h
        cmp    dword ptr [ebx + 10h], ebp
        je     NEAR PTR loc_1_78C4
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        mov    dword ptr [eax + 0ch], ebp
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        mov    dword ptr [eax + 4], 1
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        db  0C7h, 040h, 018h, 0EEh, 077h, 000h, 000h
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        mov    word ptr [eax + 0ah], 5204h
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        lea    esi, [ebx + 8]
        mov    word ptr [eax + 22h], 1
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        mov    word ptr [eax + 28h], 60h
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        mov    eax, dword ptr [eax + 24h]
        mov    byte ptr [eax + 5], 4
        db  08Bh, 03Dh, 0DCh, 07Ch, 000h, 000h
        add    edi, 1ch
        movsd
        movsw
        lea    esi, [ebx + 4]
        lea    edi, [eax + 6]
        movsd
        movsd
        add    eax, 1eh
        movsd
        mov    word ptr [eax], 400h
        mov    esi, dword ptr [ebx + 10h]
        lea    edi, [eax + 2]
        add    esi, 4
        mov    ecx, 10h
        rep movsd
        push   ebp
        db  0FFh, 035h, 0DCh, 07Ch, 000h, 000h
        call   sub_1_4092
        add    esp, 8
loc_1_78C4:
        db  0A1h, 0B4h, 058h, 000h, 000h
        mov    dword ptr [ebx], eax
        lea    eax, [ebx - 38h]
        db  0A3h, 0B4h, 058h, 000h, 000h
        db  0FFh, 005h, 048h, 058h, 000h, 000h
        cmp    dword ptr [ebx + 10h], ebp
        je     NEAR PTR loc_1_781A
        jmp    SHORT loc_1_78EE
loc_1_78E4:
        db  0C7h, 005h, 0D0h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_78EE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_78F3:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax + 8], 8
        jne    NEAR PTR loc_1_7992
        db  08Bh, 01Dh, 0C8h, 07Ch, 000h, 000h
        or     ebx, ebx
        je     NEAR PTR loc_1_7992
        mov    edx, dword ptr [esp + 10h]
        mov    ax, word ptr [edx + 20h]
loc_1_791A:
        cmp    word ptr [ebx + 4], ax
        je     SHORT loc_1_7928
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_1_791A
        jmp    SHORT loc_1_7992
loc_1_7928:
        db  083h, 03Dh, 048h, 058h, 000h, 000h, 000h
        je     SHORT loc_1_7992
        db  0FFh, 00Dh, 048h, 058h, 000h, 000h
        db  08Bh, 00Dh, 0B4h, 058h, 000h, 000h
        mov    eax, dword ptr [ecx + 38h]
        add    ecx, 38h
        db  0A3h, 0B4h, 058h, 000h, 000h
        lea    esi, [edx + 12h]
        lea    edi, [ecx + 4]
        movsd
        movsd
        movsd
        mov    dword ptr [ecx + 10h], ebx
        db  0A1h, 0CCh, 07Ch, 000h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 0CCh, 07Ch, 000h, 000h
        db  083h, 03Dh, 0D0h, 07Ch, 000h, 000h, 000h
        jne    SHORT loc_1_7992
        cmp    word ptr [edx + 20h], 400h
        push   0
        db  068h, 0EEh, 077h, 000h, 000h
        je     SHORT loc_1_7983
        call   sub_1_4AC2
        add    esp, 8
        jmp    SHORT loc_1_798D
loc_1_7983:
        push   0ah
        call   sub_1_4AAB
        add    esp, 0ch
loc_1_798D:
        db  0A3h, 0D0h, 07Ch, 000h, 000h
loc_1_7992:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_7996:
        db  0C7h, 005h, 0E4h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR sub_1_79E7
loc_1_79A5:
        db  0F6h, 005h, 0F9h, 058h, 000h, 000h, 040h
        je     SHORT loc_1_79B3
        call   sub_1_74D1
loc_1_79B3:
        db  0A1h, 0E0h, 07Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_79E6
        push   1
        push   eax
        db  0FFh, 035h, 0DCh, 07Ch, 000h, 000h
        call   sub_1_741E
        add    esp, 0ch
        db  0A3h, 0E0h, 07Ch, 000h, 000h
        db  0FFh, 035h, 0ECh, 058h, 000h, 000h
        db  0FFh, 035h, 0DCh, 07Ch, 000h, 000h
        call   sub_1_4092
        add    esp, 8
loc_1_79E6:
        ret
sub_1_79E7:
        db  083h, 03Dh, 0E4h, 07Ch, 000h, 000h, 000h
        je     SHORT loc_1_7A08
        db  0FFh, 035h, 0E4h, 07Ch, 000h, 000h
        call   sub_1_4AD5
        add    esp, 4
        db  0C7h, 005h, 0E4h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_7A08:
        db  083h, 03Dh, 0D4h, 07Ch, 000h, 000h, 000h
        je     SHORT loc_1_7A75
        db  0A1h, 0DCh, 07Ch, 000h, 000h
        cmp    byte ptr [eax + 8], 0
        je     SHORT loc_1_7A37
        push   0
        db  068h, 096h, 079h, 000h, 000h
        push   96h
        call   sub_1_4AAB
        add    esp, 0ch
        db  0A3h, 0E4h, 07Ch, 000h, 000h
        jmp    SHORT loc_1_7A75
loc_1_7A37:
        db  0A1h, 0E8h, 07Ch, 000h, 000h
        db  0FFh, 005h, 0E8h, 07Ch, 000h, 000h
        test   al, 1
        jne    SHORT loc_1_7A75
        push   1
        db  0FFh, 035h, 0C8h, 07Ch, 000h, 000h
        db  0FFh, 035h, 0DCh, 07Ch, 000h, 000h
        call   sub_1_741E
        add    esp, 0ch
        db  0A3h, 0E0h, 07Ch, 000h, 000h
        db  0FFh, 035h, 0ECh, 058h, 000h, 000h
        db  0FFh, 035h, 0DCh, 07Ch, 000h, 000h
        call   sub_1_4092
        add    esp, 8
loc_1_7A75:
        ret
        db  000h, 000h
loc_1_7A78:
        dec    ebp
        inc    ecx
        pop    eax
        push   ebx
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        add    byte ptr [eax], al
loc_1_7A84:
        dec    ebp
        inc    ecx
        pop    eax
        inc    ebx
        dec    edi
        dec    esi
        dec    esi
        inc    ebp
        inc    ebx
        push   esp
        add    byte ptr [eax], al
loc_1_7A90:
        inc    ebx
        inc    ecx
        inc    ebx
        dec    eax
        inc    ebp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [edx + 49h], dl
        push   eax
        pop    edi
        push   esp
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        pop    edi
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
loc_1_7AAC:
        push   edx
        dec    edi
        push   ebp
        push   esp
        inc    ebp
        push   edx
        pop    edi
        dec    ebp
        inc    ecx
        pop    eax
        add    byte ptr [eax], al
loc_1_7AB8:
        push   ebx
        inc    ecx
        push   eax
        pop    edi
        push   esp
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        pop    edi
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
loc_1_7AC8:
        push   ebx
        inc    ecx
        push   eax
        pop    edi
        dec    ebp
        inc    ecx
        pop    eax
        db  000h
sub_1_7AD0:
        push   edx
        dec    ecx
        push   eax
        pop    edi
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        pop    edi
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [edx + 49h], dl
        push   eax
        pop    edi
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        push   ebx
        add    byte ptr [ebp + 4eh], al
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        pop    edi
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        pop    edi
        xor    dh, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_7AFC:
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        pop    edi
        inc    esp
        inc    ebp
        push   esp
        inc    ebp
        inc    ebx
        push   esp
        dec    ecx
        dec    edi
        dec    esi
        pop    edi
        push   eax
        inc    ebp
        push   edx
        dec    ecx
        dec    edi
        inc    esp
        add    byte ptr [eax], al
loc_1_7B14:
        js     SHORT loc_1_7B90
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        and    al, 58h
        add    byte ptr [eax], al
        test   byte ptr [edx], bh
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 10000000h], al
        add    byte ptr [eax], al
        db  000h, 028h
        pop    eax
        add    byte ptr [eax], al
        nop
        jp     SHORT loc_1_7B3F
loc_1_7B3F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 078h, 058h
        add    byte ptr [eax], al
        pushfd
        jp     SHORT loc_1_7B53
loc_1_7B53:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        fcom   qword ptr [ebp]
        db  000h, 0ACh, 07Ah, 000h, 000h, 010h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        loopne loc_1_7BCB
        add    byte ptr [eax], al
        mov    eax, 7ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        db  000h, 0E4h
        push   ebp
        add    byte ptr [eax], al
        enter  7ah, 0
loc_1_7B90:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        db  0E8h, 055h, 000h, 000h, 0D0h
        jp     SHORT loc_1_7BA3
loc_1_7BA3:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, byte ptr [eax]
        db  000h, 088h, 004h, 000h, 000h, 0ACh
        jl     SHORT loc_1_7BB3
loc_1_7BB3:
        db  000h, 0E0h
        jp     SHORT loc_1_7BB7
loc_1_7BB7:
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        mov    ah, 7ch
        add    byte ptr [eax], al
        in     al, dx
        jp     SHORT loc_1_7BCB
loc_1_7BCB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0ECh
        push   ebp
        add    byte ptr [eax], al
        cld
        jp     SHORT loc_1_7BDF
loc_1_7BDF:
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax - 23ffffedh], cl
        add    byte ptr [eax], al
        db  000h, 03Ch, 058h
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
loc_1_7C04:
        dec    esi
        push   edi
        dec    esp
        dec    ecx
        dec    esi
        dec    ebx
        add    byte ptr [eax], al
loc_1_7C0C:
        pop    es
        add    byte ptr [edi], al
        db  000h, 004h, 07Ch
        add    byte ptr [eax], al
loc_1_7C14:
        push   ebx
        inc    ecx
        push   eax
        pop    edi
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        push   ebx
        pop    edi
        dec    ecx
        dec    esi
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [eax], al
        db  000h
loc_1_7C28:
        push   ebx
        inc    ecx
        push   eax
        pop    edi
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        push   ebx
        pop    edi
        inc    ebp
        pop    eax
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        and    byte ptr [ebx], dh
        xor    dword ptr cs:[eax], esp
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7C60:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7C64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7C68:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_7C6C:
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        pop    edi
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [eax], al
loc_1_7C78:
        or     eax, dword ptr [eax]
        or     eax, dword ptr [eax]
        insb
        jl     SHORT loc_1_7C7F
loc_1_7C7F:
        add    byte ptr [esi + 45h], cl
        push   esp
        push   edi
        dec    edi
        push   edx
        dec    ebx
        pop    edi
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
loc_1_7C8C:
        or     eax, dword ptr [eax]
        or     eax, dword ptr [eax]
        cmp    byte ptr [eax + eax], 46h
        dec    esp
        inc    ecx
        inc    edi
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [esi], al
        db  000h, 094h, 07Ch, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
loc_1_7CA8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CAC:
        mov    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
loc_1_7CB8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CBC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CC0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CC8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CCC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CD4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CE0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CEC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D00:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D08:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
