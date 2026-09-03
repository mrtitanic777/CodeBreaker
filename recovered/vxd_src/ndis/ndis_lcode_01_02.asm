; ndis_lcode_01_02.asm -- NDIS object 1 segment _LTEXT  (part 2/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_3DE0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, 8
        mov    esi, dword ptr [esp + 18h]
        push   ebp
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        xor    ebp, ebp
loc_1_3DF8:
        lea    eax, [esp + 10h]
        push   edi
        push   ebp
        push   10h
        push   eax
        call   sub_1_7450
        cmp    dword ptr [esp + 10h], ebp
        je     SHORT loc_1_3E62
        lea    eax, [esp + 14h]
        push   edi
        push   ebp
        push   400h
        push   eax
        call   sub_1_7450
        cmp    dword ptr [esp + 14h], ebp
        je     SHORT loc_1_3E79
        mov    eax, dword ptr [esp + 10h]
        dec    ebx
        mov    dword ptr [eax + 4], 400h
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [eax + 8], edx
        mov    edx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], edx
        mov    edx, dword ptr [esi + 20h]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 0ch], edx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 20h], eax
        jne    SHORT loc_1_3DF8
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    4
loc_1_3E62:
        sub    ebx, 8
        pop    ebp
        pop    edi
        cmp    ebx, 1
        sbb    eax, eax
        pop    esi
        and    eax, 0c0000001h
        pop    ebx
        add    esp, 8
        ret    4
loc_1_3E79:
        mov    eax, dword ptr [esp + 10h]
        push   0
        push   10h
        sub    ebx, 8
        push   eax
        call   sub_1_74E0
        pop    ebp
        cmp    ebx, 1
        sbb    eax, eax
        pop    edi
        and    eax, 0c0000001h
        pop    esi
        pop    ebx
        add    esp, 8
        ret    4
        db  08Bh, 0FFh
sub_1_3EA0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, 2
        mov    esi, dword ptr [esp + 14h]
        push   ebp
        db  08Bh, 02Dh, 000h, 000h, 000h, 000h
loc_1_3EB6:
        lea    eax, [esp + 10h]
        push   ebp
        push   0
        push   5ch
        push   eax
        call   sub_1_7450
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_3F06
        mov    edi, dword ptr [esp + 10h]
        xor    eax, eax
        mov    ecx, 17h
        rep stosd
        mov    eax, dword ptr [esp + 10h]
        add    eax, 20h
        push   eax
        call   sub_1_6C50
        mov    edx, dword ptr [esi + 24h]
        mov    ecx, dword ptr [esp + 10h]
        dec    ebx
        mov    dword ptr [ecx + 0ch], edx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [esi + 24h], ecx
        jne    SHORT loc_1_3EB6
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
loc_1_3F06:
        sub    ebx, 8
        pop    ebp
        pop    edi
        cmp    ebx, 1
        sbb    eax, eax
        pop    esi
        and    eax, 0c0000001h
        pop    ebx
        add    esp, 4
        ret    4
        db  08Dh, 049h, 000h
sub_1_3F20:
        push   esi
        xor    ecx, ecx
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 18h]
        mov    byte ptr [esi + 14h], cl
        mov    dword ptr [esi + 10h], ecx
        test   eax, eax
        je     SHORT loc_1_3F41
loc_1_3F34:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [eax + 8], ecx
        mov    eax, dword ptr [eax + 0ch]
        test   eax, eax
        jne    SHORT loc_1_3F34
loc_1_3F41:
        mov    eax, dword ptr [esi + 1ch]
        test   eax, eax
        je     SHORT loc_1_3F66
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [edx + 20h]
        mov    dword ptr [eax + 0ch], ecx
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [edx + 20h], eax
        mov    dword ptr [esi + 1ch], 0
        mov    dword ptr [esi + 18h], 0
loc_1_3F66:
        pop    esi
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3F70:
        sub    esp, 8
        lea    eax, [esp]
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        add    esi, 20h
        push   0
        push   eax
        push   0
        push   0
        push   esi
        call   sub_1_6CA0
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_1_3FBB
        xor    edi, edi
loc_1_3F96:
        lea    eax, [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        push   eax
        push   ecx
        call   sub_1_7150
        mov    ecx, dword ptr [esp + 8]
        push   ecx
        call   sub_1_6E40
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [esp + 8], ecx
        cmp    ecx, edi
        jne    SHORT loc_1_3F96
loc_1_3FBB:
        push   esi
        call   sub_1_6C50
        pop    edi
        pop    esi
        add    esp, 8
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3FD0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        push   edi
        call   sub_1_3F70
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   esi
        call   sub_1_3F20
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [esi + 24h], edi
        pop    edi
        pop    esi
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4000:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 20h]
        mov    ebx, dword ptr [edi + 18h]
        test   ebx, ebx
        je     SHORT loc_1_4050
        mov    esi, dword ptr [esp + 1ch]
        xor    ebp, ebp
loc_1_4018:
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx]
        sub    eax, dword ptr [ebx + 8]
        mov    edx, dword ptr [esi + 1ch]
        push   eax
        push   ecx
        lea    eax, [esp + 1ch]
        push   edx
        lea    ecx, [esp + 1ch]
        push   eax
        push   ecx
        call   sub_1_6D50
        cmp    dword ptr [esp + 10h], ebp
        jne    SHORT loc_1_405C
        mov    eax, dword ptr [esp + 14h]
        lea    ecx, [edi + 20h]
        push   eax
        push   ecx
        call   sub_1_7010
        mov    ebx, dword ptr [ebx + 0ch]
        test   ebx, ebx
        jne    SHORT loc_1_4018
loc_1_4050:
        mov    al, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    0ch
loc_1_405C:
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4070:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 30h]
        mov    dl, byte ptr [edi]
        cmp    dl, 0e8h
        jne    SHORT loc_1_40BE
        cmp    dword ptr [esp + 34h], 1fch
        jae    NEAR PTR loc_1_4350
        cmp    dword ptr [esp + 34h], 0
        je     NEAR PTR loc_1_4350
        mov    eax, dword ptr [esp + 34h]
        inc    edi
        dec    eax
        mov    ecx, dword ptr [esp + 2ch]
        mov    esi, dword ptr [esp + 28h]
        push   eax
        push   edi
        push   ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_45C0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    10h
loc_1_40BE:
        cmp    dword ptr [esp + 34h], 1f8h
        ja     NEAR PTR loc_1_4350
        cmp    dword ptr [esp + 34h], 0
        je     NEAR PTR loc_1_4350
        mov    ecx, dword ptr [esp + 2ch]
        inc    edi
        mov    al, byte ptr [ecx]
        mov    bl, byte ptr [ecx + 1]
        mov    byte ptr [esp + 18h], al
        mov    al, byte ptr [edi]
        mov    byte ptr [esp + 19h], bl
        mov    byte ptr [esp + 15h], al
        mov    byte ptr [esp + 1ah], dl
        cmp    al, 0ffh
        jne    SHORT loc_1_4106
        mov    al, byte ptr [edi + 4]
        add    edi, 4
        sub    dword ptr [esp + 34h], 4
        mov    byte ptr [esp + 15h], al
loc_1_4106:
        mov    al, byte ptr [esp + 15h]
        mov    cl, byte ptr [edi + 1]
        mov    byte ptr [esp + 1eh], al
        mov    byte ptr [esp + 16h], cl
        movzx  ax, byte ptr [edi + 2]
        shl    ax, 8
        add    edi, 3
        sub    dword ptr [esp + 34h], 4
        or     word ptr [esp + 16h], ax
        xor    ebp, ebp
        mov    cx, word ptr [esp + 16h]
        mov    eax, 1
        mov    word ptr [esp + 1ch], cx
        mov    esi, dword ptr [esp + 28h]
        mov    byte ptr [esp + 13h], al
        mov    byte ptr [esp + 14h], al
        mov    ebx, dword ptr [esi + 28h]
        test   ebx, ebx
        je     SHORT loc_1_41BA
loc_1_414F:
        mov    al, byte ptr [esp + 18h]
        cmp    byte ptr [ebx], al
        je     SHORT loc_1_4162
        mov    ebp, ebx
        mov    ebx, dword ptr [ebx + 0ch]
        test   ebx, ebx
        jne    SHORT loc_1_414F
        jmp    SHORT loc_1_41BA
loc_1_4162:
        mov    ax, word ptr [esp + 16h]
        cmp    word ptr [ebx + 4], ax
        jne    SHORT loc_1_41B3
        mov    al, byte ptr [esp + 19h]
        cmp    byte ptr [ebx + 1], al
        jne    SHORT loc_1_41B3
        mov    al, byte ptr [esp + 1ah]
        cmp    byte ptr [ebx + 2], al
        jne    SHORT loc_1_41B3
        xor    eax, eax
        xor    ecx, ecx
        mov    al, byte ptr [ebx + 8]
        mov    cl, byte ptr [esp + 15h]
        add    eax, eax
        cmp    eax, ecx
        jne    SHORT loc_1_41A6
        mov    byte ptr [esp + 13h], 0
        mov    al, byte ptr [ebx + 7]
        sub    al, cl
        cmp    al, 1
        sbb    al, al
        neg    al
        mov    byte ptr [esp + 14h], al
        jmp    SHORT loc_1_41BA
loc_1_41A6:
        mov    al, byte ptr [esp + 15h]
        cmp    byte ptr [ebx + 6], al
        je     NEAR PTR loc_1_4350
loc_1_41B3:
        push   ebx
        push   esi
        call   sub_1_3F20
loc_1_41BA:
        cmp    byte ptr [esp + 13h], 0
        je     NEAR PTR loc_1_424F
        cmp    byte ptr [esp + 15h], 0
        je     SHORT loc_1_41EB
        test   byte ptr [esp + 15h], 1
        je     NEAR PTR loc_1_4350
        mov    byte ptr [esp + 14h], 0
        mov    al, byte ptr [esp + 15h]
        inc    al
        mov    byte ptr [esp + 1fh], al
        mov    byte ptr [esp + 20h], 1
loc_1_41EB:
        test   ebx, ebx
        jne    SHORT loc_1_4220
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_1_4205
        push   esi
        call   sub_1_3EA0
        cmp    dword ptr [esi + 24h], 0
        je     NEAR PTR loc_1_4350
loc_1_4205:
        mov    ebx, dword ptr [esi + 24h]
        cmp    byte ptr [esp + 14h], 0
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 24h], eax
        jne    SHORT loc_1_4236
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [esi + 28h], ebx
        jmp    SHORT loc_1_4236
loc_1_4220:
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_1_4236
        mov    eax, dword ptr [ebx + 0ch]
        test   ebp, ebp
        jne    SHORT loc_1_4233
        mov    dword ptr [esi + 28h], eax
        jmp    SHORT loc_1_4236
loc_1_4233:
        mov    dword ptr [ebp + 0ch], eax
loc_1_4236:
        mov    edx, dword ptr [esp + 1ch]
        lea    ecx, [esp + 18h]
        mov    eax, dword ptr [ecx]
        mov    cx, word ptr [ecx + 8]
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], edx
        mov    word ptr [ebx + 8], cx
        jmp    SHORT loc_1_426F
loc_1_424F:
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_1_4265
        mov    eax, dword ptr [ebx + 0ch]
        test   ebp, ebp
        jne    SHORT loc_1_4262
        mov    dword ptr [esi + 28h], eax
        jmp    SHORT loc_1_4265
loc_1_4262:
        mov    dword ptr [ebp + 0ch], eax
loc_1_4265:
        mov    al, byte ptr [esp + 15h]
        inc    byte ptr [ebx + 8]
        mov    byte ptr [ebx + 6], al
loc_1_426F:
        mov    ebp, dword ptr [ebx + 1ch]
        test   ebp, ebp
        jne    SHORT loc_1_42AD
        cmp    dword ptr [esi + 20h], 0
        jne    SHORT loc_1_4297
        push   esi
        call   sub_1_3DE0
        test   eax, eax
        je     SHORT loc_1_4297
        push   ebx
        push   esi
        call   sub_1_3F20
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    10h
loc_1_4297:
        mov    ebp, dword ptr [esi + 20h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [ebx + 1ch], ebp
        mov    dword ptr [ebx + 18h], ebp
        mov    dword ptr [ebp + 0ch], 0
loc_1_42AD:
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [esp + 18h], eax
        cmp    dword ptr [ebp + 8], eax
        jae    SHORT loc_1_4318
loc_1_42BA:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 4]
        push   eax
        sub    ecx, eax
        push   edi
        add    ecx, dword ptr [ebp]
        push   ecx
        call   sub_1_64BC
        mov    eax, dword ptr [ebp + 8]
        add    edi, eax
        sub    dword ptr [esp + 18h], eax
        mov    dword ptr [ebp + 8], 0
        cmp    dword ptr [esi + 20h], 0
        jne    SHORT loc_1_42ED
        push   esi
        call   sub_1_3DE0
        test   eax, eax
        jne    SHORT loc_1_435A
loc_1_42ED:
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ebp + 0ch], eax
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [esp + 18h]
        mov    ebp, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp + 0ch], 0
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [eax + 0ch], ebp
        mov    dword ptr [ebx + 1ch], ebp
        cmp    dword ptr [ebp + 8], ecx
        jb     SHORT loc_1_42BA
loc_1_4318:
        mov    eax, dword ptr [esp + 18h]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp + 4]
        sub    eax, dword ptr [ebp + 8]
        add    eax, dword ptr [ebp]
        push   eax
        call   sub_1_64BC
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 34h]
        sub    dword ptr [ebp + 8], ecx
        add    dword ptr [ebx + 10h], edx
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_1_4350
        push   ebx
        push   esi
        call   sub_1_4370
        push   ebx
        push   esi
        call   sub_1_3FD0
loc_1_4350:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    10h
loc_1_435A:
        push   ebx
        push   esi
        call   sub_1_3F20
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
sub_1_4370:
        sub    esp, 8
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 18h]
        push   edi
        push   ebp
        push   1
        mov    ebp, dword ptr [esp + 20h]
        push   ebx
        cmp    byte ptr [ebx + 1], 1
        sbb    eax, eax
        push   ebp
        and    eax, 7
        inc    eax
        mov    dword ptr [esp + 1ch], eax
        call   sub_1_4000
        test   al, al
        je     NEAR PTR loc_1_4443
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        je     NEAR PTR loc_1_4443
loc_1_43A9:
        mov    edi, dword ptr [esi + 10h]
        mov    eax, dword ptr [esp + 10h]
        test   dword ptr [esi + 8], eax
        je     NEAR PTR loc_1_4439
        inc    dword ptr [esi + 0ch]
        mov    ecx, dword ptr [ebx + 18h]
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        mov    eax, dword ptr [ecx + 4]
        sub    eax, dword ptr [ecx + 8]
        lea    edx, [ebx + 20h]
        push   eax
        mov    ecx, dword ptr [ecx]
        push   ecx
        mov    eax, dword ptr [esi + 4]
        lea    ecx, [esp + 20h]
        push   3
        push   ebx
        push   edx
        push   eax
        push   ecx
        call   sub_1_8EB0
        mov    byte ptr [esi + 14h], 1
        mov    eax, dword ptr [esi + 0ch]
        dec    eax
        mov    dword ptr [esi + 0ch], eax
        jne    SHORT loc_1_4439
        cmp    dword ptr [ebp + 0ch], 0
        lea    ecx, [ebp + 0ch]
        je     SHORT loc_1_440D
loc_1_43F8:
        mov    eax, dword ptr [ecx]
        cmp    esi, eax
        je     SHORT loc_1_4408
        lea    ecx, [eax + 10h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_43F8
        jmp    SHORT loc_1_440D
loc_1_4408:
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ecx], eax
loc_1_440D:
        cmp    byte ptr [esi + 14h], 0
        je     SHORT loc_1_441C
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_8EF0
loc_1_441C:
        mov    eax, dword ptr [esi]
        push   eax
        call   dword ptr [ebp + 14h]
        mov    eax, 1
        mov    cl, byte ptr [esi + 15h]
        shl    eax, cl
        push   0
        or     dword ptr [ebp + 18h], eax
        push   1ch
        push   esi
        call   sub_1_74E0
loc_1_4439:
        mov    esi, edi
        test   edi, edi
        jne    NEAR PTR loc_1_43A9
loc_1_4443:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
        db  08Dh, 049h, 000h
sub_1_4450:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        lea    esi, [edi + 0ch]
        mov    ebx, dword ptr [esi]
        test   ebx, ebx
        je     SHORT loc_1_44C2
loc_1_4461:
        cmp    byte ptr [ebx + 14h], 0
        mov    ebp, dword ptr [ebx + 10h]
        je     SHORT loc_1_44BC
        mov    byte ptr [ebx + 14h], 0
        inc    dword ptr [ebx + 0ch]
        mov    eax, dword ptr [ebx + 4]
        push   eax
        call   sub_1_8EF0
        mov    eax, dword ptr [ebx + 0ch]
        dec    eax
        mov    dword ptr [ebx + 0ch], eax
        jne    SHORT loc_1_44BC
        mov    ecx, esi
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_449F
loc_1_448A:
        mov    eax, dword ptr [ecx]
        cmp    ebx, eax
        je     SHORT loc_1_449A
        lea    ecx, [eax + 10h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_448A
        jmp    SHORT loc_1_449F
loc_1_449A:
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [ecx], eax
loc_1_449F:
        mov    eax, dword ptr [ebx]
        push   eax
        call   dword ptr [edi + 14h]
        mov    eax, 1
        mov    cl, byte ptr [ebx + 15h]
        shl    eax, cl
        push   0
        or     dword ptr [edi + 18h], eax
        push   1ch
        push   ebx
        call   sub_1_74E0
loc_1_44BC:
        mov    ebx, ebp
        test   ebp, ebp
        jne    SHORT loc_1_4461
loc_1_44C2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 014h, 083h, 0ECh, 010h, 053h, 056h
        db  057h, 033h, 0F6h, 055h, 089h, 030h, 039h, 074h, 024h, 028h, 00Fh, 084h, 0CBh, 000h, 000h, 000h
        db  08Dh, 044h, 024h, 014h, 056h, 08Dh, 04Ch, 024h, 020h, 050h, 08Bh, 054h, 024h, 034h, 051h, 056h
        db  052h, 0E8h, 0A1h, 027h, 000h, 000h, 039h, 074h, 024h, 01Ch, 00Fh, 084h, 0ABh, 000h, 000h, 000h
        db  08Dh, 044h, 024h, 010h, 08Dh, 04Ch, 024h, 018h, 08Bh, 054h, 024h, 014h, 050h, 051h, 052h, 0E8h
        db  063h, 029h, 000h, 000h, 08Bh, 05Ch, 024h, 024h, 039h, 074h, 024h, 028h, 00Fh, 084h, 083h, 000h
        db  000h, 000h, 08Bh, 07Ch, 024h, 030h, 083h, 07Ch, 024h, 010h, 000h, 075h, 02Ch, 08Dh, 044h, 024h
        db  014h, 08Bh, 04Ch, 024h, 014h, 050h, 051h, 0E8h, 00Bh, 02Ch, 000h, 000h, 083h, 07Ch, 024h, 014h
        db  000h, 074h, 062h, 08Dh, 044h, 024h, 010h, 08Dh, 04Ch, 024h, 018h, 08Bh, 054h, 024h, 014h, 050h
        db  051h, 052h, 0E8h, 020h, 029h, 000h, 000h, 0EBh, 046h, 085h, 0FFh, 074h, 01Eh, 03Bh, 07Ch, 024h
        db  010h, 076h, 00Eh, 02Bh, 07Ch, 024h, 010h, 0C7h, 044h, 024h, 010h, 000h, 000h, 000h, 000h, 0EBh
        db  02Eh, 001h, 07Ch, 024h, 018h, 029h, 07Ch, 024h, 010h, 033h, 0FFh, 08Bh, 06Ch, 024h, 028h, 08Bh
        db  044h, 024h, 010h, 02Bh, 0EEh, 03Bh, 0E8h, 072h, 002h, 08Bh, 0E8h, 08Bh, 044h, 024h, 018h, 055h
        db  053h, 003h, 0F5h, 050h, 003h, 0DDh, 0E8h, 018h, 01Fh, 000h, 000h, 029h, 06Ch, 024h, 010h, 039h
        db  074h, 024h, 028h, 077h, 081h, 08Bh, 044h, 024h, 034h, 089h, 030h, 05Dh, 05Fh, 05Eh, 05Bh, 083h
        db  0C4h, 010h, 0C2h, 014h, 000h, 08Bh, 0FFh
sub_1_45C0:
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 10h]
        sub    esp, 8
        cmp    eax, 0eh
        mov    dword ptr [esp], edx
        mov    dword ptr [esp + 4], eax
        ja     SHORT loc_1_45FC
        push   0
        push   0
        mov    ecx, dword ptr [esp + 14h]
        push   0
        push   eax
        push   edx
        lea    eax, [esp + 14h]
        push   edx
        push   eax
        mov    edx, dword ptr [ecx + 0d8h]
        push   edx
        call   sub_1_4CB0
        add    esp, 8
        ret    10h
loc_1_45FC:
        lea    ecx, [eax - 0eh]
        sub    eax, 0eh
        push   ecx
        push   eax
        lea    ecx, [edx + 0eh]
        mov    eax, dword ptr [esp + 14h]
        push   ecx
        push   0eh
        push   edx
        mov    ecx, dword ptr [eax + 0d8h]
        push   edx
        lea    edx, [esp + 18h]
        push   edx
        push   ecx
        call   sub_1_4CB0
        add    esp, 8
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4630:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 44h
        push   ebx
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [esp + 4], 0
        test   byte ptr [eax + 20h], 2
        push   esi
        push   edi
        je     SHORT loc_1_46A1
        mov    esi, dword ptr [ecx + 1d4h]
        test   esi, esi
        jne    SHORT loc_1_4698
        mov    esi, dword ptr [esp + 58h]
        xor    eax, eax
        mov    ecx, 0eh
        mov    edx, dword ptr [esi]
        mov    ebx, dword ptr [esi + 4]
        lea    esi, [esp + 14h]
        mov    edi, esi
        push   ebx
        rep stosd
        lea    ecx, [esp + 14h]
        push   edx
        stosb
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ecx
        call   sub_1_6D50
        mov    ecx, dword ptr [esp + 0ch]
        lea    edx, [esp + 14h]
        push   ecx
        push   edx
        call   sub_1_6FA0
        mov    eax, dword ptr [esp + 5ch]
        add    eax, 0eh
        jmp    SHORT loc_1_46A9
loc_1_4698:
        mov    eax, dword ptr [esp + 5ch]
        add    eax, 11h
        jmp    SHORT loc_1_46A9
