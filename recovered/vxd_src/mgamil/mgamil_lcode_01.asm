; mgamil_lcode_01.asm -- MGAMIL object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

MGAMIL_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_000F
        db  0FFh, 024h, 085h, 011h, 000h, 000h, 000h
loc_1_000F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  07Dh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
MGAMIL_SYSTEM_EXIT:
        pushad
        db  0F7h, 005h, 00Ch, 008h, 000h, 000h, 000h, 040h, 000h, 000h
        jne    SHORT loc_1_00A5
        db  0A1h, 010h, 008h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_00A2
        mov    edi, eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        call   sub_1_00A8
        jmp    SHORT loc_1_00A6
loc_1_00A2:
        stc
        jmp    SHORT loc_1_00A6
loc_1_00A5:
        clc
loc_1_00A6:
        popad
        ret
sub_1_00A8:
        push   edx
        call   sub_1_015F
        db  066h, 083h, 03Dh, 018h, 008h, 000h, 000h, 000h
        je     SHORT loc_1_00DB
        mov    dx, 0cf8h
        mov    al, 80h
        out    dx, al
        jmp    SHORT loc_1_00C1
loc_1_00C1:
        db  066h, 08Bh, 015h, 018h, 008h, 000h, 000h
        in     al, dx
        jmp    SHORT loc_1_00CB
loc_1_00CB:
        and    al, 0efh
        or     al, 1
        out    dx, al
        jmp    SHORT loc_1_00D2
loc_1_00D2:
        mov    dx, 0cf8h
        mov    al, 0
        out    dx, al
        jmp    SHORT loc_1_00DB
loc_1_00DB:
        mov    dx, 3deh
        mov    al, 3
        out    dx, al
        jmp    SHORT loc_1_00E4
loc_1_00E4:
        inc    dx
        in     al, dx
        jmp    SHORT loc_1_00E9
loc_1_00E9:
        and    al, 7fh
        out    dx, al
        jmp    SHORT loc_1_00EE
loc_1_00EE:
        db  08Bh, 03Dh, 010h, 008h, 000h, 000h
        mov    byte ptr [edi + 3c00h], 1dh
        mov    byte ptr [edi + 3c0ah], 0
        mov    byte ptr [edi + 3c00h], 1ah
        mov    byte ptr [edi + 3c0ah], 0
        mov    byte ptr [edi + 3c00h], 18h
        mov    byte ptr [edi + 3c0ah], 80h
        mov    byte ptr [edi + 3c00h], 19h
        mov    byte ptr [edi + 3c0ah], 98h
        mov    byte ptr [edi + 3c00h], 39h
        mov    byte ptr [edi + 3c0ah], 18h
        mov    byte ptr [edi + 3c00h], 6
        mov    al, byte ptr [edi + 3c0ah]
        and    al, 0fch
        mov    byte ptr [edi + 3c0ah], al
        mov    byte ptr [edi + 3c00h], 1eh
        mov    byte ptr [edi + 3c0ah], 0
        pop    edx
        ret
sub_1_015F:
        push   edx
        push   eax
        mov    dx, 3deh
        mov    ax, 0
loc_1_0169:
        mov    al, ah
        out    dx, al
        jmp    SHORT loc_1_016E
loc_1_016E:
        inc    dx
        xor    al, al
        out    dx, al
        jmp    SHORT loc_1_0175
loc_1_0175:
        dec    dx
        inc    ah
        cmp    ah, 6
        jne    SHORT loc_1_0169
        pop    eax
        pop    edx
        ret
loc_1_0181:
        call   MGAMIL_SYSTEM_EXIT
        ret
loc_1_0187:
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ax, 4f15h
        jne    SHORT loc_1_019E
        pushad
        call   sub_1_03A0
        or     ax, ax
        popad
        js     SHORT loc_1_01D3
        clc
        ret
