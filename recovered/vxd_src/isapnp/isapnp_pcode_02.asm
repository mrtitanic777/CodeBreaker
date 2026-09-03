; isapnp_pcode_02.asm -- ISAPNP object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  056h, 0B8h, 013h, 000h, 000h, 000h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h, 075h, 054h, 033h
        db  0F6h, 089h, 00Dh, 000h, 000h, 000h, 000h, 056h, 056h, 068h, 000h, 000h, 000h, 000h, 0E8h, 0D1h
        db  013h, 000h, 000h, 083h, 0C4h, 00Ch, 0A1h, 000h, 000h, 000h, 000h, 056h, 068h, 000h, 000h, 000h
        db  000h, 050h, 0E8h, 0B5h, 013h, 000h, 000h, 083h, 0C4h, 00Ch, 0A1h, 000h, 000h, 000h, 000h, 06Ah
        db  001h, 068h, 000h, 000h, 000h, 000h, 050h, 0E8h, 098h, 013h, 000h, 000h, 083h, 0C4h, 00Ch, 0A1h
        db  000h, 000h, 000h, 000h, 06Ah, 002h, 056h, 056h, 050h, 0E8h, 07Eh, 013h, 000h, 000h, 083h, 0C4h
        db  010h, 033h, 0C0h, 05Eh, 0C3h
loc_2_0065:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 1
        je     SHORT loc_2_0085
        cmp    eax, 2
        je     SHORT loc_2_00D3
        cmp    eax, 4
        je     SHORT loc_2_00C9
        mov    eax, 1
        jmp    SHORT loc_2_00DE
loc_2_0085:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_00D3
        push   2
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_1404
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_00D3
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   2
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_00D3
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 4]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_00DC
loc_2_00C9:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_00D3:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_00DC:
        xor    eax, eax
loc_2_00DE:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_00E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        lea    eax, [ebp - 10h]
        push   esi
        push   edi
        push   2
        push   ecx
        push   eax
        call   sub_2_1404
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_03C4
        mov    eax, dword ptr [ebp - 10h]
        mov    bl, 40h
        mov    esi, 279h
        mov    edi, 0a79h
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 4], eax
loc_2_011B:
        cmp    dword ptr [ebp - 0ch], 4
        jae    NEAR PTR loc_2_01C0
loc_2_0125:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   1
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_0181
        mov    ecx, dword ptr [ebp - 4]
        test   byte ptr [ecx + 0ch], 2
        jne    SHORT loc_2_0125
        test   eax, eax
        jne    SHORT loc_2_0181
        mov    eax, dword ptr [ebp - 4]
        mov    edx, esi
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        shr    ecx, 8
        shr    eax, 8
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebx + 2]
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        test   al, 1
        jne    SHORT loc_2_017C
        sub    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        not    eax
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_2_0186
loc_2_017C:
        inc    dword ptr [ebp - 8]
        jmp    SHORT loc_2_0186
loc_2_0181:
        xor    ecx, ecx
        mov    dword ptr [ebp - 8], ecx
loc_2_0186:
        lea    eax, [ebx + 1]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        mov    al, bl
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        mov    cx, word ptr [ebp - 8]
        lea    eax, [ebx + 4]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        lea    eax, [ebx + 3]
        mov    edx, esi
        add    bl, 8
        inc    dword ptr [ebp - 0ch]
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        jmp    NEAR PTR loc_2_011B
loc_2_01C0:
        mov    eax, dword ptr [ebp - 10h]
        mov    esi, 279h
        mov    edi, 0a79h
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0ch], 0
        mov    byte ptr [ebp - 18h], 70h
loc_2_01DB:
        cmp    dword ptr [ebp - 0ch], 4
        jae    NEAR PTR loc_2_02CE
loc_2_01E5:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   1
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_0207
        mov    ecx, dword ptr [ebp - 4]
        test   byte ptr [ecx + 0ch], 2
        je     SHORT loc_2_01E5
loc_2_0207:
        cmp    dword ptr [ebp - 0ch], 1
        sbb    bl, bl
        and    bl, 6
        add    bl, byte ptr [ebp - 18h]
        test   eax, eax
        jne    SHORT loc_2_0247
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax + 8]
        lea    eax, [ebx + 4]
        mov    dword ptr [ebp - 8], edx
        mov    edx, esi
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        test   al, 1
        jne    SHORT loc_2_0242
        sub    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        not    eax
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_2_024C
loc_2_0242:
        inc    dword ptr [ebp - 8]
        jmp    SHORT loc_2_024C
loc_2_0247:
        xor    ecx, ecx
        mov    dword ptr [ebp - 8], ecx
loc_2_024C:
        lea    eax, [ebx + 3]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        lea    eax, [ebx + 2]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        lea    eax, [ebx + 1]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    dword ptr [ebp - 14h], ecx
        shr    dword ptr [ebp - 14h], 10h
        mov    al, byte ptr [ebp - 14h]
        out    dx, al
        mov    edx, esi
        mov    al, bl
        out    dx, al
        mov    edx, edi
        mov    dword ptr [ebp - 14h], ecx
        shr    dword ptr [ebp - 14h], 10h
        mov    cx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 13h]
        out    dx, al
        lea    eax, [ebx + 8]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        lea    eax, [ebx + 7]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        lea    eax, [ebx + 6]
        mov    edx, esi
        out    dx, al
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, edi
        shr    ecx, 10h
        mov    al, cl
        out    dx, al
        lea    eax, [ebx + 5]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        movzx  eax, byte ptr [ebp - 18h]
        add    al, 10h
        inc    dword ptr [ebp - 0ch]
        mov    byte ptr [ebp - 18h], al
        jmp    NEAR PTR loc_2_01DB
