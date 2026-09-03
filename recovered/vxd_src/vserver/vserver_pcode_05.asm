; vserver_pcode_05.asm -- VSERVER object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        db  081h, 03Dh, 0EEh, 08Bh, 000h, 000h, 000h, 0F0h, 0FFh, 0FFh
        je     SHORT loc_5_000E
loc_5_000C:
        clc
        ret
loc_5_000E:
        db  0F7h, 005h, 038h, 034h, 001h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_5_000C
        db  083h, 00Dh, 038h, 034h, 001h, 000h, 008h
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        db  0CDh, 020h, 0FFh, 000h, 001h, 000h
        test   eax, eax
        je     SHORT loc_5_0068
        db  0A3h, 0EEh, 08Bh, 000h, 000h
        db  0BEh, 0E4h, 08Bh, 000h, 000h
        db  0CDh, 020h, 004h, 001h, 001h, 000h
        db  0A1h, 00Ch, 031h, 001h, 000h
        db  0E8h, 0D5h, 07Eh, 0FAh, 0FFh
        call   sub_5_05E8
        db  0F7h, 005h, 038h, 034h, 001h, 000h, 005h, 000h, 000h, 000h
        je     SHORT loc_5_0061
        db  0E8h, 02Eh, 07Dh, 0FAh, 0FFh
loc_5_0061:
        db  083h, 00Dh, 038h, 034h, 001h, 000h, 010h
loc_5_0068:
        clc
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
        db  0FFh, 035h, 0EEh, 08Bh, 000h, 000h, 0CDh, 020h, 000h, 001h, 001h, 000h, 059h, 0C7h, 005h, 0EEh
        db  08Bh, 000h, 000h, 000h, 0F0h, 0FFh, 0FFh, 0EBh, 0DFh, 000h, 000h, 000h
sub_5_008C:
        push   ebp
        mov    ebp, esp
        sub    esp, 48h
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 10h
        push   0
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 4]
        push   eax
        call   sub_5_0CB4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_00B8
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_5_00B8:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        push   1
        db  068h, 05Ch, 034h, 001h, 000h
        push   0
        push   dword ptr [ebp - 4]
        call   sub_5_0CC4
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_5_00E0
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
loc_5_00E0:
        push   1
        lea    eax, [ebp - 48h]
        push   eax
        db  0E8h, 089h, 065h, 0FAh, 0FFh
        add    esp, 8
        lea    esi, [ebp - 48h]
        db  0BFh, 064h, 034h, 001h, 000h
        mov    ecx, 2
        repe cmpsd
        jne    SHORT loc_5_0110
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    SHORT loc_5_0110
        mov    eax, 1
        pop    edi
        pop    esi
        leave
        ret
loc_5_0110:
        lea    esi, [ebp - 48h]
        db  0BFh, 070h, 034h, 001h, 000h
        mov    ecx, 2
        repe cmpsd
        jne    SHORT loc_5_0130
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
        jne    SHORT loc_5_0130
        mov    eax, 2
        pop    edi
        pop    esi
        leave
        ret
loc_5_0130:
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
sub_5_0138:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        mov    dword ptr [ebp - 4], 10h
        mov    ecx, dword ptr [ebp + 8]
        or     ecx, ecx
        jne    SHORT loc_5_0150
        sub    eax, eax
        leave
        ret
loc_5_0150:
        push   0
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 44h]
        push   eax
        push   1
        db  068h, 07Ch, 034h, 001h, 000h
        push   0
        push   ecx
        call   sub_5_0CC4
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_5_0174
        sub    eax, eax
        leave
        ret
