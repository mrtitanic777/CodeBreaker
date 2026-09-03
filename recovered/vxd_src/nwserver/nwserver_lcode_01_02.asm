; nwserver_lcode_01_02.asm -- NWSERVER object 1 segment _LTEXT  (part 2/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4E58:
        db  0A1h, 0E0h, 0B1h, 000h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 0E0h, 0B1h, 000h, 000h
        db  0FFh, 005h, 0E4h, 0B1h, 000h, 000h
        ret
sub_1_4E6C:
        db  0A1h, 0F0h, 0B1h, 000h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 0F0h, 0B1h, 000h, 000h
        db  0FFh, 005h, 0F4h, 0B1h, 000h, 000h
        ret
sub_1_4E80:
        db  083h, 03Dh, 0F0h, 0B1h, 000h, 000h, 000h
        mov    eax, 0
        je     SHORT loc_1_4EA3
        db  0A1h, 0F0h, 0B1h, 000h, 000h
        mov    ecx, eax
        db  0FFh, 00Dh, 0F4h, 0B1h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        db  089h, 00Dh, 0F0h, 0B1h, 000h, 000h
loc_1_4EA3:
        ret
sub_1_4EA4:
        push   esi
        push   edi
        db  08Bh, 00Dh, 0F8h, 0B1h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_1_4EBC
        db  083h, 03Dh, 0F4h, 0B1h, 000h, 000h, 003h
        jb     SHORT loc_1_4EBB
        jmp    SHORT loc_1_4EF8
loc_1_4EBB:
        inc    ecx
loc_1_4EBC:
        db  0C7h, 005h, 0FCh, 0B1h, 000h, 000h, 000h, 000h, 000h, 000h
        dec    ecx
        db  089h, 00Dh, 0F8h, 0B1h, 000h, 000h
        push   0
        push   180h
        call   sub_1_4364
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_4EE6
        sub    eax, eax
        jmp    SHORT loc_1_4EFD
loc_1_4EE6:
        mov    edi, 10h
loc_1_4EEB:
        mov    ecx, esi
        add    esi, 18h
        call   sub_1_4E6C
        dec    edi
        jne    SHORT loc_1_4EEB
loc_1_4EF8:
        mov    eax, 1
loc_1_4EFD:
        pop    edi
        pop    esi
        ret
sub_1_4F00:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, edx
        mov    esi, ecx
        db  0A1h, 0D4h, 0B1h, 000h, 000h
        db  003h, 005h, 014h, 0A2h, 000h, 000h
        db  03Bh, 005h, 010h, 0A2h, 000h, 000h
        jbe    SHORT loc_1_4F6A
        db  0A1h, 018h, 0A2h, 000h, 000h
        db  039h, 005h, 01Ch, 0A2h, 000h, 000h
        jbe    SHORT loc_1_4F6A
        db  083h, 03Dh, 000h, 0B4h, 000h, 000h, 000h
        jne    SHORT loc_1_4F38
        call   sub_1_4D0C
        jmp    SHORT loc_1_4F6A
loc_1_4F38:
        db  083h, 03Dh, 0D8h, 0B1h, 000h, 000h, 000h
        jne    SHORT loc_1_4F6A
        db  0FFh, 005h, 0D8h, 0B1h, 000h, 000h
        push   0
        db  068h, 00Ch, 04Dh, 000h, 000h
        push   0
        push   2
        db  0FFh, 035h, 098h, 0B1h, 000h, 000h
        push   1000h
        call   sub_1_43BD
        add    esp, 18h
        db  0A3h, 0DCh, 0B1h, 000h, 000h
loc_1_4F6A:
        db  083h, 03Dh, 000h, 0B4h, 000h, 000h, 000h
        jne    SHORT loc_1_4F83
        db  083h, 03Dh, 0E4h, 0B1h, 000h, 000h, 003h
        jae    SHORT loc_1_4FBE
        call   sub_1_4D7C
        jmp    SHORT loc_1_4FBE
loc_1_4F83:
        db  083h, 03Dh, 0E8h, 0B1h, 000h, 000h, 000h
        jne    SHORT loc_1_4FBE
        db  083h, 03Dh, 0E4h, 0B1h, 000h, 000h, 003h
        ja     SHORT loc_1_4FBE
        db  0FFh, 005h, 0E8h, 0B1h, 000h, 000h
        push   0
        db  068h, 07Ch, 04Dh, 000h, 000h
        push   0
        push   2
        db  0FFh, 035h, 098h, 0B1h, 000h, 000h
        push   1000h
        call   sub_1_43BD
        add    esp, 18h
        db  0A3h, 0ECh, 0B1h, 000h, 000h
loc_1_4FBE:
        db  083h, 03Dh, 000h, 0B4h, 000h, 000h, 000h
        jne    SHORT loc_1_4FD7
        db  083h, 03Dh, 0F4h, 0B1h, 000h, 000h, 003h
        jae    SHORT loc_1_5012
        call   sub_1_4EA4
        jmp    SHORT loc_1_5012
loc_1_4FD7:
        db  083h, 03Dh, 0F8h, 0B1h, 000h, 000h, 000h
        jne    SHORT loc_1_5012
        db  083h, 03Dh, 0F4h, 0B1h, 000h, 000h, 003h
        ja     SHORT loc_1_5012
        db  0FFh, 005h, 0F8h, 0B1h, 000h, 000h
        push   0
        db  068h, 0A4h, 04Eh, 000h, 000h
        push   0
        push   2
        db  0FFh, 035h, 098h, 0B1h, 000h, 000h
        push   1000h
        call   sub_1_43BD
        add    esp, 18h
        db  0A3h, 0FCh, 0B1h, 000h, 000h
loc_1_5012:
        lea    eax, [esi + 0bah]
        mov    dword ptr [ebp], eax
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [esp + 18h]
        and    ecx, 0ffffh
        mov    dword ptr [eax], ecx
        call   sub_1_7C6E
        mov    ecx, dword ptr [esp + 1ch]
        or     eax, eax
        mov    dword ptr [ecx], eax
        je     SHORT loc_1_509E
        mov    edi, dword ptr [eax + 50h]
        or     edi, edi
        je     SHORT loc_1_509E
        mov    ebx, dword ptr [esp + 14h]
        mov    dword ptr [ebx], edi
        mov    eax, dword ptr [ebp]
        mov    ecx, dword ptr [edi + 58h]
        cmp    dword ptr [eax + 8], ecx
        je     SHORT loc_1_5053
        jmp    SHORT loc_1_509E
loc_1_5053:
        add    esi, 1ch
        add    edi, 72h
        movsd
        movsw
        mov    eax, dword ptr [ebp]
        mov    ax, word ptr [eax + 14h]
        push   eax
        call   sub_1_3A60
        add    esp, 4
        mov    ecx, dword ptr [ebx]
        cmp    ax, word ptr [ecx + 1ah]
        jne    SHORT loc_1_507B
        mov    eax, 1
        jmp    SHORT loc_1_50A0
loc_1_507B:
        mov    eax, dword ptr [ebp]
        mov    ax, word ptr [eax + 14h]
        push   eax
        call   sub_1_3A60
        add    esp, 4
        mov    ecx, dword ptr [ebx]
        sub    ax, word ptr [ecx + 1ah]
        cmp    ax, 1
        jne    SHORT loc_1_509E
        mov    eax, 2
        jmp    SHORT loc_1_50A0
loc_1_509E:
        sub    eax, eax
loc_1_50A0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
sub_1_50A7:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [ecx + 90h]
        mov    edi, dword ptr [ecx + 94h]
        lea    eax, [esi + edi - 1]
        mov    dword ptr [esp + 10h], eax
        mov    eax, edx
        mov    dword ptr [esp + 14h], edx
        mov    ebx, dword ptr [eax]
        cmp    edx, ebx
        je     SHORT loc_1_5129
loc_1_50CE:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 0ch]
        lea    ebp, [ebx + 0ch]
        lea    edx, [ecx + eax - 1]
        cmp    ecx, dword ptr [esp + 10h]
        ja     SHORT loc_1_5121
        cmp    edx, esi
        jb     SHORT loc_1_5121
        cmp    ecx, esi
        jb     SHORT loc_1_510C
        sub    edi, ecx
        add    edi, esi
        mov    esi, ecx
        cmp    edx, dword ptr [esp + 10h]
        ja     SHORT loc_1_5137
        mov    ebp, dword ptr [ebx + 4]
        push   ebx
        call   sub_1_0060
        add    esp, 4
        mov    ecx, ebx
        call   sub_1_4E58
        mov    ebx, ebp
        jmp    SHORT loc_1_5121
loc_1_510C:
        cmp    edx, dword ptr [esp + 10h]
        ja     SHORT loc_1_5148
        mov    ecx, edx
        sub    ecx, esi
        inc    ecx
        sub    edi, ecx
        lea    esi, [edx + 1]
        sub    eax, ecx
        mov    dword ptr [ebp], eax
loc_1_5121:
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [esp + 14h], ebx
        jne    SHORT loc_1_50CE
loc_1_5129:
        mov    eax, dword ptr [esp + 1ch]
loc_1_512D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    4
loc_1_5137:
        mov    eax, dword ptr [esp + 10h]
        inc    eax
        mov    dword ptr [ebx + 8], eax
        sub    edx, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 0ch], edx
        jmp    SHORT loc_1_5129
loc_1_5148:
        mov    eax, dword ptr [esp + 1ch]
        or     eax, eax
        je     SHORT loc_1_512D
        sub    esi, ecx
        mov    dword ptr [ebx + 0ch], esi
        mov    ecx, dword ptr [esp + 10h]
        inc    ecx
        mov    dword ptr [eax + 8], ecx
        sub    edx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 0ch], edx
        push   ebx
        push   eax
        call   sub_1_0030
        add    esp, 8
        sub    eax, eax
        jmp    SHORT loc_1_512D
sub_1_5172:
        push   esi
        mov    esi, ecx
        mov    ax, word ptr [esi + 8]
        dec    ax
        mov    word ptr [esi + 8], ax
        mov    eax, 0
        jne    SHORT loc_1_51A6
        push   esi
        call   sub_1_0060
        add    esp, 4
        push   dword ptr [esi + 10h]
        call   sub_1_144C
        add    esp, 4
        mov    ecx, esi
        call   sub_1_4E6C
        mov    eax, 1
loc_1_51A6:
        pop    esi
        ret
sub_1_51A8:
        push   esi
        push   edi
        mov    esi, ecx
        db  083h, 03Dh, 008h, 0B2h, 000h, 000h, 000h
        mov    edi, dword ptr [esi + 50h]
        jne    SHORT loc_1_51D1
        db  0C7h, 005h, 008h, 0B2h, 000h, 000h, 001h, 000h, 000h, 000h
        call   sub_1_93AB
        db  0C7h, 005h, 008h, 0B2h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_51D1:
        cmp    dword ptr [esi + 34h], 0
        je     SHORT loc_1_51DF
        test   byte ptr [edi + 18h], 4
        je     SHORT loc_1_5213
        jmp    SHORT loc_1_520C
loc_1_51DF:
        mov    eax, dword ptr [esi + 50h]
        test   byte ptr [eax + 18h], 1
        je     SHORT loc_1_5213
        mov    al, byte ptr [edi + 18h]
        test   al, 4
        jne    SHORT loc_1_520C
        or     al, 4
        mov    byte ptr [edi + 18h], al
        or     word ptr [esi + 1ah], 8
        call   sub_1_16C0
        mov    dword ptr [esi + 54h], eax
        push   0
        push   esi
        call   sub_1_19B5
        add    esp, 8
loc_1_520C:
        mov    eax, 1
        jmp    SHORT loc_1_5215
loc_1_5213:
        sub    eax, eax
loc_1_5215:
        pop    edi
        pop    esi
        ret
sub_1_5218:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 50h]
        mov    dword ptr [eax + 38h], 0
        call   sub_1_16C0
        mov    dword ptr [esi + 54h], eax
        push   0
        push   esi
        call   sub_1_19B5
        add    esp, 8
        pop    esi
        ret
loc_1_523C:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_4DD8
        mov    ecx, dword ptr [esp + 48h]
        mov    dword ptr [esp + 2ch], eax
        mov    esi, dword ptr [ecx + 98h]
        or     esi, esi
        je     SHORT loc_1_5261
        mov    ebx, dword ptr [esi + 0ch]
        mov    edi, dword ptr [esi]
        jmp    SHORT loc_1_5269
loc_1_5261:
        mov    edi, dword ptr [esp + 38h]
        mov    ebx, dword ptr [esp + 3ch]
loc_1_5269:
        mov    ecx, dword ptr [esp + 48h]
        mov    dword ptr [esp + 1ch], 1
        lea    eax, [esp + 30h]
        push   eax
        movzx  ebp, word ptr [ecx + 22h]
        lea    eax, [esp + 44h]
        push   eax
        lea    eax, [esp + 20h]
        lea    edx, [esp + 3ch]
        push   eax
        call   sub_1_4F00
        cmp    eax, 1
        jne    NEAR PTR loc_1_540B
        cmp    ebp, 1
        jbe    SHORT loc_1_52C5
        mov    eax, dword ptr [esp + 48h]
        add    eax, 2ah
        db  08Bh, 00Dh, 0C8h, 013h, 000h, 000h
loc_1_52AC:
        mov    edx, dword ptr [eax]
        and    edx, 0fffff000h
        cmp    edx, ecx
        jne    SHORT loc_1_52C5
        add    eax, 6
        inc    dword ptr [esp + 1ch]
        cmp    ebp, dword ptr [esp + 1ch]
        ja     SHORT loc_1_52AC
loc_1_52C5:
        mov    ecx, dword ptr [esp + 48h]
        mov    al, byte ptr [ecx + 9]
        or     al, al
        je     SHORT loc_1_52D8
        cmp    al, 0fdh
        jne    NEAR PTR loc_1_540B
loc_1_52D8:
        mov    eax, dword ptr [esp + 18h]
        test   byte ptr [eax + 18h], 1
        je     NEAR PTR loc_1_540B
        mov    edx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 48h]
        push   dword ptr [esp + 2ch]
        call   sub_1_50A7
        mov    dword ptr [esp + 2ch], eax
        cmp    ebp, dword ptr [esp + 1ch]
        jbe    NEAR PTR loc_1_53E1
        mov    eax, dword ptr [esp + 1ch]
        imul   eax, eax, 6
        mov    ecx, dword ptr [esp + 48h]
        lea    ecx, [eax + ecx + 24h]
        mov    dword ptr [esp + 24h], ecx
        sub    ebp, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 28h], ebp
loc_1_5320:
        mov    ecx, dword ptr [esp + 24h]
        db  0A1h, 0C8h, 013h, 000h, 000h
        cmp    dword ptr [ecx], eax
        je     NEAR PTR loc_1_53CC
        cmp    dword ptr [esi + 0ch], ebx
        je     SHORT loc_1_533F
loc_1_5336:
        mov    esi, edi
        mov    edi, dword ptr [edi]
        cmp    dword ptr [esi + 0ch], ebx
        jne    SHORT loc_1_5336
loc_1_533F:
        mov    ecx, esi
        call   sub_1_5172
        or     eax, eax
        jne    SHORT loc_1_53C8
        cmp    word ptr [esi + 8], 1
        jne    SHORT loc_1_53C8
        cmp    word ptr [esi + 0ah], 1
        jne    SHORT loc_1_53C8
        mov    edx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [esp + 10h], eax
        mov    ecx, dword ptr [edx]
        add    eax, 0fffh
        mov    dword ptr [esp + 14h], eax
        cmp    edx, ecx
        je     SHORT loc_1_538E
loc_1_5372:
        mov    eax, dword ptr [ecx + 8]
        mov    ebp, dword ptr [ecx + 0ch]
        lea    ebp, [eax + ebp - 1]
        cmp    eax, dword ptr [esp + 14h]
        ja     SHORT loc_1_5388
        cmp    dword ptr [esp + 10h], ebp
        jbe    SHORT loc_1_53C8
loc_1_5388:
        mov    ecx, dword ptr [ecx]
        cmp    edx, ecx
        jne    SHORT loc_1_5372
loc_1_538E:
        mov    ebp, dword ptr [esp + 18h]
        push   esi
        call   sub_1_0060
        add    esp, 4
        add    ebp, 10h
        mov    edx, ebp
        mov    eax, dword ptr [ebp + 4]
        cmp    ebp, eax
        je     SHORT loc_1_53B8
        mov    ecx, dword ptr [esi + 0ch]
loc_1_53AA:
        cmp    dword ptr [eax + 0ch], ecx
        jbe    SHORT loc_1_53B8
        mov    edx, eax
        mov    eax, dword ptr [eax + 4]
        cmp    ebp, eax
        jne    SHORT loc_1_53AA
loc_1_53B8:
        push   edx
        push   esi
        call   sub_1_0047
        add    esp, 8
        mov    word ptr [esi + 0ah], 2
loc_1_53C8:
        mov    esi, edi
        mov    edi, dword ptr [edi]
loc_1_53CC:
        add    ebx, 1000h
        add    dword ptr [esp + 24h], 6
        dec    dword ptr [esp + 28h]
        jne    NEAR PTR loc_1_5320
loc_1_53E1:
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 10h]
        sub    eax, dword ptr [esp + 18h]
        cmp    eax, 10h
        je     SHORT loc_1_53FA
        mov    ecx, dword ptr [esp + 30h]
        call   sub_1_51A8
loc_1_53FA:
        push   dword ptr [esp + 48h]
        call   sub_1_2811
        add    esp, 4
        jmp    NEAR PTR loc_1_549B
loc_1_540B:
        cmp    ebp, dword ptr [esp + 1ch]
        jbe    SHORT loc_1_545D
        mov    eax, dword ptr [esp + 1ch]
        imul   eax, eax, 6
        mov    ecx, dword ptr [esp + 48h]
        lea    eax, [eax + ecx + 24h]
        mov    dword ptr [esp + 20h], eax
        sub    ebp, dword ptr [esp + 1ch]
loc_1_5428:
        mov    eax, dword ptr [esp + 20h]
        db  08Bh, 00Dh, 0C8h, 013h, 000h, 000h
        cmp    dword ptr [eax], ecx
        je     SHORT loc_1_544F
        cmp    dword ptr [esi + 0ch], ebx
        je     SHORT loc_1_5444
loc_1_543B:
        mov    esi, edi
        mov    edi, dword ptr [edi]
        cmp    dword ptr [esi + 0ch], ebx
        jne    SHORT loc_1_543B
loc_1_5444:
        mov    ecx, esi
        call   sub_1_5172
        mov    esi, edi
        mov    edi, dword ptr [edi]
loc_1_544F:
        add    ebx, 1000h
        add    dword ptr [esp + 20h], 6
        dec    ebp
        jne    SHORT loc_1_5428
loc_1_545D:
        mov    eax, dword ptr [esp + 34h]
        test   byte ptr [eax + 2], 10h
        je     SHORT loc_1_5476
        mov    edx, dword ptr [esp + 30h]
        mov    ecx, dword ptr [esp + 48h]
        call   sub_1_578D
        jmp    SHORT loc_1_5482
loc_1_5476:
        push   dword ptr [esp + 48h]
        call   sub_1_2811
        add    esp, 4
loc_1_5482:
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 10h]
        sub    eax, dword ptr [esp + 18h]
        cmp    eax, 10h
        je     SHORT loc_1_549B
        mov    ecx, dword ptr [esp + 30h]
        call   sub_1_51A8
loc_1_549B:
        cmp    dword ptr [esp + 2ch], 0
        je     SHORT loc_1_54AB
        mov    ecx, dword ptr [esp + 2ch]
        call   sub_1_4E58
loc_1_54AB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
sub_1_54B3:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    ebx, ecx
        mov    ebp, dword ptr [esi + 50h]
        add    esi, 28h
        lea    edi, [edx + 6]
        movsd
        movsd
        movsw
        lea    esi, [ebp + 70h]
        lea    edi, [edx + 10h]
        movsw
        mov    esi, dword ptr [esp + 14h]
        mov    byte ptr [edx + 5], 11h
        mov    word ptr [esi], 7777h
        mov    byte ptr [esi + 3], 2
        mov    eax, dword ptr [ebp + 58h]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebp + 60h]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        lea    ecx, [eax + 1]
        mov    dword ptr [ebp + 1ch], ecx
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 10h], 0
        mov    ax, word ptr [ebp + 1ah]
        push   eax
        call   sub_1_3A60
        add    esp, 4
        mov    word ptr [esi + 14h], ax
        mov    word ptr [esi + 16h], ax
        mov    dword ptr [esi + 18h], 0
        mov    dword ptr [esi + 1ch], 0
        lea    edi, [ebx + 1ch]
        mov    word ptr [esi + 20h], 0
        mov    word ptr [esi + 22h], 0
        lea    esi, [ebp + 72h]
        movsd
        movsw
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_1_554A:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 20h], edx
        sub    esi, esi
        mov    eax, edx
        mov    dword ptr [esp + 18h], ecx
        mov    eax, dword ptr [eax + 50h]
        mov    dword ptr [esp + 10h], eax
        add    ecx, 9ch
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esp + 1ch], ecx
        mov    word ptr [eax + 22h], 1
        mov    edx, eax
        mov    dword ptr [eax + 24h], ecx
        add    edx, 0deh
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 14h], edx
        mov    edi, dword ptr [eax]
        cmp    eax, edi
        je     NEAR PTR loc_1_5631
loc_1_5595:
        db  0FFh, 005h, 074h, 0B1h, 000h, 000h
        mov    eax, esi
        imul   eax, eax, 6
        mov    ebx, dword ptr [esp + 14h]
        add    ebx, eax
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edi + 8]
        sub    ecx, dword ptr [eax + 28h]
        add    ecx, 18h
        push   ecx
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [ebx], eax
        mov    ebp, dword ptr [edi + 0ch]
        cmp    ebp, 0ffffh
        jbe    SHORT loc_1_5604
        add    ebx, 4
loc_1_55CC:
        mov    word ptr [ebx], 0ffffh
        add    ebx, 6
        inc    esi
        cmp    esi, 11h
        ja     SHORT loc_1_5631
        push   dword ptr [ebx - 0ah]
        call   sub_1_3A68
        add    esp, 4
        add    eax, 0ffffh
        push   eax
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [ebx - 4], eax
        sub    ebp, 0ffffh
        cmp    ebp, 0ffffh
        ja     SHORT loc_1_55CC
loc_1_5604:
        or     ebp, ebp
        je     SHORT loc_1_5625
        push   ebp
        call   sub_1_3A60
        add    esp, 4
        mov    ecx, esi
        imul   ecx, ecx, 6
        mov    edx, dword ptr [esp + 14h]
        mov    word ptr [ecx + edx + 4], ax
        inc    esi
        cmp    esi, 11h
        ja     SHORT loc_1_5631
loc_1_5625:
        mov    edi, dword ptr [edi]
        cmp    dword ptr [esp + 10h], edi
        jne    NEAR PTR loc_1_5595
