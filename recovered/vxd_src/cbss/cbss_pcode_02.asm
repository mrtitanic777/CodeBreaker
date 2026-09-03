; cbss_pcode_02.asm -- CBSS object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 22h], 0dea0h
        cmp    byte ptr [esi + 24h], 1
        jne    SHORT loc_2_0038
        push   0
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   8
        push   0
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 18h
        cmp    byte ptr [ebp - 1], 4
        jae    SHORT loc_2_0038
        or     byte ptr [esi + 0ah], 1
loc_2_0038:
        mov    edi, dword ptr [esi]
        xor    ebx, ebx
        push   ebx
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   0ch
        push   ebx
        push   edi
        call   sub_2_21A4
        add    esp, 18h
        cmp    byte ptr [ebp - 1], bl
        jne    SHORT loc_2_006C
        push   ebx
        lea    eax, [ebp - 1]
        mov    byte ptr [ebp - 1], 8
        push   1
        push   eax
        push   0ch
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 18h
loc_2_006C:
        push   0
        lea    eax, [ebp - 1]
        mov    byte ptr [ebp - 1], 0c0h
        push   1
        push   eax
        push   90h
        push   1
        push   edi
        call   sub_2_21A4
        mov    byte ptr [ebp - 1], 28h
        add    esp, 18h
        test   byte ptr [esi + 0ah], 1
        jne    SHORT loc_2_0096
        mov    byte ptr [ebp - 1], 38h
loc_2_0096:
        push   0
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   91h
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 18h
        mov    ecx, dword ptr [esi + 18h]
        lea    eax, [ebp - 1]
        mov    ebx, 92h
        or     byte ptr [ecx + 803h], 10h
        push   0
        push   1
        push   eax
        push   ebx
        push   0
        push   edi
        call   sub_2_21A4
        add    esp, 18h
        mov    al, byte ptr [ebp - 1]
        and    al, 6
        jne    SHORT loc_2_00F2
        push   0
        lea    eax, [ebp - 1]
        or     byte ptr [ebp - 1], 2
        push   1
        push   eax
        push   ebx
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 18h
        jmp    SHORT loc_2_0101
loc_2_00F2:
        cmp    al, 6
        jne    SHORT loc_2_0101
        mov    eax, dword ptr [esi + 8]
        or     eax, 20000h
        mov    dword ptr [esi + 8], eax
loc_2_0101:
        mov    eax, dword ptr [esi + 18h]
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax + 840h], 0
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0112:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 18h]
        test   byte ptr [esi + 0ah], 1
        mov    ecx, dword ptr [eax + 8]
        jne    NEAR PTR loc_2_01CC
        test   cl, 10h
        je     NEAR PTR loc_2_01CC
        cmp    byte ptr [ebp + 0ch], 2
        jne    SHORT loc_2_0148
        mov    dword ptr [ebp - 4], 400h
        jmp    SHORT loc_2_0177
loc_2_0148:
        cmp    byte ptr [ebp + 0ch], 3
        jne    SHORT loc_2_0157
        mov    dword ptr [ebp - 4], 800h
        jmp    SHORT loc_2_0177
loc_2_0157:
        cmp    byte ptr [ebp + 0ch], 4
        jne    SHORT loc_2_0166
        mov    dword ptr [ebp - 4], 1000h
        jmp    SHORT loc_2_0177
loc_2_0166:
        mov    al, byte ptr [ebp + 0ch]
        sub    al, 5
        cmp    al, 1
        sbb    eax, eax
        and    eax, 2000h
        mov    dword ptr [ebp - 4], eax
loc_2_0177:
        and    ecx, 3c00h
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_01CC
        test   dword ptr [ebp - 4], ecx
        jne    SHORT loc_2_01CC
        mov    eax, ecx
        not    eax
        inc    eax
        and    eax, ecx
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_2_01CC
        mov    edi, dword ptr [esi]
        push   0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ebx, 80h
        push   ebx
        push   0
        push   edi
        call   sub_2_21A4
        add    esp, 18h
        lea    eax, [ebp - 4]
        or     byte ptr [ebp - 2], 20h
        push   0
        push   4
        push   eax
        push   ebx
        push   1
        mov    ebx, 1
        push   edi
        call   sub_2_21A4
        add    esp, 18h
loc_2_01CC:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_0D62
        add    esp, 10h
        mov    edi, eax
        test   ebx, ebx
        je     SHORT loc_2_0217
        mov    esi, dword ptr [esi]
        push   0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ebx, 80h
        push   ebx
        push   0
        push   esi
        call   sub_2_21A4
        add    esp, 18h
        lea    eax, [ebp - 4]
        and    byte ptr [ebp - 2], 0dfh
        push   0
        push   4
        push   eax
        push   ebx
        push   1
        push   esi
        call   sub_2_21A4
        add    esp, 18h
loc_2_0217:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0220:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    ecx, [ebp - 1]
        push   esi
        mov    ebx, 91h
        mov    esi, dword ptr [eax]
        push   0
        push   1
        push   ecx
        push   ebx
        push   0
        push   esi
        call   sub_2_21A4
        add    esp, 18h
        lea    eax, [ebp - 1]
        or     byte ptr [ebp - 1], 40h
        push   0
        push   1
        push   eax
        push   ebx
        push   1
        push   esi
        call   sub_2_21A4
        add    esp, 18h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0265:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    ecx, [ebp - 1]
        push   esi
        mov    ebx, 91h
        mov    esi, dword ptr [eax]
        push   0
        push   1
        push   ecx
        push   ebx
        push   0
        push   esi
        call   sub_2_21A4
        add    esp, 18h
        lea    eax, [ebp - 1]
        and    byte ptr [ebp - 1], 0bfh
        push   0
        push   1
        push   eax
        push   ebx
        push   1
        push   esi
        call   sub_2_21A4
        add    esp, 18h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_02AA:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 18h]
        mov    bl, byte ptr [esp + 14h]
        shl    bl, 4
        lea    ecx, [esi + 4]
        lea    eax, [esi + 805h]
        mov    edi, dword ptr [ecx]
        mov    dl, byte ptr [eax]
        mov    dword ptr [ecx], 1
        mov    byte ptr [eax], bl
        mov    dword ptr [esi + 0ch], 4001h
        mov    byte ptr [eax], dl
        test   byte ptr [esi + 804h], 0
        mov    dword ptr [esi], 0fh
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_02EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 2]
        push   edi
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   2
        push   eax
        push   3eh
        mov    esi, dword ptr [edi]
        push   0
        push   esi
        call   sub_2_21A4
        add    esp, 18h
        lea    ecx, [ebp - 2]
        and    word ptr [ebp - 2], 0f7ffh
        push   0
        push   2
        push   ecx
        push   3eh
        push   1
        push   esi
        call   sub_2_21A4
        add    esp, 18h
        mov    ecx, dword ptr [edi + 18h]
        lea    eax, [ecx + 816h]
        mov    dl, byte ptr [eax]
        or     dl, 10h
        mov    byte ptr [eax], dl
        test   byte ptr [ecx + 93ah], 18h
        je     SHORT loc_2_034E
        add    ecx, 925h
        mov    al, byte ptr [ecx]
        or     al, 80h
        mov    byte ptr [ecx], al
loc_2_034E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0354:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 18h]
        mov    ecx, dword ptr [esi + 8]
        and    cl, 30h
        cmp    cl, 20h
        jne    SHORT loc_2_0389
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_0D62
        add    esp, 10h
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_2_0439
loc_2_0389:
        lea    edi, [esi + 802h]
        add    esi, 816h
        mov    dword ptr [ebp - 0ch], 1
        mov    dl, byte ptr [edi]
        mov    bl, 0
        cmp    byte ptr [ebp + 0ch], 1
        adc    bl, 0ffh
        mov    al, byte ptr [esi]
        and    bl, 10h
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp + 0ch]
        sub    al, 3
        cmp    al, 1
        sbb    cl, cl
        and    cl, 2
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_2_03CE
        mov    al, byte ptr [ebp + 0ch]
        cmp    byte ptr [ebp + 10h], al
        jne    SHORT loc_2_03CE
        or     bl, 1
        jmp    SHORT loc_2_03E6
loc_2_03CE:
        cmp    byte ptr [ebp + 10h], 1
        jne    SHORT loc_2_03D9
        or     bl, 2
        jmp    SHORT loc_2_03E6
loc_2_03D9:
        cmp    byte ptr [ebp + 10h], 0
        je     SHORT loc_2_03E6
        mov    dword ptr [ebp - 0ch], 0
loc_2_03E6:
        cmp    dword ptr [ebp - 0ch], 1
        jne    SHORT loc_2_0439
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 8], dl
        and    byte ptr [ebp - 8], 0a0h
        or     bl, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        and    byte ptr [ebp - 8], 0fdh
        or     cl, byte ptr [ebp - 8]
        test   bl, 10h
        jne    SHORT loc_2_040B
        and    bl, 5fh
loc_2_040B:
        cmp    dl, bl
        jne    SHORT loc_2_0414
        cmp    byte ptr [ebp - 1], cl
        je     SHORT loc_2_0439
loc_2_0414:
        push   0
        mov    byte ptr [esi], cl
        push   493e0h
        mov    byte ptr [edi], bl
        call   sub_2_21AC
        add    esp, 8
        test   bl, 10h
        je     SHORT loc_2_0439
        mov    al, bl
        and    al, 0a0h
        cmp    al, 0a0h
        je     SHORT loc_2_0439
        or     bl, 0a0h
        mov    byte ptr [edi], bl
loc_2_0439:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0443:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [edx + 18h]
        mov    ecx, dword ptr [eax + 8]
        and    cl, 30h
        cmp    cl, 20h
        jne    SHORT loc_2_046C
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   edx
        call   sub_2_0DD9
        add    esp, 10h
        jmp    SHORT loc_2_04BE
loc_2_046C:
        mov    dl, byte ptr [eax + 802h]
        mov    al, byte ptr [eax + 816h]
        test   dl, 10h
        jne    SHORT loc_2_0485
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_0493
loc_2_0485:
        and    al, 2
        cmp    al, 1
        mov    eax, dword ptr [ebp + 0ch]
        sbb    cl, cl
        add    cl, 3
        mov    byte ptr [eax], cl
loc_2_0493:
        and    dl, 3
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dl, 1
        mov    byte ptr [ecx], dl
        jne    SHORT loc_2_04A6
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx], al
        jmp    SHORT loc_2_04B7
loc_2_04A6:
        cmp    dl, 2
        jne    SHORT loc_2_04B0
        mov    byte ptr [ecx], 1
        jmp    SHORT loc_2_04B7
loc_2_04B0:
        test   dl, dl
        je     SHORT loc_2_04B7
        mov    byte ptr [ecx], 0
