; ircomm_lcode_01_02.asm -- IRCOMM object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_525F:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_5287
loc_1_526D:
        test   byte ptr [eax + 105h], 20h
        jne    SHORT loc_1_527F
        cmp    dword ptr [eax + 0e0h], -1
        jne    SHORT loc_1_5297
loc_1_527F:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_526D
loc_1_5287:
        xor    esi, esi
        db  087h, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        jmp    SHORT loc_1_52B8
loc_1_5297:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_52B8
        mov    edx, esi
        mov    eax, 64h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_1_52B8:
        ret
loc_1_52B9:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    esi, edx
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_530F
loc_1_52CD:
        test   byte ptr [eax + 105h], 20h
        jne    SHORT loc_1_5307
        cmp    dword ptr [eax + 0e0h], -1
        je     SHORT loc_1_5307
        cmp    dword ptr [eax + 4ch], 0
        je     SHORT loc_1_5307
        test   byte ptr [eax + 0cdh], 1
        jne    SHORT loc_1_5307
        xor    byte ptr [eax + 0cdh], 80h
        js     SHORT loc_1_5307
        push   eax
        push   esi
        mov    esi, eax
        mov    eax, 1
        call   sub_1_4BED
        pop    esi
        pop    eax
loc_1_5307:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_52CD
loc_1_530F:
        mov    edx, esi
        mov    eax, 64h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        ret
sub_1_5328:
        shl    ebx, 2
        db  0FFh, 093h, 000h, 000h, 000h, 000h
        sti
        jb     SHORT loc_1_5337
        mov    eax, dword ptr [esi + 34h]
loc_1_5337:
        ret
loc_1_5338:
        or     byte ptr [esi + 108h], 8
loc_1_533F:
        clc
        ret
loc_1_5341:
        and    byte ptr [esi + 108h], 0f7h
        clc
        ret
loc_1_534A:
        mov    ax, word ptr [esi + 11eh]
        push   eax
        mov    al, byte ptr [esi + 10eh]
        or     al, 2
        call   sub_1_5036
        pop    eax
        cmp    ax, word ptr [esi + 11eh]
        je     SHORT loc_1_5376
        pushad
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popad
loc_1_5376:
        clc
        ret
loc_1_5378:
        mov    ax, word ptr [esi + 11eh]
        push   eax
        mov    al, byte ptr [esi + 10eh]
        and    al, 0fdh
        call   sub_1_5036
        pop    eax
        cmp    ax, word ptr [esi + 11eh]
        je     SHORT loc_1_53A4
        pushad
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popad
loc_1_53A4:
        clc
        ret
loc_1_53A6:
        mov    ax, word ptr [esi + 11eh]
        push   eax
        mov    al, byte ptr [esi + 10eh]
        or     al, 1
        call   sub_1_5036
        pop    eax
        cmp    ax, word ptr [esi + 11eh]
        je     SHORT loc_1_53D2
        pushad
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popad
loc_1_53D2:
        clc
        ret
loc_1_53D4:
        mov    ax, word ptr [esi + 11eh]
        push   eax
        mov    al, byte ptr [esi + 10eh]
        and    al, 0feh
        call   sub_1_5036
        pop    eax
        cmp    ax, word ptr [esi + 11eh]
        je     SHORT loc_1_5400
        pushad
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popad
loc_1_5400:
        clc
        ret
loc_1_5402:
        push   esi
        call   sub_1_3430
        add    esp, 4
        clc
        ret
loc_1_540D:
        mov    ecx, 0ff40h
        jmp    SHORT loc_1_5419
loc_1_5414:
        mov    ecx, 0ffffbf00h
loc_1_5419:
        and    byte ptr [esi + 108h], ch
        or     byte ptr [esi + 108h], cl
        mov    al, byte ptr [esi + 10fh]
        and    al, ch
        or     al, cl
        mov    byte ptr [esi + 10fh], al
        or     word ptr [esi + 11eh], 40h
        pushad
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popad
        clc
        ret
loc_1_544C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, 0ch
        lea    edi, [esi + 4ch]
        or     bl, bl
        jne    SHORT loc_1_5469
        add    edi, ecx
        mov    edx, dword ptr [esi + 58h]
loc_1_5469:
        shr    ecx, 2
        xor    eax, eax
        cld
        rep stosd
        or     bl, bl
        je     SHORT loc_1_5488
        mov    ecx, eax
        call   sub_1_4D78
        sti
        push   esi
        call   sub_1_336C
        add    esp, 4
        jmp    SHORT loc_1_54A7
