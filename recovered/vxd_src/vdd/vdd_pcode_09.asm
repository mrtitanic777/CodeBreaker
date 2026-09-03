; vdd_pcode_09.asm -- VDD object 9 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_9_0000:
        clc
        ret
loc_9_0002:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_9_0094
        ret
sub_9_0010:
        pushad
        db  00Fh, 0BAh, 02Dh, 0C4h, 006h, 000h, 000h, 006h
        jb     SHORT loc_9_0078
        db  08Bh, 01Dh, 0A8h, 007h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        jne    SHORT loc_9_003C
        mov    eax, 3
        db  0E8h, 0E3h, 002h, 0F7h, 0FFh
        db  0E8h, 033h, 000h, 0F7h, 0FFh
loc_9_003C:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi + 2], 40h
        je     SHORT loc_9_0054
        test   byte ptr [edi], 80h
        jne    SHORT loc_9_0054
        db  0E8h, 03Ah, 049h, 0F7h, 0FFh
loc_9_0054:
        db  08Bh, 01Dh, 0B8h, 007h, 000h, 000h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  0E8h, 06Eh, 012h, 0F7h, 0FFh
        call   sub_9_0574
        call   sub_9_05A0
loc_9_0071:
        db  0E8h, 028h, 011h, 0F7h, 0FFh
loc_9_0076:
        popad
        ret
loc_9_0078:
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_9_0076
        db  08Bh, 01Dh, 0B8h, 007h, 000h, 000h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        jmp    SHORT loc_9_0071
        db  000h, 000h, 000h
sub_9_0094:
        db  08Bh, 00Dh, 0ACh, 00Eh, 000h, 000h
        jecxz  loc_9_00AD
        movzx  edx, cx
        shr    ecx, 10h
        mov    eax, 10h
        db  0CDh, 020h, 043h, 000h, 001h, 000h
loc_9_00AD:
        db  0E8h, 046h, 005h, 0F7h, 0FFh
        ret
        db  000h
sub_9_00B4:
        pushad
        db  089h, 015h, 0CCh, 00Eh, 000h, 000h
        movzx  ecx, ch
        db  066h, 089h, 00Dh, 0D0h, 00Eh, 000h, 000h
        popad
        ret
sub_9_00C7:
        ret
sub_9_00C8:
        ret
        db  000h, 000h, 000h
VDD_Get_Version:
        mov    ax, 400h
        db  0BEh, 0D6h, 000h, 000h, 000h
        ret
loc_9_00D6:
        push   esi
        dec    ecx
        inc    esp
        inc    ebp
        dec    edi
        push   esi
        inc    edi
        inc    ecx
VDD_Set_Sleep_Flag_Addr:
        db  0A3h, 0F4h, 00Eh, 000h, 000h
        ret
loc_9_00E4:
        push   0ffh
        push   1
        db  0E8h, 03Bh, 00Bh, 0F7h, 0FFh
        pop    ecx
        pop    ecx
        db  089h, 01Dh, 0E0h, 00Eh, 000h, 000h
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_9_0107
        call   dword ptr [edi + 0ch]
loc_9_0107:
        pop    edi
        clc
        ret
loc_9_010A:
        db  03Bh, 01Dh, 0E0h, 00Eh, 000h, 000h
        jne    SHORT loc_9_0147
        db  080h, 00Dh, 0C4h, 006h, 000h, 000h, 004h
        je     SHORT loc_9_0123
        xor    eax, eax
        db  0CDh, 020h, 007h, 000h, 00Ah, 000h
loc_9_0123:
        db  080h, 025h, 0C4h, 006h, 000h, 000h, 0BFh
        db  0C7h, 005h, 0E0h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 0A4h, 007h, 000h, 000h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  0E8h, 002h, 0FFh, 0F6h, 0FFh
loc_9_0147:
        clc
        ret
VDD_Msg_ClrScrn:
        pushad
        and    al, 7
        shl    eax, 4
        or     al, 7
        db  0A2h, 0EDh, 00Eh, 000h, 000h
        db  0A2h, 0ECh, 00Eh, 000h, 000h
        db  0E8h, 0A2h, 016h, 0F7h, 0FFh
        call   sub_9_0010
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_9_0174
        call   dword ptr [edi + 0ch]
loc_9_0174:
        pop    edi
        db  08Bh, 03Dh, 0C8h, 00Eh, 000h, 000h
        add    edi, 18000h
        mov    al, 20h
        db  08Ah, 025h, 0ECh, 00Eh, 000h, 000h
        db  08Bh, 00Dh, 0E4h, 00Eh, 000h, 000h
        db  08Bh, 015h, 0E8h, 00Eh, 000h, 000h
        imul   ecx, edx
        rep stosw
        db  0E8h, 014h, 016h, 0F7h, 0FFh
        popad
        db  0A1h, 0E8h, 00Eh, 000h, 000h
        db  08Bh, 015h, 0E4h, 00Eh, 000h, 000h
        ret
VDD_Msg_ForColor:
        push   eax
        and    al, 0fh
        db  08Ah, 025h, 0EDh, 00Eh, 000h, 000h
        and    ah, 0f0h
        or     al, ah
        db  0A2h, 0EDh, 00Eh, 000h, 000h
        pop    eax
        ret
VDD_Msg_BakColor:
        push   eax
        and    al, 7
        shl    eax, 4
        db  08Ah, 025h, 0EDh, 00Eh, 000h, 000h
        and    ah, 0fh
        or     al, ah
        db  0A2h, 0EDh, 00Eh, 000h, 000h
        pop    eax
        ret
VDD_Msg_TextOut:
        jecxz  loc_9_021C
        pushad
        call   sub_9_0010
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_9_01F1
        call   dword ptr [edi + 0ch]
loc_9_01F1:
        pop    edi
        mov    edx, 0a0h
        mul    dl
        mov    edx, dword ptr [esp + 14h]
        add    eax, edx
        add    eax, edx
        db  08Bh, 03Dh, 0C8h, 00Eh, 000h, 000h
        add    edi, 18000h
        add    edi, eax
        db  08Ah, 025h, 0EDh, 00Eh, 000h, 000h
        cld
loc_9_0216:
        lodsb
        stosw
        loop   loc_9_0216
        popad
loc_9_021C:
        ret
VDD_Msg_SetCursPos:
        push   eax
        push   edx
        call   sub_9_0010
        db  03Bh, 005h, 0E4h, 00Eh, 000h, 000h
        jae    SHORT loc_9_024F
        push   edx
        db  08Bh, 015h, 0E8h, 00Eh, 000h, 000h
        mul    dl
        pop    edx
        add    eax, edx
loc_9_0238:
        db  0E8h, 02Eh, 012h, 0F7h, 0FFh
        push   eax
        mov    al, 0fh
        out    dx, al
        pop    eax
        inc    edx
        out    dx, al
        dec    edx
        mov    al, 0eh
        out    dx, al
        inc    edx
        mov    al, ah
        out    dx, al
        pop    edx
        pop    eax
        ret
loc_9_024F:
        or     eax, 0ffffffffh
        jmp    SHORT loc_9_0238
VDD_Set_HCurTrk:
        push   ebx
        db  003h, 01Dh, 0CCh, 006h, 000h, 000h
        or     byte ptr [ebx + 1], 2
        pop    ebx
        ret
VDD_Get_GrabRtn:
        db  0BEh, 067h, 002h, 000h, 000h
        ret
loc_9_0267:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  0E8h, 0F5h, 016h, 0F7h, 0FFh
        db  0E8h, 02Fh, 0FEh, 0F6h, 0FFh
        jb     SHORT loc_9_0285
        db  0E8h, 011h, 01Ch, 0F7h, 0FFh
        db  0E8h, 063h, 045h, 0F7h, 0FFh
loc_9_0285:
        ret
VDD_Register_Extra_Screen_Selector:
        push   eax
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        pop    esi
        db  0BFh, 00Ch, 00Fh, 000h, 000h
        mov    ecx, 8
loc_9_029F:
        cmp    esi, dword ptr [edi]
        je     SHORT loc_9_02FD
        add    edi, 14h
        dec    ecx
        jne    SHORT loc_9_029F
        db  0BFh, 00Ch, 00Fh, 000h, 000h
        mov    ecx, 8
loc_9_02B3:
        cmp    dword ptr [edi], 0
        je     SHORT loc_9_02C1
        add    edi, 14h
        dec    ecx
        jne    SHORT loc_9_02B3
        stc
        jmp    SHORT loc_9_02FE
loc_9_02C1:
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], edx
        xor    ax, ax
        and    edx, 0fff0ffffh
        and    edx, 0ff7fffffh
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [edi + 10h], edx
        db  083h, 03Dh, 0ACh, 00Fh, 000h, 000h, 000h
        jne    SHORT loc_9_02FD
        mov    eax, 0dh
        db  0BEh, 078h, 030h, 000h, 000h
        db  0CDh, 020h, 080h, 000h, 001h, 000h
        db  089h, 035h, 0ACh, 00Fh, 000h, 000h
