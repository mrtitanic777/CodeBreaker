; vfbackup_icode_03.asm -- VFBACKUP object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        mov    eax, 2200h
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        or     ecx, ecx
        jne    SHORT loc_3_0047
loc_3_000F:
        mov    eax, 2760h
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        or     ecx, ecx
        jne    SHORT loc_3_008F
loc_3_001E:
        mov    eax, 299ch
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        or     ecx, ecx
        jne    NEAR PTR loc_3_00CC
loc_3_0031:
        mov    eax, 1bh
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        jecxz  loc_3_0045
        mov    eax, ecx
        call   sub_3_0079
loc_3_0045:
        clc
        ret
loc_3_0047:
        mov    eax, ecx
        mov    ecx, 2
        db  08Dh, 035h, 0DCh, 005h, 000h, 000h
        lea    edi, [eax + 0ch]
        repe cmpsd
        jne    SHORT loc_3_000F
        db  00Fh, 0B7h, 00Dh, 0D8h, 005h, 000h, 000h
        movzx  ebx, word ptr [eax + 8]
        xchg   bl, bh
        cmp    bx, cx
        ja     SHORT loc_3_000F
        db  0A3h, 0C0h, 006h, 000h, 000h
        call   sub_3_0079
        jmp    SHORT loc_3_000F
sub_3_0079:
        mov    edi, eax
        db  0CDh, 020h, 0F8h, 000h, 001h, 000h
        mov    dword ptr [edi + 14h], 0a0910000h
        db  0CDh, 020h, 0F7h, 000h, 001h, 000h
        ret
loc_3_008F:
        mov    eax, ecx
        mov    ecx, 2
        db  08Dh, 035h, 02Ch, 006h, 000h, 000h
        lea    edi, [eax + 0ch]
        repe cmpsd
        jne    NEAR PTR loc_3_001E
        db  00Fh, 0B7h, 00Dh, 028h, 006h, 000h, 000h
        movzx  ebx, word ptr [eax + 8]
        xchg   bl, bh
        cmp    bx, cx
        ja     NEAR PTR loc_3_001E
        db  0A3h, 0C4h, 006h, 000h, 000h
        call   sub_3_0079
        jmp    NEAR PTR loc_3_001E
loc_3_00CC:
        mov    eax, ecx
        mov    ecx, 2
        db  08Dh, 035h, 07Ch, 006h, 000h, 000h
        lea    edi, [eax + 0ch]
        repe cmpsd
        jne    NEAR PTR loc_3_0031
        db  00Fh, 0B7h, 00Dh, 078h, 006h, 000h, 000h
        movzx  ebx, word ptr [eax + 8]
        xchg   bl, bh
        cmp    bx, cx
        ja     NEAR PTR loc_3_0031
        db  0A3h, 0C8h, 006h, 000h, 000h
        call   sub_3_0079
        jmp    NEAR PTR loc_3_0031
loc_3_0109:
        db  0A1h, 0C0h, 006h, 000h, 000h
        or     eax, eax
        jne    NEAR PTR loc_3_01A2
loc_3_0116:
        db  0A1h, 0C4h, 006h, 000h, 000h
        or     eax, eax
        jne    NEAR PTR loc_3_0227
loc_3_0123:
        db  0A1h, 0C8h, 006h, 000h, 000h
        or     eax, eax
        jne    NEAR PTR loc_3_025A
loc_3_0130:
        call   sub_3_03BC
        jb     SHORT loc_3_0177
        call   sub_3_03EC
        jb     NEAR PTR loc_3_02F5
        db  0F7h, 005h, 0F0h, 003h, 000h, 000h, 010h, 000h, 000h, 000h
        je     NEAR PTR loc_3_028D
        db  00Fh, 0B6h, 005h, 0EDh, 003h, 000h, 000h
        db  0BEh, 084h, 005h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        jb     NEAR PTR loc_3_02F5
        db  0F7h, 005h, 0F0h, 003h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_3_0178
        clc
loc_3_0177:
        ret
loc_3_0178:
        push   0eh
        db  068h, 030h, 004h, 000h, 000h
        push   0fffh
        push   0
        push   0fh
        push   0
        push   1
        push   8
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     edx, edx
        db  089h, 015h, 034h, 004h, 000h, 000h
        ret
loc_3_01A0:
        stc
        ret
