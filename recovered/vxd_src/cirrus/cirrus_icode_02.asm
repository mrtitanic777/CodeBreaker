; cirrus_icode_02.asm -- CIRRUS object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        push   ebx
        db  089h, 01Dh, 0D0h, 000h, 000h, 000h
        db  0C6h, 005h, 0FDh, 000h, 000h, 000h, 000h
        push   0
        push   10000h
        push   0a0000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_02F6
        db  0A3h, 0E0h, 000h, 000h, 000h
        call   sub_2_0433
        or     ebx, ebx
        jne    SHORT loc_2_007E
        call   sub_2_03D1
        or     ebx, ebx
        je     NEAR PTR loc_2_02F6
        mov    dx, 3d4h
        mov    ax, 25h
        out    dx, al
        in     ax, dx
        db  088h, 025h, 0F0h, 000h, 000h, 000h
        db  0C7h, 005h, 0F1h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        test   ebx, 20000h
        je     SHORT loc_2_007E
        cmp    ah, 0c6h
        je     SHORT loc_2_0074
        cmp    ah, 68h
        ja     SHORT loc_2_007E
loc_2_0074:
        db  0C7h, 005h, 0F1h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_007E:
        db  089h, 01Dh, 0ECh, 000h, 000h, 000h
        mov    ax, 1200h
        mov    bl, 85h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        movzx  eax, al
        shl    eax, 10h
        db  0A3h, 0DCh, 000h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 00Ah, 000h
        db  089h, 035h, 0D4h, 000h, 000h, 000h
        db  089h, 03Dh, 0D8h, 000h, 000h, 000h
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_0254
        push   0
        push   57h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 0E4h, 000h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_02F6
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_02F6
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 004h, 048h, 000h, 000h, 000h
        db  0C7h, 047h, 008h, 001h, 001h, 000h, 000h
        db  0C7h, 047h, 00Ch, 085h, 001h, 000h, 000h
        db  0C7h, 047h, 058h, 01Fh, 002h, 000h, 000h
        db  0C7h, 047h, 05Ch, 091h, 002h, 000h, 000h
        db  0C7h, 047h, 010h, 0C9h, 002h, 000h, 000h
        db  0C7h, 047h, 014h, 024h, 003h, 000h, 000h
        db  0C7h, 047h, 018h, 045h, 003h, 000h, 000h
        db  0C7h, 047h, 01Ch, 04Dh, 003h, 000h, 000h
        db  0C7h, 047h, 020h, 055h, 003h, 000h, 000h
        db  0C7h, 047h, 024h, 0B1h, 004h, 000h, 000h
        db  0C7h, 047h, 068h, 038h, 004h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 022h, 00Dh, 000h, 000h
        db  0C7h, 087h, 080h, 000h, 000h, 000h, 02Ah, 00Dh, 000h, 000h
        db  0C7h, 087h, 084h, 000h, 000h, 000h, 044h, 00Dh, 000h, 000h
        db  0C7h, 087h, 094h, 000h, 000h, 000h, 06Fh, 00Dh, 000h, 000h
        db  0C7h, 087h, 088h, 000h, 000h, 000h, 0E0h, 00Dh, 000h, 000h
        db  0C7h, 087h, 09Ch, 000h, 000h, 000h, 087h, 00Dh, 000h, 000h
        db  0C7h, 087h, 0A0h, 000h, 000h, 000h, 0BEh, 00Dh, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 092h, 004h, 000h, 000h
        db  0C7h, 047h, 04Ch, 041h, 009h, 000h, 000h
        db  0C7h, 047h, 048h, 027h, 00Ah, 000h, 000h
        db  0C7h, 047h, 070h, 0A8h, 000h, 000h, 000h
        db  0C7h, 047h, 074h, 09Dh, 003h, 000h, 000h
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 0F0h, 0E3h, 01Fh, 000h
        je     SHORT loc_2_01C4
        db  0C7h, 047h, 03Ch, 088h, 008h, 000h, 000h
loc_2_01C4:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 0C0h, 003h, 01Fh, 000h
        je     SHORT loc_2_01DE
        db  0C7h, 047h, 02Ch, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 030h, 020h, 000h, 000h, 000h
loc_2_01DE:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 0E0h, 001h, 000h
        je     SHORT loc_2_0206
        db  0C7h, 047h, 024h, 00Bh, 006h, 000h, 000h
        db  0C7h, 047h, 044h, 054h, 00Bh, 000h, 000h
        db  0C7h, 047h, 078h, 07Dh, 00Bh, 000h, 000h
        db  0C7h, 047h, 07Ch, 024h, 00Ch, 000h, 000h
