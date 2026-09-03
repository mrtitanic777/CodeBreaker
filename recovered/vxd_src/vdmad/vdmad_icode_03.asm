; vdmad_icode_03.asm -- VDMAD object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  068h, 018h, 003h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 026h, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 02Ah, 003h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h
        db  000h, 0BFh, 03Ch, 020h, 000h, 000h, 068h, 074h, 003h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h
        db  000h, 08Bh, 007h, 085h, 0C0h, 075h, 00Dh, 068h, 07Ch, 003h, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 0EBh, 025h, 08Bh, 05Fh, 004h, 08Bh, 04Fh, 008h, 08Bh, 057h, 014h, 068h, 08Ch, 003h
        db  000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh, 047h, 010h, 08Bh, 05Fh, 00Ch, 068h, 0A9h
        db  003h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 083h, 0C7h, 018h, 081h, 0FFh, 0FCh, 020h
        db  000h, 000h, 072h, 0B2h, 068h, 0B7h, 003h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0A1h
        db  068h, 01Eh, 000h, 000h, 068h, 0BBh, 003h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0A1h
        db  00Ch, 021h, 000h, 000h, 08Bh, 01Dh, 008h, 021h, 000h, 000h, 03Bh, 0C3h
loc_3_009C:
        je     SHORT loc_3_00D6
        db  068h, 0D5h, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 0EFh, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A0h, 018h, 021h, 000h, 000h
        db  068h, 009h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A0h, 019h, 021h, 000h, 000h
        db  068h, 022h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        jmp    SHORT loc_3_00E1
loc_3_00D6:
        db  068h, 03Bh, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_3_00E1:
        db  068h, 062h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 066h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h
        je     NEAR PTR loc_3_028E
        db  068h, 09Bh, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A0h, 010h, 020h, 000h, 000h
        db  08Ah, 01Dh, 018h, 020h, 000h, 000h
        db  068h, 0C5h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A0h, 011h, 020h, 000h, 000h
        db  08Ah, 01Dh, 019h, 020h, 000h, 000h
        db  068h, 0F1h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A0h, 012h, 020h, 000h, 000h
        db  08Ah, 01Dh, 01Ah, 020h, 000h, 000h
        db  068h, 01Dh, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 049h, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    ecx, 4
        xor    eax, eax
loc_3_0162:
        push   ecx
        db  068h, 08Ah, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        push   eax
        db  0E8h, 056h, 001h, 0FAh, 0FFh
        mov    ebx, dword ptr [eax + 20h]
        mov    edi, dword ptr [eax + 28h]
        mov    dl, byte ptr [eax + 30h]
        mov    cl, byte ptr [eax + 31h]
        db  068h, 099h, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        pop    eax
        push   eax
        add    al, 4
        db  0E8h, 036h, 001h, 0FAh, 0FFh
        mov    ebx, dword ptr [eax + 20h]
        mov    edi, dword ptr [eax + 28h]
        mov    dl, byte ptr [eax + 30h]
        mov    cl, byte ptr [eax + 31h]
        pop    eax
        db  068h, 0ABh, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        inc    eax
        pop    ecx
        dec    ecx
        jecxz  loc_3_01B3
        jmp    SHORT loc_3_0162
loc_3_01B3:
        db  068h, 0C1h, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 0C5h, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h
        je     NEAR PTR loc_3_028E
        db  0BEh, 06Ch, 01Eh, 000h, 000h
        mov    ecx, 8
loc_3_01DF:
        call   sub_3_028F
        add    esi, 34h
        loop   loc_3_01DF
        db  068h, 0F7h, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 0FBh, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h
        je     NEAR PTR loc_3_028E
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  068h, 02Bh, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 04Eh, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    esi, ebx
        db  003h, 035h, 068h, 01Eh, 000h, 000h
        mov    al, byte ptr [esi + 8]
        mov    bl, byte ptr [esi + 9]
        db  068h, 052h, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  080h, 03Dh, 01Ah, 021h, 000h, 000h, 001h
        jne    SHORT loc_3_028E
        mov    al, byte ptr [esi + 4]
        db  068h, 077h, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    al, byte ptr [esi + 5]
        db  068h, 090h, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    al, byte ptr [esi + 6]
        db  068h, 0A9h, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    al, byte ptr [esi + 7]
        db  068h, 0C2h, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    eax, dword ptr [esi]
        db  068h, 0DBh, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_3_028E:
        ret
