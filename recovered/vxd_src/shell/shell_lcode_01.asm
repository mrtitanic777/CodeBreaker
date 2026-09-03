; shell_lcode_01.asm -- SHELL object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        db  0CDh, 020h, 000h, 000h, 00Bh, 000h
        jb     SHORT loc_1_000E
        db  0CDh, 020h, 001h, 000h, 00Bh, 000h
loc_1_000E:
        ret
sub_1_000F:
        enter  20h, 0
        push   esi
        push   edi
        push   ebx
        mov    dword ptr [ebp - 4], 1
        and    byte ptr [ebp - 6], 0
        mov    ebx, dword ptr [ebp + 8]
loc_1_0024:
        nop
        db  0E8h, 0D6h, 0FFh, 0FEh, 0FFh
        jae    SHORT loc_1_0035
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    dword ptr [ebp + 8], ebx
loc_1_0035:
        mov    edi, dword ptr [ebp + 0ch]
        and    edi, 0fh
        cmp    edi, 5
        jbe    SHORT loc_1_0042
        xor    edi, edi
loc_1_0042:
        mov    dword ptr [ebp - 0ch], edi
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_1_0068
        db  08Bh, 015h, 000h, 00Ah, 000h, 000h
        cmp    word ptr [edx + ebx + 36h], 0
        je     SHORT loc_1_0068
        xor    edx, edx
        xor    esi, esi
        lea    eax, [edx + 0fh]
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_1_0068:
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        mov    dword ptr [ebp - 20h], ebx
        db  08Bh, 01Dh, 008h, 00Ah, 000h, 000h
        test   ebx, ebx
        jne    SHORT loc_1_0081
        db  0CDh, 020h, 001h, 000h, 001h, 000h
loc_1_0081:
        db  0FEh, 005h, 0E4h, 008h, 000h, 000h
        jne    SHORT loc_1_00A7
        mov    edx, 0dh
        lea    eax, [edx + 2]
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        mov    eax, 10h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        pop    ebp
loc_1_00A7:
        call   sub_1_01B8
        test   byte ptr [ebp + 0fh], 20h
        je     SHORT loc_1_00B7
        int    1
        cli
loc_1_00B5:
        jmp    SHORT loc_1_00B5
loc_1_00B7:
        sub    esp, 6ch
        mov    edi, esp
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 00Eh, 000h, 00Dh, 000h
loc_1_00CE:
        call   sub_1_016C
        test   byte ptr [ebp - 6], 1
        je     SHORT loc_1_0107
        mov    ah, al
        db  03Ah, 025h, 0EFh, 008h, 000h, 000h
        jne    SHORT loc_1_00E6
        mov    ah, byte ptr [ebp - 8]
loc_1_00E6:
        mov    edi, dword ptr [ebp - 0ch]
        db  08Bh, 034h, 0BDh, 01Ch, 009h, 000h, 000h
        xor    ecx, ecx
loc_1_00F2:
        cmp    ah, byte ptr [esi + ecx*2 + 2]
        je     SHORT loc_1_010C
        inc    ecx
        cmp    cl, byte ptr [esi + 1]
        je     SHORT loc_1_0100
        jmp    SHORT loc_1_00F2
loc_1_0100:
        call   0
        jmp    SHORT loc_1_00CE
loc_1_0107:
        xor    eax, eax
        inc    eax
        jmp    SHORT loc_1_0111
loc_1_010C:
        movzx  eax, byte ptr [esi + ecx*2 + 3]
loc_1_0111:
        mov    dword ptr [ebp - 4], eax
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
        db  0FEh, 00Dh, 0E4h, 008h, 000h, 000h
        jns    SHORT loc_1_0149
        push   ebx
        mov    ebx, dword ptr [ebp - 20h]
        mov    eax, 0fh
        lea    edx, [eax - 2]
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        pop    ebx
        lea    eax, [eax + 2]
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_1_0149:
        db  0C6h, 005h, 0EEh, 008h, 000h, 000h, 001h
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_0158:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  080h, 03Dh, 0EEh, 008h, 000h, 000h, 000h
        je     SHORT sub_1_016C
        call   sub_1_01B8