loc_9_02FD:
        clc
loc_9_02FE:
        ret
loc_9_02FF:
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1
        jne    NEAR PTR loc_9_03C9
        push   3
        push   eax
        db  0E8h, 018h, 009h, 0F7h, 0FFh
        pop    ecx
        pop    ecx
        db  080h, 025h, 0A0h, 007h, 000h, 000h, 0FBh
        push   ebx
        push   edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        push   ebx
loc_9_0325:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  0E8h, 08Ah, 048h, 0F7h, 0FFh
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_9_0325
        pop    ebx
        pop    edi
        pop    ebx
        db  0C6h, 005h, 0B0h, 00Fh, 000h, 000h, 0FFh
        db  0CDh, 020h, 000h, 000h, 01Fh, 001h
        jae    SHORT loc_9_0356
        db  0C6h, 005h, 0B0h, 00Fh, 000h, 000h, 000h
loc_9_0356:
        mov    ax, word ptr [ebp]
        db  066h, 0A3h, 000h, 00Fh, 000h, 000h
        mov    ax, word ptr [ebp + 38h]
        db  066h, 0A3h, 002h, 00Fh, 000h, 000h
        db  066h, 0C7h, 005h, 0D0h, 00Eh, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0CCh, 00Eh, 000h, 000h, 020h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp + 14h], -1
        jne    SHORT loc_9_0392
loc_9_0386:
        db  0C7h, 005h, 0D2h, 00Eh, 000h, 000h, 020h, 000h, 000h, 000h
        jmp    SHORT loc_9_03C1
loc_9_0392:
        mov    ecx, edx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 4], 0
        je     SHORT loc_9_03A3
        call   dword ptr [edi + 4]
loc_9_03A3:
        pop    edi
        cmp    edx, dword ptr [ebp + 18h]
        je     SHORT loc_9_0386
        db  0A1h, 0CCh, 00Eh, 000h, 000h
        add    al, 2
        cmp    edx, 8000h
        jbe    SHORT loc_9_03BA
        add    al, 2
loc_9_03BA:
        db  0A3h, 0D2h, 00Eh, 000h, 000h
        add    edx, ecx
loc_9_03C1:
        and    dword ptr [ebp + 2ch], 0fffffffeh
        mov    dword ptr [ebp + 1ch], edx
        clc
loc_9_03C9:
        ret
loc_9_03CA:
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1
        jne    SHORT loc_9_0432
        db  080h, 00Dh, 0A0h, 007h, 000h, 000h, 004h
        db  0E8h, 016h, 002h, 0F7h, 0FFh
        push   ebx
        push   edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        push   ebx
loc_9_03E7:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        mov    dword ptr [edi + 4a7h], 0
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_9_03E7
        pop    ebx
        pop    edi
        pop    ebx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 68h], 0
        je     SHORT loc_9_0416
        call   dword ptr [edi + 68h]
loc_9_0416:
        pop    edi
        push   0ffh
        push   1
        db  0E8h, 008h, 008h, 0F7h, 0FFh
        pop    ecx
        pop    ecx
        mov    eax, 3
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_9_0432:
        ret
loc_9_0433:
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1
        jne    SHORT loc_9_0487
        push   3
        push   eax
        db  0E8h, 0E8h, 007h, 0F7h, 0FFh
        pop    ecx
        pop    ecx
        db  0FFh, 035h, 0ACh, 007h, 000h, 000h
        db  0FFh, 035h, 0A8h, 007h, 000h, 000h
        db  08Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        db  089h, 01Dh, 0ACh, 007h, 000h, 000h
        db  089h, 01Dh, 0A8h, 007h, 000h, 000h
        db  0A0h, 0A0h, 007h, 000h, 000h
        push   eax
        db  0C6h, 005h, 0A0h, 007h, 000h, 000h, 000h
        db  0E8h, 037h, 01Ah, 0F7h, 0FFh
        pop    eax
        db  0A2h, 0A0h, 007h, 000h, 000h
        db  08Fh, 005h, 0A8h, 007h, 000h, 000h
        db  08Fh, 005h, 0ACh, 007h, 000h, 000h
loc_9_0487:
        ret
loc_9_0488:
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi], 0
        je     SHORT loc_9_0495
        call   dword ptr [edi]
loc_9_0495:
        pop    edi
        ret
loc_9_0497:
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 008h, 00Fh, 000h, 000h
        ret
loc_9_04A7:
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 70h], 0
        je     SHORT loc_9_04B6
        call   dword ptr [edi + 70h]
loc_9_04B6:
        pop    edi
        ret
loc_9_04B8:
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 74h], 0
        je     SHORT loc_9_04C7
        call   dword ptr [edi + 74h]
loc_9_04C7:
        pop    edi
        ret
loc_9_04C9:
        db  0C7h, 005h, 0F8h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h
        mov    ax, 1418h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     SHORT loc_9_04E9
        db  0A3h, 0F8h, 00Eh, 000h, 000h
        xor    eax, eax
loc_9_04E9:
        mov    word ptr [ebp + 1ch], ax
        ret
sub_9_04EE:
        xor    edi, edi
        xor    esi, esi
        mov    eax, 0c0001030h
        db  00Fh, 0B7h, 00Dh, 012h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 080h, 017h, 000h
sub_9_050A:
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        mov    dl, 16h
        jmp    SHORT loc_9_0547
sub_9_051B:
        test   byte ptr [edi + 4], 40h
        jne    SHORT loc_9_0537
        test   byte ptr [edi], 1
        je     SHORT loc_9_0537
        db  00Fh, 0B7h, 00Dh, 010h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        mov    dl, 12h
        jmp    SHORT loc_9_0547
loc_9_0537:
        ret
sub_9_0538:
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        mov    dl, 10h
loc_9_0547:
        mov    eax, 80000030h
        movzx  edx, dl
        bts    dword ptr [edi + 8], edx
        jb     SHORT loc_9_0570
        push   edi
        push   eax
        xor    edi, edi
        xor    esi, esi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
        pop    eax
        jae    SHORT loc_9_056F
        or     eax, 1000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
loc_9_056F:
        pop    edi
loc_9_0570:
        ret
        db  000h, 000h, 000h
sub_9_0574:
        db  08Bh, 01Dh, 0B8h, 007h, 000h, 000h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  0E8h, 098h, 034h, 0F7h, 0FFh
        xor    edx, edx
        db  0BEh, 0DAh, 03Eh, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 099h, 00Eh, 000h, 000h
loc_9_0595:
        mov    eax, edx
        db  0E8h, 074h, 025h, 0F7h, 0FFh
        inc    edx
        loop   loc_9_0595
        ret
sub_9_05A0:
        db  0E8h, 015h, 013h, 0F7h, 0FFh
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_9_05B4
        call   dword ptr [edi + 0ch]
loc_9_05B4:
        pop    edi
        mov    dx, 402h
        db  0E8h, 066h, 03Dh, 0F7h, 0FFh
        push   edi
        db  08Bh, 03Dh, 0C8h, 00Eh, 000h, 000h
        call   sub_9_05CD
        pop    edi
        clc
        ret
sub_9_05CD:
        push   edx
        db  08Ah, 015h, 0D0h, 00Fh, 000h, 000h
        mov    dh, 20h
        sub    dh, dl
        mov    ecx, 100h
        db  08Bh, 035h, 0CCh, 00Fh, 000h, 000h
        cld
        xor    eax, eax
loc_9_05E6:
        push   ecx
        movzx  ecx, dl
        rep movsb
        movzx  ecx, dh
        rep stosb
        pop    ecx
        loop   loc_9_05E6
        pop    edx
        ret
        db  000h, 000h
sub_9_05F8:
        test   byte ptr [edi + 4], 4
        jne    SHORT loc_9_060D
        test   byte ptr [edi + 96h], 1
        jne    SHORT loc_9_063B
        test   byte ptr [edi + 4], 8
        je     SHORT loc_9_063B
loc_9_060D:
        bts    dword ptr [edi], 17h
        jb     SHORT loc_9_063B
        push   edx
        xor    edx, edx
        xchg   dword ptr [edi + 4a7h], edx
        push   edx
        mov    edx, 3b4h
        db  0E8h, 055h, 046h, 0F7h, 0FFh
        inc    edx
        db  0E8h, 04Fh, 046h, 0F7h, 0FFh
        mov    dl, 0bah
        db  0E8h, 048h, 046h, 0F7h, 0FFh
        pop    dword ptr [edi + 4a7h]
        pop    edx
loc_9_063B:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_9_067C:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0680:
        add    byte ptr [eax], al
