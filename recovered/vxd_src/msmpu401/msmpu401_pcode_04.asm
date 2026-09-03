; msmpu401_pcode_04.asm -- MSMPU401 object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_000E:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_001B
        cmp    dword ptr [edi + 18h], 0
        jne    SHORT loc_4_0022
loc_4_001B:
        mov    eax, 0
        jmp    SHORT loc_4_0027
loc_4_0022:
        mov    eax, 0ffffffffh
loc_4_0027:
        pop    edi
        pop    ebp
        ret
sub_4_002A:
        push   eax
        push   ecx
        push   esi
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_4_0069
        cli
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        sti
        jb     SHORT loc_4_0069
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 8
        shr    ecx, 2
        rep stosd
        pop    eax
        mov    edi, dword ptr [esp]
        mov    dword ptr [eax + 4], edi
        cli
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        sti
        clc
loc_4_0064:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
loc_4_0069:
        stc
        jmp    SHORT loc_4_0064
sub_4_006C:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     SHORT loc_4_00A4
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_00A4
loc_4_0087:
        cmp    dword ptr [eax + 4], edi
        je     SHORT loc_4_0096
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_00A4
        jmp    SHORT loc_4_0087
loc_4_0096:
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_4_00A4:
        popfd
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
sub_4_00AA:
        jmp    SHORT loc_4_00B6
loc_4_00AC:
        mov    esi, 0
        jmp    NEAR PTR loc_4_016F
loc_4_00B6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        pushad
        mov    dword ptr [ebp - 0ch], edx
        movzx  ecx, word ptr [edi]
        mov    eax, ecx
        shl    eax, 2
        add    eax, ecx
        shl    eax, 1
        mov    dword ptr [ebp - 4], eax
        push   101h
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_0169
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_00F1:
        push   ecx
        mov    ecx, 0ah
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        cld
        rep movsb
        pop    ecx
        loop   loc_4_00F1
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_010D:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0ah
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_4_0140
        add    edi, 6
        add    esi, 0ah
        loop   loc_4_010D
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 1ch], eax
        jmp    SHORT loc_4_016F
loc_4_0140:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_015B
        add    edi, 2
loc_4_014D:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_014D
loc_4_015B:
        push   0
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_0169:
        xor    eax, eax
        mov    dword ptr [esp + 1ch], eax
loc_4_016F:
        popad
        add    esp, 0ch
        pop    ebp
        ret
sub_4_0175:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_017F:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_017F
        pop    eax
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret
sub_4_019E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_4_02B8
        movzx  edx, word ptr [ebp + 0ch]
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01C6:
        add    esp, 8
        or     edi, edi
        jne    NEAR PTR loc_4_02B8
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_02B8
        mov    dword ptr [ebp - 4], eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    dword ptr [eax], 0
        push   1
        push   24h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_02B8
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 10h], eax
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [ebp + 10h]
        mov    byte ptr [edi + 8], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0222:
        mov    word ptr [edi + 0ah], ax
        xor    eax, eax
        movsx  ecx, byte ptr [edi + 8]
        cmp    ecx, -1
        je     SHORT loc_4_0264
        push   edi
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        pop    edi
        jb     SHORT loc_4_02B8
        mov    dword ptr [edi + 14h], eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 4
        jne    SHORT loc_4_0264
        or     word ptr [edi + 0ch], 8000h
loc_4_0264:
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0ch]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0276:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0276
        mov    edx, dword ptr [esp]
        call   sub_4_00AA
        pop    edi
        mov    dword ptr [edi + 20h], eax
        or     eax, eax
        je     SHORT loc_4_02B8
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_0296:
        call   sub_4_002A
        jb     SHORT loc_4_02B7
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_0296
        pop    ebx
        mov    dword ptr [esp + 1ch], 0ffffffffh
loc_4_02B1:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_02B7:
        pop    ebx
loc_4_02B8:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     SHORT loc_4_02C4
        call   sub_4_02CE
loc_4_02C4:
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_02B1
sub_4_02CE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     SHORT loc_4_0325
        mov    eax, dword ptr [edi + 20h]
        or     eax, eax
        je     SHORT loc_4_030C
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_02F7:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_02F7
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 20h]
        call   sub_4_0175
        pop    edi
loc_4_030C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_4_0319
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_4_0319:
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_0325:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        popad
        add    esp, 4
        pop    ebp
        ret
sub_4_0342:
        push   ebp
        mov    ebp, esp
        pushad
        mov    dword ptr [esp + 1ch], 13h
        movsx  edx, word ptr [ebp + 0ch]
        call   sub_4_0454
        jb     SHORT loc_4_0393
        movsx  eax, word ptr [ebp + 10h]
        or     eax, eax
        js     SHORT loc_4_0373
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_4_053A
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_0390
loc_4_0373:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_019E
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_0390
        mov    dword ptr [esp + 1ch], 0
loc_4_0390:
        popad
        pop    ebp
        ret
loc_4_0393:
        mov    dword ptr [esp + 1ch], 24h
        jmp    SHORT loc_4_0390
