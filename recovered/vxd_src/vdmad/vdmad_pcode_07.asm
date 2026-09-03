; vdmad_pcode_07.asm -- VDMAD object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_7_0000:
        sub    esp, 8
        pushad
        sub    esi, dword ptr [ebx + 4]
        dec    ecx
        add    ecx, esi
        shr    esi, 0ch
        mov    dword ptr [esp + 20h], esi
        shr    ecx, 0ch
        sub    ecx, esi
        inc    ecx
        mov    dword ptr [esp + 24h], ecx
        mov    edx, ecx
        mov    eax, edx
        dec    eax
        bsr    cx, ax
        inc    cl
        mov    eax, 1
        shl    eax, cl
        dec    eax
        push   0eh
        push   0
        db  0FFh, 035h, 000h, 021h, 000h, 000h
        push   0
        push   eax
        push   0
        push   1
        push   edx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_7_008E
        mov    edi, eax
        mov    esi, dword ptr [esp + 20h]
        shl    esi, 0ch
        mov    ecx, dword ptr [esp + 24h]
        shl    ecx, 0ah
        rep movsd
        mov    esi, eax
        shr    eax, 0ch
        push   0
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   ebx
        push   eax
        db  0CDh, 020h, 062h, 000h, 001h, 000h
        add    esp, 14h
        push   eax
        push   0
        push   esi
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    eax
        sub    eax, 1
loc_7_0089:
        popad
        add    esp, 8
        ret
loc_7_008E:
        stc
        jmp    SHORT loc_7_0089
loc_7_0091:
        add    ecx, 3ffh
        shr    ecx, 0ah
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_7_00D0
        push   0
        push   ecx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 050h, 000h, 033h, 000h
        add    esp, 0ch
        mov    eax, 10009h
        xor    edx, edx
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        db  0CDh, 020h, 02Fh, 000h, 001h, 000h
loc_7_00D0:
        push   ecx
        push   0c0001h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   esi, eax
        pushad
        mov    esi, esi
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
        add    byte ptr [eax], al
        db  000h
_PTEXT ends
        end
