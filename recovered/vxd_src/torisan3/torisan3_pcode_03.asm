; torisan3_pcode_03.asm -- TORISAN3 object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        push   esp
        dec    edi
        push   edx
        imul   edx, dword ptr [ebx + 41h], 204eh
loc_3_000A:
        push   cs
        add    byte ptr [eax], al
        db  000h
loc_3_000E:
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    ebx
        inc    esp
        push   edx
        sub    eax, 143343h
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    ebx
        inc    esp
        push   edx
        pop    edi
        inc    ebx
        xor    edx, dword ptr [eax + eax]
        add    byte ptr [eax], al
loc_3_0032:
        push   ebp
        push   esi
        mov    esi, dword ptr [ebx + 0ch]
        test   dword ptr [esi + 20h], 6000h
        jne    NEAR PTR loc_3_01AE
        test   byte ptr [esi + 24h], 2
        je     NEAR PTR loc_3_01AE
        cmp    byte ptr [esi + 74h], 1
        jne    NEAR PTR loc_3_01AE
        cmp    byte ptr [esi + 40h], 5
        jne    NEAR PTR loc_3_01AE
        cmp    byte ptr [esi + 7dh], 0
        jne    NEAR PTR loc_3_01AE
        push   esi
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 1
        lea    esi, [esi + 8ah]
loc_3_007D:
        push   esi
        push   edi
        push   ecx
        mov    cl, 8
        repe cmpsb
        pop    ecx
        pop    edi
        pop    esi
        je     SHORT loc_3_0094
        lea    edi, [edi + 0ah]
        loop   loc_3_007D
loc_3_008E:
        pop    esi
        jmp    NEAR PTR loc_3_01AE
loc_3_0094:
        movzx  ecx, word ptr [edi + 8]
        mov    esi, dword ptr [esp]
        db  08Bh, 03Ch, 08Dh, 00Ah, 000h, 000h, 000h
        movzx  ecx, word ptr [edi]
        lea    edi, [edi + 2]
        lea    esi, [esi + 92h]
loc_3_00AE:
        push   ecx
        push   esi
        push   edi
        mov    ecx, 0dh
        repe cmpsb
        pop    edi
        pop    esi
        pop    ecx
        je     SHORT loc_3_00C4
        lea    edi, [edi + 11h]
        loop   loc_3_00AE
        jmp    SHORT loc_3_008E
loc_3_00C4:
        mov    ebp, dword ptr [edi + 0dh]
        pop    esi
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 1ch]
        sub    esp, 18h
        mov    edi, esp
        mov    word ptr [edi], 5
        mov    dword ptr [edi + 12h], eax
        mov    dword ptr [edi + 4], esi
        db  0C7h, 047h, 008h, 038h, 001h, 000h, 000h
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [edi + 16h], al
        push   1
        push   ebp
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0ch], 0ffh
        mov    dword ptr [eax + 10h], 28282828h
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [edi + 10h], 0
        push   edi
        db  02Eh, 0FFh, 015h, 068h, 004h, 000h, 000h
        add    esp, 1ch
        sub    esp, 0ch
        mov    ebx, esp
        mov    word ptr [ebx], 1
        mov    word ptr [ebx + 4], 4a8h
        push   ebx
        db  0FFh, 015h, 068h, 004h, 000h, 000h
        mov    ebx, dword ptr [ebx + 6]
        add    esp, 10h
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
        mov    esi, dword ptr [esp]
        mov    dword ptr [esi + 6ch], ebx
        sti
        mov    byte ptr [ebx + 7dh], 1
        mov    dword ptr [ebx], ebx
        db  0E8h, 09Ah, 000h, 000h, 000h
        sub    esp, 0ch
        mov    ebx, esp
        mov    word ptr [ebx], 1
        mov    word ptr [ebx + 4], 4a8h
        push   ebx
        db  0FFh, 015h, 068h, 004h, 000h, 000h
        mov    ebx, dword ptr [ebx + 6]
        add    esp, 10h
        mov    ecx, 12ah
        cli
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
        mov    byte ptr [ebx + 7dh], 2
        mov    dword ptr [ebx], ebx
        db  0E8h, 055h, 000h, 000h, 000h
