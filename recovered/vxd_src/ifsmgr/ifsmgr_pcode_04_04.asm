; ifsmgr_pcode_04_04.asm -- IFSMGR object 4 segment _PTEXT  (part 4/5)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_F134:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx], 0
        mov    ax, word ptr [ecx]
        cmp    ax, 0ah
        je     SHORT loc_4_F152
        cmp    ax, 8
        jne    SHORT loc_4_F182
loc_4_F152:
        mov    eax, dword ptr [ecx + 2]
        cmp    eax, 50004ch
        jne    SHORT loc_4_F16E
        cmp    word ptr [ecx + 6], 54h
        jne    SHORT loc_4_F16E
        movzx  eax, word ptr [ecx + 8]
        sub    eax, 30h
        mov    dword ptr [edx], eax
        jmp    SHORT loc_4_F182
loc_4_F16E:
        cmp    eax, 520050h
        jne    SHORT loc_4_F182
        cmp    word ptr [ecx + 6], 4eh
        jne    SHORT loc_4_F182
        mov    dword ptr [edx], 1
loc_4_F182:
        pop    ebp
        ret
sub_4_F184:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        db  08Dh, 01Ch, 0B5h, 058h, 06Eh, 000h, 000h
        mov    eax, dword ptr [edi + 7ch]
        mov    dword ptr [ebx], eax
        push   esi
        push   edi
        call   sub_4_514C
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_F1B9
        push   esi
        push   3
        call   sub_4_20C2
        add    esp, 8
        sub    eax, eax
        jmp    SHORT loc_4_F1E0
loc_4_F1B9:
        mov    dword ptr [ebx], 0
        mov    eax, dword ptr [edi + 7ch]
        push   edi
        dec    byte ptr [eax + 19h]
        db  0E8h, 033h, 057h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [edi + 7ch], 0
        mov    word ptr [edi + 1ah], 5
        mov    eax, 5
loc_4_F1E0:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_F1E5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        push   4
        call   sub_4_20C2
        add    esp, 8
        mov    word ptr [esi + 1ah], ax
        or     ax, ax
        je     SHORT loc_4_F210
        mov    dword ptr [esi + 7ch], 0
        movsx  eax, ax
        jmp    SHORT loc_4_F227
loc_4_F210:
        push   edi
        push   esi
        call   sub_4_5525
        add    esp, 8
        db  0C7h, 004h, 0BDh, 058h, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h
        sub    eax, eax
loc_4_F227:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_F22B:
        push   ebp
        mov    ebp, esp
        db  081h, 0ECh
IFSMgr_NetFunction:
        push   eax
        add    dword ptr [eax], eax
        add    byte ptr [ebx + 56h], dl
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 0ch]
        mov    edi, 0ffffffffh
        movzx  ecx, word ptr [eax + 2]
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        lea    ebx, [ebp - 4ch]
loc_4_F250:
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_4_16071
        add    esp, 8
        mov    edi, eax
        cmp    edi, -1
        je     SHORT loc_4_F29B
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        push   0
        call   sub_4_23E5
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_F27F
        mov    word ptr [ebx], di
        add    ebx, 2
        inc    esi
        jmp    SHORT loc_4_F250
loc_4_F27F:
        cmp    edi, -1
        je     SHORT loc_4_F29B
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        db  08Bh, 004h, 0C5h, 018h, 058h, 000h, 000h
        mov    dword ptr [ecx + 7ch], eax
        sub    eax, eax
        jmp    NEAR PTR loc_4_F3C1
loc_4_F29B:
        or     esi, esi
        jne    SHORT loc_4_F2A4
        jmp    NEAR PTR loc_4_F3BC
loc_4_F2A4:
        lea    eax, [ebp - 150h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        push   1
        push   59h
        db  0E8h, 03Ch, 093h, 000h, 000h
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_4_F2CF
        mov    eax, 54h
        jmp    NEAR PTR loc_4_F3C1
loc_4_F2CF:
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 11h
        rep stosd
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp - 8]
        call   sub_4_F134
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 8
        mov    edi, dword ptr [ecx + 8ch]
        mov    al, byte ptr [ecx + 8bh]
        and    eax, 3
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   43h
        add    edi, 208h
loc_4_F307:
        movzx  eax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 2
        push   eax
loc_4_F312:
        push   edi
        call   sub_4_14A00
        add    esp, 14h
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [eax + edi], 0
        push   edi
        push   ebx
        lea    eax, [ebp - 150h]
        push   eax
        call   sub_4_4080
        add    esp, 0ch
loc_4_F333:
        mov    edi, dword ptr [ebp - 140h]
        mov    byte ptr [ebx + 31h], 0ffh
        mov    dword ptr [ebp - 130h], ebx
        mov    dword ptr [ebp - 0d4h], ebx
loc_4_F349:
        or     esi, esi
        mov    dword ptr [ebp - 4], 0
        jle    SHORT loc_4_F379
loc_4_F354:
        mov    eax, dword ptr [ebp - 4]
        push   edi
        movzx  eax, word ptr [ebp + eax*2 - 4ch]
        push   eax
        lea    eax, [ebp - 150h]
        push   eax
        call   sub_4_F3F0
loc_4_F36A:
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_F3C8
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], esi
        jl     SHORT loc_4_F354
loc_4_F379:
        cmp    dword ptr [ebp - 0d4h], 0
        je     SHORT loc_4_F38D
        push   0
        push   ebx
        db  0E8h, 076h, 092h, 000h, 000h
        add    esp, 8
loc_4_F38D:
        mov    dword ptr [ebp - 0d4h], 0
        mov    dword ptr [ebp - 140h], edi
        or     edi, edi
loc_4_F39F:
        je     SHORT loc_4_F3BC
        mov    dword ptr [ebp - 134h], 0
        push   1
loc_4_F3AD:
        lea    eax, [ebp - 150h]
        push   eax
        call   sub_4_40D4
        add    esp, 8
loc_4_F3BC:
        mov    eax, 5
loc_4_F3C1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_F3C8:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_4_F3E6
        mov    eax, dword ptr [ebp - 0ch]
        dec    eax
        push   eax
        lea    eax, [ebp - 150h]
        push   eax
        call   sub_4_F184
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_F379
loc_4_F3E6:
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    dword ptr [ecx + 7ch], ebx
        jmp    SHORT loc_4_F3C1
sub_4_F3F0:
        push   ebp
        mov    ebp, esp
loc_4_F3F3:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        sar    eax, 8
        mov    edi, dword ptr [esi + 7ch]
        mov    dword ptr [esi + 28h], eax
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffh
        imul   eax, eax, 14h
        mov    dword ptr [esi + 10h], ebx
        db  08Bh, 088h, 078h, 057h, 000h, 000h
        mov    dword ptr [esi + 84h], ecx
        mov    byte ptr [esi + 4], 0
        push   0
        push   esi
        push   1eh
        db  0FFh, 0B0h, 074h, 057h, 000h, 000h
        db  0E8h, 057h, 047h, 0FAh, 0FFh
        add    esp, 10h
        mov    word ptr [esi + 1ah], ax
        or     ax, ax
        jne    SHORT loc_4_F4AE
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [esi + 7ch]
        call   sub_4_23E5
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_F4AE
        mov    eax, dword ptr [esi + 84h]
        mov    dword ptr [edi + 1ch], eax
        mov    word ptr [edi], 7253h
        mov    byte ptr [edi + 1bh], 80h
        mov    byte ptr [edi + 1ah], 3
        mov    ecx, dword ptr [esi + 7ch]
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 10h], 1
        mov    byte ptr [edi + 19h], 1
        mov    dword ptr [edi + 20h], ebx
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [edi + 14h], ax
        db  0A1h, 080h, 06Eh, 000h, 000h
        mov    dword ptr [edi + 4], eax
        sub    eax, eax
        db  089h, 03Dh, 080h, 06Eh, 000h, 000h
        mov    word ptr [esi + 1ah], 0
        jmp    SHORT loc_4_F4C7
loc_4_F4AE:
        mov    ax, word ptr [esi + 1ah]
        or     ax, ax
        jne    SHORT loc_4_F4C4
        mov    word ptr [esi + 1ah], 54h
        mov    eax, 54h
        jmp    SHORT loc_4_F4C7
loc_4_F4C4:
        movsx  eax, ax
loc_4_F4C7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_F4CE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 7ch]
        mov    ecx, dword ptr [edi + 24h]
        cmp    word ptr [edi + 16h], 0
        jbe    SHORT loc_4_F558
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 4], eax
        or     ecx, ecx
        je     SHORT loc_4_F552
loc_4_F4F1:
        mov    ebx, dword ptr [ecx + 2ch]
        mov    edx, dword ptr [ecx + 14h]
        or     edx, edx
        je     SHORT loc_4_F50F
        movzx  eax, word ptr [edx + 2fh]
        movzx  edx, word ptr [edx + 31h]
        add    edx, eax
        shl    eax, 10h
        or     edx, eax
        mov    dword ptr [esi + 8], edx
        jmp    SHORT loc_4_F535
loc_4_F50F:
        movzx  eax, word ptr [ecx + 30h]
        sub    eax, 200h
        mov    edx, eax
        and    edx, 0ffffff3fh
        sar    edx, 6
        db  08Bh, 092h, 0E0h, 066h, 000h, 000h
        and    eax, 0ffh
        mov    eax, dword ptr [edx + eax*8 + 4]
        mov    dword ptr [esi + 8], eax
loc_4_F535:
        mov    ax, word ptr [ecx + 30h]
        mov    word ptr [esi + 6], ax
        mov    dword ptr [esi + 74h], ecx
        push   esi
        db  0E8h, 077h, 048h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 7ch], edi
        mov    ecx, ebx
        or     ebx, ebx
        jne    SHORT loc_4_F4F1
loc_4_F552:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 8], eax
loc_4_F558:
        sub    eax, eax
        db  0B9h, 058h, 06Eh, 000h, 000h
loc_4_F55F:
        cmp    dword ptr [ecx], edi
        je     SHORT loc_4_F591
        add    ecx, 4
        inc    eax
        db  081h, 0F9h, 07Ch, 06Eh, 000h, 000h
        jb     SHORT loc_4_F55F
loc_4_F56F:
        mov    eax, dword ptr [esi + 7ch]
        push   esi
        dec    byte ptr [eax + 19h]
        db  0E8h, 083h, 053h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [esi + 7ch], 0
        je     SHORT loc_4_F5A5
        mov    word ptr [esi + 1ah], 4
        mov    eax, 4
        jmp    SHORT loc_4_F5AD
loc_4_F591:
        push   eax
        push   esi
        call   sub_4_F1E5
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_F56F
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_F5AD
loc_4_F5A5:
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
loc_4_F5AD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_F5B4:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
sub_4_F5CF:
        mov    dword ptr [ebp - 88h], eax
        push   3
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_F4CE
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  0A1h, 080h, 06Eh, 000h, 000h
        or     eax, eax
        mov    dword ptr [esi + 7ch], eax
        je     SHORT loc_4_F622
loc_4_F5FB:
        mov    eax, dword ptr [esi + 7ch]
        push   3
        mov    edi, dword ptr [eax + 4]
        push   esi
        call   sub_4_F4CE
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_F616
        db  089h, 03Dh, 080h, 06Eh, 000h, 000h
loc_4_F616:
        db  0A1h, 080h, 06Eh, 000h, 000h
        mov    dword ptr [esi + 7ch], eax
        or     eax, eax
        jne    SHORT loc_4_F5FB
loc_4_F622:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_F626:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 104h]
        mov    esi, dword ptr [edi]
        push   eax
        call   sub_4_2751
        add    esp, 4
        mov    dword ptr [ebp - 78h], 0fffffbbbh
        mov    dword ptr [ebp - 0f8h], 0fffffbbbh
        mov    byte ptr [ebp - 0ffh], 1
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 0e0h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 80h], eax
        mov    byte ptr [ebp - 79h], 4
        test   byte ptr [edi + 8], 1
        je     SHORT loc_4_F67A
        mov    byte ptr [ebp - 79h], 5
loc_4_F67A:
        test   byte ptr [edi + 8], 4
        je     SHORT loc_4_F684
        or     byte ptr [ebp - 79h], 40h
loc_4_F684:
        mov    word ptr [ebp - 0feh], 0ffh
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 039h, 041h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_F6E0
        mov    eax, dword ptr [edi + 8]
        and    eax, 2
        push   eax
        push   0
        push   esi
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_D46A
        add    esp, 10h
        cmp    dword ptr [ebp - 88h], 0
loc_4_F6C0:
        je     SHORT loc_4_F6D1
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 030h, 052h, 0FAh, 0FFh
        add    esp, 4
loc_4_F6D1:
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 0E8h, 041h, 0FAh, 0FFh
        add    esp, 4
loc_4_F6E0:
        movsx  eax, word ptr [ebp - 0eah]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_F6ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        mov    dword ptr [ebp - 78h], 0fffffbbbh
        mov    dword ptr [ebp - 0f8h], 0fffffbbbh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 80h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [ebp - 0ffh], 1
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 0f0h], ecx
        mov    cl, byte ptr [eax + 4]
        mov    byte ptr [ebp - 100h], cl
        mov    word ptr [ebp - 0feh], 0ffh
        mov    byte ptr [ebp - 79h], 4
        test   byte ptr [eax + 8], 1
        je     SHORT loc_4_F74F
        mov    byte ptr [ebp - 79h], 5
loc_4_F74F:
        test   byte ptr [eax + 8], 4
        je     SHORT loc_4_F759
        or     byte ptr [ebp - 79h], 40h
loc_4_F759:
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_D18A
        add    esp, 4
        mov    esi, eax
        cmp    esi, -1
        je     SHORT loc_4_F7C1
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 057h, 040h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_F7C1
        movzx  eax, byte ptr [ebp - 100h]
        push   eax
        push   esi
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 0A5h, 04Eh, 0FAh, 0FFh
        add    esp, 0ch
        cmp    dword ptr [ebp - 88h], 0
        je     SHORT loc_4_F7B2
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 04Fh, 051h, 0FAh, 0FFh
        add    esp, 4
loc_4_F7B2:
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 007h, 041h, 0FAh, 0FFh
        add    esp, 4
loc_4_F7C1:
        movsx  eax, word ptr [ebp - 0eah]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        db  0E8h, 0ACh, 03Fh, 0FAh, 0FFh
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_4_F7E9
        mov    eax, 8
        jmp    SHORT loc_4_F82D
loc_4_F7E9:
        push   edi
        call   sub_4_2751
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 84h], eax
        mov    byte ptr [edi + 4], 4
        push   edi
        db  0E8h, 0CDh, 03Fh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_F81E
        push   edi
        call   sub_4_D20D
        add    esp, 4
        push   edi
        db  0E8h, 0AAh, 040h, 0FAh, 0FFh
        add    esp, 4
loc_4_F81E:
        movsx  esi, word ptr [edi + 1ah]
        push   edi
        db  0E8h, 08Dh, 03Fh, 0FAh, 0FFh
        add    esp, 4
        mov    eax, esi
loc_4_F82D:
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   1
loc_4_F83C:
        push   104h
        db  0E8h, 0B2h, 08Dh, 000h, 000h
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_4_F859
        mov    eax, 8
        jmp    NEAR PTR loc_4_FA22
loc_4_F859:
        push   edi
        call   sub_4_2751
        add    esp, 4
        push   1
        push   edi
        db  0E8h, 0C9h, 053h, 0FAh, 0FFh
        add    esp, 8
        db  0A1h, 088h, 06Eh, 000h, 000h
        or     eax, eax
        mov    esi, dword ptr [ebp + 8]
        je     SHORT loc_4_F884
loc_4_F879:
        cmp    eax, esi
        je     SHORT loc_4_F89A
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        jne    SHORT loc_4_F879
loc_4_F884:
        cmp    eax, esi
        je     SHORT loc_4_F89A
        push   0
        push   edi
        db  0E8h, 070h, 08Dh, 000h, 000h
        add    esp, 8
        sub    eax, eax
        jmp    NEAR PTR loc_4_FA22
loc_4_F89A:
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [edi + 84h], eax
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [edi + 4], al
        push   edi
        db  0E8h, 023h, 03Fh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_F8CB
        push   0
        push   edi
        db  0E8h, 042h, 08Dh, 000h, 000h
        add    esp, 8
        mov    eax, 8
        jmp    NEAR PTR loc_4_FA22
loc_4_F8CB:
        mov    dword ptr [edi + 7ch], esi
        push   edi
        db  0E8h, 0A7h, 054h, 0FAh, 0FFh
        add    esp, 4
        movzx  edx, byte ptr [esi + 19h]
        mov    cx, word ptr [esi + 16h]
        movzx  eax, byte ptr [esi + 18h]
        add    edx, eax
        movzx  eax, cx
        add    edx, eax
        inc    edx
        cmp    edx, dword ptr [esi + 10h]
        jae    SHORT loc_4_F8F5
        jmp    NEAR PTR loc_4_F9F9
loc_4_F8F5:
        cmp    dword ptr [ebp + 0ch], 2
        jge    SHORT loc_4_F90B
        or     cx, cx
        je     SHORT loc_4_F90B
        mov    word ptr [edi + 1ah], 961h
        jmp    NEAR PTR loc_4_F9FF
loc_4_F90B:
        cmp    dword ptr [ebp + 0ch], 3
        jge    SHORT loc_4_F946
        mov    ebx, 1
loc_4_F916:
        db  08Bh, 004h, 09Dh, 0BCh, 065h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_F940
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_4_F940
        cmp    eax, esi
        jne    SHORT loc_4_F940
        lea    eax, [ebx - 1]
        push   eax
        push   edi
        call   sub_4_501C
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_F9F9
loc_4_F940:
        inc    ebx
        cmp    ebx, 20h
        jle    SHORT loc_4_F916
loc_4_F946:
        cmp    dword ptr [ebp + 10h], 0
        jne    SHORT loc_4_F957
        mov    word ptr [edi + 1ah], 0
        jmp    NEAR PTR loc_4_F9FF
loc_4_F957:
        mov    ebx, 21h
loc_4_F95C:
        db  039h, 034h, 09Dh, 0B0h, 06Dh, 000h, 000h
        jne    SHORT loc_4_F975
        push   dword ptr [ebp + 0ch]
        push   ebx
        push   edi
        db  0E8h, 0CDh, 04Ch, 0FAh, 0FFh
        add    esp, 0ch
        mov    dword ptr [edi + 7ch], esi
loc_4_F975:
        inc    ebx
        cmp    ebx, 29h
        jle    SHORT loc_4_F95C
        mov    dword ptr [ebp - 4], 1
        db  0BBh, 0C0h, 065h, 000h, 000h
loc_4_F987:
        mov    ecx, dword ptr [ebx]
        or     ecx, ecx
        je     SHORT loc_4_F9B5
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_4_F9B5
        cmp    eax, esi
        jne    SHORT loc_4_F9B5
        test   byte ptr [ecx + 0bh], 2
        je     SHORT loc_4_F9A3
        cmp    dword ptr [ebp + 0ch], 4
        jl     SHORT loc_4_F9B5
loc_4_F9A3:
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 4]
        push   edi
        db  0E8h, 08Dh, 04Ch, 0FAh, 0FFh
        add    esp, 0ch
        mov    dword ptr [edi + 7ch], esi
loc_4_F9B5:
        add    ebx, 4
        inc    dword ptr [ebp - 4]
        db  081h, 0FBh, 03Ch, 066h, 000h, 000h
        jbe    SHORT loc_4_F987
        cmp    byte ptr [esi + 18h], 0
loc_4_F9C7:
        ja     SHORT loc_4_F9CF
        test   byte ptr [esi + 1bh], 1
        je     SHORT loc_4_F9F3
loc_4_F9CF:
        and    byte ptr [esi + 1bh], 0feh
        movzx  ebx, byte ptr [esi + 18h]
        mov    byte ptr [esi + 18h], 0
        push   dword ptr [ebp + 0ch]
        push   edi
        call   sub_4_D7A7
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_F9F0
        mov    dword ptr [edi + 7ch], esi
        jmp    SHORT loc_4_F9FF
loc_4_F9F0:
        sub    dword ptr [esi + 10h], ebx
loc_4_F9F3:
        cmp    dword ptr [esi + 10h], 1
        jbe    SHORT loc_4_F9FF
loc_4_F9F9:
        mov    word ptr [edi + 1ah], 964h
loc_4_F9FF:
        push   edi
        db  0E8h, 0F9h, 04Eh, 0FAh, 0FFh
        add    esp, 4
        push   edi
        db  0E8h, 0B7h, 03Eh, 0FAh, 0FFh
        add    esp, 4
        movsx  esi, word ptr [edi + 1ah]
        push   0
        push   edi
        db  0E8h, 0E3h, 08Bh, 000h, 000h
        add    esp, 8
        mov    eax, esi
loc_4_FA22:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_4_101EC
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_FCD1
        mov    cl, byte ptr [ebx + 8bh]
        and    ecx, 4
        mov    dword ptr [ebp - 4], ecx
        cmp    word ptr [ebx + 0a8h], 0
        jne    SHORT loc_4_FA72
        or     ecx, ecx
        jne    SHORT loc_4_FA72
        mov    word ptr [ebx + 1ah], 57h
        jmp    NEAR PTR loc_4_FCD1