loc_1_019E:
        db  0F7h, 005h, 00Ch, 008h, 000h, 000h, 000h, 040h, 000h, 000h
        je     SHORT loc_1_01D3
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ax, 4f10h
        jne    SHORT loc_1_01D3
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        push   eax
        push   ebx
        mov    ax, word ptr [ebp + 1ch]
        mov    bx, word ptr [ebp + 10h]
        call   sub_1_0288
        mov    word ptr [ebp + 1ch], ax
        mov    word ptr [ebp + 10h], bx
        pop    ebx
        pop    eax
        pop    ebp
        clc
        ret
loc_1_01D3:
        stc
        ret
loc_1_01D5:
        pushad
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ax, 0bd70h
        je     SHORT loc_1_01E7
        cmp    ax, 0bd71h
        je     SHORT loc_1_0234
        jmp    SHORT loc_1_024E
loc_1_01E7:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0A1h, 058h, 008h, 000h, 000h
        mov    word ptr [ebp + 1ch], 2543h
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        jmp    SHORT loc_1_0244
loc_1_0234:
        db  0A1h, 058h, 008h, 000h, 000h
        mov    word ptr [ebp + 8], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
loc_1_0244:
        mov    ax, 0bdh
        mov    word ptr [ebp + 1ch], ax
        popad
        ret
loc_1_024E:
        popad
        stc
        ret
loc_1_0251:
        cmp    dx, 4f00h
        je     SHORT loc_1_025A
        jmp    SHORT loc_1_0276
loc_1_025A:
        pushad
        mov    ah, 38h
        mov    al, 0
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     SHORT loc_1_0275
        db  066h, 08Bh, 00Dh, 056h, 008h, 000h, 000h
        mov    word ptr [eax + 12h], cx
loc_1_0275:
        popad
loc_1_0276:
        ret
loc_1_0277:
        or     eax, eax
        jne    SHORT loc_1_0283
        mov    eax, 0ffffffffh
        clc
        jmp    SHORT loc_1_0284
loc_1_0283:
        stc
loc_1_0284:
        ret
        db  000h, 000h, 000h
sub_1_0288:
        cmp    bl, 0
        jne    SHORT loc_1_028F
        jmp    SHORT loc_1_02A2
loc_1_028F:
        cmp    bl, 1
        jne    SHORT loc_1_0296
        jmp    SHORT loc_1_02E2
loc_1_0296:
        cmp    bl, 2
        jne    SHORT loc_1_029D
        jmp    SHORT loc_1_02AB
loc_1_029D:
        mov    ax, 14fh
        ret
loc_1_02A2:
        mov    ax, 4fh
        mov    bh, 7
        mov    bl, 10h
        ret
loc_1_02AB:
        push   edi
        db  08Bh, 03Dh, 010h, 008h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_02DF
        mov    al, 1
        mov    byte ptr [edi + 1fdeh], al
        mov    al, byte ptr [edi + 1fdfh]
        xor    bh, bh
        and    al, 30h
        je     SHORT loc_1_02D8
        inc    bh
        cmp    al, 10h
        je     SHORT loc_1_02D8
        inc    bh
        cmp    al, 20h
        je     SHORT loc_1_02D8
        shl    bh, 1
loc_1_02D8:
        mov    ax, 4fh
        pop    edi
        clc
        ret
loc_1_02DF:
        pop    edi
        stc
        ret
loc_1_02E2:
        push   edi
        db  08Bh, 03Dh, 010h, 008h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_02DF
        mov    al, 1
        mov    byte ptr [edi + 1fdeh], al
        mov    al, bh
        and    bh, 0fh
        je     SHORT loc_1_0301
        and    bl, 7
        je     SHORT loc_1_0348
loc_1_0301:
        mov    al, byte ptr [edi + 1fdfh]
        and    al, 0cfh
        or     bh, bh
        jne    SHORT loc_1_031E
        mov    byte ptr [edi + 1fdfh], al
        call   sub_1_0378
        mov    ax, 4fh
        jmp    SHORT loc_1_034C
loc_1_031E:
        cmp    bh, 1
        jne    SHORT loc_1_0327
        or     al, 10h
        jmp    SHORT loc_1_0337
loc_1_0327:
        cmp    bh, 2
        jne    SHORT loc_1_0330
        or     al, 20h
        jmp    SHORT loc_1_0337
