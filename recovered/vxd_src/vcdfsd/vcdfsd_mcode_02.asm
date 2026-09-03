; vcdfsd_mcode_02.asm -- VCDFSD object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  00Ah, 000h, 000h, 000h
loc_2_0004:
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        adc    al, byte ptr [eax]
        inc    esi
        add    byte ptr [esi], bl
        jl     SHORT loc_2_0016
loc_2_0016:
        pushfd
        add    byte ptr [esi - 31ff3c00h], bh
        add    al, bl
        add    ah, ah
        add    byte ptr [eax], al
        add    byte ptr [ebx + 44h], al
        push   edx
        xor    dword ptr [eax], esi
        xor    dword ptr [edx], edi
        and    byte ptr [esi + 6fh], cl
        db  074h, 020h
        db  072h, 065h
        popad
        jns    NEAR PTR loc_2_0036
loc_2_0036:
        inc    ebx
        inc    esp
        push   edx
        xor    dword ptr [eax], esi
        xor    edi, dword ptr [edx]
        and    byte ptr [eax + ebp*2 + 69h], dl
        db  073h, 020h
        inc    ebx
        inc    esp
        push   edx
        dec    edi
        dec    ebp
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [edi + 72h], bp
        and    byte ptr fs:[ecx + 6eh], ch
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr [ebp + 6eh], dh
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_2_00D4
        popad
        je     SHORT loc_2_006A
loc_2_006A:
        inc    ebx
        inc    esp
        sub    eax, 204d4f52h
        push   eax
        jb     NEAR PTR loc_2_00DB
        outsd
        jb     SHORT loc_2_00E5
        popad
        outsb
        arpl   word ptr [ebp + 20h], sp
        inc    esp
        popad
        je     SHORT loc_2_00E2
        add    byte ptr [edx + 79h], al
        db  074h, 065h
        jae    SHORT loc_2_00A8
        push   edx
        popad
        and    byte ptr fs:[eax], ch
        inc    edx
        jns    SHORT loc_2_0105
        jae    NEAR PTR loc_2_00B4
        push   eax
        jb     NEAR PTR loc_2_00B8
        push   ebx
        arpl   word ptr gs:[edi + 6eh], bp
        sub    dword ptr fs:[eax], eax
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        and    byte ptr [eax + 69h], cl
loc_2_00A8:
        db  074h, 073h
        and    byte ptr [eax], ch
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        jae    SHORT loc_2_00D4
loc_2_00B4:
        push   eax
        db  065h, 072h, 020h
loc_2_00B8:
        push   ebx
        arpl   word ptr gs:[edi + 6eh], bp
        sub    dword ptr fs:[eax], eax
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        and    byte ptr [ebp + 69h], cl
        db  073h, 073h
        db  065h, 073h, 020h
        sub    byte ptr [ebx + 65h], dl
        arpl   word ptr [edi + ebp*2 + 72h], si
loc_2_00D4:
        db  073h, 020h
        push   eax
        db  065h, 072h, 020h
        push   ebx
loc_2_00DB:
        arpl   word ptr gs:[edi + 6eh], bp
        sub    dword ptr fs:[eax], eax
loc_2_00E2:
        inc    ebx
        inc    esp
        push   edx
loc_2_00E5:
        dec    edi
        dec    ebp
        add    byte ptr [edx + 79h], al
        db  074h, 065h
        db  073h, 052h
        popad
        add    byte ptr fs:[ebx + 61h], al
        arpl   word ptr [eax + 65h], bp
        dec    eax
        imul   esi, dword ptr [ebx + esi*2], 68636143h
        dec    ebp
        db  069h, 073h, 073h
loc_2_0105:
        db  065h, 073h, 000h
_LMSGTABLE ends
        end
