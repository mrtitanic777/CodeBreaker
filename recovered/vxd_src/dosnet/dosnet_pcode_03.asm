; dosnet_pcode_03.asm -- DOSNET object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

DOSNET_Get_Version:
        mov    eax, 300h
        clc
        ret
DOSNET_Do_PSP_Adjust:
        xor    eax, eax
        ret
DOSNET_Send_FILESYSCHANGE:
        clc
        ret
DOSNET_SYSTEM_EXIT:
        db  0A1h, 020h, 002h, 000h, 000h
        db  0F7h, 025h, 01Ch, 002h, 000h, 000h
        mov    ecx, eax
        db  08Bh, 035h, 010h, 002h, 000h, 000h
        or     esi, esi
        je     SHORT loc_3_002C
        db  08Bh, 03Dh, 018h, 002h, 000h, 000h
        cld
        rep movsb
loc_3_002C:
        clc
        ret
DOSNET_SYS_VM_TERMINATE:
        db  0A1h, 020h, 002h, 000h, 000h
        db  0F7h, 025h, 01Ch, 002h, 000h, 000h
        mov    esi, eax
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_3_005C
        db  0A3h, 010h, 002h, 000h, 000h
        mov    edi, eax
        mov    ecx, esi
        db  08Bh, 035h, 018h, 002h, 000h, 000h
        cld
        rep movsb
loc_3_005C:
        clc
        ret
sub_3_005E:
        push   ecx
        push   1
        push   14h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        stc
        je     SHORT loc_3_0092
        mov    edx, eax
        jecxz  loc_3_0091
        push   edx
        mov    eax, ecx
        db  0F7h, 025h, 01Ch, 002h, 000h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        or     eax, eax
        je     SHORT loc_3_0093
        mov    dword ptr [edx + 0ch], eax
loc_3_0091:
        clc
loc_3_0092:
        ret
loc_3_0093:
        push   0
        push   edx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        stc
        jmp    SHORT loc_3_0092
loc_3_00A1:
        jb     NEAR PTR loc_3_016F
        test   dword ptr [ebp + 2ch], 1
        jne    NEAR PTR loc_3_016F
        mov    eax, dword ptr [edx]
        inc    eax
        je     NEAR PTR loc_3_01F1
        dec    eax
        db  03Bh, 005h, 020h, 002h, 000h, 000h
        jae    NEAR PTR loc_3_016F
        mov    eax, ebx
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        push   eax
        cmp    ebx, eax
        je     NEAR PTR loc_3_016E
        push   edx
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [edx]
        db  0F7h, 025h, 01Ch, 002h, 000h, 000h
        db  003h, 005h, 018h, 002h, 000h, 000h
        add    esi, eax
        pop    edx
        mov    edi, dword ptr [edx + 0ch]
        db  08Bh, 00Dh, 01Ch, 002h, 000h, 000h
        cld
        rep movsb
loc_3_00FC:
        test   dword ptr [ebx], 10h
        jne    SHORT loc_3_0163
        test   dword ptr [ebx], 8
        jne    NEAR PTR loc_3_0195
        db  08Bh, 03Dh, 014h, 002h, 000h, 000h
        add    edi, ebx
        mov    ecx, dword ptr [edx]
        xor    esi, esi
        xchg   dword ptr [edi + ecx*4 + 4], esi
        or     esi, esi
        je     SHORT loc_3_0152
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_3_012F
        dec    dword ptr [esi + 10h]
        jne    SHORT loc_3_0152
loc_3_012F:
        push   eax
        push   edx
        xor    eax, eax
        xchg   dword ptr [esi + 0ch], eax
        or     eax, eax
        je     SHORT loc_3_0145
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_3_0145:
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        pop    eax
loc_3_0152:
        mov    edi, dword ptr [ebx + 4]
        add    edi, eax
        mov    esi, dword ptr [edx + 0ch]
        db  08Bh, 00Dh, 01Ch, 002h, 000h, 000h
        cld
        rep movsb
loc_3_0163:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_3_00FC
loc_3_016E:
        pop    ebx
loc_3_016F:
        cmp    dword ptr [edx + 10h], 0
        jne    SHORT loc_3_0194
        mov    eax, dword ptr [edx + 0ch]
        or     eax, eax
        je     SHORT loc_3_0189
        push   edx
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
loc_3_0189:
        push   0
        push   edx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_3_0194:
        ret
loc_3_0195:
        db  08Bh, 03Dh, 014h, 002h, 000h, 000h
        add    edi, ebx
        mov    ecx, dword ptr [edx]
        mov    esi, edx
        xchg   dword ptr [edi + ecx*4 + 4], esi
        inc    dword ptr [edx + 10h]
        or     esi, esi
        je     SHORT loc_3_01DA
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_3_01B7
        dec    dword ptr [esi + 10h]
        jne    SHORT loc_3_01DA
loc_3_01B7:
        push   eax
        push   edx
        xor    eax, eax
        xchg   dword ptr [esi + 0ch], eax
        or     eax, eax
        je     SHORT loc_3_01CD
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_3_01CD:
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        pop    eax
loc_3_01DA:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_3_0163
        db  0BEh, 045h, 003h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
        mov    dword ptr [edi], esi
        jmp    NEAR PTR loc_3_0163