loc_1_46A1:
        mov    esi, dword ptr [esp + 58h]
        mov    eax, dword ptr [esp + 5ch]
loc_1_46A9:
        mov    ecx, dword ptr [esp + 68h]
        mov    edx, dword ptr [esp + 64h]
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 68h]
        push   esi
        push   eax
        push   0
        push   edx
        call   sub_1_7170
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_46D2
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        call   sub_1_6E40
loc_1_46D2:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 44h
        ret    18h
        db  08Dh, 049h, 000h
loc_1_46E0:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 4]
        lea    esi, [edi + 2eh]
        cmp    byte ptr [esi], 0
        je     SHORT loc_1_46F6
        xor    bl, bl
        jmp    SHORT loc_1_4728
loc_1_46F6:
        mov    ebx, 1
        mov    byte ptr [esi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_4728
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_4728
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 029h, 008h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_4728:
        mov    edx, dword ptr [esp + 14h]
        inc    dword ptr [eax + 34h]
        lea    ecx, [edx + 2ch]
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [edi + 1a8h], 0
        jne    SHORT loc_1_474C
        mov    dword ptr [edi + 1a8h], edx
        jmp    SHORT loc_1_4755
loc_1_474C:
        mov    eax, dword ptr [edi + 1ach]
        mov    dword ptr [eax + 2ch], edx
loc_1_4755:
        mov    dword ptr [edi + 1ach], edx
        and    byte ptr [edx + 1dh], 0feh
        cmp    dword ptr [edi + 1b0h], 0
        jne    SHORT loc_1_476E
        mov    dword ptr [edi + 1b0h], edx
loc_1_476E:
        push   0
        mov    edx, 3
        push   0
        mov    ecx, edi
        call   dword ptr [edi + 11ch]
        test   bl, bl
        je     SHORT loc_1_47D4
        mov    ecx, edi
        call   dword ptr [edi + 118h]
        test   bl, bl
        je     SHORT loc_1_47D4
        mov    byte ptr [esi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_47C1
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_47C1
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 063h, 008h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_47C1:
        cmp    byte ptr [edi + 2ch], 1
        jne    SHORT loc_1_47D4
        mov    byte ptr [edi + 2ch], 0
        push   esi
        call   sub_1_689B
        add    esp, 4
loc_1_47D4:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  090h
sub_1_47E0:
        mov    eax, dword ptr [esp + 8]
        sub    esp, 14h
        test   byte ptr [eax + 20h], 2
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_1_47FB
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    0ch
loc_1_47FB:
        mov    edi, dword ptr [esp + 24h]
        cmp    dword ptr [edi + 178h], 0
        jne    SHORT loc_1_4820
        mov    dword ptr [edi + 1c0h], 0
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    0ch
loc_1_4820:
        lea    eax, [esp + 10h]
        push   0
        mov    ebx, dword ptr [esp + 30h]
        push   eax
        push   0
        push   0
        push   ebx
        call   sub_1_6CA0
        lea    ecx, [esp + 1ch]
        lea    eax, [esp + 0ch]
        mov    edx, dword ptr [esp + 10h]
        push   ecx
        push   eax
        push   edx
        call   sub_1_6E80
        mov    esi, dword ptr [edi + 178h]
        push   3
        mov    edx, dword ptr [edi + 174h]
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [edi + 178h], ecx
        lea    ecx, [esp + 1ch]
        mov    eax, dword ptr [esi]
        push   eax
        push   edx
        lea    eax, [esp + 20h]
        push   ecx
        push   eax
        call   sub_1_6D50
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_48A1
        mov    eax, dword ptr [edi + 178h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, 103h
        mov    dword ptr [edi + 1c0h], 0
        mov    dword ptr [edi + 178h], esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    0ch
loc_1_48A1:
        mov    eax, dword ptr [edi + 17ch]
        mov    dword ptr [esi + 0ch], eax
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [edi + 17ch], esi
        push   ecx
        push   ebx
        call   sub_1_6FA0
        mov    cl, byte ptr [edi + 171h]
        mov    edx, dword ptr [esi]
        mov    byte ptr [edx], cl
        mov    eax, dword ptr [esp + 0ch]
        test   byte ptr [eax], 1
        je     SHORT loc_1_48D6
        mov    eax, dword ptr [esi]
        mov    byte ptr [eax + 1], 0
        jmp    SHORT loc_1_48E2
loc_1_48D6:
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esi]
        mov    cl, byte ptr [eax + 5]
        mov    byte ptr [edx + 1], cl
loc_1_48E2:
        mov    eax, dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax + 2], 0e8h
        add    esp, 14h
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4900:
        mov    ecx, dword ptr [esp + 8]
        sub    esp, 0ch
        push   esi
        mov    eax, dword ptr [ecx + 30h]
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_1_4987
        lea    eax, [esp + 8]
        push   eax
        push   ecx
        call   sub_1_7080
        lea    eax, [esp + 0ch]
        lea    ecx, [esp + 4]
        mov    edx, dword ptr [esp + 8]
        push   eax
        push   ecx
        push   edx
        call   sub_1_6E80
        mov    ecx, dword ptr [esp + 8]
        push   ecx
        call   sub_1_6E40
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 17ch]
        cmp    dword ptr [edx], eax
        mov    eax, dword ptr [edx + 0ch]
        jne    SHORT loc_1_4957
        mov    dword ptr [ecx + 17ch], eax
        jmp    SHORT loc_1_4978
loc_1_4957:
        mov    esi, dword ptr [esp + 4]
        cmp    dword ptr [eax], esi
        je     SHORT loc_1_496D
loc_1_495F:
        mov    edx, dword ptr [edx + 0ch]
        mov    esi, dword ptr [esp + 4]
        mov    eax, dword ptr [edx + 0ch]
        cmp    dword ptr [eax], esi
        jne    SHORT loc_1_495F
loc_1_496D:
        mov    eax, dword ptr [edx + 0ch]
        mov    esi, dword ptr [eax + 0ch]
        mov    dword ptr [edx + 0ch], esi
        mov    edx, eax
loc_1_4978:
        mov    eax, dword ptr [ecx + 178h]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [ecx + 178h], edx
loc_1_4987:
        pop    esi
        add    esp, 0ch
        ret    8
        db  08Bh, 0FFh
sub_1_4990:
        sub    esp, 20h
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esp + 8], edx
        push   ebx
        mov    dword ptr [esp + 8], ecx
        push   esi
        mov    dword ptr [esp + 18h], eax
        push   edi
        push   ebp
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [edx + 30h]
        test   byte ptr [eax + 20h], 2
        je     NEAR PTR loc_1_4A46
        lea    eax, [esp + 20h]
        mov    ecx, dword ptr [esp + 20h]
        push   eax
        push   ecx
        xor    ebx, ebx
        call   sub_1_7150
        mov    ecx, dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 14h]
        mov    byte ptr [esp + 13h], bl
        mov    esi, dword ptr [ecx + 4]
        mov    ecx, dword ptr [edx + 0d8h]
        mov    al, byte ptr [esi]
        test   al, 1
        mov    edi, dword ptr [ecx + 0ch]
        je     SHORT loc_1_4A20
        cmp    al, 0ffh
        jne    SHORT loc_1_49F5
        test   edi, 8
        je     SHORT loc_1_4A3A
        mov    bl, 1
        jmp    SHORT loc_1_4A3A
loc_1_49F5:
        test   edi, 4
        jne    SHORT loc_1_4A1C
        test   edi, 2
        je     SHORT loc_1_4A3A
        lea    eax, [esp + 28h]
        mov    edx, dword ptr [ecx + 4]
        push   eax
        mov    ecx, dword ptr [ecx + 24h]
        push   esi
        push   edx
        push   ecx
        call   sub_1_51D0
        test   al, al
        je     SHORT loc_1_4A3A
loc_1_4A1C:
        mov    bl, 1
        jmp    SHORT loc_1_4A3A
loc_1_4A20:
        mov    eax, dword ptr [esi + 2]
        cmp    dword ptr [ecx + 2eh], eax
        jne    SHORT loc_1_4A3A
        mov    ax, word ptr [esi]
        cmp    word ptr [ecx + 2ch], ax
        jne    SHORT loc_1_4A3A
        mov    ebx, 1
        mov    byte ptr [esp + 13h], bl
loc_1_4A3A:
        test   edi, 0a0h
        je     SHORT loc_1_4A7F
        mov    bl, 1
        jmp    SHORT loc_1_4A7F
loc_1_4A46:
        mov    al, byte ptr [esi + 1]
        mov    cl, byte ptr [esi]
        cmp    al, cl
        je     SHORT loc_1_4A69
        test   al, al
        jne    SHORT loc_1_4A65
        mov    edx, dword ptr [esp + 14h]
        mov    ebx, dword ptr [edx + 0e4h]
        mov    edx, dword ptr [ebx + 8]
        or     edx, 8
        jne    SHORT loc_1_4A69
loc_1_4A65:
        xor    bl, bl
        jmp    SHORT loc_1_4A6B
loc_1_4A69:
        mov    bl, 1
loc_1_4A6B:
        cmp    al, cl
        jne    SHORT loc_1_4A7A
        mov    ebx, 1
        mov    byte ptr [esp + 13h], bl
        jmp    SHORT loc_1_4A7F
loc_1_4A7A:
        mov    byte ptr [esp + 13h], 0
loc_1_4A7F:
        test   bl, bl
        je     NEAR PTR loc_1_4C70
        lea    eax, [esp + 24h]
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_6CA0
        mov    edx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [edx + 30h]
        add    ecx, 20h
        test   byte ptr [ecx], 2
        je     SHORT loc_1_4AB0
        sub    dword ptr [esp + 24h], 3
loc_1_4AB0:
        mov    eax, dword ptr [esp + 20h]
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 1ch], edx
        cmp    edx, 20eh
        jae    SHORT loc_1_4B03
        cmp    edx, dword ptr [esp + 24h]
        je     SHORT loc_1_4B03
        test   byte ptr [ecx], 2
        je     SHORT loc_1_4ADA
        mov    eax, 0eh
        mov    edi, 3
        jmp    SHORT loc_1_4AE1
loc_1_4ADA:
        mov    eax, 3
        xor    edi, edi
loc_1_4AE1:
        mov    edx, dword ptr [esp + 14h]
        lea    ecx, [esp + 1ch]
        push   ecx
        add    eax, dword ptr [edx + 1f0h]
        mov    esi, dword ptr [edx + 180h]
        push   esi
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   edi
        push   eax
        call   sub_1_8F70
loc_1_4B03:
        mov    eax, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [edx + 1d4h], eax
        mov    ecx, dword ptr [eax + 30h]
        test   byte ptr [ecx + 20h], 2
        je     SHORT loc_1_4B58
        mov    eax, dword ptr [esp + 24h]
        lea    ecx, [esi + 0eh]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 1ch]
        sub    eax, 0eh
        mov    ebx, dword ptr [esp + 18h]
        push   eax
        push   ecx
        push   0eh
        mov    eax, dword ptr [ebx + 0d8h]
        push   esi
        push   esi
        push   edx
        push   eax
        call   sub_1_4CB0
        mov    eax, dword ptr [ebx + 0d8h]
        push   eax
        call   sub_1_5170
        jmp    NEAR PTR loc_1_4C56
loc_1_4B58:
        mov    esi, dword ptr [esp + 24h]
        mov    ecx, 1f8h
        sub    esi, 3
        sub    edx, edx
        mov    dword ptr [esp + 2ch], 3
        mov    eax, esi
        div    ecx
        sub    edx, edx
        mov    byte ptr [esp + 12h], al
        mov    eax, esi
        div    ecx
        test   edx, edx
        je     SHORT loc_1_4B84
        inc    byte ptr [esp + 12h]
loc_1_4B84:
        xor    bl, bl
        cmp    byte ptr [esp + 12h], bl
        je     NEAR PTR loc_1_4C46
loc_1_4B90:
        mov    edx, dword ptr [esp + 14h]
        lea    ecx, [esp + 1ch]
        push   ecx
        mov    edi, dword ptr [edx + 180h]
        mov    edx, dword ptr [esp + 1ch]
        push   edi
        lea    eax, [edi + 2]
        push   3
        push   0
        add    edi, 3
        mov    dword ptr [esp + 38h], eax
        push   edx
        call   sub_1_8F70
        cmp    byte ptr [esp + 12h], 1
        jbe    SHORT loc_1_4BDA
        test   bl, bl
        jne    SHORT loc_1_4BCF
        mov    al, byte ptr [esp + 12h]
        add    al, al
        sub    al, 3
        mov    byte ptr [edi], al
        jmp    SHORT loc_1_4BDD
loc_1_4BCF:
        lea    eax, [ebx*2]
        mov    byte ptr [edi], al
        jmp    SHORT loc_1_4BDD
loc_1_4BDA:
        mov    byte ptr [edi], 0
loc_1_4BDD:
        mov    byte ptr [edi + 1], 0
        inc    edi
        inc    edi
        mov    ebp, 1f8h
        mov    byte ptr [edi], 0
        inc    edi
        cmp    esi, 1f8h
        ja     SHORT loc_1_4BF6
        mov    ebp, esi
loc_1_4BF6:
        lea    eax, [esp + 1ch]
        mov    ecx, dword ptr [esp + 2ch]
        mov    edx, dword ptr [esp + 18h]
        push   eax
        push   edi
        sub    esi, 1f8h
        push   ebp
        inc    bl
        push   ecx
        add    ebp, 4
        push   edx
        call   sub_1_8F70
        mov    ecx, dword ptr [esp + 28h]
        push   ebp
        mov    edx, dword ptr [esp + 18h]
        push   ecx
        mov    eax, dword ptr [edx + 180h]
        mov    ecx, dword ptr [edx + 0e4h]
        push   eax
        push   ecx
        call   sub_1_4070
        add    dword ptr [esp + 2ch], 1f8h
        cmp    bl, byte ptr [esp + 12h]
        jb     NEAR PTR loc_1_4B90
loc_1_4C46:
        mov    edx, dword ptr [esp + 14h]
        mov    eax, dword ptr [edx + 0e4h]
        push   eax
        call   sub_1_4450
loc_1_4C56:
        mov    edx, dword ptr [esp + 14h]
        mov    al, byte ptr [esp + 13h]
        pop    ebp
        pop    edi
        mov    dword ptr [edx + 1d4h], 0
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_4C70:
        mov    al, byte ptr [esp + 13h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_4C80:
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esp + 18h]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   ecx
        mov    ecx, dword ptr [esp + 18h]
        push   edx
        mov    edx, dword ptr [esp + 18h]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   ecx
        mov    ecx, dword ptr [esp + 18h]
        push   edx
        push   eax
        push   ecx
        call   sub_1_4CB0
        ret    20h
sub_1_4CB0:
        sub    esp, 0ch
        cmp    dword ptr [esp + 20h], 0eh
        push   ebx
        push   esi
        push   edi
        push   ebp
        jb     NEAR PTR loc_1_4D77
        xor    ebp, ebp
        cmp    dword ptr [esp + 3ch], ebp
        je     NEAR PTR loc_1_4D77
        mov    ecx, dword ptr [esp + 28h]
        mov    al, byte ptr [ecx]
        test   al, 1
        jne    NEAR PTR loc_1_4D69
        mov    esi, dword ptr [esp + 20h]
        test   byte ptr [esi + 0ch], 0a4h
        je     SHORT loc_1_4CFC
        mov    eax, dword ptr [esi + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_4CF7
        mov    ax, word ptr [ecx]
        cmp    word ptr [esi + 2ch], ax
        je     SHORT loc_1_4CFC
loc_1_4CF7:
        mov    ebp, 1
loc_1_4CFC:
        mov    edi, dword ptr [esi + 44h]
        test   edi, edi
        je     NEAR PTR loc_1_4E37
loc_1_4D07:
        test   byte ptr [edi + 8], 20h
        mov    ebx, dword ptr [edi + 1ch]
        jne    SHORT loc_1_4D14
        test   ebp, ebp
        jne    SHORT loc_1_4D59
loc_1_4D14:
        mov    eax, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [esp + 38h]
        mov    edx, dword ptr [esp + 34h]
        push   eax
        mov    eax, dword ptr [esp + 34h]
        push   ecx
        mov    ecx, dword ptr [esp + 34h]
        push   edx
        mov    edx, dword ptr [esp + 30h]
        push   eax
        push   ecx
        inc    dword ptr [edi + 0ch]
        push   edx
        mov    eax, dword ptr [edi + 4]
        lea    ecx, [esp + 30h]
        push   eax
        push   ecx
        call   sub_1_8EB0
        mov    byte ptr [edi + 10h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_4D59
        push   1
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4D59:
        mov    edi, ebx
        test   ebx, ebx
        jne    SHORT loc_1_4D07
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    20h
loc_1_4D69:
        sub    al, 0ffh
        cmp    al, 1
        sbb    edi, edi
        and    edi, 6
        add    edi, 2
        jmp    SHORT loc_1_4D7C
loc_1_4D77:
        mov    edi, 20h
loc_1_4D7C:
        mov    esi, dword ptr [esp + 20h]
        mov    ebx, dword ptr [esi + 48h]
        test   ebx, ebx
        je     NEAR PTR loc_1_4E37
loc_1_4D8B:
        mov    eax, dword ptr [ebx + 8]
        mov    ebp, dword ptr [ebx + 20h]
        test   al, 0a0h
        jne    SHORT loc_1_4DE8
        cmp    edi, 8
        jne    SHORT loc_1_4D9E
        test   al, 8
        jne    SHORT loc_1_4DE8
loc_1_4D9E:
        cmp    edi, 2
        jne    NEAR PTR loc_1_4E2D
        test   al, 4
        jne    SHORT loc_1_4DE8
        test   al, 2
        je     SHORT loc_1_4E2D
        lea    eax, [esp + 10h]
        mov    ecx, dword ptr [esp + 28h]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        mov    eax, dword ptr [esi + 24h]
        push   edx
        push   eax
        call   sub_1_51D0
        test   al, al
        je     SHORT loc_1_4E2D
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, 1
        mov    dword ptr [esp + 14h], ecx
        mov    cl, byte ptr [ebx + 11h]
        shl    edx, cl
        mov    ecx, dword ptr [esp + 14h]
        test   dword ptr [eax + ecx*4], edx
        je     SHORT loc_1_4E2D
loc_1_4DE8:
        mov    eax, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [esp + 38h]
        mov    edx, dword ptr [esp + 34h]
        push   eax
        mov    eax, dword ptr [esp + 34h]
        push   ecx
        mov    ecx, dword ptr [esp + 34h]
        push   edx
        mov    edx, dword ptr [esp + 30h]
        push   eax
        push   ecx
        inc    dword ptr [ebx + 0ch]
        push   edx
        mov    eax, dword ptr [ebx + 4]
        lea    ecx, [esp + 30h]
        push   eax
        push   ecx
        call   sub_1_8EB0
        mov    byte ptr [ebx + 10h], 1
        mov    eax, dword ptr [ebx + 0ch]
        dec    eax
        mov    dword ptr [ebx + 0ch], eax
        jne    SHORT loc_1_4E2D
        push   1
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4E2D:
        mov    ebx, ebp
        test   ebp, ebp
        jne    NEAR PTR loc_1_4D8B
loc_1_4E37:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    20h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4E50:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 2ch
        mov    ecx, dword ptr [esp + 34h]
        push   ebx
        mov    dword ptr [esp + 20h], ecx
        push   esi
        mov    dword ptr [esp + 20h], 0
        or     dword ptr [eax + 3ch], 1000h
        push   edi
        push   ebp
        mov    ebp, dword ptr [eax + 0d8h]
        mov    eax, dword ptr [esp + 48h]
        test   eax, eax
        je     NEAR PTR loc_1_5131
        mov    dword ptr [esp + 38h], eax
loc_1_4E88:
        mov    eax, dword ptr [esp + 2ch]
        mov    esi, dword ptr [eax]
        xor    eax, eax
        mov    ax, word ptr [esi + 1eh]
        lea    edx, [eax + esi]
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [eax + 4]
        mov    ebx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 14h], ecx
        mov    dword ptr [esp + 20h], ebx
        test   eax, eax
        je     SHORT loc_1_4EBB
loc_1_4EAE:
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax]
        add    dword ptr [esp + 20h], ecx
        test   eax, eax
        jne    SHORT loc_1_4EAE
loc_1_4EBB:
        cmp    dword ptr [edx + 1ch], 0c000009ah
        je     SHORT loc_1_4ED2
        mov    dword ptr [edx + 1ch], 0
        mov    byte ptr [esp + 13h], 0
        jmp    SHORT loc_1_4ED7
loc_1_4ED2:
        mov    byte ptr [esp + 13h], 1
loc_1_4ED7:
        mov    dword ptr [esi + 30h], 0
        cmp    dword ptr [esp + 20h], 0eh
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [esi + 2ch], eax
        mov    byte ptr [esi + 1ch], 0
        jb     NEAR PTR loc_1_5005
        mov    ecx, dword ptr [esp + 14h]
        mov    al, byte ptr [ecx]
        test   al, 1
        jne    NEAR PTR loc_1_4FF3
        mov    dword ptr [esp + 18h], 0
        test   byte ptr [ebp + 0ch], 0a4h
        je     SHORT loc_1_4F33
        mov    eax, dword ptr [ebp + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_4F2B
        mov    ax, word ptr [ecx]
        cmp    word ptr [ebp + 2ch], ax
        jne    SHORT loc_1_4F2B
        mov    dword ptr [esp + 18h], 0
        jmp    SHORT loc_1_4F33
loc_1_4F2B:
        mov    dword ptr [esp + 18h], 1
loc_1_4F33:
        mov    edi, dword ptr [ebp + 44h]
        test   edi, edi
        je     NEAR PTR loc_1_5122
loc_1_4F3E:
        test   byte ptr [edi + 8], 0a0h
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [esp + 1ch], eax
        jne    SHORT loc_1_4F56
        cmp    dword ptr [esp + 18h], 0
        jne    NEAR PTR loc_1_4FE2
loc_1_4F56:
        mov    edx, dword ptr [edi + 4]
        inc    dword ptr [esp + 28h]
        mov    byte ptr [edi + 10h], 1
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 8]
        and    eax, 0a0h
        cmp    eax, 1
        sbb    al, al
        inc    al
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_4F90
        test   al, al
        jne    SHORT loc_1_4F90
        mov    ecx, dword ptr [edx + 54h]
        test   ecx, ecx
        je     SHORT loc_1_4F90
        push   esi
        mov    eax, dword ptr [edx + 24h]
        push   eax
        call   ecx
        mov    ebx, eax
        jmp    SHORT loc_1_4FB6
