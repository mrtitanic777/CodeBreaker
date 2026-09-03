; dosmgr_pcode_11.asm -- DOSMGR object 11 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

DOSMGR_SYS_VM_INIT:
        db  0E8h, 033h, 00Ch, 0F5h, 0FFh
        db  0E8h, 02Eh, 00Ah, 0F5h, 0FFh
        db  0E9h, 006h, 000h, 0F5h, 0FFh
DOSMGR_SYS_VM_TERMINATE:
        pushad
        db  00Fh, 0B7h, 015h, 034h, 000h, 000h, 000h
        mov    eax, edx
        shl    eax, 10h
        mov    ax, 18h
        shl    edx, 4
        mov    dword ptr [edx + 34h], eax
        mov    word ptr [edx + 32h], 14h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    word ptr [ebp + 1ch], 1607h
        mov    word ptr [ebp + 10h], 15h
        mov    word ptr [ebp + 18h], 2
        db  0A1h, 09Dh, 001h, 000h, 000h
        mov    dword ptr [ebp + 14h], eax
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        clc
        ret
loc_11_0087:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        jecxz  loc_11_00B1
        push   eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   0
        push   ecx
        push   dword ptr [eax + 8]
        push   ebx
        push   dword ptr [eax + 0ch]
        db  0CDh, 020h, 05Eh, 000h, 001h, 000h
        add    esp, 14h
        pop    eax
loc_11_00B1:
        mov    edi, dword ptr [esi + 10h]
        test   edi, edi
        je     SHORT loc_11_00C8
        btr    dword ptr [eax + 14h], 0
        jae    SHORT loc_11_00C8
        db  0E8h, 03Ch, 0FFh, 0F4h, 0FFh
        and    byte ptr [edi + 6], 0fdh
loc_11_00C8:
        btr    dword ptr [eax + 14h], 1
        jae    SHORT loc_11_013F
        sub    edi, edi
        mov    edx, dword ptr [esi]
        sub    edx, 10h
        db  0A1h, 004h, 002h, 000h, 000h
        movzx  eax, word ptr [eax - 2]
        shl    eax, 4
loc_11_00E2:
        cmp    eax, edx
        ja     SHORT loc_11_00F6
        xchg   edi, eax
        movzx  eax, word ptr [edi + 3]
        inc    eax
        shl    eax, 4
        add    eax, edi
        cmp    byte ptr [edi], 4dh
        je     SHORT loc_11_00E2
loc_11_00F6:
        test   edi, edi
        je     SHORT loc_11_013F
        shr    edx, 4
        dec    edx
        mov    eax, edi
        shr    eax, 4
        sub    edx, eax
        xchg   word ptr [edi + 3], dx
        sub    dx, word ptr [edi + 3]
        jbe    SHORT loc_11_013F
        mov    ecx, dword ptr [esi + 4]
        add    ecx, 1fh
        shr    ecx, 4
        mov    eax, dword ptr [esi]
        sub    edx, ecx
        jbe    SHORT loc_11_0138
        dec    edx
        add    eax, dword ptr [esi + 4]
        push   esi
        mov    esi, edi
        xchg   edi, eax
        mov    ecx, 4
        cld
        rep movsd
        pop    esi
        mov    byte ptr [eax], 4dh
        mov    word ptr [edi - 0dh], dx
        jmp    SHORT loc_11_013F
loc_11_0138:
        mov    dl, 4dh
        xchg   byte ptr [edi], dl
        mov    byte ptr [eax - 10h], dl
loc_11_013F:
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [eax]
        jecxz  loc_11_0153
        mov    dword ptr [esi + 8], ecx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [esi + 0ch], edx
        push   esi
        call   ecx
        pop    ecx
loc_11_0153:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_11_0157:
        db  0A1h, 05Ch, 000h, 000h, 000h
        db  08Bh, 00Dh, 060h, 000h, 000h, 000h
        mov    dword ptr [eax], ecx
        ret
DOSMGR_SYSTEM_EXIT:
        db  080h, 03Dh, 01Ch, 000h, 000h, 000h, 000h
        je     SHORT loc_11_01A3
        db  08Bh, 00Dh, 028h, 000h, 000h, 000h
        jecxz  loc_11_01A3
        db  08Bh, 035h, 020h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_11_01A3
        db  083h, 03Dh, 024h, 000h, 000h, 000h, 000h
        je     SHORT loc_11_01A3
loc_11_0189:
        test   byte ptr [esi + 44h], 40h
        je     SHORT loc_11_019B
        test   byte ptr [esi + 44h], 0a0h
        jne    SHORT loc_11_019B
        mov    word ptr [esi + 49h], 0ffffh
loc_11_019B:
        db  003h, 035h, 024h, 000h, 000h, 000h
        loop   loc_11_0189
loc_11_01A3:
        clc
        ret
DOSMGR_DEVICE_REBOOT_NOTIFY:
        sub    esp, 6ch
        mov    edi, esp
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 19h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ch, byte ptr [ebp + 1ch]
        mov    byte ptr [ebp + 14h], 0
loc_11_01CC:
        mov    byte ptr [ebp + 1dh], 0eh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    cl, byte ptr [ebp + 1ch]
        mov    byte ptr [ebp + 1dh], 0dh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        inc    byte ptr [ebp + 14h]
        cmp    byte ptr [ebp + 14h], cl
        jb     SHORT loc_11_01CC
        mov    byte ptr [ebp + 14h], ch
        mov    byte ptr [ebp + 1dh], 0eh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
        clc
        ret
        db  000h
loc_11_020C:
        db  077h, 069h
        outsb
        db  064h, 069h, 072h, 000h
sub_11_0213:
        pushad
        shl    ecx, 4
        add    ecx, dword ptr [ebx + 4]
        xor    esi, esi
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        mov    edi, edx
        db  0BEh, 00Ch, 002h, 000h, 000h
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        jb     SHORT loc_11_024C
        sub    edx, edi
        sub    edx, 7
        db  0E8h, 034h, 001h, 0F5h, 0FFh
        lea    edi, [edx + ecx]
        push   edi
        sub    eax, edx
        mov    ecx, eax
        xor    eax, eax
        repne scasb
        mov    esi, edi
        pop    edi
        rep movsb
