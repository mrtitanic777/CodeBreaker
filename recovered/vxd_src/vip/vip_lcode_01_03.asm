; vip_lcode_01_03.asm -- VIP object 1 segment _LTEXT  (part 3/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_8EF4:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   1
        push   2
        push   1
        push   100h
        db  0A1h, 050h, 0C0h, 000h, 000h
        push   dword ptr [eax + 0ch]
        push   -1
        push   -1
        push   dword ptr [esi + 4]
        call   sub_1_7F60
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_8F28
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_8F28:
        mov    al, byte ptr [esi + 4]
        shr    al, 2
        and    eax, 0ffh
        and    eax, 0fffffffch
        db  08Bh, 088h, 000h, 0BDh, 000h, 000h
        mov    edi, dword ptr [esi + 0ch]
        mov    ebx, dword ptr [edi + 3ch]
        mov    eax, ecx
        not    eax
        and    ebx, eax
        mov    eax, dword ptr [esi + 4]
        and    eax, ecx
        or     ebx, eax
        push   20h
        push   0
        push   ebx
        call   sub_1_7420
        add    esp, 0ch
        or     eax, eax
        mov    eax, 0
        jne    SHORT loc_1_8F7E
        push   0
        push   2
        push   1
        movzx  eax, word ptr [esi + 3ch]
        push   eax
        push   edi
        push   -1
        push   -1
        push   ebx
        call   sub_1_7C3C
        add    esp, 20h
loc_1_8F7E:
        or     eax, eax
        je     SHORT loc_1_8F88
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_8F88:
        db  083h, 03Dh, 0F8h, 0C2h, 000h, 000h, 000h
        je     SHORT loc_1_8FC0
        push   0
        push   2
        push   1
        movzx  eax, word ptr [esi + 3ch]
        push   eax
        push   dword ptr [esi + 0ch]
        push   -1
        push   0e0h
        push   0e0h
        call   sub_1_7F60
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_8FC0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_8FC0:
        mov    ecx, dword ptr [esi + 8]
        push   0
        push   2
        push   1
        movzx  eax, word ptr [esi + 3ch]
        push   eax
        push   dword ptr [esi + 0ch]
        push   -1
        push   ecx
        mov    eax, dword ptr [esi + 4]
        and    eax, ecx
        push   eax
        call   sub_1_7F60
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_8FEC
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_8FEC:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_8FF8:
        push   ebx
        push   esi
        push   edi
        call   sub_1_B94A
        mov    esi, dword ptr [esp + 10h]
        db  039h, 035h, 050h, 0C0h, 000h, 000h
        je     NEAR PTR loc_1_90BB
        movzx  eax, word ptr [esi + 3ch]
        db  08Bh, 00Dh, 06Ch, 0BDh, 000h, 000h
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        db  0A3h, 06Ch, 0BDh, 000h, 000h
        push   1
        push   2
        push   1
        push   eax
        db  0FFh, 035h, 0C8h, 0C1h, 000h, 000h
        push   -1
        push   -1
        mov    eax, dword ptr [esi + 0ch]
        push   dword ptr [eax + 3ch]
        call   sub_1_7F60
        add    esp, 20h
        test   byte ptr [esi + 14h], 1
        je     SHORT loc_1_90BB
        push   esi
        call   sub_1_8EF4
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_9060
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_9060:
        mov    edi, dword ptr [esp + 14h]
        or     edi, edi
        je     SHORT loc_1_90BB
        mov    ebx, dword ptr [esp + 18h]
loc_1_906C:
        mov    edx, dword ptr [ebx]
        mov    ecx, edx
        and    ecx, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    edx, 10h
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        cmp    dword ptr [esi + 4], ecx
        push   1
        push   2
        push   1
        movzx  eax, word ptr [esi + 3ch]
        push   eax
        push   dword ptr [esi + 0ch]
        jne    SHORT loc_1_90A8
        push   -1
        jmp    SHORT loc_1_90A9
loc_1_90A8:
        push   ecx
loc_1_90A9:
        push   0
        push   0
        call   sub_1_7F60
        add    esp, 20h
        add    ebx, 4
        dec    edi
        jne    SHORT loc_1_906C
loc_1_90BB:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret
sub_1_90C4:
        movzx  eax, byte ptr [esp + 8]
        cmp    eax, 12h
        ja     SHORT loc_1_912F
        db  0FFh, 024h, 085h, 030h, 091h, 000h, 000h
        db  08Dh, 049h, 000h
loc_1_90D8:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 20h]
        ret
loc_1_90E0:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 8]
        ret
loc_1_90E8:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 14h]
        ret
loc_1_90F0:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 18h]
        ret
loc_1_90F8:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 1ch]
        ret
loc_1_9100:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0ch]
        ret
loc_1_9108:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 10h]
        ret
loc_1_9110:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 24h]
        ret
loc_1_9118:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 28h]
        ret
loc_1_9120:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 2ch]
        ret
loc_1_9128:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 30h]
loc_1_912F:
        ret
        db  0D8h, 090h, 000h, 000h, 02Fh, 091h, 000h, 000h, 02Fh, 091h, 000h, 000h, 0E0h, 090h, 000h, 000h
        db  0E8h, 090h, 000h, 000h, 0F0h, 090h, 000h, 000h, 02Fh, 091h, 000h, 000h, 02Fh, 091h, 000h, 000h
        db  0F8h, 090h, 000h, 000h, 02Fh, 091h, 000h, 000h, 02Fh, 091h, 000h, 000h, 000h, 091h, 000h, 000h
        db  008h, 091h, 000h, 000h, 010h, 091h, 000h, 000h, 018h, 091h, 000h, 000h, 02Fh, 091h, 000h, 000h
        db  02Fh, 091h, 000h, 000h, 020h, 091h, 000h, 000h, 028h, 091h, 000h, 000h
sub_1_917C:
        sub    esp, 8
        push   esi
        push   edi
        db  0A1h, 044h, 0C2h, 000h, 000h
        mov    dword ptr [esp + 8], eax
        or     eax, eax
        jne    SHORT loc_1_91B0
        push   60h
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 8], eax
        or     eax, eax
        jne    SHORT loc_1_91A8
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_1_91A8:
        db  0FFh, 005h, 064h, 0C0h, 000h, 000h
        jmp    SHORT loc_1_91B7
loc_1_91B0:
        mov    eax, dword ptr [eax]
        db  0A3h, 044h, 0C2h, 000h, 000h
loc_1_91B7:
        mov    esi, dword ptr [esp + 14h]
        lea    eax, [esi + 14h]
        push   eax
        push   dword ptr [esp + 0ch]
        db  0FFh, 035h, 0F4h, 0C2h, 000h, 000h
        mov    edi, dword ptr [esp + 24h]
        push   edi
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_9200
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 0ch], esi
        add    dword ptr [esp + 8], 14h
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 2], 0
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_9200:
        db  0A1h, 064h, 0C0h, 000h, 000h
        db  039h, 005h, 0ACh, 0C0h, 000h, 000h
        jae    SHORT loc_1_9228
        db  0FFh, 00Dh, 064h, 0C0h, 000h, 000h
        push   dword ptr [esp + 8]
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  090h
loc_1_9228:
        db  0A1h, 044h, 0C2h, 000h, 000h
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        db  0A3h, 044h, 0C2h, 000h, 000h
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 8
        ret
sub_1_9244:
        sub    esp, 8
        push   esi
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 8]
        push   eax
        mov    esi, dword ptr [esp + 18h]
        push   esi
        call   sub_1_BA44
        db  0A1h, 064h, 0C0h, 000h, 000h
        db  039h, 005h, 0ACh, 0C0h, 000h, 000h
        jae    SHORT loc_1_9288
        db  0FFh, 00Dh, 064h, 0C0h, 000h, 000h
        push   dword ptr [esp + 4]
        call   sub_1_B944
        add    esp, 4
        push   esi
        call   sub_1_BA3E
        pop    esi
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_9288:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        db  0A1h, 044h, 0C2h, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        db  0A3h, 044h, 0C2h, 000h, 000h
        call   sub_1_BA3E
        pop    esi
        add    esp, 8
        ret
        db  090h
sub_1_92A8:
        mov    eax, dword ptr [esp + 4]
        lea    edx, [eax + 20h]
        mov    eax, dword ptr [eax + 20h]
        or     eax, eax
        je     SHORT loc_1_92D0
        mov    cx, word ptr [esp + 8]
        nop
loc_1_92BC:
        cmp    word ptr [eax + 0ch], cx
        je     SHORT loc_1_92CC
        mov    edx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_92BC
        ret
        db  090h
loc_1_92CC:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
loc_1_92D0:
        ret
        db  08Dh, 049h, 000h
sub_1_92D4:
        sub    esp, 4
        push   esi
        lea    eax, [esp + 4]
        push   eax
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   sub_1_BA68
        push   esi
        call   sub_1_9244
        add    esp, 4
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_930C
        push   dword ptr [esp + 0ch]
        call   sub_1_B944
        add    esp, 4
        push   dword ptr [esp + 4]
        call   sub_1_BA3E
loc_1_930C:
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_9314:
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [esp + 0ch]
loc_1_931C:
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        call   sub_1_5C30
        add    esp, 8
        and    eax, 0ffffh
        add    edi, eax
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_931C
        mov    eax, edi
        and    eax, 0ffffh
        shr    edi, 10h
        add    eax, edi
        mov    ecx, eax
        shr    ecx, 10h
        add    eax, ecx
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_9350:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        db  0FFh, 005h, 070h, 0BFh, 000h, 000h
        lea    eax, [esp + 1ch]
        push   eax
        push   8
        call   sub_1_917C
        add    esp, 8
        mov    dword ptr [esp + 18h], eax
        cmp    eax, ebx
        jne    SHORT loc_1_938C
        db  0FFh, 005h, 074h, 0BFh, 000h, 000h
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
loc_1_938C:
        mov    al, byte ptr [esp + 34h]
        mov    ecx, dword ptr [esp + 18h]
        mov    byte ptr [ecx], al
        mov    byte ptr [ecx + 1], 0
        mov    ax, word ptr [esp + 38h]
        mov    word ptr [ecx + 4], ax
        mov    ax, word ptr [esp + 3ch]
        mov    word ptr [ecx + 6], ax
        push   8
        push   dword ptr [esp + 1ch]
        call   sub_1_5C30
        add    esp, 8
        cmp    dword ptr [esp + 44h], 0
        mov    word ptr [esp + 12h], ax
        not    ax
        mov    ecx, dword ptr [esp + 18h]
        mov    word ptr [ecx + 2], ax
        je     NEAR PTR loc_1_94D1
        push   dword ptr [esp + 44h]
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_9408
        push   dword ptr [esp + 1ch]
        call   sub_1_9244
        add    esp, 4
        db  0FFh, 005h, 074h, 0BFh, 000h, 000h
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_9408:
        mov    edx, dword ptr [esp + 44h]
        mov    dword ptr [esp + 14h], 0
        mov    ebp, dword ptr [esp + 40h]
loc_1_9418:
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    edi, dword ptr [esp + 14h]
        add    edi, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ebp, dword ptr [ebp]
        add    dword ptr [esp + 14h], eax
        sub    edx, eax
        jne    SHORT loc_1_9418
        push   dword ptr [esp + 44h]
        push   ebx
        db  0FFh, 035h, 0F4h, 0C2h, 000h, 000h
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 34h]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_9490
        push   ebx
        call   sub_1_B944
        add    esp, 4
        push   dword ptr [esp + 1ch]
        call   sub_1_9244
        add    esp, 4
        db  0FFh, 005h, 074h, 0BFh, 000h, 000h
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_9490:
        push   dword ptr [esp + 44h]
        push   ebx
        call   sub_1_5C30
        add    esp, 8
        and    eax, 0ffffh
        movzx  ecx, word ptr [esp + 12h]
        add    eax, ecx
        mov    ecx, eax
        shr    ecx, 10h
        and    eax, 0ffffh
        add    eax, ecx
        mov    ecx, eax
        shr    ecx, 10h
        add    eax, ecx
        not    ax
        mov    ecx, dword ptr [esp + 18h]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], eax
loc_1_94D1:
        push   dword ptr [esp + 34h]
        db  068h, 070h, 0BFh, 000h, 000h
        call   sub_1_90C4
        add    esp, 8
        push   0
        push   dword ptr [esp + 4ch]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 38h]
        mov    eax, dword ptr [esp + 54h]
        add    eax, 8
        push   eax
        push   dword ptr [esp + 30h]
        push   ebx
        db  068h, 0FCh, 0BFh, 000h, 000h
        call   sub_1_6848
        add    esp, 20h
        mov    esi, eax
        cmp    esi, 2bf7h
        je     SHORT loc_1_9521
        push   dword ptr [esp + 1ch]
        push   ebx
        call   sub_1_92D4
        add    esp, 8
loc_1_9521:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  090h
sub_1_952C:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0FFh, 005h, 070h, 0BFh, 000h, 000h
        movzx  ebx, byte ptr [esp + 3ch]
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [ebx + 8]
        push   eax
        call   sub_1_917C
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_9568
        db  0FFh, 005h, 074h, 0BFh, 000h, 000h
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_9568:
        mov    al, byte ptr [esp + 2ch]
        mov    byte ptr [ebp], al
        mov    al, byte ptr [esp + 30h]
        mov    byte ptr [ebp + 1], al
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [ebp + 4], eax
        mov    esi, dword ptr [esp + 38h]
        or     esi, esi
        je     SHORT loc_1_9596
        lea    edi, [ebp + 8]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_1_9596:
        lea    eax, [ebx + 8]
        push   eax
        push   ebp
        call   sub_1_5C30
        add    esp, 8
        not    ax
        mov    word ptr [ebp + 2], ax
        lea    eax, [esp + 14h]
        push   eax
        call   sub_1_070C
        add    esp, 4
        push   dword ptr [esp + 2ch]
        db  068h, 070h, 0BFh, 000h, 000h
        call   sub_1_90C4
        add    esp, 8
        push   0
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        lea    eax, [ebx + 8]
        push   eax
        push   dword ptr [esp + 24h]
        push   0
        db  068h, 0FCh, 0BFh, 000h, 000h
        call   sub_1_6848
        add    esp, 20h
        mov    esi, eax
        cmp    esi, 2bf7h
        je     SHORT loc_1_9606
        push   dword ptr [esp + 10h]
        push   0
        call   sub_1_92D4
        add    esp, 8
