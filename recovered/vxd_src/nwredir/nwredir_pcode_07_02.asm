; nwredir_pcode_07_02.asm -- NWREDIR object 7 segment _PTEXT  (part 2/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_7_4FFC:
        sub    esp, 0ch
        push   ebp
        push   esi
        push   edi
        push   ebx
        sub    eax, eax
        mov    ecx, dword ptr [esp + 20h]
        mov    ebx, 0fffff000h
        mov    dword ptr [esp + 10h], eax
        mov    edi, dword ptr [ecx + 2ch]
        mov    ebp, dword ptr [ecx + 48h]
        mov    eax, ebp
        and    eax, 0fffh
        mov    dword ptr [esp + 18h], eax
        and    ebx, ebp
        add    ebp, dword ptr [ecx + 4ch]
loc_7_5028:
        db  08Ah, 025h, 016h, 00Eh, 000h, 000h
        mov    al, 7
        call   sub_7_46A0
        or     esi, esi
        je     NEAR PTR loc_7_534A
loc_7_503D:
        test   byte ptr [esi + 16h], 0ffh
        jne    SHORT loc_7_4FE8
        call   sub_7_48DE
        inc    dword ptr [esp + 10h]
        mov    dword ptr [esp + 14h], esi
        push   edi
        mov    edx, ebx
        mov    edi, dword ptr [esi + 10h]
        add    edx, 1000h
        add    edi, dword ptr [esp + 1ch]
        cmp    edx, ebp
        jb     SHORT loc_7_5066
        mov    edx, ebp
loc_7_5066:
        sub    edx, ebx
        sub    edx, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, ecx
        add    ecx, 1ffh
        mov    dword ptr [esp + 1ch], 0
        add    eax, edx
        and    ecx, 0fffffe00h
        sub    eax, ecx
        jbe    SHORT loc_7_50A0
        shr    ecx, 9
        shr    eax, 9
        je     SHORT loc_7_50A0
        add    al, 8
        bts    ecx, eax
        dec    ch
        shl    ch, cl
        or     byte ptr [esi + 14h], ch
loc_7_50A0:
        mov    esi, dword ptr [esp + 28h]
        add    dword ptr [esp + 28h], edx
        mov    ecx, edx
        cmp    ecx, 10h
        jbe    SHORT loc_7_50C9
        mov    ecx, 4
        sub    ecx, esi
        and    ecx, 3
        sub    edx, ecx
        rep movsb
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
loc_7_50C9:
        rep movsb
        add    ebx, 1000h
        pop    edi
        cmp    ebx, ebp
        jb     NEAR PTR loc_7_5028
        mov    ebp, dword ptr [esp + 20h]
        mov    eax, dword ptr [ebp + 48h]
        mov    ebx, dword ptr [ebp + 4ch]
        mov    esi, dword ptr [ebp + 18h]
        add    ebx, eax
        mov    edi, esi
loc_7_50EB:
        mov    edi, dword ptr [edi]
        cmp    edi, esi
        je     NEAR PTR loc_7_5286
        cmp    byte ptr [edi + 14h], 1
        jne    SHORT loc_7_50EB
        mov    ecx, dword ptr [edi + 2ch]
        cmp    ecx, dword ptr [ebp + 2ch]
        jne    SHORT loc_7_50EB
        test   byte ptr [edi + 24h], 20h
        je     SHORT loc_7_50EB
        mov    ecx, dword ptr [edi + 48h]
        mov    edx, dword ptr [edi + 4ch]
        add    edx, ecx
        cmp    ebx, ecx
        jb     SHORT loc_7_50EB
        cmp    edx, eax
        jb     SHORT loc_7_50EB
        cmp    eax, ecx
        jae    NEAR PTR loc_7_51D3
        and    ecx, 0fffh
        je     SHORT loc_7_5159
loc_7_5129:
        mov    eax, dword ptr [edi + 48h]
        sub    eax, dword ptr [ebp + 48h]
        cmp    eax, ecx
        ja     SHORT loc_7_5135
        mov    ecx, eax
loc_7_5135:
        sub    dword ptr [edi + 48h], ecx
        add    dword ptr [edi + 4ch], ecx
        mov    esi, dword ptr [edi + 0ch]
        sub    dword ptr [esi + 14h], ecx
        add    dword ptr [esi + 1ch], ecx
        mov    esi, dword ptr [esi]
        sub    dword ptr [esi + 14h], ecx
        add    dword ptr [esi + 10h], ecx
        neg    ecx
        add    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 0ch], ecx
        cmp    ecx, dword ptr [ebp + 48h]
        je     SHORT loc_7_51C7
