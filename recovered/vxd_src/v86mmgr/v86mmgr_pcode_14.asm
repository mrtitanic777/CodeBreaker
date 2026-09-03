; v86mmgr_pcode_14.asm -- V86MMGR object 14 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  060h, 08Bh, 035h, 0FCh, 011h, 000h, 000h, 0F6h, 044h, 01Eh, 02Ch, 020h, 00Fh, 084h, 0CFh, 000h
        db  000h, 000h, 08Bh, 07Ch, 01Eh, 044h, 00Bh, 0FFh, 074h, 022h, 06Ah, 000h, 057h, 0CDh, 020h, 058h
        db  000h, 001h, 000h, 059h, 059h, 06Ah, 000h, 06Ah, 000h, 050h, 057h, 0CDh, 020h, 056h, 000h, 001h
        db  000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 00Fh, 084h, 0A4h, 000h, 000h, 000h, 08Bh, 07Ch, 01Eh, 05Ch
        db  00Bh, 0FFh, 00Fh, 084h, 099h, 000h, 000h, 000h, 08Dh, 0B7h, 080h, 002h, 000h, 000h, 0B9h, 0FFh
        db  000h, 000h, 000h, 0FCh, 0ADh, 00Bh, 0C0h, 074h, 026h, 081h, 0F9h, 0FFh, 000h, 000h, 000h, 074h
        db  01Eh, 0F6h, 040h, 02Ch, 084h, 075h, 018h, 051h, 06Ah, 000h, 06Ah, 000h, 0FFh, 070h, 028h, 0FFh
        db  030h, 0CDh, 020h, 056h, 000h, 001h, 000h, 083h, 0C4h, 010h, 059h, 00Bh, 0C0h, 074h, 005h, 0E2h
        db  0D2h, 0F8h, 0EBh, 05Dh, 081h, 0E9h, 0FFh, 000h, 000h, 000h, 0F7h, 0D9h, 0E3h, 02Ah, 08Dh, 0B7h
        db  084h, 002h, 000h, 000h, 0EBh, 020h, 0FCh, 0ADh, 00Bh, 0C0h, 074h, 01Ah, 0F6h, 040h, 02Ch, 084h
        db  075h, 014h, 051h, 06Ah, 000h, 06Ah, 000h, 0FFh, 070h, 028h, 0FFh, 030h, 0CDh, 020h, 057h, 000h
        db  001h, 000h, 083h, 0C4h, 010h, 059h, 0E2h, 0DEh, 08Bh, 035h, 0FCh, 011h, 000h, 000h, 08Bh, 07Ch
        db  01Eh, 044h, 00Bh, 0FFh, 074h, 01Ah, 06Ah, 000h, 057h, 0CDh, 020h, 058h, 000h, 001h, 000h, 059h
        db  059h, 06Ah, 000h, 06Ah, 000h, 050h, 057h, 0CDh, 020h, 057h, 000h, 001h, 000h, 083h, 0C4h, 010h
        db  0F9h, 061h, 0C3h, 000h
V86MMGR_VM_RESUME:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        call   sub_14_0100
        jb     SHORT loc_14_00FE
        call   0
        jae    SHORT loc_14_00FE
        db  0E8h, 07Fh, 0FFh, 0F1h, 0FFh
        stc
loc_14_00FE:
        ret
        db  000h
sub_14_0100:
        pushad
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        test   byte ptr [esi + ebx + 2ch], 10h
        je     SHORT loc_14_0138
        mov    edi, dword ptr [esi + ebx + 30h]
        or     edi, edi
        je     SHORT loc_14_0138
        push   0
        push   edi
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   edi
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_14_024E
loc_14_0138:
        mov    edi, dword ptr [esi + ebx + 60h]
        or     edi, edi
        je     NEAR PTR loc_14_027B
        mov    ecx, dword ptr [edi + 18h]
        jecxz  loc_14_0174
        test   byte ptr [esi + ebx + 2eh], 1
        je     SHORT loc_14_0174
        push   0
        push   ecx
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   dword ptr [edi + 18h]
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_14_027A
loc_14_0174:
        test   byte ptr [edi + 14h], 2
        je     SHORT loc_14_0196
        push   100h
        push   0
        push   10h
        push   dword ptr [edi]
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_14_0225
loc_14_0196:
        test   byte ptr [esi + ebx + 2ch], 10h
        je     NEAR PTR loc_14_027B
        lea    esi, [edi + 1ch]
        db  08Bh, 00Dh, 0ACh, 012h, 000h, 000h
        jecxz  loc_14_01D1
loc_14_01AC:
        cld
        lodsd
        or     eax, eax
        jns    SHORT loc_14_01CF
        cmp    dword ptr [eax], 0
        je     SHORT loc_14_01CF
        push   ecx
        push   0
        push   0
        push   dword ptr [eax + 8]
        push   dword ptr [eax]
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        pop    ecx
        or     eax, eax
        je     SHORT loc_14_01D7
loc_14_01CF:
        loop   loc_14_01AC
loc_14_01D1:
        clc
        jmp    NEAR PTR loc_14_027B
loc_14_01D7:
        db  02Bh, 00Dh, 0ACh, 012h, 000h, 000h
        neg    ecx
        jecxz  loc_14_0205
        lea    esi, [edi + 1ch]
loc_14_01E4:
        cld
        lodsd
        or     eax, eax
        jns    SHORT loc_14_0203
        cmp    dword ptr [eax], 0
        je     SHORT loc_14_0203
        push   ecx
        push   0
        push   0
        push   dword ptr [eax + 8]
        push   dword ptr [eax]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
        pop    ecx
loc_14_0203:
        loop   loc_14_01E4
loc_14_0205:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        test   byte ptr [edi + 14h], 2
        je     SHORT loc_14_0225
        push   100h
        push   0
        push   10h
        push   dword ptr [edi]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_14_0225:
        test   byte ptr [esi + ebx + 2ch], 10h
        je     SHORT loc_14_024E
        mov    edi, dword ptr [esi + ebx + 30h]
        or     edi, edi
        je     SHORT loc_14_024E
        push   0
        push   edi
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   edi
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_14_024E:
        mov    edi, dword ptr [esi + ebx + 60h]
        or     edi, edi
        je     SHORT loc_14_027A
        mov    edi, dword ptr [edi + 18h]
        test   byte ptr [esi + ebx + 2eh], 1
        je     SHORT loc_14_027A
        push   0
        push   edi
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   edi
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_14_027A:
        stc
loc_14_027B:
        popad
        ret
        db  000h, 000h, 000h
_PTEXT ends
        end
