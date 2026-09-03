; ifsmgr_lcode_01_02.asm -- IFSMGR object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4B67:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 7ch]
        cmp    byte ptr [edi + 30h], 1
        jbe    SHORT loc_1_4BA9
        push   esi
        call   sub_1_446F
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_4BA9
        cmp    dword ptr [ebp + 14h], 0
        jne    SHORT loc_1_4BA2
        cmp    dword ptr [ebp + 10h], 0
        jne    SHORT loc_1_4BA2
        mov    word ptr [esi + 1ah], 5
        mov    eax, 5
        jmp    NEAR PTR loc_1_4C71
loc_1_4BA2:
        sub    eax, eax
        jmp    NEAR PTR loc_1_4C71
loc_1_4BA9:
        mov    ebx, dword ptr [ebp + 14h]
        cmp    ebx, 5
        ja     NEAR PTR loc_1_4C6C
        db  0FFh, 024h, 09Dh, 076h, 04Ch, 000h, 000h
loc_1_4BBC:
        test   byte ptr [edi + 32h], 0c0h
        je     SHORT loc_1_4BD9
        mov    eax, dword ptr [edi + 34h]
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_1_4BD9
        cmp    dword ptr [ebp + 10h], 0
        jge    SHORT loc_1_4BD9
loc_1_4BD0:
        push   esi
        call   sub_1_4B40
        add    esp, 4
loc_1_4BD9:
        or     ebx, ebx
        push   dword ptr [ebp + 10h]
        push   esi
        jne    SHORT loc_1_4BE8
        db  0E8h, 0EAh, 0FFh, 0FFh, 0FFh
        jmp    SHORT loc_1_4BED
loc_1_4BE8:
        db  0E8h, 010h, 00Ah, 000h, 000h
loc_1_4BED:
        add    esp, 8
        mov    ebx, eax
        test   byte ptr [edi + 32h], 0c0h
        je     SHORT loc_1_4C17
        mov    eax, dword ptr [edi + 34h]
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_1_4C17
        cmp    dword ptr [ebp + 10h], 24h
        jne    SHORT loc_1_4C08
        jmp    SHORT loc_1_4C0E
loc_1_4C08:
        cmp    dword ptr [ebp + 10h], 0
        jge    SHORT loc_1_4C17
loc_1_4C0E:
        push   esi
        call   sub_1_4B40
        add    esp, 4
loc_1_4C17:
        mov    eax, ebx
        jmp    SHORT loc_1_4C71
loc_1_4C1B:
        push   dword ptr [ebp + 0ch]
        push   esi
        db  0E8h, 081h, 006h, 000h, 000h
        add    esp, 8
        jmp    SHORT loc_1_4C6C
loc_1_4C29:
        push   dword ptr [ebp + 0ch]
        push   esi
        db  0E8h, 046h, 006h, 000h, 000h
        add    esp, 8
        jmp    SHORT loc_1_4C6C
loc_1_4C37:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   esi
        db  0E8h, 05Dh, 015h, 000h, 000h
        add    esp, 0ch
        jmp    SHORT loc_1_4C6C
loc_1_4C48:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   esi
        db  0E8h, 098h, 016h, 000h, 000h
        add    esp, 0ch
        test   byte ptr [edi + 32h], 0c0h
        je     SHORT loc_1_4C6C
        cmp    dword ptr [ebp + 10h], 24h
        jne    SHORT loc_1_4C6C
        push   esi
        call   sub_1_4B40
        add    esp, 4
loc_1_4C6C:
        mov    eax, 0ffffffffh
loc_1_4C71:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  0BCh, 04Bh, 000h, 000h, 0BCh, 04Bh, 000h, 000h, 01Bh, 04Ch, 000h, 000h, 029h, 04Ch, 000h, 000h
        db  037h, 04Ch, 000h, 000h, 048h, 04Ch, 000h, 000h, 000h, 000h
sub_1_4C90:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 0ch]
        mov    dx, cx
        and    edx, 7
        mov    eax, 2
        test   ch, 80h
        jne    SHORT loc_1_4CB6
        and    ecx, 70h
        mov    eax, ecx
        cmp    eax, 70h
        jne    SHORT loc_1_4CB6
        sub    eax, eax
loc_1_4CB6:
        and    eax, 0fffffff3h
        shr    eax, 2
        pop    ebp
        db  08Bh, 080h, 024h, 074h, 000h, 000h
        or     eax, edx
        ret
IFSMgr_CheckAccessConflict:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_4C90
        add    esp, 4
        mov    edi, eax
        cmp    edi, -1
        jne    SHORT loc_1_4CE9
        mov    eax, 20h
        jmp    SHORT loc_1_4D5A
loc_1_4CE9:
        db  0C7h, 045h, 0FCh, 014h, 074h, 000h, 000h
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_1_4CFF
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_4D05
loc_1_4CFF:
        test   byte ptr [esi + 0eh], 1
        je     SHORT loc_1_4D0C
loc_1_4D05:
        db  0C7h, 045h, 0FCh, 01Ch, 074h, 000h, 000h
loc_1_4D0C:
        push   dword ptr [ebp + 14h]
        lea    eax, [ebp - 14h]
        push   eax
        call   dword ptr [ebp + 10h]
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_4D58
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_4C90
        add    esp, 4
        cmp    eax, -1
        je     SHORT loc_1_4D51
        mov    ecx, dword ptr [esi]
        sub    ecx, dword ptr [ebp - 14h]
        cmp    ecx, 1
        sbb    edx, edx
        neg    edx
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, 1
        mov    ecx, dword ptr [ecx + edx*4]
        movzx  edx, word ptr [ecx + edi*2]
        mov    cl, al
        shl    ebx, cl
        test   ebx, edx
        je     SHORT loc_1_4D0C
loc_1_4D51:
        mov    eax, 20h
        jmp    SHORT loc_1_4D5A
loc_1_4D58:
        sub    eax, eax
loc_1_4D5A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_4D61:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        db  0A1h, 090h, 073h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_4D80
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 090h, 073h, 000h, 000h
        db  0FFh, 00Dh, 08Ch, 073h, 000h, 000h
        jmp    SHORT loc_1_4DA9
loc_1_4D80:
        mov    dword ptr [ebp - 0ch], 1
        mov    dword ptr [ebp - 8], 1ch
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
loc_1_4DA9:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_4DAD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        db  083h, 03Dh, 08Ch, 073h, 000h, 000h, 020h
        jae    SHORT loc_1_4DD4
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 090h, 073h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 090h, 073h, 000h, 000h
        db  0FFh, 005h, 08Ch, 073h, 000h, 000h
        jmp    SHORT loc_1_4E02
loc_1_4DD4:
        db  083h, 03Dh, 074h, 059h, 000h, 000h, 000h
        jne    SHORT loc_1_4E02
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
loc_1_4E02:
        mov    esp, ebp
        pop    ebp
        ret
IFSMgr_LockFile:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_4FE9
        cmp    dword ptr [ebp + 18h], 0
        je     NEAR PTR loc_1_4FE9
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     NEAR PTR loc_1_4FE9
        mov    ebx, dword ptr [ebp + 0ch]
        lea    ecx, [eax + ebx - 1]
        mov    dword ptr [ebp - 4], ecx
        cmp    ecx, ebx
        jb     NEAR PTR loc_1_4FE9
        test   dword ptr [ebp + 1ch], 0fffffffch
        jne    NEAR PTR loc_1_4FE9
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx]
        or     esi, esi
        jne    SHORT loc_1_4E85
        call   sub_1_4D61
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_4E6B
        mov    eax, 24h
        jmp    NEAR PTR loc_1_4FEE
loc_1_4E6B:
        mov    dword ptr [esi + 8], 0ffffffffh
        mov    dword ptr [esi + 0ch], 0
        mov    dword ptr [esi + 10h], 0
        mov    dword ptr [esi], esi
        mov    dword ptr [esi + 4], esi
loc_1_4E85:
        call   sub_1_4D61
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    SHORT loc_1_4EA8
        cmp    dword ptr [esi], esi
        jne    SHORT loc_1_4E9E
        push   esi
        call   sub_1_4DAD
        add    esp, 4
loc_1_4E9E:
        mov    eax, 24h
        jmp    NEAR PTR loc_1_4FEE
loc_1_4EA8:
        mov    edi, 2
        mov    eax, dword ptr [esi + 10h]
        or     eax, dword ptr [ebp + 1ch]
        test   edi, eax
        jne    SHORT loc_1_4EE7
        mov    edx, dword ptr [esi]
        cmp    edx, esi
        je     NEAR PTR loc_1_4F6A
loc_1_4EC1:
        cmp    dword ptr [edx + 0ch], ebx
        jae    SHORT loc_1_4ED1
        mov    edx, dword ptr [edx]
        cmp    edx, esi
        jne    SHORT loc_1_4EC1
        jmp    NEAR PTR loc_1_4F6A
loc_1_4ED1:
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [edx + 8], ecx
        ja     NEAR PTR loc_1_4F6A
        mov    edi, 3
        jmp    NEAR PTR loc_1_4F6A
loc_1_4EE7:
        mov    dword ptr [ebp - 0ch], 0
        mov    edx, dword ptr [esi]
        cmp    edx, esi
        je     SHORT loc_1_4F61
loc_1_4EF4:
        cmp    edi, 2
        jne    SHORT loc_1_4F01
        cmp    dword ptr [edx + 8], ebx
        jb     SHORT loc_1_4F01
        mov    dword ptr [ebp - 0ch], edx
loc_1_4F01:
        mov    eax, dword ptr [edx + 8]
        cmp    dword ptr [ebp - 4], eax
        jb     SHORT loc_1_4F61
        cmp    ebx, dword ptr [edx + 0ch]
        ja     SHORT loc_1_4F50
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, dword ptr [edx + 10h]
        test   al, 2
        je     SHORT loc_1_4F5C
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [edx + 14h], ecx
        jne    SHORT loc_1_4F4C
        mov    ecx, dword ptr [ebp + 18h]
        cmp    dword ptr [edx + 18h], ecx
        jne    SHORT loc_1_4F4C
        cmp    ebx, dword ptr [edx + 8]
        jne    SHORT loc_1_4F50
        mov    eax, dword ptr [edx + 0ch]
        cmp    dword ptr [ebp - 4], eax
        jne    SHORT loc_1_4F50
        mov    eax, dword ptr [ebp + 1ch]
        xor    eax, dword ptr [edx + 10h]
        test   al, 1
        je     SHORT loc_1_4F58
        mov    edi, 1
        test   byte ptr [ebp + 1ch], 1
        je     SHORT loc_1_4F50
        jmp    SHORT loc_1_4F61
loc_1_4F4C:
        test   al, 1
        je     SHORT loc_1_4F5C
loc_1_4F50:
        mov    edx, dword ptr [edx]
        cmp    edx, esi
        jne    SHORT loc_1_4EF4
        jmp    SHORT loc_1_4F61
loc_1_4F58:
        sub    edi, edi
        jmp    SHORT loc_1_4F61
loc_1_4F5C:
        mov    edi, 3
loc_1_4F61:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_4F6A
        mov    edx, dword ptr [ebp - 0ch]
loc_1_4F6A:
        sub    eax, eax
        cmp    edi, eax
        je     SHORT loc_1_4FE2
        cmp    edi, 1
        je     SHORT loc_1_4F81
        cmp    edi, 2
        je     SHORT loc_1_4F89
        cmp    edi, 3
        je     SHORT loc_1_4FD2
        jmp    SHORT loc_1_4FDD
loc_1_4F81:
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [edx + 10h], ecx
        jmp    SHORT loc_1_4FE2
loc_1_4F89:
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ebx
        mov    dword ptr [edi + 0ch], ecx
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 14h], ecx
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [edi + 18h], ecx
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [edi + 10h], ecx
        mov    dword ptr [edi], edx
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [edi + 4], ecx
        mov    dword ptr [ecx], edi
        mov    dword ptr [edx + 4], edi
        cmp    dword ptr [esi + 8], ebx
        jbe    SHORT loc_1_4FBC
        mov    dword ptr [esi + 8], ebx
loc_1_4FBC:
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0ch], ecx
        jae    SHORT loc_1_4FC7
        mov    dword ptr [esi + 0ch], ecx
