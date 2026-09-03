; shell_pcode_12.asm -- SHELL object 12 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_12_0000:
        movzx  eax, word ptr [ebp + 14h]
        cmp    eax, 13h
        je     SHORT loc_12_0016
        cmp    eax, 26h
        je     SHORT loc_12_0016
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_12_0022
loc_12_0016:
        cmp    eax, 28h
        jae    SHORT loc_12_0022
        db  0FFh, 024h, 085h, 038h, 000h, 000h, 000h
loc_12_0022:
        mov    dword ptr [ebp + 1ch], 0ffffffffh
        or     byte ptr [ebp + 2ch], 1
        ret
        db  000h, 000h
sub_12_0030:
        mov    ebx, 81810000h
        shld   edx, eax, 10h
        sub    al, 60h
        cmp    al, 9
        ja     SHORT loc_12_0045
        or     ebx, 200020h
loc_12_0045:
        xchg   ah, al
        xor    dh, dh
        test   al, 8
        je     SHORT loc_12_004F
        inc    dh
loc_12_004F:
        test   al, 1
        je     SHORT loc_12_0056
        or     bl, 1
loc_12_0056:
        test   al, 4
        je     SHORT loc_12_005D
        or     bh, 1
loc_12_005D:
        test   al, 2
        je     SHORT loc_12_0064
        or     bl, 80h
loc_12_0064:
        test   al, 10h
        je     SHORT loc_12_006B
        or     bh, 80h
loc_12_006B:
        xchg   edx, eax
        ret
sub_12_006D:
        pushad
        db  08Bh, 035h, 0BCh, 00Bh, 000h, 000h
        xchg   edx, eax
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_12_008A
loc_12_007D:
        cmp    word ptr [eax], dx
        je     SHORT loc_12_00E5
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_12_007D
loc_12_008A:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_12_00E5
        xchg   edi, eax
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [edi], ax
        call   sub_12_0030
        mov    cl, 91h
        mov    edx, edi
        db  0BEh, 091h, 008h, 000h, 000h
        mov    edi, 0fa0h
        db  0CDh, 020h, 001h, 000h, 00Dh, 000h
        db  08Bh, 035h, 0BCh, 00Bh, 000h, 000h
        jae    SHORT loc_12_00C5
        xchg   edx, eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        stc
        jmp    SHORT loc_12_00E5
loc_12_00C5:
        mov    dword ptr [edx + 8], eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 00Dh, 000h
        xor    eax, eax
        mov    dword ptr [edx + 4], eax
        mov    word ptr [edx + 2], ax
        xchg   edx, eax
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        clc
loc_12_00E5:
        mov    dword ptr [esp + 14h], eax
        popad
        ret
loc_12_00EB:
        db  08Bh, 035h, 0BCh, 00Bh, 000h, 000h
        jmp    SHORT loc_12_0103
loc_12_00F3:
        mov    edx, dword ptr [eax + 8]
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        xchg   edx, eax
        db  0CDh, 020h, 002h, 000h, 00Dh, 000h
loc_12_0103:
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        jne    SHORT loc_12_00F3
        movzx  ecx, word ptr [ebp + 18h]
        mov    ax, 3c1ch
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        jecxz  loc_12_0138
loc_12_011C:
        mov    eax, dword ptr [esi + 2]
        call   sub_12_006D
        jb     SHORT loc_12_0182
        cmp    word ptr [edx + 2], 0
        jne    SHORT loc_12_0133
        mov    eax, dword ptr [esi]
        mov    word ptr [edx + 2], ax
loc_12_0133:
        add    esi, 6
        loop   loc_12_011C
loc_12_0138:
        movzx  ecx, word ptr [ebp + 10h]
        mov    ax, 3cffh
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
loc_12_0147:
        jecxz  loc_12_0182
        mov    eax, dword ptr [ecx + esi + 0ch]
        mov    dl, byte ptr [ecx + esi + 0ah]
        mov    ah, dl
        and    ah, 2
        test   dl, 1
        je     SHORT loc_12_015E
        or     ah, 4
loc_12_015E:
        test   dl, 4
        je     SHORT loc_12_0166
        or     ah, 1
loc_12_0166:
        test   dl, 8
        je     SHORT loc_12_016E
        or     ah, 10h
loc_12_016E:
        call   sub_12_006D
        jb     SHORT loc_12_0182
        mov    eax, dword ptr [ecx + esi + 0ah]
        mov    dword ptr [edx + 4], eax
        mov    cx, word ptr [ecx + esi]
        jmp    SHORT loc_12_0147
loc_12_0182:
        ret
        db  000h
_PTEXT ends
        end
