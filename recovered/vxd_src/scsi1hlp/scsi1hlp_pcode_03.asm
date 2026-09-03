; scsi1hlp_pcode_03.asm -- SCSI1HLP object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        dec    esi
        inc    ebp
        inc    ebx
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [edi + ecx*2 + 53h], dl
        dec    eax
        dec    ecx
        inc    edx
        inc    ecx
        and    byte ptr [ecx], al
        add    byte ptr [ebx + 48h], al
        dec    ecx
        dec    esi
        dec    edi
        dec    esi
        and    byte ptr [eax], ah
        add    al, byte ptr [eax]
        push   ebx
        dec    edi
        dec    esi
        pop    ecx
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    eax, dword ptr [eax]
loc_3_0028:
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        insb
        add    dword ptr [eax], eax
        db  000h, 0ECh
        add    dword ptr [eax], eax
        db  000h, 012h
        add    al, byte ptr [eax]
        db  000h
loc_3_0038:
        adc    dword ptr [eax], eax
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    bh, byte ptr [eax]
        xor    esp, dword ptr [eax]
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [ebx]
        cmp    byte ptr [eax], ah
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    bh, byte ptr [eax]
        xor    al, 31h
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    bh, byte ptr [eax]
        xor    al, 20h
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [edx]
        xor    dword ptr [eax], esi
        add    dword ptr [eax], eax
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    ebx
        inc    esp
        push   edx
        sub    eax, 50303132h
        and    byte ptr [ecx], al
        add    byte ptr [ebx + 44h], al
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [eax + esi]
        xor    byte ptr [eax], al
        add    byte ptr [ebx + 44h], al
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [3030h]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [3031h]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [3131h]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [3231h]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [eax + esi]
        xor    dword ptr [eax], eax
        add    byte ptr [ebx + 44h], al
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [3130h]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [esi]
        xor    byte ptr [ecx], dh
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [3230h]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [esi]
        xor    byte ptr [edx], dh
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    bh, byte ptr [ecx]
        xor    byte ptr [eax], dh
        add    byte ptr [eax], al
loc_3_016C:
        pop    es
        add    byte ptr [ebx + 44h], al
        sub    eax, 204d4f52h
        pop    eax
        dec    ebp
        sub    eax, 31303433h
        push   esp
        inc    ecx
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        pop    eax
        dec    ebp
        sub    eax, 31303533h
        push   esp
        inc    ecx
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        pop    eax
        dec    ebp
        sub    eax, 31303633h
        push   esp
        inc    ecx
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        pop    eax
        dec    ebp
        sub    eax, 31303335h
        push   esp
        inc    ecx
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        pop    eax
        dec    ebp
        sub    eax, 31303134h
        inc    ecx
        and    byte ptr [edx], al
        add    byte ptr [ebx + 44h], al
        sub    eax, 204d4f52h
        pop    eax
        dec    ebp
        sub    eax, 31303134h
        push   esp
        inc    ecx
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        pop    eax
        dec    ebp
        sub    eax, 31303134h
        inc    edx
        and    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [ebx + 44h], al
        sub    eax, 204d4f52h
        inc    ebx
        inc    esp
        push   ebx
        sub    eax, 20353335h
        and    byte ptr [ebx], al
        add    byte ptr [ebx + 44h], al
        sub    eax, 204d4f52h
        inc    ebx
        inc    esp
        push   ebx
        sub    eax, 20353334h
        and    byte ptr [ebx], al
        add    byte ptr [edx], al
        add    byte ptr [ebx + 44h], al
        sub    eax, 204d4f52h
        inc    ebx
        inc    esp
        push   ebp
        xor    esi, dword ptr [ebx]
        inc    ecx
        and    byte ptr [eax], ah
        and    byte ptr [eax + eax], al
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    ebx
        inc    esp
        push   ebp
        xor    esi, dword ptr [ecx]
        inc    ecx
        and    byte ptr [eax], ah
        and    byte ptr [eax + eax], al
loc_3_0238:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        test   byte ptr [eax], al
        add    byte ptr [eax], al
        db  074h, 001h
        add    byte ptr [eax], al
        aam    1
        add    byte ptr [eax], al
        pop    esp
        add    dword ptr [eax], eax
        add    byte ptr [ebp + 56h], dl
        mov    esi, dword ptr [ebx + 0ch]
        test   dword ptr [esi + 20h], 6000h
        jne    NEAR PTR loc_3_034C
        cmp    byte ptr [esi + 74h], 1
        jne    NEAR PTR loc_3_034C
        cmp    byte ptr [esi + 40h], 5
        jne    NEAR PTR loc_3_034C
        push   esi
        db  08Dh, 03Dh, 01Dh, 002h, 000h, 000h
        mov    ecx, 5
        lea    esi, [esi + 8ah]
loc_3_0284:
        push   edi
        push   esi
        push   ecx
        mov    ecx, 18h
        repe cmpsb
        jne    SHORT loc_3_02AE
        lodsd
        xchg   ah, al
        ror    eax, 10h
        xchg   ah, al
        cmp    eax, dword ptr [edi]
        jb     SHORT loc_3_02AE
        cmp    eax, dword ptr [edi + 4]
        ja     SHORT loc_3_02AE
        add    esp, 0ch
        pop    esi
        or     byte ptr [esi + 482h], 20h
        jmp    SHORT loc_3_02B7
loc_3_02AE:
        pop    ecx
        pop    esi
        pop    edi
        add    edi, 20h
        loop   loc_3_0284
        pop    esi
loc_3_02B7:
        push   esi
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 4
        lea    esi, [esi + 8ah]
loc_3_02C8:
        push   esi
        push   edi
        push   ecx
        mov    cl, 8
        repe cmpsb
        pop    ecx
        pop    edi
        pop    esi
        je     NEAR PTR loc_3_03CC
        lea    edi, [edi + 0ah]
        loop   loc_3_02C8
loc_3_02DD:
        pop    esi
        db  0BDh, 014h, 001h, 000h, 000h
        test   byte ptr [esi + 24h], 2
        jne    NEAR PTR loc_3_037C
        mov    al, byte ptr [esi + 84h]
        and    al, 7
        cmp    al, 2
        jae    SHORT loc_3_034C
        db  0BDh, 008h, 001h, 000h, 000h
loc_3_02FE:
        db  0FEh, 005h, 01Ch, 002h, 000h, 000h
        mov    eax, dword ptr [esi + 1ch]
        sub    esp, 18h
        mov    edi, esp
        mov    word ptr [edi], 5
        mov    dword ptr [edi + 12h], eax
        mov    dword ptr [edi + 4], esi
        db  0C7h, 047h, 008h, 0C4h, 000h, 000h, 000h
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [edi + 16h], al
        push   1
        push   dword ptr [ebp + 4]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [eax], ebp
        mov    ax, word ptr [ebp + 2]
        mov    word ptr [edi + 10h], ax
        push   edi
        db  02Eh, 0FFh, 015h, 07Ch, 001h, 000h, 000h
        add    esp, 1ch
loc_3_034C:
        pop    esi
        pop    ebp
        ret
loc_3_034F:
        push   esp
        dec    edi
        push   edx
        imul   edx, dword ptr [ebx + 41h], 4443204eh
        sub    eax, 204d4f52h
        inc    ebx
        inc    esp
        push   edx
        db  02Dh, 053h, 031h
loc_3_0364:
        dec    ebp
        inc    ecx
        push   esp
        push   ebx
        dec    eax
        dec    ecx
        push   esp
        inc    ecx
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        inc    ebx
        push   edx
        sub    eax, 20313735h
        and    byte ptr [eax], ah
loc_3_037C:
        push   esi
        push   edi
        lea    esi, [esi + 8ah]
        db  08Dh, 03Dh, 04Fh, 003h, 000h, 000h
        mov    ecx, 15h
        repe cmpsb
        pop    edi
        pop    esi
        je     SHORT loc_3_03C0
        cmp    ecx, 1
        jne    SHORT loc_3_03A3
        cmp    byte ptr [esi + 9eh], 33h
        je     SHORT loc_3_03C0