loc_3_01A2:
        db  0F6h, 005h, 0F0h, 003h, 000h, 000h, 010h
        je     SHORT loc_3_01A0
        db  080h, 00Dh, 0F0h, 003h, 000h, 000h, 001h
        mov    esi, dword ptr [eax + 20h]
        db  089h, 035h, 03Ch, 004h, 000h, 000h
        push   eax
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        mov    byte ptr [ebp + 1dh], 0
        call   esi
        movzx  ecx, word ptr [ebp + 1ch]
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        pop    eax
        mov    esi, dword ptr [eax + 18h]
        db  089h, 035h, 040h, 004h, 000h, 000h
        db  0BAh, 018h, 003h, 000h, 000h
        cmp    ecx, 324h
        jbe    SHORT loc_3_020B
        db  0BAh, 02Ch, 003h, 000h, 000h
loc_3_020B:
        db  089h, 015h, 040h, 003h, 000h, 000h
        db  08Dh, 035h, 0E8h, 005h, 000h, 000h
        db  068h, 016h, 001h, 000h, 000h
loc_3_021C:
        lea    edi, [eax + 18h]
        mov    ecx, 5
        rep movsd
        ret
loc_3_0227:
        db  0F6h, 005h, 0F0h, 003h, 000h, 000h, 010h
        je     NEAR PTR loc_3_01A0
        db  080h, 00Dh, 0F0h, 003h, 000h, 000h, 002h
        mov    esi, dword ptr [eax + 20h]
        db  089h, 035h, 0C4h, 004h, 000h, 000h
        mov    esi, dword ptr [eax + 18h]
        db  089h, 035h, 0CCh, 004h, 000h, 000h
        db  08Dh, 035h, 038h, 006h, 000h, 000h
        db  068h, 023h, 001h, 000h, 000h
        jmp    SHORT loc_3_021C
loc_3_025A:
        db  0F6h, 005h, 0F0h, 003h, 000h, 000h, 010h
        je     NEAR PTR loc_3_01A0
        db  080h, 00Dh, 0F0h, 003h, 000h, 000h, 004h
        mov    esi, dword ptr [eax + 20h]
        db  089h, 035h, 0A0h, 004h, 000h, 000h
        mov    esi, dword ptr [eax + 18h]
        db  089h, 035h, 0B8h, 004h, 000h, 000h
        db  08Dh, 035h, 088h, 006h, 000h, 000h
        db  068h, 030h, 001h, 000h, 000h
        jmp    SHORT loc_3_021C
loc_3_028D:
        db  0CDh, 020h, 000h, 000h, 01Bh, 000h
        jb     SHORT loc_3_029D
        cmp    ax, 400h
        jb     SHORT loc_3_02F5
        jmp    SHORT loc_3_02D1
loc_3_029D:
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        sub    esp, 8
        sub    esp, ecx
        mov    eax, ecx
        mov    esi, edx
        mov    edi, esp
        rep movsb
        lea    ecx, [eax + 8]
        mov    dword ptr [edi], 2e444656h
        mov    dword ptr [edi + 4], 445856h
        mov    edx, esp
        mov    al, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        lahf
        add    esp, ecx
        rcr    al, 1
        jb     SHORT loc_3_0312
loc_3_02D1:
        db  00Fh, 0B6h, 005h, 0ECh, 003h, 000h, 000h
        db  0E8h, 0FCh, 002h, 0FDh, 0FFh
        jb     SHORT loc_3_02F5
        db  0A3h, 014h, 004h, 000h, 000h
        mov    eax, 13h
        db  0BEh, 0F0h, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        ret
loc_3_02F5:
        stc
        ret
        db  00Fh, 0B7h, 005h, 00Ch, 000h, 000h, 000h, 003h, 005h, 004h, 000h, 000h, 000h, 060h, 08Bh, 0F0h
        db  0B8h, 001h, 000h, 000h, 000h, 0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_3_0312:
        db  00Fh, 0B7h, 005h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        pushad
        mov    esi, eax
        mov    eax, 1
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
VFBACKUP_Register_NEC:
        db  089h, 015h, 014h, 004h, 000h, 000h
        db  089h, 00Dh, 0F8h, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  0A3h, 018h, 004h, 000h, 000h
        db  089h, 03Dh, 0FCh, 003h, 000h, 000h
        db  089h, 035h, 000h, 004h, 000h, 000h
        db  0B8h, 09Ch, 004h, 000h, 000h
        db  080h, 00Dh, 0F0h, 003h, 000h, 000h, 010h
        ret
