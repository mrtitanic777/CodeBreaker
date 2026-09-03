; cdtsd_lcode_01.asm -- CDTSD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

CDTSD_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
        db  000h
loc_1_000C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 3
        db  00Fh, 084h, 0DDh, 0FFh, 0FEh, 0FFh
        cmp    si, 4
        db  00Fh, 084h, 0CFh, 000h, 0FFh, 0FFh
        cmp    si, 2
        db  00Fh, 084h, 0D5h, 000h, 0FFh, 0FFh
        call   sub_1_003D
        ret
sub_1_003D:
        mov    word ptr [ebx + 2], 0ffffh
        ret
sub_1_0044:
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
loc_1_0060:
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
loc_1_0078:
        mov    edi, dword ptr [esp + 4]
        cmp    word ptr [edi + 68h], 0
        ja     SHORT loc_1_0093
loc_1_0083:
        mov    eax, dword ptr [edi + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [edi + 10h], eax
        push   edi
        call   dword ptr [eax]
        add    esp, 4
        ret
loc_1_0093:
        jmp    SHORT loc_1_0083
        db  000h, 000h, 000h, 055h, 083h, 0ECh, 00Ch, 08Bh, 0ECh, 066h, 0C7h, 045h, 000h, 007h, 000h, 089h
        db  045h, 008h, 055h, 0FFh, 015h, 044h, 000h, 000h, 000h, 08Bh, 075h, 004h, 00Fh, 0B7h, 045h, 002h
        db  083h, 0C4h, 010h, 05Dh, 0C3h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 043h, 044h, 054h, 053h
        db  044h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 028h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
