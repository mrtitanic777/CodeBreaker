; necatapi_pcode_03.asm -- NECATAPI object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        push   esi
        mov    esi, dword ptr [ebx + 0ch]
        test   dword ptr [esi + 20h], 6000h
        jne    SHORT loc_3_007F
        cmp    byte ptr [esi + 74h], 1
        jne    SHORT loc_3_007F
        cmp    byte ptr [esi + 40h], 5
        jne    SHORT loc_3_007F
        cmp    dword ptr [esi + 8ah], 2043454eh
        jne    SHORT loc_3_007F
        cmp    dword ptr [esi + 8eh], 20202020h
        jne    SHORT loc_3_007F
        cmp    dword ptr [esi + 0a2h], 31302e31h
        jne    SHORT loc_3_007F
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 1ch]
        push   esi
        push   edi
        sub    esp, 18h
        mov    edi, esp
        mov    word ptr [edi], 5
        mov    dword ptr [edi + 12h], eax
        mov    dword ptr [edi + 4], esi
        db  0C7h, 047h, 008h, 0E8h, 001h, 000h, 000h
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [edi + 16h], al
        mov    dword ptr [edi + 0ch], 0
        xor    eax, eax
        mov    word ptr [edi + 10h], ax
        push   edi
        db  02Eh, 0FFh, 015h, 024h, 003h, 000h, 000h
        add    esp, 1ch
        pop    edi
        pop    esi
loc_3_007F:
        pop    esi
        ret
loc_3_0081:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_0090
        mov    word ptr [ebx + 2], 0ffffh
loc_3_0090:
        ret
        db  025h, 04Eh, 045h
_PTEXT ends
        end