VFBACKUP_Register_VFD:
        db  0F6h, 005h, 0F0h, 003h, 000h, 000h, 010h
        stc
        jne    SHORT loc_3_036E
        db  080h, 00Dh, 0F0h, 003h, 000h, 000h, 020h
loc_3_036E:
        ret
loc_3_036F:
        xor    ecx, ecx
        db  081h, 03Dh, 040h, 003h, 000h, 000h, 02Ch, 003h, 000h, 000h
        jne    SHORT loc_3_03AA
        mov    eax, 10099h
        db  0BEh, 0B8h, 003h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        mov    eax, 1
        db  0FFh, 015h, 040h, 004h, 000h, 000h
        pushfd
        mov    eax, 10099h
        db  0BEh, 0B8h, 003h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        popfd
loc_3_03AA:
        clc
        ret
        db  000h, 000h, 000h, 000h, 0EBh, 006h, 0FFh, 025h, 0ACh, 003h, 000h, 000h
loc_3_03B8:
        clc
        ret
        db  000h, 000h
sub_3_03BC:
        db  0C7h, 005h, 0D8h, 006h, 000h, 000h, 09Ch, 000h, 000h, 000h
        db  0C7h, 005h, 0DCh, 006h, 000h, 000h, 068h, 000h, 000h, 000h
        db  068h, 0CCh, 006h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        add    esp, 4
        db  066h, 083h, 03Dh, 0F8h, 006h, 000h, 000h, 001h
        jne    SHORT loc_3_03E9
        ret
loc_3_03E9:
        stc
        ret
        db  000h
sub_3_03EC:
        mov    ecx, 6
        db  0BFh, 0DCh, 003h, 000h, 000h
        db  0BEh, 0B9h, 001h, 000h, 000h
loc_3_03FB:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_3_040C
        add    edi, 2
        loop   loc_3_03FB
        clc
loc_3_040C:
        ret
        db  000h, 000h, 000h, 041h, 000h, 001h, 000h
loc_3_0414:
        xchg   esi, eax
        add    byte ptr [ecx], al
        db  000h, 0E9h
        add    al, 0
        db  000h, 0F1h
        add    al, 0
        add    byte ptr [ecx + 2], bh
loc_3_0425:
        db  08Bh, 004h, 08Dh, 00Ch, 004h, 000h, 000h
        db  08Bh, 034h, 08Dh, 014h, 004h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_3_046F
        db  00Fh, 0ABh, 00Dh, 0B4h, 004h, 000h, 000h
        loop   loc_3_0425
        mov    eax, 30001h
        db  0BEh, 0F7h, 002h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        mov    eax, 30011h
        db  0BEh, 041h, 003h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        mov    eax, 1
        db  0FFh, 015h, 0B8h, 004h, 000h, 000h
loc_3_046F:
        pushfd
        mov    ecx, 2
loc_3_0475:
        db  00Fh, 0B3h, 00Dh, 0B4h, 004h, 000h, 000h
        jae    SHORT loc_3_0492
        db  08Bh, 004h, 08Dh, 00Ch, 004h, 000h, 000h
        db  08Bh, 034h, 08Dh, 014h, 004h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
loc_3_0492:
        loop   loc_3_0475
        db  0C7h, 005h, 0B4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h
        push   8
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   2
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     edx, edx
        je     SHORT loc_3_04D3
        db  0C7h, 005h, 0ACh, 004h, 000h, 000h, 001h, 000h, 000h, 000h
        add    edx, 2000h
        db  089h, 015h, 0A4h, 004h, 000h, 000h
        popfd
        ret
loc_3_04D3:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_3_04D9:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_04DD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jmp    SHORT loc_3_04E9
        db  0FFh, 025h, 0D9h, 004h, 000h, 000h
loc_3_04E9:
        jmp    SHORT loc_3_04F1
        db  0FFh, 025h, 0DDh, 004h, 000h, 000h
loc_3_04F1:
        xor    esi, esi
        clc
loc_3_04F4:
        ret
        db  000h, 000h, 000h, 041h, 000h, 001h, 000h, 001h, 000h, 003h, 000h, 081h, 000h, 001h, 000h, 07Fh
        db  000h, 001h, 000h
loc_3_0508:
        add    byte ptr [eax], 1
        db  000h, 0ACh, 005h, 000h, 000h, 0B4h, 005h
        add    byte ptr [eax], al
        db  0BCh, 005h, 000h, 000h, 0C4h
        db  005h, 000h, 000h, 0CCh, 005h
        add    byte ptr [eax], al
