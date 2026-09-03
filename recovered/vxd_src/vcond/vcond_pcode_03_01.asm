; vcond_pcode_03_01.asm -- VCOND object 3 segment _PTEXT  (part 1/2)
.386p
_PTEXT segment dword public use32 'PCODE'

VCOND_V86_API:
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 202h
        jne    SHORT loc_3_0014
        call   sub_3_1078
        jmp    NEAR PTR loc_3_0116
loc_3_0014:
        cmp    ax, 205h
        jne    SHORT loc_3_0024
        call   sub_3_1030
        jmp    NEAR PTR loc_3_0116
loc_3_0024:
        cmp    ax, 206h
        jne    SHORT loc_3_0034
        call   sub_3_1051
        jmp    NEAR PTR loc_3_0116
loc_3_0034:
        cmp    ax, 204h
        jne    SHORT loc_3_0044
        call   sub_3_1026
        jmp    NEAR PTR loc_3_0116
loc_3_0044:
        cmp    ax, 207h
        jne    SHORT loc_3_0054
        call   sub_3_108B
        jmp    NEAR PTR loc_3_0116
loc_3_0054:
        cmp    ax, 208h
        jne    SHORT loc_3_0064
        call   sub_3_1092
        jmp    NEAR PTR loc_3_0116
loc_3_0064:
        cmp    ax, 209h
        jne    SHORT loc_3_0074
        call   sub_3_0F72
        jmp    NEAR PTR loc_3_0116
loc_3_0074:
        cmp    ax, 20ah
        jne    SHORT loc_3_0084
        call   sub_3_109F
        jmp    NEAR PTR loc_3_0116
loc_3_0084:
        cmp    ax, 20bh
        jne    SHORT loc_3_0094
        call   sub_3_10AA
        jmp    NEAR PTR loc_3_0116
loc_3_0094:
        cmp    ax, 203h
        jne    SHORT loc_3_00A1
        call   sub_3_10B4
        jmp    SHORT loc_3_0116
loc_3_00A1:
        cmp    ax, 20dh
        jne    SHORT loc_3_00AE
        call   sub_3_10D1
        jmp    SHORT loc_3_0116
loc_3_00AE:
        cmp    ax, 20eh
        jne    SHORT loc_3_00BB
        call   sub_3_10D8
        jmp    SHORT loc_3_0116
loc_3_00BB:
        cmp    ax, 20fh
        jne    SHORT loc_3_00C8
        call   sub_3_10FD
        jmp    SHORT loc_3_0116
loc_3_00C8:
        cmp    ax, 210h
        jne    SHORT loc_3_00D5
        call   sub_3_10F3
        jmp    SHORT loc_3_0116
loc_3_00D5:
        cmp    ax, 401h
        jne    SHORT loc_3_00E2
        call   sub_3_11A3
        jmp    SHORT loc_3_0117
loc_3_00E2:
        cmp    ax, 402h
        jne    SHORT loc_3_00EF
        call   sub_3_1206
        jmp    SHORT loc_3_0117
loc_3_00EF:
        cmp    ax, 403h
        jne    SHORT loc_3_00FC
        call   sub_3_1214
        jmp    SHORT loc_3_0117
loc_3_00FC:
        cmp    ax, 404h
        jne    SHORT loc_3_0109
        call   sub_3_123C
        jmp    SHORT loc_3_0117
loc_3_0109:
        cmp    ax, 405h
        jne    SHORT loc_3_0116
        call   sub_3_11E7
        jmp    SHORT loc_3_0117
loc_3_0116:
        ret
loc_3_0117:
        mov    word ptr [ebp + 1ch], ax
        mov    cx, word ptr [ebp + 2ch]
        shr    ecx, 1
        shl    eax, 1
        shl    ecx, 1
        mov    word ptr [ebp + 2ch], cx
        ret
VCOND_PM_API:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_3_019E
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 304h
        jne    SHORT loc_3_0143
        call   sub_3_0EC6
        jmp    SHORT loc_3_019E
loc_3_0143:
        cmp    ax, 305h
        jne    SHORT loc_3_0150
        call   sub_3_0EDE
        jmp    SHORT loc_3_019E
loc_3_0150:
        cmp    ax, 306h
        jne    SHORT loc_3_015D
        call   sub_3_0EEB
        jmp    SHORT loc_3_019E
loc_3_015D:
        cmp    ax, 307h
        jne    SHORT loc_3_016A
        call   sub_3_0F0D
        jmp    SHORT loc_3_019E
loc_3_016A:
        cmp    ax, 308h
        jne    SHORT loc_3_0177
        call   sub_3_0F1C
        jmp    SHORT loc_3_019E
loc_3_0177:
        cmp    ax, 301h
        jne    SHORT loc_3_0184
        call   sub_3_0EA4
        jmp    SHORT loc_3_019E
loc_3_0184:
        cmp    ax, 302h
        jne    SHORT loc_3_0191
        call   sub_3_0EAE
        jmp    SHORT loc_3_019E
loc_3_0191:
        cmp    ax, 303h
        jne    SHORT loc_3_019E
        call   sub_3_0EBD
        jmp    SHORT loc_3_019E
loc_3_019E:
        ret
VCOND_Launch_ConApp_Inherited:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        push   esi
        push   edi
        push   ebx
        call   sub_3_173E
        mov    ebx, 0
        or     eax, eax
        jne    SHORT loc_3_01BB
        mov    ebx, 0ffffffffh
loc_3_01BB:
        mov    eax, dword ptr [ebp + 2ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 18h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 14h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 4]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 28h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 24h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 3ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 38h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 40h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 44h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ah, bl
        mov    al, 98h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, 3
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    edx, dword ptr [eax + 2]
        mov    cx, word ptr [ebp + 34h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 30h]
        mov    dword ptr [eax + 2], ecx
        ror    edx, 10h
        mov    eax, edx
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        shr    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  08Bh, 00Dh, 070h, 000h, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 049h, 000h, 001h, 000h
        sub    word ptr [ebp + 30h], 40h
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cld
        mov    edi, eax
        db  08Bh, 035h, 0EAh, 001h, 000h, 000h
        xor    ecx, ecx
loc_3_02AE:
        lodsb
        stosb
        inc    ecx
        or     al, al
        je     SHORT loc_3_02B7
        jmp    SHORT loc_3_02AE
loc_3_02B7:
        mov    ax, word ptr [ebp + 30h]
        mov    word ptr [ebp + 14h], ax
        mov    ax, word ptr [ebp + 34h]
        mov    word ptr [ebp + 3ch], ax
        db  0A1h, 0B0h, 001h, 000h, 000h
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        shr    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        ret
loc_3_02DF:
        add    word ptr [ebp + 30h], 40h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    bx, ax
        shl    ebx, 10h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    bx, ax
        ror    ebx, 10h
        add    word ptr [ebp + 30h], 4
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 44h], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 40h], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 38h], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 3ch], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 24h], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 28h], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 8], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 4], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 14h], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 18h], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 10h], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    dx, word ptr [ebp + 2ch]
        and    dx, 1
        and    ax, 0fffeh
        or     ax, dx
        mov    word ptr [ebp + 2ch], ax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [eax + 2], ebx
        ret
loc_3_03AA:
        pushad
        push   ebx
        call   sub_3_4408
        popad
        db  0FFh, 025h, 0E2h, 001h, 000h, 000h
loc_3_03B8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_07DC
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_3_03CC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_081D
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
loc_3_03E0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0842
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_03F4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0883
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    18h
loc_3_0408:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_085B
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    18h
loc_3_041C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_08AB
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    1ch
loc_3_0430:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_08D6
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    18h
loc_3_0444:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_08FE
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    18h
loc_3_0458:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0926
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    1ch
loc_3_046C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0951
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    18h
loc_3_0480:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_09A1
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_0494:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_09C0
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_04A8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_09DF
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_04BC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0A01
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_04D0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0A23
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_04E4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0A45
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_04F8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0A64
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    18h
loc_3_050C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0A8C
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    18h
loc_3_0520:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0AB1
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
loc_3_0534:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0AD6
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_0548:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0AFC
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_055C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0B21
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_0570:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0B34
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_0584:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0B53
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
loc_3_0598:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0BC4
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_05AC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0979
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    18h
loc_3_05C0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C05
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_05D4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C24
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_05E8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C46
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_3_05FC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0BE3
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_0610:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C4E
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_0624:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C70
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_3_0638:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C8A
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    20h
loc_3_064C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0CB8
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
loc_3_0660:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_07E2
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    18h
loc_3_0674:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0CDD
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
loc_3_0688:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0D17
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_069C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0D44
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    14h
loc_3_06B0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0D72
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_06C4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0D7D
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_06D8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0D88
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_06EC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0DB0
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_0700:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0DD8
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_0714:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0E00
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_0728:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0E1F
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_073C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0E3E
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_0750:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0F25
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    1ch
loc_3_0764:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0F3E
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_0778:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0F58
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_078C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0F4B
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_07A0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0E5D
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    10h
loc_3_07B4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0F65
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_3_07C8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_3_0C7C
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
sub_3_07DC:
        mov    eax, 100h
        ret
sub_3_07E2:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_080A
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_18C6
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_3_080A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax], 0
        xor    eax, eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_081D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_083D
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2E2C
loc_3_083D:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0842:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_3_169F
        movsx  eax, ax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_085B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_087E
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_29FE
loc_3_087E:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0883:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_08A6
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_29A6
loc_3_08A6:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_08AB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_08D1
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2A59
loc_3_08D1:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_08D6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_08F9
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2ACA
loc_3_08F9:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_08FE:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0921
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2B1F
loc_3_0921:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0926:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_094C
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2B7A
loc_3_094C:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0951:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0974
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2CD5
loc_3_0974:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0979:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_099C
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2C19
loc_3_099C:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_09A1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_09BB
        push   eax
        call   sub_3_2D5C
loc_3_09BB:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_09C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_09DA
        push   eax
        call   sub_3_2D98
loc_3_09DA:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_09DF:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_09FC
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2DE8
loc_3_09FC:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0A01:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0A1E
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2E00
loc_3_0A1E:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0A23:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0A40
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_304B
loc_3_0A40:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0A45:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0A5F
        push   eax
        call   sub_3_3084
loc_3_0A5F:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0A64:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0A87
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_3091
loc_3_0A87:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0A8C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0AAC
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_322D
loc_3_0AAC:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0AB1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0AD1
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_34D6
loc_3_0AD1:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0AD6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, 1
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0AF7
        push   edx
        push   edx
        push   eax
        call   sub_3_1A4B
loc_3_0AF7:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0AFC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        xor    edx, edx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0B1C
        push   dword ptr [ebp + 0ch]
        push   edx
        push   eax
        call   sub_3_1A4B
loc_3_0B1C:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0B21:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_3_1838
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0B34:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0B4E
        push   eax
        call   sub_3_35A1
loc_3_0B4E:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0B53:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        cmp    dword ptr [ebp + 0ch], 502h
        jne    SHORT loc_3_0B7A
        cmp    dword ptr [ebp + 10h], 2
        jne    SHORT loc_3_0B72
        push   dword ptr [ebp + 8]
        call   sub_3_23BB
        jmp    SHORT loc_3_0B7A
loc_3_0B72:
        push   dword ptr [ebp + 8]
        call   sub_3_2394
loc_3_0B7A:
        xor    eax, eax
        mov    ax, word ptr [ebp + 10h]
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        xor    edx, edx
        db  0BEh, 025h, 010h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0B98:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_3_0B53
        add    esp, 0ch
        leave
        ret    0ch
sub_3_0BB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        pop    edi
        pop    ebx
        leave
        ret    4
sub_3_0BC4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0BDE
        push   eax
        call   sub_3_2EF7
loc_3_0BDE:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0BE3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0C00
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2F10
loc_3_0C00:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0C05:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0C1F
        push   eax
        call   sub_3_38B0
loc_3_0C1F:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0C24:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0C41
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2FF6
loc_3_0C41:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0C46:
        db  00Fh, 0B7h, 005h, 0B4h, 001h, 000h, 000h
        ret
sub_3_0C4E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0C6B
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_2F2D
loc_3_0C6B:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0C70:
        push   esi
        push   edi
        push   ebx
        call   sub_3_1A6C
        pop    ebx
        pop    edi
        pop    esi
        ret
sub_3_0C7C:
        push   esi
        push   edi
        push   ebx
        push   0
        call   sub_3_1AEB
        pop    ebx
        pop    edi
        pop    esi
        ret
sub_3_0C8A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0CB3
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_39A4
loc_3_0CB3:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0CB8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0CD8
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_1B8A
loc_3_0CD8:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0CDD:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 0ch]
        db  0BFh, 0B6h, 001h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, 8
        jbe    SHORT loc_3_0CF9
        mov    ecx, 8
        int3
loc_3_0CF9:
        push   ds
        pop    es
        cld
        rep movsd
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  089h, 03Dh, 0D6h, 001h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        db  0A3h, 0EAh, 001h, 000h, 000h
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0D17:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0D3F
        push   edx
        push   ecx
        push   0
        push   1
        push   eax
        call   sub_3_3E30
loc_3_0D3F:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0D44:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0D6D
        push   edx
        push   ecx
        push   dword ptr [ebp + 10h]
        push   2
        push   eax
        call   sub_3_3E30
loc_3_0D6D:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0D72:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0D7D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0D88:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0DAB
        push   0
        push   dword ptr [ebp + 0ch]
        push   0
        push   4
        push   eax
        call   sub_3_3E30
loc_3_0DAB:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0DB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0DD3
        push   0
        push   dword ptr [ebp + 0ch]
        push   0
        push   8
        push   eax
        call   sub_3_3E30
loc_3_0DD3:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0DD8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0DFB
        push   0
        push   dword ptr [ebp + 0ch]
        push   0
        push   10h
        push   eax
        call   sub_3_3E30
loc_3_0DFB:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0E00:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0E1A
        push   eax
        call   sub_3_4306
loc_3_0E1A:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0E1F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0E39
        push   eax
        call   sub_3_438F
loc_3_0E39:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0E3E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0E58
        push   eax
        call   sub_3_45C0
loc_3_0E58:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0E5D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0F0h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0E77
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_3_1DE5
loc_3_0E77:
        leave
        ret
