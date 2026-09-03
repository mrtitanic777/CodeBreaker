; vmm_icode_05_02.asm -- VMM object 5 segment _ITEXT  (part 2/2)
.386p
_ITEXT segment dword public use32 'ICODE'

sub_5_E693:
        test   al, 0fh
        je     SHORT loc_5_E6B7
        test   al, 1
        je     SHORT loc_5_E6A1
        inc    esi
        and    esi, 0fffffffeh
        jmp    SHORT loc_5_E6B7
loc_5_E6A1:
        test   al, 2
        je     SHORT loc_5_E6AD
        add    esi, 3
        and    esi, 0fffffffch
        jmp    SHORT loc_5_E6B7
loc_5_E6AD:
        test   al, 4
        je     SHORT loc_5_E6B7
        add    esi, 0fh
        and    esi, 0fffffff0h
loc_5_E6B7:
        ret
        db  081h, 03Dh, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 010h, 072h, 036h, 083h, 03Dh, 074h, 0E9h
        db  000h, 000h, 000h, 07Fh, 025h, 07Ch, 02Bh, 060h, 033h, 0C0h, 08Dh, 048h
loc_5_E6D4:
        add    cl, ch
        and    byte ptr [eax], bl
        add    byte ptr [esi], al
        add    byte ptr [eax + 78h], cl
        push   ss
        inc    eax
        db  0A3h, 074h, 0E9h, 000h, 000h
loc_5_E6E4:
        db  0A3h, 078h, 0E9h, 000h, 000h
        shr    eax, 0ch
        db  0A2h, 070h, 003h, 000h, 000h
        popad
        clc
        ret
        db  0A3h, 074h, 0E9h, 000h, 000h, 061h, 0F9h, 0C3h, 033h, 0F6h, 0E8h, 0B5h, 0FFh, 0FFh, 0FFh, 00Fh
        db  082h, 0ABh, 000h, 000h, 000h, 08Bh, 035h, 078h, 0E9h, 000h, 000h, 0F6h
loc_5_E710:
        les    ecx, ptr [eax]
        db  074h, 00Fh
        and    esi, 0fffh
        neg    esi
loc_5_E71C:
        add    esi, 1000h
        ret
        db  0E8h
loc_5_E724:
        imul   edi, edi, -1
        db  0FFh
loc_5_E728:
        push   ecx
        lea    ecx, [edx + esi]
loc_5_E72C:
        db  02Bh, 00Dh, 074h, 0E9h, 000h, 000h
        add    ecx, 0fffh
        shr    ecx, 0ch
        cmp    ecx, 2
        ja     SHORT loc_5_E7B9
        push   eax
        push   edx
        push   edi
        mov    edi, ecx
        xor    ecx, ecx
loc_5_E747:
        db  00Fh, 0A3h, 00Dh, 084h, 0E9h, 000h, 000h
        jb     SHORT loc_5_E79B
        push   ecx
        xor    eax, eax
        push   100h
        push   eax
        push   eax
        push   eax
        push   eax
        db  0FFh, 035h, 00Ch, 0F7h, 000h, 000h
        push   0
        push   1
        db  0CDh, 020h, 053h, 000h, 001h, 000h
loc_5_E76C:
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_5_E7B5
        db  08Bh
loc_5_E774:
        or     al, 24h
        db  002h, 00Dh, 070h, 003h, 000h, 000h
        push   0
        push   0
        push   1
        push   ecx
        db  0FFh, 035h, 00Ch, 0F7h, 000h, 000h
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        pop    ecx
        db  00Fh, 0ABh, 00Dh, 084h, 0E9h, 000h, 000h
loc_5_E79B:
        inc    ecx
        cmp    ecx, edi
        jb     SHORT loc_5_E747
        db  088h, 00Dh, 071h, 003h, 000h, 000h
        pop    edi
        pop    edx
        pop    eax
        lea    ecx, [edx + esi]
        db  089h, 00Dh, 078h, 0E9h, 000h, 000h
        clc
        pop    ecx
        ret
