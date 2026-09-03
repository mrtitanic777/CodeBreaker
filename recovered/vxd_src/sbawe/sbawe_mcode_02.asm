; sbawe_mcode_02.asm -- SBAWE object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 027h, 000h
loc_2_0010:
        push   ebx
        outsd
        db  075h, 06Eh
        inc    edx
        insb
        popad
        db  073h, 074h
        db  065h, 072h, 020h
        inc    ecx
        push   edi
        inc    ebp
        xor    esi, dword ptr [edx]
        and    byte ptr [ecx + ebp*2 + 72h], al
        arpl   word ptr gs:[eax + 53h], si
        outsd
        db  075h, 06Eh
        and    byte ptr fs:[edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 000h
_LMSGTABLE ends
        end