loc_1_5631:
        mov    edi, dword ptr [esp + 18h]
        add    edi, 0bah
        push   dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 1ch]
        push   edi
        call   sub_1_54B3
        mov    byte ptr [edi + 2], 80h
        push   esi
        call   sub_1_3A60
        add    esp, 4
        mov    word ptr [edi + 22h], ax
        add    si, 0bh
        mov    ecx, dword ptr [esp + 1ch]
        imul   si, si, 6
        mov    eax, dword ptr [esp + 18h]
        mov    word ptr [ecx + 2], si
        mov    word ptr [eax + 28h], si
        db  0C7h, 040h, 004h, 011h, 028h, 000h, 000h
        push   eax
        call   sub_1_318D
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_1_568E:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   edx
        lea    edi, [esi + 9ch]
        lea    ebx, [esi + 0bah]
        push   ebx
        mov    edx, edi
        mov    ecx, esi
        call   sub_1_54B3
        mov    word ptr [esi + 22h], 1
        mov    dword ptr [esi + 24h], edi
        mov    word ptr [edi + 2], 42h
        mov    word ptr [esi + 28h], 42h
        mov    byte ptr [ebx + 2], 88h
        mov    word ptr [ebx + 22h], 0
        db  0C7h, 046h, 004h, 011h, 028h, 000h, 000h
        push   esi
        call   sub_1_318D
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_56DD:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        mov    dword ptr [esp + 10h], edx
        lea    eax, [esi + 0deh]
        lea    edi, [esi + 0bah]
        mov    dword ptr [esp + 14h], eax
        mov    eax, edx
        lea    ebx, [esi + 9ch]
        mov    ebp, dword ptr [eax + 50h]
        mov    ecx, dword ptr [ebp + 20h]
        or     ecx, ecx
        jne    SHORT loc_1_5718
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_1_5728
loc_1_5718:
        sub    edx, edx
        call   sub_1_2A00
        movzx  ecx, al
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], ecx
loc_1_5728:
        push   dword ptr [esp + 10h]
        push   edi
        mov    edx, ebx
        mov    ecx, esi
        call   sub_1_54B3
        mov    word ptr [esi + 22h], 1
        mov    dword ptr [esi + 24h], ebx
        mov    word ptr [ebx + 2], 46h
        mov    word ptr [esi + 28h], 46h
        mov    byte ptr [edi + 2], 10h
        mov    ax, word ptr [ebp + 1ah]
        inc    ax
        push   eax
        call   sub_1_3A60
        add    esp, 4
        mov    word ptr [edi + 16h], ax
        mov    word ptr [edi + 20h], 400h
        mov    dword ptr [edi + 18h], 4000000h
        mov    word ptr [edi + 22h], 0
        db  0C7h, 046h, 004h, 011h, 028h, 000h, 000h
        push   esi
        call   sub_1_318D
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_578D:
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, dword ptr [esi + 50h]
        test   byte ptr [edi + 18h], 1
        je     SHORT loc_1_57BF
        cmp    dword ptr [edi], edi
        mov    edx, esi
        jne    SHORT loc_1_57B8
        call   sub_1_568E
        mov    eax, dword ptr [edi + 10h]
        sub    eax, edi
        cmp    eax, 10h
        je     SHORT loc_1_57C6
        mov    ecx, esi
        call   sub_1_51A8
        jmp    SHORT loc_1_57C6
loc_1_57B8:
        call   sub_1_554A
        jmp    SHORT loc_1_57C6
loc_1_57BF:
        mov    edx, esi
        call   sub_1_56DD
loc_1_57C6:
        pop    edi
        pop    esi
        ret
loc_1_57C9:
        sub    esp, 10h
        push   esi
        mov    dword ptr [esp + 4], 0
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 0ch]
        lea    edx, [esp + 18h]
        mov    esi, dword ptr [esp + 20h]
        push   eax
        mov    ecx, esi
        call   sub_1_4F00
        cmp    eax, 1
        je     SHORT loc_1_5812
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_5807
        mov    eax, dword ptr [esp + 4]
        and    byte ptr [eax + 18h], 0f7h
loc_1_5807:
        push   esi
        call   sub_1_2811
        add    esp, 4
        jmp    SHORT loc_1_5825
loc_1_5812:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, esi
        and    byte ptr [eax + 18h], 0f7h
        mov    edx, dword ptr [esp + 8]
        call   sub_1_578D
loc_1_5825:
        pop    esi
        add    esp, 10h
        ret
loc_1_582A:
        sub    esp, 10h
        push   esi
        mov    dword ptr [esp + 8], 0
        lea    eax, [esp + 4]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 10h]
        lea    edx, [esp + 18h]
        mov    esi, dword ptr [esp + 20h]
        push   eax
        mov    ecx, esi
        call   sub_1_4F00
        cmp    eax, 1
        je     SHORT loc_1_5864
        push   esi
        call   sub_1_2811
        add    esp, 4
        jmp    SHORT loc_1_586F
loc_1_5864:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, esi
        call   sub_1_56DD
loc_1_586F:
        pop    esi
        add    esp, 10h
        ret
loc_1_5874:
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 8], 0
        cmp    byte ptr [esi + 9], 0
        jne    SHORT loc_1_58C6
        lea    eax, [esp + 0ch]
        push   eax
        mov    ecx, esi
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 10h]
        lea    edx, [esp + 18h]
        push   eax
        call   sub_1_4F00
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 8]
        add    edi, 24h
        mov    cl, byte ptr [ecx + 18h]
        and    cl, 1
        jne    SHORT loc_1_58C6
        cmp    eax, 1
        je     SHORT loc_1_58DB
        cmp    eax, 2
        je     NEAR PTR loc_1_5960
loc_1_58C6:
        db  0FFh, 005h, 074h, 0B1h, 000h, 000h
        push   esi
        call   sub_1_2811
        add    esp, 4
loc_1_58D5:
        pop    edi
        pop    esi
        add    esp, 10h
        ret
loc_1_58DB:
        or     cl, cl
        je     SHORT loc_1_58EC
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, esi
        call   sub_1_568E
        jmp    SHORT loc_1_58D5
loc_1_58EC:
        mov    eax, dword ptr [esp + 8]
        add    eax, 20h
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_594A
        push   dword ptr [edi + 14h]
        call   sub_1_3A68
        mov    ecx, dword ptr [esp + 0ch]
        add    esp, 4
        cmp    eax, dword ptr [ecx + 54h]
        jne    SHORT loc_1_58C6
        mov    eax, dword ptr [edi + 4]
        cmp    dword ptr [ecx + 24h], eax
        jne    SHORT loc_1_58C6
        mov    eax, ecx
        or     byte ptr [eax + 18h], 1
        mov    eax, dword ptr [esp + 8]
        add    eax, 2ch
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_5931
        push   esi
        call   sub_1_2811
        add    esp, 4
        jmp    SHORT loc_1_5933
loc_1_5931:
        mov    dword ptr [eax], esi
loc_1_5933:
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_1_51A8
        or     eax, eax
        jne    SHORT loc_1_58D5
        mov    eax, dword ptr [esp + 8]
        and    byte ptr [eax + 18h], 0feh
        jmp    SHORT loc_1_58D5
loc_1_594A:
        mov    dword ptr [eax], 0ffh
        mov    edx, esi
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_1_5BC5
        jmp    NEAR PTR loc_1_58D5
loc_1_5960:
        mov    eax, dword ptr [esp + 8]
        inc    word ptr [eax + 1ah]
        mov    eax, dword ptr [esp + 8]
        db  0FFh, 005h, 080h, 0B1h, 000h, 000h
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [esp + 8]
        or     byte ptr [eax + 18h], 1
        mov    eax, dword ptr [esp + 8]
        and    byte ptr [eax + 18h], 0fdh
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 24h], eax
        push   dword ptr [edi + 10h]
        call   sub_1_3A68
        mov    ecx, dword ptr [esp + 0ch]
        add    esp, 4
        mov    dword ptr [ecx + 28h], eax
        push   dword ptr [edi + 14h]
        call   sub_1_3A68
        mov    ecx, dword ptr [esp + 0ch]
        add    esp, 4
        mov    dword ptr [ecx + 54h], eax
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 2ch]
        or     eax, eax
        je     SHORT loc_1_59CB
        push   eax
        call   sub_1_2811
        add    esp, 4
loc_1_59CB:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 2ch], esi
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_1_51A8
        or     eax, eax
        jne    NEAR PTR loc_1_58D5
        mov    eax, dword ptr [esp + 8]
        and    byte ptr [eax + 18h], 0feh
        jmp    NEAR PTR loc_1_58D5
loc_1_59F0:
        sub    esp, 10h
        push   esi
        mov    dword ptr [esp + 4], 0
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 0ch]
        lea    edx, [esp + 14h]
        mov    esi, dword ptr [esp + 20h]
        push   eax
        mov    ecx, esi
        call   sub_1_4F00
        mov    cl, byte ptr [esi + 9]
        or     cl, cl
        je     SHORT loc_1_5A26
        cmp    cl, 0fdh
        jne    SHORT loc_1_5A34
loc_1_5A26:
        cmp    eax, 1
        je     SHORT loc_1_5A57
        cmp    eax, 2
        je     NEAR PTR loc_1_5B16
loc_1_5A34:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_5A43
        mov    eax, dword ptr [esp + 4]
        and    byte ptr [eax + 18h], 0f7h
loc_1_5A43:
        db  0FFh, 005h, 074h, 0B1h, 000h, 000h
        push   esi
        call   sub_1_2811
        add    esp, 4
loc_1_5A52:
        pop    esi
        add    esp, 10h
        ret
loc_1_5A57:
        mov    eax, dword ptr [esp + 4]
        and    byte ptr [eax + 18h], 0f7h
        mov    eax, dword ptr [esp + 0ch]
        mov    ax, word ptr [eax + 22h]
        or     ax, ax
        je     SHORT loc_1_5A34
        push   eax
        call   sub_1_3A60
        add    esp, 4
        cmp    ax, 11h
        jbe    SHORT loc_1_5A85
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [eax + 22h], 1100h
loc_1_5A85:
        mov    eax, dword ptr [esp + 4]
        or     byte ptr [eax + 18h], 1
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 20h], 0
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 2ch]
        add    ecx, 2ch
        or     eax, eax
        je     SHORT loc_1_5AE8
        test   byte ptr [eax + 0bch], 80h
        je     SHORT loc_1_5AC1
        push   eax
        call   sub_1_2811
        mov    eax, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [eax + 2ch], esi
        jmp    SHORT loc_1_5ACA
loc_1_5AC1:
        push   esi
        call   sub_1_2811
        add    esp, 4
loc_1_5ACA:
        mov    ecx, dword ptr [esp + 8]
        call   sub_1_51A8
        or     eax, eax
        jne    NEAR PTR loc_1_5A52
        mov    eax, dword ptr [esp + 4]
        and    byte ptr [eax + 18h], 0feh
        jmp    NEAR PTR loc_1_5A52
loc_1_5AE8:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 18h], 4
        jne    NEAR PTR loc_1_5A34
        mov    dword ptr [ecx], esi
        mov    ecx, dword ptr [esp + 8]
        call   sub_1_51A8
        or     eax, eax
        jne    NEAR PTR loc_1_5A52
        mov    eax, dword ptr [esp + 4]
        and    byte ptr [eax + 18h], 0feh
        jmp    NEAR PTR loc_1_5A52
loc_1_5B16:
        mov    edx, esi
        mov    ecx, dword ptr [esp + 8]
        call   sub_1_5B64
        mov    eax, dword ptr [esp + 4]
        and    byte ptr [eax + 18h], 0f7h
        jmp    NEAR PTR loc_1_5A52
sub_1_5B2E:
        sub    eax, eax
        db  08Bh, 00Dh, 010h, 0A2h, 000h, 000h
        db  039h, 00Dh, 014h, 0A2h, 000h, 000h
        jae    SHORT loc_1_5B43
        call   sub_1_27D3
loc_1_5B43:
        or     eax, eax
        jne    SHORT loc_1_5B63
        db  08Bh, 00Dh, 018h, 0A2h, 000h, 000h
        db  039h, 00Dh, 01Ch, 0A2h, 000h, 000h
        jbe    SHORT loc_1_5B63
        call   sub_1_2869
        or     eax, eax
        jne    SHORT loc_1_5B63
        call   sub_1_27D3
loc_1_5B63:
        ret
sub_1_5B64:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, edx
        mov    esi, dword ptr [ecx + 50h]
        lea    ebp, [ebx + 9ch]
        lea    edi, [ebx + 0bah]
        push   ecx
        push   edi
        mov    edx, ebp
        mov    ecx, ebx
        call   sub_1_54B3
        mov    word ptr [ebx + 22h], 1
        mov    dword ptr [ebx + 24h], ebp
        mov    word ptr [ebp + 2], 42h
        mov    word ptr [ebx + 28h], 42h
        mov    byte ptr [edi + 2], 84h
        mov    ax, word ptr [esi + 1ah]
        inc    ax
        push   eax
        call   sub_1_3A60
        add    esp, 4
        mov    word ptr [edi + 16h], ax
        db  0C7h, 043h, 004h, 011h, 028h, 000h, 000h
        push   ebx
        call   sub_1_318D
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_5BC5:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, edx
        mov    ebx, dword ptr [ecx + 50h]
        lea    ebp, [edi + 9ch]
        lea    esi, [edi + 0bah]
        push   ecx
        push   esi
        mov    edx, ebp
        mov    ecx, edi
        call   sub_1_54B3
        mov    word ptr [edi + 22h], 1
        mov    dword ptr [edi + 24h], ebp
        mov    word ptr [ebp + 2], 4ah
        mov    word ptr [edi + 28h], 4ah
        mov    byte ptr [esi + 2], 10h
        mov    dword ptr [esi + 18h], 8000000h
        mov    word ptr [esi + 20h], 800h
        mov    ax, word ptr [ebx + 1ah]
        inc    ax
        push   eax
        call   sub_1_3A60
        add    esp, 4
        mov    word ptr [esi + 16h], ax
        lea    esi, [edi + 0deh]
        sub    edx, edx
        mov    ecx, dword ptr [ebx + 20h]
        call   sub_1_2A00
        movzx  eax, al
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], 0
        db  0C7h, 047h, 004h, 011h, 028h, 000h, 000h
        push   edi
        call   sub_1_318D
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_5C4F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 90h]
        mov    ecx, dword ptr [eax]
        dec    ecx
        mov    dword ptr [eax], ecx
        jne    SHORT loc_1_5C8D
        db  083h, 03Dh, 000h, 0B4h, 000h, 000h, 000h
        push   eax
        jne    SHORT loc_1_5C72
        call   sub_1_1632
        jmp    SHORT loc_1_5C77
loc_1_5C72:
        call   sub_1_167C
loc_1_5C77:
        add    esp, 4
        push   dword ptr [esi + 94h]
        db  068h, 030h, 0B9h, 000h, 000h
        call   sub_1_151C
        add    esp, 8
loc_1_5C8D:
        push   esi
        call   sub_1_2811
        add    esp, 4
        pop    esi
        ret
sub_1_5C98:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 34h]
        or     eax, eax
        je     SHORT loc_1_5CB6
        push   eax
        call   sub_1_425C
        add    esp, 4
        mov    dword ptr [esi + 34h], 0
        jmp    SHORT loc_1_5CC1
loc_1_5CB6:
        push   dword ptr [esi + 30h]
        call   sub_1_15A8
        add    esp, 4
loc_1_5CC1:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_5CD4
        push   dword ptr [esp + 10h]
        call   sub_1_15D4
        add    esp, 4
loc_1_5CD4:
        pop    esi
        ret
sub_1_5CD6:
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 5ch]
        mov    dword ptr [esp + 54h], 0
        mov    ebx, dword ptr [eax + 50h]
        mov    dword ptr [esp + 28h], 0
        mov    dword ptr [ebx + 34h], 0
        mov    eax, dword ptr [eax + 44h]
        mov    ecx, dword ptr [ebx + 24h]
        movzx  edx, word ptr [eax - 4]
        cmp    ecx, edx
        jbe    SHORT loc_1_5D0B
        jmp    SHORT loc_1_5D15
loc_1_5D0B:
        cmp    dword ptr [eax + ecx*4], edx
        mov    eax, 1
        ja     SHORT loc_1_5D17
loc_1_5D15:
        sub    eax, eax
loc_1_5D17:
        or     eax, eax
        jne    SHORT loc_1_5D27
        mov    dword ptr [ebx + 20h], 6
        jmp    NEAR PTR loc_1_6411
loc_1_5D27:
        mov    eax, dword ptr [esp + 5ch]
        mov    eax, dword ptr [eax + 44h]
        mov    eax, dword ptr [eax + ecx*4]
        mov    ax, word ptr [eax]
        mov    word ptr [esp + 16h], ax
        mov    eax, dword ptr [esp + 5ch]
        mov    eax, dword ptr [eax + 44h]
        mov    eax, dword ptr [eax + ecx*4]
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ecx + 491h], ax
        mov    ax, word ptr [ebx + 6ch]
        mov    word ptr [esp + 12h], ax
        mov    eax, dword ptr [ebx + 2ch]
        or     eax, eax
        mov    dword ptr [esp + 48h], eax
        je     NEAR PTR loc_1_6301
loc_1_5D6C:
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [ebx + 2ch], 0
        lea    ecx, [eax + 0bah]
        mov    dword ptr [esp + 4ch], ecx
        mov    esi, ecx
        add    esi, 24h
        mov    dword ptr [esp + 34h], esi
        mov    dword ptr [esp + 2ch], 1
        mov    al, byte ptr [ecx + 2]
        test   al, 80h
        jne    NEAR PTR loc_1_5E84
        or     al, 80h
        mov    byte ptr [ecx + 2], al
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 1ah]
        call   sub_1_2B15
        add    esp, 8
        cmp    word ptr [esp + 14h], 0
        jne    NEAR PTR loc_1_6320
        mov    edx, dword ptr [ebx + 28h]
        mov    edi, dword ptr [ebx + 54h]
        add    edi, edx
        cmp    edi, eax
        jbe    SHORT loc_1_5DD5
        mov    edi, eax
        cmp    eax, edx
        jbe    NEAR PTR loc_1_6340
loc_1_5DD5:
        mov    eax, edi
        sub    eax, edx
        push   eax
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [ebx + 40h], eax
        mov    eax, dword ptr [ebx + 28h]
        and    eax, 0fffffffch
        sub    edi, eax
        add    edi, 8
        push   edi
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [ebx + 44h], eax
        mov    dword ptr [esp + 30h], 1
        mov    dword ptr [esi], 0
        or     edi, edi
        je     SHORT loc_1_5E6A
        mov    ebp, 0ffffh
loc_1_5E13:
        cmp    edi, ebp
        jbe    SHORT loc_1_5E53
        mov    ecx, dword ptr [esp + 34h]
        sub    edi, 0ffffh
        inc    dword ptr [esp + 30h]
        mov    word ptr [ecx + 4], bp
        add    dword ptr [esp + 34h], 6
        mov    ecx, dword ptr [esp + 34h]
        push   dword ptr [ecx - 6]
        call   sub_1_3A68
        add    esp, 4
        add    eax, 0ffffh
        push   eax
        call   sub_1_3A68
        mov    ecx, dword ptr [esp + 38h]
        add    esp, 4
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_5E66
loc_1_5E53:
        push   edi
        call   sub_1_3A60
        mov    ecx, dword ptr [esp + 38h]
        add    esp, 4
        mov    word ptr [ecx + 4], ax
        sub    edi, edi
loc_1_5E66:
        or     edi, edi
        jne    SHORT loc_1_5E13
loc_1_5E6A:
        mov    dword ptr [esp + 34h], esi
        push   dword ptr [esp + 30h]
        call   sub_1_3A60
        mov    ecx, dword ptr [esp + 50h]
        add    esp, 4
        mov    word ptr [ecx + 22h], ax
        jmp    SHORT loc_1_5E9C
loc_1_5E84:
        mov    ecx, dword ptr [esp + 4ch]
        mov    ax, word ptr [ecx + 22h]
        push   eax
        call   sub_1_3A60
        add    esp, 4
        movzx  eax, ax
        mov    dword ptr [esp + 30h], eax
loc_1_5E9C:
        mov    eax, dword ptr [esp + 48h]
        push   dword ptr [eax + 0cah]
        call   sub_1_3A68
        add    esp, 4
        mov    ecx, 0ah
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 3ch], eax
        cmp    eax, 6eh
        jae    SHORT loc_1_5ED8
        cmp    dword ptr [esp + 54h], 0
        jne    SHORT loc_1_5ED8
        mov    dword ptr [esp + 54h], 1
        push   eax
        call   sub_1_15BC
        add    esp, 4
loc_1_5ED8:
        push   0
        lea    eax, [ebx + 30h]
        push   eax
        call   sub_1_156C
        add    esp, 8
        mov    dword ptr [ebx + 34h], eax
        cmp    dword ptr [esp + 30h], 1
        jb     NEAR PTR loc_1_62D3
loc_1_5EF4:
        mov    ecx, dword ptr [esp + 34h]
        push   dword ptr [ecx]
        call   sub_1_3A68
        mov    dword ptr [esp + 44h], eax
        mov    edi, dword ptr [esp + 38h]
        add    esp, 4
        mov    ax, word ptr [edi + 4]
        add    edi, 4
        push   eax
        call   sub_1_3A60
        add    esp, 4
        movzx  esi, ax
        or     esi, esi
        jne    SHORT loc_1_5F26
        jmp    NEAR PTR loc_1_62BC
loc_1_5F26:
        mov    eax, dword ptr [ebx + 68h]
        cmp    eax, esi
        jae    SHORT loc_1_5F69
        mov    cx, si
        mov    esi, eax
        sub    cx, si
        push   ecx
        call   sub_1_3A60
        add    esp, 4
        mov    word ptr [edi], ax
        mov    eax, dword ptr [esp + 40h]
        add    eax, esi
        push   eax
        call   sub_1_3A68
        mov    ecx, dword ptr [esp + 38h]
        add    esp, 4
        mov    dword ptr [ecx], eax
        dec    dword ptr [esp + 2ch]
        sub    dword ptr [esp + 34h], 6
        mov    dword ptr [esp + 50h], 1
        jmp    SHORT loc_1_5F8B
loc_1_5F69:
        mov    word ptr [edi], 0
        mov    eax, dword ptr [esp + 40h]
        add    eax, esi
        push   eax
        call   sub_1_3A68
        mov    ecx, dword ptr [esp + 38h]
        add    esp, 4
        mov    dword ptr [ecx], eax
        mov    dword ptr [esp + 50h], 0
