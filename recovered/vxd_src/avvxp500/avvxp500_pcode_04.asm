; avvxp500_pcode_04.asm -- AVVXP500 object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        db  083h, 03Dh, 01Ch, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0010
        mov    eax, 0
        jmp    SHORT loc_4_0015
loc_4_0010:
        mov    eax, 0ffffffffh
loc_4_0015:
        ret
sub_4_0016:
        push   ebp
        mov    ebp, esp
        pushad
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 014h, 000h, 000h, 000h
        mov    ax, word ptr [ebp + 0ch]
        db  066h, 0A3h, 006h, 000h, 000h, 000h
        mov    ax, word ptr [ebp + 10h]
        db  0A2h, 008h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        db  0A3h, 010h, 000h, 000h, 000h
        push   0
        push   8000h
        push   dword ptr [ebp + 14h]
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_4_00E0
        push   0
        push   0
        push   92h
        push   7fffh
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        db  066h, 0A3h, 00Eh, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0089:
        db  066h, 0A3h, 00Ah, 000h, 000h, 000h
        xor    eax, eax
        db  00Fh, 0BEh, 00Dh, 008h, 000h, 000h, 000h
        cmp    ecx, -1
        je     SHORT loc_4_00D5
        push   edi
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        pop    edi
        jb     SHORT loc_4_00E0
        db  0A3h, 018h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 4
        jne    SHORT loc_4_00D5
        db  066h, 081h, 00Dh, 00Ch, 000h, 000h, 000h, 000h, 080h
loc_4_00D5:
        mov    dword ptr [esp + 1ch], 0ffffffffh
loc_4_00DD:
        popad
        pop    ebp
        ret
loc_4_00E0:
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_00DD
sub_4_00EA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        db  0A1h, 018h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_0100
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_4_0100:
        popad
        add    esp, 4
        pop    ebp
        ret
sub_4_0106:
        push   ebp
        mov    ebp, esp
        pushad
        mov    dword ptr [esp + 1ch], 13h
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_0016
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_0132
        mov    dword ptr [esp + 1ch], 0
loc_4_0132:
        popad
        pop    ebp
        ret
        db  0C7h, 044h, 024h, 01Ch, 024h, 000h, 000h, 000h, 0EBh, 0F3h
sub_4_013F:
        push   edi
        push   ebx
        call   0
        or     eax, eax
        je     SHORT loc_4_0159
        db  08Bh, 01Dh, 01Ch, 000h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_4_0159
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0159:
        call   sub_4_00EA
        mov    eax, 0ffffffffh
        pop    ebx
        pop    edi
        ret
loc_4_0166:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_017D:
        add    esp, 14h
        leave
        clc
        ret
loc_4_0183:
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
        stc
        ret
_PTEXT ends
        end