loc_11_024C:
        popad
        ret
loc_11_024E:
        db  08Bh, 03Dh, 010h, 000h, 000h, 000h
        add    edi, dword ptr [ebx + 4]
        xor    esi, esi
        db  087h, 035h, 03Ch, 000h, 000h, 000h
        push   174h
        push   esi
        push   edi
        db  0CDh, 020h, 07Dh, 001h, 001h, 000h
        add    esp, 0ch
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    esi, dword ptr [ebx + 8]
        mov    byte ptr [esi + 1dh], 48h
        or     word ptr [esi + 10h], 0ffffh
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        db  0E9h, 0C9h, 005h, 0F5h, 0FFh
        db  000h
loc_11_02A4:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [edx + ebx], 10h
        jne    SHORT loc_11_02B6
        mov    byte ptr [ebp + 1ch], 3
        clc
        ret
loc_11_02B6:
        mov    eax, 24h
        db  0CDh, 020h, 041h, 001h, 001h, 000h
        db  03Bh, 005h, 044h, 000h, 000h, 000h
        je     SHORT loc_11_02D5
        push   ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        pop    ebx
        or     ecx, ecx
        jne    SHORT loc_11_02D7
loc_11_02D5:
        stc
        ret
loc_11_02D7:
        mov    eax, 0fah
        db  0BEh, 0E9h, 002h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        jmp    SHORT loc_11_02D5
loc_11_02E9:
        jae    SHORT loc_11_02F7
        push   ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        pop    ebx
        jecxz  loc_11_02F7
        jmp    SHORT sub_11_02F8
loc_11_02F7:
        ret
sub_11_02F8:
        mov    eax, 0fh
        xor    edx, edx
        xor    esi, esi
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        db  080h, 03Dh, 0E0h, 000h, 000h, 000h, 000h
        je     SHORT loc_11_031A
        db  03Bh, 01Dh, 0DCh, 000h, 000h, 000h
        jne    SHORT loc_11_0320
        jmp    SHORT loc_11_0339
loc_11_031A:
        db  0FEh, 005h, 0E0h, 000h, 000h, 000h
loc_11_0320:
        db  089h, 01Dh, 0DCh, 000h, 000h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    edx, eax
        db  0BEh, 03Bh, 003h, 000h, 000h
        db  0CDh, 020h, 024h, 000h, 001h, 000h
loc_11_0339:
        stc
        ret
loc_11_033B:
        db  0C6h, 005h, 0E0h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, edx
        db  0CDh, 020h, 037h, 000h, 001h, 000h
        ret
        db  000h, 000h, 000h
loc_11_0354:
        pushad
        mov    ax, word ptr [ebp + 3ch]
        mov    edx, dword ptr [ebp + 14h]
        db  0E8h, 007h, 002h, 0F5h, 0FFh
        jae    SHORT loc_11_03CE
        mov    esi, eax
        mov    ecx, dword ptr [ebp + 18h]
        test   word ptr [ebx], 180h
        jne    SHORT loc_11_0372
        movzx  ecx, cx
loc_11_0372:
        dec    ecx
        and    eax, 0fffh
        add    ecx, eax
        shr    ecx, 0ch
        inc    ecx
        mov    eax, esi
        shr    eax, 0ch
        push   0
        push   ecx
        push   eax
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_11_03CB
        mov    ecx, dword ptr [ebp + 18h]
        shl    esi, 0ch
        shr    si, 0ch
        xchg   word ptr [ebp + 14h], si
        ror    esi, 10h
        xchg   word ptr [ebp + 60h], si
        db  0E8h, 0FEh, 000h, 0F5h, 0FFh
        mov    word ptr [ebp + 60h], si
        shr    esi, 10h
        mov    word ptr [ebp + 14h], si
        mov    dword ptr [ebp + 18h], ecx
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        popad
        ret
loc_11_03CB:
        add    esp, 0ch
loc_11_03CE:
        popad
        db  0BAh, 030h, 001h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_11_03DA:
        add    dword ptr [3c1400h], eax
        db  021h
loc_11_03E1:
        add    dword ptr [edi], ecx
        adc    al, 3ch
        add    byte ptr [ecx], ah
loc_11_03E8:
        add    dword ptr [ecx + eax], eax
        adc    al, 3ch
        add    byte ptr [ecx], ah
loc_11_03EF:
        db  005h, 0CDh, 000h, 000h, 000h
        db  003h, 0FDh
        add    eax, dword ptr [eax]
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
loc_11_03FD:
        cmp    byte ptr [ebp + 19h], 8
        je     SHORT loc_11_040F
        movzx  ecx, word ptr fs:[esi + 5ch]
        shl    ecx, 2
        add    ecx, 5eh
        ret
loc_11_040F:
        movzx  ecx, word ptr fs:[esi + 26h]
        shl    ecx, 2
        add    ecx, 28h
        ret
loc_11_041B:
        cmp    byte ptr [ebp + 19h], 8
        je     SHORT loc_11_0427
        mov    ecx, 5ch
        ret
loc_11_0427:
        mov    ecx, 26h
        ret
sub_11_042D:
        db  003h, 036h
        add    al, 0
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
loc_11_0436:
        movzx  ecx, byte ptr fs:[esi]
        ret
loc_11_043B:
        db  005h, 040h, 004h, 000h, 000h
loc_11_0440:
        pushad
        push   fs
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 60h]
        movzx  edx, byte ptr [ebp + 10h]
        call   sub_11_09D3
        jb     NEAR PTR loc_11_057B
        push   eax
        push   edx
        push   ecx
        mov    ax, 3c14h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        mov    ecx, 0dh
        stc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     NEAR PTR loc_11_0578
        mov    word ptr [ebp + 14h], di
        movzx  edx, di
        shr    edi, 10h
        mov    word ptr [ebp + 60h], di
        shl    edi, 4
        add    edi, edx
        movzx  eax, word ptr [edi + 7]
        mul    dword ptr [esp]
        mov    ecx, eax
        test   word ptr [ebx], 180h
        jne    SHORT loc_11_04A6
        movzx  edx, word ptr fs:[esi + 9]
        mov    ax, word ptr fs:[esi + 0bh]
        jmp    SHORT loc_11_04AF