loc_3_03A3:
        push   esi
        push   edi
        lea    esi, [esi + 8ah]
        db  08Dh, 03Dh, 064h, 003h, 000h, 000h
        mov    ecx, 6
        repe cmpsd
        pop    edi
        pop    esi
        jne    NEAR PTR loc_3_02FE
loc_3_03C0:
        or     byte ptr [esi + 482h], 40h
        jmp    NEAR PTR loc_3_02FE
loc_3_03CC:
        movzx  ecx, word ptr [edi + 8]
        mov    esi, dword ptr [esp]
        cmp    ecx, 0
        jne    SHORT loc_3_0423
        cmp    dword ptr [esi + 92h], 522d4443h
        jne    SHORT loc_3_0423
        cmp    dword ptr [esi + 96h], 44204d4fh
        jne    SHORT loc_3_0423
        cmp    dword ptr [esi + 9ah], 45564952h
        jne    SHORT loc_3_0423
        cmp    dword ptr [esi + 9eh], 3230353ah
        jne    SHORT loc_3_0423
        mov    eax, dword ptr [esi + 0a2h]
        xchg   ah, al
        ror    eax, 10h
        xchg   ah, al
        cmp    eax, 322e3079h
        jae    NEAR PTR loc_3_02DD
loc_3_0423:
        db  08Bh, 03Ch, 08Dh, 028h, 000h, 000h, 000h
        movzx  ecx, word ptr [edi]
        lea    edi, [edi + 2]
        lea    esi, [esi + 92h]
loc_3_0436:
        push   ecx
        push   esi
        push   edi
        mov    ecx, 4
        repe cmpsd
        pop    edi
        pop    esi
        pop    ecx
        je     SHORT loc_3_044F
        lea    edi, [edi + 12h]
        loop   loc_3_0436
        jmp    NEAR PTR loc_3_02DD
loc_3_044F:
        movzx  ebp, word ptr [edi + 10h]
        db  08Bh, 02Ch, 0ADh, 038h, 002h, 000h, 000h
        pop    esi
        or     byte ptr [esi + 482h], 8
        jmp    NEAR PTR loc_3_02FE
loc_3_0467:
        mov    esi, dword ptr [ebx + 0ch]
        cmp    byte ptr [esi + 74h], 1
        jne    SHORT loc_3_0488
        cmp    byte ptr [esi + 40h], 5
        jne    SHORT loc_3_0488
        mov    al, byte ptr [esi + 84h]
        and    al, 7
        cmp    al, 2
        jae    SHORT loc_3_0488
        db  0FEh, 00Dh, 01Ch, 002h, 000h, 000h
loc_3_0488:
        ret
loc_3_0489:
        db  080h, 03Dh, 01Ch, 002h, 000h, 000h, 000h
        jne    SHORT loc_3_0498
        mov    word ptr [ebx + 2], 0ffffh
loc_3_0498:
        ret
        db  000h, 000h, 000h
loc_3_049C:
        mov    esi, edx
        mov    ebx, dword ptr [esi + 8]
        test   byte ptr [esi + 6dh], 80h
        je     SHORT loc_3_0502
        mov    edi, dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 14h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 10h], eax
        movsx  eax, word ptr [eax + 10h]
        add    eax, esi
        test   byte ptr [eax + 0dh], 1
        je     SHORT loc_3_04C5
        cmp    dword ptr [eax + 1fh], 0
        jne    SHORT loc_3_0508
loc_3_04C5:
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_050F
loc_3_04CB:
        mov    ebx, dword ptr [esi + 10h]
        movsx  ebx, word ptr [ebx + 10h]
        add    ebx, esi
        test   byte ptr [ebx + 0dh], 1
        je     SHORT loc_3_0502
        push   edi
        push   esi
        mov    cl, byte ptr [ebx]
        mov    byte ptr [edi + 0ah], cl
        mov    ecx, dword ptr [ebx + 17h]
        mov    dword ptr [edi + 10h], ecx
        mov    ecx, dword ptr [ebx + 1bh]
        mov    dword ptr [edi + 0ch], ecx
        mov    ecx, dword ptr [ebx + 13h]
        mov    dword ptr [esi + 7ch], ecx
        lea    esi, [ebx + 1]
        lea    edi, [edi + 30h]
        mov    ecx, 3
        rep movsd
        pop    esi
        pop    edi
loc_3_0502:
        call   sub_3_0578
loc_3_0507:
        ret
loc_3_0508:
        call   dword ptr [eax + 1fh]
        cmp    al, 3
        jne    SHORT loc_3_0522
loc_3_050F:
        xor    eax, eax
        mov    edx, dword ptr [esi + 10h]
        mov    edx, dword ptr [edx + 8]
        mov    edx, dword ptr [edx]
        cmp    dword ptr [edx + 8], 0
        je     SHORT loc_3_0522
        call   dword ptr [edx + 8]
loc_3_0522:
        or     al, al
        je     SHORT loc_3_04CB
        cmp    al, 2
        je     SHORT loc_3_0507
        mov    eax, dword ptr [esi + 10h]
        add    dword ptr [esi + 14h], 8
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 10h], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        jmp    SHORT loc_3_0507
loc_3_053F:
        cmp    byte ptr [edi + 30h], 43h
        jne    SHORT loc_3_0575
        mov    byte ptr [edi + 3], 1
        mov    eax, dword ptr [esi + 80h]
        mov    dword ptr [eax], 1011200h
        add    eax, 4
        mov    dword ptr [eax], 10000h
        mov    dword ptr [eax + 4], 4e20h
        add    eax, 8
        mov    dword ptr [eax], 0aa0000h
        mov    dword ptr [eax + 4], 1770h
loc_3_0575:
        xor    al, al
        ret
sub_3_0578:
        mov    edi, dword ptr [esi + 14h]
        sub    edi, 8
        mov    dword ptr [esi + 14h], edi
        push   esi
        call   dword ptr [edi]
        add    esp, 4
        ret
sub_3_0588:
        push   ebx
        push   dword ptr [ebx + 10h]
        call   sub_3_05E0
        or     edi, edi
        je     SHORT loc_3_05BF
        mov    dword ptr [edi + 0a4h], edi
        pop    dword ptr [edi + 10h]
        mov    word ptr [edi + 68h], 12h
        mov    dword ptr [edi + 8], esi
        mov    ebx, edi
        call   sub_3_0664
        pop    dword ptr [ebx + 90h]
        mov    dl, byte ptr [esi + 7dh]
        shl    dl, 5
        mov    byte ptr [edi + 31h], dl
        clc
        ret
loc_3_05BF:
        pop    ebx
        pop    ebx
        stc
        ret
sub_3_05C3:
        push   esi
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        db  0FFh, 015h, 07Ch, 001h, 000h, 000h
        add    esp, 0ch
        pop    esi
        ret
sub_3_05E0:
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
        mov    byte ptr [edi + 0eh], 0
        mov    word ptr [edi], 2
        push   edi
        db  0FFh, 015h, 07Ch, 001h, 000h, 000h
        mov    edi, dword ptr [edi + 0ah]
        add    esp, 14h
        mov    eax, edi
        or     edi, edi
        je     SHORT loc_3_0662
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
loc_3_0662:
        pop    esi
        ret
sub_3_0664:
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
        mov    al, byte ptr [esi + 75h]
        mov    byte ptr [edi + 5], al
        mov    al, byte ptr [esi + 7ch]
        mov    byte ptr [edi + 6], al
        mov    al, byte ptr [esi + 7dh]
        mov    byte ptr [edi + 7], al
        mov    edx, edi
        add    edx, 50h
        mov    dword ptr [edi + 28h], edx
        movzx  eax, word ptr [esi + 80h]
        add    edx, eax
        mov    dword ptr [edi + 1ch], edx
        mov    byte ptr [edi + 0bh], 0eh
        pop    edx
        ret
sub_3_06C2:
        push   ebx
        db  0FFh, 015h, 090h, 001h, 000h, 000h
        add    esp, 4
        or     al, al
        je     SHORT loc_3_06D4
        or     dword ptr [ebx + 6ch], 40h