loc_7_5159:
        call   sub_7_4831
        mov    esi, eax
        mov    edx, dword ptr [edi + 0ch]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [edx], esi
        mov    dword ptr [esi + 20h], edx
        inc    byte ptr [edx + 2dh]
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 1ch]
        mov    edx, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 0ch], edx
        mov    edx, dword ptr [eax + 10h]
        mov    ecx, 1000h
        add    edx, ecx
        mov    dword ptr [esi + 14h], edx
        mov    dword ptr [esi + 18h], edx
        mov    dword ptr [esi + 10h], 0
        mov    dword ptr [esi + 24h], 14h
        mov    edx, dword ptr [eax + 2ch]
        mov    dword ptr [edx + 28h], esi
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [esi + 2ch], edx
        mov    dword ptr [eax + 2ch], esi
        xchg   esi, eax
        call   sub_7_556A
        xchg   esi, eax
        inc    word ptr [eax + 19h]
        mov    byte ptr [eax + 32h], 1
        jmp    NEAR PTR loc_7_5129
loc_7_51C7:
        mov    ebx, dword ptr [ebp + 48h]
        mov    edx, dword ptr [edi + 48h]
        add    ebx, dword ptr [ebp + 4ch]
        add    edx, dword ptr [edi + 4ch]
loc_7_51D3:
        cmp    ebx, edx
        jbe    NEAR PTR loc_7_5356
        mov    esi, dword ptr [edi + 0ch]
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [eax + 0ch]
        and    ecx, 0fffh
        add    ecx, dword ptr [eax + 10h]
        neg    ecx
        add    ecx, 1000h
        je     SHORT loc_7_5217
loc_7_51F7:
        mov    eax, ebx
        sub    eax, edx
        cmp    ecx, eax
        jb     SHORT loc_7_5201
        mov    ecx, eax
loc_7_5201:
        add    dword ptr [edi + 4ch], ecx
        add    edx, ecx
        add    dword ptr [esi + 1ch], ecx
        mov    eax, dword ptr [esi + 4]
        add    dword ptr [eax + 10h], ecx
        cmp    ebx, edx
        je     NEAR PTR loc_7_5356
loc_7_5217:
        call   sub_7_4831
        mov    ebp, eax
        mov    ecx, dword ptr [edi + 0ch]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [eax], ebp
        mov    dword ptr [ebp], ecx
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [ecx + 4], ebp
        mov    dword ptr [ebp + 20h], ecx
        inc    byte ptr [esi + 2dh]
        mov    eax, dword ptr [ebp + 4]
        mov    eax, dword ptr [eax + 1ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp + 1ch], eax
        mov    dword ptr [ebp + 24h], 14h
        mov    ecx, dword ptr [eax + 2ch]
        mov    dword ptr [ecx + 28h], ebp
        mov    dword ptr [ebp + 28h], eax
        mov    dword ptr [ebp + 2ch], ecx
        mov    dword ptr [eax + 2ch], ebp
        xchg   esi, eax
        call   sub_7_556A
        xchg   esi, eax
        inc    word ptr [eax + 19h]
        mov    byte ptr [eax + 32h], 1
        push   dword ptr [eax + 8]
        pop    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp + 14h], eax
        mov    dword ptr [ebp + 18h], eax
        mov    dword ptr [ebp + 10h], 0
        mov    ecx, 1000h
        jmp    NEAR PTR loc_7_51F7
loc_7_5286:
        call   sub_7_4831
        mov    edi, eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 0ch], edi
        mov    dword ptr [edi + 0ch], ebp
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [ebp + 10h], edi
        mov    dword ptr [edi + 24h], ebp
        sub    eax, eax
        mov    dword ptr [edi + 18h], eax
        mov    byte ptr [edi + 2dh], al
        mov    dword ptr [edi + 1ch], eax
        mov    dword ptr [edi], edi
        mov    dword ptr [edi + 4], edi
        mov    ebx, dword ptr [esp + 14h]
        mov    edx, dword ptr [esp + 10h]