loc_9_0682:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0690:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0694:
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
loc_9_06B4:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_06B8:
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
loc_9_06D8:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_06E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_06F0:
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_06F4:
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
loc_9_0714:
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
loc_9_0734:
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
loc_9_0754:
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
loc_9_0774:
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
loc_9_0794:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0798:
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
loc_9_07B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_07BC:
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
loc_9_07DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_07E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_07E4:
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
loc_9_0860:
        add    byte ptr [eax], 0
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
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 064h, 069h, 073h, 070h, 06Ch
        db  061h, 079h, 05Ch, 073h, 065h, 074h, 074h, 069h, 06Eh, 067h, 073h, 000h
loc_9_0915:
        inc    edx
        imul   esi, dword ptr [ebx + esi*2 + 50h], 69507265h
        js     SHORT loc_9_0985
        insb
        db  000h
loc_9_0922:
        push   edx
        db  065h, 073h, 06Fh
        insb
        jne    SHORT loc_9_099D
        db  069h, 06Fh, 06Eh, 000h
loc_9_092D:
        inc    esp
        push   eax
        dec    ecx
        dec    esp
        outsd
        db  067h, 069h, 063h, 061h, 06Ch, 058h, 000h
loc_9_0939:
        inc    ebp
        db  078h, 074h
        dec    ebp
        outsd
        push   ebx
        ja     SHORT loc_9_09AC
        db  074h, 063h
        push   73654400h
        imul   esi, dword ptr [edi + ebp*2 + 70h], 50h
        outsd
        jae    SHORT loc_9_0952
loc_9_0952:
        inc    esp
        db  065h, 073h, 06Bh
        db  074h, 06Fh
        jo     SHORT loc_9_09AD
        imul   edi, dword ptr [edx + 65h], 73694400h
        db  070h, 06Ch
        popad
        jns    SHORT loc_9_09AC
        insb
        popad
        jae    NEAR PTR loc_9_096B
loc_9_096B:
        push   ebx
        arpl   word ptr [edx + 65h], si
        outsb
        push   edx
        jae    NEAR PTR loc_9_09E4
        insb
        jne    SHORT loc_9_09EC
        db  069h, 06Fh, 06Eh, 000h
loc_9_097C:
        inc    esp
        push   edx
        push   esi
        add    byte ptr [edx + edx*2 + 56h], al
        xor    al, byte ptr [eax]
loc_9_0985:
        dec    ebp
        dec    ecx
        dec    esi
        dec    ecx
        push   esi
        inc    esp
        inc    esp
        add    byte ptr [ebx + 41h], al
        push   edx
        inc    esp
        push   esi
        inc    esp
        inc    esp
        add    byte ptr [ebx + 48h], al
        dec    ecx
        push   eax
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
loc_9_099D:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 4fh], cl
        inc    esp
        inc    ebp
        push   ebx
        pop    esp
        add    byte ptr [ebp + eax*2 + 46h], al
        inc    ecx
        push   ebp
loc_9_09AC:
        dec    esp
loc_9_09AD:
        push   esp
        add    byte ptr [31h], ch
        add    byte ptr [eax], al
loc_9_09B6:
        dec    ebp
        dec    edi
        inc    esp
        inc    ebp
        add    byte ptr [ebp + 4fh], cl
        inc    esp
        inc    ebp
        push   ebx
        add    byte ptr [eax], al
loc_9_09C2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_09C6:
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
loc_9_09E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_09EC:
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
loc_9_0AC6:
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
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_9_0B0E:
        dec    esi
        dec    edi
        dec    esi
        inc    ebp
        db  000h
loc_9_0B13:
        xor    al, 30h
        sub    al, 34h
        cmp    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0B1E:
        add    byte ptr [eax], al
loc_9_0B20:
        add    byte ptr [eax], al
loc_9_0B22:
        inc    esp
        imul   esi, dword ptr [ebx + 70h], 79616ch
loc_9_0B2A:
        dec    ecx
        outsb
        outsd
        db  072h, 065h
        push   edx
        db  065h, 067h, 069h, 073h, 074h, 072h, 079h, 000h
loc_9_0B39:
        push   edx
        db  065h, 066h, 072h, 065h
        db  073h, 068h
        push   edx
        popad
        db  074h, 065h
        add    byte ptr [edx + 65h], dl
        db  066h, 072h, 065h
        db  073h, 068h
        push   edx
        popad
        db  074h, 065h
        inc    esi
        db  075h, 064h
        add    byte ptr gs:[bx + si], al
        add    byte ptr [eax], al
        db  000h
sub_9_0B5A:
        push   ebp
        mov    ebp, esp
        lea    eax, [ebp + 8]
        push   0
        push   dword ptr [ebp + 8]
        push   eax
        db  0CDh, 020h, 004h, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        mov    eax, 0
        jne    SHORT loc_9_0B7B
        mov    eax, dword ptr [ebp + 8]
loc_9_0B7B:
        leave
        ret
sub_9_0B7D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cld
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 040h, 000h
        jne    NEAR PTR loc_9_0D8D
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 010h, 000h
        jne    NEAR PTR loc_9_0D8D
        or     ebx, ebx
        je     NEAR PTR loc_9_0D8D
        db  066h, 081h, 00Dh, 01Bh, 00Bh, 000h, 000h, 0B0h, 000h
        db  089h, 01Dh, 05Ch, 006h, 000h, 000h
        xor    eax, eax
        db  0BEh, 022h, 00Bh, 000h, 000h
        db  0BFh, 02Ah, 00Bh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 01Dh, 00Bh, 000h, 000h
        db  066h, 083h, 025h, 01Bh, 00Bh, 000h, 000h, 0FBh
        db  066h, 083h, 025h, 01Bh, 00Bh, 000h, 000h, 0F7h
        db  066h, 083h, 00Dh, 01Bh, 00Bh, 000h, 000h, 004h
        db  080h, 03Dh, 01Dh, 00Bh, 000h, 000h, 000h
        jne    NEAR PTR loc_9_0D8D
        call   sub_9_0D97
        jb     NEAR PTR loc_9_0D8D
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 002h, 000h
        je     NEAR PTR loc_9_0D4C
        db  0BFh, 098h, 007h, 000h, 000h
        db  0BEh, 00Eh, 00Bh, 000h, 000h
        call   sub_9_1CDB
        je     NEAR PTR loc_9_0D4C
        db  0B8h, 098h, 007h, 000h, 000h
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   2
        push   eax
        push   ebx
        db  0CDh, 020h, 02Fh, 000h, 033h, 000h
        add    esp, 10h
        db  066h, 083h, 025h, 01Eh, 00Bh, 000h, 000h, 0FDh
        cmp    eax, 0
        je     SHORT loc_9_0C66
        db  066h, 083h, 00Dh, 01Eh, 00Bh, 000h, 000h, 002h
        db  066h, 081h, 025h, 01Bh, 00Bh, 000h, 000h, 07Fh, 0FFh
        mov    eax, 24h
        jmp    NEAR PTR loc_9_0D92
loc_9_0C66:
        xor    eax, eax
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0a8h], 0
        je     SHORT loc_9_0C7D
        call   dword ptr [edi + 0a8h]
loc_9_0C7D:
        pop    edi
        push   eax
        db  0B8h, 0A7h, 009h, 000h, 000h
        mov    ebx, dword ptr [ebp + 8]
        db  0B9h, 095h, 009h, 000h, 000h
        db  0BAh, 0E0h, 007h, 000h, 000h
        db  0C7h, 005h, 09Ch, 009h, 000h, 000h, 006h, 000h, 000h, 000h
        db  0BEh, 09Ch, 009h, 000h, 000h
        push   1
        push   esi
        push   edx
        push   3
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        pop    ecx
        cmp    eax, 0
        jne    SHORT loc_9_0D07
        db  066h, 083h, 025h, 01Eh, 00Bh, 000h, 000h, 0FBh
        db  03Bh, 00Dh, 0E0h, 007h, 000h, 000h
        je     NEAR PTR loc_9_0D4C
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        db  0BAh, 0E0h, 007h, 000h, 000h
        lea    ecx, [edx + 4]
        mov    dword ptr [ecx], 4
        push   0
        push   ecx
        push   ecx
        push   edx
        push   eax
        db  0CDh, 020h, 044h, 000h, 033h, 000h
        add    esp, 14h
        pop    ecx
        db  083h, 03Dh, 0E0h, 007h, 000h, 000h, 001h
        ja     SHORT loc_9_0D07
        db  066h, 083h, 025h, 01Bh, 00Bh, 000h, 000h, 0DFh
        db  066h, 083h, 00Dh, 01Eh, 00Bh, 000h, 000h, 004h
