; cdfs_icode_02.asm -- CDFS object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0010:
        push   ebx
        db  079h, 073h
        je     SHORT loc_2_007A
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        jb     SHORT loc_2_0081
        outsb
        je     SHORT loc_2_0062
        outsd
        outsb
        je     SHORT loc_2_0095
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_2_0085
        inc    ebx
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        pop    esp
        inc    esi
        imul   ebp, dword ptr [ebp + 53h], 65747379h
        insd
        pop    esp
        inc    ebx
        inc    esp
        inc    esi
        push   ebx
        add    byte ptr [ebx + 61h], al
        arpl   word ptr [eax + 65h], bp
        push   ebx
        imul   edi, dword ptr [edx + 65h], 65725000h
        db  066h, 065h, 074h, 063h
        db  068h, 000h
loc_2_0054:
        push   eax
        db  072h, 065h
        je     NEAR PTR loc_2_00BE
        push   6c696154h
        db  000h
loc_2_0061:
        push   edx
loc_2_0062:
        popad
        ja     SHORT loc_2_00A8
        popad
        arpl   word ptr [eax + 65h], bp
        db  000h
loc_2_006A:
        inc    ebp
        db  078h, 074h
        outsb
        inc    ebp
        db  072h, 072h
        outsd
        jb     SHORT loc_2_00EB
        db  000h
loc_2_0079:
        push   ebx
loc_2_007A:
        push   esi
        inc    esp
        push   ebx
        outsb
        db  073h, 065h
loc_2_0081:
        add    byte ptr [ebx + 68h], dl
        outsd
loc_2_0085:
        db  077h, 056h
        db  065h, 072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 534d0073h
        inc    ebx
        inc    esp
        inc    ebp
        pop    eax
loc_2_0095:
        inc    ebx
        outsd
        insd
        db  070h, 061h
        db  074h, 04Eh
        popad
        insd
        jae    NEAR PTR loc_2_00A1
loc_2_00A1:
        dec    esi
        outsd
        push   edx
        popad
        inc    ecx
loc_2_00A8:
        push   646165h
loc_2_00AD:
        db  080h, 00Dh, 07Ch, 008h, 000h, 000h, 001h
        db  0C7h, 005h, 018h, 000h, 000h, 000h, 050h, 070h, 000h, 000h
loc_2_00BE:
        db  0C7h, 005h, 01Ch, 000h, 000h, 000h, 0D0h, 003h, 000h, 000h
        db  0CDh, 020h, 044h, 001h, 001h, 000h
        db  0A3h, 01Ch, 09Ch, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 010h, 000h
        jb     NEAR PTR loc_2_020A
        db  0CDh, 020h, 000h, 000h, 041h, 000h
        jb     NEAR PTR loc_2_020A
loc_2_00EB:
        cmp    eax, 400h
        jb     NEAR PTR loc_2_020A
        db  0CDh, 020h, 001h, 000h, 041h, 000h
        cmp    word ptr [eax], 4
        jne    NEAR PTR loc_2_020A
        db  0A3h, 090h, 008h, 000h, 000h
        movzx  ecx, word ptr [eax + 4]
        and    ecx, 3
        cmp    ecx, 1
        jne    NEAR PTR loc_2_020A
        cmp    word ptr [eax + 6], 22h
        jne    NEAR PTR loc_2_020A
        or     word ptr [eax + 4], 2
        db  080h, 00Dh, 07Ch, 008h, 000h, 000h, 002h
        movzx  ecx, word ptr [eax + 8]
        db  089h, 00Dh, 0FCh, 09Bh, 000h, 000h
        mov    ecx, dword ptr [eax + 0ch]
        db  089h, 00Dh, 08Ch, 008h, 000h, 000h
        shr    ecx, 0ch
        db  089h, 00Dh, 088h, 008h, 000h, 000h
        db  0F7h, 005h, 004h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        jne    SHORT loc_2_016E
        db  0BEh, 008h, 000h, 000h, 000h
        db  0CDh, 020h, 0EFh, 000h, 001h, 000h
        jb     SHORT loc_2_016E
        db  083h, 00Dh, 004h, 000h, 000h, 000h, 002h
loc_2_016E:
        push   1
        db  0E8h, 058h, 085h, 0FEh, 0FFh
        lea    esp, [esp + 4]
        db  0A3h, 098h, 008h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_01F8
        db  080h, 00Dh, 07Ch, 008h, 000h, 000h, 004h
        call   sub_2_020C
        db  080h, 00Dh, 07Ch, 008h, 000h, 000h, 010h
        db  068h, 020h, 007h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        lea    esp, [esp + 4]
        db  068h, 0DCh, 008h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 041h, 000h
        lea    esp, [esp + 4]
        or     eax, eax
        jne    SHORT loc_2_01F8
        db  080h, 00Dh, 07Ch, 008h, 000h, 000h, 020h
        db  083h, 00Dh, 004h, 000h, 000h, 000h, 001h
        db  0F7h, 005h, 004h, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        je     SHORT loc_2_01DB
        db  083h, 00Dh, 004h, 000h, 000h, 000h, 010h
