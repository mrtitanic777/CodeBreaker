; vxdldr_mcode_02.asm -- VXDLDR object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

loc_2_0000:
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_2_0004:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
loc_2_000A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0
        das
        add    byte ptr [eax], al
        push   0a7007e00h
        add    al, al
        add    cl, dl
        add    dh, ch
        add    byte ptr [esi], cl
        add    dword ptr [5a013f01h], esp
        add    dword ptr [ecx + eax - 72h], esi
        add    dword ptr [eax + 4f01c201h], ebp
        db  075h, 074h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        jb     SHORT loc_2_00B6
        sub    al, 31h
        xor    ebp, dword ptr [ecx + esi]
        xor    byte ptr [eax], al
        inc    ecx
        db  074h, 074h
        insd
        jo     SHORT loc_2_00BF
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  072h, 065h
        outsb
        db  074h, 065h
        jb     SHORT loc_2_0079
        inc    esp
        dec    edi
        push   ebx
        or     eax, 6946000ah
        insb
        and    byte ptr gs:[esi + 6fh], ch
        je     SHORT loc_2_0088
        outsw
        db  075h, 06Eh
        or     eax, 7245000ah
        jb     SHORT loc_2_00E3
        db  072h, 020h
        db  072h, 065h
        popad
loc_2_0079:
        imul   ebp, dword ptr fs:[esi + 67h], 6c696620h
        and    byte ptr gs:[736f7020h], ch
loc_2_0088:
        db  073h, 069h
        bound  ebp, qword ptr [ecx + edi*2 + 20h]
        arpl   word ptr [edi + 72h], bp
        jb     SHORT loc_2_0108
        db  070h, 074h
        or     eax, 7544000ah
        jo     SHORT loc_2_0108
        imul   esp, dword ptr [ebx + 61h], 64206574h
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
        dec    ecx
        inc    esp
        or     eax, 6f4e000ah
        je     SHORT loc_2_00D2
        popad
        and    byte ptr [esi + 61h], dh
loc_2_00B6:
        insb
        imul   esp, dword ptr [eax + 64h], 63697665h
loc_2_00BF:
        and    byte ptr gs:[eax], ah
        sub    eax, 736f7020h
        db  073h, 069h
        bound  ebp, qword ptr [ecx + edi*2 + 20h]
        arpl   word ptr [edi + 72h], bp
        db  072h, 075h
loc_2_00D2:
        db  070h, 074h
        or     eax, 6544000ah
        db  076h, 069h
        arpl   word ptr [ebp + 20h], sp
        db  072h, 065h
        db  066h, 075h, 073h
loc_2_00E3:
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        insb
        outsd
        popad
        or     eax, 6f4e000ah
        and    byte ptr [ebx + 75h], dh
        arpl   word ptr [eax + 20h], bp
        jbe    NEAR PTR loc_2_0165
        arpl   word ptr [ebp + 0dh], sp
        or     al, byte ptr [eax]
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
loc_2_0108:
        arpl   word ptr [ecx + 6eh], sp
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        bound  esp, qword ptr [ebp + 20h]
        db  075h, 06Eh
        insb
        outsd
        popad
        or     eax, 6e55000ah
        popad
        bound  ebp, qword ptr [ebp + 20h]
        je     SHORT loc_2_0196
        and    byte ptr [ecx + 6ch], ah
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[esi + 38h], dl
        and    byte ptr ss:[edx + 75h], ah
        db  066h, 066h, 065h, 072h, 00Dh
        or     al, byte ptr [eax]
        dec    ecx
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 41h], 66204950h
        jne    SHORT loc_2_01BB
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        or     eax, 7944000ah
        outsb
        popad
        insd
        imul   esp, dword ptr [ebx + 20h], 64616f6ch
        and    byte ptr [ebp + 76h], ah
loc_2_0165:
        imul   esp, dword ptr [ebx + 65h], 73252020h
        or     eax, 7944000ah
        outsb
        popad
        insd
        imul   esp, dword ptr [ebx + 20h], 64616f6ch
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 73252020h
        and    byte ptr [edx], bh
        and    byte ptr [eax], al
        inc    esp
        db  079h, 06Eh
        popad
        insd
        imul   esp, dword ptr [ebx + 20h], 64616f6ch
loc_2_0196:
        and    byte ptr [ebx + 75h], dh
        arpl   word ptr [ebx + 65h], sp
        db  073h, 073h
        and    byte ptr [0a0d73h], ah
        inc    esp
        db  079h, 06Eh
        popad
        insd
        imul   esp, dword ptr [ebx + 20h], 74696e69h
        and    byte ptr [ebp + 76h], ah
        imul   esp, dword ptr [ebx + 65h], 73252020h
loc_2_01BB:
        or     eax, 7944000ah
        outsb
        popad
        insd
        imul   esp, dword ptr [ebx + 20h], 74696e69h
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 73252020h
        or     eax, 7944000ah
        outsb
        popad
        insd
        imul   esp, dword ptr [ebx + 20h], 74696e69h
        and    byte ptr [ebx + 75h], dh
        arpl   word ptr [ebx + 65h], sp
        db  073h, 073h
        and    byte ptr [0a0d73h], ah
_LMSGTABLE ends
        end