loc_5_E7B5:
        pop    ecx
        pop    edi
        pop    edx
        pop    eax
loc_5_E7B9:
        pop    ecx
        stc
        ret
        db  060h, 08Bh, 0D0h, 02Dh, 0A0h, 000h, 000h, 000h, 07Ch, 068h, 083h, 0F8h, 05Fh, 077h, 063h, 02Ah
        db  015h, 070h, 003h, 000h, 000h, 07Ch, 008h, 02Ah, 015h, 071h, 003h, 000h, 000h, 072h, 054h, 033h
        db  0FFh, 087h, 03Ch, 085h, 088h, 0E9h, 000h, 000h, 085h, 0FFh, 074h, 047h, 0C1h, 0EFh, 00Ch, 06Ah
        db  000h, 06Ah, 001h, 057h, 0CDh, 020h, 063h, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 08Bh, 044h, 024h
        db  01Ch, 083h, 0ECh, 004h, 08Bh, 0D4h, 06Ah, 000h, 052h, 06Ah, 001h, 050h, 0CDh, 020h, 061h, 000h
        db  001h, 000h, 083h, 0C4h, 010h, 05Ah, 0C1h, 0EAh, 00Ch, 08Bh, 044h, 024h, 01Ch, 06Ah, 000h, 06Ah
        db  001h, 050h, 0FFh, 035h, 010h, 0F7h, 000h, 000h, 052h, 0CDh, 020h, 05Eh, 000h, 001h, 000h, 083h
        db  0C4h, 014h, 0F8h, 061h, 0C3h, 060h, 033h, 0C0h, 068h, 000h, 001h, 000h, 000h, 050h, 050h, 050h
        db  050h, 050h, 06Ah, 001h, 051h, 0CDh, 020h, 053h, 000h, 001h, 000h, 083h, 0C4h, 020h, 085h, 0C0h
        db  074h, 03Fh, 08Bh, 074h, 024h, 01Ch, 0C1h, 0EEh, 00Ch, 08Dh, 096h, 060h, 0FFh, 0FFh, 0FFh, 08Bh
        db  04Ch, 024h, 018h, 050h, 089h, 004h, 095h, 088h, 0E9h, 000h, 000h, 042h, 005h, 000h, 010h, 000h
        db  000h, 0E2h, 0F1h, 058h, 08Bh, 04Ch, 024h, 018h
loc_5_E874:
        push   0
        push   0
        push   ecx
        push   esi
        db  0FFh, 035h, 010h, 0F7h, 000h, 000h
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        clc
        popad
        ret
        db  0F9h, 061h, 0C3h, 0C8h, 004h, 000h, 000h, 053h, 056h, 057h, 083h, 03Dh, 020h, 0E7h, 000h, 000h
        db  000h, 00Fh, 085h, 05Ch, 002h, 000h, 000h, 08Bh, 045h, 00Ch, 0A8h, 008h, 074h, 00Eh, 0F6h, 0C4h
        db  004h, 074h, 009h, 0F6h, 0C4h, 021h, 00Fh, 085h, 047h, 002h, 000h, 000h, 0F6h, 0C4h, 010h, 074h
        db  006h, 0CDh, 020h, 00Ch, 000h, 015h, 000h, 033h, 0D2h, 0F6h, 0C4h, 008h, 075h, 00Bh, 08Bh, 055h
        db  008h, 085h, 0D2h, 00Fh, 084h, 02Ah, 002h, 000h, 000h, 0F6h, 0C4h, 021h, 074h, 016h, 0E8h, 01Ch
        db  0FEh, 0FFh, 0FFh, 00Fh, 083h, 09Dh, 001h, 000h, 000h, 0F6h, 0C4h, 008h, 00Fh, 085h, 0A4h, 001h
        db  000h, 000h, 0EBh, 000h, 0B9h, 020h, 000h, 000h, 000h, 089h, 055h, 0FCh, 0BBh, 074h, 0E7h, 000h
        db  000h, 0FFh, 075h, 00Ch, 0E8h, 0D8h, 004h, 000h, 000h, 00Fh, 083h, 0FAh, 000h, 000h, 000h, 0B9h
        db  020h, 000h, 000h, 000h, 0BBh, 074h, 0E8h, 000h, 000h, 0FFh, 075h, 00Ch, 0E8h, 0C0h, 004h, 000h
        db  000h, 00Fh, 083h, 0E2h, 000h, 000h, 000h, 08Bh, 0C2h, 0F6h, 045h, 00Dh, 008h, 00Fh, 085h, 065h
        db  001h, 000h, 000h, 0F6h, 045h, 00Dh, 040h, 075h, 049h, 0E8h, 07Dh, 0FDh, 0FFh, 0FFh, 072h, 042h
        db  08Bh, 04Dh, 008h, 081h, 0C1h, 0FFh, 00Fh, 000h, 000h, 0C1h, 0E9h, 00Ch, 0CDh, 020h, 018h, 000h
        db  006h, 000h, 085h, 0C0h, 074h, 02Ch, 0F6h, 045h, 00Dh, 004h, 00Fh, 085h, 038h, 001h, 000h, 000h
        db  0E8h, 0CFh, 0FEh, 0FFh, 0FFh, 072h, 01Bh, 0C1h, 0E1h, 00Ch, 06Ah, 000h, 051h, 050h, 0CDh, 020h
        db  0DFh, 000h, 001h, 000h, 083h, 0C4h, 00Ch