loc_11_04A6:
        mov    edx, dword ptr fs:[esi + 9]
        mov    ax, word ptr fs:[esi + 0dh]
loc_11_04AF:
        db  0E8h, 0B4h, 000h, 0F5h, 0FFh
        mov    esi, eax
        jae    SHORT loc_11_0509
        push   eax
        push   ecx
        dec    ecx
        and    eax, 0fffh
        add    ecx, eax
        shr    ecx, 0ch
        inc    ecx
        mov    eax, esi
        shr    eax, 0ch
        push   0
        push   ecx
        push   eax
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_11_04E7
        add    esp, 0ch
        pop    ecx
        pop    eax
        jmp    SHORT loc_11_0509
loc_11_04E7:
        shl    esi, 0ch
        shr    si, 0ch
        mov    word ptr [edi + 9], si
        shr    esi, 10h
        mov    word ptr [edi + 0bh], si
        db  0E8h, 0B2h, 0FFh, 0F4h, 0FFh
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 14h
        jmp    SHORT loc_11_055F
loc_11_0509:
        mov    ax, ds
        mov    fs, ax
        db  0CDh, 020h, 00Ah, 000h, 006h, 000h
        mov    eax, ecx
        xor    edx, edx
        div    dword ptr [esp]
        movzx  ecx, word ptr [edi + 7]
loc_11_0520:
        mov    edx, ecx
        cmp    ecx, eax
        jbe    SHORT loc_11_0528
        mov    edx, eax
loc_11_0528:
        call   sub_11_058F
        jb     SHORT loc_11_055F
        sub    ecx, edx
        je     SHORT loc_11_055F
        add    word ptr [edi + 5], dx
loc_11_0537:
        movzx  edx, word ptr [edi + 5]
        sub    edx, dword ptr [esp + 8]
        jb     SHORT loc_11_0520
        mov    word ptr [edi + 5], dx
        inc    word ptr [edi + 1]
        mov    edx, dword ptr [esp + 4]
        cmp    word ptr [edi + 1], dx
        jb     SHORT loc_11_0537
        mov    word ptr [edi + 1], 0
        inc    word ptr [edi + 3]
        jmp    SHORT loc_11_0537
loc_11_055F:
        mov    ecx, 0dh
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        add    esp, 0ch
        pop    dword ptr [ebp + 60h]
        pop    dword ptr [ebp + 14h]
        pop    fs
        popad
        ret
loc_11_0578:
        add    esp, 0ch
loc_11_057B:
        pop    dword ptr [ebp + 60h]
        pop    dword ptr [ebp + 14h]
        pop    fs
        popad
        mov    word ptr [ebp + 1ch], 2
        or     byte ptr [ebp + 2ch], 1
        ret
sub_11_058F:
        push   eax
        push   ecx
        push   dword ptr [ebp + 1ch]
        mov    dword ptr [edi + 7], edx
        mov    eax, edx
        push   edx
        mul    dword ptr [esp + 14h]
        pop    edx
        mov    ecx, eax
        push   edi
        bt     dword ptr [ebp + 18h], 5
        cmc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     SHORT loc_11_05D9
        mov    eax, edi
        pop    edi
        mov    dword ptr [edi + 9], eax
        db  0E8h, 0F5h, 0FEh, 0F4h, 0FFh
        bt     dword ptr [ebp + 18h], 5
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        add    esi, ecx
loc_11_05C8:
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_11_05D5
        pop    dword ptr [ebp + 1ch]
        jmp    SHORT loc_11_05D6
loc_11_05D5:
        pop    ecx
loc_11_05D6:
        pop    ecx
        pop    eax
        ret
loc_11_05D9:
        pop    edi
        or     byte ptr [ebp + 2ch], 1
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_11_05C8
        db  000h, 000h
loc_11_05E8:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 8]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 4]
        mov    dword ptr [ebp + 4], ecx
        add    ecx, 12h
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        mov    word ptr [ebp + 8], cx
        sub    dword ptr [ebp + 30h], 18h
        xchg   edi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        mov    ecx, 6
        cld
        rep movsd
        mov    eax, 24h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        add    dword ptr [ebp + 30h], 18h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        cmp    al, 2
        jne    SHORT loc_11_0669
        mov    al, 3
loc_11_0669:
        mov    byte ptr [ebp + 1ch], al
        ret
loc_11_066D:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, 24h
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        je     SHORT loc_11_06D5
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    NEAR PTR loc_11_05E8
        db  08Bh, 035h, 0F4h, 001h, 000h, 000h
        test   esi, esi
        je     SHORT loc_11_06D5
        mov    edx, dword ptr [esi + edi]
        test   edx, edx
        je     SHORT loc_11_06D5
        mov    ax, word ptr [ebp + 1ch]
        mov    word ptr [edx + 2], ax
        mov    ax, word ptr [ebp]
        mov    word ptr [edx + 4], ax
        mov    ax, word ptr [ebp + 8]
        mov    word ptr [edx + 6], ax
        mov    ax, word ptr [ebp + 4]
        mov    word ptr [edx + 8], ax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    ah, byte ptr [eax + 1]
        mov    al, 0ffh
        mov    word ptr [edx], ax
loc_11_06D5:
        db  08Bh, 035h, 0D4h, 004h, 000h, 000h
        mov    ecx, edi
        lea    edi, [edi + 33h]
        cld
        movsb
        movsd
        movsd
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 30h
        xor    eax, eax
        mov    al, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 82h
        mov    al, 2ah
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        jecxz  loc_11_0725
        db  080h, 03Dh, 01Eh, 000h, 000h, 000h, 000h
        je     SHORT loc_11_0725
        mov    word ptr [ebp + 1ch], 5d07h
        xor    eax, eax
        mov    al, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_11_0725:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    edi, ecx
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 10h], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 18h], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 14h], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 4], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 8], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 3ch], ax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    word ptr [ebp + 38h], ax
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        movzx  eax, word ptr [edi + 34h]
        movzx  ecx, byte ptr [ebp + 1dh]
        push   eax
        push   ecx
        db  0CDh, 020h, 06Dh, 000h, 040h, 000h
        add    esp, 8
        mov    word ptr [ebp + 1ch], ax
        or     byte ptr [ebp + 2ch], 1
        ret