loc_1_5F8B:
        push   dword ptr [ebx + 44h]
        call   sub_1_3A68
        mov    ecx, dword ptr [esp + 44h]
        add    esp, 4
        add    ecx, esi
        cmp    eax, ecx
        jb     NEAR PTR loc_1_6356
        lea    eax, [esi + 1003h]
        and    eax, 0fffff000h
        mov    dword ptr [esp + 44h], eax
        push   eax
        db  068h, 030h, 0B9h, 000h, 000h
        call   sub_1_14C0
        mov    eax, dword ptr [esp + 4ch]
        add    esp, 8
        shr    eax, 0ch
        push   eax
        call   sub_1_1610
        mov    dword ptr [esp + 28h], eax
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_638D
        mov    ecx, eax
        mov    dword ptr [ecx], 1
        mov    ecx, eax
        add    ecx, 4
        mov    dword ptr [esp + 38h], ecx
        cmp    dword ptr [esp + 40h], 0ch
        lea    eax, [esp + 14h]
        push   eax
        jae    SHORT loc_1_6061
        sub    si, 8
        push   esi
        mov    eax, dword ptr [ebx + 28h]
        and    eax, 0fffffffch
        push   eax
        mov    eax, dword ptr [esp + 44h]
        add    eax, 8
        push   eax
        push   dword ptr [esp + 26h]
        call   sub_1_2B3E
        add    esp, 14h
        add    eax, 8
        mov    dword ptr [esp + 20h], eax
        cmp    word ptr [esp + 14h], 0
        jne    NEAR PTR loc_1_63C0
        mov    ecx, dword ptr [esp + 4ch]
        test   byte ptr [ecx + 2], 80h
        je     SHORT loc_1_6048
        mov    ecx, dword ptr [esp + 38h]
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 40h]
        mov    dword ptr [ecx + 4], eax
        jmp    SHORT loc_1_6090
loc_1_6048:
        mov    ecx, dword ptr [esp + 38h]
        mov    dword ptr [ecx], 0
        mov    dword ptr [ebx + 20h], 0
        mov    eax, dword ptr [ebx + 40h]
        mov    dword ptr [ecx + 4], eax
        jmp    SHORT loc_1_6090
loc_1_6061:
        push   esi
        mov    eax, dword ptr [ebx + 28h]
        and    eax, 0fffffffch
        add    eax, dword ptr [esp + 48h]
        sub    eax, 8
        push   eax
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 26h]
        call   sub_1_2B3E
        mov    dword ptr [esp + 34h], eax
        add    esp, 14h
        cmp    word ptr [esp + 14h], 0
        jne    NEAR PTR loc_1_644A
loc_1_6090:
        mov    dword ptr [esp + 18h], 0
loc_1_6098:
        call   sub_1_5B2E
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_64A9
        lea    ecx, [ebp + 9ch]
        mov    dword ptr [esp + 1ch], ecx
        mov    word ptr [ebp + 22h], 2
        mov    dword ptr [ebp + 24h], ecx
        mov    word ptr [ebp + 28h], 42h
        mov    eax, dword ptr [esp + 38h]
        add    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebp + 2ah], eax
        mov    ax, word ptr [esp + 12h]
        mov    word ptr [ebp + 2eh], ax
        movzx  eax, ax
        add    eax, dword ptr [esp + 18h]
        cmp    eax, dword ptr [esp + 20h]
        jbe    SHORT loc_1_60EE
        mov    eax, dword ptr [esp + 20h]
        sub    ax, word ptr [esp + 18h]
        mov    word ptr [ebp + 2eh], ax
loc_1_60EE:
        mov    ax, word ptr [ebp + 28h]
        mov    ecx, dword ptr [esp + 1ch]
        add    ax, word ptr [ebp + 2eh]
        mov    word ptr [ecx + 2], ax
        mov    word ptr [ebp + 0bah], 7777h
        mov    eax, dword ptr [esp + 2ch]
        cmp    eax, dword ptr [esp + 30h]
        jne    SHORT loc_1_612C
        movzx  eax, word ptr [ebp + 2eh]
        add    eax, dword ptr [esp + 18h]
        cmp    eax, dword ptr [esp + 20h]
        jne    SHORT loc_1_612C
        cmp    dword ptr [ebx + 2ch], 0
        mov    byte ptr [ebp + 0bch], 10h
        je     SHORT loc_1_6133
loc_1_612C:
        mov    byte ptr [ebp + 0bch], 0
loc_1_6133:
        mov    byte ptr [ebp + 0bdh], 2
        mov    eax, dword ptr [ebx + 58h]
        mov    dword ptr [ebp + 0beh], eax
        mov    eax, dword ptr [ebx + 60h]
        mov    dword ptr [ebp + 0c2h], eax
        mov    eax, dword ptr [ebx + 1ch]
        push   eax
        lea    ecx, [eax + 1]
        mov    dword ptr [ebx + 1ch], ecx
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [ebp + 0c6h], eax
        mov    eax, dword ptr [esp + 48h]
        mov    eax, dword ptr [eax + 0cah]
        mov    dword ptr [ebp + 0cah], eax
        mov    ax, word ptr [ebx + 1ah]
        push   eax
        call   sub_1_3A60
        add    esp, 4
        mov    word ptr [ebp + 0ceh], ax
        mov    ax, word ptr [ebx + 1ah]
        inc    ax
        push   eax
        call   sub_1_3A60
        add    esp, 4
        mov    word ptr [ebp + 0d0h], ax
        mov    eax, dword ptr [ebx + 44h]
        mov    dword ptr [ebp + 0d2h], eax
        mov    eax, dword ptr [esp + 40h]
        add    eax, dword ptr [esp + 18h]
        push   eax
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [ebp + 0d6h], eax
        mov    ecx, dword ptr [esp + 1ch]
        mov    ax, word ptr [ecx + 2]
        sub    ax, 42h
        push   eax
        call   sub_1_3A60
        add    esp, 4
        mov    word ptr [ebp + 0dah], ax
        mov    word ptr [ebp + 0dch], 0
        mov    esi, dword ptr [esp + 5ch]
        add    esi, 28h
        mov    edi, dword ptr [esp + 1ch]
        add    edi, 6
        movsd
        movsd
        movsd
        lea    esi, [ebx + 72h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    ax, word ptr [ebx + 70h]
        mov    word ptr [ecx + 10h], ax
        mov    byte ptr [ecx + 5], 11h
        lea    edi, [ebp + 1ch]
        movsd
        movsw
        db  0C7h, 045h, 004h, 04Fh, 05Ch, 000h, 000h
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ebp + 90h], ecx
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [ebp + 94h], eax
        inc    dword ptr [ecx]
        movzx  eax, word ptr [ebp + 2eh]
        add    dword ptr [esp + 18h], eax
        call   sub_1_93B1
        push   dword ptr [ebx + 30h]
        call   sub_1_15A8
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 4
        cmp    dword ptr [esp + 20h], eax
        jne    SHORT loc_1_6256
        mov    eax, dword ptr [esp + 2ch]
        cmp    eax, dword ptr [esp + 30h]
        jae    SHORT loc_1_6280
loc_1_6256:
        mov    eax, dword ptr [esp + 28h]
        add    eax, dword ptr [esp + 3ch]
        mov    dword ptr [esp + 28h], eax
        cmp    eax, 3e8h
        jae    NEAR PTR loc_1_6507
        push   dword ptr [esp + 3ch]
        lea    eax, [ebx + 30h]
        push   eax
        call   sub_1_156C
        add    esp, 8
        mov    dword ptr [ebx + 34h], eax
loc_1_6280:
        push   ebp
        call   sub_1_318D
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 4
        cmp    dword ptr [esp + 20h], eax
        ja     NEAR PTR loc_1_6098
        mov    ecx, dword ptr [esp + 24h]
        mov    eax, dword ptr [ecx]
        dec    eax
        mov    dword ptr [ecx], eax
        jne    SHORT loc_1_62BC
        push   ecx
        call   sub_1_1632
        add    esp, 4
        push   dword ptr [esp + 44h]
        db  068h, 030h, 0B9h, 000h, 000h
        call   sub_1_151C
        add    esp, 8
loc_1_62BC:
        inc    dword ptr [esp + 2ch]
        add    dword ptr [esp + 34h], 6
        mov    eax, dword ptr [esp + 2ch]
        cmp    eax, dword ptr [esp + 30h]
        jbe    NEAR PTR loc_1_5EF4
loc_1_62D3:
        push   dword ptr [esp + 48h]
        call   sub_1_2811
        add    esp, 4
        cmp    dword ptr [esp + 54h], 0
        je     SHORT loc_1_62F2
        push   dword ptr [esp + 3ch]
        call   sub_1_15D4
        add    esp, 4
loc_1_62F2:
        mov    eax, dword ptr [ebx + 2ch]
        mov    dword ptr [esp + 48h], eax
        or     eax, eax
        jne    NEAR PTR loc_1_5D6C
loc_1_6301:
        mov    eax, dword ptr [esp + 5ch]
        and    word ptr [eax + 1ah], 0fff7h
        mov    al, byte ptr [ebx + 18h]
        and    al, 0fbh
        mov    byte ptr [ebx + 18h], al
        and    al, 0feh
        mov    byte ptr [ebx + 18h], al
loc_1_6318:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
loc_1_6320:
        movzx  eax, word ptr [esp + 14h]
        mov    dword ptr [ebx + 20h], eax
        mov    dword ptr [ebx + 40h], 0
        push   dword ptr [esp + 48h]
        call   sub_1_2811
        add    esp, 4
        jmp    NEAR PTR loc_1_6411
loc_1_6340:
        mov    dword ptr [ebx + 20h], 0
        mov    edx, dword ptr [esp + 48h]
        mov    ecx, dword ptr [esp + 5ch]
        call   sub_1_5BC5
        jmp    SHORT loc_1_6301
loc_1_6356:
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 58h]
        push   ebx
        call   sub_1_5C98
        mov    eax, dword ptr [esp + 68h]
        add    esp, 0ch
        and    word ptr [eax + 1ah], 0fff7h
        mov    al, byte ptr [ebx + 18h]
        and    al, 0fbh
        mov    byte ptr [ebx + 18h], al
        and    al, 0feh
        mov    byte ptr [ebx + 18h], al
        mov    edx, dword ptr [esp + 48h]
        mov    ecx, dword ptr [esp + 5ch]
        call   sub_1_5B64
        jmp    SHORT loc_1_6318
loc_1_638D:
        push   dword ptr [esp + 44h]
        db  068h, 030h, 0B9h, 000h, 000h
        call   sub_1_151C
        add    esp, 8
        push   dword ptr [esp + 48h]
        call   sub_1_2811
        add    esp, 4
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 58h]
        push   ebx
        call   sub_1_5C98
        add    esp, 0ch
        jmp    NEAR PTR loc_1_6301
loc_1_63C0:
        movzx  eax, word ptr [esp + 14h]
        mov    dword ptr [ebx + 20h], eax
        mov    dword ptr [ebx + 40h], 0
        push   dword ptr [esp + 48h]
        call   sub_1_2811
        mov    ecx, dword ptr [esp + 28h]
        add    esp, 4
        mov    eax, dword ptr [ecx]
        dec    eax
        mov    dword ptr [ecx], eax
        jne    SHORT loc_1_6400
        push   ecx
        call   sub_1_1632
        add    esp, 4
        push   dword ptr [esp + 44h]
        db  068h, 030h, 0B9h, 000h, 000h
        call   sub_1_151C
        add    esp, 8
loc_1_6400:
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 58h]
        push   ebx
        call   sub_1_5C98
        add    esp, 0ch
loc_1_6411:
        mov    edx, dword ptr [ebx + 2ch]
        or     edx, edx
        je     SHORT loc_1_642D
        mov    dword ptr [ebx + 2ch], 0
        mov    ecx, dword ptr [esp + 5ch]
        call   sub_1_5BC5
        jmp    NEAR PTR loc_1_6301
loc_1_642D:
        call   sub_1_5B2E
        or     eax, eax
        je     NEAR PTR loc_1_6301
        mov    edx, eax
        mov    ecx, dword ptr [esp + 5ch]
        call   sub_1_5BC5
        jmp    NEAR PTR loc_1_6301
loc_1_644A:
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 58h]
        push   ebx
        call   sub_1_5C98
        mov    eax, dword ptr [esp + 68h]
        add    esp, 0ch
        and    word ptr [eax + 1ah], 0fff7h
        mov    al, byte ptr [ebx + 18h]
        and    al, 0fbh
        mov    byte ptr [ebx + 18h], al
        and    al, 0feh
        mov    ecx, dword ptr [esp + 24h]
        mov    byte ptr [ebx + 18h], al
        mov    eax, dword ptr [ecx]
        dec    eax
        mov    dword ptr [ecx], eax
        jne    SHORT loc_1_6497
        push   ecx
        call   sub_1_1632
        add    esp, 4
        push   dword ptr [esp + 44h]
        db  068h, 030h, 0B9h, 000h, 000h
        call   sub_1_151C
        add    esp, 8
loc_1_6497:
        mov    edx, dword ptr [esp + 48h]
        mov    ecx, dword ptr [esp + 5ch]
        call   sub_1_5B64
        jmp    NEAR PTR loc_1_6318
loc_1_64A9:
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 58h]
        push   ebx
        call   sub_1_5C98
        mov    eax, dword ptr [esp + 68h]
        add    esp, 0ch
        and    word ptr [eax + 1ah], 0fff7h
        mov    al, byte ptr [ebx + 18h]
        and    al, 0fbh
        mov    byte ptr [ebx + 18h], al
        and    al, 0feh
        mov    ecx, dword ptr [esp + 24h]
        mov    byte ptr [ebx + 18h], al
        mov    eax, dword ptr [ecx]
        dec    eax
        mov    dword ptr [ecx], eax
        jne    SHORT loc_1_64F6
        push   ecx
        call   sub_1_1632
        add    esp, 4
        push   dword ptr [esp + 44h]
        db  068h, 030h, 0B9h, 000h, 000h
        call   sub_1_151C
        add    esp, 8
loc_1_64F6:
        push   dword ptr [esp + 48h]
        call   sub_1_2811
        add    esp, 4
        jmp    NEAR PTR loc_1_6318
loc_1_6507:
        push   ebp
        call   sub_1_318D
        mov    ecx, dword ptr [esp + 28h]
        add    esp, 4
        mov    eax, dword ptr [ecx]
        dec    eax
        mov    dword ptr [ecx], eax
        jne    SHORT loc_1_6535
        push   ecx
        call   sub_1_1632
        add    esp, 4
        push   dword ptr [esp + 44h]
        db  068h, 030h, 0B9h, 000h, 000h
        call   sub_1_151C
        add    esp, 8
loc_1_6535:
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [ebx + 2ch], eax
        cmp    dword ptr [esp + 50h], 0
        je     SHORT loc_1_6548
        add    dword ptr [esp + 34h], 6
loc_1_6548:
        mov    ecx, dword ptr [esp + 34h]
        push   dword ptr [ecx]
        call   sub_1_3A68
        sub    eax, dword ptr [esp + 24h]
        add    eax, dword ptr [esp + 1ch]
        add    esp, 4
        push   eax
        call   sub_1_3A68
        mov    ecx, dword ptr [esp + 38h]
        add    esp, 4
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [ecx + 4]
        push   eax
        call   sub_1_3A60
        sub    ax, word ptr [esp + 1ch]
        add    ax, word ptr [esp + 24h]
        add    esp, 4
        push   eax
        call   sub_1_3A60
        mov    ecx, dword ptr [esp + 38h]
        add    esp, 4
        mov    word ptr [ecx + 4], ax
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 60h]
        call   sub_1_15EC
        add    esp, 8
        mov    dword ptr [ebx + 38h], eax
        cmp    dword ptr [esp + 54h], 0
        je     NEAR PTR loc_1_6318
        push   dword ptr [esp + 3ch]
        call   sub_1_15D4
        add    esp, 4
        jmp    NEAR PTR loc_1_6318
sub_1_65C4:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        add    dword ptr [esi + 5ch], 10000h
        cmp    dword ptr [esi], esi
        je     SHORT loc_1_65EC
loc_1_65D6:
        mov    edi, dword ptr [esi]
        push   edi
        call   sub_1_0060
        add    esp, 4
        mov    ecx, edi
        call   sub_1_4E58
        cmp    dword ptr [esi], esi
        jne    SHORT loc_1_65D6
loc_1_65EC:
        mov    eax, dword ptr [esi + 8]
        lea    edi, [esi + 8]
        sub    eax, esi
        cmp    eax, 8
        je     SHORT loc_1_6624
        mov    ebx, dword ptr [esi + 8]
        push   edi
        call   sub_1_0060
        add    esp, 4
        mov    dword ptr [edi], edi
        mov    dword ptr [esi + 0ch], edi
        inc    word ptr [ebx + 8]
        mov    ecx, ebx
loc_1_6610:
        mov    edi, dword ptr [ecx]
        call   sub_1_5172
        mov    ecx, edi
        cmp    ebx, edi
        jne    SHORT loc_1_6610
        mov    ecx, ebx
        call   sub_1_5172
loc_1_6624:
        mov    eax, dword ptr [esi + 10h]
        sub    eax, esi
        cmp    eax, 10h
        je     SHORT loc_1_6656
        sub    ebx, ebx
loc_1_6630:
        mov    edi, dword ptr [esi + 10h]
        push   edi
        mov    word ptr [edi + 0ah], bx
        call   sub_1_0060
        add    esp, 4
        mov    dword ptr [edi], edi
        mov    dword ptr [edi + 4], edi
        mov    ecx, edi
        call   sub_1_5172
        mov    eax, dword ptr [esi + 10h]
        sub    eax, esi
        cmp    eax, 10h
        jne    SHORT loc_1_6630
loc_1_6656:
        mov    eax, dword ptr [esi + 38h]
        or     eax, eax
        je     SHORT loc_1_666D
        push   eax
        call   sub_1_425C
        add    esp, 4
        mov    dword ptr [esi + 38h], 0
loc_1_666D:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_6671:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esi + 50h]
        mov    ebx, dword ptr [edi + 24h]
        call   sub_1_5B2E
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esi + 44h]
        movzx  ecx, word ptr [eax - 4]
        cmp    ebx, ecx
        jbe    SHORT loc_1_6698
        jmp    SHORT loc_1_66A2
loc_1_6698:
        cmp    dword ptr [eax + ebx*4], ecx
        mov    eax, 1
        ja     SHORT loc_1_66A4
loc_1_66A2:
        sub    eax, eax
loc_1_66A4:
        or     eax, eax
        jne    SHORT loc_1_66B4
        mov    dword ptr [edi + 20h], 88h
        jmp    NEAR PTR loc_1_6750
loc_1_66B4:
        mov    eax, dword ptr [esi + 44h]
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax + ebx*4]
        mov    ax, word ptr [eax]
        mov    word ptr [esp + 12h], ax
        mov    eax, dword ptr [esi + 44h]
        mov    eax, dword ptr [eax + ebx*4]
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ecx + 491h], ax
loc_1_66D9:
        mov    eax, dword ptr [edi + 10h]
        sub    eax, edi
        cmp    eax, 10h
        je     SHORT loc_1_6759
        mov    ebx, dword ptr [edi + 10h]
        push   ebx
        call   sub_1_0060
        add    esp, 4
        mov    dword ptr [ebx], ebx
        mov    dword ptr [ebx + 4], ebx
        mov    eax, dword ptr [ebx + 14h]
        mov    ecx, 1000h
        mov    edx, dword ptr [ebx + 0ch]
        cmp    dword ptr [edi + 48h], edx
        jne    SHORT loc_1_6718
        mov    ebp, dword ptr [edi + 50h]
        sub    ecx, ebp
        add    eax, ebp
        cmp    dword ptr [edi + 4ch], edx
        jne    SHORT loc_1_6714
        add    edx, ebp
        jmp    SHORT loc_1_671D
loc_1_6714:
        add    edx, ebp
        jmp    SHORT loc_1_6725
loc_1_6718:
        cmp    dword ptr [edi + 4ch], edx
        jne    SHORT loc_1_6725
loc_1_671D:
        mov    ecx, dword ptr [edi + 28h]
        add    ecx, dword ptr [edi + 54h]
        sub    ecx, edx
loc_1_6725:
        lea    ebp, [esp + 10h]
        push   ebp
        push   ecx
        push   edx
        push   eax
        push   dword ptr [esp + 22h]
        call   sub_1_2B78
        add    esp, 14h
        mov    ecx, ebx
        call   sub_1_5172
        cmp    word ptr [esp + 10h], 0
        je     SHORT loc_1_66D9
        movzx  eax, word ptr [esp + 10h]
        mov    dword ptr [edi + 20h], eax
loc_1_6750:
        push   edi
        call   sub_1_65C4
        add    esp, 4
loc_1_6759:
        and    word ptr [esi + 1ah], 0fff7h
        mov    cl, byte ptr [edi + 18h]
        and    cl, 0fbh
        mov    byte ptr [edi + 18h], cl
        cmp    dword ptr [edi], edi
        jne    SHORT loc_1_6790
        mov    eax, dword ptr [edi + 8]
        sub    eax, edi
        cmp    eax, 8
        jne    SHORT loc_1_6790
        and    cl, 0feh
        mov    byte ptr [edi + 18h], cl
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_67A3
        mov    edx, esi
        mov    ecx, dword ptr [esp + 14h]
        call   sub_1_56DD
        jmp    SHORT loc_1_67A3
loc_1_6790:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_67A3
        push   dword ptr [esp + 14h]
        call   sub_1_2811
        add    esp, 4
loc_1_67A3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_67AB:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 50h]
        or     ecx, ecx
        je     SHORT loc_1_67D1
        test   byte ptr [ecx + 18h], 2
        push   eax
        je     SHORT loc_1_67C7
        call   sub_1_6671
        add    esp, 4
        jmp    SHORT loc_1_67D7
loc_1_67C7:
        call   sub_1_5CD6
        add    esp, 4
        jmp    SHORT loc_1_67D7
loc_1_67D1:
        and    word ptr [eax + 1ah], 0fff7h
loc_1_67D7:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_67E0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 1ch]
        cmp    esi, ebx
        je     SHORT loc_1_67F3
        movzx  ebx, byte ptr [esi]
loc_1_67F3:
        push   200h
        lea    eax, [ebx + 9]
        push   eax
        call   sub_1_4364
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_6870
        mov    ax, word ptr [esp + 18h]
        mov    word ptr [ebp], ax
        db  0A1h, 094h, 0B8h, 000h, 000h
        or     ebx, ebx
        mov    ax, word ptr [eax + 491h]
        mov    word ptr [ebp + 2], ax
        mov    byte ptr [ebp + 8], bl
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebp + 4], eax
        je     SHORT loc_1_6843
        inc    esi
        lea    edi, [ebp + 9]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_1_6843:
        mov    ecx, dword ptr [esp + 14h]
        add    ecx, 44h
        mov    edx, ebp
        call   sub_1_300B
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_6867
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_1_6870
        mov    eax, dword ptr [esp + 20h]
        inc    dword ptr [eax + 4]
        jmp    SHORT loc_1_6870
loc_1_6867:
        push   ebp
        call   sub_1_4390
        add    esp, 4
loc_1_6870:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_6877:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [edi + 44h]
        mov    ebx, dword ptr [eax + esi*4]
        mov    eax, dword ptr [edi + 48h]
        or     eax, eax
        je     SHORT loc_1_68A3
loc_1_688F:
        movzx  ecx, word ptr [eax + 0ch]
        cmp    ecx, esi
        jne    SHORT loc_1_689D
        mov    word ptr [eax + 0ch], 0ffffh
loc_1_689D:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_688F
loc_1_68A3:
        or     ebx, ebx
        je     SHORT loc_1_68C0
        mov    eax, dword ptr [ebx + 4]
        or     eax, eax
        je     SHORT loc_1_68B7
        push   eax
        db  0E8h, 08Ch, 0CAh, 0FEh, 0FFh
        add    esp, 4