loc_2_01DB:
        db  0F7h, 005h, 004h, 000h, 000h, 000h, 000h, 020h, 000h, 000h
        je     SHORT loc_2_01F1
        db  081h, 00Dh, 004h, 000h, 000h, 000h, 000h, 010h, 000h, 000h
loc_2_01F1:
        call   sub_2_06D0
        clc
        ret
loc_2_01F8:
        db  0E8h, 003h, 0FEh, 0FDh, 0FFh
        xor    eax, eax
        db  086h, 005h, 07Ch, 008h, 000h, 000h
        shr    al, 4
        cmc
        ret
loc_2_020A:
        stc
        ret
sub_2_020C:
        db  08Dh, 005h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 010h, 000h, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
        lea    esp, [esp + 0ch]
        or     eax, eax
        jne    NEAR PTR loc_2_05D5
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  08Dh, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 008h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 00Ch, 000h, 000h, 000h
        push   eax
        push   0
        db  068h, 041h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        lea    esp, [esp + 18h]
        or     eax, eax
        jne    NEAR PTR loc_2_05D6
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 004h
        je     SHORT loc_2_029A
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 003h
        jne    NEAR PTR loc_2_05D6
        db  083h, 03Dh, 004h, 000h, 000h, 000h, 004h
        jne    NEAR PTR loc_2_05D6
loc_2_029A:
        db  0A1h, 008h, 000h, 000h, 000h
        cmp    eax, 8
        jb     NEAR PTR loc_2_05D6
        db  0A3h, 000h, 09Ch, 000h, 000h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  08Dh, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 008h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 00Ch, 000h, 000h, 000h
        push   eax
        push   0
        db  068h, 04Bh, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        lea    esp, [esp + 18h]
        or     eax, eax
        jne    NEAR PTR loc_2_05D6
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 004h
        je     SHORT loc_2_0318
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 003h
        jne    NEAR PTR loc_2_05D6
        db  083h, 03Dh, 004h, 000h, 000h, 000h, 004h
        jne    NEAR PTR loc_2_05D6
loc_2_0318:
        db  0A1h, 008h, 000h, 000h, 000h
        db  0A3h, 004h, 09Ch, 000h, 000h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  08Dh, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 008h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 00Ch, 000h, 000h, 000h
        push   eax
        push   0
        db  068h, 054h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        lea    esp, [esp + 18h]
        or     eax, eax
        jne    NEAR PTR loc_2_05D6
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 004h
        je     SHORT loc_2_038D
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 003h
        jne    NEAR PTR loc_2_05D6
        db  083h, 03Dh, 004h, 000h, 000h, 000h, 004h
        jne    NEAR PTR loc_2_05D6
loc_2_038D:
        db  0A1h, 008h, 000h, 000h, 000h
        db  0A3h, 008h, 09Ch, 000h, 000h
loc_2_0397:
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  08Dh, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 008h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 00Ch, 000h, 000h, 000h
        push   eax
        push   0
        db  068h, 061h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        lea    esp, [esp + 18h]
        or     eax, eax
        jne    SHORT loc_2_0401
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 004h
        je     SHORT loc_2_03ED
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 003h
        jne    SHORT loc_2_0401
loc_2_03ED:
        db  0A1h, 008h, 000h, 000h, 000h
        cmp    eax, 4
        ja     SHORT loc_2_03FC
        mov    eax, 4
loc_2_03FC:
        db  0A3h, 00Ch, 09Ch, 000h, 000h
loc_2_0401:
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  08Dh, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 008h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 00Ch, 000h, 000h, 000h
        push   eax
        push   0
        db  068h, 06Ah, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        lea    esp, [esp + 18h]
        or     eax, eax
        jne    SHORT loc_2_0458
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 003h
        jne    SHORT loc_2_0458
        db  0A0h, 008h, 000h, 000h, 000h
        db  0A2h, 0E8h, 09Bh, 000h, 000h
loc_2_0458:
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  08Dh, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 008h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 00Ch, 000h, 000h, 000h
        push   eax
        push   0
        db  068h, 079h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        lea    esp, [esp + 18h]
        or     eax, eax
        jne    SHORT loc_2_04B5
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 003h
        jne    SHORT loc_2_04B5
        db  00Fh, 0B6h, 005h, 008h, 000h, 000h, 000h
        cmp    al, 2
        ja     SHORT loc_2_04B5
        db  0A2h, 078h, 008h, 000h, 000h
loc_2_04B5:
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  08Dh, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 008h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 00Ch, 000h, 000h, 000h
        push   eax
        push   0
        db  068h, 082h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        lea    esp, [esp + 18h]
        or     eax, eax
        jne    SHORT loc_2_050C
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 003h
        jne    SHORT loc_2_050C
        db  0A0h, 008h, 000h, 000h, 000h
        db  0A2h, 080h, 008h, 000h, 000h