loc_11_07A9:
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        xchg   edi, eax
        jecxz  loc_11_07C2
        cmp    eax, edi
        jne    SHORT loc_11_07C2
        mov    byte ptr [ebp + 1ch], 3
        db  0CDh, 020h, 047h, 080h, 001h, 000h
loc_11_07C2:
        db  066h, 08Bh, 00Dh, 042h, 001h, 000h, 000h
        db  08Bh, 015h, 03Eh, 001h, 000h, 000h
        mov    word ptr [ebp + 28h], cx
        mov    dword ptr [ebp + 24h], edx
        ret
loc_11_07D7:
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        push   fs
        push   edx
        movzx  edx, byte ptr [ebp + 1ch]
        inc    edx
        call   sub_11_09D3
        jae    SHORT loc_11_07F6
        mov    ecx, 200h
loc_11_07F6:
        mov    ax, 3c10h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        push   ecx
        db  0CDh, 020h, 00Ah, 000h, 006h, 000h
        lea    eax, [ecx - 0ah]
        cmp    eax, 1000h
        jbe    SHORT loc_11_0816
        mov    eax, 1000h
loc_11_0816:
        xor    edx, edx
        pop    ecx
        div    ecx
        db  066h, 0A3h, 038h, 001h, 000h, 000h
        pop    eax
        push   dword ptr [ebp + 60h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 4]
        push   dword ptr [ebp]
        push   dword ptr [ebp + 8]
        cmp    word ptr [ebp + 18h], -1
        je     SHORT loc_11_0887
loc_11_0841:
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        db  066h, 08Bh, 015h, 038h, 001h, 000h, 000h
        cmp    dx, word ptr [ebp + 18h]
        jae    SHORT loc_11_085B
        mov    word ptr [ebp + 18h], dx
loc_11_085B:
        mov    edx, dword ptr [ebp + 18h]
        call   sub_11_0954
        jb     SHORT loc_11_087E
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 18h]
        pop    dword ptr [ebp + 1ch]
        add    word ptr [ebp + 14h], dx
        sub    word ptr [ebp + 18h], dx
        ja     SHORT loc_11_0841
        clc
        jmp    NEAR PTR loc_11_091C
loc_11_087E:
        add    esp, 0ch
        stc
        jmp    NEAR PTR loc_11_091C
loc_11_0887:
        push   ecx
        mov    ecx, 0ah
        stc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        pop    ecx
        jb     NEAR PTR loc_11_0945
        mov    word ptr [ebp + 10h], di
        movzx  edx, di
        shr    edi, 10h
        mov    word ptr [ebp + 60h], di
        shl    edi, 4
        add    edx, edi
        movzx  edi, word ptr fs:[esi + 4]
        push   fs
        push   esi
        push   eax
        mov    ax, word ptr fs:[esi + 8]
        movzx  esi, word ptr fs:[esi + 6]
        mov    fs, ax
        pop    eax
loc_11_08C5:
        push   eax
        db  066h, 0A1h, 038h, 001h, 000h, 000h
        cmp    ax, word ptr [edx + 4]
        jae    SHORT loc_11_08D6
        mov    word ptr [edx + 4], ax
loc_11_08D6:
        pop    eax
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 10h]
        mov    word ptr [ebp + 18h], 0ffffh
        call   sub_11_0954
        jb     SHORT loc_11_0905
        pop    dword ptr [ebp + 10h]
        pop    dword ptr [ebp + 1ch]
        push   eax
        movzx  eax, word ptr [edx + 4]
        sub    edi, eax
        je     SHORT loc_11_090A
        cmc
        jae    SHORT loc_11_090A
        add    dword ptr [edx], eax
        mov    word ptr [edx + 4], di
        pop    eax
        jmp    SHORT loc_11_08C5
loc_11_0905:
        add    esp, 8
        push   eax
        stc
loc_11_090A:
        pop    eax
        pop    esi
        pop    fs
        pushfd
        mov    ecx, 0ah
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        popfd
loc_11_091C:
        pop    dword ptr [ebp + 8]
        pop    dword ptr [ebp]
        pop    dword ptr [ebp + 4]
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 18h]
        pop    dword ptr [ebp + 10h]
        pop    eax
        pop    dword ptr [ebp + 60h]
        jb     SHORT loc_11_0937
        mov    dword ptr [ebp + 1ch], eax
loc_11_0937:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        pop    fs
        db  0CDh, 020h, 020h, 080h, 001h, 000h
loc_11_0945:
        or     word ptr [ebp + 2ch], 1
        mov    dword ptr [ebp + 1ch], 204h
        stc
        jmp    SHORT loc_11_091C
sub_11_0954:
        push   eax
        push   edi
        push   ecx
        push   eax
        push   edx
        mov    eax, ecx
        cmp    word ptr [ebp + 18h], -1
        jne    SHORT loc_11_0968
        mul    word ptr [edx + 4]
        jmp    SHORT loc_11_096C
loc_11_0968:
        mul    word ptr [ebp + 18h]
loc_11_096C:
        pop    edx
        mov    ecx, eax
        pop    eax
        bt     eax, 0
        cmc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     SHORT loc_11_09C4
        cmp    word ptr [ebp + 18h], -1
        jne    SHORT loc_11_0989
        mov    dword ptr [edx + 6], edi
        jmp    SHORT loc_11_0994
loc_11_0989:
        mov    word ptr [ebp + 10h], di
        shr    edi, 10h
        mov    word ptr [ebp + 60h], di
loc_11_0994:
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        add    word ptr [ebp + 30h], 2
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        bt     eax, 0
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_11_09C0
        add    esi, ecx
        clc
loc_11_09C0:
        pop    ecx
        pop    edi
        pop    eax
        ret
loc_11_09C4:
        or     word ptr [ebp + 2ch], 1
        mov    dword ptr [ebp + 1ch], 204h
        stc
        jmp    SHORT loc_11_09C0