loc_4_FA72:
        or     cl, cl
        je     SHORT loc_4_FA7E
        mov    eax, dword ptr [ebx + 9ch]
        jmp    SHORT loc_4_FA8E
loc_4_FA7E:
        lea    eax, [ebx + 14h]
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_FA8E:
        mov    dword ptr [ebx + 14h], eax
        push   ebx
        call   sub_4_15558
        add    esp, 4
        cmp    eax, 3
        je     NEAR PTR loc_4_FCD1
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_FADA
        db  0B8h, 050h, 06Fh, 000h, 000h
        mov    edx, dword ptr [ebx + 0ch]
        add    edx, 4
        mov    ecx, dword ptr [eax]
        cmp    ecx, dword ptr [edx]
        jne    SHORT loc_4_FAC2
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
loc_4_FAC2:
        je     SHORT loc_4_FACF
        mov    word ptr [ebx + 1ah], 7bh
loc_4_FACA:
        jmp    NEAR PTR loc_4_FCCA
loc_4_FACF:
        push   1
        push   ebx
        call   sub_4_15FA0
        add    esp, 8
loc_4_FADA:
        db  0BEh, 03Ch, 06Fh, 000h, 000h
        mov    edi, dword ptr [ebx + 0ch]
        mov    ecx, 4
        lea    edi, [edi + 4]
        repe cmpsd
        jne    NEAR PTR loc_4_FCC4
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    NEAR PTR loc_4_FCC4
        mov    eax, dword ptr [ebx + 0ch]
        movzx  ecx, word ptr [eax + 4]
        cmp    word ptr [eax + ecx + 4], 0
        je     NEAR PTR loc_4_FCC4
        push   1
        push   ebx
        call   sub_4_15FA0
        add    esp, 8
        db  0C7h, 043h, 07Ch, 0F8h, 06Eh, 000h, 000h
        push   ebx
        db  0E8h, 052h, 052h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 18h], 0
        push   0
        push   ebx
        db  0E8h, 016h, 044h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_FCD1
        push   ebx
        db  0E8h, 047h, 048h, 0FAh, 0FFh
        add    esp, 4
        mov    esi, eax
        cmp    esi, -1
        jne    SHORT loc_4_FB69
        mov    word ptr [ebx + 1ah], 4
        push   ebx
        db  0E8h, 036h, 047h, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_4_FCD1
loc_4_FB69:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_4_FBF7
        mov    eax, dword ptr [ebx + 0a8h]
        mov    dword ptr [ebx + 24h], eax
        mov    dword ptr [ebx], 1000h
        or     eax, eax
        jne    SHORT loc_4_FB92
        mov    dword ptr [ebx + 24h], 1000h
        or     word ptr [ebx + 18h], 4
loc_4_FB92:
        mov    dword ptr [ebp - 18h], 1
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp - 14h], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebx + 14h], eax
        or     eax, eax
        jne    SHORT loc_4_FBDE
        push   esi
        push   ebx
        db  0E8h, 027h, 048h, 0FAh, 0FFh
        add    esp, 8
        push   ebx
        db  0E8h, 0C7h, 046h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 1ah], 8
        jmp    NEAR PTR loc_4_FCD1
loc_4_FBDE:
        mov    eax, dword ptr [ebx + 0b0h]
        mov    dword ptr [ebx + 28h], eax
        or     word ptr [ebx + 18h], 1
        mov    eax, dword ptr [ebx + 0ach]
        mov    dword ptr [ebx + 2ch], eax
        jmp    SHORT loc_4_FC51
loc_4_FBF7:
        movzx  eax, word ptr [ebx + 0a8h]
        mov    dword ptr [ebx + 24h], eax
        movzx  eax, word ptr [ebx + 0b0h]
        mov    dword ptr [ebx], eax
        test   byte ptr [ebx + 8bh], 4
        je     SHORT loc_4_FC1B
        mov    eax, dword ptr [ebx + 98h]
        jmp    SHORT loc_4_FC28
loc_4_FC1B:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_FC28:
        mov    dword ptr [ebx + 14h], eax
        movzx  eax, word ptr [ebx + 0d0h]
        mov    word ptr [ebx + 18h], 0
        shl    eax, 10h
        movzx  ecx, word ptr [ebx + 98h]
        add    eax, ecx
        mov    dword ptr [ebx + 2ch], eax
        movzx  eax, word ptr [ebx + 0ach]
        mov    dword ptr [ebx + 28h], eax
loc_4_FC51:
        push   ebx
        call   sub_4_10259
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_FC9E
        test   byte ptr [ebx + 18h], 1
        je     SHORT loc_4_FC89
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 20h], eax
loc_4_FC89:
        push   esi
        push   ebx
        db  0E8h, 05Eh, 047h, 0FAh, 0FFh
        add    esp, 8
        push   ebx
        db  0E8h, 0FEh, 045h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_FCD1
loc_4_FC9E:
        mov    eax, dword ptr [ebx + 74h]
        mov    word ptr [ebx + 0b4h], si
        mov    word ptr [eax + 20h], 1
        mov    ecx, dword ptr [ebx + 74h]
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [ecx + 0ch], eax
        push   0
        push   ebx
        call   sub_4_FCD8
        db  083h
IFSMgr_SetErrString:
        les    ecx, ptr [eax]
        jmp    SHORT loc_4_FCD1
loc_4_FCC4:
        mov    word ptr [ebx + 1ah], 3
loc_4_FCCA:
        mov    dword ptr [ebx + 7ch], 0
loc_4_FCD1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_FCD8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [esi + 4], al
        sub    edi, edi
        db  066h, 0A1h, 00Eh, 06Fh, 000h, 000h
        mov    word ptr [esi + 18h], ax
        db  080h, 03Dh, 02Ah, 059h, 000h, 000h, 000h
        jbe    SHORT loc_4_FD16
        db  0BBh, 034h, 057h, 000h, 000h
loc_4_FD01:
        push   esi
        call   dword ptr [ebx]
        add    esp, 4
        add    ebx, 10h
        inc    edi
        db  00Fh, 0B6h, 005h, 02Ah, 059h, 000h, 000h
        cmp    eax, edi
        jg     SHORT loc_4_FD01
loc_4_FD16:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_FD1B:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C43D
        add    esp, 4
        or     eax, eax
        db  075h, 029h
        mov    ecx, dword ptr [esi + 94h]
        movzx  eax, word ptr [esi + 6]
        mov    eax, dword ptr [ecx + eax*4 + 0d4h]
        db  081h, 078h, 010h, 0F8h, 06Eh, 000h, 000h
        db  074h, 00Dh
        db  066h
IFSMgr_GetErrString:
        mov    dword ptr [esi + 1ah], 6b80006h
        add    byte ptr [eax], al
        add    bl, ch
        add    ch, byte ptr [ebx]
        rcr    byte ptr [esi + 5dh], 0c3h
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_FD1B
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_FDB6
        push   esi
        db  0E8h, 089h, 04Bh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 14h], 0
        push   esi
        call   sub_4_6FEC
        add    esp, 4
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_4_FDAB
        mov    word ptr [esi + 98h], ax
        shr    eax, 10h
        mov    word ptr [esi + 0d0h], ax
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [esi + 0ach], ax
loc_4_FDAB:
        push   1
        push   esi
        call   sub_4_FCD8
        add    esp, 8
loc_4_FDB6:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_FD1B
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_FE11
        push   esi
        call   sub_4_10947
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_FE11
        mov    eax, dword ptr [esi]
        mov    dword ptr [esi + 0b4h], eax
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [esi + 0a8h], eax
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [esi + 0b0h], eax
        mov    dword ptr [esi + 0ach], 0
        movzx  eax, word ptr [esi + 18h]
        mov    dword ptr [esi + 9ch], eax
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [esi + 98h], eax
loc_4_FE11:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_FD1B
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_FE9E
        mov    dword ptr [esi], 0
        db  0F6h, 086h, 08Bh, 000h, 000h, 000h
IFSMgr_Get_DOSTime:
        add    al, 74h
        or     byte ptr [ebx + 9886h], cl
        add    bl, ch
        or     eax, 380068h
        add    al, ch
        or     esp, dword ptr [ebx]
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 089h, 046h, 014h, 00Fh, 0B7h, 086h, 0ACh, 000h, 000h, 000h, 0C1h
        db  0E0h, 010h, 00Fh, 0B7h, 08Eh, 0B0h, 000h, 000h, 000h, 003h, 0C1h, 089h, 046h, 028h, 056h, 0E8h
        db  0F8h, 008h, 000h, 000h, 083h, 0C4h, 004h, 00Bh, 0C0h, 07Dh, 009h, 056h, 0E8h, 0D8h, 04Fh, 0FAh
        db  0FFh, 083h, 0C4h, 004h, 066h, 083h, 07Eh, 01Ah, 000h, 075h, 01Ch, 08Bh, 006h, 066h, 089h, 086h
        db  0B4h, 000h, 000h, 000h, 08Bh, 046h, 028h, 066h, 089h, 086h, 0B0h, 000h, 000h, 000h, 066h, 0C7h
        db  086h, 0ACh, 000h, 000h, 000h, 000h, 000h
loc_4_FE9E:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], 0
        mov    al, byte ptr [ebx + 8bh]
        and    eax, 4
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_4_FECD
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_4_FED7
loc_4_FECD:
        movzx  eax, word ptr [ebx + 0ach]
        mov    dword ptr [ebp - 4], eax
loc_4_FED7:
        push   ebx
        call   sub_4_101EC
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_100EE
        cmp    dword ptr [ebp - 4], 9
        jg     NEAR PTR loc_4_100E8
        cmp    word ptr [ebx + 0a8h], 2
        jne    NEAR PTR loc_4_100E8
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_FF1E
        db  0C7h, 045h
IFSMGR_Service_119:
        in     al, 0
        add    byte ptr [eax], al
        add    byte ptr [ebx + 9883h], cl
        add    byte ptr [ecx - 7c7417bbh], cl
        pushfd
        add    byte ptr [eax], al
        add    bl, ch
        dec    esi
loc_4_FF1E:
        test   byte ptr [ebx + 8bh], 4
        je     SHORT loc_4_FF2F
        mov    eax, dword ptr [ebx + 98h]
        jmp    SHORT loc_4_FF3C
loc_4_FF2F:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_FF3C:
        push   8
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_4_6220
        add    esp, 0ch
        test   byte ptr [ebx + 8bh], 4
        lea    eax, [ebx + 14h]
        je     SHORT loc_4_FF5F
        mov    eax, dword ptr [ebx + 9ch]
IFSMgr_NetToWin32Time:
        jmp    SHORT loc_4_FF6C
loc_4_FF5F:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_FF6C:
        mov    dword ptr [ebx + 14h], eax
        push   ebx
        call   sub_4_15558
        add    esp, 4
        and    eax, 0fh
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        je     SHORT loc_4_FF9E
        cmp    eax, 2
        je     SHORT loc_4_FFDA
        cmp    eax, 6
        je     SHORT loc_4_FFA5
        mov    word ptr [ebx + 1ah], 3
        mov    dword ptr [ebx + 7ch], 0
        jmp    NEAR PTR loc_4_100EE
loc_4_FF9E:
        mov    dword ptr [ebx + 7ch], 0
loc_4_FFA5:
        mov    edi, dword ptr [ebx + 0ch]
        add    edi, 4
loc_4_FFAB:
        mov    eax, dword ptr [ebx + 8ch]
        add    eax, 208h
        mov    dword ptr [ebx + 10h], eax
        db  0BEh, 03Ch, 06Fh, 000h, 000h
        mov    ecx, 4
        repe cmpsd
        jne    SHORT loc_4_FFCD
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
loc_4_FFCD:
        je     SHORT loc_4_10017
        mov    word ptr [ebx + 1ah], 3
        jmp    NEAR PTR loc_4_100EE
loc_4_FFDA:
        mov    eax, dword ptr [ebx + 0ch]
        cmp    dword ptr [ebp - 8], 0
        movzx  edx, word ptr [eax + 4]
        lea    ecx, [eax + 4]
        lea    edi, [edx + eax + 4]
        je     SHORT loc_4_FFAB
        db  0BAh, 050h, 06Fh, 000h, 000h
        mov    eax, dword ptr [edx]
        cmp    eax, dword ptr [ecx]
        jne    SHORT loc_4_FFAB
        mov    ax, word ptr [edx + 4]
        cmp    ax, word ptr [ecx + 4]
sub_4_10001:
        jne    SHORT loc_4_FFAB
        mov    dword ptr [ebp - 0ch], 1
        push   1
        push   ebx
        call   sub_4_15FA0
        add    esp, 8
        jmp    SHORT loc_4_FFAB
loc_4_10017:
        cmp    dword ptr [ebp - 10h], 2
        jne    SHORT loc_4_10086
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_10086
        test   byte ptr [ebx + 8bh], 4
        je     SHORT loc_4_10031
        mov    eax, dword ptr [ebp - 18h]
        jmp    SHORT loc_4_1003C
loc_4_10031:
        push   dword ptr [ebp - 18h]
        call   sub_4_21A2
        add    esp, 4
loc_4_1003C:
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebx + 28h], eax
        movzx  eax, word ptr [ebx + 0b0h]
        sub    esi, esi
        mov    dword ptr [ebx + 24h], eax
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebx + 18h], ax
        mov    byte ptr [ebx + 4], 2
        db  080h, 03Dh, 02Ah, 059h, 000h, 000h, 000h
        jbe    NEAR PTR loc_4_100EE
        db  0BFh, 034h, 057h, 000h, 000h
loc_4_1006F:
        push   ebx
        call   dword ptr [edi]
        add    esp, 4
        add    edi, 10h
        inc    esi
        db  00Fh, 0B6h, 005h, 02Ah, 059h, 000h, 000h
        cmp    eax, esi
        jg     SHORT loc_4_1006F
        jmp    SHORT loc_4_100EE
loc_4_10086:
        mov    dword ptr [ebx + 20h], 0
        test   byte ptr [ebx + 8bh], 4
        je     SHORT loc_4_1009B
        mov    eax, dword ptr [ebp - 18h]
        jmp    SHORT loc_4_100A6
loc_4_1009B:
        push   dword ptr [ebp - 18h]
        call   sub_4_21A2
        add    esp, 4
loc_4_100A6:
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebx + 28h], eax
        movzx  eax, word ptr [ebx + 0b0h]
        mov    dword ptr [ebx], eax
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebx + 18h], ax
        push   1
        push   ebx
        call   sub_4_15FA0
        add    esp, 8
        mov    byte ptr [ebx + 4], 0ffh
        mov    dword ptr [ebx + 10h], 0
        mov    dword ptr [ebx + 24h], 0
        push   ebx
        call   sub_4_105AE
        add    esp, 4
        jmp    SHORT loc_4_100EE
loc_4_100E8:
        mov    word ptr [ebx + 1ah], 57h
loc_4_100EE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0f8h], eax
        mov    dword ptr [ebp - 0e0h], 0
        mov    dword ptr [ebp - 0dch], 0
        mov    dword ptr [ebp - 0f4h], 0
        mov    dword ptr [ebp - 0dch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0f0h], eax
        lea    eax, [ebp - 104h]
        mov    word ptr [ebp - 0eah], 0
        push   eax
        call   sub_4_10259
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_1019F
        db  0C7h, 085h, 078h, 0FFh, 0FFh, 0FFh, 0F8h, 06Eh, 000h, 000h
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 002h, 04Ch, 0FAh, 0FFh
        add    esp, 4
        db  066h, 0FFh, 005h, 00Eh, 06Fh, 000h, 000h
        push   0
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_FCD8
        add    esp, 8
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 0f4h]
        mov    dword ptr [eax], ecx
loc_4_1019F:
        movsx  eax, word ptr [ebp - 0eah]
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0e4h], eax
        mov    byte ptr [ebp - 100h], 2
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_103ED
        add    esp, 4
        movsx  eax, word ptr [ebp - 0eah]
        mov    esp, ebp
        pop    ebp
        ret
sub_4_101EC:
        push   ebp
        mov    ebp, esp
        db  083h, 03Dh, 0F4h, 06Eh, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        jne    SHORT loc_4_10201
        mov    word ptr [eax + 1ah], 1
loc_4_10201:
        movsx  eax, word ptr [eax + 1ah]
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    ecx, dword ptr [eax + 0b0h]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [eax + 0ch], ecx
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 20h]
        cmp    byte ptr [esi + 4], 0
        je     SHORT loc_4_10245
        push   dword ptr [esi + 28h]
        call   sub_4_637E
        add    esp, 4
        mov    dword ptr [edi + 4bh], eax
        jmp    SHORT loc_4_10253
loc_4_10245:
        push   dword ptr [edi + 4bh]
        call   sub_4_6260
        add    esp, 4
        mov    dword ptr [esi + 28h], eax
loc_4_10253:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_10259:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 0ch]
        call   sub_4_109F5
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_10287
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1ah], 0b7h
        mov    eax, 0b7h
        jmp    NEAR PTR loc_4_103E6
loc_4_10287:
        mov    dword ptr [ebp - 0ch], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        movzx  eax, word ptr [eax]
        add    eax, 61h
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    ebx, dword ptr [ebp - 4]
        or     ebx, ebx
        mov    eax, dword ptr [ebp + 8]
        jne    SHORT loc_4_102CF
        mov    word ptr [eax + 1ah], 8
        mov    eax, 8
        jmp    NEAR PTR loc_4_103E6
loc_4_102CF:
        mov    esi, dword ptr [eax + 0ch]
        mov    edi, dword ptr [ebp - 4]
        movzx  eax, word ptr [esi]
        add    eax, 2
        add    edi, 5fh
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 24h], 0
        je     NEAR PTR loc_4_103A8
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebx + 18h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 18h], 1
        je     SHORT loc_4_10340
        mov    word ptr [ebx + 24h], 0
        mov    dword ptr [ebx + 26h], 0ffffffffh
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebx + 4fh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    dword ptr [ebx + 0ch], eax
        jmp    SHORT loc_4_10361
loc_4_10340:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 4fh], 0ffffffffh
        mov    eax, dword ptr [eax + 28h]
        mov    word ptr [ebx + 24h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebx + 26h], eax
        mov    dword ptr [ebx + 0ch], 3e8h
loc_4_10361:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 18h]
        mov    word ptr [ebx + 49h], ax
        mov    dword ptr [ebx + 20h], 0
        mov    dword ptr [ebx + 1ch], 0
        mov    dword ptr [ebx + 2ah], 0
        call   sub_4_11FAE
        mov    dword ptr [ebx + 4bh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 20h], ebx
        mov    eax, dword ptr [eax + 10h]
        db  0C7h, 000h, 063h, 0E7h, 000h, 000h
        db  0C7h, 040h, 004h, 0AEh, 0E5h, 000h, 000h
        db  0C7h, 040h, 008h, 0BCh, 06Fh, 000h, 000h
        jmp    SHORT loc_4_103CB
loc_4_103A8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebx + 2ah], eax
        mov    dword ptr [ebx + 26h], 0
        mov    dword ptr [ebx + 8], 1
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebx + 49h], 2
        mov    dword ptr [eax + 10h], ebx
loc_4_103CB:
        db  0C7h, 003h, 058h, 06Fh, 000h, 000h
        db  0A1h, 05Ch, 06Fh, 000h, 000h
        mov    dword ptr [ebx + 4], eax
        db  089h, 01Dh, 05Ch, 06Fh, 000h, 000h
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax], ebx
        sub    eax, eax
loc_4_103E6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_103ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 20h]
        cmp    byte ptr [edi + 4], 2
        je     SHORT loc_4_10408
        sub    eax, eax
        jmp    NEAR PTR loc_4_104B4
loc_4_10408:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        cmp    dword ptr [esi + 44h], 0
        je     SHORT loc_4_10457
        test   byte ptr [esi + 48h], 1
        je     SHORT loc_4_1042F
        lea    eax, [esi + 2eh]
        push   eax
        db  0E8h, 033h, 02Ch, 0FAh, 0FFh
        add    esp, 4
loc_4_1042F:
        mov    word ptr [edi + 1ah], 6
        test   byte ptr [esi + 48h], 2
        je     SHORT loc_4_10446
        push   dword ptr [esi + 44h]
        call   sub_4_11104
        add    esp, 4
loc_4_10446:
        push   dword ptr [esi + 44h]
        db  0E8h, 0A3h, 02Ah, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [edi + 1ah], 0
loc_4_10457:
        mov    eax, dword ptr [esi + 26h]
        mov    dword ptr [edi + 14h], eax
        mov    ax, word ptr [esi + 24h]
        mov    word ptr [edi + 18h], ax
        test   byte ptr [esi + 49h], 1
        je     SHORT loc_4_10490
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
loc_4_10490:
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], esi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
        sub    eax, eax
loc_4_104B4:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, 8
        push   0
        db  0E8h, 0BAh, 032h, 0FAh, 0FFh
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_4_10524
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 14h], eax
        movzx  eax, word ptr [ebp + 10h]
        mov    dword ptr [esi], eax
        movzx  ax, byte ptr [ebp + 14h]
        mov    word ptr [esi + 6], ax
        mov    dword ptr [esi + 28h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 8], 0
        mov    dword ptr [esi + 20h], 0
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [esi + 24h], eax
        push   esi
        call   sub_4_105AE
        add    esp, 4
        mov    edi, eax
        push   esi
        db  0E8h, 094h, 032h, 0FAh, 0FFh
        add    esp, 4
