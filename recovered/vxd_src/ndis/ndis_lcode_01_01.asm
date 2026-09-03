; ndis_lcode_01_01.asm -- NDIS object 1 segment _LTEXT  (part 1/3)
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0080:
        pop    esp
        popad
        imul   bp, word ptr [esp + esi*2 + 65h], 2e72h
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        popad
        imul   bp, word ptr [esp + esi*2 + 65h], 2e72h
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        dec    ecx
        dec    esi
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        dec    esp
loc_1_00B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    dword ptr [eax]
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    eax, 6000000h
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h
loc_1_0420:
        sub    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
loc_1_0428:
        add    dword ptr [eax], eax
        add    byte ptr [edx - 55fffffeh], ch
        add    eax, dword ptr [eax]
        add    byte ptr [edx - 55fffffch], ch
        add    eax, 6aa0000h
        add    byte ptr [eax], al
        stosb
        pop    es
        add    byte ptr [eax], al
        stosb
        or     byte ptr [eax], al
        add    byte ptr [edx - 55fffff7h], ch
        or     al, byte ptr [eax]
        add    byte ptr [edx - 55fffff5h], ch
        or     al, 0
        add    byte ptr [edx - 55fffff3h], ch
        push   cs
        add    byte ptr [eax], al
        stosb
        sldt   word ptr [eax]
        stosb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0468:
        sldt   word ptr [eax]
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
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_06B8:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_06BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_06C0:
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        dec    esp
        dec    edi
        inc    edi
        push   esp
        pop    eax
        push   esp
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 59h], dl
        push   ebx
        push   esp
        inc    ebp
        dec    ebp
        pop    esp
        inc    ebx
        db  075h, 072h
        jb     SHORT loc_1_0791
        outsb
        db  074h, 043h
        outsd
        outsb
        je     SHORT loc_1_07A5
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_1_0795
        push   ebx
        jb     NEAR PTR loc_1_07B3
        imul   esp, dword ptr [ebx + 65h], 6c435c73h
        popad
        jae    SHORT loc_1_07BA
        pop    esp
        dec    esi
        db  065h, 074h, 054h
        db  072h, 061h
        outsb
        jae    SHORT loc_1_0751
loc_1_0751:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 59h], dl
        push   ebx
        push   esp
        inc    ebp
        dec    ebp
        pop    esp
        inc    ebx
        jne    SHORT loc_1_07D0
        jb     SHORT loc_1_07C5
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 6c435c73h
        popad
        db  073h, 073h
        pop    esp
        dec    esi
        je     NEAR PTR loc_1_0780
loc_1_0780:
        push   ebx
        pop    ecx
        push   ebx
        push   esp
        inc    ebp
        dec    ebp
        pop    esp
        inc    ebx
        jne    SHORT loc_1_07FC
        db  072h, 065h
        outsb
        je     SHORT loc_1_07D2
        outsd
        outsb
loc_1_0791:
        je     SHORT loc_1_0805
        outsd
        insb
loc_1_0795:
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
loc_1_07A5:
        add    byte ptr [eax], al
        add    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07B0:
        dec    esi
        inc    esp
        dec    ecx
loc_1_07B3:
        push   ebx
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_07B8:
        dec    edi
        inc    esp
loc_1_07BA:
        dec    ecx
        add    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        pop    esp
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
loc_1_07C5:
        xor    al, byte ptr [eax]
        add    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        pop    esp
        dec    edi
        inc    esp
        dec    ecx
loc_1_07D0:
        add    byte ptr [eax], al
loc_1_07D2:
        add    byte ptr [eax], al
loc_1_07D4:
        inc    edx
        imul   ebp, dword ptr [esi + 64h], 73676e69h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07E0:
        dec    esp
        outsd
        inc    esp
        db  072h, 069h
        db  076h, 065h
        db  072h, 04Eh
        popad
        insd
        add    byte ptr gs:[eax], al
        add    byte ptr [ebp + 76h], al
        imul   esp, dword ptr [ebx + 65h], 73447856h
        add    byte ptr [eax], al
loc_1_07FC:
        push   ebx
        je     SHORT loc_1_0860
        je     SHORT loc_1_086A
        arpl   word ptr [esi + 78h], dx
        inc    esp
loc_1_0805:
        add    byte ptr [eax], al
        add    byte ptr [edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 04Eh
        popad
        insd
        add    byte ptr gs:[eax], al
loc_1_0814:
        dec    esi
        inc    ebp
        push   esp
        push   edi
        dec    edi
        push   edx
        dec    ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebp
        dec    esi
        push   ebp
        dec    ebp
        pop    esp
        dec    esi
        inc    ebp
        push   esp
        push   edi
        dec    edi
        push   edx
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0830:
        inc    ebx
        insb
        popad
        jae    SHORT loc_1_08A8
        add    byte ptr [eax], al
        add    byte ptr [edx + esi*2 + 69h], al
        jbe    SHORT loc_1_08A2
        jb     SHORT loc_1_083F
loc_1_083F:
        add    byte ptr [ebp + 4eh], al
        push   ebp
        dec    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0848:
        push   eax
        jb     SHORT loc_1_08BA
        imul   bp, word ptr [ebp + 46h], 616ch
        jae    NEAR PTR loc_1_0855
loc_1_0855:
        add    byte ptr [eax], al
        add    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        xor    dl, byte ptr [ebx + 55h]
        push   eax
loc_1_0860:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0864:
        dec    ebp
        push   ebx
        dec    edi
        inc    esp
        dec    ecx
        push   ebx
loc_1_086A:
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0870:
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 44h], sp
        db  065h, 073h, 063h
        add    byte ptr [eax], al
        inc    esp
        insb
        popad
        jns    SHORT loc_1_08C4
        imul   ebp, dword ptr [esi + 64h], 676e69h
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6ch], dl
        outsd
        db  074h, 04Eh
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
loc_1_0898:
        push   ebx
        insb
        outsd
        je     SHORT loc_1_089D
loc_1_089D:
        add    byte ptr [eax], al
        add    byte ptr [ecx + 64h], al
loc_1_08A2:
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_08F1
loc_1_08A8:
        add    byte ptr fs:[eax], al
        add    byte ptr [ebx + 6fh], al
        insd
        jo     SHORT loc_1_0912
        je     SHORT loc_1_091C
        bound  ebp, qword ptr [ebp + 49h]
        inc    esp
        jae    SHORT loc_1_08BA
loc_1_08BA:
        add    byte ptr [eax], al
loc_1_08BC:
        push   ebp
        jae    SHORT loc_1_0924
        dec    ecx
        push   edx
        push   ecx
        xor    al, byte ptr [eax]
loc_1_08C4:
        inc    esp
        imul   esi, dword ptr [ebx + 61h], 57656c62h
        popad
        db  072h, 06Eh
        imul   ebp, dword ptr [esi + 67h], 75420000h
        jae    SHORT loc_1_092C
        db  079h, 070h
        add    byte ptr gs:[ebp + 73h], dl
        inc    esp
        insd
        popad
        inc    edx
        jne    SHORT loc_1_094B
        jb     NEAR PTR loc_1_08E9
loc_1_08E9:
        add    byte ptr [eax], al
        add    byte ptr [esi], cl
        add    byte ptr [esi], cl
        db  000h, 000h
loc_1_08F1:
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], dl
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], dl
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0912:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0918:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_091C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0920:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0924:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0928:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_092C:
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
loc_1_0934:
        push   eax
        db  072h, 06Fh
        arpl   word ptr [ebp + 73h], sp
        db  073h, 06Fh
        jb     SHORT loc_1_0992
        db  079h, 070h
        add    byte ptr gs:[eax], al
        add    byte ptr [edi + ebp*2 + 73h], al
        dec    ebp
        popad
        push   6fh
loc_1_094B:
        jb     SHORT loc_1_09A3
        db  065h, 072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 736f4400h
        dec    ebp
        imul   ebp, dword ptr [esi + 6fh], 72655672h
        db  073h, 069h
        outsd
        outsb
        add    byte ptr [ebp + 6eh], al
        db  076h, 069h
        db  072h, 06Fh
        outsb
        insd
        outsb
        je     SHORT loc_1_0970
loc_1_0970:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_097C:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0988:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
loc_1_0992:
        add    byte ptr [eax], al
loc_1_0994:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_09A0:
        pop    esp
loc_1_09A3:
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_09AC:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_09B8:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_09C4:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_09D0:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_09DC:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_09E8:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_09F4:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A00:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A0C:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A18:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A24:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A30:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A3C:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A48:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A54:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A60:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A6C:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A78:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A84:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A90:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0A9C:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0AA8:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0AB4:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
loc_1_0AC0:
        pop    esp
        imul   ebp, dword ptr [esi + 69h], 632e74h
        add    byte ptr [eax], al
        dec    esi
        je     NEAR PTR loc_1_0B47
        outsd
        db  072h, 06Bh
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
loc_1_0ADC:
        dec    esi
        je     NEAR PTR loc_1_0B57
        outsd
        db  072h, 06Bh
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_0AF8:
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_0B04:
        pop    esp
        insd
        imul   ebp, dword ptr [esi + 69h], 74726f70h
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_0B14:
        pop    esp
        insd
        imul   ebp, dword ptr [esi + 69h], 74726f70h
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_0B24:
        pop    esp
        insd
        imul   ebp, dword ptr [esi + 69h], 74726f70h
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_0B34:
        pop    esp
        insd
        imul   ebp, dword ptr [esi + 69h], 74726f70h
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_0B44:
        pop    esp
loc_1_0B47:
        insd
        imul   ebp, dword ptr [esi + 69h], 74726f70h
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_0B54:
        pop    esp
loc_1_0B57:
        insd
        imul   ebp, dword ptr [esi + 69h], 74726f70h
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_0B64:
        pop    esp
        insd
        imul   ebp, dword ptr [esi + 69h], 2e6e6977h
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        insd
        imul   ebp, dword ptr [esi + 69h], 2e6e6977h
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        insd
        imul   ebp, dword ptr [esi + 69h], 2e6e6977h
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        insd
        imul   ebp, dword ptr [esi + 69h], 2e6e6977h
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [edx], al
        add    byte ptr [ebx], al
        add    byte ptr [eax + eax], al
        add    eax, 7000600h
        add    byte ptr [eax], cl
        add    byte ptr [ecx], cl
        add    byte ptr [edx], cl
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0f000e00h
        add    byte ptr [eax], dl
        add    byte ptr [ecx], dl
        add    byte ptr [edx], dl
        add    byte ptr [ebx], dl
        add    byte ptr [eax + eax], dl
        adc    eax, 17001600h
        add    byte ptr [eax], bl
        add    byte ptr [ecx], bl
        add    byte ptr [edx], bl
        add    byte ptr [ebx], bl
        add    byte ptr [eax + eax], bl
        sbb    eax, 1f001e00h
        add    byte ptr [eax], ah
        add    byte ptr [ecx], ah
        add    byte ptr [edx], ah
        add    byte ptr [ebx], ah
        add    byte ptr [eax + eax], ah
        and    eax, 27002600h
        add    byte ptr [eax], ch
        add    byte ptr [ecx], ch
        add    byte ptr [edx], ch
        add    byte ptr [ebx], ch
        add    byte ptr [eax + eax], ch
        sub    eax, 2f002e00h
        add    byte ptr [eax], dh
        add    byte ptr [ecx], dh
        add    byte ptr [edx], dh
        add    byte ptr [ebx], dh
        add    byte ptr [eax + eax], dh
        xor    eax, 37003600h
        add    byte ptr [eax], bh
        add    byte ptr [ecx], bh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        add    byte ptr [eax + eax], bh
        cmp    eax, 3f003e00h
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        add    byte ptr [eax + eax + 45h], al
        add    byte ptr [esi], al
        inc    edi
        add    byte ptr [eax], cl
        dec    ecx
        add    byte ptr [edx], cl
        dec    ebx
        add    byte ptr [eax + eax + 4dh], cl
        add    byte ptr [esi], cl
        dec    edi
        add    byte ptr [eax], dl
        push   ecx
        add    byte ptr [edx], dl
        push   ebx
        add    byte ptr [eax + eax + 55h], dl
        add    byte ptr [esi], dl
        push   edi
        add    byte ptr [eax], bl
        pop    ecx
        add    byte ptr [edx], bl
        pop    ebx
        add    byte ptr [eax + eax + 5dh], bl
        add    byte ptr [esi], bl
        pop    edi
        add    byte ptr [eax], ah
        popad
        add    byte ptr [edx], ah
        arpl   word ptr [eax], ax
        add    byte ptr fs:[ebp], ah
        add    byte ptr [edi], ah
        push   6a006900h
        add    byte ptr [ebx], ch
        insb
        add    byte ptr [ebp], ch
        outsb
        add    byte ptr [edi], ch
        jo     SHORT loc_1_0C86
loc_1_0C86:
        jno    SHORT loc_1_0C88
loc_1_0C88:
        jb     SHORT loc_1_0C8A
loc_1_0C8A:
        jae    SHORT loc_1_0C8C
