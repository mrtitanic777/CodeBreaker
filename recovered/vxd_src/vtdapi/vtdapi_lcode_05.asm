; vtdapi_lcode_05.asm -- VTDAPI object 5 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_5_0000:
        sbb    al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        db  000h, 069h, 000h
        add    byte ptr [eax], al
        xchg   edx, eax
        add    byte ptr [eax], al
        db  000h, 0A4h, 000h, 000h, 000h, 0B6h, 000h
        add    byte ptr [eax], al
        stosb
        add    al, byte ptr [eax]
        db  000h, 0D0h
        add    byte ptr [eax], al
        db  000h, 0BCh, 002h, 000h, 000h, 0D2h, 002h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0F7h, 002h, 000h, 000h, 012h, 003h
        add    byte ptr [eax], al
        cmp    byte ptr [ebx], al
        add    byte ptr [eax], al
        db  069h, 003h, 000h, 000h, 083h, 003h
        add    byte ptr [eax], al
        mov    eax, dword ptr [ebx]
        add    byte ptr [eax], al
        xchg   ebx, eax
        add    eax, dword ptr [eax]
        db  000h, 0A2h, 003h, 000h, 000h, 0AEh
        add    esp, dword ptr [ebx]
        push   esi
        push   esp
        inc    esp
        inc    ecx
        push   eax
        dec    ecx
        and    byte ptr [esi + 69h], dl
        db  072h, 074h
        jne    SHORT loc_5_00BD
        insb
        and    byte ptr [ebp + 76h], al
        imul   esp, dword ptr [ebx + 65h], 65562809h
        jb     SHORT loc_5_00DD
        imul   ebp, dword ptr [edi + 6eh], 302e3420h
        sub    dword ptr [eax], eax
        add    byte ptr [edx], cl
        push   esi
        push   esp
        inc    esp
        inc    ecx
        push   eax
        dec    ecx
        pop    edi
        inc    esp
        inc    esp
        inc    edx
        add    dword ptr [eax], eax
        add    bh, bh
        adc    byte ptr [eax], al
        inc    dword ptr [ecx]
        add    byte ptr [eax], dh
        add    ah, ah
        add    dword ptr [eax], eax
        add    ah, ah
        add    dword ptr [eax + eax], esi
        push   esi
        push   ebx
        pop    edi
        push   esi
        inc    ebp
        push   edx
        push   ebx
        dec    ecx
        dec    edi
        dec    esi
        pop    edi
        dec    ecx
        dec    esi
        inc    esi
        dec    edi
        add    byte ptr [ebp + 0feef04h], bh
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        mov    dh, 3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        mov    dh, 3
        add    byte ptr [eax], al
        aas
        add    byte ptr [eax], al
loc_5_00BD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [0], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 1], dh
        add    byte ptr [eax], al
        push   ebx
        je     SHORT loc_5_014F
loc_5_00DD:
        imul   ebp, dword ptr [esi + 67h], 656c6946h
        dec    ecx
        outsb
        outsw
        add    byte ptr [eax], al
        popad
        add    dword ptr [eax], eax
        add    byte ptr [eax], dh
        xor    al, 30h
        cmp    dword ptr [eax], esi
        xor    al, 45h
        xor    al, 0
        add    byte ptr [eax], al
        add    byte ptr [esi], ah
        add    byte ptr [esi], dl
        add    byte ptr [ebx + 6fh], al
        insd
        jo     SHORT loc_5_0164
        outsb
        db  079h, 04Eh
        popad
        insd
        add    byte ptr gs:[ebp + 69h], cl
        arpl   word ptr [edx + 6fh], si
        jae    SHORT loc_5_0180
        db  066h, 074h, 020h
        inc    ebx
        outsd
        jb     SHORT loc_5_0188
        outsd
        db  072h, 061h
        je     SHORT loc_5_0186
        outsd
        outsb
        add    byte ptr [eax], al
        add    byte ptr [ecx], bh
        add    byte ptr [6c694600h], ah
        inc    esp
        jae    NEAR PTR loc_5_0191
        db  072h, 069h
        db  070h, 074h
        imul   ebp, dword ptr [edi + 6eh], 44545600h
        inc    ecx
        push   eax
        dec    ecx
        and    byte ptr [esi + 69h], dl
        db  072h, 074h
        db  075h, 061h
        insb
        and    byte ptr [ebp + 76h], al
        imul   esp, dword ptr [ebx + 65h], 56282020h
