; vnbt_lcode_01_03.asm -- VNBT object 1 segment _LTEXT  (part 3/5)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_9154:
        sub    esp, 1ch
        push   esi
        db  00Fh, 0B7h, 005h, 0D4h, 032h, 001h, 000h
        add    eax, 39h
        mov    ecx, dword ptr [esp + 30h]
        cmp    eax, ecx
        jle    SHORT loc_1_9174
        mov    eax, 17h
        pop    esi
        add    esp, 1ch
        ret
loc_1_9174:
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ecx
        mov    esi, dword ptr [esp + 3ch]
        lea    eax, [esi + 0ch]
        push   eax
        call   sub_1_FFAC
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_91AC
        cmp    eax, 0ffh
        je     SHORT loc_1_91AC
        mov    eax, 17h
        pop    esi
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_91AC:
        lea    eax, [esp + 4]
        push   eax
        push   dword ptr [esp + 10h]
        lea    eax, [esp + 18h]
        push   eax
        db  0FFh, 035h, 0B4h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_91D3
        cmp    eax, 0ffh
        jne    SHORT loc_1_9228
loc_1_91D3:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + eax + 11h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        and    ecx, 0ffh
        or     edx, ecx
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     SHORT loc_1_9228
        lea    eax, [edx + edx*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        shl    eax, 3
        cmp    dword ptr [ecx + 38h], eax
        jae    SHORT loc_1_9228
        mov    dword ptr [ecx + 38h], eax
        or     word ptr [ecx + 3ch], 10h
loc_1_9228:
        pop    esi
        mov    eax, 17h
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
sub_1_9234:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [ecx + eax + 11h]
        mov    ecx, edx
        and    ecx, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    eax, 8
        or     ecx, eax
        shr    edx, 18h
        and    edx, 0ffh
        or     ecx, edx
        cmp    ecx, 418937h
        mov    esi, 0fffffffeh
        jae    SHORT loc_1_9289
        lea    eax, [ecx + ecx*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    esi, [eax*8]
loc_1_9289:
        or     esi, esi
        jne    SHORT loc_1_9294
        mov    esi, 0ffffffffh
        jmp    SHORT loc_1_92A1
loc_1_9294:
        cmp    esi, 493e0h
        jae    SHORT loc_1_92A1
        mov    esi, 493e0h
loc_1_92A1:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 18h], esi
        db  039h, 035h, 0E0h, 032h, 001h, 000h
        jbe    SHORT loc_1_9324
        db  089h, 035h, 0E0h, 032h, 001h, 000h
        db  0A1h, 0F0h, 032h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_92D5
        mov    ecx, eax
        mov    dword ptr [ecx + 28h], 0
        push   0
        push   0
        push   eax
        call   sub_1_10AEC
        add    esp, 0ch
loc_1_92D5:
        sub    edx, edx
        mov    ecx, 927c0h
        db  0A1h, 0E0h, 032h, 001h, 000h
        div    ecx
        db  0A3h, 0E4h, 032h, 001h, 000h
        cmp    eax, 8
        jae    SHORT loc_1_92F7
        db  0C7h, 005h, 0E4h, 032h, 001h, 000h, 008h, 000h, 000h, 000h
loc_1_92F7:
        db  068h, 0F0h, 032h, 001h, 000h
        push   0
        push   0
        push   0
        db  068h, 0C0h, 0F5h, 000h, 000h
        push   0
        push   0
        mov    eax, esi
        sub    edx, edx
        db  0F7h, 035h, 0E4h, 032h, 001h, 000h
        push   eax
        call   sub_1_109B8
        add    esp, 20h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_9324:
        jae    SHORT loc_1_9389
        sub    ebx, ebx
        db  0A1h, 0B4h, 032h, 001h, 000h
        mov    edx, dword ptr [eax]
        cmp    edx, ebx
        jle    SHORT loc_1_9364
        mov    edi, eax
        add    edi, 8
loc_1_9338:
        mov    eax, dword ptr [edi]
        cmp    edi, eax
        je     SHORT loc_1_935C
loc_1_933E:
        cmp    byte ptr [eax + 34h], 2ah
        je     SHORT loc_1_9356
        mov    ecx, dword ptr [eax + 20h]
        test   cl, 10h
        je     SHORT loc_1_9356
        cmp    dword ptr [eax + 18h], esi
        jae    SHORT loc_1_9356
        test   cl, 1
        je     SHORT loc_1_9390
loc_1_9356:
        mov    eax, dword ptr [eax]
        cmp    edi, eax
        jne    SHORT loc_1_933E
loc_1_935C:
        inc    ebx
        add    edi, 8
        cmp    ebx, edx
        jl     SHORT loc_1_9338
loc_1_9364:
        mov    eax, esi
        db  089h, 035h, 0E0h, 032h, 001h, 000h
        mov    ecx, 927c0h
        sub    edx, edx
        div    ecx
        db  0A3h, 0E4h, 032h, 001h, 000h
        cmp    eax, 8
        jae    SHORT loc_1_9389
        db  0C7h, 005h, 0E4h, 032h, 001h, 000h, 008h, 000h, 000h, 000h
loc_1_9389:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_9390:
        mov    eax, dword ptr [eax + 18h]
        cmp    eax, 493e0h
        jb     SHORT loc_1_9389
        db  0A3h, 0E0h, 032h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_93A4:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 02Dh, 06Ch, 032h, 001h, 000h
        db  081h, 0FDh, 06Ch, 032h, 001h, 000h
        je     NEAR PTR loc_1_9483
loc_1_93BD:
        mov    ebx, ebp
        push   10h
        mov    eax, dword ptr [ebp + 38h]
        add    eax, 34h
        push   eax
        push   dword ptr [esp + 2ch]
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, 10h
        jne    NEAR PTR loc_1_9474
        mov    ecx, dword ptr [ebp]
        mov    edx, ecx
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        mov    ebp, ecx
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 28h]
        lea    esi, [eax + ecx + 0dh]
        mov    eax, ecx
        add    eax, 0dh
        sub    dword ptr [esp + 20h], eax
        mov    ecx, dword ptr [ebx + 30h]
        or     ecx, ecx
        je     SHORT loc_1_9477
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0ch
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_9477
        push   dword ptr [esp + 20h]
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_9477
        mov    edx, dword ptr [esp + 20h]
        mov    edi, eax
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [ebx + 44h], ecx
        mov    dword ptr [ebx + 30h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebx + 40h], eax
        push   0
        db  068h, 090h, 094h, 000h, 000h
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 20h]
        push   ebx
        call   sub_1_3C00
        add    esp, 14h
        jmp    SHORT loc_1_9477
        db  08Dh, 049h, 000h
loc_1_9474:
        mov    ebp, dword ptr [ebp]
loc_1_9477:
        db  081h, 0FDh, 06Ch, 032h, 001h, 000h
        jne    NEAR PTR loc_1_93BD
loc_1_9483:
        pop    ebp
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_9490:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 34h]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ecx + 40h]
        mov    dword ptr [esp + 24h], eax
        mov    dword ptr [ecx + 40h], 0
        mov    edx, dword ptr [ecx + 30h]
        mov    dword ptr [esp + 28h], edx
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [esp + 2ch], eax
        movzx  eax, byte ptr [edx + 0ah]
        mov    dword ptr [esp + 20h], eax
        add    eax, eax
        lea    ebx, [eax + eax*8]
        lea    ecx, [ebx + 3ch]
        mov    dword ptr [esp + 1ch], ecx
        cmp    ecx, 0ffffh
        jbe    SHORT loc_1_94E0
        mov    ebx, 0c0000001h
        jmp    NEAR PTR loc_1_9687
loc_1_94E0:
        movzx  eax, word ptr [esp + 1ch]
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_9500
        mov    ebx, 1
        jmp    NEAR PTR loc_1_9687
        db  08Bh, 0FFh
loc_1_9500:
        mov    edi, ebp
        mov    edx, dword ptr [esp + 1ch]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        lea    eax, [ebx + 2eh]
        cmp    eax, dword ptr [esp + 24h]
        ja     SHORT loc_1_952C
        mov    edx, dword ptr [esp + 28h]
        lea    esi, [ebx + edx + 0bh]
        mov    edi, ebp
        movsd
        movsw
loc_1_952C:
        add    ebx, 39h
        mov    byte ptr [ebp + 6], 3
        mov    byte ptr [ebp + 8], 0feh
        cmp    ebx, dword ptr [esp + 24h]
        ja     SHORT loc_1_955C
        mov    edx, dword ptr [esp + 28h]
        mov    cl, byte ptr [edx + ebx - 2]
        movzx  ax, cl
        shl    ax, 8
        shr    cl, 8
        and    cx, 0ffh
        or     ax, cx
        mov    word ptr [ebp + 36h], ax
loc_1_955C:
        lea    eax, [ebp + 3ch]
        mov    dword ptr [esp + 18h], 0bh
        mov    byte ptr [esp + 13h], 0
        mov    dword ptr [esp + 14h], 0
        cmp    dword ptr [esp + 20h], 0
        je     NEAR PTR loc_1_9605
        mov    ecx, dword ptr [esp + 28h]
        add    ecx, 0bh
loc_1_9586:
        add    dword ptr [esp + 18h], 12h
        mov    edx, dword ptr [esp + 18h]
        cmp    edx, dword ptr [esp + 24h]
        ja     SHORT loc_1_9600
        inc    word ptr [ebp + 3ah]
        mov    dl, byte ptr [ecx + 10h]
        mov    bl, dl
        and    bl, 80h
        cmp    bl, 1
        cmc
        sbb    bl, bl
        and    bl, 80h
        mov    byte ptr [eax + 11h], bl
        test   dl, 8
        je     SHORT loc_1_95C4
        test   dl, 10h
        je     SHORT loc_1_95BC
        or     bl, 7
        jmp    SHORT loc_1_95D3
loc_1_95BC:
        or     bl, 6
        jmp    SHORT loc_1_95D3
        db  08Dh, 049h, 000h
loc_1_95C4:
        test   dl, 10h
        je     SHORT loc_1_95D0
        or     bl, 5
        jmp    SHORT loc_1_95D3
        db  08Bh, 0FFh
loc_1_95D0:
        or     bl, 4
loc_1_95D3:
        mov    edi, eax
        mov    byte ptr [eax + 11h], bl
        mov    edx, dword ptr [esp + 14h]
        inc    dl
        mov    byte ptr [eax + 10h], dl
        mov    esi, ecx
        movsd
        movsd
        movsd
        movsd
        add    eax, 12h
        add    ecx, 12h
        inc    dword ptr [esp + 14h]
        mov    edx, dword ptr [esp + 14h]
        cmp    edx, dword ptr [esp + 20h]
        jb     SHORT loc_1_9586
        jmp    SHORT loc_1_9605
        db  08Dh, 049h, 000h
loc_1_9600:
        mov    byte ptr [esp + 13h], 1
loc_1_9605:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_9614
        mov    ebx, 0c0000001h
        jmp    SHORT loc_1_967E
        db  090h
loc_1_9614:
        mov    eax, dword ptr [esp + 2ch]
        movzx  esi, word ptr [eax + 8]
        cmp    dword ptr [esp + 1ch], esi
        jbe    SHORT loc_1_9654
        cmp    esi, 3ch
        jae    SHORT loc_1_9630
        mov    word ptr [ebp + 3ah], 0
        jmp    SHORT loc_1_9641
        db  090h
loc_1_9630:
        lea    eax, [esi - 3ch]
        mov    cx, 12h
        sub    dx, dx
        div    cx
        mov    word ptr [ebp + 3ah], ax
loc_1_9641:
        cmp    dword ptr [esp + 1ch], esi
        jbe    SHORT loc_1_9654
        mov    ebx, 9
        mov    dword ptr [esp + 1ch], esi
        jmp    SHORT loc_1_9656
        db  08Bh, 0FFh
loc_1_9654:
        sub    ebx, ebx
loc_1_9656:
        mov    esi, ebp
        mov    edx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 2ch]
        mov    edi, dword ptr [eax + 4]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 2ch]
        mov    cx, word ptr [esp + 1ch]
        mov    word ptr [eax + 8], cx
loc_1_967E:
        push   ebp
        call   sub_1_0024
        add    esp, 4
loc_1_9687:
        mov    eax, dword ptr [esp + 34h]
        mov    edi, dword ptr [eax + 0ch]
        mov    esi, dword ptr [eax + 10h]
        push   dword ptr [esp + 28h]
        call   sub_1_0024
        add    esp, 4
        push   dword ptr [esp + 34h]
        call   sub_1_0024
        add    esp, 4
        push   ebx
        push   edi
        call   esi
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
sub_1_96B8:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 34h]
        add    eax, 11h
        mov    ebp, dword ptr [esp + 2ch]
        cmp    eax, ebp
        jae    SHORT loc_1_96DC
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
loc_1_96DC:
        db  08Bh, 015h, 0D8h, 032h, 001h, 000h
        mov    esi, dword ptr [esp + 28h]
        lea    edi, [esi + 2dh]
        lea    eax, [ebp - 31h]
        db  00Fh, 0B7h, 00Dh, 0D4h, 032h, 001h, 000h
        cmp    ecx, eax
        je     SHORT loc_1_9704
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_9704:
        sub    ebx, ebx
        db  066h, 039h, 01Dh, 0D4h, 032h, 001h, 000h
        jbe    SHORT loc_1_971D
        nop
loc_1_9710:
        mov    al, byte ptr [edx]
        cmp    byte ptr [edi], al
        jne    SHORT loc_1_9758
        inc    ebx
        inc    edi
        inc    edx
        cmp    ebx, ecx
        jb     SHORT loc_1_9710
loc_1_971D:
        db  068h, 034h, 032h, 001h, 000h
        call   sub_1_10288
        add    esp, 4
        add    eax, eax
        lea    eax, [eax + eax*8]
        lea    ecx, [eax + ebp + 35h]
        mov    dword ptr [esp + 20h], ecx
        movzx  eax, cx
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_9768
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_9758:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_9768:
        mov    edi, ebx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    word ptr [ebx + 2], 84h
        mov    byte ptr [ebx + 5], 0
        mov    byte ptr [ebx + 7], 1
        mov    eax, dword ptr [esp + 34h]
        lea    ecx, [eax + ebx + 0dh]
        mov    dword ptr [esp + 18h], ecx
        mov    dword ptr [ecx + 4], 0
        mov    edx, ecx
        add    edx, 0bh
        mov    eax, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [eax + 88h]
        mov    dword ptr [esp + 1ch], ecx
        mov    dword ptr [esp + 10h], 0
        db  08Bh, 02Dh, 074h, 032h, 001h, 000h
        db  081h, 0FDh, 074h, 032h, 001h, 000h
        je     NEAR PTR loc_1_9873
loc_1_97C6:
        mov    eax, dword ptr [ebp + 14h]
        cmp    word ptr [ebp + 20h], 1
        cmc
        sbb    cl, cl
        and    cl, 80h
        mov    byte ptr [edx + 10h], cl
        or     cl, 4
        mov    byte ptr [edx + 10h], cl
        mov    esi, dword ptr [eax + 20h]
        and    esi, 0f0h
        cmp    esi, 20h
        je     SHORT loc_1_97FC
        cmp    esi, 40h
        je     SHORT loc_1_9804
        cmp    esi, 80h
        je     SHORT loc_1_9864
        jmp    SHORT loc_1_980A
        db  08Bh, 0FFh
loc_1_97FC:
        or     cl, 10h
        jmp    SHORT loc_1_9807
        db  08Dh, 049h, 000h
loc_1_9804:
        or     cl, 8
loc_1_9807:
        mov    byte ptr [edx + 10h], cl
loc_1_980A:
        db  066h, 08Bh, 00Dh, 064h, 033h, 001h, 000h
        and    ecx, 0fh
        cmp    ecx, 2
        je     SHORT loc_1_9828
        cmp    ecx, 4
        je     SHORT loc_1_9830
        cmp    ecx, 8
        je     SHORT loc_1_9830
        jmp    SHORT loc_1_9834
        db  08Dh, 049h, 000h
loc_1_9828:
        or     byte ptr [edx + 10h], 20h
        jmp    SHORT loc_1_9834
        db  08Bh, 0FFh
loc_1_9830:
        or     byte ptr [edx + 10h], 40h
loc_1_9834:
        mov    edi, edx
        mov    byte ptr [edx + 11h], 0
        lea    esi, [eax + 34h]
        movsd
        movsd
        movsd
        movsd
        mov    ecx, dword ptr [eax + 20h]
        test   cl, 1
        jne    SHORT loc_1_9864
        cmp    byte ptr [eax + 34h], 2ah
        je     SHORT loc_1_9864
        test   cl, 80h
        jne    SHORT loc_1_9864
        mov    ecx, dword ptr [esp + 1ch]
        test   dword ptr [eax + 28h], ecx
        je     SHORT loc_1_9864
        inc    dword ptr [esp + 10h]
        add    edx, 12h
loc_1_9864:
        mov    ebp, dword ptr [ebp]
        db  081h, 0FDh, 074h, 032h, 001h, 000h
        jne    NEAR PTR loc_1_97C6
loc_1_9873:
        mov    al, byte ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 18h]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [esp + 10h]
        add    eax, eax
        lea    eax, [eax + eax*8]
        lea    ecx, [eax + 2fh]
        mov    dl, ch
        mov    dh, cl
        mov    ecx, dword ptr [esp + 18h]
        mov    word ptr [ecx + 8], dx
        lea    edx, [eax + ecx + 0bh]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0bh
        rep stosd
        stosw
        mov    esi, dword ptr [esp + 3ch]
        add    esi, 8ch
        mov    edi, edx
        movsd
        movsw
        lea    eax, [esp + 14h]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_98E4
        cmp    esi, 0ffh
        je     SHORT loc_1_98E4
        push   ebx
        call   sub_1_0024
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_98E4:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [eax + 18h], ecx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 1ch], ebx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 20h], 0
        push   10h
        push   89h
        push   dword ptr [esp + 1ch]
        db  068h, 0C4h, 015h, 001h, 000h
        mov    eax, dword ptr [esp + 4ch]
        push   dword ptr [eax + 68h]
        push   dword ptr [esp + 4ch]
        push   dword ptr [esp + 2ch]
        call   sub_1_115D4
        add    esp, 1ch
        mov    esi, eax
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
sub_1_9940:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        mov    eax, dword ptr [esi + 20h]
        test   al, 80h
        je     NEAR PTR loc_1_9AF7
        and    eax, 0ffffff0fh
        mov    dword ptr [esi + 20h], eax
        or     eax, 10h
        mov    dword ptr [esi + 20h], eax
        mov    edi, dword ptr [esp + 20h]
        mov    cx, word ptr [edi]
        test   cl, 80h
        je     NEAR PTR loc_1_9A9C
        and    eax, 0fffffff0h
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [esp + 28h]
        cmp    eax, 6
        jne    SHORT loc_1_99E0
        cmp    byte ptr [esp + 30h], 0
        je     SHORT loc_1_9990
        cmp    dword ptr [edi + 2], -1
        jne    SHORT loc_1_99E0
loc_1_9990:
        cmp    dword ptr [edi + 2], -1
        jne    SHORT loc_1_99A8
        mov    dword ptr [esi + 0ch], 0
        or     dword ptr [esi + 20h], 4
        jmp    NEAR PTR loc_1_9AF7
        db  08Bh, 0FFh
loc_1_99A8:
        mov    ecx, dword ptr [edi + 2]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        and    ecx, 0ffh
        or     edx, ecx
        mov    dword ptr [esi + 0ch], edx
        or     dword ptr [esi + 20h], 4
        jmp    NEAR PTR loc_1_9AF7
