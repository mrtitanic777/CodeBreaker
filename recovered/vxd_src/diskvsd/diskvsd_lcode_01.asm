; diskvsd_lcode_01.asm -- DISKVSD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

DiskVSD_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
        db  000h
loc_1_000C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    ax, word ptr [ebx]
        cmp    ax, 3
        db  00Fh, 084h, 0DDh, 0FFh, 0FEh, 0FFh
        cmp    ax, 4
        db  00Fh, 084h, 054h, 001h, 0FFh, 0FFh
        cmp    ax, 2
        db  00Fh, 084h, 060h, 001h, 0FFh, 0FFh
        mov    word ptr [ebx + 2], 0ffffh
        ret
        db  000h, 000h
loc_1_0040:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 20h]
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_1_0069
        mov    word ptr [esi + 6ah], 0
loc_1_0056:
        mov    edi, dword ptr [esi + 14h]
        sub    edi, 8
        mov    dword ptr [esi + 14h], edi
        push   esi
        call   dword ptr [edi]
        add    esp, 4
loc_1_0065:
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_1_0069:
        call   sub_1_0298
        or     al, al
        je     SHORT loc_1_0056
        cmp    al, 2
        je     SHORT loc_1_0065
        add    dword ptr [esi + 14h], 8
        mov    ecx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [esi + 10h], ecx
        mov    eax, dword ptr [esi + 7ch]
        test   dword ptr [esi + 6ch], 200h
        jne    SHORT loc_1_0098
        mov    edx, dword ptr [esi + 4]
        mul    dword ptr [edx + 0dbh]
loc_1_0098:
        mov    dword ptr [edi + 10h], eax
        push   esi
        call   dword ptr [ecx]
        add    esp, 4
        jmp    SHORT loc_1_0065
loc_1_00A3:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 20h]
        mov    ebx, dword ptr [esi + 4]
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_1_011E
        mov    eax, dword ptr [ebx + 0d7h]
        xchg   ah, al
        ror    eax, 10h
        xchg   ah, al
        mov    dword ptr [ebx + 0d3h], eax
        xor    ecx, ecx
        mov    dword ptr [ebx + 0d7h], ecx
        mov    ecx, dword ptr [ebx + 0dbh]
        xor    cl, cl
        xchg   cl, ch
        ror    ecx, 10h
        xchg   cl, ch
        mov    dword ptr [ebx + 0dbh], ecx
        bsf    ecx, ecx
        mov    byte ptr [ebx + 3ch], cl
        mov    cx, 200h
        cmp    eax, 600h
        jbe    SHORT loc_1_0100
        cmp    eax, 0d20h
        ja     SHORT loc_1_011E
        mov    ch, 7
loc_1_0100:
        mov    eax, dword ptr [esi + 98h]
        mov    word ptr [eax], cx
        mov    word ptr [esi + 6ah], 0
        mov    dword ptr [esi + 0a0h], 0
        jmp    NEAR PTR loc_1_0056
loc_1_011E:
        mov    word ptr [esi + 6ah], 10h
        mov    dword ptr [esi + 0a0h], 5
        jmp    NEAR PTR loc_1_0056
loc_1_0133:
        mov    edx, dword ptr [esp + 4]
        db  0BEh, 042h, 001h, 000h, 000h
        db  0CDh, 020h, 00Eh, 080h, 001h, 000h
loc_1_0142:
        push   edi
        push   esi
        push   ebx
        mov    esi, edx
        mov    ebx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 80h]
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    edi, dword ptr [esi + 20h]
loc_1_015F:
        mov    eax, esi
        mov    esi, dword ptr [esi + 90h]
        push   word ptr [edi + 3]
        cmp    byte ptr [edi + 3], 1
        push   esi
        push   edi
        mov    ecx, dword ptr [esi + 20h]
        mov    esi, dword ptr [edi + 1ch]
        mov    edi, dword ptr [ecx + 1ch]
        movzx  ecx, byte ptr [ecx + 0bh]
        rep movsd
        pop    edi
        pop    esi
        call   sub_1_0AE4
        mov    edi, dword ptr [esi + 20h]
        pop    word ptr [edi + 3]
        cmp    byte ptr [edi + 3], 1
        jne    NEAR PTR loc_1_0069
        mov    word ptr [esi + 6ah], 0
        jmp    NEAR PTR loc_1_0056