loc_3_06D4:
        mov    eax, dword ptr [ebx + 7ch]
        test   byte ptr [ebx + 6dh], 2
        jne    SHORT loc_3_06E3
        mul    dword ptr [esi + 0dbh]
loc_3_06E3:
        mov    dword ptr [edi + 10h], eax
        db  0BAh, 0C4h, 000h, 000h, 000h
        mov    byte ptr [edi + 3], 0
        mov    cx, word ptr [ebx + 0eh]
        mov    word ptr [ebx + 0ch], cx
        xchg   esi, edi
        xchg   edi, ebx
        mov    dword ptr [edi + 0a4h], edi
        db  0FFh, 015h, 088h, 001h, 000h, 000h
        ret
loc_3_0708:
        push   ebp
        push   edi
        push   esi
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [ebx + 20h]
        mov    eax, dword ptr [ebx + 10h]
        mov    ecx, dword ptr [ebx + 14h]
        mov    dword ptr [ecx + 4], eax
        db  0C7h, 001h, 040h, 000h, 000h, 000h
        mov    edx, dword ptr [eax + 8]
        mov    edx, dword ptr [edx]
        mov    al, byte ptr [edi + 30h]
        movzx  ecx, word ptr [edx]
        lea    edx, [edx + 0ch]
        jecxz  loc_3_073B
loc_3_0732:
        cmp    byte ptr [edx], al
        je     SHORT loc_3_0742
        lea    edx, [edx + 0ch]
        loop   loc_3_0732
loc_3_073B:
        mov    al, 1
        db  0E9h, 0B3h, 0F9h, 0FCh, 0FFh
loc_3_0742:
        push   edi
        push   esi
        lea    esi, [edi + 30h]
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        mov    cl, byte ptr [edi + 0ah]
        mov    byte ptr [eax], cl
        or     byte ptr [eax + 0dh], 1
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 17h], ecx
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 1bh], ecx
        mov    ecx, dword ptr [ebx + 7ch]
        mov    dword ptr [eax + 13h], ecx
        lea    edi, [eax + 1]
        mov    ecx, 3
        rep movsd
        pop    esi
        pop    edi
        mov    ecx, dword ptr [edx + 8]
        mov    dword ptr [eax + 1fh], ecx
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [edi + 30h], al
        xor    eax, eax
        cmp    dword ptr [edx + 4], eax
        db  00Fh, 084h, 067h, 0F9h, 0FCh, 0FFh
        call   dword ptr [edx + 4]
        db  0E9h, 05Fh, 0F9h, 0FCh, 0FFh
sub_3_0796:
        push   edx
        mov    eax, 1194h
        movzx  cx, ch
        mul    cx
        shl    edx, 10h
        add    eax, edx
        ror    ecx, 10h
        movzx  edx, ch
        add    edx, eax
        xchg   ecx, edx
        mov    eax, 4bh
        mul    dl
        add    ecx, eax
        cmp    ecx, 96h
        jb     SHORT loc_3_07C9
        sub    ecx, 96h
loc_3_07C9:
        pop    edx
        ret
sub_3_07CB:
        push   edx
        push   ebx
        add    ecx, 96h
        mov    ebx, 1194h
        mov    eax, ecx
        xor    edx, edx
        div    ebx
        movzx  ecx, al
        mov    eax, edx
        mov    ebx, 4bh
        xor    edx, edx
        div    bx
        mov    ch, al
        shl    ecx, 8
        shl    edx, 18h
        or     ecx, edx
        pop    ebx
        pop    edx
        ret
sub_3_07FA:
        mov    ecx, 3
        mov    eax, ebx
loc_3_0801:
        rol    eax, 8
        aam    0ah
        shl    ah, 4
        or     ah, al
        mov    bl, ah
        shl    ebx, 8
        loop   loc_3_0801
        ret
sub_3_0813:
        xor    al, al
        push   ecx
        mov    cl, 3
        movzx  ecx, cl
loc_3_081B:
        rol    eax, 8
        shl    ax, 4
        shr    al, 4
        aad    0ah
        shl    ax, 8
        loop   loc_3_081B
        pop    ecx
        ret
        db  000h
loc_3_0830:
        call   sub_3_1101
        jb     SHORT loc_3_0844
        test   dl, 4
        jne    SHORT loc_3_0844
        mov    byte ptr [edi + 30h], 0d8h
        mov    byte ptr [edi + 0ah], 0ah
loc_3_0844:
        xor    al, al
        ret
sub_3_0847:
        mov    cl, byte ptr [esi + 84h]
        and    cl, 7
        cmp    cl, 2
        jae    NEAR PTR loc_3_0DC2
        mov    ecx, dword ptr [edi + 32h]
        call   sub_3_0796
        mov    ebp, ecx
        xchg   cl, ch
        ror    ecx, 10h
        xchg   cl, ch
        mov    dword ptr [edi + 32h], ecx
        mov    ecx, dword ptr [edi + 35h]
        call   sub_3_0796
        jmp    SHORT loc_3_08A9
sub_3_0877:
        movzx  ecx, word ptr [edi + 37h]
        shl    ecx, 10h
        jecxz  loc_3_08C3
        mov    dword ptr [edi + 36h], ecx
sub_3_0883:
        mov    al, byte ptr [esi + 84h]
        and    al, 7
        cmp    al, 2
        jae    NEAR PTR loc_3_0DC2
        mov    ecx, dword ptr [edi + 36h]
        xchg   cl, ch
        ror    ecx, 10h
        xchg   cl, ch
        mov    eax, dword ptr [edi + 32h]
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
        add    ecx, eax
loc_3_08A9:
        mov    byte ptr [edi + 30h], 0d8h
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        mov    dword ptr [eax + 0fh], ecx
        xor    eax, eax
        mov    dword ptr [edi + 36h], eax
        mov    byte ptr [edi + 0ah], 0ah
        ret
loc_3_08C3:
        mov    byte ptr [edi + 3], 1
        mov    al, 2
        ret
sub_3_08CA:
        mov    ecx, dword ptr [eax + 0fh]
        xchg   ch, cl
        ror    ecx, 10h
        xchg   ch, cl
        cmp    ecx, dword ptr [edi + 32h]
        je     SHORT loc_3_08FF
        cmp    byte ptr [edi + 3], 1
        jne    NEAR PTR loc_3_10A3
        mov    dword ptr [eax + 1fh], 0
        mov    dword ptr [edi + 32h], ecx
        mov    ebx, dword ptr [esi + 10h]
        mov    ebx, dword ptr [ebx + 8]
        mov    ah, byte ptr [ebx + 4]
        mov    al, 0d9h
        mov    word ptr [edi + 30h], ax
        mov    al, 1
        ret
loc_3_08FF:
        mov    byte ptr [edi + 3], 1
        xor    al, al
        ret
loc_3_0906:
        call   sub_3_0847
        jmp    SHORT loc_3_0919
loc_3_090D:
        call   sub_3_0883
        jmp    SHORT loc_3_0919
loc_3_0914:
        call   sub_3_0877
loc_3_0919:
        cmp    al, 2
        je     SHORT loc_3_0941
loc_3_091D:
        mov    ecx, dword ptr [edi + 32h]
        xchg   cl, ch
        ror    ecx, 10h
        xchg   cl, ch
        push   ebx
        push   eax
        call   sub_3_07CB
        mov    ebx, ecx
        call   sub_3_07FA
        pop    eax
        ror    ebx, 8
        mov    dword ptr [edi + 32h], ebx
        pop    ebx
        mov    byte ptr [edi + 39h], 40h
loc_3_0941:
        ret
loc_3_0942:
        call   sub_3_08CA
        cmp    al, 1
        jne    SHORT loc_3_0941
        jmp    SHORT loc_3_091D
