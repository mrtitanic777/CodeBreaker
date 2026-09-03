; nwserver_lcode_01_01.asm -- NWSERVER object 1 segment _LTEXT  (part 1/3)
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        db  0BEh, 030h, 09Ah, 000h, 000h
        mov    word ptr [eax], 65h
        mov    edi, dword ptr [esp + 10h]
        add    edi, 8
        mov    ecx, 19h
        rep movsd
        movsb
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0030:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ecx], edx
        ret
sub_1_0047:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx + 4], ecx
        ret
sub_1_0060:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0080:
        push   edi
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_93BC
        add    esp, 4
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_1_00CA
        db  0BFh, 048h, 09Dh, 000h, 000h
        sub    eax, eax
        mov    ecx, 80h
        rep stosd
        db  0C7h, 005h, 048h, 09Fh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0BFh, 044h, 09Bh, 000h, 000h
        mov    ecx, 80h
        rep stosd
        db  0C7h, 005h, 044h, 09Dh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        mov    eax, 1
loc_1_00CA:
        pop    edi
        ret
        db  000h, 000h, 000h, 000h
loc_1_00D0:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 30h], 0
        jne    SHORT loc_1_00ED
        db  066h, 08Bh, 00Dh, 0E4h, 0AFh, 000h, 000h
        mov    word ptr [eax + 28h], cx
        push   eax
        call   sub_1_2748
        jmp    SHORT loc_1_00F6
loc_1_00ED:
        dec    dword ptr [eax + 30h]
        push   eax
        call   sub_1_318D
loc_1_00F6:
        add    esp, 4
        ret
sub_1_00FA:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 20h]
        or     ebx, ebx
        jne    SHORT loc_1_0116
        call   sub_1_2730
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_0116
        jmp    NEAR PTR loc_1_01A1
loc_1_0116:
        db  0C7h, 043h, 004h, 0D0h, 000h, 000h, 000h
        mov    word ptr [ebx + 0ah], 5505h
        mov    esi, dword ptr [esp + 18h]
        push   6
        lea    eax, [esi + 0ch]
        push   eax
        lea    eax, [ebx + 1ch]
        push   eax
        call   sub_1_4288
        add    esp, 0ch
        mov    dword ptr [ebx + 30h], 1
        mov    ax, word ptr [esp + 1ch]
        mov    ebp, dword ptr [ebx + 24h]
        mov    word ptr [ebx + 28h], ax
        lea    eax, [ebp + 6]
        mov    byte ptr [ebp + 5], 14h
        push   0ch
        push   esi
        push   eax
        call   sub_1_4288
        add    esp, 0ch
        mov    dword ptr [ebp + 3eh], 53575242h
        mov    dword ptr [ebp + 46h], 10006h
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebp + 52h], eax
        db  0BFh, 0BCh, 0B1h, 000h, 000h
        db  0A1h, 0D0h, 09Fh, 000h, 000h
        mov    dword ptr [ebp + 4ah], eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp + 56h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_01A1:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_01A8:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax + 24h]
        lea    edx, [ecx + 1eh]
        lea    esi, [ecx + 12h]
        lea    edi, [ecx + 6]
        movsd
        movsd
        movsd
        db  0C7h, 040h, 004h, 0D0h, 000h, 000h, 000h
        mov    word ptr [eax + 0ah], 5505h
        mov    dword ptr [eax + 30h], 0
        mov    word ptr [eax + 28h], 86h
        mov    byte ptr [ecx + 5], 4
        mov    dword ptr [edx], 53575242h
        mov    dword ptr [edx + 8], 10006h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx + 14h], eax
        db  0A1h, 0D0h, 09Fh, 000h, 000h
        mov    dword ptr [edx + 0ch], eax
        pop    edi
        pop    esi
        ret
sub_1_01FA:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edx + 24h]
        lea    esi, [eax + 12h]
        lea    edi, [eax + 6]
        movsd
        movsd
        movsd
        mov    byte ptr [eax + 5], 4
        db  0C7h, 042h, 004h, 0D0h, 000h, 000h, 000h
        mov    word ptr [edx + 0ah], 5505h
        mov    ecx, dword ptr [esp + 10h]
        add    cx, 28h
        mov    word ptr [edx + 28h], cx
        lea    ecx, [eax + 1eh]
        mov    dword ptr [edx + 30h], 0
        mov    byte ptr [ecx + 1], 0
        or     byte ptr [ecx], 80h
        mov    word ptr [ecx + 4], 100h
        mov    dl, byte ptr [esp + 14h]
        mov    byte ptr [ecx + 6], dl
        mov    word ptr [ecx + 8], 1e00h
        pop    edi
        pop    esi
        ret
loc_1_0250:
        mov    ecx, dword ptr [esp + 4]
        cmp    byte ptr [ecx + 9], 0
        jne    SHORT loc_1_029C
        mov    eax, dword ptr [ecx + 24h]
        cmp    byte ptr [eax + 5], 14h
        jne    SHORT loc_1_0268
        add    eax, 3eh
        jmp    SHORT loc_1_026B
loc_1_0268:
        add    eax, 1eh
loc_1_026B:
        cmp    dword ptr [eax], 53575242h
        jne    SHORT loc_1_028E
        mov    eax, dword ptr [eax + 14h]
        cmp    eax, 0eh
        jbe    SHORT loc_1_0283
        db  0C7h, 001h, 022h, 032h, 000h, 000h
        jmp    SHORT loc_1_0294
loc_1_0283:
        db  08Bh, 004h, 085h, 0F8h, 09Fh, 000h, 000h
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_0294
loc_1_028E:
        db  0C7h, 001h, 03Ah, 008h, 000h, 000h
loc_1_0294:
        push   ecx
        call   sub_1_6D70
        jmp    SHORT loc_1_02A2
loc_1_029C:
        push   ecx
        call   sub_1_2748
loc_1_02A2:
        add    esp, 4
        ret
loc_1_02A6:
        push   ebx
        push   esi
        db  083h, 03Dh, 03Ch, 0B4h, 000h, 000h, 000h
        je     SHORT loc_1_02B8
        sub    eax, eax
        jmp    NEAR PTR loc_1_036D
loc_1_02B8:
        mov    esi, dword ptr [esp + 10h]
        mov    ax, word ptr [esi + 2]
        mov    cx, ax
        shl    cx, 8
        shr    ax, 8
        add    cx, ax
        movzx  eax, cx
        cmp    eax, 22h
        jae    SHORT loc_1_02DD
        sub    eax, eax
        jmp    NEAR PTR loc_1_036D
loc_1_02DD:
        mov    al, byte ptr [esi + 5]
        cmp    al, 14h
        lea    edx, [esi + 1eh]
        jne    SHORT loc_1_02EA
        lea    edx, [esi + 3eh]
loc_1_02EA:
        cmp    dword ptr [edx], 53575242h
        je     SHORT loc_1_0318
        cmp    al, 14h
        je     SHORT loc_1_02FB
        add    esi, 1eh
        jmp    SHORT loc_1_02FE
loc_1_02FB:
        add    esi, 3eh
loc_1_02FE:
        mov    al, byte ptr [esi]
        cmp    al, 10h
        jb     SHORT loc_1_0314
        cmp    al, 16h
        ja     SHORT loc_1_0314
        cmp    word ptr [esi + 4], 100h
        je     SHORT loc_1_035D
        sub    eax, eax
        jmp    SHORT loc_1_036D
loc_1_0314:
        sub    eax, eax
        jmp    SHORT loc_1_036D
loc_1_0318:
        cmp    dword ptr [edx + 14h], 16h
        jbe    SHORT loc_1_0322
        sub    eax, eax
        jmp    SHORT loc_1_036D
loc_1_0322:
        lea    eax, [edx + 18h]
        db  0B9h, 0BCh, 0B1h, 000h, 000h
loc_1_032A:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [ecx]
        jne    SHORT loc_1_034A
        or     bl, bl
        je     SHORT loc_1_0346
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [ecx + 1]
        jne    SHORT loc_1_034A
        add    eax, 2
        add    ecx, 2
        or     bl, bl
        jne    SHORT loc_1_032A
loc_1_0346:
        sub    eax, eax
        jmp    SHORT loc_1_034F
loc_1_034A:
        sbb    eax, eax
        sbb    eax, -1
loc_1_034F:
        or     eax, eax
        je     SHORT loc_1_035D
        cmp    dword ptr [edx + 14h], 5
        je     SHORT loc_1_035D
        sub    eax, eax
        jmp    SHORT loc_1_036D
loc_1_035D:
        call   sub_1_2730
        or     eax, eax
        je     SHORT loc_1_036D
        db  0C7h, 040h, 004h, 050h, 002h, 000h, 000h
loc_1_036D:
        pop    esi
        pop    ebx
        ret
sub_1_0370:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 1ch]
        add    edx, 0ah
        cmp    byte ptr [edx + 1], 1
        je     SHORT loc_1_038B
        mov    bl, 4
        jmp    NEAR PTR loc_1_0456
loc_1_038B:
        lea    ebx, [edx + 2]
        movzx  ebp, byte ptr [edx]
        lea    edx, [edx + ebp + 2]
        cmp    byte ptr [edx + 1], 2
        je     SHORT loc_1_03A2
        mov    bl, 4
        jmp    NEAR PTR loc_1_0456
loc_1_03A2:
        mov    ax, word ptr [edx + 2]
        add    edx, 4
        mov    word ptr [esp + 12h], ax
        cmp    byte ptr [edx + 1], 3
        je     SHORT loc_1_03BB
        mov    bl, 4
        jmp    NEAR PTR loc_1_0456
loc_1_03BB:
        movzx  eax, byte ptr [edx]
        db  03Bh, 005h, 0CCh, 0B1h, 000h, 000h
        jne    NEAR PTR loc_1_0454
        lea    esi, [edx + 2]
        db  0BFh, 0BCh, 0B1h, 000h, 000h
        db  0A1h, 0CCh, 0B1h, 000h, 000h
        mov    ecx, eax
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_1_0454
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_1_0454
        movzx  eax, byte ptr [edx]
        add    eax, 2
        add    edx, eax
        cmp    byte ptr [edx + 1], 4
        je     SHORT loc_1_03FB
        mov    bl, 4
        jmp    SHORT loc_1_0456
loc_1_03FB:
        lea    esi, [edx + 6]
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_94C8
        add    esp, 4
        push   0
        push   0
        push   ebp
        push   ebx
        push   dword ptr [esp + 22h]
        call   sub_1_096E
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_043A
        push   esi
        push   ebp
        push   ebx
        push   dword ptr [esp + 1eh]
        call   sub_1_0A28
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_0436
        jmp    SHORT loc_1_0443
loc_1_0436:
        mov    bl, 1
        jmp    SHORT loc_1_0445
loc_1_043A:
        mov    edi, dword ptr [eax + 0ch]
        add    edi, 10h
        movsd
        movsd
        movsd
loc_1_0443:
        sub    bl, bl
loc_1_0445:
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_950C
        add    esp, 4
        jmp    SHORT loc_1_0456
loc_1_0454:
        mov    bl, 2
loc_1_0456:
        push   ebx
        mov    esi, dword ptr [esp + 1ch]
        push   0
        push   esi
        call   sub_1_01FA
        add    esp, 0ch
        push   esi
        call   sub_1_318D
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_0477:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 24h]
        mov    ebx, dword ptr [esp + 24h]
        mov    ax, word ptr [eax + 2]
        mov    cx, ax
        shl    cx, 8
        shr    ax, 8
        add    cx, ax
        db  068h, 028h, 09Bh, 000h, 000h
        movzx  eax, cx
        add    ebx, 0ah
        lea    eax, [eax + ebx - 28h]
        mov    dword ptr [esp + 1ch], eax
        call   sub_1_94C8
        add    esp, 4
        cmp    dword ptr [esp + 18h], ebx
        jbe    NEAR PTR loc_1_05CF
loc_1_04C0:
        cmp    byte ptr [ebx + 1], 1
        jne    NEAR PTR loc_1_05A7
        lea    eax, [ebx + 2]
        movzx  ebp, byte ptr [ebx]
        lea    ebx, [ebx + ebp + 2]
        mov    dword ptr [esp + 14h], eax
        cmp    byte ptr [ebx + 1], 2
        jne    NEAR PTR loc_1_05AE
        mov    dx, word ptr [ebx + 2]
        add    ebx, 4
        cmp    byte ptr [ebx + 1], 3
        jne    NEAR PTR loc_1_05B5
        movzx  eax, byte ptr [ebx]
        db  03Bh, 005h, 0CCh, 0B1h, 000h, 000h
        jne    NEAR PTR loc_1_05CA
        lea    esi, [ebx + 2]
        db  0BFh, 0BCh, 0B1h, 000h, 000h
        db  0A1h, 0CCh, 0B1h, 000h, 000h
        mov    ecx, eax
        shr    ecx, 2
        repe cmpsd
        jne    NEAR PTR loc_1_05CA
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    NEAR PTR loc_1_05CA
        movzx  eax, byte ptr [ebx]
        add    eax, 2
        add    ebx, eax
        mov    edi, 0
        cmp    byte ptr [ebx + 1], 4
        jne    SHORT loc_1_0547
        lea    edi, [ebx + 6]
        movzx  eax, byte ptr [ebx]
        add    eax, 2
        add    ebx, eax
loc_1_0547:
        push   0
        push   0
        push   ebp
        push   dword ptr [esp + 20h]
        push   edx
        call   sub_1_096E
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_05BC
        or     edi, edi
        je     SHORT loc_1_0570
        mov    esi, dword ptr [eax + 0ch]
        mov    ecx, 3
        lea    esi, [esi + 10h]
        repe cmpsd
        jne    SHORT loc_1_05C3
loc_1_0570:
        mov    ecx, dword ptr [esp + 24h]
        cmp    byte ptr [ecx], 12h
        jne    SHORT loc_1_058D
        cmp    dword ptr [eax + 10h], -1
        je     SHORT loc_1_0596
        db  08Bh, 00Dh, 0D8h, 09Fh, 000h, 000h
        add    ecx, 1eh
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_0596
loc_1_058D:
        push   eax
        db  0E8h, 088h, 02Ch, 0FFh, 0FFh
        add    esp, 4
loc_1_0596:
        mov    byte ptr [esp + 13h], 0
        cmp    dword ptr [esp + 18h], ebx
        ja     NEAR PTR loc_1_04C0
        jmp    SHORT loc_1_05CF
loc_1_05A7:
        mov    byte ptr [esp + 13h], 4
        jmp    SHORT loc_1_05CF
loc_1_05AE:
        mov    byte ptr [esp + 13h], 4
        jmp    SHORT loc_1_05CF
loc_1_05B5:
        mov    byte ptr [esp + 13h], 4
        jmp    SHORT loc_1_05CF
loc_1_05BC:
        mov    byte ptr [esp + 13h], 7
        jmp    SHORT loc_1_05CF
loc_1_05C3:
        mov    byte ptr [esp + 13h], 7
        jmp    SHORT loc_1_05CF
loc_1_05CA:
        mov    byte ptr [esp + 13h], 2
loc_1_05CF:
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_950C
        add    esp, 4
        push   dword ptr [esp + 13h]
        push   0
        push   dword ptr [esp + 28h]
        call   sub_1_01FA
        add    esp, 0ch
        push   dword ptr [esp + 20h]
        call   sub_1_318D
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_0602:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 30h]
        sub    dx, dx
        lea    ecx, [edi + 10h]
        lea    esi, [edi + 0ah]
        mov    ebp, dword ptr [esp + 2ch]
        mov    al, byte ptr [ecx + 1]
        mov    ebp, dword ptr [ebp + 24h]
        cmp    al, 1
        lea    ebx, [ebp + 1eh]
        jne    NEAR PTR loc_1_071C
        cmp    byte ptr [ebp + 5], 14h
        jne    SHORT loc_1_063D
        mov    al, byte ptr [edi]
        mov    byte ptr [ebx], al
        mov    ax, word ptr [edi + 2]
        mov    word ptr [ebx + 2], ax
loc_1_063D:
        mov    ebp, dword ptr [esp + 2ch]
        mov    edx, dword ptr [ebp + 24h]
        add    edx, 0ah
        db  0BFh, 0C8h, 09Fh, 000h, 000h
        mov    eax, dword ptr [edx]
        cmp    eax, dword ptr [edi]
        jne    SHORT loc_1_0664
        mov    ax, word ptr [edx + 4]
        cmp    ax, word ptr [edi + 4]
        mov    dword ptr [esp + 14h], 0
        je     SHORT loc_1_066C
loc_1_0664:
        mov    dword ptr [esp + 14h], 1
loc_1_066C:
        mov    si, word ptr [esi]
        and    esi, 2000h
        lea    eax, [ecx + 2]
        movzx  edi, byte ptr [ecx]
        lea    ecx, [ecx + edi + 2]
        mov    dword ptr [esp + 20h], eax
        cmp    byte ptr [ecx + 1], 2
        je     SHORT loc_1_0694
        mov    bl, 4
        mov    ebp, dword ptr [esp + 1ch]
        jmp    NEAR PTR loc_1_07AC
loc_1_0694:
        mov    bp, word ptr [ecx + 2]
        lea    eax, [ebx + 10h]
        mov    dword ptr [esp + 24h], eax
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_94C8
        add    esp, 4
        push   esi
        push   dword ptr [esp + 28h]
        push   edi
        push   dword ptr [esp + 2ch]
        push   ebp
        call   sub_1_096E
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_06EE
        mov    dword ptr [ebx + 0ah], 0
        mov    word ptr [ebx + 0eh], 100h
        sub    bl, bl
        or     esi, esi
        mov    ebp, 18h
        je     SHORT loc_1_06E4
        db  08Bh, 02Dh, 0CCh, 0B1h, 000h, 000h
        add    ebp, 1ah
loc_1_06E4:
        mov    dword ptr [esp + 14h], 1
        jmp    SHORT loc_1_070A
loc_1_06EE:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_0702
        sub    ebp, ebp
        mov    dword ptr [ebx + 0ah], ebp
        mov    word ptr [ebx + 0eh], bp
        mov    bl, 7
        jmp    SHORT loc_1_070A
loc_1_0702:
        mov    ebp, dword ptr [esp + 1ch]
        mov    bl, byte ptr [esp + 13h]
loc_1_070A:
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_950C
        add    esp, 4
        jmp    NEAR PTR loc_1_07AC
loc_1_071C:
        mov    dword ptr [esp + 14h], 1
        mov    esi, dword ptr [edi + 0ch]
        mov    dword ptr [esp + 18h], esi
        mov    di, word ptr [edi + 8]
        mov    si, di
        shl    si, 8
        shr    di, 8
        mov    ebp, dword ptr [esp + 2ch]
        add    si, di
        movsx  ebp, word ptr [ebp + 28h]
        movzx  esi, si
        cmp    ebp, esi
        jb     SHORT loc_1_074E
        mov    ebp, esi
loc_1_074E:
        sub    ebp, 28h
        cmp    al, 2
        jne    SHORT loc_1_0761
        mov    dx, word ptr [ecx + 2]
        movzx  eax, byte ptr [ecx]
        add    eax, 2
        add    ecx, eax
loc_1_0761:
        cmp    byte ptr [ecx + 1], 3
        je     SHORT loc_1_076B
        mov    bl, 4
        jmp    SHORT loc_1_07AC
loc_1_076B:
        lea    esi, [ecx + 2]
        db  0BFh, 0BCh, 0B1h, 000h, 000h
        movzx  eax, byte ptr [ecx]
        mov    ecx, eax
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_1_0786
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
loc_1_0786:
        je     SHORT loc_1_0796
        mov    dword ptr [esp + 14h], 0
        mov    bl, byte ptr [esp + 13h]
        jmp    SHORT loc_1_07AC
loc_1_0796:
        lea    eax, [ebx + 0ah]
        push   eax
        push   edx
        push   ebp
        sub    bl, bl
        push   dword ptr [esp + 24h]
        call   sub_1_1207
        add    esp, 10h
        mov    ebp, eax
loc_1_07AC:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_07CC
        push   ebx
        push   ebp
        push   dword ptr [esp + 34h]
        call   sub_1_01FA
        add    esp, 0ch
        push   dword ptr [esp + 2ch]
        call   sub_1_318D
        jmp    SHORT loc_1_07D5
loc_1_07CC:
        push   dword ptr [esp + 2ch]
        call   sub_1_2748
loc_1_07D5:
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_1_07E0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        or     byte ptr [edi], 80h
        mov    ebx, dword ptr [edi + 0ch]
        mov    ax, word ptr [edi + 8]
        mov    cx, ax
        shl    cx, 8
        shr    ax, 8
        mov    esi, dword ptr [esp + 10h]
        add    cx, ax
        movsx  edx, word ptr [esi + 28h]
        movzx  eax, cx
        cmp    edx, eax
        jb     SHORT loc_1_0810
        mov    edx, eax
loc_1_0810:
        add    edi, 0ah
        push   edi
        sub    edx, 28h
        push   edx
        push   ebx
        call   sub_1_112E
        add    esp, 0ch
        push   0
        push   eax
        push   esi
        call   sub_1_01FA
        add    esp, 0ch
        push   esi
        call   sub_1_318D
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_083A:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 24h]
        cmp    byte ptr [edx + 5], 14h
        je     SHORT loc_1_084C
        add    edx, 1eh
        jmp    SHORT loc_1_084F
loc_1_084C:
        add    edx, 3eh
loc_1_084F:
        movzx  ecx, byte ptr [edx]
        cmp    ecx, 10h
        je     SHORT loc_1_086D
        cmp    ecx, 12h
        jl     SHORT loc_1_0892
        cmp    ecx, 13h
        jle    SHORT loc_1_0876
        cmp    ecx, 14h
        je     SHORT loc_1_087F
        cmp    ecx, 15h
        je     SHORT loc_1_0888
        jmp    SHORT loc_1_0892
loc_1_086D:
        push   edx
        push   eax
        call   sub_1_0370
        jmp    SHORT loc_1_088F
loc_1_0876:
        push   edx
        push   eax
        call   sub_1_0477
        jmp    SHORT loc_1_088F
loc_1_087F:
        push   edx
        push   eax
        call   sub_1_0602
        jmp    SHORT loc_1_088F
loc_1_0888:
        push   edx
        push   eax
        call   sub_1_07E0
loc_1_088F:
        add    esp, 8
loc_1_0892:
        ret
loc_1_0893:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        push   66h
        db  068h, 0BCh, 09Fh, 000h, 000h
        push   2
        call   sub_1_00FA
        add    esp, 10h
        push   esi
        call   sub_1_318D
        add    esp, 4
        pop    esi
        ret
loc_1_08B5:
        push   esi
        push   edi
        db  0F6h, 005h, 0D4h, 09Fh, 000h, 000h, 001h
        mov    esi, dword ptr [esp + 0ch]
        je     NEAR PTR loc_1_0959
        mov    edi, dword ptr [esi + 24h]
        push   0ah
        db  068h, 004h, 0B4h, 000h, 000h
        lea    eax, [edi + 12h]
        push   eax
        call   sub_1_426C
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0959
        cmp    byte ptr [edi + 5], 14h
        je     SHORT loc_1_08ED
        add    edi, 1eh
        jmp    SHORT loc_1_08F0
loc_1_08ED:
        add    edi, 3eh
loc_1_08F0:
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 10006h
        jbe    SHORT loc_1_0906
        push   1
        call   sub_1_0DBE
        add    esp, 4
        jmp    SHORT loc_1_0959
loc_1_0906:
        jne    SHORT loc_1_0937
        mov    eax, dword ptr [edi + 0ch]
        db  03Bh, 005h, 0D0h, 09Fh, 000h, 000h
        jbe    SHORT loc_1_091F
        push   1
        call   sub_1_0DBE
        add    esp, 4
        jmp    SHORT loc_1_0959
loc_1_091F:
        jne    SHORT loc_1_0937
        db  0A1h, 0E0h, 09Fh, 000h, 000h
        cmp    dword ptr [edi + 10h], eax
        jbe    SHORT loc_1_0937
        push   1
        call   sub_1_0DBE
        add    esp, 4
        jmp    SHORT loc_1_0959
loc_1_0937:
        push   esi
        push   66h
        db  068h, 0BCh, 09Fh, 000h, 000h
        push   2
        call   sub_1_00FA
        add    esp, 10h
        db  0A1h, 0E0h, 09Fh, 000h, 000h
        mov    dword ptr [edi + 10h], eax
        push   esi
        call   sub_1_318D
        jmp    SHORT loc_1_0968
loc_1_0959:
        push   esi
        call   sub_1_3222
        add    esp, 4
        push   esi
        call   sub_1_2748
loc_1_0968:
        add    esp, 4
        pop    edi
        pop    esi
        ret
