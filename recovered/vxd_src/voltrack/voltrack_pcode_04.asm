; voltrack_pcode_04.asm -- VOLTRACK object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  0BEh, 000h, 000h, 000h, 000h
        db  0BFh, 012h, 000h, 000h, 000h
        call   sub_4_087E
        jae    SHORT loc_4_0012
        ret
loc_4_0012:
        mov    esi, edx
        mov    ebx, dword ptr [esi + 8]
        xor    ecx, ecx
        test   byte ptr [ebx + 1eh], 4
        je     SHORT loc_4_0028
        mov    ax, 6
        call   sub_4_0731
loc_4_0028:
        mov    ax, 0ch
        call   sub_4_0731
        mov    ebx, dword ptr [esi + 4]
        or     eax, eax
        jne    NEAR PTR loc_4_018A
        and    dword ptr [ebx + 20h], 0fff7ffffh
        push   esi
        mov    ebx, dword ptr [esi + 8]
        mov    ax, 22h
        xor    ecx, ecx
        db  0BAh, 01Bh, 007h, 000h, 000h
        test   byte ptr [ebx + 21h], 20h
        je     SHORT loc_4_0066
        test   byte ptr [ebx + 0b8h], 40h
        je     SHORT loc_4_0066
        db  0BAh, 048h, 008h, 000h, 000h
loc_4_0066:
        call   edx
        cmp    ax, 10h
        jb     NEAR PTR loc_4_0114
        and    byte ptr [ebx + 20h], 0fdh
        mov    eax, ebx
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_4_0080
        mov    eax, dword ptr [ebx]
loc_4_0080:
        mov    edx, dword ptr [eax + 0dbh]
        sub    esp, 10h
        mov    esi, esp
        mov    word ptr [esi + 4], dx
        mov    word ptr [esi + 0ah], 9
        mov    word ptr [esi], 3
        mov    byte ptr [esi + 0ch], 8
        push   esi
        db  0FFh, 015h, 0BCh, 005h, 000h, 000h
        mov    eax, dword ptr [esi + 6]
        add    esp, 14h
        or     eax, eax
        je     NEAR PTR loc_4_0178
        mov    ecx, eax
        test   byte ptr [ebx + 21h], 20h
        je     SHORT loc_4_00D5
        xor    esi, esi
        mov    ax, 0
        call   sub_4_071B
        cmp    dword ptr [ecx + 3], 4d44534dh
        jne    SHORT loc_4_00D5
        cmp    byte ptr [ecx + 7], 46h
        je     SHORT loc_4_00FD
loc_4_00D5:
        xor    esi, esi
        inc    esi
        mov    ax, 0
        call   sub_4_071B
        cmp    ax, 10h
        jae    SHORT loc_4_00FD
        xor    esi, esi
        inc    esi
        mov    ax, 1
        call   sub_4_071B
        cmp    ax, 10h
        jae    SHORT loc_4_00FD
        or     byte ptr [ebx + 20h], 2
loc_4_00FD:
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], ecx
        mov    word ptr [esi], 4
        push   esi
        db  0FFh, 015h, 0BCh, 005h, 000h, 000h
        add    esp, 0ch
loc_4_0114:
        mov    ecx, ebx
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_4_011E
        mov    ecx, dword ptr [ebx]
loc_4_011E:
        db  08Bh, 015h, 0D8h, 005h, 000h, 000h
        mov    edx, dword ptr [edx + 40h]
        cli
loc_4_0128:
        or     edx, edx
        je     SHORT loc_4_0158
        cmp    dword ptr [edx + 28h], ecx
        jne    SHORT loc_4_0153
        mov    eax, dword ptr [ecx + 28h]
        mov    dword ptr [edx + 20h], eax
        or     byte ptr [edx + 2], 80h
        test   byte ptr [ebx + 22h], 4
        jne    SHORT loc_4_0145
        and    byte ptr [edx + 2], 7fh
loc_4_0145:
        or     byte ptr [edx + 4], 40h
        test   byte ptr [ebx + 20h], 2
        je     SHORT loc_4_0153
        and    byte ptr [edx + 4], 0bfh
loc_4_0153:
        mov    edx, dword ptr [edx + 24h]
        jmp    SHORT loc_4_0128
