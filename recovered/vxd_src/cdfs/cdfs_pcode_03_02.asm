; cdfs_pcode_03_02.asm -- CDFS object 3 segment _PTEXT  (part 2/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_5150:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_5180:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_51B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    eax, eax
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        inc    eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 0ch
        push   eax
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_3_51F0
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_3_51F0
        mov    eax, dword ptr [ebp - 4]
        xor    eax, dword ptr [ebp + 8]
        and    eax, 0fffh
        xor    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
loc_3_51F0:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_5200:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        lea    ecx, [ecx + ecx*4]
        lea    eax, [eax + ecx*2 - 210h]
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_3_5220:
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    eax, 30h
        mov    ecx, 11h
        repe scasb
        sete   byte ptr [esp + 0bh]
        cmp    byte ptr [esp + 0bh], 0
        je     SHORT loc_3_5258
        mov    eax, dword ptr [esp + 14h]
        pop    edi
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        pop    esi
        add    esp, 4
        ret
loc_3_5258:
        mov    eax, dword ptr [esp + 10h]
        add    eax, 0eh
        push   eax
        call   sub_3_5200
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esp + 14h]
        add    eax, 0ch
        push   eax
        call   sub_3_5200
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esp + 18h]
        add    eax, 0ah
        push   eax
        call   sub_3_5200
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        add    eax, 8
        push   eax
        call   sub_3_5200
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esp + 20h]
        add    eax, 6
        push   eax
        call   sub_3_5200
        add    esp, 4
        dec    eax
        push   eax
        mov    eax, dword ptr [esp + 24h]
        add    eax, 4
        push   eax
        call   sub_3_5200
        add    esp, 4
        dec    eax
        push   eax
        mov    eax, dword ptr [esp + 28h]
        add    eax, 2
        push   eax
        call   sub_3_5200
        add    esp, 4
        mov    esi, eax
        mov    eax, dword ptr [esp + 28h]
        push   eax
        call   sub_3_5200
        shl    eax, 2
        add    esp, 4
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    esi, eax
        mov    eax, dword ptr [esp + 2ch]
        push   esi
        push   eax
        call   sub_3_53A0
        add    esp, 20h
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_3_5300:
        sub    esp, 4
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, 30h
        mov    ecx, 7
        repe scasb
        sete   byte ptr [esp + 7]
        cmp    byte ptr [esp + 7], 0
        je     SHORT loc_3_5336
        mov    eax, dword ptr [esp + 10h]
        pop    edi
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        add    esp, 4
        ret
loc_3_5336:
        mov    eax, dword ptr [esp + 0ch]
        push   0
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [esp + 14h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [esp + 18h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        dec    eax
        push   eax
        mov    eax, dword ptr [esp + 20h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        dec    eax
        push   eax
        mov    eax, dword ptr [esp + 24h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    eax, 76ch
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   eax
        call   sub_3_53A0
        add    esp, 20h
        pop    edi
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_3_53A0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 1ch]
        cmp    ecx, 641h
        jb     NEAR PTR loc_3_5483
        mov    esi, dword ptr [esp + 20h]
        cmp    esi, 0ch
        jae    NEAR PTR loc_3_5483
        sub    ecx, 641h
        xor    edi, edi
        db  066h, 08Bh, 03Ch, 075h, 0C8h, 09Eh, 000h, 000h
        mov    ebx, 64h
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        sub    edi, eax
        lea    eax, [ecx + ecx*8]
        lea    eax, [ecx + eax*8]
        mov    ebx, 190h
        lea    eax, [eax + eax*4]
        add    edi, eax
        mov    eax, ecx
        shr    eax, 2
        add    edi, eax
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        add    edi, eax
        add    edi, dword ptr [esp + 24h]
        mov    dword ptr [esp + 0ch], edi
        inc    ecx
        test   cl, 3
        jne    SHORT loc_3_5416
        cmp    esi, 2
        jb     SHORT loc_3_5416
        inc    dword ptr [esp + 0ch]
loc_3_5416:
        mov    eax, dword ptr [esp + 28h]
        lea    ecx, [eax*4]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        add    ecx, dword ptr [esp + 2ch]
        shl    ecx, 2
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        add    ecx, dword ptr [esp + 30h]
        shl    ecx, 2
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        add    ecx, dword ptr [esp + 34h]
        mov    dword ptr [esp + 10h], ecx
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, 83d600h
        mul    edx
        add    eax, dword ptr [esp + 10h]
        adc    edx, 0
        mov    ebx, eax
        mov    eax, edx
        mov    edx, 186a0h
        mul    edx
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, eax
        mov    eax, ebx
        mov    edx, 186a0h
        mul    edx
        add    edx, ecx
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi], eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_5483:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esp + 18h]
        pop    edi
        mov    dword ptr [eax], 0
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
sub_3_54A0:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [esi + 24h], 0
        xor    eax, eax
        db  087h, 005h, 014h, 09Ch, 000h, 000h
        mov    dword ptr [esp + 8], eax
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_3_54D6
        push   0
        push   596h
        call   sub_3_43B0
        mov    dword ptr [esp + 10h], eax
        add    esp, 8
loc_3_54D6:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 10h], eax
        or     eax, eax
        jne    SHORT loc_3_54F0
        mov    dword ptr [esi + 24h], 8
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_3_54F0:
        mov    eax, dword ptr [esp + 8]
        lea    ecx, [eax + 4ch]
        mov    dword ptr [eax], ecx
        lea    ecx, [eax + 254h]
        mov    dword ptr [eax + 4], ecx
        lea    ecx, [eax + 4dch]
        mov    dword ptr [eax + 8], ecx
        xor    ecx, ecx
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax + 28h], ecx
        mov    dword ptr [eax + 2ch], ecx
        mov    dword ptr [eax + 48h], ecx
        test   byte ptr [esi + 23h], 2
        jne    SHORT loc_3_5568
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0ch]
        mov    ax, word ptr [esi]
        sub    ax, 4
        mov    word ptr [ecx + 48h], ax
        mov    ax, word ptr [esi + 2]
        mov    ecx, dword ptr [esp + 8]
        sub    ax, 4
        mov    word ptr [ecx + 4ah], ax
        mov    eax, dword ptr [esp + 8]
        add    esi, 4
        mov    ax, word ptr [eax + 48h]
        and    eax, 0ffffh
        mov    edi, dword ptr [esp + 8]
        add    edi, 4ch
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_5568:
        mov    eax, dword ptr [esp + 8]
        mov    ax, word ptr [eax + 48h]
        and    eax, 0fffeh
        mov    ecx, dword ptr [esp + 8]
        mov    word ptr [eax + ecx + 4ch], 0
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_5590:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_3_55D2
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [esp + 4]
        db  087h, 005h, 014h, 09Ch, 000h, 000h
        mov    dword ptr [esp + 4], eax
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_3_55CB
        mov    eax, dword ptr [esi + 20h]
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0ch
loc_3_55CB:
        mov    dword ptr [esi + 10h], 0
loc_3_55D2:
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_55E0:
        sub    esp, 0c4h
        mov    eax, dword ptr [esp + 0cch]
        push   eax
        mov    eax, dword ptr [esp + 0cch]
        push   eax
        lea    eax, [esp + 8]
        push   eax
        call   sub_3_5680
        mov    eax, dword ptr [esp + 0d4h]
        add    esp, 0ch
        cmp    word ptr [eax + 1ah], 0
        je     SHORT loc_3_5623
        mov    eax, dword ptr [esp + 0c8h]
        movsx  eax, word ptr [eax + 1ah]
        add    esp, 0c4h
        ret
loc_3_5623:
        mov    ax, word ptr [esp + 0d0h]
        mov    word ptr [esp + 2eh], ax
        lea    eax, [esp]
        push   eax
        call   sub_3_83D0
        lea    eax, [esp + 4]
        add    esp, 4
        push   eax
        call   sub_3_75B0
        lea    eax, [esp + 4]
        add    esp, 4
        push   eax
        call   sub_3_5590
        lea    eax, [esp + 4]
        add    esp, 4
        push   eax
        call   sub_3_6820
        add    esp, 4
        db  0E8h, 0B7h, 0ACh, 0FCh, 0FFh
        mov    eax, dword ptr [esp + 0c8h]
        movsx  eax, word ptr [eax + 1ah]
        add    esp, 0c4h
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_3_5680:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 31h
        rep stosd
        mov    dword ptr [esi + 28h], 53464443h
        mov    edi, dword ptr [esp + 10h]
        mov    dword ptr [esi], edi
        db  0A1h, 010h, 09Ch, 000h, 000h
        mov    dword ptr [esi + 68h], eax
        db  0F6h, 005h, 084h, 008h, 000h, 000h, 001h
        je     SHORT loc_3_56B8
        mov    word ptr [edi + 1ah], 5
        pop    edi
        pop    esi
        ret
loc_3_56B8:
        db  0E8h, 083h, 0AAh, 0FCh, 0FFh
        mov    ecx, dword ptr [esi]
        mov    word ptr [ecx + 1ah], ax
        mov    eax, dword ptr [esi]
        cmp    word ptr [eax + 1ah], 0
        jne    NEAR PTR loc_3_5766
        mov    byte ptr [esi + 63h], 5
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, 1
        je     SHORT loc_3_56F0
        cmp    eax, 2
        je     SHORT loc_3_5703
        cmp    eax, 3
        je     SHORT loc_3_5722
        cmp    eax, 5
        je     SHORT loc_3_5703
        pop    edi
        pop    esi
        ret
        db  090h
loc_3_56F0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 28h]
        mov    dword ptr [esi + 40h], eax
        or     byte ptr [esi + 6fh], 5
        pop    edi
        pop    esi
        ret
loc_3_5703:
        mov    ecx, dword ptr [edi + 1ch]
        mov    dword ptr [esi + 14h], ecx
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esi + 44h], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esi + 40h], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 4], eax
        or     byte ptr [esi + 6fh], 2
        pop    edi
        pop    esi
        ret
loc_3_5722:
        mov    eax, dword ptr [edi + 20h]
        mov    dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [esi + 1ch]
        test   byte ptr [eax + 0bh], 1
        je     SHORT loc_3_5740
        mov    eax, dword ptr [esi]
        pop    edi
        mov    word ptr [eax + 1ah], 6
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_5740:
        mov    eax, dword ptr [esi + 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, eax
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esi + 44h], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esi + 40h], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 4], eax
loc_3_5766:
        pop    edi
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5770:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   0
        push   esi
        call   sub_3_71C0
        add    esp, 8
        mov    dword ptr [esi + 50h], eax
        mov    eax, dword ptr [esi + 50h]
        cmp    eax, 2
        je     SHORT loc_3_57A0
        cmp    eax, 3
        je     SHORT loc_3_57B0
        cmp    eax, 4
        je     SHORT loc_3_57C5
        mov    dword ptr [esi + 24h], 8
        pop    esi
        ret
        db  08Bh, 0FFh
loc_3_57A0:
        db  0A1h, 090h, 008h, 000h, 000h
        pop    esi
        inc    dword ptr [eax + 0bch]
        ret
        db  08Dh, 049h, 000h
loc_3_57B0:
        mov    ax, word ptr [esi + 2ch]
        mov    word ptr [esi + 2eh], ax
        push   46h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret
loc_3_57C5:
        db  0A1h, 090h, 008h, 000h, 000h
        pop    esi
        inc    dword ptr [eax + 0b8h]
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_57E0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 44h]
        mov    edi, dword ptr [edi + 1ch]
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        pop    edi
        pop    esi
        ret
sub_3_57F5:
        mov    ecx, dword ptr [esp + 4]
        db  0CDh, 020h, 00Fh, 080h, 010h, 000h
sub_3_57FF:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 01Ch, 080h, 001h, 000h
sub_3_5809:
        db  0CDh, 020h, 055h, 080h, 001h, 000h
        movzx  edx, word ptr [esp + 4]
        movzx  ecx, word ptr [esp + 6]
        db  0CDh, 020h, 048h, 080h, 001h, 000h
        db  0CDh, 020h, 04Ah, 080h, 001h, 000h
sub_3_5825:
        push   ebx
        push   edi
        push   esi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [edi + 1ch]
        mov    ecx, dword ptr [esi + 4]
        mov    ebx, dword ptr [edi + 28h]
        mov    eax, dword ptr [ebx + 1ah]
        mov    ax, word ptr [ebx + 18h]
        and    ax, 0bfffh
        cmp    eax, 440d0866h
        je     NEAR PTR loc_3_58EE
        push   ecx
        call   sub_3_36E0
        lea    esp, [esp + 4]
        mov    edx, eax
        mov    ecx, dword ptr [esi + 4]
        mov    word ptr [edx + 4], 0dh
        mov    dword ptr [edx + 8], 510h
        mov    dword ptr [edx + 3ch], 1
        mov    dword ptr [edx + 38h], ebx
        movzx  eax, word ptr [ebx + 10h]
        mov    word ptr [edx + 2ch], ax
        dec    al
        mov    byte ptr [edx + 4dh], al
        movzx  eax, word ptr [ebx + 18h]
        mov    dword ptr [edx + 30h], eax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [edx + 34h], eax
        movzx  eax, word ptr [ebx + 1ch]
        mov    word ptr [edx + 2eh], ax
        xor    ecx, ecx
        mov    dword ptr [edx + 44h], ecx
        push   edx
        push   edx
        call   sub_3_57E0
        pop    esi
        pop    esi
        mov    eax, dword ptr [esi + 3ch]
        mov    word ptr [ebx + 1ch], ax
        cmp    word ptr [esi + 6], 10h
        jae    SHORT loc_3_58B8
        xor    eax, eax
loc_3_58B8:
        mov    word ptr [edi + 1ah], ax
        push   esi
        call   sub_3_3780
        lea    esp, [esp + 4]
        movzx  eax, word ptr [edi + 1ah]
loc_3_58CA:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_3_58CE:
        inc    ebx
        inc    esp
        push   edx
        dec    edi
        dec    ebp
        and    byte ptr [eax], ah
        db  020h
loc_3_58D6:
        inc    ebx
        inc    esp
        xor    byte ptr [eax], dh
        xor    dword ptr [eax], esp
        and    byte ptr [eax], ah
loc_3_58DE:
        inc    ebx
        inc    esp
        inc    ecx
        push   ebp
        inc    esp
        dec    ecx
        dec    edi
        db  020h
loc_3_58E6:
        inc    esi
        dec    edi
        push   edx
        inc    ebp
        dec    ecx
        inc    edi
        dec    esi
        db  020h
loc_3_58EE:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    word ptr [eax], 0
        mov    ecx, dword ptr [esi + 3ch]
        mov    dword ptr [eax + 2], ecx
        mov    ecx, dword ptr [esi + 58h]
        mov    dword ptr [eax + 6], ecx
        mov    ecx, dword ptr [esi + 5ch]
        mov    dword ptr [eax + 0ah], ecx
        mov    ecx, dword ptr [esi + 60h]
        mov    dword ptr [eax + 0eh], ecx
        test   byte ptr [esi + 1fh], 2
        je     SHORT loc_3_592D
        db  0BAh, 0E6h, 058h, 000h, 000h
        test   byte ptr [esi + 1fh], 20h
        jne    SHORT loc_3_593D
        db  0BAh, 0DEh, 058h, 000h, 000h
        jmp    SHORT loc_3_593D
loc_3_592D:
        db  0BAh, 0CEh, 058h, 000h, 000h
        cmp    byte ptr [esi + 1eh], 0
        je     SHORT loc_3_593D
        db  0BAh, 0D6h, 058h, 000h, 000h
loc_3_593D:
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax + 11h], ecx
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [eax + 15h], ecx
        and    word ptr [ebx + 2ch], 0fffeh
        xor    eax, eax
        mov    word ptr [edi + 1ah], ax
        jmp    NEAR PTR loc_3_58CA
sub_3_5958:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, ecx
        shr    eax, 10h
        shl    eax, 4
        movzx  ecx, cx
        add    eax, ecx
        mov    ecx, dword ptr [esp + 4]
        add    eax, dword ptr [ecx + 4]
        ret
sub_3_5971:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 0b0h]
        mov    ecx, dword ptr [eax + 20h]
        and    dword ptr [eax + 20h], 0fffff9ffh
        test   ecx, 100h
        jne    SHORT loc_3_59B3
        cmp    dword ptr [edx], 0
        jne    SHORT loc_3_59AC
        test   ecx, 400h
        jne    SHORT loc_3_59A0
        cmp    byte ptr [edx + 4], 0
        jne    SHORT loc_3_59AC
loc_3_59A0:
        inc    byte ptr [edx + 6]
        jmp    SHORT loc_3_59D5
loc_3_59A5:
        or     dword ptr [eax + 20h], 200h
loc_3_59AC:
        mov    eax, 1
        jmp    SHORT loc_3_59D7
loc_3_59B3:
        cmp    dword ptr [edx], eax
        je     SHORT loc_3_59D2
        cmp    dword ptr [edx], 0
        jne    SHORT loc_3_59A5
        cmp    byte ptr [edx + 6], 0
        jne    SHORT loc_3_59A5
        test   ecx, 200h
        jne    SHORT loc_3_59D0
        cmp    byte ptr [edx + 4], 0
        jne    SHORT loc_3_59A5
loc_3_59D0:
        mov    dword ptr [edx], eax
loc_3_59D2:
        inc    byte ptr [edx + 7]
loc_3_59D5:
        xor    eax, eax
loc_3_59D7:
        ret
sub_3_59D8:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 0b0h]
        cmp    dword ptr [edx], 0
        je     SHORT loc_3_59FD
        dec    byte ptr [edx + 7]
        jne    SHORT loc_3_5A09
        mov    dword ptr [edx], 0
loc_3_59F2:
        movzx  ecx, byte ptr [edx + 4]
        or     cl, byte ptr [edx + 5]
        je     SHORT loc_3_5A09
        jmp    SHORT loc_3_5A04
loc_3_59FD:
        dec    byte ptr [edx + 6]
        jne    SHORT loc_3_5A09
        jmp    SHORT loc_3_59F2
loc_3_5A04:
        call   sub_3_8690
loc_3_5A09:
        ret
        db  056h, 057h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 074h, 024h, 010h, 08Bh, 04Ch, 024h, 014h, 0E8h, 084h
        db  0A6h, 0FCh, 0FFh, 05Fh, 05Eh, 0C3h
loc_3_5A20:
        add    esi, 8
loc_3_5A23:
        add    esi, 4
loc_3_5A26:
        add    esi, 4
        ret
loc_3_5A2A:
        add    esi, 8
loc_3_5A2D:
        add    esi, 4
loc_3_5A30:
        add    esi, 4
        shld   edx, eax, 10h
        add    ax, 8000h
        add    dx, 8000h
        mov    al, ah
        mov    ah, dh
        ret
loc_3_5A45:
        add    esi, 8
loc_3_5A48:
        add    esi, 4
loc_3_5A4B:
        add    esi, 4
        shld   edx, eax, 10h
        movsx  edx, dx
        movsx  eax, ax
        add    eax, edx
        sar    eax, 1
        ret
loc_3_5A5D:
        add    esi, 8
loc_3_5A60:
        add    esi, 4
loc_3_5A63:
        add    esi, 4
        shld   edx, eax, 10h
        movsx  edx, dx
        movsx  eax, ax
        add    eax, edx
        sar    eax, 1
        add    ax, 8000h
        mov    al, ah
        ret
        db  000h, 000h, 000h, 000h, 000h
loc_3_5A80:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   4
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_5AA0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 50h]
        or     eax, eax
        je     SHORT loc_3_5AB6
        cmp    eax, 2
        je     SHORT loc_3_5AD1
        cmp    eax, 4
        jne    SHORT loc_3_5AD8
loc_3_5AB6:
        and    dword ptr [esi + 20h], 0fffeffffh
        push   esi
        call   sub_3_69B0
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret
loc_3_5AD1:
        or     dword ptr [esi + 20h], 10000h
loc_3_5AD8:
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_5AE0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   1
        push   esi
        call   sub_3_71C0
        add    esp, 8
        mov    dword ptr [esi + 50h], eax
        mov    eax, dword ptr [esi + 50h]
        or     eax, eax
        je     SHORT loc_3_5B10
        cmp    eax, 1
        je     SHORT loc_3_5B21
        cmp    eax, 2
        je     SHORT loc_3_5B40
        cmp    eax, 3
        je     SHORT loc_3_5B56
        cmp    eax, 4
        je     SHORT loc_3_5B70
        pop    esi
        ret
loc_3_5B10:
        test   byte ptr [esi + 21h], 10h
        je     SHORT loc_3_5B84
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret
loc_3_5B21:
        test   byte ptr [esi + 21h], 10h
        je     SHORT loc_3_5B84
        mov    dword ptr [esi + 24h], 8
        mov    dword ptr [esi + 50h], 0
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret
loc_3_5B40:
        db  0A1h, 090h, 008h, 000h, 000h
        push   esi
        inc    dword ptr [eax + 0bch]
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret
loc_3_5B56:
        mov    ax, word ptr [esi + 2ch]
        mov    word ptr [esi + 2eh], ax
        push   46h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_5B70:
        db  0A1h, 090h, 008h, 000h, 000h
        push   esi
        inc    dword ptr [eax + 0b8h]
        call   sub_3_83B0
        add    esp, 4
loc_3_5B84:
        pop    esi
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5B90:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_84F0
        add    esp, 4
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 48h]
        mov    dword ptr [edx + 10h], eax
        mov    dword ptr [edx + 14h], 0
        test   byte ptr [esi + 22h], 8
        je     SHORT loc_3_5BE2
        mov    eax, dword ptr [edx + 8]
        and    eax, 0fffffffdh
        mov    dword ptr [edx + 8], eax
        mov    ecx, dword ptr [esi + 4ch]
        mov    dword ptr [edx + 1ch], ecx
        mov    dword ptr [edx + 18h], 930h
        or     eax, 200h
        mov    dword ptr [edx + 8], eax
        test   byte ptr [esi + 22h], 4
        je     SHORT loc_3_5C12
        or     eax, 40000h
        mov    dword ptr [edx + 8], eax
        jmp    SHORT loc_3_5C12
loc_3_5BE2:
        mov    ecx, dword ptr [esi + 0b4h]
        or     ecx, ecx
        je     SHORT loc_3_5C01
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_3_5C01
        or     dword ptr [edx + 8], 2
        mov    dword ptr [edx + 18h], eax
        add    ecx, 8
        mov    dword ptr [edx + 1ch], ecx
        jmp    SHORT loc_3_5C12
loc_3_5C01:
        and    dword ptr [edx + 8], 0fffffffdh
        mov    dword ptr [edx + 18h], 1
        mov    eax, dword ptr [esi + 4ch]
        mov    dword ptr [edx + 1ch], eax
loc_3_5C12:
        or     dword ptr [esi + 20h], 10000h
        push   esi
        call   sub_3_69B0
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0FFh, 015h, 0B8h, 008h, 000h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_5C3A
        mov    eax, dword ptr [esi + 8]
        or     dword ptr [eax + 8], 40h
loc_3_5C3A:
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_3_57E0
        add    esp, 4
        push   esi
        call   sub_3_68D0
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5C60:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 8]
        mov    ax, word ptr [eax + 6]
        cmp    ax, 10h
        jb     SHORT loc_3_5C99
        db  080h, 03Dh, 0E8h, 09Bh, 000h, 000h, 000h
        je     SHORT loc_3_5C90
        and    eax, 0ffffh
        push   eax
        call   sub_3_57F5
        add    esp, 4
        mov    dword ptr [esi + 24h], eax
        pop    esi
        ret
        db  08Bh, 0FFh
loc_3_5C90:
        mov    dword ptr [esi + 24h], 65h
        pop    esi
        ret
loc_3_5C99:
        test   byte ptr [esi + 23h], 8
        jne    SHORT loc_3_5CC4
        test   byte ptr [esi + 22h], 8
        jne    SHORT loc_3_5CB4
        mov    eax, dword ptr [esi + 0b4h]
        or     eax, eax
        je     SHORT loc_3_5CB4
        cmp    dword ptr [eax], 0
        jne    SHORT loc_3_5CC4
