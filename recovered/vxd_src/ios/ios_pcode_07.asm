; ios_pcode_07.asm -- IOS object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_7_0000:
        push   esi
        db  08Bh, 035h, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [esi + 2ah], 1
        jne    SHORT loc_7_000E
        stc
loc_7_000E:
        pop    esi
        ret
loc_7_0010:
        sub    esp, 4
        mov    esi, esp
        mov    byte ptr [esi], 0ah
        mov    al, byte ptr [ebp + 14h]
        mov    byte ptr [esi + 2], al
        push   esi
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        add    esp, 4
        cmp    byte ptr [esi + 1], 0
        jne    SHORT loc_7_003B
        mov    ax, word ptr [esi + 2]
        mov    word ptr [ebp + 14h], ax
        add    esp, 4
        clc
        ret
loc_7_003B:
        add    esp, 4
        stc
        ret
loc_7_0040:
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        db  08Dh, 035h, 045h, 005h, 000h, 000h
        cld
        movsd
        movsd
        xor    eax, eax
        stosb
        db  0A0h, 044h, 005h, 000h, 000h
        mov    dword ptr [ebp + 14h], eax
        ret
loc_7_0061:
        push   ebp
        movzx  eax, byte ptr [ebp + 14h]
        db  0E8h, 022h, 012h, 0F9h, 0FFh
        or     ebx, ebx
        je     SHORT loc_7_00EC
        mov    ebx, dword ptr [ebx]
        mov    ebp, ebx
        mov    esi, dword ptr [ebp + 4]
        add    esi, 64h
        sub    esp, 10h
        mov    ebx, esp
        mov    dword ptr [ebx + 6], esi
        add    esi, 58h
        mov    word ptr [ebx + 4], si
        mov    byte ptr [ebx + 0eh], 2
        mov    word ptr [ebx], 2
        push   ebx
        db  0E8h, 079h, 012h, 0F9h, 0FFh
        mov    ebx, dword ptr [ebx + 0ah]
        add    esp, 14h
        add    ebx, dword ptr [ebp + 4]
        mov    word ptr [ebx + 68h], 0ch
        mov    byte ptr [ebx + 0b1h], al
        xor    eax, eax
        mov    dword ptr [ebx + 64h], eax
        mov    dword ptr [ebx + 0a8h], eax
        mov    dword ptr [ebx + 7ch], eax
        mov    dword ptr [ebx + 80h], eax
        mov    dword ptr [ebx + 6ch], 500h
        mov    edi, ebx
        add    ebx, 64h
        xchg   esi, ebx
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        mov    ax, word ptr [edi + 6ah]
        mov    edi, dword ptr [edi + 88h]
        db  0E8h, 03Bh, 009h, 0F9h, 0FFh
        cmp    ax, 10h
        jae    SHORT loc_7_00EC
        clc
loc_7_00EA:
        pop    ebp
        ret
loc_7_00EC:
        stc
        jmp    SHORT loc_7_00EA
        db  000h
loc_7_00F0:
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [edi], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 55h], dl
        dec    esi
        inc    esp
        dec    esp
        dec    esp
        add    byte ptr [ebx + 79h], dl
        db  073h, 044h
        dec    ebp
        inc    ebx
        db  070h, 06Ch
        sub    al, 44h
        imul   esi, dword ptr [ebx + 70h], 5279616ch
        popad
        insb
        dec    ebp
        outsd
        push   edi
        popad
        db  072h, 06Eh
        db  069h, 06Eh, 067h, 021h
_PTEXT ends
        end