sub_11_09D3:
        push   edi
        push   esi
        push   fs
        db  080h, 03Dh, 064h, 001h, 000h, 000h, 000h
        je     SHORT loc_11_09FE
        or     dl, dl
        je     SHORT loc_11_09FE
        db  0BEh, 044h, 001h, 000h, 000h
        cmp    byte ptr [esi], dl
        je     NEAR PTR loc_11_0A92
        db  0BEh, 054h, 001h, 000h, 000h
        cmp    byte ptr [esi], dl
        je     NEAR PTR loc_11_0A92
loc_11_09FE:
        mov    ecx, 28h
        mov    ax, ds
        mov    fs, ax
        xor    esi, esi
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     SHORT loc_11_0A8D
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        movzx  eax, di
        mov    word ptr [ebp + 14h], di
        shr    edi, 10h
        mov    word ptr [ebp + 3ch], di
        shl    edi, 4
        add    edi, eax
        mov    ax, 440dh
        xchg   word ptr [ebp + 1ch], ax
        mov    byte ptr [ebp + 10h], dl
        mov    word ptr [ebp + 18h], 860h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0E8h, 08Fh, 0F6h, 0F4h, 0FFh
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        test   byte ptr [ebp + 2ch], 1
        jne    SHORT loc_11_0A9D
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        movzx  eax, word ptr [edi + 14h]
        movzx  edx, word ptr [edi + 16h]
        movzx  esi, word ptr [edi + 7]
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        mov    ecx, esi
loc_11_0A8D:
        pop    fs
        pop    esi
        pop    edi
        ret
loc_11_0A92:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 0ch]
        jmp    SHORT loc_11_0A8D
loc_11_0A9D:
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        stc
        jmp    SHORT loc_11_0A8D
        db  000h, 000h
sub_11_0AB8:
        push   eax
        push   ecx
        db  08Bh, 00Dh, 07Ch, 001h, 000h, 000h
        db  0A1h, 080h, 001h, 000h, 000h
        jecxz  loc_11_0AC9
        mov    dword ptr [ecx], eax
loc_11_0AC9:
        pop    ecx
        pop    eax
        clc
        ret
DOSMGR_Add_Device:
        pushad
        mov    esi, eax
        shl    esi, 0ch
        rol    si, 4
        or     ebx, ebx
        jne    SHORT loc_11_0AE1
        db  0CDh, 020h, 003h, 000h, 001h, 000h
loc_11_0AE1:
        add    eax, dword ptr [ebx + 4]
        mov    edi, eax
        test   byte ptr [edi + 5], 80h
        stc
        je     SHORT loc_11_0B1B
        db  08Bh, 015h, 07Ch, 001h, 000h, 000h
        or     edx, edx
        jne    SHORT loc_11_0AFC
        db  0E8h, 018h, 007h, 0F5h, 0FFh
loc_11_0AFC:
        add    edx, dword ptr [ebx + 4]
        mov    eax, dword ptr [edx]
        cmp    ax, -1
        je     SHORT loc_11_0B16
        movzx  ecx, ax
        shr    eax, 10h
        shl    eax, 4
        add    eax, ecx
        mov    edx, eax
        jmp    SHORT loc_11_0AFC
loc_11_0B16:
        or     dword ptr [edi], 0ffffffffh
        mov    dword ptr [edx], esi
loc_11_0B1B:
        popad
        ret
DOSMGR_Remove_Device:
        pushad
        or     ebx, ebx
        je     SHORT loc_11_0B25
        add    eax, dword ptr [ebx + 4]
loc_11_0B25:
        mov    edi, eax
        test   byte ptr [edi + 5], 80h
        je     SHORT loc_11_0B8F
        db  08Bh, 015h, 07Ch, 001h, 000h, 000h
        or     edx, edx
        je     SHORT loc_11_0B8F
        or     ebx, ebx
        je     SHORT loc_11_0B3E
        add    edx, dword ptr [ebx + 4]
loc_11_0B3E:
        mov    eax, dword ptr [edx]
        cmp    ax, -1
        je     SHORT loc_11_0B8F
        movzx  ecx, ax
        shr    eax, 10h
        shl    eax, 4
        add    eax, ecx
        or     ebx, ebx
        je     SHORT loc_11_0B58
        add    eax, dword ptr [ebx + 4]
loc_11_0B58:
        cmp    eax, edi
        je     SHORT loc_11_0B60
        mov    edx, eax
        jmp    SHORT loc_11_0B3E
loc_11_0B60:
        mov    eax, dword ptr [edi]
        mov    dword ptr [edx], eax
        or     ebx, ebx
        jne    SHORT loc_11_0B8C
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        jmp    SHORT loc_11_0B7E
loc_11_0B70:
        mov    ecx, edi
        mov    esi, edx
        add    ecx, dword ptr [ebx + 4]
        add    esi, dword ptr [ebx + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
loc_11_0B7E:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_11_0B70
loc_11_0B8C:
        clc
loc_11_0B8D:
        popad
        ret
loc_11_0B8F:
        stc
        jmp    SHORT loc_11_0B8D
        db  000h, 000h
sub_11_0B94:
        mov    eax, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        xchg   edi, eax
        jecxz  loc_11_0BA6
        cmp    eax, edi
        jne    SHORT loc_11_0BA6
        mov    al, 3
        ret
loc_11_0BA6:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        lea    esi, [edx + 0ch]
        push   edi
        mov    edi, ebp
        mov    ecx, 8
        cld
        rep movsd
        add    edi, 18h
        add    esi, 18h
        movsd
        movsd
        pop    edi
        mov    ax, word ptr [ebp + 38h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 3ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 4]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 14h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 18h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 10h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 1ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [edx + 2]
        mov    word ptr [ebp + 1ch], ax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [ebp], ax
        movzx  ecx, word ptr [edx + 8]
        movzx  eax, word ptr [edx + 6]
        shl    eax, 4
        mov    dword ptr [ebp + 4], ecx
        add    ecx, 12h
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        mov    word ptr [ebp + 8], cx
        mov    eax, dword ptr [ebp + 2ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  066h, 08Bh, 00Dh, 042h, 001h, 000h, 000h
        db  08Bh, 015h, 03Eh, 001h, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        mov    al, byte ptr [ebp + 1ch]
        cmp    al, 1
        jne    SHORT loc_11_0C7E
        add    esp, 6ch
        ret
loc_11_0C7E:
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
        ret
loc_11_0C8A:
        push   fs
        push   eax
        push   esi
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 14h]
        mov    eax, 3c14h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        mov    byte ptr [ebp + 1dh], 2
loc_11_0CA3:
        mov    al, byte ptr fs:[esi]
        cmp    al, 24h
        je     SHORT loc_11_0CB5
        mov    byte ptr [ebp + 14h], al
        db  0E8h, 0FEh, 0F7h, 0F4h, 0FFh
        inc    esi
        jmp    SHORT loc_11_0CA3
loc_11_0CB5:
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 1ch]
        pop    esi
        pop    eax
        pop    fs
        ret
