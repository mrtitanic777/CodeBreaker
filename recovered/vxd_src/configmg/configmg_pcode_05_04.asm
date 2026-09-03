; configmg_pcode_05_04.asm -- CONFIGMG object 5 segment _PTEXT  (part 4/4)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_CA6C:
        push   esi
        mov    esi, ecx
        test   esi, esi
        je     SHORT loc_5_CAAF
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_CA8A
        mov    ecx, esi
        call   sub_5_C8A8
        test   eax, eax
        jne    SHORT loc_5_CA8A
        xor    eax, eax
        jmp    SHORT loc_5_CAB4
loc_5_CA8A:
        mov    ecx, esi
        call   sub_5_C869
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_CAAF
loc_5_CA97:
        mov    ecx, esi
        call   sub_5_CA6C
        test   eax, eax
        je     SHORT loc_5_CAB6
        mov    ecx, esi
        call   sub_5_C884
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_5_CA97
loc_5_CAAF:
        mov    eax, 1
loc_5_CAB4:
        pop    esi
        ret
loc_5_CAB6:
        xor    eax, eax
        jmp    SHORT loc_5_CAB4
loc_5_CABA:
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    ecx, esi
        call   sub_5_CA6C
        test   eax, eax
        jne    SHORT loc_5_CAD1
        xor    eax, eax
        jmp    SHORT loc_5_CB03
loc_5_CAD1:
        cmp    esi, dword ptr [esp + 0ch]
        je     SHORT loc_5_CAFE
loc_5_CAD7:
        mov    edi, dword ptr [esi + 20h]
loc_5_CADA:
        mov    ecx, esi
        call   sub_5_C884
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_CAF6
        mov    ecx, esi
        call   sub_5_CA6C
        test   eax, eax
        jne    SHORT loc_5_CADA
        xor    eax, eax
        jmp    SHORT loc_5_CB03
loc_5_CAF6:
        mov    esi, edi
        cmp    edi, dword ptr [esp + 0ch]
        jne    SHORT loc_5_CAD7
loc_5_CAFE:
        mov    eax, 1
loc_5_CB03:
        pop    edi
        pop    esi
        ret
loc_5_CB06:
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
loc_5_CB0E:
        mov    edi, dword ptr [esi + 20h]
        test   esi, esi
        je     SHORT loc_5_CB3B
loc_5_CB15:
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_CB28
        mov    ecx, esi
        call   sub_5_C8A8
        test   eax, eax
        je     SHORT loc_5_CB4E
loc_5_CB28:
        cmp    esi, dword ptr [esp + 0ch]
        je     SHORT loc_5_CB52
        mov    ecx, esi
        call   sub_5_C884
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_5_CB15
loc_5_CB3B:
        cmp    dword ptr [edi + 28h], 0
        mov    esi, edi
        je     SHORT loc_5_CB0E
loc_5_CB43:
        mov    esi, dword ptr [esi + 28h]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_5_CB43
        jmp    SHORT loc_5_CB0E
loc_5_CB4E:
        xor    eax, eax
        jmp    SHORT loc_5_CB57
loc_5_CB52:
        mov    eax, 1
loc_5_CB57:
        pop    edi
        pop    esi
        ret
sub_5_CB5A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], ebx
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        push   edx
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0FFh, 014h, 08Dh, 000h, 000h, 000h, 000h
        add    esp, 4
        mov    edx, dword ptr [ebp + 0ch]
        test   edx, edx
        je     SHORT loc_5_CBBB
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [edx], ecx
loc_5_CBBB:
        mov    ebx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        pop    edi
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        pop    esi
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_5_CBE2:
        push   dword ptr [esp + 4]
        push   0
        push   0
        call   sub_5_CB5A
        ret    4
sub_5_CBF2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        lea    ebx, [ebp - 4]
        push   dword ptr [ebp + 8]
        push   ebx
        push   0
        call   sub_5_CB5A
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_CC25
        lea    ecx, [esi + 4]
        mov    edx, edi
        push   dword ptr [ebp + 0ch]
        push   0
        push   dword ptr [ebp - 4]
        call   sub_5_CB5A
loc_5_CC25:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_CC30:
        push   edx
        cmp    ecx, 1
        sbb    ecx, ecx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    ecx, 3
        call   sub_5_CBE2
        ret
loc_5_CC45:
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 8]
        cmp    ecx, esi
        je     SHORT loc_5_CC75
        push   0
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   ecx
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_1E4B
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        jmp    NEAR PTR loc_5_CD45
loc_5_CC75:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        je     SHORT loc_5_CC89
        cmp    eax, 4
        mov    eax, 0
        jne    SHORT loc_5_CC8E