loc_2_04B7:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], cl
loc_2_04BE:
        pop    ebp
        ret
loc_2_04C0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 18h]
        add    eax, 816h
        mov    cl, byte ptr [eax]
        or     cl, 1
        mov    byte ptr [eax], cl
        ret
loc_2_04D4:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 18h]
        add    eax, 816h
        mov    cl, byte ptr [eax]
        and    cl, 0feh
        mov    byte ptr [eax], cl
        ret
loc_2_04E8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 18h]
        or     byte ptr [ecx + 83eh], 1
        ret
loc_2_04F7:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 18h]
        mov    ecx, dword ptr [edi + 8]
        and    cl, 30h
        cmp    cl, 20h
        jne    SHORT loc_2_0528
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_0D62
        add    esp, 10h
        mov    esi, eax
        jmp    NEAR PTR loc_2_05AA
loc_2_0528:
        mov    esi, 1
        add    edi, 802h
        mov    cl, byte ptr [ebp + 0ch]
        mov    bl, 0
        mov    al, byte ptr [edi]
        cmp    cl, 1
        adc    bl, 0ffh
        and    bl, 10h
        cmp    cl, 3
        jne    SHORT loc_2_054B
        or     bl, 8
loc_2_054B:
        test   cl, cl
        je     SHORT loc_2_0559
        cmp    byte ptr [ebp + 10h], cl
        jne    SHORT loc_2_0559
        or     bl, 1
        jmp    SHORT loc_2_056C
loc_2_0559:
        cmp    byte ptr [ebp + 10h], 1
        jne    SHORT loc_2_0564
        or     bl, 2
        jmp    SHORT loc_2_056C
loc_2_0564:
        cmp    byte ptr [ebp + 10h], 0
        je     SHORT loc_2_056C
        xor    esi, esi
loc_2_056C:
        cmp    esi, 1
        jne    SHORT loc_2_05AA
        mov    cl, al
        and    cl, 0a0h
        or     bl, cl
        test   bl, 10h
        jne    SHORT loc_2_0580
        and    bl, 5fh
loc_2_0580:
        or     bl, 40h
        cmp    al, bl
        je     SHORT loc_2_05AA
        push   0
        mov    byte ptr [edi], bl
        push   493e0h
        call   sub_2_21AC
        add    esp, 8
        test   bl, 10h
        je     SHORT loc_2_05AA
        mov    al, bl
        and    al, 0a0h
        cmp    al, 0a0h
        je     SHORT loc_2_05AA
        or     bl, 0a0h
        mov    byte ptr [edi], bl
loc_2_05AA:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_2_05B1:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [edx + 18h]
        mov    ecx, dword ptr [eax + 8]
        and    cl, 30h
        cmp    cl, 20h
        jne    SHORT loc_2_05DA
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   edx
        call   sub_2_0DD9
        add    esp, 10h
        jmp    SHORT loc_2_0628
loc_2_05DA:
        mov    dl, byte ptr [eax + 802h]
        test   dl, 10h
        jne    SHORT loc_2_05ED
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_05FD
loc_2_05ED:
        mov    al, dl
        and    al, 8
        cmp    al, 1
        mov    eax, dword ptr [ebp + 0ch]
        sbb    cl, cl
        add    cl, 3
        mov    byte ptr [eax], cl
loc_2_05FD:
        and    dl, 3
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dl, 1
        mov    byte ptr [ecx], dl
        jne    SHORT loc_2_0610
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx], al
        jmp    SHORT loc_2_0621
loc_2_0610:
        cmp    dl, 2
        jne    SHORT loc_2_061A
        mov    byte ptr [ecx], 1
        jmp    SHORT loc_2_0621
loc_2_061A:
        test   dl, dl
        je     SHORT loc_2_0621
        mov    byte ptr [ecx], 0
loc_2_0621:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], cl
loc_2_0628:
        pop    ebp
        ret
loc_2_062A:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 18h]
        mov    byte ptr [eax + 83ch], 3
        or     byte ptr [eax + 816h], 2
        ret
loc_2_0640:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 18h]
        mov    byte ptr [eax + 83ch], 0
        and    byte ptr [eax + 816h], 0fdh
        ret
loc_2_0656:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 18h]
        or     byte ptr [ecx + 803h], 10h
        ret
loc_2_0665:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 18h]
        mov    byte ptr [ecx + 83bh], 3
        ret
loc_2_0674:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 18h]
        mov    byte ptr [ecx + 83bh], 0
        ret
loc_2_0683:
        sub    esp, 4
        lea    eax, [esp + 3]
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   0
        push   1
        push   eax
        push   50h
        push   0
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 18h
        test   byte ptr [esp + 7], 80h
        jne    SHORT loc_2_06B0
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
loc_2_06B0:
        pop    esi
        add    esp, 4
        ret
loc_2_06B5:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 18h]
        mov    eax, dword ptr [esi + 8]
        and    al, 30h
        cmp    al, 10h
        jne    SHORT loc_2_0746
        mov    edi, 1
        add    esi, 802h
        mov    al, byte ptr [ebp + 0ch]
        mov    bl, 10h
        mov    cl, byte ptr [esi]
        cmp    al, 2
        je     SHORT loc_2_06EC
        lea    edx, [eax - 3]
        cmp    dl, 1
        sbb    bl, bl
        and    bl, 8
loc_2_06EC:
        test   al, al
        je     SHORT loc_2_06FA
        cmp    byte ptr [ebp + 10h], al
        jne    SHORT loc_2_06FA
        or     bl, 1
        jmp    SHORT loc_2_070D
loc_2_06FA:
        cmp    byte ptr [ebp + 10h], 0
        je     SHORT loc_2_070D
        cmp    byte ptr [ebp + 10h], 1
        jne    SHORT loc_2_070B
        or     bl, 2
        jmp    SHORT loc_2_070D
loc_2_070B:
        xor    edi, edi
loc_2_070D:
        cmp    edi, 1
        jne    SHORT loc_2_075A
        mov    al, cl
        and    al, 80h
        or     bl, al
        test   bl, 18h
        jne    SHORT loc_2_0720
        and    bl, 7fh
loc_2_0720:
        cmp    bl, cl
        je     SHORT loc_2_075A
        push   0
        mov    byte ptr [esi], bl
        push   493e0h
        call   sub_2_21AC
        add    esp, 8
        test   bl, 18h
        je     SHORT loc_2_075A
        test   bl, 80h
        jne    SHORT loc_2_075A
        or     bl, 80h
        mov    byte ptr [esi], bl
        jmp    SHORT loc_2_075A
loc_2_0746:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   sub_2_0D62
        add    esp, 10h
        mov    edi, eax
loc_2_075A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_2_0761:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [edx + 18h]
        mov    ecx, dword ptr [eax + 8]
        and    cl, 30h
        cmp    cl, 10h
        jne    SHORT loc_2_07C2
        mov    dl, byte ptr [eax + 802h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    cl, dl
        and    cl, 18h
        mov    byte ptr [eax], cl
        cmp    cl, 10h
        jne    SHORT loc_2_0790
        mov    byte ptr [eax], 2
        jmp    SHORT loc_2_079B
loc_2_0790:
        cmp    cl, 8
        mov    byte ptr [eax], 3
        je     SHORT loc_2_079B
        mov    byte ptr [eax], 0
loc_2_079B:
        and    dl, 3
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dl, 1
        mov    byte ptr [ecx], dl
        jne    SHORT loc_2_07AE
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx], al
        jmp    SHORT loc_2_07B9
loc_2_07AE:
        cmp    dl, 2
        mov    byte ptr [ecx], 1
        je     SHORT loc_2_07B9
        mov    byte ptr [ecx], 0
loc_2_07B9:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], cl
        jmp    SHORT loc_2_07D1
loc_2_07C2:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   edx
        call   sub_2_0DD9
loc_2_07D1:
        mov    esp, ebp
        pop    ebp
        ret
loc_2_07D5:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 18h]
        or     byte ptr [ecx + 81eh], 20h
        ret
loc_2_07E4:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 18h]
        and    byte ptr [ecx + 81eh], 0dfh
        ret
loc_2_07F3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    ecx, [ebp - 2]
        push   esi
        mov    ebx, 84h
        mov    esi, dword ptr [eax]
        push   0
        push   2
        push   ecx
        push   ebx
        push   0
        push   esi
        call   sub_2_21A4
        add    esp, 18h
        lea    eax, [ebp - 2]
        or     byte ptr [ebp - 2], 30h
        push   0
        push   2
        push   eax
        push   ebx
        push   1
        push   esi
        call   sub_2_21A4
        add    esp, 18h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0838:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 18h]
        or     byte ptr [ecx + 82fh], 8
        ret
loc_2_0847:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 18h]
        and    byte ptr [ecx + 82fh], 0f7h
        ret
sub_2_0856:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        mov    ecx, 40000000h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 8]
        test   ecx, eax
        jne    NEAR PTR loc_2_0925
        or     eax, ecx
        lea    edi, [esi + 20h]
        xor    ebx, ebx
        push   1
        mov    word ptr [edi], bx
        lea    ecx, [ebp - 18h]
        mov    dword ptr [ebp - 4], 10h
        mov    dword ptr [esi + 8], eax
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        push   3
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   ebx
        push   dword ptr [esi]
        call   sub_2_21B4
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_2_08CF
        cmp    dword ptr [ebp - 4], 10h
        jne    SHORT loc_2_08CF
        xor    cl, cl
loc_2_08AF:
        movzx  edx, cl
        mov    al, byte ptr [ebp + edx - 18h]
        mov    byte ptr [edx + esi + 28h], al
        test   al, al
        je     SHORT loc_2_08C8
        mov    ax, 1
        shl    ax, cl
        or     word ptr [edi], ax
loc_2_08C8:
        inc    cl
        cmp    cl, 10h
        jb     SHORT loc_2_08AF
loc_2_08CF:
        xor    ebx, ebx
        cmp    word ptr [edi], bx
        jne    SHORT loc_2_0925
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 4], 4
        push   ecx
        push   3
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   ebx
        push   dword ptr [esi]
        call   sub_2_21B4
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_2_0925
        cmp    dword ptr [ebp - 8], ebx
        je     SHORT loc_2_0925
        mov    eax, dword ptr [ebp - 8]
        xor    cl, cl
        mov    word ptr [edi], ax
loc_2_090B:
        mov    eax, 1
        shl    eax, cl
        test   dword ptr [ebp - 8], eax
        je     SHORT loc_2_091E
        movzx  eax, cl
        mov    byte ptr [eax + esi + 28h], cl
loc_2_091E:
        inc    cl
        cmp    cl, 10h
        jb     SHORT loc_2_090B
loc_2_0925:
        mov    ax, word ptr [esi + 20h]
        test   ax, ax
        jne    SHORT loc_2_0937
        push   esi
        call   sub_2_0942
        add    esp, 4
