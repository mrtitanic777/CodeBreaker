; irlapfrm_lcode_01.asm -- IRLAPFRM object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0070:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        xor    edx, edx
        db  0CDh, 020h, 007h, 000h, 005h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 049h, 052h, 04Ch, 041h
        db  050h, 046h, 052h, 04Dh, 002h, 000h, 000h, 030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00E0:
        inc    ebx
        dec    edi
        dec    ebp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx - 64dcedefh], cl
loc_1_0167:
        xor    ah, byte ptr [esi + eax*2]
        lodsd
        push   edi
        mov    edi, 0c18c4874h
        popfd
        pop    edx
        scasd
        sar    dword ptr [esi - 241a3594h], cl
        jle    SHORT loc_1_0167
        idiv   eax
        adc    dword ptr [eax], 33930108h
        sbb    ah, byte ptr [edx]
        movsd
        push   esi
        sub    al, 47h
        mov    bh, 75h
        leave
        pushfd
        inc    eax
        db  08Dh, 0DBh, 0BFh, 052h, 0AEh, 0EDh, 0DAh, 064h, 0CBh, 0FFh, 0F9h, 076h, 0E8h, 002h, 021h, 08Bh
        db  030h, 010h, 002h, 099h, 013h, 026h, 067h, 0AFh, 076h, 034h, 044h, 0BDh, 055h, 04Ah, 0ADh, 0C3h
        db  0BCh, 058h, 08Eh, 0D1h, 09Fh, 06Eh, 0EBh, 0E7h, 0FAh, 07Ch, 0C8h, 0F5h, 0D9h, 083h, 031h, 00Ah
        db  020h, 091h, 012h, 018h, 003h, 0A7h, 077h, 02Eh, 066h, 0B5h, 054h, 03Ch, 045h, 0CBh, 0BDh, 042h
        db  0ACh, 0D9h, 09Eh, 050h, 08Fh, 0EFh, 0FBh, 066h, 0EAh, 0FDh, 0D8h, 074h, 0C9h, 004h, 042h, 08Dh
        db  053h, 016h, 061h, 09Fh, 070h, 020h, 004h, 0A9h, 015h, 032h, 027h, 0BBh, 036h, 04Ch, 0CEh, 0C5h
        db  0DFh, 05Eh, 0EDh, 0D7h, 0FCh, 068h, 088h, 0E1h, 099h, 07Ah, 0ABh, 0F3h, 0BAh, 085h, 052h, 00Ch
        db  043h, 097h, 071h, 01Eh, 060h, 0A1h, 014h, 028h, 005h, 0B3h, 037h, 03Ah, 026h, 0CDh, 0DEh, 044h
        db  0CFh, 0DFh, 0FDh, 056h, 0ECh, 0E9h, 098h, 060h, 089h, 0FBh, 0BBh, 072h, 0AAh, 006h, 063h, 08Fh
        db  072h, 014h, 040h, 09Dh, 051h, 022h, 025h, 0ABh, 034h, 030h, 006h, 0B9h, 017h, 04Eh, 0EFh, 0C7h
        db  0FEh, 05Ch, 0CCh, 0D5h, 0DDh, 06Ah, 0A9h, 0E3h, 0B8h, 078h, 08Ah, 0F1h, 09Bh, 087h, 073h, 00Eh
        db  062h, 095h, 050h, 01Ch, 041h, 0A3h, 035h, 02Ah, 024h, 0B1h, 016h, 038h, 007h, 0CFh, 0FFh, 046h
        db  0EEh, 0DDh, 0DCh, 054h, 0CDh, 0EBh, 0B9h, 062h, 0A8h, 0F9h, 09Ah, 070h, 08Bh, 008h, 084h, 081h
        db  095h, 01Ah, 0A7h, 093h, 0B6h, 02Ch, 0C2h, 0A5h, 0D3h, 03Eh, 0E1h, 0B7h, 0F0h, 040h, 008h, 0C9h
        db  019h, 052h, 02Bh, 0DBh, 03Ah, 064h, 04Eh, 0EDh, 05Fh, 076h, 06Dh, 0FFh, 07Ch, 089h, 094h, 000h
        db  085h, 09Bh, 0B7h, 012h, 0A6h, 0ADh, 0D2h, 024h, 0C3h, 0BFh, 0F1h, 036h, 0E0h, 0C1h, 018h, 048h
        db  009h, 0D3h, 03Bh, 05Ah, 02Ah, 0E5h, 05Eh, 06Ch, 04Fh, 0F7h, 07Dh, 07Eh, 06Ch, 00Ah, 0A5h, 083h
        db  0B4h, 018h, 086h, 091h, 097h, 02Eh, 0E3h, 0A7h, 0F2h, 03Ch, 0C0h, 0B5h, 0D1h, 042h, 029h, 0CBh
        db  038h, 050h, 00Ah, 0D9h, 01Bh, 066h, 06Fh, 0EFh, 07Eh, 074h, 04Ch, 0FDh, 05Dh, 08Bh, 0B5h, 002h
        db  0A4h, 099h, 096h, 010h, 087h, 0AFh, 0F3h, 026h, 0E2h, 0BDh, 0D0h, 034h, 0C1h, 0C3h, 039h, 04Ah
        db  028h, 0D1h, 01Ah, 058h, 00Bh, 0E7h, 07Fh, 06Eh, 06Eh, 0F5h, 05Ch, 07Ch, 04Dh, 00Ch, 0C6h, 085h
        db  0D7h, 01Eh, 0E5h, 097h, 0F4h, 028h, 080h, 0A1h, 091h, 03Ah, 0A3h, 0B3h, 0B2h, 044h, 04Ah, 0CDh
        db  05Bh, 056h, 069h, 0DFh, 078h, 060h, 00Ch, 0E9h, 01Dh, 072h, 02Fh, 0FBh, 03Eh, 08Dh, 0D6h, 004h
        db  0C7h, 09Fh, 0F5h, 016h, 0E4h, 0A9h, 090h, 020h, 081h, 0BBh, 0B3h, 032h, 0A2h, 0C5h, 05Ah, 04Ch
        db  04Bh, 0D7h, 079h, 05Eh, 068h, 0E1h, 01Ch, 068h, 00Dh, 0F3h, 03Fh, 07Ah, 02Eh, 00Eh, 0E7h, 087h
        db  0F6h, 01Ch, 0C4h, 095h, 0D5h, 02Ah, 0A1h, 0A3h, 0B0h, 038h, 082h, 0B1h, 093h, 046h, 06Bh, 0CFh
        db  07Ah, 054h, 048h, 0DDh, 059h, 062h, 02Dh, 0EBh, 03Ch, 070h, 00Eh, 0F9h, 01Fh, 08Fh, 0F7h, 006h
        db  0E6h, 09Dh, 0D4h, 014h, 0C5h, 0ABh, 0B1h, 022h, 0A0h, 0B9h, 092h, 030h, 083h, 0C7h, 07Bh, 04Eh
        db  06Ah, 0D5h, 058h, 05Ch, 049h, 0E3h, 03Dh, 06Ah, 02Ch, 0F1h, 01Eh, 078h, 00Fh