sub_1_016C:
        db  0CDh, 020h, 00Ch, 000h, 00Dh, 000h
        je     SHORT loc_1_0158
        test   byte ptr [ebp - 6], 1
        je     SHORT loc_1_01AA
        test   ah, 80h
        je     SHORT loc_1_019E
        test   ah, 0ch
        jne    SHORT loc_1_01A3
        db  0BEh, 038h, 009h, 000h, 000h
        mov    ecx, dword ptr [esi]
        lea    esi, [esi + 4]
loc_1_018E:
        cmp    al, byte ptr [esi]
        je     SHORT loc_1_0199
        lea    esi, [esi + 2]
        loop   loc_1_018E
        jmp    SHORT loc_1_01A3
loc_1_0199:
        mov    al, byte ptr [esi + 1]
        jmp    SHORT loc_1_01AA
loc_1_019E:
        test   ah, 0ch
        je     SHORT loc_1_01AA
loc_1_01A3:
        call   0
        jmp    SHORT sub_1_016C
loc_1_01AA:
        ret
sub_1_01AB:
        push   ecx
        xor    al, al
        repne scasb
        pop    eax
        pushfd
        sub    ecx, eax
        not    ecx
        popfd
        ret
sub_1_01B8:
        db  0A1h, 0DCh, 008h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 00Ah, 000h
        mov    dword ptr [ebp - 10h], edx
        mov    dword ptr [ebp - 14h], eax
        and    dword ptr [ebp - 18h], 0
        db  080h, 025h, 0EEh, 008h, 000h, 000h, 000h
        or     byte ptr [ebp - 6], 2
        call   sub_1_0219
        mov    edx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 18h]
        sub    edx, eax
        jae    SHORT loc_1_01E9
        xor    edx, edx
loc_1_01E9:
        shr    edx, 1
        mov    dword ptr [ebp - 18h], edx
        db  0A1h, 0DCh, 008h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 00Ah, 000h
        db  0A1h, 0E0h, 008h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 00Ah, 000h
        and    byte ptr [ebp - 6], 0fdh
        call   sub_1_0219
        mov    eax, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 1ch]
        db  0CDh, 020h, 00Bh, 080h, 00Ah, 000h
sub_1_0219:
        sub    esp, 5ch
        mov    dword ptr [ebp - 1ch], 5
        db  0BEh, 0F0h, 008h, 000h, 000h
        call   sub_1_039C
        mov    edi, dword ptr [ebp + 14h]
        or     edi, edi
        je     SHORT loc_1_0295
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_0295
        mov    ecx, 50h
        call   sub_1_01AB
        shr    ecx, 1
        sub    ecx, 22h
        sub    dword ptr [ebp - 1ch], ecx
        db  0A1h, 0E0h, 008h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 00Ah, 000h
        db  0A1h, 0DCh, 008h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 00Ah, 000h
        mov    al, 20h
        call   sub_1_03B0
        mov    esi, dword ptr [ebp + 14h]
        call   sub_1_039C
        call   sub_1_03B0
        db  0A1h, 0DCh, 008h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 00Ah, 000h
        db  0A1h, 0E0h, 008h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 00Ah, 000h
        db  0BEh, 0F0h, 008h, 000h, 000h
        call   sub_1_039C
loc_1_0295:
        mov    esi, dword ptr [ebp + 10h]
loc_1_0298:
        xor    edx, edx
        lea    edi, [esp + 4]
        lea    ecx, [edi + 46h]
        mov    dword ptr [esp], esi
        add    dword ptr [esp], 46h
        cld
loc_1_02A9:
        lodsb
        or     al, al
        je     SHORT loc_1_02FE
        cmp    al, 0dh
        je     SHORT loc_1_02D7
        cmp    al, 0ah
        je     SHORT loc_1_02D7
        cmp    al, 9
        je     SHORT loc_1_02CC
        cmp    al, 20h
        je     SHORT loc_1_02CC
        stosb
loc_1_02BF:
        inc    edx
        cmp    edx, 46h
        jb     SHORT loc_1_02A9
        mov    edi, ecx
        mov    esi, dword ptr [esp]
        jmp    SHORT loc_1_02DC
loc_1_02CC:
        mov    ecx, edi
        stosb
        mov    dword ptr [esp], esi
        dec    dword ptr [esp]
        jmp    SHORT loc_1_02BF
