; dynapage_pcode_06.asm -- DYNAPAGE object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_6_0000:
        pushad
        xor    esi, esi
        mov    edi, edx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0FEh, 005h, 0ACh, 000h, 000h, 000h
        shl    edx, 0ch
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0A1h, 09Ch, 000h, 000h, 000h
        mov    word ptr [ebp + 30h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 34h], ax
        call   sub_6_01DD
        db  0A1h, 0F0h, 000h, 000h, 000h
        mov    dword ptr [ebp + 10h], eax
        mov    dword ptr [ebp + 14h], edx
        shr    edx, 10h
        mov    dword ptr [ebp + 18h], edx
        mov    word ptr [ebp + 1ch], 4200h
        call   sub_6_0241
        jb     SHORT loc_6_00BA
        mov    byte ptr [ebp + 1dh], 40h
        mov    dword ptr [ebp + 18h], esi
        call   sub_6_0241
        jb     SHORT loc_6_00BA
        mov    byte ptr [ebp + 1dh], 45h
        call   sub_6_0241
        jb     SHORT loc_6_0087
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp + 10h], eax
        mov    byte ptr [ebp + 1dh], 3eh
        call   sub_6_0241
loc_6_0087:
        db  0A1h, 0F0h, 000h, 000h, 000h
        mov    dword ptr [ebp + 10h], eax
        mov    word ptr [ebp + 1ch], 4202h
        mov    dword ptr [ebp + 18h], esi
        mov    dword ptr [ebp + 14h], esi
        call   sub_6_0241
        jb     SHORT loc_6_00BA
        mov    eax, dword ptr [ebp + 14h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 1ch]
        shr    eax, 0ch
        db  0A3h, 088h, 000h, 000h, 000h
        cmp    eax, edi
        jne    SHORT loc_6_00BA
        mov    esi, esp
loc_6_00BA:
        call   sub_6_0215
        db  0FEh, 00Dh, 0ACh, 000h, 000h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        sub    esi, 1
        popad
        ret
PageFile_Get_Version:
        db  08Ah, 01Dh, 0F4h, 000h, 000h, 000h
sub_6_00E5:
        mov    eax, 400h
        clc
        ret
PageFile_Cancel:
        ret
PAGEFILE_PM_API:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_6_010B
        movzx  eax, word ptr [ebp + 1ch]
        cmp    ax, 4
        jae    SHORT loc_6_010B
        and    byte ptr [ebp + 2ch], 0feh
        db  0FFh, 014h, 085h, 0B0h, 000h, 000h, 000h
loc_6_010A:
        ret
loc_6_010B:
        or     byte ptr [ebp + 2ch], 1
        jmp    SHORT loc_6_010A
loc_6_0111:
        call   sub_6_00E5
        mov    dword ptr [ebp + 1ch], eax
        ret
loc_6_011A:
        xor    eax, eax
        db  039h, 005h, 080h, 000h, 000h, 000h
        je     SHORT loc_6_0168
        xchg   edi, eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        inc    edi
        je     SHORT loc_6_016C
        dec    edi
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 20h
        cld
        rep movsd
        db  0A1h, 080h, 000h, 000h, 000h
        shl    eax, 0ch
        mov    dword ptr [ebp + 18h], eax
        db  0A1h, 084h, 000h, 000h, 000h
        shl    eax, 0ch
        mov    dword ptr [ebp + 14h], eax
        mov    eax, 1
        db  080h, 03Dh, 0F4h, 000h, 000h, 000h, 003h
        je     SHORT loc_6_0168
        or     ah, 40h
loc_6_0168:
        mov    dword ptr [ebp + 1ch], eax
loc_6_016B:
        ret
loc_6_016C:
        or     byte ptr [ebp + 2ch], 1
        jmp    SHORT loc_6_016B
loc_6_0172:
        ret
loc_6_0173:
        xor    eax, eax
        mov    word ptr [ebp + 14h], ax
        mov    word ptr [ebp + 1ch], ax
        db  039h, 005h, 080h, 000h, 000h, 000h
        je     SHORT loc_6_0198
        db  0A1h, 088h, 000h, 000h, 000h
        shl    eax, 0ch
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
loc_6_0198:
        ret
loc_6_0199:
        db  080h, 03Dh, 0ACh, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_01A4
        stc
        ret