loc_3_094D:
        cmp    byte ptr [edi + 39h], 80h
        je     NEAR PTR loc_3_0A25
        call   sub_3_0588
        jb     NEAR PTR loc_3_09F8
        sub    esp, 10h
        mov    edx, esp
        mov    word ptr [edx + 4], 41ch
        mov    word ptr [edx + 0ah], 9
        mov    word ptr [edx], 3
        mov    byte ptr [edx + 0ch], 8
        push   edx
        db  0FFh, 015h, 07Ch, 001h, 000h, 000h
        mov    eax, dword ptr [edx + 6]
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_3_0A09
        mov    dword ptr [ebx + 80h], eax
        mov    dword ptr [ebx + 7ch], 41ch
        and    dword ptr [ebx + 6ch], 0fffffffdh
        or     dword ptr [ebx + 6ch], 200h
        mov    dword ptr [edi + 0ch], 248h
        mov    byte ptr [edi + 0ah], 0ah
        mov    word ptr [edi + 30h], 3deh
        xor    eax, eax
        mov    dword ptr [edi + 32h], eax
        mov    dword ptr [edi + 36h], eax
        mov    byte ptr [ebx + 0b0h], al
        db  0C7h, 043h, 070h, 052h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        db  0C7h, 040h, 01Fh, 054h, 00Ah, 000h, 000h
        mov    eax, dword ptr [ebx + 90h]
        mov    eax, dword ptr [eax + 20h]
        test   byte ptr [eax + 39h], 40h
        jne    SHORT loc_3_0A1F
loc_3_09EA:
        push   ebx
        push   edi
        push   esi
        call   sub_3_06C2
        pop    esi
        pop    edi
        pop    ebx
        mov    al, 3
        ret
loc_3_09F8:
        mov    word ptr [ebx + 6ah], 1dh
        mov    byte ptr [edi + 3], 4
        mov    byte ptr [edi + 4], 8
        mov    al, 2
        ret
loc_3_0A09:
        push   dword ptr [ebx + 90h]
        mov    edi, ebx
        mov    ebx, dword ptr [edi + 8]
        call   sub_3_05C3
        pop    ebx
        mov    edi, dword ptr [ebx + 20h]
        jmp    SHORT loc_3_09F8
loc_3_0A1F:
        mov    byte ptr [edi + 32h], 0b0h
        jmp    SHORT loc_3_09EA
loc_3_0A25:
        mov    word ptr [ebx + 6ah], 16h
        mov    byte ptr [edi + 3], 86h
        mov    byte ptr [edi + 4], 2
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [eax], 50070h
        mov    dword ptr [eax + 4], 0b000000h
        xor    ecx, ecx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0eh], ecx
        mov    word ptr [eax + 0ch], 24h
        mov    al, 2
        ret
loc_3_0A54:
        cmp    byte ptr [edi + 3], 1
        je     SHORT loc_3_0A64
        cmp    byte ptr [edi + 3], 12h
        jne    NEAR PTR loc_3_0BF8
loc_3_0A64:
        mov    ebp, dword ptr [esi + 80h]
        mov    ecx, dword ptr [esi + 90h]
        mov    edx, dword ptr [ecx + 80h]
        cmp    byte ptr [edi + 32h], 0b0h
        je     NEAR PTR loc_3_0C4F
        movzx  eax, byte ptr [ebp + 13h]
        shl    ax, 4
        shr    al, 4
        aad    0ah
        mov    bl, al
        movzx  eax, byte ptr [ebp + 9]
        shl    ax, 4
        shr    al, 4
        aad    0ah
        mov    ah, bl
        mov    ebp, dword ptr [ecx + 20h]
        mov    word ptr [edx + 2], ax
        movzx  ecx, byte ptr [ebp + 36h]
        or     ecx, ecx
        je     NEAR PTR loc_3_0BF1
        cmp    cl, al
        jb     NEAR PTR loc_3_0C2A
        cmp    cl, ah
        ja     NEAR PTR loc_3_0C2A
loc_3_0AC1:
        sub    ah, cl
        sub    cl, al
        xchg   cl, ah
        movzx  ebx, ah
        inc    ecx
        lea    ebx, [ebx + ebx*4]
        lea    eax, [ecx*8 + 0ah]
        xchg   al, ah
        mov    word ptr [edx], ax
        push   edi
        push   esi
        lea    edi, [edx + 4]
        mov    esi, dword ptr [esi + 80h]
        lea    esi, [esi + ebx*2 + 20h]
loc_3_0AE9:
        xor    eax, eax
        lodsw
        ror    al, 4
        shl    eax, 18h
        lodsb
        shl    ax, 4
        shr    al, 4
        aad    0ah
        ror    eax, 10h
        stosd
        lea    esi, [esi + 3]
        lodsd
        call   sub_3_0813
        test   byte ptr [ebp + 31h], 2
        jne    SHORT loc_3_0B22
        push   ecx
        mov    ecx, eax
        call   sub_3_0796
        mov    eax, ecx
        pop    ecx
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
loc_3_0B22:
        stosd
        loop   loc_3_0AE9
        mov    eax, 0aa0000h
        stosd
        pop    esi
        mov    ebx, dword ptr [esi + 80h]
        mov    eax, dword ptr [ebx + 1ch]
        call   sub_3_0813
        test   byte ptr [ebp + 31h], 2
        jne    SHORT loc_3_0B50
        mov    ecx, eax
        call   sub_3_0796
        mov    eax, ecx
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
loc_3_0B50:
        stosd
        pop    edi
        mov    byte ptr [ebp + 3], 1
        mov    ebx, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [ecx + 8]
        mov    byte ptr [ecx + 5], 0
        mov    eax, dword ptr [esi + 80h]
        cmp    byte ptr [eax + 0ah], 20h
        jne    SHORT loc_3_0BC1
        mov    byte ptr [ecx + 5], 0c0h
        mov    dword ptr [esi + 7ch], 0ah
        mov    dword ptr [edi + 30h], 15h
        mov    word ptr [edi + 34h], 0ah
        mov    byte ptr [edi + 0ah], 6
        mov    dword ptr [edi + 0ch], 288h
        xor    edx, edx
        mov    dword ptr [eax], edx
        mov    byte ptr [eax + 4], 0c0h
        mov    dword ptr [eax + 5], edx
        mov    byte ptr [eax + 9], 0fh
        mov    eax, dword ptr [esi + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, esi
        db  0C7h, 040h, 01Fh, 0C1h, 00Bh, 000h, 000h
        push   ebx
        push   edi
        push   esi
        xchg   esi, ebx
        call   sub_3_06C2
        pop    esi
        pop    edi
        pop    ebx
        mov    al, 2
        ret
loc_3_0BC1:
        mov    ebx, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 80h]
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], edx
        mov    word ptr [eax], 4
        push   eax
        db  0FFh, 015h, 07Ch, 001h, 000h, 000h
        add    esp, 0ch
        mov    edi, esi
        mov    esi, dword ptr [edi + 90h]
        call   sub_3_05C3
        xor    al, al
        ret
loc_3_0BF1:
        mov    cl, al
        jmp    NEAR PTR loc_3_0AC1
loc_3_0BF8:
        mov    eax, dword ptr [esi + 90h]
        mov    eax, dword ptr [eax + 20h]
        movzx  ecx, word ptr [edi + 3]
        mov    word ptr [eax + 3], cx
        push   edi
        push   esi
        movzx  ecx, byte ptr [eax + 0bh]
        mov    esi, dword ptr [edi + 1ch]
        mov    edi, dword ptr [eax + 1ch]
        rep movsb
        pop    esi
        pop    edi
        jmp    SHORT loc_3_0BC1
loc_3_0C1B:
        jo     SHORT loc_3_0C1D
loc_3_0C1D:
        add    eax, 60000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        db  000h
loc_3_0C2A:
        mov    eax, dword ptr [esi + 90h]
        mov    eax, dword ptr [eax + 20h]
        mov    word ptr [eax + 3], 284h
        mov    edi, dword ptr [eax + 1ch]
        push   esi
        db  0BEh, 01Bh, 00Ch, 000h, 000h
        mov    ecx, 0fh
        rep movsb
        pop    esi
        jmp    NEAR PTR loc_3_0BC1
loc_3_0C4F:
        mov    ebx, dword ptr [ecx + 20h]
        mov    word ptr [edx], 0a00h
        cmp    byte ptr [ebp + 0eh], 0b0h
        je     SHORT loc_3_0C83
        mov    word ptr [edx + 2], 101h
        mov    eax, 20000h
        test   byte ptr [ebx + 31h], 2
        jne    SHORT loc_3_0C70
        xor    eax, eax