loc_1_68B7:
        push   ebx
        call   sub_1_4390
        add    esp, 4
loc_1_68C0:
        mov    eax, dword ptr [edi + 44h]
        pop    edi
        movzx  ecx, word ptr [eax + 2]
        mov    dword ptr [eax + esi*4], ecx
        mov    word ptr [eax + 2], si
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_68E0:
        push   0
        call   sub_1_92D8
        add    esp, 4
        db  0A3h, 0B8h, 0B3h, 000h, 000h
        or     eax, eax
        mov    eax, 1
        jne    SHORT loc_1_68FA
        sub    eax, eax
loc_1_68FA:
        ret
sub_1_68FB:
        db  0FFh, 035h, 0B8h, 0B3h, 000h, 000h
        call   sub_1_92E7
        add    esp, 4
        db  0C7h, 005h, 0B8h, 0B3h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_1_6914:
        push   esi
        db  08Bh, 035h, 0C0h, 0B3h, 000h, 000h
        db  081h, 03Dh, 0C4h, 0B3h, 000h, 000h, 0C0h, 0B3h, 000h, 000h
        mov    eax, 0
        je     SHORT loc_1_693E
        push   esi
        call   sub_1_0060
        add    esp, 4
        mov    dword ptr [esi], 0
        lea    eax, [esi - 38h]
loc_1_693E:
        pop    esi
        ret
sub_1_6940:
        db  083h, 03Dh, 0B4h, 0B3h, 000h, 000h, 000h
        jne    SHORT loc_1_6960
        db  0C7h, 005h, 0B4h, 0B3h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 0ACh, 0B3h, 000h, 000h
        call   sub_1_6D70
        add    esp, 4
loc_1_6960:
        ret
loc_1_6961:
        db  0FFh, 035h, 064h, 0B8h, 000h, 000h
        call   sub_1_92FB
        add    esp, 4
        db  0C7h, 005h, 0B4h, 0B3h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 058h, 0B8h, 000h, 000h
        db  0A1h, 04Ch, 0B8h, 000h, 000h
        cmp    ecx, eax
        jae    SHORT loc_1_699D
        inc    ecx
        cmp    ecx, eax
        jae    SHORT loc_1_6992
        call   sub_1_6940
loc_1_6992:
        db  0FEh, 00Dh, 0D1h, 0B1h, 000h, 000h
        call   sub_1_8C70
loc_1_699D:
        db  0FFh, 035h, 064h, 0B8h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
        sub    eax, eax
        ret
loc_1_69AE:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
loc_1_69B5:
        call   sub_1_93B1
        db  0FFh, 035h, 0B8h, 0B3h, 000h, 000h
        call   sub_1_930A
        add    esp, 4
        db  00Fh, 0B6h, 005h, 0D1h, 0B1h, 000h, 000h
        db  039h, 005h, 05Ch, 0B8h, 000h, 000h
        jae    SHORT loc_1_69DC
        db  0A3h, 05Ch, 0B8h, 000h, 000h
loc_1_69DC:
        db  08Bh, 00Dh, 04Ch, 0B8h, 000h, 000h
        db  039h, 00Dh, 050h, 0B8h, 000h, 000h
        jbe    SHORT loc_1_6A48
        cmp    ecx, eax
        jb     SHORT loc_1_69F8
        mov    eax, ecx
        db  039h, 005h, 054h, 0B8h, 000h, 000h
        jbe    SHORT loc_1_6A48
loc_1_69F8:
        db  0FFh, 035h, 064h, 0B8h, 000h, 000h
        call   sub_1_92FB
        add    esp, 4
        db  0A1h, 04Ch, 0B8h, 000h, 000h
        db  039h, 005h, 050h, 0B8h, 000h, 000h
        jbe    SHORT loc_1_6A3A
        db  00Fh, 0B6h, 005h, 0D1h, 0B1h, 000h, 000h
        db  039h, 005h, 04Ch, 0B8h, 000h, 000h
        jb     SHORT loc_1_6A30
        db  08Bh, 00Dh, 04Ch, 0B8h, 000h, 000h
        db  039h, 00Dh, 054h, 0B8h, 000h, 000h
        jbe    SHORT loc_1_6A3A
loc_1_6A30:
        db  0A3h, 058h, 0B8h, 000h, 000h
        call   sub_1_8C8E
loc_1_6A3A:
        db  0FFh, 035h, 064h, 0B8h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
loc_1_6A48:
        db  083h, 03Dh, 0C8h, 0B3h, 000h, 000h, 000h
        je     SHORT loc_1_6A5B
        call   sub_1_6DF4
        jmp    NEAR PTR loc_1_6D42
loc_1_6A5B:
        db  068h, 058h, 0B4h, 000h, 000h
        call   sub_1_943C
        add    esp, 4
        call   sub_1_6914
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_6D35
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    dword ptr [eax], ebx
        push   dword ptr [ebx + 10h]
        call   sub_1_92FB
        add    esp, 4
        test   byte ptr [ebx + 1ah], 8
        je     SHORT loc_1_6A9A
        push   ebx
        call   sub_1_67AB
        jmp    NEAR PTR loc_1_6D11
loc_1_6A9A:
        mov    eax, dword ptr [ebx + 34h]
        mov    dword ptr [esp + 14h], eax
        mov    ecx, dword ptr [eax + 24h]
        mov    word ptr [esp + 10h], 0
        db  0A1h, 094h, 0B8h, 000h, 000h
        lea    ebp, [ecx + 1eh]
        mov    byte ptr [eax + 493h], 0
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    byte ptr [eax + 494h], 0
        mov    ax, word ptr [ebx + 1ah]
        test   al, 4
        je     NEAR PTR loc_1_6B55
        cmp    dword ptr [ebx + 1ch], 0
        je     SHORT loc_1_6B01
        push   dword ptr [esp + 14h]
        call   sub_1_27A4
        add    esp, 4
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [ebx + 34h], 0
        cmp    byte ptr [eax + 8], 0
        jne    NEAR PTR loc_1_6D14
        jmp    NEAR PTR loc_1_6D08
loc_1_6B01:
        test   al, 1
        je     SHORT loc_1_6B37
        mov    esi, dword ptr [ebx + 4ch]
        lea    edi, [ebp + 6]
        mov    eax, dword ptr [esi]
        add    esi, 4
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 4ch]
        mov    eax, dword ptr [eax]
        sub    ax, 2
        mov    word ptr [esp + 10h], ax
        mov    word ptr [ebp], 3333h
        jmp    NEAR PTR loc_1_6CBE
loc_1_6B37:
        test   al, 2
        jne    SHORT loc_1_6B55
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [ebp + 6], 0ffh
        mov    dword ptr [eax + 30h], 0
        mov    word ptr [ebp], 3333h
        jmp    NEAR PTR loc_1_6CC9
loc_1_6B55:
        and    ax, 0fffch
        mov    word ptr [ebx + 1ah], ax
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    dword ptr [eax], ebx
        movzx  eax, word ptr [ebp]
        cmp    eax, 2222h
        je     SHORT loc_1_6B7F
        cmp    eax, 5555h
        je     NEAR PTR loc_1_6C48
        jmp    NEAR PTR loc_1_6C7C
loc_1_6B7F:
        db  08Bh, 015h, 094h, 0B8h, 000h, 000h
        movzx  ax, byte ptr [ebp + 4]
        mov    word ptr [edx + 491h], ax
        db  0A0h, 0A2h, 0B0h, 000h, 000h
        cmp    byte ptr [ebp + 6], al
        jne    NEAR PTR loc_1_6C24
        db  0A0h, 0A3h, 0B0h, 000h, 000h
        cmp    byte ptr [ebp + 9], al
        jne    SHORT loc_1_6C24
        db  066h, 0A1h, 0A0h, 0B0h, 000h, 000h
        cmp    word ptr [ecx + 10h], ax
        jne    SHORT loc_1_6C24
        lea    edi, [ebp + 0ah]
        lea    esi, [ebp + 8]
        mov    ax, word ptr [ebp + 7]
        push   eax
        call   sub_1_3A60
        add    esp, 4
        movzx  eax, ax
        cmp    eax, 7
        jb     SHORT loc_1_6C24
        mov    ecx, dword ptr [edi]
        sub    ecx, eax
        cmp    ecx, -1
        jne    SHORT loc_1_6C24
        movzx  eax, word ptr [edi + 4]
        db  03Bh, 005h, 040h, 0AFh, 000h, 000h
        jae    SHORT loc_1_6C24
        mov    dword ptr [esp + 18h], 0
        lea    eax, [esp + 18h]
        push   ebx
        push   eax
        lea    eax, [esi + 8]
        push   eax
        push   edi
        movzx  eax, word ptr [edi + 4]
        db  0FFh, 014h, 085h, 018h, 0AFh, 000h, 000h
        add    esp, 10h
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 18h]
        add    ax, 8
        mov    word ptr [esp + 10h], ax
        movzx  eax, ax
        mov    dword ptr [esi], eax
        mov    word ptr [esp + 12h], 0
        jmp    SHORT loc_1_6C3F
loc_1_6C24:
        lea    eax, [esp + 10h]
        push   ebx
        push   eax
        push   ebp
        push   ebp
        movzx  eax, byte ptr [ebp + 6]
        db  0FFh, 014h, 085h, 050h, 0A2h, 000h, 000h
        mov    word ptr [esp + 22h], ax
        add    esp, 10h
loc_1_6C3F:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 30h], ebx
        jmp    SHORT loc_1_6C76
loc_1_6C48:
        db  0A1h, 094h, 0B8h, 000h, 000h
        push   ebx
        mov    word ptr [eax + 491h], 0ffh
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax + 30h], 0
        lea    eax, [esp + 14h]
        push   eax
        push   ebp
        push   ebp
        call   sub_1_7A0E
        mov    word ptr [esp + 22h], ax
        add    esp, 10h
loc_1_6C76:
        mov    word ptr [ebp], 3333h
loc_1_6C7C:
        mov    al, byte ptr [esp + 12h]
        mov    byte ptr [ebp + 6], al
        test   byte ptr [ebx + 1ah], 2
        je     SHORT loc_1_6C8B
        jmp    SHORT loc_1_6CBE
loc_1_6C8B:
        cmp    word ptr [esp + 10h], 3eh
        ja     SHORT loc_1_6CC9
        movzx  eax, word ptr [esp + 10h]
        mov    ecx, dword ptr [ebx + 4ch]
        add    eax, 2
        mov    dword ptr [ecx], eax
        mov    edi, dword ptr [ebx + 4ch]
        lea    esi, [ebp + 6]
        mov    eax, dword ptr [edi]
        add    edi, 4
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        or     word ptr [ebx + 1ah], 1
loc_1_6CBE:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 30h], 0
loc_1_6CC9:
        cmp    word ptr [ebx + 60h], 0
        mov    byte ptr [ebp + 7], 40h
        jne    SHORT loc_1_6CD8
        mov    byte ptr [ebp + 7], 0
loc_1_6CD8:
        movzx  edx, word ptr [esp + 10h]
        add    edx, 26h
        mov    ecx, dword ptr [esp + 14h]
        call   sub_1_1964
        mov    eax, dword ptr [esp + 14h]
        db  0C7h, 040h, 004h, 050h, 019h, 000h, 000h
        cmp    dword ptr [eax + 30h], 0
        je     SHORT loc_1_6CFD
        mov    dword ptr [ebx + 1ch], eax
loc_1_6CFD:
        or     ebx, ebx
        je     SHORT loc_1_6D08
        mov    dword ptr [ebx + 34h], 0
loc_1_6D08:
        push   dword ptr [esp + 14h]
        call   sub_1_318D
loc_1_6D11:
        add    esp, 4
loc_1_6D14:
        mov    ax, word ptr [ebx + 1ah]
        test   al, 80h
        je     SHORT loc_1_6D2A
        and    ax, 0ff7fh
        mov    word ptr [ebx + 1ah], ax
        db  0FFh, 00Dh, 03Ch, 0B4h, 000h, 000h
loc_1_6D2A:
        push   dword ptr [ebx + 10h]
        call   sub_1_92F1
        add    esp, 4
loc_1_6D35:
        db  068h, 058h, 0B4h, 000h, 000h
        call   sub_1_9490
        add    esp, 4
loc_1_6D42:
        db  0FEh, 00Dh, 0D1h, 0B1h, 000h, 000h
        jmp    NEAR PTR loc_1_69B5
loc_1_6D4D:
        sub    ax, ax
        ret
loc_1_6D51:
        mov    ax, 0ffh
        ret
loc_1_6D56:
        mov    ax, 0fbh
        ret
loc_1_6D5B:
        mov    ax, 0bfh
        ret
loc_1_6D60:
        sub    ax, ax
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_6D70:
        db  083h, 03Dh, 0C8h, 0B3h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        je     SHORT loc_1_6D94
        mov    dword ptr [eax + 4], 0
        db  08Bh, 00Dh, 0CCh, 0B3h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A3h, 0CCh, 0B3h, 000h, 000h
        jmp    SHORT loc_1_6DA5
loc_1_6D94:
        db  0A3h, 0C8h, 0B3h, 000h, 000h
        db  0A3h, 0CCh, 0B3h, 000h, 000h
        mov    dword ptr [eax + 4], 0
loc_1_6DA5:
        db  0FEh, 005h, 0D1h, 0B1h, 000h, 000h
        db  0FFh, 035h, 0B8h, 0B3h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
        ret
sub_1_6DBA:
        db  0A1h, 0C8h, 0B3h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_6DF3
        db  039h, 005h, 0CCh, 0B3h, 000h, 000h
        je     SHORT loc_1_6DD8
        mov    ecx, eax
        mov    ecx, dword ptr [ecx + 4]
        db  089h, 00Dh, 0C8h, 0B3h, 000h, 000h
        jmp    SHORT loc_1_6DEC
loc_1_6DD8:
        db  0C7h, 005h, 0CCh, 0B3h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0C8h, 0B3h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_6DEC:
        mov    dword ptr [eax + 4], 0
loc_1_6DF3:
        ret
sub_1_6DF4:
        call   sub_1_6DBA
        or     eax, eax
        je     SHORT loc_1_6E05
        push   eax
        call   dword ptr [eax]
        add    esp, 4
        jmp    SHORT loc_1_6E07
loc_1_6E05:
        sub    eax, eax
loc_1_6E07:
        ret
sub_1_6E08:
        push   ebx
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    ebx, dword ptr [eax]
        mov    bx, word ptr [ebx + 24h]
        shl    ebx, 10h
        mov    bx, word ptr [eax + 491h]
        add    ebx, 90000000h
        lea    eax, [eax + 4]
        rol    ebx, 10h
        mov    dword ptr [eax + 1ch], ebx
        pop    ebx
        ret
        db  090h, 053h, 08Dh, 098h, 03Bh, 002h, 000h, 000h, 089h, 058h, 00Ch, 05Bh, 0C3h
sub_1_6E3C:
        push   ebx
        lea    ebx, [eax + 23bh]
        mov    dword ptr [eax + 14h], ebx
        pop    ebx
        ret
sub_1_6E48:
        mov    byte ptr [eax + 24h], 1
        db  08Bh, 01Dh, 094h, 0B8h, 000h, 000h
        cmp    byte ptr [ebx + 493h], 0
        je     SHORT loc_1_6E5F
        or     byte ptr [eax + 24h], 4
loc_1_6E5F:
        lea    ebx, [eax + 23bh]
        mov    dword ptr [eax + 20h], ebx
        push   eax
        db  08Bh, 01Dh, 098h, 0B1h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    esi, eax
        movzx  ecx, byte ptr [esi + 1]
        cmp    ecx, 0bdh
        je     SHORT loc_1_6EAE
        cmp    ecx, 0bch
        je     SHORT loc_1_6EAE
loc_1_6E8C:
        db  0CDh, 020h, 00Ah, 000h, 040h, 000h
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        jae    SHORT loc_1_6EA2
        mov    dword ptr [ebp + 1ch], 2
        stc
        jmp    SHORT loc_1_6EA6
loc_1_6EA2:
        mov    ah, byte ptr [ebp + 2ch]
        sahf
loc_1_6EA6:
        mov    ebx, dword ptr [ebp + 1ch]
        pop    eax
        mov    dword ptr [eax], ebx
        cld
        ret
loc_1_6EAE:
        mov    ecx, 5ch
        jmp    SHORT loc_1_6E8C
        db  02Eh, 08Bh, 0C0h
sub_1_6EB8:
        movzx  eax, word ptr [eax]
        ret
sub_1_6EBC:
        push   edx
        push   eax
        movzx  ecx, byte ptr [ebp]
        cmp    ecx, 207h
        jae    SHORT loc_1_6EF3
        mov    esi, ebp
        inc    esi
        cmp    ecx, 0ch
        jb     SHORT loc_1_6EEA
        mov    eax, edi
        neg    eax
        and    eax, 3
        xchg   ecx, eax
        sub    eax, ecx
        rep movsb
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
loc_1_6EEA:
        rep movsb
        mov    byte ptr [edi], 0
        clc
        pop    eax
loc_1_6EF1:
        pop    edx
        ret
loc_1_6EF3:
        pop    eax
        mov    word ptr [eax], 3
        stc
        jmp    SHORT loc_1_6EF1
        db  055h, 053h, 056h, 057h, 08Bh, 074h, 024h, 014h, 08Bh, 0FEh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 0BAh
        db  0FFh, 000h, 000h, 000h, 033h, 0C0h, 0E8h, 065h, 000h, 000h, 000h, 073h, 002h, 033h, 0C0h, 05Fh
        db  05Eh, 05Bh, 05Dh, 0C3h, 083h, 0F9h, 0FFh, 075h, 00Dh, 057h, 050h, 08Bh, 0FEh, 02Bh, 0C0h, 0F2h
        db  0AEh, 0F7h, 0D1h, 049h, 058h, 05Fh, 0E3h, 03Ch, 0A8h, 080h, 074h, 023h, 08Dh, 004h, 009h, 03Bh
        db  0D0h, 072h, 034h, 052h, 06Ah, 001h, 051h, 056h, 057h, 0CDh, 020h, 042h, 000h, 040h, 000h, 083h
        db  0C4h, 010h, 05Ah, 04Ah, 03Bh, 0D0h, 076h, 006h, 066h, 0C7h, 004h, 038h, 000h, 000h, 0C3h
loc_1_6F5B:
        cmp    edx, ecx
        jb     SHORT loc_1_6F73
        mov    edx, ecx
        mov    eax, ecx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        ret
loc_1_6F70:
        sub    eax, eax
        ret
loc_1_6F73:
        mov    eax, 7ah
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_6F7C:
        cmp    ecx, -1
        jne    SHORT loc_1_6F92
        push   edi
        push   eax
        mov    edi, esi
        sub    eax, eax
        repne scasw
        not    ecx
        lea    ecx, [ecx + ecx - 2]
        pop    eax
        pop    edi
loc_1_6F92:
        jecxz  loc_1_6F70
        test   eax, 80h
        jne    SHORT loc_1_6F5B
        cmp    edx, ecx
        jb     SHORT loc_1_6F73
        push   edx
        push   1
        push   edx
        push   ecx
        push   esi
        push   edi
        db  0CDh, 020h, 040h, 000h, 040h, 000h
        add    esp, 14h
        pop    edx
        cmp    edx, eax
        jbe    SHORT loc_1_6FB8
        mov    byte ptr [eax + edi], 0
loc_1_6FB8:
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_6FBC:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  ebx, word ptr [esp + 1ch]
        mov    bh, bl
        and    bx, 1016h
        mov    ebp, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        call   sub_1_7064
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        jae    SHORT loc_1_6FE1
        xor    eax, eax
loc_1_6FE1:
        ret
        db  08Bh, 0C0h
sub_1_6FE4:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  ebx, word ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        call   sub_1_7064
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        jae    SHORT loc_1_7002
        xor    eax, eax
loc_1_7002:
        ret
        db  090h
sub_1_7004:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  ebx, word ptr [esp + 1ch]
        test   bh, 80h
        je     SHORT loc_1_7018
        mov    bx, 16h
        jmp    SHORT loc_1_701F
loc_1_7018:
        mov    bh, bl
        and    bx, 1016h
loc_1_701F:
        mov    ebp, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        call   sub_1_7064
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        jae    SHORT loc_1_7034
        xor    eax, eax
loc_1_7034:
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_7038:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  ebx, word ptr [esp + 1ch]
        test   bh, 80h
        jne    SHORT loc_1_705D
loc_1_7046:
        mov    ebp, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        call   sub_1_7064
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        jb     SHORT loc_1_705A
        ret
loc_1_705A:
        xor    eax, eax
        ret
loc_1_705D:
        or     bl, 16h
        xor    bh, bh
        jmp    SHORT loc_1_7046
sub_1_7064:
        call   sub_1_6E08
        mov    dword ptr [eax + 8], ebx
        lea    edi, [eax + 28h]
        mov    dword ptr [eax + 0ch], edi
        push   ecx
        call   sub_1_6EBC
        pop    ecx
        jb     SHORT loc_1_70A8
        mov    word ptr [eax], 714eh
        mov    dword ptr [eax + 10h], 1
        call   sub_1_6E3C
        call   sub_1_6E48
        jb     SHORT loc_1_70A8
        mov    ebx, ecx
        call   sub_1_7138
        jb     SHORT loc_1_70A8
        movzx  ebx, word ptr [eax]
        call   sub_1_6EB8
        mov    eax, ebx
        clc
        ret
loc_1_70A8:
        call   sub_1_6EB8
        stc
        ret
        db  090h
sub_1_70B0:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 18h]
        call   sub_1_70CC
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        jb     SHORT loc_1_70C9
        xor    eax, eax
loc_1_70C9:
        ret
        db  08Bh, 0C0h
sub_1_70CC:
        call   sub_1_6E08
        mov    word ptr [eax], 714fh
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 10h], 1
        call   sub_1_6E3C
        call   sub_1_6E48
        jb     SHORT loc_1_70FD
        mov    ebx, esi
        call   sub_1_7138
        jb     SHORT loc_1_7102
loc_1_70F5:
        call   sub_1_6EB8
        mov    eax, ebx
        ret
loc_1_70FD:
        movzx  ebx, word ptr [eax]
        jmp    SHORT loc_1_70F5
loc_1_7102:
        mov    ebx, 1
        jmp    SHORT loc_1_70F5
        db  02Eh, 08Bh, 0C0h