loc_2_0937:
        or     word ptr [esi + 1ch], ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0942:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 24h], 4
        jne    SHORT loc_2_0983
        mov    ebx, dword ptr [edi]
        push   esi
        push   2
        lea    eax, [ebp - 2]
        push   eax
        push   3eh
        push   esi
        push   ebx
        call   sub_2_21A4
        add    esp, 18h
        lea    ecx, [ebp - 2]
        or     byte ptr [ebp - 1], 8
        push   esi
        push   2
        push   ecx
        push   3eh
        push   1
        push   ebx
        call   sub_2_21A4
        add    esp, 18h
loc_2_0983:
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_21CC
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_2_0A29
        pushfd
        cli
        in     al, 0a1h
        mov    ah, al
        in     al, 21h
        push   eax
        mov    al, 0ffh
        out    0a1h, al
        out    21h, al
        xor    bl, bl
loc_2_09B0:
        mov    ax, 1
        mov    cl, bl
        shl    ax, cl
        test   word ptr [edi + 22h], ax
        je     SHORT loc_2_09FA
        test   word ptr [edi + 1eh], ax
        jne    SHORT loc_2_09FA
        push   ebx
        push   edi
        call   sub_2_0A6A
        mov    word ptr [ebp - 6], ax
        add    esp, 8
        test   ax, ax
        je     SHORT loc_2_09FA
        bsf    ax, word ptr [ebp - 6]
        mov    word ptr [ebp - 4], ax
        movzx  eax, word ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 4]
        mov    byte ptr [eax + edi + 28h], bl
        mov    ax, 1
        shl    ax, cl
        or     si, ax
        or     word ptr [edi + 1eh], ax
loc_2_09FA:
        inc    bl
        cmp    bl, 10h
        jb     SHORT loc_2_09B0
        movzx  eax, si
        push   eax
        call   sub_2_21C4
        add    esp, 4
        pop    eax
        out    21h, al
        mov    al, ah
        out    0a1h, al
        popfd
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_21BC
        add    esp, 0ch
loc_2_0A29:
        cmp    byte ptr [edi + 24h], 4
        jne    SHORT loc_2_0A60
        mov    edi, dword ptr [edi]
        xor    ebx, ebx
        push   ebx
        lea    eax, [ebp - 2]
        push   2
        push   eax
        push   3eh
        push   ebx
        push   edi
        call   sub_2_21A4
        add    esp, 18h
        lea    ecx, [ebp - 2]
        and    word ptr [ebp - 2], 0f7ffh
        push   ebx
        push   2
        push   ecx
        push   3eh
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 18h
loc_2_0A60:
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0A6A:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 18h
        sub    esp, 14h
        mov    dword ptr [ebp - 0ch], eax
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [eax]
        mov    ecx, dword ptr [edi + 4]
        lea    esi, [edi + 805h]
        mov    dword ptr [ebp - 14h], ecx
        mov    al, byte ptr [esi]
        lea    ecx, [edi + 803h]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp + 0ch]
        mov    dl, byte ptr [ecx]
        shl    al, 4
        mov    byte ptr [ebp - 1], dl
        or     al, 8
        mov    byte ptr [ecx], 40h
        mov    byte ptr [ebp - 10h], al
        mov    byte ptr [esi], al
        mov    dword ptr [edi + 4], 6
        test   byte ptr [edi + 804h], bl
        mov    dword ptr [edi], 0fh
        call   sub_2_0B7E
        cmp    byte ptr [edi + 800h], 82h
        mov    word ptr [ebp - 4], ax
        je     SHORT loc_2_0B05
        mov    byte ptr [edi + 816h], 20h
        call   sub_2_0B7E
        test   byte ptr [edi + 804h], bl
        mov    word ptr [ebp - 6], ax
        mov    dword ptr [edi], 0fh
        call   sub_2_0B7E
        mov    bx, word ptr [ebp - 6]
        xor    bx, ax
        mov    ax, word ptr [ebp - 6]
        xor    ax, word ptr [ebp - 4]
        and    bx, ax
loc_2_0B05:
        test   bx, bx
        jne    SHORT loc_2_0B57
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [esi], 0
        test   byte ptr [eax + 0ah], 2
        je     SHORT loc_2_0B22
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_02AA
        add    esp, 8
loc_2_0B22:
        call   sub_2_0B7E
        mov    si, ax
        mov    dl, byte ptr [ebp - 10h]
        mov    bx, si
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax]
        mov    byte ptr [ecx + 805h], dl
        test   byte ptr [ecx + 804h], 0
        mov    dword ptr [ecx], 0fh
        call   sub_2_0B7E
        xor    bx, ax
        xor    si, word ptr [ebp - 4]
        and    bx, si
loc_2_0B57:
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [eax]
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 4], edx
        mov    dl, byte ptr [ebp - 1]
        mov    byte ptr [ecx + 805h], al
        mov    ax, bx
        mov    byte ptr [ecx + 803h], dl
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0B7E:
        sub    esp, 4
        mov    al, 0ah
        out    0a0h, al
        in     al, 0a0h
        mov    ah, al
        mov    al, 0ah
        out    20h, al
        in     al, 20h
        mov    word ptr [esp + 2], ax
        mov    ax, word ptr [esp + 2]
        add    esp, 4
        ret
sub_2_0B9D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    byte ptr [ebp - 3], 0
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        lea    ecx, [ebp - 5]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], 0
        push   0
        push   eax
        movzx  eax, byte ptr [esi + 24h]
        imul   eax, eax, 24h
        push   ecx
        push   esi
        db  0FFh, 090h, 010h, 000h, 000h, 000h
        add    esp, 10h
        mov    edi, eax
        cmp    edi, 1
        jne    NEAR PTR loc_2_0CCC
        mov    al, byte ptr [ebp + 0ch]
        mov    cl, al
        mov    bl, al
        and    cl, 7
        and    bl, 10h
        mov    byte ptr [ebp - 2], cl
        je     SHORT loc_2_0BF7
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 4], al
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 2], al
        jmp    SHORT loc_2_0C20
loc_2_0BF7:
        test   al, 20h
        jne    SHORT loc_2_0C0D
        cmp    byte ptr [ebp - 5], 2
        je     SHORT loc_2_0C0D
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 1], al
        je     SHORT loc_2_0C0D
        xor    edi, edi
        jmp    SHORT loc_2_0C20
loc_2_0C0D:
        mov    al, byte ptr [ebp + 10h]
        and    al, 70h
        shr    al, 4
        mov    byte ptr [ebp - 3], al
        mov    al, byte ptr [ebp + 10h]
        and    al, 7
        mov    byte ptr [ebp - 4], al
loc_2_0C20:
        cmp    edi, 1
        jne    NEAR PTR loc_2_0CB9
        cmp    byte ptr [ebp - 2], 3
        ja     SHORT loc_2_0C3B
        cmp    byte ptr [ebp - 3], 3
        ja     SHORT loc_2_0C3B
        cmp    byte ptr [ebp - 4], 3
        jbe    SHORT loc_2_0C3D
loc_2_0C3B:
        xor    edi, edi
loc_2_0C3D:
        cmp    edi, 1
        jne    NEAR PTR loc_2_0CCC
        movzx  eax, byte ptr [ebp - 4]
        movzx  ecx, byte ptr [ebp - 3]
        push   eax
        push   ecx
        movzx  edx, byte ptr [ebp - 2]
        movzx  eax, byte ptr [esi + 24h]
        imul   eax, eax, 24h
        push   edx
        push   esi
        db  0FFh, 090h, 008h, 000h, 000h, 000h
        add    esp, 10h
        mov    edi, eax
        cmp    edi, 1
        jne    SHORT loc_2_0CCC
        test   bl, bl
        je     SHORT loc_2_0CB9
        cmp    byte ptr [ebp - 2], 3
        jne    SHORT loc_2_0CB9
        cmp    byte ptr [ebp - 5], 1
        jne    SHORT loc_2_0CB9
        push   1
        lea    eax, [ebp - 1]
        push   eax
        push   0
        movzx  eax, byte ptr [esi + 24h]
        imul   eax, eax, 24h
        push   esi
        db  0FFh, 090h, 010h, 000h, 000h, 000h
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_0CB9
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_2_0CB9
        movzx  eax, byte ptr [esi + 24h]
        imul   eax, eax, 24h
        push   2
        push   2
        push   2
        push   esi
        db  0FFh, 090h, 008h, 000h, 000h, 000h
        add    esp, 10h
        mov    edi, eax
loc_2_0CB9:
        cmp    edi, 1
        jne    SHORT loc_2_0CCC
        test   bl, bl
        je     SHORT loc_2_0CC8
        or     byte ptr [esi + 0bh], 20h
        jmp    SHORT loc_2_0CCC
loc_2_0CC8:
        and    byte ptr [esi + 0bh], 0dfh
loc_2_0CCC:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0CD5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 3]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        mov    edi, dword ptr [ebp + 0ch]
        movzx  eax, byte ptr [esi + 24h]
        imul   eax, eax, 24h
        push   edi
        push   esi
        db  0FFh, 090h, 00Ch, 000h, 000h, 000h
        add    esp, 10h
        mov    cl, byte ptr [ebp - 4]
        shl    cl, 4
        mov    eax, dword ptr [ebp + 10h]
        or     cl, byte ptr [ebp - 3]
        push   0
        lea    edx, [ebp - 1]
        mov    byte ptr [eax], cl
        push   edx
        lea    ecx, [ebp - 2]
        movzx  eax, byte ptr [esi + 24h]
        imul   eax, eax, 24h
        push   ecx
        push   esi
        db  0FFh, 090h, 010h, 000h, 000h, 000h
        add    esp, 10h
        test   byte ptr [esi + 0bh], 20h
        je     SHORT loc_2_0D31
        or     byte ptr [edi], 10h
        jmp    SHORT loc_2_0D42
loc_2_0D31:
        cmp    byte ptr [ebp - 2], 2
        je     SHORT loc_2_0D42
        mov    al, byte ptr [edi]
        cmp    byte ptr [ebp - 1], al
        je     SHORT loc_2_0D42
        or     al, 20h
        mov    byte ptr [edi], al
loc_2_0D42:
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_2_0D5C
        cmp    byte ptr [ebp - 1], 3
        jne    SHORT loc_2_0D53
        or     byte ptr [edi], 40h
        jmp    SHORT loc_2_0D5C
loc_2_0D53:
        cmp    byte ptr [ebp - 1], 4
        jne    SHORT loc_2_0D5C
        or     byte ptr [edi], 80h