sub_3_028F:
        xor    ebx, ebx
        mov    eax, dword ptr [esi]
        db  068h, 0F5h, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_3_02B2
        db  068h, 01Eh, 007h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        jmp    SHORT loc_3_02BD
loc_3_02B2:
        db  068h, 025h, 007h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_3_02BD:
        movzx  eax, byte ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_3_02D2
        db  068h, 02Ch, 007h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        jmp    SHORT loc_3_02E5
loc_3_02D2:
        movzx  eax, byte ptr [esi + 15h]
        or     eax, eax
        je     SHORT loc_3_0317
        db  068h, 040h, 007h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_3_02E5:
        mov    eax, dword ptr [esi + 0ch]
        mov    ebx, dword ptr [esi + 10h]
VDMAD_Reserve_Buffer_Space:
        db  068h, 057h, 007h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    ax, word ptr [esi + 16h]
        mov    bx, word ptr [esi + 18h]
        db  068h, 083h, 007h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    eax, dword ptr [esi + 8]
        db  068h, 0B1h, 007h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_3_0317:
        ret
        db  056h, 044h, 04Dh, 041h
loc_3_031C:
        inc    esp
        and    byte ptr [ebx + 74h], dh
        popad
        db  074h, 065h
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        and    byte ptr [eax + 61h], cl
        outsb
        insb
        and    byte ptr gs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   ebx
        imul   edi, dword ptr [edx + 65h], 20202020h
        and    byte ptr [eax + 68h], dl
        jns    SHORT loc_3_03B7
        imul   esp, dword ptr [ebx + 61h], 2020206ch
        and    byte ptr [ecx + ebp*2 + 6eh], cl
        popad
        db  072h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + ebp*2 + 6dh], dl
        and    byte ptr gs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [edi + 77h], cl
        outsb
        db  065h, 072h, 020h
        push   esi
        dec    ebp
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 74h], dl
        popad
        je     SHORT loc_3_03D6
        or     eax, 6523000ah
        imul   esp, dword ptr fs:[eax], 28002020h
        push   ebp
        outsb
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        sub    dword ptr fs:[6523000ah], ecx
        popad
        db  078h, 020h
        and    byte ptr [eax], ah
        and    esp, dword ptr [ebp + 62h]
        db  078h, 020h
        and    byte ptr [eax], ah
        and    esp, dword ptr [ebp + 63h]
        db  078h, 020h
        and    byte ptr [eax], ah
        and    esp, dword ptr [ebp + 64h]
        db  078h, 020h
        and    byte ptr [eax], ah
        add    byte ptr [ebx], ah
        popad
        db  078h, 020h
        and    byte ptr [eax], ah
        and    esp, dword ptr [ebp + 62h]
        db  078h, 00Dh
        or     al, byte ptr [eax]
loc_3_03B7:
        and    byte ptr [4243000ah], cl
        and    byte ptr [edi + 66h], ch
        db  066h, 073h, 065h
        je     SHORT loc_3_03E5
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popad
        db  078h, 00Dh
        or     al, byte ptr [eax]
        dec    eax
loc_3_03D6:
        db  077h, 020h
        dec    ecx
        outsb
        db  074h, 020h
        imul   bp, word ptr [esp + esi*2 + 65h], 2072h
        and    byte ptr [eax], ah
loc_3_03E5:
        and    byte ptr [eax], ah
        and    byte ptr [edi], bh
        popad
        db  078h, 00Dh
        or     al, byte ptr [eax]
        dec    esi
        js     NEAR PTR loc_3_0467
        and    byte ptr [eax + 77h], cl
        and    byte ptr [ecx + 6eh], cl
        db  074h, 020h
        imul   bp, word ptr [esp + esi*2 + 65h], 2072h
        aas
        bound  edi, qword ptr gs:[eax + 0dh]
        or     al, byte ptr [eax]
        inc    esi
        imul   esi, dword ptr [edx + 73h], 4d442074h
        inc    ecx
        and    byte ptr [ebx + 74h], dh
        popad
        je     SHORT loc_3_048D
        db  073h, 020h
        and    byte ptr [eax], ah
        and    esp, dword ptr [ecx + 6ch]
        or     eax, 6553000ah
        arpl   word ptr [edi + 6eh], bp
        and    byte ptr fs:[ebp + ecx*2 + 41h], al
        and    byte ptr [ebx + 74h], dh
        popad
        db  074h, 075h
        jae    SHORT loc_3_0454
        and    byte ptr [ebx], ah
        popad
        insb
        or     eax, 5056000ah
        dec    ecx
        inc    ebx
        inc    esp
        and    byte ptr [eax + 57h], cl
        and    byte ptr [ecx + 4eh], cl
        push   esp
        and    byte ptr [esi + 6fh], ch
        db  074h, 069h
        imul   sp, word ptr [ebx + 61h], 6974h
        outsd
        outsb