sub_4_039D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_03D0
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03BD:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_03D0
        mov    ebx, dword ptr [edi + 18h]
        or     ebx, ebx
        je     SHORT loc_4_03D0
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03D0:
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_041D
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_041D
loc_4_03E6:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 10h], ebx
        je     SHORT loc_4_03F7
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_041D
        jmp    SHORT loc_4_03E6
loc_4_03F7:
        popfd
        pushfd
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_0400:
        call   sub_4_006C
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_0400
        pop    ebx
        mov    edi, eax
        call   sub_4_02CE
        mov    eax, 0ffffffffh
loc_4_041D:
        popfd
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_0423:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_043A:
        add    esp, 14h
        leave
        clc
        ret
loc_4_0440:
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
        stc
        ret
sub_4_0454:
        enter  8, 0
        push   eax
        push   ecx
        push   edx
        mov    dword ptr [ebp - 8], 0
loc_4_0462:
        mov    dword ptr [ebp - 4], 0
        mov    ecx, 40h
loc_4_046E:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 80h
        jne    SHORT loc_4_0478
        in     al, dx
        loop   loc_4_046E
loc_4_0478:
        mov    ecx, 6000h
loc_4_047D:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 40h
        je     SHORT loc_4_049A
        loop   loc_4_047D
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_04C4
        mov    dword ptr [ebp - 4], 0ffffffffh
        inc    edx
        mov    al, 0ffh
        out    dx, al
        dec    edx
        jmp    SHORT loc_4_0478
loc_4_049A:
        mov    al, 0ffh
        inc    edx
        out    dx, al
        dec    edx
        mov    ecx, 6000h
loc_4_04A4:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 80h
        je     SHORT loc_4_04BC
        loop   loc_4_04A4
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_04C4
        mov    dword ptr [ebp - 8], 0ffffffffh
        jmp    SHORT loc_4_0462
loc_4_04BC:
        in     al, dx
        cmp    al, 0feh
        jne    SHORT loc_4_04C4
        clc
        jmp    SHORT loc_4_04C5
loc_4_04C4:
        stc
loc_4_04C5:
        pop    edx
        pop    ecx
        pop    eax
        leave
        ret
sub_4_04CA:
        push   ebp
        mov    ebp, esp
        mov    ecx, 6000h
        movzx  edx, word ptr [ebp + 8]
        in     al, dx
        inc    edx
loc_4_04D8:
        in     al, dx
        test   al, 40h
        je     SHORT loc_4_04E1
        loop   loc_4_04D8
        jmp    SHORT loc_4_04E5
loc_4_04E1:
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
loc_4_04E5:
        pop    ebp
        ret
sub_4_04E7:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   ecx
        push   0ffh
        push   dword ptr [ebp + 8]
        call   sub_4_04CA
        add    esp, 8
        movzx  edx, word ptr [ebp + 8]
        inc    edx
        mov    ecx, 6000h
loc_4_0507:
        in     al, dx
        test   al, 80h
        je     SHORT loc_4_050E
        loop   loc_4_0507
loc_4_050E:
        mov    ecx, 2000h
loc_4_0513:
        in     al, dx
        loop   loc_4_0513
        pop    ecx
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_4_051B:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        inc    edx
        mov    ecx, 6000h
loc_4_052B:
        in     al, dx
        test   al, 80h
        jne    SHORT loc_4_0535
        dec    edx
        in     al, dx
        inc    edx
        loop   loc_4_052B
loc_4_0535:
        pop    edx
        pop    ecx
        pop    eax
        pop    ebp
        ret
sub_4_053A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        movzx  eax, word ptr [ebp + 0ch]
        xor    ecx, ecx
        bts    ecx, eax
        pushfd
        cli
        in     al, 0a1h
        mov    ah, al
        in     al, 21h
        push   eax
        or     eax, ecx
        xchg   ah, al
        out    0a1h, al
        xchg   ah, al
        out    21h, al
        mov    al, 0ah
        out    0a0h, al
        jmp    SHORT loc_4_0562
loc_4_0562:
        out    20h, al
        jmp    SHORT loc_4_0566
loc_4_0566:
        push   dword ptr [ebp + 8]
        call   sub_4_051B
        add    esp, 4
        in     al, 0a0h
        mov    ah, al
        in     al, 20h
        test   ecx, eax
        jne    SHORT loc_4_0590
        push   dword ptr [ebp + 8]
        call   sub_4_04E7
        add    esp, 4
        in     al, 0a0h
        mov    ah, al
        in     al, 20h
        test   ecx, eax
        jne    SHORT loc_4_0592
loc_4_0590:
        xor    eax, eax
loc_4_0592:
        push   dword ptr [ebp + 8]
        call   sub_4_051B
        add    esp, 4
        mov    edx, eax
        pop    eax
        xchg   ah, al
        out    0a1h, al
        xchg   ah, al
        out    21h, al
        mov    edx, eax
        popfd
        pop    edx
        pop    ecx
        pop    ebx
        pop    ebp
        ret
_PTEXT ends
        end
