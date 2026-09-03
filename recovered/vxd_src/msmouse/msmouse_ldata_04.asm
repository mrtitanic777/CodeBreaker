; msmouse_ldata_04.asm -- MSMOUSE object 4 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  000h
sub_4_0001:
        salc
        add    dword ptr [eax], eax
        db  000h, 022h
        add    al, byte ptr [eax]
        db  000h, 04Dh, 002h
        add    byte ptr [eax], al
        aam    1
        add    byte ptr [eax], al
        db  0A1h, 002h, 000h, 000h, 0A9h
        add    al, byte ptr [eax]
        db  000h, 017h
        add    eax, dword ptr [eax]
        db  000h, 02Fh
        add    eax, dword ptr [eax]
        db  000h, 046h, 003h
        add    byte ptr [eax], al
        mov    bh, 3
        add    byte ptr [eax], al
        fadd   qword ptr [ebx]
        add    byte ptr [eax], al
loc_4_002D:
        add    byte ptr [edx], al
        add    eax, dword ptr [ecx]
loc_4_0031:
        add    byte ptr [ecx + ecx], al
        or     eax, 1f1b1612h
        and    ebp, dword ptr [eax]
        sub    al, 31h
        xor    eax, 46423e39h
        dec    ebx
        dec    edi
        push   ebx
        push   edi
        pop    ebx
        pushad
        push   7874706ch
        db  07Ch, 080h
        add    dword ptr [edi - 696d7075h], -67h
        popfd
        mov    al, byte ptr [0aeaba7a4h]
        mov    cl, 0b4h
        mov    bh, 0bah
        mov    esi, 0c9c6c3c0h
        int3
        into
        rcl    ebx, 1
        salc
        fcomp  st(3)
        fstp   st(7)
        db  0E1h, 0E3h
        in     eax, 0e7h
        db  0E9h, 0EBh, 0ECh, 0EEh, 0F0h
        db  0F1h, 0F3h, 0F4h, 0F5h, 0F6h, 0F7h, 0F8h, 0F9h, 0FAh, 0FBh, 0FCh, 0FDh, 0FDh, 0FEh, 0FEh, 0FEh
        db  0FFh, 0FFh, 0FFh, 0FFh, 009h, 00Dh, 015h, 001h, 001h, 005h, 007h, 009h, 00Bh, 025h, 027h, 029h
        db  02Bh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 001h, 009h, 00Ch, 00Fh, 012h, 015h, 018h, 01Bh
        db  01Eh, 021h, 025h, 029h, 02Eh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 001h, 002h, 003h, 004h, 005h, 007h, 009h, 00Bh
        db  00Dh, 010h, 013h, 016h, 019h, 01Ch, 01Fh, 023h, 027h, 02Bh, 030h, 035h, 03Ch, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 001h, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 010h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Ch
        db  030h, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 010h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Ch
        db  030h, 034h, 038h, 03Ch, 040h, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 010h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Ch
        db  030h, 034h, 038h, 03Ch, 040h, 044h, 048h, 04Ch, 050h, 054h, 058h, 05Ch, 060h, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 010h, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 053h, 06Ch, 06Fh, 077h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 04Dh, 06Fh, 064h, 065h, 072h, 061h, 074h, 065h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 046h, 061h, 073h, 074h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 055h, 06Eh, 061h, 063h, 063h, 065h, 06Ch, 065h
        db  072h, 061h, 074h, 065h, 064h, 020h, 020h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 04Fh, 055h, 053h, 045h, 000h, 04Dh, 06Fh
        db  075h, 073h, 065h, 000h, 04Dh, 06Fh, 075h, 073h, 065h, 049h, 06Eh, 069h, 000h, 04Dh, 053h, 049h
        db  04Eh, 050h, 055h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_0284:
        db  0E8h, 004h, 000h, 000h, 0F0h
        add    al, 0
        db  000h, 0F6h
        add    al, 0
        db  000h, 06Fh, 005h
        add    byte ptr [eax], al
        outsd
        db  005h, 000h, 000h, 06Fh, 005h
        add    byte ptr [eax], al
        into
        push   es
        add    byte ptr [eax], al
        db  081h, 005h, 000h, 000h, 07Bh, 005h, 000h, 000h, 0A5h, 005h
        add    byte ptr [eax], al
        db  07Bh, 005h
        add    byte ptr [eax], al
        int3
        db  005h, 000h, 000h, 07Bh, 005h
        add    byte ptr [eax], al
        db  0F6h, 005h, 000h, 000h, 07Bh, 005h, 000h
        db  000h, 003h
        push   es
        add    byte ptr [eax], al
        rol    byte ptr [esi], 1
        add    byte ptr [eax], al
        and    byte ptr [esi], al
        add    byte ptr [eax], al
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
        pop    ebp
        push   es
        add    byte ptr [eax], al
        jb     SHORT loc_4_02DD
        add    byte ptr [eax], al
        js     SHORT loc_4_02E0
        add    byte ptr [eax], al
        cwde