loc_5_E974:
        mov    edx, dword ptr [ebp + 8]
        db  085h
loc_5_E978:
        ror    byte ptr [edi], 85h
        db  072h, 0FFh
        db  0FFh, 0FFh, 08Bh, 035h, 01Ch, 0E7h, 000h, 000h, 00Bh, 0F6h, 00Fh
loc_5_E988:
        test   byte ptr [edi], cl
        add    dword ptr [eax], eax
        add    byte ptr [ebx + 8a80c45h], cl
        jne    SHORT loc_5_E99B
        call   sub_5_E693
        jmp    SHORT loc_5_E9C5
loc_5_E99B:
        test   si, 0fffh
        je     SHORT loc_5_E9C5
        push   esi
        mov    edi, esi
        add    esi, 0fffh
        and    esi, 0fffff000h
        and    edi, 0fffh
        sub    edi, 1000h
        neg    edi
        push   edi
        db  0E8h, 0AAh, 005h, 000h, 000h
loc_5_E9C5:
        mov    ebx, esi
        add    ebx, dword ptr [ebp + 8]
        db  0A1h, 06Ch, 0F6h, 000h, 000h
        shl    eax, 0ch
        cmp    ebx, eax
        jbe    SHORT loc_5_E9FE
        mov    edi, ebx
        sub    edi, eax
        sub    eax, esi
        mov    dword ptr [ebp - 4], eax
        add    edi, 0fffh
        shr    edi, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 400h
        push   eax
        push   edi
        db  0E8h, 040h, 00Fh, 000h, 000h
        inc    eax
        je     NEAR PTR loc_5_EB00
loc_5_E9FE:
        db  089h, 01Dh, 01Ch, 0E7h, 000h, 000h
        jmp    SHORT loc_5_EA1D
        db  08Bh, 0C6h, 003h, 045h, 008h, 08Bh, 0C8h, 087h, 003h, 02Bh, 0C8h, 029h, 04Bh, 004h, 073h, 007h
        db  0C7h, 043h, 004h, 000h, 000h, 000h, 000h
loc_5_EA1D:
        test   byte ptr [ebp + 0dh], 4
        je     SHORT loc_5_EA46
        test   byte ptr [ebp + 0ch], 8
        je     SHORT loc_5_EA93
        mov    ecx, dword ptr [ebp - 4]
        shr    ecx, 0ch
        jecxz  loc_5_EA93
        mov    edx, esi
        shr    edx, 0ch
        push   ecx
        push   edx
        db  0E8h, 092h, 00Fh, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_EA93
        jmp    NEAR PTR loc_5_EB00
