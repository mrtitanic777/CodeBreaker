; vmpoll_pcode_08.asm -- VMPOLL object 8 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  090h
loc_8_0001:
        db  0CDh, 020h, 040h, 000h, 001h, 000h
        mov    ecx, eax
        xchg   dword ptr [esi + 4], eax
        sub    ecx, eax
        cmp    ecx, dword ptr [esi + 0ch]
        jae    SHORT loc_8_003E
        test   dl, 40h
        je     SHORT loc_8_003E
        test   byte ptr [esi + 10h], 2
        jne    SHORT loc_8_003E
        db  081h, 03Eh, 001h, 000h, 000h, 000h
        jne    SHORT loc_8_0033
        db  0CDh, 020h, 005h, 000h, 00Ah, 000h
        mov    dword ptr [esi + 8], eax
        dec    dword ptr [esi]
        jmp    SHORT loc_8_0048
loc_8_0033:
        db  0CDh, 020h, 005h, 000h, 00Ah, 000h
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_8_004E
loc_8_003E:
        and    byte ptr [esi + 10h], 0fdh
        db  0C7h, 006h, 001h, 000h, 000h, 000h
loc_8_0048:
        db  0A1h, 070h, 001h, 000h, 000h
        ret
loc_8_004E:
        db  0C7h, 006h, 054h, 000h, 000h, 000h
loc_8_0054:
        test   dl, 40h
        je     SHORT loc_8_003E
        db  0CDh, 020h, 005h, 000h, 00Ah, 000h
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_8_003E
        test   byte ptr [esi + 10h], 4
        jne    SHORT loc_8_003E
        bts    dword ptr [esi + 10h], 3
        jb     SHORT loc_8_0087
        or     byte ptr [ebx + 1], 80h
        mov    ecx, 63h
        db  0BEh, 08Bh, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_8_0087:
        xor    eax, eax
        inc    eax
        ret
loc_8_008B:
        db  08Bh, 035h, 080h, 001h, 000h, 000h
        add    esi, ebx
        btr    dword ptr [esi + 10h], 3
        jae    SHORT loc_8_00AE
        test   byte ptr [ebx + 1], 80h
        je     SHORT loc_8_00AE
        or     byte ptr [esi + 10h], 1
        mov    eax, 7fffffffh
        db  0E8h, 09Bh, 0FFh, 0F7h, 0FFh
loc_8_00AE:
        ret
_PTEXT ends
        end