loc_1_4FC7:
        mov    ecx, dword ptr [ebp + 1ch]
        and    ecx, 2
        or     dword ptr [esi + 10h], ecx
        jmp    SHORT loc_1_4FE2
loc_1_4FD2:
        push   dword ptr [ebp - 8]
        call   sub_1_4DAD
        add    esp, 4
loc_1_4FDD:
        mov    eax, 21h
loc_1_4FE2:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], esi
        jmp    SHORT loc_1_4FEE
loc_1_4FE9:
        mov    eax, 21h
loc_1_4FEE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
IFSMgr_UnlockFile:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_51AC
        cmp    dword ptr [ebp + 18h], 0
        je     NEAR PTR loc_1_51AC
        mov    ecx, dword ptr [ebp + 10h]
        or     ecx, ecx
        je     NEAR PTR loc_1_51AC
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     NEAR PTR loc_1_51AC
        mov    eax, dword ptr [ebp + 0ch]
        lea    edx, [ecx + eax - 1]
        cmp    eax, edx
        ja     NEAR PTR loc_1_51AC
        test   dword ptr [ebp + 1ch], 0fffffffch
        jne    NEAR PTR loc_1_51AC
        mov    dword ptr [ebp - 10h], 21h
        lea    ecx, [esi + 10h]
        mov    dword ptr [ebp - 14h], ecx
        mov    ecx, dword ptr [ecx]
        or     ecx, dword ptr [ebp + 1ch]
        test   cl, 2
        jne    NEAR PTR loc_1_50EC
        mov    edi, dword ptr [esi]
        cmp    edi, esi
        je     SHORT loc_1_5072
loc_1_5067:
        cmp    dword ptr [edi + 0ch], eax
        jae    SHORT loc_1_5072
        mov    edi, dword ptr [edi]
        cmp    edi, esi
        jne    SHORT loc_1_5067
loc_1_5072:
        cmp    dword ptr [edi + 8], eax
        jne    NEAR PTR loc_1_51A2
        cmp    dword ptr [edi + 0ch], edx
        jne    NEAR PTR loc_1_51A2
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [edi + 14h], ecx
        jne    NEAR PTR loc_1_51A2
        mov    ecx, dword ptr [ebp + 18h]
        cmp    dword ptr [edi + 18h], ecx
        jne    NEAR PTR loc_1_51A2
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        push   edi
        call   sub_1_4DAD
        add    esp, 4
        cmp    dword ptr [esi], esi
        jne    SHORT loc_1_50C5
        push   esi
        call   sub_1_4DAD
        add    esp, 4
        sub    esi, esi
        jmp    SHORT loc_1_50E0
loc_1_50C5:
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [esi + 0ch], eax
        jbe    SHORT loc_1_50D3
        mov    dword ptr [esi + 0ch], eax
loc_1_50D3:
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 8]
        cmp    dword ptr [esi + 8], eax
        jae    SHORT loc_1_50E0
        mov    dword ptr [esi + 8], eax
loc_1_50E0:
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_51A2
loc_1_50EC:
        mov    dword ptr [ebp - 8], 0ffffffffh
        mov    dword ptr [ebp - 4], 0
        sub    edi, edi
        mov    dword ptr [ebp - 0ch], edi
        mov    ebx, dword ptr [esi]
        cmp    ebx, esi
        je     SHORT loc_1_515A
loc_1_5105:
        cmp    eax, dword ptr [ebx + 8]
        jne    SHORT loc_1_5136
        cmp    dword ptr [ebx + 0ch], edx
        jne    SHORT loc_1_5136
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [ebx + 14h], ecx
        jne    SHORT loc_1_5136
        mov    ecx, dword ptr [ebp + 18h]
        cmp    dword ptr [ebx + 18h], ecx
        jne    SHORT loc_1_5136
        mov    ecx, dword ptr [ebx + 10h]
        xor    ecx, dword ptr [ebp + 1ch]
        test   cl, 3
        jne    SHORT loc_1_5136
        mov    dword ptr [ebp - 0ch], ebx
        mov    dword ptr [ebp - 10h], 0
        jmp    SHORT loc_1_5154
loc_1_5136:
        mov    ecx, dword ptr [ebx + 8]
        cmp    dword ptr [ebp - 8], ecx
        jbe    SHORT loc_1_5141
        mov    dword ptr [ebp - 8], ecx
loc_1_5141:
        mov    ecx, dword ptr [ebx + 0ch]
        cmp    dword ptr [ebp - 4], ecx
        jae    SHORT loc_1_514C
        mov    dword ptr [ebp - 4], ecx
loc_1_514C:
        mov    ecx, dword ptr [ebx + 10h]
        and    ecx, 2
        or     edi, ecx
loc_1_5154:
        mov    ebx, dword ptr [ebx]
        cmp    ebx, esi
        jne    SHORT loc_1_5105
loc_1_515A:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_51A2
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        push   dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        call   sub_1_4DAD
        add    esp, 4
        cmp    dword ptr [esi], esi
        jne    SHORT loc_1_5191
        push   esi
        call   sub_1_4DAD
        add    esp, 4
        sub    esi, esi
        jmp    SHORT loc_1_51A2
loc_1_5191:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [ecx], edi
loc_1_51A2:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp - 10h]
        jmp    SHORT loc_1_51B1
loc_1_51AC:
        mov    eax, 21h
loc_1_51B1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
IFSMgr_RemoveLocks:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     SHORT loc_1_5233
        mov    ecx, dword ptr [esi]
        cmp    ecx, esi
        je     SHORT loc_1_520C
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 10h]
loc_1_51D6:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 4], eax
        or     ebx, ebx
        je     SHORT loc_1_51E4
        cmp    dword ptr [ecx + 18h], ebx
        jne    SHORT loc_1_5205
loc_1_51E4:
        or     edi, edi
        je     SHORT loc_1_51ED
        cmp    dword ptr [ecx + 14h], edi
        jne    SHORT loc_1_5205
loc_1_51ED:
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax], edx
        push   ecx
        call   sub_1_4DAD
        add    esp, 4
loc_1_5205:
        mov    ecx, dword ptr [ebp - 4]
        cmp    ecx, esi
        jne    SHORT loc_1_51D6
loc_1_520C:
        cmp    dword ptr [esi], esi
        jne    SHORT loc_1_521D
        push   esi
        call   sub_1_4DAD
        add    esp, 4
        sub    esi, esi
        jmp    SHORT loc_1_522E
loc_1_521D:
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 8], eax
loc_1_522E:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], esi
loc_1_5233:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
IFSMgr_CheckLocks:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        jne    SHORT loc_1_524B
        sub    eax, eax
        jmp    SHORT loc_1_529F
loc_1_524B:
        mov    edi, dword ptr [ebp + 0ch]
        lea    esi, [eax + edi - 1]
        cmp    edi, esi
        jbe    SHORT loc_1_5258
        jmp    SHORT loc_1_529A
loc_1_5258:
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, eax
        cmp    dword ptr [ecx + 0ch], edi
        jb     SHORT loc_1_529F
        cmp    dword ptr [ecx + 8], esi
        ja     SHORT loc_1_529F
        mov    ebx, dword ptr [ecx]
        cmp    ecx, ebx
        je     SHORT loc_1_529F
loc_1_526D:
        cmp    dword ptr [ebx + 8], esi
        ja     SHORT loc_1_529F
        cmp    dword ptr [ebx + 0ch], edi
        jb     SHORT loc_1_5292
        mov    edx, dword ptr [ebp + 14h]
        cmp    dword ptr [ebx + 14h], edx
        jne    SHORT loc_1_5287
        mov    edx, dword ptr [ebp + 18h]
        cmp    dword ptr [ebx + 18h], edx
        je     SHORT loc_1_5292
loc_1_5287:
        mov    edx, dword ptr [ebx + 10h]
        and    edx, dword ptr [ebp + 1ch]
        test   dl, 1
        je     SHORT loc_1_529A
loc_1_5292:
        mov    ebx, dword ptr [ebx]
        cmp    ecx, ebx
        jne    SHORT loc_1_526D
        jmp    SHORT loc_1_529F
loc_1_529A:
        mov    eax, 21h
loc_1_529F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
IFSMgr_CountLocks:
        push   ebp
loc_1_52A5:
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        or     ecx, ecx
        jne    SHORT loc_1_52B3
        sub    eax, eax
        jmp    SHORT loc_1_52CA
loc_1_52B3:
        sub    eax, eax
        mov    esi, dword ptr [ecx]
        cmp    esi, ecx
        je     SHORT loc_1_52CA
        mov    edx, dword ptr [ebp + 0ch]
loc_1_52BE:
        cmp    dword ptr [esi + 18h], edx
        jne    SHORT loc_1_52C4
        inc    eax
loc_1_52C4:
        mov    esi, dword ptr [esi]
        cmp    esi, ecx
        jne    SHORT loc_1_52BE
loc_1_52CA:
        pop    esi
        pop    ebp
        ret
IFSMgr_ReassignLockFileInst:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        jne    SHORT loc_1_52DB
        jmp    SHORT loc_1_5306
loc_1_52DB:
        mov    esi, dword ptr [eax]
        cmp    eax, esi
        je     SHORT loc_1_5306
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
loc_1_52E7:
        mov    ebx, dword ptr [esi + 10h]
        test   bl, 80h
        je     SHORT loc_1_5300
        cmp    dword ptr [esi + 18h], ecx
        jne    SHORT loc_1_5300
        and    ebx, 0ffffff7fh
        mov    dword ptr [esi + 10h], ebx
        mov    dword ptr [esi + 18h], edx
loc_1_5300:
        mov    esi, dword ptr [esi]
        cmp    eax, esi
        jne    SHORT loc_1_52E7
loc_1_5306:
        sub    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret
IFSMgr_UnassignLockList:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        jne    SHORT loc_1_5318
        jmp    SHORT loc_1_532B
loc_1_5318:
        mov    ecx, dword ptr [eax]
        cmp    ecx, eax
        je     SHORT loc_1_532B
loc_1_531E:
        or     dword ptr [ecx + 10h], 80h
        mov    ecx, dword ptr [ecx]
        cmp    ecx, eax
        jne    SHORT loc_1_531E
loc_1_532B:
        sub    eax, eax
        pop    ebp
        ret
        db  000h
loc_1_5330:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
loc_1_5338:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
loc_1_5340:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_5348:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_5350:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_5358:
        pop    edx
        pop    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        jmp    edx
        db  000h, 000h
