; msmouse_pcode_03.asm -- MSMOUSE object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        call   sub_3_0020
        jae    SHORT loc_3_0019
        db  08Bh, 00Dh, 0C8h, 004h, 000h, 000h
        db  08Bh, 015h, 0CCh, 004h, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_3_0019:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        ret
sub_3_0020:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 1ah
        jne    SHORT loc_3_006D
        movzx  eax, word ptr [ebp + 10h]
        test   eax, 800h
        je     SHORT loc_3_0050
        and    ax, 0ffh
        mov    word ptr [ebp + 10h], ax
        movzx  eax, word ptr [ebp + 14h]
        db  0A3h, 035h, 005h, 000h, 000h
        mov    eax, 64h
        mov    word ptr [ebp + 14h], ax
        jmp    SHORT loc_3_009A
loc_3_0050:
        movzx  eax, word ptr [ebp + 10h]
        db  03Bh, 005h, 035h, 005h, 000h, 000h
        jne    SHORT loc_3_0064
        xor    eax, eax
        db  0A3h, 035h, 005h, 000h, 000h
        ret
loc_3_0064:
        xor    eax, eax
        db  0A3h, 035h, 005h, 000h, 000h
        jmp    SHORT loc_3_009A
loc_3_006D:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 3ah
        jne    SHORT loc_3_007C
        db  0E8h, 021h, 003h, 0FDh, 0FFh
        ret
loc_3_007C:
        movzx  eax, word ptr [ebp + 1ch]
        sub    eax, 2bh
        jb     SHORT loc_3_009A
        cmp    eax, 0bh
        jae    SHORT loc_3_009A
        mov    edx, ebx
        db  003h, 015h, 0D0h, 004h, 000h, 000h
        db  0FFh, 014h, 085h, 001h, 000h, 000h, 000h
        ret
loc_3_009A:
        stc
        ret
loc_3_009C:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 36h
        jne    SHORT loc_3_00AC
        db  0E8h, 07Eh, 002h, 0FDh, 0FFh
        jmp    SHORT loc_3_00E5
loc_3_00AC:
        cmp    eax, 37h
        jne    SHORT loc_3_00B8
        db  0E8h, 0F7h, 001h, 0FDh, 0FFh
        jmp    SHORT loc_3_00E5
loc_3_00B8:
        cmp    eax, 102h
        jne    SHORT loc_3_00CA
        mov    word ptr [ebp + 1ch], 0
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_3_00CA:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 4
        jne    SHORT loc_3_00E5
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_3_00E5
        push   ebx
        mov    ebx, dword ptr [ebp + 10h]
        call   sub_3_01A6
        pop    ebx
loc_3_00E5:
        db  0FFh, 025h, 0DCh, 004h, 000h, 000h
loc_3_00EB:
        or     edx, edx
        je     SHORT loc_3_00F4
        cmp    edx, 0ch
        jne    SHORT loc_3_00F9
loc_3_00F4:
        call   sub_3_01A6
loc_3_00F9:
        clc
        ret
loc_3_00FB:
        mov    edx, ebx
        db  003h, 015h, 0D0h, 004h, 000h, 000h
        mov    edi, ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    ebx, edi
        je     SHORT loc_3_0148
        mov    edi, dword ptr [edi + 4]
        db  003h, 03Dh, 0D4h, 004h, 000h, 000h
        db  0BEh, 08Ch, 000h, 000h, 000h
        mov    ecx, 51h
        cld
        rep movsd
        db  0BEh, 0D0h, 001h, 000h, 000h
        mov    ecx, 24h
        rep movsd
        mov    edi, edx
        mov    esi, ebx
        db  003h, 035h, 0D0h, 004h, 000h, 000h
        mov    ecx, 1ch
        rep movsb
        mov    byte ptr [edx + 0ch], 1
        clc
        ret
loc_3_0148:
        call   sub_3_046D
        mov    byte ptr [edx + 0ch], 3
        call   sub_3_01B4
        mov    edi, dword ptr [ebx + 4]
        db  003h, 03Dh, 0D4h, 004h, 000h, 000h
        add    edi, 144h
        db  0BEh, 0D0h, 001h, 000h, 000h
        mov    ecx, 24h
        rep movsd
        xor    eax, eax
        mov    byte ptr [edx], al
        mov    byte ptr [edx + 1], al
        mov    byte ptr [edx + 2], al
        mov    byte ptr [edx + 3], al
        mov    word ptr [edx + 4], ax
        mov    word ptr [edx + 6], ax
        mov    word ptr [edx + 8], ax
        mov    word ptr [edx + 0ah], 0ffh
        mov    byte ptr [edx + 0dh], al
        mov    byte ptr [edx + 0eh], al
        mov    byte ptr [edx + 0fh], 0ch
        mov    byte ptr [edx + 10h], 10h
        mov    byte ptr [edx + 11h], 20h
        mov    byte ptr [edx + 12h], al
        ret