loc_1_5488:
        and    byte ptr [esi + 104h], 0bfh
        sti
        or     edx, edx
        je     SHORT loc_1_54A7
        cmp    dword ptr [esi + 0e4h], 0
        je     SHORT loc_1_54A7
        mov    eax, 2
        call   sub_1_4BED
loc_1_54A7:
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_54B2:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_1_4D2C
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        dec    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_1_54CE:
        enter  4, 0
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 120h], 1
        jne    SHORT loc_1_5503
        cld
        push   esi
        lea    edi, [esi + 94h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 0ch
        rep movsd
        pop    esi
        mov    dword ptr [esi + 94h], 30h
        jmp    NEAR PTR loc_1_5620
loc_1_5503:
        mov    bx, word ptr [esi + 11eh]
        mov    word ptr [ebp - 4], bx
        mov    ebx, dword ptr [ebp + 0ch]
        test   byte ptr [ebp + 10h], 1
        je     SHORT loc_1_551E
        call   sub_1_4EDE
        jecxz  loc_1_552B
loc_1_551E:
        test   byte ptr [ebp + 10h], 70h
        je     SHORT loc_1_5530
        call   sub_1_4F1F
        jae    SHORT loc_1_5530
loc_1_552B:
        jmp    NEAR PTR loc_1_5622
loc_1_5530:
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_1_4DC8
        lea    ebx, [esi + 94h]
        test   byte ptr [ebp + 10h], 70h
        je     SHORT loc_1_555D
        call   sub_1_4F1F
        mov    byte ptr [esi + 10fh], al
        mov    eax, edi
        and    byte ptr [esi + 9ch], ah
        mov    byte ptr [esi + 107h], al
loc_1_555D:
        mov    al, 0bh
        test   byte ptr [ebp + 10h], 2
        je     SHORT loc_1_55A3
        mov    al, byte ptr [esi + 9ch]
        and    al, 82h
        rol    al, 1
        shr    al, 1
        adc    al, 0
        mov    ah, al
        xor    al, 0bh
        mov    ecx, eax
        mov    al, byte ptr [esi + 9dh]
        rol    al, 1
        rol    al, 1
        rol    al, 1
        and    al, 3
        not    ah
        and    al, ah
        mov    byte ptr [esi + 109h], al
        mov    al, byte ptr [esi + 9ch]
        shl    al, 1
        and    al, 30h
        mov    byte ptr [esi + 10ah], al
        mov    eax, ecx
loc_1_55A3:
        call   sub_1_5036
        test   byte ptr [esi + 9dh], 80h
        jne    SHORT loc_1_55E7
        mov    al, byte ptr [esi + 10dh]
        mov    ah, byte ptr [esi + 10ah]
        and    al, ah
        cmp    al, ah
        je     SHORT loc_1_55E7
        cmp    ah, 30h
        jne    SHORT loc_1_55E0
        test   byte ptr [esi + 104h], 1
        jne    SHORT loc_1_55E0
        cmp    al, 10h
        jne    SHORT loc_1_55E0
        and    ah, 0dfh
        mov    byte ptr [esi + 10ah], ah
        jmp    SHORT loc_1_55E7
loc_1_55E0:
        or     byte ptr [esi + 108h], 0a0h
loc_1_55E7:
        mov    eax, dword ptr [esi + 40h]
        sub    eax, dword ptr [esi + 0a4h]
        mov    dword ptr [esi + 0e8h], eax
        cmp    dword ptr [esi + 110h], 5
        je     SHORT loc_1_5606
        or     byte ptr [esi + 108h], 8
loc_1_5606:
        mov    bx, word ptr [esi + 11eh]
        cmp    bx, word ptr [ebp - 4]
        je     SHORT loc_1_5620
        pushad
        push   4
        push   esi
        call   sub_1_35B5
        add    esp, 8
        popad
loc_1_5620:
        xor    eax, eax
loc_1_5622:
        mov    dword ptr [esi + 0ch], eax
        or     eax, eax
        jne    SHORT loc_1_562C
        dec    eax
        jmp    SHORT loc_1_562E
loc_1_562C:
        xor    eax, eax
loc_1_562E:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_1_5633:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        lea    edi, [edx + 4ch]
        mov    ecx, 6
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        cli
        cld
        rep stosd
        mov    edi, edx
        and    byte ptr [edi + 105h], 7fh
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        jne    SHORT loc_1_5683
        test   byte ptr [edi + 105h], 8
        je     NEAR PTR loc_1_56F0
loc_1_566A:
        and    byte ptr [edi + 105h], 0f7h
        push   0
        push   dword ptr [edi + 3ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_1_56F0
loc_1_5683:
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        jne    SHORT loc_1_56C9
        test   byte ptr [edi + 105h], 8
        je     SHORT loc_1_56AA
        push   1
        push   dword ptr [ebp + 10h]
        push   dword ptr [edi + 3ch]
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_566A
        jne    SHORT loc_1_56E9
loc_1_56AA:
        push   1
        push   dword ptr [ebp + 10h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_57A2
        or     byte ptr [edi + 105h], 8
        jmp    SHORT loc_1_56E9
loc_1_56C9:
        test   byte ptr [edi + 105h], 8
        je     SHORT loc_1_56E9
        push   eax
        push   0
        push   dword ptr [edi + 3ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    eax
        and    byte ptr [edi + 105h], 0f7h
loc_1_56E9:
        or     byte ptr [edi + 105h], 80h
loc_1_56F0:
        mov    dword ptr [edi + 3ch], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [edi + 40h], eax
        sub    eax, dword ptr [edi + 0a4h]
        mov    dword ptr [edi + 0e8h], eax
        and    byte ptr [edi + 105h], 0efh
        mov    eax, dword ptr [ebp + 18h]
        or     eax, eax
        jne    SHORT loc_1_5739
        test   byte ptr [edi + 105h], 4
        je     NEAR PTR loc_1_57A2
loc_1_5720:
        and    byte ptr [edi + 105h], 0fbh
        push   0
        push   dword ptr [edi + 44h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_1_57A2
loc_1_5739:
        mov    eax, dword ptr [ebp + 14h]
        or     eax, eax
        jne    SHORT loc_1_577B
        test   byte ptr [edi + 105h], 4
        je     SHORT loc_1_5760
        push   1
        push   dword ptr [ebp + 18h]
        push   dword ptr [edi + 44h]
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_5720
        jne    SHORT loc_1_579B
loc_1_5760:
        push   1
        push   dword ptr [ebp + 18h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_57A2
        or     byte ptr [edi + 105h], 4
        jmp    SHORT loc_1_579B
loc_1_577B:
        test   byte ptr [edi + 105h], 4
        je     SHORT loc_1_579B
        push   eax
        push   0
        push   dword ptr [edi + 44h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    eax
        and    byte ptr [edi + 105h], 0fbh
loc_1_579B:
        or     byte ptr [edi + 105h], 10h
loc_1_57A2:
        mov    dword ptr [edi + 44h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [edi + 48h], eax
        or     al, 1
        sti
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_57B3:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 0ffffffe2h
        cmp    dword ptr [esi + 110h], 5
        je     SHORT loc_1_57D1
        or     byte ptr [esi + 451h], 8
        jmp    SHORT loc_1_5807
loc_1_57D1:
        test   byte ptr [esi + 121h], 7
        je     SHORT loc_1_57E3
        or     byte ptr [esi + 121h], 8
        jmp    SHORT loc_1_5807
loc_1_57E3:
        and    byte ptr [esi + 121h], 0f7h
        test   byte ptr [esi + 104h], 40h
        jne    SHORT loc_1_5807
        mov    ah, byte ptr [ebp + 0ch]
        cli
        or     byte ptr [esi + 104h], 40h
        mov    byte ptr [esi + 106h], ah
        sti
        xor    eax, eax
loc_1_5807:
        mov    dword ptr [esi + 0ch], eax
        or     eax, eax
        mov    eax, 0ffffffffh
        je     SHORT loc_1_5815
        xor    eax, eax
loc_1_5815:
        pop    esi
        leave
        ret
loc_1_5818:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_1_4D2C
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        xchg   dword ptr [esi + 34h], eax
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [edx], eax
        or     al, 1
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_583D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    ebx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    dword ptr [esi + 0ch], 0ffffffech
        cmp    bl, 0c8h
        jne    SHORT loc_1_586E
        mov    dword ptr [esi + 0ch], eax
        cmp    dword ptr [esi + 4ch], eax
        je     SHORT loc_1_58D1
        mov    eax, dword ptr [esi + 3ch]
        add    eax, dword ptr [esi + 50h]
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], cl
        jmp    SHORT loc_1_58CF
loc_1_586E:
        cmp    bl, 15h
        jne    SHORT loc_1_587C
        and    byte ptr [esi + 105h], 0dfh
        jmp    SHORT loc_1_58CF
loc_1_587C:
        cmp    bl, 14h
        jne    SHORT loc_1_588A
        or     byte ptr [esi + 105h], 40h
        jmp    SHORT loc_1_58CF
loc_1_588A:
        cmp    bl, 10h
        jne    SHORT loc_1_5898
        or     byte ptr [esi + 105h], 20h
        jmp    SHORT loc_1_58CF
loc_1_5898:
        cmp    bl, 13h
        jne    SHORT loc_1_58A8
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 100h], eax
        jmp    SHORT loc_1_58CF
loc_1_58A8:
        cmp    bl, 6eh
        jne    SHORT loc_1_58BB
        mov    dword ptr [esi + 0ch], eax
        push   esi
        call   sub_1_325F
        add    esp, 4
        jmp    SHORT loc_1_58C8
loc_1_58BB:
        cmp    bl, 0dh
        ja     SHORT loc_1_58D1
        mov    dword ptr [esi + 0ch], eax
        call   sub_1_5328
loc_1_58C8:
        mov    ecx, dword ptr [ebp + 14h]
        jecxz  loc_1_58CF
        mov    dword ptr [ecx], eax
loc_1_58CF:
        or     al, 1
loc_1_58D1:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_58D6:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0ch], 0
        mov    edx, dword ptr [edx + 0f0h]
        mov    eax, dword ptr [ebp + 0ch]
        not    eax
        cli
        mov    ecx, dword ptr [edx]
        and    eax, ecx
        mov    dword ptr [edx], eax
        sti
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [edx], ecx
        or     al, 1
        leave
        ret
loc_1_58FF:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 110h], 5
        je     SHORT loc_1_5947
        or     byte ptr [esi + 451h], 8
        or     dword ptr [esi + 34h], 100h
        xor    ecx, ecx
        cmp    dword ptr [esi + 0d8h], 0
        je     SHORT loc_1_598D
        mov    eax, dword ptr [esi + 58h]
        cmp    eax, dword ptr [esi + 0e4h]
        jae    SHORT loc_1_5945
        mov    eax, 2
        call   sub_1_4BED
loc_1_5945:
        jmp    SHORT loc_1_598D
loc_1_5947:
        and    byte ptr [esi + 121h], 0f7h
        test   byte ptr [esi + 105h], 10h
        jne    SHORT loc_1_599C
        xor    eax, eax
        cmp    dword ptr [esi + 58h], eax
        je     SHORT loc_1_5968
        int3
        or     dword ptr [esi + 34h], 100h
        jmp    SHORT loc_1_59A1
loc_1_5968:
        mov    dword ptr [esi + 44h], edi
        mov    dword ptr [esi + 48h], ecx
        mov    dword ptr [esi + 58h], ecx
        mov    dword ptr [esi + 60h], eax
        mov    dword ptr [esi + 5ch], eax
        and    dword ptr [esi + 34h], 0fffffeffh
        cmp    ecx, dword ptr [esi + 0e4h]
        jb     SHORT loc_1_598D
        and    byte ptr [esi + 0cdh], 0fdh
loc_1_598D:
        push   ecx
        push   3
        push   esi
        call   sub_1_35B5
        add    esp, 8
        pop    eax
        jmp    SHORT loc_1_59A1
loc_1_599C:
        call   sub_1_4CA8
loc_1_59A1:
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], eax
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_59B1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        test   byte ptr [esi + 105h], 40h
        jne    SHORT loc_1_59D3
        or     eax, dword ptr [esi + 34h]
        je     SHORT loc_1_59D3
        mov    ecx, eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_5A7C
loc_1_59D3:
        mov    ecx, dword ptr [ebp + 10h]
        or     ecx, ecx
        je     NEAR PTR loc_1_5A7A
        test   byte ptr [esi + 105h], 80h
        je     NEAR PTR loc_1_5A7A
        mov    eax, dword ptr [esi + 4ch]
        cmp    ecx, eax
        jbe    SHORT loc_1_59F4
        mov    ecx, eax
loc_1_59F4:
        mov    eax, dword ptr [esi + 4ch]
        or     eax, eax
        je     SHORT loc_1_5A5C
        cmp    ecx, eax
        jbe    SHORT loc_1_5A01
        mov    ecx, eax
loc_1_5A01:
        push   ecx
        mov    edx, dword ptr [esi + 40h]
        mov    eax, dword ptr [esi + 50h]
        sub    edx, eax
        cmp    edx, ecx
        jbe    SHORT loc_1_5A10
        mov    edx, ecx
loc_1_5A10:
        xchg   edx, ecx
        sub    edx, ecx
        mov    edi, dword ptr [ebp + 0ch]
        push   esi
        mov    ebx, dword ptr [esi + 3ch]
        mov    esi, ebx
        add    esi, eax
        cld
        rep movsb
        mov    ecx, edx
        jecxz  loc_1_5A2A
        mov    esi, ebx
        rep movsb
loc_1_5A2A:
        sub    esi, ebx
        mov    ebx, esi
        pop    esi
        pop    eax
        cli
        mov    dword ptr [esi + 50h], ebx
        sub    dword ptr [esi + 4ch], eax
        sti
        mov    ecx, dword ptr [esi + 4ch]
        cmp    ecx, dword ptr [esi + 0e0h]
        jae    SHORT loc_1_5A4A
        and    byte ptr [esi + 0cdh], 0feh
loc_1_5A4A:
        push   eax
        call   sub_1_4D78
        push   esi
        call   sub_1_343B
        add    esp, 4
        pop    eax
        jmp    SHORT loc_1_5A7A
loc_1_5A5C:
        test   byte ptr [esi + 9ch], 1
        jne    SHORT loc_1_5A7A
        test   byte ptr [esi + 104h], 20h
        je     SHORT loc_1_5A7A
        mov    cl, byte ptr [esi + 0b0h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    byte ptr [edi], cl
        inc    eax
loc_1_5A7A:
        xor    ecx, ecx
loc_1_5A7C:
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], eax
        mov    dword ptr [esi + 0ch], ecx
        mov    eax, 0ffffffffh
        jecxz  loc_1_5A8D
        xor    eax, eax
loc_1_5A8D:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_1_5A92:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 105h], 80h
        jne    SHORT loc_1_5AA8
loc_1_5AA4:
        xor    eax, eax
        jmp    SHORT loc_1_5B25
loc_1_5AA8:
        mov    ecx, dword ptr [ebp + 10h]
        or     ecx, ecx
        je     SHORT loc_1_5AA4
        mov    eax, dword ptr [esi + 40h]
        sub    eax, dword ptr [esi + 4ch]
        jle    SHORT loc_1_5AA4
        test   byte ptr [esi + 105h], 1
        je     SHORT loc_1_5AC4
        mov    eax, ecx
        jmp    SHORT loc_1_5B25
loc_1_5AC4:
        cmp    ecx, eax
        jbe    SHORT loc_1_5ACA
        mov    ecx, eax
loc_1_5ACA:
        push   ecx
        mov    eax, dword ptr [esi + 54h]
        xor    edx, edx
        add    eax, ecx
        sub    eax, dword ptr [esi + 40h]
        jbe    SHORT loc_1_5ADB
        mov    edx, eax
        sub    ecx, edx
loc_1_5ADB:
        mov    edi, dword ptr [ebp + 0ch]
        push   esi
        mov    eax, dword ptr [esi + 3ch]
        mov    esi, dword ptr [esi + 54h]
        add    esi, eax
        xchg   edi, esi
        cld
        rep movsb
        mov    ecx, edx
        jecxz  loc_1_5AF4
        mov    edi, eax
        rep movsb
loc_1_5AF4:
        pop    esi
        sub    edi, eax
        cmp    edi, dword ptr [esi + 40h]
        jb     SHORT loc_1_5AFE
        xor    edi, edi
loc_1_5AFE:
        mov    dword ptr [esi + 54h], edi
        pop    ecx
        add    dword ptr [esi + 4ch], ecx
        test   byte ptr [esi + 0cdh], 1
        jne    SHORT loc_1_5B23
        mov    eax, dword ptr [esi + 4ch]
        cmp    eax, dword ptr [esi + 0e0h]
        jb     SHORT loc_1_5B23
        mov    eax, 1
        call   sub_1_4BED
loc_1_5B23:
        mov    eax, ecx
loc_1_5B25:
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], eax
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_1_5B35:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 105h], 80h
        jne    SHORT loc_1_5B53