loc_2_050C:
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  08Dh, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 008h, 000h, 000h, 000h
        push   eax
        db  08Dh, 005h, 00Ch, 000h, 000h, 000h
        push   eax
        push   0
        db  068h, 08Fh, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        lea    esp, [esp + 18h]
        or     eax, eax
        jne    SHORT loc_2_0563
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 003h
        jne    SHORT loc_2_0563
        db  0A0h, 008h, 000h, 000h, 000h
        db  0A2h, 0ECh, 09Bh, 000h, 000h
loc_2_0563:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        db  068h, 0A1h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        lea    esp, [esp + 18h]
        or     eax, eax
        jne    SHORT loc_2_05C5
        db  0A1h, 008h, 09Ch, 000h, 000h
        db  0A3h, 004h, 09Ch, 000h, 000h
        db  08Dh, 00Dh, 004h, 09Ch, 000h, 000h
        db  08Dh, 015h, 04Bh, 000h, 000h, 000h
        push   4
        push   ecx
        push   4
        push   0
        push   edx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        db  08Dh, 015h, 0A1h, 000h, 000h, 000h
        push   edx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 04Fh, 001h, 001h, 000h
        add    esp, 8
loc_2_05C5:
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        lea    esp, [esp + 4]
loc_2_05D5:
        ret
loc_2_05D6:
        call   sub_2_05E0
        jmp    NEAR PTR loc_2_0397
sub_2_05E0:
        push   ebx
        sub    esp, 40h
        mov    ebx, esp
        push   0
        push   ebx
        db  0CDh, 020h, 06Fh, 000h, 001h, 000h
        add    esp, 8
        mov    eax, dword ptr [ebx + 4]
        add    esp, 40h
        pop    ebx
        cmp    eax, 0b40h
        jae    SHORT loc_2_0647
        cmp    eax, 740h
        jae    SHORT loc_2_0627
        db  0C7h, 005h, 000h, 09Ch, 000h, 000h, 000h, 001h, 000h, 000h
        db  0C7h, 005h, 004h, 09Ch, 000h, 000h, 084h, 000h, 000h, 000h
        db  0C7h, 005h, 008h, 09Ch, 000h, 000h, 020h, 000h, 000h, 000h
        jmp    SHORT loc_2_0665
loc_2_0627:
        db  0C7h, 005h, 000h, 09Ch, 000h, 000h, 039h, 001h, 000h, 000h
        db  0C7h, 005h, 004h, 09Ch, 000h, 000h, 072h, 000h, 000h, 000h
        db  0C7h, 005h, 008h, 09Ch, 000h, 000h, 040h, 000h, 000h, 000h
        jmp    SHORT loc_2_0665
loc_2_0647:
        db  0C7h, 005h, 000h, 09Ch, 000h, 000h, 06Bh, 002h, 000h, 000h
        db  0C7h, 005h, 004h, 09Ch, 000h, 000h, 0E4h, 000h, 000h, 000h
        db  0C7h, 005h, 008h, 09Ch, 000h, 000h, 080h, 000h, 000h, 000h
loc_2_0665:
        db  08Dh, 00Dh, 000h, 09Ch, 000h, 000h
        db  08Dh, 015h, 041h, 000h, 000h, 000h
        push   4
        push   ecx
        push   4
        push   0
        push   edx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        db  08Dh, 00Dh, 004h, 09Ch, 000h, 000h
        db  08Dh, 015h, 04Bh, 000h, 000h, 000h
        push   4
        push   ecx
        push   4
        push   0
        push   edx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        db  08Dh, 00Dh, 008h, 09Ch, 000h, 000h
        db  08Dh, 015h, 054h, 000h, 000h, 000h
        push   4
        push   ecx
        push   4
        push   0
        push   edx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        ret
        db  000h
sub_2_06D0:
        db  0C7h, 005h, 0B4h, 09Ch, 000h, 000h, 043h, 044h, 046h, 053h
        db  066h, 0C7h, 005h, 0BAh, 09Ch, 000h, 000h, 048h, 000h
        db  0C7h, 005h, 0ACh, 09Ch, 000h, 000h, 000h, 010h, 000h, 000h
        db  0C6h, 005h, 0EFh, 09Ch, 000h, 000h, 004h
        db  0A1h, 01Ch, 09Ch, 000h, 000h
        db  0C7h, 005h, 040h, 09Dh, 000h, 000h, 050h, 09Dh, 000h, 000h
        mov    eax, dword ptr [eax]
        db  0A3h, 078h, 09Ch, 000h, 000h
        db  068h, 08Ch, 09Ch, 000h, 000h
        db  0E8h, 0DCh, 07Dh, 0FEh, 0FFh
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_0720:
        inc    ebx
        inc    esp
        inc    esi
        push   ebx
        and    byte ptr [edx + edx*2 + 50h], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        pushad
        db  005h, 000h, 000h, 0A8h, 008h
        add    byte ptr [eax], al
        inc    ebx
        outsd
        insd
        db  070h, 061h
        arpl   word ptr [eax + 44h], si
        imul   esi, dword ptr [ebx + 63h], 44534620h
        add    dword ptr [eax], eax
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
