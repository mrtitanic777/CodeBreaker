; vfat_mcode_02.asm -- VFAT object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

loc_2_0000:
        sub    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_2_0004:
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx], dl
        add    byte ptr [ebx], dl
loc_2_0012:
        out    dx, eax
        db  000h
loc_2_0014:
        adc    al, byte ptr [ecx]
loc_2_0016:
        db  07Bh, 001h
loc_2_0018:
        mov    byte ptr [ecx], al
loc_2_001A:
        mov    es, word ptr [ecx]
loc_2_001C:
        popfd
        db  001h
loc_2_001E:
        test   al, 1
loc_2_0020:
        in     eax, 1
        db  0F3h, 001h
loc_2_0024:
        std
        db  001h
loc_2_0026:
        cmp    dword ptr [edx], eax
loc_2_0028:
        dec    esp
        db  002h
loc_2_002A:
        push   edi
        db  002h, 083h, 002h
loc_2_002E:
        cdq
        db  002h
loc_2_0030:
        movsd
        db  002h
loc_2_0032:
        rol    byte ptr [edx], cl
loc_2_0034:
        fiadd  word ptr [edx]
loc_2_0036:
        db  0E9h, 002h, 0AFh, 003h, 0C2h
        db  003h, 0D1h, 003h, 0F0h, 003h, 006h, 004h, 013h, 004h, 035h, 004h, 046h, 004h, 055h, 004h, 074h
        db  004h, 092h, 004h, 0A1h, 004h, 0CAh, 004h, 0EAh, 004h, 0F8h, 004h, 021h, 005h, 046h, 005h, 053h
        db  005h, 093h, 005h, 0A4h, 005h, 0BAh, 005h, 0D8h, 005h
loc_2_0064:
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 69725720h
        db  074h, 065h
        and    byte ptr [ebp + 72h], al
        db  072h, 06Fh
        jb     SHORT loc_2_0075
loc_2_0075:
        add    byte ptr [ebp + 6eh], dl
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [edi + 72h], dh
        imul   esi, dword ptr [ebp + 20h], 64206f74h
        imul   esi, dword ptr [ebx + 6bh], 206e6920h
        db  064h, 072h, 069h
        jbe    SHORT loc_2_00FB
        and    byte ptr [440a0d73h], ah
        popad
        db  074h, 061h
        and    byte ptr [edi + 72h], ch
        and    byte ptr [esi + 69h], ah
        insb
        db  065h, 073h, 020h
        insd
        popad
        db  079h, 020h
        bound  esp, qword ptr [ebp + 20h]
        insb
        outsd
        db  073h, 074h
        add    byte ptr cs:[eax], al
        push   esi
        inc    esi
        inc    ecx
        push   esp
        and    byte ptr [esi], ah
        and    byte ptr [edi + ecx*2 + 53h], al
        and    byte ptr [esi + 6fh], dh
        insb
        db  075h, 06Dh
        and    byte ptr gs:[ecx + 6eh], ch
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        and    byte ptr [616c2073h], ah
        jns    SHORT loc_2_0148
        db  075h, 074h
        db  073h, 020h
        popad
        db  072h, 065h
        and    byte ptr [ebp + 49h], cl
        push   ebx
        dec    ebp
        inc    ecx
        push   esp
        inc    ebx
        dec    eax
        inc    ebp
        inc    esp
        or     eax, 6e61430ah
        outsb
        outsd
        db  074h, 020h
        db  070h, 072h
        outsd
        arpl   word ptr [ebp + 65h], sp
loc_2_00FB:
        and    byte ptr fs:[esi + 75h], ah
        db  072h, 074h
        push   202e7265h
        and    byte ptr [edi + ebp*2 + 69h], al
        outsb
        and    byte ptr [bp + di + 6fh], dh
        and    byte ptr [ebp + 61h], ch
        db  079h, 020h
        db  064h, 065h, 073h, 074h
        db  072h, 06Fh
        db  079h, 020h
        imul   esi, dword ptr fs:[ebx + 6bh], 74616420h
        popad
        or     eax, 656c500ah
        popad
        db  073h, 065h
        and    byte ptr [edx + 65h], dh
        db  070h, 06Fh
        jb     SHORT loc_2_01A9
        and    byte ptr [eax + ebp*2 + 69h], dh
        jae    SHORT loc_2_015B
        arpl   word ptr [edi + 6eh], bp
        imul   esi, dword ptr fs:[ecx + ebp*2 + 6fh], 6d69206eh
        insd
