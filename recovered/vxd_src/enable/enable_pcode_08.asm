; enable_pcode_08.asm -- ENABLE object 8 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_8_0000:
        cmp    dword ptr [esp + 4], 54h
        jne    SHORT loc_8_0013
        pushad
        db  0E8h, 02Bh, 003h, 0F8h, 0FFh
        db  0E8h, 040h, 014h, 0F8h, 0FFh
        popad
loc_8_0013:
        or     eax, 0ffffffffh
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_8_0020:
        sub    esp, 8
        push   0
        lea    eax, [esp + 4]
        push   dword ptr [esp + 10h]
        push   eax
        call   sub_8_008C
        add    esp, 0ch
        cmp    eax, 5
        jne    SHORT loc_8_003F
        sub    eax, eax
        jmp    SHORT loc_8_0088
loc_8_003F:
        cmp    eax, 4
        jne    SHORT loc_8_0048
        sub    eax, eax
        jmp    SHORT loc_8_0088
loc_8_0048:
        cmp    eax, 3
        jne    SHORT loc_8_0051
        sub    eax, eax
        jmp    SHORT loc_8_0088
loc_8_0051:
        cmp    eax, 0dh
        mov    eax, 0
        je     SHORT loc_8_0088
        mov    dword ptr [esp + 4], 46h
        lea    eax, [esp + 4]
        push   0
        push   eax
        push   dword ptr [esp + 18h]
        push   1
        db  068h, 0C8h, 011h, 000h, 000h
        push   0
        push   dword ptr [esp + 18h]
        call   sub_8_0094
        add    esp, 1ch
        mov    eax, 1
loc_8_0088:
        add    esp, 8
        ret
sub_8_008C:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_8_0094:
        db  0CDh, 020h, 03Eh, 080h, 033h, 023h
_PTEXT ends
        end
