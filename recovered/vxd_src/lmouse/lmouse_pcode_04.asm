; lmouse_pcode_04.asm -- LMOUSE object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  083h, 03Dh, 03Eh, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0020
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ah, 0c2h
        jne    SHORT loc_4_0020
        cmp    al, 7
        ja     SHORT loc_4_0020
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        db  0E9h, 0BCh, 039h, 0FCh, 0FFh
loc_4_0020:
        db  08Bh, 015h, 03Ah, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 038h, 000h, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 017h
        dec    esp
loc_4_0034:
        dec    ebp
        dec    edi
        push   ebp
        push   ebx
loc_4_0038:
        inc    ebp
        db  02Eh
loc_4_003A:
        push   esi
        pop    eax
        inc    esp
        db  020h
loc_4_003E:
        push   esi
        db  065h, 072h, 073h
        db  069h, 06Fh
_PTEXT ends
        end