loc_2_0D5C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0D62:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        mov    ebx, 1
        push   edi
        mov    esi, dword ptr [eax + 18h]
        movzx  edi, byte ptr [esp + 14h]
        movzx  eax, byte ptr [esp + 18h]
        shl    edi, 4
        mov    ecx, dword ptr [esi + 10h]
        test   byte ptr [esi + 8], 0
        or     edi, eax
        mov    eax, ecx
        and    al, 88h
        or     edi, eax
        cmp    edi, ecx
        je     SHORT loc_2_0DD3
        push   0
        mov    dword ptr [esi + 10h], edi
        push   493e0h
        call   sub_2_21AC
        add    esp, 8
        mov    eax, dword ptr [esi + 8]
        test   ah, 2
        je     SHORT loc_2_0DAF
        xor    ebx, ebx
        jmp    SHORT loc_2_0DD3
loc_2_0DAF:
        and    eax, 30h
        cmp    eax, 10h
        jne    SHORT loc_2_0DC8
        test   edi, 70h
        je     SHORT loc_2_0DCC
        or     byte ptr [esi + 802h], 0a0h
        jmp    SHORT loc_2_0DD3
loc_2_0DC8:
        test   eax, eax
        jne    SHORT loc_2_0DD3
loc_2_0DCC:
        and    byte ptr [esi + 802h], 5fh
loc_2_0DD3:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_0DD9:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [eax + 18h]
        mov    edx, dword ptr [eax + 10h]
        test   byte ptr [eax + 8], 0
        mov    ebx, edx
        mov    eax, dword ptr [ebp + 0ch]
        shr    ebx, 4
        and    dl, 7
        and    bl, 7
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [eax], bl
        cmp    bl, 3
        mov    byte ptr [ecx], dl
        ja     SHORT loc_2_0E0A
        cmp    bl, 1
        jne    SHORT loc_2_0E0D
loc_2_0E0A:
        mov    byte ptr [eax], 0
loc_2_0E0D:
        cmp    dl, 3
        jbe    SHORT loc_2_0E15
        mov    byte ptr [ecx], 0
loc_2_0E15:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 14h]
        pop    ebx
        pop    ebp
        mov    byte ptr [eax], cl
        ret
sub_2_0E1F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        mov    ebx, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ebp + 8]
        test   ebx, ebx
        je     SHORT loc_2_0E50
        mov    eax, dword ptr [edi + 18h]
        push   0
        push   493e0h
        mov    dword ptr [eax + 0ch], 4000h
        call   sub_2_21AC
        add    esp, 8
loc_2_0E50:
        mov    eax, dword ptr [edi + 18h]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, ecx
        and    eax, 30h
        cmp    eax, 10h
        jne    SHORT loc_2_0E66
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_2_0E8B
loc_2_0E66:
        cmp    eax, 20h
        jne    SHORT loc_2_0E71
        mov    byte ptr [ebp - 2], 2
        jmp    SHORT loc_2_0E8B
loc_2_0E71:
        cmp    eax, 30h
        jne    SHORT loc_2_0E87
        mov    al, byte ptr [edi + 24h]
        cmp    al, 8
        je     SHORT loc_2_0E81
        cmp    al, 9
        jne    SHORT loc_2_0E87
loc_2_0E81:
        mov    byte ptr [ebp - 2], 2
        jmp    SHORT loc_2_0E8B
loc_2_0E87:
        mov    byte ptr [ebp - 2], 0
loc_2_0E8B:
        mov    eax, ecx
        and    eax, 0f003ffffh
        shr    eax, 12h
        and    ecx, eax
        test   ch, 4
        je     SHORT loc_2_0EA2
        mov    byte ptr [ebp - 1], 2
        jmp    SHORT loc_2_0ECA
loc_2_0EA2:
        test   ch, 8
        je     SHORT loc_2_0EAD
        mov    byte ptr [ebp - 1], 3
        jmp    SHORT loc_2_0ECA
loc_2_0EAD:
        test   ch, 10h
        je     SHORT loc_2_0EB8
        mov    byte ptr [ebp - 1], 4
        jmp    SHORT loc_2_0ECA
loc_2_0EB8:
        and    ecx, 2000h
        mov    al, 0
        cmp    ecx, 1
        adc    al, 0ffh
        and    al, 5
        mov    byte ptr [ebp - 1], al
loc_2_0ECA:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_2_0EF4
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_2_0EF4
        mov    esi, 0
        test   ebx, ebx
        jne    SHORT loc_2_0EF4
        push   1
        lea    eax, [ebp - 1]
        push   eax
        lea    ecx, [ebp - 2]
        push   ecx
        push   edi
        call   sub_2_0E1F
        add    esp, 10h
        mov    esi, eax
loc_2_0EF4:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_2_0F00
        mov    cl, byte ptr [ebp - 2]
        mov    byte ptr [eax], cl
loc_2_0F00:
        mov    eax, dword ptr [ebp + 10h]
        test   eax, eax
        je     SHORT loc_2_0F0C
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [eax], cl
loc_2_0F0C:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0F15:
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 4]
        shr    edx, 18h
        mov    eax, dword ptr [eax + 18h]
        movzx  ecx, byte ptr [esp + 8]
        mov    byte ptr [ecx + eax + 840h], dl
        xor    al, al
        ret
loc_2_0F32:
        mov    eax, dword ptr [esp + 4]
        movzx  ecx, byte ptr [esp + 8]
        mov    edx, dword ptr [eax + 18h]
        movzx  eax, byte ptr [ecx + edx + 840h]
        shl    eax, 18h
        ret
sub_2_0F4A:
        push   esi
        mov    eax, 1
        push   edi
        xor    dl, dl
        mov    edi, dword ptr [esp + 0ch]
loc_2_0F57:
        cmp    dl, 5
        jae    SHORT loc_2_0F86
        mov    esi, dword ptr [edi + 18h]
        movzx  ecx, dl
        lea    esi, [ecx + esi + 840h]
        mov    cl, byte ptr [esi]
        mov    byte ptr [esi], 55h
        cmp    byte ptr [esi], 55h
        jne    SHORT loc_2_0F7B
        mov    byte ptr [esi], 0aah
        cmp    byte ptr [esi], 0aah
        je     SHORT loc_2_0F7D
loc_2_0F7B:
        xor    eax, eax
loc_2_0F7D:
        inc    dl
        mov    byte ptr [esi], cl
        cmp    eax, 1
        je     SHORT loc_2_0F57
loc_2_0F86:
        pop    edi
        pop    esi
        ret
sub_2_0F89:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        cmp    byte ptr [esi], 0
        je     SHORT loc_2_0FC7
        mov    eax, dword ptr [esp + 14h]
loc_2_0F9A:
        mov    dl, byte ptr [esi]
        inc    esi
        mov    cl, dl
        mov    bl, byte ptr [esi]
        inc    esi
        dec    dl
        test   cl, cl
        je     SHORT loc_2_0FC2
        mov    edi, dword ptr [eax + 18h]
loc_2_0FAB:
        movzx  ebp, bl
        mov    cl, byte ptr [esi]
        inc    bl
        inc    esi
        mov    byte ptr [ebp + edi + 800h], cl
        mov    cl, dl
        dec    dl
        test   cl, cl
        jne    SHORT loc_2_0FAB
loc_2_0FC2:
        cmp    byte ptr [esi], 0
        jne    SHORT loc_2_0F9A
loc_2_0FC7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_0FCC:
        xor    al, al
        mov    edx, dword ptr [esp + 4]
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_0FFA
loc_2_0FDB:
        movzx  ecx, al
        imul   ecx, ecx, 24h
        db  039h, 091h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_0FFA
        inc    al
        movzx  ecx, al
        imul   ecx, ecx, 24h
        db  083h, 0B9h, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_0FDB
loc_2_0FFA:
        movzx  ecx, al
        imul   ecx, ecx, 24h
        db  039h, 091h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_100A
        xor    al, al
loc_2_100A:
        ret
sub_2_100B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        lea    eax, [ebp - 0ch]
        mov    word ptr [esi + 22h], 0deb8h
        push   edi
        push   4
        push   eax
        push   edi
        push   edi
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 18h
        push   dword ptr [ebp - 0ch]
        call   sub_2_0FCC
        add    esp, 4
        mov    byte ptr [esi + 24h], al
        movzx  eax, al
        imul   eax, eax, 24h
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        db  039h, 0B8h, 014h, 000h, 000h, 000h
        je     SHORT loc_2_105E
        db  0C7h, 046h, 014h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_109D
loc_2_105E:
        push   esi
        call   sub_2_0F4A
        add    esp, 4
        test   eax, eax
        je     SHORT loc_2_1096
        movzx  eax, byte ptr [esi + 24h]
        imul   eax, eax, 24h
        db  0C7h, 080h, 014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        movzx  eax, byte ptr [esi + 24h]
        imul   eax, eax, 24h
        db  0C7h, 080h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 014h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_109D
loc_2_1096:
        db  0C7h, 046h, 014h, 000h, 000h, 000h, 000h
loc_2_109D:
        movzx  eax, byte ptr [esi + 24h]
        imul   eax, eax, 24h
        db  083h, 0B8h, 01Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_10BB
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
loc_2_10BB:
        db  068h, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        push   esi
        call   sub_2_0F89
        add    esp, 8
        mov    eax, dword ptr [esi + 18h]
        mov    edi, dword ptr [esi]
        push   ebx
        push   2
        mov    dword ptr [eax + 10h], ebx
        mov    dword ptr [eax + 4], ebx
        test   byte ptr [eax + 804h], bl
        mov    dword ptr [eax], 0fh
        lea    eax, [ebp - 2]
        push   eax
        push   3eh
        push   ebx
        push   edi
        call   sub_2_21A4
        add    esp, 18h
        lea    ecx, [ebp - 2]
        or     word ptr [ebp - 2], 480h
        push   ebx
        push   2
        push   ecx
        push   3eh
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 18h
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], ebx
        push   ebx
        push   4
        push   ecx
        push   44h
        push   1
        push   edi
        call   sub_2_21A4
        movzx  eax, byte ptr [esi + 24h]
        imul   eax, eax, 24h
        add    esp, 18h
        db  08Bh, 080h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_113D
        push   esi
        call   eax
        add    esp, 4
loc_2_113D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_1144:
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    word ptr [eax + 10h], 501h
        mov    byte ptr [eax + 1ch], cl
        mov    byte ptr [eax + 18h], cl
        mov    byte ptr [eax + 19h], 1
        and    byte ptr [eax + 20h], 0feh
        mov    byte ptr [eax + 1dh], cl
        ret