loc_9_0D07:
        db  089h, 00Dh, 0E0h, 007h, 000h, 000h
        db  0C7h, 005h, 0E4h, 007h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0B8h, 0A7h, 009h, 000h, 000h
        mov    ebx, dword ptr [ebp + 8]
        db  0B9h, 095h, 009h, 000h, 000h
        db  0BAh, 0E0h, 007h, 000h, 000h
        push   1
        push   6
        push   edx
        push   3
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        mov    eax, 24h
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 020h, 000h
        je     SHORT loc_9_0D92
loc_9_0D4C:
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 000h, 004h
        je     SHORT loc_9_0D8D
        db  0BFh, 0BCh, 007h, 000h, 000h
        db  0BEh, 00Eh, 00Bh, 000h, 000h
        call   sub_9_1CDB
        je     SHORT loc_9_0D8D
        db  0B8h, 0BCh, 007h, 000h, 000h
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   2
        push   eax
        push   ebx
        db  0CDh, 020h, 02Fh, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        jne    SHORT loc_9_0D8D
        db  066h, 081h, 00Dh, 01Eh, 00Bh, 000h, 000h, 000h, 004h
loc_9_0D8D:
        mov    eax, 0
loc_9_0D92:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_9_0D97:
        db  083h, 03Dh, 05Ch, 006h, 000h, 000h, 000h
        je     NEAR PTR loc_9_1444
        db  066h, 083h, 025h, 01Bh, 00Bh, 000h, 000h, 0FCh
        db  0B8h, 068h, 006h, 000h, 000h
        push   4
        push   eax
        push   0
        db  0FFh, 035h, 05Ch, 006h, 000h, 000h
        db  0E8h, 06Fh, 0FCh, 0F6h, 0FFh
        add    esp, 10h
        cmp    eax, 0
        je     SHORT loc_9_0E1F
        db  0C6h, 005h, 06Ch, 006h, 000h, 000h, 034h
        db  0C6h, 005h, 06Dh, 006h, 000h, 000h, 000h
        db  0BEh, 013h, 00Bh, 000h, 000h
        db  0BFh, 094h, 006h, 000h, 000h
        mov    ecx, 8
        rep movsb
        db  066h, 0C7h, 005h, 080h, 006h, 000h, 000h, 039h, 036h
        db  0C6h, 005h, 082h, 006h, 000h, 000h, 000h
        db  0C7h, 005h, 0F4h, 008h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0A1h, 05Ch, 006h, 000h, 000h
        db  03Bh, 005h, 03Ch, 006h, 000h, 000h
        je     NEAR PTR loc_9_119C
        db  0C6h, 005h, 06Ch, 006h, 000h, 000h, 038h
        jmp    NEAR PTR loc_9_109D
loc_9_0E1F:
        db  0A1h, 068h, 006h, 000h, 000h
        db  0BBh, 015h, 009h, 000h, 000h
        db  0B9h, 060h, 006h, 000h, 000h
        db  0BAh, 06Ch, 006h, 000h, 000h
        db  0C7h, 005h, 07Ch, 006h, 000h, 000h, 010h, 000h, 000h, 000h
        db  0BFh, 07Ch, 006h, 000h, 000h
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        db  066h, 083h, 025h, 01Eh, 00Bh, 000h, 000h, 0F7h
        cmp    eax, 0
        je     SHORT loc_9_0E67
        db  066h, 083h, 00Dh, 01Eh, 00Bh, 000h, 000h, 008h
loc_9_0E67:
        db  0A1h, 068h, 006h, 000h, 000h
        db  0BBh, 022h, 009h, 000h, 000h
        db  0B9h, 060h, 006h, 000h, 000h
        db  0BAh, 094h, 006h, 000h, 000h
        db  0C7h, 005h, 0B4h, 006h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BFh, 0B4h, 006h, 000h, 000h
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        db  066h, 083h, 025h, 01Eh, 00Bh, 000h, 000h, 0EFh
        cmp    eax, 0
        je     SHORT loc_9_0EAF
        db  066h, 083h, 00Dh, 01Eh, 00Bh, 000h, 000h, 010h
loc_9_0EAF:
        db  0A1h, 068h, 006h, 000h, 000h
        db  0BBh, 02Dh, 009h, 000h, 000h
        db  0B9h, 060h, 006h, 000h, 000h
        db  0BAh, 080h, 006h, 000h, 000h
        db  0C7h, 005h, 090h, 006h, 000h, 000h, 010h, 000h, 000h, 000h
        db  0BFh, 090h, 006h, 000h, 000h
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        db  066h, 083h, 025h, 01Eh, 00Bh, 000h, 000h, 0DFh
        cmp    eax, 0
        je     SHORT loc_9_0EF7
        db  066h, 083h, 00Dh, 01Eh, 00Bh, 000h, 000h, 020h
loc_9_0EF7:
        db  0A1h, 068h, 006h, 000h, 000h
        db  0BBh, 05Eh, 009h, 000h, 000h
        db  0B9h, 060h, 006h, 000h, 000h
        db  0BAh, 0F4h, 006h, 000h, 000h
        db  0C7h, 005h, 064h, 006h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BFh, 064h, 006h, 000h, 000h
        db  0C6h, 005h, 0F4h, 006h, 000h, 000h, 000h
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        db  0A1h, 068h, 006h, 000h, 000h
        db  0BBh, 052h, 009h, 000h, 000h
        db  0B9h, 060h, 006h, 000h, 000h
        db  0BAh, 034h, 007h, 000h, 000h
        db  0C7h, 005h, 064h, 006h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BFh, 064h, 006h, 000h, 000h
        db  0C6h, 005h, 034h, 007h, 000h, 000h, 000h
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        db  0A1h, 068h, 006h, 000h, 000h
        db  0BBh, 047h, 009h, 000h, 000h
        db  0B9h, 060h, 006h, 000h, 000h
        db  0BAh, 014h, 007h, 000h, 000h
        db  0C7h, 005h, 064h, 006h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BFh, 064h, 006h, 000h, 000h
        db  0C6h, 005h, 014h, 007h, 000h, 000h, 000h
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        db  0A1h, 068h, 006h, 000h, 000h
        db  0BBh, 06Bh, 009h, 000h, 000h
        db  0B9h, 060h, 006h, 000h, 000h
        db  0BAh, 054h, 007h, 000h, 000h
        db  0C7h, 005h, 064h, 006h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BFh, 064h, 006h, 000h, 000h
        db  0C6h, 005h, 054h, 007h, 000h, 000h, 000h
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        db  0A1h, 068h, 006h, 000h, 000h
        db  0BBh, 045h, 00Bh, 000h, 000h
        db  0B9h, 060h, 006h, 000h, 000h
        db  0BAh, 0E0h, 007h, 000h, 000h
        db  0C7h, 005h, 064h, 006h, 000h, 000h, 080h, 000h, 000h, 000h
        db  0BFh, 064h, 006h, 000h, 000h
        db  0C6h, 005h, 0E0h, 007h, 000h, 000h, 000h
        push   edi
        push   edx
        push   ecx
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    ecx, 0
        db  0BEh, 0E0h, 007h, 000h, 000h
        cmp    byte ptr [esi], 0
        je     SHORT loc_9_102D
        call   sub_9_1D21
loc_9_102D:
        db  089h, 00Dh, 056h, 00Bh, 000h, 000h
        db  0A1h, 068h, 006h, 000h, 000h
        push   eax
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        db  0B8h, 0A7h, 009h, 000h, 000h
        db  08Bh, 01Dh, 05Ch, 006h, 000h, 000h
        db  0B9h, 039h, 009h, 000h, 000h
        db  0BAh, 0E8h, 006h, 000h, 000h
        db  0C7h, 005h, 0F0h, 006h, 000h, 000h, 008h, 000h, 000h, 000h
        db  0BEh, 0F0h, 006h, 000h, 000h
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        db  066h, 081h, 00Dh, 01Bh, 00Bh, 000h, 000h, 000h, 008h
        cmp    eax, 0
        jne    SHORT loc_9_109D
        db  0BEh, 0E8h, 006h, 000h, 000h
        call   sub_9_1D21
        or     ecx, ecx
        jne    SHORT loc_9_109D
        db  066h, 081h, 025h, 01Bh, 00Bh, 000h, 000h, 0FFh, 0F7h