sub_3_0E79:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        db  0B8h, 0B6h, 001h, 000h, 000h
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ebx*4]
        db  0FFh, 035h, 0D6h, 001h, 000h, 000h
        push   dword ptr [ebp + 0ch]
        push   eax
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret    8
sub_3_0EA4:
        call   sub_3_1E87
        mov    word ptr [ebp + 1ch], ax
        ret
sub_3_0EAE:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 10h]
        call   sub_3_1EB0
        ret
sub_3_0EBD:
        push   dword ptr [ebp + 18h]
        call   sub_3_38BF
        ret
sub_3_0EC6:
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        push   dword ptr [ebp + 14h]
        call   sub_3_3E64
        mov    word ptr [ebp + 1ch], ax
        ret
sub_3_0EDE:
        push   dword ptr [ebp + 14h]
        call   sub_3_3E90
        mov    word ptr [ebp + 1ch], ax
        ret
sub_3_0EEB:
        mov    ebx, dword ptr [ebp + 10h]
        call   sub_3_1124
        mov    edx, dword ptr [ebp]
        shr    dx, 3
        rol    edx, 10h
        shr    dx, 3
        push   eax
        push   dword ptr [ebp + 18h]
        push   edx
        push   ebx
        call   sub_3_38CC
        ret
sub_3_0F0D:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        call   sub_3_4438
        ret
sub_3_0F1C:
        push   dword ptr [ebp + 14h]
        call   sub_3_4562
        ret
sub_3_0F25:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_3_4693
        leave
        ret
sub_3_0F3E:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_3_47F9
        leave
        ret
sub_3_0F4B:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_3_4831
        leave
        ret
sub_3_0F58:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_3_485A
        leave
        ret
sub_3_0F65:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_3_4792
        leave
        ret
sub_3_0F72:
        db  0A1h, 0DEh, 001h, 000h, 000h
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        db  08Bh, 035h, 0DAh, 001h, 000h, 000h
        db  083h, 00Dh, 0DAh, 001h, 000h, 000h, 001h
        db  0A1h, 0DEh, 001h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        test   esi, 1
        jne    SHORT loc_3_0FBA
        mov    eax, 0a0006h
        db  0BEh, 0AAh, 003h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_3_0FBA
        db  089h, 035h, 0E2h, 001h, 000h, 000h
loc_3_0FBA:
        push   ebx
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_3_1BE5
        pop    ebx
        test   word ptr [ebp + 18h], 1
        je     SHORT loc_3_0FD9
        push   ebx
        call   sub_3_1B57
        or     eax, eax
        je     SHORT loc_3_101D
        jmp    SHORT loc_3_1016
loc_3_0FD9:
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        push   dword ptr [ebp + 18h]
        push   ebx
        call   sub_3_1969
        or     eax, eax
        je     SHORT loc_3_101D
        mov    eax, 200000h
        mov    ax, word ptr [ebp + 10h]
        mov    ecx, 500h
        xor    edx, edx
        db  0BEh, 025h, 010h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        push   ebx
        call   sub_3_1D96
        mov    word ptr [ebp + 18h], ax
loc_3_1016:
        mov    word ptr [ebp + 1ch], 1
loc_3_101C:
        ret
loc_3_101D:
        mov    word ptr [ebp + 1ch], 0
        jmp    SHORT loc_3_101C
loc_3_1025:
        ret
sub_3_1026:
        push   dword ptr [ebp + 18h]
        push   ebx
        call   sub_3_25C5
        ret
sub_3_1030:
        mov    eax, 1000h
        mov    ecx, 2
        mov    edx, dword ptr [ebp + 18h]
        db  0BEh, 049h, 010h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        ret
loc_3_1049:
        push   edx
        push   ebx
        call   sub_3_266C
        ret
sub_3_1051:
        mov    eax, 1000h
        mov    ecx, 2
        mov    edx, dword ptr [ebp + 18h]
        db  0BEh, 06Ah, 010h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        ret
loc_3_106A:
        mov    eax, edx
        shr    edx, 10h
        push   eax
        push   edx
        push   ebx
        call   sub_3_26ED
        ret
sub_3_1078:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        push   ebp
        push   ebx
        call   sub_3_23D8
        ret
sub_3_108B:
        push   ebx
        call   sub_3_25AF
        ret
sub_3_1092:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 18h]
        push   ebx
        call   sub_3_2745
        ret
sub_3_109F:
        mov    ax, word ptr [ebp + 18h]
        db  066h, 0A3h, 0B4h, 001h, 000h, 000h
        ret
sub_3_10AA:
        push   dword ptr [ebp + 18h]
        push   ebx
        call   sub_3_2F57
        ret
sub_3_10B4:
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 18h]
        call   sub_3_3B4F
        mov    dword ptr [ebp + 1ch], eax
        ret
sub_3_10D1:
        push   ebx
        call   sub_3_2779
        ret
sub_3_10D8:
        movzx  ecx, word ptr [ebp + 18h]
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   ecx
        push   eax
        push   ebx
        call   sub_3_27D5
        mov    word ptr [ebp + 1ch], ax
        ret
sub_3_10F3:
        push   ebx
        call   sub_3_285C
        mov    dword ptr [ebp + 1ch], eax
        ret
sub_3_10FD:
        push   esi
        mov    esi, ebp
        enter  4, 0
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [esi + 14h]
        db  0CDh, 020h, 065h, 000h, 040h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 18h], ecx
        mov    dword ptr [esi + 1ch], eax
        leave
        pop    esi
        ret
sub_3_111D:
        db  0CDh, 020h, 001h, 000h, 00Bh, 000h
        ret
sub_3_1124:
        push   ebx
        mov    ebx, dword ptr [ebx + 4]
        mov    eax, 0ffh
        test   byte ptr [ebx + 496h], 8
        jne    SHORT loc_3_113A
        and    ax, 0fffeh
loc_3_113A:
        test   word ptr [ebx + 417h], 200h
        jne    SHORT loc_3_1149
        and    ax, 0fffdh
loc_3_1149:
        test   byte ptr [ebx + 496h], 4
        jne    SHORT loc_3_1156
        and    ax, 0fffbh
loc_3_1156:
        test   word ptr [ebx + 417h], 100h
        jne    SHORT loc_3_1165
        and    ax, 0fff7h
loc_3_1165:
        test   word ptr [ebx + 417h], 3
        jne    SHORT loc_3_1174
        and    ax, 0ffefh
loc_3_1174:
        test   word ptr [ebx + 417h], 20h
        jne    SHORT loc_3_1183
        and    ax, 0ffdfh
loc_3_1183:
        test   word ptr [ebx + 417h], 10h
        jne    SHORT loc_3_1192
        and    ax, 0ffbfh
loc_3_1192:
        test   word ptr [ebx + 417h], 40h
        jne    SHORT loc_3_11A1
        and    ax, 0ff7fh
loc_3_11A1:
        pop    ebx
        ret
sub_3_11A3:
        call   sub_3_4C24
        sub    esp, 4
        mov    eax, esp
        push   eax
        mov    ax, 3c18h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        mov    ax, 3c00h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_3_488D
        pop    ecx
        mov    dword ptr [ebp + 10h], ecx
        ret
sub_3_11E7:
        mov    ax, 3c00h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_3_492A
        ret
sub_3_1206:
        movzx  edx, word ptr [ebp + 14h]
        push   edx
        push   dword ptr [ebp + 10h]
        call   sub_3_4BE6
        ret
sub_3_1214:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        db  0E8h, 023h, 0F0h, 0FCh, 0FFh
        ret
sub_3_1220:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 06Bh, 000h, 040h, 000h
        add    esp, 8
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret    8
sub_3_123C:
        push   esi
        mov    esi, ebp
        enter  4, 0
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [esi + 18h]
        push   dword ptr [esi + 14h]
        db  0CDh, 020h, 063h, 000h, 040h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 18h], ecx
        mov    dword ptr [esi + 1ch], eax
        leave
        pop    esi
        ret
        db  000h
sub_3_1260:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        add    eax, 1ch
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1278:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_1285
        xor    eax, eax
        jmp    SHORT loc_3_1302
loc_3_1285:
        mov    dword ptr [ebp - 0ch], 1
        mov    ecx, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_1299
        xor    eax, eax
loc_3_1299:
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [ebp - 8]
        mov    dword ptr [esi + 18h], edi
        test   edi, edi
        jne    SHORT loc_3_12B5
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_12AE:
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_3_1302
loc_3_12B5:
        mov    ebx, dword ptr [ebp + 0ch]
        push   0
        inc    ebx
        imul   ebx, dword ptr [ebp + 8]
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_12C5:
        add    esp, 8
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        jne    SHORT loc_3_12EA
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_12E3:
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_3_1302
loc_3_12EA:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 0ch], eax
        add    eax, ebx
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 14h], eax
        mov    dword ptr [esi], edx
        mov    eax, esi
loc_3_1302:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_3_130B:
        sub    esp, 0ch
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 18h]
        push   edi
        push   ebp
        mov    edi, dword ptr [esi + 0ch]
        mov    eax, dword ptr [esi + 10h]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 18h], ecx
        mov    ecx, dword ptr [esi + 14h]
        cmp    edi, eax
        jbe    SHORT loc_3_1336
        sub    edi, eax
        mov    ecx, edi
        xor    edi, edi
        jmp    SHORT loc_3_133E
loc_3_1336:
        sub    ecx, dword ptr [esp + 10h]
        sub    edi, dword ptr [esp + 18h]
loc_3_133E:
        mov    ebp, dword ptr [esi + 4]
        lea    eax, [ecx + edi]
        sub    edx, edx
        div    ebp
        dec    eax
        mov    edx, dword ptr [esp + 24h]
        cmp    eax, edx
        jb     SHORT loc_3_1353
        mov    eax, edx
loc_3_1353:
        mov    ebx, eax
        mov    dword ptr [esp + 14h], eax
        imul   ebx, ebp
        cmp    ebx, ecx
        mov    ebp, ebx
        jb     SHORT loc_3_1364
        mov    ebp, ecx
loc_3_1364:
        test   ebp, ebp
        je     SHORT loc_3_137F
        push   1
        push   ebp
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 1ch]
        call   sub_3_1E10
        mov    dword ptr [esi + 10h], eax
        add    dword ptr [esp + 28h], ebp
loc_3_137F:
        sub    ebx, ebp
        cmp    ebx, edi
        jb     SHORT loc_3_1387
        mov    ebx, edi
loc_3_1387:
        test   ebx, ebx
        je     SHORT loc_3_139E
        push   1
        push   ebx
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 24h]
        call   sub_3_1E10
        mov    dword ptr [esi + 10h], eax
loc_3_139E:
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [esi + 10h], eax
        jne    SHORT loc_3_13AC
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 10h], eax
loc_3_13AC:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    0ch
sub_3_13BA:
        sub    esp, 0ch
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 18h]
        push   edi
        push   ebp
        mov    eax, dword ptr [esi + 0ch]
        mov    ebx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 18h], ecx
        mov    ecx, dword ptr [esi + 14h]
        cmp    ebx, eax
        jne    SHORT loc_3_13E4
        xor    eax, eax
        jmp    NEAR PTR loc_3_146D
loc_3_13E4:
        cmp    ebx, dword ptr [esp + 10h]
        jbe    SHORT loc_3_13F4
        sub    ebx, dword ptr [esp + 10h]
        mov    ecx, ebx
        xor    ebx, ebx
        jmp    SHORT loc_3_13FC
loc_3_13F4:
        sub    ecx, dword ptr [esp + 10h]
        sub    ebx, dword ptr [esp + 18h]
loc_3_13FC:
        mov    ebp, dword ptr [esi + 4]
        lea    eax, [ecx + ebx]
        sub    edx, edx
        div    ebp
        mov    edx, dword ptr [esp + 24h]
        cmp    eax, edx
        jb     SHORT loc_3_1410
        mov    eax, edx
loc_3_1410:
        mov    edi, eax
        mov    dword ptr [esp + 14h], eax
        imul   edi, ebp
        cmp    edi, ecx
        mov    ebp, edi
        jb     SHORT loc_3_1421
        mov    ebp, ecx
loc_3_1421:
        test   ebp, ebp
        je     SHORT loc_3_143C
        push   0
        push   ebp
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 34h]
        call   sub_3_1E10
        mov    dword ptr [esi + 0ch], eax
        add    dword ptr [esp + 28h], ebp
loc_3_143C:
        sub    edi, ebp
        cmp    edi, ebx
        jb     SHORT loc_3_1444
        mov    edi, ebx
loc_3_1444:
        test   edi, edi
        je     SHORT loc_3_145B
        push   0
        push   edi
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 34h]
        call   sub_3_1E10
        mov    dword ptr [esi + 0ch], eax
loc_3_145B:
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [esi + 0ch], eax
        jne    SHORT loc_3_1469
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 0ch], eax
loc_3_1469:
        mov    eax, dword ptr [esp + 14h]
loc_3_146D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    0ch
sub_3_1477:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 10h], ecx
        jne    SHORT loc_3_1487
        xor    eax, eax
        jmp    SHORT loc_3_149B
loc_3_1487:
        push   0
        push   dword ptr [eax + 4]
        push   ecx
        push   dword ptr [esp + 14h]
        call   sub_3_1E10
        mov    eax, 1
loc_3_149B:
        ret    8
sub_3_149E:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 10h]
        cmp    dword ptr [eax + 0ch], edx
        jne    SHORT loc_3_14AE
        xor    eax, eax
        jmp    SHORT loc_3_14CE
loc_3_14AE:
        lea    ecx, [eax + 4]
        cmp    dword ptr [eax + 8], edx
        jne    SHORT loc_3_14B9
        mov    edx, dword ptr [eax + 14h]
loc_3_14B9:
        sub    edx, dword ptr [ecx]
        push   0
        push   dword ptr [ecx]
        push   edx
        push   dword ptr [esp + 14h]
        call   sub_3_1E10
        mov    eax, 1
loc_3_14CE:
        ret    8
sub_3_14D1:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 10h], ecx
        jne    SHORT loc_3_14E1
        xor    eax, eax
        jmp    SHORT loc_3_14F5
loc_3_14E1:
        push   0
        push   dword ptr [eax + 4]
        push   dword ptr [esp + 10h]
        push   ecx
        call   sub_3_1E10
        mov    eax, 1
