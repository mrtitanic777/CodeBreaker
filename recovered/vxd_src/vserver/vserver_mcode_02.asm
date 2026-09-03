; vserver_mcode_02.asm -- VSERVER object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  01Ah, 000h, 000h, 000h
loc_2_0004:
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  000h
loc_2_000E:
        and    eax, dword ptr [eax]
loc_2_0010:
        cmp    al, 0
loc_2_0012:
        inc    esi
        db  000h
loc_2_0014:
        dec    ebp
        db  000h
loc_2_0016:
        push   esp
        db  000h
loc_2_0018:
        outsd
        db  000h
loc_2_001A:
        ja     SHORT loc_2_001C
loc_2_001C:
        jg     SHORT loc_2_001E
loc_2_001E:
        xchg   edi, eax
        db  000h
loc_2_0020:
        wait
        db  000h
loc_2_0022:
        lahf
        db  000h
loc_2_0024:
        mov    bh, 0
        db  0C6h, 000h
loc_2_0028:
        into
        add    dl, dh
        db  000h
loc_2_002C:
        add    dword ptr [ecx], eax
loc_2_002E:
        pop    es
        db  001h
loc_2_0030:
        daa
        db  001h
loc_2_0032:
        cmp    dword ptr [ecx], eax
loc_2_0034:
        inc    eax
        db  001h
loc_2_0036:
        popad
        add    dword ptr [ebx + 1], ebp
loc_2_003A:
        jbe    SHORT loc_2_003D
        lodsd
loc_2_003D:
        add    eax, ecx
        add    esp, esp
        add    dword ptr [eax], eax
        add    byte ptr [edx + 65h], dl
        insd
        outsd
        je     SHORT loc_2_00AF
        and    byte ptr [ecx + 6eh], cl
        db  074h, 065h
        jb     SHORT loc_2_0071
        push   eax
        db  072h, 06Fh
        arpl   word ptr [ebp + 73h], sp
        db  073h, 020h
        inc    ebx
        outsd
        insd
        insd
        db  075h, 06Eh
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        add    byte ptr [ebp + 69h], cl
        arpl   word ptr [edx + 6fh], si
        jae    SHORT loc_2_00DD
        db  066h, 074h, 020h
loc_2_0071:
        dec    esi
        db  065h, 074h, 077h
        outsd
        jb     SHORT loc_2_00E3
        and    byte ptr [ebx + 65h], dl
        db  072h, 076h
        jb     NEAR PTR loc_2_0080
loc_2_0080:
        dec    ebp
        push   ebx
        dec    esi
        push   ebx
        jb     NEAR PTR loc_2_00FD
        jb     NEAR PTR loc_2_008A
loc_2_008A:
        dec    ebp
        insd
        outsd
        db  072h, 079h
        add    byte ptr [edx + 42h], al
        db  079h, 074h
        jae    NEAR PTR loc_2_0098
loc_2_0098:
        dec    ebp
        insd
        outsd
        db  072h, 079h
        and    byte ptr [ebp + 73h], dh
        and    byte ptr fs:[edx + 79h], ah
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
        db  072h, 076h
loc_2_00AF:
        jb     NEAR PTR loc_2_00E0
        add    byte ptr [edx + 75h], al
        jb     NEAR PTR loc_2_012D
        add    byte ptr [edx + 75h], al
        jb     NEAR PTR loc_2_0135
        add    byte ptr [ebx + 65h], dl
        jb     SHORT loc_2_013D
        jb     NEAR PTR loc_2_00EA
        db  077h, 06Fh
        db  072h, 06Bh
        imul   ebp, dword ptr [esi + 67h], 66756220h
        jb     NEAR PTR loc_2_014C
        add    byte ptr cs:[esi + 42h], cl
loc_2_00DD:
        jae    SHORT loc_2_00DF
loc_2_00DF:
        dec    esi
loc_2_00E0:
        inc    edx
        jae    SHORT loc_2_00E3
loc_2_00E3:
        push   ebx
        jb     NEAR PTR loc_2_015D
        jb     NEAR PTR loc_2_010A