sub_1_710C:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        call   sub_1_6E08
        mov    word ptr [eax], 71a1h
        mov    dword ptr [eax + 4], ecx
        call   sub_1_6E3C
        call   sub_1_6E48
        call   sub_1_6EB8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_7138:
        push   edi
        push   esi
        push   eax
        mov    esi, eax
        movzx  eax, word ptr [esi + 23bh]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 25bh]
        mov    dword ptr [ebx + 20h], eax
        mov    eax, dword ptr [esi + 243h]
        mov    edx, dword ptr [esi + 23fh]
        push   eax
        push   edx
        db  0CDh, 020h, 03Ah, 000h, 040h, 000h
        add    esp, 8
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 24bh]
        mov    edx, dword ptr [esi + 247h]
        push   eax
        push   edx
        db  0CDh, 020h, 03Ah, 000h, 040h, 000h
        add    esp, 8
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [esi + 253h]
        mov    edx, dword ptr [esi + 24fh]
        push   eax
        push   edx
        db  0CDh, 020h, 03Ah, 000h, 040h, 000h
        add    esp, 8
        mov    dword ptr [ebx + 14h], eax
        lea    esi, [esi + 267h]
        lea    edi, [ebx + 2ch]
        mov    ecx, 0ffffffffh
        mov    edx, 0ffh
        xor    eax, eax
        call   sub_1_6F7C
        jb     SHORT loc_1_71E0
        mov    eax, dword ptr [esp]
        lea    esi, [eax + 46fh]
        lea    edi, [ebx + 234h]
        mov    ecx, 0ffffffffh
        mov    edx, 1ch
        xor    eax, eax
        call   sub_1_6F7C
        jb     SHORT loc_1_71E0
        clc
loc_1_71DC:
        pop    eax
        pop    esi
        pop    edi
        ret
loc_1_71E0:
        stc
        jmp    SHORT loc_1_71DC
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_71F0:
        sub    esp, 4
        mov    dword ptr [esp], 0
        db  0E8h, 003h, 0C1h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_1_7212
        push   0
        push   0
        push   186dh
        jmp    NEAR PTR loc_1_732B
loc_1_7212:
        call   sub_1_13CC
        db  0E8h, 02Dh, 091h, 0FEh, 0FFh
        db  0A3h, 098h, 0B1h, 000h, 000h
        lea    eax, [esp]
        push   eax
        call   sub_1_434C
        add    esp, 4
        cmp    dword ptr [esp], 2
        jne    SHORT loc_1_723F
        db  0C7h, 005h, 000h, 0B4h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_723F:
        call   sub_1_82B6
        or     eax, eax
        jne    NEAR PTR loc_1_7322
        db  080h, 03Dh, 0ACh, 0B1h, 000h, 000h, 000h
        je     NEAR PTR loc_1_7322
        db  068h, 0D0h, 09Fh, 000h, 000h
        call   sub_1_7F68
        add    esp, 4
        call   sub_1_837A
        or     eax, eax
        jne    SHORT loc_1_7278
        db  083h, 03Dh, 0CCh, 0B1h, 000h, 000h, 000h
        jne    SHORT loc_1_729A
loc_1_7278:
        db  0A1h, 0D4h, 09Fh, 000h, 000h
        test   ah, 10h
        jne    SHORT loc_1_7290
        push   0
        push   0
        push   1874h
        jmp    NEAR PTR loc_1_732B
loc_1_7290:
        and    eax, 0ffffdfffh
        db  0A3h, 0D4h, 09Fh, 000h, 000h
loc_1_729A:
        db  0E8h, 0B5h, 090h, 0FEh, 0FFh
        or     eax, eax
        jne    SHORT loc_1_72C2
        db  0A1h, 0D4h, 09Fh, 000h, 000h
        test   ah, 10h
        jne    SHORT loc_1_72B8
        push   0
        push   0
        push   1874h
        jmp    SHORT loc_1_732B
loc_1_72B8:
        and    eax, 0ffffdfffh
        db  0A3h, 0D4h, 09Fh, 000h, 000h
loc_1_72C2:
        call   sub_1_443A
        db  08Ah, 00Dh, 0D0h, 0B3h, 000h, 000h
        cmp    cl, 7
        jae    SHORT loc_1_72F5
        push   0
        push   7
        db  00Fh, 0B6h, 005h, 0D1h, 0B3h, 000h, 000h
        push   eax
        movzx  eax, cl
        push   eax
        push   0
        push   4
        push   17e2h
        call   sub_1_8D90
        add    esp, 1ch
        jmp    SHORT loc_1_7333
loc_1_72F5:
        db  068h, 0D8h, 0B3h, 000h, 000h
        db  068h, 0D4h, 0B3h, 000h, 000h
        call   sub_1_7D30
        add    esp, 8
        push   0
        call   sub_1_92D8
        add    esp, 4
        db  0A3h, 0F4h, 0B3h, 000h, 000h
        call   sub_1_81FB
        mov    eax, 1
        jmp    SHORT loc_1_7335
loc_1_7322:
        push   0
        push   0
        push   186ch
loc_1_732B:
        call   sub_1_8D90
        add    esp, 0ch
loc_1_7333:
        sub    eax, eax
loc_1_7335:
        add    esp, 4
        ret
sub_1_7339:
        push   ebp
        mov    ebp, esp
        sub    esp, 64h
        push   esi
        push   edi
        db  083h, 03Dh, 0E8h, 0B3h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_7643
        db  083h, 03Dh, 0ECh, 0B3h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_7643
        db  083h, 03Dh, 010h, 0B4h, 000h, 000h, 000h
        je     SHORT loc_1_736B
        sub    eax, eax
        jmp    NEAR PTR loc_1_7648
loc_1_736B:
        db  0C7h, 005h, 0E8h, 0B3h, 000h, 000h, 001h, 000h, 000h, 000h
        call   sub_1_3A40
        db  083h, 03Dh, 0E0h, 0B3h, 000h, 000h, 000h
        jne    SHORT loc_1_738E
        db  0FFh, 035h, 090h, 0B1h, 000h, 000h
        jmp    NEAR PTR loc_1_761C
loc_1_738E:
        mov    dword ptr [ebp - 1ch], 0
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 005h, 000h, 093h, 004h
        add    esp, 4
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_1_73BB
        mov    eax, dword ptr [ebp - 1ch]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_73CB
loc_1_73BB:
        db  0FFh, 035h, 090h, 0B1h, 000h, 000h
        push   30h
        call   sub_1_91EB
        add    esp, 8
loc_1_73CB:
        db  083h, 03Dh, 088h, 0B4h, 000h, 000h, 000h
        je     SHORT loc_1_73EE
        db  0C7h, 005h, 088h, 0B4h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 094h, 0B1h, 000h, 000h
        push   30h
        call   sub_1_91EB
        add    esp, 8
loc_1_73EE:
        db  0E8h, 082h, 013h, 0FFh, 0FFh
        or     eax, eax
        je     NEAR PTR loc_1_7616
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        call   sub_1_68E0
        or     eax, eax
        je     NEAR PTR loc_1_7616
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        db  0E8h, 03Ch, 00Ch, 0FFh, 0FFh
        or     eax, eax
        je     NEAR PTR loc_1_7616
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        call   sub_1_7930
        or     eax, eax
        je     NEAR PTR loc_1_7616
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        call   sub_1_46F0
        or     eax, eax
        je     NEAR PTR loc_1_7616
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        call   sub_1_3480
        or     eax, eax
        je     NEAR PTR loc_1_7616
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        call   sub_1_2912
        or     eax, eax
        je     NEAR PTR loc_1_7616
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        call   sub_1_0080
        or     eax, eax
        je     NEAR PTR loc_1_7616
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        db  0E8h, 000h, 05Bh, 0FFh, 0FFh
        or     eax, eax
        je     NEAR PTR loc_1_7616
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        call   sub_1_23B0
        or     eax, eax
        je     NEAR PTR loc_1_7616
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        call   sub_1_8D0E
        or     eax, eax
        jne    SHORT loc_1_74BA
        jmp    NEAR PTR loc_1_7616
loc_1_74BA:
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        db  0E8h, 09Bh, 020h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_1_75E0
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        db  0E8h, 0D5h, 014h, 0FFh, 0FFh
        or     eax, eax
        je     NEAR PTR loc_1_75E0
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        call   sub_1_2506
        call   sub_1_25AB
        or     eax, eax
        je     NEAR PTR loc_1_75E0
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        db  068h, 0E6h, 019h, 000h, 000h
        lea    eax, [ebp - 2]
        push   eax
        push   5104h
        call   sub_1_3112
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_75F0
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        db  068h, 004h, 0B4h, 000h, 000h
        call   sub_1_31F4
        add    esp, 4
        db  0F6h, 005h, 0D5h, 09Fh, 000h, 000h, 010h
        db  066h, 0C7h, 005h, 00Eh, 0B4h, 000h, 000h, 004h, 051h
        je     SHORT loc_1_7582
        mov    word ptr [ebp - 64h], 400h
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 62h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    word ptr [ebp - 28h], 5104h
        lea    eax, [ebp - 64h]
        push   eax
        call   sub_1_328E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_75E0
loc_1_7582:
        db  0F6h, 005h, 0D5h, 09Fh, 000h, 000h, 020h
        je     SHORT loc_1_75A6
        push   5104h
        push   7d06h
        db  068h, 0ACh, 0B1h, 000h, 000h
        call   sub_1_0EE3
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_75E0
loc_1_75A6:
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        call   sub_1_3830
        or     eax, eax
        je     SHORT loc_1_75E0
        db  0FFh, 005h, 010h, 0B4h, 000h, 000h
        db  0C7h, 005h, 018h, 0B4h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0ECh, 0B3h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 0E8h, 0B3h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    SHORT loc_1_7648
loc_1_75E0:
        db  0FFh, 035h, 08Ch, 0B1h, 000h, 000h
        push   30h
        call   sub_1_91EB
        add    esp, 8
loc_1_75F0:
        call   sub_1_7688
        db  0FFh, 035h, 0F4h, 0B3h, 000h, 000h
        call   sub_1_92FB
        add    esp, 4
        call   sub_1_8CC6
        db  0C7h, 005h, 0E8h, 0B3h, 000h, 000h, 000h, 000h, 000h, 000h
        sub    eax, eax
        jmp    SHORT loc_1_7648
loc_1_7616:
        db  0FFh, 035h, 08Ch, 0B1h, 000h, 000h
loc_1_761C:
        push   30h
        call   sub_1_91EB
        add    esp, 8
        call   sub_1_76A9
        db  0C7h, 005h, 0ECh, 0B3h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0E8h, 0B3h, 000h, 000h, 000h, 000h, 000h, 000h
        sub    eax, eax
        jmp    SHORT loc_1_7648
loc_1_7643:
        db  0A1h, 0ECh, 0B3h, 000h, 000h
loc_1_7648:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_764E:
        db  083h, 03Dh, 0E4h, 0B3h, 000h, 000h, 000h
        je     SHORT loc_1_7665
        db  083h, 03Dh, 0ECh, 0B3h, 000h, 000h, 000h
        jne    SHORT loc_1_7665
        call   sub_1_7339
loc_1_7665:
        ret
sub_1_7666:
        db  0C7h, 005h, 0E4h, 0B3h, 000h, 000h, 001h, 000h, 000h, 000h
        db  083h, 03Dh, 084h, 0B4h, 000h, 000h, 000h
        je     SHORT loc_1_7687
        db  083h, 03Dh, 0ECh, 0B3h, 000h, 000h, 000h
        jne    SHORT loc_1_7687
        call   sub_1_7339
loc_1_7687:
        ret
sub_1_7688:
        db  083h, 03Dh, 0F0h, 0B3h, 000h, 000h, 000h
        jne    SHORT loc_1_76A8
        db  0C7h, 005h, 0F0h, 0B3h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 0F8h, 0B3h, 000h, 000h
        call   sub_1_6D70
        add    esp, 4
loc_1_76A8:
        ret
sub_1_76A9:
        sub    esp, 40h
        push   esi
        push   edi
        db  0A1h, 010h, 0B4h, 000h, 000h
        dec    eax
        cmp    eax, 12h
        ja     NEAR PTR loc_1_7782
        db  0FFh, 024h, 085h, 092h, 077h, 000h, 000h
loc_1_76C4:
        call   sub_1_3A0A
loc_1_76C9:
        db  0A1h, 0D4h, 09Fh, 000h, 000h
        test   ah, 10h
        je     SHORT loc_1_7719
        mov    word ptr [esp + 8], 400h
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 0ah]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    word ptr [esp + 44h], 5104h
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_32A3
        add    esp, 4
        db  0A1h, 0D4h, 09Fh, 000h, 000h
loc_1_7719:
        test   ah, 20h
        je     SHORT loc_1_7735
        push   7d06h
        db  068h, 0ACh, 0B1h, 000h, 000h
        call   sub_1_3257
        add    esp, 8
        db  0A1h, 0D4h, 09Fh, 000h, 000h
loc_1_7735:
        test   al, 1
        je     SHORT loc_1_7743
        push   2
        call   sub_1_0DBE
        add    esp, 4
loc_1_7743:
        push   5104h
        call   sub_1_315A
        add    esp, 4
loc_1_7750:
        db  0E8h, 09Fh, 012h, 0FFh, 0FFh
loc_1_7755:
        call   sub_1_95E0
loc_1_775A:
        call   sub_1_23BE
loc_1_775F:
        db  0E8h, 03Bh, 058h, 0FFh, 0FFh
loc_1_7764:
        call   sub_1_29BE
loc_1_7769:
        call   sub_1_3517
loc_1_776E:
        call   sub_1_4716
loc_1_7773:
        call   sub_1_79B9
loc_1_7778:
        db  0E8h, 0BEh, 009h, 0FFh, 0FFh
loc_1_777D:
        db  0E8h, 00Eh, 010h, 0FFh, 0FFh
loc_1_7782:
        db  0C7h, 005h, 010h, 0B4h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        add    esp, 40h
        ret
        db  07Dh, 077h, 000h, 000h, 07Dh, 077h, 000h, 000h, 078h, 077h, 000h, 000h, 073h, 077h, 000h, 000h
        db  06Eh, 077h, 000h, 000h, 069h, 077h, 000h, 000h, 064h, 077h, 000h, 000h, 064h, 077h, 000h, 000h
        db  05Fh, 077h, 000h, 000h, 05Ah, 077h, 000h, 000h, 05Ah, 077h, 000h, 000h, 055h, 077h, 000h, 000h
        db  050h, 077h, 000h, 000h, 050h, 077h, 000h, 000h, 043h, 077h, 000h, 000h, 0C9h, 076h, 000h, 000h
        db  0C4h, 076h, 000h, 000h, 0C4h, 076h, 000h, 000h, 0C4h, 076h, 000h, 000h
loc_1_77DE:
        push   esi
        db  0C7h, 005h, 018h, 0B4h, 000h, 000h, 001h, 000h, 000h, 000h
        db  083h, 03Dh, 040h, 0B4h, 000h, 000h, 000h
        je     SHORT loc_1_7802
        db  0C7h, 005h, 040h, 0B4h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 00Dh, 03Ch, 0B4h, 000h, 000h
loc_1_7802:
        db  068h, 058h, 0B4h, 000h, 000h
        call   sub_1_94C8
        add    esp, 4
        db  08Bh, 035h, 050h, 0B4h, 000h, 000h
        db  081h, 0FEh, 050h, 0B4h, 000h, 000h
        je     SHORT loc_1_7845
loc_1_781D:
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    dword ptr [eax], esi
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [eax]
        push   dword ptr [eax + 10h]
        call   sub_1_92FB
        add    esp, 4
        call   sub_1_7B2C
        db  081h, 0FEh, 050h, 0B4h, 000h, 000h
        jne    SHORT loc_1_781D
loc_1_7845:
        db  068h, 058h, 0B4h, 000h, 000h
        call   sub_1_950C
        add    esp, 4
        call   sub_1_76A9
        db  0FFh, 005h, 03Ch, 0A1h, 000h, 000h
        db  0FFh, 035h, 040h, 0A1h, 000h, 000h
        call   sub_1_43E2
        add    esp, 4
        db  0FFh, 005h, 0D8h, 0B1h, 000h, 000h
        db  0FFh, 035h, 0DCh, 0B1h, 000h, 000h
        call   sub_1_43E2
        add    esp, 4
        db  0FFh, 005h, 0E8h, 0B1h, 000h, 000h
        db  0FFh, 035h, 0ECh, 0B1h, 000h, 000h
        call   sub_1_43E2
        add    esp, 4
        db  0C7h, 005h, 0F0h, 0B3h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 0F4h, 0B3h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
        sub    eax, eax
        pop    esi
        ret
sub_1_78AF:
        db  083h, 03Dh, 014h, 0B4h, 000h, 000h, 000h
        je     SHORT loc_1_78BA
        jmp    SHORT loc_1_791F
loc_1_78BA:
        db  0C7h, 005h, 014h, 0B4h, 000h, 000h, 001h, 000h, 000h, 000h
        db  083h, 03Dh, 0ECh, 0B3h, 000h, 000h, 000h
        je     SHORT loc_1_7915
        call   sub_1_7688
        db  0FFh, 035h, 0F4h, 0B3h, 000h, 000h
        call   sub_1_92FB
        add    esp, 4
        db  0C7h, 005h, 0ECh, 0B3h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_8CC6
        call   sub_1_68FB
        db  0A1h, 03Ch, 0A1h, 000h, 000h
        dec    eax
        db  0A3h, 03Ch, 0A1h, 000h, 000h
        db  0A1h, 0D8h, 0B1h, 000h, 000h
        dec    eax
        db  0A3h, 0D8h, 0B1h, 000h, 000h
        db  0A1h, 0E8h, 0B1h, 000h, 000h
        dec    eax
        db  0A3h, 0E8h, 0B1h, 000h, 000h
loc_1_7915:
        db  0C7h, 005h, 014h, 0B4h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_791F:
        mov    eax, 1
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_7930:
        push   esi
        push   0
        db  0A1h, 024h, 0B4h, 000h, 000h
        lea    eax, [eax*4 + 4]
        push   eax
        call   sub_1_4364
        add    esp, 8
        db  0A3h, 04Ch, 0B4h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_7955
        sub    eax, eax
        jmp    SHORT loc_1_79B7
loc_1_7955:
        db  068h, 058h, 0B4h, 000h, 000h
        call   sub_1_93BC
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_7978
        db  0FFh, 035h, 04Ch, 0B4h, 000h, 000h
        call   sub_1_4390
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_79B7
loc_1_7978:
        sub    si, si
        db  083h, 03Dh, 024h, 0B4h, 000h, 000h, 000h
        jbe    SHORT loc_1_79A0
loc_1_7984:
        movzx  eax, si
        db  08Bh, 015h, 04Ch, 0B4h, 000h, 000h
        lea    ecx, [eax + 1]
        inc    si
        mov    dword ptr [edx + eax*4], ecx
        movzx  eax, si
        db  03Bh, 005h, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_1_7984
loc_1_79A0:
        db  0A1h, 04Ch, 0B4h, 000h, 000h
        db  08Bh, 00Dh, 024h, 0B4h, 000h, 000h
        mov    dword ptr [eax + ecx*4], 0
        mov    eax, 1
loc_1_79B7:
        pop    esi
        ret
sub_1_79B9:
        db  0FFh, 035h, 04Ch, 0B4h, 000h, 000h
        call   sub_1_4390
        add    esp, 4
        db  068h, 058h, 0B4h, 000h, 000h
        call   sub_1_9408
        add    esp, 4
        ret
sub_1_79D5:
        db  0A1h, 04Ch, 0B4h, 000h, 000h
        mov    ax, word ptr [eax]
        or     ax, ax
        je     SHORT loc_1_79F0
        db  08Bh, 015h, 04Ch, 0B4h, 000h, 000h
        movzx  ecx, ax
        mov    ecx, dword ptr [edx + ecx*4]
        mov    dword ptr [edx], ecx
loc_1_79F0:
        ret
sub_1_79F1:
        db  0A1h, 04Ch, 0B4h, 000h, 000h
        movzx  edx, word ptr [esp + 4]
        db  08Bh, 00Dh, 04Ch, 0B4h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + edx*4], eax
        db  0A1h, 04Ch, 0B4h, 000h, 000h
        mov    dword ptr [eax], edx
        ret
sub_1_7A0E:
        mov    eax, dword ptr [esp + 10h]
        push   2
        mov    byte ptr [eax + 22h], 80h
        call   sub_1_96B9
        add    esp, 4
        sub    ax, ax
        ret
sub_1_7A24:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 24h]
        add    edi, 1eh
        cmp    byte ptr [edi + 2], 0
        jne    NEAR PTR loc_1_7B20
        cmp    byte ptr [edi + 3], 0ffh
        jne    NEAR PTR loc_1_7B20
        db  083h, 03Dh, 018h, 0B4h, 000h, 000h, 000h
        jne    SHORT loc_1_7ACA
        db  0A1h, 01Ch, 0B4h, 000h, 000h
        db  039h, 005h, 020h, 0B4h, 000h, 000h
        jbe    SHORT loc_1_7ACA
        db  083h, 03Dh, 03Ch, 0B4h, 000h, 000h, 000h
        jne    SHORT loc_1_7ACA
        db  0A1h, 034h, 0B4h, 000h, 000h
        db  0FFh, 005h, 034h, 0B4h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_7AAE
        call   sub_1_16C0
        db  02Bh, 005h, 038h, 0B4h, 000h, 000h
        cmp    eax, 61a8h
        jbe    SHORT loc_1_7A9F
        db  083h, 03Dh, 040h, 0B4h, 000h, 000h, 000h
        jne    SHORT loc_1_7AD0
        db  0C7h, 005h, 040h, 0B4h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0FFh, 005h, 03Ch, 0B4h, 000h, 000h
        jmp    SHORT loc_1_7AD0
loc_1_7A9F:
        mov    word ptr [edi], 9999h
        mov    byte ptr [edi + 6], 0
        mov    byte ptr [edi + 7], 0
        jmp    SHORT loc_1_7ADD
loc_1_7AAE:
        call   sub_1_16C0
        db  0A3h, 038h, 0B4h, 000h, 000h
        db  089h, 035h, 030h, 0B4h, 000h, 000h
        db  068h, 044h, 0B4h, 000h, 000h
        call   sub_1_6D70
        jmp    SHORT loc_1_7B26
loc_1_7ACA:
        db  0FFh, 005h, 034h, 0B4h, 000h, 000h
loc_1_7AD0:
        mov    word ptr [edi], 3333h
        mov    byte ptr [edi + 6], 0ffh
        mov    byte ptr [edi + 7], 4
loc_1_7ADD:
        db  0FFh, 00Dh, 034h, 0B4h, 000h, 000h
        jne    SHORT loc_1_7AFE
        db  083h, 03Dh, 040h, 0B4h, 000h, 000h, 000h
        je     SHORT loc_1_7AFE
        db  0C7h, 005h, 040h, 0B4h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 00Dh, 03Ch, 0B4h, 000h, 000h
loc_1_7AFE:
        mov    edx, 26h
        mov    ecx, esi
        call   sub_1_1964
        db  0C7h, 046h, 004h, 050h, 019h, 000h, 000h
        mov    dword ptr [esi + 30h], 0
        push   esi
        call   sub_1_318D
        jmp    SHORT loc_1_7B26
loc_1_7B20:
        push   esi
        call   sub_1_27A4
loc_1_7B26:
        add    esp, 4
        pop    edi
        pop    esi
        ret