loc_2_1162:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 10h]
        mov    word ptr [esi + 18h], bx
        push   edi
        mov    byte ptr [esi + 10h], 1
        mov    byte ptr [esi + 11h], 7
        mov    word ptr [esi + 14h], bx
        call   sub_2_0856
        add    esp, 4
        mov    eax, dword ptr [edi + 0ch]
        movzx  ecx, word ptr [edi + 1ch]
        push   14h
        mov    dword ptr [eax + 2], ecx
        push   eax
        push   dword ptr [esi]
        call   sub_2_1E64
        add    esp, 0ch
        mov    byte ptr [esi + 1dh], bl
        and    byte ptr [esi + 20h], 0feh
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_11AA:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        movzx  ax, byte ptr [edx + 26h]
        mov    word ptr [ecx], ax
        mov    byte ptr [ecx + 15h], 0
        mov    byte ptr [ecx + 1dh], 0
        and    byte ptr [ecx + 20h], 0feh
        ret
loc_2_11C7:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ax, word ptr [esi]
        test   al, 20h
        jne    SHORT loc_2_1212
        test   al, 40h
        je     SHORT loc_2_1212
        mov    edx, dword ptr [esp + 8]
        mov    cl, byte ptr [edx + 26h]
        xor    cl, al
        test   cl, 1fh
        jne    SHORT loc_2_1212
        mov    byte ptr [edx + 26h], al
        test   al, 80h
        je     SHORT loc_2_11FC
        mov    eax, dword ptr [edx + 18h]
        or     byte ptr [edx + 0bh], 4
        mov    dword ptr [eax + 4], 6
        jmp    SHORT loc_2_120A
loc_2_11FC:
        mov    ecx, dword ptr [edx + 18h]
        and    byte ptr [edx + 0bh], 0fbh
        mov    dword ptr [ecx + 4], 0
loc_2_120A:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_1218
loc_2_1212:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 6
loc_2_1218:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1223
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_1227
loc_2_1223:
        or     byte ptr [esi + 20h], 1
loc_2_1227:
        pop    esi
        ret
loc_2_1229:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        cmp    byte ptr [esi + 11h], 0ffh
        jne    SHORT loc_2_1279
        mov    al, byte ptr [esi + 15h]
        cmp    al, 7
        jae    SHORT loc_2_1279
        cmp    al, 5
        jb     SHORT loc_2_124F
        db  0B8h, 000h, 000h, 000h, 000h
        mov    ecx, 18h
        mov    byte ptr [esi + 10h], 84h
        jmp    SHORT loc_2_125F
loc_2_124F:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, 20h
        mov    byte ptr [esi + 10h], 83h
        mov    eax, dword ptr [eax + 14h]
loc_2_125F:
        mov    word ptr [esi + 18h], 1
        push   ecx
        push   eax
        push   dword ptr [esi]
        call   sub_2_1E64
        add    esp, 0ch
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_127F
loc_2_1279:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 11h
loc_2_127F:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_128A
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_128E
loc_2_128A:
        or     byte ptr [esi + 20h], 1
loc_2_128E:
        pop    esi
        ret
loc_2_1290:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    cl, byte ptr [esi + 11h]
        cmp    cl, 7
        jb     SHORT loc_2_12AB
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 11h
        jmp    NEAR PTR loc_2_13A3
loc_2_12AB:
        lea    edi, [esi + 15h]
        mov    al, 1
        shl    al, cl
        mov    byte ptr [edi], 0
        cmp    cl, 5
        mov    byte ptr [esi + 10h], 10h
        jb     SHORT loc_2_1319
        add    al, al
        mov    edx, dword ptr [esp + 14h]
        mov    edx, dword ptr [edx + 18h]
        mov    byte ptr [edi], 1
        mov    byte ptr [esi + 14h], 0
        test   byte ptr [edx + 806h], al
        je     SHORT loc_2_12D9
        mov    byte ptr [edi], 3
loc_2_12D9:
        lea    eax, [ecx - 5]
        cmp    al, 1
        sbb    al, al
        and    al, 0f1h
        add    al, 10h
        test   byte ptr [edx + 807h], al
        je     SHORT loc_2_12EF
        or     byte ptr [edi], 4
loc_2_12EF:
        movzx  eax, cl
        lea    eax, [edx + eax*4 + 7f4h]
        movzx  ecx, word ptr [eax]
        movzx  eax, word ptr [eax + 2]
        mov    dword ptr [esi], ecx
        test   eax, eax
        lea    ecx, [esi + 18h]
        mov    dword ptr [ecx], eax
        je     NEAR PTR loc_2_139D
        sub    eax, dword ptr [esi]
        inc    eax
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_2_139D
loc_2_1319:
        mov    edx, dword ptr [esp + 14h]
        mov    ebx, dword ptr [edx + 18h]
        test   byte ptr [ebx + 806h], al
        je     SHORT loc_2_132B
        mov    byte ptr [edi], 2
loc_2_132B:
        movzx  ecx, cl
        lea    ebp, [ebx + ecx*8 + 810h]
        xor    ebx, ebx
        mov    ax, word ptr [ebp]
        mov    bx, ax
        and    ebx, 0fffh
        test   ah, 80h
        mov    dword ptr [esi], ebx
        je     SHORT loc_2_134E
        or     byte ptr [edi], 4
loc_2_134E:
        mov    ax, word ptr [ebp + 2]
        lea    edi, [esi + 18h]
        xor    ebp, ebp
        mov    bp, ax
        and    ebp, 0fffh
        mov    dword ptr [edi], ebp
        je     SHORT loc_2_1369
        sub    ebp, ebx
        inc    ebp
        mov    dword ptr [edi], ebp
loc_2_1369:
        shl    ebx, 0ch
        shr    ax, 0eh
        mov    dword ptr [esi], ebx
        movzx  ebp, ax
        shl    dword ptr [edi], 0ch
        db  08Ah, 085h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 14h], al
        movzx  eax, byte ptr [edx + 24h]
        imul   eax, eax, 24h
        db  08Bh, 080h, 018h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_139D
        push   ecx
        push   edx
        call   eax
        add    esp, 8
        or     eax, ebx
        mov    dword ptr [esi], eax
loc_2_139D:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
loc_2_13A3:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_13AE
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_13B2
loc_2_13AE:
        or     byte ptr [esi + 20h], 1
loc_2_13B2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_13B7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [esi + 11h]
        cmp    bl, 7
        jb     SHORT loc_2_13D6
        lea    edi, [esi + 1dh]
        mov    byte ptr [edi], 11h
        jmp    NEAR PTR loc_2_157F
loc_2_13D6:
        mov    al, byte ptr [esi + 10h]
        test   al, 10h
        je     NEAR PTR loc_2_1579
        test   al, 0fh
        jne    NEAR PTR loc_2_1579
        lea    edi, [esi + 1dh]
        cmp    bl, 5
        mov    byte ptr [edi], 0
        jb     NEAR PTR loc_2_149A
        mov    al, byte ptr [esi + 15h]
        test   al, 1
        jne    SHORT loc_2_1407
        mov    byte ptr [edi], 0dh
        jmp    NEAR PTR loc_2_155A
loc_2_1407:
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   dword ptr [esi + 18h]
        push   dword ptr [esi]
        call   sub_2_1D74
        add    esp, 10h
        test   al, al
        jne    NEAR PTR loc_2_1558
        push   0
        mov    al, byte ptr [esi + 11h]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_1E34
        movzx  eax, byte ptr [esi + 11h]
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 18h]
        mov    edx, dword ptr [ecx + 18h]
        test   ebx, ebx
        lea    ecx, [edx + eax*4 + 7f4h]
        mov    ax, word ptr [esi]
        mov    word ptr [ecx], ax
        je     SHORT loc_2_1458
        add    ax, bx
        dec    ax
        jmp    SHORT loc_2_145C
loc_2_1458:
        mov    ax, 0ffffh
loc_2_145C:
        mov    word ptr [ecx + 2], ax
        mov    al, byte ptr [esi + 15h]
        and    al, 4
        mov    ecx, dword ptr [ebp + 8]
        cmp    al, 1
        mov    al, 0
        adc    al, 0ffh
        and    al, 0bh
        cmp    byte ptr [ecx + 24h], 5
        jne    SHORT loc_2_147E
        test   byte ptr [ecx + 0bh], 8
        je     SHORT loc_2_147E
        and    al, 0fdh
loc_2_147E:
        cmp    byte ptr [esi + 11h], 5
        mov    cl, 0f0h
        jbe    SHORT loc_2_148B
        shl    al, 4
        mov    cl, 0fh
loc_2_148B:
        add    edx, 807h
        and    byte ptr [edx], cl
        or     byte ptr [edx], al
        jmp    NEAR PTR loc_2_155A
loc_2_149A:
        test   byte ptr [esi + 15h], 1
        je     SHORT loc_2_14A8
        mov    byte ptr [edi], 0dh
        jmp    NEAR PTR loc_2_155A
loc_2_14A8:
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [esi + 15h]
        push   dword ptr [ecx + 14h]
        push   edx
        push   dword ptr [esi + 18h]
        push   dword ptr [esi]
        call   sub_2_1D74
        add    esp, 10h
        test   al, al
        jne    NEAR PTR loc_2_1558
        lea    eax, [ebp - 1]
        mov    cl, byte ptr [esi + 14h]
        push   eax
        push   ecx
        call   sub_2_1DCD
        add    esp, 8
        test   al, al
        jne    SHORT loc_2_1558
        push   0
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_2_1E34
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        movzx  edx, bl
        mov    eax, dword ptr [ecx + 18h]
        lea    ecx, [eax + edx*8 + 810h]
        mov    eax, dword ptr [esi]
        shr    eax, 0ch
        and    ax, 0fffh
        test   byte ptr [esi + 15h], 4
        je     SHORT loc_2_150B
        or     ah, 80h
loc_2_150B:
        mov    word ptr [ecx], ax
        cmp    dword ptr [esi + 18h], 0
        mov    ax, 0fffh
        je     SHORT loc_2_1525
        mov    eax, dword ptr [esi]
        add    eax, dword ptr [esi + 18h]
        dec    eax
        shr    eax, 0ch
        and    ax, 0fffh
loc_2_1525:
        movzx  bx, byte ptr [ebp - 1]
        shl    bx, 0eh
        or     ax, bx
        mov    word ptr [ecx + 2], ax
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [ecx + 24h]
        imul   eax, eax, 24h
        db  08Bh, 088h, 014h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_2_155A
        push   dword ptr [esi]
        push   edx
        push   dword ptr [ebp + 8]
        call   ecx
        add    esp, 0ch
        test   al, al
        je     SHORT loc_2_155A
loc_2_1558:
        mov    byte ptr [edi], al
loc_2_155A:
        cmp    byte ptr [edi], 0
        jne    SHORT loc_2_158A
        xor    eax, eax
        mov    cl, byte ptr [esi + 11h]
        mov    al, byte ptr [esi + 15h]
        and    eax, 2
        push   eax
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_2_1E34
        add    esp, 0ch
        jmp    SHORT loc_2_157F
loc_2_1579:
        lea    edi, [esi + 1dh]
        mov    byte ptr [edi], 0bh
