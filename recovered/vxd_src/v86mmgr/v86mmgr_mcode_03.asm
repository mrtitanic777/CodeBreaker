; v86mmgr_mcode_03.asm -- V86MMGR object 3 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  001h, 000h, 000h, 000h
loc_3_0004:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_3_000C:
        add    byte ptr [eax], al
        jb     SHORT loc_3_0010
loc_3_0010:
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        popad
        outsb
        outsb
        outsd
        db  074h, 020h
        db  072h, 065h
        db  074h, 075h
        db  072h, 06Eh
        and    byte ptr [edi + ebp*2 + 20h], dh
        dec    ebp
        push   ebx
        sub    eax, 20534f44h
        db  064h, 075h, 065h
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  074h, 068h
        and    byte ptr gs:[ebx + 74h], dh
        popad
        db  074h, 065h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 78h], ah
        db  070h, 061h
        outsb
        sub    eax, 6f6d656dh
        db  072h, 079h
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 02Eh
        and    byte ptr [ecx + 6fh], bl
        db  075h, 020h
        outsb
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  072h, 065h
        db  073h, 074h
        popad
        db  072h, 074h
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 075h
        db  074h, 065h
        db  072h, 02Eh
        db  000h
_LMSGTABLE ends
        end