loc_11_0CC0:
        movzx  ecx, byte ptr fs:[esi]
        add    ecx, 2
        ret
loc_11_0CC8:
        pushad
        movzx  eax, byte ptr [edx - 6]
        movzx  edi, word ptr [eax + ebp + 24h]
        shl    edi, 4
        movzx  eax, byte ptr [edx - 7]
        movzx  eax, word ptr [eax + ebp]
        add    edi, eax
        push   100h
        push   0
        push   1
        push   edi
        push   edi
        db  0CDh, 020h, 074h, 000h, 040h, 000h
        add    esp, 14h
        popad
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_11_0CFA:
        push   edx
        db  0CDh, 020h, 006h, 000h, 006h, 000h
        pop    edx
        pushad
        test   byte ptr [ebp + 2ch], 1
        jne    SHORT loc_11_0D33
        movzx  eax, byte ptr [edx - 6]
        movzx  edi, word ptr [eax + ebp + 24h]
        shl    edi, 4
        movzx  eax, byte ptr [edx - 7]
        movzx  eax, word ptr [eax + ebp]
        add    edi, eax
        movzx  ecx, word ptr [edx - 9]
        push   ecx
        push   1
        push   0
        push   edi
        push   edi
        db  0CDh, 020h, 070h, 000h, 040h, 000h
        add    esp, 14h
loc_11_0D33:
        popad
        clc
        ret
loc_11_0D36:
        db  0BAh, 010h, 003h, 000h, 000h
        cmp    word ptr [ebp + 14h], -1
        je     SHORT loc_11_0D47
        db  0BAh, 07Dh, 004h, 000h, 000h
loc_11_0D47:
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_11_0D4D:
        db  0BAh, 033h, 004h, 000h, 000h
        cmp    byte ptr [ebp + 1ch], 0
        je     SHORT loc_11_0D5D
        db  0BAh, 070h, 004h, 000h, 000h
loc_11_0D5D:
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_11_0D63:
        mov    al, byte ptr [ebp + 1ch]
        cmp    al, 7
        db  0BAh, 033h, 004h, 000h, 000h
        jbe    SHORT loc_11_0D88
        and    al, 7fh
        cmp    al, 21h
        db  0BAh, 039h, 004h, 000h, 000h
        je     SHORT loc_11_0D88
        cmp    al, 22h
        db  0BAh, 058h, 003h, 000h, 000h
        je     SHORT loc_11_0D88
        db  0BAh, 010h, 003h, 000h, 000h
loc_11_0D88:
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_11_0D8E:
        test   word ptr [ebx], 180h
        jne    SHORT loc_11_0DA8
        movzx  edi, word ptr [ebp + 10h]
        mov    dword ptr [ebp + 10h], edi
        cmp    di, -1
        je     NEAR PTR loc_11_0E89
        jmp    SHORT loc_11_0DB4
loc_11_0DA8:
        mov    edi, dword ptr [ebp + 10h]
        cmp    edi, -1
        je     NEAR PTR loc_11_0E89
loc_11_0DB4:
        shl    edi, 4
        add    edi, 0ah
        shr    edi, 0ch
        inc    edi
        push   0
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   edi
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, edx
        je     NEAR PTR loc_11_0E89
        mov    dword ptr [edx], eax
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        mov    ecx, dword ptr [edi]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [edi], edx
        add    edx, 0ah
        mov    esi, edx
        mov    edi, dword ptr [ebp + 10h]
        shl    edi, 4
        dec    edi
        mov    ecx, edi
        call   sub_11_10C2
        mov    ecx, 1
        test   word ptr [ebx], 180h
        jne    SHORT loc_11_0E17
        mov    ecx, edi
        shr    ecx, 10h
        inc    ecx
loc_11_0E17:
        push   ecx
        push   0
        push   ecx
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        pop    ecx
        test   eax, eax
        je     SHORT loc_11_0E79
        mov    word ptr [ebp + 1ch], ax
        push   esi
        sub    esi, 0ah
        mov    word ptr [esi + 8], ax
        pop    esi
        and    edi, 0ffffh
loc_11_0E3F:
        dec    ecx
        je     SHORT loc_11_0E74
        add    eax, 8
        add    esi, 10000h
        push   ecx
        cmp    ecx, 1
        mov    ecx, edi
        je     SHORT loc_11_0E58
        mov    ecx, 0ffffh
loc_11_0E58:
        push   eax
        call   sub_11_10C2
        mov    ecx, dword ptr [esp]
        push   0
        push   eax
        push   edx
        push   ebx
        push   ecx
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        pop    eax
        pop    ecx
        jmp    SHORT loc_11_0E3F
loc_11_0E74:
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_11_0E79:
        sub    esi, 4
        push   0
        push   dword ptr [esi]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_11_0E89:
        push   0
        db  0CDh, 020h, 059h, 000h, 001h, 000h
        add    esp, 4
        test   eax, eax
        je     SHORT loc_11_0EB1
        shl    eax, 8
        sub    eax, 1
        test   word ptr [ebx], 180h
        jne    SHORT loc_11_0EB1
        cmp    eax, 0ffffh
        jb     SHORT loc_11_0EB1
        mov    eax, 0fffeh
loc_11_0EB1:
        mov    dword ptr [ebp + 10h], eax
        mov    word ptr [ebp + 1ch], 8
        or     byte ptr [ebp + 2ch], 1
        ret
