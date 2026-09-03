; vserver_icode_03.asm -- VSERVER object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  0FCh, 00Fh, 0B7h, 03Dh, 00Ch, 000h, 000h, 000h, 003h, 03Dh, 004h, 000h, 000h, 000h, 08Bh, 0F7h
        db  0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 032h, 0C0h, 0F2h, 0AEh, 0F7h, 0D9h, 049h, 051h, 052h, 051h, 0E8h
        db  092h, 08Ch, 0FCh, 0FFh, 05Ah, 05Ah, 059h, 085h, 0C0h, 074h, 004h, 08Bh, 0F8h, 0F3h, 0A4h, 0A3h
        db  0E0h, 024h, 001h, 000h, 0B9h, 00Dh, 000h, 000h, 000h, 0BEh, 05Ch, 026h, 001h, 000h, 0BFh, 0BDh
        db  024h, 001h, 000h, 0F3h, 0A4h, 0C7h, 005h, 0ACh, 024h, 001h, 000h, 000h, 01Ch, 001h, 000h, 0C7h
        db  005h, 0B0h, 024h, 001h, 000h, 000h, 01Ch, 001h, 000h, 0C7h, 005h, 000h, 01Ch, 001h, 000h, 0ACh
        db  024h, 001h, 000h, 0C7h, 005h, 004h, 01Ch, 001h, 000h, 0ACh, 024h, 001h, 000h, 0C7h, 005h, 028h
        db  01Ch, 001h, 000h, 0A0h, 023h, 001h, 000h, 0C7h, 005h, 01Ch, 01Ch, 001h, 000h, 084h, 023h, 001h
        db  000h, 0C7h, 005h, 020h, 01Ch, 001h, 000h, 0A0h, 023h, 001h, 000h, 0C7h, 005h, 024h, 01Ch, 001h
        db  000h, 03Ah, 024h, 001h, 000h, 033h, 0C0h, 0B9h, 000h, 004h, 000h, 000h, 0BFh, 01Ch, 03Ah, 001h
        db  000h, 0F3h, 0ABh, 0B9h, 000h, 004h, 000h, 000h, 0BFh, 01Ch, 04Ah, 001h, 000h, 0F3h, 0AAh, 0C7h
        db  005h, 0F4h, 01Bh, 001h, 000h, 000h, 000h, 000h, 000h, 0F6h, 005h, 038h, 034h, 001h, 000h, 001h
        db  074h, 031h, 0CDh, 020h, 003h, 000h, 014h, 000h, 033h, 0DBh, 083h, 0FAh, 000h, 074h, 024h, 0D1h
        db  0EAh, 073h, 01Dh, 052h, 053h, 06Ah, 001h, 053h, 0E8h, 032h, 07Bh, 0FCh, 0FFh, 083h, 0C4h, 008h
        db  05Bh, 053h, 068h, 0DCh, 0E9h, 000h, 000h, 0E8h, 0EFh, 0E7h, 0FCh, 0FFh, 083h, 0C4h, 008h, 05Ah
        db  043h, 0EBh, 0D7h, 0F8h, 0C3h, 000h, 000h, 000h
sub_3_00F8:
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_3_013D
        cmp    eax, 22h
        jne    SHORT loc_3_0147
        db  0CDh, 020h, 000h, 000h, 080h, 004h
        jb     NEAR PTR loc_3_01D4
        cmp    eax, 86h
        jne    NEAR PTR loc_3_01D6
        db  0CDh, 020h, 000h, 000h, 014h, 000h
        jb     SHORT loc_3_018A
        cmp    eax, 30ah
        jb     SHORT loc_3_0194
        db  0CDh, 020h, 001h, 000h, 080h, 004h
        jb     NEAR PTR loc_3_0216
        db  0A3h, 020h, 031h, 001h, 000h
        ret
loc_3_013D:
        mov    eax, 17eah
        jmp    NEAR PTR loc_3_0216
loc_3_0147:
        db  0C7h, 005h, 0BCh, 030h, 001h, 000h, 0EBh, 017h, 000h, 000h
        movzx  ecx, ah
        db  089h, 00Dh, 0C8h, 030h, 001h, 000h
        mov    cl, al
        db  089h, 00Dh, 0CCh, 030h, 001h, 000h
        db  0C7h, 005h, 0D0h, 030h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D4h, 030h, 001h, 000h, 022h, 000h, 000h, 000h
        db  066h, 083h, 005h, 0C4h, 030h, 001h, 000h, 010h
        db  0C6h, 005h, 0C6h, 030h, 001h, 000h, 004h
        jmp    NEAR PTR loc_3_0234
