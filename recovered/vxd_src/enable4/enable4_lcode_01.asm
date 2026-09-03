; enable4_lcode_01.asm -- ENABLE4 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  0EBh, 006h, 0FFh, 025h, 01Ch, 029h, 000h, 000h
loc_1_0008:
        pushad
        db  0A3h, 024h, 029h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 037h, 000h
        db  0A3h, 020h, 029h, 000h, 000h
        db  081h, 03Dh, 024h, 029h, 000h, 000h, 080h, 000h, 000h, 000h
        jne    SHORT loc_1_0081
        db  0C6h, 005h, 09Fh, 029h, 000h, 000h, 001h
        db  08Bh, 035h, 004h, 029h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        push   0
        db  0FFh, 035h, 000h, 029h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 02Bh, 000h
        add    esp, 8
        push   1
        db  0FFh, 035h, 000h, 029h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 02Bh, 000h
        add    esp, 8
        db  0FFh, 035h, 000h, 029h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 02Bh, 000h
        add    esp, 4
        xor    edx, edx
        mov    eax, 64h
        db  0BEh, 0DBh, 000h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 004h, 029h, 000h, 000h
loc_1_0081:
        popad
        clc
        db  0FFh, 015h, 01Ch, 029h, 000h, 000h
        ret
sub_1_008A:
        cmp    eax, 12ch
        jne    SHORT loc_1_0097
        mov    eax, 0ff11h
        ret
loc_1_0097:
        cmp    eax, 4b0h
        jne    SHORT loc_1_00A4
        mov    eax, 0ff13h
        ret
loc_1_00A4:
        cmp    eax, 960h
        jne    SHORT loc_1_00B1
        mov    eax, 0ff14h
        ret
loc_1_00B1:
        cmp    eax, 12c0h
        jne    SHORT loc_1_00BE
        mov    eax, 0ff15h
        ret
loc_1_00BE:
        cmp    eax, 2580h
        jne    SHORT loc_1_00CB
        mov    eax, 0ff16h
        ret
loc_1_00CB:
        cmp    eax, 4b00h
        jne    SHORT loc_1_00D8
        mov    eax, 0ff18h
        ret
loc_1_00D8:
        xor    eax, eax
        ret
loc_1_00DB:
        db  0C7h, 005h, 004h, 029h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 066h, 029h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
        push   eax
        db  0CDh, 020h, 004h, 000h, 02Bh, 000h
        add    esp, 8
        db  0A3h, 000h, 029h, 000h, 000h
        cmp    eax, -1
        jne    SHORT loc_1_010A
        jmp    NEAR PTR loc_1_01C7
loc_1_010A:
        cmp    eax, -2
        jne    SHORT loc_1_0114
        jmp    NEAR PTR loc_1_01C7
loc_1_0114:
        db  068h, 06Eh, 029h, 000h, 000h
        db  0FFh, 035h, 000h, 029h, 000h, 000h
        db  0CDh, 020h, 006h, 000h, 02Bh, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_012E
        jmp    SHORT loc_1_01AD
loc_1_012E:
        db  0A1h, 05Eh, 029h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        call   sub_1_008A
        db  0A3h, 072h, 029h, 000h, 000h
        db  0C6h, 005h, 084h, 029h, 000h, 000h, 008h
        db  0C6h, 005h, 085h, 029h, 000h, 000h, 000h
        db  0C6h, 005h, 086h, 029h, 000h, 000h, 000h
        push   57h
        db  068h, 06Eh, 029h, 000h, 000h
        db  0FFh, 035h, 000h, 029h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 02Bh, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_0171
        jmp    SHORT loc_1_01AD
loc_1_0171:
        push   100h
        push   0
        push   100h
        push   0
        db  0FFh, 035h, 000h, 029h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 02Bh, 000h
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_0194
        jmp    SHORT loc_1_01AD
loc_1_0194:
        db  0A1h, 000h, 029h, 000h, 000h
        db  0A3h, 0A8h, 029h, 000h, 000h
        db  0C7h, 005h, 06Ch, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        call   sub_1_1516
loc_1_01AD:
        xor    edx, edx
        mov    eax, 64h
        db  0BEh, 038h, 003h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 004h, 029h, 000h, 000h
        clc
        ret
loc_1_01C7:
        stc
        ret
sub_1_01C9:
        db  00Fh, 0B6h, 01Dh, 029h, 029h, 000h, 000h
        db  03Ah, 01Dh, 028h, 029h, 000h, 000h
        je     SHORT loc_1_0243
        cmp    ebx, 18h
        jne    SHORT loc_1_01E2
        mov    ebx, 0
loc_1_01E2:
        db  00Fh, 0BFh, 03Ch, 05Dh, 02Ah, 029h, 000h, 000h
        inc    ebx
        db  088h, 01Dh, 029h, 029h, 000h, 000h
        db  00Fh, 0B6h, 01Dh, 029h, 029h, 000h, 000h
        db  03Ah, 01Dh, 028h, 029h, 000h, 000h
        je     SHORT loc_1_0243
        cmp    ebx, 18h
        jne    SHORT loc_1_020A
        mov    ebx, 0
loc_1_020A:
        db  00Fh, 0BFh, 034h, 05Dh, 02Ah, 029h, 000h, 000h
        inc    ebx
        db  088h, 01Dh, 029h, 029h, 000h, 000h
        db  00Fh, 0B6h, 01Dh, 029h, 029h, 000h, 000h
        db  03Ah, 01Dh, 028h, 029h, 000h, 000h
        je     SHORT loc_1_0243
        cmp    ebx, 18h
        jne    SHORT loc_1_0232
        mov    ebx, 0
loc_1_0232:
        db  066h, 08Bh, 004h, 05Dh, 02Ah, 029h, 000h, 000h
        inc    ebx
        db  088h, 01Dh, 029h, 029h, 000h, 000h
        clc
        ret
loc_1_0243:
        stc
        ret
sub_1_0245:
        call   sub_1_01C9
        jb     SHORT loc_1_0254
        db  0CDh, 020h, 003h, 000h, 00Ch, 000h
        stc
        ret
loc_1_0254:
        clc
        ret
sub_1_0256:
        db  068h, 06Eh, 029h, 000h, 000h
        db  0FFh, 035h, 000h, 029h, 000h, 000h
        db  0CDh, 020h, 006h, 000h, 02Bh, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0270
        jmp    SHORT loc_1_02B0
loc_1_0270:
        mov    eax, dword ptr [esp + 4]
        db  0A3h, 072h, 029h, 000h, 000h
        db  0C6h, 005h, 084h, 029h, 000h, 000h, 008h
        db  0C6h, 005h, 085h, 029h, 000h, 000h, 000h
        db  0C6h, 005h, 086h, 029h, 000h, 000h, 000h
        push   57h
        db  068h, 06Eh, 029h, 000h, 000h
        db  0FFh, 035h, 000h, 029h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 02Bh, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_02AA
        jmp    SHORT loc_1_02B0
loc_1_02AA:
        mov    eax, 0ffffffffh
        ret
loc_1_02B0:
        xor    eax, eax
        ret
sub_1_02B3:
        mov    eax, dword ptr [esp + 4]
        mov    ebx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        db  068h, 014h, 029h, 000h, 000h
        push   ecx
        push   ebx
        push   eax
        db  0CDh, 020h, 013h, 000h, 02Bh, 000h
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_02DA
        db  0A1h, 014h, 029h, 000h, 000h
        ret
loc_1_02DA:
        mov    eax, dword ptr [esp + 4]
        db  068h, 018h, 029h, 000h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 00Bh, 000h, 02Bh, 000h
        add    esp, 0ch
        xor    eax, eax
        ret
sub_1_02F2:
        mov    eax, dword ptr [esp + 4]
        mov    ebx, dword ptr [esp + 8]
        db  068h, 018h, 029h, 000h, 000h
        push   ebx
        push   eax
        db  0CDh, 020h, 00Bh, 000h, 02Bh, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0314
        db  0A1h, 018h, 029h, 000h, 000h
        ret
loc_1_0314:
        xor    eax, eax
        ret
sub_1_0317:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   0
        push   0dh
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 02Bh, 000h
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0332
        xor    eax, eax
        ret
loc_1_0332:
        mov    eax, 0ffffffffh
        ret
loc_1_0338:
        db  0C7h, 005h, 004h, 029h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_1266
        call   sub_1_0245
        db  080h, 03Dh, 09Fh, 029h, 000h, 000h, 000h
        je     SHORT loc_1_0356
        ret
loc_1_0356:
        mov    eax, 32h
        mov    edx, 0
        db  0BEh, 038h, 003h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 004h, 029h, 000h, 000h
        ret
sub_1_0372:
        mov    eax, dword ptr [esp + 4]
        push   esi
        db  0A2h, 09Eh, 029h, 000h, 000h
        db  0BEh, 09Eh, 029h, 000h, 000h
        mov    ecx, 1
        db  0CDh, 020h, 007h, 000h, 00Dh, 000h
        pop    esi
        ret
sub_1_038E:
        mov    eax, dword ptr [esp + 8]
        db  00Fh, 0B6h, 01Dh, 028h, 029h, 000h, 000h
        cmp    ebx, 18h
        jne    SHORT loc_1_03A3
        mov    ebx, 0
loc_1_03A3:
        db  066h, 089h, 004h, 05Dh, 02Ah, 029h, 000h, 000h
        inc    ebx
        db  088h, 01Dh, 028h, 029h, 000h, 000h
        mov    eax, dword ptr [esp + 0ch]
        db  00Fh, 0B6h, 01Dh, 028h, 029h, 000h, 000h
        cmp    ebx, 18h
        jne    SHORT loc_1_03C7
        mov    ebx, 0
loc_1_03C7:
        db  066h, 089h, 004h, 05Dh, 02Ah, 029h, 000h, 000h
        inc    ebx
        db  088h, 01Dh, 028h, 029h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0ffh
        db  00Fh, 0B6h, 01Dh, 028h, 029h, 000h, 000h
        cmp    ebx, 18h
        jne    SHORT loc_1_03F0
        mov    ebx, 0
loc_1_03F0:
        db  066h, 089h, 004h, 05Dh, 02Ah, 029h, 000h, 000h
        inc    ebx
        db  088h, 01Dh, 028h, 029h, 000h, 000h
        ret
sub_1_0400:
        db  0CDh, 020h, 001h, 000h, 00Bh, 000h
        ret
loc_1_0407:
        db  0C6h, 005h, 09Fh, 029h, 000h, 000h, 001h
        db  08Bh, 035h, 004h, 029h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    eax, 370001h
        db  0BEh, 008h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 0e0000000h
        jae    SHORT loc_1_0471
        db  083h, 03Dh, 000h, 029h, 000h, 000h, 000h
        je     SHORT loc_1_0471
        push   0
        db  0FFh, 035h, 000h, 029h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 02Bh, 000h
        add    esp, 8
        push   1
        db  0FFh, 035h, 000h, 029h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 02Bh, 000h
        add    esp, 8
        db  0FFh, 035h, 000h, 029h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 02Bh, 000h
        add    esp, 4
loc_1_0471:
        clc
        ret
loc_1_0473:
        mov    eax, 400h
        clc
        ret
ENABLE4_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 07Dh, 0FBh, 0FEh, 0FFh
        cmp    eax, 1ch
        je     NEAR PTR loc_1_0407
        cmp    eax, 1
        db  00Fh, 084h, 06Bh, 0FBh, 0FEh, 0FFh
        cmp    eax, 5
        je     NEAR PTR loc_1_0407
        clc
        ret
loc_1_04A0:
        ret
loc_1_04A1:
        ret
loc_1_04A2:
        movzx  eax, byte ptr [esp + 4]
        cmp    eax, 4
        jg     SHORT loc_1_04BF
        je     SHORT loc_1_04FB
        cmp    eax, 1
        je     SHORT loc_1_04D7
        cmp    eax, 2
        je     SHORT loc_1_04E3
        cmp    eax, 3
        je     SHORT loc_1_04EF
        jmp    SHORT loc_1_04D0
loc_1_04BF:
        cmp    eax, 5
        je     SHORT loc_1_0507
        cmp    eax, 6
        je     SHORT loc_1_0513
        cmp    eax, 0ffh
        je     SHORT loc_1_051F
loc_1_04D0:
        call   sub_1_0652
        jmp    SHORT loc_1_0537
loc_1_04D7:
        db  0C7h, 005h, 0A4h, 034h, 000h, 000h, 011h, 0FFh, 000h, 000h
        jmp    SHORT loc_1_0537
loc_1_04E3:
        db  0C7h, 005h, 0A4h, 034h, 000h, 000h, 012h, 0FFh, 000h, 000h
        jmp    SHORT loc_1_0537
loc_1_04EF:
        db  0C7h, 005h, 0A4h, 034h, 000h, 000h, 013h, 0FFh, 000h, 000h
        jmp    SHORT loc_1_0537
loc_1_04FB:
        db  0C7h, 005h, 0A4h, 034h, 000h, 000h, 014h, 0FFh, 000h, 000h
        jmp    SHORT loc_1_0537
loc_1_0507:
        db  0C7h, 005h, 0A4h, 034h, 000h, 000h, 015h, 0FFh, 000h, 000h
        jmp    SHORT loc_1_0537
loc_1_0513:
        db  0C7h, 005h, 0A4h, 034h, 000h, 000h, 016h, 0FFh, 000h, 000h
        jmp    SHORT loc_1_0537
loc_1_051F:
        db  083h, 03Dh, 0A4h, 034h, 000h, 000h, 000h
        je     SHORT loc_1_0537
        db  0C7h, 005h, 0B0h, 029h, 000h, 000h, 001h, 000h, 000h, 000h
        call   sub_1_0400
loc_1_0537:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0540:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, eax
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0562
loc_1_054B:
        mov    dl, byte ptr [ecx]
        cmp    dl, 41h
        jl     SHORT loc_1_055C
        cmp    dl, 5ah
        jg     SHORT loc_1_055C
        add    dl, 20h
        mov    byte ptr [ecx], dl
loc_1_055C:
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_054B
loc_1_0562:
        ret
sub_1_0563:
        mov    ecx, dword ptr [esp + 4]
        sub    eax, eax
        cmp    byte ptr [ecx], al
        je     SHORT loc_1_0574
loc_1_056D:
        inc    ecx
        inc    eax
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_056D
loc_1_0574:
        ret
sub_1_0575:
        push   ebx
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, eax
        mov    bl, byte ptr [edx]
        mov    byte ptr [eax], bl
        or     bl, bl
        je     SHORT loc_1_0593
loc_1_0588:
        inc    ecx
        mov    bl, byte ptr [edx + 1]
        inc    edx
        mov    byte ptr [ecx], bl
        or     bl, bl
        jne    SHORT loc_1_0588
loc_1_0593:
        pop    ebx
        ret
sub_1_0595:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_1_059D:
        mov    dl, byte ptr [eax]
        or     dl, dl
        jne    SHORT loc_1_05A8
        cmp    byte ptr [ecx], 0
        je     SHORT loc_1_05B0
loc_1_05A8:
        cmp    byte ptr [ecx], dl
        jne    SHORT loc_1_05B4
        inc    eax
        inc    ecx
        jmp    SHORT loc_1_059D
