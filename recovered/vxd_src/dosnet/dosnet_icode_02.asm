; dosnet_icode_02.asm -- DOSNET object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

DOSNET_DEVICE_INIT:
        push   0
        push   6ch
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_2_009C
        db  0A3h, 014h, 002h, 000h, 000h
        mov    eax, 21h
        db  0BEh, 028h, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    esi, dword ptr [ebx + 8]
        mov    word ptr [esi + 1ch], 5200h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  edi, word ptr [esi + 38h]
        shl    edi, 4
        movzx  ecx, word ptr [esi + 10h]
        add    edi, ecx
        mov    eax, dword ptr [edi + 16h]
        movzx  ecx, ax
        shr    eax, 10h
        shl    eax, 4
        add    eax, ecx
        db  0A3h, 018h, 002h, 000h, 000h
        movzx  eax, byte ptr [edi + 21h]
        db  0A3h, 020h, 002h, 000h, 000h
        db  0C7h, 005h, 01Ch, 002h, 000h, 000h, 058h, 000h, 000h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        clc
        ret
loc_2_009C:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
        add    byte ptr [eax], al
_ITEXT ends
        end