loc_1_0C8C:
        je     SHORT loc_1_0C8E
loc_1_0C8E:
        jne    SHORT loc_1_0C90
loc_1_0C90:
        jbe    SHORT loc_1_0C92
loc_1_0C92:
        ja     SHORT loc_1_0C94
loc_1_0C94:
        js     SHORT loc_1_0C96
loc_1_0C96:
        jns    SHORT loc_1_0C98
loc_1_0C98:
        jp     SHORT loc_1_0C9A
loc_1_0C9A:
        jnp    SHORT loc_1_0C9C
loc_1_0C9C:
        jl     SHORT loc_1_0C9E
loc_1_0C9E:
        jge    SHORT loc_1_0CA0
loc_1_0CA0:
        jle    SHORT loc_1_0CA2
loc_1_0CA2:
        jg     SHORT loc_1_0CA4
loc_1_0CA4:
        add    byte ptr [eax], 81h
        add    byte ptr [edx - 7bff7d00h], al
        add    byte ptr [ebp - 78ff7a00h], al
        add    byte ptr [eax - 75ff7700h], cl
        add    byte ptr [ebx - 72ff7400h], cl
        add    byte ptr [esi - 6fff7100h], cl
        add    byte ptr [ecx - 6cff6e00h], dl
        add    byte ptr [eax + eax + 960095h], dl
        xchg   edi, eax
        add    byte ptr [eax - 65ff6700h], bl
        add    byte ptr [ebx - 62ff6400h], bl
        add    byte ptr [esi - 5fff6100h], bl
        add    byte ptr [ecx - 5cff5e00h], ah
        add    byte ptr [eax + eax + 0a600a5h], ah
        cmpsd
        add    byte ptr [eax - 55ff5700h], ch
        add    byte ptr [ebx - 52ff5400h], ch
        add    byte ptr [esi - 4fff5100h], ch
        add    byte ptr [ecx - 4cff4e00h], dh
        add    byte ptr [eax + eax + 0b600b5h], dh
        mov    bh, 0
        mov    eax, 0ba00b900h
        add    byte ptr [ebx - 42ff4400h], bh
        add    byte ptr [esi - 3fff4100h], bh
        add    cl, al
        add    dl, al
        add    bl, al
        add    ah, al
        add    ch, al
        add    dh, al
        add    bh, al
        add    al, cl
        add    cl, cl
        add    dl, cl
        add    bl, cl
        add    ah, cl
        add    ch, cl
        add    dh, cl
        add    bh, cl
        add    al, dl
        add    cl, dl
        add    dl, dl
        add    bl, dl
        add    ah, dl
        add    ch, dl
        add    dh, dl
        add    bh, dl
        add    al, bl
        add    cl, bl
        add    dl, bl
        add    bl, bl
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 41h], 69757163h
        db  072h, 065h
        push   ebx
        db  070h, 069h
        outsb
        dec    esp
        outsd
        arpl   word ptr [ebx], bp
loc_1_1570:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 636f6c6ch
        popad
        je     SHORT loc_1_15E1
        inc    edx
        db  075h, 066h
        jb     NEAR PTR loc_1_1583
loc_1_1583:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 41h], 636f6c6ch
        popad
        db  074h, 065h
        inc    edx
        db  075h, 066h
        db  066h, 065h, 072h, 050h
        outsd
        outsd
        insb
        add    byte ptr [eax], al
loc_1_159C:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 636f6c6ch
        popad
        db  074h, 065h
        inc    esp
        insd
        popad
        inc    ebx
        push   656e6e61h
        insb
        add    byte ptr [eax], al
loc_1_15B4:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 636f6c6ch
        popad
        je     SHORT loc_1_1625
        dec    ebp
        insd
        outsd
        db  072h, 079h
        add    byte ptr [eax], al
loc_1_15C8:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 636f6c6ch
        popad
        db  074h, 065h
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        db  074h, 050h
        outsd
        outsd
        insb
        add    byte ptr [eax], al
loc_1_15E0:
        dec    esi
loc_1_15E1:
        imul   esi, dword ptr fs:[ebx + 41h], 636f6c6ch
        popad
        je     SHORT loc_1_1651
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_15F3
loc_1_15F3:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 41h], 636f6c6ch
        popad
        je     SHORT loc_1_1665
        push   ebx
        push   64657261h
        dec    ebp
        insd
        outsd
        jb     SHORT loc_1_1685
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1610:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 41h], 636f6c6ch
        popad
        db  074h, 065h
        push   ebx
        jo     SHORT loc_1_1688
        outsb
        dec    esp
        outsd
        arpl   word ptr [ebx], bp
loc_1_1625:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 43h], 65636e61h
        insb
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 644e0072h
        imul   esi, dword ptr [ebx + 43h], 6e696168h
        inc    edx
        db  075h, 066h
        db  066h, 065h, 072h, 041h
        db  074h, 042h
        popad
        arpl   word ptr [ebx], bp
        add    byte ptr [eax], al
loc_1_1650:
        dec    esi
loc_1_1651:
        imul   esi, dword ptr fs:[ebx + 43h], 6e696168h
        inc    edx
        db  075h, 066h
        db  066h, 065h, 072h, 041h
        db  074h, 046h
        db  072h, 06Fh
        outsb
loc_1_1665:
        je     SHORT loc_1_1667
loc_1_1667:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 43h], 65736f6ch
        inc    ecx
        popad
        jo     SHORT loc_1_16EA
        jb     NEAR PTR loc_1_1679
loc_1_1679:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 43h], 65736f6ch
loc_1_1685:
        inc    ebx
        outsd
        outsb
loc_1_1688:
        imul   sp, word ptr [edi + 75h], 6172h
        je     SHORT loc_1_16F9
        outsd
        outsb
        add    byte ptr [eax], al
loc_1_1694:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 43h], 65736f6ch
        inc    esi
        imul   ebp, dword ptr [ebp], 644e0000h
        imul   esi, dword ptr [ebx + 43h], 6c706d6fh
        je     NEAR PTR loc_1_1715
        inc    edx
        imul   ebp, dword ptr [esi + 64h], 70616441h
        db  074h, 065h
        jb     SHORT loc_1_16BC
loc_1_16BC:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 43h], 6c706d6fh
        je     NEAR PTR loc_1_172D
        inc    ebx
        insb
        outsd
        db  073h, 065h
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_16D5
loc_1_16D5:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 43h], 6c706d6fh
        db  065h, 074h, 065h
        inc    esp
        insd
        popad
        push   esp
        jb     SHORT loc_1_174B
loc_1_16EA:
        outsb
        db  073h, 066h
        jb     NEAR PTR loc_1_16F0
loc_1_16F0:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 43h], 6c706d6fh
loc_1_16F9:
        je     NEAR PTR loc_1_1761
        dec    edi
        db  070h, 065h
        outsb
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_1708
loc_1_1708:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 43h], 6c706d6fh
        db  065h, 074h, 065h
        push   ecx
loc_1_1715:
        jne    SHORT loc_1_177C
        db  072h, 079h
        push   ebx
        je     SHORT loc_1_177D
        db  074h, 069h
        db  073h, 074h
        imul   esp, dword ptr [ebx + 73h], 69644e00h
        jae    SHORT loc_1_176C
        outsd
        insd
        jo     SHORT loc_1_1799
loc_1_172D:
        db  065h, 074h, 065h
        push   edx
        jae    NEAR PTR loc_1_1799
        je     SHORT loc_1_1736
loc_1_1736:
        add    byte ptr [eax], al
loc_1_1738:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 43h], 6c706d6fh
        db  065h, 074h, 065h
        push   edx
        jno    NEAR PTR loc_1_17BD
        db  065h, 073h, 074h
loc_1_174B:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 43h], 6c706d6fh
        je     NEAR PTR loc_1_17BD
        push   ebx
        outsb
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
loc_1_1760:
        dec    esi
loc_1_1761:
        imul   esi, dword ptr fs:[ebx + 43h], 6c706d6fh
        db  065h, 074h, 065h
loc_1_176C:
        push   esp
        jb     SHORT loc_1_17D0
        outsb
        jae    SHORT loc_1_17D8
        jb     NEAR PTR loc_1_17B9
        popad
        db  074h, 061h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_177C:
        dec    esi
loc_1_177D:
        imul   esi, dword ptr fs:[ebx + 43h], 6c706d6fh
        je     NEAR PTR loc_1_17ED
        push   ebp
        outsb
        bound  ebp, qword ptr [ecx + 6eh]
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_1796
loc_1_1796:
        add    byte ptr [eax], al
loc_1_1798:
        dec    esi
loc_1_1799:
        imul   esi, dword ptr fs:[ebx + 43h], 4279706fh
        jne    SHORT loc_1_1809
        jb     NEAR PTR loc_1_17A7
loc_1_17A7:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 43h], 4679706fh
        db  072h, 06Fh
        insd
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
loc_1_17B9:
        db  074h, 054h
        outsd
        push   eax
loc_1_17BD:
        popad
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_17C3
loc_1_17C3:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 44h], 67657265h
        inc    ecx
        popad
loc_1_17D0:
        db  070h, 074h
        push   ebx
        push   6f647475h
loc_1_17D8:
        db  077h, 06Eh
        add    byte ptr [eax], al
loc_1_17DC:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 44h], 67657265h
        imul   esi, dword ptr [ebx + 74h], 64417265h
        popad
loc_1_17ED:
        db  070h, 074h
        jb     NEAR PTR loc_1_17F2
loc_1_17F2:
        add    byte ptr [eax], al
loc_1_17F4:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 44h], 67657265h
        imul   esi, dword ptr [ebx + 74h], 614d7265h
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_1808:
        dec    esi
loc_1_1809:
        imul   esi, dword ptr fs:[ebx + 44h], 67657265h
        imul   esi, dword ptr [ebx + 74h], 72507265h
        outsd
        je     SHORT loc_1_188A
        arpl   word ptr [edi + 6ch], bp
        add    byte ptr [eax], al
loc_1_1820:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 45h], 6c617571h
        push   ebx
        db  074h, 072h
        imul   ebp, dword ptr [esi + 67h], 69644e00h
        db  073h, 046h
        jb     SHORT loc_1_189C
        inc    edx
        db  075h, 066h
        jb     NEAR PTR loc_1_183F
loc_1_183F:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 46h], 42656572h
        db  075h, 066h
        db  066h, 065h, 072h, 050h
        outsd
        outsd
        insb
        add    byte ptr [eax], al
loc_1_1854:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 46h], 44656572h
        insd
        popad
        inc    ebx
        push   656e6e61h
        insb
        add    byte ptr [eax], al
loc_1_1868:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 46h], 4d656572h
        insd
        outsd
        db  072h, 079h
        add    byte ptr [eax], al
loc_1_1878:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 46h], 50656572h
        popad
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_1887
loc_1_1887:
        add    byte ptr [esi + 64h], cl
loc_1_188A:
        imul   esi, dword ptr [ebx + 46h], 50656572h
        popad
        arpl   word ptr [ebx + 65h], bp
        db  074h, 050h
        outsd
        outsd
        insb
        add    byte ptr [eax], al
loc_1_189C:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 46h], 53656572h
        push   64657261h
        dec    ebp
        insd
        outsd
        db  072h, 079h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_18B4:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 46h], 53656572h
        jo     SHORT loc_1_1928
        outsb
        dec    esp
        outsd
        arpl   word ptr [ebx], bp
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 47h], 75427465h
        jb     NEAR PTR loc_1_1926
        push   63697379h
        popad
        insb
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_1957
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_18E8:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 47h], 654e7465h
        js     SHORT loc_1_1967
        inc    edx
        jne    SHORT loc_1_195C
        jb     NEAR PTR loc_1_18FA
loc_1_18FA:
        add    byte ptr [eax], al
loc_1_18FC:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 49h], 6369646eh
        popad
        db  074h, 065h
        push   edx
        arpl   word ptr gs:[ebp + 69h], sp
        jbe    SHORT loc_1_1974
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 49h], 6369646eh
        popad
        db  074h, 065h
        push   edx
        arpl   word ptr gs:[ebp + 69h], sp
        jbe    SHORT loc_1_1988
        inc    ebx
        outsd
        insd
loc_1_1926:
        db  070h, 06Ch
loc_1_1928:
        db  065h, 074h, 065h
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 49h], 6369646eh
        popad
        db  074h, 065h
        push   ebx
        db  074h, 061h
        db  074h, 075h
        jae    SHORT loc_1_193F
loc_1_193F:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 49h], 6369646eh
        popad
        je     SHORT loc_1_19B1
        push   ebx
        db  074h, 061h
        db  074h, 075h
        jae    SHORT loc_1_1996
        outsd
        insd
        jo     SHORT loc_1_19C3
loc_1_1957:
        db  065h, 074h, 065h
        add    byte ptr [eax], al
loc_1_195C:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 49h], 6974696eh
        popad
        insb
loc_1_1967:
        imul   edi, dword ptr [edx + 65h], 65746e49h
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_1974
loc_1_1974:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 49h], 6974696eh
        popad
        insb
        imul   edi, dword ptr [edx + 65h], 70617257h
        jo     SHORT loc_1_19ED
