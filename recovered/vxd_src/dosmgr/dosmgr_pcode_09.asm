; dosmgr_pcode_09.asm -- DOSMGR object 9 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_9_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_9_0004:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jmp    SHORT loc_9_0010
        db  0FFh, 025h, 000h, 000h, 000h, 000h
loc_9_0010:
        cmp    al, 21h
        jne    SHORT loc_9_0066
        pushad
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        test   byte ptr [ebx], 20h
        jne    SHORT loc_9_0065
        db  03Bh, 03Dh, 004h, 000h, 000h, 000h
        jne    SHORT loc_9_0065
        mov    ebp, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp + 8]
        mov    esi, eax
        sub    eax, esp
        cmp    eax, 1000h
        jae    SHORT loc_9_0065
        push   80000000h
        push   10h
        db  068h, 065h, 000h, 000h, 000h
        push   14h
        push   esi
        db  0CDh, 020h, 073h, 001h, 001h, 000h
        lea    esp, [esp + 14h]
        jb     SHORT loc_9_0065
        mov    eax, dword ptr [ebp + 1ch]
        cmp    eax, dword ptr [esi]
        db  00Fh, 084h, 040h, 013h, 0F7h, 0FFh
loc_9_0065:
        popad
loc_9_0066:
        db  0FFh, 025h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