loc_5_0174:
        push   1
        lea    eax, [ebp - 44h]
        push   eax
        db  0E8h, 0F5h, 064h, 0FAh, 0FFh
        add    esp, 8
        lea    ecx, [ebp - 44h]
        db  0BAh, 08Ch, 034h, 001h, 000h
        mov    eax, dword ptr [ecx]
        cmp    eax, dword ptr [edx]
        jne    SHORT loc_5_01AC
        mov    ax, word ptr [ecx + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_5_01AC
        mov    al, byte ptr [ecx + 6]
        cmp    al, byte ptr [edx + 6]
        jne    SHORT loc_5_01AC
        mov    eax, 1
        leave
        ret
        db  08Dh, 049h, 000h
loc_5_01AC:
        sub    eax, eax
        leave
        ret
sub_5_01B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        sub    eax, eax
        db  08Ah, 01Dh, 058h, 034h, 001h, 000h
        cmp    bl, al
        je     SHORT loc_5_01D8
        db  0BAh, 0D4h, 04Fh, 001h, 000h
        mov    ecx, dword ptr [ebp + 8]
loc_5_01C9:
        cmp    dword ptr [edx], ecx
        je     SHORT loc_5_01E0
        add    edx, 4
        inc    eax
        movzx  esi, bl
        cmp    esi, eax
        ja     SHORT loc_5_01C9
loc_5_01D8:
        sub    eax, eax
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_5_01E0:
        inc    eax
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
sub_5_01E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], esi
        push   esi
        call   sub_5_008C
        add    esp, 4
        cmp    eax, 1
        je     SHORT loc_5_0210
        cmp    eax, 2
        je     SHORT loc_5_0228
        sub    eax, eax
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_5_0210:
        push   0
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_5_0CB4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_0228
        sub    eax, eax
        pop    esi
        leave
        ret
loc_5_0228:
        push   0
        push   dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        call   sub_5_0CB4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_0244
        sub    eax, eax
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_5_0244:
        pop    esi
        mov    eax, dword ptr [ebp - 4]
        leave
        ret
        db  08Bh, 0FFh
sub_5_024C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_5_01E8
        add    esp, 4
        mov    esi, eax
        db  080h, 03Dh, 058h, 034h, 001h, 000h, 011h
        jae    SHORT loc_5_028E
        push   esi
        call   sub_5_01B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_028E
        db  00Fh, 0B6h, 005h, 058h, 034h, 001h, 000h
        db  089h, 034h, 085h, 0D4h, 04Fh, 001h, 000h
        db  0C6h, 080h, 0C0h, 04Fh, 001h, 000h, 000h
        db  0FEh, 005h, 058h, 034h, 001h, 000h
loc_5_028E:
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_5_0294:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  081h, 03Ch, 085h, 0B0h, 01Bh, 001h, 000h, 000h, 0F0h, 0FFh, 0FFh
        je     SHORT loc_5_02CF
        cmp    eax, 10h
        jne    SHORT loc_5_02B4
        db  0FFh, 00Dh, 034h, 034h, 001h, 000h
        jne    SHORT loc_5_02CF
loc_5_02B4:
        db  068h, 030h, 01Ch, 001h, 000h
        db  083h, 00Dh, 038h, 034h, 001h, 000h, 020h
        db  080h, 088h, 0B8h, 01Ch, 001h, 000h, 040h
        db  0E8h, 0F0h, 088h, 0FAh, 0FFh
        add    esp, 4
loc_5_02CF:
        leave
        ret
        db  08Dh, 049h, 000h
sub_5_02D4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
        db  081h, 03Ch, 0B5h, 0B0h, 01Bh, 001h, 000h, 000h, 0F0h, 0FFh, 0FFh
        jne    NEAR PTR loc_5_03AC
        push   esi
        db  0E8h, 009h, 0E6h, 0FAh, 0FFh
        add    esp, 4
        mov    ebx, eax
        test   bl, 1
        jne    SHORT loc_5_030C
        mov    edi, 13h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_5_030C:
        push   0
        db  080h, 0A6h, 0B8h, 01Ch, 001h, 000h, 03Fh
        db  0C6h, 086h, 044h, 034h, 001h, 000h, 000h
        push   esi
        db  0E8h, 0EDh, 078h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_0338
        mov    edi, 13h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_5_0338:
        cmp    esi, 10h
        jne    SHORT loc_5_0343
        db  0FFh, 005h, 034h, 034h, 001h, 000h
loc_5_0343:
        test   bl, 2
        je     SHORT loc_5_034F
        db  080h, 08Eh, 0B8h, 01Ch, 001h, 000h, 020h