loc_9_109D:
        db  0C7h, 005h, 0F4h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    word ptr [ebp + 1ch], 8085h
        je     NEAR PTR loc_9_119C
        db  0B8h, 0E0h, 007h, 000h, 000h
        db  0BEh, 0A0h, 009h, 000h, 000h
        db  0BFh, 06Ch, 006h, 000h, 000h
        call   sub_9_1CB5
        mov    byte ptr [ecx + eax], 5ch
        mov    byte ptr [ecx + eax + 1], 0
        mov    esi, eax
        db  0BFh, 094h, 006h, 000h, 000h
        call   sub_9_1CB5
        db  0B8h, 0E0h, 007h, 000h, 000h
        db  08Bh, 01Dh, 05Ch, 006h, 000h, 000h
        db  0B9h, 039h, 00Bh, 000h, 000h
        db  0BAh, 0B8h, 006h, 000h, 000h
        db  0C7h, 005h, 0D8h, 006h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BEh, 0D8h, 006h, 000h, 000h
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        cmp    eax, 0
        je     SHORT loc_9_115C
        db  0B8h, 0A7h, 009h, 000h, 000h
        db  08Bh, 01Dh, 05Ch, 006h, 000h, 000h
        db  0B9h, 039h, 00Bh, 000h, 000h
        db  0BAh, 0B8h, 006h, 000h, 000h
        db  0C7h, 005h, 0D8h, 006h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BEh, 0D8h, 006h, 000h, 000h
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        db  0C7h, 005h, 0F4h, 008h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_9_119C
loc_9_115C:
        db  0C7h, 005h, 0F4h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0BEh, 0AFh, 009h, 000h, 000h
        db  0BFh, 0B8h, 006h, 000h, 000h
        call   sub_9_1CDB
        je     SHORT loc_9_119C
        db  0C7h, 005h, 0F4h, 008h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0BEh, 0B8h, 006h, 000h, 000h
        call   sub_9_1D08
        jb     SHORT loc_9_119C
        call   sub_9_1D21
        or     ecx, ecx
        je     SHORT loc_9_119C
        db  089h, 00Dh, 0F4h, 008h, 000h, 000h
loc_9_119C:
        db  0C7h, 005h, 074h, 007h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 05Ch, 006h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_9_13EB
        db  03Bh, 005h, 03Ch, 006h, 000h, 000h
        jne    NEAR PTR loc_9_12D5
        db  066h, 0F7h, 005h, 01Eh, 00Bh, 000h, 000h, 018h, 000h
        jne    NEAR PTR loc_9_12D5
        db  0B8h, 0E0h, 007h, 000h, 000h
        db  0BEh, 0A0h, 009h, 000h, 000h
        db  0BFh, 06Ch, 006h, 000h, 000h
        call   sub_9_1CB5
        mov    byte ptr [ecx + eax], 5ch
        mov    byte ptr [ecx + eax + 1], 0
        mov    esi, eax
        db  0BFh, 094h, 006h, 000h, 000h
        call   sub_9_1CB5
        db  08Bh, 01Dh, 05Ch, 006h, 000h, 000h
        db  0B9h, 07Ch, 009h, 000h, 000h
        db  0BAh, 074h, 007h, 000h, 000h
        db  0C7h, 005h, 094h, 007h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BEh, 094h, 007h, 000h, 000h
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        db  066h, 083h, 00Dh, 01Bh, 00Bh, 000h, 000h, 001h
        cmp    eax, 0
        je     SHORT loc_9_123D
        db  066h, 083h, 025h, 01Bh, 00Bh, 000h, 000h, 0FEh
loc_9_123D:
        db  0B8h, 0E0h, 007h, 000h, 000h
        db  08Bh, 01Dh, 05Ch, 006h, 000h, 000h
        db  0B9h, 085h, 009h, 000h, 000h
        db  0BAh, 098h, 007h, 000h, 000h
        db  0C7h, 005h, 0B8h, 007h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BEh, 0B8h, 007h, 000h, 000h
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        db  066h, 083h, 00Dh, 01Bh, 00Bh, 000h, 000h, 002h
        cmp    eax, 0
        je     SHORT loc_9_1288
        db  066h, 083h, 025h, 01Bh, 00Bh, 000h, 000h, 0FDh
loc_9_1288:
        db  0B8h, 0E0h, 007h, 000h, 000h
        db  08Bh, 01Dh, 05Ch, 006h, 000h, 000h
        db  0B9h, 08Dh, 009h, 000h, 000h
        db  0BAh, 0BCh, 007h, 000h, 000h
        db  0C7h, 005h, 0DCh, 007h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BEh, 0DCh, 007h, 000h, 000h
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        db  066h, 081h, 00Dh, 01Bh, 00Bh, 000h, 000h, 000h, 004h
        cmp    eax, 0
        je     SHORT loc_9_12D5
        db  066h, 081h, 025h, 01Bh, 00Bh, 000h, 000h, 0FFh, 0FBh
loc_9_12D5:
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 001h, 000h
        jne    SHORT loc_9_133D
        db  0BEh, 080h, 009h, 000h, 000h
        db  0A1h, 05Ch, 006h, 000h, 000h
        db  03Bh, 005h, 03Ch, 006h, 000h, 000h
        jne    SHORT loc_9_12F7
        db  0BEh, 07Ch, 009h, 000h, 000h
loc_9_12F7:
        db  0BAh, 074h, 007h, 000h, 000h
        db  0C7h, 005h, 094h, 007h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0B9h, 094h, 007h, 000h, 000h
        db  0B8h, 0A7h, 009h, 000h, 000h
        db  08Bh, 01Dh, 05Ch, 006h, 000h, 000h
        push   1
        push   ecx
        push   edx
        push   1
        push   esi
        push   eax
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        db  066h, 083h, 00Dh, 01Bh, 00Bh, 000h, 000h, 001h
        cmp    eax, 0
        je     SHORT loc_9_133D
        db  066h, 083h, 025h, 01Bh, 00Bh, 000h, 000h, 0FEh
loc_9_133D:
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 002h, 000h
        jne    SHORT loc_9_1393
        db  0B8h, 0A7h, 009h, 000h, 000h
        db  08Bh, 01Dh, 05Ch, 006h, 000h, 000h
        db  0B9h, 085h, 009h, 000h, 000h
        db  0BAh, 098h, 007h, 000h, 000h
        db  0C7h, 005h, 0B8h, 007h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BEh, 0B8h, 007h, 000h, 000h
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        db  066h, 083h, 00Dh, 01Bh, 00Bh, 000h, 000h, 002h
        cmp    eax, 0
        je     SHORT loc_9_1393
        db  066h, 083h, 025h, 01Bh, 00Bh, 000h, 000h, 0FDh
loc_9_1393:
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 000h, 004h
        jne    SHORT loc_9_13EB
        db  0B8h, 0A7h, 009h, 000h, 000h
        db  08Bh, 01Dh, 05Ch, 006h, 000h, 000h
        db  0B9h, 08Dh, 009h, 000h, 000h
        db  0BAh, 0BCh, 007h, 000h, 000h
        db  0C7h, 005h, 0DCh, 007h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0BEh, 0DCh, 007h, 000h, 000h
        push   1
        push   esi
        push   edx
        push   1
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        db  066h, 081h, 00Dh, 01Bh, 00Bh, 000h, 000h, 000h, 004h
        cmp    eax, 0
        je     SHORT loc_9_13EB
        db  066h, 081h, 025h, 01Bh, 00Bh, 000h, 000h, 0FFh, 0FBh
loc_9_13EB:
        db  066h, 081h, 00Dh, 01Eh, 00Bh, 000h, 000h, 080h, 001h
        db  0A1h, 0F4h, 008h, 000h, 000h
        or     eax, eax
        js     SHORT loc_9_1441
        db  066h, 081h, 025h, 01Eh, 00Bh, 000h, 000h, 0FFh, 0FEh
        db  066h, 081h, 00Dh, 01Eh, 00Bh, 000h, 000h, 000h, 002h
        or     eax, eax
        jne    SHORT loc_9_1441
        db  066h, 081h, 025h, 01Eh, 00Bh, 000h, 000h, 0FFh, 0FDh
        db  066h, 083h, 00Dh, 01Eh, 00Bh, 000h, 000h, 040h
        db  0FFh, 035h, 05Ch, 006h, 000h, 000h
        call   sub_9_0B5A
        pop    ecx
        or     eax, eax
        je     SHORT loc_9_1441
        db  066h, 083h, 025h, 01Eh, 00Bh, 000h, 000h, 0BFh
        call   sub_9_1446
loc_9_1441:
        clc
        jmp    SHORT loc_9_1445
loc_9_1444:
        stc
loc_9_1445:
        ret