sub_3_01A6:
        db  03Bh, 01Dh, 0C4h, 004h, 000h, 000h
        je     SHORT loc_3_01D3
        db  089h, 01Dh, 0C4h, 004h, 000h, 000h
sub_3_01B4:
        call   sub_3_0484
        xor    eax, eax
        db  066h, 0A3h, 0EAh, 004h, 000h, 000h
        db  066h, 0A3h, 0ECh, 004h, 000h, 000h
        db  066h, 0A3h, 0EEh, 004h, 000h, 000h
        db  066h, 0A3h, 0F0h, 004h, 000h, 000h
loc_3_01D3:
        ret
loc_3_01D4:
        clc
        ret
loc_3_01D6:
        movsx  eax, word ptr [ebp + 10h]
        cmp    eax, -1
        jne    SHORT loc_3_01EB
        call   sub_3_046D
        call   sub_3_0484
        jmp    SHORT loc_3_0216
loc_3_01EB:
        dec    eax
        cmp    eax, 4
        jae    SHORT loc_3_021C
        mov    byte ptr [edx + 0ch], al
        xchg   esi, eax
        mov    ax, 3804h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        db  08Bh, 03Dh, 0D4h, 004h, 000h, 000h
        add    edi, dword ptr [ebx + 4]
        mov    ecx, 51h
        cld
        rep movsd
        call   sub_3_0484
loc_3_0216:
        mov    byte ptr [ebp + 1ch], 0
        clc
        ret
loc_3_021C:
        mov    byte ptr [ebp + 1ch], 0ffh
        clc
        ret
loc_3_0222:
        xor    eax, eax
        mov    word ptr [ebp + 1ch], ax
        mov    al, byte ptr [edx + 0ch]
        inc    eax
        mov    word ptr [ebp + 10h], ax
        db  0A1h, 0D4h, 004h, 000h, 000h
        add    eax, dword ptr [ebx + 4]
        mov    ecx, 143h
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        mov    word ptr [ebp + 38h], cx
        mov    word ptr [ebp + 4], dx
        clc
        ret
loc_3_024D:
        xor    ecx, ecx
        movzx  eax, byte ptr [edx + 0ch]
        movsx  esi, word ptr [ebp + 10h]
        cmp    esi, -1
        je     SHORT loc_3_0273
        dec    esi
        dec    ecx
        dec    ecx
        cmp    esi, 4
        jae    SHORT loc_3_0273
        xchg   esi, eax
        mov    byte ptr [edx + 0ch], al
        call   sub_3_0484
        xor    ecx, ecx
        movzx  eax, byte ptr [edx + 0ch]
loc_3_0273:
        inc    eax
        mov    word ptr [ebp + 1ch], cx
        mov    word ptr [ebp + 10h], ax
        dec    eax
        shl    eax, 4
        db  003h, 005h, 0D4h, 004h, 000h, 000h
        lea    eax, [eax + 104h]
        mov    ecx, 0fh
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        mov    word ptr [ebp + 38h], cx
        mov    word ptr [ebp + 4], dx
        clc
        ret
loc_3_02A1:
        xor    eax, eax
        dec    eax
        mov    word ptr [ebp + 1ch], ax
        ret
loc_3_02A9:
        movzx  ecx, word ptr [ebp + 18h]
        jecxz  loc_3_02E2
        mov    al, 3ch
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_02BC
        mov    al, 30h
loc_3_02BC:
        and    ch, al
        je     SHORT loc_3_02D9
        and    cl, al
        je     SHORT loc_3_02D9
        test   ch, cl
        jne    SHORT loc_3_02D9
        mov    word ptr [edx + 10h], cx
        or     cl, ch
        xor    al, cl
        mov    byte ptr [edx + 0fh], al
        mov    al, byte ptr [ebp + 14h]
        mov    byte ptr [edx + 0dh], al
loc_3_02D9:
        movsx  eax, word ptr [ebp + 10h]
        call   sub_3_0418