loc_5_034F:
        db  0F6h, 005h, 038h, 034h, 001h, 000h, 010h
        je     SHORT loc_5_0366
        db  0F6h, 005h, 038h, 034h, 001h, 000h, 004h
        jne    SHORT loc_5_0366
        db  0E8h, 029h, 07Ah, 0FAh, 0FFh
loc_5_0366:
        push   esi
        db  083h, 00Dh, 038h, 034h, 001h, 000h, 004h
        db  068h, 0DCh, 0E9h, 000h, 000h
        db  0E8h, 063h, 0E5h, 0FAh, 0FFh
        add    esp, 8
        db  080h, 03Dh, 040h, 034h, 001h, 000h, 000h
        je     SHORT loc_5_03B7
        push   esi
        push   200h
        push   200h
        db  0E8h, 03Bh, 0E5h, 0FAh, 0FFh
        add    esp, 0ch
        push   0ah
        push   esi
        db  0E8h, 04Eh, 0E5h, 0FAh, 0FFh
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_5_03AC:
        cmp    esi, 10h
        jne    SHORT loc_5_03B7
        db  0FFh, 005h, 034h, 034h, 001h, 000h
loc_5_03B7:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_5_03C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_5_03D9
        push   edi
        call   sub_5_01E8
        add    esp, 4
        mov    edi, eax
loc_5_03D9:
        push   edi
        db  0E8h, 039h, 07Ch, 0FAh, 0FFh
        add    esp, 4
        mov    esi, eax
        cmp    esi, 0ffh
        jbe    SHORT loc_5_03FE
        push   edi
        call   sub_5_0138
        add    esp, 4
        or     eax, eax
        je     SHORT loc_5_03FE
        mov    esi, 10h
loc_5_03FE:
        mov    eax, esi
        pop    edi
        pop    esi
        leave
        ret
sub_5_0404:
        db  080h, 03Dh, 058h, 034h, 001h, 000h, 000h
        je     SHORT loc_5_0463
loc_5_040D:
        push   0
        db  0FEh, 00Dh, 058h, 034h, 001h, 000h
        db  00Fh, 0B6h, 005h, 058h, 034h, 001h, 000h
        db  0FFh, 034h, 085h, 0D4h, 04Fh, 001h, 000h
        call   sub_5_03C0
        add    esp, 8
        cmp    eax, -1
        je     SHORT loc_5_045A
        db  00Fh, 0B6h, 00Dh, 058h, 034h, 001h, 000h
        db  08Ah, 089h, 0C0h, 04Fh, 001h, 000h
        db  088h, 088h, 044h, 034h, 001h, 000h
        cmp    cl, 4
        jb     SHORT loc_5_045A
        db  080h, 088h, 0B8h, 01Ch, 001h, 000h, 080h
        cmp    eax, 10h
        jne    SHORT loc_5_045A
        db  0FFh, 005h, 034h, 034h, 001h, 000h
loc_5_045A:
        db  080h, 03Dh, 058h, 034h, 001h, 000h, 000h
        jne    SHORT loc_5_040D
loc_5_0463:
        ret
loc_5_0464:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   1
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        call   sub_5_03C0
        add    esp, 8
        mov    esi, eax
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        cmp    eax, 0dh
        ja     SHORT loc_5_048C
        db  0FFh, 024h, 085h, 044h, 005h, 000h, 000h
        db  08Dh, 049h, 000h
