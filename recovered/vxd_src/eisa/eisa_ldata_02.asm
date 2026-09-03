; eisa_ldata_02.asm -- EISA object 2 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 045h, 049h, 053h, 041h
        db  05Ch, 02Ah, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 05Ch, 03Fh, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 045h, 049h, 053h, 041h, 020h, 063h, 061h, 072h, 064h, 020h, 069h, 06Eh
        db  020h, 073h, 06Ch, 06Fh, 074h, 020h, 023h, 025h, 031h, 064h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 000h, 000h, 000h, 04Dh, 000h, 000h, 000h
        db  055h, 000h, 000h, 000h, 007h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        db  003h, 000h, 000h, 000h, 009h, 000h, 000h, 000h, 007h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  014h, 000h, 000h, 000h
sub_2_0074:
        in     al, 2
        add    byte ptr [eax], al
        pop    ebp
        add    eax, dword ptr [eax]
        db  000h, 0CEh
        add    al, 0
        db  000h, 0D7h
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_008D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0091:
        dec    ebx
        inc    ebp
        push   edx
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 49h], al
        push   ebx
        inc    ecx
        push   ebx
        imul   ebp, dword ptr [ebp + 75h], 6574616ch
        dec    ecx
        outsb
        je     SHORT loc_2_00DD
        xor    eax, 56000000h
        push   ebx
        je     SHORT loc_2_0120
        je     SHORT loc_2_011E
        inc    esp
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 52h], dl
        outsd
        insd
        inc    esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 76h], al
        imul   esp, dword ptr [ebx + 65h], 63736544h
        add    byte ptr [eax], al
loc_2_00D4:
        and    eax, 252e6433h
        xor    esp, dword ptr [eax + eax + 48h]
loc_2_00DD:
        push   edi
        push   edx
        db  065h, 076h, 069h
        db  073h, 069h
        outsd
        outsb
        add    byte ptr [eax], al
loc_2_00E8:
        and    eax, 5c005832h
        and    eax, 5831h
        add    byte ptr [eax], al
loc_2_00F4:
        inc    ebp
        dec    ecx
        push   ebx
        inc    ecx
        push   edx
        popad
        outsb
        jae    NEAR PTR loc_2_00FF
loc_2_00FF:
        add    byte ptr [ebp + 49h], al
        push   ebx
        inc    ecx
        inc    esi
        insb
        popad
        jae    NEAR PTR loc_2_010A
loc_2_010A:
        add    byte ptr [eax], al
loc_2_010C:
        inc    ebp
        dec    ecx
        push   ebx
        inc    ecx
        inc    esi
        db  075h, 06Eh
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        jae    SHORT loc_2_011A
loc_2_011A:
        add    byte ptr [eax], al
loc_2_011C:
        inc    ebp
        dec    ecx
loc_2_011E:
        push   ebx
        inc    ecx
loc_2_0120:
        inc    esi
        db  075h, 06Eh
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        db  073h, 04Dh
        popad
        db  073h, 06Bh
        add    byte ptr [eax], al
        add    byte ptr [ebp + 49h], al
        push   ebx
        inc    ecx
        pop    esp
        sub    dl, byte ptr [eax + 4eh]
        push   eax
        xor    byte ptr [ecx + 30h], al
        xor    byte ptr [eax + esi], bl
_LDATA ends
        end
