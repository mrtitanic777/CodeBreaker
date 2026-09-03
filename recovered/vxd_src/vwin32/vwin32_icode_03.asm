; vwin32_icode_03.asm -- VWIN32 object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        db  0BEh, 084h, 002h, 000h, 000h
        db  0CDh, 020h, 0EFh, 000h, 001h, 000h
        mov    ecx, 11h
        sub    eax, eax
loc_3_0012:
        db  08Bh, 034h, 085h, 068h, 003h, 000h, 000h
        or     esi, esi
        je     SHORT loc_3_0035
        db  0CDh, 020h, 080h, 000h, 001h, 000h
        jb     SHORT loc_3_0043
        or     esi, esi
        jne    SHORT loc_3_002E
        db  0BEh, 00Fh, 00Fh, 000h, 000h
loc_3_002E:
        db  089h, 034h, 085h, 068h, 003h, 000h, 000h
loc_3_0035:
        inc    eax
        loop   loc_3_0012
        db  0C7h, 005h, 084h, 003h, 000h, 000h, 00Fh, 00Fh, 000h, 000h
        clc
loc_3_0043:
        ret
loc_3_0044:
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        cmp    ax, 400h
        jb     NEAR PTR loc_3_0208
        db  0BEh, 0C8h, 011h, 000h, 000h
        db  0CDh, 020h, 0E9h, 000h, 001h, 000h
        db  0CDh, 020h, 0FFh, 000h, 001h, 000h
        test   eax, eax
        jne    SHORT loc_3_006E
        jmp    NEAR PTR loc_3_0208
loc_3_006E:
        db  0A3h, 0ECh, 010h, 000h, 000h
        push   0
        push   0
        db  0CDh, 020h, 001h, 001h, 001h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_0089
        jmp    NEAR PTR loc_3_0208
loc_3_0089:
        db  0A3h, 0F8h, 010h, 000h, 000h
        push   2
        push   0
        db  0CDh, 020h, 001h, 001h, 001h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_00A4
        jmp    NEAR PTR loc_3_0208
loc_3_00A4:
        db  0A3h, 0FCh, 010h, 000h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 5200h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 38h]
        push   eax
        mov    word ptr [ebp + 1ch], 0ea00h
        and    word ptr [ebp + 10h], 0
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_3_0155
        mov    al, 1
        db  0A2h, 0C8h, 002h, 000h, 000h
        mov    word ptr [ebp + 1ch], 58d4h
        and    word ptr [ebp + 18h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 18h], 3fcfh
        jne    SHORT loc_3_0131
        cmp    word ptr [ebp + 14h], 2
        jne    SHORT loc_3_0131
        mov    ax, word ptr [ebp + 1ch]
        cmp    al, 1
        jne    SHORT loc_3_0131
        sub    al, ah
        jne    SHORT loc_3_0131
        db  0A2h, 0C8h, 002h, 000h, 000h
        jmp    SHORT loc_3_0155
loc_3_0131:
        mov    word ptr [ebp + 1ch], 7a20h
        and    word ptr [ebp + 10h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        or     eax, eax
        jne    SHORT loc_3_0155
        db  0FFh, 005h, 0C8h, 002h, 000h, 000h
loc_3_0155:
        pop    eax
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        shl    eax, 4
        mov    ecx, dword ptr [ebx + 4]
        lea    eax, [ecx + eax + 330h]
        db  0A3h, 0F0h, 010h, 000h, 000h
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 0ECh, 010h, 000h, 000h
        push   0
        push   0
        push   0
        push   0
        db  0E8h, 0C4h, 023h, 0FDh, 0FFh
        or     eax, eax
        je     SHORT loc_3_0208
        mov    dword ptr [edi], eax
        db  0A3h, 0F4h, 010h, 000h, 000h
        db  0A3h, 0E0h, 010h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 084h, 010h, 000h, 000h
        db  0CDh, 020h, 029h, 001h, 001h, 000h
        add    esp, 8
        db  0BEh, 0F7h, 010h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     SHORT loc_3_0208
        db  0A3h, 044h, 003h, 000h, 000h
        mov    edx, 8
        db  0BEh, 086h, 011h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     SHORT loc_3_0208
        db  0A3h, 048h, 003h, 000h, 000h
        db  0BEh, 076h, 026h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     SHORT loc_3_0208
        db  0A3h, 0BCh, 002h, 000h, 000h
        db  0BEh, 0CAh, 013h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     SHORT loc_3_0208
        db  0A3h, 0C0h, 002h, 000h, 000h
        clc
        ret
loc_3_0208:
        stc
        ret
loc_3_020A:
        db  0CDh, 020h, 000h, 000h, 021h, 000h
        or     eax, eax
        je     SHORT loc_3_0219
        cmp    bl, 3
        je     SHORT loc_3_0220
loc_3_0219:
        db  083h, 00Dh, 01Ch, 011h, 000h, 000h, 002h
loc_3_0220:
        call   sub_3_022C
        db  0A3h, 080h, 002h, 000h, 000h
        clc
        ret
sub_3_022C:
        db  0BEh, 08Ch, 011h, 000h, 000h
        mov    ecx, 1
loc_3_0236:
        push   ecx
        mov    edx, dword ptr [esi]
        db  0BFh, 0D4h, 002h, 000h, 000h
        db  0CDh, 020h, 0B8h, 000h, 001h, 000h
        jb     SHORT loc_3_02A3
        mov    ebx, eax
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  0BFh, 0D4h, 002h, 000h, 000h
        mov    ecx, 80h
        sub    eax, eax
        cld
        repne scasb
        sub    ecx, 80h
        neg    ecx
        mov    ebx, ecx
        push   0
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_02A3
        mov    edx, esi
        mov    ecx, ebx
        db  0BEh, 0D4h, 002h, 000h, 000h
        mov    edi, eax
        shr    ecx, 2
        rep movsd
        mov    cl, bl
        and    cl, 3
        rep movsb
        mov    esi, edx
        mov    dword ptr [esi + 4], eax
        pop    ecx
        add    esi, 10h
        loop   loc_3_0236
        mov    eax, 1
loc_3_02A2:
        ret
loc_3_02A3:
        add    esp, 4
        db  0BEh, 08Ch, 011h, 000h, 000h
        mov    ebx, 1
loc_3_02B0:
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_3_02CE
        push   0
        push   dword ptr [esi + 4]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [esi + 4], 0
        dec    ebx
        jne    SHORT loc_3_02B0
loc_3_02CE:
        sub    eax, eax
        jmp    SHORT loc_3_02A2
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h
_ITEXT ends
        end