loc_1_5B47:
        xor    eax, eax
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], eax
        jmp    NEAR PTR loc_1_5D15
loc_1_5B53:
        mov    ecx, dword ptr [ebp + 10h]
        or     ecx, ecx
        je     SHORT loc_1_5B47
        test   byte ptr [esi + 108h], 12h
        jne    SHORT loc_1_5B47
        mov    eax, dword ptr [esi + 40h]
        sub    eax, dword ptr [esi + 4ch]
        jle    SHORT loc_1_5B47
        cmp    ecx, eax
        jbe    SHORT loc_1_5B71
        mov    ecx, eax
loc_1_5B71:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    ebx, dword ptr [esi + 100h]
        mov    dword ptr [ebx], eax
        and    byte ptr [esi + 0cdh], 7fh
        mov    ebx, dword ptr [esi + 0f0h]
        mov    eax, dword ptr [ebx]
        push   eax
        or     byte ptr [ebx], 1
        push   ecx
        mov    edx, dword ptr [ebp + 0ch]
loc_1_5B96:
        or     ecx, ecx
        je     NEAR PTR loc_1_5CC5
        test   byte ptr [esi + 108h], 12h
        jne    NEAR PTR loc_1_5CC5
        mov    al, byte ptr [edx]
        inc    edx
        dec    ecx
        test   byte ptr [esi + 104h], 20h
        je     SHORT loc_1_5BBE
        or     dword ptr [esi + 34h], 1
        jmp    SHORT loc_1_5B96
