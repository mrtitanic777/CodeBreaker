; vtd_pcode_06.asm -- VTD object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_6_0000:
        db  003h, 01Dh, 0F4h, 004h, 000h, 000h
        mov    dword ptr [ebx + 4], 10000h
        mov    dword ptr [ebx + 8], 10000h
        mov    byte ptr [ebx + 17h], 6
        clc
        ret
loc_6_001A:
        xchg   ebx, eax
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        cmp    eax, ebx
        db  00Fh, 084h, 0B4h, 001h, 0FAh, 0FFh
        clc
        ret
_PTEXT ends
        end