loc_1_01A3:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 20h]
        mov    ebx, dword ptr [esi + 4]
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_1_0234
        test   byte ptr [ebx + 24h], 40h
        jne    SHORT loc_1_020F
        test   byte ptr [ebx + 17h], 10h
        jne    SHORT loc_1_01EF
        movzx  eax, word ptr [ebx + 55h]
        xchg   ah, al
        shr    eax, 3
        dec    eax
        mov    dword ptr [ebx + 0e3h], eax
        mov    dword ptr [ebx + 0dfh], 40h
        mov    dword ptr [ebx + 0e7h], 20h
        shl    eax, 0bh
        mov    dword ptr [ebx + 0d3h], eax
loc_1_01EF:
        xor    eax, eax
        mov    dword ptr [ebx + 54h], eax
        xchg   dword ptr [ebx + 58h], eax
loc_1_01F7:
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [ebx + 0dbh], eax
        bsf    eax, eax
        mov    byte ptr [ebx + 3ch], al
        jmp    NEAR PTR loc_1_0056
loc_1_020F:
        mov    eax, dword ptr [ebx + 0d7h]
        xchg   ah, al
        ror    eax, 10h
        xchg   ah, al
        mov    dword ptr [ebx + 0d3h], eax
        xor    eax, eax
        mov    dword ptr [ebx + 0d7h], eax
        mov    eax, dword ptr [ebx + 0dbh]
        xor    al, al
        jmp    SHORT loc_1_01F7
loc_1_0234:
        test   byte ptr [ebx + 24h], 40h
        jne    NEAR PTR loc_1_0069
        mov    dword ptr [ebx + 54h], 0
        mov    dword ptr [ebx + 58h], 0
        jmp    NEAR PTR loc_1_0069
loc_1_0251:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 20h]
        cmp    byte ptr [edi + 3], 1
        jne    NEAR PTR loc_1_0069
        mov    byte ptr [edi + 30h], 2fh
        mov    eax, dword ptr [esi + 14h]
        db  0C7h, 000h, 040h, 000h, 000h, 000h
        mov    dword ptr [edi + 0ch], 0
        mov    dword ptr [edi + 10h], 0
        mov    byte ptr [edi + 39h], 0
        mov    eax, dword ptr [esi + 10h]
        add    eax, 14h
        mov    dword ptr [esi + 10h], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        pop    ebx
        pop    esi
        pop    edi
        ret
        db  000h
sub_1_0298:
        mov    ebx, dword ptr [esi + 14h]
        mov    ebx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 10h], ebx
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [edi + 3], 80h
        jne    SHORT loc_1_02EB
        cmp    byte ptr [edi + 3], 4
        jne    SHORT loc_1_02BC
        cmp    byte ptr [edi + 4], 8
        je     SHORT loc_1_02D9
        cmp    byte ptr [edi + 4], 18h
        je     SHORT loc_1_02D9
loc_1_02BC:
        cmp    byte ptr [edi + 4ch], 5
        jb     SHORT loc_1_02D9
        mov    word ptr [esi + 6ah], 1ah
loc_1_02C8:
        xor    al, al
        ret
loc_1_02CB:
        cmp    byte ptr [edi + 4ch], 5
        jb     SHORT loc_1_02D9
        mov    al, byte ptr [ecx + 2]
        jmp    NEAR PTR loc_1_0409
loc_1_02D9:
        inc    byte ptr [edi + 4ch]
        mov    byte ptr [edi + 3], 0
        mov    ax, word ptr [esi + 0eh]
        mov    word ptr [esi + 0ch], ax
        mov    al, 1
        ret
loc_1_02EB:
        mov    ecx, dword ptr [edi + 1ch]
        mov    al, byte ptr [ecx + 2]
        and    eax, 0fh
        cmp    al, 0
        je     SHORT loc_1_02CB
        cmp    al, 5
        jne    SHORT loc_1_0336
        cmp    byte ptr [edi + 30h], 1bh
        jne    SHORT loc_1_0318
        test   byte ptr [edi + 34h], 2
        je     SHORT loc_1_030F
        and    byte ptr [edi + 34h], 0fdh
        mov    al, 1
        ret