loc_1_5BBE:
        or     al, al
        jne    SHORT loc_1_5BCB
        test   byte ptr [esi + 9dh], 8
        jne    SHORT loc_1_5B96
loc_1_5BCB:
        cmp    al, byte ptr [esi + 0b0h]
        jne    SHORT loc_1_5BE5
        test   byte ptr [esi + 9ch], 1
        jne    SHORT loc_1_5BE5
        or     byte ptr [esi + 104h], 20h
        jmp    SHORT loc_1_5B96
loc_1_5BE5:
        push   edx
        push   ecx
        mov    ecx, dword ptr [esi + 4ch]
        test   byte ptr [esi + 9dh], 1
        je     SHORT loc_1_5C49
        cmp    al, byte ptr [esi + 0aeh]
        jne    SHORT loc_1_5C28
        or     byte ptr [esi + 108h], 8
        test   byte ptr [esi + 9ch], 60h
        je     SHORT loc_1_5C21
        cmp    ecx, dword ptr [esi + 0a0h]
        ja     SHORT loc_1_5C21
        and    byte ptr [esi + 108h], 0f7h
        and    byte ptr [esi + 108h], 0ebh
loc_1_5C21:
        pop    ecx
        pop    edx
        jmp    NEAR PTR loc_1_5B96
