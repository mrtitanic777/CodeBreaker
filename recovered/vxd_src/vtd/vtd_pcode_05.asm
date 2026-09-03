; vtd_pcode_05.asm -- VTD object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VTD_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 0
        je     SHORT loc_5_001B
        cmp    ah, 1
        je     SHORT loc_5_0025
loc_5_0016:
        or     byte ptr [ebp + 2ch], 1
        ret
loc_5_001B:
        mov    ah, 4
loc_5_001D:
        mov    dword ptr [ebp + 1ch], eax
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_5_0025:
        cmp    al, 1
        jb     SHORT loc_5_0037
        je     SHORT loc_5_0042
        cmp    al, 2
        jne    SHORT loc_5_0016
        db  0CDh, 020h, 040h, 000h, 001h, 000h
        jmp    SHORT loc_5_001D
loc_5_0037:
        db  0CDh, 020h, 007h, 000h, 005h, 000h
        mov    dword ptr [ebp + 14h], edx
        jmp    SHORT loc_5_001D
loc_5_0042:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        jmp    SHORT loc_5_001D
loc_5_004A:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 160eh
        mov    byte ptr [ebp + 10h], 2
        xor    eax, eax
        mov    al, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, dword ptr [20h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [70h]
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        xor    eax, ecx
        db  0A3h, 020h, 005h, 000h, 000h
        mov    word ptr [ebp + 1ch], 160eh
        mov    byte ptr [ebp + 10h], 1
        xor    eax, eax
        mov    al, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        clc
        ret
loc_5_00BB:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        jecxz  loc_5_00D3
        db  0A1h, 004h, 000h, 000h, 000h
        mov    dword ptr [20h], ecx
        mov    dword ptr [70h], eax
loc_5_00D3:
        db  0A1h, 030h, 005h, 000h, 000h
        mov    dword ptr [46ch], eax
        mov    byte ptr [470h], 0
        mov    al, 36h
        out    43h, al
        jmp    SHORT loc_5_00EA
loc_5_00EA:
        jmp    SHORT loc_5_00EC
loc_5_00EC:
        xor    al, al
        out    40h, al
        jmp    SHORT loc_5_00F2
loc_5_00F2:
        jmp    SHORT loc_5_00F4
loc_5_00F4:
        out    40h, al
        clc
        ret
loc_5_00F8:
        inc    ebx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        and    al, 0
loc_5_00FF:
        pushad
        mov    ax, 3d00h
        db  0BAh, 0F8h, 000h, 000h, 000h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_5_0174
        mov    ebx, eax
        mov    ax, 4400h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_5_016A
        mov    dh, 0
        or     dl, 20h
        mov    ax, 4401h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_5_016A
        db  08Bh, 035h, 0C8h, 004h, 000h, 000h
loc_5_013C:
        sti
        mov    di, word ptr [esi]
        push   6
        pop    ecx
        sub    esp, 8
        mov    edx, esp
        mov    ah, 3fh
        push   esi
        push   edi
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edi
        pop    esi
        pop    ecx
        pop    edx
        jb     SHORT loc_5_016A
        cmp    ax, 6
        jne    SHORT loc_5_016A
        cli
        cmp    word ptr [esi], di
        jne    SHORT loc_5_013C
        mov    word ptr [esi], cx
        sti
loc_5_016A:
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_5_0174:
        popad
        db  0C6h, 005h, 036h, 005h, 000h, 000h, 000h
        ret
_PTEXT ends
        end