loc_3_14F5:
        ret    8
sub_3_14F8:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 10h]
        cmp    dword ptr [ecx + 0ch], edx
        jne    SHORT loc_3_1508
        xor    eax, eax
        jmp    SHORT loc_3_1528
loc_3_1508:
        lea    eax, [ecx + 4]
        cmp    dword ptr [ecx + 8], edx
        jne    SHORT loc_3_1513
        mov    edx, dword ptr [ecx + 14h]
loc_3_1513:
        sub    edx, dword ptr [eax]
        push   0
        push   dword ptr [eax]
        push   dword ptr [esp + 10h]
        push   edx
        call   sub_3_1E10
        mov    eax, 1
loc_3_1528:
        ret    8
sub_3_152B:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ecx + 10h], eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
sub_3_153B:
        mov    edx, dword ptr [esp + 4]
        push   esi
        mov    eax, dword ptr [edx + 0ch]
        mov    esi, dword ptr [edx + 10h]
        cmp    eax, esi
        jne    SHORT loc_3_154E
        xor    eax, eax
        jmp    SHORT loc_3_1572
loc_3_154E:
        mov    ecx, dword ptr [esp + 0ch]
        test   ecx, ecx
        je     SHORT loc_3_1572
        add    ecx, dword ptr [edx + 4]
        cmp    dword ptr [edx + 14h], ecx
        jne    SHORT loc_3_1561
        mov    ecx, dword ptr [edx + 8]
loc_3_1561:
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, ecx
loc_3_1572:
        pop    esi
        ret    8
sub_3_1576:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        push   0
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1599:
        add    esp, 8
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_15A4:
        add    esp, 8
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_3_15AE:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 0ch]
        mov    ecx, dword ptr [edi + 10h]
        mov    edx, dword ptr [edi + 8]
        mov    esi, dword ptr [edi + 14h]
        cmp    ecx, eax
        jne    SHORT loc_3_15C8
        xor    eax, eax
        jmp    SHORT loc_3_15DE
loc_3_15C8:
        jbe    SHORT loc_3_15D2
        sub    ecx, eax
        mov    esi, ecx
        xor    ecx, ecx
        jmp    SHORT loc_3_15D6
loc_3_15D2:
        sub    esi, eax
        sub    ecx, edx
loc_3_15D6:
        lea    eax, [esi + ecx]
        sub    edx, edx
        div    dword ptr [edi + 4]
loc_3_15DE:
        pop    edi
        pop    esi
        ret    4
sub_3_15E3:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 10h]
        cmp    dword ptr [ecx + 0ch], eax
        je     SHORT loc_3_1604
        cmp    dword ptr [ecx + 8], eax
        jne    SHORT loc_3_15F7
        mov    eax, dword ptr [ecx + 14h]
loc_3_15F7:
        sub    eax, dword ptr [ecx + 4]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, 1
        jmp    SHORT loc_3_1606
loc_3_1604:
        xor    eax, eax
loc_3_1606:
        ret    4
        db  000h, 000h, 000h
sub_3_160C:
        push   ebp
        mov    eax, 0
        mov    ebp, esp
        mov    ecx, 40h
        sub    esp, 14h
        push   edi
        db  0BFh, 000h, 000h, 000h, 000h
        rep stosd
        mov    dword ptr [ebp - 14h], eax
        mov    ecx, dword ptr [ebp - 14h]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_1634
        xor    eax, eax
loc_3_1634:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_3_1647
        xor    eax, eax
        jmp    SHORT loc_3_169A
loc_3_1647:
        mov    dword ptr [ebp - 10h], 0
        mov    ecx, dword ptr [ebp - 10h]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_165B
        xor    eax, eax
loc_3_165B:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_3_166E
        xor    eax, eax
        jmp    SHORT loc_3_169A
loc_3_166E:
        push   0
        push   8
        push   4
        call   sub_3_1260
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_3_169A
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        xor    eax, eax
loc_3_169A:
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_169F:
        mov    dx, 40h
        push   esi
        push   edi
        db  0B8h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 0ch]
loc_3_16AE:
        mov    ecx, dword ptr [eax]
        test   ecx, ecx
        je     SHORT loc_3_16BE
        cmp    dword ptr [ecx], esi
        jne    SHORT loc_3_16BE
        mov    dword ptr [eax], 0
loc_3_16BE:
        add    eax, 4
        dec    dx
        jne    SHORT loc_3_16AE
        xor    edi, edi
        xor    eax, eax
loc_3_16C9:
        movsx  ecx, di
        db  039h, 004h, 08Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_3_16E6
        inc    di
        cmp    di, 40h
        jl     SHORT loc_3_16C9
loc_3_16DD:
        mov    ax, 0ffffh
loc_3_16E1:
        pop    edi
        pop    esi
        ret    8
loc_3_16E6:
        push   0
        push   60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_16EF:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_16DD
        xor    edx, edx
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 10h], edx
        mov    dword ptr [eax + 14h], edx
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 8], edx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 0ch], edx
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [eax + 20h], ecx
        mov    dword ptr [eax + 28h], esi
        mov    dword ptr [eax + 5ch], edx
        mov    dword ptr [eax + 1ch], 12h
        mov    word ptr [eax + 18h], 50h
        mov    dword ptr [eax + 58h], edx
        movsx  edx, di
        mov    word ptr [eax + 1ah], 19h
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        mov    ax, di
        jmp    SHORT loc_3_16E1
sub_3_173E:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 18h]
        call   sub_3_193C
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_3_175B
        xor    eax, eax
        jmp    NEAR PTR loc_3_182E
loc_3_175B:
        mov    esi, dword ptr [esp + 20h]
        push   0
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1774:
        add    esp, 8
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_3_1784
        xor    eax, eax
        jmp    NEAR PTR loc_3_182E
loc_3_1784:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebp
        rep movsd
        mov    ecx, eax
        push   0
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [esp + 20h]
        mov    ecx, 0ffffffffh
        mov    edi, esi
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_17BE:
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_17D8
        push   0
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_17D1:
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_3_182E
loc_3_17D8:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [esp + 10h]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 10h]
        test   eax, eax
        je     SHORT loc_3_180D
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_180A:
        add    esp, 8
loc_3_180D:
        mov    eax, dword ptr [ebx + 14h]
        test   eax, eax
        je     SHORT loc_3_181F
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_181C:
        add    esp, 8
loc_3_181F:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 10h], ebp
        mov    dword ptr [ebx + 14h], eax
        mov    eax, 1
loc_3_182E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
sub_3_1838:
        movsx  eax, word ptr [esp + 4]
        push   esi
        db  08Dh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [eax]
        test   esi, esi
        jne    SHORT loc_3_184F
        xor    eax, eax
        jmp    SHORT loc_3_18C2
loc_3_184F:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_3_1862
        push   eax
        call   sub_3_303F
loc_3_1862:
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     SHORT loc_3_186F
        push   eax
        call   sub_3_20A2
loc_3_186F:
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        je     SHORT loc_3_187C
        push   eax
        call   sub_3_3F60
loc_3_187C:
        mov    eax, dword ptr [esi + 14h]
        test   eax, eax
        je     SHORT loc_3_188E
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_188B:
        add    esp, 8
loc_3_188E:
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_3_18A0
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_189D:
        add    esp, 8
loc_3_18A0:
        mov    eax, dword ptr [esi + 5ch]
        test   eax, eax
        je     SHORT loc_3_18B2
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_18AF:
        add    esp, 8
loc_3_18B2:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_18BA:
        add    esp, 8
        mov    eax, 1
loc_3_18C2:
        pop    esi
        ret    4
sub_3_18C6:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [edx + 14h]
        test   edi, edi
        je     SHORT loc_3_1934
        mov    ebx, dword ptr [edx + 10h]
        test   ebx, ebx
        je     SHORT loc_3_1934
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [esp + 14h]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    ebx, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [esp + 18h]
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        lea    esi, [edx + 3ch]
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 6
        rep movsd
        mov    eax, 1
        jmp    SHORT loc_3_1936
loc_3_1934:
        xor    eax, eax
loc_3_1936:
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
sub_3_193C:
        mov    eax, dword ptr [esp + 4]
        push   esi
        test   eax, eax
        je     SHORT loc_3_195F
        xor    edx, edx
        db  0B9h, 000h, 000h, 000h, 000h
loc_3_194C:
        mov    esi, dword ptr [ecx]
        test   esi, esi
        je     SHORT loc_3_1956
        cmp    dword ptr [esi], eax
        je     SHORT loc_3_1965
loc_3_1956:
        inc    edx
        add    ecx, 4
        cmp    edx, 40h
        jl     SHORT loc_3_194C
loc_3_195F:
        xor    eax, eax
loc_3_1961:
        pop    esi
        ret    4
loc_3_1965:
        mov    eax, dword ptr [ecx]
        jmp    SHORT loc_3_1961
sub_3_1969:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_193C
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_3_1988
        xor    eax, eax
        jmp    NEAR PTR loc_3_1A42
loc_3_1988:
        cmp    dword ptr [ebx + 4], 0
        jne    SHORT loc_3_19A2
        push   esi
        call   sub_3_2024
        mov    dword ptr [ebx + 4], eax
        test   eax, eax
        jne    SHORT loc_3_19A2
        xor    eax, eax
        jmp    NEAR PTR loc_3_1A42
loc_3_19A2:
        cmp    dword ptr [ebx + 8], 0
        jne    SHORT loc_3_19BB
        call   sub_3_3010
        mov    dword ptr [ebx + 8], eax
        test   eax, eax
        jne    SHORT loc_3_19BB
        xor    eax, eax
        jmp    NEAR PTR loc_3_1A42
loc_3_19BB:
        cmp    dword ptr [ebx + 0ch], 0
        jne    SHORT loc_3_19D1
        call   sub_3_3EAD
        mov    dword ptr [ebx + 0ch], eax
        test   eax, eax
        jne    SHORT loc_3_19D1
        xor    eax, eax
        jmp    SHORT loc_3_1A42
loc_3_19D1:
        mov    dword ptr [ebx + 54h], 0
        mov    word ptr [ebx + 3ah], 0
        mov    esi, dword ptr [ebp + 10h]
        test   esi, esi
        je     SHORT loc_3_1A3D
        lea    edx, [ebx + 3ch]
        mov    ecx, 6
        mov    edi, edx
        rep movsd
        mov    ax, word ptr [edx]
        cmp    ax, 2
        je     SHORT loc_3_1A20
        cmp    ax, 1
        je     SHORT loc_3_1A20
        mov    ax, word ptr [edx + 8]
        cmp    ax, 2
        je     SHORT loc_3_1A20
        cmp    ax, 1
        je     SHORT loc_3_1A20
        mov    ax, word ptr [edx + 10h]
        cmp    ax, 2
        je     SHORT loc_3_1A20
        cmp    ax, 1
        jne    SHORT loc_3_1A3D
loc_3_1A20:
        mov    dword ptr [ebp - 8], 0
        mov    ecx, dword ptr [ebp - 8]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_1A34
        xor    eax, eax
loc_3_1A34:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 54h], eax
loc_3_1A3D:
        mov    eax, 1
loc_3_1A42:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_3_1A4B:
        mov    eax, 10ch
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 0ch]
        cmp    dword ptr [esp + 0ch], 1
        adc    eax, -1
        push   eax
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        ret    0ch
sub_3_1A6C:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 18h
        mov    dword ptr [ebp - 14h], 1
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        lea    eax, [ebp - 4]
        push   eax
        push   1
        push   edx
        call   sub_3_13BA
        test   eax, eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [eax + 18h]
        je     SHORT loc_3_1AD9
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_3_1AE5
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 28h]
        jmp    SHORT loc_3_1AE7
loc_3_1AD9:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_1AE5:
        xor    eax, eax
loc_3_1AE7:
        mov    esp, ebp
        pop    ebp
        ret
sub_3_1AEB:
        push   ebp
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_3_1B1F
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        test   eax, eax
        je     SHORT loc_3_1B1F
        or     byte ptr [eax + 19h], 1
loc_3_1B1F:
        lea    eax, [ebp + 8]
        push   eax
        push   1
        push   edx
        call   sub_3_130B
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 18h]
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_1B57:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_3_193C
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_3_1B79
        push   0
        push   0
        push   esi
        call   sub_3_1969
        test   eax, eax
        jne    SHORT loc_3_1B79
        xor    edi, edi
loc_3_1B79:
        push   edi
        call   sub_3_1AEB
        cmp    edi, 1
        pop    edi
        sbb    eax, eax
        pop    esi
        inc    eax
        ret    4
sub_3_1B8A:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [ecx + 34h]
        cmp    word ptr [ebx], 0
        mov    esi, ebx
        je     SHORT loc_3_1BA4
loc_3_1B9D:
        inc    esi
        cmp    word ptr [esi], 0
        jne    SHORT loc_3_1B9D
loc_3_1BA4:
        sub    esi, ebx
        mov    edi, dword ptr [ecx + 58h]
        add    esi, 2
        lea    eax, [esi + edi]
        cmp    dword ptr [esp + 1ch], eax
        jb     SHORT loc_3_1BDB
        test   edi, edi
        je     SHORT loc_3_1BCB
        mov    ebp, dword ptr [esp + 18h]
        push   1
        push   edi
        push   dword ptr [ecx + 5ch]
        push   ebp
        call   sub_3_1E10
        jmp    SHORT loc_3_1BCF
loc_3_1BCB:
        mov    ebp, dword ptr [esp + 18h]
loc_3_1BCF:
        push   1
        add    ebp, edi
        push   esi
        push   ebx
        push   ebp
        call   sub_3_1E10
loc_3_1BDB:
        lea    eax, [esi + edi]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
sub_3_1BE5:
        push   ebp
        mov    ebp, esp
        sub    esp, 184h
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        mov    dword ptr [ebp - 10h], 1
        je     NEAR PTR loc_3_1D59
        movzx  eax, word ptr [ebp + 0ch]
        shl    eax, 4
        mov    ecx, dword ptr [ebp - 8]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ecx + 34h], eax
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_3_1C2E
        mov    dword ptr [ebp - 10h], 0