loc_4_10524:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_1052A:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        db  0FFh, 035h, 0F2h, 06Ah, 000h, 000h
        push   dword ptr [eax + 57h]
        push   dword ptr [eax + 53h]
        call   sub_4_216C
        mov    esp, ebp
        pop    ebp
        ret
sub_4_10545:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 14h]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 4], eax
        mov    eax, ebx
        shr    eax, 0ch
        mov    esi, dword ptr [ebp - 4]
        add    esi, ebx
        add    esi, 0fffh
        shr    esi, 0ch
        sub    esi, eax
        db  0FFh, 035h, 0EEh, 06Ah, 000h, 000h
        push   esi
        push   eax
        call   sub_4_2164
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_1058C
        mov    word ptr [edi + 1ah], 5
        sub    eax, eax
        jmp    SHORT loc_4_105A7
loc_4_1058C:
        and    ebx, 0fffh
        lea    ecx, [eax + ebx]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 5bh], ecx
        shr    ecx, 0ch
        mov    dword ptr [eax + 53h], ecx
        mov    dword ptr [eax + 57h], esi
        mov    eax, dword ptr [ebp - 4]
loc_4_105A7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_105AE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 20h]
        or     edi, edi
        jne    SHORT loc_4_105E2
        push   dword ptr [esi + 0ch]
        call   sub_4_109F5
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_4_105E2
        mov    word ptr [esi + 1ah], 2
        mov    eax, 2
        jmp    NEAR PTR loc_4_106C2
loc_4_105E2:
        mov    eax, dword ptr [edi + 18h]
        or     eax, eax
        je     SHORT loc_4_105F1
        cmp    dword ptr [esi], eax
        ja     NEAR PTR loc_4_106C2
loc_4_105F1:
        mov    ecx, dword ptr [edi + 2ah]
        or     ecx, ecx
        je     SHORT loc_4_10612
        push   dword ptr [esi + 24h]
        push   dword ptr [esi + 10h]
        mov    ax, word ptr [esi + 6]
        push   eax
        push   dword ptr [esi]
        push   dword ptr [esi + 14h]
        call   ecx
        add    esp, 14h
        jmp    NEAR PTR loc_4_106C2
loc_4_10612:
        push   edi
        call   sub_4_10920
        add    esp, 4
        movzx  eax, ax
        mov    ecx, dword ptr [esi]
        add    ecx, 2
        cmp    eax, ecx
        jae    SHORT loc_4_10637
        mov    word ptr [esi + 1ah], 79h
        mov    eax, 79h
        jmp    NEAR PTR loc_4_106C2
loc_4_10637:
        push   2
        push   esi
        push   dword ptr [edi + 20h]
        push   edi
        call   sub_4_106C9
        add    esp, 10h
        push   dword ptr [esi]
        push   dword ptr [esi + 14h]
        push   eax
        push   edi
        call   sub_4_106C9
        add    esp, 10h
        mov    dword ptr [edi + 20h], eax
        mov    esi, dword ptr [edi + 44h]
        or     esi, esi
        je     SHORT loc_4_106C0
        mov    bl, byte ptr [edi + 48h]
        mov    dword ptr [edi + 44h], 0
        mov    byte ptr [edi + 48h], 0
        test   bl, 1
        je     SHORT loc_4_1067E
        lea    eax, [edi + 2eh]
        push   eax
        db  0E8h, 0E4h, 029h, 0FAh, 0FFh
        add    esp, 4
loc_4_1067E:
        mov    word ptr [esi + 1ah], 0
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [edi + 5bh]
        mov    dword ptr [esi + 14h], eax
        push   esi
        call   sub_4_1083A
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
        push   edi
        push   esi
        call   sub_4_1052A
        add    esp, 8
        test   bl, 2
        je     SHORT loc_4_106B7
        push   esi
        call   sub_4_11104
        add    esp, 4
loc_4_106B7:
        push   esi
        db  0E8h, 034h, 028h, 0FAh, 0FFh
        add    esp, 4
loc_4_106C0:
        sub    eax, eax
loc_4_106C2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_106C9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebx + 14h]
        sub    edx, dword ptr [ebp + 0ch]
        cmp    edx, eax
        jb     SHORT loc_4_106E1
        mov    edx, eax
loc_4_106E1:
        mov    edi, dword ptr [ebx + 10h]
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, edx
        shr    ecx, 2
        add    edi, dword ptr [ebp + 0ch]
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        cmp    edx, eax
        jae    SHORT loc_4_10716
        sub    eax, edx
        mov    esi, dword ptr [ebp + 10h]
        add    esi, edx
        mov    edi, dword ptr [ebx + 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_4_10722
loc_4_10716:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, edx
        cmp    dword ptr [ebx + 14h], eax
        ja     SHORT loc_4_10722
        sub    eax, eax
loc_4_10722:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [eax + 44h]
        or     ecx, ecx
        je     SHORT loc_4_10751
        mov    dword ptr [eax + 44h], 0
        and    byte ptr [eax + 48h], 0feh
        mov    word ptr [ecx + 1ah], 79h
        push   ecx
        db  0E8h, 0A3h, 027h, 0FAh, 0FFh
        add    esp, 4
loc_4_10751:
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     SHORT loc_4_10761
        push   eax
        db  0E8h, 057h, 030h, 0FAh, 0FFh
        add    esp, 4
loc_4_10761:
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 20h]
        cmp    dword ptr [ebx + 44h], 0
        je     SHORT loc_4_1077F
        mov    eax, 8
        jmp    NEAR PTR loc_4_10835
loc_4_1077F:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_1078B
        mov    eax, dword ptr [ebx + 18h]
        mov    dword ptr [esi], eax
        jmp    SHORT loc_4_10795
loc_4_1078B:
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 28h], eax
        mov    byte ptr [esi + 4], 0
loc_4_10795:
        mov    eax, dword ptr [ebx + 20h]
        cmp    dword ptr [ebx + 1ch], eax
        jne    NEAR PTR loc_4_1082C
        test   byte ptr [esi + 4], 1
        jne    SHORT loc_4_1081B
        cmp    dword ptr [esi + 28h], 0
        je     SHORT loc_4_1081B
        mov    dword ptr [ebx + 44h], esi
        push   ebx
        push   esi
        call   sub_4_10545
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_107C4
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_10835
loc_4_107C4:
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_4_107D0
        or     byte ptr [ebx + 48h], 2
        jmp    SHORT loc_4_10814
loc_4_107D0:
        mov    word ptr [esi + 1ah], 0fffdh
        cmp    dword ptr [esi + 28h], -1
        je     SHORT loc_4_10814
        or     byte ptr [ebx + 48h], 1
        lea    ecx, [ebx + 2eh]
        mov    edi, ecx
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        stosd
        stosw
        mov    dword ptr [ebx + 3eh], ebx
        db  0C7h, 043h, 03Ah, 027h, 0E7h, 000h, 000h
        cmp    dword ptr [esi + 28h], 7fffffffh
        jbe    SHORT loc_4_10808
        mov    dword ptr [esi + 28h], 7fffffffh
loc_4_10808:
        push   dword ptr [esi + 28h]
        push   ecx
        db  0E8h, 037h, 027h, 0FAh, 0FFh
        add    esp, 8
loc_4_10814:
        mov    eax, 0fffffffdh
        jmp    SHORT loc_4_10835
loc_4_1081B:
        mov    dword ptr [esi + 28h], 0
        mov    dword ptr [esi], 0
        sub    eax, eax
        jmp    SHORT loc_4_10835
loc_4_1082C:
        push   esi
        call   sub_4_1083A
        add    esp, 4
loc_4_10835:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_1083A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 2]
        mov    edi, dword ptr [esi + 20h]
        push   2
        push   eax
        push   dword ptr [edi + 1ch]
        push   edi
        call   sub_4_108B6
        add    esp, 10h
        movzx  ecx, word ptr [ebp - 2]
        cmp    dword ptr [esi], ecx
        jae    SHORT loc_4_10870
        mov    word ptr [esi + 1ah], 6fh
        mov    eax, 6fh
        jmp    SHORT loc_4_108AF
loc_4_10870:
        push   ecx
        push   dword ptr [esi + 14h]
        push   eax
        push   edi
        call   sub_4_108B6
        add    esp, 10h
        mov    ebx, eax
        movzx  eax, word ptr [ebp - 2]
        mov    dword ptr [esi], eax
        cmp    dword ptr [edi + 20h], ebx
        jne    SHORT loc_4_10894
        mov    dword ptr [esi + 28h], 0
        jmp    SHORT loc_4_108A4
loc_4_10894:
        push   2
        lea    eax, [esi + 28h]
        push   eax
        push   ebx
        push   edi
        call   sub_4_108B6
        add    esp, 10h
loc_4_108A4:
        test   byte ptr [esi + 4], 1
        jne    SHORT loc_4_108AD
        mov    dword ptr [edi + 1ch], ebx
loc_4_108AD:
        sub    eax, eax
loc_4_108AF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_108B6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [edx + 14h]
        sub    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, eax
        jb     SHORT loc_4_108CE
        mov    ebx, eax
loc_4_108CE:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_4_108EB
        mov    esi, dword ptr [edx + 10h]
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, ebx
        shr    ecx, 2
        add    esi, dword ptr [ebp + 0ch]
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_4_108EB:
        cmp    ebx, eax
        jae    SHORT loc_4_1090F
        sub    eax, ebx
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_4_1091B
        mov    edi, dword ptr [ebp + 10h]
        add    edi, ebx
        mov    esi, dword ptr [edx + 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_4_1091B
loc_4_1090F:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, ebx
        cmp    dword ptr [edx + 14h], eax
        ja     SHORT loc_4_1091B
        sub    eax, eax
loc_4_1091B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_10920:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [eax + 20h]
        cmp    edx, ecx
        jbe    SHORT loc_4_1093A
        sub    dx, cx
        dec    dx
        mov    ax, dx
        jmp    SHORT loc_4_10945
loc_4_1093A:
        mov    eax, dword ptr [eax + 14h]
        sub    ax, cx
        add    ax, dx
        dec    ax
loc_4_10945:
        pop    ebp
        ret
sub_4_10947:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 20h]
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 2ch], eax
        mov    eax, dword ptr [edi + 14h]
        dec    eax
        mov    dword ptr [esi + 24h], eax
        mov    ax, word ptr [edi + 49h]
        test   al, 1
        je     SHORT loc_4_1097C
        test   al, 4
        je     SHORT loc_4_1097C
        mov    dword ptr [esi], 0
        jmp    SHORT loc_4_10981
loc_4_1097C:
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [esi], eax
loc_4_10981:
        mov    eax, dword ptr [edi + 1ch]
        cmp    dword ptr [edi + 20h], eax
        jne    SHORT loc_4_109A3
        mov    word ptr [esi + 18h], 0
        mov    ax, word ptr [edi + 49h]
        and    ax, 1
        cmp    ax, 1
        cmc
        sbb    eax, eax
        mov    dword ptr [esi + 28h], eax
        jmp    SHORT loc_4_109EC
loc_4_109A3:
        push   2
        lea    ecx, [ebp - 2]
        push   ecx
        push   eax
        push   edi
        call   sub_4_108B6
        add    esp, 10h
        movzx  ecx, word ptr [ebp - 2]
        mov    dword ptr [esi + 28h], ecx
        mov    ebx, 1
loc_4_109BF:
        movzx  ecx, word ptr [ebp - 2]
        push   ecx
        push   0
        push   eax
        push   edi
        call   sub_4_108B6
        add    esp, 10h
        cmp    dword ptr [edi + 20h], eax
        je     SHORT loc_4_109E8
        push   2
        lea    ecx, [ebp - 2]
        push   ecx
        push   eax
        push   edi
        call   sub_4_108B6
        add    esp, 10h
        inc    ebx
        jmp    SHORT loc_4_109BF
loc_4_109E8:
        mov    word ptr [esi + 18h], bx
loc_4_109EC:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_109F5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 058h, 06Fh, 000h, 000h
        db  03Dh, 058h, 06Fh, 000h, 000h
        je     SHORT loc_4_10A2D
        mov    edx, dword ptr [ebp + 8]
loc_4_10A0A:
        lea    esi, [eax + 5fh]
        movzx  ebx, word ptr [edx]
        mov    ecx, ebx
        shr    ecx, 2
        mov    edi, edx
        repe cmpsd
        jne    SHORT loc_4_10A22
        mov    ecx, ebx
        and    ecx, 3
        repe cmpsb
loc_4_10A22:
        je     SHORT loc_4_10A2F
        mov    eax, dword ptr [eax]
        db  03Dh, 058h, 06Fh, 000h, 000h
        jne    SHORT loc_4_10A0A
loc_4_10A2D:
        sub    eax, eax
loc_4_10A2F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C43D
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_10A9A
        mov    byte ptr [esi + 4], 22h
        test   byte ptr [esi + 8bh], 4
        mov    ax, word ptr [esi + 0ach]
        mov    word ptr [esi + 18h], ax
        je     SHORT loc_4_10A74
        mov    eax, dword ptr [esi + 9ch]
        jmp    SHORT loc_4_10A81
loc_4_10A74:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_10A81:
        mov    dword ptr [esi + 14h], eax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0fh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 18h]
        db  0E8h, 0F9h, 030h, 0FAh, 0FFh
        add    esp, 10h
loc_4_10A9A:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C43D
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_10ADA
        mov    byte ptr [esi + 4], 21h
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0fh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 18h]
        db  0E8h, 0C8h, 030h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_10ADA
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [esi + 0b4h], ax
loc_4_10ADA:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C43D
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_10B16
        mov    byte ptr [esi + 4], 1
        mov    ax, word ptr [esi + 0b0h]
        mov    word ptr [esi + 18h], ax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0fh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 18h]
        db  0E8h, 07Dh, 030h, 0FAh, 0FFh
        add    esp, 10h
loc_4_10B16:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C43D
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_10B99
        mov    byte ptr [esi + 4], 23h
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_10B42
        mov    eax, dword ptr [esi + 9ch]
        jmp    SHORT loc_4_10B4F
loc_4_10B42:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_10B4F:
        mov    dword ptr [esi + 14h], eax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0fh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 18h]
        db  0E8h, 02Bh, 030h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_10B74
        cmp    word ptr [esi + 1ah], 0eah
        jne    SHORT loc_4_10B99
loc_4_10B74:
        mov    eax, dword ptr [esi]
        mov    dword ptr [esi + 0a8h], eax
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [esi + 0b0h], eax
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [esi + 0ach], eax
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [esi + 98h], ax
loc_4_10B99:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C43D
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_10C38
        mov    byte ptr [esi + 4], 26h
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_10BC9
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_10BD6
loc_4_10BC9:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_10BD6:
        mov    dword ptr [esi + 14h], eax
        movzx  eax, word ptr [esi + 0ach]
        mov    dword ptr [esi], eax
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_10BF3
        mov    eax, dword ptr [esi + 9ch]
        jmp    SHORT loc_4_10C00
loc_4_10BF3:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_10C00:
        mov    dword ptr [esi + 10h], eax
        movzx  eax, word ptr [esi + 0b0h]
        mov    dword ptr [esi + 24h], eax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0fh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 18h]
        db  0E8h, 070h, 02Fh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_10C2F
        cmp    word ptr [esi + 1ah], 0eah
        jne    SHORT loc_4_10C38
loc_4_10C2F:
        mov    eax, dword ptr [esi]
        mov    word ptr [esi + 0b0h], ax
loc_4_10C38:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 8bh]
        mov    cl, al
        and    cl, 4
        je     SHORT loc_4_10C77
        or     al, 40h
        mov    byte ptr [esi + 8bh], al
        push   1ch
        push   dword ptr [esi + 98h]
        lea    eax, [ebp - 34h]
        push   eax
        call   sub_4_6220
        add    esp, 0ch
        lea    edi, [esi + 14h]
        mov    eax, dword ptr [ebp - 1ch]
        jmp    SHORT loc_4_10CBB
loc_4_10C77:
        or     cl, cl
        je     SHORT loc_4_10C83
        mov    eax, dword ptr [esi + 9ch]
        jmp    SHORT loc_4_10C90
loc_4_10C83:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_10C90:
        push   18h
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_4_6220
        add    esp, 0ch
        test   byte ptr [esi + 8bh], 4
        lea    edi, [esi + 14h]
        je     SHORT loc_4_10CB0
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_4_10CBB
loc_4_10CB0:
        push   dword ptr [ebp - 4]
        call   sub_4_21A2
        add    esp, 4
loc_4_10CBB:
        mov    dword ptr [edi], eax
        push   0
        push   esi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_10D7A
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_10CF4
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [esi + 28h], eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [ebp - 20h]
        jmp    SHORT loc_4_10D3B
loc_4_10CF4:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 28h], eax
        movzx  eax, word ptr [ebp - 10h]
        mov    dword ptr [esi], eax
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_10D0E
        mov    eax, dword ptr [ebp - 0eh]
        jmp    SHORT loc_4_10D19
loc_4_10D0E:
        push   dword ptr [ebp - 0eh]
        call   sub_4_21A2
        add    esp, 4
loc_4_10D19:
        mov    dword ptr [edi], eax
        movzx  eax, word ptr [ebp - 0ah]
        mov    dword ptr [esi + 24h], eax
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_10D30
        mov    eax, dword ptr [ebp - 8]
        jmp    SHORT loc_4_10D3B
loc_4_10D30:
        push   dword ptr [ebp - 8]
        call   sub_4_21A2
        add    esp, 4
loc_4_10D3B:
        mov    dword ptr [esi + 10h], eax
        mov    byte ptr [esi + 4], 54h
        mov    dword ptr [esi + 20h], 0
        mov    word ptr [esi + 18h], 0
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   29h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 2ch]
        db  0E8h, 02Eh, 02Eh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_10D71
        cmp    word ptr [esi + 1ah], 0eah
        jne    SHORT loc_4_10D7A
loc_4_10D71:
        mov    eax, dword ptr [esi]
        mov    word ptr [esi + 0b0h], ax
loc_4_10D7A:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 8bh]
        test   al, 4
        je     SHORT loc_4_10DB5
        or     al, 40h
        mov    byte ptr [esi + 8bh], al
        test   al, 4
        je     SHORT loc_4_10DA5
        mov    eax, dword ptr [esi + 0ach]
        jmp    SHORT loc_4_10DB2
loc_4_10DA5:
        push   3c14h
        call   sub_4_2154
        add    esp, 4
loc_4_10DB2:
        mov    dword ptr [esi + 14h], eax
loc_4_10DB5:
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_4_10E15
        cmp    byte ptr [eax], 5ch
        jne    SHORT loc_4_10E15
        cmp    byte ptr [eax + 1], 5ch
        jne    SHORT loc_4_10E15
        push   0
        push   esi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_10E1B
        mov    byte ptr [esi + 4], 53h
        mov    dword ptr [esi + 20h], 0
        movzx  eax, word ptr [esi + 0a8h]
        shl    eax, 10h
        movzx  ecx, word ptr [esi + 0b0h]
        add    eax, ecx
        mov    dword ptr [esi + 28h], eax
        mov    word ptr [esi + 18h], 0
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   29h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 2ch]
        db  0E8h, 080h, 02Dh, 0FAh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_4_10E1B
loc_4_10E15:
        mov    word ptr [esi + 1ah], 0fdh
loc_4_10E1B:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C43D
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_10E72
        mov    al, byte ptr [esi + 8bh]
        and    al, 4
        je     SHORT loc_4_10E45
        mov    eax, dword ptr [esi + 0ach]
        mov    dword ptr [esi + 14h], eax
loc_4_10E45:
        mov    ax, word ptr [esi + 98h]
        inc    al
        mov    byte ptr [esi + 4], al
        mov    ax, word ptr [esi + 9ch]
        mov    word ptr [esi + 18h], ax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   10h
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 1ch]
        db  0E8h, 021h, 02Dh, 0FAh, 0FFh
        add    esp, 10h
loc_4_10E72:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C43D
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_10ECA
        mov    al, byte ptr [esi + 8bh]
        and    al, 4
        je     SHORT loc_4_10E9C
        mov    eax, dword ptr [esi + 0ach]
        mov    dword ptr [esi + 14h], eax
loc_4_10E9C:
        mov    byte ptr [esi + 4], 0
        mov    ax, word ptr [esi + 9ch]
        mov    word ptr [esi + 18h], ax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   10h
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 1ch]
        db  0E8h, 0D2h, 02Ch, 0FAh, 0FFh
        add    esp, 10h
        mov    eax, dword ptr [esi]
        mov    word ptr [esi + 0b0h], ax
loc_4_10ECA:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 1
        mov    dword ptr [ebp - 10h], 0aeh
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    ebx, dword ptr [ebp - 4]
        or     ebx, ebx
        mov    ecx, dword ptr [ebp + 8]
        jne    SHORT loc_4_10F11
        mov    word ptr [ecx + 1ah], 8
        jmp    NEAR PTR loc_4_1109C
loc_4_10F11:
        test   byte ptr [ecx + 8bh], 4
        je     SHORT loc_4_10F22
        mov    eax, dword ptr [ecx + 9ch]
        jmp    SHORT loc_4_10F2F