sub_1_096E:
        sub    esp, 38h
        push   esi
        push   edi
        mov    ax, word ptr [esp + 44h]
        mov    word ptr [esp + 0ch], ax
        lea    edi, [esp + 0fh]
        mov    eax, dword ptr [esp + 4ch]
        mov    byte ptr [esp + 0eh], al
        mov    esi, dword ptr [esp + 48h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        db  0E8h, 095h, 014h, 0FFh, 0FFh
        add    esp, 8
        or     ax, ax
        jne    SHORT loc_1_0A16
        mov    edx, dword ptr [esp + 50h]
        or     edx, edx
        je     SHORT loc_1_0A1E
        cmp    dword ptr [esp + 54h], 0
        je     SHORT loc_1_09ED
        db  0A0h, 0CCh, 0B1h, 000h, 000h
        mov    byte ptr [edx], al
        inc    edx
        db  0BEh, 0BCh, 0B1h, 000h, 000h
        mov    byte ptr [edx], 3
        inc    edx
        db  0A1h, 0CCh, 0B1h, 000h, 000h
        mov    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  003h, 015h, 0CCh, 0B1h, 000h, 000h
loc_1_09ED:
        mov    byte ptr [edx], 10h
        inc    edx
        mov    byte ptr [edx], 4
        inc    edx
        mov    byte ptr [edx], 1
        inc    edx
        mov    byte ptr [edx], 10h
        inc    edx
        mov    byte ptr [edx], 6
        inc    edx
        mov    byte ptr [edx], 0ch
        mov    eax, dword ptr [esp + 8]
        lea    edi, [edx + 1]
        mov    esi, dword ptr [eax + 0ch]
        add    esi, 10h
        movsd
        movsd
        movsd
        jmp    SHORT loc_1_0A1E
loc_1_0A16:
        mov    dword ptr [esp + 8], 0
loc_1_0A1E:
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 38h
        ret
sub_1_0A28:
        sub    esp, 38h
        push   esi
        push   edi
        mov    byte ptr [esp + 0ch], 1
        mov    byte ptr [esp + 0dh], 40h
        lea    edi, [esp + 11h]
        mov    ax, word ptr [esp + 44h]
        mov    word ptr [esp + 0eh], ax
        mov    eax, dword ptr [esp + 4ch]
        mov    byte ptr [esp + 10h], al
        mov    ecx, eax
        mov    esi, dword ptr [esp + 48h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        db  0E8h, 04Eh, 00Eh, 0FFh, 0FFh
        add    esp, 8
        or     ax, ax
        je     SHORT loc_1_0A80
        mov    eax, 0ffh
        jmp    NEAR PTR loc_1_0B10
loc_1_0A80:
        push   200h
        push   20h
        call   sub_1_4364
        mov    ecx, dword ptr [esp + 10h]
        add    esp, 8
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0ch], 0
        jne    SHORT loc_1_0AB0
        push   eax
        db  0E8h, 002h, 027h, 0FFh, 0FFh
        add    esp, 4
        mov    eax, 0ffh
        jmp    SHORT loc_1_0B10
loc_1_0AB0:
        db  0A1h, 0D8h, 09Fh, 000h, 000h
        mov    ecx, dword ptr [esp + 8]
        add    eax, 1eh
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 8]
        push   0ch
        mov    eax, dword ptr [eax + 0ch]
        mov    byte ptr [eax + 4], 2
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [eax + 0ch]
        add    esi, 8
        push   dword ptr [esp + 54h]
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_4288
        add    esp, 0ch
        db  0FFh, 005h, 0E8h, 09Fh, 000h, 000h
        db  068h, 0ECh, 09Fh, 000h, 000h
        push   esi
        call   sub_1_0047
        mov    eax, dword ptr [esp + 10h]
        add    esp, 8
        mov    dword ptr [esi + 14h], eax
        sub    eax, eax
loc_1_0B10:
        pop    edi
        pop    esi
        add    esp, 38h
        ret
loc_1_0B16:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 28h]
        mov    ecx, 0ffffffffh
        mov    ebp, dword ptr [ebx + 24h]
        add    ebp, 1eh
        mov    word ptr [esp + 10h], 7b06h
        lea    edi, [ebp + 18h]
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 12h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_3A7C
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0B7A
        push   11h
        push   ebx
        call   sub_1_01A8
        add    esp, 8
        lea    edi, [ebp + 2ah]
        lea    esi, [ebp + 18h]
        movsd
        movsd
        movsd
        movsd
        jmp    SHORT loc_1_0B92
loc_1_0B7A:
        lea    esi, [eax + 14h]
        lea    edi, [ebp + 28h]
        mov    ecx, 10h
        rep movsd
        push   10h
        push   ebx
        call   sub_1_01A8
        add    esp, 8
loc_1_0B92:
        db  0BEh, 0BCh, 0B1h, 000h, 000h
        lea    edi, [ebp + 18h]
        movsd
        movsd
        movsd
        movsd
        mov    word ptr [ebx + 28h], 86h
        push   ebx
        call   sub_1_318D
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_0BB5:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        db  0BFh, 0BCh, 0B1h, 000h, 000h
        mov    edx, dword ptr [ebx + 24h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 36h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   15h
        push   ebx
        call   sub_1_01A8
        add    esp, 8
        mov    word ptr [ebx + 28h], 46h
        push   ebx
        call   sub_1_318D
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0C02:
        push   dword ptr [esp + 4]
        call   sub_1_2748
        add    esp, 4
        ret
loc_1_0C0F:
        sub    esp, 4
        push   esi
        db  083h, 03Dh, 0DCh, 09Fh, 000h, 000h, 000h
        je     SHORT loc_1_0C8E
        db  0C7h, 005h, 0DCh, 09Fh, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 03Ch, 0B4h, 000h, 000h, 000h
        jne    SHORT loc_1_0C77
        db  0F6h, 005h, 0D8h, 09Fh, 000h, 000h, 001h
        je     SHORT loc_1_0C93
        push   0
        push   66h
        db  068h, 0BCh, 09Fh, 000h, 000h
        push   2
        call   sub_1_00FA
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_0C71
        mov    dword ptr [esi + 30h], 0
        call   sub_1_320B
        db  0A3h, 0E0h, 09Fh, 000h, 000h
        mov    ecx, dword ptr [esi + 24h]
        push   esi
        mov    dword ptr [ecx + 4eh], eax
        call   sub_1_318D
        add    esp, 4
loc_1_0C71:
        db  0FFh, 005h, 0D8h, 09Fh, 000h, 000h
loc_1_0C77:
        push   0ea60h
        db  068h, 00Fh, 00Ch, 000h, 000h
        call   sub_1_4244
        add    esp, 8
        db  0A3h, 0DCh, 09Fh, 000h, 000h
loc_1_0C8E:
        pop    esi
        add    esp, 4
        ret
loc_1_0C93:
        mov    dword ptr [esp + 4], 0
        lea    eax, [esp + 4]
        push   5006h
        push   eax
        db  0E8h, 0EDh, 012h, 0FFh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0C71
        push   0
        push   66h
        db  068h, 0BCh, 09Fh, 000h, 000h
        push   12h
        call   sub_1_00FA
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0CD1
        push   eax
        call   sub_1_318D
        add    esp, 4
loc_1_0CD1:
        push   1
        call   sub_1_0DBE
        add    esp, 4
        jmp    SHORT loc_1_0C8E
sub_1_0CDD:
        sub    esp, 40h
        push   esi
        push   edi
        db  083h, 03Dh, 03Ch, 0B4h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_0DB8
        push   0
        push   66h
        db  068h, 0BCh, 09Fh, 000h, 000h
        push   2
        call   sub_1_00FA
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_0DB8
        db  0C7h, 005h, 0D4h, 09Fh, 000h, 000h, 001h, 000h, 000h, 000h
        push   5505h
        call   sub_1_315A
        add    esp, 4
        db  068h, 0A6h, 002h, 000h, 000h
        push   0
        push   5505h
        call   sub_1_3112
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0D47
        db  0C7h, 005h, 0D0h, 09Fh, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0DB8
loc_1_0D47:
        call   sub_1_320B
        db  0A3h, 0E0h, 09Fh, 000h, 000h
        mov    ecx, dword ptr [esi + 24h]
        push   esi
        mov    dword ptr [ecx + 4eh], eax
        call   sub_1_318D
        add    esp, 4
        push   0ea60h
        db  068h, 00Fh, 00Ch, 000h, 000h
        call   sub_1_4244
        add    esp, 8
        db  0A3h, 0DCh, 09Fh, 000h, 000h
        mov    word ptr [esp + 8], 7b06h
        db  0BFh, 0BCh, 0B1h, 000h, 000h
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
        mov    word ptr [esp + 44h], 5505h
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_328E
        add    esp, 4
loc_1_0DB8:
        pop    edi
        pop    esi
        add    esp, 40h
        ret
sub_1_0DBE:
        sub    esp, 0dch
        push   ebx
        push   esi
        push   edi
        db  0C7h, 005h, 0D4h, 09Fh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 0DCh, 09Fh, 000h, 000h
        call   sub_1_425C
        add    esp, 4
        db  0C7h, 005h, 0DCh, 09Fh, 000h, 000h, 000h, 000h, 000h, 000h
        push   5505h
        call   sub_1_315A
        mov    word ptr [esp + 44h], 7b06h
        add    esp, 4
        db  0BFh, 0BCh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 42h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    word ptr [esp + 7ch], 5505h
        lea    eax, [esp + 40h]
        push   eax
        call   sub_1_32A3
        add    esp, 4
        cmp    dword ptr [esp + 0ech], 1
        jne    SHORT loc_1_0E48
        call   sub_1_326F
        jmp    SHORT loc_1_0EB5
loc_1_0E48:
        db  0C7h, 005h, 0D0h, 09Fh, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        call   sub_1_327B
        mov    word ptr [esp + 32h], 1
        lea    eax, [esp + 84h]
        mov    dword ptr [esp + 34h], eax
        lea    eax, [esp + 10h]
        add    esp, 4
        push   eax
        push   66h
        db  068h, 0BCh, 09Fh, 000h, 000h
        push   1
        call   sub_1_00FA
        mov    dword ptr [esp + 4ch], 0
        mov    dword ptr [esp + 20h], 0
        lea    eax, [esp + 1ch]
        add    esp, 10h
        push   eax
        call   sub_1_318D
        add    esp, 4
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_1_0EB5
        sub    ebx, ebx
loc_1_0EAA:
        call   sub_1_31E6
        cmp    byte ptr [esp + 14h], bl
        jne    SHORT loc_1_0EAA
loc_1_0EB5:
        db  0A1h, 0D8h, 09Fh, 000h, 000h
        add    eax, 1eh
        db  0A3h, 0D8h, 09Fh, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0dch
        ret
loc_1_0ECC:
        push   dword ptr [esp + 4]
        call   sub_1_4390
        add    esp, 4
        db  0C7h, 005h, 0F4h, 09Fh, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_1_0EE3:
        push   200h
        push   10h
        call   sub_1_4364
        add    esp, 8
        db  0A3h, 0F4h, 09Fh, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_0EFD
        jmp    SHORT loc_1_0F77
loc_1_0EFD:
        db  0A1h, 0F4h, 09Fh, 000h, 000h
        db  0C7h, 040h, 004h, 0CCh, 00Eh, 000h, 000h
        db  0FFh, 035h, 0D0h, 09Fh, 000h, 000h
        call   sub_1_327B
        add    esp, 4
        db  0F6h, 005h, 0D5h, 09Fh, 000h, 000h, 010h
        jne    SHORT loc_1_0F2E
        call   sub_1_0F7D
        cmp    eax, 0ffh
        jne    SHORT loc_1_0F2E
        jmp    SHORT loc_1_0F6D
loc_1_0F2E:
        movzx  eax, word ptr [esp + 0ch]
        db  0FFh, 035h, 0F4h, 09Fh, 000h, 000h
        push   eax
        movzx  eax, word ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 10h]
        call   sub_1_3237
        add    esp, 10h
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_1_0F7C
        db  0FFh, 035h, 0F4h, 09Fh, 000h, 000h
        call   sub_1_4390
        add    esp, 4
        db  0C7h, 005h, 0F4h, 09Fh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0F6D:
        push   0
        call   sub_1_327B
        add    esp, 4
loc_1_0F77:
        mov    eax, 0ffh
loc_1_0F7C:
        ret
sub_1_0F7D:
        sub    esp, 38h
        push   esi
        push   edi
        mov    word ptr [esp + 0eh], 400h
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [esp + 10h], cl
        movzx  eax, cl
        push   eax
        lea    eax, [esp + 15h]
        db  068h, 0ACh, 0B1h, 000h, 000h
        push   eax
        call   sub_1_4288
        add    esp, 0ch
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_94C8
        lea    eax, [esp + 0ch]
        add    esp, 4
        push   eax
        lea    eax, [esp + 12h]
        push   eax
        db  0E8h, 06Fh, 00Eh, 0FFh, 0FFh
        add    esp, 8
        or     ax, ax
        jne    SHORT loc_1_0FDD
        jmp    NEAR PTR loc_1_109B
loc_1_0FDD:
        mov    byte ptr [esp + 0ch], 1
        mov    byte ptr [esp + 0dh], 0
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        db  0E8h, 0C6h, 008h, 0FFh, 0FFh
        add    esp, 8
        or     ax, ax
        jne    NEAR PTR loc_1_109B
        push   200h
        push   20h
        call   sub_1_4364
        mov    ecx, dword ptr [esp + 10h]
        add    esp, 8
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0ch], 0
        jne    SHORT loc_1_1049
        push   eax
        db  0E8h, 080h, 021h, 0FFh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 020h, 09Bh, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_950C
        add    esp, 4
        mov    eax, 0ffh
        jmp    SHORT loc_1_10AA
loc_1_1049:
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        db  0A3h, 020h, 09Bh, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 10h], 0ffffffffh
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 8]
        push   0ch
        mov    eax, dword ptr [eax + 0ch]
        mov    byte ptr [eax + 4], 2
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [eax + 0ch]
        add    esi, 8
        db  068h, 004h, 0B4h, 000h, 000h
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_4288
        mov    eax, dword ptr [esp + 14h]
        add    esp, 0ch
        mov    dword ptr [esi + 14h], eax
loc_1_109B:
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_950C
        add    esp, 4
        sub    eax, eax
loc_1_10AA:
        pop    edi
        pop    esi
        add    esp, 38h
        ret
sub_1_10B0:
        sub    esp, 38h
        push   esi
        push   edi
        mov    word ptr [esp + 0ch], 400h
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [esp + 0eh], cl
        movzx  eax, cl
        db  0BEh, 0ACh, 0B1h, 000h, 000h
        lea    edi, [esp + 0fh]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_94C8
        lea    eax, [esp + 0ch]
        add    esp, 4
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        db  0E8h, 038h, 00Dh, 0FFh, 0FFh
        add    esp, 8
        or     ax, ax
        jne    SHORT loc_1_111B
        push   dword ptr [esp + 8]
        db  0E8h, 090h, 020h, 0FFh, 0FFh
        add    esp, 4
loc_1_111B:
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_950C
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 38h
        ret
sub_1_112E:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        db  0A1h, 024h, 09Bh, 000h, 000h
        mov    dword ptr [esp + 10h], esi
        sub    edi, edi
        inc    dword ptr [eax + 0ch]
        mov    ebp, dword ptr [esp + 20h]
        add    ebp, 6
        sub    esi, 6
        lea    eax, [esp + 18h]
        push   7b06h
        push   eax
        db  0E8h, 037h, 00Eh, 0FFh, 0FFh
        add    esp, 8
        cmp    eax, edi
        je     SHORT loc_1_11CB
loc_1_1167:
        db  0A1h, 024h, 09Bh, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + ecx*4]
        mov    ebx, dword ptr [eax + 54h]
        lea    eax, [ebx - 1]
        sub    ebx, 3
        cmp    eax, esi
        ja     SHORT loc_1_11D3
        mov    byte ptr [ebp], bl
        inc    ebp
        inc    edi
        mov    byte ptr [ebp], 3
        db  0A1h, 024h, 09Bh, 000h, 000h
        push   ebx
        mov    eax, dword ptr [eax + 4]
        inc    ebp
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax + ecx*4]
        add    eax, 16h
        push   eax
        push   ebp
        call   sub_1_1874
        add    esp, 0ch
        add    ebp, ebx
        add    ebx, 2
        sub    esi, ebx
        inc    dword ptr [esp + 18h]
        lea    eax, [esp + 18h]
        push   7b06h
        push   eax
        db  0E8h, 0D3h, 00Dh, 0FFh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1167
loc_1_11CB:
        mov    dword ptr [esp + 18h], 0
loc_1_11D3:
        db  0A1h, 024h, 09Bh, 000h, 000h
        mov    ecx, dword ptr [esp + 20h]
        dec    dword ptr [eax + 0ch]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx], eax
        mov    eax, edi
        shr    eax, 8
        and    ax, 0ffh
        shl    di, 8
        add    ax, di
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [esp + 10h]
        sub    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_1207:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 14h], 0ffffffffh
        sub    ebx, ebx
        mov    dword ptr [esp + 18h], ebx
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [esp + 1ch], eax
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_94C8
        mov    eax, dword ptr [esp + 34h]
        add    esp, 4
        add    eax, 6
        mov    dword ptr [esp + 0ch], eax
        sub    dword ptr [esp + 28h], 6
        mov    esi, dword ptr [esp + 24h]
        cmp    esi, ebx
        je     SHORT loc_1_1267
        push   esi
        db  0E8h, 0ECh, 0FFh, 0FEh, 0FFh
        add    esp, 4
        mov    ebx, eax
        mov    eax, esi
        and    eax, 7f0000h
        shr    eax, 10h
        mov    dword ptr [esp + 14h], eax
        or     ebx, ebx
        je     SHORT loc_1_1267
        mov    ebx, dword ptr [ebx]
loc_1_1267:
        or     ebx, ebx
        jne    SHORT loc_1_129A
        mov    eax, dword ptr [esp + 14h]
        db  08Dh, 004h, 085h, 048h, 09Dh, 000h, 000h
loc_1_1276:
        add    eax, 4
        inc    dword ptr [esp + 14h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_1276
        mov    eax, dword ptr [esp + 14h]
        db  08Bh, 01Ch, 085h, 048h, 09Dh, 000h, 000h
        cmp    ebx, -1
        je     NEAR PTR loc_1_1380
        or     ebx, ebx
        je     SHORT loc_1_1267
loc_1_129A:
        mov    edx, dword ptr [ebx + 4]
        mov    ax, word ptr [esp + 2ch]
        cmp    word ptr [edx + 16h], ax
        je     SHORT loc_1_12B1
        or     ax, ax
        jne    NEAR PTR loc_1_1371
loc_1_12B1:
        db  0A1h, 0D8h, 09Fh, 000h, 000h
        cmp    dword ptr [edx + 10h], eax
        jae    SHORT loc_1_12CB
        mov    ebx, dword ptr [ebx]
        push   edx
        db  0E8h, 058h, 01Fh, 0FFh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_1_1373
loc_1_12CB:
        mov    eax, dword ptr [edx + 0ch]
        or     eax, eax
        je     NEAR PTR loc_1_1371
        cmp    byte ptr [eax + 4], 2
        jne    NEAR PTR loc_1_1371
        movzx  eax, byte ptr [edx + 18h]
        add    eax, 2
        cmp    word ptr [esp + 2ch], 0
        jne    SHORT loc_1_12F2
        add    eax, 4
loc_1_12F2:
        cmp    eax, dword ptr [esp + 28h]
        ja     NEAR PTR loc_1_1382
        mov    esi, dword ptr [esp + 0ch]
        mov    cl, byte ptr [edx + 18h]
        mov    byte ptr [esi], cl
        inc    dword ptr [esp + 0ch]
        lea    esi, [edx + 19h]
        mov    ecx, dword ptr [esp + 0ch]
        mov    byte ptr [ecx], 1
        inc    dword ptr [esp + 0ch]
        movzx  ecx, byte ptr [edx + 18h]
        mov    dword ptr [esp + 10h], ecx
        shr    ecx, 2
        mov    edi, dword ptr [esp + 0ch]
        rep movsd
        mov    ecx, dword ptr [esp + 10h]
        and    ecx, 3
        rep movsb
        movzx  ecx, byte ptr [edx + 18h]
        add    dword ptr [esp + 0ch], ecx
        cmp    word ptr [esp + 2ch], 0
        jne    SHORT loc_1_1367
        mov    ecx, dword ptr [esp + 0ch]
        mov    byte ptr [ecx], 2
        inc    dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 0ch]
        mov    byte ptr [ecx], 2
        inc    dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 0ch]
        mov    cx, word ptr [edx + 16h]
        mov    word ptr [esi], cx
        add    dword ptr [esp + 0ch], 2
loc_1_1367:
        sub    dword ptr [esp + 28h], eax
        mov    esi, dword ptr [edx]
        inc    dword ptr [esp + 18h]
loc_1_1371:
        mov    ebx, dword ptr [ebx]
loc_1_1373:
        or     ebx, ebx
        jne    NEAR PTR loc_1_129A
        jmp    NEAR PTR loc_1_1267
loc_1_1380:
        sub    esi, esi
loc_1_1382:
        db  068h, 028h, 09Bh, 000h, 000h
        call   sub_1_950C
        mov    eax, dword ptr [esp + 34h]
        add    esp, 4
        mov    dword ptr [eax], esi
        mov    ecx, dword ptr [esp + 18h]
        shr    ecx, 8
        and    cx, 0ffh
        mov    eax, dword ptr [esp + 18h]
        shl    ax, 8
        add    cx, ax
        mov    eax, dword ptr [esp + 30h]
        mov    word ptr [eax + 4], cx
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [esp + 28h]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_13CC:
        push   esi
        db  08Bh, 035h, 0C4h, 013h, 000h, 000h
        or     esi, esi
        jne    SHORT loc_1_1408
        db  0CDh, 020h, 06Ah, 000h, 001h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        add    esp, 8
        db  089h, 015h, 0C8h, 013h, 000h, 000h
        db  0BEh, 00Fh, 014h, 000h, 000h
        xor    ecx, ecx
        db  0CDh, 020h, 001h, 000h, 08Bh, 004h
        movzx  ax, al
        movzx  eax, ax
        db  0A3h, 0C4h, 013h, 000h, 000h
loc_1_1408:
        pop    esi
        mov    eax, 1
        ret
loc_1_140F:
        ret
sub_1_1410:
        push   ebp
        push   esi
        push   edi
        push   ebx
        db  08Ah, 025h, 0C4h, 013h, 000h, 000h
        mov    ebx, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 1ch]
        mov    al, 13h
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        jae    SHORT loc_1_1446
        or     esi, esi
        je     SHORT loc_1_143A
        db  081h, 005h, 06Ch, 0B1h, 000h, 000h, 000h, 010h, 000h, 000h
loc_1_143A:
        xchg   esi, eax
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], esi
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_1446:
        int3
        jmp    SHORT loc_1_1446
        db  02Eh, 08Bh, 0C0h
sub_1_144C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        db  081h, 02Dh, 06Ch, 0B1h, 000h, 000h, 000h, 010h, 000h, 000h
        pop    esi
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_146C:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        xor    ecx, ecx
        mov    dword ptr [ebp], edx
        mov    dword ptr [ebp + 4], ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        mov    dword ptr [ebp + 10h], eax
        inc    ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        mov    dword ptr [ebp + 0ch], eax
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        mov    dword ptr [ebp + 8], eax
        pop    ebp
        ret
        db  090h
sub_1_149C:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        mov    eax, dword ptr [ebp + 10h]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        pop    ebp
        ret
        db  08Bh, 0C0h
sub_1_14C0:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, 31h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        cmp    dword ptr [ebp], edx
        jb     SHORT loc_1_14FF
loc_1_14E5:
        sub    dword ptr [ebp], edx
        db  001h, 015h, 06Ch, 0B1h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        pop    ebp
        ret