sub_1_7B2C:
        push   esi
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    esi, dword ptr [eax]
        mov    ax, word ptr [esi + 24h]
        push   eax
        call   sub_1_79F1
        add    esp, 4
        lea    eax, [esi + 38h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_7B52
        push   eax
        call   sub_1_0060
        add    esp, 4
loc_1_7B52:
        db  0FFh, 00Dh, 01Ch, 0B4h, 000h, 000h
        test   byte ptr [esi + 1ah], 80h
        je     SHORT loc_1_7B64
        db  0FFh, 00Dh, 03Ch, 0B4h, 000h, 000h
loc_1_7B64:
        push   1
        db  0E8h, 06Ah, 01Ch, 0FFh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 44h]
        sub    eax, 4
        push   eax
        call   sub_1_4390
        add    esp, 4
        push   1
        db  0E8h, 07Eh, 01Bh, 0FFh, 0FFh
        add    esp, 4
        push   1
        push   esi
        db  0E8h, 0F0h, 0C7h, 0FEh, 0FFh
        add    esp, 8
        mov    eax, dword ptr [esi + 40h]
        sub    eax, 4
        push   eax
        call   sub_1_4390
        add    esp, 4
        push   1
        db  0E8h, 035h, 004h, 0FFh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 34h]
        or     eax, eax
        je     SHORT loc_1_7BBB
        push   eax
        call   sub_1_27A4
        add    esp, 4
loc_1_7BBB:
        mov    eax, dword ptr [esi + 1ch]
        or     eax, eax
        je     SHORT loc_1_7BDB
        mov    dword ptr [eax + 30h], 0
        mov    eax, dword ptr [esi + 1ch]
        cmp    byte ptr [eax + 8], 0
        jne    SHORT loc_1_7BDB
        push   eax
        call   sub_1_27A4
        add    esp, 4
loc_1_7BDB:
        mov    eax, dword ptr [esi + 50h]
        or     eax, eax
        je     SHORT loc_1_7C17
        push   eax
        call   sub_1_65C4
        add    esp, 4
        mov    eax, dword ptr [esi + 50h]
        mov    eax, dword ptr [eax + 2ch]
        or     eax, eax
        je     SHORT loc_1_7BFE
        push   eax
        call   sub_1_2811
        add    esp, 4
loc_1_7BFE:
        mov    eax, dword ptr [esi + 50h]
        push   dword ptr [eax + 30h]
        call   sub_1_92E7
        add    esp, 4
        push   dword ptr [esi + 50h]
        call   sub_1_4390
        add    esp, 4
loc_1_7C17:
        db  0A1h, 094h, 0B8h, 000h, 000h
        push   esi
        mov    dword ptr [eax], 0
        call   sub_1_0060
        add    esp, 4
        mov    eax, dword ptr [esi + 4ch]
        or     eax, eax
        je     SHORT loc_1_7C3B
        push   eax
        call   sub_1_4390
        add    esp, 4
loc_1_7C3B:
        push   esi
        db  0E8h, 081h, 006h, 0FFh, 0FFh
        add    esp, 4
        push   esi
        db  0E8h, 01Eh, 01Ch, 0FFh, 0FFh
        add    esp, 4
        push   dword ptr [esi + 10h]
        call   sub_1_92F1
        add    esp, 4
        push   dword ptr [esi + 10h]
        call   sub_1_92E7
        add    esp, 4
        push   esi
        call   sub_1_4390
        add    esp, 4
        pop    esi
        ret
sub_1_7C6E:
        db  039h, 00Dh, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_1_7C8A
        or     ecx, ecx
        je     SHORT loc_1_7C8A
        db  0A1h, 04Ch, 0B4h, 000h, 000h
        mov    eax, dword ptr [eax + ecx*4]
        db  039h, 005h, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_1_7C8C
loc_1_7C8A:
        sub    eax, eax
loc_1_7C8C:
        ret
        db  000h, 000h, 000h
loc_1_7C90:
        mov    eax, dword ptr [esp + 4]
        dec    eax
        cmp    eax, 0dh
        ja     SHORT loc_1_7CD9
        db  0FFh, 024h, 085h, 0DCh, 07Ch, 000h, 000h
loc_1_7CA1:
        db  0FFh, 005h, 084h, 0B4h, 000h, 000h
        push   0
        call   sub_1_4404
        add    esp, 4
        call   sub_1_764E
        jmp    SHORT loc_1_7CCF
loc_1_7CB8:
        db  0FFh, 00Dh, 084h, 0B4h, 000h, 000h
        jne    SHORT loc_1_7CD9
        push   0
        call   sub_1_4404
        add    esp, 4
        call   sub_1_78AF
loc_1_7CCF:
        push   0
        call   sub_1_440A
        add    esp, 4
loc_1_7CD9:
        sub    eax, eax
        ret
loc_1_7CDC:
        db  0A1h, 07Ch, 000h, 000h, 0D9h
        jl     SHORT loc_1_7CE3
loc_1_7CE3:
        db  000h, 0D9h
        jl     SHORT loc_1_7CE7
loc_1_7CE7:
        db  000h, 0B8h, 07Ch, 000h, 000h, 0D9h
        jl     SHORT loc_1_7CEF
loc_1_7CEF:
        db  000h, 0D9h
        jl     SHORT loc_1_7CF3
loc_1_7CF3:
        db  000h, 0D9h
        jl     SHORT loc_1_7CF7
loc_1_7CF7:
        db  000h, 0D9h
        jl     SHORT loc_1_7CFB
loc_1_7CFB:
        db  000h, 0D9h
        jl     SHORT loc_1_7CFF
loc_1_7CFF:
        db  000h, 0D9h
        jl     SHORT loc_1_7D03
loc_1_7D03:
        db  000h, 0D9h
        jl     SHORT loc_1_7D07
loc_1_7D07:
        db  000h, 0D9h
        jl     SHORT loc_1_7D0B
loc_1_7D0B:
        db  000h, 0D9h
        jl     SHORT loc_1_7D0F
loc_1_7D0F:
        db  000h, 0D9h
        db  07Ch, 000h
        db  000h
sub_1_7D14:
        push   0
        push   0
        db  068h, 090h, 07Ch, 000h, 000h
        push   dword ptr [esp + 10h]
        call   sub_1_43FE
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_7D2F
        sub    eax, eax
loc_1_7D2F:
        ret
sub_1_7D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 4
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi], 1
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [edi], 1
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 54h], eax
        db  0C7h, 045h, 0B0h, 08Ch, 0B4h, 000h, 000h
        mov    dword ptr [ebp - 4ch], 80000002h
        push   dword ptr [ebp - 54h]
        push   dword ptr [ebp - 50h]
        push   dword ptr [ebp - 4ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_1_7E52
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 48h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 44h], eax
        lea    eax, [ebp - 30h]
        mov    dword ptr [ebp - 40h], eax
        mov    dword ptr [ebp - 3ch], 0
        db  0C7h, 045h, 0C8h, 0C8h, 0B4h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 34h], eax
        push   dword ptr [ebp - 48h]
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_7DDF
        cmp    dword ptr [ebp - 0ch], 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [edi], eax
loc_1_7DDF:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 2ch], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 28h], eax
        lea    eax, [ebp - 30h]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 0D8h, 0B4h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 18h], eax
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_7E38
        cmp    dword ptr [ebp - 0ch], 1
        sbb    ecx, ecx
        neg    ecx
        mov    dword ptr [esi], ecx
        or     eax, eax
        je     SHORT loc_1_7E3A
loc_1_7E38:
        mov    ebx, eax
loc_1_7E3A:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_1_7E52:
        push   3
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 0E8h, 0B4h, 000h, 000h
        db  068h, 0F4h, 0B4h, 000h, 000h
        push   80000002h
        call   sub_1_7EB5
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_7E7F
        cmp    byte ptr [ebp - 4], 4eh
        jne    SHORT loc_1_7E7F
        mov    dword ptr [edi], 0
loc_1_7E7F:
        push   3
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 024h, 0B5h, 000h, 000h
        db  068h, 034h, 0B5h, 000h, 000h
        push   80000002h
        call   sub_1_7EB5
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_7EAC
        cmp    byte ptr [ebp - 4], 4eh
        jne    SHORT loc_1_7EAC
        mov    dword ptr [esi], 0
loc_1_7EAC:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_7EB5:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 18h]
        lea    eax, [esi + 1]
        mov    dword ptr [ebp - 24h], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 28h], eax
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_7F5F
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 20h], eax
        mov    ebx, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 1ch], ebx
        lea    eax, [ebp - 34h]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_7F47
        mov    byte ptr [ebx + esi], 0
loc_1_7F47:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_1_7F5F:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_7F68:
        push   ebp
        mov    ebp, esp
        sub    esp, 94h
        mov    dword ptr [ebp - 10h], 4
        db  0C7h, 005h, 088h, 0B4h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 94h], eax
        db  0C7h, 085h, 070h, 0FFh, 0FFh, 0FFh, 064h, 0B5h, 000h, 000h
        mov    dword ptr [ebp - 8ch], 80000002h
        push   dword ptr [ebp - 94h]
        push   dword ptr [ebp - 90h]
        push   dword ptr [ebp - 8ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_81E4
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 88h], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 84h], eax
        lea    eax, [ebp - 70h]
        mov    dword ptr [ebp - 80h], eax
        mov    dword ptr [ebp - 7ch], 0
        db  0C7h, 045h, 088h, 09Ch, 0B5h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 74h], eax
        push   dword ptr [ebp - 88h]
        push   dword ptr [ebp - 84h]
        push   dword ptr [ebp - 80h]
        push   dword ptr [ebp - 7ch]
        push   dword ptr [ebp - 78h]
        push   dword ptr [ebp - 74h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_802F
        cmp    byte ptr [ebp - 8], 31h
        jne    SHORT loc_1_802F
        db  081h, 00Dh, 0D4h, 09Fh, 000h, 000h, 000h, 010h, 000h, 000h
loc_1_802F:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 6ch], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 68h], eax
        lea    eax, [ebp - 70h]
        mov    dword ptr [ebp - 64h], eax
        mov    dword ptr [ebp - 60h], 0
        db  0C7h, 045h, 0A4h, 0A4h, 0B5h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 58h], eax
        push   dword ptr [ebp - 6ch]
        push   dword ptr [ebp - 68h]
        push   dword ptr [ebp - 64h]
        push   dword ptr [ebp - 60h]
        push   dword ptr [ebp - 5ch]
        push   dword ptr [ebp - 58h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_80BF
        movzx  eax, byte ptr [ebp - 8]
        cmp    eax, 30h
        je     SHORT loc_1_80BF
        cmp    eax, 32h
        je     SHORT loc_1_8098
        cmp    eax, 33h
        mov    eax, dword ptr [ebp + 8]
        je     SHORT loc_1_80AD
        mov    dword ptr [eax], 20000h
        jmp    SHORT loc_1_80B3
loc_1_8098:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 30000h
        db  081h, 00Dh, 0D4h, 09Fh, 000h, 000h, 000h, 020h, 000h, 000h
        jmp    SHORT loc_1_80C8
loc_1_80AD:
        mov    dword ptr [eax], 0
loc_1_80B3:
        db  081h, 00Dh, 0D4h, 09Fh, 000h, 000h, 000h, 020h, 000h, 000h
        jmp    SHORT loc_1_80C8
loc_1_80BF:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
loc_1_80C8:
        db  0F6h, 005h, 0D5h, 09Fh, 000h, 000h, 030h
        jne    NEAR PTR loc_1_81CA
        db  081h, 00Dh, 0D4h, 09Fh, 000h, 000h, 000h, 020h, 000h, 000h
        mov    dword ptr [eax], 20000h
        mov    dword ptr [ebp - 54h], 1
        db  0C7h, 045h, 0B0h, 0B4h, 0B5h, 000h, 000h
        mov    dword ptr [ebp - 4ch], 1
        mov    dword ptr [ebp - 48h], 0
        db  0C7h, 045h, 0BCh, 0B8h, 0B5h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 40h], eax
        push   dword ptr [ebp - 54h]
        push   dword ptr [ebp - 50h]
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 3ch], eax
        db  0C7h, 045h, 0C8h, 0C8h, 0B5h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 34h], eax
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_81C0
        mov    dword ptr [ebp - 30h], 1
        db  0C7h, 045h, 0D4h, 0E0h, 0B5h, 000h, 000h
        mov    dword ptr [ebp - 28h], 1
        mov    dword ptr [ebp - 24h], 0
        mov    dword ptr [ebp - 20h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 1ch], eax
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 18h], eax
        push   dword ptr [ebp - 18h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_1_81C0:
        db  0C7h, 005h, 088h, 0B4h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_81CA:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        jmp    SHORT loc_1_81F7
loc_1_81E4:
        mov    eax, dword ptr [ebp + 8]
        db  081h, 00Dh, 0D4h, 09Fh, 000h, 000h, 000h, 020h, 000h, 000h
        mov    dword ptr [eax], 20000h
loc_1_81F7:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_81FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 28h], 4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 34h], eax
        db  0C7h, 045h, 0D0h, 0E4h, 0B5h, 000h, 000h
        mov    dword ptr [ebp - 2ch], 80000002h
        push   dword ptr [ebp - 34h]
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_82B2
        lea    eax, [ebp - 28h]
        mov    dword ptr [ebp - 24h], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 20h], eax
        lea    eax, [ebp - 38h]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 18h], 0
        db  0C7h, 045h, 0ECh, 010h, 0B6h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_829A
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_829A
        db  0C7h, 005h, 044h, 0AFh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_829A:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_1_82B2:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_82B6:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        push   0fh
        db  068h, 0ACh, 0B1h, 000h, 000h
        db  068h, 020h, 0B6h, 000h, 000h
        db  068h, 030h, 0B6h, 000h, 000h
        push   80000002h
        call   sub_1_7EB5
        add    esp, 14h
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_8335
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  0BFh, 09Ch, 0B1h, 000h, 000h
        lea    ebx, [ecx - 1]
        push   ebx
        db  068h, 0ACh, 0B1h, 000h, 000h
        db  068h, 0ACh, 0B1h, 000h, 000h
        call   sub_1_1874
        add    esp, 0ch
        mov    eax, 20202020h
        stosd
        stosd
        stosd
        stosd
        db  0BEh, 0ACh, 0B1h, 000h, 000h
        db  0BFh, 09Ch, 0B1h, 000h, 000h
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_1_8335:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_833C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   0feh
        lea    edi, [esi + 1]
        mov    byte ptr [esi], 0
        push   edi
        db  068h, 06Ch, 0B6h, 000h, 000h
        db  068h, 074h, 0B6h, 000h, 000h
        push   80000002h
        call   sub_1_7EB5
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_8377
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [esi], cl
loc_1_8377:
        pop    edi
        pop    esi
        ret
sub_1_837A:
        push   ebx
        push   esi
        push   edi
        db  0BFh, 0BCh, 0B1h, 000h, 000h
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        push   0fh
        db  068h, 0BCh, 0B1h, 000h, 000h
        db  068h, 0A4h, 0B6h, 000h, 000h
        db  068h, 0B0h, 0B6h, 000h, 000h
        push   80000002h
        call   sub_1_7EB5
        add    esp, 14h
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_83D8
        db  0BFh, 0BCh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [ecx - 1]
        push   esi
        db  068h, 0BCh, 0B1h, 000h, 000h
        db  068h, 0BCh, 0B1h, 000h, 000h
        call   sub_1_1874
        add    esp, 0ch
        db  089h, 035h, 0CCh, 0B1h, 000h, 000h
loc_1_83D8:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_83DE:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 14h], 0
        push   200h
        push   100h
        call   sub_1_4364
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    SHORT loc_1_8413
        jmp    NEAR PTR loc_1_86B1
loc_1_8413:
        lea    eax, [ebp - 2ch]
        mov    dword ptr [ebp - 40h], eax
        db  0C7h, 045h, 0C4h, 0E0h, 0B6h, 000h, 000h
        mov    dword ptr [ebp - 38h], 80000002h
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_86A6
        mov    dword ptr [ebp - 4], 0
loc_1_844E:
        mov    dword ptr [ebp - 28h], 100h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [ebp - 1ch], eax
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Ch, 001h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_1_859F
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   200h
        lea    eax, [ecx - 1]
        mov    dword ptr [ebp - 0ch], eax
        add    eax, 1ah
        push   eax
        call   sub_1_4364
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_8575
        mov    eax, dword ptr [ebp - 0ch]
        inc    eax
        lea    edi, [ebx + 19h]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [ebp - 4]
        sub    ecx, ecx
loc_1_84D6:
        mov    edi, 0ah
        mov    eax, esi
        sub    edx, edx
        div    edi
        mov    eax, ebx
        sub    eax, ecx
        add    dl, 30h
        mov    byte ptr [eax + 17h], dl
        mov    eax, esi
        sub    edx, edx
        div    edi
        mov    esi, eax
        inc    ecx
        cmp    ecx, 4
        jb     SHORT loc_1_84D6
        mov    byte ptr [ebx + 18h], 0
        db  0BEh, 014h, 0B7h, 000h, 000h
        lea    edi, [ebx + 4]
        movsd
        movsd
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_8535
        mov    ecx, 0ah
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    ecx
        add    al, 30h
        mov    byte ptr [ebx + 0bh], al
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    ecx
        add    dl, 30h
        mov    byte ptr [ebx + 0ch], dl
        mov    byte ptr [ebx + 0dh], 24h
        mov    byte ptr [ebx + 0eh], 0
        jmp    SHORT loc_1_853D
loc_1_8535:
        mov    byte ptr [ebx + 0bh], 24h
        mov    byte ptr [ebx + 0ch], 0
loc_1_853D:
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 0ch]
        lea    ecx, [ecx + eax + 1]
        mov    dword ptr [ebp - 18h], ecx
        mov    dword ptr [ebx], 0
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_1_855B
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax], ebx
loc_1_855B:
        db  083h, 03Dh, 02Ch, 0B2h, 000h, 000h, 000h
        jne    SHORT loc_1_856A
        db  089h, 01Dh, 02Ch, 0B2h, 000h, 000h
loc_1_856A:
        mov    dword ptr [ebp - 14h], ebx
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_844E
loc_1_8575:
        push   dword ptr [ebp - 8]
        call   sub_1_4390
        add    esp, 4
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [ebp - 34h], eax
        push   dword ptr [ebp - 34h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        sub    eax, eax
        jmp    NEAR PTR loc_1_86B3
loc_1_859F:
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 30h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [ecx + eax*4 + 8]
        db  0A3h, 064h, 0B9h, 000h, 000h
        push   200h
        push   eax
        call   sub_1_4364
        add    esp, 8
        db  0A3h, 02Ch, 0B9h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        lea    esi, [eax*4]
        push   200h
        push   esi
        call   sub_1_4364
        add    esp, 8
        db  0A3h, 04Ch, 0B9h, 000h, 000h
        db  083h, 03Dh, 02Ch, 0B9h, 000h, 000h, 000h
        je     NEAR PTR loc_1_86A6
        or     eax, eax
        je     NEAR PTR loc_1_86A6
        db  08Bh, 00Dh, 02Ch, 0B9h, 000h, 000h
        db  0A1h, 064h, 0B9h, 000h, 000h
        mov    dword ptr [ecx], eax
        db  08Bh, 00Dh, 02Ch, 0B9h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 02Ch, 0B9h, 000h, 000h
        lea    eax, [esi + eax + 8]
        mov    dword ptr [ebp - 10h], eax
        db  08Bh, 01Dh, 02Ch, 0B2h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_8694
        sub    edx, edx
loc_1_863C:
        db  0A1h, 04Ch, 0B9h, 000h, 000h
        mov    dword ptr [edx + eax], ebx
        mov    eax, dword ptr [ebp - 10h]
        db  02Bh, 005h, 02Ch, 0B9h, 000h, 000h
        lea    edi, [ebx + 19h]
        db  08Bh, 00Dh, 02Ch, 0B9h, 000h, 000h
        mov    dword ptr [edx + ecx + 8], eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebp - 10h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    edi, [ebx + 19h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    dword ptr [ebp - 10h], ecx
        mov    ebx, dword ptr [ebx]
        add    edx, 4
        or     ebx, ebx
        jne    SHORT loc_1_863C
loc_1_8694:
        push   dword ptr [ebp - 8]
        call   sub_1_4390
        add    esp, 4
        mov    eax, 1
        jmp    SHORT loc_1_86B3
loc_1_86A6:
        push   dword ptr [ebp - 8]
        call   sub_1_4390
        add    esp, 4
loc_1_86B1:
        sub    eax, eax
loc_1_86B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_86BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0cch
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 10h
        jbe    SHORT loc_1_86E5
        mov    eax, 57h
        jmp    NEAR PTR loc_1_890B
loc_1_86E5:
        db  0BEh, 01Ch, 0B7h, 000h, 000h
        lea    edi, [ebp - 0cch]
        mov    ecx, 0eh
        rep movsd
        movsb
        mov    byte ptr [ebp - 94h], 5ch
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 93h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 80h], eax
        lea    eax, [ebp - 0cch]
        mov    dword ptr [ebp - 7ch], eax
        mov    dword ptr [ebp - 78h], 80000002h
        push   dword ptr [ebp - 80h]
        push   dword ptr [ebp - 7ch]
        push   dword ptr [ebp - 78h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_890B
        mov    dword ptr [ebp - 74h], 4
        lea    eax, [ebp + 18h]
        mov    dword ptr [ebp - 70h], eax
        mov    dword ptr [ebp - 6ch], 4
        mov    dword ptr [ebp - 68h], 0
        db  0C7h, 045h, 09Ch, 058h, 0B7h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 60h], eax
        push   dword ptr [ebp - 74h]
        push   dword ptr [ebp - 70h]
        push   dword ptr [ebp - 6ch]
        push   dword ptr [ebp - 68h]
        push   dword ptr [ebp - 64h]
        push   dword ptr [ebp - 60h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_1_88CC
        mov    dword ptr [ebp - 5ch], 4
        lea    eax, [ebp + 14h]
        mov    dword ptr [ebp - 58h], eax
        mov    dword ptr [ebp - 54h], 4
        mov    dword ptr [ebp - 50h], 0
        db  0C7h, 045h, 0B4h, 060h, 0B7h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 48h], eax
        push   dword ptr [ebp - 5ch]
        push   dword ptr [ebp - 58h]
        push   dword ptr [ebp - 54h]
        push   dword ptr [ebp - 50h]
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_1_88CC
        mov    edx, dword ptr [ebp + 0ch]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 44h], ecx
        mov    dword ptr [ebp - 40h], edx
        mov    dword ptr [ebp - 3ch], 1
        mov    dword ptr [ebp - 38h], 0
        db  0C7h, 045h, 0CCh, 068h, 0B7h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        push   dword ptr [ebp - 30h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_88CC
        mov    edx, dword ptr [ebp + 10h]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 2ch], ecx
        mov    dword ptr [ebp - 28h], edx
        mov    dword ptr [ebp - 24h], 1
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 070h, 0B7h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 18h], eax
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_88CC
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_890B
loc_1_88CC:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        lea    eax, [ebp - 0cch]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 80000002h
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Bh, 001h, 001h, 000h
        add    esp, 8
        mov    eax, esi
loc_1_890B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_8911:
        push   ebp
        mov    ebp, esp
        sub    esp, 0cch
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 10h
        jbe    SHORT loc_1_893C
        mov    eax, 57h
        jmp    NEAR PTR loc_1_8B48