loc_1_9606:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_9610:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    al, byte ptr [esi]
        and    al, 0fh
        lea    ebx, [eax*4]
        cmp    byte ptr [esi + 9], 1
        jne    SHORT loc_1_9638
        movzx  eax, bl
        cmp    byte ptr [eax + esi], 8
        je     SHORT loc_1_9638
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_9638:
        push   dword ptr [esi + 10h]
        call   sub_1_7044
        add    esp, 4
        cmp    al, 0ffh
        je     SHORT loc_1_96C4
        test   al, 1
        jne    SHORT loc_1_96C4
        push   dword ptr [esi + 0ch]
        call   sub_1_7044
        add    esp, 4
        cmp    al, 0ffh
        je     SHORT loc_1_96BC
        test   al, 1
        jne    SHORT loc_1_96BC
        cmp    byte ptr [esi + 10h], 7fh
        jne    SHORT loc_1_9668
        or     al, al
        jne    SHORT loc_1_96BC
loc_1_9668:
        mov    edi, dword ptr [esp + 10h]
        or     edi, edi
        je     SHORT loc_1_96B4
        push   edi
        call   sub_1_7044
        add    esp, 4
        or     al, al
        jne    SHORT loc_1_96B4
        movzx  eax, word ptr [esi + 6]
        test   eax, 0ffffff1fh
        je     SHORT loc_1_9690
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_9690:
        add    bl, 8
        push   ebx
        push   esi
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esi + 0ch]
        push   edi
        call   sub_1_952C
        add    esp, 1ch
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_96B4:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_96BC:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_96C4:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_96CC:
        push   esi
        push   edi
        sub    esi, esi
        call   sub_1_B92C
        mov    ecx, dword ptr [esp + 0ch]
        lea    edi, [ecx + 20h]
        mov    edx, dword ptr [ecx + 20h]
        cmp    edx, esi
        je     SHORT loc_1_9704
        nop
loc_1_96E4:
        cmp    byte ptr [edx + 0eh], 0
        je     SHORT loc_1_96FC
        cmp    dword ptr [edx + 4], eax
        jae    SHORT loc_1_96FC
        mov    ecx, dword ptr [edx]
        mov    dword ptr [edi], ecx
        mov    dword ptr [edx], esi
        mov    esi, edx
        mov    edx, ecx
        jmp    SHORT loc_1_9700
        db  090h
loc_1_96FC:
        mov    edi, edx
        mov    edx, dword ptr [edx]
loc_1_9700:
        or     edx, edx
        jne    SHORT loc_1_96E4
loc_1_9704:
        or     esi, esi
        je     SHORT loc_1_9722
loc_1_9708:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   0
        push   0
        push   0
        push   2b02h
        push   eax
        call   dword ptr [eax + 8]
        add    esp, 14h
        or     esi, esi
        jne    SHORT loc_1_9708
loc_1_9722:
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_9728:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 4]
        db  08Bh, 015h, 090h, 0C0h, 000h, 000h
        or     edx, edx
        je     SHORT loc_1_9773
loc_1_973A:
        cmp    word ptr [edx + 16h], cx
        je     SHORT loc_1_9746
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_1_973A
loc_1_9746:
        or     edx, edx
        je     SHORT loc_1_9773
        mov    ax, word ptr [eax + 6]
        push   eax
        push   edx
        call   sub_1_92A8
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_9773
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   eax
        call   dword ptr [eax + 8]
        add    esp, 14h
loc_1_9773:
        ret
loc_1_9774:
        sub    esp, 10h
        cmp    byte ptr [esp + 14h], 0
        jne    SHORT loc_1_97B5
        mov    ecx, dword ptr [esp + 2ch]
        cmp    byte ptr [ecx], 8
        jne    SHORT loc_1_97B5
        mov    dword ptr [esp], 0
        lea    eax, [esp + 24h]
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0ch], 4
        push   0
        push   4
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esp + 24h]
        push   ecx
        call   sub_1_9728
        add    esp, 14h
loc_1_97B5:
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
sub_1_97BC:
        movzx  eax, byte ptr [esp + 4]
        cmp    eax, 3
        je     SHORT loc_1_97DC
        cmp    eax, 4
        je     SHORT loc_1_9820
        cmp    eax, 0bh
        je     SHORT loc_1_9828
        cmp    eax, 0ch
        je     SHORT loc_1_9840
        mov    eax, 2b2ah
        ret
        db  090h
loc_1_97DC:
        movzx  eax, byte ptr [esp + 8]
        cmp    eax, 0ch
        ja     SHORT loc_1_97F0
        db  0FFh, 024h, 085h, 048h, 098h, 000h, 000h
        db  08Dh, 049h, 000h
loc_1_97F0:
        mov    eax, 2afah
        ret
        db  08Bh, 0FFh
loc_1_97F8:
        add    eax, 2afah
        ret
        db  08Bh, 0FFh
loc_1_9800:
        mov    eax, 2b01h
        ret
        db  08Bh, 0FFh
loc_1_9808:
        mov    eax, 2b04h
        ret
        db  08Bh, 0FFh
loc_1_9810:
        mov    eax, 2afah
        ret
        db  08Bh, 0FFh
loc_1_9818:
        mov    eax, 2afbh
        ret
        db  08Bh, 0FFh
loc_1_9820:
        mov    eax, 2b08h
        ret
        db  08Bh, 0FFh
loc_1_9828:
        cmp    byte ptr [esp + 8], 0
        jne    SHORT loc_1_9838
        mov    eax, 2b05h
        ret
        db  08Dh, 049h, 000h
loc_1_9838:
        mov    eax, 2b06h
        ret
        db  08Bh, 0FFh
loc_1_9840:
        mov    eax, 2b07h
        ret
        db  08Bh, 0FFh, 0F8h, 097h, 000h, 000h, 0F8h, 097h, 000h, 000h, 0F8h, 097h, 000h, 000h, 0F8h, 097h
        db  000h, 000h, 000h, 098h, 000h, 000h, 008h, 098h, 000h, 000h, 010h, 098h, 000h, 000h, 018h, 098h
        db  000h, 000h, 010h, 098h, 000h, 000h, 010h, 098h, 000h, 000h, 018h, 098h, 000h, 000h, 010h, 098h
        db  000h, 000h, 018h, 098h, 000h, 000h
loc_1_987C:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0FFh, 005h, 0B0h, 0BFh, 000h, 000h
        push   dword ptr [esp + 28h]
        call   sub_1_7044
        add    esp, 4
        mov    ebp, dword ptr [esp + 34h]
        cmp    ebp, 8
        jb     NEAR PTR loc_1_9B00
        cmp    al, 0ffh
        je     NEAR PTR loc_1_9B00
        test   al, 1
        jne    NEAR PTR loc_1_9B00
        cmp    byte ptr [esp + 24h], 7fh
        jne    SHORT loc_1_98C1
        or     al, al
        jne    NEAR PTR loc_1_9B00
loc_1_98C1:
        mov    esi, dword ptr [esp + 30h]
        push   esi
        call   sub_1_9314
        add    esp, 4
        cmp    ax, 0ffffh
        jne    NEAR PTR loc_1_9B00
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esp + 38h], 0
        je     SHORT loc_1_98EC
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_98EC:
        lea    eax, [edi + 8]
        mov    dword ptr [esi + 8], eax
        sub    dword ptr [esi + 0ch], 8
        sub    ebp, 8
        mov    ebx, eax
        jne    SHORT loc_1_98FF
        sub    ebx, ebx
loc_1_98FF:
        mov    al, byte ptr [edi]
        movzx  ecx, al
        cmp    ecx, 11h
        ja     SHORT loc_1_9910
        db  0FFh, 024h, 08Dh, 010h, 09Bh, 000h, 000h
loc_1_9910:
        push   eax
        db  068h, 0B0h, 0BFh, 000h, 000h
        call   sub_1_90C4
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_9928:
        db  0FFh, 005h, 0D0h, 0BFh, 000h, 000h
        push   dword ptr [esp + 40h]
        push   ebp
        push   esi
        push   0
        push   edi
        call   sub_1_9728
        add    esp, 14h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_1_994C:
        or     ebx, ebx
        je     NEAR PTR loc_1_9A40
        cmp    ebp, 14h
        jb     NEAR PTR loc_1_9A40
        mov    al, byte ptr [ebx]
        and    eax, 0fh
        lea    esi, [eax*4]
        lea    eax, [esi + 8]
        cmp    eax, ebp
        jbe    SHORT loc_1_9980
        db  0FFh, 005h, 0B4h, 0BFh, 000h, 000h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_9980:
        push   dword ptr [ebx + 0ch]
        call   sub_1_7044
        add    esp, 4
        or     al, al
        je     SHORT loc_1_99A0
        db  0FFh, 005h, 0B4h, 0BFh, 000h, 000h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
loc_1_99A0:
        mov    al, byte ptr [edi]
        cmp    al, 5
        je     SHORT loc_1_9A14
        push   eax
        db  068h, 0B0h, 0BFh, 000h, 000h
        call   sub_1_90C4
        add    esp, 8
        mov    al, byte ptr [ebx + 9]
        push   eax
        call   sub_1_6CFC
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_99EE
        lea    eax, [ebx + esi]
        push   eax
        push   dword ptr [edi + 4]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 0ch]
        push   dword ptr [ebx + 10h]
        mov    al, byte ptr [edi + 1]
        push   eax
        mov    al, byte ptr [edi]
        push   eax
        call   sub_1_97BC
        add    esp, 8
        push   eax
        push   0
        call   ebp
        add    esp, 1ch
loc_1_99EE:
        cmp    byte ptr [edi + 1], 4
        jne    NEAR PTR loc_1_9AF3
        mov    ax, word ptr [edi + 6]
        mov    ch, al
        mov    cl, ah
        push   ecx
        push   ebx
        call   sub_1_80F4
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_9A14:
        db  0FFh, 005h, 0C8h, 0BFh, 000h, 000h
        push   dword ptr [edi + 4]
        push   dword ptr [ebx + 0ch]
        push   dword ptr [ebx + 10h]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        call   sub_1_7FE0
        add    esp, 14h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_1_9A40:
        sub    eax, eax
        db  0FFh, 005h, 0B4h, 0BFh, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_9A50:
        db  0FFh, 005h, 0CCh, 0BFh, 000h, 000h
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_070C
        add    esp, 4
        mov    ecx, dword ptr [esp + 40h]
        mov    al, byte ptr [ecx + 0ah]
        mov    byte ptr [esp + 1ah], al
        mov    al, byte ptr [ecx + 0bh]
        mov    byte ptr [esp + 1bh], al
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_9A90
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        lea    eax, [esp + 18h]
        push   eax
        push   ecx
        call   sub_1_0498
        add    esp, 10h
loc_1_9A90:
        lea    eax, [esp + 10h]
        push   eax
        push   ebp
        push   esi
        mov    ax, word ptr [edi + 6]
        push   eax
        mov    ax, word ptr [edi + 4]
        push   eax
        push   0
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 44h]
        call   sub_1_9350
        add    esp, 20h
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_0A78
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_9ACC:
        db  0FFh, 005h, 0DCh, 0BFh, 000h, 000h
        push   4
        mov    eax, dword ptr [esp + 24h]
        add    eax, 8
        push   eax
        push   dword ptr [edi + 4]
        push   0
        push   12h
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 44h]
        call   sub_1_952C
        add    esp, 1ch
loc_1_9AF3:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_1_9B00:
        sub    eax, eax
        db  0FFh, 005h, 0B4h, 0BFh, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  028h, 099h, 000h, 000h, 010h, 099h, 000h, 000h, 010h, 099h, 000h, 000h, 04Ch, 099h, 000h, 000h
        db  04Ch, 099h, 000h, 000h, 04Ch, 099h, 000h, 000h, 010h, 099h, 000h, 000h, 010h, 099h, 000h, 000h
        db  050h, 09Ah, 000h, 000h, 010h, 099h, 000h, 000h, 010h, 099h, 000h, 000h, 04Ch, 099h, 000h, 000h
        db  04Ch, 099h, 000h, 000h, 010h, 099h, 000h, 000h, 010h, 099h, 000h, 000h, 010h, 099h, 000h, 000h
        db  010h, 099h, 000h, 000h, 0CCh, 09Ah, 000h, 000h
sub_1_9B58:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 50h]
        cmp    byte ptr [esi + 9], 0
        jne    SHORT loc_1_9B78
        mov    eax, 2affh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Bh, 0FFh
loc_1_9B78:
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_070C
        add    esp, 4
        mov    al, byte ptr [esi + 9]
        mov    byte ptr [esp + 21h], al
        mov    al, byte ptr [esi + 0bh]
        mov    byte ptr [esp + 23h], al
        mov    al, byte ptr [esi + 0ah]
        and    al, 0fch
        mov    byte ptr [esp + 22h], al
        mov    cl, byte ptr [esi + 8]
        or     cl, cl
        je     SHORT loc_1_9BBE
        lea    eax, [esp + 18h]
        push   eax
        movzx  eax, cl
        push   eax
        push   dword ptr [esi]
        call   sub_1_0738
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_9D1D
loc_1_9BBE:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_9BCC
        mov    esi, dword ptr [esp + 1ch]
        jmp    SHORT loc_1_9BD0
        db  090h
loc_1_9BCC:
        mov    esi, dword ptr [esp + 4ch]