loc_2_02CE:
        mov    eax, dword ptr [ebp - 10h]
        mov    bl, 60h
        mov    esi, 8
        mov    edi, 279h
        mov    dword ptr [ebp - 4], eax
loc_2_02E0:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   2
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 14h
        mov    ecx, 0
        test   eax, eax
        jne    SHORT loc_2_0305
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, word ptr [eax + 4]
loc_2_0305:
        lea    eax, [ebx + 1]
        mov    edx, edi
        out    dx, al
        mov    edx, 0a79h
        mov    al, cl
        out    dx, al
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    edx, 0a79h
        mov    al, ch
        out    dx, al
        add    bl, 2
        dec    esi
        jne    SHORT loc_2_02E0
        mov    eax, dword ptr [ebp - 10h]
        mov    bl, 70h
        mov    esi, 2
        mov    edi, 279h
        mov    dword ptr [ebp - 4], eax
loc_2_0338:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   4
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 14h
        mov    ecx, 0
        test   eax, eax
        jne    SHORT loc_2_035D
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, word ptr [eax + 2]
loc_2_035D:
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    edx, 0a79h
        mov    al, cl
        out    dx, al
        lea    eax, [ebx + 1]
        mov    edx, edi
        out    dx, al
        mov    edx, 0a79h
        mov    al, 2
        out    dx, al
        add    bl, al
        dec    esi
        jne    SHORT loc_2_0338
        mov    eax, dword ptr [ebp - 10h]
        mov    bl, 74h
        mov    esi, 2
        mov    dword ptr [ebp - 4], eax
loc_2_038A:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   3
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 14h
        mov    ecx, 4
        test   eax, eax
        jne    SHORT loc_2_03AF
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, byte ptr [eax + 1]
loc_2_03AF:
        mov    edx, 279h
        mov    al, bl
        out    dx, al
        mov    edx, 0a79h
        mov    al, cl
        out    dx, al
        inc    bl
        dec    esi
        jne    SHORT loc_2_038A
loc_2_03C4:
        mov    edx, 279h
        mov    al, 30h
        out    dx, al
        mov    edx, 0a79h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret
sub_2_03DB:
        push   ebx
        push   esi
        mov    bl, 70h
        mov    esi, 2
loc_2_03E4:
        mov    edx, 279h
        mov    al, bl
        out    dx, al
        mov    edx, 0a79h
        xor    al, al
        out    dx, al
        lea    eax, [ebx + 1]
        mov    edx, 279h
        out    dx, al
        mov    edx, 0a79h
        mov    al, 2
        out    dx, al
        add    bl, al
        dec    esi
        jne    SHORT loc_2_03E4
        mov    edx, 279h
        mov    al, 30h
        out    dx, al
        mov    edx, 0a79h
        xor    al, al
        out    dx, al
        push   1
        push   100000h
        push   ecx
        call   sub_2_140C
        add    esp, 0ch
        pop    esi
        pop    ebx
        ret
sub_2_042D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0c8h
        cmp    byte ptr [ebp + 8], 1
        push   ebx
        push   esi
        mov    bl, dl
        mov    esi, ecx
        jne    SHORT loc_2_048F
        push   0
        lea    eax, [ebp - 0c8h]
        push   0c8h
        push   eax
        push   esi
        call   sub_2_1424
        add    esp, 10h
        xor    eax, eax
loc_2_045B:
        cmp    byte ptr [ebp + eax - 0c8h], 5ch
        je     SHORT loc_2_0468
        inc    eax
        jmp    SHORT loc_2_045B
loc_2_0468:
        xor    ecx, ecx
        push   ecx
        mov    byte ptr [ebp + eax - 0c0h], cl
        lea    eax, [ebp + eax - 0c8h]
        push   ecx
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax], 2ah
        push   ecx
        push   esi
        call   sub_2_141C
        add    esp, 1ch
loc_2_048F:
        call   sub_2_0917
        movzx  eax, bl
        push   1
        push   eax
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 9
        push   eax
        call   sub_2_1414
        add    esp, 10h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_04B4:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        cmp    eax, dword ptr [ebp + 14h]
        push   esi
        push   edi
        jne    SHORT loc_2_0543
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     SHORT loc_2_04E8
        cmp    eax, 2
        je     SHORT loc_2_050E
        cmp    eax, 4
        je     SHORT loc_2_052A
        cmp    eax, 5
        je     SHORT loc_2_0539
        mov    eax, 1
        jmp    NEAR PTR loc_2_0644
loc_2_04E8:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   1
        push   1
        call   sub_2_1444
        add    esp, 20h
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
        jmp    NEAR PTR loc_2_0644
loc_2_050E:
        xor    esi, esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        call   sub_2_143C
        add    esp, 8
        db  089h, 035h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_0642
loc_2_052A:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_0642
loc_2_0539:
        call   sub_2_1032
        jmp    NEAR PTR loc_2_0642
loc_2_0543:
        push   0
        lea    edx, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 14h]
        push   edx
        call   sub_2_1434
        add    esp, 10h
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0565
        mov    eax, 1
        jmp    NEAR PTR loc_2_0644
loc_2_0565:
        mov    al, byte ptr [ebp - 8]
        mov    bl, byte ptr [ebp - 7]
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 10h
        mov    byte ptr [ebp - 5], al
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        jb     SHORT loc_2_058E
        cmp    eax, 2
        jbe    SHORT loc_2_0598
        cmp    eax, 4
        je     SHORT loc_2_0598
        cmp    eax, 6
        je     SHORT loc_2_0598
