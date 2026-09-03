; nwserver_mcode_03.asm -- NWSERVER object 3 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  01Dh, 000h, 000h, 000h
loc_3_0004:
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  000h
loc_3_000E:
        pop    es
        db  000h
loc_3_0010:
        adc    byte ptr [eax], al
loc_3_0012:
        and    al, byte ptr [eax]
loc_3_0014:
        sub    al, 0
loc_3_0016:
        push   eax
        add    byte ptr [ebp], ah
loc_3_001A:
        jo     SHORT loc_3_001C
loc_3_001C:
        xchg   edi, eax
        db  000h
loc_3_001E:
        scasd
        db  000h, 0BDh, 000h, 0F3h, 000h
loc_3_0024:
        pop    es
        db  001h
loc_3_0026:
        pop    ss
        db  001h
loc_3_0028:
        inc    edi
        db  001h
loc_3_002A:
        push   edx
        db  001h
loc_3_002C:
        pop    esp
        db  001h
loc_3_002E:
        mov    word ptr [ecx], es
loc_3_0030:
        cdq
        db  001h, 0A2h, 001h
loc_3_0034:
        fild   word ptr [ecx]
loc_3_0036:
        out    1, eax
loc_3_0038:
        out    dx, eax
        db  001h
loc_3_003A:
        adc    eax, dword ptr [edx]
loc_3_003C:
        and    eax, dword ptr [edx]
loc_3_003E:
        das
        add    al, byte ptr [ebx - 59fd66feh]
        add    ah, dl
        add    dl, byte ptr [ebx + 65h]
        db  072h, 076h
        jb     NEAR PTR loc_3_004F
loc_3_004F:
        dec    esi
        push   edi
        push   ebx
        inc    ebp
        push   edx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [edx + 79h], al
        db  074h, 065h
        db  073h, 020h
        db  072h, 065h
        popad
        das
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 52h], al
        popad
        db  064h, 073h, 053h
        arpl   word ptr gs:[eax], ax
        inc    edx
        jns    SHORT loc_3_00EB
        jae    NEAR PTR loc_3_009A
        jb     SHORT loc_3_00E1
        popad
        and    byte ptr fs:[edx + 79h], ah
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
        db  072h, 076h
        db  065h, 072h, 020h
        db  070h, 065h
        jb     SHORT loc_3_00B1
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 79h], al
loc_3_009A:
        db  074h, 065h
        jae    SHORT loc_3_00BE
        db  077h, 072h
        imul   esi, dword ptr [esp + esi*2 + 65h], 65732f6eh
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 57h], al
        db  072h, 069h
loc_3_00B1:
        je     SHORT loc_3_0118
        db  073h, 053h
        arpl   word ptr gs:[eax], ax
        inc    edx
        db  079h, 074h
        db  065h, 073h, 020h
loc_3_00BE:
        ja     SHORT loc_3_0132
        imul   esi, dword ptr [esp + esi*2 + 65h], 7962206eh
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
        db  072h, 076h
        db  065h, 072h, 020h
        db  070h, 065h
        db  072h, 020h
        jae    SHORT loc_3_013F
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 79h], al
loc_3_00E1:
        je     SHORT loc_3_0148
        jae    SHORT loc_3_0105
        db  074h, 068h
        db  072h, 06Fh
        jne    SHORT loc_3_0152
loc_3_00EB:
        push   2f747570h
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[eax + ebp*2 + 72h], dl
        outsd
        db  075h, 067h
        push   53747570h
        arpl   word ptr gs:[eax], ax
loc_3_0105:
        push   esp
        outsd
        je     SHORT loc_3_016A
        insb
        and    byte ptr [edx + 79h], ah
        je     SHORT loc_3_0174
        db  073h, 020h
        jb     SHORT loc_3_0178
        popad
        and    byte ptr fs:[ecx + 6eh], ah
loc_3_0118:
        and    byte ptr fs:[edi + 72h], dh
        imul   esi, dword ptr [esp + esi*2 + 65h], 7962206eh
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
        db  072h, 076h
        db  065h, 072h, 020h
        jo     SHORT loc_3_0197
loc_3_0132:
        db  072h, 020h
        jae    SHORT loc_3_019B
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + esi*2 + 61h], dl
        outsb
loc_3_013F:
        jae    SHORT loc_3_01A2
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        db  073h, 02Fh
loc_3_0148:
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + esi*2 + 61h], dl
loc_3_0152:
        outsb
        jae    SHORT loc_3_01B6
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        db  073h, 053h
        arpl   word ptr gs:[eax], ax
        push   esp
        db  072h, 061h
        outsb
        db  073h, 061h
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
loc_3_016A:
        db  073h, 020h
        jo     SHORT loc_3_01E0
        outsd
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_3_01D9
loc_3_0174:
        and    byte ptr fs:[edx + 79h], ah
loc_3_0178:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
        db  072h, 076h
        jb     NEAR PTR loc_3_01A4
        db  070h, 065h
        jb     SHORT loc_3_01A8
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edi + ebp*2 + 74h], dl
        popad
        insb
        and    byte ptr [eax + 65h], cl
