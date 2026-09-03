; serwave_ldata_03.asm -- SERWAVE object 3 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

sub_3_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        db  000h, 0B0h, 00Bh, 000h, 000h, 0ADh
        adc    byte ptr [eax], al
        db  000h, 089h, 011h, 000h, 000h, 003h
        adc    eax, dword ptr [eax]
        db  000h, 0B0h, 002h, 000h, 000h, 02Ch
        add    al, 0
        db  000h, 0D5h
        db  005h, 000h, 000h, 0D6h, 009h
        add    byte ptr [eax], al
        enter  13h, 0
        salc
        add    byte ptr [eax], al
        db  000h, 01Fh
        or     eax, dword ptr [eax]
        db  000h, 0E2h
        adc    eax, dword ptr [eax]
        db  000h, 041h, 00Bh
        add    byte ptr [eax], al
        or     edx, dword ptr [eax + eax]
        db  000h, 012h
        push   ss
        add    byte ptr [eax], al
loc_3_0048:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_004C:
        push   esi
        outsd
        imul   esp, dword ptr [ebx + 65h], 646f4d20h
        insd
        and    byte ptr [edi + 61h], dl
        jbe    SHORT loc_3_00C1
        and    byte ptr [ebx], ah
        and    eax, 6432h
        add    byte ptr [edi + 61h], dl
        jbe    SHORT loc_3_00CD
        dec    esi
        popad
        insd
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_3_0070:
        pop    esp
        inc    esp
        jb     SHORT loc_3_00DD
        db  076h, 065h
        db  072h, 073h
        pop    esp
        ja     SHORT loc_3_00DC
        jbe    SHORT loc_3_00E2
        add    byte ptr [eax], al
        add    byte ptr [ebx + 79h], dl
        db  073h, 074h
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        arpl   word ptr [edi + 6eh], bp
        je     SHORT loc_3_0110
        outsd
        insb
        pop    esp
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 52h], 756f7365h
        jb     SHORT loc_3_0110
        db  065h, 073h, 05Ch
        ja     SHORT loc_3_0113
        db  076h, 065h
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bh
        add    byte ptr [eax], al
loc_3_00BC:
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], dh
loc_3_00C1:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 61h], cl
        db  070h, 070h
        popad
        bound  ebp, qword ptr [ebp]
loc_3_00CD:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 76h], al
        imul   esp, dword ptr [ebx + 65h], 63736544h
        add    byte ptr [eax], al
loc_3_00DC:
        dec    ebp
loc_3_00DD:
        dec    edi
        inc    esp
        inc    ebp
        dec    ebp
        push   edi
loc_3_00E2:
        inc    ecx
        push   esi
        inc    ebp
        pop    esp
        add    byte ptr [eax], al
loc_3_00E8:
        push   edi
        popad
        db  076h, 065h
        and    byte ptr [ebp + 76h], al
        imul   esp, dword ptr [ebx + 65h], 726f6620h
        and    byte ptr [esi + 6fh], dl
        imul   esp, dword ptr [ebx + 65h], 646f4d20h
        insd
        add    byte ptr [ebp + 76h], al
        imul   esp, dword ptr [ebx + 65h], 63736544h
        add    byte ptr [eax], al
loc_3_0110:
        dec    ebp
        inc    ebp
        inc    esp
loc_3_0113:
        dec    ecx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6ch], al
        popad
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [eax + 61h], cl
        db  072h, 064h
        db  077h, 061h
        db  072h, 065h
        dec    ecx
        inc    esp
        and    al, 53h
_LDATA ends
        end