loc_3_0C70:
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 4], 100h
        mov    byte ptr [ebx + 3], 1
        jmp    NEAR PTR loc_3_0BC1
loc_3_0C83:
        mov    word ptr [edx + 2], 201h
        mov    eax, dword ptr [ebp + 0eh]
        call   sub_3_0813
        ror    eax, 8
        add    ah, 2
        cmp    ah, 3ch
        jb     SHORT loc_3_0CA1
        sub    ah, 3ch
        inc    al
loc_3_0CA1:
        rol    eax, 8
        test   byte ptr [ebx + 31h], 2
        jne    SHORT loc_3_0C70
        mov    ecx, eax
        call   sub_3_0796
        xchg   ch, cl
        ror    ecx, 10h
        xchg   ch, cl
        mov    eax, ecx
        jmp    SHORT loc_3_0C70
loc_3_0CBC:
        mov    ebp, dword ptr [ebx + 80h]
        movzx  eax, byte ptr [ebp + 3]
        cmp    byte ptr [eax + ebp + 4], 1
        je     SHORT loc_3_0D40
        lea    ebp, [eax + ebp + 4]
        cmp    byte ptr [ebp], 0eh
        jne    SHORT loc_3_0D31
        mov    cl, byte ptr [esi + 84h]
        and    cl, 7
        cmp    cl, 2
        jae    NEAR PTR loc_3_0DC2
        xor    edx, edx
        movzx  eax, byte ptr [ebp + 0bh]
        sub    eax, 1
        cmc
        rcl    dh, 1
        movzx  eax, byte ptr [ebp + 9]
        sub    eax, 1
        cmc
        rcl    dh, 1
        mov    eax, dword ptr [ebx + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    byte ptr [eax + 4], dh
        test   byte ptr [esi + 47eh], 1
        mov    byte ptr [edi + 0ah], 0ah
        mov    dl, 0d9h
        mov    dword ptr [edi + 30h], edx
        xor    eax, eax
        mov    dword ptr [ebx + 7ch], eax
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 0ch], 0
        mov    dword ptr [edi + 34h], eax
        mov    ah, 0c0h
        mov    word ptr [edi + 38h], ax
        ret
loc_3_0D31:
        mov    al, 1
        ret
loc_3_0D34:
        or     byte ptr [eax], al
        or     dword ptr [eax], esp
        or     dword ptr [ecx + ecx], esp
        db  030h
loc_3_0D3C:
        pop    ss
        pop    es
        add    al, byte ptr [eax]
loc_3_0D40:
        movzx  eax, word ptr [ebp + 0ah]
        push   edi
        db  0BFh, 034h, 00Dh, 000h, 000h
        mov    ecx, 4
        repne scasw
        pop    edi
        jne    NEAR PTR loc_3_0A25
        mov    edx, dword ptr [ebx + 10h]
        mov    edx, dword ptr [edx + 8]
        movzx  eax, byte ptr [edx + 5]
        and    al, 0c0h
        db  00Ah, 081h, 03Ch, 00Dh, 000h, 000h
        mov    byte ptr [edx + 5], al
        mov    cl, byte ptr [esi + 84h]
        and    cl, 7
        cmp    cl, 2
        jae    SHORT loc_3_0DB8
        and    byte ptr [edi + 31h], 0efh
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [ebx + 5ch], ecx
        mov    cl, 0ah
        movzx  ecx, cl
        mov    dword ptr [ebx + 7ch], ecx
        mov    dword ptr [edi + 10h], ecx
        mov    byte ptr [edi + 34h], cl
        and    al, 0efh
        xor    ecx, ecx
        mov    dword ptr [ebp], ecx
        mov    dword ptr [ebp + 4], eax
        mov    byte ptr [ebp + 8], cl
        mov    byte ptr [ebp + 9], 0ah
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        db  0C7h, 040h, 01Fh, 0D5h, 00Dh, 000h, 000h
        xor    al, al
        ret
loc_3_0DB8:
        test   al, 10h
        je     SHORT loc_3_0DC2
        mov    word ptr [ebp + 0ah], 2409h
loc_3_0DC2:
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        mov    dword ptr [eax + 1fh], 0
        xor    al, al
        ret
loc_3_0DD5:
        mov    ecx, dword ptr [esi + 80h]
        mov    eax, dword ptr [esi + 5ch]
        mov    dword ptr [ecx], eax
        xor    al, al
        mov    byte ptr [ecx + 4], al
        ret
loc_3_0DE6:
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_3_0DF9
        mov    eax, dword ptr [edi + 1ch]
        cmp    byte ptr [eax + 0ch], 2ch
        jne    SHORT loc_3_0DF9
        mov    byte ptr [edi + 3], 1
loc_3_0DF9:
        xor    al, al
        ret
loc_3_0DFC:
        sbb    eax, dword ptr [ecx]
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        push   cs
        push   cs
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0E18:
        mov    al, byte ptr [edi + 32h]
        and    al, 3fh
        cmp    al, 1
        je     SHORT loc_3_0E93
        cmp    al, 0eh
        jne    NEAR PTR loc_3_0D31
        test   byte ptr [edi + 32h], 40h
        jne    SHORT loc_3_0E3D
        mov    cl, byte ptr [esi + 84h]
        and    cl, 7
        cmp    cl, 2
        jae    SHORT loc_3_0DC2
loc_3_0E3D:
        cmp    dword ptr [edi + 10h], 1bh
        jb     NEAR PTR loc_3_0D31
        push   ebp
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 80h]
        mov    ebp, edi
        db  08Dh, 035h, 0FCh, 00Dh, 000h, 000h
        mov    ecx, 1bh
        rep movsb
        pop    esi
        pop    edi
        mov    eax, dword ptr [ebx + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax + 4]
        rcr    al, 1
        mov    dl, 0ffh
        test   byte ptr [edi + 32h], 40h
        jne    SHORT loc_3_0E79
        sbb    dl, dl
loc_3_0E79:
        mov    byte ptr [ebp + 15h], dl
        rcr    al, 1
        mov    dl, 0ffh
        test   byte ptr [edi + 32h], 40h
        jne    SHORT loc_3_0E88
        sbb    dl, dl
loc_3_0E88:
        mov    byte ptr [ebp + 17h], dl
        mov    byte ptr [edi + 3], 1
        pop    ebp
        mov    al, 2
        ret
loc_3_0E93:
        cmp    dword ptr [ebx + 7ch], 14h
        test   byte ptr [edi + 32h], 40h
        jne    SHORT loc_3_0EEA
        mov    eax, dword ptr [ebx + 10h]
        mov    eax, dword ptr [eax + 8]
        movzx  eax, byte ptr [eax + 5]
        and    al, 3fh
        push   edi
        mov    ecx, 4
        db  0BFh, 03Ch, 00Dh, 000h, 000h
        repne scasb
        pop    edi
        db  00Fh, 0B7h, 004h, 04Dh, 034h, 00Dh, 000h, 000h
loc_3_0EBF:
        mov    ecx, dword ptr [ebx + 80h]
        mov    dword ptr [ecx], 8000013h
        shl    eax, 10h
        mov    dword ptr [ecx + 8], eax
        xor    eax, eax
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 0ch], 80001h
        mov    dword ptr [ecx + 10h], eax
        mov    byte ptr [edi + 3], 1
        mov    eax, 2
        ret
loc_3_0EEA:
        mov    eax, 0ff0fh
        jmp    SHORT loc_3_0EBF
loc_3_0EF1:
        cmp    byte ptr [edi + 33h], 3
        ja     SHORT loc_3_0F14
        mov    dword ptr [ebx + 7ch], 0ah
        mov    dword ptr [edi + 10h], 0ah
        mov    word ptr [edi + 30h], 0addh
        xor    eax, eax
        mov    dword ptr [edi + 32h], eax
        mov    dword ptr [edi + 36h], eax
        ret
loc_3_0F14:
        mov    al, 1
        ret
loc_3_0F17:
        adc    dword ptr [eax], eax
        adc    dl, byte ptr [ebx]
loc_3_0F1B:
        sub    ecx, dword ptr [ebx]
        adc    edx, dword ptr [ebx]