loc_1_030F:
        and    byte ptr [ebx + 23h], 7fh
        jmp    NEAR PTR loc_1_0409
loc_1_0318:
        cmp    byte ptr [edi + 30h], 2eh
        jne    NEAR PTR loc_1_0409
        mov    byte ptr [edi + 30h], 2ah
        mov    edx, dword ptr [esi + 14h]
        db  0C7h, 002h, 051h, 002h, 000h, 000h
        and    word ptr [ebx + 7ah], 0fffeh
        jmp    SHORT loc_1_02CB
loc_1_0336:
        cmp    al, 1
        jne    SHORT loc_1_0343
        mov    word ptr [esi + 6ah], 2
        xor    al, al
        ret
loc_1_0343:
        cmp    al, 3
        je     SHORT loc_1_02CB
        cmp    al, 4
        je     SHORT loc_1_02CB
        cmp    al, 6
        jne    SHORT loc_1_0359
        test   byte ptr [ebx + 20h], 4
        je     NEAR PTR loc_1_02CB
loc_1_0359:
        cmp    al, 0bh
        je     NEAR PTR loc_1_02CB
        cmp    al, 2
        jne    NEAR PTR loc_1_0409
        cmp    byte ptr [edi + 30h], 1bh
        je     NEAR PTR loc_1_0409
        cmp    byte ptr [ecx + 0ch], 4
        jne    NEAR PTR loc_1_0409
        cmp    byte ptr [ecx + 0dh], 3
        je     NEAR PTR loc_1_0409
        cmp    byte ptr [ecx + 0dh], 4
        je     SHORT loc_1_0409
        bts    word ptr [ebx + 7ah], 1
        jb     SHORT loc_1_0409
        mov    edx, edi
        db  0BEh, 0A5h, 003h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    al, 2
        ret
loc_1_03A5:
        mov    edi, edx
        mov    esi, dword ptr [edi + 40h]
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [ebx + 24h], 20h
        jne    SHORT loc_1_03D0
loc_1_03B3:
        mov    ax, 7
        xor    ecx, ecx
        call   sub_1_0936
loc_1_03BE:
        and    byte ptr [ebx + 7ah], 0fdh
        or     eax, eax
        jne    SHORT loc_1_03F5
        push   esi
        call   sub_1_0440
        add    esp, 4
        ret
loc_1_03D0:
        cmp    word ptr [esi + 68h], 1
        jne    SHORT loc_1_03B3
        sub    esp, 34h
        mov    ecx, esp
        add    ecx, 3
        and    ecx, 0fffffffch
        mov    edx, 2ah
        mov    ax, 2ah
        call   sub_1_0936
        add    esp, 34h
        jmp    SHORT loc_1_03BE
loc_1_03F5:
        mov    word ptr [esi + 6ah], ax
        mov    eax, dword ptr [esi + 14h]
        sub    eax, 8
        mov    dword ptr [esi + 14h], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        ret
loc_1_0409:
        mov    ebx, dword ptr [edi + 1ch]
        cmp    byte ptr [ebx + 0ch], 30h
        je     SHORT loc_1_0422
        db  00Fh, 0B6h, 080h, 02Dh, 004h, 000h, 000h
        mov    word ptr [esi + 6ah], ax
        jmp    NEAR PTR loc_1_02C8
loc_1_0422:
        mov    word ptr [esi + 6ah], 1ch
        jmp    NEAR PTR loc_1_02C8
        db  000h, 000h, 020h, 014h, 01Bh, 01Ah, 019h, 01Fh, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah
        db  000h, 000h, 000h
sub_1_0440:
        mov    ebx, dword ptr [esp + 4]
        mov    esi, dword ptr [ebx + 4]
        test   byte ptr [esi + 24h], 60h
        jne    SHORT loc_1_04AA
loc_1_044D:
        cmp    word ptr [ebx + 68h], 0fh
        je     SHORT loc_1_049A
        call   sub_1_0A4B
        mov    eax, dword ptr [ebx + 7ch]
        test   dword ptr [ebx + 6ch], 200h
        jne    SHORT loc_1_046B
        mul    dword ptr [esi + 0dbh]