loc_1_5C28:
        cmp    al, byte ptr [esi + 0adh]
        jne    SHORT loc_1_5C49
        and    byte ptr [esi + 108h], 0f7h
        test   byte ptr [esi + 9ch], 60h
        je     SHORT loc_1_5C47
        and    byte ptr [esi + 108h], 0f7h
loc_1_5C47:
        jmp    SHORT loc_1_5C21
loc_1_5C49:
        cmp    al, byte ptr [esi + 0b1h]
        jne    SHORT loc_1_5C5A
        mov    ebx, dword ptr [esi + 0f0h]
        or     byte ptr [ebx], 2
loc_1_5C5A:
        test   byte ptr [esi + 105h], 1
        jne    SHORT loc_1_5C21
        mov    edi, dword ptr [esi + 3ch]
        mov    ebx, dword ptr [esi + 54h]
        mov    byte ptr [edi + ebx], al
        inc    ebx
        cmp    ebx, dword ptr [esi + 40h]
        jb     SHORT loc_1_5C74
        xor    ebx, ebx
loc_1_5C74:
        mov    dword ptr [esi + 54h], ebx
        inc    ecx
        mov    dword ptr [esi + 4ch], ecx
        cmp    ecx, dword ptr [esi + 0e8h]
        jb     SHORT loc_1_5C21
        test   byte ptr [esi + 108h], 12h
        jne    SHORT loc_1_5C21
        mov    ah, byte ptr [esi + 109h]
        or     ah, ah
        je     SHORT loc_1_5CAC
        mov    al, byte ptr [esi + 10eh]
        not    ah
        and    al, ah
        or     byte ptr [esi + 108h], 2
        call   sub_1_5036