loc_1_05B0:
        sub    eax, eax
        jmp    SHORT loc_1_05BC
loc_1_05B4:
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [ecx]
        sub    eax, ecx
loc_1_05BC:
        ret
sub_1_05BD:
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, eax
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_05D0
loc_1_05CA:
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_1_05CA
loc_1_05D0:
        mov    edx, dword ptr [esp + 14h]
        or     edx, edx
        je     SHORT loc_1_05E9
        mov    ecx, dword ptr [esp + 10h]
loc_1_05DC:
        mov    bl, byte ptr [ecx]
        or     bl, bl
        je     SHORT loc_1_05E9
        mov    byte ptr [esi], bl
        inc    esi
        inc    ecx
        dec    edx
        jne    SHORT loc_1_05DC
loc_1_05E9:
        mov    byte ptr [esi], 0
        pop    esi
        pop    ebx
        ret
sub_1_05EF:
        push   esi
        sub    esi, esi
        mov    eax, 1
        mov    edx, dword ptr [esp + 8]
loc_1_05FB:
        mov    cl, byte ptr [edx]
        cmp    cl, 20h
        je     SHORT loc_1_0607
        cmp    cl, 9
        jne    SHORT loc_1_060A
loc_1_0607:
        inc    edx
        jmp    SHORT loc_1_05FB
loc_1_060A:
        mov    cl, byte ptr [edx]
        cmp    cl, 2dh
        jne    SHORT loc_1_0618
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_061D
loc_1_0618:
        cmp    cl, 2bh
        jne    SHORT loc_1_061E
loc_1_061D:
        inc    edx
loc_1_061E:
        cmp    byte ptr [edx], 30h
        jl     SHORT loc_1_063A
loc_1_0623:
        mov    cl, byte ptr [edx]
        cmp    cl, 39h
        jg     SHORT loc_1_063A
        movsx  ecx, cl
        imul   esi, esi, 0ah
        inc    edx
        lea    esi, [ecx + esi - 30h]
        cmp    byte ptr [edx], 30h
        jge    SHORT loc_1_0623
loc_1_063A:
        imul   eax, esi
        pop    esi
        ret
        db  066h, 02Bh, 0C0h, 0C3h
sub_1_0643:
        call   sub_1_0400
        call   sub_1_0400
        jmp    NEAR PTR sub_1_1449
sub_1_0652:
        call   sub_1_0400
        jmp    NEAR PTR sub_1_1449
sub_1_065C:
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        db  0A1h, 0E8h, 029h, 000h, 000h
        db  0BEh, 0E8h, 029h, 000h, 000h
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_06A0
        mov    ebx, dword ptr [esp + 10h]
loc_1_0674:
        or     edi, edi
        jne    SHORT loc_1_06A0
        push   dword ptr [esi]
        db  068h, 004h, 036h, 000h, 000h
        call   sub_1_0595
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0695
        mov    edi, 1
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [ebx], al
loc_1_0695:
        mov    eax, dword ptr [esi + 5]
        add    esi, 5
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0674
loc_1_06A0:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_06A6:
        push   ebx
        push   esi
        push   edi
        db  081h, 03Dh, 034h, 036h, 000h, 000h, 078h, 030h, 000h, 000h
        jne    SHORT loc_1_06FB
        db  068h, 004h, 036h, 000h, 000h
        call   sub_1_0563
        add    esp, 4
        cmp    eax, 1
        jne    SHORT loc_1_06FB
        db  00Fh, 0BEh, 005h, 004h, 036h, 000h, 000h
        add    eax, eax
        db  08Ah, 088h, 038h, 02Ch, 000h, 000h
        or     cl, cl
        jne    SHORT loc_1_06DE
        sub    eax, eax
        jmp    SHORT loc_1_073C
loc_1_06DE:
        cmp    cl, 3ah
        je     SHORT loc_1_06E8
        cmp    cl, 2ch
        jne    SHORT loc_1_06EE
loc_1_06E8:
        db  08Ah, 088h, 039h, 02Ch, 000h, 000h
loc_1_06EE:
        mov    ebx, dword ptr [esp + 10h]
        mov    eax, 1
        mov    byte ptr [ebx], cl
        jmp    SHORT loc_1_073C
loc_1_06FB:
        db  08Bh, 035h, 034h, 036h, 000h, 000h
        sub    edi, edi
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_073A
        mov    ebx, dword ptr [esp + 10h]
loc_1_070E:
        or     edi, edi
        jne    SHORT loc_1_073A
        push   dword ptr [esi]
        db  068h, 004h, 036h, 000h, 000h
        call   sub_1_0595
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_072F
        mov    edi, 1
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [ebx], al
loc_1_072F:
        mov    eax, dword ptr [esi + 5]
        add    esi, 5
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_070E
loc_1_073A:
        mov    eax, edi
loc_1_073C:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0740:
        db  08Bh, 00Dh, 030h, 036h, 000h, 000h
        cmp    ecx, 14h
        mov    eax, 0
        jge    SHORT loc_1_0779
        lea    edx, [ecx*4]
        db  0A1h, 034h, 036h, 000h, 000h
        db  089h, 082h, 07Ch, 035h, 000h, 000h
        db  0A1h, 068h, 035h, 000h, 000h
        db  089h, 082h, 0F4h, 034h, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 030h, 036h, 000h, 000h
        mov    eax, 1
loc_1_0779:
        ret
loc_1_077A:
        db  0A1h, 030h, 036h, 000h, 000h
        or     eax, eax
        jle    SHORT loc_1_07AD
        dec    eax
        lea    ecx, [eax*4]
        db  0A3h, 030h, 036h, 000h, 000h
        db  08Bh, 081h, 07Ch, 035h, 000h, 000h
        db  0A3h, 034h, 036h, 000h, 000h
        db  08Bh, 081h, 0F4h, 034h, 000h, 000h
        db  0A3h, 068h, 035h, 000h, 000h
        mov    eax, 1
        jmp    SHORT loc_1_07AF
loc_1_07AD:
        sub    eax, eax
loc_1_07AF:
        ret
sub_1_07B0:
        db  08Bh, 015h, 030h, 036h, 000h, 000h
        or     edx, edx
        mov    eax, 0
        jle    SHORT loc_1_07E9
        lea    ecx, [edx*4 - 4]
        dec    edx
        db  08Bh, 081h, 07Ch, 035h, 000h, 000h
        db  0A3h, 034h, 036h, 000h, 000h
        db  08Bh, 081h, 0F4h, 034h, 000h, 000h
        db  0A3h, 068h, 035h, 000h, 000h
        inc    edx
        db  089h, 015h, 030h, 036h, 000h, 000h
        mov    eax, 1
loc_1_07E9:
        ret
sub_1_07EA:
        db  0A1h, 01Ch, 036h, 000h, 000h
        db  02Bh, 005h, 000h, 036h, 000h, 000h
        cmp    eax, -1
        je     SHORT loc_1_083D
        db  083h, 03Dh, 01Ch, 036h, 000h, 000h, 013h
        jne    SHORT loc_1_080C
        db  083h, 03Dh, 000h, 036h, 000h, 000h, 000h
        je     SHORT loc_1_083D
loc_1_080C:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [eax]
        db  0A1h, 01Ch, 036h, 000h, 000h
        db  0FFh, 005h, 01Ch, 036h, 000h, 000h
        db  088h, 088h, 0CCh, 035h, 000h, 000h
        db  083h, 03Dh, 01Ch, 036h, 000h, 000h, 014h
        jne    SHORT loc_1_0836
        db  0C7h, 005h, 01Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0836:
        mov    eax, 1
        jmp    SHORT loc_1_083F
loc_1_083D:
        sub    eax, eax
loc_1_083F:
        ret
sub_1_0840:
        db  083h, 03Dh, 000h, 036h, 000h, 000h, 014h
        jne    SHORT loc_1_0853
        db  0C7h, 005h, 000h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0853:
        db  0A1h, 01Ch, 036h, 000h, 000h
        db  039h, 005h, 000h, 036h, 000h, 000h
        mov    eax, 0
        je     SHORT loc_1_0881
        db  0FFh, 005h, 000h, 036h, 000h, 000h
        db  0A1h, 000h, 036h, 000h, 000h
        db  08Ah, 088h, 0CBh, 035h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax], cl
        mov    eax, 1
loc_1_0881:
        ret
loc_1_0882:
        ret
loc_1_0883:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 96h
        jg     SHORT loc_1_08AD
        je     NEAR PTR loc_1_0A47
        sub    eax, 2
        cmp    eax, 12h
        ja     NEAR PTR loc_1_0AA2
        shl    eax, 2
        db  0FFh, 0A0h, 0C9h, 00Ah, 000h, 000h
loc_1_08AD:
        cmp    eax, 97h
        je     NEAR PTR loc_1_0A5D
        cmp    eax, 0a0h
        je     NEAR PTR loc_1_0A73
        cmp    eax, 0fdh
        je     NEAR PTR loc_1_0A89
        jmp    NEAR PTR loc_1_0AA2
loc_1_08D3:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 0DAh, 01Dh, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 078h, 030h, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_08EC:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 0DEh, 01Ch, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 078h, 030h, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_0905:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 0B7h, 01Bh, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 078h, 030h, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_091E:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 084h, 01Ah, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 078h, 030h, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_0937:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 07Dh, 019h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 078h, 030h, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_0950:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 0C6h, 01Eh, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 014h, 02Bh, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_0969:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 06Dh, 025h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 00Ch, 02Ch, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_0982:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 078h, 024h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 00Ch, 02Ch, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_099B:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 025h, 027h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 00Ch, 02Ch, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_09B4:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 04Bh, 026h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 00Ch, 02Ch, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_09CD:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 021h, 023h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 0ACh, 034h, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_09E6:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 00Eh, 022h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 0ACh, 034h, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_09FF:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 0C4h, 01Fh, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 0ACh, 034h, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_0A18:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 003h, 021h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 0ACh, 034h, 000h, 000h
        jmp    NEAR PTR loc_1_0ABD
loc_1_0A31:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 072h, 028h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 088h, 02Bh, 000h, 000h
        jmp    SHORT loc_1_0ABD
loc_1_0A47:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 0A1h, 004h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 0A0h, 02Bh, 000h, 000h
        jmp    SHORT loc_1_0ABD
loc_1_0A5D:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 0A2h, 004h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 0D4h, 02Bh, 000h, 000h
        jmp    SHORT loc_1_0ABD
loc_1_0A73:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 0A0h, 004h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 094h, 02Bh, 000h, 000h
        jmp    SHORT loc_1_0ABD
loc_1_0A89:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_0AA2:
        cmp    bl, 0ech
        jb     SHORT loc_1_0AAE
        call   sub_1_0643
        jmp    SHORT loc_1_0AC7
loc_1_0AAE:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
loc_1_0ABD:
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_0AC7:
        pop    ebx
        ret
loc_1_0AC9:
        ror    dword ptr [eax], cl
        add    byte ptr [eax], al
        in     al, dx
        or     byte ptr [eax], al
        db  000h, 005h, 009h, 000h, 000h, 01Eh
        or     dword ptr [eax], eax
        db  000h, 037h
        or     dword ptr [eax], eax
        db  000h, 0A2h, 00Ah, 000h, 000h, 0A2h
        or     al, byte ptr [eax]
        db  000h, 0A2h, 00Ah, 000h, 000h, 050h
        or     dword ptr [eax], eax
        db  000h, 069h, 009h
        add    byte ptr [eax], al
        or     byte ptr [ecx], 0
        db  000h, 09Bh, 009h, 000h, 000h, 0B4h
        or     dword ptr [eax], eax
        db  000h, 0CDh
        or     dword ptr [eax], eax
        db  000h, 0E6h
        or     dword ptr [eax], eax
        db  000h, 0FFh
        or     dword ptr [eax], eax
        db  000h, 018h
        or     al, byte ptr [eax]
        db  000h, 0A2h, 00Ah, 000h, 000h, 031h
        or     al, byte ptr [eax]
        add    bh, bh
        db  074h, 024h
        add    al, 0ffh
        db  015h, 068h, 035h, 000h, 000h
        add    esp, 4
        db  0A1h, 0E4h, 035h, 000h, 000h
        dec    eax
        db  0A3h, 0E4h, 035h, 000h, 000h
        jne    SHORT loc_1_0B43
        db  0C7h, 005h, 074h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 025h, 00Ch, 000h, 000h
loc_1_0B43:
        ret
loc_1_0B44:
        db  0A1h, 0E4h, 035h, 000h, 000h
        mov    ecx, eax
        dec    eax
        db  0A3h, 0E4h, 035h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_0B64
        push   dword ptr [esp + 4]
        db  0FFh, 015h, 068h, 035h, 000h, 000h
        add    esp, 4
        jmp    SHORT loc_1_0B86
loc_1_0B64:
        db  0C7h, 005h, 074h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    byte ptr [esp + 4], 0ffh
        jne    SHORT loc_1_0B81
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 025h, 00Ch, 000h, 000h
        jmp    SHORT loc_1_0B86
loc_1_0B81:
        call   sub_1_0643
loc_1_0B86:
        ret
loc_1_0B87:
        movzx  eax, byte ptr [esp + 4]
        db  0A3h, 0E4h, 035h, 000h, 000h
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 044h, 00Bh, 000h, 000h
        db  0C7h, 005h, 074h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        ret
loc_1_0BA6:
        cmp    byte ptr [esp + 4], 0ffh
        jne    SHORT loc_1_0BB4
        call   sub_1_1449
        jmp    SHORT loc_1_0BB9
loc_1_0BB4:
        call   sub_1_0643
loc_1_0BB9:
        ret
loc_1_0BBA:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        cmp    bl, 0ffh
        jne    SHORT loc_1_0C1E
        db  0B8h, 07Ah, 007h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_0BCF
        jmp    SHORT loc_1_0C1E
loc_1_0BCF:
        call   sub_1_07B0
        or     eax, eax
        je     SHORT loc_1_0BEE
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 025h, 00Ch, 000h, 000h
        jmp    SHORT loc_1_0C16
loc_1_0BEE:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 083h, 008h, 000h, 000h
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 01Ah, 00Eh, 000h, 000h
        db  0C7h, 005h, 054h, 036h, 000h, 000h, 0CEh, 011h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0C16:
        db  088h, 01Dh, 060h, 035h, 000h, 000h
        jmp    SHORT loc_1_0C23
loc_1_0C1E:
        call   sub_1_0643
loc_1_0C23:
        pop    ebx
        ret
loc_1_0C25:
        push   ebx
        db  083h, 03Dh, 020h, 036h, 000h, 000h, 000h
        je     SHORT loc_1_0C4F
        cmp    byte ptr [esp + 8], 0fbh
        jne    SHORT loc_1_0C45
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 0A6h, 00Bh, 000h, 000h
        jmp    NEAR PTR loc_1_0DE4
loc_1_0C45:
        call   sub_1_0643
        jmp    NEAR PTR loc_1_0DE4
loc_1_0C4F:
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        sub    eax, 0f3h
        cmp    eax, 0ch
        ja     SHORT loc_1_0C67
        db  0FFh, 024h, 085h, 0E6h, 00Dh, 000h, 000h