loc_1_9BD0:
        push   esi
        call   sub_1_7044
        add    esp, 4
        cmp    al, 0ffh
        jne    SHORT loc_1_9BF8
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_0A78
        add    esp, 4
        mov    eax, 2b0ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  090h
loc_1_9BF8:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   0
        push   esi
        call   sub_1_7500
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_9C2C
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_0A78
        add    esp, 4
        mov    eax, 2afbh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  090h
loc_1_9C2C:
        mov    ebx, 0
        db  083h, 03Dh, 0FCh, 0BBh, 000h, 000h, 000h
        jne    SHORT loc_1_9C46
        push   eax
        push   esi
        call   sub_1_6F30
        add    esp, 8
        mov    ebx, eax
loc_1_9C46:
        or     ebx, ebx
        jne    SHORT loc_1_9C64
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_0A78
        add    esp, 4
        mov    eax, 2afbh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_1_9C64:
        call   sub_1_B92C
        add    eax, dword ptr [esp + 3ch]
        mov    edi, dword ptr [esp + 38h]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [edi + 8], eax
        mov    byte ptr [edi + 0eh], 0
        mov    bp, word ptr [ebx + 3eh]
        inc    bp
        mov    word ptr [ebx + 3eh], bp
        mov    word ptr [edi + 0ch], bp
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [edi], eax
        mov    dword ptr [ebx + 20h], edi
        mov    dword ptr [esp + 24h], 0
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [esp + 30h], eax
        lea    ecx, [esp + 18h]
        push   ecx
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        push   ebp
        mov    ax, word ptr [ebx + 16h]
        push   eax
        push   8
        push   dword ptr [ebx + 4]
        push   esi
        call   sub_1_9350
        add    esp, 20h
        mov    esi, eax
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_0A78
        add    esp, 4
        cmp    esi, 2bf7h
        je     SHORT loc_1_9D00
        or     esi, esi
        je     SHORT loc_1_9D00
        push   ebp
        push   ebx
        call   sub_1_92A8
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_9D00
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  090h
loc_1_9D00:
        mov    eax, dword ptr [ebx + 20h]
        or     eax, eax
        je     SHORT loc_1_9D18
        nop
loc_1_9D08:
        cmp    eax, edi
        je     SHORT loc_1_9D14
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_9D08
        jmp    SHORT loc_1_9D18
loc_1_9D14:
        mov    byte ptr [edi + 0eh], 1
loc_1_9D18:
        mov    eax, 2bf7h
loc_1_9D1D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 049h, 000h
sub_1_9D28:
        sub    esp, 0ch
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 18h]
        lea    esi, [ecx + eax]
        cmp    eax, 14h
        jae    SHORT loc_1_9D48
        mov    eax, 2af9h
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
loc_1_9D48:
        mov    ax, word ptr [ecx + 0ah]
        or     ax, ax
        je     SHORT loc_1_9D7C
        mov    dx, word ptr [ecx + 8]
        cmp    dx, 14h
        jb     SHORT loc_1_9D6E
        and    edx, 0ffffh
        and    eax, 0ffffh
        add    edx, eax
        add    edx, ecx
        cmp    edx, esi
        jbe    SHORT loc_1_9D7C
loc_1_9D6E:
        pop    edi
        mov    eax, 2b2ah
        pop    esi
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_1_9D7C:
        mov    dl, byte ptr [ecx + 12h]
        or     dl, dl
        je     SHORT loc_1_9DA8
        mov    ax, word ptr [ecx + 10h]
        cmp    ax, 14h
        jb     SHORT loc_1_9D9D
        movzx  edi, dl
        and    eax, 0ffffh
        add    edi, eax
        add    edi, ecx
        cmp    edi, esi
        jbe    SHORT loc_1_9DA8
loc_1_9D9D:
        pop    edi
        mov    eax, 2b2ah
        pop    esi
        add    esp, 0ch
        ret
loc_1_9DA8:
        cmp    byte ptr [ecx + 0ch], 0
        je     SHORT loc_1_9DEC
        mov    byte ptr [esp + 10h], dl
        cmp    byte ptr [ecx + 12h], 0
        jbe    SHORT loc_1_9DC4
        movzx  eax, word ptr [ecx + 10h]
        add    eax, ecx
        mov    dword ptr [esp + 8], eax
        jmp    SHORT loc_1_9DCC
loc_1_9DC4:
        mov    dword ptr [esp + 8], 0
loc_1_9DCC:
        mov    dword ptr [esp + 0ch], 0
        mov    al, byte ptr [ecx + 0dh]
        mov    byte ptr [esp + 11h], al
        mov    al, byte ptr [ecx + 0eh]
        mov    byte ptr [esp + 12h], al
        mov    al, byte ptr [ecx + 0fh]
        mov    byte ptr [esp + 13h], al
        jmp    SHORT loc_1_9E10
        db  090h
loc_1_9DEC:
        mov    byte ptr [esp + 10h], 0
        mov    dword ptr [esp + 8], 0
        mov    dword ptr [esp + 0ch], 0
        mov    byte ptr [esp + 11h], 20h
        mov    byte ptr [esp + 12h], 0
        mov    byte ptr [esp + 13h], 0
loc_1_9E10:
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [ecx]
        push   dword ptr [esp + 2ch]
        movzx  eax, word ptr [ecx + 0ah]
        push   eax
        movzx  eax, word ptr [ecx + 8]
        add    eax, ecx
        push   eax
        push   dword ptr [ecx + 4]
        push   dword ptr [esp + 38h]
        call   sub_1_9B58
        add    esp, 1ch
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
sub_1_9E3C:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, 1ch
        mov    ecx, dword ptr [esp + 24h]
        mov    ebx, dword ptr [ecx + 14h]
        mov    ebp, dword ptr [esp + 2ch]
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_1_9E68
        mov    ecx, dword ptr [esp + 34h]
        mov    cl, byte ptr [ecx + 8]
        mov    byte ptr [esp + 13h], cl
        jmp    SHORT loc_1_9E6D
        db  090h
loc_1_9E68:
        mov    byte ptr [esp + 13h], 0
loc_1_9E6D:
        lea    ecx, [ebx + 1ch]
        mov    byte ptr [ebx + 17h], 0
        mov    dword ptr [ebx + 18h], ecx
        mov    word ptr [ebx + 0ch], 0
        mov    dword ptr [ebx + 10h], ecx
        mov    edx, dword ptr [esp + 28h]
        or     edx, edx
        je     SHORT loc_1_9EA0
        cmp    dword ptr [esp + 30h], 0
        jne    SHORT loc_1_9EA0
        mov    word ptr [ebx + 0eh], 0
        mov    dword ptr [ebx + 4], edx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_9EA0:
        or     edx, edx
        je     SHORT loc_1_9EB4
        mov    edi, ebx
        mov    esi, dword ptr [ebp + 8]
        movsd
        mov    dword ptr [esp + 30h], 0
        sub    ebp, ebp
loc_1_9EB4:
        mov    esi, dword ptr [esp + 30h]
        movzx  ecx, byte ptr [esp + 13h]
        mov    dword ptr [esp + 18h], ecx
        add    esi, ecx
        add    esi, 1ch
        mov    ecx, dword ptr [esp + 24h]
        cmp    esi, dword ptr [ecx + 18h]
        jbe    SHORT loc_1_9EE4
        mov    word ptr [ebx + 0eh], 0
        mov    dword ptr [ebx + 4], 2af9h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_9EE4:
        mov    dword ptr [ebx + 4], edx
        mov    word ptr [ebx + 0eh], 1
        call   sub_1_B92C
        mov    ecx, dword ptr [esp + 24h]
        sub    eax, dword ptr [ecx + 10h]
        mov    dword ptr [ebx + 8], eax
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_1_9F3F
        mov    ecx, dword ptr [esp + 34h]
        mov    al, byte ptr [ecx + 9]
        mov    byte ptr [ebx + 14h], al
        mov    al, byte ptr [ecx + 0ah]
        mov    byte ptr [ebx + 15h], al
        mov    al, byte ptr [ecx + 0bh]
        mov    byte ptr [ebx + 16h], al
        mov    cl, byte ptr [esp + 13h]
        mov    byte ptr [ebx + 17h], cl
        or     cl, cl
        je     SHORT loc_1_9F3F
        mov    eax, dword ptr [esp + 18h]
        mov    edi, dword ptr [ebx + 18h]
        mov    ecx, dword ptr [esp + 34h]
        mov    esi, dword ptr [ecx]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_9F3F:
        mov    ax, word ptr [esp + 30h]
        mov    word ptr [esp + 1ch], ax
        movzx  ecx, byte ptr [ebx + 17h]
        mov    word ptr [ebx + 0ch], ax
        mov    eax, dword ptr [ebx + 18h]
        add    eax, ecx
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [ebx + 10h], eax
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_1_9F93
        mov    edx, eax
loc_1_9F66:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esp + 30h]
        sub    eax, ecx
        sbb    esi, esi
        and    eax, esi
        add    eax, ecx
        mov    edi, edx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    edx, eax
        sub    dword ptr [esp + 30h], eax
        mov    ebp, dword ptr [ebp]
        jne    SHORT loc_1_9F66
loc_1_9F93:
        movzx  eax, word ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 18h]
        lea    eax, [eax + ecx + 1ch]
        mov    ecx, dword ptr [ebx + 18h]
        sub    ecx, ebx
        mov    dword ptr [ebx + 18h], ecx
        mov    ecx, dword ptr [esp + 14h]
        sub    ecx, ebx
        mov    dword ptr [ebx + 10h], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_9FBC:
        push   esi
        push   edi
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        mov    edi, dword ptr [esp + 1ch]
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        push   esi
        call   sub_1_9E3C
        add    esp, 14h
        mov    dword ptr [esi + 18h], eax
        push   edi
        add    esi, 1ch
        push   esi
        call   sub_1_B95C
        add    esp, 8
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_9FF0:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        call   sub_1_B92C
        mov    dword ptr [esp + 1ch], eax
        mov    esi, dword ptr [esp + 40h]
        mov    dword ptr [esp + 20h], esi
        mov    edi, dword ptr [esp + 44h]
        mov    eax, dword ptr [edi]
        mov    dword ptr [esp + 24h], eax
        mov    dword ptr [esp + 28h], 0
        mov    dword ptr [esp + 2ch], 0
        mov    dword ptr [esp + 30h], 0
        db  068h, 0BCh, 09Fh, 000h, 000h
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax]
        mov    ebx, dword ptr [esp + 44h]
        push   ebx
        call   sub_1_9D28
        add    esp, 10h
        cmp    eax, 2bf7h
        jne    SHORT loc_1_A074
        lea    eax, [esp + 28h]
        push   eax
        call   sub_1_B950
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_A062
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi], eax
loc_1_A062:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Dh, 049h, 000h
loc_1_A074:
        mov    dword ptr [edi], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Dh, 049h, 000h
loc_1_A084:
        sub    esp, 60h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 13h], 0
        db  083h, 03Dh, 078h, 0BDh, 000h, 000h, 000h
        jne    NEAR PTR loc_1_A1DE
        db  0C7h, 005h, 078h, 0BDh, 000h, 000h, 001h, 000h, 000h, 000h
        db  0A1h, 070h, 0BDh, 000h, 000h
        mov    dword ptr [esp + 24h], eax
        or     eax, eax
        je     NEAR PTR loc_1_A1C8
loc_1_A0B8:
        mov    eax, dword ptr [esp + 24h]
        mov    eax, dword ptr [eax + 24h]
        db  0A3h, 070h, 0BDh, 000h, 000h
        db  0FFh, 00Dh, 0B4h, 0C1h, 000h, 000h
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   0
        push   0
        push   dword ptr [esp + 34h]
        call   sub_1_BA20
        mov    eax, dword ptr [esp + 28h]
        db  001h, 005h, 0A0h, 0C1h, 000h, 000h
        db  001h, 005h, 088h, 0C1h, 000h, 000h
        db  083h, 03Dh, 07Ch, 0C1h, 000h, 000h, 001h
        jne    NEAR PTR loc_1_A19C
        mov    ebx, dword ptr [esp + 28h]
        sub    ebx, 44h
        sbb    eax, eax
        and    ebx, eax
        add    ebx, 44h
        mov    ebp, 0
        lea    eax, [esp + 2ch]
        mov    dword ptr [esp + 18h], eax
        je     SHORT loc_1_A16D
        nop
loc_1_A11C:
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_BA44
        mov    eax, ebx
        sub    eax, ebp
        mov    ecx, dword ptr [esp + 20h]
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        mov    edi, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    ebp, eax
        add    dword ptr [esp + 18h], eax
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 18h]
        call   sub_1_BA68
        cmp    ebp, ebx
        jb     SHORT loc_1_A11C
loc_1_A16D:
        db  0FFh, 005h, 08Ch, 0C1h, 000h, 000h
        mov    byte ptr [esp + 13h], 1
        push   0
        push   0
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   ebx
        lea    eax, [esp + 40h]
        push   eax
        push   dword ptr [esp + 90h]
        call   sub_1_5514
        add    esp, 1ch
        jmp    SHORT loc_1_A1A2
        db  090h
loc_1_A19C:
        db  0FFh, 005h, 094h, 0C1h, 000h, 000h
loc_1_A1A2:
        push   0
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 80h]
        call   sub_1_5BD8
        add    esp, 0ch
        db  0A1h, 070h, 0BDh, 000h, 000h
        mov    dword ptr [esp + 24h], eax
        or     eax, eax
        jne    NEAR PTR loc_1_A0B8
loc_1_A1C8:
        db  0C7h, 005h, 078h, 0BDh, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_A1DE
        call   sub_1_4BCC
loc_1_A1DE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 60h
        ret
        db  08Bh, 0FFh
loc_1_A1E8:
        db  0FFh, 005h, 0A4h, 0C1h, 000h, 000h
        db  083h, 03Dh, 07Ch, 0C1h, 000h, 000h, 001h
        jne    SHORT loc_1_A248
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 24h], 0
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        jne    SHORT loc_1_A214
        db  0A3h, 070h, 0BDh, 000h, 000h
        jmp    SHORT loc_1_A21D
        db  08Bh, 0FFh