loc_1_0360:
        aam    30h
        add    byte ptr [eax], al
        push   18h
        add    byte ptr [eax], al
        loop   loc_1_036E
        add    byte ptr [eax], al
        jno    SHORT loc_1_0370
loc_1_036E:
        add    byte ptr [eax], al
loc_1_0370:
        jge    SHORT loc_1_0372
loc_1_0372:
        add    byte ptr [eax], al
        add    byte ptr ds:[eax], al
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0380:
        inc    edx
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
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 3f000000h], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        aas
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    al, 0
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        sbb    byte ptr [esi + eax], cl
        add    eax, dword ptr [edx]
        add    dword ptr [eax], eax
loc_1_0400:
        xor    byte ptr [esi + eax], cl
        add    eax, dword ptr [edx]
        add    dword ptr [eax], eax
        add    byte ptr [edx], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    eax, 1
        add    byte ptr [eax], al
        adc    al, 0ah
        add    al, byte ptr [ecx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    byte ptr [esp + eax], dl
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
        sbb    eax, 10306h
        add    byte ptr [eax], al
        db  073h, 03Ah
        or     al, 6
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0450:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0454:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0458:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_045C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0460:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0464:
        push   esp
        db  078h, 044h
        popad
        db  074h, 061h
        push   ebx
        imul   edi, dword ptr [edx + 65h], 70530000h
        add    byte ptr fs:[eax], al
        add    byte ptr [edi + ebp*2 + 6eh], al
        insb
        push   esp
        db  079h, 070h
        add    byte ptr gs:[eax], al
loc_1_0484:
        dec    ebp
        imul   ebp, dword ptr [esi + 54h], 546e7275h
        imul   ebp, dword ptr [ebp + 65h], 64644100h
        imul   esi, dword ptr [ecx + ebp*2 + 6fh], 426c616eh
        dec    edi
        inc    esi
        jae    SHORT loc_1_049F
loc_1_049F:
        add    byte ptr [eax + 6fh], dl
        jb     SHORT loc_1_0518
        dec    esi
        popad
        insd
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
IRLAPFRM_Control:
        cmp    eax, 1bh
        je     SHORT loc_1_04CA
        cmp    eax, 1ch
        jne    SHORT loc_1_04C3
        call   sub_1_0B72
        cmp    eax, 1
        ret
loc_1_04C3:
        cmp    eax, 1ah
        je     SHORT loc_1_04EB
        clc
        ret
loc_1_04CA:
        db  0CDh, 020h, 000h, 000h, 0A2h, 004h
        jb     SHORT loc_1_04DD
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_04D7:
        or     eax, eax
        je     SHORT loc_1_04DD
        clc
        ret
loc_1_04DD:
        stc
        ret
sub_1_04DF:
        db  0CDh, 020h, 001h, 080h, 0A2h, 004h
sub_1_04E5:
        db  0CDh, 020h, 025h, 080h, 0A2h, 004h
loc_1_04EB:
        push   bp
        push   edx
        push   ebx
        push   esi
        call   sub_1_0BAE
        pop    esi
        pop    ebx
        pop    edx
        pop    bp
        clc
        ret
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h, 0C3h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
        db  0CCh, 0CCh, 0CCh, 0CCh
loc_1_0510:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh
loc_1_0518:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        test   byte ptr [esi], dh
        add    byte ptr [eax], al
        db  000h, 0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 24h], 0
        jne    NEAR PTR loc_1_054F
        call   sub_1_1EC6
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 24h], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_0563
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_054F:
        mov    eax, 4
        jmp    NEAR PTR loc_1_0563
        db  0B8h, 00Eh, 000h, 000h, 000h, 0E9h, 000h, 000h, 000h, 000h
loc_1_0563:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0568:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_0585
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_05C0
loc_1_0585:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 24h], 0
        je     NEAR PTR loc_1_05B6
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 24h], 0
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_05C0
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_05B6:
        mov    eax, 8
        jmp    NEAR PTR loc_1_05C0
loc_1_05C0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_05C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 54h], 1
        call   sub_1_254F
        mov    dword ptr [ebp - 50h], eax
        cmp    dword ptr [ebp - 50h], 0
        je     NEAR PTR loc_1_05EF
        mov    eax, dword ptr [ebp - 50h]
        jmp    NEAR PTR loc_1_07ED