loc_4_10F22:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_10F2F:
        push   18h
        push   eax
        lea    eax, [ebx + 78h]
        push   eax
        call   sub_4_6220
        add    esp, 0ch
        movzx  eax, word ptr [ebx + 8ah]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_C499
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_1109C
        mov    edi, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 1dh
        rep movsd
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 8bh], 4
        je     SHORT loc_4_10F76
        mov    eax, dword ptr [ebx + 7eh]
        jmp    SHORT loc_4_10F81
loc_4_10F76:
        push   dword ptr [ebx + 7eh]
        call   sub_4_21A2
        add    esp, 4
loc_4_10F81:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 90h], eax
        test   byte ptr [ecx + 8bh], 4
        jne    SHORT loc_4_10FA1
        push   dword ptr [ebx + 78h]
        call   sub_4_21A2
        add    esp, 4
        mov    dword ptr [ebx + 78h], eax
loc_4_10FA1:
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 8bh], 4
        jne    SHORT loc_4_10FC1
        push   dword ptr [ebx + 82h]
        call   sub_4_21A2
        add    esp, 4
        mov    dword ptr [ebx + 82h], eax
loc_4_10FC1:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 88h], 0b5h
        jbe    SHORT loc_4_10FD4
        or     dword ptr [ebx + 0aah], 1
loc_4_10FD4:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 90h]
        test   byte ptr [eax], 20h
        je     SHORT loc_4_10FE9
        or     dword ptr [ebx + 0aah], 2
loc_4_10FE9:
        db  0C7h, 083h, 0A0h, 000h, 000h, 000h, 0A3h, 0F0h, 000h, 000h
        mov    dword ptr [ebx + 0a4h], ebx
        db  0E8h, 016h, 01Fh, 0FAh, 0FFh
        mov    dword ptr [ebx + 9ch], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebx + 0a8h], 20h
        mov    esi, dword ptr [ecx + 74h]
        test   byte ptr [ecx + 88h], 1
        jne    SHORT loc_4_1101E
        mov    eax, dword ptr [esi]
        jmp    SHORT loc_4_11021
loc_4_1101E:
        mov    eax, dword ptr [esi + 4]
loc_4_11021:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 74h], eax
        test   byte ptr [ecx + 88h], 1
        jne    SHORT loc_4_1103F
        cmp    word ptr [ebx + 7ch], 0
        jne    SHORT loc_4_1103F
        mov    dword ptr [ebx], 0
        jmp    SHORT loc_4_11079
loc_4_1103F:
        push   ebx
        call   sub_4_110D8
        add    esp, 4
        or     eax, eax
        jl     SHORT loc_4_11082
        je     SHORT loc_4_11079
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], ax
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], ebx
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
        jmp    SHORT loc_4_1109C
loc_4_11079:
        push   ebx
        call   sub_4_11104
        add    esp, 4
loc_4_11082:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 24h]
        add    eax, dword ptr [ecx]
        mov    dword ptr [esi + 18h], eax
        test   byte ptr [ecx + 88h], 1
        je     SHORT loc_4_1109C
        and    word ptr [esi + 1ch], 0ffbfh
loc_4_1109C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     SHORT loc_4_110BD
        push   eax
        db  0E8h, 0FBh, 026h, 0FAh, 0FFh
        add    esp, 4
loc_4_110BD:
        push   esi
        call   sub_4_110D8
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_110D5
        jl     SHORT loc_4_110D5
        push   esi
        call   sub_4_11104
        add    esp, 4
loc_4_110D5:
        pop    esi
        pop    ebp
        ret
sub_4_110D8:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, word ptr [ecx + 7ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ecx + 90h]
        mov    dword ptr [ecx + 14h], eax
        lea    eax, [ecx + 94h]
        mov    dword ptr [ecx + 30h], eax
        mov    word ptr [ecx + 18h], 0
        push   ecx
        call   dword ptr [ecx + 74h]
        mov    esp, ebp
        pop    ebp
        ret
sub_4_11104:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 30h]
        or     eax, eax
        jne    SHORT loc_4_11119
        push   ecx
        db  0E8h, 0DAh, 01Dh, 0FAh, 0FFh
        jmp    SHORT loc_4_11132
loc_4_11119:
        db  0C7h, 040h, 00Ch, 036h, 0F1h, 000h, 000h
        mov    eax, dword ptr [ecx + 30h]
        mov    dword ptr [eax + 4], 0
        push   dword ptr [ecx + 30h]
        db  0E8h, 01Eh, 01Fh, 0FAh, 0FFh
loc_4_11132:
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 10h]
        mov    esi, dword ptr [ecx + 7eh]
        mov    edi, dword ptr [ecx + 8ch]
        mov    ebx, dword ptr [ecx + 86h]
        mov    eax, dword ptr [ecx + 0aah]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 78h]
        mov    word ptr [edx], ax
        mov    ax, word ptr [ecx + 1ah]
        mov    edx, dword ptr [ecx + 82h]
        mov    word ptr [edx], ax
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], ecx
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     SHORT loc_4_111A4
        push   eax
        db  0E8h, 014h, 026h, 0FAh, 0FFh
        add    esp, 4
loc_4_111A4:
        push   dword ptr [ebp - 4]
        push   edi
        push   esi
        push   ebx
        call   sub_4_231E
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C3DC
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_11229
        mov    eax, dword ptr [esi + 7ch]
        test   byte ptr [eax + 1bh], 80h
        jne    SHORT loc_4_111E1
        mov    word ptr [esi + 1ah], 1
        jmp    SHORT loc_4_11229
loc_4_111E1:
        mov    byte ptr [esi + 4], 23h
        mov    dword ptr [esi], 1
        lea    eax, [ebp - 1]
        mov    dword ptr [esi + 14h], eax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0fh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 18h]
        db  0E8h, 08Ch, 029h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_11211
        mov    dword ptr [esi], 0
loc_4_11211:
        mov    ax, word ptr [esi]
        mov    word ptr [esi + 0b0h], ax
        or     ax, ax
        je     SHORT loc_4_11229
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0ach], al
loc_4_11229:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 1ah], 1
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 087h, 025h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_11272
        mov    al, byte ptr [esi + 8bh]
        and    eax, 4
        push   eax
        push   dword ptr [esi + 84h]
        lea    eax, [esi + 98h]
        push   eax
        push   esi
        db  0E8h, 001h, 015h, 0FAh, 0FFh
        add    esp, 10h
loc_4_11272:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0a0h], 41524a44h
        jne    SHORT loc_4_1128F
        or     byte ptr [esi + 8bh], 4
loc_4_1128F:
        push   esi
        db  0E8h, 03Dh, 025h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_112F6
        movzx  eax, byte ptr [esi + 0b4h]
        sub    eax, 2
        cmp    eax, 53h
        ja     SHORT loc_4_112D6
        db  0FFh, 024h, 085h, 00Fh, 0F4h, 000h, 000h
        db  056h, 0E8h, 088h, 0B5h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh, 039h, 056h, 0E8h, 094h, 0BBh, 0FFh
        db  0FFh, 083h, 0C4h, 004h, 066h, 083h, 07Eh, 01Ah, 000h, 074h, 029h, 080h, 0BEh, 0A8h, 000h, 000h
        db  000h, 000h, 075h, 020h
loc_4_112D6:
        mov    al, byte ptr [esi + 8bh]
        and    eax, 4
        push   eax
        push   dword ptr [esi + 84h]
        lea    eax, [esi + 98h]
        push   eax
        push   esi
        db  0E8h, 07Dh, 014h, 0FAh, 0FFh
        add    esp, 10h
loc_4_112F6:
        pop    esi
        pop    ebp
        ret
        db  056h, 0E8h, 0ADh, 0BDh, 0FFh, 0FFh, 083h, 0C4h, 004h, 083h, 0F8h, 0FFh, 0EBh, 0CDh, 056h, 0E8h
        db  033h, 0B5h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh, 0E4h, 056h, 0E8h, 028h, 0F7h, 0FFh, 0FFh, 083h
        db  0C4h, 004h
loc_4_1131B:
        jmp    SHORT loc_4_112F6
        db  056h, 0E8h, 07Ah, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh, 0CEh, 056h, 0E8h, 0AFh, 0F7h, 0FFh
        db  0FFh, 083h, 0C4h, 004h, 0EBh, 0C3h, 056h, 0E8h, 0E0h, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh
        db  0B8h, 056h, 0E8h, 058h, 0F8h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh, 0ADh, 056h, 0E8h, 0ECh, 0F8h
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh, 0A2h, 056h, 0E8h, 026h, 0FAh, 0FFh, 0FFh, 083h, 0C4h, 004h
        db  0EBh, 097h, 056h, 0E8h, 0B9h, 0FAh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh, 08Ch, 056h, 0E8h, 005h
        db  0FBh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh, 081h, 056h, 0E8h, 0BCh, 006h, 000h, 000h, 083h, 0C4h
        db  004h, 0E9h, 073h, 0FFh, 0FFh, 0FFh, 056h, 0E8h, 0F5h, 0B6h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h
        db  065h, 0FFh, 0FFh, 0FFh, 056h, 0E8h, 01Ah, 0BCh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 057h, 0FFh
        db  0FFh, 0FFh, 056h, 0E8h, 007h, 0BDh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 049h, 0FFh, 0FFh, 0FFh
        db  056h, 0E8h, 06Ah, 0B8h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 03Bh, 0FFh, 0FFh, 0FFh, 056h, 0E8h
        db  06Fh, 0E6h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 02Dh, 0FFh, 0FFh, 0FFh, 056h, 0E8h, 08Ch, 0E9h
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 01Fh, 0FFh, 0FFh, 0FFh, 056h, 0E8h, 0DCh, 0E9h, 0FFh, 0FFh
        db  083h, 0C4h, 004h, 0E9h, 011h, 0FFh, 0FFh, 0FFh, 056h, 0E8h, 029h, 0EAh, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 0E9h, 003h, 0FFh, 0FFh, 0FFh, 056h, 0E8h, 0A8h, 0EAh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h
        db  0F5h, 0FEh, 0FFh, 0FFh, 056h, 0E8h, 0EDh, 0BDh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 0E7h, 0FEh
        db  0FFh, 0FFh, 0B2h, 0F2h, 000h, 000h, 0BDh, 0F2h, 000h, 000h, 0F9h, 0F2h, 000h, 000h, 007h, 0F3h
        db  000h, 000h, 0D6h, 0F2h, 000h, 000h, 0D6h, 0F2h, 000h, 000h, 0D6h, 0F2h, 000h, 000h, 0D6h, 0F2h
        db  000h, 000h, 0D6h, 0F2h, 000h, 000h, 0D6h, 0F2h, 000h, 000h, 0D6h, 0F2h, 000h, 000h, 0D6h, 0F2h
        db  000h, 000h, 0D6h, 0F2h, 000h, 000h, 0D6h, 0F2h, 000h, 000h, 0D6h, 0F2h, 000h, 000h
loc_4_1144B:
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        adc    dh, bl
        add    byte ptr [eax], al
        db  01Dh, 0F3h, 000h, 000h, 028h
        add    byte ptr [eax], al
        xor    esi, ebx
        add    byte ptr [eax], al
        add    byte ptr ds:[eax], al
        dec    ecx
        add    byte ptr [eax], al
        push   esp
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        pop    edi
        add    byte ptr [eax], al
        push   -0dh
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
loc_4_11508:
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        jne    SHORT loc_4_11508
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        xor    ebx, 0
        db  000h, 091h, 0F3h, 000h, 000h, 09Fh
        add    byte ptr [eax], al
        lodsd
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        db  0BBh, 0F3h, 000h, 000h, 0C9h
        add    byte ptr [eax], al
        xlatb
        add    byte ptr [eax], al
        in     eax, 0f3h
        add    byte ptr [eax], al
        in     eax, 0f3h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        salc
        add    byte ptr [eax], al
        add    esp, esi
        add    byte ptr [eax], al
        ret
sub_4_11560:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        push   edi
        lea    edi, [ebp - 104h]
        sub    eax, eax
        mov    ecx, 41h
        rep stosd
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0f0h], eax
        sub    esi, esi
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_C2C2
        add    esp, 4
        cmp    eax, 2
        jne    SHORT loc_4_115B8
        push   dword ptr [ebp - 0f8h]
        db  0E8h, 0BEh, 06Eh, 000h, 000h
        add    esp, 4
        mov    esi, eax
loc_4_115B8:
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 001h, 023h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0ch]
        cmp    eax, 64h
        jb     SHORT loc_4_115FB
        cmp    eax, 65h
        jbe    SHORT loc_4_11602
        cmp    eax, 66h
        je     SHORT loc_4_11666
        cmp    eax, 67h
        je     NEAR PTR loc_4_11698
        cmp    eax, -1
        je     SHORT loc_4_11656
loc_4_115FB:
        mov    eax, 57h
        jmp    SHORT loc_4_11658
loc_4_11602:
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 24h
        jbe    SHORT loc_4_11611
        mov    eax, 6fh
        jmp    SHORT loc_4_11658
loc_4_11611:
        jb     SHORT loc_4_1165F
        cmp    dword ptr [ebx + 1ch], 24h
        jb     SHORT loc_4_1165F
        lea    edi, [ebp - 24h]
        mov    esi, dword ptr [ebx + 10h]
        mov    ecx, 9
        rep movsd
        lea    eax, [ebp - 24h]
        push   eax
        db  0E8h, 051h, 038h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebx + 1ch]
        lea    esi, [ebp - 24h]
        mov    edi, dword ptr [ebx + 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 20h]
        or     eax, eax
        je     SHORT loc_4_11656
        mov    dword ptr [eax], 24h
loc_4_11656:
        sub    eax, eax
loc_4_11658:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1165F:
        mov    eax, 7ah
        jmp    SHORT loc_4_11658
loc_4_11666:
        cmp    dword ptr [ebx + 14h], 2
        jne    SHORT loc_4_11691
        cmp    dword ptr [ebx + 1ch], 4
        jne    SHORT loc_4_11691
        mov    eax, dword ptr [ebx + 10h]
        movzx  eax, word ptr [eax]
        push   eax
        call   sub_4_120F7
        add    esp, 4
        mov    ecx, dword ptr [ebx + 18h]
        or     eax, eax
        mov    dword ptr [ecx], eax
        je     SHORT loc_4_11656
        mov    eax, 6
        jmp    SHORT loc_4_11658
loc_4_11691:
        mov    eax, 57h
        jmp    SHORT loc_4_11658
loc_4_11698:
        cmp    dword ptr [ebx + 1ch], 4
        jne    SHORT loc_4_116B0
        push   dword ptr [ebx + 10h]
        call   sub_4_11560
        add    esp, 4
        mov    ecx, dword ptr [ebx + 18h]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_11656
loc_4_116B0:
        mov    eax, 6
        jmp    SHORT loc_4_11658
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 20h]
        test   byte ptr [esi + 18h], 80h
        mov    eax, dword ptr [edi + 48h]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [esi + 24h], eax
        jne    SHORT loc_4_1173D
        cmp    dword ptr [edi + 4ch], 0ffffh
        jne    SHORT loc_4_116F7
        mov    word ptr [esi + 1ah], 15h
        mov    eax, 15h
        jmp    SHORT loc_4_11762
loc_4_116F7:
        mov    ebx, dword ptr [edi + 54h]
        or     ebx, ebx
        je     SHORT loc_4_1173D
        cmp    byte ptr [ebx], 0
        je     SHORT loc_4_1173D
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [ebx + 1], 0
        movzx  eax, byte ptr [ebx]
        mov    dword ptr [esi], eax
        lea    eax, [ebx + 2]
        mov    dword ptr [esi + 14h], eax
        push   esi
        call   dword ptr [edi + 40h]
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_1172B
        mov    byte ptr [ebx], 0
loc_4_1172B:
        mov    al, 0eh
        sub    al, byte ptr [ebx]
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
loc_4_1173D:
        push   esi
        call   dword ptr [edi + 40h]
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_1175E
        db  0F6h, 005h, 0E0h, 06Fh, 000h, 000h, 002h
        je     SHORT loc_4_1175E
        mov    byte ptr [edi + 5bh], 1
        push   esi
        call   sub_4_5A5E
        add    esp, 4
loc_4_1175E:
        movsx  eax, word ptr [esi + 1ah]
loc_4_11762:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 90h]
        test   byte ptr [eax], 8
        jne    SHORT loc_4_1178A
        push   ebx
        push   esi
        call   sub_4_11886
        add    esp, 8
loc_4_1178A:
        mov    eax, dword ptr [ebx + 4ch]
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_4_11796
        sub    eax, eax
        jmp    SHORT loc_4_117FF
loc_4_11796:
        mov    byte ptr [esi + 4], 2
        mov    eax, dword ptr [ebx + 48h]
        mov    dword ptr [esi + 20h], eax
        push   esi
        call   dword ptr [ebx + 44h]
        add    esp, 4
        mov    edi, eax
        cmp    edi, -3
        jne    SHORT loc_4_117B5
        mov    eax, 0fffffffdh
        jmp    SHORT loc_4_117FF
loc_4_117B5:
        mov    eax, dword ptr [ebx + 10h]
        push   esi
        dec    word ptr [eax + 16h]
        db  0E8h, 03Ch, 031h, 0FAh, 0FFh
        add    esp, 4
        db  0C7h, 003h, 080h, 041h, 000h, 000h
        db  0C7h, 043h, 004h, 04Fh, 04Dh, 000h, 000h
        db  0C7h, 043h, 008h, 030h, 06Dh, 000h, 000h
        mov    dword ptr [ebx + 4ch], 0
        mov    dword ptr [ebx + 18h], 0
        mov    eax, dword ptr [esi + 90h]
        test   byte ptr [eax], 8
        jne    SHORT loc_4_117FD
        mov    eax, dword ptr [ebx + 54h]
        or     eax, eax
        je     SHORT loc_4_117FD
        mov    byte ptr [eax + 1], 1
loc_4_117FD:
        mov    eax, edi
loc_4_117FF:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 0ach]
        mov    ecx, dword ptr [esi + 94h]
        push   dword ptr [ecx + eax*4 + 24h]
        push   esi
        call   sub_4_11886
        add    esp, 8
        cmp    word ptr [esi + 1ah], 0
        je     SHORT loc_4_11839
        mov    byte ptr [esi + 0b5h], 8
        mov    word ptr [esi + 1ah], 0
loc_4_11839:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, ebx
        db  0A1h, 044h, 059h, 000h, 000h
        mov    di, word ptr [esi + 1ah]
        mov    word ptr [eax + 130h], 0
loc_4_11859:
        db  083h, 0BBh, 034h, 06Eh, 000h, 000h, 000h
        je     SHORT loc_4_11875
        mov    eax, dword ptr [esi + 94h]
        push   dword ptr [ebx + eax + 24h]
        push   esi
        call   sub_4_11886
        add    esp, 8
loc_4_11875:
        add    ebx, 4
        cmp    ebx, 0ch
        jl     SHORT loc_4_11859
        mov    word ptr [esi + 1ah], di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_11886:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 18h]
        mov    eax, dword ptr [esi + 7ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [eax + 54h]
        or     ebx, ebx
        je     NEAR PTR loc_4_11935
        cmp    byte ptr [ebx], 0
        je     NEAR PTR loc_4_11935
        mov    byte ptr [eax + 5bh], 0
        mov    dword ptr [esi + 74h], eax
        push   esi
        db  0E8h, 06Bh, 02Bh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_11935
        mov    byte ptr [ebx + 1], 0
        mov    eax, dword ptr [esi + 74h]
        mov    word ptr [esi + 18h], 80h
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [esi + 24h], eax
        movzx  eax, byte ptr [ebx]
        mov    dword ptr [esi], eax
        lea    eax, [ebx + 2]
        mov    dword ptr [esi + 14h], eax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   1
        push   dword ptr [eax + 4]
        db  0E8h, 09Bh, 022h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jge    SHORT loc_4_1190D
        cmp    dword ptr [esi + 30h], 0
        jne    SHORT loc_4_11935
        push   esi
        db  0E8h, 048h, 035h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
loc_4_1190D:
        jne    SHORT loc_4_1191B
        mov    ecx, dword ptr [esi + 74h]
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ecx + 18h], eax
        mov    byte ptr [ebx], 0
loc_4_1191B:
        mov    al, 0eh
        sub    al, byte ptr [ebx]
        mov    byte ptr [ebx + 1], al
        push   esi
        db  0E8h, 0D6h, 02Fh, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [esi + 18h], di
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 7ch], eax
loc_4_11935:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   -1
        lea    eax, [esi + 98h]
        push   eax
        push   esi
        db  0E8h, 01Bh, 00Eh, 0FAh, 0FFh
        add    esp, 10h
        mov    word ptr [esi + 1ah], 0
        db  0F6h, 005h, 0E0h, 06Fh, 000h, 000h, 001h
        je     SHORT loc_4_119A2
        sub    edi, edi
loc_4_11969:
        mov    eax, dword ptr [esi + 94h]
        mov    edx, dword ptr [eax + edi*4 + 24h]
        or     edx, edx
        je     SHORT loc_4_11995
        mov    ecx, edx
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [ecx + 4ch], eax
        jne    SHORT loc_4_11995
        push   edx
        push   esi
        call   sub_4_11886
        add    esp, 8
        push   edi
        push   esi
        call   sub_4_6F90
        add    esp, 8