loc_1_99E0:
        sub    edx, edx
        mov    ecx, 6
        div    ecx
        mov    dword ptr [esp + 10h], eax
        cmp    eax, 3e8h
        jle    SHORT loc_1_9A04
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_1_9A04:
        mov    eax, dword ptr [esp + 10h]
        lea    eax, [eax*4 + 8]
        and    eax, 0ffffh
        push   eax
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_9A30
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_9A30:
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [esp + 10h]
        inc    ecx
        cmp    ecx, 1
        jle    SHORT loc_1_9A87
        lea    ebx, [eax + 4]
        inc    ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [esp + 14h], ecx
loc_1_9A4C:
        mov    ecx, dword ptr [edi + 2]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    ebp, ecx
        shl    ebp, 10h
        or     edx, ebp
        shl    edx, 8
        mov    ebp, ecx
        and    ebp, 0ff0000h
        shr    ebp, 8
        or     edx, ebp
        shr    ecx, 18h
        and    ecx, 0ffh
        or     edx, ecx
        mov    dword ptr [ebx], edx
        add    edi, 6
        add    ebx, 4
        dec    dword ptr [esp + 14h]
        jne    SHORT loc_1_9A4C
loc_1_9A87:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + ecx*4 + 4], 0ffffffffh
        mov    dword ptr [esi + 0ch], eax
        or     dword ptr [esi + 20h], 8
        jmp    SHORT loc_1_9AF7
loc_1_9A9C:
        mov    eax, dword ptr [esp + 28h]
        cmp    eax, 6
        jbe    SHORT loc_1_9AC8
        cmp    byte ptr [esp + 30h], 0
        je     SHORT loc_1_9AC8
        lea    ecx, [esp + 18h]
        push   ecx
        push   dword ptr [esp + 30h]
        push   eax
        push   edi
        call   sub_1_9C54
        add    esp, 10h
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 0ch], eax
        jmp    SHORT loc_1_9AF7
loc_1_9AC8:
        mov    ecx, dword ptr [edi + 2]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        and    ecx, 0ffh
        or     edx, ecx
        mov    dword ptr [esi + 0ch], edx
loc_1_9AF7:
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   esi
        call   sub_1_6608
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_9B1A
        cmp    eax, 0ffh
        je     SHORT loc_1_9B1A
        push   esi
        call   sub_1_DEA4
        add    esp, 4
loc_1_9B1A:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_9B24:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        db  08Bh, 035h, 03Ch, 032h, 001h, 000h
        db  081h, 0FEh, 03Ch, 032h, 001h, 000h
        je     NEAR PTR loc_1_9C4A
        inc    ecx
        mov    ecx, dword ptr [esp + 2ch]
loc_1_9B44:
        mov    edi, dword ptr [esp + 28h]
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_1_9B59
        cmp    esi, dword ptr [esp + 20h]
        jne    NEAR PTR loc_1_9C3C
loc_1_9B59:
        cmp    byte ptr [esp + 34h], 0
        je     SHORT loc_1_9B68
        mov    edx, dword ptr [esi + 58h]
        and    edx, dword ptr [esi + 5ch]
        jmp    SHORT loc_1_9B6B
loc_1_9B68:
        mov    edx, dword ptr [esi + 64h]
loc_1_9B6B:
        mov    dword ptr [esp + 14h], edx
        cmp    dword ptr [esp + 24h], 0
        je     NEAR PTR loc_1_9C3C
        mov    edx, dword ptr [esp + 24h]
        mov    dword ptr [esp + 18h], edx
loc_1_9B82:
        mov    edx, dword ptr [edi + 2]
        mov    ebx, edx
        and    ebx, 0ff00h
        mov    ebp, edx
        shl    ebp, 10h
        or     ebx, ebp
        shl    ebx, 8
        mov    ebp, edx
        and    ebp, 0ff0000h
        shr    ebp, 8
        or     ebx, ebp
        shr    edx, 18h
        and    edx, 0ffh
        or     ebx, edx
        mov    dword ptr [esp + 10h], ebx
        cmp    byte ptr [esp + 34h], 0
        je     SHORT loc_1_9BD4
        mov    edx, dword ptr [esi + 5ch]
        and    edx, ebx
        cmp    edx, dword ptr [esp + 14h]
        jne    SHORT loc_1_9C2F
        mov    edx, dword ptr [esp + 30h]
        shr    edx, 2
        cmp    edx, eax
        jbe    SHORT loc_1_9C2F
        mov    edx, ebx
        jmp    SHORT loc_1_9C2B
loc_1_9BD4:
        test   byte ptr [esp + 13h], 80h
        jne    SHORT loc_1_9BE8
        mov    ebx, dword ptr [esp + 10h]
        and    ebx, 0ff000000h
        jmp    SHORT loc_1_9C16
        db  090h
loc_1_9BE8:
        mov    dl, byte ptr [esp + 13h]
        and    dl, 0c0h
        cmp    dl, 80h
        jne    SHORT loc_1_9C00
        mov    ebx, dword ptr [esp + 10h]
        and    ebx, 0ffff0000h
        jmp    SHORT loc_1_9C16
loc_1_9C00:
        mov    dl, byte ptr [esp + 13h]
        and    dl, 0e0h
        cmp    dl, 0c0h
        jne    SHORT loc_1_9C16
        mov    ebx, dword ptr [esp + 10h]
        and    ebx, 0ffffff00h
loc_1_9C16:
        cmp    ebx, dword ptr [esp + 14h]
        jne    SHORT loc_1_9C2F
        mov    edx, dword ptr [esp + 30h]
        shr    edx, 2
        cmp    edx, eax
        jbe    SHORT loc_1_9C2F
        mov    edx, dword ptr [esp + 10h]
loc_1_9C2B:
        mov    dword ptr [ecx + eax*4], edx
        inc    eax
loc_1_9C2F:
        add    edi, 6
        dec    dword ptr [esp + 18h]
        jne    NEAR PTR loc_1_9B82
loc_1_9C3C:
        mov    esi, dword ptr [esi]
        db  081h, 0FEh, 03Ch, 032h, 001h, 000h
        jne    NEAR PTR loc_1_9B44
loc_1_9C4A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
sub_1_9C54:
        sub    esp, 0f0h
        push   ebx
        push   esi
        push   edi
        mov    ecx, 6
        mov    edi, dword ptr [esp + 104h]
        mov    eax, edi
        sub    edx, edx
        div    ecx
        mov    esi, eax
        lea    eax, [esi + esi*2]
        add    eax, eax
        cmp    eax, edi
        jne    NEAR PTR loc_1_9D54
        push   1
        push   0f0h
        lea    eax, [esp + 14h]
        push   eax
        mov    edi, dword ptr [esp + 10ch]
        push   edi
        push   esi
        push   dword ptr [esp + 11ch]
        call   sub_1_9B24
        add    esp, 18h
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_9CE0
        push   1
        push   0f0h
        lea    eax, [esp + 14h]
        push   eax
        push   edi
        push   esi
        push   0
        call   sub_1_9B24
        add    esp, 18h
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_9CE0
        push   0
        push   0f0h
        lea    eax, [esp + 14h]
        push   eax
        push   edi
        push   esi
        push   0
        call   sub_1_9B24
        add    esp, 18h
        mov    ebx, eax
loc_1_9CE0:
        call   sub_1_000C
        or     ebx, ebx
        je     SHORT loc_1_9D08
        shr    eax, 4
        sub    edx, edx
        div    ebx
        mov    ecx, dword ptr [esp + edx*4 + 0ch]
        mov    eax, dword ptr [esp + 10ch]
        mov    dword ptr [eax], ecx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0f0h
        ret
        db  090h
loc_1_9D08:
        sub    edx, edx
        shr    eax, 4
        div    esi
        lea    eax, [edx + edx*2]
        mov    ecx, dword ptr [edi + eax*2 + 2]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        and    ecx, 0ffh
        or     edx, ecx
        mov    eax, dword ptr [esp + 10ch]
        mov    dword ptr [eax], edx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0f0h
        ret
        db  08Bh, 0FFh
loc_1_9D54:
        pop    edi
        mov    eax, 0c0000001h
        pop    esi
        pop    ebx
        add    esp, 0f0h
        ret
        db  090h
sub_1_9D64:
        db  08Bh, 00Dh, 03Ch, 032h, 001h, 000h
        mov    ax, word ptr [esp + 8]
        db  066h, 039h, 005h, 010h, 033h, 001h, 000h
        jne    SHORT loc_1_9D98
        db  081h, 0F9h, 03Ch, 032h, 001h, 000h
        je     SHORT loc_1_9D98
        mov    eax, dword ptr [esp + 4]
loc_1_9D84:
        cmp    dword ptr [ecx + 74h], eax
        je     SHORT loc_1_9D9C
        cmp    dword ptr [ecx + 78h], eax
        je     SHORT loc_1_9D9C
        mov    ecx, dword ptr [ecx]
        db  081h, 0F9h, 03Ch, 032h, 001h, 000h
        jne    SHORT loc_1_9D84
loc_1_9D98:
        sub    al, al
        ret
        db  090h
loc_1_9D9C:
        mov    al, 1
        ret
        db  090h
sub_1_9DA0:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bx, word ptr [esp + 54h]
        test   bl, 80h
        je     NEAR PTR loc_1_A05C
        mov    eax, dword ptr [esp + 48h]
        mov    ecx, dword ptr [eax + 0ah]
        mov    esi, ecx
        and    esi, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     esi, eax
        shl    esi, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     esi, eax
        shr    ecx, 18h
        and    ecx, 0ffh
        or     esi, ecx
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 58h]
        mov    edi, dword ptr [esp + 58h]
        lea    eax, [edi + 0ch]
        push   eax
        call   sub_1_10130
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_9E12
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_A05C
loc_1_9E12:
        lea    eax, [esp + 10h]
        push   eax
        push   0
        push   edi
        lea    eax, [esp + 2ch]
        push   eax
        call   sub_1_A0EC
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_9E5B
        cmp    eax, 0ffh
        je     SHORT loc_1_9E4C
        mov    byte ptr [esp + 2fh], 0
        lea    eax, [esp + 10h]
        push   eax
        push   0
        push   edi
        lea    eax, [esp + 2ch]
        push   eax
        call   sub_1_A0EC
        add    esp, 10h
loc_1_9E4C:
        or     eax, eax
        je     SHORT loc_1_9E5B
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_A05C
loc_1_9E5B:
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, dword ptr [eax + 14h]
        or     ebp, ebp
        je     SHORT loc_1_9E6C
        mov    word ptr [ebp + 3eh], 1
loc_1_9E6C:
        cmp    dword ptr [esp + 50h], 22h
        jl     NEAR PTR loc_1_A05C
        test   bh, 0fh
        jne    NEAR PTR loc_1_A05C
        cmp    byte ptr [edi + 7], 0
        je     NEAR PTR loc_1_A05C
        mov    eax, dword ptr [esp + 14h]
        lea    ebx, [eax + edi + 0dh]
        mov    ecx, eax
        cmp    byte ptr [edi + 5], 0
        je     SHORT loc_1_9EBE
        add    ebx, 4
        mov    ecx, eax
        add    ecx, 4
        cmp    byte ptr [ebx], 0c0h
        jne    SHORT loc_1_9EB0
        add    ebx, 2
        add    ecx, 2
        jmp    SHORT loc_1_9EBE
        db  090h
loc_1_9EB0:
        mov    eax, dword ptr [esp + 14h]
        inc    eax
        add    ebx, eax
        mov    eax, dword ptr [esp + 14h]
        inc    eax
        add    ecx, eax
loc_1_9EBE:
        mov    edi, ebx
        mov    ax, word ptr [ebx]
        mov    dx, ax
        and    edx, 0ffh
        shl    edx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     edx, eax
        cmp    edx, 5
        jne    SHORT loc_1_9F42
        add    ebx, 0ah
        mov    ax, word ptr [edi + 8]
        mov    dx, ax
        and    edx, 0ffh
        shl    edx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     edx, eax
        add    ebx, edx
        lea    ecx, [edx + ecx + 0ah]
        cmp    byte ptr [ebx], 0c0h
        jne    SHORT loc_1_9F10
        add    ebx, 2
        jmp    SHORT loc_1_9F40
        db  08Dh, 049h, 000h
loc_1_9F10:
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 34h]
        push   eax
        mov    eax, dword ptr [esp + 58h]
        sub    eax, ecx
        push   eax
        push   ebx
        call   sub_1_10130
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_9F39
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_A05C
loc_1_9F39:
        mov    eax, dword ptr [esp + 14h]
        inc    eax
        add    ebx, eax
loc_1_9F40:
        mov    edi, ebx
loc_1_9F42:
        or     ebp, ebp
        je     NEAR PTR loc_1_A05C
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   ebp
        call   sub_1_10AEC
        add    esp, 0ch
        cmp    dword ptr [esp + 18h], 0
        je     NEAR PTR loc_1_A05C
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        add    eax, 20h
        mov    ecx, dword ptr [eax]
        test   cl, 80h
        je     NEAR PTR loc_1_A02C
        and    ecx, 0ffffff0fh
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 10h]
        or     dword ptr [eax + 20h], 10h
        mov    ecx, dword ptr [edi + 0ah]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        and    ecx, 0ffh
        or     edx, ecx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 0ch], edx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 28h], 0ffffffffh
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [esp + 14h]
        call   sub_1_6608
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_A020
        cmp    eax, 0ffh
        je     SHORT loc_1_A020
        push   dword ptr [esp + 10h]
        call   sub_1_DEA4
        add    esp, 4
loc_1_A020:
        push   1
        push   0
        call   sub_1_FA40
        add    esp, 8
loc_1_A02C:
        mov    eax, dword ptr [esp + 44h]
        cmp    dword ptr [eax + 80h], esi
        jne    SHORT loc_1_A044
        mov    ecx, dword ptr [eax + 7ch]
        mov    dword ptr [eax + 80h], ecx
        mov    dword ptr [eax + 7ch], esi
loc_1_A044:
        push   eax
        db  068h, 000h, 032h, 000h, 000h
        push   dword ptr [esp + 20h]
        push   0
        push   dword ptr [esp + 2ch]
        call   sub_1_3C00
        add    esp, 14h
loc_1_A05C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
sub_1_A064:
        db  08Bh, 00Dh, 03Ch, 032h, 001h, 000h
        db  081h, 0F9h, 03Ch, 032h, 001h, 000h
        je     SHORT loc_1_A085
        mov    eax, dword ptr [esp + 4]
loc_1_A076:
        cmp    dword ptr [ecx + 58h], eax
        je     SHORT loc_1_A088
        mov    ecx, dword ptr [ecx]
        db  081h, 0F9h, 03Ch, 032h, 001h, 000h
        jne    SHORT loc_1_A076
loc_1_A085:
        sub    al, al
        ret
loc_1_A088:
        mov    al, 1
        ret
        db  090h
sub_1_A08C:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 74h]
        mov    edx, dword ptr [eax + 78h]
        mov    dword ptr [eax + 74h], edx
        mov    dword ptr [eax + 78h], ecx
        mov    cl, byte ptr [eax + 93h]
        not    cl
        mov    byte ptr [eax + 93h], cl
        ret
        db  090h
sub_1_A0AC:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        lea    edi, [esi + edx + 11h]
        mov    al, byte ptr [edi]
        and    al, 0c0h
        cmp    al, 0c0h
        mov    ecx, 10h
        je     SHORT loc_1_A0D5
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ecx, 0eh
loc_1_A0D5:
        lea    eax, [edx + ecx]
        mov    cl, byte ptr [eax + esi + 0dh]
        movzx  ax, cl
        mov    ah, al
        movsx  cx, cl
        mov    al, ch
        pop    edi
        pop    esi
        ret
        db  090h
sub_1_A0EC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0BFh, 07Ch, 032h, 001h, 000h
        mov    esi, dword ptr [esp + 14h]
        mov    ebp, dword ptr [esp + 18h]
        mov    bl, byte ptr [esp + 1ch]
loc_1_A101:
        mov    edi, dword ptr [edi]
        db  081h, 0FFh, 07Ch, 032h, 001h, 000h
        je     SHORT loc_1_A144
        or     bl, bl
        jne    SHORT loc_1_A137
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_A101
        mov    eax, dword ptr [eax + 20h]
        mov    cx, word ptr [ebp]
        cmp    word ptr [eax + 40h], cx
        jne    SHORT loc_1_A101
        push   10h
        push   esi
        lea    eax, [edi + 34h]
        push   eax
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, 10h
        jne    SHORT loc_1_A101
loc_1_A137:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_A144:
        pop    ebp
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh, 08Bh, 04Ch, 024h, 008h, 08Bh, 044h, 024h, 004h, 08Ah, 04Ch, 008h, 01Fh, 066h, 00Fh
        db  0BEh, 0C1h, 066h, 08Bh, 0D0h, 081h, 0E2h, 000h, 0FFh, 000h, 000h, 00Fh, 0BFh, 0C0h, 0C1h, 0E0h
        db  010h, 00Bh, 0D0h, 0C1h, 0E2h, 008h, 066h, 00Fh, 0BEh, 0C5h, 025h, 000h, 0FFh, 000h, 000h, 00Bh
        db  0D0h, 0C0h, 0F9h, 018h, 00Fh, 0B6h, 0C1h, 00Bh, 0D0h, 08Bh, 044h, 024h, 00Ch, 039h, 050h, 00Ch
        db  075h, 004h, 02Bh, 0C0h, 0C3h, 090h, 0B8h, 0BCh, 000h, 000h, 0C0h, 0C3h, 08Bh, 0FFh
sub_1_A19C:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 2ch], 0
        jne    SHORT loc_1_A1B2
        mov    dword ptr [esp + 2ch], 7f000000h
loc_1_A1B2:
        mov    esi, dword ptr [esp + 28h]
        mov    ax, word ptr [esi]
        mov    word ptr [esp + 12h], ax
        add    esi, 2
        cmp    byte ptr [esi], 2ah
        jne    SHORT loc_1_A1D1
        lea    edi, [esi + 1]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosw
        stosb
loc_1_A1D1:
        lea    eax, [esp + 14h]
        mov    dword ptr [esp + 14h], 0
        push   eax
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   esi
        db  0A1h, 080h, 02Fh, 001h, 000h
        push   dword ptr [eax + 80h]
        call   sub_1_6954
        add    esp, 10h
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_A20A
        cmp    ebx, 0ffh
        jne    NEAR PTR loc_1_A4EC
loc_1_A20A:
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [eax + 8]
        or     edi, edi
        je     NEAR PTR loc_1_A4EC
        cmp    dword ptr [esp + 2ch], 7f000000h
        je     SHORT loc_1_A230
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_1_A230
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_A230:
        inc    dword ptr [edi + 18h]
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax + 20h], 40h
        jne    SHORT loc_1_A242
        mov    ebx, 0c0000001h
loc_1_A242:
        or     ebx, ebx
        je     SHORT loc_1_A275
        cmp    ebx, 0ffh
        je     SHORT loc_1_A275
        db  080h, 03Dh, 050h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_A270
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 28h]
        mov    ecx, dword ptr [esp + 34h]
        test   dword ptr [ecx + 88h], eax
        jne    SHORT loc_1_A270
        sub    ebx, ebx
        jmp    SHORT loc_1_A275
        db  08Bh, 0FFh
loc_1_A270:
        mov    ebx, 0c0000001h
loc_1_A275:
        or     ebx, ebx
        je     SHORT loc_1_A2EC
        cmp    ebx, 0ffh
        je     SHORT loc_1_A2BC
        push   6
        mov    eax, dword ptr [esp + 38h]
        add    eax, 8ch
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        add    eax, 3eh
        push   eax
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, 6
        jne    SHORT loc_1_A2B0
        mov    eax, dword ptr [edi + 0ch]
        mov    eax, dword ptr [eax]
        sub    eax, edi
        cmp    eax, 0ch
        jne    SHORT loc_1_A2BC
        jmp    SHORT loc_1_A2BA