loc_1_14FF:
        mov    dword ptr [ebp + 4], edx
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        jmp    SHORT loc_1_14E5
sub_1_151C:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, 31h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        add    dword ptr [ebp], edx
        db  029h, 015h, 06Ch, 0B1h, 000h, 000h
        mov    edx, dword ptr [ebp + 4]
        or     edx, edx
        je     SHORT loc_1_1563
        cmp    dword ptr [ebp], edx
        jb     SHORT loc_1_1563
        mov    dword ptr [ebp + 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        pop    ebp
        ret
loc_1_1563:
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        pop    ebp
        ret
        db  090h
sub_1_156C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_1_158A
        db  0BEh, 098h, 015h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        leave
        ret
loc_1_158A:
        mov    eax, dword ptr [edx]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        xor    eax, eax
        pop    esi
        leave
        ret
        db  090h
loc_1_1598:
        mov    eax, dword ptr [edx]
        mov    dword ptr [edx + 4], 0
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        ret
sub_1_15A8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, 31h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        leave
        ret
        db  090h
sub_1_15BC:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_1_15D2
        shr    eax, 2
        jne    SHORT loc_1_15CC
        inc    eax
loc_1_15CC:
        db  0CDh, 020h, 003h, 000h, 005h, 000h
loc_1_15D2:
        leave
        ret
sub_1_15D4:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_1_15EA
        shr    eax, 2
        jne    SHORT loc_1_15E4
        inc    eax
loc_1_15E4:
        db  0CDh, 020h, 004h, 000h, 005h, 000h
loc_1_15EA:
        leave
        ret
sub_1_15EC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        db  0BEh, 008h, 016h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        leave
        ret
        db  08Bh, 0C0h
loc_1_1608:
        push   edx
        call   sub_1_5218
        pop    eax
        ret
sub_1_1610:
        push   ebp
        mov    ebp, esp
        push   80h
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        leave
        ret
sub_1_1632:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        leave
        ret
        db  02Eh, 08Bh, 0C0h, 000h, 000h, 000h, 000h, 000h, 02Eh, 08Bh, 0C0h
loc_1_1650:
        push   ecx
        jmp    SHORT loc_1_1667
loc_1_1653:
        mov    ebx, dword ptr [ecx]
        db  089h, 01Dh, 048h, 016h, 000h, 000h
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_1_1667:
        db  08Bh, 00Dh, 048h, 016h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_1_1653
        db  0C6h, 005h, 04Ch, 016h, 000h, 000h, 000h
        pop    ecx
        ret
        db  08Bh, 0C0h
sub_1_167C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 048h, 016h, 000h, 000h
        mov    dword ptr [ebx], ecx
        db  089h, 01Dh, 048h, 016h, 000h, 000h
        xor    ebx, ebx
        db  03Ah, 01Dh, 04Ch, 016h, 000h, 000h
        jne    SHORT loc_1_16BB
        db  0C6h, 005h, 04Ch, 016h, 000h, 000h, 001h
        mov    eax, 4
        db  0BEh, 050h, 016h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, 0ah
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_16BB:
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
sub_1_16C0:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        ret
        db  000h
sub_1_16C8:
        db  083h, 03Dh, 034h, 0A0h, 000h, 000h, 000h
        jne    SHORT loc_1_16D2
loc_1_16D1:
        ret
loc_1_16D2:
        jecxz  loc_1_16D1
        cmp    byte ptr [esi], 0
        je     SHORT loc_1_16D1
        pushfd
        push   edi
        push   eax
        mov    edi, esi
        dec    esi
        std
        xor    eax, eax
loc_1_16E2:
        dec    ecx
        je     SHORT loc_1_16F7
        lodsb
        test   eax, eax
        je     SHORT loc_1_1704
        db  080h, 0B8h, 038h, 0A0h, 000h, 000h, 000h
        je     SHORT loc_1_16E2
        lea    esi, [esi + 2]
        inc    ecx
loc_1_16F7:
        sub    edi, esi
        and    edi, 0fffffffeh
        add    esi, edi
        add    ecx, edi
loc_1_1700:
        pop    eax
        pop    edi
        popfd
        ret
loc_1_1704:
        inc    esi
        jmp    SHORT loc_1_1700
        db  0E3h, 006h, 04Eh, 049h, 074h, 002h, 0EBh, 0B9h, 0C3h
sub_1_1710:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, esi
        sub    ecx, dword ptr [esp + 8]
        je     SHORT loc_1_1726
        dec    esi
        dec    ecx
        je     SHORT loc_1_1726
        call   sub_1_16C8
loc_1_1726:
        mov    eax, esi
        pop    esi
        ret
        db  056h, 08Bh, 074h, 024h, 00Ch, 08Bh, 0CEh, 02Bh, 04Ch, 024h, 008h, 074h, 005h, 0E8h, 08Ch, 0FFh
        db  0FFh, 0FFh, 08Bh, 0C6h, 05Eh, 0C3h
sub_1_1740:
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        jecxz  loc_1_1756
        mov    esi, dword ptr [esp + 8]
        add    esi, ecx
        dec    esi
        dec    ecx
        je     SHORT loc_1_1756
        call   sub_1_16C8
loc_1_1756:
        mov    eax, ecx
        pop    esi
        ret
        db  056h, 08Bh, 04Ch, 024h, 00Ch, 0E3h, 00Bh, 08Bh, 074h, 024h, 008h, 003h, 0F1h, 0E8h, 05Ch, 0FFh
        db  0FFh, 0FFh, 08Bh, 0C1h, 05Eh, 0C3h, 051h, 056h, 057h, 08Bh, 0FEh, 0E8h, 04Eh, 0FFh, 0FFh, 0FFh
        db  03Bh, 0FEh, 05Fh, 05Eh, 059h, 0C3h, 053h, 057h, 0E3h, 01Eh, 08Bh, 0FEh, 0FDh, 04Fh, 0F2h, 0AEh
        db  075h, 016h, 047h, 08Bh, 0F7h, 08Bh, 0D9h, 0E8h, 032h, 0FFh, 0FFh, 0FFh, 03Bh, 0F7h, 075h, 004h
        db  0FCh, 05Fh, 05Bh, 0C3h, 08Bh, 0CBh, 0EBh, 0E5h, 0F9h, 0EBh, 0F5h, 02Eh, 08Bh, 0C0h
loc_1_17A8:
        push   edi
        mov    edi, dword ptr [esp + 8]
        movzx  ecx, byte ptr [edi]
        jecxz  loc_1_17C5
        inc    edi
        mov    al, byte ptr [esp + 0ch]
        mov    dl, byte ptr [esp + 10h]
        cld
loc_1_17BC:
        repne scasb
        jecxz  loc_1_17C5
        mov    byte ptr [edi - 1], dl
        jmp    SHORT loc_1_17BC
loc_1_17C5:
        pop    edi
        ret
        db  090h
sub_1_17C8:
        db  083h, 03Dh, 034h, 0A0h, 000h, 000h, 000h
        je     SHORT loc_1_17A8
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esi]
        jecxz  loc_1_17FD
        inc    esi
        mov    dh, byte ptr [esp + 0ch]
        mov    dl, byte ptr [esp + 10h]
        xor    eax, eax
loc_1_17E6:
        lodsb
        test   al, al
        je     SHORT loc_1_17FD
        db  080h, 0B8h, 038h, 0A0h, 000h, 000h, 000h
        je     SHORT loc_1_17FF
        cmp    al, dh
        jne    SHORT loc_1_17FB
        mov    byte ptr [esi - 1], dl
loc_1_17FB:
        loop   loc_1_17E6
loc_1_17FD:
        pop    esi
        ret
loc_1_17FF:
        inc    esi
        dec    ecx
        jecxz  loc_1_17FD
        loop   loc_1_17E6
        jmp    SHORT loc_1_17FD
        db  090h
sub_1_1808:
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    ecx, dword ptr [esp + 18h]
        jecxz  loc_1_183C
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 14h]
        db  08Dh, 01Dh, 038h, 0A0h, 000h, 000h
        cld
loc_1_1822:
        lodsb
        test   al, al
        je     SHORT loc_1_1843
        mov    edx, eax
        xlatb
        test   al, al
        je     SHORT loc_1_1848
        mov    edx, eax
        mov    al, byte ptr [edi]
        xlatb
        cmp    al, dl
        jne    SHORT loc_1_183C
loc_1_1837:
        inc    edi
        loop   loc_1_1822
loc_1_183A:
        xor    eax, eax
loc_1_183C:
        setne  al
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_1843:
        cmp    byte ptr [edi], 0
        jmp    SHORT loc_1_183C
loc_1_1848:
        cmp    dl, byte ptr [edi]
        jne    SHORT loc_1_183C
        dec    ecx
        jecxz  loc_1_183A
        lodsb
        inc    edi
        cmp    al, 10h
        jb     SHORT loc_1_185A
        cmp    al, 13h
        ja     SHORT loc_1_185A
        xlatb
loc_1_185A:
        mov    edx, eax
        mov    al, byte ptr [edi]
        cmp    al, 10h
        jb     SHORT loc_1_1867
        cmp    al, 13h
        ja     SHORT loc_1_1867
        xlatb
loc_1_1867:
        cmp    al, dl
        jne    SHORT loc_1_183C
        test   al, al
        je     SHORT loc_1_183C
        jmp    SHORT loc_1_1837
        db  02Eh, 08Bh, 0C0h
sub_1_1874:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        db  08Dh, 01Dh, 038h, 0A0h, 000h, 000h
        cld
        jecxz  loc_1_189B
loc_1_188C:
        lodsb
        test   al, al
        je     SHORT loc_1_189F
        mov    edx, eax
        xlatb
        test   al, al
        je     SHORT loc_1_18A2
        stosb
loc_1_1899:
        loop   loc_1_188C
loc_1_189B:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_189F:
        stosb
        jmp    SHORT loc_1_189B
loc_1_18A2:
        mov    eax, edx
        stosb
        dec    ecx
        jecxz  loc_1_189B
        lodsb
        test   al, al
        je     SHORT loc_1_18B9
        cmp    al, 10h
        jb     SHORT loc_1_18B6
        cmp    al, 13h
        ja     SHORT loc_1_18B6
        xlatb
loc_1_18B6:
        stosb
        jmp    SHORT loc_1_1899
loc_1_18B9:
        stosb
        jmp    SHORT loc_1_189B
sub_1_18BC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esi]
        jecxz  loc_1_18EC
        inc    esi
        xor    eax, eax
        cmp    cl, 11h
        jb     SHORT loc_1_18D0
        mov    cl, 11h
loc_1_18D0:
        lodsb
        cmp    al, 5ch
        je     SHORT loc_1_18EC
        cmp    al, 3ah
        je     SHORT loc_1_18EE
        cmp    al, 2fh
        je     SHORT loc_1_18EC
        test   al, al
        je     SHORT loc_1_18EC
        db  080h, 0B8h, 038h, 0A0h, 000h, 000h, 000h
        je     SHORT loc_1_18F0
        loop   loc_1_18D0
loc_1_18EC:
        xor    eax, eax
loc_1_18EE:
        pop    esi
        ret
loc_1_18F0:
        dec    ecx
        je     SHORT loc_1_18EC
        lodsb
        test   al, al
        je     SHORT loc_1_18EC
        loop   loc_1_18D0
        jmp    SHORT loc_1_18EC
sub_1_18FC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esi]
        jecxz  loc_1_1933
        xor    edx, edx
        inc    esi
        xor    eax, eax
        cmp    cl, 11h
        jb     SHORT loc_1_1912
        mov    cl, 11h
loc_1_1912:
        lodsb
        cmp    al, 5ch
        je     SHORT loc_1_1933
        cmp    al, 3ah
        je     SHORT loc_1_192F
        cmp    al, 2fh
        je     SHORT loc_1_1933
        test   al, al
        je     SHORT loc_1_192F
        db  080h, 0B8h, 038h, 0A0h, 000h, 000h, 000h
        je     SHORT loc_1_1937
        inc    edx
        loop   loc_1_1912
loc_1_192F:
        mov    eax, edx
        pop    esi
        ret
loc_1_1933:
        xor    edx, edx
        jmp    SHORT loc_1_192F
loc_1_1937:
        dec    ecx
        je     SHORT loc_1_1933
        lodsb
        test   al, al
        je     SHORT loc_1_1933
        add    edx, 2
        loop   loc_1_1912
        jmp    SHORT loc_1_192F
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1950:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 30h], 0
        jne    SHORT loc_1_1963
        push   eax
        call   sub_1_27A4
        add    esp, 4
loc_1_1963:
        ret
sub_1_1964:
        push   esi
        push   edi
        mov    edi, dword ptr [ecx + 24h]
        mov    word ptr [ecx + 28h], dx
        lea    esi, [edi + 12h]
        add    edi, 6
        movsd
        movsd
        movsd
        pop    edi
        pop    esi
        ret
sub_1_1979:
        push   esi
        push   edi
        db  03Bh, 015h, 024h, 0B4h, 000h, 000h
        ja     SHORT loc_1_19B0
        or     edx, edx
        je     SHORT loc_1_19B0
        db  0A1h, 04Ch, 0B4h, 000h, 000h
        mov    eax, dword ptr [eax + edx*4]
        db  039h, 005h, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_1_1999
        jmp    SHORT loc_1_19B0
loc_1_1999:
        lea    esi, [ecx + 12h]
        lea    edi, [eax + 28h]
        mov    ecx, 2
        repe cmpsd
        jne    SHORT loc_1_19AE
        mov    cx, word ptr [esi]
        cmp    cx, word ptr [edi]
loc_1_19AE:
        je     SHORT loc_1_19B2
loc_1_19B0:
        sub    eax, eax
loc_1_19B2:
        pop    edi
        pop    esi
        ret
sub_1_19B5:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 34h], ecx
        add    eax, 38h
        db  068h, 0C0h, 0B3h, 000h, 000h
        push   eax
        call   sub_1_0047
        add    esp, 8
        db  0FEh, 005h, 0D1h, 0B1h, 000h, 000h
        db  0FFh, 035h, 0B8h, 0B3h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
        ret
loc_1_19E6:
        mov    ecx, dword ptr [esp + 8]
        cmp    word ptr [ecx + 1eh], 7777h
        jne    SHORT loc_1_1A0A
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   ecx
        call   sub_1_4995
        add    esp, 10h
        jmp    SHORT loc_1_1A19
loc_1_1A0A:
        call   sub_1_2760
        or     eax, eax
        jne    SHORT loc_1_1A1D
        db  0FFh, 005h, 038h, 0A1h, 000h, 000h
loc_1_1A19:
        or     eax, eax
        je     SHORT loc_1_1A26
loc_1_1A1D:
        mov    cx, word ptr [esp + 4]
        mov    word ptr [eax + 0ah], cx
loc_1_1A26:
        ret
sub_1_1A27:
        push   esi
        db  08Bh, 035h, 0FCh, 0A1h, 000h, 000h
        db  003h, 035h, 038h, 0A1h, 000h, 000h
        db  0A1h, 03Ch, 0A1h, 000h, 000h
        dec    eax
        db  0A3h, 03Ch, 0A1h, 000h, 000h
        db  0C7h, 005h, 040h, 0A1h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 038h, 0A1h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1A53:
        db  039h, 035h, 0F8h, 0A1h, 000h, 000h
        jae    SHORT loc_1_1A71
        db  0A1h, 000h, 0A2h, 000h, 000h
        db  039h, 005h, 004h, 0A2h, 000h, 000h
        jbe    SHORT loc_1_1A71
        call   sub_1_2887
        or     eax, eax
        jne    SHORT loc_1_1A53
loc_1_1A71:
        pop    esi
        ret
loc_1_1A73:
        push   ebx
        push   esi
        push   edi
        db  0A1h, 0FCh, 0A1h, 000h, 000h
        db  003h, 005h, 038h, 0A1h, 000h, 000h
        db  03Bh, 005h, 0F8h, 0A1h, 000h, 000h
        jbe    SHORT loc_1_1AD8
        db  0A1h, 000h, 0A2h, 000h, 000h
        db  039h, 005h, 004h, 0A2h, 000h, 000h
        jbe    SHORT loc_1_1AD8
        db  083h, 03Dh, 000h, 0B4h, 000h, 000h, 000h
        jne    SHORT loc_1_1AA6
        call   sub_1_1A27
        jmp    SHORT loc_1_1AD8
loc_1_1AA6:
        db  083h, 03Dh, 03Ch, 0A1h, 000h, 000h, 000h
        jne    SHORT loc_1_1AD8
        db  0FFh, 005h, 03Ch, 0A1h, 000h, 000h
        push   0
        db  068h, 027h, 01Ah, 000h, 000h
        push   0
        push   2
        db  0FFh, 035h, 098h, 0B1h, 000h, 000h
        push   1000h
        call   sub_1_43BD
        add    esp, 18h
        db  0A3h, 040h, 0A1h, 000h, 000h
loc_1_1AD8:
        mov    esi, dword ptr [esp + 10h]
        cmp    byte ptr [esi + 9], 0
        jne    NEAR PTR loc_1_1C75
        mov    ecx, dword ptr [esi + 24h]
        movzx  eax, word ptr [ecx + 1eh]
        lea    edi, [ecx + 1eh]
        cmp    eax, 1111h
        je     SHORT loc_1_1B0A
        cmp    eax, 2222h
        je     SHORT loc_1_1B15
        cmp    eax, 5555h
        je     SHORT loc_1_1B15
        jmp    NEAR PTR loc_1_1C75
loc_1_1B0A:
        push   esi
        call   sub_1_7A24
        jmp    NEAR PTR loc_1_1C7B
loc_1_1B15:
        movzx  eax, byte ptr [edi + 5]
        shl    eax, 8
        movzx  edx, byte ptr [edi + 3]
        add    edx, eax
        call   sub_1_1979
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_1B70
        cmp    word ptr [edi], 5555h
        jne    SHORT loc_1_1B3E
        mov    byte ptr [edi + 6], 0
        mov    byte ptr [edi + 7], 0
        jmp    SHORT loc_1_1B46
loc_1_1B3E:
        mov    byte ptr [edi + 6], 0ffh
        mov    byte ptr [edi + 7], 1
loc_1_1B46:
        mov    word ptr [edi], 3333h
        mov    edx, 26h
        mov    ecx, esi
        call   sub_1_1964
        db  0C7h, 046h, 004h, 050h, 019h, 000h, 000h
        mov    dword ptr [esi + 30h], 0
        push   esi
        call   sub_1_318D
        jmp    NEAR PTR loc_1_1C7B
loc_1_1B70:
        test   byte ptr [ebx + 1ah], 8
        jne    NEAR PTR loc_1_1C19
        mov    al, byte ptr [edi + 2]
        cmp    byte ptr [ebx + 21h], al
        jne    SHORT loc_1_1BAE
        cmp    dword ptr [ebx + 34h], 0
        jne    NEAR PTR loc_1_1C19
        call   sub_1_16C0
        mov    dword ptr [ebx + 54h], eax
        db  0FFh, 005h, 070h, 0B1h, 000h, 000h
        or     word ptr [ebx + 1ah], 4
        push   esi
        push   ebx
        call   sub_1_19B5
        add    esp, 8
        jmp    NEAR PTR loc_1_1C7E
loc_1_1BAE:
        cmp    dword ptr [ebx + 34h], 0
        jne    NEAR PTR loc_1_1C75
        call   sub_1_16C0
        mov    dword ptr [ebx + 54h], eax
        mov    eax, dword ptr [ebx + 1ch]
        or     eax, eax
        je     SHORT loc_1_1BE7
        mov    dword ptr [eax + 30h], 0
        mov    eax, dword ptr [ebx + 1ch]
        cmp    byte ptr [eax + 8], 0
        jne    SHORT loc_1_1BE0
        push   eax
        call   sub_1_27A4
        add    esp, 4
loc_1_1BE0:
        mov    dword ptr [ebx + 1ch], 0
loc_1_1BE7:
        mov    al, byte ptr [ebx + 21h]
        movzx  ecx, byte ptr [edi + 2]
        movzx  edx, al
        inc    edx
        and    edx, 0ffh
        cmp    edx, ecx
        jne    SHORT loc_1_1C75
        inc    al
        mov    byte ptr [ebx + 21h], al
        db  0FFh, 005h, 080h, 0B1h, 000h, 000h
        and    word ptr [ebx + 1ah], 0fffbh
        push   esi
        push   ebx
        call   sub_1_19B5
        add    esp, 8
        jmp    SHORT loc_1_1C7E
loc_1_1C19:
        db  083h, 03Dh, 03Ch, 0B4h, 000h, 000h, 000h
        jne    SHORT loc_1_1C75
        call   sub_1_16C0
        sub    eax, dword ptr [ebx + 54h]
        cmp    eax, 0afc8h
        ja     SHORT loc_1_1C69
        mov    word ptr [edi], 9999h
        mov    byte ptr [edi + 6], 0
        cmp    word ptr [ebx + 60h], 1
        cmc
        sbb    al, al
        and    al, 40h
        mov    byte ptr [edi + 7], al
        mov    edx, 26h
        mov    ecx, esi
        call   sub_1_1964
        db  0C7h, 046h, 004h, 050h, 019h, 000h, 000h
        mov    dword ptr [esi + 30h], 0
        push   esi
        call   sub_1_318D
        jmp    SHORT loc_1_1C7B
loc_1_1C69:
        db  0FFh, 005h, 03Ch, 0B4h, 000h, 000h
        or     word ptr [ebx + 1ah], 80h
loc_1_1C75:
        push   esi
        call   sub_1_27A4
loc_1_1C7B:
        add    esp, 4
loc_1_1C7E:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1C90:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        sub    ebp, ebp
        mov    word ptr [esp + 12h], bx
        mov    edx, dword ptr [esp + 34h]
        sub    edi, edi
        cmp    byte ptr [edx], bl
        mov    esi, dword ptr [esp + 30h]
        jne    SHORT loc_1_1CBD
        movzx  eax, byte ptr [esi]
        mov    byte ptr [esi + eax + 1], bl
        sub    ax, ax
        jmp    NEAR PTR loc_1_209D
loc_1_1CBD:
        mov    al, byte ptr [esi]
        or     al, al
        je     SHORT loc_1_1CDB
        lea    ecx, [esi + 1]
        movzx  eax, al
        add    eax, esi
        inc    eax
        push   eax
        push   ecx
        call   sub_1_1710
        add    esp, 8
        cmp    byte ptr [eax], 5ch
        je     SHORT loc_1_1CF1
loc_1_1CDB:
        mov    al, byte ptr [esi]
        cmp    byte ptr [esp + 38h], al
        jne    SHORT loc_1_1CE7
        inc    byte ptr [esp + 38h]
loc_1_1CE7:
        movzx  eax, al
        mov    byte ptr [esi + eax + 1], 5ch
        inc    byte ptr [esi]
loc_1_1CF1:
        mov    edx, dword ptr [esp + 34h]
        cmp    byte ptr [edx], 0
        jbe    NEAR PTR loc_1_2044
        movzx  eax, byte ptr [esp + 3ch]
        mov    dword ptr [esp + 18h], eax
loc_1_1D07:
        cmp    ebx, dword ptr [esp + 18h]
        jae    NEAR PTR loc_1_2044
        mov    ecx, dword ptr [esp + 34h]
        add    ecx, ebp
        mov    dword ptr [esp + 28h], ecx
        mov    al, byte ptr [ecx + 1]
        or     al, al
        je     NEAR PTR loc_1_2075
        movzx  ecx, al
        db  080h, 0B9h, 038h, 0A0h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1DE3
        or     edi, edi
        je     SHORT loc_1_1D4E
        cmp    edi, 1
        je     SHORT loc_1_1D5A
        cmp    edi, 2
        je     SHORT loc_1_1D6F
        cmp    edi, 3
        je     SHORT loc_1_1D53
        jmp    NEAR PTR loc_1_202C
loc_1_1D4E:
        movzx  ebx, byte ptr [esi]
        jmp    SHORT loc_1_1D5A
loc_1_1D53:
        mov    word ptr [esp + 12h], 9ch
loc_1_1D5A:
        mov    edi, 2
        mov    dword ptr [esp + 24h], 0
        mov    dword ptr [esp + 14h], 0
loc_1_1D6F:
        lea    ecx, [ebx + 1]
        cmp    dword ptr [esp + 18h], ecx
        jbe    NEAR PTR loc_1_203D
        mov    edx, dword ptr [esp + 34h]
        inc    ebp
        movzx  edx, byte ptr [edx]
        cmp    edx, ebp
        jbe    NEAR PTR loc_1_203D
        mov    edx, dword ptr [esp + 34h]
        lea    edx, [ebp + edx + 1]
        cmp    byte ptr [edx], 0
        je     NEAR PTR loc_1_203D
        mov    byte ptr [esi + ebx + 1], al
        mov    al, byte ptr [edx]
        inc    ebp
        cmp    byte ptr [esp + 40h], 0ffh
        je     SHORT loc_1_1DD2
        movzx  edx, al
        cmp    edx, 10h
        je     SHORT loc_1_1DC4
        cmp    edx, 11h
        je     SHORT loc_1_1DC8
        cmp    edx, 12h
        je     SHORT loc_1_1DCC
        cmp    edx, 13h
        je     SHORT loc_1_1DD0
        jmp    SHORT loc_1_1DD2
loc_1_1DC4:
        mov    al, 0bfh
        jmp    SHORT loc_1_1DD2
loc_1_1DC8:
        mov    al, 0aeh
        jmp    SHORT loc_1_1DD2
loc_1_1DCC:
        mov    al, 0aah
        jmp    SHORT loc_1_1DD2
loc_1_1DD0:
        mov    al, 5ch
loc_1_1DD2:
        mov    byte ptr [esi + ecx + 1], al
        lea    ebx, [ecx + 1]
        add    dword ptr [esp + 14h], 2
        jmp    NEAR PTR loc_1_202C