loc_2_0206:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 00Ch, 000h, 000h
        je     SHORT loc_2_0220
        db  0C7h, 047h, 044h, 0EAh, 00Ah, 000h, 000h
        db  0C7h, 047h, 040h, 027h, 00Bh, 000h, 000h
loc_2_0220:
        db  0F7h, 005h, 0F1h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_2_0242
        mov    edx, 3c2h
        db  0B9h, 022h, 009h, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        db  089h, 00Dh, 0F5h, 000h, 000h, 000h
loc_2_0242:
        mov    edx, 3ceh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        jmp    NEAR PTR loc_2_02ED
loc_2_0254:
        push   0
        push   5
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 0E4h, 000h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_02F6
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     SHORT loc_2_02F6
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 004h, 048h, 000h, 000h, 000h
        db  0C7h, 047h, 008h, 040h, 001h, 000h, 000h
        db  0C7h, 047h, 00Ch, 0D2h, 001h, 000h, 000h
        db  0C7h, 047h, 058h, 055h, 002h, 000h, 000h
        db  0C7h, 047h, 05Ch, 0ADh, 002h, 000h, 000h
        db  0C7h, 047h, 010h, 0C9h, 002h, 000h, 000h
        db  0C7h, 047h, 014h, 024h, 003h, 000h, 000h
        db  0C7h, 047h, 018h, 045h, 003h, 000h, 000h
        db  0C7h, 047h, 01Ch, 04Dh, 003h, 000h, 000h
        db  0C7h, 047h, 020h, 059h, 004h, 000h, 000h
        db  0C7h, 047h, 024h, 0B9h, 007h, 000h, 000h
        db  0C7h, 047h, 050h, 0BCh, 00Ch, 000h, 000h
        db  0C7h, 047h, 054h, 0F2h, 00Ch, 000h, 000h
        db  0C7h, 047h, 068h, 038h, 004h, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 092h, 004h, 000h, 000h
        jmp    SHORT loc_2_02ED
loc_2_02ED:
        db  08Bh, 01Dh, 0D0h, 000h, 000h, 000h
        clc
        jmp    SHORT loc_2_02F7
loc_2_02F6:
        stc
loc_2_02F7:
        pop    ebx
        ret
loc_2_02F9:
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        adc    eax, 10000000h
        add    byte ptr [eax], al
        add    byte ptr [esi], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [edx], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [ecx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [ebx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    byte ptr [ebx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [esi], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [ecx], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 430000h], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_2_03B9:
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
sub_2_03D1:
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 6
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        mov    al, 12h
        out    dx, al
        in     al, dx
        cmp    al, 12h
        jne    SHORT loc_2_041E
        not    al
        out    dx, al
        in     al, dx
        cmp    al, 0fh
        jne    SHORT loc_2_041E
        mov    al, 12h
        out    dx, al
        mov    ax, 1200h
        mov    bl, 80h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  08Dh, 01Dh, 0F9h, 002h, 000h, 000h
loc_2_040C:
        mov    ecx, dword ptr [ebx]
        and    ecx, ecx
        je     SHORT loc_2_041E
        add    ebx, 8
        cmp    al, cl
        jne    SHORT loc_2_040C
        mov    ebx, dword ptr [ebx - 4]
        jmp    SHORT loc_2_0420
loc_2_041E:
        xor    ebx, ebx
loc_2_0420:
        mov    edx, 3c4h
        mov    al, 6
        out    dx, al
        inc    edx
        rol    eax, 8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        ret
sub_2_0433:
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0ah
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        mov    al, 0ceh
        out    dx, al
        in     al, dx
        cmp    al, 0
        jne    SHORT loc_2_047D
        mov    al, 0ech
        out    dx, al
        in     al, dx
        cmp    al, 1
        jne    SHORT loc_2_047D
        mov    ax, 1200h
        mov    bl, 80h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  08Dh, 01Dh, 0B9h, 003h, 000h, 000h
loc_2_046B:
        mov    ecx, dword ptr [ebx]
        and    ecx, ecx
        je     SHORT loc_2_047D
        add    ebx, 8
        cmp    al, cl
        jne    SHORT loc_2_046B
        mov    ebx, dword ptr [ebx - 4]
        jmp    SHORT loc_2_047F
loc_2_047D:
        xor    ebx, ebx
loc_2_047F:
        mov    edx, 3ceh
        mov    al, 0ah
        out    dx, al
        inc    edx
        rol    eax, 8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        ret
loc_2_0492:
        db  0A1h, 0ECh, 000h, 000h, 000h
        ret
_ITEXT ends
        end