loc_1_0C67:
        push   ebx
        db  0FFh, 015h, 068h, 035h, 000h, 000h
        add    esp, 4
        jmp    NEAR PTR loc_1_0DDE
loc_1_0C76:
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 015h, 00Bh, 000h, 000h
        db  0C7h, 005h, 0E4h, 035h, 000h, 000h, 008h, 000h, 000h, 000h
        db  0C7h, 005h, 074h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        push   ebx
        db  0FFh, 015h, 068h, 035h, 000h, 000h
        add    esp, 4
        jmp    NEAR PTR loc_1_0DDE
loc_1_0CA3:
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 015h, 00Bh, 000h, 000h
        db  0C7h, 005h, 0E4h, 035h, 000h, 000h, 004h, 000h, 000h, 000h
        db  0C7h, 005h, 074h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        push   ebx
        db  0FFh, 015h, 068h, 035h, 000h, 000h
        add    esp, 4
        jmp    NEAR PTR loc_1_0DDE
loc_1_0CD0:
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 015h, 00Bh, 000h, 000h
        db  0C7h, 005h, 0E4h, 035h, 000h, 000h, 002h, 000h, 000h, 000h
        db  0C7h, 005h, 074h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        push   ebx
        db  0FFh, 015h, 068h, 035h, 000h, 000h
        add    esp, 4
        jmp    NEAR PTR loc_1_0DDE
loc_1_0CFD:
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 015h, 00Bh, 000h, 000h
        db  0C7h, 005h, 0E4h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 074h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        push   ebx
        db  0FFh, 015h, 068h, 035h, 000h, 000h
        add    esp, 4
        jmp    NEAR PTR loc_1_0DDE
loc_1_0D2A:
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 087h, 00Bh, 000h, 000h
        push   ebx
        db  0FFh, 015h, 068h, 035h, 000h, 000h
        add    esp, 4
        jmp    NEAR PTR loc_1_0DDE
loc_1_0D43:
        db  080h, 03Dh, 060h, 035h, 000h, 000h, 0FFh
        jne    SHORT loc_1_0D62
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 0BAh, 00Bh, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0DDE
loc_1_0D62:
        call   sub_1_0643
        jmp    SHORT loc_1_0DE4
loc_1_0D69:
        db  083h, 03Dh, 078h, 035h, 000h, 000h, 000h
        je     SHORT loc_1_0D84
        call   sub_1_0740
        or     eax, eax
        je     SHORT loc_1_0D7D
        jmp    SHORT loc_1_0DDE
loc_1_0D7D:
        call   sub_1_0643
        jmp    SHORT loc_1_0DE4
loc_1_0D84:
        call   sub_1_0643
        jmp    SHORT loc_1_0DE4
loc_1_0D8B:
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 0A6h, 00Bh, 000h, 000h
        jmp    SHORT loc_1_0DDE
loc_1_0D97:
        push   ebx
        db  0FFh, 015h, 068h, 035h, 000h, 000h
        add    esp, 4
        call   sub_1_07B0
        or     eax, eax
        jne    SHORT loc_1_0DD4
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 083h, 008h, 000h, 000h
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 01Ah, 00Eh, 000h, 000h
        db  0C7h, 005h, 054h, 036h, 000h, 000h, 0CEh, 011h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0DDE
loc_1_0DD4:
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_0DDE:
        db  088h, 01Dh, 060h, 035h, 000h, 000h
loc_1_0DE4:
        pop    ebx
        ret
loc_1_0DE6:
        db  076h, 00Ch
        add    byte ptr [eax], al
        db  0A3h, 00Ch, 000h, 000h, 0D0h
        or     al, 0
        db  000h, 0FDh
        or     al, 0
        db  000h, 02Ah
        db  00Dh, 000h, 000h, 067h, 00Ch
        add    byte ptr [eax], al
        inc    ebx
        db  00Dh, 000h, 000h, 069h, 00Dh
        add    byte ptr [eax], al
        db  08Bh, 00Dh, 000h, 000h, 067h, 00Ch
        add    byte ptr [eax], al
        or     al, 0
        db  000h, 067h, 00Ch
        add    byte ptr [eax], al
        xchg   edi, eax
        db  00Dh, 000h, 000h
loc_1_0E1A:
        push   ebx
        push   esi
        mov    cl, byte ptr [esp + 0ch]
        cmp    cl, 1bh
        jne    SHORT loc_1_0E34
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 025h, 00Ch, 000h, 000h
        jmp    NEAR PTR loc_1_0EF2
loc_1_0E34:
        db  083h, 03Dh, 020h, 036h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_0EED
        cmp    cl, 7fh
        ja     NEAR PTR loc_1_0EED
        movzx  eax, cl
        lea    esi, [eax*2]
        db  08Ah, 09Eh, 038h, 02Ch, 000h, 000h
        or     bl, bl
        jne    SHORT loc_1_0E72
        call   sub_1_0652
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_0EF2
loc_1_0E72:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0E9D
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0E9D
        db  0A1h, 0E8h, 035h, 000h, 000h
        db  088h, 098h, 0ECh, 035h, 000h, 000h
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
loc_1_0E9D:
        db  08Ah, 09Eh, 039h, 02Ch, 000h, 000h
        or     bl, bl
        je     SHORT loc_1_0ED2
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0ED2
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0ED2
        db  0A1h, 0E8h, 035h, 000h, 000h
        db  088h, 098h, 0ECh, 035h, 000h, 000h
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
loc_1_0ED2:
        call   sub_1_1715
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0EF2
loc_1_0EED:
        call   sub_1_0643
loc_1_0EF2:
        pop    esi
        pop    ebx
        ret
sub_1_0EF5:
        db  0C7h, 005h, 0D0h, 034h, 000h, 000h, 004h, 036h, 000h, 000h
        db  068h, 004h, 036h, 000h, 000h
        call   sub_1_0563
        add    esp, 4
        cmp    eax, 14h
        jle    SHORT loc_1_0F1E
        db  0A1h, 0D0h, 034h, 000h, 000h
        mov    byte ptr [eax], 0fdh
        jmp    NEAR PTR loc_1_0FE8
loc_1_0F1E:
        db  0FFh, 035h, 0D0h, 034h, 000h, 000h
        call   sub_1_065C
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_1_0FE8
        db  068h, 004h, 036h, 000h, 000h
        call   sub_1_0540
        add    esp, 4
        db  0FFh, 035h, 0D0h, 034h, 000h, 000h
        call   sub_1_06A6
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_1_0FE8
        db  00Fh, 0BEh, 005h, 004h, 036h, 000h, 000h
        cmp    eax, 2bh
        je     SHORT loc_1_0F9F
        cmp    eax, 2dh
        je     SHORT loc_1_0F9F
        cmp    eax, 30h
        je     SHORT loc_1_0F9F
        db  068h, 004h, 036h, 000h, 000h
        call   sub_1_05EF
        add    esp, 4
        db  0A3h, 0D4h, 034h, 000h, 000h
        cmp    eax, 0ffh
        jbe    SHORT loc_1_0F90
        db  0A1h, 0D0h, 034h, 000h, 000h
        mov    byte ptr [eax], 0fdh
        jmp    SHORT loc_1_0FE8
loc_1_0F90:
        db  0A1h, 0D0h, 034h, 000h, 000h
        db  08Ah, 00Dh, 0D4h, 034h, 000h, 000h
        mov    byte ptr [eax], cl
        jmp    SHORT loc_1_0FE8
loc_1_0F9F:
        db  068h, 004h, 036h, 000h, 000h
        call   sub_1_05EF
        db  0A3h, 0D4h, 034h, 000h, 000h
        db  0A1h, 0D0h, 034h, 000h, 000h
        add    esp, 4
        mov    byte ptr [eax], 0f5h
        db  0FFh, 035h, 0D0h, 034h, 000h, 000h
        call   sub_1_07EA
        add    esp, 4
        db  0C7h, 005h, 0D0h, 034h, 000h, 000h, 0D4h, 034h, 000h, 000h
        db  0C7h, 005h, 0D0h, 034h, 000h, 000h, 0D5h, 034h, 000h, 000h
        db  068h, 0D4h, 034h, 000h, 000h
        call   sub_1_07EA
        add    esp, 4
loc_1_0FE8:
        db  0FFh, 035h, 0D0h, 034h, 000h, 000h
        call   sub_1_07EA
        add    esp, 4
        ret
sub_1_0FF7:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        db  0C7h, 005h, 0DCh, 034h, 000h, 000h, 0D8h, 034h, 000h, 000h
        movzx  eax, bl
        cmp    eax, 20h
        jg     SHORT loc_1_1025
        je     SHORT loc_1_1078
        cmp    eax, 9
        jl     SHORT loc_1_102F
        cmp    eax, 0dh
        jle    SHORT loc_1_1078
        cmp    eax, 1bh
        je     NEAR PTR loc_1_10A5
        jmp    SHORT loc_1_102F
loc_1_1025:
        cmp    eax, 2ch
        je     SHORT loc_1_108D
        cmp    eax, 2eh
        je     SHORT loc_1_108D
loc_1_102F:
        cmp    bl, 20h
        jb     SHORT loc_1_106E
        cmp    bl, 7eh
        ja     SHORT loc_1_106E
        db  068h, 004h, 036h, 000h, 000h
        call   sub_1_0563
        add    esp, 4
        cmp    eax, 15h
        jge    NEAR PTR loc_1_1117
        db  088h, 01Dh, 0E0h, 034h, 000h, 000h
        push   1
        db  068h, 0E0h, 034h, 000h, 000h
        db  068h, 004h, 036h, 000h, 000h
        call   sub_1_05BD
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1117
loc_1_106E:
        call   sub_1_0643
        jmp    NEAR PTR loc_1_1117
loc_1_1078:
        db  068h, 004h, 036h, 000h, 000h
        call   sub_1_0563
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_1117
loc_1_108D:
        db  068h, 004h, 036h, 000h, 000h
        call   sub_1_0563
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_10AE
        call   sub_1_0EF5
        jmp    SHORT loc_1_10C3
loc_1_10A5:
        db  0C6h, 005h, 004h, 036h, 000h, 000h, 000h
        jmp    SHORT loc_1_1117
loc_1_10AE:
        db  0C6h, 005h, 0D8h, 034h, 000h, 000h, 0FCh
        db  0FFh, 035h, 0DCh, 034h, 000h, 000h
        call   sub_1_07EA
        add    esp, 4
loc_1_10C3:
        cmp    bl, 2eh
        jne    SHORT loc_1_10DD
        db  0C6h, 005h, 0D8h, 034h, 000h, 000h, 0FFh
        db  0FFh, 035h, 0DCh, 034h, 000h, 000h
        call   sub_1_07EA
        add    esp, 4
loc_1_10DD:
        db  0C6h, 005h, 004h, 036h, 000h, 000h, 000h
        db  0FFh, 035h, 0DCh, 034h, 000h, 000h
        call   sub_1_0840
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1117
loc_1_10F6:
        db  0A0h, 0D8h, 034h, 000h, 000h
        push   eax
        db  0FFh, 015h, 044h, 036h, 000h, 000h
        add    esp, 4
        db  0FFh, 035h, 0DCh, 034h, 000h, 000h
        call   sub_1_0840
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_10F6
loc_1_1117:
        pop    ebx
        ret
loc_1_1119:
        push   dword ptr [esp + 4]
        db  0FFh, 015h, 044h, 036h, 000h, 000h
        add    esp, 4
        ret
loc_1_1127:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 1bh
        je     NEAR PTR loc_1_11CC
        cmp    eax, 2ch
        je     SHORT loc_1_11B8
        cmp    bl, 20h
        jb     SHORT loc_1_11A2
        cmp    bl, 7eh
        ja     SHORT loc_1_11A2
        db  0C7h, 005h, 054h, 036h, 000h, 000h, 0F7h, 00Fh, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 098h, 02Ah, 000h, 000h
        db  068h, 0B8h, 029h, 000h, 000h
        db  068h, 0E4h, 034h, 000h, 000h
        call   sub_1_0575
        add    esp, 8
        db  0A3h, 0ECh, 034h, 000h, 000h
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_1197
loc_1_1177:
        db  0A1h, 0ECh, 034h, 000h, 000h
        mov    al, byte ptr [eax]
        push   eax
        call   sub_1_0FF7
        db  0FFh, 005h, 0ECh, 034h, 000h, 000h
        db  0A1h, 0ECh, 034h, 000h, 000h
        add    esp, 4
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_1177
loc_1_1197:
        push   ebx
        call   sub_1_0FF7
        add    esp, 4
        jmp    SHORT loc_1_11CC
loc_1_11A2:
        db  0C7h, 005h, 054h, 036h, 000h, 000h, 019h, 011h, 000h, 000h
        push   ebx
        db  0FFh, 015h, 044h, 036h, 000h, 000h
        add    esp, 4
        jmp    SHORT loc_1_11CC
loc_1_11B8:
        db  0C7h, 005h, 054h, 036h, 000h, 000h, 0F7h, 00Fh, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 098h, 02Ah, 000h, 000h
loc_1_11CC:
        pop    ebx
        ret
loc_1_11CE:
        mov    cl, byte ptr [esp + 4]
        cmp    cl, 1bh
        jne    SHORT loc_1_11F5
        db  0C7h, 005h, 054h, 036h, 000h, 000h, 027h, 011h, 000h, 000h
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 083h, 008h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_11F5:
        push   ecx
        db  0FFh, 015h, 044h, 036h, 000h, 000h
        add    esp, 4
        ret
sub_1_1200:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        db  088h, 01Dh, 0F0h, 034h, 000h, 000h
        db  0C6h, 005h, 0F1h, 034h, 000h, 000h, 000h
        db  083h, 03Dh, 074h, 035h, 000h, 000h, 000h
        jne    SHORT loc_1_1255
        or     bl, bl
        jne    SHORT loc_1_1255
        call   sub_1_0400
        db  0A1h, 040h, 036h, 000h, 000h
        inc    eax
        db  0A3h, 040h, 036h, 000h, 000h
        cmp    eax, 3
        jl     SHORT loc_1_1255
        call   sub_1_1449
        push   0ff11h
        call   sub_1_0256
        add    esp, 4
        call   sub_1_0400
        call   sub_1_0400
        call   sub_1_0400
loc_1_1255:
        push   ebx
        db  0FFh, 015h, 054h, 036h, 000h, 000h
        add    esp, 4
        mov    eax, 1
        pop    ebx
        ret
sub_1_1266:
        push   ebx
        db  083h, 03Dh, 06Ch, 035h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1335
        push   1
        db  068h, 044h, 035h, 000h, 000h
        db  0FFh, 035h, 0A8h, 029h, 000h, 000h
        call   sub_1_02B3
        add    esp, 0ch
        or     eax, eax
        jle    SHORT loc_1_12A1
        db  0A0h, 044h, 035h, 000h, 000h
        push   eax
        call   sub_1_1200
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1306
        jmp    SHORT loc_1_1301