loc_2_058E:
        mov    eax, 1
        jmp    NEAR PTR loc_2_0644
loc_2_0598:
        mov    esi, 279h
        call   sub_2_07E1
        mov    edx, esi
        mov    al, 2
        mov    edi, 0a79h
        push   0
        out    dx, al
        mov    edx, edi
        push   7d0h
        out    dx, al
        call   sub_2_142C
        add    esp, 8
        call   sub_2_07E1
        mov    edx, esi
        mov    al, 3
        out    dx, al
        mov    edx, edi
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     SHORT loc_2_05F1
        cmp    eax, 2
        je     SHORT loc_2_060C
        cmp    eax, 4
        je     SHORT loc_2_060C
        cmp    eax, 6
        je     SHORT loc_2_0616
        jmp    SHORT loc_2_0634
loc_2_05F1:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_0602
        mov    eax, 13h
        jmp    SHORT loc_2_0644
loc_2_0602:
        mov    ecx, dword ptr [ebp + 14h]
        call   sub_2_00E2
        jmp    SHORT loc_2_0634
loc_2_060C:
        mov    ecx, dword ptr [ebp + 14h]
        call   sub_2_03DB
        jmp    SHORT loc_2_0634
loc_2_0616:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_0627
        mov    eax, 13h
        jmp    SHORT loc_2_0644
loc_2_0627:
        mov    edx, ebx
        mov    ecx, dword ptr [ebp + 14h]
        push   dword ptr [ebp - 5]
        call   sub_2_042D
loc_2_0634:
        mov    edx, 279h
        mov    al, 2
        out    dx, al
        mov    edx, 0a79h
        out    dx, al
loc_2_0642:
        xor    eax, eax
loc_2_0644:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_064B:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        cmp    dword ptr [ebp + 10h], eax
        push   esi
        push   edi
        jne    SHORT loc_2_0668
        mov    eax, 13h
        jmp    NEAR PTR loc_2_07C3
loc_2_0668:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_067C
        mov    eax, 13h
        jmp    NEAR PTR loc_2_07C3
loc_2_067C:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    edx, [ebp - 4]
        push   dword ptr [ebp + 10h]
        push   edx
        call   sub_2_1434
        add    esp, 10h
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_06A3
        mov    eax, 13h
        jmp    NEAR PTR loc_2_07C3
loc_2_06A3:
        mov    esi, 279h
        call   sub_2_07E1
        mov    edx, esi
        mov    al, 2
        mov    edi, 0a79h
        push   0
        out    dx, al
        mov    edx, edi
        push   7d0h
        out    dx, al
        call   sub_2_142C
        add    esp, 8
        call   sub_2_07E1
        mov    edx, esi
        mov    al, 3
        out    dx, al
        mov    edx, edi
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    edx, edi
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_2_0703
        cmp    eax, 1
        je     SHORT loc_2_0745
        cmp    eax, 2
        je     NEAR PTR loc_2_077E
        mov    ecx, 20h
        jmp    NEAR PTR loc_2_07B3
loc_2_0703:
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, ebx
        cmp    eax, 100h
        jbe    SHORT loc_2_071C
        mov    ecx, 13h
        jmp    NEAR PTR loc_2_07B3
loc_2_071C:
        test   ebx, ebx
        je     NEAR PTR loc_2_07B1
        mov    edi, dword ptr [ebp + 14h]
loc_2_0727:
        mov    edx, 279h
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        dec    ebx
        inc    dword ptr [ebp + 0ch]
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        mov    byte ptr [edi], al
        inc    edi
        test   ebx, ebx
        jne    SHORT loc_2_0727
        jmp    SHORT loc_2_07B1
loc_2_0745:
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, ebx
        cmp    eax, 100h
        jbe    SHORT loc_2_075B
        mov    ecx, 13h
        jmp    SHORT loc_2_07B3
loc_2_075B:
        test   ebx, ebx
        je     SHORT loc_2_07B1
        mov    edi, dword ptr [ebp + 14h]
loc_2_0762:
        mov    edx, 279h
        mov    al, byte ptr [ebp + 0ch]
        dec    ebx
        inc    dword ptr [ebp + 0ch]
        out    dx, al
        mov    edx, 0a79h
        mov    al, byte ptr [edi]
        out    dx, al
        inc    edi
        test   ebx, ebx
        jne    SHORT loc_2_0762
        jmp    SHORT loc_2_07B1
loc_2_077E:
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, ebx
        mov    ecx, 13h
        cmp    eax, 1000h
        ja     SHORT loc_2_07B3
        call   sub_2_0917
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 14h]
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_2_07B1:
        xor    ecx, ecx
loc_2_07B3:
        mov    edx, 279h
        mov    al, 2
        out    dx, al
        mov    edx, 0a79h
        out    dx, al
        mov    eax, ecx
loc_2_07C3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_07CA:
        lea    eax, [ecx*2]
        add    dl, dl
        xor    al, cl
        shr    cl, 1
        and    al, 0feh
        xor    al, dl
        shl    al, 6
        or     al, cl
        ret
sub_2_07E1:
        push   ebx
        mov    cl, 6ah
        mov    edx, 279h
        xor    al, al
        out    dx, al
        out    dx, al
        mov    bl, 20h
loc_2_07EF:
        mov    edx, 279h
        mov    al, cl
        out    dx, al
        xor    edx, edx
        call   sub_2_07CA
        mov    cl, al
        dec    bl
        jne    SHORT loc_2_07EF
        pop    ebx
        ret