loc_1_893C:
        db  0BEh, 078h, 0B7h, 000h, 000h
        lea    edi, [ebp - 0cch]
        mov    ecx, 0eh
        rep movsd
        movsb
        mov    byte ptr [ebp - 94h], 5ch
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 93h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 80h], eax
        lea    eax, [ebp - 0cch]
        mov    dword ptr [ebp - 7ch], eax
        mov    dword ptr [ebp - 78h], 80000002h
        push   dword ptr [ebp - 80h]
        push   dword ptr [ebp - 7ch]
        push   dword ptr [ebp - 78h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_8B48
        mov    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp + 18h]
        or     eax, eax
        je     SHORT loc_1_8A0D
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 74h], ecx
        mov    dword ptr [ebp - 70h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 6ch], eax
        mov    dword ptr [ebp - 68h], 0
        db  0C7h, 045h, 09Ch, 0B4h, 0B7h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 60h], eax
        push   dword ptr [ebp - 74h]
        push   dword ptr [ebp - 70h]
        push   dword ptr [ebp - 6ch]
        push   dword ptr [ebp - 68h]
        push   dword ptr [ebp - 64h]
        push   dword ptr [ebp - 60h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_1_8ABD
loc_1_8A0D:
        mov    dword ptr [ebp - 4], 4
        mov    eax, dword ptr [ebp + 14h]
        or     eax, eax
        je     SHORT loc_1_8A65
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 5ch], ecx
        mov    dword ptr [ebp - 58h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 54h], eax
        mov    dword ptr [ebp - 50h], 0
        db  0C7h, 045h, 0B4h, 0BCh, 0B7h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 48h], eax
        push   dword ptr [ebp - 5ch]
        push   dword ptr [ebp - 58h]
        push   dword ptr [ebp - 54h]
        push   dword ptr [ebp - 50h]
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_8ABD
loc_1_8A65:
        mov    dword ptr [ebp - 4], 0ffh
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     SHORT loc_1_8AD9
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 44h], ecx
        mov    dword ptr [ebp - 40h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 3ch], eax
        mov    dword ptr [ebp - 38h], 0
        db  0C7h, 045h, 0CCh, 0C4h, 0B7h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        push   dword ptr [ebp - 30h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_8AD9
loc_1_8ABD:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 2ch], eax
        push   dword ptr [ebp - 2ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        mov    eax, esi
        jmp    SHORT loc_1_8B48
loc_1_8AD9:
        mov    dword ptr [ebp - 4], 0ffh
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_8B2E
        mov    byte ptr [eax], 0
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 28h], ecx
        mov    dword ptr [ebp - 24h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 20h], eax
        mov    dword ptr [ebp - 1ch], 0
        db  0C7h, 045h, 0E8h, 0CCh, 0B7h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
loc_1_8B2E:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        sub    eax, eax
loc_1_8B48:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_8B4E:
        push   ebp
        mov    ebp, esp
        sub    esp, 54h
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 10h
        jbe    SHORT loc_1_8B73
        mov    eax, 57h
        jmp    SHORT loc_1_8BCB
loc_1_8B73:
        db  0BEh, 0D4h, 0B7h, 000h, 000h
        lea    edi, [ebp - 54h]
        mov    ecx, 0eh
        rep movsd
        movsb
        mov    byte ptr [ebp - 1ch], 5ch
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 1bh]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [ebp - 54h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], 80000002h
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Bh, 001h, 001h, 000h
        add    esp, 8
loc_1_8BCB:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_8BD1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
        db  0C7h, 045h, 0F8h, 010h, 0B8h, 000h, 000h
        mov    dword ptr [ebp - 4], 80000002h
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        mov    esp, ebp
        pop    ebp
        ret
sub_1_8C07:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        mov    dword ptr [ebp - 10h], 11h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Ch, 001h, 001h, 000h
        add    esp, 10h
        mov    esp, ebp
        pop    ebp
        ret
sub_1_8C45:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_8C70:
        db  0FFh, 00Dh, 04Ch, 0B8h, 000h, 000h
        db  0FFh, 035h, 064h, 0B8h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
        jmp    NEAR PTR sub_1_20F9
loc_1_8C89:
        jmp    NEAR PTR loc_1_69AE
sub_1_8C8E:
        db  0FFh, 005h, 04Ch, 0B8h, 000h, 000h
        push   0
        db  068h, 089h, 08Ch, 000h, 000h
        push   0
        push   0
        push   0
        push   495h
        call   sub_1_21EB
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_8CBF
        db  08Bh, 00Dh, 04Ch, 0B8h, 000h, 000h
        dec    ecx
        db  089h, 00Dh, 04Ch, 0B8h, 000h, 000h
loc_1_8CBF:
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret
sub_1_8CC6:
        db  0FFh, 035h, 064h, 0B8h, 000h, 000h
        call   sub_1_92FB
        add    esp, 4
        db  0C7h, 005h, 058h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 054h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 04Ch, 0B8h, 000h, 000h, 000h
        je     SHORT loc_1_8CF6
        call   sub_1_6940
loc_1_8CF6:
        db  0FFh, 035h, 064h, 0B8h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
        call   sub_1_2302
        jmp    NEAR PTR loc_1_235A
sub_1_8D0E:
        push   esi
        push   edi
        sub    esi, esi
        call   sub_1_229F
        cmp    eax, esi
        je     SHORT loc_1_8D7D
        push   1
        call   sub_1_92D8
        add    esp, 4
        db  0A3h, 064h, 0B8h, 000h, 000h
        db  0C7h, 005h, 058h, 0B8h, 000h, 000h, 002h, 000h, 000h, 000h
        db  0C7h, 005h, 054h, 0B8h, 000h, 000h, 002h, 000h, 000h, 000h
        push   eax
        call   sub_1_92FB
        add    esp, 4
        sub    edi, edi
        db  039h, 035h, 054h, 0B8h, 000h, 000h
        je     SHORT loc_1_8D6A
loc_1_8D51:
        call   sub_1_8C8E
        or     eax, eax
        db  0A1h, 054h, 0B8h, 000h, 000h
        je     SHORT loc_1_8D66
        inc    edi
        cmp    edi, eax
        jb     SHORT loc_1_8D51
        jmp    SHORT loc_1_8D6A
loc_1_8D66:
        or     edi, edi
        je     SHORT loc_1_8D6F
loc_1_8D6A:
        mov    esi, 1
loc_1_8D6F:
        db  0FFh, 035h, 064h, 0B8h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
loc_1_8D7D:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_8D90:
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    edx, [esp + 5ch]
        mov    ecx, dword ptr [esp + 54h]
        lea    eax, [ecx*4 + 0ch]
        mov    dword ptr [esp + 14h], eax
        lea    esi, [esp + 24h]
        mov    eax, dword ptr [esp + 50h]
        mov    dword ptr [esp + 18h], eax
        mov    dword ptr [esp + 1ch], 492h
        mov    byte ptr [esp + 22h], cl
        lea    ebx, [esp + ecx*4 + 24h]
        mov    al, byte ptr [esp + 58h]
        mov    byte ptr [esp + 23h], al
        or     ecx, ecx
        je     SHORT loc_1_8DDF
loc_1_8DD2:
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        add    edx, 4
        add    esi, 4
        dec    ecx
        jne    SHORT loc_1_8DD2
loc_1_8DDF:
        sub    ebp, ebp
        cmp    dword ptr [esp + 58h], ebp
        jbe    SHORT loc_1_8E33
loc_1_8DE7:
        mov    edi, dword ptr [edx]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [esp + 10h], ecx
        mov    edi, dword ptr [edx]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    ebx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 14h]
        add    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 14h], eax
        add    edx, 4
        inc    ebp
        cmp    dword ptr [esp + 58h], ebp
        ja     SHORT loc_1_8DE7
loc_1_8E33:
        mov    ax, word ptr [esp + 14h]
        mov    word ptr [esp + 20h], ax
        lea    eax, [esp + 18h]
        push   eax
        push   5ch
        push   400h
        call   sub_1_4434
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
sub_1_8E59:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0C6h, 005h, 020h, 0B9h, 000h, 000h, 000h
        mov    ecx, dword ptr [esp + 1ch]
        movzx  esi, word ptr [esp + 20h]
        db  0BBh, 020h, 0B9h, 000h, 000h
        mov    di, word ptr [esp + 24h]
loc_1_8E77:
        dec    di
        mov    eax, ecx
        sub    edx, edx
        div    esi
        inc    ebx
        db  08Ah, 082h, 06Ch, 0B8h, 000h, 000h
        mov    byte ptr [ebx], al
        mov    eax, ecx
        sub    edx, edx
        div    esi
        mov    ecx, eax
        or     ecx, ecx
        jne    SHORT loc_1_8E77
        or     di, di
        mov    esi, dword ptr [esp + 14h]
        mov    ebp, dword ptr [esp + 18h]
        jle    SHORT loc_1_8EAF
loc_1_8EA1:
        push   dword ptr [esp + 28h]
        push   ebp
        call   esi
        add    esp, 8
        dec    di
        jne    SHORT loc_1_8EA1
loc_1_8EAF:
        mov    al, byte ptr [ebx]
        push   eax
        push   ebp
        call   esi
        dec    ebx
        add    esp, 8
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_1_8EAF
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_8EC3:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 28h]
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [esp + 18h], eax
        movsx  edi, byte ptr [esi]
        or     edi, edi
        je     NEAR PTR loc_1_905D
loc_1_8EE1:
        cmp    edi, 25h
        je     SHORT loc_1_8F1D
        push   edi
        push   dword ptr [esp + 28h]
        call   dword ptr [esp + 28h]
        add    esp, 8
        db  080h, 0BFh, 038h, 0A0h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_9050
        movsx  eax, byte ptr [esi + 1]
        inc    esi
        or     eax, eax
        je     NEAR PTR loc_1_905D
        push   eax
        push   dword ptr [esp + 28h]
        call   dword ptr [esp + 28h]
        add    esp, 8
        jmp    NEAR PTR loc_1_9050
loc_1_8F1D:
        mov    byte ptr [esp + 12h], 0
        mov    byte ptr [esp + 13h], 0
        sub    bx, bx
        mov    dx, 0ffffh
        mov    byte ptr [esp + 11h], bl
        mov    cl, 20h
        mov    edi, esi
        lea    eax, [esi + 1]
        mov    bp, 0ah
        cmp    byte ptr [eax], 2dh
        jne    SHORT loc_1_8F49
        mov    byte ptr [esp + 11h], 1
        mov    esi, eax
loc_1_8F49:
        mov    al, byte ptr [esi + 1]
        inc    esi
        cmp    al, 30h
        jne    SHORT loc_1_8F53
        mov    cl, 30h
loc_1_8F53:
        movsx  eax, al
        cmp    eax, 30h
        jl     SHORT loc_1_8F72
loc_1_8F5B:
        cmp    eax, 39h
        jg     SHORT loc_1_8F72
        imul   bx, bx, 0ah
        lea    ebx, [eax + ebx - 30h]
        movsx  eax, byte ptr [esi + 1]
        inc    esi
        cmp    eax, 30h
        jge    SHORT loc_1_8F5B
loc_1_8F72:
        cmp    eax, 2eh
        jne    SHORT loc_1_8F9B
        sub    dx, dx
        movsx  eax, byte ptr [esi + 1]
        inc    esi
        cmp    eax, 30h
        jl     SHORT loc_1_8F9B
loc_1_8F84:
        cmp    eax, 39h
        jg     SHORT loc_1_8F9B
        imul   dx, dx, 0ah
        lea    edx, [eax + edx - 30h]
        movsx  eax, byte ptr [esi + 1]
        inc    esi
        cmp    eax, 30h
        jge    SHORT loc_1_8F84
loc_1_8F9B:
        cmp    eax, 6ch
        jne    SHORT loc_1_8FAA
        mov    byte ptr [esp + 12h], 1
        movsx  eax, byte ptr [esi + 1]
        inc    esi
loc_1_8FAA:
        cmp    eax, 46h
        jne    SHORT loc_1_8FB9
        mov    byte ptr [esp + 13h], 1
        movsx  eax, byte ptr [esi + 1]
        inc    esi
loc_1_8FB9:
        cmp    eax, 64h
        jg     SHORT loc_1_8FDF
        je     SHORT loc_1_9031
        cmp    eax, 25h
        je     SHORT loc_1_9008
        cmp    eax, 47h
        je     SHORT loc_1_9017
        cmp    eax, 50h
        je     SHORT loc_1_902D
        cmp    eax, 58h
        je     SHORT loc_1_902D
        cmp    eax, 63h
        je     NEAR PTR loc_1_9065
        jmp    SHORT loc_1_8FF7
loc_1_8FDF:
        cmp    eax, 67h
        je     SHORT loc_1_9017
        cmp    eax, 70h
        je     SHORT loc_1_902D
        cmp    eax, 73h
        je     NEAR PTR loc_1_907E
        cmp    eax, 78h
        je     SHORT loc_1_902D
loc_1_8FF7:
        push   25h
        mov    esi, edi
        push   dword ptr [esp + 28h]
        call   dword ptr [esp + 28h]
        add    esp, 8
        jmp    SHORT loc_1_9050
loc_1_9008:
        push   25h
        push   dword ptr [esp + 28h]
        call   dword ptr [esp + 28h]
        add    esp, 8
        jmp    SHORT loc_1_9050
loc_1_9017:
        or     bx, bx
        je     SHORT loc_1_9050
        mov    eax, dword ptr [esp + 2ch]
        movzx  ecx, bx
        lea    eax, [eax + ecx*4 - 4]
        mov    dword ptr [esp + 18h], eax
        jmp    SHORT loc_1_9050
loc_1_902D:
        mov    bp, 10h
loc_1_9031:
        add    dword ptr [esp + 18h], 4
        push   ecx
        push   ebx
        mov    eax, dword ptr [esp + 20h]
        push   ebp
        push   dword ptr [eax - 4]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        call   sub_1_8E59
        add    esp, 18h
loc_1_9050:
        movsx  edi, byte ptr [esi + 1]
        inc    esi
        or     edi, edi
        jne    NEAR PTR loc_1_8EE1
loc_1_905D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_9065:
        add    dword ptr [esp + 18h], 4
        mov    eax, dword ptr [esp + 18h]
        push   dword ptr [eax - 4]
        push   dword ptr [esp + 28h]
        call   dword ptr [esp + 28h]
        add    esp, 8
        jmp    SHORT loc_1_9050
loc_1_907E:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_9085
loc_1_9085:
        add    dword ptr [esp + 18h], 4
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [esp + 12h], 0
        mov    eax, dword ptr [eax - 4]
        mov    dword ptr [esp + 14h], eax
        jne    SHORT loc_1_90B0
        mov    edi, dword ptr [esp + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx - 1]
        jmp    SHORT loc_1_90BB
loc_1_90B0:
        mov    eax, dword ptr [esp + 14h]
        inc    dword ptr [esp + 14h]
        movzx  edi, byte ptr [eax]
loc_1_90BB:
        or     dx, dx
        je     SHORT loc_1_90C9
        movzx  eax, dx
        cmp    edi, eax
        jbe    SHORT loc_1_90C9
        mov    edi, eax
loc_1_90C9:
        or     bx, bx
        je     SHORT loc_1_90FA
        cmp    byte ptr [esp + 11h], 0
        jne    SHORT loc_1_90FA
        mov    ax, bx
        dec    bx
        movzx  eax, ax
        cmp    eax, edi
        jbe    SHORT loc_1_90FA
loc_1_90E1:
        push   20h
        push   dword ptr [esp + 28h]
        call   dword ptr [esp + 28h]
        add    esp, 8
        mov    ax, bx
        dec    bx
        movzx  eax, ax
        cmp    eax, edi
        ja     SHORT loc_1_90E1
loc_1_90FA:
        sub    ebp, ebp
        cmp    edi, ebp
        je     SHORT loc_1_911B
        mov    ebp, edi
loc_1_9102:
        mov    eax, dword ptr [esp + 14h]
        inc    dword ptr [esp + 14h]
        mov    al, byte ptr [eax]
        push   eax
        push   dword ptr [esp + 28h]
        call   dword ptr [esp + 28h]
        add    esp, 8
        dec    edi
        jne    SHORT loc_1_9102
loc_1_911B:
        mov    eax, ebp
        inc    ebp
        movzx  edi, bx
        cmp    edi, eax
        jbe    NEAR PTR loc_1_9050
loc_1_9129:
        push   20h
        push   dword ptr [esp + 28h]
        call   dword ptr [esp + 28h]
        add    esp, 8
        mov    eax, ebp
        inc    ebp
        cmp    edi, eax
        ja     SHORT loc_1_9129
        jmp    NEAR PTR loc_1_9050
loc_1_9142:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    dl, byte ptr [esp + 8]
        mov    byte ptr [ecx], dl
        inc    dword ptr [eax]
        ret
sub_1_9151:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esp + 10h]
        lea    eax, [esp + 0ch]
        push   dword ptr [esp + 10h]
        push   eax
        db  068h, 042h, 091h, 000h, 000h
        call   sub_1_8EC3
        mov    eax, dword ptr [esp + 18h]
        add    esp, 10h
        mov    byte ptr [eax], 0
        mov    eax, esi
        pop    esi
        ret
        db  056h, 08Dh, 044h, 024h, 010h, 050h, 08Bh, 074h, 024h, 00Ch, 0FFh, 074h, 024h, 010h, 056h, 0E8h
        db  0C2h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 08Bh, 0C6h, 05Eh, 0C3h
loc_1_9196:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [edx + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [edx]
        or     eax, 10000000h
        mov    dword ptr [ebp - 0ch], eax
        db  0A1h, 098h, 0B1h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        mov    ebx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 0ch]
        or     eax, 1000h
        mov    ecx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        push   edx
        call   sub_1_4390
        add    esp, 4
        db  0FFh, 00Dh, 068h, 0B8h, 000h, 000h
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_91EB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        db  0FFh, 005h, 068h, 0B8h, 000h, 000h
        db  083h, 03Dh, 068h, 0B8h, 000h, 000h, 005h
        jbe    SHORT loc_1_9203
        jmp    SHORT loc_1_9276
loc_1_9203:
        push   200h
        push   408h
        call   sub_1_4364
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_921D
        jmp    SHORT loc_1_9276
loc_1_921D:
        lea    eax, [ebp + 10h]
        push   eax
        push   dword ptr [ebp + 0ch]
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_9151
        add    esp, 0ch
        db  0A1h, 084h, 0B1h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        mov    dword ptr [ebp - 4], 1
        mov    dword ptr [ebp - 0ch], esi
        db  0C7h, 045h, 0F8h, 096h, 091h, 000h, 000h
        push   dword ptr [ebp]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_927C
        push   esi
        call   sub_1_4390
        add    esp, 4
loc_1_9276:
        db  0FFh, 00Dh, 068h, 0B8h, 000h, 000h
loc_1_927C:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 0CDh, 020h, 085h, 080h, 001h, 000h
sub_1_928A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 14h]
        db  0CDh, 020h, 013h, 000h, 02Ah, 000h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_92A7:
        db  0CDh, 020h, 0FFh, 080h, 001h, 000h
sub_1_92AD:
        db  0CDh, 020h, 000h, 081h, 001h, 000h
sub_1_92B3:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        pop    edi
        ret
sub_1_92BE:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 004h, 001h, 001h, 000h
        pop    esi
        ret
sub_1_92CB:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 012h, 001h, 001h, 000h
        pop    esi
        ret
sub_1_92D8:
        mov    ecx, dword ptr [esp + 4]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_1_92E6
        xor    eax, eax
loc_1_92E6:
        ret
sub_1_92E7:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 026h, 080h, 001h, 000h
sub_1_92F1:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 028h, 080h, 001h, 000h
sub_1_92FB:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, 21h
        db  0CDh, 020h, 027h, 080h, 001h, 000h
sub_1_930A:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, 31h
        db  0CDh, 020h, 027h, 080h, 001h, 000h
sub_1_9319:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        shl    ecx, 2
        sub    esp, ecx
        mov    eax, esp
        push   esi
        push   edi
        mov    edi, eax
        mov    esi, dword ptr [ebp + 10h]
        cld
        shr    ecx, 2
        rep movsd
        pop    edi
        pop    esi
        call   dword ptr [ebp + 8]
        mov    esp, ebp
        pop    ebp
        ret
loc_1_933C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        lea    eax, [esp - 14h]
        mov    edx, dword ptr [ebp + 8]
        push   edx
        push   eax
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    edx, dword ptr [ebp + 30h]
        db  0CDh, 020h, 08Bh, 000h, 001h, 000h
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0CDh, 020h, 08Ch, 000h, 001h, 000h
        push   edx
        push   eax
        call   sub_1_2131
        add    esp, 10h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_939A:
        db  003h, 03Dh, 090h, 0B8h, 000h, 000h
        mov    edi, dword ptr [edi]
        db  089h, 03Dh, 094h, 0B8h, 000h, 000h
        ret
NWSERVER_DESTROY_THREAD:
        clc
        ret
sub_1_93AB:
        db  0CDh, 020h, 069h, 080h, 040h, 000h
sub_1_93B1:
        xor    eax, eax
        db  0CDh, 020h, 061h, 081h, 001h, 000h
        mov    eax, eax
sub_1_93BC:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    dword ptr [ebp + 18h], ecx
        mov    dword ptr [ebp + 14h], ecx
        mov    dword ptr [ebp + 10h], ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_9407
        mov    dword ptr [ebp + 14h], eax
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_9407
        mov    dword ptr [ebp + 18h], eax
        mov    dword ptr [ebp], ecx
        mov    dword ptr [ebp + 4], ecx
        mov    dword ptr [ebp + 8], ecx
        mov    dword ptr [ebp + 0ch], ecx
        inc    ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_9407
        mov    dword ptr [ebp + 10h], eax
        mov    eax, 1
        pop    ebp
        ret
loc_1_9407:
        pop    ebp
sub_1_9408:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        xor    edx, edx
        mov    eax, dword ptr [ebp + 14h]
        or     eax, eax
        je     SHORT loc_1_941C
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_1_941C:
        mov    eax, dword ptr [ebp + 18h]
        or     eax, eax
        je     SHORT loc_1_9429
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_1_9429:
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_9436
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_1_9436:
        xor    eax, eax
        pop    ebp
        ret
        db  08Bh, 0C0h
sub_1_943C:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, 31h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    edx, dword ptr [ebp + 4]
        or     edx, dword ptr [ebp + 0ch]
        jne    SHORT loc_1_9469
        inc    dword ptr [ebp]
loc_1_945A:
        cmp    dword ptr [ebp + 8], edx
        jne    SHORT loc_1_947F
        mov    eax, dword ptr [ebp + 10h]
        pop    ebp
        db  0CDh, 020h, 028h, 080h, 001h, 000h
loc_1_9469:
        inc    dword ptr [ebp + 8]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 14h]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        xor    edx, edx
        jmp    SHORT loc_1_945A
loc_1_947F:
        dec    dword ptr [ebp + 8]
        inc    dword ptr [ebp]
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        db  0CDh, 020h, 028h, 080h, 001h, 000h
        nop
