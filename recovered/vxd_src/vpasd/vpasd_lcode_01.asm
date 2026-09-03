; vpasd_lcode_01.asm -- VPASD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  083h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  085h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  084h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  086h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
loc_1_0040:
        sal    byte ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0050:
        sal    dword ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 003h, 032h, 004h, 000h, 000h, 000h, 076h, 070h, 061h, 073h
        db  064h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0BCh
        db  0BDh, 0BEh, 0BFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00FB:
        add    eax, 0
        add    byte ptr [eax], al
        db  000h
loc_1_0103:
        and    byte ptr [edx], al
        add    byte ptr [eax], al
loc_1_0107:
        pop    es
        add    byte ptr [eax], al
        db  000h
loc_1_010B:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_010F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0113:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 200h], al
        add    byte ptr [0], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, cl
        int3
loc_1_0144:
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        or     byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    al, 20h
        add    byte ptr [eax + ebp], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + esi], ch
        xor    al, 0
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        mov    eax, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        ret    0c200h
        db  0C2h, 000h, 000h, 000h, 000h, 0C2h, 000h, 0C2h, 0C2h, 000h, 000h, 000h, 000h, 0C2h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0C2h, 000h, 000h, 000h, 020h, 020h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h
        db  0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 000h, 000h, 000h, 000h, 0C2h, 0C2h, 0C2h
        db  0C2h, 000h, 000h, 000h, 000h, 0C2h, 0C2h, 0C2h, 0C2h, 002h, 000h, 000h, 000h, 0C2h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0C2h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
vpasd_Control:
        cmp    eax, 0
        je     SHORT loc_1_03EA
        cmp    eax, 7
        je     NEAR PTR sub_1_0456
        cmp    eax, 0eh
        je     NEAR PTR loc_1_078C
        cmp    eax, 0dh
        je     NEAR PTR loc_1_0780
        cmp    eax, 6
        je     SHORT loc_1_03EC
        cmp    eax, 0ch
        je     SHORT loc_1_03FB
        cmp    eax, 0bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_03C3:
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_03CC:
        cmp    eax, 1ch
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_03D5:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_03DE:
        cmp    eax, 9
        je     SHORT loc_1_03FD
        cmp    eax, 3
        je     SHORT sub_1_03FE
        clc
        ret
loc_1_03EA:
        clc
        ret
loc_1_03EC:
        push   esi
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        call   sub_1_074B
        pop    esi
        clc
        ret
loc_1_03FB:
        clc
        ret
loc_1_03FD:
        ret
sub_1_03FE:
        push   esi
        push   eax
        push   edx
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 0BFh
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h
        jne    SHORT loc_1_044D
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        jne    SHORT loc_1_044D
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    dx, 0b8bh
        in     al, dx
        test   al, 2
        je     SHORT loc_1_0449
        mov    dx, 0f788h
        in     al, dx
        test   al, 2
        je     SHORT loc_1_0449
        mov    dx, 0f789h
        in     al, dx
        test   al, 0fh
        je     SHORT loc_1_0449
        mov    byte ptr [esi + 2eh], 0ffh
        jmp    SHORT loc_1_044D
loc_1_0449:
        mov    byte ptr [esi + 2eh], 0
loc_1_044D:
        call   sub_1_0456
        pop    edx
        pop    eax
        pop    esi
        ret
sub_1_0456:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        test   byte ptr [esi + 44h], 1
        jne    SHORT loc_1_046A
        call   sub_1_0548
loc_1_046A:
        pop    esi
        clc
        ret
        db  0F8h, 0C3h, 090h
sub_1_0470:
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0480
        cmp    ebx, eax
        je     SHORT loc_1_048A
        jmp    SHORT loc_1_048D
loc_1_0480:
        call   sub_1_0617
        call   sub_1_065E
loc_1_048A:
        pop    eax
        clc
        ret
loc_1_048D:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        test   dword ptr [esi + 46h], 2
        jne    SHORT loc_1_04C1
        or     dword ptr [esi + 46h], 2
        pushad
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        mov    eax, 30h
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
        popad
loc_1_04C1:
        pop    esi
        pop    eax
        stc
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_04C8:
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_04D8
        cmp    ebx, eax
        je     SHORT loc_1_050D
        jmp    SHORT loc_1_0510
loc_1_04D8:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_04E5
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_04E5:
        db  087h, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_0502
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_1_0502
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0502:
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_050D:
        pop    eax
        clc
        ret
loc_1_0510:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        test   dword ptr [esi + 46h], 1
        jne    SHORT loc_1_0544
        or     dword ptr [esi + 46h], 1
        pushad
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        mov    eax, 30h
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
        popad
loc_1_0544:
        pop    esi
        pop    eax
        stc
        ret