loc_3_0F1F:
        cmp    byte ptr [edi + 3], 1
        jne    NEAR PTR loc_3_0FF0
        mov    ebx, dword ptr [esi + 80h]
        movzx  eax, byte ptr [ebx]
        db  00Fh, 0B6h, 080h, 017h, 00Fh, 000h, 000h
        mov    ebp, dword ptr [esi + 10h]
        movsx  ebp, word ptr [ebp + 10h]
        add    ebp, esi
        movzx  ecx, byte ptr [ebp + 4]
        cmp    cl, 3
        ja     NEAR PTR loc_3_0FF0
        mov    edx, dword ptr [ebx + 1]
        push   edi
        lea    edi, [ebx + 5]
        mov    byte ptr [ebx + 4], cl
        cmp    cl, 1
        pushfd
        db  00Fh, 0B6h, 089h, 01Bh, 00Fh, 000h, 000h
        shl    ecx, 18h
        mov    dword ptr [ebx], ecx
        shr    ecx, 18h
        mov    byte ptr [ebx + 1], al
        popfd
        ja     SHORT loc_3_0FEA
        mov    al, dl
        push   word ptr [ebx + 9]
        push   dword ptr [ebx + 5]
        shl    al, 4
        stosb
        movzx  eax, dh
        shl    eax, 4
        shr    al, 4
        aad    0ah
        shr    edx, 10h
        mov    ah, dh
        xor    dh, dh
        shl    dx, 4
        shr    dl, 4
        xchg   dx, ax
        aad    0ah
        mov    ah, al
        mov    al, dl
        xor    dl, dl
        stosw
        shrd   eax, edx, 10h
        pop    ax
        ror    eax, 10h
        call   sub_3_0813
        pop    edx
        shl    edx, 8
        xchg   edx, eax
        call   sub_3_0813
        test   byte ptr [ebp + 2], 2
        jne    SHORT loc_3_0FE3
        push   ecx
        mov    ecx, eax
        call   sub_3_0796
        xchg   cl, ch
        ror    ecx, 10h
        xchg   cl, ch
        xchg   edx, ecx
        call   sub_3_0796
        xchg   cl, ch
        ror    ecx, 10h
        xchg   cl, ch
        mov    eax, ecx
        pop    ecx
        xchg   edx, eax
loc_3_0FE3:
        stosd
        mov    eax, edx
        stosd
        sub    ecx, 0bh
loc_3_0FEA:
        xor    al, al
        rep stosb
        pop    edi
        ret
loc_3_0FF0:
        xor    al, al
        ret
loc_3_0FF3:
        mov    word ptr [ebx + 0eh], 3ch
        mov    word ptr [ebx + 0ch], 3ch
        call   sub_3_1101
        jb     SHORT loc_3_100B
        test   dl, 4
        je     SHORT loc_3_1063
loc_3_100B:
        mov    edx, dword ptr [ebx + 10h]
        mov    edx, dword ptr [edx + 8]
        test   byte ptr [edx + 5], 10h
        je     SHORT loc_3_1060
        cmp    dword ptr [ebx + 7ch], 0ch
        jb     SHORT loc_3_1060
        mov    eax, dword ptr [ebx + 80h]
        mov    dword ptr [eax], 0fffffffh
        mov    dword ptr [eax + 4], 0ffffffffh
        mov    dword ptr [eax + 8], 0fffffff0h
        add    dword ptr [ebx + 80h], 0ch
        sub    dword ptr [ebx + 7ch], 0ch
        sub    dword ptr [edi + 10h], 0ch
        push   ebx
        db  0FFh, 015h, 090h, 001h, 000h, 000h
        add    esp, 4
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        db  0C7h, 040h, 01Fh, 08Dh, 010h, 000h, 000h
loc_3_1060:
        xor    al, al
        ret
loc_3_1063:
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        db  0C7h, 040h, 01Fh, 0E1h, 010h, 000h, 000h
        xor    eax, eax
        mov    dword ptr [edi + 30h], eax
        mov    dword ptr [edi + 34h], eax
        mov    dword ptr [ebx + 7ch], eax
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 0ch], 0
        mov    byte ptr [edi + 0ah], 6
        ret
loc_3_108D:
        add    dword ptr [esi + 7ch], 0ch
        sub    dword ptr [esi + 80h], 0ch
        push   esi
        db  0FFh, 015h, 090h, 001h, 000h, 000h
        lea    esp, [esp + 4]
loc_3_10A3:
        cmp    byte ptr [edi + 3], 1
        je     SHORT loc_3_10D8
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_3_10D8
        mov    eax, dword ptr [edi + 1ch]
        mov    cl, byte ptr [eax + 0ch]
        cmp    cl, 1dh
        je     SHORT loc_3_10C9
        cmp    cl, 21h
        je     SHORT loc_3_10C9
        cmp    cl, 0dh
        je     SHORT loc_3_10DB
        cmp    cl, 1ch
        jne    SHORT loc_3_10D8
loc_3_10C9:
        mov    eax, dword ptr [edi + 1ch]
        mov    byte ptr [edi + 3], 84h
        mov    byte ptr [eax + 0ch], 64h
        mov    byte ptr [eax + 2], 5
loc_3_10D8:
        xor    al, al
        ret
loc_3_10DB:
        mov    byte ptr [eax + 0ch], 3ah
        jmp    SHORT loc_3_10D8
loc_3_10E1:
        cmp    byte ptr [edi + 3], 1
        je     SHORT loc_3_10C9
        xor    al, al
        ret
loc_3_10EA:
        xor    eax, eax
        cmp    byte ptr [edi + 34h], 2
        jne    SHORT loc_3_1100
        mov    byte ptr [edi + 30h], 0dch
        mov    byte ptr [edi + 0ah], 0ah
        mov    dword ptr [edi + 32h], eax
        mov    dword ptr [edi + 36h], eax
loc_3_1100:
        ret
sub_3_1101:
        push   ebx
        mov    esi, dword ptr [ebx + 8]
        movzx  ecx, word ptr [esi + 134h]
        xchg   cl, ch
        movzx  eax, byte ptr [esi + 137h]
        sub    al, byte ptr [esi + 136h]
        inc    eax
        mov    edx, eax
        imul   eax, eax, 8
        cmp    eax, ecx
        jg     SHORT loc_3_115D
        cmp    ecx, 324h
        jg     SHORT loc_3_115D
        lea    ebp, [esi + 138h]
        mov    ecx, edx
        movzx  edx, byte ptr [ebp + 1]
        mov    ebx, dword ptr [edi + 32h]
        xchg   bl, bh
        rol    ebx, 10h
        xchg   bl, bh
        inc    ecx
loc_3_1144:
        push   ecx
        mov    ecx, dword ptr [ebp + 4]
        call   sub_3_0796
        mov    eax, ecx
        pop    ecx
        cmp    ebx, eax
        jb     SHORT loc_3_115D
        movzx  edx, byte ptr [ebp + 1]
        lea    ebp, [ebp + 8]
        loop   loc_3_1144
loc_3_115D:
        pop    ebx
        cmc
        ret
loc_3_1160:
        cmp    byte ptr [edi + 39h], 80h
        je     NEAR PTR loc_3_0A25
        cmp    byte ptr [edi + 39h], 40h
        jne    SHORT loc_3_11A1
        mov    byte ptr [edi + 30h], 0c7h
        mov    byte ptr [edi + 31h], 3
        xor    ecx, ecx
        mov    dword ptr [edi + 32h], ecx
        mov    dword ptr [edi + 36h], ecx
        mov    dword ptr [edi + 10h], 4
        mov    dword ptr [ebx + 7ch], 4
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        db  0C7h, 040h, 01Fh, 0A4h, 011h, 000h, 000h
        xor    al, al
        ret
loc_3_11A1:
        mov    al, 1
        ret
loc_3_11A4:
        cmp    byte ptr [edi + 3], 1
        je     SHORT loc_3_11B0
        cmp    byte ptr [edi + 3], 12h
        jne    SHORT loc_3_120E