loc_1_046B:
        mov    dword ptr [edi + 10h], eax
        mov    edx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 000h, 040h, 000h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        add    dword ptr [ebx + 14h], 8
        movzx  eax, word ptr [ebx + 68h]
        cmp    word ptr [ebx + 68h], 0ch
        ja     SHORT loc_1_04B8
        db  0FFh, 014h, 085h, 05Ah, 005h, 000h, 000h
loc_1_0493:
        or     dword ptr [ebx + 6ch], 8000h
loc_1_049A:
        mov    eax, dword ptr [ebx + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebx + 10h], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        ret
loc_1_04AA:
        mov    word ptr [ebx + 0ch], 32h
        mov    word ptr [ebx + 0eh], 32h
        jmp    SHORT loc_1_044D
loc_1_04B8:
        cmp    eax, 0dh
        jne    SHORT loc_1_0515
        test   byte ptr [esi + 24h], 40h
        je     SHORT loc_1_0515
        cmp    word ptr [ebx + 92h], 4411h
        je     SHORT loc_1_04D9
        cmp    word ptr [ebx + 92h], 440dh
        jne    SHORT loc_1_0515
loc_1_04D9:
        cmp    dword ptr [ebx + 94h], 4868h
        je     SHORT loc_1_04F1
        cmp    dword ptr [ebx + 94h], 868h
        jne    SHORT loc_1_0515
loc_1_04F1:
        cmp    word ptr [ebx + 92h], 4411h
        je     SHORT loc_1_0503
        call   sub_1_05A4
        jmp    SHORT loc_1_0493
loc_1_0503:
        mov    dword ptr [ebx + 0a0h], 0
        mov    word ptr [ebx + 6ah], 0
        jmp    SHORT loc_1_0594
loc_1_0515:
        cmp    eax, 2ch
        jne    SHORT loc_1_0524
        call   sub_1_0A22
        jmp    NEAR PTR loc_1_0493
loc_1_0524:
        cmp    eax, 0eh
        je     NEAR PTR loc_1_05B4
        cmp    eax, 2ah
        jne    SHORT loc_1_053C
        call   sub_1_09F8
        jmp    NEAR PTR loc_1_0493
loc_1_053C:
        cmp    eax, 26h
        jne    SHORT loc_1_054B
        call   sub_1_07D3
        jmp    NEAR PTR loc_1_0493
loc_1_054B:
        sub    dword ptr [ebx + 14h], 8
        jmp    NEAR PTR loc_1_049A
loc_1_0554:
        call   sub_1_058E
        ret
sub_1_055A:
        db  069h, 006h, 000h, 000h, 0A0h, 006h
        add    byte ptr [eax], al
        and    al, 7
        add    byte ptr [eax], al
        db  08Eh, 005h, 000h, 000h, 0D7h, 006h
        add    byte ptr [eax], al
        bound  eax, qword ptr [edi]
        add    byte ptr [eax], al
        bound  eax, qword ptr [edi]
        add    byte ptr [eax], al
        xlatb
        pop    es
        add    byte ptr [eax], al
        fild   dword ptr [edi]
        add    byte ptr [eax], al
        stosb
        pop    es
        add    byte ptr [eax], al
        scasb
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [edi], 0
        db  000h, 040h, 008h
        add    byte ptr [eax], al
sub_1_058E:
        mov    word ptr [ebx + 6ah], 16h
loc_1_0594:
        mov    eax, dword ptr [ebx + 14h]
        sub    eax, 10h
        mov    dword ptr [ebx + 14h], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        ret
sub_1_05A4:
        call   sub_1_0840
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 040h, 0F8h, 0A3h, 000h, 000h, 000h
        ret
loc_1_05B4:
        db  0BEh, 0C1h, 005h, 000h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 00Eh, 080h, 001h, 000h