sub_1_0548:
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 014h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 18h], 7fh
        mov    byte ptr [esi + 19h], 0aah
        mov    byte ptr [esi + 1ah], 0ffh
        mov    byte ptr [esi + 1bh], 0
        mov    byte ptr [esi + 1ch], 0aah
        mov    byte ptr [esi + 1dh], 0
        mov    byte ptr [esi + 1eh], 0
        mov    byte ptr [esi + 1fh], 0
        mov    byte ptr [esi + 20h], 0ffh
        mov    byte ptr [esi + 21h], 0ffh
        mov    word ptr [esi + 22h], 0
        mov    byte ptr [esi + 24h], 36h
        mov    byte ptr [esi + 25h], 74h
        mov    word ptr [esi + 26h], 6ch
        mov    word ptr [esi + 28h], 800h
        mov    byte ptr [esi + 2ah], 24h
        mov    byte ptr [esi + 2bh], 9
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h
        je     SHORT loc_1_05B7
        mov    byte ptr [esi + 2bh], 0
loc_1_05B7:
        mov    byte ptr [esi + 2ch], 29h
        mov    dx, 0b8bh
        in     al, dx
        and    al, 0f3h
        mov    byte ptr [esi + 2dh], al
        mov    byte ptr [esi + 44h], 5
        mov    byte ptr [esi + 45h], 0
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h
        jne    SHORT loc_1_0616
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        jne    SHORT loc_1_05FD
        push   esi
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 2eh]
        pop    ebx
        pop    esi
        mov    byte ptr [esi + 2eh], al
loc_1_05FD:
        mov    byte ptr [esi + 21h], 0
        mov    byte ptr [esi + 1bh], 0
        push   esi
        push   edi
        cld
        lea    edi, [esi + 38h]
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        movsd
        movsd
        movsd
        pop    edi
        pop    esi
loc_1_0616:
        ret
sub_1_0617:
        push   esi
        push   edi
        push   eax
        push   ebx
        push   edx
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_1_0658
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 2ah]
        and    al, 0bfh
        mov    edx, 0b8ah
        out    dx, al
        mov    dx, 0b89h
        out    dx, al
        mov    edx, 0b8bh
        in     al, dx
        mov    byte ptr [esi + 2dh], al
        and    al, 0e7h
        out    dx, al
        push   edi
        lea    edi, [esi + 38h]
        call   sub_1_0716
        pop    edi
loc_1_0658:
        pop    edx
        pop    ebx
        pop    eax
        pop    edi
        pop    esi
        ret
sub_1_065E:
        push   esi
        push   edi
        push   edx
        push   ebx
        push   eax
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        or     ebx, ebx
        jne    SHORT loc_1_0673
        db  0CDh, 020h, 003h, 000h, 001h, 000h
loc_1_0673:
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        test   byte ptr [esi + 44h], 1
        jne    SHORT loc_1_0686
        call   sub_1_0456
loc_1_0686:
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        jne    SHORT loc_1_069A
        mov    al, byte ptr [esi + 2eh]
        call   sub_1_138C
loc_1_069A:
        push   esi
        lea    esi, [esi + 38h]
        call   sub_1_074B
        pop    esi
        mov    ax, word ptr [esi + 26h]
        call   sub_1_108C
        mov    ax, word ptr [esi + 28h]
        call   sub_1_10A5
        mov    al, byte ptr [esi + 2bh]
        mov    edx, 0f8ah
        out    dx, al
        mov    al, 0ffh
        xchg   byte ptr [esi + 2ch], al
        call   sub_1_10E6
        mov    edx, 0b8bh
        mov    al, byte ptr [esi + 2dh]
        out    dx, al
        mov    al, byte ptr [esi + 2ah]
        mov    edx, 0b8ah
        out    dx, al
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_0710
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    dl, 0eh
        in     al, dx
        test   byte ptr [esi + 44h], 30h
        je     SHORT loc_1_0710
        test   byte ptr [esi + 44h], 4
        jne    SHORT loc_1_0710
        and    byte ptr [esi + 44h], 0cfh
        or     byte ptr [esi + 44h], 8
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_0710:
        pop    eax
        pop    ebx
        pop    edx
        pop    edi
        pop    esi
        ret
sub_1_0716:
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_074A
        cld
        mov    edx, 8388h
        in     al, dx
        stosb
        inc    edx
        in     al, dx
        stosb
        inc    edx
        in     al, dx
        stosb
        inc    edx
        in     al, dx
        stosb
        mov    edx, 0f388h
        in     al, dx
        stosb
        inc    edx
        in     al, dx
        stosb
        inc    edx
        in     al, dx
        stosb
        inc    edx
        in     al, dx
        stosb
        mov    edx, 0bf8ah
        in     al, dx
        stosb
loc_1_074A:
        ret