loc_3_01AE:
        pop    esi
        pop    ebp
        ret
loc_3_01B1:
        mov    esi, dword ptr [ebx + 0ch]
        cmp    byte ptr [esi + 74h], 1
        jne    SHORT loc_3_01D2
        cmp    byte ptr [esi + 40h], 5
        jne    SHORT loc_3_01D2
        mov    al, byte ptr [esi + 84h]
        and    al, 7
        cmp    al, 2
        jae    SHORT loc_3_01D2
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
loc_3_01D2:
        ret
loc_3_01D3:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_01E2
        mov    word ptr [ebx + 2], 0ffffh
loc_3_01E2:
        ret
loc_3_01E3:
        inc    ebp
        push   ebx
        inc    esp
        dec    ecx
        pop    esp
        db  000h
loc_3_01E9:
        push   ebp
        db  073h, 065h
        db  072h, 044h
        db  072h, 069h
        db  076h, 065h
        dec    esp
        db  065h, 074h, 074h
        db  065h, 072h, 041h
        db  073h, 073h
        imul   esp, dword ptr [edi + 6eh], 746e656dh
        add    byte ptr [ebx - 729ff314h], al
        inc    ebx
        db  070h, 06Ah
        add    bh, bh
        db  076h, 070h
        push   eax
        db  0CDh, 020h, 003h, 000h, 033h, 000h
        add    esp, 0ch
        push   esi
        lea    eax, [esp + 2ch]
        push   0
        push   dword ptr [ebx + 70h]
        push   eax
        db  0CDh, 020h, 006h, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_3_02CB
        mov    eax, dword ptr [esp + 2ch]
        add    eax, 22h
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_02CB
        push   eax
        db  08Dh, 035h, 0E3h, 001h, 000h, 000h
        mov    ecx, 5
        mov    edi, eax
        rep movsb
        lea    esi, [ebx + 8ah]
        mov    ecx, 19h
        rep movsb
        mov    al, 5ch
        stosb
        mov    eax, dword ptr [esp + 30h]
        push   0
        push   eax
        push   edi
        push   dword ptr [ebx + 70h]
        db  0CDh, 020h, 007h, 000h, 033h, 000h
        add    esp, 10h
        pop    esi
        xor    ecx, ecx
        or     eax, eax
        jne    SHORT loc_3_02CB
        lea    edi, [esi + 1fh]
        mov    al, 5ch
        mov    ecx, dword ptr [esp + 2ch]
        mov    edx, ecx
loc_3_0292:
        repne scasb
        jecxz  loc_3_029C
        mov    byte ptr [edi - 1], 26h
        jmp    SHORT loc_3_0292
loc_3_029C:
        lea    eax, [esi + 1fh]
        add    eax, edx
        mov    cx, word ptr [ebx + 7ch]
        add    cx, 3030h
        mov    word ptr [eax], cx
        lea    edi, [esp + 28h]
        push   0
        push   dword ptr [ebx + 70h]
        push   esi
        push   edi
        db  0CDh, 020h, 00Bh, 000h, 033h, 000h
        add    esp, 10h
        xor    ecx, ecx
        or     eax, eax
        jne    SHORT loc_3_02CB
        mov    ecx, dword ptr [esp + 28h]
loc_3_02CB:
        mov    dword ptr [ebx + 70h], ecx
        mov    word ptr [ebx + 48h], 0ffffh
        lea    ecx, [esp + 24h]
        sub    esp, 18h
        mov    eax, esp
        mov    word ptr [eax], 11h
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 0ch], 1
        mov    dword ptr [eax + 10h], 3
        mov    dword ptr [eax + 14h], ecx
        db  0C7h, 040h, 008h, 0E9h, 001h, 000h, 000h
        push   eax
        db  0FFh, 015h, 068h, 004h, 000h, 000h
        movzx  eax, word ptr [eax + 2]
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_3_031C
        mov    ax, word ptr [esp + 24h]
        sub    ax, 4141h
        mov    word ptr [ebx + 48h], ax
