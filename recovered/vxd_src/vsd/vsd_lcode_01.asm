; vsd_lcode_01.asm -- VSD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VSD_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 0dh
        je     SHORT loc_1_001A
        cmp    eax, 0bh
        je     SHORT sub_1_0032
        cmp    eax, 6
        je     SHORT loc_1_003A
        clc
        ret
loc_1_001A:
        db  03Bh, 01Dh, 014h, 002h, 000h, 000h
        jne    SHORT loc_1_0030
        call   sub_1_0032
        db  003h, 01Dh, 010h, 002h, 000h, 000h
        mov    byte ptr [ebx], 2
loc_1_0030:
        clc
        ret
sub_1_0032:
        db  039h, 01Dh, 014h, 002h, 000h, 000h
        jne    SHORT loc_1_0051
loc_1_003A:
        in     al, 61h
        and    al, 0feh
        jmp    SHORT loc_1_0040
loc_1_0040:
        out    61h, al
        db  0C7h, 005h, 014h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_00E4
loc_1_0051:
        clc
        ret
loc_1_0053:
        db  08Bh, 035h, 010h, 002h, 000h, 000h
        mov    edx, eax
        in     al, 61h
        and    al, 3
        cmp    al, 3
        je     SHORT loc_1_0098
        db  083h, 03Dh, 014h, 002h, 000h, 000h, 000h
        jne    SHORT loc_1_0073
        cmp    byte ptr [esi + ebx], 2
        je     SHORT loc_1_0085
loc_1_0072:
        ret
loc_1_0073:
        call   sub_1_00E4
        xor    eax, eax
        db  087h, 005h, 014h, 002h, 000h, 000h
        mov    byte ptr [esi + eax], 0
        ret
loc_1_0085:
        in     al, 61h
        or     al, 3
        jmp    SHORT loc_1_008B
loc_1_008B:
        out    61h, al
        db  089h, 01Dh, 014h, 002h, 000h, 000h
        mov    byte ptr [esi + ebx], 1
        ret
loc_1_0098:
        db  080h, 03Dh, 01Ch, 002h, 000h, 000h, 000h
        jne    SHORT loc_1_0072
        db  083h, 03Dh, 014h, 002h, 000h, 000h, 000h
        jne    SHORT loc_1_00CB
        db  089h, 015h, 014h, 002h, 000h, 000h
        mov    byte ptr [esi + edx], 1
loc_1_00B4:
        db  0BEh, 0F5h, 000h, 000h, 000h
        mov    eax, 9c4h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 018h, 002h, 000h, 000h
        ret
loc_1_00CB:
        db  039h, 01Dh, 014h, 002h, 000h, 000h
        jne    SHORT loc_1_00D9
        call   sub_1_00E4
        ret
loc_1_00D9:
        db  039h, 015h, 014h, 002h, 000h, 000h
        je     SHORT loc_1_00B4
        ret
        db  08Bh, 0C0h
sub_1_00E4:
        push   esi
        xor    esi, esi
        db  087h, 035h, 018h, 002h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret
loc_1_00F5:
        xor    ebx, ebx
        db  089h, 01Dh, 018h, 002h, 000h, 000h
        in     al, 61h
        and    al, 0fch
        jmp    SHORT loc_1_0103
loc_1_0103:
        out    61h, al
        db  087h, 01Dh, 014h, 002h, 000h, 000h
        db  003h, 01Dh, 010h, 002h, 000h, 000h
        mov    byte ptr [ebx], 2
        ret
VSD_Get_Version:
        mov    eax, 300h
        clc
        ret
VSD_Bell:
        push   eax
        push   esi
        mov    ax, 538h
        out    42h, al
        mov    al, ah
        jmp    SHORT loc_1_0128
loc_1_0128:
        out    42h, al
        jmp    SHORT loc_1_012C
loc_1_012C:
        in     al, 61h
        or     al, 3
        jmp    SHORT loc_1_0132
loc_1_0132:
        out    61h, al
        mov    eax, 1f4h
        db  0BEh, 04Eh, 001h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  0C6h, 005h, 01Ch, 002h, 000h, 000h, 0FFh
        pop    esi
        pop    eax
        ret
loc_1_014E:
        db  0C6h, 005h, 01Ch, 002h, 000h, 000h, 000h
        in     al, 61h
        and    al, 0fch
        jmp    SHORT loc_1_015B
loc_1_015B:
        out    61h, al
        ret
loc_1_015E:
        db  0C6h, 005h, 01Dh, 002h, 000h, 000h, 004h
        db  0CDh, 020h, 097h, 080h, 001h, 000h
VSD_SoundOn:
        ret
loc_1_016C:
        cmp    ecx, 4
        jbe    SHORT loc_1_0177
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0177:
        db  074h, 00Ah
        in     al, dx
        db  0FEh, 00Dh, 01Dh, 002h, 000h, 000h
        jns    SHORT loc_1_01A8
        cmp    al, 0eeh
        push   eax
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        and    al, 3
        cmp    al, 3
        jne    SHORT loc_1_0197
        db  0CDh, 020h, 002h, 000h, 00Bh, 000h
loc_1_0197:
        mov    eax, 3e8h
        db  0BEh, 05Eh, 001h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        pop    eax
loc_1_01A8:
        ret
VSD_TakeSoundPort:
        db  0A2h, 01Ch, 002h, 000h, 000h
        clc
        ret
        db  015h, 001h, 000h, 000h, 01Ch, 001h, 000h, 000h, 06Bh, 001h, 000h, 000h, 0A9h, 001h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 00Bh, 000h, 002h, 000h, 000h, 000h, 056h, 053h, 044h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 02Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0B0h, 001h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_021D:
        add    al, 0
        db  000h
_LTEXT ends
        end