sub_9_1446:
        db  0C6h, 005h, 020h, 00Bh, 000h, 000h, 000h
        db  0C6h, 005h, 021h, 00Bh, 000h, 000h, 000h
        db  0C7h, 005h, 0E4h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0E8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0ECh, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0F0h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0F4h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0F8h, 008h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        mov    ebx, eax
        db  066h, 0F7h, 005h, 01Eh, 00Bh, 000h, 000h, 010h, 000h
        jne    NEAR PTR loc_9_17C2
        db  0BEh, 094h, 006h, 000h, 000h
        call   sub_9_1D58
        db  089h, 015h, 0FCh, 008h, 000h, 000h
        db  089h, 00Dh, 000h, 009h, 000h, 000h
        db  0B9h, 0BBh, 009h, 000h, 000h
        db  0BAh, 0C6h, 009h, 000h, 000h
        mov    edi, 100h
        push   1
        push   edi
        push   edx
        push   ecx
        push   ebx
        db  0CDh, 020h, 03Dh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    NEAR PTR loc_9_17C2
        db  0B8h, 0C6h, 009h, 000h, 000h
        db  0B9h, 0B2h, 009h, 000h, 000h
        push   ecx
        push   eax
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    NEAR PTR loc_9_1784
        xor    ecx, ecx
        db  0C7h, 005h, 0E6h, 00Ah, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0BAh, 0C6h, 00Ah, 000h, 000h
loc_9_1512:
        push   ecx
        push   edx
        push   20h
        push   edx
        push   ecx
        db  0FFh, 035h, 0B2h, 009h, 000h, 000h
        db  0CDh, 020h, 04Ch, 001h, 001h, 000h
        add    esp, 10h
        pop    edx
        pop    ecx
        cmp    eax, 0
        jne    SHORT loc_9_1572
        push   ecx
        db  0BEh, 0C6h, 00Ah, 000h, 000h
        mov    cl, 2ch
        call   sub_9_1DAB
        call   sub_9_1D21
        db  0A1h, 0FCh, 008h, 000h, 000h
        mov    esi, ecx
        pop    ecx
        or     esi, esi
        je     SHORT loc_9_1569
        sub    esi, eax
        jns    SHORT loc_9_1555
        neg    esi
loc_9_1555:
        db  03Bh, 035h, 0E6h, 00Ah, 000h, 000h
        jae    SHORT loc_9_1569
        db  089h, 035h, 0E6h, 00Ah, 000h, 000h
        db  089h, 00Dh, 0EAh, 00Ah, 000h, 000h
loc_9_1569:
        inc    ecx
        cmp    ecx, 100h
        jb     SHORT loc_9_1512
loc_9_1572:
        db  083h, 03Dh, 0E6h, 00Ah, 000h, 000h, 0FFh
        je     SHORT loc_9_1597
        db  08Bh, 00Dh, 0EAh, 00Ah, 000h, 000h
        push   20h
        push   edx
        push   ecx
        db  0FFh, 035h, 0B2h, 009h, 000h, 000h
        db  0CDh, 020h, 04Ch, 001h, 001h, 000h
        add    esp, 10h
loc_9_1597:
        db  0FFh, 035h, 0B2h, 009h, 000h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        db  083h, 03Dh, 0E6h, 00Ah, 000h, 000h, 0FFh
        je     NEAR PTR loc_9_17C2
        db  0BEh, 0A0h, 009h, 000h, 000h
        db  0BFh, 0C6h, 00Ah, 000h, 000h
        db  0B8h, 0C6h, 009h, 000h, 000h
        call   sub_9_1CB5
        db  0C6h, 005h, 0C1h, 009h, 000h, 000h, 001h
loc_9_15CE:
        db  0BEh, 0C1h, 009h, 000h, 000h
        db  0BFh, 0C2h, 009h, 000h, 000h
        mov    ecx, 1
        db  0F6h, 005h, 0C1h, 009h, 000h, 000h, 0F0h
        je     SHORT loc_9_15E7
        inc    ecx
loc_9_15E7:
        call   sub_9_1D6E
        db  0BEh, 0B6h, 009h, 000h, 000h
        db  0BFh, 0C2h, 009h, 000h, 000h
        db  0B8h, 0EEh, 00Ah, 000h, 000h
        call   sub_9_1CB5
        db  0C7h, 005h, 0E0h, 007h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0B9h, 0C6h, 009h, 000h, 000h
        db  0BAh, 0E0h, 007h, 000h, 000h
        db  0C7h, 005h, 060h, 008h, 000h, 000h, 080h, 000h, 000h, 000h
        db  0BEh, 060h, 008h, 000h, 000h
        push   1
        push   esi
        push   edx
        push   1
        push   eax
        push   ecx
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        cmp    eax, 0
        jne    NEAR PTR loc_9_175D
        db  083h, 03Dh, 0E0h, 007h, 000h, 000h, 000h
        je     NEAR PTR loc_9_175D
        db  0BEh, 0E0h, 007h, 000h, 000h
        mov    cl, 2ch
        call   sub_9_1DAB
        push   edi
        mov    cl, 2dh
        call   sub_9_1DC2
        jb     SHORT loc_9_167A
        call   sub_9_1DAB
        push   edi
        mov    cl, 2eh
        call   sub_9_1DAB
        call   sub_9_1D21
        db  089h, 00Dh, 0E4h, 008h, 000h, 000h
        pop    esi
loc_9_167A:
        mov    cl, 2eh
        call   sub_9_1DAB
        call   sub_9_1D21
        db  089h, 00Dh, 0E8h, 008h, 000h, 000h
        pop    esi
        mov    cl, 2ch
        call   sub_9_1DAB
        mov    al, byte ptr [edi]
        db  0A2h, 020h, 00Bh, 000h, 000h
        mov    al, byte ptr [edi + 2]
        db  0A2h, 021h, 00Bh, 000h, 000h
        mov    cl, 2dh
        call   sub_9_1DC2
        jb     SHORT loc_9_16C5
        call   sub_9_1DAB
        push   edi
        mov    cl, 2eh
        call   sub_9_1DAB
        call   sub_9_1D21
        db  089h, 00Dh, 0ECh, 008h, 000h, 000h
        pop    esi
loc_9_16C5:
        mov    cl, 2eh
        call   sub_9_1DAB
        call   sub_9_1D21
        db  089h, 00Dh, 0F0h, 008h, 000h, 000h
        db  0A1h, 000h, 009h, 000h, 000h
        db  08Bh, 00Dh, 056h, 00Bh, 000h, 000h
        cmp    ecx, 64h
        jae    SHORT loc_9_16F8
        mov    ecx, 69h
        cmp    eax, 258h
        jbe    SHORT loc_9_16F8
        mov    ecx, 6ch
loc_9_16F8:
        mul    ecx
        mov    ecx, eax
        db  0A1h, 0E8h, 008h, 000h, 000h
        mov    edx, 186a0h
        mul    edx
        div    ecx
        db  08Bh, 015h, 0F0h, 008h, 000h, 000h
        cmp    eax, edx
        jbe    SHORT loc_9_1716
        mov    eax, edx
loc_9_1716:
        db  03Bh, 005h, 0F4h, 008h, 000h, 000h
        jbe    SHORT loc_9_1723
        db  0A3h, 0F4h, 008h, 000h, 000h
loc_9_1723:
        db  0A1h, 0E4h, 008h, 000h, 000h
        mov    edx, 186a0h
        mul    edx
        div    ecx
        db  08Bh, 015h, 0ECh, 008h, 000h, 000h
        cmp    eax, edx
        jae    SHORT loc_9_173D
        mov    eax, edx
loc_9_173D:
        db  03Bh, 005h, 0F8h, 008h, 000h, 000h
        jae    SHORT loc_9_174A
        db  0A3h, 0F8h, 008h, 000h, 000h
loc_9_174A:
        db  0FEh, 005h, 0C1h, 009h, 000h, 000h
        db  080h, 03Dh, 0C1h, 009h, 000h, 000h, 0FFh
        jb     NEAR PTR loc_9_15CE
loc_9_175D:
        db  083h, 03Dh, 0F8h, 008h, 000h, 000h, 0FFh
        je     SHORT loc_9_1784
        db  0A1h, 0F4h, 008h, 000h, 000h
        db  03Bh, 005h, 0F8h, 008h, 000h, 000h
        jae    SHORT loc_9_1778
        db  0A3h, 0F8h, 008h, 000h, 000h
loc_9_1778:
        db  066h, 081h, 025h, 01Eh, 00Bh, 000h, 000h, 07Fh, 0FFh
        clc
        jmp    SHORT loc_9_17CC
loc_9_1784:
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 008h, 000h
        je     SHORT loc_9_17C2
        db  08Dh, 005h, 0F8h, 008h, 000h, 000h
        db  08Dh, 015h, 0F4h, 008h, 000h, 000h
        push   edx
        push   eax
        db  0FFh, 035h, 000h, 009h, 000h, 000h
        db  0FFh, 035h, 0FCh, 008h, 000h, 000h
        push   ebx
        db  0E8h, 0CEh, 0FBh, 0F6h, 0FFh
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_9_17C2
        db  066h, 081h, 00Dh, 01Eh, 00Bh, 000h, 000h, 080h, 002h
        clc
        jmp    SHORT loc_9_17CC
loc_9_17C2:
        db  066h, 081h, 00Dh, 01Eh, 00Bh, 000h, 000h, 080h, 000h
        stc
loc_9_17CC:
        ret
