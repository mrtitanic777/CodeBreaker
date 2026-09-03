; v86mmgr_pcode_12.asm -- V86MMGR object 12 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  060h, 033h, 0C9h, 087h, 04Ch, 01Eh, 010h, 0E3h, 00Bh, 06Ah, 000h, 051h, 0CDh, 020h, 055h, 000h
        db  001h, 000h, 059h, 059h, 061h, 0C3h, 000h, 000h
sub_12_0018:
        pushad
        xor    eax, eax
        mov    dword ptr [esi + ebx + 48h], eax
        mov    dword ptr [esi + ebx + 4ch], eax
        xchg   dword ptr [esi + ebx + 44h], eax
        or     eax, eax
        je     SHORT loc_12_0036
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_12_0036:
        xor    al, al
        xchg   byte ptr [esi + ebx + 27h], al
        or     al, al
        je     SHORT loc_12_0052
        db  08Ah, 025h, 0EAh, 002h, 000h, 000h
        cmp    al, ah
        ja     SHORT loc_12_004C
        mov    ah, al
loc_12_004C:
        db  028h, 025h, 0EAh, 002h, 000h, 000h
loc_12_0052:
        xor    edi, edi
        xchg   dword ptr [esi + ebx + 5ch], edi
        or     edi, edi
        je     NEAR PTR loc_12_00E8
        mov    ecx, dword ptr [edi + 67ch]
        db  029h, 00Dh, 00Ch, 000h, 000h, 000h
        jae    SHORT loc_12_0078
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_12_0078:
        mov    ecx, 0ffh
        lea    esi, [edi + 280h]
loc_12_0083:
        cld
        lodsd
        or     eax, eax
        je     SHORT loc_12_00BB
        cmp    ecx, 0ffh
        je     SHORT loc_12_00AE
        test   byte ptr [eax + 2ch], 80h
        jne    SHORT loc_12_00BB
        test   byte ptr [eax + 2ch], 4
        jne    SHORT loc_12_00AE
        push   ecx
        push   eax
        mov    eax, dword ptr [eax]
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    eax
        pop    ecx
loc_12_00AE:
        push   ecx
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
loc_12_00BB:
        loop   loc_12_0083
        mov    ecx, 0fh
        lea    esi, [edi + 204h]
loc_12_00C8:
        cld
        lodsd
        or     eax, eax
        je     SHORT loc_12_00DB
        push   ecx
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
loc_12_00DB:
        loop   loc_12_00C8
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_12_00E8:
        popad
        ret
        db  000h, 000h
V86MMGR_END_PM_APP:
        db  0A1h, 0FCh, 011h, 000h, 000h
        xor    ecx, ecx
        mov    word ptr [eax + ebx + 24h], cx
        ret
loc_12_00F9:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        call   0
        call   sub_12_0018
        call   sub_12_0170
        xor    eax, eax
        xchg   dword ptr [esi + ebx + 4], eax
        or     eax, eax
        je     SHORT loc_12_0123
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_12_0123:
        xor    eax, eax
        xchg   dword ptr [esi + ebx + 8], eax
        or     eax, eax
        je     SHORT loc_12_0138
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_12_0138:
        xor    eax, eax
        xchg   dword ptr [esi + ebx], eax
        or     eax, eax
        je     SHORT loc_12_016B
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        push   0
        push   0
        push   dword ptr [esi + ebx + 0ch]
        push   eax
        push   ebx
        db  0FFh, 035h, 000h, 012h, 000h, 000h
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
loc_12_016B:
        clc
        ret
        db  000h, 000h, 000h
sub_12_0170:
        pushad
        xor    ecx, ecx
        mov    dword ptr [esi + ebx + 34h], ecx
        mov    dword ptr [esi + ebx + 38h], ecx
        xchg   dword ptr [esi + ebx + 30h], ecx
        jecxz  loc_12_018C
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_12_018C:
        xor    edi, edi
        xchg   dword ptr [esi + ebx + 60h], edi
        or     edi, edi
        je     NEAR PTR loc_12_022F
        btr    word ptr [edi + 14h], 1
        jae    SHORT loc_12_01AE
        push   0
        push   dword ptr [edi]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_12_01AE:
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 020h
        je     SHORT loc_12_01C2
        push   4
        push   ebx
        db  0CDh, 020h, 07Dh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_12_01C2:
        lea    esi, [edi + 1ch]
        db  08Bh, 00Dh, 0ACh, 012h, 000h, 000h
        jecxz  loc_12_01F5
loc_12_01CD:
        cld
        lodsd
        or     eax, eax
        jns    SHORT loc_12_01F3
        push   ecx
        cmp    dword ptr [eax], 0
        je     SHORT loc_12_01E7
        push   eax
        push   0
        push   dword ptr [eax]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    eax
loc_12_01E7:
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
loc_12_01F3:
        loop   loc_12_01CD
loc_12_01F5:
        xor    ecx, ecx
        xchg   dword ptr [edi + 18h], ecx
        jecxz  loc_12_0207
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_12_0207:
        mov    esi, dword ptr [edi + 0ch]
loc_12_020A:
        cmp    word ptr [esi + 4], -1
        je     SHORT loc_12_0221
        push   dword ptr [esi]
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    esi
        jmp    SHORT loc_12_020A
loc_12_0221:
        mov    eax, edi
        db  08Bh, 035h, 0B0h, 012h, 000h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_12_022F:
        popad
        ret
        db  000h, 000h, 000h
_PTEXT ends
        end