loc_1_A2B0:
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax + 20h], 40h
        je     SHORT loc_1_A2BC
loc_1_A2BA:
        sub    ebx, ebx
loc_1_A2BC:
        or     ebx, ebx
        je     SHORT loc_1_A2EC
        cmp    ebx, 0ffh
        je     SHORT loc_1_A2EC
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ecx], 0
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    ebx, 0c0000043h
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_A2EC:
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 20h]
        test   al, 40h
        je     NEAR PTR loc_1_A3B0
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     SHORT loc_1_A343
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0ch
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_A338
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 28h], 0
        push   0
        push   dword ptr [esp + 20h]
        call   dword ptr [esp + 20h]
        add    esp, 8
loc_1_A338:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 14h], 0
loc_1_A343:
        mov    eax, dword ptr [edi + 0ch]
        sub    eax, edi
        cmp    eax, 0ch
        jne    SHORT loc_1_A398
        mov    byte ptr [esp + 11h], 1
        mov    eax, dword ptr [esp + 14h]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [esp + 14h]
        or     dword ptr [eax + 20h], 80h
loc_1_A368:
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 8]
        call   sub_1_FB08
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_1_A404
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    ebx, 1
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_A398:
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    ebx, 0c00000bdh
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_A3B0:
        mov    byte ptr [esp + 11h], 0
        cmp    word ptr [esp + 12h], 0
        je     SHORT loc_1_A3E4
        cmp    word ptr [esp + 12h], 2
        je     SHORT loc_1_A3E4
        test   al, 4
        jne    SHORT loc_1_A368
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    ebx, 0c0000043h
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_A3E4:
        test   al, 2
        jne    NEAR PTR loc_1_A368
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    ebx, 0c0000043h
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_A404:
        mov    edx, dword ptr [esp + 38h]
        mov    dword ptr [eax + 0ch], edx
        mov    ecx, dword ptr [esp + 34h]
        mov    dword ptr [eax + 80h], ecx
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ecx], eax
        mov    ebx, dword ptr [esp + 14h]
        add    ebx, 28h
        mov    edx, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 34h]
        mov    ecx, dword ptr [ecx + 88h]
        test   ecx, edx
        jne    SHORT loc_1_A444
        or     edx, ecx
        mov    dword ptr [ebx], edx
        mov    ecx, dword ptr [esp + 14h]
        test   byte ptr [ecx + 20h], 10h
        je     SHORT loc_1_A448
        mov    cl, 1
        jmp    SHORT loc_1_A44A
loc_1_A444:
        mov    byte ptr [edi + 23h], 1
loc_1_A448:
        sub    cl, cl
loc_1_A44A:
        mov    edx, dword ptr [esp + 14h]
        test   byte ptr [edx + 20h], 10h
        je     SHORT loc_1_A470
        or     cl, cl
        jne    SHORT loc_1_A470
        sub    ebx, ebx
        mov    dword ptr [eax + 0ch], ebx
        mov    byte ptr [eax + 89h], bl
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_A470:
        or     cl, cl
        mov    edx, dword ptr [esp + 38h]
        mov    dword ptr [eax + 0ch], edx
        jne    SHORT loc_1_A498
        cmp    byte ptr [esp + 11h], 0
        jne    SHORT loc_1_A498
        mov    byte ptr [eax + 89h], 1
        mov    ebx, 0ffh
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_A498:
        mov    byte ptr [eax + 89h], 0
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 16h]
        db  068h, 018h, 0A6h, 000h, 000h
        push   eax
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   esi
        push   0
        push   0
        call   sub_1_E71C
        add    esp, 20h
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_A60D
        cmp    ebx, 0ffh
        je     NEAR PTR loc_1_A60D
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_A4EC:
        push   24h
        call   sub_1_001E
        add    esp, 4
        mov    ebp, eax
        mov    ebx, 1
        or     ebp, ebp
        je     NEAR PTR loc_1_A60D
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        mov    ecx, dword ptr [esp + 34h]
        mov    dword ptr [ebp + 1ch], ecx
        mov    dword ptr [ebp + 18h], ebx
        mov    byte ptr [ebp + 22h], 2
        cmp    word ptr [esp + 12h], ax
        je     SHORT loc_1_A531
        cmp    word ptr [esp + 12h], 2
        mov    word ptr [ebp + 20h], bx
        jne    SHORT loc_1_A537
loc_1_A531:
        mov    word ptr [ebp + 20h], 0
loc_1_A537:
        push   ebp
        lea    eax, [ebp + 0ch]
        mov    dword ptr [ebp + 10h], eax
        mov    dword ptr [eax], eax
        call   sub_1_FB08
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_A604
        mov    edx, dword ptr [esp + 38h]
        mov    dword ptr [edi + 0ch], edx
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ecx], edi
        db  0C7h, 045h, 000h, 074h, 032h, 001h, 000h
        db  0A1h, 078h, 032h, 001h, 000h
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        db  089h, 02Dh, 078h, 032h, 001h, 000h
        mov    dword ptr [ebp + 8], 12345678h
        mov    ecx, dword ptr [esp + 34h]
        mov    dword ptr [edi + 80h], ecx
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_A5AB
        cmp    dword ptr [esp + 2ch], 7f000000h
        je     SHORT loc_1_A5A3
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_A5A3:
        mov    dword ptr [esp + 2ch], 0
loc_1_A5AB:
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 16h]
        db  068h, 018h, 0A6h, 000h, 000h
        push   edi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   esi
        push   dword ptr [esp + 44h]
        push   0
        call   sub_1_E71C
        add    esp, 20h
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_A60D
        cmp    ebx, 0ffh
        je     SHORT loc_1_A60D
        push   edi
        call   sub_1_FAD8
        add    esp, 4
        mov    ecx, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   ebp
        call   sub_1_FA98
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_A604:
        push   ebp
        call   sub_1_FA98
        add    esp, 4
loc_1_A60D:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_A618:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        lea    eax, [esp + 24h]
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [esp + 30h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 30h]
        mov    eax, dword ptr [eax + 80h]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 1ch]
        inc    dword ptr [eax + 18h]
        mov    esi, dword ptr [eax + 14h]
        mov    edi, dword ptr [esp + 34h]
        or     edi, edi
        je     SHORT loc_1_A678
        mov    eax, dword ptr [esi + 20h]
        and    eax, 0ffffff0fh
        mov    dword ptr [esi + 20h], eax
        or     eax, 40h
        mov    dword ptr [esi + 20h], eax
        mov    edi, 0c00000bdh
        jmp    SHORT loc_1_A690
        db  08Bh, 0FFh
loc_1_A678:
        lea    eax, [esi + 20h]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esi + 20h]
        and    eax, 0ffffff0fh
        mov    dword ptr [esi + 20h], eax
        or     eax, 10h
        mov    dword ptr [esi + 20h], eax
loc_1_A690:
        inc    eax
        mov    eax, dword ptr [esp + 1ch]
        add    eax, 0ch
        mov    dword ptr [esp + 10h], eax
loc_1_A69C:
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [eax]
        cmp    ebx, eax
        je     NEAR PTR loc_1_A73D
loc_1_A6AA:
        mov    ebp, ebx
        mov    ebx, dword ptr [ebx]
        mov    ecx, dword ptr [ebp + 0ch]
        or     ecx, ecx
        je     SHORT loc_1_A733
        cmp    ebp, dword ptr [esp + 30h]
        je     SHORT loc_1_A6DD
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [ebp + 80h], eax
        jne    SHORT loc_1_A6D0
        cmp    byte ptr [ebp + 89h], 0
        jne    SHORT loc_1_A6DD
loc_1_A6D0:
        or     edi, edi
        je     SHORT loc_1_A733
        cmp    byte ptr [ebp + 89h], 0
        je     SHORT loc_1_A733
loc_1_A6DD:
        or     edi, edi
        je     SHORT loc_1_A701
        cmp    edi, 0ffh
        je     SHORT loc_1_A701
        mov    dword ptr [esi + 28h], 0
        mov    dword ptr [ebp + 10h], 0
        mov    edx, ebx
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
loc_1_A701:
        push   ebp
        mov    byte ptr [ebp + 89h], 0
        inc    dword ptr [esp + 18h]
        push   edi
        push   ecx
        call   sub_1_25A0
        add    esp, 0ch
        mov    dword ptr [ebp + 0ch], 0
        or     edi, edi
        je     SHORT loc_1_A733
        cmp    edi, 0ffh
        je     SHORT loc_1_A733
        push   ebp
        call   sub_1_FAD8
        add    esp, 4
loc_1_A733:
        cmp    ebx, dword ptr [esp + 10h]
        jne    NEAR PTR loc_1_A6AA
loc_1_A73D:
        or     edi, edi
        je     SHORT loc_1_A74D
        cmp    edi, 0ffh
        jne    NEAR PTR loc_1_A7D4
loc_1_A74D:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
loc_1_A754:
        cmp    eax, dword ptr [esp + 10h]
        je     NEAR PTR loc_1_A7F8
        mov    ebx, eax
        mov    eax, dword ptr [eax]
        cmp    byte ptr [ebx + 89h], 0
        je     SHORT loc_1_A754
        mov    byte ptr [ebx + 89h], 0
        mov    ebp, dword ptr [esi + 20h]
        test   ebp, 2
        mov    ax, 0
        jne    SHORT loc_1_A785
        mov    ax, 1
loc_1_A785:
        test   ebp, 1
        je     SHORT loc_1_A791
        or     ax, 2
loc_1_A791:
        push   dword ptr [ebx + 80h]
        push   eax
        db  068h, 018h, 0A6h, 000h, 000h
        push   ebx
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        lea    eax, [esi + 34h]
        push   eax
        push   0
        push   0
        call   sub_1_E71C
        add    esp, 20h
        mov    edi, eax
        mov    dword ptr [esi + 20h], ebp
        or     edi, edi
        je     SHORT loc_1_A7F8
        cmp    edi, 0ffh
        je     SHORT loc_1_A7F8
        mov    byte ptr [ebx + 89h], 1
        jmp    NEAR PTR loc_1_A69C
        db  08Dh, 049h, 000h
loc_1_A7D4:
        mov    eax, dword ptr [esp + 14h]
        dec    dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_A7F8
loc_1_A7E0:
        push   dword ptr [esp + 1ch]
        call   sub_1_DDE4
        add    esp, 4
        mov    eax, dword ptr [esp + 14h]
        dec    dword ptr [esp + 14h]
        or     eax, eax
        jne    SHORT loc_1_A7E0
loc_1_A7F8:
        push   dword ptr [esp + 1ch]
        call   sub_1_DDE4
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
sub_1_A810:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   71h
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_A830
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_A830:
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 1ch
        rep stosd
        stosb
        lea    eax, [esi + 14h]
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 1ch]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 24h], eax
        mov    dword ptr [esi + 68h], 0
        mov    dword ptr [esi + 8], 87654321h
        mov    dword ptr [esi + 64h], 1
        mov    byte ptr [esi + 6dh], 8
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], esi
        mov    edi, dword ptr [esp + 1ch]
        lea    ecx, [edi + 0ch]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [ecx], esi
        cmp    dword ptr [edi + 70h], 0
        je     SHORT loc_1_A8F0
        push   49h
        call   sub_1_001E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_A8D0
        push   edi
        push   ebp
        call   sub_1_A908
        add    esp, 8
        mov    ebx, eax
        mov    dword ptr [ebp + 40h], 0
        or     ebx, ebx
        je     SHORT loc_1_A8C8
        cmp    ebx, 0ffh
        je     SHORT loc_1_A8C8
        push   ebp
        call   sub_1_0024
        add    esp, 4
        jmp    SHORT loc_1_A8D5
        db  08Dh, 049h, 000h
loc_1_A8C8:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_A8D0:
        mov    ebx, 1
loc_1_A8D5:
        push   edi
        push   esi
        call   sub_1_A8F8
        add    esp, 8
        push   esi
        call   sub_1_FAF0
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_A8F0:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_A8F8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        ret
        db  090h
sub_1_A908:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   edi
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   sub_1_3630
        add    esp, 8
        mov    ebx, eax
        mov    dword ptr [esi + 14h], 0
        or     ebx, ebx
        je     SHORT loc_1_A932
        cmp    ebx, 0ffh
        jne    SHORT loc_1_A96F
loc_1_A932:
        push   dword ptr [edi + 70h]
        push   dword ptr [esi + 10h]
        call   sub_1_3690
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_A960
        cmp    ebx, 0ffh
        je     SHORT loc_1_A960
        push   esi
        call   sub_1_37F0
        add    esp, 4
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_A960:
        add    edi, 1ch
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [edi], esi
loc_1_A96F:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_A978:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 8], 87654321h
        jne    SHORT loc_1_A9F0
        mov    dword ptr [esp + 4], 0
        mov    edx, dword ptr [esp + 10h]
        cmp    dword ptr [edx + 8], 18273645h
        jne    SHORT loc_1_A9E4
        mov    dword ptr [esp + 4], 0
        cmp    dword ptr [esi + 68h], 0
        je     SHORT loc_1_A9B8
        mov    eax, 0c0000008h
        pop    esi
        add    esp, 4
        ret
loc_1_A9B8:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [esi + 68h], 1
        mov    dword ptr [esi + 10h], edx
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        add    edx, 14h
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi], edx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [edx + 4], esi
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
        db  090h
loc_1_A9E4:
        pop    esi
        mov    eax, 0c0000008h
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_A9F0:
        pop    esi
        mov    eax, 0c0000008h
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_A9FC:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 2ch]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 8], 87654321h
        jne    NEAR PTR loc_1_AAF4
        mov    dword ptr [esp + 14h], 0
        mov    edi, dword ptr [esi + 10h]
        mov    eax, 4
        mov    ecx, dword ptr [esi + 68h]
        dec    ecx
        cmp    ecx, 8
        ja     NEAR PTR loc_1_AAE8
        db  0FFh, 024h, 08Dh, 004h, 0ABh, 000h, 000h
        db  08Bh, 0FFh
loc_1_AA3C:
        mov    eax, 2
loc_1_AA41:
        push   0
        mov    dword ptr [esp + 1ch], esi
        push   0
        push   0
        push   eax
        db  068h, 068h, 033h, 001h, 000h
        lea    eax, [esp + 2ch]
        push   eax
        call   sub_1_BDC0
        add    esp, 18h
        mov    dword ptr [esp + 14h], eax
loc_1_AA62:
        or     edi, edi
        mov    eax, dword ptr [edi + 80h]
        mov    dword ptr [esp + 10h], eax
        je     SHORT loc_1_AAAB
        add    edi, 2ch
        mov    ebx, dword ptr [edi]
        cmp    ebx, edi
        je     SHORT loc_1_AAAB
loc_1_AA79:
        mov    ebp, ebx
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebp + 10h], esi
        jne    SHORT loc_1_AAA7
        mov    ecx, ebx
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0c0000120h
        push   dword ptr [ebp + 8]
        call   sub_1_25A0
        add    esp, 0ch
        push   ebp
        call   sub_1_0024
        add    esp, 4
loc_1_AAA7:
        cmp    ebx, edi
        jne    SHORT loc_1_AA79
loc_1_AAAB:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [esi + 4], esi
        mov    dword ptr [esi], esi
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], esi
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 68h], 0
        mov    dword ptr [esi + 10h], 0
        mov    byte ptr [esi + 6fh], 0
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, 0ch
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 4], esi
loc_1_AAE8:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_AAF4:
        pop    ebp
        mov    eax, 0c0000008h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h, 062h, 0AAh, 000h, 000h, 03Ch, 0AAh, 000h, 000h, 03Ch, 0AAh, 000h, 000h, 03Ch
        db  0AAh, 000h, 000h, 03Ch, 0AAh, 000h, 000h, 03Ch, 0AAh, 000h, 000h, 041h, 0AAh, 000h, 000h, 062h
        db  0AAh, 000h, 000h, 062h, 0AAh, 000h, 000h
sub_1_AB28:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 10h], 0
        jne    SHORT loc_1_AB40
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
        db  090h
loc_1_AB40:
        cmp    dword ptr [esi + 8], 18273645h
        jne    SHORT loc_1_AB80
        mov    dword ptr [esp + 4], 0
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 0ch], eax
        push   dword ptr [esi + 80h]
        push   esi
        call   sub_1_AB8C
        add    esp, 8
        mov    dword ptr [esp + 4], eax
        push   esi
        call   sub_1_DD58
        add    esp, 4
        mov    dword ptr [esp + 4], eax
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_AB80:
        pop    esi
        mov    eax, 0c0000008h
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_AB8C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 10h], 0
        jne    SHORT loc_1_ABA4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_ABA4:
        lea    ebx, [esi + 24h]
        mov    edi, dword ptr [ebx]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 24h
        je     SHORT loc_1_ABDF
        mov    dword ptr [esi + 28h], ebx
        mov    dword ptr [ebx], ebx
        cmp    edi, ebx
        je     SHORT loc_1_ABDF
loc_1_ABBC:
        mov    ebp, edi
        push   0
        push   0c00000cah
        push   dword ptr [edi + 8]
        call   sub_1_25A0
        add    esp, 0ch
        mov    edi, dword ptr [edi]
        push   ebp
        call   sub_1_0024
        add    esp, 4
        cmp    edi, ebx
        jne    SHORT loc_1_ABBC
loc_1_ABDF:
        mov    eax, dword ptr [esi + 80h]
        mov    dword ptr [esi + 8], 18273640h
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esi + 1ch]
        sub    eax, esi
        cmp    eax, 1ch
        je     SHORT loc_1_AC79
        mov    ebx, dword ptr [esp + 18h]
        add    ebx, 0ch
loc_1_AC01:
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [edi]
        lea    ebp, [edi + 4]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebp], edi
        mov    dword ptr [edi], edi
        mov    ecx, dword ptr [edi + 0ch]
        cmp    word ptr [ecx + 32h], 3
        jne    SHORT loc_1_AC3D
        cmp    byte ptr [ecx + 48h], 1
        jne    SHORT loc_1_AC3D
        mov    edx, dword ptr [ecx + 28h]
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        mov    byte ptr [ecx + 48h], 0
        lea    eax, [ecx + 28h]
        mov    dword ptr [ecx + 2ch], eax
        mov    dword ptr [eax], eax
loc_1_AC3D:
        push   dword ptr [esp + 18h]
        push   edi
        call   sub_1_ADBC
        add    esp, 8
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edi + 68h], 0
        mov    dword ptr [edi + 10h], 0
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi], ebx
        mov    dword ptr [ebp], eax
        mov    dword ptr [eax], edi
        mov    dword ptr [ebx + 4], edi
        mov    eax, dword ptr [esi + 1ch]
        sub    eax, esi
        cmp    eax, 1ch
        jne    SHORT loc_1_AC01
loc_1_AC79:
        lea    ebp, [esi + 14h]
        mov    edi, dword ptr [ebp]
        mov    dword ptr [ebp + 4], ebp
        mov    dword ptr [ebp], ebp
        cmp    edi, ebp
        je     SHORT loc_1_ACE9
        mov    ebx, dword ptr [esp + 18h]
        add    ebx, 0ch
loc_1_AC90:
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [eax + 1ch]
        mov    eax, ecx
        sub    eax, dword ptr [esp + 18h]
        cmp    eax, 1ch
        je     SHORT loc_1_ACB5
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        push   ecx
        call   sub_1_DF20
        add    esp, 4