loc_1_02D7:
        movzx  eax, al
        jmp    SHORT loc_1_02E1
loc_1_02DC:
        mov    eax, 0a0dh
loc_1_02E1:
        stosd
        xchg   esi, eax
        lea    esi, [esp + 4]
        call   sub_1_039C
        xchg   esi, eax
        cld
loc_1_02EE:
        lodsb
        or     al, al
        je     SHORT loc_1_02FE
        cmp    al, 9
        je     SHORT loc_1_02EE
        cmp    al, 20h
        je     SHORT loc_1_02EE
        dec    esi
        jmp    SHORT loc_1_0298
loc_1_02FE:
        stosb
        db  0BEh, 0F2h, 008h, 000h, 000h
        or     edx, edx
        je     SHORT loc_1_0316
        lea    esi, [esp + 4]
        call   sub_1_039C
        db  0BEh, 0F0h, 008h, 000h, 000h
loc_1_0316:
        call   sub_1_039C
        test   byte ptr [ebp + 0fh], 20h
        je     SHORT loc_1_0335
        db  00Fh, 0B7h, 035h, 00Eh, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        call   sub_1_039C
        jmp    SHORT loc_1_0398
loc_1_0335:
        mov    edi, dword ptr [ebp - 0ch]
        db  08Bh, 014h, 0BDh, 01Ch, 009h, 000h, 000h
        movzx  esi, byte ptr [edx]
        db  00Fh, 0B7h, 034h, 075h, 010h, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        call   sub_1_039C
        mov    al, byte ptr [edx + 1]
        or     al, al
        je     SHORT loc_1_0398
        or     byte ptr [ebp - 6], 1
        mov    ecx, dword ptr [ebp + 0dh]
        and    ecx, 0fh
        cmp    cl, al
        jae    SHORT loc_1_036F
        cmp    edi, 1
        jne    SHORT loc_1_0371
loc_1_036F:
        xor    ecx, ecx
loc_1_0371:
        mov    al, byte ptr [edx + ecx*2 + 2]
        mov    byte ptr [ebp - 8], al
        movzx  esi, byte ptr [edx]
        lea    esi, [esi + ecx + 1]
        db  00Fh, 0B7h, 034h, 075h, 010h, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        push   dword ptr [ebp - 1ch]
        call   sub_1_039C
        pop    dword ptr [ebp - 1ch]
loc_1_0398:
        add    esp, 5ch
        ret
sub_1_039C:
        pushfd
        push   esi
        push   eax
loc_1_039F:
        cld
        lodsb
        test   al, al
        je     SHORT loc_1_03AC
        call   sub_1_03B0
        jmp    SHORT loc_1_039F
loc_1_03AC:
        pop    eax
        pop    esi
        popfd
        ret
sub_1_03B0:
        cmp    al, 0ah
        jne    SHORT loc_1_03B9
        inc    dword ptr [ebp - 18h]
        jmp    SHORT loc_1_03BD
loc_1_03B9:
        cmp    al, 0dh
        jne    SHORT loc_1_03C6
loc_1_03BD:
        mov    dword ptr [ebp - 1ch], 5
        jmp    SHORT loc_1_03F9
loc_1_03C6:
        cmp    al, 8
        jne    SHORT loc_1_03D5
        cmp    dword ptr [ebp - 1ch], 5
        je     SHORT loc_1_03F9
        dec    dword ptr [ebp - 1ch]
        jmp    SHORT loc_1_03F9
loc_1_03D5:
        push   esi
        push   ecx
        push   edx
        push   eax
        mov    esi, esp
        mov    ecx, 1
        mov    eax, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 1ch]
        test   byte ptr [ebp - 6], 2
        jne    SHORT loc_1_03F2
        db  0CDh, 020h, 00Ah, 000h, 00Ah, 000h
loc_1_03F2:
        inc    dword ptr [ebp - 1ch]
        pop    eax
        pop    edx
        pop    ecx
        pop    esi
loc_1_03F9:
        ret
sub_1_03FA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 10h]
        db  0BFh, 0E5h, 008h, 000h, 000h
        cld
        lodsd
        stosd
        lodsd
        stosd
        mov    ecx, 8