loc_11_0EBF:
        xor    edi, edi
        xchg   word ptr [ebp + 38h], di
        test   di, 4
        je     SHORT loc_11_0F26
sub_11_0ECC:
        lea    esi, [ebp + 3ch]
        mov    ecx, 3
loc_11_0ED4:
        cmp    word ptr [esi], di
        jne    SHORT loc_11_0EDE
        mov    word ptr [esi], 0
loc_11_0EDE:
        add    esi, 4
        loop   loc_11_0ED4
        call   sub_11_1072
        jb     SHORT loc_11_0F26
        push   0
        push   dword ptr [edx]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_11_0F26
        mov    esi, 1
        test   word ptr [ebx], 180h
        jne    SHORT loc_11_0F0E
        lsl    esi, edi
        shr    esi, 10h
        inc    esi
loc_11_0F0E:
        push   0
        push   edi
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
        add    edi, 8
        dec    esi
        jne    SHORT loc_11_0F0E
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_11_0F26:
        mov    word ptr [ebp + 1ch], 9
        or     byte ptr [ebp + 2ch], 1
        ret
loc_11_0F31:
        movzx  edi, word ptr [ebp + 38h]
        call   sub_11_1072
        jb     NEAR PTR loc_11_102C
        mov    esi, eax
        test   word ptr [ebx], 180h
        jne    SHORT loc_11_0F83
        lsl    ecx, edi
        shr    ecx, 10h
        movzx  edx, word ptr [ebp + 10h]
        mov    dword ptr [ebp + 10h], edx
        shr    edx, 0ch
        push   ecx
        sub    ecx, edx
        pop    edx
        jb     NEAR PTR loc_11_102C
        je     SHORT loc_11_0F83
        lea    edx, [edi + edx*8]
loc_11_0F68:
        push   edx
        push   0
        push   edx
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        je     NEAR PTR loc_11_102C
        sub    edx, 8
        dec    ecx
        jne    SHORT loc_11_0F68
loc_11_0F83:
        sub    esi, 0ah
        mov    ecx, dword ptr [ebp + 10h]
        shl    ecx, 4
        push   ecx
        add    ecx, 0ah
        shr    ecx, 0ch
        inc    ecx
        push   0
        push   ecx
        push   dword ptr [esi]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, edx
        pop    ecx
        je     NEAR PTR loc_11_102C
        dec    ecx
        mov    dword ptr [edx], eax
        push   edi
        push   ecx
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        mov    ecx, dword ptr [edi]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [edi], edx
        pop    ecx
        pop    edi
        add    edx, 0ah
        mov    esi, edx
        push   ecx
        call   sub_11_10C2
        push   0
        push   eax
        push   edx
        push   ebx
        push   edi
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        pop    ecx
        test   word ptr [ebx], 180h
        jne    SHORT loc_11_1027
        shr    ecx, 10h
        je     SHORT loc_11_1027
loc_11_0FEC:
        add    edi, 8
        add    esi, 10000h
        push   ecx
        cmp    ecx, 1
        je     SHORT loc_11_1002
        mov    ecx, 0ffffh
        jmp    SHORT loc_11_100F
loc_11_1002:
        mov    ecx, dword ptr [ebp + 10h]
        shl    ecx, 4
        dec    ecx
        and    ecx, 0ffffh
loc_11_100F:
        call   sub_11_10C2
        push   0
        push   eax
        push   edx
        push   ebx
        push   edi
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        pop    ecx
        dec    ecx
        jne    SHORT loc_11_0FEC
loc_11_1027:
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_11_102C:
        push   0
        db  0CDh, 020h, 05Ah, 000h, 001h, 000h
        add    esp, 4
        shl    eax, 8
        test   word ptr [ebx], 180h
        jne    SHORT loc_11_1064
        cmp    eax, 0ffffh
        jb     SHORT loc_11_104D
        mov    eax, 0fffeh
loc_11_104D:
        test   word ptr [ebx], 180h
        jne    SHORT loc_11_1064
        lsl    ecx, edi
        mov    cx, 0ffffh
        shr    ecx, 4
        cmp    ecx, eax
        ja     SHORT loc_11_1064
        mov    eax, ecx
loc_11_1064:
        mov    dword ptr [ebp + 10h], eax
        mov    word ptr [ebp + 1ch], 8
        or     byte ptr [ebp + 2ch], 1
        ret
sub_11_1072:
        push   0
        push   edi
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        push   eax
        sub    eax, 0ah
        mov    edx, eax
        push   edi
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        mov    eax, edi
        pop    edi
        db  003h, 005h, 004h, 000h, 000h, 000h
        cmp    dword ptr [eax], 0
        je     SHORT loc_11_10BF
        cmp    dword ptr [eax], edx
        je     SHORT loc_11_10AE
        mov    eax, dword ptr [eax]
loc_11_10A0:
        cmp    dword ptr [eax + 4], edx
        je     SHORT loc_11_10B6
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_11_10A0
        jmp    SHORT loc_11_10BF
loc_11_10AE:
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        clc
        jmp    SHORT loc_11_10BD
loc_11_10B6:
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], ecx
        clc
loc_11_10BD:
        pop    eax
        ret
loc_11_10BF:
        stc
        jmp    SHORT loc_11_10BD
sub_11_10C2:
        cmp    ecx, 0fffffh
        ja     SHORT loc_11_10DF
        push   0
        push   0
        push   92h
        push   ecx
        push   esi
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        ret
loc_11_10DF:
        add    ecx, 0fffh
        shr    ecx, 0ch
        push   0
        push   80h
        push   92h
        push   ecx
        push   esi
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        ret
loc_11_1100:
        mov    ecx, 13h
        db  080h, 03Dh, 0A4h, 001h, 000h, 000h, 004h
        jb     SHORT loc_11_1111
        add    ecx, 6
loc_11_1111:
        ret
loc_11_1112:
        cmp    byte ptr [ebp + 1ch], 0
        jne    SHORT loc_11_111A
        jmp    SHORT loc_11_1123
loc_11_111A:
        or     byte ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1ch], 1
        ret