loc_1_1DE3:
        movzx  edx, byte ptr [esp + 40h]
        or     edx, edx
        je     SHORT loc_1_1DFE
        cmp    edx, 4
        je     SHORT loc_1_1DFE
        cmp    edx, 0feh
        je     SHORT loc_1_1E4D
        jmp    NEAR PTR loc_1_1ED6
loc_1_1DFE:
        cmp    ecx, 2fh
        je     SHORT loc_1_1E10
        cmp    ecx, 0ffh
        je     SHORT loc_1_1E17
        jmp    NEAR PTR loc_1_1ED6
loc_1_1E10:
        mov    al, 5ch
        jmp    NEAR PTR loc_1_1ED6
loc_1_1E17:
        lea    ecx, [ebp + 1]
        mov    edx, dword ptr [esp + 34h]
        mov    dword ptr [esp + 20h], ecx
        movzx  ecx, byte ptr [edx]
        cmp    ecx, dword ptr [esp + 20h]
        jbe    NEAR PTR loc_1_1ED6
        mov    ecx, dword ptr [esp + 28h]
        mov    cl, byte ptr [ecx + 2]
        and    cl, 7fh
        cmp    cl, 3fh
        je     NEAR PTR loc_1_1ED0
        cmp    cl, 2ah
        je     NEAR PTR loc_1_1ED0
        jmp    SHORT loc_1_1ECB
loc_1_1E4D:
        cmp    ecx, 0aah
        jg     SHORT loc_1_1E6D
        je     SHORT loc_1_1E97
        cmp    ecx, 10h
        je     SHORT loc_1_1E87
        cmp    ecx, 11h
        je     SHORT loc_1_1E8B
        cmp    ecx, 12h
        je     SHORT loc_1_1E8F
        cmp    ecx, 2fh
        je     SHORT loc_1_1E93
        jmp    SHORT loc_1_1ED6
loc_1_1E6D:
        cmp    ecx, 0aeh
        je     SHORT loc_1_1E9B
        cmp    ecx, 0bfh
        je     SHORT loc_1_1E9F
        cmp    ecx, 0ffh
        je     SHORT loc_1_1EA3
        jmp    SHORT loc_1_1ED6
loc_1_1E87:
        mov    al, 0bfh
        jmp    SHORT loc_1_1ED6
loc_1_1E8B:
        mov    al, 0aeh
        jmp    SHORT loc_1_1ED6
loc_1_1E8F:
        mov    al, 0aah
        jmp    SHORT loc_1_1ED6
loc_1_1E93:
        mov    al, 5ch
        jmp    SHORT loc_1_1ED6
loc_1_1E97:
        mov    al, 2ah
        jmp    SHORT loc_1_1ED6
loc_1_1E9B:
        mov    al, 2eh
        jmp    SHORT loc_1_1ED6
loc_1_1E9F:
        mov    al, 3fh
        jmp    SHORT loc_1_1ED6
loc_1_1EA3:
        lea    ecx, [ebp + 1]
        mov    edx, dword ptr [esp + 34h]
        mov    dword ptr [esp + 20h], ecx
        movzx  ecx, byte ptr [edx]
        cmp    ecx, dword ptr [esp + 20h]
        jbe    SHORT loc_1_1ED6
        mov    ecx, dword ptr [esp + 28h]
        mov    cl, byte ptr [ecx + 2]
        and    cl, 7fh
        cmp    cl, 3fh
        je     SHORT loc_1_1ED0
        cmp    cl, 2ah
        je     SHORT loc_1_1ED0
loc_1_1ECB:
        cmp    cl, 2eh
        jne    SHORT loc_1_1ED6
loc_1_1ED0:
        mov    ebp, dword ptr [esp + 20h]
        mov    al, cl
loc_1_1ED6:
        cmp    al, 3fh
        je     SHORT loc_1_1EDE
        cmp    al, 2ah
        jne    SHORT loc_1_1EEB
loc_1_1EDE:
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        mov    byte ptr [ecx + 494h], 1
loc_1_1EEB:
        or     edi, edi
        je     SHORT loc_1_1F07
        cmp    edi, 1
        je     SHORT loc_1_1F1F
        cmp    edi, 2
        je     NEAR PTR loc_1_1FC9
        cmp    edi, 3
        je     SHORT loc_1_1F42
        jmp    NEAR PTR loc_1_202C
loc_1_1F07:
        mov    edi, 1
        cmp    al, 5ch
        jne    SHORT loc_1_1F1C
        add    ebp, edi
        movzx  ebx, byte ptr [esp + 38h]
        jmp    NEAR PTR loc_1_202C
loc_1_1F1C:
        movzx  ebx, byte ptr [esi]
loc_1_1F1F:
        mov    dword ptr [esp + 24h], 0
        mov    dword ptr [esp + 14h], 0
        cmp    al, 2eh
        jne    NEAR PTR loc_1_1FC4
        inc    ebp
        mov    edi, 3
        jmp    NEAR PTR loc_1_202C
loc_1_1F42:
        cmp    al, 2eh
        jne    SHORT loc_1_1F96
        inc    ebp
        or     ebx, ebx
        je     NEAR PTR loc_1_202C
        push   ebx
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_1740
        movzx  ecx, byte ptr [esp + 40h]
        mov    dword ptr [esp + 24h], ecx
        add    esp, 8
loc_1_1F65:
        cmp    eax, dword ptr [esp + 1ch]
        jb     NEAR PTR loc_1_202C
        or     eax, eax
        je     SHORT loc_1_1F8F
        push   eax
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_1740
        add    esp, 8
        cmp    byte ptr [esi + eax + 1], 5ch
        jne    SHORT loc_1_1F65
        lea    ebx, [eax + 1]
        jmp    NEAR PTR loc_1_202C
loc_1_1F8F:
        sub    ebx, ebx
        jmp    NEAR PTR loc_1_202C
loc_1_1F96:
        cmp    al, 5ch
        jne    SHORT loc_1_1FA0
        inc    ebp
        jmp    NEAR PTR loc_1_2027
loc_1_1FA0:
        mov    word ptr [esp + 12h], 9ch
        mov    byte ptr [esi + ebx + 1], al
        inc    ebx
        inc    ebp
        mov    edi, 2
        mov    dword ptr [esp + 24h], 0
        mov    dword ptr [esp + 14h], 0
        jmp    SHORT loc_1_202C
loc_1_1FC4:
        mov    edi, 2
loc_1_1FC9:
        inc    ebp
        cmp    al, 2ah
        jne    SHORT loc_1_2004
        cmp    byte ptr [esp + 40h], 0feh
        jne    SHORT loc_1_2004
        cmp    dword ptr [esp + 24h], 1
        sbb    ecx, ecx
        and    ecx, 5
        add    ecx, 3
        cmp    dword ptr [esp + 14h], ecx
        jae    SHORT loc_1_202C
        mov    eax, 3fh
loc_1_1FED:
        cmp    ebx, dword ptr [esp + 18h]
        jae    SHORT loc_1_203D
        mov    byte ptr [esi + ebx + 1], al
        inc    ebx
        inc    dword ptr [esp + 14h]
        cmp    dword ptr [esp + 14h], ecx
        jb     SHORT loc_1_1FED
        jmp    SHORT loc_1_202C
loc_1_2004:
        mov    byte ptr [esi + ebx + 1], al
        inc    ebx
        cmp    al, 2eh
        jne    SHORT loc_1_201F
        mov    dword ptr [esp + 24h], 1
        mov    dword ptr [esp + 14h], 0
        jmp    SHORT loc_1_202C
loc_1_201F:
        inc    dword ptr [esp + 14h]
        cmp    al, 5ch
        jne    SHORT loc_1_202C
loc_1_2027:
        mov    edi, 1
loc_1_202C:
        mov    edx, dword ptr [esp + 34h]
        movzx  eax, byte ptr [edx]
        cmp    eax, ebp
        ja     NEAR PTR loc_1_1D07
        jmp    SHORT loc_1_2044
loc_1_203D:
        mov    word ptr [esp + 12h], 9ch
loc_1_2044:
        cmp    ebx, 3
        ja     SHORT loc_1_205E
        jne    SHORT loc_1_2075
        movzx  eax, byte ptr [esi + 1]
        db  080h, 0B8h, 038h, 0A0h, 000h, 000h, 000h
        je     SHORT loc_1_2075
        cmp    byte ptr [esi + 2], 3ah
        je     SHORT loc_1_2075
loc_1_205E:
        lea    eax, [esi + ebx]
        inc    eax
        push   eax
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_1710
        add    esp, 8
        cmp    byte ptr [eax], 5ch
        jne    SHORT loc_1_2075
        dec    ebx
loc_1_2075:
        mov    byte ptr [esi], bl
        movzx  eax, bl
        mov    edx, dword ptr [esp + 34h]
        mov    byte ptr [esi + eax + 1], 0
        movzx  eax, byte ptr [edx]
        cmp    eax, ebp
        jbe    SHORT loc_1_2098
        cmp    byte ptr [edx + ebp + 1], 0
        je     SHORT loc_1_2098
        mov    word ptr [esp + 12h], 9ch
loc_1_2098:
        mov    ax, word ptr [esp + 12h]
loc_1_209D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_20B0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     SHORT loc_1_20F7
        mov    ecx, dword ptr [eax - 28h]
        lea    esi, [eax - 3ch]
        or     ecx, ecx
        je     SHORT loc_1_20C9
        push   eax
        call   ecx
        add    esp, 4
loc_1_20C9:
        push   esi
        call   sub_1_4390
        add    esp, 4
        db  0FFh, 00Dh, 044h, 0A1h, 000h, 000h
        jne    SHORT loc_1_20F7
        db  083h, 03Dh, 048h, 0A1h, 000h, 000h, 000h
        je     SHORT loc_1_20F7
        db  0FFh, 00Dh, 048h, 0A1h, 000h, 000h
        db  0FFh, 035h, 04Ch, 0A1h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
loc_1_20F7:
        pop    esi
        ret
sub_1_20F9:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        db  08Bh, 035h, 094h, 0B8h, 000h, 000h
        sub    esi, 3ch
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 4], eax
        call   sub_1_92B3
        db  08Bh, 00Dh, 090h, 0B8h, 000h, 000h
        mov    dword ptr [ecx + eax], 0
        add    esi, 3ch
        push   esi
        call   sub_1_20B0
        add    esp, 4
        mov    esp, dword ptr [ebp - 4]
        ret
sub_1_2131:
        push   esi
        mov    esi, dword ptr [esp + 14h]
        call   sub_1_92B3
        mov    dword ptr [esi + 18h], eax
        lea    edx, [esi + 3ch]
        db  08Bh, 00Dh, 090h, 0B8h, 000h, 000h
        mov    dword ptr [eax + ecx], edx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 8], eax
        lea    eax, [esi + 1ch]
        db  089h, 015h, 094h, 0B8h, 000h, 000h
        push   eax
        push   dword ptr [esi + 10h]
        push   dword ptr [esi + 0ch]
        call   sub_1_9319
        add    esp, 0ch
        call   sub_1_20F9
        pop    esi
        ret
loc_1_2170:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi + 4]
        call   dword ptr [esi]
        add    esp, 4
        add    esi, 3ch
        push   esi
        call   sub_1_20B0
        add    esp, 4
        pop    esi
        ret
sub_1_218B:
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [esp + 8]
        push   201h
        add    eax, 3ch
        push   eax
        call   sub_1_4364
        add    esp, 8
        cmp    eax, esi
        je     SHORT loc_1_21B1
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 14h], ecx
        lea    esi, [eax + 3ch]
loc_1_21B1:
        mov    eax, esi
        pop    esi
        ret
sub_1_21B5:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        call   sub_1_92B3
        mov    dword ptr [edi - 24h], eax
        db  08Bh, 00Dh, 090h, 0B8h, 000h, 000h
        mov    dword ptr [eax + ecx], edi
        db  089h, 03Dh, 094h, 0B8h, 000h, 000h
        lea    esi, [edi - 3ch]
        pop    edi
        pop    esi
        ret
sub_1_21D8:
        call   sub_1_92B3
        db  08Bh, 00Dh, 090h, 0B8h, 000h, 000h
        mov    dword ptr [eax + ecx], 0
        ret
sub_1_21EB:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        cmp    dword ptr [esp + 24h], 8
        jbe    SHORT loc_1_21FF
        mov    dword ptr [esp + 24h], 8
loc_1_21FF:
        mov    eax, dword ptr [esp + 10h]
        push   201h
        add    eax, 3ch
        push   eax
        call   sub_1_4364
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_2299
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edi + 0ch], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 14h], eax
        sub    ecx, ecx
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edi + 10h], eax
        cmp    dword ptr [esp + 24h], ecx
        jbe    SHORT loc_1_224F
        lea    edx, [esp + 28h]
        lea    eax, [edi + 1ch]
loc_1_223E:
        mov    ebx, dword ptr [edx]
        mov    dword ptr [eax], ebx
        add    edx, 4
        add    eax, 4
        inc    ecx
        cmp    ecx, dword ptr [esp + 24h]
        jb     SHORT loc_1_223E
loc_1_224F:
        db  0FFh, 005h, 044h, 0A1h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        or     eax, eax
        mov    ecx, 0
        je     SHORT loc_1_2270
        db  0B9h, 070h, 021h, 000h, 000h
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 4], eax
loc_1_2270:
        push   ecx
        db  068h, 03Ch, 093h, 000h, 000h
        push   edi
        push   400h
        call   sub_1_928A
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_2296
        add    edi, 3ch
        push   edi
        call   sub_1_20B0
        add    esp, 4
        jmp    SHORT loc_1_2299
loc_1_2296:
        lea    esi, [edi + 3ch]
loc_1_2299:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_229F:
        push   esi
        sub    esi, esi
        push   esi
        call   sub_1_92D8
        add    esp, 4
        db  0A3h, 04Ch, 0A1h, 000h, 000h
        db  0A1h, 090h, 0B8h, 000h, 000h
        cmp    eax, esi
        jne    SHORT loc_1_22BE
        call   sub_1_92A7
loc_1_22BE:
        db  0A3h, 090h, 0B8h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_22DE
        db  068h, 09Ah, 093h, 000h, 000h
        call   sub_1_92BE
        add    esp, 4
        mov    esi, 1
        db  0A1h, 090h, 0B8h, 000h, 000h
loc_1_22DE:
        db  0A3h, 090h, 0B8h, 000h, 000h
        or     esi, esi
        jne    SHORT loc_1_22FE
        or     eax, eax
        je     SHORT loc_1_22FE
        push   eax
        call   sub_1_92AD
        add    esp, 4
        db  0C7h, 005h, 090h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_22FE:
        mov    eax, esi
        pop    esi
        ret
sub_1_2302:
        db  083h, 03Dh, 044h, 0A1h, 000h, 000h, 000h
        je     SHORT loc_1_2359
        db  08Bh, 00Dh, 048h, 0A1h, 000h, 000h
loc_1_2311:
        inc    ecx
        db  089h, 00Dh, 048h, 0A1h, 000h, 000h
        db  0FFh, 035h, 04Ch, 0A1h, 000h, 000h
        call   sub_1_92FB
        add    esp, 4
        db  08Bh, 00Dh, 048h, 0A1h, 000h, 000h
        or     ecx, ecx
        db  0A1h, 044h, 0A1h, 000h, 000h
        je     SHORT loc_1_2355
        dec    ecx
        db  089h, 00Dh, 048h, 0A1h, 000h, 000h
        db  0FFh, 035h, 04Ch, 0A1h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
        db  08Bh, 00Dh, 048h, 0A1h, 000h, 000h
        db  0A1h, 044h, 0A1h, 000h, 000h
loc_1_2355:
        or     eax, eax
        jne    SHORT loc_1_2311
loc_1_2359:
        ret
loc_1_235A:
        call   sub_1_2302
        db  068h, 09Ah, 093h, 000h, 000h
        call   sub_1_92CB
        add    esp, 4
        db  0C7h, 005h, 094h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 090h, 0B8h, 000h, 000h
        call   sub_1_92AD
        add    esp, 4
        db  0C7h, 005h, 090h, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 04Ch, 0A1h, 000h, 000h
        call   sub_1_92E7
        add    esp, 4
        db  0C7h, 005h, 04Ch, 0A1h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_23B0:
        db  068h, 054h, 0A1h, 000h, 000h
        call   sub_1_93BC
        add    esp, 4
        ret
sub_1_23BE:
        push   esi
        db  068h, 054h, 0A1h, 000h, 000h
        call   sub_1_9408
        add    esp, 4
        db  083h, 03Dh, 050h, 0A1h, 000h, 000h, 000h
        je     SHORT loc_1_23F7
        sub    esi, esi
loc_1_23D7:
        db  08Bh, 00Dh, 050h, 0A1h, 000h, 000h
        mov    eax, ecx
        push   ecx
        mov    eax, dword ptr [eax]
        db  0A3h, 050h, 0A1h, 000h, 000h
        call   sub_1_4390
        add    esp, 4
        db  039h, 035h, 050h, 0A1h, 000h, 000h
        jne    SHORT loc_1_23D7
loc_1_23F7:
        pop    esi
        ret
sub_1_23F9:
        sub    esp, 8
        push   esi
        push   edi
        db  0BEh, 0B8h, 0A1h, 000h, 000h
        lea    edi, [esp + 0ch]
        movsd
        db  0BEh, 0BCh, 0A1h, 000h, 000h
        lea    edi, [esp + 8]
        movsw
        movsb
        mov    esi, 1
loc_1_2419:
        push   esi
        call   sub_1_3B90
        add    esp, 4
        cmp    eax, 1
        jne    SHORT loc_1_243F
        push   0
        push   0
        lea    eax, [esp + 14h]
        push   5
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        db  0E8h, 059h, 010h, 0FFh, 0FFh
        add    esp, 14h
loc_1_243F:
        inc    esi
        inc    byte ptr [esp + 0ch]
        inc    byte ptr [esp + 8]
        cmp    esi, 1ah
        jle    SHORT loc_1_2419
        push   0
        push   0
        push   5
        call   sub_1_3C75
        push   eax
        db  068h, 0C0h, 0A1h, 000h, 000h
        db  0E8h, 032h, 010h, 0FFh, 0FFh
        add    esp, 14h
        mov    eax, 1
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  083h, 0ECh, 008h, 056h, 057h, 0BEh, 0C8h, 0A1h, 000h, 000h, 08Dh, 07Ch, 024h, 008h, 066h, 0A5h
        db  0A4h, 0BEh, 001h, 000h, 000h, 000h, 056h, 0E8h, 003h, 017h, 000h, 000h, 083h, 0C4h, 004h, 083h
        db  0F8h, 001h, 075h, 02Eh, 08Dh, 044h, 024h, 00Ch, 050h, 08Dh, 044h, 024h, 00Ch, 050h, 0E8h, 0CDh
        db  00Eh, 0FFh, 0FFh, 083h, 0C4h, 008h, 00Bh, 0C0h, 074h, 018h, 0FFh, 074h, 024h, 00Ch, 0E8h, 070h
        db  013h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0FFh, 074h, 024h, 00Ch, 0E8h, 080h, 00Eh, 0FFh, 0FFh, 083h
        db  0C4h, 004h, 046h, 0FEh, 044h, 024h, 008h, 083h, 0FEh, 01Ah, 07Eh, 0BAh, 08Dh, 044h, 024h, 00Ch
        db  050h, 068h, 0CCh, 0A1h, 000h, 000h, 0E8h, 095h, 00Eh, 0FFh, 0FFh, 083h, 0C4h, 008h, 00Bh, 0C0h
        db  074h, 018h, 0FFh, 074h, 024h, 00Ch, 0E8h, 038h, 013h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0FFh, 074h
        db  024h, 00Ch, 0E8h, 048h, 00Eh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0B8h, 001h, 000h, 000h, 000h, 05Fh
        db  05Eh, 083h, 0C4h, 008h, 0C3h
sub_1_2506:
        push   ebx
        push   esi
        push   edi
        db  0F6h, 005h, 0D5h, 09Fh, 000h, 000h, 010h
        je     NEAR PTR loc_1_25A7
        push   0
        db  068h, 0D4h, 0A1h, 000h, 000h
        db  0E8h, 04Fh, 00Eh, 0FFh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_25A7
        push   200h
        push   105h
        call   sub_1_4364
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_25A7
        call   sub_1_3C75
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_259E
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, ebx
        lea    eax, [ecx - 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BEh, 0A0h, 0A1h, 000h, 000h
        lea    edi, [ebx + eax]
        movsd
        movsd
        movsb
        push   ebx
        call   sub_1_454F
        add    esp, 4
        cmp    ax, 0ffffh
        je     SHORT loc_1_259E
        test   al, 10h
        je     SHORT loc_1_259E
        push   0
        push   0
        push   9
        push   ebx
        db  068h, 0D8h, 0A1h, 000h, 000h
        db  0E8h, 0FAh, 00Eh, 0FFh, 0FFh
        add    esp, 14h
loc_1_259E:
        push   ebx
        call   sub_1_4390
        add    esp, 4
loc_1_25A7:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_25AB:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        push   211h
        call   sub_1_4364
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_25CE
        sub    eax, eax
        jmp    NEAR PTR loc_1_26A9
loc_1_25CE:
        lea    ebp, [esi + 111h]
        lea    ebx, [esi + 11h]
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_8BD1
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_25ED
        jmp    NEAR PTR loc_1_26A4
loc_1_25ED:
        mov    dword ptr [esp + 14h], 0
        push   esi
        push   0
        push   dword ptr [esp + 24h]
        call   sub_1_8C07
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_2698
loc_1_260C:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   ebp
        push   ebx
        push   esi
        call   sub_1_8911
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_267B
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_2657
        sub    eax, eax
        test   byte ptr [esp + 19h], 2
        je     SHORT loc_1_2648
        mov    eax, 1
loc_1_2648:
        push   ecx
        push   ebp
        push   eax
        push   ebx
        push   esi
        db  0E8h, 043h, 00Eh, 0FFh, 0FFh
        add    esp, 14h
        jmp    SHORT loc_1_266E
loc_1_2657:
        cmp    dword ptr [esp + 10h], 1
        mov    eax, 0ffh
        jne    SHORT loc_1_266E
        push   ebp
        push   ebx
        push   esi
        db  0E8h, 0A9h, 075h, 0FFh, 0FFh
        add    esp, 0ch
loc_1_266E:
        or     eax, eax
        je     SHORT loc_1_267B
        push   esi
        call   sub_1_8B4E
        add    esp, 4
loc_1_267B:
        inc    dword ptr [esp + 14h]
        push   esi
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 24h]
        call   sub_1_8C07
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_1_260C
loc_1_2698:
        push   dword ptr [esp + 1ch]
        call   sub_1_8C45
        add    esp, 4
loc_1_26A4:
        mov    eax, 1
loc_1_26A9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  000h, 000h, 000h
loc_1_26B4:
        push   esi
        db  0B8h, 050h, 0A6h, 000h, 000h
        mov    edx, 30h
        mov    esi, 9
        jmp    SHORT loc_1_270E
loc_1_26C6:
        push   esi
        db  0B8h, 080h, 0AAh, 000h, 000h
        mov    edx, 0ffh
        mov    esi, 9
        jmp    SHORT loc_1_270E
loc_1_26D8:
        push   esi
        db  0B8h, 080h, 0AEh, 000h, 000h
        mov    edx, 1eh
        mov    esi, 7
        jmp    SHORT loc_1_270E
loc_1_26EA:
        push   esi
        db  0B8h, 050h, 0AAh, 000h, 000h
        mov    edx, 0bh
        mov    esi, 9
        jmp    SHORT loc_1_270E
loc_1_26FC:
        push   esi
        db  0B8h, 000h, 0AFh, 000h, 000h
        mov    edx, 5
        mov    esi, 7
        jmp    SHORT loc_1_270E
loc_1_270E:
        mov    ecx, dword ptr [esp + 8]
        movzx  ecx, byte ptr [esi + ecx]
        cmp    edx, ecx
        jb     SHORT loc_1_271E
        pop    esi
        jmp    dword ptr [eax + ecx*4]
loc_1_271E:
        mov    eax, 0fbh
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2730:
        db  0A1h, 0F0h, 0A1h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_2747
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 0F0h, 0A1h, 000h, 000h
        db  0FFh, 00Dh, 0DCh, 0A1h, 000h, 000h
loc_1_2747:
        ret
sub_1_2748:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 0F0h, 0A1h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 0F0h, 0A1h, 000h, 000h
        db  0FFh, 005h, 0DCh, 0A1h, 000h, 000h
        ret
sub_1_2760:
        db  0A1h, 0F4h, 0A1h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_2799
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 0F4h, 0A1h, 000h, 000h
        db  0FFh, 00Dh, 0F8h, 0A1h, 000h, 000h
        db  08Bh, 00Dh, 008h, 0A2h, 000h, 000h
        db  039h, 00Dh, 0F8h, 0A1h, 000h, 000h
        jae    SHORT loc_1_2791
        db  08Bh, 00Dh, 0F8h, 0A1h, 000h, 000h
        db  089h, 00Dh, 008h, 0A2h, 000h, 000h