sub_2_0806:
        sub    esp, 4
        xor    al, al
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        push   ebp
        mov    edx, 80h
        db  08Bh, 02Dh, 000h, 000h, 000h, 000h
        lea    edi, [ebp + 9]
loc_2_0822:
        mov    ecx, edi
        sub    ecx, ebp
        cmp    ecx, 0ffdh
        jae    SHORT loc_2_087A
        mov    cl, byte ptr [edi]
        mov    bl, cl
        and    bl, dl
        cmp    bl, dl
        jne    SHORT loc_2_0841
        movzx  ebx, word ptr [edi + 1]
        add    ebx, 3
        jmp    SHORT loc_2_0849
loc_2_0841:
        xor    ebx, ebx
        mov    bl, cl
        and    ebx, 7
        inc    ebx
loc_2_0849:
        mov    dword ptr [esp + 10h], ebx
        mov    bl, cl
        and    bl, dl
        cmp    bl, dl
        je     SHORT loc_2_0858
        and    cl, 0f8h
loc_2_0858:
        test   cl, cl
        je     SHORT loc_2_087A
        cmp    cl, 0ffh
        je     SHORT loc_2_087A
        cmp    cl, 78h
        je     SHORT loc_2_087A
        cmp    cl, 10h
        jne    SHORT loc_2_086D
        inc    al
loc_2_086D:
        add    edi, dword ptr [esp + 10h]
        inc    esi
        cmp    esi, 100h
        jbe    SHORT loc_2_0822
loc_2_087A:
        test   al, al
        jne    SHORT loc_2_0880
        inc    al
loc_2_0880:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_2_0888:
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_1454
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        shr    eax, 0ch
        push   0
        push   1
        push   eax
        call   sub_2_144C
        add    esp, 0ch
        cli
        ret
loc_2_08B3:
        sti
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_1464
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        shr    eax, 0ch
        push   0
        push   1
        push   eax
        call   sub_2_145C
        add    esp, 0ch
        ret
sub_2_08DE:
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        mov    esi, 1000h
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
loc_2_08F2:
        mov    edx, 279h
        mov    al, 5
        out    dx, al
loc_2_08FA:
        mov    dx, cx
        in     al, dx
        test   al, 1
        je     SHORT loc_2_08FA
        mov    edx, 279h
        mov    al, 4
        out    dx, al
        mov    dx, cx
        in     al, dx
        mov    byte ptr [edi], al
        inc    edi
        dec    esi
        jne    SHORT loc_2_08F2
        pop    edi
        pop    esi
        ret
sub_2_0917:
        call   sub_2_0888
        call   sub_2_08DE
        jmp    NEAR PTR loc_2_08B3
loc_2_0926:
        nop
        ret
sub_2_0928:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        mov    word ptr [ebp - 26h], 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        xor    ebx, ebx
        mov    word ptr [ebp - 28h], bx
        mov    byte ptr [ebp - 1eh], bl
        mov    word ptr [ebp - 24h], bx
        mov    byte ptr [ebp - 1dh], bl
        mov    word ptr [ebp - 22h], bx
        mov    dword ptr [ebp - 38h], ebx
        mov    word ptr [ebp - 20h], bx
        mov    dword ptr [ebp - 34h], ebx
        mov    word ptr [ebp - 3ah], 14h
        mov    word ptr [ebp - 3ch], bx
        mov    word ptr [ebp - 30h], bx
        mov    word ptr [ebp - 2eh], bx
        mov    byte ptr [ebp - 10h], bl
        mov    word ptr [ebp - 44h], bx
        mov    byte ptr [ebp - 0fh], bl
        mov    word ptr [ebp - 42h], bx
        mov    byte ptr [ebp - 0eh], bl
        mov    word ptr [ebp - 40h], bx
        mov    edx, 279h
        mov    word ptr [ebp - 3eh], bx
        mov    al, 30h
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 0dh], bl
        in     al, dx
        test   al, 1
        je     NEAR PTR loc_2_0D87
        push   ebx
        lea    eax, [ebp - 14h]
        push   esi
        push   eax
        call   sub_2_1404
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_0D87
        push   3
        lea    eax, [ebp - 0ch]
        push   1
        push   esi
        push   eax
        call   sub_2_147C
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_0D87
        mov    eax, dword ptr [ebp - 14h]
        xor    esi, esi
        mov    byte ptr [ebp - 5], 40h
        mov    dword ptr [ebp - 4], eax
loc_2_09D8:
        mov    edx, 279h
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        movzx  edi, al
        shl    edi, 8
        mov    al, byte ptr [ebp - 5]
        inc    al
        mov    edx, 279h
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        movzx  eax, al
        or     edi, eax
        shl    edi, 8
        test   edi, edi
        je     NEAR PTR loc_2_0AB6
loc_2_0A12:
        push   0
        lea    eax, [ebp - 2ch]
        push   eax
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_13FC
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_0A2F
        xor    edi, edi
loc_2_0A2F:
        test   edi, edi
        je     SHORT loc_2_0A68
        mov    eax, dword ptr [ebp - 2ch]
        and    ah, 7fh
        cmp    eax, 1
        jne    SHORT loc_2_0A12
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 20h], 2
        jne    SHORT loc_2_0A12
        test   edi, edi
        je     SHORT loc_2_0AB6
        push   0
        lea    ecx, [ebp - 18h]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_1474
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0A68
        cmp    dword ptr [ebp - 18h], 24h
        jae    SHORT loc_2_0A68
        xor    edi, edi