loc_1_A214:
        db  08Bh, 00Dh, 074h, 0BDh, 000h, 000h
        mov    dword ptr [ecx + 24h], eax
loc_1_A21D:
        db  0A3h, 074h, 0BDh, 000h, 000h
        db  0FFh, 005h, 0B4h, 0C1h, 000h, 000h
        db  083h, 03Dh, 078h, 0BDh, 000h, 000h, 000h
        jne    SHORT loc_1_A242
        push   dword ptr [esp + 4]
        db  068h, 098h, 0C0h, 000h, 000h
        call   sub_1_B932
        add    esp, 8
loc_1_A242:
        mov    eax, 103h
        ret
loc_1_A248:
        sub    eax, eax
        db  0FFh, 005h, 0ACh, 0C1h, 000h, 000h
        ret
        db  08Dh, 049h, 000h
loc_1_A254:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        lea    eax, [esp + 18h]
        push   eax
        push   0
        push   0
        push   dword ptr [esp + 40h]
        call   sub_1_BA20
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_BA44
        mov    esi, dword ptr [esp + 38h]
        cmp    dword ptr [esp + 10h], esi
        ja     SHORT loc_1_A2B7
loc_1_A28C:
        sub    esi, dword ptr [esp + 10h]
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 18h]
        call   sub_1_BA68
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_BA44
        cmp    dword ptr [esp + 10h], esi
        jbe    SHORT loc_1_A28C
loc_1_A2B7:
        push   0
        add    dword ptr [esp + 20h], esi
        sub    dword ptr [esp + 14h], esi
        lea    eax, [esp + 1ch]
        push   eax
        push   0
        push   0
        push   dword ptr [esp + 50h]
        call   sub_1_BA20
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_BA44
        sub    ebx, ebx
        mov    ebp, dword ptr [esp + 3ch]
        cmp    ebp, ebx
        je     NEAR PTR loc_1_A394
loc_1_A2F4:
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 10h]
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        mov    edi, dword ptr [esp + 24h]
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    ebx, eax
        add    dword ptr [esp + 24h], eax
        add    dword ptr [esp + 1ch], eax
        sub    ebp, eax
        sub    dword ptr [esp + 10h], eax
        sub    dword ptr [esp + 20h], eax
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_A35D
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 18h]
        call   sub_1_BA68
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_A394
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_BA44
loc_1_A35D:
        cmp    dword ptr [esp + 20h], 0
        jne    SHORT loc_1_A38C
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_BA68
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_A394
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_BA44
loc_1_A38C:
        or     ebp, ebp
        jne    NEAR PTR loc_1_A2F4
loc_1_A394:
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [eax], ebx
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_1_A3A4:
        ret
        db  08Dh, 049h, 000h
loc_1_A3A8:
        ret
        db  08Dh, 049h, 000h
loc_1_A3AC:
        sub    esp, 4
        push   esi
        push   edi
        mov    dword ptr [esp + 8], 0
        mov    esi, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esi]
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [ecx + 4]
        cmp    dword ptr [ecx], 200h
        jne    NEAR PTR loc_1_A4D4
        db  03Bh, 005h, 0A8h, 0BFh, 000h, 000h
        jne    NEAR PTR loc_1_A4D4
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 0ch], 100h
        je     SHORT loc_1_A3F8
        mov    eax, 1ah
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_1_A3F8:
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 100h
        jne    SHORT loc_1_A448
        cmp    dword ptr [ecx + 10h], 1
        jne    SHORT loc_1_A43C
        cmp    edi, 4
        jb     SHORT loc_1_A430
        lea    eax, [esp + 8]
        push   eax
        push   4
        db  068h, 098h, 0BDh, 000h, 000h
        push   dword ptr [esp + 24h]
        call   sub_1_AAE0
        add    esp, 10h
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_A430:
        pop    edi
        mov    eax, 21h
        pop    esi
        add    esp, 4
        ret
        db  090h
loc_1_A43C:
        pop    edi
        mov    eax, 1ah
        pop    esi
        add    esp, 4
        ret
        db  090h
loc_1_A448:
        cmp    eax, 200h
        je     SHORT loc_1_A45C
        mov    eax, 1ah
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_A45C:
        cmp    dword ptr [ecx + 10h], 1
        jne    SHORT loc_1_A4C8
        cmp    edi, 5ch
        jae    SHORT loc_1_A474
        mov    eax, 21h
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_A474:
        lea    eax, [esp + 8]
        push   eax
        push   5ch
        db  068h, 060h, 0C1h, 000h, 000h
        push   dword ptr [esp + 24h]
        call   sub_1_AAE0
        add    esp, 10h
        cmp    edi, 76h
        jb     SHORT loc_1_A4B4
        lea    ecx, [esp + 8]
        push   ecx
        push   1ah
        db  068h, 07Ch, 0BDh, 000h, 000h
        push   eax
        call   sub_1_AAE0
        add    esp, 10h
        mov    dword ptr [esi], 76h
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_1_A4B4:
        pop    edi
        mov    dword ptr [esi], 5ch
        mov    eax, 9
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_A4C8:
        pop    edi
        mov    eax, 1ah
        pop    esi
        add    esp, 4
        ret
        db  090h
loc_1_A4D4:
        pop    edi
        mov    eax, 0feh
        pop    esi
        add    esp, 4
        ret
        db  090h
loc_1_A4E0:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 4]
        cmp    dword ptr [eax], 200h
        jne    SHORT loc_1_A554
        db  03Bh, 00Dh, 0A8h, 0BFh, 000h, 000h
        jne    SHORT loc_1_A554
        cmp    dword ptr [eax + 8], 200h
        jne    SHORT loc_1_A54C
        cmp    dword ptr [eax + 0ch], 100h
        jne    SHORT loc_1_A54C
        cmp    dword ptr [eax + 10h], 1
        jne    SHORT loc_1_A544
        cmp    dword ptr [esp + 10h], 5ch
        jb     SHORT loc_1_A53C
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        cmp    eax, 1
        je     SHORT loc_1_A534
        cmp    eax, 2
        je     SHORT loc_1_A534
        cmp    eax, 3
        je     SHORT loc_1_A539
        mov    eax, 1ah
        ret
        db  08Bh, 0FFh
loc_1_A534:
        db  0A3h, 07Ch, 0C1h, 000h, 000h
loc_1_A539:
        sub    eax, eax
        ret
loc_1_A53C:
        mov    eax, 1ah
        ret
        db  08Bh, 0FFh
loc_1_A544:
        mov    eax, 1ah
        ret
        db  08Bh, 0FFh
loc_1_A54C:
        mov    eax, 1ah
        ret
        db  08Bh, 0FFh
loc_1_A554:
        mov    eax, 0feh
        ret
        db  08Bh, 0FFh
loc_1_A55C:
        mov    eax, 1
        ret
        db  08Bh, 0FFh
loc_1_A564:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 1ch]
        mov    edx, dword ptr [ecx]
        sub    edi, edi
        cmp    edx, edi
        je     SHORT loc_1_A598
        mov    esi, edx
        mov    eax, dword ptr [esp + 18h]
loc_1_A57A:
        cmp    dword ptr [eax], 200h
        jne    SHORT loc_1_A590
        mov    ebx, dword ptr [eax + 4]
        inc    ebx
        sub    ebx, edi
        sbb    ebp, ebp
        not    ebp
        and    ebx, ebp
        add    edi, ebx
loc_1_A590:
        add    eax, 8
        dec    esi
        jne    SHORT loc_1_A57A
        jmp    SHORT loc_1_A59C
loc_1_A598:
        mov    eax, dword ptr [esp + 18h]
loc_1_A59C:
        inc    edx
        cmp    edx, 20h
        jbe    SHORT loc_1_A5AC
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_A5AC:
        pop    ebp
        db  089h, 03Dh, 0A8h, 0BFh, 000h, 000h
        mov    dword ptr [eax], 200h
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [ecx], edx
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_A5C8:
        db  0A1h, 0F4h, 0BFh, 000h, 000h
        imul   eax, eax, 48c27395h
        or     eax, eax
        jge    SHORT loc_1_A5DD
        add    eax, 7fffffffh
        inc    eax
loc_1_A5DD:
        or     eax, eax
        db  0A3h, 0F4h, 0BFh, 000h, 000h
        jne    SHORT loc_1_A5FE
        call   sub_1_B92C
        db  003h, 005h, 0F0h, 0BFh, 000h, 000h
        mov    ecx, 5f5e100h
        cdq
        idiv   ecx
        or     edx, 1
        mov    eax, edx
loc_1_A5FE:
        sub    edx, edx
        db  0A3h, 0F4h, 0BFh, 000h, 000h
        mov    ecx, 14h
        div    ecx
        lea    eax, [edx + 1]
        ret
sub_1_A610:
        mov    eax, dword ptr [esp + 4]
        lea    edx, [eax + 40h]
        mov    eax, dword ptr [eax + 40h]
        or     eax, eax
        je     SHORT loc_1_A636
        mov    ecx, dword ptr [esp + 8]
loc_1_A622:
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_1_A630
        mov    edx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_A622
        ret
loc_1_A630:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx], edx
loc_1_A636:
        ret
        db  090h
loc_1_A638:
        sub    esp, 4
        push   esi
        push   edi
        db  083h, 03Dh, 0F8h, 0C2h, 000h, 000h, 002h
        je     SHORT loc_1_A650
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_A650:
        mov    edi, dword ptr [esp + 18h]
        push   edi
        call   sub_1_7044
        add    esp, 4
        cmp    al, 0ffh
        je     NEAR PTR loc_1_A734
        test   al, 1
        jne    NEAR PTR loc_1_A734
        cmp    dword ptr [esp + 24h], 8
        jne    NEAR PTR loc_1_A72C
        mov    eax, dword ptr [esp + 20h]
        cmp    dword ptr [eax + 0ch], 8
        jne    NEAR PTR loc_1_A72C
        mov    esi, dword ptr [eax + 8]
        push   8
        push   esi
        call   sub_1_5C30
        add    esp, 8
        cmp    ax, 0ffffh
        je     SHORT loc_1_A6A4
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_A6A4:
        mov    al, byte ptr [esi]
        cmp    al, 11h
        je     SHORT loc_1_A6B8
        cmp    al, 12h
        je     SHORT loc_1_A6B8
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_A6B8:
        cmp    dword ptr [esp + 1ch], edi
        jne    SHORT loc_1_A6C8
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_A6C8:
        mov    ecx, dword ptr [esp + 10h]
        test   byte ptr [ecx + 14h], 1
        je     SHORT loc_1_A736
        cmp    al, 11h
        jne    SHORT loc_1_A700
        mov    esi, dword ptr [ecx + 40h]
        or     esi, esi
        je     SHORT loc_1_A736
loc_1_A6DD:
        cmp    dword ptr [esi + 4], 10000e0h
        je     SHORT loc_1_A6F4
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_1_A6F4
        call   sub_1_A5C8
        mov    dword ptr [esi + 0ch], eax
loc_1_A6F4:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_A6DD
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_1_A700:
        mov    eax, dword ptr [esi + 4]
        cmp    eax, dword ptr [esp + 14h]
        jne    SHORT loc_1_A736
        lea    edx, [esp + 8]
        push   edx
        push   eax
        push   ecx
        call   sub_1_A610
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_A736
        mov    dword ptr [eax + 0ch], 0
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_A72C:
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_1_A734:
        sub    eax, eax
loc_1_A736:
        pop    edi
        pop    esi
        add    esp, 4
        ret
sub_1_A73C:
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        cmp    esi, 10000e0h
        je     NEAR PTR loc_1_A7D1
        lea    eax, [esp + 8]
        push   eax
        push   8
        call   sub_1_917C
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_A7D1
        mov    byte ptr [edi], 12h
        mov    byte ptr [edi + 1], 0
        mov    word ptr [edi + 2], 0
        mov    dword ptr [edi + 4], esi
        push   8
        push   edi
        call   sub_1_5C30
        add    esp, 8
        not    ax
        mov    word ptr [edi + 2], ax
        lea    eax, [esp + 0ch]
        push   eax
        call   sub_1_070C
        add    esp, 4
        mov    byte ptr [esp + 15h], 1
        push   0
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        push   esi
        push   8
        push   dword ptr [esp + 1ch]
        push   0
        db  0FFh, 035h, 04Ch, 0C0h, 000h, 000h
        call   sub_1_6848
        add    esp, 20h
        cmp    eax, 2bf7h
        je     SHORT loc_1_A7D1
        push   dword ptr [esp + 8]
        push   0
        call   sub_1_92D4
        add    esp, 8
loc_1_A7D1:
        pop    edi
        pop    esi
        add    esp, 10h
        ret
        db  090h
sub_1_A7D8:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  083h, 03Dh, 0F8h, 0C2h, 000h, 000h, 002h
        je     SHORT loc_1_A7F8
        mov    eax, 2b03h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_A7F8:
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 20h], 0
        db  080h, 03Dh, 0F0h, 0C2h, 000h, 000h, 000h
        je     SHORT loc_1_A84C
        db  0B8h, 0B0h, 0C0h, 000h, 000h
        db  083h, 03Dh, 0B0h, 0C0h, 000h, 000h, 000h
        je     SHORT loc_1_A82F
        nop
loc_1_A828:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_A828
loc_1_A82F:
        lea    ecx, [esp + 14h]
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [eax], ecx
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_B950
        add    esp, 4
        jmp    SHORT loc_1_A853
loc_1_A84C:
        db  0C6h, 005h, 0F0h, 0C2h, 000h, 000h, 001h
loc_1_A853:
        sub    esi, esi
        mov    edi, dword ptr [esp + 28h]
        test   byte ptr [edi + 14h], 1
        jne    SHORT loc_1_A870
        cmp    dword ptr [esp + 30h], 2
        je     SHORT loc_1_A870
        mov    esi, 2b03h
        jmp    NEAR PTR loc_1_A9D1
