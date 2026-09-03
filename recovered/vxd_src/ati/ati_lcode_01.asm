; ati_lcode_01.asm -- ATI object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

ATI_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_000F
        db  0FFh, 024h, 085h, 011h, 000h, 000h, 000h
loc_1_000F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_007D:
        push   edi
        db  080h, 03Dh, 0A3h, 001h, 000h, 000h, 000h
        jne    SHORT loc_1_00B2
        mov    edi, ebx
        db  003h, 03Dh, 088h, 001h, 000h, 000h
        test   ecx, 4
        je     SHORT loc_1_00B2
        cmp    ecx, 0ch
        jne    SHORT loc_1_00A2
        mov    word ptr [edi + 43h], ax
        jmp    SHORT loc_1_00B2
loc_1_00A2:
        cmp    ecx, 4
        jne    SHORT loc_1_00B2
        add    edi, 43h
        cmp    dl, 0eeh
        je     SHORT loc_1_00B0
        inc    edi
loc_1_00B0:
        mov    byte ptr [edi], al
loc_1_00B2:
        pop    edi
        db  0CDh, 020h, 015h, 080h, 00Ah, 000h
loc_1_00B9:
        push   edi
        db  080h, 03Dh, 0A3h, 001h, 000h, 000h, 000h
        jne    SHORT loc_1_00EE
        mov    edi, ebx
        db  003h, 03Dh, 088h, 001h, 000h, 000h
        test   ecx, 4
        je     SHORT loc_1_00EE
        cmp    ecx, 0ch
        jne    SHORT loc_1_00DE
        mov    word ptr [edi + 41h], ax
        jmp    SHORT loc_1_00EE
loc_1_00DE:
        cmp    ecx, 4
        jne    SHORT loc_1_00EE
        add    edi, 41h
        cmp    dl, 0e8h
        je     SHORT loc_1_00EC
        inc    edi
loc_1_00EC:
        mov    byte ptr [edi], al
loc_1_00EE:
        pop    edi
        db  0CDh, 020h, 015h, 080h, 00Ah, 000h
loc_1_00F5:
        or     eax, eax
        jne    SHORT loc_1_0100
        call   sub_1_010A
        jb     SHORT loc_1_0108
loc_1_0100:
        mov    eax, 0ffffffffh
        clc
        jmp    SHORT loc_1_0109
loc_1_0108:
        stc
loc_1_0109:
        ret
sub_1_010A:
        push   edi
        mov    edi, ebx
        db  003h, 03Dh, 088h, 001h, 000h, 000h
        mov    al, byte ptr [edi + 43h]
        or     al, byte ptr [edi + 41h]
        test   al, 1
        clc
        je     SHORT loc_1_011F
        stc
loc_1_011F:
        pop    edi
        ret
        db  000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 001h, 000h, 000h, 041h, 054h, 049h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  020h, 037h, 036h, 031h, 032h, 039h, 035h, 035h, 032h, 030h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_019C:
        inc    dword ptr [eax]
loc_1_019E:
        add    byte ptr [eax], al
loc_1_01A0:
        add    byte ptr [eax], al
loc_1_01A2:
        add    byte ptr [eax], al
loc_1_01A4:
        add    byte ptr [eax], al
loc_1_01A6:
        add    byte ptr [eax], al
_LTEXT ends
        end