loc_1_05EF:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 34h], 0
        jne    NEAR PTR loc_1_07E3
        push   -1
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_25D4
        add    esp, 8
        mov    dword ptr [ebp - 4ch], eax
        cmp    dword ptr [ebp - 4ch], -1
        jne    NEAR PTR loc_1_0629
        mov    eax, 5
        jmp    NEAR PTR loc_1_07ED
        db  0E9h, 014h, 000h, 000h, 000h
loc_1_0629:
        cmp    dword ptr [ebp - 4ch], -2
        jne    NEAR PTR loc_1_063D
        mov    eax, 0bh
        jmp    NEAR PTR loc_1_07ED
loc_1_063D:
        mov    eax, dword ptr [ebp - 4ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 34h], eax
        push   0
        push   0
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 38h]
        push   eax
        mov    eax, dword ptr [ebp - 4ch]
        push   eax
        call   sub_1_25CC
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_1_0677
        jmp    NEAR PTR loc_1_07A4
loc_1_0677:
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 4ch]
        push   eax
        call   sub_1_25C4
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0694
        jmp    NEAR PTR loc_1_07A4
loc_1_0694:
        mov    dword ptr [ebp - 2ch], 2580h
        mov    byte ptr [ebp - 1ah], 8
        mov    byte ptr [ebp - 19h], 0
        mov    dword ptr [ebp - 28h], 1
        mov    byte ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 48h], 57h
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 4ch]
        push   eax
        call   sub_1_25BC
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_06D6
        jmp    NEAR PTR loc_1_07A4
loc_1_06D6:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_073E
        db  0B8h, 000h, 000h, 000h, 000h
        add    eax, 4
        lea    ecx, [ebp - 44h]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], eax
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp - 4ch]
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0723
        mov    dword ptr [ebp - 54h], 36h
        jmp    NEAR PTR loc_1_07A4
loc_1_0723:
        lea    eax, [ebp - 44h]
        db  0B9h, 000h, 000h, 000h, 000h
        add    ecx, 4
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], eax
loc_1_073E:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 44h
        push   eax
        push   85h
        mov    eax, dword ptr [ebp - 4ch]
        push   eax
        call   sub_1_25B4
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0765
        jmp    NEAR PTR loc_1_07A4
loc_1_0765:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 44h], 85h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c2h], 0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4ch]
        push   eax
        call   sub_1_25AC
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_079D
        jmp    NEAR PTR loc_1_07A4
loc_1_079D:
        xor    eax, eax
        jmp    NEAR PTR loc_1_07ED
loc_1_07A4:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_07BE
        mov    eax, dword ptr [ebp - 4ch]
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 4
loc_1_07BE:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 34h], 0
        mov    eax, dword ptr [ebp - 4ch]
        push   eax
        call   sub_1_25A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 54h]
        jmp    NEAR PTR loc_1_07ED
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_07E3:
        mov    eax, 4
        jmp    NEAR PTR loc_1_07ED
loc_1_07ED:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_07F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 34h], 0
        je     NEAR PTR loc_1_091C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 34h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 18h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 18h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 14h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 1ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 20h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 20h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 1ch], eax
        push   0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25AC
        add    esp, 0ch
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25FC
        add    esp, 0ch
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25F4
        add    esp, 8
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25F4
        add    esp, 8
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_08C4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 4
loc_1_08C4:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25A4
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_08EE
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25EC
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 4], 1
loc_1_08EE:
        call   sub_1_1D30
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 48h]
        push   eax
        call   sub_1_25DC
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4ch]
        push   eax
        call   sub_1_25DC
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0926
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_091C:
        mov    eax, 6
        jmp    NEAR PTR loc_1_0926
loc_1_0926:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_092B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 2ch]
        jmp    NEAR PTR loc_1_093E
loc_1_093E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0943:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 90h
        jmp    NEAR PTR loc_1_0958
loc_1_0958:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_095D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        jmp    NEAR PTR loc_1_0978
loc_1_0978:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_097D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c4h]
        and    eax, 0ffh
        jmp    NEAR PTR loc_1_0998
loc_1_0998:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_099D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0c4h], al
        xor    eax, eax
        jmp    NEAR PTR loc_1_09B9
loc_1_09B9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_09BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_0AAF
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 44h]
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 44h
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_25B4
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0A08
        jmp    NEAR PTR loc_1_0AD8
loc_1_0A08:
        push   0
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_25AC
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0A2A
        jmp    NEAR PTR loc_1_0AD8
loc_1_0A2A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0A4D
        jmp    NEAR PTR loc_1_0AD8
loc_1_0A4D:
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_25AC
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0A72
        jmp    NEAR PTR loc_1_0AD8
loc_1_0A72:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 44h
        push   eax
        push   85h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_25B4
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0A9E
        jmp    NEAR PTR loc_1_0AD8
loc_1_0A9E:
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 44h], eax
        jmp    NEAR PTR loc_1_0AD1
loc_1_0AAF:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_230E
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0AD1
        jmp    NEAR PTR loc_1_0AD8
loc_1_0AD1:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0AE2
loc_1_0AD8:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0AE2
loc_1_0AE2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0AE7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0B55
        test   byte ptr [ebp + 14h], 4
        je     NEAR PTR loc_1_0B50
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 48h], 0
        jne    NEAR PTR loc_1_0B50
        lea    eax, [ebp - 4]
        push   eax
        push   4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_261C
        add    esp, 0ch
        push   30c001h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2604
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 48h], eax
loc_1_0B50:
        jmp    NEAR PTR loc_1_0B6D
loc_1_0B55:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1693
        add    esp, 10h