loc_3_1C2E:
        pop    ebx
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_3_1D59
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 58h], 0
        je     SHORT loc_3_1C65
        mov    eax, dword ptr [ecx + 5ch]
        test   eax, eax
        je     SHORT loc_3_1C65
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1C51:
        add    esp, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 58h], 0
        mov    dword ptr [ecx + 5ch], 0
loc_3_1C65:
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 18h]
        movsd
        movsd
        xor    esi, esi
        lea    ebx, [ebp - 80h]
loc_3_1C74:
        lea    eax, [ebp - 184h]
        push   eax
        push   esi
        call   sub_3_1220
        test   eax, eax
        jne    SHORT loc_3_1CAA
        lea    edi, [ebp - 184h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, ecx
        mov    dword ptr [ebx], ecx
        test   eax, eax
        je     SHORT loc_3_1CB0
        add    eax, 8
        mov    ecx, dword ptr [ebp - 8]
        add    dword ptr [ecx + 58h], eax
        jmp    SHORT loc_3_1CB0
loc_3_1CAA:
        mov    dword ptr [ebx], 0
loc_3_1CB0:
        add    ebx, 4
        inc    esi
        lea    eax, [ebp - 18h]
        cmp    ebx, eax
        jb     SHORT loc_3_1C74
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx + 58h]
        test   eax, eax
        je     NEAR PTR loc_3_1D59
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1CD1:
        add    esp, 8
        mov    ebx, eax
        mov    ecx, dword ptr [ebp - 8]
        test   ebx, ebx
        mov    dword ptr [ecx + 5ch], ebx
        jne    SHORT loc_3_1CE9
        mov    dword ptr [ecx + 58h], 0
        jmp    SHORT loc_3_1D59
loc_3_1CE9:
        mov    dword ptr [ebp - 4], 0
loc_3_1CF0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + eax*4 - 80h]
        mov    dword ptr [ebp - 0ch], ecx
        test   ecx, ecx
        je     SHORT loc_3_1D50
        lea    edi, [ebp - 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebx
        rep movsd
        mov    ecx, eax
        add    ebx, 7
        and    ecx, 3
        push   ebx
        rep movsb
        push   dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        add    al, 41h
        mov    byte ptr [ebx - 3], al
        mov    byte ptr [ebx - 6], al
        call   sub_3_1220
        test   eax, eax
        je     SHORT loc_3_1D4C
        push   0
        sub    ebx, 7
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_1F1E
        jmp    SHORT loc_3_1D50
loc_3_1D4C:
        add    ebx, dword ptr [ebp - 0ch]
        inc    ebx
loc_3_1D50:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 1ah
        jl     SHORT loc_3_1CF0
loc_3_1D59:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_1D62:
        db  0BAh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
loc_3_1D6B:
        mov    ecx, dword ptr [edx]
        test   ecx, ecx
        je     SHORT loc_3_1D75
        cmp    dword ptr [ecx], eax
        je     SHORT loc_3_1D82
loc_3_1D75:
        add    edx, 4
        db  081h, 0FAh, 000h, 001h, 000h, 000h
        jb     SHORT loc_3_1D6B
        jmp    SHORT loc_3_1D93
loc_3_1D82:
        push   dword ptr [ecx + 28h]
        push   5
        mov    dword ptr [ecx + 20h], 0
        call   sub_3_0E79
loc_3_1D93:
        ret    4
sub_3_1D96:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        cmp    dword ptr [eax + 54h], 0
        mov    edx, eax
        je     SHORT loc_3_1DDB
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [edx + 54h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [edx + 54h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    dword ptr [edx + 54h], 0
loc_3_1DDB:
        mov    ax, word ptr [edx + 3ah]
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_1DE5:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3ah], cx
        mov    eax, dword ptr [eax + 54h]
        test   eax, eax
        je     SHORT loc_3_1E09
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_1E09:
        mov    esp, ebp
        pop    ebp
        ret    8
        db  000h
sub_3_1E10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        cld
        rep movsb
        mov    dword ptr [ebp - 4], esi
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_3_1E30
        mov    dword ptr [ebp - 4], edi
loc_3_1E30:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_3_1E3B:
        push   esi
        mov    ecx, 0ffffffffh
        mov    esi, dword ptr [esp + 8]
        push   edi
        push   0
        mov    edi, esi
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1E56:
        add    esp, 8
        mov    edx, eax
        test   edx, edx
        je     SHORT loc_3_1E80
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, edx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_1E80:
        mov    eax, edx
        pop    edi
        pop    esi
        ret    4
sub_3_1E87:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_1EAD
        push   1
        push   300h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1E9C:
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_3_1EAF
loc_3_1EAD:
        xor    eax, eax
loc_3_1EAF:
        ret
sub_3_1EB0:
        mov    cl, byte ptr [esp + 4]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        and    cl, 7fh
        test   edx, edx
        je     SHORT loc_3_1EDE
        mov    ax, word ptr [esp + 8]
        cmp    ax, 5
        ja     SHORT loc_3_1EDE
        movzx  ecx, cl
        movzx  eax, ax
        imul   ecx, ecx, 6
        add    ecx, eax
        mov    al, byte ptr [esp + 0ch]
        mov    byte ptr [ecx + edx], al
loc_3_1EDE:
        ret    0ch
sub_3_1EE1:
        mov    ax, word ptr [esp + 4]
        push   esi
        and    ax, 7fh
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        jne    SHORT loc_3_1EFA
        xor    ax, ax
        jmp    SHORT loc_3_1F1A
loc_3_1EFA:
        mov    cx, word ptr [esp + 0ch]
        cmp    cx, 5
        jbe    SHORT loc_3_1F0A
        xor    ax, ax
        jmp    SHORT loc_3_1F1A
loc_3_1F0A:
        movzx  edx, ax
        movzx  eax, cx
        imul   edx, edx, 6
        add    edx, eax
        movzx  ax, byte ptr [edx + esi]
loc_3_1F1A:
        pop    esi
        ret    8
sub_3_1F1E:
        ret    8
loc_3_1F21:
        ret    4
sub_3_1F24:
        push   esi
        xor    al, al
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    dl, byte ptr [esp + 8]
loc_3_1F31:
        movzx  ecx, al
        imul   ecx, ecx, 6
        add    ecx, esi
        cmp    byte ptr [ecx], dl
        je     SHORT loc_3_1F5E
        cmp    byte ptr [ecx + 1], dl
        je     SHORT loc_3_1F5E
        cmp    byte ptr [ecx + 2], dl
        je     SHORT loc_3_1F5E
        cmp    byte ptr [ecx + 3], dl
        je     SHORT loc_3_1F5E
        cmp    byte ptr [ecx + 4], dl
        je     SHORT loc_3_1F5E
        cmp    byte ptr [ecx + 5], dl
        je     SHORT loc_3_1F5E
        inc    al
        cmp    al, 80h
        jb     SHORT loc_3_1F31
        xor    al, al
loc_3_1F5E:
        pop    esi
        ret    4
sub_3_1F62:
        mov    edx, dword ptr [esp + 4]
        push   esi
        push   edi
        xor    esi, esi
        mov    al, byte ptr [edx]
        test   al, al
        jne    SHORT loc_3_1F77
        mov    eax, 2
        jmp    SHORT loc_3_1F90
loc_3_1F77:
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    edx, ecx
        add    esi, ecx
        cmp    byte ptr [edx], 0
        jne    SHORT loc_3_1F77
        lea    eax, [esi + 1]
loc_3_1F90:
        pop    edi
        pop    esi
        ret    4
sub_3_1F95:
        push   esi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, esi
        dec    esi
        test   eax, eax
        je     SHORT loc_3_1FBC
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
loc_3_1FA9:
        mov    cl, byte ptr [eax]
        cmp    byte ptr [edx], cl
        jne    SHORT loc_3_1FC0
        test   cl, cl
        je     SHORT loc_3_1FBC
        inc    eax
        inc    edx
        mov    ecx, esi
        dec    esi
        test   ecx, ecx
        jne    SHORT loc_3_1FA9
loc_3_1FBC:
        xor    eax, eax
loc_3_1FBE:
        pop    esi
        ret
loc_3_1FC0:
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [edx]
        sub    eax, ecx
        jmp    SHORT loc_3_1FBE
sub_3_1FCA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        test   esi, esi
        je     SHORT loc_3_2016
        mov    ebx, dword ptr [esp + 18h]
        test   ebx, ebx
        je     SHORT loc_3_2016
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        cmp    byte ptr [esi], 0
        lea    ebp, [ecx - 1]
        je     SHORT loc_3_2016
loc_3_1FF3:
        push   ebp
        push   ebx
        push   esi
        call   sub_3_1F95
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_3_201F
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    esi, ecx
        cmp    byte ptr [esi], 0
        jne    SHORT loc_3_1FF3
loc_3_2016:
        xor    eax, eax
loc_3_2018:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_3_201F:
        mov    eax, esi
        jmp    SHORT loc_3_2018
        db  000h
sub_3_2024:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   14h
        push   100h
        push   6
        call   sub_3_1260
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_2043
        xor    eax, eax
        jmp    SHORT loc_3_209B
loc_3_2043:
        call   sub_3_21DF
        mov    dword ptr [esi + 28h], eax
        test   eax, eax
        jne    SHORT loc_3_2059
        push   esi
        call   sub_3_1576
        xor    eax, eax
        jmp    SHORT loc_3_209B
loc_3_2059:
        mov    dword ptr [ebp - 8], 100h
        mov    ecx, dword ptr [ebp - 8]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_206D
        xor    eax, eax
loc_3_206D:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], eax
        test   eax, eax
        jne    SHORT loc_3_208C
        push   dword ptr [esi + 28h]
        call   sub_3_226C
        push   esi
        call   sub_3_1576
        xor    eax, eax
        jmp    SHORT loc_3_209B
loc_3_208C:
        mov    dword ptr [esi + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 24h], eax
        mov    eax, esi
loc_3_209B:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_20A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 28h]
        call   sub_3_226C
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        push   esi
        call   sub_3_1576
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_20D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        mov    dword ptr [ebp - 0ch], 1
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ebp - 18h], ax
        mov    dword ptr [ebp - 16h], ecx
        lea    eax, [ebp - 18h]
        push   eax
        push   1
        push   esi
        call   sub_3_130B
        push   dword ptr [esi + 24h]
        inc    dword ptr [esi + 1ch]
        call   sub_3_0BB0
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_3_2142:
        ret    4
loc_3_2145:
        ret    4
loc_3_2148:
        push   esi
        xor    ecx, ecx
        db  0BAh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
loc_3_2154:
        mov    esi, dword ptr [edx]
        cmp    dword ptr [esi], eax
        je     SHORT loc_3_216C
        add    edx, 4
        inc    ecx
        db  081h, 0FAh, 000h, 001h, 000h, 000h
        jb     SHORT loc_3_2154
        xor    eax, eax
loc_3_2168:
        pop    esi
        ret    4
loc_3_216C:
        db  08Bh, 004h, 08Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        jmp    SHORT loc_3_2168
sub_3_2178:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        push   1
        push   esi
        call   sub_3_13BA
        cmp    eax, 1
        mov    eax, 0
        jne    SHORT loc_3_21AB
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    eax, 1
loc_3_21AB:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_21B2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_15E3
        test   eax, eax
        je     SHORT loc_3_21D8
        mov    eax, dword ptr [esi + 20h]
        dec    dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_21D8:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_21DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   0ch
        push   32h
        push   1
        call   sub_3_1260
        mov    edx, eax
        test   edx, edx
        jne    SHORT loc_3_21FA
        xor    eax, eax
        jmp    SHORT loc_3_2268
loc_3_21FA:
        mov    dword ptr [ebp - 14h], 0
        mov    ecx, dword ptr [ebp - 14h]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_220E
        xor    eax, eax
loc_3_220E:
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [edx + 20h], eax
        test   eax, eax
        jne    SHORT loc_3_2225
        push   edx
        call   sub_3_1576
        xor    eax, eax
        jmp    SHORT loc_3_2268
loc_3_2225:
        mov    dword ptr [ebp - 0ch], 1
        mov    ecx, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_2239
        xor    eax, eax
loc_3_2239:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edx + 24h], eax
        test   eax, eax
        jne    SHORT loc_3_225F
        mov    eax, dword ptr [edx + 20h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        push   edx
        call   sub_3_1576
        xor    eax, eax
        jmp    SHORT loc_3_2268
loc_3_225F:
        mov    dword ptr [edx + 1ch], 0
        mov    eax, edx
loc_3_2268:
        mov    esp, ebp
        pop    ebp
        ret
sub_3_226C:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        push   ecx
        call   sub_3_1576
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_22A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 10h]
loc_3_22C6:
        push   ebx
        push   esi
        push   edi
        call   sub_3_130B
        mov    ecx, eax
        test   ecx, ecx
        jne    SHORT loc_3_231D
        mov    edx, dword ptr [edi + 1ch]
        mov    eax, dword ptr [edi + 18h]
        inc    edx
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [edi + 1ch], edx
        mov    eax, dword ptr [ebp - 10h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [edi + 20h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        dec    edx
        mov    dword ptr [edi + 1ch], edx
        jmp    SHORT loc_3_22C6
loc_3_231D:
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_3_2337:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_3_13BA
        cmp    dword ptr [esi + 1ch], 0
        mov    ecx, eax
        je     SHORT loc_3_237C
        test   ecx, ecx
        je     SHORT loc_3_237C
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_237C:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    eax, ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_3_2394:
        push   esi
        push   dword ptr [esp + 8]
        call   sub_3_193C
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_3_23B7
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_3_23B7
        push   esi
        call   sub_3_2878
        push   eax
        push   esi
        call   sub_3_2937
loc_3_23B7:
        pop    esi
        ret    4
sub_3_23BB:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        test   eax, eax
        je     SHORT loc_3_23D2
        cmp    dword ptr [eax + 4], 0
        je     SHORT loc_3_23D2
        or     byte ptr [eax + 1ch], 20h
loc_3_23D2:
        ret    4
        db  000h, 000h, 000h
sub_3_23D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
loc_3_23E4:
        push   edi
        call   sub_3_193C
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_3_24FA
        test   byte ptr [ebx + 1ch], 20h
        jne    NEAR PTR loc_3_24AB
        mov    esi, dword ptr [ebx + 4]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        lea    eax, [ebp - 28h]
        push   eax
        push   esi
        call   sub_3_1477
        test   eax, eax
        je     NEAR PTR loc_3_24B6
        movzx  eax, word ptr [ebp - 28h]
        sub    eax, 101h
        cmp    eax, 11h
        ja     NEAR PTR loc_3_24E1
        db  00Fh, 0B6h, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_3_2447:
        mov    cx, word ptr [ebp - 26h]
        mov    eax, dword ptr [ebp - 26h]
        shr    eax, 10h
        and    ax, 0ffh
        shr    cx, 8
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 26h]
        shr    eax, 18h
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ebp - 26h]
        and    eax, 0ffh
        cmp    word ptr [ebp - 28h], 10eh
        push   eax
        push   dword ptr [ebp - 4]
        push   ecx
        push   ebx
        jne    SHORT loc_3_2485
        call   sub_3_2BE5
        jmp    SHORT loc_3_248A