loc_2_00EA:
        dec    esi
        je     NEAR PTR loc_2_0165
        outsd
        db  072h, 06Bh
        and    byte ptr [edx + 75h], al
        db  066h, 066h, 065h, 072h, 073h
        add    byte ptr cs:[ebx + 65h], dl
loc_2_00FD:
        db  072h, 076h
        jb     NEAR PTR loc_2_0122
        push   esp
        push   64616572h
        jae    SHORT loc_2_010A
loc_2_010A:
        push   esp
        push   64616572h
        jae    SHORT loc_2_0112
loc_2_0112:
        push   edi
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        je     SHORT loc_2_0184
        db  072h, 065h
        popad
        db  064h, 073h, 020h
loc_2_0122:
        jne    SHORT loc_2_0197
        and    byte ptr fs:[edx + 79h], ah
        and    byte ptr [eax + ebp*2 + 65h], dh
loc_2_012D:
        and    byte ptr [ebx + 65h], dh
        db  072h, 076h
        jb     NEAR PTR loc_2_0163
loc_2_0135:
        add    byte ptr [edx + 79h], al
        je     SHORT loc_2_019F
        db  073h, 020h
        push   edx
loc_2_013D:
        popad
        das
        db  073h, 065h
        arpl   word ptr [eax], ax
        inc    edx
        db  079h, 074h
        jae    NEAR PTR loc_2_014B
loc_2_014B:
        inc    esp
loc_2_014C:
        imul   esi, dword ptr [ebx + 6bh], 61657220h
        jae    NEAR PTR loc_2_0176
        imul   ebp, dword ptr [esi + 20h], 65747962h
loc_2_015D:
        db  073h, 020h
        db  070h, 065h
        db  072h, 020h
loc_2_0163:
        db  073h, 065h
loc_2_0165:
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr cs:[edx + 79h], al
        db  074h, 065h
        db  073h, 020h
        push   edi
        db  072h, 069h
        db  074h, 074h
loc_2_0176:
        outsb
        das
        db  073h, 065h
        arpl   word ptr [eax], ax
        push   edi
        jb     SHORT loc_2_01E9
        je     SHORT loc_2_01E7
        jae    SHORT loc_2_0184
loc_2_0184:
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 69727720h
        je     SHORT loc_2_01F3
        jae    SHORT loc_2_01B0
        imul   ebp, dword ptr [esi + 20h], 65747962h
loc_2_0197:
        db  073h, 020h
        db  070h, 065h
        jb     SHORT loc_2_01BD
        jae    SHORT loc_2_0204
loc_2_019F:
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr cs:[edx + 79h], al
        je     SHORT loc_2_020E
        jae    SHORT loc_2_01DA
        db  073h, 065h
        arpl   word ptr [eax], ax
        push   esp
loc_2_01B0:
        push   67756f72h
        push   747570h
        push   ebx
        db  075h, 06Dh
loc_2_01BD:
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ecx + ebp*2 + 73h], ah
        imul   esp, dword ptr [eax], 72h
        popad
        db  064h, 073h, 020h
        popad
        outsb
        and    byte ptr fs:[ecx + ebp*2 + 73h], ah
        imul   esp, dword ptr [eax], 77h
        db  072h, 069h
        db  074h, 065h
loc_2_01DA:
        jae    SHORT loc_2_01FC
        imul   ebp, dword ptr [esi + 20h], 65747962h
        db  073h, 020h
        db  070h, 065h
loc_2_01E7:
        db  072h, 020h
loc_2_01E9:
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr cs:[ebp + 69h], cl
loc_2_01F3:
        arpl   word ptr [edx + 6fh], si
        db  073h, 06Fh
        db  066h, 074h, 020h
        push   edi
loc_2_01FC:
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        push   esi
loc_2_0204:
        db  065h, 072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 694d0020h
loc_2_020E:
        arpl   word ptr [edx + 6fh], si
        db  073h, 06Fh
        db  066h, 074h, 020h
        push   ebx
        dec    ebp
        inc    edx
        and    byte ptr [eax + 65h], dl
        db  065h, 072h, 020h
        push   ebx
        db  065h, 072h, 076h
        db  069h, 063h, 065h, 073h, 000h
_LMSGTABLE ends
        end