loc_1_0B6D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0B72:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_0B9F
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_2624
        add    esp, 8
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0B9F:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0BA9
loc_1_0BA9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0BAE:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_0BC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_1_0BC6:
        cmp    dword ptr [ebp + 10h], 0
        jbe    NEAR PTR loc_1_0C14
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0ffffh
        xor    eax, ecx
        and    eax, 0ffh
        db  066h, 08Bh, 004h, 045h, 000h, 000h, 000h, 000h
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0ffffh
        sar    ecx, 8
        xor    eax, ecx
        mov    word ptr [ebp + 8], ax
        inc    dword ptr [ebp + 0ch]
        dec    dword ptr [ebp + 10h]
        jmp    NEAR PTR loc_1_0BC6
loc_1_0C14:
        mov    ax, word ptr [ebp + 8]
        jmp    NEAR PTR loc_1_0C1D
loc_1_0C1D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_0C48:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     NEAR PTR loc_1_0C64
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0C48
loc_1_0C64:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_0C6C
loc_1_0C6C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0C71:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0CA4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0CD2
loc_1_0CD2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0CD7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_0D54:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 8], eax
        je     NEAR PTR loc_1_0D83
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 0ch], eax
        jne    NEAR PTR loc_1_0D76
        mov    eax, 1
        jmp    NEAR PTR loc_1_0D8A
loc_1_0D76:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0D54
loc_1_0D83:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0D8A
loc_1_0D8A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        jne    NEAR PTR loc_1_0DC0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     NEAR PTR loc_1_0DC7
loc_1_0DC0:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0E20
loc_1_0DC7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0DDC
loc_1_0DD4:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_0DDC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     NEAR PTR loc_1_0E16
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        jne    NEAR PTR loc_1_0E0A
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], ecx
        je     NEAR PTR loc_1_0E11
loc_1_0E0A:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0E20
loc_1_0E11:
        jmp    NEAR PTR loc_1_0DD4
loc_1_0E16:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0E20
loc_1_0E20:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
loc_1_0E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 34h], 0
        jne    NEAR PTR loc_1_0E52
        mov    eax, 6
        jmp    NEAR PTR loc_1_0EFA
loc_1_0E52:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 28h], 2
        je     NEAR PTR loc_1_0E85
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 44h], 85h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 28h], 2
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 2ch], 5
loc_1_0E85:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 2ch], 5
        jne    NEAR PTR loc_1_0ED8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0FBB
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_0EB2
        mov    eax, 1
        jmp    NEAR PTR loc_1_0EFA
loc_1_0EB2:
        db  0A1h, 000h, 000h, 000h, 000h
        and    dword ptr [eax + 44h], 0fffffffbh
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c2h], 1
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 30h], eax
        jmp    NEAR PTR loc_1_0EF0
loc_1_0ED8:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 35h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_0C71
        add    esp, 8
loc_1_0EF0:
        mov    eax, 2
        jmp    NEAR PTR loc_1_0EFA
loc_1_0EFA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0EFF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 40h], ecx
        jne    NEAR PTR loc_1_0F48
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        push   eax
        push   9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 24h]
        call   dword ptr [eax + 8]
        add    esp, 8
        db  0C6h, 005h, 03Fh, 000h, 000h, 000h, 004h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 030h, 000h, 000h, 000h, 000h
loc_1_0F48:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 0ch], eax
loc_1_0F53:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        cmp    eax, dword ptr [ebp - 0ch]
        je     NEAR PTR loc_1_0FAF
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 35h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 40h], ecx
        jne    NEAR PTR loc_1_0FA4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0CD7
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 24h]
        call   dword ptr [eax + 8]
        add    esp, 8
loc_1_0FA4:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_0F53
loc_1_0FAF:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0FB6
loc_1_0FB6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0FBB:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 40h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 3dh]
        and    eax, 0ffh
        dec    eax
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_0FEE
loc_1_0FEB:
        dec    dword ptr [ebp - 10h]
loc_1_0FEE:
        cmp    dword ptr [ebp - 10h], 0
        jbe    NEAR PTR loc_1_101A
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 40h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 3ch]
        mov    byte ptr [eax + ecx], 0ffh
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 40h]
        jmp    NEAR PTR loc_1_0FEB
loc_1_101A:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 40h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 3ch]
        mov    byte ptr [eax + ecx], 0c0h
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 40h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 45h
        push   eax
        push   0ffffh
        call   sub_1_0BC0
        add    esp, 0ch
        mov    word ptr [ebp - 1ch], ax
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_106A
loc_1_1067:
        inc    dword ptr [ebp - 10h]
loc_1_106A:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        cmp    eax, dword ptr [ebp - 10h]
        jbe    NEAR PTR loc_1_1096
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 45h]
        push   eax
        call   sub_1_1463
        add    esp, 4
        jmp    NEAR PTR loc_1_1067
loc_1_1096:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_119F
loc_1_10A9:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 3]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_0BC0
        add    esp, 0ch
        mov    word ptr [ebp - 1ch], ax
        jmp    NEAR PTR loc_1_10E4
loc_1_10DE:
        dec    dword ptr [ebp - 10h]
        inc    dword ptr [ebp - 4]
loc_1_10E4:
        cmp    dword ptr [ebp - 10h], 0
        jbe    NEAR PTR loc_1_1101
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        push   eax
        call   sub_1_1463
        add    esp, 4
        jmp    NEAR PTR loc_1_10DE
loc_1_1101:
        jmp    NEAR PTR loc_1_11B8
loc_1_1106:
        mov    dword ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_1115
loc_1_1112:
        inc    dword ptr [ebp - 18h]
