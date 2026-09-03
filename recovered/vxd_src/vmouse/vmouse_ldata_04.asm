; vmouse_ldata_04.asm -- VMOUSE object 4 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 020h, 008h, 010h, 004h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 00Eh, 000h, 00Ch, 000h, 000h, 000h, 000h, 001h, 000h, 077h, 0FFh, 077h
        db  000h, 000h, 000h, 000h, 050h, 000h, 010h, 000h, 000h, 000h, 010h, 000h, 010h, 000h, 000h, 000h
        db  000h, 000h, 001h, 000h, 0FFh, 03Fh, 0FFh, 01Fh, 0FFh, 00Fh, 0FFh, 007h, 0FFh, 003h, 0FFh, 001h
        db  0FFh, 000h, 07Fh, 000h, 03Fh, 000h, 01Fh, 000h, 0FFh, 001h, 0FFh, 010h, 0FFh, 030h, 07Fh, 0F8h
        db  07Fh, 0F8h, 07Fh, 0FCh, 000h, 000h, 000h, 040h, 000h, 060h, 000h, 070h, 000h, 078h, 000h, 07Ch
        db  000h, 07Eh, 000h, 07Fh, 080h, 07Fh, 000h, 07Ch, 000h, 06Ch, 000h, 046h, 000h, 006h, 000h, 003h
        db  000h, 003h, 000h, 000h, 032h, 032h, 020h, 010h, 0FFh, 003h, 042h, 075h, 073h, 006h, 053h, 065h
        db  072h, 069h, 061h, 06Ch, 006h, 049h, 06Eh, 070h, 06Fh, 072h, 074h, 003h, 050h, 053h, 032h, 000h
sub_4_00C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00C8:
        add    byte ptr [eax], al
loc_4_00CA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00D6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00DA:
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
loc_4_011A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_011E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0124:
        push   ss
        add    byte ptr [80000h], al
        or     byte ptr [eax], cl
        or     byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0D5h
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_015C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0160:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0164:
        clc
        add    eax, dword ptr [eax + eax]
        clc
        add    al, byte ptr [ebx]
        add    al, ch
        add    eax, dword ptr [eax + eax]
        db  0E8h, 002h, 003h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0179:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_017D:
        add    byte ptr [eax], al
loc_4_017F:
        dec    ebp
        outsd
        db  075h, 073h
        inc    esp
        db  072h, 069h
        db  076h, 065h
        jb     SHORT loc_4_018B
loc_4_018B:
        dec    ebp
        outsd
        db  075h, 073h
        push   esp
        db  079h, 070h
        add    byte ptr gs:[eax], al
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
        add    byte ptr [ebx + 6fh], ah
        insd
        imul   ebp, dword ptr [esi + 66h], 64006fh
        add    byte ptr [eax], al
loc_4_01C1:
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_4_01C5:
        add    byte ptr [eax], al
        add    bh, bh
loc_4_01C9:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_01CD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LDATA ends
        end