loc_1_A870:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 30h]
        push   edi
        call   sub_1_A610
        add    esp, 0ch
        mov    ebx, dword ptr [edi + 0ch]
        mov    ecx, dword ptr [esp + 30h]
        or     ecx, ecx
        je     SHORT loc_1_A8A4
        cmp    ecx, 1
        je     NEAR PTR loc_1_A958
        cmp    ecx, 2
        je     NEAR PTR loc_1_A99C
        jmp    NEAR PTR loc_1_A9D1
loc_1_A8A4:
        or     eax, eax
        jne    NEAR PTR loc_1_A950
        mov    al, byte ptr [esp + 2ch]
        and    al, 0f0h
        cmp    al, 0e0h
        je     SHORT loc_1_A8C0
        mov    esi, 2b03h
        jmp    NEAR PTR loc_1_A9D1
loc_1_A8C0:
        push   10h
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_A944
        push   0
        push   dword ptr [esp + 30h]
        push   1
        push   dword ptr [ebx + 4]
        call   dword ptr [ebx + 18h]
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_A930
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [ebp + 8], 1
        mov    dword ptr [ebp + 0ch], 0
        mov    eax, dword ptr [edi + 40h]
        mov    dword ptr [ebp], eax
        mov    dword ptr [edi + 40h], ebp
        cmp    dword ptr [esp + 2ch], 10000e0h
        je     NEAR PTR loc_1_A9D1
        call   sub_1_A5C8
        mov    dword ptr [ebp + 0ch], eax
        push   dword ptr [edi + 4]
        push   dword ptr [esp + 30h]
        call   sub_1_A73C
        add    esp, 8
        jmp    NEAR PTR loc_1_A9D1
        db  08Dh, 049h, 000h
loc_1_A930:
        push   ebp
        call   sub_1_B944
        add    esp, 4
        mov    esi, 2afeh
        jmp    NEAR PTR loc_1_A9D1
        db  090h
loc_1_A944:
        mov    esi, 2afeh
        jmp    NEAR PTR loc_1_A9D1
        db  08Bh, 0FFh
loc_1_A950:
        inc    dword ptr [eax + 8]
        jmp    NEAR PTR loc_1_A9D1
loc_1_A958:
        or     eax, eax
        je     SHORT loc_1_A994
        cmp    dword ptr [esp + 2ch], 10000e0h
        je     SHORT loc_1_A9D1
        mov    ecx, dword ptr [eax + 8]
        dec    ecx
        mov    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_A9D1
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [edx], ecx
        push   eax
        call   sub_1_B944
        add    esp, 4
        push   0
        push   dword ptr [esp + 30h]
        push   1
        push   dword ptr [ebx + 4]
        call   dword ptr [ebx + 1ch]
        add    esp, 10h
        jmp    SHORT loc_1_A9D1
        db  090h
loc_1_A994:
        mov    esi, 2b03h
        jmp    SHORT loc_1_A9D1
        db  090h
loc_1_A99C:
        mov    ebp, dword ptr [edi + 40h]
        mov    dword ptr [edi + 40h], 0
        or     ebp, ebp
        je     SHORT loc_1_A9D1
loc_1_A9AA:
        push   0
        push   dword ptr [ebp + 4]
        push   1
        push   dword ptr [ebx + 4]
        call   dword ptr [ebx + 1ch]
        add    esp, 10h
        mov    dword ptr [esp + 10h], ebp
        mov    ebp, dword ptr [ebp]
        push   dword ptr [esp + 10h]
        call   sub_1_B944
        add    esp, 4
        or     ebp, ebp
        jne    SHORT loc_1_A9AA
loc_1_A9D1:
        db  08Bh, 00Dh, 0B0h, 0C0h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_A9FC
        mov    eax, ecx
        mov    eax, dword ptr [eax]
        db  0A3h, 0B0h, 0C0h, 000h, 000h
        push   0
        add    ecx, 4
        push   ecx
        call   sub_1_B95C
        add    esp, 8
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_A9FC:
        mov    eax, esi
        db  0C6h, 005h, 0F0h, 0C2h, 000h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
sub_1_AA10:
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 0F8h, 0C2h, 000h, 000h, 002h
        jne    SHORT loc_1_AA70
        mov    esi, dword ptr [esp + 10h]
        lea    ebx, [esi + 40h]
        mov    edi, dword ptr [ebx]
        or     edi, edi
        je     SHORT loc_1_AA70
loc_1_AA29:
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_1_AA68
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_AA68
        test   byte ptr [esi + 14h], 1
        je     SHORT loc_1_AA68
        inc    dword ptr [edi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [edi + 4]
        call   sub_1_A73C
        add    esp, 8
        mov    eax, dword ptr [edi + 8]
        dec    eax
        mov    dword ptr [edi + 8], eax
        jne    SHORT loc_1_AA68
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx], eax
        push   edi
        call   sub_1_B944
        add    esp, 4
        mov    edi, dword ptr [ebx]
        jmp    SHORT loc_1_AA6C
        db  090h
loc_1_AA68:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
loc_1_AA6C:
        or     edi, edi
        jne    SHORT loc_1_AA29
loc_1_AA70:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_AA74:
        db  083h, 03Dh, 0F8h, 0C2h, 000h, 000h, 002h
        jne    SHORT loc_1_AA90
        push   0
        push   10000e0h
        push   dword ptr [esp + 0ch]
        call   sub_1_A7D8
        add    esp, 0ch
loc_1_AA90:
        db  083h, 03Dh, 0F0h, 0BFh, 000h, 000h, 000h
        jne    SHORT loc_1_AAC1
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        db  0A3h, 0F0h, 0BFh, 000h, 000h
        call   sub_1_B92C
        db  003h, 005h, 0F0h, 0BFh, 000h, 000h
        mov    ecx, 5f5e100h
        cdq
        idiv   ecx
        or     edx, 1
        db  089h, 015h, 0F4h, 0BFh, 000h, 000h
loc_1_AAC1:
        ret
        db  08Bh, 0FFh
sub_1_AAC4:
        db  083h, 03Dh, 0F8h, 0C2h, 000h, 000h, 002h
        jne    SHORT loc_1_AADD
        push   2
        push   0
        push   dword ptr [esp + 0ch]
        call   sub_1_A7D8
        add    esp, 0ch
loc_1_AADD:
        ret
        db  08Bh, 0FFh
sub_1_AAE0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 28h]
        mov    esi, dword ptr [eax]
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 24h]
        call   sub_1_BA44
        mov    edx, dword ptr [esp + 10h]
        add    edx, esi
        mov    eax, dword ptr [esp + 14h]
        sub    eax, esi
        mov    ebx, dword ptr [esp + 24h]
loc_1_AB10:
        mov    ecx, eax
        sub    ecx, ebx
        sbb    esi, esi
        and    ecx, esi
        lea    ebp, [ecx + ebx]
        mov    edi, edx
        mov    esi, dword ptr [esp + 20h]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    edx, ebp
        add    dword ptr [esp + 20h], ebp
        sub    ebx, ebp
        je     SHORT loc_1_AB64
        sub    eax, ebp
        jne    SHORT loc_1_AB10
        mov    eax, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 24h]
        call   sub_1_BA44
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 14h]
        jmp    SHORT loc_1_AB10
loc_1_AB64:
        sub    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 1ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_AB7C:
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 54h]
        mov    ebp, dword ptr [eax]
        sub    ebx, ebx
        mov    dword ptr [esp + 10h], ebx
        mov    esi, dword ptr [esp + 4ch]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        cmp    ecx, 301h
        je     SHORT loc_1_ABF0
        cmp    ecx, 380h
        je     SHORT loc_1_ABE8
        db  08Bh, 01Dh, 0B4h, 0C0h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_ABDA
        mov    edi, dword ptr [esp + 50h]
loc_1_ABB6:
        push   dword ptr [esp + 58h]
        push   dword ptr [esp + 58h]
        push   edi
        push   esi
        push   dword ptr [ebx + 4]
        call   dword ptr [ebx + 20h]
        add    esp, 14h
        cmp    eax, 0feh
        jne    NEAR PTR loc_1_AFC1
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_1_ABB6
loc_1_ABDA:
        pop    ebp
        mov    eax, 0feh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  090h
loc_1_ABE8:
        cmp    ecx, 301h
        jne    SHORT loc_1_ABFC
loc_1_ABF0:
        db  03Bh, 005h, 038h, 0C2h, 000h, 000h
        jne    NEAR PTR loc_1_AFBC
loc_1_ABFC:
        db  03Bh, 005h, 04Ch, 0C1h, 000h, 000h
        jne    NEAR PTR loc_1_AFBC
        mov    eax, dword ptr [esp + 54h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 100h
        jne    SHORT loc_1_AC8C
        cmp    dword ptr [esi + 0ch], 100h
        jne    SHORT loc_1_AC7C
        cmp    dword ptr [esi + 10h], 1
        jne    SHORT loc_1_AC7C
        cmp    ebp, 4
        jb     SHORT loc_1_AC6C
        sub    ecx, 301h
        cmp    ecx, 1
        sbb    eax, eax
        and    eax, 0ffffff83h
        add    eax, 380h
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 10h]
        push   eax
        push   4
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 5ch]
        call   sub_1_AAE0
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Dh, 049h, 000h
loc_1_AC6C:
        pop    ebp
        mov    eax, 21h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Dh, 049h, 000h
loc_1_AC7C:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Dh, 049h, 000h
loc_1_AC8C:
        cmp    eax, 200h
        jne    NEAR PTR loc_1_AFAC
        cmp    dword ptr [esi + 0ch], 100h
        jne    NEAR PTR loc_1_AFAC
        cmp    ecx, 380h
        jne    SHORT loc_1_AD18
        cmp    dword ptr [esi + 10h], 1
        je     SHORT loc_1_ACC0
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  090h
loc_1_ACC0:
        cmp    ebp, 68h
        jae    SHORT loc_1_ACD4
        mov    eax, 21h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Bh, 0FFh
loc_1_ACD4:
        lea    eax, [esp + 10h]
        push   eax
        push   34h
        db  068h, 0B0h, 0BFh, 000h, 000h
        push   dword ptr [esp + 5ch]
        call   sub_1_AAE0
        add    esp, 10h
        lea    ecx, [esp + 10h]
        push   ecx
        push   34h
        db  068h, 070h, 0BFh, 000h, 000h
        push   eax
        call   sub_1_AAE0
        add    esp, 10h
        mov    eax, dword ptr [esp + 54h]
        mov    dword ptr [eax], 68h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Dh, 049h, 000h
loc_1_AD18:
        mov    eax, dword ptr [esi + 10h]
        cmp    eax, 1
        je     SHORT loc_1_AD4C
        cmp    eax, 101h
        je     SHORT loc_1_AD98
        cmp    eax, 102h
        je     NEAR PTR loc_1_AE10
        cmp    eax, 103h
        je     NEAR PTR loc_1_AEF0
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Bh, 0FFh
loc_1_AD4C:
        cmp    ebp, 5ch
        jae    SHORT loc_1_AD60
        mov    eax, 21h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Bh, 0FFh
loc_1_AD60:
        db  0A1h, 054h, 0C0h, 000h, 000h
        db  0A3h, 028h, 0C2h, 000h, 000h
        db  0A1h, 054h, 0C1h, 000h, 000h
        db  0A3h, 02Ch, 0C2h, 000h, 000h
        lea    eax, [esp + 10h]
        push   eax
        push   5ch
        db  068h, 0D8h, 0C1h, 000h, 000h
        push   dword ptr [esp + 5ch]
        call   sub_1_AAE0
        add    esp, 10h
        mov    ebx, 5ch
        sub    ebp, ebp
        jmp    NEAR PTR loc_1_AF9B
loc_1_AD98:
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 5ch]
        call   sub_1_7658
        add    esp, 8
        mov    esi, eax
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_ADC0
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  090h
loc_1_ADC0:
        or     esi, esi
        je     SHORT loc_1_AE02
        mov    edi, dword ptr [esp + 50h]
loc_1_ADC8:
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, 30h
        jl     SHORT loc_1_AE02
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 5ch]
        call   sub_1_7544
        add    esp, 8
        mov    esi, eax
        add    ebx, 30h
        lea    eax, [esp + 10h]
        push   eax
        push   30h
        lea    eax, [esp + 20h]
        push   eax
        push   edi
        call   sub_1_AAE0
        add    esp, 10h
        mov    edi, eax
        or     esi, esi
        jne    SHORT loc_1_ADC8
loc_1_AE02:
        cmp    esi, 1
        cmc
        sbb    ebp, ebp
        and    ebp, 9
        jmp    NEAR PTR loc_1_AF9B
loc_1_AE10:
        db  08Bh, 035h, 090h, 0C0h, 000h, 000h
        mov    eax, dword ptr [esp + 58h]
        mov    ax, word ptr [eax]
        or     ax, ax
        je     SHORT loc_1_AE44
        or     esi, esi
        je     SHORT loc_1_AE36
loc_1_AE26:
        cmp    word ptr [esi + 16h], ax
        je     SHORT loc_1_AE32
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_AE26
loc_1_AE32:
        or     esi, esi
        jne    SHORT loc_1_AE4C
loc_1_AE36:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  090h
loc_1_AE44:
        or     esi, esi
        je     NEAR PTR loc_1_AED1
loc_1_AE4C:
        mov    edi, dword ptr [esp + 50h]
loc_1_AE50:
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, 18h
        jl     SHORT loc_1_AECD
        mov    ax, word ptr [esi + 14h]
        test   al, 8
        je     SHORT loc_1_AEC7
        test   al, 1
        je     SHORT loc_1_AE78
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 20h], eax
        jmp    SHORT loc_1_AE88
        db  08Dh, 049h, 000h