loc_1_2791:
        db  0FFh, 00Dh, 0E4h, 0A1h, 000h, 000h
        jmp    SHORT loc_1_27A3
loc_1_2799:
        db  0C7h, 005h, 008h, 0A2h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_27A3:
        ret
sub_1_27A4:
        mov    ecx, dword ptr [esp + 4]
        db  0C7h, 041h, 004h, 073h, 01Ah, 000h, 000h
        db  066h, 0A1h, 0E4h, 0AFh, 000h, 000h
        mov    word ptr [ecx + 28h], ax
        db  0A1h, 0F4h, 0A1h, 000h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 0F4h, 0A1h, 000h, 000h
        db  0FFh, 005h, 0F8h, 0A1h, 000h, 000h
        db  0FFh, 005h, 0E4h, 0A1h, 000h, 000h
        ret
sub_1_27D3:
        db  0A1h, 00Ch, 0A2h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_2806
        mov    word ptr [eax + 0ah], 5104h
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 00Ch, 0A2h, 000h, 000h
        db  0FFh, 00Dh, 010h, 0A2h, 000h, 000h
        db  08Bh, 00Dh, 010h, 0A2h, 000h, 000h
        db  039h, 00Dh, 020h, 0A2h, 000h, 000h
        jbe    SHORT loc_1_2810
        db  089h, 00Dh, 020h, 0A2h, 000h, 000h
        jmp    SHORT loc_1_2810
loc_1_2806:
        db  0C7h, 005h, 020h, 0A2h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2810:
        ret
sub_1_2811:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 00Ch, 0A2h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 00Ch, 0A2h, 000h, 000h
        db  0FFh, 005h, 010h, 0A2h, 000h, 000h
        ret
sub_1_2829:
        mov    eax, dword ptr [esp + 4]
        push   0
        add    eax, 34h
        push   eax
        call   sub_1_4364
        add    esp, 8
        ret
sub_1_283C:
        push   esi
        push   110h
        call   sub_1_2829
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_2865
        db  0FFh, 005h, 018h, 0A2h, 000h, 000h
        mov    word ptr [esi + 0ah], 5104h
        push   esi
        call   sub_1_2811
        add    esp, 4
loc_1_2865:
        mov    eax, esi
        pop    esi
        ret
sub_1_2869:
        push   110h
        call   sub_1_2829
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2886
        db  0FFh, 005h, 018h, 0A2h, 000h, 000h
        mov    word ptr [eax + 0ah], 5104h
loc_1_2886:
        ret
sub_1_2887:
        push   esi
        db  00Fh, 0B7h, 005h, 0E4h, 0AFh, 000h, 000h
        push   eax
        call   sub_1_2829
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_28C6
        db  0FFh, 005h, 000h, 0A2h, 000h, 000h
        mov    word ptr [esi + 0ah], 5104h
        mov    word ptr [esi + 22h], 1
        lea    eax, [esi + 34h]
        mov    dword ptr [esi + 24h], eax
        mov    dword ptr [esi + 30h], 0
        push   esi
        call   sub_1_27A4
        add    esp, 4
loc_1_28C6:
        mov    eax, esi
        pop    esi
        ret
sub_1_28CA:
        push   dword ptr [esp + 4]
        call   sub_1_4390
        add    esp, 4
        db  0A1h, 000h, 0A2h, 000h, 000h
        dec    eax
        db  0A3h, 000h, 0A2h, 000h, 000h
        ret
sub_1_28E2:
        push   dword ptr [esp + 4]
        call   sub_1_4390
        add    esp, 4
        db  0A1h, 018h, 0A2h, 000h, 000h
        dec    eax
        db  0A3h, 018h, 0A2h, 000h, 000h
        ret
        db  0A1h, 024h, 0A2h, 000h, 000h, 00Bh, 0C0h, 074h, 00Eh, 08Bh, 008h, 089h, 00Dh, 024h, 0A2h, 000h
        db  000h, 0FFh, 00Dh, 0E8h, 0A1h, 000h, 000h, 0C3h
sub_1_2912:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        db  039h, 03Dh, 000h, 0A2h, 000h, 000h
        jne    SHORT loc_1_293C
        sub    esi, esi
        db  039h, 035h, 0FCh, 0A1h, 000h, 000h
        jbe    SHORT loc_1_293C
loc_1_292A:
        call   sub_1_2887
        or     eax, eax
        je     SHORT loc_1_293C
        inc    esi
        db  039h, 035h, 0FCh, 0A1h, 000h, 000h
        ja     SHORT loc_1_292A
loc_1_293C:
        db  083h, 03Dh, 018h, 0A2h, 000h, 000h, 000h
        jne    SHORT loc_1_2961
        sub    esi, esi
        db  039h, 035h, 014h, 0A2h, 000h, 000h
        jbe    SHORT loc_1_2961
loc_1_294F:
        call   sub_1_283C
        or     eax, eax
        je     SHORT loc_1_2961
        inc    esi
        db  03Bh, 035h, 014h, 0A2h, 000h, 000h
        jb     SHORT loc_1_294F
loc_1_2961:
        mov    esi, 5104h
        mov    ebx, 1
        sub    ebp, ebp
loc_1_296D:
        db  00Fh, 0B7h, 005h, 0E4h, 0AFh, 000h, 000h
        push   eax
        call   sub_1_2829
        add    esp, 4
        cmp    eax, ebp
        je     SHORT loc_1_29B4
        mov    word ptr [eax + 0ah], si
        mov    word ptr [eax + 22h], bx
        lea    ecx, [eax + 34h]
        mov    dword ptr [eax + 24h], ecx
        db  066h, 08Bh, 00Dh, 0E4h, 0AFh, 000h, 000h
        mov    word ptr [eax + 28h], cx
        mov    dword ptr [eax + 30h], ebp
        cmp    edi, 5
        jge    SHORT loc_1_29AE
        mov    dword ptr [eax + 4], ebp
        push   eax
        call   sub_1_2748
        add    esp, 4
loc_1_29AE:
        inc    edi
        cmp    edi, 5
        jl     SHORT loc_1_296D
loc_1_29B4:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_29BE:
        call   sub_1_2730
        or     eax, eax
        je     SHORT loc_1_29D2
        push   eax
        call   sub_1_4390
        add    esp, 4
        jmp    SHORT sub_1_29BE
loc_1_29D2:
        call   sub_1_27D3
        or     eax, eax
        je     SHORT loc_1_29E6
        push   eax
        call   sub_1_28E2
        add    esp, 4
        jmp    SHORT loc_1_29D2
loc_1_29E6:
        call   sub_1_2760
        or     eax, eax
        je     SHORT loc_1_29FA
        push   eax
        call   sub_1_28CA
        add    esp, 4
        jmp    SHORT loc_1_29E6
loc_1_29FA:
        ret
        db  000h, 000h, 000h, 000h, 000h
sub_1_2A00:
        sub    esp, 8
        push   esi
        push   edi
        mov    dword ptr [esp + 0ch], edx
        mov    word ptr [esp + 0ah], cx
        or     cx, cx
        jne    SHORT loc_1_2A17
        sub    al, al
        jmp    SHORT loc_1_2A8D
loc_1_2A17:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_2A4C
        mov    edi, dword ptr [esp + 0ch]
        movzx  edx, byte ptr [edi]
        inc    edi
        or     edx, edx
        je     SHORT loc_1_2A4C
        mov    ax, word ptr [esp + 0ah]
        xor    ecx, ecx
        lea    esi, [edi + edx*2]
        dec    ecx
        mov    word ptr [esi - 2], ax
        repne scasw
        neg    ecx
        mov    al, byte ptr [esi + ecx - 2]
        or     al, al
        je     SHORT loc_1_2A4C
        mov    byte ptr [esp + 9], al
        jmp    SHORT loc_1_2A61
loc_1_2A4C:
        cmp    word ptr [esp + 0ah], 28h
        movzx  eax, word ptr [esp + 0ah]
        jae    SHORT loc_1_2A67
        db  08Ah, 080h, 028h, 0A2h, 000h, 000h
        jmp    SHORT loc_1_2A8D
loc_1_2A61:
        mov    al, byte ptr [esp + 9]
        jmp    SHORT loc_1_2A8D
loc_1_2A67:
        cmp    eax, 50h
        je     SHORT loc_1_2A7F
        cmp    eax, 70h
        je     SHORT loc_1_2A83
        cmp    eax, 71h
        je     SHORT loc_1_2A87
        cmp    eax, 7bh
        je     SHORT loc_1_2A8B
        mov    al, 0ffh
        jmp    SHORT loc_1_2A8D
loc_1_2A7F:
        mov    al, 92h
        jmp    SHORT loc_1_2A8D
loc_1_2A83:
        mov    al, 1
        jmp    SHORT loc_1_2A8D
loc_1_2A87:
        mov    al, 81h
        jmp    SHORT loc_1_2A8D
loc_1_2A8B:
        mov    al, 0efh
loc_1_2A8D:
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2AA0:
        push   ebp
        push   ebx
        push   esi
        push   edi
        call   sub_1_6E08
        mov    ebp, dword ptr [esp + 14h]
        lea    edi, [eax + 28h]
        mov    dword ptr [eax + 10h], edi
        call   sub_1_6EBC
        jb     SHORT loc_1_2B05
        mov    bx, word ptr [esp + 1ch]
        mov    cx, word ptr [esp + 18h]
        mov    dx, word ptr [esp + 20h]
        or     bh, 20h
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0ch], edx
        mov    dword ptr [eax + 4], ebx
        mov    word ptr [eax], 716ch
        call   sub_1_6E48
        jb     SHORT loc_1_2B05
        mov    ecx, dword ptr [esp + 24h]
        movzx  ebx, word ptr [ebp + 18h]
        cmp    bx, 3
        ja     SHORT loc_1_2AF4
        test   bx, bx
        jne    SHORT loc_1_2AF8
loc_1_2AF4:
        mov    bx, 1
loc_1_2AF8:
        mov    word ptr [ecx], bx
        call   sub_1_6EB8
loc_1_2B00:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2B05:
        call   sub_1_6EB8
        mov    ebx, dword ptr [esp + 28h]
        mov    word ptr [ebx], ax
        xor    eax, eax
        jmp    SHORT loc_1_2B00
sub_1_2B15:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        xor    ebx, ebx
        mov    ecx, 2
        call   sub_1_2D68
        mov    ebx, dword ptr [esp + 18h]
        jb     SHORT loc_1_2B39
        mov    word ptr [ebx], 0
loc_1_2B34:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2B39:
        mov    word ptr [ebx], ax
        jmp    SHORT loc_1_2B34
sub_1_2B3E:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  eax, word ptr [esp + 14h]
        movzx  ebx, word ptr [esp + 20h]
        mov    ecx, ebx
        mov    edx, dword ptr [esp + 1ch]
        mov    esi, dword ptr [esp + 18h]
        call   sub_1_2CB4
        mov    ebx, dword ptr [esp + 24h]
        jb     SHORT loc_1_2B73
        mov    eax, edx
        db  001h, 015h, 078h, 0B1h, 000h, 000h
        mov    word ptr [ebx], 0
loc_1_2B6E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2B73:
        mov    word ptr [ebx], ax
        jmp    SHORT loc_1_2B6E
sub_1_2B78:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  eax, word ptr [esp + 14h]
        movzx  ebx, word ptr [esp + 20h]
        mov    ecx, ebx
        mov    edx, dword ptr [esp + 1ch]
        mov    esi, dword ptr [esp + 18h]
        call   sub_1_2CAC
        mov    ebx, dword ptr [esp + 24h]
        jb     SHORT loc_1_2BAD
        mov    eax, edx
        db  001h, 015h, 07Ch, 0B1h, 000h, 000h
        mov    word ptr [ebx], 0
loc_1_2BA8:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2BAD:
        mov    word ptr [ebx], ax
        jmp    SHORT loc_1_2BA8
sub_1_2BB2:
        push   ebp
        push   ebx
        push   esi
        push   edi
        xor    edx, edx
        movzx  eax, word ptr [esp + 14h]
        mov    dl, byte ptr [esp + 20h]
        mov    ebx, dword ptr [esp + 18h]
        mov    dh, 0bch
        mov    ecx, dword ptr [esp + 1ch]
        call   sub_1_2EB0
        jb     SHORT loc_1_2BD4
        xor    eax, eax
loc_1_2BD4:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_2BD9:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  eax, word ptr [esp + 14h]
        mov    ebx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    edx, 0bd00h
        call   sub_1_2EB0
        jb     SHORT loc_1_2BF8
        xor    eax, eax
loc_1_2BF8:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_2BFD:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    ecx, 0ffffffffh
        movzx  eax, word ptr [esp + 14h]
        call   sub_1_2C70
        jb     SHORT loc_1_2C14
        xor    eax, eax
loc_1_2C14:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_2C19:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  eax, word ptr [esp + 14h]
        call   sub_1_2D10
        jb     SHORT loc_1_2C2B
        xor    eax, eax
loc_1_2C2B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_2C30:
        push   ebp
        push   ebx
        push   esi
        push   edi
        call   sub_1_6E08
        movzx  edx, word ptr [esp + 14h]
        mov    dword ptr [eax + 4], edx
        mov    ebx, dword ptr [esp + 18h]
        movzx  ecx, word ptr [ebx]
        xchg   cl, ch
        mov    dword ptr [eax + 8], ecx
        mov    cx, word ptr [ebx + 2]
        xchg   cl, ch
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax], 5701h
        call   sub_1_6E48
        call   sub_1_6EB8
        jb     SHORT loc_1_2C6A
        xor    eax, eax
loc_1_2C6A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h
sub_1_2C70:
        xchg   ebx, eax
        call   sub_1_6E08
        mov    dword ptr [eax + 4], ebx
        cmp    ecx, -1
        je     SHORT loc_1_2C91
        push   eax
        xchg   ecx, eax
        pop    eax
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0ch], edx
        mov    word ptr [eax], 5701h
        call   sub_1_6E48
loc_1_2C91:
        mov    word ptr [eax], 3e00h
        call   sub_1_6E48
        call   sub_1_6EB8
        ret
        db  02Eh, 08Bh, 0C0h, 033h, 0DBh, 033h, 0C9h, 033h, 0F6h, 033h, 0EDh
sub_1_2CAC:
        push   4000h
        jmp    SHORT loc_1_2CB9
        db  090h
sub_1_2CB4:
        push   3f00h
loc_1_2CB9:
        push   ebx
        push   esi
        xchg   ebx, eax
        call   sub_1_6E08
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 0ch], edx
        shr    edx, 10h
        mov    dword ptr [eax + 8], edx
        mov    word ptr [eax], 4200h
        call   sub_1_6E48
        jb     SHORT loc_1_2D08
        pop    ebx
        mov    dword ptr [eax + 0ch], ebx
        pop    esi
        mov    dword ptr [eax + 8], esi
        pop    ecx
        mov    dword ptr [eax], ecx
        call   sub_1_6E48
        jb     SHORT loc_1_2CF5
        movzx  edx, word ptr [eax]
        mov    ebx, edx
        cmp    ebx, esi
        jne    SHORT loc_1_2CFB
loc_1_2CF4:
        clc
loc_1_2CF5:
        call   sub_1_6EB8
        ret
loc_1_2CFB:
        cmp    ch, 40h
        jne    SHORT loc_1_2CF4
        mov    word ptr [eax], 70h
loc_1_2D05:
        stc
        jmp    SHORT loc_1_2CF5
loc_1_2D08:
        add    esp, 0ch
        xor    ebx, ebx
        jmp    SHORT loc_1_2D05
        db  090h
sub_1_2D10:
        xchg   ebx, eax
        call   sub_1_6E08
        mov    dword ptr [eax + 4], ebx
        mov    word ptr [eax], 6800h
        call   sub_1_6E48
        call   sub_1_6EB8
        ret
        db  02Eh, 08Bh, 0C0h, 08Bh, 01Dh, 098h, 0B1h, 000h, 000h, 08Bh, 06Bh, 008h, 066h, 0C7h, 045h, 01Ch
        db  001h, 05Dh, 0CDh, 020h, 082h, 000h, 001h, 000h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h
        db  000h, 001h, 000h, 066h, 0C7h, 045h, 01Ch, 000h, 00Dh, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h
        db  084h, 000h, 001h, 000h, 0CDh, 020h, 086h, 000h, 001h, 000h, 0FCh, 0F8h, 0C3h, 08Bh, 0C0h
sub_1_2D68:
        xchg   edx, eax
        call   sub_1_6E08
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 0ch], ebx
        shr    ebx, 10h
        mov    dword ptr [eax + 8], ebx
        mov    ch, 42h
        mov    dword ptr [eax], ecx
        call   sub_1_6E48
        jb     SHORT loc_1_2D98
        mov    dx, word ptr [ebp + 14h]
        shl    edx, 10h
        mov    dx, word ptr [ebp + 1ch]
        call   sub_1_6EB8
        xchg   edx, eax
        clc
        ret
loc_1_2D98:
        call   sub_1_6EB8
        ret
        db  08Bh, 0C0h, 092h, 0E8h, 062h, 040h, 000h, 000h, 089h, 050h, 004h, 057h, 08Dh, 078h, 028h, 089h
        db  078h, 00Ch, 0E8h, 007h, 041h, 000h, 000h, 05Fh, 072h, 068h, 066h, 0C7h, 000h, 000h, 043h, 0E8h
        db  086h, 040h, 000h, 000h, 072h, 05Ch, 08Bh, 04Dh, 018h, 066h, 089h, 04Fh, 014h, 066h, 0C7h, 000h
        db  000h, 057h, 0E8h, 073h, 040h, 000h, 000h, 072h, 049h, 08Bh, 05Dh, 014h, 066h, 089h, 05Fh, 008h
        db  08Bh, 05Dh, 018h, 066h, 089h, 05Fh, 00Ah, 033h, 0DBh, 089h, 01Fh, 089h, 05Fh, 004h, 066h, 0C7h
        db  000h, 002h, 042h, 066h, 0C7h, 040h, 008h, 000h, 000h, 066h, 0C7h, 040h, 00Ch, 000h, 000h, 0E8h
        db  046h, 040h, 000h, 000h, 072h, 01Ch, 08Bh, 05Dh, 014h, 0C1h, 0E3h, 010h, 066h, 08Bh, 05Dh, 01Ch
        db  089h, 05Fh, 00Ch, 081h, 0C3h, 0FFh, 007h, 000h, 000h, 081h, 0E3h, 000h, 0F8h, 0FFh, 0FFh, 089h
        db  05Fh, 010h, 0E8h, 093h, 040h, 000h, 000h, 0C3h, 08Bh, 0C0h, 092h, 0E8h, 0DAh, 03Fh, 000h, 000h
        db  089h, 050h, 004h, 066h, 08Bh, 05Eh, 008h, 089h, 058h, 00Ch, 066h, 08Bh, 05Eh, 00Ah, 089h, 058h
        db  008h, 066h, 0C7h, 000h, 001h, 057h, 0E8h, 0FFh, 03Fh, 000h, 000h, 0E8h, 06Ah, 040h, 000h, 000h
        db  0C3h, 090h, 08Bh, 01Dh, 098h, 0B1h, 000h, 000h, 08Bh, 06Bh, 008h, 08Dh, 050h, 001h, 066h, 089h
        db  055h, 014h, 066h, 0C7h, 045h, 01Ch, 000h, 036h, 0CDh, 020h, 082h, 000h, 001h, 000h, 0B8h, 021h
        db  000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 0CDh, 020h, 086h, 000h, 001h, 000h, 0FCh
        db  066h, 083h, 07Dh, 01Ch, 0FFh, 074h, 012h, 00Fh, 0B7h, 04Dh, 014h, 00Fh, 0B7h, 055h, 010h, 00Fh
        db  0B7h, 075h, 01Ch, 00Fh, 0B7h, 07Dh, 018h, 0F8h, 0C3h, 0B0h, 00Fh, 0F9h, 0C3h, 090h, 0B0h, 032h
        db  0F9h, 0C3h, 068h, 001h, 05Ch, 000h, 000h, 0EBh, 00Ah, 090h, 068h, 000h, 05Ch, 000h, 000h, 0EBh
        db  002h, 090h
sub_1_2EB0:
        push   edx
        xchg   edx, eax
        call   sub_1_6E08
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 0ch], ebx
        shr    ebx, 10h
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 14h], ecx
        shr    ecx, 10h
        mov    dword ptr [eax + 10h], ecx
        pop    ebx
        mov    dword ptr [eax], ebx
        call   sub_1_6E48
        call   sub_1_6EB8
        ret
        db  08Bh, 0C0h, 0F8h, 0C3h, 0F9h, 0C3h, 056h, 0E8h, 022h, 03Fh, 000h, 000h, 08Dh, 078h, 028h, 089h
        db  078h, 00Ch, 0E8h, 0CBh, 03Fh, 000h, 000h, 072h, 025h, 057h, 066h, 0C7h, 040h, 008h, 000h, 000h
        db  066h, 0C7h, 000h, 000h, 05Ah, 0E8h, 044h, 03Fh, 000h, 000h, 05Eh, 05Fh, 072h, 00Ah, 080h, 03Eh
        db  05Ch, 075h, 001h, 046h, 0A5h, 0A5h, 0A5h, 0A4h, 0E8h, 0A1h, 03Fh, 000h, 000h, 0C3h, 05Eh, 0EBh
        db  0F7h
sub_1_2F1B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_4308
        pop    ecx
        mov    ecx, eax
        inc    ecx
        cmp    ecx, 20ah
        jb     SHORT loc_1_2F3A
        mov    ecx, 20ah
loc_1_2F3A:
        call   sub_1_6E08
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [eax + 28h]
        mov    dword ptr [eax + 10h], edi
        rep movsb
        mov    byte ptr [eax + 231h], 0
        mov    word ptr [eax], 6c00h
        mov    word ptr [eax + 4], 22h
        mov    word ptr [eax + 8], 0
        mov    word ptr [eax + 0ch], 12h
        push   ebp
        call   sub_1_6E48
        pop    ebp
        call   sub_1_6EB8
        jb     SHORT loc_1_2F7E
        mov    esi, dword ptr [ebp + 0ch]
        mov    word ptr [esi], ax
        xor    eax, eax
loc_1_2F7E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2F90:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        push   0
        lea    eax, [edi*4 + 8]
        push   eax
        call   sub_1_4364
        add    esp, 8
        mov    dword ptr [esi], eax
        or     eax, eax
        jne    SHORT loc_1_2FB2
        sub    eax, eax
        jmp    SHORT loc_1_3006
loc_1_2FB2:
        mov    word ptr [eax], di
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [ecx + 2], ax
        mov    edx, 1
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [esi]
        mov    word ptr [eax + 6], 1
        mov    eax, dword ptr [esi]
        cmp    word ptr [eax], dx
        jbe    SHORT loc_1_2FF1
loc_1_2FDD:
        mov    eax, dword ptr [esi]
        lea    ecx, [edx + 1]
        mov    dword ptr [eax + edx*4 + 4], ecx
        mov    eax, dword ptr [esi]
        mov    edx, ecx
        movzx  eax, word ptr [eax]
        cmp    eax, ecx
        ja     SHORT loc_1_2FDD
loc_1_2FF1:
        mov    eax, dword ptr [esi]
        movzx  ecx, word ptr [eax]
        mov    dword ptr [eax + ecx*4 + 4], 0
        add    dword ptr [esi], 4
        mov    eax, 1
loc_1_3006:
        pop    edi
        pop    esi
        ret    8
sub_1_300B:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        mov    dword ptr [esp + 10h], edx
        mov    eax, dword ptr [esi]
        lea    edi, [eax - 4]
        cmp    word ptr [eax + 2], 0
        jne    NEAR PTR loc_1_30DA
loc_1_3028:
        mov    ecx, dword ptr [esi]
        mov    ax, word ptr [ecx - 4]
        cmp    word ptr [ecx], ax
        jb     NEAR PTR loc_1_30F8
        movzx  ecx, word ptr [ecx - 2]
        movzx  eax, ax
        add    ecx, eax
        lea    ebp, [ecx*4 + 8]
        push   0
        push   ebp
        call   sub_1_4364
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_30FC
        mov    eax, dword ptr [esi]
        mov    ecx, eax
        sub    ecx, edi
        cmp    ecx, 4
        je     SHORT loc_1_306A
        push   ebx
        jmp    SHORT loc_1_30C2
loc_1_306A:
        push   ebp
        sub    eax, 4
        push   eax
        push   ebx
        call   sub_1_4288
        add    esp, 0ch
        movzx  ecx, word ptr [ebx + 2]
        mov    ax, word ptr [ebx]
        inc    ax
        mov    word ptr [ebx + 6], ax
        movzx  eax, word ptr [ebx]
        add    ecx, eax
        lea    edx, [eax + 1]
        cmp    ecx, edx
        jbe    SHORT loc_1_30A7
