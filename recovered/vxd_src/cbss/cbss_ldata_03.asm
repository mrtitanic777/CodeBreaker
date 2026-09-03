; cbss_ldata_03.asm -- CBSS object 3 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 0E0h, 078h, 060h
        db  032h, 0E0h, 021h, 0E0h, 037h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 037h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 001h, 000h, 041h, 001h, 000h, 000h, 000h, 000h, 01Fh, 008h, 000h, 000h
        db  001h, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h, 010h, 000h, 000h, 0FFh, 0FFh, 0FEh, 000h, 000h, 010h
        db  000h, 000h, 000h, 010h, 000h, 000h, 000h, 010h, 000h, 000h, 072h, 008h, 01Fh, 008h, 000h, 000h
        db  001h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 010h, 000h, 000h, 0FFh, 0FFh, 0FEh, 000h, 000h, 010h
        db  000h, 000h, 000h, 010h, 000h, 000h, 000h, 010h, 000h, 000h, 072h, 008h, 01Fh, 000h, 000h, 000h
        db  000h, 000h, 0FFh, 0FFh, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h
        db  000h, 000h, 010h, 000h, 042h, 052h, 062h, 072h, 0FFh, 032h, 02Ah, 022h, 00Ah, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
sub_3_00B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_00C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_00C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esp
        adc    byte ptr [edx], dl
        lodsb
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
        dec    esp
        db  010h, 015h, 0ACh, 000h, 000h, 000h
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
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + edx + 13h], cl
        lodsb
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
        adc    edx, dword ptr [eax]
        adc    byte ptr [ecx], dl
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
        db  079h, 011h
        or     al, byte ptr [esi]
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
        adc    byte ptr [ecx], 66h
        db  004h, 000h
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
        db  000h, 0B3h, 010h, 006h, 031h, 000h
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
        add    byte ptr [ebp + 10h], al
        and    al, 0c8h
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
        inc    ebp
        adc    byte ptr [eax + ecx*8], dl
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
loc_3_023C:
        adc    al, 2
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 018h, 0FFh
        db  00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 020h, 0FFh, 00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 028h, 0FFh
        db  00Fh, 0FFh, 00Fh, 000h, 000h, 006h, 030h, 0FFh, 00Fh, 0FFh, 00Fh, 000h, 000h, 000h, 000h, 043h
        db  061h, 072h, 064h, 042h, 075h, 073h, 020h, 053h, 06Fh, 063h, 06Bh, 065h, 074h, 020h, 053h, 065h
        db  072h, 076h, 069h, 063h, 065h, 073h, 020h, 028h, 043h, 029h, 020h, 043h, 06Fh, 070h, 079h, 072h
        db  069h, 067h, 068h, 074h, 020h, 04Dh, 049h, 043h, 052h, 04Fh, 053h, 04Fh, 046h, 054h, 020h, 043h
        db  06Fh, 072h, 070h, 02Eh, 02Ch, 020h, 031h, 039h, 039h, 035h, 02Eh, 020h, 041h, 06Ch, 06Ch, 020h
        db  052h, 069h, 067h, 068h, 074h, 073h, 020h, 052h, 065h, 073h, 065h, 072h, 076h, 065h, 064h, 000h
        db  000h, 000h, 000h, 043h, 042h, 053h, 053h, 049h, 052h, 051h, 04Dh, 061h, 070h, 000h, 000h, 043h
        db  042h, 053h, 053h, 04Fh, 070h, 074h, 069h, 06Fh, 06Eh, 073h, 000h, 043h, 042h, 053h, 053h, 046h
        db  06Ch, 061h, 067h, 073h, 000h
_LDATA ends
        end
