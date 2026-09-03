; apix_pcode_04.asm -- APIX object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        inc    ecx
        push   ebx
        push   eax
        dec    ecx
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        push   edi
        db  069h, 06Eh, 064h, 06Fh, 077h, 073h
loc_4_0010:
        inc    ecx
        push   ebx
        push   eax
        dec    ecx
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        push   edi
        dec    ecx
        dec    esi
        xor    esi, dword ptr [edx]
        and    byte ptr [eax], ah
loc_4_0020:
        cmp    cl, byte ptr [ebx]
        inc    eax
        or     al, 40h
        sbb    byte ptr [eax], bl
        db  000h
loc_4_0028:
        cmp    al, 0ch
        inc    eax
        or     al, 40h
        sbb    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    dword ptr [edx], eax
        add    dword ptr [esp + eax], eax
        add    byte ptr [ecx + 4110482h], 0
        add    al, 4
        add    al, 4
        add    al, 12h
        adc    edx, dword ptr [eax + eax*4]
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  080h, 080h, 080h, 004h
loc_4_0054:
        dec    ebp
        dec    ecx
        inc    ebx
        push   edx
        dec    edi
        push   ebx
        dec    edi
        inc    esi
        push   esp
        sub    al, 20h
        inc    ebx
        dec    edi
        push   edx
        push   eax
        db  000h
loc_4_0064:
        dec    edi
        inc    ebp
        dec    ebp
        and    byte ptr [ebx + 43h], dl
        push   ebx
        dec    ecx
        and    byte ptr [eax + 42h], cl
        inc    ecx
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
loc_4_0074:
        push   es
        or     cl, byte ptr [edx]
        or     al, 0ch
        or     al, 0ah
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_008B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0095:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_4_00DC:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
loc_4_00E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0100:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0104:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0108:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_010C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PTEXT ends
        end