loc_1_1115:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 18h]
        jbe    NEAR PTR loc_1_1195
        mov    eax, dword ptr [ebp - 18h]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*2 + 5]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 18h]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 9]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_0BC0
        add    esp, 0ch
        mov    word ptr [ebp - 1ch], ax
        jmp    NEAR PTR loc_1_1173
loc_1_116D:
        dec    dword ptr [ebp - 10h]
        inc    dword ptr [ebp - 4]
loc_1_1173:
        cmp    dword ptr [ebp - 10h], 0
        jbe    NEAR PTR loc_1_1190
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        push   eax
        call   sub_1_1463
        add    esp, 4
        jmp    NEAR PTR loc_1_116D
loc_1_1190:
        jmp    NEAR PTR loc_1_1112
loc_1_1195:
        jmp    NEAR PTR loc_1_11B8
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_119F:
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_10A9
        cmp    dword ptr [ebp - 20h], 1
        je     NEAR PTR loc_1_1106
        jmp    NEAR PTR loc_1_11B8
loc_1_11B8:
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        not    eax
        mov    word ptr [ebp - 1ch], ax
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffh
        push   eax
        call   sub_1_1463
        add    esp, 4
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        push   eax
        call   sub_1_1463
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 40h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 3ch]
        mov    byte ptr [eax + ecx], 0c1h
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 40h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 3eh]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_123E
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 40h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 3ch]
        mov    byte ptr [eax + ecx], 0ffh
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 40h]
loc_1_123E:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 2ch], 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12DD
        add    esp, 4
        lea    eax, [ebp - 0ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 40h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 3ch]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_262C
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_1296
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 40h], ecx
        je     NEAR PTR loc_1_12B4
loc_1_1296:
        lea    eax, [ebp - 8]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_25FC
        add    esp, 0ch
        mov    dword ptr [ebp - 14h], 1
loc_1_12B4:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 7ch
        push   eax
        call   sub_1_1DB4
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 0ach]
        mov    eax, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_12D8
loc_1_12D8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_12DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4fh]
        and    eax, 0ffh
        cmp    eax, 7
        jge    NEAR PTR loc_1_1359
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4fh]
        and    eax, 0ffh
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        call   sub_1_0070
        add    eax, dword ptr [ebp - 0ch]
        adc    edx, 0
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 8], edx
loc_1_1320:
        call   sub_1_0070
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        jb     NEAR PTR loc_1_134F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        jne    NEAR PTR loc_1_1354
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 10h], eax
        jb     NEAR PTR loc_1_1354
loc_1_134F:
        jmp    NEAR PTR loc_1_1359
loc_1_1354:
        jmp    NEAR PTR loc_1_1320
loc_1_1359:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_135E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 48h], 0
        call   sub_1_137A
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_137A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_13AA
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 34h], 0
        jne    NEAR PTR loc_1_13AF
loc_1_13AA:
        jmp    NEAR PTR loc_1_145E
loc_1_13AF:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 7ch
        push   eax
        call   sub_1_1E15
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 2ch], 6
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 24h]
        call   dword ptr [eax + 8]
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 14h
        cmp    dword ptr [eax + 14h], ecx
        jne    NEAR PTR loc_1_1412
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 30h], 0
        call   sub_1_1647
        jmp    NEAR PTR loc_1_145E
loc_1_1412:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        push   eax
        call   sub_1_0CA4
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 35h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 30h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        push   eax
        call   sub_1_0FBB
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_145E
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 0b4h]
        call   sub_1_137A
loc_1_145E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1463:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 0c0h
        je     NEAR PTR loc_1_1497
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 0c1h
        je     NEAR PTR loc_1_1497
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 7dh
        jne    NEAR PTR loc_1_14D7
loc_1_1497:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 40h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 3ch]
        mov    byte ptr [eax + ecx], 7dh
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        xor    al, 20h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 40h]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [edx + 3ch]
        mov    byte ptr [ecx + edx + 1], al
        db  0A1h, 000h, 000h, 000h, 000h
        add    dword ptr [eax + 40h], 2
        jmp    NEAR PTR loc_1_14F7
loc_1_14D7:
        mov    al, byte ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 40h]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [edx + 3ch]
        mov    byte ptr [ecx + edx], al
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 40h]
loc_1_14F7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_14FC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 34h], 0
        je     NEAR PTR loc_1_154E
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_25F4
        add    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_25FC
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 0b0h]
        call   sub_1_137A
loc_1_154E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_1560:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0ch
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 1ch
        push   eax
        call   sub_1_0C71
        add    esp, 8
        call   sub_1_19AC
        xor    eax, eax
        jmp    NEAR PTR loc_1_1593
loc_1_1593:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1598:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 34h], 0
        jne    NEAR PTR loc_1_15BA
        mov    eax, 6
        jmp    NEAR PTR loc_1_1642
loc_1_15BA:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_160E
loc_1_15C5:
        call   sub_1_1647
        jmp    NEAR PTR loc_1_1631
loc_1_15CF:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 44h], 85h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c2h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 28h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 2ch], 0
        jmp    NEAR PTR loc_1_1631
        db  0E9h, 02Fh, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_160E:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_15CF
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_15C5
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_1_1638
        jmp    NEAR PTR loc_1_1631
loc_1_1631:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1642
loc_1_1638:
        mov    eax, 1
        jmp    NEAR PTR loc_1_1642
loc_1_1642:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1647:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c2h], 0
        call   sub_1_19AC
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 28h], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 2ch], 3
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c3h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 44h], 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1693:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c4h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_16C5
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c1h], 1
        jmp    NEAR PTR loc_1_16D1