sub_1_5364:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  000h
sub_1_5370:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        pop    edi
        ret
        db  000h, 055h, 003h, 000h, 000h, 08Ah, 003h, 000h, 000h, 01Fh, 004h, 000h, 000h, 090h, 004h, 000h
        db  000h, 049h, 007h, 000h, 000h, 049h, 007h, 000h, 000h, 008h, 034h, 000h, 000h, 034h, 0FEh, 000h
        db  000h, 020h, 0B8h, 000h, 000h, 050h, 007h, 000h, 000h, 06Ch, 01Dh, 000h, 000h, 004h, 0F1h, 000h
        db  000h, 0A3h, 036h, 000h, 000h, 00Dh, 037h, 000h, 000h, 093h, 038h, 000h, 000h, 068h, 007h, 000h
        db  000h, 068h, 007h, 000h, 000h, 0C8h, 034h, 000h, 000h, 0DCh, 00Eh, 000h, 000h, 0F1h, 00Eh, 000h
        db  000h, 047h, 004h, 000h, 000h, 048h, 00Fh, 000h, 000h, 050h, 010h, 000h, 000h, 05Fh, 010h, 000h
        db  000h, 0B5h, 017h, 000h, 000h, 0F5h, 0E0h, 000h, 000h, 06Bh, 007h, 000h, 000h, 0BAh, 0E4h, 000h
        db  000h, 000h, 000h, 001h, 000h, 040h, 034h, 000h, 000h, 068h, 007h, 000h, 000h, 068h, 007h, 000h
        db  000h, 068h, 007h, 000h, 000h, 068h, 007h, 000h, 000h, 068h, 007h, 000h, 000h, 068h, 007h, 000h
        db  000h, 030h, 0F2h, 000h, 000h, 00Dh, 0B2h, 000h, 000h, 0C0h, 0FCh, 000h, 000h, 04Ah, 0FDh, 000h
        db  000h, 019h, 020h, 000h, 000h, 073h, 020h, 000h, 000h, 073h, 0AFh, 000h, 000h, 040h, 0B1h, 000h
        db  000h, 0A3h, 017h, 000h, 000h, 070h, 06Bh, 000h, 000h, 0CDh, 0D7h, 000h, 000h, 057h, 090h, 000h
        db  000h, 0FDh, 090h, 000h, 000h, 0B3h, 063h, 000h, 000h, 051h, 03Bh, 000h, 000h, 012h, 008h, 000h
        db  000h, 07Ch, 007h, 000h, 000h, 0FFh, 03Bh, 000h, 000h, 0FFh, 00Eh, 000h, 000h, 060h, 042h, 000h
        db  000h, 07Eh, 043h, 000h, 000h, 02Dh, 044h, 000h, 000h, 045h, 044h, 000h, 000h, 05Dh, 0FFh, 000h
        db  000h, 07Fh, 0FFh, 000h, 000h, 07Bh, 02Bh, 001h, 000h, 0C5h, 042h, 001h, 000h, 068h, 007h, 000h
        db  000h, 000h, 02Ah, 001h, 000h, 026h, 02Ah, 001h, 000h, 069h, 02Ah, 001h, 000h, 08Ah, 02Ah, 001h
        db  000h, 07Ch, 02Dh, 001h, 000h, 041h, 030h, 001h, 000h, 0B6h, 02Ch, 001h, 000h, 068h, 02Fh, 001h
        db  000h, 0C2h, 02Ah, 001h, 000h, 023h, 02Bh, 001h, 000h, 058h, 035h, 001h, 000h, 07Dh, 007h, 000h
        db  000h, 081h, 028h, 001h, 000h, 0B9h, 046h, 001h, 000h, 002h, 049h, 001h, 000h, 0DEh, 047h, 001h
        db  000h, 0B4h, 0D5h, 000h, 000h, 05Ch, 029h, 001h, 000h, 0C6h, 04Ch, 000h, 000h, 006h, 04Eh, 000h
        db  000h, 0F5h, 04Fh, 000h, 000h, 0B8h, 051h, 000h, 000h, 03Ah, 052h, 000h, 000h, 0A4h, 052h, 000h
        db  000h, 0CDh, 052h, 000h, 000h, 00Ch, 053h, 000h, 000h, 0E2h, 023h, 000h, 000h, 057h, 024h, 000h
        db  000h, 03Bh, 025h, 000h, 000h, 0DCh, 0A2h, 000h, 000h, 0C2h, 0A2h, 000h, 000h, 0B0h, 0A2h, 000h
        db  000h, 06Bh, 064h, 001h, 000h, 038h, 046h, 001h, 000h, 0FFh, 004h, 000h, 000h, 059h, 066h, 000h
        db  000h, 068h, 007h, 000h, 000h, 0EEh, 067h, 000h, 000h, 0B3h, 0A2h, 000h, 000h, 0BAh, 019h, 000h
        db  000h, 0A0h, 01Ah, 000h, 000h, 0F2h, 007h, 000h, 000h, 031h, 0D8h, 000h, 000h, 0E7h, 00Bh, 000h
        db  000h, 00Dh, 0B8h, 000h, 000h, 0A2h, 033h, 000h, 000h, 0EDh, 031h, 001h, 000h, 010h, 016h, 000h
        db  000h, 00Dh, 033h, 001h, 000h, 0E0h, 0A5h, 000h, 000h, 061h, 0A7h, 000h, 000h, 090h, 03Dh, 001h
        db  000h, 028h, 034h, 001h, 000h, 07Ch, 006h, 000h, 000h, 0F3h, 005h, 000h, 000h, 008h, 0FFh, 000h
        db  000h, 062h, 029h, 001h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 040h, 000h, 003h, 000h, 000h, 000h, 049h, 046h, 053h, 04Dh
        db  047h, 052h, 020h, 020h, 000h, 010h, 001h, 0A0h, 0D4h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  07Ch, 053h, 000h, 000h, 079h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_55D0:
        xor    edi, dword ptr [eax]
        inc    ebp
        outsb
        db  068h, 000h
loc_1_55D7:
        dec    ecx
        outsb
        je     SHORT loc_1_5647
        add    byte ptr [eax], al
loc_1_55DD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_55E1:
        add    dword ptr [eax], eax
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
loc_1_55FD:
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
loc_1_5647:
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
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_5914:
        hlt
        push   ebp
        add    byte ptr [eax], al
        xchg   esp, eax
        push   esi
        add    byte ptr [eax], al
        xor    al, 57h
        add    byte ptr [eax], al
        db  074h, 057h
        add    byte ptr [eax], al
loc_1_5924:
        adc    byte ptr [eax + edx], dl
loc_1_5927:
        adc    al, 0
loc_1_5929:
        add    byte ptr [eax], al
        db  000h
loc_1_592C:
        or     cl, byte ptr [eax]
        add    al, 8
loc_1_5930:
        add    byte ptr [eax], al
loc_1_5932:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5938:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_593C:
        add    byte ptr [eax], al
loc_1_593E:
        add    byte ptr [eax], al
loc_1_5940:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5944:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5948:
        add    byte ptr [eax], al
loc_1_594A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5954:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5958:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5960:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5964:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  070h, 059h
        add    byte ptr [eax], al
loc_1_5970:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5974:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5978:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_597C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5980:
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
loc_1_5A84:
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
loc_1_61A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_61A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_61A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_61AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_61B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_61B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_61B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_61BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_61C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_61C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
loc_1_61CE:
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
sub_1_61DC:
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        db  082h, 088h, 000h, 000h, 0F6h, 00Ch, 000h
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 021h
        jns    SHORT loc_1_624B
loc_1_624B:
        db  000h, 021h
        jns    SHORT loc_1_624F
loc_1_624F:
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0ADh, 07Eh, 000h, 000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 04Dh, 07Dh
        add    byte ptr [eax], al
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        and    dword ptr [ecx], edi
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 08Bh, 06Ah, 000h, 000h, 08Bh
        push   0
        db  000h, 00Fh
        insb
        add    byte ptr [eax], al
        pushad
        inc    edi
        add    byte ptr [eax], al
        pushad
        inc    edi
        add    byte ptr [eax], al
        in     al, dx
        dec    edi
        add    byte ptr [eax], al
        not    dword ptr [edx]
        db  000h, 0F7h
        push   edx
        add    byte ptr [eax], al
        db  021h, 082h, 000h, 000h, 06Fh, 055h
        add    byte ptr [eax], al
        db  0A0h, 075h, 000h, 000h, 025h
        pop    ecx
        add    byte ptr [eax], al
        db  068h, 062h, 000h, 000h, 0A5h
        arpl   word ptr [eax], ax
        db  000h, 0B1h, 06Eh, 000h, 000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 05Dh, 09Ch
        add    byte ptr [eax], al
        pop    ebp
        pushfd
        add    byte ptr [eax], al
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        db  015h, 087h, 000h, 000h, 0C2h
        jg     SHORT loc_1_633B
loc_1_633B:
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 060h, 047h
        add    byte ptr [eax], al
loc_1_634C:
        xor    dword ptr [esi], edx
        db  000h, 0F6h
        or     al, 0
        db  000h, 0CAh
        cli
        add    byte ptr [eax], al
        jne    SHORT loc_1_634C
        add    byte ptr [eax], al
        pop    eax
        mov    dword ptr [eax], eax
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 0F6h
        or     al, 0
        db  000h, 060h, 057h
        add    byte ptr [eax], al
        cli
        push   edi
        add    byte ptr [eax], al
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        pushad
        inc    edi
        add    byte ptr [eax], al
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        test   byte ptr [eax + eax], 0
        and    dword ptr [ebx], ebp
        db  000h, 021h
        imul   eax, dword ptr [eax], 0
        and    dword ptr [ebx], ebp
        db  000h, 021h
        imul   eax, dword ptr [eax], 0
        db  00Fh, 06Ch, 000h, 000h, 051h, 050h, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 053h, 054h, 000h, 000h
        db  053h, 054h, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 06Eh, 07Ch, 000h, 000h
        db  0D2h, 078h, 000h, 000h, 0D2h, 078h, 000h, 000h, 068h, 078h, 000h, 000h, 068h, 078h, 000h, 000h
        db  0D6h, 087h, 000h, 000h, 0D6h, 087h, 000h, 000h, 080h, 084h, 000h, 000h, 080h, 084h, 000h, 000h
        db  060h, 044h, 000h, 000h, 060h, 044h, 000h, 000h, 060h, 044h, 000h, 000h, 060h, 044h, 000h, 000h
        db  000h, 09Ah, 000h, 000h, 000h, 09Ah, 000h, 000h, 000h, 09Ah, 000h, 000h, 000h, 09Ah, 000h, 000h
        db  03Ch, 0F9h, 000h, 000h, 075h, 0F2h, 000h, 000h, 0CAh, 0FAh, 000h, 000h, 0F6h, 00Ch, 000h, 000h
        db  06Fh, 055h, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h
        db  0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h
        db  0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h
        db  0F6h, 00Ch, 000h, 000h, 060h, 044h, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 03Eh, 0F2h, 000h, 000h
        db  03Eh, 0F2h, 000h, 000h, 03Eh, 0F2h, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 03Eh, 0F2h, 000h, 000h
        db  0F6h, 00Ch, 000h, 000h, 0CDh, 0EEh, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h
        db  0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 03Eh, 0F2h, 000h, 000h, 03Eh, 0F2h, 000h, 000h
        db  03Eh, 0F2h, 000h, 000h, 03Eh, 0F2h, 000h, 000h, 0CDh, 0EEh, 000h, 000h, 0CDh, 0EEh, 000h, 000h
        db  0CDh, 0EEh, 000h, 000h, 0B9h, 0F1h, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h
        db  0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0FFh, 04Bh, 000h, 000h
        db  0FFh, 04Bh, 000h, 000h, 0FFh, 04Bh, 000h, 000h, 0FFh, 04Bh, 000h, 000h, 0FFh, 04Bh, 000h, 000h
        db  0FFh, 04Bh, 000h, 000h, 0FFh, 04Bh, 000h, 000h, 0FFh, 04Bh, 000h, 000h, 0FFh, 04Bh, 000h, 000h
        db  016h, 00Dh, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 070h, 029h, 000h, 000h, 0F6h, 00Ch, 000h, 000h
        db  0F6h, 00Ch, 000h, 000h, 004h, 0F8h, 000h, 000h, 0C7h, 0F9h, 000h, 000h, 0F6h, 00Ch, 000h, 000h
        db  0BFh, 068h, 000h, 000h, 0BFh, 068h, 000h, 000h, 03Ch, 0F8h, 000h, 000h, 07Bh, 0FDh, 000h, 000h
        db  09Ah, 03Ah, 000h, 000h, 03Eh, 0F2h, 000h, 000h, 050h, 03Ch, 000h, 000h, 023h, 03Eh, 000h, 000h
        db  06Ch, 040h, 000h, 000h, 0FEh, 03Fh, 000h, 000h, 0F6h, 00Ch, 000h, 000h, 0F6h, 00Ch, 000h, 000h
        db  037h, 077h, 000h, 000h, 0CEh, 0A0h, 000h, 000h, 0D1h, 025h, 001h, 000h, 0D1h, 025h, 001h, 000h
        db  061h, 041h, 000h, 000h, 036h, 0A1h, 000h, 000h, 016h, 0A2h, 000h, 000h, 047h, 0A2h, 000h, 000h
        db  0C0h, 057h, 000h, 000h, 03Bh, 08Fh, 000h, 000h, 061h, 091h, 000h, 000h, 060h, 047h, 000h, 000h
        db  012h, 094h, 000h, 000h, 0D5h, 07Eh, 000h, 000h, 0D1h, 025h, 001h, 000h, 073h, 087h, 000h, 000h
loc_1_6588:
        mov    byte ptr [ebp], ah
        db  000h, 088h, 065h, 000h, 000h, 000h
        add    byte ptr [eax], al
        db  000h, 0E1h
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 3c000005h], bl
        add    eax, dword ptr [eax]
        add    byte ptr [ebx + 5], ch
        add    byte ptr [eax], al
loc_1_65AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_65B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 1ah], bl
        add    byte ptr [eax], al
        db  0F0h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_6AEC:
        add    dword ptr [eax], eax