loc_2_0A68:
        test   edi, edi
        je     SHORT loc_2_0AB6
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 38h], edi
        add    ecx, 10h
        push   0
        push   10h
        mov    eax, dword ptr [ecx + 4]
        lea    edx, [edi + eax - 1]
        lea    eax, [ebp - 3ch]
        push   eax
        mov    dword ptr [ebp - 34h], edx
        mov    cx, word ptr [ecx + 10h]
        push   1
        push   dword ptr [ebp - 0ch]
        mov    word ptr [ebp - 30h], cx
        lea    ecx, [ebp - 1ch]
        push   ecx
        call   sub_2_146C
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_2_0D87
        add    byte ptr [ebp - 5], 8
        inc    esi
        cmp    esi, 4
        jb     NEAR PTR loc_2_09D8
loc_2_0AB6:
        mov    eax, dword ptr [ebp - 14h]
        mov    bl, 70h
        mov    edi, 279h
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
loc_2_0ACA:
        cmp    dword ptr [ebp - 8], 1
        sbb    cl, cl
        mov    edx, edi
        and    cl, 6
        add    cl, bl
        mov    al, cl
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        movzx  esi, al
        shl    esi, 8
        lea    eax, [ecx + 1]
        mov    edx, edi
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        movzx  eax, al
        or     esi, eax
        mov    edx, edi
        shl    esi, 8
        lea    eax, [ecx + 2]
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        movzx  eax, al
        or     esi, eax
        mov    edx, edi
        shl    esi, 8
        lea    eax, [ecx + 3]
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        movzx  eax, al
        or     esi, eax
        je     NEAR PTR loc_2_0BD6
loc_2_0B2D:
        push   0
        lea    eax, [ebp - 2ch]
        push   eax
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_13FC
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_0B4A
        xor    esi, esi
loc_2_0B4A:
        test   esi, esi
        je     SHORT loc_2_0B83
        mov    eax, dword ptr [ebp - 2ch]
        and    ah, 7fh
        cmp    eax, 1
        jne    SHORT loc_2_0B2D
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_2_0B2D
        test   esi, esi
        je     SHORT loc_2_0BD6
        push   0
        lea    ecx, [ebp - 18h]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_1474
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0B83
        cmp    dword ptr [ebp - 18h], 24h
        jae    SHORT loc_2_0B83
        xor    esi, esi
loc_2_0B83:
        test   esi, esi
        je     SHORT loc_2_0BD6
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 38h], esi
        add    ecx, 10h
        push   0
        push   10h
        mov    eax, dword ptr [ecx + 4]
        lea    edx, [esi + eax - 1]
        lea    eax, [ebp - 3ch]
        push   eax
        mov    dword ptr [ebp - 34h], edx
        mov    cx, word ptr [ecx + 10h]
        push   1
        push   dword ptr [ebp - 0ch]
        mov    word ptr [ebp - 30h], cx
        lea    ecx, [ebp - 1ch]
        push   ecx
        call   sub_2_146C
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_2_0D87
        movzx  ebx, bl
        add    bl, 10h
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 4
        jb     NEAR PTR loc_2_0ACA
loc_2_0BD6:
        mov    bl, 60h
        mov    eax, dword ptr [ebp - 14h]
        xor    esi, esi
        mov    dword ptr [ebp - 4], eax
loc_2_0BE0:
        mov    edx, 279h
        mov    al, bl
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        movzx  di, al
        shl    di, 8
        lea    eax, [ebx + 1]
        mov    edx, 279h
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        movzx  ax, al
        or     di, ax
        je     NEAR PTR loc_2_0CAB
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   2
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_0C4A
        push   0
        lea    ecx, [ebp - 18h]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_1474
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0C4D
        cmp    dword ptr [ebp - 18h], 18h
        jae    SHORT loc_2_0C4D
loc_2_0C4A:
        xor    di, di
loc_2_0C4D:
        test   di, di
        je     SHORT loc_2_0CAB
        mov    word ptr [ebp - 24h], di
        mov    eax, dword ptr [ebp - 4]
        mov    cx, word ptr [eax + 0eh]
        add    eax, 0ch
        dec    cx
        push   0
        add    di, cx
        push   0ch
        mov    word ptr [ebp - 22h], di
        mov    dl, byte ptr [eax + 0ah]
        mov    byte ptr [ebp - 1eh], dl
        lea    edx, [ebp - 28h]
        mov    cl, byte ptr [eax + 0bh]
        push   edx
        push   2
        mov    byte ptr [ebp - 1dh], cl
        mov    ax, word ptr [eax + 8]
        push   dword ptr [ebp - 0ch]
        mov    word ptr [ebp - 20h], ax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_2_146C
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_2_0D87
        add    bl, 2
        inc    esi
        cmp    esi, 8
        jb     NEAR PTR loc_2_0BE0
loc_2_0CAB:
        mov    eax, dword ptr [ebp - 14h]
        xor    esi, esi
        mov    byte ptr [ebp - 5], 70h
        mov    dword ptr [ebp - 4], eax
loc_2_0CB7:
        mov    edx, 279h
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        mov    bl, al
        test   bl, bl
        je     SHORT loc_2_0D1C
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   4
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_0CE9
        xor    bl, bl
loc_2_0CE9:
        test   bl, bl
        je     SHORT loc_2_0D1C
        push   0
        lea    ecx, [ebp - 44h]
        movzx  ax, bl
        mov    word ptr [ebp - 42h], ax
        push   8
        push   ecx
        lea    eax, [ebp - 1ch]
        push   4
        push   dword ptr [ebp - 0ch]
        push   eax
        call   sub_2_146C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_2_0D87
        add    byte ptr [ebp - 5], 2
        inc    esi
        cmp    esi, 2
        jb     SHORT loc_2_0CB7