loc_2_157F:
        cmp    byte ptr [edi], 0
        jne    SHORT loc_2_158A
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_158E
loc_2_158A:
        or     byte ptr [esi + 20h], 1
loc_2_158E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_1595:
        mov    edx, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    cl, byte ptr [edx + 11h]
        cmp    cl, 5
        jb     SHORT loc_2_15AB
        lea    eax, [edx + 1dh]
        mov    byte ptr [eax], 11h
        jmp    SHORT loc_2_1618
loc_2_15AB:
        cmp    byte ptr [edx + 10h], 0
        je     SHORT loc_2_15B9
        lea    eax, [edx + 1dh]
        mov    byte ptr [eax], 8
        jmp    SHORT loc_2_1618
loc_2_15B9:
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [eax + 18h]
        movzx  eax, cl
        mov    di, word ptr [esi + eax*8 + 814h]
        lea    eax, [esi + eax*8 + 810h]
        mov    ax, word ptr [eax]
        and    ax, 0fffh
        add    ax, di
        and    ax, 3fffh
        mov    word ptr [edx], ax
        lea    eax, [edx + 14h]
        test   di, 8000h
        mov    byte ptr [eax], 0
        je     SHORT loc_2_15F3
        mov    byte ptr [eax], 4
loc_2_15F3:
        test   di, 4000h
        je     SHORT loc_2_15FD
        or     byte ptr [eax], 1
loc_2_15FD:
        movzx  esi, byte ptr [esi + 806h]
        mov    edi, 1
        shl    edi, cl
        test   edi, esi
        je     SHORT loc_2_1612
        or     byte ptr [eax], 2
loc_2_1612:
        lea    eax, [edx + 1dh]
        mov    byte ptr [eax], 0
loc_2_1618:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1623
        and    byte ptr [edx + 20h], 0feh
        jmp    SHORT loc_2_1627
loc_2_1623:
        or     byte ptr [edx + 20h], 1
loc_2_1627:
        pop    edi
        pop    esi
        ret
loc_2_162A:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    al, byte ptr [esi + 11h]
        cmp    al, 5
        jb     SHORT loc_2_163F
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 11h
        jmp    SHORT loc_2_16B4
loc_2_163F:
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_2_164D
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 8
        jmp    SHORT loc_2_16B4
loc_2_164D:
        mov    di, word ptr [esi]
        test   di, 0c000h
        je     SHORT loc_2_165F
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 7
        jmp    SHORT loc_2_16B4
loc_2_165F:
        mov    cl, byte ptr [esi + 14h]
        movzx  edx, cl
        test   edx, 0fffffff8h
        je     SHORT loc_2_1675
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 2
        jmp    SHORT loc_2_16B4
loc_2_1675:
        movzx  edx, al
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        lea    eax, [eax + edx*8 + 810h]
        mov    dx, word ptr [eax]
        and    dx, 0fffh
        sub    di, dx
        and    di, 3fffh
        test   cl, 4
        je     SHORT loc_2_16A0
        or     di, 8000h
loc_2_16A0:
        test   cl, 1
        je     SHORT loc_2_16AA
        or     di, 4000h
loc_2_16AA:
        mov    word ptr [eax + 4], di
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
loc_2_16B4:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_16BF
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_16C3
loc_2_16BF:
        or     byte ptr [esi + 20h], 1
loc_2_16C3:
        pop    edi
        pop    esi
        ret
loc_2_16C6:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_2_16D9
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0bh
        jmp    SHORT loc_2_171D
loc_2_16D9:
        mov    byte ptr [esi + 11h], 0f0h
        mov    byte ptr [esi + 15h], 0f0h
        mov    byte ptr [esi + 14h], 0
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, word ptr [eax + 1ch]
        mov    edx, dword ptr [eax + 10h]
        mov    al, byte ptr [eax + 26h]
        and    al, 1fh
        push   12h
        push   edx
        mov    dword ptr [edx + 2], ecx
        push   dword ptr [esi]
        cmp    al, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0ffffh
        mov    dword ptr [edx + 6], eax
        call   sub_2_1E64
        add    esp, 0ch
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
loc_2_171D:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1728
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_172C
loc_2_1728:
        or     byte ptr [esi + 20h], 1
loc_2_172C:
        pop    esi
        ret
loc_2_172E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_2_1747
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0bh
        jmp    SHORT loc_2_179F
loc_2_1747:
        push   esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1B49
        movzx  ax, byte ptr [edi + 27h]
        mov    word ptr [esi + 8], ax
        add    esp, 8
        mov    eax, dword ptr [edi + 18h]
        mov    cl, byte ptr [eax + 805h]
        mov    eax, dword ptr [eax + 4]
        and    cl, 7
        and    al, 6
        shl    cl, 4
        cmp    al, 6
        mov    byte ptr [esi + 11h], cl
        jne    SHORT loc_2_177C
        or     byte ptr [esi + 11h], 80h
loc_2_177C:
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 2]
        push   eax
        push   ecx
        push   edi
        call   sub_2_0CD5
        add    esp, 0ch
        mov    cl, byte ptr [ebp - 2]
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 19h], cl
        mov    byte ptr [esi + 18h], al
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
loc_2_179F:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_17AA
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_17AE
loc_2_17AA:
        or     byte ptr [esi + 20h], 1
loc_2_17AE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_17B4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        cmp    byte ptr [edi + 10h], 0
        je     SHORT loc_2_17D1
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0bh
        jmp    NEAR PTR loc_2_1A93
loc_2_17D1:
        mov    dl, byte ptr [edi + 19h]
        mov    al, dl
        and    al, 30h
        cmp    al, 30h
        jne    SHORT loc_2_17E7
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 2
        jmp    NEAR PTR loc_2_1A93
loc_2_17E7:
        mov    ax, word ptr [edi]
        and    ah, 3
        cmp    ah, 3
        jne    SHORT loc_2_1804
        cmp    word ptr [edi + 8], 1
        jb     SHORT loc_2_1804
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0dh
        jmp    NEAR PTR loc_2_1A93
loc_2_1804:
        mov    ax, word ptr [edi]
        and    al, 1fh
        mov    byte ptr [ebp - 2], al
        mov    ax, word ptr [edi]
        mov    cl, byte ptr [edi + 18h]
        shr    ax, 8
        and    al, 3
        mov    byte ptr [ebp - 7], al
        mov    al, dl
        and    al, 7
        mov    byte ptr [ebp - 5], al
        mov    al, cl
        and    al, 70h
        shr    al, 4
        mov    byte ptr [ebp - 3], al
        mov    al, cl
        and    al, 7
        cmp    byte ptr [ebp - 7], 1
        mov    byte ptr [ebp - 4], al
        je     SHORT loc_2_187A
        test   byte ptr [edi], 80h
        je     SHORT loc_2_187A
        mov    ax, word ptr [edi]
        and    ax, 40h
        je     SHORT loc_2_185D
        cmp    byte ptr [ebp - 2], 0fh
        ja     SHORT loc_2_186F
        mov    esi, dword ptr [ebp + 8]
        movzx  ebx, byte ptr [ebp - 2]
        cmp    byte ptr [ebx + esi + 28h], 0
        je     SHORT loc_2_186F
        jmp    SHORT loc_2_1860
loc_2_185D:
        mov    esi, dword ptr [ebp + 8]
loc_2_1860:
        test   ax, ax
        jne    SHORT loc_2_187D
        mov    al, byte ptr [esi + 26h]
        and    al, 1fh
        cmp    al, byte ptr [ebp - 2]
        je     SHORT loc_2_187D
loc_2_186F:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 6
        jmp    NEAR PTR loc_2_1A93
loc_2_187A:
        mov    esi, dword ptr [ebp + 8]
loc_2_187D:
        mov    ebx, dword ptr [esi + 0ch]
        movzx  ax, byte ptr [ebp - 5]
        cmp    word ptr [ebx + 0ah], ax
        jbe    NEAR PTR loc_2_1A8D
        movzx  eax, ax
        test   byte ptr [ebx + eax*2 + 0dh], 80h
        je     NEAR PTR loc_2_1A8D
        mov    al, byte ptr [ebp - 3]
        cmp    byte ptr [ebp - 4], al
        jne    NEAR PTR loc_2_1A85
        movzx  ax, al
        cmp    word ptr [ebx + 0ah], ax
        jbe    NEAR PTR loc_2_1A85
        movzx  eax, ax
        test   byte ptr [ebx + eax*2 + 0dh], 40h
        je     NEAR PTR loc_2_1A85
        push   ecx
        push   edx
        push   esi
        call   sub_2_0B9D
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_2_1A8D
        mov    al, byte ptr [edi + 15h]
        mov    edx, dword ptr [esi + 18h]
        not    al
        mov    bl, byte ptr [edi + 11h]
        and    byte ptr [esi + 25h], al
        mov    cl, bl
        mov    al, byte ptr [edx + 805h]
        and    cl, 70h
        and    al, 0f0h
        shr    cl, 4
        or     cl, al
        test   bl, 80h
        mov    byte ptr [edx + 805h], cl
        je     SHORT loc_2_1910
        lea    ebx, [esi + 8]
        or     byte ptr [edx + 4], 6
        or     byte ptr [ebx + 3], 4
        jmp    SHORT loc_2_191B
loc_2_1910:
        lea    ebx, [esi + 8]
        and    byte ptr [edx + 4], 0f9h
        and    byte ptr [ebx + 3], 0fbh
loc_2_191B:
        add    edx, 803h
        mov    al, byte ptr [edx]
        mov    cl, al
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0d0h
        and    cl, 0fh
        cmp    byte ptr [ebp - 7], 2
        je     SHORT loc_2_193B
        cmp    byte ptr [ebp - 7], 3
        jne    SHORT loc_2_1953
loc_2_193B:
        or     byte ptr [ebp - 1], 20h
        mov    ax, word ptr [edi]
        and    al, 0c0h
        cmp    al, 0c0h
        jne    SHORT loc_2_1953
        movzx  eax, byte ptr [ebp - 2]
        mov    al, byte ptr [eax + esi + 28h]
        or     byte ptr [ebp - 1], al
loc_2_1953:
        test   byte ptr [ebx + 2], 2
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [edx], al
        je     SHORT loc_2_1975
        test   cl, cl
        je     SHORT loc_2_1975
        mov    al, byte ptr [ebp - 1]
        and    al, 0fh
        cmp    al, cl
        je     SHORT loc_2_1975
        push   ecx
        push   esi
        call   sub_2_02AA
        add    esp, 8
loc_2_1975:
        cmp    byte ptr [ebp - 7], 3
        jne    SHORT loc_2_1999
        movzx  ecx, byte ptr [esi + 24h]
        mov    al, byte ptr [edi + 8]
        push   esi
        mov    byte ptr [esi + 27h], al
        imul   ecx, ecx, 24h
        movzx  eax, al
        or     byte ptr [ebx + 3], 8
        db  0FFh, 094h, 0C1h, 01Ch, 000h, 000h, 000h
        jmp    SHORT loc_2_19BB