loc_1_5CAC:
        test   byte ptr [esi + 9dh], 2
        je     NEAR PTR loc_1_5C21
        or     byte ptr [esi + 108h], 10h
        jmp    NEAR PTR loc_1_5C21
loc_1_5CC5:
        pop    eax
        sub    eax, ecx
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], eax
        test   byte ptr [esi + 0cdh], 1
        jne    SHORT loc_1_5CEB
        mov    eax, dword ptr [esi + 4ch]
        cmp    eax, dword ptr [esi + 0e0h]
        jb     SHORT loc_1_5CEB
        mov    eax, 1
        call   sub_1_4BED
loc_1_5CEB:
        mov    ebx, dword ptr [esi + 0f0h]
        mov    eax, dword ptr [esi + 0ech]
        and    eax, dword ptr [ebx]
        mov    dword ptr [ebx], eax
        pop    ebx
        test   byte ptr [esi + 0cdh], 40h
        je     SHORT loc_1_5D15
        not    ebx
        and    eax, ebx
        je     SHORT loc_1_5D15
        mov    eax, 4
        call   sub_1_4BED
loc_1_5D15:
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_1_5D20:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        xor    eax, eax
        or     ecx, ecx
        je     SHORT loc_1_5D6A
        mov    eax, dword ptr [esi + 58h]
        or     eax, eax
        je     SHORT loc_1_5D6A
        cmp    ecx, eax
        jbe    SHORT loc_1_5D3F
        mov    ecx, eax