loc_1_AE78:
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 20h], 0
loc_1_AE88:
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [eax + 30h]
        mov    dword ptr [esp + 1ch], ecx
        mov    eax, dword ptr [eax + 3ch]
        and    eax, 1
        mov    dword ptr [esp + 24h], eax
        mov    dword ptr [esp + 28h], 0ffffh
        mov    ax, word ptr [esi + 16h]
        mov    word ptr [esp + 2ch], ax
        lea    eax, [esp + 10h]
        push   eax
        push   18h
        lea    eax, [esp + 20h]
        push   eax
        push   edi
        call   sub_1_AAE0
        add    esp, 10h
        mov    edi, eax
        add    ebx, 18h
loc_1_AEC7:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_AE50
loc_1_AECD:
        or     esi, esi
        jne    SHORT loc_1_AED8
loc_1_AED1:
        sub    ebp, ebp
        jmp    NEAR PTR loc_1_AF9B
loc_1_AED8:
        mov    ebp, 9
        mov    ax, word ptr [esi + 16h]
        mov    ecx, dword ptr [esp + 58h]
        mov    word ptr [ecx], ax
        jmp    NEAR PTR loc_1_AF9B
        db  08Dh, 049h, 000h
loc_1_AEF0:
        db  08Bh, 035h, 090h, 0C0h, 000h, 000h
        mov    eax, dword ptr [esp + 58h]
        mov    eax, dword ptr [eax]
        or     esi, esi
        je     SHORT loc_1_AF15
loc_1_AF00:
        test   byte ptr [esi + 14h], 1
        je     SHORT loc_1_AF0B
        cmp    dword ptr [esi + 4], eax
        je     SHORT loc_1_AF11
loc_1_AF0B:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_AF00
loc_1_AF11:
        or     esi, esi
        jne    SHORT loc_1_AF20
loc_1_AF15:
        mov    ebp, 1ah
        jmp    NEAR PTR loc_1_AF9B
        db  090h
loc_1_AF20:
        cmp    ebp, 10h
        jae    SHORT loc_1_AF2C
        mov    ebp, 21h
        jmp    SHORT loc_1_AF9B
loc_1_AF2C:
        mov    esi, dword ptr [esi + 0ch]
        mov    eax, dword ptr [esi + 48h]
        and    eax, 1
        or     eax, eax
        mov    eax, 0
        setne  al
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esi + 40h]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esi + 44h]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esi + 4ch]
        mov    dword ptr [esp + 24h], eax
        mov    ebx, 10h
        add    eax, ebx
        cmp    eax, ebp
        mov    ebp, 21h
        ja     SHORT loc_1_AF9B
        sub    ebp, ebp
        lea    eax, [esp + 10h]
        push   eax
        push   ebx
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 5ch]
        call   sub_1_AAE0
        add    esp, 10h
        lea    ecx, [esp + 10h]
        push   ecx
        push   dword ptr [esi + 4ch]
        push   dword ptr [esi + 50h]
        push   eax
        call   sub_1_AAE0
        add    esp, 10h
        mov    ebx, dword ptr [esi + 4ch]
        add    ebx, 10h
loc_1_AF9B:
        mov    eax, dword ptr [esp + 54h]
        mov    dword ptr [eax], ebx
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  090h
loc_1_AFAC:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Dh, 049h, 000h
loc_1_AFBC:
        mov    eax, 0feh
loc_1_AFC1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Dh, 049h, 000h
loc_1_AFCC:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ebx + 4]
        cmp    dword ptr [ebx], 301h
        je     SHORT loc_1_B020
        db  08Bh, 02Dh, 0B4h, 0C0h, 000h, 000h
        or     ebp, ebp
        je     SHORT loc_1_B012
        mov    esi, dword ptr [esp + 20h]
        mov    edi, dword ptr [esp + 24h]
loc_1_AFF4:
        push   edi
        push   esi
        push   ebx
        push   dword ptr [ebp + 4]
        call   dword ptr [ebp + 24h]
        add    esp, 10h
        cmp    eax, 0feh
        jne    NEAR PTR loc_1_B2A1
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    SHORT loc_1_AFF4
loc_1_B012:
        pop    ebp
        mov    eax, 0feh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_B020:
        db  03Bh, 005h, 038h, 0C2h, 000h, 000h
        je     SHORT loc_1_B038
        mov    eax, 0feh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_B038:
        mov    eax, dword ptr [ebx + 10h]
        cmp    eax, 101h
        jne    NEAR PTR loc_1_B250
        cmp    dword ptr [esp + 24h], 30h
        jae    SHORT loc_1_B05C
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_B05C:
        sub    edi, edi
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 20h]
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [esp + 10h], eax
        cmp    eax, ebx
        je     NEAR PTR loc_1_B240
        cmp    al, 7fh
        je     NEAR PTR loc_1_B240
        and    al, 0f0h
        cmp    al, 0e0h
        je     NEAR PTR loc_1_B240
        cmp    al, 0f0h
        jne    SHORT loc_1_B09A
        cmp    dword ptr [esp + 10h], -1
        jne    NEAR PTR loc_1_B240
loc_1_B09A:
        cmp    byte ptr [esp + 14h], 7fh
        je     NEAR PTR loc_1_B230
        mov    al, byte ptr [esp + 14h]
        and    al, 0f0h
        cmp    al, 0e0h
        je     NEAR PTR loc_1_B230
        cmp    al, 0f0h
        jne    SHORT loc_1_B0C2
        cmp    dword ptr [esp + 14h], -1
        jne    NEAR PTR loc_1_B230
loc_1_B0C2:
        db  0A1h, 040h, 0C1h, 000h, 000h
        cmp    dword ptr [esi + 4], eax
        jne    SHORT loc_1_B0DC
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_B0DC:
        db  08Bh, 02Dh, 090h, 0C0h, 000h, 000h
        or     ebp, ebp
        je     SHORT loc_1_B131
loc_1_B0E6:
        or     edi, edi
        jne    SHORT loc_1_B0F7
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 4]
        cmp    dword ptr [eax + 30h], ecx
        jne    SHORT loc_1_B0F7
        mov    edi, ebp
loc_1_B0F7:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [ebp + 4], eax
        jne    SHORT loc_1_B108
        test   byte ptr [ebp + 14h], 1
        je     SHORT loc_1_B108
        mov    ebx, ebp
loc_1_B108:
        push   ebp
        push   dword ptr [esp + 14h]
        call   sub_1_6FA4
        add    esp, 8
        or     al, al
        jne    SHORT loc_1_B144
        push   ebp
        push   dword ptr [esp + 18h]
        call   sub_1_6FA4
        add    esp, 8
        or     al, al
        jne    SHORT loc_1_B154
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    SHORT loc_1_B0E6
loc_1_B131:
        or     edi, edi
        jne    SHORT loc_1_B164
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_B144:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_B154:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_B164:
        or     ebx, ebx
        je     SHORT loc_1_B1AC
        mov    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [esi + 4]
        cmp    dword ptr [eax + 30h], ecx
        je     SHORT loc_1_B180
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_B180:
        mov    eax, dword ptr [esi + 1ch]
        cmp    eax, 3
        je     SHORT loc_1_B19C
        cmp    eax, 2
        je     SHORT loc_1_B19C
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_B19C:
        mov    edi, ebx
        or     ebx, ebx
        je     SHORT loc_1_B1AC
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_B1BF
        db  08Dh, 049h, 000h
loc_1_B1AC:
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esp + 10h], eax
        mov    eax, 0ffffffffh
        je     SHORT loc_1_B1BF
        mov    eax, dword ptr [esp + 10h]
loc_1_B1BF:
        cmp    dword ptr [esi + 1ch], 2
        je     SHORT loc_1_B1E8
        push   1
        push   dword ptr [esi + 20h]
        push   dword ptr [esi + 8]
        movzx  ecx, word ptr [edi + 3ch]
        push   ecx
        push   dword ptr [edi + 0ch]
        push   eax
        push   dword ptr [esi + 28h]
        push   dword ptr [esp + 30h]
        call   sub_1_7F60
        add    esp, 20h
        jmp    SHORT loc_1_B1FB
        db  090h
loc_1_B1E8:
        push   dword ptr [edi + 0ch]
        push   eax
        push   dword ptr [esi + 28h]
        push   dword ptr [esp + 20h]
        call   sub_1_7F8C
        add    esp, 10h
loc_1_B1FB:
        or     eax, eax
        jne    SHORT loc_1_B20C
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_B20C:
        cmp    eax, 2afeh
        jne    SHORT loc_1_B220
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_B220:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_B230:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_B240:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_B250:
        cmp    eax, 1
        jne    SHORT loc_1_B29C
        mov    esi, dword ptr [esp + 20h]
        mov    eax, dword ptr [esi + 4]
        cmp    eax, 0ffh
        ja     SHORT loc_1_B28C
        db  080h, 03Dh, 0D4h, 0C1h, 000h, 000h, 000h
        jne    SHORT loc_1_B271
        cmp    dword ptr [esi], 1
        je     SHORT loc_1_B28C
loc_1_B271:
        pop    ebp
        db  0A3h, 058h, 0C1h, 000h, 000h
        cmp    dword ptr [esi], 1
        sete   al
        db  0A2h, 0A4h, 0BFh, 000h, 000h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_B28C:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_B29C:
        mov    eax, 1ah
loc_1_B2A1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_B2AC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esi]
        mov    ecx, dword ptr [esp + 18h]
        sub    edi, edi
        sub    ebx, ebx
        cmp    edx, ebx
        je     SHORT loc_1_B302
        mov    dword ptr [esp + 10h], edx
loc_1_B2C9:
        mov    eax, dword ptr [ecx]
        cmp    eax, 301h
        jne    SHORT loc_1_B2E4
        mov    eax, dword ptr [ecx + 4]
        inc    eax
        sub    eax, edi
        sbb    ebp, ebp
        not    ebp
        and    eax, ebp
        add    edi, eax
        jmp    SHORT loc_1_B2F9
        db  08Bh, 0FFh
loc_1_B2E4:
        cmp    eax, 380h
        jne    SHORT loc_1_B2F9
        mov    eax, dword ptr [ecx + 4]
        inc    eax
        sub    eax, ebx
        sbb    ebp, ebp
        not    ebp
        and    eax, ebp
        add    ebx, eax
loc_1_B2F9:
        add    ecx, 8
        dec    dword ptr [esp + 10h]
        jne    SHORT loc_1_B2C9
loc_1_B302:
        db  089h, 03Dh, 038h, 0C2h, 000h, 000h
        db  089h, 01Dh, 04Ch, 0C1h, 000h, 000h
        add    edx, 2
        cmp    edx, 20h
        jbe    SHORT loc_1_B324
        mov    eax, 20h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_B324:
        mov    dword ptr [ecx], 301h
        mov    dword ptr [ecx + 4], edi
        add    ecx, 8
        mov    dword ptr [ecx], 380h
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [esi], edx
        db  08Bh, 03Dh, 0B4h, 0C0h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_B35D
loc_1_B345:
        push   esi
        push   dword ptr [esp + 1ch]
        push   dword ptr [edi + 4]
        call   dword ptr [edi + 28h]
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_B368
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_B345
loc_1_B35D:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_B368:
        pop    ebp
        mov    eax, 21h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_B378:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B94A
        push   30h
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_B3A0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_B3A0:
        db  0C7h, 044h, 024h, 020h, 09Ch, 0BDh, 000h, 000h
        mov    word ptr [esp + 1ch], 6
        mov    word ptr [esp + 1eh], 6
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_B9BA
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_B3E4
        push   esi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_B3E4:
        db  0C7h, 044h, 024h, 020h, 044h, 0BFh, 000h, 000h
        mov    word ptr [esp + 1ch], 0dh
        mov    word ptr [esp + 1eh], 0dh
        push   0
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_B428
        db  0C7h, 005h, 044h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_B434
        db  090h
loc_1_B428:
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 4]
        db  0A3h, 044h, 0C1h, 000h, 000h
loc_1_B434:
        db  0C7h, 044h, 024h, 020h, 054h, 0BFh, 000h, 000h
        mov    word ptr [esp + 1ch], 15h
        mov    word ptr [esp + 1eh], 15h
        push   0
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_B478
        db  0C7h, 005h, 03Ch, 0C2h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_B484
        db  090h
loc_1_B478:
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 4]
        db  0A3h, 03Ch, 0C2h, 000h, 000h
loc_1_B484:
        db  0BBh, 02Ch, 0BEh, 000h, 000h
loc_1_B489:
        mov    edi, dword ptr [ebx]
        mov    ebp, dword ptr [ebx + 0ch]
        add    ebp, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   dword ptr [ebx + 4]
        mov    word ptr [esp + 20h], cx
        mov    ax, cx
        mov    word ptr [esp + 22h], ax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 24h], eax
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_B4DC
        cmp    dword ptr [ebx + 10h], 0
        jne    SHORT loc_1_B504
        mov    eax, dword ptr [ebx + 8]
        jmp    SHORT loc_1_B4E3
        db  090h
loc_1_B4DC:
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 4]
loc_1_B4E3:
        mov    dword ptr [ebp], eax
        add    ebx, 14h
        db  081h, 0FBh, 0E0h, 0BEh, 000h, 000h
        jb     SHORT loc_1_B489
        push   dword ptr [esp + 18h]
        call   sub_1_BA80
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_B504:
        push   dword ptr [esp + 18h]
        call   sub_1_BA80
        push   esi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_1_B520:
        sub    esp, 4
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 10h]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_B9BA
        cmp    dword ptr [esp], 0
        jne    SHORT loc_1_B548
        mov    eax, 1
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_B548:
        sub    eax, eax
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_B550:
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, ecx
        sub    esi, esi
loc_1_B55D:
        sub    ebx, ebx
        nop
loc_1_B560:
        mov    al, byte ptr [edx]
        cmp    al, 30h
        jl     SHORT loc_1_B584
        cmp    al, 39h
        jg     SHORT loc_1_B584
        movsx  eax, al
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [eax + ecx*2 - 30h]
        cmp    ecx, 0ffh
        ja     SHORT loc_1_B5C0
        inc    edx
        mov    ebx, 1
        jmp    SHORT loc_1_B560