loc_1_0423:
        cmp    byte ptr [edi - 1], 20h
        jne    SHORT loc_1_042C
        dec    edi
        loop   loc_1_0423
loc_1_042C:
        mov    byte ptr [edi], 0
        push   dword ptr [ebp + 18h]
        db  068h, 0E5h, 008h, 000h, 000h
        push   dword ptr [ebp + 14h]
        push   140003h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        db  00Fh, 0B7h, 03Dh, 00Ch, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        push   edi
        push   eax
        push   34h
        push   ebx
        call   sub_1_000F
        add    esp, 10h
        add    esp, dword ptr [esp]
        cmp    eax, 6
        mov    eax, ebx
        je     SHORT loc_1_0471
        mov    eax, dword ptr [ebp + 0ch]
loc_1_0471:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_047C:
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ah, 17h
        db  00Fh, 084h, 020h, 001h, 0FFh, 0FFh
        cmp    ax, 168eh
        db  00Fh, 084h, 0EAh, 001h, 0FFh, 0FFh
        cmp    ax, 168fh
        db  00Fh, 084h, 0F5h, 002h, 0FFh, 0FFh
        stc
        ret
SHELL_Get_Version:
        mov    eax, 400h
        clc
        ret
SHELL_SYSMODAL_Message:
        or     ah, 10h
        pushad
        mov    ebp, esp
        test   byte ptr [ebp + 1fh], 60h
        jne    SHORT loc_1_04C2
        push   edi
        push   ecx
        push   eax
        push   ebx
        db  0E8h, 026h, 0FEh, 0FEh, 0FFh
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_1_04C7
loc_1_04C2:
        call   sub_1_04F5
loc_1_04C7:
        mov    dword ptr [ebp + 1ch], eax
        popad
        ret
SHELL_Message:
        pushad
        mov    ebp, esp
        db  0E8h, 05Eh, 0FEh, 0FEh, 0FFh
        cmp    eax, -1
        jne    SHORT loc_1_04EF
        call   sub_1_04F5
        cmp    dword ptr [ebp + 4], 0
        je     SHORT loc_1_04EF
        mov    edx, dword ptr [ebp + 14h]
        call   dword ptr [ebp + 4]
        mov    ebp, esp
        or     eax, 0ffffffffh
loc_1_04EF:
        mov    dword ptr [ebp + 1ch], eax
        clc
        popad
        ret
sub_1_04F5:
        mov    edx, ebp
        db  0BEh, 049h, 005h, 000h, 000h
        test   byte ptr [ebp + 1fh], 20h
        je     SHORT loc_1_050A
        mov    word ptr [esi + 2], 3d90h
        jmp    SHORT loc_1_0518
loc_1_050A:
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        db  0CDh, 020h, 009h, 001h, 001h, 000h
        jne    SHORT loc_1_051F
loc_1_0518:
        call   sub_1_0549
        jmp    SHORT loc_1_0545
loc_1_051F:
        or     dword ptr [ebp + 0ch], 0ffffffffh
        xor    ebx, ebx
        xor    eax, eax
        mov    ecx, 800h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        jmp    SHORT loc_1_053F
loc_1_0534:
        push   5
        push   ebp
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_1_053F:
        cmp    dword ptr [ebp + 0ch], -1
        je     SHORT loc_1_0534
loc_1_0545:
        mov    eax, dword ptr [ebp + 0ch]
        ret
sub_1_0549:
        xor    ecx, ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    ebp, edx
        mov    ebx, dword ptr [ebp + 10h]
        xor    esi, esi
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_1_0571
        mov    esi, dword ptr [ebp]
        or     esi, esi
        jne    SHORT loc_1_0571
        db  08Bh, 035h, 000h, 00Ah, 000h, 000h
        lea    esi, [esi + ebx + 3ah]