loc_1_16C5:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c1h], 0
loc_1_16D1:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c4h], 1
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 0a8h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c3h], 1
        test   byte ptr [ebp + 14h], 80h
        je     NEAR PTR loc_1_1759
        lea    eax, [ebp - 4]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_25FC
        add    esp, 0ch
        test   byte ptr [ebp - 4], 8
        je     NEAR PTR loc_1_172A
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 0a0h]
loc_1_172A:
        test   byte ptr [ebp - 4], 2
        je     NEAR PTR loc_1_173F
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 0a4h]
loc_1_173F:
        lea    eax, [ebp - 4]
        push   eax
        push   80h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_261C
        add    esp, 0ch
loc_1_1759:
        test   byte ptr [ebp + 14h], 1
        jne    NEAR PTR loc_1_177B
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c1h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_179F
loc_1_177B:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 4ch], 0
        jne    NEAR PTR loc_1_179F
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2604
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 4ch], eax
loc_1_179F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_17A4:
        push   ebp
        mov    ebp, esp
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 2ch], 0
        mov    dword ptr [ebp - 30h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 34h], 0
        jne    NEAR PTR loc_1_17CF
        jmp    NEAR PTR loc_1_19A7
loc_1_17CF:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c1h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_17F7
        push   17h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 24h]
        call   dword ptr [eax + 0ch]
        add    esp, 4
loc_1_17F7:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 34h], 0
        je     NEAR PTR loc_1_1821
        lea    eax, [ebp - 2ch]
        push   eax
        push   28h
        lea    eax, [ebp - 28h]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_2634
        add    esp, 10h
loc_1_1821:
        cmp    dword ptr [ebp - 2ch], 0
        jne    NEAR PTR loc_1_186A
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 34h], 0
        je     NEAR PTR loc_1_1851
        lea    eax, [ebp - 30h]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_25FC
        add    esp, 0ch
loc_1_1851:
        cmp    dword ptr [ebp - 30h], 0
        jne    NEAR PTR loc_1_1865
        jmp    NEAR PTR loc_1_198F
        db  0E9h, 005h, 000h, 000h, 000h
loc_1_1865:
        jmp    NEAR PTR loc_1_17F7
loc_1_186A:
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 0a8h]
        mov    dword ptr [ebp - 34h], 0
        jmp    NEAR PTR loc_1_1884
loc_1_1881:
        inc    dword ptr [ebp - 34h]
loc_1_1884:
        mov    eax, dword ptr [ebp - 2ch]
        cmp    dword ptr [ebp - 34h], eax
        jae    NEAR PTR loc_1_198A
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [ebp + eax - 28h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0c0h], al
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 10h]
        test   eax, eax
        je     NEAR PTR loc_1_1985
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        push   eax
        push   0ffffh
        call   sub_1_0BC0
        add    esp, 0ch
        and    eax, 0ffffh
        cmp    eax, 0f0b8h
        jne    NEAR PTR loc_1_197A
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 8]
        sub    eax, 2
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx], eax
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 94h]
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 34h], 0
        je     NEAR PTR loc_1_1936
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 24h]
        call   dword ptr [eax + 4]
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_1936
        jmp    NEAR PTR loc_1_19A7
loc_1_1936:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], 0
        call   sub_1_19AC
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 34h], 0
        je     NEAR PTR loc_1_1975
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1975
        push   0
        push   0ah
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 24h]
        call   dword ptr [eax + 4]
        add    esp, 8
loc_1_1975:
        jmp    NEAR PTR loc_1_1985
loc_1_197A:
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 90h]
loc_1_1985:
        jmp    NEAR PTR loc_1_1881
loc_1_198A:
        jmp    NEAR PTR loc_1_17F7
loc_1_198F:
        pushfd
        cli
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 4ch], 0
        db  0A1h, 000h, 000h, 000h, 000h
        and    dword ptr [eax + 44h], 0fffffffeh
        popfd
loc_1_19A7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_19AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_1A4B
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 1ch
        cmp    dword ptr [eax + 1ch], ecx
        je     NEAR PTR loc_1_1A3F
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 1ch
        push   eax
        call   sub_1_0CA4
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 8], 0
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1A4B
loc_1_1A3F:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
loc_1_1A4B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1A50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 2ch], 2
        xor    eax, eax
        jmp    NEAR PTR loc_1_1A69
loc_1_1A69:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1A6E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c0h]
        and    eax, 0ffh
        cmp    eax, 0c0h
        jne    NEAR PTR loc_1_1AA7
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 8], 0
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
loc_1_1AA7:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 2ch], 2
        xor    eax, eax
        jmp    NEAR PTR loc_1_1ABA
loc_1_1ABA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1ABF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c0h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1B51
        db  0E9h, 095h, 000h, 000h, 000h
loc_1_1AE5:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1B7A
loc_1_1AF6:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1B7A
loc_1_1B07:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c0h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 8]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [edx + 4]
        mov    byte ptr [ecx + edx], al
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 8]
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 2ch], 1
        jmp    NEAR PTR loc_1_1B7A
        db  0E9h, 029h, 000h, 000h, 000h
loc_1_1B51:
        cmp    dword ptr [ebp - 4], 7dh
        je     NEAR PTR loc_1_1AF6
        cmp    dword ptr [ebp - 4], 0c0h
        je     NEAR PTR loc_1_1B7A
        cmp    dword ptr [ebp - 4], 0c1h
        je     NEAR PTR loc_1_1AE5
        jmp    NEAR PTR loc_1_1B07
loc_1_1B7A:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1B81
loc_1_1B81:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1B86:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c0h]
        and    eax, 0ffh
        cmp    eax, 0c1h
        jne    NEAR PTR loc_1_1BCF
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 9ch]
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 2ch], 2
        jmp    NEAR PTR loc_1_1BF2
loc_1_1BCF:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c0h]
        and    eax, 0ffh
        xor    al, 20h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0c0h], al
        call   sub_1_1CB3
