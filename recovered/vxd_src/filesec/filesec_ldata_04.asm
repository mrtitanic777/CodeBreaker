; filesec_ldata_04.asm -- FILESEC object 4 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

sub_4_0000:
        mov    al, 0
        add    byte ptr [eax], al
        jo     9
        add    byte ptr [eax], al
        adc    byte ptr [eax + eax], al
        db  000h, 030h
        db  005h, 000h, 000h, 050h, 008h
        add    byte ptr [eax], al
        dec    byte ptr [eax + eax]
        db  000h, 001h
        sldt   word ptr [eax]
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0080:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        ljmp   8:4b000000h
        add    byte ptr [esi + 53000008h], ch
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00A4:
        pop    esp
        inc    ecx
        inc    esp
        dec    ebp
        dec    ecx
        dec    esi
        pop    esp
        push   edx
        inc    ebp
        dec    ebp
        dec    edi
        push   esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebx + 45h], dl
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00C8:
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00DC:
        add    al, 0
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [edx], ch
        add    byte ptr [eax], al
        add    byte ptr [ebx + 45h], dl
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_010C:
        push   edi
        jae    SHORT loc_4_0163
        add    byte ptr [edx], bh
        add    byte ptr [eax], al
loc_4_0114:
        jp     SHORT loc_4_016D
        push   edi
        jb     SHORT loc_4_0165
        push   72577a00h
        dec    esp
        push   7a000000h
        db  07Ah, 068h
        add    byte ptr [edx + 57h], bh
        jae    SHORT loc_4_0180
        push   eax
        add    byte ptr [eax], al
        db  000h, 02Dh, 000h, 000h, 000h, 026h
        db  01Dh, 000h, 000h, 00Ch, 001h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr cs:[eax], al
        db  000h, 073h, 01Dh
        add    byte ptr [eax], al
        adc    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Ah, 000h, 000h, 000h, 097h, 01Dh, 000h, 000h, 014h, 001h, 000h
loc_4_0163:
        add    byte ptr [eax], al
loc_4_0165:
        add    byte ptr [eax], al
        cmp    bh, 0ffh
        db  0FFh, 0FFh, 02Bh
loc_4_016D:
        add    byte ptr [eax], al
        db  000h, 0CAh
        db  01Dh, 000h, 000h, 01Ch, 001h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_4_0180:
        db  069h, 000h, 000h, 000h, 001h, 01Eh
        add    byte ptr [eax], al
        and    al, 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Ch, 000h, 000h, 000h, 07Ah, 01Eh, 000h, 000h, 028h, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 053h, 045h, 043h, 055h
        db  052h, 049h, 054h, 059h, 05Ch, 041h, 043h, 043h, 045h, 053h, 053h, 05Ch, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 053h, 045h, 043h, 055h, 052h, 049h, 054h, 059h
        db  05Ch, 041h, 043h, 043h, 045h, 053h, 053h, 05Ch, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
_LDATA ends
        end
