; vmouse_icode_02.asm -- VMOUSE object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        pushad
        call   sub_2_0180
        popad
        movzx  eax, word ptr [463h]
        db  0A3h, 0F8h, 007h, 000h, 000h
        mov    eax, 30018h
        db  0BEh, 0C1h, 007h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        mov    eax, 10095h
        db  0BEh, 0A0h, 005h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_2_005D
        db  0CDh, 020h, 000h, 000h, 00Dh, 000h
        jb     SHORT loc_2_0056
        cmp    eax, 20ah
        jae    SHORT loc_2_005D
        mov    eax, 30001h
        db  0BEh, 066h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_2_005D
loc_2_0056:
        db  080h, 035h, 071h, 007h, 000h, 000h, 001h
loc_2_005D:
        ret
        db  0EBh, 006h, 0FFh, 025h, 004h, 005h, 000h, 000h
loc_2_0066:
        cmp    word ptr [edi], 1
        je     SHORT loc_2_0072
        db  0FFh, 025h, 004h, 005h, 000h, 000h
loc_2_0072:
        db  0FFh, 015h, 004h, 005h, 000h, 000h
        jb     SHORT loc_2_007F
        db  0A3h, 000h, 000h, 000h, 000h
loc_2_007F:
        ret
        db  0EBh, 006h, 0FFh, 025h, 008h, 005h, 000h, 000h
loc_2_0088:
        cmp    eax, 15h
        jne    SHORT loc_2_00BA
        db  080h, 03Dh, 0C5h, 001h, 000h, 000h, 000h
        jne    SHORT loc_2_00BA
        pushad
        mov    eax, 10041h
        db  0BEh, 088h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        mov    eax, 11h
        db  0BEh, 0C0h, 000h, 000h, 000h
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        popad
        clc
        ret
loc_2_00BA:
        db  0FFh, 025h, 008h, 005h, 000h, 000h
loc_2_00C0:
        xor    eax, eax
        db  038h, 005h, 0C5h, 001h, 000h, 000h
        jne    SHORT loc_2_00D5
        db  0BEh, 0D7h, 000h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
loc_2_00D5:
        stc
        ret
loc_2_00D7:
        and    byte ptr [ebp + 1ch], 0fbh
        ret
loc_2_00DC:
        db  089h, 01Dh, 090h, 006h, 000h, 000h
        db  083h, 03Dh, 004h, 005h, 000h, 000h, 000h
        je     SHORT loc_2_011D
        mov    eax, 30001h
        db  0BEh, 066h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        mov    eax, 11h
        db  0BEh, 0C0h, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, 10041h
        db  0BEh, 088h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_2_016C
loc_2_011D:
        push   0
        push   0a9h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_2_0136
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_0136:
        db  0A3h, 08Ch, 006h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 017h, 000h
        jb     SHORT loc_2_0150
        cmp    eax, 314h
        jb     SHORT loc_2_0150
        db  0FEh, 005h, 098h, 006h, 000h, 000h
loc_2_0150:
        call   sub_2_0407
        db  080h, 00Dh, 006h, 000h, 000h, 000h, 001h
        push   0
        db  068h, 03Ch, 006h, 000h, 000h
        db  0CDh, 020h, 032h, 000h, 033h, 000h
        pop    ecx
        pop    ecx
        clc
loc_2_016C:
        ret
loc_2_016D:
        push   ebx
        call   sub_2_020C
        pop    ebx
        db  0E8h, 07Ah, 005h, 0FEh, 0FFh
        call   sub_2_0387
        clc
        ret
sub_2_0180:
        mov    eax, 33h
        db  0CDh, 020h, 042h, 000h, 001h, 000h
        shl    ecx, 10h
        mov    cx, dx
        db  089h, 00Dh, 04Ch, 007h, 000h, 000h
        push   ecx
        push   0
        push   6
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        or     eax, eax
        jne    SHORT loc_2_01AF
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_01AF:
        inc    eax
        mov    edi, eax
        mov    dword ptr [eax + 1], 8ca90h
        shr    eax, 4
        movzx  ecx, ax
        mov    edx, edi
        and    edx, 0fh
        mov    eax, 33h
        db  0CDh, 020h, 043h, 000h, 001h, 000h
        mov    eax, ecx
        shl    eax, 10h
        add    eax, edx
        xor    edx, edx
        db  0BEh, 0ECh, 000h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        db  0BEh, 0ECh, 000h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jae    SHORT loc_2_01F6
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_01F6:
        mov    ecx, eax
        shr    ecx, 10h
        movzx  edx, ax
        mov    eax, 33h
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        clc
        ret
        db  000h