loc_3_0454:
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        push   656b6f6fh
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        inc    ebp
loc_3_0467:
        push   ebx
        inc    ebx
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  071h, 075h
        imul   esi, dword ptr [esp + ebp + 20h], 6120726fh
        outsb
        db  079h, 020h
        arpl   word ptr [eax + 61h], bp
        db  072h, 020h
        db  074h, 06Fh
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[ebx + 6fh], ah
        outsb
        je     SHORT loc_3_04FD
        outsd
        insb
loc_3_048D:
        insb
        db  065h, 072h, 020h
        jae    SHORT loc_3_0507
        popad
        je     SHORT loc_3_04FB
        jae    SHORT loc_3_04D2
        or     eax, 6f43000ah
        outsb
        db  074h, 072h
        outsd
        insb
        insb
        db  065h, 072h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx], dh
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [edx], dh
        or     eax, 7473000ah
        popad
        je     SHORT loc_3_053F
        db  073h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_3_04D2:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [ecx + 6ch]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [edx + 6ch]
        or     eax, 616d000ah
        db  073h, 06Bh
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_3_04FB:
        and    byte ptr [eax], ah
loc_3_04FD:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popad
        insb
loc_3_0507:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [edx + 6ch]
        or     eax, 6572000ah
        db  071h, 075h
        db  065h, 073h, 074h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_3_052C:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [ecx + 6ch]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_3_053F:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [edx + 6ch]
        or     eax, 2020000ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        popad
        db  064h, 072h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        arpl   word ptr [esi + 74h], bp
        and    byte ptr [eax], ah
        and    byte ptr [ebp + 6fh], ch
        and    byte ptr fs:[ebp + 6dh], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        popad
        jb     NEAR PTR loc_3_0597
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        arpl   word ptr [esi + 74h], bp
        and    byte ptr [eax], ah
        and    byte ptr [ebp + 6fh], ch
        and    byte ptr fs:[ebp + 6dh], ah
        or     eax, 6863000ah
        popad
        outsb
        outsb
        insb
        and    byte ptr [ebx], ah
        popad
        insb
        and    byte ptr [eax], ah
loc_3_0597:
        and    byte ptr [eax], al
        and    esp, dword ptr [ebp + 62h]
        js     SHORT loc_3_05BE
        and    esp, dword ptr [ebp + 64h]
        imul   esp, dword ptr [eax], 206c6423h
        and    esp, dword ptr [ebx + 6ch]
        add    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        bound  edi, qword ptr gs:[eax + 20h]
        and    esp, dword ptr [ebp + 64h]
        imul   esp, dword ptr [eax], 206c6423h
        and    esp, dword ptr [ebx + 6ch]
loc_3_05BE:
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        inc    ebp
        push   ebx
        inc    ebx
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  071h, 075h
        imul   esi, dword ptr [esp + ebp + 20h], 6120726fh
        outsb
        jns    SHORT loc_3_05F9
        arpl   word ptr [eax + 61h], bp
        jb     SHORT loc_3_05FE
        db  074h, 06Fh
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[ebx + 68h], ah
        popad
        outsb
        outsb
        insb
        and    byte ptr [ebx + 74h], dh
        popad
        db  074h, 065h
        db  073h, 03Ah
        or     eax, 0d20000ah
loc_3_05F9:
        or     al, byte ptr [eax]
        inc    ebp
        push   ebx
        inc    ebx
loc_3_05FE:
        and    byte ptr [edi + ebp*2 + 20h], dh
        jno    SHORT loc_3_0679
        imul   esi, dword ptr [esp + ebp + 20h], 6120726fh
        outsb
        jns    SHORT loc_3_062F
        arpl   word ptr [eax + 61h], bp
        db  072h, 020h
        je     SHORT loc_3_0685
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[ebx + 75h], al
        db  072h, 056h
        dec    ebp
loc_3_0620:
        and    byte ptr [ebx + 74h], dh
        popad
        db  074h, 065h
        db  073h, 03Ah
        or     eax, 6956000ah
        db  072h, 074h