loc_2_0148:
        imul   esp, dword ptr fs:[ecx + 74h], 2e796c65h
        add    byte ptr [eax], al
        push   esi
        inc    esi
        inc    ecx
        push   esp
        and    byte ptr [ebp + 76h], al
loc_2_015B:
        imul   esp, dword ptr [ebx + 65h], 696e4920h
        db  074h, 069h
        popad
        insb
        imul   edi, dword ptr [edx + 61h], 6e6f6974h
        and    byte ptr [esi + 61h], al
        imul   ebp, dword ptr [ebp + 64h], 20410000h
        db  064h, 065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
        outsd
        db  072h, 020h
        db  072h, 065h
        jae    SHORT loc_2_01F5
        db  075h, 072h
        arpl   word ptr [ebp + 20h], sp
        jb     SHORT loc_2_01F2
        jno    SHORT loc_2_0204
        imul   esi, dword ptr [edx + 65h], 79622064h
        and    byte ptr [esi + 46h], dl
        inc    ecx
        push   esp
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [esi + 6fh], ch
        je     SHORT loc_2_01C3
        db  070h, 072h
        db  065h, 073h, 065h
        outsb
loc_2_01A9:
        db  074h, 00Dh
        or     ch, byte ptr [edi + 72h]
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ebp + 6eh], dh
        popad
        jbe    SHORT loc_2_0218
        imul   ebp, dword ptr [ecx + 62h], 202e656ch
        and    byte ptr [esi + 46h], dl
        inc    ecx
loc_2_01C3:
        push   esp
        and    byte ptr [ebx + 61h], ah
        outsb
        outsb
        outsd
        je     SHORT loc_2_01EC
        arpl   word ptr [edi + 6eh], bp
        je     SHORT loc_2_023A
        outsb
        db  075h, 065h
        and    byte ptr [edi + ebp*2 + 61h], ch
        imul   ebp, dword ptr fs:[esi + 67h], 4600002eh
        imul   ebp, dword ptr [ebp + 20h], 74737953h
        insd
        add    byte ptr [eax], al
loc_2_01EC:
        push   esi
        inc    esi
        inc    ecx
        push   esp
        add    byte ptr [eax], al
loc_2_01F2:
        push   edi
        db  072h, 069h
loc_2_01F5:
        db  074h, 065h
        db  073h, 02Fh
        jae    SHORT loc_2_0260
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[eax], al
        push   edi
        jb     SHORT loc_2_026D
loc_2_0204:
        db  074h, 065h
        db  073h, 053h
        arpl   word ptr gs:[eax], ax
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edi + 72h], dh
loc_2_0218:
        imul   esi, dword ptr [ebp + 20h], 75716572h
        jae    NEAR PTR loc_2_0297
        db  073h, 020h
        insd
        popad
        and    byte ptr gs:[edi + ebp*2 + 20h], dh
        je     SHORT loc_2_0297
        and    byte ptr gs:[esi + 69h], ah
        insb
        and    byte ptr gs:[ebx + 79h], dh
        db  073h, 074h
loc_2_023A:
        insd
        and    byte ptr [eax + 65h], dh
        db  072h, 020h
        jae    SHORT loc_2_02A8
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[eax], al
        push   edx
        popad
        db  064h, 073h, 02Fh
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[eax], al
        push   edx
        popad
        jae    NEAR PTR loc_2_02B0
        arpl   word ptr gs:[eax], ax
loc_2_0260:
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 65h], dh
loc_2_026D:
        popad
        and    byte ptr fs:[edx + 65h], dh
        db  071h, 075h
        db  065h, 073h, 074h
        db  073h, 020h
        insd
        popad
        and    byte ptr gs:[edi + ebp*2 + 20h], dh
        db  074h, 068h
        and    byte ptr gs:[esi + 69h], ah
        insb
        and    byte ptr gs:[ebx + 79h], dh
        db  073h, 074h
        insd
        and    byte ptr [eax + 65h], dh
        db  072h, 020h
        db  073h, 065h
loc_2_0297:
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[eax], al
        inc    edx
        db  079h, 074h
        jae    NEAR PTR loc_2_02C3
        db  072h, 065h
        popad
        das
loc_2_02A8:
        jae    SHORT loc_2_030F
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[eax], al
loc_2_02B0:
        inc    edx
        push   edx
        popad
        db  064h, 073h, 053h
        arpl   word ptr gs:[eax], ax
        add    byte ptr [edx + 79h], al
        db  074h, 065h
        jae    SHORT loc_2_02E1
        db  072h, 065h