loc_5_014F:
        db  065h, 072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 302e3420h
        sub    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [ecx], cl
        add    byte ptr [esi + 69h], al
loc_5_0164:
        insb
        push   esi
        db  065h, 072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 302e3400h
        xor    byte ptr [esi], ch
        cmp    dword ptr [30h], esi
        add    byte ptr [ebx], bl
        add    byte ptr [edi], al
        add    byte ptr [ecx + 6eh], cl
loc_5_0180:
        je     SHORT loc_5_01E7
        jb     SHORT loc_5_01F2
        popad
        insb
loc_5_0186:
        dec    esi
        popad
loc_5_0188:
        insd
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_5_018E:
        push   esi
        push   esp
        inc    esp
loc_5_0191:
        inc    ecx
        push   eax
        dec    ecx
        add    byte ptr [eax], al
        cmp    al, byte ptr [eax]
        add    byte ptr es:[ebp + 67h], cl
        popad
loc_5_019E:
        insb
        inc    ebx
        outsd
        jo     SHORT loc_5_021C
        db  072h, 069h
        push   43000074h
        outsd
        db  070h, 079h
        jb     SHORT loc_5_0219
        push   20a92074h
        dec    ebp
        imul   esp, dword ptr [ebx + 72h], 666f736fh
loc_5_01BE:
        db  074h, 020h
        inc    ebx
        outsd
        db  072h, 070h
        and    byte ptr cs:[ecx], dh
        cmp    dword ptr [eax], edi
loc_5_01C9:
        cmp    byte ptr [35393931h], ch
        add    byte ptr [eax], al
        add    byte ptr [ebx], ah
        add    byte ptr [ebx], cl
        add    byte ptr [edi + 72h], cl
        imul   esp, dword ptr [edi + 69h], 466c616eh
        imul   ebp, dword ptr [ebp + 6eh], 656d61h
loc_5_01E7:
        add    byte ptr [eax], al
        add    byte ptr [esi + 54h], dl
        inc    esp
        inc    ecx
        push   eax
        dec    ecx
        push   esi
loc_5_01F2:
        pop    eax
        inc    esp
        add    byte ptr [eax], al
        xor    eax, 50002500h
        jb     SHORT loc_5_026C
        jne    NEAR PTR loc_5_0263
        je     SHORT loc_5_0250
        popad
        insd
        add    byte ptr gs:[ebp + 69h], cl
        arpl   word ptr [edx + 6fh], si
        db  073h, 06Fh
loc_5_020D:
        je     NEAR PTR loc_5_01BE
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        scasb
loc_5_0219:
        and    byte ptr [edi + 70h], cl
loc_5_021C:
        db  065h, 072h, 061h
loc_5_021F:
        db  074h, 069h
        outsb
        and    byte ptr [bp + di + 79h], dl
        db  073h, 074h
        insd
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    eax, 50000900h
        db  072h, 06Fh
        db  064h, 075h, 063h
        db  074h, 056h
        db  065h, 072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 2e340000h
        xor    byte ptr [eax], dh
        cmp    dword ptr cs:[30h], esi
loc_5_024D:
        add    byte ptr [eax + eax], ah
loc_5_0250:
        add    byte ptr [eax], al
        push   esi
        popad
        db  072h, 046h
        imul   ebp, dword ptr [ebp + 49h], 6f666eh
        adc    al, 0
        add    al, 0
        push   esp
loc_5_0263:
        db  072h, 061h
        outsb
        db  073h, 06Ch
        popad
        db  074h, 069h
        outsd
loc_5_026C:
        outsb
        add    byte ptr [ecx], cl
_LTEXT ends
        end