loc_1_05C1:
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 4]
        call   sub_1_08CC
        mov    dword ptr [edi + 30h], 1704h
        xor    eax, eax
        mov    dword ptr [edi + 34h], eax
        mov    dword ptr [edi + 38h], eax
        mov    dword ptr [edi + 0ch], 280h
        mov    byte ptr [edi + 0ah], 0ah
        push   1
        push   0ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0653
        mov    dword ptr [ebx + 80h], eax
        mov    dword ptr [edi + 18h], eax
        mov    dword ptr [edi + 10h], 0ch
        mov    dword ptr [ebx + 7ch], 0ch
        mov    edx, dword ptr [ebx + 90h]
        movzx  ecx, word ptr [edx + 5eh]
        mov    byte ptr [edi + 32h], cl
        mov    ecx, 8000000h
        mov    ch, byte ptr [edx + 5ch]
        or     ch, 0b0h
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esi + 0d3h]
        xchg   cl, ch
        ror    ecx, 10h
        xchg   cl, ch
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esi + 0dbh]
        xchg   cl, ch
        ror    ecx, 10h
        xchg   cl, ch
        mov    dword ptr [eax + 8], ecx
        db  0C7h, 043h, 070h, 033h, 001h, 000h, 000h
        jmp    NEAR PTR loc_1_0AA7
loc_1_0653:
        push   edi
        push   esi
        push   ebx
        mov    byte ptr [edi + 3], 4
        mov    byte ptr [edi + 4], 0
        mov    byte ptr [edi + 4ch], 5
        xchg   ebx, esi
        jmp    NEAR PTR loc_1_015F
loc_1_0669:
        mov    dword ptr [edi + 0ch], 240h
        mov    byte ptr [edi + 0ah], 0ah
        mov    byte ptr [edi + 30h], 28h
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    eax, dword ptr [ebx + 74h]
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
        mov    dword ptr [edi + 32h], eax
        mov    byte ptr [edi + 36h], 0
        mov    eax, dword ptr [ebx + 7ch]
        xchg   al, ah
        mov    word ptr [edi + 37h], ax
        mov    byte ptr [edi + 39h], 0
        ret
loc_1_06A0:
        mov    dword ptr [edi + 0ch], 280h
        mov    byte ptr [edi + 0ah], 0ah
        mov    byte ptr [edi + 30h], 2ah
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    eax, dword ptr [ebx + 74h]
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
        mov    dword ptr [edi + 32h], eax
        mov    byte ptr [edi + 36h], 0
        mov    eax, dword ptr [ebx + 7ch]
        xchg   al, ah
        mov    word ptr [edi + 37h], ax
        mov    byte ptr [edi + 39h], 0
        ret
loc_1_06D7:
        mov    dword ptr [edi + 0ch], 280h
        mov    byte ptr [edi + 0ah], 0ah
        mov    byte ptr [edi + 30h], 2eh
        mov    al, byte ptr [esi + 7dh]
        test   word ptr [esi + 7ah], 1
        jne    SHORT loc_1_06FF
        mov    byte ptr [edi + 30h], 2ah
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 040h, 0F8h, 051h, 002h, 000h, 000h
loc_1_06FF:
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    eax, dword ptr [ebx + 74h]
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
        mov    dword ptr [edi + 32h], eax
        mov    byte ptr [edi + 36h], 0
        mov    eax, dword ptr [ebx + 7ch]
        xchg   al, ah
        mov    word ptr [edi + 37h], ax
        mov    byte ptr [edi + 39h], 0
        ret
loc_1_0724:
        mov    dword ptr [edi + 10h], 0
        mov    dword ptr [edi + 0ch], 0
        mov    byte ptr [edi + 0ah], 0ah
        mov    byte ptr [edi + 30h], 2fh
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    eax, dword ptr [ebx + 74h]
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
        mov    dword ptr [edi + 32h], eax
        mov    byte ptr [edi + 36h], 0
        mov    eax, dword ptr [ebx + 7ch]
        xchg   al, ah
        mov    word ptr [edi + 37h], ax
        mov    byte ptr [edi + 39h], 0
        ret
loc_1_0762:
        mov    dword ptr [edi + 0ch], 0
        mov    byte ptr [edi + 0ah], 6
        mov    byte ptr [edi + 30h], 0
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    dword ptr [edi + 32h], 0
        ret
loc_1_0782:
        mov    dword ptr [edi + 0ch], 40h
        mov    byte ptr [edi + 0ah], 6
        mov    byte ptr [edi + 30h], 3
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    eax, dword ptr [ebx + 7ch]
        mov    byte ptr [edi + 34h], al
        xor    eax, eax
        mov    word ptr [edi + 32h], ax
        mov    byte ptr [edi + 35h], al
        ret