loc_7_52B7:
        call   sub_7_4831
        mov    esi, eax
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [edi], esi
        mov    eax, dword ptr [ebx + 2ch]
        mov    dword ptr [eax + 28h], esi
        mov    dword ptr [esi + 28h], ebx
        mov    dword ptr [esi + 2ch], eax
        mov    dword ptr [ebx + 2ch], esi
        mov    dword ptr [esi + 1ch], ebx
        xchg   ebx, esi
        call   sub_7_556A
        xchg   ebx, esi
        inc    word ptr [ebx + 19h]
        mov    byte ptr [ebx + 32h], 1
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [esi + 14h], eax
        mov    dword ptr [esi + 18h], eax
        mov    eax, 1000h
        mov    dword ptr [esi + 10h], eax
        add    dword ptr [edi + 1ch], eax
        inc    byte ptr [edi + 2dh]
        mov    dword ptr [esi + 24h], 14h
        mov    dword ptr [esi + 20h], edi
        dec    edx
        mov    ebx, dword ptr [ebx + 20h]
        jne    SHORT loc_7_52B7
        mov    eax, dword ptr [ebp + 48h]
        sub    eax, dword ptr [esi + 0ch]
        sub    dword ptr [edi + 1ch], eax
        sub    dword ptr [esi + 10h], eax
        add    dword ptr [esi + 14h], eax
        add    dword ptr [esi + 18h], eax
        add    eax, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [edi + 1ch]
        sub    eax, dword ptr [ebp + 4ch]
        sub    dword ptr [edi + 1ch], eax
        mov    esi, dword ptr [edi + 4]
        sub    dword ptr [esi + 10h], eax
        or     byte ptr [ebp + 24h], 20h
        jmp    SHORT loc_7_5356
loc_7_534A:
        sub    ebp, ebp
        inc    ebp
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_7_5376
        jmp    SHORT loc_7_5358
loc_7_5356:
        sub    ebp, ebp
loc_7_5358:
        mov    ecx, dword ptr [esp + 14h]
loc_7_535C:
        mov    esi, ecx
        mov    ecx, dword ptr [esi + 20h]
        db  068h, 070h, 053h, 000h, 000h
        db  068h, 070h, 055h, 000h, 000h
        jmp    NEAR PTR loc_7_48FC
loc_7_5370:
        dec    dword ptr [esp + 10h]
        jne    SHORT loc_7_535C
loc_7_5376:
        mov    eax, ebp
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        add    esp, 0ch
        ret
sub_7_5380:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        sub    dword ptr [ebp + 14h], eax
        jbe    NEAR PTR loc_7_541E
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 10h]
        sub    edi, dword ptr [eax + 24h]
        add    edi, dword ptr [eax + 14h]
        mov    ebx, dword ptr [ebp + 8]
        sub    esi, esi
loc_7_53A3:
        mov    ebx, dword ptr [ebx]
        cmp    esi, dword ptr [ebx + 1ch]
        je     SHORT loc_7_53A3
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebx + 1ch]
        and    eax, 0fffff000h
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_7_53A3
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 14h]
        push   ebp
        mov    ebp, esi
loc_7_53C3:
        mov    esi, edx
        and    esi, 0fffh
        mov    ecx, 1000h
        sub    ecx, esi
        add    esi, dword ptr [ebp + 10h]
        cmp    ecx, eax
        jb     SHORT loc_7_53DB
        mov    ecx, eax
loc_7_53DB:
        add    edx, ecx
        sub    eax, ecx
        push   ecx
        mov    ecx, dword ptr [esp]
        cmp    ecx, 10h
        jbe    SHORT loc_7_5405
        mov    ecx, 4
        sub    ecx, esi
        and    ecx, 3
        sub    dword ptr [esp], ecx
        rep movsb
        mov    ecx, dword ptr [esp]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [esp]
        and    ecx, 3