loc_3_2485:
        call   sub_3_2CA0
loc_3_248A:
        lea    eax, [ebp - 28h]
        push   eax
        push   esi
        call   sub_3_2178
        mov    ecx, dword ptr [esi + 18h]
        dec    dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        jmp    NEAR PTR loc_3_23E4
loc_3_24AB:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 1ch], 108h
        jmp    SHORT loc_3_24FA
loc_3_24B6:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 18h]
        mov    word ptr [ecx + 1ch], 108h
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        jmp    SHORT loc_3_24FA
loc_3_24D0:
        mov    ax, word ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 1ch], ax
        mov    edx, dword ptr [ebp - 26h]
        mov    dword ptr [ecx + 18h], edx
loc_3_24E1:
        lea    eax, [ebp - 28h]
        push   eax
        push   esi
        call   sub_3_2178
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 10h], ecx
        mov    eax, dword ptr [ebp - 10h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_24FA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_3_2503:
        mov    ecx, dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 10h]
        mov    word ptr [ecx + 1ch], 107h
        mov    ax, word ptr [ebp - 26h]
        mov    word ptr [ecx + 18h], ax
        movzx  ecx, word ptr [ebp - 26h]
        push   ecx
        push   dword ptr [esi + 28h]
        call   sub_3_2337
        jmp    SHORT loc_3_24E1
loc_3_2526:
        mov    word ptr [ebp - 28h], 10ah
        lea    eax, [ebp - 28h]
        push   eax
        push   esi
        call   sub_3_14D1
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 1ch], ecx
        mov    eax, dword ptr [ebp - 1ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 1ch], 108h
        mov    eax, dword ptr [ebp - 26h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        jmp    SHORT loc_3_24FA
loc_3_255F:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 1ch], 108h
        jmp    SHORT loc_3_24FA
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 001h, 001h, 002h, 008h, 003h, 004h, 005h, 005h
        db  005h, 006h, 006h, 007h, 007h, 007h
sub_3_25AF:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        test   eax, eax
        je     SHORT loc_3_25C2
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 1ch]
loc_3_25C2:
        ret    4
sub_3_25C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    word ptr [ebp - 14h], si
        mov    dword ptr [ebp - 10h], esi
        mov    word ptr [ebp - 0ch], si
        lea    edi, [ebp - 4]
        mov    word ptr [ebp - 0ah], si
        xor    eax, eax
        mov    word ptr [ebp - 8], si
        mov    word ptr [ebp - 6], si
        stosd
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    word ptr [ebp - 14h], 8000h
        mov    bx, word ptr [ebp + 0ch]
        mov    edi, eax
        mov    ax, bx
        push   1
        shr    ax, 8
        lea    ecx, [ebp - 14h]
        mov    word ptr [ebp - 0ah], ax
        push   ecx
        mov    word ptr [ebp - 0ch], si
        push   edi
        mov    byte ptr [ebp - 6], bl
        call   sub_3_32BE
        cmp    eax, 1
        je     SHORT loc_3_2629
        call   sub_3_111D
loc_3_2629:
        cmp    bx, 0dh
        jne    SHORT loc_3_2663
        push   edi
        call   sub_3_3084
        test   al, 1
        je     SHORT loc_3_2663
        push   edi
        call   sub_3_3084
        test   al, 2
        je     SHORT loc_3_2663
        push   1
        lea    eax, [ebp - 14h]
        mov    byte ptr [ebp - 6], 0ah
        mov    word ptr [ebp - 0ch], 1
        push   eax
        push   edi
        call   sub_3_32BE
        cmp    eax, 1
        je     SHORT loc_3_2663
        call   sub_3_111D
loc_3_2663:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_266C:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    word ptr [ebp - 14h], 2
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, esi
        mov    edi, esi
        shr    ecx, 10h
        and    edi, 3e00h
        and    cx, 0ffh
        mov    word ptr [ebp - 10h], cx
        mov    ecx, esi
        shr    ecx, 18h
        mov    word ptr [ebp - 0eh], cx
        mov    ecx, edi
        shr    ecx, 9
        mov    dword ptr [ebp - 0ch], ecx
        mov    ecx, esi
        and    ecx, 0ffh
        and    esi, 8000h
        push   1
        lea    edx, [ebp - 14h]
        push   edx
        mov    dword ptr [ebp - 8], ecx
        push   eax
        cmp    esi, 1
        sbb    ecx, ecx
        neg    ecx
        mov    dword ptr [ebp - 4], ecx
        call   sub_3_32BE
        cmp    eax, 1
        je     SHORT loc_3_26E5
        test   esi, esi
        je     SHORT loc_3_26E5
        test   edi, edi
        je     SHORT loc_3_26E5
        call   sub_3_111D
loc_3_26E5:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_26ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    word ptr [ebp - 14h], 1
        mov    esi, eax
        mov    cl, byte ptr [ebp + 0ch]
        mov    word ptr [ebp - 0ch], 1
        mov    al, cl
        and    al, 80h
        cmp    al, 1
        sbb    eax, eax
        lea    edx, [ebp - 10h]
        and    cx, 7fh
        push   edx
        neg    eax
        mov    word ptr [ebp - 8], cx
        mov    dword ptr [ebp - 10h], eax
        movzx  eax, word ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        call   sub_3_3697
        push   1
        lea    ecx, [ebp - 14h]
        push   ecx
        push   esi
        call   sub_3_32BE
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_3_2745:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        test   eax, eax
        je     SHORT loc_3_2776
        mov    cx, word ptr [esp + 8]
        push   dword ptr [eax + 28h]
        push   4
        mov    dx, word ptr [esp + 12h]
        mov    word ptr [eax + 18h], cx
        movzx  ecx, word ptr [esp + 14h]
        mov    word ptr [eax + 1ah], dx
        mov    dword ptr [eax + 2ch], ecx
        call   sub_3_0E79
loc_3_2776:
        ret    0ch
sub_3_2779:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_3_27CE
        and    byte ptr [esi + 1ch], 0bfh
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_3_279E
        push   esi
        call   sub_3_38B0
loc_3_279E:
        mov    ecx, dword ptr [esi + 28h]
        test   ecx, ecx
        je     SHORT loc_3_27CE
        mov    eax, dword ptr [ecx + 18h]
        test   al, 80h
        je     SHORT loc_3_27CE
        mov    dword ptr [ebp - 4], 1
        or     ah, 4
        mov    dword ptr [ecx + 18h], eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
loc_3_27CE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_27D5:
        sub    esp, 4
        push   ebx
        push   esi
        xor    bx, bx
        push   edi
        push   ebp
        push   dword ptr [esp + 18h]
        call   sub_3_193C
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_3_27F3
        xor    ax, ax
        jmp    SHORT loc_3_2852
loc_3_27F3:
        mov    ebp, dword ptr [edi + 1ch]
        and    ebp, 80h
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_3_2846
        mov    esi, dword ptr [esp + 1ch]
loc_3_2807:
        lea    eax, [esp + 13h]
        push   1
        push   -1
        push   eax
        push   edi
        call   sub_3_3091
        test   eax, eax
        je     SHORT loc_3_2846
        mov    cl, byte ptr [esp + 13h]
        mov    byte ptr [esi], cl
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_3_2832
        push   dword ptr [esp + 13h]
        call   sub_3_1F24
        jmp    SHORT loc_3_2835
loc_3_2832:
        shr    eax, 18h
loc_3_2835:
        inc    bx
        mov    byte ptr [esi + 1], al
        movzx  eax, bx
        add    esi, 2
        cmp    eax, dword ptr [esp + 20h]
        jb     SHORT loc_3_2807
loc_3_2846:
        and    byte ptr [edi + 1ch], 7fh
        cmp    ebp, 1
        sbb    eax, eax
        and    ax, bx
loc_3_2852:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
sub_3_285C:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        test   eax, eax
        jne    SHORT loc_3_286D
        xor    eax, eax
        jmp    SHORT loc_3_2873
loc_3_286D:
        push   eax
        call   sub_3_35E8
loc_3_2873:
        ret    4
        db  000h, 000h
sub_3_2878:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 28h
        cmp    dword ptr [eax + 20h], 0
        push   esi
        mov    edx, dword ptr [eax + 4]
        jne    SHORT loc_3_28A7
        mov    dword ptr [ebp - 24h], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 24h]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
loc_3_28A7:
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        cmp    dword ptr [edx + 1ch], 0
        jne    SHORT loc_3_28D4
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        xor    eax, eax
        jmp    SHORT loc_3_2930
loc_3_28D4:
        mov    dword ptr [ebp - 18h], 0
        mov    ecx, dword ptr [ebp - 18h]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_28E8
        xor    eax, eax
loc_3_28E8:
        mov    dword ptr [ebp - 14h], eax
        mov    esi, dword ptr [ebp - 14h]
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        push   esi
        push   109h
        push   edx
        call   sub_3_20D0
        mov    dword ptr [ebp - 8], 1
        mov    dword ptr [ebp - 0ch], esi
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], esi
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    eax, 1
loc_3_2930:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_2937:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        cmp    dword ptr [ebp + 0ch], 0
        push   esi
        je     SHORT loc_3_297A
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        lea    eax, [ebp - 10h]
        dec    dword ptr [esi + 1ch]
        push   eax
        push   esi
        call   sub_3_2178
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_297A:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_2981:
        mov    edx, dword ptr [esp + 4]
        movsx  ecx, word ptr [esp + 0ah]
        movsx  eax, word ptr [edx + 18h]
        imul   eax, ecx
        mov    ecx, dword ptr [edx + 2ch]
        lea    eax, [eax + ecx*8]
        movsx  ecx, word ptr [esp + 8]
        add    eax, ecx
        add    eax, eax
        add    eax, dword ptr [edx + 20h]
        ret    8
sub_3_29A6:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   0
        push   esi
        call   sub_3_2F75
        push   esi
        call   sub_3_2878
        push   dword ptr [esp + 1ch]
        push   esi
        mov    edi, eax
        call   sub_3_2981
        mov    ebx, dword ptr [esp + 18h]
        test   ebx, ebx
        je     SHORT loc_3_29E9
        mov    edx, dword ptr [esp + 14h]
loc_3_29D3:
        mov    cl, byte ptr [eax]
        dec    ebx
        inc    eax
        mov    byte ptr [edx], cl
        movzx  cx, byte ptr [eax]
        mov    word ptr [edx + 2], cx
        inc    eax
        add    edx, 4
        test   ebx, ebx
        jne    SHORT loc_3_29D3
loc_3_29E9:
        push   edi
        push   esi
        call   sub_3_2937
        push   1
        push   esi
        call   sub_3_2F75
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
sub_3_29FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   ebx
        call   sub_3_2F75
        push   ebx
        call   sub_3_2878
        push   dword ptr [ebp + 10h]
        push   ebx
        mov    dword ptr [ebp - 4], eax
        call   sub_3_2981
        mov    dword ptr [ebp - 8], eax
        push   ds
        pop    es
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        cld
        or     ecx, ecx
loc_3_2A35:
        je     SHORT loc_3_2A3C
        movsb
        inc    esi
        dec    ecx
        jmp    SHORT loc_3_2A35
loc_3_2A3C:
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_3_2937
        push   1
        push   ebx
        call   sub_3_2F75
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_3_2A59:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   ebx
        call   sub_3_2F75
        push   ebx
        call   sub_3_2878
        push   dword ptr [ebp + 10h]
        push   ebx
        mov    dword ptr [ebp - 4], eax
        call   sub_3_2981
        mov    dword ptr [ebp - 8], eax
        push   ds
        pop    es
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        cld
        cmp    dword ptr [ebp + 18h], 0
        jne    SHORT loc_3_2AA3
        or     ecx, ecx
loc_3_2A96:
        je     SHORT loc_3_2AAD
        lodsw
        mov    al, ah
        xor    ah, ah
        stosw
        dec    ecx
        jmp    SHORT loc_3_2A96
loc_3_2AA3:
        jecxz  loc_3_2AAD
loc_3_2AA5:
        lodsw
        mov    al, ah
        stosb
        dec    ecx
        jne    SHORT loc_3_2AA5
loc_3_2AAD:
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_3_2937
        push   1
        push   ebx
        call   sub_3_2F75
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
sub_3_2ACA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   0
        push   esi
        call   sub_3_2F75
        push   esi
        call   sub_3_2878
        push   dword ptr [esp + 1ch]
        push   esi
        mov    edi, eax
        call   sub_3_2981
        mov    ebx, dword ptr [esp + 18h]
        test   ebx, ebx
        je     SHORT loc_3_2B0A
        mov    edx, dword ptr [esp + 14h]
loc_3_2AF7:
        mov    cl, byte ptr [edx]
        dec    ebx
        add    edx, 4
        mov    byte ptr [eax], cl
        inc    eax
        mov    cl, byte ptr [edx - 2]
        mov    byte ptr [eax], cl
        inc    eax
        test   ebx, ebx
        jne    SHORT loc_3_2AF7
loc_3_2B0A:
        push   edi
        push   esi
        call   sub_3_2937
        push   1
        push   esi
        call   sub_3_2F75
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
sub_3_2B1F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   ebx
        call   sub_3_2F75
        push   ebx
        call   sub_3_2878
        push   dword ptr [ebp + 10h]
        push   ebx
        mov    dword ptr [ebp - 4], eax
        call   sub_3_2981
        mov    dword ptr [ebp - 8], eax
        push   ds
        pop    es
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 14h]
        cld
        or     ecx, ecx