loc_1_3091:
        lea    eax, [edx + 1]
        mov    dword ptr [ebx + edx*4 + 4], eax
        mov    edx, eax
        movzx  ecx, word ptr [ebx + 2]
        movzx  ebp, word ptr [ebx]
        add    ecx, ebp
        cmp    ecx, eax
        ja     SHORT loc_1_3091
loc_1_30A7:
        mov    ax, word ptr [ebx + 2]
        add    ax, word ptr [ebx]
        mov    word ptr [ebx], ax
        movzx  eax, ax
        mov    dword ptr [ebx + eax*4 + 4], 0
        add    ebx, 4
        mov    dword ptr [esi], ebx
        push   edi
loc_1_30C2:
        call   sub_1_4390
        add    esp, 4
        mov    eax, dword ptr [esi]
        lea    edi, [eax - 4]
        cmp    word ptr [eax + 2], 0
        je     NEAR PTR loc_1_3028
loc_1_30DA:
        mov    ecx, dword ptr [esi]
        movzx  eax, word ptr [ecx + 2]
        mov    edx, dword ptr [ecx + eax*4]
        mov    word ptr [ecx + 2], dx
        mov    edx, dword ptr [esi]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [edx + eax*4], ecx
loc_1_30F0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_30F8:
        sub    eax, eax
        jmp    SHORT loc_1_30F0
loc_1_30FC:
        sub    eax, eax
        jmp    SHORT loc_1_30F0
loc_1_3100:
        push   0
        push   esi
        push   ecx
        push   edi
        movzx  eax, ax
        push   eax
        call   edx
        add    esp, 10h
        pop    ebx
        mov    esi, eax
        ret
sub_1_3112:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, 0eh
        mov    edx, dword ptr [ebp + 8]
        db  0BEh, 000h, 031h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        mov    ebx, dword ptr [ebp + 0ch]
        or     ebx, ebx
        je     SHORT loc_1_3137
        mov    word ptr [ebx], dx
loc_1_3137:
        pop    esi
        pop    ebx
        leave
        ret
sub_1_313B:
        push   ebp
        mov    ebp, esp
        push   ebx
        xor    ebx, ebx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        mov    ebx, dword ptr [ebp + 10h]
        or     ebx, ebx
        je     SHORT loc_1_3157
        mov    word ptr [ebx], dx
loc_1_3157:
        pop    ebx
        leave
        ret
sub_1_315A:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, 1
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    ebx
        leave
        ret
sub_1_316F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, 2
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_318D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    edx, edx
        mov    ebx, 3
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        db  0FFh, 005h, 0E0h, 0A1h, 000h, 000h
        pop    esi
        pop    ebx
        leave
        ret
sub_1_31AC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ecx, ecx
        mov    ebx, 4
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        or     eax, eax
        jne    SHORT loc_1_31CB
        db  0FFh, 005h, 0ECh, 0A1h, 000h, 000h
loc_1_31CB:
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, 6
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    esi
        pop    ebx
        leave
        ret
sub_1_31E6:
        push   ebx
        mov    ebx, 0ah
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    ebx
        ret
sub_1_31F4:
        push   ebx
        push   esi
        xor    edx, edx
        mov    ebx, 9
        db  0BEh, 004h, 0B4h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    esi
        pop    ebx
        ret
sub_1_320B:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        ret
sub_1_3212:
        push   ebx
        xor    edx, edx
        mov    ebx, 1ah
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    ebx
        ret
sub_1_3222:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, 5
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 094h, 004h
        pop    esi
        leave
        ret
sub_1_3237:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, 3
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [ebp + 14h]
        db  0CDh, 020h, 001h, 000h, 094h, 004h
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3257:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, 2
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 001h, 000h, 094h, 004h
        pop    esi
        leave
        ret
sub_1_326F:
        mov    eax, 6
        db  0CDh, 020h, 001h, 000h, 094h, 004h
        ret
sub_1_327B:
        push   ebp
        mov    ebp, esp
        mov    eax, 7
        mov    ecx, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 094h, 004h
        leave
        ret
sub_1_328E:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, 101h
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 005h, 000h, 087h, 004h
        pop    esi
        leave
        ret
sub_1_32A3:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, 102h
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 005h, 000h, 087h, 004h
        pop    esi
        leave
        ret
NWSERVER_Service_02:
        call   sub_1_0CDD
        ret
NWSERVER_Service_03:
        call   sub_1_31F4
        db  080h, 03Dh, 0ACh, 0B1h, 000h, 000h, 000h
        je     SHORT loc_1_32FF
        db  083h, 03Dh, 0ECh, 0B3h, 000h, 000h, 000h
        je     SHORT loc_1_32FF
        db  0F7h, 005h, 0D4h, 09Fh, 000h, 000h, 000h, 010h, 000h, 000h
        jne    SHORT loc_1_32EB
        call   sub_1_10B0
        call   sub_1_0F7D
loc_1_32EB:
        db  0F7h, 005h, 0D4h, 09Fh, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_32FF
        push   1
        call   sub_1_0DBE
        pop    eax
loc_1_32FF:
        ret
sub_1_3300:
        db  083h, 03Dh, 054h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_1_331E
        db  0A1h, 054h, 0AFh, 000h, 000h
        db  0FFh, 005h, 04Ch, 0AFh, 000h, 000h
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 054h, 0AFh, 000h, 000h
        jmp    SHORT loc_1_3351
loc_1_331E:
        push   0
        push   104h
        call   sub_1_4364
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_3358
        db  0FFh, 005h, 048h, 0AFh, 000h, 000h
        db  0FFh, 005h, 04Ch, 0AFh, 000h, 000h
        db  08Bh, 00Dh, 04Ch, 0AFh, 000h, 000h
        db  039h, 00Dh, 050h, 0AFh, 000h, 000h
        jae    SHORT loc_1_3351
        db  089h, 00Dh, 050h, 0AFh, 000h, 000h
loc_1_3351:
        or     eax, eax
        je     SHORT loc_1_3358
        mov    byte ptr [eax], 0
loc_1_3358:
        ret
sub_1_3359:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 054h, 0AFh, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 054h, 0AFh, 000h, 000h
        db  0FFh, 00Dh, 04Ch, 0AFh, 000h, 000h
        ret
sub_1_3371:
        db  083h, 03Dh, 064h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_1_338F
        db  0A1h, 064h, 0AFh, 000h, 000h
        db  0FFh, 005h, 05Ch, 0AFh, 000h, 000h
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 064h, 0AFh, 000h, 000h
        jmp    SHORT loc_1_33C8
loc_1_338F:
        push   0
        push   250h
        call   sub_1_4364
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_33D5
        db  0FFh, 005h, 058h, 0AFh, 000h, 000h
        db  0FFh, 005h, 05Ch, 0AFh, 000h, 000h
        db  08Bh, 00Dh, 060h, 0AFh, 000h, 000h
        db  039h, 00Dh, 05Ch, 0AFh, 000h, 000h
        jbe    SHORT loc_1_33C8
        db  08Bh, 00Dh, 05Ch, 0AFh, 000h, 000h
        db  089h, 00Dh, 060h, 0AFh, 000h, 000h
loc_1_33C8:
        or     eax, eax
        je     SHORT loc_1_33D5
        mov    word ptr [eax + 234h], 0
loc_1_33D5:
        ret
sub_1_33D6:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 064h, 0AFh, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 064h, 0AFh, 000h, 000h
        db  0FFh, 00Dh, 05Ch, 0AFh, 000h, 000h
        ret
sub_1_33EE:
        push   esi
        db  083h, 03Dh, 054h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_1_342B
        sub    esi, esi
loc_1_33FA:
        db  0A1h, 048h, 0AFh, 000h, 000h
        db  039h, 005h, 050h, 0AFh, 000h, 000h
        jae    SHORT loc_1_342B
        db  0A1h, 054h, 0AFh, 000h, 000h
        push   eax
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 054h, 0AFh, 000h, 000h
        call   sub_1_4390
        add    esp, 4
        db  0FFh, 00Dh, 048h, 0AFh, 000h, 000h
        db  039h, 035h, 054h, 0AFh, 000h, 000h
        jne    SHORT loc_1_33FA
loc_1_342B:
        db  0C7h, 005h, 050h, 0AFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 064h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_1_3471
        sub    esi, esi
loc_1_3440:
        db  0A1h, 060h, 0AFh, 000h, 000h
        db  039h, 005h, 058h, 0AFh, 000h, 000h
        jbe    SHORT loc_1_3471
        db  0A1h, 064h, 0AFh, 000h, 000h
        push   eax
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 064h, 0AFh, 000h, 000h
        call   sub_1_4390
        add    esp, 4
        db  0FFh, 00Dh, 058h, 0AFh, 000h, 000h
        db  039h, 035h, 064h, 0AFh, 000h, 000h
        jne    SHORT loc_1_3440
loc_1_3471:
        db  0C7h, 005h, 060h, 0AFh, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
        ret
        db  000h, 000h, 000h
sub_1_3480:
        call   sub_1_3212
        db  066h, 0A3h, 0E4h, 0AFh, 000h, 000h
        lea    ecx, [eax - 32h]
        db  066h, 089h, 00Dh, 0ECh, 0AFh, 000h, 000h
        lea    ecx, [eax - 42h]
        db  066h, 089h, 00Dh, 0E8h, 0AFh, 000h, 000h
        push   0
        movzx  eax, ax
        push   eax
        call   sub_1_4364
        add    esp, 8
        db  0A3h, 0F0h, 0AFh, 000h, 000h
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_1_3516
        db  068h, 0F4h, 0AFh, 000h, 000h
        push   0ffh
        push   0
        call   sub_1_313B
        db  0C7h, 005h, 0FCh, 0AFh, 000h, 000h, 036h, 035h, 000h, 000h
        db  066h, 0A1h, 0F4h, 0AFh, 000h, 000h
        db  066h, 0A3h, 002h, 0B0h, 000h, 000h
        db  066h, 0C7h, 005h, 01Ah, 0B0h, 000h, 000h, 001h, 000h
        add    esp, 0ch
        db  0A1h, 0F0h, 0AFh, 000h, 000h
        db  0A3h, 01Ch, 0B0h, 000h, 000h
        db  066h, 0A1h, 0E4h, 0AFh, 000h, 000h
        db  066h, 0A3h, 020h, 0B0h, 000h, 000h
        db  068h, 0F8h, 0AFh, 000h, 000h
        call   sub_1_31AC
        add    esp, 4
        mov    eax, 1
loc_1_3516:
        ret
sub_1_3517:
        db  00Fh, 0B7h, 005h, 0F4h, 0AFh, 000h, 000h
        push   eax
        call   sub_1_315A
        add    esp, 4
        db  0FFh, 035h, 0F0h, 0AFh, 000h, 000h
        call   sub_1_4390
        add    esp, 4
        ret
loc_1_3536:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        mov    cl, byte ptr [ebx + 9]
        movsx  ax, cl
        cmp    ax, 0ffh
        je     SHORT loc_1_3583
        or     cl, cl
        jne    SHORT loc_1_357A
        mov    eax, dword ptr [ebx + 24h]
        lea    esi, [eax + 12h]
        lea    edi, [eax + 6]
        movsd
        movsd
        movsd
        mov    ax, word ptr [eax + 2]
        push   eax
        call   sub_1_3A60
        add    esp, 4
        mov    word ptr [ebx + 28h], ax
        db  0C7h, 043h, 004h, 087h, 035h, 000h, 000h
        push   ebx
        call   sub_1_318D
        jmp    SHORT loc_1_3580
loc_1_357A:
        push   ebx
        call   sub_1_3587
loc_1_3580:
        add    esp, 4
loc_1_3583:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_3587:
        mov    eax, dword ptr [esp + 4]
        db  066h, 08Bh, 00Dh, 0E4h, 0AFh, 000h, 000h
        mov    word ptr [eax + 28h], cx
        db  0C7h, 040h, 004h, 036h, 035h, 000h, 000h
        push   eax
        call   sub_1_31AC
        add    esp, 4
        ret
loc_1_35A7:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ax, word ptr [esi + 7]
        push   eax
        call   sub_1_3A60
        add    esp, 4
        db  066h, 03Bh, 005h, 0E4h, 0AFh, 000h, 000h
        jbe    SHORT loc_1_35DB
        db  066h, 0A1h, 0E4h, 0AFh, 000h, 000h
        push   eax
        call   sub_1_3A60
        mov    edx, dword ptr [esp + 10h]
        add    esp, 4
        mov    word ptr [edx + 8], ax
        jmp    SHORT loc_1_35EF
loc_1_35DB:
        mov    edx, dword ptr [esp + 0ch]
        mov    cx, word ptr [esi + 7]
        mov    word ptr [edx + 8], cx
        mov    ecx, dword ptr [esp + 14h]
        mov    word ptr [ecx + 26h], ax
loc_1_35EF:
        db  066h, 0A1h, 0F4h, 0AFh, 000h, 000h
        mov    word ptr [edx + 0ah], ax
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [edx + 0ch], 0
        mov    word ptr [eax], 5
        sub    ax, ax
        pop    esi
        ret
loc_1_360B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ax, word ptr [esi + 7]
        push   eax
        call   sub_1_3A60
        add    esp, 4
        db  066h, 039h, 005h, 0ECh, 0AFh, 000h, 000h
        jae    SHORT loc_1_363F
        db  066h, 0A1h, 0ECh, 0AFh, 000h, 000h
        push   eax
        call   sub_1_3A60
        mov    ecx, dword ptr [esp + 10h]
        add    esp, 4
        mov    word ptr [ecx + 8], ax
        jmp    SHORT loc_1_365C
loc_1_363F:
        mov    ecx, dword ptr [esp + 0ch]
        mov    ax, word ptr [esi + 7]
        mov    word ptr [ecx + 8], ax
        push   eax
        call   sub_1_3A60
        mov    ecx, dword ptr [esp + 18h]
        add    esp, 4
        mov    word ptr [ecx + 26h], ax
loc_1_365C:
        mov    eax, dword ptr [esp + 10h]
        pop    esi
        mov    word ptr [eax], 2
        sub    ax, ax
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_3670:
        db  0FFh, 035h, 02Ch, 0B0h, 000h, 000h
        call   sub_1_92FB
        add    esp, 4
        db  0FFh, 035h, 030h, 0B0h, 000h, 000h
        call   sub_1_21B5
        add    esp, 4
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        db  0A1h, 060h, 0B9h, 000h, 000h
        mov    dword ptr [ecx], eax
        db  0A1h, 094h, 0B8h, 000h, 000h
        db  068h, 058h, 0B4h, 000h, 000h
        mov    word ptr [eax + 491h], 0
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    byte ptr [eax + 493h], 0
        call   sub_1_943C
        db  0A1h, 060h, 0B9h, 000h, 000h
        add    esp, 4
        push   dword ptr [eax + 10h]
        call   sub_1_92FB
        add    esp, 4
        ret
sub_1_36D1:
        db  0A1h, 060h, 0B9h, 000h, 000h
        push   dword ptr [eax + 10h]
        call   sub_1_92F1
        add    esp, 4
        db  068h, 058h, 0B4h, 000h, 000h
        call   sub_1_9490
        add    esp, 4
        call   sub_1_21D8
        db  0FFh, 035h, 02Ch, 0B0h, 000h, 000h
        call   sub_1_92F1
        add    esp, 4
        ret
sub_1_3702:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    edx, dword ptr [eax]
        and    word ptr [edx + 1ah], 0fffch
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    word ptr [eax + 491h], 0
        db  0A1h, 054h, 0B9h, 000h, 000h
        mov    word ptr [eax], 2222h
        db  0A1h, 054h, 0B9h, 000h, 000h
        mov    byte ptr [eax + 2], 1
        db  0A1h, 054h, 0B9h, 000h, 000h
        mov    byte ptr [eax + 3], 0ffh
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    cl, byte ptr [eax + 491h]
        db  0A1h, 054h, 0B9h, 000h, 000h
        mov    byte ptr [eax + 4], cl
        db  0A1h, 054h, 0B9h, 000h, 000h
        mov    byte ptr [eax + 5], 0ffh
        db  0A1h, 054h, 0B9h, 000h, 000h
        mov    bl, byte ptr [esp + 14h]
        mov    byte ptr [eax + 6], bl
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, eax
        db  08Bh, 03Dh, 054h, 0B9h, 000h, 000h
        add    edi, 7
        mov    esi, dword ptr [esp + 18h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    word ptr [esp + 0eh], 0
        lea    eax, [esp + 0eh]
        push   edx
        push   eax
        movzx  eax, bl
        db  0FFh, 035h, 054h, 0B9h, 000h, 000h
        db  0FFh, 035h, 054h, 0B9h, 000h, 000h
        db  0FFh, 014h, 085h, 050h, 0A2h, 000h, 000h
        movzx  edx, word ptr [esp + 1eh]
        add    esp, 10h
        db  08Bh, 035h, 054h, 0B9h, 000h, 000h
        mov    edi, dword ptr [esp + 20h]
        add    esi, 8
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_37D3:
        sub    esp, 4
        push   esi
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax]
        push   eax
        and    word ptr [eax + 1ah], 0fffch
        lea    eax, [esp + 8]
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        push   eax
        mov    word ptr [ecx + 491h], 0
        mov    esi, dword ptr [esp + 18h]
        mov    dword ptr [esp + 0ch], 0
        lea    eax, [esi + 8]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   eax
        movzx  eax, word ptr [eax + 4]
        db  0FFh, 014h, 085h, 018h, 0AFh, 000h, 000h
        add    esp, 10h
        mov    dword ptr [esi + 4], eax
        mov    ecx, dword ptr [esp + 4]
        add    ecx, 8
        mov    dword ptr [esi], ecx
        sub    ax, ax
        pop    esi
        add    esp, 4
        ret
sub_1_3830:
        push   esi
        push   edi
        db  083h, 03Dh, 030h, 0B0h, 000h, 000h, 000h
        je     SHORT loc_1_3840
        jmp    NEAR PTR loc_1_39C6
loc_1_3840:
        push   1
        call   sub_1_92D8
        add    esp, 4
        db  0A3h, 02Ch, 0B0h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_385A
        sub    eax, eax
        jmp    NEAR PTR loc_1_39CB
loc_1_385A:
        push   0
        push   495h
        call   sub_1_218B
        add    esp, 8
        db  0A3h, 030h, 0B0h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_3879
        sub    eax, eax
        jmp    NEAR PTR loc_1_39CB
loc_1_3879:
        push   1
        push   0a4h
        call   sub_1_4364
        add    esp, 8
        db  0A3h, 060h, 0B9h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_3898
        sub    eax, eax
        jmp    NEAR PTR loc_1_39CB
loc_1_3898:
        push   0
        push   400h
        call   sub_1_4364
        add    esp, 8
        db  0A3h, 054h, 0B9h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_38B7
        sub    eax, eax
        jmp    NEAR PTR loc_1_39CB
loc_1_38B7:
        push   1
        call   sub_1_92D8
        db  08Bh, 00Dh, 060h, 0B9h, 000h, 000h
        add    esp, 4
        mov    dword ptr [ecx + 10h], eax
        db  0A1h, 060h, 0B9h, 000h, 000h
        cmp    dword ptr [eax + 10h], 0
        jne    SHORT loc_1_38DC
        sub    eax, eax
        jmp    NEAR PTR loc_1_39CB
loc_1_38DC:
        push   3e8h
        push   10h
        db  08Bh, 00Dh, 060h, 0B9h, 000h, 000h
        mov    edx, 10h
        add    ecx, 44h
        call   sub_1_2F90
        or     eax, eax
        jne    SHORT loc_1_3901
        sub    eax, eax
        jmp    NEAR PTR loc_1_39CB
loc_1_3901:
        db  0FFh, 035h, 060h, 0B9h, 000h, 000h
        db  0E8h, 0C7h, 00Ah, 0FFh, 0FFh
        add    esp, 4
        or     eax, eax
        mov    eax, 0
        je     NEAR PTR loc_1_39CB
        db  0A1h, 060h, 0B9h, 000h, 000h
        mov    word ptr [eax + 18h], 100h
        db  066h, 0A1h, 0E4h, 0AFh, 000h, 000h
        db  08Bh, 00Dh, 060h, 0B9h, 000h, 000h
        db  0BEh, 034h, 0B0h, 000h, 000h
        mov    word ptr [ecx + 26h], ax
        db  0A1h, 060h, 0B9h, 000h, 000h
        mov    byte ptr [eax + 6ah], 0
        db  0A1h, 060h, 0B9h, 000h, 000h
        mov    byte ptr [eax + 72h], 0dh
        db  08Bh, 03Dh, 060h, 0B9h, 000h, 000h
        add    edi, 73h
        movsd
        movsd
        movsd
        movsb
        db  0A1h, 060h, 0B9h, 000h, 000h
        mov    word ptr [eax + 1ah], 50h
        db  0A1h, 060h, 0B9h, 000h, 000h
        db  08Bh, 00Dh, 060h, 0B9h, 000h, 000h
        add    eax, 8
        mov    dword ptr [ecx + 0ch], eax
        db  0A1h, 060h, 0B9h, 000h, 000h
        db  08Bh, 00Dh, 060h, 0B9h, 000h, 000h
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 8], eax
        db  0A1h, 060h, 0B9h, 000h, 000h
        db  08Bh, 00Dh, 060h, 0B9h, 000h, 000h
        add    eax, 58h
        mov    dword ptr [ecx + 5ch], eax
        db  0A1h, 060h, 0B9h, 000h, 000h
        db  08Bh, 00Dh, 060h, 0B9h, 000h, 000h
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ecx + 58h], eax
        db  0A1h, 060h, 0B9h, 000h, 000h
        add    eax, 6bh
        push   eax
        call   sub_1_3A94
        db  0A1h, 060h, 0B9h, 000h, 000h
        add    esp, 4
        mov    word ptr [eax + 24h], 0
loc_1_39C6:
        mov    eax, 1
loc_1_39CB:
        pop    edi
        pop    esi
        ret
loc_1_39CE:
        push   esi
        db  083h, 03Dh, 0ECh, 0B3h, 000h, 000h, 000h
        mov    eax, 0ffh
        je     SHORT loc_1_3A08
        call   sub_1_3670
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        call   sub_1_3702
        add    esp, 14h
        movzx  esi, ax
        call   sub_1_36D1
        mov    eax, esi
loc_1_3A08:
        pop    esi
        ret
sub_1_3A0A:
        ret
loc_1_3A0B:
        push   esi
        db  083h, 03Dh, 0ECh, 0B3h, 000h, 000h, 000h
        mov    eax, 0ffh
        je     SHORT loc_1_3A3D
        call   sub_1_3670
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        call   sub_1_37D3
        add    esp, 0ch
        movzx  esi, ax
        call   sub_1_36D1
        mov    eax, esi
loc_1_3A3D:
        pop    esi
        ret
        db  000h
sub_1_3A40:
        push   esi
        db  0CDh, 020h, 002h, 000h, 087h, 004h
        db  0A3h, 024h, 09Bh, 000h, 000h
        db  089h, 035h, 078h, 03Ah, 000h, 000h
        pop    esi
        ret
sub_1_3A54:
        mov    eax, dword ptr [esp + 4]
        dec    dword ptr [eax]
        je     SHORT loc_1_3A5D
        ret
loc_1_3A5D:
        xor    eax, eax
        ret
sub_1_3A60:
        mov    eax, dword ptr [esp + 4]
        xchg   ah, al
        ret
        db  090h
sub_1_3A68:
        mov    eax, dword ptr [esp + 4]
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
        ret
        db  0C1h, 0C8h, 010h, 0C3h
sub_1_3A78:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_3A7C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0FFh, 015h, 078h, 03Ah, 000h, 000h
        jb     SHORT loc_1_3A90
        mov    eax, esi
loc_1_3A8D:
        pop    esi
        leave
        ret
loc_1_3A90:
        xor    eax, eax
        jmp    SHORT loc_1_3A8D
sub_1_3A94:
        push   ebp
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  08Bh, 01Dh, 098h, 0B1h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    word ptr [ebp + 1ch], 2a00h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    edx, dword ptr [esp + 14h]
        mov    bx, word ptr [ebp + 1ch]
        mov    byte ptr [edx + 6], bl
        mov    bx, word ptr [ebp + 18h]
        sub    bx, 76ch
loc_1_3ACE:
        cmp    bx, 64h
        jle    SHORT loc_1_3ADA
        sub    bx, 64h
        jmp    SHORT loc_1_3ACE