loc_7_5405:
        rep movsb
        add    esp, 4
        or     eax, eax
        je     SHORT loc_7_541A
loc_7_540E:
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebx + 1ch], ebp
        je     SHORT loc_7_540E
        mov    ebp, dword ptr [ebx + 1ch]
        jmp    SHORT loc_7_53C3
loc_7_541A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
loc_7_541E:
        leave
        ret
loc_7_5420:
        mov    edx, ebx
loc_7_5422:
        mov    edx, dword ptr [edx]
        cmp    ebx, edx
        je     SHORT loc_7_5480
        mov    eax, dword ptr [edx + 0ch]
        mov    edi, dword ptr [ebp + 24h]
        cmp    edi, eax
        jae    SHORT loc_7_5434
        mov    edi, eax
loc_7_5434:
        add    eax, dword ptr [edx + 10h]
        mov    ecx, dword ptr [ebp + 24h]
        add    ecx, dword ptr [ebp]
        cmp    ecx, eax
        jb     SHORT loc_7_5443
        mov    ecx, eax
loc_7_5443:
        sub    ecx, edi
        jbe    SHORT loc_7_547E
        mov    eax, dword ptr [ebp + 14h]
        mov    esi, edi
        sub    eax, dword ptr [ebp + 24h]
        add    edi, eax
        mov    eax, dword ptr [edx + 18h]
        sub    eax, dword ptr [edx + 0ch]
        add    esi, eax
        mov    eax, ecx
        mov    ecx, eax
        cmp    ecx, 10h
        jbe    SHORT loc_7_547C
        mov    ecx, 4
        sub    ecx, esi
        and    ecx, 3
        sub    eax, ecx
        rep movsb
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
loc_7_547C:
        rep movsb
loc_7_547E:
        jmp    SHORT loc_7_5422
loc_7_5480:
        ret
        db  053h, 056h, 057h, 055h, 08Bh, 05Ch, 024h, 014h, 08Bh, 043h, 024h, 08Bh, 068h, 028h, 0E8h, 08Ch
        db  0FFh, 0FFh, 0FFh, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h
sub_7_5499:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 14h]
        mov    ebp, dword ptr [edx + 28h]
        mov    ebx, dword ptr [esp + 14h]
loc_7_54A8:
        mov    ebx, dword ptr [ebx + 0ch]
        cmp    ebx, dword ptr [esp + 14h]
        je     SHORT loc_7_54D7
        test   byte ptr [ebx + 18h], 1
        je     SHORT loc_7_54A8
        mov    eax, dword ptr [ebx + 14h]
        add    eax, dword ptr [ebx + 1ch]
        mov    edx, dword ptr [esp + 14h]
        cmp    eax, dword ptr [edx + 20h]
        db  068h, 0A8h, 054h, 000h, 000h
        jb     NEAR PTR loc_7_5420
        mov    dword ptr [edx + 20h], eax
        jmp    NEAR PTR loc_7_5420
loc_7_54D7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_7_54DC:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_7_554C
        push   ebx
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [eax + 0ch]
        jmp    SHORT loc_7_54F5
loc_7_54F3:
        mov    eax, dword ptr [eax]
loc_7_54F5:
        add    edx, dword ptr [eax + 10h]
        cmp    edx, ecx
        jbe    SHORT loc_7_54F3
        mov    ebx, dword ptr [eax + 1ch]
        sub    ecx, dword ptr [ebx + 8]
        shr    ecx, 9
        inc    ch
        mov    edx, dword ptr [esp + 10h]
        shl    ch, cl
        shr    edx, 9
loc_7_5510:
        or     byte ptr [ebx + 14h], ch
        xor    byte ptr [ebx + 16h], ch
        test   byte ptr [ebx + 17h], ch
        jne    SHORT loc_7_5536
        cmp    byte ptr [ebx + 18h], 0
        jne    SHORT loc_7_5539
loc_7_5521:
        dec    edx
        je     SHORT loc_7_554B
        add    ch, ch
        jne    SHORT loc_7_5510
loc_7_5528:
        mov    eax, dword ptr [eax]
        cmp    ebx, dword ptr [eax + 1ch]
        je     SHORT loc_7_5528
        mov    ebx, dword ptr [eax + 1ch]
        inc    ch
        jmp    SHORT loc_7_5510