loc_2_02C3:
        popad
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr gs:[ebx + 79h], dh
        db  073h, 074h
        insd
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_2_02FF
        db  073h, 065h
loc_2_02E1:
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[eax], al
        inc    edx
        db  079h, 074h
        jae    NEAR PTR loc_2_030D
        ja     SHORT loc_2_0361
        imul   esi, dword ptr [esp + esi*2 + 65h], 65732f6eh
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[eax], al
        inc    edx
        push   edi
loc_2_02FF:
        db  072h, 069h
        db  074h, 065h
        db  073h, 053h
        arpl   word ptr gs:[eax], ax
        add    byte ptr [edx + 79h], al
        db  074h, 065h
loc_2_030D:
        db  073h, 020h
loc_2_030F:
        db  077h, 072h
        imul   esi, dword ptr [esp + esi*2 + 65h], 6f74206eh
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr gs:[ebx + 79h], dh
        jae    SHORT loc_2_039B
        insd
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_2_034E
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[eax], al
        inc    esp
        imul   esi, dword ptr [edx + 74h], 61642079h
        je     SHORT loc_2_03A1
        add    byte ptr [eax], al
        inc    esp
        imul   esi, dword ptr [edx + 74h], 74614479h
        popad
        add    byte ptr [eax], al
        push   esp
loc_2_034E:
        push   756e2065h
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 79h], ah
        db  074h, 065h
        jae    SHORT loc_2_0381
loc_2_0361:
        db  077h, 061h
        imul   esi, dword ptr [ecx + ebp*2 + 6eh], 6f742067h
        and    byte ptr [edx + 65h], ah
        and    byte ptr [edi + 72h], dh
        imul   esi, dword ptr [esp + esi*2 + 65h], 6f74206eh
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ecx + ebp*2 + 73h], ah
loc_2_0381:
        imul   ebp, dword ptr [esi], 20h
        and    byte ptr [esi + 6fh], cl
        db  074h, 065h
        and    byte ptr [eax + ebp*2 + 61h], dh
        db  074h, 020h
        or     eax, 7269640ah
        db  074h, 079h
        and    byte ptr [ecx + 74h], ah
        popad
loc_2_039B:
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ebx + 65h], ch
loc_2_03A1:
        jo     SHORT loc_2_0417
        and    byte ptr [edi + 6eh], ch
        and    byte ptr [ecx + 20h], ah
        db  070h, 065h
        db  072h, 020h
        arpl   word ptr [ecx + 63h], sp
        push   6c622065h
        outsd
        arpl   word ptr [ebx + 20h], bp
        bound  esp, qword ptr [ecx + 73h]
        imul   esi, dword ptr [ebx + 2ch], 646e6120h
        and    byte ptr [ebx + 6fh], dh
        and    byte ptr [ecx + 20h], ah
        outsb
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [6769680ah], cl
        push   74207265h
        push   74206e61h
        push   63612065h
        je     SHORT loc_2_045B
        popad
        insb
        and    byte ptr [esi + 75h], ch
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 79h], ah
        db  074h, 065h
        db  073h, 020h
        ja     SHORT loc_2_046D
        imul   esi, dword ptr [esp + esi*2 + 65h], 616d206eh
        db  079h, 020h
        bound  esp, qword ptr [ebp + 20h]
        jb     SHORT loc_2_046F
        db  070h, 06Fh
        db  072h, 074h
        add    byte ptr cs:[eax], al
        and    esp, dword ptr [eax]
        dec    esp
        outsd
loc_2_0417:
        popad
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        inc    edx
        insb
        outsd
        arpl   word ptr [ebx + 73h], bp
        add    byte ptr [eax], al
        dec    esp
        outsd
        popad
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        push   ebx
        imul   edi, dword ptr [edx + 65h], 754e0000h
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 73h], bp
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [edi + ebp*2 + 61h], ch
        and    byte ptr fs:[ebx + 61h], ah
        arpl   word ptr [eax + 65h], bp
        add    byte ptr [ebx], ah
        and    byte ptr [ebp + 6dh], al
        popad
        outsb
loc_2_045B:
        and    byte ptr fs:[edi + ebp*2 + 61h], cl
        and    byte ptr fs:[esi + 61h], al
        db  075h, 06Ch
        db  074h, 073h
        add    byte ptr [eax], al
        inc    esp
        insd
loc_2_046D:
        popad
        outsb
loc_2_046F:
        dec    esp
        outsd
        popad
        jae    NEAR PTR loc_2_0476