loc_1_3ADA:
        mov    byte ptr [edx], bl
        mov    bx, word ptr [ebp + 14h]
        rol    bx, 8
        mov    word ptr [edx + 1], bx
        mov    word ptr [ebp + 1ch], 2c00h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    bx, word ptr [ebp + 18h]
        rol    bx, 8
        mov    word ptr [edx + 3], bx
        mov    bx, word ptr [ebp + 14h]
        mov    byte ptr [edx + 5], bh
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h
sub_1_3B18:
        push   ebp
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [esp + 14h]
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  08Bh, 01Dh, 098h, 0B1h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        movzx  ecx, byte ptr [esi]
        cmp    cl, 50h
        jae    SHORT loc_1_3B3C
        add    cl, 64h
loc_1_3B3C:
        add    ecx, 76ch
        mov    word ptr [ebp + 18h], cx
        mov    ah, byte ptr [esi + 1]
        mov    al, byte ptr [esi + 2]
        mov    word ptr [ebp + 14h], ax
        mov    word ptr [ebp + 1ch], 2b00h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ah, byte ptr [esi + 3]
        mov    al, byte ptr [esi + 4]
        mov    word ptr [ebp + 18h], ax
        xor    eax, eax
        mov    ah, byte ptr [esi + 5]
        mov    word ptr [ebp + 14h], ax
        mov    word ptr [ebp + 1ch], 2d00h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_3B90:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   ebp
        db  08Bh, 01Dh, 098h, 0B1h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        sub    edx, edx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4409h
        mov    byte ptr [ebp + 10h], al
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 2ch]
        test   eax, 1
        jne    NEAR PTR loc_1_3C5B
        mov    ax, word ptr [ebp + 14h]
        test   eax, 1000h
        jne    SHORT loc_1_3BF2
        test   eax, 8000h
        je     SHORT loc_1_3C2F
        mov    edx, 10h
        jmp    SHORT loc_1_3C5B
loc_1_3BF2:
        mov    edx, 4
        mov    word ptr [ebp + 1ch], 150bh
        movzx  ax, byte ptr [ebp + 10h]
        dec    ax
        mov    word ptr [ebp + 10h], 0
        mov    word ptr [ebp + 18h], ax
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 10h], 0adadh
        jne    SHORT loc_1_3C5B
        cmp    word ptr [ebp + 1ch], 0
        je     SHORT loc_1_3C5B
        mov    edx, 8
        jmp    SHORT loc_1_3C5B
loc_1_3C2F:
        or     edx, 2
        mov    word ptr [ebp + 1ch], 4408h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 2ch]
        test   eax, 1
        jne    SHORT loc_1_3C5B
        mov    ax, word ptr [ebp + 1ch]
        or     al, al
        je     SHORT loc_1_3C5B
        mov    edx, 1
loc_1_3C5B:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        mov    eax, edx
        pop    ebp
        pop    ebx
        ret
sub_1_3C75:
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    eax, edx
        ret
        db  08Bh, 0C0h
sub_1_3C80:
        push   ebp
        push   ebx
        push   esi
        push   edi
        call   sub_1_6E08
        mov    word ptr [eax], 4202h
        movzx  ebx, word ptr [esp + 14h]
        mov    dword ptr [eax + 4], ebx
        xor    ebx, ebx
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0ch], ebx
        call   sub_1_6E48
        jb     SHORT loc_1_3CC3
        mov    bx, word ptr [ebp + 14h]
        rol    ebx, 10h
        mov    bx, word ptr [ebp + 1ch]
loc_1_3CB0:
        call   sub_1_6EB8
        mov    ecx, dword ptr [esp + 18h]
        mov    word ptr [ecx], ax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_3CC3:
        mov    ebx, 0ffffffffh
        jmp    SHORT loc_1_3CB0
        db  08Bh, 0C0h
sub_1_3CCC:
        push   ebp
        push   ebx
        push   esi
        push   edi
        call   sub_1_6E08
        xor    edx, edx
        mov    dl, byte ptr [esp + 18h]
        mov    word ptr [eax], 3600h
        mov    dword ptr [eax + 0ch], edx
        call   sub_1_6E48
        xchg   ecx, eax
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, -1
        je     SHORT loc_1_3D37
        mov    bx, 200h
        xor    edx, edx
        movzx  ebx, bx
        movzx  eax, word ptr [ebp + 18h]
        shl    eax, 8
        div    ebx
        movzx  ebx, word ptr [ebp + 1ch]
        mul    ebx
        shr    eax, 8
        mov    edx, dword ptr [esp + 14h]
        xchg   ah, al
        mov    word ptr [edx], ax
        mov    ax, word ptr [ebp + 14h]
        xchg   ah, al
        mov    word ptr [edx + 2], ax
        mov    ax, word ptr [ebp + 10h]
        xchg   ah, al
        mov    word ptr [edx + 4], ax
        mov    ax, 0ff7fh
        mov    word ptr [edx + 6], ax
        mov    word ptr [edx + 8], ax
loc_1_3D37:
        xchg   ecx, eax
        call   sub_1_6EB8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_3D42:
        enter  34h, 0
        lea    ecx, [ebp - 34h]
        movzx  eax, byte ptr [ebp + 8]
        shl    eax, 10h
        mov    ah, 0f2h
        mov    al, byte ptr [ebp + 0ch]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        movzx  eax, word ptr [ebp + 14h]
        mov    dword ptr [ecx + 18h], eax
        mov    ax, word ptr [ebp + 1ch]
        mov    dword ptr [ecx + 14h], eax
        push   ecx
        db  0CDh, 020h, 001h, 000h, 08Eh, 004h
        pop    ecx
        lea    ecx, [ebp - 34h]
        movzx  eax, byte ptr [ecx + 1ch]
        leave
        ret
        db  058h, 08Bh, 04Ch, 024h, 020h, 089h, 001h, 0CDh, 020h, 053h, 000h, 001h, 000h, 08Bh, 04Ch, 024h
        db  020h, 0FFh, 031h, 089h, 001h, 08Bh, 0C2h, 0C3h
sub_1_3D9A:
        db  0CDh, 020h, 004h, 080h, 091h, 004h
sub_1_3DA0:
        db  0CDh, 020h, 005h, 080h, 091h, 004h
sub_1_3DA6:
        db  0CDh, 020h, 006h, 080h, 091h, 004h
        db  0CDh, 020h, 007h, 080h, 091h, 004h
sub_1_3DB2:
        db  0CDh, 020h, 008h, 080h, 091h, 004h
sub_1_3DB8:
        db  0CDh, 020h, 009h, 080h, 091h, 004h
sub_1_3DBE:
        db  0CDh, 020h, 00Ah, 080h, 091h, 004h
sub_1_3DC4:
        db  0CDh, 020h, 010h, 080h, 02Ch, 000h
sub_1_3DCA:
        db  0CDh, 020h, 001h, 080h, 02Ch, 000h
sub_1_3DD0:
        db  0CDh, 020h, 002h, 080h, 02Ch, 000h
sub_1_3DD6:
        db  0CDh, 020h, 003h, 080h, 02Ch, 000h
sub_1_3DDC:
        db  0CDh, 020h, 004h, 080h, 02Ch, 000h
        db  0CDh, 020h, 005h, 080h, 02Ch, 000h
        db  0CDh, 020h, 006h, 080h, 02Ch, 000h
sub_1_3DEE:
        db  0CDh, 020h, 007h, 080h, 02Ch, 000h
sub_1_3DF4:
        db  0CDh, 020h, 008h, 080h, 02Ch, 000h
sub_1_3DFA:
        db  0CDh, 020h, 009h, 080h, 02Ch, 000h
sub_1_3E00:
        db  0CDh, 020h, 00Ah, 080h, 02Ch, 000h
sub_1_3E06:
        db  0CDh, 020h, 00Bh, 080h, 02Ch, 000h
sub_1_3E0C:
        db  0CDh, 020h, 00Ch, 080h, 02Ch, 000h
sub_1_3E12:
        db  0CDh, 020h, 00Dh, 080h, 02Ch, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_3E20:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    ebp, dword ptr [esp + 20h]
        sub    ebx, ebx
        movzx  eax, byte ptr [ebp]
        push   eax
        lea    eax, [ebp + 1]
        push   eax
        call   sub_1_431C
        mov    dword ptr [esp + 18h], eax
        mov    esi, dword ptr [esp + 24h]
        add    esp, 8
        movzx  eax, byte ptr [esi]
        push   eax
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_431C
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        cmp    dword ptr [esp + 10h], ebx
        jne    NEAR PTR loc_1_4002
        jmp    NEAR PTR loc_1_401D
loc_1_3E6B:
        mov    cl, byte ptr [esi + edi + 1]
        movzx  eax, cl
        cmp    eax, 2ah
        je     NEAR PTR loc_1_3F11
        cmp    eax, 3fh
        je     NEAR PTR loc_1_3F22
        cmp    eax, 0aeh
        je     NEAR PTR loc_1_3F2A
        cmp    eax, 0bfh
        je     NEAR PTR loc_1_3F3D
        db  08Ah, 080h, 038h, 0A0h, 000h, 000h
        or     al, al
        jne    SHORT loc_1_3EFC
        lea    eax, [ebx + 1]
        cmp    eax, dword ptr [esp + 10h]
        jae    NEAR PTR loc_1_3F6D
        lea    eax, [edi + 1]
        cmp    eax, dword ptr [esp + 14h]
        jae    NEAR PTR loc_1_3F6D
        lea    eax, [ebp + ebx]
        cmp    byte ptr [eax + 1], cl
        jne    NEAR PTR loc_1_3F6D
        mov    al, byte ptr [eax + 2]
        mov    cl, byte ptr [esi + edi + 2]
        cmp    al, 10h
        jl     SHORT loc_1_3EE3
        cmp    al, 13h
        jg     SHORT loc_1_3EE3
        movsx  eax, al
        db  08Ah, 080h, 038h, 0A0h, 000h, 000h
loc_1_3EE3:
        cmp    cl, 10h
        jl     SHORT loc_1_3EF6
        cmp    cl, 13h
        jg     SHORT loc_1_3EF6
        movsx  ecx, cl
        db  08Ah, 089h, 038h, 0A0h, 000h, 000h
loc_1_3EF6:
        cmp    cl, al
        jne    SHORT loc_1_3F6D
        jmp    SHORT loc_1_3F52
loc_1_3EFC:
        cmp    ebx, dword ptr [esp + 10h]
        jae    SHORT loc_1_3F6D
        movzx  ecx, byte ptr [ebp + ebx + 1]
        db  038h, 081h, 038h, 0A0h, 000h, 000h
        jne    SHORT loc_1_3F6D
        jmp    SHORT loc_1_3F52
loc_1_3F11:
        mov    ebx, dword ptr [esp + 10h]
        mov    al, bl
        db  088h, 087h, 0D8h, 0B0h, 000h, 000h
        jmp    NEAR PTR loc_1_3FEC
loc_1_3F22:
        cmp    ebx, dword ptr [esp + 10h]
        jae    SHORT loc_1_3F6D
        jmp    SHORT loc_1_3F52
loc_1_3F2A:
        cmp    ebx, dword ptr [esp + 10h]
        jae    NEAR PTR loc_1_3FE6
        cmp    byte ptr [ebp + ebx + 1], 2eh
        jne    SHORT loc_1_3F6D
        jmp    SHORT loc_1_3F52
loc_1_3F3D:
        cmp    ebx, dword ptr [esp + 10h]
        jae    NEAR PTR loc_1_3FE6
        cmp    byte ptr [ebp + ebx + 1], 2eh
        je     NEAR PTR loc_1_3FE6
loc_1_3F52:
        movzx  eax, byte ptr [ebp + ebx + 1]
        db  080h, 0B8h, 038h, 0A0h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_3FE5
        add    ebx, 2
        cmp    ebx, dword ptr [esp + 10h]
        jbe    SHORT loc_1_3FE6
loc_1_3F6D:
        or     edi, edi
        je     NEAR PTR loc_1_401D
        push   edi
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_1740
        add    esp, 8
        mov    edi, eax
        movzx  eax, byte ptr [esi + edi + 1]
        cmp    eax, 2ah
        je     SHORT loc_1_3F9C
        cmp    eax, 0aeh
        je     SHORT loc_1_3F6D
        cmp    eax, 0bfh
        jne    SHORT loc_1_3F6D
loc_1_3F9C:
        or     edi, edi
        je     SHORT loc_1_3FBD
        push   edi
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_1740
        add    esp, 8
        db  08Ah, 08Fh, 0D8h, 0B0h, 000h, 000h
        db  038h, 088h, 0D8h, 0B0h, 000h, 000h
        je     SHORT loc_1_3F6D
        jmp    SHORT loc_1_3FC6
loc_1_3FBD:
        db  080h, 03Dh, 0D8h, 0B0h, 000h, 000h, 000h
        je     SHORT loc_1_401D
loc_1_3FC6:
        db  00Fh, 0B6h, 087h, 0D8h, 0B0h, 000h, 000h
        push   eax
        lea    eax, [ebp + 1]
        push   eax
        call   sub_1_1740
        add    esp, 8
        db  088h, 087h, 0D8h, 0B0h, 000h, 000h
        movzx  ebx, al
        jmp    SHORT loc_1_3FEC
loc_1_3FE5:
        inc    ebx
loc_1_3FE6:
        db  088h, 09Fh, 0D8h, 0B0h, 000h, 000h
loc_1_3FEC:
        movzx  eax, byte ptr [esi + edi + 1]
        db  080h, 0B8h, 038h, 0A0h, 000h, 000h, 000h
        jne    SHORT loc_1_4001
        inc    edi
        cmp    edi, dword ptr [esp + 14h]
        jae    SHORT loc_1_401D
loc_1_4001:
        inc    edi
loc_1_4002:
        cmp    edi, dword ptr [esp + 14h]
        jb     NEAR PTR loc_1_3E6B
        cmp    ebx, dword ptr [esp + 10h]
        jne    NEAR PTR loc_1_3F6D
        mov    eax, 1
        jmp    SHORT loc_1_401F
loc_1_401D:
        sub    eax, eax
loc_1_401F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  000h
NWSERVER_Control:
        cmp    eax, 33h
        jae    SHORT loc_1_4034
        db  0FFh, 024h, 085h, 036h, 040h, 000h, 000h
loc_1_4034:
        clc
        ret
        db  002h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 03Dh, 003h, 000h, 000h, 034h, 040h, 000h, 000h
        db  034h, 040h, 000h, 000h, 002h, 041h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h
        db  034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h
        db  034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h
        db  034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h
        db  034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h
        db  034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h
        db  034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h
        db  034h, 040h, 000h, 000h, 0A9h, 093h, 000h, 000h, 031h, 042h, 000h, 000h, 0BAh, 041h, 000h, 000h
        db  034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h
        db  034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h
        db  034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h, 034h, 040h, 000h, 000h
        db  034h, 040h, 000h, 000h, 023h, 042h, 000h, 000h, 02Ah, 042h, 000h, 000h
NWSERVER_SYSTEM_EXIT:
        db  0A1h, 068h, 0B1h, 000h, 000h
        cmp    eax, 0
        je     SHORT loc_1_4116
        push   eax
        db  0CDh, 020h, 002h, 000h, 048h, 000h
        add    esp, 4
loc_1_4116:
        clc
        ret
        db  051h, 00Fh, 0B7h, 04Fh, 03Ch, 051h, 00Fh, 0B7h, 04Fh, 014h, 051h, 066h, 089h, 047h, 03Ch, 066h
        db  089h, 05Fh, 014h, 092h, 066h, 0B8h, 014h, 03Ch, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 092h, 059h
        db  066h, 089h, 04Fh, 014h, 059h, 066h, 089h, 04Fh, 03Ch, 059h, 0C3h
NWSERVER_Service_00:
        mov    eax, 1
        clc
        ret
NWSERVER_Service_01:
        db  089h, 035h, 06Ch, 041h, 000h, 000h
        db  089h, 03Dh, 070h, 041h, 000h, 000h
        db  089h, 01Dh, 074h, 041h, 000h, 000h
        db  0C7h, 005h, 0E0h, 0B3h, 000h, 000h, 001h, 000h, 000h, 000h
        clc
        ret
loc_1_4168:
        xor    eax, eax
        dec    eax
        ret
loc_1_416C:
        db  068h, 041h, 000h, 000h, 068h
        inc    ecx
        add    byte ptr [eax], al
loc_1_4174:
        push   0ff000041h
        db  025h, 06Ch, 041h, 000h, 000h
        db  0FFh, 025h, 070h, 041h, 000h, 000h
sub_1_4184:
        db  0FFh, 025h, 074h, 041h, 000h, 000h
        db  08Fh, 03Ah, 000h, 000h, 0FFh, 037h, 000h, 000h, 095h, 039h, 000h, 000h, 0DBh, 039h, 000h, 000h
        db  043h, 03Bh, 000h, 000h, 049h, 09Eh, 000h, 000h, 014h, 09Ch, 000h, 000h, 085h, 09Ch, 000h, 000h
        db  0C7h, 09Ch, 000h, 000h, 0B1h, 09Fh, 000h, 000h, 0CEh, 039h, 000h, 000h, 00Bh, 03Ah, 000h, 000h
NWSERVER_W32_DEVICEIOCONTROL:
        push   edi
        push   ebx
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 0
        je     SHORT loc_1_420F
        cmp    ecx, -1
        je     SHORT loc_1_4217
        dec    ecx
        cmp    ecx, 0ch
        jae    SHORT loc_1_4208
        mov    eax, ecx
        push   esi
        mov    ecx, dword ptr [esi + 14h]
        sub    esp, ecx
        mov    ebx, ecx
        mov    edi, esp
        mov    esi, dword ptr [esi + 10h]
        shr    ecx, 2
        cld
        rep movsd
        db  0FFh, 014h, 085h, 08Ah, 041h, 000h, 000h
        add    esp, ebx
        pop    esi
        mov    ecx, dword ptr [esi + 1ch]
        cmp    ecx, 4
        jb     SHORT loc_1_4204
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi + 20h]
        mov    dword ptr [ecx], 4
loc_1_4204:
        xor    eax, eax
        jmp    SHORT loc_1_4219
loc_1_4208:
        mov    eax, 0ffh
        jmp    SHORT loc_1_4219
loc_1_420F:
        call   NWSERVER_Service_00
        movzx  ecx, ax
loc_1_4217:
        xor    eax, eax
loc_1_4219:
        pop    ebx
        pop    edi
        ret
        db  0E8h, 08Eh, 036h, 000h, 000h, 0F8h, 0C3h
loc_1_4223:
        call   sub_1_7666
        clc
        ret
loc_1_422A:
        call   sub_1_78AF
        clc
        ret
NWSERVER_PNP_NEW_DEVNODE:
        push   edx
        push   ebx
        call   sub_1_7D14
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_4240
        stc
loc_1_4240:
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_4244:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        leave
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_425C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        leave
        ret
sub_1_426C:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        cld
        repe cmpsb
        setne  al
        movzx  eax, al
        pop    edi
        pop    esi
        ret
        db  08Bh, 0C0h
sub_1_4288:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        cld
        cmp    ecx, 4
        jb     SHORT loc_1_42B4
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
loc_1_42B4:
        rep movsb
        pop    edi
        pop    esi
        ret
        db  02Eh, 08Bh, 0C0h, 056h, 057h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 074h, 024h, 010h, 08Bh, 04Ch, 024h
        db  014h, 03Bh, 0F7h, 076h, 0C8h, 0FDh, 04Eh, 04Fh, 003h, 0F1h, 003h, 0F9h, 083h, 0F9h, 004h, 072h
        db  026h, 047h, 08Bh, 0C7h, 04Fh, 083h, 0E0h, 003h, 091h, 02Bh, 0C1h, 0F3h, 0A4h, 08Bh, 0C8h, 0C1h
        db  0E9h, 002h, 083h, 0EEh, 003h, 083h, 0EFh, 003h, 0F3h, 0A5h, 08Bh, 0C8h, 0F7h, 0D9h, 083h, 0E1h
        db  003h, 083h, 0C6h, 003h, 083h, 0C7h, 003h, 0F3h, 0A4h, 0FCh, 05Fh, 05Eh, 0C3h, 08Bh, 0C0h
sub_1_4308:
        push   edi
        mov    edi, dword ptr [esp + 8]
        xor    eax, eax
        mov    ecx, eax
        dec    ecx
        cld
        repne scasb
        mov    eax, ecx
        not    eax
        dec    eax
        pop    edi
        ret
sub_1_431C:
        push   edi
        mov    edi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        xor    eax, eax
        jecxz  loc_1_4335
        cld
        repne scasb
        jne    SHORT loc_1_432F
        inc    ecx
loc_1_432F:
        mov    eax, dword ptr [esp + 0ch]
        sub    eax, ecx
loc_1_4335:
        pop    edi
        ret
        db  057h, 08Bh, 07Ch, 024h, 008h, 033h, 0C0h, 08Bh, 0C8h, 049h, 0FCh, 0F2h, 066h, 0AFh, 08Bh, 0C1h
        db  0F7h, 0D0h, 048h, 05Fh, 0C3h
sub_1_434C:
        push   ebp
        mov    ebp, esp
        push   ebx
        xor    ebx, ebx
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx], ebx
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h
sub_1_4364:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        db  001h, 005h, 060h, 043h, 000h, 000h
        push   ecx
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_4382
        ret
loc_1_4382:
        mov    ecx, dword ptr [esp + 4]
        db  029h, 00Dh, 060h, 043h, 000h, 000h
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_4390:
        push   ebx
        xor    eax, eax
        mov    ebx, dword ptr [esp + 8]
        push   eax
        push   ebx
        db  0CDh, 020h, 052h, 000h, 001h, 000h
        add    esp, 8
        db  029h, 005h, 060h, 043h, 000h, 000h
        xor    eax, eax
        push   eax
        push   ebx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    ebx
        ret
        db  0CDh, 020h, 0A8h, 080h, 001h, 000h
sub_1_43BD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 1ch]
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_43E2:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 015h, 000h, 001h, 000h
        pop    esi
        pop    ebp
        ret
        db  0CDh, 020h, 011h, 080h, 017h, 000h, 0CDh, 020h, 012h, 080h, 017h, 000h
sub_1_43FE:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
sub_1_4404:
        db  0CDh, 020h, 016h, 080h, 033h, 000h
sub_1_440A:
        db  0CDh, 020h, 017h, 080h, 033h, 000h
sub_1_4410:
        db  0CDh, 020h, 071h, 080h, 040h, 000h
sub_1_4416:
        db  0CDh, 020h, 072h, 080h, 040h, 000h
sub_1_441C:
        db  0CDh, 020h, 060h, 080h, 040h, 000h
sub_1_4422:
        db  0CDh, 020h, 03Ah, 080h, 040h, 000h
sub_1_4428:
        db  0CDh, 020h, 039h, 080h, 040h, 000h
sub_1_442E:
        db  0CDh, 020h, 070h, 080h, 040h, 000h
sub_1_4434:
        db  0CDh, 020h, 003h, 080h, 080h, 004h
sub_1_443A:
        mov    ax, 3000h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  066h, 0A3h, 0D0h, 0B3h, 000h, 000h
        ret
        db  000h, 000h, 000h
sub_1_4450:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    ebp, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 1ch]
        call   sub_1_4614
        jb     SHORT loc_1_4469
        xor    eax, eax
loc_1_4469:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_446E:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    ebp, dword ptr [esp + 14h]
        mov    edx, dword ptr [esp + 18h]
        call   sub_1_45BC
        jb     SHORT loc_1_4483
        xor    eax, eax
loc_1_4483:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_4488:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    ebp, dword ptr [esp + 14h]
        call   sub_1_460C
        jb     SHORT loc_1_4499
        xor    eax, eax
loc_1_4499:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_449E:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    ebp, dword ptr [esp + 14h]
        call   sub_1_4610
        jb     SHORT loc_1_44AF
        xor    eax, eax
loc_1_44AF:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_44B4:
        push   ebp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    ebp, dword ptr [esp + 14h]
        movzx  ebx, byte ptr [esp + 18h]
        call   sub_1_46B8
        jb     SHORT loc_1_44CC
        xor    eax, eax
loc_1_44CC:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_44D1:
        push   ebp
        push   ebx
        push   esi
        push   edi
        call   sub_1_6E08
        mov    ebp, dword ptr [esp + 14h]
        movzx  edx, byte ptr [esp + 20h]
        test   dl, 1
        je     SHORT loc_1_44FE
        mov    ebx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 1ch]
        movzx  ebx, word ptr [ebx]
        movzx  ecx, word ptr [ecx]
        mov    word ptr [eax + 14h], bx
        mov    word ptr [eax + 8], cx
loc_1_44FE:
        lea    edi, [eax + 28h]
        mov    dword ptr [eax + 0ch], edi
        call   sub_1_6EBC
        jb     SHORT loc_1_4548
        mov    word ptr [eax], 7143h
        mov    word ptr [eax + 4], dx
        call   sub_1_6E48
        jb     SHORT loc_1_4548
        mov    dl, byte ptr [esp + 20h]
        test   dl, 1
        je     SHORT loc_1_4530