loc_1_12A1:
        db  068h, 024h, 036h, 000h, 000h
        db  0FFh, 035h, 0A8h, 029h, 000h, 000h
        call   sub_1_02F2
        add    esp, 8
        mov    ebx, eax
        test   bl, 10h
        je     SHORT loc_1_12C9
        db  0FFh, 035h, 0A8h, 029h, 000h, 000h
        call   sub_1_0317
        add    esp, 4
loc_1_12C9:
        test   bl, 8
        je     SHORT loc_1_1306
        call   sub_1_0400
        db  0A1h, 0ACh, 029h, 000h, 000h
        inc    eax
        db  0A3h, 0ACh, 029h, 000h, 000h
        cmp    eax, 3
        jl     SHORT loc_1_1306
        db  0C7h, 005h, 0ACh, 029h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0A4h, 034h, 000h, 000h, 011h, 0FFh, 000h, 000h
        db  0C7h, 005h, 0B0h, 029h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_1301:
        call   sub_1_1449
loc_1_1306:
        db  083h, 03Dh, 0B0h, 029h, 000h, 000h, 000h
        je     SHORT loc_1_1335
        db  0C7h, 005h, 0B0h, 029h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 0A4h, 034h, 000h, 000h
        call   sub_1_0256
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1335
        db  0C7h, 005h, 0B4h, 029h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_1335:
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 058h, 036h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0C7h, 005h, 050h, 036h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 0B4h
        db  034h, 000h, 000h, 002h, 000h, 000h, 000h, 0C7h, 005h, 0B8h, 034h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0C7h, 005h, 0BCh, 034h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 0C0h, 034h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 0C8h, 034h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h
        db  005h, 0CCh, 034h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 078h, 035h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0C7h, 005h, 020h, 036h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 070h
        db  035h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 064h, 035h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0C7h, 005h, 074h, 035h, 000h, 000h, 000h, 000h, 000h, 000h, 0C6h, 005h, 060h, 035h, 000h
        db  000h, 000h, 0C7h, 005h, 0E4h, 035h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 040h, 036h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 035h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 064h, 036h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0C6h, 005h, 004h, 036h, 000h, 000h, 000h, 0C7h, 005h, 030h, 036h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 000h, 036h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h
        db  005h, 01Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 054h, 036h, 000h, 000h, 0CEh
        db  011h, 000h, 000h, 0C7h, 005h, 044h, 036h, 000h, 000h, 01Ah, 00Eh, 000h, 000h, 0C7h, 005h, 068h
        db  035h, 000h, 000h, 083h, 008h, 000h, 000h, 0C7h, 005h, 034h, 036h, 000h, 000h, 0ACh, 034h, 000h
        db  000h, 0C3h
sub_1_1449:
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 020h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 070h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 064h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 074h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 060h, 035h, 000h, 000h, 000h
        db  0C7h, 005h, 0E4h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 040h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 004h, 036h, 000h, 000h, 000h
        db  0C7h, 005h, 030h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 054h, 036h, 000h, 000h, 0CEh, 011h, 000h, 000h
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 01Ah, 00Eh, 000h, 000h
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 083h, 008h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 0ACh, 034h, 000h, 000h
        db  0C7h, 005h, 0B4h, 034h, 000h, 000h, 002h, 000h, 000h, 000h
        db  0C7h, 005h, 0B8h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0BCh, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_1_1516:
        db  0C7h, 005h, 058h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 050h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0B4h, 034h, 000h, 000h, 002h, 000h, 000h, 000h
        db  0C7h, 005h, 0B8h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0BCh, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0C8h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0CCh, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 020h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 070h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 064h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 074h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 060h, 035h, 000h, 000h, 000h
        db  0C7h, 005h, 0E4h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 040h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 064h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 004h, 036h, 000h, 000h, 000h
        db  0C7h, 005h, 030h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 054h, 036h, 000h, 000h, 0CEh, 011h, 000h, 000h
        db  0C7h, 005h, 044h, 036h, 000h, 000h, 01Ah, 00Eh, 000h, 000h
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 083h, 008h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 0ACh, 034h, 000h, 000h
        ret
        db  000h
sub_1_1620:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 10h]
        or     edx, edx
        je     SHORT loc_1_1642
        mov    ecx, dword ptr [esp + 0ch]
loc_1_1631:
        movsx  esi, byte ptr [eax]
        cmp    esi, ecx
        je     SHORT loc_1_163E
        dec    edx
        inc    eax
        or     edx, edx
        jne    SHORT loc_1_1631
loc_1_163E:
        or     edx, edx
        jne    SHORT loc_1_1644
loc_1_1642:
        sub    eax, eax
loc_1_1644:
        pop    esi
        ret
sub_1_1646:
        mov    al, byte ptr [esp + 4]
        or     al, al
        je     SHORT loc_1_1665
        movzx  eax, al
        db  08Bh, 004h, 085h, 0A4h, 032h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_1665
        push   eax
        call   sub_1_0372
        add    esp, 4
loc_1_1665:
        ret
sub_1_1666:
        mov    al, byte ptr [esp + 4]
        or     al, al
        je     SHORT loc_1_168A
        movzx  eax, al
        db  08Bh, 004h, 085h, 0A4h, 032h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_168A
        or     eax, 80h
        push   eax
        call   sub_1_0372
        add    esp, 4
loc_1_168A:
        ret
sub_1_168B:
        push   esi
        sub    esi, esi
        db  039h, 035h, 048h, 035h, 000h, 000h
        jle    SHORT loc_1_16B0
loc_1_1696:
        mov    eax, esi
        inc    esi
        db  08Ah, 080h, 04Ch, 035h, 000h, 000h
        push   eax
        call   sub_1_1646
        add    esp, 4
        db  03Bh, 035h, 048h, 035h, 000h, 000h
        jl     SHORT loc_1_1696
loc_1_16B0:
        sub    esi, esi
        db  039h, 035h, 0E8h, 035h, 000h, 000h
        jle    SHORT loc_1_16E1
loc_1_16BA:
        db  08Ah, 086h, 0ECh, 035h, 000h, 000h
        push   eax
        call   sub_1_1646
        add    esp, 4
        db  08Ah, 086h, 0ECh, 035h, 000h, 000h
        push   eax
        call   sub_1_1666
        add    esp, 4
        inc    esi
        db  03Bh, 035h, 0E8h, 035h, 000h, 000h
        jl     SHORT loc_1_16BA
loc_1_16E1:
        db  08Bh, 035h, 048h, 035h, 000h, 000h
        or     esi, esi
        jle    SHORT loc_1_16FF
loc_1_16EB:
        db  08Ah, 086h, 04Bh, 035h, 000h, 000h
        dec    esi
        push   eax
        call   sub_1_1666
        add    esp, 4
        or     esi, esi
        jg     SHORT loc_1_16EB
loc_1_16FF:
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
        ret
sub_1_1715:
        push   esi
        sub    esi, esi
        db  039h, 035h, 048h, 035h, 000h, 000h
        jle    SHORT loc_1_173A
loc_1_1720:
        mov    eax, esi
        inc    esi
        db  08Ah, 080h, 04Ch, 035h, 000h, 000h
        push   eax
        call   sub_1_1646
        add    esp, 4
        db  039h, 035h, 048h, 035h, 000h, 000h
        jg     SHORT loc_1_1720
loc_1_173A:
        sub    esi, esi
        db  039h, 035h, 0E8h, 035h, 000h, 000h
        jle    SHORT loc_1_175E
loc_1_1744:
        mov    eax, esi
        inc    esi
        db  08Ah, 080h, 0ECh, 035h, 000h, 000h
        push   eax
        call   sub_1_1646
        add    esp, 4
        db  039h, 035h, 0E8h, 035h, 000h, 000h
        jg     SHORT loc_1_1744
loc_1_175E:
        db  08Bh, 035h, 0E8h, 035h, 000h, 000h
        or     esi, esi
        jle    SHORT loc_1_177C
loc_1_1768:
        db  08Ah, 086h, 0EBh, 035h, 000h, 000h
        dec    esi
        push   eax
        call   sub_1_1666
        add    esp, 4
        or     esi, esi
        jg     SHORT loc_1_1768
loc_1_177C:
        db  08Bh, 035h, 048h, 035h, 000h, 000h
        or     esi, esi
        jle    SHORT loc_1_179A
loc_1_1786:
        db  08Ah, 086h, 04Bh, 035h, 000h, 000h
        dec    esi
        push   eax
        call   sub_1_1666
        add    esp, 4
        or     esi, esi
        jg     SHORT loc_1_1786
loc_1_179A:
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
        ret
sub_1_17B0:
        movzx  eax, byte ptr [esp + 4]
        db  0FFh, 035h, 064h, 036h, 000h, 000h
        push   eax
        db  068h, 068h, 036h, 000h, 000h
        call   sub_1_1620
        add    esp, 0ch
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_1_17D7
        mov    eax, 1
loc_1_17D7:
        ret
sub_1_17D8:
        movzx  eax, byte ptr [esp + 4]
        db  0FFh, 035h, 048h, 035h, 000h, 000h
        push   eax
        db  068h, 04Ch, 035h, 000h, 000h
        call   sub_1_1620
        add    esp, 0ch
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_1_17FF
        mov    eax, 1
loc_1_17FF:
        ret
sub_1_1800:
        movzx  eax, byte ptr [esp + 4]
        db  0FFh, 035h, 0E8h, 035h, 000h, 000h
        push   eax
        db  068h, 0ECh, 035h, 000h, 000h
        call   sub_1_1620
        add    esp, 0ch
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_1_1827
        mov    eax, 1
loc_1_1827:
        ret
sub_1_1828:
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 000h
        je     SHORT loc_1_1893
        mov    esi, 0
        mov    eax, 0
        jle    SHORT loc_1_1880
        sub    edi, edi
loc_1_1842:
        db  08Ah, 098h, 0ECh, 035h, 000h, 000h
        cmp    bl, 0fch
        je     SHORT loc_1_1880
        mov    ecx, edi
        db  039h, 03Dh, 048h, 035h, 000h, 000h
        jle    SHORT loc_1_1871
loc_1_1857:
        db  08Ah, 091h, 04Ch, 035h, 000h, 000h
        db  088h, 096h, 04Ch, 035h, 000h, 000h
        cmp    bl, dl
        je     SHORT loc_1_1868
        inc    esi
loc_1_1868:
        inc    ecx
        db  03Bh, 00Dh, 048h, 035h, 000h, 000h
        jl     SHORT loc_1_1857
loc_1_1871:
        db  089h, 035h, 048h, 035h, 000h, 000h
        inc    eax
        db  03Bh, 005h, 0E8h, 035h, 000h, 000h
        jl     SHORT loc_1_1842
loc_1_1880:
        db  080h, 0B8h, 0ECh, 035h, 000h, 000h, 0FCh
        jne    SHORT loc_1_1893
        db  0C7h, 005h, 048h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1893:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1897:
        push   ebx
        mov    dl, byte ptr [esp + 8]
        or     dl, dl
        je     SHORT loc_1_18CC
        sub    ecx, ecx
        sub    eax, eax
        db  039h, 005h, 048h, 035h, 000h, 000h
        jle    SHORT loc_1_18C6
loc_1_18AC:
        db  08Ah, 098h, 04Ch, 035h, 000h, 000h
        db  088h, 099h, 04Ch, 035h, 000h, 000h
        cmp    dl, bl
        je     SHORT loc_1_18BD
        inc    ecx
loc_1_18BD:
        inc    eax
        db  03Bh, 005h, 048h, 035h, 000h, 000h
        jl     SHORT loc_1_18AC
loc_1_18C6:
        db  089h, 00Dh, 048h, 035h, 000h, 000h
loc_1_18CC:
        pop    ebx
        ret
sub_1_18CE:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1973
        mov    edi, 0
        mov    ebp, 0
        jle    SHORT loc_1_1934
loc_1_18EB:
        db  08Ah, 09Dh, 0ECh, 035h, 000h, 000h
        cmp    bl, 0fch
        je     SHORT loc_1_1934
        sub    esi, esi
        db  039h, 035h, 064h, 036h, 000h, 000h
        jle    SHORT loc_1_1925
loc_1_1900:
        db  08Ah, 086h, 068h, 036h, 000h, 000h
        db  088h, 087h, 068h, 036h, 000h, 000h
        cmp    al, bl
        je     SHORT loc_1_1913
        inc    edi
        jmp    SHORT loc_1_191C
loc_1_1913:
        push   ebx
        call   sub_1_1666
        add    esp, 4
loc_1_191C:
        inc    esi
        db  039h, 035h, 064h, 036h, 000h, 000h
        jg     SHORT loc_1_1900
loc_1_1925:
        db  089h, 03Dh, 048h, 035h, 000h, 000h
        inc    ebp
        db  03Bh, 02Dh, 0E8h, 035h, 000h, 000h
        jl     SHORT loc_1_18EB
loc_1_1934:
        db  080h, 0BDh, 0ECh, 035h, 000h, 000h, 0FCh
        jne    SHORT loc_1_1973
        sub    esi, esi
        db  039h, 035h, 064h, 036h, 000h, 000h
        jle    SHORT loc_1_195F
loc_1_1947:
        db  08Ah, 086h, 068h, 036h, 000h, 000h
        push   eax
        call   sub_1_1666
        add    esp, 4
        inc    esi
        db  03Bh, 035h, 064h, 036h, 000h, 000h
        jl     SHORT loc_1_1947
loc_1_195F:
        db  0C7h, 005h, 064h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1973:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_1978:
        ret
loc_1_1979:
        ret
loc_1_197A:
        ret
        db  0C3h
loc_1_197C:
        ret
loc_1_197D:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 0fdh
        je     NEAR PTR loc_1_1A33
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_1A58
        cmp    bl, 0ech
        jb     SHORT loc_1_19AF
        cmp    bl, 0fch
        je     SHORT loc_1_19AF
        call   sub_1_0643
        jmp    NEAR PTR loc_1_1A82
loc_1_19AF:
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 014h
        jl     SHORT loc_1_19CC
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        jmp    NEAR PTR loc_1_1A78
loc_1_19CC:
        cmp    bl, 0fch
        jne    SHORT loc_1_19D5
        mov    bl, 0fch
        jmp    SHORT loc_1_1A09
loc_1_19D5:
        or     bl, bl
        jne    SHORT loc_1_19ED
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        jmp    NEAR PTR loc_1_1A78
loc_1_19ED:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1A07
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1A09
loc_1_1A07:
        sub    bl, bl
loc_1_1A09:
        push   ebx
        call   sub_1_1800
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1A27
        db  0A1h, 0E8h, 035h, 000h, 000h
        db  088h, 098h, 0ECh, 035h, 000h, 000h
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
loc_1_1A27:
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_1A82
loc_1_1A33:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_1_1A82
loc_1_1A58:
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 000h
        jne    SHORT loc_1_1A6E
        db  0C6h, 005h, 0ECh, 035h, 000h, 000h, 0FCh
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
loc_1_1A6E:
        call   sub_1_18CE
        call   sub_1_1828
loc_1_1A78:
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1A82:
        pop    ebx
        ret
loc_1_1A84:
        push   ebx
        push   esi
        mov    bl, byte ptr [esp + 0ch]
        movzx  eax, bl
        cmp    eax, 0fdh
        je     NEAR PTR loc_1_1B2A
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_1B4F
        cmp    bl, 0ech
        jb     SHORT loc_1_1AB2
        call   sub_1_0643
        jmp    NEAR PTR loc_1_1BB4