loc_1_0330:
        cmp    bh, 4
        jne    SHORT loc_1_0348
        or     al, 30h
loc_1_0337:
        mov    byte ptr [edi + 1fdfh], al
        call   sub_1_034F
        mov    ax, 4fh
        jmp    SHORT loc_1_034C
loc_1_0348:
        mov    ax, 14fh
loc_1_034C:
        pop    edi
        clc
        ret
sub_1_034F:
        mov    al, byte ptr [edi + 1fc4h]
        xchg   ah, al
        mov    al, 1
        mov    byte ptr [edi + 1fc4h], al
        mov    al, byte ptr [edi + 1fc5h]
        or     al, 20h
        mov    byte ptr [edi + 1fc5h], al
        xchg   ah, al
        dec    dx
        mov    byte ptr [edi + 1fc4h], al
        ret
sub_1_0378:
        mov    al, byte ptr [edi + 1fc4h]
        xchg   ah, al
        mov    al, 1
        mov    byte ptr [edi + 1fc4h], al
        mov    al, byte ptr [edi + 1fc5h]
        and    al, 0dfh
        mov    byte ptr [edi + 1fc5h], al
        xchg   ah, al
        mov    byte ptr [edi + 1fc4h], al
        ret
        db  000h
sub_1_03A0:
        db  0A1h, 010h, 008h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_03B8
        mov    bx, word ptr [ebp + 10h]
        cmp    bl, 0
        jne    SHORT loc_1_03B8
        call   sub_1_03BD
        ret
loc_1_03B8:
        mov    ax, 0ffffh
        ret
sub_1_03BD:
        call   sub_1_03CF
        or     ax, ax
        js     SHORT sub_1_03CF
        mov    word ptr [ebp + 1ch], ax
        mov    word ptr [ebp + 10h], bx
sub_1_03CF:
        call   sub_1_055C
        mov    edx, eax
        shr    edx, 8
        call   sub_1_057D
        mov    cl, 0fh
loc_1_03E0:
        call   sub_1_058A
        or     al, al
        je     SHORT loc_1_0409
        mov    al, 0a0h
        call   sub_1_0632
        call   sub_1_05F6
        or     al, al
        je     SHORT loc_1_0404
        call   sub_1_05CB
        mov    ax, 0ffffh
        jmp    SHORT loc_1_0418
loc_1_0404:
        call   sub_1_05CB
loc_1_0409:
        dec    cl
        jne    SHORT loc_1_03E0
        mov    ah, 1
        mov    al, 4fh
        mov    ax, 100h
        xor    bx, bx
loc_1_0418:
        ret
sub_1_0419:
        push   edi
        db  08Bh, 03Dh, 010h, 008h, 000h, 000h
        add    edi, 3c00h
        mov    byte ptr [edi], cl
        db  08Bh, 03Dh, 010h, 008h, 000h, 000h
        add    edi, 3c0ah
        mov    cl, byte ptr [edi]
        pop    edi
        ret
sub_1_0438:
        push   edi
        db  08Bh, 03Dh, 010h, 008h, 000h, 000h
        add    edi, 3c00h
        mov    byte ptr [edi], cl
        db  08Bh, 03Dh, 010h, 008h, 000h, 000h
        add    edi, 3c0ah
        mov    byte ptr [edi], ch
        pop    edi
        ret
sub_1_0457:
        push   ecx
        push   edx
        mov    cl, 2ah
        call   sub_1_0419
        or     cl, 10h
        mov    ch, cl
        mov    cl, 2ah
        call   sub_1_0438
        mov    cl, 2bh
        call   sub_1_0419
        or     cl, 10h
        mov    ch, cl
        mov    cl, 2bh
        call   sub_1_0438
        pop    edx
        pop    ecx
        ret
sub_1_0482:
        push   ecx
        push   edx
        mov    cl, 2ah
        call   sub_1_0419
        or     cl, 4
        mov    ch, cl
        mov    cl, 2ah
        call   sub_1_0438
        mov    cl, 2bh
        call   sub_1_0419
        or     cl, 4
        mov    ch, cl
        mov    cl, 2bh
        call   sub_1_0438
        pop    edx
        pop    ecx
        ret