loc_1_4F90:
        xor    ebx, ebx
        mov    eax, dword ptr [esp + 20h]
        sub    eax, 0eh
        lea    ecx, [esp + 34h]
        push   eax
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        add    eax, 0eh
        push   eax
        push   0eh
        mov    eax, dword ptr [esp + 24h]
        push   eax
        push   esi
        push   edx
        push   ecx
        call   sub_1_8EB0
loc_1_4FB6:
        test   ebx, ebx
        je     SHORT loc_1_4FD0
        xor    eax, eax
        mov    ax, word ptr [esi + 1eh]
        mov    byte ptr [esp + 13h], 1
        mov    dword ptr [eax + esi + 1ch], 103h
        add    dword ptr [esi + 30h], ebx
loc_1_4FD0:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_4FE2
        push   1
        push   edi
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4FE2:
        mov    edi, dword ptr [esp + 1ch]
        test   edi, edi
        jne    NEAR PTR loc_1_4F3E
        jmp    NEAR PTR loc_1_5122
loc_1_4FF3:
        sub    al, 0ffh
        cmp    al, 1
        sbb    eax, eax
        and    eax, 6
        add    eax, 2
        mov    dword ptr [esp + 24h], eax
        jmp    SHORT loc_1_500D
loc_1_5005:
        mov    dword ptr [esp + 24h], 20h
loc_1_500D:
        mov    edi, dword ptr [ebp + 48h]
        test   edi, edi
        je     NEAR PTR loc_1_5122
loc_1_5018:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 20h]
        mov    ebx, ecx
        mov    dword ptr [esp + 1ch], eax
        and    ebx, 0a0h
        jne    SHORT loc_1_5092
        cmp    dword ptr [esp + 24h], 8
        jne    SHORT loc_1_5038
        test   cl, 8
        jne    SHORT loc_1_5092
loc_1_5038:
        cmp    dword ptr [esp + 24h], 2
        jne    NEAR PTR loc_1_5116
        test   cl, 4
        jne    SHORT loc_1_5092
        test   cl, 2
        je     NEAR PTR loc_1_5116
        lea    eax, [esp + 30h]
        mov    ecx, dword ptr [esp + 14h]
        push   eax
        mov    edx, dword ptr [ebp + 4]
        push   ecx
        mov    eax, dword ptr [ebp + 24h]
        push   edx
        push   eax
        call   sub_1_51D0
        test   al, al
        je     NEAR PTR loc_1_5116
        mov    eax, dword ptr [esp + 30h]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, 1
        mov    dword ptr [esp + 18h], ecx
        mov    cl, byte ptr [edi + 11h]
        shl    edx, cl
        mov    ecx, dword ptr [esp + 18h]
        test   dword ptr [ecx + eax*4], edx
        je     NEAR PTR loc_1_5116
loc_1_5092:
        mov    eax, dword ptr [edi + 4]
        inc    dword ptr [esp + 28h]
        mov    byte ptr [edi + 10h], 1
        inc    dword ptr [edi + 0ch]
        cmp    ebx, 1
        sbb    cl, cl
        inc    cl
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_50C4
        test   cl, cl
        jne    SHORT loc_1_50C4
        mov    ecx, dword ptr [eax + 54h]
        test   ecx, ecx
        je     SHORT loc_1_50C4
        push   esi
        mov    eax, dword ptr [eax + 24h]
        push   eax
        call   ecx
        mov    ebx, eax
        jmp    SHORT loc_1_50EA
loc_1_50C4:
        xor    ebx, ebx
        mov    ecx, dword ptr [esp + 20h]
        sub    ecx, 0eh
        push   ecx
        push   ecx
        mov    ecx, dword ptr [esp + 1ch]
        add    ecx, 0eh
        push   ecx
        push   0eh
        mov    ecx, dword ptr [esp + 24h]
        push   ecx
        push   esi
        push   eax
        lea    eax, [esp + 50h]
        push   eax
        call   sub_1_8EB0
loc_1_50EA:
        test   ebx, ebx
        je     SHORT loc_1_5104
        xor    eax, eax
        mov    ax, word ptr [esi + 1eh]
        mov    byte ptr [esp + 13h], 1
        mov    dword ptr [eax + esi + 1ch], 103h
        add    dword ptr [esi + 30h], ebx
loc_1_5104:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_5116
        push   1
        push   edi
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5116:
        mov    edi, dword ptr [esp + 1ch]
        test   edi, edi
        jne    NEAR PTR loc_1_5018
loc_1_5122:
        add    dword ptr [esp + 2ch], 4
        dec    dword ptr [esp + 38h]
        jne    NEAR PTR loc_1_4E88
loc_1_5131:
        mov    eax, dword ptr [esp + 40h]
        and    dword ptr [eax + 3ch], 0ffffefffh
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_1_5149
        push   ebp
        call   sub_1_5170
loc_1_5149:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5160:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_5170
        ret    4
        db  08Dh, 049h, 000h
sub_1_5170:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    ebx, dword ptr [edi + 10h]
        test   ebx, ebx
        je     SHORT loc_1_51CA
loc_1_517E:
        cmp    byte ptr [ebx + 10h], 0
        mov    esi, dword ptr [ebx + 14h]
        je     SHORT loc_1_51C4
        mov    byte ptr [ebx + 10h], 0
        inc    dword ptr [ebx + 0ch]
        mov    eax, dword ptr [ebx + 4]
        push   eax
        call   sub_1_8EF0
        mov    eax, dword ptr [ebx + 0ch]
        dec    eax
        mov    dword ptr [ebx + 0ch], eax
        jne    SHORT loc_1_51C4
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_51A7:
        mov    eax, dword ptr [ebx]
        push   eax
        call   dword ptr [edi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [ebx + 11h]
        shl    eax, cl
        push   0
        or     dword ptr [edi + 28h], eax
        push   24h
        push   ebx
        call   sub_1_74E0
loc_1_51C4:
        mov    ebx, esi
        test   esi, esi
        jne    SHORT loc_1_517E
loc_1_51CA:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_1_51D0:
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    ebx, dword ptr [esp + 10h]
        push   ebp
        mov    edi, ebx
        shr    edi, 1
        test   ebx, ebx
        je     SHORT loc_1_5248
        dec    ebx
        cmp    ebx, edi
        jb     SHORT loc_1_5248
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 1ch]
loc_1_51F0:
        cmp    edi, esi
        jb     SHORT loc_1_5248
        lea    eax, [edi + edi*2]
        lea    ebp, [ecx + eax*2]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ebp + 2], eax
        jbe    SHORT loc_1_5209
        mov    eax, 1
        jmp    SHORT loc_1_5227
loc_1_5209:
        jae    SHORT loc_1_5212
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_5227
loc_1_5212:
        mov    ax, word ptr [ebp]
        mov    bp, word ptr [edx]
        cmp    ax, bp
        jbe    SHORT loc_1_5225
        mov    eax, 1
        jmp    SHORT loc_1_5227
loc_1_5225:
        sbb    eax, eax
loc_1_5227:
        test   eax, eax
        je     SHORT loc_1_5257
        jle    SHORT loc_1_5236
        test   edi, edi
        je     SHORT loc_1_5248
        lea    ebx, [edi - 1]
        jmp    SHORT loc_1_5239
loc_1_5236:
        lea    esi, [edi + 1]
loc_1_5239:
        mov    eax, ebx
        sub    eax, esi
        inc    eax
        shr    eax, 1
        lea    edi, [eax + esi]
        cmp    ebx, edi
        jae    SHORT loc_1_51F0
loc_1_5248:
        mov    eax, dword ptr [esp + 20h]
        pop    ebp
        mov    dword ptr [eax], edi
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_1_5257:
        mov    eax, dword ptr [esp + 20h]
        pop    ebp
        mov    dword ptr [eax], edi
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5270:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        sub    esp, 4
        mov    al, byte ptr [edx]
        push   ebx
        push   esi
        mov    esi, dword ptr [ecx + 0ch]
        xor    bl, bl
        test   al, 1
        je     SHORT loc_1_52BF
        cmp    al, 0ffh
        jne    SHORT loc_1_5296
        test   esi, 8
        je     SHORT loc_1_52D2
        jmp    SHORT loc_1_52D0
loc_1_5296:
        test   esi, 4
        jne    SHORT loc_1_52D0
        test   esi, 2
        je     SHORT loc_1_52D2
        lea    eax, [esp + 8]
        push   eax
        push   edx
        mov    edx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ecx + 24h]
        push   edx
        push   ecx
        call   sub_1_51D0
        test   al, al
        je     SHORT loc_1_52D2
        jmp    SHORT loc_1_52D0
loc_1_52BF:
        mov    eax, dword ptr [ecx + 2eh]
        cmp    dword ptr [edx + 2], eax
        jne    SHORT loc_1_52D2
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx + 2ch], ax
        jne    SHORT loc_1_52D2
loc_1_52D0:
        mov    bl, 1
loc_1_52D2:
        test   esi, 0a0h
        je     SHORT loc_1_52DC
        mov    bl, 1
loc_1_52DC:
        mov    al, bl
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_52F0:
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   ecx
        mov    ecx, dword ptr [esp + 14h]
        push   edx
        mov    edx, dword ptr [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   ecx
        push   edx
        push   eax
        call   sub_1_5320
        ret    1ch
        db  005h, 000h, 000h, 000h, 000h
sub_1_5320:
        sub    esp, 0ch
        mov    eax, dword ptr [esp + 18h]
        push   ebx
        mov    ebx, dword ptr [esp + 1ch]
        push   esi
        push   edi
        add    ebx, 2
        push   ebp
        add    eax, 8
        cmp    dword ptr [esp + 2ch], 0eh
        jb     NEAR PTR loc_1_547D
        xor    edx, edx
        cmp    dword ptr [esp + 38h], edx
        je     NEAR PTR loc_1_547D
        mov    cl, byte ptr [ebx]
        mov    byte ptr [esp + 14h], cl
        and    cl, 80h
        jne    NEAR PTR loc_1_53F5
        mov    edi, dword ptr [esp + 20h]
        mov    dword ptr [esp + 14h], edx
        test   dword ptr [edi + 10h], 20a0h
        je     SHORT loc_1_5385
        mov    eax, dword ptr [ebx + 2]
        cmp    dword ptr [edi + 1ah], eax
        jne    SHORT loc_1_537D
        mov    ax, word ptr [ebx]
        cmp    word ptr [edi + 18h], ax
        je     SHORT loc_1_5385
loc_1_537D:
        mov    dword ptr [esp + 14h], 1
loc_1_5385:
        mov    esi, dword ptr [edi + 44h]
        test   esi, esi
        je     NEAR PTR loc_1_554B
loc_1_5390:
        test   byte ptr [esi + 8], 20h
        mov    ebx, dword ptr [esi + 2ch]
        jne    SHORT loc_1_53A0
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_53E5
loc_1_53A0:
        mov    eax, dword ptr [esp + 38h]
        mov    ecx, dword ptr [esp + 34h]
        mov    edx, dword ptr [esp + 30h]
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   ecx
        mov    ecx, dword ptr [esp + 30h]
        push   edx
        mov    edx, dword ptr [esp + 30h]
        push   eax
        push   ecx
        inc    dword ptr [esi + 18h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        lea    ecx, [esp + 30h]
        push   eax
        push   ecx
        call   sub_1_8EB0
        mov    byte ptr [esi + 1eh], 1
        mov    eax, dword ptr [esi + 18h]
        dec    eax
        mov    dword ptr [esi + 18h], eax
        jne    SHORT loc_1_53E5
        push   1
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_53E5:
        mov    esi, ebx
        test   ebx, ebx
        jne    SHORT loc_1_5390
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    1ch
loc_1_53F5:
        mov    al, byte ptr [eax]
        mov    edx, dword ptr [esp + 28h]
        and    al, 80h
        mov    byte ptr [esp + 13h], al
        mov    al, byte ptr [edx + 1]
        and    al, 0fch
        cmp    al, 1
        sbb    al, al
        neg    al
        mov    byte ptr [esp + 12h], al
        test   cl, cl
        je     SHORT loc_1_5489
        mov    ax, word ptr [ebx]
        cmp    ax, 0ffffh
        je     SHORT loc_1_5423
        cmp    ax, 0c0h
        jne    SHORT loc_1_542E
loc_1_5423:
        cmp    dword ptr [ebx + 2], -1
        mov    eax, 1
        je     SHORT loc_1_5430
loc_1_542E:
        xor    eax, eax
loc_1_5430:
        test   al, al
        je     SHORT loc_1_543B
        mov    esi, 8
        jmp    SHORT loc_1_548D
loc_1_543B:
        mov    al, byte ptr [ebx + 2]
        and    al, byte ptr [esp + 14h]
        and    al, 80h
        cmp    al, 1
        sbb    esi, esi
        xor    ecx, ecx
        and    esi, 3000h
        mov    cl, byte ptr [ebx + 4]
        add    esi, 1000h
        xor    eax, eax
        mov    al, byte ptr [ebx + 2]
        shl    eax, 10h
        or     ecx, eax
        xor    eax, eax
        shl    ecx, 8
        mov    al, byte ptr [ebx + 3]
        shl    eax, 10h
        or     ecx, eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 5]
        or     ecx, eax
        mov    dword ptr [esp + 14h], ecx
        jmp    SHORT loc_1_548D
loc_1_547D:
        mov    byte ptr [esp + 13h], 0
        mov    esi, 20h
        jmp    SHORT loc_1_548D
loc_1_5489:
        mov    esi, dword ptr [esp + 18h]
loc_1_548D:
        mov    edi, dword ptr [esp + 20h]
        mov    ebx, dword ptr [edi + 48h]
        test   ebx, ebx
        je     NEAR PTR loc_1_554B
loc_1_549C:
        mov    ecx, dword ptr [ebx + 8]
        mov    edx, esi
        and    edx, ecx
        mov    ebp, dword ptr [ebx + 30h]
        test   cl, 0a0h
        jne    SHORT loc_1_54FC
        cmp    esi, 8
        jne    SHORT loc_1_54B5
        test   cl, 8
        jne    SHORT loc_1_54FC
loc_1_54B5:
        cmp    esi, 4000h
        jne    SHORT loc_1_54D0
        test   ch, 20h
        jne    SHORT loc_1_54FC
        test   ch, 40h
        je     SHORT loc_1_54D0
        mov    eax, dword ptr [esp + 14h]
        test   dword ptr [ebx + 0ch], eax
        jne    SHORT loc_1_54FC
loc_1_54D0:
        test   dh, 10h
        je     SHORT loc_1_54E4
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_1_54E4
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [edi + 8], eax
        je     SHORT loc_1_54FC
loc_1_54E4:
        test   cl, 10h
        je     SHORT loc_1_54F0
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_54FC
loc_1_54F0:
        test   ch, 80h
        je     SHORT loc_1_5541
        cmp    byte ptr [esp + 12h], 0
        je     SHORT loc_1_5541
loc_1_54FC:
        mov    eax, dword ptr [esp + 38h]
        mov    ecx, dword ptr [esp + 34h]
        mov    edx, dword ptr [esp + 30h]
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   ecx
        mov    ecx, dword ptr [esp + 30h]
        push   edx
        mov    edx, dword ptr [esp + 30h]
        push   eax
        push   ecx
        inc    dword ptr [ebx + 18h]
        push   edx
        mov    eax, dword ptr [ebx + 4]
        lea    ecx, [esp + 30h]
        push   eax
        push   ecx
        call   sub_1_8EB0
        mov    byte ptr [ebx + 1eh], 1
        mov    eax, dword ptr [ebx + 18h]
        dec    eax
        mov    dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_5541
        push   1
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5541:
        mov    ebx, ebp
        test   ebp, ebp
        jne    NEAR PTR loc_1_549C
loc_1_554B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5560:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 30h
        push   ebx
        mov    edx, dword ptr [eax + 0dch]
        mov    ecx, dword ptr [esp + 3ch]
        push   esi
        mov    dword ptr [esp + 0ch], edx
        push   edi
        mov    dword ptr [esp + 2ch], ecx
        push   ebp
        mov    dword ptr [esp + 2ch], 0
        or     dword ptr [eax + 3ch], 1000h
        mov    eax, dword ptr [esp + 4ch]
        test   eax, eax
        je     NEAR PTR loc_1_58D7
        mov    dword ptr [esp + 3ch], eax
loc_1_559C:
        mov    eax, dword ptr [esp + 30h]
        mov    edi, dword ptr [eax]
        xor    eax, eax
        mov    ax, word ptr [edi + 1eh]
        add    eax, edi
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [edi + 8]
        mov    ebp, dword ptr [eax + 4]
        mov    esi, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 28h], esi
        test   eax, eax
        je     SHORT loc_1_55CE
loc_1_55C1:
        mov    esi, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax]
        add    dword ptr [esp + 28h], esi
        test   eax, eax
        jne    SHORT loc_1_55C1
loc_1_55CE:
        mov    eax, dword ptr [esp + 20h]
        cmp    dword ptr [eax + 1ch], 0c000009ah
        je     SHORT loc_1_55E9
        mov    dword ptr [eax + 1ch], 0
        mov    byte ptr [esp + 13h], 0
        jmp    SHORT loc_1_55EE
loc_1_55E9:
        mov    byte ptr [esp + 13h], 1
loc_1_55EE:
        mov    eax, dword ptr [esp + 44h]
        lea    ecx, [ebp + 2]
        lea    edx, [ebp + 8]
        mov    esi, dword ptr [esp + 28h]
        mov    dword ptr [edi + 30h], 0
        mov    dword ptr [edi + 2ch], eax
        mov    eax, dword ptr [esp + 20h]
        mov    byte ptr [edi + 1ch], 0
        cmp    dword ptr [eax + 10h], esi
        ja     NEAR PTR loc_1_57AE
        mov    al, byte ptr [ecx]
        mov    byte ptr [esp + 18h], al
        and    al, 80h
        jne    NEAR PTR loc_1_5722
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [esp + 18h], 0
        test   dword ptr [edx + 10h], 20a0h
        je     SHORT loc_1_565D
        mov    eax, dword ptr [ecx + 2]
        cmp    dword ptr [edx + 1ah], eax
        jne    SHORT loc_1_5655
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 18h], ax
        jne    SHORT loc_1_5655
        mov    dword ptr [esp + 18h], 0
        jmp    SHORT loc_1_565D
loc_1_5655:
        mov    dword ptr [esp + 18h], 1
loc_1_565D:
        mov    edx, dword ptr [esp + 14h]
        mov    ebx, dword ptr [edx + 44h]
        test   ebx, ebx
        je     NEAR PTR loc_1_58C8
loc_1_566C:
        test   byte ptr [ebx + 8], 0a0h
        mov    eax, dword ptr [ebx + 2ch]
        mov    dword ptr [esp + 24h], eax
        jne    SHORT loc_1_5684
        cmp    dword ptr [esp + 18h], 0
        jne    NEAR PTR loc_1_5711
loc_1_5684:
        mov    ecx, dword ptr [ebx + 4]
        inc    dword ptr [esp + 2ch]
        mov    byte ptr [ebx + 1eh], 1
        inc    dword ptr [ebx + 18h]
        mov    eax, dword ptr [ebx + 8]
        and    eax, 0a0h
        cmp    eax, 1
        sbb    al, al
        inc    al
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_56BE
        test   al, al
        jne    SHORT loc_1_56BE
        mov    edx, dword ptr [ecx + 54h]
        test   edx, edx
        je     SHORT loc_1_56BE
        push   edi
        mov    eax, dword ptr [ecx + 24h]
        push   eax
        call   edx
        mov    esi, eax
        jmp    SHORT loc_1_56E1
loc_1_56BE:
        xor    esi, esi
        mov    eax, dword ptr [esp + 20h]
        mov    edx, dword ptr [eax + 10h]
        mov    eax, dword ptr [esp + 28h]
        sub    eax, edx
        push   eax
        push   eax
        lea    eax, [edx + ebp]
        push   eax
        push   edx
        push   ebp
        push   edi
        push   ecx
        lea    ecx, [esp + 54h]
        push   ecx
        call   sub_1_8EB0
loc_1_56E1:
        test   esi, esi
        je     SHORT loc_1_56FB
        xor    eax, eax
        mov    ax, word ptr [edi + 1eh]
        mov    byte ptr [esp + 13h], 1
        mov    dword ptr [eax + edi + 1ch], 103h
        add    dword ptr [edi + 30h], esi
loc_1_56FB:
        mov    eax, dword ptr [ebx + 18h]
        dec    eax
        mov    dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_5711
        mov    edx, dword ptr [esp + 14h]
        push   1
        push   ebx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5711:
        mov    ebx, dword ptr [esp + 24h]
        test   ebx, ebx
        jne    NEAR PTR loc_1_566C
        jmp    NEAR PTR loc_1_58C8
loc_1_5722:
        mov    dl, byte ptr [edx]
        and    dl, 80h
        mov    byte ptr [esp + 12h], dl
        mov    dl, byte ptr [ebp + 1]
        and    dl, 0fch
        cmp    dl, 1
        sbb    dl, dl
        neg    dl
        mov    byte ptr [esp + 11h], dl
        test   al, al
        je     SHORT loc_1_57BB
        mov    ax, word ptr [ecx]
        cmp    ax, 0ffffh
        je     SHORT loc_1_574F
        cmp    ax, 0c0h
        jne    SHORT loc_1_575A
loc_1_574F:
        cmp    dword ptr [ecx + 2], -1
        mov    eax, 1
        je     SHORT loc_1_575C
loc_1_575A:
        xor    eax, eax
loc_1_575C:
        test   al, al
        je     SHORT loc_1_576A
        mov    dword ptr [esp + 1ch], 8
        jmp    SHORT loc_1_57BB
loc_1_576A:
        mov    al, byte ptr [ecx + 2]
        and    al, byte ptr [esp + 18h]
        and    al, 80h
        cmp    al, 1
        sbb    eax, eax
        xor    edx, edx
        and    eax, 3000h
        mov    dl, byte ptr [ecx + 4]
        add    eax, 1000h
        mov    dword ptr [esp + 1ch], eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 2]
        shl    eax, 10h
        or     edx, eax
        xor    eax, eax
        shl    edx, 8
        mov    al, byte ptr [ecx + 3]
        shl    eax, 10h
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 5]
        or     edx, eax
        mov    dword ptr [esp + 34h], edx
        jmp    SHORT loc_1_57BB
loc_1_57AE:
        mov    dword ptr [esp + 1ch], 20h
        mov    byte ptr [esp + 12h], 0
loc_1_57BB:
        mov    edx, dword ptr [esp + 14h]
        mov    ebx, dword ptr [edx + 48h]
        test   ebx, ebx
        je     NEAR PTR loc_1_58C8