loc_5_EA46:
        test   byte ptr [ebp + 0dh], 21h
        je     SHORT loc_5_EA83
        call   sub_5_EDC5
        jb     NEAR PTR loc_5_EB00
        mov    dword ptr [edi + 8], esi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [edi + 10h], 200h
        test   byte ptr [ebp + 0dh], 20h
        je     SHORT loc_5_EA74
        or     dword ptr [edi + 10h], 400h
loc_5_EA74:
        push   0
        push   edi
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_5_EB00
loc_5_EA83:
        test   byte ptr [ebp + 0dh], 2
        je     SHORT loc_5_EA93
        mov    edi, esi
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        cld
        rep stosb
loc_5_EA93:
        mov    eax, esi
loc_5_EA95:
        pop    edi
        pop    esi
        pop    ebx
        leave
        xor    ecx, ecx
        ret
        db  0F6h, 045h, 00Dh, 004h, 074h, 027h, 08Bh, 01Dh, 078h, 0F6h, 000h, 000h, 08Bh, 00Dh, 06Ch, 0F6h
        db  000h, 000h, 02Bh, 0D9h, 076h, 017h, 0C1h, 0E1h, 00Ch, 051h, 08Bh, 0CBh, 0C1h, 0E1h, 00Ch, 051h
        db  0E8h, 0AEh, 004h, 000h, 000h, 06Ah, 000h, 053h, 0E8h, 06Eh, 00Eh, 000h, 000h, 08Bh, 055h, 008h
        db  081h, 0C2h, 0FFh, 00Fh, 000h, 000h, 0C1h, 0EAh, 00Ch, 08Bh, 045h, 00Ch, 025h, 000h, 004h, 000h
        db  000h, 050h, 052h, 0E8h, 053h, 00Eh, 000h, 000h, 083h, 0F8h, 0FFh, 074h, 017h, 08Bh, 0F0h, 0C1h
        db  0E6h, 00Ch, 089h, 035h, 01Ch, 0E7h, 000h, 000h, 0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h, 0E9h
        db  08Dh, 0FEh, 0FFh, 0FFh
loc_5_EB00:
        xor    eax, eax
        jmp    SHORT loc_5_EA95
loc_5_EB04:
        sub    esp, 4
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        jecxz  loc_5_EB89
        add    ecx, eax
        dec    ecx
        db  0C1h, 0E8h
loc_5_EB18:
        or     al, 0c1h
        db  0E9h, 00Ch
loc_5_EB1C:
        db  03Bh, 005h, 070h, 0F6h, 000h, 000h
        ja     SHORT loc_5_EB2E
loc_5_EB24:
        db  03Bh, 00Dh, 06Ch, 0F6h, 000h, 000h
        jbe    SHORT loc_5_EB36
        jmp    SHORT loc_5_EB89
loc_5_EB2E:
        cmp    ecx, 100h
loc_5_EB34:
        jae    SHORT loc_5_EB89
loc_5_EB36:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, 20h
        xor    ebx, ebx
        mov    dword ptr [esp + 8], ebx
        db  0BBh, 074h, 0E7h, 000h, 000h
loc_5_EB4E:
        cmp    dword ptr [ebx + 4], 0
        jne    SHORT loc_5_EB61
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_5_EB73
        mov    dword ptr [esp + 8], ebx
        jmp    SHORT loc_5_EB73
loc_5_EB61:
        mov    esi, dword ptr [ebx]
        cmp    eax, esi
        jb     SHORT loc_5_EB73
        add    esi, dword ptr [ebx + 4]
        cmp    eax, esi
loc_5_EB6C:
        jne    SHORT loc_5_EB73
        add    dword ptr [ebx + 4], edx
        jmp    SHORT loc_5_EB8B