sub_1_074B:
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_077F
        cld
        mov    edx, 8388h
        lodsb
        out    dx, al
        inc    edx
        lodsb
        out    dx, al
        inc    edx
        lodsb
        out    dx, al
        inc    edx
        lodsb
        out    dx, al
        mov    edx, 0f388h
        lodsb
        out    dx, al
        inc    edx
        lodsb
        out    dx, al
        inc    edx
        lodsb
        out    dx, al
        inc    edx
        lodsb
        out    dx, al
        mov    edx, 0bf8ah
        lodsb
        out    dx, al
loc_1_077F:
        ret
loc_1_0780:
        push   ecx
        mov    cx, 0
        call   sub_1_0798
        pop    ecx
        ret
loc_1_078C:
        push   ecx
        mov    cx, 1
        call   sub_1_0798
        pop    ecx
        ret
sub_1_0798:
        push   eax
        push   ebx
        mov    ax, 0bc0fh
        mov    bx, 5
        push   2fh
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebx
        pop    eax
        ret
loc_1_07B0:
        push   esi
        push   edx
        push   ebx
        push   eax
        mov    edx, 0b89h
        in     al, dx
        test   al, 1fh
        je     SHORT loc_1_080B
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jae    SHORT loc_1_07D4
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        jmp    SHORT loc_1_080B
loc_1_07D4:
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        test   byte ptr [esi + 44h], 30h
        je     SHORT loc_1_07FF
        test   byte ptr [esi + 44h], 20h
        je     SHORT loc_1_07FF
        and    byte ptr [esi + 44h], 0dfh
        and    byte ptr [esi + 2ah], 0bfh
        mov    al, byte ptr [esi + 2ah]
        mov    edx, 0b8ah
        out    dx, al
        mov    edx, 0b89h
        out    dx, al
loc_1_07FF:
        pop    eax
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        pop    ebx
        pop    edx
        pop    esi
        clc
        ret
loc_1_080B:
        pop    eax
        pop    ebx
        pop    edx
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        pop    esi
        clc
        ret
loc_1_0817:
        ret
loc_1_0818:
        push   ebx
        push   esi
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0849
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        test   byte ptr [esi + 44h], 8
        jne    SHORT loc_1_0842
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_1_0842:
        and    byte ptr [esi + 44h], 0f7h
        pop    esi
        pop    ebx
        ret
loc_1_0849:
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 44h], 0f7h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        pop    esi
        pop    ebx
        ret
loc_1_085E:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 44h], 0fbh
        jecxz  loc_1_0871
        or     byte ptr [esi + 44h], 4
loc_1_0871:
        pop    esi
        ret
loc_1_0873:
        ret
loc_1_0874:
        cmp    ecx, 4
        jbe    SHORT loc_1_087F
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_087F:
        je     SHORT loc_1_0884
        mov    al, 0ffh
        ret
loc_1_0884:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0894
        call   sub_1_0470
        jae    SHORT loc_1_0894
        ret
loc_1_0894:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        test   al, 1
        je     SHORT loc_1_08FC
        call   sub_1_0DEB
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 014h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 19h], 0aah
        or     byte ptr [esi + 1ah], 80h
        and    byte ptr [esi + 18h], 7fh
        mov    byte ptr [esi + 1bh], 0
        mov    byte ptr [esi + 20h], 0ffh
        mov    byte ptr [esi + 21h], 0ffh
        mov    al, 9
        mov    byte ptr [esi + 2bh], al
        and    byte ptr [esi + 44h], 0c5h
        mov    byte ptr [esi + 2ch], 0ffh
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_08FC
        mov    al, byte ptr [esi + 2bh]
        mov    dx, 0f8ah
        out    dx, al
        mov    dx, 0b89h
        out    dx, al
        sub    eax, eax
        call   sub_1_10E6
loc_1_08FC:
        pop    esi
        ret
loc_1_08FE:
        cmp    ecx, 4
        jbe    SHORT loc_1_0909
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0909:
        je     SHORT loc_1_092E
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 18h]
        test   byte ptr [esi + 2bh], 80h
        je     SHORT loc_1_0923
        test   byte ptr [esi + 2ah], 40h
        jne    SHORT loc_1_0925
loc_1_0923:
        pop    esi
        ret
loc_1_0925:
        xor    byte ptr [esi + 1fh], 80h
        xor    al, byte ptr [esi + 1fh]
        pop    esi
        ret
loc_1_092E:
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_0941
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 001h
loc_1_0941:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        or     byte ptr [esi + 18h], 80h
        test   byte ptr [esi + 44h], 20h
        je     SHORT loc_1_095D
        cmp    al, 0d0h
        je     SHORT loc_1_095D
        call   sub_1_0DEB
loc_1_095D:
        call   dword ptr [esi + 0ch]
        pop    esi
        ret
