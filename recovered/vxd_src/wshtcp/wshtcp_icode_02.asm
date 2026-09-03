; wshtcp_icode_02.asm -- WSHTCP object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 001h, 000h, 095h, 004h
        db  0A3h, 025h, 001h, 000h, 000h
        pop    ecx
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 002h, 000h, 095h, 004h
        db  0C7h, 005h, 025h, 001h, 000h, 000h, 021h, 001h, 000h, 000h
        leave
        ret
loc_2_003B:
        db  080h, 03Dh, 020h, 001h, 000h, 000h, 000h
        jne    SHORT loc_2_0068
        db  0C6h, 005h, 020h, 001h, 000h, 000h, 001h
loc_2_004B:
        db  0BFh, 0B4h, 00Fh, 000h, 000h
        db  0B9h, 0B4h, 00Fh, 000h, 000h
        sub    ecx, edi
        shr    ecx, 2
        xor    eax, eax
        cld
        rep stosd
        db  080h, 03Dh, 020h, 001h, 000h, 000h, 000h
        jne    SHORT loc_2_006A
loc_2_0068:
        clc
        ret
loc_2_006A:
        call   sub_2_0080
        cmp    eax, 1
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0080:
        db  068h, 0ACh, 000h, 000h, 000h
        db  068h, 044h, 001h, 000h, 000h
        db  068h, 03Ch, 001h, 000h, 000h
        call   0
        add    esp, 0ch
        ret
_ITEXT ends
        end