loc_5_EB73:
        add    ebx, 8
        loop   loc_5_EB4E
        mov    ebx, dword ptr [esp + 8]
loc_5_EB7C:
        test   ebx, ebx
        je     SHORT loc_5_EB89
loc_5_EB80:
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], edx
        mov    al, 1
        jmp    SHORT loc_5_EB8B
loc_5_EB89:
        xor    eax, eax
loc_5_EB8B:
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  060h, 0B9h, 020h, 000h, 000h, 000h, 0BBh, 074h, 0E7h, 000h, 000h
loc_5_EB9C:
        mov    eax, dword ptr [ebx + 4]
        cmp    eax, 1000h
        jb     SHORT loc_5_EBD5
        mov    edx, dword ptr [ebx]
        mov    esi, edx
        add    edx, 0fffh
        and    edx, 0fffff000h
        mov    edi, edx
        sub    edx, esi
        sub    eax, edx
loc_5_EBBC:
        shr    eax, 0ch
        je     SHORT loc_5_EBD5
        shr    edi, 0ch
        db  03Bh, 03Dh, 078h, 0F6h, 000h, 000h
        jb     SHORT loc_5_EBD5
        push   ecx
        push   eax
        push   edi
        db  0E8h, 0FBh, 00Dh, 000h, 000h
        pop    ecx
loc_5_EBD5:
        add    ebx, 8
        loop   loc_5_EB9C
        popad
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        push   esi
        push   edi
        test   byte ptr [ebp + 0ch], 1
        jne    NEAR PTR loc_5_ECAB
        db  083h, 03Dh, 020h, 0E7h, 000h, 000h, 000h
        jne    NEAR PTR loc_5_ECA7
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, 20h
        db  0BBh, 074h, 0E7h, 000h, 000h
        push   4
        call   sub_5_EDDE
        jae    SHORT loc_5_EC7C
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, 20h
        db  0BBh, 074h, 0E8h, 000h, 000h
        push   4
        call   sub_5_EDDE
        jae    SHORT loc_5_EC7C
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0fffh
        shr    ebx, 0ch
        db  08Bh, 035h, 06Ch, 0F6h, 000h, 000h
        lea    ecx, [ebx + esi]
        db  03Bh, 00Dh, 070h, 0F6h, 000h, 000h
        ja     SHORT loc_5_ECA7
        xor    eax, eax
        push   0ah
        push   eax
        push   ecx
        push   esi
        push   eax
        push   eax
        push   1
        push   ebx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     eax, eax
        db  0A3h, 024h, 0E7h, 000h, 000h
        je     SHORT loc_5_ECA7
        db  089h, 01Dh, 028h, 0E7h, 000h, 000h
        shr    edx, 0ch
        push   ebx
        push   esi
        push   edx
        db  0E8h, 09Bh, 013h, 0FBh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_EC94
        shl    esi, 0ch
loc_5_EC7C:
        db  0FFh, 005h, 020h, 0E7h, 000h, 000h
loc_5_EC82:
        mov    edi, esi
loc_5_EC84:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        cld
        rep stosb
loc_5_EC8C:
        mov    eax, esi
loc_5_EC8E:
        pop    edi
        pop    esi
loc_5_EC90:
        pop    edx
        pop    ebx
        leave
        ret
loc_5_EC94:
        xor    eax, eax
        db  087h, 005h, 024h, 0E7h, 000h, 000h
loc_5_EC9C:
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_5_ECA7:
        xor    eax, eax
        jmp    SHORT loc_5_EC8E
loc_5_ECAB:
        db  081h, 03Dh, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 010h
        jb     SHORT loc_5_ECA7
        db  083h, 03Dh, 06Ch, 0E7h, 000h, 000h, 010h
        je     SHORT loc_5_ECA7
        db  08Bh, 00Dh, 030h, 0EBh, 000h, 000h
loc_5_ECC6:
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0fffh
        shr    edi, 0ch
        mov    esi, ecx
        sub    esi, edi
        db  039h, 035h, 06Ch, 0F6h, 000h, 000h
        jae    SHORT loc_5_ECA7
        xor    eax, eax