loc_2_0D1C:
        mov    eax, dword ptr [ebp - 14h]
        xor    esi, esi
        xor    edi, edi
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [ebp - 5], 74h
loc_2_0D2A:
        mov    edx, 279h
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        cmp    al, 4
        mov    bl, al
        je     SHORT loc_2_0D87
        push   edi
        lea    ecx, [ebp - 4]
        push   edi
        push   3
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_0D5A
        mov    bl, 4
loc_2_0D5A:
        cmp    bl, 4
        je     SHORT loc_2_0D87
        push   edi
        lea    eax, [ebp - 10h]
        push   4
        lea    edx, [ebp - 1ch]
        push   eax
        mov    byte ptr [ebp - 0fh], bl
        push   3
        push   dword ptr [ebp - 0ch]
        push   edx
        call   sub_2_146C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_2_0D87
        inc    esi
        inc    byte ptr [ebp - 5]
        cmp    esi, 2
        jb     SHORT loc_2_0D2A
loc_2_0D87:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0D8E:
        push   ebp
        mov    edx, 279h
        mov    ebp, esp
        mov    al, 3
        out    dx, al
        sub    esp, 0e0h
        mov    edx, 0a79h
        mov    al, cl
        mov    byte ptr [ebp - 2], cl
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        out    dx, al
        call   sub_2_0917
        call   sub_2_0806
        mov    byte ptr [ebp - 1], al
        cmp    al, bl
        je     NEAR PTR loc_2_0F16
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_2_0DCA:
        mov    edx, 279h
        mov    al, 7
        out    dx, al
        mov    edx, 0a79h
        mov    al, bl
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        cmp    al, bl
        jne    NEAR PTR loc_2_0F16
        cmp    byte ptr [ebp - 1], 1
        jbe    SHORT loc_2_0E0D
        movzx  eax, bl
        push   eax
        lea    ecx, [ebp - 18h]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_149C
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_0E11
loc_2_0E0D:
        mov    byte ptr [ebp - 18h], 0
loc_2_0E11:
        push   dword ptr [ecx + 4]
        movzx  eax, byte ptr [ecx + 3]
        mov    dl, byte ptr [ecx + 1]
        lea    esi, [ebp - 18h]
        push   esi
        push   eax
        movzx  esi, byte ptr [ecx + 2]
        push   esi
        xor    eax, eax
        mov    al, dl
        shr    dl, 5
        and    eax, 1fh
        add    eax, 40h
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx]
        and    eax, 3
        movzx  edx, dl
        lea    eax, [edx + eax*8 + 40h]
        push   eax
        mov    al, byte ptr [ecx]
        shr    al, 2
        lea    ecx, [ebp - 0e0h]
        movzx  eax, al
        add    eax, 40h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_149C
        add    esp, 24h
        lea    edx, [ebp - 0e0h]
        lea    eax, [ebp - 8]
        push   0
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   edx
        push   eax
        call   sub_2_1494
        add    esp, 10h
        mov    esi, eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_2_0E8E
        cmp    esi, 10h
        jne    SHORT loc_2_0F0B
loc_2_0E8E:
        movzx  edi, bl
        movzx  eax, byte ptr [ebp - 1]
        movzx  ecx, byte ptr [ebp - 2]
        shl    eax, 8
        push   0
        add    eax, edi
        shl    eax, 8
        add    eax, ecx
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_2_148C
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    esp, 10h
        mov    word ptr [ebp - 0ah], dx
        mov    cl, byte ptr [ebp - 2]
        push   0
        mov    byte ptr [ebp - 0ch], cl
        lea    ecx, [ebp - 0ch]
        mov    byte ptr [ebp - 0bh], bl
        push   ecx
        push   4
        push   10h
        push   dword ptr [ebp - 8]
        call   sub_2_1484
        add    esp, 14h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    esi, 10h
        je     SHORT loc_2_0F0B
        push   0
        add    ecx, 9
        push   edi
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_1414
        add    esp, 10h
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0928
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_2_0F0B:
        inc    bl
        cmp    byte ptr [ebp - 1], bl
        ja     NEAR PTR loc_2_0DCA
loc_2_0F16:
        mov    edx, 279h
        mov    al, 3
        out    dx, al
        mov    edx, 0a79h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret
sub_2_0F2D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    byte ptr [ebp - 2], cl
        push   esi
        xor    eax, eax
        push   edi
        mov    edx, 279h
        mov    byte ptr [ebp - 1], 6ah
        lea    edi, [ebp - 0bh]
        push   0
        mov    byte ptr [ebp - 0ch], 0
        stosd
        stosd
        mov    al, 1
        push   2eeh
        out    dx, al
        xor    bl, bl
        call   sub_2_142C
        mov    byte ptr [ebp - 3], 0
        add    esp, 8
loc_2_0F65:
        push   0
        push   0fah
        call   sub_2_142C
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    esp, 8
        in     al, dx
        cmp    al, 55h
        mov    cl, al
        in     al, dx
        jne    SHORT loc_2_0F89
        mov    dl, 1
        cmp    al, 0aah
        je     SHORT loc_2_0F8B
loc_2_0F89:
        xor    dl, dl
loc_2_0F8B:
        mov    al, bl
        mov    cl, bl
        shr    al, 3
        and    cl, 7
        movzx  esi, al
        mov    al, dl
        or     byte ptr [ebp - 3], dl
        shl    al, cl
        or     byte ptr [ebp + esi - 0ch], al
        cmp    bl, 40h
        jae    SHORT loc_2_0FB3
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_07CA
        mov    byte ptr [ebp - 1], al