loc_2_1999:
        mov    eax, dword ptr [ebx]
        test   eax, 8000000h
        je     SHORT loc_2_19BE
        and    eax, 0f7ffffffh
        push   esi
        mov    dword ptr [ebx], eax
        movzx  eax, byte ptr [esi + 24h]
        imul   eax, eax, 24h
        mov    byte ptr [esi + 27h], 0
        db  0FFh, 090h, 020h, 000h, 000h, 000h
loc_2_19BB:
        add    esp, 4
loc_2_19BE:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_2_19DF
        cmp    byte ptr [ebp - 7], 2
        je     SHORT loc_2_19D0
        cmp    byte ptr [ebp - 7], 3
        jne    SHORT loc_2_19DF
loc_2_19D0:
        mov    ax, word ptr [edi]
        and    al, 0c0h
        cmp    al, 80h
        jne    SHORT loc_2_19DF
        or     byte ptr [ebx + 3], 10h
        jmp    SHORT loc_2_19E3
loc_2_19DF:
        and    byte ptr [ebx + 3], 0efh
loc_2_19E3:
        mov    ebx, dword ptr [ebx]
        push   0
        test   ebx, 10000h
        je     SHORT loc_2_1A33
        push   1
        lea    eax, [ebp - 1]
        push   eax
        push   91h
        push   0
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 18h
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [ecx + 8]
        and    al, 30h
        cmp    al, 20h
        je     SHORT loc_2_1A20
        test   ebx, 10000000h
        jne    SHORT loc_2_1A20
        and    byte ptr [ebp - 1], 0efh
        jmp    SHORT loc_2_1A24
loc_2_1A20:
        or     byte ptr [ebp - 1], 10h
loc_2_1A24:
        push   0
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   91h
        jmp    SHORT loc_2_1A71
loc_2_1A33:
        push   2
        lea    eax, [ebp - 8]
        push   eax
        push   3eh
        push   0
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 18h
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [ecx + 8]
        and    al, 30h
        cmp    al, 20h
        je     SHORT loc_2_1A61
        test   ebx, 10000000h
        jne    SHORT loc_2_1A61
        or     byte ptr [ebp - 8], 80h
        jmp    SHORT loc_2_1A67
loc_2_1A61:
        and    word ptr [ebp - 8], 0ff7fh
loc_2_1A67:
        push   0
        lea    eax, [ebp - 8]
        push   2
        push   eax
        push   3eh
loc_2_1A71:
        push   1
        push   dword ptr [esi]
        call   sub_2_21A4
        add    esp, 18h
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_1A93
loc_2_1A85:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0fh
        jmp    SHORT loc_2_1A93
loc_2_1A8D:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0eh
loc_2_1A93:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1A9E
        and    byte ptr [edi + 20h], 0feh
        jmp    SHORT loc_2_1AA2
loc_2_1A9E:
        or     byte ptr [edi + 20h], 1
loc_2_1AA2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_1AA9:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_2_1ABE
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0bh
        jmp    SHORT loc_2_1B36
loc_2_1ABE:
        mov    edi, dword ptr [esp + 10h]
        push   esi
        push   edi
        lea    ebx, [esi + 11h]
        call   sub_2_1B49
        add    esp, 8
        mov    ecx, dword ptr [edi + 18h]
        mov    byte ptr [ebx], 0
        mov    edx, dword ptr [ecx + 8]
        mov    eax, edx
        and    eax, 30h
        cmp    eax, 10h
        jne    SHORT loc_2_1B11
        mov    cl, byte ptr [ecx + 801h]
        mov    al, cl
        shl    al, 4
        not    al
        and    al, 30h
        test   cl, 10h
        mov    byte ptr [ebx], al
        je     SHORT loc_2_1AFC
        or     al, 1
        mov    byte ptr [ebx], al
loc_2_1AFC:
        test   cl, 20h
        je     SHORT loc_2_1B04
        or     byte ptr [ebx], 40h
loc_2_1B04:
        and    cl, 0ch
        cmp    cl, 0ch
        jne    SHORT loc_2_1B30
        or     byte ptr [ebx], 80h
        jmp    SHORT loc_2_1B30
loc_2_1B11:
        cmp    eax, 20h
        jne    SHORT loc_2_1B30
        test   dh, 1
        je     SHORT loc_2_1B1E
        mov    byte ptr [ebx], 30h
loc_2_1B1E:
        mov    al, byte ptr [ebx]
        and    dl, 6
        or     al, 40h
        cmp    dl, 6
        mov    byte ptr [ebx], al
        je     SHORT loc_2_1B30
        or     al, 80h
        mov    byte ptr [ebx], al
loc_2_1B30:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
loc_2_1B36:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1B41
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_1B45
loc_2_1B41:
        or     byte ptr [esi + 20h], 1
loc_2_1B45:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_1B49:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        lea    ecx, [esi + 25h]
        mov    eax, dword ptr [esi + 18h]
        mov    dl, byte ptr [eax + 804h]
        and    dl, 0fh
        or     byte ptr [ecx], dl
        mov    edx, dword ptr [eax]
        and    dl, 6
        cmp    dl, 6
        jne    SHORT loc_2_1B6D
        or     byte ptr [ecx], 8
loc_2_1B6D:
        mov    cl, byte ptr [ecx]
        mov    edx, dword ptr [esp + 10h]
        shl    cl, 4
        mov    dword ptr [eax], 0fh
        mov    byte ptr [edx + 15h], cl
        xor    ecx, ecx
        mov    word ptr [edx], cx
        mov    byte ptr [edx + 14h], cl
        mov    cl, byte ptr [eax + 803h]
        mov    eax, dword ptr [eax + 8]
        and    al, 30h
        cmp    al, 10h
        jne    SHORT loc_2_1BB7
        test   cl, 20h
        mov    eax, 1
        je     SHORT loc_2_1BB0
        mov    eax, dword ptr [esi + 8]
        and    eax, 8000000h
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 3
loc_2_1BB0:
        shl    ax, 8
        mov    word ptr [edx], ax
loc_2_1BB7:
        and    cl, 0fh
        je     SHORT loc_2_1BD3
        xor    al, al
loc_2_1BBE:
        movzx  edi, al
        cmp    byte ptr [edi + esi + 28h], cl
        je     SHORT loc_2_1BCF
        inc    al
        cmp    al, 10h
        jb     SHORT loc_2_1BBE
        jmp    SHORT loc_2_1BE7
loc_2_1BCF:
        or     al, 0c0h
        jmp    SHORT loc_2_1BE0
loc_2_1BD3:
        test   byte ptr [esi + 0bh], 10h
        je     SHORT loc_2_1BE7
        mov    al, byte ptr [esi + 26h]
        and    al, 1fh
        or     al, 80h
loc_2_1BE0:
        movzx  ax, al
        or     word ptr [edx], ax
loc_2_1BE7:
        pop    edi
        pop    esi
        ret
loc_2_1BEA:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        push   ebp
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_2_1C06
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0bh
        jmp    NEAR PTR loc_2_1CF0
loc_2_1C06:
        mov    edi, dword ptr [esp + 18h]
        mov    ebp, dword ptr [edi + 18h]
        test   byte ptr [ebp + 8], 6
        je     SHORT loc_2_1C1E
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 14h
        jmp    NEAR PTR loc_2_1CF0
loc_2_1C1E:
        mov    dword ptr [ebp + 0ch], 4000h
        push   0
        push   493e0h
        call   sub_2_21AC
        test   byte ptr [ebp + 8], 0
        movzx  eax, byte ptr [edi + 24h]
        imul   eax, eax, 24h
        mov    byte ptr [ebp + 806h], 20h
        and    byte ptr [ebp + 803h], 0dfh
        add    esp, 8
        db  08Bh, 080h, 020h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_1C5D
        push   edi
        call   eax
        add    esp, 4
loc_2_1C5D:
        and    byte ptr [edi + 0bh], 0f7h
        mov    byte ptr [edi + 27h], 0
        push   0
        push   10h
        push   edi
        call   sub_2_0B9D
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_1C7E
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0eh
        jmp    SHORT loc_2_1CF0
loc_2_1C7E:
        mov    ebp, dword ptr [edi + 18h]
        xor    ebx, ebx
        add    ebp, 803h
        mov    edi, dword ptr [edi]
        lea    eax, [esp + 12h]
        push   ebx
        push   2
        and    byte ptr [ebp], 0bfh
        push   eax
        push   3eh
        push   ebx
        push   edi
        call   sub_2_21A4
        or     byte ptr [esp + 2ah], 40h
        lea    ecx, [esp + 2ah]
        add    esp, 18h
        push   ebx
        push   2
        push   ecx
        push   3eh
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 18h
        push   ebx
        push   7d0h
        call   sub_2_21AC
        lea    ecx, [esp + 1ah]
        add    esp, 8
        and    word ptr [esp + 12h], 0ffbfh
        push   ebx
        push   2
        push   ecx
        push   3eh
        push   1
        push   edi
        call   sub_2_21A4
        add    esp, 18h
        lea    eax, [esi + 1dh]
        or     byte ptr [ebp], 40h
        mov    byte ptr [eax], bl
loc_2_1CF0:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1CFB
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_1CFF
loc_2_1CFB:
        or     byte ptr [esi + 20h], 1
loc_2_1CFF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_2_1D07:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_2_1D1C
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 15h
        jmp    SHORT loc_2_1D61
loc_2_1D1C:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_21D4
        add    esp, 4
        mov    ebx, eax
        mov    edi, dword ptr [esi]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 14h], 100h
        push   ebx
        push   eax
        push   edi
        call   sub_2_1E64
        mov    ax, word ptr [edi]
        add    esp, 0ch
        cmp    bx, ax
        jae    SHORT loc_2_1D53
        mov    byte ptr [edi + ebx + 4], 0
        jmp    SHORT loc_2_1D5B
loc_2_1D53:
        movzx  eax, ax
        mov    byte ptr [edi + eax + 3], 0
loc_2_1D5B:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
loc_2_1D61:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1D6C
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_1D70
loc_2_1D6C:
        or     byte ptr [esi + 20h], 1
loc_2_1D70:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_1D74:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 14h]
        lea    eax, [ecx + edx - 1]
        cmp    dword ptr [esi + 2], ecx
        ja     SHORT loc_2_1D90
        cmp    dword ptr [esi + 6], ecx
        jae    SHORT loc_2_1D9A
loc_2_1D90:
        test   byte ptr [ebp + 10h], 2
        jne    SHORT loc_2_1DC8
        test   ecx, ecx
        jne    SHORT loc_2_1DC8