loc_4_0158:
        sti
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_4_0178
        mov    ecx, dword ptr [ecx + 10h]
        jecxz  loc_4_0178
        cmp    ecx, ebx
        je     SHORT loc_4_011E
        and    byte ptr [ecx + 20h], 0fdh
        test   byte ptr [ebx + 20h], 2
        je     SHORT loc_4_011E
        or     byte ptr [ecx + 20h], 2
        jmp    SHORT loc_4_011E
loc_4_0178:
        mov    ebx, dword ptr [ebx]
        and    byte ptr [ebx + 0b8h], 0fdh
        db  0E8h, 086h, 0FEh, 0FBh, 0FFh
        add    esp, 4
        ret
loc_4_018A:
        mov    ebx, dword ptr [esi + 4]
        and    byte ptr [ebx + 0b8h], 0fdh
        inc    byte ptr [ebx + 78h]
        cmp    word ptr [esi + 68h], 0dh
        jne    SHORT loc_4_01A7
        mov    word ptr [esi + 6ah], ax
        db  0E9h, 009h, 001h, 0FCh, 0FFh
loc_4_01A7:
        mov    word ptr [esi + 6ah], ax
        cmp    ax, 1ch
        je     SHORT loc_4_01B7
        mov    word ptr [esi + 6ah], 18h
loc_4_01B7:
        push   esi
        db  0E8h, 0C7h, 002h, 0FCh, 0FFh
        add    esp, 4
        ret
        db  083h, 0ECh, 00Ch, 066h, 0C7h, 004h, 024h, 00Eh, 000h, 089h, 054h, 024h, 004h, 054h, 0FFh, 015h
        db  0BCh, 005h, 000h, 000h, 083h, 0C4h, 010h, 0C3h
sub_4_01D9:
        add    al, 41h
        db  0A2h, 004h, 000h, 000h, 000h
        xor    edi, edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  08Dh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   0c0005h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        mov    ecx, eax
        mov    eax, 40001010h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        jmp    SHORT loc_4_0211
loc_4_0211:
        mov    ebx, dword ptr [esi + 4]
        mov    al, byte ptr [esi + 0b1h]
        mov    byte ptr [ebx + 0aeh], al
        mov    edi, dword ptr [esi + 0a8h]
        push   word ptr [esi + 0eh]
        pop    word ptr [esi + 0ch]
        ret
loc_4_022F:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        call   sub_4_023F
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_023F:
        mov    esi, edx
        mov    word ptr [esi + 6ah], 16h
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [ebx + 1eh], 2
        db  00Fh, 084h, 0B8h, 002h, 0FCh, 0FFh
        or     byte ptr [ebx + 0b8h], 1
        test   byte ptr [esi + 6fh], 10h
        je     SHORT loc_4_02C9
        db  0E8h, 024h, 0FFh, 0FBh, 0FFh
        je     SHORT loc_4_029B
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 10000030h
        db  00Fh, 0B7h, 03Dh, 016h, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 01Ah, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        mov    ebx, dword ptr [esi + 4]
        jmp    NEAR PTR loc_4_0360
loc_4_029B:
        sub    esp, 0ch
        mov    eax, esp
        mov    word ptr [eax], 0ch
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], 1
        push   eax
        db  0FFh, 015h, 0BCh, 005h, 000h, 000h
        movzx  eax, word ptr [eax + 2]
        add    esp, 10h
        mov    word ptr [esi + 6ah], 25h
        or     eax, eax
        je     SHORT loc_4_02ED
        jmp    SHORT loc_4_0319
loc_4_02C9:
        mov    ecx, dword ptr [ebx + 10h]
loc_4_02CC:
        or     ecx, ecx
        je     SHORT loc_4_02ED
        cmp    byte ptr [ecx + 46h], 0
        je     SHORT loc_4_02E8
        push   ecx
        push   0
        movzx  ecx, byte ptr [ecx + 14h]
        push   ecx
        db  0CDh, 020h, 04Ch, 000h, 040h, 000h
        add    esp, 8
        pop    ecx
loc_4_02E8:
        mov    ecx, dword ptr [ecx + 10h]
        jmp    SHORT loc_4_02CC
loc_4_02ED:
        cmp    byte ptr [ebx + 79h], 0
        jne    SHORT loc_4_0319