loc_1_ACB5:
        mov    ecx, edi
        mov    eax, dword ptr [edi]
        mov    edi, eax
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ecx + 68h], 0
        mov    dword ptr [ecx + 8], 87654320h
        mov    dword ptr [ecx + 10h], 0
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebx + 4], ecx
        cmp    edi, ebp
        jne    SHORT loc_1_AC90
loc_1_ACE9:
        lea    ebx, [esi + 2ch]
        mov    ebp, dword ptr [ebx]
        mov    dword ptr [ebx + 4], ebx
        mov    dword ptr [ebx], ebx
        cmp    ebp, ebx
        je     SHORT loc_1_AD1C
loc_1_ACF8:
        mov    edi, ebp
        mov    ebp, dword ptr [ebp]
        push   0
        push   0c00000cah
        push   dword ptr [edi + 8]
        call   sub_1_25A0
        add    esp, 0ch
        push   edi
        call   sub_1_0024
        add    esp, 4
        cmp    ebp, ebx
        jne    SHORT loc_1_ACF8
loc_1_AD1C:
        add    esi, 3ch
        mov    ebx, dword ptr [esi]
        mov    dword ptr [esi + 4], esi
        mov    dword ptr [esi], esi
        cmp    ebx, esi
        je     SHORT loc_1_AD5F
loc_1_AD2A:
        mov    edi, ebx
        mov    ebx, dword ptr [ebx]
        push   1
        push   0c00000cah
        push   dword ptr [edi + 1ch]
        call   sub_1_25A0
        add    esp, 0ch
        db  0C7h, 007h, 09Ch, 032h, 001h, 000h
        db  0A1h, 0A0h, 032h, 001h, 000h
        mov    dword ptr [edi + 4], eax
        db  0A1h, 0A0h, 032h, 001h, 000h
        mov    dword ptr [eax], edi
        db  089h, 03Dh, 0A0h, 032h, 001h, 000h
        cmp    ebx, esi
        jne    SHORT loc_1_AD2A
loc_1_AD5F:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_AD68:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 8], 87654321h
        jne    SHORT loc_1_ADB0
        mov    dword ptr [esp + 4], 0
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_ADBC
        add    esp, 8
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 58h], eax
        push   esi
        call   sub_1_DEE4
        add    esp, 4
        mov    dword ptr [esp + 4], eax
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_ADB0:
        pop    esi
        mov    eax, 0c0000008h
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_ADBC:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        mov    bl, 1
        mov    esi, dword ptr [esp + 34h]
        cmp    dword ptr [esi + 68h], 0
        je     SHORT loc_1_AE52
        cmp    dword ptr [esi + 8], 87654321h
        jne    NEAR PTR loc_1_AE64
        mov    dword ptr [esp + 14h], 0
        mov    eax, dword ptr [esi + 68h]
        cmp    eax, 3
        je     SHORT loc_1_ADF9
        cmp    eax, 2
        jne    SHORT loc_1_AE1D
loc_1_ADF9:
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   dword ptr [esp + 40h]
        push   esi
        call   sub_1_AFAC
        add    esp, 10h
        mov    dword ptr [esp + 14h], eax
        cmp    eax, 0ffh
        je     SHORT loc_1_AE1D
        sub    bl, bl
loc_1_AE1D:
        or     bl, bl
        mov    dword ptr [esp + 20h], esi
        je     SHORT loc_1_AE43
        push   0
        push   0
        push   0
        push   2
        db  068h, 068h, 033h, 001h, 000h
        lea    eax, [esp + 34h]
        push   eax
        call   sub_1_BDC0
        add    esp, 18h
        mov    dword ptr [esp + 14h], eax
loc_1_AE43:
        cmp    dword ptr [esp + 14h], 0fh
        jne    SHORT loc_1_AE52
        mov    dword ptr [esp + 14h], 0
loc_1_AE52:
        cmp    dword ptr [esi + 8], 87654321h
        jne    SHORT loc_1_AE74
        mov    byte ptr [esp + 13h], 1
        jmp    SHORT loc_1_AE79
        db  08Bh, 0FFh
loc_1_AE64:
        pop    ebp
        mov    eax, 0c0000008h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_AE74:
        mov    byte ptr [esp + 13h], 0
loc_1_AE79:
        mov    dword ptr [esi + 8], 87654320h
        lea    eax, [esp + 18h]
        push   eax
        push   esi
        call   sub_1_AF30
        add    esp, 8
        mov    ebp, dword ptr [esi + 10h]
        or     ebp, ebp
        je     SHORT loc_1_AED2
        add    ebp, 2ch
        mov    edi, dword ptr [ebp]
        cmp    edi, ebp
        je     SHORT loc_1_AED2
        nop
loc_1_AEA0:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        cmp    dword ptr [ebx + 10h], esi
        jne    SHORT loc_1_AECE
        mov    ecx, edi
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0c0000120h
        push   dword ptr [ebx + 8]
        call   sub_1_25A0
        add    esp, 0ch
        push   ebx
        call   sub_1_0024
        add    esp, 4
loc_1_AECE:
        cmp    edi, ebp
        jne    SHORT loc_1_AEA0
loc_1_AED2:
        cmp    byte ptr [esi + 6ch], 0
        jne    SHORT loc_1_AF04
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_AF04
        mov    eax, dword ptr [esp + 38h]
        mov    edx, dword ptr [eax + 1ch]
        mov    eax, edx
        sub    eax, dword ptr [esp + 38h]
        cmp    eax, 1ch
        je     SHORT loc_1_AF04
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   edx
        call   sub_1_DF20
        add    esp, 4
loc_1_AF04:
        mov    ecx, dword ptr [esi]
        cmp    dword ptr [esi + 68h], 0
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        jle    SHORT loc_1_AF22
        mov    dword ptr [esi + 68h], 0
        mov    dword ptr [esi + 10h], 0
loc_1_AF22:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Bh, 0FFh
sub_1_AF30:
        push   esi
        mov    esi, dword ptr [esp + 8]
        add    esi, 1ch
        cmp    dword ptr [esi], esi
        je     SHORT loc_1_AF5E
loc_1_AF3C:
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   0
        push   0c0000120h
        push   dword ptr [eax + 1ch]
        call   sub_1_25A0
        add    esp, 0ch
        cmp    dword ptr [esi], esi
        jne    SHORT loc_1_AF3C
loc_1_AF5E:
        pop    esi
        ret
sub_1_AF60:
        push   esi
        mov    edx, 1
        mov    ecx, dword ptr [esp + 0ch]
loc_1_AF6A:
        db  08Bh, 035h, 06Ch, 033h, 001h, 000h
        db  081h, 0FEh, 06Ch, 033h, 001h, 000h
        je     SHORT loc_1_AF87
loc_1_AF78:
        cmp    dword ptr [esi + 8], ecx
        je     SHORT loc_1_AF98
        mov    esi, dword ptr [esi]
        db  081h, 0FEh, 06Ch, 033h, 001h, 000h
        jne    SHORT loc_1_AF78
loc_1_AF87:
        mov    eax, edx
        dec    edx
        or     eax, eax
        jne    SHORT loc_1_AF6A
        mov    eax, 0c0000001h
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_AF98:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    esi
        ret
sub_1_AFAC:
        sub    esp, 14h
        push   esi
        push   edi
        mov    edi, 0c0000001h
        mov    dword ptr [esp + 0ch], 0
        mov    edx, dword ptr [esp + 20h]
        mov    eax, dword ptr [edx + 0ch]
        mov    ecx, dword ptr [edx + 68h]
        cmp    ecx, 3
        jne    NEAR PTR loc_1_B0E0
        cmp    dword ptr [eax + 38h], 3
        jne    SHORT loc_1_AFE4
        mov    eax, 0ffh
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_AFE4:
        mov    esi, dword ptr [edx + 4ch]
        or     esi, esi
        je     NEAR PTR loc_1_B0EE
        mov    dword ptr [esp + 8], 0
        lea    eax, [esp + 8]
        push   eax
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        mov    eax, dword ptr [esi + 38h]
        add    eax, 34h
        push   eax
        db  0FFh, 035h, 0B8h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_B029
        cmp    edi, 0ffh
        jne    NEAR PTR loc_1_B0F2
loc_1_B029:
        mov    eax, dword ptr [esp + 8]
        test   byte ptr [eax + 20h], 10h
        je     SHORT loc_1_B040
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_B040:
        mov    eax, dword ptr [esp + 8]
        add    eax, 14h
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_1_B0A0
        mov    dword ptr [eax], 0
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0ch
        mov    edi, eax
        mov    eax, dword ptr [esp + 8]
        and    dword ptr [eax + 20h], 0ffffff7fh
        mov    eax, dword ptr [esp + 8]
        or     dword ptr [eax + 20h], 20h
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 10h], 0
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_B099
        push   dword ptr [esp + 8]
        call   sub_1_DEA4
        add    esp, 4
loc_1_B099:
        sub    esi, esi
        jmp    SHORT loc_1_B0F2
        db  08Dh, 049h, 000h
loc_1_B0A0:
        db  083h, 03Dh, 074h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_B0C8
        db  0A1h, 074h, 033h, 001h, 000h
        cmp    dword ptr [eax + 8], esi
        jne    SHORT loc_1_B0C8
        mov    dword ptr [esp + 0ch], eax
        db  0C7h, 005h, 074h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        sub    edi, edi
        jmp    SHORT loc_1_B0F2
        db  08Dh, 049h, 000h
loc_1_B0C8:
        lea    eax, [esp + 0ch]
        push   eax
        push   esi
        db  068h, 06Ch, 033h, 001h, 000h
        call   sub_1_AF60
        add    esp, 0ch
        mov    edi, eax
        jmp    SHORT loc_1_B0F2
        db  090h
loc_1_B0E0:
        cmp    ecx, 2
        jne    SHORT loc_1_B0EE
        mov    eax, dword ptr [edx + 4ch]
        mov    word ptr [eax + 4ch], 40h
loc_1_B0EE:
        mov    esi, dword ptr [esp + 18h]
loc_1_B0F2:
        or     edi, edi
        je     SHORT loc_1_B0FE
        cmp    edi, 0ffh
        jne    SHORT loc_1_B16D
loc_1_B0FE:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_B13F
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 14h], eax
        or     esi, esi
        je     SHORT loc_1_B133
        push   dword ptr [esi + 38h]
        call   sub_1_12CC0
        add    esp, 4
        push   esi
        call   sub_1_F280
        add    esp, 4
loc_1_B133:
        push   dword ptr [esp + 0ch]
        call   sub_1_0024
        add    esp, 4
loc_1_B13F:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_B168
        sub    edi, edi
        push   0c0000120h
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        call   sub_1_E64C
        add    esp, 0ch
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_B168:
        mov    edi, 0c0000001h
loc_1_B16D:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_B178:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 20h]
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [esi + 14h]
        mov    dword ptr [esi + 30h], 0
        test   byte ptr [esi + 4ch], 40h
        je     SHORT loc_1_B1D8
        mov    eax, dword ptr [edi + 54h]
        or     eax, eax
        je     SHORT loc_1_B1B3
        mov    dword ptr [edi + 54h], 0
        push   0
        push   0c0000120h
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
loc_1_B1B3:
        mov    eax, dword ptr [esi + 44h]
        or     eax, eax
        jne    SHORT loc_1_B1CC
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_B1CC:
        dec    eax
        mov    dword ptr [esi + 44h], eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_B1D8:
        mov    dword ptr [esp + 0ch], edi
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 68h], 1
        push   eax
        push   esi
        push   0
        push   0
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_B238
        add    esp, 14h
        mov    esi, eax
        push   ebx
        call   sub_1_0024
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_B22E
        cmp    esi, 0ffh
        je     SHORT loc_1_B22E
        cmp    dword ptr [edi + 54h], 0
        je     SHORT loc_1_B22E
        mov    dword ptr [edi + 54h], 0
        push   0
        push   0c00000bch
        push   0
        call   sub_1_25A0
        add    esp, 0ch
loc_1_B22E:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
sub_1_B238:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        mov    ecx, dword ptr [esp + 34h]
        mov    ebx, dword ptr [ecx]
        cmp    dword ptr [esp + 3ch], eax
        je     SHORT loc_1_B26D
        mov    ecx, dword ptr [ebx + 10h]
        mov    ecx, dword ptr [ecx + 80h]
        cmp    dword ptr [ecx + 70h], eax
        je     SHORT loc_1_B2A8
        cmp    dword ptr [ecx + 58h], eax
        je     SHORT loc_1_B2A8
        cmp    dword ptr [ebx + 8], 87654321h
        jne    SHORT loc_1_B298
        mov    dword ptr [esp + 14h], eax
loc_1_B26D:
        mov    edx, dword ptr [esp + 40h]
        cmp    dword ptr [esp + 3ch], 0
        jne    SHORT loc_1_B2D0
        mov    dword ptr [esp + 18h], edx
        test   byte ptr [edx + 4ch], 40h
        je     SHORT loc_1_B2D0
        mov    eax, dword ptr [edx + 44h]
        or     eax, eax
        jne    SHORT loc_1_B2B8
        push   5
        push   edx
        call   sub_1_D5C0
        add    esp, 8
        jmp    SHORT loc_1_B2BC
        db  08Bh, 0FFh
loc_1_B298:
        pop    ebp
        mov    eax, 0c0000008h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_B2A8:
        pop    ebp
        mov    eax, 0c00000beh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_B2B8:
        dec    eax
        mov    dword ptr [edx + 44h], eax
loc_1_B2BC:
        mov    dword ptr [esp + 14h], 0c0000120h
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_B2D0:
        mov    ecx, dword ptr [ebx + 68h]
        cmp    ecx, 1
        je     SHORT loc_1_B2F4
        cmp    ecx, 9
        je     SHORT loc_1_B2F4
        mov    dword ptr [esp + 14h], 0c0000010h
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_B2F4:
        mov    ebp, dword ptr [ebx + 10h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx + 14h]
        test   byte ptr [ecx + 20h], 40h
        je     SHORT loc_1_B318
        mov    dword ptr [esp + 14h], 0c00000bdh
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  090h
loc_1_B318:
        mov    ecx, dword ptr [ebp + 80h]
        mov    dword ptr [esp + 1ch], ecx
        cmp    dword ptr [ecx + 58h], 0
        jne    SHORT loc_1_B33C
        mov    dword ptr [esp + 14h], 0c00000beh
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_B33C:
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [ebx + 68h], 3
        add    dword ptr [ebx + 64h], 2
        mov    esi, dword ptr [ebx + 4]
        mov    dword ptr [esi], ecx
        mov    dword ptr [ecx + 4], esi
        lea    esi, [ebp + 1ch]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebx], esi
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ecx], ebx
        mov    dword ptr [esi + 4], ebx
        cmp    dword ptr [esp + 3ch], 0
        je     NEAR PTR loc_1_B4AC
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [ebx + 54h], eax
        mov    byte ptr [ebx + 6ch], 1
        mov    byte ptr [ebx + 6eh], 2
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax + 14h]
        call   sub_1_FF70
        add    esp, 0ch
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        je     SHORT loc_1_B3B4
        cmp    eax, 0ffh
        je     SHORT loc_1_B3B4
        mov    dword ptr [ebx + 68h], 1
        mov    dword ptr [esp + 14h], 0c00000beh
        jmp    SHORT loc_1_B3DF
loc_1_B3B4:
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        je     SHORT loc_1_B410
        cmp    eax, 0ffh
        je     SHORT loc_1_B410
        mov    dword ptr [ebx + 68h], 1
        mov    dword ptr [esp + 14h], 1
loc_1_B3DF:
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        add    ebp, 14h
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebx], ebp
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [eax], ebx
        mov    dword ptr [ebp + 4], ebx
        mov    eax, dword ptr [ebx + 64h]
        dec    eax
        mov    dword ptr [ebx + 64h], eax
        dec    eax
        mov    dword ptr [ebx + 64h], eax
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_B410:
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax + 20h], 0
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax + 44h], 1
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [eax + 2ch], ecx
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax + 14h], ebx
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax + 34h], 8bh
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 38h], eax
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [ecx + 3ch], eax
        db  066h, 0A1h, 0D4h, 032h, 001h, 000h
        lea    eax, [eax*2 + 46h]
        mov    word ptr [esp + 12h], ax
        mov    dword ptr [esp + 14h], 1
        and    eax, 0ffffh
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 1ch], eax
        lea    edi, [ebx + 28h]
        mov    esi, dword ptr [esp + 20h]
        movsd
        movsd
        movsd
        movsd
        jmp    SHORT loc_1_B4B3
        db  090h
loc_1_B4AC:
        mov    dword ptr [esp + 18h], edx
        inc    dword ptr [edx + 44h]
loc_1_B4B3:
        mov    dword ptr [ebx + 5ch], 0
        cmp    dword ptr [esp + 3ch], 0
        je     SHORT loc_1_B537
        or     eax, eax
        je     NEAR PTR loc_1_B70A
        cmp    dword ptr [esp + 3ch], 0
        je     SHORT loc_1_B537
        mov    byte ptr [eax], 81h
        mov    byte ptr [eax + 1], 0
        movzx  esi, word ptr [esp + 12h]
        lea    ecx, [esi - 4]
        shr    ecx, 8
        mov    dx, word ptr [esp + 12h]
        sub    dx, 4
        mov    ch, dl
        mov    word ptr [eax + 2], cx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 18h], esi
        db  00Fh, 0B7h, 00Dh, 0D4h, 032h, 001h, 000h
        push   ecx
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [esp + 28h]
        add    eax, 4
        push   eax
        call   sub_1_10064
        add    esp, 10h
        db  00Fh, 0B7h, 00Dh, 0D4h, 032h, 001h, 000h
        push   ecx
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx + 14h]
        add    ecx, 34h
        push   ecx
        push   eax
        call   sub_1_10064
        add    esp, 10h
loc_1_B537:
        push   0
        push   dword ptr [esp + 20h]
        push   ebx
        call   sub_1_36C0
        add    esp, 0ch
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        je     SHORT loc_1_B559
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_B70A
loc_1_B559:
        mov    eax, dword ptr [ebx + 0ch]
        mov    byte ptr [eax + 45h], 1
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [eax + 38h], 1
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 60h], eax
        cmp    dword ptr [esp + 3ch], 0
        je     SHORT loc_1_B5A0
        mov    ecx, dword ptr [esp + 1ch]
        mov    edx, dword ptr [ecx + 1ch]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, 1ch
        je     SHORT loc_1_B5B2
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   edx
        call   sub_1_DF20
        add    esp, 4
        jmp    SHORT loc_1_B5B2
        db  08Dh, 049h, 000h
loc_1_B5A0:
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [esp + 20h], eax
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 38h], eax
loc_1_B5B2:
        lea    esi, [ebx + 4ch]
        lea    eax, [esp + 24h]
        push   eax
        push   1
        push   esi
        db  068h, 08Ch, 0B7h, 000h, 000h
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        call   sub_1_C224
        add    esp, 1ch
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        jne    SHORT loc_1_B5FC
        mov    eax, dword ptr [esp + 24h]
        cmp    dword ptr [eax + 24h], 0facf0000h
        jne    SHORT loc_1_B5F0
        mov    ecx, dword ptr [eax + 0ch]
        jmp    SHORT loc_1_B600
loc_1_B5F0:
        mov    ecx, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [ecx + 58h]
        jmp    SHORT loc_1_B600
        db  08Dh, 049h, 000h
loc_1_B5FC:
        mov    ecx, dword ptr [esp + 2ch]
loc_1_B600:
        cmp    dword ptr [esp + 3ch], 0
        jne    SHORT loc_1_B612
        cmp    dword ptr [esp + 44h], 0
        je     SHORT loc_1_B612
        mov    ecx, dword ptr [esp + 44h]