loc_3_5CB4:
        mov    eax, dword ptr [esi + 4ch]
        push   eax
        push   esi
        call   sub_3_7610
        add    esp, 8
        mov    dword ptr [esi + 50h], eax
loc_3_5CC4:
        mov    dword ptr [esi + 24h], 0
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_3_5CD0:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 44h]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 20h]
        sub    ebx, ebx
        mov    esi, dword ptr [eax + 18h]
        lea    eax, [esi + 28h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    edi, dword ptr [eax + 0a4h]
        add    edi, dword ptr [eax + 98h]
        cmp    dword ptr [esp + 10h], ebx
        je     SHORT loc_3_5D2D
        sub    ecx, ecx
        lea    ecx, [ecx]
loc_3_5D10:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        add    eax, ebx
        cmp    edi, eax
        jb     SHORT loc_3_5D2D
        mov    ebx, eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 10h], eax
        cmp    eax, ecx
        jne    SHORT loc_3_5D10
loc_3_5D2D:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_3_5D50
        mov    eax, dword ptr [esp + 20h]
        push   eax
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5D50:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 4], 0
        jne    SHORT loc_3_5D90
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   eax
        call   sub_3_96B0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_5D90
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax + 24h], 15h
        mov    eax, dword ptr [esp + 20h]
        push   eax
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_5D90:
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 14h]
        cmp    dword ptr [ecx + 4], eax
        je     SHORT loc_3_5DC0
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax + 24h], 15h
        mov    eax, dword ptr [esp + 20h]
        push   eax
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_3_5DC0:
        sub    edi, ebx
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esp + 14h], edi
        mov    edi, dword ptr [eax + 0a0h]
        sub    edi, dword ptr [eax + 0a4h]
        jne    SHORT loc_3_5DF0
        push   eax
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5DF0:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 0ch]
        add    ecx, edi
        cmp    ecx, eax
        jbe    SHORT loc_3_5E07
        sub    eax, dword ptr [esp + 14h]
        mov    edi, eax
loc_3_5E07:
        mov    dx, word ptr [esi + 12h]
        test   dx, 850h
        jne    NEAR PTR loc_3_5EC8
        mov    eax, dword ptr [esp + 10h]
        mov    dl, byte ptr [eax + 13h]
        or     dl, dl
        je     SHORT loc_3_5E91
        mov    al, byte ptr [eax + 12h]
        or     al, al
        je     SHORT loc_3_5E91
        xor    ebx, ebx
        mov    bl, al
        mov    cl, byte ptr [esp + 18h]
        shl    ebx, cl
        mov    ebp, edx
        and    ebp, 0ffh
        shl    ebp, cl
        mov    eax, esi
        sub    eax, dword ptr [esp + 10h]
        cmp    eax, -28h
        je     SHORT loc_3_5E50
        sub    esi, esi
        jmp    SHORT loc_3_5E56
        db  005h, 000h, 000h, 000h, 000h
loc_3_5E50:
        movzx  esi, byte ptr [esi + 26h]
        shl    esi, cl
loc_3_5E56:
        add    dword ptr [esp + 14h], esi
        mov    eax, dword ptr [esp + 14h]
        sub    edx, edx
        div    ebx
        mov    ecx, eax
        mov    eax, dword ptr [esp + 14h]
        add    eax, edi
        sub    edx, edx
        div    ebx
        cmp    eax, ecx
        je     SHORT loc_3_5E7C
        lea    edi, [ecx + 1]
        imul   edi, ebx
        sub    edi, dword ptr [esp + 14h]
loc_3_5E7C:
        add    ebp, ebx
        imul   ebp, ecx
        mov    eax, dword ptr [esp + 14h]
        sub    edx, edx
        div    ebx
        add    ebp, edx
        sub    ebp, esi
        mov    dword ptr [esp + 14h], ebp
loc_3_5E91:
        mov    eax, dword ptr [esp + 10h]
        xor    edx, edx
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [esp + 18h]
        shld   edx, eax, cl
        shl    eax, cl
        add    eax, dword ptr [esp + 14h]
        adc    edx, 0
        mov    ecx, eax
        shrd   eax, edx, 0bh
        and    ecx, 7ffh
        mov    edx, dword ptr [esp + 20h]
        mov    dword ptr [edx + 48h], eax
        mov    dword ptr [edx + 0ach], ecx
        jmp    NEAR PTR loc_3_5F60
loc_3_5EC8:
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 0ach], eax
        mov    ebx, 930h
        test   dl, 40h
        je     SHORT loc_3_5F00
        xor    eax, eax
        mov    al, byte ptr [esi + 44h]
        db  08Ah, 08Ch, 080h, 054h, 0A0h, 000h, 000h
        shr    cl, 4
        mov    eax, dword ptr [esp + 20h]
        shr    ebx, cl
        or     dword ptr [eax + 20h], 40000h
        jmp    SHORT loc_3_5F05
        db  08Bh, 0FFh
loc_3_5F00:
        test   dl, 10h
        je     SHORT loc_3_5F10
loc_3_5F05:
        mov    eax, dword ptr [esp + 20h]
        sub    dword ptr [eax + 0ach], 2ch
loc_3_5F10:
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 0ach]
        mov    dword ptr [ecx + 48h], eax
        sub    edx, edx
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [ecx + 0ach]
        add    ecx, 0ach
        div    ebx
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [ecx + 48h]
        add    ecx, 48h
        sub    edx, edx
        div    ebx
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 8]
        add    dword ptr [ecx + 48h], eax
        mov    eax, dword ptr [esp + 20h]
        or     dword ptr [eax + 20h], 80000h
loc_3_5F60:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax + 0a8h], edi
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 0b4h]
        or     eax, eax
        je     SHORT loc_3_5F7E
        mov    dword ptr [eax], 0
loc_3_5F7E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5F90:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
loc_3_5F97:
        push   esi
        call   sub_3_7170
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_3_60D0
        lea    eax, [ebx + 14h]
        mov    dword ptr [esi + 0b0h], eax
        and    dword ptr [esi + 20h], 0fffffeffh
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_3_6138
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    edi, eax
        mov    al, byte ptr [ebx + 1eh]
        or     al, al
        lea    ecx, [eax + 1]
        mov    byte ptr [ebx + 1eh], cl
        db  0A1h, 090h, 008h, 000h, 000h
        jne    SHORT loc_3_5FF7
        push   ebx
        push   edi
        call   sub_3_7A20
        add    esp, 8
        db  0A1h, 090h, 008h, 000h, 000h
loc_3_5FF7:
        inc    dword ptr [eax + 0b8h]
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [esi + 4ch], eax
        push   esi
        call   sub_3_6430
        add    esp, 4
        mov    dword ptr [esi + 4ch], 0
        mov    al, byte ptr [ebx + 1eh]
        dec    al
        mov    byte ptr [ebx + 1eh], al
        jne    SHORT loc_3_607A
        mov    al, byte ptr [ebx + 1ch]
        cmp    al, 3
        jne    SHORT loc_3_6062
        test   byte ptr [ebx + 1fh], 1
        jne    SHORT loc_3_6062
        cmp    al, 3
        jne    SHORT loc_3_6048
        db  0A1h, 01Ch, 09Ch, 000h, 000h
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [edi + 50h]
        cmp    eax, dword ptr [edi + 58h]
        ja     SHORT loc_3_6048
        mov    eax, dword ptr [ebx + 10h]
        sub    eax, dword ptr [edi + 54h]
        cmp    eax, dword ptr [edi + 5ch]
        jbe    SHORT loc_3_6062
loc_3_6048:
        push   ebx
        push   edi
        call   sub_3_7D10
        add    esp, 8
        mov    byte ptr [ebx + 1ch], 2
        push   ebx
        push   edi
        call   sub_3_7D70
        add    esp, 8
        jmp    SHORT loc_3_607A
loc_3_6062:
        mov    al, byte ptr [ebx + 1fh]
        and    al, 1
        cmp    al, 1
        sbb    eax, eax
        and    eax, 4
        inc    eax
        push   eax
        push   ebx
        push   edi
        call   sub_3_7AB0
        add    esp, 0ch
loc_3_607A:
        push   esi
        call   sub_3_59D8
        add    esp, 4
        cmp    dword ptr [esi + 24h], 0
        jne    NEAR PTR loc_3_6172
        mov    eax, dword ptr [esi + 0a4h]
        cmp    dword ptr [esi + 0a0h], eax
        je     NEAR PTR loc_3_6172
        cmp    dword ptr [esi + 0a8h], 0
        je     NEAR PTR loc_3_6147
        and    dword ptr [esi + 20h], 0fffeffffh
        push   esi
        call   sub_3_69B0
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_3_6151
        inc    dword ptr [esi + 48h]
        jmp    NEAR PTR loc_3_5F97
        db  08Dh, 064h, 024h, 000h
loc_3_60D0:
        mov    ecx, dword ptr [esi + 0b4h]
        or     ecx, ecx
        je     SHORT loc_3_6129
        test   byte ptr [esi + 22h], 8
        jne    SHORT loc_3_6129
        cmp    dword ptr [esi + 0a8h], 800h
        jb     SHORT loc_3_6129
        mov    edx, dword ptr [esi + 0a8h]
        shr    edx, 0bh
        cmp    edx, 10h
        jbe    SHORT loc_3_60FF
        mov    edx, 10h
loc_3_60FF:
        mov    eax, dword ptr [esi + 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        cmp    eax, edx
        jae    SHORT loc_3_6111
        mov    edx, eax
loc_3_6111:
        mov    eax, dword ptr [esi + 48h]
        add    eax, edx
        dec    eax
        mov    dword ptr [ecx + 4], eax
        push   4ah
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_6129:
        push   0ah
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_6138:
        push   0ah
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_6147:
        mov    word ptr [esi + 2eh], 38h
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_6151:
        inc    dword ptr [esi + 48h]
        push   esi
        call   sub_3_84F0
        add    esp, 4
        push   esi
        call   sub_3_8580
        add    esp, 4
        mov    ax, word ptr [esi + 2ch]
        mov    word ptr [esi + 2eh], ax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_6172:
        and    dword ptr [esi + 20h], 0fffeffffh
        push   esi
        call   sub_3_69B0
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_3_6190:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0b4h]
        or     eax, eax
        je     SHORT loc_3_61A5
        mov    dword ptr [eax], 0
loc_3_61A5:
        cmp    dword ptr [esi + 24h], 0
        push   esi
        je     SHORT loc_3_61B6
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret
loc_3_61B6:
        call   sub_3_6430
        add    esp, 4
        push   esi
        call   sub_3_75B0
        add    esp, 4
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_3_61F1
        mov    eax, dword ptr [esi + 0a4h]
        cmp    dword ptr [esi + 0a0h], eax
        je     SHORT loc_3_61F1
        cmp    dword ptr [esi + 0a8h], 0
        jne    SHORT loc_3_61EC
        mov    word ptr [esi + 2eh], 38h
        pop    esi
        ret
loc_3_61EC:
        inc    dword ptr [esi + 48h]
        pop    esi
        ret
loc_3_61F1:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_6200:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    ebx, eax
        mov    eax, dword ptr [esi + 0b4h]
        mov    ebp, dword ptr [eax + 4]
        sub    ebp, dword ptr [esi + 48h]
        inc    ebp
        cmp    ebp, 10h
        jbe    SHORT loc_3_622A
        mov    ebp, 10h
loc_3_622A:
        mov    eax, dword ptr [esi + 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        cmp    ebp, eax
        jbe    SHORT loc_3_623C
        mov    ebp, eax
loc_3_623C:
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 50h], 0
        sub    edi, edi
        cmp    ebp, edi
        je     SHORT loc_3_62B7
loc_3_6250:
        push   esi
        call   sub_3_7170
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_62B7
        test   byte ptr [esi + 21h], 10h
        je     SHORT loc_3_6275
        mov    eax, dword ptr [ebx + 1ch]
        cmp    dword ptr [ebx + 2ch], eax
        ja     SHORT loc_3_62B7
        mov    eax, dword ptr [ebx + 18h]
        add    eax, edi
        cmp    eax, dword ptr [ebx + 78h]
        jae    SHORT loc_3_62B7
loc_3_6275:
        mov    eax, dword ptr [esi + 20h]
        and    eax, 8
        push   eax
        push   esi
        call   sub_3_71C0
        add    esp, 8
        mov    dword ptr [esi + 50h], eax
        mov    eax, dword ptr [esi + 50h]
        or     eax, eax
        db  08Bh, 00Dh, 090h, 008h, 000h, 000h
        je     SHORT loc_3_62B0
        cmp    eax, 1
        je     NEAR PTR loc_3_6340
        cmp    eax, 2
        je     SHORT loc_3_6300
        cmp    eax, 4
        je     NEAR PTR loc_3_6357
loc_3_62AC:
        and    dword ptr [esi + 20h], 0ffffffbfh
loc_3_62B0:
        mov    dword ptr [esi + 50h], 0
loc_3_62B7:
        or     edi, edi
        je     NEAR PTR loc_3_6365
        mov    eax, dword ptr [esi + 0b4h]
        mov    ecx, dword ptr [esi + 48h]
        dec    ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], edi
        sub    dword ptr [esi + 48h], edi
        mov    dword ptr [eax + edi*8 + 0ch], 0
        mov    eax, dword ptr [esi + 0b4h]
        push   6
        mov    dword ptr [eax + edi*8 + 8], 0
        mov    dword ptr [esi + 24h], 0
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_3_6300:
        inc    dword ptr [ecx + 0bch]
        mov    ecx, dword ptr [esi + 0b4h]
        mov    eax, dword ptr [esi + 4ch]
        inc    edi
        mov    dword ptr [ecx + edi*8 + 4], eax
        mov    eax, dword ptr [esi + 0b4h]
        mov    dword ptr [eax + edi*8], 1
        mov    dword ptr [esi + 50h], 0
        mov    dword ptr [esi + 4ch], 0
        inc    dword ptr [esi + 48h]
        cmp    ebp, edi
        ja     NEAR PTR loc_3_6250
        jmp    NEAR PTR loc_3_62B7
        db  090h
loc_3_6340:
        or     edi, edi
        jne    NEAR PTR loc_3_62AC
        test   byte ptr [esi + 21h], 10h
        jne    NEAR PTR loc_3_62B7
        jmp    NEAR PTR loc_3_62AC
loc_3_6357:
        push   esi
        call   sub_3_75B0
        add    esp, 4
        jmp    NEAR PTR loc_3_62B7
loc_3_6365:
        test   byte ptr [esi + 21h], 10h
        je     SHORT loc_3_6380
        mov    dword ptr [esi + 24h], 5
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_6380:
        mov    dword ptr [esi + 24h], 0
        push   esi
        call   sub_3_83B0
        add    esp, 4
        push   0ah
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_63A0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        db  0BFh, 070h, 076h, 000h, 000h
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_3_63C1
        db  0BFh, 010h, 076h, 000h, 000h
loc_3_63C1:
        mov    dword ptr [esi + 54h], 0
        mov    eax, dword ptr [esi + 0b4h]
        sub    ebx, ebx
        cmp    dword ptr [eax], ebx
        jbe    SHORT loc_3_6404
        sub    ebp, ebp
loc_3_63D6:
        mov    eax, dword ptr [esi + 0b4h]
        mov    eax, dword ptr [ebp + eax + 0ch]
        mov    dword ptr [esi + 4ch], eax
        push   eax
        push   esi
        call   edi
        add    esp, 8
        mov    dword ptr [esi + 50h], eax
        push   esi
        call   sub_3_75B0
        add    esp, 4
        add    ebp, 8
        mov    eax, dword ptr [esi + 0b4h]
        inc    ebx
        cmp    dword ptr [eax], ebx
        ja     SHORT loc_3_63D6
loc_3_6404:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_3_642B
        test   byte ptr [esi + 21h], 10h
        jne    SHORT loc_3_642B
        mov    eax, dword ptr [esi + 0b4h]
        mov    word ptr [esi + 2eh], 39h
        mov    dword ptr [eax], 0
loc_3_642B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_6430:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 2ch]
        mov    esi, 1
        mov    eax, dword ptr [eax + 14h]
        sub    edx, edx
        mov    eax, dword ptr [eax + 0ch]
        mov    cl, byte ptr [eax + 0ch]
        mov    eax, dword ptr [esp + 2ch]
        shl    esi, cl
        mov    eax, dword ptr [eax + 0a8h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 2ch]
        test   byte ptr [eax + 22h], 8
        je     SHORT loc_3_64A3
        mov    esi, 930h
        mov    ecx, eax
        mov    eax, dword ptr [ecx + 18h]
        add    ecx, 18h
        test   byte ptr [eax + 12h], 40h
        je     SHORT loc_3_64A3
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        db  0F6h, 084h, 080h, 054h, 0A0h, 000h, 000h, 0F0h
        je     SHORT loc_3_64A3
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        db  08Dh, 094h, 080h, 054h, 0A0h, 000h, 000h
        mov    cl, byte ptr [edx]
        shr    cl, 4
        shr    esi, cl
loc_3_64A3:
        mov    eax, dword ptr [esp + 2ch]
        mov    ecx, dword ptr [eax + 0ach]
        add    eax, 0ach
        add    ecx, dword ptr [esp + 10h]
        cmp    ecx, esi
        jbe    SHORT loc_3_64C0
        sub    esi, dword ptr [eax]
        mov    dword ptr [esp + 10h], esi
loc_3_64C0:
        or     edx, edx
        jne    SHORT loc_3_650C
        mov    eax, dword ptr [esp + 2ch]
        mov    esi, dword ptr [eax + 4ch]
        mov    edi, dword ptr [eax + 9ch]
        mov    ecx, dword ptr [esp + 10h]
        add    esi, dword ptr [eax + 0ach]
        add    edi, dword ptr [eax + 0a4h]
        db  0E8h, 0BBh, 09Bh, 0FCh, 0FFh
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esp + 10h], eax
        je     NEAR PTR loc_3_65D7
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [eax + 24h], 8
        jmp    NEAR PTR loc_3_65D7
loc_3_650C:
        mov    cl, byte ptr [edx]
        mov    al, cl
        and    al, 2
        shr    al, 1
        and    eax, 0ffh
        and    ecx, 1
        add    ecx, eax
        mov    eax, 4
        sar    eax, cl
        mov    dword ptr [esp + 0ch], eax
        dec    eax
        mov    dword ptr [esp + 1ch], eax
        mov    eax, 4
        mov    cl, byte ptr [edx]
        and    cl, 0ch
        shr    cl, 2
        shl    eax, cl
        dec    eax
        not    eax
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [edx + 1]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    esi, dword ptr [eax + 0ach]
        and    esi, dword ptr [esp + 24h]
        add    esi, dword ptr [eax + 4ch]
        mov    edi, dword ptr [eax + 0a4h]
        mov    ebx, edi
        and    ebx, dword ptr [esp + 1ch]
        add    edi, dword ptr [eax + 9ch]
        mov    ecx, dword ptr [esp + 10h]
        jecxz  loc_3_65D7
        or     ebx, ebx
        mov    ebx, dword ptr [esp + 20h]
        je     SHORT loc_3_65A9
        mov    eax, dword ptr [esi]
        call   ebx
        mov    dword ptr [esp + 18h], eax
        mov    eax, esi
        lea    esi, [esp + 18h]
        mov    edx, ecx
        and    ecx, dword ptr [esp + 1ch]
        sub    edx, ecx
        rep movsb
        mov    ecx, edx
        mov    esi, eax
        jmp    SHORT loc_3_65A9
loc_3_65A2:
        mov    dword ptr [edi], eax
        add    edi, 2
loc_3_65A7:
        inc    edi
loc_3_65A8:
        inc    edi
loc_3_65A9:
        sub    ecx, dword ptr [esp + 0ch]
        jb     SHORT loc_3_65C3
        mov    eax, dword ptr [esi]
        call   ebx
        cmp    dword ptr [esp + 0ch], 2
        jg     SHORT loc_3_65A2
        mov    byte ptr [edi], al
        jl     SHORT loc_3_65A8
        mov    word ptr [edi], ax
        jmp    SHORT loc_3_65A7
loc_3_65C3:
        add    ecx, dword ptr [esp + 0ch]
        je     SHORT loc_3_65D7
        mov    eax, dword ptr [esi]
        call   ebx
        mov    dword ptr [esp + 18h], eax
        lea    esi, [esp + 18h]
        rep movsb
loc_3_65D7:
        mov    eax, dword ptr [esp + 2ch]
        mov    ecx, dword ptr [esp + 14h]
        sub    dword ptr [eax + 0a8h], ecx
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, dword ptr [esp + 14h]
        add    dword ptr [ecx + 0a4h], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [eax + 0ach], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_6610:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0a4h]
        cmp    dword ptr [esi + 0a0h], eax
        ja     SHORT loc_3_6630
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret
        db  08Bh, 0FFh
loc_3_6630:
        mov    eax, dword ptr [esi + 0a4h]
        mov    dword ptr [esi + 48h], eax
        test   byte ptr [esi + 22h], 8
        jne    SHORT loc_3_6645
        shr    dword ptr [esi + 48h], 0bh
        jmp    SHORT loc_3_6654
loc_3_6645:
        mov    ecx, 930h
        mov    eax, dword ptr [esi + 48h]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esi + 48h], eax
loc_3_6654:
        mov    eax, dword ptr [esi + 98h]
        add    dword ptr [esi + 48h], eax
        push   0ah
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_6670:
        sub    esp, 8
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 8], 800h
        cmp    dword ptr [eax + 24h], 0
        je     SHORT loc_3_6696
        push   eax
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_3_6696:
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax + 22h], 8
        je     SHORT loc_3_66A8
        mov    dword ptr [esp + 8], 930h
loc_3_66A8:
        mov    eax, dword ptr [esp + 14h]
        mov    esi, dword ptr [eax + 4ch]
        mov    edi, dword ptr [eax + 9ch]
        add    edi, dword ptr [eax + 0a4h]
        mov    ecx, dword ptr [esp + 8]
        db  0E8h, 0DDh, 099h, 0FCh, 0FFh
        mov    dword ptr [esp + 0ch], eax
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 0ch]
        add    dword ptr [eax + 0a4h], ecx
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [esp + 0ch], eax
        jae    SHORT loc_3_66F8
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 24h], 8
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_3_83B0
        add    esp, 4
loc_3_66F8:
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_3_6700:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 8], 0
        je     SHORT loc_3_671B
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2ch]
        cmp    ecx, 6
        jne    SHORT loc_3_671B
        mov    word ptr [eax + 2eh], 8
loc_3_671B:
        ret
        db  08Dh, 064h, 024h, 000h
sub_3_6720:
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [esp + 18h]
        mov    word ptr [esp + 4], 2
        mov    eax, dword ptr [esi + 4]
        mov    cx, word ptr [eax + 8]
        shl    cx, 3
        add    cx, word ptr [eax + 0ah]
        mov    word ptr [esp + 8], cx
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 0ah], eax
        mov    eax, dword ptr [esi + 20h]
        and    eax, 1080h
        cmp    eax, 1
        sbb    al, al
        and    al, 1
        inc    al
        mov    byte ptr [esp + 12h], al
        lea    eax, [esp + 4]
        push   eax
        db  0FFh, 015h, 0A8h, 008h, 000h, 000h
        add    esp, 4
        cmp    dword ptr [esp + 0eh], 0
        jne    SHORT loc_3_6796
        mov    dword ptr [esi + 24h], 8
        test   byte ptr [esi + 21h], 10h
        jne    NEAR PTR loc_3_6819
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        add    esp, 10h
        ret
loc_3_6796:
        mov    ecx, dword ptr [esp + 0ah]
        add    ecx, dword ptr [esp + 0eh]
        mov    dword ptr [esi + 8], ecx
        lea    eax, [ecx + 58h]
        mov    dword ptr [ecx + 48h], eax
        mov    dword ptr [ecx], 0
        mov    word ptr [ecx + 4], 0
        mov    word ptr [ecx + 6], 0
        mov    dword ptr [ecx + 8], 401h
        db  0C7h, 041h, 00Ch, 080h, 085h, 000h, 000h
        mov    dword ptr [ecx + 40h], esi
        mov    dword ptr [ecx + 10h], 0
        mov    dword ptr [ecx + 14h], 0
        mov    dword ptr [ecx + 18h], 0
        mov    byte ptr [ecx + 4ch], 0
        mov    eax, dword ptr [esp + 0ah]
        mov    dword ptr [ecx + 20h], eax
        mov    al, byte ptr [esi + 40h]
        mov    byte ptr [ecx + 4dh], al
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 44h], eax
        mov    al, byte ptr [esi + 6fh]
        test   al, 1
        je     SHORT loc_3_6801
        or     dword ptr [ecx + 8], 8
