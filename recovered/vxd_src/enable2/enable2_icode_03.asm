; enable2_icode_03.asm -- ENABLE2 object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        call   sub_3_00AB
        jb     NEAR PTR loc_3_0098
        db  08Dh, 03Dh, 0BCh, 016h, 000h, 000h
        db  0C7h, 005h, 0E0h, 016h, 000h, 000h, 01Ch, 017h, 000h, 000h
        db  0B8h, 0DBh, 001h, 000h, 000h
        db  0A3h, 0E4h, 016h, 000h, 000h
        db  0CDh, 020h, 006h, 000h, 037h, 000h
        db  0A1h, 0DCh, 016h, 000h, 000h
        db  0A3h, 000h, 017h, 000h, 000h
        db  0A1h, 0E8h, 016h, 000h, 000h
        db  0A3h, 00Ch, 017h, 000h, 000h
        db  0A1h, 0ECh, 016h, 000h, 000h
        db  0A3h, 014h, 017h, 000h, 000h
        db  0A1h, 0F0h, 016h, 000h, 000h
        db  0A3h, 018h, 017h, 000h, 000h
        db  0A1h, 0F4h, 016h, 000h, 000h
        db  0A3h, 010h, 017h, 000h, 000h
        db  0A1h, 0F8h, 016h, 000h, 000h
        db  0A3h, 050h, 018h, 000h, 000h
        db  0A1h, 0D4h, 016h, 000h, 000h
        db  0A3h, 024h, 018h, 000h, 000h
        mov    eax, 370001h
        db  0BEh, 008h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0FCh, 016h, 000h, 000h
        jb     SHORT loc_3_0098
        db  0E8h, 062h, 00Ah, 0FDh, 0FFh
        jb     SHORT loc_3_0098
        db  089h, 035h, 069h, 017h, 000h, 000h
        clc
        ret
loc_3_0098:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 5
        db  0CDh, 020h, 003h, 000h, 037h, 000h
        stc
        ret
sub_3_00AB:
        db  0C7h, 005h, 072h, 017h, 000h, 000h, 018h, 000h, 000h, 000h
        db  0C7h, 005h, 08Ah, 017h, 000h, 000h, 008h, 000h, 000h, 000h
        db  0C7h, 005h, 092h, 017h, 000h, 000h, 01Ch, 000h, 000h, 000h
        db  0C7h, 005h, 0AEh, 017h, 000h, 000h, 00Eh, 000h, 000h, 000h
        db  0C7h, 005h, 0BCh, 017h, 000h, 000h, 008h, 000h, 000h, 000h
        db  0C7h, 005h, 0C4h, 017h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  0C7h, 005h, 0D0h, 017h, 000h, 000h, 018h, 000h, 000h, 000h
        db  0C7h, 005h, 0E8h, 017h, 000h, 000h, 030h, 000h, 000h, 000h
        db  0C7h, 005h, 018h, 018h, 000h, 000h, 00Ch, 000h, 000h, 000h
        mov    ebx, 100h
        db  0BFh, 072h, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        mov    ebx, 200h
        db  0BFh, 08Ah, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        mov    ebx, 40h
        db  0BFh, 092h, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        pushad
        db  0E8h, 092h, 009h, 0FDh, 0FFh
        popad
        mov    ebx, 400h
        db  0BFh, 0AEh, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        mov    ebx, 1
        db  0BFh, 0BCh, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        mov    ebx, 1000h
        db  0BFh, 0C4h, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        mov    ebx, 80h
        db  0BFh, 0D0h, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        mov    ebx, 8
        db  0BFh, 0E8h, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        mov    ebx, 800h
        db  0BFh, 018h, 018h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        db  0CDh, 020h, 002h, 000h, 037h, 000h
        db  0A3h, 004h, 017h, 000h, 000h
        db  0C7h, 005h, 0BCh, 016h, 000h, 000h, 0E5h, 001h, 000h, 000h
        db  0C7h, 005h, 0D4h, 016h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D8h, 016h, 000h, 000h, 0B1h, 014h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0CDh, 020h, 011h, 000h, 00Dh, 000h
        and    al, 0fh
        db  0A2h, 038h, 017h, 000h, 000h
        db  0A2h, 037h, 017h, 000h, 000h
        db  0A2h, 024h, 01Ah, 000h, 000h
        clc
        ret
        db  043h, 06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 028h, 043h, 029h, 020h, 04Dh, 069h
        db  063h, 072h, 06Fh, 073h, 06Fh, 066h, 074h, 020h, 043h, 06Fh, 072h, 070h, 02Eh, 020h, 031h, 039h
        db  039h, 033h, 02Ch, 020h, 031h, 039h, 039h, 034h, 02Eh, 020h, 020h, 041h, 06Ch, 06Ch, 020h, 072h
        db  069h, 067h, 068h, 074h, 073h, 020h, 072h, 065h, 073h, 065h, 072h, 076h, 065h, 064h, 02Eh, 000h
_ITEXT ends
        end