loc_3_031C:
        pop    esi
        db  0E8h, 023h, 000h, 000h, 000h
        jae    SHORT loc_3_0329
        call   sub_3_0382
loc_3_0329:
        popad
        add    esp, 0ch
        ret
loc_3_032E:
        inc    ecx
        jne    SHORT loc_3_03A5
        outsd
        dec    ecx
        outsb
        jae    SHORT loc_3_039B
        jb     SHORT loc_3_03AC
        dec    esi
        outsd
        je     SHORT loc_3_03A5
        imul   sp, word ptr [ebx + 61h], 6974h
        outsd
        outsb
        add    byte ptr [ebx - 3374e314h], al
        mov    word ptr [ecx], 11h
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 0ch], 3
        mov    dword ptr [ecx + 10h], 1
        lea    edx, [ecx + 18h]
        mov    dword ptr [ecx + 14h], edx
        db  0C7h, 041h, 008h, 02Eh, 003h, 000h, 000h
        push   ecx
        db  0FFh, 015h, 068h, 004h, 000h, 000h
        movzx  ecx, word ptr [ecx + 2]
        movzx  edx, byte ptr [edx]
        add    esp, 20h
        jecxz  loc_3_0381
        stc
loc_3_0381:
        ret
sub_3_0382:
        enter  0ch, 0
        pushad
        mov    dword ptr [ebp - 0ch], 1
        mov    edx, dword ptr [esi + 70h]
        lea    eax, [ebp - 8]
        mov    byte ptr [eax], 0
        push   0
        push   1
loc_3_039B:
        push   eax
        push   3
        db  068h, 02Eh, 003h, 000h, 000h
        push   0
loc_3_03A5:
        push   edx
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
loc_3_03AC:
        add    esp, 1ch
        popad
        leave
        ret
        db  000h, 000h
sub_3_03B4:
        push   esi
        push   ebx
        push   dword ptr [ebx + 10h]
        mov    esi, dword ptr [ebx + 10h]
        mov    esi, dword ptr [esi + 8]
        mov    esi, dword ptr [esi + 8]
        call   sub_3_03F2
        or     edi, edi
        je     SHORT loc_3_03ED
        mov    dword ptr [edi + 0a4h], edi
        pop    dword ptr [edi + 10h]
        mov    word ptr [edi + 68h], 12h
        mov    dword ptr [edi + 8], esi
        mov    ebx, edi
        call   sub_3_0476
        pop    dword ptr [ebx + 90h]
        pop    esi
        clc
        ret
loc_3_03ED:
        pop    ebx
        pop    ebx
        pop    esi
        stc
        ret
sub_3_03F2:
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
        db  0FFh, 015h, 068h, 004h, 000h, 000h
        mov    edi, dword ptr [edi + 0ah]
        add    esp, 14h
        mov    eax, edi
        or     edi, edi
        je     SHORT loc_3_0474
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
loc_3_0474:
        pop    esi
        ret
sub_3_0476:
        push   edx
        mov    edi, ecx
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
sub_3_04D8:
        mov    esi, dword ptr [ebx + 10h]
        mov    esi, dword ptr [esi + 8]
        mov    esi, dword ptr [esi + 8]
        mov    dword ptr [ebx + 4], esi
        db  0BAh, 038h, 001h, 000h, 000h
        mov    byte ptr [edi + 3], 0
        mov    cx, word ptr [ebx + 0eh]
        mov    word ptr [ebx + 0ch], cx
        xchg   esi, edi
        xchg   edi, ebx
        mov    dword ptr [edi + 0a4h], edi
        db  0FFh, 015h, 074h, 004h, 000h, 000h
        ret
_PTEXT ends
        end