loc_3_6801:
        test   al, 2
        je     SHORT loc_3_680C
        or     dword ptr [ecx + 8], 20000h
loc_3_680C:
        test   byte ptr [esi + 20h], 80h
        je     SHORT loc_3_6819
        or     dword ptr [ecx + 8], 800h
loc_3_6819:
        pop    esi
        add    esp, 10h
        ret
        db  08Bh, 0FFh
sub_3_6820:
        sub    esp, 8
        mov    eax, dword ptr [esp + 0ch]
        cmp    dword ptr [eax + 8], 0
        je     SHORT loc_3_6856
        mov    word ptr [esp], 4
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 8]
        sub    ecx, dword ptr [edx + 0ch]
        mov    dword ptr [esp + 4], ecx
        mov    dword ptr [eax + 8], 0
        lea    eax, [esp]
        push   eax
        db  0FFh, 015h, 0A8h, 008h, 000h, 000h
        add    esp, 4
loc_3_6856:
        add    esp, 8
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_6860:
        sub    esp, 4
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_3_6870
        sub    eax, eax
        add    esp, 4
        ret
loc_3_6870:
        mov    eax, 20h
        mov    edx, dword ptr [esp + 8]
loc_3_6879:
        dec    eax
        shl    edx, 1
        jae    SHORT loc_3_6879
        mov    dword ptr [esp], eax
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_6890:
        sub    esp, 4
        push   ebx
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        shr    ecx, 2
loc_3_68A3:
        add    bl, byte ptr [edx + ecx*4 - 4]
        add    bh, byte ptr [edx + ecx*4 - 3]
        add    al, byte ptr [edx + ecx*4 - 2]
        add    ah, byte ptr [edx + ecx*4 - 1]
        dec    ecx
        jne    SHORT loc_3_68A3
        shl    eax, 10h
        mov    ax, bx
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [esp + 4]
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_68D0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        test   dword ptr [esi + 20h], 40008h
        jne    NEAR PTR loc_3_6996
        db  083h, 03Dh, 0F8h, 09Bh, 000h, 000h, 000h
        jne    NEAR PTR loc_3_6996
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_3_6996
        mov    eax, dword ptr [edi + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     SHORT loc_3_6989
        sub    ebx, ebx
loc_3_6918:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [edi + 3ch], eax
        je     SHORT loc_3_6989
        mov    eax, dword ptr [eax + 20h]
        mov    ebp, eax
        sub    ebp, dword ptr [edi + 68h]
        and    ebp, 0fffff1ffh
        shr    ebp, 9
        add    ebp, dword ptr [edi + 4]
        cmp    word ptr [ebp + 6], bx
        jne    SHORT loc_3_697A
        push   ebx
        push   1
        shr    eax, 0ch
        push   eax
        call   sub_3_5150
        add    esp, 0ch
        cmp    eax, ebx
        je     SHORT loc_3_69A0
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 20h]
        and    eax, 0fffff000h
        push   eax
        call   sub_3_51B0
        add    esp, 4
        mov    dword ptr [ebp], eax
        inc    word ptr [ebp + 6]
        test   byte ptr [esi + 20h], 2
        jne    SHORT loc_3_6996
        mov    eax, dword ptr [edi + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 10h], eax
loc_3_697A:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 10h], eax
        cmp    eax, ebx
        jne    SHORT loc_3_6918
loc_3_6989:
        db  0A1h, 0F8h, 09Bh, 000h, 000h
        or     eax, 1
        db  0A3h, 0F8h, 09Bh, 000h, 000h
loc_3_6996:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_3_69A0:
        call   sub_3_7F30
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_3_69B0:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        mov    dword ptr [esp + 10h], 0
        test   byte ptr [esi + 21h], 50h
        jne    NEAR PTR loc_3_6C61
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_3_69E6
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_3_69E6:
        mov    edx, dword ptr [edi + 2ch]
        cmp    dword ptr [edi + 28h], edx
        ja     SHORT loc_3_69F8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_3_69F8:
        test   byte ptr [esi + 22h], 1
        jne    NEAR PTR loc_3_6AF0
        db  083h, 03Dh, 080h, 09Ch, 000h, 000h, 000h
        je     NEAR PTR loc_3_6AF0
        db  0C7h, 044h, 024h, 018h, 08Ch, 09Ch, 000h, 000h
        db  0F6h, 005h, 0ADh, 09Ch, 000h, 000h, 020h
        jne    NEAR PTR loc_3_6AB0
        db  0A1h, 0E8h, 09Ch, 000h, 000h
        db  039h, 005h, 0D4h, 09Ch, 000h, 000h
        jae    SHORT loc_3_6AB0
        mov    ecx, dword ptr [edi + 1ch]
        cmp    edx, ecx
        ja     SHORT loc_3_6AB0
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [edi + 78h], eax
        jbe    SHORT loc_3_6AB0
        db  08Bh, 01Dh, 074h, 09Ch, 000h, 000h
        cmp    dword ptr [ebx + 10h], 0
        je     SHORT loc_3_6A78
        cmp    dword ptr [ebx + 14h], eax
        jae    SHORT loc_3_6A78
        test   byte ptr [esi + 22h], 8
        jne    SHORT loc_3_6A78
        mov    eax, dword ptr [esi + 14h]
        mov    edi, dword ptr [ebx + 18h]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        cmp    eax, edi
        jb     SHORT loc_3_6A78
        lea    eax, [edx + edi]
        cmp    eax, ecx
        ja     NEAR PTR loc_3_6C9F
loc_3_6A78:
        db  0A1h, 01Ch, 09Ch, 000h, 000h
        db  081h, 00Dh, 0ACh, 09Ch, 000h, 000h, 000h, 020h, 000h, 000h
        mov    eax, dword ptr [eax]
        db  0A3h, 078h, 09Ch, 000h, 000h
        db  0E8h, 0ADh, 098h, 0FCh, 0FFh
        db  068h, 08Ch, 09Ch, 000h, 000h
        call   sub_3_8580
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_3_6AB0:
        mov    eax, dword ptr [edi + 4ch]
        cmp    dword ptr [esi + 48h], eax
        jne    NEAR PTR loc_3_6C9F
        db  0A1h, 01Ch, 09Ch, 000h, 000h
        db  08Bh, 01Dh, 074h, 09Ch, 000h, 000h
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [edi + 48h]
        cmp    eax, dword ptr [ebx + 20h]
        jae    NEAR PTR loc_3_6C9F
        cmp    byte ptr [esi + 63h], 5
        jne    NEAR PTR loc_3_6C9F
        mov    dword ptr [edi + 4ch], 0ffffffffh
        mov    dword ptr [esp + 14h], 1
        jmp    SHORT loc_3_6AF8
loc_3_6AF0:
        mov    dword ptr [esp + 14h], 0
loc_3_6AF8:
        mov    eax, dword ptr [esi + 0b4h]
        or     eax, eax
        je     SHORT loc_3_6B08
        mov    ebx, dword ptr [eax]
        or     ebx, ebx
        jne    SHORT loc_3_6B0D
loc_3_6B08:
        mov    ebx, 1
loc_3_6B0D:
        add    ebx, dword ptr [esi + 48h]
        xor    eax, eax
        mov    al, byte ptr [esi + 63h]
        cmp    eax, 5
        je     SHORT loc_3_6B24
        cmp    eax, 6
        je     SHORT loc_3_6B70
        jmp    NEAR PTR loc_3_6BC0
loc_3_6B24:
        cmp    dword ptr [esi + 18h], 0
        je     NEAR PTR loc_3_6BC0
        mov    edx, dword ptr [esi + 18h]
        mov    ebp, dword ptr [esi + 14h]
        add    edx, 28h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 0ch]
        sub    cl, byte ptr [ebp + 44h]
loc_3_6B40:
        mov    eax, dword ptr [edx + 8]
        shr    eax, cl
        cmp    eax, ebx
        ja     SHORT loc_3_6B61
        mov    ebp, dword ptr [edx + 0ch]
        shr    ebp, 0bh
        add    eax, ebp
        cmp    eax, ebx
        jb     SHORT loc_3_6B61
        cmp    byte ptr [edx + 12h], 0
        je     SHORT loc_3_6B69
        cmp    byte ptr [edx + 13h], 0
        je     SHORT loc_3_6B69
loc_3_6B61:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_3_6B40
        jmp    SHORT loc_3_6BC0
loc_3_6B69:
        mov    dword ptr [esp + 10h], eax
        jmp    SHORT loc_3_6BC0
        db  090h
loc_3_6B70:
        test   byte ptr [esi + 23h], 1
        je     SHORT loc_3_6B90
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [esi + 1ch]
        mov    eax, dword ptr [eax + 20h]
        lea    eax, [ecx + eax - 1]
        mov    dword ptr [esp + 10h], eax
        jmp    SHORT loc_3_6BC0
        db  08Dh, 064h, 024h, 000h
loc_3_6B90:
        mov    ecx, dword ptr [esi + 14h]
        or     ecx, ecx
        je     SHORT loc_3_6BC0
        cmp    dword ptr [ecx + 24h], 0
        je     SHORT loc_3_6BC0
        cmp    dword ptr [ecx + 28h], 0
        je     SHORT loc_3_6BC0
        mov    eax, dword ptr [ecx + 24h]
        mov    edx, dword ptr [ecx + 28h]
        lea    eax, [eax + edx - 1]
        mov    dword ptr [esp + 10h], eax
        sub    eax, ebx
        cmp    eax, dword ptr [ecx + 28h]
        jbe    SHORT loc_3_6BC0
        mov    dword ptr [esp + 10h], 0
loc_3_6BC0:
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_3_6C9F
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_6BDC
        mov    eax, dword ptr [esp + 18h]
        test   byte ptr [eax + 21h], 20h
        jne    SHORT loc_3_6C54
loc_3_6BDC:
        mov    eax, dword ptr [esp + 10h]
        db  03Bh, 005h, 084h, 09Ch, 000h, 000h
        jne    SHORT loc_3_6C16
        db  039h, 01Dh, 07Ch, 09Ch, 000h, 000h
        ja     SHORT loc_3_6C16
        db  0A1h, 07Ch, 09Ch, 000h, 000h
        add    eax, 10h
        cmp    eax, ebx
        jb     SHORT loc_3_6C16
        db  0A1h, 080h, 09Ch, 000h, 000h
        cmp    dword ptr [esi + 14h], eax
        jne    SHORT loc_3_6C16
        mov    eax, dword ptr [esi + 20h]
        db  033h, 005h, 088h, 09Ch, 000h, 000h
        test   eax, 0c0000h
        je     SHORT loc_3_6C25
loc_3_6C16:
        cmp    dword ptr [esp + 14h], 0
        mov    byte ptr [edi + 43h], 4
        je     SHORT loc_3_6C25
        mov    byte ptr [edi + 43h], 5
loc_3_6C25:
        push   edi
        call   sub_3_7E10
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        push   eax
        push   ebx
        push   esi
        call   sub_3_6CB0
        add    esp, 0ch
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_3_6C54
        db  0A1h, 01Ch, 09Ch, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi + 50h], eax
        mov    eax, dword ptr [esi + 48h]
        mov    dword ptr [edi + 54h], eax
loc_3_6C54:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_3_6C61:
        test   byte ptr [esi + 21h], 10h
        je     SHORT loc_3_6C9F
        db  0A1h, 080h, 09Ch, 000h, 000h
        cmp    dword ptr [esi + 14h], eax
        jne    SHORT loc_3_6C9F
        db  0A1h, 07Ch, 09Ch, 000h, 000h
        cmp    dword ptr [esi + 48h], eax
        jne    SHORT loc_3_6C9F
        test   byte ptr [esi + 22h], 8
        je     SHORT loc_3_6C91
        db  0FFh, 005h, 07Ch, 09Ch, 000h, 000h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_3_6C91:
        mov    eax, dword ptr [esi + 0b4h]
        mov    eax, dword ptr [eax]
        db  001h, 005h, 07Ch, 09Ch, 000h, 000h
loc_3_6C9F:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_6CB0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 14h]
        db  0A3h, 080h, 09Ch, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        db  0A3h, 07Ch, 09Ch, 000h, 000h
        mov    eax, dword ptr [esp + 0ch]
        db  0A3h, 084h, 09Ch, 000h, 000h
        mov    eax, dword ptr [ecx + 20h]
        db  0A3h, 088h, 09Ch, 000h, 000h
        push   ecx
        call   sub_3_78F0
        add    esp, 4
        db  0F6h, 005h, 0ADh, 09Ch, 000h, 000h, 020h
        jne    SHORT loc_3_6D10
        db  0A1h, 01Ch, 09Ch, 000h, 000h
        db  081h, 00Dh, 0ACh, 09Ch, 000h, 000h, 000h, 020h, 000h, 000h
        mov    eax, dword ptr [eax]
        db  0A3h, 078h, 09Ch, 000h, 000h
        db  0E8h, 03Dh, 096h, 0FCh, 0FFh
        db  068h, 08Ch, 09Ch, 000h, 000h
        call   sub_3_8580
        add    esp, 4
loc_3_6D10:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_6D20:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 24h], 0
        db  0A1h, 01Ch, 09Ch, 000h, 000h
        mov    eax, dword ptr [eax]
        db  0A3h, 078h, 09Ch, 000h, 000h
        db  0A1h, 080h, 09Ch, 000h, 000h
        cmp    dword ptr [esi + 14h], eax
        jne    SHORT loc_3_6D53
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_3_6D53
        cmp    dword ptr [esi + 4], 0
        jne    NEAR PTR loc_3_6E38
loc_3_6D53:
        push   esi
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [esi + 4]
        lea    edi, [esi + 4]
        or     eax, eax
        je     SHORT loc_3_6D79
        push   eax
        mov    eax, dword ptr [esi + 40h]
        push   eax
        call   sub_3_31B6
        add    esp, 8
        mov    dword ptr [edi], 0
loc_3_6D79:
        db  0A1h, 080h, 09Ch, 000h, 000h
        mov    dword ptr [esi + 14h], eax
        db  0A1h, 07Ch, 09Ch, 000h, 000h
        mov    dword ptr [esi + 48h], eax
        and    dword ptr [esi + 20h], 0fff3ffffh
        db  0A1h, 088h, 09Ch, 000h, 000h
        and    eax, 0c0000h
        or     dword ptr [esi + 20h], eax
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_3_6E0F
        inc    word ptr [eax + 1ch]
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [esi + 40h], edx
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 44h], ecx
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 1ch]
        sub    ecx, 0c3h
        test   byte ptr [ecx + 21h], 10h
        jne    SHORT loc_3_6DF2
        cmp    dword ptr [ecx + 18h], eax
        jne    SHORT loc_3_6DF2
        push   1
        push   edi
        push   edx
        call   sub_3_31B0
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_6DF2
        mov    ecx, dword ptr [esi + 14h]
        mov    eax, dword ptr [edi]
        cmp    dword ptr [ecx + 4], eax
        je     SHORT loc_3_6DF8
        push   eax
        mov    eax, dword ptr [esi + 40h]
        push   eax
        call   sub_3_31B6
        add    esp, 8
loc_3_6DF2:
        mov    dword ptr [edi], 0
loc_3_6DF8:
        push   esi
        call   sub_3_4660
        add    esp, 4
        cmp    dword ptr [edi], 0
        je     SHORT loc_3_6E0F
        push   esi
        call   sub_3_6720
        add    esp, 4
loc_3_6E0F:
        mov    ecx, dword ptr [esi + 14h]
        or     ecx, ecx
        je     NEAR PTR loc_3_6F30
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     NEAR PTR loc_3_6F30
        or     dword ptr [eax + 8], 20000h
        db  03Bh, 00Dh, 080h, 09Ch, 000h, 000h
        jne    NEAR PTR loc_3_6F37
loc_3_6E38:
        db  0A1h, 07Ch, 09Ch, 000h, 000h
        mov    dword ptr [esi + 48h], eax
        db  0A1h, 084h, 09Ch, 000h, 000h
        mov    dword ptr [esi + 5ch], eax
        and    dword ptr [esi + 20h], 0fff3ffffh
        db  0A1h, 088h, 09Ch, 000h, 000h
        and    eax, 0c0000h
        or     dword ptr [esi + 20h], eax
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_3_6E75
        mov    dword ptr [esi + 24h], 5
        pop    edi
        pop    esi
        ret
loc_3_6E75:
        mov    eax, dword ptr [esi + 5ch]
        cmp    dword ptr [esi + 48h], eax
        ja     SHORT loc_3_6EB1
loc_3_6E7D:
        push   esi
        call   sub_3_7170
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_6EB1
        inc    dword ptr [esi + 48h]
        mov    ecx, dword ptr [esi + 5ch]
        cmp    dword ptr [esi + 48h], ecx
        ja     SHORT loc_3_6EB1
loc_3_6E95:
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     SHORT loc_3_6EAC
        mov    edx, dword ptr [esi + 48h]
        cmp    dword ptr [eax + 10h], edx
        jne    SHORT loc_3_6EAC
        inc    dword ptr [esi + 48h]
        cmp    dword ptr [esi + 48h], ecx
        jbe    SHORT loc_3_6E95
loc_3_6EAC:
        cmp    dword ptr [esi + 48h], ecx
        jbe    SHORT loc_3_6E7D
loc_3_6EB1:
        mov    eax, dword ptr [esi + 48h]
        db  0A3h, 07Ch, 09Ch, 000h, 000h
        mov    ecx, dword ptr [esi + 5ch]
        cmp    dword ptr [esi + 48h], ecx
        ja     SHORT loc_3_6F25
        mov    eax, dword ptr [edi + 1ch]
        cmp    dword ptr [edi + 2ch], eax
        ja     SHORT loc_3_6F25
        mov    eax, dword ptr [edi + 78h]
        cmp    dword ptr [edi + 18h], eax
        jae    SHORT loc_3_6F25
        sub    ecx, dword ptr [esi + 48h]
        db  0A1h, 074h, 09Ch, 000h, 000h
        inc    ecx
        mov    eax, dword ptr [eax + 2ch]
        cmp    ecx, eax
        jbe    SHORT loc_3_6EE3
        mov    ecx, eax
loc_3_6EE3:
        test   byte ptr [esi + 22h], 8
        je     SHORT loc_3_6EEE
        mov    ecx, 1
loc_3_6EEE:
        mov    eax, dword ptr [esi + 0b4h]
        mov    edx, dword ptr [esi + 48h]
        add    edx, ecx
        dec    edx
        mov    dword ptr [eax + 4], edx
        test   byte ptr [esi + 22h], 8
        je     SHORT loc_3_6F11
        push   0ah
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_3_6F11:
        mov    dword ptr [eax], 0
        push   4ah
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_3_6F25:
        mov    dword ptr [esi + 24h], 5
        pop    edi
        pop    esi
        ret
        db  090h
loc_3_6F30:
        mov    dword ptr [esi + 24h], 5
loc_3_6F37:
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_6F40:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 22h], 8
        je     SHORT loc_3_6F54
        push   esi
        call   sub_3_75B0
        add    esp, 4
loc_3_6F54:
        cmp    dword ptr [esi + 24h], 0
        je     NEAR PTR loc_3_7015
        mov    dword ptr [esi + 24h], 0
        cmp    dword ptr [esi + 50h], 1
        jne    SHORT loc_3_6FDA
        mov    dword ptr [esi + 50h], 0
        db  0F6h, 005h, 084h, 008h, 000h, 000h, 002h
        jne    SHORT loc_3_6FDA
        db  080h, 00Dh, 084h, 008h, 000h, 000h, 002h
        db  080h, 025h, 084h, 008h, 000h, 000h, 0FBh
        db  083h, 025h, 004h, 000h, 000h, 000h, 0EFh
        db  083h, 00Dh, 004h, 000h, 000h, 000h, 040h
        db  0E8h, 0A4h, 093h, 0FCh, 0FFh
        db  0F6h, 005h, 004h, 000h, 000h, 000h, 020h
        jne    SHORT loc_3_6FC8
        db  083h, 00Dh, 004h, 000h, 000h, 000h, 020h
        db  080h, 00Dh, 084h, 008h, 000h, 000h, 008h
        push   esi
        db  068h, 020h, 000h, 000h, 000h
        db  0E8h, 032h, 098h, 0FCh, 0FFh
        add    esp, 8
        db  080h, 025h, 084h, 008h, 000h, 000h, 0F7h
loc_3_6FC8:
        db  0F6h, 005h, 084h, 008h, 000h, 000h, 004h
        je     SHORT loc_3_6FDA
        db  080h, 025h, 084h, 008h, 000h, 000h, 0FBh
        pop    esi
        ret
loc_3_6FDA:
        db  0E8h, 041h, 093h, 0FCh, 0FFh
        push   esi
        call   sub_3_84F0
        add    esp, 4
        push   esi
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_3_700C
        push   eax
        mov    eax, dword ptr [esi + 40h]
        push   eax
        call   sub_3_31B6
        add    esp, 8
        mov    dword ptr [esi + 4], 0
loc_3_700C:
        and    dword ptr [esi + 20h], 0ffffdfffh
        pop    esi
        ret
loc_3_7015:
        db  0A1h, 080h, 09Ch, 000h, 000h
        cmp    dword ptr [esi + 14h], eax
        jne    SHORT loc_3_7045
        db  0A1h, 07Ch, 09Ch, 000h, 000h
        cmp    dword ptr [esi + 48h], eax
        jne    SHORT loc_3_7045
        test   byte ptr [esi + 22h], 8
        je     SHORT loc_3_7037
        db  0FFh, 005h, 07Ch, 09Ch, 000h, 000h
        pop    esi
        ret
loc_3_7037:
        mov    eax, dword ptr [esi + 0b4h]
        mov    eax, dword ptr [eax]
        db  001h, 005h, 07Ch, 09Ch, 000h, 000h
loc_3_7045:
        pop    esi
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_7050:
        push   ebx
        push   esi
        push   edi
        db  0F6h, 005h, 084h, 008h, 000h, 000h, 008h
        jne    SHORT loc_3_7083
        db  0F6h, 005h, 084h, 008h, 000h, 000h, 001h
        jne    NEAR PTR loc_3_714A
        db  0F6h, 005h, 0F8h, 09Bh, 000h, 000h, 002h
        jne    NEAR PTR loc_3_714A
        db  0F6h, 005h, 0ADh, 09Ch, 000h, 000h, 020h
        jne    NEAR PTR loc_3_714A
loc_3_7083:
        db  068h, 08Ch, 09Ch, 000h, 000h
        call   sub_3_7EF0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_3_714A
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [eax + 4]
        or     ecx, ecx
        je     NEAR PTR loc_3_714A
        sub    eax, eax
loc_3_70AA:
        cmp    byte ptr [ecx + 1dh], al
        jne    SHORT loc_3_70CA
        mov    ebx, dword ptr [ecx + 20h]
        and    ebx, 0fffff000h
        mov    edi, ebx
        sub    edi, dword ptr [esi + 68h]
        mov    edx, dword ptr [esi + 4]
        shr    edi, 0ch
        cmp    word ptr [edx + edi*8 + 6], ax
        je     SHORT loc_3_70D3
loc_3_70CA:
        mov    ecx, dword ptr [ecx + 4]
        cmp    ecx, eax
        jne    SHORT loc_3_70AA
        jmp    SHORT loc_3_714A