loc_1_1988:
        jb     SHORT loc_1_198A
loc_1_198A:
        add    byte ptr [eax], al
loc_1_198C:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 69467061h
        insb
loc_1_1996:
        add    byte ptr gs:[esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 6f497061h
        push   ebx
        db  070h, 061h
        arpl   word ptr [ebp], sp
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4fh], 416e6570h
loc_1_19B1:
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_19B8
loc_1_19B8:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4fh], 436e6570h
        outsd
        outsb
loc_1_19C3:
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
        outsb
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4fh], 466e6570h
        imul   ebp, dword ptr [ebp], 4e000000h
        imul   esi, dword ptr fs:[ebx + 4fh], 506e6570h
        db  072h, 06Fh
        db  074h, 06Fh
loc_1_19ED:
        arpl   word ptr [edi + 6ch], bp
        inc    ebx
        outsd
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
        outsb
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 51h], 79726575h
        inc    edx
        db  075h, 066h
        jb     NEAR PTR loc_1_1A10
loc_1_1A10:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 51h], 79726575h
        inc    edi
        insb
        outsd
        bound  esp, qword ptr [ecx + 6ch]
        push   ebx
        je     SHORT loc_1_1A83
        je     SHORT loc_1_1A8D
        db  073h, 074h
        imul   esp, dword ptr [ebx + 73h], 4e000000h
        imul   esi, dword ptr fs:[ebx + 51h], 79726575h
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_1A3C
loc_1_1A3C:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 52h], 43646165h
        outsd
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        je     SHORT loc_1_1AB8
        outsd
        outsb
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 52h], 44646165h
        insd
        popad
        inc    ebx
        outsd
        jne    SHORT loc_1_1AD1
        je     SHORT loc_1_1ACA
        jb     SHORT loc_1_1A67
loc_1_1A67:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 52h], 45646165h
        imul   esi, dword ptr [ebx + 61h], 746f6c53h
        dec    ecx
        outsb
        outsw
        db  072h, 06Dh
        popad
        db  074h, 069h
        outsd
        outsb
loc_1_1A83:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 52h], 4d646165h
loc_1_1A8D:
        arpl   word ptr [ecx + 50h], sp
        outsd
        db  073h, 049h
        outsb
        outsw
        jb     SHORT loc_1_1B05
        popad
        db  074h, 069h
        outsd
        outsb
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 52h], 4e646165h
        je     NEAR PTR loc_1_1B23
        outsd
        db  072h, 06Bh
        inc    ecx
        jb     NEAR PTR loc_1_1B19
        db  073h, 073h
        add    byte ptr [eax], al
loc_1_1AB8:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 52h], 64416765h
        popad
        db  070h, 074h
        push   ebx
        push   6f647475h
loc_1_1ACA:
        ja     SHORT loc_1_1B3A
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1AD0:
        dec    esi
loc_1_1AD1:
        imul   esi, dword ptr fs:[ebx + 52h], 73696765h
        db  074h, 065h
        db  072h, 041h
        popad
        jo     SHORT loc_1_1B55
        jb     NEAR PTR loc_1_1AE4
loc_1_1AE4:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 52h], 73696765h
        je     SHORT loc_1_1B54
        db  072h, 04Dh
        popad
        arpl   word ptr [eax], ax
loc_1_1AF4:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 52h], 73696765h
        je     SHORT loc_1_1B64
        jb     SHORT loc_1_1B51
        db  072h, 06Fh
        db  074h, 06Fh
loc_1_1B05:
        arpl   word ptr [edi + 6ch], bp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B0C:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 52h], 696e6965h
        db  074h, 069h
        popad
        insb
loc_1_1B19:
        imul   edi, dword ptr [edx + 65h], 6b636150h
        je     NEAR PTR loc_1_1B23
loc_1_1B23:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 52h], 61656c65h
        db  073h, 065h
        inc    ecx
        popad
        jo     SHORT loc_1_1BA8
        jb     NEAR PTR loc_1_1B89
        jae    NEAR PTR loc_1_1BA9
loc_1_1B3A:
        jne    SHORT loc_1_1BAE
        arpl   word ptr [ebp + 73h], sp
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 52h], 61656c65h
        db  073h, 065h
        push   ebx
        db  070h, 069h
        outsb
        dec    esp
        outsd
loc_1_1B51:
        arpl   word ptr [ebx], bp
loc_1_1B54:
        dec    esi
loc_1_1B55:
        imul   esi, dword ptr fs:[ebx + 52h], 766f6d65h
        dec    ecx
        outsb
        db  074h, 065h
        db  072h, 072h
loc_1_1B64:
        db  075h, 070h
        je     SHORT loc_1_1B68
loc_1_1B68:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 52h], 65757165h
        jae    SHORT loc_1_1BE7
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 52h], 74657365h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 53h], 646e65h
loc_1_1B89:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 53h], 69547465h
        insd
        jb     NEAR PTR loc_1_1B99
loc_1_1B99:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 53h], 70757465h
        inc    esp
        insd
        popad
loc_1_1BA8:
        push   esp
loc_1_1BA9:
        db  072h, 061h
        outsb
        jae    SHORT loc_1_1C14
loc_1_1BAE:
        jb     NEAR PTR loc_1_1BB1
loc_1_1BB1:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 53h], 6c6c6174h
        inc    ebp
        db  078h, 065h
        arpl   word ptr [ebp + 74h], si
        imul   ebp, dword ptr [edi + 6eh], 644e0000h
        imul   esi, dword ptr [ebx + 53h], 68636e79h
        jb     SHORT loc_1_1C42
        outsb
        imul   edi, dword ptr [edx + 65h], 68746957h
        dec    ecx
        outsb
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_1BE5
loc_1_1BE5:
        add    byte ptr [eax], al
loc_1_1BE7:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 54h], 696d7265h
        outsb
        popad
        je     SHORT loc_1_1C5A
        push   edi
        db  072h, 061h
        jo     SHORT loc_1_1C6A
        jb     NEAR PTR loc_1_1BFD
loc_1_1BFD:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 54h], 736e6172h
        jb     NEAR PTR loc_1_1C51
        popad
        db  074h, 061h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C14:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 55h], 6168636eh
        imul   ebp, dword ptr [esi + 42h], 65666675h
        jb     SHORT loc_1_1C67
        je     SHORT loc_1_1C6A
        popad
        arpl   word ptr [ebx], bp
loc_1_1C2C:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 55h], 6168636eh
        imul   ebp, dword ptr [esi + 42h], 65666675h
        jb     SHORT loc_1_1C7F
        db  074h, 046h
        jb     SHORT loc_1_1CB1
loc_1_1C42:
        outsb
        je     SHORT loc_1_1C45
loc_1_1C45:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 55h], 70616d6eh
loc_1_1C51:
        inc    esi
        imul   ebp, dword ptr [ebp], 644e0000h
loc_1_1C5A:
        imul   esi, dword ptr [ebx + 57h], 65746972h
        inc    ebp
        db  072h, 072h
        outsd
        jb     SHORT loc_1_1CB3
loc_1_1C67:
        outsd
        inc    ebp
loc_1_1C6A:
        outsb
        db  074h, 072h
        jns    SHORT loc_1_1C6F
loc_1_1C6F:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 6f6c6c41h
        arpl   word ptr [ecx + 74h], sp
        dec    ebp
        popad
loc_1_1C7F:
        db  070h, 052h
        imul   esi, dword ptr gs:[bp + di + 74h], 737265h
        add    byte ptr [eax], al
loc_1_1C8C:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 6f6c6c41h
        arpl   word ptr [ecx + 74h], sp
        push   ebx
        push   64657261h
        dec    ebp
        insd
        outsd
        db  072h, 079h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 636e6143h
loc_1_1CB1:
        insb
loc_1_1CB3:
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 72h
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 65726544h
        imul   esi, dword ptr [bp + di + 74h], 6d447265h
        popad
        inc    ebx
        push   656e6e61h
        insb
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 65726544h
        imul   esi, dword ptr [bp + di + 74h], 6e497265h
        je     SHORT loc_1_1D50
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_1CF1
loc_1_1CF1:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 65726544h
        imul   esi, dword ptr [bp + di + 74h], 6f497265h
        push   eax
        outsd
        db  072h, 074h
        push   edx
        popad
        outsb
        add    byte ptr gs:[bx + si], al
loc_1_1D10:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 65657246h
        dec    ebp
        popad
        jo     SHORT loc_1_1D6F
        imul   esi, dword ptr gs:[bp + di + 74h], 737265h
        add    byte ptr [eax], al
loc_1_1D28:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 65657246h
        push   ebx
        push   64657261h
        dec    ebp
        insd
        outsd
        db  072h, 079h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 49h], 64656d6dh
        imul   esp, dword ptr [ecx + 74h], 61655265h
loc_1_1D50:
        push   eax
        outsd
        jb     SHORT loc_1_1DC9
        push   ebp
        arpl   word ptr [eax + 61h], bp
        jb     SHORT loc_1_1D5B
loc_1_1D5B:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 49h], 64656d6dh
        imul   esp, dword ptr [ecx + 74h], 61655265h
        push   eax
        outsd
loc_1_1D6F:
        db  072h, 074h
        push   ebp
        db  073h, 068h
        outsd
        db  072h, 074h
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 49h], 64656d6dh
        imul   esp, dword ptr [ecx + 74h], 61655265h
        push   eax
        outsd
        jb     SHORT loc_1_1E01
        push   ebp
        insb
        outsd
        outsb
        add    byte ptr [bx + si], al
loc_1_1D94:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 49h], 64656d6dh
        imul   esp, dword ptr [ecx + 74h], 69725765h
        db  074h, 065h
        push   eax
        outsd
        jb     SHORT loc_1_1E1E
        push   ebp
        arpl   word ptr [eax + 61h], bp
        jb     SHORT loc_1_1DB0
loc_1_1DB0:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 49h], 64656d6dh
        imul   esp, dword ptr [ecx + 74h], 69725765h
        db  074h, 065h
        push   eax
        outsd
        db  072h, 074h
        push   ebp
        db  073h, 068h