loc_1_4524:
        call   sub_1_6EB8
        xor    eax, eax
loc_1_452B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_4530:
        mov    ebx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    dx, word ptr [eax + 14h]
        mov    si, word ptr [eax + 10h]
        mov    word ptr [ebx], dx
        mov    word ptr [ecx], si
        jmp    SHORT loc_1_4524
loc_1_4548:
        call   sub_1_6EB8
        jmp    SHORT loc_1_452B
sub_1_454F:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 14h]
        mov    eax, 7143h
        xor    ebx, ebx
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jae    SHORT loc_1_456D
        mov    ecx, 0ffffffffh
loc_1_456D:
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_4574:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    ebp, dword ptr [esp + 14h]
        call   sub_1_4590
        jae    SHORT loc_1_4588
        mov    ecx, 0ffffffffh
loc_1_4588:
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h
sub_1_4590:
        call   sub_1_6E08
        lea    edi, [eax + 28h]
        mov    dword ptr [eax + 0ch], edi
        call   sub_1_6EBC
        jb     SHORT loc_1_45B4
        mov    word ptr [eax], 7143h
        mov    byte ptr [eax + 4], 0
        call   sub_1_6E48
        movzx  ecx, word ptr [ebp + 18h]
loc_1_45B4:
        call   sub_1_6EB8
        ret
        db  08Bh, 0C0h
sub_1_45BC:
        call   sub_1_6E08
        mov    dword ptr [eax + 8], edx
        lea    edi, [eax + 28h]
        mov    dword ptr [eax + 0ch], edi
        call   sub_1_6EBC
        jb     SHORT loc_1_45E2
        mov    word ptr [eax], 7141h
        mov    dword ptr [eax + 10h], 1
        call   sub_1_6E48
loc_1_45E2:
        call   sub_1_6EB8
        ret
        db  0B2h, 03Bh
loc_1_45EA:
        call   sub_1_6E08
        lea    edi, [eax + 28h]
        mov    dword ptr [eax + 0ch], edi
        call   sub_1_6EBC
        jb     SHORT loc_1_4605
        mov    dh, 71h
        mov    dword ptr [eax], edx
        call   sub_1_6E48
loc_1_4605:
        call   sub_1_6EB8
        ret
        db  090h
sub_1_460C:
        mov    dl, 39h
        jmp    SHORT loc_1_45EA
sub_1_4610:
        mov    dl, 3ah
        jmp    SHORT loc_1_45EA
sub_1_4614:
        push   eax
        call   sub_1_6E08
        mov    dword ptr [eax + 8], edx
        lea    edi, [eax + 28h]
        mov    dword ptr [eax + 0ch], edi
        call   sub_1_6EBC
        pop    ebp
        jb     SHORT loc_1_4645
        lea    edi, [eax + 23bh]
        mov    dword ptr [eax + 14h], edi
        call   sub_1_6EBC
        jb     SHORT loc_1_4645
        mov    word ptr [eax], 7156h
        call   sub_1_6E48
loc_1_4645:
        call   sub_1_6EB8
        ret
        db  090h, 081h, 07Dh, 000h, 002h, 000h, 001h, 000h, 074h, 048h, 0E8h, 0AEh, 027h, 000h, 000h, 066h
        db  0C7h, 040h, 008h, 016h, 000h, 08Dh, 078h, 028h, 089h, 078h, 00Ch, 0E8h, 051h, 028h, 000h, 000h
        db  072h, 039h, 066h, 0C7h, 000h, 000h, 04Eh, 0E8h, 0D1h, 027h, 000h, 000h, 072h, 02Dh, 08Dh, 0B8h
        db  03Bh, 002h, 000h, 000h, 00Fh, 0B6h, 05Fh, 015h, 08Bh, 06Fh, 01Ah, 066h, 08Bh, 04Fh, 016h, 066h
        db  08Bh, 057h, 018h, 097h, 097h, 0E8h, 023h, 028h, 000h, 000h, 097h, 033h, 0C9h, 033h, 0D2h, 033h
        db  0FFh, 0C3h, 033h, 0C0h, 033h, 0EDh, 08Dh, 058h, 010h, 0EBh, 0F0h, 066h, 083h, 038h, 012h, 075h
        db  005h, 066h, 0C7h, 000h, 002h, 000h, 0E8h, 002h, 028h, 000h, 000h, 0F9h, 0C3h
sub_1_46B8:
        test   eax, eax
        jne    SHORT loc_1_46E7
        call   sub_1_6E08
        mov    dword ptr [eax + 8], ebx
        lea    edi, [eax + 28h]
        mov    dword ptr [eax + 0ch], edi
        call   sub_1_6EBC
        jb     SHORT loc_1_46E1
        mov    word ptr [eax + 4], 1
        mov    word ptr [eax], 7143h
        call   sub_1_6E48
loc_1_46E1:
        call   sub_1_6EB8
        ret
loc_1_46E7:
        mov    al, 32h
        stc
        ret
        db  000h, 000h, 000h, 000h, 000h
sub_1_46F0:
        push   40000h
        db  068h, 030h, 0B9h, 000h, 000h
        call   sub_1_146C
        add    esp, 8
        call   sub_1_4D7C
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_1_4715
        call   sub_1_4EA4
loc_1_4715:
        ret
sub_1_4716:
        db  068h, 030h, 0B9h, 000h, 000h
        call   sub_1_149C
        add    esp, 4
        ret
loc_1_4724:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 24h]
        mov    esi, dword ptr [edi + 50h]
        or     esi, esi
        je     SHORT loc_1_4765
        push   esi
        call   sub_1_65C4
        add    esp, 4
        mov    eax, dword ptr [esi + 2ch]
        or     eax, eax
        je     SHORT loc_1_474F
        push   eax
        call   sub_1_2811
        add    esp, 4
loc_1_474F:
        push   dword ptr [esi + 30h]
        call   sub_1_92E7
        add    esp, 4
        push   dword ptr [edi + 50h]
        call   sub_1_4390
        add    esp, 4
loc_1_4765:
        push   1
        push   78h
        call   sub_1_4364
        add    esp, 8
        mov    dword ptr [edi + 50h], eax
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_4783
        mov    ax, 96h
        jmp    NEAR PTR loc_1_48A7
loc_1_4783:
        mov    dword ptr [esi], esi
        lea    eax, [esi + 8]
        mov    dword ptr [esi + 4], esi
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0ch], eax
        lea    eax, [esi + 10h]
        push   0
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 14h], eax
        mov    byte ptr [esi + 18h], 0
        mov    dword ptr [esi + 20h], 0ffffffffh
        call   sub_1_92D8
        add    esp, 4
        mov    dword ptr [esi + 30h], eax
        mov    word ptr [esi + 1ah], 0ffffh
        mov    ebx, dword ptr [esp + 18h]
        mov    dword ptr [esi + 1ch], 0
        mov    ax, word ptr [ebx + 0fh]
        mov    word ptr [esi + 70h], ax
        mov    eax, dword ptr [ebx + 7]
        mov    dword ptr [esi + 60h], eax
        push   dword ptr [ebx + 0bh]
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [esi + 64h], eax
        push   dword ptr [ebx + 11h]
        call   sub_1_3A68
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        push   dword ptr [ebx + 15h]
        call   sub_1_3A68
        add    esp, 4
        mov    ebp, eax
        movzx  eax, word ptr [edi + 24h]
        mov    dword ptr [esi + 5ch], eax
        mov    ebx, dword ptr [esp + 1ch]
        movzx  eax, word ptr [edi + 24h]
        db  00Bh, 005h, 004h, 0B2h, 000h, 000h
        mov    dword ptr [ebx + 8], eax
        db  081h, 005h, 004h, 0B2h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 58h], eax
        db  00Fh, 0B7h, 005h, 0E4h, 0AFh, 000h, 000h
        cmp    dword ptr [esi + 64h], eax
        jbe    SHORT loc_1_4830
        mov    dword ptr [esi + 64h], eax
loc_1_4830:
        push   dword ptr [esi + 64h]
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [ebx + 0ch], eax
        db  08Bh, 03Dh, 000h, 0B2h, 000h, 000h
        cmp    edi, ebp
        jb     SHORT loc_1_484B
        push   ebp
        jmp    SHORT loc_1_484C
loc_1_484B:
        push   edi
loc_1_484C:
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [ebx + 10h], eax
        cmp    edi, dword ptr [esp + 10h]
        jb     SHORT loc_1_4863
        push   dword ptr [esp + 10h]
        jmp    SHORT loc_1_4864
loc_1_4863:
        push   edi
loc_1_4864:
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [esi + 64h]
        cmp    eax, 1000h
        jbe    SHORT loc_1_4886
        and    eax, 0fffff000h
        sub    eax, 4
        mov    dword ptr [esi + 68h], eax
        jmp    SHORT loc_1_488D
loc_1_4886:
        mov    dword ptr [esi + 68h], 0ffch
loc_1_488D:
        mov    eax, dword ptr [esi + 64h]
        sub    eax, 42h
        and    eax, 0ffffffch
        mov    dword ptr [esi + 6ch], eax
        mov    eax, dword ptr [esp + 20h]
        mov    word ptr [eax], 10h
        sub    ax, ax
loc_1_48A7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_48AF:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, edx
        mov    ebx, dword ptr [esp + 18h]
        mov    esi, ecx
        mov    byte ptr [ebx], 0
        lea    edx, [esi + 8]
        mov    dword ptr [esp + 10h], edx
        mov    eax, dword ptr [edx]
        cmp    eax, edx
        je     SHORT loc_1_48E1
loc_1_48CE:
        mov    ecx, dword ptr [eax + 0ch]
        cmp    edi, ecx
        je     SHORT loc_1_4912
        jb     SHORT loc_1_4918
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [eax]
        cmp    eax, edx
        jne    SHORT loc_1_48CE
loc_1_48E1:
        mov    ebp, dword ptr [esi]
        lea    eax, [edi + 0fffh]
        cmp    esi, ebp
        je     SHORT loc_1_4906
loc_1_48ED:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        lea    edx, [ecx + edx - 1]
        cmp    eax, ecx
        jb     SHORT loc_1_48FF
        cmp    edi, edx
        jbe    SHORT loc_1_493F
loc_1_48FF:
        mov    ebp, dword ptr [ebp]
        cmp    esi, ebp
        jne    SHORT loc_1_48ED
loc_1_4906:
        sub    eax, eax
loc_1_4908:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
loc_1_4912:
        inc    word ptr [eax + 8]
        jmp    SHORT loc_1_4908
loc_1_4918:
        mov    ebp, dword ptr [esi]
        lea    eax, [edi + 0fffh]
        cmp    esi, ebp
        je     SHORT loc_1_4906
loc_1_4924:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        lea    edx, [ecx + edx - 1]
        cmp    ecx, eax
        ja     SHORT loc_1_4936
        cmp    edi, edx
        jbe    SHORT loc_1_48E1
loc_1_4936:
        mov    ebp, dword ptr [ebp]
        cmp    esi, ebp
        jne    SHORT loc_1_4924
        jmp    SHORT loc_1_4906
loc_1_493F:
        call   sub_1_4E80
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_494F
        mov    byte ptr [ebx], 0ffh
        jmp    SHORT loc_1_4906
loc_1_494F:
        push   edi
        push   dword ptr [esi + 5ch]
        lea    eax, [ebp + 14h]
        push   eax
        call   sub_1_1410
        add    esp, 0ch
        mov    dword ptr [ebp + 10h], eax
        or     eax, eax
        jne    SHORT loc_1_4972
        mov    ecx, ebp
        call   sub_1_4E6C
        mov    byte ptr [ebx], 0ffh
        jmp    SHORT loc_1_4906
loc_1_4972:
        push   dword ptr [esp + 10h]
        push   ebp
        call   sub_1_0030
        add    esp, 8
        mov    word ptr [ebp + 8], 2
        mov    word ptr [ebp + 0ah], 1
        mov    dword ptr [ebp + 0ch], edi
        mov    eax, ebp
        jmp    NEAR PTR loc_1_4908
sub_1_4995:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 14h]
        movzx  edx, word ptr [ecx + 26h]
        lea    esi, [ecx + 1eh]
        call   sub_1_1979
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_4D05
        cmp    dword ptr [edi + 50h], 0
        je     NEAR PTR loc_1_4D05
        call   sub_1_27D3
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_49D9
        db  0FFh, 005h, 074h, 0B1h, 000h, 000h
        db  0FFh, 005h, 0D4h, 0B1h, 000h, 000h
        jmp    NEAR PTR loc_1_4D05
loc_1_49D9:
        mov    edi, dword ptr [edi + 50h]
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [edi + 58h], eax
        jne    NEAR PTR loc_1_4CF6
        mov    al, byte ptr [esi + 2]
        mov    byte ptr [ebx + 0bch], al
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebx + 0c2h], eax
        test   byte ptr [esi + 2], 80h
        mov    ax, word ptr [esi + 14h]
        push   eax
        je     SHORT loc_1_4A74
        call   sub_1_3A60
        add    esp, 4
        mov    cl, byte ptr [edi + 18h]
        test   cl, 8
        jne    SHORT loc_1_4A66
        mov    dx, word ptr [edi + 1ah]
        cmp    ax, dx
        je     SHORT loc_1_4A28
        sub    dx, ax
        cmp    dx, 0ffffh
        jne    SHORT loc_1_4A66
loc_1_4A28:
        test   cl, 2
        db  0C7h, 043h, 004h, 0C9h, 057h, 000h, 000h
        jne    SHORT loc_1_4A3B
        db  0C7h, 043h, 004h, 0F0h, 059h, 000h, 000h
loc_1_4A3B:
        or     byte ptr [edi + 18h], 8
        lea    eax, [ebx + 0aeh]
        mov    dword ptr [ebx + 24h], eax
        mov    word ptr [ebx + 28h], 96h
        mov    word ptr [ebx + 22h], 1
        mov    ebp, dword ptr [esp + 20h]
        mov    eax, ebx
        mov    dword ptr [ebp], 12h
        jmp    NEAR PTR loc_1_4D07
loc_1_4A66:
        push   ebx
        call   sub_1_2811
        add    esp, 4
        jmp    NEAR PTR loc_1_4D05
loc_1_4A74:
        call   sub_1_3A60
        add    esp, 4
        mov    cl, byte ptr [edi + 18h]
        test   cl, 1
        jne    NEAR PTR loc_1_4C13
        mov    dx, word ptr [edi + 1ah]
        cmp    dx, ax
        jne    SHORT loc_1_4B06
        test   cl, 2
        je     SHORT loc_1_4AC4
        db  0C7h, 043h, 004h, 02Ah, 058h, 000h, 000h
        lea    eax, [ebx + 0aeh]
        mov    dword ptr [ebx + 24h], eax
        mov    word ptr [ebx + 28h], 96h
        mov    word ptr [ebx + 22h], 1
        mov    ebp, dword ptr [esp + 20h]
        mov    eax, ebx
        mov    dword ptr [ebp], 12h
        jmp    NEAR PTR loc_1_4D07
loc_1_4AC4:
        cmp    dword ptr [esi + 1ch], 0
        jne    NEAR PTR loc_1_4CF6
        cmp    dword ptr [esi + 24h], 1
        jne    NEAR PTR loc_1_4CF6
        db  0C7h, 043h, 004h, 074h, 058h, 000h, 000h
        lea    eax, [ebx + 0aeh]
        mov    dword ptr [ebx + 24h], eax
        mov    word ptr [ebx + 28h], 96h
        mov    word ptr [ebx + 22h], 1
        mov    ebp, dword ptr [esp + 20h]
        mov    eax, ebx
        mov    dword ptr [ebp], 12h
        jmp    NEAR PTR loc_1_4D07
loc_1_4B06:
        sub    dx, ax
        cmp    dx, 0ffffh
        jne    NEAR PTR loc_1_4CF6
        cmp    dword ptr [esi + 1ch], 0
        jne    NEAR PTR loc_1_4CF6
        mov    eax, dword ptr [esi + 24h]
        cmp    eax, 2
        jne    NEAR PTR loc_1_4BDC
        call   sub_1_4D58
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_4CF6
        mov    eax, dword ptr [edi + 2ch]
        or     eax, eax
        je     SHORT loc_1_4B50
        push   eax
        call   sub_1_2811
        add    esp, 4
        mov    dword ptr [edi + 2ch], 0
loc_1_4B50:
        mov    dword ptr [edi + 20h], 0
        or     byte ptr [edi + 18h], 3
        inc    word ptr [edi + 1ah]
        db  0FFh, 005h, 080h, 0B1h, 000h, 000h
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [edi + 24h], eax
        push   dword ptr [esi + 34h]
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [edi + 28h], eax
        and    eax, 0fffff000h
        mov    dword ptr [edi + 48h], eax
        mov    eax, dword ptr [edi + 28h]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [edi + 28h]
        sub    eax, dword ptr [edi + 48h]
        mov    dword ptr [edi + 50h], eax
        push   dword ptr [esi + 38h]
        call   sub_1_3A68
        add    esp, 4
        mov    dword ptr [edi + 54h], eax
        mov    dword ptr [ebp + 0ch], eax
        mov    eax, dword ptr [edi + 28h]
        add    eax, dword ptr [edi + 54h]
        and    eax, 0fffff000h
        mov    dword ptr [edi + 4ch], eax
        mov    dword ptr [edi + 40h], 0
        mov    dword ptr [edi + 44h], 0
        push   edi
        push   ebp
        call   sub_1_0030
        add    esp, 8
        mov    ebp, dword ptr [esp + 20h]
        push   ebx
        push   ebp
        push   edi
        push   esi
        call   sub_1_9830
        add    esp, 10h
        jmp    NEAR PTR loc_1_4D07
loc_1_4BDC:
        cmp    eax, 1
        jne    NEAR PTR loc_1_4CF6
        db  0C7h, 043h, 004h, 074h, 058h, 000h, 000h
        lea    eax, [ebx + 0aeh]
        mov    dword ptr [ebx + 24h], eax
        mov    word ptr [ebx + 28h], 96h
        mov    word ptr [ebx + 22h], 1
        mov    ebp, dword ptr [esp + 20h]
        mov    eax, ebx
        mov    dword ptr [ebp], 12h
        jmp    NEAR PTR loc_1_4D07
loc_1_4C13:
        cmp    word ptr [edi + 1ah], ax
        jne    NEAR PTR loc_1_4CF6
        test   cl, 2
        je     NEAR PTR loc_1_4CBF
        mov    ebp, dword ptr [esp + 20h]
        push   ebx
        push   ebp
        push   edi
        push   esi
        call   sub_1_9830
        add    esp, 10h
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_4CBB
        mov    eax, dword ptr [edi + 10h]
        sub    eax, edi
        cmp    eax, 10h
        je     SHORT loc_1_4CBB
        call   sub_1_27D3
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_4CBB
        mov    ax, word ptr [esi + 14h]
        push   eax
        call   sub_1_3A60
        add    esp, 4
        mov    cl, byte ptr [edi + 18h]
        test   cl, 8
        jne    SHORT loc_1_4CB0
        mov    dx, word ptr [edi + 1ah]
        cmp    ax, dx
        je     SHORT loc_1_4C79
        sub    dx, ax
        cmp    dx, 0ffffh
        jne    SHORT loc_1_4CB0
loc_1_4C79:
        test   cl, 2
        db  0C7h, 043h, 004h, 0C9h, 057h, 000h, 000h
        jne    SHORT loc_1_4C8C
        db  0C7h, 043h, 004h, 0F0h, 059h, 000h, 000h
loc_1_4C8C:
        or     byte ptr [edi + 18h], 8
        lea    eax, [ebx + 0aeh]
        mov    dword ptr [ebx + 24h], eax
        mov    word ptr [ebx + 28h], 96h
        mov    word ptr [ebx + 22h], 1
        mov    dword ptr [ebp], 12h
        mov    eax, ebx
        jmp    SHORT loc_1_4D07
loc_1_4CB0:
        push   ebx
        call   sub_1_2811
        add    esp, 4
        jmp    SHORT loc_1_4D05
loc_1_4CBB:
        mov    eax, ebx
        jmp    SHORT loc_1_4D07
loc_1_4CBF:
        cmp    dword ptr [esi + 1ch], 0
        jne    SHORT loc_1_4CF6
        cmp    dword ptr [esi + 24h], 1
        jne    SHORT loc_1_4CF6
        db  0C7h, 043h, 004h, 074h, 058h, 000h, 000h
        lea    eax, [ebx + 0aeh]
        mov    dword ptr [ebx + 24h], eax
        mov    word ptr [ebx + 28h], 96h
        mov    word ptr [ebx + 22h], 1
        mov    ebp, dword ptr [esp + 20h]
        mov    eax, ebx
        mov    dword ptr [ebp], 12h
        jmp    SHORT loc_1_4D07
loc_1_4CF6:
        db  0FFh, 005h, 074h, 0B1h, 000h, 000h
        push   ebx
        call   sub_1_2811
        add    esp, 4
loc_1_4D05:
        sub    eax, eax
loc_1_4D07:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_4D0C:
        push   esi
        db  0A1h, 0D8h, 0B1h, 000h, 000h
        dec    eax
        db  0A3h, 0D8h, 0B1h, 000h, 000h
        db  0C7h, 005h, 0DCh, 0B1h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 035h, 0D4h, 0B1h, 000h, 000h
        db  003h, 035h, 014h, 0A2h, 000h, 000h
        db  0C7h, 005h, 0D4h, 0B1h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4D38:
        db  03Bh, 035h, 010h, 0A2h, 000h, 000h
        jbe    SHORT loc_1_4D56
        db  0A1h, 018h, 0A2h, 000h, 000h
        db  039h, 005h, 01Ch, 0A2h, 000h, 000h
        jbe    SHORT loc_1_4D56
        call   sub_1_283C
        or     eax, eax
        jne    SHORT loc_1_4D38
loc_1_4D56:
        pop    esi
        ret
sub_1_4D58:
        db  083h, 03Dh, 0E0h, 0B1h, 000h, 000h, 000h
        mov    eax, 0
        je     SHORT loc_1_4D7B
        db  0A1h, 0E0h, 0B1h, 000h, 000h
        mov    ecx, eax
        db  0FFh, 00Dh, 0E4h, 0B1h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        db  089h, 00Dh, 0E0h, 0B1h, 000h, 000h
loc_1_4D7B:
        ret
sub_1_4D7C:
        push   esi
        push   edi
        db  08Bh, 00Dh, 0E8h, 0B1h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_1_4D94
        db  083h, 03Dh, 0E4h, 0B1h, 000h, 000h, 003h
        jb     SHORT loc_1_4D93
        jmp    SHORT loc_1_4DD0
loc_1_4D93:
        inc    ecx
loc_1_4D94:
        db  0C7h, 005h, 0ECh, 0B1h, 000h, 000h, 000h, 000h, 000h, 000h
        dec    ecx
        db  089h, 00Dh, 0E8h, 0B1h, 000h, 000h
        push   0
        push   100h
        call   sub_1_4364
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_4DBE
        sub    eax, eax
        jmp    SHORT loc_1_4DD5
loc_1_4DBE:
        mov    edi, 10h
loc_1_4DC3:
        mov    ecx, esi
        add    esi, 10h
        call   sub_1_4E58
        dec    edi
        jne    SHORT loc_1_4DC3
loc_1_4DD0:
        mov    eax, 1
loc_1_4DD5:
        pop    edi
        pop    esi
        ret
sub_1_4DD8:
        push   esi
        db  083h, 03Dh, 000h, 0B4h, 000h, 000h, 000h
        je     SHORT loc_1_4E2A
        db  083h, 03Dh, 0E8h, 0B1h, 000h, 000h, 000h
        jne    SHORT loc_1_4E1D
        db  083h, 03Dh, 0E4h, 0B1h, 000h, 000h, 003h
        ja     SHORT loc_1_4E1D
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
loc_1_4E1D:
        db  083h, 03Dh, 0E0h, 0B1h, 000h, 000h, 000h
        jne    SHORT loc_1_4E41
        sub    eax, eax
        jmp    SHORT loc_1_4E56
loc_1_4E2A:
        sub    esi, esi
loc_1_4E2C:
        db  039h, 035h, 0E0h, 0B1h, 000h, 000h
        jne    SHORT loc_1_4E41
        call   sub_1_4D7C
        cmp    eax, esi
        jne    SHORT loc_1_4E2C
        sub    eax, eax
        jmp    SHORT loc_1_4E56
loc_1_4E41:
        db  0A1h, 0E0h, 0B1h, 000h, 000h
        mov    ecx, eax
        db  0FFh, 00Dh, 0E4h, 0B1h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        db  089h, 00Dh, 0E0h, 0B1h, 000h, 000h
loc_1_4E56:
        pop    esi
        ret
_LTEXT ends
        end