loc_1_57CA:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 30h]
        mov    edx, ecx
        mov    dword ptr [esp + 24h], eax
        and    edx, dword ptr [esp + 1ch]
        mov    eax, ecx
        and    eax, 0a0h
        jne    SHORT loc_1_5837
        cmp    dword ptr [esp + 1ch], 8
        jne    SHORT loc_1_57EF
        test   cl, 8
        jne    SHORT loc_1_5837
loc_1_57EF:
        cmp    dword ptr [esp + 1ch], 4000h
        jne    SHORT loc_1_580C
        test   ch, 20h
        jne    SHORT loc_1_5837
        test   ch, 40h
        je     SHORT loc_1_580C
        mov    esi, dword ptr [esp + 34h]
        test   dword ptr [ebx + 0ch], esi
        jne    SHORT loc_1_5837
loc_1_580C:
        test   dh, 10h
        je     SHORT loc_1_5817
        cmp    byte ptr [ebx + 1dh], 0
        jne    SHORT loc_1_5837
loc_1_5817:
        test   cl, 10h
        je     SHORT loc_1_5823
        cmp    byte ptr [esp + 12h], 0
        jne    SHORT loc_1_5837
loc_1_5823:
        test   ch, 80h
        je     NEAR PTR loc_1_58BC
        cmp    byte ptr [esp + 11h], 0
        je     NEAR PTR loc_1_58BC
loc_1_5837:
        mov    ecx, dword ptr [ebx + 4]
        inc    dword ptr [esp + 2ch]
        mov    byte ptr [ebx + 1eh], 1
        inc    dword ptr [ebx + 18h]
        cmp    eax, 1
        sbb    al, al
        inc    al
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_5869
        test   al, al
        jne    SHORT loc_1_5869
        mov    edx, dword ptr [ecx + 54h]
        test   edx, edx
        je     SHORT loc_1_5869
        push   edi
        mov    eax, dword ptr [ecx + 24h]
        push   eax
        call   edx
        mov    esi, eax
        jmp    SHORT loc_1_588C
loc_1_5869:
        xor    esi, esi
        mov    eax, dword ptr [esp + 20h]
        mov    edx, dword ptr [eax + 10h]
        mov    eax, dword ptr [esp + 28h]
        sub    eax, edx
        push   eax
        push   eax
        lea    eax, [edx + ebp]
        push   eax
        push   edx
        push   ebp
        push   edi
        push   ecx
        lea    ecx, [esp + 54h]
        push   ecx
        call   sub_1_8EB0
loc_1_588C:
        test   esi, esi
        je     SHORT loc_1_58A6
        xor    eax, eax
        mov    ax, word ptr [edi + 1eh]
        mov    byte ptr [esp + 13h], 1
        mov    dword ptr [eax + edi + 1ch], 103h
        add    dword ptr [edi + 30h], esi
loc_1_58A6:
        mov    eax, dword ptr [ebx + 18h]
        dec    eax
        mov    dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_58BC
        mov    edx, dword ptr [esp + 14h]
        push   1
        push   ebx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_58BC:
        mov    ebx, dword ptr [esp + 24h]
        test   ebx, ebx
        jne    NEAR PTR loc_1_57CA
loc_1_58C8:
        add    dword ptr [esp + 30h], 4
        dec    dword ptr [esp + 3ch]
        jne    NEAR PTR loc_1_559C
loc_1_58D7:
        mov    eax, dword ptr [esp + 44h]
        and    dword ptr [eax + 3ch], 0ffffefffh
        cmp    dword ptr [esp + 2ch], 0
        je     SHORT loc_1_58F3
        mov    edx, dword ptr [esp + 14h]
        push   edx
        call   sub_1_5910
loc_1_58F3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_5900:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_5910
        ret    4
        db  08Dh, 049h, 000h
sub_1_5910:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0ch]
        push   edi
        mov    esi, dword ptr [ebx + 14h]
        test   esi, esi
        je     SHORT loc_1_5966
loc_1_591E:
        inc    dword ptr [esi + 18h]
        cmp    byte ptr [esi + 1eh], 0
        mov    edi, dword ptr [esi + 10h]
        je     SHORT loc_1_5937
        mov    byte ptr [esi + 1eh], 0
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_8EF0
loc_1_5937:
        dec    dword ptr [esi + 18h]
        jne    SHORT loc_1_5960
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5943:
        mov    eax, dword ptr [esi]
        push   eax
        call   dword ptr [ebx + 2ch]
        mov    eax, 1
        mov    cl, byte ptr [esi + 1ch]
        shl    eax, cl
        push   0
        or     dword ptr [ebx + 30h], eax
        push   34h
        push   esi
        call   sub_1_74E0
loc_1_5960:
        mov    esi, edi
        test   edi, edi
        jne    SHORT loc_1_591E
loc_1_5966:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_5970:
        sub    esp, 4
        xor    al, al
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        xor    edx, edx
        xor    ecx, ecx
        mov    dl, byte ptr [esi + 4]
        mov    cl, byte ptr [esi + 2]
        shl    ecx, 10h
        or     edx, ecx
        xor    ecx, ecx
        shl    edx, 8
        mov    cl, byte ptr [esi + 3]
        shl    ecx, 10h
        mov    bl, byte ptr [esi]
        or     edx, ecx
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 5]
        or     edx, ecx
        mov    cl, byte ptr [esi + 2]
        and    cl, bl
        test   cl, 80h
        mov    ecx, dword ptr [esp + 14h]
        je     SHORT loc_1_59C2
        cmp    dword ptr [ecx + 8], edx
        jne    SHORT loc_1_59C2
        cmp    dword ptr [ecx + 0ch], 0
        je     SHORT loc_1_59C2
        mov    al, 1
        mov    edi, dword ptr [esp + 0ch]
        jmp    SHORT loc_1_5A21
loc_1_59C2:
        mov    edi, dword ptr [ecx + 10h]
        mov    edx, dword ptr [esp + 1ch]
        test   byte ptr [edx], 80h
        je     SHORT loc_1_59D6
        test   edi, 10h
        jne    SHORT loc_1_5A1F
loc_1_59D6:
        test   bl, 80h
        je     SHORT loc_1_5A0E
        mov    cx, word ptr [esi]
        cmp    cx, 0ffffh
        je     SHORT loc_1_59EC
        cmp    cx, 0c0h
        jne    SHORT loc_1_59F4
loc_1_59EC:
        cmp    dword ptr [esi + 2], -1
        mov    cl, 1
        je     SHORT loc_1_59F6
loc_1_59F4:
        xor    cl, cl
loc_1_59F6:
        test   cl, cl
        je     SHORT loc_1_5A04
        test   edi, 8
        je     SHORT loc_1_5A21
        jmp    SHORT loc_1_5A1F
loc_1_5A04:
        test   edi, 6000h
        je     SHORT loc_1_5A21
        jmp    SHORT loc_1_5A1F
loc_1_5A0E:
        mov    edx, dword ptr [esi + 2]
        cmp    dword ptr [ecx + 1ah], edx
        jne    SHORT loc_1_5A21
        mov    dx, word ptr [esi]
        cmp    word ptr [ecx + 18h], dx
        jne    SHORT loc_1_5A21
loc_1_5A1F:
        mov    al, 1
loc_1_5A21:
        test   edi, 0a0h
        je     SHORT loc_1_5A2B
        mov    al, 1
loc_1_5A2B:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5A40:
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   ecx
        mov    ecx, dword ptr [esp + 1ch]
        push   edx
        mov    edx, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   ecx
        mov    ecx, dword ptr [esp + 1ch]
        push   edx
        mov    edx, dword ptr [esp + 1ch]
        push   eax
        push   ecx
        push   edx
        call   sub_1_5A80
        ret    24h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5A80:
        mov    eax, dword ptr [esp + 10h]
        sub    esp, 0ch
        add    eax, eax
        push   ebx
        cmp    eax, dword ptr [esp + 28h]
        push   esi
        push   edi
        push   ebp
        jae    NEAR PTR loc_1_5B9E
        cmp    dword ptr [esp + 40h], 0
        je     NEAR PTR loc_1_5B9E
        xor    cl, cl
        mov    eax, dword ptr [esp + 30h]
        mov    al, byte ptr [eax]
        and    al, 0f0h
        cmp    al, 40h
        je     SHORT loc_1_5ABB
        mov    ecx, dword ptr [esp + 28h]
        mov    cl, byte ptr [ecx]
        not    cl
        and    cl, 1
loc_1_5ABB:
        test   cl, cl
        je     NEAR PTR loc_1_5B7B
        xor    bl, bl
        mov    esi, dword ptr [esp + 20h]
        test   byte ptr [esi + 14h], 0a4h
        je     SHORT loc_1_5B0E
        cmp    dword ptr [esp + 2ch], 6
        lea    eax, [esi + 3ch]
        je     SHORT loc_1_5ADC
        lea    eax, [esi + 42h]
loc_1_5ADC:
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [esp + 28h]
        cmp    word ptr [ecx], ax
        jne    SHORT loc_1_5B0C
        cmp    dword ptr [esp + 2ch], 2
        je     SHORT loc_1_5B08
        cmp    dword ptr [esp + 2ch], 6
        lea    eax, [esi + 3ch]
        je     SHORT loc_1_5AFC
        lea    eax, [esi + 42h]
loc_1_5AFC:
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [esp + 28h]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_5B0C
loc_1_5B08:
        xor    bl, bl
        jmp    SHORT loc_1_5B0E
loc_1_5B0C:
        mov    bl, 1
loc_1_5B0E:
        mov    edi, dword ptr [esi + 60h]
        test   edi, edi
        je     NEAR PTR loc_1_5CBA
loc_1_5B19:
        test   byte ptr [edi + 8], 20h
        mov    ebp, dword ptr [edi + 1ch]
        jne    SHORT loc_1_5B26
        test   bl, bl
        jne    SHORT loc_1_5B6B
loc_1_5B26:
        mov    eax, dword ptr [esp + 40h]
        mov    ecx, dword ptr [esp + 3ch]
        mov    edx, dword ptr [esp + 38h]
        push   eax
        mov    eax, dword ptr [esp + 38h]
        push   ecx
        mov    ecx, dword ptr [esp + 38h]
        push   edx
        mov    edx, dword ptr [esp + 30h]
        push   eax
        push   ecx
        inc    dword ptr [edi + 0ch]
        push   edx
        mov    eax, dword ptr [edi + 4]
        lea    ecx, [esp + 2ch]
        push   eax
        push   ecx
        call   sub_1_8EB0
        mov    byte ptr [edi + 10h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_5B6B
        push   1
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5B6B:
        mov    edi, ebp
        test   ebp, ebp
        jne    SHORT loc_1_5B19
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    24h
loc_1_5B7B:
        cmp    al, 40h
        jne    SHORT loc_1_5B86
        mov    edi, 40h
        jmp    SHORT loc_1_5BA9
loc_1_5B86:
        mov    ecx, dword ptr [esp + 28h]
        mov    al, byte ptr [ecx]
        test   al, 1
        je     SHORT loc_1_5BA5
        sub    al, 0ffh
        cmp    al, 1
        sbb    edi, edi
        and    edi, 6
        add    edi, 2
        jmp    SHORT loc_1_5BA9
loc_1_5B9E:
        mov    edi, 20h
        jmp    SHORT loc_1_5BA9
loc_1_5BA5:
        mov    edi, dword ptr [esp + 14h]
loc_1_5BA9:
        mov    esi, dword ptr [esp + 20h]
        mov    ebx, dword ptr [esi + 64h]
        test   ebx, ebx
        je     NEAR PTR loc_1_5CBA
loc_1_5BB8:
        mov    eax, dword ptr [ebx + 8]
        mov    ecx, dword ptr [ebx + 20h]
        mov    dword ptr [esp + 18h], ecx
        test   al, 0a0h
        jne    NEAR PTR loc_1_5C69
        cmp    edi, 8
        jne    SHORT loc_1_5BD7
        test   al, 8
        jne    NEAR PTR loc_1_5C69
loc_1_5BD7:
        cmp    edi, 40h
        jne    SHORT loc_1_5BE4
        test   al, 40h
        jne    NEAR PTR loc_1_5C69
loc_1_5BE4:
        cmp    edi, 2
        jne    NEAR PTR loc_1_5CAE
        test   al, 4
        jne    SHORT loc_1_5C69
        test   al, 2
        je     NEAR PTR loc_1_5CAE
        cmp    dword ptr [esp + 2ch], 6
        jne    SHORT loc_1_5C31
        lea    eax, [esp + 10h]
        mov    ecx, dword ptr [esp + 28h]
        push   eax
        mov    edx, dword ptr [esi + 4]
        push   ecx
        mov    eax, dword ptr [esi + 30h]
        push   edx
        push   eax
        call   sub_1_6130
        test   al, al
        je     SHORT loc_1_5C31
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, 1
        mov    cl, byte ptr [ebx + 11h]
        shl    ebp, cl
        test   dword ptr [eax + edx*4], ebp
        jne    SHORT loc_1_5C69
loc_1_5C31:
        cmp    dword ptr [esp + 2ch], 2
        jne    SHORT loc_1_5CAE
        lea    eax, [esp + 10h]
        mov    ecx, dword ptr [esp + 28h]
        push   eax
        mov    edx, dword ptr [esi + 0ch]
        push   ecx
        mov    eax, dword ptr [esi + 34h]
        push   edx
        push   eax
        call   sub_1_61D0
        test   al, al
        je     SHORT loc_1_5CAE
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, 1
        mov    cl, byte ptr [ebx + 11h]
        shl    ebp, cl
        test   dword ptr [eax + edx*4], ebp
        je     SHORT loc_1_5CAE
loc_1_5C69:
        mov    eax, dword ptr [esp + 40h]
        mov    ecx, dword ptr [esp + 3ch]
        mov    edx, dword ptr [esp + 38h]
        push   eax
        mov    eax, dword ptr [esp + 38h]
        push   ecx
        mov    ecx, dword ptr [esp + 38h]
        push   edx
        mov    edx, dword ptr [esp + 30h]
        push   eax
        push   ecx
        inc    dword ptr [ebx + 0ch]
        push   edx
        mov    eax, dword ptr [ebx + 4]
        lea    ecx, [esp + 2ch]
        push   eax
        push   ecx
        call   sub_1_8EB0
        mov    byte ptr [ebx + 10h], 1
        mov    eax, dword ptr [ebx + 0ch]
        dec    eax
        mov    dword ptr [ebx + 0ch], eax
        jne    SHORT loc_1_5CAE
        push   1
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5CAE:
        mov    ebx, dword ptr [esp + 18h]
        test   ebx, ebx
        jne    NEAR PTR loc_1_5BB8
loc_1_5CBA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    24h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5CD0:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 38h
        mov    ecx, dword ptr [esp + 40h]
        push   ebx
        mov    dword ptr [esp + 30h], ecx
        push   esi
        mov    dword ptr [esp + 30h], 0
        or     dword ptr [eax + 3ch], 1000h
        push   edi
        push   ebp
        mov    ebp, dword ptr [eax + 0e0h]
        mov    eax, dword ptr [esp + 54h]
        test   eax, eax
        je     NEAR PTR loc_1_6091
        mov    dword ptr [esp + 44h], eax
loc_1_5D08:
        mov    eax, dword ptr [esp + 3ch]
        mov    edi, dword ptr [eax]
        xor    eax, eax
        mov    ax, word ptr [edi + 1eh]
        add    eax, edi
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [edi + 8]
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 18h], ecx
        mov    dword ptr [esp + 30h], edx
        test   eax, eax
        je     SHORT loc_1_5D3E
loc_1_5D31:
        mov    edx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax]
        add    dword ptr [esp + 30h], edx
        test   eax, eax
        jne    SHORT loc_1_5D31
loc_1_5D3E:
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esp + 28h], ecx
        inc    dword ptr [esp + 18h]
        mov    al, byte ptr [ecx]
        mov    ecx, dword ptr [esp + 24h]
        and    al, 40h
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0fffffffch
        add    eax, 6
        cmp    dword ptr [ecx + 1ch], 0c000009ah
        mov    dword ptr [esp + 20h], eax
        je     SHORT loc_1_5D77
        mov    dword ptr [ecx + 1ch], 0
        mov    byte ptr [esp + 13h], 0
        jmp    SHORT loc_1_5D7C
loc_1_5D77:
        mov    byte ptr [esp + 13h], 1
loc_1_5D7C:
        mov    eax, dword ptr [esp + 4ch]
        mov    edx, dword ptr [esp + 30h]
        mov    dword ptr [edi + 30h], 0
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [edi + 2ch], eax
        mov    byte ptr [edi + 1ch], 0
        cmp    dword ptr [ecx + 10h], edx
        jae    NEAR PTR loc_1_5EFA
        xor    dl, dl
        mov    ecx, dword ptr [esp + 18h]
        mov    cl, byte ptr [ecx]
        mov    al, cl
        and    al, 0f0h
        cmp    al, 40h
        je     SHORT loc_1_5DB6
        mov    dl, cl
        not    dl
        and    dl, 1
loc_1_5DB6:
        test   dl, dl
        je     NEAR PTR loc_1_5ED3
        mov    byte ptr [esp + 14h], 0
        test   byte ptr [ebp + 14h], 0a4h
        je     SHORT loc_1_5E0E
        cmp    dword ptr [esp + 20h], 6
        lea    edx, [ebp + 3ch]
        je     SHORT loc_1_5DD6
        lea    edx, [ebp + 42h]
loc_1_5DD6:
        mov    ecx, dword ptr [esp + 18h]
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx], ax
        jne    SHORT loc_1_5E09
        cmp    dword ptr [esp + 20h], 2
        je     SHORT loc_1_5E02
        cmp    dword ptr [esp + 20h], 6
        lea    edx, [ebp + 3ch]
        je     SHORT loc_1_5DF6
        lea    edx, [ebp + 42h]
loc_1_5DF6:
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [ecx + 2]
        cmp    dword ptr [edx + 2], eax
        jne    SHORT loc_1_5E09
loc_1_5E02:
        mov    byte ptr [esp + 14h], 0
        jmp    SHORT loc_1_5E0E
loc_1_5E09:
        mov    byte ptr [esp + 14h], 1
loc_1_5E0E:
        mov    esi, dword ptr [ebp + 60h]
        test   esi, esi
        je     NEAR PTR loc_1_6082
loc_1_5E19:
        test   byte ptr [esi + 8], 0a0h
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esp + 2ch], eax
        jne    SHORT loc_1_5E31
        cmp    byte ptr [esp + 14h], 0
        jne    NEAR PTR loc_1_5EC2
loc_1_5E31:
        mov    eax, dword ptr [esi + 4]
        inc    dword ptr [esp + 38h]
        mov    byte ptr [esi + 10h], 1
        inc    dword ptr [esi + 0ch]
        mov    ecx, dword ptr [esi + 8]
        and    ecx, 0a0h
        cmp    ecx, 1
        sbb    cl, cl
        inc    cl
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_5E6C
        test   cl, cl
        jne    SHORT loc_1_5E6C
        mov    ecx, dword ptr [eax + 54h]
        test   ecx, ecx
        je     SHORT loc_1_5E6C
        push   edi
        mov    eax, dword ptr [eax + 24h]
        push   eax
        call   ecx
        mov    ebx, eax
        jmp    SHORT loc_1_5E96
loc_1_5E6C:
        xor    ebx, ebx
        mov    ecx, dword ptr [esp + 24h]
        mov    edx, dword ptr [ecx + 10h]
        mov    ecx, dword ptr [esp + 30h]
        sub    ecx, edx
        push   ecx
        push   ecx
        mov    ecx, dword ptr [esp + 30h]
        add    ecx, edx
        push   ecx
        push   edx
        mov    edx, dword ptr [esp + 38h]
        push   edx
        push   edi
        push   eax
        lea    eax, [esp + 5ch]
        push   eax
        call   sub_1_8EB0
loc_1_5E96:
        test   ebx, ebx
        je     SHORT loc_1_5EB0
        xor    eax, eax
        mov    ax, word ptr [edi + 1eh]
        mov    byte ptr [esp + 13h], 1
        mov    dword ptr [eax + edi + 1ch], 103h
        add    dword ptr [edi + 30h], ebx
loc_1_5EB0:
        mov    eax, dword ptr [esi + 0ch]
        dec    eax
        mov    dword ptr [esi + 0ch], eax
        jne    SHORT loc_1_5EC2
        push   1
        push   esi
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5EC2:
        mov    esi, dword ptr [esp + 2ch]
        test   esi, esi
        jne    NEAR PTR loc_1_5E19
        jmp    NEAR PTR loc_1_6082
loc_1_5ED3:
        cmp    al, 40h
        jne    SHORT loc_1_5EE1
        mov    dword ptr [esp + 1ch], 40h
        jmp    SHORT loc_1_5F02
loc_1_5EE1:
        test   cl, 1
        je     SHORT loc_1_5F02
        sub    cl, 0ffh
        cmp    cl, 1
        sbb    eax, eax
        and    eax, 6
        add    eax, 2
        mov    dword ptr [esp + 1ch], eax
        jmp    SHORT loc_1_5F02
loc_1_5EFA:
        mov    dword ptr [esp + 1ch], 20h
loc_1_5F02:
        mov    esi, dword ptr [ebp + 64h]
        test   esi, esi
        je     NEAR PTR loc_1_6082
loc_1_5F0D:
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 20h]
        mov    ebx, ecx
        mov    dword ptr [esp + 2ch], eax
        and    ebx, 0a0h
        jne    NEAR PTR loc_1_5FEE
        cmp    dword ptr [esp + 1ch], 8
        jne    SHORT loc_1_5F35
        test   cl, 8
        jne    NEAR PTR loc_1_5FEE
loc_1_5F35:
        cmp    dword ptr [esp + 1ch], 40h
        jne    SHORT loc_1_5F45
        test   cl, 40h
        jne    NEAR PTR loc_1_5FEE
loc_1_5F45:
        cmp    dword ptr [esp + 1ch], 2
        jne    NEAR PTR loc_1_6076
        test   cl, 4
        jne    NEAR PTR loc_1_5FEE
        test   cl, 2
        je     NEAR PTR loc_1_6076
        cmp    dword ptr [esp + 20h], 6
        jne    SHORT loc_1_5FA2
        lea    eax, [esp + 34h]
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        mov    edx, dword ptr [ebp + 4]
        push   ecx
        mov    eax, dword ptr [ebp + 30h]
        push   edx
        push   eax
        call   sub_1_6130
        test   al, al
        je     SHORT loc_1_5FA2
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esp + 34h]
        mov    edx, 1
        mov    dword ptr [esp + 14h], ecx
        mov    cl, byte ptr [esi + 11h]
        shl    edx, cl
        mov    ecx, dword ptr [esp + 14h]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_5FEE