loc_3_70D3:
        push   0
        push   1
        mov    eax, ebx
        shr    eax, 0ch
        push   eax
        call   sub_3_5150
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_714A
        push   ebx
        call   sub_3_51B0
        add    esp, 4
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ecx + edi*8], eax
        mov    eax, dword ptr [esi + 4]
        inc    word ptr [eax + edi*8 + 6]
        db  080h, 00Dh, 084h, 008h, 000h, 000h, 004h
        db  0F6h, 005h, 084h, 008h, 000h, 000h, 008h
        jne    SHORT loc_3_714A
        db  0F6h, 005h, 0ADh, 09Ch, 000h, 000h, 020h
        jne    SHORT loc_3_714A
        db  0A1h, 0E8h, 09Ch, 000h, 000h
        db  039h, 005h, 0D4h, 09Ch, 000h, 000h
        jae    SHORT loc_3_714A
        mov    eax, dword ptr [esi + 1ch]
        cmp    dword ptr [esi + 2ch], eax
        ja     SHORT loc_3_714A
        db  081h, 00Dh, 0ACh, 09Ch, 000h, 000h, 000h, 020h, 000h, 000h
        db  0E8h, 003h, 092h, 0FCh, 0FFh
        db  068h, 08Ch, 09Ch, 000h, 000h
        call   sub_3_8580
        add    esp, 4
loc_3_714A:
        db  0E8h, 0D1h, 091h, 0FCh, 0FFh
        db  083h, 025h, 004h, 000h, 000h, 000h, 0BFh
        db  080h, 025h, 084h, 008h, 000h, 000h, 0FDh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7170:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_7186
        sub    eax, eax
        pop    esi
        ret
loc_3_7186:
        mov    ecx, dword ptr [esi + 44h]
        mov    ecx, dword ptr [ecx + 8]
        mov    edx, dword ptr [esi + 48h]
        lea    esi, [ecx + edx]
        and    esi, dword ptr [eax + 6ch]
        mov    eax, dword ptr [eax + esi*4 + 80h]
        or     eax, eax
        je     SHORT loc_3_71B1
loc_3_71A0:
        cmp    dword ptr [eax + 10h], edx
        jne    SHORT loc_3_71AA
        cmp    dword ptr [eax + 0ch], ecx
        je     SHORT loc_3_71B1
loc_3_71AA:
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    SHORT loc_3_71A0
loc_3_71B1:
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_71C0:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        add    eax, 0
loc_3_71D0:
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_3_7220
        cmp    dword ptr [esp + 28h], 0
        jne    SHORT loc_3_7210
        mov    eax, dword ptr [esi + 20h]
        and    eax, 80000h
        push   eax
        call   sub_3_00B0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_71D0
        mov    dword ptr [esi + 24h], 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_7210:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_3_7220:
        cmp    dword ptr [esi + 50h], 3
        jne    SHORT loc_3_7249
        mov    eax, edi
        sub    eax, dword ptr [esi + 0b0h]
        cmp    eax, -70h
        je     SHORT loc_3_7249
        test   byte ptr [esi + 21h], 1
        je     SHORT loc_3_7240
        or     dword ptr [esi + 20h], 600h
loc_3_7240:
        push   esi
        call   sub_3_59D8
        add    esp, 4
loc_3_7249:
        mov    eax, dword ptr [esi + 44h]
        mov    ebp, dword ptr [eax + 8]
        mov    eax, dword ptr [esi + 48h]
        mov    dword ptr [esp + 10h], eax
        add    eax, ebp
        mov    dword ptr [esi + 4ch], 0
        mov    dword ptr [esi + 54h], 0
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [edi + 6ch]
        and    eax, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [edi + eax*4 + 80h]
        or     ebx, ebx
        je     NEAR PTR loc_3_7463
loc_3_7280:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [ebx + 10h], eax
        jne    SHORT loc_3_728E
        cmp    dword ptr [ebx + 0ch], ebp
        je     SHORT loc_3_7295
loc_3_728E:
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        jne    SHORT loc_3_7280
loc_3_7295:
        or     ebx, ebx
        je     NEAR PTR loc_3_7463
        test   byte ptr [esi + 21h], 10h
        je     SHORT loc_3_72B4
        mov    dword ptr [esi + 24h], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_3_72B4:
        lea    eax, [ebx + 14h]
        mov    dword ptr [esi + 0b0h], eax
        and    dword ptr [esi + 20h], 0fffffeffh
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_72E0
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_3_72E0:
        mov    ecx, dword ptr [ebx + 20h]
        mov    eax, ecx
        and    eax, 0fffff000h
        mov    dword ptr [esp + 14h], eax
        mov    ebp, eax
        sub    ebp, dword ptr [edi + 68h]
        shr    ebp, 0ch
        mov    al, byte ptr [ebx + 1dh]
        or     al, al
        mov    edx, dword ptr [edi + 4]
        jne    NEAR PTR loc_3_7446
        lea    edx, [edx + ebp*8]
        cmp    word ptr [edx + 6], 0
        je     SHORT loc_3_7340
        inc    word ptr [edx + 4]
        inc    al
        mov    byte ptr [ebx + 1dh], al
        mov    dword ptr [esi + 4ch], ecx
        mov    al, byte ptr [ebx + 1eh]
        or     al, al
        lea    ecx, [eax + 1]
        mov    byte ptr [ebx + 1eh], cl
        jne    SHORT loc_3_7331
        push   ebx
        push   edi
        call   sub_3_7A20
        add    esp, 8
loc_3_7331:
        mov    eax, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_3_7340:
        cmp    dword ptr [esp + 28h], 0
        push   esi
        je     SHORT loc_3_7360
        call   sub_3_59D8
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_3_7360:
        call   sub_3_59D8
        add    esp, 4
        or     dword ptr [esi + 20h], 100h
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_7389
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_3_7389:
        push   0
        mov    eax, dword ptr [esp + 18h]
        push   1
        shr    eax, 0ch
        push   eax
        call   sub_3_5150
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_3_7430
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_3_51B0
        add    esp, 4
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ecx + ebp*8], eax
        mov    eax, dword ptr [edi + 4]
        inc    word ptr [eax + ebp*8 + 6]
        mov    eax, dword ptr [edi + 4]
        push   esi
        inc    word ptr [eax + ebp*8 + 4]
        inc    byte ptr [ebx + 1dh]
        inc    byte ptr [ebx + 1eh]
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [esi + 4ch], eax
        call   sub_3_59D8
        add    esp, 4
        and    dword ptr [esi + 20h], 0fffffeffh
        or     dword ptr [esi + 20h], 400h
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_7415
        cmp    byte ptr [ebx + 1eh], 1
        jne    SHORT loc_3_7408
        push   ebx
        push   edi
        call   sub_3_7A20
        add    esp, 8
loc_3_7408:
        mov    eax, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_3_7415:
        dec    byte ptr [ebx + 1dh]
        dec    byte ptr [ebx + 1eh]
        mov    ecx, dword ptr [edi + 4]
        mov    eax, 3
        dec    word ptr [ecx + ebp*8 + 4]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_3_7430:
        push   esi
        call   sub_3_59D8
        add    esp, 4
        call   sub_3_7F30
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_3_7446:
        inc    word ptr [edx + ebp*8 + 4]
        inc    al
        mov    byte ptr [ebx + 1dh], al
        inc    byte ptr [ebx + 1eh]
        mov    dword ptr [esi + 4ch], ecx
        mov    eax, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_3_7463:
        push   edi
        push   esi
        call   sub_3_79A0
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_7490
        add    edi, 70h
        mov    dword ptr [esi + 0b0h], edi
        or     dword ptr [esi + 20h], 40h
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_3_7490:
        lea    eax, [ebx + 14h]
        mov    dword ptr [esi + 0b0h], eax
        or     dword ptr [esi + 20h], 100h
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_74B4
        mov    ebp, 3
        jmp    SHORT loc_3_7522
loc_3_74B4:
        push   ebx
        push   edi
        call   sub_3_78A0
        add    esp, 8
        mov    eax, dword ptr [ebx + 20h]
        and    eax, 0fffff000h
        mov    dword ptr [esp + 14h], eax
        sub    eax, dword ptr [edi + 68h]
        shr    eax, 0ch
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 18h]
        cmp    word ptr [eax + ecx*8 + 6], 0
        jne    SHORT loc_3_7557
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_3_74FA
        mov    ebp, 1
        push   esi
        call   sub_3_59D8
        add    esp, 4
        jmp    SHORT loc_3_7522
loc_3_74FA:
        push   0
        mov    eax, dword ptr [esp + 18h]
        push   1
        shr    eax, 0ch
        push   eax
        call   sub_3_5150
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_7538
        push   esi
        call   sub_3_59D8
        add    esp, 4
        call   sub_3_7F30
        mov    ebp, eax
loc_3_7522:
        push   1
        push   ebx
        push   edi
        call   sub_3_7AB0
        add    esp, 0ch
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_3_7538:
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_3_51B0
        add    esp, 4
        mov    edx, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [edx + ecx*8], eax
        mov    eax, dword ptr [edi + 4]
        inc    word ptr [eax + ecx*8 + 6]
loc_3_7557:
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 18h]
        inc    word ptr [eax + ecx*8 + 4]
        inc    byte ptr [ebx + 1dh]
        inc    byte ptr [ebx + 1eh]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 10h], eax
        mov    dword ptr [ebx + 0ch], ebp
        mov    eax, dword ptr [edi + 6ch]
        and    eax, dword ptr [esp + 1ch]
        lea    eax, [edi + eax*4 + 80h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [eax], ebx
        mov    edx, dword ptr [ebx + 20h]
        mov    dword ptr [esi + 4ch], edx
        and    edx, 0fffh
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + ecx*8]
        add    eax, edx
        mov    dword ptr [esi + 54h], eax
        mov    eax, 2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_3_75B0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 50h]
        cmp    eax, 1
        je     SHORT loc_3_75FA
        cmp    eax, 2
        je     SHORT loc_3_75D0
        cmp    eax, 3
        je     SHORT loc_3_75F3
        cmp    eax, 4
        je     SHORT loc_3_75E0
        pop    esi
        ret
        db  08Bh, 0FFh
loc_3_75D0:
        mov    eax, dword ptr [esi + 4ch]
        push   eax
        push   esi
        call   sub_3_7670
        add    esp, 8
        jmp    SHORT loc_3_75F3
        db  090h
loc_3_75E0:
        xor    eax, eax
        mov    al, byte ptr [esi + 63h]
        push   eax
        mov    eax, dword ptr [esi + 4ch]
        push   eax
        push   esi
        call   sub_3_7690
        add    esp, 0ch
loc_3_75F3:
        mov    dword ptr [esi + 4ch], 0
loc_3_75FA:
        mov    dword ptr [esi + 50h], 0
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7610:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    ecx, dword ptr [esp + 0ch]
        push   ecx
        push   eax
        call   sub_3_7F10
        add    esp, 8
        add    eax, 14h
        mov    dword ptr [esi + 0b0h], eax
        push   esi
        call   sub_3_59D8
        add    esp, 4
        and    dword ptr [esi + 20h], 0fffffeffh
        or     dword ptr [esi + 20h], 400h
        push   esi
        call   sub_3_5971
        add    esp, 4
        mov    eax, dword ptr [esp + 0ch]
        and    byte ptr [eax], 0ffh
        mov    eax, 4
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7670:
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        push   1
        mov    esi, dword ptr [esp + 0ch]
        push   eax
        push   esi
        call   sub_3_7690
        add    esp, 0ch
        mov    dword ptr [esi + 4ch], 0
        pop    esi
        ret
        db  08Bh, 0FFh
sub_3_7690:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        push   ebx
        call   sub_3_7EF0
        add    esp, 4
        mov    esi, eax
        mov    ebp, dword ptr [esp + 18h]
        push   ebp
        push   esi
        call   sub_3_7F10
        add    esp, 8
        mov    edi, eax
        sub    ebp, dword ptr [esi + 68h]
        dec    byte ptr [edi + 1dh]
        dec    byte ptr [edi + 1eh]
        mov    eax, dword ptr [esi + 4]
        shr    ebp, 0ch
        dec    word ptr [eax + ebp*8 + 4]
        lea    eax, [edi + 14h]
        mov    dword ptr [ebx + 0b0h], eax
        push   ebx
        call   sub_3_59D8
        add    esp, 4
        cmp    byte ptr [edi + 1eh], 0
        jne    SHORT loc_3_7703
        mov    cl, byte ptr [edi + 1fh]
        test   cl, 1
        je     SHORT loc_3_76F4
        mov    eax, 1
        and    cl, 0feh
        mov    byte ptr [edi + 1fh], cl
        jmp    SHORT loc_3_76F8
loc_3_76F4:
        mov    eax, dword ptr [esp + 1ch]
loc_3_76F8:
        push   eax
        push   edi
        push   esi
        call   sub_3_7AB0
        add    esp, 0ch
loc_3_7703:
        cmp    byte ptr [esi + 75h], 0
        je     SHORT loc_3_770E
        call   sub_3_8690
loc_3_770E:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7720:
        db  0A1h, 0E4h, 09Bh, 000h, 000h
        inc    eax
        db  0A3h, 0E4h, 09Bh, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h
sub_3_7730:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 035h, 0D8h, 09Bh, 000h, 000h
        or     esi, esi
        mov    ebp, dword ptr [esp + 14h]
        je     SHORT loc_3_7789
loc_3_7742:
        mov    edi, dword ptr [esi]
        mov    ebx, dword ptr [esi + 60h]
        or     ebx, ebx
        je     SHORT loc_3_7789
loc_3_774B:
        cmp    dword ptr [edi + 0ch], ebp
        jne    SHORT loc_3_7781
        mov    cl, byte ptr [edi + 1eh]
        or     cl, cl
        lea    eax, [ecx + 1]
        mov    byte ptr [edi + 1eh], al
        jne    SHORT loc_3_7778
        push   edi
        push   esi
        call   sub_3_7A20
        add    esp, 8
        dec    byte ptr [edi + 1eh]
        push   1
        push   edi
        push   esi
        call   sub_3_7AB0
        add    esp, 0ch
        jmp    SHORT loc_3_7781
loc_3_7778:
        dec    al
        mov    byte ptr [edi + 1eh], al
        or     byte ptr [edi + 1fh], 1
loc_3_7781:
        dec    ebx
        add    edi, 24h
        or     ebx, ebx
        jne    SHORT loc_3_774B
loc_3_7789:
        db  039h, 035h, 0D8h, 09Bh, 000h, 000h
        jne    SHORT loc_3_779B
        db  08Bh, 035h, 0DCh, 09Bh, 000h, 000h
        or     esi, esi
        jne    SHORT loc_3_7742
loc_3_779B:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_77B0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        db  08Bh, 035h, 0D0h, 09Bh, 000h, 000h
        or     esi, esi
        mov    ebx, dword ptr [edi + 8]
        je     SHORT loc_3_77D8
loc_3_77C4:
        cmp    dword ptr [esi + 8], ebx
        jne    SHORT loc_3_77D2
        push   esi
        call   sub_3_7800
        add    esp, 4
loc_3_77D2:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_3_77C4
loc_3_77D8:
        or     byte ptr [edi + 1fh], 1
        db  0A1h, 0D8h, 09Bh, 000h, 000h
        mov    ecx, dword ptr [edi + 104h]
        inc    ecx
        shr    ecx, 1
        add    dword ptr [eax + 20h], ecx
        push   edi
        call   sub_3_7840
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_7800:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    al, byte ptr [esi + 1fh]
        and    al, 3
        cmp    al, 1
        jne    SHORT loc_3_7839
        mov    word ptr [esi + 20h], 0
        mov    dword ptr [esi + 28h], 0
        push   esi
        call   sub_3_7840
        add    esp, 4
        db  0A1h, 0D8h, 09Bh, 000h, 000h
        mov    ecx, dword ptr [esi + 104h]
        inc    ecx
        shr    ecx, 1
        sub    dword ptr [eax + 20h], ecx
        and    byte ptr [esi + 1fh], 0feh
loc_3_7839:
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_3_7840:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    al, byte ptr [edx + 1fh]
        and    al, 3
        cmp    al, 1
        jne    SHORT loc_3_789C
        db  0A1h, 0D8h, 09Bh, 000h, 000h
        xor    ecx, ecx
        mov    cx, word ptr [edx + 12eh]
        sub    dword ptr [eax + 24h], ecx
        mov    eax, dword ptr [edx + 28h]
        mov    word ptr [edx + 12eh], ax
        mov    si, word ptr [edx + 12eh]
        db  0A1h, 0D8h, 09Bh, 000h, 000h
        mov    ecx, esi
        and    ecx, 0ffffh
        add    dword ptr [eax + 24h], ecx
        db  0A1h, 0D8h, 09Bh, 000h, 000h
        cmp    dword ptr [eax + 24h], 0
        jne    SHORT loc_3_789C
        inc    si
        mov    word ptr [edx + 12eh], si
        db  0A1h, 0D8h, 09Bh, 000h, 000h
        inc    dword ptr [eax + 24h]
loc_3_789C:
        pop    esi
        ret
        db  08Bh, 0FFh
sub_3_78A0:
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 10h]
        add    ecx, dword ptr [edx + 0ch]
        and    ecx, dword ptr [eax + 6ch]
        lea    ecx, [eax + ecx*4 + 80h]
        mov    dword ptr [edx + 10h], 0
        mov    dword ptr [edx + 0ch], 0
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_3_78D9
loc_3_78CC:
        cmp    eax, edx
        je     SHORT loc_3_78DD
        lea    ecx, [eax + 8]
        mov    eax, dword ptr [ecx]
        or     eax, eax
        jne    SHORT loc_3_78CC
loc_3_78D9:
        cmp    eax, edx
        jne    SHORT loc_3_78E2
loc_3_78DD:
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx], eax
loc_3_78E2:
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_78F0:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        call   sub_3_7EF0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_3_7997
        mov    eax, dword ptr [esi + 8]
        mov    edi, dword ptr [eax + 4]
        or     edi, edi
        je     SHORT loc_3_7938
loc_3_7913:
        cmp    byte ptr [edi + 1ch], 4
        jne    SHORT loc_3_7931
        push   edi
        push   esi
        call   sub_3_7D10
        add    esp, 8
        mov    byte ptr [edi + 1ch], 2
        push   edi
        push   esi
        call   sub_3_7D70
        add    esp, 8
loc_3_7931:
        mov    edi, dword ptr [edi + 4]
        or     edi, edi
        jne    SHORT loc_3_7913
loc_3_7938:
        mov    eax, dword ptr [esi + 1ch]
        cmp    dword ptr [esi + 2ch], eax
        jb     SHORT loc_3_7997
        mov    eax, dword ptr [esi + 8]
        mov    edi, dword ptr [eax + 4]
        or     edi, edi
        je     SHORT loc_3_7997
        sub    ecx, ecx
        lea    esp, [esp]
loc_3_7950:
        cmp    byte ptr [edi + 1eh], cl
        jne    SHORT loc_3_7960
        mov    al, byte ptr [edi + 1ch]
        cmp    al, 1
        je     SHORT loc_3_7960
        cmp    al, 3
        jne    SHORT loc_3_7967
loc_3_7960:
        mov    edi, dword ptr [edi + 4]
        cmp    edi, ecx
        jne    SHORT loc_3_7950
loc_3_7967:
        or     edi, edi
        je     SHORT loc_3_7997
        push   edi
        push   esi
        call   sub_3_7A20
        add    esp, 8
        push   4
        push   edi
        push   esi
        call   sub_3_7AB0
        add    esp, 0ch
        push   edi
        push   esi
        call   sub_3_7A20
        add    esp, 8
        push   1
        push   edi
        push   esi
        call   sub_3_7AB0
        add    esp, 0ch
loc_3_7997:
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_79A0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    ebp, dword ptr [esp + 14h]
        lea    esp, [esp]
loc_3_79B0:
        mov    eax, dword ptr [esi + 8]
        sub    ebx, ebx
        mov    edi, dword ptr [eax + 4]
        cmp    edi, ebx
        je     SHORT loc_3_7A0C
        test   byte ptr [ebp + 6fh], 4
        jne    SHORT loc_3_79C8
        cmp    dword ptr [esi + 0ch], 2
        jbe    SHORT loc_3_7A17
loc_3_79C8:
        mov    al, byte ptr [edi + 1ch]
        cmp    al, 4
        je     SHORT loc_3_79D7
        cmp    al, 3
        je     SHORT loc_3_79D7
        cmp    al, 2
        jne    SHORT loc_3_79F4
loc_3_79D7:
        xor    ebx, ebx
        mov    bl, byte ptr [edi + 1eh]
        push   edi
        push   esi
        call   sub_3_7D10
        add    esp, 8
        mov    byte ptr [edi + 1ch], 1
        push   edi
        push   esi
        call   sub_3_7D70
        add    esp, 8
loc_3_79F4:
        inc    byte ptr [edi + 1eh]
        push   edi
        push   esi
        call   sub_3_7A20
        add    esp, 8
        dec    byte ptr [edi + 1eh]
        mov    byte ptr [edi + 1ch], 1
        and    byte ptr [edi + 1fh], 0fbh
loc_3_7A0C:
        or     ebx, ebx
        jne    SHORT loc_3_79B0
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_7A17:
        sub    edi, edi
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_7A20:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    al, byte ptr [edi + 1ch]
        cmp    al, 4
        je     SHORT loc_3_7A8A
        cmp    al, 2
        je     SHORT loc_3_7A8A
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 30h], edi
        jne    SHORT loc_3_7A3F
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 30h], eax
loc_3_7A3F:
        cmp    dword ptr [esi + 34h], edi
        jne    SHORT loc_3_7A49
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 34h], eax
loc_3_7A49:
        cmp    dword ptr [esi + 38h], edi
        jne    SHORT loc_3_7A53
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 38h], eax
loc_3_7A53:
        cmp    dword ptr [esi + 3ch], edi
        jne    SHORT loc_3_7A5D
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 3ch], eax
loc_3_7A5D:
        mov    ecx, dword ptr [edi + 4]
        or     ecx, ecx
        je     SHORT loc_3_7A68
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
loc_3_7A68:
        mov    eax, dword ptr [edi]
        push   edi
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 4], 0
        dec    dword ptr [esi + 0ch]
        push   esi
        call   sub_3_7D10
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_3_7A8A:
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   esi
        call   sub_3_7D10
        add    esp, 8
        mov    byte ptr [edi + 1ch], 3
        push   edi
        push   esi
        call   sub_3_7D70
        add    esp, 8
        pop    edi
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7AB0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        test   byte ptr [esp + 1fh], 80h
        je     SHORT loc_3_7AD0
        and    dword ptr [esp + 1ch], 7fffffffh
        mov    edi, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 18h]
        jmp    NEAR PTR loc_3_7C8B
loc_3_7AD0:
        cmp    dword ptr [esp + 1ch], 5
        jne    SHORT loc_3_7B40
        mov    esi, dword ptr [esp + 18h]
        cmp    byte ptr [esi + 1ch], 3
        jne    SHORT loc_3_7B09
        db  0A1h, 01Ch, 09Ch, 000h, 000h
        mov    edi, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [edi + 50h]
        cmp    eax, dword ptr [edi + 58h]
        ja     SHORT loc_3_7AFF
        mov    eax, dword ptr [esi + 10h]
        sub    eax, dword ptr [edi + 54h]
        cmp    eax, dword ptr [edi + 5ch]
        jbe    SHORT loc_3_7B0D
loc_3_7AFF:
        mov    dword ptr [esp + 1ch], 2
        jmp    SHORT loc_3_7B0D
loc_3_7B09:
        mov    edi, dword ptr [esp + 14h]
loc_3_7B0D:
        mov    cl, byte ptr [esi + 1fh]
        test   cl, 4
        je     SHORT loc_3_7B2D
        db  0A1h, 01Ch, 09Ch, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi + 48h], eax
        db  0A1h, 074h, 09Ch, 000h, 000h
        mov    eax, dword ptr [eax + 1ch]
        add    eax, dword ptr [esi + 10h]
        mov    dword ptr [edi + 4ch], eax
loc_3_7B2D:
        mov    eax, dword ptr [esi + 10h]
        cmp    dword ptr [edi + 54h], eax
        jne    SHORT loc_3_7B48
        or     cl, 4
        mov    byte ptr [esi + 1fh], cl
        jmp    SHORT loc_3_7B48
        db  08Dh, 049h, 000h
loc_3_7B40:
        mov    edi, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 18h]