loc_1_0962:
        cmp    ecx, 4
        jbe    SHORT loc_1_096D
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_096D:
        je     SHORT loc_1_098A
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 19h]
        and    byte ptr [esi + 1ah], 7fh
        cmp    byte ptr [esi + 1bh], 0
        je     SHORT loc_1_0988
        call   dword ptr [esi + 10h]
loc_1_0988:
        pop    esi
        ret
loc_1_098A:
        ret
loc_1_098B:
        cmp    ecx, 4
        jbe    SHORT loc_1_0996
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0996:
        je     SHORT loc_1_09B7
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        cmp    byte ptr [esi + 1dh], 0
        je     SHORT loc_1_09B0
        dec    byte ptr [esi + 1dh]
        jne    SHORT loc_1_09B0
        or     byte ptr [esi + 1ah], 80h
loc_1_09B0:
        call   dword ptr [esi + 14h]
        mov    al, byte ptr [esi + 1ah]
        pop    esi
loc_1_09B7:
        ret
loc_1_09B8:
        cmp    ecx, 4
        jbe    SHORT loc_1_09C3
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_09C3:
        je     SHORT loc_1_09C7
        mov    al, 0ffh
loc_1_09C7:
        ret
loc_1_09C8:
        cmp    ecx, 4
        jbe    SHORT loc_1_09D3
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_09D3:
        je     SHORT loc_1_09D7
        mov    al, 0ffh
loc_1_09D7:
        ret
loc_1_09D8:
        cmp    ecx, 4
        jbe    SHORT loc_1_09E3
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_09E3:
        je     SHORT loc_1_09F3
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 2ah]
        pop    esi
        ret
loc_1_09F3:
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_0A16
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_1_0A16
        mov    al, 0ah
        call   sub_1_10E6
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
loc_1_0A16:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0A26
        call   sub_1_0470
        jae    SHORT loc_1_0A26
        ret
loc_1_0A26:
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_0A47
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_1_0A47
        call   sub_1_1302
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
loc_1_0A47:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 2ah], al
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0A62
        call   sub_1_0470
        jb     SHORT loc_1_0A63
loc_1_0A62:
        out    dx, al
loc_1_0A63:
        pop    esi
        ret
loc_1_0A65:
        cmp    ecx, 4
        jbe    SHORT loc_1_0A70
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0A70:
        je     SHORT loc_1_0A80
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 2dh]
        pop    esi
        ret
loc_1_0A80:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0A90
        call   sub_1_0470
        jae    SHORT loc_1_0A90
        ret
loc_1_0A90:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 44h], 0efh
        test   al, 1fh
        je     SHORT loc_1_0AAF
        mov    ah, al
        xor    ah, 1fh
        test   ah, 1fh
        je     SHORT loc_1_0AAF
        or     byte ptr [esi + 44h], 10h
loc_1_0AAF:
        and    byte ptr [esi + 2dh], 0e0h
        and    al, 1fh
        or     byte ptr [esi + 2dh], al
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0AC1
        out    dx, al
loc_1_0AC1:
        pop    esi
        ret
loc_1_0AC3:
        cmp    ecx, 4
        jbe    SHORT loc_1_0ACE
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0ACE:
        je     SHORT loc_1_0ADE
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 2bh]
        pop    esi
        ret
loc_1_0ADE:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_0AF6
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0AF6
        call   sub_1_0470
        jae    SHORT loc_1_0AF6
        ret
loc_1_0AF6:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h
        je     SHORT loc_1_0B0D
        and    al, 0f0h
loc_1_0B0D:
        mov    byte ptr [esi + 2bh], al
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0B19
        out    dx, al
loc_1_0B19:
        pop    esi
        ret
loc_1_0B1B:
        cmp    ecx, 4
        jbe    SHORT loc_1_0B26
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0B26:
        je     SHORT loc_1_0B2A
        in     al, dx
        ret
loc_1_0B2A:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        test   byte ptr [esi + 45h], 1
        jne    SHORT loc_1_0B41
        btc    word ptr [esi + 24h], 7
        jb     SHORT loc_1_0B4A
loc_1_0B41:
        mov    byte ptr [esi + 26h], al
        mov    byte ptr [esi + 27h], 0
        jmp    SHORT loc_1_0B4D
loc_1_0B4A:
        mov    byte ptr [esi + 27h], al
loc_1_0B4D:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0B56
        out    dx, al
loc_1_0B56:
        pop    esi
        ret
loc_1_0B58:
        cmp    ecx, 4
        jbe    SHORT loc_1_0B63
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0B63:
        je     SHORT loc_1_0B67
        in     al, dx
        ret
loc_1_0B67:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        test   byte ptr [esi + 45h], 1
        jne    SHORT loc_1_0B7E
        btc    word ptr [esi + 25h], 7
        jb     SHORT loc_1_0B87