loc_1_6AEE:
        add    byte ptr [eax], al
        add    al, al
loc_1_6AF2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
sub_1_6B04:
        popfd
        push   ds
        add    byte ptr [eax], al
loc_1_6B08:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6B0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6B10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6B14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6B18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6B1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6B20:
        push   ebp
        mov    ebp, esp
        push   10000002h
        push   dword ptr [ebp + 8]
        push   8000h
        db  0E8h, 003h, 0DBh, 0FFh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret
loc_1_6B39:
        push   ebp
        mov    ebp, esp
        push   10000002h
        push   dword ptr [ebp + 8]
        push   8004h
        db  0E8h, 0EAh, 0DAh, 0FFh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret
sub_1_6B52:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    dword ptr [ebp - 14h], 12h
        mov    dword ptr [ebp - 10h], 2
        mov    eax, 1
        mov    cl, byte ptr [ebp + 0ch]
        shl    eax, cl
        mov    dword ptr [ebp - 8], eax
        mov    word ptr [ebp - 4], 1
        lea    eax, [ebp - 14h]
        push   eax
        mov    ax, word ptr [ebp + 8]
        push   eax
        call   sub_1_6BB3
        mov    esp, ebp
        pop    ebp
        ret
loc_1_6B8B:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   8000h
        call   sub_1_6B52
        mov    esp, ebp
        pop    ebp
        ret
loc_1_6B9F:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   8004h
        call   sub_1_6B52
        mov    esp, ebp
        pop    ebp
        ret
sub_1_6BB3:
        push   ebp
        mov    ebp, esp
        mov    cx, word ptr [ebp + 8]
        lea    eax, [ecx - 8001h]
        cmp    ax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   dword ptr [ebp + 0ch]
        movzx  eax, cx
        push   eax
        db  0E8h, 02Bh, 094h, 0FEh, 0FFh
        add    esp, 0ch
        cmp    eax, 2ah
        jne    SHORT loc_1_6BE4
        mov    eax, 1
        jmp    SHORT loc_1_6BF2
loc_1_6BE4:
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_1_6BF2
        mov    eax, 0ffh
loc_1_6BF2:
        pop    ebp
        ret
loc_1_6BF4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    si, word ptr [ebp + 8]
        push   esi
        call   sub_1_6BB3
        add    esp, 8
        movzx  ecx, si
        cmp    ecx, 8000h
        je     SHORT loc_1_6C33
        cmp    ecx, 8001h
        je     SHORT loc_1_6C58
        cmp    ecx, 8003h
        je     SHORT loc_1_6CA4
        cmp    ecx, 8004h
        je     SHORT loc_1_6C7E
        jmp    SHORT loc_1_6CA4
loc_1_6C33:
        cmp    dword ptr [edi + 4], 2
        jne    SHORT loc_1_6CA4
        test   byte ptr [edi + 10h], 2
        jne    SHORT loc_1_6CA4
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    ecx, dword ptr [ebp - 0ch]
        bsf    eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 0ch]
        db  0E8h, 063h, 0DAh, 0FFh, 0FFh
        jmp    SHORT loc_1_6CA1
loc_1_6C58:
        or     eax, eax
        jne    SHORT loc_1_6CA4
        cmp    dword ptr [edi + 4], 2
        jne    SHORT loc_1_6C7B
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [ebp - 8]
        bsf    eax, ecx
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        db  0E8h, 065h, 0DBh, 0FFh, 0FFh
        jmp    SHORT loc_1_6CA1
loc_1_6C7B:
        int3
        jmp    SHORT loc_1_6CA4
loc_1_6C7E:
        cmp    dword ptr [edi + 4], 2
        jne    SHORT loc_1_6CA4
        test   byte ptr [edi + 10h], 2
        jne    SHORT loc_1_6CA4
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp - 4]
        bsf    eax, ecx
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp - 4]
        db  0E8h, 035h, 0DBh, 0FFh, 0FFh
loc_1_6CA1:
        add    esp, 4
loc_1_6CA4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h
loc_1_6CB0:
        db  068h, 030h, 083h, 000h, 000h
        call   IFSMgr_printf
        add    esp, 4
        ret
loc_1_6CBE:
        db  068h, 060h, 083h, 000h, 000h
        call   IFSMgr_printf
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_6CD4:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_6CD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6CDC:
        xor    byte ptr [ebp], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0ADh, 019h, 000h, 000h, 0ADh
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_6D00:
        pop    ds
        add    byte ptr [ebx], bh
        add    byte ptr [edx], bl
        js     SHORT loc_1_6D08
loc_1_6D08:
        xchg   edi, eax
        add    byte ptr [ebp - 0cff2c00h], dh
        add    byte ptr [ecx], dl
        add    dword ptr [eax], esi
        add    dword ptr [esi + 1], ecx
        insd
        db  001h
loc_1_6D18:
        push   ss
        add    byte ptr [eax + eax], cl
        or     byte ptr [eax], al
        inc    esp
        add    byte ptr [ebp], al
        push   esi
        add    byte ptr [edx], cl
        add    byte ptr [eax + eax + 50h], cl
        add    byte ptr [eax + eax + 31h], dl
        add    byte ptr [eax], al
        db  000h
loc_1_6D30:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        add    byte ptr [ecx], 0
        add    byte ptr [ecx], 0
        add    byte ptr [ecx], 0
        add    byte ptr [ecx], 0
        add    byte ptr [ecx], 0
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
loc_1_6D54:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        add    byte ptr [ecx], 0
        db  069h, 0F7h, 000h, 000h, 080h, 041h
        add    byte ptr [eax], al
        add    byte ptr [ecx], 0
        add    byte ptr [ecx], 0
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
loc_1_6D78:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        insd
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [ecx], 0
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
loc_1_6D9C:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
loc_1_6DB0:
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
loc_1_6DD4:
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
loc_1_6DDC:
        push   ebx
        jb     SHORT loc_1_6DDF
loc_1_6DDF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 09Ch, 06Dh, 000h, 000h, 001h, 000h
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
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
loc_1_6E20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6E24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6E28:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6E2C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6E30:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6E34:
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
loc_1_6E58:
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
loc_1_6E7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6E80:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6E84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6E88:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6E8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0DDh, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h, 050h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_6EAC:
        dec    esp
        push   eax
        push   esp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx + ebx*2 + 50h], dh
        add    byte ptr [edx + ebx*2 + 50h], dh
        push   ebx
        xor    al, 57h
        add    byte ptr [eax], al
loc_1_6EC0:
        db  074h, 05Ah
        push   eax
        push   ebx
        push   edi
        pop    edx
        pop    edx
        push   ebx
        arpl   word ptr [eax], ax
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        db  000h, 0B4h, 06Eh, 000h, 000h, 01Ah, 000h
        add    byte ptr [eax], al
        mov    eax, 3400006eh
        add    byte ptr [eax], al
        db  000h, 0C0h
        outsb
        add    byte ptr [eax], al
loc_1_6EE4:
        cmp    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_6EE8:
        dec    esp
        push   eax
        push   esp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_6EF4:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_6EF8:
        push   ebx
        jb     SHORT loc_1_6EFB
loc_1_6EFB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
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
        add    byte ptr [edx], dl
        add    byte ptr [ebp], cl
        inc    ecx
        add    byte ptr [ecx], cl
        dec    esp
        add    byte ptr [ebx], dl
        dec    esp
        add    byte ptr [edi], cl
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax + eax + 2eh], bl
        add    byte ptr [eax], al
        db  000h, 058h, 06Fh
        add    byte ptr [eax], al
loc_1_6F5C:
        pop    eax
        outsd
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
loc_1_6FBC:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        insd
        inc    ecx
        add    byte ptr [eax], al
        in     eax, dx
        jecxz  loc_1_6FC7
loc_1_6FC7:
        db  000h, 06Dh, 041h
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        and    al, 0e2h
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        adc    dl, ah
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
loc_1_6FE0:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_6FE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6FE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_6FEC:
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
loc_1_7008:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_700C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7018:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7024:
        or     al, 70h
        add    byte ptr [eax], al
        sbb    byte ptr [eax], dh
        add    byte ptr [ecx], al
        inc    dword ptr [edi]
        add    al, 2
        dec    dword ptr [eax]
        add    eax, dword ptr [ebx]
        dec    dword ptr [eax]
        add    eax, dword ptr [edi + edi*8]
        add    dword ptr [eax + 60303ffh], eax
        inc    dword ptr [edi]
        add    al, 8
        inc    dword ptr [ecx]
        add    al, 0ch
        inc    dword ptr [edi]
        add    al, 0fh
        dec    dword ptr [eax]
        add    edx, dword ptr [eax]
        inc    dword ptr [ebx]
        add    edx, dword ptr [ecx]
        dec    dword ptr [8ff1203h]
        add    edx, dword ptr [50070202h]
        dec    dword ptr [ebx + eax]
        and    byte ptr [eax], ah
        or     al, byte ptr [edx]
        and    dword ptr [ecx], esp
        or     al, byte ptr [edx]
        push   esp
        inc    dword ptr [ecx]
        add    al, 52h
        inc    dword ptr [ecx]
        add    al, 32h
        dec    dword ptr [ecx]
        add    edx, dword ptr [ebp - 1]
        or     al, 3
        push   edi
        dec    dword ptr [ecx]
        add    edx, dword ptr [ebx - 1]
        or     eax, 1242404h
        add    al, 27h
        daa
        add    dword ptr [ebx + ebx], eax
        or     byte ptr [ebx], cl
        add    al, 1eh
        or     eax, dword ptr [0d0c1f04h]
        add    al, 13h
        add    byte ptr [ebx], cl
        pop    es
        push   esi
        inc    dword ptr [ebx]
        add    eax, dword ptr [ecx - 1]
        add    eax, dword ptr [ebx]
        inc    eax
        or     al, 8
        pop    es
        inc    ebx
        dec    dword ptr [eax]
        add    eax, dword ptr [edx - 1]
        or     dword ptr [ebx], eax
        cmp    eax, 3e070209h
        or     dword ptr [edx], eax
        pop    es
        aas
        or     dword ptr [edx], eax
        pop    es
        inc    esi
        add    al, byte ptr [edx]
        add    al, byte ptr [edi + 2]
        or     dword ptr [edx + edi], eax
        or     al, 0dh
        add    al, 36h
        inc    dword ptr [edx]
        add    bh, byte ptr [edx]
        or     al, 9
        add    al, 37h
        add    al, byte ptr [0e0c3404h]
        add    al, 45h
        inc    dword ptr [ecx]
        add    al, 33h
        inc    dword ptr [edx]
        add    dh, byte ptr [3c0308ffh]
        or     al, 5
        add    al, 38h
        inc    dword ptr [ecx]
        add    bh, byte ptr [ebx]
        or     al, 6
        add    al, 64h
        or     al, 0dh
        add    eax, 70b0b65h
        or     al, 4
        add    al, 67h
        pop    es
        or     dword ptr [edi], eax
        push   70b02h
        dec    dword ptr [0d0c0004h]
        add    al, 0
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
        db  000h, 09Ch, 03Ah, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax + 65h], cl
        popad
        db  070h, 020h
        push   eax
        popad
        db  067h, 065h, 073h, 03Ah
        or     ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 64h], ah
        jb     NEAR PTR loc_1_716D
        and    byte ptr [eax], ah
        and    byte ptr [eax + 61h], ch
        outsb
        insb
        and    byte ptr gs:[eax], ah
        and    byte ptr [ebx + 69h], dh
        and    byte ptr [bx + si], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 69h], dh
        db  07Ah, 065h
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 73h], ch
        db  074h, 020h
loc_1_716D:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        outsb
        db  065h, 078h, 074h
        jb     NEAR PTR loc_1_71DD
        and    byte ptr gs:[eax], ah
        and    byte ptr [ebp + 61h], ch
        db  078h, 066h
        jb     SHORT loc_1_71E7
        and    byte ptr gs:[eax], ah
        popad
        insb
        insb
        outsd
        arpl   word ptr [ebx + 6eh], sp
        db  074h, 00Ah
        add    byte ptr [eax], al