sub_1_04AD:
        push   ecx
        push   edx
        mov    cl, 2ah
        call   sub_1_0419
        or     cl, 10h
        mov    ch, cl
        mov    cl, 2ah
        call   sub_1_0438
        mov    cl, 2bh
        call   sub_1_0419
        and    cl, 0efh
        mov    ch, cl
        mov    cl, 2bh
        call   sub_1_0438
        pop    edx
        pop    ecx
        ret
sub_1_04D8:
        push   ecx
        push   edx
        mov    cl, 2ah
        call   sub_1_0419
        or     cl, 4
        mov    ch, cl
        mov    cl, 2ah
        call   sub_1_0438
        mov    cl, 2bh
        call   sub_1_0419
        and    cl, 0fbh
        mov    ch, cl
        mov    cl, 2bh
        call   sub_1_0438
        pop    edx
        pop    ecx
        ret
sub_1_0503:
        push   ecx
        push   edx
        mov    cl, 2ah
        call   sub_1_0419
        and    cl, 0fbh
        mov    ch, cl
        mov    cl, 2ah
        call   sub_1_0438
        mov    cl, 2bh
        call   sub_1_0419
        shl    cl, 6
        pop    edx
        pop    ecx
        ret
sub_1_0525:
        push   ecx
        push   edx
        mov    cl, 2ah
        call   sub_1_0419
        and    cl, 0efh
        mov    ch, cl
        mov    cl, 2ah
        call   sub_1_0438
        mov    cl, 2bh
        call   sub_1_0419
        shl    cl, 4
        pop    edx
        pop    ecx
        ret
sub_1_0547:
        push   ecx
        push   edx
        mov    ecx, edx
        xor    esi, esi
        mov    dx, 3dah
loc_1_0551:
        inc    esi
        in     al, dx
        and    al, 8
        cmp    ecx, esi
        jne    SHORT loc_1_0551
        pop    edx
        pop    ecx
        ret
sub_1_055C:
        push   ecx
        mov    dx, 3dah
loc_1_0561:
        in     al, dx
        and    al, 8
        jne    SHORT loc_1_0561
loc_1_0566:
        in     al, dx
        and    al, 8
        je     SHORT loc_1_0566
        xor    ecx, ecx
loc_1_056D:
        inc    ecx
        in     al, dx
        and    al, 8
        jne    SHORT loc_1_056D
loc_1_0573:
        inc    ecx
        in     al, dx
        and    al, 8
        je     SHORT loc_1_0573
        mov    eax, ecx
        pop    ecx
        ret
sub_1_057D:
        push   ecx
        call   sub_1_04AD
        call   sub_1_0547
        pop    ecx
        ret
sub_1_058A:
        push   ecx
        call   sub_1_0482
        call   sub_1_0547
        call   sub_1_0457
        mov    cl, 0fh
loc_1_059C:
        call   sub_1_0547
        dec    cl
        je     SHORT loc_1_05C7
        call   sub_1_0525
        jae    SHORT loc_1_059C
        call   sub_1_0503
        jae    SHORT loc_1_059C
        call   sub_1_04D8
        call   sub_1_0547
        call   sub_1_04AD
        call   sub_1_0547
loc_1_05C7:
        mov    al, cl
        pop    ecx
        ret
sub_1_05CB:
        push   ecx
        call   sub_1_04D8
        call   sub_1_0547
        call   sub_1_04AD
        call   sub_1_0547
        call   sub_1_0457
        call   sub_1_0547
        call   sub_1_0482
        call   sub_1_0547
        pop    ecx
        ret
sub_1_05F6:
        push   ecx
        call   sub_1_0482
        mov    cl, 0fh
loc_1_05FE:
        call   sub_1_0547
        call   sub_1_0503
        jae    SHORT loc_1_0610
        dec    cl
        jne    SHORT loc_1_05FE
        jmp    SHORT loc_1_062E
