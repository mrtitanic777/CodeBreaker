; pccard_pcode_04.asm -- PCCARD object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        mov    ecx, dword ptr [esp + 4]
        db  00Fh, 0B7h, 004h, 04Dh, 00Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
_PTEXT ends
        end