sub_2_020C:
        pushad
        db  0BEh, 062h, 005h, 000h, 000h
        push   esi
        push   0
        db  068h, 066h, 005h, 000h, 000h
        push   2
        db  0CDh, 020h, 06Fh, 001h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_2_0385
        db  0C7h, 005h, 06Eh, 005h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  0BBh, 06Ah, 005h, 000h, 000h
        db  0BFh, 073h, 005h, 000h, 000h
        db  068h, 06Eh, 005h, 000h, 000h
        push   edi
        push   ebx
        push   0
        db  068h, 00Ch, 005h, 000h, 000h
        push   dword ptr [esi]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_2_026C
        push   esi
        push   ebx
        db  0BFh, 073h, 005h, 000h, 000h
        db  0E8h, 059h, 00Dh, 0FEh, 0FFh
        pop    ebx
        pop    esi
loc_2_026C:
        db  0C7h, 005h, 06Eh, 005h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 06Eh, 005h, 000h, 000h
        db  068h, 0A4h, 000h, 000h, 000h
        push   ebx
        push   0
        db  068h, 016h, 005h, 000h, 000h
        push   dword ptr [esi]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_2_02A7
        db  080h, 03Dh, 0A4h, 000h, 000h, 000h, 064h
        jbe    SHORT loc_2_02A7
        db  0C6h, 005h, 0A4h, 000h, 000h, 000h, 064h
loc_2_02A7:
        db  0C7h, 005h, 06Eh, 005h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 06Eh, 005h, 000h, 000h
        db  068h, 0A5h, 000h, 000h, 000h
        push   ebx
        push   0
        db  068h, 02Ch, 005h, 000h, 000h
        push   dword ptr [esi]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_2_02E2
        db  080h, 03Dh, 0A5h, 000h, 000h, 000h, 064h
        jbe    SHORT loc_2_02E2
        db  0C6h, 005h, 0A5h, 000h, 000h, 000h, 064h
loc_2_02E2:
        db  0C7h, 005h, 06Eh, 005h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 06Eh, 005h, 000h, 000h
        db  068h, 072h, 005h, 000h, 000h
        push   ebx
        push   0
        db  068h, 040h, 005h, 000h, 000h
        push   dword ptr [esi]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_2_0320
        db  00Fh, 0B6h, 005h, 072h, 005h, 000h, 000h
        dec    eax
        db  08Ah, 080h, 05Eh, 005h, 000h, 000h
        db  0A2h, 0A6h, 000h, 000h, 000h
loc_2_0320:
        db  0C7h, 005h, 06Eh, 005h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 06Eh, 005h, 000h, 000h
        db  068h, 0A7h, 000h, 000h, 000h
        push   ebx
        push   0
        db  068h, 04Eh, 005h, 000h, 000h
        push   dword ptr [esi]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_2_035E
        db  00Fh, 0B6h, 005h, 072h, 005h, 000h, 000h
        dec    eax
        db  08Ah, 080h, 05Eh, 005h, 000h, 000h
        db  0A2h, 0A7h, 000h, 000h, 000h
loc_2_035E:
        db  0C7h, 005h, 06Eh, 005h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 06Eh, 005h, 000h, 000h
        db  068h, 098h, 007h, 000h, 000h
        push   ebx
        push   0
        db  068h, 084h, 005h, 000h, 000h
        push   dword ptr [esi]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
loc_2_0385:
        popad
        ret
sub_2_0387:
        db  0BEh, 062h, 005h, 000h, 000h
        push   esi
        push   1
        db  068h, 066h, 005h, 000h, 000h
        push   2
        db  0CDh, 020h, 06Fh, 001h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_2_03E5
        db  00Fh, 0B6h, 005h, 094h, 006h, 000h, 000h
        dec    eax
        jb     SHORT loc_2_03E5
        cmp    eax, 3
        ja     SHORT loc_2_03E5
        db  00Fh, 0B6h, 0B8h, 07Eh, 005h, 000h, 000h
        db  081h, 0C7h, 0A9h, 000h, 000h, 000h
        movzx  eax, byte ptr [edi]
        inc    edi
        movzx  ebx, byte ptr [eax + edi]
        mov    byte ptr [eax + edi], bh
        push   eax
        push   eax
        push   edi
        push   1
        push   0
        db  068h, 00Ch, 005h, 000h, 000h
        push   dword ptr [esi]
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        pop    eax
        mov    byte ptr [eax + edi], bl
