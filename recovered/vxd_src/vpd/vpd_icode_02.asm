; vpd_icode_02.asm -- VPD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        pop    es
        add    byte ptr [eax], al
        db  000h, 027h
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    eax, dword ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        hlt
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 4], bh
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
loc_2_0034:
        push   ebx
        inc    edi
        push   edx
        inc    ecx
        inc    edx
        dec    esp
        push   eax
        push   esp
        db  000h
loc_2_003D:
        dec    esp
        push   eax
        push   esp
loc_2_0040:
        js     SHORT loc_2_0083
        push   ebp
        push   esp
        dec    edi
        inc    ecx
        push   ebx
        push   ebx
        dec    ecx
        inc    edi
        dec    esi
        add    byte ptr [eax + edx*2 + 54h], cl
loc_2_004F:
        db  078h, 044h
        dec    ecx
        push   ebx
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        add    byte ptr [eax + edx*2 + 54h], cl
        push   ebx
        db  077h, 069h
        db  074h, 063h
        push   656d6954h
        outsd
        db  075h, 074h
        add    byte ptr [edi + 50h], dl
        push   ebx
        dec    esp
        push   eax
        push   esp
loc_2_006F:
        js     SHORT loc_2_0071
loc_2_0071:
        dec    esi
        dec    edi
        dec    esi
        dec    edx
        push   ebp
        dec    ebp
        inc    edx
        dec    edi
        dec    esp
        push   eax
        push   esp
loc_2_007C:
        js     SHORT loc_2_007E
loc_2_007E:
        push   eax
        push   edx
        dec    esi
        and    byte ptr [eax], ah
loc_2_0083:
        and    byte ptr [eax], ah
        db  020h
_ITEXT ends
        end