loc_1_0571:
        push   esi
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 1ch]
        push   ebx
        call   sub_1_000F
        add    esp, 10h
        mov    dword ptr [ebp + 0ch], eax
        push   ebp
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        db  0CDh, 020h, 020h, 080h, 001h, 000h
SHELL_Resolve_Contention:
        pushad
        mov    ebp, esp
        push   esi
        push   ebx
        push   eax
        db  0E8h, 089h, 0FFh, 0FEh, 0FFh
        add    esp, 0ch
        cmp    eax, -1
        jne    SHORT loc_1_0605
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp + 0ch], eax
        db  08Bh, 035h, 000h, 00Ah, 000h, 000h
        mov    ebx, dword ptr [ebp + 1ch]
        lea    edi, [esi + ebx + 3ah]
        mov    ebx, dword ptr [ebp + 10h]
        lea    esi, [esi + ebx + 3ah]
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        push   edi
        push   esi
        push   dword ptr [ebp + 4]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 10h]
        call   sub_1_03FA
        add    esp, 14h
        push   eax
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 037h, 000h, 001h, 000h
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        pop    eax
        mov    dword ptr [ebp + 10h], eax
        push   eax
        db  0E8h, 081h, 0FFh, 0FEh, 0FFh
        pop    ecx
        jmp    SHORT loc_1_0608
loc_1_0605:
        mov    dword ptr [ebp + 10h], eax
loc_1_0608:
        clc
        popad
        ret
        db  090h
SHELL_Update_User_Activity:
        db  0C6h, 005h, 019h, 00Ah, 000h, 000h, 001h
        ret
sub_1_0614:
        cmp    eax, 20h
        jae    SHORT loc_1_0620
        db  0FFh, 024h, 085h, 022h, 006h, 000h, 000h
loc_1_0620:
        clc
        ret
        db  044h, 002h, 000h, 000h, 09Fh, 002h, 000h, 000h, 012h, 000h, 000h, 000h, 029h, 001h, 000h, 000h
        db  020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 020h, 001h, 000h, 000h
        db  020h, 006h, 000h, 000h, 05Ah, 002h, 000h, 000h, 020h, 006h, 000h, 000h, 0C8h, 000h, 000h, 000h
        db  020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 0B1h, 006h, 000h, 000h
        db  020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h
        db  020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h
        db  020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h
        db  020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 020h, 006h, 000h, 000h, 000h, 000h, 000h, 000h
SHELL_Control:
        pushad
        call   sub_1_0614
        popad
        jb     SHORT loc_1_06B0
        call   sub_1_06C4
loc_1_06B0:
        ret
loc_1_06B1:
        or     edx, edx
        je     SHORT loc_1_06BA
        cmp    edx, 0ah
        jne    SHORT loc_1_06C0
loc_1_06BA:
        db  089h, 01Dh, 008h, 00Ah, 000h, 000h
loc_1_06C0:
        clc
        ret
        db  000h, 000h
sub_1_06C4:
        cmp    eax, 0
        db  00Fh, 084h, 067h, 0FDh, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 07Ch, 0FDh, 0FEh, 0FFh
        cmp    eax, 7
        db  00Fh, 084h, 0C4h, 0FCh, 0FEh, 0FFh
        cmp    eax, 0ch
        db  00Fh, 084h, 0E4h, 0FBh, 0FEh, 0FFh
        cmp    eax, 8
        db  00Fh, 084h, 05Ch, 0FDh, 0FEh, 0FFh
        cmp    eax, 9
        db  00Fh, 084h, 0FEh, 0FCh, 0FEh, 0FFh
        cmp    eax, 0fh
        db  00Fh, 084h, 0E6h, 0F9h, 0FEh, 0FFh
        cmp    eax, 15h
        db  00Fh, 084h, 0BDh, 0FBh, 0FEh, 0FFh
        cmp    eax, 16h
        db  00Fh, 084h, 023h, 0FCh, 0FEh, 0FFh
        cmp    eax, 20h
        db  00Fh, 084h, 00Eh, 0F9h, 0FEh, 0FFh
        cmp    eax, 23h
        db  00Fh, 084h, 06Ch, 004h, 0FFh, 0FFh
        clc
        ret
        db  000h, 000h, 000h
sub_1_072C:
        push   ecx
        push   ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        pop    ebx
        pop    ecx
        jb     SHORT loc_1_073F
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        clc
loc_1_073F:
        ret