loc_6_01A4:
        mov    byte ptr [ebp + 1ch], 3
        clc
        ret
loc_6_01AA:
        db  080h, 03Dh, 0ACh, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_01B5
        stc
        ret
loc_6_01B5:
        clc
        ret
sub_6_01B7:
        pushad
        db  0CDh, 020h, 06Ah, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        push   10h
        push   0
        push   1
        db  0FFh, 035h, 098h, 000h, 000h, 000h
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        popad
        ret
sub_6_01DD:
        mov    byte ptr [ebp + 1dh], 51h
        call   sub_6_0241
        mov    eax, dword ptr [ebp + 10h]
        db  066h, 0A3h, 0A8h, 000h, 000h, 000h
        db  00Fh, 0B7h, 005h, 0AAh, 000h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 015h, 000h
        mov    dword ptr [ebp + 10h], eax
        shl    eax, 4
        push   dword ptr [eax + 2eh]
        db  08Fh, 005h, 0A4h, 000h, 000h, 000h
        mov    byte ptr [ebp + 1dh], 50h
        call   sub_6_0241
        ret
sub_6_0215:
        db  066h, 0A1h, 0A8h, 000h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 015h, 000h
        mov    dword ptr [ebp + 10h], eax
        mov    byte ptr [ebp + 1dh], 50h
        call   sub_6_0241
        db  00Fh, 0B7h, 005h, 0AAh, 000h, 000h, 000h
        shl    eax, 4
        db  0FFh, 035h, 0A4h, 000h, 000h, 000h
        pop    dword ptr [eax + 2eh]
        ret
sub_6_0241:
        push   eax
        push   ecx
        push   edx
        db  08Bh, 00Dh, 094h, 000h, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        and    byte ptr [ebp + 2dh], 0fbh
        db  0CDh, 020h, 035h, 001h, 001h, 000h
        db  0CDh, 020h, 037h, 001h, 001h, 000h
        pushfd
        db  0CDh, 020h, 036h, 001h, 001h, 000h
        popfd
        jb     SHORT loc_6_0276
        bt     word ptr [ebp + 2ch], 0
loc_6_0276:
        pop    edx
        pop    ecx
        pop    eax
        ret
loc_6_027A:
        pushad
        mov    di, 1
        mov    esi, ebx
        mov    eax, dword ptr [esi + 8]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        shr    eax, 0ch
        push   0
        push   1
        db  0FFh, 035h, 098h, 000h, 000h, 000h
        push   ebx
        push   eax
        db  0CDh, 020h, 062h, 000h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_6_0370
        mov    ch, byte ptr [esi + 4]
        db  0A1h, 0A0h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_6_02B8
        inc    byte ptr [eax]
loc_6_02B8:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0A1h, 09Ch, 000h, 000h, 000h
        mov    word ptr [ebp + 30h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 34h], ax
        db  0FEh, 005h, 0ACh, 000h, 000h, 000h
        call   sub_6_01DD
        mov    word ptr [ebp + 1ch], 4200h
        db  0A1h, 0F0h, 000h, 000h, 000h
        mov    dword ptr [ebp + 10h], eax
        mov    eax, dword ptr [esi + 0ch]
        shl    eax, 0ch
        mov    dword ptr [ebp + 14h], eax
        shr    eax, 10h
        mov    dword ptr [ebp + 18h], eax
        call   sub_6_0241
        jb     SHORT loc_6_0340
        add    ch, 3fh
        mov    byte ptr [ebp + 1dh], ch
        mov    word ptr [ebp + 18h], 1000h
        db  0A1h, 098h, 000h, 000h, 000h
        shl    eax, 8
        mov    word ptr [ebp + 3ch], ax
        mov    word ptr [ebp + 14h], 0
        call   sub_6_0241
        jb     SHORT loc_6_0340
        cmp    word ptr [ebp + 1ch], 1000h
        jne    SHORT loc_6_0340
        mov    di, 0
loc_6_0340:
        call   sub_6_0215
        db  0FEh, 00Dh, 0ACh, 000h, 000h, 000h
        call   sub_6_01B7
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0A1h, 0A0h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_6_0370
        dec    byte ptr [eax]
loc_6_0370:
        mov    eax, edi
        mov    byte ptr [esi + 6], al
        popad
        ret
        db  000h
_PTEXT ends
        end
