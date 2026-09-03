; biosxlat_pcode_05.asm -- BIOSXLAT object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, edx
        mov    edx, dword ptr [ebp + 2ch]
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   dh, 2
        je     SHORT loc_5_001C
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_5_001C:
        db  0CDh, 020h, 086h, 080h, 001h, 000h
loc_5_0022:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ah, 13h
        je     SHORT loc_5_005B
        cmp    ah, 10h
        jne    0
        cmp    al, 2
        je     SHORT loc_5_0050
        cmp    al, 9
        je     SHORT loc_5_0050
        cmp    al, 12h
        je     SHORT loc_5_0045
        cmp    al, 17h
        jne    0
loc_5_0045:
        db  0BAh, 007h, 000h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_5_0050:
        db  0BAh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_5_005B:
        db  0BAh, 010h, 000h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 025h
_PTEXT ends
        end