loc_1_1BF2:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1BF9
loc_1_1BF9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1BFE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c0h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1C7E
loc_1_1C1F:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 8], 0
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1CA7
loc_1_1C3C:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 2ch], 3
        mov    eax, 1
        jmp    NEAR PTR loc_1_1CAE
loc_1_1C5E:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1CA7
loc_1_1C6F:
        call   sub_1_1CB3
        jmp    NEAR PTR loc_1_1CA7
        db  0E9h, 029h, 000h, 000h, 000h
loc_1_1C7E:
        cmp    dword ptr [ebp - 4], 7dh
        je     NEAR PTR loc_1_1C5E
        cmp    dword ptr [ebp - 4], 0c0h
        je     NEAR PTR loc_1_1C1F
        cmp    dword ptr [ebp - 4], 0c1h
        je     NEAR PTR loc_1_1C3C
        jmp    NEAR PTR loc_1_1C6F
loc_1_1CA7:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1CAE
loc_1_1CAE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1CB3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [eax + 8], ecx
        jb     NEAR PTR loc_1_1CEC
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 98h]
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1D20
loc_1_1CEC:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c0h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 8]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [edx + 4]
        mov    byte ptr [ecx + edx], al
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 8]
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
loc_1_1D20:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_1D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_1D39:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 74h
        cmp    dword ptr [eax + 74h], ecx
        je     NEAR PTR loc_1_1D75
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 74h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0ch
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1E15
        add    esp, 4
        jmp    NEAR PTR loc_1_1D39
loc_1_1D75:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1D7A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0ch
        push   eax
        call   sub_1_0CD7
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        call   dword ptr [eax + 4]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1DB4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1E15
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_263C
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1E10
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0ch
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 74h
        push   eax
        call   sub_1_0C71
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        jmp    NEAR PTR loc_1_1E10
loc_1_1E10:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1E15:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_1E4D
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0ch
        push   eax
        call   sub_1_0CD7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_2658
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0
loc_1_1E4D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
loc_1_1E60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1EB2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2390
        add    esp, 4
        call   sub_1_2001
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 0c5h
        push   eax
        call   sub_1_2668
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1EB2
        xor    eax, eax
        jmp    NEAR PTR loc_1_1EC1
loc_1_1EB2:
        call   sub_1_1EC6
        db  0B8h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1EC1
loc_1_1EC1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1EC6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1FFC
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1FFC
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 0c5h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_219D
        add    esp, 8
        add    dword ptr [ebp - 4], 0c5h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 14h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 18h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 18h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 14h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 1ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 20h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 20h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 1ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 74h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 78h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 78h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 74h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 7ch], 3e8h
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 38h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 3ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 4
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 58h], eax
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 54h], eax
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 70h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 50h
        push   eax
        call   sub_1_1560
        add    esp, 4
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_1FFC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2001:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 010h, 000h, 000h, 000h
        push   eax
        call   sub_1_213D
        add    esp, 4
        db  066h, 08Bh, 004h, 045h, 000h, 000h, 000h, 000h
        and    eax, 0ffffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 014h, 000h, 000h, 000h
        push   eax
        call   sub_1_213D
        add    esp, 4
        inc    eax
        db  0A2h, 000h, 000h, 000h, 000h
        db  0A1h, 004h, 000h, 000h, 000h
        push   eax
        call   sub_1_213D
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 008h, 000h, 000h, 000h
        push   eax
        call   sub_1_213D
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        db  08Ah, 084h, 0C8h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        db  0A1h, 00Ch, 000h, 000h, 000h
        push   eax
        call   sub_1_213D
        add    esp, 4
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 10h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        shl    eax, 4
        mov    ecx, 64h
        sub    edx, edx
        div    ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, eax
        add    ecx, dword ptr [ebp - 0ch]
        add    ecx, dword ptr [ebp - 10h]
        add    ecx, 0eh
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        imul   ecx, eax
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        cmp    eax, 2
        jge    NEAR PTR loc_1_20F3
        db  0A1h, 000h, 000h, 000h, 000h
        db  001h, 005h, 000h, 000h, 000h, 000h
loc_1_20F3:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 1a80h
        jb     NEAR PTR loc_1_2108
        mov    eax, 1a80h
loc_1_2108:
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 0A1h, 01Ch, 000h, 000h, 000h
        add    eax, eax
        add    eax, 3eh
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  003h, 005h, 000h, 000h, 000h, 000h
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 4
        db  0A3h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_213D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        and    dword ptr [ebp + 8], 0ffffh
        jne    NEAR PTR loc_1_215A
        xor    eax, eax
        jmp    NEAR PTR loc_1_2198
loc_1_215A:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_2169
loc_1_2166:
        inc    dword ptr [ebp - 4]
loc_1_2169:
        cmp    dword ptr [ebp - 4], 0fh
        jae    NEAR PTR loc_1_218B
        shl    dword ptr [ebp + 8], 1
        test   byte ptr [ebp + 0ah], 1
        je     NEAR PTR loc_1_2186
        jmp    NEAR PTR loc_1_218B
loc_1_2186:
        jmp    NEAR PTR loc_1_2166
loc_1_218B:
        mov    eax, 0fh
        sub    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_2198
loc_1_2198:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_219D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_21B4
loc_1_21B1:
        dec    dword ptr [ebp + 0ch]
loc_1_21B4:
        cmp    dword ptr [ebp + 0ch], 0
        jbe    NEAR PTR loc_1_21CC
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_21B1
loc_1_21CC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_21E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_1_21E6:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     NEAR PTR loc_1_2228
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     NEAR PTR loc_1_2228
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, byte ptr [ecx]
        cmp    eax, ecx
        je     NEAR PTR loc_1_221D
        xor    eax, eax
        jmp    NEAR PTR loc_1_2252