loc_3_7B48:
        cmp    dword ptr [esi], 0
        je     SHORT loc_3_7B81
        mov    al, byte ptr [esi + 1ch]
        cmp    al, 4
        je     SHORT loc_3_7B58
        cmp    al, 3
        jne    SHORT loc_3_7B81
loc_3_7B58:
        cmp    dword ptr [esp + 1ch], 2
        je     SHORT loc_3_7B81
        push   esi
        push   edi
        call   sub_3_7D10
        add    esp, 8
        mov    byte ptr [esi + 1ch], 1
        push   esi
        push   edi
        call   sub_3_7D70
        add    esp, 8
        push   esi
        push   edi
        call   sub_3_7A20
        add    esp, 8
loc_3_7B81:
        mov    eax, dword ptr [edi + 10h]
        cmp    dword ptr [edi + 20h], eax
        jae    SHORT loc_3_7BBB
        mov    ebx, 6
        mov    edi, edi
loc_3_7B90:
        mov    eax, dword ptr [edi + 34h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edi + 34h], eax
        push   eax
        push   edi
        call   sub_3_7D10
        add    esp, 8
        mov    eax, dword ptr [edi + 34h]
        push   eax
        mov    byte ptr [eax + 1ch], bl
        push   edi
        call   sub_3_7D70
        add    esp, 8
        mov    eax, dword ptr [edi + 10h]
        cmp    dword ptr [edi + 20h], eax
        jb     SHORT loc_3_7B90
loc_3_7BBB:
        mov    eax, dword ptr [edi + 14h]
        cmp    dword ptr [edi + 24h], eax
        jae    SHORT loc_3_7BF3
        mov    ebx, 5
loc_3_7BC8:
        mov    eax, dword ptr [edi + 38h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edi + 38h], eax
        push   eax
        push   edi
        call   sub_3_7D10
        add    esp, 8
        mov    eax, dword ptr [edi + 38h]
        push   eax
        mov    byte ptr [eax + 1ch], bl
        push   edi
        call   sub_3_7D70
        add    esp, 8
        mov    eax, dword ptr [edi + 14h]
        cmp    dword ptr [edi + 24h], eax
        jb     SHORT loc_3_7BC8
loc_3_7BF3:
        cmp    dword ptr [esp + 1ch], 4
        jne    NEAR PTR loc_3_7C7A
        mov    ebx, dword ptr [edi + 0ch]
        sub    ebx, dword ptr [edi + 14h]
        sub    ebx, dword ptr [edi + 10h]
        mov    eax, dword ptr [edi + 28h]
        cmp    ebx, eax
        jbe    SHORT loc_3_7C10
        mov    ebx, eax
loc_3_7C10:
        mov    eax, dword ptr [edi + 18h]
        sub    eax, dword ptr [edi + 0ch]
        add    eax, dword ptr [edi + 1ch]
        add    eax, dword ptr [edi + 60h]
        cmp    eax, ebx
        jae    SHORT loc_3_7C7A
loc_3_7C20:
        mov    eax, dword ptr [edi + 3ch]
        cmp    dword ptr [edi + 38h], eax
        je     SHORT loc_3_7C7A
        mov    ebp, dword ptr [eax + 4]
        push   ebp
        push   edi
        call   sub_3_7A20
        add    esp, 8
        push   80000001h
        push   ebp
        push   edi
        call   sub_3_7AB0
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 20h]
        sub    eax, dword ptr [edi + 68h]
        and    eax, 0fffff1ffh
        shr    eax, 9
        mov    ecx, dword ptr [edi + 4]
        cmp    word ptr [eax + ecx + 6], 0
        db  0A1h, 0F8h, 09Bh, 000h, 000h
        jne    SHORT loc_3_7C6A
        and    eax, 0fffffffeh
        db  0A3h, 0F8h, 09Bh, 000h, 000h
loc_3_7C6A:
        mov    eax, dword ptr [edi + 18h]
        sub    eax, dword ptr [edi + 0ch]
        add    eax, dword ptr [edi + 1ch]
        add    eax, dword ptr [edi + 60h]
        cmp    eax, ebx
        jb     SHORT loc_3_7C20
loc_3_7C7A:
        cmp    dword ptr [esp + 1ch], 1
        jne    SHORT loc_3_7C8B
        push   esi
        push   edi
        call   sub_3_78A0
        add    esp, 8
loc_3_7C8B:
        mov    al, byte ptr [esp + 1ch]
        mov    byte ptr [esi + 1ch], al
        push   esi
        push   edi
        call   sub_3_7D70
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_7CEF
        inc    dword ptr [edi + 0ch]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    eax, dword ptr [ecx + 4]
        or     eax, eax
        mov    dword ptr [esi + 4], eax
        je     SHORT loc_3_7CB3
        mov    dword ptr [eax], esi
loc_3_7CB3:
        mov    dword ptr [ecx + 4], esi
        xor    eax, eax
        mov    al, byte ptr [esi + 1ch]
        cmp    eax, 4
        je     SHORT loc_3_7CD4
        cmp    eax, 5
        je     SHORT loc_3_7CDC
        cmp    eax, 6
        je     SHORT loc_3_7CE4
        cmp    eax, 7
        je     SHORT loc_3_7CEC
        cmp    dword ptr [edi + 3ch], ecx
        jne    SHORT loc_3_7CD7
loc_3_7CD4:
        mov    dword ptr [edi + 3ch], esi
loc_3_7CD7:
        cmp    dword ptr [edi + 38h], ecx
        jne    SHORT loc_3_7CDF
loc_3_7CDC:
        mov    dword ptr [edi + 38h], esi
loc_3_7CDF:
        cmp    dword ptr [edi + 34h], ecx
        jne    SHORT loc_3_7CE7
loc_3_7CE4:
        mov    dword ptr [edi + 34h], esi
loc_3_7CE7:
        cmp    dword ptr [edi + 30h], ecx
        jne    SHORT loc_3_7CEF
loc_3_7CEC:
        mov    dword ptr [edi + 30h], esi
loc_3_7CEF:
        cmp    byte ptr [esi + 1ch], 5
        jne    SHORT loc_3_7D02
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 10h]
        sub    eax, dword ptr [esi + 10h]
        cmp    eax, -1
        jne    SHORT loc_3_7D06
loc_3_7D02:
        and    byte ptr [esi + 1fh], 0fbh
loc_3_7D06:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_3_7D10:
        mov    eax, dword ptr [esp + 8]
        mov    al, byte ptr [eax + 1ch]
        and    eax, 0ffh
        cmp    eax, 1
        jl     SHORT loc_3_7D6F
        cmp    eax, 2
        jle    SHORT loc_3_7D36
        cmp    eax, 4
        je     SHORT loc_3_7D40
        cmp    eax, 6
        je     SHORT loc_3_7D60
        cmp    eax, 7
        je     SHORT loc_3_7D68
        ret
loc_3_7D36:
        mov    ecx, dword ptr [esp + 4]
        dec    dword ptr [ecx + 1ch]
        ret
        db  08Bh, 0FFh
loc_3_7D40:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 18h]
        dec    eax
        mov    dword ptr [ecx + 18h], eax
        test   byte ptr [ecx + 42h], 1
        je     SHORT loc_3_7D6F
        cmp    dword ptr [ecx + 7ch], eax
        jb     SHORT loc_3_7D6F
        push   ecx
        call   sub_3_7E10
        add    esp, 4
        ret
loc_3_7D60:
        mov    ecx, dword ptr [esp + 4]
        dec    dword ptr [ecx + 14h]
        ret
loc_3_7D68:
        mov    ecx, dword ptr [esp + 4]
        dec    dword ptr [ecx + 10h]
loc_3_7D6F:
        ret
sub_3_7D70:
        mov    ecx, dword ptr [esp + 8]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 1ch]
        and    ecx, 0ffh
        dec    ecx
        cmp    ecx, 6
        ja     SHORT loc_3_7DE7
        db  0FFh, 024h, 08Dh, 0E8h, 07Dh, 000h, 000h
        db  08Dh, 064h, 024h, 000h
loc_3_7D90:
        mov    ecx, dword ptr [esp + 4]
        lea    eax, [ecx + 8]
        inc    dword ptr [ecx + 1ch]
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_7DA0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 18h]
        inc    eax
        mov    dword ptr [ecx + 18h], eax
        cmp    dword ptr [ecx + 78h], eax
        ja     SHORT loc_3_7DB4
        or     byte ptr [ecx + 42h], 1
loc_3_7DB4:
        lea    eax, [ecx + 3ch]
        ret
loc_3_7DB8:
        mov    eax, dword ptr [esp + 4]
        add    eax, 38h
        ret
loc_3_7DC0:
        mov    ecx, dword ptr [esp + 4]
        inc    dword ptr [ecx + 14h]
        lea    eax, [ecx + 34h]
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_7DD0:
        mov    ecx, dword ptr [esp + 4]
        inc    dword ptr [ecx + 10h]
        lea    eax, [ecx + 30h]
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_7DE0:
        mov    ecx, dword ptr [esp + 4]
        inc    dword ptr [ecx + 1ch]
loc_3_7DE7:
        ret
        db  090h, 07Dh, 000h, 000h, 0E0h, 07Dh, 000h, 000h, 0E7h, 07Dh, 000h, 000h, 0A0h, 07Dh, 000h, 000h
        db  0B8h, 07Dh, 000h, 000h, 0C0h, 07Dh, 000h, 000h, 0D0h, 07Dh, 000h, 000h, 005h, 000h, 000h, 000h
        db  000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7E10:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        xor    eax, eax
        mov    dl, byte ptr [ecx + 42h]
        and    dl, 0feh
        mov    byte ptr [ecx + 42h], dl
        mov    al, byte ptr [ecx + 43h]
        cmp    eax, 5
        ja     NEAR PTR loc_3_7EC6
        db  0FFh, 024h, 085h, 0D4h, 07Eh, 000h, 000h
loc_3_7E33:
        mov    byte ptr [ecx + 43h], 1
        db  08Bh, 035h, 074h, 09Ch, 000h, 000h
        mov    eax, dword ptr [esi + 34h]
        add    dword ptr [ecx + 78h], eax
        mov    eax, dword ptr [ecx + 78h]
        dec    eax
        mov    dword ptr [ecx + 7ch], eax
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_7E50:
        mov    byte ptr [ecx + 43h], 2
        db  08Bh, 035h, 074h, 09Ch, 000h, 000h
        mov    eax, dword ptr [esi + 38h]
        add    dword ptr [ecx + 78h], eax
        mov    eax, dword ptr [ecx + 78h]
        dec    eax
        mov    dword ptr [ecx + 7ch], eax
        pop    esi
        ret
loc_3_7E69:
        mov    eax, dword ptr [ecx + 78h]
        cmp    dword ptr [ecx + 28h], eax
        jb     SHORT loc_3_7EC6
        db  08Bh, 035h, 074h, 09Ch, 000h, 000h
        mov    edx, dword ptr [esi + 40h]
        add    edx, eax
        mov    dword ptr [ecx + 78h], edx
        mov    eax, dword ptr [ecx + 78h]
        dec    eax
        mov    dword ptr [ecx + 7ch], eax
        pop    esi
        ret
loc_3_7E88:
        mov    byte ptr [ecx + 43h], 0
        db  08Bh, 035h, 074h, 09Ch, 000h, 000h
        or     dl, 1
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [ecx + 78h], eax
        mov    byte ptr [ecx + 42h], dl
        mov    eax, dword ptr [ecx + 78h]
        dec    eax
        mov    dword ptr [ecx + 7ch], eax
        pop    esi
        ret
loc_3_7EA7:
        mov    byte ptr [ecx + 43h], 2
        db  08Bh, 035h, 074h, 09Ch, 000h, 000h
        or     dl, 1
        mov    eax, dword ptr [esi + 3ch]
        mov    dword ptr [ecx + 78h], eax
        mov    byte ptr [ecx + 42h], dl
        mov    eax, dword ptr [ecx + 78h]
        dec    eax
        mov    dword ptr [ecx + 7ch], eax
        pop    esi
        ret
loc_3_7EC6:
        mov    byte ptr [ecx + 43h], 3
loc_3_7ECA:
        mov    eax, dword ptr [ecx + 78h]
        dec    eax
        mov    dword ptr [ecx + 7ch], eax
        pop    esi
        ret
        db  090h, 033h, 07Eh, 000h, 000h, 050h, 07Eh, 000h, 000h, 069h, 07Eh, 000h, 000h, 0CAh, 07Eh, 000h
        db  000h, 088h, 07Eh, 000h, 000h, 0A7h, 07Eh, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_3_7EF0:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 22h], 8
        je     SHORT loc_3_7F00
        db  0A1h, 0DCh, 09Bh, 000h, 000h
        ret
loc_3_7F00:
        db  0A1h, 0D8h, 09Bh, 000h, 000h
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7F10:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        xor    esi, esi
        sub    eax, dword ptr [ecx + 68h]
        mov    si, word ptr [ecx + 40h]
        sub    edx, edx
        div    esi
        shl    eax, 2
        pop    esi
        lea    eax, [eax + eax*8]
        add    eax, dword ptr [ecx]
        ret
sub_3_7F30:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 00Dh, 0F8h, 09Bh, 000h, 000h
        or     ecx, 2
        db  089h, 00Dh, 0F8h, 09Bh, 000h, 000h
        db  08Bh, 01Dh, 0D8h, 09Bh, 000h, 000h
        mov    ecx, ebx
        sub    eax, eax
        mov    edi, dword ptr [ecx + 4]
        mov    esi, ebx
        add    esi, 64h
        sub    ebp, ebp
        cmp    dword ptr [esi], eax
        jbe    SHORT loc_3_7FA3
loc_3_7F5B:
        cmp    word ptr [edi + 4], 0
        jne    SHORT loc_3_7F9B
loc_3_7F62:
        cmp    word ptr [edi + 6], 0
        je     SHORT loc_3_7F9B
        dec    word ptr [edi + 6]
        push   0
        push   1
        mov    eax, dword ptr [ebx + 68h]
        shr    eax, 0ch
        add    eax, ebp
        push   eax
        call   sub_3_5180
        add    esp, 0ch
        cmp    word ptr [edi + 6], 0
        jne    SHORT loc_3_7F94
        mov    dword ptr [edi], 0
        mov    eax, 3
loc_3_7F94:
        cmp    word ptr [edi + 4], 0
        je     SHORT loc_3_7F62
loc_3_7F9B:
        inc    ebp
        add    edi, 8
        cmp    dword ptr [esi], ebp
        ja     SHORT loc_3_7F5B
loc_3_7FA3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_7FB0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esi]
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_3_7FD9
        test   byte ptr [esi + 21h], 8
        je     NEAR PTR loc_3_8226
        mov    dword ptr [esi + 24h], 5
        jmp    NEAR PTR loc_3_8226
loc_3_7FD9:
        lea    eax, [esi + 20h]
        test   byte ptr [esi + 22h], 20h
        jne    SHORT loc_3_8021
        mov    eax, dword ptr [esi + 14h]
        or     dword ptr [esi + 20h], 200100h
        inc    word ptr [eax + 1ch]
        add    eax, 14h
        mov    dword ptr [esi + 0b0h], eax
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_8021
        mov    ax, word ptr [esi + 2ch]
        mov    word ptr [esi + 2eh], ax
        push   46h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_8021:
        sub    ebp, ebp
        cmp    dword ptr [esi + 18h], ebp
        jne    SHORT loc_3_8031
        push   esi
        call   sub_3_4560
        add    esp, 4
loc_3_8031:
        cmp    dword ptr [esi + 18h], 0
        jne    SHORT loc_3_8056
        push   esi
        call   sub_3_43D0
        add    esp, 4
        cmp    dword ptr [esi + 24h], 0
        jne    NEAR PTR loc_3_8226
        mov    ebp, 1
        or     dword ptr [esi + 20h], 800000h
loc_3_8056:
        test   byte ptr [esi + 22h], 40h
        jne    SHORT loc_3_80A7
        mov    eax, dword ptr [esi + 18h]
        or     dword ptr [esi + 20h], 400100h
        mov    ax, word ptr [eax + 10h]
        mov    ecx, dword ptr [esi + 18h]
        inc    ax
        mov    word ptr [ecx + 10h], ax
        mov    eax, dword ptr [esi + 18h]
        add    eax, 8
        mov    dword ptr [esi + 0b0h], eax
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_80A7
        mov    ax, word ptr [esi + 2ch]
        mov    word ptr [esi + 2eh], ax
        push   46h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_80A7:
        mov    ebx, dword ptr [esi + 18h]
        test   byte ptr [esi + 23h], 10h
        je     SHORT loc_3_80BB
        and    dword ptr [esi + 20h], 0efffffffh
        dec    word ptr [ebx + 10h]
loc_3_80BB:
        test   byte ptr [ebx + 13h], 4
        je     NEAR PTR loc_3_819C
        or     ebp, ebp
        jne    NEAR PTR loc_3_8159
        push   0
        push   14h
        call   sub_3_43B0
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_3_80F0
        mov    dword ptr [esi + 24h], 8
        jmp    NEAR PTR loc_3_822A
        db  005h, 000h, 000h, 000h, 000h
loc_3_80F0:
        mov    dword ptr [ebp], 0
        mov    dword ptr [ebp + 4], 0
        mov    eax, dword ptr [esi + 90h]
        push   0
        mov    eax, dword ptr [eax + 2]
        mov    dword ptr [ebp + 8], eax
        push   esi
        call   sub_3_9620
        add    esp, 4
        push   eax
        push   esi
        call   sub_3_9650
        add    esp, 0ch
        mov    dword ptr [ebp + 0ch], eax
        mov    ecx, dword ptr [esi + 90h]
        push   ebp
        mov    al, byte ptr [ecx + 1ah]
        mov    byte ptr [ebp + 12h], al
        mov    al, byte ptr [ecx + 1bh]
        mov    byte ptr [ebp + 13h], al
        mov    ax, word ptr [ecx + 1ch]
        mov    word ptr [ebp + 10h], ax
        push   esi
        call   sub_3_96B0
        add    esp, 8
        lea    eax, [ebx + 28h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_3_8157
        mov    edi, edi
loc_3_8150:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_3_8150
loc_3_8157:
        mov    dword ptr [eax], ebp
loc_3_8159:
        test   byte ptr [esi + 23h], 20h
        je     SHORT loc_3_8196
        mov    byte ptr [esi + 6dh], 24h
        mov    eax, dword ptr [esi + 84h]
        inc    eax
        mov    dword ptr [esi + 80h], eax
        mov    ax, word ptr [esi + 2ch]
        mov    word ptr [esi + 2eh], ax
        push   24h
        push   esi
        call   sub_3_8380
        add    esp, 8
        push   20h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_8196:
        and    word ptr [ebx + 12h], 0fbffh
loc_3_819C:
        push   esi
        call   sub_3_45C0
        add    esp, 4
        cmp    dword ptr [esi + 24h], 0
        jne    NEAR PTR loc_3_822A
        test   byte ptr [esi + 22h], 80h
        je     SHORT loc_3_81BE
        push   esi
        call   sub_3_4520
        add    esp, 4
loc_3_81BE:
        push   esi
        call   sub_3_4620
        add    esp, 4
        mov    byte ptr [edi + 4], 0
        mov    word ptr [edi + 18h], 1
        mov    dword ptr [edi], 1
        lea    eax, [ebx + 28h]
        mov    dword ptr [edi + 24h], 0
        or     eax, eax
        je     SHORT loc_3_81F1
loc_3_81E5:
        mov    ecx, dword ptr [eax + 0ch]
        add    dword ptr [edi + 24h], ecx
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_3_81E5
loc_3_81F1:
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [edi + 20h], eax
        lea    eax, [ebx + 1ch]
        push   eax
        call   sub_3_4850
        add    esp, 4
        mov    dword ptr [edi + 28h], eax
        mov    eax, dword ptr [edi + 10h]
        db  0C7h, 000h, 050h, 03Bh, 000h, 000h
        db  0C7h, 040h, 004h, 040h, 03Ch, 000h, 000h
        db  0C7h, 040h, 008h, 0FCh, 008h, 000h, 000h
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_3_822A
loc_3_8226:
        mov    ebx, dword ptr [esp + 10h]
loc_3_822A:
        test   byte ptr [esi + 22h], 20h
        je     SHORT loc_3_824E
        mov    eax, dword ptr [esi + 14h]
        add    eax, 14h
        mov    dword ptr [esi + 0b0h], eax
        push   esi
        call   sub_3_59D8
        add    esp, 4
        push   esi
        call   sub_3_4660
        add    esp, 4
loc_3_824E:
        test   byte ptr [esi + 22h], 40h
        je     SHORT loc_3_8272
        mov    eax, dword ptr [esi + 18h]
        add    eax, 8
        mov    dword ptr [esi + 0b0h], eax
        push   esi
        call   sub_3_59D8
        add    esp, 4
        push   esi
        call   sub_3_4730
        add    esp, 4
loc_3_8272:
        and    dword ptr [esi + 20h], 0ff9fffffh
        cmp    dword ptr [esi + 24h], 0
        jne    NEAR PTR loc_3_8360
        mov    eax, dword ptr [esi + 18h]
        mov    ax, word ptr [eax + 12h]
        and    ax, 4040h
        cmp    ax, 40h
        jne    SHORT loc_3_82F6
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   0
        push   930h
        call   sub_3_43B0
        add    esp, 8
        mov    dword ptr [esi + 4ch], eax
        cmp    dword ptr [esi + 4ch], 0
        jne    SHORT loc_3_82BC
        mov    dword ptr [esi + 24h], 8
loc_3_82BC:
        mov    eax, dword ptr [esi + 18h]
        mov    eax, dword ptr [eax + 30h]
        mov    dword ptr [esi + 48h], eax
        mov    dword ptr [esi + 50h], 2
        or     dword ptr [esi + 20h], 80c4000h
        mov    word ptr [esi + 2eh], 4ch
        cmp    dword ptr [esi + 24h], 0
        jne    NEAR PTR loc_3_836B
        push   6
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_82F6:
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_3_8360
        cmp    dword ptr [edi + 24h], 0
        je     SHORT loc_3_8360
        test   word ptr [ebx + 12h], 5ff0h
        jne    SHORT loc_3_8360
        db  0F6h, 005h, 0ADh, 09Ch, 000h, 000h, 020h
        jne    SHORT loc_3_8360
        db  0A1h, 01Ch, 09Ch, 000h, 000h
        mov    eax, dword ptr [eax]
        db  02Bh, 005h, 078h, 09Ch, 000h, 000h
        cmp    eax, 3e8h
        jbe    SHORT loc_3_8360
        push   esi
        call   sub_3_75B0
        add    esp, 4
        mov    byte ptr [esi + 63h], 5
        mov    edx, dword ptr [esi + 14h]
        mov    ebx, dword ptr [ebx + 30h]
        mov    eax, dword ptr [edx + 0ch]
        push   0ah
        mov    eax, dword ptr [eax + 0ch]
        sub    al, byte ptr [edx + 44h]
        mov    cl, al
        mov    word ptr [esi + 2eh], 1
        shr    ebx, cl
        mov    dword ptr [esi + 48h], ebx
        push   esi
        call   sub_3_8380
        add    esp, 8
        jmp    SHORT loc_3_8364
        db  08Dh, 049h, 000h
loc_3_8360:
        or     dword ptr [esi + 20h], 10h
loc_3_8364:
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [edi + 1ah], ax
loc_3_836B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8380:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ax, word ptr [esi + 30h]
        mov    cx, word ptr [esi + 2eh]
        mov    edx, eax
        and    edx, 0ffffh
        mov    word ptr [esi + edx*2 + 32h], cx
        inc    ax
        mov    word ptr [esi + 30h], ax
        mov    ax, word ptr [esp + 0ch]
        mov    word ptr [esi + 2eh], ax
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_3_83B0:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 30h]
        dec    ax
        mov    word ptr [ecx + 30h], ax
        and    eax, 0ffffh
        mov    ax, word ptr [ecx + eax*2 + 32h]
        mov    word ptr [ecx + 2eh], ax
        ret
        db  08Dh, 049h, 000h