loc_3_0520:
        mov    eax, 24h
        db  0BEh, 06Fh, 003h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    ecx, 5
loc_3_0535:
        db  08Bh, 004h, 08Dh, 0F4h, 004h, 000h, 000h
        db  08Bh, 034h, 08Dh, 008h, 005h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_3_055F
        db  00Fh, 0ABh, 00Dh, 0C8h, 004h, 000h, 000h
        loop   loc_3_0535
        mov    eax, 1
        db  0FFh, 015h, 0CCh, 004h, 000h, 000h
loc_3_055F:
        pushfd
        mov    ecx, 5
loc_3_0565:
        db  00Fh, 0B3h, 00Dh, 0C8h, 004h, 000h, 000h
        jae    SHORT loc_3_0582
        db  08Bh, 004h, 08Dh, 0F4h, 004h, 000h, 000h
        db  08Bh, 034h, 08Dh, 008h, 005h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
loc_3_0582:
        loop   loc_3_0565
        db  0C7h, 005h, 0C8h, 004h, 000h, 000h, 000h, 000h, 000h, 000h
        popfd
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0EBh, 006h, 0FFh, 025h, 090h, 005h, 000h, 000h
loc_3_05AC:
        jmp    SHORT loc_3_05B4
        db  0FFh, 025h, 094h, 005h, 000h, 000h
loc_3_05B4:
        jmp    SHORT loc_3_05BC
        db  0FFh, 025h, 098h, 005h, 000h, 000h
loc_3_05BC:
        jmp    SHORT loc_3_05C4
        db  0FFh, 025h, 09Ch, 005h, 000h, 000h
loc_3_05C4:
        jmp    SHORT loc_3_05CC
        db  0FFh, 025h, 0A0h, 005h, 000h, 000h
loc_3_05CC:
        xor    esi, esi
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 022h
loc_3_05D8:
        add    dword ptr [ebx], eax
        add    byte ptr [eax], al
loc_3_05DC:
        push   esi
        inc    esi
        dec    ecx
        dec    esi
        push   esp
        inc    esp
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        db  000h, 080h
loc_3_05E8:
        pop    ebx
        add    byte ptr [eax], al
        db  000h, 005h, 003h, 000h, 000h, 006h
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 65h], dh
        jb     SHORT loc_3_0660
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], dh
        db  076h, 073h
        push   edx
        xor    dh, byte ptr [esi + 73h]
        push   edx
        xor    esi, dword ptr [esi + 73h]
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        daa
loc_3_0628:
        add    dword ptr [ebx], eax
        add    byte ptr [eax], al
loc_3_062C:
        inc    esi
        inc    edx
        push   edi
        pop    edi
        push   esi
        pop    eax
        inc    esp
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        db  080h
loc_3_0638:
        add    eax, dword ptr fs:[eax]
        db  000h, 084h, 006h, 000h, 000h, 0AAh, 006h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  076h, 065h
        jb     SHORT loc_3_06B0
loc_3_0660:
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], dh
        jbe    SHORT loc_3_06DA
        push   edx
        xor    dh, byte ptr [esi + 73h]
        push   edx
        xor    esi, dword ptr [esi + 73h]
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esp + ebx*4], al
        sub    dword ptr [eax], eax
        add    al, 0
        db  000h
loc_3_067C:
        inc    ebx
        dec    ebp
        push   ebx
        push   edi
        push   esp
        inc    ecx
        push   eax
        inc    ebp
        add    byte ptr [eax], al
        db  000h, 080h, 0B4h, 002h, 000h, 000h
        and    al, 6
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  076h, 065h
        jb     SHORT loc_3_0700
loc_3_06B0:
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], dh
        db  076h, 073h
        push   edx
        xor    dh, byte ptr [esi + 73h]
        push   edx
        xor    esi, dword ptr [esi + 73h]
        push   edx
loc_3_06C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_06C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_06C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_06CC:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    al, 0
        add    byte ptr [eax], al
loc_3_06D8:
        add    byte ptr [eax], al
loc_3_06DA:
        add    byte ptr [eax], al
loc_3_06DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   esi
        inc    esi
        inc    edx
        inc    ecx
        inc    ebx
        dec    ebx
        push   ebp
        push   eax
        and    byte ptr [esi + 58h], dl
        inc    esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_06F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0700:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