loc_2_03E5:
        ret
        db  000h, 000h
loc_2_03E8:
        or     ah, ah
        je     SHORT loc_2_03F9
        cmp    ah, 11h
        db  075h, 011h
        cmp    al, 3
        db  074h, 00Dh
        cmp    al, 30h
        db  074h, 009h
loc_2_03F9:
        nop
        pushfd
        lcall  0cf90h, 0
        db  0EAh, 000h, 000h, 000h, 000h
sub_2_0407:
        mov    eax, 10h
        db  0CDh, 020h, 042h, 000h, 001h, 000h
        shl    ecx, 10h
        mov    cx, dx
        db  089h, 00Dh, 0CCh, 007h, 000h, 000h
        push   ecx
        push   0
        push   1fh
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        or     eax, eax
        jne    SHORT loc_2_0439
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_0439:
        mov    edi, eax
        db  0BEh, 0E8h, 003h, 000h, 000h
        mov    ecx, 1fh
        cld
        rep movsb
        mov    edi, eax
        mov    dword ptr [edi + 14h], edx
        mov    dword ptr [edi + 1bh], edx
        shr    eax, 4
        movzx  ecx, ax
        mov    edx, edi
        and    edx, 0fh
        mov    eax, 10h
        db  0CDh, 020h, 043h, 000h, 001h, 000h
        mov    eax, ecx
        shl    eax, 10h
        add    eax, edx
        db  0A3h, 0D0h, 007h, 000h, 000h
        add    eax, 11h
        xor    edx, edx
        db  0BEh, 00Dh, 012h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        add    eax, 7
        db  0BEh, 033h, 012h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        mov    eax, 10h
        db  0BEh, 0A2h, 011h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        call   sub_2_04AC
        ret
        db  000h, 000h
sub_2_04AC:
        push   0
        push   0f8h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_2_04FB
        db  0A3h, 0D8h, 007h, 000h, 000h
        db  0B8h, 0FCh, 007h, 000h, 000h
        db  0BEh, 045h, 008h, 000h, 000h
        mov    ecx, 0fh
        db  0CDh, 020h, 004h, 000h, 00Ch, 000h
        db  0CDh, 020h, 000h, 000h, 00Ah, 000h
        mov    al, 20h
        cmp    dword ptr [esi + 4], 4147454fh
        jne    SHORT loc_2_04F5
        test   byte ptr [487h], 60h
        jne    SHORT loc_2_04F5
        shr    al, 1
loc_2_04F5:
        db  0A2h, 040h, 008h, 000h, 000h
        ret
loc_2_04FB:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 6fh], cl
        jne    SHORT loc_2_0583
        push   esp
        db  079h, 070h
        add    byte ptr gs:[eax + 6fh], cl
        jb     SHORT loc_2_0583
        db  07Ah, 06Fh
        outsb
        db  074h, 061h
        insb
        push   ebx
        outsb
        jae    SHORT loc_2_058E
        je     SHORT loc_2_0590
        jbe    SHORT loc_2_0592
        db  074h, 079h
        add    byte ptr [esi + 65h], dl
        db  072h, 074h
        imul   esp, dword ptr [ebx + 61h], 6e65536ch
        db  073h, 069h
        db  074h, 069h
        db  076h, 069h
        db  074h, 079h
        add    byte ptr [eax + 72h], dl
        imul   ebp, dword ptr [ebp + 61h], 75427972h
        db  074h, 074h
        outsd
        outsb
        add    byte ptr [ebx + 65h], dl
        arpl   word ptr [edi + 6eh], bp
        popad
        db  072h, 079h
        inc    edx
        db  075h, 074h
        db  074h, 06Fh
        outsb
        add    byte ptr [eax], ah
        or     byte ptr [eax], dl
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [esi + 4dh], dl
        inc    esp
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0573:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0bh
        adc    al, byte ptr [eax]
loc_2_0583:
        add    byte ptr [ecx + 4eh], cl
        dec    ecx
        push   esp
        push   eax
        push   ebx
        xor    cl, byte ptr [ebp + 4fh]
        push   ebp
loc_2_058E:
        push   ebx
        inc    ebp
loc_2_0590:
        inc    ecx
        push   esp
loc_2_0592:
        inc    ebp
        pop    eax
        dec    ecx
        push   esp
        add    byte ptr [eax], al
_ITEXT ends
        end