loc_11_1123:
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        push   0
        xchg   esi, eax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        push   esi
        mov    ax, 3810h
        push   fs
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        test   word ptr [ebx], 180h
        jne    SHORT loc_11_1164
        mov    eax, dword ptr fs:[esi + 2]
        movzx  esi, ax
        shr    eax, 10h
        mov    fs, ax
        jmp    SHORT loc_11_1168
loc_11_1164:
        lfs    esi, ptr fs:[esi]
loc_11_1168:
        push   0
        push   fs
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    fs
        add    eax, esi
        push   eax
        push   ebx
        db  0CDh, 020h, 001h, 000h, 015h, 000h
        add    esp, 10h
        db  08Bh, 015h, 010h, 000h, 000h, 000h
        lea    eax, [edx + 0b0h]
        shl    eax, 0ch
        shr    ax, 0ch
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 60h], ax
        lea    eax, [edx + 20h]
        shl    eax, 0ch
        shr    ax, 0ch
        mov    word ptr [ebp + 10h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 5ch], ax
        db  0E8h, 0EEh, 0F0h, 0F4h, 0FFh
        db  0E8h, 0EEh, 0F2h, 0F4h, 0FFh
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        push   fs
        mov    fs, word ptr [edi + 2ah]
        mov    word ptr fs:[2ch], ax
        pop    fs
        xor    eax, eax
        mov    dword ptr [ebx + 3ch], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp + 2ch]
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        mov    word ptr [ebp + 1ch], ax
        and    byte ptr [ebp + 2ch], 0feh
        and    ecx, 1
        je     SHORT loc_11_1201
        or     dword ptr [ebp + 2ch], ecx
loc_11_1201:
        popad
        ret
DOSMGR_Translate_Server_DOS_Call:
        sub    esp, 84h
        pushad
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        xor    ecx, ecx
        mov    dword ptr [esp + 20h], ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_11_1253
        call   sub_11_1391
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_11_137E
        push   esi
        db  0BEh, 010h, 000h, 000h, 000h
        mov    eax, 10084h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        pop    esi
loc_11_1253:
        push   16h
        pop    ecx
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jae    SHORT loc_11_1261
        xor    ecx, ecx
loc_11_1261:
        cmp    ecx, 16h
        jb     NEAR PTR loc_11_1353
        mov    dword ptr [esp + 30h], edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 08Ah, 000h, 001h, 000h
        db  0CDh, 020h, 08Bh, 000h, 001h, 000h
        or     byte ptr [ebx + 1], 1
        mov    byte ptr [ebp + 1dh], 2fh
        xor    eax, eax
        mov    al, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        push   dword ptr [ebp + 38h]
        push   dword ptr [ebp + 10h]
        mov    edx, dword ptr [esi + 20h]
        mov    dword ptr [ebp + 14h], edx
        mov    word ptr [ebp + 3ch], ds
        mov    byte ptr [ebp + 1dh], 1ah
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cld
        lodsd
        mov    dword ptr [ebp + 1ch], eax
        mov    dword ptr [esp + 2ch], eax
        lodsd
        mov    dword ptr [ebp + 10h], eax
        lodsd
        mov    dword ptr [ebp + 18h], eax
        lodsd
        mov    dword ptr [ebp + 14h], eax
        lodsd
        mov    dword ptr [ebp + 4], eax
        lodsd
        mov    dword ptr [ebp], eax
        db  0CDh, 020h, 06Dh, 000h, 001h, 000h
        mov    word ptr [ebp + 3ch], ax
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebp + 40h], ax
        mov    word ptr [ebp + 44h], ax
        lodsd
        mov    dword ptr [esp + 30h], eax
        lodsd
        mov    dword ptr [esp + 34h], eax
        db  0C7h, 044h, 024h, 03Ch, 065h, 000h, 000h, 000h
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  089h, 03Dh, 004h, 000h, 000h, 000h
        lea    eax, [esp + 2ch]
        mov    dword ptr [ebp + 8], eax
        db  0E8h, 0A9h, 0EEh, 0F4h, 0FFh
        inc    dword ptr [esp + 28h]
        db  0FFh, 005h, 004h, 000h, 000h, 000h
        inc    dword ptr [esp + 3ch]
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [ebp + 8], eax
        lea    edi, [esp + 40h]
        mov    esi, edi
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 3ch]
        mov    byte ptr [ebp + 1dh], 1ah
        xor    eax, eax
        mov    al, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        db  0CDh, 020h, 08Ch, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   16h
        pop    ecx
loc_11_1353:
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        db  0BEh, 010h, 000h, 000h, 000h
        mov    eax, 10084h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        jb     SHORT loc_11_137E
        call   sub_11_1391
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_11_137E:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        popad
        cmp    dword ptr [esp], 1
        lea    esp, [esp + 84h]
        ret
sub_11_1391:
        db  0B8h, 000h, 000h, 000h, 000h
        db  0B9h, 06Bh, 000h, 000h, 000h
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        inc    ecx
        ret
loc_11_13A5:
        mov    ecx, dword ptr [esi + 0ch]
        mov    eax, ecx
        shr    eax, 0ch
        movzx  edi, cx
        and    al, 0f0h
        add    edi, eax
        add    edi, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebp + 1ch]
        stosw
        mov    eax, dword ptr [ebp + 10h]
        stosw
        mov    eax, dword ptr [ebp + 18h]
        stosw
        mov    eax, dword ptr [ebp + 14h]
        stosw
        mov    eax, dword ptr [ebp + 4]
        stosw
        mov    eax, dword ptr [ebp]
        stosw
        mov    eax, dword ptr [ebp + 3ch]
        stosw
        mov    eax, dword ptr [ebp + 38h]
        stosw
        xor    eax, eax
        stosw
        mov    eax, dword ptr [esi + 8]
        stosd
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp + 8], eax
        mov    word ptr [ebp + 14h], cx
        shr    ecx, 10h
        mov    word ptr [ebp + 3ch], cx
        mov    word ptr [ebp + 1ch], 5d00h
        xor    eax, eax
        mov    al, 21h
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp + 8], esi
        mov    dword ptr [esi + 4], eax
        popad
        ret
_PTEXT ends
        end