loc_1_1DC9:
        outsd
        db  072h, 074h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1DD0:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 49h], 64656d6dh
        imul   esp, dword ptr [ecx + 74h], 69725765h
        db  074h, 065h
        push   eax
        outsd
        db  072h, 074h
        push   ebp
        insb
        outsd
        outsb
        add    byte ptr [bp + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 69646e49h
        arpl   word ptr [ecx + 74h], sp
        push   ebx
        je     SHORT loc_1_1E5D
        je     SHORT loc_1_1E73
        jae    SHORT loc_1_1E00
loc_1_1E00:
        dec    esi
loc_1_1E01:
        imul   esi, dword ptr fs:[ebx + 4dh], 69646e49h
        arpl   word ptr [ecx + 74h], sp
        push   ebx
        je     SHORT loc_1_1E71
        je     SHORT loc_1_1E87
        jae    SHORT loc_1_1E57
        outsd
        insd
        db  070h, 06Ch
        db  065h, 074h, 065h
        add    byte ptr [esi + 64h], cl
loc_1_1E1E:
        imul   esi, dword ptr [ebx + 4dh], 74696e49h
        imul   esp, dword ptr [ecx + 6ch], 54657a69h
        imul   ebp, dword ptr [ebp + 65h], 72h
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 74696e49h
        imul   esp, dword ptr [ecx + 6ch], 57657a69h
        jb     SHORT loc_1_1EA7
        db  070h, 070h
        jb     NEAR PTR loc_1_1E4B
loc_1_1E4B:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 4970614dh
        outsd
        push   ebx
loc_1_1E57:
        db  070h, 061h
        arpl   word ptr [ebp], sp
loc_1_1E5C:
        dec    esi
loc_1_1E5D:
        imul   esi, dword ptr fs:[ebx + 4dh], 72657551h
        db  079h, 049h
        outsb
        outsw
        db  072h, 06Dh
        popad
        db  074h, 069h
        outsd
        outsb
loc_1_1E71:
        inc    ebx
        outsd
loc_1_1E73:
        insd
        db  070h, 06Ch
        db  065h, 074h, 065h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 64616552h
        inc    esp
        insd
loc_1_1E87:
        popad
        inc    ebx
        outsd
        db  075h, 06Eh
        db  074h, 065h
        jb     SHORT loc_1_1E90
loc_1_1E90:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 69676552h
        db  073h, 074h
        db  065h, 072h, 044h
        insd
        popad
        inc    ebx
        push   656e6e61h
        insb
loc_1_1EA7:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 69676552h
        db  073h, 074h
        jb     NEAR PTR loc_1_1F03
        imul   ebp, dword ptr [esi + 69h], 74726f70h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 69676552h
        db  073h, 074h
        db  065h, 072h, 049h
        outsb
        db  074h, 065h
        jb     SHORT loc_1_1F45
        jne    SHORT loc_1_1F45
        je     SHORT loc_1_1ED7
loc_1_1ED7:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 69676552h
        db  073h, 074h
        jb     NEAR PTR loc_1_1F2F
        outsd
        push   eax
        outsd
        jb     SHORT loc_1_1F5F
        push   edx
        popad
        outsb
        add    byte ptr gs:[bx + si], al
        add    byte ptr [eax], al
loc_1_1EF4:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 65736552h
        db  074h, 043h
        outsd
        insd
        db  070h, 06Ch
loc_1_1F03:
        db  065h, 074h, 065h
        add    byte ptr [eax], al
loc_1_1F08:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 646e6553h
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
        db  065h, 074h, 065h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 646e6553h
        push   edx
        db  065h, 073h, 06Fh
        db  075h, 072h
        arpl   word ptr [ebp + 73h], sp
        inc    ecx
loc_1_1F2F:
        db  076h, 061h
        imul   ebp, dword ptr [ecx + 62h], 4e00656ch
        imul   esi, dword ptr fs:[ebx + 4dh], 41746553h
        db  074h, 074h
        db  072h, 069h
loc_1_1F45:
        bound  esi, qword ptr [ebp + 74h]
        jae    NEAR PTR loc_1_1F4B
loc_1_1F4B:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 49746553h
        outsb
        outsw
        db  072h, 06Dh
        popad
        db  074h, 069h
        outsd
        outsb
loc_1_1F5F:
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
        db  065h, 074h, 065h
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 636e7953h
        push   696e6f72h
        db  07Ah, 065h
        push   edi
        imul   esi, dword ptr [eax + ebp*2 + 49h], 7265746eh
        db  072h, 075h
        jo     SHORT loc_1_1FF9
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 6e617254h
        jae    SHORT loc_1_1FF9
        jb     NEAR PTR loc_1_1FDA
        popad
        db  074h, 061h
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
        db  065h, 074h, 065h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 616d6e55h
        db  070h, 049h
        outsd
        push   ebx
        jo     SHORT loc_1_2014
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
loc_1_1FB8:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 496e6157h
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 496e6157h
        outsb
loc_1_1FDA:
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
        db  065h, 074h, 065h
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 536e6157h
loc_1_1FF9:
        outsb
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
loc_1_2000:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 536e6157h
        outsb
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
        je     NEAR PTR loc_1_2079
loc_1_2014:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2018:
        inc    esi
        imul   eax, dword ptr fs:[ebx + 72h], 65746165h
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 72h
        add    byte ptr [esi + 64h], al
        imul   eax, dword ptr fs:[ebp + 6ch], 46657465h
        imul   ebp, dword ptr [esp + esi*2 + 65h], 72h
        add    byte ptr [esi + 64h], al
        imul   ecx, dword ptr fs:[esi + 6fh], 69466574h
        insb
        je     SHORT loc_1_20B2
        db  072h, 04Fh
        db  070h, 065h
        outsb
        inc    ecx
        popad
        jo     SHORT loc_1_20CB
        jb     NEAR PTR loc_1_205A
loc_1_205A:
        add    byte ptr [eax], al
loc_1_205C:
        inc    esi
        imul   eax, dword ptr fs:[ebp + 6ch], 46657465h
        imul   ebp, dword ptr [esp + esi*2 + 65h], 65704f72h
        outsb
        inc    ecx
        popad
        jo     SHORT loc_1_20E9
        jb     NEAR PTR loc_1_2078
loc_1_2078:
        inc    esi
loc_1_2079:
        imul   eax, dword ptr fs:[ebx + 68h], 65676e61h
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 6e6f4c72h
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        jae    NEAR PTR loc_1_2096
loc_1_2096:
        add    byte ptr [eax], al
loc_1_2098:
        inc    esi
        imul   eax, dword ptr fs:[ebx + 68h], 65676e61h
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 6f685372h
        db  072h, 074h
        inc    ecx
        jb     NEAR PTR loc_1_2117
loc_1_20B2:
        jae    SHORT loc_1_2127
        jae    NEAR PTR loc_1_20B7
loc_1_20B7:
        add    byte ptr [esi + 64h], al
        imul   edx, dword ptr fs:[ebx + 68h], 646c756fh
        inc    ecx
        jb     NEAR PTR loc_1_212C
        db  073h, 073h
        dec    esp
        outsd
loc_1_20CB:
        outsd
        db  070h, 042h
        popad
        arpl   word ptr [ebx], bp
        add    byte ptr [eax], al
loc_1_20D4:
        inc    esi
        imul   eax, dword ptr fs:[esi + 69h], 7265746ch
        inc    ecx
        push   75h
        db  073h, 074h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_20E8:
        inc    esi
loc_1_20E9:
        imul   ecx, dword ptr fs:[esi + 75h], 7265626dh
        dec    edi
        dec    di
        db  070h, 065h
        outsb
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 6e6f4c72h
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_217C
        jae    NEAR PTR loc_1_210C
loc_1_210C:
        inc    esi
        imul   ecx, dword ptr fs:[esi + 75h], 7265626dh
        dec    edi
loc_1_2117:
        dec    di
        db  070h, 065h
        outsb
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 6f685372h
        db  072h, 074h
loc_1_2127:
        inc    ecx
        db  064h, 064h, 072h, 065h
loc_1_212C:
        db  073h, 073h
        jae    NEAR PTR loc_1_2131
loc_1_2131:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], al
        imul   edx, dword ptr fs:[ecx + 75h], 47797265h
        insb
        outsd
        bound  esp, qword ptr [ecx + 6ch]
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 6e6f4c72h
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        jae    NEAR PTR loc_1_2157
loc_1_2157:
        add    byte ptr [esi + 64h], al
        imul   edx, dword ptr fs:[ecx + 75h], 47797265h
        insb
        outsd
        bound  esp, qword ptr [ecx + 6ch]
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 6f685372h
        db  072h, 074h
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        jae    NEAR PTR loc_1_217C
loc_1_217C:
        inc    esi
        imul   edx, dword ptr fs:[ecx + 75h], 4f797265h
        jo     SHORT loc_1_21ED
        outsb
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 6e6f4c72h
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        jae    NEAR PTR loc_1_219D
loc_1_219D:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], al
        imul   edx, dword ptr fs:[ecx + 75h], 4f797265h
        db  070h, 065h
        outsb
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 6f685372h
        db  072h, 074h
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_2232
        jae    NEAR PTR loc_1_21C2
loc_1_21C2:
        add    byte ptr [eax], al
loc_1_21C4:
        inc    esi
        imul   eax, dword ptr fs:[esi + 69h], 7265746ch
        dec    ecx
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_2242
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], al
        imul   eax, dword ptr fs:[esi + 69h], 7265746ch
        inc    esp
        db  070h, 072h
loc_1_21ED:
        dec    ecx
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2200:
        inc    esi
        imul   eax, dword ptr fs:[esi + 69h], 7265746ch
        dec    ecx
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_227E
        inc    ebx
        outsd
        insd
        jo     SHORT loc_1_228A
        db  065h, 074h, 065h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], al
        imul   eax, dword ptr fs:[esi + 69h], 7265746ch
        inc    esp
        jo     SHORT loc_1_22A3
        dec    ecx
loc_1_2232:
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        inc    ebx
        outsd
loc_1_2242:
        insd
        jo     SHORT loc_1_22B1
        db  065h, 074h, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_224C:
        inc    ecx
        db  072h, 063h
        inc    ebx
        db  072h, 065h
        popad
        je     SHORT loc_1_22BA
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 72410072h
        arpl   word ptr [ebp + 6ch], ax
        je     NEAR PTR loc_1_22CA
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 72410072h
        arpl   word ptr [esi + 6fh], cx
        db  074h, 065h
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 65704f72h
        outsb
        inc    ecx
loc_1_227E:
        popad
        jo     SHORT loc_1_22F6
        jb     NEAR PTR loc_1_2285
loc_1_2285:
        add    byte ptr [eax], al
        add    byte ptr [ecx + 72h], al
loc_1_228A:
        arpl   word ptr [ebp + 6ch], ax
        je     NEAR PTR loc_1_22F6
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 65704f72h
        outsb
        inc    ecx
        popad
        jo     SHORT loc_1_2314
        jb     NEAR PTR loc_1_22A3
loc_1_22A3:
        add    byte ptr [ecx + 72h], al
        arpl   word ptr [esi + 69h], ax
        insb
        je     SHORT loc_1_2311
        db  072h, 041h
        push   75h
loc_1_22B1:
        db  073h, 074h
        add    byte ptr [ecx + 72h], al
        arpl   word ptr [esi + 69h], ax
        insb
loc_1_22BA:
        je     SHORT loc_1_2321
        db  072h, 044h
        db  070h, 072h
        dec    ecx
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
loc_1_22CA:
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        inc    ebx
        outsd
        insd
        jo     SHORT loc_1_2340
        db  065h, 074h, 065h
        add    byte ptr [ecx + 72h], al
        arpl   word ptr [esi + 69h], ax
        insb
        db  074h, 065h
        db  072h, 044h
        jo     SHORT loc_1_2356
        dec    ecx
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        add    byte ptr [ecx + 72h], al
loc_1_22F6:
        arpl   word ptr [esi + 69h], ax
        insb
        db  074h, 065h
        db  072h, 054h
        db  072h, 061h
        outsb
        db  073h, 066h
        db  065h, 072h, 044h
        popad
        db  074h, 061h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 72h], al
        arpl   word ptr [esi + 72h], ax
loc_1_2311:
        dec    esi
loc_1_2314:
        imul   esi, dword ptr fs:[ebx + 50h], 656b6361h
        je     SHORT loc_1_231E
loc_1_231E:
        add    byte ptr [eax], al
loc_1_2320:
        inc    ecx
loc_1_2321:
        db  072h, 063h
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 496f4472h
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 6e6f6974h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 72h], al
        arpl   word ptr [ebp + 73h], ax
        db  074h, 072h
loc_1_2340:
        outsd
        db  079h, 050h
        popad
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_2349
loc_1_2349:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 74h], al
        push   61657243h
        je     SHORT loc_1_23BA
        inc    esi
loc_1_2356:
        imul   ebp, dword ptr [esp + esi*2 + 65h], 74450072h
        push   656c6544h
        je     SHORT loc_1_23CA
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 74450072h
        push   65746f4eh
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 65704f72h
        outsb
        inc    ecx
        popad
        jo     SHORT loc_1_23F6
        jb     NEAR PTR loc_1_2385
loc_1_2385:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 74h], al
        push   656c6544h
        je     SHORT loc_1_23F6
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 65704f72h
        outsb
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_23A3
loc_1_23A3:
        add    byte ptr [ebp + 74h], al
        push   6e616843h
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 64644172h
        db  072h, 065h
        jae    SHORT loc_1_242D
loc_1_23BA:
        jae    NEAR PTR loc_1_23BD
loc_1_23BD:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 74h], al
        push   756f6853h
        insb
        inc    ecx
loc_1_23CA:
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_2443
        dec    esp
        outsd
        outsd
        jo     SHORT loc_1_2417
        popad
        arpl   word ptr [ebx], bp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 74h], al
        push   746c6946h
        jb     NEAR PTR loc_1_2427
        push   75h
        db  073h, 074h
        add    byte ptr [ebp + 74h], al
        push   626d754eh
        jb     NEAR PTR loc_1_2445
loc_1_23F6:
        dec    di
        db  070h, 065h
        outsb
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 64644172h
        db  072h, 065h
        db  073h, 073h
        jae    NEAR PTR loc_1_240B
loc_1_240B:
        add    byte ptr [ebp + 74h], al
        push   72657551h
        db  079h, 047h
        insb
        outsd
loc_1_2417:
        bound  esp, qword ptr [ecx + 6ch]
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 64644172h
        jb     SHORT loc_1_248A
        jae    SHORT loc_1_249A
loc_1_2427:
        jae    NEAR PTR loc_1_242A
loc_1_242A:
        add    byte ptr [eax], al
loc_1_242C:
        inc    ebp
loc_1_242D:
        db  074h, 068h
        push   ecx
        db  075h, 065h
        db  072h, 079h
        dec    edi
        db  070h, 065h
        outsb
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 64644172h
        jb     SHORT loc_1_24A8
loc_1_2443:
        db  073h, 073h
loc_1_2445:
        jae    NEAR PTR loc_1_2448