loc_4_11995:
        inc    edi
        cmp    edi, 9
        jl     SHORT loc_4_11969
        mov    dword ptr [esi + 7ch], 0
loc_4_119A2:
        push   0
        push   esi
        call   sub_4_5E48
        add    esp, 8
        push   1
        push   dword ptr [esi + 8]
        call   sub_4_13C9E
        add    esp, 8
        push   esi
        call   sub_4_5DAE
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  ebx, word ptr [esi + 0ach]
        mov    eax, dword ptr [esi + 94h]
        mov    edi, dword ptr [eax + ebx*4 + 24h]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 4ch], eax
        push   edi
        push   esi
        call   sub_4_11886
        add    esp, 8
        push   ebx
        push   esi
        call   sub_4_6F90
        add    esp, 8
        mov    dword ptr [edi + 4ch], 0
        mov    ax, word ptr [esi + 0b4h]
        cmp    ax, 203h
        jne    SHORT loc_4_11A18
        mov    dword ptr [edi + 4ch], 0ffffh
        jmp    SHORT loc_4_11A25
loc_4_11A18:
        cmp    ax, 0bf02h
        jne    SHORT loc_4_11A25
        mov    dword ptr [edi + 4ch], 1
loc_4_11A25:
        mov    dword ptr [esi + 7ch], 0
        mov    word ptr [esi + 1ah], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    di, word ptr [esi + 0b4h]
        call   sub_4_C43D
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_11AC6
        mov    eax, dword ptr [esi + 74h]
        db  081h, 078h, 004h, 04Fh, 04Dh, 000h, 000h
        jne    SHORT loc_4_11A75
        push   esi
        call   sub_4_6D9F
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_11AC6
        movzx  eax, word ptr [esi + 0b0h]
        mov    dword ptr [esi], eax
loc_4_11A75:
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_11A86
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_11A93
loc_4_11A86:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_11A93:
        mov    dword ptr [esi + 14h], eax
        mov    word ptr [esi + 0b4h], 3
        mov    al, byte ptr [esi + 8bh]
        and    eax, 4
        push   eax
        push   dword ptr [esi + 84h]
        lea    eax, [esi + 98h]
        push   eax
        push   esi
        db  0E8h, 0B4h, 00Ch, 0FAh, 0FFh
        add    esp, 10h
        mov    word ptr [esi + 0b4h], di
loc_4_11AC6:
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  0BFh, 0C0h, 065h, 000h, 000h
        movzx  edx, word ptr [esi + 0a8h]
        mov    ecx, 8
loc_4_11AE7:
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_4_11AFD
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_4_11AFD
        test   byte ptr [eax + 1bh], cl
        je     SHORT loc_4_11AFD
        or     edx, edx
        je     SHORT loc_4_11B33
        dec    edx
loc_4_11AFD:
        add    edi, 4
        db  081h, 0FFh, 03Ch, 066h, 000h, 000h
        jbe    SHORT loc_4_11AE7
        sub    ecx, ecx
        db  0B8h, 034h, 06Eh, 000h, 000h
loc_4_11B0F:
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_11B19
        or     edx, edx
        je     SHORT loc_4_11B3E
        dec    edx
loc_4_11B19:
        add    eax, 4
        inc    ecx
        db  03Dh, 058h, 06Eh, 000h, 000h
        jb     SHORT loc_4_11B0F
        or     edx, edx
        je     SHORT loc_4_11B3E
        mov    word ptr [esi + 1ah], 57h
        jmp    NEAR PTR loc_4_11CB1
loc_4_11B33:
        mov    word ptr [esi + 1ah], 57h
        jmp    NEAR PTR loc_4_11CB1
loc_4_11B3E:
        mov    eax, dword ptr [esi + 94h]
        mov    ebx, dword ptr [eax + ecx*4 + 24h]
        movzx  eax, byte ptr [esi + 0b4h]
        cmp    eax, 2
        je     SHORT loc_4_11B7A
        cmp    eax, 3
        je     NEAR PTR loc_4_11C3F
        cmp    eax, 4
        je     NEAR PTR loc_4_11C87
        cmp    eax, 5
        je     NEAR PTR loc_4_11CA5
        mov    word ptr [esi + 1ah], 1
        jmp    NEAR PTR loc_4_11CB1
loc_4_11B7A:
        movzx  eax, word ptr [esi + 0b0h]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 40h
        jle    SHORT loc_4_11B94
        mov    word ptr [esi + 1ah], 57h
        jmp    NEAR PTR loc_4_11CB1
loc_4_11B94:
        cmp    dword ptr [ebx + 50h], 0
        je     SHORT loc_4_11BBF
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebx + 50h]
        mov    dword ptr [ebp - 14h], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 1ch], eax
loc_4_11BBF:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_11C37
        mov    dword ptr [ebp - 10h], 201h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 50h], eax
        or     eax, eax
        jne    SHORT loc_4_11C03
        mov    word ptr [esi + 1ah], 8
        mov    byte ptr [ebx + 59h], 0
        jmp    NEAR PTR loc_4_11CB1
loc_4_11C03:
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_11C14
        mov    eax, dword ptr [esi + 9ch]
        jmp    SHORT loc_4_11C21
loc_4_11C14:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_11C21:
        mov    edx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebx + 50h]
        mov    esi, eax
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_4_11C37:
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [ebx + 59h], al
        jmp    SHORT loc_4_11CB1
loc_4_11C3F:
        movzx  ax, byte ptr [ebx + 59h]
        mov    word ptr [esi + 0b0h], ax
        or     ax, ax
        je     SHORT loc_4_11CB1
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_11C61
        mov    edi, dword ptr [esi + 98h]
        jmp    SHORT loc_4_11C70
loc_4_11C61:
        push   3800h
        call   sub_4_2154
        add    esp, 4
        mov    edi, eax
loc_4_11C70:
        movzx  eax, byte ptr [ebx + 59h]
        mov    esi, dword ptr [ebx + 50h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_4_11CB1
loc_4_11C87:
        test   word ptr [esi + 0ach], 0fffeh
        db  074h, 008h
        mov    word ptr [esi + 1ah], 57h
        jmp    SHORT loc_4_11CB1
        db  08Ah, 086h, 0ACh, 000h
sub_4_11C9E:
        add    byte ptr [eax], al
        mov    byte ptr [ebx + 5ah], al
        jmp    SHORT loc_4_11CB1
loc_4_11CA5:
        movzx  ax, byte ptr [ebx + 5ah]
        mov    word ptr [esi + 0ach], ax
loc_4_11CB1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        jle    SHORT loc_4_11D3F
        mov    edi, dword ptr [ebp + 10h]
        cmp    edi, 1
        jg     SHORT loc_4_11D3F
        mov    dword ptr [ebp - 0ch], 201h
        mov    dword ptr [ebp - 8], 0ch
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp - 4]
        or     edx, edx
        jne    SHORT loc_4_11D0B
        mov    eax, 8
        jmp    SHORT loc_4_11D44
loc_4_11D0B:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], esi
        mov    eax, dword ptr [ebp + 8]
        db  089h, 042h
sub_4_11D16:
        db  008h, 08Bh, 004h, 0BDh, 024h, 070h
        add    byte ptr [eax], al
loc_4_11D1E:
        mov    ecx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_4_11D2B
        cmp    dword ptr [eax + 4], esi
        jl     SHORT loc_4_11D1E
loc_4_11D2B:
        cmp    dword ptr [ecx + 4], esi
        jne    SHORT loc_4_11D37
        mov    eax, 5
        jmp    SHORT loc_4_11D44
loc_4_11D37:
        mov    dword ptr [edx], eax
        mov    dword ptr [ecx], edx
        sub    eax, eax
        jmp    SHORT loc_4_11D44
loc_4_11D3F:
        mov    eax, 57h
loc_4_11D44:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_11D4A:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 1
        jle    SHORT loc_4_11D57
        jmp    SHORT loc_4_11D72
loc_4_11D57:
        db  08Bh, 004h, 085h, 024h, 070h, 000h, 000h
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_4_11D72
        mov    eax, dword ptr [ebp + 8]
loc_4_11D67:
        cmp    dword ptr [ecx + 4], eax
        je     SHORT loc_4_11D76
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_4_11D67
loc_4_11D72:
        sub    eax, eax
loc_4_11D74:
        pop    ebp
        ret
loc_4_11D76:
        mov    eax, dword ptr [ecx + 8]
        jmp    SHORT loc_4_11D74
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        movzx  di, byte ptr [ebx + 0b4h]
        cmp    di, 2
        ja     SHORT loc_4_11D99
        mov    di, word ptr [ebx + 0a8h]
loc_4_11D99:
        push   0
        movzx  eax, di
        push   eax
        call   sub_4_11D4A
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_4_11DB5
        mov    word ptr [ebx + 1ah], 1
        jmp    SHORT loc_4_11E2E
loc_4_11DB5:
        cmp    di, 37h
        je     SHORT loc_4_11DCD
        and    di, 0fffeh
        cmp    di, 40h
        mov    byte ptr [ebx + 0b4h], 1
        jne    SHORT loc_4_11DD4
loc_4_11DCD:
        mov    byte ptr [ebx + 0b4h], 0
loc_4_11DD4:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        cmp    ecx, 50h
        jl     SHORT loc_4_11DEB
        mov    ecx, 4fh
loc_4_11DEB:
        mov    eax, ecx
        db  08Bh, 03Dh, 044h, 059h, 000h, 000h
        add    edi, 162h
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0A1h, 044h, 059h, 000h, 000h
        add    eax, 162h
        shr    eax, 4
        mov    word ptr [ebx + 0d0h], ax
        db  0A1h, 044h, 059h, 000h, 000h
        add    eax, 162h
        and    ax, 0fh
        mov    word ptr [ebx + 98h], ax
loc_4_11E2E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 0CDh, 020h, 008h, 000h, 005h, 000h, 039h, 015h, 01Ch, 06Bh, 000h, 000h, 075h, 032h, 039h
        db  005h, 018h, 06Bh, 000h, 000h, 075h, 00Ch, 0A1h, 010h, 06Bh, 000h, 000h, 08Bh, 015h, 014h, 06Bh
        db  000h, 000h, 0C3h, 08Bh, 0C8h, 02Bh, 00Dh, 018h, 06Bh, 000h, 000h, 003h, 00Dh, 014h, 06Bh, 000h
        db  000h, 081h, 0F9h, 0D0h, 007h, 000h, 000h, 073h, 008h, 089h, 00Dh, 014h, 06Bh, 000h, 000h, 0EBh
        db  0D6h, 0A3h
loc_4_11E75:
        sbb    byte ptr [ebx], ch
        db  000h, 089h, 015h, 01Ch, 06Bh, 000h
        add    al, ch
        or     al, 0
        add    byte ptr [eax], al
        db  0A3h, 010h, 06Bh, 000h, 000h
        db  089h, 015h, 014h, 06Bh, 000h, 000h
        ret
        db  053h, 056h, 08Bh, 0DAh, 033h, 0D2h, 0B9h, 0D0h, 007h, 000h, 000h, 0F7h, 0F1h, 052h, 0B9h, 01Eh
        db  000h, 000h, 000h, 033h, 0D2h, 0F7h, 0F1h, 0B1h, 03Ch, 0F6h, 0F1h, 086h, 0C4h, 0C0h, 0E0h, 002h
        db  08Dh, 00Ch, 0C2h, 08Bh, 0C3h, 033h, 0D2h
loc_4_11EB7:
        db  0F7h, 035h, 040h, 001h, 000h, 000h
        lea    ebx, [eax*4]
        db  0BEh, 048h, 001h, 000h, 000h
        mov    byte ptr [esi + 1], 1dh
        mov    eax, 16eh
        sub    edx, eax
        jb     SHORT loc_4_11EE0
        dec    eax
loc_4_11ED7:
        inc    ebx
        sub    edx, eax
        jae    SHORT loc_4_11ED7
        mov    byte ptr [esi + 1], 1ch
loc_4_11EE0:
        add    edx, eax
        sub    eax, eax
loc_4_11EE4:
        lodsb
        sub    edx, eax
        jae    SHORT loc_4_11EE4
        lea    edx, [eax + edx + 1]
        db  081h, 0EEh, 048h, 001h, 000h, 000h
        add    ebx, ebx
        mov    dh, bl
        shl    esi, 5
        or     edx, esi
        shl    edx, 10h
        mov    eax, edx
        mov    ax, cx
        pop    edx
        pop    esi
        pop    ebx
        ret
        db  053h, 08Bh, 0D8h, 0CDh, 020h, 008h, 000h, 005h, 000h, 089h, 015h, 01Ch, 06Bh, 000h, 000h, 0A3h
        db  018h, 06Bh, 000h, 000h, 0E8h, 06Fh, 0FFh, 0FFh, 0FFh, 0A3h, 010h, 06Bh, 000h, 000h, 089h, 015h
        db  014h, 06Bh, 000h, 000h, 089h, 043h, 004h, 08Bh, 0C2h, 0B1h, 00Ah, 0F6h, 0F1h, 088h, 043h, 008h
        db  0B9h, 000h, 05Ch, 026h, 005h, 08Bh, 015h, 01Ch, 06Bh, 000h, 000h, 0A1h, 018h, 06Bh, 000h, 000h
        db  005h, 0CFh, 007h, 000h, 000h, 03Bh, 0C1h, 072h, 003h, 02Bh, 0C1h, 042h, 0E8h, 037h, 0FFh, 0FFh
        db  0FFh, 089h, 003h, 05Bh, 0C3h
sub_4_11F5D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_4_11F7B
        mov    ecx, 989680h
        mul    ecx
        add    eax, 0d53e8000h
        adc    edx, 19db1deh
        leave
        ret
loc_4_11F7B:
        mov    edx, eax
        leave
        ret
sub_4_11F7F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, eax
        or     ecx, edx
        je     SHORT loc_4_11FA8
        sub    eax, 0d53e8000h
        sbb    edx, 19db1deh
        jb     SHORT loc_4_11FA8
        mov    ecx, 989680h
        cmp    edx, ecx
        jae    SHORT loc_4_11FA8
        div    ecx
        leave
        ret
loc_4_11FA8:
        sub    eax, eax
        sub    edx, edx
        leave
        ret
sub_4_11FAE:
        db  0CDh, 020h, 006h, 000h, 040h, 000h
        ret
sub_4_11FB5:
        push   ebp
        mov    ebp, esp
        db  0E8h, 0AEh, 064h, 000h, 000h
        neg    eax
        mov    edx, 23c34600h
        imul   edx
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_4_11FD4
        cmp    dword ptr [ecx + 4], 0
        je     SHORT loc_4_11FFA
loc_4_11FD4:
        add    eax, dword ptr [ecx]
        adc    edx, dword ptr [ecx + 4]
        cmp    edx, 1a8e79fh
        jb     SHORT loc_4_11FFA
        jne    SHORT loc_4_11FEA
        cmp    eax, 0e1d58000h
        jb     SHORT loc_4_11FFA
loc_4_11FEA:
        cmp    edx, 23868b8h
        jne    SHORT loc_4_11FFB
        cmp    eax, 66d29300h
        ja     SHORT loc_4_11FFB
        clc
loc_4_11FFA:
        cmc
loc_4_11FFB:
        sbb    eax, eax
        leave
        ret
        db  000h, 0C3h
sub_4_12001:
        push   edi
        xor    eax, eax
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        or     ecx, ecx
        je     SHORT loc_4_1201C
        mov    edx, edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        cmp    edx, edi
loc_4_12018:
        jne    SHORT loc_4_1201C
        mov    eax, ecx
loc_4_1201C:
        pop    edi
        ret
sub_4_1201E:
        push   edi
        xor    eax, eax
        mov    edi, dword ptr [esp + 8]
        mov    ecx, 104h
        mov    edx, edi
        repne scasw
        sub    edi, edx
        lea    eax, [edi - 2]
        pop    edi
        ret
        db  0CDh, 020h, 000h, 000h, 02Ah, 000h, 072h, 00Fh
loc_4_1203E:
        mov    eax, dword ptr [esp + 4]
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
        lea    esp, [esp + 4]
        ret
        db  0CDh, 020h, 000h, 000h, 02Ah, 000h, 072h, 00Fh, 08Bh, 044h, 024h, 004h, 050h, 0CDh, 020h, 010h
        db  000h, 02Ah, 000h, 08Dh, 064h, 024h
loc_4_12064:
        add    al, 0c3h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        pop    ebx
        sete   al
        movzx  eax, al
        ret
        db  033h, 0C0h, 039h, 005h, 078h, 059h, 000h, 000h, 075h, 036h, 0CDh, 020h, 000h, 000h, 02Ah, 000h
        db  072h, 035h, 06Ah, 004h, 06Ah, 0FFh, 06Ah, 000h, 068h, 0DAh, 05Ah, 001h, 000h, 0CDh, 020h, 019h
        db  000h, 02Ah, 000h, 08Dh, 064h, 024h, 010h, 0A3h, 078h, 059h, 000h, 000h, 074h, 009h, 083h, 025h
        db  07Ch, 059h, 000h, 000h, 0FDh, 0EBh, 010h, 083h, 00Dh, 07Ch, 059h, 000h, 000h, 002h, 0EBh, 007h
        db  083h, 00Dh, 07Ch, 059h, 000h, 000h, 001h, 0C3h, 0CDh, 020h, 0CFh, 000h, 001h, 000h, 0C3h
sub_4_120CA:
        db  0CDh, 020h, 000h, 000h, 041h, 000h
        jb     SHORT loc_4_120F4
        db  0CDh, 020h, 001h, 000h, 041h, 000h
        cmp    word ptr [eax], 4
        jne    SHORT loc_4_120F4
        mov    ecx, dword ptr [esp + 4]
        cmp    ecx, 1ah
        jae    SHORT loc_4_120F4
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [eax + ecx*2 + 14h]
        test   byte ptr [ecx + 1], 20h
        jne    SHORT loc_4_120F6
loc_4_120F4:
        xor    eax, eax
loc_4_120F6:
        ret
sub_4_120F7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        sub    esp, 104h
        mov    edi, esp
        movzx  edx, word ptr [ebp + 8]
        push   edx
        push   edi
        call   sub_4_2751
        db  0A1h, 09Ch, 065h, 000h, 000h
        movzx  eax, word ptr [eax + 10h]
        shl    eax, 4
        mov    dword ptr [edi + 80h], eax
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  003h, 01Dh, 0B4h, 055h, 000h, 000h
        mov    dword ptr [edi + 94h], ebx
        call   sub_4_C36E
        cmp    eax, 1
        jae    SHORT loc_4_1214C
        mov    eax, dword ptr [edi + 74h]
        mov    eax, dword ptr [eax + 10h]
        xor    eax, 0a5a5a5a5h
        rol    eax, 0dh
        stc
loc_4_1214C:
        sbb    ecx, ecx
        and    eax, ecx
        add    esp, 10ch
        pop    edi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0D3h, 0CDh, 020h, 008h, 000h, 00Dh, 000h, 0F6h, 0C4h, 040h, 075h, 00Dh, 0CDh, 020h, 006h
        db  000h, 00Dh, 000h, 092h, 0CDh, 020h, 005h, 000h, 00Dh, 000h, 0CDh, 020h, 004h, 000h, 001h, 000h
        db  074h, 01Ch, 0A1h, 0B4h, 055h, 000h, 000h, 0F6h, 044h, 003h, 008h, 020h, 074h, 010h, 08Bh, 015h
        db  044h, 059h, 000h, 000h, 003h
loc_4_1218F:
        push   ebx
        add    al, 80h
        mov    ch, byte ptr [esi]
        add    dword ptr [eax], eax
        add    byte ptr [edx], al
        ret
sub_4_12199:
        push   ebx
        sub    esp, 12h
        mov    ebx, esp
        db  00Fh, 0B7h, 005h, 022h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebx + 0eh], eax
        db  00Fh, 0B7h, 005h, 020h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebx + 0ah], eax
        mov    word ptr [ebx + 8], 3
        mov    word ptr [ebx + 6], 8004h
        mov    word ptr [ebx + 4], 0
        db  0CDh, 020h, 01Ah, 000h, 02Ah, 000h
        add    esp, 12h
        or     eax, eax
        je     SHORT loc_4_121EA
        cmp    eax, 1
        mov    eax, 0
        je     SHORT loc_4_121EA
        not    eax
loc_4_121EA:
        pop    ebx
        ret
        db  00Fh, 0B3h, 015h, 024h, 06Eh, 000h, 000h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_4_12200:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        movzx  edi, byte ptr [ebp + 0ch]
        push   1
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_4336
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_1222B
        push   0
        push   edi
        call   sub_4_4114
        add    esp, 8
        mov    word ptr [esi + 1ah], ax
loc_4_1222B:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_12233:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        mov    dword ptr [ebp - 4], 1
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, 8041h
        je     SHORT loc_4_12258
        cmp    ebx, 8044h
        jne    SHORT loc_4_1225D
loc_4_12258:
        mov    esi, 1