loc_3_02E2:
        mov    al, byte ptr [edx + 12h]
        mov    bx, word ptr [edx + 6]
        mov    cx, word ptr [edx + 10h]
        mov    dl, byte ptr [edx + 0dh]
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_0303
        xchg   edx, ebx
        mov    bh, bl
        mov    bl, al
        mov    ax, 0ffffh
loc_3_0303:
        cbw
        mov    word ptr [ebp + 1ch], ax
        mov    word ptr [ebp + 10h], bx
        mov    word ptr [ebp + 18h], cx
        mov    word ptr [ebp + 14h], dx
        clc
        ret
loc_3_0317:
        xor    eax, eax
        mov    word ptr [ebp + 1ch], ax
        mov    word ptr [ebp + 10h], ax
        mov    eax, 400h
        mov    word ptr [ebp + 18h], ax
        mov    word ptr [ebp + 14h], ax
        ret
loc_3_032F:
        mov    word ptr [ebp + 1ch], 0ffbfh
        mov    word ptr [ebp + 10h], 8000h
        xor    eax, eax
        mov    word ptr [ebp + 14h], ax
        mov    word ptr [ebp + 18h], ax
        ret
loc_3_0346:
        xor    eax, eax
        mov    word ptr [ebp + 1ch], ax
        mov    ecx, 15ah
        xchg   word ptr [ebp + 18h], cx
        or     ecx, ecx
        jne    SHORT loc_3_035A
        ret
loc_3_035A:
        xchg   edi, eax
        mov    ax, 3814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cld
        stosb
        stosb
        mov    ax, 32h
        stosb
        stosb
        xor    eax, eax
        stosb
        mov    al, byte ptr [edx + 0ch]
        inc    eax
        stosb
        xor    eax, eax
        stosb
        stosb
        stosb
        stosb
        stosb
        mov    ax, word ptr [edx + 6]
        stosw
        mov    al, byte ptr [edx + 11h]
        stosb
        mov    al, byte ptr [edx + 10h]
        stosb
        mov    al, byte ptr [edx + 0dh]
        stosb
        db  08Bh, 035h, 0D4h, 004h, 000h, 000h
        add    esi, dword ptr [ebx + 4]
        mov    ecx, 51h
        rep movsd
        mov    al, byte ptr [edx]
        stosb
        mov    al, byte ptr [edx + 1]
        stosb
        mov    al, byte ptr [edx + 2]
        stosb
        mov    al, byte ptr [edx + 3]
        stosb
        mov    al, byte ptr [edx + 4]
        stosb
        xor    eax, eax
        stosb
        ret
loc_3_03B7:
        db  0A1h, 0D4h, 004h, 000h, 000h
        add    eax, dword ptr [ebx + 4]
        add    eax, 144h
        mov    ecx, 8fh
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        mov    word ptr [ebp + 38h], cx
        mov    word ptr [ebp + 14h], dx
        xor    ax, ax
        clc
        ret