loc_3_2B56:
        je     SHORT loc_3_2B5D
        movsb
        inc    edi
        dec    ecx
        jmp    SHORT loc_3_2B56
loc_3_2B5D:
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_3_2937
        push   1
        push   ebx
        call   sub_3_2F75
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_3_2B7A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   ebx
        call   sub_3_2F75
        push   ebx
        call   sub_3_2878
        push   dword ptr [ebp + 10h]
        push   ebx
        mov    dword ptr [ebp - 4], eax
        call   sub_3_2981
        mov    dword ptr [ebp - 8], eax
        push   ds
        pop    es
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 14h]
        cld
        cmp    dword ptr [ebp + 18h], 0
        jne    SHORT loc_3_2BC0
        or     ecx, ecx
loc_3_2BB7:
        je     SHORT loc_3_2BC8
        lodsw
        inc    edi
        stosb
        dec    ecx
        jmp    SHORT loc_3_2BB7
loc_3_2BC0:
        jecxz  loc_3_2BC8
loc_3_2BC2:
        lodsb
        inc    edi
        stosb
        dec    ecx
        jne    SHORT loc_3_2BC2
loc_3_2BC8:
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_3_2937
        push   1
        push   ebx
        call   sub_3_2F75
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
sub_3_2BE5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_3_2981
        mov    dword ptr [ebp - 4], eax
        push   ds
        pop    es
        mov    al, byte ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 14h]
        cld
        or     ecx, ecx
loc_3_2C08:
        je     SHORT loc_3_2C0F
        stosb
        inc    edi
        dec    ecx
        jmp    SHORT loc_3_2C08
loc_3_2C0F:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_3_2C19:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 1ch], 1
        jbe    SHORT loc_3_2C69
        cmp    edi, 100h
        jae    SHORT loc_3_2C69
        movzx  ebx, byte ptr [ebp + 0ch]
        movsx  eax, word ptr [ebp + 12h]
        shl    eax, 10h
        push   0
        or     ebx, eax
        push   esi
        movsx  eax, word ptr [ebp + 10h]
        shl    ebx, 8
        shl    eax, 10h
        or     ebx, eax
        call   sub_3_2F75
        or     ebx, edi
        push   ebx
        push   10eh
        push   dword ptr [esi + 4]
        call   sub_3_20D0
        jmp    SHORT loc_3_2C8F
loc_3_2C69:
        push   0
        push   esi
        call   sub_3_2F75
        push   esi
        call   sub_3_2878
        mov    ebx, eax
        push   edi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_3_2BE5
        mov    edi, eax
        push   ebx
        push   esi
        call   sub_3_2937
loc_3_2C8F:
        push   1
        push   esi
        call   sub_3_2F75
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
sub_3_2CA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_3_2981
        mov    dword ptr [ebp - 4], eax
        push   ds
        pop    es
        mov    ax, word ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 14h]
        cld
        or     ecx, ecx
loc_3_2CC4:
        je     SHORT loc_3_2CCB
        inc    edi
        stosb
        dec    ecx
        jmp    SHORT loc_3_2CC4
loc_3_2CCB:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_3_2CD5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 1ch], 1
        jbe    SHORT loc_3_2D25
        cmp    edi, 100h
        jae    SHORT loc_3_2D25
        movsx  ebx, word ptr [ebp + 12h]
        movzx  eax, word ptr [ebp + 0ch]
        shl    ebx, 10h
        push   0
        or     ebx, eax
        push   esi
        movsx  eax, word ptr [ebp + 10h]
        shl    ebx, 8
        shl    eax, 10h
        or     ebx, eax
        call   sub_3_2F75
        or     ebx, edi
        push   ebx
        push   10fh
        push   dword ptr [esi + 4]
        call   sub_3_20D0
        jmp    SHORT loc_3_2D4B
loc_3_2D25:
        push   0
        push   esi
        call   sub_3_2F75
        push   esi
        call   sub_3_2878
        mov    ebx, eax
        push   edi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_3_2CA0
        mov    edi, eax
        push   ebx
        push   esi
        call   sub_3_2937
loc_3_2D4B:
        push   1
        push   esi
        call   sub_3_2F75
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
sub_3_2D5C:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_3_2878
        mov    ecx, dword ptr [esi + 20h]
        push   eax
        add    ecx, 450h
        push   esi
        movzx  dx, byte ptr [ecx]
        mov    word ptr [esp + 0ch], dx
        movzx  cx, byte ptr [ecx + 1]
        mov    word ptr [esp + 0eh], cx
        call   sub_3_2937
        mov    eax, dword ptr [esp + 4]
        pop    esi
        add    esp, 4
        ret    4
sub_3_2D98:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   esi
        call   sub_3_2878
        mov    edi, eax
        mov    ecx, dword ptr [esi + 20h]
        mov    al, byte ptr [ecx + 460h]
        mov    dl, byte ptr [ecx + 461h]
        and    eax, 1fh
        and    edx, 1fh
        movzx  ecx, byte ptr [ecx + 485h]
        sub    eax, edx
        inc    eax
        imul   eax, eax, 64h
        cdq
        idiv   ecx
        mov    bx, ax
        test   byte ptr [esi + 1ch], 2
        je     SHORT loc_3_2DD8
        or     bh, 80h
loc_3_2DD8:
        push   edi
        push   esi
        call   sub_3_2937
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_3_2DE8:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        push   101h
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        ret    8
sub_3_2E00:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        test   ax, ax
        je     SHORT loc_3_2E14
        or     byte ptr [ecx + 1ch], 2
        jmp    SHORT loc_3_2E18
loc_3_2E14:
        and    byte ptr [ecx + 1ch], 0fdh
loc_3_2E18:
        movzx  eax, ax
        push   eax
        push   102h
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        ret    8
sub_3_2E2C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 20h
        cmp    dword ptr [ebp + 10h], 0
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 4]
        mov    esi, dword ptr [edi + 28h]
        je     NEAR PTR loc_3_2EEE
loc_3_2E49:
        mov    dword ptr [ebp - 14h], 1
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_3_22A0
        mov    ebx, eax
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        lea    eax, [ebp - 20h]
        push   eax
        push   edi
        call   sub_3_149E
        test   eax, eax
        je     SHORT loc_3_2EB8
        cmp    word ptr [ebp - 20h], 107h
        jne    SHORT loc_3_2EB8
        lea    eax, [ebp - 20h]
        add    dword ptr [ebp - 1eh], ebx
        push   eax
        push   edi
        call   sub_3_14F8
        mov    ecx, dword ptr [edi + 18h]
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        jmp    SHORT loc_3_2ED3
loc_3_2EB8:
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        push   ebx
        push   107h
        push   edi
        call   sub_3_20D0
loc_3_2ED3:
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        add    dword ptr [ebp + 0ch], ebx
        sub    dword ptr [ebp + 10h], ebx
        jne    NEAR PTR loc_3_2E49
loc_3_2EEE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_3_2EF7:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_2878
        push   eax
        push   esi
        call   sub_3_2937
        mov    eax, dword ptr [esi + 18h]
        pop    esi
        ret    4
sub_3_2F10:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        push   110h
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        mov    eax, 1
        ret    8
sub_3_2F2D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        push   111h
        push   dword ptr [esi + 4]
        call   sub_3_20D0
        push   esi
        call   sub_3_2878
        push   eax
        push   esi
        call   sub_3_2937
        mov    eax, dword ptr [esi + 30h]
        pop    esi
        ret    8
sub_3_2F57:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        mov    cx, word ptr [esp + 8]
        mov    dx, word ptr [esp + 0ah]
        mov    word ptr [eax + 30h], cx
        mov    word ptr [eax + 32h], dx
        ret    8
sub_3_2F75:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        test   byte ptr [eax + 1ch], 10h
        push   esi
        mov    esi, dword ptr [eax + 4]
        je     SHORT loc_3_2FEF
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        lea    eax, [ebp - 14h]
        push   eax
        push   esi
        call   sub_3_149E
        test   eax, eax
        je     SHORT loc_3_2FD2
        cmp    word ptr [ebp - 14h], 10dh
        jne    SHORT loc_3_2FD2
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 12h], eax
        je     SHORT loc_3_2FD2
        push   esi
        call   sub_3_21B2
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        jmp    SHORT loc_3_2FEF
loc_3_2FD2:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        push   dword ptr [ebp + 0ch]
        push   10dh
        push   esi
        call   sub_3_20D0
loc_3_2FEF:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_2FF6:
        movzx  eax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        push   eax
        push   103h
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        ret    8
sub_3_3010:
        push   24h
        push   100h
        push   14h
        call   sub_3_1260
        test   eax, eax
        jne    SHORT loc_3_3026
        xor    eax, eax
        jmp    SHORT loc_3_303E
loc_3_3026:
        mov    dword ptr [eax + 1ch], 7
        mov    word ptr [eax + 2ch], 0ffffh
        xor    ecx, ecx
        mov    dword ptr [eax + 28h], ecx
        mov    dword ptr [eax + 34h], ecx
        mov    dword ptr [eax + 38h], ecx
loc_3_303E:
        ret
sub_3_303F:
        push   dword ptr [esp + 4]
        call   sub_3_1576
        ret    4
sub_3_304B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 8]
        test   ecx, ecx
        je     SHORT loc_3_3080
        and    byte ptr [esi + 1ch], 0bfh
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 1ch], eax
        test   al, 10h
        jne    SHORT loc_3_3078
        push   0
        push   esi
        call   sub_3_3589
        push   0
        push   esi
        call   sub_3_2F75
        jmp    SHORT loc_3_3080
loc_3_3078:
        push   1
        push   esi
        call   sub_3_3589
loc_3_3080:
        pop    esi
        ret    8
sub_3_3084:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ecx + 1ch]
        ret    4
sub_3_3091:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 8]
        xor    eax, eax
        mov    dword ptr [ebp - 0ch], eax
        mov    byte ptr [ebp - 1], al
        cmp    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [ebp + 8]
        je     SHORT loc_3_30B7
        and    byte ptr [eax + 1ch], 7fh
        jmp    SHORT loc_3_30BB
loc_3_30B7:
        or     byte ptr [eax + 1ch], 80h
loc_3_30BB:
        cmp    dword ptr [ebp + 10h], -1
        jne    SHORT loc_3_30D2
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_3_30D2
        mov    eax, 1
        mov    dword ptr [ebp + 10h], eax
        mov    dword ptr [ebp - 0ch], eax
loc_3_30D2:
        mov    eax, dword ptr [ebx + 18h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        push   ebx
        call   sub_3_3968
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        cmp    dword ptr [ebp + 14h], 0
        mov    dword ptr [ebp - 18h], eax
        jne    SHORT loc_3_3104
        push   dword ptr [ebp + 8]
        call   sub_3_393F
loc_3_3104:
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_3_31D6
loc_3_3115:
        lea    eax, [ebp - 30h]
        push   eax
        push   1
        push   ebx
        call   sub_3_13BA
        test   eax, eax
        je     NEAR PTR loc_3_31D6
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_3_319B
        cmp    word ptr [ebp - 30h], 1
        jne    SHORT loc_3_3159
        cmp    dword ptr [ebp - 2ch], 0
        je     SHORT loc_3_3159
        cmp    word ptr [ebp - 24h], 2eh
        jne    SHORT loc_3_3159
        test   byte ptr [ebp - 20h], 0ch
        je     SHORT loc_3_3159
        mov    byte ptr [ebp - 22h], 3
        mov    word ptr [ebp - 30h], 8000h
        mov    word ptr [ebp - 28h], 1
loc_3_3159:
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_3_31A1
        cmp    word ptr [ebp - 30h], 8000h
        jne    SHORT loc_3_319B
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_3_3173
        test   byte ptr [ebp - 28h], 1
        jne    SHORT loc_3_319B
loc_3_3173:
        cmp    byte ptr [ebp - 22h], 0
        jne    SHORT loc_3_317F
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_3_31CA
loc_3_317F:
        mov    cl, byte ptr [ebp - 22h]
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [ebp - 8]
        inc    dword ptr [ebp + 0ch]
        mov    byte ptr [eax], cl
        cmp    byte ptr [ebp - 22h], 0
        jne    SHORT loc_3_31CA
        mov    al, byte ptr [ebp - 26h]
        mov    byte ptr [ebp - 1], al
        jmp    SHORT loc_3_31CA
loc_3_319B:
        cmp    dword ptr [ebp + 14h], 0
        jne    SHORT loc_3_31CA
loc_3_31A1:
        cmp    word ptr [ebp - 30h], 8000h
        je     SHORT loc_3_31CA
        cmp    word ptr [ebp - 30h], 2
        jne    SHORT loc_3_31B6
        test   byte ptr [ebx + 1ch], 10h
        je     SHORT loc_3_31CA
loc_3_31B6:
        lea    esi, [ebp - 30h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 5
        rep movsd
        add    dword ptr [ebp + 0ch], 14h
        inc    dword ptr [ebp - 8]
loc_3_31CA:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 10h], eax
        ja     NEAR PTR loc_3_3115
loc_3_31D6:
        cmp    dword ptr [ebp - 18h], 0
        jne    SHORT loc_3_3205
        push   ebx
        call   sub_3_3968
        test   eax, eax
        jne    SHORT loc_3_3205
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 18h]
        mov    dword ptr [ebp - 14h], edx
        mov    eax, dword ptr [ebp - 14h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        push   dword ptr [ebp + 8]
        call   sub_3_3578
        jmp    SHORT loc_3_321A
loc_3_3205:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 18h]
        mov    dword ptr [ebp - 10h], edx
        mov    eax, dword ptr [ebp - 10h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_321A:
        movsx  eax, byte ptr [ebp - 1]
        shl    eax, 18h
        pop    edi
        or     eax, dword ptr [ebp - 8]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_3_322D:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        push   dword ptr [ebp + 8]
        call   sub_3_393F
        mov    dword ptr [ebp - 4], 0
        push   0
        push   esi
        call   sub_3_153B
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_3_329D
        mov    edi, dword ptr [ebp + 0ch]
loc_3_3271:
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 10h]
        jae    SHORT loc_3_329D
        cmp    word ptr [ebx], 8000h
        je     SHORT loc_3_3290
        push   1
        push   14h
        push   ebx
        push   edi
        call   sub_3_1E10
        mov    edi, eax
        inc    dword ptr [ebp - 4]