loc_1_2448:
        inc    ebp
        db  074h, 068h
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 646e4972h
        imul   esp, dword ptr [ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2464:
        inc    ebp
        db  074h, 068h
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 72704472h
        dec    ecx
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        add    byte ptr [ebp + 74h], al
        push   746c6946h
        db  065h, 072h, 049h
loc_1_248A:
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_24FD
        inc    ebx
        outsd
loc_1_249A:
        insd
        jo     SHORT loc_1_2509
        je     NEAR PTR loc_1_2505
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_24A4:
        inc    ebp
        db  074h, 068h
        inc    esi
loc_1_24A8:
        imul   ebp, dword ptr [esp + esi*2 + 65h], 72704472h
        dec    ecx
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_2524
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
        je     NEAR PTR loc_1_252C
        add    byte ptr [edx + esi*2 + 43h], dl
        db  072h, 065h
        popad
        je     SHORT loc_1_2535
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 54000072h
        jb     SHORT loc_1_251F
        insb
        je     NEAR PTR loc_1_2545
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 54000072h
        db  072h, 04Eh
        outsd
        je     SHORT loc_1_2553
        inc    esi
        imul   ebp, dword ptr [esp + esi*2 + 65h], 65704f72h
        outsb
        inc    ecx
        popad
        db  070h, 074h
loc_1_24FD:
        jb     NEAR PTR loc_1_2500
loc_1_2500:
        push   esp
        db  072h, 044h
        insb
loc_1_2505:
        db  065h, 074h, 065h
        inc    esi
loc_1_2509:
        imul   ebp, dword ptr [esp + esi*2 + 65h], 65704f72h
        outsb
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_251A
loc_1_251A:
        add    byte ptr [eax], al
loc_1_251C:
        push   esp
        jb     SHORT loc_1_2562
loc_1_251F:
        push   65676e61h
loc_1_2524:
        inc    esi
        db  075h, 06Eh
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
loc_1_252C:
        popad
        insb
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_25A8
loc_1_2535:
        add    byte ptr [eax], al
        add    byte ptr [edx + esi*2 + 43h], dl
        push   65676e61h
        inc    edi
        db  072h, 06Fh
        db  075h, 070h
loc_1_2545:
        inc    ecx
        jb     NEAR PTR loc_1_25AF
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2550:
        push   esp
        db  072h, 053h
loc_1_2553:
        push   646c756fh
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        dec    esp
        outsd
        outsd
loc_1_2562:
        db  070h, 042h
        popad
        arpl   word ptr [ebx], bp
loc_1_2568:
        push   esp
        db  072h, 046h
        imul   ebp, dword ptr [esp + esi*2 + 65h], 6a644172h
        jne    SHORT loc_1_25E8
        je     SHORT loc_1_2577
loc_1_2577:
        add    byte ptr [edx + esi*2 + 46h], dl
        imul   ebp, dword ptr [esp + esi*2 + 65h], 646e4972h
        imul   esp, dword ptr [ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_25F4
        add    byte ptr [edx + esi*2 + 46h], dl
        imul   ebp, dword ptr [esp + esi*2 + 65h], 72704472h
        dec    ecx
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
loc_1_25A8:
        db  076h, 065h
        add    byte ptr [eax], al
loc_1_25AC:
        push   esp
        jb     SHORT loc_1_25F5
loc_1_25AF:
        imul   ebp, dword ptr [esp + esi*2 + 65h], 646e4972h
        imul   esp, dword ptr [ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
        db  065h, 074h, 065h
        add    byte ptr [edx + esi*2 + 46h], dl
        imul   ebp, dword ptr [esp + esi*2 + 65h], 72704472h
        dec    ecx
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 65526574h
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_1_264B
        inc    ebx
        outsd
loc_1_25E8:
        insd
        db  070h, 06Ch
        db  065h, 074h, 065h
        add    byte ptr [eax], al
loc_1_25F0:
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
loc_1_25F4:
        pop    edi
loc_1_25F5:
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        pop    edi
        push   esp
        dec    edi
        pop    edi
        push   ebx
        push   eax
        inc    ecx
        dec    esi
        pop    edi
        push   eax
        inc    ecx
        inc    edi
        inc    ebp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 53h], 65747379h
        insd
        push   eax
        db  072h, 06Fh
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_1_268D
        jb     SHORT loc_1_2663
        outsd
        db  075h, 06Eh
        je     SHORT loc_1_2625
loc_1_2625:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 72617453h
        je     SHORT loc_1_2675
        db  075h, 066h
        db  066h, 065h, 072h, 050h
        push   63697379h
        popad
        insb
        dec    ebp
        popad
        db  070h, 070h
        imul   ebp, dword ptr [esi + 67h], 69644e00h
loc_1_264B:
        db  073h, 04Dh
        inc    ebx
        outsd
        insd
        db  070h, 06Ch
        je     NEAR PTR loc_1_26BA
        inc    edx
        db  075h, 066h
        db  066h, 065h, 072h, 050h
        push   63697379h
        popad
        insb
loc_1_2663:
        dec    ebp
        popad
        db  070h, 070h
        imul   ebp, dword ptr [esi + 67h], 644e0000h
        imul   esi, dword ptr [ebx + 57h], 65746972h
loc_1_2675:
        inc    ebx
        outsd
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        je     SHORT loc_1_26E9
        outsd
        outsb
        add    byte ptr [eax], al
loc_1_2684:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 69676552h
loc_1_268D:
        db  073h, 074h
        db  065h, 072h, 041h
        popad
        jo     SHORT loc_1_270A
        jb     NEAR PTR loc_1_26EC
        push   6f647475h
        ja     SHORT loc_1_270E
        dec    eax
        popad
        outsb
        insb
        jb     NEAR PTR loc_1_26A8
loc_1_26A8:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 65726544h
        imul   esi, dword ptr [bp + di + 74h], 64417265h
        popad
loc_1_26BA:
        db  070h, 074h
        db  065h, 072h, 053h
        push   6f647475h
        ja     SHORT loc_1_2734
        dec    eax
        popad
        outsb
        insb
        jb     NEAR PTR loc_1_26CE
loc_1_26CE:
        add    byte ptr [eax], al
loc_1_26D0:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 49h], 64656d6dh
        imul   esp, dword ptr [ecx + 74h], 61655265h
        push   eax
        arpl   word ptr [ecx + 53h], bp
        insb
        outsd
        je     SHORT loc_1_2732
loc_1_26E9:
        outsb
        outsw
loc_1_26EC:
        db  072h, 06Dh
        popad
        db  074h, 069h
        outsd
        outsb
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 49h], 64656d6dh
        imul   esp, dword ptr [ecx + 74h], 69725765h
        je     SHORT loc_1_276B
        push   eax
        arpl   word ptr [ecx + 53h], bp
loc_1_270A:
        insb
        outsd
        db  074h, 049h
loc_1_270E:
        outsb
        outsw
        db  072h, 06Dh
        popad
        db  074h, 069h
        outsd
        outsb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_271C:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 52h], 50646165h
        arpl   word ptr [ecx + 53h], bp
        insb
        outsd
        je     SHORT loc_1_2775
        outsb
        outsw
        db  072h, 06Dh
        popad
loc_1_2732:
        je     SHORT loc_1_279D
loc_1_2734:
        outsd
        outsb
        add    byte ptr [eax], al
loc_1_2738:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 57h], 65746972h
        push   eax
        arpl   word ptr [ecx + 53h], bp
        insb
        outsd
        db  074h, 049h
        outsb
        outsw
        jb     SHORT loc_1_27BB
        popad
        db  074h, 069h
        outsd
        outsb
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 41696350h
        jae    SHORT loc_1_27D2
        imul   esp, dword ptr [edi + 6eh], 6f736552h
        db  075h, 072h
        arpl   word ptr [ebp + 73h], sp
loc_1_276B:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4fh], 72726576h
loc_1_2775:
        imul   esp, dword ptr [ebp + 42h], 754e7375h
        insd
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 55h], 74616470h
        push   ebx
        push   64657261h
        dec    ebp
        insd
        outsd
        jb     SHORT loc_1_2813
        add    byte ptr [eax], al
loc_1_279C:
        inc    esp
loc_1_279D:
        bound  esp, qword ptr [edi + 42h]
        db  072h, 065h
        popad
        imul   edx, dword ptr [eax + 6fh], 69h
        outsb
        je     SHORT loc_1_27AA
loc_1_27AA:
        add    byte ptr [eax], al
loc_1_27AC:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 72657551h
        db  079h, 041h
        popad
        jo     SHORT loc_1_282F
loc_1_27BB:
        jb     NEAR PTR loc_1_2810
        db  065h, 073h, 06Fh
        db  075h, 072h
        arpl   word ptr [ebp + 73h], sp
        add    byte ptr [eax], al
loc_1_27C8:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 51h], 79726575h
        inc    edx
loc_1_27D2:
        db  075h, 066h
        db  066h, 065h, 072h, 04Fh
        db  066h, 066h, 073h, 065h
        je     SHORT loc_1_27DE
loc_1_27DE:
        add    byte ptr [eax], al
loc_1_27E0:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 49h], 6974696eh
        popad
        insb
        imul   edi, dword ptr [edx + 65h], 6e657645h
        je     SHORT loc_1_27F4
loc_1_27F4:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 53h], 76457465h
        outsb
        je     SHORT loc_1_2801
loc_1_2801:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 52h], 74657365h
        inc    ebp
        db  076h, 065h
loc_1_2810:
        outsb
        je     SHORT loc_1_2813
loc_1_2813:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 57h], 45746961h
        db  076h, 065h
        outsb
        je     SHORT loc_1_2822
loc_1_2822:
        add    byte ptr [eax], al
loc_1_2824:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 41746553h
        je     SHORT loc_1_28A3
loc_1_282F:
        db  072h, 069h
        bound  esi, qword ptr [ebp + 74h]
        db  065h, 073h, 045h
        js     SHORT loc_1_2839
loc_1_2839:
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 50746553h
        db  065h, 072h, 069h
        outsd
        imul   esp, dword ptr fs:[ebx + 54h], 72656d69h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 65656c53h
        jo     SHORT loc_1_285F
loc_1_285F:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 41h], 73756a64h
        je     SHORT loc_1_28AD
        db  075h, 066h
        db  066h, 065h, 072h, 04Ch
        outsb
        db  067h, 074h, 068h
        add    byte ptr [eax], al
loc_1_2878:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 49h], 6977534dh
        db  074h, 063h
        push   694d6f54h
        outsb
        imul   esi, dword ptr [eax + 6fh], 7472h
loc_1_2890:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 49h], 7665524dh
        jb     NEAR PTR loc_1_2910
        inc    edx
        popad
        arpl   word ptr [ebx], bp
        add    byte ptr [eax], al
loc_1_28A3:
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 49h], 6575514dh
loc_1_28AD:
        db  075h, 065h
        dec    ebp
        imul   ebp, dword ptr [esi + 69h], 74726f70h
        inc    ebx
        popad
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   eax, dword ptr [eax], 4eh
        imul   esi, dword ptr fs:[ebx + 47h], 75437465h
        db  072h, 072h
        outsb
        db  074h, 050h
        db  072h, 06Fh
        arpl   word ptr [ebp + 73h], sp
        db  073h, 06Fh
        db  072h, 043h
        db  070h, 075h
        push   ebp
        db  073h, 061h
        add    byte ptr gs:[bp + 64h], cl
        imul   esi, dword ptr [ebx + 47h], 75437465h
        db  072h, 072h
        outsb
        je     SHORT loc_1_2942
        jns    SHORT loc_1_2964
        je     SHORT loc_1_2958
        insd
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 0
loc_1_28FC:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 44h], 63417270h
        db  071h, 075h
        imul   esi, dword ptr [edx + 65h], 6e697053h
        dec    esp
        outsd
loc_1_2910:
        arpl   word ptr [ebx], bp
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 44h], 65527270h
        insb
        popad
        db  073h, 065h
        push   ebx
        db  070h, 069h
        outsb
        dec    esp
        outsd
        arpl   word ptr [ebx], bp
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 51h], 79726575h
        dec    ebp
        popad
        db  070h, 052h
        imul   esi, dword ptr gs:[bp + di + 74h], 6f437265h
loc_1_2942:
        db  075h, 06Eh
        je     SHORT loc_1_2946
loc_1_2946:
        add    byte ptr [eax], al
loc_1_2948:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 61657243h
        db  074h, 065h
        dec    esp
        outsd
        add    byte ptr [bx + si], al
loc_1_2958:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 4dh], 736f6c43h
        dec    esp
        outsd
loc_1_2964:
        add    byte ptr [bx + si], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 74697257h
        dec    esp
        outsd
        inc    esp
        popad
        je     SHORT loc_1_29DA
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 73756c46h
        push   676f4ch
        add    byte ptr [eax], al
loc_1_298C:
        dec    esi
        imul   esi, dword ptr fs:[ebx + 49h], 4174696eh
        outsb
        jae    SHORT loc_1_2A01
        push   ebx
        db  074h, 072h
        imul   ebp, dword ptr [esi + 67h], 644e0000h
        imul   esi, dword ptr [ebx + 49h], 5574696eh
        outsb
        imul   esp, dword ptr [ebx + 6fh], 74536564h
        db  072h, 069h
        outsb
        add    byte ptr [bx + si], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 41h], 5369736eh
        db  074h, 072h
        imul   ebp, dword ptr [esi + 67h], 6e556f54h
        imul   esp, dword ptr [ebx + 6fh], 74536564h
        db  072h, 069h
        outsb
        add    byte ptr [bx + si], al
        add    byte ptr [esi + 64h], cl
loc_1_29DA:
        imul   esi, dword ptr [ebx + 55h], 6f63696eh
        push   ebx
        je     SHORT loc_1_2A58
        imul   ebp, dword ptr [esi + 67h], 6e416f54h
        jae    SHORT loc_1_2A58
        push   ebx
        db  074h, 072h
        imul   ebp, dword ptr [esi + 67h], 4e000000h
        imul   esi, dword ptr fs:[ebx + 49h], 6765524dh