loc_1_221D:
        inc    dword ptr [ebp + 8]
        inc    dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_21E6
loc_1_2228:
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, byte ptr [ecx]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_224B
        mov    eax, 1
        jmp    NEAR PTR loc_1_2252
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_224B:
        xor    eax, eax
        jmp    NEAR PTR loc_1_2252
loc_1_2252:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2257:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_226F
loc_1_226C:
        inc    dword ptr [ebp - 4]
loc_1_226F:
        mov    eax, dword ptr [ebp - 4]
        db  039h, 005h, 000h, 000h, 000h, 000h
        jbe    NEAR PTR loc_1_22EB
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + ecx]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21E0
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_22E6
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + ecx + 4]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + ecx + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + ecx + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_2309
loc_1_22E6:
        jmp    NEAR PTR loc_1_226C
loc_1_22EB:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2309:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_230E:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_25C4
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_2336
        xor    eax, eax
        jmp    NEAR PTR loc_1_237C
loc_1_2336:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 2ch], eax
        mov    byte ptr [ebp - 1ah], 8
        mov    byte ptr [ebp - 19h], 0
        mov    byte ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 34h], 55h
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_25BC
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_2372
        xor    eax, eax
        jmp    NEAR PTR loc_1_237C
loc_1_2372:
        mov    eax, 1
        jmp    NEAR PTR loc_1_237C
loc_1_237C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_2390:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 24h], 4
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_1_23F4
        cmp    dword ptr [ebp - 28h], 4
        jne    NEAR PTR loc_1_23F4
        cmp    dword ptr [ebp - 2ch], 0
        jbe    NEAR PTR loc_1_23F4
        mov    eax, dword ptr [ebp - 2ch]
        cmp    eax, 800h
        jb     NEAR PTR loc_1_23EE
        mov    eax, 800h
loc_1_23EE:
        db  066h, 0A3h, 01Ch, 000h, 000h, 000h
loc_1_23F4:
        mov    dword ptr [ebp - 24h], 4
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_1_2441
        cmp    dword ptr [ebp - 28h], 4
        jne    NEAR PTR loc_1_2441
        cmp    dword ptr [ebp - 2ch], 0
        jbe    NEAR PTR loc_1_2441
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 3fh
        db  0A3h, 004h, 000h, 000h, 000h
loc_1_2441:
        mov    dword ptr [ebp - 24h], 20h
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_1_2485
        cmp    dword ptr [ebp - 28h], 1
        jne    NEAR PTR loc_1_2485
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_1_2257
        add    esp, 4
loc_1_2485:
        mov    dword ptr [ebp - 24h], 4
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_1_24D4
        cmp    dword ptr [ebp - 28h], 4
        jne    NEAR PTR loc_1_24D4
        cmp    dword ptr [ebp - 2ch], 0
        jbe    NEAR PTR loc_1_24D4
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 0ffh
        db  0A3h, 008h, 000h, 000h, 000h
loc_1_24D4:
        mov    dword ptr [ebp - 24h], 4
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_1_2523
        cmp    dword ptr [ebp - 28h], 4
        jne    NEAR PTR loc_1_2523
        cmp    dword ptr [ebp - 2ch], 0
        jbe    NEAR PTR loc_1_2523
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 0ffh
        db  0A3h, 00Ch, 000h, 000h, 000h
loc_1_2523:
        mov    dword ptr [ebp - 24h], 80h
        lea    eax, [ebp - 24h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 28h]
        push   eax
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 18h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_254F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_256C
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_258C
loc_1_256C:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 24h], 0
        jne    NEAR PTR loc_1_2585
        mov    eax, 8
        jmp    NEAR PTR loc_1_258C
loc_1_2585:
        xor    eax, eax
        jmp    NEAR PTR loc_1_258C
loc_1_258C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2591:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        jmp    NEAR PTR loc_1_259E
loc_1_259E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh
sub_1_25A4:
        db  0CDh, 020h, 009h, 080h, 02Bh, 000h
        int3
        int3
sub_1_25AC:
        db  0CDh, 020h, 014h, 080h, 02Bh, 000h
        int3
        int3
sub_1_25B4:
        db  0CDh, 020h, 010h, 080h, 02Bh, 000h
        int3
        int3
sub_1_25BC:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        int3
        int3
sub_1_25C4:
        db  0CDh, 020h, 006h, 080h, 02Bh, 000h
        int3
        int3
sub_1_25CC:
        db  0CDh, 020h, 007h, 080h, 02Bh, 000h
        int3
        int3
sub_1_25D4:
        db  0CDh, 020h, 004h, 080h, 02Bh, 000h
        int3
        int3
sub_1_25DC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        pop    esi
        ret    4
        db  0CCh
sub_1_25EC:
        db  0CDh, 020h, 015h, 080h, 02Bh, 000h
        int3
        int3
sub_1_25F4:
        db  0CDh, 020h, 00Fh, 080h, 02Bh, 000h
        int3
        int3
sub_1_25FC:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
        int3
        int3
sub_1_2604:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    8
        db  0CCh, 0CCh, 0CCh
sub_1_261C:
        db  0CDh, 020h, 011h, 080h, 02Bh, 000h
        int3
        int3
sub_1_2624:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_1_262C:
        db  0CDh, 020h, 012h, 080h, 02Bh, 000h
        int3
        int3
sub_1_2634:
        db  0CDh, 020h, 013h, 080h, 02Bh, 000h
        int3
        int3
sub_1_263C:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  0CCh, 0CCh, 0CCh
sub_1_2658:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  0CCh
sub_1_2668:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_1_2670:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
_LTEXT ends
        end
