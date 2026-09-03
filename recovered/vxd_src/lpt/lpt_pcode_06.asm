; lpt_pcode_06.asm -- LPT object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_6_0000:
        db  0CDh, 020h, 019h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_6_0008:
        db  0CDh, 020h, 021h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_6_0010:
        db  0CDh, 020h, 022h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_6_0018:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_6_0020:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_6_0028:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    8
        db  000h, 000h, 000h
sub_6_0040:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_6_005C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h
sub_6_006C:
        db  0CDh, 020h, 055h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_6_0074:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_6_007C:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_6_0084:
        db  0CDh, 020h, 03Eh, 080h, 033h, 020h
        dec    esp
        push   eax
_PTEXT ends
        end
