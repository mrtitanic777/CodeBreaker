; vcd_mcode_02.asm -- VCD object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

loc_2_0000:
        add    al, 0
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        retf
        db  000h, 075h, 001h, 012h, 002h, 083h, 002h, 000h, 000h
loc_2_0018:
        push   esp
        push   65722065h
        db  071h, 075h
        db  065h, 073h, 074h
        and    byte ptr fs:[ebx + 4fh], al
        dec    ebp
        and    byte ptr [eax + 6fh], dh
        jb     SHORT loc_2_00A2
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [esi + 6fh], ch
        je     SHORT loc_2_0056
        popad
        db  076h, 061h
        imul   ebp, dword ptr [ecx + 62h], 202c656ch
        bound  esp, qword ptr [ebp + 63h]
        popad
        db  075h, 073h
        and    byte ptr gs:[ecx + 20h], ah
        push   77647261h
        popad
        db  072h, 065h
        and    byte ptr [ecx + 6eh], ch
loc_2_0056:
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        db  074h, 020h
        arpl   word ptr [edi + 6eh], bp
        insb
        imul   esp, dword ptr [ebx + 74h], 73616820h
        and    byte ptr [edx + 65h], ah
        outsb
        and    byte ptr [ebp + 74h], ah
        arpl   word ptr gs:[ebp + 64h], si
        and    byte ptr cs:[eax], ah
        pop    ecx
        outsd
        db  075h, 020h
        insd
        db  075h, 073h
        je     SHORT loc_2_00A4
        arpl   word ptr [eax + 61h], bp
        outsb
        and    byte ptr gs:[bx + di + 6fh], bh
        db  075h, 072h
        and    byte ptr [eax + 61h], ch
        jb     SHORT loc_2_00F8
        db  077h, 061h
        db  072h, 065h
        and    byte ptr [ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
loc_2_00A2:
        db  074h, 069h
loc_2_00A4:
        outsd
        outsb
        and    byte ptr [edi + ebp*2 + 20h], dh
        bound  esp, qword ptr [ebp + 20h]
        popad
        bound  ebp, qword ptr [ebp + 20h]
        je     SHORT loc_2_0123
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[eax + ebp*2 + 69h], dh
        db  073h, 020h
        inc    ebx
        dec    edi
        dec    ebp
        and    byte ptr [eax + 6fh], dh
        jb     SHORT loc_2_013A
        and    byte ptr [esi + 72h], ah
        outsd
        insd
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [ecx + 70h], ah
        db  070h, 06Ch
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        jae    SHORT loc_2_010F
        add    byte ptr [eax], al
        pop    ecx
        outsd
        jne    SHORT loc_2_0107
        push   20657661h
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
        and    byte ptr fs:[ecx + 20h], ah
loc_2_00F8:
        push   edi
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        xor    ebp, dword ptr [esi]
        xor    byte ptr [eax], ah
        arpl   word ptr [edi + 6dh], bp
loc_2_0107:
        db  070h, 061h
        db  074h, 069h
        bound  ebp, qword ptr [ebp + 20h]
loc_2_010F:
        arpl   word ptr [edi + 6dh], bp
        insd
        jne    SHORT loc_2_0183
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        jae    SHORT loc_2_013E
        jb     NEAR PTR loc_2_018A
        jbe    SHORT loc_2_0188
loc_2_0123:
        db  072h, 02Eh
        and    byte ptr [eax], ah
        push   esp
        outsd
        and    byte ptr [eax + 72h], dh
        outsd
        db  070h, 065h
        db  072h, 06Ch
        db  079h, 020h
        db  073h, 075h
        db  070h, 070h
        outsd
        db  072h, 074h
loc_2_013A:
        and    byte ptr [eax + ebp*2 + 69h], dh
loc_2_013E:
        db  073h, 020h
        db  064h, 072h, 069h
        db  076h, 065h
        db  072h, 020h
        jns    SHORT loc_2_01B8
        db  075h, 020h
        insd
        jne    SHORT loc_2_01C1
        db  074h, 020h
        db  073h, 065h
        db  074h, 020h
        and    al, byte ptr [ebx + 4fh]
        dec    ebp
        dec    ebp
        inc    esp
        push   edx
        push   esi
        xor    esi, dword ptr [eax]
        cmp    eax, 45555254h
        and    ah, byte ptr [eax]
        imul   ebp, dword ptr [esi + 20h], 20656874h
        pop    ebx
        xor    edi, dword ptr [eax]
        inc    ebp
        outsb
        push   6573205dh
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ebx + 59h], dl
        push   ebx
