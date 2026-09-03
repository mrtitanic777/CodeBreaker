; vnetbios_mcode_03.asm -- VNETBIOS object 3 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  002h, 000h, 000h, 000h
loc_3_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_3_000C:
        add    byte ptr [eax], al
loc_3_000E:
        sbb    eax, 8b00h
        add    byte ptr [esi + 69h], dl
        jb     SHORT loc_3_008C
        jne    SHORT loc_3_007B
        insb
        and    byte ptr [esi + 65h], cl
        je     SHORT loc_3_0097
        outsd
        db  072h, 06Bh
        and    byte ptr [ebp + 76h], al
        imul   esp, dword ptr [ebx + 65h], 72724520h
        outsd
        jb     SHORT loc_3_0031
loc_3_0031:
        push   esp
        push   69562065h
        db  072h, 074h
        jne    SHORT loc_3_009C
        insb
        and    byte ptr [esi + 65h], cl
        db  074h, 077h
        outsd
        db  072h, 06Bh
        and    byte ptr [ebp + 76h], al
        imul   esp, dword ptr [ebx + 65h], 73617720h
        and    byte ptr [ebp + 6eh], dh
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [ecx + 6eh], ch
        imul   esi, dword ptr [ecx + ebp*2 + 61h], 657a696ch
        and    byte ptr cs:[eax + 72h], dl
        outsd
        db  067h, 072h, 061h
        insd
        jae    SHORT loc_3_008F
        db  074h, 068h
        popad
        je     SHORT loc_3_0094
        db  075h, 073h
        and    byte ptr gs:[eax + ebp*2 + 65h], dh
loc_3_007B:
        and    byte ptr [esi + 65h], ch
        db  074h, 077h
        outsd
        db  072h, 06Bh
        and    byte ptr [ebp + 61h], ch
        db  079h, 020h
        outsb
        outsd
        db  074h, 020h
loc_3_008C:
        db  066h, 075h, 06Eh
loc_3_008F:
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
loc_3_0094:
        and    byte ptr [eax + 72h], dh
loc_3_0097:
        outsd
        db  070h, 065h
        db  072h, 06Ch
loc_3_009C:
        db  079h, 02Eh
        db  000h
_LMSGTABLE ends
        end