loc_3_0197:
        popad
        jo     SHORT loc_3_019A
loc_3_019A:
        push   esp
loc_3_019B:
        outsd
        je     SHORT loc_3_01FF
        insb
        dec    eax
        popad
loc_3_01A2:
        jo     SHORT loc_3_01A4
loc_3_01A4:
        push   esp
        outsd
        je     SHORT loc_3_0209
loc_3_01A8:
        insb
        and    byte ptr [edx + 79h], ah
        db  074h, 065h
        db  073h, 020h
        outsd
        and    byte ptr [eax + 65h], ch
        popad
loc_3_01B6:
        db  070h, 020h
        insd
        insd
        outsd
        jb     SHORT loc_3_0237
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[edx + 79h], ah
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
        db  072h, 076h
        jb     NEAR PTR loc_3_01D4
loc_3_01D4:
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
loc_3_01D9:
        and    byte ptr [ebp + 65h], cl
        insd
        outsd
        db  072h, 079h
loc_3_01E0:
        add    byte ptr [ebx + 61h], al
        arpl   word ptr [eax + 65h], bp
        dec    ebp
        insd
        add    byte ptr [edx + 79h], al
        db  074h, 065h
        jae    SHORT loc_3_0210
        outsd
        and    byte ptr [ebx + 61h], ah
        arpl   word ptr [eax + 65h], bp
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
loc_3_01FF:
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[edx + 79h], ah
loc_3_0209:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
loc_3_0210:
        db  072h, 076h
        jb     NEAR PTR loc_3_0235
        outsw
        jb     SHORT loc_3_0239
        bound  esi, qword ptr [ebp + 72h]
        db  073h, 074h
        and    byte ptr [edx + 75h], ah
        jb     NEAR PTR loc_3_0299
        add    byte ptr [eax + ebp*2 + 72h], dl
        popad
        jae    NEAR PTR loc_3_022F
loc_3_022F:
        push   esp
        push   64616572h
loc_3_0235:
        jae    SHORT loc_3_0237
loc_3_0237:
        xor    esi, dword ptr [edx]
loc_3_0239:
        and    byte ptr [edx + 69h], ah
        db  074h, 020h
        je     SHORT loc_3_02A8
        db  072h, 065h
        popad
        jae    NEAR PTR loc_3_0266
        imul   ebp, dword ptr [esi + 20h], 20657375h
        bound  edi, qword ptr [ecx + 20h]
        je     SHORT loc_3_02BA
        and    byte ptr gs:[ebx + 65h], dh
        jb     SHORT loc_3_02CE
        jb     NEAR PTR loc_3_025B
loc_3_025B:
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
        and    byte ptr [edx + esi*2 + 6fh], ah
loc_3_0266:
        jo     SHORT loc_3_02D8
        add    byte ptr fs:[eax + 6bh], dl
        db  074h, 073h
        inc    esp
        db  072h, 06Fh
        db  070h, 070h
        add    byte ptr fs:[edi + 63615020h], bh
        imul   esp, dword ptr [ebp + 74h], 73h
        and    byte ptr [edi + ebp*2 + 73h], ch
        db  074h, 020h
        imul   ebp, dword ptr [esi + 20h], 6e617274h
        db  073h, 06Dh
        imul   esi, dword ptr [ebx + 73h], 206e6f69h
        db  074h, 06Fh
loc_3_0299:
        and    byte ptr [edi + 72h], ch
        and    byte ptr [esi + 72h], ah
        outsd
        insd
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
loc_3_02A8:
        db  072h, 076h
        db  065h, 072h, 020h
        sub    byte ptr [esi + 6fh], ch
        db  074h, 020h
        imul   ebp, dword ptr [esi + 63h], 6964756ch
        outsb
loc_3_02BA:
        and    byte ptr [bp + si + 75h], ah
        db  072h, 073h
        db  074h, 020h
        jo     SHORT loc_3_0325
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
        sub    dword ptr [eax], eax
        inc    edx
        db  075h, 072h
loc_3_02CE:
        db  073h, 074h
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
loc_3_02D8:
        and    byte ptr [edx + esi*2 + 6fh], ah
        db  070h, 070h
        add    byte ptr fs:[edx + 50h], al
        imul   esi, dword ptr [ebx + esi*2 + 44h], 72h
        outsd
        db  070h, 070h
        add    byte ptr fs:[edi + 72754220h], bh
        db  073h, 074h
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
        and    byte ptr [edi + ebp*2 + 73h], ch
        db  074h, 020h
        imul   ebp, dword ptr [esi + 20h], 6e617274h
        db  073h, 06Dh
        imul   esi, dword ptr [ebx + 73h], 206e6f69h
        db  074h, 06Fh
        and    byte ptr [edi + 72h], ch
        and    byte ptr [esi + 72h], ah
        outsd
        insd
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
loc_3_0325:
        db  072h, 076h
        db  065h, 072h, 000h
_LMSGTABLE ends
        end