loc_1_2A01:
        imul   esi, dword ptr [ebx + 74h], 614c7265h
        jns    SHORT loc_1_2A6F
        jb     SHORT loc_1_2A71
        dec    ebp
        imul   ebp, dword ptr [esi + 69h], 74726f70h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 49h], 696e494dh
        je     SHORT loc_1_2A8C
        popad
        insb
        imul   edi, dword ptr [edx + 65h], 69766544h
        arpl   word ptr [ebp + 49h], sp
        outsb
        jae    SHORT loc_1_2AA6
        popad
        outsb
        arpl   word ptr [ebp], sp
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 52h], 72757465h
        outsb
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_2ABC
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 4dh], 6f6c6c41h
        arpl   word ptr [ecx + 74h], sp
loc_1_2A58:
        push   ebx
        push   64657261h
        dec    ebp
        insd
        outsd
        jb     SHORT loc_1_2ADE
        inc    ecx
        jae    SHORT loc_1_2AE1
        outsb
        arpl   word ptr [eax], ax
        add    byte ptr [edx + 45h], dl
        inc    ecx
loc_1_2A6F:
        inc    esp
        pop    edi
loc_1_2A71:
        push   eax
        dec    edi
        push   edx
        push   esp
        pop    edi
        push   ebp
        inc    ebx
        dec    eax
        inc    ecx
        push   edx
        add    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        pop    edi
        push   eax
        dec    edi
        push   edx
        push   esp
        pop    edi
        push   ebp
        push   ebx
        dec    eax
        dec    edi
        push   edx
        push   esp
loc_1_2A8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A90:
        push   edx
        inc    ebp
        inc    ecx
        inc    esp
        pop    edi
        push   eax
        dec    edi
        push   edx
        push   esp
        pop    edi
        push   ebp
        dec    esp
        dec    edi
        dec    esi
        inc    edi
        add    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        pop    edi
        push   eax
loc_1_2AA6:
        dec    edi
        push   edx
        push   esp
        pop    edi
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        pop    edi
        push   ebp
        inc    ebx
        dec    eax
        inc    ecx
        push   edx
        add    byte ptr [eax], al
loc_1_2AB8:
        push   edx
        inc    ebp
        inc    ecx
        inc    esp
loc_1_2ABC:
        pop    edi
        push   eax
        dec    edi
        push   edx
        push   esp
        pop    edi
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        pop    edi
        push   ebp
        push   ebx
        dec    eax
        dec    edi
        push   edx
        push   esp
        add    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        pop    edi
        push   eax
        dec    edi
        push   edx
        push   esp
        pop    edi
        inc    edx
        push   ebp
        inc    esi
        inc    esi
loc_1_2ADE:
        inc    ebp
        push   edx
        pop    edi
loc_1_2AE1:
        push   ebp
        dec    esp
        dec    edi
        dec    esi
        inc    edi
        add    byte ptr [eax], al
loc_1_2AE8:
        push   edi
        push   edx
        dec    ecx
        push   esp
        inc    ebp
        pop    edi
        push   eax
        dec    edi
        push   edx
        push   esp
        pop    edi
        push   ebp
        inc    ebx
        dec    eax
        inc    ecx
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2AFC:
        push   edi
        push   edx
        dec    ecx
        push   esp
        inc    ebp
        pop    edi
        push   eax
        dec    edi
        push   edx
        push   esp
        pop    edi
        push   ebp
        push   ebx
        dec    eax
        dec    edi
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [edi + 52h], dl
        dec    ecx
        push   esp
        inc    ebp
        pop    edi
        push   eax
        dec    edi
        push   edx
        push   esp
        pop    edi
        push   ebp
        dec    esp
        dec    edi
        dec    esi
        inc    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B24:
        push   edi
        push   edx
        dec    ecx
        push   esp
        inc    ebp
        pop    edi
        push   eax
        dec    edi
        push   edx
        push   esp
        pop    edi
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        pop    edi
        push   ebp
        inc    ebx
        dec    eax
        inc    ecx
        push   edx
        add    byte ptr [edi + 52h], dl
        dec    ecx
        push   esp
        inc    ebp
        pop    edi
        push   eax
        dec    edi
        push   edx
        push   esp
        pop    edi
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        pop    edi
        push   ebp
        push   ebx
        dec    eax
        dec    edi
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B58:
        push   edi
        push   edx
        dec    ecx
        push   esp
        inc    ebp
        pop    edi
        push   eax
        dec    edi
        push   edx
        push   esp
        pop    edi
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        pop    edi
        push   ebp
        dec    esp
        dec    edi
        dec    esi
        inc    edi
        add    byte ptr [ebx + 65h], cl
        push   ebx
        db  074h, 061h
        insb
        insb
        inc    ebp
        db  078h, 065h
        arpl   word ptr [ebp + 74h], si
        imul   ebp, dword ptr [edi + 6eh], 636f7250h
        jae    NEAR PTR loc_1_2BFA
        outsd
        jb     SHORT loc_1_2B8A
loc_1_2B8A:
        add    byte ptr [eax], al
loc_1_2B8C:
        dec    ebx
        inc    ecx
        arpl   word ptr [ecx + 75h], si
        imul   esi, dword ptr [edx + 65h], 6e697053h
        dec    esp
        outsd
        arpl   word ptr [ebx], bp
        add    byte ptr [eax], al
loc_1_2BA0:
        dec    ebx
        inc    ecx
        arpl   word ptr [ecx + 75h], si
        imul   esi, dword ptr [edx + 65h], 6e697053h
        dec    esp
        outsd
        arpl   word ptr [ebx + 41h], bp
        db  074h, 044h
        jo     SHORT loc_1_2C19
        dec    esp
        jbe    NEAR PTR loc_1_2C1F
        insb
        add    byte ptr [ebx + 65h], cl
        push   edx
        insb
        popad
        db  073h, 065h
        push   ebx
        jo     SHORT loc_1_2C31
        outsb
        dec    esp
        outsd
        arpl   word ptr [ebx], bp
        add    byte ptr [eax], al
loc_1_2BD0:
        dec    ebx
        push   ebx
        db  079h, 06Eh
        arpl   word ptr [eax + 72h], bp
        outsd
        outsb
        imul   edi, dword ptr [edx + 65h], 63657845h
        db  075h, 074h
        imul   ebp, dword ptr [edi + 6eh], 654b0000h
        push   edx
        insb
        popad
        db  073h, 065h
        push   ebx
        db  070h, 069h
        outsb
        dec    esp
        outsd
        arpl   word ptr [ebx + 46h], bp
loc_1_2BFA:
        jb     SHORT loc_1_2C6B
        insd
        inc    esp
        db  070h, 063h
        dec    esp
        jbe    NEAR PTR loc_1_2C69
        insb
        add    byte ptr [eax], al
        add    byte ptr [ebp + 78h], al
        dec    ecx
        outsb
        db  074h, 065h
        db  072h, 06Ch
        outsd
        arpl   word ptr [ebx + 65h], bp
        inc    ecx
        push   ebp
loc_1_2C19:
        insb
        outsd
        outsb
        add    byte ptr [bx + si], al
loc_1_2C1F:
        add    byte ptr [ebp + 78h], al
        dec    ecx
        outsb
        je     SHORT loc_1_2C8B
        jb     SHORT loc_1_2C94
        outsd
        arpl   word ptr [ebx + 65h], bp
        dec    ecx
        outsb
        db  073h, 065h
loc_1_2C31:
        db  072h, 074h
        dec    eax
        popad
        dec    esp
        imul   esi, dword ptr [ebx + 74h], 49784500h
        outsb
        db  074h, 065h
        jb     SHORT loc_1_2CB0
        outsd
        arpl   word ptr [ebx + 65h], bp
        dec    ecx
        outsb
        db  073h, 065h
        db  072h, 074h
        push   esp
        popad
        imul   ebp, dword ptr [esp + ecx*2 + 69h], 45007473h
        js     SHORT loc_1_2CA4
        outsb
        db  074h, 065h
        jb     SHORT loc_1_2CCC
        outsd
        arpl   word ptr [ebx + 65h], bp
        push   edx
        insd
        outsd
loc_1_2C69:
        jbe    SHORT loc_1_2CD0
loc_1_2C6B:
        dec    eax
        popad
        dec    esp
        imul   esi, dword ptr [ebx + 74h], 466f4900h
        insb
        jne    SHORT loc_1_2CED
        push   70616441h
        je     SHORT loc_1_2CE6
        db  072h, 042h
        db  075h, 066h
        jb     NEAR PTR loc_1_2CFC
        add    byte ptr [eax], al
loc_1_2C8B:
        add    byte ptr [ecx + 6fh], cl
        dec    ebp
        popad
        jo     SHORT loc_1_2CE6
        db  072h, 061h
loc_1_2C94:
        outsb
        db  073h, 066h
        jb     NEAR PTR loc_1_2C9A
loc_1_2C9A:
        add    byte ptr [eax], al
loc_1_2C9C:
        push   edx
        je     SHORT loc_1_2D0B
        inc    ebp
        jno    SHORT loc_1_2D17
        popad
        insb
loc_1_2CA4:
        push   ebp
        outsb
        imul   esp, dword ptr [ebx + 6fh], 74536564h
        db  072h, 069h
        outsb
loc_1_2CB0:
        add    byte ptr [bx + si], al
        add    byte ptr [edx + 74h], dl
        insb
        inc    ebx
        outsd
        jo     SHORT loc_1_2D34
        push   ebp
        outsb
        imul   esp, dword ptr [ebx + 6fh], 74536564h
        db  072h, 069h
        outsb
        add    byte ptr [bx + si], al
        add    byte ptr [eax], al
loc_1_2CCC:
        push   edx
        je     SHORT loc_1_2D3B
        inc    ecx
loc_1_2CD0:
        db  073h, 073h
        jb     NEAR PTR loc_1_2D49
        add    byte ptr [eax], al
        add    byte ptr [ebx + 65h], cl
        inc    edx
        db  075h, 067h
        inc    ebx
        push   6b6365h
        add    byte ptr [ebx + 65h], cl
loc_1_2CE6:
        dec    esi
        jne    SHORT loc_1_2D56
        bound  esp, qword ptr [ebp + 72h]
        push   eax
loc_1_2CED:
        jb     SHORT loc_1_2D5E
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_1_2D63
        jb     SHORT loc_1_2D69
        add    byte ptr [eax], al
loc_1_2CF8:
        inc    esp
        bound  esp, qword ptr [edi + 50h]
loc_1_2CFC:
        jb     SHORT loc_1_2D67
        outsb
        je     SHORT loc_1_2D01
loc_1_2D01:
        add    byte ptr [eax], al
        add    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        pop    edi
        push   edx
        inc    ebp
loc_1_2D0B:
        inc    edi
        dec    ecx
        push   ebx
        push   esp
        inc    ebp
        push   edx
        pop    edi
        push   ebp
        inc    ebx
        dec    eax
        inc    ecx
        push   edx
loc_1_2D17:
        add    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        pop    edi
        push   edx
        inc    ebp
        inc    edi
        dec    ecx
        push   ebx
        push   esp
        inc    ebp
        push   edx
        pop    edi
        push   ebp
        push   ebx
        dec    eax
        dec    edi
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2D30:
        push   edx
        inc    ebp
        inc    ecx
        inc    esp
loc_1_2D34:
        pop    edi
        push   edx
        inc    ebp
        inc    edi
        dec    ecx
        push   ebx
        push   esp
loc_1_2D3B:
        inc    ebp
        push   edx
        pop    edi
        push   ebp
        dec    esp
        dec    edi
        dec    esi
        inc    edi
        add    byte ptr [edi + 52h], dl
        dec    ecx
        push   esp
        inc    ebp
loc_1_2D49:
        pop    edi
        push   edx
        inc    ebp
        inc    edi
        dec    ecx
        push   ebx
        push   esp
        inc    ebp
        push   edx
        pop    edi
        push   ebp
        inc    ebx
        dec    eax
loc_1_2D56:
        inc    ecx
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2D5C:
        push   edi
        push   edx
loc_1_2D5E:
        dec    ecx
        push   esp
        inc    ebp
        pop    edi
        push   edx
loc_1_2D63:
        inc    ebp
        inc    edi
        dec    ecx
        push   ebx
loc_1_2D67:
        push   esp
        inc    ebp
loc_1_2D69:
        push   edx
        pop    edi
        push   ebp
        push   ebx
        dec    eax
        dec    edi
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [edi + 52h], dl
        dec    ecx
        push   esp
        inc    ebp
        pop    edi
        push   edx
        inc    ebp
        inc    edi
        dec    ecx
        push   ebx
        push   esp
        inc    ebp
        push   edx
        pop    edi
        push   ebp
        dec    esp
        dec    edi
        dec    esi
        inc    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2D8C:
        push   ebx
        jns    SHORT loc_1_2E02
        je     SHORT loc_1_2DF6
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
        je     NEAR PTR loc_1_2E01
        inc    ebx
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        pop    esp
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 656e6f5ah
        dec    ecx
        outsb
        outsw
        db  072h, 06Dh
        popad
        je     SHORT loc_1_2E27
        outsd
        outsb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2DC4:
        inc    ecx
        arpl   word ptr [ecx + ebp*2 + 76h], si
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 73616942h
        add    byte ptr [eax], al
        dec    esi
        je     NEAR PTR loc_1_2DD8