loc_2_0FB3:
        inc    bl
        cmp    bl, 48h
        jb     SHORT loc_2_0F65
        cmp    byte ptr [ebp - 3], 0
        je     SHORT loc_2_0FD2
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 4], al
        je     SHORT loc_2_0FCE
        cmp    byte ptr [ebp - 4], 0
        jne    SHORT loc_2_0FD2
loc_2_0FCE:
        mov    bl, 1
        jmp    SHORT loc_2_0FD4
loc_2_0FD2:
        xor    bl, bl
loc_2_0FD4:
        push   0
        push   0fah
        call   sub_2_142C
        add    esp, 8
        test   bl, bl
        je     SHORT loc_2_1028
        mov    edx, 279h
        mov    al, 6
        out    dx, al
        mov    edx, 0a79h
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        cmp    al, byte ptr [ebp - 2]
        je     SHORT loc_2_1009
        xor    bl, bl
        jmp    SHORT loc_2_1028
loc_2_1009:
        push   0
        push   0fah
        call   sub_2_142C
        add    esp, 8
        mov    edx, 279h
        mov    al, 3
        out    dx, al
        mov    edx, 0a79h
        xor    al, al
        out    dx, al
loc_2_1028:
        movzx  eax, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1032:
        push   ebp
        mov    ebp, esp
        sub    esp, 0fch
        mov    byte ptr [ebp - 5], 1
        push   ebx
        push   esi
        xor    esi, esi
        push   1
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_11A7
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_1494
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_13BB
        mov    word ptr [ebp - 2], si
        lea    eax, [ebp - 14h]
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   2
        call   sub_2_14E4
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_10BD
        lea    eax, [ebp - 20h]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 24h]
        mov    dword ptr [ebp - 20h], 2
        push   ecx
        push   edx
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 14h]
        call   sub_2_14DC
        add    esp, 18h
        push   dword ptr [ebp - 14h]
        call   sub_2_14D4
        add    esp, 4
loc_2_10BD:
        xor    ebx, ebx
        cmp    word ptr [ebp - 2], bx
        jne    SHORT loc_2_1115
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   2000h
        lea    ecx, [ebp - 0ch]
        push   eax
        push   ecx
        call   sub_2_147C
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_13BB
loc_2_10E5:
        movzx  eax, bl
        imul   eax, eax, 18h
        push   0
        db  005h, 000h, 000h, 000h, 000h
        push   18h
        push   eax
        push   2
        push   dword ptr [ebp - 0ch]
        push   0
        call   sub_2_146C
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_2_13BB
        inc    bl
        cmp    bl, 4
        jb     SHORT loc_2_10E5
        jmp    SHORT loc_2_111F
loc_2_1115:
        mov    ax, word ptr [ebp - 2]
        db  066h, 0A3h, 010h, 000h, 000h, 000h
loc_2_111F:
        xor    esi, esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        lea    ecx, [ebp - 0ch]
        push   3000h
        push   eax
        push   ecx
        call   sub_2_147C
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_13BB
        push   esi
        push   18h
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   dword ptr [ebp - 0ch]
        push   esi
        call   sub_2_146C
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_2_13BB
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   5000h
        lea    ecx, [ebp - 0ch]
        push   eax
        push   ecx
        call   sub_2_147C
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_13BB
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        call   sub_2_13E4
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        push   3
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_13DC
        add    esp, 10h
        jmp    SHORT loc_2_11B5
loc_2_11A7:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_14CC
        add    esp, 8
loc_2_11B5:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    ecx, [ebp - 0fch]
        push   ecx
        call   sub_2_14C4
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_13BB
        cmp    word ptr [ebp - 0a0h], 0
        je     NEAR PTR loc_2_13BB
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_14BC
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        push   1
        push   200h
        push   eax
        call   sub_2_140C
        add    esp, 0ch
        call   sub_2_07E1
        mov    edx, 279h
        mov    al, 2
        out    dx, al
        mov    edx, 0a79h
        mov    al, 6
        push   0
        xor    bl, bl
        out    dx, al
        push   7d0h
        call   sub_2_142C
        add    esp, 8
        call   sub_2_07E1
        cmp    word ptr [ebp - 0a0h], 0
        jbe    SHORT loc_2_12B8
        mov    esi, 279h
loc_2_1241:
        movzx  eax, bl
        mov    ax, word ptr [ebp + eax*2 - 9eh]
        mov    edx, esi
        add    ax, 3
        push   0
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        push   3e8h
        mov    al, 3
        out    dx, al
        mov    edx, 0a79h
        xor    al, al
        out    dx, al
        call   sub_2_142C
        add    esp, 8
        mov    edx, esi
        xor    al, al
        mov    ecx, dword ptr [ebp - 5]
        out    dx, al
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    edx, 0a79h
        shr    ax, 2
        out    dx, al
        call   sub_2_0F2D
        test   eax, eax
        je     SHORT loc_2_12A2
loc_2_1293:
        inc    byte ptr [ebp - 5]
        mov    ecx, dword ptr [ebp - 5]
        call   sub_2_0F2D
        test   eax, eax
        jne    SHORT loc_2_1293
loc_2_12A2:
        cmp    byte ptr [ebp - 5], 1
        ja     SHORT loc_2_12B8
        inc    bl
        movzx  ecx, word ptr [ebp - 0a0h]
        movzx  eax, bl
        cmp    eax, ecx
        jb     SHORT loc_2_1241