loc_1_0B7E:
        mov    byte ptr [esi + 28h], al
        mov    byte ptr [esi + 29h], 0
        jmp    SHORT loc_1_0B8A
loc_1_0B87:
        mov    byte ptr [esi + 29h], al
loc_1_0B8A:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0B93
        out    dx, al
loc_1_0B93:
        pop    esi
        ret
loc_1_0B95:
        cmp    ecx, 4
        jbe    SHORT loc_1_0BA0
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0BA0:
        je     SHORT loc_1_0BA4
        in     al, dx
        ret
loc_1_0BA4:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0BB6
        out    dx, al
loc_1_0BB6:
        mov    ah, al
        and    ah, 0c0h
        je     SHORT loc_1_0BD7
        cmp    ah, 40h
        je     SHORT loc_1_0BC4
        pop    esi
        ret
loc_1_0BC4:
        mov    byte ptr [esi + 25h], al
        not    al
        or     byte ptr [esi + 45h], 2
        test   al, 30h
        jne    SHORT loc_1_0BD5
        xor    byte ptr [esi + 45h], 2
loc_1_0BD5:
        pop    esi
        ret
loc_1_0BD7:
        mov    byte ptr [esi + 24h], al
        not    al
        or     byte ptr [esi + 45h], 1
        test   al, 30h
        jne    SHORT loc_1_0BE8
        xor    byte ptr [esi + 45h], 1
loc_1_0BE8:
        pop    esi
        ret
loc_1_0BEA:
        cmp    ecx, 4
        jbe    SHORT loc_1_0BF5
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0BF5:
        je     SHORT loc_1_0BFA
        in     al, dx
loc_1_0BF8:
        clc
        ret
loc_1_0BFA:
        jmp    SHORT loc_1_0BF8
loc_1_0BFC:
        cmp    ecx, 4
        jbe    SHORT loc_1_0C07
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0C07:
        je     SHORT loc_1_0C17
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 38h]
        pop    esi
        ret
loc_1_0C17:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0C27
        call   sub_1_0470
        jae    SHORT loc_1_0C27
        ret
loc_1_0C27:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 38h], al
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0C3C
        out    dx, al
loc_1_0C3C:
        pop    esi
        ret
loc_1_0C3E:
        cmp    ecx, 4
        jbe    SHORT loc_1_0C49
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0C49:
        je     SHORT loc_1_0C59
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 40h]
        pop    esi
        ret
loc_1_0C59:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0C69
        call   sub_1_0470
        jae    SHORT loc_1_0C69
        ret
loc_1_0C69:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 40h], al
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0C7E
        out    dx, al
loc_1_0C7E:
        pop    esi
        ret
loc_1_0C80:
        xchg   ebx, edx
        db  08Dh, 01Dh, 000h, 000h, 000h, 000h
        xlatb
        movzx  eax, al
        db  08Dh, 01Dh, 000h, 000h, 000h, 000h
        xchg   ebx, edx
        call   word ptr [eax + edx]
        ret
loc_1_0C99:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0CA9
        call   sub_1_0470
        jae    SHORT loc_1_0CA9
        ret
loc_1_0CA9:
        mov    al, 10h
        call   sub_1_0F53
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0CBC:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0CC9
        mov    dx, 0f88h
        out    dx, al
loc_1_0CC9:
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
loc_1_0CD0:
        and    byte ptr [esi + 18h], 7fh
loc_1_0CD4:
        ret
loc_1_0CD5:
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0CF0
        call   sub_1_0470
        jae    SHORT loc_1_0CF0
        ret
loc_1_0CF0:
        ret
loc_1_0CF1:
        mov    byte ptr [esi + 28h], al
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0D00:
        mov    byte ptr [esi + 29h], al
        call   sub_1_0FC3
        call   sub_1_1007
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0D19:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0D29
        call   sub_1_0470
        jae    SHORT loc_1_0D29
        ret
loc_1_0D29:
        mov    al, 0
        call   sub_1_0F53
        mov    dx, 0f88h
        in     al, dx
        mov    byte ptr [esi + 19h], al
        or     byte ptr [esi + 1ah], 80h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0D41:
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0D5C
        call   sub_1_0470
        jae    SHORT loc_1_0D5C
        ret
loc_1_0D5C:
        ret
loc_1_0D5D:
        mov    byte ptr [esi + 28h], al
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0D6C:
        mov    byte ptr [esi + 29h], al
        call   sub_1_0FE5
        call   sub_1_1007
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0D85:
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0D91:
        call   sub_1_0F3B
        call   sub_1_10BE
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0DA7:
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0DC2
        call   sub_1_0470
        jae    SHORT loc_1_0DC2
        ret
loc_1_0DC2:
        ret