loc_1_0610:
        call   sub_1_0457
        call   sub_1_0547
        call   sub_1_04AD
        call   sub_1_0547
        call   sub_1_0482
        call   sub_1_0547
loc_1_062E:
        mov    al, cl
        pop    ecx
        ret
sub_1_0632:
        push   ecx
        mov    cl, 8
        mov    ch, al
loc_1_0637:
        shl    ch, 1
        jb     SHORT loc_1_0642
        call   sub_1_04D8
        jmp    SHORT loc_1_0647
loc_1_0642:
        call   sub_1_0482
loc_1_0647:
        call   sub_1_0547
        call   sub_1_0457
        call   sub_1_0547
        call   sub_1_04AD
        call   sub_1_0547
        dec    cl
        jne    SHORT loc_1_0637
        call   sub_1_0482
        call   sub_1_0547
        pop    ecx
        ret
sub_1_0670:
        db  0C7h, 005h, 05Ch, 008h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0B8h, 060h, 008h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    eax
        test   word ptr [eax + 2], 40h
        jne    NEAR PTR loc_1_073A
        mov    eax, dword ptr [eax + 2ch]
        push   0
        push   100h
        db  068h, 0AAh, 008h, 000h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 03Dh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    SHORT loc_1_073A
        db  068h, 0AAh, 009h, 000h, 000h
        db  068h, 0AAh, 008h, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_1_073A
loc_1_06D8:
        db  068h, 00Dh, 00Ah, 000h, 000h
        db  068h, 0B9h, 009h, 000h, 000h
        db  068h, 009h, 00Ah, 000h, 000h
        push   0
        db  068h, 0AEh, 009h, 000h, 000h
        db  0FFh, 035h, 0AAh, 009h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        cmp    eax, 0eah
        je     SHORT loc_1_06D8
        cmp    eax, 0
        jne    SHORT loc_1_073A
        db  0BBh, 0B9h, 009h, 000h, 000h
        xor    ecx, ecx
loc_1_0710:
        cmp    byte ptr [ebx], 0
        je     SHORT loc_1_0725
        mov    eax, dword ptr [ebx]
        inc    ebx
        and    eax, 0ffffffh
        cmp    eax, 504e50h
        jne    SHORT loc_1_0710
        inc    ecx
loc_1_0725:
        db  089h, 00Dh, 05Ch, 008h, 000h, 000h
        db  0FFh, 035h, 0AAh, 009h, 000h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_1_073A:
        db  0A1h, 05Ch, 008h, 000h, 000h
        mov    dword ptr [ebp + 14h], eax
        ret
sub_1_0743:
        movzx  eax, word ptr [ebp + 38h]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        je     SHORT loc_1_07B8
        dec    eax
        movzx  edi, word ptr [ebp]
        add    edi, eax
        db  089h, 03Dh, 011h, 00Ah, 000h, 000h
        db  0B8h, 060h, 008h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    eax
        test   word ptr [eax + 2], 40h
        jne    SHORT loc_1_07B8
        mov    eax, dword ptr [eax + 2ch]
        push   0
        push   100h
        db  068h, 0AAh, 008h, 000h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 03Dh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    SHORT loc_1_07B8
        db  08Bh, 03Dh, 011h, 00Ah, 000h, 000h
        db  0BBh, 0AAh, 008h, 000h, 000h
loc_1_07AC:
        mov    al, byte ptr [ebx]
        mov    byte ptr [edi], al
        cmp    al, 0
        je     SHORT loc_1_07B8
        inc    ebx
        inc    edi
        jmp    SHORT loc_1_07AC
loc_1_07B8:
        ret
        db  000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 001h, 000h, 000h, 04Dh, 047h, 041h, 04Dh
        db  049h, 04Ch, 020h, 020h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0856:
        and    byte ptr [eax], al
loc_1_0858:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_085C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0860:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_08AA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_09AA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_09AE:
        dec    eax
        popad
        jb     SHORT loc_1_0A16
        db  077h, 061h
        db  072h, 065h
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A16:
        add    byte ptr [eax], al
_LTEXT ends
        end