loc_3_3290:
        push   ebx
        push   esi
        call   sub_3_153B
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_3_3271
loc_3_329D:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 18h]
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_3_32BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        test   edi, edi
        jne    SHORT loc_3_32D8
        xor    eax, eax
        jmp    NEAR PTR loc_3_34CD
loc_3_32D8:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 14h], ecx
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        push   edi
        call   sub_3_3968
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_3_34AC
        mov    ebx, dword ptr [ebp + 0ch]
loc_3_3319:
        mov    cx, word ptr [ebx]
        cmp    cx, 1
        jne    SHORT loc_3_3370
        test   byte ptr [ebx + 10h], 0ch
        je     SHORT loc_3_3370
        mov    ax, word ptr [ebx + 0ch]
        cmp    ax, 46h
        jne    SHORT loc_3_334B
        cmp    dword ptr [ebx + 4], 0
        je     NEAR PTR loc_3_349A
        push   dword ptr [esi + 28h]
        push   3
        call   sub_3_0E79
        jmp    NEAR PTR loc_3_349A
loc_3_334B:
        cmp    ax, 2eh
        jne    SHORT loc_3_3370
        test   byte ptr [edi + 1ch], 1
        je     SHORT loc_3_3370
        cmp    dword ptr [ebx + 4], 0
        je     NEAR PTR loc_3_349A
        push   dword ptr [esi + 28h]
        push   2
        call   sub_3_0E79
        jmp    NEAR PTR loc_3_349A
loc_3_3370:
        test   byte ptr [edi + 1ch], 10h
        jne    SHORT loc_3_3380
        cmp    cx, 2
        je     NEAR PTR loc_3_349A
loc_3_3380:
        cmp    cx, 2
        jne    NEAR PTR loc_3_341F
        mov    eax, dword ptr [ebx + 8]
        mov    ecx, dword ptr [edi + 34h]
        not    ecx
        and    ecx, eax
        mov    dword ptr [edi + 34h], eax
        je     SHORT loc_3_33DD
        cmp    dword ptr [edi + 28h], ecx
        jne    SHORT loc_3_33C4
        mov    ax, word ptr [edi + 20h]
        cmp    word ptr [ebx + 4], ax
        jne    SHORT loc_3_33C4
        mov    ax, word ptr [edi + 22h]
        cmp    word ptr [ebx + 6], ax
        jne    SHORT loc_3_33C4
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [edi + 24h]
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jae    SHORT loc_3_33C4
        or     byte ptr [ebx + 10h], 2
loc_3_33C4:
        mov    ax, word ptr [ebx + 4]
        mov    dx, word ptr [ebx + 6]
        mov    word ptr [edi + 20h], ax
        mov    word ptr [edi + 22h], dx
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [edi + 28h], ecx
loc_3_33DD:
        lea    eax, [ebp - 28h]
        push   eax
        push   edi
        call   sub_3_149E
        test   eax, eax
        je     SHORT loc_3_341F
        mov    eax, dword ptr [ebp - 20h]
        cmp    dword ptr [ebx + 8], eax
        jne    SHORT loc_3_341F
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [ebx + 0ch], eax
        jne    SHORT loc_3_341F
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [ebx + 10h], eax
        jne    SHORT loc_3_341F
        mov    ax, word ptr [ebx + 4]
        lea    edx, [ebp - 28h]
        mov    word ptr [ebp - 24h], ax
        push   edx
        mov    cx, word ptr [ebx + 6]
        push   edi
        mov    word ptr [ebp - 22h], cx
        call   sub_3_14F8
        jmp    SHORT loc_3_349A
loc_3_341F:
        lea    eax, [ebp - 28h]
        push   eax
        push   edi
        call   sub_3_149E
        test   eax, eax
        je     SHORT loc_3_3464
        cmp    word ptr [ebx], 1
        jne    SHORT loc_3_3464
        cmp    dword ptr [ebx + 4], 0
        je     SHORT loc_3_3464
        cmp    word ptr [ebp - 28h], 1
        jne    SHORT loc_3_3464
        cmp    dword ptr [ebp - 24h], 0
        je     SHORT loc_3_3464
        mov    ax, word ptr [ebp - 1ch]
        cmp    word ptr [ebx + 0ch], ax
        jne    SHORT loc_3_3464
        mov    ax, word ptr [ebx + 8]
        lea    ecx, [ebp - 28h]
        add    word ptr [ebp - 20h], ax
        push   ecx
        push   edi
        call   sub_3_14F8
        jmp    SHORT loc_3_349A
loc_3_3464:
        push   ebx
        push   1
        push   edi
        call   sub_3_130B
        cmp    eax, 1
        jne    SHORT loc_3_34AC
        mov    ax, word ptr [ebx]
        cmp    ax, 1
        je     SHORT loc_3_3493
        cmp    ax, 2
        je     SHORT loc_3_3493
        cmp    ax, 4
        je     SHORT loc_3_3493
        cmp    ax, 8
        je     SHORT loc_3_3493
        cmp    ax, 10h
        jne    SHORT loc_3_349A
loc_3_3493:
        mov    dword ptr [ebp - 0ch], 1
loc_3_349A:
        add    ebx, 14h
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 10h]
        jb     NEAR PTR loc_3_3319
loc_3_34AC:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 10h], ecx
        mov    eax, dword ptr [ebp - 10h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_3_34CA
        push   esi
        call   sub_3_3567
loc_3_34CA:
        mov    eax, dword ptr [ebp - 4]
loc_3_34CD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_3_34D6:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    ebp, ebp
        mov    word ptr [esp + 10h], bp
        mov    dword ptr [esp + 14h], ebp
        mov    word ptr [esp + 18h], bp
        lea    edi, [esp + 20h]
        mov    word ptr [esp + 1ah], bp
        xor    eax, eax
        mov    word ptr [esp + 1ch], bp
        xor    ebx, ebx
        mov    word ptr [esp + 1eh], bp
        stosd
        cmp    dword ptr [esp + 30h], ebp
        je     SHORT loc_3_355B
        mov    esi, dword ptr [esp + 28h]
        mov    edi, dword ptr [esp + 2ch]
loc_3_3513:
        push   1
        push   edi
        push   esi
        call   sub_3_32BE
        cmp    eax, 1
        jne    SHORT loc_3_355B
        cmp    word ptr [edi], 1
        jne    SHORT loc_3_3551
        cmp    dword ptr [edi + 4], ebp
        je     SHORT loc_3_3551
        lea    ecx, [esp + 10h]
        push   1
        mov    word ptr [esp + 14h], 8000h
        mov    al, byte ptr [edi + 0eh]
        push   ecx
        mov    word ptr [esp + 20h], bp
        push   esi
        mov    byte ptr [esp + 2ah], al
        call   sub_3_32BE
        cmp    eax, 1
        jne    SHORT loc_3_355B
loc_3_3551:
        inc    ebx
        add    edi, 14h
        cmp    ebx, dword ptr [esp + 30h]
        jb     SHORT loc_3_3513
loc_3_355B:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    0ch
sub_3_3567:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 28h]
        push   1
        call   sub_3_0E79
        ret    4
sub_3_3578:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 28h]
        push   0
        call   sub_3_0E79
        ret    4
sub_3_3589:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        push   106h
        push   dword ptr [ecx + 4]
        call   sub_3_20D0
        ret    8
sub_3_35A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_393F
        mov    esi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        push   esi
        call   sub_3_3968
        mov    ecx, eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    eax, ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_35E8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    ebx, esi
        xor    eax, eax
        xor    edi, edi
loc_3_3612:
        push   eax
        push   ebx
        call   sub_3_153B
        test   eax, eax
        je     SHORT loc_3_362B
        movzx  ecx, word ptr [eax]
        cmp    ecx, 8000h
        jne    SHORT loc_3_3612
        inc    edi
        jmp    SHORT loc_3_3612
loc_3_362B:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_3645:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        jne    SHORT loc_3_3690
        xor    esi, esi
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_3_3657:
        push   4
        push   esi
        call   sub_3_1EE1
        test   ax, ax
        jne    SHORT loc_3_367C
        push   5
        push   esi
        call   sub_3_1EE1
        test   ax, ax
        jne    SHORT loc_3_367C
        inc    si
        cmp    si, 80h
        jb     SHORT loc_3_3657
        jmp    SHORT loc_3_3686
loc_3_367C:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_3_3686:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_3_3690:
        db  0A1h, 000h, 000h, 000h, 000h
        pop    esi
        ret
sub_3_3697:
        sub    esp, 8
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        push   ebp
        mov    ecx, dword ptr [esi + 0ch]
        mov    ebx, ecx
        mov    ebp, ecx
        and    ebx, 10h
        and    ebp, 0ch
        mov    edi, ecx
        mov    eax, ecx
        and    edi, 3
        and    eax, 1
        mov    edx, ecx
        mov    dword ptr [esp + 14h], eax
        mov    eax, ecx
        and    edx, 100h
        and    eax, 20h
        test   eax, eax
        je     SHORT loc_3_36D6
        test   ebx, ebx
        je     SHORT loc_3_36DA
        test   eax, eax
        jne    SHORT loc_3_36EE
loc_3_36D6:
        test   ebx, ebx
        je     SHORT loc_3_36EE
loc_3_36DA:
        test   edi, edi
        jne    SHORT loc_3_36EE
        test   ebp, ebp
        jne    SHORT loc_3_36EE
        mov    dword ptr [esp + 10h], 1
        test   edx, edx
        je     SHORT loc_3_36F6
loc_3_36EE:
        mov    dword ptr [esp + 10h], 0
loc_3_36F6:
        test   cl, 80h
        je     SHORT loc_3_3702
        cmp    ebx, 1
        sbb    ebx, ebx
        neg    ebx
loc_3_3702:
        mov    ax, word ptr [esi + 8]
        push   3
        push   eax
        call   sub_3_1EE1
        mov    word ptr [esi + 0ah], 0
        mov    word ptr [esi + 6], ax
        call   sub_3_3645
        test   eax, eax
        je     SHORT loc_3_3733
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_3733
        cmp    ebx, 1
        sbb    ecx, ecx
        add    cx, 5
        jmp    SHORT loc_3_3742
loc_3_3733:
        mov    cx, 2
        test   ebp, ebp
        jne    SHORT loc_3_3742
        cmp    ebx, 1
        sbb    ecx, ecx
        inc    cx
loc_3_3742:
        movzx  edx, word ptr [esi + 6]
        cmp    edx, 6ah
        je     SHORT loc_3_377A
        cmp    edx, 6bh
        je     SHORT loc_3_3788
        cmp    edx, 6dh
        je     SHORT loc_3_37A6
loc_3_3755:
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_3_3845
        sub    edx, 0ch
        cmp    edx, 22h
        ja     NEAR PTR loc_3_3845
        db  00Fh, 0B6h, 082h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_3_377A:
        test   cx, cx
        jne    SHORT loc_3_3755
        mov    byte ptr [esi + 0ah], 2ah
        jmp    NEAR PTR loc_3_3853
loc_3_3788:
        lea    eax, [ecx - 2]
        cmp    edi, 1
        sbb    edi, edi
        neg    edi
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        test   eax, edi
        je     SHORT loc_3_3755
        mov    byte ptr [esi + 0ah], 2bh
        jmp    NEAR PTR loc_3_3853
loc_3_37A6:
        lea    eax, [ecx - 2]
        cmp    edi, 1
        sbb    edi, edi
        neg    edi
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        test   eax, edi
        je     SHORT loc_3_3755
        mov    byte ptr [esi + 0ah], 2dh
        jmp    NEAR PTR loc_3_3853
loc_3_37C4:
        mov    byte ptr [esi + 0ah], 35h
        mov    word ptr [esi + 6], 65h
        jmp    NEAR PTR loc_3_3853
loc_3_37D3:
        mov    byte ptr [esi + 0ah], 39h
        mov    word ptr [esi + 6], 69h
        jmp    SHORT loc_3_3853
loc_3_37DF:
        mov    byte ptr [esi + 0ah], 33h
        mov    word ptr [esi + 6], 63h
        jmp    SHORT loc_3_3853
loc_3_37EB:
        mov    byte ptr [esi + 0ah], 31h
        mov    word ptr [esi + 6], 61h
        jmp    SHORT loc_3_3853
loc_3_37F7:
        mov    byte ptr [esi + 0ah], 37h
        mov    word ptr [esi + 6], 67h
        jmp    SHORT loc_3_3853
loc_3_3803:
        mov    byte ptr [esi + 0ah], 34h
        mov    word ptr [esi + 6], 64h
        jmp    SHORT loc_3_3853
loc_3_380F:
        mov    byte ptr [esi + 0ah], 38h
        mov    word ptr [esi + 6], 68h
        jmp    SHORT loc_3_3853
loc_3_381B:
        mov    byte ptr [esi + 0ah], 36h
        mov    word ptr [esi + 6], 66h
        jmp    SHORT loc_3_3853
loc_3_3827:
        mov    byte ptr [esi + 0ah], 32h
        mov    word ptr [esi + 6], 62h
        jmp    SHORT loc_3_3853
loc_3_3833:
        mov    byte ptr [esi + 0ah], 30h
        mov    word ptr [esi + 6], 60h
        jmp    SHORT loc_3_3853
loc_3_383F:
        mov    byte ptr [esi + 0ah], 2eh
        jmp    SHORT loc_3_3853
loc_3_3845:
        mov    ax, word ptr [esi + 8]
        push   ecx
        push   eax
        call   sub_3_1EE1
        mov    byte ptr [esi + 0ah], al
loc_3_3853:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 00Bh
        db  00Bh, 00Bh, 00Bh, 00Bh, 00Bh, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 00Bh, 00Bh, 00Bh
        db  00Bh, 009h, 00Ah
sub_3_38B0:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 8]
        call   sub_3_152B
        ret    4
sub_3_38BF:
        movzx  eax, word ptr [esp + 4]
        db  0A3h, 000h, 000h, 000h, 000h
        ret    4