loc_1_0DC3:
        mov    byte ptr [esi + 28h], al
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0DD2:
        mov    byte ptr [esi + 29h], al
        call   sub_1_0FE5
        call   sub_1_1007
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
sub_1_0DEB:
        push   eax
        push   edx
        and    byte ptr [esi + 2ah], 0bfh
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0E0B
        mov    dx, 0f8ah
        mov    al, byte ptr [esi + 2bh]
        and    al, 3fh
        out    dx, al
        mov    al, byte ptr [esi + 2ah]
        mov    dx, 0b8ah
        out    dx, al
loc_1_0E0B:
        and    byte ptr [esi + 44h], 0dfh
        and    byte ptr [esi + 18h], 7fh
        pop    edx
        pop    eax
        ret
loc_1_0E16:
        or     byte ptr [esi + 2bh], 10h
        or     byte ptr [esi + 44h], 2
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0E35
        mov    al, byte ptr [esi + 2bh]
        mov    dx, 0f8ah
        out    dx, al
        mov    al, 29h
        call   sub_1_10E6
loc_1_0E35:
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0E3A:
        and    byte ptr [esi + 44h], 0fdh
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0E4D
        sub    eax, eax
        call   sub_1_10E6
loc_1_0E4D:
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0E52:
        or     byte ptr [esi + 2ah], 40h
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0E6E
        mov    dx, 0f8ah
        mov    al, byte ptr [esi + 2bh]
        out    dx, al
        mov    al, byte ptr [esi + 2ah]
        mov    dx, 0b8ah
        out    dx, al
loc_1_0E6E:
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0E73:
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0E7F:
        not    al
        mov    byte ptr [esi + 19h], al
        or     byte ptr [esi + 1ah], 80h
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0E94:
        mov    byte ptr [esi + 19h], 1
        or     byte ptr [esi + 1ah], 80h
        mov    byte ptr [esi + 1bh], 1
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0EAC:
        mov    byte ptr [esi + 19h], 5
        or     byte ptr [esi + 1ah], 80h
        mov    byte ptr [esi + 1bh], 0
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
        ret
loc_1_0EC0:
        and    byte ptr [esi + 18h], 7fh
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0EDB
        call   sub_1_0470
        jae    SHORT loc_1_0EDB
        ret
loc_1_0EDB:
        ret
loc_1_0EDC:
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0EE8:
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0EF4:
        mov    byte ptr [esi + 1ch], al
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0F03:
        mov    al, byte ptr [esi + 1ch]
        mov    byte ptr [esi + 19h], al
        or     byte ptr [esi + 1ah], 80h
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0F12:
        call   sub_1_0F29
        and    byte ptr [esi + 18h], 7fh
        ret
loc_1_0F1C:
        mov    byte ptr [esi + 1dh], 32h
        mov    byte ptr [esi + 19h], 0
        and    byte ptr [esi + 18h], 7fh
        ret
sub_1_0F29:
        push   ebx
        or     byte ptr [esi + 44h], 8
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        pop    ebx
        ret
sub_1_0F3B:
        push   eax
        mov    ah, 24h
        cmp    al, 59h
        jb     SHORT loc_1_0F4A
        mov    ah, 39h
        cmp    al, 0a5h
        jb     SHORT loc_1_0F4A
        mov    ah, 31h
loc_1_0F4A:
        and    byte ptr [esi + 2ah], 0c0h
        or     byte ptr [esi + 2ah], ah
        pop    eax
        ret
sub_1_0F53:
        mov    byte ptr [esi + 20h], 0ffh
        mov    ah, al
        xchg   byte ptr [esi + 21h], al
        cmp    al, ah
        je     SHORT loc_1_0FC2
        mov    dx, 0b8bh
        in     al, dx
        or     al, 0ch
        mov    byte ptr [esi + 2dh], al
        out    dx, al
        mov    al, 0a5h
        call   sub_1_0F3B
        call   sub_1_10BE
        or     al, byte ptr [esi + 21h]
        or     al, 69h
        mov    byte ptr [esi + 2bh], al
        and    byte ptr [esi + 2ah], 3fh
        or     byte ptr [esi + 2ah], 40h
        mov    al, byte ptr [esi + 2ah]
        and    byte ptr [esi + 44h], 0cfh
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0FC2
        sub    eax, eax
        call   sub_1_10E6
        mov    dx, 0b8bh
        mov    al, byte ptr [esi + 2dh]
        out    dx, al
        mov    ax, word ptr [esi + 26h]
        mov    byte ptr [esi + 24h], 36h
        call   sub_1_108C
        mov    al, byte ptr [esi + 2bh]
        mov    dx, 0f8ah
        out    dx, al
        mov    al, byte ptr [esi + 2ah]
        mov    dx, 0b8ah
        out    dx, al
loc_1_0FC2:
        ret