loc_1_5FA2:
        cmp    dword ptr [esp + 20h], 2
        jne    NEAR PTR loc_1_6076
        lea    eax, [esp + 34h]
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [ebp + 34h]
        push   edx
        push   eax
        call   sub_1_61D0
        test   al, al
        je     NEAR PTR loc_1_6076
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esp + 34h]
        mov    edx, 1
        mov    dword ptr [esp + 14h], ecx
        mov    cl, byte ptr [esi + 11h]
        shl    edx, cl
        mov    ecx, dword ptr [esp + 14h]
        test   dword ptr [eax + ecx*4], edx
        je     NEAR PTR loc_1_6076
loc_1_5FEE:
        mov    eax, dword ptr [esi + 4]
        inc    dword ptr [esp + 38h]
        mov    byte ptr [esi + 10h], 1
        inc    dword ptr [esi + 0ch]
        cmp    ebx, 1
        sbb    cl, cl
        inc    cl
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_6020
        test   cl, cl
        jne    SHORT loc_1_6020
        mov    ecx, dword ptr [eax + 54h]
        test   ecx, ecx
        je     SHORT loc_1_6020
        push   edi
        mov    eax, dword ptr [eax + 24h]
        push   eax
        call   ecx
        mov    ebx, eax
        jmp    SHORT loc_1_604A
loc_1_6020:
        xor    ebx, ebx
        mov    ecx, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 30h]
        mov    ecx, dword ptr [ecx + 10h]
        sub    edx, ecx
        push   edx
        push   edx
        mov    edx, dword ptr [esp + 30h]
        add    edx, ecx
        push   edx
        push   ecx
        mov    ecx, dword ptr [esp + 38h]
        push   ecx
        push   edi
        push   eax
        lea    eax, [esp + 5ch]
        push   eax
        call   sub_1_8EB0
loc_1_604A:
        test   ebx, ebx
        je     SHORT loc_1_6064
        xor    eax, eax
        mov    ax, word ptr [edi + 1eh]
        mov    byte ptr [esp + 13h], 1
        mov    dword ptr [eax + edi + 1ch], 103h
        add    dword ptr [edi + 30h], ebx
loc_1_6064:
        mov    eax, dword ptr [esi + 0ch]
        dec    eax
        mov    dword ptr [esi + 0ch], eax
        jne    SHORT loc_1_6076
        push   1
        push   esi
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_6076:
        mov    esi, dword ptr [esp + 2ch]
        test   esi, esi
        jne    NEAR PTR loc_1_5F0D
loc_1_6082:
        add    dword ptr [esp + 3ch], 4
        dec    dword ptr [esp + 44h]
        jne    NEAR PTR loc_1_5D08
loc_1_6091:
        mov    eax, dword ptr [esp + 4ch]
        and    dword ptr [eax + 3ch], 0ffffefffh
        cmp    dword ptr [esp + 38h], 0
        je     SHORT loc_1_60A9
        push   ebp
        call   sub_1_60D0
loc_1_60A9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_60C0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_60D0
        ret    4
        db  08Dh, 049h, 000h
sub_1_60D0:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    edi, dword ptr [esi + 18h]
        test   edi, edi
        je     SHORT loc_1_612A
loc_1_60DE:
        cmp    byte ptr [edi + 10h], 0
        mov    ebx, dword ptr [edi + 14h]
        je     SHORT loc_1_6124
        mov    byte ptr [edi + 10h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_8EF0
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_6124
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_6107:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 24h]
        mov    eax, 1
        mov    cl, byte ptr [edi + 11h]
        shl    eax, cl
        push   0
        or     dword ptr [esi + 38h], eax
        push   24h
        push   edi
        call   sub_1_74E0
loc_1_6124:
        mov    edi, ebx
        test   ebx, ebx
        jne    SHORT loc_1_60DE
loc_1_612A:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_1_6130:
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    ebx, dword ptr [esp + 10h]
        push   ebp
        mov    edi, ebx
        shr    edi, 1
        test   ebx, ebx
        je     SHORT loc_1_61A7
        dec    ebx
        cmp    ebx, edi
        jb     SHORT loc_1_61A7
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 1ch]
loc_1_6150:
        cmp    edi, esi
        jb     SHORT loc_1_61A7
        lea    eax, [edi + edi*2]
        lea    ebp, [ecx + eax*2]
        mov    ax, word ptr [edx]
        cmp    word ptr [ebp], ax
        jbe    SHORT loc_1_616A
        mov    eax, 1
        jmp    SHORT loc_1_6186
loc_1_616A:
        jae    SHORT loc_1_6173
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_6186
loc_1_6173:
        mov    eax, dword ptr [ebp + 2]
        mov    ebp, dword ptr [edx + 2]
        cmp    eax, ebp
        jbe    SHORT loc_1_6184
        mov    eax, 1
        jmp    SHORT loc_1_6186
loc_1_6184:
        sbb    eax, eax
loc_1_6186:
        test   eax, eax
        je     SHORT loc_1_61B6
        jle    SHORT loc_1_6195
        test   edi, edi
        je     SHORT loc_1_61A7
        lea    ebx, [edi - 1]
        jmp    SHORT loc_1_6198
loc_1_6195:
        lea    esi, [edi + 1]
loc_1_6198:
        mov    eax, ebx
        sub    eax, esi
        inc    eax
        shr    eax, 1
        lea    edi, [eax + esi]
        cmp    ebx, edi
        jae    SHORT loc_1_6150
loc_1_61A7:
        mov    eax, dword ptr [esp + 20h]
        pop    ebp
        mov    dword ptr [eax], edi
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_1_61B6:
        mov    eax, dword ptr [esp + 20h]
        pop    ebp
        mov    dword ptr [eax], edi
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_61D0:
        push   ebx
        xor    ecx, ecx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   ebp
        mov    edx, esi
        shr    edx, 1
        test   esi, esi
        je     SHORT loc_1_622A
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_1_622A
        mov    edi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 1ch]
loc_1_61F0:
        cmp    edx, ecx
        jb     SHORT loc_1_622A
        mov    bp, word ptr [edi + edx*2]
        mov    ax, word ptr [ebx]
        cmp    bp, ax
        jbe    SHORT loc_1_6207
        mov    eax, 1
        jmp    SHORT loc_1_6209
loc_1_6207:
        sbb    eax, eax
loc_1_6209:
        test   eax, eax
        je     SHORT loc_1_6239
        jle    SHORT loc_1_6218
        test   edx, edx
        je     SHORT loc_1_622A
        lea    esi, [edx - 1]
        jmp    SHORT loc_1_621B
loc_1_6218:
        lea    ecx, [edx + 1]
loc_1_621B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [eax + ecx]
        cmp    esi, edx
        jae    SHORT loc_1_61F0
loc_1_622A:
        mov    eax, dword ptr [esp + 20h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], edx
        xor    al, al
        ret    10h
loc_1_6239:
        mov    eax, dword ptr [esp + 20h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], edx
        mov    al, 1
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_6250:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        xor    bl, bl
        mov    ebp, dword ptr [esi + 14h]
        mov    edi, dword ptr [esp + 1ch]
        mov    al, byte ptr [edi]
        test   al, 1
        je     SHORT loc_1_62CD
        cmp    al, 0ffh
        jne    SHORT loc_1_627F
        test   ebp, 8
        je     NEAR PTR loc_1_6304
        jmp    NEAR PTR loc_1_6302
loc_1_627F:
        test   ebp, 4
        jne    SHORT loc_1_6302
        test   ebp, 2
        je     SHORT loc_1_6304
        cmp    dword ptr [esp + 20h], 6
        jne    SHORT loc_1_62AD
        lea    eax, [esp + 10h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        mov    edx, dword ptr [esi + 30h]
        push   edi
        push   ecx
        push   edx
        call   sub_1_6130
        test   al, al
        jne    SHORT loc_1_6302
loc_1_62AD:
        cmp    dword ptr [esp + 20h], 2
        jne    SHORT loc_1_6304
        lea    eax, [esp + 10h]
        mov    ecx, dword ptr [esi + 0ch]
        push   eax
        mov    edx, dword ptr [esi + 34h]
        push   edi
        push   ecx
        push   edx
        call   sub_1_61D0
        test   al, al
        je     SHORT loc_1_6304
        jmp    SHORT loc_1_6302
loc_1_62CD:
        cmp    dword ptr [esp + 20h], 6
        jne    SHORT loc_1_62F0
        mov    ax, word ptr [edi]
        cmp    word ptr [esi + 3ch], ax
        jne    SHORT loc_1_62E9
        mov    eax, dword ptr [esi + 3eh]
        cmp    dword ptr [edi + 2], eax
        jne    SHORT loc_1_62E9
        xor    eax, eax
        jmp    SHORT loc_1_62FE
loc_1_62E9:
        mov    eax, 1
        jmp    SHORT loc_1_62FE
loc_1_62F0:
        mov    ax, word ptr [esi + 42h]
        sub    ax, word ptr [edi]
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
loc_1_62FE:
        test   eax, eax
        jne    SHORT loc_1_6304
loc_1_6302:
        mov    bl, 1
loc_1_6304:
        test   ebp, 0a0h
        je     SHORT loc_1_630E
        mov    bl, 1
loc_1_630E:
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
NDIS_Service_00:
        or     eax, 400h
        clc
        ret
NDIS_Control:
        cmp    eax, 0
        je     SHORT loc_1_6369
        cmp    eax, 1
        je     SHORT loc_1_639C
        cmp    eax, 2
        je     NEAR PTR loc_1_6459
        cmp    eax, 3
        je     NEAR PTR loc_1_648E
        cmp    eax, 4
        je     NEAR PTR loc_1_6490
        cmp    eax, 5
        je     NEAR PTR loc_1_64AB
        cmp    eax, 6
        je     NEAR PTR loc_1_64B2
        cmp    eax, 22h
        je     NEAR PTR loc_1_647A
        clc
        ret
loc_1_6369:
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        pop    ebx
        db  08Dh, 005h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_6393:
        cmp    eax, 0
        jne    SHORT loc_1_639A
        clc
        ret
loc_1_639A:
        stc
        ret
loc_1_639C:
        db  0CDh, 020h, 000h, 000h, 080h, 004h
        jb     SHORT loc_1_63AE
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_63AE:
        db  0CDh, 020h, 000h, 000h, 096h, 004h
        jb     SHORT loc_1_63C0
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_63C0:
        db  0CDh, 020h, 000h, 000h, 097h, 004h
        jb     SHORT loc_1_63D2
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_63D2:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_63D7:
        push   1
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_63DE:
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_63EE
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_63EB:
        add    esp, 4
loc_1_63EE:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_63F3:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_6402:
        cmp    eax, 0
        jne    SHORT loc_1_6457
        db  0CDh, 020h, 008h, 000h, 005h, 000h
        add    edx, 0e44h
        mov    ecx, 15180h
        imul   edx, ecx
        push   edx
        mov    ecx, 3e8h
        xor    edx, edx
        div    ecx
        pop    edx
        add    edx, eax
        db  0A1h, 000h, 000h, 000h, 000h
        shl    eax, 2
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, edx
        mov    ecx, 989680h
        mul    ecx
        add    eax, 0d53e8000h
        adc    edx, 19db1deh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        clc
        ret
loc_1_6457:
        stc
        ret
loc_1_6459:
        push   ebx
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        jb     SHORT loc_1_6468
        db  088h, 01Dh, 000h, 000h, 000h, 000h
loc_1_6468:
        pop    ebx
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_6478:
        clc
        ret
loc_1_647A:
        push   edx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_6481:
        add    esp, 8
        cmp    eax, 0
        jne    SHORT loc_1_648C
        clc
        jmp    SHORT loc_1_648D
loc_1_648C:
        stc
loc_1_648D:
        ret
loc_1_648E:
        clc
        ret
loc_1_6490:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_64A9:
        clc
        ret
loc_1_64AB:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_64B0:
        clc
        ret
loc_1_64B2:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_64B7:
        clc
        ret
NDIS_Service_97:
        ret
loc_1_64BA:
        int3
        ret
sub_1_64BC:
        cld
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        rep movsd
        mov    cl, al
        rep movsb
        pop    edi
        pop    esi
        ret    0ch
sub_1_64DD:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_64FD
        or     ecx, ecx
        je     SHORT loc_1_6513
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_6511
loc_1_64FD:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_6511
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_6511:
        rep movsb
loc_1_6513:
        popfd
        pop    edi
        pop    esi
        ret    0ch
sub_1_6519:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ax, 5830h
        stosw
        mov    ecx, 8
loc_1_652E:
        sub    al, al
        shld   eax, edx, 4
        shl    edx, 4
        cmp    al, 9
        jbe    SHORT loc_1_653D
        add    al, 7
loc_1_653D:
        add    al, 30h
        stosb
        loop   loc_1_652E
        pop    edi
        pop    ebp
        ret
sub_1_6545:
        pop    eax
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ecx], eax
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        mov    ecx, dword ptr [esp + 20h]
        push   dword ptr [ecx]
        mov    dword ptr [ecx], eax
        mov    eax, edx
        ret
sub_1_655D:
        db  0CDh, 020h, 055h, 080h, 001h, 000h
sub_1_6563:
        mov    eax, dword ptr [esp + 4]
        add    eax, 4
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 002h
        je     SHORT loc_1_658D
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_658C
        mov    dword ptr [eax], 0
        add    eax, 4
loc_1_658C:
        ret
loc_1_658D:
        push   eax
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_658C
        mov    dword ptr [eax], 1
        add    eax, 4
        ret
sub_1_65A5:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_65AF
        ret
loc_1_65AF:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_65F5
        sub    dword ptr [esp + 4], 4
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx]
        and    eax, 0fffeh
        je     SHORT loc_1_65CA
        jmp    SHORT loc_1_65F5
loc_1_65CA:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 002h
        je     SHORT loc_1_65D9
        db  0CDh, 020h, 051h, 080h, 001h, 000h
loc_1_65D9:
        mov    edx, dword ptr [esp + 4]
        push   esi
        push   ebx
        db  0BEh, 000h, 000h, 000h, 000h
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ebx
        pop    esi
loc_1_65F5:
        ret
loc_1_65F6:
        push   0
        push   edx
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_6608
        db  0CDh, 020h, 051h, 000h, 001h, 000h
loc_1_6604:
        add    esp, 8
        ret
loc_1_6608:
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        jmp    SHORT loc_1_6604
sub_1_6610:
        db  0CDh, 020h, 011h, 000h, 040h, 000h
sub_1_6616:
        db  0CDh, 020h, 06Ch, 080h, 001h, 000h
sub_1_661C:
        db  0CDh, 020h, 061h, 080h, 001h, 000h
sub_1_6622:
        db  0CDh, 020h, 003h, 080h, 080h, 004h
sub_1_6628:
        db  0CDh, 020h, 00Ch, 080h, 027h, 000h
sub_1_662E:
        db  0CDh, 020h, 011h, 080h, 027h, 000h
sub_1_6634:
        db  0CDh, 020h, 00Ah, 080h, 027h, 000h
sub_1_663A:
        db  0CDh, 020h, 00Eh, 080h, 027h, 000h
sub_1_6640:
        push   ebx
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  06Ah, 000h, 068h, 000h, 0F0h, 00Fh, 000h, 068h, 000h, 010h, 000h, 000h, 0CDh, 020h, 05Dh, 001h
        db  001h, 000h, 083h, 0C4h, 00Ch, 0C3h
NDIS_Service_12:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_6691
        mov    eax, dword ptr [ebp + 0ch]
        db  0BEh, 000h, 000h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_6686
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        jmp    SHORT loc_1_668F
loc_1_6686:
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
loc_1_668F:
        mov    dword ptr [eax], esi
loc_1_6691:
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_6698:
        xor    eax, eax
        push   eax
        push   eax
        push   dword ptr [edx + 8]
        push   eax
        mov    dword ptr [edx], eax
        mov    edx, dword ptr [edx + 4]
        cld
        call   edx
        ret
NDIS_Service_13:
        push   esi
        mov    eax, dword ptr [esp + 8]
        xor    esi, esi
        xchg   dword ptr [eax], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        mov    byte ptr [ecx], 1
        pop    esi
        ret    8
loc_1_66C3:
        push   eax
        xor    ebx, ebx
        push   edx
        push   dword ptr [edx + 4]
        mov    ecx, dword ptr [edx + 8]
        or     ecx, ecx
        je     SHORT loc_1_6712
        cld
        call   ecx
        or     bh, al
        pop    edx
        or     al, al
        je     SHORT loc_1_66F7
        mov    cx, word ptr [edx]
        test   cx, 1
        jne    SHORT loc_1_66F7
        db  0BEh, 000h, 000h, 000h, 000h
        or     word ptr [edx], 1
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    dword ptr [edx + 18h], esi
loc_1_66F7:
        mov    cx, word ptr [edx]
        test   cx, 2
        je     SHORT loc_1_6705
        or     bh, bh
        je     SHORT loc_1_6715
loc_1_6705:
        pop    eax
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        bt     cx, 2
        ret
loc_1_6712:
        add    esp, 8
loc_1_6715:
        pop    eax
        stc
        ret
loc_1_6718:
        mov    eax, edx
        mov    ecx, dword ptr [eax]
        push   ecx
        push   eax
        push   ecx
        call   sub_1_8A60
        test   al, al
        pop    eax
        je     SHORT loc_1_6731
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
loc_1_6731:
        stc
        ret
loc_1_6733:
        mov    dword ptr [edx + 18h], 0
        and    word ptr [edx], 0fffeh
        mov    eax, dword ptr [edx + 4]
        push   eax
        push   eax
        push   eax
        push   eax
        mov    eax, dword ptr [edx + 0ch]
        or     eax, eax
        je     SHORT loc_1_6750
        cld
        call   eax
        ret
loc_1_6750:
        add    esp, 10h
        ret
        db  0C3h
NDIS_Service_17:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_1_6784
        mov    ecx, dword ptr [eax + 10h]
        mov    eax, dword ptr [ecx + 10h]
        push   eax
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        push   dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        cld
        call   eax
        mov    ecx, eax
        pop    eax
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        mov    eax, ecx
loc_1_6780:
        pop    ebp
        ret    0ch
loc_1_6784:
        push   dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        cld
        call   eax
        jmp    SHORT loc_1_6780
NDIS_Service_14:
        push   ebp
        mov    ebp, esp
        in     al, 21h
        jmp    SHORT loc_1_6796
loc_1_6796:
        push   eax
        mov    al, 0feh
        out    21h, al
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 10h
        ja     SHORT loc_1_67B7
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mul    ecx
        mov    ecx, eax
        mov    eax, eax
loc_1_67B0:
        loop   loc_1_67B0
        pop    eax
        out    21h, al
        jmp    SHORT loc_1_6825
loc_1_67B7:
        pop    eax
        out    21h, al
        jmp    SHORT loc_1_67BC
loc_1_67BC:
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        shld   edx, eax, 14h
        shl    eax, 14h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        div    ecx
        mov    ecx, eax
        mov    eax, eax
loc_1_67D4:
        loop   loc_1_67D4
        cmp    dword ptr [ebp + 8], 2710h
        jb     SHORT loc_1_6825
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     SHORT loc_1_6825
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        mov    eax, 1
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
loc_1_6825:
        pop    ebp
        cld
        db  0A1h, 000h, 000h, 000h, 000h
        ret    4
loc_1_682F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_1_6866
        cmp    eax, 10h
        ja     SHORT loc_1_684C
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mul    ecx
        mov    ecx, eax
loc_1_6848:
        loop   loc_1_6848
        jmp    SHORT loc_1_6866
loc_1_684C:
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        shld   edx, eax, 14h
        shl    eax, 14h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        div    ecx
        mov    ecx, eax
        mov    eax, eax
loc_1_6864:
        loop   loc_1_6864
loc_1_6866:
        pop    ebp
        cld
        db  0A1h, 000h, 000h, 000h, 000h
        ret    4
sub_1_6870:
        mov    eax, dword ptr [esp + 4]
loc_1_6874:
        cli
        cmp    dword ptr [eax], 1
        je     SHORT loc_1_6888
        push   5
        push   eax
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_1_6874
loc_1_6888:
        sti
        ret
sub_1_688A:
        mov    eax, dword ptr [esp + 4]
        push   5
        push   eax
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
        ret
sub_1_689B:
        push   dword ptr [esp + 4]
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        ret
sub_1_68A9:
        push   dword ptr [esp + 4]
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        ret
sub_1_68B7:
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        ret
NDIS_Service_01:
        push   ecx
        mov    ecx, 1
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        pop    ecx
        ret
NDIS_Service_03:
        push   ecx
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        pop    ecx
        ret
NDIS_Service_04:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        db  0CDh, 020h, 028h, 080h, 001h, 000h
NDIS_Service_02:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        db  0CDh, 020h, 026h, 080h, 001h, 000h
NDIS_Service_24:
        jmp    NEAR PTR loc_1_6BA0
NDIS_Service_25:
        jmp    NEAR PTR sub_1_6C50
NDIS_Service_26:
        jmp    NEAR PTR loc_1_6C80
NDIS_Service_27:
        jmp    NEAR PTR sub_1_6CA0
NDIS_Service_30:
        jmp    NEAR PTR sub_1_6D50
NDIS_Service_56:
        jmp    NEAR PTR sub_1_76E0
NDIS_Service_31:
        jmp    NEAR PTR loc_1_6E10
NDIS_Service_32:
        jmp    NEAR PTR sub_1_6E40
NDIS_Service_33:
        jmp    NEAR PTR sub_1_6E80
NDIS_Service_34:
        jmp    NEAR PTR sub_1_6EB0
NDIS_Service_35:
        jmp    NEAR PTR sub_1_6FA0
NDIS_Service_36:
        jmp    NEAR PTR sub_1_7010
NDIS_Service_37:
        jmp    NEAR PTR sub_1_7080
NDIS_Service_38:
        jmp    NEAR PTR loc_1_70D0
NDIS_Service_39:
        jmp    NEAR PTR sub_1_7150
NDIS_Service_40:
        jmp    NEAR PTR sub_1_7170
NDIS_Service_45:
        jmp    NEAR PTR loc_1_8E30
NDIS_Service_96:
        jmp    NEAR PTR loc_1_9C10
NDIS_Service_46:
        jmp    NEAR PTR loc_1_8F00
NDIS_Service_57:
        jmp    NEAR PTR loc_1_8E80
NDIS_Service_58:
        jmp    NEAR PTR loc_1_8F50
NDIS_Service_61:
        jmp    NEAR PTR sub_1_8EB0