loc_5_CC89:
        mov    eax, 1
loc_5_CC8E:
        push   eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   ecx
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_1E4B
        test   eax, eax
        jne    NEAR PTR loc_5_CD43
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_5_CCBD
        cmp    eax, 5
        mov    eax, 0
        jne    SHORT loc_5_CCC2
loc_5_CCBD:
        mov    eax, 1
loc_5_CCC2:
        push   eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_1E4B
        test   eax, eax
        jne    SHORT loc_5_CCE6
        mov    eax, 1
        jmp    SHORT loc_5_CD45
loc_5_CCE6:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, -1
        je     SHORT loc_5_CD43
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        je     SHORT loc_5_CD05
        cmp    eax, 4
        mov    eax, 0
        jne    SHORT loc_5_CD0A
loc_5_CD05:
        mov    eax, 1
loc_5_CD0A:
        push   eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_5_1E4B
        test   eax, eax
        je     SHORT loc_5_CD43
        mov    edx, 0bh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_731F
        push   0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   100h
        call   sub_5_1906
loc_5_CD43:
        xor    eax, eax
loc_5_CD45:
        pop    esi
        ret
loc_5_CD47:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        push   ebp
        cmp    esi, edi
        je     SHORT loc_5_CD77
        push   0
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_1E4B
        test   eax, eax
        jne    SHORT loc_5_CDE2
        jmp    NEAR PTR loc_5_CDFC
loc_5_CD77:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        je     SHORT loc_5_CD8B
        mov    ecx, 0
        cmp    eax, 4
        jne    SHORT loc_5_CD90
loc_5_CD8B:
        mov    ecx, 1
loc_5_CD90:
        test   eax, eax
        je     SHORT loc_5_CD9E
        mov    ebp, 0
        cmp    eax, 5
        jne    SHORT loc_5_CDA3
loc_5_CD9E:
        mov    ebp, 1
loc_5_CDA3:
        push   ecx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_1E4B
        push   ebp
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   edi
        cmp    eax, 1
        sbb    ebx, ebx
        call   sub_5_1E4B
        neg    ebx
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        test   eax, ebx
        mov    eax, 1
        jne    SHORT loc_5_CE01
loc_5_CDE2:
        mov    edx, 0bh
        mov    ecx, edi
        call   sub_5_731F
        push   0
        mov    ecx, edi
        push   100h
        call   sub_5_1906
loc_5_CDFC:
        mov    eax, 1
loc_5_CE01:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_CE06:
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 8]
        cmp    esi, ecx
        je     SHORT loc_5_CE32
        push   0
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   ecx
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_1E4B
        test   eax, eax
        jne    SHORT loc_5_CEA1
        xor    eax, eax
        jmp    SHORT loc_5_CEA6
loc_5_CE32:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        je     SHORT loc_5_CE46
        cmp    eax, 4
        mov    eax, 0
        jne    SHORT loc_5_CE4B
loc_5_CE46:
        mov    eax, 1
loc_5_CE4B:
        push   eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   ecx
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_1E4B
        test   eax, eax
        jne    SHORT loc_5_CE67
        xor    eax, eax
        jmp    SHORT loc_5_CEA6
loc_5_CE67:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_5_CE7A
        cmp    eax, 5
        mov    eax, 0
        jne    SHORT loc_5_CE7F
loc_5_CE7A:
        mov    eax, 1
loc_5_CE7F:
        push   eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_1E4B
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_5_CEA6
loc_5_CEA1:
        mov    eax, 1
loc_5_CEA6:
        pop    esi
        ret
sub_5_CEA8:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        mov    dword ptr [ebp - 4], edx
        push   esi
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 0ch], edx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 10h], eax
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 14h]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp + 18h], 0
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 000h, 000h, 000h, 000h
        db  0B8h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_CF1F
        db  0B8h, 000h, 000h, 000h, 000h
loc_5_CF1F:
        push   eax
        mov    edx, dword ptr [ebp - 4]
        lea    eax, [ebp - 18h]
        push   eax
        push   0
        call   sub_5_CB5A
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_5_CF6B
        cmp    dword ptr [ebp + 10h], -1
        je     SHORT loc_5_CF6B
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        db  068h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 4]
        push   0
        db  0A3h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 18h]
        db  083h, 005h, 000h, 000h, 000h, 000h, 004h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_CB5A