loc_3_018A:
        mov    eax, 17ddh
        jmp    NEAR PTR loc_3_0216
loc_3_0194:
        db  0C7h, 005h, 0BCh, 030h, 001h, 000h, 0DEh, 017h, 000h, 000h
        movzx  ecx, ah
        db  089h, 00Dh, 0C8h, 030h, 001h, 000h
        mov    cl, al
        db  089h, 00Dh, 0CCh, 030h, 001h, 000h
        db  0C7h, 005h, 0D0h, 030h, 001h, 000h, 003h, 000h, 000h, 000h
        db  0C7h, 005h, 0D4h, 030h, 001h, 000h, 00Ah, 000h, 000h, 000h
        db  066h, 083h, 005h, 0C4h, 030h, 001h, 000h, 010h
        db  0C6h, 005h, 0C6h, 030h, 001h, 000h, 004h
        jmp    SHORT loc_3_0234
loc_3_01D4:
        stc
        ret
loc_3_01D6:
        db  0C7h, 005h, 0BCh, 030h, 001h, 000h, 0DCh, 017h, 000h, 000h
        movzx  ecx, ah
        db  089h, 00Dh, 0C8h, 030h, 001h, 000h
        mov    cl, al
        db  089h, 00Dh, 0CCh, 030h, 001h, 000h
        db  0C7h, 005h, 0D0h, 030h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D4h, 030h, 001h, 000h, 086h, 000h, 000h, 000h
        db  066h, 083h, 005h, 0C4h, 030h, 001h, 000h, 010h
        db  0C6h, 005h, 0C6h, 030h, 001h, 000h, 004h
        jmp    SHORT loc_3_0234
loc_3_0216:
        db  0A3h, 0BCh, 030h, 001h, 000h
        db  0C7h, 005h, 0C8h, 030h, 001h, 000h, 0D4h, 017h, 000h, 000h
        db  066h, 083h, 005h, 0C4h, 030h, 001h, 000h, 004h
        db  0C6h, 005h, 0C6h, 030h, 001h, 000h, 004h
loc_3_0234:
        db  0E8h, 023h, 08Ch, 0FCh, 0FFh
        stc
        ret
sub_3_023B:
        push   ebx
        push   ecx
        push   edx
        db  00Fh, 0B7h, 01Dh, 00Eh, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 010h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 048h, 000h
        or     eax, eax
        je     SHORT loc_3_0277
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0CDh, 020h, 001h, 000h, 048h, 000h
        add    esp, 18h
loc_3_0277:
        or     eax, eax
        je     NEAR PTR loc_3_047C
        db  0A3h, 0C0h, 031h, 001h, 000h
        db  00Fh, 0B7h, 01Dh, 012h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 014h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 016h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 01Ch, 028h, 001h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 0C0h, 031h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 018h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 01Ah, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 01Ch, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 0ECh, 022h, 001h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 0C0h, 031h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 01Eh, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 020h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 022h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 0F4h, 022h, 001h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 0C0h, 031h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 024h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 026h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 028h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 0F4h, 027h, 001h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 0C0h, 031h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 02Ah, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 02Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 02Eh, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 0E0h, 031h, 001h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   2
        push   0
        push   esp
        db  0FFh, 035h, 0C0h, 031h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 030h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 032h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 034h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 0E4h, 031h, 001h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   2
        push   0
        push   esp
        db  0FFh, 035h, 0C0h, 031h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 036h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 038h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 03Ah, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 0E8h, 031h, 001h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   2
        push   0
        push   esp
        db  0FFh, 035h, 0C0h, 031h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
loc_3_047C:
        pop    edx
        pop    ecx
        pop    ebx
        ret
sub_3_0480:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  089h, 01Dh, 014h, 031h, 001h, 000h
        xor    ebx, ebx
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        db  088h, 01Dh, 0CAh, 01Ch, 001h, 000h
        db  0E8h, 0D6h, 089h, 0FCh, 0FFh
        jb     NEAR PTR loc_3_0574
        call   sub_3_0648
        db  0A2h, 0C9h, 01Ch, 001h, 000h
        db  0A1h, 020h, 031h, 001h, 000h
        mov    ecx, dword ptr [eax + 30h]
        not    ecx
        db  089h, 00Dh, 01Ch, 031h, 001h, 000h
        mov    ecx, dword ptr [eax + 34h]
        db  089h, 00Dh, 0B8h, 030h, 001h, 000h
        mov    ecx, dword ptr [eax + 38h]
        db  089h, 00Dh, 00Ch, 031h, 001h, 000h
        imul   ecx, dword ptr [eax + 40h], 0ea60h
        dec    ecx
        db  089h, 00Dh, 0A0h, 01Bh, 001h, 000h
        imul   ecx, dword ptr [eax + 3ch], 3ch
        dec    ecx
        db  089h, 00Dh, 048h, 01Ah, 001h, 000h
        mov    edx, dword ptr [eax + 4]
        xor    eax, eax
        mov    ecx, 0ffffffffh
        mov    edi, edx
        repne scasb
        not    ecx
        db  089h, 00Dh, 0A2h, 024h, 001h, 000h
        dec    ecx
        mov    eax, 10h
        sub    eax, ecx
        ja     SHORT loc_3_0514
        mov    ecx, 0fh
        mov    eax, 1