sub_1_0FC3:
        mov    byte ptr [esi + 21h], 0ffh
        cmp    byte ptr [esi + 20h], 10h
        mov    byte ptr [esi + 20h], 10h
        je     SHORT loc_1_0FE4
        or     byte ptr [esi + 44h], 2
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0FE4
        mov    al, 29h
        call   sub_1_10E6
loc_1_0FE4:
        ret
sub_1_0FE5:
        mov    byte ptr [esi + 21h], 0ffh
        cmp    byte ptr [esi + 20h], 0
        mov    byte ptr [esi + 20h], 0
        je     SHORT loc_1_1006
        and    byte ptr [esi + 44h], 0fdh
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0FE4
        sub    eax, eax
        call   sub_1_10E6
loc_1_1006:
        ret
sub_1_1007:
        cmp    byte ptr [esi + 20h], 0ffh
        je     SHORT loc_1_1025
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        add    ax, 2
        cmp    ax, word ptr [esi + 28h]
        jge    SHORT loc_1_1025
        sub    ax, 2
        sub    word ptr [esi + 28h], ax
loc_1_1025:
        mov    al, byte ptr [esi + 2dh]
        and    al, 0f3h
        or     al, 8
        mov    byte ptr [esi + 2dh], al
        mov    al, byte ptr [esi + 20h]
        or     al, 0e9h
        mov    byte ptr [esi + 2bh], al
        or     byte ptr [esi + 44h], 20h
        or     byte ptr [esi + 2ah], 0c0h
        mov    al, byte ptr [esi + 2ah]
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_108B
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [esi + 26h]
        mov    byte ptr [esi + 24h], 36h
        call   sub_1_108C
        mov    ax, word ptr [esi + 28h]
        inc    ax
        cmp    ax, 2
        adc    ax, 0
        mov    byte ptr [esi + 25h], 74h
        call   sub_1_10A5
        mov    dx, 0b8bh
        mov    al, byte ptr [esi + 2dh]
        out    dx, al
        mov    al, byte ptr [esi + 2bh]
        mov    dx, 0f8ah
        out    dx, al
        mov    al, byte ptr [esi + 2ah]
        mov    dx, 0b8ah
        out    dx, al
loc_1_108B:
        ret
sub_1_108C:
        push   eax
        mov    al, byte ptr [esi + 24h]
        and    al, 7fh
        mov    dx, 138bh
        out    dx, al
        pop    eax
        mov    dx, 1388h
        out    dx, al
        jmp    SHORT loc_1_109F
loc_1_109F:
        xchg   al, ah
        out    dx, al
        xchg   al, ah
        ret
sub_1_10A5:
        push   eax
        mov    al, byte ptr [esi + 25h]
        and    al, 7fh
        mov    dx, 138bh
        out    dx, al
        pop    eax
        mov    dx, 1389h
        out    dx, al
        xchg   al, ah
        jmp    SHORT loc_1_10BA
loc_1_10BA:
        out    dx, al
        xchg   al, ah
        ret
sub_1_10BE:
        push   ebx
        mov    bx, 100h
        sub    ah, ah
        sub    bx, ax
        mov    dx, 0fh
        mov    ax, 4240h
        div    bx
        mov    dx, 12h
        mov    bx, 34dch
        xchg   bx, ax
        div    bx
        mov    word ptr [esi + 26h], ax
        pop    ebx
        ret
sub_1_10E6:
        cmp    al, byte ptr [esi + 2ch]
        mov    byte ptr [esi + 2ch], al
        je     SHORT loc_1_10FE
        push   eax
        mov    ah, 0dh
        call   sub_1_10FF
        pop    eax
        mov    ah, 6
        call   sub_1_10FF
loc_1_10FE:
        ret
sub_1_10FF:
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 030h, 000h, 000h, 000h
        je     SHORT loc_1_110C
        ret
loc_1_110C:
        push   eax
        or     ah, 80h
        or     ah, 40h
        call   sub_1_1124
        pop    eax
        mov    ah, al
        or     ah, 40h
        call   sub_1_1124
        ret
sub_1_1124:
        pushfd
        cli
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 3fh
        mov    ecx, 8
        mov    dx, 0b88h
        out    dx, al
loc_1_1137:
        xor    al, 2
        jmp    SHORT loc_1_113B
loc_1_113B:
        out    dx, al
        ror    al, 1
        ror    ah, 1
        adc    al, al
        out    dx, al
        xor    al, 2
        jmp    SHORT loc_1_1147
loc_1_1147:
        out    dx, al
        loop   loc_1_1137
        xor    al, 4
        out    dx, al
        jmp    SHORT loc_1_114F
loc_1_114F:
        xor    al, 4
        out    dx, al
        db  0A2h, 000h, 000h, 000h, 000h
        popfd
        ret
        db  0CCh, 0CCh, 0CCh