loc_4_1225D:
        mov    dword ptr [ebp - 1ch], 14h
        db  0E8h, 0D7h, 063h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        mov    al, byte ptr [ebp + 14h]
        mov    byte ptr [ebp - 0ch], al
loc_4_12272:
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 9], 0
        mov    al, byte ptr [edi + 0b0h]
        cmp    al, 4bh
        je     SHORT loc_4_1228F
        cmp    al, 6bh
loc_4_12285:
        je     SHORT loc_4_1228F
        mov    eax, dword ptr [edi + 7ch]
        mov    al, byte ptr [eax + 14h]
        jmp    SHORT loc_4_12299
loc_4_1228F:
        mov    byte ptr [ebp - 9], 1
        mov    al, byte ptr [edi + 0a8h]
loc_4_12299:
        mov    byte ptr [ebp - 0ah], al
        mov    al, byte ptr [ebp + 10h]
        dec    al
        mov    byte ptr [ebp - 0bh], al
        lea    eax, [ebp - 1ch]
        push   eax
        push   ebx
        push   219h
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        db  0E8h, 038h, 063h, 000h, 000h
        add    esp, 14h
        cmp    eax, -1
        jne    SHORT loc_4_122C2
        sub    eax, eax
loc_4_122C2:
        cmp    esi, 1
        jne    SHORT loc_4_122D8
        or     eax, eax
        jne    SHORT loc_4_122D8
        mov    word ptr [edi + 1ah], 2ah
        mov    eax, 2ah
        jmp    SHORT loc_4_122DA
loc_4_122D8:
        sub    eax, eax
loc_4_122DA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_122E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [ebp + 0ch], 1
        mov    ebx, dword ptr [esi + 74h]
        je     SHORT loc_4_1231A
        mov    byte ptr [esi + 4], 1
        push   0
        push   esi
        mov    eax, dword ptr [ebx + 8]
        push   11h
        push   dword ptr [eax + 20h]
        db  0E8h, 086h, 018h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_1231A
        movsx  eax, word ptr [esi + 1ah]
        jmp    NEAR PTR loc_4_123AB
loc_4_1231A:
        mov    eax, dword ptr [esi + 0ch]
        movzx  eax, word ptr [eax]
        add    eax, 2
        mov    dword ptr [ebp - 4], eax
        test   byte ptr [ebx + 1fh], 60h
        jne    SHORT loc_4_12339
        test   byte ptr [ebx + 23h], 10h
        jne    SHORT loc_4_12339
        push   201h
        jmp    SHORT loc_4_1233E
loc_4_12339:
        push   1
        mov    eax, dword ptr [ebp - 4]
loc_4_1233E:
        add    eax, 16h
        push   eax
        db  0E8h, 0B1h, 062h, 000h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_1235B
        mov    word ptr [esi + 1ah], 8
        mov    eax, 8
        jmp    SHORT loc_4_123AB
loc_4_1235B:
        mov    dword ptr [eax + 0ch], 0
        mov    word ptr [eax + 10h], 0
        mov    word ptr [eax + 12h], 0
        mov    edx, dword ptr [ebp - 4]
        lea    edi, [eax + 14h]
        mov    esi, dword ptr [esi + 0ch]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    dword ptr [ebx + 24h], eax
        test   byte ptr [ebp + 0ch], 2
        je     SHORT loc_4_123A9
        mov    edi, eax
        mov    esi, ebx
        movsd
        movsd
        movsd
        db  0C7h, 003h, 02Dh, 049h, 000h, 000h
        db  0C7h, 043h, 004h, 040h, 049h, 000h, 000h
        db  0C7h, 043h, 008h, 050h, 072h, 000h, 000h
loc_4_123A9:
        sub    eax, eax
loc_4_123AB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_123B2:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 74h]
        sub    eax, eax
        movzx  esi, word ptr [edx + 30h]
        db  03Bh, 035h, 0D0h, 066h, 000h, 000h
        jg     SHORT loc_4_12403
        mov    ecx, dword ptr [ecx + 94h]
        cmp    dword ptr [ecx + esi*4 + 0d4h], edx
        jne    SHORT loc_4_123DB
        jmp    SHORT loc_4_123FE
loc_4_123DB:
        db  08Bh, 00Dh, 088h, 065h, 000h, 000h
        db  081h, 0F9h, 088h, 065h, 000h, 000h
        je     SHORT loc_4_12403
loc_4_123E9:
        cmp    dword ptr [ecx + esi*4 + 0d4h], edx
        je     SHORT loc_4_123FE
        mov    ecx, dword ptr [ecx]
        db  081h, 0F9h, 088h, 065h, 000h, 000h
        jne    SHORT loc_4_123E9
        jmp    SHORT loc_4_12403
loc_4_123FE:
        mov    eax, 1
loc_4_12403:
        pop    esi
        pop    ebp
        ret
sub_4_12406:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 74h]
        mov    ebx, dword ptr [edi + 14h]
        or     ebx, ebx
        je     SHORT loc_4_12437
        push   esi
        call   sub_4_123B2
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_124A4
        movzx  ecx, word ptr [ebx + 2fh]
        movzx  eax, word ptr [ebx + 31h]
        add    eax, ecx
        shl    ecx, 10h
        or     eax, ecx
        jmp    SHORT loc_4_124A7
loc_4_12437:
        mov    ax, word ptr [edi + 32h]
        or     ax, ax
        je     SHORT loc_4_1246A
        movzx  ecx, ax
        sub    ecx, 200h
        mov    eax, ecx
        and    eax, 0ffffff3fh
        sar    eax, 6
        db  08Bh, 080h, 0E0h, 066h, 000h, 000h
        and    ecx, 0ffh
        mov    eax, dword ptr [eax + ecx*8 + 4]
        mov    dword ptr [esi + 8], eax
        or     eax, eax
        jne    SHORT loc_4_124AA
loc_4_1246A:
        movzx  ecx, word ptr [edi + 30h]
        sub    ecx, 200h
        mov    eax, ecx
        and    eax, 0ffffff3fh
        sar    eax, 6
        db  005h, 0E0h, 066h, 000h, 000h
        and    ecx, 0ffh
        lea    edx, [ecx*8]
        mov    ecx, dword ptr [eax]
        mov    ecx, dword ptr [edx + ecx + 4]
        mov    dword ptr [esi + 8], ecx
        or     ecx, ecx
        je     SHORT loc_4_124A4
        mov    eax, dword ptr [eax]
        cmp    dword ptr [edx + eax], edi
        je     SHORT loc_4_124AA
loc_4_124A4:
        mov    eax, dword ptr [edi + 34h]
loc_4_124A7:
        mov    dword ptr [esi + 8], eax
loc_4_124AA:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_124AF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 74h]
        test   byte ptr [edi + 23h], 10h
        je     SHORT loc_4_124FB
        mov    eax, dword ptr [esi + 7ch]
        push   1
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 20h], eax
        push   esi
        call   sub_4_122E1
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_124EB
        movsx  eax, word ptr [esi + 1ah]
        jmp    NEAR PTR loc_4_125A8
loc_4_124EB:
        push   0
        push   edi
        db  0E8h, 005h, 03Eh, 0FAh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_4_125A6
loc_4_124FB:
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [esi + 78h], eax
        mov    ax, word ptr [edi + 30h]
        mov    word ptr [esi + 6], ax
        mov    ebx, dword ptr [esi + 8]
        push   esi
        call   sub_4_12406
        add    esp, 4
        cmp    dword ptr [edi + 0ch], edi
        jne    SHORT loc_4_12520
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 4ch], eax
loc_4_12520:
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [edi + 1fh]
        test   al, 3
        je     SHORT loc_4_1257C
        mov    eax, dword ptr [esi + 7ch]
        push   0
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    word ptr [esi + 18h], 0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0ch
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 0ch]
        db  0E8h, 03Bh, 016h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_12562
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_125A8
loc_4_12562:
        or     word ptr [edi + 1ch], 40h
        push   3
        push   esi
        call   sub_4_122E1
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_1259D
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_125A8
loc_4_1257C:
        test   al, 0ch
        je     SHORT loc_4_1259D
        mov    eax, dword ptr [esi + 0ch]
        push   2
        mov    word ptr [eax], 0
        push   esi
        call   sub_4_122E1
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_1259D
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_125A8
loc_4_1259D:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 4], al
        mov    dword ptr [esi + 8], ebx
loc_4_125A6:
        sub    eax, eax
loc_4_125A8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_125AF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        mov    eax, dword ptr [edi + 7ch]
        mov    esi, dword ptr [eax + 24h]
        db  0E8h, 04Eh, 013h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_125D1
        movsx  eax, word ptr [edi + 1ah]
        jmp    SHORT loc_4_12612
loc_4_125D1:
        or     esi, esi
        je     SHORT loc_4_12601
        mov    ebx, 0ffffh
loc_4_125DA:
        cmp    word ptr [esi + 20h], bx
        jne    SHORT loc_4_125E2
        jmp    SHORT loc_4_125FA
loc_4_125E2:
        test   byte ptr [esi + 3ch], 1
        je     SHORT loc_4_125EA
        jmp    SHORT loc_4_125FA
loc_4_125EA:
        mov    dword ptr [edi + 74h], esi
        push   edi
        call   sub_4_124AF
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_12617
loc_4_125FA:
        mov    esi, dword ptr [esi + 2ch]
        or     esi, esi
        jne    SHORT loc_4_125DA
loc_4_12601:
        push   edi
        db  0E8h, 071h, 013h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [edi + 1ah], 0
        sub    eax, eax
loc_4_12612:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_12617:
        push   edi
        db  0E8h, 05Bh, 013h, 0FAh, 0FFh
        add    esp, 4
        movsx  eax, word ptr [edi + 1ah]
        jmp    SHORT loc_4_12612
sub_4_12626:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 24h]
        or     eax, eax
        mov    dword ptr [ebp - 4], eax
        je     SHORT loc_4_1269B
        test   byte ptr [ebx + 23h], 10h
        je     SHORT loc_4_1264F
        push   1
        push   ebx
        db  0E8h, 0AEh, 03Ch, 0FAh, 0FFh
        add    esp, 8
        jmp    SHORT loc_4_12672
loc_4_1264F:
        db  081h, 03Bh, 02Dh, 049h, 000h, 000h
        jne    SHORT loc_4_12672
        test   byte ptr [ebx + 1fh], 20h
        je     SHORT loc_4_1266A
        push   1
        push   ebx
        db  0E8h, 093h, 03Ch, 0FAh, 0FFh
        add    esp, 8
        jmp    SHORT loc_4_12672
loc_4_1266A:
        mov    edi, ebx
        mov    esi, dword ptr [ebp - 4]
        movsd
        movsd
        movsd
loc_4_12672:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 12h
        mov    dword ptr [ebx + 24h], 0
        cmp    word ptr [eax], 0
        je     SHORT loc_4_1268E
        push   eax
        db  0E8h, 066h, 008h, 0FAh, 0FFh
        add    esp, 4
loc_4_1268E:
        push   0
        push   dword ptr [ebp - 4]
        db  0E8h, 068h, 05Fh, 000h, 000h
        add    esp, 8
loc_4_1269B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_126A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ecx + 7ch]
        mov    dword ptr [ebp - 10h], eax
        mov    ebx, dword ptr [eax + 24h]
        mov    byte ptr [ebp - 1], 0
        push   ecx
        db  0E8h, 04Ah, 012h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_126D6
        mov    dword ptr [ebp - 0ch], 1
loc_4_126D6:
        mov    ecx, dword ptr [ebp + 8]
        or     ebx, ebx
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 18h], eax
        je     NEAR PTR loc_4_128A5
loc_4_126E7:
        mov    eax, dword ptr [ebx + 2ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 7ch]
        mov    dword ptr [ebp - 14h], eax
        cmp    word ptr [ebx + 20h], 0ffffh
        je     NEAR PTR loc_4_12891
        test   byte ptr [ebx + 23h], 10h
        je     SHORT loc_4_12711
        or     byte ptr [ebx + 1eh], 4
        jmp    NEAR PTR loc_4_12891
loc_4_12711:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 74h], ebx
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx + 20h], eax
        mov    ax, word ptr [ebx + 30h]
        mov    word ptr [ecx + 6], ax
        push   ecx
        call   sub_4_12406
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 8]
        cmp    dword ptr [eax + 28h], ecx
        je     NEAR PTR loc_4_12891
        mov    al, byte ptr [ebx + 1fh]
        test   al, 60h
        je     SHORT loc_4_12779
        test   byte ptr [ebx + 1ch], 40h
        jne    SHORT loc_4_12770
        push   2
        push   dword ptr [ebp + 8]
        db  0E8h, 02Ch, 040h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_1276C
        or     word ptr [ebx + 1ch], 40h
loc_4_1276C:
        or     byte ptr [ebp - 1], 10h
loc_4_12770:
        or     byte ptr [ebx + 1eh], 4
        jmp    NEAR PTR loc_4_12891
loc_4_12779:
        test   al, 8
        jne    NEAR PTR loc_4_12891
        cmp    dword ptr [ebx + 24h], 0
        je     NEAR PTR loc_4_12891
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 0
        mov    cl, byte ptr [ebx + 1fh]
        test   cl, 3
        je     SHORT loc_4_12807
        mov    eax, dword ptr [ebx + 24h]
        test   byte ptr [eax + 10h], 1
        jne    SHORT loc_4_12807
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 4], 2
        mov    eax, dword ptr [ecx + 7ch]
        test   byte ptr [eax + 32h], 20h
        jne    SHORT loc_4_127BA
        or     word ptr [ecx + 18h], 1
loc_4_127BA:
        mov    ecx, dword ptr [ebp + 8]
        push   1
        or     word ptr [ecx + 18h], 2
        push   ecx
        db  0E8h, 0C2h, 03Fh, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_127D6
        jmp    NEAR PTR loc_4_1287D
loc_4_127D6:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 24h]
        mov    ecx, dword ptr [ebx + 24h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebx + 24h]
        or     word ptr [eax + 10h], 1
        mov    ax, word ptr [ebx + 1ch]
        test   al, 40h
        jne    NEAR PTR loc_4_12891
        or     ax, 40h
        or     byte ptr [ebp - 1], 10h
        mov    word ptr [ebx + 1ch], ax
        jmp    NEAR PTR loc_4_12891
loc_4_12807:
        test   cl, 4
        je     NEAR PTR loc_4_12891
        mov    eax, dword ptr [ebx + 24h]
        test   byte ptr [eax + 10h], 1
        jne    SHORT loc_4_12891
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_1286B
        mov    ecx, dword ptr [ebp + 8]
        push   7
        mov    byte ptr [ecx + 4], 2
        push   ecx
        db  0E8h, 05Eh, 03Fh, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_1285B
        mov    edi, ebx
        mov    esi, dword ptr [ebx + 24h]
        movsd
        movsd
        movsd
        push   ebx
        call   sub_4_12626
        add    esp, 4
        push   2
        push   dword ptr [ebp + 8]
        call   sub_4_122E1
        add    esp, 8
        mov    eax, dword ptr [ebx + 24h]
        or     word ptr [eax + 10h], 1
loc_4_1285B:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_1286B
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 1ah], 0
        je     SHORT loc_4_12891
loc_4_1286B:
        mov    ecx, dword ptr [ebp + 8]
        push   1
        mov    byte ptr [ecx + 4], 2
        push   ecx
        db  0E8h, 012h, 03Fh, 0FAh, 0FFh
        add    esp, 8
loc_4_1287D:
        push   ebx
        call   sub_4_12626
        add    esp, 4
        push   dword ptr [ebp + 8]
        call   sub_4_71C5
        add    esp, 4
loc_4_12891:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 7ch], eax
        mov    ebx, dword ptr [ebp - 8]
        or     ebx, ebx
        jne    NEAR PTR loc_4_126E7
loc_4_128A5:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 7ch]
        movzx  cx, byte ptr [ebp - 1]
        or     word ptr [eax + 32h], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 8], eax
        push   ecx
        db  0E8h, 0B5h, 010h, 0FAh, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_128CD:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ebp - 104h]
        mov    ebx, dword ptr [ecx + 74h]
        push   eax
        mov    esi, dword ptr [ebx + 14h]
        call   sub_4_2751
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 4
        mov    eax, dword ptr [ecx + 7ch]
        mov    dword ptr [ebp - 88h], eax
        mov    dword ptr [ebp - 90h], ebx
        mov    dword ptr [ebp - 8ch], esi
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 0FDh, 00Fh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_12921
        jmp    NEAR PTR loc_4_12B41
loc_4_12921:
        mov    esi, dword ptr [ebx + 24h]
        movzx  eax, word ptr [esi + 14h]
        add    esi, 14h
        add    eax, 2
        mov    edi, dword ptr [ebp - 0f8h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 7ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0e8h], eax
        mov    ax, word ptr [ebx + 30h]
        mov    word ptr [ebp - 0feh], ax
IFSMgr_GetConversionTablePtrs:
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 0ffh], al
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_12406
        add    esp, 4
        mov    eax, dword ptr [ebp - 0fch]
        and    eax, 0ffffh
        shl    eax, 4
        mov    ecx, dword ptr [ebp - 0fch]
        shr    ecx, 10h
        shl    ecx, 4
        sub    eax, ecx
        mov    dword ptr [ebp - 84h], eax
        mov    al, byte ptr [ebx + 1fh]
        test   al, 3
        je     NEAR PTR loc_4_12AB8
        mov    al, byte ptr [ebx + 22h]
        mov    byte ptr [ebp - 100h], al
        mov    word ptr [ebp - 0ech], 1
        mov    ax, word ptr [ebx + 22h]
        and    ax, 0ff01h
        or     ax, 801h
        mov    word ptr [ebp - 0ech], ax
        mov    dword ptr [ebp - 0f4h], ebx
        mov    dword ptr [ebp - 104h], 0
        mov    dword ptr [ebp - 0e0h], 0
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 0e4h], eax
        mov    eax, dword ptr [ebx + 24h]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0e0h], eax
        lea    eax, [ebp - 104h]
        push   eax
        mov    eax, dword ptr [ebp - 88h]
        push   24h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 18h]
        db  0E8h, 07Fh, 011h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_12AEA
        mov    eax, dword ptr [ebp - 0e4h]
        mov    dword ptr [ebx + 0ch], eax
        mov    esi, dword ptr [ebx + 14h]
        or     esi, esi
        je     SHORT loc_4_12A67
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_123B2
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_12A67
        mov    al, byte ptr [ebp - 104h]
        mov    byte ptr [esi + 4], al
        mov    eax, dword ptr [ebp - 0e0h]
        mov    dword ptr [esi + 11h], eax
        mov    ax, word ptr [ebp - 0dch]
        mov    word ptr [esi + 0dh], ax
        mov    ax, word ptr [ebp - 0dah]
        mov    word ptr [esi + 0fh], ax
loc_4_12A67:
        mov    byte ptr [ebp - 100h], 0
        mov    eax, dword ptr [ebx + 18h]
        mov    dword ptr [ebp - 0e0h], eax
        push   0
        lea    eax, [ebp - 104h]
        push   eax
        mov    eax, dword ptr [ebx + 8]
        push   0ah
        push   dword ptr [eax + 4]
        db  0E8h, 003h, 011h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_12AEA
        test   byte ptr [ebp + 0ch], 1
        je     SHORT loc_4_12AE1
        mov    edi, dword ptr [ebx + 24h]
        mov    esi, ebx
        movsd
        movsd
        movsd
        db  0C7h, 003h, 02Dh, 049h, 000h, 000h
        db  0C7h, 043h, 004h, 040h, 049h, 000h, 000h
        db  0C7h, 043h, 008h, 050h, 072h, 000h, 000h
        jmp    SHORT loc_4_12AE1
loc_4_12AB8:
        test   al, 4
        je     SHORT loc_4_12AEA
        mov    byte ptr [ebp - 100h], 3
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 0e4h], eax
        push   7
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 0B2h, 03Ch, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_12AEA
loc_4_12AE1:
        mov    eax, dword ptr [ebx + 24h]
        and    word ptr [eax + 10h], 0fffeh
loc_4_12AEA:
        cmp    word ptr [ebp - 0eah], 0
        je     SHORT loc_4_12B32
        push   ebx
        call   sub_4_12626
        add    esp, 4
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_71C5
        add    esp, 4
        test   byte ptr [ebp + 0ch], 2
        je     SHORT loc_4_12B32
        push   dword ptr [ebp + 8]
        db  0E8h, 0E4h, 01Dh, 0FAh, 0FFh
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 88h]
        mov    dword ptr [ecx + 7ch], eax
        push   ecx
        db  0E8h, 04Ch, 022h, 0FAh, 0FFh
        add    esp, 4
loc_4_12B32:
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 03Ah, 00Eh, 0FAh, 0FFh
        add    esp, 4
loc_4_12B41:
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ebp - 0eah]
        mov    word ptr [ecx + 1ah], ax
        movsx  eax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_12B59:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 7ch]
        mov    ebx, dword ptr [eax + 24h]
        or     ebx, ebx
        je     SHORT loc_4_12BCB
loc_4_12B6F:
        mov    esi, dword ptr [ebx + 2ch]
        mov    eax, dword ptr [edi + 7ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebx + 24h]