NDIS_Service_62:
        jmp    NEAR PTR sub_1_8EF0
NDIS_Service_10:
        jmp    NEAR PTR sub_1_7450
NDIS_Service_11:
        jmp    NEAR PTR sub_1_74E0
NDIS_Service_66:
        db  0E9h, 000h, 000h, 000h, 000h
NDIS_Service_95:
        jmp    NEAR PTR loc_1_84A0
NDIS_Service_98:
        jmp    NEAR PTR sub_1_7360
NDIS_Service_99:
        jmp    NEAR PTR sub_1_7380
NDIS_Service_100:
        jmp    NEAR PTR sub_1_73C0
NDIS_Service_101:
        jmp    NEAR PTR sub_1_73D0
NDIS_Service_102:
        jmp    NEAR PTR loc_1_7EB0
NDIS_Service_103:
        jmp    NEAR PTR loc_1_7E30
NDIS_Service_75:
        push   ebp
        mov    ebp, esp
        pushad
        mov    edi, dword ptr [ebp + 0ch]
        and    byte ptr [edi + 0ch], 0f3h
        cmp    byte ptr [ebp + 1ch], 1
        je     SHORT loc_1_69EC
        or     byte ptr [edi + 0ch], 4
        jmp    SHORT loc_1_69F0
loc_1_69EC:
        or     byte ptr [edi + 0ch], 8
loc_1_69F0:
        cmp    dword ptr [edi + 8], 3
        jne    SHORT loc_1_6A0A
        and    byte ptr [edi + 10h], 0f3h
        cmp    byte ptr [ebp + 1ch], 1
        je     SHORT loc_1_6A06
        or     byte ptr [edi + 10h], 8
        jmp    SHORT loc_1_6A0A
loc_1_6A06:
        or     byte ptr [edi + 10h], 0
loc_1_6A0A:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_6A47
        mov    ecx, dword ptr [ebp + 18h]
        db  0CDh, 020h, 00Eh, 000h, 004h, 000h
        jb     SHORT loc_1_6A95
        mov    dword ptr [edi + 18h], ebx
        mov    dword ptr [edi + 14h], edx
        mov    esi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [esi + 4]
        add    esi, dword ptr [ebp + 14h]
        cmp    dword ptr [ebp + 1ch], 1
        jne    SHORT loc_1_6A3D
        xor    edi, edi
        db  0CDh, 020h, 010h, 000h, 004h, 000h
        jb     SHORT loc_1_6A95
loc_1_6A3D:
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 4]
        sub    bh, bh
        jmp    SHORT loc_1_6A6A
loc_1_6A47:
        mov    esi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [esi + 4]
        add    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 18h]
        sub    dl, dl
        db  0CDh, 020h, 009h, 000h, 004h, 000h
        jb     SHORT loc_1_6A95
        mov    dword ptr [edi + 14h], edx
        mov    bx, 0ff00h
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 4]
loc_1_6A6A:
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        mov    dl, byte ptr [edi + 0ch]
        mov    dh, byte ptr [edi + 10h]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 006h, 000h, 004h, 000h
        mov    edx, eax
        db  0CDh, 020h, 019h, 000h, 004h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_1_6A9E
loc_1_6A95:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
loc_1_6A9E:
        popad
        pop    ebp
        ret    18h
NDIS_Service_76:
        push   ebp
        mov    ebp, esp
        pushad
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 4]
        db  0CDh, 020h, 018h, 000h, 004h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_6AE6
        mov    ebx, dword ptr [edi + 18h]
        cmp    word ptr [ebp + 1ch], 1
        je     SHORT loc_1_6ADC
        mov    esi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [esi + 4]
        add    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 18h]
        xor    edi, edi
        db  0CDh, 020h, 011h, 000h, 004h, 000h
        jb     SHORT loc_1_6B05
loc_1_6ADC:
        db  0CDh, 020h, 00Fh, 000h, 004h, 000h
        jb     SHORT loc_1_6B05
        jmp    SHORT loc_1_6AFA
loc_1_6AE6:
        mov    esi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [esi + 4]
        add    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 18h]
        db  0CDh, 020h, 00Ah, 000h, 004h, 000h
        jb     SHORT loc_1_6B05
loc_1_6AFA:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_1_6B0E
loc_1_6B05:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
loc_1_6B0E:
        popad
        pop    ebp
        ret    18h
NDIS_Service_77:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx]
        cmp    eax, 4
        jae    SHORT loc_1_6B28
        mov    edx, 0ch
        jmp    SHORT loc_1_6B2D
loc_1_6B28:
        mov    edx, 0d8h
loc_1_6B2D:
        out    dx, al
        db  00Fh, 0B7h, 014h, 045h, 000h, 000h, 000h, 000h
        call   sub_1_6B5C
loc_1_6B3B:
        mov    ecx, eax
        call   sub_1_6B5C
        cmp    ah, ch
        jne    SHORT loc_1_6B3B
        inc    eax
        mov    ecx, 0ffffffh
        cmp    dword ptr [ebx + 8], 2
        je     SHORT loc_1_6B55
        shr    ecx, 8
loc_1_6B55:
        and    eax, ecx
        pop    ebx
        pop    ebp
        ret    4
sub_1_6B5C:
        push   edx
        in     al, dx
        jmp    SHORT loc_1_6B60
loc_1_6B60:
        xchg   ah, al
        in     al, dx
        xchg   ah, al
        cmp    dword ptr [ebx + 8], 2
        jne    SHORT loc_1_6B77
        add    dx, 400h
        shl    eax, 10h
        in     al, dx
        rol    eax, 10h
loc_1_6B77:
        pop    edx
        ret
loc_1_6B79:
        xor    eax, eax
        ret    4
        db  033h, 0C0h, 0C2h, 008h, 000h
loc_1_6B83:
        xor    eax, eax
        ret    0ch
loc_1_6B88:
        xor    eax, eax
        ret    10h
        db  033h, 0C0h, 0C2h, 014h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
        db  0CCh, 0CCh, 0CCh
loc_1_6BA0:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        test   esi, esi
        jne    SHORT loc_1_6BC5
        mov    edx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [edx], 0
        mov    dword ptr [eax], 0c0000001h
        ret    0ch
loc_1_6BC5:
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [esp + 14h]
        test   eax, eax
        jne    SHORT loc_1_6BE6
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0c000009ah
        ret    0ch
loc_1_6BE6:
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax + 8]
        mov    ebx, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 8], eax
        mov    edi, dword ptr [edx]
        xor    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    ecx, dword ptr [edx]
        mov    dword ptr [ecx + 10h], esi
        mov    ecx, dword ptr [edx]
        mov    byte ptr [ecx + 1ch], 1
        mov    ecx, dword ptr [edx]
        mov    byte ptr [ecx + 1dh], 80h
        mov    ecx, dword ptr [edx]
        mov    dword ptr [ecx + 34h], 4b41504eh
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [edx]
        sub    ax, 30h
        mov    word ptr [ecx + 1eh], ax
        mov    eax, dword ptr [edx]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 1eh]
        mov    dword ptr [ecx + eax + 28h], eax
        mov    eax, dword ptr [esp + 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6C50:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp + 8]
        xor    edx, edx
        test   eax, eax
        mov    ecx, dword ptr [eax + 10h]
        je     SHORT loc_1_6C76
        test   ecx, ecx
        je     SHORT loc_1_6C76
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [eax + 0ch], edx
        mov    dword ptr [eax + 14h], edx
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], edx
loc_1_6C76:
        mov    edx, dword ptr [esp]
        add    esp, 4
        ret    4
loc_1_6C80:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     SHORT loc_1_6C98
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        je     SHORT loc_1_6C98
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [ecx + 8], eax
loc_1_6C98:
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_6CA0:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        test   esi, esi
        je     NEAR PTR loc_1_6D3E
        mov    eax, dword ptr [esp + 24h]
        test   eax, eax
        je     SHORT loc_1_6CC0
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [eax], ecx
loc_1_6CC0:
        cmp    dword ptr [esp + 28h], 0
        jne    SHORT loc_1_6CD5
        cmp    dword ptr [esp + 20h], 0
        jne    SHORT loc_1_6CD5
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_6D3E
loc_1_6CD5:
        xor    ebx, ebx
        cmp    byte ptr [esi + 1ch], bl
        jne    SHORT loc_1_6D0F
        mov    ebp, dword ptr [esi + 8]
        xor    edi, edi
        mov    dword ptr [esp + 10h], ebx
        test   ebp, ebp
        je     SHORT loc_1_6CFF
loc_1_6CE9:
        add    ebx, dword ptr [ebp + 0ch]
        push   ebp
        call   sub_1_83F0
        add    edi, eax
        mov    ebp, dword ptr [ebp]
        inc    dword ptr [esp + 10h]
        test   ebp, ebp
        jne    SHORT loc_1_6CE9
loc_1_6CFF:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 4], ebx
        mov    dword ptr [esi + 14h], eax
        mov    dword ptr [esi], edi
        mov    byte ptr [esi + 1ch], 1
loc_1_6D0F:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_6D1E
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], ecx
loc_1_6D1E:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_1_6D2E
        mov    ecx, dword ptr [esi + 14h]
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], ecx
loc_1_6D2E:
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_1_6D3E
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax], ecx
loc_1_6D3E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_6D50:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        test   edx, edx
        jne    SHORT loc_1_6DB3
        push   0
        push   14h
        call   sub_1_6563
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_6D9F
        mov    esi, dword ptr [esp + 14h]
        mov    edx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], edx
        mov    dword ptr [eax + 10h], 4655424eh
        mov    dword ptr [ecx], 0
        mov    ecx, dword ptr [esp + 0ch]
        pop    esi
        mov    dword ptr [ecx], eax
        ret    14h
loc_1_6D9F:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], 0c000009ah
        mov    ecx, dword ptr [esp + 0ch]
        pop    esi
        mov    dword ptr [ecx], eax
        ret    14h
loc_1_6DB3:
        mov    eax, dword ptr [edx + 8]
        test   eax, eax
        jne    SHORT loc_1_6DC8
        mov    ecx, dword ptr [esp + 8]
        pop    esi
        mov    dword ptr [ecx], 0c000009ah
        ret    14h
loc_1_6DC8:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx + 8], eax
        mov    esi, dword ptr [ecx]
        mov    dword ptr [esi], 0
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 4], esi
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 8], edx
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 0ch], edx
        mov    eax, dword ptr [ecx]
        mov    ecx, dword ptr [esp + 8]
        pop    esi
        mov    dword ptr [eax + 10h], 4655424eh
        mov    dword ptr [ecx], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_6E10:
        mov    eax, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 10h]
        push   eax
        mov    ecx, dword ptr [edx]
        add    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        push   ecx
        mov    ecx, dword ptr [esp + 0ch]
        push   edx
        push   eax
        push   ecx
        call   sub_1_6D50
        ret    18h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6E40:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     SHORT loc_1_6E6F
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_1_6E5C
        push   0
        push   eax
        call   sub_1_65A5
        add    esp, 8
        ret    4
loc_1_6E5C:
        test   eax, eax
        je     SHORT loc_1_6E6F
        mov    ecx, dword ptr [eax + 8]
        test   ecx, ecx
        je     SHORT loc_1_6E6F
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 8], eax
loc_1_6E6F:
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6E80:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     SHORT loc_1_6E9E
        mov    ecx, dword ptr [esp + 8]
        test   ecx, ecx
        je     SHORT loc_1_6E95
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
loc_1_6E95:
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
loc_1_6E9E:
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6EB0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, 1
        mov    edi, dword ptr [esp + 10h]
        push   ebp
        test   edi, edi
        jne    SHORT loc_1_6ED3
        mov    ebp, dword ptr [esp + 1ch]
        mov    dword ptr [ebp], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_1_6ED3:
        mov    eax, dword ptr [edi + 0ch]
        test   eax, eax
        je     SHORT loc_1_6EF2
        mov    ecx, dword ptr [edi + 4]
        mov    ebp, dword ptr [esp + 1ch]
        add    eax, ecx
        shr    ecx, 0ch
        dec    eax
        shr    eax, 0ch
        sub    eax, ecx
        inc    eax
        mov    dword ptr [ebp], eax
        jmp    SHORT loc_1_6EFD
loc_1_6EF2:
        mov    ebp, dword ptr [esp + 1ch]
        mov    dword ptr [ebp], 0
loc_1_6EFD:
        mov    eax, dword ptr [ebp]
        test   eax, eax
        je     NEAR PTR loc_1_6F93
        mov    esi, dword ptr [esp + 18h]
        push   0
        lea    ecx, [esi + eax*4]
        push   ecx
        push   eax
        mov    eax, dword ptr [edi + 4]
        shr    eax, 0ch
        push   eax
        call   sub_1_661C
        add    esp, 10h
        mov    edx, dword ptr [ebp]
        mov    eax, dword ptr [edi + 4]
        lea    ecx, [esi + edx*4]
        mov    edx, dword ptr [edi + 0ch]
        xor    eax, dword ptr [ecx]
        and    eax, 0fffh
        xor    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        mov    eax, 1000h
        mov    edi, dword ptr [edi + 4]
        and    edi, 0fffh
        sub    eax, edi
        cmp    edx, eax
        jae    SHORT loc_1_6F53
        mov    eax, edx
        xor    edx, edx
        jmp    SHORT loc_1_6F55
loc_1_6F53:
        sub    edx, eax
loc_1_6F55:
        mov    dword ptr [esi + 4], eax
        test   edx, edx
        je     SHORT loc_1_6F90
        add    esi, 8
loc_1_6F5F:
        mov    eax, dword ptr [ecx + 4]
        add    ecx, 4
        and    eax, 0fffff000h
        cmp    edx, 1000h
        mov    dword ptr [esi], eax
        jae    SHORT loc_1_6F7A
        mov    eax, edx
        xor    edx, edx
        jmp    SHORT loc_1_6F85
loc_1_6F7A:
        mov    eax, 1000h
        sub    edx, 1000h
loc_1_6F85:
        mov    dword ptr [esi + 4], eax
        add    esi, 8
        inc    ebx
        test   edx, edx
        jne    SHORT loc_1_6F5F
loc_1_6F90:
        mov    dword ptr [ebp], ebx
loc_1_6F93:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_6FA0:
        push   ebx
        xor    edx, edx
        mov    ebx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        xor    esi, esi
        push   ebp
        xor    edi, edi
        cmp    ebx, edi
        je     SHORT loc_1_6FFA
        mov    ecx, dword ptr [esp + 14h]
        cmp    ecx, edx
        je     SHORT loc_1_6FFA
loc_1_6FBA:
        mov    ebp, dword ptr [ebx + 0ch]
        add    edx, ebp
        test   ebp, ebp
        je     SHORT loc_1_6FD4
        mov    eax, dword ptr [ebx + 4]
        add    ebp, eax
        shr    eax, 0ch
        dec    ebp
        shr    ebp, 0ch
        sub    ebp, eax
        inc    ebp
        add    esi, ebp
loc_1_6FD4:
        inc    edi
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_1_6FDF
        mov    ebx, eax
        jmp    SHORT loc_1_6FBA
loc_1_6FDF:
        mov    eax, dword ptr [ecx + 8]
        test   eax, eax
        jne    SHORT loc_1_6FE9
        mov    dword ptr [ecx + 0ch], ebx
loc_1_6FE9:
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 8], eax
        add    dword ptr [ecx + 14h], edi
        add    dword ptr [ecx + 4], edx
        add    dword ptr [ecx], esi
loc_1_6FFA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7010:
        mov    edx, dword ptr [esp + 8]
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        xor    esi, esi
        push   ebp
        xor    edi, edi
        cmp    edx, ebx
        je     SHORT loc_1_7079
        mov    ecx, dword ptr [esp + 14h]
        cmp    ecx, ebx
        je     SHORT loc_1_7079
loc_1_702A:
        mov    ebp, dword ptr [edx + 0ch]
        add    esi, ebp
        test   ebp, ebp
        je     SHORT loc_1_7044
        mov    eax, dword ptr [edx + 4]
        add    ebp, eax
        shr    eax, 0ch
        dec    ebp
        shr    ebp, 0ch
        sub    ebp, eax
        inc    ebp
        add    edi, ebp
loc_1_7044:
        inc    ebx
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     SHORT loc_1_704F
        mov    edx, eax
        jmp    SHORT loc_1_702A
loc_1_704F:
        cmp    dword ptr [ecx + 8], 0
        je     SHORT loc_1_7061
        mov    ebp, dword ptr [ecx + 0ch]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebp], eax
        jmp    SHORT loc_1_7068
loc_1_7061:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 8], eax
loc_1_7068:
        mov    dword ptr [ecx + 0ch], edx
        mov    dword ptr [edx], 0
        add    dword ptr [ecx + 14h], ebx
        add    dword ptr [ecx + 4], esi
        add    dword ptr [ecx], edi
loc_1_7079:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_1_7080:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        test   ecx, ecx
        je     SHORT loc_1_70CC
        mov    eax, dword ptr [ecx + 8]
        mov    edx, dword ptr [esp + 0ch]
        test   eax, eax
        mov    dword ptr [edx], eax
        je     SHORT loc_1_70CC
        dec    dword ptr [ecx + 14h]
        mov    eax, dword ptr [edx]
        mov    esi, dword ptr [eax]
        mov    dword ptr [ecx + 8], esi
        mov    eax, dword ptr [edx]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [edx]
        mov    esi, dword ptr [eax + 0ch]
        sub    dword ptr [ecx + 4], esi
        mov    eax, dword ptr [edx]
        mov    edx, dword ptr [eax + 0ch]
        test   edx, edx
        je     SHORT loc_1_70CC
        mov    eax, dword ptr [eax + 4]
        mov    esi, eax
        add    eax, edx
        shr    esi, 0ch
        dec    eax
        shr    eax, 0ch
        sub    esi, eax
        dec    esi
        add    dword ptr [ecx], esi
loc_1_70CC:
        pop    esi
        ret    8
loc_1_70D0:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    eax, dword ptr [ecx + 8]
        test   edx, edx
        je     SHORT loc_1_7142
        test   ecx, ecx
        je     SHORT loc_1_7142
        test   eax, eax
        je     SHORT loc_1_713E
        mov    esi, dword ptr [ecx + 0ch]
        cmp    esi, eax
        jne    SHORT loc_1_70FA
        mov    dword ptr [ecx + 8], 0
        jmp    SHORT loc_1_710D
loc_1_70FA:
        cmp    dword ptr [eax], esi
        je     SHORT loc_1_7104
loc_1_70FE:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], esi
        jne    SHORT loc_1_70FE
loc_1_7104:
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx + 0ch], eax
loc_1_710D:
        mov    dword ptr [esi], 0
        dec    dword ptr [ecx + 14h]
        mov    eax, dword ptr [esi + 0ch]
        sub    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        je     SHORT loc_1_7140
        mov    edi, dword ptr [esi + 4]
        mov    ebx, edi
        add    eax, edi
        shr    ebx, 0ch
        dec    eax
        shr    eax, 0ch
        pop    edi
        sub    ebx, eax
        dec    ebx
        add    dword ptr [ecx], ebx
        mov    dword ptr [edx], esi
        pop    esi
        pop    ebx
        ret    8
loc_1_713E:
        xor    esi, esi
loc_1_7140:
        mov    dword ptr [edx], esi
loc_1_7142:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_7150:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     SHORT loc_1_7160
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
loc_1_7160:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7170:
        mov    eax, dword ptr [esp + 18h]
        sub    esp, 14h
        push   ebx
        mov    dword ptr [eax], 0
        mov    dword ptr [esp + 10h], 0
        cmp    dword ptr [esp + 24h], 0
        push   esi
        push   edi
        push   ebp
        je     NEAR PTR loc_1_72CD
        mov    eax, dword ptr [esp + 28h]
        cmp    dword ptr [eax + 14h], 0
        mov    esi, dword ptr [eax + 8]
        je     NEAR PTR loc_1_72CD
        mov    eax, dword ptr [esi + 4]
        mov    edi, dword ptr [esi + 0ch]
        lea    ecx, [esp + 10h]
        push   0
        mov    edx, dword ptr [esp + 38h]
        push   ecx
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 28h]
        push   eax
        push   0
        push   edx
        call   sub_1_6CA0
        cmp    dword ptr [esp + 20h], 0
        je     NEAR PTR loc_1_72CD
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 30h], 0
        mov    ecx, dword ptr [eax + 4]
        mov    ebp, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 18h], ecx
        je     NEAR PTR loc_1_72C3
loc_1_71EB:
        test   edi, edi
        jne    SHORT loc_1_7208
        mov    esi, dword ptr [esi]
        test   esi, esi
        je     NEAR PTR loc_1_72C3
        mov    eax, dword ptr [esi + 4]
        mov    edi, dword ptr [esi + 0ch]
        mov    dword ptr [esp + 14h], eax
        jmp    NEAR PTR loc_1_72B5
loc_1_7208:
        test   ebp, ebp
        jne    SHORT loc_1_7231
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esp + 10h], ecx
        test   ecx, ecx
        je     NEAR PTR loc_1_72C3
        mov    ecx, dword ptr [ecx + 4]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 18h], ecx
        mov    ebp, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_1_72B5
loc_1_7231:
        cmp    dword ptr [esp + 2ch], 0
        je     SHORT loc_1_7258
        cmp    edi, dword ptr [esp + 2ch]
        jae    SHORT loc_1_7246
        sub    dword ptr [esp + 2ch], edi
        xor    edi, edi
        jmp    SHORT loc_1_72B5
loc_1_7246:
        mov    eax, dword ptr [esp + 2ch]
        sub    edi, eax
        add    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 2ch], 0
loc_1_7258:
        cmp    dword ptr [esp + 38h], 0
        je     SHORT loc_1_727F
        cmp    dword ptr [esp + 38h], ebp
        jbe    SHORT loc_1_726D
        sub    dword ptr [esp + 38h], ebp
        xor    ebp, ebp
        jmp    SHORT loc_1_72B5
loc_1_726D:
        mov    eax, dword ptr [esp + 38h]
        sub    ebp, eax
        add    dword ptr [esp + 18h], eax
        mov    dword ptr [esp + 38h], 0
loc_1_727F:
        cmp    edi, ebp
        mov    ebx, edi
        jb     SHORT loc_1_7287
        mov    ebx, ebp
loc_1_7287:
        mov    eax, dword ptr [esp + 30h]
        sub    eax, dword ptr [esp + 1ch]
        cmp    ebx, eax
        jb     SHORT loc_1_7295
        mov    ebx, eax
loc_1_7295:
        mov    eax, dword ptr [esp + 18h]
        push   ebx
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        push   ecx
        sub    ebp, ebx
        sub    edi, ebx
        call   sub_1_64BC
        add    dword ptr [esp + 14h], ebx
        add    dword ptr [esp + 18h], ebx
        add    dword ptr [esp + 1ch], ebx