loc_1_07AA:
        mov    al, 1
        jmp    SHORT loc_1_07B0
loc_1_07AE:
        xor    al, al
loc_1_07B0:
        mov    dword ptr [edi + 0ch], 0
        mov    byte ptr [edi + 0ah], 6
        mov    byte ptr [edi + 30h], 1eh
        mov    dword ptr [edi + 32h], 0
        mov    byte ptr [edi + 34h], al
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        ret
sub_1_07D3:
        mov    al, 1
        jmp    SHORT loc_1_080A
loc_1_07D7:
        mov    al, 3
        jmp    SHORT loc_1_080A
loc_1_07DB:
        cmp    dword ptr [esi + 8ah], 75517953h
        jne    SHORT loc_1_0808
        push   ebx
        mov    ebx, esi
        xor    ecx, ecx
        mov    ax, 6
        call   sub_1_0936
        pop    ebx
        cmp    ax, 20h
        jne    SHORT loc_1_0808
        add    esp, 4
        jmp    NEAR PTR loc_1_0554
        db  032h, 0C0h, 0EBh, 002h
loc_1_0808:
        mov    al, 2
loc_1_080A:
        mov    dword ptr [edi + 10h], 0
        mov    dword ptr [edi + 0ch], 0
        mov    byte ptr [edi + 0ah], 6
        mov    byte ptr [edi + 30h], 1bh
        mov    dword ptr [edi + 32h], 0
        mov    byte ptr [edi + 34h], al
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    word ptr [ebx + 0ch], 3ch
        mov    word ptr [ebx + 0eh], 3ch
        ret
sub_1_0840:
        mov    dword ptr [ebx + 80h], esi
        test   byte ptr [esi + 24h], 40h
        jne    SHORT loc_1_08A4
        add    dword ptr [ebx + 80h], 54h
        mov    byte ptr [edi + 30h], 25h
        mov    dword ptr [edi + 32h], 0
        mov    dword ptr [edi + 36h], 0
        mov    eax, 8
loc_1_086A:
        mov    byte ptr [edi + 0ah], 0ah
        mov    dword ptr [ebx + 7ch], eax
        mov    dword ptr [edi + 10h], eax
        or     dword ptr [ebx + 6ch], 200h
        and    byte ptr [ebx + 6ch], 0fdh
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 040h, 0F8h, 0A3h, 001h, 000h, 000h
        push   ebx
        db  0FFh, 015h, 014h, 00Bh, 000h, 000h
        add    esp, 4
        mov    dword ptr [edi + 0ch], 40h
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        ret
loc_1_08A4:
        add    dword ptr [ebx + 80h], 0d3h
        mov    byte ptr [edi + 30h], 23h
        mov    dword ptr [edi + 32h], 0
        mov    dword ptr [edi + 36h], 0c0000h
        mov    eax, 0ch
        jmp    SHORT loc_1_086A
        db  0C6h, 047h, 002h, 002h, 0C3h
sub_1_08CC:
        push   ebx
        push   dword ptr [ebx + 10h]
        mov    ecx, dword ptr [esi + 4]
        add    ecx, 64h
        sub    esp, 10h
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    cx, 68h
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0eh], 0
        mov    word ptr [edi], 2
        push   edi
        db  0FFh, 015h, 000h, 00Bh, 000h, 000h
        mov    edi, dword ptr [edi + 0ah]
        add    esp, 14h
        pop    eax
        or     edi, edi
        stc
        je     SHORT loc_1_0935
        add    edi, dword ptr [esi + 4]
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 0a4h], edi
        mov    dword ptr [edi + 6ch], 8409h
        mov    word ptr [edi + 68h], 12h
        mov    dword ptr [edi + 8], esi
        mov    ebx, edi
        call   sub_1_0A4B
        pop    dword ptr [ebx + 90h]
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        clc
loc_1_0935:
        ret