loc_1_0740:
        push   5
        pop    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    edi, edx
        cmp    word ptr [edi + 4], 0
        je     NEAR PTR loc_1_07D8
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        xor    edx, edx
        and    byte ptr [ebp + 2dh], 0feh
        mov    dword ptr [ebp + 8], edx
        mov    eax, dword ptr [edi]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [edi + 4]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [edi + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [edi + 0ch]
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  00Fh, 0B7h, 00Dh, 00Eh, 00Ch, 000h, 000h
        db  00Fh, 0B7h, 015h, 00Ch, 00Ch, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        movzx  edx, word ptr [ebp + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
loc_1_07D8:
        cmp    dword ptr [edi + 10h], 0
        je     SHORT loc_1_07E9
        push   edi
        push   dword ptr [edi + 14h]
        push   edx
        call   dword ptr [edi + 10h]
        pop    ecx
        pop    ecx
        pop    edi
loc_1_07E9:
        mov    eax, 0fffff000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        mov    eax, edi
        db  08Bh, 035h, 080h, 00Ah, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        db  0CDh, 020h, 020h, 080h, 001h, 000h
        add    byte ptr [eax], al
_SHELL_QueryAppyTimeAvailable:
        push   edi
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 0a0000000h
        mov    eax, 0
        jae    SHORT loc_1_0852
        db  08Bh, 03Dh, 0D8h, 00Bh, 000h, 000h
        test   edi, edi
        je     SHORT loc_1_0852
        inc    eax
        db  0CDh, 020h, 009h, 001h, 001h, 000h
        jne    SHORT loc_1_0852
        mov    ecx, dword ptr [edi + 10h]
        mov    edx, dword ptr [ecx + 26h]
        mov    dx, word ptr [ecx + 24h]
        db  03Bh, 015h, 014h, 00Ch, 000h, 000h
        je     SHORT loc_1_0850
        db  03Bh, 015h, 018h, 00Ch, 000h, 000h
        jne    SHORT loc_1_0852
loc_1_0850:
        or     al, 2
loc_1_0852:
        pop    edi
        ret
_SHELL_CallAtAppyTime:
        push   ebx
        push   esi
        pushfd
        cli
        db  08Bh, 035h, 0E0h, 00Bh, 000h, 000h
        or     esi, esi
        jne    SHORT loc_1_086D
        db  0E8h, 059h, 0FEh, 0FEh, 0FFh
        db  08Bh, 035h, 0E0h, 00Bh, 000h, 000h
loc_1_086D:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_1_08D5
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 0ch], ecx
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        xchg   edx, eax
        and    ecx, 2
        je     SHORT loc_1_08A6
        mov    eax, dword ptr [esp + 1ch]
        db  0BEh, 0F4h, 00Dh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    ecx, esi
loc_1_08A6:
        mov    dword ptr [edx + 8], ecx
        db  083h, 03Dh, 0D8h, 00Bh, 000h, 000h, 000h
        je     SHORT loc_1_08D4
        db  00Fh, 0BAh, 02Dh, 0E8h, 00Bh, 000h, 000h, 000h
        jb     SHORT loc_1_08D4
        xor    eax, eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    ecx, 0ah
        db  0BEh, 03Ah, 000h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_08D4:
        xchg   edx, eax
loc_1_08D5:
        popfd
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h
loc_1_08DC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_08E4:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_08EE:
        add    byte ptr [eax], al
        db  00Dh, 00Ah
loc_1_08F2:
        or     eax, 0ah
        add    byte ptr [eax], al
        db  000h
loc_1_08FA:
        add    dword ptr [edx], eax
        add    byte ptr [ecx], al
        add    byte ptr [edx], al
loc_1_0900:
        add    al, 3
        add    byte ptr [ebx], al
        add    byte ptr [eax + eax], al
        add    eax, 6000308h
        add    byte ptr [edi], al
        add    byte ptr [edx], al
loc_1_0910:
        or     al, 2
        add    byte ptr [esi], al
        add    byte ptr [edi], al
loc_1_0916:
        lar    eax, word ptr [eax]
        add    al, 0
        db  002h
loc_1_091C:
        clc
        or     byte ptr [eax], al
        db  000h, 0FAh
        or     byte ptr [eax], al
        db  000h, 000h
        or     dword ptr [eax], eax
        db  000h, 008h
        or     dword ptr [eax], eax
        db  000h, 010h
        or     dword ptr [eax], eax
        db  000h, 016h
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [esp + ebx], bl
        add    byte ptr [eax], al
        db  09Eh, 004h, 000h, 000h, 094h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 0A5h, 004h, 000h, 000h
        db  0CCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 08Ch, 00Bh, 000h, 000h, 024h, 004h, 000h, 000h
        db  017h, 00Eh, 000h, 000h, 002h, 007h, 000h, 000h, 007h, 000h, 000h, 000h, 07Bh, 000h, 000h, 000h
        db  00Ch, 006h, 000h, 000h, 010h, 008h, 000h, 000h, 054h, 008h, 000h, 000h, 06Ch, 006h, 000h, 000h
        db  08Ch, 000h, 000h, 000h, 0C4h, 00Eh, 000h, 000h, 01Fh, 00Fh, 000h, 000h, 0BAh, 001h, 000h, 000h
        db  06Ah, 002h, 000h, 000h, 097h, 002h, 000h, 000h, 0DDh, 002h, 000h, 000h, 0FDh, 002h, 000h, 000h
        db  063h, 003h, 000h, 000h, 074h, 009h, 000h, 000h, 01Ch, 000h, 000h, 000h, 04Ch, 00Ch, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 017h, 000h, 004h, 000h, 000h, 000h, 053h, 048h, 045h, 04Ch
        db  04Ch, 020h, 020h, 020h, 000h, 000h, 000h, 0B0h, 0A2h, 006h, 000h, 000h, 065h, 000h, 000h, 000h
        db  065h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  040h, 009h, 000h, 000h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0A0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A18:
        add    byte ptr [eax], al
loc_1_0A1A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        push   edi
        push   ebx
        dec    eax
        inc    ebp
        dec    esp
        dec    esp
        and    byte ptr [eax], ah
        add    dword ptr [eax], eax
        db  000h, 0B0h, 0C4h, 006h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jbe    SHORT loc_1_0ABF
        jb     SHORT loc_1_0AAC
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], dh
        jbe    SHORT loc_1_0AD6
        push   edx
        xor    dh, byte ptr [esi + 73h]
        push   edx
        xor    esi, dword ptr [esi + 73h]
        push   edx