loc_1_7190:
        and    byte ptr [eax], ah
        and    eax, 25207838h
        cmp    byte ptr [eax + 20h], bh
        and    eax, 2073342eh
        and    eax, 25207838h
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [eax], ah
        and    eax, 25207838h
        cmp    byte ptr [esi], ch
        xor    al, 78h
        and    byte ptr [78342e38h], ah
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_71BC:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_71C0:
        dec    eax
        popad
        jo     SHORT loc_1_71FF
        or     ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        jae    SHORT loc_1_723E
        db  07Ah, 065h
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 74h], dh
        popad
loc_1_71DD:
        db  074h, 065h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
loc_1_71E7:
        add    byte ptr [edx + 55h], al
        push   ebx
        pop    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_71F0:
        inc    esi
        push   edx
        inc    ebp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_71F8:
        and    byte ptr [eax], ah
        and    eax, 25207838h
loc_1_71FF:
        cmp    byte ptr [eax + 20h], bh
        and    eax, 73342e34h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_7210:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        bound  ebx, qword ptr [esi]
        add    dword ptr [eax], eax
        jne    SHORT loc_1_7238
        add    dword ptr [eax], eax
        cdq
        push   ds
        add    dword ptr [eax], eax
        mov    bh, 1eh
        add    dword ptr [eax], eax
        retf   11eh
        db  000h, 0DDh, 01Eh, 001h, 000h, 005h, 020h, 001h, 000h, 018h, 020h, 001h, 000h, 02Bh, 020h, 001h
        db  000h
loc_1_7238:
        and    byte ptr ds:[ecx], al
        db  000h, 051h, 020h
loc_1_723E:
        add    dword ptr [eax], eax
        and    byte ptr fs:[ecx], al
        db  000h, 0F4h
        and    byte ptr [ecx], al
        db  000h, 007h
        and    dword ptr [ecx], eax
        db  000h, 03Fh
        and    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    edx, dword ptr [eax]
        db  008h, 08Fh, 021h, 001h, 000h, 0A2h
        and    dword ptr [ecx], eax
        db  000h, 0F8h
        and    dword ptr [ecx], eax
        db  000h, 022h
        and    al, byte ptr [ecx]
        db  000h, 035h, 022h, 001h, 000h, 05Fh
        and    al, byte ptr [ecx]
        db  000h, 072h, 022h
        add    dword ptr [eax], eax
        test   dword ptr [edx], esp
        add    dword ptr [eax], eax
loc_1_7274:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        out    49h, al
        add    byte ptr [eax], al
        cmp    dword ptr [edx], ecx
        db  000h, 039h
        dec    edx
        add    byte ptr [eax], al
        cmp    dword ptr [edx], ecx
        db  000h, 039h
        dec    edx
        add    byte ptr [eax], al
        cmp    dword ptr [edx], ecx
        db  000h, 039h
        dec    edx
        add    byte ptr [eax], al
        test   dword ptr [ecx], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 72h], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
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
loc_1_72E8:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        insd
        inc    ecx
        add    byte ptr [eax], al
        xchg   byte ptr [ecx + eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
loc_1_730C:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        insd
        inc    ecx
        add    byte ptr [eax], al
        mov    bh, 52h
        add    dword ptr [eax], eax
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
        insd
        inc    ecx
        add    byte ptr [eax], al
loc_1_7330:
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
loc_1_7334:
        push   ebx
        db  079h, 073h
        je     SHORT loc_1_739E
        insd
        pop    esp
        inc    ebx
        jne    SHORT loc_1_73B0
        db  072h, 065h
        outsb
        db  074h, 043h
        outsd
        outsb
        je     SHORT loc_1_73B9
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        inc    ebx
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        pop    esp
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 656e6f5ah
        dec    ecx
        outsb
        outsw
        db  072h, 06Dh
        popad
        je     SHORT loc_1_73CF
        outsd
        outsb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_736C:
        inc    ecx
        arpl   word ptr [ecx + ebp*2 + 76h], si
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 73616942h
        add    byte ptr [eax], al
loc_1_737C:
        push   ebx
        db  065h, 072h, 076h
        db  065h, 072h, 04Eh
        popad
        insd
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 06Fh, 06Fh
loc_1_739E:
        jmp    dword ptr [esi - 1]
        db  06Fh, 06Fh, 06Fh, 0DFh, 0DFh, 0FFh, 0DDh, 0FFh, 0DFh, 0DFh, 0DFh, 00Fh, 00Fh, 0FFh, 000h
loc_1_73B0:
        dec    dword ptr [edi]
        db  00Fh, 00Fh, 0F0h, 0FFh, 0F0h, 0FFh, 0F0h
loc_1_73B9:
        push   eax
        jmp    ptr [edi + 6fh]
        db  0FFh, 066h, 0FFh, 06Fh, 06Fh, 06Fh, 0DFh, 0DFh, 0FFh, 0DDh, 0FFh, 0DFh, 0DFh, 0DFh, 00Fh, 00Fh
        db  0FFh
loc_1_73CF:
        add    bh, bh
        db  00Fh, 00Fh, 00Fh
loc_1_73D4:
        outsw
        jmp    ptr [edi - 1]
        db  06Fh, 066h, 06Fh, 066h, 06Fh, 0FFh, 066h, 0FFh, 06Fh, 066h, 06Fh, 0DFh, 0DFh, 0FFh, 0DDh, 0FFh
        db  0DFh, 0DFh, 0DFh, 000h, 00Fh, 0FFh, 000h, 0FFh, 00Fh, 000h, 00Fh
loc_1_73F4:
        pushad
        outsd
        db  0F0h, 06Fh, 0F0h, 06Fh, 060h, 06Fh, 066h, 06Fh, 0FFh, 066h, 0FFh, 06Fh, 066h, 06Fh, 0DFh, 0DFh
        db  0FFh, 0DDh, 0FFh, 0DFh, 0DFh, 0DFh, 000h, 00Fh, 0FFh, 000h, 0FFh, 00Fh, 000h, 00Fh
loc_1_7414:
        xchg   esp, eax
        db  073h, 000h
        db  000h, 0B4h, 073h, 000h, 000h
loc_1_741C:
        aam    73h
        add    byte ptr [eax], al
        hlt
        jae    SHORT loc_1_7423
loc_1_7423:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 004h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_7444:
        jb     NEAR PTR loc_1_74B0
        jbe    SHORT loc_1_74AE
        and    byte ptr [ecx + 6eh], ch
        outsw
        add    byte ptr [eax], al
loc_1_7450:
        bound  ebp, qword ptr [edi + ebp*2 + 63h]
        imul   esp, dword ptr [eax], 69h
        outsb
        outsw
        add    byte ptr [eax], al
loc_1_745C:
        jae    SHORT loc_1_74C6
        insb
        insb
        and    byte ptr [edx + 65h], dh
        db  073h, 06Fh
        jne    SHORT loc_1_74DA
        arpl   word ptr [ebp + 20h], sp
        db  069h, 06Eh, 066h, 06Fh, 000h
loc_1_7470:
        outsd
        jo     SHORT loc_1_74D8
        outsb
        and    byte ptr [ebx + 46h], dl
        push   esp
        and    byte ptr [esi + 69h], ah
        insb
        jae    NEAR PTR loc_1_747F
loc_1_747F:
        add    byte ptr [edi + 70h], ch
        outsb
        and    byte ptr [ebp + 78h], ah
        db  074h, 065h
        outsb
        and    byte ptr fs:[esi + 69h], ah
        insb
        jae    NEAR PTR loc_1_7494
loc_1_7494:
        push   eax
        db  065h, 072h, 056h
        dec    ebp
        and    byte ptr [ecx + 74h], ah
        popad
        add    byte ptr [eax], al
loc_1_74A0:
        dec    eax
        popad
        db  070h, 020h
        inc    edx
        insb
        outsd
        arpl   word ptr [ebx + 20h], bp
        db  069h, 06Eh, 066h
loc_1_74AE:
        outsd
        db  000h
loc_1_74B0:
        dec    eax
        popad
        db  070h, 020h
        inc    ecx
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [edi + 6eh], 666e6920h
        outsd
        add    byte ptr [eax], al
loc_1_74C6:
        add    byte ptr [eax], al
loc_1_74C8:
        push   esp
        db  072h, 061h
        arpl   word ptr [ebp + 20h], sp
        dec    esp
        outsd
        add    byte ptr [bx + si], al
        add    byte ptr [ebx + 65h], dl
        je     SHORT loc_1_74F8
loc_1_74D8:
        dec    esp
        outsd
loc_1_74DA:
        and    byte ptr [di + 61h], cl
        db  073h, 06Bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_74E4:
        inc    esp
        imul   esi, dword ptr [ebx + 70h], 2079616ch
        imul   esp, dword ptr [esi + 73h], 716572h
        db  000h
loc_1_74F4:
        outsd
        db  070h, 065h
        outsb
loc_1_74F8:
        and    byte ptr [esi + 69h], ah
        insb
        jae    NEAR PTR loc_1_751F
        outsd
        outsb
        and    byte ptr [esi + 6fh], dh
        insb
        db  075h, 06Dh
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_750C:
        inc    esp
        imul   esi, dword ptr [ebx + 70h], 2079616ch
        push   6e61h
        insb
        add    byte ptr gs:[edi + ebp*2 + 67h], dl
loc_1_751F:
        insb
        and    byte ptr gs:[esi + 6fh], dh
        insb
        jne    SHORT loc_1_7595
        and    byte ptr gs:[edi + ebp*2 + 63h], ch
        imul   esp, dword ptr [eax], 77h
        db  072h, 069h
        db  074h, 065h
        and    byte ptr [ebx + 74h], dh
        popad
        je     SHORT loc_1_759F
        and    byte ptr [edx + esi*2 + 61h], dh
        db  070h, 070h
        imul   ebp, dword ptr [esi + 67h], 73694400h
        db  070h, 06Ch
        popad
        jns    SHORT loc_1_756C
        db  074h, 068h
        db  072h, 065h
        popad
        db  064h, 073h, 020h
        imul   ebp, dword ptr [esi + 20h], 6d736669h
        jb     NEAR PTR loc_1_755E
loc_1_755E:
        add    byte ptr [eax], al
loc_1_7560:
        inc    esp
        je     SHORT loc_1_7563
loc_1_7563:
        db  000h, 001h
        add    al, 0
        db  000h, 050h, 074h
        add    byte ptr [eax], al
loc_1_756C:
        int1
        add    eax, dword ptr [eax]
        db  000h, 05Ch, 074h, 000h
        db  000h, 079h, 004h
        add    byte ptr [eax], al
        jo     SHORT loc_1_75EE
        add    byte ptr [eax], al
        mov    eax, dword ptr [edi]
        add    byte ptr [eax], al
        db  080h, 074h, 000h, 000h, 0FEh
        or     byte ptr [eax], al
        db  000h, 094h, 074h, 000h, 000h, 0C7h, 006h
        add    byte ptr [eax], al
        db  0A0h, 074h, 000h, 000h, 0E0h
loc_1_7595:
        add    dword ptr [eax], eax
        db  000h, 0B0h, 074h, 000h, 000h, 037h
        add    al, byte ptr [eax]
loc_1_759F:
        db  000h, 0C8h
        je     SHORT loc_1_75A3
loc_1_75A3:
        db  000h, 0B0h, 06Ch, 000h, 000h, 0D4h
        je     SHORT loc_1_75AB
loc_1_75AB:
        db  000h, 0BEh, 06Ch, 000h, 000h, 0E4h
        je     SHORT loc_1_75B3
loc_1_75B3:
        db  000h, 0A8h, 00Bh, 000h, 000h, 0F4h
        je     SHORT loc_1_75BB
loc_1_75BB:
        db  000h, 0ECh
        or     al, 0
        db  000h, 00Ch, 075h, 000h, 000h, 0D6h, 00Dh
        add    byte ptr [eax], al
        sbb    al, 75h
        add    byte ptr [eax], al
        xor    byte ptr [edi], cl
        add    byte ptr [eax], al
        inc    esp
        jne    SHORT loc_1_75D3
loc_1_75D3:
        db  000h, 03Eh
        sldt   word ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_75E0:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_75E4:
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 25h], bl
        arpl   word ptr [ebp + 20h], bx
        and    byte ptr [eax], ah
loc_1_75EE:
        sub    eax, 0a732520h
        db  000h
loc_1_75F4:
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 45h], bl
        push   ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  071h, 075h
        imul   esi, dword ptr [edx + ecx + 20h], 203fh
        add    byte ptr [eax], al