IFSMgr_MetaMatch:
        or     eax, eax
        je     SHORT loc_4_12B93
        test   byte ptr [eax + 10h], 1
        je     SHORT loc_4_12B93
        mov    dword ptr [edi + 74h], ebx
        push   0
        push   edi
        call   sub_4_128CD
        add    esp, 8
loc_4_12B93:
        test   byte ptr [ebx + 1eh], 4
        je     SHORT loc_4_12BB6
        mov    dword ptr [edi + 74h], ebx
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 20h], eax
        mov    eax, dword ptr [ebx + 10h]
        push   0
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [edi + 1ch], eax
        push   edi
        db  0E8h, 088h, 037h, 0FAh, 0FFh
        add    esp, 8
loc_4_12BB6:
        push   ebx
        call   sub_4_12626
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 7ch], eax
        mov    ebx, esi
        or     esi, esi
        jne    SHORT loc_4_12B6F
loc_4_12BCB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_12BD2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 7ch]
        mov    edi, dword ptr [eax + 24h]
        or     edi, edi
        je     SHORT loc_4_12C28
loc_4_12BE8:
        mov    eax, dword ptr [edi + 2ch]
        mov    dword ptr [ebp - 4], eax
        mov    ebx, dword ptr [esi + 7ch]
        mov    eax, dword ptr [edi + 24h]
        or     eax, eax
        je     SHORT loc_4_12C1E
        test   byte ptr [eax + 10h], 1
        je     SHORT loc_4_12C1E
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_4_12C1E
        mov    dword ptr [esi + 74h], edi
        push   1
        push   esi
        call   sub_4_128CD
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_12C1E
        mov    eax, dword ptr [edi + 24h]
        or     word ptr [eax + 10h], 2
loc_4_12C1E:
        mov    dword ptr [esi + 7ch], ebx
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        jne    SHORT loc_4_12BE8
loc_4_12C28:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_12C2F:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7ch]
        mov    esi, dword ptr [eax + 24h]
        cmp    byte ptr [eax + 30h], 0
        ja     SHORT loc_4_12C56
        or     esi, esi
        je     SHORT loc_4_12C56
loc_4_12C46:
        push   esi
        call   sub_4_12626
        add    esp, 4
        mov    esi, dword ptr [esi + 2ch]
        or     esi, esi
        jne    SHORT loc_4_12C46
loc_4_12C56:
        pop    esi
        pop    ebp
        ret
sub_4_12C59:
        push   ebp
        mov    ebp, esp
        sub    al, al
        mov    cx, word ptr [ebp + 8]
        test   cl, 20h
        je     SHORT loc_4_12C69
        mov    al, 1
loc_4_12C69:
        test   cl, 8
        je     SHORT loc_4_12C70
        or     al, 2
loc_4_12C70:
        test   ch, 1
        je     SHORT loc_4_12C77
        or     al, 4
loc_4_12C77:
        pop    ebp
        ret
sub_4_12C79:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, ecx
        or     ecx, ecx
        mov    edx, dword ptr [ebp + 0ch]
        je     SHORT loc_4_12C92
loc_4_12C88:
        mov    dword ptr [eax + 10h], edx
        mov    eax, dword ptr [eax + 2ch]
        or     eax, eax
        jne    SHORT loc_4_12C88
loc_4_12C92:
        mov    dword ptr [edx + 24h], ecx
        mov    eax, dword ptr [ebp + 10h]
        add    dword ptr [edx + 10h], eax
        mov    word ptr [edx + 16h], ax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edx + 40h], eax
        mov    dword ptr [eax + 1ch], edx
        push   0
        push   ecx
        db  0E8h, 071h, 038h, 0FAh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret
sub_4_12CB4:
        push   ebp
        db  08Bh
MatchBasisName:
        in     al, dx
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 7ch]
        mov    al, byte ptr [ecx + 14h]
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [ecx + 24h]
        movzx  ebx, byte ptr [ebp - 1]
        mov    dx, word ptr [ecx + 16h]
        mov    dword ptr [ebp - 8], eax
        movzx  eax, dx
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ecx + 40h]
        mov    dword ptr [ebp - 10h], eax
        db  08Bh, 004h, 09Dh, 0C0h, 065h, 000h, 000h
        test   byte ptr [eax + 0bh], 4
        je     SHORT loc_4_12D00
        mov    word ptr [esi + 1ah], 29h
        mov    eax, 29h
        jmp    NEAR PTR loc_4_12DCE
loc_4_12D00:
        mov    di, word ptr [ecx + 32h]
        and    edi, 1
        or     dx, dx
        je     SHORT loc_4_12D33
        mov    word ptr [ecx + 16h], 0
        mov    eax, dword ptr [ebp - 0ch]
        sub    dword ptr [ecx + 10h], eax
        mov    dword ptr [ecx + 24h], 0
        mov    dword ptr [ecx + 40h], 0
        push   1
        push   dword ptr [ebp - 8]
        db  0E8h, 0F1h, 037h, 0FAh, 0FFh
        add    esp, 8
loc_4_12D33:
        push   edi
        push   esi
        call   sub_4_1344B
        add    esp, 8
        push   esi
        db  0E8h, 07Bh, 035h, 0FAh, 0FFh
        add    esp, 4
        push   3
        push   ebx
        call   sub_4_4114
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_12DC7
        push   2
        push   ebx
        call   sub_4_4114
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_12DC7
        mov    eax, 1
        mov    cl, byte ptr [ebp - 1]
        shl    eax, cl
        not    eax
        db  021h, 005h, 064h, 000h, 000h, 000h
        db  08Bh, 004h, 09Dh, 0C0h, 065h, 000h, 000h
        push   esi
        mov    ebx, dword ptr [eax]
        mov    dword ptr [esi + 7ch], ebx
        db  0E8h, 079h, 034h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_12DA5
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [esi + 7ch]
        push   dword ptr [ebp - 8]
        call   sub_4_12C79
        add    esp, 10h
loc_4_12DA5:
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebx + 2ch], eax
        db  0C7h, 043h, 00Ch, 010h, 072h, 000h, 000h
        mov    eax, dword ptr [esi + 8]
        or     di, 100h
        mov    dword ptr [ebx + 28h], eax
        mov    byte ptr [ebx + 30h], 1
        or     word ptr [ebx + 32h], di
        sub    eax, eax
loc_4_12DC7:
        mov    word ptr [esi + 1ah], ax
        movsx  eax, ax
loc_4_12DCE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_12DD5:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 7ch]
        mov    al, byte ptr [edi + 14h]
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [edi + 24h]
        mov    cx, word ptr [edi + 16h]
        mov    dword ptr [ebp - 8], eax
        movzx  eax, cx
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [edi + 40h]
        mov    dword ptr [ebp - 10h], eax
        mov    bx, word ptr [edi + 32h]
        and    ebx, 1
        or     cx, cx
        je     SHORT loc_4_12E33
        mov    word ptr [edi + 16h], 0
        mov    eax, dword ptr [ebp - 0ch]
        sub    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 24h], 0
        mov    dword ptr [edi + 40h], 0
        push   1
        push   dword ptr [ebp - 8]
        db  0E8h, 0F1h, 036h, 0FAh, 0FFh
        add    esp, 8
loc_4_12E33:
        mov    byte ptr [edi + 30h], 0
        mov    dword ptr [edi + 28h], 0
        mov    eax, dword ptr [edi + 2ch]
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [edi + 32h], 0
        push   esi
        db  0E8h, 06Eh, 034h, 0FAh, 0FFh
        add    esp, 4
        movzx  edi, byte ptr [ebp - 1]
        push   3
        push   edi
        call   sub_4_4114
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_12F0E
        push   0
        push   edi
        call   sub_4_4114
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_12F0E
        mov    eax, 1
        mov    cl, byte ptr [ebp - 1]
        shl    eax, cl
        db  009h, 005h, 064h, 000h, 000h, 000h
        db  08Bh, 004h, 0BDh, 0C0h, 065h, 000h, 000h
        push   esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        db  0E8h, 062h, 033h, 0FAh, 0FFh
        add    esp, 4
        mov    al, byte ptr [esi + 0a9h]
        mov    byte ptr [ebp - 2], al
        mov    di, word ptr [esi + 0ach]
        mov    byte ptr [esi + 0a9h], 0
        or     ebx, 2
        mov    word ptr [esi + 0ach], 0
        push   ebx
        push   esi
        call   sub_4_1331B
        add    esp, 8
        mov    word ptr [esi + 0ach], 4
        push   ebx
        push   esi
        call   sub_4_1331B
        add    esp, 8
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_12EFC
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [esi + 7ch]
        push   dword ptr [ebp - 8]
        call   sub_4_12C79
        add    esp, 10h
loc_4_12EFC:
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [esi + 0a9h], al
        mov    word ptr [esi + 0ach], di
        sub    eax, eax
loc_4_12F0E:
        mov    word ptr [esi + 1ah], ax
        movsx  eax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_12F1C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 16h], 0
        jne    SHORT loc_4_12F2B
        jmp    SHORT loc_4_12F43
loc_4_12F2B:
        mov    eax, dword ptr [eax + 24h]
        or     eax, eax
        je     SHORT loc_4_12F43
        mov    ecx, 8
loc_4_12F37:
        test   byte ptr [eax + 1fh], cl
        je     SHORT loc_4_12F4A
        mov    eax, dword ptr [eax + 2ch]
        or     eax, eax
        jne    SHORT loc_4_12F37
loc_4_12F43:
        mov    eax, 1
loc_4_12F48:
        pop    ebp
        ret
loc_4_12F4A:
        sub    eax, eax
        jmp    SHORT loc_4_12F48
sub_4_12F4E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7ch]
        mov    ebx, dword ptr [eax + 24h]
        or     ebx, ebx
        je     SHORT loc_4_12FAC
loc_4_12F61:
        push   201h
        push   16h
AppendBasisTail:
        db  0E8h, 08Bh, 056h, 000h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_12FB3
        mov    dword ptr [eax + 0ch], 0
        mov    word ptr [eax + 10h], 0
        mov    word ptr [eax + 12h], 0
        mov    dword ptr [ebx + 24h], eax
        mov    edi, eax
        mov    esi, ebx
        movsd
        movsd
        movsd
        db  0C7h, 003h, 02Dh, 049h, 000h, 000h
        db  0C7h, 043h, 004h, 040h, 049h, 000h, 000h
        db  0C7h, 043h, 008h, 050h, 072h, 000h, 000h
        mov    ebx, dword ptr [ebx + 2ch]
        or     ebx, ebx
        jne    SHORT loc_4_12F61
loc_4_12FAC:
        sub    eax, eax
loc_4_12FAE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_12FB3:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1ah], 8
        mov    eax, 8
        jmp    SHORT loc_4_12FAE
sub_4_12FC3:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7ch]
        mov    esi, dword ptr [eax + 24h]
        or     esi, esi
        je     SHORT loc_4_12FE4
loc_4_12FD4:
        push   esi
        call   sub_4_12626
        add    esp, 4
        mov    esi, dword ptr [esi + 2ch]
        or     esi, esi
        jne    SHORT loc_4_12FD4
loc_4_12FE4:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 0a9h]
        inc    bl
        mov    ax, word ptr [esi + 0ach]
        mov    word ptr [ebp - 2], ax
        mov    edi, dword ptr [esi + 7ch]
        movzx  eax, bl
        cmp    eax, 1
        je     SHORT loc_4_13038
        cmp    eax, 2
        je     NEAR PTR loc_4_13186
        cmp    eax, 3
        jl     SHORT loc_4_13028
        cmp    eax, 4
        jle    NEAR PTR loc_4_130D6
loc_4_13028:
        mov    word ptr [esi + 1ah], 29h
        mov    eax, 29h
        jmp    NEAR PTR loc_4_13172
loc_4_13038:
        test   byte ptr [edi + 1bh], 8
        je     SHORT loc_4_1304E
        db  066h, 0C7h, 046h
CreateBasis:
        sbb    ch, byte ptr [ecx]
        add    byte ptr [eax + 29h], bh
        jmp    NEAR PTR loc_4_13172
loc_4_1304E:
        push   edi
        call   sub_4_12F1C
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_1306B
        mov    word ptr [esi + 1ah], 2ah
        mov    eax, 2ah
        jmp    NEAR PTR loc_4_13172
loc_4_1306B:
        mov    al, byte ptr [edi + 30h]
        cmp    al, 1
        ja     SHORT loc_4_130C6
        test   byte ptr [edi + 33h], 1
        jne    SHORT loc_4_130C6
        cmp    al, 1
        jne    SHORT loc_4_13083
        cmp    word ptr [ebp - 2], 4
        jne    SHORT loc_4_130C6
loc_4_13083:
        cmp    word ptr [ebp - 2], 4
        ja     SHORT loc_4_130B6
        jne    SHORT loc_4_1309D
        cmp    al, 1
        jne    SHORT loc_4_130B6
        push   esi
        db  0E8h, 0D9h, 033h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_130B6
loc_4_1309D:
        cmp    word ptr [ebp - 2], 4
        jne    NEAR PTR loc_4_13186
        push   esi
        call   sub_4_12CB4
        add    esp, 4
        jmp    NEAR PTR loc_4_13172
loc_4_130B6:
        mov    word ptr [esi + 1ah], 29h
        mov    eax, 29h
        jmp    NEAR PTR loc_4_13172
loc_4_130C6:
        mov    word ptr [esi + 1ah], 28h
        mov    eax, 28h
        jmp    NEAR PTR loc_4_13172
loc_4_130D6:
        movzx  ecx, byte ptr [edi + 30h]
        sub    ecx, eax
        cmp    ecx, -1
        jne    NEAR PTR loc_4_13179
        push   esi
        db  0E8h, 084h, 033h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_4_13179
        test   byte ptr [ebp + 0ch], 1
        jne    SHORT loc_4_1310F
        test   byte ptr [edi + 32h], 1
        je     SHORT loc_4_1310F
        mov    word ptr [esi + 1ah], 29h
        mov    eax, 29h
        jmp    SHORT loc_4_13172
loc_4_1310F:
        push   0
        push   ebx
        push   8041h
        push   esi
        call   sub_4_12233
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_1315D
        cmp    bl, 3
        je     NEAR PTR loc_4_13248
        push   esi
        call   sub_4_126A2
        add    esp, 4
        mov    word ptr [esi + 18h], 0
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 1ch], eax
        push   0
        push   esi
        mov    eax, dword ptr [edi + 0ch]
        push   22h
        push   dword ptr [eax + 10h]
        db  0E8h, 03Eh, 00Ah, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_4_13248
loc_4_1315D:
        push   0
        push   ebx
        push   8043h
        push   esi
        call   sub_4_12233
        add    esp, 10h
        movsx  eax, word ptr [esi + 1ah]
loc_4_13172:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_13179:
        mov    word ptr [esi + 1ah], 29h
        mov    eax, 29h
        jmp    SHORT loc_4_13172
loc_4_13186:
        test   byte ptr [edi + 1bh], 8
        je     SHORT loc_4_13199
        mov    word ptr [esi + 1ah], 29h
        mov    eax, 29h
        jmp    SHORT loc_4_13172
loc_4_13199:
        cmp    bl, 2
        jne    SHORT loc_4_131CA
        cmp    word ptr [ebp - 2], 3
        jbe    SHORT loc_4_131B7
        mov    ax, word ptr [esi + 1ah]
        sub    ax, 29h
        cmp    ax, 1
        sbb    eax, eax
        neg    eax
        jmp    SHORT loc_4_13172
loc_4_131B7:
        cmp    byte ptr [edi + 30h], 0
        je     SHORT loc_4_131CA
        mov    word ptr [esi + 1ah], 28h
        mov    eax, 28h
        jmp    SHORT loc_4_13172
loc_4_131CA:
        push   dword ptr [ebp - 2]
        push   ebx
        push   8041h
        push   esi
        call   sub_4_12233
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_1315D
        cmp    bl, 2
        jne    SHORT loc_4_1320C
        push   esi
        call   sub_4_125AF
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_1315D
        push   0
        call   sub_4_2D2C
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_1315D
loc_4_1320C:
        mov    word ptr [esi + 18h], 0
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 1ch], eax
        push   0
        push   esi
        mov    eax, dword ptr [edi + 0ch]
        push   22h
        push   dword ptr [eax + 10h]
        db  0E8h, 068h, 009h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_1315D
        cmp    bl, 1
        jne    SHORT loc_4_13248
        cmp    word ptr [edi + 16h], 0
        jbe    SHORT loc_4_13248
        push   esi
        call   sub_4_12F4E
        add    esp, 4
loc_4_13248:
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        jmp    NEAR PTR loc_4_13172
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 7ch]
        cmp    byte ptr [edi + 30h], 0
        jne    SHORT loc_4_13276
        mov    word ptr [esi + 1ah], 29h
        mov    eax, 29h
        jmp    NEAR PTR loc_4_13317
loc_4_13276:
        push   esi
        db  0E8h, 0F3h, 031h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_13293
        mov    word ptr [esi + 1ah], 29h
        mov    eax, 29h
        jmp    NEAR PTR loc_4_13317
loc_4_13293:
        test   byte ptr [ebp + 0ch], 1
        jne    SHORT loc_4_132AC
        test   byte ptr [edi + 32h], 1
        je     SHORT loc_4_132AC
        mov    word ptr [esi + 1ah], 29h
        mov    eax, 29h
        jmp    SHORT loc_4_13317
loc_4_132AC:
        cmp    byte ptr [esi + 0b0h], 6bh
        je     SHORT loc_4_132C8
        cmp    byte ptr [edi + 31h], 0ffh
        je     SHORT loc_4_132C8
        mov    word ptr [esi + 1ah], 29h
        mov    eax, 29h
        jmp    SHORT loc_4_13317
loc_4_132C8:
        mov    al, byte ptr [edi + 30h]
        cmp    al, 1
        jne    SHORT loc_4_132E0
        test   byte ptr [edi + 33h], 1
        je     SHORT loc_4_132E0
        push   esi
        call   sub_4_12DD5
        add    esp, 4
        jmp    SHORT loc_4_13317
loc_4_132E0:
        push   0
        push   eax
        push   8044h
        push   esi
        call   sub_4_12233
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_1330F
        push   0
        mov    al, byte ptr [edi + 30h]
        push   eax
        push   8046h
        push   esi
        call   sub_4_12233
        add    esp, 10h
        movsx  eax, word ptr [esi + 1ah]
BcsToBcs:
        jmp    SHORT loc_4_13317
loc_4_1330F:
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
loc_4_13317:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_1331B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 0a9h]
        inc    bl
        mov    edi, dword ptr [esi + 7ch]
        cmp    bl, 3
        jae    NEAR PTR loc_4_133CE
        cmp    bl, 1
        jne    SHORT loc_4_13351
        test   byte ptr [esi + 0ach], 4
        je     SHORT loc_4_13351
        or     word ptr [edi + 32h], 100h
        jmp    NEAR PTR loc_4_13410
loc_4_13351:
        cmp    bl, 2
        jne    SHORT loc_4_13372
        test   byte ptr [esi + 0ach], 1
        je     SHORT loc_4_13364
        or     word ptr [edi + 32h], 20h
loc_4_13364:
        test   byte ptr [esi + 0ach], 2
        je     SHORT loc_4_13372
        or     word ptr [edi + 32h], 8
loc_4_13372:
        cmp    bl, 1
        je     SHORT loc_4_1337D
        test   byte ptr [edi + 32h], 20h
        jne    SHORT loc_4_1338A
loc_4_1337D:
        push   100h
        call   sub_4_2C8C
        add    esp, 4
loc_4_1338A:
        cmp    bl, 1
        jne    SHORT loc_4_133B2
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    byte ptr [esi + 4], 2
        mov    word ptr [esi + 18h], 4
        push   0
        push   esi
        mov    eax, dword ptr [edi + 0ch]
        push   2dh
        push   dword ptr [eax + 3ch]
        db  0E8h, 0E1h, 007h, 0FAh, 0FFh
        add    esp, 10h
loc_4_133B2:
        mov    ecx, dword ptr [ebp + 0ch]
        test   cl, 1
        je     SHORT loc_4_133BF
        or     word ptr [edi + 32h], 1
loc_4_133BF:
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [edi + 2ch], eax
        db  0C7h, 047h, 00Ch, 010h, 072h, 000h, 000h
        jmp    SHORT loc_4_13413
loc_4_133CE:
        cmp    bl, 4
        jne    SHORT loc_4_13410
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    byte ptr [esi + 4], 2
        mov    word ptr [esi + 18h], 1
        push   0
        push   esi
        mov    eax, dword ptr [edi + 0ch]
        push   2dh
        push   dword ptr [eax + 3ch]
        db  0E8h, 09Dh, 007h, 0FAh, 0FFh
        add    esp, 10h
        test   byte ptr [edi + 32h], 20h
        je     SHORT loc_4_13409
        push   80h
        call   sub_4_2C8C
        add    esp, 4
loc_4_13409:
        push   5
        db  0E8h, 0F8h, 051h, 000h, 000h
loc_4_13410:
        mov    ecx, dword ptr [ebp + 0ch]
loc_4_13413:
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 28h], eax
        mov    byte ptr [edi + 30h], bl
        test   cl, 2
        jne    SHORT loc_4_1343E
        mov    ax, word ptr [edi + 32h]
        push   eax
        db  0E8h, 02Eh