sub_3_83D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
loc_3_83D5:
        mov    ax, word ptr [esi + 2eh]
        mov    word ptr [esi + 2ch], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 2ch]
        db  066h, 08Bh, 004h, 0C5h, 0AEh, 0A0h, 000h, 000h
        and    dword ptr [esi + 20h], 7fffffffh
        mov    word ptr [esi + 2eh], ax
        xor    eax, eax
        push   esi
        mov    ax, word ptr [esi + 2ch]
        db  0FFh, 014h, 0C5h, 0B0h, 0A0h, 000h, 000h
        add    esp, 4
        test   byte ptr [esi + 20h], 1
        je     SHORT loc_3_8432
        or     dword ptr [esi + 20h], 40000000h
        test   byte ptr [esi + 23h], 80h
        jne    SHORT loc_3_842B
loc_3_841A:
        push   1
        push   esi
        db  0E8h, 041h, 07Ch, 0FCh, 0FFh
        add    esp, 8
        test   byte ptr [esi + 23h], 80h
        je     SHORT loc_3_841A
loc_3_842B:
        and    dword ptr [esi + 20h], 3ffffffch
loc_3_8432:
        test   byte ptr [esi + 20h], 10h
        je     SHORT loc_3_83D5
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_8440:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        xor    eax, eax
        or     dword ptr [esi + 20h], 8
        mov    ax, word ptr [esi + 2eh]
        db  0F6h, 004h, 0C5h, 0ADh, 0A0h, 000h, 000h, 080h
        je     SHORT loc_3_84A1
        mov    edi, 8000h
        nop
loc_3_8460:
        test   byte ptr [esi + 20h], 16h
        jne    SHORT loc_3_84A1
        mov    ax, word ptr [esi + 2eh]
        mov    word ptr [esi + 2ch], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 2ch]
        db  066h, 08Bh, 004h, 0C5h, 0AEh, 0A0h, 000h, 000h
        mov    word ptr [esi + 2eh], ax
        push   esi
        xor    eax, eax
        mov    ax, word ptr [esi + 2ch]
        db  0FFh, 014h, 0C5h, 0B0h, 0A0h, 000h, 000h
        add    esp, 4
        xor    eax, eax
        mov    ax, word ptr [esi + 2eh]
        db  066h, 085h, 03Ch, 0C5h, 0ACh, 0A0h, 000h, 000h
        jne    SHORT loc_3_8460
loc_3_84A1:
        test   byte ptr [esi + 20h], 4
        je     SHORT loc_3_84B2
        and    dword ptr [esi + 20h], 0fffffffbh
        and    dword ptr [esi + 20h], 0fffffff7h
        pop    edi
        pop    esi
        ret
loc_3_84B2:
        test   byte ptr [esi + 21h], 10h
        jne    SHORT loc_3_84E0
        or     dword ptr [esi + 20h], 80000000h
        test   byte ptr [esi + 23h], 40h
        je     SHORT loc_3_84E4
        push   esi
        db  0E8h, 09Eh, 07Bh, 0FCh, 0FFh
        add    esp, 4
        and    dword ptr [esi + 20h], 0ffffffdfh
        and    dword ptr [esi + 20h], 0fffffff7h
        pop    edi
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_84E0:
        and    dword ptr [esi + 20h], 0fffffffch
loc_3_84E4:
        and    dword ptr [esi + 20h], 0ffffffdfh
        and    dword ptr [esi + 20h], 0fffffff7h
        pop    edi
        pop    esi
        ret
        db  090h
sub_3_84F0:
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 20h], 21h
        test   byte ptr [eax + 20h], 8
        je     SHORT loc_3_8502
        or     dword ptr [eax + 20h], 4
loc_3_8502:
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8510:
        sub    esp, 4
        cli
        db  0A1h, 0C0h, 09Bh, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_8536
        db  08Dh, 00Dh, 0C0h, 09Bh, 000h, 000h
loc_3_8523:
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_3_8530
        lea    ecx, [eax + 0ch]
        mov    eax, dword ptr [ecx]
        jmp    SHORT loc_3_8523
loc_3_8530:
        mov    dword ptr [ecx], 0
loc_3_8536:
        mov    dword ptr [esp], eax
        sti
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_8550:
        db  0C7h, 005h, 0D4h, 09Bh, 000h, 000h, 000h, 000h, 000h, 000h
        lea    ebx, [ebx]
loc_3_8560:
        call   sub_3_8510
        or     eax, eax
        je     SHORT loc_3_8574
        push   eax
        call   sub_3_8440
        add    esp, 4
        jmp    SHORT loc_3_8560
loc_3_8574:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8580:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushfd
        pop    dword ptr [ebp - 4]
        cli
        db  08Bh, 00Dh, 0C0h, 09Bh, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], ecx
        db  0A3h, 0C0h, 09Bh, 000h, 000h
        db  083h, 03Dh, 0D4h, 09Bh, 000h, 000h, 000h
        jne    SHORT loc_3_85B9
        push   0
        db  068h, 050h, 085h, 000h, 000h
        call   sub_3_8730
        add    esp, 8
        db  0A3h, 0D4h, 09Bh, 000h, 000h
loc_3_85B9:
        push   dword ptr [ebp - 4]
        popfd
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_85D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 20h], 40h
        je     SHORT loc_3_85F0
        mov    eax, dword ptr [esi + 0b0h]
        push   esi
        inc    byte ptr [eax + 5]
        call   sub_3_8670
        add    esp, 4
        pop    esi
        ret
        db  090h
loc_3_85F0:
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_8620
        test   byte ptr [esi + 21h], 1
        mov    eax, dword ptr [esi + 0b0h]
        je     SHORT loc_3_8610
        inc    byte ptr [eax + 4]
        jmp    SHORT loc_3_8613
        db  08Bh, 0FFh
loc_3_8610:
        inc    byte ptr [eax + 5]
loc_3_8613:
        push   esi
        call   sub_3_8670
        add    esp, 4
        pop    esi
        ret
        db  08Bh, 0FFh
loc_3_8620:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_8630:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 20h], 40h
        je     SHORT loc_3_8651
        mov    ecx, dword ptr [eax + 0b0h]
        and    dword ptr [eax + 20h], 0ffffffbfh
        dec    byte ptr [ecx + 5]
        push   eax
        call   sub_3_83B0
        add    esp, 4
        ret
loc_3_8651:
        test   byte ptr [eax + 21h], 1
        mov    eax, dword ptr [eax + 0b0h]
        je     SHORT loc_3_8661
        dec    byte ptr [eax + 4]
        ret
loc_3_8661:
        dec    byte ptr [eax + 5]
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8670:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 0C4h, 09Bh, 000h, 000h
        mov    dword ptr [ecx + 0ch], eax
        db  089h, 00Dh, 0C4h, 09Bh, 000h, 000h
        push   ecx
        call   sub_3_84F0
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h
sub_3_8690:
        push   esi
        db  08Bh, 015h, 0C4h, 09Bh, 000h, 000h
        db  0C7h, 005h, 0C4h, 09Bh, 000h, 000h, 000h, 000h, 000h, 000h
        sub    ecx, ecx
        cmp    edx, ecx
        je     SHORT loc_3_86B5
loc_3_86A7:
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [edx + 0ch], ecx
        mov    ecx, edx
        mov    edx, eax
        or     eax, eax
        jne    SHORT loc_3_86A7
loc_3_86B5:
        or     ecx, ecx
        je     SHORT loc_3_86CB
loc_3_86B9:
        mov    esi, dword ptr [ecx + 0ch]
        push   ecx
        call   sub_3_8580
        add    esp, 4
        mov    ecx, esi
        or     esi, esi
        jne    SHORT loc_3_86B9
loc_3_86CB:
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_3_86D0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_86E0
        xor    eax, eax
loc_3_86E0:
        nop
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_86F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_3_8700:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_3_8710:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8730:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8750:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        test   byte ptr [ebp + 10h], 8
        je     SHORT loc_3_87A2
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        db  0F6h, 005h, 005h, 000h, 000h, 000h, 001h
        jne    SHORT loc_3_879C
        db  081h, 00Dh, 004h, 000h, 000h, 000h, 000h, 001h, 000h, 000h
        push   0
        db  068h, 046h, 000h, 000h, 000h
        db  0E8h, 057h, 080h, 0FCh, 0FFh
        add    esp, 8
loc_3_879C:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
loc_3_87A2:
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_87C0:
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 20h], 10h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_87D0:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_87E0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax + 1ah], 78h
        ret
        db  08Dh, 049h, 000h
loc_3_87F0:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 24h], 0
        je     SHORT loc_3_8803
        push   eax
        call   sub_3_83B0
        add    esp, 4
loc_3_8803:
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8810:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 14h]
        lea    ecx, [esi + 90h]
        cmp    byte ptr [eax + 1eh], 0
        mov    eax, dword ptr [ecx]
        je     SHORT loc_3_8830
        movzx  edi, byte ptr [eax + 19h]
        jmp    SHORT loc_3_8834
        db  08Dh, 049h, 000h
loc_3_8830:
        movzx  edi, byte ptr [eax + 18h]
loc_3_8834:
        and    edi, 6
        or     edi, 1
        mov    eax, dword ptr [ecx]
        push   edi
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 20h]
        push   ecx
        add    eax, 21h
        push   eax
        push   esi
        call   sub_3_88E0
        add    esp, 10h
        push   edi
        push   esi
        call   sub_3_8AB0
        add    esp, 8
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_3_8860:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        lea    ecx, [esi + 94h]
        cmp    byte ptr [eax + 1eh], 0
        mov    eax, dword ptr [ecx]
        je     SHORT loc_3_8880
        mov    al, byte ptr [eax]
        jmp    SHORT loc_3_8883
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_8880:
        mov    al, byte ptr [eax + 5]
loc_3_8883:
        and    eax, 0ffh
        push   2
        push   eax
        mov    eax, dword ptr [ecx]
        add    eax, 8
        push   eax
        push   esi
        call   sub_3_88E0
        add    esp, 10h
        push   2
        push   esi
        call   sub_3_8AB0
        add    esp, 8
        pop    esi
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_88B0:
        push   esi
        push   8
        mov    esi, dword ptr [esp + 0ch]
        push   20h
        mov    eax, dword ptr [esi + 14h]
        add    eax, 58h
        push   eax
        push   esi
        call   sub_3_88E0
        add    esp, 10h
        push   8
        push   esi
        call   sub_3_8AB0
        add    esp, 8
        pop    esi
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_88E0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 14h]
        mov    ecx, dword ptr [eax + 4ch]
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 0ch], eax
        and    word ptr [eax + 0eh], 0fecfh
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 0ch]
        add    esi, 254h
        sub    esi, dword ptr [eax + 4]
        add    esi, 288h
        mov    ebx, dword ptr [esp + 24h]
        test   bl, 2
        mov    eax, dword ptr [esp + 20h]
        je     SHORT loc_3_895F
        cmp    eax, 1
        jne    SHORT loc_3_895F
        mov    edx, dword ptr [esp + 1ch]
        mov    dl, byte ptr [edx]
        or     dl, dl
        jne    SHORT loc_3_8948
        db  0C7h, 044h, 024h, 01Ch, 0C0h, 09Eh, 000h, 000h
        mov    eax, 2
        mov    ecx, 4b0h
        jmp    SHORT loc_3_895F
loc_3_8948:
        cmp    dl, 1
        jne    SHORT loc_3_895F
        db  0C7h, 044h, 024h, 01Ch, 0C4h, 09Eh, 000h, 000h
        mov    eax, 4
        mov    ecx, 4b0h
loc_3_895F:
        mov    dword ptr [esp + 10h], eax
        cmp    ecx, 4b0h
        jne    SHORT loc_3_89B0
        cmp    esi, eax
        jae    SHORT loc_3_897C
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esp + 10h], esi
        or     word ptr [eax + 0eh], 10h
loc_3_897C:
        mov    ecx, dword ptr [esp + 10h]
        add    cx, 2
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [eax], cx
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 1ch]
        mov    edi, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 10h]
        lea    edi, [edi + 2]
loc_3_89A0:
        lodsw
        sub    ecx, 2
        xchg   ah, al
        stosw
        ja     SHORT loc_3_89A0
        jmp    SHORT loc_3_89E9
        db  08Dh, 049h, 000h
loc_3_89B0:
        shr    esi, 1
        cmp    esi, eax
        jae    SHORT loc_3_89C3
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esp + 10h], esi
        or     word ptr [eax + 0eh], 10h
loc_3_89C3:
        mov    eax, dword ptr [esp + 0ch]
        push   1
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   eax
        lea    eax, [esi + 2]
        push   eax
        call   sub_3_4910
        add    esp, 10h
        add    ax, 2
        mov    word ptr [esi], ax
loc_3_89E9:
        test   bl, 8
        je     SHORT loc_3_8A28
        mov    eax, dword ptr [esp + 18h]
        mov    edx, dword ptr [eax + 10h]
        mov    ebx, dword ptr [edx + 4]
        mov    eax, 20h
        movzx  ecx, word ptr [ebx]
        lea    edi, [ebx + 2]
        shr    ecx, 1
        je     SHORT loc_3_8A1D
        dec    ecx
        je     SHORT loc_3_8A1D
        lea    edi, [edi + ecx*2 - 2]
        std
        repe scasw
        cld
        lea    edi, [edi + 2]
        je     SHORT loc_3_8A1D
        lea    edi, [edi + 2]
        jmp    SHORT loc_3_8A23
loc_3_8A1D:
        or     word ptr [edx + 0eh], 100h
loc_3_8A23:
        sub    edi, ebx
        mov    word ptr [ebx], di
loc_3_8A28:
        mov    eax, dword ptr [esp + 18h]
        mov    edx, dword ptr [eax + 10h]
        mov    edi, dword ptr [edx + 4]
        movzx  ecx, word ptr [edi]
        mov    eax, 3bh
        shr    ecx, 1
        lea    edi, [edi + 2]
        dec    ecx
        mov    ebx, edi
        je     SHORT loc_3_8A49
        repne scasw
        je     SHORT loc_3_8A4C
loc_3_8A49:
        add    edi, 2
loc_3_8A4C:
        db  081h, 07Ch, 024h, 01Ch, 0C4h, 09Eh, 000h, 000h
        je     SHORT loc_3_8A6E
loc_3_8A56:
        lea    eax, [edi - 4]
        cmp    ebx, eax
        jae    SHORT loc_3_8A6E
        cmp    word ptr [eax], 2eh
        je     SHORT loc_3_8A69
        cmp    word ptr [eax], 20h
        jne    SHORT loc_3_8A6E
loc_3_8A69:
        sub    edi, 2
        jmp    SHORT loc_3_8A56
loc_3_8A6E:
        sub    edi, ebx
        mov    word ptr [edx + 0ch], di
        db  080h, 03Dh, 080h, 008h, 000h, 000h, 000h
        jne    SHORT loc_3_8A83
        mov    edx, dword ptr [edx + 4]
        mov    word ptr [edx], di
loc_3_8A83:
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 4]
        xor    ecx, ecx
        mov    cx, word ptr [eax]
        add    ecx, eax
        mov    eax, dword ptr [esp + 0ch]
        add    eax, 45ch
        cmp    ecx, eax
        jbe    SHORT loc_3_8AA7
        mov    eax, dword ptr [esp + 0ch]
        or     word ptr [eax + 0eh], 20h
loc_3_8AA7:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
sub_3_8AB0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [esp + 0ch], eax
        and    word ptr [eax + 0eh], 0fd3fh
        cmp    edi, 8
        jne    SHORT loc_3_8B20
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, 6
        lea    edi, [edx + 10h]
        mov    eax, 200020h
        mov    esi, dword ptr [edx + 4]
        rep stosd
        sub    edi, 18h
        lodsw
        movzx  ecx, ax
        jecxz  loc_3_8B06
        sub    ecx, 2
        jbe    SHORT loc_3_8B06
        cmp    ecx, 16h
        jbe    SHORT loc_3_8AFF
        mov    ecx, 16h
loc_3_8AFF:
        shr    ecx, 1
        rep movsw
        jmp    SHORT loc_3_8B0C
loc_3_8B06:
        or     word ptr [edx + 0eh], 200h
loc_3_8B0C:
        mov    eax, dword ptr [esp + 0ch]
        or     word ptr [eax + 0eh], 8
        jmp    NEAR PTR loc_3_8BF3
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_8B20:
        mov    eax, dword ptr [esp + 0ch]
        mov    cx, word ptr [eax + 0ch]
        and    ecx, 0fffeh
        add    ecx, dword ptr [eax + 4]
        mov    bx, word ptr [ecx]
        mov    word ptr [ecx], 0
        mov    eax, dword ptr [esp + 0ch]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        sub    eax, 2
        push   eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 4]
        add    eax, 2
        push   eax
        mov    eax, dword ptr [esp + 14h]
        add    eax, 10h
        push   eax
        call   sub_3_48AF
        mov    ecx, dword ptr [esp + 18h]
        add    esp, 0ch
        mov    dx, word ptr [ecx + 0ch]
        and    edx, 0fffeh
        mov    ecx, dword ptr [ecx + 4]
        mov    word ptr [edx + ecx], bx
        test   edi, 4
        jne    SHORT loc_3_8BA0
        test   edi, 3
        je     SHORT loc_3_8B8E
        test   al, 3
        jne    SHORT loc_3_8BA0
loc_3_8B8E:
        test   al, 4
        je     SHORT loc_3_8BF3
        mov    eax, dword ptr [esp + 0ch]
        or     word ptr [eax + 0eh], 8
        jmp    SHORT loc_3_8BF3
        db  08Dh, 049h, 000h
loc_3_8BA0:
        mov    eax, dword ptr [esp + 0ch]
        or     word ptr [eax + 0eh], 48h
        test   edi, 2
        je     SHORT loc_3_8BC0
        mov    eax, dword ptr [esi + 7ch]
        lea    ecx, [eax*2 + 1]
        jmp    SHORT loc_3_8BCD
        db  08Dh, 049h, 000h
loc_3_8BC0:
        mov    eax, dword ptr [esi + 84h]
        lea    ecx, [eax*2 + 2]
loc_3_8BCD:
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [eax + 4ch], 4b0h
        je     SHORT loc_3_8BE2
        db  080h, 03Dh, 0ECh, 09Bh, 000h, 000h, 000h
        jne    SHORT loc_3_8BF3
loc_3_8BE2:
        mov    eax, dword ptr [esp + 0ch]
        push   ecx
        add    eax, 10h
        push   eax
        call   sub_3_48FE
        add    esp, 8
loc_3_8BF3:
        mov    eax, dword ptr [esp + 0ch]
        push   0
        mov    word ptr [eax + 26h], 0
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax + 8]
        add    eax, 10h
        push   eax
        lea    eax, [esi + 2]
        push   eax
        call   sub_3_4904
        add    esp, 0ch
        add    ax, 2
        mov    word ptr [esi], ax
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 8]
        xor    ecx, ecx
        mov    cx, word ptr [eax]
        add    ecx, eax
        mov    eax, dword ptr [esp + 0ch]
        add    eax, 57ch
        cmp    ecx, eax
        jbe    SHORT loc_3_8C42
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [eax + 0eh], 80h
loc_3_8C42:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8C50:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 14h], 0
        mov    edi, dword ptr [esi + 10h]
        mov    byte ptr [esi + 6ch], 0
        mov    ax, word ptr [edi + 0eh]
        test   al, 2
        je     SHORT loc_3_8C84
        test   byte ptr [esi + 6dh], 1
        jne    NEAR PTR loc_3_8D84
        mov    dword ptr [esp + 14h], 1
loc_3_8C84:
        test   byte ptr [esi + 21h], 80h
        je     NEAR PTR loc_3_8D49
        test   al, 10h
        jne    NEAR PTR loc_3_8D84
        test   al, 6
        jne    SHORT loc_3_8CA0
        mov    ax, word ptr [edi + 0ch]
        jmp    SHORT loc_3_8CA6
loc_3_8CA0:
        mov    eax, dword ptr [edi + 4]
        mov    ax, word ptr [eax]
loc_3_8CA6:
        mov    ecx, dword ptr [edi + 4]
        and    eax, 0ffffh
        lea    ebp, [ecx + eax]
        mov    dx, word ptr [ebp]
        mov    word ptr [esp + 10h], dx
        mov    word ptr [ebp], 0
        mov    eax, dword ptr [edi]
        xor    edx, edx
        mov    dx, word ptr [eax]
        lea    ebx, [eax + edx]
        add    ecx, 2
        mov    dx, word ptr [ebx]
        mov    word ptr [esp + 12h], dx
        mov    word ptr [ebx], 0
        mov    edx, dword ptr [esp + 14h]
        or     edx, 2
        push   edx
        push   ecx
        add    eax, 2
        push   eax
        call   sub_3_490A
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_8D36
        test   byte ptr [edi + 0eh], 80h
        jne    SHORT loc_3_8D36
        mov    dx, word ptr [esp + 10h]
        mov    word ptr [ebp], dx
        mov    ecx, dword ptr [edi + 8]
        xor    eax, eax
        mov    ax, word ptr [ecx]
        lea    ebp, [ecx + eax]
        mov    eax, dword ptr [esp + 14h]
        mov    dx, word ptr [ebp]
        mov    word ptr [esp + 10h], dx
        mov    word ptr [ebp], 0
        or     eax, 4
        push   eax
        add    ecx, 2
        push   ecx
        mov    eax, dword ptr [edi]
        add    eax, 2
        push   eax
        call   sub_3_490A
        add    esp, 0ch
loc_3_8D36:
        mov    dx, word ptr [esp + 12h]
        mov    word ptr [ebx], dx
        mov    dx, word ptr [esp + 10h]
        mov    word ptr [ebp], dx
        jmp    SHORT loc_3_8D67
loc_3_8D49:
        test   al, 80h
        mov    eax, 0
        jne    SHORT loc_3_8D67
        mov    eax, dword ptr [esp + 14h]
        push   eax
        lea    eax, [edi + 10h]
        push   eax
        add    edi, 30h
        push   edi
        call   sub_3_490A
        add    esp, 0ch
loc_3_8D67:
        or     eax, eax
        je     SHORT loc_3_8D84
        test   byte ptr [esp + 14h], 1
        je     SHORT loc_3_8D80
        mov    byte ptr [esi + 6ch], 2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_3_8D80:
        mov    byte ptr [esi + 6ch], 1
loc_3_8D84:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h
sub_3_8D90:
        sub    esp, 8
        push   edi
        mov    edx, dword ptr [esp + 10h]
        mov    edx, dword ptr [edx + 10h]
        mov    dword ptr [esp + 8], edx
        mov    eax, 3bh
        mov    edi, dword ptr [edx]
        movzx  ecx, word ptr [edi]
        lea    edi, [edi + ecx - 2]
        and    word ptr [edx + 0eh], 0fffbh
        shr    ecx, 1
        je     SHORT loc_3_8DC5
        dec    ecx
        je     SHORT loc_3_8DC5
        std
        repne scasw
        cld
        jne    SHORT loc_3_8DC5
        or     word ptr [edx + 0eh], 4
loc_3_8DC5:
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [eax + 21h], 80h
        jne    SHORT loc_3_8E18
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [ecx]
        cmp    ax, 2
        jbe    SHORT loc_3_8DF0
        and    eax, 0ffffh
        sub    eax, 2
        push   eax
        add    ecx, 2
        push   ecx
        jmp    SHORT loc_3_8DFE
        db  08Dh, 049h, 000h
loc_3_8DF0:
        mov    word ptr [esp + 6], 20h
        lea    eax, [esp + 6]
        push   2
        push   eax
loc_3_8DFE:
        mov    eax, dword ptr [esp + 10h]
        add    eax, 30h
        push   eax
        call   sub_3_491C
        mov    eax, dword ptr [esp + 14h]
        add    esp, 0ch
        mov    word ptr [eax + 46h], 0
loc_3_8E18:
        pop    edi
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_3_8E20:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_3_75B0
        add    esp, 4
        mov    byte ptr [esi + 63h], 6
        mov    eax, dword ptr [esi + 10h]
        mov    cl, byte ptr [esi + 6dh]
        and    ecx, 0fh
        cmp    ecx, 2
        jl     SHORT loc_3_8E53
        cmp    ecx, 3
        jle    NEAR PTR loc_3_8F0F
        cmp    ecx, 4
        je     NEAR PTR loc_3_8F6B