loc_1_760C:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_7610:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        dec    ecx
        inc    esi
        push   ebx
        dec    ebp
        inc    edi
        push   edx
        add    byte ptr [eax], al
loc_1_761C:
        dec    ecx
        inc    esi
        push   ebx
        dec    ebp
        inc    edi
        push   edx
        add    byte ptr [eax], al
loc_1_7624:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_7628:
        push   esp
        push   69207369h
        db  073h, 020h
        outsb
        outsd
        je     SHORT loc_1_7654
        popad
        and    byte ptr [esi + 61h], dh
        insb
        imul   esp, dword ptr [eax + 73h], 69767265h
        arpl   word ptr [ebp + 20h], sp
        popad
        outsb
        db  079h, 020h
        insd
        outsd
        db  072h, 065h
        and    byte ptr [65737520h], ch
        and    byte ptr [edi], ah
loc_1_7654:
        jbe    NEAR PTR loc_1_76C4
        insd
        and    byte ptr [ebx + 27h], ah
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        inc    esp
        db  072h, 069h
        db  076h, 065h
        and    byte ptr [73692063h], ah
        and    byte ptr [ebp + 6eh], dh
        insd
        outsd
        db  075h, 06Eh
        db  074h, 069h
        outsb
        or     al, byte ptr [bx + si]
        add    byte ptr [eax], al
        db  000h
loc_1_767C:
        and    eax, 2d203a63h
        and    byte ptr [esi + 65h], cl
        je     SHORT loc_1_76FD
        outsd
        db  072h, 06Bh
        or     al, byte ptr [eax]
        add    byte ptr [2d203a63h], ah
        and    byte ptr [esi + 6ch], al
        popad
        je     SHORT loc_1_76DD
        popad
        je     SHORT loc_1_76A4
        add    byte ptr [eax], al
loc_1_769C:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_76A4:
        dec    esi
        outsd
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        and    byte ptr [edx + 65h], dh
        imul   esi, dword ptr fs:[edx + 65h], 6f697463h
        outsb
        jae    SHORT loc_1_76C4
        add    byte ptr [eax], al
loc_1_76BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_76C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_76C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_76C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_76CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_76D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_76D4:
        or     ah, byte ptr [52685373h]
        jae    NEAR PTR loc_1_7717
loc_1_76DD:
        and    byte ptr [eax], al
        add    byte ptr [esi + 6fh], al
        jb     SHORT loc_1_7747
        and    byte ptr fs:[ebx + 6ch], al
        outsd
        db  073h, 065h
        add    byte ptr fs:[eax], al
        add    byte ptr [ebp + 61h], cl
        imul   ebp, dword ptr [ebx + edx*2 + 6ch], 746fh
        add    byte ptr [eax], al
loc_1_76FC:
        dec    ebp
loc_1_76FD:
        popad
        db  070h, 070h
        and    byte ptr fs:[esi + eax*2 + 4eh], cl
        and    byte ptr [esi + 69h], al
        outsb
        jae    NEAR PTR loc_1_770D
loc_1_770D:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 6eh], dl
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
loc_1_7717:
        and    byte ptr [esi + 65h], ch
        je     SHORT loc_1_773C
        arpl   word ptr [edi + 6eh], bp
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        add    byte ptr [eax], al
loc_1_7728:
        inc    esp
        db  072h, 069h
        jbe    SHORT loc_1_7792
        and    byte ptr [20203a63h], ah
        sub    byte ptr [29786ch], ah
        add    byte ptr [eax], al
        db  000h
loc_1_773C:
        push   ebp
        outsb
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        and    byte ptr [edx + esi*2 + 69h], ah
loc_1_7747:
        jbe    SHORT loc_1_77AE
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        popad
        db  074h, 020h
        and    eax, 250a786ch
        jae    SHORT loc_1_7759
loc_1_7759:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 72h], dh
        pop    edi
        jae    SHORT loc_1_77CA
        cmp    eax, 27732527h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        db  073h, 072h
        pop    edi
        jae    SHORT loc_1_77DF
        db  072h, 069h
        popad
        insb
        cmp    eax, 2e382d25h
        xor    bh, byte ptr [eax + 20h]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 72h], dh
        pop    edi
loc_1_7792:
        imul   esp, dword ptr [eax + edi*2 + 3dh], 2e382d25h
        xor    bh, byte ptr [eax + 0ah]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 72h], dh
        pop    edi
        outsb
        db  065h, 078h, 074h
        cmp    eax, 382e3825h
        js     SHORT loc_1_77CE
loc_1_77AE:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        jae    SHORT loc_1_782A
        pop    edi
        jb     SHORT loc_1_7823
        cmp    eax, 382e3825h
        js     SHORT loc_1_77E2
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_77CA:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_77CE:
        and    byte ptr [ebx + 72h], dh
        pop    edi
        jne    NEAR PTR loc_1_7843
        arpl   word ptr [382e3825h], di
        db  078h, 00Ah
        add    byte ptr [eax], al
loc_1_77DF:
        add    byte ptr [ebx + 72h], dh
loc_1_77E2:
        pop    edi
        imul   ebp, dword ptr [esi + 55h], 253d6573h
        cmp    byte ptr [esi], ch
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 72h], dh
        pop    edi
        db  075h, 077h
        outsd
        jb     SHORT loc_1_7862
        cmp    eax, 2e382d25h
        xor    al, 78h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 72h], dh
        pop    edi
        dec    eax
        outsb
        inc    ebx
        outsb
        je     SHORT loc_1_7857
        and    eax, 342e382dh
        js     SHORT loc_1_782B
        add    byte ptr [eax], al
loc_1_7823:
        add    byte ptr [ebx + 72h], dh
        pop    edi
        push   ebp
        dec    esi
        inc    ebx
loc_1_782A:
        inc    ebx
loc_1_782B:
        outsb
        je     SHORT loc_1_786B
        and    eax, 322e382dh
        db  078h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        jae    SHORT loc_1_78AF
        pop    edi
        inc    esp
        jb     SHORT loc_1_78B7
        inc    ebx
        outsb
loc_1_7843:
        je     SHORT loc_1_7882
        and    eax, 322e382dh
        db  078h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 72h], dh
loc_1_7857:
        pop    edi
        db  072h, 074h
        db  079h, 070h
        cmp    eax, 2e382d25h
loc_1_7862:
        xor    bh, byte ptr [eax + 0ah]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 72h], dh
        pop    edi
loc_1_786B:
        insb
        popad
        jae    NEAR PTR loc_1_78AE
        and    eax, 322e382dh
        js     SHORT loc_1_7898
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 72h], dh
        pop    edi
loc_1_7882:
        push   eax
        jb     SHORT loc_1_78F4
        dec    ecx
        inc    esp
        cmp    eax, 382e3825h
        js     SHORT loc_1_78AE
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_7898:
        db  073h, 072h
        pop    edi
        push   esi
        outsd
        insb
        dec    ecx
        outsb
        outsw
        cmp    eax, 382e3825h
        js     SHORT loc_1_78B3
        add    byte ptr [eax], al
        add    byte ptr [ebx + 72h], dh
loc_1_78AE:
        pop    edi
loc_1_78AF:
        push   6e61h
loc_1_78B3:
        insb
        dec    eax
loc_1_78B7:
        popad
        cmp    eax, 382e3825h
        db  078h, 020h
        and    byte ptr [ebx + 72h], dh
        pop    edi
        dec    esp
        outsd
        arpl   word ptr [ebx + 50h], bp
        imul   esp, dword ptr [ebp + edi + 25h], 78382e38h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 72h], dh
        pop    edi
        dec    esp
        outsd
        arpl   word ptr [ebx + 53h], bp
        popad
        db  076h, 046h
        jne    SHORT loc_1_7956
        arpl   word ptr [382e3825h], di
        db  078h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_78F4:
        db  073h, 072h
        pop    edi
        dec    esp
        outsd
        arpl   word ptr [ebx + 54h], bp
        jns    SHORT loc_1_796E
        cmp    eax, 2e382d25h
        xor    bh, byte ptr [eax + 20h]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        db  073h, 072h
        pop    edi
        dec    esp
        outsd
        arpl   word ptr [ebx + 46h], bp
        insb
        popad
        jae    NEAR PTR loc_1_7955
        and    eax, 322e382dh
        js     SHORT loc_1_793F
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        jae    SHORT loc_1_7999
        pop    edi
        push   eax
        push   6e557379h
        imul   esi, dword ptr [ebp + edi + 25h], 322e382dh
        db  078h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_793C:
        jae    SHORT loc_1_79B0
        pop    edi
loc_1_793F:
        dec    esp
        outsd
        arpl   word ptr [ebx + 57h], bp
        popad
        imul   esi, dword ptr [ebx + eax*2 + 6eh], 2d253d74h
        cmp    byte ptr [esi], ch
        xor    bh, byte ptr [eax + 20h]
        and    byte ptr [ebx + 72h], dh
loc_1_7955:
        pop    edi
loc_1_7956:
        dec    esp
        outsd
        arpl   word ptr [ebx + 4fh], bp
        ja     SHORT loc_1_79CB
        jb     NEAR PTR loc_1_799D
        and    eax, 78382e38h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 72h], dh
loc_1_796E:
        pop    edi
        dec    esp
        outsd
        arpl   word ptr [ebx + 52h], bp
        popad
        inc    ebx
        outsb
        je     SHORT loc_1_79B8
        and    eax, 342e382dh
        js     SHORT loc_1_798C
        add    byte ptr [eax], al
loc_1_7984:
        db  073h, 072h
        pop    edi
        dec    esp
        outsd
        arpl   word ptr [ebx + 57h], bp
loc_1_798C:
        jb     SHORT loc_1_79F7
        je     SHORT loc_1_79F5
        inc    ebx
        outsb
        je     SHORT loc_1_79D1
        and    eax, 342e382dh
loc_1_7999:
        db  078h, 020h
        db  073h, 072h
loc_1_799D:
        pop    edi
        insb
        popad
        jae    NEAR PTR loc_1_79D6
        cmp    eax, 2e382d25h
        xor    bh, byte ptr [eax + 20h]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_79B0:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 72h], dh
        pop    edi
loc_1_79B8:
        jo     SHORT loc_1_7A28
        jbe    SHORT loc_1_79F9
        and    eax, 78382e38h
        or     al, byte ptr [eax]
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_79CB:
        add    byte ptr [204d5673h], ah
loc_1_79D1:
        and    eax, 786ch
loc_1_79D6:
        add    byte ptr [eax], al
loc_1_79D8:
        and    byte ptr [eax], ah
        sub    byte ptr [ebx + 79h], dl
        db  073h, 074h
        insd
        and    byte ptr [esi + 4dh], dl
        sub    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_79E8:
        and    byte ptr [eax], ah
        sub    byte ptr [ebx + 75h], al
        jb     SHORT loc_1_7A61
        outsb
        je     SHORT loc_1_7A13
        push   esi
        dec    ebp
loc_1_79F5:
        sub    dword ptr [eax], eax
loc_1_79F7:
        add    byte ptr [edx], cl
loc_1_79F9:
        and    eax, 20202073h
        and    byte ptr [ebx + 74h], dl
        popad
        je     SHORT loc_1_7A79
        db  073h, 020h
        and    eax, 202c786ch
        dec    eax
        imul   esp, dword ptr [edi + 68h], 206e694ch
loc_1_7A13:
        and    eax, 202c786ch
        inc    ebx
        insb
        imul   esp, dword ptr [ebp + 6eh], 72745074h
        and    byte ptr [202c786ch], ah
        push   esi
loc_1_7A28:
        dec    ebp
        dec    ecx
        and    byte ptr fs:[0a786ch], ah
        add    byte ptr [eax], al
        add    byte ptr [20202073h], ah
        and    byte ptr [ecx + 66h], cl
        db  073h, 050h
        push   esi
        and    byte ptr [202c786ch], ah
        add    byte ptr [eax], al
        add    byte ptr [ecx + 66h], ch
        jae    SHORT loc_1_7ABE
        db  065h, 071h, 020h
        and    eax, 0a786ch
loc_1_7A54:
        outsb
        outsd
        and    byte ptr [ecx + 66h], ch
        jae    SHORT loc_1_7ACD
        db  065h, 071h, 00Ah
        add    byte ptr [eax], al
