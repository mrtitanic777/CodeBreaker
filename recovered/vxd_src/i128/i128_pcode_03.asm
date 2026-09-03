; i128_pcode_03.asm -- I128 object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    eax, dword ptr [ebp + 18h]
        db  0A3h, 086h, 001h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        db  0A3h, 082h, 001h, 000h, 000h
        mov    eax, dword ptr [ebp + 4]
        db  0A3h, 08Ah, 001h, 000h, 000h
        db  0B8h, 038h, 001h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        mov    word ptr [ebp + 10h], 0
        mov    word ptr [ebp + 18h], 0
        db  066h, 08Bh, 03Dh, 058h, 001h, 000h, 000h
        or     di, di
        je     SHORT loc_3_0047
        mov    word ptr [ebp + 18h], di
loc_3_0047:
        ret
loc_3_0048:
        db  0E8h, 0B3h, 0FFh, 0FCh, 0FFh
        ret
_PTEXT ends
        end
