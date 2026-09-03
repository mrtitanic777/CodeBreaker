; dynapage_icode_02.asm -- DYNAPAGE object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        movzx  eax, dx
        shr    edx, 10h
        shl    edx, 4
        add    edx, eax
        db  089h, 015h, 0A0h, 000h, 000h, 000h
        clc
        ret
PageFile_Init_File:
        pushad
        mov    eax, 0ffffffffh
        xor    esi, esi
        db  0BFh, 06Ch, 003h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        test   eax, eax
        je     SHORT loc_2_0047
        call   sub_2_005A
        db  08Bh, 01Dh, 080h, 000h, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_2_0047
loc_2_0039:
        popad
        db  0A1h, 088h, 000h, 000h, 000h
        db  08Bh, 01Dh, 080h, 000h, 000h, 000h
        clc
        ret
loc_2_0047:
        db  0C7h, 005h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 0F4h, 000h, 000h, 000h, 001h
        jmp    SHORT loc_2_0039
sub_2_005A:
        enter  5ch, 0
        pushad
        mov    byte ptr [ebp - 54h], 0
        mov    byte ptr [ebp - 58h], 0
        mov    byte ptr [ebp - 5ch], 0
        lea    esi, [ebp - 40h]
        push   0
        push   esi
        db  0CDh, 020h, 06Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        mov    ecx, dword ptr [esi + 4]
        shl    ecx, 2
        mov    eax, 2400h
        sub    eax, ecx
        jns    SHORT loc_2_008A
        xor    eax, eax
loc_2_008A:
        mov    dword ptr [ebp - 44h], eax
        xor    esi, esi
        db  0BFh, 0ADh, 003h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        shr    eax, 2
        db  0A3h, 084h, 000h, 000h, 000h
        mov    eax, 200h
        xor    esi, esi
        db  0BFh, 09Ch, 003h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        shr    eax, 2
        db  0A3h, 090h, 000h, 000h, 000h
        mov    eax, 200000h
        xor    esi, esi
        db  0BFh, 08Ah, 003h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        shr    eax, 2
        db  0A3h, 080h, 000h, 000h, 000h
        xor    esi, esi
        db  0BFh, 073h, 003h, 000h, 000h
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        jb     SHORT loc_2_00F5
        mov    esi, edx
        db  0BFh, 000h, 000h, 000h, 000h
        cld
loc_2_00ED:
        lodsb
        stosb
        test   al, al
        jne    SHORT loc_2_00ED
        jmp    SHORT loc_2_0137
loc_2_00F5:
        xor    esi, esi
        db  0BFh, 07Eh, 003h, 000h, 000h
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        cld
        jb     SHORT loc_2_0117
        mov    bl, byte ptr [edx]
loc_2_010C:
        mov    al, bl
        stosb
        mov    ax, 5c3ah
        stosw
        jmp    SHORT loc_2_012B
loc_2_0117:
        mov    byte ptr [ebp - 54h], 1
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    esi, edx
loc_2_0123:
        lodsb
        test   al, al
        je     SHORT loc_2_012B
        stosb
        jmp    SHORT loc_2_0123
loc_2_012B:
        db  0BEh, 060h, 003h, 000h, 000h
        mov    ecx, 0bh
        rep movsb
loc_2_0137:
        db  00Fh, 0B6h, 015h, 000h, 000h, 000h, 000h
        and    dl, 0dfh
        sub    dl, 40h
        db  089h, 015h, 08Ch, 000h, 000h, 000h
        dec    edx
        lea    eax, [ebp - 48h]
        mov    byte ptr [eax + 2], dl
        mov    byte ptr [eax], 0ah
        push   eax
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        pop    eax
        cmp    byte ptr [eax + 1], 0
        jne    SHORT loc_2_018B
        test   byte ptr [eax + 3], 3
        je     SHORT loc_2_018B
        test   byte ptr [eax + 3], 2
        jne    SHORT loc_2_018B
        cmp    byte ptr [ebp - 54h], 0
        je     SHORT loc_2_018B
        cmp    byte ptr [ebp - 5ch], 0
        jne    SHORT loc_2_018B
        mov    bl, byte ptr [eax + 2]
        add    bl, 41h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 58h], 1
        jmp    SHORT loc_2_010C
loc_2_018B:
        db  0C6h, 005h, 0F4h, 000h, 000h, 000h, 002h
        db  08Bh, 015h, 08Ch, 000h, 000h, 000h
        dec    edx
        db  0CDh, 020h, 035h, 000h, 040h, 000h
        jb     NEAR PTR loc_2_0225
        test   eax, 1
        jne    SHORT loc_2_01D9
        db  08Bh, 015h, 08Ch, 000h, 000h, 000h
        add    dl, 40h
        lea    eax, [ebp - 50h]
        mov    byte ptr [eax + 2], dl
        mov    byte ptr [eax], 2
        push   eax
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        pop    eax
        cmp    byte ptr [eax + 1], 0
        jne    SHORT loc_2_01D2
        test   byte ptr [eax + 5], 40h
        jne    SHORT loc_2_01D9