loc_7_5536:
        xor    byte ptr [ebx + 17h], ch
loc_7_5539:
        push   eax
        push   ecx
        push   edx
        push   ebx
        db  0CDh, 020h, 013h, 000h, 040h, 000h
        add    esp, 4
        pop    edx
        pop    ecx
        pop    eax
        jmp    SHORT loc_7_5521
loc_7_554B:
        pop    ebx
loc_7_554C:
        ret
        db  08Bh, 0D6h, 08Bh, 074h, 024h, 004h, 0CDh, 020h, 00Dh, 000h, 08Bh, 004h, 08Bh, 0F2h, 0C3h
sub_7_555C:
        mov    edx, esi
        mov    esi, dword ptr [esp + 4]
        call   sub_7_556A
        mov    esi, edx
        ret
sub_7_556A:
        db  0CDh, 020h, 007h, 080h, 08Bh, 004h
sub_7_5570:
        cmp    byte ptr [esi + 18h], 0
        jne    SHORT loc_7_557C
loc_7_5576:
        db  0CDh, 020h, 008h, 080h, 08Bh, 004h
loc_7_557C:
        pushad
        push   esi
        db  0CDh, 020h, 013h, 000h, 040h, 000h
        add    esp, 4
        popad
        jmp    SHORT loc_7_5576
sub_7_558A:
        mov    edx, esi
        mov    esi, dword ptr [esp + 4]
        call   sub_7_5570
        mov    esi, edx
        ret
sub_7_5598:
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    edi, dword ptr [esp + 10h]
        mov    esi, edi
loc_7_55A3:
        mov    esi, dword ptr [esi]
        cmp    esi, edi
        je     SHORT loc_7_55BD
        test   byte ptr [esi + 54h], 80h
        je     SHORT loc_7_55A3
        push   esi
        inc    ebx
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edx], eax
        jmp    SHORT loc_7_55A3
loc_7_55BD:
        or     ebx, ebx
        je     SHORT loc_7_55E1
loc_7_55C1:
        mov    esi, dword ptr [esp]
        mov    esi, dword ptr [esi + 78h]
        or     esi, esi
        je     SHORT loc_7_55D5
        push   esi
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 4
loc_7_55D5:
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 4
        dec    ebx
        jne    SHORT loc_7_55C1
loc_7_55E1:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_7_55E5:
        push   ebp
        mov    ebp, esp
        movzx  ecx, byte ptr [ebp + 8]
        db  00Fh, 0B7h, 004h, 04Dh, 01Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        leave
        ret
        db  033h, 006h, 000h, 000h, 000h, 037h, 007h, 000h, 000h, 000h
sub_7_5606:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 0ch]
        mov    edx, 8
        db  08Dh, 00Dh, 0F7h, 055h, 000h, 000h
loc_7_5617:
        add    ecx, 5
        db  081h, 0F9h, 006h, 056h, 000h, 000h
        je     SHORT loc_7_5629
        cmp    byte ptr [ecx], al
        jne    SHORT loc_7_5617
        mov    edx, dword ptr [ecx + 1]
loc_7_5629:
        test   dh, 80h
        je     SHORT loc_7_563E
        db  00Fh, 0B7h, 004h, 055h, 00Ah, 000h, 0FFh, 0FFh
        db  003h, 005h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_7_564C
loc_7_563E:
        db  00Fh, 0B7h, 004h, 055h, 00Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_7_564C:
        leave
        ret
sub_7_564E:
        push   ebp
        mov    ebp, esp
        db  00Fh, 0B7h, 005h, 01Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  00Fh, 0B7h, 005h, 014h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  00Fh, 0B7h, 005h, 012h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  00Fh, 0B7h, 005h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   dword ptr [ebp + 8]
        db  0E8h, 0D1h, 071h, 0F9h, 0FFh
        add    esp, 14h
        leave
        ret
sub_7_56A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 0c0001035h
        mov    ecx, dword ptr [ebp + 8]
        db  00Fh, 0B7h, 03Dh, 00Ch, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        pop    edi
        pop    ebx
        leave
        ret
        db  000h, 000h
_PTEXT ends
        end