loc_5_ECE0:
        push   0ah
        push   eax
        push   ecx
        push   esi
        push   eax
        push   eax
        push   1
        push   edi
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_5_ECA7
        mov    ecx, esi
        db  08Bh, 035h, 06Ch, 0E7h, 000h, 000h
        db  089h, 004h, 0B5h, 02Ch, 0E7h, 000h, 000h
        shr    edx, 0ch
        push   edi
        push   ecx
        push   edx
        db  0E8h, 0FCh, 012h, 0FBh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_ED39
        inc    esi
        db  089h, 035h, 06Ch, 0E7h, 000h, 000h
        db  029h, 03Dh, 030h, 0EBh, 000h, 000h
        db  08Bh, 035h, 030h, 0EBh, 000h, 000h
        db  001h, 03Dh, 070h, 0E7h, 000h, 000h
        shl    esi, 0ch
        jmp    NEAR PTR loc_5_EC82
loc_5_ED39:
        db  08Bh, 004h, 0B5h, 02Ch, 0E7h, 000h, 000h
        jmp    NEAR PTR loc_5_EC9C
        db  08Bh, 00Dh, 070h, 0E7h, 000h, 000h, 0E3h, 02Eh, 06Ah, 000h, 051h, 0FFh, 035h, 030h, 0EBh, 000h
        db  000h, 0E8h, 083h, 012h, 0FBh, 0FFh, 083h, 0C4h, 00Ch, 033h, 0F6h, 08Bh, 004h, 0B5h, 02Ch, 0E7h
        db  000h, 000h, 06Ah, 000h, 050h, 0CDh, 020h, 055h, 000h, 001h, 000h, 059h, 059h, 046h, 03Bh, 035h
        db  06Ch, 0E7h, 000h, 000h, 07Ch, 0E5h, 0C3h, 053h, 056h, 057h, 083h, 03Dh, 020h, 0E7h, 000h, 000h
        db  001h, 072h, 039h, 0FFh, 00Dh, 020h, 0E7h, 000h, 000h, 033h, 0F6h, 087h, 035h, 024h, 0E7h, 000h
        db  000h, 00Bh, 0F6h, 074h, 021h, 06Ah, 000h, 0FFh, 035h, 028h, 0E7h, 000h, 000h, 0FFh, 035h, 06Ch
        db  0F6h, 000h, 000h, 0E8h, 031h, 012h, 0FBh, 0FFh, 083h, 0C4h, 00Ch, 06Ah, 000h, 056h, 0CDh, 020h
        db  055h, 000h, 001h, 000h, 059h, 059h, 0B0h, 001h, 05Fh, 05Eh, 05Bh, 0C3h, 033h, 0C0h, 0EBh, 0F8h
sub_5_EDC5:
        push   ecx
        push   401h
        push   14h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        stc
        je     SHORT loc_5_EDDC
        xchg   edi, eax
        clc
loc_5_EDDC:
        pop    ecx
        ret
sub_5_EDDE:
        push   ebp
        mov    ebp, esp
        test   byte ptr [ebp + 9], 8
        je     SHORT loc_5_EDF2
        cmp    edx, dword ptr [ebx + 4]
        db  00Fh, 083h, 05Dh, 001h, 000h, 000h
loc_5_EDF0:
        jmp    SHORT loc_5_EDFB
loc_5_EDF2:
        cmp    edx, dword ptr [ebx + 4]
        db  00Fh, 087h, 052h, 001h, 000h, 000h
loc_5_EDFB:
        mov    esi, dword ptr [ebx]
        test   byte ptr [ebp + 9], 40h
        jne    SHORT loc_5_EE15
        test   byte ptr [ebp + 9], 10h
        db  075h, 01Dh
        db  081h, 03Dh, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 010h
        db  073h, 011h
loc_5_EE15:
        push   esi
        db  0C1h, 0EEh
_ITEXT ends
        end
