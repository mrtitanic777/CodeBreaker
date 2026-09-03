; ndis_mcode_02.asm -- NDIS object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 012h, 000h
        db  023h, 000h, 034h, 000h, 046h, 000h, 058h, 000h, 067h, 000h, 073h, 000h, 07Ch, 000h, 08Ch, 000h
        db  0A6h, 000h, 0C1h, 000h, 0E1h, 000h, 0F5h, 000h, 009h, 001h, 01Ah, 001h, 026h, 001h, 0CCh, 0CCh
loc_2_0030:
        push   edx
        db  065h, 073h, 06Fh
        db  075h, 072h
        arpl   word ptr [ebp + 20h], sp
        inc    ebx
        outsd
        outsb
        insb
        imul   esp, dword ptr [ebx + 74h], 74754f00h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 65h], dl
        db  073h, 06Fh
        db  075h, 072h
        arpl   word ptr [ebp + 73h], sp
        add    byte ptr [eax + 61h], cl
        jb     SHORT loc_2_00BB
        db  077h, 061h
        jb     SHORT loc_2_00C0
        and    byte ptr [esi + 61h], al
        imul   ebp, dword ptr [ebp + esi*2 + 72h], 64410065h
        popad
        db  070h, 074h
        db  065h, 072h, 020h
        dec    esi
        outsd
        je     SHORT loc_2_0090
        outsw
        db  075h, 06Eh
        add    byte ptr fs:[ecx + 6eh], cl
        db  074h, 065h
        jb     SHORT loc_2_00EE
        jne    SHORT loc_2_00EE
        db  074h, 020h
        inc    ebx
        outsd
        outsb
        outsb
        arpl   word ptr gs:[eax + eax + 44h], si
        db  072h, 069h
        jbe    SHORT loc_2_00F2
        db  072h, 020h
        inc    esi
loc_2_0090:
        popad
        imul   ebp, dword ptr [ebp + esi*2 + 72h], 61420065h
        and    byte ptr fs:[esi + 65h], dl
        jb     SHORT loc_2_0112
        imul   ebp, dword ptr [edi + 6eh], 6d695400h
        and    byte ptr gs:[edi + 75h], ch
        je     SHORT loc_2_00AC
loc_2_00AC:
        dec    esi
        je     NEAR PTR loc_2_0127
        outsd
        db  072h, 06Bh
        and    byte ptr [ecx + 64h], al
        db  064h, 072h, 065h
        db  073h, 073h
loc_2_00BB:
        add    byte ptr [ebp + 6eh], dl
        jae    SHORT loc_2_0135
loc_2_00C0:
        db  070h, 070h
        outsd
        db  072h, 074h
        and    byte ptr fs:[ebx + 6fh], al
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        je     SHORT loc_2_013C
        outsd
        outsb
        add    byte ptr [ecx + 6eh], cl
        jbe    SHORT loc_2_013B
        insb
        imul   esp, dword ptr [eax + 56h], 65756c61h
        and    byte ptr [esi + 72h], al
        outsd
        insd
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
loc_2_00EE:
        jb     NEAR PTR loc_2_00F1
loc_2_00F1:
        dec    ebp
loc_2_00F2:
        imul   esi, dword ptr [ebx + 73h], 20676e69h
        inc    ebx
        outsd
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [eax + 61h], dl
        db  072h, 061h
        insd
        db  065h, 074h, 065h
        jb     SHORT loc_2_0111
loc_2_0111:
        inc    edx
loc_2_0112:
        popad
        and    byte ptr fs:[ecx + 4fh], cl
        and    byte ptr [edx + 61h], al
        db  073h, 065h
        and    byte ptr [ecx + 64h], al
        db  064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [edx + 65h], dl
loc_2_0127:
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr [ebx + 70h], dl
        popad
        arpl   word ptr [ebp + 20h], sp
        push   ebx
        insd
loc_2_0135:
        popad
        insb
        insb
        add    byte ptr [ecx + 64h], al
loc_2_013B:
        popad
loc_2_013C:
        db  070h, 074h
        db  065h, 072h, 020h
        inc    esp
        imul   esi, dword ptr [ebx + 61h], 64656c62h
        add    byte ptr [ebp + 72h], al
        db  072h, 06Fh
        db  072h, 020h
        inc    ebx
        outsd
        db  064h, 065h, 03Dh, 000h
_LMSGTABLE ends
        end