loc_4_02F3:
        mov    ax, 0ah
        xor    ecx, ecx
        call   sub_4_071B
        inc    byte ptr [ebx + 78h]
        test   byte ptr [ebx + 22h], 4
        db  00Fh, 084h, 0A5h, 0FFh, 0FBh, 0FFh
        mov    eax, ebx
        mov    cl, 1
        db  0E8h, 008h, 001h, 0FCh, 0FFh
        db  0E9h, 097h, 0FFh, 0FBh, 0FFh
loc_4_0319:
        mov    ebx, dword ptr [esi + 4]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 40001011h
        test   byte ptr [esi + 6fh], 10h
        je     SHORT loc_4_0332
        mov    eax, 10000031h
loc_4_0332:
        db  00Fh, 0B7h, 03Dh, 016h, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 018h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        mov    ebx, dword ptr [esi + 4]
        cmp    eax, 1
        jne    SHORT loc_4_0360
        mov    byte ptr [ebx + 79h], 0
        jmp    SHORT loc_4_02F3
loc_4_0360:
        inc    byte ptr [ebx + 78h]
        mov    word ptr [esi + 6ah], 23h
        push   esi
        db  0E8h, 015h, 001h, 0FCh, 0FFh
        add    esp, 4
        ret
loc_4_0373:
        test   byte ptr [ebx + 21h], 20h
        db  00Fh, 085h, 0AAh, 0FCh, 0FBh, 0FFh
        cmp    word ptr [esi + 92h], 440dh
        db  00Fh, 085h, 09Bh, 0FCh, 0FBh, 0FFh
        mov    eax, dword ptr [esi + 94h]
        cmp    ax, 848h
        je     SHORT loc_4_03D0
        cmp    ax, 849h
        db  00Fh, 085h, 085h, 0FCh, 0FBh, 0FFh
        mov    dword ptr [esi + 0a0h], 0
        push   ebx
        mov    ebx, dword ptr [esi + 8]
        xor    ecx, ecx
        mov    ax, 8
        call   sub_4_0731
        mov    word ptr [esi + 6ah], ax
        pop    ebx
        call   sub_4_041D
        mov    dword ptr [esi + 0a0h], eax
        db  0E9h, 082h, 0FDh, 0FBh, 0FFh
loc_4_03D0:
        mov    word ptr [esi + 6ah], 16h
        mov    dword ptr [esi + 0a0h], 1
        mov    ecx, dword ptr [esi + 98h]
        movzx  eax, byte ptr [ecx]
        cmp    al, 2
        db  00Fh, 087h, 061h, 0FDh, 0FBh, 0FFh
        je     SHORT loc_4_040C
        add    eax, 9
        xor    ecx, ecx
        call   sub_4_0731
        mov    word ptr [esi + 6ah], ax
        call   sub_4_041D
        mov    dword ptr [esi + 0a0h], eax
loc_4_040C:
        mov    ecx, dword ptr [esi + 98h]
        mov    al, byte ptr [ebx + 79h]
        mov    byte ptr [ecx + 1], al
        db  0E9h, 035h, 0FDh, 0FBh, 0FFh
sub_4_041D:
        cmp    ax, 10h
        jb     SHORT loc_4_042D
        movzx  ecx, ax
        db  0CDh, 020h, 00Fh, 000h, 010h, 000h
        ret
loc_4_042D:
        mov    al, 0
        ret
loc_4_0430:
        db  0BEh, 030h, 004h, 000h, 000h
        db  0BFh, 042h, 004h, 000h, 000h
        call   sub_4_087E
        jae    SHORT loc_4_0442
        ret
loc_4_0442:
        mov    esi, edx
        mov    ebx, dword ptr [esi + 8]
        mov    edi, dword ptr [esi + 0a8h]
        xor    edx, edx
        xchg   dword ptr [ebx + 18h], edx
        sti
loc_4_0453:
        pushad
        test   byte ptr [ebx + 1eh], 4
        je     SHORT loc_4_0474
        xor    ecx, ecx
        mov    ax, 6
        call   sub_4_0731
        cmp    eax, 10h
        jae    SHORT loc_4_0474
        cmp    edx, edi
        jne    SHORT loc_4_0474
        test   byte ptr [ebx + 22h], 10h
        je     SHORT loc_4_04B5