loc_5_048C:
        pop    edi
        mov    eax, 1
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_5_0498:
        push   edi
        call   sub_5_01E8
        add    esp, 4
        push   eax
        call   sub_5_01B0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_5_04BC
        db  080h, 080h, 0BFh, 04Fh, 001h, 000h, 002h
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_5_04BC:
        cmp    esi, -1
        jne    SHORT loc_5_04CC
        mov    eax, 13h
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_5_04CC:
        db  08Ah, 08Eh, 044h, 034h, 001h, 000h
        inc    cl
        db  088h, 08Eh, 044h, 034h, 001h, 000h
        db  0F6h, 005h, 038h, 034h, 001h, 000h, 008h
        je     SHORT loc_5_04F8
        cmp    cl, 4
        jb     SHORT loc_5_04F8
        push   0
        push   esi
        call   sub_5_02D4
        add    esp, 8
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_5_04F8:
        sub    eax, eax
        cmp    cl, 4
        jb     SHORT loc_5_053F
        db  080h, 08Eh, 0B8h, 01Ch, 001h, 000h, 080h
        cmp    esi, 10h
        jne    SHORT loc_5_053F
        db  0FFh, 005h, 034h, 034h, 001h, 000h
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_5_0518:
        cmp    esi, -1
        je     SHORT loc_5_053D
        db  0C6h, 086h, 044h, 034h, 001h, 000h, 000h
        db  080h, 0A6h, 0B8h, 01Ch, 001h, 000h, 07Fh
        db  0F6h, 005h, 038h, 034h, 001h, 000h, 008h
        je     SHORT loc_5_053D
        push   esi
        call   sub_5_0294
        add    esp, 4
loc_5_053D:
        sub    eax, eax
loc_5_053F:
        pop    edi
        pop    esi
        leave
        ret
        db  090h, 098h, 004h, 000h, 000h, 018h, 005h, 000h, 000h, 03Dh, 005h, 000h, 000h, 03Dh, 005h, 000h
        db  000h, 08Ch, 004h, 000h, 000h, 08Ch, 004h, 000h, 000h, 08Ch, 004h, 000h, 000h, 08Ch, 004h, 000h
        db  000h, 08Ch, 004h, 000h, 000h, 08Ch, 004h, 000h, 000h, 08Ch, 004h, 000h, 000h, 018h, 005h, 000h
        db  000h, 08Ch, 004h, 000h, 000h, 03Dh, 005h, 000h, 000h
sub_5_057C:
        push   ebp
        mov    ebp, esp
        push   esi
        db  0E8h, 013h, 0E0h, 0FAh, 0FFh
        or     eax, eax
        jne    SHORT loc_5_0594
        mov    eax, 1
        pop    esi
        leave
        ret    8
        db  090h
loc_5_0594:
        push   1
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_5_03C0
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_5_05B4
        push   esi
        call   sub_5_024C
        add    esp, 4
        jmp    SHORT loc_5_05CE
        db  08Bh, 0FFh
loc_5_05B4:
        db  08Ah, 088h, 044h, 034h, 001h, 000h
        inc    cl
        db  088h, 088h, 044h, 034h, 001h, 000h
        cmp    cl, 4
        jb     SHORT loc_5_05CE
        db  080h, 088h, 0B8h, 01Ch, 001h, 000h, 080h
loc_5_05CE:
        push   0
        push   0
        db  068h, 064h, 004h, 000h, 000h
        push   esi
        call   sub_5_0CBC
        add    esp, 10h
        pop    esi
        leave
        ret    8
        db  08Dh, 049h, 000h
sub_5_05E8:
        push   esi
        db  0E8h, 04Ah, 0B3h, 0FAh, 0FFh
        call   sub_5_0404
        sub    esi, esi
loc_5_05F5:
        db  0F6h, 086h, 0B8h, 01Ch, 001h, 000h, 080h
        je     SHORT loc_5_0626
        push   0
        push   esi
        call   sub_5_02D4
        add    esp, 8
        cmp    esi, 10h
        jne    SHORT loc_5_0626
        or     eax, eax
        je     SHORT loc_5_0620
        db  0C7h, 005h, 034h, 034h, 001h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_0626
        db  08Bh, 0FFh
loc_5_0620:
        db  0FFh, 00Dh, 034h, 034h, 001h, 000h
loc_5_0626:
        inc    esi
        cmp    esi, 11h
        jl     SHORT loc_5_05F5
        pop    esi
        ret
        db  08Bh, 0FFh