loc_1_2DD8:
        dec    esi
        je     NEAR PTR loc_1_2DDC
loc_1_2DDC:
        dec    esi
        je     NEAR PTR loc_1_2E34
        jb     SHORT loc_1_2E43
        outsb
        jae    SHORT loc_1_2DE5
loc_1_2DE5:
        add    byte ptr [eax], al
        add    byte ptr [esi + 65h], cl
        db  074h, 054h
        db  072h, 061h
        outsb
        jae    SHORT loc_1_2DF1
loc_1_2DF1:
        add    byte ptr [eax], al
        add    byte ptr [esi + 44h], cl
loc_1_2DF6:
        dec    ecx
        push   ebx
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2E00:
        dec    eax
loc_1_2E01:
        inc    ecx
loc_1_2E02:
        dec    esp
        inc    esp
        dec    esp
        dec    esp
        add    byte ptr [esi + 54h], cl
        dec    edi
        push   ebx
        dec    ebx
        push   edx
        dec    esi
        dec    esp
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2E18:
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        jo     SHORT loc_1_2E93
        outsd
        je     SHORT loc_1_2E93
        arpl   word ptr [edi + 6ch], bp
loc_1_2E27:
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_2E2C:
        pop    esp
        jo     SHORT loc_1_2EA3
        outsd
        je     SHORT loc_1_2EA3
loc_1_2E34:
        arpl   word ptr [edi + 6ch], bp
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_2E3C:
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
loc_1_2E43:
        jb     SHORT loc_1_2EAA
        db  071h, 075h
        jae    NEAR PTR loc_1_2EBE
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_2E50:
        pop    esp
        jb     SHORT loc_1_2EBA
        db  071h, 075h
        jae    NEAR PTR loc_1_2ECE
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_2E60:
        pop    esp
        jb     SHORT loc_1_2ECA
        db  071h, 075h
        jae    NEAR PTR loc_1_2EDE
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_2E70:
        pop    esp
        jb     SHORT loc_1_2EDA
        db  071h, 075h
        jae    NEAR PTR loc_1_2EEE
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_2E80:
        pop    esp
        jb     SHORT loc_1_2EEA
        db  071h, 075h
        jae    NEAR PTR loc_1_2EFE
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_2E90:
        pop    esp
loc_1_2E93:
        jb     SHORT loc_1_2EFA
        db  071h, 075h
        jae    NEAR PTR loc_1_2F0E
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_2EA0:
        pop    esp
loc_1_2EA3:
        jb     SHORT loc_1_2F0A
        db  071h, 075h
        jae    NEAR PTR loc_1_2F1E
loc_1_2EAA:
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [eax], al
loc_1_2EB0:
        pop    esp
        jb     SHORT loc_1_2F1A
        jno    SHORT loc_1_2F2C
        db  065h, 073h, 074h
loc_1_2EBA:
        insd
        arpl   word ptr cs:[eax], ax
loc_1_2EBE:
        add    byte ptr [eax], al
loc_1_2EC0:
        pop    esp
        db  072h, 065h
        db  071h, 075h
        jae    NEAR PTR loc_1_2F3E
loc_1_2ECA:
        insd
        arpl   word ptr cs:[eax], ax
loc_1_2ECE:
        add    byte ptr [eax], al
loc_1_2ED0:
        pop    esp
        db  072h, 065h
        db  071h, 075h
        db  065h, 073h, 074h
loc_1_2EDA:
        insd
        arpl   word ptr cs:[eax], ax
loc_1_2EDE:
        add    byte ptr [eax], al
loc_1_2EE0:
        pop    esp
        jb     SHORT loc_1_2F4A
        jno    SHORT loc_1_2F5C
        db  065h, 073h, 074h
loc_1_2EEA:
        insd
        arpl   word ptr cs:[eax], ax
loc_1_2EEE:
        add    byte ptr [eax], al
loc_1_2EF0:
        pop    esp
        db  072h, 065h
        db  071h, 075h
        jae    NEAR PTR loc_1_2F6E
loc_1_2EFA:
        insd
        arpl   word ptr cs:[eax], ax
loc_1_2EFE:
        add    byte ptr [eax], al
loc_1_2F00:
        pop    esp
        db  072h, 065h
        db  071h, 075h
        db  065h, 073h, 074h
loc_1_2F0A:
        insd
        arpl   word ptr cs:[eax], ax
loc_1_2F0E:
        add    byte ptr [eax], al
loc_1_2F10:
        pop    esp
        jb     SHORT loc_1_2F7A
        jno    SHORT loc_1_2F8C
        db  065h, 073h, 074h
loc_1_2F1A:
        insd
        arpl   word ptr cs:[eax], ax
loc_1_2F1E:
        add    byte ptr [eax], al
loc_1_2F20:
        pop    esp
        db  074h, 069h
        insd
        db  065h, 072h, 06Dh
        arpl   word ptr cs:[eax], ax
loc_1_2F2C:
        pop    esp
        db  074h, 069h
        insd
        db  065h, 072h, 06Dh
        arpl   word ptr cs:[eax], ax
loc_1_2F38:
        pop    esp
        db  074h, 069h
        insd
loc_1_2F3E:
        db  065h, 072h, 06Dh
        arpl   word ptr cs:[eax], ax
loc_1_2F44:
        pop    esp
        db  074h, 069h
        insd
loc_1_2F4A:
        db  065h, 072h, 06Dh
        arpl   word ptr cs:[eax], ax
loc_1_2F50:
        pop    esp
        db  074h, 069h
        insd
        jb     NEAR PTR loc_1_2FC6
        arpl   word ptr cs:[eax], ax
loc_1_2F5C:
        pop    esp
        db  074h, 069h
        insd
        db  065h, 072h, 06Dh
        arpl   word ptr cs:[eax], ax
loc_1_2F68:
        pop    esp
        je     SHORT loc_1_2FD6
        insd
loc_1_2F6E:
        db  065h, 072h, 06Dh
        arpl   word ptr cs:[eax], ax
loc_1_2F74:
        pop    esp
        je     SHORT loc_1_2FE2
        insd
loc_1_2F7A:
        db  065h, 072h, 06Dh
        arpl   word ptr cs:[eax], ax
loc_1_2F80:
        pop    esp
        je     SHORT loc_1_2FEE
        insd
        db  065h, 072h, 06Dh
        arpl   word ptr cs:[eax], ax
loc_1_2F8C:
        pop    esp
        je     SHORT loc_1_2FFA
        insd
        db  065h, 072h, 06Dh
        arpl   word ptr cs:[eax], ax
loc_1_2F98:
        pop    esp
        je     SHORT loc_1_3006
        insd
        db  065h, 072h, 06Dh
        arpl   word ptr cs:[eax], ax
loc_1_2FA4:
        pop    esp
        je     SHORT loc_1_3012
        insd
        db  065h, 072h, 06Dh
        arpl   word ptr cs:[eax], ax
loc_1_2FB0:
        pop    esp
        ja     SHORT loc_1_3027
        popad
        db  070h, 070h
        jb     NEAR PTR loc_1_2FE9
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        db  077h, 072h
        popad
loc_1_2FC6:
        jo     SHORT loc_1_3038
        jb     NEAR PTR loc_1_2FF9
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        pop    esp
        db  073h, 065h
        outsb
loc_1_2FD6:
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [esi], ch
        pop    esp
        db  073h, 065h
        outsb
loc_1_2FE2:
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [esi], ch
loc_1_2FE9:
        pop    esp
        db  073h, 065h
        outsb
loc_1_2FEE:
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [esi], ch
        pop    esp
        jae    SHORT loc_1_305E
loc_1_2FF9:
        outsb
loc_1_2FFA:
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [esi], ch
        pop    esp
        jae    SHORT loc_1_306A
        outsb
loc_1_3006:
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [esi], ch
        pop    esp
        jae    SHORT loc_1_3076
        outsb
loc_1_3012:
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [esi], ch
        pop    esp
        db  073h, 065h
        outsb
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [esi], ch
        pop    esp
loc_1_3027:
        jae    SHORT loc_1_308E
        outsb
        insd
        arpl   word ptr cs:[eax], ax
        add    byte ptr [esi], ch
        pop    esp
        db  073h, 065h
        outsb
        insd
loc_1_3038:
        arpl   word ptr cs:[eax], ax
        add    byte ptr [esi], ch
        pop    esp
        db  073h, 065h
        outsb
        insd
        arpl   word ptr cs:[eax], ax
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
sub_1_3050:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [eax + 8], ecx
        mov    eax, dword ptr [esp + 4]
loc_1_305E:
        mov    edx, dword ptr [eax + 30h]
        mov    dword ptr [eax + 8], edx
        ret    8
        db  08Bh, 0FFh, 08Dh
loc_1_306A:
        movsb
        and    al, 0
        add    byte ptr [eax], al
        db  000h
sub_1_3070:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
loc_1_3076:
        push   ebp
        xor    edi, edi
        mov    dword ptr [esp + 18h], edi
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jbe    SHORT loc_1_30A5
        db  0B8h, 000h, 000h, 000h, 000h
loc_1_308A:
        mov    ecx, dword ptr [esp + 28h]
loc_1_308E:
        cmp    dword ptr [eax], ecx
        je     SHORT loc_1_30A5
        add    eax, 4
        inc    dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 18h]
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        ja     SHORT loc_1_308A
loc_1_30A5:
        mov    ecx, dword ptr [esp + 18h]
        lea    eax, [esp + 1ch]
        push   ecx
        xor    ebp, ebp
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_30B6:
        mov    ecx, dword ptr [esp + 24h]
        add    esp, 8
        cmp    byte ptr [ecx], 0
        je     SHORT loc_1_30CE
loc_1_30C2:
        inc    ebp
        mov    eax, dword ptr [esp + 1ch]
        cmp    byte ptr [ebp + eax], 0
        jne    SHORT loc_1_30C2
loc_1_30CE:
        inc    ebp
        mov    eax, dword ptr [esp + 2ch]
        shl    eax, 4
        mov    ecx, dword ptr [esp + 18h]
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        lea    ebx, [eax + ebp + 0ah]
        ja     SHORT loc_1_30E9
        add    ebx, 10h
loc_1_30E9:
        mov    esi, dword ptr [esp + 24h]
        test   esi, esi
        je     SHORT loc_1_3156
        cmp    byte ptr [esp + 34h], 0
        je     SHORT loc_1_3101
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_3156
loc_1_3101:
        cmp    dword ptr [esi + 64h], 0
        jne    SHORT loc_1_3115
        push   7530h
        lea    eax, [esi + 64h]
        push   eax
        call   NDIS_Service_12
loc_1_3115:
        mov    eax, dword ptr [esi + 60h]
        inc    eax
        mov    dword ptr [esi + 60h], eax
        cmp    eax, 5
        jbe    SHORT loc_1_312E
        call   sub_1_68B7
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_312E:
        jne    SHORT loc_1_3156
        lea    eax, [esp + 18h]
        lea    ecx, [esi + 64h]
        push   eax
        push   ecx
        call   NDIS_Service_13
        push   0
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_314B:
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_3156:
        xor    eax, eax
        xor    ecx, ecx
        mov    ax, word ptr [esi + 58h]
        push   0
        mov    cx, word ptr [esi + 10h]
        add    eax, ecx
        lea    edx, [ebx + eax + 2]
        push   edx
        call   sub_1_6563
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_1_3358
        test   esi, esi
        je     SHORT loc_1_31CB
        mov    ecx, dword ptr [esi + 5ch]
        test   ecx, ecx
        je     SHORT loc_1_31A3
        xor    eax, eax
        xor    edi, edi
        mov    ax, word ptr [esi + 58h]
        dec    eax
        push   eax
        push   ecx
        push   ebx
        call   sub_1_64BC
        mov    di, word ptr [esi + 58h]
        dec    edi
        mov    byte ptr [edi + ebx], 20h
        inc    edi
loc_1_31A3:
        mov    byte ptr [edi + ebx], 28h
        inc    edi
        xor    eax, eax
        mov    ecx, dword ptr [esi + 14h]
        mov    ax, word ptr [esi + 10h]
        lea    edx, [edi + ebx]
        dec    eax
        push   eax
        push   ecx
        push   edx
        call   sub_1_64BC
        xor    eax, eax
        mov    ax, word ptr [esi + 10h]
        dec    eax
        add    edi, eax
        mov    byte ptr [edi + ebx], 29h
        inc    edi
loc_1_31CB:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_3276
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_3276
        test   esi, esi
        je     SHORT loc_1_31F3
        test   byte ptr [esi + 41h], 2
        jne    NEAR PTR loc_1_3276
