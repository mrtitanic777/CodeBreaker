; socketsv_pcode_02.asm -- SOCKETSV object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 3ah], 82h
        jne    SHORT loc_2_0081
        mov    edi, dword ptr [esi + 18h]
        push   1fh
        mov    al, byte ptr [esi + 28h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0023:
        add    esp, 0ch
        mov    cl, byte ptr [esi + 28h]
        mov    byte ptr [ebp - 1], al
        push   1fh
        push   ecx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0035:
        add    esp, 0ch
        xor    al, byte ptr [ebp - 1]
        and    al, 0c0h
        cmp    al, 0c0h
        jne    SHORT loc_2_0081
        push   1fh
        mov    al, byte ptr [esi + 28h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_004D:
        add    esp, 0ch
        xor    al, byte ptr [ebp - 1]
        test   al, 0c0h
        jne    SHORT loc_2_0081
        push   ebx
        lea    eax, [ebp - 4]
        push   ebx
        mov    dword ptr [ebp - 8], ebx
        lea    ebx, [ebp - 8]
        push   ebx
        push   eax
        push   dword ptr [esi]
        call   sub_2_2A24
        mov    byte ptr [esi + 39h], 2
        add    esp, 14h
        cmp    dword ptr [ebp - 4], 4
        je     SHORT loc_2_007C
        mov    byte ptr [esi + 39h], 1
loc_2_007C:
        mov    ebx, 1
loc_2_0081:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_008A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 1
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
        cmp    byte ptr [esi + 39h], 2
        jne    SHORT loc_2_00B8
        db  0C7h, 046h, 014h, 000h, 000h, 000h, 000h
loc_2_00B8:
        or     byte ptr [esi + 0bh], 4
        mov    bl, byte ptr [esi + 38h]
        cmp    bl, 1
        jbe    SHORT loc_2_00DD
        push   1fh
        mov    al, byte ptr [esi + 28h]
        push   eax
        push   dword ptr [esi + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_00D2:
        add    esp, 0ch
        test   al, 20h
        jne    SHORT loc_2_00DD
        mov    byte ptr [esi + 3bh], 0
loc_2_00DD:
        mov    byte ptr [ebp - 1], 0
        test   bl, bl
        je     NEAR PTR loc_2_019E
loc_2_00E9:
        movzx  ebx, byte ptr [ebp - 1]
        push   16h
        mov    edi, dword ptr [esi + ebx*4 + 18h]
        mov    al, byte ptr [ebx + esi + 28h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_00FE:
        add    esp, 0ch
        or     al, 10h
        mov    cl, byte ptr [ebx + esi + 28h]
        push   eax
        push   16h
        push   ecx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0111:
        add    esp, 10h
        cmp    byte ptr [esi + 39h], 2
        jne    SHORT loc_2_0149
        push   25h
        mov    al, byte ptr [ebx + esi + 28h]
        push   2eh
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0129:
        add    esp, 10h
        mov    cl, byte ptr [ebx + esi + 28h]
        push   2fh
        push   ecx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0139:
        add    esp, 0ch
        and    al, 3
        cmp    al, 3
        jne    SHORT loc_2_0149
        mov    dword ptr [ebp - 8], 0
loc_2_0149:
        cmp    dword ptr [ebp - 8], 1
        jne    SHORT loc_2_018F
        cmp    byte ptr [esi + 39h], 2
        db  0B8h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_015F
        db  0B8h, 000h, 000h, 000h, 000h
loc_2_015F:
        push   eax
        mov    al, byte ptr [ebx + esi + 28h]
        push   eax
        push   edi
        call   sub_2_01A8
        add    esp, 0ch
        cmp    byte ptr [esi + 39h], 2
        je     SHORT loc_2_018F
        test   byte ptr [esi + 8], 2
        jne    SHORT loc_2_018F
        push   1
        mov    al, byte ptr [ebx + esi + 28h]
        push   1eh
        push   eax
        push   dword ptr [esi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_018C:
        add    esp, 10h
loc_2_018F:
        inc    byte ptr [ebp - 1]
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [esi + 38h], al
        ja     NEAR PTR loc_2_00E9
loc_2_019E:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_01A8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 10h]
        cmp    byte ptr [esi], 0
        je     SHORT loc_2_01EE
loc_2_01B5:
        mov    al, byte ptr [esi]
        mov    bl, al
        and    bl, 7fh
        test   al, 80h
        jne    SHORT loc_2_01D3
        push   ebx
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_01CC:
        add    esp, 0ch
        test   al, al
        jne    SHORT loc_2_01E6
loc_2_01D3:
        mov    al, byte ptr [esi + 1]
        push   eax
        push   ebx
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_01E3:
        add    esp, 10h
loc_2_01E6:
        add    esi, 2
        cmp    byte ptr [esi], 0
        jne    SHORT loc_2_01B5
loc_2_01EE:
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_2_01F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 1
        push   esi
        push   edi
        movzx  esi, byte ptr [ebp + 0ch]
        push   2
        mov    edi, dword ptr [eax + esi*4 + 18h]
        mov    al, byte ptr [esi + eax + 28h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_021A:
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], al
        push   16h
        mov    al, byte ptr [esi + ecx + 28h]
        mov    bl, 0
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0232:
        add    esp, 0ch
        mov    cl, byte ptr [ebp + 10h]
        cmp    cl, 1
        lea    edx, [ecx - 3]
        adc    bl, 0ffh
        and    bl, 10h
        cmp    dl, 1
        sbb    dl, dl
        and    dl, 2
        test   cl, cl
        je     SHORT loc_2_025A
        cmp    byte ptr [ebp + 14h], cl
        jne    SHORT loc_2_025A
        or     bl, 1
        jmp    SHORT loc_2_0272
loc_2_025A:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_026F
        cmp    byte ptr [ebp + 14h], 2
        je     SHORT loc_2_026F
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_0272
loc_2_026F:
        or     bl, byte ptr [ebp + 14h]
loc_2_0272:
        cmp    dword ptr [ebp - 8], 1
        jne    SHORT loc_2_02F3
        mov    cl, byte ptr [ebp - 1]
        and    cl, 0a0h
        or     bl, cl
        mov    cl, al
        and    cl, 0fdh
        or     dl, cl
        test   bl, 10h
        jne    SHORT loc_2_028F
        and    bl, 5fh
loc_2_028F:
        cmp    bl, byte ptr [ebp - 1]
        jne    SHORT loc_2_0298
        cmp    dl, al
        je     SHORT loc_2_02F3
loc_2_0298:
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   16h
        mov    al, byte ptr [esi + eax + 28h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_02A9:
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        push   ebx
        mov    al, byte ptr [esi + ecx + 28h]
        push   2
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_02BD:
        add    esp, 10h
        push   0
        push   493e0h
        call   sub_2_2A2C
        add    esp, 8
        test   bl, 10h
        je     SHORT loc_2_02F3
        mov    al, bl
        and    al, 0a0h
        cmp    al, 0a0h
        je     SHORT loc_2_02F3
        or     bl, 0a0h
        mov    eax, dword ptr [ebp + 8]
        push   ebx
        push   2
        mov    al, byte ptr [esi + eax + 28h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_02F0:
        add    esp, 10h
loc_2_02F3:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_02FD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        movzx  edi, byte ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        push   2
        mov    al, byte ptr [edi + esi + 28h]
        push   eax
        push   dword ptr [esi + edi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_031A:
        add    esp, 0ch
        mov    bl, al
        mov    cl, byte ptr [edi + esi + 28h]
        push   16h
        push   ecx
        push   dword ptr [esi + edi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_032F:
        add    esp, 0ch
        test   bl, 10h
        jne    SHORT loc_2_033F
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_0350
loc_2_033F:
        and    al, 2
        cmp    al, 1
        mov    eax, dword ptr [ebp + 10h]
        sbb    cl, cl
        and    cl, 0feh
        add    cl, 3
        mov    byte ptr [eax], cl
loc_2_0350:
        and    bl, 3
        mov    ecx, dword ptr [ebp + 14h]
        cmp    bl, 1
        mov    byte ptr [ecx], bl
        jne    SHORT loc_2_0363
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx], al
        jmp    SHORT loc_2_036B
loc_2_0363:
        cmp    bl, 3
        jne    SHORT loc_2_036B
        mov    byte ptr [ecx], 0
loc_2_036B:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax], cl
        pop    ebp
        ret
loc_2_0377:
        movzx  eax, byte ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        push   16h
        mov    dl, byte ptr [eax + ecx + 28h]
        push   edx
        push   dword ptr [ecx + eax*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0390:
        add    esp, 0ch
        and    al, 1
        cmp    al, 1
        mov    al, 2
        adc    al, 0ffh
        ret
loc_2_039C:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    bl, byte ptr [esp + 0ch]
        push   esi
        movzx  ecx, bl
        push   0ah
        push   2eh
        mov    esi, dword ptr [eax + ecx*4 + 18h]
        push   40h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_03B9:
        add    esp, 10h
        push   2fh
        push   40h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_03C6:
        add    esp, 0ch
        lea    ecx, [ebx*2]
        shr    al, cl
        and    al, 3
        pop    esi
        sub    al, 3
        pop    ebx
        cmp    al, 1
        sbb    al, al
        add    al, 2
        ret
loc_2_03DF:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esp + 0ch]
        shr    dword ptr [esp + 14h], 18h
        mov    ebp, esp
        push   ebx
        push   esi
        mov    bl, byte ptr [ecx + eax + 28h]
        mov    esi, dword ptr [eax + ecx*4 + 18h]
        mov    al, byte ptr [ebp + 10h]
        add    al, 5
        push   eax
        push   2eh
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0409:
        add    esp, 10h
        push   dword ptr [ebp + 14h]
        push   2fh
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0418:
        add    esp, 10h
        xor    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_2_0421:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        movzx  ecx, byte ptr [esp + 0ch]
        push   esi
        mov    bl, byte ptr [ecx + eax + 28h]
        mov    esi, dword ptr [eax + ecx*4 + 18h]
        mov    al, byte ptr [esp + 14h]
        add    al, 5
        push   eax
        push   2eh
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0444:
        add    esp, 10h
        push   2fh
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0450:
        movzx  eax, al
        shl    eax, 18h
        add    esp, 0ch
        pop    esi
        pop    ebx
        ret
sub_2_045C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    ebp, ebp
        mov    edi, dword ptr [esp + 18h]
        test   byte ptr [edi + 0bh], 10h
        je     SHORT loc_2_04DE
        mov    bl, byte ptr [edi + 28h]
        mov    esi, dword ptr [edi + 18h]
        add    esi, 8000h
        push   ebp
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0483:
        mov    byte ptr [esp + 1fh], al
        add    esp, 0ch
        push   55h
        push   ebp
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0494:
        add    esp, 10h
        push   ebp
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_049F:
        add    esp, 0ch
        cmp    al, 55h
        jne    SHORT loc_2_04CE
        push   0aah
        push   ebp
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_04B3:
        add    esp, 10h
        push   ebp
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_04BE:
        add    esp, 0ch
        cmp    al, 0aah
        jne    SHORT loc_2_04CE
        mov    byte ptr [edi + 39h], 3
        mov    ebp, 1
loc_2_04CE:
        push   dword ptr [esp + 13h]
        push   0
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_04DB:
        add    esp, 10h
loc_2_04DE:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_2_04E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
        or     byte ptr [esi + 0bh], 4
        cmp    byte ptr [esi + 38h], 0
        je     SHORT loc_2_0557
loc_2_0510:
        movzx  eax, byte ptr [ebp - 1]
        push   0
        push   0
        mov    bl, byte ptr [eax + esi + 28h]
        mov    edi, dword ptr [esi + eax*4 + 18h]
        add    edi, 8000h
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_052D:
        add    esp, 10h
        push   0
        push   1
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_053B:
        add    esp, 10h
        push   10h
        push   2
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0549:
        add    esp, 10h
        mov    al, byte ptr [esi + 38h]
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], al
        jb     SHORT loc_2_0510
loc_2_0557:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0563:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  edi, byte ptr [ebp + 0ch]
        push   2
        mov    al, byte ptr [esi + edi + 28h]
        push   eax
        mov    bl, 10h
        push   dword ptr [esi + edi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_058C:
        add    esp, 0ch
        mov    cl, byte ptr [ebp + 10h]
        cmp    cl, 1
        je     SHORT loc_2_05A2
        sub    cl, 3
        cmp    cl, 1
        sbb    bl, bl
        and    bl, 18h
loc_2_05A2:
        mov    cl, byte ptr [ebp + 14h]
        cmp    cl, 1
        jne    SHORT loc_2_05AF
        or     bl, 1
        jmp    SHORT loc_2_05C4
loc_2_05AF:
        cmp    cl, 2
        jne    SHORT loc_2_05B9
        or     bl, 2
        jmp    SHORT loc_2_05C4
loc_2_05B9:
        test   cl, cl
        je     SHORT loc_2_05C4
        mov    dword ptr [ebp - 4], 0
loc_2_05C4:
        cmp    dword ptr [ebp - 4], 1
        jne    SHORT loc_2_0624
        mov    cl, al
        and    cl, 0a0h
        or     bl, cl
        test   bl, 10h
        jne    SHORT loc_2_05D9
        and    bl, 5fh
loc_2_05D9:
        cmp    bl, al
        je     SHORT loc_2_0624
        push   ebx
        mov    al, byte ptr [esi + edi + 28h]
        push   2
        push   eax
        push   dword ptr [esi + edi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_05EE:
        add    esp, 10h
        push   0
        push   493e0h
        call   sub_2_2A2C
        add    esp, 8
        test   bl, 10h
        je     SHORT loc_2_0624
        mov    al, bl
        and    al, 0a0h
        cmp    al, 0a0h
        je     SHORT loc_2_0624
        or     bl, 0a0h
        mov    al, byte ptr [esi + edi + 28h]
        push   ebx
        push   2
        push   eax
        push   dword ptr [esi + edi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0621:
        add    esp, 10h
loc_2_0624:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_062E:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esp + 0ch]
        mov    ebp, esp
        push   2
        mov    dl, byte ptr [eax + ecx + 28h]
        push   edx
        push   dword ptr [eax + ecx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_064A:
        add    esp, 0ch
        mov    dl, al
        and    dl, 18h
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dl, 10h
        mov    byte ptr [ecx], dl
        jne    SHORT loc_2_0661
        mov    byte ptr [ecx], 1
        jmp    SHORT loc_2_066C
loc_2_0661:
        cmp    dl, 18h
        mov    byte ptr [ecx], 3
        je     SHORT loc_2_066C
        mov    byte ptr [ecx], 0
loc_2_066C:
        and    al, 3
        mov    ecx, dword ptr [ebp + 14h]
        cmp    al, 1
        mov    byte ptr [ecx], al
        jne    SHORT loc_2_067C
        mov    byte ptr [ecx], 1
        jmp    SHORT loc_2_0686
loc_2_067C:
        cmp    al, 2
        mov    byte ptr [ecx], 2
        je     SHORT loc_2_0686
        mov    byte ptr [ecx], 0
loc_2_0686:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 18h]
        pop    ebp
        mov    byte ptr [eax], cl
        ret
loc_2_068F:
        movzx  ecx, byte ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        push   3
        mov    dl, byte ptr [eax + ecx + 28h]
        mov    eax, dword ptr [eax + ecx*4 + 18h]
        push   edx
        add    eax, 8000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_06AE:
        add    esp, 0ch
        and    al, 4
        cmp    al, 1
        mov    al, 2
        adc    al, 0ffh
        ret
sub_2_06BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 3ah], 83h
        jne    NEAR PTR loc_2_0793
        mov    eax, dword ptr [edi + 18h]
        mov    cl, byte ptr [edi + 28h]
        mov    dword ptr [ebp - 8], eax
        mov    byte ptr [ebp - 1], cl
        mov    edx, dword ptr [ebp - 8]
        mov    al, 0eh
        add    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 37h
        add    al, byte ptr [ebp - 1]
        out    dx, al
        push   esi
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_06F9:
        add    esp, 0ch
        mov    byte ptr [ebp - 2], al
        xor    al, 8
        push   eax
        push   esi
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_070E:
        add    esp, 10h
        push   esi
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_071D:
        add    esp, 0ch
        xor    al, byte ptr [ebp - 2]
        test   al, 8
        je     SHORT loc_2_0780
        push   3ah
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0734:
        add    esp, 0ch
        mov    bl, al
        and    bl, 3fh
        mov    al, bl
        or     al, 0c0h
        push   eax
        push   3ah
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_074E:
        add    esp, 10h
        push   esi
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_075D:
        add    esp, 0ch
        and    al, 0fh
        cmp    al, 0ch
        jne    SHORT loc_2_076F
        mov    byte ptr [edi + 39h], 4
        mov    esi, 1
loc_2_076F:
        push   ebx
        push   3ah
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_077D:
        add    esp, 10h
loc_2_0780:
        push   dword ptr [ebp - 2]
        push   0
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0790:
        add    esp, 10h
loc_2_0793:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_079C:
        push   esi
        push   2fh
        mov    esi, dword ptr [esp + 0ch]
        mov    al, byte ptr [esi + 28h]
        push   eax
        push   dword ptr [esi + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_07AF:
        add    esp, 0ch
        test   al, 40h
        je     SHORT loc_2_07C4
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
loc_2_07C4:
        mov    eax, 1
        pop    esi
        ret
loc_2_07CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 1
        push   esi
        push   edi
        movzx  esi, byte ptr [ebp + 0ch]
        push   2
        mov    edi, dword ptr [eax + esi*4 + 18h]
        mov    al, byte ptr [eax + esi + 28h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_07F3:
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], al
        push   2fh
        mov    al, byte ptr [ecx + esi + 28h]
        mov    bl, 0
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_080B:
        add    esp, 0ch
        mov    cl, byte ptr [ebp + 10h]
        cmp    cl, 1
        lea    edx, [ecx - 3]
        adc    bl, 0ffh
        and    bl, 10h
        cmp    dl, 1
        sbb    dl, dl
        neg    dl
        test   cl, cl
        je     SHORT loc_2_0832
        cmp    byte ptr [ebp + 14h], cl
        jne    SHORT loc_2_0832
        or     bl, 1
        jmp    SHORT loc_2_084A
loc_2_0832:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_0847
        cmp    byte ptr [ebp + 14h], 2
        je     SHORT loc_2_0847
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_084A
loc_2_0847:
        or     bl, byte ptr [ebp + 14h]
loc_2_084A:
        cmp    dword ptr [ebp - 8], 1
        jne    SHORT loc_2_08CB
        mov    cl, byte ptr [ebp - 1]
        and    cl, 0a0h
        or     bl, cl
        mov    cl, al
        and    cl, 0fch
        or     dl, cl
        test   bl, 10h
        jne    SHORT loc_2_0867
        and    bl, 5fh
loc_2_0867:
        cmp    bl, byte ptr [ebp - 1]
        jne    SHORT loc_2_0870
        cmp    dl, al
        je     SHORT loc_2_08CB
loc_2_0870:
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   2fh
        mov    al, byte ptr [eax + esi + 28h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0881:
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        push   ebx
        mov    al, byte ptr [ecx + esi + 28h]
        push   2
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0895:
        add    esp, 10h
        push   0
        push   493e0h
        call   sub_2_2A2C
        add    esp, 8
        test   bl, 10h
        je     SHORT loc_2_08CB
        mov    al, bl
        and    al, 0a0h
        cmp    al, 0a0h
        je     SHORT loc_2_08CB
        or     bl, 0a0h
        mov    eax, dword ptr [ebp + 8]
        push   ebx
        push   2
        mov    al, byte ptr [eax + esi + 28h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_08C8:
        add    esp, 10h
loc_2_08CB:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_08D5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        movzx  esi, byte ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        push   2
        mov    al, byte ptr [esi + edi + 28h]
        push   eax
        push   dword ptr [edi + esi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_08F2:
        add    esp, 0ch
        mov    bl, al
        mov    cl, byte ptr [esi + edi + 28h]
        push   2fh
        push   ecx
        push   dword ptr [edi + esi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0907:
        add    esp, 0ch
        and    al, 3
        test   bl, 10h
        jne    SHORT loc_2_0919
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_0928
loc_2_0919:
        cmp    al, 1
        mov    eax, dword ptr [ebp + 10h]
        sbb    cl, cl
        and    cl, 0feh
        add    cl, 3
        mov    byte ptr [eax], cl
loc_2_0928:
        and    bl, 3
        mov    ecx, dword ptr [ebp + 14h]
        cmp    bl, 1
        mov    byte ptr [ecx], bl
        jne    SHORT loc_2_093B
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx], al
        jmp    SHORT loc_2_0943
loc_2_093B:
        cmp    bl, 3
        jne    SHORT loc_2_0943
        mov    byte ptr [ecx], 0
loc_2_0943:
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax], cl
        pop    ebp
        ret
loc_2_094F:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    bl, byte ptr [esp + 0ch]
        push   1fh
        movzx  ecx, bl
        mov    dl, byte ptr [eax + ecx + 28h]
        push   edx
        push   dword ptr [eax + ecx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_096B:
        add    esp, 0ch
        lea    ecx, [ebx*2]
        shr    al, cl
        and    al, 3
        pop    ebx
        sub    al, 3
        cmp    al, 1
        sbb    al, al
        add    al, 2
        ret
sub_2_0983:
        xor    eax, eax
        mov    ecx, dword ptr [esp + 4]
        cmp    byte ptr [ecx + 3ah], 8ah
        jne    SHORT loc_2_0998
        mov    byte ptr [ecx + 39h], 5
        mov    eax, 1
loc_2_0998:
        ret
loc_2_0999:
        mov    eax, dword ptr [esp + 4]
        db  0C7h, 040h, 00Ch, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        mov    eax, 1
        ret
loc_2_09B1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        push   ebx
        push   esi
        movzx  esi, byte ptr [ebp + 0ch]
        push   edi
        push   2
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + edi + 28h]
        mov    bl, 0
        push   eax
        push   dword ptr [edi + esi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_09DA:
        add    esp, 0ch
        mov    cl, byte ptr [ebp + 10h]
        cmp    cl, 1
        adc    bl, 0ffh
        and    bl, 10h
        cmp    cl, 1
        jne    SHORT loc_2_09F3
        or     bl, 4
        jmp    SHORT loc_2_0A00
loc_2_09F3:
        lea    edx, [ecx - 3]
        cmp    dl, 1
        sbb    dl, dl
        and    dl, 0ch
        or     bl, dl
loc_2_0A00:
        test   cl, cl
        je     SHORT loc_2_0A0E
        cmp    byte ptr [ebp + 14h], cl
        jne    SHORT loc_2_0A0E
        or     bl, 3
        jmp    SHORT loc_2_0A31
loc_2_0A0E:
        cmp    byte ptr [ebp + 14h], 1
        jne    SHORT loc_2_0A19
        or     bl, 1
        jmp    SHORT loc_2_0A31
loc_2_0A19:
        cmp    byte ptr [ebp + 14h], 2
        jne    SHORT loc_2_0A24
        or     bl, 2
        jmp    SHORT loc_2_0A31
loc_2_0A24:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_0A31
        mov    dword ptr [ebp - 4], 0
loc_2_0A31:
        cmp    dword ptr [ebp - 4], 1
        jne    SHORT loc_2_0A94
        mov    cl, al
        and    cl, 0a0h
        or     bl, cl
        test   bl, 10h
        jne    SHORT loc_2_0A46
        and    bl, 5fh
loc_2_0A46:
        or     bl, 40h
        cmp    bl, al
        je     SHORT loc_2_0A94
        push   ebx
        mov    al, byte ptr [esi + edi + 28h]
        push   2
        push   eax
        push   dword ptr [edi + esi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0A5E:
        add    esp, 10h
        push   0
        push   493e0h
        call   sub_2_2A2C
        add    esp, 8
        test   bl, 10h
        je     SHORT loc_2_0A94
        mov    al, bl
        and    al, 0a0h
        cmp    al, 0a0h
        je     SHORT loc_2_0A94
        or     bl, 0a0h
        mov    al, byte ptr [esi + edi + 28h]
        push   ebx
        push   2
        push   eax
        push   dword ptr [edi + esi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0A91:
        add    esp, 10h
loc_2_0A94:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0A9E:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esp + 0ch]
        mov    ebp, esp
        push   2
        mov    dl, byte ptr [ecx + eax + 28h]
        push   edx
        push   dword ptr [eax + ecx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0ABA:
        add    esp, 0ch
        test   al, 10h
        je     SHORT loc_2_0ADF
        mov    dl, al
        mov    ecx, dword ptr [ebp + 10h]
        and    dl, 0ch
        mov    byte ptr [ecx], dl
        cmp    dl, 4
        jne    SHORT loc_2_0AD5
        mov    byte ptr [ecx], 1
        jmp    SHORT loc_2_0AE5
loc_2_0AD5:
        cmp    dl, 0ch
        jne    SHORT loc_2_0AE2
        mov    byte ptr [ecx], 3
        jmp    SHORT loc_2_0AE5
loc_2_0ADF:
        mov    ecx, dword ptr [ebp + 10h]
loc_2_0AE2:
        mov    byte ptr [ecx], 0
loc_2_0AE5:
        and    al, 3
        mov    edx, dword ptr [ebp + 14h]
        cmp    al, 3
        mov    byte ptr [edx], al
        jne    SHORT loc_2_0AF6
        mov    al, byte ptr [ecx]
        mov    byte ptr [edx], al
        jmp    SHORT loc_2_0B09
loc_2_0AF6:
        cmp    al, 1
        jne    SHORT loc_2_0AFF
        mov    byte ptr [edx], 1
        jmp    SHORT loc_2_0B09
loc_2_0AFF:
        cmp    al, 2
        mov    byte ptr [edx], 2
        je     SHORT loc_2_0B09
        mov    byte ptr [edx], 0
loc_2_0B09:
        mov    cl, byte ptr [edx]
        mov    eax, dword ptr [ebp + 18h]
        pop    ebp
        mov    byte ptr [eax], cl
        ret
loc_2_0B12:
        movzx  ecx, byte ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        push   17h
        mov    dl, byte ptr [eax + ecx + 28h]
        push   edx
        push   dword ptr [eax + ecx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0B2B:
        add    esp, 0ch
        and    al, 3
        sub    al, 3
        cmp    al, 1
        sbb    al, al
        add    al, 2
        ret
sub_2_0B39:
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
        jne    NEAR PTR loc_2_0C08
        or     eax, ecx
        lea    edi, [esi + 34h]
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
        call   sub_2_2A34
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_2_0BB2
        cmp    dword ptr [ebp - 4], 10h
        jne    SHORT loc_2_0BB2
        xor    cl, cl
loc_2_0B92:
        movzx  edx, cl
        mov    al, byte ptr [ebp + edx - 18h]
        mov    byte ptr [edx + esi + 3dh], al
        test   al, al
        je     SHORT loc_2_0BAB
        mov    ax, 1
        shl    ax, cl
        or     word ptr [edi], ax
loc_2_0BAB:
        inc    cl
        cmp    cl, 10h
        jb     SHORT loc_2_0B92
loc_2_0BB2:
        xor    ebx, ebx
        cmp    word ptr [edi], bx
        jne    SHORT loc_2_0C08
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
        call   sub_2_2A34
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_2_0C08
        cmp    dword ptr [ebp - 8], ebx
        je     SHORT loc_2_0C08
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [edi], ax
loc_2_0BEC:
        mov    eax, 1
        mov    cl, bl
        shl    eax, cl
        test   dword ptr [ebp - 8], eax
        je     SHORT loc_2_0C01
        movzx  eax, bl
        mov    byte ptr [eax + esi + 3dh], bl
loc_2_0C01:
        inc    bl
        cmp    bl, 10h
        jb     SHORT loc_2_0BEC
loc_2_0C08:
        mov    ax, word ptr [esi + 34h]
        lea    edi, [esi + 30h]
        test   ax, ax
        jne    SHORT loc_2_0C1D
        push   esi
        call   sub_2_0DC6
        add    esp, 4
loc_2_0C1D:
        or     word ptr [edi], ax
        xor    ecx, ecx
        cmp    word ptr [edi], cx
        jne    SHORT loc_2_0C4D
        cmp    dword ptr [esi + 18h], ecx
        je     SHORT loc_2_0C4D
        mov    word ptr [edi], 698h
loc_2_0C31:
        movzx  eax, word ptr [edi]
        mov    edx, 1
        shl    edx, cl
        test   edx, eax
        je     SHORT loc_2_0C46
        movzx  eax, cl
        mov    byte ptr [eax + esi + 3dh], cl
loc_2_0C46:
        inc    cl
        cmp    cl, 10h
        jb     SHORT loc_2_0C31
loc_2_0C4D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0C54:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 3ah], 83h
        jne    NEAR PTR loc_2_0DBF
        test   byte ptr [esi + 3bh], 1fh
        je     NEAR PTR loc_2_0DBF
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_2A44
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_2_0DBF
        pushfd
        cli
        in     al, 0a1h
        mov    ah, al
        in     al, 21h
        push   eax
        mov    al, 0ffh
        out    0a1h, al
        out    21h, al
        mov    byte ptr [ebp - 1], 0
        mov    bl, byte ptr [esi + 38h]
        test   bl, bl
        je     SHORT loc_2_0D02
loc_2_0CAA:
        movzx  ecx, byte ptr [ebp - 1]
        movzx  edx, byte ptr [ebp - 1]
        mov    al, byte ptr [esi + ecx + 28h]
        push   3
        mov    edi, dword ptr [esi + edx*4 + 18h]
        mov    byte ptr [ebp - 8], al
        push   dword ptr [ebp - 8]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0CC8:
        movzx  ecx, byte ptr [ebp - 1]
        add    esp, 0ch
        mov    byte ptr [ebp + ecx - 10h], al
        push   40h
        push   3
        push   dword ptr [ebp - 8]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0CE0:
        add    esp, 10h
        mov    al, byte ptr [esi + 3bh]
        shl    al, 4
        or     al, 8
        push   eax
        push   5
        push   dword ptr [ebp - 8]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0CF7:
        add    esp, 10h
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], bl
        jb     SHORT loc_2_0CAA
loc_2_0D02:
        mov    byte ptr [ebp - 1], 0
        test   bl, bl
        je     SHORT loc_2_0D5A
loc_2_0D0A:
        movzx  edi, byte ptr [ebp - 1]
        mov    al, byte ptr [esi + 3bh]
        mov    cl, byte ptr [esi + edi + 28h]
        and    al, 1fh
        push   eax
        push   ecx
        push   dword ptr [esi + edi*4 + 18h]
        mov    al, byte ptr [esi + 3ah]
        push   eax
        call   sub_2_0EDD
        add    esp, 10h
        test   ax, ax
        je     SHORT loc_2_0D4E
        mov    al, byte ptr [esi + 3bh]
        mov    cl, byte ptr [esi + edi + 28h]
        and    al, 1fh
        push   eax
        push   ecx
        push   dword ptr [esi + edi*4 + 18h]
        mov    al, byte ptr [esi + 3ah]
        push   eax
        call   sub_2_0EDD
        add    esp, 10h
        test   ax, ax
        jne    SHORT loc_2_0D52
loc_2_0D4E:
        mov    byte ptr [esi + 3bh], 0
loc_2_0D52:
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], bl
        jb     SHORT loc_2_0D0A
loc_2_0D5A:
        mov    byte ptr [ebp - 1], 0
        test   bl, bl
        je     SHORT loc_2_0DA3
loc_2_0D62:
        movzx  eax, byte ptr [ebp - 1]
        movzx  ecx, byte ptr [ebp - 1]
        mov    al, byte ptr [esi + eax + 28h]
        push   0
        mov    edi, dword ptr [esi + ecx*4 + 18h]
        push   5
        mov    byte ptr [ebp - 0ch], al
        push   dword ptr [ebp - 0ch]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0D82:
        add    esp, 10h
        movzx  eax, byte ptr [ebp - 1]
        push   dword ptr [ebp + eax - 10h]
        push   3
        push   dword ptr [ebp - 0ch]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0D98:
        add    esp, 10h
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], bl
        jb     SHORT loc_2_0D62
loc_2_0DA3:
        pop    eax
        out    21h, al
        mov    al, ah
        out    0a1h, al
        popfd
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_2A3C
        add    esp, 0ch
loc_2_0DBF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0DC6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    word ptr [ebp - 4], bx
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_2A44
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_2_0ED2
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 18h]
        test   edi, edi
        je     SHORT loc_2_0E08
        mov    al, byte ptr [esi + 28h]
        mov    cl, byte ptr [esi + 3ah]
        mov    byte ptr [ebp - 2], al
        mov    byte ptr [ebp - 1], cl
        jmp    SHORT loc_2_0E25
loc_2_0E08:
        mov    edi, 3e0h
        push   0
        mov    byte ptr [ebp - 2], 0
        push   edi
        call   sub_2_12FD
        add    esp, 8
        mov    byte ptr [ebp - 1], al
        mov    word ptr [esi + 36h], 0ceb8h
loc_2_0E25:
        push   dword ptr [ebp - 1]
        call   sub_2_1374
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_2_0EBE
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
loc_2_0E49:
        mov    ax, 1
        mov    cl, bl
        shl    ax, cl
        test   word ptr [esi + 36h], ax
        mov    word ptr [ebp - 6], ax
        je     SHORT loc_2_0EA2
        test   word ptr [esi + 32h], ax
        jne    SHORT loc_2_0EA2
        push   ebx
        push   dword ptr [ebp - 2]
        push   edi
        push   dword ptr [ebp - 1]
        call   sub_2_0EDD
        mov    word ptr [ebp - 0ah], ax
        add    esp, 10h
        test   ax, ax
        je     SHORT loc_2_0EA2
        bsf    ax, word ptr [ebp - 0ah]
        mov    word ptr [ebp - 8], ax
        movzx  eax, word ptr [ebp - 8]
        mov    cx, word ptr [ebp - 6]
        mov    byte ptr [eax + esi + 3dh], bl
        mov    ax, 1
        or     word ptr [esi + 32h], cx
        mov    cl, byte ptr [ebp - 8]
        shl    ax, cl
        or     word ptr [ebp - 4], ax
loc_2_0EA2:
        inc    bl
        cmp    bl, 10h
        jb     SHORT loc_2_0E49
        movzx  eax, word ptr [ebp - 4]
        push   eax
        call   sub_2_2A4C
        add    esp, 4
        pop    eax
        out    21h, al
        mov    al, ah
        out    0a1h, al
        popfd
loc_2_0EBE:
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_2A3C
        add    esp, 0ch
loc_2_0ED2:
        mov    ax, word ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0EDD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        xor    si, si
        push   edi
        push   5
        mov    bl, byte ptr [ebp + 10h]
        push   ebx
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0EF8:
        add    esp, 0ch
        mov    byte ptr [ebp - 2], al
        push   3
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F07:
        add    esp, 0ch
        mov    byte ptr [ebp - 1], al
        push   40h
        push   3
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F18:
        add    esp, 10h
        mov    al, byte ptr [ebp + 14h]
        shl    al, 4
        or     al, 8
        mov    byte ptr [ebp - 0ch], al
        push   dword ptr [ebp - 0ch]
        push   5
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F32:
        add    esp, 10h
        push   4
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F3E:
        add    esp, 0ch
        call   sub_2_1000
        cmp    byte ptr [ebp + 8], 82h
        mov    word ptr [ebp - 6], ax
        je     SHORT loc_2_0F8A
        push   20h
        push   16h
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F5B:
        add    esp, 10h
        call   sub_2_1000
        mov    word ptr [ebp - 4], ax
        push   4
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F70:
        add    esp, 0ch
        call   sub_2_1000
        mov    si, ax
        mov    ax, word ptr [ebp - 4]
        xor    si, word ptr [ebp - 4]
        xor    ax, word ptr [ebp - 6]
        and    si, ax
loc_2_0F8A:
        test   si, si
        jne    SHORT loc_2_0FD8
        push   0
        push   5
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F9A:
        add    esp, 10h
        call   sub_2_1000
        push   dword ptr [ebp - 0ch]
        push   5
        mov    word ptr [ebp - 4], ax
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0FB2:
        add    esp, 10h
        push   4
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0FBE:
        add    esp, 0ch
        call   sub_2_1000
        mov    si, ax
        mov    ax, word ptr [ebp - 4]
        xor    si, word ptr [ebp - 4]
        xor    ax, word ptr [ebp - 6]
        and    si, ax
loc_2_0FD8:
        push   dword ptr [ebp - 2]
        push   5
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0FE4:
        add    esp, 10h
        push   dword ptr [ebp - 1]
        push   3
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0FF3:
        mov    ax, si
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1000:
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
sub_2_101F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], 1
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 0
        push   ebx
        push   esi
        movzx  esi, byte ptr [ebp + 0ch]
        push   edi
        push   esi
        mov    edi, dword ptr [ebp + 8]
        movzx  eax, byte ptr [edi + 39h]
        shl    eax, 5
        push   edi
        db  0FFh, 090h, 00Ch, 000h, 000h, 000h
        add    esp, 8
        mov    dl, byte ptr [ebp + 10h]
        mov    cl, dl
        mov    bl, dl
        and    cl, 7
        and    bl, 10h
        mov    byte ptr [ebp - 1], cl
        je     SHORT loc_2_1088
        cmp    al, 1
        je     SHORT loc_2_1079
        test   byte ptr [edi + 8], 1
        je     SHORT loc_2_1079
        sub    al, 2
        cmp    al, 1
        sbb    al, al
        and    al, 3
        mov    byte ptr [ebp - 1], al
        jmp    SHORT loc_2_107D
loc_2_1079:
        mov    byte ptr [ebp - 1], 1
loc_2_107D:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 2], al
        jmp    SHORT loc_2_10C7
loc_2_1088:
        test   dl, 20h
        jne    SHORT loc_2_10B4
        cmp    byte ptr [ebp - 1], 1
        jne    SHORT loc_2_1097
        cmp    al, 1
        je     SHORT loc_2_10B4
loc_2_1097:
        cmp    byte ptr [ebp - 1], 3
        jne    SHORT loc_2_10A1
        cmp    al, 2
        je     SHORT loc_2_10B4
loc_2_10A1:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_2_10AB
        test   al, al
        je     SHORT loc_2_10B4
loc_2_10AB:
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_10C7
loc_2_10B4:
        mov    al, byte ptr [ebp + 14h]
        and    al, 70h
        shr    al, 4
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp + 14h]
        and    al, 7
        mov    byte ptr [ebp - 3], al
loc_2_10C7:
        cmp    dword ptr [ebp - 8], 1
        jne    SHORT loc_2_1145
        movzx  eax, byte ptr [ebp - 3]
        movzx  ecx, byte ptr [ebp - 2]
        push   eax
        push   ecx
        movzx  edx, byte ptr [ebp - 1]
        movzx  eax, byte ptr [edi + 39h]
        shl    eax, 5
        push   edx
        push   esi
        push   edi
        db  0FFh, 090h, 004h, 000h, 000h, 000h
        add    esp, 14h
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 1
        jne    SHORT loc_2_1131
        test   bl, bl
        je     SHORT loc_2_1131
        cmp    byte ptr [ebp - 1], 3
        jne    SHORT loc_2_1131
        movzx  eax, byte ptr [edi + 39h]
        shl    eax, 5
        push   esi
        push   edi
        db  0FFh, 090h, 00Ch, 000h, 000h, 000h
        add    esp, 8
        cmp    al, 1
        jne    SHORT loc_2_1131
        movzx  eax, byte ptr [edi + 39h]
        shl    eax, 5
        push   1
        push   1
        push   1
        push   esi
        push   edi
        db  0FFh, 090h, 004h, 000h, 000h, 000h
        add    esp, 14h
        mov    dword ptr [ebp - 8], eax
loc_2_1131:
        cmp    dword ptr [ebp - 8], 1
        jne    SHORT loc_2_1145
        test   bl, bl
        je     SHORT loc_2_1141
        or     byte ptr [edi + 0bh], 20h
        jmp    SHORT loc_2_1145
loc_2_1141:
        and    byte ptr [edi + 0bh], 0dfh
loc_2_1145:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_114F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        lea    ecx, [ebp - 2]
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        movzx  esi, byte ptr [ebp + 0ch]
        movzx  eax, byte ptr [edi + 39h]
        shl    eax, 5
        push   ecx
        push   ebx
        push   esi
        push   edi
        db  0FFh, 090h, 008h, 000h, 000h, 000h
        movzx  eax, byte ptr [edi + 39h]
        shl    eax, 5
        add    esp, 14h
        push   esi
        push   edi
        db  0FFh, 090h, 00Ch, 000h, 000h, 000h
        add    esp, 8
        mov    cl, byte ptr [ebp - 2]
        shl    cl, 4
        mov    edx, dword ptr [ebp + 14h]
        or     cl, byte ptr [ebp - 1]
        test   byte ptr [edi + 0bh], 20h
        mov    byte ptr [edx], cl
        je     SHORT loc_2_11A8
        or     byte ptr [ebx], 10h
        jmp    SHORT loc_2_11C6
loc_2_11A8:
        cmp    al, 1
        jne    SHORT loc_2_11B1
        cmp    byte ptr [ebx], 1
        jne    SHORT loc_2_11C3
loc_2_11B1:
        cmp    al, 2
        jne    SHORT loc_2_11BA
        cmp    byte ptr [ebx], 3
        jne    SHORT loc_2_11C3
loc_2_11BA:
        test   al, al
        jne    SHORT loc_2_11C6
        cmp    byte ptr [ebx], 0
        je     SHORT loc_2_11C6
loc_2_11C3:
        or     byte ptr [ebx], 20h
loc_2_11C6:
        cmp    al, 1
        je     SHORT loc_2_11D1
        cmp    al, 2
        jne    SHORT loc_2_11D1
        or     byte ptr [ebx], 40h
loc_2_11D1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_11D8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        movzx  esi, byte ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        push   2
        mov    al, byte ptr [esi + edi + 28h]
        mov    bl, 0
        push   eax
        push   dword ptr [edi + esi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_11F7:
        add    esp, 0ch
        mov    cl, byte ptr [ebp + 10h]
        cmp    cl, 1
        adc    bl, 0ffh
        and    bl, 10h
        test   cl, cl
        je     SHORT loc_2_1214
        cmp    byte ptr [ebp + 14h], cl
        jne    SHORT loc_2_1214
        or     bl, 1
        jmp    SHORT loc_2_1217
loc_2_1214:
        or     bl, byte ptr [ebp + 14h]
loc_2_1217:
        test   cl, cl
        je     SHORT loc_2_1225
        cmp    byte ptr [ebp + 18h], cl
        jne    SHORT loc_2_1225
        or     bl, 4
        jmp    SHORT loc_2_122D
loc_2_1225:
        mov    cl, byte ptr [ebp + 18h]
        shl    cl, 2
        or     bl, cl
loc_2_122D:
        mov    cl, al
        and    cl, 0a0h
        or     bl, cl
        test   bl, 10h
        jne    SHORT loc_2_123C
        and    bl, 5fh
loc_2_123C:
        or     bl, 40h
        cmp    bl, al
        je     SHORT loc_2_128A
        push   ebx
        mov    al, byte ptr [esi + edi + 28h]
        push   2
        push   eax
        push   dword ptr [edi + esi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1254:
        add    esp, 10h
        push   0
        push   493e0h
        call   sub_2_2A2C
        add    esp, 8
        test   bl, 10h
        je     SHORT loc_2_128A
        mov    al, bl
        and    al, 0a0h
        cmp    al, 0a0h
        je     SHORT loc_2_128A
        or     bl, 0a0h
        mov    al, byte ptr [esi + edi + 28h]
        push   ebx
        push   2
        push   eax
        push   dword ptr [edi + esi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1287:
        add    esp, 10h
loc_2_128A:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_2_1294:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esp + 0ch]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    dl, byte ptr [ecx + eax + 28h]
        push   2
        push   edx
        push   dword ptr [eax + ecx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_12B2:
        add    esp, 0ch
        mov    dl, al
        and    dl, 10h
        mov    ecx, dword ptr [ebp + 10h]
        shr    dl, 4
        mov    bl, al
        and    bl, 3
        mov    byte ptr [ecx], dl
        mov    ecx, dword ptr [ebp + 14h]
        cmp    bl, 1
        mov    byte ptr [ecx], bl
        jne    SHORT loc_2_12D5
        mov    byte ptr [ecx], dl
        jmp    SHORT loc_2_12DD
loc_2_12D5:
        cmp    bl, 3
        jne    SHORT loc_2_12DD
        mov    byte ptr [ecx], 0
loc_2_12DD:
        and    al, 0ch
        mov    esi, dword ptr [ebp + 18h]
        shr    al, 2
        mov    byte ptr [esi], al
        cmp    al, 1
        jne    SHORT loc_2_12EF
        mov    byte ptr [esi], dl
        jmp    SHORT loc_2_12F6
loc_2_12EF:
        cmp    al, 3
        jne    SHORT loc_2_12F6
        mov    byte ptr [ecx], 0
loc_2_12F6:
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_2_12FA:
        mov    al, 1
        ret
sub_2_12FD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   0
        push   dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1310:
        add    esp, 0ch
        mov    bl, al
        push   1
        push   dword ptr [ebp + 0ch]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1320:
        add    esp, 0ch
        push   ebx
        call   sub_2_1374
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_136E
        push   1
        push   1eh
        push   dword ptr [ebp + 0ch]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_133D:
        add    esp, 10h
        push   0
        push   1eh
        push   dword ptr [ebp + 0ch]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_134D:
        add    esp, 10h
        push   0
        push   dword ptr [ebp + 0ch]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_135B:
        add    esp, 0ch
        mov    bl, al
        push   1
        push   dword ptr [ebp + 0ch]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_136B:
        add    esp, 0ch
loc_2_136E:
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_1374:
        mov    al, byte ptr [esp + 4]
        cmp    al, 8ah
        je     SHORT loc_2_1390
        cmp    al, 87h
        je     SHORT loc_2_1390
        cmp    al, 8fh
        je     SHORT loc_2_1390
        cmp    al, 82h
        jb     SHORT loc_2_138C
        cmp    al, 84h
        jbe    SHORT loc_2_1390
loc_2_138C:
        xor    eax, eax
        jmp    SHORT loc_2_1395
loc_2_1390:
        mov    eax, 1
loc_2_1395:
        ret
sub_2_1396:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    word ptr [esi + 36h], 0ceb8h
        mov    edi, 1
        mov    al, byte ptr [esi + 3bh]
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 014h, 000h, 000h, 000h, 000h
        mov    cl, al
        and    cl, 1fh
        test   al, 80h
        je     SHORT loc_2_13D6
        movzx  eax, cl
        mov    al, byte ptr [eax + esi + 3dh]
        test   al, al
        je     SHORT loc_2_13D6
        mov    cl, al
loc_2_13D6:
        test   cl, cl
        jne    SHORT loc_2_13E3
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_13EF
loc_2_13E3:
        shl    cl, 4
        or     cl, 8
        db  088h, 00Dh, 005h, 000h, 000h, 000h
loc_2_13EF:
        xor    bl, bl
        cmp    byte ptr [esi + 38h], bl
        jbe    SHORT loc_2_1416
loc_2_13F6:
        movzx  ecx, bl
        db  068h, 000h, 000h, 000h, 000h
        inc    bl
        mov    al, byte ptr [esi + ecx + 28h]
        push   eax
        push   dword ptr [esi + ecx*4 + 18h]
        call   sub_2_146D
        add    esp, 0ch
        cmp    byte ptr [esi + 38h], bl
        ja     SHORT loc_2_13F6
loc_2_1416:
        push   esi
        call   0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_144E
        push   esi
        call   sub_2_045C
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_144E
        push   esi
        call   sub_2_06BA
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_144E
        push   esi
        call   sub_2_0983
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_144E
        mov    byte ptr [esi + 39h], 0
loc_2_144E:
        movzx  eax, byte ptr [esi + 39h]
        shl    eax, 5
        db  08Bh, 080h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_1467
        push   esi
        call   eax
        add    esp, 4
        mov    edi, eax
loc_2_1467:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_146D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 10h]
        cmp    byte ptr [esi], 0
        je     SHORT loc_2_14B6
loc_2_147D:
        mov    bl, byte ptr [esi]
        inc    esi
        mov    cl, bl
        mov    al, byte ptr [esi]
        inc    esi
        dec    bl
        mov    byte ptr [ebp - 1], al
        test   cl, cl
        je     SHORT loc_2_14B1
loc_2_148E:
        mov    eax, esi
        mov    cl, byte ptr [ebp - 1]
        inc    esi
        inc    byte ptr [ebp - 1]
        mov    al, byte ptr [eax]
        push   eax
        push   ecx
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_14A6:
        add    esp, 10h
        mov    cl, bl
        dec    bl
        test   cl, cl
        jne    SHORT loc_2_148E
loc_2_14B1:
        cmp    byte ptr [esi], 0
        jne    SHORT loc_2_147D
loc_2_14B6:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_14BC:
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    word ptr [eax + 10h], 501h
        mov    byte ptr [eax + 1ch], cl
        mov    byte ptr [eax + 18h], cl
        mov    byte ptr [eax + 19h], 1
        and    byte ptr [eax + 20h], 0feh
        mov    byte ptr [eax + 1dh], cl
        ret
loc_2_14DA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 10h]
        mov    word ptr [esi + 18h], bx
        mov    cl, 7
        mov    word ptr [esi + 14h], bx
        mov    al, byte ptr [edi + 38h]
        push   edi
        mov    byte ptr [esi + 10h], al
        imul   cl
        mov    byte ptr [esi + 11h], al
        call   sub_2_0B39
        add    esp, 4
        mov    ecx, dword ptr [edi + 0ch]
        movzx  eax, word ptr [edi + 30h]
        push   14h
        mov    dword ptr [ecx + 2], eax
        push   ecx
        push   dword ptr [esi]
        call   sub_2_23A7
        add    esp, 0ch
        mov    byte ptr [esi + 1dh], bl
        and    byte ptr [esi + 20h], 0feh
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_1527:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        movzx  ax, byte ptr [edx + 3bh]
        mov    word ptr [ecx], ax
        mov    byte ptr [ecx + 15h], 0
        mov    byte ptr [ecx + 1dh], 0
        and    byte ptr [ecx + 20h], 0feh
        ret
loc_2_1544:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    cx, word ptr [esi]
        test   cl, 20h
        jne    NEAR PTR loc_2_15E8
        test   cl, 40h
        je     NEAR PTR loc_2_15E8
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [edi + 3bh]
        xor    al, cl
        test   al, 1fh
        jne    SHORT loc_2_15E8
        mov    ax, cx
        mov    byte ptr [edi + 3bh], cl
        and    al, 1fh
        movzx  edx, al
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [edx + edi + 3dh]
        test   al, al
        je     SHORT loc_2_158A
        mov    byte ptr [ebp - 1], al
loc_2_158A:
        shl    byte ptr [ebp - 1], 4
        or     byte ptr [ebp - 1], 8
        test   cl, 80h
        jne    SHORT loc_2_159B
        mov    byte ptr [ebp - 1], 0
loc_2_159B:
        mov    byte ptr [ebp - 2], 0
        cmp    byte ptr [edi + 38h], 0
        je     SHORT loc_2_15E0
loc_2_15A5:
        movzx  ebx, byte ptr [ebp - 2]
        push   5
        mov    al, byte ptr [ebx + edi + 28h]
        push   eax
        push   dword ptr [edi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_15B9:
        add    esp, 0ch
        and    al, 7
        or     al, byte ptr [ebp - 1]
        mov    cl, byte ptr [ebx + edi + 28h]
        push   eax
        push   5
        push   ecx
        push   dword ptr [edi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_15D2:
        add    esp, 10h
        mov    cl, byte ptr [edi + 38h]
        inc    byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 2], cl
        jb     SHORT loc_2_15A5
loc_2_15E0:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_15EE
loc_2_15E8:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 6
loc_2_15EE:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_15F9
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_15FD
loc_2_15F9:
        or     byte ptr [esi + 20h], 1
loc_2_15FD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_1604:
        push   ebx
        mov    cl, 7
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    bl, 7
        mov    dl, byte ptr [esi + 15h]
        mov    al, dl
        sub    ah, ah
        div    cl
        mov    cl, al
        mov    al, dl
        sub    ah, ah
        div    bl
        cmp    byte ptr [esi + 11h], 0ffh
        mov    al, ah
        jne    SHORT loc_2_1670
        mov    edx, dword ptr [esp + 10h]
        cmp    byte ptr [edx + 38h], cl
        jbe    SHORT loc_2_1670
        cmp    al, 5
        jb     SHORT loc_2_1645
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edx, 18h
        mov    byte ptr [esi + 10h], 84h
        jmp    SHORT loc_2_1651
loc_2_1645:
        mov    byte ptr [esi + 10h], 83h
        mov    eax, dword ptr [edx + 14h]
        mov    edx, 20h
loc_2_1651:
        mov    di, 1
        push   edx
        shl    di, cl
        mov    word ptr [esi + 18h], di
        push   eax
        push   dword ptr [esi]
        call   sub_2_23A7
        add    esp, 0ch
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_1676
loc_2_1670:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 11h
loc_2_1676:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1681
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_1685
loc_2_1681:
        or     byte ptr [esi + 20h], 1
loc_2_1685:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_1689:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    dl, 7
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    bl, 7
        mov    cl, byte ptr [edi + 11h]
        mov    al, cl
        sub    ah, ah
        mov    esi, dword ptr [ebp + 8]
        div    dl
        mov    dl, al
        mov    al, cl
        sub    ah, ah
        div    bl
        mov    byte ptr [ebp - 1], ah
        cmp    byte ptr [esi + 38h], dl
        ja     SHORT loc_2_16C0
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 11h
        jmp    NEAR PTR loc_2_1850
loc_2_16C0:
        mov    al, dl
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edi + 15h], 0
        or     al, 10h
        mov    byte ptr [edi + 10h], al
        mov    al, 1
        shl    al, cl
        mov    byte ptr [ebp - 5], al
        cmp    cl, 5
        jb     NEAR PTR loc_2_1781
        mov    byte ptr [edi + 15h], 1
        add    al, al
        push   6
        mov    byte ptr [ebp - 5], al
        movzx  eax, dl
        mov    byte ptr [edi + 14h], 0
        mov    bl, byte ptr [esi + eax + 28h]
        mov    esi, dword ptr [esi + eax*4 + 18h]
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_16FF:
        add    esp, 0ch
        test   byte ptr [ebp - 5], al
        je     SHORT loc_2_170B
        mov    byte ptr [edi + 15h], 3
loc_2_170B:
        mov    al, byte ptr [ebp - 1]
        mov    cl, byte ptr [ebp - 8]
        sub    al, 5
        push   7
        push   ebx
        cmp    al, 1
        sbb    byte ptr [ebp - 8], cl
        push   esi
        and    byte ptr [ebp - 8], 0f1h
        add    byte ptr [ebp - 8], 10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1729:
        add    esp, 0ch
        test   byte ptr [ebp - 8], al
        je     SHORT loc_2_1735
        or     byte ptr [edi + 15h], 4
loc_2_1735:
        sub    byte ptr [ebp - 1], 5
        mov    al, byte ptr [ebp - 1]
        lea    eax, [eax*4 + 8]
        push   eax
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_174B:
        movzx  ecx, ax
        add    esp, 0ch
        mov    al, byte ptr [ebp - 1]
        shl    al, 2
        mov    dword ptr [edi], ecx
        add    al, 0ah
        push   eax
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1763:
        movzx  ecx, ax
        add    esp, 0ch
        mov    eax, ecx
        mov    dword ptr [edi + 18h], ecx
        test   eax, eax
        je     NEAR PTR loc_2_184A
        sub    eax, dword ptr [edi]
        inc    eax
        mov    dword ptr [edi + 18h], eax
        jmp    NEAR PTR loc_2_184A
loc_2_1781:
        movzx  eax, dl
        mov    ebx, eax
        push   6
        mov    dword ptr [ebp - 0ch], eax
        mov    cl, byte ptr [esi + ebx + 28h]
        push   ecx
        push   dword ptr [esi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1799:
        add    esp, 0ch
        test   byte ptr [ebp - 5], al
        je     SHORT loc_2_17A5
        mov    byte ptr [edi + 15h], 2
loc_2_17A5:
        mov    al, byte ptr [ebp - 1]
        mov    cl, byte ptr [esi + ebx + 28h]
        lea    eax, [eax*8 + 10h]
        push   eax
        push   ecx
        push   dword ptr [esi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_17BE:
        add    esp, 0ch
        xor    ecx, ecx
        mov    cx, ax
        and    ecx, 0fffh
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [edi], ecx
        test   ah, 80h
        je     SHORT loc_2_17DA
        or     byte ptr [edi + 15h], 4
loc_2_17DA:
        mov    al, byte ptr [ebp - 1]
        mov    cl, byte ptr [esi + ebx + 28h]
        shl    al, 3
        add    al, 12h
        push   eax
        push   ecx
        push   dword ptr [esi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_17F1:
        add    esp, 0ch
        lea    ecx, [edi + 18h]
        xor    edx, edx
        mov    dx, ax
        and    edx, 0fffh
        mov    dword ptr [ecx], edx
        je     SHORT loc_2_180C
        sub    edx, dword ptr [ebp - 8]
        inc    edx
        mov    dword ptr [ecx], edx
loc_2_180C:
        shr    ax, 0eh
        movzx  edx, ax
        movzx  eax, byte ptr [esi + 39h]
        shl    eax, 5
        db  08Ah, 09Ah, 000h, 000h, 000h, 000h
        shl    dword ptr [ecx], 0ch
        mov    byte ptr [edi + 14h], bl
        mov    ebx, dword ptr [ebp - 8]
        shl    ebx, 0ch
        mov    dword ptr [edi], ebx
        db  08Bh, 088h, 014h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_2_184A
        movzx  eax, byte ptr [ebp - 1]
        push   eax
        push   dword ptr [ebp - 0ch]
        push   esi
        call   ecx
        add    esp, 0ch
        or     eax, ebx
        mov    dword ptr [edi], eax
loc_2_184A:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0
loc_2_1850:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_185B
        and    byte ptr [edi + 20h], 0feh
        jmp    SHORT loc_2_185F
loc_2_185B:
        or     byte ptr [edi + 20h], 1
loc_2_185F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_1866:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    dl, 7
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [edi + 11h]
        mov    al, cl
        sub    ah, ah
        div    dl
        mov    byte ptr [ebp - 2], al
        mov    al, cl
        sub    ah, ah
        mov    ecx, dword ptr [ebp + 8]
        div    dl
        mov    byte ptr [ebp - 1], ah
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ecx + 38h], al
        ja     SHORT loc_2_189D
        mov    byte ptr [edi + 1dh], 11h
        jmp    NEAR PTR loc_2_1AC5
loc_2_189D:
        mov    al, byte ptr [edi + 10h]
        test   al, 10h
        je     NEAR PTR loc_2_1AC1
        and    al, 0fh
        cmp    al, byte ptr [ebp - 2]
        jne    NEAR PTR loc_2_1AC1
        mov    byte ptr [edi + 1dh], 0
        cmp    byte ptr [ebp - 1], 5
        jb     NEAR PTR loc_2_19A2
        mov    al, byte ptr [edi + 15h]
        test   al, 1
        jne    SHORT loc_2_18D1
        mov    byte ptr [edi + 1dh], 0dh
        jmp    NEAR PTR loc_2_1A95
loc_2_18D1:
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   dword ptr [edi + 18h]
        push   dword ptr [edi]
        call   sub_2_22A1
        add    esp, 10h
        test   al, al
        jne    NEAR PTR loc_2_1A92
        mov    ecx, dword ptr [ebp + 8]
        push   0
        movzx  eax, byte ptr [ebp - 2]
        push   dword ptr [ebp - 1]
        mov    bl, byte ptr [eax + ecx + 28h]
        mov    esi, dword ptr [ecx + eax*4 + 18h]
        push   ebx
        push   esi
        call   sub_2_2362
        mov    ax, word ptr [edi]
        add    esp, 10h
        mov    cl, byte ptr [ebp - 1]
        push   eax
        lea    edx, [ecx*4 - 0ch]
        push   edx
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1920:
        add    esp, 10h
        mov    eax, dword ptr [edi + 18h]
        test   eax, eax
        je     SHORT loc_2_1931
        add    ax, word ptr [edi]
        dec    ax
        jmp    SHORT loc_2_1935
loc_2_1931:
        mov    ax, 0ffffh
loc_2_1935:
        push   eax
        mov    al, byte ptr [ebp - 1]
        shl    al, 2
        sub    al, 0ah
        push   eax
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1946:
        add    esp, 10h
        test   byte ptr [edi + 15h], 4
        jne    SHORT loc_2_1955
        mov    byte ptr [ebp - 5], 0
        jmp    SHORT loc_2_196C
loc_2_1955:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 8]
        and    eax, 4000000h
        cmp    eax, 1
        sbb    al, al
        and    al, 8
        add    al, 3
        mov    byte ptr [ebp - 5], al
loc_2_196C:
        mov    byte ptr [ebp - 9], 0f0h
        cmp    byte ptr [ebp - 1], 5
        jbe    SHORT loc_2_197E
        shl    byte ptr [ebp - 5], 4
        mov    byte ptr [ebp - 9], 0fh
loc_2_197E:
        push   7
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1987:
        add    esp, 0ch
        and    al, byte ptr [ebp - 9]
        or     al, byte ptr [ebp - 5]
        push   eax
        push   7
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_199A:
        add    esp, 10h
        jmp    NEAR PTR loc_2_1A95
loc_2_19A2:
        test   byte ptr [edi + 15h], 1
        je     SHORT loc_2_19B1
        mov    byte ptr [edi + 1dh], 0dh
        jmp    NEAR PTR loc_2_1A95
loc_2_19B1:
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [edi + 15h]
        push   dword ptr [ecx + 14h]
        push   edx
        push   dword ptr [edi + 18h]
        push   dword ptr [edi]
        call   sub_2_22A1
        add    esp, 10h
        test   al, al
        jne    NEAR PTR loc_2_1A95
        lea    eax, [ebp - 5]
        mov    cl, byte ptr [edi + 14h]
        push   eax
        push   ecx
        call   sub_2_22FB
        add    esp, 8
        test   al, al
        jne    NEAR PTR loc_2_1A95
        mov    ecx, dword ptr [ebp + 8]
        push   0
        push   dword ptr [ebp - 1]
        movzx  eax, byte ptr [ebp - 2]
        mov    dword ptr [ebp - 0ch], eax
        mov    bl, byte ptr [eax + ecx + 28h]
        mov    esi, dword ptr [ecx + eax*4 + 18h]
        push   ebx
        push   esi
        call   sub_2_2362
        add    esp, 10h
        mov    eax, dword ptr [edi]
        shr    eax, 0ch
        and    ax, 0fffh
        test   byte ptr [edi + 15h], 4
        je     SHORT loc_2_1A1B
        or     ah, 80h
loc_2_1A1B:
        push   eax
        mov    al, byte ptr [ebp - 1]
        lea    eax, [eax*8 + 10h]
        push   eax
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1A2E:
        mov    ax, 0fffh
        add    esp, 10h
        cmp    dword ptr [edi + 18h], 0
        je     SHORT loc_2_1A48
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [edi + 18h]
        dec    eax
        shr    eax, 0ch
        and    ax, 0fffh
loc_2_1A48:
        movzx  cx, byte ptr [ebp - 5]
        shl    cx, 0eh
        or     ax, cx
        push   eax
        mov    al, byte ptr [ebp - 1]
        shl    al, 3
        add    al, 12h
        push   eax
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1A65:
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [ecx + 39h]
        shl    eax, 5
        db  08Bh, 088h, 010h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_2_1A95
        push   dword ptr [edi]
        movzx  edx, byte ptr [ebp - 1]
        push   edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp + 8]
        call   ecx
        add    esp, 10h
        test   al, al
        je     SHORT loc_2_1A95
loc_2_1A92:
        mov    byte ptr [edi + 1dh], al
loc_2_1A95:
        cmp    byte ptr [edi + 1dh], 0
        jne    SHORT loc_2_1AD1
        xor    ecx, ecx
        mov    edx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [ebp - 2]
        mov    cl, byte ptr [edi + 15h]
        mov    bl, byte ptr [eax + edx + 28h]
        and    ecx, 2
        push   ecx
        push   dword ptr [ebp - 1]
        push   ebx
        push   dword ptr [edx + eax*4 + 18h]
        call   sub_2_2362
        add    esp, 10h
        jmp    SHORT loc_2_1AC5
loc_2_1AC1:
        mov    byte ptr [edi + 1dh], 0bh
loc_2_1AC5:
        cmp    byte ptr [edi + 1dh], 0
        jne    SHORT loc_2_1AD1
        and    byte ptr [edi + 20h], 0feh
        jmp    SHORT loc_2_1AD5
loc_2_1AD1:
        or     byte ptr [edi + 20h], 1
loc_2_1AD5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_1ADC:
        sub    esp, 4
        mov    cl, 7
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        mov    dl, byte ptr [edi + 11h]
        mov    al, dl
        sub    ah, ah
        mov    esi, dword ptr [esp + 18h]
        div    cl
        mov    bl, al
        mov    al, dl
        sub    ah, ah
        div    cl
        mov    byte ptr [esp + 13h], ah
        cmp    byte ptr [esi + 38h], bl
        jbe    NEAR PTR loc_2_1BCB
        cmp    ah, 5
        jae    NEAR PTR loc_2_1BCB
        cmp    byte ptr [edi + 10h], 0
        je     SHORT loc_2_1B25
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 8
        jmp    NEAR PTR loc_2_1BD1
loc_2_1B25:
        movzx  ecx, bl
        mov    al, byte ptr [esp + 13h]
        mov    dl, byte ptr [esi + ecx + 28h]
        shl    al, 3
        add    al, 14h
        push   eax
        push   edx
        movzx  eax, bl
        push   dword ptr [esi + eax*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1B43:
        movzx  edx, bl
        mov    bp, ax
        mov    cl, byte ptr [esp + 1fh]
        add    esp, 0ch
        lea    eax, [ecx*8 + 10h]
        mov    cl, byte ptr [esi + edx + 28h]
        push   eax
        movzx  eax, bl
        push   ecx
        push   dword ptr [esi + eax*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1B69:
        and    ax, 0fffh
        add    esp, 0ch
        mov    byte ptr [edi + 14h], 0
        add    ax, bp
        and    ax, 3fffh
        test   bp, 8000h
        mov    word ptr [edi], ax
        je     SHORT loc_2_1B89
        mov    byte ptr [edi + 14h], 4
loc_2_1B89:
        test   bp, 4000h
        je     SHORT loc_2_1B94
        or     byte ptr [edi + 14h], 1
loc_2_1B94:
        movzx  eax, bl
        movzx  ecx, bl
        push   6
        mov    al, byte ptr [esi + eax + 28h]
        push   eax
        push   dword ptr [esi + ecx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1BAA:
        movzx  ebx, al
        mov    cl, byte ptr [esp + 1fh]
        add    esp, 0ch
        mov    edx, 1
        shl    edx, cl
        test   edx, ebx
        je     SHORT loc_2_1BC3
        or     byte ptr [edi + 14h], 2
loc_2_1BC3:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_1BD1
loc_2_1BCB:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 11h
loc_2_1BD1:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1BDC
        and    byte ptr [edi + 20h], 0feh
        jmp    SHORT loc_2_1BE0
loc_2_1BDC:
        or     byte ptr [edi + 20h], 1
loc_2_1BE0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_2_1BE8:
        sub    esp, 8
        mov    cl, 7
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 20h]
        mov    dl, byte ptr [edi + 11h]
        mov    al, dl
        sub    ah, ah
        mov    esi, dword ptr [esp + 1ch]
        div    cl
        mov    bl, al
        mov    al, dl
        sub    ah, ah
        div    cl
        mov    byte ptr [esp + 13h], ah
        cmp    byte ptr [esi + 38h], bl
        jbe    NEAR PTR loc_2_1CC8
        cmp    ah, 5
        jae    NEAR PTR loc_2_1CC8
        cmp    byte ptr [edi + 10h], 0
        je     SHORT loc_2_1C31
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 8
        jmp    NEAR PTR loc_2_1CCE
loc_2_1C31:
        mov    bp, word ptr [edi]
        test   bp, 0c000h
        je     SHORT loc_2_1C46
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 7
        jmp    NEAR PTR loc_2_1CCE
loc_2_1C46:
        movzx  eax, byte ptr [edi + 14h]
        test   eax, 0fffffff8h
        je     SHORT loc_2_1C59
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 2
        jmp    SHORT loc_2_1CCE
loc_2_1C59:
        movzx  ecx, bl
        mov    al, byte ptr [esp + 13h]
        mov    dl, byte ptr [esi + ecx + 28h]
        lea    eax, [eax*8 + 10h]
        push   eax
        push   edx
        movzx  eax, bl
        push   dword ptr [esi + eax*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1C79:
        and    ax, 0fffh
        add    esp, 0ch
        sub    bp, ax
        and    bp, 3fffh
        test   byte ptr [edi + 14h], 4
        je     SHORT loc_2_1C93
        or     bp, 8000h
loc_2_1C93:
        test   byte ptr [edi + 14h], 1
        je     SHORT loc_2_1C9E
        or     bp, 4000h
loc_2_1C9E:
        mov    al, byte ptr [esp + 13h]
        push   ebp
        movzx  ecx, bl
        shl    al, 3
        mov    dl, byte ptr [esi + ecx + 28h]
        add    al, 14h
        push   eax
        push   edx
        movzx  eax, bl
        push   dword ptr [esi + eax*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1CBD:
        add    esp, 10h
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_1CCE
loc_2_1CC8:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 11h
loc_2_1CCE:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1CD9
        and    byte ptr [edi + 20h], 0feh
        jmp    SHORT loc_2_1CDD
loc_2_1CD9:
        or     byte ptr [edi + 20h], 1
loc_2_1CDD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_2_1CE5:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        mov    al, byte ptr [esi + 10h]
        cmp    byte ptr [ebx + 38h], al
        ja     SHORT loc_2_1D00
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0bh
        jmp    SHORT loc_2_1D40
loc_2_1D00:
        mov    byte ptr [esi + 11h], 0f0h
        mov    ecx, dword ptr [ebx + 10h]
        mov    edi, dword ptr [esi]
        mov    byte ptr [esi + 15h], 0f0h
        mov    byte ptr [esi + 14h], 0
        movzx  eax, word ptr [ebx + 30h]
        push   12h
        mov    dword ptr [ecx + 2], eax
        push   ecx
        push   edi
        call   sub_2_23A7
        movzx  eax, byte ptr [ebx + 39h]
        shl    eax, 5
        add    esp, 0ch
        db  083h, 0B8h, 018h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_1D3A
        mov    word ptr [edi + 10h], 1
loc_2_1D3A:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
loc_2_1D40:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1D4B
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_1D4F
loc_2_1D4B:
        or     byte ptr [esi + 20h], 1
loc_2_1D4F:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_1D53:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 10h]
        cmp    byte ptr [edi + 38h], al
        ja     SHORT loc_2_1D72
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0bh
        jmp    SHORT loc_2_1DC8
loc_2_1D72:
        push   esi
        push   edi
        call   sub_2_2082
        add    esp, 8
        mov    bl, byte ptr [esi + 10h]
        movzx  ax, byte ptr [edi + 3ch]
        movzx  ecx, bl
        mov    word ptr [esi + 8], ax
        push   5
        mov    al, byte ptr [ecx + edi + 28h]
        push   eax
        push   dword ptr [edi + ecx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1D9B:
        shl    al, 4
        add    esp, 0ch
        lea    ecx, [ebp - 1]
        lea    edx, [ebp - 2]
        push   ecx
        mov    byte ptr [esi + 11h], al
        push   edx
        push   ebx
        push   edi
        call   sub_2_114F
        add    esp, 10h
        mov    cl, byte ptr [ebp - 2]
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 19h], cl
        mov    byte ptr [esi + 18h], al
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
loc_2_1DC8:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1DD3
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_1DD7
loc_2_1DD3:
        or     byte ptr [esi + 20h], 1
loc_2_1DD7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_1DDE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ax, word ptr [edi]
        mov    cl, byte ptr [edi + 10h]
        shr    ax, 8
        and    al, 3
        cmp    byte ptr [esi + 38h], cl
        mov    byte ptr [ebp - 1], al
        ja     SHORT loc_2_1E0C
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0bh
        jmp    NEAR PTR loc_2_1FF1
loc_2_1E0C:
        mov    al, byte ptr [edi + 19h]
        and    al, 30h
        cmp    al, 30h
        jne    SHORT loc_2_1E20
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 2
        jmp    NEAR PTR loc_2_1FF1
loc_2_1E20:
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_2_1FEB
        cmp    byte ptr [ebp - 1], 3
        jne    SHORT loc_2_1E3B
        cmp    word ptr [edi + 8], 1
        jae    NEAR PTR loc_2_1FEB
loc_2_1E3B:
        mov    ax, word ptr [edi]
        mov    cl, byte ptr [edi + 18h]
        and    al, 1fh
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [edi + 19h]
        and    al, 7
        mov    byte ptr [ebp - 9], al
        mov    al, cl
        and    al, 70h
        shr    al, 4
        mov    byte ptr [ebp - 5], al
        mov    al, cl
        and    al, 7
        cmp    byte ptr [ebp - 1], 1
        mov    byte ptr [ebp - 3], al
        je     SHORT loc_2_1E86
        test   byte ptr [edi], 80h
        je     SHORT loc_2_1E86
        cmp    byte ptr [ebp - 2], 0fh
        ja     SHORT loc_2_1E7B
        movzx  eax, byte ptr [ebp - 2]
        cmp    byte ptr [eax + esi + 3dh], 0
        jne    SHORT loc_2_1E86
loc_2_1E7B:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 6
        jmp    NEAR PTR loc_2_1FF1
loc_2_1E86:
        mov    edx, dword ptr [esi + 0ch]
        movzx  ax, byte ptr [ebp - 9]
        mov    bx, word ptr [edx + 0ah]
        cmp    bx, ax
        jbe    NEAR PTR loc_2_1FE3
        movzx  eax, byte ptr [ebp - 9]
        test   byte ptr [edx + eax*2 + 0dh], 80h
        je     NEAR PTR loc_2_1FE3
        movzx  ax, byte ptr [ebp - 5]
        cmp    bx, ax
        jbe    NEAR PTR loc_2_1FDB
        movzx  eax, byte ptr [ebp - 5]
        test   byte ptr [edx + eax*2 + 0dh], 40h
        je     NEAR PTR loc_2_1FDB
        movzx  ax, byte ptr [ebp - 3]
        cmp    bx, ax
        jbe    NEAR PTR loc_2_1FDB
        movzx  eax, byte ptr [ebp - 3]
        test   byte ptr [edx + eax*2 + 0dh], 20h
        je     NEAR PTR loc_2_1FDB
        push   ecx
        mov    al, byte ptr [edi + 19h]
        push   eax
        mov    cl, byte ptr [edi + 10h]
        push   ecx
        push   esi
        call   sub_2_101F
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_2_1FD3
        mov    al, byte ptr [edi + 15h]
        push   5
        movzx  ebx, byte ptr [edi + 10h]
        not    al
        and    byte ptr [ebx + esi + 2ch], al
        mov    cl, byte ptr [ebx + esi + 28h]
        push   ecx
        push   dword ptr [esi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1F1B:
        add    esp, 0ch
        and    al, 0f0h
        mov    cl, byte ptr [edi + 11h]
        mov    dl, byte ptr [ebx + esi + 28h]
        shr    cl, 4
        or     al, cl
        push   eax
        push   5
        push   edx
        push   dword ptr [esi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1F39:
        add    esp, 10h
        mov    cl, byte ptr [ebx + esi + 28h]
        push   3
        push   ecx
        push   dword ptr [esi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1F4C:
        add    esp, 0ch
        and    al, 0d0h
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_2_1F5D
        cmp    byte ptr [ebp - 1], 3
        jne    SHORT loc_2_1F6C
loc_2_1F5D:
        or     al, 20h
        test   byte ptr [edi], 80h
        je     SHORT loc_2_1F6C
        movzx  ecx, byte ptr [ebp - 2]
        or     al, byte ptr [ecx + esi + 3dh]
loc_2_1F6C:
        push   eax
        push   3
        mov    al, byte ptr [ebx + esi + 28h]
        push   eax
        push   dword ptr [esi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1F7D:
        add    esp, 10h
        cmp    byte ptr [ebp - 1], 3
        jne    SHORT loc_2_1FA4
        mov    cl, byte ptr [edi + 8]
        push   esi
        mov    byte ptr [esi + 3ch], cl
        movzx  eax, byte ptr [esi + 39h]
        movzx  ecx, cl
        or     byte ptr [esi + 0bh], 8
        lea    ebx, [ecx + eax*4]
        db  0FFh, 014h, 0DDh, 018h, 000h, 000h, 000h
        jmp    SHORT loc_2_1FC8
loc_2_1FA4:
        mov    eax, dword ptr [esi + 8]
        test   eax, 8000000h
        je     SHORT loc_2_1FCB
        and    eax, 0f7ffffffh
        push   esi
        mov    dword ptr [esi + 8], eax
        movzx  eax, byte ptr [esi + 39h]
        shl    eax, 5
        mov    byte ptr [esi + 3ch], 0
        db  0FFh, 090h, 01Ch, 000h, 000h, 000h
loc_2_1FC8:
        add    esp, 4
loc_2_1FCB:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_2_1FF1
loc_2_1FD3:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0eh
        jmp    SHORT loc_2_1FF1
loc_2_1FDB:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0fh
        jmp    SHORT loc_2_1FF1
loc_2_1FE3:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0eh
        jmp    SHORT loc_2_1FF1
loc_2_1FEB:
        lea    eax, [edi + 1dh]
        mov    byte ptr [eax], 0dh
loc_2_1FF1:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1FFC
        and    byte ptr [edi + 20h], 0feh
        jmp    SHORT loc_2_2000
loc_2_1FFC:
        or     byte ptr [edi + 20h], 1
loc_2_2000:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_2007:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    al, byte ptr [esi + 10h]
        cmp    byte ptr [edi + 38h], al
        ja     SHORT loc_2_2021
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0bh
        jmp    SHORT loc_2_2070
loc_2_2021:
        push   esi
        push   edi
        call   sub_2_2082
        movzx  eax, byte ptr [esi + 10h]
        add    esp, 8
        mov    cl, byte ptr [eax + edi + 28h]
        push   1
        push   ecx
        push   dword ptr [edi + eax*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_203F:
        add    esp, 0ch
        lea    ecx, [esi + 11h]
        mov    dl, al
        shl    dl, 4
        not    dl
        and    dl, 30h
        test   al, 10h
        mov    byte ptr [ecx], dl
        je     SHORT loc_2_205A
        or     dl, 1
        mov    byte ptr [ecx], dl
loc_2_205A:
        test   al, 20h
        je     SHORT loc_2_2061
        or     byte ptr [ecx], 40h
loc_2_2061:
        and    al, 0ch
        cmp    al, 0ch
        jne    SHORT loc_2_206A
        or     byte ptr [ecx], 80h
loc_2_206A:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
loc_2_2070:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_207B
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_207F
loc_2_207B:
        or     byte ptr [esi + 20h], 1
loc_2_207F:
        pop    edi
        pop    esi
        ret
sub_2_2082:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 10h]
        push   edi
        mov    edi, dword ptr [esp + 10h]
        push   ebp
        movzx  esi, byte ptr [ebx + 10h]
        push   4
        lea    ebp, [edi + esi*4 + 18h]
        mov    al, byte ptr [esi + edi + 28h]
        push   eax
        push   dword ptr [ebp]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_20A5:
        add    esp, 0ch
        and    al, 0fh
        mov    byte ptr [ebx + 14h], 0
        push   3
        or     byte ptr [esi + edi + 2ch], al
        mov    al, byte ptr [esi + edi + 2ch]
        mov    cl, byte ptr [esi + edi + 28h]
        shl    al, 4
        push   ecx
        mov    esi, 1
        mov    byte ptr [ebx + 15h], al
        push   dword ptr [ebp]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_20D0:
        add    esp, 0ch
        test   al, 20h
        je     SHORT loc_2_20E8
        mov    ecx, dword ptr [edi + 8]
        and    ecx, 8000000h
        cmp    ecx, 1
        sbb    esi, esi
        add    esi, 3
loc_2_20E8:
        shl    si, 8
        and    al, 0fh
        mov    word ptr [ebx], si
        je     SHORT loc_2_2113
        xor    cl, cl
loc_2_20F5:
        movzx  edx, cl
        cmp    byte ptr [edx + edi + 3dh], al
        je     SHORT loc_2_2107
        inc    cl
        cmp    cl, 10h
        jb     SHORT loc_2_20F5
        jmp    SHORT loc_2_2113
loc_2_2107:
        movzx  ax, cl
        or     ax, si
        or     al, 0c0h
        mov    word ptr [ebx], ax
loc_2_2113:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_2118:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        mov    ebx, dword ptr [esp + 14h]
        push   ebp
        mov    al, byte ptr [esi + 10h]
        cmp    byte ptr [ebx + 38h], al
        ja     SHORT loc_2_213A
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0bh
        jmp    NEAR PTR loc_2_221D
loc_2_213A:
        movzx  edi, byte ptr [esi + 10h]
        push   1
        mov    ebp, dword ptr [ebx + edi*4 + 18h]
        mov    al, byte ptr [edi + ebx + 28h]
        push   eax
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_214F:
        add    esp, 0ch
        and    al, 0ch
        cmp    al, 0ch
        je     SHORT loc_2_2163
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 14h
        jmp    NEAR PTR loc_2_221D
loc_2_2163:
        push   20h
        mov    al, byte ptr [edi + ebx + 28h]
        push   6
        push   eax
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2172:
        add    esp, 10h
        mov    cl, byte ptr [edi + ebx + 28h]
        push   3
        push   ecx
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2182:
        add    esp, 0ch
        and    al, 0cfh
        mov    byte ptr [esp + 13h], al
        movzx  eax, byte ptr [ebx + 39h]
        shl    eax, 5
        db  08Bh, 080h, 01Ch, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_21A2
        push   ebx
        call   eax
        add    esp, 4
loc_2_21A2:
        push   dword ptr [esp + 13h]
        push   3
        and    byte ptr [ebx + 0bh], 0f7h
        mov    byte ptr [ebx + 3ch], 0
        mov    al, byte ptr [edi + ebx + 28h]
        push   eax
        push   dword ptr [ebx + edi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_21BE:
        add    esp, 10h
        mov    cl, byte ptr [esi + 10h]
        push   0
        push   10h
        push   ecx
        push   ebx
        call   sub_2_101F
        add    esp, 10h
        mov    ebp, dword ptr [ebx + edi*4 + 18h]
        mov    al, byte ptr [edi + ebx + 28h]
        and    byte ptr [esp + 13h], 0bfh
        push   dword ptr [esp + 13h]
        push   3
        push   eax
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_21EC:
        add    esp, 10h
        push   0
        push   7d0h
        call   sub_2_2A2C
        or     byte ptr [esp + 1bh], 40h
        add    esp, 8
        mov    cl, byte ptr [edi + ebx + 28h]
        push   dword ptr [esp + 13h]
        push   3
        push   ecx
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2214:
        add    esp, 10h
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
loc_2_221D:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_2228
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_222C
loc_2_2228:
        or     byte ptr [esi + 20h], 1
loc_2_222C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_2_2234:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_2_2249
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 15h
        jmp    SHORT loc_2_228E
loc_2_2249:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_2A54
        add    esp, 4
        mov    ebx, eax
        mov    edi, dword ptr [esi]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 14h], 100h
        push   ebx
        push   eax
        push   edi
        call   sub_2_23A7
        mov    ax, word ptr [edi]
        add    esp, 0ch
        cmp    bx, ax
        jae    SHORT loc_2_2280
        mov    byte ptr [edi + ebx + 4], 0
        jmp    SHORT loc_2_2288
loc_2_2280:
        movzx  eax, ax
        mov    byte ptr [edi + eax + 3], 0
loc_2_2288:
        lea    eax, [esi + 1dh]
        mov    byte ptr [eax], 0
loc_2_228E:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_2299
        and    byte ptr [esi + 20h], 0feh
        jmp    SHORT loc_2_229D
loc_2_2299:
        or     byte ptr [esi + 20h], 1
loc_2_229D:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_22A1:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 14h]
        lea    esi, [eax + ecx - 1]
        cmp    dword ptr [edx + 2], eax
        ja     SHORT loc_2_22BD
        cmp    dword ptr [edx + 6], eax
        jae    SHORT loc_2_22C7
loc_2_22BD:
        test   byte ptr [ebp + 10h], 2
        jne    SHORT loc_2_22F6
        test   eax, eax
        jne    SHORT loc_2_22F6
loc_2_22C7:
        cmp    dword ptr [edx + 0ah], ecx
        ja     SHORT loc_2_22D1
        cmp    dword ptr [edx + 0eh], ecx
        jae    SHORT loc_2_22DB
loc_2_22D1:
        test   byte ptr [ebp + 10h], 2
        jne    SHORT loc_2_22F2
        test   ecx, ecx
        jne    SHORT loc_2_22F2
loc_2_22DB:
        cmp    dword ptr [edx + 6], esi
        jb     SHORT loc_2_22EE
        xor    esi, eax
        test   esi, 0ff000000h
        jne    SHORT loc_2_22EE
        xor    al, al
        jmp    SHORT loc_2_22F8
loc_2_22EE:
        mov    al, 0ah
        jmp    SHORT loc_2_22F8
loc_2_22F2:
        mov    al, 0ah
        jmp    SHORT loc_2_22F8
loc_2_22F6:
        mov    al, 3
loc_2_22F8:
        pop    esi
        pop    ebp
        ret
sub_2_22FB:
        push   ebp
        mov    al, 17h
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    cl, byte ptr [ebp + 8]
        test   cl, 80h
        jne    SHORT loc_2_235D
        mov    dl, cl
        and    dl, 7
        and    cl, 78h
        mov    byte ptr [ebp - 2], dl
        mov    byte ptr [ebp - 1], cl
        jne    SHORT loc_2_2328
        movzx  ecx, dl
        db  08Ah, 089h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 1], cl
loc_2_2328:
        xor    dl, dl
loc_2_232A:
        movzx  ecx, dl
        db  08Ah, 089h, 000h, 000h, 000h, 000h
        mov    bl, cl
        and    bl, 7
        mov    byte ptr [ebp - 3], bl
        mov    bl, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 3], bl
        ja     SHORT loc_2_2356
        jne    SHORT loc_2_234D
        and    cl, 78h
        cmp    cl, byte ptr [ebp - 1]
        jae    SHORT loc_2_2356
loc_2_234D:
        inc    dl
        cmp    dl, 4
        jb     SHORT loc_2_232A
        jmp    SHORT loc_2_235D
loc_2_2356:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], dl
        xor    al, al
loc_2_235D:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2362:
        push   ebp
        mov    cl, byte ptr [esp + 10h]
        mov    ebp, esp
        push   ebx
        mov    bl, 1
        shl    bl, cl
        cmp    cl, 5
        jb     SHORT loc_2_2375
        add    bl, bl
loc_2_2375:
        push   6
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2382:
        add    esp, 0ch
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_2_238F
        or     al, bl
        jmp    SHORT loc_2_2393
loc_2_238F:
        not    bl
        and    al, bl
loc_2_2393:
        push   eax
        push   6
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_23A1:
        add    esp, 10h
        pop    ebx
        pop    ebp
        ret
sub_2_23A7:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        movzx  eax, word ptr [edi]
        mov    word ptr [edi + 2], dx
        cmp    edx, eax
        jle    SHORT loc_2_23BE
        mov    edx, eax
loc_2_23BE:
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
sub_2_23D6:
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
sub_2_23F3:
        cmp    dword ptr [esp + 8], 2
        push   esi
        push   edi
        jne    SHORT loc_2_2461
        mov    edi, 13h
        push   1
        push   4dh
        call   sub_2_2A6C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_2_243A
        push   esi
        push   dword ptr [esp + 10h]
        call   sub_2_26B9
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_243A
        push   3
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 18h]
        call   sub_2_2A64
        add    esp, 10h
        mov    edi, eax
loc_2_243A:
        cmp    edi, 13h
        jne    SHORT loc_2_2466
        test   esi, esi
        je     SHORT loc_2_2466
        lea    ecx, [esi + 4dh]
        mov    eax, esi
        push   0
        push   ecx
        push   eax
        call   sub_2_2A44
        add    esp, 0ch
        push   0
        push   esi
        call   sub_2_2A5C
        add    esp, 8
        jmp    SHORT loc_2_2466
loc_2_2461:
        mov    edi, 1
loc_2_2466:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    8
loc_2_246D:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        test   eax, eax
        push   ebx
        mov    dword ptr [ebp - 8], 0
        push   esi
        push   edi
        je     SHORT loc_2_24A4
        cmp    eax, 1
        je     SHORT loc_2_24B4
        cmp    eax, 4
        je     SHORT loc_2_2508
        cmp    eax, 8
        je     NEAR PTR loc_2_252A
        mov    dword ptr [ebp - 8], 1
        jmp    NEAR PTR loc_2_2644
loc_2_24A4:
        push   dword ptr [ebp + 14h]
        call   sub_2_264E
        add    esp, 4
        jmp    NEAR PTR loc_2_2644
loc_2_24B4:
        mov    esi, dword ptr [ebp + 14h]
        push   esi
        call   sub_2_26EA
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_24D0
        mov    dword ptr [ebp - 8], 13h
        jmp    NEAR PTR loc_2_2644
loc_2_24D0:
        cmp    dword ptr [ebp + 0ch], 1
        jne    NEAR PTR loc_2_2644
        push   esi
        call   sub_2_29EC
        add    esp, 4
        test   eax, eax
        je     SHORT loc_2_24F3
        mov    dword ptr [ebp - 8], 13h
        jmp    NEAR PTR loc_2_2644
loc_2_24F3:
        push   0
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_2_2A74
        add    esp, 0ch
        jmp    NEAR PTR loc_2_2644
loc_2_2508:
        mov    esi, dword ptr [ebp + 14h]
        push   0
        lea    eax, [esi + 4dh]
        push   eax
        push   esi
        call   sub_2_2A3C
        add    esp, 0ch
        push   0
        push   esi
        call   sub_2_2A5C
        add    esp, 8
        jmp    NEAR PTR loc_2_2644
loc_2_252A:
        and    byte ptr [ebp + 0fh], 7fh
        cmp    dword ptr [ebp + 0ch], 7
        mov    esi, dword ptr [ebp + 14h]
        je     SHORT loc_2_253D
        cmp    dword ptr [ebp + 0ch], 8
        jne    SHORT loc_2_258A
loc_2_253D:
        cmp    byte ptr [esi + 39h], 1
        jne    SHORT loc_2_254C
        push   esi
        call   sub_2_008A
        add    esp, 4
loc_2_254C:
        mov    bl, byte ptr [esi + 28h]
        mov    edi, dword ptr [esi + 18h]
        push   10h
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_255B:
        add    esp, 0ch
        test   ax, ax
        je     SHORT loc_2_2576
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   edi
        call   sub_2_29B6
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_258A
loc_2_2576:
        push   esi
        call   sub_2_1396
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_258A
        mov    dword ptr [ebp - 8], 13h
loc_2_258A:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_2644
        test   byte ptr [esi + 0bh], 2
        je     NEAR PTR loc_2_2644
        cmp    dword ptr [ebp + 0ch], 4
        je     SHORT loc_2_2600
        cmp    dword ptr [ebp + 0ch], 5
        je     SHORT loc_2_2600
        cmp    dword ptr [ebp + 0ch], 6
        je     SHORT loc_2_25BA
        cmp    dword ptr [ebp + 0ch], 7
        jne    NEAR PTR loc_2_2644
loc_2_25BA:
        mov    byte ptr [ebp - 1], 0
        mov    bl, byte ptr [esi + 38h]
        test   bl, bl
        je     SHORT loc_2_2644
loc_2_25C5:
        movzx  edi, byte ptr [ebp - 1]
        push   3
        mov    al, byte ptr [edi + esi + 28h]
        push   eax
        push   dword ptr [esi + edi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_25D9:
        add    esp, 0ch
        test   al, 80h
        je     SHORT loc_2_25F6
        and    al, 7fh
        mov    cl, byte ptr [edi + esi + 28h]
        push   eax
        push   3
        push   ecx
        push   dword ptr [esi + edi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_25F3:
        add    esp, 10h
loc_2_25F6:
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], bl
        jb     SHORT loc_2_25C5
        jmp    SHORT loc_2_2644
loc_2_2600:
        mov    byte ptr [ebp - 1], 0
        mov    bl, byte ptr [esi + 38h]
        test   bl, bl
        je     SHORT loc_2_2644
loc_2_260B:
        movzx  edi, byte ptr [ebp - 1]
        push   3
        mov    al, byte ptr [edi + esi + 28h]
        push   eax
        push   dword ptr [esi + edi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_261F:
        add    esp, 0ch
        test   al, 20h
        je     SHORT loc_2_263C
        or     al, 80h
        mov    cl, byte ptr [edi + esi + 28h]
        push   eax
        push   3
        push   ecx
        push   dword ptr [esi + edi*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2639:
        add    esp, 10h
loc_2_263C:
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], bl
        jb     SHORT loc_2_260B
loc_2_2644:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_264E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_0B39
        mov    si, word ptr [edi + 30h]
        add    esp, 4
        test   si, si
        je     SHORT loc_2_26B3
        push   1
        lea    ecx, [ebp - 4]
        push   dword ptr [edi]
        push   ecx
        call   sub_2_2A8C
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_26B3
        xor    edi, edi
loc_2_2681:
        push   edi
        lea    ecx, [ebp - 8]
        push   edi
        push   4
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_2A84
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_269F
        mov    eax, dword ptr [ebp - 8]
        and    word ptr [eax + 4], si
loc_2_269F:
        push   edi
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_2A7C
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_2681
loc_2_26B3:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_26B9:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 10h]
        lea    eax, [esi + 4dh]
        push   eax
        push   esi
        call   sub_2_2A44
        add    esp, 0ch
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_2_26E8
        mov    dword ptr [esi + 4], 5353534dh
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi], eax
        mov    eax, 1
loc_2_26E8:
        pop    esi
        ret
sub_2_26EA:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        push   2
        mov    edi, dword ptr [ebp + 8]
        lea    ecx, [ebp - 14h]
        push   dword ptr [edi]
        push   ecx
        call   sub_2_2A8C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_28CB
        push   esi
        lea    ecx, [ebp - 0ch]
        push   esi
        push   4
        push   dword ptr [ebp - 14h]
        push   ecx
        call   sub_2_2A84
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_2734
        cmp    eax, 0fh
        jne    NEAR PTR loc_2_28CB
        test   eax, eax
        jne    SHORT loc_2_273F
loc_2_2734:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [edi + 3bh], al
        jmp    SHORT loc_2_2743
loc_2_273F:
        mov    byte ptr [edi + 3bh], 0
loc_2_2743:
        push   0
        lea    ecx, [ebp - 0ch]
        push   0
        push   2
        push   dword ptr [ebp - 14h]
        push   ecx
        call   sub_2_2A84
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_2_28CB
        mov    byte ptr [edi + 38h], 0
loc_2_2764:
        mov    eax, dword ptr [ebp - 0ch]
        xor    bl, bl
        mov    ax, word ptr [eax + 4]
        cmp    byte ptr [edi + 3ah], bl
        mov    word ptr [ebp - 4], ax
        jne    SHORT loc_2_279C
        movzx  eax, ax
        push   0
        push   eax
        call   sub_2_12FD
        add    esp, 8
        mov    byte ptr [ebp - 1], al
        push   dword ptr [ebp - 1]
        call   sub_2_1374
        add    esp, 4
        test   eax, eax
        je     SHORT loc_2_279C
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [edi + 3ah], al
loc_2_279C:
        mov    ecx, dword ptr [ebp - 0ch]
        movzx  eax, byte ptr [ecx + 0ah]
        movzx  ecx, word ptr [ebp - 4]
        shl    eax, 8
        push   eax
        push   ecx
        push   edi
        call   sub_2_28D4
        add    esp, 0ch
loc_2_27B5:
        movzx  eax, word ptr [ebp - 4]
        push   ebx
        push   eax
        call   sub_2_12FD
        add    esp, 8
        mov    byte ptr [ebp - 1], al
        push   dword ptr [ebp - 1]
        call   sub_2_1374
        add    esp, 4
        test   eax, eax
        je     SHORT loc_2_2807
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [edi + 3ah], al
        jne    SHORT loc_2_2807
        movzx  eax, word ptr [ebp - 4]
        push   ebx
        push   eax
        push   edi
        call   sub_2_291B
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_2807
        movzx  ecx, byte ptr [edi + 38h]
        movzx  eax, word ptr [ebp - 4]
        mov    dword ptr [edi + ecx*4 + 18h], eax
        movzx  edx, byte ptr [edi + 38h]
        mov    byte ptr [edx + edi + 28h], bl
        inc    byte ptr [edi + 38h]
loc_2_2807:
        add    bl, 40h
        cmp    byte ptr [edi + 38h], 4
        jae    SHORT loc_2_2837
        test   bl, bl
        jne    SHORT loc_2_27B5
        cmp    byte ptr [edi + 38h], 4
        jae    SHORT loc_2_2837
        push   0
        lea    ecx, [ebp - 0ch]
        push   0
        push   2
        push   dword ptr [ebp - 0ch]
        push   ecx
        call   sub_2_2A84
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_2_2764
loc_2_2837:
        cmp    byte ptr [edi + 38h], 0
        je     NEAR PTR loc_2_28CB
        push   1
        lea    eax, [ebp - 10h]
        push   eax
        lea    ecx, [ebp - 1ch]
        mov    dword ptr [ebp - 10h], 4
        push   ecx
        push   3
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   0
        push   dword ptr [edi]
        call   sub_2_2A34
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_2_287B
        cmp    dword ptr [ebp - 10h], 4
        jne    SHORT loc_2_287B
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        or     dword ptr [edi + 8], eax
loc_2_287B:
        db  0CDh, 020h, 000h, 000h, 026h, 000h
        mov    word ptr [ebp - 6], ax
        cmp    word ptr [ebp - 6], 0
        je     SHORT loc_2_28A7
        lea    eax, [ebp - 18h]
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 026h, 000h
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_28A7
        test   byte ptr [ebp - 18h], 0c0h
        je     SHORT loc_2_28A7
        or     byte ptr [edi + 0bh], 2
loc_2_28A7:
        push   edi
        call   sub_2_0B39
        add    esp, 4
        push   edi
        call   sub_2_1396
        add    esp, 4
        test   eax, eax
        je     SHORT loc_2_28CB
        push   edi
        mov    esi, 1
        call   sub_2_0C54
        add    esp, 4
loc_2_28CB:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_28D4:
        push   esi
        xor    eax, eax
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 8]
        test   edx, 10000000h
        jne    SHORT loc_2_2914
        cmp    byte ptr [ecx + 3ah], 84h
        jne    SHORT loc_2_2919
        cmp    byte ptr [ecx + 38h], 0
        jbe    SHORT loc_2_2901
        mov    esi, dword ptr [ecx + 18h]
        sub    esi, dword ptr [esp + 0ch]
        cmp    esi, 0ffff8000h
        je     SHORT loc_2_290B
loc_2_2901:
        cmp    dword ptr [esp + 10h], 8000h
        jne    SHORT loc_2_2919
loc_2_290B:
        or     edx, 10000000h
        mov    dword ptr [ecx + 8], edx
loc_2_2914:
        mov    eax, 1
loc_2_2919:
        pop    esi
        ret
sub_2_291B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   10h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2931:
        xor    edi, edi
        add    esp, 0ch
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], al
        movzx  ebx, byte ptr [esi + 38h]
        dec    ebx
        js     SHORT loc_2_29AD
loc_2_2943:
        test   edi, edi
        jne    SHORT loc_2_29AD
        push   10h
        mov    al, byte ptr [esi + ebx + 28h]
        push   eax
        push   dword ptr [esi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2957:
        add    esp, 0ch
        cmp    al, byte ptr [ebp - 1]
        jne    SHORT loc_2_29AA
        mov    al, byte ptr [ebp - 1]
        not    al
        mov    byte ptr [ebp - 8], al
        push   dword ptr [ebp - 8]
        push   10h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2977:
        add    esp, 10h
        mov    cl, byte ptr [esi + ebx + 28h]
        push   10h
        push   ecx
        push   dword ptr [esi + ebx*4 + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_298A:
        add    esp, 0ch
        cmp    al, byte ptr [ebp - 8]
        jne    SHORT loc_2_2997
        mov    edi, 1
loc_2_2997:
        push   dword ptr [ebp - 1]
        push   10h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_29A7:
        add    esp, 10h
loc_2_29AA:
        dec    ebx
        jns    SHORT loc_2_2943
loc_2_29AD:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_29B6:
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [esp + 14h]
        cmp    byte ptr [esi], 0
        je     SHORT loc_2_29E7
loc_2_29C3:
        mov    al, byte ptr [esi]
        push   eax
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_29D3:
        add    esp, 0ch
        test   al, al
        jne    SHORT loc_2_29E2
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_2_29C3
        jmp    SHORT loc_2_29E7
loc_2_29E2:
        mov    edi, 1
loc_2_29E7:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_2_29EC:
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
sub_2_2A24:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_2_2A2C:
        db  0CDh, 020h, 015h, 080h, 033h, 000h
        int3
        int3
sub_2_2A34:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_2_2A3C:
        db  0CDh, 020h, 095h, 081h, 001h, 000h
        int3
        int3
sub_2_2A44:
        db  0CDh, 020h, 094h, 081h, 001h, 000h
        int3
        int3
sub_2_2A4C:
        db  0CDh, 020h, 019h, 080h, 003h, 000h
        int3
        int3
sub_2_2A54:
        db  0CDh, 020h, 07Ch, 081h, 001h, 000h
        int3
        int3
sub_2_2A5C:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_2_2A64:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        int3
        int3
sub_2_2A6C:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_2_2A74:
        db  0CDh, 020h, 049h, 080h, 033h, 000h
        int3
        int3
sub_2_2A7C:
        db  0CDh, 020h, 01Bh, 080h, 033h, 000h
        int3
        int3
sub_2_2A84:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_2_2A8C:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
_PTEXT ends
        end