loc_1_115C:
        push   esi
        push   ebx
        push   edx
        push   eax
        mov    edx, 0b8bh
        in     al, dx
        mov    ah, 1fh
        and    ah, al
        mov    edx, 0b89h
        in     al, dx
        and    ah, al
        je     SHORT loc_1_11A8
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_11A8
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jae    SHORT loc_1_1194
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        jmp    SHORT loc_1_11A8
loc_1_1194:
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        pop    eax
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        pop    edx
        pop    ebx
        pop    esi
        clc
        ret
loc_1_11A8:
        pop    eax
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        pop    edx
        pop    ebx
        pop    esi
        clc
        ret
loc_1_11B4:
        ret
loc_1_11B5:
        push   esi
        push   ebx
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_11D9
        test   byte ptr [esi + 44h], 8
        jne    SHORT loc_1_11D9
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_1_11D9:
        and    byte ptr [esi + 44h], 0c7h
        pop    ebx
        pop    esi
        ret
loc_1_11E0:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        jecxz  loc_1_11F1
        or     byte ptr [esi + 44h], 4
        pop    esi
        ret
loc_1_11F1:
        and    byte ptr [esi + 44h], 0fbh
        test   byte ptr [esi + 44h], 8
        je     SHORT loc_1_1208
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        pop    eax
loc_1_1208:
        pop    esi
        ret
loc_1_120A:
        ret
loc_1_120B:
        cmp    ecx, 4
        jbe    SHORT loc_1_1216
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_1216:
        je     SHORT loc_1_121A
        in     al, dx
        ret
loc_1_121A:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_122A
        call   sub_1_0470
        jae    SHORT loc_1_122A
        ret
loc_1_122A:
        call   sub_1_129A
        call   sub_1_136A
        out    dx, al
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 001h
        ret
loc_1_123D:
        cmp    ecx, 4
        jbe    SHORT loc_1_1248
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_1248:
        je     SHORT loc_1_124C
        in     al, dx
        ret
loc_1_124C:
        out    dx, al
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 001h
        ret
loc_1_1255:
        cmp    ecx, 4
        jbe    SHORT loc_1_1260
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_1260:
        je     SHORT loc_1_1264
        in     al, dx
        ret
loc_1_1264:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_1274
        call   sub_1_0470
        jae    SHORT loc_1_1274
        ret
loc_1_1274:
        call   sub_1_129A
        out    dx, al
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 001h
        ret
loc_1_1282:
        cmp    ecx, 4
        jbe    SHORT loc_1_128D
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_128D:
        je     SHORT loc_1_1291
        in     al, dx
        ret
loc_1_1291:
        out    dx, al
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 001h
        ret
sub_1_129A:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 2eh], 0ffh
        mov    edx, 6
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        mov    edx, 0ah
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        mov    edx, 0ch
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        mov    edx, 0eh
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_12FE
        mov    ax, 0ffh
        call   sub_1_138C
loc_1_12FE:
        pop    edx
        pop    eax
        pop    esi
        ret
sub_1_1302:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 2eh], 0
        mov    edx, 6
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        mov    edx, 0ah
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        mov    edx, 0ch
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        mov    edx, 0eh
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_1366
        mov    ax, 0
        call   sub_1_138C
loc_1_1366:
        pop    edx
        pop    eax
        pop    esi
        ret
sub_1_136A:
        push   eax
        push   edx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    dl, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        in     al, dx
        sub    eax, eax
        out    dx, al
loc_1_137F:
        in     al, dx
        dec    ah
        jne    SHORT loc_1_137F
        add    dx, 4
        in     al, dx
        pop    edx
        pop    eax
        ret
sub_1_138C:
        push   ecx
        push   edx
        db  0A2h, 000h, 000h, 000h, 000h
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h
        je     SHORT loc_1_13F2
        cbw
        and    ah, 2
        mov    dx, 0f788h
        in     al, dx
        and    al, 0fdh
        or     al, ah
        out    dx, al
        mov    dx, 0f789h
        in     al, dx
        and    al, 0f0h
        db  08Ah, 025h, 000h, 000h, 000h, 000h
        mov    cl, 4
        shr    ah, cl
        db  022h, 025h, 000h, 000h, 000h, 000h
        or     al, ah
        out    dx, al
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    dx, 6
        mov    al, 1
        out    dx, al
        jmp    SHORT loc_1_13D8
loc_1_13D8:
        in     al, dx
        in     al, dx
        in     al, dx
        in     al, dx
        sub    ax, ax
        out    dx, al
        mov    ah, 2
        db  022h, 025h, 000h, 000h, 000h, 000h
        mov    dx, 0b8bh
        in     al, dx
        and    al, 0fdh
        or     al, ah
        out    dx, al
loc_1_13F2:
        pop    edx
        pop    ecx
        ret
        db  0CCh, 0CCh, 0CCh
_LTEXT ends
        end