loc_3_11B0:
        mov    edx, dword ptr [esi + 80h]
        mov    eax, dword ptr [edx]
        mov    ebp, dword ptr [esi + 10h]
        mov    ecx, dword ptr [ebp + 8]
        or     al, al
        je     SHORT loc_3_11C4
        mov    al, 83h
loc_3_11C4:
        mov    byte ptr [ecx + 5], al
        call   sub_3_0813
        movsx  ebp, word ptr [ebp + 10h]
        add    ebp, esi
        test   byte ptr [ebp + 2], 2
        jne    SHORT loc_3_11E8
        mov    ecx, eax
        call   sub_3_0796
        mov    eax, ecx
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
loc_3_11E8:
        mov    word ptr [edx], 0a00h
        or     eax, eax
        jne    SHORT loc_3_1211
        mov    word ptr [edx + 2], 101h
        mov    eax, 20000h
        test   byte ptr [ebp + 2], 2
        jne    SHORT loc_3_1204
        xor    eax, eax
loc_3_1204:
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 4], 100h
loc_3_120E:
        xor    al, al
        ret
loc_3_1211:
        mov    ecx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [ecx + 8]
        mov    byte ptr [ecx + 5], 83h
        mov    word ptr [edx + 2], 201h
        jmp    SHORT loc_3_1204
loc_3_1223:
        mov    eax, dword ptr [ebx + 10h]
        mov    eax, dword ptr [eax + 8]
        cmp    byte ptr [eax + 4], 82h
        je     NEAR PTR loc_3_12BB
        mov    dl, 82h
loc_3_1235:
        push   edx
        call   sub_3_0588
        pop    edx
        jb     NEAR PTR loc_3_09F8
        lea    ecx, [esi + 45ch]
        mov    dword ptr [ebx + 80h], ecx
        mov    byte ptr [ecx], 0
        mov    byte ptr [ecx + 3], 8
        mov    byte ptr [ecx + 4], dl
        mov    eax, 930h
        cmp    dl, 82h
        je     SHORT loc_3_1268
        mov    eax, dword ptr [esi + 490h]
loc_3_1268:
        xchg   al, ah
        mov    word ptr [ecx + 0ah], ax
        mov    byte ptr [edi + 30h], 15h
        mov    byte ptr [edi + 0ah], 6
        xor    ecx, ecx
        mov    word ptr [edi + 32h], cx
        mov    byte ptr [edi + 35h], cl
        mov    cl, 14h
        mov    dword ptr [ebx + 7ch], ecx
        mov    byte ptr [edi + 34h], cl
        or     byte ptr [edi + 31h], 10h
        or     byte ptr [ebx + 6dh], 2
        mov    dword ptr [edi + 0ch], 288h
        db  0C7h, 043h, 070h, 052h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        db  0C7h, 040h, 01Fh, 0BEh, 012h, 000h, 000h
        push   ebx
        push   edi
        push   esi
        call   sub_3_06C2
        pop    esi
        pop    edi
        pop    ebx
        mov    al, 3
        ret
loc_3_12BB:
        mov    al, 1
        ret