sub_5_0630:
        push   ebp
        mov    ebp, esp
        sub    esp, 174h
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 0ch
        jbe    SHORT loc_5_065C
        mov    eax, 57h
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_5_065C:
        db  0BEh, 014h, 035h, 001h, 000h
        lea    edi, [ebp - 174h]
        mov    ecx, 0eh
        rep movsd
        movsb
        mov    byte ptr [ebp - 13ch], 5ch
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 13bh]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 120h], eax
        lea    eax, [ebp - 174h]
        mov    dword ptr [ebp - 11ch], eax
        mov    dword ptr [ebp - 118h], 80000002h
        push   dword ptr [ebp - 120h]
        push   dword ptr [ebp - 11ch]
        push   dword ptr [ebp - 118h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_5_0C1E
        mov    dword ptr [ebp - 4], 4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 114h], eax
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 110h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 10ch], eax
        mov    dword ptr [ebp - 108h], 0
        db  0C7h, 085h, 0FCh, 0FEh, 0FFh, 0FFh, 050h, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 100h], eax
        push   dword ptr [ebp - 114h]
        push   dword ptr [ebp - 110h]
        push   dword ptr [ebp - 10ch]
        push   dword ptr [ebp - 108h]
        push   dword ptr [ebp - 104h]
        push   dword ptr [ebp - 100h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_5_0C04
        mov    dword ptr [ebp - 4], 4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0fch], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 0f8h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 0f4h], eax
        mov    dword ptr [ebp - 0f0h], 0
        db  0C7h, 085h, 014h, 0FFh, 0FFh, 0FFh, 058h, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0e8h], eax
        push   dword ptr [ebp - 0fch]
        push   dword ptr [ebp - 0f8h]
        push   dword ptr [ebp - 0f4h]
        push   dword ptr [ebp - 0f0h]
        push   dword ptr [ebp - 0ech]
        push   dword ptr [ebp - 0e8h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_5_0C04
        mov    dword ptr [ebp - 4], 105h
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0e4h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0e0h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 0dch], eax
        mov    dword ptr [ebp - 0d8h], 0
        db  0C7h, 085h, 02Ch, 0FFh, 0FFh, 0FFh, 060h, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0d0h], eax
        push   dword ptr [ebp - 0e4h]
        push   dword ptr [ebp - 0e0h]
        push   dword ptr [ebp - 0dch]
        push   dword ptr [ebp - 0d8h]
        push   dword ptr [ebp - 0d4h]
        push   dword ptr [ebp - 0d0h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_5_0C04
        mov    dword ptr [ebp - 4], 31h
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax], 0
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0cch], ecx
        mov    dword ptr [ebp - 0c8h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 0c4h], eax
        mov    dword ptr [ebp - 0c0h], 0
        db  0C7h, 085h, 044h, 0FFh, 0FFh, 0FFh, 068h, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0b8h], eax
        push   dword ptr [ebp - 0cch]
        push   dword ptr [ebp - 0c8h]
        push   dword ptr [ebp - 0c4h]
        push   dword ptr [ebp - 0c0h]
        push   dword ptr [ebp - 0bch]
        push   dword ptr [ebp - 0b8h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, dword ptr [ebp + 14h]
        or     esi, esi
        je     NEAR PTR loc_5_0A81
        mov    dword ptr [ebp - 4], 9
        mov    byte ptr [esi], 0
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0b4h], eax
        mov    dword ptr [ebp - 0b0h], esi
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 0ach], eax
        mov    dword ptr [ebp - 0a8h], 0
        db  0C7h, 085h, 05Ch, 0FFh, 0FFh, 0FFh, 070h, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0a0h], eax
        push   dword ptr [ebp - 0b4h]
        push   dword ptr [ebp - 0b0h]
        push   dword ptr [ebp - 0ach]
        push   dword ptr [ebp - 0a8h]
        push   dword ptr [ebp - 0a4h]
        push   dword ptr [ebp - 0a0h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_5_0A14
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx - 1]
        lea    eax, [ebp - 128h]
        push   eax
        push   esi
        push   6ah
        db  0E8h, 07Ch, 0D1h, 0FAh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 9ch], edi
        lea    eax, [ebp - 128h]
        mov    dword ptr [ebp - 98h], eax
        mov    dword ptr [ebp - 94h], 3
        mov    dword ptr [ebp - 90h], 0
        db  0C7h, 085h, 074h, 0FFh, 0FFh, 0FFh, 078h, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 88h], eax
        push   dword ptr [ebp - 9ch]
        push   dword ptr [ebp - 98h]
        push   dword ptr [ebp - 94h]
        push   dword ptr [ebp - 90h]
        push   dword ptr [ebp - 8ch]
        push   dword ptr [ebp - 88h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_5_0A81
        db  0C7h, 085h, 07Ch, 0FFh, 0FFh, 0FFh, 084h, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 80h], eax
        push   dword ptr [ebp - 84h]
        push   dword ptr [ebp - 80h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Fh, 001h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_5_0A81
        db  08Dh, 049h, 000h
loc_5_0A14:
        mov    dword ptr [ebp - 4], 9
        mov    byte ptr [esi], 0
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 7ch], eax
        lea    eax, [ebp - 128h]
        mov    dword ptr [ebp - 78h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 74h], eax
        mov    dword ptr [ebp - 70h], 0
        db  0C7h, 045h, 094h, 08Ch, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 68h], eax
        push   dword ptr [ebp - 7ch]
        push   dword ptr [ebp - 78h]
        push   dword ptr [ebp - 74h]
        push   dword ptr [ebp - 70h]
        push   dword ptr [ebp - 6ch]
        push   dword ptr [ebp - 68h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_5_0A81
        push   dword ptr [ebp - 4]
        lea    eax, [ebp - 128h]
        push   eax
        push   esi
        push   6ah
        db  0E8h, 09Eh, 0D0h, 0FAh, 0FFh
        add    esp, 10h
loc_5_0A81:
        mov    esi, dword ptr [ebp + 18h]
        or     esi, esi
        je     NEAR PTR loc_5_0BE5
        mov    dword ptr [ebp - 4], 9
        mov    byte ptr [esi], 0
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 64h], eax
        mov    dword ptr [ebp - 60h], esi
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 5ch], eax
        mov    dword ptr [ebp - 58h], 0
        db  0C7h, 045h, 0ACh, 098h, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 50h], eax
        push   dword ptr [ebp - 64h]
        push   dword ptr [ebp - 60h]
        push   dword ptr [ebp - 5ch]
        push   dword ptr [ebp - 58h]
        push   dword ptr [ebp - 54h]
        push   dword ptr [ebp - 50h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_5_0B78
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx - 1]
        lea    eax, [ebp - 128h]
        push   eax
        push   esi
        push   6ah
        db  0E8h, 0EBh, 0CFh, 0FAh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 4ch], edi
        lea    eax, [ebp - 128h]
        mov    dword ptr [ebp - 48h], eax
        mov    dword ptr [ebp - 44h], 3
        mov    dword ptr [ebp - 40h], 0
        db  0C7h, 045h, 0C4h, 0A0h, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 38h], eax
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_5_0BE5
        db  0C7h, 045h, 0CCh, 0ACh, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 34h]
        push   dword ptr [ebp - 30h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Fh, 001h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_5_0BE5
loc_5_0B78:
        mov    dword ptr [ebp - 4], 9
        mov    byte ptr [esi], 0
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 2ch], eax
        lea    eax, [ebp - 128h]
        mov    dword ptr [ebp - 28h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 0B4h, 035h, 001h, 000h
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
        jne    SHORT loc_5_0BE5
        push   dword ptr [ebp - 4]
        lea    eax, [ebp - 128h]
        push   eax
        push   esi
        push   6ah
        db  0E8h, 03Ah, 0CFh, 0FAh, 0FFh
        add    esp, 10h
loc_5_0BE5:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_5_0C04:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        mov    eax, esi
loc_5_0C1E:
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
sub_5_0C24:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
        db  0C7h, 045h, 0F8h, 0FCh, 035h, 001h, 000h
        mov    dword ptr [ebp - 4], 80000002h
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        leave
        ret
sub_5_0C58:
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
        leave
        ret
sub_5_0C94:
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
        leave
        ret
sub_5_0CB4:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_5_0CBC:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_5_0CC4:
        db  0CDh, 020h, 03Eh, 080h, 033h, 025h
_PTEXT ends
        end
