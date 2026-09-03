; i128_pcode_04.asm -- I128 object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

I128_SYSTEM_EXIT:
        db  0A1h, 08Ah, 001h, 000h, 000h
        db  08Bh, 035h, 082h, 001h, 000h, 000h
        cmp    al, 4
        jne    SHORT loc_4_0068
        mov    byte ptr [esi + 18h], 18h
        mov    byte ptr [esi + 1ch], 80h
        mov    byte ptr [esi + 18h], 19h
        mov    byte ptr [esi + 1ch], 98h
        mov    byte ptr [esi + 18h], 1ah
        mov    byte ptr [esi + 1ch], 0
        mov    byte ptr [esi + 18h], 1bh
        mov    byte ptr [esi + 1ch], 3eh
        mov    byte ptr [esi + 18h], 1eh
        mov    byte ptr [esi + 1ch], 0
        mov    byte ptr [esi + 18h], 1dh
        mov    byte ptr [esi + 1ch], 0
        mov    byte ptr [esi + 18h], 29h
        mov    byte ptr [esi + 1ch], 9
        mov    byte ptr [esi + 18h], 38h
        mov    byte ptr [esi + 1ch], 10h
        mov    byte ptr [esi + 18h], 6
        mov    byte ptr [esi + 1ch], 0
        mov    ecx, 171880h
        mov    edx, 2
        db  0E8h, 017h, 000h, 0FCh, 0FFh
        jmp    SHORT loc_4_00C0
loc_4_0068:
        mov    byte ptr [esi + 14h], 0
        mov    byte ptr [esi + 1ch], 1
        mov    byte ptr [esi + 10h], 30h
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 10h], 70h
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 10h], 2
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 18h], 2
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 18h], 1
        mov    byte ptr [esi + 10h], 10h
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 10h], 15h
        mov    byte ptr [esi + 18h], 8
        mov    byte ptr [esi + 18h], 41h
loc_4_00C0:
        db  0E8h, 00Eh, 000h, 0FCh, 0FFh
        db  0E8h, 0F0h, 0FFh, 0FBh, 0FFh
        clc
        ret
loc_4_00CC:
        call   0
        ret
_PTEXT ends
        end