loc_2_1D9A:
        cmp    dword ptr [esi + 0ah], edx
        ja     SHORT loc_2_1DA4
        cmp    dword ptr [esi + 0eh], edx
        jae    SHORT loc_2_1DAE
loc_2_1DA4:
        test   byte ptr [ebp + 10h], 2
        jne    SHORT loc_2_1DC4
        test   edx, edx
        jne    SHORT loc_2_1DC4
loc_2_1DAE:
        cmp    dword ptr [esi + 6], eax
        jb     SHORT loc_2_1DC0
        xor    eax, ecx
        test   eax, 0ff000000h
        jne    SHORT loc_2_1DC0
        xor    al, al
        jmp    SHORT loc_2_1DCA
loc_2_1DC0:
        mov    al, 0ah
        jmp    SHORT loc_2_1DCA
loc_2_1DC4:
        mov    al, 0ah
        jmp    SHORT loc_2_1DCA
loc_2_1DC8:
        mov    al, 3
loc_2_1DCA:
        pop    esi
        pop    ebp
        ret
sub_2_1DCD:
        push   ebp
        mov    al, 17h
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    cl, byte ptr [ebp + 8]
        test   cl, 80h
        jne    SHORT loc_2_1E2F
        mov    dl, cl
        and    dl, 7
        and    cl, 78h
        mov    byte ptr [ebp - 2], dl
        mov    byte ptr [ebp - 1], cl
        jne    SHORT loc_2_1DFA
        movzx  ecx, dl
        db  08Ah, 089h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 1], cl
loc_2_1DFA:
        xor    dl, dl
loc_2_1DFC:
        movzx  ecx, dl
        db  08Ah, 089h, 000h, 000h, 000h, 000h
        mov    bl, cl
        and    bl, 7
        mov    byte ptr [ebp - 3], bl
        mov    bl, byte ptr [ebp - 2]
        cmp    bl, byte ptr [ebp - 3]
        jb     SHORT loc_2_1E28
        jne    SHORT loc_2_1E1F
        and    cl, 78h
        cmp    cl, byte ptr [ebp - 1]
        jae    SHORT loc_2_1E28
loc_2_1E1F:
        inc    dl
        cmp    dl, 4
        jb     SHORT loc_2_1DFC
        jmp    SHORT loc_2_1E2F
loc_2_1E28:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], dl
        xor    al, al
loc_2_1E2F:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1E34:
        push   ebp
        mov    dl, 1
        mov    ebp, esp
        mov    cl, byte ptr [esp + 0ch]
        shl    dl, cl
        cmp    cl, 5
        jb     SHORT loc_2_1E46
        add    dl, dl
loc_2_1E46:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 10h], 0
        mov    ecx, dword ptr [eax + 18h]
        je     SHORT loc_2_1E5A
        or     byte ptr [ecx + 806h], dl
        jmp    SHORT loc_2_1E62
loc_2_1E5A:
        not    dl
        and    byte ptr [ecx + 806h], dl
loc_2_1E62:
        pop    ebp
        ret
sub_2_1E64:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        movzx  eax, word ptr [edi]
        mov    word ptr [edi + 2], dx
        cmp    eax, edx
        jge    SHORT loc_2_1E7B
        mov    edx, eax
loc_2_1E7B:
        add    edi, 4
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        ret
sub_2_1E93:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0CDh, 020h, 000h, 000h, 033h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 400h
        sbb    eax, eax
        mov    esp, ebp
        inc    eax
        pop    ebp
        ret
sub_2_1EB0:
        cmp    dword ptr [esp + 8], 2
        push   esi
        push   edi
        jne    SHORT loc_2_1F1E
        mov    edi, 13h
        push   1
        push   38h
        call   sub_2_21EC
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_2_1EF7
        push   esi
        push   dword ptr [esp + 10h]
        call   sub_2_2006
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_1EF7
        push   3
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 18h]
        call   sub_2_21E4
        add    esp, 10h
        mov    edi, eax
loc_2_1EF7:
        cmp    edi, 13h
        jne    SHORT loc_2_1F23
        test   esi, esi
        je     SHORT loc_2_1F23
        lea    ecx, [esi + 38h]
        mov    eax, esi
        push   0
        push   ecx
        push   eax
        call   sub_2_21BC
        add    esp, 0ch
        push   0
        push   esi
        call   sub_2_21DC
        add    esp, 8
        jmp    SHORT loc_2_1F23
loc_2_1F1E:
        mov    edi, 1
loc_2_1F23:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    8
loc_2_1F2A:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        cmp    eax, 1
        je     SHORT loc_2_1F4E
        cmp    eax, 4
        je     SHORT loc_2_1F98
        cmp    eax, 8
        je     SHORT loc_2_1FB7
        mov    esi, 1
        jmp    NEAR PTR loc_2_2000
loc_2_1F4E:
        mov    edi, dword ptr [ebp + 14h]
        push   edi
        call   sub_2_2037
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_1F68
        mov    esi, 13h
        jmp    NEAR PTR loc_2_2000
loc_2_1F68:
        cmp    dword ptr [ebp + 0ch], 1
        jne    NEAR PTR loc_2_2000
        push   edi
        call   sub_2_216C
        add    esp, 4
        test   eax, eax
        je     SHORT loc_2_1F86
        mov    esi, 13h
        jmp    SHORT loc_2_2000
loc_2_1F86:
        push   0
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_2_21F4
        add    esp, 0ch
        jmp    SHORT loc_2_2000
loc_2_1F98:
        mov    edi, dword ptr [ebp + 14h]
        push   0
        lea    eax, [edi + 38h]
        push   eax
        push   edi
        call   sub_2_21BC
        add    esp, 0ch
        push   0
        push   edi
        call   sub_2_21DC
        add    esp, 8
        jmp    SHORT loc_2_2000
loc_2_1FB7:
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp + 0ch]
        and    edx, 7fffffffh
        test   byte ptr [eax + 0bh], 2
        je     SHORT loc_2_2000
        cmp    edx, 4
        je     SHORT loc_2_1FD3
        cmp    edx, 5
        jne    SHORT loc_2_1FE6
loc_2_1FD3:
        mov    ecx, dword ptr [eax + 18h]
        add    ecx, 803h
        test   byte ptr [ecx], 20h
        je     SHORT loc_2_1FE6
        or     byte ptr [ecx], 80h
        jmp    SHORT loc_2_2000
loc_2_1FE6:
        cmp    edx, 6
        je     SHORT loc_2_1FF0
        cmp    edx, 7
        jne    SHORT loc_2_2000
loc_2_1FF0:
        mov    eax, dword ptr [eax + 18h]
        add    eax, 803h
        test   byte ptr [eax], 80h
        je     SHORT loc_2_2000
        and    byte ptr [eax], 7fh
loc_2_2000:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_2006:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 10h]
        lea    eax, [esi + 38h]
        push   eax
        push   esi
        call   sub_2_21CC
        add    esp, 0ch
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_2_2035
        mov    dword ptr [esi + 4], 53534243h
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi], eax
        mov    eax, 1
loc_2_2035:
        pop    esi
        ret
sub_2_2037:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        push   2
        lea    eax, [ebp - 1ch]
        mov    edi, dword ptr [esi]
        push   edi
        push   eax
        call   sub_2_220C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_2163
        push   ebx
        lea    ecx, [ebp - 10h]
        push   ebx
        push   43h
        push   dword ptr [ebp - 1ch]
        push   ecx
        call   sub_2_21FC
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_2_2163
        push   ebx
        lea    ecx, [ebp - 8]
        push   ebx
        push   1
        push   dword ptr [ebp - 10h]
        push   ecx
        call   sub_2_21FC
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_2_2163
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        sub    eax, ecx
        inc    eax
        push   eax
        push   ecx
        call   sub_2_2204
        add    esp, 0ch
        mov    dword ptr [esi + 18h], eax
        cmp    eax, -1
        jne    SHORT loc_2_20BB
        mov    dword ptr [esi + 18h], ebx
        jmp    NEAR PTR loc_2_2163
loc_2_20BB:
        push   0
        lea    ecx, [ebp - 8]
        push   0
        push   4
        push   dword ptr [ebp - 10h]
        push   ecx
        call   sub_2_21FC
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_20DD
        cmp    eax, 0fh
        jne    NEAR PTR loc_2_2163
loc_2_20DD:
        mov    dword ptr [ebp - 0ch], 4
        test   eax, eax
        jne    SHORT loc_2_20F3
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [esi + 26h], al
        jmp    SHORT loc_2_20F7
loc_2_20F3:
        mov    byte ptr [esi + 26h], 0
loc_2_20F7:
        push   1
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 18h]
        push   ecx
        push   3
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   0
        push   edi
        call   sub_2_21B4
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_2_2129
        cmp    dword ptr [ebp - 0ch], 4
        jne    SHORT loc_2_2129
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        or     dword ptr [esi + 8], eax
loc_2_2129:
        db  0CDh, 020h, 000h, 000h, 026h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0
        je     SHORT loc_2_2155
        lea    eax, [ebp - 14h]
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 026h, 000h
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_2155
        test   byte ptr [ebp - 14h], 0c0h
        je     SHORT loc_2_2155
        or     byte ptr [esi + 0bh], 2
loc_2_2155:
        push   esi
        mov    ebx, 1
        call   sub_2_100B
        add    esp, 4
loc_2_2163:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_216C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        sub    esp, 8
        mov    ebx, esp
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebx], 3
        mov    dword ptr [ebx + 2], esi
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        xor    edx, edx
        mov    ecx, 6
        mov    eax, 32h
        db  0CDh, 020h, 001h, 000h, 07Ch, 009h
        add    esp, 8
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
        db  0CCh, 0CCh
sub_2_21A4:
        db  0CDh, 020h, 054h, 080h, 033h, 000h
        int3
        int3
sub_2_21AC:
        db  0CDh, 020h, 015h, 080h, 033h, 000h
        int3
        int3
sub_2_21B4:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_2_21BC:
        db  0CDh, 020h, 095h, 081h, 001h, 000h
        int3
        int3
sub_2_21C4:
        db  0CDh, 020h, 019h, 080h, 003h, 000h
        int3
        int3
sub_2_21CC:
        db  0CDh, 020h, 094h, 081h, 001h, 000h
        int3
        int3
sub_2_21D4:
        db  0CDh, 020h, 07Ch, 081h, 001h, 000h
        int3
        int3
sub_2_21DC:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_2_21E4:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        int3
        int3
sub_2_21EC:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_2_21F4:
        db  0CDh, 020h, 049h, 080h, 033h, 000h
        int3
        int3
sub_2_21FC:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_2_2204:
        db  0CDh, 020h, 06Ch, 080h, 001h, 000h
        int3
        int3
sub_2_220C:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
_PTEXT ends
        end
