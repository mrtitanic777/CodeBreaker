; cdvsd_icode_02.asm -- CDVSD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        call   sub_2_0030
        db  068h, 048h, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        add    esp, 4
        db  066h, 083h, 03Dh, 074h, 000h, 000h, 000h, 001h
        je     SHORT loc_2_002B
        db  066h, 083h, 03Dh, 074h, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_0029
        jmp    SHORT loc_2_002B
loc_2_0029:
        jmp    SHORT loc_2_002E
loc_2_002B:
        xor    eax, eax
        ret
loc_2_002E:
        stc
        ret
sub_2_0030:
        db  0C7h, 005h, 054h, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0C7h, 005h, 058h, 000h, 000h, 000h, 054h, 00Fh, 000h, 000h
        ret
        db  000h, 000h, 000h
loc_2_0048:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
loc_2_0054:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0058:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebx
        push   ebx
        dec    ecx
        xor    ah, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        push   esi
        push   ebx
        inc    esp
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_ITEXT ends
        end