loc_2_0183:
        push   esp
        inc    ebp
        dec    ebp
        dec    ecx
loc_2_0188:
        dec    esi
        dec    ecx
loc_2_018A:
        add    byte ptr cs:[eax], al
        push   esp
        push   76207369h
        imul   esi, dword ptr [edx + 74h], 206c6175h
        jbe    NEAR PTR loc_2_0207
        arpl   word ptr [ebp + 20h], sp
        imul   esi, dword ptr [ebx + 20h], 69797274h
        outsb
        and    byte ptr [si + 6fh], dh
        and    byte ptr [edi + 65h], ah
        db  074h, 020h
        popad
        arpl   word ptr [ebx + 65h], sp
        db  073h, 073h
loc_2_01B8:
        and    byte ptr [edi + ebp*2 + 20h], dh
        and    eax, 68772073h
loc_2_01C1:
        imul   esp, dword ptr [ebx + 68h], 20736920h
        popad
        insb
        db  072h, 065h
        popad
        db  064h, 079h, 020h
        imul   ebp, dword ptr [esi + 20h], 20657375h
        bound  edi, qword ptr [ecx + 20h]
        popad
        outsb
        and    byte ptr [ecx + 70h], ah
        jo     SHORT loc_2_024D
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        or     eax, 6f440a0ah
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        ja     SHORT loc_2_0256
        outsb
        je     SHORT loc_2_0218
        db  074h, 068h
        and    byte ptr gs:[esi + 69h], dh
        db  072h, 074h
        db  075h, 061h
        insb
        and    byte ptr [ebp + 76h], ah
loc_2_0207:
        imul   esp, dword ptr [ebx + 65h], 206f7420h
        je     SHORT loc_2_0271
        imul   esp, dword ptr [ebp + 20h], 63h
        outsd
        outsb
        je     SHORT loc_2_028A
loc_2_0218:
        outsd
        insb
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        aas
        and    byte ptr [eax], al
        add    byte ptr [eax + ebp*2 + 69h], dl
        db  073h, 020h
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        daa
        jae    SHORT loc_2_025D
        popad
        db  074h, 074h
        insd
        db  070h, 074h
        and    byte ptr [edi + ebp*2 + 20h], dh
        je     NEAR PTR loc_2_026C
        popad
loc_2_024D:
        arpl   word ptr [ebx + 65h], sp
        db  073h, 073h
        and    byte ptr [edi + ebp*2 + 20h], dh
loc_2_0256:
        and    eax, 61682073h
        jae    SHORT loc_2_027D
loc_2_025D:
        bound  esp, qword ptr [ebp + 65h]
        outsb
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 6562202ch
loc_2_026C:
        arpl   word ptr [ecx + 75h], sp
        db  073h, 065h
loc_2_0271:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        and    byte ptr [ecx + 73h], ch
loc_2_027D:
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[edx + 79h], ah
        and    byte ptr [ecx + 20h], ah
loc_2_028A:
        db  076h, 069h
        db  072h, 074h
        db  075h, 061h
        insb
        and    byte ptr [ebp + 76h], ah
        db  069h, 063h, 065h, 02Eh, 000h, 000h
_LMSGTABLE ends
        end