sub_1_9490:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, 31h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        dec    dword ptr [ebp]
        jne    SHORT loc_1_94AE
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_94B5
loc_1_94AE:
        pop    ebp
        db  0CDh, 020h, 028h, 080h, 001h, 000h
loc_1_94B5:
        dec    dword ptr [ebp + 0ch]
        inc    dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp + 18h]
        pop    ebp
        db  0CDh, 020h, 028h, 080h, 001h, 000h
        mov    eax, eax
sub_1_94C8:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, 31h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    edx, dword ptr [ebp]
        or     edx, dword ptr [ebp + 4]
        je     SHORT loc_1_94FF
        inc    dword ptr [ebp + 0ch]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 18h]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        pop    ebp
        db  0CDh, 020h, 028h, 080h, 001h, 000h
loc_1_94FF:
        inc    dword ptr [ebp + 4]
        pop    ebp
        db  0CDh, 020h, 028h, 080h, 001h, 000h
        mov    eax, eax
sub_1_950C:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, 31h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        dec    dword ptr [ebp + 4]
        xor    edx, edx
        cmp    edx, dword ptr [ebp + 0ch]
        jne    SHORT loc_1_953E
        cmp    edx, dword ptr [ebp + 8]
        je     SHORT loc_1_954E
        dec    dword ptr [ebp + 8]
        inc    dword ptr [ebp]
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        db  0CDh, 020h, 028h, 080h, 001h, 000h
loc_1_953E:
        dec    dword ptr [ebp + 0ch]
        inc    dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp + 18h]
        pop    ebp
        db  0CDh, 020h, 028h, 080h, 001h, 000h
loc_1_954E:
        pop    ebp
        db  0CDh, 020h, 028h, 080h, 001h, 000h
        add    byte ptr [eax], al
        add    bh, bh
        loop   loc_1_955B
loc_1_955B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi + 6ah], dl
        add    al, ch
        db  070h, 0FDh
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 0A3h, 0CCh, 0B8h, 000h, 000h, 068h, 061h, 096h, 000h, 000h, 068h
        db  0BCh, 0B8h, 000h, 000h, 06Ah, 000h, 0E8h, 091h, 09Bh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 0BFh, 0D0h
        db  0B8h, 000h, 000h, 02Bh, 0C0h, 0B9h, 00Dh, 000h, 000h, 000h, 0F3h, 0ABh, 0C7h, 005h, 0D4h, 0B8h
        db  000h, 000h, 019h, 097h, 000h, 000h, 066h, 0A1h, 0BCh, 0B8h, 000h, 000h, 066h, 0A3h, 0DAh, 0B8h
        db  000h, 000h, 066h, 0C7h, 005h, 0F2h, 0B8h, 000h, 000h, 001h, 000h, 0C7h, 005h, 0F4h, 0B8h, 000h
        db  000h, 098h, 0B8h, 000h, 000h, 066h, 0C7h, 005h, 0F8h, 0B8h, 000h, 000h, 020h, 000h, 066h, 0C7h
        db  005h, 09Ah, 0B8h, 000h, 000h, 020h, 000h, 0C6h, 005h, 09Dh, 0B8h, 000h, 000h, 000h, 0E8h, 02Bh
        db  001h, 000h, 000h, 0B8h, 001h, 000h, 000h, 000h, 05Fh, 0C3h
sub_1_95E0:
        db  0FFh, 035h, 0C0h, 0B8h, 000h, 000h
        call   sub_1_425C
        add    esp, 4
        db  0C7h, 005h, 0C0h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 0C4h, 0B8h, 000h, 000h
        call   sub_1_425C
        add    esp, 4
        db  0C7h, 005h, 0C4h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 0B8h, 0B8h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_962A
        dec    eax
        db  0A3h, 0B8h, 0B8h, 000h, 000h
        db  0A1h, 03Ch, 0B4h, 000h, 000h
        dec    eax
        db  0A3h, 03Ch, 0B4h, 000h, 000h
loc_1_962A:
        db  0FFh, 035h, 0C8h, 0B8h, 000h, 000h
        call   sub_1_425C
        add    esp, 4
        db  0C7h, 005h, 0C8h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Fh, 0B7h, 005h, 0BCh, 0B8h, 000h, 000h
        push   eax
        call   sub_1_315A
        add    esp, 4
        db  0FFh, 035h, 0CCh, 0B8h, 000h, 000h
        call   sub_1_92E7
        add    esp, 4
        ret
loc_1_9661:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        movzx  edx, byte ptr [esi + 1eh]
        mov    al, byte ptr [esi + 1fh]
        cmp    al, 59h
        je     SHORT loc_1_9675
        cmp    al, 4eh
        jne    SHORT loc_1_9694
loc_1_9675:
        mov    ecx, esi
        call   sub_1_1979
        or     eax, eax
        je     SHORT loc_1_9694
        cmp    byte ptr [esi + 1fh], 4eh
        jne    SHORT loc_1_968A
        mov    byte ptr [eax + 22h], 10h
loc_1_968A:
        mov    byte ptr [eax + 22h], 0
        mov    cl, byte ptr [eax + 21h]
        mov    byte ptr [eax + 23h], cl
loc_1_9694:
        sub    eax, eax
        pop    esi
        ret
loc_1_9698:
        db  0C7h, 005h, 0C4h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 0B8h, 0B8h, 000h, 000h
        inc    eax
        db  0A3h, 0B8h, 0B8h, 000h, 000h
        db  0A1h, 03Ch, 0B4h, 000h, 000h
        inc    eax
        db  0A3h, 03Ch, 0B4h, 000h, 000h
        ret
sub_1_96B9:
        mov    eax, dword ptr [esp + 4]
        db  009h, 005h, 00Ch, 0B9h, 000h, 000h
        db  083h, 03Dh, 010h, 0B9h, 000h, 000h, 000h
        jne    SHORT loc_1_9703
        db  0C7h, 005h, 010h, 0B9h, 000h, 000h, 001h, 000h, 000h, 000h
        db  083h, 03Dh, 0C4h, 0B8h, 000h, 000h, 000h
        jne    SHORT loc_1_96F6
        push   0ea60h
        db  068h, 098h, 096h, 000h, 000h
        call   sub_1_4244
        add    esp, 8
        db  0A3h, 0C4h, 0B8h, 000h, 000h
loc_1_96F6:
        db  068h, 004h, 0B9h, 000h, 000h
        call   sub_1_6D70
        add    esp, 4
loc_1_9703:
        ret
loc_1_9704:
        db  0C7h, 005h, 0C0h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        push   1
        call   sub_1_96B9
        add    esp, 4
        ret
loc_1_9719:
        db  0FFh, 035h, 0CCh, 0B8h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
        ret
sub_1_9728:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        db  068h, 0ECh, 0B8h, 000h, 000h
        lea    esi, [ebx + 28h]
        push   esi
        call   sub_1_316F
        db  0BFh, 09Eh, 0B8h, 000h, 000h
        movsd
        movsd
        movsd
        db  066h, 0A1h, 0A8h, 0B8h, 000h, 000h
        add    esp, 8
        push   eax
        call   sub_1_3A60
        add    esp, 4
        inc    ax
        push   eax
        call   sub_1_3A60
        add    esp, 4
        db  066h, 0A3h, 0A8h, 0B8h, 000h, 000h
        mov    al, byte ptr [ebx + 24h]
        db  0A2h, 0B6h, 0B8h, 000h, 000h
        db  0C6h, 005h, 0B7h, 0B8h, 000h, 000h, 03Fh
        db  068h, 0D0h, 0B8h, 000h, 000h
        call   sub_1_318D
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_9788:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        db  068h, 0ECh, 0B8h, 000h, 000h
        lea    esi, [ebx + 28h]
        push   esi
        call   sub_1_316F
        db  0BFh, 09Eh, 0B8h, 000h, 000h
        movsd
        movsd
        movsd
        db  066h, 0A1h, 0A8h, 0B8h, 000h, 000h
        add    esp, 8
        push   eax
        call   sub_1_3A60
        add    esp, 4
        add    ax, 2
        push   eax
        call   sub_1_3A60
        add    esp, 4
        db  066h, 0A3h, 0A8h, 0B8h, 000h, 000h
        mov    al, byte ptr [ebx + 24h]
        db  0A2h, 0B6h, 0B8h, 000h, 000h
        db  0C6h, 005h, 0B7h, 0B8h, 000h, 000h, 021h
        db  068h, 0D0h, 0B8h, 000h, 000h
        call   sub_1_318D
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_97EA:
        db  083h, 03Dh, 0C8h, 0B8h, 000h, 000h, 000h
        jne    SHORT loc_1_9819
        db  083h, 03Dh, 014h, 0B9h, 000h, 000h, 000h
        jne    SHORT loc_1_9819
        db  083h, 03Dh, 094h, 0B0h, 000h, 000h, 000h
        je     SHORT loc_1_9819
        db  0C7h, 005h, 014h, 0B9h, 000h, 000h, 001h, 000h, 000h, 000h
        push   4
        call   sub_1_96B9
        add    esp, 4
loc_1_9819:
        ret
loc_1_981A:
        db  0C7h, 005h, 0C8h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR sub_1_97EA
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_9830:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 24h]
        push   dword ptr [eax + 1ch]
        call   sub_1_3A68
        mov    ecx, dword ptr [esp + 2ch]
        add    esp, 4
        mov    edi, dword ptr [ecx + 28h]
        add    edi, eax
        mov    eax, dword ptr [esp + 24h]
        mov    ax, word ptr [eax + 20h]
        push   eax
        call   sub_1_3A60
        movzx  esi, ax
        mov    ebx, dword ptr [esp + 34h]
        add    esp, 4
        lea    eax, [ebx + 0bah]
        mov    dword ptr [ebx + 24h], eax
        mov    eax, dword ptr [esp + 24h]
        cmp    dword ptr [eax + 1ch], 0
        jne    SHORT loc_1_9885
        sub    esi, 18h
        mov    word ptr [ebx + 28h], 3ch
        jmp    SHORT loc_1_988E
loc_1_9885:
        sub    edi, 18h
        mov    word ptr [ebx + 28h], 24h
loc_1_988E:
        mov    ecx, dword ptr [esp + 28h]
        lea    eax, [edi + esi - 1]
        mov    ebp, dword ptr [ecx]
        cmp    ecx, ebp
        je     SHORT loc_1_98B7
loc_1_989C:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        lea    edx, [ecx + edx - 1]
        cmp    ecx, eax
        ja     SHORT loc_1_98AE
        cmp    edx, edi
        jae    SHORT loc_1_98D0
loc_1_98AE:
        mov    ebp, dword ptr [ebp]
        cmp    dword ptr [esp + 28h], ebp
        jne    SHORT loc_1_989C
loc_1_98B7:
        db  0FFh, 005h, 074h, 0B1h, 000h, 000h
        push   ebx
        call   sub_1_2811
        add    esp, 4
        sub    eax, eax
loc_1_98C8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_98D0:
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [eax], 1eh
        mov    eax, edi
        and    eax, 0fffh
        mov    dword ptr [esp + 1ch], eax
        mov    ebp, edi
        and    ebp, 0fffff000h
        mov    dword ptr [ebx + 90h], edi
        mov    dword ptr [ebx + 94h], esi
        mov    dword ptr [esp + 18h], 1
        mov    dword ptr [ebx + 98h], 0
        lea    eax, [esp + 13h]
        push   eax
        mov    edx, ebp
        mov    ecx, dword ptr [esp + 2ch]
        call   sub_1_48AF
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_992F
        jmp    NEAR PTR loc_1_99E1
loc_1_992F:
        add    ebp, 1000h
        or     eax, eax
        je     SHORT loc_1_9948
        mov    ecx, dword ptr [eax + 14h]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [ebx + 98h], eax
        jmp    SHORT loc_1_994E
loc_1_9948:
        db  08Bh, 00Dh, 0C8h, 013h, 000h, 000h
loc_1_994E:
        add    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 1ch]
        add    eax, esi
        cmp    eax, 1000h
        mov    eax, esi
        jbe    SHORT loc_1_996A
        mov    eax, 1000h
        sub    eax, dword ptr [esp + 1ch]
loc_1_996A:
        mov    dword ptr [ebx + 2ah], ecx
        mov    word ptr [ebx + 2eh], ax
        sub    esi, eax
        mov    dword ptr [esp + 18h], 2
        je     SHORT loc_1_99E7
        lea    edi, [ebx + 30h]
loc_1_9980:
        lea    eax, [esp + 13h]
        push   eax
        mov    edx, ebp
        mov    ecx, dword ptr [esp + 2ch]
        call   sub_1_48AF
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_99E1
        add    ebp, 1000h
        or     eax, eax
        je     SHORT loc_1_99B7
        mov    ecx, dword ptr [eax + 14h]
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_99BD
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [ebx + 98h], eax
        jmp    SHORT loc_1_99BD
loc_1_99B7:
        db  08Bh, 00Dh, 0C8h, 013h, 000h, 000h
loc_1_99BD:
        cmp    esi, 1000h
        mov    eax, 1000h
        ja     SHORT loc_1_99CC
        mov    eax, esi
loc_1_99CC:
        mov    dword ptr [edi], ecx
        inc    dword ptr [esp + 18h]
        mov    word ptr [edi + 4], ax
        sub    esi, eax
        add    edi, 6
        or     esi, esi
        jne    SHORT loc_1_9980
        jmp    SHORT loc_1_99E7
loc_1_99E1:
        sub    dword ptr [ebx + 94h], esi
loc_1_99E7:
        mov    ax, word ptr [esp + 18h]
        cmp    dword ptr [esp + 14h], 0
        mov    word ptr [ebx + 22h], ax
        jne    SHORT loc_1_9A0B
        mov    eax, dword ptr [esp + 24h]
        test   byte ptr [eax + 2], 10h
        je     NEAR PTR loc_1_98B7
        mov    word ptr [ebx + 22h], 1
loc_1_9A0B:
        db  0C7h, 043h, 004h, 03Ch, 052h, 000h, 000h
        mov    eax, ebx
        jmp    NEAR PTR loc_1_98C8
        db  000h, 000h, 000h
loc_1_9A1C:
        push   edi
        dec    ecx
        dec    esi
        inc    esp
        dec    edi
        push   edi
        push   ebx
        pop    edi
        push   eax
        inc    ecx
        push   ebx
        push   ebx
        push   esp
        dec    eax
        push   edx
        push   ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9A30:
        dec    ebp
        imul   esp, dword ptr [ebx + 72h], 666f736fh
        je     SHORT loc_1_9A3A
loc_1_9A3A:
        push   edi
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        cmp    dword ptr [2e347620h], esi
        xor    byte ptr [eax], dh
        cmp    dword ptr cs:[634f0030h], esi
        je     SHORT loc_1_9A73
        xor    dword ptr [ecx], edi
        and    byte ptr [ecx], dh
        cmp    dword ptr [ecx], edi
        xor    eax, 29432800h
        and    byte ptr [ebx + 6fh], al
        jo     SHORT loc_1_9ADC
        db  072h, 069h
        push   39312074h
        cmp    dword ptr [63694d20h], esi
        jb     SHORT loc_1_9AE2
loc_1_9A73:
        jae    SHORT loc_1_9AE4
        db  066h, 074h, 020h
        inc    ebx
        outsd
        jb     SHORT loc_1_9AEC
        or     eax, 6c6c410ah
        and    byte ptr [edx + 69h], dl
        push   52207374h
        db  065h, 073h, 065h
        jb     SHORT loc_1_9B06
        add    byte ptr cs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [edx], al
        add    dword ptr [eax], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
loc_1_9ADC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9AE2:
        add    byte ptr [eax], al
loc_1_9AE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9AEC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B06:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B18:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_9B1C:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_9B20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B28:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B44:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9D44:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9D48:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9F48:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9F4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9F50:
        add    al, ch
        add    byte ptr [eax], al
loc_1_9F54:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9F58:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax + 41h], dl
        push   ebx
        push   ebx
        push   edi
        dec    edi
        push   edx
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_9F6C:
        dec    esi
        inc    ebp
        push   esp
        pop    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        db  000h
loc_1_9F78:
        dec    esi
        inc    ebp
        push   esp
        pop    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [esi + 45h], cl
        push   esp
        pop    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        db  000h
loc_1_9F90:
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    edi
        inc    ebp
        push   ecx
        push   ebp
        inc    ecx
        dec    esp
        push   ebx
        db  000h
loc_1_9FA0:
        inc    edi
        push   edx
        dec    edi
        push   ebp
        push   eax
        push   ebx
        pop    edi
        dec    ecx
        daa
        dec    ebp
        pop    edi
        dec    ecx
        dec    esi
        add    byte ptr [eax], al
        db  000h
loc_1_9FB0:
        dec    esi
        inc    ebp
        push   esp
        pop    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 005h, 055h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_9FEC:
        in     al, dx
        lahf
        add    byte ptr [eax], al
        in     al, dx
        lahf
        add    byte ptr [eax], al
loc_1_9FF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9FF8:
        add    cl, byte ptr [eax + eax]
        db  000h, 093h, 008h, 000h, 000h, 0B5h
        or     byte ptr [eax], al
        db  000h, 002h
        or     al, 0
        db  000h, 002h
        or     al, 0
        db  000h, 016h
        or     eax, dword ptr [eax]
        db  000h, 002h
        or     al, 0
        db  000h, 002h
        or     al, 0
        db  000h, 002h
        or     al, 0
        db  000h, 002h
        or     al, 0
        db  000h, 002h
        or     al, 0
        db  000h, 002h
        or     al, 0
        db  000h, 002h
        or     al, 0
        db  000h, 0B5h, 00Bh, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [edx], eax
        add    eax, dword ptr [eax + 9080706h]
        or     cl, byte ptr [ebx]
        or     al, 0dh
        push   cs
        movsx  ebp, word ptr [esi + 15145caah]
        push   ss
        pop    ss
        sbb    byte ptr [ecx], bl
        sbb    bl, byte ptr [ebx]
        sbb    al, 1dh
        push   ds
        pop    ds
        and    byte ptr [ecx], ah
        and    ah, byte ptr [ebx]
        and    al, 25h
        daa
        sub    byte ptr [ecx], ch
        sub    ch, byte ptr [ebx]
        sub    al, 2dh
        das
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        cmp    bh, byte ptr [ebx]
        cmp    al, 3dh
        aas
        inc    eax
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        inc    edi
        dec    eax
        dec    ecx
        dec    edx
        dec    ebx
        dec    esp
        dec    ebp
        dec    esi
        dec    edi
        push   eax
        push   ecx
        push   edx
        push   ebx
        push   esp
        push   ebp
        push   esi
        push   edi
        pop    eax
        pop    ecx
        pop    edx
        pop    ebx
        pop    esp
        pop    ebp
        pop    esi
        pop    edi
        pushad
        popad
        bound  esp, qword ptr [ebx + 64h]
        push   6a69h
        imul   ebp, dword ptr [ebp + ebp*2 + 6eh], 6fh
        jo     SHORT loc_1_A11B
        jb     SHORT loc_1_A11F
        je     SHORT loc_1_A123
        jbe    SHORT loc_1_A127
        js     SHORT loc_1_A12B
        jp     SHORT loc_1_A12F
        jl     SHORT loc_1_A133
        jle    SHORT loc_1_A137
        add    byte ptr [ecx - 7a7b7c7eh], 86h
        xchg   dword ptr [eax - 73747577h], ecx
        lea    ecx, [esi - 6d6e6f71h]
        xchg   ebx, eax
        xchg   esp, eax
        xchg   ebp, eax
        xchg   esi, eax
        xchg   edi, eax
        cwde
        cdq
        lcall  0a09fh, 9e9d9c9bh
        mov    eax, dword ptr [0a5a4a3a2h]
        cmpsb
        cmpsd
        test   al, 0a9h
        stosb
        stosd
        lodsb
        lodsd
        scasb
        scasd
        mov    al, 0b1h
        mov    dl, 0b3h
        mov    ah, 0b5h
        mov    dh, 0b7h
        mov    eax, 0bcbbbab9h
        mov    ebp, 0c1c0bfbeh
        ret    0c4c3h
        db  0C5h, 0C6h, 0C7h, 0C8h
loc_1_A101:
        leave
        retf   0cccbh
        db  0CDh, 0CEh, 0CFh, 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h, 0D8h
loc_1_A111:
        fstpnce st(2), st(0)
        fcmovnu st(0), st(4)
        fstp   st(6)
        fnstsw ax
        db  0E1h, 0E2h
loc_1_A11B:
        jecxz  loc_1_A101
        in     eax, 0e6h
loc_1_A11F:
        out    0e8h, eax
        db  0E9h, 0EAh
loc_1_A123:
        jmp    SHORT loc_1_A111
        db  0EDh, 0EEh
loc_1_A127:
        out    dx, eax
        db  0F0h, 0F1h, 0F2h
loc_1_A12B:
        hlt
        cmc
        db  0F6h
loc_1_A12F:
        idiv   eax
        stc
        cli
loc_1_A133:
        sti
        cld
        std
        db  0FEh
loc_1_A137:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_A170:
        add    eax, 49474f4ch
        dec    esi
        add    byte ptr [eax], al
loc_1_A178:
        das
        pop    esp
        cmp    bh, byte ptr [ebx]
        sub    al, 2ah
        aas
        sub    ebp, dword ptr [3c5d5b3dh]
        and    ch, byte ptr ds:[esi]
        daa
        pushad
        pop    esi
        add    byte ptr [edx], al
        add    eax, 0ff000000h
        add    byte ptr [eax], al
loc_1_A194:
        add    al, byte ptr [8a000000h]
        add    byte ptr [eax], al
loc_1_A19C:
        add    ch, byte ptr [esi]
        add    byte ptr cs:[esi + 57h], cl
        push   ebx
        pop    ecx
        push   ebx
        push   esi
        dec    edi
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_A1AC:
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr [ebx + 59h], dl
        push   ebx
        db  000h
loc_1_A1B4:
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr [ecx + 3ah], al
        pop    esp
        add    byte ptr [ecx + 24h], al
        add    byte ptr [eax], al
loc_1_A1C0:
        inc    ecx
        inc    esp
        dec    ebp
        dec    ecx
        dec    esi
        and    al, 0
        add    byte ptr [ecx + 24h], al
        add    byte ptr [eax], al
loc_1_A1CC:
        inc    ecx
        inc    esp
        dec    ebp
        dec    ecx
        dec    esi
        and    al, 0
        add    byte ptr [ebx + 59h], dl
        push   ebx
        add    byte ptr [ebx + 59h], dl
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_A1FC:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_A200:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_A204:
        add    byte ptr fs:[eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 09Ch, 081h, 094h, 088h, 0FFh, 096h, 0FFh, 0FFh, 0FFh, 094h, 0FFh, 0FFh, 098h, 0FFh
        db  09Ah, 0FFh, 094h, 098h, 083h, 0FFh, 083h, 0FFh, 0FFh, 0FFh, 0E7h, 0D0h, 083h, 083h, 0FFh, 080h
        db  0A2h, 0FFh, 0FFh, 001h, 0FFh, 0FFh, 001h
_LTEXT ends
        end