loc_1_B612:
        cmp    dword ptr [esp + 14h], 0
        jne    NEAR PTR loc_1_B700
        mov    eax, dword ptr [esp + 18h]
        test   byte ptr [eax + 4ch], 40h
        je     SHORT loc_1_B640
        push   dword ptr [esp + 24h]
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [esp + 14h], 0c0000120h
        jmp    NEAR PTR loc_1_B70A
loc_1_B640:
        mov    eax, dword ptr [esp + 24h]
        test   byte ptr [eax + 20h], 2
        jne    SHORT loc_1_B66C
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        jne    SHORT loc_1_B66C
        push   eax
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [esp + 14h], 0c00000beh
        jmp    NEAR PTR loc_1_B70A
        db  08Dh, 049h, 000h
loc_1_B66C:
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [eax + 68h], 3
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [eax + 44h], 0
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [eax + 48h], 0
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        mov    eax, ecx
        shr    eax, 18h
        and    eax, 0ffh
        or     edx, eax
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [eax + 34h], edx
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [eax + 38h], 3
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [esp + 18h]
        push   dword ptr [eax + 34h]
        db  068h, 0E8h, 0B9h, 000h, 000h
        push   dword ptr [eax + 2ch]
        push   ecx
        push   dword ptr [esp + 28h]
        call   sub_1_116C0
        add    esp, 14h
        mov    dword ptr [esp + 14h], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Bh, 0FFh
loc_1_B700:
        cmp    dword ptr [esp + 14h], 0ffh
        je     SHORT loc_1_B780
loc_1_B70A:
        push   ebx
        mov    esi, dword ptr [ebx + 0ch]
        mov    dword ptr [ebx + 0ch], 0
        mov    dword ptr [ebx + 68h], 1
        call   sub_1_BCD4
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_B754
        mov    dword ptr [esi + 0ch], 0
        push   esi
        push   ebx
        call   sub_1_D61C
        add    esp, 8
        inc    dword ptr [esi + 3ch]
        push   dword ptr [esi + 18h]
        db  068h, 030h, 079h, 000h, 000h
        push   0
        push   esi
        push   0
        call   sub_1_3C00
        add    esp, 14h
        jmp    SHORT loc_1_B75D
        db  090h
loc_1_B754:
        push   ebx
        call   sub_1_DEE4
        add    esp, 4
loc_1_B75D:
        push   5
        push   dword ptr [esp + 1ch]
        call   sub_1_D5C0
        add    esp, 8
        push   ebx
        call   sub_1_DEE4
        add    esp, 4
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_B780:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_B78C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esi + 14h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [esp + 20h]
        or     edi, edi
        jne    NEAR PTR loc_1_B879
        test   byte ptr [esi + 4ch], 40h
        jne    NEAR PTR loc_1_B879
        mov    dword ptr [ebp + 4ch], 0
        lea    eax, [esp + 14h]
        push   eax
        push   esi
        call   sub_1_C4AC
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_B874
        cmp    dword ptr [eax + 24h], 0facf0000h
        jne    SHORT loc_1_B7E0
        mov    edx, dword ptr [eax + 0ch]
        jmp    SHORT loc_1_B7EC
        db  08Bh, 0FFh
loc_1_B7E0:
        mov    ecx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx + 80h]
        mov    edx, dword ptr [ecx + 58h]
loc_1_B7EC:
        test   byte ptr [esp + 14h], 2
        jne    SHORT loc_1_B7FC
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        je     SHORT loc_1_B874
loc_1_B7FC:
        mov    edi, edx
        mov    dword ptr [ebx + 38h], 3
        mov    dword ptr [ebp + 68h], 3
        mov    dword ptr [ebp + 44h], 0
        mov    dword ptr [ebp + 48h], 0
        mov    dword ptr [esi + 38h], eax
        and    edi, 0ff00h
        mov    ecx, edx
        shl    ecx, 10h
        or     edi, ecx
        shl    edi, 8
        mov    ecx, edx
        and    ecx, 0ff0000h
        shr    ecx, 8
        or     edi, ecx
        mov    ecx, edx
        shr    ecx, 18h
        and    ecx, 0ffh
        or     edi, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 34h], edi
        mov    dword ptr [ebp + 4ch], 0
        inc    dword ptr [eax + 1ch]
        push   dword ptr [esi + 34h]
        db  068h, 0E8h, 0B9h, 000h, 000h
        push   dword ptr [esi + 2ch]
        push   edx
        push   esi
        call   sub_1_116C0
        add    esp, 14h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_B874:
        mov    edi, 0c00000beh
loc_1_B879:
        mov    eax, dword ptr [ebp + 54h]
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [ebp + 54h], 0
        mov    bx, word ptr [esi + 4ch]
        and    bl, 40h
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        cmp    dword ptr [ebp + 68h], 0
        je     SHORT loc_1_B8A6
        mov    dword ptr [ebp + 68h], 1
loc_1_B8A6:
        sub    esi, esi
        or     bl, bl
        je     SHORT loc_1_B8B4
        mov    esi, dword ptr [ebp + 5ch]
        mov    edi, 0c0000120h
loc_1_B8B4:
        push   ebp
        call   sub_1_B908
        add    esp, 4
        cmp    dword ptr [ebp + 68h], 0
        je     SHORT loc_1_B8CC
        push   ebp
        call   sub_1_BCD4
        add    esp, 4
loc_1_B8CC:
        push   ebp
        call   sub_1_DEE4
        add    esp, 4
        cmp    edi, 15h
        jne    SHORT loc_1_B8DF
        mov    edi, 0c00000beh
loc_1_B8DF:
        push   0
        push   edi
        push   dword ptr [esp + 18h]
        call   sub_1_25A0
        add    esp, 0ch
        or     esi, esi
        je     SHORT loc_1_B8FF
        push   0
        push   0
        push   esi
        call   sub_1_25A0
        add    esp, 0ch
loc_1_B8FF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
sub_1_B908:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     SHORT loc_1_B96D
        mov    esi, dword ptr [ecx + 0ch]
        or     esi, esi
        je     SHORT loc_1_B96D
        mov    eax, dword ptr [esi + 38h]
        or     eax, eax
        jle    SHORT loc_1_B96D
        cmp    eax, 8
        jge    SHORT loc_1_B96D
        mov    dword ptr [esi + 38h], 8
        cmp    dword ptr [ecx + 68h], 0
        je     SHORT loc_1_B938
        mov    dword ptr [ecx + 68h], 9
loc_1_B938:
        cmp    eax, 6
        mov    dword ptr [ecx + 0ch], 0
        mov    dword ptr [esi + 0ch], 0
        jae    SHORT loc_1_B94E
        inc    dword ptr [esi + 3ch]
loc_1_B94E:
        push   esi
        push   ecx
        call   sub_1_D61C
        add    esp, 8
        push   dword ptr [esi + 18h]
        db  068h, 030h, 079h, 000h, 000h
        push   0
        push   esi
        push   0
        call   sub_1_3C00
        add    esp, 14h
loc_1_B96D:
        pop    esi
        ret
        db  090h
sub_1_B970:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 3ch]
        or     eax, eax
        je     SHORT loc_1_B988
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 4], eax
        jmp    SHORT loc_1_B990
        db  090h
loc_1_B988:
        mov    dword ptr [esp + 4], 0
loc_1_B990:
        cmp    dword ptr [esp + 4], 2710h
        jae    SHORT loc_1_B9A2
        mov    dword ptr [esp + 4], 2710h
loc_1_B9A2:
        lea    eax, [esp + 8]
        push   eax
        push   0
        db  068h, 058h, 0BCh, 000h, 000h
        push   esi
        db  068h, 058h, 0BCh, 000h, 000h
        push   0
        push   esi
        push   dword ptr [esp + 20h]
        call   sub_1_109B8
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_B9DC
        cmp    eax, 0ffh
        je     SHORT loc_1_B9DC
        mov    dword ptr [esi + 30h], 0
        pop    esi
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_B9DC:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [esi + 30h], ecx
        pop    esi
        add    esp, 8
        ret
loc_1_B9E8:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 2ch]
        mov    edi, dword ptr [esi + 14h]
        push   dword ptr [esi + 38h]
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, dword ptr [edi + 0ch]
        test   byte ptr [esi + 4ch], 40h
        mov    ebp, dword ptr [esp + 30h]
        jne    SHORT loc_1_BA89
        or     ebp, ebp
        je     SHORT loc_1_BA1A
        cmp    ebp, 0ffh
        jne    SHORT loc_1_BA89
loc_1_BA1A:
        or     eax, eax
        mov    dword ptr [edi + 68h], 6
        je     SHORT loc_1_BA84
        mov    dword ptr [eax + 38h], 6
        inc    dword ptr [eax + 3ch]
        mov    word ptr [eax + 32h], 0
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 18h], eax
        db  0C7h, 044h, 024h, 01Ch, 044h, 0BBh, 000h, 000h
        mov    dword ptr [esp + 20h], esi
        push   edi
        push   esi
        call   sub_1_B970
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_BA60
        cmp    ebp, 0ffh
        jne    SHORT loc_1_BA89
loc_1_BA60:
        add    esi, 18h
        push   0
        push   esi
        lea    eax, [esp + 1ch]
        push   eax
        push   dword ptr [esi]
        push   0
        lea    eax, [esp + 2ch]
        push   eax
        call   sub_1_3A60
        add    esp, 18h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_1_BA84:
        mov    ebp, 0c0000001h
loc_1_BA89:
        mov    eax, dword ptr [edi + 54h]
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [edi + 54h], 0
        mov    bx, word ptr [esi + 4ch]
        and    bl, 40h
        mov    eax, dword ptr [esi + 38h]
        cmp    dword ptr [eax + 24h], 0facf0000h
        jne    SHORT loc_1_BABF
        cmp    dword ptr [eax + 1ch], 1
        jne    SHORT loc_1_BABF
        test   byte ptr [eax + 20h], 10h
        je     SHORT loc_1_BABF
        push   eax
        call   sub_1_DEA4
        add    esp, 4
loc_1_BABF:
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        cmp    dword ptr [edi + 68h], 0
        je     SHORT loc_1_BAD7
        mov    dword ptr [edi + 68h], 1
loc_1_BAD7:
        sub    esi, esi
        or     bl, bl
        je     SHORT loc_1_BAE5
        mov    esi, dword ptr [edi + 5ch]
        mov    ebp, 0c0000120h
loc_1_BAE5:
        cmp    ebp, 0eh
        je     SHORT loc_1_BAEF
        cmp    ebp, 15h
        jne    SHORT loc_1_BAF4
loc_1_BAEF:
        mov    ebp, 0c00000beh
loc_1_BAF4:
        push   edi
        call   sub_1_B908
        add    esp, 4
        cmp    dword ptr [edi + 68h], 0
        je     SHORT loc_1_BB0C
        push   edi
        call   sub_1_BCD4
        add    esp, 4
loc_1_BB0C:
        push   edi
        call   sub_1_DEE4
        add    esp, 4
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_BB2B
        push   0
        push   ebp
        push   dword ptr [esp + 18h]
        call   sub_1_25A0
        add    esp, 0ch
loc_1_BB2B:
        or     esi, esi
        je     SHORT loc_1_BB3C
        push   0
        push   0
        push   esi
        call   sub_1_25A0
        add    esp, 0ch
loc_1_BB3C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_1_BB44:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esi + 14h]
        mov    ebx, dword ptr [esp + 1ch]
        or     ebx, ebx
        je     NEAR PTR loc_1_BC2C
        cmp    ebx, 0ffh
        je     NEAR PTR loc_1_BC2C
        mov    eax, dword ptr [esi + 30h]
        or     eax, eax
        je     SHORT loc_1_BB8C
        push   0
        lea    ecx, [esp + 14h]
        push   ecx
        push   eax
        call   sub_1_10AEC
        add    esp, 0ch
        mov    dword ptr [esi + 30h], 0
        jmp    SHORT loc_1_BB94
        db  08Bh, 0FFh
loc_1_BB8C:
        mov    dword ptr [esp + 10h], 0
loc_1_BB94:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_BC04
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        mov    dword ptr [edi + 60h], 0
        mov    esi, dword ptr [edi + 68h]
        mov    dword ptr [edi + 68h], 1
        mov    ebp, dword ptr [edi + 54h]
        mov    dword ptr [edi + 54h], 0
        mov    dword ptr [edi + 5ch], 0
        cmp    esi, 6
        jne    SHORT loc_1_BBD6
        push   edi
        call   sub_1_B908
        add    esp, 4
loc_1_BBD6:
        or     esi, esi
        je     SHORT loc_1_BBE3
        push   edi
        call   sub_1_BCD4
        add    esp, 4
loc_1_BBE3:
        push   edi
        call   sub_1_DEE4
        add    esp, 4
        or     ebp, ebp
        je     SHORT loc_1_BC4D
        push   0
        push   ebx
        push   ebp
        call   sub_1_25A0
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_BC04:
        cmp    dword ptr [edi + 60h], esi
        jne    SHORT loc_1_BC1B
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        mov    dword ptr [edi + 60h], 0
loc_1_BC1B:
        push   edi
        call   sub_1_DEE4
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_BC2C:
        mov    eax, dword ptr [esi + 44h]
        or     eax, eax
        jne    SHORT loc_1_BC40
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        jmp    SHORT loc_1_BC44
loc_1_BC40:
        dec    eax
        mov    dword ptr [esi + 44h], eax
loc_1_BC44:
        push   edi
        call   sub_1_DEE4
        add    esp, 4
loc_1_BC4D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_BC58:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_BCCD
        mov    eax, dword ptr [esp + 10h]
        mov    edi, dword ptr [eax + 14h]
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 30h], 0
        or     ecx, ecx
        je     SHORT loc_1_BCCD
        cmp    dword ptr [edi + 68h], 6
        jne    SHORT loc_1_BCCD
        mov    ebx, dword ptr [edi + 54h]
        or     ebx, ebx
        je     SHORT loc_1_BCCD
        mov    dword ptr [edi + 54h], 0
        mov    si, word ptr [edi + 68h]
        mov    dword ptr [edi + 68h], 1
        push   edi
        call   sub_1_B908
        add    esp, 4
        or     si, si
        je     SHORT loc_1_BCAD
        push   edi
        call   sub_1_BCD4
        add    esp, 4
loc_1_BCAD:
        mov    eax, 0c0000120h
        cmp    dword ptr [esp + 14h], 0c0000120h
        je     SHORT loc_1_BCC1
        mov    eax, 15h
loc_1_BCC1:
        push   0
        push   eax
        push   ebx
        call   sub_1_25A0
        add    esp, 0ch
loc_1_BCCD:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_BCD4:
        mov    edx, dword ptr [esp + 4]
        cmp    dword ptr [edx + 68h], 0
        je     SHORT loc_1_BD02
        mov    dword ptr [edx + 68h], 1
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edx + 10h]
        add    ecx, 14h
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
loc_1_BD02:
        ret
        db  090h, 02Bh, 0C0h, 0C3h, 090h
sub_1_BD08:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    ebx, dword ptr [edi]
        cmp    dword ptr [ebx + 8], 87654321h
        jne    NEAR PTR loc_1_BDB4
        mov    dword ptr [esp + 0ch], 0
        cmp    dword ptr [ebx + 68h], 1
        je     SHORT loc_1_BD3C
        mov    eax, 0c0000008h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_BD3C:
        mov    esi, dword ptr [ebx + 10h]
        push   24h
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_BD5C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_BD5C:
        mov    dword ptr [eax + 10h], ebx
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 18h], ecx
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [eax + 1ch], ecx
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [eax + 20h], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax + 0ch], ecx
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [eax + 14h], ecx
        add    esi, 2ch
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esp + 0ch], 0
        mov    dword ptr [esp + 0ch], 0ffh
        mov    eax, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_BDB4:
        pop    edi
        mov    eax, 0c0000008h
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_BDC0:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    byte ptr [esp + 13h], 0
        mov    eax, dword ptr [esp + 28h]
        mov    esi, dword ptr [eax]
        mov    eax, dword ptr [esi + 68h]
        cmp    eax, 1
        jle    NEAR PTR loc_1_C034
        cmp    eax, 8
        jge    NEAR PTR loc_1_C034
        mov    ebx, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 5ch], edi
        cmp    dword ptr [esp + 30h], 1
        jne    SHORT loc_1_BE20
        cmp    eax, 7
        jne    SHORT loc_1_BE10
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [esi + 58h], eax
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
loc_1_BE10:
        pop    ebp
        mov    eax, 0fh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_BE20:
        or     ebx, ebx
        je     NEAR PTR loc_1_BFE0
        mov    eax, dword ptr [esi + 68h]
        cmp    eax, 1
        jle    NEAR PTR loc_1_BF90
        mov    edi, dword ptr [esi + 60h]
        cmp    eax, 2
        je     NEAR PTR loc_1_BECC
        cmp    eax, 3
        je     NEAR PTR loc_1_BEEC
        cmp    eax, 6
        je     NEAR PTR loc_1_BF40
loc_1_BE52:
        cmp    dword ptr [esi + 68h], 7
        je     SHORT loc_1_BE60
        mov    dword ptr [esp + 30h], 2
loc_1_BE60:
        mov    dword ptr [esi + 68h], 1
        mov    dword ptr [esi + 0ch], 0
        mov    dword ptr [ebx + 0ch], 0
        mov    edi, dword ptr [ebx + 38h]
        mov    ax, word ptr [ebx + 32h]
        movzx  ebp, ax
        cmp    ax, 3
        jne    SHORT loc_1_BEA1
        cmp    byte ptr [ebx + 48h], 1
        jne    SHORT loc_1_BEA1
        lea    edx, [ebx + 28h]
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [ebx + 2ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    byte ptr [ebx + 48h], 0
        mov    dword ptr [ebx + 2ch], edx
        mov    dword ptr [edx], edx
loc_1_BEA1:
        mov    dword ptr [ebx + 38h], 8
        cmp    dword ptr [esi + 5ch], 0
        jne    SHORT loc_1_BEB5
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [ebx + 40h], eax
loc_1_BEB5:
        push   ebx
        push   esi
        call   sub_1_D61C
        add    esp, 8
        mov    byte ptr [esp + 13h], 1
        jmp    NEAR PTR loc_1_BFA0
        db  08Dh, 049h, 000h
loc_1_BECC:
        lea    eax, [esp + 14h]
        or     word ptr [edi + 4ch], 40h
        push   eax
        push   esi
        call   sub_1_AF30
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_BEEC:
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [ebx + 18h]
        push   esi
        call   sub_1_AFAC
        add    esp, 10h
        cmp    eax, 0c0000001h
        jne    SHORT loc_1_BF24
        mov    word ptr [edi + 4ch], 40h
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [esi + 5ch], eax
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
loc_1_BF24:
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_C081
        mov    word ptr [edi + 4ch], 40h
        inc    dword ptr [ebx + 3ch]
        jmp    NEAR PTR loc_1_BE52
        db  08Dh, 049h, 000h
loc_1_BF40:
        mov    esi, dword ptr [edi + 30h]
        or     esi, esi
        je     SHORT loc_1_BF80
        mov    dword ptr [edi + 30h], 0
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   esi
        call   sub_1_10AEC
        add    esp, 0ch
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_BF76
        push   esi
        push   0
        push   dword ptr [esp + 24h]
        call   dword ptr [esp + 24h]
        add    esp, 0ch
loc_1_BF76:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_BF80:
        sub    eax, eax
        or     word ptr [edi + 4ch], 40h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
loc_1_BF90:
        sub    ebp, ebp
        mov    dword ptr [ebx + 0ch], 0
        mov    dword ptr [esi + 0ch], 0
loc_1_BFA0:
        cmp    ebp, 2
        jne    SHORT loc_1_BFD2
        mov    eax, dword ptr [esi + 60h]
        or     eax, eax
        je     SHORT loc_1_BFCA
        mov    dword ptr [esi + 60h], 0
        push   0
        push   0c0000120h
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        mov    dword ptr [esi + 60h], 0
loc_1_BFCA:
        mov    dword ptr [esp + 30h], 2
loc_1_BFD2:
        cmp    dword ptr [esi + 40h], 0
        je     SHORT loc_1_BFE0
        mov    dword ptr [esp + 30h], 2
loc_1_BFE0:
        lea    eax, [esp + 14h]
        push   eax
        push   esi
        call   sub_1_AF30
        add    esp, 8
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_BFFE
        push   esi
        call   sub_1_BCD4
        add    esp, 4
loc_1_BFFE:
        mov    al, 1
        cmp    dword ptr [esp + 3ch], 0
        je     SHORT loc_1_C009
        sub    al, al
loc_1_C009:
        push   eax
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 38h]
        push   edi
        push   ebx
        call   sub_1_C08C
        add    esp, 14h
        cmp    dword ptr [esi + 5ch], 0
        je     SHORT loc_1_C081
        cmp    eax, 1
        je     SHORT loc_1_C081
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_C034:
        mov    al, byte ptr [esi + 6fh]
        or     al, al
        je     SHORT loc_1_C07C
        cmp    dword ptr [esp + 30h], 1
        jne    SHORT loc_1_C06C
        cmp    al, 2
        jne    SHORT loc_1_C058
        mov    eax, 14h
        mov    byte ptr [esi + 6fh], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