loc_1_5D3F:
        push   ecx
        mov    edx, dword ptr [esi + 48h]
        mov    eax, dword ptr [esi + 5ch]
        sub    edx, eax
        cmp    edx, ecx
        jbe    SHORT loc_1_5D4E
        mov    edx, ecx
loc_1_5D4E:
        xchg   edx, ecx
        sub    edx, ecx
        push   esi
        mov    ebx, dword ptr [esi + 44h]
        mov    esi, ebx
        add    esi, eax
        mov    edi, dword ptr [ebp + 0ch]
        cld
        rep movsb
        mov    ecx, edx
        jecxz  loc_1_5D68
        mov    esi, ebx
        rep movsb
loc_1_5D68:
        pop    esi
        pop    eax
loc_1_5D6A:
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], eax
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_1_5D7A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        xor    eax, eax
        or     ecx, ecx
        je     SHORT loc_1_5DFB
        mov    eax, dword ptr [esi + 58h]
        or     eax, eax
        jne    SHORT loc_1_5D9E
        mov    ebx, dword ptr [esi + 0f0h]
        or     byte ptr [ebx], 4
        jmp    SHORT loc_1_5DFB
loc_1_5D9E:
        cmp    ecx, eax
        jbe    SHORT loc_1_5DA4
        mov    ecx, eax
loc_1_5DA4:
        push   ecx
        test   byte ptr [esi + 9ch], 20h
        je     SHORT loc_1_5DD1
        mov    eax, dword ptr [esi + 0f8h]
        or     eax, eax
        jne    SHORT loc_1_5DBF
        or     byte ptr [esi + 108h], 8
loc_1_5DBF:
        add    eax, ecx
        cmp    eax, dword ptr [esi + 0a0h]
        jb     SHORT loc_1_5DCB
        xor    eax, eax
loc_1_5DCB:
        mov    dword ptr [esi + 0f8h], eax
loc_1_5DD1:
        mov    edx, dword ptr [esi + 48h]
        mov    eax, dword ptr [esi + 5ch]
        sub    edx, eax
        cmp    edx, ecx
        jbe    SHORT loc_1_5DDF
        mov    edx, ecx
loc_1_5DDF:
        xchg   edx, ecx
        sub    edx, ecx
        push   esi
        mov    ebx, dword ptr [esi + 44h]
        mov    esi, ebx
        add    esi, eax
        mov    edi, dword ptr [ebp + 0ch]
        cld
        rep movsb
        mov    ecx, edx
        jecxz  loc_1_5DF9
        mov    esi, ebx
        rep movsb
loc_1_5DF9:
        pop    esi
        pop    eax
loc_1_5DFB:
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], eax
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_1_5E0B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        or     ecx, ecx
        je     SHORT loc_1_5E4F
        cli
        mov    ebx, dword ptr [esi + 5ch]
        add    ebx, ecx
        mov    edx, dword ptr [esi + 48h]
        cmp    ebx, edx
        jb     SHORT loc_1_5E2A
        sub    ebx, edx
loc_1_5E2A:
        mov    dword ptr [esi + 5ch], ebx
        sub    dword ptr [esi + 58h], ecx
        sti
        test   byte ptr [esi + 0cdh], 2
        jne    SHORT loc_1_5E4F
        mov    eax, dword ptr [esi + 58h]
        cmp    eax, dword ptr [esi + 0e4h]
        jae    SHORT loc_1_5E4F
        mov    eax, 2
        call   sub_1_4BED
loc_1_5E4F:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_1_5E54:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        or     ecx, ecx
        je     SHORT loc_1_5ED2
        cli
        mov    ebx, dword ptr [esi + 5ch]
        add    ebx, ecx
        mov    edx, dword ptr [esi + 48h]
        cmp    ebx, edx
        jb     SHORT loc_1_5E73
        sub    ebx, edx