BcsToBcsUpper:
        clc
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 050h, 053h, 068h, 042h, 080h, 000h, 000h, 056h, 0E8h, 0F8h, 0EDh
        db  0FFh, 0FFh, 083h, 0C4h, 010h
loc_4_1343E:
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_1344B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 7ch]
        mov    bl, byte ptr [edi + 30h]
        cmp    bl, 3
        jae    NEAR PTR loc_4_1352B
        test   byte ptr [edi + 33h], 1
        jne    SHORT loc_4_13488
        mov    word ptr [esi + 18h], 0
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 1ch], eax
        push   0
        push   esi
        mov    eax, dword ptr [edi + 0ch]
        push   22h
        push   dword ptr [eax + 10h]
        db  0E8h, 00Bh, 007h, 0FAh, 0FFh
        add    esp, 10h
loc_4_13488:
        cmp    bl, 2
        jne    SHORT loc_4_1349F
        push   esi
        call   sub_4_12B59
        add    esp, 4
        push   1
        call   sub_4_2D2C
        jmp    SHORT loc_4_134D5
loc_4_1349F:
        test   byte ptr [edi + 33h], 1
        jne    SHORT loc_4_134D8
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    byte ptr [esi + 4], 2
        mov    word ptr [esi + 18h], 8
        push   0
        push   esi
        mov    eax, dword ptr [edi + 0ch]
        push   2dh
        push   dword ptr [eax + 3ch]
        db  0E8h, 0CBh, 006h, 0FAh, 0FFh
        add    esp, 10h
        cmp    word ptr [edi + 16h], 0
        jbe    SHORT loc_4_134D8
        push   esi
        call   sub_4_12FC3
loc_4_134D5:
        add    esp, 4
loc_4_134D8:
        mov    ax, word ptr [edi + 32h]
        test   ah, 1
        je     SHORT loc_4_1350C
        and    ax, 0feffh
        mov    word ptr [edi + 32h], ax
        inc    byte ptr [esi + 89h]
        mov    byte ptr [esi + 4], 0
        db  068h, 0C7h, 029h, 000h, 000h
        push   esi
        call   sub_4_2547
        add    esp, 8
        dec    byte ptr [esi + 89h]
        jmp    NEAR PTR loc_4_1359F
loc_4_1350C:
        mov    byte ptr [edi + 30h], 0
        mov    dword ptr [edi + 28h], 0
        mov    eax, dword ptr [edi + 2ch]
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [edi + 2ch], 0
        and    word ptr [edi + 32h], 2
        jmp    SHORT loc_4_1359F
loc_4_1352B:
        dec    bl
        mov    byte ptr [edi + 30h], bl
        cmp    bl, 3
        jne    SHORT loc_4_13583
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    byte ptr [esi + 4], 2
        mov    word ptr [esi + 18h], 2
        push   0
        push   esi
        mov    eax, dword ptr [edi + 0ch]
        push   2dh
        push   dword ptr [eax + 3ch]
        db  0E8h, 03Bh, 006h, 0FAh, 0FFh
        add    esp, 10h
IFSMgr_ParsePath:
        inc    byte ptr [edi + 2]
        push   esi
        call   sub_4_12BD2
        add    edi, 3ah
        add    esp, 4
        cmp    word ptr [edi], 0
        jbe    SHORT loc_4_1357B
        mov    word ptr [edi], 0
        push   edi
        db  0E8h, 079h, 0F9h, 0F9h, 0FFh
        add    esp, 4
loc_4_1357B:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        jmp    SHORT loc_4_1359F
loc_4_13583:
        cmp    bl, 2
        jne    SHORT loc_4_1359F
        add    edi, 3ch
        cmp    word ptr [edi], 0
        jbe    SHORT loc_4_1359F
        mov    word ptr [edi], 0
        push   edi
        db  0E8h, 055h, 0F9h, 0F9h, 0FFh
        add    esp, 4
loc_4_1359F:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_135A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0a8h
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0ffffffffh
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 20h], 0
        mov    edi, dword ptr [ebp + 0ch]
        cmp    edi, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 14h], eax
        mov    esi, dword ptr [ebp + 8]
        db  0C7h, 045h, 0F0h, 01Bh, 013h, 001h, 000h
        mov    al, byte ptr [esi + 89h]
        db  0C7h, 045h, 0E4h, 0E7h, 00Fh, 001h, 000h
        movzx  ecx, al
        db  0F6h, 081h, 08Ch, 000h, 000h, 000h, 080h
        je     SHORT loc_4_1360D
        mov    word ptr [esi + 1ah], 29h
        mov    eax, 29h
        jmp    NEAR PTR loc_4_13923
loc_4_1360D:
        test   byte ptr [esi + 8bh], 80h
        je     SHORT loc_4_13626
        mov    word ptr [esi + 1ah], 5
        mov    eax, 5
        jmp    NEAR PTR loc_4_13923
loc_4_13626:
        push   eax
        push   esi
        call   sub_4_12200
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_13639
        jmp    NEAR PTR loc_4_1391F
loc_4_13639:
        movzx  eax, byte ptr [esi + 89h]
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        or     eax, eax
        jne    SHORT loc_4_13660
        mov    word ptr [esi + 1ah], 0fh
        mov    eax, 0fh
        jmp    NEAR PTR loc_4_13923
loc_4_13660:
        cmp    byte ptr [esi + 0b0h], 4ah
        jne    SHORT loc_4_1367E
        cmp    byte ptr [esi + 0a9h], 3
        jne    SHORT loc_4_1367E
        db  0E8h, 0B5h, 02Fh, 0FAh, 0FFh
        mov    dword ptr [ebp - 20h], 1
loc_4_1367E:
        mov    al, byte ptr [esi + 0b0h]
        cmp    al, 4bh
        je     SHORT loc_4_13699
        cmp    al, 6bh
        je     SHORT loc_4_13699
        db  068h, 098h, 072h, 000h, 000h
        db  0E8h, 062h, 02Fh, 0FAh, 0FFh
        add    esp, 4
loc_4_13699:
        mov    al, byte ptr [esi + 0b0h]
        cmp    al, 6ah
        je     SHORT loc_4_136A7
        cmp    al, 6bh
        jne    SHORT loc_4_136B5
loc_4_136A7:
        db  0C7h, 045h, 0E4h, 055h, 012h, 001h, 000h
        db  0C7h, 045h, 0F0h, 04Bh, 014h, 001h, 000h
loc_4_136B5:
        push   esi
        db  0E8h, 046h, 02Bh, 0FAh, 0FFh
        add    esp, 4
        push   0
        push   esi
        call   dword ptr [ebp - 1ch]
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_136E3
        push   esi
        call   sub_4_12C2F
        add    esp, 4
        mov    dword ptr [ebp - 0ch], 1
        mov    bl, byte ptr [ebp - 1]
        jmp    NEAR PTR loc_4_138A8
loc_4_136E3:
        or     edi, edi
        je     SHORT loc_4_13702
        mov    byte ptr [ebp - 28h], 6
        mov    al, byte ptr [esi + 89h]
        mov    byte ptr [ebp - 26h], al
        lea    eax, [ebp - 28h]
        push   eax
        db  0E8h, 0E3h, 04Eh, 000h, 000h
        add    esp, 4
        jmp    SHORT loc_4_13709
loc_4_13702:
        mov    dword ptr [ebp - 25h], 0
loc_4_13709:
        cmp    dword ptr [ebp - 25h], 0
        je     NEAR PTR loc_4_137B1
        lea    edi, [ebp - 0a8h]
        db  0C7h, 045h, 0E8h, 0BCh, 065h, 000h, 000h
loc_4_13720:
        add    dword ptr [ebp - 18h], 4
        inc    ebx
        mov    eax, 1
        mov    cl, bl
        shl    eax, cl
        test   dword ptr [ebp - 25h], eax
        je     SHORT loc_4_1377E
        not    eax
        and    dword ptr [ebp - 25h], eax
        movzx  eax, byte ptr [esi + 89h]
        cmp    eax, ebx
        je     SHORT loc_4_1377E
        push   ebx
        push   esi
        call   sub_4_12200
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_13786
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        or     eax, eax
        je     SHORT loc_4_1378F
        push   esi
        db  0E8h, 09Ch, 02Ah, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [ebp - 14h]
        push   esi
        call   dword ptr [ebp - 1ch]
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_13798
        mov    dword ptr [edi], ebx
        add    edi, 4
        inc    dword ptr [ebp - 8]
loc_4_1377E:
        cmp    dword ptr [ebp - 25h], 0
        jne    SHORT loc_4_13720
        jmp    SHORT loc_4_137B1
loc_4_13786:
        mov    dword ptr [ebp - 0ch], 1
        jmp    SHORT loc_4_137B1
loc_4_1378F:
        mov    dword ptr [ebp - 0ch], 1
        jmp    SHORT loc_4_137B1
loc_4_13798:
        push   esi
        call   sub_4_12C2F
        add    esp, 4
        mov    dword ptr [ebp - 0ch], 1
        push   esi
        db  0E8h, 010h, 02Bh, 0FAh, 0FFh
        add    esp, 4
loc_4_137B1:
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [ebp - 8]
        or     eax, eax
        je     NEAR PTR loc_4_1385B
        mov    eax, dword ptr [ebp - 8]
        lea    edi, [ebp + eax*4 - 0a8h]
loc_4_137C9:
        mov    eax, dword ptr [edi]
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        cmp    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        mov    bl, byte ptr [eax + 30h]
        jne    SHORT loc_4_13809
        push   dword ptr [ebp - 14h]
        push   esi
        call   dword ptr [ebp - 10h]
        add    esp, 8
        cmp    bl, 2
        ja     SHORT loc_4_13812
        mov    eax, dword ptr [esi + 7ch]
        cmp    byte ptr [eax + 30h], 0
        jne    SHORT loc_4_13812
        push   1
        push   -1
        push   -1
        push   esi
        db  0E8h, 063h, 033h, 0FAh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_4_13812
loc_4_13809:
        push   esi
        call   sub_4_12C2F
        add    esp, 4
loc_4_13812:
        push   esi
        db  0E8h, 0A6h, 02Ah, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_1384A
        db  081h, 07Dh, 0F0h, 04Bh, 014h, 001h, 000h
        jne    SHORT loc_4_1384A
        mov    eax, dword ptr [esi + 7ch]
        mov    ax, word ptr [eax + 32h]
        push   eax
        call   sub_4_12C59
        add    esp, 4
        push   eax
        push   ebx
        push   8045h
        push   esi
        call   sub_4_12233
        add    esp, 10h
loc_4_1384A:
        mov    eax, dword ptr [ebp - 8]
        sub    edi, 4
        dec    dword ptr [ebp - 8]
        or     eax, eax
        jne    NEAR PTR loc_4_137C9
loc_4_1385B:
        movzx  eax, byte ptr [esi + 89h]
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        cmp    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        mov    bl, byte ptr [eax + 30h]
        jne    SHORT loc_4_1389F
        push   0
loc_4_13879:
        push   esi
        call   dword ptr [ebp - 10h]
        add    esp, 8
        cmp    bl, 2
        ja     SHORT loc_4_138A8
        mov    eax, dword ptr [esi + 7ch]
        cmp    byte ptr [eax + 30h], 0
        jne    SHORT loc_4_138A8
        push   1
        push   -1
        push   -1
        push   esi
        db  0E8h, 0CDh, 032h, 0FAh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_4_138A8
loc_4_1389F:
        push   esi
        call   sub_4_12C2F
        add    esp, 4
loc_4_138A8:
        push   esi
        db  0E8h, 010h, 02Ah, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_138F1
        db  081h, 07Dh, 0F0h, 04Bh, 014h, 001h, 000h
        jne    SHORT loc_4_138F1
        mov    eax, dword ptr [esi + 7ch]
        mov    ax, word ptr [eax + 32h]
        push   eax
        call   sub_4_12C59
        add    esp, 4
        push   eax
        push   ebx
        push   8045h
        push   esi
        call   sub_4_12233
        add    esp, 10h
        mov    eax, dword ptr [esi + 7ch]
        movzx  ax, byte ptr [eax + 30h]
        dec    ax
        mov    word ptr [esi + 0b4h], ax
loc_4_138F1:
        mov    al, byte ptr [esi + 0b0h]
        cmp    al, 4bh
        je     SHORT loc_4_1390C
        cmp    al, 6bh
        je     SHORT loc_4_1390C
        db  068h, 098h, 072h, 000h, 000h
        db  0E8h, 009h, 02Dh, 0FAh, 0FFh
        add    esp, 4
loc_4_1390C:
        cmp    dword ptr [ebp - 20h], 0
        je     SHORT loc_4_13918
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_4_13918:
        mov    dword ptr [esi + 7ch], 0
loc_4_1391F:
        movsx  eax, word ptr [esi + 1ah]
loc_4_13923:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 98h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 1], 0ffh
        mov    dword ptr [ebp - 10h], 0
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 0ffffffffh
        cmp    byte ptr [esi + 0b0h], 4bh
        jne    SHORT loc_4_13966
        mov    al, byte ptr [esi + 0a8h]
        mov    byte ptr [ebp - 1], al
loc_4_13966:
        mov    byte ptr [ebp - 18h], 7
        mov    al, byte ptr [esi + 0a8h]
        mov    byte ptr [ebp - 16h], al
        lea    eax, [ebp - 18h]
        push   eax
        db  0E8h, 064h, 04Ch, 000h, 000h
        add    esp, 4
        cmp    byte ptr [ebp - 17h], 0
        je     SHORT loc_4_13995
        mov    word ptr [esi + 1ah], 0fh
        mov    eax, 0fh
loc_4_13990:
        jmp    NEAR PTR loc_4_13ABC
loc_4_13995:
        cmp    byte ptr [esi + 0b0h], 4bh
        jne    SHORT loc_4_139B3
        cmp    byte ptr [esi + 0a9h], 3
        jne    SHORT loc_4_139B3
        db  0E8h, 080h, 02Ch, 0FAh, 0FFh
        mov    dword ptr [ebp - 10h], 1
loc_4_139B3:
        db  068h, 098h, 072h, 000h, 000h
        db  0E8h, 03Bh, 02Ch, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [ebp - 15h], 0
        je     SHORT loc_4_13A25
        lea    edi, [ebp - 98h]
        db  0C7h, 045h, 0F4h, 0BCh, 065h, 000h, 000h
loc_4_139D3:
        add    dword ptr [ebp - 0ch], 4
        inc    ebx
        mov    eax, 1
        mov    cl, bl
        shl    eax, cl
        test   dword ptr [ebp - 15h], eax
        je     SHORT loc_4_13A19
        not    eax
        and    dword ptr [ebp - 15h], eax
        mov    byte ptr [esi + 89h], bl
        push   0
        push   esi
        call   sub_4_135A8
        add    esp, 8
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_4_13A21
        mov    eax, dword ptr [ebp - 0ch]
        add    edi, 4
        mov    eax, dword ptr [eax]
        inc    dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax]
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ecx + 31h], al
        mov    dword ptr [edi - 4], ebx
loc_4_13A19:
        cmp    dword ptr [ebp - 15h], 0
        jne    SHORT loc_4_139D3
        jmp    SHORT loc_4_13A25
loc_4_13A21:
        mov    byte ptr [ebp - 2], 1
loc_4_13A25:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_4_13A9F
        cmp    byte ptr [esi + 0b0h], 4bh
        jne    SHORT loc_4_13A9F
        movsx  edi, word ptr [esi + 1ah]
        mov    word ptr [esi + 1ah], 0
        mov    al, byte ptr [esi + 0b0h]
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [esi + 0b0h], 6bh
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [ebp - 8]
        or     eax, eax
        je     SHORT loc_4_13A92
        mov    eax, dword ptr [ebp - 8]
        lea    ebx, [ebp + eax*4 - 98h]
loc_4_13A62:
        mov    al, byte ptr [ebx]
        mov    byte ptr [esi + 89h], al
        movzx  eax, al
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        push   0
        mov    eax, dword ptr [eax]
        push   esi
        sub    ebx, 4
        mov    byte ptr [eax + 31h], 0ffh
        call   sub_4_135A8
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [ebp - 8]
        or     eax, eax
        jne    SHORT loc_4_13A62
loc_4_13A92:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [esi + 0b0h], al
        mov    word ptr [esi + 1ah], di
loc_4_13A9F:
        db  068h, 098h, 072h, 000h, 000h
        db  0E8h, 069h, 02Bh, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_4_13AB8
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_4_13AB8:
        movsx  eax, word ptr [esi + 1ah]
loc_4_13ABC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_13AC3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 7ch]
        mov    dword ptr [ebp - 0ch], 0
        cmp    byte ptr [edi + 30h], 0
        je     NEAR PTR loc_4_13C97
        db  083h, 03Dh, 098h, 072h, 000h, 000h, 000h
        je     SHORT loc_4_13AF9
        db  0E8h, 04Fh, 04Bh, 000h, 000h
        db  03Bh, 005h, 09Ch, 072h, 000h, 000h
        je     SHORT loc_4_13B0D
loc_4_13AF9:
        db  068h, 098h, 072h, 000h, 000h
        db  0E8h, 09Dh, 00Ah, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 0ch], 1
loc_4_13B0D:
        mov    ax, word ptr [edi + 32h]
        and    ax, 1
        cmp    ax, 1
        sbb    ebx, ebx
        inc    ebx
        mov    al, byte ptr [esi + 0b0h]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [esi + 0a8h]
        mov    byte ptr [ebp - 2], al
        cmp    byte ptr [edi + 31h], 0ffh
        je     SHORT loc_4_13B46
        mov    byte ptr [esi + 0b0h], 6bh
        mov    al, byte ptr [edi + 31h]
        mov    byte ptr [esi + 0a8h], al
        jmp    SHORT loc_4_13B4D
loc_4_13B46:
        mov    byte ptr [esi + 0b0h], 6ah
loc_4_13B4D:
        mov    byte ptr [edi + 31h], 0ffh
        mov    al, byte ptr [edi + 30h]
        mov    byte ptr [ebp - 3], al
        mov    cx, word ptr [edi + 32h]
        cmp    al, 2
        mov    word ptr [ebp - 6], cx
        jbe    SHORT loc_4_13B9F
        cmp    al, 4
        jne    SHORT loc_4_13B83
        push   esi
        db  0E8h, 094h, 026h, 0FAh, 0FFh
        add    esp, 4
        push   ebx
        push   esi
        call   sub_4_1344B
        add    esp, 8
        push   esi
        db  0E8h, 03Eh, 027h, 0FAh, 0FFh
        add    esp, 4
loc_4_13B83:
        push   esi
        db  0E8h, 078h, 026h, 0FAh, 0FFh
        add    esp, 4
        push   ebx
        push   esi
        call   sub_4_1344B
        add    esp, 8
        push   esi
        db  0E8h, 022h, 027h, 0FAh, 0FFh
        add    esp, 4
loc_4_13B9F:
        test   byte ptr [edi + 33h], 1
        je     SHORT loc_4_13BCA
        push   esi
        db  0E8h, 056h, 026h, 0FAh, 0FFh
        add    esp, 4
        push   esi
        call   sub_4_12DD5
        add    esp, 4
        push   ebx
        push   esi
        call   sub_4_1344B
        add    esp, 8
        push   esi
        db  0E8h, 0F7h, 026h, 0FAh, 0FFh
        add    esp, 4
loc_4_13BCA:
        push   esi
        db  0E8h, 031h, 026h, 0FAh, 0FFh
        add    esp, 4
        push   ebx
        push   esi
        call   sub_4_1344B
        add    esp, 8
        push   esi
        db  0E8h, 0DBh, 026h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_4_13BF9
        db  068h, 098h, 072h, 000h, 000h
        db  0E8h, 028h, 00Ah, 0FAh, 0FFh
        add    esp, 4
loc_4_13BF9:
        movzx  eax, byte ptr [ebp - 3]
        cmp    eax, 1
        je     SHORT loc_4_13C13
        cmp    eax, 2
        je     SHORT loc_4_13C69
        cmp    eax, 3
        je     SHORT loc_4_13C57
        cmp    eax, 4
        je     SHORT loc_4_13C45
        jmp    SHORT loc_4_13C85
loc_4_13C13:
        test   byte ptr [ebp - 5], 1
        je     SHORT loc_4_13C35
        push   dword ptr [ebp - 6]
        call   sub_4_12C59
        add    esp, 4
        push   eax
        push   1
        push   8045h
        push   esi
        call   sub_4_12233
        add    esp, 10h
loc_4_13C35:
        push   dword ptr [ebp - 6]
        call   sub_4_12C59
        add    esp, 4
        push   eax
        push   1
        jmp    SHORT loc_4_13C77
loc_4_13C45:
        push   0
        push   4
        push   8045h
        push   esi
        call   sub_4_12233
        add    esp, 10h
loc_4_13C57:
        push   0
        push   3
        push   8045h
        push   esi
        call   sub_4_12233
        add    esp, 10h
loc_4_13C69:
        push   dword ptr [ebp - 6]
        call   sub_4_12C59
        add    esp, 4
        push   eax
        push   2
loc_4_13C77:
        push   8045h
        push   esi
        call   sub_4_12233
        add    esp, 10h
loc_4_13C85:
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [esi + 0a8h], al
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0b0h], al
loc_4_13C97:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
_PTEXT ends
        end