loc_1_C058:
        pop    ebp
        mov    eax, 16h
        mov    byte ptr [esi + 6fh], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_C06C:
        sub    eax, eax
        mov    byte ptr [esi + 6fh], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_C07C:
        mov    eax, 0fh
loc_1_C081:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
sub_1_C08C:
        sub    esp, 4
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [esp + 10h]
        cmp    esi, edi
        je     SHORT loc_1_C107
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, 7
        ja     SHORT loc_1_C107
        cmp    eax, 3
        jb     SHORT loc_1_C107
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_1_C0D0
        cmp    eax, 0ffh
        je     SHORT loc_1_C0D0
        mov    edi, 1
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_C0D0:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 14h], esi
        push   dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 1ch]
        push   esi
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 14h]
        call   sub_1_118E4
        add    esp, 10h
        mov    edi, eax
        cmp    esi, 2
        jne    SHORT loc_1_C107
        push   0
        push   0
        push   dword ptr [esp + 10h]
        call   sub_1_1192C
        add    esp, 0ch
loc_1_C107:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  090h
sub_1_C110:
        sub    esp, 4
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax + 8], 87654321h
        jne    SHORT loc_1_C188
        mov    dword ptr [esp], 0
        cmp    dword ptr [eax + 68h], 5
        jne    SHORT loc_1_C178
        mov    dword ptr [eax + 68h], 7
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [eax + 38h], 7
        mov    word ptr [eax + 32h], 0
        push   0
        push   82h
        push   eax
        call   sub_1_11788
        add    esp, 0ch
        mov    dword ptr [esp], eax
        or     eax, eax
        je     SHORT loc_1_C166
        cmp    eax, 0ffh
        jne    SHORT loc_1_C180
loc_1_C166:
        mov    dword ptr [esp], 0
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_C178:
        mov    dword ptr [esp], 0c0000001h
loc_1_C180:
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret
loc_1_C188:
        mov    eax, 0c0000008h
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_C194:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 8], 18273645h
        jne    SHORT loc_1_C218
        mov    dword ptr [esp + 4], 0
        push   1ch
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_C1C8
        mov    eax, 1
        pop    esi
        add    esp, 4
        ret
        db  090h
loc_1_C1C8:
        mov    ecx, dword ptr [esp + 28h]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 14h], ecx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 18h], ecx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [eax + 0ch], ecx
        add    esi, 24h
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp + 4], 0ffh
        mov    eax, 0ffh
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_C218:
        pop    esi
        mov    eax, 0c0000008h
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_C224:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 2ch]
        or     ebx, ebx
        je     SHORT loc_1_C239
        mov    dword ptr [ebx], 0
loc_1_C239:
        mov    ebp, dword ptr [esp + 20h]
        cmp    byte ptr [ebp], 2ah
        je     NEAR PTR loc_1_C2E4
        cmp    byte ptr [ebp + 0fh], 1dh
        jne    SHORT loc_1_C258
        cmp    byte ptr [esp + 30h], 0
        jne    NEAR PTR loc_1_C2E4
loc_1_C258:
        mov    esi, 0c0000001h
        mov    dword ptr [esp + 10h], 0
        cmp    byte ptr [ebp + 0fh], 1ch
        je     SHORT loc_1_C287
        lea    eax, [esp + 10h]
        push   eax
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   ebp
        db  0FFh, 035h, 0B4h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        mov    esi, eax
loc_1_C287:
        or     esi, esi
        je     SHORT loc_1_C293
        cmp    esi, 0ffh
        jne    SHORT loc_1_C2BE
loc_1_C293:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 20h]
        test   al, 0c0h
        jne    SHORT loc_1_C2BE
        test   al, 0ch
        je     NEAR PTR loc_1_C357
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        jne    NEAR PTR loc_1_C357
        cmp    byte ptr [esp + 30h], 0
        jne    NEAR PTR loc_1_C357
loc_1_C2BE:
        lea    eax, [esp + 10h]
        mov    dword ptr [esp + 10h], 0
        push   eax
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   ebp
        db  0FFh, 035h, 0B8h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        mov    esi, eax
        jmp    SHORT loc_1_C357
loc_1_C2E4:
        push   44h
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     NEAR PTR loc_1_C49C
        mov    edi, eax
        add    edi, 34h
        mov    esi, ebp
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [esp + 24h]
        mov    eax, dword ptr [eax + 60h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 20h], 14h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 24h], 0dead0000h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        sub    esi, esi
loc_1_C357:
        or     esi, esi
        je     SHORT loc_1_C367
        cmp    esi, 0ffh
        jne    NEAR PTR loc_1_C430
loc_1_C367:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [eax + 20h]
        test   dl, 20h
        jne    NEAR PTR loc_1_C430
        or     esi, esi
        je     SHORT loc_1_C383
        cmp    esi, 0ffh
        jne    SHORT loc_1_C39A
loc_1_C383:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 28h]
        mov    eax, dword ptr [esp + 24h]
        test   dword ptr [eax + 88h], ecx
        je     NEAR PTR loc_1_C430
loc_1_C39A:
        test   dl, 10h
        je     SHORT loc_1_C3F4
        mov    eax, dword ptr [esp + 10h]
        inc    dword ptr [eax + 1ch]
        db  066h, 0A1h, 01Ch, 033h, 001h, 000h
        mov    ecx, dword ptr [esp + 10h]
        mov    word ptr [ecx + 30h], ax
        mov    edi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [edi + 38h]
        cmp    byte ptr [eax + 0fh], 1ch
        jne    SHORT loc_1_C3D9
        db  068h, 08Ch, 034h, 001h, 000h
        push   eax
        call   sub_1_12108
        add    esp, 8
        mov    dword ptr [edi + 14h], eax
        or     eax, eax
        je     SHORT loc_1_C3D9
        inc    dword ptr [eax + 1ch]
loc_1_C3D9:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edi + 38h], eax
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [eax], ecx
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_C3F4:
        test   dl, 80h
        je     SHORT loc_1_C420
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 1ch]
        push   ebp
        push   dword ptr [esp + 20h]
        call   sub_1_C50C
        add    esp, 14h
        mov    esi, eax
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_C420:
        pop    ebp
        mov    esi, 0c00000c4h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_C430:
        or     esi, esi
        je     SHORT loc_1_C43C
        cmp    esi, 0ffh
        jne    SHORT loc_1_C455
loc_1_C43C:
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [eax + 20h], 20h
        je     SHORT loc_1_C455
        cmp    dword ptr [eax + 1ch], 1
        jne    SHORT loc_1_C455
        push   eax
        call   sub_1_DEA4
        add    esp, 4
loc_1_C455:
        mov    edi, dword ptr [esp + 1ch]
        lea    eax, [edi + 8]
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [eax], eax
        lea    eax, [esp + 14h]
        push   eax
        push   ebx
        push   dword ptr [esp + 2ch]
        push   0
        db  066h, 0A1h, 064h, 033h, 001h, 000h
        and    eax, 0fh
        push   eax
        push   dword ptr [esp + 3ch]
        push   edi
        push   0
        push   0
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   ebp
        call   sub_1_E088
        add    esp, 2ch
        mov    esi, eax
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_C49C:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
sub_1_C4AC:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esp + 10h]
        push   edi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [esi + 38h]
        push   1
        call   sub_1_DFA0
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_C4E8
        push   edi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [esi + 38h]
        push   0
        call   sub_1_DFA0
        add    esp, 10h
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_C4E8:
        mov    ecx, dword ptr [esi + 2ch]
        mov    ecx, dword ptr [ecx + 88h]
        test   dword ptr [eax + 28h], ecx
        je     SHORT loc_1_C504
        db  066h, 08Bh, 00Dh, 01Ch, 033h, 001h, 000h
        mov    word ptr [eax + 30h], cx
        pop    edi
        pop    esi
        ret
loc_1_C504:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_C50C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        cmp    byte ptr [esi + 32h], 0
        je     SHORT loc_1_C56C
        push   0
        push   0
        push   dword ptr [esi + 14h]
        call   sub_1_10AEC
        add    esp, 0ch
        mov    dword ptr [esi + 14h], 0
        mov    dword ptr [esi + 20h], 20h
        lea    eax, [esp + 14h]
        push   eax
        push   0
        mov    edi, dword ptr [esp + 20h]
        push   dword ptr [edi + 2ch]
        push   esi
        db  066h, 0A1h, 064h, 033h, 001h, 000h
        and    eax, 0fh
        push   eax
        push   dword ptr [esp + 30h]
        push   edi
        push   0
        push   0
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [esp + 38h]
        call   sub_1_E088
        add    esp, 2ch
        pop    edi
        pop    esi
        ret
loc_1_C56C:
        mov    edx, dword ptr [esi + 10h]
        mov    edi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 48h], eax
        add    edx, 8
        mov    ecx, dword ptr [edx + 4]
        lea    eax, [edi + 8]
        mov    dword ptr [eax], edx
        mov    dword ptr [edi + 0ch], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [edx + 4], eax
        mov    eax, 0ffh
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_C598:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 1ch]
        cmp    dword ptr [edi + 58h], 0
        je     NEAR PTR loc_1_C6C4
        cmp    dword ptr [edi + 6ch], 0
        je     NEAR PTR loc_1_C6C4
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [eax]
        cmp    dword ptr [ebx + 8], 18273645h
        jne    NEAR PTR loc_1_C6B8
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_1_FF70
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_1_C600
        cmp    eax, 0ffh
        je     SHORT loc_1_C600
        mov    eax, 0c000000dh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_C600:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp - 0ch]
        push   ebx
        push   dword ptr [esi + 14h]
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        call   sub_1_C6D0
        add    esp, 20h
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_1_C634
        cmp    eax, 0ffh
        je     SHORT loc_1_C634
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_C634:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 2ch], edi
        lea    eax, [ebp - 10h]
        push   eax
        push   1
        push   0
        db  068h, 030h, 0C9h, 000h, 000h
        push   edi
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        call   sub_1_C224
        add    esp, 1ch
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    SHORT loc_1_C66B
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 10h]
        call   sub_1_C9D8
        add    esp, 8
loc_1_C66B:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_C698
        cmp    dword ptr [ebp - 4], 0ffh
        je     SHORT loc_1_C6A1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 38h], 0
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        call   sub_1_CCDC
        add    esp, 0ch
        jmp    SHORT loc_1_C6A8
        db  08Bh, 0FFh
loc_1_C698:
        cmp    dword ptr [ebp - 4], 0ffh
        jne    SHORT loc_1_C6A8
loc_1_C6A1:
        mov    dword ptr [ebp - 4], 0
loc_1_C6A8:
        pop    edi
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_C6B8:
        pop    edi
        mov    eax, 0c0000008h
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_C6C4:
        pop    edi
        mov    eax, 0c0000010h
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_C6D0:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  00Fh, 0B7h, 005h, 0D4h, 032h, 001h, 000h
        lea    ecx, [eax*2 + 50h]
        mov    dword ptr [esp + 1ch], ecx
        mov    ebp, ecx
        add    ebp, 3
        and    ebp, 0fffffffch
        mov    ebx, dword ptr [esp + 24h]
        lea    ecx, [ebp + ebx + 10h]
        mov    dword ptr [esp + 14h], ecx
        push   ecx
        lea    eax, [esp + 14h]
        push   eax
        call   sub_1_C8D0
        add    esp, 8
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_C720
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_C720:
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [eax], ecx
        db  066h, 08Bh, 00Dh, 00Ch, 033h, 001h, 000h
        shr    cx, 0ah
        or     cl, 2
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 1], cl
        db  08Ah, 02Dh, 00Eh, 033h, 001h, 000h
        db  08Ah, 00Dh, 00Fh, 033h, 001h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 2], cx
        call   sub_1_C8B4
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 8], 8a00h
        db  066h, 0A1h, 0D4h, 032h, 001h, 000h
        add    ax, ax
        movzx  ecx, ax
        movzx  eax, bx
        add    ecx, eax
        add    ecx, 42h
        shr    ecx, 8
        db  066h, 0A1h, 0D4h, 032h, 001h, 000h
        lea    eax, [ebx + eax*2 + 42h]
        mov    ch, al
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 0ah], cx
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [esp + 28h]
        mov    ecx, dword ptr [eax + 58h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        and    ecx, 0ffh
        or     edx, ecx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, 0eh
        db  00Fh, 0B7h, 005h, 0D4h, 032h, 001h, 000h
        push   eax
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        mov    eax, dword ptr [esp + 38h]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 14h]
        add    eax, 34h
        push   eax
        push   ecx
        call   sub_1_10064
        add    esp, 10h
        db  00Fh, 0B7h, 00Dh, 0D4h, 032h, 001h, 000h
        push   ecx
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        mov    esi, dword ptr [esp + 3ch]
        push   esi
        push   eax
        call   sub_1_10064
        add    esp, 10h
        mov    eax, dword ptr [esp + 10h]
        add    eax, ebp
        mov    dword ptr [esp + 18h], eax
        mov    edi, eax
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [esp + 10h]
        lea    ebp, [ebp + eax + 10h]
        or     ebx, ebx
        je     SHORT loc_1_C848
        mov    edi, ebp
        mov    esi, dword ptr [esp + 38h]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_1_C84A
loc_1_C848:
        sub    ebp, ebp
loc_1_C84A:
        call   sub_1_FE74
        or     eax, eax
        je     SHORT loc_1_C894
        mov    dword ptr [eax + 24h], ebp
        mov    dword ptr [eax + 20h], ebx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 1ch], ecx
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [eax + 14h], 0
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 38h], ecx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 44h], ecx
        mov    dword ptr [eax + 14h], 0
        mov    ecx, dword ptr [esp + 40h]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_C894:
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        call   sub_1_C904
        add    esp, 8
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
sub_1_C8B4:
        db  066h, 0FFh, 005h, 00Eh, 033h, 001h, 000h
        db  066h, 081h, 03Dh, 00Eh, 033h, 001h, 000h, 0FFh, 0FFh
        jne    SHORT loc_1_C8CF
        db  066h, 0C7h, 005h, 00Eh, 033h, 001h, 000h, 000h, 000h
loc_1_C8CF:
        ret
sub_1_C8D0:
        db  0A1h, 0A4h, 033h, 001h, 000h
        db  03Bh, 005h, 01Fh, 033h, 001h, 000h
        jbe    SHORT loc_1_C8E8
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], 0
        ret
loc_1_C8E8:
        mov    eax, dword ptr [esp + 8]
        db  001h, 005h, 0A4h, 033h, 001h, 000h
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        ret
        db  08Bh, 0FFh
sub_1_C904:
        mov    eax, dword ptr [esp + 8]
        db  03Bh, 005h, 0A4h, 033h, 001h, 000h
        ja     SHORT loc_1_C918
        db  029h, 005h, 0A4h, 033h, 001h, 000h
        jmp    SHORT loc_1_C922
loc_1_C918:
        db  0C7h, 005h, 0A4h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_C922:
        push   dword ptr [esp + 4]
        call   sub_1_0024
        add    esp, 4
        ret
        db  090h
loc_1_C930:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 18h]
        cmp    esi, ebx
        mov    edi, dword ptr [esp + 14h]
        je     SHORT loc_1_C94D
        mov    eax, dword ptr [edi + 38h]
        cmp    byte ptr [eax + 0fh], 1ch
        jne    SHORT loc_1_C9B0
loc_1_C94D:
        lea    eax, [esp + 0ch]
        push   eax
        push   edi
        call   sub_1_C4AC
        add    esp, 8
        mov    ebx, eax
        mov    eax, dword ptr [edi + 38h]
        cmp    byte ptr [eax + 0fh], 1ch
        jne    SHORT loc_1_C984
        db  068h, 08Ch, 034h, 001h, 000h
        push   eax
        call   sub_1_12108
        add    esp, 8
        mov    dword ptr [edi + 14h], eax
        or     ebx, ebx
        jne    SHORT loc_1_C988
        mov    ebx, eax
        mov    dword ptr [edi + 14h], 0
loc_1_C984:
        or     ebx, ebx
        je     SHORT loc_1_C9B4
loc_1_C988:
        inc    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_C995
        inc    dword ptr [eax + 1ch]
loc_1_C995:
        push   edi
        mov    dword ptr [edi + 38h], ebx
        push   ebx
        call   sub_1_C9D8
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_C9D1
        cmp    esi, 0ffh
        je     SHORT loc_1_C9D1
loc_1_C9B0:
        or     ebx, ebx
        jne    SHORT loc_1_C9BB
loc_1_C9B4:
        mov    dword ptr [edi + 38h], 0
loc_1_C9BB:
        cmp    esi, 15h
        jne    SHORT loc_1_C9C5
        mov    esi, 0c00000beh
loc_1_C9C5:
        push   0
        push   esi
        push   edi
        call   sub_1_CCDC
        add    esp, 0ch
loc_1_C9D1:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_C9D8:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edx + 20h]
        mov    edi, esi
        and    edi, 2
        je     SHORT loc_1_C9F4
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [ecx + 1ch]
        mov    byte ptr [eax], 10h
        jmp    SHORT loc_1_CA0B
loc_1_C9F4:
        cmp    byte ptr [edx + 34h], 2ah
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [ecx + 1ch]
        jne    SHORT loc_1_CA08
        mov    byte ptr [eax], 12h
        jmp    SHORT loc_1_CA0B
        db  08Bh, 0FFh
loc_1_CA08:
        mov    byte ptr [eax], 11h
loc_1_CA0B:
        test   esi, 8
        je     SHORT loc_1_CA20
        push   edx
        push   ecx
        call   sub_1_CC18
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_1_CA20:
        cmp    dword ptr [edx + 24h], 0facf0000h
        je     SHORT loc_1_CA38
        or     edi, edi
        je     SHORT loc_1_CA38
        mov    eax, dword ptr [ecx + 2ch]
        mov    edx, dword ptr [eax + 58h]
        jmp    SHORT loc_1_CA4E
        db  08Dh, 049h, 000h
loc_1_CA38:
        mov    edx, dword ptr [edx + 0ch]
        test   esi, 4
        je     SHORT loc_1_CA4E
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        je     SHORT loc_1_CA4E
        sub    edx, edx