loc_2_12B8:
        mov    bl, 1
        cmp    byte ptr [ebp - 5], bl
        jbe    SHORT loc_2_12CD
loc_2_12BF:
        mov    ecx, ebx
        inc    bl
        call   sub_2_0D8E
        cmp    bl, byte ptr [ebp - 5]
        jb     SHORT loc_2_12BF
loc_2_12CD:
        mov    edx, 279h
        mov    al, 2
        out    dx, al
        mov    edx, 0a79h
        xor    esi, esi
        out    dx, al
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_14B4
        add    esp, 8
        cmp    byte ptr [ebp - 5], 1
        jne    SHORT loc_2_12F9
        db  066h, 089h, 035h, 000h, 000h, 000h, 000h
loc_2_12F9:
        xor    esi, esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        lea    ecx, [ebp - 10h]
        push   eax
        push   esi
        push   ecx
        call   sub_2_1494
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_13BB
        push   2
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    ecx, [ebp - 1ch]
        push   ecx
        call   sub_2_1404
        add    esp, 0ch
        lea    edx, [ebp - 18h]
        push   2
        push   3000h
        push   dword ptr [ebp - 10h]
        push   edx
        call   sub_2_147C
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_13BB
        mov    eax, dword ptr [ebp - 1ch]
        push   esi
        push   esi
        lea    ecx, [ebp - 8]
        push   2
        mov    dword ptr [ebp - 8], eax
        push   eax
        push   ecx
        call   sub_2_13FC
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_13AE
loc_2_1360:
        mov    eax, dword ptr [ebp - 8]
        db  00Fh, 0B7h, 015h, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [eax + 4]
        sub    ecx, edx
        cmp    ecx, -3
        je     SHORT loc_2_1397
        push   esi
        push   0ch
        push   eax
        push   2
        push   dword ptr [ebp - 18h]
        push   esi
        call   sub_2_146C
        add    esp, 18h
        lea    edx, [ebp - 8]
        push   esi
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_2_14AC
        add    esp, 0ch
loc_2_1397:
        push   esi
        lea    ecx, [ebp - 8]
        push   esi
        push   2
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_13FC
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_1360
loc_2_13AE:
        push   0
        push   dword ptr [ebp - 10h]
        call   sub_2_14A4
        add    esp, 8
loc_2_13BB:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_13C1:
        cmp    dword ptr [esp + 8], 1
        mov    eax, 1
        jne    SHORT loc_2_13D6
        mov    ecx, dword ptr [esp + 4]
        call   0
loc_2_13D6:
        ret    8
        db  0CCh, 0CCh, 0CCh
sub_2_13DC:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        int3
        int3
sub_2_13E4:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        int3
        int3
        db  0CDh, 020h, 053h, 080h, 033h, 000h
        int3
        int3
        db  0CDh, 020h, 002h, 080h, 033h, 000h
        int3
        int3
sub_2_13FC:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_2_1404:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        int3
        int3
sub_2_140C:
        db  0CDh, 020h, 05Bh, 080h, 033h, 000h
        int3
        int3
sub_2_1414:
        db  0CDh, 020h, 038h, 080h, 033h, 000h
        int3
        int3
sub_2_141C:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        int3
        int3
sub_2_1424:
        db  0CDh, 020h, 007h, 080h, 033h, 000h
        int3
        int3
sub_2_142C:
        db  0CDh, 020h, 015h, 080h, 033h, 000h
        int3
        int3
sub_2_1434:
        db  0CDh, 020h, 009h, 080h, 033h, 000h
        int3
        int3
sub_2_143C:
        db  0CDh, 020h, 055h, 080h, 001h, 000h
        int3
        int3
sub_2_1444:
        db  0CDh, 020h, 053h, 080h, 001h, 000h
        int3
        int3
sub_2_144C:
        db  0CDh, 020h, 063h, 080h, 001h, 000h
        int3
        int3
sub_2_1454:
        db  0CDh, 020h, 094h, 081h, 001h, 000h
        int3
        int3
sub_2_145C:
        db  0CDh, 020h, 064h, 080h, 001h, 000h
        int3
        int3
sub_2_1464:
        db  0CDh, 020h, 095h, 081h, 001h, 000h
        int3
        int3
sub_2_146C:
        db  0CDh, 020h, 01Ch, 080h, 033h, 000h
        int3
        int3
sub_2_1474:
        db  0CDh, 020h, 021h, 080h, 033h, 000h
        int3
        int3
sub_2_147C:
        db  0CDh, 020h, 018h, 080h, 033h, 000h
        int3
        int3
sub_2_1484:
        db  0CDh, 020h, 043h, 080h, 033h, 000h
        int3
        int3
sub_2_148C:
        db  0CDh, 020h, 00Ah, 080h, 033h, 000h
        int3
        int3
sub_2_1494:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        int3
        int3
sub_2_149C:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_2_14A4:
        db  0CDh, 020h, 00Dh, 080h, 033h, 000h
        int3
        int3
sub_2_14AC:
        db  0CDh, 020h, 01Eh, 080h, 033h, 000h
        int3
        int3
sub_2_14B4:
        db  0CDh, 020h, 037h, 080h, 033h, 000h
        int3
        int3
sub_2_14BC:
        db  0CDh, 020h, 035h, 080h, 033h, 000h
        int3
        int3
sub_2_14C4:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
sub_2_14CC:
        db  0CDh, 020h, 05Ch, 080h, 033h, 000h
        int3
        int3
sub_2_14D4:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_2_14DC:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_2_14E4:
        db  0CDh, 020h, 06Fh, 081h, 001h, 000h
_PTEXT ends
        end
