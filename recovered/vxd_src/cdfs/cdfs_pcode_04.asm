; cdfs_pcode_04.asm -- CDFS object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0A1h, 000h, 000h, 000h
        db  0BDh, 000h, 000h, 000h, 0D6h, 000h, 000h, 000h
sub_4_0018:
        push   eax
        db  070h, 000h
        db  000h
sub_4_001C:
        rol    byte ptr [ebx], 1
        add    byte ptr [eax], al
loc_4_0020:
        db  0F7h, 005h, 004h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_003E
        db  0F7h, 005h, 004h, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        jne    SHORT loc_4_003E
        db  0FFh, 015h, 018h, 000h, 000h, 000h
loc_4_003E:
        db  083h, 025h, 004h, 000h, 000h, 000h, 0CFh
        ret
loc_4_0046:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        jecxz  loc_4_006D
        mov    eax, dword ptr [ecx]
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0046
        db  0A3h, 000h, 000h, 000h, 000h
        push   dword ptr [ecx + 4]
        push   ecx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        lea    esp, [esp + 8]
        jmp    SHORT loc_4_0046
loc_4_006D:
        db  081h, 025h, 004h, 000h, 000h, 000h, 0FFh, 0FEh, 0FFh, 0FFh
        ret
loc_4_0078:
        db  0F7h, 005h, 004h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_0096
        db  0F7h, 005h, 004h, 000h, 000h, 000h, 000h, 010h, 000h, 000h
        jne    SHORT loc_4_0096
        db  0FFh, 015h, 01Ch, 000h, 000h, 000h
loc_4_0096:
        db  081h, 025h, 004h, 000h, 000h, 000h, 0FFh, 0CFh, 0FFh, 0FFh
        ret
sub_4_00A1:
        mov    eax, ecx
        shr    ecx, 2
        stc
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        ret
loc_4_00B1:
        xor    ecx, ecx
        movsb
        inc    eax
loc_4_00B5:
        xor    ecx, ecx
        movsb
        inc    eax
loc_4_00B9:
        xor    ecx, ecx
        movsb
        inc    eax
loc_4_00BD:
        stc
        ret
loc_4_00BF:
        push   eax
        shr    eax, 2
        sub    eax, ecx
        shl    eax, 2
        pop    ecx
        sub    ecx, eax
        cmp    ecx, 2
        jecxz  loc_4_00BD
        jb     SHORT loc_4_00B9
        je     SHORT loc_4_00B5
        jmp    SHORT loc_4_00B1
loc_4_00D6:
        jb     SHORT loc_4_00BF
        sub    eax, ecx
        stc
        ret
_PTEXT ends
        end