loc_1_CA4E:
        mov    eax, dword ptr [ecx + 2ch]
        mov    word ptr [ecx + 34h], 0
        cmp    dword ptr [eax + 58h], 0
        je     SHORT loc_1_CA64
        mov    eax, dword ptr [eax + 6ch]
        jmp    SHORT loc_1_CA66
        db  08Bh, 0FFh
loc_1_CA64:
        sub    eax, eax
loc_1_CA66:
        push   20h
        push   8ah
        push   ecx
        db  068h, 0CCh, 0CAh, 000h, 000h
        push   eax
        push   edx
        push   ecx
        call   sub_1_115D4
        add    esp, 1ch
        mov    eax, 0ffh
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_CA88:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [ecx + 2ch]
        cmp    dword ptr [eax + 58h], 0
        je     SHORT loc_1_CAA4
        mov    eax, dword ptr [eax + 6ch]
        jmp    SHORT loc_1_CAA6
        db  08Dh, 049h, 000h
loc_1_CAA4:
        sub    eax, eax
loc_1_CAA6:
        push   20h
        push   8ah
        push   ecx
        db  068h, 0CCh, 0CAh, 000h, 000h
        push   eax
        push   edx
        push   ecx
        call   sub_1_115D4
        add    esp, 1ch
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_CACC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    dx, word ptr [esi + 34h]
        dec    dword ptr [esi + 40h]
        or     dx, dx
        je     SHORT loc_1_CB2C
        cmp    dx, 0fffdh
        jae    SHORT loc_1_CB1C
        movzx  ecx, dx
        mov    eax, dword ptr [esi + 38h]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + ecx*4]
        inc    dx
        mov    word ptr [esi + 34h], dx
        cmp    eax, -1
        je     SHORT loc_1_CB14
        inc    dword ptr [esi + 40h]
        push   dword ptr [esi + 2ch]
        db  068h, 088h, 0CAh, 000h, 000h
        push   0
        push   eax
        push   esi
        call   sub_1_3C00
        add    esp, 14h
        pop    esi
        ret
loc_1_CB14:
        mov    word ptr [esi + 34h], 0fffdh
        pop    esi
        ret
loc_1_CB1C:
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_1_CB60
        mov    word ptr [esi + 34h], 0fffeh
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_CB2C:
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_1_CB60
        mov    eax, dword ptr [esi + 38h]
        cmp    byte ptr [eax + 43h], 1ch
        jne    SHORT loc_1_CB60
        push   eax
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [esi + 14h], 0
        mov    dword ptr [esi + 38h], eax
        push   eax
        push   esi
        call   sub_1_CC18
        add    esp, 8
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_CB60:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_CCDC
        add    esp, 0ch
        pop    esi
        ret
        db  090h
loc_1_CB74:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        cmp    word ptr [edi + 34h], 0fffeh
        jne    SHORT loc_1_CBF8
        cmp    dword ptr [edi + 40h], 0
        jne    SHORT loc_1_CBE8
        mov    ebx, dword ptr [edi + 14h]
        or     ebx, ebx
        je     SHORT loc_1_CBD4
        mov    dword ptr [edi + 14h], 0
        mov    eax, dword ptr [ebx + 0ch]
        mov    esi, dword ptr [eax]
        push   dword ptr [edi + 38h]
        mov    word ptr [edi + 34h], 1
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [edi + 38h], ebx
        inc    dword ptr [edi + 40h]
        push   dword ptr [edi + 2ch]
        db  068h, 088h, 0CAh, 000h, 000h
        push   0
        push   esi
        push   edi
        call   sub_1_3C00
        add    esp, 14h
        mov    eax, dword ptr [esp + 18h]
        or     word ptr [eax + 3ch], 1
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_CBD4:
        push   0
        push   0
        push   edi
        call   sub_1_CCDC
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_CBE8:
        mov    eax, dword ptr [esp + 18h]
        or     word ptr [eax + 3ch], 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_CBF8:
        inc    dword ptr [edi + 40h]
        push   0
        push   0
        push   edi
        call   sub_1_CACC
        add    esp, 0ch
        mov    eax, dword ptr [esp + 18h]
        or     word ptr [eax + 3ch], 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_CC18:
        push   esi
        push   edi
        mov    edi, 0c0000001h
        sub    edx, edx
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 38h]
        inc    ecx
        mov    ecx, dword ptr [eax + 0ch]
loc_1_CC2C:
        mov    eax, edx
        inc    edx
        cmp    dword ptr [ecx + eax*4], -1
        jne    SHORT loc_1_CC2C
        mov    word ptr [esi + 34h], dx
        cmp    dword ptr [esi + 44h], 0
        mov    eax, 1
        je     SHORT loc_1_CC46
        sub    eax, eax
loc_1_CC46:
        mov    ecx, dword ptr [ecx + eax*4]
        cmp    ecx, -1
        je     SHORT loc_1_CCBB
        mov    word ptr [esi + 34h], 1
        mov    dword ptr [esi + 40h], 1
        mov    eax, dword ptr [esi + 2ch]
        cmp    dword ptr [eax + 58h], 0
        je     SHORT loc_1_CC6C
        mov    eax, dword ptr [eax + 6ch]
        jmp    SHORT loc_1_CC6E
        db  08Dh, 049h, 000h
loc_1_CC6C:
        sub    eax, eax
loc_1_CC6E:
        push   20h
        push   8ah
        push   esi
        db  068h, 0CCh, 0CAh, 000h, 000h
        push   eax
        push   ecx
        push   esi
        call   sub_1_115D4
        add    esp, 1ch
        mov    edi, eax
        push   0
        push   dword ptr [esp + 14h]
        push   1
        db  068h, 074h, 0CBh, 000h, 000h
        push   esi
        db  068h, 074h, 0CBh, 000h, 000h
        push   esi
        push   1f4h
        call   sub_1_1094C
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_CCBB
        cmp    eax, 0ffh
        je     SHORT loc_1_CCBB
        mov    dword ptr [esi + 30h], 0
loc_1_CCBB:
        or     edi, edi
        je     SHORT loc_1_CCD7
        cmp    edi, 0ffh
        je     SHORT loc_1_CCD7
        dec    dword ptr [esi + 40h]
        push   0
        push   0
        push   esi
        call   sub_1_CCDC
        add    esp, 0ch
loc_1_CCD7:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_1_CCDC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 38h]
        or     eax, eax
        je     SHORT loc_1_CCF1
        push   eax
        call   sub_1_DEA4
        add    esp, 4
loc_1_CCF1:
        push   dword ptr [esi + 44h]
        push   dword ptr [esi + 1ch]
        call   sub_1_C904
        add    esp, 8
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_CD0C:
        sub    esp, 4
        mov    edx, dword ptr [esp + 8]
        cmp    dword ptr [edx + 8], 18273645h
        jne    NEAR PTR loc_1_CE64
        mov    dword ptr [esp], 0
        cmp    dword ptr [edx + 10h], 0
        jne    SHORT loc_1_CD38
        mov    eax, 0c0000001h
        add    esp, 4
        ret
        db  090h
loc_1_CD38:
        mov    ecx, dword ptr [esp + 10h]
        or     ecx, ecx
        mov    eax, dword ptr [esp + 0ch]
        je     NEAR PTR loc_1_CDD0
        cmp    eax, 6
        ja     NEAR PTR loc_1_CE0C
        db  0FFh, 024h, 085h, 070h, 0CEh, 000h, 000h
loc_1_CD58:
        mov    dword ptr [edx + 48h], ecx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 4ch], eax
        sub    eax, eax
        add    esp, 4
        ret
loc_1_CD68:
        mov    dword ptr [edx + 58h], ecx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 5ch], eax
        mov    dword ptr [edx + 60h], ecx
        mov    dword ptr [edx + 64h], eax
        sub    eax, eax
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_CD80:
        mov    dword ptr [edx + 50h], ecx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 54h], eax
        sub    eax, eax
        add    esp, 4
        ret
loc_1_CD90:
        mov    dword ptr [edx + 68h], ecx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 6ch], eax
        sub    eax, eax
        add    esp, 4
        ret
loc_1_CDA0:
        mov    dword ptr [edx + 70h], ecx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 74h], eax
        sub    eax, eax
        add    esp, 4
        ret
loc_1_CDB0:
        mov    dword ptr [edx + 78h], ecx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 7ch], eax
        sub    eax, eax
        add    esp, 4
        ret
loc_1_CDC0:
        mov    dword ptr [edx + 60h], ecx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 64h], eax
        sub    eax, eax
        add    esp, 4
        ret
loc_1_CDD0:
        cmp    eax, 6
        ja     SHORT loc_1_CE0C
        db  0FFh, 024h, 085h, 08Ch, 0CEh, 000h, 000h
loc_1_CDDC:
        sub    eax, eax
        mov    dword ptr [edx + 48h], 0
        mov    dword ptr [edx + 4ch], 0
        add    esp, 4
        ret
loc_1_CDF0:
        mov    dword ptr [edx + 58h], 0
        mov    dword ptr [edx + 5ch], 0
loc_1_CDFE:
        mov    dword ptr [edx + 60h], 0
        mov    dword ptr [edx + 64h], 0
loc_1_CE0C:
        sub    eax, eax
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_CE14:
        sub    eax, eax
        mov    dword ptr [edx + 50h], 0
        mov    dword ptr [edx + 54h], 0
        add    esp, 4
        ret
loc_1_CE28:
        sub    eax, eax
        mov    dword ptr [edx + 68h], 0
        mov    dword ptr [edx + 6ch], 0
        add    esp, 4
        ret
loc_1_CE3C:
        sub    eax, eax
        mov    dword ptr [edx + 70h], 0
        mov    dword ptr [edx + 74h], 0
        add    esp, 4
        ret
loc_1_CE50:
        sub    eax, eax
        mov    dword ptr [edx + 78h], 0
        mov    dword ptr [edx + 7ch], 0
        add    esp, 4
        ret
loc_1_CE64:
        mov    eax, 0c0000008h
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_CE70:
        pop    eax
        int    0
        db  000h, 068h, 0CDh
        add    byte ptr [eax], al
        ror    ch, 0
        db  000h, 080h, 0CDh, 000h, 000h, 090h
        int    0
        db  000h, 0A0h, 0CDh, 000h, 000h, 0B0h
        int    0
        db  000h, 0DCh
        int    0
        db  000h, 0F0h
        int    0
        db  000h, 0FEh
        int    0
        db  000h, 014h, 0CEh
        add    byte ptr [eax], al
        sub    dh, cl
        add    byte ptr [eax], al
        cmp    al, 0ceh
        add    byte ptr [eax], al
        push   eax
        into
        add    byte ptr [eax], al
sub_1_CEA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   esi
        push   edi
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 14h]
        call   sub_1_FF70
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_CED5
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_D0F8
loc_1_CED5:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_D0F8
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_CEFA
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_D0FD
loc_1_CEFA:
        push   dword ptr [ebp - 4]
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        push   0
        push   0
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_1_10E60
        add    esp, 20h
        or     eax, eax
        jne    SHORT loc_1_CF38
        push   4
        push   dword ptr [ebp - 4]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, 1
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_CF38:
        mov    eax, dword ptr [ebp - 0ch]
        and    word ptr [eax + 2], 0fffeh
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [eax + ecx - 3], 21h
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 34h], 2
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 38h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 2ch], esi
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 000h, 06Ch, 032h, 001h, 000h
        db  0A1h, 070h, 032h, 001h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 070h, 032h, 001h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 070h, 032h, 001h, 000h
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        db  068h, 098h, 0D2h, 000h, 000h
        push   esi
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 4]
        call   sub_1_C224
        add    esp, 1ch
        mov    edi, eax
        or     edi, edi
        jne    NEAR PTR loc_1_D0D4
        mov    ecx, dword ptr [ebp + 14h]
        or     ecx, ecx
        je     SHORT loc_1_CFFF
        mov    eax, dword ptr [ebp - 10h]
        cmp    byte ptr [eax], 2ah
        jne    SHORT loc_1_CFFF
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_CFFF
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0ch], ecx
loc_1_CFFF:
        push   0
        push   dword ptr [ebp - 8]
        db  066h, 0A1h, 0D0h, 032h, 001h, 000h
        push   eax
        db  068h, 090h, 0D3h, 000h, 000h
        push   dword ptr [ebp - 4]
        db  068h, 0A8h, 0ECh, 000h, 000h
        push   dword ptr [ebp - 4]
        db  0FFh, 035h, 0CCh, 032h, 001h, 000h
        call   sub_1_1094C
        add    esp, 20h
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_D058
        cmp    edi, 0ffh
        je     SHORT loc_1_D058
        push   dword ptr [ebp - 8]
        call   sub_1_DE94
        add    esp, 4
        push   7
        push   dword ptr [ebp - 4]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_D058:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 24h], 0facf0000h
        je     SHORT loc_1_D078
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_1_D078
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 2ch]
        mov    ecx, dword ptr [eax + 58h]
        jmp    SHORT loc_1_D07E
        db  08Dh, 049h, 000h
loc_1_D078:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0ch]
loc_1_D07E:
        mov    eax, 0
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_D08C
        mov    eax, dword ptr [esi + 68h]
loc_1_D08C:
        push   10h
        push   89h
        push   dword ptr [ebp - 0ch]
        db  068h, 0A8h, 010h, 001h, 000h
        push   eax
        push   ecx
        push   dword ptr [ebp - 4]
        call   sub_1_115D4
        add    esp, 1ch
        mov    esi, eax
        push   dword ptr [ebp - 4]
        call   sub_1_F280
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_D0C8
        cmp    esi, 0ffh
        je     SHORT loc_1_D0C8
        mov    eax, esi
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_1_D0C8:
        pop    edi
        mov    eax, 0ffh
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_D0D4:
        cmp    edi, 0ffh
        je     SHORT loc_1_D0F0
        push   7
        push   dword ptr [ebp - 4]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_1_D0F0:
        mov    eax, edi
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_D0F8:
        mov    eax, 0c000000dh
loc_1_D0FD:
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_D104:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    byte ptr [esp + 30h], 0
        jne    SHORT loc_1_D14C
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   dword ptr [eax + 14h]
        call   sub_1_FF70
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_D136
        cmp    eax, 0ffh
        jne    SHORT loc_1_D13D
loc_1_D136:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_D14C
loc_1_D13D:
        pop    ebp
        mov    eax, 0c000000dh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_D14C:
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_D168
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_D213
loc_1_D168:
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 2ch]
        mov    dword ptr [eax + 10h], esi
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 28h]
        mov    dword ptr [eax + 2ch], edi
        lea    eax, [esp + 14h]
        push   eax
        push   0
        push   0
        db  068h, 01Ch, 0D2h, 000h, 000h
        push   edi
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 28h]
        call   sub_1_C224
        add    esp, 1ch
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_D1B6
        cmp    ebx, 0ffh
        je     SHORT loc_1_D211
loc_1_D1B6:
        push   4
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, dword ptr [eax + 10h]
        push   dword ptr [esp + 14h]
        call   sub_1_D5C0
        add    esp, 8
        or     ebp, ebp
        je     SHORT loc_1_D1FC
        or     ebx, ebx
        jne    SHORT loc_1_D211
        push   dword ptr [edi + 58h]
        push   esi
        push   dword ptr [esp + 1ch]
        call   sub_1_D3DC
        add    esp, 0ch
        mov    ebx, eax
        push   dword ptr [esp + 14h]
        call   sub_1_DE94
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_D1FC:
        or     ebx, ebx
        jne    SHORT loc_1_D20C
        push   dword ptr [esp + 14h]
        call   sub_1_DE94
        add    esp, 4
loc_1_D20C:
        mov    ebx, 0ffh
loc_1_D211:
        mov    eax, ebx
loc_1_D213:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_D21C:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 10h]
        mov    dword ptr [esi + 10h], 0
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_D278
        lea    eax, [esp + 8]
        push   eax
        push   esi
        call   sub_1_C4AC
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_D278
        mov    ecx, dword ptr [esi + 2ch]
        push   dword ptr [ecx + 58h]
        push   edi
        push   eax
        call   sub_1_D3DC
        add    esp, 0ch
        push   -1
        push   eax
        push   edi
        call   sub_1_25A0
        add    esp, 0ch
        push   4
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_D278:
        push   0
        push   15h
        push   edi
        call   sub_1_25A0
        add    esp, 0ch
        push   4
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_D298:
        sub    esp, 8
        push   esi
        push   edi
        cmp    dword ptr [esp + 18h], 0
        jne    NEAR PTR loc_1_D36C
        lea    eax, [esp + 0ch]
        push   eax
        mov    esi, dword ptr [esp + 18h]
        push   esi
        call   sub_1_C4AC
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_D370
        lea    eax, [esp + 8]
        push   eax
        inc    dword ptr [edi + 1ch]
        db  066h, 0A1h, 0D0h, 032h, 001h, 000h
        push   eax
        db  068h, 090h, 0D3h, 000h, 000h
        push   esi
        db  068h, 0A8h, 0ECh, 000h, 000h
        push   0
        push   esi
        db  0FFh, 035h, 0CCh, 032h, 001h, 000h
        call   sub_1_109B8
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_D308
        cmp    eax, 0ffh
        je     SHORT loc_1_D308
        push   edi
        call   sub_1_DEA4
        add    esp, 4
        jmp    SHORT loc_1_D370
        db  08Dh, 049h, 000h
loc_1_D308:
        mov    dword ptr [esi + 38h], edi
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 30h], eax
        cmp    dword ptr [edi + 24h], 0facf0000h
        je     SHORT loc_1_D32C
        test   byte ptr [edi + 20h], 2
        je     SHORT loc_1_D32C
        mov    eax, dword ptr [esi + 2ch]
        mov    ecx, dword ptr [eax + 58h]
        jmp    SHORT loc_1_D32F
        db  08Dh, 049h, 000h
loc_1_D32C:
        mov    ecx, dword ptr [edi + 0ch]
loc_1_D32F:
        mov    eax, dword ptr [esi + 2ch]
        cmp    dword ptr [eax + 58h], 0
        je     SHORT loc_1_D340
        mov    eax, dword ptr [eax + 68h]
        jmp    SHORT loc_1_D342
        db  08Dh, 049h, 000h
loc_1_D340:
        sub    eax, eax
loc_1_D342:
        push   10h
        push   89h
        push   dword ptr [esi + 1ch]
        db  068h, 0A8h, 010h, 001h, 000h
        push   eax
        push   ecx
        push   esi
        call   sub_1_115D4
        add    esp, 1ch
        push   esi
        call   sub_1_F280
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  090h
loc_1_D36C:
        mov    esi, dword ptr [esp + 14h]
loc_1_D370:
        push   7
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        push   0
        push   15h
        push   dword ptr [esi + 10h]
        call   sub_1_25A0
        add    esp, 0ch
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_1_D390:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 10h]
        push   dword ptr [esi + 38h]
        call   sub_1_DEA4
        add    esp, 4
        push   esi
        call   sub_1_F280
        add    esp, 4
        mov    eax, dword ptr [esp + 10h]
        or     eax, eax
        je     SHORT loc_1_D3CC
        cmp    eax, 9
        je     SHORT loc_1_D3CC
        push   0
        push   15h
        push   edi
        call   sub_1_25A0
        add    esp, 0ch
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_D3CC:
        push   -1
        push   eax
        push   edi
        call   sub_1_25A0
        add    esp, 0ch
        pop    edi
        pop    esi
        ret
        db  090h
sub_1_D3DC:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        test   byte ptr [ebx + 20h], 8
        je     SHORT loc_1_D404
        sub    esi, esi
        mov    eax, dword ptr [ebx + 0ch]
        cmp    dword ptr [eax], -1
        je     SHORT loc_1_D409