loc_3_8E53:
        mov    dword ptr [esi + 80h], 0
        mov    dword ptr [esi + 7ch], 0
        mov    edx, 0ffffffb4h
        sub    edx, eax
        xor    ecx, ecx
        add    edx, dword ptr [eax]
        mov    cx, word ptr [eax + 4ah]
        cmp    edx, ecx
        jb     NEAR PTR loc_3_8F41
        cmp    word ptr [eax + 48h], 2
        ja     SHORT loc_3_8EF6
        mov    edi, dword ptr [esi + 14h]
        add    edi, 10ch
        mov    dword ptr [esi + 90h], edi
        push   esi
        call   sub_3_94D0
        add    esp, 4
        mov    byte ptr [esi + 6eh], al
        push   esi
        call   sub_3_496E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_8EB4
        mov    dword ptr [esi + 24h], 2
        pop    edi
        pop    esi
        ret
loc_3_8EB4:
        mov    word ptr [esi + 66h], 1
        mov    al, byte ptr [edi + 1]
        and    eax, 0ffff00ffh
        mov    word ptr [esi + 76h], ax
        mov    ecx, dword ptr [edi + 2]
        mov    dword ptr [esi + 78h], ecx
        mov    dword ptr [esi + 84h], 0
        mov    edx, eax
        mov    eax, dword ptr [esi + 14h]
        and    edx, 0ffffh
        mov    cl, byte ptr [eax + 44h]
        shl    edx, cl
        mov    dword ptr [esi + 88h], edx
        mov    dword ptr [esi + 24h], 0
        pop    edi
        pop    esi
        ret
loc_3_8EF6:
        push   esi
        call   sub_3_8D90
        add    esp, 4
        mov    eax, dword ptr [esi]
        test   byte ptr [eax + 3], 80h
        je     SHORT loc_3_8F0F
        mov    eax, dword ptr [esi + 10h]
        or     word ptr [eax + 0eh], 2
loc_3_8F0F:
        mov    ax, word ptr [esi + 70h]
        test   al, 8
        je     SHORT loc_3_8F6B
        cmp    al, 8
        je     SHORT loc_3_8F22
        cmp    word ptr [esi + 66h], 1
        jne    SHORT loc_3_8F65
loc_3_8F22:
        push   esi
        call   sub_3_88B0
        add    esp, 4
        mov    byte ptr [esi + 6eh], 8
        test   byte ptr [esi + 21h], 80h
        jne    SHORT loc_3_8F50
        cmp    byte ptr [esi + 70h], 8
        jne    SHORT loc_3_8F50
        mov    byte ptr [esi + 6ch], 2
        jmp    SHORT loc_3_8F59
loc_3_8F41:
        mov    dword ptr [esi + 24h], 3
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_8F50:
        push   esi
        call   sub_3_8C50
        add    esp, 4
loc_3_8F59:
        cmp    byte ptr [esi + 6ch], 0
        jne    SHORT loc_3_8FB8
        cmp    byte ptr [esi + 70h], 8
        je     SHORT loc_3_8FBE
loc_3_8F65:
        and    word ptr [esi + 70h], 0fff7h
loc_3_8F6B:
        test   byte ptr [esi + 23h], 1
        jne    NEAR PTR loc_3_9013
        mov    ecx, dword ptr [esi + 14h]
        mov    ax, word ptr [ecx + 20h]
        cmp    word ptr [esi + 66h], ax
        jne    SHORT loc_3_8FF1
        mov    eax, dword ptr [ecx + 2ch]
        cmp    dword ptr [esi + 80h], eax
        jb     SHORT loc_3_8FF1
        mov    eax, dword ptr [ecx + 28h]
        mov    dword ptr [esi + 8ch], eax
        mov    ax, word ptr [ecx + 22h]
        mov    word ptr [esi + 76h], ax
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [esi + 84h], eax
        mov    eax, dword ptr [ecx + 30h]
        mov    dword ptr [esi + 88h], eax
        mov    eax, dword ptr [ecx + 34h]
        mov    dword ptr [esi + 7ch], eax
        jmp    SHORT loc_3_9013
loc_3_8FB8:
        cmp    byte ptr [esi + 70h], 8
        jne    SHORT loc_3_8FC4
loc_3_8FBE:
        mov    word ptr [esi + 66h], 0
loc_3_8FC4:
        push   esi
        call   sub_3_496E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_8FD8
        mov    dword ptr [esi + 24h], 12h
loc_3_8FD8:
        cmp    byte ptr [esi + 6ch], 0
        jne    SHORT loc_3_8FE5
        mov    dword ptr [esi + 24h], 12h
loc_3_8FE5:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        ret
loc_3_8FF1:
        mov    dword ptr [esi + 84h], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 76h]
        mov    cl, byte ptr [ecx + 44h]
        shl    eax, cl
        mov    dword ptr [esi + 88h], eax
        mov    dword ptr [esi + 7ch], 0
loc_3_9013:
        push   20h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_9030:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 14h]
        test   byte ptr [eax + 1fh], 2
        jne    SHORT loc_3_9070
        mov    edx, dword ptr [eax + 44h]
        mov    eax, dword ptr [eax + 0ch]
        mov    cl, dl
        mov    esi, dword ptr [edi + 88h]
        shr    esi, cl
        mov    ecx, dword ptr [eax + 0ch]
        sub    cl, dl
        add    esi, dword ptr [edi + 78h]
        shr    esi, cl
        mov    dword ptr [edi + 48h], esi
        push   0ah
        push   edi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_9070:
        mov    dword ptr [edi + 24h], 0
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_9080:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        cmp    dword ptr [ebx + 24h], 0
        je     SHORT loc_3_90A2
        push   ebx
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_90A2:
        mov    eax, dword ptr [ebx + 14h]
        test   byte ptr [eax + 1fh], 2
        jne    NEAR PTR loc_3_9153
        mov    eax, dword ptr [eax + 0ch]
        mov    edi, 1
        mov    eax, dword ptr [eax + 0ch]
        mov    cl, al
        shl    edi, cl
        mov    dword ptr [esp + 14h], eax
        lea    eax, [edi - 1]
        and    eax, dword ptr [ebx + 88h]
        add    eax, dword ptr [ebx + 4ch]
        mov    dword ptr [esp + 10h], eax
        mov    eax, 1
        shl    eax, cl
        mov    edx, dword ptr [ebx + 4ch]
        lea    esi, [eax + edx - 21h]
        cmp    dword ptr [ebx + 84h], 0
        jne    NEAR PTR loc_3_9271
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, dword ptr [ebx + 88h]
        mov    edx, dword ptr [eax + 0ah]
        add    edx, ebp
        add    edx, edi
        dec    edx
        shr    edx, cl
        mov    dword ptr [ebx + 8ch], edx
        test   byte ptr [ebx + 23h], 1
        je     SHORT loc_3_9118
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [eax + 24h], edx
        jmp    NEAR PTR loc_3_9271
loc_3_9118:
        mov    ecx, dword ptr [ebx + 14h]
        mov    ax, word ptr [ebx + 66h]
        mov    word ptr [ecx + 20h], ax
        mov    ax, word ptr [ebx + 76h]
        mov    word ptr [ecx + 22h], ax
        mov    dword ptr [ecx + 2ch], 0
        mov    dword ptr [ecx + 28h], edx
        mov    dword ptr [ecx + 30h], ebp
        mov    eax, dword ptr [ebx + 78h]
        mov    dword ptr [ecx + 24h], eax
        mov    dword ptr [ecx + 34h], 0
        push   ecx
        call   sub_3_7840
        add    esp, 4
        jmp    NEAR PTR loc_3_9271
loc_3_9153:
        mov    eax, dword ptr [ebx]
        add    eax, 3ch
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [ebx + 14h]
        db  0BEh, 020h, 09Fh, 000h, 000h
        mov    edx, dword ptr [eax + 134h]
        xor    eax, eax
        db  0A0h, 020h, 09Fh, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 14h]
        test   byte ptr [eax + 1fh], 20h
        jne    NEAR PTR loc_3_9268
        mov    eax, dword ptr [ebx + 84h]
        or     eax, eax
        je     NEAR PTR loc_3_9268
        cmp    eax, 1
        je     NEAR PTR loc_3_9260
        xor    eax, eax
        db  0A0h, 044h, 09Fh, 000h, 000h
        sub    eax, 5
        db  0BEh, 044h, 09Fh, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 10h]
        mov    esi, 4bh
        and    byte ptr [eax + 19h], 0fdh
        mov    ecx, dword ptr [ebx + 84h]
        mov    eax, dword ptr [edx + ecx*8 - 10h]
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, 2
        mov    dword ptr [edx + 2], eax
        sub    edx, edx
        div    esi
        mov    edi, eax
        mov    esi, 3ch
        sub    edx, edx
        div    esi
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 17h], dl
        mov    eax, edi
        sub    edx, edx
        div    esi
        mov    edi, eax
        sub    edx, edx
        div    esi
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 16h], dl
        mov    eax, edi
        sub    edx, edx
        div    esi
        mov    esi, 18h
        sub    edx, edx
        div    esi
        mov    eax, dword ptr [esp + 10h]
        mov    edi, 0ah
        mov    byte ptr [eax + 15h], dl
        mov    eax, dword ptr [ebx + 14h]
        mov    esi, dword ptr [esp + 10h]
        add    ecx, dword ptr [eax + 138h]
        mov    eax, ecx
        sub    edx, edx
        div    edi
        add    byte ptr [esi + 27h], dl
        mov    esi, edi
        mov    eax, ecx
        sub    edx, edx
        div    esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, edi
        sub    edx, edx
        div    esi
        add    byte ptr [ecx + 26h], dl
        jmp    SHORT loc_3_9268
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_9260:
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 21h], 1
loc_3_9268:
        mov    eax, dword ptr [esp + 10h]
        movzx  esi, byte ptr [eax]
        add    esi, eax
loc_3_9271:
        mov    byte ptr [ebx + 6ch], 0
        cmp    esi, dword ptr [esp + 10h]
        jbe    NEAR PTR loc_3_939B
        mov    ebp, 2
loc_3_9284:
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_3_939B
        mov    dword ptr [ebx + 90h], eax
        mov    edi, dword ptr [ebx + 20h]
        mov    eax, dword ptr [ebx + 14h]
        and    dword ptr [ebx + 20h], 0dfffffffh
        cmp    byte ptr [eax + 1eh], 0
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_3_92B3
        mov    al, byte ptr [eax + 19h]
        jmp    SHORT loc_3_92B6
loc_3_92B3:
        mov    al, byte ptr [eax + 18h]
loc_3_92B6:
        and    eax, 0ffh
        test   al, 80h
        je     SHORT loc_3_92C6
        or     dword ptr [ebx + 20h], 20000000h
loc_3_92C6:
        push   ebx
        call   sub_3_8810
        add    esp, 4
        push   ebx
        call   sub_3_94D0
        add    esp, 4
        mov    byte ptr [ebx + 6eh], al
        mov    eax, dword ptr [ebx + 84h]
        cmp    dword ptr [ebx + 80h], eax
        ja     SHORT loc_3_9356
        test   byte ptr [ebx + 6eh], 4
        jne    SHORT loc_3_9356
        cmp    word ptr [ebx + 66h], 1
        jne    SHORT loc_3_9306
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax + 20h], 1
        jne    SHORT loc_3_9306
        cmp    byte ptr [eax + 21h], 1
        jbe    SHORT loc_3_9356
loc_3_9306:
        mov    al, byte ptr [ebx + 6dh]
        and    al, 0fh
        cmp    al, 4
        je     NEAR PTR loc_3_9397
        test   edi, 20000000h
        je     SHORT loc_3_9320
        jmp    SHORT loc_3_9352
        db  08Dh, 049h, 000h
loc_3_9320:
        push   ebx
        call   sub_3_8C50
        add    esp, 4
        cmp    byte ptr [ebx + 6ch], 0
        je     SHORT loc_3_9356
        push   ebx
        call   sub_3_496E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_939B
        mov    eax, dword ptr [ebx + 10h]
        or     dword ptr [ebx + 20h], 800h
        test   word ptr [eax + 0eh], bp
        jne    SHORT loc_3_9352
        test   byte ptr [ebx + 6eh], 6
        je     SHORT loc_3_9391
loc_3_9352:
        mov    byte ptr [ebx + 6ch], 0
loc_3_9356:
        test   byte ptr [ebx + 6eh], 10h
        je     SHORT loc_3_935F
        inc    dword ptr [ebx + 7ch]
loc_3_935F:
        inc    dword ptr [ebx + 84h]
        mov    eax, dword ptr [esp + 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    dword ptr [ebx + 88h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    dword ptr [esp + 10h], eax
        cmp    esi, dword ptr [esp + 10h]
        ja     NEAR PTR loc_3_9284
        jmp    SHORT loc_3_939B
loc_3_9391:
        mov    byte ptr [ebx + 6ch], 3
        jmp    SHORT loc_3_939B
loc_3_9397:
        mov    byte ptr [ebx + 6ch], 1
loc_3_939B:
        xor    eax, eax
        mov    al, byte ptr [ebx + 6ch]
        or     eax, eax
        je     SHORT loc_3_93C3
        cmp    eax, 1
        jl     NEAR PTR loc_3_942F
        cmp    eax, 2
        jle    NEAR PTR loc_3_9437
        cmp    eax, 3
        je     SHORT loc_3_941F
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_93C3:
        mov    eax, dword ptr [ebx + 14h]
        mov    cl, byte ptr [eax + 1fh]
        mov    dl, cl
        and    dl, 22h
        cmp    dl, 22h
        je     SHORT loc_3_941B
        test   cl, 2
        je     SHORT loc_3_93F1
        mov    eax, dword ptr [eax + 130h]
        add    eax, 2
        cmp    eax, dword ptr [ebx + 84h]
        jbe    SHORT loc_3_941B
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_93F1:
        mov    eax, dword ptr [ebx + 4ch]
        sub    eax, dword ptr [esp + 10h]
        mov    edx, 1
        mov    cl, byte ptr [esp + 14h]
        shl    edx, cl
        add    eax, edx
        add    dword ptr [ebx + 88h], eax
        mov    eax, dword ptr [ebx + 8ch]
        shl    eax, cl
        cmp    eax, dword ptr [ebx + 88h]
        ja     SHORT loc_3_942F
loc_3_941B:
        mov    byte ptr [ebx + 6ch], 3
loc_3_941F:
        mov    dword ptr [ebx + 24h], 2
        push   ebx
        call   sub_3_83B0
        add    esp, 4
loc_3_942F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_9437:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 90h], eax
        push   ebx
        call   sub_3_83B0
        add    esp, 4
        test   byte ptr [ebx + 23h], 1
        je     SHORT loc_3_942F
        push   ebx
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_9470:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_3_94C4
        test   byte ptr [esi + 22h], 2
        jne    SHORT loc_3_94C4
        mov    ecx, dword ptr [esi + 14h]
        mov    ax, word ptr [esi + 66h]
        mov    word ptr [ecx + 20h], ax
        mov    ax, word ptr [esi + 76h]
        mov    word ptr [ecx + 22h], ax
        mov    eax, dword ptr [esi + 78h]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [esi + 8ch]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [esi + 84h]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [esi + 88h]
        mov    dword ptr [ecx + 30h], eax
        mov    eax, dword ptr [esi + 7ch]
        mov    dword ptr [ecx + 34h], eax
        push   ecx
        call   sub_3_7840
        add    esp, 4
loc_3_94C4:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret
        db  090h
sub_3_94D0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 90h]
        cmp    byte ptr [ecx + 1eh], 0
        je     SHORT loc_3_94F0
        mov    al, byte ptr [eax + 19h]
        push   eax
        call   sub_3_9500
        add    esp, 4
        ret
loc_3_94F0:
        mov    al, byte ptr [eax + 18h]
        push   eax
        call   sub_3_9500
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_3_9500:
        mov    al, 1
        mov    cl, byte ptr [esp + 4]
        test   al, cl
        je     SHORT loc_3_950C
        mov    al, 3
loc_3_950C:
        test   cl, 2
        je     SHORT loc_3_9515
        or     al, 10h
        and    al, 0feh
loc_3_9515:
        test   cl, 4
        je     SHORT loc_3_951C
        or     al, 6
loc_3_951C:
        ret
        db  08Dh, 049h, 000h
sub_3_9520:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_9620
        add    esp, 4
        test   dword ptr [esi + 20h], 20020000h
        jne    SHORT loc_3_9550
        push   1
        push   eax
        push   esi
        call   sub_3_9650
        add    esp, 0ch
        mov    dword ptr [esi + 0c0h], eax
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_3_9550:
        test   byte ptr [esi + 23h], 20h
        jne    SHORT loc_3_958A
        push   0
        push   eax
        push   esi
        call   sub_3_9650
        add    esp, 0ch
        add    dword ptr [esi + 0c0h], eax
        and    dword ptr [esi + 20h], 0fffdffffh
        mov    eax, dword ptr [esi + 0bch]
        mov    dword ptr [esi + 88h], eax
        mov    eax, dword ptr [esi + 0b8h]
        dec    eax
        mov    dword ptr [esi + 84h], eax
        jmp    SHORT loc_3_95ED
loc_3_958A:
        test   byte ptr [esi + 22h], 2
        jne    SHORT loc_3_95D4
        cmp    dword ptr [esi + 0b8h], 0
        je     SHORT loc_3_95A0
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_3_95A0:
        mov    ecx, dword ptr [esi + 84h]
        inc    ecx
        mov    dword ptr [esi + 0b8h], ecx
        mov    ecx, dword ptr [esi + 88h]
        mov    dword ptr [esi + 0bch], ecx
        or     dword ptr [esi + 20h], 20000h
        push   1
        push   eax
        push   esi
        call   sub_3_9650
        add    esp, 0ch
        mov    dword ptr [esi + 0c0h], eax
        jmp    SHORT loc_3_95E6
loc_3_95D4:
        push   0
        push   eax
        push   esi
        call   sub_3_9650
        add    esp, 0ch
        add    dword ptr [esi + 0c0h], eax
loc_3_95E6:
        mov    eax, dword ptr [esi + 84h]
        inc    eax
loc_3_95ED:
        mov    dword ptr [esi + 80h], eax
        mov    byte ptr [esi + 6dh], 24h
        mov    ax, word ptr [esi + 2ch]
        mov    word ptr [esi + 2eh], ax
        push   24h
        push   esi
        call   sub_3_8380
        add    esp, 8
        push   20h
        push   esi
        call   sub_3_8380
        add    esp, 8
        mov    eax, 1
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
sub_3_9620:
        mov    eax, dword ptr [esp + 4]
        xor    edx, edx
        mov    ecx, dword ptr [eax + 90h]
        mov    al, byte ptr [ecx + 20h]
        or     al, 1
        and    eax, 0ffh
        lea    eax, [ecx + eax + 21h]
        mov    dl, byte ptr [ecx]
        add    edx, ecx
        lea    ecx, [eax + 0eh]
        cmp    edx, ecx
        jb     SHORT loc_3_964D
        cmp    word ptr [eax + 6], 4158h
        je     SHORT loc_3_964F
loc_3_964D:
        sub    eax, eax
loc_3_964F:
        ret
sub_3_9650:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 90h]
        or     ecx, ecx
        mov    eax, dword ptr [eax + 0ah]
        je     SHORT loc_3_96AE
        mov    dx, word ptr [ecx + 4]
        test   dl, 50h
        je     SHORT loc_3_96AE
        add    eax, 7ffh
        shr    eax, 0bh
        mov    esi, eax
        lea    eax, [eax + eax*8]
        lea    eax, [esi + eax*8]
        lea    eax, [esi + eax*2]
        shl    eax, 4
        test   dl, 40h
        je     SHORT loc_3_96A4
        mov    cl, byte ptr [ecx + 8]
        cmp    cl, 10h
        jae    SHORT loc_3_96A4
        and    ecx, 0ffh
        db  08Ah, 08Ch, 089h, 054h, 0A0h, 000h, 000h
        shr    cl, 4
        shr    eax, cl
loc_3_96A4:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_96AE
        add    eax, 2ch
loc_3_96AE:
        pop    esi
        ret
sub_3_96B0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [eax + 4]
        cmp    word ptr [ecx + 0f8h], 1
        je     SHORT loc_3_96D8
        mov    eax, dword ptr [esp + 8]
        mov    dx, word ptr [ecx + 0fah]
        cmp    word ptr [eax + 10h], dx
        je     SHORT loc_3_96DC
        sub    eax, eax
        ret
loc_3_96D8:
        mov    eax, dword ptr [esp + 8]
loc_3_96DC:
        mov    dword ptr [eax + 4], ecx
        mov    eax, 1
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_96F0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_3_75B0
        add    esp, 4
        mov    byte ptr [esi + 63h], 7
        mov    dl, byte ptr [esi + 6dh]
        mov    al, dl
        and    eax, 0f0h
        cmp    eax, 10h
        je     SHORT loc_3_9746
        cmp    eax, 20h
        je     SHORT loc_3_977A
        push   esi
        call   sub_3_8D90
        add    esp, 4
        mov    word ptr [esi + 66h], 1
        mov    eax, dword ptr [esi + 14h]
        mov    word ptr [esi + 74h], 1
        mov    al, byte ptr [eax + 1fh]
        and    al, 2
        cmp    al, 1
        mov    dword ptr [esi + 7ch], 1
        sbb    eax, eax
        inc    eax
        mov    word ptr [esi + 64h], ax
        jmp    SHORT loc_3_97B0
loc_3_9746:
        mov    ecx, dword ptr [esi + 14h]
        mov    ax, word ptr [esi + 66h]
        cmp    word ptr [ecx + 20h], ax
        jne    SHORT loc_3_9771
        mov    word ptr [esi + 74h], ax
        mov    ax, word ptr [ecx + 22h]
        mov    word ptr [esi + 76h], ax
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 78h], eax
        mov    eax, dword ptr [ecx + 28h]
        mov    dword ptr [esi + 8ch], eax
        pop    edi
        pop    esi
        ret
loc_3_9771:
        and    dl, 0fh
        or     dl, 20h
        mov    byte ptr [esi + 6dh], dl
loc_3_977A:
        mov    eax, dword ptr [esi + 14h]
        sub    edx, edx
        mov    edi, dword ptr [eax + 108h]
        mov    ecx, dword ptr [eax + 104h]
        dec    ecx
        je     SHORT loc_3_97A5
        mov    ax, word ptr [esi + 66h]
loc_3_9792:
        cmp    word ptr [edi], ax
        ja     SHORT loc_3_979D
        cmp    word ptr [edi + 0ch], ax
        ja     SHORT loc_3_97A5
loc_3_979D:
        inc    edx
        add    edi, 0ch
        cmp    edx, ecx
        jb     SHORT loc_3_9792
loc_3_97A5:
        mov    ax, word ptr [edi]
        mov    word ptr [esi + 74h], ax
        mov    word ptr [esi + 64h], dx
loc_3_97B0:
        push   1eh
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_3_97C0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    si, word ptr [edi + 64h]
        test   si, 1
        jne    SHORT loc_3_9806
        mov    edx, dword ptr [edi + 14h]
        test   byte ptr [edx + 1fh], 2
        jne    SHORT loc_3_9806
        mov    eax, dword ptr [edx + 0ch]
        mov    ebx, dword ptr [edx + 0fch]
        mov    ecx, dword ptr [eax + 0ch]
        shr    si, 1
        sub    cl, byte ptr [edx + 44h]
        mov    eax, esi
        shr    ebx, cl
        and    eax, 0ffffh
        add    ebx, eax
        mov    dword ptr [edi + 48h], ebx
        push   0ah
        push   edi
        call   sub_3_8380
        add    esp, 8
loc_3_9806:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_9810:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        mov    dword ptr [esp + 14h], 0
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_3_983A
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_3_983A:
        mov    edi, dword ptr [esi + 10h]
        mov    eax, dword ptr [esi + 14h]
        mov    dx, word ptr [esi + 64h]
        mov    ecx, edx
        and    ecx, 0ffffh
        mov    eax, dword ptr [eax + 108h]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [eax + ecx*4]
        xor    ebx, ebx
        mov    bx, word ptr [ecx + 2]
        test   dl, 1
        je     SHORT loc_3_9868
        mov    eax, dword ptr [ecx + 8]
        jmp    SHORT loc_3_9871