sub_1_0936:
        pushad
        push   ecx
        mov    ecx, dword ptr [ebx + 4]
        add    ecx, 64h
        sub    esp, 10h
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    cx, 68h
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0eh], 0
        mov    word ptr [edi], 2
        push   edi
        db  0FFh, 015h, 000h, 00Bh, 000h, 000h
        mov    edi, dword ptr [edi + 0ah]
        add    esp, 14h
        or     edi, edi
        je     NEAR PTR loc_1_09EF
        add    edi, dword ptr [ebx + 4]
        mov    word ptr [edi + 68h], ax
        xor    eax, eax
        mov    dword ptr [edi + 0a8h], eax
        mov    dword ptr [edi + 7ch], eax
        mov    dword ptr [edi + 6ch], 8f09h
        mov    al, byte ptr [ebx + 15h]
        mov    byte ptr [edi + 0b1h], al
        pop    ecx
        or     ecx, ecx
        je     SHORT loc_1_09C1
        mov    dword ptr [edi + 0ach], edi
        add    dword ptr [edi + 0ach], 0bch
        mov    dword ptr [edi + 80h], ecx
        mov    dword ptr [edi + 7ch], edx
        mov    dword ptr [edi + 8], ebx
        push   edi
        db  0FFh, 015h, 014h, 00Bh, 000h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_09C1
        or     dword ptr [edi + 6ch], 40h
loc_1_09C1:
        mov    esi, edi
        xor    edx, edx
        db  0FFh, 015h, 00Ch, 00Bh, 000h, 000h
        movzx  eax, word ptr [edi + 6ah]
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        db  0FFh, 015h, 000h, 00Bh, 000h, 000h
        add    esp, 0ch
loc_1_09E9:
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
loc_1_09EF:
        mov    ax, 1dh
        add    esp, 4
        jmp    SHORT loc_1_09E9
sub_1_09F8:
        mov    dword ptr [edi + 0ch], 40h
        mov    byte ptr [edi + 0ah], 6
        mov    byte ptr [edi + 30h], 1ah
        mov    dword ptr [edi + 32h], 0
        mov    eax, dword ptr [edi + 10h]
        mov    byte ptr [edi + 34h], al
        mov    byte ptr [edi + 32h], 2eh
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        ret
sub_1_0A22:
        mov    dword ptr [edi + 0ch], 40h
        mov    byte ptr [edi + 0ah], 0ch
        mov    dword ptr [edi + 30h], 12h
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [edi + 34h], eax
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        mov    byte ptr [edi + 31h], al
        mov    dword ptr [edi + 38h], 0
        ret
sub_1_0A4B:
        mov    eax, dword ptr [ebx + 10h]
        movsx  edi, word ptr [eax + 10h]
        add    edi, ebx
        mov    dword ptr [edi], 40h
        mov    dword ptr [edi + 40h], ebx
        mov    dword ptr [ebx + 20h], edi
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 3], 0
        xor    eax, eax
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [edi + 44h], eax
        mov    dword ptr [edi + 20h], eax
        mov    byte ptr [edi + 4ch], al
        mov    byte ptr [edi + 8], al
        mov    byte ptr [edi + 9], al
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
        ret
loc_1_0AA7:
        push   ebx
        db  0FFh, 015h, 014h, 00Bh, 000h, 000h
        add    esp, 4
        or     al, al
        je     SHORT loc_1_0AB9
        or     dword ptr [ebx + 6ch], 40h
loc_1_0AB9:
        mov    byte ptr [edi + 3], 0
        mov    dword ptr [edi + 44h], 0
        mov    cx, word ptr [ebx + 0eh]
        mov    word ptr [ebx + 0ch], cx
        xor    edx, edx
        xchg   esi, edi
        xchg   edi, ebx
        mov    dword ptr [edi + 0a4h], edi
        db  0BAh, 040h, 004h, 000h, 000h
        db  0FFh, 015h, 00Ch, 00Bh, 000h, 000h
        ret
sub_1_0AE4:
        sub    eax, dword ptr [ebx + 4]
        sub    esp, 8
        mov    ecx, esp
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [ecx], 4
        push   ecx
        db  0FFh, 015h, 000h, 00Bh, 000h, 000h
        add    esp, 0ch
        ret
        db  000h
sub_1_0B00:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0B0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0B14:
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
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 044h, 069h, 073h, 06Bh
        db  056h, 053h, 044h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
_LTEXT ends
        end