loc_4_0474:
        sub    esp, 174h
        mov    ebx, esp
        push   edi
        mov    edi, ebx
        xor    eax, eax
        mov    ecx, 1dh
        rep stosd
        pop    edi
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 28h], eax
        mov    dword ptr [ebx + 4], 2
        lea    eax, [ebx + 74h]
        mov    dword ptr [ebx + 14h], eax
        push   ebx
        call   dword ptr [edi + 18h]
        cli
        add    esp, 4
        cmp    word ptr [ebx + 1ah], 0
        jne    NEAR PTR loc_4_0549
        add    esp, 174h
loc_4_04B5:
        popad
        and    dword ptr [ebx + 20h], 0ffefefffh
        and    byte ptr [edi + 4], 0fbh
        mov    dword ptr [ebx + 18h], edi
        mov    edx, dword ptr [esi + 4]
        mov    byte ptr [edx + 0b7h], 0
        dec    byte ptr [edx + 76h]
        sti
        mov    ax, 23h
        xor    ecx, ecx
        call   sub_4_071B
        push   edi
        push   edx
        push   word ptr [esi + 0eh]
        pop    word ptr [esi + 0ch]
        db  0E8h, 0C4h, 0FDh, 0FBh, 0FFh
        pop    ebx
        pop    edi
        cmp    dword ptr [ebx + 0afh], 0
        je     NEAR PTR loc_4_071A
        cli
        xor    esi, esi
        lea    edx, [ebx + 0afh]
loc_4_0504:
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_0528
        mov    ebx, dword ptr [edx]
        cmp    dword ptr [ebx + 0a8h], edi
        jne    SHORT loc_4_0521
        mov    eax, dword ptr [ebx + 64h]
        mov    dword ptr [edx], eax
        mov    eax, esi
        mov    esi, ebx
        mov    dword ptr [ebx + 64h], eax
        jmp    SHORT loc_4_0504
loc_4_0521:
        mov    edx, dword ptr [edx]
        lea    edx, [edx + 64h]
        jmp    SHORT loc_4_0504
loc_4_0528:
        sti
loc_4_0529:
        or     esi, esi
        je     NEAR PTR loc_4_071A
        push   word ptr [esi + 0eh]
        pop    word ptr [esi + 0ch]
        push   esi
        mov    eax, dword ptr [esi + 64h]
        mov    esi, eax
        db  0E8h, 0C8h, 0FAh, 0FBh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_0529
loc_4_0549:
        mov    eax, dword ptr [esi + 8]
        or     dword ptr [eax + 20h], 101000h
        sti
        test   byte ptr [esi + 6eh], 2
        jne    NEAR PTR loc_4_06DA
        test   byte ptr [eax + 22h], 4
        je     SHORT loc_4_0579
        test   byte ptr [eax + 1eh], 2
        je     SHORT loc_4_0579
        push   ebx
        mov    ebx, eax
        xor    ecx, ecx
        mov    ax, 8
        call   sub_4_071B
        pop    ebx
loc_4_0579:
        db  0A1h, 0F0h, 005h, 000h, 000h
        mov    ecx, 5
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        cld
        push   esi
        db  08Dh, 03Dh, 011h, 000h, 000h, 000h
        mov    esi, dword ptr [ebx + 14h]
        mov    ecx, 0bh
        rep movsb
        xor    al, al
        db  08Dh, 03Dh, 011h, 000h, 000h, 000h
        mov    ecx, 0bh
        repne scasb
        jne    SHORT loc_4_05B2
        dec    edi
        inc    ecx
        mov    al, 20h
        rep stosb
loc_4_05B2:
        xor    ecx, ecx
        mov    edx, dword ptr [ebx + 28h]
loc_4_05B7:
        rol    edx, 4
        mov    al, dl
        and    al, 0fh
        cmp    al, 9
        ja     SHORT loc_4_05DF
        add    al, 30h
loc_4_05C4:
        db  088h, 081h, 007h, 000h, 000h, 000h
        inc    ecx
        cmp    ecx, 4
        jne    SHORT loc_4_05D8
        db  0C6h, 081h, 007h, 000h, 000h, 000h, 02Dh
        inc    ecx
loc_4_05D8:
        cmp    ecx, 9
        je     SHORT loc_4_05E3
        jmp    SHORT loc_4_05B7