loc_3_12BE:
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_12D6
        mov    edx, dword ptr [esi + 80h]
        mov    dl, byte ptr [edx + 4]
        mov    eax, dword ptr [esi + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    byte ptr [eax + 4], dl
loc_3_12D6:
        mov    edi, esi
        mov    esi, dword ptr [edi + 90h]
        call   sub_3_05C3
        mov    al, 1
        ret
loc_3_12E6:
        mov    eax, dword ptr [ebx + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dl, byte ptr [eax + 5]
        cmp    dl, byte ptr [eax + 4]
        jne    NEAR PTR loc_3_1235
loc_3_12F8:
        mov    al, 1
        ret
loc_3_12FB:
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [eax + 4], 83h
        jne    SHORT loc_3_12F8
        cmp    word ptr [eax + 0ah], 3009h
        jne    SHORT loc_3_12F8
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        db  0C7h, 040h, 01Fh, 026h, 013h, 000h, 000h
        xor    al, al
        ret
loc_3_1326:
        mov    eax, dword ptr [esi + 80h]
        mov    byte ptr [eax + 4], 83h
        xor    al, al
        ret
loc_3_1333:
        mov    eax, dword ptr [ebx + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    byte ptr [eax + 4], 0ffh
        mov    al, 1
        ret
loc_3_1340:
        or     byte ptr [edi + 31h], 2
        xor    al, al
        ret
loc_3_1347:
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_138C
        mov    eax, dword ptr [esi + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, esi
        test   byte ptr [eax + 2], 2
        jne    SHORT loc_3_138C
        cmp    byte ptr [edi + 33h], 1
        ja     SHORT loc_3_138C
        mov    edx, dword ptr [esi + 80h]
        mov    ecx, dword ptr [edx + 8]
        call   sub_3_0796
        xchg   ch, cl
        ror    ecx, 10h
        xchg   ch, cl
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [edx + 0ch]
        call   sub_3_0796
        xchg   ch, cl
        ror    ecx, 10h
        xchg   ch, cl
        mov    dword ptr [edx + 0ch], ecx
loc_3_138C:
        xor    al, al
        ret
loc_3_138F:
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [eax + 3], 8
        jne    SHORT loc_3_1414
        mov    edx, dword ptr [ebx + 10h]
        mov    edx, dword ptr [edx + 8]
        movzx  ecx, word ptr [eax + 0ah]
        cmp    ecx, dword ptr [edx + 4]
        je     SHORT loc_3_13CB
        cmp    cx, 8
        je     SHORT loc_3_13B7
        cmp    cx, 3009h
        jne    SHORT loc_3_1414
loc_3_13B7:
        mov    dword ptr [edx + 4], ecx
        cmp    byte ptr [eax + 0ch], 1
        jne    SHORT loc_3_13CB
        mov    byte ptr [edi + 3], 1
        mov    byte ptr [edi + 4], 0
        mov    al, 2
        ret
loc_3_13CB:
        xor    ecx, ecx
        mov    byte ptr [edi + 30h], 55h
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebx + 7ch]
        shl    ecx, 8
        sub    ch, 6
        sub    ch, 6
        shr    ecx, 8
        push   edi
        push   esi
        lea    edi, [eax + 8]
        lea    esi, [eax + 0ch]
        rep movsb
        pop    esi
        pop    edi
        movzx  eax, byte ptr [eax + 8]
        mov    dword ptr [edi + 32h], eax
loc_3_13F8:
        mov    eax, dword ptr [ebx + 7ch]
        sub    al, 4
        mov    byte ptr [edi + 34h], 0
        mov    dword ptr [edi + 38h], eax
        mov    dword ptr [ebx + 7ch], eax
        mov    dword ptr [edi + 10h], eax
        xor    eax, eax
        mov    dword ptr [edi + 4], eax
        mov    byte ptr [edi + 0ah], 0ch
        ret
loc_3_1414:
        jmp    NEAR PTR loc_3_0A25
loc_3_1419:
        mov    eax, dword ptr [esi + 80h]
        push   edi
        push   esi
        mov    ecx, dword ptr [esi + 7ch]
        sub    cl, 8
        mov    edx, ecx
        sub    esp, ecx
        mov    edi, esp
        lea    esi, [eax + 8]
        rep movsb
        mov    ecx, edx
        mov    esi, esp
        lea    edi, [eax + 0ch]
        rep movsb
        add    esp, edx
        pop    esi
        pop    edi
        movzx  ecx, byte ptr [eax + 1]
        add    ecx, 8000005h
        mov    dword ptr [eax], ecx
        mov    edx, dword ptr [esi + 10h]
        mov    edx, dword ptr [edx + 8]
        mov    ecx, 80000h
        cmp    dword ptr [edx + 4], 3009h
        jne    SHORT loc_3_1464
        mov    ecx, 30090000h
loc_3_1464:
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 4], 0
        xor    al, al
        ret
loc_3_1471:
        call   sub_3_1101
        jb     SHORT loc_3_1481
        test   dl, 4
        je     NEAR PTR loc_3_1063
loc_3_1481:
        mov    edx, dword ptr [ebx + 10h]
        mov    edx, dword ptr [edx + 8]
        cmp    dword ptr [edx + 4], 3009h
        jne    SHORT loc_3_14A7
sub_3_1490:
        mov    word ptr [edi + 30h], 0beh
        mov    byte ptr [edi + 39h], 0f8h
        mov    word ptr [edi + 3ah], 0
        mov    byte ptr [edi + 0ah], 0ch
loc_3_14A4:
        xor    al, al
        ret
loc_3_14A7:
        test   byte ptr [esi + 482h], 40h
        je     SHORT loc_3_14A4
        call   sub_3_1101
        jb     SHORT loc_3_14A4
        test   dl, 4
        je     SHORT loc_3_14A4
        call   sub_3_1490
        mov    byte ptr [edi + 39h], 10h
        jmp    SHORT loc_3_14A4
loc_3_14C7:
        cmp    byte ptr [edi + 3], 1
        je     SHORT loc_3_14F0
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_3_14F0
        mov    eax, dword ptr [edi + 1ch]
        mov    cx, word ptr [eax + 0ch]
        cmp    cl, 21h
        je     SHORT loc_3_14E6
        cmp    cx, 230h
        jne    SHORT loc_3_14F0
loc_3_14E6:
        mov    word ptr [eax + 0ch], 64h
        mov    byte ptr [eax + 2], 5
loc_3_14F0:
        xor    al, al
        ret
loc_3_14F3:
        mov    byte ptr [edi + 31h], 4
        mov    eax, dword ptr [edi + 36h]
        shr    eax, 8
        mov    dword ptr [edi + 36h], eax
        mov    byte ptr [edi + 39h], 0f0h
        mov    word ptr [edi + 3ah], 0
        xor    al, al
        ret
        db  000h, 000h, 000h
loc_3_1510:
        xor    eax, eax
        cmp    word ptr [edi + 37h], ax
        jne    SHORT loc_3_151E
        mov    byte ptr [edi + 3], 1
        mov    al, 2
loc_3_151E:
        ret
loc_3_151F:
        mov    al, byte ptr [edi + 32h]
        and    al, 3fh
        cmp    al, 0eh
        jne    NEAR PTR loc_3_15C9
        test   byte ptr [edi + 32h], 40h
        jne    NEAR PTR loc_3_15F3
        call   sub_3_0588
        jb     NEAR PTR loc_3_15CC
        sub    esp, 10h
        mov    edx, esp
        mov    word ptr [edx + 4], 24h
        mov    word ptr [edx + 0ah], 9
        mov    word ptr [edx], 3
        mov    byte ptr [edx + 0ch], 8
        push   edx
        db  0FFh, 015h, 07Ch, 001h, 000h, 000h
        mov    eax, dword ptr [edx + 6]
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_3_15DD
        mov    dword ptr [ebx + 80h], eax
        mov    dword ptr [ebx + 7ch], 24h
        and    dword ptr [ebx + 6ch], 0fffffffdh
        or     dword ptr [ebx + 6ch], 200h
        mov    dword ptr [edi + 0ch], 248h
        mov    byte ptr [edi + 0ah], 6
        mov    word ptr [edi + 30h], 1ah
        xor    eax, eax
        mov    dword ptr [edi + 32h], eax
        mov    byte ptr [edi + 34h], 24h
        mov    byte ptr [ebx + 0b0h], al
        db  0C7h, 043h, 070h, 052h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        db  0C7h, 040h, 01Fh, 01Dh, 016h, 000h, 000h
        push   ebx
        push   edi
        push   esi
        call   sub_3_06C2
        pop    esi
        pop    edi
        pop    ebx
        mov    al, 3
        ret
loc_3_15C9:
        xor    al, al
        ret
loc_3_15CC:
        mov    word ptr [ebx + 6ah], 1dh
        mov    byte ptr [edi + 3], 4
        mov    byte ptr [edi + 4], 8
        mov    al, 2
        ret
loc_3_15DD:
        push   dword ptr [ebx + 90h]
        mov    edi, ebx
        mov    ebx, dword ptr [edi + 8]
        call   sub_3_05C3
        pop    ebx
        mov    edi, dword ptr [ebx + 20h]
        jmp    SHORT loc_3_15CC
loc_3_15F3:
        push   ebp
        push   edi
        push   esi
        db  08Dh, 035h, 0FCh, 00Dh, 000h, 000h
        lea    ecx, [1bh]
        mov    edi, dword ptr [ebx + 80h]
        mov    ebp, edi
        rep movsb
        pop    esi
        pop    edi
        mov    dword ptr [ebp + 14h], 0ff0fff0fh
        pop    ebp
        mov    byte ptr [edi + 3], 1
        mov    al, 2
        ret
loc_3_161D:
        mov    ecx, dword ptr [esi + 90h]
        mov    edx, dword ptr [ecx + 20h]
        movzx  eax, word ptr [edi + 3]
        mov    word ptr [edx + 3], ax
        push   edi
        push   esi
        push   ebx
        push   ecx
        mov    ebx, esi
        mov    esi, dword ptr [edi + 1ch]
        mov    edi, dword ptr [edx + 1ch]
        mov    eax, dword ptr [ecx + 80h]
        movzx  ecx, byte ptr [edx + 0bh]
        rep movsb
        mov    esi, dword ptr [ebx + 80h]
        mov    edi, eax
        mov    ecx, 0ch
        rep movsb
        add    esi, 8
        mov    ecx, 10h
        rep movsb
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        mov    byte ptr [eax], 1bh
        jmp    NEAR PTR loc_3_0BC1
loc_3_166B:
        cmp    byte ptr [edi + 39h], 80h
        je     NEAR PTR loc_3_0A25
        xor    eax, eax
        cmp    byte ptr [edi + 39h], al
        je     SHORT loc_3_16AA
        cmp    dword ptr [esi + 9dh], 20353334h
        je     NEAR PTR loc_3_0A25
        mov    dword ptr [edi + 30h], 26h
        mov    dword ptr [edi + 34h], eax
        mov    word ptr [edi + 38h], ax
        mov    ecx, dword ptr [ebx + 10h]
        movsx  ecx, word ptr [ecx + 10h]
        add    ecx, ebx
        db  0C7h, 041h, 01Fh, 0ABh, 016h, 000h, 000h
loc_3_16AA:
        ret
loc_3_16AB:
        mov    ecx, dword ptr [esi + 80h]
        mov    eax, dword ptr [ecx]
        mov    edx, 1010a00h
        or     eax, eax
        je     SHORT loc_3_16C1
        mov    edx, 2010a00h
loc_3_16C1:
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [esi + 10h]
        movsx  edx, word ptr [edx + 10h]
        add    edx, esi
        push   ecx
        test   byte ptr [edx + 2], 2
        je     SHORT loc_3_16E3
        mov    ecx, eax
        xchg   cl, ch
        ror    ecx, 10h
        xchg   cl, ch
        call   sub_3_07CB
        mov    eax, ecx
loc_3_16E3:
        pop    edx
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 4], 100h
        mov    byte ptr [edi + 3], 1
        xor    al, al
        ret
loc_3_16F5:
        xor    eax, eax
        cmp    byte ptr [edi + 34h], 2
        jne    SHORT loc_3_170B
        mov    byte ptr [edi + 30h], 0c0h
        mov    byte ptr [edi + 0ah], 0ah
        mov    dword ptr [edi + 32h], eax
        mov    dword ptr [edi + 36h], eax
loc_3_170B:
        ret
loc_3_170C:
        mov    ecx, dword ptr [esi + 80h]
        movzx  eax, byte ptr [ecx + 6]
        shl    ax, 4
        shr    al, 4
        aad    0ah
        mov    byte ptr [ecx + 6], al
        movzx  eax, byte ptr [ecx + 7]
        shl    ax, 4
        shr    al, 4
        aad    0ah
        mov    byte ptr [ecx + 7], al
        mov    al, 0
        ret
        db  000h, 000h, 000h
loc_3_1738:
        cmp    word ptr [ebx + 0eh], 3ch
        ja     SHORT loc_3_174B
        mov    word ptr [ebx + 0ch], 3ch
        mov    word ptr [ebx + 0eh], 3ch
loc_3_174B:
        mov    al, 4
        ret
_PTEXT ends
        end