loc_1_72B5:
        mov    ecx, dword ptr [esp + 1ch]
        cmp    dword ptr [esp + 30h], ecx
        ja     NEAR PTR loc_1_71EB
loc_1_72C3:
        mov    eax, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [eax], ecx
loc_1_72CD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    18h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_72E0:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   ecx
        push   edx
        push   eax
        call   sub_1_7450
        ret    14h
        db  08Dh, 064h, 024h, 000h
loc_1_7300:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        in     al, dx
        mov    byte ptr [ecx], al
        ret    0ch
        db  08Bh, 0FFh
loc_1_7310:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        in     ax, dx
        mov    word ptr [ecx], ax
        ret    0ch
loc_1_7320:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        in     eax, dx
        mov    dword ptr [ecx], eax
        ret    0ch
        db  08Bh, 0FFh
loc_1_7330:
        mov    edx, dword ptr [esp + 8]
        mov    al, byte ptr [esp + 0ch]
        out    dx, al
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_1_7340:
        mov    ax, word ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        out    dx, ax
        ret    0ch
        db  08Bh, 0FFh
loc_1_7350:
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        out    dx, eax
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_1_7360:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    dword ptr [eax], 1
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7380:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 4], 1
        jne    SHORT loc_1_73A3
        lea    eax, [esp + 7]
        mov    ecx, dword ptr [esi + 8]
        push   eax
        push   ecx
        call   NDIS_Service_13
        mov    dword ptr [esi + 4], 0
loc_1_73A3:
        cmp    dword ptr [esi], 1
        jne    SHORT loc_1_73B7
        push   esi
        mov    dword ptr [esi], 0
        call   sub_1_689B
        add    esp, 4
loc_1_73B7:
        pop    esi
        add    esp, 4
        ret    4
        db  08Bh, 0FFh
sub_1_73C0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_7360
        ret    4
        db  08Dh, 049h, 000h
sub_1_73D0:
        sub    esp, 48h
        push   esi
        cli
        mov    esi, dword ptr [esp + 50h]
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_741C
        mov    eax, dword ptr [esp + 54h]
        test   eax, eax
        je     SHORT loc_1_7413
        lea    ecx, [esp + 4]
        mov    dword ptr [esp + 0ch], esi
        push   eax
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esp + 8], 0
        db  0C7h, 044h, 024h, 00Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 4], 1
        lea    ecx, [esp + 8]
        push   ecx
        call   NDIS_Service_12
loc_1_7413:
        push   esi
        call   sub_1_688A
        add    esp, 4
loc_1_741C:
        sti
        cmp    dword ptr [esi + 4], 1
        sbb    al, al
        pop    esi
        neg    al
        add    esp, 48h
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_7430:
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    dword ptr [eax], 0
        call   sub_1_689B
        add    esp, 4
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7450:
        sub    esp, 8
        test   byte ptr [esp + 14h], 1
        push   esi
        je     SHORT loc_1_74B6
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        test   ecx, ecx
        mov    dword ptr [esi], 0
        jne    SHORT loc_1_7479
        mov    eax, 0c0000001h
        pop    esi
        add    esp, 8
        ret    10h
loc_1_7479:
        lea    eax, [esp + 8]
        lea    edx, [esp + 4]
        push   eax
        push   0eh
        mov    eax, dword ptr [esp + 24h]
        push   edx
        shr    eax, 0ch
        dec    ecx
        shr    ecx, 0ch
        push   eax
        push   0
        inc    ecx
        push   0
        push   0
        push   1
        push   ecx
        call   sub_1_6545
        add    esp, 24h
        mov    dword ptr [esi], eax
        cmp    eax, 1
        pop    esi
        sbb    eax, eax
        add    esp, 8
        and    eax, 0c000009ah
        ret    10h
loc_1_74B6:
        mov    ecx, dword ptr [esp + 14h]
        push   0
        push   ecx
        call   sub_1_6563
        mov    esi, dword ptr [esp + 18h]
        add    esp, 8
        cmp    eax, 1
        mov    dword ptr [esi], eax
        sbb    eax, eax
        pop    esi
        and    eax, 0c000009ah
        add    esp, 8
        ret    10h
        db  08Dh, 064h, 024h, 000h
sub_1_74E0:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_750A
        mov    eax, dword ptr [esp + 4]
        push   0
        test   byte ptr [esp + 10h], 1
        push   eax
        je     SHORT loc_1_7502
        call   sub_1_655D
        add    esp, 8
        ret    0ch
loc_1_7502:
        call   sub_1_65A5
        add    esp, 8
loc_1_750A:
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_7510:
        mov    edx, dword ptr [esp + 10h]
        push   esi
        mov    esi, 10h
        cmp    byte ptr [edx + 2], 0
        je     SHORT loc_1_752D
        mov    esi, dword ptr [edx + 0fh]
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 10h]
        jmp    SHORT loc_1_754A
loc_1_752D:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 34h]
        test   ecx, ecx
        je     SHORT loc_1_7540
        mov    esi, dword ptr [ecx]
        mov    ecx, dword ptr [esp + 8]
        jmp    SHORT loc_1_754A
loc_1_7540:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], 0c0000001h
loc_1_754A:
        cmp    esi, 8
        jae    SHORT loc_1_7568
        mov    eax, dword ptr [eax + 74h]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        push   edx
        mov    edx, dword ptr [esp + 18h]
        push   esi
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_7565:
        add    esp, 18h
loc_1_7568:
        pop    esi
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_1_7570:
        mov    eax, dword ptr [esp + 14h]
        push   0
        mov    ecx, dword ptr [esp + 14h]
        push   eax
        push   ecx
        call   sub_1_6616
        mov    ecx, dword ptr [esp + 14h]
        add    esp, 0ch
        test   eax, eax
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 4]
        je     SHORT loc_1_759B
        mov    dword ptr [eax], 0
        ret    14h
loc_1_759B:
        mov    dword ptr [eax], 0c0000001h
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_75B0:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 32h
        ret    4
        db  08Dh, 049h, 000h
loc_1_75C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    eax, eax
        xor    edx, edx
        db  0CDh, 020h, 007h, 000h, 005h, 000h
        shl    eax, 3
        xor    edx, edx
        db  003h, 005h, 000h, 000h, 000h, 000h
        db  013h, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], edx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_7600:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_3370
        ret    4
        db  08Dh, 049h, 000h
loc_1_7610:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_3390
        ret    4
        db  08Dh, 049h, 000h
loc_1_7620:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [ecx + 5ch], eax
        je     SHORT loc_1_763A
        push   eax
        push   28h
        call   sub_1_6563
        add    esp, 8
loc_1_763A:
        test   eax, eax
        jne    SHORT loc_1_7647
        mov    eax, 0c0000001h
        pop    esi
        ret    10h
loc_1_7647:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dl, byte ptr [esp + 10h]
        push   eax
        mov    dword ptr [eax + 10h], esi
        mov    esi, dword ptr [esp + 18h]
        db  068h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 14h], ecx
        mov    byte ptr [eax + 18h], dl
        mov    dword ptr [eax + 20h], esi
        call   sub_1_A2F0
        mov    eax, 103h
        pop    esi
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7680:
        sub    esp, 10h
        mov    dword ptr [esp], edx
        lea    eax, [esp + 8]
        lea    ecx, [esp + 4]
        mov    edx, dword ptr [esp]
        push   eax
        push   ecx
        mov    al, byte ptr [edx + 18h]
        push   eax
        mov    ecx, dword ptr [edx + 14h]
        push   ecx
        mov    eax, dword ptr [edx + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_76A6:
        mov    edx, dword ptr [esp]
        mov    ecx, dword ptr [edx + 10h]
        mov    eax, dword ptr [edx + 20h]
        push   eax
        mov    eax, dword ptr [edx + 14h]
        lea    edx, [esp + 0ch]
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   edx
        push   eax
        mov    edx, dword ptr [ecx + 0ch]
        push   edx
        mov    eax, dword ptr [ecx + 8]
        call   dword ptr [eax + 5ch]
        mov    ecx, dword ptr [esp]
        push   0
        push   ecx
        call   sub_1_65A5
        add    esp, 18h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_76E0:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 28h]
        add    ecx, 8
        push   ecx
        mov    dword ptr [ecx - 8], edx
        mov    dword ptr [ecx], 1
        call   sub_1_68A9
        add    esp, 4
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7710:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax + 2eh], 0
        je     SHORT loc_1_7726
        xor    cl, cl
        cmp    cl, 1
        sbb    al, al
        inc    al
        ret    8
loc_1_7726:
        mov    ecx, 1
        mov    byte ptr [eax + 2eh], cl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_7759
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_7759
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 04Ah, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
loc_1_7759:
        cmp    cl, 1
        sbb    al, al
        inc    al
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7770:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   dword ptr [esi + 118h]
        lea    eax, [esi + 2eh]
        mov    byte ptr [eax], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_77B2
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_77B2
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 08Dh, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_77B2:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_1_77C5
        mov    byte ptr [esi + 2ch], 0
        push   eax
        call   sub_1_689B
        add    esp, 4
loc_1_77C5:
        pop    esi
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_77D0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        lea    esi, [edi + 2eh]
        cmp    byte ptr [esi], 0
        je     SHORT loc_1_77E3
        xor    bl, bl
        jmp    SHORT loc_1_7815
loc_1_77E3:
        mov    ebx, 1
        mov    byte ptr [esi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_7815
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_7815
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0B6h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_7815:
        mov    eax, dword ptr [esp + 18h]
        test   bl, bl
        push   eax
        jne    SHORT loc_1_7845
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, 9
        push   ecx
        mov    ecx, edi
        call   dword ptr [edi + 120h]
        pop    edi
        cmp    eax, 1
        sbb    eax, eax
        pop    esi
        and    eax, 40000102h
        pop    ebx
        sub    eax, 3fffffffh
        ret    0ch
loc_1_7845:
        push   edi
        call   dword ptr [esp + 1ch]
        mov    ecx, edi
        call   dword ptr [edi + 118h]
        test   bl, bl
        je     SHORT loc_1_789B
        mov    byte ptr [esi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_7888
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_7888
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0E6h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_7888:
        cmp    byte ptr [edi + 2ch], 1
        jne    SHORT loc_1_789B
        mov    byte ptr [edi + 2ch], 0
        push   esi
        call   sub_1_689B
        add    esp, 4
loc_1_789B:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 056h, 057h, 08Bh
        db  084h, 091h, 038h, 001h, 000h, 000h, 08Dh, 03Ch, 091h, 08Bh, 0F0h, 085h, 0C0h, 074h, 008h, 08Bh
        db  000h, 089h, 087h, 038h, 001h, 000h, 000h, 085h, 0F6h, 074h, 01Dh, 08Bh, 044h, 024h, 00Ch, 089h
        db  056h, 004h, 08Bh, 054h, 024h, 010h, 089h, 046h, 008h, 089h, 056h, 00Ch, 08Bh, 047h, 044h, 089h
        db  006h, 089h, 077h, 044h, 033h, 0F6h, 0EBh, 005h, 0BEh, 003h, 000h, 001h, 000h, 081h, 0FEh, 003h
        db  000h, 001h, 000h, 074h, 00Eh, 06Ah, 00Fh, 08Bh, 081h, 028h, 001h, 000h, 000h, 050h, 0E8h, 05Bh
        db  0EDh, 0FFh, 0FFh, 08Bh, 0C6h, 05Fh, 05Eh, 0C2h, 008h, 000h, 08Dh, 049h, 000h, 033h, 0C0h, 0C2h
        db  008h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 053h, 056h, 08Bh
        db  041h, 06Ch, 057h, 08Bh, 0DAh, 08Bh, 0F1h, 08Bh, 0C8h, 085h, 0C9h, 074h, 005h, 08Bh, 009h, 089h
        db  04Eh, 06Ch, 085h, 0C0h, 075h, 01Dh, 06Ah, 000h, 06Ah, 010h, 0E8h, 021h, 0ECh, 0FFh, 0FFh, 083h
        db  0C4h, 008h, 085h, 0C0h, 075h, 007h, 0BFh, 001h, 000h, 000h, 0C0h, 0EBh, 02Eh, 0FFh, 086h, 0D0h
        db  002h, 000h, 000h, 033h, 0FFh, 08Bh, 04Ch, 024h, 010h, 08Bh, 054h, 024h, 014h, 089h, 038h, 089h
        db  078h, 004h, 089h, 078h, 008h, 089h, 078h, 00Ch, 089h, 058h, 004h, 089h, 048h, 008h, 08Dh, 04Ch
        db  09Eh, 044h, 089h, 050h, 00Ch, 08Bh, 019h, 089h, 018h, 089h, 001h, 06Ah, 00Fh, 08Bh, 086h, 028h
        db  001h, 000h, 000h, 050h, 0E8h, 0D5h, 0ECh, 0FFh, 0FFh, 08Bh, 0C7h, 05Fh, 05Eh, 05Bh, 0C2h, 008h
        db  000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 033h, 0C0h, 0C2h
        db  008h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_79B0:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ecx + edx*4 + 44h]
        lea    edi, [ecx + edx*4]
        mov    esi, eax
        test   eax, eax
        je     SHORT loc_1_79C5
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi + 44h], eax
loc_1_79C5:
        test   esi, esi
        je     SHORT loc_1_7A3D
        mov    eax, dword ptr [esp + 10h]
        test   eax, eax
        je     SHORT loc_1_79D6
        mov    ebx, dword ptr [esi + 8]
        mov    dword ptr [eax], ebx
loc_1_79D6:
        mov    eax, dword ptr [esp + 14h]
        test   eax, eax
        je     SHORT loc_1_79E3
        mov    ebx, dword ptr [esi + 0ch]
        mov    dword ptr [eax], ebx
loc_1_79E3:
        dec    edx
        cmp    edx, 8
        ja     SHORT loc_1_79F8
        xor    eax, eax
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_79F8:
        mov    eax, dword ptr [edi + 138h]
        mov    dword ptr [esi], eax
        mov    dword ptr [edi + 138h], esi
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_1_7A0C:
        mov    dword ptr [esi + 4], 6
        mov    eax, dword ptr [ecx + 5ch]
        pop    edi
        mov    dword ptr [esi], eax
        mov    dword ptr [ecx + 5ch], esi
        pop    esi
        pop    ebx
        ret    8
loc_1_7A21:
        mov    eax, dword ptr [ecx + 13ch]
        pop    edi
        mov    dword ptr [esi], eax
        mov    dword ptr [ecx + 13ch], esi
        pop    esi
        pop    ebx
        ret    8
loc_1_7A35:
        mov    eax, dword ptr [ecx + 6ch]
        mov    dword ptr [esi], eax
        mov    dword ptr [ecx + 6ch], esi
loc_1_7A3D:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 003h, 003h, 003h, 003h, 001h, 002h, 002h, 002h, 08Dh, 049h, 000h
loc_1_7A60:
        push   esi
        mov    eax, dword ptr [ecx + edx*4 + 138h]
        lea    esi, [ecx + edx*4]
        test   eax, eax
        mov    ecx, eax
        je     SHORT loc_1_7A79
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 138h], eax
loc_1_7A79:
        test   ecx, ecx
        je     SHORT loc_1_7A9C
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], edx
        mov    eax, dword ptr [esi + 44h]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        mov    dword ptr [esi + 44h], ecx
        pop    esi
        ret    8
loc_1_7A9C:
        mov    eax, 10003h
        pop    esi
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 033h, 0C0h, 0C2h, 008h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7AC0:
        push   ebx
        push   esi
        mov    eax, dword ptr [ecx + 6ch]
        push   edi
        mov    edi, edx
        mov    esi, ecx
        mov    ecx, eax
        test   ecx, ecx
        je     SHORT loc_1_7AD5
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [esi + 6ch], ecx
loc_1_7AD5:
        test   eax, eax
        jne    SHORT loc_1_7AFA
        push   0
        push   10h
        call   sub_1_6563
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_7AF4
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_1_7AF4:
        inc    dword ptr [esi + 2d0h]
loc_1_7AFA:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], ecx
        lea    ecx, [esi + edi*4 + 44h]
        pop    edi
        mov    dword ptr [eax + 0ch], edx
        mov    ebx, dword ptr [ecx]
        pop    esi
        mov    dword ptr [eax], ebx
        pop    ebx
        mov    dword ptr [ecx], eax
        xor    eax, eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 033h, 0C0h, 0C2h, 008h, 000h, 08Dh, 064h, 024h
        db  000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h
        db  000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7B60:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
loc_1_7B68:
        xor    bl, bl
        cmp    dword ptr [esi + 5ch], 0
        jne    NEAR PTR loc_1_7CA9
        cmp    dword ptr [esi + 54h], 0
        jne    NEAR PTR loc_1_7D0B
        cmp    dword ptr [esi + 68h], 0
        je     SHORT loc_1_7BAA
        lea    eax, [esp + 18h]
        lea    ecx, [esp + 1ch]
        push   eax
        mov    edx, 9
        push   ecx
        mov    bl, 1
        mov    ecx, esi
        call   dword ptr [esi + 124h]
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esi + 0ch]
        push   ecx
        push   edx
        call   dword ptr [esp + 24h]
loc_1_7BAA:
        cmp    dword ptr [esi + 44h], 0
        jne    NEAR PTR loc_1_7CF0
        cmp    dword ptr [esi + 60h], 0
        jne    NEAR PTR loc_1_7CDC
        cmp    dword ptr [esi + 64h], 0
        je     SHORT loc_1_7C00
        lea    eax, [esp + 14h]
        push   0
        push   eax
        mov    edx, 8
        mov    ecx, esi
        call   dword ptr [esi + 124h]
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_7BE3:
        cmp    eax, 103h
        jne    SHORT loc_1_7BFE
        mov    eax, dword ptr [esp + 14h]
        push   0
        push   eax
        mov    edx, 8
        mov    ecx, esi
        call   dword ptr [esi + 120h]
loc_1_7BFE:
        mov    bl, 1
loc_1_7C00:
        cmp    dword ptr [esi + 48h], 0
        je     SHORT loc_1_7C3B
        lea    eax, [esp + 0fh]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_7C11:
        cmp    eax, 103h
        mov    edi, eax
        je     NEAR PTR loc_1_7CEA
        mov    eax, dword ptr [esp + 0fh]
        push   eax
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_7C2A:
        cmp    byte ptr [esp + 0fh], 0
        je     SHORT loc_1_7C35
        test   edi, edi
        je     SHORT loc_1_7C9D
loc_1_7C35:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_7C3B:
        cmp    dword ptr [esi + 4ch], 0
        je     SHORT loc_1_7C5A
        push   esi
        mov    bl, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_7C49:
        push   0
        mov    edx, 2
        push   0
        mov    ecx, esi
        call   dword ptr [esi + 124h]
loc_1_7C5A:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_7C7E
        mov    ecx, esi
        call   sub_1_94D0
        test   al, al
        jne    SHORT loc_1_7C7C
        push   0
        mov    edx, 5
        push   0
        mov    ecx, esi
        call   dword ptr [esi + 124h]
loc_1_7C7C:
        mov    bl, 1
loc_1_7C7E:
        cmp    dword ptr [esi + 50h], 0
        je     SHORT loc_1_7C9F
        mov    ecx, esi
        call   dword ptr [esi + 160h]
        push   0
        mov    edx, 3
        push   0
        mov    ecx, esi
        call   dword ptr [esi + 124h]
loc_1_7C9D:
        mov    bl, 1
loc_1_7C9F:
        test   bl, bl
        jne    NEAR PTR loc_1_7B68
        jmp    SHORT loc_1_7D0B
loc_1_7CA9:
        cmp    dword ptr [esi + 60h], 0
        je     SHORT loc_1_7CBD
        lea    eax, [esp + 10h]
        push   0
        push   eax
        mov    edx, 7
        jmp    SHORT loc_1_7CF9
loc_1_7CBD:
        cmp    dword ptr [esi + 4ch], 0
        je     SHORT loc_1_7D0B
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_7CC9:
        push   0
        mov    edx, 2
        push   0
        mov    ecx, esi
        call   dword ptr [esi + 124h]
        jmp    SHORT loc_1_7D0B
loc_1_7CDC:
        lea    eax, [esp + 10h]
        push   0
        push   eax
        mov    edx, 7
        jmp    SHORT loc_1_7CF9
loc_1_7CEA:
        cmp    dword ptr [esi + 44h], 0
        je     SHORT loc_1_7D0B
loc_1_7CF0:
        lea    eax, [esp + 10h]
        push   0
        push   eax
        xor    edx, edx
loc_1_7CF9:
        mov    ecx, esi
        call   dword ptr [esi + 124h]
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_1_8630
loc_1_7D0B:
        and    dword ptr [esi + 3ch], 0dfffffffh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7D20:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        cmp    esi, 40010006h
        jne    SHORT loc_1_7D50
        mov    ebx, dword ptr [esp + 20h]
        cmp    ebx, 4
        jne    SHORT loc_1_7D5E
        mov    edi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [edi + 1], 0c8h
        je     SHORT loc_1_7D66
        mov    byte ptr [eax + 170h], 0fh
        jmp    SHORT loc_1_7D66
loc_1_7D50:
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [esp + 20h]
        jmp    SHORT loc_1_7D66
loc_1_7D5E:
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 1ch]
loc_1_7D66:
        mov    ebp, dword ptr [eax + 18h]
        test   ebp, ebp
        je     SHORT loc_1_7D81
loc_1_7D6D:
        push   ebx
        mov    eax, dword ptr [ebp + 10h]
        push   edi
        push   esi
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 5ch]
        mov    ebp, dword ptr [ebp + 18h]
        test   ebp, ebp
        jne    SHORT loc_1_7D6D
loc_1_7D81:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_7D90:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 18h]
        test   esi, esi
        je     SHORT loc_1_7DAD
loc_1_7D9C:
        mov    eax, dword ptr [esi + 10h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        call   dword ptr [eax + 60h]
        mov    esi, dword ptr [esi + 18h]
        test   esi, esi
        jne    SHORT loc_1_7D9C
loc_1_7DAD:
        pop    esi
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7DC0:
        mov    eax, dword ptr [esp + 8]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [eax + 18h]
        test   ebp, ebp
        je     SHORT loc_1_7DF1
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [esp + 20h]
loc_1_7DDB:
        mov    eax, dword ptr [esp + 24h]
        push   eax
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 54h]
        mov    dword ptr [esi], eax
        mov    ebp, dword ptr [ebp + 18h]
        test   ebp, ebp
        jne    SHORT loc_1_7DDB
