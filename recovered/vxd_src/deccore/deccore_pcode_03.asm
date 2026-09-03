; deccore_pcode_03.asm -- DECCORE object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        inc    esp
        inc    ebp
        inc    ebx
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        xor    edi, dword ptr cs:[eax]
        and    byte ptr ss:[eax], ah
        push   esi
        xor    dword ptr [esi], ebp
        xor    byte ptr [esi], ch
        xor    dh, byte ptr [0a0d2020h]
        and    al, 43h
        outsd
        db  070h, 079h
        jb     SHORT loc_3_0088
        push   63282074h
        sub    dword ptr [eax], esp
        inc    esp
        imul   esp, dword ptr [edi + 69h], 206c6174h
        inc    ebp
        jno    SHORT loc_3_00A7
        imul   esi, dword ptr [eax + 6dh], 20746e65h
        inc    ebx
        outsd
        db  072h, 070h
        outsd
        db  072h, 061h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr cs:[ecx], dh
        cmp    dword ptr [ecx], edi
        xor    al, 2ch
        and    byte ptr [ecx], dh
        cmp    dword ptr [ecx], edi
        xor    eax, 6c41202eh
        insb
        and    byte ptr [edx + 69h], dl
        push   52207374h
        jae    NEAR PTR loc_3_00C6
        db  072h, 076h
        and    byte ptr cs:[5750240ah], cl
        push   ebx
        imul   esp, dword ptr [edi + 6eh], 6e6fh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    eax, 4672030ah
        test   ebx, 40750003h
        mov    eax, 89660003h
loc_3_0088:
        push   cs
        je     SHORT loc_3_008B
loc_3_008B:
        xor    cx, cx
        xor    esi, esi
        mov    edi, 1eff006ah
        je     SHORT loc_3_0097
loc_3_0097:
        db  0E3h, 01Fh
        push   edx
        mov    ah, 0fh
        int    10h
        cmp    al, 3
        je     SHORT loc_3_00A7
        mov    eax, 10cd0003h
loc_3_00A7:
        mov    eax, 0ba0900h
        add    ch, cl
        and    dword ptr [eax + 19ba0900h], edi
        add    ch, cl
        and    dword ptr [edx + 33h], ebx
        db  0DBh, 033h, 0F6h, 066h, 033h, 0D2h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh, 033h
loc_3_00C6:
        mul    byte ptr [esi + 33h]
        db  0D2h, 0B8h, 001h, 080h, 0C3h
_PTEXT ends
        end