loc_1_7A60:
        dec    esi
loc_1_7A61:
        je     NEAR PTR loc_1_7A84
        popad
        outsb
        and    byte ptr fs:[esi + 6ch], al
        popad
        db  074h, 046h
        popad
        db  074h, 020h
        inc    esi
        imul   ebp, dword ptr [ebp + 73h], 726f6620h
loc_1_7A79:
        and    byte ptr [esi + 4dh], dl
        and    byte ptr [2820786ch], ah
        db  073h, 075h
loc_1_7A84:
        db  073h, 070h
        outsb
        sub    dword ptr fs:[edx], edi
        or     al, byte ptr [eax]
        add    byte ptr [esi + 65h], cl
        je     SHORT loc_1_7AB4
        popad
        outsb
        and    byte ptr fs:[esi + 6ch], al
        popad
        db  074h, 046h
        popad
        db  074h, 020h
        inc    esi
        imul   ebp, dword ptr [ebp + 73h], 726f6620h
        and    byte ptr [esi + 4dh], dl
        and    byte ptr [0a3a786ch], ah
        add    byte ptr [eax], al
loc_1_7AB4:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7ABC:
        dec    esi
        outsd
loc_1_7ABE:
        and    byte ptr [edi + 70h], ch
        outsb
        and    byte ptr [esi + 65h], cl
        je     SHORT loc_1_7AE8
        outsd
        db  072h, 020h
        inc    esi
        insb
loc_1_7ACD:
        popad
        je     SHORT loc_1_7B16
        popad
        db  074h, 020h
        imul   bp, word ptr [ebp + 73h], 6620h
        outsd
        db  072h, 020h
        je     SHORT loc_1_7B47
        imul   esi, dword ptr [ebx + 20h], 0a4d56h
        add    byte ptr [eax], al
loc_1_7AE8:
        and    eax, 74654e73h
        add    byte ptr [eax], al
        add    byte ptr [616c4673h], ah
        db  074h, 046h
        popad
        je     SHORT loc_1_7AFA
loc_1_7AFA:
        add    byte ptr [eax], al
loc_1_7AFC:
        and    byte ptr [eax + 72h], dh
        imul   ebp, dword ptr [esi + 74h], 66200000h
        imul   ebp, dword ptr [ebp + 2ch], 3d4e4620h
        and    eax, 2c202078h
        and    byte ptr [esi + 68h], ah
loc_1_7B16:
        popad
        outsb
        insb
        cmp    eax, 2c786c25h
        and    byte ptr [ebx + 68h], dl
        insb
        insb
        and    byte ptr [edx + 65h], dl
        db  073h, 06Fh
        jne    SHORT loc_1_7B9F
        arpl   word ptr [ebp + 3dh], sp
        and    eax, 0a786ch
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        push   725fh
        pop    di
        arpl   word ptr [edi + 75h], bp
        outsb
        db  074h, 03Dh
loc_1_7B47:
        and    eax, 70202c78h
        imul   esp, dword ptr [ebp + edi + 25h], 0a78h
loc_1_7B54:
        and    eax, 20202073h
        and    byte ptr [eax + 6fh], dh
        db  073h, 03Dh
        and    eax, 202c786ch
        push   eax
        jb     SHORT loc_1_7BDA
        dec    ecx
        js     NEAR PTR loc_1_7B8A
        cmp    eax, 2c642520h
        and    byte ptr [eax + 66h], ch
        insb
        popad
        cmp    eax, 202c7825h
        push   745fh
        db  079h, 070h
        cmp    eax, 202c7825h
        push   6d5fh
loc_1_7B8A:
        outsd
        cmp    eax, 0a7825h
        add    byte ptr [eax], al
loc_1_7B94:
        and    eax, 20202073h
        and    byte ptr [ebx + 70h], dl
        outsd
        outsd
        insb
loc_1_7B9F:
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr gs:[ecx + 73h], ch
        and    byte ptr [edi + 70h], ch
        outsb
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_7BB0:
        and    eax, 20202073h
        and    byte ptr [ebx + 70h], dl
        outsd
        outsd
        insb
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr gs:[ecx + 73h], ch
        and    byte ptr [ebx + 6ch], ah
        outsd
        db  073h, 065h
        or     al, byte ptr fs:[eax]
loc_1_7BCC:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_7BD0:
        dec    esi
        db  065h, 074h, 020h
        popad
        outsb
        and    byte ptr fs:[esi + 6ch], al
loc_1_7BDA:
        popad
        db  074h, 046h
        popad
        db  074h, 020h
        inc    ebp
        db  078h, 074h
        outsb
        and    byte ptr fs:[esi + 69h], al
        insb
        jae    NEAR PTR loc_1_7BF9
        add    byte ptr [edx], cl
        and    byte ptr [edi + ebp*2 + 74h], dl
        popad
        insb
        inc    ecx
        insb
loc_1_7BF9:
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr fs:[2c782520h], bh
        and    byte ptr [esi + 65h], cl
        db  078h, 074h
        inc    esi
        jb     SHORT loc_1_7C73
        cmp    eax, 202c7825h
        inc    esi
        db  072h, 065h
        inc    ebx
        outsb
        je     SHORT loc_1_7C59
        and    eax, 4c202c78h
        outsd
        db  077h, 065h
        db  073h, 074h
        inc    esi
        db  072h, 065h
        cmp    eax, 0a0a7825h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7C34:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7C3C:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7C44:
        dec    esi
        outsd
        and    byte ptr [edi + 70h], ch
        outsb
        and    byte ptr [esi + 65h], cl
        db  074h, 020h
        outsd
        jb     SHORT loc_1_7C73
        inc    esi
        insb
        popad
        db  074h, 046h
        popad
loc_1_7C59:
        je     SHORT loc_1_7C7B
        imul   bp, word ptr [ebp + 73h], 0ah
        add    byte ptr [eax], al
loc_1_7C64:
        and    eax, 74654e73h
        add    byte ptr [eax], al
        add    byte ptr [616c4673h], ah
        je     SHORT loc_1_7CB9
loc_1_7C73:
        popad
        je     SHORT loc_1_7C76
loc_1_7C76:
        add    byte ptr [eax], al
loc_1_7C78:
        and    byte ptr [eax + 72h], dh
loc_1_7C7B:
        imul   ebp, dword ptr [esi + 74h], 66200000h
        imul   ebp, dword ptr [ebp + 2ch], 4e465320h
        cmp    eax, 20207825h
        sub    al, 20h
        push   6e61h
        insb
        cmp    eax, 2c786c25h
        and    byte ptr [ebx + 68h], dl
        insb
        insb
        and    byte ptr [edx + 65h], dl
        db  073h, 06Fh
        jne    SHORT loc_1_7D1C
        arpl   word ptr [ebp + 3dh], sp
        and    eax, 0a786ch
        add    byte ptr [eax], al
loc_1_7CB4:
        and    eax, 20202073h
loc_1_7CB9:
        and    byte ptr [ebx + 46h], dl
        push   esp
        cmp    eax, 0a786c25h
        add    byte ptr [eax], al
loc_1_7CC4:
        and    byte ptr [eax], ah
        daa
        add    byte ptr [edi], ah
        sub    al, 20h
        db  073h, 066h
        pop    edi
        jb     SHORT loc_1_7D35
        pop    di
        arpl   word ptr [edi + 75h], bp
        outsb
        db  074h, 03Dh
        and    eax, 0a000078h
        add    byte ptr [eax], al
        add    byte ptr [20202073h], ah
        and    byte ptr [eax + 6fh], dh
        jae    SHORT loc_1_7D27
        and    eax, 202c786ch
        push   eax
        db  072h, 074h
        dec    ecx
        db  064h, 078h, 020h
        cmp    eax, 2c642520h
        and    byte ptr [eax + 66h], ch
        insb
        popad
        cmp    eax, 202c7825h
        push   705fh
        push   ebx
        inc    esi
        push   esp
        cmp    eax, 0a786c25h
        add    byte ptr [eax], al
loc_1_7D14:
        and    eax, 20202073h
        and    byte ptr [ebx + 70h], dl
loc_1_7D1C:
        outsd
        outsd
        insb
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr gs:[ecx + 73h], ch
loc_1_7D27:
        and    byte ptr [edi + 70h], ch
        outsb
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_7D30:
        and    eax, 20202073h
loc_1_7D35:
        and    byte ptr [ebx + 70h], dl
        outsd
        outsd
        insb
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr gs:[ecx + 73h], ch
        and    byte ptr [ebx + 6ch], ah
        outsd
        jae    SHORT loc_1_7DAE
        or     al, byte ptr fs:[eax]
loc_1_7D4C:
        and    eax, 49000073h
        outsd
        push   edx
        jno    NEAR PTR loc_1_7D76
        cmp    eax, 2e382520h
        cmp    byte ptr [eax + 20h], bh
        or     al, byte ptr [eax]
loc_1_7D60:
        imul   esi, dword ptr [edx + 5fh], 676e656ch
        je     SHORT loc_1_7DD1
        cmp    eax, 382e3825h
        db  078h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_7D76:
        imul   esi, dword ptr [edx + 5fh], 67616c66h
        jae    SHORT loc_1_7DBC
        and    eax, 322e382dh
        db  078h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 72h], ch
        pop    edi
        db  075h, 073h
        jb     NEAR PTR loc_1_7DD4
        and    eax, 322e382dh
        db  078h, 00Ah
        add    byte ptr [eax], al
loc_1_7DA0:
        imul   esi, dword ptr [edx + 5fh], 3d6e6673h
        and    eax, 342e382dh
        js     SHORT loc_1_7DCE
loc_1_7DAE:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 72h], ch
        pop    edi
        db  070h, 069h
loc_1_7DBC:
        cmp    eax, 382e3825h
        db  078h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_7DCE:
        and    byte ptr [ecx + 72h], ch
loc_1_7DD1:
        pop    edi
        db  070h, 070h
loc_1_7DD4:
        popad
        db  074h, 068h
        cmp    eax, 382e3825h
        db  078h, 00Ah
        add    byte ptr [eax], al
loc_1_7DE0:
        imul   esi, dword ptr [edx + 5fh], 31787561h
        cmp    eax, 382e3825h
        js     SHORT loc_1_7E0E
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        imul   esi, dword ptr [edx + 5fh], 61746164h
        cmp    eax, 382e3825h
        js     SHORT loc_1_7E24
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_7E0E:
        imul   esi, dword ptr [edx + 5fh], 6974706fh
        outsd
        outsb
        db  073h, 03Dh
        and    eax, 342e382dh
        db  078h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7E24:
        imul   esi, dword ptr [edx + 5fh], 6f727265h
        db  072h, 03Dh
        and    eax, 342e382dh
        db  078h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 72h], ch
        pop    edi
        db  072h, 068h
        cmp    eax, 382e3825h
        js     SHORT loc_1_7E67
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        imul   esi, dword ptr [edx + 5fh], 253d6866h
        cmp    byte ptr [esi], ch
        cmp    byte ptr [eax + 0ah], bh
        add    byte ptr [ecx + 72h], ch
        pop    edi
        jo     SHORT loc_1_7ED4
        db  073h, 03Dh
loc_1_7E67:
        and    eax, 78382e38h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        imul   esi, dword ptr [edx + 5fh], 32787561h
        cmp    eax, 382e3825h
        db  078h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        imul   esi, dword ptr [edx + 5fh], 33787561h
        cmp    eax, 382e3825h
        db  078h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7EA0:
        imul   esi, dword ptr [edx + 5fh], 3d766570h
        and    eax, 78382e38h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_7EB0:
        or     cl, byte ptr [ecx + 66h]
        db  073h, 052h
        db  065h, 071h, 03Dh
        and    eax, 78382e38h
        or     al, byte ptr [eax]
        add    byte ptr [ecx + 66h], ch
        db  073h, 05Fh
        jo     SHORT loc_1_7F2C
        push   2e38253dh
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_7ED4:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        imul   esp, dword ptr [esi + 73h], 6673705fh
        db  074h, 03Dh
        and    eax, 78382e38h
        or     al, byte ptr [eax]