loc_1_D3F8:
        inc    esi
        add    eax, 4
        cmp    dword ptr [eax], -1
        jne    SHORT loc_1_D3F8
        jmp    SHORT loc_1_D409
        db  090h
loc_1_D404:
        mov    esi, 1
loc_1_D409:
        mov    ecx, dword ptr [esp + 20h]
        movzx  eax, word ptr [ecx + 8]
        mov    ecx, esi
        shl    ecx, 5
        lea    ecx, [ecx + esi + 4]
        mov    dword ptr [esp + 14h], ecx
        cmp    ecx, eax
        jbe    SHORT loc_1_D447
        cmp    eax, 4
        mov    esi, 0
        jbe    SHORT loc_1_D43A
        mov    ecx, 21h
        sub    eax, 4
        sub    edx, edx
        div    ecx
        mov    esi, eax
loc_1_D43A:
        mov    eax, esi
        shl    eax, 5
        lea    ecx, [eax + esi + 4]
        mov    dword ptr [esp + 14h], ecx
loc_1_D447:
        cmp    dword ptr [esp + 14h], 0ffffh
        jbe    SHORT loc_1_D460
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_D460:
        movzx  eax, word ptr [esp + 14h]
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        jne    SHORT loc_1_D484
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_D484:
        sub    eax, eax
        mov    edx, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    edx, dword ptr [esp + 10h]
        add    edx, 4
        mov    ecx, dword ptr [esp + 10h]
        mov    word ptr [ecx], si
        mov    edi, dword ptr [ebx + 20h]
        and    edi, 2
        cmp    edi, 1
        sbb    al, al
        and    al, 80h
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, eax
        and    ecx, 0ff00h
        mov    ebp, eax
        shl    ebp, 10h
        or     ecx, ebp
        shl    ecx, 8
        mov    ebp, eax
        and    ebp, 0ff0000h
        shr    ebp, 8
        or     ecx, ebp
        shr    eax, 18h
        and    eax, 0ffh
        or     ecx, eax
        test   byte ptr [ebx + 20h], 8
        je     SHORT loc_1_D538
        mov    eax, dword ptr [ebx + 0ch]
        or     esi, esi
        je     NEAR PTR loc_1_D57E
loc_1_D4F8:
        mov    edi, dword ptr [eax]
        mov    ebx, edi
        and    ebx, 0ff00h
        mov    ebp, edi
        shl    ebp, 10h
        or     ebx, ebp
        shl    ebx, 8
        mov    ebp, edi
        and    ebp, 0ff0000h
        shr    ebp, 8
        or     ebx, ebp
        shr    edi, 18h
        and    edi, 0ffh
        or     ebx, edi
        mov    dword ptr [edx + 0bh], ebx
        add    eax, 4
        mov    dword ptr [edx + 5], ecx
        add    edx, 21h
        dec    esi
        jne    SHORT loc_1_D4F8
        jmp    SHORT loc_1_D57E
        db  08Dh, 049h, 000h
loc_1_D538:
        cmp    dword ptr [ebx + 24h], 0facf0000h
        je     SHORT loc_1_D54C
        or     edi, edi
        je     SHORT loc_1_D54C
        mov    dword ptr [edx + 0bh], ecx
        jmp    SHORT loc_1_D57B
        db  08Bh, 0FFh
loc_1_D54C:
        mov    esi, dword ptr [ebx + 0ch]
        mov    edi, esi
        and    edi, 0ff00h
        mov    eax, esi
        shl    eax, 10h
        or     edi, eax
        shl    edi, 8
        mov    eax, esi
        and    eax, 0ff0000h
        shr    eax, 8
        or     edi, eax
        shr    esi, 18h
        and    esi, 0ffh
        or     edi, esi
        mov    dword ptr [edx + 0bh], edi
loc_1_D57B:
        mov    dword ptr [edx + 5], ecx
loc_1_D57E:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 20h]
        mov    edi, dword ptr [ecx + 4]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 20h]
        push   dword ptr [esp + 10h]
        mov    ax, word ptr [esp + 18h]
        mov    word ptr [ecx + 8], ax
        call   sub_1_0024
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
sub_1_D5C0:
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        test   al, 2
        mov    esi, dword ptr [esp + 8]
        je     SHORT loc_1_D5D7
        mov    edx, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
loc_1_D5D7:
        test   al, 1
        je     SHORT loc_1_D5EB
        mov    eax, dword ptr [esi + 1ch]
        or     eax, eax
        je     SHORT loc_1_D5EB
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_D5EB:
        mov    ecx, dword ptr [esi + 4eh]
        mov    dword ptr [esi + 1ch], 0
        mov    eax, dword ptr [esi + 52h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        db  0C7h, 006h, 064h, 032h, 001h, 000h
        db  0A1h, 068h, 032h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 068h, 032h, 001h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 068h, 032h, 001h, 000h
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_D61C:
        push   dword ptr [esp + 4]
        call   sub_1_DEE4
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_D62C:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 2ch], 0
        cmp    dword ptr [esp + 38h], 0
        je     SHORT loc_1_D694
        mov    eax, dword ptr [esp + 38h]
        cmp    dword ptr [eax + 8], 54637281h
        jne    SHORT loc_1_D684
        mov    dword ptr [esp + 30h], 0
        cmp    dword ptr [esp + 38h], 0
        je     SHORT loc_1_D694
        db  0BDh, 074h, 032h, 001h, 000h
        db  08Bh, 01Dh, 074h, 032h, 001h, 000h
        db  068h, 034h, 032h, 001h, 000h
        call   sub_1_10288
        add    esp, 4
        inc    eax
        mov    dword ptr [esp + 18h], eax
        mov    ecx, 12h
        jmp    SHORT loc_1_D6DB
        db  08Bh, 0FFh
loc_1_D684:
        pop    ebp
        mov    eax, 0c0000008h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 049h, 000h
loc_1_D694:
        db  08Bh, 03Dh, 0B8h, 032h, 001h, 000h
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 24h], edi
        mov    eax, edi
        mov    eax, dword ptr [eax]
        or     eax, eax
        jle    SHORT loc_1_D6CA
        inc    edx
        mov    edx, edi
        add    edx, 8
loc_1_D6B4:
        mov    ecx, dword ptr [edx]
        cmp    ecx, edx
        je     SHORT loc_1_D6C4
loc_1_D6BA:
        mov    ecx, dword ptr [ecx]
        inc    dword ptr [esp + 18h]
        cmp    ecx, edx
        jne    SHORT loc_1_D6BA
loc_1_D6C4:
        add    edx, 8
        dec    eax
        jne    SHORT loc_1_D6B4
loc_1_D6CA:
        db  08Bh, 02Dh, 0B8h, 032h, 001h, 000h
        add    ebp, 8
        mov    ebx, dword ptr [ebp]
        mov    ecx, 1bh
loc_1_D6DB:
        mov    eax, dword ptr [esp + 18h]
        imul   eax, ecx
        lea    edx, [eax + 3ch]
        mov    dword ptr [esp + 28h], edx
        mov    eax, dword ptr [esp + 40h]
        mov    eax, dword ptr [eax]
        cmp    edx, eax
        jle    SHORT loc_1_D725
        mov    dword ptr [esp + 2ch], 1
        cmp    eax, 3ch
        ja     SHORT loc_1_D70C
        mov    dword ptr [esp + 18h], 0
        jmp    SHORT loc_1_D717
        db  08Bh, 0FFh
loc_1_D70C:
        sub    edx, edx
        sub    eax, 3ch
        div    ecx
        mov    dword ptr [esp + 18h], eax
loc_1_D717:
        mov    eax, dword ptr [esp + 18h]
        imul   eax, ecx
        lea    edx, [eax + 3ch]
        mov    dword ptr [esp + 28h], edx
loc_1_D725:
        movzx  eax, word ptr [esp + 28h]
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 1ch], eax
        or     eax, eax
        jne    SHORT loc_1_D748
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_1_D748:
        sub    eax, eax
        mov    edx, dword ptr [esp + 28h]
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        cmp    dword ptr [esp + 38h], eax
        je     SHORT loc_1_D76C
        mov    esi, dword ptr [esp + 38h]
        jmp    SHORT loc_1_D772
loc_1_D76C:
        db  08Bh, 035h, 03Ch, 032h, 001h, 000h
loc_1_D772:
        add    esi, 8ch
        mov    edi, dword ptr [esp + 1ch]
        movsd
        movsw
        mov    edx, dword ptr [esp + 1ch]
        mov    byte ptr [edx + 6], 3
        mov    byte ptr [edx + 8], 0feh
        mov    eax, dword ptr [esp + 38h]
        movzx  ax, byte ptr [eax + 4fh]
        mov    word ptr [edx + 34h], ax
        mov    word ptr [edx + 36h], ax
        mov    word ptr [edx + 28h], 40h
        mov    word ptr [edx + 2ah], 40h
        mov    word ptr [edx + 2ch], 40h
        mov    word ptr [edx + 30h], 200h
        mov    word ptr [edx + 38h], 0ffffh
        mov    eax, edx
        add    eax, 3ch
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 20h], 0
        sub    ecx, ecx
        cmp    dword ptr [esp + 18h], ecx
        je     NEAR PTR loc_1_D937
        nop
loc_1_D7D8:
        cmp    dword ptr [esp + 38h], 0
        je     SHORT loc_1_D808
        cmp    ebp, ebx
        je     NEAR PTR loc_1_D937
        mov    esi, dword ptr [ebx + 14h]
        mov    ebx, dword ptr [ebx]
        cmp    byte ptr [esi + 34h], 2ah
        je     NEAR PTR loc_1_D928
        test   byte ptr [esi + 20h], 1
        je     NEAR PTR loc_1_D899
        jmp    NEAR PTR loc_1_D928
        db  08Dh, 049h, 000h
loc_1_D808:
        sub    al, al
        mov    byte ptr [esp + 13h], 0
        mov    esi, ebx
        mov    edi, dword ptr [esp + 24h]
        lea    edx, [edi + ecx*8 + 8]
        mov    edi, dword ptr [edi]
loc_1_D81C:
        cmp    ecx, edi
        je     SHORT loc_1_D830
        cmp    ebp, ebx
        jne    SHORT loc_1_D838
        add    edx, 8
        inc    ecx
        mov    ebp, edx
        mov    ebx, dword ptr [edx]
        mov    al, 1
        jmp    SHORT loc_1_D81C
loc_1_D830:
        mov    byte ptr [esp + 13h], 1
        jmp    SHORT loc_1_D83E
        db  090h
loc_1_D838:
        or     al, al
        jne    SHORT loc_1_D83E
        mov    ebx, dword ptr [ebx]
loc_1_D83E:
        cmp    byte ptr [esp + 13h], 0
        jne    NEAR PTR loc_1_D937
        or     al, al
        jne    NEAR PTR loc_1_D92C
        mov    edx, dword ptr [esi + 20h]
        test   dh, 10h
        jne    NEAR PTR loc_1_D920
        test   dl, 10h
        je     NEAR PTR loc_1_D920
        test   dl, 8
        mov    eax, dword ptr [esi + 0ch]
        je     SHORT loc_1_D870
        mov    eax, dword ptr [eax]
loc_1_D870:
        test   dh, 8
        mov    edi, dword ptr [esp + 14h]
        mov    dword ptr [edi + 13h], eax
        mov    eax, 0ffffffffh
        jne    SHORT loc_1_D892
        movzx  eax, word ptr [esi + 30h]
        lea    eax, [eax*4 + 4]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
loc_1_D892:
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [edx + 17h], eax
loc_1_D899:
        mov    edx, dword ptr [esi + 20h]
        mov    eax, edx
        and    eax, 2
        cmp    eax, 1
        sbb    al, al
        and    al, 80h
        mov    edi, dword ptr [esp + 14h]
        mov    byte ptr [edi + 11h], al
        and    edx, 0f0h
        cmp    edx, 20h
        je     SHORT loc_1_D8D4
        cmp    edx, 40h
        je     SHORT loc_1_D8E0
        cmp    edx, 80h
        je     SHORT loc_1_D8E8
        mov    eax, dword ptr [esp + 14h]
        or     byte ptr [eax + 11h], 4
        jmp    SHORT loc_1_D8E8
        db  08Dh, 049h, 000h
loc_1_D8D4:
        mov    eax, dword ptr [esp + 14h]
        or     byte ptr [eax + 11h], 5
        jmp    SHORT loc_1_D8E8
        db  08Bh, 0FFh
loc_1_D8E0:
        mov    eax, dword ptr [esp + 14h]
        or     byte ptr [eax + 11h], 6
loc_1_D8E8:
        add    esi, 34h
        mov    eax, dword ptr [esp + 20h]
        inc    al
        mov    edx, dword ptr [esp + 14h]
        mov    byte ptr [edx + 10h], al
        mov    edi, dword ptr [esp + 14h]
        movsd
        movsd
        movsd
        movsd
        cmp    dword ptr [esp + 38h], 0
        je     SHORT loc_1_D910
        add    dword ptr [esp + 14h], 12h
        jmp    SHORT loc_1_D915
        db  08Bh, 0FFh
loc_1_D910:
        add    dword ptr [esp + 14h], 1bh
loc_1_D915:
        dec    dword ptr [esp + 18h]
        inc    dword ptr [esp + 20h]
        jmp    SHORT loc_1_D92C
        db  090h
loc_1_D920:
        mov    edx, dword ptr [esp + 1ch]
        dec    word ptr [edx + 3ah]
loc_1_D928:
        dec    dword ptr [esp + 18h]
loc_1_D92C:
        cmp    dword ptr [esp + 18h], 0
        jne    NEAR PTR loc_1_D7D8
loc_1_D937:
        mov    edx, dword ptr [esp + 1ch]
        cmp    dword ptr [esp + 2ch], 1
        mov    ax, word ptr [esp + 20h]
        mov    word ptr [edx + 3ah], ax
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 40h]
        mov    edx, dword ptr [esp + 28h]
        mov    dword ptr [eax], edx
        cmc
        sbb    eax, eax
        and    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  090h
sub_1_D968:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 2ch], 0
        sub    ebx, ebx
        db  08Bh, 00Dh, 074h, 032h, 001h, 000h
        db  081h, 0F9h, 074h, 032h, 001h, 000h
        je     SHORT loc_1_D9C5
        nop
loc_1_D988:
        lea    esi, [ecx + 0ch]
        mov    edx, dword ptr [esi]
        cmp    esi, edx
        je     SHORT loc_1_D9BB
loc_1_D991:
        mov    edi, edx
        mov    edx, dword ptr [edx]
        lea    ebp, [edi + 1ch]
        mov    eax, dword ptr [ebp]
        cmp    ebp, eax
        je     SHORT loc_1_D9A7
loc_1_D9A0:
        mov    eax, dword ptr [eax]
        inc    ebx
        cmp    ebp, eax
        jne    SHORT loc_1_D9A0
loc_1_D9A7:
        add    edi, 2ch
        mov    eax, dword ptr [edi]
        cmp    edi, eax
        je     SHORT loc_1_D9B7
loc_1_D9B0:
        mov    eax, dword ptr [eax]
        inc    ebx
        cmp    edi, eax
        jne    SHORT loc_1_D9B0
loc_1_D9B7:
        cmp    esi, edx
        jne    SHORT loc_1_D991
loc_1_D9BB:
        mov    ecx, dword ptr [ecx]
        db  081h, 0F9h, 074h, 032h, 001h, 000h
        jne    SHORT loc_1_D988
loc_1_D9C5:
        lea    eax, [ebx*4]
        lea    eax, [eax + eax*2]
        lea    ecx, [ebx + eax*4 + 35h]
        mov    dword ptr [esp + 24h], ecx
        mov    eax, dword ptr [esp + 3ch]
        mov    eax, dword ptr [eax]
        cmp    eax, ecx
        jge    SHORT loc_1_DA13
        mov    dword ptr [esp + 2ch], 1
        cmp    eax, 35h
        mov    ebx, 0
        jbe    SHORT loc_1_DA01
        mov    ecx, 31h
        sub    eax, 35h
        sub    edx, edx
        div    ecx
        mov    ebx, eax
loc_1_DA01:
        lea    eax, [ebx*4]
        lea    eax, [eax + eax*2]
        lea    ecx, [ebx + eax*4 + 35h]
        mov    dword ptr [esp + 24h], ecx
loc_1_DA13:
        push   dword ptr [esp + 24h]
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 28h], eax
        or     eax, eax
        jne    SHORT loc_1_DA34
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_DA34:
        sub    eax, eax
        mov    edx, dword ptr [esp + 24h]
        mov    edi, dword ptr [esp + 28h]
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    ecx, dword ptr [esp + 28h]
        mov    dword ptr [ecx], ebx
        mov    edx, ecx
        add    edx, 4
        db  0A1h, 074h, 032h, 001h, 000h
        mov    dword ptr [esp + 20h], eax
        sub    ecx, ecx
loc_1_DA62:
        db  081h, 07Ch, 024h, 020h, 074h, 032h, 001h, 000h
        je     NEAR PTR loc_1_DB48
        mov    esi, dword ptr [esp + 20h]
        mov    eax, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 20h], eax
        lea    eax, [esi + 0ch]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 14h], eax
loc_1_DA89:
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esp + 1ch], eax
        je     NEAR PTR loc_1_DB40
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 18h]
        add    eax, 1ch
        mov    dword ptr [esp + 10h], eax
        mov    ebp, dword ptr [eax]
loc_1_DAAE:
        cmp    dword ptr [esp + 10h], ebp
        je     SHORT loc_1_DB00
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    esi, dword ptr [eax + 14h]
        lea    edi, [edx + 8]
        add    esi, 34h
        movsd
        movsd
        movsd
        movsd
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        je     SHORT loc_1_DAF0
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [edx + 4], eax
        mov    al, byte ptr [esi + 45h]
        mov    byte ptr [edx + 30h], al
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [edx + 28h], eax
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edx + 2ch], eax
        lea    esi, [ebp + 28h]
        lea    edi, [edx + 18h]
        movsd
        movsd
        movsd
        movsd
loc_1_DAF0:
        inc    ecx
        mov    eax, dword ptr [ebp + 68h]
        mov    dword ptr [edx], eax
        add    edx, 31h
        mov    ebp, dword ptr [ebp]
        cmp    ecx, ebx
        jne    SHORT loc_1_DAAE
loc_1_DB00:
        cmp    ecx, ebx
        je     SHORT loc_1_DB48
        mov    ebp, dword ptr [esp + 18h]
        add    ebp, 2ch
        mov    eax, dword ptr [ebp]
loc_1_DB0E:
        cmp    ebp, eax
        je     SHORT loc_1_DB38
        mov    esi, dword ptr [eax + 10h]
        mov    esi, dword ptr [esi + 10h]
        mov    esi, dword ptr [esi + 10h]
        mov    esi, dword ptr [esi + 14h]
        lea    edi, [edx + 8]
        add    esi, 34h
        movsd
        movsd
        movsd
        movsd
        mov    dword ptr [edx], 14h
        inc    ecx
        add    edx, 31h
        mov    eax, dword ptr [eax]
        cmp    ecx, ebx
        jne    SHORT loc_1_DB0E
loc_1_DB38:
        cmp    ecx, ebx
        jne    NEAR PTR loc_1_DA89
loc_1_DB40:
        cmp    ecx, ebx
        jne    NEAR PTR loc_1_DA62
loc_1_DB48:
        mov    eax, dword ptr [esp + 38h]
        mov    ecx, dword ptr [esp + 28h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [eax], ecx
        cmp    dword ptr [esp + 2ch], 1
        cmc
        sbb    eax, eax
        and    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  090h
_LTEXT ends
        end
