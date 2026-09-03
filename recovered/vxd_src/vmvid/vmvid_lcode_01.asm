; vmvid_lcode_01.asm -- VMVID object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  083h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  085h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  084h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  086h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
loc_1_0040:
        sal    byte ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0050:
        sal    dword ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 002h, 032h, 003h, 000h, 000h, 000h, 056h, 064h, 073h, 070h
        db  044h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00E0:
        add    al, 0
        mov    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    dword ptr [ebx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    al, byte ptr [ebx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    eax, dword ptr [ebx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00FA:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0108:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_010C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0110:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0114:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0118:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_011C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0120:
        add    byte ptr [eax], al
loc_1_0122:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0126:
        add    byte ptr [eax], al
loc_1_0128:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_012C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0130:
        and    byte ptr [edx], al
loc_1_0132:
        pop    es
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], dh
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_01D6:
        push   es
        add    byte ptr [edi], al
        add    byte ptr [edx], cl
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0f000e00h
        add    ah, cl
        int3
VdspD_Control:
        cmp    eax, 0bh
        je     NEAR PTR loc_1_0377
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_01FA:
        cmp    eax, 1ch
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0203:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_020C:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0215:
        clc
        ret
loc_1_0217:
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_023C
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_023A
        pop    eax
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 080h, 003h, 000h
        jmp    SHORT loc_1_0243
loc_1_023A:
        mov    ebx, eax
loc_1_023C:
        pop    eax
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_0243:
        clc
        ret
loc_1_0245:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 080h, 003h, 000h
loc_1_0251:
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_026B
        jecxz  loc_1_0263
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_1_0271
loc_1_0263:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_1_0271
loc_1_026B:
        db  0CDh, 020h, 00Ah, 000h, 003h, 000h
loc_1_0271:
        clc
        ret
sub_1_0273:
        push   ecx
        push   edx
        add    dx, 0eh
        mov    ecx, 3e8h
loc_1_027E:
        in     al, dx
        test   al, 80h
        jne    SHORT loc_1_0288
        loop   loc_1_027E
        stc
        jmp    SHORT loc_1_028E
loc_1_0288:
        sub    dx, 4
        in     al, dx
        clc
loc_1_028E:
        pop    edx
        pop    ecx
        ret
sub_1_0291:
        push   edx
        push   ecx
        push   eax
        add    dx, 0ch
        mov    ecx, 100000h
loc_1_029D:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_02A8
        loop   loc_1_029D
        pop    eax
        stc
        jmp    SHORT loc_1_02AB
loc_1_02A8:
        pop    eax
        out    dx, al
        clc
loc_1_02AB:
        pop    ecx
        pop    edx
        ret
sub_1_02AE:
        push   eax
        push   ecx
        push   edx
        add    dx, 6
        mov    al, 1
        out    dx, al
        mov    ecx, 0c8h
loc_1_02BD:
        loop   loc_1_02BD
        xor    al, al
        out    dx, al
        mov    ecx, 19h
        pop    edx
        push   edx
loc_1_02C9:
        call   sub_1_0273
        jb     SHORT loc_1_02D4
        cmp    al, 0aah
        je     SHORT loc_1_02D9
loc_1_02D4:
        loop   loc_1_02C9
        stc
        jmp    SHORT loc_1_02E0
loc_1_02D9:
        mov    al, 0d1h
        call   sub_1_0291
loc_1_02E0:
        pop    edx
        pop    ecx
        pop    eax
        ret
loc_1_02E4:
        mov    al, 0e1h
        call   sub_1_0291
        jb     SHORT loc_1_02FD
        call   sub_1_0273
        jb     SHORT loc_1_02FD
        mov    ah, al
        call   sub_1_0273
        jae    SHORT loc_1_0302
loc_1_02FD:
        xor    ax, ax
        stc
        ret
loc_1_0302:
        clc
        ret
loc_1_0304:
        xor    ah, ah
        mov    al, 0fah
        call   sub_1_0291
        jb     SHORT loc_1_0316
        call   sub_1_0273
        jae    SHORT loc_1_031B
loc_1_0316:
        xor    ax, ax
        stc
        ret
loc_1_031B:
        clc
        ret
sub_1_031D:
        push   esi
        push   eax
        push   ecx
        mov    ecx, 8
        db  0BEh, 000h, 000h, 000h, 000h
loc_1_032A:
        mov    edx, 0
        mov    dx, word ptr [esi]
        db  066h, 003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        add    esi, 2
        loop   loc_1_032A
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_1_0349:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    ecx, 8
        db  0BEh, 000h, 000h, 000h, 000h
loc_1_0357:
        mov    edx, 0
        mov    dx, word ptr [esi]
        db  066h, 003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        add    esi, 2
        loop   loc_1_0357
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
loc_1_0377:
        push   eax
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_03AE
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        mov    eax, 2
        call   sub_1_03E4
        or     edx, edx
        je     SHORT loc_1_03E1
        db  00Fh, 0B7h, 015h, 000h, 000h, 000h, 000h
        call   sub_1_02AE
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_03AE:
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_03C5
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_03BB:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_03C5:
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_03E1
        mov    eax, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_03D7:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_03E1:
        pop    eax
        clc
        ret
sub_1_03E4:
        push   eax
        push   edx
        test   eax, 2
        je     SHORT loc_1_0431
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0431
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        call   sub_1_031D
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    edx, ebx
        db  003h, 015h, 000h, 000h, 000h, 000h
        and    dword ptr [edx], 0fffffffeh
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 003h, 000h
loc_1_0431:
        pop    edx
        pop    eax
        clc
        ret
sub_1_0435:
        push   eax
        push   ecx
        push   edx
        test   eax, 2
        je     SHORT loc_1_0494
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0453
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0493
        stc
        jb     SHORT loc_1_0494
loc_1_0453:
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        call   sub_1_0349
        test   eax, 4
        je     SHORT loc_1_046D
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0482
loc_1_046D:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0482
        db  00Fh, 0B7h, 015h, 000h, 000h, 000h, 000h
        call   sub_1_02AE
loc_1_0482:
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 003h, 000h
loc_1_0493:
        clc
loc_1_0494:
        pop    edx
        pop    ecx
        pop    eax
        ret
VdspD_Service_00:
        mov    eax, 400h
        clc
        ret
VdspD_Service_01:
        db  066h, 039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_04AA
        stc
        ret
loc_1_04AA:
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        clc
        ret
sub_1_04B2:
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        test   dword ptr [esi], 1
        jne    SHORT loc_1_04EC
        or     dword ptr [esi], 1
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        jne    SHORT loc_1_04EC
        mov    eax, 30h
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
loc_1_04EC:
        ret
loc_1_04ED:
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, ebx
        je     SHORT loc_1_050D
        or     eax, eax
        jne    SHORT loc_1_0520
        push   ebx
        push   ecx
        push   edx
        mov    eax, 6
        call   sub_1_0435
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_1_0520
loc_1_050D:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_1_0519
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0519:
        je     SHORT loc_1_051E
        in     al, dx
        jmp    SHORT loc_1_051F
loc_1_051E:
        out    dx, al
loc_1_051F:
        ret
loc_1_0520:
        pop    eax
        call   sub_1_04B2
        xor    eax, eax
        dec    eax
        ret
VdspD_PM_API:
        movzx  eax, word ptr [ebp + 14h]
        cmp    ah, 7
        jae    SHORT loc_1_0554
        movzx  ecx, ah
        shl    ecx, 3
        db  08Dh, 089h, 000h, 000h, 000h, 000h
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    SHORT loc_1_0554
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     SHORT loc_1_0554
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_1_0554:
        or     dword ptr [ebp + 2ch], 1
        ret
loc_1_0559:
        db  0CDh, 020h, 000h, 000h, 002h, 032h
        mov    word ptr [ebp + 1ch], ax
        clc
        ret
loc_1_0565:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_1_057A
        db  066h, 039h, 015h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0582
loc_1_057A:
        mov    word ptr [ebp + 1ch], 1
loc_1_0580:
        stc
        ret
loc_1_0582:
        mov    word ptr [ebp + 1ch], 0
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0596
        mov    word ptr [ebp + 1ch], 8000h
loc_1_0596:
        call   sub_1_0435
        jb     SHORT loc_1_059E
        ret
loc_1_059E:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_1_0580
loc_1_05A6:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_1_05BB
        db  066h, 039h, 015h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_05C3
loc_1_05BB:
        mov    word ptr [ebp + 1ch], 1
loc_1_05C1:
        stc
        ret
loc_1_05C3:
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_05D3
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_1_05C1
loc_1_05D3:
        call   sub_1_03E4
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_1_05E0:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 1ch], ax
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 14h], ax
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    ch, al
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    cl, al
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        shl    ax, 1
        shl    ax, 1
        shl    ax, 1
        shl    ax, 1
        or     cl, al
        mov    word ptr [ebp + 18h], cx
        clc
        ret
loc_1_062D:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 1ch], ax
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 14h], ax
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 18h], ax
        clc
        ret
loc_1_065B:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_068F
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 18h], cx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 1ch], ax
        ror    eax, 10h
        mov    word ptr [ebp + 14h], ax
        ror    eax, 10h
        db  066h, 08Bh, 01Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 10h], bx
        jmp    SHORT loc_1_069D
loc_1_068F:
        mov    word ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 14h], 0
        stc
        ret
loc_1_069D:
        clc
        ret
loc_1_069F:
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ax, 0feh
        call   sub_1_0291
        call   sub_1_0273
        mov    ah, al
        call   sub_1_0273
        mov    word ptr [ebp + 1ch], ax
        ret
_LTEXT ends
        end