loc_4_05DF:
        add    al, 37h
        jmp    SHORT loc_4_05C4
loc_4_05E3:
        pop    edx
        mov    al, byte ptr [edx + 0b1h]
        add    al, 41h
        db  0A2h, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [edx + 4]
        cmp    byte ptr [eax + 40h], 5
        je     SHORT loc_4_064A
        db  08Dh, 03Dh, 01Dh, 000h, 000h, 000h
        db  08Dh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        db  0CDh, 020h, 058h, 001h, 001h, 000h
        lea    esp, [esp + 10h]
        xor    eax, eax
        pushfd
        cld
        repne scasb
        popfd
        push   edi
        db  08Dh, 00Dh, 011h, 000h, 000h, 000h
        db  08Dh, 015h, 007h, 000h, 000h, 000h
        db  08Dh, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   edx
        push   ecx
        push   2
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        db  0CDh, 020h, 058h, 001h, 001h, 000h
        lea    esp, [esp + 18h]
        jmp    SHORT loc_4_0698
loc_4_064A:
        db  08Dh, 03Dh, 01Dh, 000h, 000h, 000h
        db  08Dh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        db  0CDh, 020h, 058h, 001h, 001h, 000h
        lea    esp, [esp + 10h]
        xor    eax, eax
        pushfd
        cld
        repne scasb
        popfd
        push   edi
        db  08Dh, 00Dh, 011h, 000h, 000h, 000h
        db  08Dh, 015h, 007h, 000h, 000h, 000h
        db  08Dh, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   edx
        push   ecx
        push   4
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        db  0CDh, 020h, 058h, 001h, 001h, 000h
        lea    esp, [esp + 18h]
loc_4_0698:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        pop    ecx
        db  08Dh, 03Dh, 01Dh, 000h, 000h, 000h
        mov    eax, 40001011h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, 174h
        mov    ecx, eax
        db  0A1h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        cmp    ecx, 1
        popad
        jne    SHORT loc_4_06CE
        jmp    NEAR PTR loc_4_0453
loc_4_06CE:
        or     dword ptr [edi + 4], 21h
        mov    word ptr [esi + 6ah], 14h
        jmp    SHORT loc_4_06E7
loc_4_06DA:
        add    esp, 174h
        popad
        mov    word ptr [esi + 6ah], 2ah
loc_4_06E7:
        and    dword ptr [edi + 4], 0fffffffbh
        or     dword ptr [ebx + 20h], 101000h
        mov    edx, dword ptr [ebx]
        mov    byte ptr [edx + 79h], 0
        mov    ax, 24h
        xor    ecx, ecx
        call   sub_4_071B
        dec    byte ptr [edx + 76h]
        push   esi
        db  0E8h, 078h, 0FDh, 0FBh, 0FFh
        add    esp, 4
        mov    ax, 23h
        xor    ecx, ecx
        call   sub_4_071B
loc_4_071A:
        ret
sub_4_071B:
        pushad
        db  068h, 00Ch, 000h, 000h, 000h
        push   ecx
        push   0
        jmp    SHORT loc_4_0737
sub_4_0726:
        pushad
        db  068h, 00Ch, 000h, 000h, 000h
        push   ecx
        push   1
        jmp    SHORT loc_4_0737
sub_4_0731:
        pushad
        push   0
        push   ecx
        push   0
loc_4_0737:
        mov    ecx, dword ptr [ebx + 4]
        add    ecx, 64h
        sub    esp, 10h
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    ecx, 68h
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0eh], 0
        mov    word ptr [edi], 2
        push   edi
        db  0FFh, 015h, 0BCh, 005h, 000h, 000h
        mov    edi, dword ptr [edi + 0ah]
        add    esp, 14h
        or     edi, edi
        je     NEAR PTR loc_4_0835
        add    edi, dword ptr [ebx + 4]
        xor    edx, edx
        mov    dword ptr [edi + 0a8h], edx
        mov    dword ptr [edi + 7ch], edx
        mov    dword ptr [edi + 80h], edx
        mov    dword ptr [edi + 78h], edx
        mov    dword ptr [edi + 64h], edx
        mov    word ptr [edi + 68h], ax
        mov    dword ptr [edi + 6ch], 10d09h
        pop    edx
        or     edx, edx
        je     SHORT loc_4_07A8
        and    dword ptr [edi + 6ch], 0fffff6ffh
        db  0C7h, 047h, 070h, 03Eh, 008h, 000h, 000h
        mov    dword ptr [edi + 0a4h], edi