loc_1_1AB2:
        db  0A1h, 064h, 036h, 000h, 000h
        db  003h, 005h, 0E8h, 035h, 000h, 000h
        cmp    eax, 14h
        jl     SHORT loc_1_1AD6
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        jmp    NEAR PTR loc_1_1BAA
loc_1_1AD6:
        or     bl, bl
        jne    SHORT loc_1_1AEE
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        jmp    NEAR PTR loc_1_1BAA
loc_1_1AEE:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1AFD
        sub    bl, bl
loc_1_1AFD:
        push   ebx
        call   sub_1_1800
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1B1B
        db  0A1h, 0E8h, 035h, 000h, 000h
        db  088h, 098h, 0ECh, 035h, 000h, 000h
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
loc_1_1B1B:
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1BB4
loc_1_1B2A:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_1_1BB4
loc_1_1B4F:
        sub    esi, esi
        db  039h, 035h, 0E8h, 035h, 000h, 000h
        jle    SHORT loc_1_1B9C
loc_1_1B59:
        db  08Ah, 09Eh, 0ECh, 035h, 000h, 000h
        or     bl, bl
        je     SHORT loc_1_1B93
        db  0A1h, 064h, 036h, 000h, 000h
        push   ebx
        db  088h, 098h, 068h, 036h, 000h, 000h
        db  0FFh, 005h, 064h, 036h, 000h, 000h
        call   sub_1_1646
        add    esp, 4
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1B93
        push   ebx
        call   sub_1_1897
        add    esp, 4
loc_1_1B93:
        inc    esi
        db  039h, 035h, 0E8h, 035h, 000h, 000h
        jg     SHORT loc_1_1B59
loc_1_1B9C:
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 000h
        jne    SHORT loc_1_1BAA
        call   sub_1_0652
loc_1_1BAA:
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1BB4:
        pop    esi
        pop    ebx
        ret
loc_1_1BB7:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 0fdh
        je     NEAR PTR loc_1_1C66
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_1C8B
        cmp    bl, 0ech
        jb     SHORT loc_1_1BE4
        call   sub_1_0643
        jmp    NEAR PTR loc_1_1CDC
loc_1_1BE4:
        db  0A1h, 048h, 035h, 000h, 000h
        db  003h, 005h, 0E8h, 035h, 000h, 000h
        cmp    eax, 14h
        jl     SHORT loc_1_1C08
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        jmp    NEAR PTR loc_1_1CD2
loc_1_1C08:
        or     bl, bl
        jne    SHORT loc_1_1C20
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        jmp    NEAR PTR loc_1_1CD2
loc_1_1C20:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1C3A
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1C3C
loc_1_1C3A:
        sub    bl, bl
loc_1_1C3C:
        push   ebx
        call   sub_1_1800
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1C5A
        db  0A1h, 0E8h, 035h, 000h, 000h
        db  088h, 098h, 0ECh, 035h, 000h, 000h
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
loc_1_1C5A:
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_1CDC
loc_1_1C66:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_1_1CDC
loc_1_1C8B:
        sub    ecx, ecx
        db  039h, 00Dh, 0E8h, 035h, 000h, 000h
        jle    SHORT loc_1_1CC4
        sub    edx, edx
loc_1_1C97:
        db  0A1h, 048h, 035h, 000h, 000h
        db  08Ah, 099h, 0ECh, 035h, 000h, 000h
        db  088h, 098h, 04Ch, 035h, 000h, 000h
        db  0A1h, 048h, 035h, 000h, 000h
        db  038h, 090h, 04Ch, 035h, 000h, 000h
        je     SHORT loc_1_1CBB
        db  0FFh, 005h, 048h, 035h, 000h, 000h
loc_1_1CBB:
        inc    ecx
        db  03Bh, 00Dh, 0E8h, 035h, 000h, 000h
        jl     SHORT loc_1_1C97
loc_1_1CC4:
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 000h
        jne    SHORT loc_1_1CD2
        call   sub_1_0652
loc_1_1CD2:
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1CDC:
        pop    ebx
        ret
loc_1_1CDE:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 0fdh
        je     NEAR PTR loc_1_1D9A
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_1DBF
        cmp    bl, 0ech
        jb     SHORT loc_1_1D0B
        call   sub_1_0643
        jmp    NEAR PTR loc_1_1DD8
loc_1_1D0B:
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 014h
        jl     SHORT loc_1_1D32
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1DD8
loc_1_1D32:
        or     bl, bl
        jne    SHORT loc_1_1D54
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1DD8
loc_1_1D54:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1D6E
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1D70
loc_1_1D6E:
        sub    bl, bl
loc_1_1D70:
        push   ebx
        call   sub_1_1800
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1D8E
        db  0A1h, 0E8h, 035h, 000h, 000h
        db  088h, 098h, 0ECh, 035h, 000h, 000h
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
loc_1_1D8E:
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_1DD8
loc_1_1D9A:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_1_1DD8
loc_1_1DBF:
        call   sub_1_1715
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1DD8:
        pop    ebx
        ret
loc_1_1DDA:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 0fdh
        je     NEAR PTR loc_1_1E86
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_1EAB
        cmp    bl, 0ech
        jb     SHORT loc_1_1E07
        call   sub_1_0643
        jmp    NEAR PTR loc_1_1EC4
loc_1_1E07:
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 014h
        jl     SHORT loc_1_1E2E
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1EC4
loc_1_1E2E:
        or     bl, bl
        jne    SHORT loc_1_1E4D
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_1EC4
loc_1_1E4D:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1E67
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1E69
loc_1_1E67:
        sub    bl, bl
loc_1_1E69:
        db  0A1h, 0E8h, 035h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  088h, 098h, 0ECh, 035h, 000h, 000h
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
        jmp    SHORT loc_1_1EC4
loc_1_1E86:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_1_1EC4
loc_1_1EAB:
        call   sub_1_168B
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1EC4:
        pop    ebx
        ret
loc_1_1EC6:
        mov    cl, byte ptr [esp + 4]
        movzx  eax, cl
        cmp    eax, 6
        je     SHORT loc_1_1EFE
        cmp    eax, 7
        je     SHORT loc_1_1F14
        cmp    eax, 8
        je     SHORT loc_1_1F2A
        cmp    eax, 9
        je     SHORT loc_1_1F40
        cmp    cl, 0ech
        jae    SHORT loc_1_1EED
        call   sub_1_0643
        jmp    SHORT loc_1_1F5E
loc_1_1EED:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        jmp    SHORT loc_1_1F54
loc_1_1EFE:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 078h, 019h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 07Ch, 02Bh, 000h, 000h
        jmp    SHORT loc_1_1F54
loc_1_1F14:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 079h, 019h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 070h, 02Bh, 000h, 000h
        jmp    SHORT loc_1_1F54
loc_1_1F2A:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 07Ch, 019h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 04Ch, 02Bh, 000h, 000h
        jmp    SHORT loc_1_1F54
loc_1_1F40:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 07Ah, 019h, 000h, 000h
        db  0C7h, 005h, 034h, 036h, 000h, 000h, 064h, 02Bh, 000h, 000h
loc_1_1F54:
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_1F5E:
        ret
        db  000h
sub_1_1F60:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0ch]
        test   al, 1
        jne    SHORT loc_1_1F9E
        test   al, 2
        je     SHORT loc_1_1F77
        db  083h, 00Dh, 0C4h, 029h, 000h, 000h, 020h
loc_1_1F77:
        test   byte ptr [esi + 0ch], 8
        je     SHORT loc_1_1F84
        db  083h, 00Dh, 0C4h, 029h, 000h, 000h, 010h
loc_1_1F84:
        test   byte ptr [esi + 0ch], 4
        je     SHORT loc_1_1F91
        db  083h, 025h, 0C4h, 029h, 000h, 000h, 0DFh
loc_1_1F91:
        test   byte ptr [esi + 0ch], 10h
        je     SHORT loc_1_1F9E
        db  083h, 025h, 0C4h, 029h, 000h, 000h, 0EFh
loc_1_1F9E:
        db  0FFh, 035h, 0C4h, 029h, 000h, 000h
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   sub_1_038E
        add    esp, 10h
        mov    dword ptr [esi + 8], 0
        mov    dword ptr [esi + 4], 0
        pop    esi
        ret
loc_1_1FC4:
        cmp    byte ptr [esp + 4], 0ffh
        jne    SHORT loc_1_2019
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 014h, 000h, 000h, 000h
        db  0FFh, 035h, 0C4h, 034h, 000h, 000h
        call   sub_1_1F60
        add    esp, 4
        db  0C7h, 005h, 0C4h, 029h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        push   0fffff830h
        push   0fffff830h
        push   2
        call   sub_1_038E
        add    esp, 10h
        db  0C7h, 005h, 058h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 050h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_2028
loc_1_2019:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
loc_1_2028:
        ret
sub_1_2029:
        push   esi
        push   edi
        db  00Fh, 0B6h, 005h, 0ECh, 035h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 0EDh, 035h, 000h, 000h
        shl    ecx, 8
        lea    esi, [eax + ecx]
        db  00Fh, 0B6h, 00Dh, 0EFh, 035h, 000h, 000h
        db  00Fh, 0B6h, 005h, 0EEh, 035h, 000h, 000h
        shl    ecx, 8
        lea    edi, [eax + ecx]
        db  089h, 035h, 050h, 036h, 000h, 000h
        db  089h, 03Dh, 058h, 036h, 000h, 000h
        db  0FFh, 035h, 0C4h, 029h, 000h, 000h
        push   0fffff830h
        push   0fffff830h
        push   2
        call   sub_1_038E
        add    esp, 10h
        db  0FFh, 035h, 0C4h, 029h, 000h, 000h
        push   esi
        push   edi
        push   2
        call   sub_1_038E
        add    esp, 10h
        pop    edi
        pop    esi
        ret
sub_1_208E:
        push   esi
        db  00Fh, 0B6h, 005h, 0ECh, 035h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 0EDh, 035h, 000h, 000h
        shl    ecx, 8
        lea    edx, [eax + ecx]
        db  00Fh, 0B6h, 00Dh, 0EFh, 035h, 000h, 000h
        db  00Fh, 0B6h, 005h, 0EEh, 035h, 000h, 000h
        shl    ecx, 8
        lea    esi, [eax + ecx]
        db  08Bh, 00Dh, 050h, 036h, 000h, 000h
        add    ecx, edx
        db  0A1h, 058h, 036h, 000h, 000h
        add    eax, esi
        or     ecx, ecx
        jge    SHORT loc_1_20CC
        sub    ecx, ecx
loc_1_20CC:
        or     eax, eax
        jge    SHORT loc_1_20D2
        sub    eax, eax
loc_1_20D2:
        db  089h, 00Dh, 050h, 036h, 000h, 000h
        db  0A3h, 058h, 036h, 000h, 000h
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 001h, 000h, 000h, 000h
        db  089h, 035h, 0B8h, 034h, 000h, 000h
        db  089h, 015h, 0BCh, 034h, 000h, 000h
        db  0FFh, 035h, 0C4h, 034h, 000h, 000h
        call   sub_1_1F60
        add    esp, 4
        pop    esi
        ret
loc_1_2103:
        movzx  eax, byte ptr [esp + 4]
        cmp    eax, 0ffh
        je     SHORT loc_1_2116
        call   sub_1_0643
        jmp    SHORT loc_1_217B
loc_1_2116:
        db  083h, 03Dh, 0C0h, 029h, 000h, 000h, 000h
        je     SHORT loc_1_215D
        db  0A1h, 0E0h, 035h, 000h, 000h
        db  039h, 005h, 018h, 036h, 000h, 000h
        jne    SHORT loc_1_214C
        db  0A1h, 05Ch, 036h, 000h, 000h
        db  0A3h, 050h, 036h, 000h, 000h
        db  0A1h, 060h, 036h, 000h, 000h
        db  0A3h, 058h, 036h, 000h, 000h
        db  0C7h, 005h, 0C0h, 029h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_2167
loc_1_214C:
        call   sub_1_0400
        db  0C7h, 005h, 0C0h, 029h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_2167
loc_1_215D:
        db  0C7h, 005h, 0C0h, 029h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_2167:
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_217B:
        ret
loc_1_217C:
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 004h
        jl     SHORT loc_1_218C
        call   sub_1_0643
        jmp    SHORT loc_1_21BD
loc_1_218C:
        db  0A1h, 0E8h, 035h, 000h, 000h
        mov    cl, byte ptr [esp + 4]
        db  088h, 088h, 0ECh, 035h, 000h, 000h
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 002h
        je     SHORT loc_1_21B3
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 004h
        jne    SHORT loc_1_21BD
loc_1_21B3:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 00Eh, 022h, 000h, 000h
loc_1_21BD:
        ret
loc_1_21BE:
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 004h
        jl     SHORT loc_1_21CE
        call   sub_1_0643
        jmp    SHORT loc_1_220D
loc_1_21CE:
        db  0A1h, 0E8h, 035h, 000h, 000h
        mov    cl, byte ptr [esp + 4]
        db  088h, 088h, 0ECh, 035h, 000h, 000h
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
        cmp    cl, 7fh
        db  0A1h, 0E8h, 035h, 000h, 000h
        jbe    SHORT loc_1_21F6
        db  0C6h, 080h, 0ECh, 035h, 000h, 000h, 0FFh
        jmp    SHORT loc_1_21FD
loc_1_21F6:
        db  0C6h, 080h, 0ECh, 035h, 000h, 000h, 000h
loc_1_21FD:
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 00Eh, 022h, 000h, 000h
loc_1_220D:
        ret
loc_1_220E:
        movzx  eax, byte ptr [esp + 4]
        cmp    eax, 0f5h
        je     SHORT loc_1_222F
        cmp    eax, 0f6h
        je     SHORT loc_1_2245
        cmp    eax, 0ffh
        je     SHORT loc_1_2275
        call   sub_1_0643
        jmp    SHORT loc_1_228E
loc_1_222F:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 07Ch, 021h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_228E
loc_1_2245:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 0BEh, 021h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_228E
        db  08Bh, 00Dh, 0E8h, 035h, 000h, 000h, 088h, 091h, 0ECh, 035h, 000h, 000h, 0FFh, 005h, 0E8h, 035h
        db  000h, 000h, 039h, 005h, 0E8h, 035h, 000h, 000h, 07Dh, 0E6h
loc_1_2275:
        call   sub_1_2029
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_228E:
        ret
loc_1_228F:
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 004h
        jl     SHORT loc_1_229F
        call   sub_1_0643
        jmp    SHORT loc_1_22D0
loc_1_229F:
        db  0A1h, 0E8h, 035h, 000h, 000h
        mov    cl, byte ptr [esp + 4]
        db  088h, 088h, 0ECh, 035h, 000h, 000h
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 002h
        je     SHORT loc_1_22C6
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 004h
        jne    SHORT loc_1_22D0
loc_1_22C6:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 021h, 023h, 000h, 000h
loc_1_22D0:
        ret