loc_1_31F3:
        lea    ecx, [edi + 0dh]
        mov    byte ptr [edi + ebx], 0
        mov    word ptr [esp + 12h], cx
        inc    edi
        mov    eax, dword ptr [esp + 12h]
        push   0
        and    eax, 0ffffh
        push   eax
        call   sub_1_6563
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_3275
        mov    cx, word ptr [esp + 12h]
        push   edi
        mov    dword ptr [eax], 1838h
        push   ebx
        mov    dword ptr [eax + 4], 28h
        mov    word ptr [eax + 8], cx
        add    eax, 0ch
        push   eax
        mov    byte ptr [eax - 2], 0
        mov    byte ptr [eax - 1], 1
        call   sub_1_64BC
        mov    ecx, dword ptr [esp + 14h]
        push   ecx
        push   30ah
        push   30ah
        call   sub_1_6622
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 0ch
        push   0
        push   ecx
        call   sub_1_65A5
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        add    esp, 8
loc_1_3275:
        dec    edi
loc_1_3276:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_334D
        mov    byte ptr [edi + ebx], 2eh
        inc    edi
        lea    eax, [ebp - 1]
        inc    edi
        push   eax
        lea    edx, [edi + ebx]
        mov    byte ptr [edi + ebx - 1], 20h
        mov    ecx, dword ptr [esp + 20h]
        lea    edi, [ebp + edi - 1]
        push   ecx
        push   edx
        call   sub_1_64BC
        mov    ecx, dword ptr [esp + 18h]
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        ja     SHORT loc_1_32C4
        lea    eax, [edi + ebx]
        mov    ecx, dword ptr [esp + 28h]
        push   eax
        add    edi, 0ah
        push   ecx
        call   sub_1_6519
        add    esp, 8
loc_1_32C4:
        cmp    dword ptr [esp + 2ch], 1
        jb     SHORT loc_1_3302
        mov    ebp, dword ptr [esp + 30h]
        mov    eax, dword ptr [esp + 2ch]
        add    ebp, 4
        mov    dword ptr [esp + 18h], eax
loc_1_32DA:
        lea    eax, [edi + ebx + 2]
        mov    byte ptr [edi + ebx], 2ch
        mov    byte ptr [edi + ebx + 1], 20h
        inc    edi
        mov    ecx, dword ptr [ebp]
        inc    edi
        push   eax
        add    edi, 0ah
        push   ecx
        add    ebp, 4
        call   sub_1_6519
        add    esp, 8
        dec    dword ptr [esp + 18h]
        jne    SHORT loc_1_32DA
loc_1_3302:
        mov    byte ptr [edi + ebx], 0dh
        inc    edi
        inc    edi
        push   0
        push   0ch
        mov    byte ptr [edi + ebx - 1], 0ah
        call   sub_1_6563
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_3358
        mov    dword ptr [eax], ebx
        test   esi, esi
        mov    dword ptr [eax + 4], edi
        je     SHORT loc_1_332E
        mov    ecx, dword ptr [esi + 38h]
        mov    dword ptr [eax + 8], ecx
        jmp    SHORT loc_1_3335
loc_1_332E:
        mov    dword ptr [eax + 8], 0
loc_1_3335:
        push   1
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3342:
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_334D:
        push   0
        push   ebx
        call   sub_1_65A5
        add    esp, 8
loc_1_3358:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_3360:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3370:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3380:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3390:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_33A0:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        add    eax, dword ptr [edx]
        mov    dword ptr [edx], eax
        ret    0ch
        db  090h
loc_1_33B0:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [ecx]
        cmp    esi, ecx
        mov    eax, esi
        jne    SHORT loc_1_33BF
        xor    eax, eax
loc_1_33BF:
        mov    edx, dword ptr [esp + 0ch]
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [esi + 4], edx
        pop    esi
        mov    dword ptr [ecx], edx
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_33E0:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [ecx + 4]
        cmp    esi, ecx
        mov    eax, esi
        jne    SHORT loc_1_33F0
        xor    eax, eax
loc_1_33F0:
        mov    edx, dword ptr [esp + 0ch]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], esi
        mov    dword ptr [esi], edx
        pop    esi
        mov    dword ptr [ecx + 4], edx
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3410:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3430:
        mov    al, 1
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3440:
        sub    esp, 8
        xor    eax, eax
        mov    edx, eax
        add    esp, 8
        ret    18h
        db  08Dh, 049h, 000h
loc_1_3450:
        mov    edx, dword ptr [esp + 4]
        in     al, dx
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3460:
        mov    edx, dword ptr [esp + 4]
        in     ax, dx
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3470:
        mov    edx, dword ptr [esp + 4]
        in     eax, dx
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3480:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        cld
        rep insb
        pop    edi
        pop    esi
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_34A0:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        cld
        rep insw
        pop    edi
        pop    esi
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_34C0:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        cld
        rep insd
        pop    edi
        pop    esi
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_34E0:
        mov    edx, dword ptr [esp + 4]
        mov    al, byte ptr [esp + 8]
        out    dx, al
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_34F0:
        mov    ax, word ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        out    dx, ax
        ret    8
        db  08Bh, 0FFh
loc_1_3500:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        out    dx, eax
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_3510:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsb
        pop    esi
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3530:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsw
        pop    esi
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3550:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsd
        pop    esi
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3570:
        mov    cl, byte ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax], cl
        ret    8
        db  08Dh, 049h, 000h
loc_1_3580:
        mov    cx, word ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax], cx
        ret    8
        db  090h
loc_1_3590:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        ret    8
        db  08Dh, 049h, 000h
loc_1_35A0:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax]
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_35B0:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax]
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_35C0:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax]
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_35D0:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    si, word ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        shr    si, 1
        mov    edi, dword ptr [esp + 14h]
        mov    ax, word ptr [edi]
        mov    edx, dword ptr [edi + 4]
        shr    ax, 1
        cmp    ax, si
        je     SHORT loc_1_35FC
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_1_35FC:
        cmp    byte ptr [esp + 18h], 0
        je     SHORT loc_1_3655
        test   si, si
        je     SHORT loc_1_3645
loc_1_3608:
        mov    edi, ecx
        mov    eax, edx
        mov    ax, word ptr [eax]
        add    ecx, 2
        add    edx, 2
        cmp    word ptr [edi], ax
        je     SHORT loc_1_3641
        mov    bx, word ptr [ecx - 2]
        cmp    bl, 61h
        jb     SHORT loc_1_362C
        cmp    bl, 7ah
        ja     SHORT loc_1_362C
        sub    bx, 20h
loc_1_362C:
        mov    ax, word ptr [edx - 2]
        cmp    al, 61h
        jb     SHORT loc_1_363C
        cmp    al, 7ah
        ja     SHORT loc_1_363C
        sub    ax, 20h
loc_1_363C:
        cmp    ax, bx
        jne    SHORT loc_1_364D
loc_1_3641:
        dec    si
        jne    SHORT loc_1_3608
loc_1_3645:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_1_364D:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_1_3655:
        test   si, si
        je     SHORT loc_1_3645
loc_1_365A:
        mov    edi, ecx
        mov    eax, edx
        mov    ax, word ptr [eax]
        add    ecx, 2
        add    edx, 2
        cmp    word ptr [edi], ax
        jne    SHORT loc_1_3678
        dec    si
        jne    SHORT loc_1_365A
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_1_3678:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_1_3680:
        mov    eax, dword ptr [esp + 8]
        push   ebx
        push   esi
        test   eax, eax
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        je     SHORT loc_1_36D5
        mov    edx, dword ptr [ebp + 4]
        mov    esi, dword ptr [eax + 4]
        xor    ebx, ebx
        mov    bx, word ptr [eax]
        mov    ax, word ptr [ebp + 2]
        cmp    bx, ax
        jbe    SHORT loc_1_36A7
        movzx  ebx, ax
loc_1_36A7:
        mov    word ptr [ebp], bx
        mov    edi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    ax, word ptr [ebp + 2]
        cmp    word ptr [ebp], ax
        jae    SHORT loc_1_36DB
        and    ebx, 0fffffffeh
        pop    ebp
        mov    word ptr [ebx + edx], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_1_36D5:
        mov    word ptr [ebp], 0
loc_1_36DB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_36F0:
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3700:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3710:
        mov    al, 1
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3720:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        test   esi, esi
        je     SHORT loc_1_3730
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3730:
        mov    eax, dword ptr [esp + 14h]
        mov    ebx, dword ptr [esp + 10h]
        push   eax
        call   ebx
        mov    bl, al
        test   esi, esi
        je     SHORT loc_1_3747
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3747:
        mov    al, bl
        pop    esi
        pop    ebx
        ret    0ch
        db  08Bh, 0FFh
sub_1_3750:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        jne    SHORT loc_1_375C
        xor    eax, eax
        jmp    SHORT loc_1_3767
loc_1_375C:
        mov    ecx, 0ffffd8f0h
        mov    eax, dword ptr [eax]
        sub    edx, edx
        div    ecx
loc_1_3767:
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   eax
        call   sub_1_73D0
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        ret    8
        db  08Bh, 0FFh
loc_1_3780:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 4], 0
        jne    SHORT loc_1_37BA
        mov    eax, dword ptr [esp + 0ch]
        test   eax, eax
        je     SHORT loc_1_37A9
        push   esi
        mov    byte ptr [esi], 0
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_37A2:
        mov    dword ptr [esi + 4], eax
        pop    esi
        ret    8
loc_1_37A9:
        push   esi
        mov    byte ptr [esi], 0ffh
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_A2F0
        mov    dword ptr [esi + 4], eax
loc_1_37BA:
        pop    esi
        ret    8
        db  08Bh, 0FFh
sub_1_37C0:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        test   esi, esi
        je     SHORT loc_1_37D0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_37D0:
        mov    eax, dword ptr [esp + 14h]
        mov    ebx, dword ptr [esp + 10h]
        push   eax
        call   ebx
        mov    bl, al
        test   esi, esi
        je     SHORT loc_1_37E7
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_37E7:
        mov    al, bl
        pop    esi
        pop    ebx
        ret    0ch
        db  08Bh, 0FFh
loc_1_37F0:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 184h]
        lea    ecx, [esi + 90h]
        push   eax
        push   ecx
        call   NDIS_Service_12
        push   0
        push   0
        push   esi
        add    esi, 0b8h
        push   esi
        call   sub_1_88C0
        pop    esi
        ret    10h
        db  08Dh, 049h, 000h
loc_1_3820:
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esp + 8]
loc_1_382B:
        cmp    dword ptr [ecx], edx
        je     SHORT loc_1_383B
        add    ecx, 0ch
        inc    eax
        db  081h, 0F9h, 078h, 000h, 000h, 000h
        jb     SHORT loc_1_382B
loc_1_383B:
        cmp    eax, 0ah
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        push   edx
        jb     SHORT loc_1_3857
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 1ch]
        push   ecx
        call   dword ptr [eax + 50h]
        ret    0ch
loc_1_3857:
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [ecx + 14h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3864:
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3870:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 10h]
        push   edi
        mov    esi, dword ptr [esp + 18h]
        test   esi, esi
        je     SHORT loc_1_38A8
        mov    edi, dword ptr [esp + 10h]
loc_1_3883:
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [edi + 10h]
        push   eax
        push   ecx
        call   dword ptr [edi + 3ch]
        cmp    eax, 103h
        je     SHORT loc_1_38A2
        push   eax
        mov    ecx, dword ptr [edi + 24h]
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        mov    eax, dword ptr [edi + 1ch]
        call   dword ptr [eax + 44h]
loc_1_38A2:
        add    ebx, 4
        dec    esi
        jne    SHORT loc_1_3883
loc_1_38A8:
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  08Bh, 0FFh
loc_1_38B0:
        mov    eax, 0c001000dh
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_38C0:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 10h]
        mov    edi, dword ptr [esp + 18h]
        test   edi, edi
        je     SHORT loc_1_38EE
        mov    ebx, dword ptr [esp + 14h]
loc_1_38D6:
        mov    eax, dword ptr [ebx]
        push   0c001000dh
        push   eax
        mov    ecx, dword ptr [esi + 10h]
        push   ecx
        mov    eax, dword ptr [esi + 8]
        add    ebx, 4
        call   dword ptr [eax + 44h]
        dec    edi
        jne    SHORT loc_1_38D6
loc_1_38EE:
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 028h, 000h, 004h, 000h, 000h, 000h, 04Eh, 044h, 049h, 053h
        db  020h, 020h, 020h, 020h, 000h, 020h, 000h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 068h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_3CD0:
        dec    esi
        db  065h, 074h, 077h
        outsd
        jb     SHORT loc_1_3D42
        and    byte ptr [edx + esi*2 + 69h], al
        jbe    SHORT loc_1_3D42
        jb     SHORT loc_1_3D52
        add    byte ptr [ebp + 76h], al
        inc    esp
        imul   esi, dword ptr [edx], 3000190h
        add    byte ptr [0c2000700h], al
        add    dh, al
        add    dl, cl
        add    dh, cl
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_3D42:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_3D52:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
_LTEXT ends
        end