loc_4_07A8:
        pop    edx
        or     edx, edx
        je     SHORT loc_4_07E2
        mov    dword ptr [edi + 7ch], 1
        mov    dword ptr [edi + 74h], esi
        mov    dword ptr [edi + 0ach], edi
        add    dword ptr [edi + 0ach], 0bch
        mov    dword ptr [edi + 80h], edx
        mov    dword ptr [edi + 8], ebx
        push   edi
        db  0FFh, 015h, 0D0h, 005h, 000h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_07E2
        or     dword ptr [edi + 6ch], 40h
loc_4_07E2:
        mov    al, byte ptr [ebx + 14h]
        test   byte ptr [ebx + 1dh], 20h
        je     SHORT loc_4_07F9
        mov    al, 0
        or     edx, edx
        jne    SHORT loc_4_07F9
        mov    eax, dword ptr [ebx]
        mov    al, byte ptr [eax + 0aeh]
loc_4_07F9:
        mov    byte ptr [edi + 0b1h], al
        pop    edx
        mov    esi, dword ptr [edi + 6ch]
        db  0FFh, 015h, 0C8h, 005h, 000h, 000h
        test   esi, 100h
        je     SHORT loc_4_0833
loc_4_0811:
        movzx  eax, word ptr [edi + 6ah]
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        db  0FFh, 015h, 0BCh, 005h, 000h, 000h
        add    esp, 0ch
        mov    dword ptr [esp + 1ch], eax
loc_4_0833:
        popad
        ret
loc_4_0835:
        add    esp, 0ch
        mov    ax, 1dh
        jmp    SHORT loc_4_0833
loc_4_083E:
        pushad
        mov    edi, dword ptr [esp + 24h]
        mov    ebx, dword ptr [edi + 8]
        jmp    SHORT loc_4_0811
loc_4_0848:
        push   ebx
        sub    esp, 0ch
        movzx  ecx, byte ptr [ebx + 14h]
        test   byte ptr [ebx + 1dh], 20h
        je     SHORT loc_4_0858
        xor    ecx, ecx
loc_4_0858:
        or     ecx, 40000000h
        mov    dword ptr [esp + 8], ecx
        mov    word ptr [esp], 7
        push   esp
        db  0FFh, 015h, 0BCh, 005h, 000h, 000h
        pop    esp
        mov    ebx, dword ptr [esp + 4]
        add    esp, 0ch
        call   sub_4_0731
        pop    ebx
        ret
sub_4_087E:
        mov    ecx, dword ptr [edx + 8]
        test   byte ptr [ecx + 21h], 20h
        je     SHORT loc_4_08DC
        mov    dword ptr [edx + 60h], esi
        mov    dword ptr [edx + 20h], edi
        db  0BEh, 0B1h, 008h, 000h, 000h
        xor    ebx, ebx
        mov    ecx, 820h
        mov    eax, 10h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        stc
        ret
loc_4_08A6:
        mov    eax, 64h
        db  0CDh, 020h, 03Ch, 080h, 001h, 000h
loc_4_08B1:
        db  08Bh, 00Dh, 0D8h, 005h, 000h, 000h
        mov    eax, dword ptr [ecx + 3ch]
        mov    esi, dword ptr [edx + 60h]
        push   eax
        push   ecx
        call   sub_4_08FD
        pop    ecx
        pop    eax
        jb     SHORT loc_4_08A6
        cmp    dword ptr [eax + 107h], 1
        je     SHORT loc_4_08A6
        mov    eax, dword ptr [edx + 20h]
        test   byte ptr [ecx + 28h], 80h
        jne    SHORT loc_4_08A6
        jmp    eax
loc_4_08DC:
        ret
        db  033h, 009h, 000h, 000h, 033h, 009h, 000h, 000h, 039h, 009h, 000h, 000h, 039h, 009h, 000h, 000h
        db  035h, 009h, 000h, 000h, 035h, 009h, 000h, 000h, 033h, 009h, 000h, 000h, 035h, 009h, 000h, 000h