loc_3_0514:
        push   eax
        push   ecx
        mov    esi, edx
        db  0BFh, 048h, 023h, 001h, 000h
        db  0E8h, 08Eh, 0DFh, 0FCh, 0FFh
        mov    ecx, eax
        mov    al, 20h
        rep stosb
        pop    ecx
        pop    eax
        mov    esi, edx
        db  0BFh, 092h, 024h, 001h, 000h
        db  0E8h, 07Ah, 0DFh, 0FCh, 0FFh
        mov    ecx, eax
        sub    eax, eax
        rep stosb
        sub    eax, eax
        not    eax
        call   0
        call   sub_3_023B
        db  0C7h, 005h, 0C0h, 030h, 001h, 000h, 032h, 000h, 000h, 000h
        db  0E8h, 003h, 089h, 0FCh, 0FFh
        clc
        ret
        db  0B8h, 0E3h, 017h, 000h, 000h, 0A3h, 0BCh, 030h, 001h, 000h, 066h, 083h, 005h, 0C4h, 030h, 001h
        db  000h, 000h, 0C6h, 005h, 0C6h, 030h, 001h, 000h, 000h
loc_3_0574:
        db  0E8h, 0E3h, 088h, 0FCh, 0FFh
        stc
        ret
sub_3_057B:
        db  0F7h, 005h, 038h, 034h, 001h, 000h, 002h, 000h, 000h, 000h
        jne    NEAR PTR loc_3_0644
        db  0E8h, 008h, 0E0h, 0FCh, 0FFh
        or     eax, eax
        je     NEAR PTR loc_3_0644
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 58h
        mov    byte ptr [ebp + 1ch], 0d4h
        mov    word ptr [ebp + 18h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        sub    ebx, ebx
        movzx  eax, byte ptr [ebp + 1ch]
        movzx  edx, word ptr [ebp + 14h]
        movzx  ecx, word ptr [ebp + 18h]
        cmp    cx, 3fcfh
        jne    SHORT loc_3_05EB
        or     eax, eax
        je     SHORT loc_3_05EB
        cmp    edx, 2
        jne    SHORT loc_3_05EB
        db  083h, 00Dh, 038h, 034h, 001h, 000h, 040h
loc_3_05EB:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0E8h, 042h, 0DCh, 0FCh, 0FFh
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 40000000h
        jae    SHORT loc_3_062F
        db  0CDh, 020h, 003h, 000h, 014h, 000h
        test   edx, edx
        je     SHORT loc_3_062F
        db  0F7h, 005h, 038h, 034h, 001h, 000h, 040h, 000h, 000h, 000h
        jne    SHORT loc_3_062F
        db  083h, 00Dh, 038h, 034h, 001h, 000h, 001h
loc_3_062F:
        call   sub_3_00F8
        jb     SHORT loc_3_0645
        call   sub_3_0480
        jb     SHORT loc_3_0645
        db  083h, 00Dh, 038h, 034h, 001h, 000h, 002h
loc_3_0644:
        clc
loc_3_0645:
        ret
        db  000h, 000h
sub_3_0648:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 34h], eax
        db  0C7h, 045h, 0D0h, 010h, 037h, 001h, 000h
        mov    dword ptr [ebp - 2ch], 80000002h
        push   dword ptr [ebp - 34h]
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_0684
        sub    eax, eax
        leave
        ret
        db  08Bh, 0FFh
loc_3_0684:
        mov    dword ptr [ebp - 28h], 1
        lea    eax, [ebp - 28h]
        mov    dword ptr [ebp - 24h], eax
        lea    eax, [ebp - 1]
        mov    dword ptr [ebp - 20h], eax
        lea    eax, [ebp - 38h]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 18h], 0
        db  0C7h, 045h, 0ECh, 03Ch, 037h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_3_06DA
        mov    byte ptr [ebp - 1], 0
loc_3_06DA:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        movzx  eax, byte ptr [ebp - 1]
        leave
        ret
_ITEXT ends
        end