loc_4_02DD:
        pop    es
        add    byte ptr [eax], al
loc_4_02E0:
        outsd
        db  005h, 000h, 000h, 06Fh, 005h
        add    byte ptr [eax], al
        outsd
        db  005h, 000h, 000h, 06Fh, 005h
        add    byte ptr [eax], al
        outsd
        db  005h, 000h, 000h, 06Fh, 005h
        add    byte ptr [eax], al
        mov    bl, 6
        add    byte ptr [eax], al
        db  0E9h, 007h, 000h, 000h, 06Fh
        db  005h, 000h, 000h, 06Fh, 005h, 000h, 000h, 06Fh, 005h, 000h, 000h, 06Fh, 005h, 000h, 000h, 06Fh
        db  005h, 000h, 000h, 06Fh, 005h, 000h, 000h, 001h, 005h, 007h, 009h, 00Bh, 025h, 027h, 029h, 02Bh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 001h, 009h, 00Ch, 00Fh, 012h, 015h, 018h, 01Bh, 01Eh
        db  021h, 025h, 029h, 02Eh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 001h, 002h, 003h, 004h, 005h, 007h, 009h, 00Bh, 00Dh
        db  010h, 013h, 016h, 019h, 01Ch, 01Fh, 023h, 027h, 02Bh, 030h, 035h, 03Ch, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 001h, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 023h, 04Dh, 053h, 04Dh, 049h, 04Eh, 049h, 020h, 056h
        db  069h, 072h, 074h, 075h, 061h, 06Ch, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 009h, 028h, 056h
        db  065h, 072h, 073h, 069h, 06Fh, 06Eh, 020h, 034h, 02Eh, 030h, 029h, 000h, 000h, 00Ah, 04Dh, 053h
        db  04Dh, 049h, 04Eh, 049h, 05Fh, 044h, 044h, 042h, 001h, 000h, 000h, 0FFh, 010h, 000h, 0FFh, 001h
        db  000h, 030h, 000h, 0E4h, 001h, 000h, 000h, 0E4h, 001h, 034h, 000h, 056h, 053h, 05Fh, 056h, 045h
        db  052h, 053h, 049h, 04Fh, 04Eh, 05Fh, 049h, 04Eh, 046h, 04Fh, 000h, 0BDh, 004h, 0EFh, 0FEh, 000h
        db  000h, 001h, 000h, 000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h, 000h, 000h, 004h, 000h, 0B6h
        db  003h, 000h, 000h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 005h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 075h
        db  001h, 000h, 000h, 053h, 074h, 072h, 069h, 06Eh, 067h, 046h, 069h, 06Ch, 065h, 049h, 06Eh, 066h
        db  06Fh, 000h, 000h, 061h, 001h, 000h, 000h, 030h, 034h, 030h, 039h, 030h, 034h, 045h, 034h, 000h
        db  000h, 000h, 000h, 026h, 000h, 016h, 000h, 043h, 06Fh, 06Dh, 070h, 061h, 06Eh, 079h, 04Eh, 061h
        db  06Dh, 065h, 000h, 04Dh, 069h, 063h, 072h, 06Fh, 073h, 06Fh, 066h, 074h, 020h, 043h, 06Fh, 072h
        db  070h, 06Fh, 072h, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 000h, 000h, 039h, 000h, 025h, 000h, 046h
        db  069h, 06Ch, 065h, 044h, 065h, 073h, 063h, 072h, 069h, 070h, 074h, 069h, 06Fh, 06Eh, 000h, 04Dh
        db  053h, 04Dh, 049h, 04Eh, 049h, 020h, 056h, 069h, 072h, 074h, 075h, 061h, 06Ch, 020h, 044h, 065h
        db  076h, 069h, 063h, 065h, 020h, 020h, 028h, 056h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 020h, 034h
        db  02Eh, 030h, 029h, 000h, 000h, 000h, 000h, 019h, 000h, 009h, 000h, 046h, 069h, 06Ch, 065h, 056h
        db  065h, 072h, 073h, 069h, 06Fh, 06Eh, 000h, 034h, 02Eh, 030h, 030h, 02Eh, 039h, 035h, 030h, 000h
        db  000h, 000h, 000h, 01Bh, 000h, 007h, 000h, 049h, 06Eh, 074h, 065h, 072h, 06Eh, 061h, 06Ch, 04Eh
        db  061h, 06Dh, 065h, 000h, 000h, 000h, 000h, 04Dh, 053h, 04Dh, 049h, 04Eh, 049h, 000h, 000h
_LDATA ends
        end