loc_2_0476:
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ebp + 6dh], al
        popad
        outsb
        and    byte ptr fs:[edi + ebp*2 + 61h], cl
        and    byte ptr fs:[eax + 61h], dl
        and    byte ptr gs:[bp + 61h], al
        jne    SHORT loc_2_0502
        je     SHORT loc_2_050B
        add    byte ptr [ebx], ah
        and    byte ptr [edi + ebp*2 + 61h], cl
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        dec    eax
        imul   esi, dword ptr [ebx + esi*2], 616f4c00h
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        dec    eax
        imul   esi, dword ptr [ebx + esi*2], 6d754e00h
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 73h], bp
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [edi + ebp*2 + 61h], ch
        and    byte ptr fs:[ebx + 61h], ah
        arpl   word ptr [eax + 65h], bp
        add    byte ptr [ebx], ah
        and    byte ptr [ebp + 6dh], al
        popad
        outsb
        dec    esp
        outsd
        popad
        push   edx
        popad
        db  064h, 073h, 020h
        sub    byte ptr [ecx + 6eh], ch
        and    byte ptr [eax + 61h], dl
        db  067h, 065h, 073h, 029h
        add    byte ptr [eax], al
        dec    esp
        outsd
        popad
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        push   edx
        popad
loc_2_0502:
        add    byte ptr fs:[eax], al
        dec    esi
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
loc_2_050B:
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + 61h], dl
        jae    NEAR PTR loc_2_0535
        push   edx
        popad
        and    byte ptr fs:[ecx + 6eh], ch
        and    byte ptr [edx + 79h], ah
        and    byte ptr [ebp + 6dh], al
        popad
        outsb
        and    byte ptr fs:[edi + ebp*2 + 61h], cl
        jb     NEAR PTR loc_2_052E
loc_2_052E:
        and    esp, dword ptr [eax]
        dec    esp
        outsd
        popad
        inc    ebx
loc_2_0535:
        popad
        arpl   word ptr [eax + 65h], bp
        inc    esp
        imul   esi, dword ptr [ebx + 63h], 73647261h
        and    byte ptr [eax], ch
        imul   ebp, dword ptr [esi + 20h], 65676150h
        db  073h, 029h
        add    byte ptr [eax], al
        dec    esp
        outsd
        popad
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        inc    esp
        jae    SHORT loc_2_05BD
        add    byte ptr [eax], al
        dec    esi
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + 61h], dl
        jae    NEAR PTR loc_2_058C
        inc    esp
        imul   esi, dword ptr [ebx + 63h], 65647261h
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [edi + ebp*2 + 61h], cl
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        add    byte ptr [ebx], ah
        and    byte ptr [edi + ebp*2 + 61h], cl
        inc    ebx
loc_2_058C:
        popad
        arpl   word ptr [eax + 65h], bp
        inc    ebx
        insb
        outsd
        db  073h, 065h
        inc    esp
        imul   esi, dword ptr [ebx + 63h], 73647261h
        and    byte ptr [eax], ch
        imul   ebp, dword ptr [esi + 20h], 65676150h
        db  073h, 029h
        add    byte ptr [eax], al
        dec    esp
        inc    ebx
        sub    eax, 736f6c43h
        inc    esp
        db  073h, 063h
        add    byte ptr [eax], al
        dec    esi
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
loc_2_05BD:
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + 61h], dl
        db  067h, 065h, 073h, 020h
        inc    esp
        imul   esi, dword ptr [ebx + 63h], 65647261h
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [edi + ebp*2 + 61h], cl
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        or     eax, 2074610ah
        arpl   word ptr [edi + ebp*2 + 73h], bp
        and    byte ptr gs:[esi + 69h], ah
        insb
        and    byte ptr gs:[ecx + ebp*2 + 6dh], dh
        and    byte ptr gs:[614d000ah], cl
        db  078h, 020h
        dec    esp
        outsd
        popad
        jb     NEAR PTR loc_2_0622
        push   edx
        popad
        add    byte ptr fs:[eax], al
        dec    ebp
        popad
        db  078h, 04Ch
        outsd
        popad
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        push   edx
        popad
        push   ebx
        imul   edi, dword ptr [edx + 65h], 614d0000h
        db  078h, 06Dh
loc_2_0622:
        imul   ebp, dword ptr [ebp + 61h], 6f4c206ch
        popad
        inc    ebx
        insb
        db  075h, 073h
        db  074h, 065h
        db  072h, 020h
        imul   ebp, dword ptr [esi + 20h], 65676150h
        db  073h, 000h
_LMSGTABLE ends
        end
