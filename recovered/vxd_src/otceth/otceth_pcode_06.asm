; otceth_pcode_06.asm -- OTCETH object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_6_0000:
        push   ebx
        push   esi
        push   edi
        sub    esp, 10h
        db  0CDh, 020h, 000h, 000h, 07Ch, 009h
        jb     SHORT loc_6_0054
        mov    al, 0bh
        xor    edx, edx
        xor    esi, esi
        mov    cx, 0eh
        mov    ebx, esp
        mov    word ptr [ebx + 2], 0
        db  0CDh, 020h, 001h, 000h, 07Ch, 009h
        cmp    ax, 0
        jne    SHORT loc_6_0054
        mov    ax, 19h
        cmp    byte ptr [ebx + 2], 43h
        jne    SHORT loc_6_0054
        cmp    byte ptr [ebx + 3], 53h
        jne    SHORT loc_6_0054
        mov    ax, word ptr [ebx + 4]
        db  066h, 0A3h, 0DCh, 008h, 000h, 000h
        mov    ax, word ptr [ebx + 8]
        db  066h, 0A3h, 0D8h, 008h, 000h, 000h
        mov    ax, 0
loc_6_0054:
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_6_005B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    ax, word ptr [ebp + 8]
        mov    dx, word ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 10h]
        mov    cx, word ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp + 18h]
        db  0CDh, 020h, 001h, 000h, 07Ch, 009h
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        db  083h, 03Dh, 0E8h, 008h, 000h, 000h, 000h
        jne    SHORT loc_6_009B
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  089h, 01Dh, 0E8h, 008h, 000h, 000h
loc_6_009B:
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 01Dh, 0E8h, 008h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 003h, 000h
        jb     SHORT loc_6_00BF
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        je     SHORT loc_6_00B8
        xor    eax, eax
        jmp    SHORT loc_6_00C4
loc_6_00B8:
        mov    eax, 2
        jmp    SHORT loc_6_00C4
loc_6_00BF:
        mov    eax, 1
loc_6_00C4:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    word ptr [ebx], cx
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx], edx
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        cmp    al, 14h
        je     SHORT loc_6_010F
        cmp    al, 0dh
        jne    SHORT loc_6_00E8
        db  066h, 089h, 015h, 0DAh, 008h, 000h, 000h
        jmp    SHORT loc_6_0108
loc_6_00E8:
        cmp    al, 5
        jne    SHORT loc_6_0157
        db  066h, 083h, 03Dh, 0E0h, 008h, 000h, 000h, 001h
        jne    SHORT loc_6_0108
        db  066h, 03Bh, 00Dh, 0DEh, 008h, 000h, 000h
        jne    SHORT loc_6_0108
        db  066h, 0C7h, 005h, 0E0h, 008h, 000h, 000h, 000h, 000h
loc_6_0108:
        mov    esp, ebp
        pop    ebp
        xor    ax, ax
        ret
loc_6_010F:
        mov    word ptr [ebx + 4], 0ch
        mov    word ptr [ebx + 8], 210h
        mov    word ptr [ebx + 6], 100h
        mov    word ptr [ebx + 0ah], 1e22h
        mov    word ptr [ebx + 0ch], 14h
        mov    word ptr [ebx + 0eh], 1
        mov    word ptr [ebx + 10h], 15h
        mov    word ptr [ebx + 12h], 1
        mov    edi, ebx
        add    edi, 14h
        db  0BEh, 0ECh, 008h, 000h, 000h
        mov    cx, 2
        rep movsb
        mov    word ptr [ebx + 2], 16h
        jmp    SHORT loc_6_0108
loc_6_0157:
        cmp    al, 40h
        je     SHORT loc_6_015D
        jmp    SHORT loc_6_0108
loc_6_015D:
        db  066h, 083h, 03Dh, 0E0h, 008h, 000h, 000h, 001h
        je     SHORT loc_6_0108
        jne    SHORT loc_6_0108
        db  066h, 0C7h, 005h, 0E0h, 008h, 000h, 000h, 001h, 000h
        jmp    SHORT loc_6_0108
_PTEXT ends
        end