sub_4_08FD:
        mov    eax, dword ptr [ebx + 4]
        db  003h, 005h, 0F4h, 005h, 000h, 000h
        xor    ecx, ecx
        cmp    byte ptr [eax + 12b9h], 0
        je     SHORT loc_4_0914
        or     ecx, 1
loc_4_0914:
        cmp    byte ptr [eax + 12b8h], 0
        je     SHORT loc_4_0920
        or     ecx, 2
loc_4_0920:
        cmp    byte ptr [eax + 321h], 0
        je     SHORT loc_4_092C
        or     ecx, 4
loc_4_092C:
        db  0FFh, 024h, 08Dh, 0DDh, 008h, 000h, 000h
loc_4_0933:
        clc
        ret
loc_4_0935:
        xor    ecx, ecx
        stc
        ret
loc_4_0939:
        or     ecx, 0ffffffffh
        stc
        ret
        db  000h, 000h
loc_4_0940:
        mov    esi, dword ptr [ebx + 0ch]
        mov    eax, dword ptr [esi + 20h]
        test   eax, 4
        je     SHORT loc_4_09BB
        cmp    byte ptr [esi + 40h], 0ah
        je     SHORT loc_4_096A
        test   eax, 4000h
        je     SHORT loc_4_09BB
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_4_096A
        cmp    byte ptr [esi + 40h], 5
        je     SHORT loc_4_096A
        jmp    SHORT loc_4_09BB
loc_4_0968:
        jmp    SHORT loc_4_09BB
loc_4_096A:
        test   dword ptr [esi + 1ch], 23h
        jne    SHORT loc_4_0968
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        push   ebp
        sub    esp, 18h
        mov    ebp, esp
        mov    dword ptr [ebp + 4], esi
        db  0C7h, 045h, 008h, 00Ch, 000h, 000h, 000h
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ebp + 16h], al
        xor    eax, eax
        mov    dword ptr [ebp + 0ch], eax
        mov    word ptr [ebp + 10h], ax
        mov    eax, dword ptr [esi + 1ch]
        and    eax, 0ffffffe3h
        mov    dword ptr [ebp + 12h], eax
        mov    word ptr [ebp], 5
        push   ebp
        db  0FFh, 015h, 0BCh, 005h, 000h, 000h
        add    esp, 1ch
        pop    ebp
        mov    esi, dword ptr [esi]
        or     dword ptr [esi + 20h], 80000h
loc_4_09BB:
        ret
loc_4_09BC:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_09CB
        mov    word ptr [ebx + 2], 0ffffh
loc_4_09CB:
        ret
loc_4_09CC:
        db  08Bh, 015h, 0D8h, 005h, 000h, 000h
        mov    eax, dword ptr [edx + 30h]
loc_4_09D5:
        or     eax, eax
        je     SHORT loc_4_0A14
        test   byte ptr [eax + 21h], 80h
        je     SHORT loc_4_0A0F
        cmp    byte ptr [eax + 40h], 0
        je     SHORT loc_4_09F1
        cmp    byte ptr [eax + 40h], 0ah
        je     SHORT loc_4_09F1
        cmp    byte ptr [eax + 40h], 5
        jne    SHORT loc_4_0A0F
loc_4_09F1:
        cmp    byte ptr [eax + 79h], 0
        jne    SHORT loc_4_0A00
        cmp    dword ptr [eax + 0b3h], 0
        je     SHORT loc_4_0A0F
loc_4_0A00:
        push   eax
        mov    ebx, eax
        mov    ax, 0ah
        xor    ecx, ecx
        call   sub_4_071B
        pop    eax
loc_4_0A0F:
        mov    eax, dword ptr [eax + 0ch]
        jmp    SHORT loc_4_09D5
loc_4_0A14:
        ret
        db  000h, 000h, 000h
loc_4_0A18:
        db  0BEh, 018h, 00Ah, 000h, 000h
        db  0BFh, 02Ah, 00Ah, 000h, 000h
        call   sub_4_087E
        jae    SHORT loc_4_0A2A
        ret
loc_4_0A2A:
        push   esi
        push   edi
        push   ebx
        mov    esi, edx
        mov    ebx, dword ptr [esi + 4]
        mov    edi, dword ptr [esi + 0a8h]
        cmp    word ptr [esi + 6ah], 19h
        je     SHORT loc_4_0A51
        cmp    word ptr [esi + 6ah], 18h
        je     SHORT loc_4_0A51
        cmp    word ptr [esi + 6ah], 20h
        jne    NEAR PTR loc_4_0B09