loc_1_22D1:
        db  083h, 03Dh, 0E8h, 035h, 000h, 000h, 004h
        jl     SHORT loc_1_22E1
        call   sub_1_0643
        jmp    SHORT loc_1_2320
loc_1_22E1:
        db  0A1h, 0E8h, 035h, 000h, 000h
        mov    cl, byte ptr [esp + 4]
        db  088h, 088h, 0ECh, 035h, 000h, 000h
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
        cmp    cl, 7fh
        db  0A1h, 0E8h, 035h, 000h, 000h
        jbe    SHORT loc_1_2309
        db  0C6h, 080h, 0ECh, 035h, 000h, 000h, 0FFh
        jmp    SHORT loc_1_2310
loc_1_2309:
        db  0C6h, 080h, 0ECh, 035h, 000h, 000h, 000h
loc_1_2310:
        db  0FFh, 005h, 0E8h, 035h, 000h, 000h
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 021h, 023h, 000h, 000h
loc_1_2320:
        ret
loc_1_2321:
        movzx  eax, byte ptr [esp + 4]
        cmp    eax, 0f5h
        je     SHORT loc_1_2342
        cmp    eax, 0f6h
        je     SHORT loc_1_2358
        cmp    eax, 0ffh
        je     SHORT loc_1_2388
        call   sub_1_0643
        jmp    SHORT loc_1_23A1
loc_1_2342:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 08Fh, 022h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_23A1
loc_1_2358:
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 0D1h, 022h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_23A1
        db  08Bh, 00Dh, 0E8h, 035h, 000h, 000h, 088h, 091h, 0ECh, 035h, 000h, 000h, 0FFh, 005h, 0E8h, 035h
        db  000h, 000h, 039h, 005h, 0E8h, 035h, 000h, 000h, 07Dh, 0E6h
loc_1_2388:
        call   sub_1_208E
        db  0C7h, 005h, 0E8h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_23A1:
        ret
sub_1_23A2:
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 038h, 036h, 000h, 000h, 000h
        je     SHORT loc_1_23D2
        db  083h, 03Dh, 048h, 036h, 000h, 000h, 000h
        jne    SHORT loc_1_23D2
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 002h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 036h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_23D2:
        db  083h, 03Dh, 03Ch, 036h, 000h, 000h, 000h
        je     SHORT loc_1_23F5
        db  083h, 03Dh, 04Ch, 036h, 000h, 000h, 000h
        jne    SHORT loc_1_23F5
        db  083h, 005h, 0C0h, 034h, 000h, 000h, 008h
        db  0C7h, 005h, 04Ch, 036h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_23F5:
        db  083h, 03Dh, 0C0h, 034h, 000h, 000h, 000h
        je     SHORT loc_1_240C
        db  0FFh, 035h, 0C4h, 034h, 000h, 000h
        call   sub_1_1F60
        add    esp, 4
loc_1_240C:
        ret
sub_1_240D:
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 038h, 036h, 000h, 000h, 000h
        je     SHORT loc_1_243D
        db  083h, 03Dh, 048h, 036h, 000h, 000h, 000h
        je     SHORT loc_1_243D
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 004h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_243D:
        db  083h, 03Dh, 03Ch, 036h, 000h, 000h, 000h
        je     SHORT loc_1_2460
        db  083h, 03Dh, 04Ch, 036h, 000h, 000h, 000h
        je     SHORT loc_1_2460
        db  083h, 005h, 0C0h, 034h, 000h, 000h, 010h
        db  0C7h, 005h, 04Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2460:
        db  083h, 03Dh, 0C0h, 034h, 000h, 000h, 000h
        je     SHORT loc_1_2477
        db  0FFh, 035h, 0C4h, 034h, 000h, 000h
        call   sub_1_1F60
        add    esp, 4
loc_1_2477:
        ret
loc_1_2478:
        mov    cl, byte ptr [esp + 4]
        movzx  eax, cl
        cmp    eax, 1
        je     SHORT loc_1_24C6
        cmp    eax, 2
        je     SHORT loc_1_24DF
        cmp    eax, 0fch
        je     SHORT loc_1_24C6
        cmp    eax, 0fdh
        je     SHORT loc_1_24F5
        cmp    eax, 0ffh
        je     SHORT loc_1_2506
        cmp    cl, 0ech
        jb     SHORT loc_1_24AD
        call   sub_1_0643
        jmp    NEAR PTR loc_1_256C
loc_1_24AD:
        db  0C7h, 005h, 0C8h, 034h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_256C
loc_1_24C6:
        db  0C7h, 005h, 038h, 036h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_256C
loc_1_24DF:
        db  0C7h, 005h, 03Ch, 036h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_256C
loc_1_24F5:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        jmp    SHORT loc_1_256C
loc_1_2506:
        db  083h, 03Dh, 038h, 036h, 000h, 000h, 000h
        jne    SHORT loc_1_253F
        db  083h, 03Dh, 03Ch, 036h, 000h, 000h, 000h
        jne    SHORT loc_1_253F
        db  083h, 03Dh, 0C8h, 034h, 000h, 000h, 000h
        jne    SHORT loc_1_253F
        db  0C7h, 005h, 0C8h, 034h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 03Ch, 036h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 038h, 036h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_253F:
        call   sub_1_240D
        db  0C7h, 005h, 0C8h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 03Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 038h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_256C:
        ret
loc_1_256D:
        mov    cl, byte ptr [esp + 4]
        movzx  eax, cl
        cmp    eax, 1
        je     SHORT loc_1_25BB
        cmp    eax, 2
        je     SHORT loc_1_25D1
        cmp    eax, 0fch
        je     SHORT loc_1_25BB
        cmp    eax, 0fdh
        je     SHORT loc_1_25E7
        cmp    eax, 0ffh
        je     SHORT loc_1_25F8
        cmp    cl, 0ech
        jb     SHORT loc_1_25A2
        call   sub_1_0643
        jmp    NEAR PTR loc_1_264A
loc_1_25A2:
        db  0C7h, 005h, 0C8h, 034h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_264A
loc_1_25BB:
        db  0C7h, 005h, 038h, 036h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_264A
loc_1_25D1:
        db  0C7h, 005h, 03Ch, 036h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_264A
loc_1_25E7:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        jmp    SHORT loc_1_264A
loc_1_25F8:
        db  083h, 03Dh, 038h, 036h, 000h, 000h, 000h
        jne    SHORT loc_1_261D
        db  083h, 03Dh, 03Ch, 036h, 000h, 000h, 000h
        jne    SHORT loc_1_261D
        db  083h, 03Dh, 0C8h, 034h, 000h, 000h, 000h
        jne    SHORT loc_1_261D
        db  0C7h, 005h, 038h, 036h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_261D:
        call   sub_1_23A2
        db  0C7h, 005h, 0C8h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 03Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 038h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_264A:
        ret
loc_1_264B:
        mov    al, byte ptr [esp + 4]
        cmp    al, 0ffh
        jne    NEAR PTR loc_1_271B
        db  0A1h, 038h, 036h, 000h, 000h
        or     eax, eax
        db  08Bh, 015h, 03Ch, 036h, 000h, 000h
        jne    SHORT loc_1_2678
        or     edx, edx
        jne    SHORT loc_1_2678
        db  083h, 03Dh, 0C8h, 034h, 000h, 000h, 000h
        jne    SHORT loc_1_2678
        mov    eax, 1
loc_1_2678:
        db  0A3h, 038h, 036h, 000h, 000h
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_26A8
        db  083h, 03Dh, 048h, 036h, 000h, 000h, 000h
        je     SHORT loc_1_26A8
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 004h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_26A8:
        or     edx, edx
        je     SHORT loc_1_26C6
        db  083h, 03Dh, 04Ch, 036h, 000h, 000h, 000h
        je     SHORT loc_1_26C6
        db  083h, 005h, 0C0h, 034h, 000h, 000h, 010h
        db  0C7h, 005h, 04Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_26C6:
        db  083h, 03Dh, 0C0h, 034h, 000h, 000h, 000h
        jne    SHORT loc_1_26DD
        db  0FFh, 035h, 0C4h, 034h, 000h, 000h
        call   sub_1_1F60
        add    esp, 4
loc_1_26DD:
        call   sub_1_23A2
        call   sub_1_240D
        call   sub_1_23A2
        call   sub_1_240D
        db  0C7h, 005h, 0C8h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 03Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 038h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_1_2724
loc_1_271B:
        push   eax
        call   sub_1_2725
        add    esp, 4
loc_1_2724:
        ret
sub_1_2725:
        mov    cl, byte ptr [esp + 4]
        movzx  eax, cl
        cmp    eax, 1
        je     SHORT loc_1_2773
        cmp    eax, 2
        je     SHORT loc_1_278C
        cmp    eax, 0fch
        je     SHORT loc_1_2773
        cmp    eax, 0fdh
        je     SHORT loc_1_27A5
        cmp    eax, 0ffh
        je     SHORT loc_1_27B9
        cmp    cl, 0ech
        jb     SHORT loc_1_275A
        call   sub_1_0643
        jmp    NEAR PTR loc_1_2871
loc_1_275A:
        db  0C7h, 005h, 0C8h, 034h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_2871
loc_1_2773:
        db  0C7h, 005h, 038h, 036h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_2871
loc_1_278C:
        db  0C7h, 005h, 03Ch, 036h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_2871
loc_1_27A5:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        jmp    NEAR PTR loc_1_2871
loc_1_27B9:
        db  0A1h, 038h, 036h, 000h, 000h
        or     eax, eax
        db  08Bh, 015h, 03Ch, 036h, 000h, 000h
        jne    SHORT loc_1_27DA
        or     edx, edx
        jne    SHORT loc_1_27DA
        db  083h, 03Dh, 0C8h, 034h, 000h, 000h, 000h
        jne    SHORT loc_1_27DA
        mov    eax, 1
loc_1_27DA:
        db  0A3h, 038h, 036h, 000h, 000h
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_280A
        db  083h, 03Dh, 048h, 036h, 000h, 000h, 000h
        je     SHORT loc_1_280A
        db  0C7h, 005h, 0C0h, 034h, 000h, 000h, 004h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_280A:
        or     edx, edx
        je     SHORT loc_1_2828
        db  083h, 03Dh, 04Ch, 036h, 000h, 000h, 000h
        je     SHORT loc_1_2828
        db  083h, 005h, 0C0h, 034h, 000h, 000h, 010h
        db  0C7h, 005h, 04Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2828:
        db  083h, 03Dh, 0C0h, 034h, 000h, 000h, 000h
        jne    SHORT loc_1_283F
        db  0FFh, 035h, 0C4h, 034h, 000h, 000h
        call   sub_1_1F60
        add    esp, 4
loc_1_283F:
        call   sub_1_23A2
        call   sub_1_240D
        db  0C7h, 005h, 0C8h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 03Ch, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 038h, 036h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_2871:
        ret
loc_1_2872:
        mov    cl, byte ptr [esp + 4]
        movzx  eax, cl
        cmp    eax, 0fdh
        je     SHORT loc_1_288E
        cmp    cl, 0ech
        jae    SHORT loc_1_2887
        jmp    SHORT loc_1_288E
loc_1_2887:
        call   sub_1_0643
        jmp    SHORT loc_1_28A7
loc_1_288E:
        call   sub_1_0652
        db  0C7h, 005h, 068h, 035h, 000h, 000h, 082h, 008h, 000h, 000h
        db  0C7h, 005h, 078h, 035h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_28A7:
        ret
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 080h, 000h, 000h, 045h, 04Eh, 041h, 042h
        db  04Ch, 045h, 034h, 020h, 000h, 000h, 000h, 000h, 07Ah, 004h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_1_291C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2920:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2924:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2928:
        add    byte ptr [eax], al
loc_1_292A:
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
loc_1_295A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_295E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2966:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_296E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2972:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2984:
        add    byte ptr [eax], al
loc_1_2986:
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
loc_1_299E:
        add    byte ptr [eax], al
        jae    SHORT loc_1_29A6
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
loc_1_29A6:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_29B8:
        db  070h, 072h
        db  065h, 073h, 073h
        sub    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_29C8:
        inc    edx
        inc    ebp
        inc    edi
        dec    ecx
        dec    esi
        add    byte ptr [eax], al
        db  000h
loc_1_29D0:
        inc    edx
        dec    esp
        dec    ebx
        push   esp
        push   edx
        dec    esi
        push   ebx
        db  000h
loc_1_29D8:
        inc    ebx
        dec    esp
        inc    ebp
        inc    ecx
        push   edx
        add    byte ptr [eax], al
        db  000h
loc_1_29E0:
        inc    ebp
        dec    esi
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0C8h
        sub    dword ptr [eax], eax
        add    dl, bh
        shr    byte ptr [ecx], 1
        add    byte ptr [eax], al
        db  0F7h, 0D8h
        sub    dword ptr [eax], eax
        add    bl, bh
        db  0E0h, 029h
        add    byte ptr [eax], al
        stc
        in     al, 29h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A04:
        bound  esp, qword ptr [ecx + 75h]
        db  064h, 072h, 061h
        db  074h, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A10:
        arpl   word ptr [ecx + ebp*2 + 63h], bp
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx + 6fh], ah
        insd
        bound  ebp, qword ptr [ecx + 6eh]
        db  065h, 000h
loc_1_2A20:
        arpl   word ptr [edi + 6dh], bp
        insd
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A28:
        bound  ebp, qword ptr fs:[ebx + 6ch]
        imul   esp, dword ptr [ebx + 6bh], 0
loc_1_2A34:
        outsb
        db  000h
loc_1_2A38:
        outsd
        db  074h, 06Fh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A40:
        push   646c6fh
        add    byte ptr [eax], al
        add    byte ptr [ebx + 62h], ch
        add    byte ptr fs:[edi + ebp*2 + 63h], ch
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_1_2A54:
        insd
        outsd
        jne    SHORT loc_1_2A58
loc_1_2A58:
        insd
        outsd
        jne    SHORT loc_1_2ABD
        outsb
        arpl   word ptr [eax + 6fh], bp
        jb     SHORT loc_1_2A62
loc_1_2A62:
        add    byte ptr [eax], al
loc_1_2A64:
        insd
        outsd
        jne    SHORT loc_1_2AD4
        outsd
        arpl   word ptr [ebx], bp
loc_1_2A6C:
        insd
        outsd
        db  075h, 072h
        insb
        add    byte ptr [eax], al
loc_1_2A74:
        insd
        outsd
        jne    SHORT loc_1_2AEA
        db  065h, 073h, 065h
        je     SHORT loc_1_2A7D
loc_1_2A7D:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 6fh], ch
        db  076h, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A88:
        jo     SHORT loc_1_2AFC
        jae    NEAR PTR loc_1_2B00
        add    byte ptr [eax], al
        add    byte ptr [edx + 65h], dh
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 004h, 02Ah
        add    byte ptr [eax], al
        xchg   edi, eax
        adc    byte ptr [edx], ch
        add    byte ptr [eax], al
        db  00Dh, 018h, 02Ah, 000h, 000h
        db  003h, 020h
        sub    al, byte ptr [eax]
        db  000h, 096h, 028h, 02Ah, 000h, 000h
        push   cs
        xor    al, 2ah
        add    byte ptr [eax], al
        db  0A0h, 038h, 02Ah, 000h, 000h
        db  010h, 040h, 02Ah
