; vkd_pcode_08.asm -- VKD object 8 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VKD_VM_NOT_EXECUTEABLE:
        xor    eax, eax
        mov    al, 2
        call   sub_8_003D
        clc
        ret
VKD_DESTROY_VM:
        call   sub_8_0080
        db  03Bh, 01Dh, 014h, 012h, 000h, 000h
        jne    SHORT loc_8_002C
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    eax, eax
        mov    al, 0fh
        xor    edx, edx
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        pop    ebx
loc_8_002C:
        db  0A1h, 010h, 012h, 000h, 000h
        mov    esi, dword ptr [ebx + eax + 4]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        clc
        ret
sub_8_003D:
        pushad
        mov    edi, ebx
        db  003h, 03Dh, 010h, 012h, 000h, 000h
        test   byte ptr [edi + 1], 0ch
        je     SHORT loc_8_007C
        xor    esi, esi
        mov    dword ptr [edi + 14h], esi
        mov    dword ptr [edi + 18h], esi
        xchg   dword ptr [edi + 24h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        and    byte ptr [edi + 1], 0c0h
        push   eax
        push   0
        push   dword ptr [edi + 10h]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    eax
        cmp    dword ptr [edi + 1ch], 0
        je     SHORT loc_8_007C
        mov    edx, dword ptr [edi + 20h]
        call   dword ptr [edi + 1ch]
loc_8_007C:
        popad
        ret
        db  000h, 000h
sub_8_0080:
        db  080h, 03Dh, 011h, 015h, 000h, 000h, 0FFh
        jne    SHORT loc_8_00C8
        db  080h, 03Dh, 002h, 015h, 000h, 000h, 0FFh
        jne    SHORT loc_8_00C8
        db  03Bh, 01Dh, 004h, 015h, 000h, 000h
        jne    SHORT loc_8_00C8
        push   eax
        db  0A1h, 0FEh, 014h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0C6h, 005h, 002h, 015h, 000h, 000h, 000h
        db  0C6h, 005h, 003h, 015h, 000h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  089h, 01Dh, 0B4h, 014h, 000h, 000h
        pop    ebx
        db  0E8h, 0FDh, 001h, 0F8h, 0FFh
        pop    eax
loc_8_00C8:
        clc
        ret
_PTEXT ends
        end