loc_9_17CD:
        mov    ecx, dword ptr [ebp + 18h]
        cmp    ecx, 22h
        jl     SHORT loc_9_182C
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     SHORT loc_9_182C
        mov    ebx, dword ptr [ebp + 10h]
        test   ebx, 0ffff0000h
        jne    SHORT loc_9_17FC
        dec    ebx
        cmp    ebx, 8
        jae    SHORT loc_9_182C
        db  08Bh, 01Ch, 09Dh, 03Ch, 006h, 000h, 000h
loc_9_17FC:
        or     ebx, ebx
        je     SHORT loc_9_182C
        db  089h, 01Dh, 05Ch, 006h, 000h, 000h
        mov    ecx, dword ptr [ebp + 18h]
        call   sub_9_184A
        jb     SHORT loc_9_182C
        test   word ptr [eax + 2], 6
        jne    SHORT loc_9_182C
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 001h, 000h
        je     SHORT loc_9_182C
        mov    dword ptr [ebp + 1ch], 0
        jmp    SHORT loc_9_1833
loc_9_182C:
        mov    dword ptr [ebp + 1ch], 0ffffffffh
loc_9_1833:
        ret
VDD_Get_DISPLAYINFO:
        cmp    ecx, 22h
        jl     SHORT loc_9_1844
        call   sub_9_184A
        jb     SHORT loc_9_1844
        xor    eax, eax
        jmp    SHORT loc_9_1849
loc_9_1844:
        mov    eax, 0ffffffffh
loc_9_1849:
        ret
sub_9_184A:
        cld
        push   eax
        push   ecx
        db  066h, 083h, 00Dh, 01Eh, 00Bh, 000h, 000h, 001h
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        pop    ecx
        pop    eax
        cmp    dword ptr [edi + 24h], 0
        jne    SHORT loc_9_187D
        db  066h, 083h, 025h, 01Eh, 00Bh, 000h, 000h, 0FEh
        push   eax
        push   ecx
        mov    word ptr [eax], cx
        call   sub_9_0D97
        pop    ecx
        pop    eax
        jb     NEAR PTR loc_9_1A5A
loc_9_187D:
        mov    word ptr [eax], cx
        db  066h, 08Bh, 00Dh, 01Eh, 00Bh, 000h, 000h
        mov    word ptr [eax + 2], cx
        db  08Bh, 00Dh, 05Ch, 006h, 000h, 000h
        mov    dword ptr [eax + 4], ecx
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 001h, 000h
        je     SHORT loc_9_18AE
        lea    edi, [eax + 8]
        db  0BEh, 074h, 007h, 000h, 000h
        mov    ecx, 10h
        rep movsb
loc_9_18AE:
        test   word ptr [eax + 2], 8
        jne    SHORT loc_9_18C3
        db  0BEh, 06Ch, 006h, 000h, 000h
        call   sub_9_1D21
        mov    byte ptr [eax + 1fh], cl
loc_9_18C3:
        mov    byte ptr [eax + 1eh], 1
        test   word ptr [eax + 2], 10h
        jne    SHORT loc_9_18E1
        db  0BEh, 094h, 006h, 000h, 000h
        call   sub_9_1D58
        mov    word ptr [eax + 18h], dx
        mov    word ptr [eax + 1ah], cx
loc_9_18E1:
        mov    word ptr [eax + 1ch], 60h
        test   word ptr [eax + 2], 20h
        jne    SHORT loc_9_18FD
        db  0BEh, 080h, 006h, 000h, 000h
        call   sub_9_1D21
        mov    word ptr [eax + 1ch], cx
loc_9_18FD:
        cmp    word ptr [eax], 4ah
        jb     SHORT loc_9_1978
        db  0BEh, 0F4h, 006h, 000h, 000h
        call   sub_9_1D21
        mov    dword ptr [eax + 36h], ecx
        db  0BEh, 014h, 007h, 000h, 000h
        call   sub_9_1D58
        mov    dword ptr [eax + 3ah], edx
        mov    dword ptr [eax + 3eh], ecx
        movzx  edx, word ptr [eax + 18h]
        movzx  ecx, word ptr [eax + 1ah]
        mov    dword ptr [eax + 42h], edx
        mov    dword ptr [eax + 46h], ecx
        db  0BEh, 034h, 007h, 000h, 000h
        cmp    byte ptr [esi], 0
        je     SHORT loc_9_194F
        call   sub_9_1D58
        cmp    word ptr [eax + 18h], dx
        ja     SHORT loc_9_194F
        cmp    word ptr [eax + 1ah], cx
        ja     SHORT loc_9_194F
        mov    dword ptr [eax + 42h], edx
        mov    dword ptr [eax + 46h], ecx
loc_9_194F:
        db  0BEh, 054h, 007h, 000h, 000h
        cmp    byte ptr [esi], 0
        je     SHORT loc_9_1978
        call   sub_9_1D58
        cmp    word ptr [eax + 18h], dx
        jb     SHORT loc_9_1978
        cmp    word ptr [eax + 1ah], cx
        jb     SHORT loc_9_1978
        xchg   word ptr [eax + 18h], dx
        xchg   word ptr [eax + 1ah], cx
        mov    dword ptr [eax + 42h], edx
        mov    dword ptr [eax + 46h], ecx
loc_9_1978:
        cmp    word ptr [eax], 32h
        jb     NEAR PTR loc_9_1A2C
        mov    word ptr [eax + 20h], 0
        mov    word ptr [eax + 22h], 0
        mov    word ptr [eax + 24h], 0
        mov    word ptr [eax + 26h], 0
        mov    word ptr [eax + 28h], 0
        mov    word ptr [eax + 2ah], 0
        mov    byte ptr [eax + 30h], 0
        mov    byte ptr [eax + 31h], 0
        mov    ebx, eax
        db  0FFh, 035h, 05Ch, 006h, 000h, 000h
        call   sub_9_0B5A
        pop    ecx
        xchg   ebx, eax
        mov    dword ptr [eax + 2ch], ebx
        test   word ptr [eax + 2], 100h
        jne    SHORT loc_9_1A2C
        db  066h, 08Bh, 00Dh, 0F4h, 008h, 000h, 000h
        mov    word ptr [eax + 20h], cx
        test   word ptr [eax + 2], 200h
        jne    SHORT loc_9_1A2C
        test   word ptr [eax + 2], 80h
        jne    SHORT loc_9_1A2C
        db  066h, 08Bh, 00Dh, 0F8h, 008h, 000h, 000h
        mov    word ptr [eax + 22h], cx
        db  066h, 08Bh, 00Dh, 0E4h, 008h, 000h, 000h
        mov    word ptr [eax + 24h], cx
        db  066h, 08Bh, 00Dh, 0E8h, 008h, 000h, 000h
        mov    word ptr [eax + 26h], cx
        db  066h, 08Bh, 00Dh, 0ECh, 008h, 000h, 000h
        mov    word ptr [eax + 28h], cx
        db  066h, 08Bh, 00Dh, 0F0h, 008h, 000h, 000h
        mov    word ptr [eax + 2ah], cx
        db  08Ah, 00Dh, 020h, 00Bh, 000h, 000h
        mov    byte ptr [eax + 30h], cl
        db  08Ah, 00Dh, 021h, 00Bh, 000h, 000h
        mov    byte ptr [eax + 31h], cl
loc_9_1A2C:
        test   word ptr [eax + 2], 1
        jne    SHORT loc_9_1A4F
        db  08Bh, 015h, 03Ch, 006h, 000h, 000h
        db  03Bh, 015h, 05Ch, 006h, 000h, 000h
        je     SHORT loc_9_1A4F
        db  089h, 015h, 05Ch, 006h, 000h, 000h
        push   eax
        call   sub_9_0D97
        pop    eax
loc_9_1A4F:
        test   word ptr [eax + 2], 18h
        jne    SHORT loc_9_1A5A
        clc
        jmp    SHORT loc_9_1A5B
loc_9_1A5A:
        stc
loc_9_1A5B:
        ret