loc_3_9868:
        xor    eax, eax
        mov    ax, word ptr [ecx + 4]
        add    eax, dword ptr [esi + 4ch]
loc_3_9871:
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esi + 14h]
        test   byte ptr [esi + 6dh], 0f0h
        movzx  ebp, byte ptr [eax + 1eh]
        je     SHORT loc_3_98BA
        cmp    word ptr [ecx + 6], 0
        je     SHORT loc_3_98BA
        xor    eax, eax
        xor    edx, edx
        mov    dx, word ptr [esi + 74h]
        mov    ax, word ptr [esi + 66h]
        sub    eax, edx
        cmp    eax, 40h
        jl     SHORT loc_3_98BA
        mov    ax, word ptr [esi + 74h]
        add    ax, 3fh
        mov    word ptr [esi + 74h], ax
        xor    eax, eax
        mov    ax, word ptr [ecx + 6]
        add    dword ptr [esp + 10h], eax
        xor    eax, eax
        mov    ax, word ptr [ecx + 6]
        sub    ebx, eax
loc_3_98BA:
        or     ebx, ebx
        jle    NEAR PTR loc_3_9A58
loc_3_98C2:
        test   byte ptr [esi + 6dh], 0f0h
        jne    NEAR PTR loc_3_9960
        cmp    word ptr [esi + 74h], 1
        je     NEAR PTR loc_3_996D
        mov    ecx, dword ptr [esp + 10h]
        mov    ax, word ptr [esi + 66h]
        cmp    word ptr [ecx + 6], ax
        jne    NEAR PTR loc_3_99D9
        inc    dword ptr [esi + 7ch]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 94h], eax
        push   esi
        call   sub_3_8860
        add    esp, 4
        push   esi
        call   sub_3_8C50
        add    esp, 4
        cmp    byte ptr [esi + 6ch], 0
        je     NEAR PTR loc_3_99D9
        mov    eax, dword ptr [edi]
        xor    ecx, ecx
        mov    cx, word ptr [eax]
        add    ecx, eax
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    ax, word ptr [ecx]
        add    word ptr [edi + 2ah], ax
        add    word ptr [edi + 28h], ax
        and    eax, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [edi + 4], eax
        mov    ax, word ptr [ecx]
        add    word ptr [edi + 2eh], ax
        add    word ptr [edi + 2ch], ax
        and    eax, 0ffffh
        add    eax, ecx
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [esi + 7ch], 1
        push   esi
        call   sub_3_8D90
        add    esp, 4
        jmp    SHORT loc_3_996D
        db  090h
loc_3_9960:
        lea    eax, [esi + 74h]
        mov    ax, word ptr [esi + 74h]
        cmp    word ptr [esi + 66h], ax
        jne    SHORT loc_3_99D9
loc_3_996D:
        mov    ax, word ptr [esi + 76h]
        mov    word ptr [esi + 5ch], ax
        mov    ax, word ptr [esi + 66h]
        mov    word ptr [esi + 5eh], ax
        or     ebp, ebp
        mov    eax, dword ptr [esi + 78h]
        mov    dword ptr [esi + 98h], eax
        mov    ax, word ptr [esi + 74h]
        mov    word ptr [esi + 66h], ax
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_3_999B
        mov    eax, dword ptr [eax + 2]
        jmp    SHORT loc_3_999D
loc_3_999B:
        mov    eax, dword ptr [eax]
loc_3_999D:
        mov    dword ptr [esi + 78h], eax
        or     ebp, ebp
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_3_99B0
        mov    al, byte ptr [eax + 1]
        jmp    SHORT loc_3_99B3
        db  08Dh, 049h, 000h
loc_3_99B0:
        mov    al, byte ptr [eax + 4]
loc_3_99B3:
        and    eax, 0ffff00ffh
        test   byte ptr [esi + 6dh], 0f0h
        mov    word ptr [esi + 76h], ax
        jne    NEAR PTR loc_3_9A50
        mov    eax, 0ffffffb4h
        sub    eax, edi
        xor    ecx, ecx
        add    eax, dword ptr [edi]
        mov    cx, word ptr [edi + 4ah]
        cmp    eax, ecx
        jae    SHORT loc_3_9A50
loc_3_99D9:
        inc    word ptr [esi + 74h]
        or     ebp, ebp
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_3_99EB
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        jmp    SHORT loc_3_99F0
loc_3_99EB:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 5]
loc_3_99F0:
        or     ebp, ebp
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_3_9A00
        mov    al, byte ptr [eax]
        jmp    SHORT loc_3_9A03
        db  08Dh, 064h, 024h, 000h
loc_3_9A00:
        mov    al, byte ptr [eax + 5]
loc_3_9A03:
        and    eax, 0ffh
        and    eax, 1
        add    eax, ecx
        add    eax, 8
        sub    ebx, eax
        or     ebp, ebp
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_3_9A20
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        jmp    SHORT loc_3_9A25
loc_3_9A20:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 5]
loc_3_9A25:
        or     ebp, ebp
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_3_9A31
        mov    al, byte ptr [eax]
        jmp    SHORT loc_3_9A34
loc_3_9A31:
        mov    al, byte ptr [eax + 5]
loc_3_9A34:
        and    eax, 0ffh
        and    eax, 1
        add    eax, ecx
        add    eax, 8
        add    dword ptr [esp + 10h], eax
        or     ebx, ebx
        jg     NEAR PTR loc_3_98C2
        jmp    SHORT loc_3_9A58
        db  090h
loc_3_9A50:
        mov    dword ptr [esp + 14h], 1
loc_3_9A58:
        test   byte ptr [esi + 6dh], 0f0h
        je     SHORT loc_3_9A80
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_3_9A6C
        mov    dword ptr [esi + 24h], 2
loc_3_9A6C:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_3_9A80:
        mov    ax, word ptr [esi + 64h]
        inc    ax
        cmp    dword ptr [esp + 14h], 0
        mov    word ptr [esi + 64h], ax
        mov    dword ptr [esi + 24h], 0
        jne    SHORT loc_3_9AA8
        mov    ecx, dword ptr [esi + 14h]
        and    eax, 0ffffh
        cmp    dword ptr [ecx + 104h], eax
        ja     SHORT loc_3_9AB1
loc_3_9AA8:
        push   esi
        call   sub_3_83B0
        add    esp, 4
loc_3_9AB1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_9AC0:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 1ch]
        db  0A1h, 090h, 008h, 000h, 000h
        lea    ecx, [edx + edx*2]
        lea    ebx, [eax + ecx*2 + 14h]
        mov    esi, dword ptr [esp + 10h]
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 1dh
        rep stosd
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 34h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 38h], eax
        cmp    edx, 1ah
        mov    dword ptr [esi + 28h], edx
        jae    SHORT loc_3_9B30
        mov    al, byte ptr [ebx + 1]
        test   al, 8
        je     SHORT loc_3_9B30
        test   al, 10h
        jne    SHORT loc_3_9B10
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_9B10:
        push   esi
        call   sub_3_9B40
        add    esp, 4
        cmp    word ptr [esi + 1ah], 0
        je     SHORT loc_3_9B36
        mov    word ptr [esi + 1ah], 1eh
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_9B30:
        mov    word ptr [esi + 1ah], 0fh
loc_3_9B36:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
sub_3_9B40:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        sub    edi, edi
        lea    ebx, [esi + 10h]
        push   1
        push   ebx
        mov    eax, dword ptr [esi + 28h]
        push   eax
        call   sub_3_31B0
        add    esp, 0ch
        cmp    eax, edi
        je     SHORT loc_3_9B63
        mov    dword ptr [ebx], edi
        jmp    SHORT loc_3_9B7B
loc_3_9B63:
        db  08Bh, 03Dh, 0D0h, 09Bh, 000h, 000h
        or     edi, edi
        je     SHORT loc_3_9B7B
        mov    eax, dword ptr [ebx]
loc_3_9B70:
        cmp    dword ptr [edi + 4], eax
        je     SHORT loc_3_9B7B
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_3_9B70
loc_3_9B7B:
        mov    dword ptr [esi + 1ch], edi
        or     edi, edi
        jne    SHORT loc_3_9B90
        mov    word ptr [esi + 1ah], 1eh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_9B90:
        mov    word ptr [esi + 1ah], 0
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_9BA0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        mov    eax, dword ptr [eax + 28h]
        push   eax
        call   sub_3_31B6
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_9BE4:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_9BE8:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_9BEC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9BF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9BF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9BFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9C00:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9C04:
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_3_9C08:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_3_9C0C:
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_3_9C10:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_9C14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9C1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9C20:
        inc    ebx
        inc    esp
        inc    esi
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    ah, dh
        add    dword ptr [eax], eax
        add    ah, dh
        add    dword ptr [eax], eax
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_9C74:
        and    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9CAD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9CEF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 050h, 039h
        add    byte ptr [eax], al
        db  0A0h, 039h, 000h, 000h, 0F0h
        sldt   word ptr [eax]
        db  0F0h, 00Fh, 000h, 000h, 0F0h, 00Fh, 000h, 000h, 0D0h, 010h, 000h, 000h, 000h, 012h, 000h, 000h
        db  000h, 012h, 000h, 000h, 010h, 012h, 000h, 000h, 020h, 013h, 000h, 000h, 040h, 013h, 000h, 000h
        db  0F0h, 039h, 000h, 000h, 030h, 03Ah, 000h, 000h, 050h, 03Ah, 000h, 000h, 050h, 013h, 000h, 000h
        db  040h, 014h, 000h, 000h, 0D0h, 031h, 000h, 000h
loc_3_9E24:
        mov    al, 5
        add    byte ptr [eax], al
        mov    al, byte ptr [32h]
        add    byte ptr [eax], al
        db  000h, 0D0h
        db  005h, 000h, 000h, 000h, 006h
        add    byte ptr [eax], al
        db  0A0h, 006h, 000h, 000h, 000h
        xor    eax, dword ptr [eax]
        db  000h, 0D0h
        push   es
        add    byte ptr [eax], al
        xor    byte ptr [edi], al
        add    byte ptr [eax], al
        rol    byte ptr [edi], 0
        db  000h, 030h
        or     byte ptr [eax], al
        db  000h, 0A0h, 008h, 000h, 000h, 050h
        xor    eax, dword ptr [eax]
        db  000h, 010h
        or     dword ptr [eax], eax
        db  000h, 090h, 009h, 000h, 000h, 0F0h
        xor    eax, dword ptr [eax]
        db  000h
loc_3_9E64:
        lock or dword ptr [eax], eax
        db  000h, 020h
        or     al, byte ptr [eax]
        db  000h, 060h, 00Ah
        add    byte ptr [eax], al
        nop
        or     al, byte ptr [eax]
        db  000h, 020h
        or     eax, dword ptr [eax]
        db  000h, 050h, 00Bh
        add    byte ptr [eax], al
        push   eax
        xor    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E0h
        or     eax, dword ptr [eax]
        db  000h, 0F0h
        or     eax, dword ptr [eax]
        db  000h, 040h, 00Ch
        add    byte ptr [eax], al
        ror    byte ptr [0], 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
loc_3_9EA0:
        push   ds
        add    byte ptr [eax + eax], al
        sbb    al, byte ptr [eax]
        aas
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [esi], ch
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr cs:[eax], al
loc_3_9EC4:
        add    byte ptr [esi], ch
        add    byte ptr [esi], ch
loc_3_9EC8:
        add    byte ptr [eax], al
        pop    ds
        add    byte ptr [ebx], bh
        add    byte ptr [edx], bl
        js     SHORT loc_3_9ED2
loc_3_9ED2:
        xchg   edi, eax
        add    byte ptr [ebp - 0cff2c00h], dh
        add    byte ptr [ecx], dl
        add    dword ptr [eax], esi
        add    dword ptr [esi + 1], ecx
loc_3_9EE0:
        inc    ebx
        inc    esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_3_9F00:
        inc    ecx
        db  075h, 064h
        imul   ebp, dword ptr [edi + 20h], 20204443h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_3_9F20:
        and    al, byte ptr [eax]
        xchg   esi, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xchg   esi, eax
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        pop    edi
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
        add    byte ptr [esi], dl
        add    byte ptr [eax], al
        xchg   esi, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9F56:
        pop    edi
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        push   esp
        db  072h, 061h
        arpl   word ptr [ebx + 30h], bp
        xor    byte ptr [esi], ch
        arpl   word ptr [ecx], sp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    eax
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9F7C:
        push   edx
        dec    ecx
        inc    esi
        inc    esi
        and    al, 0
        add    byte ptr [eax], al
        push   edi
        inc    ecx
        push   esi
        inc    ebp
        insw
        je     SHORT loc_3_9FAC
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        inc    esp
        lodsb
        add    byte ptr [eax], al
        adc    byte ptr [ecx + 40002h], dh
        adc    byte ptr [eax], al
        popad
        je     SHORT loc_3_A005
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9FA8:
        push   edx
        dec    ecx
        inc    esi
        inc    esi
loc_3_9FAC:
        and    al, 0
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        inc    esp
        inc    ecx
        insw
        je     SHORT loc_3_9FD8
        sbb    byte ptr [eax], al
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
loc_3_9FD4:
        push   edx
        dec    ecx
        inc    esi
        inc    esi
loc_3_9FD8:
        and    al, 0
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        pop    eax
        inc    ecx
        insw
        db  074h, 020h
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    eax
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        popad
        db  074h, 061h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_A000:
        add    dword ptr [eax], eax
        xchg   esi, eax
        add    byte ptr [eax], al
loc_3_A005:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h
        mov    al, byte ptr [20000h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_A030:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 20000h], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_A054:
        db  000h, 026h
        pop    edx
        add    byte ptr [eax], al
        db  014h, 023h
        pop    edx
        add    byte ptr [eax], al
        db  028h, 020h
        pop    edx
        add    byte ptr [eax], al
        db  000h, 026h
        pop    edx
        add    byte ptr [eax], al
        db  012h, 030h
        pop    edx
        add    byte ptr [eax], al
        db  026h, 02Dh, 05Ah, 000h, 000h, 03Ah
        sub    bl, byte ptr [edx]
        add    byte ptr [eax], al
        pop    edx
        add    byte ptr [eax], al
        db  011h, 04Bh, 05Ah
        add    byte ptr [eax], al
        db  025h, 048h, 05Ah, 000h, 000h
        db  039h, 045h, 05Ah
        add    byte ptr [eax], al
        db  000h, 026h
        pop    edx
        add    byte ptr [eax], al
        db  023h, 063h, 05Ah
        add    byte ptr [eax], al
        aaa
        pushad
        pop    edx
        add    byte ptr [eax], al
        dec    ebx
        pop    ebp
        pop    edx
        add    byte ptr [eax], al
        db  000h, 026h
        pop    edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_A0AC:
        db  000h, 080h, 001h, 000h, 0D0h, 087h
        add    byte ptr [eax], al
        db  001h, 080h, 000h, 000h, 0C0h, 087h
        add    byte ptr [eax], al
        db  002h, 080h, 000h, 000h, 0B0h, 083h
        add    byte ptr [eax], al
        db  003h, 080h, 001h, 000h, 0E0h, 087h
        add    byte ptr [eax], al
        add    al, 80h
        db  005h, 000h, 0E0h, 05Ah, 000h
        db  000h, 005h, 000h, 002h, 000h, 070h
        push   edi
        add    byte ptr [eax], al
        push   es
        add    byte ptr [edi], 0
        add    byte ptr [edi], ah
        add    byte ptr [edi], al
        add    byte ptr [eax], cl
        db  000h, 020h
        add    byte ptr [bx + si], al
        db  008h, 080h, 009h, 000h, 090h, 05Bh
        add    byte ptr [eax], al
        db  009h, 080h, 002h, 000h, 060h, 05Ch
        add    byte ptr [eax], al
        db  00Ah, 080h, 00Bh, 000h, 080h, 05Ah
        add    byte ptr [eax], al
        db  00Bh, 080h, 006h, 000h, 0A0h, 05Ah
        add    byte ptr [eax], al
        or     al, 0
        db  00Dh, 000h, 030h, 01Eh, 000h
        db  000h, 00Dh, 000h, 00Eh, 000h, 010h
        and    dword ptr [eax], eax
        add    byte ptr [esi], cl
        add    byte ptr [edi], cl
        db  000h, 050h, 023h
        add    byte ptr [eax], al
        lldt   word ptr [eax]
        db  000h, 0A0h, 023h, 000h, 000h, 010h
        add    byte ptr [ecx], dl
        db  000h, 080h, 027h, 000h, 000h, 011h
        add    byte ptr [edx], dl
        db  000h, 080h, 02Eh, 000h, 000h, 012h
        add    byte ptr [ebx], dl
        db  000h, 0A0h, 02Eh, 000h, 000h, 013h
        add    byte ptr [eax + eax], dl
        adc    byte ptr [edi], ch
        add    byte ptr [eax], al
        adc    al, 0
        add    dword ptr [eax], eax
        xor    byte ptr [edi], ch
        add    byte ptr [eax], al
        db  015h, 000h, 001h, 000h, 060h
        xor    byte ptr [eax], al
        add    byte ptr [esi], dl
        add    byte ptr [edi], dl
        db  000h, 050h, 01Fh
        add    byte ptr [eax], al
        pop    ss
        add    byte ptr [esi], dl
        db  000h, 000h
        and    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [ecx], bl
        db  000h, 070h, 021h
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        sbb    byte ptr [eax], al
        and    byte ptr [ecx], 0
        add    byte ptr [edx], bl
        add    byte ptr [ebx], bl
        db  000h, 020h
        sub    byte ptr [eax], al
        add    byte ptr [ebx], bl
        add    byte ptr [edx], bl
        db  000h, 010h
        sub    dword ptr [eax], eax
        add    byte ptr [eax + eax], bl
        db  01Dh, 000h, 0C0h, 02Eh, 000h
        db  000h, 01Dh, 000h, 01Ch, 000h, 0E0h
        add    byte ptr cs:[eax], al
        push   ds
        sbb    byte ptr [edi], 0
        rcl    byte ptr [edi - 7fe10000h], 1eh
        db  000h, 010h
        cwde
        add    byte ptr [eax], al
        db  020h, 080h, 021h, 000h, 030h, 090h
        add    byte ptr [eax], al
        db  021h, 080h, 020h, 000h, 080h, 090h
        add    byte ptr [eax], al
        db  022h, 080h, 002h, 000h, 0F0h, 096h
        add    byte ptr [eax], al
        db  023h, 080h, 024h, 000h, 020h, 08Eh
        add    byte ptr [eax], al
        and    al, 80h
        add    al, byte ptr [eax]
        db  070h, 094h
        add    byte ptr [eax], al
        db  025h, 080h, 026h, 000h, 0F0h
        xchg   esi, eax
        add    byte ptr [eax], al
        and    byte ptr es:[edi], 0
        lock xchg dword ptr [eax], eax
        add    byte ptr [edi], ah
        sub    byte ptr [eax], 0
        and    byte ptr [esi - 7fd80000h], cl
        add    al, byte ptr [eax]
        db  070h, 094h
        add    byte ptr [eax], al
        sub    dword ptr [eax], eax
        sub    al, byte ptr [eax]
        push   eax
        cmp    al, 0
        add    byte ptr [edx], ch
        add    byte ptr [ecx], al
        db  000h, 0C0h
        cmp    al, 0
        add    byte ptr [ebx], ch
        add    byte ptr [eax + eax], ch
        cmp    byte ptr [2c0000h], 1
        db  000h, 0D0h
        cmp    eax, 2d0000h
        db  02Eh, 000h, 060h, 03Eh
        add    byte ptr [eax], al
        add    byte ptr cs:[ecx], al
        db  000h, 0B0h, 07Fh, 000h, 000h, 02Fh
        add    byte ptr [eax], dh
        db  000h, 080h, 04Ah, 000h, 000h, 030h
        add    byte ptr [ecx], al
        db  000h, 0C0h
        dec    ebx
        add    byte ptr [eax], al
        xor    dword ptr [eax], eax
        xor    al, byte ptr [eax]
        mov    al, 3eh
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        add    dword ptr [eax], eax
        add    byte ptr [ecx], 0
        xor    eax, dword ptr [eax]
        add    dword ptr [eax], eax
        db  0E0h, 041h
        add    byte ptr [eax], al
        xor    al, 0
        add    dword ptr [eax], eax
        inc    eax
        inc    edx
        add    byte ptr [eax], al
        db  035h, 000h, 001h, 000h, 070h
        inc    edx
        add    byte ptr [eax], al
        add    byte ptr ss:[edi], dh
        db  000h, 000h
        dec    ebp
        add    byte ptr [eax], al
        aaa
        add    byte ptr [ecx], al
        db  000h, 090h, 04Eh, 000h, 000h, 038h
        add    byte ptr [ecx], bh
        db  000h, 0D0h
        pop    esp
        add    byte ptr [eax], al
        cmp    dword ptr [eax], eax
        cmp    al, byte ptr [eax]
        nop
        pop    edi
        add    byte ptr [eax], al
        cmp    al, byte ptr [eax]
        cmp    dword ptr [eax], eax
        nop
        popad
        add    byte ptr [eax], al
        cmp    eax, dword ptr [eax]
        add    dword ptr [eax], eax
        xor    byte ptr [ebx], bl
        add    byte ptr [eax], al
        cmp    al, 0
        db  03Dh, 000h, 080h, 01Bh, 000h
        db  000h, 03Dh, 000h, 001h, 000h, 020h
        sbb    al, 0
        add    byte ptr [esi], bh
        add    byte ptr [edi], bh
        db  000h, 0A0h, 015h, 000h, 000h, 03Fh
        add    byte ptr [ecx], al
        db  000h, 0C0h
        adc    eax, 400000h
        inc    ecx
        db  000h, 030h
        push   cs
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [ecx], al
        db  000h, 0E0h
        push   cs
        add    byte ptr [eax], al
        inc    edx
        add    byte ptr [ebx], al
        adc    byte ptr [esi], ah
        add    byte ptr [ebx], al
        inc    edx
        db  000h, 070h, 066h
        add    byte ptr [eax], al
        inc    esp
        add    byte ptr [ebp], al
        mov    al, byte ptr [45000016h]
        add    byte ptr [ecx], al
        db  000h, 0A0h, 017h, 000h, 000h, 046h
        db  080h, 047h, 000h, 0D0h
        test   dword ptr [eax], eax
        add    byte ptr [edi - 80h], al
        inc    esi
        db  000h, 030h
        xchg   byte ptr [eax], al
        add    byte ptr [eax - 80h], cl
        dec    ecx
        db  000h, 020h
        insd
        add    byte ptr [eax], al
        dec    ecx
        db  080h, 048h, 000h, 040h
        outsd
        add    byte ptr [eax], al
        dec    edx
        db  080h, 04Bh, 000h, 000h
        bound  eax, qword ptr [eax]
        add    byte ptr [ebx - 80h], cl
        add    al, byte ptr [eax]
        mov    al, byte ptr [4c000063h]
        add    byte ptr [ecx], al
        db  000h, 0C0h
        sbb    al, byte ptr [eax]
        add    byte ptr [ebp], cl
        add    dword ptr [eax], eax
        db  0F0h, 042h, 000h, 000h, 04Eh, 000h, 04Fh, 000h, 060h, 04Ah, 000h, 000h, 04Fh, 000h, 001h, 000h
        db  090h, 04Eh, 000h, 000h, 050h, 000h, 001h, 000h, 010h, 03Eh, 000h, 000h
loc_3_A334:
        inc    ebx
        inc    esp
        push   edx
        dec    edi
        dec    ebp
        add    byte ptr [eax], al
        db  000h
loc_3_A33C:
        inc    ebx
        inc    esp
        xor    byte ptr [eax], dh
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_A344:
        inc    ebx
        inc    esp
        push   edx
        dec    edi
        dec    ebp
        add    byte ptr [eax], al
        db  000h
loc_3_A34C:
        inc    ebx
        inc    esp
        xor    byte ptr [eax], dh
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_A354:
        inc    ebx
        inc    esp
        sub    eax, 30304158h
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_PTEXT ends
        end