loc_1_7DF1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_7E00:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 18h]
        test   edi, edi
        je     SHORT loc_1_7E1F
        mov    esi, dword ptr [esp + 10h]
loc_1_7E11:
        push   esi
        mov    eax, dword ptr [edi + 8]
        call   dword ptr [eax + 58h]
        mov    edi, dword ptr [edi + 18h]
        test   edi, edi
        jne    SHORT loc_1_7E11
loc_1_7E1F:
        pop    edi
        pop    esi
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7E30:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        push   edi
        mov    eax, 0c00000bbh
        mov    edx, dword ptr [ecx + 10h]
        mov    esi, dword ptr [edx + 4]
        cmp    dword ptr [edx + 0ch], ecx
        jne    SHORT loc_1_7EA6
        test   byte ptr [esi + 3dh], 10h
        je     SHORT loc_1_7EA6
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 10h]
        mov    cx, word ptr [eax + 1eh]
        lea    esi, [ecx + eax]
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax], ecx
        cmp    dword ptr [esp + 20h], ecx
        mov    eax, 0c0010016h
        jb     SHORT loc_1_7E7E
        push   ecx
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esp + 20h]
        push   eax
        push   ecx
        call   sub_1_64BC
        xor    eax, eax
loc_1_7E7E:
        mov    ecx, dword ptr [esp + 14h]
        test   ecx, ecx
        je     SHORT loc_1_7E90
        mov    edx, dword ptr [esi]
        mov    edi, dword ptr [esi + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], edi
loc_1_7E90:
        mov    edx, dword ptr [esp + 18h]
        test   edx, edx
        je     SHORT loc_1_7EA6
        mov    edi, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0ch]
        add    esi, 8
        mov    dword ptr [edx], edi
        mov    dword ptr [edx + 4], ecx
loc_1_7EA6:
        pop    edi
        pop    esi
        ret    1ch
        db  005h, 000h, 000h, 000h, 000h
loc_1_7EB0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        test   esi, esi
        je     SHORT loc_1_7EDD
        mov    edi, dword ptr [esp + 0ch]
loc_1_7EBE:
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [eax + 2ch]
        mov    ecx, dword ptr [eax + 30h]
        dec    ecx
        mov    dword ptr [eax + 30h], ecx
        jne    SHORT loc_1_7ED7
        mov    ecx, dword ptr [edx + 8]
        push   eax
        mov    eax, dword ptr [edx + 0ch]
        push   eax
        call   dword ptr [ecx + 54h]
loc_1_7ED7:
        add    edi, 4
        dec    esi
        jne    SHORT loc_1_7EBE
loc_1_7EDD:
        pop    edi
        pop    esi
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7EF0:
        mov    eax, dword ptr [esp + 8]
        sub    esp, 20h
        mov    ecx, dword ptr [esp + 24h]
        push   ebx
        mov    dword ptr [esp + 14h], eax
        push   esi
        mov    eax, dword ptr [esp + 34h]
        push   edi
        mov    dword ptr [esp + 20h], 0
        or     dword ptr [ecx + 3ch], 1000h
        push   ebp
        test   eax, eax
        je     NEAR PTR loc_1_8015
        mov    dword ptr [esp + 28h], eax
loc_1_7F22:
        mov    eax, dword ptr [esp + 20h]
        mov    esi, dword ptr [eax]
        xor    eax, eax
        mov    ax, word ptr [esi + 1eh]
        add    eax, esi
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esi + 8]
        mov    edi, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 1ch], ecx
        test   eax, eax
        je     SHORT loc_1_7F54
loc_1_7F47:
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax]
        add    dword ptr [esp + 1ch], ecx
        test   eax, eax
        jne    SHORT loc_1_7F47
loc_1_7F54:
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [eax + 1ch], 0c000009ah
        je     SHORT loc_1_7F6F
        mov    dword ptr [eax + 1ch], 0
        mov    byte ptr [esp + 13h], 0
        jmp    SHORT loc_1_7F74
loc_1_7F6F:
        mov    byte ptr [esp + 13h], 1
loc_1_7F74:
        mov    dword ptr [esi + 30h], 0
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [esi + 2ch], eax
        mov    byte ptr [esi + 1ch], 0
        mov    ebx, dword ptr [eax + 18h]
        test   ebx, ebx
        je     SHORT loc_1_8006
loc_1_7F8D:
        mov    eax, dword ptr [ebx + 18h]
        inc    dword ptr [esp + 24h]
        mov    byte ptr [ebx + 68h], 1
        mov    dword ptr [esp + 14h], eax
        inc    dword ptr [ebx + 34h]
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_7FBB
        mov    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [eax + 54h]
        test   ecx, ecx
        je     SHORT loc_1_7FBB
        push   esi
        mov    eax, dword ptr [eax + 24h]
        push   eax
        call   ecx
        mov    ebp, eax
        jmp    SHORT loc_1_7FE1
loc_1_7FBB:
        xor    ebp, ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [eax + 10h]
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, ecx
        lea    edx, [ecx + edi]
        push   eax
        push   eax
        lea    eax, [esp + 34h]
        push   edx
        push   ecx
        push   edi
        push   esi
        mov    ecx, dword ptr [ebx + 0ch]
        push   ecx
        push   eax
        call   sub_1_8EB0
loc_1_7FE1:
        test   ebp, ebp
        je     SHORT loc_1_7FFB
        xor    eax, eax
        mov    ax, word ptr [esi + 1eh]
        mov    byte ptr [esp + 13h], 1
        mov    dword ptr [eax + esi + 1ch], 103h
        add    dword ptr [esi + 30h], ebp
loc_1_7FFB:
        dec    dword ptr [ebx + 34h]
        mov    ebx, dword ptr [esp + 14h]
        test   ebx, ebx
        jne    SHORT loc_1_7F8D
loc_1_8006:
        add    dword ptr [esp + 20h], 4
        dec    dword ptr [esp + 28h]
        jne    NEAR PTR loc_1_7F22
loc_1_8015:
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_8042
        mov    eax, dword ptr [esp + 34h]
        mov    esi, dword ptr [eax + 18h]
        test   esi, esi
        je     SHORT loc_1_8042
loc_1_8027:
        cmp    dword ptr [esi + 54h], 0
        je     SHORT loc_1_803A
        inc    dword ptr [esi + 34h]
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   dword ptr [esi + 54h]
        dec    dword ptr [esi + 34h]
loc_1_803A:
        mov    esi, dword ptr [esp + 14h]
        test   esi, esi
        jne    SHORT loc_1_8027
loc_1_8042:
        mov    eax, dword ptr [esp + 34h]
        pop    ebp
        pop    edi
        pop    esi
        and    dword ptr [eax + 3ch], 0ffffefffh
        pop    ebx
        add    esp, 20h
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8060:
        mov    edx, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edx + 1cch]
        cmp    esi, eax
        jne    SHORT loc_1_807E
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [edx + 1cch], eax
        jmp    SHORT loc_1_809F
loc_1_807E:
        cmp    dword ptr [eax + 2ch], esi
        je     SHORT loc_1_808B
loc_1_8083:
        mov    eax, dword ptr [eax + 2ch]
        cmp    dword ptr [eax + 2ch], esi
        jne    SHORT loc_1_8083
loc_1_808B:
        mov    ecx, dword ptr [esi + 2ch]
        mov    dword ptr [eax + 2ch], ecx
        cmp    dword ptr [edx + 1d0h], esi
        jne    SHORT loc_1_809F
        mov    dword ptr [edx + 1d0h], eax
loc_1_809F:
        mov    ecx, dword ptr [esi + 30h]
        mov    eax, dword ptr [esp + 14h]
        mov    edx, dword ptr [esp + 10h]
        push   eax
        push   edx
        mov    eax, dword ptr [ecx + 8]
        push   esi
        mov    esi, dword ptr [ecx + 10h]
        push   esi
        call   dword ptr [eax + 48h]
        pop    esi
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_1_80C0:
        mov    edx, dword ptr [esp + 14h]
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        push   esi
        push   edi
        lea    ecx, [edx + 2ch]
        mov    eax, dword ptr [ebx + 4]
        mov    edi, dword ptr [eax + 1d4h]
        test   edi, edi
        jne    SHORT loc_1_8106
        mov    edi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 14h]
        push   edi
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], ebx
        mov    ecx, dword ptr [esp + 1ch]
        push   ecx
        mov    esi, dword ptr [ebx + 14h]
        mov    ecx, dword ptr [esp + 2ch]
        push   eax
        push   esi
        push   ecx
        push   edx
        call   dword ptr [ebx + 44h]
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
loc_1_8106:
        xor    eax, eax
        mov    ecx, dword ptr [esp + 14h]
        mov    ax, word ptr [ecx + 1eh]
        mov    esi, dword ptr [esp + 24h]
        push   esi
        mov    eax, dword ptr [eax + ecx + 10h]
        add    eax, dword ptr [esp + 1ch]
        push   eax
        push   edi
        mov    edi, dword ptr [esp + 28h]
        push   edi
        push   0
        push   edx
        call   sub_1_7170
        mov    eax, dword ptr [esi]
        sub    eax, edi
        pop    edi
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        pop    ebx
        adc    eax, -1
        and    eax, 0c0000001h
        ret    18h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8150:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [eax + 4]
        mov    esi, dword ptr [esp + 24h]
        mov    ecx, dword ptr [ebx + 1d4h]
        test   ecx, ecx
        lea    edx, [esi + 2ch]
        jne    NEAR PTR loc_1_81F6
        mov    dword ptr [edx], 0
        mov    dword ptr [edx + 4], eax
        cmp    dword ptr [ebx + 1cch], 0
        mov    ebp, dword ptr [ebx + 1d0h]
        jne    SHORT loc_1_8190
        mov    dword ptr [ebx + 1cch], esi
        jmp    SHORT loc_1_8193
loc_1_8190:
        mov    dword ptr [ebp + 2ch], esi
loc_1_8193:
        mov    ecx, dword ptr [esp + 20h]
        mov    edi, dword ptr [esp + 18h]
        push   ecx
        mov    dword ptr [ebx + 1d0h], esi
        mov    eax, dword ptr [edx + 4]
        mov    edx, dword ptr [esp + 20h]
        push   edx
        push   edi
        mov    ecx, dword ptr [eax + 14h]
        mov    edx, dword ptr [esp + 34h]
        push   ecx
        push   edx
        push   esi
        call   dword ptr [eax + 44h]
        cmp    eax, 103h
        je     SHORT loc_1_8232
        cmp    dword ptr [ebx + 1cch], esi
        je     SHORT loc_1_81DB
        mov    dword ptr [ebp + 2ch], 0
        mov    dword ptr [ebx + 1d0h], ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
loc_1_81DB:
        mov    dword ptr [ebx + 1cch], 0
        mov    dword ptr [ebx + 1d0h], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
loc_1_81F6:
        xor    eax, eax
        mov    edx, dword ptr [esp + 18h]
        mov    ax, word ptr [edx + 1eh]
        mov    edi, dword ptr [esp + 28h]
        push   edi
        mov    eax, dword ptr [eax + edx + 10h]
        add    eax, dword ptr [esp + 20h]
        push   eax
        push   ecx
        mov    ecx, dword ptr [esp + 2ch]
        push   ecx
        push   0
        push   esi
        call   sub_1_7170
        mov    eax, dword ptr [edi]
        sub    eax, dword ptr [esp + 20h]
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0c0000001h
loc_1_8232:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8240:
        push   esi
        xor    eax, eax
        mov    edx, dword ptr [esp + 0ch]
        push   edi
        mov    ax, word ptr [edx + 1eh]
        mov    esi, dword ptr [esp + 20h]
        mov    edi, dword ptr [esp + 18h]
        push   esi
        mov    ecx, dword ptr [eax + edx + 10h]
        mov    eax, dword ptr [esp + 20h]
        add    ecx, dword ptr [esp + 18h]
        push   ecx
        push   edx
        push   edi
        push   0
        push   eax
        call   sub_1_7170
        mov    eax, dword ptr [esi]
        sub    eax, edi
        pop    edi
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0c0000001h
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8290:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    edx, 3
        push   0
        mov    ecx, esi
        test   byte ptr [esi + 3dh], 20h
        push   0
        call   dword ptr [esi + 124h]
        mov    edi, dword ptr [esi + 1a8h]
        mov    eax, dword ptr [esi + 1b0h]
        mov    dword ptr [esp + 10h], eax
        xor    eax, eax
        mov    dword ptr [esi + 1b4h], eax
        mov    dword ptr [esi + 1b0h], eax
        mov    dword ptr [esi + 1a8h], eax
        mov    dword ptr [esi + 1ach], eax
        test   edi, edi
        je     SHORT loc_1_8335
loc_1_82DE:
        mov    ebx, dword ptr [edi + 2ch]
        mov    ebp, dword ptr [edi + 30h]
        cmp    dword ptr [esp + 10h], edi
        jne    SHORT loc_1_82F2
        mov    dword ptr [esp + 10h], 0
loc_1_82F2:
        cmp    dword ptr [esi + 0fch], 7
        jne    SHORT loc_1_8309
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_8309
        push   edi
        push   esi
        call   sub_1_4900
loc_1_8309:
        push   0c001000ch
        mov    eax, dword ptr [ebp + 10h]
        push   edi
        push   eax
        mov    edi, dword ptr [ebp + 8]
        call   dword ptr [edi + 44h]
        test   byte ptr [esi + 3dh], 20h
        jne    SHORT loc_1_832F
        mov    eax, dword ptr [ebp + 34h]
        dec    eax
        mov    dword ptr [ebp + 34h], eax
        jne    SHORT loc_1_832F
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_832F:
        mov    edi, ebx
        test   ebx, ebx
        jne    SHORT loc_1_82DE
loc_1_8335:
        push   0
        mov    edx, 2
        push   0
        mov    ecx, esi
        test   byte ptr [esi + 3dh], 20h
        call   dword ptr [esi + 124h]
        mov    ebx, dword ptr [esi + 1dch]
        mov    eax, dword ptr [esi + 1e0h]
        and    dword ptr [esi + 3ch], 0fffffdffh
        mov    dword ptr [esi + 1dch], 0
        test   eax, eax
        je     SHORT loc_1_83A6
        cmp    dword ptr [eax + 4], 0
        je     SHORT loc_1_838E
        cmp    dword ptr [eax + 10h], 2
        jne    SHORT loc_1_838E
        push   0c001000ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8382:
        mov    dword ptr [esi + 1e0h], 0
        jmp    SHORT loc_1_83A6
loc_1_838E:
        cmp    dword ptr [eax + 10h], 1
        push   0c001000ch
        push   esi
        jne    SHORT loc_1_83A1
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_839F:
        jmp    SHORT loc_1_83A6
loc_1_83A1:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_83A6:
        test   ebx, ebx
        je     SHORT loc_1_83E3
loc_1_83AA:
        cmp    dword ptr [ebx + 10h], 2
        mov    edi, dword ptr [ebx]
        jne    SHORT loc_1_83BF
        push   0c001000ch
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_83BD:
        jmp    SHORT loc_1_83DD
loc_1_83BF:
        push   0c001000ch
        mov    dword ptr [esi + 1e0h], ebx
        cmp    dword ptr [ebx + 10h], 1
        push   esi
        jne    SHORT loc_1_83D8
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_83D6:
        jmp    SHORT loc_1_83DD
loc_1_83D8:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_83DD:
        mov    ebx, edi
        test   edi, edi
        jne    SHORT loc_1_83AA
loc_1_83E3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
        db  08Dh, 049h, 000h
sub_1_83F0:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 0ch]
        test   edx, edx
        jne    SHORT loc_1_8403
        mov    eax, 1
        ret    4
loc_1_8403:
        mov    ecx, dword ptr [eax + 4]
        lea    eax, [edx + ecx]
        shr    ecx, 0ch
        dec    eax
        shr    eax, 0ch
        sub    eax, ecx
        inc    eax
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8420:
        mov    ecx, dword ptr [esp + 8]
        sub    esp, 200h
        xor    eax, eax
        push   ebx
        push   esi
        cmp    dword ptr [ecx + 4], eax
        push   edi
        jne    SHORT loc_1_8453
        cmp    dword ptr [ecx + 0ch], eax
        jne    SHORT loc_1_8453
        mov    edi, dword ptr [esp + 220h]
        mov    esi, dword ptr [esp + 224h]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [esi], eax
        jmp    SHORT loc_1_8492
loc_1_8453:
        mov    esi, dword ptr [esp + 224h]
        lea    eax, [esp + 0ch]
        push   esi
        push   eax
        push   ecx
        call   sub_1_6EB0
        xor    ecx, ecx
        cmp    dword ptr [esi], ecx
        jbe    SHORT loc_1_8492
        mov    edi, dword ptr [esp + 220h]
        lea    edx, [esp + 0ch]
        xor    ebx, ebx
loc_1_8479:
        mov    eax, dword ptr [edx]
        add    edi, 10h
        add    edx, 8
        inc    ecx
        mov    dword ptr [edi - 10h], eax
        mov    eax, dword ptr [edx - 4]
        mov    dword ptr [edi - 8], eax
        mov    dword ptr [edi - 0ch], ebx
        cmp    dword ptr [esi], ecx
        ja     SHORT loc_1_8479
loc_1_8492:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 200h
        ret    18h
        db  08Bh, 0FFh
loc_1_84A0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [eax + 4]
        and    edx, 0fffh
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], edx
        ret    0ch
        db  090h
loc_1_84C0:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0ch], ecx
        ret    8
        db  08Bh, 0FFh
loc_1_84D0:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        mov    edi, dword ptr [esp + 1ch]
        push   ebp
        mov    ebp, dword ptr [esi + 0ch]
        cmp    ebp, edi
        jb     NEAR PTR loc_1_85A2
        mov    ecx, dword ptr [esi + 14h]
        sub    ebp, ecx
        cmp    ebp, edi
        jb     SHORT loc_1_854F
        mov    eax, dword ptr [esp + 1ch]
        push   edi
        push   eax
        lea    eax, [esi + ecx]
        add    eax, 1ch
        push   eax
        call   sub_1_64BC
        mov    edx, dword ptr [esi + 10h]
        add    edx, edi
        mov    dword ptr [esi + 10h], edx
        mov    ecx, dword ptr [esi + 14h]
        mov    eax, dword ptr [esi + 18h]
        cmp    ecx, eax
        jae    SHORT loc_1_8539
        lea    ebx, [edi + ecx]
        cmp    ebx, eax
        jbe    SHORT loc_1_8539
        sub    ecx, eax
        add    ecx, edx
        mov    dword ptr [esi + 10h], ecx
        mov    eax, dword ptr [esi + 14h]
        add    eax, edi
        mov    dword ptr [esi + 18h], eax
        cmp    dword ptr [esi + 0ch], eax
        jne    SHORT loc_1_8539
        mov    dword ptr [esi + 18h], 0
loc_1_8539:
        mov    eax, dword ptr [esi + 14h]
        add    eax, edi
        mov    dword ptr [esi + 14h], eax
        cmp    dword ptr [esi + 0ch], eax
        jne    SHORT loc_1_85AA
        mov    dword ptr [esi + 14h], 0
        jmp    SHORT loc_1_85AA
loc_1_854F:
        mov    eax, dword ptr [esp + 1ch]
        push   ebp
        push   eax
        mov    ebx, edi
        lea    eax, [esi + ecx]
        sub    ebx, ebp
        add    eax, 1ch
        push   eax
        call   sub_1_64BC
        mov    eax, dword ptr [esp + 1ch]
        push   ebx
        add    eax, ebp
        lea    ecx, [esi + 1ch]
        push   eax
        push   ecx
        call   sub_1_64BC
        mov    ecx, dword ptr [esi + 10h]
        add    ecx, edi
        mov    dword ptr [esi + 10h], ecx
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, ecx
        jae    SHORT loc_1_8588
        mov    dword ptr [esi + 10h], eax
loc_1_8588:
        mov    eax, dword ptr [esi + 18h]
        cmp    dword ptr [esi + 14h], eax
        jb     SHORT loc_1_8594
        cmp    ebx, eax
        jbe    SHORT loc_1_859D
loc_1_8594:
        mov    dword ptr [esi + 18h], ebx
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 10h], eax
loc_1_859D:
        mov    dword ptr [esi + 14h], ebx
        jmp    SHORT loc_1_85AA
loc_1_85A2:
        mov    dword ptr [esp + 10h], 0c0000001h
loc_1_85AA:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_85C0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     SHORT loc_1_85DE
        push   eax
        cmp    byte ptr [esi], 0
        jne    SHORT loc_1_85D9
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_85D7:
        jmp    SHORT loc_1_85DE
loc_1_85D9:
        call   sub_1_A308
loc_1_85DE:
        mov    al, 1
        mov    byte ptr [esi], 0
        mov    dword ptr [esi + 4], 0
        mov    dword ptr [esi + 24h], 0
        pop    esi
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8600:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        push   0
        push   0
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esp + 8]
        push   eax
        add    eax, 28h
        push   eax
        call   sub_1_8750
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_8630:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    al, byte ptr [esi + 2]
        test   al, al
        je     SHORT loc_1_8642
        xor    al, al
        pop    esi
        ret    4
loc_1_8642:
        inc    al
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 2], al
        call   sub_1_A2F0
        mov    dword ptr [esi + 14h], eax
        mov    al, 1
        pop    esi
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_8660:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [esp]
        mov    ecx, dword ptr [eax + 18h]
        mov    edx, dword ptr [eax + 14h]
        push   ecx
        push   edx
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        push   eax
        call   dword ptr [eax + 0ch]
        add    esp, 4
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8690:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 4], 0
        jne    SHORT loc_1_86B1
        mov    eax, dword ptr [esp + 0ch]
        push   esi
        push   eax
        mov    dword ptr [esi + 24h], eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_86AE:
        mov    dword ptr [esi + 4], eax
loc_1_86B1:
        pop    esi
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_86C0:
        sub    esp, 4
        push   esi
        mov    dword ptr [esp + 4], edx
        mov    eax, dword ptr [esp + 4]
        push   0
        push   0
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        add    eax, 28h
        push   eax
        call   sub_1_8750
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 24h]
        test   eax, eax
        je     SHORT loc_1_8706
        lea    esi, [ecx + 4]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_8706
        push   ecx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8704:
        mov    dword ptr [esi], eax
loc_1_8706:
        pop    esi
        add    esp, 4
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_8710:
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [esp + 18h]
        test   esi, esi
        je     SHORT loc_1_8740
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_7360
        lea    eax, [esi + 3e7h]
        mov    ecx, 3e8h
        sub    edx, edx
        div    ecx
        push   eax
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_73D0
loc_1_8740:
        pop    esi
        add    esp, 10h
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