loc_1_7EE8:
        imul   esp, dword ptr [esi + 73h], 7273705fh
        cmp    eax, 382e3825h
        js     SHORT loc_1_7F16
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        imul   esp, dword ptr [esi + 73h], 6264705fh
        cmp    eax, 382e3825h
        js     SHORT loc_1_7F2C
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_7F16:
        imul   esp, dword ptr [esi + 73h], 6f72705fh
        imul   esp, dword ptr [ebp + edi + 25h], 78382e38h
        or     al, byte ptr [eax]
        add    byte ptr [ecx + 66h], ch
        jae    SHORT loc_1_7F8B
loc_1_7F2C:
        jne    NEAR PTR loc_1_7F9D
        arpl   word ptr [2e382d25h], di
        xor    bh, byte ptr [eax + 20h]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 66h], ch
        jae    SHORT loc_1_7FA2
        jb     NEAR PTR loc_1_7FBC
        cmp    eax, 2e382d25h
        xor    bh, byte ptr [eax + 20h]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        imul   esp, dword ptr [esi + 73h], 6c66685fh
        popad
        cmp    eax, 2e382d25h
        xor    bh, byte ptr [eax + 0ah]
        add    byte ptr [eax], al
        add    byte ptr [ecx + 66h], ch
        db  073h, 05Fh
        outsb
        insb
        popad
        db  067h, 073h, 03Dh
        and    eax, 322e382dh
        db  078h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 66h], ch
        jae    SHORT loc_1_7FE6
        db  070h, 062h
        db  075h, 066h
loc_1_7F8B:
        db  066h, 065h, 072h, 03Dh
        and    eax, 78382e38h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 66h], ch
loc_1_7F9D:
        jae    SHORT loc_1_7FFE
        push   esi
        dec    ebp
        dec    eax
loc_1_7FA2:
        popad
        outsb
        insb
        cmp    eax, 382e3825h
        db  078h, 00Ah
        add    byte ptr [eax], al
loc_1_7FB0:
        imul   esp, dword ptr [esi + 73h], 3d56505fh
        and    eax, 78382e38h
loc_1_7FBC:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_7FC0:
        dec    esi
        outsd
        and    byte ptr [edi + 70h], ch
        outsb
        and    byte ptr [esi + 69h], ah
        insb
        jae    NEAR PTR loc_1_7FEE
        outsw
        jb     SHORT loc_1_7FF2
        je     SHORT loc_1_803C
        imul   esi, dword ptr [ebx + 20h], 6c656873h
        insb
        and    byte ptr [edx + 65h], dh
        db  073h, 06Fh
        jne    SHORT loc_1_8055
        arpl   word ptr [ebp + 0ah], sp
loc_1_7FE6:
        add    byte ptr [eax], al
loc_1_7FE8:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
loc_1_7FEE:
        add    byte ptr [eax], al
loc_1_7FF0:
        and    byte ptr [eax], ah
loc_1_7FF2:
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7FF8:
        inc    ebp
        jb     SHORT loc_1_806D
        outsd
        jb     SHORT loc_1_8038
loc_1_7FFE:
        and    byte ptr [esi + 68h], ah
        popad
        outsb
        insb
        and    byte ptr gs:[78382e38h], ah
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        outsw
        db  075h, 06Eh
        and    byte ptr fs:[ecx + 6eh], ch
        and    byte ptr [ecx + 6eh], ah
        jns    SHORT loc_1_803E
        push   ebx
        inc    esi
        push   esp
        popad
        bound  esp, qword ptr [ecx]
        and    dword ptr [ecx], esp
        or     al, byte ptr [eax]
loc_1_8028:
        push   65725f66h
        popad
        and    byte ptr fs:[2e382520h], bh
        cmp    byte ptr [eax + 20h], bh
loc_1_8038:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_803C:
        and    byte ptr [eax], ah
loc_1_803E:
        push   72775f66h
        imul   esi, dword ptr [ebp + 20h], 3825203dh
        cmp    byte ptr cs:[eax + 20h], bh
        and    byte ptr [eax], ah
        and    byte ptr [eax + 66h], ch
        pop    edi
loc_1_8055:
        insd
        imul   esi, dword ptr [ebx + 63h], 25203d20h
        cmp    byte ptr [esi], ch
        cmp    byte ptr [eax + 0ah], bh
        add    byte ptr [eax], al
loc_1_8064:
        push   665fh
        push   25203d20h
loc_1_806D:
        cmp    byte ptr [esi], ch
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   705fh
        db  073h, 072h
        and    byte ptr [2e382520h], bh
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [esi + 68h], ah
        pop    edi
        db  070h, 053h
        inc    esi
        push   esp
        and    byte ptr [2e382520h], bh
        cmp    byte ptr [eax + 0ah], bh
        add    byte ptr [eax], al
loc_1_80A0:
        push   705fh
        outsd
        jae    SHORT loc_1_8110
        je     SHORT loc_1_8112
        outsd
        outsb
        and    byte ptr [2e382520h], bh
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [eax], ah
        push   645fh
        jbe    NEAR PTR loc_1_8123
        insb
        popad
        jae    NEAR PTR loc_1_80E2
        cmp    eax, 382d2520h
        xor    al, 78h
        and    byte ptr [esi + 68h], ah
        pop    edi
        push   67616c66h
        and    byte ptr [382d2520h], bh
        xor    bh, byte ptr cs:[eax + 0ah]
        add    byte ptr [eax], al
        add    byte ptr [esi + 68h], ah
loc_1_80E2:
        pop    edi
        db  074h, 079h
        db  070h, 065h
        and    byte ptr [382d2520h], bh
        xor    bh, byte ptr cs:[eax + 20h]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   725fh
        pop    di
        arpl   word ptr [edi + 75h], bp
        outsb
        db  074h, 020h
        cmp    eax, 382d2520h
        xor    al, 78h
        and    byte ptr [esi + 68h], ah
        pop    edi
loc_1_8110:
        insd
        outsd
loc_1_8112:
        and    byte ptr gs:[382d2520h], bh
        xor    al, 78h
        or     al, byte ptr [eax]
        add    byte ptr [esi + 68h], ah
        pop    edi
loc_1_8123:
        push   6b636f6ch
        imul   ebp, dword ptr [esi + 66h], 203d206fh
        and    eax, 78382e38h
        and    byte ptr [eax], ah
        push   705fh
        db  072h, 065h
        db  076h, 020h
        cmp    eax, 2e382520h
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   6e5fh
        db  065h, 078h, 074h
        and    byte ptr [2e382520h], bh
        cmp    byte ptr [eax + 0ah], bh
        add    byte ptr [eax], al
loc_1_815C:
        push   735fh
        outsb
        and    byte ptr [382d2520h], bh
        xor    al, 78h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   6d5fh
        insd
        db  073h, 066h
        outsb
        and    byte ptr [382d2520h], bh
        xor    al, 78h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   705fh
        imul   esp, dword ptr [eax + 3dh], 2e382520h
        cmp    byte ptr [eax + 0ah], bh
        add    byte ptr [esi + 68h], ah
        pop    edi
        outsb
        db  074h, 069h
        and    byte ptr fs:[2e382520h], bh
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   665fh
        push   67616c46h
        jae    SHORT loc_1_81D9
        cmp    eax, 382d2520h
        xor    al, 78h
        and    byte ptr [eax], ah
        push   495fh
        outsb
        inc    ebx
        insb
        outsd
        jae    SHORT loc_1_8232
        inc    ebx
        outsb
        db  074h, 020h
        cmp    eax, 382d2520h
        xor    al, 78h
loc_1_81D9:
        or     al, byte ptr [eax]
        add    byte ptr [6f562d2dh], ch
        insb
        db  075h, 06Dh
        and    byte ptr gs:[edi + ebp*2 + 63h], ch
        imul   ebp, dword ptr [ecx + 6eh], 67h
        and    byte ptr [ecx + 6eh], ch
        outsw
        sub    eax, 0a2d2dh
        add    byte ptr [eax + 6ch], ch
        pop    edi
        jb     SHORT loc_1_8262
        popad
        and    byte ptr fs:[2e382520h], bh
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   72775f6ch
        imul   esi, dword ptr [ebp + 20h], 3825203dh
        cmp    byte ptr cs:[eax + 20h], bh
        and    byte ptr [eax], ah
        and    byte ptr [eax + 6ch], ch
        pop    edi
        insd
        imul   esi, dword ptr [ebx + 63h], 25203d20h
        cmp    byte ptr [esi], ch
        cmp    byte ptr [eax + 0ah], bh
loc_1_8232:
        add    byte ptr [eax], al
loc_1_8234:
        push   6f6c5f6ch
        arpl   word ptr [ebx + 20h], bp
        cmp    eax, 2e382520h
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   6c665f6ch
        popad
        jae    NEAR PTR loc_1_8273
        cmp    eax, 382d2520h
        xor    al, 78h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 6ch], ch
loc_1_8262:
        pop    edi
        db  070h, 061h
        db  074h, 068h
        outsb
        popad
        insd
        and    byte ptr gs:[0a732520h], bh
        add    byte ptr [eax], al
loc_1_8273:
        add    byte ptr [eax + 6ch], ch
        pop    edi
        db  070h, 061h
        db  074h, 068h
        outsb
        popad
        insd
        and    byte ptr gs:[0a272720h], bh
        add    byte ptr [eax], al
        add    byte ptr [edi + 72h], dl
        imul   esi, dword ptr [ebp + 20h], 74617473h
        and    byte ptr gs:[edx + esi*2 + 61h], dh
        db  070h, 070h
        imul   ebp, dword ptr [esi + 67h], 6e6f6420h
        and    byte ptr gs:[edi + 6eh], ch
        insb
        jns    SHORT loc_1_82C7
        outsd
        outsb
        and    byte ptr [ebp + 62h], ah
        jne    SHORT loc_1_8316
        and    byte ptr [edx + 75h], ah
        imul   ebp, dword ptr [esp + 73h], 6854000ah
        db  072h, 065h
        popad
        jae    NEAR PTR loc_1_82E0
        imul   ebp, dword ptr [esi + 20h], 6d736669h
loc_1_82C7:
        db  067h, 072h, 03Ah
        or     al, byte ptr [eax]
loc_1_82CC:
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 0a2d2dh
loc_1_82E0:
        push   esp
        push   64616572h
        and    byte ptr [2e382520h], bh
        cmp    byte ptr [eax + 20h], bh
        sub    byte ptr [eax], dh
        js     SHORT loc_1_8318
        sub    eax, 78322e32h
        sub    dword ptr [eax], esp
        inc    ebx
        outsd
        db  075h, 06Eh
        db  074h, 020h
        cmp    eax, 25783020h
        sub    eax, 78342e38h
        and    byte ptr [ebp + 61h], cl
        db  072h, 06Bh
        cmp    eax, 20206425h
loc_1_8316:
        inc    edx
        insb
loc_1_8318:
        outsd
        arpl   word ptr [ebx + 65h], bp
        cmp    eax, 20206425h
        dec    esi
        outsd
        inc    edx
        insb
        outsd
        arpl   word ptr [ebx + 3dh], bp
        and    eax, 0a64h
        add    byte ptr [edi + ebp*2 + 67h], cl
        and    byte ptr [ecx + ebp*2 + 73h], ah
        db  070h, 06Ch
        popad
        db  079h, 020h
        outsb
        outsd
        db  074h, 020h
        db  073h, 075h
        jo     SHORT loc_1_83B4
        outsd
        db  072h, 074h
        and    byte ptr fs:[edi + 6eh], ch
        and    byte ptr [edx + 65h], dh
        je     SHORT loc_1_83B2
        imul   ebp, dword ptr [eax + 62h], 646c6975h
        db  073h, 02Eh
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 61h], cl
        db  073h, 06Bh
        and    byte ptr [ebx + 65h], dh
        db  074h, 074h
        imul   ebp, dword ptr [esi + 67h], 746f6e20h
        and    byte ptr [ebx + 75h], dh
        db  070h, 070h
        outsd
        jb     SHORT loc_1_83EC
        and    byte ptr fs:[edi + 6eh], ch
        and    byte ptr [edx + 65h], dh
        db  074h, 061h
        imul   ebp, dword ptr [eax + 62h], 646c6975h
        jae    SHORT loc_1_83BA
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_8390:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_8394:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_8398:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_83A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_83A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_83AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_83B0:
        add    byte ptr [eax], al
loc_1_83B2:
        add    byte ptr [eax], al
loc_1_83B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_83BA:
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
loc_1_83EC:
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
loc_1_87B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_87B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