loc_1_B584:
        movsx  eax, al
        or     eax, eax
        je     SHORT loc_1_B5C8
        cmp    eax, 9
        je     SHORT loc_1_B5A8
        cmp    eax, 20h
        je     SHORT loc_1_B5A8
        cmp    eax, 2ch
        je     SHORT loc_1_B5C8
        cmp    eax, 2eh
        je     SHORT loc_1_B5AC
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_B5A8:
        inc    edx
        jmp    SHORT loc_1_B560
        db  090h
loc_1_B5AC:
        cmp    esi, 3
        jae    SHORT loc_1_B5F4
        or     ebx, ebx
        je     SHORT loc_1_B5F4
        inc    edx
        shl    edi, 8
        add    edi, ecx
        sub    ecx, ecx
        inc    esi
        jmp    SHORT loc_1_B55D
loc_1_B5C0:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_B5C8:
        cmp    esi, 3
        jne    SHORT loc_1_B5EC
        or     ebx, ebx
        je     SHORT loc_1_B5EC
        shl    edi, 8
        add    ecx, edi
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], edx
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_B5EC:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_B5F4:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_B5FC:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0BBh, 0F8h, 0BEh, 000h, 000h
        mov    esi, dword ptr [esp + 2ch]
loc_1_B60C:
        mov    edi, dword ptr [ebx]
        mov    ebp, dword ptr [ebx + 0ch]
        add    ebp, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   dword ptr [ebx + 4]
        mov    word ptr [esp + 24h], cx
        mov    ax, cx
        mov    word ptr [esp + 26h], ax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 28h], eax
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 38h]
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_B660
        cmp    dword ptr [ebx + 10h], 0
        jne    SHORT loc_1_B6CC
        mov    eax, dword ptr [ebx + 8]
        jmp    SHORT loc_1_B667
        db  08Bh, 0FFh
loc_1_B660:
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 4]
loc_1_B667:
        mov    dword ptr [ebp], eax
        add    ebx, 14h
        db  081h, 0FBh, 020h, 0BFh, 000h, 000h
        jb     SHORT loc_1_B60C
        mov    dword ptr [esi + 8], 0
        lea    edi, [esi + 0ch]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        stosd
        mov    word ptr [esp + 20h], 0fh
        mov    word ptr [esp + 22h], 0fh
        db  0C7h, 044h, 024h, 024h, 034h, 0BFh, 000h, 000h
        push   2
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 38h]
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_B6D8
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_B6CC:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_B6D8:
        sub    edi, edi
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [eax + 8]
loc_1_B6E1:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   ecx
        call   sub_1_B550
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_B718
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [esi + ecx*4 + 0ch], eax
        inc    dword ptr [esi + 8]
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_B718
        mov    ecx, eax
        inc    ecx
        inc    edi
        cmp    edi, 5
        jb     SHORT loc_1_B6E1
loc_1_B718:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
sub_1_B728:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 20h], 0ah
        mov    word ptr [esp + 22h], 0ah
        db  0C7h, 044h, 024h, 024h, 020h, 0BFh, 000h, 000h
        push   2
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 38h]
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_B770
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_1_B770:
        mov    esi, 1
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 8]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_B78D
loc_1_B781:
        cmp    byte ptr [eax], 2ch
        jne    SHORT loc_1_B787
        inc    esi
loc_1_B787:
        inc    eax
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_B781
loc_1_B78D:
        lea    eax, [esi*8]
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_B7B0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
loc_1_B7B0:
        sub    ebx, ebx
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [eax + 8]
        cmp    esi, ebx
        je     SHORT loc_1_B7F2
        mov    ebp, edi
        nop
loc_1_B7C0:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ecx
        call   sub_1_B550
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_B810
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_B7F2
        mov    ecx, eax
        inc    ecx
        add    ebp, 8
        inc    ebx
        cmp    esi, ebx
        ja     SHORT loc_1_B7C0
loc_1_B7F2:
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 1
        je     SHORT loc_1_B824
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_B810:
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  090h
loc_1_B824:
        db  0C7h, 044h, 024h, 024h, 02Ch, 0BFh, 000h, 000h
        mov    word ptr [esp + 20h], 7
        mov    word ptr [esp + 22h], 7
        push   2
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 38h]
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_B870
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_B870:
        sub    ebx, ebx
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [eax + 8]
        cmp    esi, ebx
        je     SHORT loc_1_B8B2
        lea    ebp, [edi + 4]
loc_1_B880:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ecx
        call   sub_1_B550
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_B8D0
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_B8B2
        mov    ecx, eax
        inc    ecx
        add    ebp, 8
        inc    ebx
        cmp    esi, ebx
        ja     SHORT loc_1_B880
loc_1_B8B2:
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 1
        je     SHORT loc_1_B8E4
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_B8D0:
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  090h
loc_1_B8E4:
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [eax], esi
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_1_B8F4:
        push   dword ptr [esp + 4]
        call   sub_1_BA80
        ret
        db  08Bh, 0FFh
sub_1_B900:
        push   dword ptr [esp + 4]
        call   sub_1_B944
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_B910:
        db  0A1h, 044h, 0C1h, 000h, 000h
        ret
        db  08Bh, 0FFh
sub_1_B918:
        db  0A1h, 03Ch, 0C2h, 000h, 000h
        ret
        db  08Bh, 0FFh
sub_1_B920:
        db  0CDh, 020h, 001h, 080h, 088h, 004h
sub_1_B926:
        db  0CDh, 020h, 002h, 080h, 088h, 004h
sub_1_B92C:
        db  0CDh, 020h, 03Fh, 080h, 001h, 000h
sub_1_B932:
        db  0CDh, 020h, 003h, 080h, 088h, 004h
        db  0CDh, 020h, 004h, 080h, 088h, 004h
sub_1_B93E:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_B944:
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
sub_1_B94A:
        db  0CDh, 020h, 011h, 080h, 040h, 000h
sub_1_B950:
        db  0CDh, 020h, 005h, 080h, 088h, 004h
        db  0CDh, 020h, 00Dh, 080h, 088h, 004h
sub_1_B95C:
        db  0CDh, 020h, 006h, 080h, 088h, 004h
sub_1_B962:
        db  0A1h, 06Ch, 0BFh, 000h, 000h
        db  0CDh, 020h, 009h, 080h, 088h, 004h
        db  0A1h, 06Ch, 0BFh, 000h, 000h
        db  0CDh, 020h, 00Ch, 080h, 088h, 004h
        db  0A1h, 06Ch, 0BFh, 000h, 000h
        db  0CDh, 020h, 00Bh, 080h, 088h, 004h
        add    ch, cl
        and    byte ptr [ecx], ch
        sub    byte ptr [eax], 0
sub_1_B98A:
        db  0CDh, 020h, 02Ah, 080h, 028h, 000h
sub_1_B990:
        db  0CDh, 020h, 02Bh, 080h, 028h, 000h
sub_1_B996:
        db  0CDh, 020h, 02Ch, 080h, 028h, 000h
sub_1_B99C:
        db  0CDh, 020h, 02Dh, 080h, 028h, 000h
sub_1_B9A2:
        db  0CDh, 020h, 02Eh, 080h, 028h, 000h
        db  0CDh, 020h, 02Fh, 080h, 028h, 000h
sub_1_B9AE:
        db  0CDh, 020h, 030h, 080h, 028h, 000h
        db  0CDh, 020h, 050h, 080h, 028h, 000h
sub_1_B9BA:
        db  0CDh, 020h, 051h, 080h, 028h, 000h
        db  0CDh, 020h, 052h, 080h, 028h, 000h
        db  0CDh, 020h, 053h, 080h, 028h, 000h
        db  0CDh, 020h, 000h, 080h, 028h, 000h
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
sub_1_BA02:
        db  0CDh, 020h, 016h, 080h, 028h, 000h
sub_1_BA08:
        db  0CDh, 020h, 017h, 080h, 028h, 000h
sub_1_BA0E:
        db  0CDh, 020h, 018h, 080h, 028h, 000h
sub_1_BA14:
        db  0CDh, 020h, 019h, 080h, 028h, 000h
sub_1_BA1A:
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_BA20:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_BA26:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
sub_1_BA2C:
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_BA32:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
sub_1_BA3E:
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_BA44:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
sub_1_BA50:
        db  0CDh, 020h, 023h, 080h, 028h, 000h
sub_1_BA56:
        db  0CDh, 020h, 024h, 080h, 028h, 000h
sub_1_BA5C:
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_BA68:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_BA7A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_BA80:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  000h, 000h, 000h, 000h, 06Ch, 000h, 000h, 000h, 071h, 000h, 000h, 000h, 076h, 000h, 000h, 000h
        db  078h, 000h, 000h, 000h, 02Bh, 002h, 000h, 000h, 015h, 000h, 000h, 000h, 01Ah, 000h, 000h, 000h
        db  07Dh, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 089h, 004h, 003h, 000h, 000h, 000h, 056h, 049h, 050h, 020h
        db  020h, 020h, 020h, 020h, 000h, 080h, 000h, 054h, 0BAh, 002h, 000h, 000h, 03Bh, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0DCh, 0BAh, 000h, 000h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  054h, 068h, 065h, 020h, 073h, 079h, 073h, 074h, 065h, 06Dh, 020h, 068h, 061h, 073h, 020h, 064h
        db  065h, 074h, 065h, 063h, 074h, 065h, 064h, 020h, 061h, 020h, 063h, 06Fh, 06Eh, 066h, 06Ch, 069h
        db  063h, 074h, 020h, 077h, 069h, 074h, 068h, 020h, 074h, 068h, 065h, 020h, 073h, 079h, 073h, 074h
        db  065h, 06Dh, 020h, 068h, 061h, 076h, 069h, 06Eh, 067h, 020h, 068h, 061h, 072h, 064h, 077h, 061h
        db  072h, 065h, 020h, 061h, 064h, 064h, 072h, 065h, 073h, 073h, 020h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 020h, 066h, 06Fh, 072h
        db  020h, 049h, 050h, 020h, 061h, 064h, 064h, 072h, 065h, 073h, 073h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 000h, 054h, 068h, 065h, 020h
        db  06Ch, 06Fh, 063h, 061h, 06Ch, 020h, 069h, 06Eh, 074h, 065h, 072h, 066h, 061h, 063h, 065h, 020h
        db  068h, 061h, 073h, 020h, 062h, 065h, 065h, 06Eh, 020h, 064h, 069h, 073h, 061h, 062h, 06Ch, 065h
        db  064h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_BBFB:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax - 1000000h], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 006h, 000h, 000h
loc_1_BC1C:
        adc    byte ptr [eax - 1], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 082h, 070h, 000h, 000h, 000h
        db  000h
loc_1_BC30:
        push   edi
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0D5h, 000h
loc_1_BC44:
        add    dword ptr [eax], eax
        pop    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_BC4C:
        push   edi
loc_1_BC4D:
        add    dword ptr [eax], eax
        pop    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BC54:
        stosb
        stosb
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BC60:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [esp + ecx], al
        add    cl, byte ptr [edx]
        push   es
        push   cs
        add    dword ptr [ecx], ecx
        add    eax, 70b030dh
        sldt   word ptr [eax - 5fdf3fc0h]
        pushad
        loopne loc_1_BC91
        nop
        push   eax
        sal    byte ptr [eax], 1
        mov    al, 70h
        db  0F0h
loc_1_BC88:
        add    al, 2
        fadd   qword ptr [11780804h]
        db  0FFh
loc_1_BC91:
        pop    ds
        add    byte ptr [eax], al
loc_1_BC94:
        add    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  080h, 035h, 000h, 000h, 09Ch, 035h, 000h
        db  000h, 0B8h, 035h, 000h, 000h, 03Ch
        add    byte ptr ss:[eax], al
        pop    esp
        add    byte ptr ss:[eax], al
        pushad
        add    byte ptr ss:[eax], al
        db  07Ch, 036h
        add    byte ptr [eax], al
        les    esi, ptr [edi]
        add    byte ptr [eax], al
        int3
        aaa
        add    byte ptr [eax], al
        in     al, dx
        aaa
        add    byte ptr [eax], al
        or     byte ptr [ecx], 0
        js     SHORT loc_1_BD12
        add    byte ptr [eax], al
        inc    eax
        dec    ebx
        add    byte ptr [eax], al
        push   es
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [edx], dh
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
        db  0FFh, 0FFh, 00Fh, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_BD00:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
loc_1_BD12:
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 0E0h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_BD40:
        db  0EBh, 0FFh
        db  000h, 000h, 0ECh, 07Ch, 000h, 000h, 0E6h, 045h, 000h, 000h, 0D2h, 01Fh, 000h, 000h, 0ECh, 010h
        db  000h, 000h, 0BEh, 007h, 000h, 000h, 0C0h, 005h, 000h, 000h, 0DAh, 003h, 000h, 000h, 0E8h, 001h
        db  000h, 000h, 014h, 001h, 000h, 000h
loc_1_BD68:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_BD7C:
        dec    ebp
        push   ebx
        and    byte ptr [ebx + eax*2 + 50h], dl
        and    byte ptr [edi + ebp*2 + 6fh], cl
        jo     SHORT loc_1_BDEA
        popad
        arpl   word ptr [ebx + 20h], bp
        imul   ebp, dword ptr [esi + 74h], 61667265h
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
loc_1_BD98:
        add    al, byte ptr [edx]
        add    byte ptr [eax], al
loc_1_BD9C:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        db  000h
loc_1_BDA4:
        inc    ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 52h]
        outsd
        db  075h, 074h
        db  069h, 06Eh, 067h, 000h, 000h, 000h