loc_2_01D2:
        db  0C6h, 005h, 0F4h, 000h, 000h, 000h, 003h
loc_2_01D9:
        mov    eax, 0d500h
        mov    bx, 6092h
        xor    ecx, ecx
        mov    dl, 11h
        mov    dh, 13h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 032h, 000h, 040h, 000h
        jb     SHORT loc_2_022C
        db  0A3h, 0F0h, 000h, 000h, 000h
loc_2_01FA:
        db  08Bh, 015h, 084h, 000h, 000h, 000h
        db  0E8h, 0FBh, 0FDh, 0FDh, 0FFh
        jae    SHORT loc_2_021A
        mov    edx, dword ptr [ebp - 44h]
        shr    edx, 2
        db  089h, 015h, 084h, 000h, 000h, 000h
        db  0E8h, 0E8h, 0FDh, 0FDh, 0FFh
        jb     SHORT loc_2_021D
loc_2_021A:
        popad
        leave
        ret
loc_2_021D:
        db  0CDh, 020h, 002h, 000h, 021h, 000h
        jmp    SHORT loc_2_022C
loc_2_0225:
        call   sub_2_026A
        jae    SHORT loc_2_01FA
loc_2_022C:
        db  0C6h, 005h, 0F5h, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        cmp    byte ptr [ebp - 54h], 0
        je     NEAR PTR loc_2_0117
        mov    byte ptr [ebp - 5ch], 1
        cmp    byte ptr [ebp - 58h], 0
        mov    byte ptr [ebp - 58h], 0
        jne    NEAR PTR loc_2_0117
        db  0C7h, 005h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 44h], 0
        je     SHORT loc_2_021A
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
sub_2_026A:
        pushad
        mov    eax, 21h
        db  0CDh, 020h, 042h, 000h, 001h, 000h
        shl    ecx, 10h
        mov    cx, dx
        db  089h, 00Dh, 094h, 000h, 000h, 000h
        db  0CDh, 020h, 0B9h, 000h, 001h, 000h
        db  066h, 0A3h, 0AAh, 000h, 000h, 000h
        mov    ah, 3ch
        xor    ecx, ecx
        db  0BAh, 000h, 000h, 000h, 000h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     NEAR PTR loc_2_0334
        mov    ebx, eax
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    ah, 3dh
        mov    al, 92h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_2_0334
        db  0A3h, 0F0h, 000h, 000h, 000h
        push   408h
        push   1000h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_2_0336
        shr    eax, 0ch
        db  0A3h, 098h, 000h, 000h, 000h
        db  0E8h, 0D0h, 0FEh, 0FDh, 0FFh
        push   202h
        push   190h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_2_0336
        shl    eax, 0ch
        shr    ax, 0ch
        add    ax, 190h
        db  0A3h, 09Ch, 000h, 000h, 000h
        db  0BEh, 099h, 001h, 000h, 000h
        mov    eax, 24h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  0BEh, 0AAh, 001h, 000h, 000h
        mov    eax, 23h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  0FEh, 005h, 0F5h, 000h, 000h, 000h
        clc
loc_2_0334:
        popad
        ret
loc_2_0336:
        db  0CDh, 020h, 002h, 000h, 021h, 000h
        stc
        jmp    SHORT loc_2_0334
PageFile_Set_Async_Manager:
        db  081h, 03Dh, 05Ch, 003h, 000h, 000h, 058h, 003h, 000h, 000h
        jne    SHORT loc_2_0358
        db  0A3h, 05Ch, 003h, 000h, 000h
        clc
        ret
PageFile_Call_Async_Manager:
        db  0FFh, 025h, 05Ch, 003h, 000h, 000h
loc_2_0358:
        stc
        ret
        db  000h, 000h
loc_2_035C:
        pop    eax
        add    eax, dword ptr [eax]
        add    byte ptr [edi + 49h], dl
        dec    esi
        xor    edi, dword ptr [eax]
        push   ebx
        push   edi
        push   eax
        add    byte ptr [eax], al
loc_2_036C:
        push   eax
        popad
        imul   ebp, dword ptr [bp + 67h], 67615000h
        imul   ebp, dword ptr [esi + 67h], 656c6946h
        add    byte ptr [eax + 61h], dl
        imul   ebp, dword ptr [bp + 67h], 76697244h
        add    byte ptr gs:[ebp + 61h], cl
        db  078h, 050h
        popad
        imul   ebp, dword ptr [bp + 67h], 656c6946h
        push   ebx
        imul   edi, dword ptr [edx + 65h], 6e694d00h
        push   ebp
        db  073h, 065h
        db  072h, 044h
        imul   esi, dword ptr [ebx + 6bh], 63617053h
        add    byte ptr gs:[ebp + 69h], cl
        outsb
        push   eax
        popad
        imul   ebp, dword ptr [bp + 67h], 656c6946h
        push   ebx
        db  069h, 07Ah, 065h, 000h
_ITEXT ends
        end
