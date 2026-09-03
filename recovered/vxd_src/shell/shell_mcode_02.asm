; shell_mcode_02.asm -- SHELL object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  014h, 000h, 000h, 000h
loc_2_0004:
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        adc    byte ptr [eax], al
loc_2_0010:
        pop    ds
        add    byte ptr [eax], dl
        xchg   byte ptr [eax], al
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [eax]
        mov    byte ptr [eax], 0c8h
        add    dl, cl
        add    ah, cl
        add    byte ptr [esi], al
        add    dword ptr [eax], ecx
        add    dword ptr [edx], ecx
        add    dword ptr [ecx], edx
        add    dword ptr [ebx + 1], eax
        inc    ebp
        add    dword ptr [edi + 1], eax
        db  07Ch, 001h
        db  07Eh, 001h
        test   dword ptr [ecx], eax
        add    byte ptr [eax], al
loc_2_0038:
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
        inc    ebx
        outsd
        outsb
        insb
        imul   esp, dword ptr [ebx + 74h], 73795300h
        db  074h, 065h
        insd
        and    byte ptr [eax + 61h], ch
        insb
        db  074h, 065h
        add    byte ptr cs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 72h], dl
        db  065h, 073h, 073h
        and    byte ptr [ecx + 6eh], ah
        db  079h, 020h
        imul   esp, dword ptr [ebp + 79h], 20h
        je     SHORT loc_2_00EC
        and    byte ptr [ebx + 6fh], ah
        outsb
        je     SHORT loc_2_00EC
        outsb
        db  075h, 065h
        and    byte ptr [eax], al
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   eax
        jb     SHORT loc_2_0100
        jae    SHORT loc_2_0110
        and    byte ptr [ebp + 4eh], al
        push   esp
        inc    ebp
        push   edx
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        dec    edi
        dec    ebx
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ebp + 53h], al
        inc    ebx
        and    byte ptr [edi + ebp*2 + 20h], dh
        inc    ebx
        popad
        outsb
        arpl   word ptr [ebp + 6ch], sp
        cmp    ah, byte ptr [eax]
        add    byte ptr [edi + 4bh], cl
        add    byte ptr [eax], bl
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   eax
        jb     SHORT loc_2_0137
        db  073h, 073h
        and    byte ptr [ecx + 20h], al
        je     SHORT loc_2_0148
        and    byte ptr [ecx + 62h], al
        outsd
        db  072h, 074h
        sub    al, 20h
        push   edx
        and    byte ptr [edi + ebp*2 + 20h], dh
        push   edx
        je     NEAR PTR loc_2_015C
        jns    SHORT loc_2_0118
loc_2_00EC:
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ecx + 20h], cl
        db  074h, 06Fh
        and    byte ptr [ecx + 67h], cl
        outsb
        outsd
        db  072h, 065h
        cmp    ah, byte ptr [eax]
        add    byte ptr [ecx], al
loc_2_0100:
        push   edx
        add    byte ptr [ecx], cl
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0110:
        push   eax
        jb     SHORT loc_2_0178
        jae    SHORT loc_2_0188
        and    byte ptr [ecx + 20h], bl
loc_2_0118:
        outsw
        db  072h, 020h
        pop    ecx
        jae    NEAR PTR loc_2_014C
        and    byte ptr [esi + 20h], cl
        outsw
        db  072h, 020h
        dec    esi
        outsd
        sub    al, 20h
        outsd
        jb     SHORT loc_2_014E
        inc    ebp
        push   ebx
        inc    ebx
        and    byte ptr [edi + ebp*2 + 20h], dh
        inc    ebx
        popad
loc_2_0137:
        outsb
        arpl   word ptr [ebp + 6ch], sp
        cmp    ah, byte ptr [eax]
        add    byte ptr [ecx], bl
        dec    esi
        add    byte ptr [ebx + 61h], al
        outsb
        arpl   word ptr [ebp + 6ch], sp
loc_2_0148:
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_014C:
        and    byte ptr [eax], ah
loc_2_014E:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_015C:
        and    byte ptr [eax + 72h], dl
        db  065h, 073h, 073h
        and    byte ptr [ecx + 20h], bl
        outsw
        db  072h, 020h
        pop    ecx
        db  065h, 073h, 020h
        outsd
        jb     SHORT loc_2_0190
        dec    esi
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        dec    esi
        outsd
loc_2_0178:
        cmp    ah, byte ptr [eax]
        add    byte ptr [ecx], bl
        dec    esi
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0188:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0190:
        push   eax
        db  072h, 065h
        db  073h, 073h
        and    byte ptr [edx + 20h], dl
        db  074h, 06Fh
        and    byte ptr [edx + 65h], dl
        db  074h, 072h
        db  079h, 020h
        outsd
        db  072h, 020h
        inc    ebp
        push   ebx
        inc    ebx
        and    byte ptr [edi + ebp*2 + 20h], dh
        inc    ebx
        popad
        outsb
        arpl   word ptr [ebp + 6ch], sp
        cmp    ah, byte ptr [eax]
        add    byte ptr [edx], dl
        inc    ebx
        popad
        outsb
        arpl   word ptr [ebp + 6ch], sp
        db  000h
_LMSGTABLE ends
        end