sub_3_38CC:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    word ptr [ebp - 14h], 2
        mov    cx, word ptr [ebp + 0ch]
        mov    dx, word ptr [ebp + 0eh]
        mov    word ptr [ebp - 10h], cx
        mov    word ptr [ebp - 0eh], dx
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], 0
        movzx  ecx, dx
        shr    edx, 10h
        mov    dword ptr [ebp - 0ch], ecx
        test   dx, 0fffeh
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 8], ecx
        jne    SHORT loc_3_3917
        mov    dword ptr [ebp - 4], 1
loc_3_3917:
        push   1
        lea    ecx, [ebp - 14h]
        push   ecx
        push   eax
        call   sub_3_32BE
        cmp    eax, 1
        je     SHORT loc_3_3939
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_3_3939
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_3_3939
        call   sub_3_111D
loc_3_3939:
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_3_393F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [eax + 1ch], 10h
        je     SHORT loc_3_3964
        mov    eax, dword ptr [esi + 1ch]
        test   al, 40h
        jne    SHORT loc_3_3964
        test   al, 10h
        je     SHORT loc_3_3964
        push   1
        push   esi
        call   sub_3_2F75
        or     byte ptr [esi + 1ch], 40h
loc_3_3964:
        pop    esi
        ret    4
sub_3_3968:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [esp + 8]
        push   edi
        xor    edi, edi
loc_3_3972:
        push   eax
        push   esi
        call   sub_3_153B
        test   eax, eax
        je     SHORT loc_3_399C
        movzx  ecx, word ptr [eax]
        cmp    ecx, 1
        jl     SHORT loc_3_3972
        cmp    ecx, 2
        jle    SHORT loc_3_3999
        cmp    ecx, 4
        je     SHORT loc_3_3999
        cmp    ecx, 8
        je     SHORT loc_3_3999
        cmp    ecx, 10h
        jne    SHORT loc_3_3972
loc_3_3999:
        inc    edi
        jmp    SHORT loc_3_3972
loc_3_399C:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
        db  000h
sub_3_39A4:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   1
        push   1ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_39B6:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_3_3AAD
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     NEAR PTR loc_3_3AAD
        push   eax
        call   sub_3_1E3B
        mov    edi, eax
        mov    dword ptr [esi], eax
        test   edi, edi
        je     NEAR PTR loc_3_3AA2
        mov    eax, dword ptr [ebp + 10h]
        test   eax, eax
        jne    SHORT loc_3_39F0
        mov    dword ptr [esi + 4], 0
        jmp    SHORT loc_3_3A01
loc_3_39F0:
        push   eax
        call   sub_3_1E3B
        mov    dword ptr [esi + 4], eax
        test   eax, eax
        je     NEAR PTR loc_3_3A96
loc_3_3A01:
        mov    eax, dword ptr [ebp + 14h]
        test   eax, eax
        jne    SHORT loc_3_3A11
        mov    dword ptr [esi + 8], 0
        jmp    SHORT loc_3_3A1E
loc_3_3A11:
        push   eax
        call   sub_3_1E3B
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        je     SHORT loc_3_3A84
loc_3_3A1E:
        mov    ecx, dword ptr [ebp + 18h]
        lea    ebx, [esi + 0ch]
        test   ecx, ecx
        jne    SHORT loc_3_3A30
        mov    dword ptr [ebx], 0
        jmp    SHORT loc_3_3A40
loc_3_3A30:
        push   edi
        push   dword ptr [esi + 8]
        push   ecx
        call   sub_3_3D08
        mov    dword ptr [ebx], eax
        test   eax, eax
        je     SHORT loc_3_3A72
loc_3_3A40:
        mov    dword ptr [ebp - 14h], 0
        mov    ecx, dword ptr [ebp - 14h]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_3A54
        xor    eax, eax
loc_3_3A54:
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 10h], eax
        test   eax, eax
        jne    SHORT loc_3_3AB8
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_3_3A72
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3A6F:
        add    esp, 8
loc_3_3A72:
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_3_3A84
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3A81:
        add    esp, 8
loc_3_3A84:
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     SHORT loc_3_3A96
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3A93:
        add    esp, 8
loc_3_3A96:
        push   0
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3A9F:
        add    esp, 8
loc_3_3AA2:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3AAA:
        add    esp, 8
loc_3_3AAD:
        xor    eax, eax
loc_3_3AAF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_3_3AB8:
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   105h
        push   dword ptr [eax + 4]
        call   sub_3_20D0
        mov    dword ptr [ebp - 8], 1
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    edi, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esi + 14h]
        mov    eax, dword ptr [ebp + 1ch]
        mov    edx, dword ptr [esi + 10h]
        mov    dword ptr [ebp - 4], edx
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_3_3B0D
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3B0A:
        add    esp, 8
loc_3_3B0D:
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_3_3B1F
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3B1C:
        add    esp, 8
loc_3_3B1F:
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     SHORT loc_3_3B31
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3B2E:
        add    esp, 8
loc_3_3B31:
        push   0
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3B3A:
        add    esp, 8
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3B45:
        add    esp, 8
        mov    eax, edi
        jmp    NEAR PTR loc_3_3AAF
sub_3_3B4F:
        push   ebp
        movzx  eax, word ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 8
        cmp    eax, 1
        push   esi
        push   edi
        je     SHORT loc_3_3B98
        cmp    eax, 2
        je     SHORT loc_3_3BDF
        cmp    eax, 3
        je     NEAR PTR loc_3_3C46
        cmp    eax, 4
        je     NEAR PTR loc_3_3C88
        cmp    eax, 5
        je     NEAR PTR loc_3_3C9E
        cmp    eax, 6
        je     NEAR PTR loc_3_3CC0
        cmp    eax, 7
        je     NEAR PTR loc_3_3CE1
        jmp    NEAR PTR loc_3_3D00
loc_3_3B98:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        test   edx, edx
        je     SHORT loc_3_3BD8
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp + 14h]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        jmp    NEAR PTR loc_3_3D00
loc_3_3BD8:
        xor    eax, eax
        jmp    NEAR PTR loc_3_3D00
loc_3_3BDF:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 8]
        test   edx, edx
        je     SHORT loc_3_3C3F
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 7fh
        ja     SHORT loc_3_3C0E
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [ecx - 1]
        jmp    SHORT loc_3_3C13
loc_3_3C0E:
        mov    esi, 7fh
loc_3_3C13:
        push   1
        mov    edi, dword ptr [ebp + 14h]
        push   esi
        push   edx
        push   edi
        call   sub_3_1E10
        lea    eax, [edi + esi]
        cmp    esi, 7fh
        jae    SHORT loc_3_3C31
        mov    byte ptr [eax], 0dh
        mov    byte ptr [eax + 1], 0
        jmp    SHORT loc_3_3C38
loc_3_3C31:
        mov    byte ptr [eax - 1], 0dh
        mov    byte ptr [eax], 0
loc_3_3C38:
        mov    eax, esi
        jmp    NEAR PTR loc_3_3D00
loc_3_3C3F:
        xor    eax, eax
        jmp    NEAR PTR loc_3_3D00
loc_3_3C46:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 4]
        test   edx, edx
        je     SHORT loc_3_3C84
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp + 14h]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        jmp    SHORT loc_3_3D00
loc_3_3C84:
        xor    eax, eax
        jmp    SHORT loc_3_3D00
loc_3_3C88:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        test   eax, eax
        jne    SHORT loc_3_3C96
        xor    eax, eax
        jmp    SHORT loc_3_3D00
loc_3_3C96:
        push   eax
        call   sub_3_1F62
        jmp    SHORT loc_3_3D00
loc_3_3C9E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        test   eax, eax
        je     SHORT loc_3_3CB9
        push   1
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        push   eax
        push   dword ptr [ebp + 14h]
        call   sub_3_1E10
        jmp    SHORT loc_3_3CBC
loc_3_3CB9:
        mov    esi, dword ptr [ebp + 10h]
loc_3_3CBC:
        mov    eax, esi
        jmp    SHORT loc_3_3D00
loc_3_3CC0:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [eax + 14h], esi
        mov    dword ptr [eax + 18h], 0
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        jmp    SHORT loc_3_3D00
loc_3_3CE1:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [eax + 14h], esi
        mov    dword ptr [eax + 18h], 1
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_3D00:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_3_3D08:
        sub    esp, 10h
        cmp    dword ptr [esp + 18h], 0
        push   ebx
        push   esi
        push   edi
        push   ebp
        jne    SHORT loc_3_3D1E
        db  0C7h, 044h, 024h, 028h, 000h, 000h, 000h, 000h
loc_3_3D1E:
        mov    ebx, dword ptr [esp + 24h]
        push   ebx
        call   sub_3_1F62
        mov    dword ptr [esp + 14h], eax
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        call   sub_3_1FCA
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_3_3D4C
        mov    dword ptr [esp + 10h], 1
        mov    ebp, dword ptr [esp + 14h]
        dec    ebp
        jmp    SHORT loc_3_3D67
loc_3_3D4C:
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        sub    ebp, ebx
        mov    eax, dword ptr [esp + 14h]
        not    ecx
        sub    eax, ebp
        sub    eax, ecx
        mov    dword ptr [esp + 10h], eax
loc_3_3D67:
        mov    edi, dword ptr [esp + 28h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        mov    edi, dword ptr [esp + 2ch]
        sub    eax, eax
        not    ecx
        mov    dword ptr [esp + 18h], ecx
        mov    ecx, 0ffffffffh
        repne scasb
        mov    eax, dword ptr [esp + 18h]
        push   0
        not    ecx
        dec    ecx
        mov    dword ptr [esp + 20h], ecx
        lea    edx, [eax + ecx + 8]
        mov    ecx, dword ptr [esp + 14h]
        add    ecx, ebp
        add    ecx, edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_3DA6:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_3E23
        mov    edi, eax
        mov    esi, ebx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        lea    edx, [ebp + eax]
        and    ecx, 3
        rep movsb
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, edx
        movsd
        movsd
        add    edx, 8
        mov    ebp, dword ptr [esp + 1ch]
        mov    edi, edx
        mov    esi, dword ptr [esp + 2ch]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    edx, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 18h]
        mov    edi, edx
        mov    esi, dword ptr [esp + 28h]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    edx, dword ptr [esp + 18h]
        mov    ebp, dword ptr [esp + 10h]
        sub    ebx, ebp
        mov    edi, edx
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, ebp
        shr    ecx, 2
        add    esi, ebx
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
loc_3_3E23:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret    0ch
        db  000h, 000h, 000h
sub_3_3E30:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 10h
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [ebp + 18h]
        lea    ecx, [ebp - 10h]
        push   ecx
        mov    dword ptr [ebp - 8], edx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [edx + 0ch]
        call   sub_3_4064
        mov    esp, ebp
        pop    ebp
        ret    14h
sub_3_3E64:
        push   esi
        push   dword ptr [esp + 8]
        call   sub_3_193C
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        push   dword ptr [eax + 0ch]
        call   sub_3_4201
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_3_3E8C
        xor    eax, eax
        mov    dword ptr [esi], 0
loc_3_3E8C:
        pop    esi
        ret    8
sub_3_3E90:
        push   dword ptr [esp + 4]
        call   sub_3_193C
        test   eax, eax
        jne    SHORT loc_3_3EA2
        xor    ax, ax
        jmp    SHORT loc_3_3EAA
loc_3_3EA2:
        push   dword ptr [eax + 0ch]
        call   sub_3_4285
loc_3_3EAA:
        ret    4
sub_3_3EAD:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   10h
        push   10h
        push   10h
        call   sub_3_1260
        mov    edx, eax
        test   edx, edx
        je     NEAR PTR loc_3_3F4F
        mov    dword ptr [ebp - 20h], 10h
        mov    ecx, dword ptr [ebp - 20h]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_3EDC
        xor    eax, eax
loc_3_3EDC:
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [edx + 1ch], eax
        test   eax, eax
        je     SHORT loc_3_3F49
        mov    dword ptr [ebp - 18h], 0
        mov    ecx, dword ptr [ebp - 18h]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_3EFD
        xor    eax, eax
loc_3_3EFD:
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [edx + 20h], eax
        test   eax, eax
        je     SHORT loc_3_3F3A
        mov    dword ptr [ebp - 10h], 0
        mov    ecx, dword ptr [ebp - 10h]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_3F1E
        xor    eax, eax
loc_3_3F1E:
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [edx + 24h], eax
        test   eax, eax
        jne    SHORT loc_3_3F55
        mov    eax, dword ptr [edx + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_3_3F3A:
        mov    eax, dword ptr [edx + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_3_3F49:
        push   edx
        call   sub_3_1576
loc_3_3F4F:
        xor    eax, eax
loc_3_3F51:
        mov    esp, ebp
        pop    ebp
        ret
loc_3_3F55:
        mov    dword ptr [edx + 28h], 0
        mov    eax, edx
        jmp    SHORT loc_3_3F51
sub_3_3F60:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        push   ecx
        call   sub_3_1576
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_3FA3:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 24h
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [edx + 10h]
        cmp    dword ptr [edx + 0ch], eax
        jne    NEAR PTR loc_3_404F
        mov    eax, dword ptr [edx + 28h]
        test   al, 2
        je     SHORT loc_3_401C
        and    al, 0fdh
        mov    ecx, dword ptr [edx + 18h]
        mov    dword ptr [edx + 28h], eax
        mov    dword ptr [ebp - 20h], ecx
        or     al, 10h
        mov    dword ptr [edx + 28h], eax
        or     al, 20h
        mov    dword ptr [edx + 28h], eax
        mov    eax, dword ptr [ebp - 20h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    eax, dword ptr [edx + 20h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    dword ptr [ebp - 14h], 1
        mov    eax, dword ptr [edx + 24h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        jmp    SHORT loc_3_405E
loc_3_401C:
        test   al, 10h
        je     SHORT loc_3_404F
        or     al, 20h
        mov    ecx, dword ptr [edx + 18h]
        mov    dword ptr [edx + 28h], eax
        mov    dword ptr [ebp - 10h], ecx
        mov    eax, dword ptr [ebp - 10h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [edx + 24h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        jmp    SHORT loc_3_405E
loc_3_404F:
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_405E:
        mov    esp, ebp
        pop    ebp
        ret    4
_PTEXT ends
        end