loc_3_01F1:
        mov    ecx, ebx
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, ecx
        push   ecx
        je     NEAR PTR loc_3_016E
        pop    ecx
        cmp    dword ptr [edx + 4], 0
        je     NEAR PTR loc_3_016F
        mov    esi, dword ptr [ecx + 4]
        db  003h, 035h, 018h, 002h, 000h, 000h
        mov    edi, dword ptr [edx + 0ch]
        db  0A1h, 020h, 002h, 000h, 000h
        push   edx
        db  0F7h, 025h, 01Ch, 002h, 000h, 000h
        pop    edx
        push   ecx
        mov    ecx, eax
        cld
        rep movsb
loc_3_022C:
        db  08Bh, 03Dh, 014h, 002h, 000h, 000h
        add    edi, ebx
        test   dword ptr [ebx], 10h
        jne    NEAR PTR loc_3_02CF
        test   dword ptr [ebx], 8
        jne    NEAR PTR loc_3_02E3
        xor    eax, eax
        db  08Bh, 00Dh, 020h, 002h, 000h, 000h
loc_3_0254:
        bt     dword ptr [edx + 4], eax
        jae    SHORT loc_3_0294
        xor    esi, esi
        xchg   dword ptr [edi + eax*4 + 4], esi
        or     esi, esi
        je     SHORT loc_3_0294
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_3_026F
        dec    dword ptr [esi + 10h]
        jne    SHORT loc_3_0294
loc_3_026F:
        push   eax
        push   edx
        push   ecx
        xor    eax, eax
        xchg   dword ptr [esi + 0ch], eax
        or     eax, eax
        je     SHORT loc_3_0286
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_3_0286:
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        pop    edx
        pop    eax
loc_3_0294:
        inc    eax
        loop   loc_3_0254
        mov    edi, dword ptr [ebx + 4]
        db  003h, 03Dh, 018h, 002h, 000h, 000h
        mov    esi, dword ptr [edx + 0ch]
        xor    eax, eax
        db  08Bh, 00Dh, 020h, 002h, 000h, 000h
loc_3_02AB:
        bt     dword ptr [edx + 4], eax
        jae    SHORT loc_3_02C0
        push   ecx
        push   esi
        push   edi
        db  08Bh, 00Dh, 01Ch, 002h, 000h, 000h
        cld
        rep movsb
        pop    edi
        pop    esi
        pop    ecx
loc_3_02C0:
        inc    eax
        db  003h, 035h, 01Ch, 002h, 000h, 000h
        db  003h, 03Dh, 01Ch, 002h, 000h, 000h
        loop   loc_3_02AB
loc_3_02CF:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    NEAR PTR loc_3_022C
        jmp    NEAR PTR loc_3_016E
loc_3_02E3:
        db  08Bh, 00Dh, 020h, 002h, 000h, 000h
        xor    eax, eax
loc_3_02EB:
        bt     dword ptr [edx + 4], eax
        jae    SHORT loc_3_032E
        mov    esi, edx
        xchg   dword ptr [edi + eax*4 + 4], esi
        inc    dword ptr [edx + 10h]
        or     esi, esi
        je     SHORT loc_3_032E
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_3_0309
        dec    dword ptr [esi + 10h]
        jne    SHORT loc_3_032E
loc_3_0309:
        push   eax
        push   ecx
        push   edx
        xor    eax, eax
        xchg   dword ptr [esi + 0ch], eax
        or     eax, eax
        je     SHORT loc_3_0320
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_3_0320:
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
loc_3_032E:
        inc    eax
        loop   loc_3_02EB
        cmp    dword ptr [edi], 0
        jne    SHORT loc_3_02CF
        db  0BEh, 045h, 003h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
        mov    dword ptr [edi], esi
        jmp    SHORT loc_3_02CF
loc_3_0345:
        db  08Bh, 03Dh, 014h, 002h, 000h, 000h
        add    edi, ebx
        lea    esi, [edi + 4]
        mov    ecx, 1ah
        xor    eax, eax
loc_3_0357:
        xor    edx, edx
        xchg   dword ptr [esi], edx
        or     edx, edx
        je     SHORT loc_3_03BD
        push   esi
        push   edi
        push   ecx
        push   eax
        push   edx
        db  0F7h, 025h, 01Ch, 002h, 000h, 000h
        mov    edi, dword ptr [ebx + 4]
        db  003h, 03Dh, 018h, 002h, 000h, 000h
        add    edi, eax
        pop    edx
        mov    esi, dword ptr [edx + 0ch]
        cmp    dword ptr [edx], -1
        jne    SHORT loc_3_0380
        add    esi, eax
loc_3_0380:
        db  08Bh, 00Dh, 01Ch, 002h, 000h, 000h
        cld
        rep movsb
        pop    eax
        pop    ecx
        pop    edi
        pop    esi
        cmp    dword ptr [edx + 10h], 0
        je     SHORT loc_3_0398
        dec    dword ptr [edx + 10h]
        jne    SHORT loc_3_03BD
loc_3_0398:
        push   eax
        push   ecx
        push   edx
        xor    eax, eax
        xchg   dword ptr [edx + 0ch], eax
        or     eax, eax
        je     SHORT loc_3_03AF
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_3_03AF:
        pop    edx
        push   0
        push   edx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        pop    eax
loc_3_03BD:
        inc    eax
        add    esi, 4
        dec    ecx
        jne    SHORT loc_3_0357
        mov    dword ptr [edi], 0
        ret
_PTEXT ends
        end