sub_9_1A5C:
        enter  74h, 0
        lea    edi, [ebp - 6ch]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        mov    eax, 1388h
        mov    edx, ebp
        db  0BEh, 0ACh, 01Ah, 000h, 000h
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0CDh, 020h, 0FDh, 000h, 001h, 000h
        mov    dword ptr [ebp - 70h], esi
        xor    eax, eax
        mov    dword ptr [ebp - 74h], eax
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        xor    esi, esi
        xchg   dword ptr [ebp - 70h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        xor    esi, esi
        xchg   dword ptr [ebp - 74h], esi
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        leave
        ret
loc_9_1AAC:
        mov    ebp, edx
        mov    ebx, edi
        xor    eax, eax
        mov    dword ptr [ebp - 70h], eax
        mov    ecx, 120h
        db  0BEh, 0C9h, 01Ah, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [ebp - 74h], esi
        ret
loc_9_1AC9:
        mov    ebp, edx
        mov    dword ptr [ebp - 74h], 0
        lea    esi, [ebp - 6ch]
        mov    dword ptr [esi + 1ch], 0ffffffffh
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        ret
sub_9_1AE3:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp], 0
        mov    word ptr [ebp + 38h], 0
        mov    word ptr [ebp + 1ch], 4f15h
        mov    byte ptr [ebp + 10h], 0
        mov    word ptr [ebp + 18h], 0
        call   sub_9_1A5C
        mov    al, byte ptr [ebp + 10h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        cmp    ax, 4fh
        jne    NEAR PTR loc_9_1CA4
        test   eax, 30000h
        je     NEAR PTR loc_9_1CAB
        mov    ecx, 80h
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     SHORT loc_9_1B7E
        xor    eax, eax
        cmp    ecx, 80h
        jae    SHORT loc_9_1BAA
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        jmp    NEAR PTR loc_9_1CAB
loc_9_1B7E:
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 040h, 000h
        jne    NEAR PTR loc_9_1CAB
        push   0
        push   80h
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_9_1CAB
        mov    edi, eax
        shl    edi, 0ch
loc_9_1BAA:
        push   eax
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp], di
        ror    edi, 10h
        mov    word ptr [ebp + 38h], di
        mov    word ptr [ebp + 1ch], 4f15h
        mov    byte ptr [ebp + 10h], 1
        mov    word ptr [ebp + 18h], 0
        mov    word ptr [ebp + 14h], 0
        call   sub_9_1A5C
        mov    ax, word ptr [ebp + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    esi
        or     esi, esi
        je     SHORT loc_9_1C1A
        cld
        mov    edi, dword ptr [esp + 14h]
        push   eax
        mov    ecx, 20h
        rep movsd
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
        pop    eax
        jmp    SHORT loc_9_1C32
loc_9_1C1A:
        mov    esi, dword ptr [esp + 14h]
        push   eax
        mov    ecx, 80h
        mov    ax, ds
        mov    fs, ax
        stc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        pop    eax
loc_9_1C32:
        cmp    ax, -1
        je     SHORT loc_9_1CA4
        cmp    ax, 4fh
        jne    SHORT loc_9_1CAB
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esi]
        cmp    eax, 0ffffff00h
        jne    SHORT loc_9_1CAB
        mov    eax, dword ptr [esi + 4]
        cmp    eax, 0ffffffh
        jne    SHORT loc_9_1CAB
        mov    ax, word ptr [esi + 8]
        mov    cl, al
        shr    cl, 2
        and    cl, 1fh
        add    cl, 40h
        mov    byte ptr [edi], cl
        mov    cx, ax
        xchg   ch, cl
        shr    cx, 5
        and    cl, 1fh
        add    cl, 40h
        mov    byte ptr [edi + 1], cl
        and    ah, 1fh
        add    ah, 40h
        mov    byte ptr [edi + 2], ah
        lea    esi, [esi + 0ah]
        add    edi, 3
        mov    ecx, 4
        call   sub_9_1D6E
        db  066h, 083h, 00Dh, 01Bh, 00Bh, 000h, 000h, 008h
        mov    eax, 0
        jmp    SHORT loc_9_1CB0
loc_9_1CA4:
        mov    eax, 20h
        jmp    SHORT loc_9_1CB0
loc_9_1CAB:
        mov    eax, 13h
loc_9_1CB0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_9_1CB5:
        cld
        push   eax
        push   ebx
        push   esi
        push   edi
        xor    ecx, ecx
loc_9_1CBC:
        mov    bl, byte ptr [esi]
        or     bl, bl
        je     SHORT loc_9_1CC9
        mov    byte ptr [eax], bl
        inc    eax
        inc    esi
        inc    ecx
        jmp    SHORT loc_9_1CBC
loc_9_1CC9:
        mov    bl, byte ptr [edi]
        mov    byte ptr [eax], bl
        or     bl, bl
        je     SHORT loc_9_1CD6
        inc    eax
        inc    edi
        inc    ecx
        jmp    SHORT loc_9_1CC9
loc_9_1CD6:
        pop    edi
        pop    esi
        pop    ebx
        pop    eax
        ret
sub_9_1CDB:
        cld
        push   eax
        push   edi
        push   esi
loc_9_1CDF:
        mov    al, byte ptr [esi]
        mov    ah, byte ptr [edi]
        inc    esi
        inc    edi
        cmp    al, 61h
        jb     SHORT loc_9_1CEF
        cmp    al, 7ah
        ja     SHORT loc_9_1CEF
        and    al, 0dfh
loc_9_1CEF:
        cmp    ah, 61h
        jb     SHORT loc_9_1CFC
        cmp    ah, 7ah
        ja     SHORT loc_9_1CFC
        and    ah, 0dfh
loc_9_1CFC:
        cmp    al, ah
        jne    SHORT loc_9_1D04
        or     al, al
        jne    SHORT loc_9_1CDF
loc_9_1D04:
        pop    esi
        pop    edi
        pop    eax
        ret
sub_9_1D08:
        cld
        push   eax
        push   esi
loc_9_1D0B:
        mov    al, byte ptr [esi]
        inc    esi
        or     al, al
        je     SHORT loc_9_1D1D
        cmp    al, 30h
        jb     SHORT loc_9_1D1A
        cmp    al, 39h
        jbe    SHORT loc_9_1D0B
loc_9_1D1A:
        stc
        jmp    SHORT loc_9_1D1E
loc_9_1D1D:
        clc
loc_9_1D1E:
        pop    esi
        pop    eax
        ret
sub_9_1D21:
        push   eax
        push   ebx
        push   edx
        push   edi
        xor    ecx, ecx
        xor    bl, bl
loc_9_1D29:
        cmp    byte ptr [esi], 0
        je     SHORT loc_9_1D33
        inc    bl
        inc    esi
        jmp    SHORT loc_9_1D29
loc_9_1D33:
        or     bl, bl
        je     SHORT loc_9_1D53
        dec    esi
        mov    edi, 1
loc_9_1D3D:
        movzx  eax, byte ptr [esi]
        sub    eax, 30h
        mul    edi
        add    ecx, eax
        dec    bl
        je     SHORT loc_9_1D53
        dec    esi
        lea    edi, [edi + edi*4]
        add    edi, edi
        jmp    SHORT loc_9_1D3D
loc_9_1D53:
        pop    edi
        pop    edx
        pop    ebx
        pop    eax
        ret
sub_9_1D58:
        mov    cl, 2ch
        call   sub_9_1DAB
        call   sub_9_1D21
        mov    edx, ecx
        mov    esi, edi
        call   sub_9_1D21
        ret
sub_9_1D6E:
        or     ecx, ecx
        je     SHORT loc_9_1DAA
        push   eax
        push   esi
        push   edi
        push   ecx
        mov    eax, ecx
        add    edi, eax
        mov    byte ptr [edi], 0
loc_9_1D7D:
        mov    al, byte ptr [esi]
        mov    ah, al
        shr    al, 4
        and    ah, 0fh
        add    ax, 3030h
        cmp    al, 3ah
        jb     SHORT loc_9_1D91
        add    al, 7
loc_9_1D91:
        cmp    ah, 3ah
        jb     SHORT loc_9_1D99
        add    ah, 7
loc_9_1D99:
        dec    edi
        mov    byte ptr [edi], ah
        dec    ecx
        je     SHORT loc_9_1DA6
        dec    edi
        mov    byte ptr [edi], al
        inc    esi
        dec    ecx
        jne    SHORT loc_9_1D7D
loc_9_1DA6:
        pop    ecx
        pop    edi
        pop    esi
        pop    eax
loc_9_1DAA:
        ret
sub_9_1DAB:
        push   esi
        mov    edi, esi
loc_9_1DAE:
        cmp    byte ptr [esi], cl
        je     SHORT loc_9_1DBA
        cmp    byte ptr [esi], 0
        je     SHORT loc_9_1DBA
        inc    esi
        jmp    SHORT loc_9_1DAE
loc_9_1DBA:
        mov    byte ptr [esi], 0
        mov    edi, esi
        inc    edi
        pop    esi
        ret
sub_9_1DC2:
        push   esi
loc_9_1DC3:
        cmp    byte ptr [esi], cl
        je     SHORT loc_9_1DCF
        cmp    byte ptr [esi], 0
        je     SHORT loc_9_1DD2
        inc    esi
        jmp    SHORT loc_9_1DC3
loc_9_1DCF:
        clc
        jmp    SHORT loc_9_1DD3
loc_9_1DD2:
        stc
loc_9_1DD3:
        pop    esi
        ret
        db  000h, 000h, 000h
_PTEXT ends
        end