loc_1_2ABD:
        add    byte ptr [eax], al
        db  004h, 048h
        sub    al, byte ptr [eax]
        add    byte ptr [edx], cl
        dec    esp
        sub    al, byte ptr [eax]
        db  000h, 005h, 054h, 02Ah, 000h, 000h
        db  014h, 058h
        sub    al, byte ptr [eax]
        add    byte ptr [edx], dl
loc_1_2AD4:
        sub    al, byte ptr fs:[eax]
        add    byte ptr [ebx], cl
        insb
        sub    al, byte ptr [eax]
        db  000h, 00Ch, 074h
        sub    al, byte ptr [eax]
        add    byte ptr [ecx], dl
        sub    byte ptr [edx], 0
        add    byte ptr [edi], cl
        mov    byte ptr [edx], ch
loc_1_2AEA:
        add    byte ptr [eax], al
        db  002h, 090h, 02Ah, 000h, 000h, 006h
        xchg   esp, eax
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
        db  000h
loc_1_2AF8:
        jae    NEAR PTR loc_1_2B5F
loc_1_2AFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B00:
        imul   ebp, dword ptr [esi + 64h], 646f6d00h
        insb
        add    byte ptr [eax], al
        add    byte ptr [esi + 65h], dh
        jb     SHORT loc_1_2B10
loc_1_2B10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B14:
        clc
        sub    al, byte ptr [eax]
        add    byte ptr [ecx], cl
        add    byte ptr [ebx], ch
        add    byte ptr [eax], al
        push   es
        add    al, 2bh
        add    byte ptr [eax], al
        db  008h, 00Ch, 02Bh
        add    byte ptr [eax], al
        pop    es
        adc    byte ptr [ebx], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B30:
        imul   esp, dword ptr [edx + 6dh], 7461h
        db  000h
loc_1_2B38:
        imul   esp, dword ptr [edx + 6dh], 63686e65h
        add    byte ptr [ecx + 62h], ch
        insd
        jo     SHORT loc_1_2BA8
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 030h
        sub    eax, dword ptr [eax]
        add    byte ptr [edx], al
        cmp    byte ptr [ebx], ch
        add    byte ptr [eax], al
        db  001h, 040h, 02Bh
        add    byte ptr [eax], al
        db  003h, 048h, 02Bh
        add    byte ptr [eax], al
loc_1_2B5F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 060h, 02Bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B70:
        insb
        sub    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 078h, 02Bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B88:
        test   byte ptr [ebx], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B90:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B94:
        nop
        sub    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 09Ch, 02Bh, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
loc_1_2BA8:
        xor    dword ptr [edx], esi
        xor    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BB0:
        xor    dh, byte ptr [eax + esi]
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_2BB8:
        xor    esi, dword ptr [eax]
        xor    byte ptr [eax], al
loc_1_2BBC:
        xor    al, 38h
        xor    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BC4:
        xor    byte ptr ss:[eax], dh
        add    byte ptr [ecx], bh
        xor    byte ptr ss:[eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BD4:
        test   al, 2bh
        add    byte ptr [eax], al
        db  003h, 0B0h, 02Bh, 000h, 000h, 004h
        mov    eax, 100002bh
        mov    esp, 500002bh
        les    ebp, ptr [ebx]
        add    byte ptr [eax], al
        db  002h, 0C8h
        sub    eax, dword ptr [eax]
        add    byte ptr [esi], al
        shr    byte ptr [ebx], 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BF8:
        insb
        je     NEAR PTR loc_1_2BFD
loc_1_2BFD:
        add    byte ptr [eax], al
        add    byte ptr [edx + 69h], dh
        push   74h
loc_1_2C08:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C0C:
        clc
        sub    eax, dword ptr [eax]
        add    byte ptr [ecx], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [edx], al
        or     byte ptr [eax + eax], ch
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax], ch
        add    byte ptr [eax], al
        pop    eax
        add    al, byte ptr [eax]
        add    byte ptr [eax + 60000004h], dh
        or     dword ptr [eax], eax
        add    al, al
        adc    al, byte ptr [eax]
        add    byte ptr [eax + 25h], al
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
loc_1_2C39:
        add    edi, dword ptr [edx]
        pop    ds
        cmp    dh, byte ptr [edx]
        cmp    dh, byte ptr [eax]
        cmp    ah, byte ptr [ecx]
        cmp    dl, byte ptr [ebx]
        cmp    ah, byte ptr [edx]
        cmp    ah, byte ptr [ebx]
        cmp    ah, byte ptr [edx + edi]
        sbb    byte ptr [edx], bh
        and    eax, 273a263ah
        cmp    dh, byte ptr [edx + edi]
        xor    edi, dword ptr [edx]
        sbb    dword ptr [edx], edi
        sbb    bh, byte ptr [edx]
        adc    dword ptr [edx], edi
        adc    al, 3ah
        and    byte ptr [edx], bh
        adc    eax, 313a173ah
        cmp    dl, byte ptr [edx]
        cmp    ch, byte ptr [edi]
        cmp    dl, byte ptr [esi]
        cmp    ch, byte ptr [esi]
        cmp    bl, byte ptr [ebx]
        cmp    bl, byte ptr [73a1c3ah]
        cmp    cl, byte ptr [edi + 2c022c00h]
        sub    dword ptr [esp + eax], ebp
        sub    al, 5
        sub    al, 6
        sub    al, 8
        sub    dword ptr [eax], eax
        sub    al, 0ah
        sub    al, 0bh
        sub    al, 9
        sub    al, 0dh
        xor    eax, 36000c00h
        add    byte ptr [edi], dh
        add    byte ptr [ebx], cl
        add    byte ptr [edx], al
        add    byte ptr [ebx], al
        add    byte ptr [eax + eax], al
        add    eax, 7000600h
        add    byte ptr [eax], cl
        add    byte ptr [ecx], cl
        add    byte ptr [edx], cl
        add    byte ptr [eax + ebp], ch
        sub    byte ptr [eax], al
        sub    al, 35h
        or     eax, 2c362c00h
        aaa
        sub    al, 3
        sub    al, 1fh
        sub    al, 32h
        sub    al, 30h
        sub    al, 21h
        sub    al, 13h
        sub    al, 22h
        sub    al, 23h
        sub    al, 24h
        sub    al, 18h
        sub    al, 25h
        sub    al, 26h
        sub    al, 27h
        sub    al, 34h
        sub    al, 33h
        sub    al, 19h
        sub    al, 1ah
        sub    al, 11h
        sub    al, 14h
        sub    al, 20h
        sub    al, 15h
        sub    al, 17h
        sub    al, 31h
        sub    al, 12h
        sub    al, 2fh
        sub    al, 16h
        sub    al, 2eh
        sbb    eax, dword ptr [eax]
        sbb    eax, 2c001c00h
        pop    es
        sub    al, 0ch
        add    dword ptr [eax], eax
        pop    ds
        add    byte ptr [edx], dh
        add    byte ptr [eax], dh
        add    byte ptr [ecx], ah
        add    byte ptr [ebx], dl
        add    byte ptr [edx], ah
        add    byte ptr [ebx], ah
        add    byte ptr [eax + eax], ah
        sbb    byte ptr [eax], al
        and    eax, 27002600h
        add    byte ptr [eax + eax], dh
        xor    eax, dword ptr [eax]
        sbb    dword ptr [eax], eax
        sbb    al, byte ptr [eax]
        adc    dword ptr [eax], eax
        adc    al, 0
        and    byte ptr [eax], al
        adc    eax, 31001700h
        add    byte ptr [edx], dl
        add    byte ptr [edi], ch
        add    byte ptr [esi], dl
        add    byte ptr [esi], ch
        add    byte ptr [ebx + ebx], ch
        sub    al, 1dh
        sub    al, 1ch
        sub    al, 1
        db  03Ah, 068h
loc_1_2D38:
        popad
        insb
        je     SHORT loc_1_2D3C
loc_1_2D3C:
        bound  esp, qword ptr [ecx + 63h]
        imul   esi, dword ptr [ebx + 70h], 61h
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
loc_1_2D48:
        bound  ebp, qword ptr [ebx + 73h]
        jo     SHORT loc_1_2D4D
loc_1_2D4D:
        add    byte ptr [eax], al
        add    byte ptr [edx + 72h], ah
        popad
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx + 61h], ah
        jo     SHORT loc_1_2DCF
        insb
        imul   eax, dword ptr [eax], 0
loc_1_2D60:
        arpl   word ptr [ecx + 70h], sp
        db  073h, 06Ch
        outsd
        arpl   word ptr [ebx], bp
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6fh], ah
        insd
        insd
        popad
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 072h
        outsd
        insb
        db  000h
loc_1_2D7C:
        arpl   word ptr [edx + esi*2 + 6ch], si
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2D84:
        insb
        add    byte ptr [ebp + 6ch], ah
        db  065h, 074h, 065h
        add    byte ptr [eax], al
loc_1_2D90:
        imul   esi, dword ptr fs:[esi + 69h], 6564h
loc_1_2D98:
        outsd
        db  077h, 06Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2DA0:
        outsb
        add    byte ptr fs:[ebp + 6eh], ah
        db  074h, 065h
        jb     SHORT loc_1_2DAA
loc_1_2DAA:
        add    byte ptr [eax], al
loc_1_2DAC:
        db  065h, 073h, 063h
        add    byte ptr [ebp + 73h], ah
        arpl   word ptr [ecx + 70h], sp
        add    byte ptr gs:[eax], al
loc_1_2DB8:
        xor    word ptr [eax], ax
        db  000h
loc_1_2DBC:
        xor    word ptr [eax], si
        add    byte ptr [esi + 31h], ah
        xor    dword ptr [eax], eax
loc_1_2DC4:
        xor    word ptr [edx], si
        add    byte ptr [esi + 32h], ah
        add    byte ptr [eax], al
loc_1_2DCC:
        xor    ax, word ptr [eax]
loc_1_2DCF:
        add    byte ptr [esi + 34h], ah
        add    byte ptr [eax], al
loc_1_2DD4:
        xor    ax, 0
loc_1_2DD8:
        add    byte ptr ss:[eax], al
loc_1_2DDC:
        aaa
        add    byte ptr [eax], al
loc_1_2DE0:
        cmp    byte ptr [eax], al
        add    byte ptr [esi + 39h], ah
        add    byte ptr [eax], al
loc_1_2DE8:
        push   656d6fh
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], ch
        jae    SHORT loc_1_2DF4
loc_1_2DF4:
        imul   ebp, dword ptr [esi + 73h], 747265h
        db  000h
loc_1_2DFC:
        imul   esi, dword ptr [eax + 2ah], 0
loc_1_2E00:
        imul   esi, dword ptr [eax + 2bh], 0
loc_1_2E04:
        imul   esi, dword ptr [eax + 2dh], 0
loc_1_2E08:
        imul   esi, dword ptr [eax + 2fh], 0
loc_1_2E0C:
        imul   esi, dword ptr [eax + 30h], 0
loc_1_2E10:
        imul   esi, dword ptr [eax + 31h], 0
loc_1_2E14:
        imul   esi, dword ptr [eax + 32h], 0
loc_1_2E18:
        imul   esi, dword ptr [eax + 33h], 0
loc_1_2E1C:
        imul   esi, dword ptr [eax + 34h], 0
loc_1_2E20:
        imul   esi, dword ptr [eax + 35h], 0
loc_1_2E24:
        imul   esi, dword ptr [eax + 36h], 0
loc_1_2E28:
        imul   esi, dword ptr [eax + 37h], 0
loc_1_2E2C:
        imul   esi, dword ptr [eax + 38h], 0
loc_1_2E30:
        imul   esi, dword ptr [eax + 39h], 0
loc_1_2E34:
        imul   esi, dword ptr [eax + 64h], 65h
        insb
        add    byte ptr [eax], al
        db  000h
loc_1_2E3C:
        imul   esi, dword ptr [eax + 64h], 65h
        insb
        db  065h, 074h, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2E48:
        imul   esi, dword ptr [eax + 64h], 69h
        db  076h, 069h
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_2E54:
        imul   esi, dword ptr [eax + 64h], 6eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2E5C:
        imul   esi, dword ptr [eax + 64h], 6fh
        ja     SHORT loc_1_2ED0
        add    byte ptr [eax], al
loc_1_2E64:
        imul   esi, dword ptr [eax + 64h], 70h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2E6C:
        imul   esi, dword ptr [eax + 65h], 6eh
        add    byte ptr fs:[eax], al
        add    byte ptr [ebx + 70h], ch
        outsb
        db  074h, 065h
        jb     SHORT loc_1_2E7C
loc_1_2E7C:
        imul   esi, dword ptr [eax + 68h], 6fh
        insd
        add    byte ptr gs:[eax], al
loc_1_2E84:
        imul   esi, dword ptr [eax + 69h], 6eh
        jae    SHORT loc_1_2E8A
loc_1_2E8A:
        add    byte ptr [eax], al
loc_1_2E8C:
        imul   esi, dword ptr [eax + 69h], 6eh
        jae    SHORT loc_1_2EF7
        jb     SHORT loc_1_2F08
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2E98:
        imul   esi, dword ptr [eax + 6ch], 65h
        je     NEAR PTR loc_1_2E9F
loc_1_2E9F:
        add    byte ptr [ebx + 70h], ch
        insd
        imul   esp, dword ptr [esp + ebp*2], 6d706b00h
        imul   ebp, dword ptr [esi + 75h], 706b0073h
        db  070h, 061h
        outsd
        db  077h, 06Eh
        add    byte ptr [eax], al
loc_1_2EBC:
        imul   esi, dword ptr [eax + 70h], 61h
        jne    NEAR PTR loc_1_2F34
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2EC8:
        imul   esi, dword ptr [eax + 70h], 67h
        outsb
        add    byte ptr [eax], al
loc_1_2ED0:
        imul   esi, dword ptr [eax + 70h], 67h
        db  075h, 070h
        add    byte ptr [eax], al
loc_1_2ED8:
        imul   esi, dword ptr [eax + 70h], 6ch
        jne    SHORT loc_1_2F51
        add    byte ptr [eax], al
loc_1_2EE0:
        imul   esi, dword ptr [eax + 72h], 69h
        db  067h, 068h, 074h, 000h
sub_1_2EE8:
        imul   esi, dword ptr [eax + 73h], 6ch
        popad
        db  073h, 068h
        add    byte ptr [ebx + 70h], ch
        db  073h, 074h
        popad
        jb     SHORT loc_1_2EF7
loc_1_2EF7:
        add    byte ptr [ebx + 70h], ch
        je     SHORT loc_1_2F65
        insd
        jae    NEAR PTR loc_1_2F00
loc_1_2F00:
        imul   esi, dword ptr [eax + 75h], 70h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F08:
        insb
        popad
        insb
        je     SHORT loc_1_2F0D
loc_1_2F0D:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6fh], ch
        outsb
        je     SHORT loc_1_2F88
        outsd
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F1C:
        insb
        arpl   word ptr [edx + esi*2 + 6ch], si
        add    byte ptr [eax], al
        add    byte ptr [ebp + 66h], ch
        je     SHORT loc_1_2F29