loc_5_CF6B:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 8]
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
sub_5_CFA3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        db  068h, 000h, 000h, 000h, 000h
        push   0
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        push   0
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_CB5A
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        pop    edi
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        pop    esi
        db  089h, 015h, 000h, 000h, 000h, 000h
        pop    ebx
        cmp    eax, 1
        sbb    eax, eax
        mov    esp, ebp
        neg    eax
        pop    ebp
        ret    8
        db  0CCh, 0CCh
sub_5_D024:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_D034:
        db  0CDh, 020h, 055h, 080h, 001h, 000h
        int3
        int3
sub_5_D03C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_5_D050:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_D060:
        db  0CDh, 020h, 08Ch, 081h, 001h, 000h
        int3
        int3
sub_5_D068:
        db  0CDh, 020h, 053h, 080h, 001h, 000h
        int3
        int3
sub_5_D070:
        pop    edx
        pop    eax
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        jmp    edx
        db  0CCh, 0CCh
sub_5_D07C:
        db  0CDh, 020h, 010h, 080h, 017h, 000h
        int3
        int3
sub_5_D084:
        pop    edx
        pop    eax
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        jmp    edx
        db  0CCh, 0CCh
sub_5_D090:
        pop    edx
        pop    eax
        pop    ecx
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        jmp    edx
        db  0CCh
sub_5_D09C:
        pop    edx
        pop    ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_5_D0AC:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  0CCh
sub_5_D0B8:
        db  0CDh, 020h, 00Dh, 080h, 017h, 000h
        int3
        int3
sub_5_D0C0:
        db  0CDh, 020h, 050h, 080h, 001h, 000h
        int3
        int3
sub_5_D0C8:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_5_D0D0:
        pop    edx
        pop    esi
        pop    eax
        db  0CDh, 020h, 0BEh, 080h, 001h, 000h
        int3
        int3
        int3
sub_5_D0DC:
        db  0CDh, 020h, 055h, 081h, 001h, 000h
        int3
        int3
sub_5_D0E4:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_5_D0EC:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        int3
        int3
sub_5_D0F4:
        db  0CDh, 020h, 06Fh, 081h, 001h, 000h
        int3
        int3
sub_5_D0FC:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_5_D104:
        db  0CDh, 020h, 018h, 080h, 017h, 000h
        int3
        int3
sub_5_D10C:
        db  0CDh, 020h, 017h, 080h, 017h, 000h
        int3
        int3
sub_5_D114:
        db  0CDh, 020h, 015h, 080h, 017h, 000h
        int3
        int3
sub_5_D11C:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_5_D124:
        db  0CDh, 020h, 016h, 080h, 017h, 000h
        int3
        int3
sub_5_D12C:
        db  0CDh, 020h, 073h, 081h, 001h, 000h
        int3
        int3
sub_5_D134:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_5_D13C:
        db  0CDh, 020h, 096h, 081h, 001h, 000h
        int3
        int3
sub_5_D144:
        db  0CDh, 020h, 076h, 081h, 001h, 000h
        int3
        int3
sub_5_D14C:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        int3
        int3
sub_5_D154:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_5_D15C:
        db  0CDh, 020h, 071h, 081h, 001h, 000h
        int3
        int3
sub_5_D164:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        int3
        int3
sub_5_D16C:
        db  0CDh, 020h, 04Ch, 081h, 001h, 000h
        int3
        int3
sub_5_D174:
        db  0CDh, 020h, 079h, 081h, 001h, 000h
        int3
        int3
sub_5_D17C:
        db  0CDh, 020h, 005h, 080h, 027h, 000h
        int3
        int3
sub_5_D184:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 24h]
        mov    esi, dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 10h]
        db  0CDh, 020h, 047h, 001h, 001h, 000h
        sbb    ebx, ebx
        and    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
        db  0CCh
sub_5_D1B0:
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        movzx  eax, word ptr [esp + 8]
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        mov    eax, ecx
        pop    edi
        ret    8
        db  0CCh, 0CCh
sub_5_D1C8:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_5_D1E6
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_D1E6:
        ret    10h
        db  0CCh, 0CCh, 0CCh
sub_5_D1EC:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_5_D200:
        db  0CDh, 020h, 014h, 080h, 017h, 000h
        int3
        int3
sub_5_D208:
        db  0CDh, 020h, 013h, 080h, 017h, 000h
        int3
        int3
sub_5_D210:
        push   ebx
        push   edi
        mov    ebx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 10h]
        or     eax, 1000h
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        pop    edi
        pop    ebx
        ret    10h
        db  0CCh, 0CCh
sub_5_D234:
        db  0CDh, 020h, 050h, 081h, 001h, 000h
        int3
        int3
sub_5_D23C:
        db  0CDh, 020h, 00Bh, 080h, 026h, 000h
_PTEXT ends
        end