loc_1_0A6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A74:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A78:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A80:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_0A8C:
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A94:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    ecx, dword ptr [ecx]
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], 0fh
        add    byte ptr [ecx + 8000h], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0AAC:
        lodsd
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [eax - 6effc6ffh], eax
        add    byte ptr [ecx], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0ABF:
        db  000h, 0ADh, 009h, 000h, 000h, 000h
        add    dword ptr [eax - 6effe3ffh], eax
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   cs
        db  009h
loc_1_0AD6:
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], 1
        add    byte ptr [edx + 80000h], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E0h
        or     byte ptr [eax], al
        add    byte ptr [eax + 1018100h], al
        add    byte ptr [ecx + 20000h], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 03Dh, 009h, 000h, 000h, 000h
        add    dword ptr [ecx - 6effc8ffh], eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 03Dh, 009h, 000h, 000h, 000h
        add    dword ptr [ecx - 6effabffh], eax
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 03Dh, 009h, 000h, 000h, 001h
        add    byte ptr [ecx - 6effc8ffh], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 03Dh, 009h, 000h, 000h, 000h
        add    byte ptr [ecx - 6efec8ffh], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
        mov    al, byte ptr [80000008h]
        add    byte ptr [ecx - 6effd1ffh], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
        mov    al, byte ptr [8]
        add    byte ptr [ecx - 6efffeffh], al
        add    byte ptr [eax], al
loc_1_0B6D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 06Bh, 006h
        db  000h, 000h, 06Bh, 006h, 000h, 000h, 06Bh, 006h, 000h, 000h, 06Ch, 006h, 000h, 000h, 08Ah, 006h
        db  000h, 000h, 092h, 006h, 000h, 000h, 06Bh, 006h, 000h, 000h, 06Bh, 006h, 000h, 000h, 06Bh, 006h
        db  000h, 000h, 0C1h, 006h, 000h, 000h, 08Ah, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_0BB4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BB8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BBC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BC0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BC8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BCC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BD4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BE0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BEC:
        add    byte ptr [eax], al
loc_1_0BEE:
        add    byte ptr [eax], al
loc_1_0BF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C00:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C08:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C0C:
        add    byte ptr [eax], al
loc_1_0C0E:
        add    byte ptr [eax], al
loc_1_0C10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