loc_1_2F29:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 66h], ch
        je     SHORT loc_1_2F92
        insb
        je     SHORT loc_1_2F34
loc_1_2F34:
        insb
        je     NEAR PTR loc_1_2F9C
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        add    byte ptr [ebp + 66h], ch
        je     SHORT loc_1_2FA8
        db  074h, 072h
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F4C:
        insb
        je     NEAR PTR loc_1_2FC4
loc_1_2F51:
        push   746669h
        add    byte ptr [eax], al
loc_1_2F58:
        insb
        db  073h, 068h
        imul   esp, dword ptr [esi + 74h], 756d0000h
        insb
        je     SHORT loc_1_2FCE
loc_1_2F65:
        jo     SHORT loc_1_2FD3
        jns    SHORT loc_1_2F69
loc_1_2F69:
        add    byte ptr [eax], al
        add    byte ptr [esi + 75h], ch
        insd
        insb
        imul   eax, dword ptr [eax], 0
        add    byte ptr [esi + 75h], ch
        insd
        insb
        outsd
        arpl   word ptr [ebx], bp
loc_1_2F7C:
        db  070h, 061h
        outsd
        ja     SHORT loc_1_2FF2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F88:
        db  070h, 061h
        jne    NEAR PTR loc_1_2FFE
        add    byte ptr [eax], al
loc_1_2F90:
        db  070h, 061h
loc_1_2F92:
        db  075h, 073h
        add    byte ptr gs:[eax], al
        db  000h
loc_1_2F98:
        db  070h, 065h
        db  072h, 069h
loc_1_2F9C:
        outsd
        add    byte ptr fs:[eax], al
loc_1_2FA0:
        db  070h, 067h
        outsb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2FA8:
        db  070h, 067h
        db  075h, 070h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2FB0:
        db  070h, 072h
        db  069h, 06Eh, 074h, 000h, 000h, 000h
loc_1_2FB8:
        jo     SHORT loc_1_302C
        imul   ebp, dword ptr [esi + 74h], 65726373h
        outsb
        db  000h
loc_1_2FC4:
        db  070h, 072h
        db  074h, 073h
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_2FCC:
        jb     SHORT loc_1_302F
loc_1_2FCE:
        insb
        je     SHORT loc_1_2FD1
loc_1_2FD1:
        add    byte ptr [eax], al
loc_1_2FD3:
        add    byte ptr [edx + 63h], dh
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2FE0:
        jb     SHORT loc_1_3045
        db  074h, 072h
        insb
        add    byte ptr [eax], al
        add    byte ptr [edx + 65h], dh
        je     SHORT loc_1_2FEC
loc_1_2FEC:
        db  072h, 065h
        db  074h, 075h
        jb     SHORT loc_1_3060
loc_1_2FF2:
        add    byte ptr [eax], al
loc_1_2FF4:
        db  072h, 069h
        push   74h
loc_1_2FFC:
        jb     SHORT loc_1_3067
loc_1_2FFE:
        push   746c6174h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3008:
        db  072h, 069h
        push   6e6f6374h
        db  074h, 072h
        outsd
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3018:
        jb     SHORT loc_1_3083
        push   72746374h
        insb
        add    byte ptr [eax], al
        add    byte ptr [edx + 69h], dh
        push   69687374h
loc_1_302C:
        je     NEAR PTR loc_1_302F
loc_1_302F:
        add    byte ptr [edx + 73h], dh
        push   746669h
        add    byte ptr [ebx + 63h], dh
        jb     SHORT loc_1_30AB
        insb
        insb
        add    byte ptr [eax], al
loc_1_3040:
        db  073h, 063h
        db  072h, 06Fh
        insb
loc_1_3045:
        insb
        insb
        outsd
        arpl   word ptr [ebx], bp
        add    byte ptr [ebx + 68h], dh
        imul   esp, dword ptr [esi + 74h], 73000000h
        db  070h, 061h
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
loc_1_305C:
        db  073h, 079h
        db  073h, 072h
loc_1_3060:
        jno    NEAR PTR loc_1_3063
loc_1_3063:
        add    byte ptr [ecx + 62h], dh
loc_1_3067:
        add    byte ptr [ecx + ebp*2 + 6ch], dh
        add    byte ptr gs:[eax], al
        add    byte ptr [ebp + 70h], dh
        add    byte ptr [eax], al
loc_1_3074:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3078:
        db  038h, 02Dh, 000h, 000h, 03Ch, 03Ch
        db  02Dh, 000h, 000h, 00Fh, 048h
loc_1_3083:
        db  02Dh, 000h, 000h, 00Fh, 050h
        db  02Dh, 000h, 000h, 07Eh, 058h
        db  02Dh, 000h, 000h, 01Eh, 060h
        db  02Dh, 000h, 000h, 01Eh, 06Ch
        db  02Dh, 000h, 000h, 035h, 074h
        db  02Dh, 000h, 000h, 03Ah, 07Ch
        db  02Dh, 000h, 000h, 03Ah, 084h
        db  02Dh, 000h, 000h, 068h, 088h
loc_1_30AB:
        db  02Dh, 000h, 000h, 04Ch, 090h
        db  02Dh, 000h, 000h, 05Fh, 098h
        db  02Dh, 000h, 000h, 054h, 0A0h
        db  02Dh, 000h, 000h, 051h, 0A4h
        db  02Dh, 000h, 000h, 02Bh, 0ACh
        db  02Dh, 000h, 000h, 06Eh, 0B0h
        db  02Dh, 000h, 000h, 06Eh, 0B8h
        db  02Dh, 000h, 000h, 070h, 0BCh
        db  02Dh, 000h, 000h, 079h, 0C0h
        db  02Dh, 000h, 000h, 07Ah, 0C4h
        db  02Dh, 000h, 000h, 07Bh, 0C8h
        db  02Dh, 000h, 000h, 071h, 0CCh
        db  02Dh, 000h, 000h, 072h, 0D0h
        db  02Dh, 000h, 000h, 073h, 0D4h
        db  02Dh, 000h, 000h, 074h, 0D8h
        db  02Dh, 000h, 000h, 075h, 0DCh
        db  02Dh, 000h, 000h, 076h, 0E0h
        db  02Dh, 000h, 000h, 077h, 0E4h
        db  02Dh, 000h, 000h, 078h, 0E8h
        db  02Dh, 000h, 000h, 050h, 0F0h
        db  02Dh, 000h, 000h, 063h, 0F4h
        db  02Dh, 000h, 000h, 04Bh, 0FCh
        db  02Dh, 000h, 000h, 064h, 000h
        add    byte ptr cs:[eax], al
        db  06Ah, 004h
        add    byte ptr cs:[eax], al
        db  069h, 008h, 02Eh, 000h, 000h, 05Fh
        or     al, 2eh
        add    byte ptr [eax], al
        db  063h, 010h
        add    byte ptr cs:[eax], al
        pop    ebp
        adc    al, 2eh
        add    byte ptr [eax], al
        db  062h, 018h
        add    byte ptr cs:[eax], al
        db  067h, 01Ch, 02Eh
        add    byte ptr [eax], al
        pop    esp
        and    byte ptr [esi], ch
        add    byte ptr [eax], al
        popad
        and    al, 2eh
        add    byte ptr [eax], al
        db  066h, 028h, 02Eh
        add    byte ptr [eax], al
        pop    ebx
        sub    al, 2eh
        add    byte ptr [eax], al
        pushad
        xor    byte ptr [esi], ch
        add    byte ptr [eax], al
        db  065h, 034h, 02Eh
        add    byte ptr [eax], al
        db  068h, 03Ch, 02Eh, 000h, 000h
        db  068h, 048h, 02Eh, 000h, 000h
        pop    edi
        push   esp
        add    byte ptr cs:[eax], al
        db  062h, 05Ch, 02Eh, 000h
        db  000h, 062h, 064h
        add    byte ptr cs:[eax], al
        db  068h, 06Ch, 02Eh, 000h, 000h
        pop    ebp
        db  074h, 02Eh
        add    byte ptr [eax], al
        insb
        db  07Ch, 02Eh
        add    byte ptr [eax], al
        pop    ebx
        test   byte ptr [esi], ch
        add    byte ptr [eax], al
        db  063h, 08Ch, 02Eh, 000h, 000h, 063h, 098h
        add    byte ptr cs:[eax], al
        pop    esp
        mov    al, byte ptr [6100002eh]
        test   al, 2eh
        add    byte ptr [eax], al
        db  069h, 0B0h, 02Eh, 000h, 000h, 067h, 0BCh, 02Eh, 000h, 000h
        db  065h, 0C8h, 02Eh, 000h, 000h
        db  067h, 0D0h, 02Eh, 000h, 000h
        db  065h, 0D8h, 02Eh
loc_1_31BA:
        add    byte ptr [eax], al
        db  06Ah, 0E0h
        add    byte ptr cs:[eax], al
        db  066h, 0E8h, 02Eh, 000h
        db  000h, 05Fh, 0F0h
        add    byte ptr cs:[eax], al
        db  064h, 0F8h
        add    byte ptr cs:[eax], al
        db  064h, 000h, 02Fh
        add    byte ptr [eax], al
        pushad
        or     byte ptr [edi], ch
        add    byte ptr [eax], al
        db  03Ch, 010h
        das
        add    byte ptr [eax], al
        db  03Ah, 01Ch, 02Fh
        add    byte ptr [eax], al
        db  03Ah, 024h, 02Fh
        add    byte ptr [eax], al
        dec    edi
        sub    al, 2fh
        add    byte ptr [eax], al
loc_1_31EE:
        db  03Ch, 034h
        das
        add    byte ptr [eax], al
loc_1_31F3:
        db  03Ah, 040h, 02Fh
        add    byte ptr [eax], al
        db  03Ah, 04Ch, 02Fh, 000h
        db  000h, 02Ch, 058h
loc_1_31FF:
        das
        add    byte ptr [eax], al
        db  02Ch, 060h
        das
        add    byte ptr [eax], al
        db  064h, 06Ch
        das
        add    byte ptr [eax], al
loc_1_320C:
        pop    edx
        je     SHORT loc_1_323E
        add    byte ptr [eax], al
        pop    edx
        jl     SHORT loc_1_3243
        add    byte ptr [eax], al
        push   esi
        mov    byte ptr [edi], ch
        add    byte ptr [eax], al
        push   ebp
        nop
        das
        add    byte ptr [eax], al
        db  07Eh, 098h
        das
        add    byte ptr [eax], al
        db  036h, 0A0h, 02Fh, 000h, 000h, 056h
        test   al, 2fh
        add    byte ptr [eax], al
        push   ebp
        mov    al, 2fh
        add    byte ptr [eax], al
        db  07Ch, 0B8h
        das
        add    byte ptr [eax], al
        db  07Ch, 0C4h
        das
        add    byte ptr [eax], al
loc_1_323E:
        db  07Ch, 0CCh
        das
        add    byte ptr [eax], al
loc_1_3243:
        db  03Eh, 0D4h, 02Fh
        add    byte ptr [eax], al
        inc    eax
        loopne loc_1_327A
        add    byte ptr [eax], al
        inc    eax
        db  0E8h, 02Fh, 000h, 000h, 02Bh
        in     al, dx
        das
        add    byte ptr [eax], al
        db  02Bh, 0F4h
        das
        add    byte ptr [eax], al
        pop    ecx
        cld
        das
        add    byte ptr [eax], al
        db  03Eh, 008h, 030h
        add    byte ptr [eax], al
        inc    eax
        sbb    byte ptr [eax], dh
        add    byte ptr [eax], al
        inc    eax
        and    al, 30h
        add    byte ptr [eax], al
        db  039h, 030h
        xor    byte ptr [eax], al
        add    byte ptr [ecx], bh
        cmp    byte ptr [eax], dh
        add    byte ptr [eax], al
loc_1_327A:
        db  07Dh, 040h
        xor    byte ptr [eax], al
        db  000h, 07Dh, 04Ch
        xor    byte ptr [eax], al
        db  000h, 02Ch, 054h
        xor    byte ptr [eax], al
        db  000h, 03Dh, 05Ch, 030h, 000h, 000h
        db  07Ch, 064h
        xor    byte ptr [eax], al
        add    byte ptr [eax], dl
        push   1000030h
        jo     SHORT loc_1_32CB
        add    byte ptr [eax], al
        push   ebx
        db  074h, 030h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_32A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    eax, 6000000h
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
loc_1_32CB:
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        or     eax, 0
        add    byte ptr [eax], al
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [edx], dl
        add    byte ptr [eax], al
        add    byte ptr [ebx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dl
        add    byte ptr [eax], al
        adc    eax, 16000000h
        add    byte ptr [eax], al
        add    byte ptr [edi], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        add    byte ptr [edx], bl
        add    byte ptr [eax], al
        add    byte ptr [ebx], bl
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [ebx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        and    eax, 26000000h
        add    byte ptr [eax], al
        add    byte ptr [edi], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 0
        add    byte ptr [eax], al
        sub    eax, 2e000000h
        add    byte ptr [eax], al
        add    byte ptr [edi], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [edx], dh
        add    byte ptr [eax], al
        add    byte ptr [ebx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dh
        add    byte ptr [eax], al
        xor    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [esi], dh
        add    byte ptr [eax], al
        add    byte ptr [0], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        add    byte ptr [ecx], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], bh
        loopne loc_1_339F
loc_1_339F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [0e0h], bl
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
        add    byte ptr [edx - 20h], dl
        add    byte ptr [eax], al
        push   ebx
        loopne loc_1_33D7
loc_1_33D7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx - 20h], cl
        add    byte ptr [eax], al
        inc    edi
        loopne loc_1_33E7
loc_1_33E7:
        add    byte ptr [edi - 20h], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    eax
        loopne loc_1_33F3
loc_1_33F3:
        add    byte ptr [eax - 20h], dl
        add    byte ptr [eax], al
        dec    ecx
        loopne loc_1_33FB
loc_1_33FB:
        add    byte ptr [ecx - 20h], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        loopne loc_1_340B
loc_1_340B:
        add    byte ptr [ebp], al
        add    byte ptr [eax], al
        inc    edi
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [480000e0h], dh
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [edi], dh
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        dec    ebp
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    eax, dword ptr [eax]
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    eax, 3e000000h
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [edi], dl
        add    byte ptr [eax], al
        pop    eax
        add    byte ptr [eax], al
        add    byte ptr [edi], dh
        loopne loc_1_3497
loc_1_3497:
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0A8h, 034h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0B4h, 034h, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
loc_1_34CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34F4:
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
loc_1_3544:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3548:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_354C:
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
loc_1_3560:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3564:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_3568:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_356C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3570:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3574:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3578:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_357C:
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
loc_1_35CC:
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
loc_1_35E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35EC:
        add    byte ptr [eax], al
loc_1_35EE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3600:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3604:
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
loc_1_3618:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_361C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3620:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3624:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3630:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3634:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3638:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_363C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3640:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_3644:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3648:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_364C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3650:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_3654:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3658:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_365C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3660:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3664:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3668:
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
_LTEXT ends
        end