loc_3_03DC:
        movsx  eax, word ptr [ebp + 10h]
        cmp    eax, -1
        jne    SHORT loc_3_03FC
        mov    eax, dword ptr [edx]
        mov    word ptr [ebp + 10h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [ebp + 14h], ax
        jmp    SHORT loc_3_0411
loc_3_03FC:
        mov    ax, word ptr [ebp + 18h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 10h]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [ebp + 14h]
        mov    word ptr [edx + 4], ax
loc_3_0411:
        xor    eax, eax
        mov    word ptr [ebp + 1ch], ax
        ret
sub_3_0418:
        pushad
        mov    edi, edx
        mov    ebx, 168h
        cdq
        idiv   ebx
        xchg   edx, eax
        or     eax, eax
        jns    SHORT loc_3_042A
        add    eax, ebx
loc_3_042A:
        mov    word ptr [edi + 6], ax
        mov    bx, 5ah
        idiv   bl
        sub    bl, ah
        db  00Fh, 0B6h, 08Bh, 031h, 000h, 000h, 000h
        mov    bl, ah
        db  00Fh, 0B6h, 09Bh, 031h, 000h, 000h, 000h
        movzx  eax, al
        test   al, 1
        je     SHORT loc_3_044F
        xchg   ecx, ebx
loc_3_044F:
        db  08Ah, 080h, 02Dh, 000h, 000h, 000h
        shr    al, 1
        jae    SHORT loc_3_045B
        neg    ebx
loc_3_045B:
        shr    al, 1
        jae    SHORT loc_3_0461
        neg    ecx
loc_3_0461:
        cli
        mov    word ptr [edi + 8], bx
        mov    word ptr [edi + 0ah], cx
        sti
        popad
        ret
sub_3_046D:
        db  0BEh, 08Ch, 000h, 000h, 000h
        db  08Bh, 03Dh, 0D4h, 004h, 000h, 000h
        add    edi, dword ptr [ebx + 4]
        mov    ecx, 51h
        cld
        rep movsd
        ret
sub_3_0484:
        db  03Bh, 01Dh, 0C4h, 004h, 000h, 000h
        jne    SHORT loc_3_04CE
        mov    edx, ebx
        db  003h, 015h, 0D0h, 004h, 000h, 000h
        movzx  eax, byte ptr [edx + 0ch]
        db  08Bh, 035h, 0D4h, 004h, 000h, 000h
        add    esi, dword ptr [ebx + 4]
        movzx  ecx, byte ptr [eax + esi]
        cli
        db  088h, 00Dh, 0F4h, 004h, 000h, 000h
        push   ecx
        push   esi
        shl    eax, 5
        db  0BFh, 0F5h, 004h, 000h, 000h
        lea    esi, [eax + esi + 4]
        cld
        rep movsb
        pop    esi
        pop    ecx
        db  0BFh, 015h, 005h, 000h, 000h
        lea    esi, [eax + esi + 84h]
        rep movsb
        sti
loc_3_04CE:
        ret
        db  000h
loc_3_04D0:
        mov    eax, 0ffffffffh
        cmp    ecx, 3
        jae    SHORT loc_3_04E6
        shl    ecx, 2
        db  0FFh, 091h, 084h, 002h, 000h, 000h
loc_3_04E6:
        clc
        ret
loc_3_04E8:
        mov    ecx, 100h
        xor    eax, eax
        ret
sub_3_04F0:
        mov    eax, 0ffffffffh
        ret
loc_3_04F6:
        mov    eax, 0ffffffffh
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_3_056E
        cmp    dword ptr [esi + 14h], 10h
        jne    SHORT loc_3_056E
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_3_056E
        cmp    dword ptr [esi + 1ch], 16h
        jne    SHORT loc_3_056E
        cmp    dword ptr [esi + 20h], 0
        je     SHORT loc_3_056E
        mov    edi, dword ptr [esi + 20h]
        or     edi, edi
        je     SHORT loc_3_056E
        mov    dword ptr [edi], 16h
        mov    edi, dword ptr [esi + 18h]
        mov    esi, dword ptr [esi + 10h]
        mov    word ptr [edi], 0
        movzx  ecx, word ptr [esi + 2]
        xor    ebx, ebx
        xor    edx, edx
        test   ecx, 1
        je     SHORT loc_3_0546
        mov    edx, 4
loc_3_0546:
        or     ecx, ecx
        je     SHORT loc_3_056C
        inc    ebx
        shr    ecx, 1
        jae    SHORT loc_3_0546
        push   edi
        push   esi
        push   ebx
        push   ecx
        push   edx
        shl    ebx, 3
        add    ebx, edx
        db  0FFh, 093h, 090h, 002h, 000h, 000h
        pop    edx
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        jae    SHORT loc_3_0546
        or     ax, bx
        mov    word ptr [edi], ax
loc_3_056C:
        xor    eax, eax
loc_3_056E:
        ret
loc_3_056F:
        mov    ax, 900h
        stc
        ret
        db  066h, 0B8h, 000h, 003h, 0F9h, 0C3h
loc_3_057B:
        mov    ax, 100h
        stc
        ret
loc_3_0581:
        push   ecx
        push   edx
        db  0CDh, 020h, 007h, 000h, 00Ch, 000h
        shr    ax, 8
        and    ax, 0fh
        or     cx, cx
        je     SHORT loc_3_059A
        or     ax, 2000h
loc_3_059A:
        mov    word ptr [edi + 4], ax
        pop    edx
        pop    ecx
        xor    ax, ax
        clc
        ret
loc_3_05A5:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  003h, 01Dh, 0D0h, 004h, 000h, 000h
        cli
        mov    al, byte ptr [ebx + 12h]
        push   ecx
        mov    bl, al
        and    al, 0ch
        mov    cl, 4
        shr    bl, cl
        or     al, bl
        mov    byte ptr [edi + 6], al
        pop    ecx
        sti
        pop    ebx
        xor    ax, ax
        clc
        ret
loc_3_05CC:
        cli
        db  066h, 0A1h, 047h, 005h, 000h, 000h
        mov    word ptr [edi + 8], ax
        db  066h, 0A1h, 049h, 005h, 000h, 000h
        mov    word ptr [edi + 0ah], ax
        xor    ax, ax
        db  066h, 0A3h, 047h, 005h, 000h, 000h
        db  066h, 0A3h, 049h, 005h, 000h, 000h
        sti
        xor    ax, ax
        clc
        ret
loc_3_05F6:
        mov    ax, 0a0ah
        mov    word ptr [edi + 0ch], ax
        xor    ax, ax
        clc
        ret
loc_3_0603:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  003h, 01Dh, 0D0h, 004h, 000h, 000h
        cli
        mov    ax, word ptr [ebx + 6]
        mov    word ptr [edi + 0eh], ax
        sti
        pop    ebx
        xor    ax, ax
        clc
        ret
loc_3_0620:
        push   ebx
        push   ecx
        push   edx
        mov    ax, 30h
        mov    cx, 0
        push   33h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        push   ecx
        mov    al, ch
        mov    bl, al
        and    al, 0ch
        mov    cl, 4
        shr    bl, cl
        or     al, bl
        mov    byte ptr [edi + 10h], al
        mov    ch, al
        pop    ecx
        mov    al, cl
        mov    bl, al
        and    al, 0ch
        mov    cl, 4
        shr    bl, cl
        or     al, bl
        mov    byte ptr [edi + 11h], al
        pop    edx
        pop    ecx
        pop    ebx
        xor    ax, ax
        clc
        ret
loc_3_065D:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  003h, 01Dh, 0D0h, 004h, 000h, 000h
        cli
        mov    al, byte ptr [ebx + 0dh]
        mov    byte ptr [edi + 12h], al
        sti
        pop    ebx
        xor    ax, ax
        clc
        ret
loc_3_0678:
        push   ebx
        push   ecx
        push   edx
        mov    ax, 1bh
        push   33h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    ax, bx
        inc    ax
        inc    ax
        mov    dl, 5
        idiv   dl
        and    ax, 0ffh
        mov    byte ptr [edi + 13h], al
        mov    ax, cx
        inc    ax
        inc    ax
        mov    dl, 5
        idiv   dl
        and    ax, 0ffh
        mov    byte ptr [edi + 14h], al
        pop    edx
        pop    ecx
        pop    ebx
        xor    ax, ax
        clc
        ret
loc_3_06B3:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  003h, 01Dh, 0D0h, 004h, 000h, 000h
        cli
        mov    al, byte ptr [ebx + 0ch]
        mov    byte ptr [edi + 15h], al
        sti
        pop    ebx
        xor    ax, ax
        clc
        ret
loc_3_06CE:
        clc
        ret
loc_3_06D0:
        push   ebx
        push   ecx
        push   edx
        mov    ax, 30h
        mov    cx, 0
        push   33h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    ax, -1
        jne    SHORT loc_3_06EB
        mov    dl, bh
loc_3_06EB:
        mov    bx, word ptr [esi + 4]
        mov    ax, 30h
        push   33h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    ax, -1
        jne    SHORT loc_3_0707
        mov    word ptr [edi + 0eh], dx
        jmp    SHORT loc_3_070B
loc_3_0707:
        mov    word ptr [edi + 0eh], bx
loc_3_070B:
        pop    edx
        pop    ecx
        pop    ebx
        xor    ax, ax
        clc
        ret
loc_3_0713:
        push   ebx
        push   ecx
        push   edx
        mov    ax, 30h
        mov    cx, 0
        push   33h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        push   eax
        push   ebx
        mov    al, byte ptr [esi + 6]
        mov    bl, al
        and    al, 3
        and    bl, 0ch
        mov    cl, 4
        shl    al, cl
        or     al, bl
        mov    ch, al
        mov    al, byte ptr [esi + 7]
        mov    bl, al
        and    al, 3
        and    bl, 0ch
        mov    cl, 4
        shl    al, cl
        or     al, bl
        mov    cl, al
        pop    ebx
        pop    eax
        cmp    ax, -1
        jne    SHORT loc_3_075E
        mov    al, bh
        xchg   dx, bx
        xor    dx, dx
        mov    dl, al
loc_3_075E:
        mov    ax, 30h
        push   33h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edx
        pop    ecx
        pop    ebx
        xor    ax, ax
        clc
        ret
loc_3_0772:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  003h, 01Dh, 0D0h, 004h, 000h, 000h
        cli
        mov    al, byte ptr [esi + 8]
        mov    byte ptr [ebx + 0dh], al
        cmp    al, 21h
        jne    SHORT loc_3_0791
        db  0C6h, 005h, 0F2h, 004h, 000h, 000h, 002h
loc_3_0791:
        sti
        pop    ebx
        xor    ax, ax
        clc
        ret
loc_3_0798:
        push   ebx
        push   ecx
        push   edx
        mov    ax, 1bh
        push   33h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        push   dx
        mov    al, byte ptr [esi + 9]
        mov    dl, 5
        imul   dl
        and    ax, 0ffh
        mov    bx, ax
        mov    al, byte ptr [esi + 0ah]
        mov    dl, 5
        imul   dl
        and    ax, 0ffh
        mov    cx, ax
        pop    dx
        or     bx, 800h
        xor    ax, ax
        mov    al, byte ptr [esi + 9]
        mov    dx, ax
        mov    ax, 1ah
        push   33h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edx
        pop    ecx
        pop    ebx
        xor    ax, ax
        clc
        ret
loc_3_07E9:
        mov    al, byte ptr [esi + 0bh]
        cmp    al, 20h
        jne    SHORT loc_3_082D
        mov    ax, word ptr [esi + 0ch]
        cmp    ax, 0
        je     SHORT loc_3_0812
        db  066h, 0A3h, 0DCh, 004h, 000h, 000h
        mov    ax, word ptr [esi + 0eh]
        cmp    ax, 0
        je     SHORT loc_3_0812
        db  066h, 0A3h, 0DEh, 004h, 000h, 000h
        jmp    SHORT loc_3_0824
loc_3_0812:
        db  066h, 0C7h, 005h, 0DCh, 004h, 000h, 000h, 080h, 002h
        db  066h, 0C7h, 005h, 0DEh, 004h, 000h, 000h, 0E0h, 001h
loc_3_0824:
        cli
        call   sub_3_0848
        sti
        jmp    SHORT loc_3_0843
loc_3_082D:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  003h, 01Dh, 0D0h, 004h, 000h, 000h
        cli
        mov    al, byte ptr [esi + 0bh]
        mov    byte ptr [ebx + 0ch], al
        sti
        pop    ebx
loc_3_0843:
        xor    ax, ax
        clc
        ret
sub_3_0848:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        db  00Fh, 0B7h, 005h, 0DCh, 004h, 000h, 000h
        db  00Fh, 0B7h, 01Dh, 0DEh, 004h, 000h, 000h
        add    ax, bx
        shr    ax, 1
        xchg   bx, ax
        db  0BEh, 08Ch, 000h, 000h, 000h
        db  0BFh, 098h, 003h, 000h, 000h
        mov    ecx, 41h
        rep movsd
        db  0BEh, 018h, 003h, 000h, 000h
        db  0B9h, 010h, 001h, 000h, 000h
        db  0B8h, 090h, 000h, 000h, 000h
        sub    ecx, eax
        db  0BFh, 09Ch, 003h, 000h, 000h
        db  066h, 081h, 03Dh, 0DCh, 004h, 000h, 000h, 080h, 002h
        jne    SHORT loc_3_08A0
        db  066h, 081h, 03Dh, 0DEh, 004h, 000h, 000h, 0E0h, 001h
        je     SHORT loc_3_08C1
loc_3_08A0:
        xor    eax, eax
        mov    al, byte ptr [esi]
        cmp    al, 7fh
        je     SHORT loc_3_08B5
        xor    edx, edx
        push   ecx
        mov    ecx, 230h
        imul   ecx
        idiv   ebx
        pop    ecx
loc_3_08B5:
        jne    SHORT loc_3_08B9
        inc    al
loc_3_08B9:
        mov    byte ptr [edi], al
        inc    esi
        inc    edi
        loop   loc_3_08A0
        jmp    SHORT loc_3_08C3
loc_3_08C1:
        rep movsb
loc_3_08C3:
        db  0BEh, 098h, 003h, 000h, 000h
        db  08Bh, 03Dh, 0D4h, 004h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        add    edi, dword ptr [ebx + 4]
        mov    ecx, 41h
        cld
        rep movsd
        call   sub_3_0484
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
_PTEXT ends
        end