loc_3_062F:
        jne    SHORT loc_3_0692
        insb
        and    byte ptr [ebp + ecx*2 + 41h], al
        and    byte ptr [ebx + 74h], dl
        popad
        db  074h, 065h
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_3_0661
        inc    ebx
        jne    SHORT loc_3_06B6
        push   esi
        dec    ebp
        and    byte ptr [ebx], ah
        inc    ebp
        inc    edx
        pop    eax
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        insb
        imul   esi, dword ptr [eax + 66h], 3a706f6ch
        and    byte ptr [eax], ah
        arpl   word ptr [edx + esi*2 + 6ch], si
loc_3_0661:
        and    byte ptr [ecx], dh
        and    byte ptr [ebx], ah
        inc    ecx
        dec    esp
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 74h], ah
        db  072h, 06Ch
        and    byte ptr [edx], dh
        and    byte ptr [ebx], ah
        inc    edx
        dec    esp
        or     eax, 5350000ah
loc_3_0679:
        xor    bh, byte ptr [edx]
        inc    esp
        dec    ebp
        inc    ecx
        pop    edi
        push   eax
        push   ebx
        xor    bl, byte ptr [edi + 63h]
        insd
loc_3_0685:
        and    byte ptr fs:[eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [ecx + 6ch]
        or     eax, 5350000ah
loc_3_0692:
        xor    bh, byte ptr [edx]
        inc    esp
        dec    ebp
        inc    ecx
        pop    edi
        db  077h, 072h
        imul   esi, dword ptr [ecx + ebp*2 + 6eh], 20202067h
        and    byte ptr [ebx], ah
        popad
        insb
        or     eax, 5350000ah
        xor    bh, byte ptr [edx]
        inc    esp
        dec    ebp
        inc    ecx
        pop    edi
        bound  edi, qword ptr [ecx + 74h]
        push   eax
loc_3_06B6:
        db  074h, 072h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [ecx + 6ch]
        or     eax, 5350000ah
        xor    bh, byte ptr [edx]
        inc    esp
        dec    ebp
        inc    ecx
        pop    edi
        popad
        db  074h, 061h
        inc    edx
        db  079h, 074h
        db  065h, 073h, 020h
        and    byte ptr [ebx], ah
        popad
        insb
        or     eax, 5350000ah
        xor    bh, byte ptr [edx]
        inc    esp
        dec    ebp
        inc    ecx
        pop    edi
        popad
        db  074h, 061h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popad
        js     SHORT loc_3_0700
        or     al, byte ptr [eax]
        inc    esp
        dec    ebp
        inc    ecx
        and    byte ptr [ebx + 68h], ah
        popad
        outsb
        outsb
        insb
loc_3_0700:
        and    byte ptr [ebx], ah
        and    esp, dword ptr [ecx + 6ch]
        and    byte ptr [eax], ah
        insd
        popad
        db  078h, 03Dh
        and    esp, dword ptr [edx + 78h]
        dec    ebx
        bound  esp, qword ptr [eax]
        and    byte ptr [ebx + 61h], ah
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 3dh
        and    byte ptr [eax], al
        aas
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 6f6e000ah
        outsb
        or     eax, 2020000ah
        and    byte ptr [eax], ah
        db  072h, 065h
        imul   ebp, dword ptr [bx + 6eh], 636f6c20h
        imul   esp, dword ptr [ebp + 64h], 0dh
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        bound  esi, qword ptr [ebp + 66h]
        jb     NEAR PTR loc_3_076B
        imul   esp, dword ptr [eax + 20h], 61232020h
        db  078h, 00Dh
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        db  072h, 065h
        imul   ebp, dword ptr [bx + 6eh], 72646120h
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popad
loc_3_076B:
        js     SHORT loc_3_078D
        and    byte ptr [eax], ah
        db  072h, 065h
        imul   ebp, dword ptr [bx + 6eh], 7a697320h
        and    byte ptr gs:[eax], ah
        and    esp, dword ptr [ebp + 62h]
        db  078h, 00Dh
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    esp
loc_3_0788:
        dec    ebp
        inc    ecx
        and    byte ptr [eax + 61h], dh
loc_3_078D:
        and    byte ptr gs:[bx + si], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popad
        js     SHORT loc_3_07B8
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    esp
        dec    ebp
        inc    ecx
        and    byte ptr [edx + 61h], ah
        db  073h, 065h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        bound  edi, qword ptr [eax + 0dh]
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        outsd
        db  077h, 06Eh
loc_3_07B8:
        db  065h, 072h, 020h
        push   esi
        dec    ebp
loc_3_07BD:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popad
        db  078h, 00Dh
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
