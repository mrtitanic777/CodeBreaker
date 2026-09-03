; vmpoll_lcode_01.asm -- VMPOLL object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VMPOLL_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 0B2h, 001h, 0FFh, 0FFh
        cmp    eax, 7
        db  00Fh, 084h, 039h, 000h, 0FFh, 0FFh
        cmp    eax, 5
        db  00Fh, 084h, 0DCh, 0FFh, 0FEh, 0FFh
        db  0F7h, 005h, 02Ah, 000h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_1_0039
        cmp    eax, 14h
        db  00Fh, 084h, 0C7h, 0FFh, 0FEh, 0FFh
loc_1_0039:
        clc
        ret
        db  090h
VMPoll_Check_Idle:
        db  08Bh, 035h, 080h, 001h, 000h, 000h
        add    esi, ebx
        jmp    dword ptr [esi]
loc_1_0046:
        xor    eax, eax
        ret
sub_1_0049:
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        and    byte ptr [esi + 10h], 0feh
        test   byte ptr [esi + 10h], 10h
        je     SHORT loc_1_0065
        test   byte ptr [ebx + 1], 40h
        je     SHORT loc_1_0065
        db  0CDh, 020h, 061h, 001h, 001h, 000h
loc_1_0065:
        ret
        db  08Bh, 0C0h
VMPoll_Reset_Detection:
        call   sub_1_0074
        db  02Bh, 01Dh, 080h, 001h, 000h, 000h
        ret
sub_1_0074:
        db  003h, 01Dh, 080h, 001h, 000h, 000h
        db  081h, 03Bh, 046h, 000h, 000h, 000h
        je     SHORT loc_1_008C
        db  0C7h, 003h, 001h, 000h, 000h, 000h
        or     byte ptr [ebx + 10h], 2
loc_1_008C:
        stc
        ret
        db  08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h, 074h, 001h, 000h, 000h
loc_1_0098:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  003h, 01Dh, 080h, 001h, 000h, 000h
        test   byte ptr [ebx + 10h], 1
        jne    SHORT loc_1_00C1
        db  081h, 03Bh, 046h, 000h, 000h, 000h
        je     SHORT loc_1_00C1
        and    byte ptr [ebx + 10h], 0f7h
        db  0C7h, 003h, 001h, 000h, 000h, 000h
        or     byte ptr [ebx + 10h], 2
loc_1_00C1:
        pop    ebx
        db  0FFh, 025h, 074h, 001h, 000h, 000h
loc_1_00C8:
        sub    esp, 6ch
        mov    edi, esp
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 1607h
        mov    word ptr [ebp + 10h], 18h
        mov    word ptr [ebp + 18h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ecx, dword ptr [ebp + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
        cmp    cx, 1
        cmc
        ret
        db  000h, 000h, 000h, 000h, 007h, 000h, 000h, 000h, 068h, 000h, 000h, 000h, 03Ch, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 018h, 000h, 004h, 000h, 000h, 000h, 056h, 04Dh, 050h, 04Fh
        db  04Ch, 04Ch, 020h, 020h, 000h, 000h, 000h, 064h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  010h, 001h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
loc_1_0174:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0178:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_017C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0180:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0184:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0188:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
