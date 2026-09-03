; diskvsd_pcode_03.asm -- DISKVSD object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    esi, dword ptr [ebx + 0ch]
        test   dword ptr [esi + 20h], 6000h
        jne    NEAR PTR loc_3_0180
        cmp    byte ptr [esi + 74h], 1
        jne    NEAR PTR loc_3_0180
        cmp    byte ptr [esi + 40h], 0
        jne    NEAR PTR loc_3_0180
        mov    eax, dword ptr [esi + 1ch]
        or     eax, 40000h
        test   byte ptr [esi + 83h], 80h
        je     SHORT loc_3_00A4
        cmp    dword ptr [esi + 8ah], 49534e49h
        jne    SHORT loc_3_005B
        cmp    dword ptr [esi + 8eh], 20204554h
        jne    SHORT loc_3_008E
        cmp    dword ptr [esi + 92h], 35323349h
        jne    SHORT loc_3_008E
        jmp    SHORT loc_3_008A
loc_3_005B:
        cmp    word ptr [esi + 0a0h], 462ah
        jne    SHORT loc_3_008E
        cmp    dword ptr [esi + 8ah], 454d4f49h
        jne    SHORT loc_3_008E
        cmp    dword ptr [esi + 8eh], 20204147h
        jne    SHORT loc_3_008E
        cmp    dword ptr [esi + 92h], 30326f49h
        jne    SHORT loc_3_008E
loc_3_008A:
        or     byte ptr [esi + 24h], 20h
loc_3_008E:
        test   byte ptr [esi + 24h], 40h
        je     SHORT loc_3_0098
        mov    byte ptr [esi + 40h], 0ah
loc_3_0098:
        or     dword ptr [esi + 20h], 10004h
        or     eax, 20000h
loc_3_00A4:
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        sub    esp, 18h
        mov    ecx, esp
        mov    word ptr [ecx], 5
        or     eax, 8
        and    eax, 0fffffffeh
        mov    dword ptr [ecx + 12h], eax
        mov    dword ptr [ecx + 4], esi
        db  0C7h, 041h, 008h, 040h, 004h, 000h, 000h
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ecx + 16h], al
        mov    dword ptr [ecx + 0ch], 0
        movzx  eax, word ptr [esi + 80h]
        add    eax, 5eh
        mov    word ptr [ecx + 10h], ax
        push   ecx
        db  0FFh, 015h, 000h, 00Bh, 000h, 000h
        add    esp, 1ch
        cmp    byte ptr [esi + 40h], 0ah
        jne    SHORT loc_3_0141
        sub    esp, 2ch
        mov    ecx, esp
        mov    eax, 2ch
        mov    edx, 2ch
        push   ecx
        mov    ebx, dword ptr [esi]
        db  0E8h, 02Dh, 008h, 0FDh, 0FFh
        pop    ecx
        cmp    al, 0
        jne    SHORT loc_3_013E
        cmp    byte ptr [ecx + 4], 27h
        jb     SHORT loc_3_013E
        mov    ecx, dword ptr [ecx + 27h]
        xor    cl, cl
        xchg   ch, cl
        ror    ecx, 10h
        xchg   ch, cl
        mov    dword ptr [esi + 54h], ecx
        mov    dword ptr [esi + 60h], 8
        mov    dword ptr [esi + 68h], 20h
        mov    dword ptr [esi + 5ch], 200h
        shr    ecx, 8
        mov    dword ptr [esi + 64h], ecx
loc_3_013E:
        add    esp, 2ch
loc_3_0141:
        mov    word ptr [esi + 7ah], 1
        test   byte ptr [esi + 83h], 80h
        je     SHORT loc_3_0180
        mov    ebx, esi
        mov    ax, 6
        xor    ecx, ecx
        db  0E8h, 0D9h, 007h, 0FDh, 0FFh
        mov    ax, 9
        xor    ecx, ecx
        db  0E8h, 0CEh, 007h, 0FDh, 0FFh
        cmp    ax, 10h
        jae    SHORT loc_3_0180
        mov    ax, 0ah
        xor    ecx, ecx
        db  0E8h, 0BDh, 007h, 0FDh, 0FFh
        or     dword ptr [ebx + 1ch], 10000h
loc_3_0180:
        ret
loc_3_0181:
        mov    esi, dword ptr [ebx + 0ch]
        cmp    byte ptr [esi + 74h], 1
        jne    SHORT loc_3_0196
        cmp    byte ptr [esi + 40h], 0
        jne    SHORT loc_3_0196
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
loc_3_0196:
        ret
loc_3_0197:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_01A6
        mov    word ptr [ebx + 2], 0ffffh
loc_3_01A6:
        ret
        db  025h
_PTEXT ends
        end