loc_1_5E73:
        mov    dword ptr [esi + 5ch], ebx
        sub    dword ptr [esi + 58h], ecx
        sti
        test   byte ptr [esi + 9ch], 40h
        je     SHORT loc_1_5EA8
        add    ecx, dword ptr [esi + 0f8h]
        mov    dword ptr [esi + 0f8h], ecx
        cmp    ecx, dword ptr [esi + 0a0h]
        jbe    SHORT loc_1_5EA8
        mov    dword ptr [esi + 0f8h], 0
        or     byte ptr [esi + 108h], 8
loc_1_5EA8:
        mov    ebx, dword ptr [esi + 0f0h]
        or     dword ptr [ebx], 4000h
        test   byte ptr [esi + 0cdh], 2
        jne    SHORT loc_1_5ED2
        mov    eax, dword ptr [esi + 58h]
        cmp    eax, dword ptr [esi + 0e4h]
        jae    SHORT loc_1_5ED2
        mov    eax, 2
        call   sub_1_4BED
loc_1_5ED2:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
        db  0CCh
sub_1_5ED8:
        db  0CDh, 020h, 000h, 000h, 0A2h, 004h
        jae    SHORT loc_1_5EE2
        xor    eax, eax
loc_1_5EE2:
        ret
sub_1_5EE3:
        db  0CDh, 020h, 002h, 080h, 0A2h, 004h
sub_1_5EE9:
        db  0CDh, 020h, 01Ch, 080h, 0A2h, 004h
sub_1_5EEF:
        db  0CDh, 020h, 01Dh, 080h, 0A2h, 004h
        db  0CDh, 020h, 01Eh, 080h, 0A2h, 004h
sub_1_5EFB:
        db  0CDh, 020h, 004h, 080h, 0A2h, 004h
sub_1_5F01:
        db  0CDh, 020h, 005h, 080h, 0A2h, 004h
sub_1_5F07:
        db  0CDh, 020h, 006h, 080h, 0A2h, 004h
sub_1_5F0D:
        db  0CDh, 020h, 007h, 080h, 0A2h, 004h
sub_1_5F13:
        db  0CDh, 020h, 008h, 080h, 0A2h, 004h
sub_1_5F19:
        db  0CDh, 020h, 009h, 080h, 0A2h, 004h
sub_1_5F1F:
        db  0CDh, 020h, 00Ah, 080h, 0A2h, 004h
        db  0CDh, 020h, 00Ch, 080h, 0A2h, 004h
sub_1_5F2B:
        db  0CDh, 020h, 00Eh, 080h, 0A2h, 004h
sub_1_5F31:
        db  0CDh, 020h, 019h, 080h, 0A2h, 004h
sub_1_5F37:
        db  0CDh, 020h, 01Ah, 080h, 0A2h, 004h
sub_1_5F3D:
        db  0CDh, 020h, 018h, 080h, 0A2h, 004h
sub_1_5F43:
        db  0CDh, 020h, 022h, 080h, 0A2h, 004h
        int3
        int3
        int3
sub_1_5F4C:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_5F52:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_5F5F
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
loc_1_5F5F:
        ret
sub_1_5F60:
        db  0CDh, 020h, 011h, 080h, 040h, 000h
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
sub_1_5F70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_5FD1
loc_1_5FD1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_6053:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     NEAR PTR loc_1_6082
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], eax
        jne    NEAR PTR loc_1_6075
        mov    eax, 1
        jmp    NEAR PTR loc_1_6089
loc_1_6075:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_6053
loc_1_6082:
        xor    eax, eax
        jmp    NEAR PTR loc_1_6089
loc_1_6089:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        jne    NEAR PTR loc_1_60BF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     NEAR PTR loc_1_60C6
loc_1_60BF:
        xor    eax, eax
        jmp    NEAR PTR loc_1_611F
loc_1_60C6:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_60DB
loc_1_60D3:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_60DB:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     NEAR PTR loc_1_6115
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        jne    NEAR PTR loc_1_6109
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], ecx
        je     NEAR PTR loc_1_6110
loc_1_6109:
        xor    eax, eax
        jmp    NEAR PTR loc_1_611F
loc_1_6110:
        jmp    NEAR PTR loc_1_60D3
loc_1_6115:
        mov    eax, 1
        jmp    NEAR PTR loc_1_611F
loc_1_611F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6124:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_1_6134:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_1_6144:
        db  0CDh, 020h, 027h, 081h, 001h, 000h
        int3
        int3
sub_1_614C:
        db  0CDh, 020h, 0FAh, 080h, 001h, 000h
        int3
        int3
_LTEXT ends
        end
