; smartvsd_pcode_03.asm -- SMARTVSD object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_0000:
        db  0A0h, 009h, 012h, 000h, 000h
        shl    al, 2
        db  00Ah, 005h, 000h, 012h, 000h, 000h
        db  0A2h, 0ACh, 011h, 000h, 000h
        db  080h, 03Dh, 0ACh, 011h, 000h, 000h, 000h
        jne    SHORT loc_3_0022
        mov    word ptr [ebx + 2], 0ffffh
loc_3_0022:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0030:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 21h], 60h
        jne    NEAR PTR loc_3_008B
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 74h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_3_0075
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 40h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_3_0082
loc_3_0075:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 24h], 2
        je     NEAR PTR loc_3_008B
loc_3_0082:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_0090
loc_3_008B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_0090:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ech
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    byte ptr [ebp - 14h], 0
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 70h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 047h, 0FFh, 0FCh, 0FFh
        add    esp, 0ch
        mov    byte ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_3_00C8
loc_3_00C5:
        inc    byte ptr [ebp - 0ch]
loc_3_00C8:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 2
        jge    NEAR PTR loc_3_0153
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        db  039h, 00Ch, 085h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_3_014E
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        db  08Ah, 084h, 0C0h, 000h, 012h, 000h, 000h
        and    eax, 0ffh
        or     al, 2
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0ch]
        db  088h, 084h, 0C9h, 000h, 012h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0ch]
        db  089h, 084h, 0C9h, 005h, 012h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 24h], 2
        je     NEAR PTR loc_3_0145
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        db  08Ah, 084h, 0C0h, 000h, 012h, 000h, 000h
        and    eax, 0ffh
        or     al, 20h
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0ch]
        db  088h, 084h, 0C9h, 000h, 012h, 000h, 000h
loc_3_0145:
        mov    byte ptr [ebp - 14h], 0ffh
        jmp    NEAR PTR loc_3_0153
loc_3_014E:
        jmp    NEAR PTR loc_3_00C5
loc_3_0153:
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        or     eax, eax
        jne    NEAR PTR loc_3_01FD
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ech]
        push   eax
        db  0E8h, 096h, 0FEh, 0FCh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_3_017D
loc_3_017D:
        xor    eax, eax
        mov    ax, word ptr [ebp - 8eh]
        sub    eax, 1f0h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        db  08Ah, 084h, 0C0h, 000h, 012h, 000h, 000h
        and    eax, 0ffh
        or     al, 1
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        db  088h, 084h, 0C9h, 000h, 012h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        db  089h, 084h, 0C9h, 001h, 012h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 24h], 2
        je     NEAR PTR loc_3_01FD
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        db  08Ah, 084h, 0C0h, 000h, 012h, 000h, 000h
        and    eax, 0ffh
        or     al, 10h
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        db  088h, 084h, 0C9h, 000h, 012h, 000h, 000h
loc_3_01FD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
_PTEXT ends
        end