loc_4_0A51:
        test   byte ptr [ebx + 22h], 8
        jne    SHORT loc_4_0A57
loc_4_0A57:
        db  08Bh, 015h, 0D8h, 005h, 000h, 000h
        mov    eax, ebx
        mov    cl, 1
        db  0E8h, 0B6h, 0F9h, 0FBh, 0FFh
        cmp    byte ptr [ebx + 79h], 0
        je     SHORT loc_4_0A7D
        test   byte ptr [ebx + 1eh], 1
        je     SHORT loc_4_0A7D
        mov    ax, 9
        xor    ecx, ecx
        call   sub_4_071B
loc_4_0A7D:
        btr    dword ptr [esi + 6ch], 14h
        jae    SHORT loc_4_0A8D
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 28h]
        sub    dword ptr [esi + 74h], eax
loc_4_0A8D:
        test   byte ptr [esi + 6ch], 8
        jne    SHORT loc_4_0AD0
        cmp    dword ptr [esi + 0a8h], 0
        je     SHORT loc_4_0B09
        test   byte ptr [edi + 4], 20h
        je     SHORT loc_4_0AAA
        mov    word ptr [esi + 6ah], 11h
        jmp    SHORT loc_4_0B09
loc_4_0AAA:
        db  0E8h, 0A1h, 0FAh, 0FBh, 0FFh
        cli
        test   byte ptr [edi + 4], 4
        je     SHORT loc_4_0B23
        dec    byte ptr [ebx + 78h]
        dec    byte ptr [ebx + 76h]
        mov    eax, dword ptr [ebx + 0afh]
        mov    dword ptr [esi + 64h], eax
        mov    dword ptr [ebx + 0afh], esi
        sti
loc_4_0ACC:
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_4_0AD0:
        cmp    word ptr [esi + 68h], 6
        je     SHORT loc_4_0B09
        cmp    word ptr [esi + 6ah], 19h
        jne    SHORT loc_4_0B09
        bts    dword ptr [esi + 6ch], 17h
        jb     SHORT loc_4_0B09
        dec    byte ptr [ebx + 76h]
        dec    byte ptr [ebx + 78h]
        mov    ebx, dword ptr [esi + 8]
        xor    ecx, ecx
        mov    ax, 6
        call   sub_4_0731
        db  0E8h, 052h, 0FAh, 0FBh, 0FFh
        push   esi
        db  0E8h, 008h, 0F5h, 0FBh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_0ACC
loc_4_0B09:
        dec    byte ptr [ebx + 76h]
        mov    edx, dword ptr [esi + 8]
        xchg   ebx, edx
        mov    ax, 23h
        xor    ecx, ecx
        call   sub_4_071B
        mov    ebx, edx
        db  0E9h, 088h, 0F9h, 0FBh, 0FFh
loc_4_0B23:
        or     dword ptr [edi + 4], 4
        sti
        mov    ebx, dword ptr [esi + 8]
        db  0E8h, 03Ch, 0F8h, 0FBh, 0FFh
        jmp    SHORT loc_4_0ACC
sub_4_0B32:
        lea    edx, [ebx + 0afh]
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_0B62
loc_4_0B3D:
        mov    esi, dword ptr [edx]
        cmp    dword ptr [esi + 64h], 0
        je     SHORT loc_4_0B4A
        lea    edx, [esi + 64h]
        jmp    SHORT loc_4_0B3D
loc_4_0B4A:
        mov    dword ptr [edx], 0
        sti
        push   word ptr [esi + 0eh]
        pop    word ptr [esi + 0ch]
        push   esi
        db  0E8h, 0ADh, 0F4h, 0FBh, 0FFh
        add    esp, 4
loc_4_0B62:
        sti
        ret
loc_4_0B64:
        mov    edx, dword ptr [esp + 4]
        sub    esp, 0ch
        mov    ecx, esp
        mov    word ptr [ecx], 0dh
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], 1
        push   ecx
        db  0FFh, 015h, 0BCh, 005h, 000h, 000h
        add    esp, 10h
        ret
        db  000h
_PTEXT ends
        end
