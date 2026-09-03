; ibmtok_pcode_04.asm -- IBMTOK object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    ebx, ebx
        mov    dword ptr [eax], ebx
        sub    edx, edx
        mov    ecx, dword ptr [ebp + 18h]
        cmp    edx, ecx
        je     SHORT loc_4_0031
        mov    eax, dword ptr [ebp + 14h]
        lea    esp, [esp]
loc_4_0020:
        cmp    dword ptr [eax], 1
        je     SHORT loc_4_002D
        add    eax, 4
        inc    edx
        cmp    edx, ecx
        jb     SHORT loc_4_0020
loc_4_002D:
        cmp    edx, ecx
        jne    SHORT loc_4_0040
loc_4_0031:
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  090h
loc_4_0040:
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 20h]
        mov    dword ptr [eax], edx
        inc    dword ptr [esi + 5dh]
        db  0A1h, 0C8h, 056h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   96h
        push   eax
        db  0E8h, 0D5h, 005h, 0FCh, 0FFh
        or     eax, eax
        jne    NEAR PTR loc_4_0213
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, ecx
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 85h]
        push   eax
        db  0E8h, 02Ah, 000h, 0FCh, 0FFh
        or     al, al
        jne    SHORT loc_4_0100
        push   4
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        db  0E8h, 027h, 006h, 0FCh, 0FFh
        add    esp, 14h
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 048h, 005h, 0FCh, 0FFh
        mov    ebx, 0c0000001h
        jmp    NEAR PTR loc_4_01E6
        db  08Bh, 0FFh
loc_4_0100:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0f0h
        mov    dword ptr [esi + 164h], 0f0h
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [esi + 61h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 65h]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 65h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 65h], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
        cmp    byte ptr [esi + 16fh], 0
        je     NEAR PTR loc_4_01E6
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, 103h
        mov    byte ptr [eax + 1ch], 1
        cmp    byte ptr [esi + 170h], 0
        jne    NEAR PTR loc_4_01E6
        mov    byte ptr [esi + 170h], 1
        mov    dword ptr [esi + 15bh], 3
        xor    edx, edx
        mov    dx, word ptr [esi + 0b5h]
        add    edx, dword ptr [esi + 0b1h]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0fh
        rep stosd
        mov    byte ptr [edx], 3
        mov    word ptr [edx + 8], 1
        sub    ecx, ecx
loc_4_0195:
        mov    al, byte ptr [esi + ecx + 4dh]
        mov    byte ptr [edx + ecx + 0ah], al
        inc    ecx
        cmp    ecx, 6
        jb     SHORT loc_4_0195
        mov    ax, word ptr [esi + 0e3h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edx + 18h], cx
        mov    ax, word ptr [esi + 0e5h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edx + 1ah], cx
        mov    ax, word ptr [esi + 0ddh]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edx + 1ch], cx
        mov    al, byte ptr [esi + 0dbh]
        mov    byte ptr [edx + 1eh], al
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e4bh], 20h
loc_4_01E6:
        mov    eax, dword ptr [esi + 5dh]
        dec    eax
        mov    dword ptr [esi + 5dh], eax
        jne    SHORT loc_4_0208
        cmp    byte ptr [esi + 15fh], 0
        jne    SHORT loc_4_0202
        mov    eax, dword ptr [esi + 69h]
        sub    eax, esi
        cmp    eax, 69h
        je     SHORT loc_4_0208
loc_4_0202:
        push   esi
        db  0E8h, 078h, 04Eh, 0FCh, 0FFh
loc_4_0208:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_0213:
        push   5
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        db  0E8h, 0DEh, 004h, 0FCh, 0FFh
        add    esp, 14h
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  005h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