loc_1_BDB4:
        inc    esi
        outsd
        db  072h, 077h
        popad
        jb     SHORT loc_1_BE1F
        inc    edx
        db  072h, 06Fh
        popad
        arpl   word ptr fs:[ecx + 73h], sp
        je     SHORT loc_1_BE38
        add    byte ptr [eax], al
        add    byte ptr [ebp + 61h], al
        inc    edi
        push   edi
        inc    esp
        db  065h, 074h, 065h
        arpl   word ptr [eax + eax], si
        add    byte ptr [eax], al
loc_1_BDD8:
        push   eax
        dec    ebp
        push   esp
        push   ebp
        inc    esp
        imul   esi, dword ptr [ebx + 63h], 7265766fh
        jns    SHORT loc_1_BDE6
loc_1_BDE6:
        add    byte ptr [eax], al
loc_1_BDE8:
        dec    ecx
        inc    edi
loc_1_BDEA:
        dec    ebp
        push   eax
        dec    esp
        db  065h, 076h, 065h
        insb
        add    byte ptr [eax], al
        db  000h
loc_1_BDF4:
        inc    esp
        popaw
        jne    SHORT loc_1_BE66
        je     SHORT loc_1_BE50
        push   esp
        dec    esp
        add    byte ptr [eax], al
loc_1_BE00:
        inc    esp
        popaw
        jne    SHORT loc_1_BE72
        je     SHORT loc_1_BE5C
        dec    edi
        push   ebx
        add    byte ptr [eax], al
loc_1_BE0C:
        push   edx
        outsd
        db  075h, 074h
        imul   ebp, dword ptr gs:[esi + 67h], 53667542h
        imul   edi, dword ptr [edx + 65h], 756f5200h
loc_1_BE1F:
        db  074h, 065h
        imul   ebp, dword ptr [esi + 67h], 6b636150h
        je     NEAR PTR loc_1_BE9E
        db  000h, 0A4h, 0BDh, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BE38:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    ah, 0bdh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
loc_1_BE50:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        enter  0bdh, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BE5C:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BE66:
        add    byte ptr [eax], al
        fdivr  dword ptr [ebp]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
loc_1_BE72:
        add    byte ptr [eax], al
        sbb    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0E8h, 0BDh, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0F4h
        mov    ebp, 0
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        and    al, 0
loc_1_BE9E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BEB8:
        or     al, 0beh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    dword ptr [eax], eax
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    al, 0beh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BEE0:
        pop    edx
        db  065h, 072h, 06Fh
        inc    edx
        jb     SHORT loc_1_BF56
        popad
        arpl   word ptr fs:[ecx + 73h], sp
        je     SHORT loc_1_BEEE
loc_1_BEEE:
        add    byte ptr [eax], al
loc_1_BEF0:
        dec    ebp
        popad
        js     SHORT loc_1_BF41
        push   esp
        push   ebp
        add    byte ptr [eax], al
loc_1_BEF8:
        loopne loc_1_BEB8
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0F0h, 0BEh, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 004h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_BF20:
        dec    ecx
        push   eax
        inc    ecx
        jb     NEAR PTR loc_1_BF8C
        jae    SHORT loc_1_BF9C
        add    byte ptr [eax], al
        db  000h
loc_1_BF2C:
        dec    ecx
        push   eax
        dec    ebp
        popad
        db  073h, 06Bh
        add    byte ptr [eax], al
loc_1_BF34:
        inc    esp
        popaw
        jne    SHORT loc_1_BFA6
        db  074h, 047h
        popad
        je     SHORT loc_1_BFA4
        ja     SHORT loc_1_BFA2
loc_1_BF41:
        jns    SHORT loc_1_BF43
loc_1_BF43:
        add    byte ptr [ebp + 74h], al
        push   656e7265h
        je     SHORT loc_1_BFA0
        dec    esi
        inc    ecx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF54:
        inc    ecx
        push   edx
loc_1_BF56:
        push   eax
        inc    ecx
        insb
        ja     SHORT loc_1_BFBC
        jns    SHORT loc_1_BFD0
        push   ebx
        outsd
        db  075h, 072h
        arpl   word ptr [ebp + 52h], sp
        outsd
        db  075h, 074h
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_BF6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF74:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF9C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFA0:
        add    byte ptr [eax], al
loc_1_BFA2:
        add    byte ptr [eax], al
loc_1_BFA4:
        add    byte ptr [eax], al
loc_1_BFA6:
        add    byte ptr [eax], al
loc_1_BFA8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFAC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFB0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFB4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFBC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFC8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFCC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFEC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_C000:
        inc    byte ptr [0bbfbh]
        jne    SHORT loc_1_C024
loc_1_C008:
        mov    edi, 0bf70h
        mov    ecx, 0c300h
        sub    ecx, edi
        shr    ecx, 2
        sub    eax, eax
        cld
        rep stosd
        call   sub_1_C2B8
        cmp    eax, 1
loc_1_C024:
        ret
        db  080h, 03Fh, 000h, 074h, 03Ah, 080h, 03Fh, 020h, 074h, 005h, 080h, 03Fh, 009h, 075h, 003h, 047h
        db  0EBh, 0EEh, 080h, 03Fh, 02Ch, 074h, 02Bh, 08Bh, 0F7h, 047h
loc_1_C03F:
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_C062
        cmp    byte ptr [edi], 2ch
        jne    SHORT loc_1_C04F
        mov    byte ptr [edi], 0
loc_1_C04C:
        inc    edi
        jmp    SHORT loc_1_C062
loc_1_C04F:
        cmp    byte ptr [edi], 20h
        je     SHORT loc_1_C05C
loc_1_C054:
        cmp    byte ptr [edi], 9
        je     SHORT loc_1_C05C
        inc    edi
        jmp    SHORT loc_1_C03F
loc_1_C05C:
        mov    byte ptr [edi], 0
        inc    edi
loc_1_C060:
        jmp    SHORT loc_1_C03F
loc_1_C062:
        clc
        ret
loc_1_C064:
        sub    esi, esi
        ret
        db  0F9h
loc_1_C068:
        ret
        db  050h, 0CDh, 020h
loc_1_C06C:
        mov    dl, 0
        add    dword ptr [eax], eax
        mov    edx, eax
        pop    eax
        ret
        db  050h, 052h, 0CDh, 020h, 0ACh, 000h, 001h, 000h, 03Bh, 054h, 024h, 004h, 074h, 003h, 08Bh, 0D0h
        db  0F9h, 058h, 058h, 0F5h, 0C3h
sub_1_C089:
        cmp    byte ptr [eax], 0
loc_1_C08C:
        je     SHORT loc_1_C09B
        cmp    byte ptr [eax], 20h
        je     SHORT loc_1_C098
        cmp    byte ptr [eax], 9
        jne    SHORT loc_1_C09B
loc_1_C098:
        inc    eax
        jmp    SHORT sub_1_C089
loc_1_C09B:
        ret
loc_1_C09C:
        push   ebx
        push   ecx
        push   esi
        mov    eax, edx
        sub    ecx, ecx
        db  02Bh
loc_1_C0A4:
        shr    byte ptr [ebx], cl
        fld    xword ptr [ebx]
        db  0F6h
loc_1_C0A9:
        shl    edx, 8
loc_1_C0AC:
        or     edx, esi
        sub    esi, esi
loc_1_C0B0:
        call   sub_1_C089
        mov    bl, byte ptr [eax]
        sub    bl, 30h
        db  080h, 0FBh
loc_1_C0BC:
        or     dword ptr [edi + 48h], esi
loc_1_C0BF:
        imul   esi, esi, 0ah
        add    esi, ebx
loc_1_C0C4:
        inc    eax
        call   sub_1_C089
        cmp    byte ptr [eax], 2eh
        je     SHORT loc_1_C0E5
        cmp    byte ptr [eax], 0
        db  074h, 027h
loc_1_C0D4:
        cmp    byte ptr [eax], 2ch
        db  074h, 022h
        mov    bl, byte ptr [eax]
        sub    bl, 30h
        cmp    bl, 9
        ja     SHORT loc_1_C107
        jmp    SHORT loc_1_C0BF
loc_1_C0E5:
        inc    eax
        inc    ecx
        cmp    ecx, 4
        jb     SHORT loc_1_C0A9
        call   sub_1_C089
        cmp    byte ptr [eax], 0
loc_1_C0F4:
        db  074h, 005h
        cmp    byte ptr [eax], 2ch
        jne    SHORT loc_1_C107
        db  083h
loc_1_C0FC:
        stc
        add    esi, dword ptr [ebp + 7]
        shl    edx, 8
        or     edx, esi
        jmp    SHORT loc_1_C10A
loc_1_C107:
        sub    edx, edx
        stc
loc_1_C10A:
        pop    esi
        pop    ecx
        pop    ebx
        ret
sub_1_C10E:
        push   ebp
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 2ch
        mov    eax, 21h
        db  0CDh
loc_1_C138:
        and    byte ptr [eax + eax - 3fd4ffffh], al
        movzx  ecx, byte ptr [ebp + 19h]
        imul   ecx, ecx, 36ee80h
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 18h]
        imul   ecx, ecx, 0ea60h
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 15h]
        imul   ecx, ecx, 3e8h
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 14h]
        imul   ecx, ecx, 0ah
        add    eax, ecx
loc_1_C16C:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebx
        pop    ebp
        ret
sub_1_C184:
        sub    ecx, ecx
        movzx  edx, byte ptr [esi]
loc_1_C189:
        imul   ecx, ecx, 0ah
loc_1_C18C:
        sub    edx, 30h
        add    ecx, edx
        inc    esi
        movzx  edx, byte ptr [esi]
        cmp    dl, 30h
        jb     SHORT loc_1_C19F
        cmp    dl, 39h
        jb     SHORT loc_1_C189
loc_1_C19F:
        ret
sub_1_C1A0:
        push   esi
        mov    esi, 0b4ch
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
loc_1_C1AC:
        jb     SHORT loc_1_C22A
        mov    esi, edx
        add    esi, 3
        sub    eax, eax
        cmp    byte ptr [esi], 2dh
loc_1_C1B8:
        jne    SHORT loc_1_C1C2
        mov    eax, 2932e00h
        inc    esi
loc_1_C1C0:
        jmp    SHORT loc_1_C1C8
loc_1_C1C2:
        cmp    byte ptr [esi], 2bh
        jne    SHORT loc_1_C1C8
        inc    esi
loc_1_C1C8:
        cmp    byte ptr [esi], 30h
        jb     SHORT loc_1_C22A
        cmp    byte ptr [esi], 39h
loc_1_C1D0:
        ja     SHORT loc_1_C22A
        call   sub_1_C184
        cmp    ecx, 17h
        ja     SHORT loc_1_C22A
loc_1_C1DC:
        imul   ecx, ecx, 36ee80h
        add    eax, ecx
loc_1_C1E4:
        cmp    byte ptr [esi], 3ah
        jne    SHORT loc_1_C228
        inc    esi
        cmp    byte ptr [esi], 30h
        jb     SHORT loc_1_C22A
        cmp    byte ptr [esi], 39h
        ja     SHORT loc_1_C22A
loc_1_C1F4:
        call   sub_1_C184
        cmp    ecx, 3bh
loc_1_C1FC:
        ja     SHORT loc_1_C22A
        imul   ecx, ecx, 0ea60h
loc_1_C204:
        add    eax, ecx
        cmp    byte ptr [esi], 3ah
        jne    SHORT loc_1_C228
        inc    esi
loc_1_C20C:
        cmp    byte ptr [esi], 30h
        jb     SHORT loc_1_C22A
        cmp    byte ptr [esi], 39h
loc_1_C214:
        ja     SHORT loc_1_C22A
        call   sub_1_C184
        cmp    ecx, 3bh
        ja     SHORT loc_1_C22A
loc_1_C220:
        imul   ecx, ecx, 3e8h
        add    eax, ecx
loc_1_C228:
        pop    esi
        ret
loc_1_C22A:
        mov    eax, 1b77400h
        pop    esi
loc_1_C230:
        ret
        db  000h, 000h, 000h
loc_1_C234:
        push   esi
        push   edi
        mov    dword ptr [0c1d0h], 0
loc_1_C240:
        call   sub_1_C1A0
        mov    esi, eax
        cmp    esi, -1
        jne    SHORT loc_1_C25C
        mov    dword ptr [0c094h], 80000000h
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_C25C:
        call   sub_1_C10E
        cmp    eax, 5265c00h
        jbe    SHORT loc_1_C278
        mov    dword ptr [0c094h], 80h
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_C278:
        mov    edi, eax
        db  0E8h, 0D9h, 06Fh, 0FFh, 0FFh
        sub    edi, eax
        lea    eax, [edi + esi]
        mov    dword ptr [0c1d0h], eax
        mov    dword ptr [0c094h], 0
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh, 056h, 08Bh, 035h, 090h, 0C0h, 000h, 000h, 00Bh, 0F6h, 074h, 010h, 090h, 056h, 0E8h
        db  0DEh, 06Fh, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Bh, 036h, 00Bh, 0F6h, 075h, 0F1h, 05Eh, 0C3h, 08Dh
        db  049h, 000h
sub_1_C2B8:
        sub    esp, 4
        push   esi
        db  0E8h, 067h, 008h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_C2CC
        sub    eax, eax
        pop    esi
loc_1_C2C8:
        add    esp, 4
        ret
loc_1_C2CC:
        db  0E8h, 0C3h, 06Fh, 0FFh, 0FFh
        call   sub_1_26F0
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_C2E4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
        db  090h
loc_1_C2E4:
        push   esi
        mov    eax, dword ptr [esi + 2ch]
        inc    eax
        mov    dword ptr [0bc00h], eax
        db  0E8h, 009h
loc_1_C2F0:
        push   es
        add    byte ptr [eax], al
        db  083h
loc_1_C2F4:
        les    eax, ptr [ebx]
        nop
loc_1_C2F8:
        rol    byte ptr [eax], 0
        db  00Bh
loc_1_C2FC:
        sal    byte ptr [ebp + 9], 2bh
loc_1_C300:
        rcr    byte ptr [esi - 7dh], 0c4h
_LTEXT ends
        end
