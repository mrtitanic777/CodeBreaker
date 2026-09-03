; xga_pcode_04.asm -- XGA object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

XGA_SYSTEM_EXIT:
        call   7
        clc
        ret
sub_4_0007:
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        je     SHORT loc_4_005C
        db  08Bh, 015h, 0FCh, 000h, 000h, 000h
        inc    dl
        xor    al, al
        out    dx, al
        add    dl, 3
        out    dx, al
        inc    dl
        mov    al, 0ffh
        out    dx, al
        add    dl, 5
        mov    ax, 1550h
        out    dx, ax
        mov    ax, 1450h
        out    dx, ax
        mov    ax, 51h
        out    dx, ax
        mov    ax, 454h
        out    dx, ax
        mov    ax, 70h
        out    dx, ax
        mov    ax, 202ah
        out    dx, ax
        sub    dl, 0ah
        mov    al, 1
        out    dx, al
        mov    edx, 3c3h
        mov    al, 1
        out    dx, al
loc_4_005C:
        ret
loc_4_005D:
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        jne    SHORT loc_4_0070
        db  0C6h, 005h, 064h, 001h, 000h, 000h, 0FFh
loc_4_0070:
        call   7
        ret
loc_4_0076:
        db  0C6h, 005h, 064h, 001h, 000h, 000h, 000h
        ret
loc_4_007E:
        db  03Bh, 01Dh, 0E8h, 000h, 000h, 000h
        je     SHORT loc_4_00AC
        push   edi
        push   esi
        mov    edi, ebx
        db  003h, 03Dh, 0F0h, 000h, 000h, 000h
        mov    word ptr [edi], ax
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  03Bh, 03Dh, 0E8h, 000h, 000h, 000h
        je     SHORT loc_4_00AA
        push   eax
        push   edx
        call   7
        pop    edx
        pop    eax
loc_4_00AA:
        pop    esi
        pop    edi
loc_4_00AC:
        ret
sub_4_00AD:
        inc    ebx
        db  00Dh, 000h, 000h, 03Dh, 00Eh
        add    byte ptr [eax], al
        db  0BAh, 00Fh, 000h, 000h, 0AAh
        adc    al, byte ptr [eax]
        db  000h, 0C0h
        adc    al, byte ptr [eax]
        db  000h, 0D3h
        adc    al, byte ptr [eax]
        db  000h, 0F3h
        adc    al, byte ptr [eax]
        db  000h, 091h, 013h, 000h, 000h, 0B6h
        adc    eax, dword ptr [eax]
        db  000h, 0CFh
        adc    eax, dword ptr [eax]
        db  000h, 0EEh
        adc    al, 0
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx + eax], eax
        add    eax, 0ff011101h
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx + eax], eax
        add    eax, 7010601h
        add    dword ptr [ecx], edx
        add    dword ptr [ecx + eax], edx
        pop    ss
        add    edi, edi
        db  0FFh
loc_4_00FD:
        jge    SHORT loc_4_0101
        add    byte ptr [eax], al
loc_4_0101:
        xor    dword ptr [eax], ecx
loc_4_0103:
        add    byte ptr [edx], 80h
        add    dl, byte ptr [eax + 1]
        db  000h, 0DFh
        add    al, byte ptr [eax]
        add    byte ptr [ecx], dh
        or     byte ptr [eax - 1ffd7ffeh], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 067h, 004h
        add    byte ptr [eax], al
        aaa
        or     byte ptr [eax], ah
        add    esp, dword ptr [eax]
        add    ebx, dword ptr [eax + 2]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    dword ptr [31000003h], eax
        add    al, 0
        add    al, 0
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        db  000h, 005h, 004h, 000h, 000h, 031h
        or     byte ptr [eax], al
        add    al, 0
        add    al, 0
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 08Dh, 006h, 000h, 000h, 037h
        or     byte ptr [eax], al
        add    eax, 4000500h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        add    eax, dword ptr [eax]
        add    byte ptr [ecx], dh
        adc    byte ptr [eax - 1ffafffeh], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], dl
        push   eax
        add    byte ptr [eax], dl
        arpl   word ptr [ecx], dx
        add    byte ptr [edx], dl
        dec    edi
        adc    eax, dword ptr [eax]
        adc    al, 4fh
        adc    eax, 17631600h
        add    byte ptr [eax], bl
        push   ebp
        sbb    dword ptr [eax], eax
        sbb    ah, byte ptr [ecx + 1bh]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        shl    byte ptr [ecx], 1
        and    cl, byte ptr [edi - 70dbfeddh]
        and    eax, 27c02601h
        add    dword ptr [eax], ebp
        pushfd
        sub    dword ptr [ecx], eax
        sub    bl, byte ptr [esi - 0d200d4h]
        add    byte ptr ss:[eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        push   eax
        inc    esp
        add    byte ptr [eax + eax + 51h], dl
        add    esi, dword ptr [eax]
        push   eax
        inc    edi
        push   ebp
        add    byte ptr [edi + edi*8 + 32h], ah
        add    byte ptr [39003800h], dh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_02DF:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ebx + 11h], ah
        add    byte ptr [edx], dl
        dec    edi
        adc    eax, dword ptr [eax]
        adc    al, 4fh
        adc    eax, 17631600h
        add    byte ptr [eax], bl
        push   ebp
        sbb    dword ptr [eax], eax
        sbb    ah, byte ptr [ecx + 1bh]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        or     al, 21h
        add    ah, byte ptr [edx]
        fbld   tbyte ptr [ebx]
        add    dword ptr [edi + ebx*8], esp
        and    eax, 270c2601h
        add    ch, byte ptr [eax]
        ljmp   0ff2ch:0ec2a0129h
        sub    eax, 400036ffh
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx + 50h], al
        inc    esp
        add    byte ptr [eax + eax + 51h], dl
        add    esi, dword ptr [eax]
        push   eax
        db  0C7h, 055h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h, 000h, 039h, 000h, 03Ah, 000h, 03Bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_0341:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ebx + 11h], ah
        add    byte ptr [edx], dl
        dec    edi
        adc    eax, dword ptr [eax]
        adc    al, 4fh
        adc    eax, 17631600h
        add    byte ptr [eax], bl
        push   ebp
        sbb    dword ptr [eax], eax
        sbb    ah, byte ptr [ecx + 1bh]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        or     al, 21h
        add    ah, byte ptr [edx]
        fbld   tbyte ptr [ebx]
        add    dword ptr [edi + ebx*8], esp
        and    eax, 270c2601h
        add    ch, byte ptr [eax]
        ljmp   0ff2ch:0ec2a0129h
        sub    eax, 400036ffh
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx - 60h], al
        inc    esp
        add    byte ptr [eax + eax + 51h], dl
        add    al, 70h
        add    byte ptr [eax - 39h], dl
        push   ebp
        add    byte ptr [edi + edi*8 + 32h], ah
        add    byte ptr [39003800h], dh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_03A3:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ebp + 7f120011h], bl
        adc    eax, dword ptr [eax]
        adc    al, 7fh
        adc    eax, 179d1600h
        add    byte ptr [eax], bl
        xchg   dword ptr [ecx], ebx
        add    byte ptr [edx], bl
        pushfd
        sbb    eax, dword ptr [eax]
        sbb    al, 40h
        push   ds
        add    al, 20h
        xor    byte ptr [ecx], ah
        add    esp, dword ptr [edx]
        jmp    dword ptr [ebx]
        db  002h, 024h, 0FFh, 025h, 002h, 026h, 030h, 027h, 003h, 028h, 000h, 029h, 003h, 02Ah, 008h, 02Ch
        db  0FFh, 02Dh, 0FFh, 036h, 000h, 040h, 000h, 041h, 000h, 042h, 000h, 043h, 040h, 044h, 000h, 054h
        db  00Dh, 051h, 002h, 070h, 000h, 050h, 00Fh, 055h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h
        db  000h, 039h, 000h, 03Ah, 000h, 03Bh, 0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_0405:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ebp + 7f120011h], bl
        adc    eax, dword ptr [eax]
        adc    al, 7fh
        adc    eax, 179d1600h
        add    byte ptr [eax], bl
        xchg   dword ptr [ecx], ebx
        add    byte ptr [edx], bl
        pushfd
        sbb    eax, dword ptr [eax]
        sbb    al, 40h
        push   ds
        add    al, 20h
        xor    byte ptr [ecx], ah
        add    esp, dword ptr [edx]
        jmp    dword ptr [ebx]
        db  002h, 024h, 0FFh, 025h, 002h, 026h, 030h, 027h, 003h, 028h, 000h, 029h, 003h, 02Ah, 008h, 02Ch
        db  0FFh, 02Dh, 0FFh, 036h, 000h, 040h, 000h, 041h, 000h, 042h, 000h, 043h, 080h, 044h, 000h, 054h
        db  00Dh, 051h, 003h, 070h, 000h, 050h, 00Fh, 055h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h
        db  000h, 039h, 000h, 03Ah, 000h, 03Bh, 0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_0467:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ebx + 63120011h], al
        adc    eax, dword ptr [eax]
        adc    al, 63h
        adc    eax, 17831600h
        add    byte ptr [eax], bl
        imul   ebx, dword ptr [ecx], 0
        sbb    bh, byte ptr [ebx + 1bh]
        add    byte ptr [eax], bl
        push   ds
        add    ah, byte ptr [eax]
        jae    SHORT loc_4_04AB
        add    ah, byte ptr [edx]
        push   edi
        and    eax, dword ptr [edx]
        and    al, 57h
        and    eax, 27732602h
        add    ch, byte ptr [eax]
        pop    eax
        sub    dword ptr [edx], eax
        sub    bl, byte ptr [esp + ebp - 1]
        sub    eax, 400036ffh
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx + 64h], al
loc_4_04AB:
        inc    esp
        add    byte ptr [ecx + eax + 70h], dl
        add    byte ptr [ecx + 3], dl
        pop    eax
        dec    edi
        push   esp
        adc    byte ptr [eax + 7], 55h
        add    byte ptr [eax], ah
        popad
        add    byte ptr [edx], ah
        arpl   word ptr [eax], ax
        push   dword ptr fs:[edx]
        add    byte ptr [39003800h], dh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_04D5:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ecx + 63120011h], al
        adc    eax, dword ptr [eax]
        adc    al, 63h
        adc    eax, 17811600h
        add    byte ptr [eax], bl
        db  070h, 019h
        add    byte ptr [edx], bl
        db  07Fh, 01Bh
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        cdq
        and    dword ptr [edx], eax
        and    dl, byte ptr [edi + 23h]
        add    ah, byte ptr [edi + edx*2]
        and    eax, 27992602h
        add    ch, byte ptr [eax]
        jl     SHORT loc_4_0531
        add    ch, byte ptr [edx]
        sub    byte ptr [edi + edi*8], 2dh
        push   dword ptr [esi]
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        inc    esp
        add    byte ptr [ecx + eax + 70h], dl
        add    byte ptr [ecx + 3], dl
        pop    eax
        arpl   word ptr [ecx + eax*4 + 50h], dx
        pop    es
        push   ebp
        add    byte ptr [eax], ah
        popad
        add    byte ptr [edx], ah
        arpl   word ptr [eax], ax
loc_4_0531:
        push   dword ptr fs:[edx]
        add    byte ptr [39003800h], dh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_0543:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [edi + 7f120011h], ah
        adc    eax, dword ptr [eax]
        adc    al, 7fh
        adc    eax, 17a71600h
        add    byte ptr [eax], bl
        mov    byte ptr [ecx], bl
        add    byte ptr [edx], bl
        cdq
        sbb    eax, dword ptr [eax]
        sbb    al, 0
        push   ds
        add    byte ptr [eax], ah
        and    eax, 0ff220321h
        and    eax, dword ptr [edx]
        and    al, 0ffh
        and    eax, 27252602h
        add    ebp, dword ptr [eax]
        add    ch, byte ptr [ecx]
        add    ebp, dword ptr [edx]
        or     byte ptr [edi + edi*8], ch
        sub    eax, 400036ffh
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx - 80h], al
        inc    esp
        add    byte ptr [ecx + eax + 70h], dl
        add    byte ptr [ecx + 3], dl
        pop    eax
        adc    byte ptr [ecx + eax*4 + 50h], 0c7h
        push   ebp
        add    byte ptr [eax], ah
        popad
        add    byte ptr [edx], ah
        arpl   word ptr [eax], ax
        push   dword ptr fs:[edx]
        add    byte ptr [39003800h], dh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_05B1:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ecx + 7f120011h], ch
        adc    eax, dword ptr [eax]
        adc    al, 7fh
        adc    eax, 17a91600h
        add    byte ptr [eax], bl
        xchg   byte ptr [ecx], bl
        add    byte ptr [edx], bl
        test   eax, 1c001bh
        push   ds
        add    byte ptr [eax], ah
        and    dword ptr es:[ebx], eax
        and    bh, bh
        and    eax, dword ptr [edx]
        and    al, 0ffh
        and    eax, 27262602h
        add    ebp, dword ptr [eax]
        jmp    ptr [ecx]
        db  002h, 02Ah, 007h, 02Ch, 0FFh, 02Dh, 0FFh, 036h, 000h, 040h, 000h, 041h, 000h, 042h, 000h, 043h
        db  080h, 044h, 000h, 054h, 001h, 070h, 000h, 051h, 003h, 058h, 08Eh, 054h, 081h, 050h, 007h, 055h
        db  000h, 060h, 000h, 061h, 000h, 062h, 000h, 063h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h
        db  000h, 039h, 000h, 03Ah, 000h, 03Bh, 0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_061F:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [edi + 7f120011h], ch
        adc    eax, dword ptr [eax]
        adc    al, 7fh
        adc    eax, 17af1600h
        add    byte ptr [eax], bl
        xchg   byte ptr [ecx], bl
        add    byte ptr [edx], bl
        scasb
        sbb    eax, dword ptr [eax]
        sbb    al, 0
        push   ds
        add    byte ptr [eax], ah
        and    eax, 0ff220321h
        and    eax, dword ptr [edx]
        and    al, 0ffh
        and    eax, 27252602h
        add    ebp, dword ptr [eax]
        jmp    ptr [ecx]
        db  002h, 02Ah, 007h, 02Ch, 0FFh, 02Dh, 0FFh, 036h, 000h, 040h, 000h, 041h, 000h, 042h, 000h, 043h
        db  080h, 044h, 000h, 054h, 001h, 070h, 000h, 051h, 003h, 058h, 095h, 054h, 081h, 050h, 007h, 055h
        db  000h, 060h, 000h, 061h, 000h, 062h, 000h, 063h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h
        db  000h, 039h, 000h, 03Ah, 000h, 03Bh, 0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_068D:
        push   eax
        add    dword ptr [eax], edx
        adc    cl, dl
        adc    dword ptr [eax], eax
        adc    bl, byte ptr [edi - 60ebffedh]
        adc    eax, 17d11600h
        add    byte ptr [eax], bl
        cmpsd
        sbb    dword ptr [eax], eax
        sbb    cl, bh
        sbb    eax, dword ptr [eax]
        sbb    al, 0
        push   ds
        add    byte ptr [eax], ah
        inc    esp
        and    dword ptr [edx], eax
        jmp    dword ptr [ebx]
        db  003h, 024h, 0FFh, 025h, 003h, 026h, 044h, 027h, 004h, 028h, 002h, 029h, 004h, 02Ah, 00Ah, 02Ch
        db  0FFh, 02Dh, 0FFh, 036h, 000h, 040h, 000h, 041h, 000h, 042h, 000h, 043h, 0A0h, 044h, 000h, 054h
        db  001h, 070h, 000h, 051h, 003h, 058h, 091h, 054h, 081h, 050h, 04Fh, 055h, 000h, 060h, 000h, 061h
        db  000h, 062h, 000h, 063h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h, 000h, 039h, 000h, 03Ah
        db  000h, 03Bh, 0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_06FB:
        jge    SHORT loc_4_06FF
        add    byte ptr [eax], al
loc_4_06FF:
        xor    dword ptr [eax], ecx
loc_4_0701:
        add    byte ptr [edx], 80h
        add    dl, byte ptr [eax + 1]
        db  000h, 07Bh, 008h
        add    byte ptr [eax], al
        sub    eax, 28008h
        add    al, 0e0h
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 02Fh
        or     dword ptr [eax], eax
        add    byte ptr [32008h], ch
        add    al, 58h
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        db  000h, 0E3h
        or     dword ptr [eax], eax
        add    byte ptr [40004h], ch
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        db  000h, 03Dh, 00Ah, 000h, 000h, 02Dh
        or     byte ptr [eax], al
        add    al, 0
        add    al, 0
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        db  000h, 0F1h
        or     al, byte ptr [eax]
        add    byte ptr [80050008h], ch
        add    al, byte ptr [eax]
        add    al, 0
        add    byte ptr [eax], al
        db  000h, 04Bh, 00Bh
        add    byte ptr [eax], al
        sub    eax, 50008h
        or     al, al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0D5h
        or     byte ptr [eax], al
        add    byte ptr [28010h], ch
        or     al, ah
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 089h, 009h, 000h, 000h, 02Dh
        adc    byte ptr [eax], ah
        add    eax, dword ptr [eax + 6]
        pop    eax
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 097h, 00Ah, 000h, 000h, 02Dh
        adc    byte ptr [eax], al
        add    al, 0
        or     byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], dl
        push   eax
        add    byte ptr [eax], dl
        pop    edi
        adc    dword ptr [eax], eax
        adc    cl, byte ptr [edi + 13h]
        add    byte ptr [edi + ecx*2], dl
        adc    eax, 175f1600h
        add    byte ptr [eax], bl
        push   ebp
        sbb    dword ptr [eax], eax
        sbb    ah, byte ptr [ecx + 1bh]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        or     al, 21h
        add    ah, byte ptr [edx]
        fbld   tbyte ptr [ebx]
        add    dword ptr [edi + ebx*8], esp
        and    eax, 270c2601h
        add    ch, byte ptr [eax]
        ljmp   0ff2ch:0ec2a0129h
        sub    eax, 400036ffh
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx - 80h], al
        inc    esp
        add    byte ptr [eax + eax + 51h], dl
        add    esi, dword ptr [eax]
        push   eax
        db  0C7h, 055h, 000h, 07Fh, 030h, 06Dh, 003h, 06Eh, 0DFh, 06Fh, 041h, 077h, 004h
loc_4_08D5:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [edi + 11h], bl
        add    byte ptr [edx], dl
        dec    edi
        adc    eax, dword ptr [eax]
        adc    al, 4fh
        adc    eax, 175f1600h
        add    byte ptr [eax], bl
        push   ebp
        sbb    dword ptr [eax], eax
        sbb    ah, byte ptr [ecx + 1bh]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        or     al, 21h
        add    ah, byte ptr [edx]
        fbld   tbyte ptr [ebx]
        add    dword ptr [edi + ebx*8], esp
        and    eax, 270c2601h
        add    ch, byte ptr [eax]
        ljmp   0ff2ch:0ec2a0129h
        sub    eax, 400036ffh
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx], al
        inc    esp
        add    dword ptr [ebp + ecx + 51h], edx
        add    al, 70h
        add    byte ptr [eax - 39h], dl
        push   ebp
        add    byte ptr [edi + 30h], bh
        insd
        add    ebp, dword ptr [esi + 3fh]
        outsd
        push   ebp
        db  077h, 022h
loc_4_092F:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [eax + 63120011h], al
        adc    eax, dword ptr [eax]
        adc    al, 63h
        adc    eax, 17801600h
        add    byte ptr [eax], bl
        outsb
        sbb    dword ptr [eax], eax
        sbb    bh, byte ptr [eax + 1bh]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        jae    SHORT loc_4_0973
        add    ah, byte ptr [edx]
        push   edi
        and    eax, dword ptr [edx]
        and    al, 57h
        and    eax, 27732602h
        add    ch, byte ptr [eax]
        pop    eax
        sub    dword ptr [edx], eax
        sub    bl, byte ptr [esp + ebp]
        jmp    ptr [40003607h]
        db  000h, 041h, 000h, 042h, 000h, 043h, 080h
loc_4_0973:
        inc    esp
        add    byte ptr [ebp + ecx + 51h], dl
        add    esi, dword ptr [eax]
        push   eax
        pop    es
        push   ebp
        add    byte ptr [edi + 12h], bh
        insd
        add    ebp, dword ptr [esi - 61h]
        outsd
        push   ecx
        ja     SHORT loc_4_098D
loc_4_0989:
        push   eax
        add    dword ptr [eax], edx
loc_4_098D:
        adc    ah, bh
        adc    dword ptr [eax], eax
        adc    al, bh
        adc    eax, dword ptr [eax]
        adc    al, 0c7h
        adc    eax, 17fc1600h
        add    byte ptr [eax], bl
        rcr    dword ptr [ecx], 1
        add    byte ptr [edx], bl
        fstp   qword ptr [ebx]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        and    byte ptr [ecx], 2
        and    dl, byte ptr [edi + 23h]
        add    ah, byte ptr [edi + edx*2]
        and    eax, 27802602h
        add    ch, byte ptr [eax]
        pop    esi
        sub    dword ptr [edx], eax
        sub    cl, byte ptr [0ff2dff2ch]
        add    byte ptr ss:[eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        enter  44h, 54h
        or     eax, 700351h
        push   eax
        pop    es
        push   ebp
        add    byte ptr [edi + 12h], bh
        insd
        add    ebp, dword ptr [esi - 61h]
        outsd
        adc    eax, 1500077h
        push   eax
        add    byte ptr [eax], dl
        movsb
        adc    dword ptr [eax], eax
        adc    bh, byte ptr [edi + 13h]
        add    byte ptr [edi + edi*2], dl
        adc    eax, 17a41600h
        add    byte ptr [eax], bl
        mov    bl, byte ptr [ecx]
        add    byte ptr [edx], bl
        xchg   ebx, eax
        sbb    eax, dword ptr [eax]
        sbb    al, 40h
        push   ds
        add    al, 20h
        and    eax, 0ff220321h
        and    eax, dword ptr [edx]
        and    al, 0ffh
        and    eax, 27252602h
        add    ebp, dword ptr [eax]
        add    byte ptr [ecx], ch
        add    ebp, dword ptr [edx]
        or     byte ptr [edi + edi*8], ch
        sub    eax, 400036ffh
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx + 40h], al
        inc    esp
        add    byte ptr [ebp + ecx + 51h], dl
        add    dh, byte ptr [eax]
        push   eax
        andnps xmm0, xmmword ptr [eax]
        jg     SHORT loc_4_0A67
        insd
        add    ebp, dword ptr [esi + 1fh]
        outsd
        push   ebp
        db  077h, 034h
loc_4_0A3D:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ecx + edx + 137f1200h], ah
        add    byte ptr [edi + edi*2], dl
        adc    eax, 17a41600h
        add    byte ptr [eax], bl
        mov    bl, byte ptr [ecx]
        add    byte ptr [edx], bl
        xchg   ebx, eax
        sbb    eax, dword ptr [eax]
        sbb    al, 40h
        push   ds
        add    al, 20h
        and    eax, 0ff220321h
        and    eax, dword ptr [edx]
        and    al, 0ffh
loc_4_0A67:
        and    eax, 27252602h
        add    ebp, dword ptr [eax]
        add    byte ptr [ecx], ch
        add    ebp, dword ptr [edx]
        or     byte ptr [edi + edi*8], ch
        sub    eax, 400036ffh
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx - 80h], al
        inc    esp
        add    byte ptr [ebp + ecx + 51h], dl
        add    esi, dword ptr [eax]
        push   eax
        pop    ss
        push   ebp
        add    byte ptr [edi + 32h], bh
        insd
        add    ebp, dword ptr [esi + 5fh]
        outsd
        db  065h, 077h, 034h
loc_4_0A97:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [edi + 7f120011h], ah
        adc    eax, dword ptr [eax]
        adc    al, 7fh
        adc    eax, 17a71600h
        add    byte ptr [eax], bl
        sbb    byte ptr [ecx], 0
        sbb    dl, byte ptr [ebx + 401c001bh]
        push   ds
        add    al, 20h
        and    eax, 0ff220321h
        and    eax, dword ptr [edx]
        and    al, 0ffh
        and    eax, 27252602h
        add    ebp, dword ptr [eax]
        add    ch, byte ptr [ecx]
        add    ebp, dword ptr [edx]
        or     byte ptr [edi + edi*8], ch
        sub    eax, 40003607h
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx], al
        inc    esp
        add    dword ptr [ebp + ecx + 51h], edx
        add    al, 70h
        add    byte ptr [eax + 0bh], dl
        push   ebp
        add    byte ptr [edi], bh
        insd
        add    ebp, dword ptr [esi + 5ch]
        outsd
        db  074h, 077h
        push   eax
        add    dword ptr [eax], edx
        adc    ah, cl
        adc    dword ptr [eax], eax
        adc    bl, byte ptr [edi - 60ebffedh]
        adc    eax, 17cc1600h
        add    byte ptr [eax], bl
        mov    al, 19h
        add    byte ptr [edx], bl
        mov    esi, 401c001bh
        push   ds
        add    byte ptr [eax], ah
        db  0E0h, 021h
        add    esp, dword ptr [edx]
        mov    edi, 0bf240323h
        and    eax, 27e02603h
        add    ebp, dword ptr [eax]
        shr    dword ptr [ecx], 3
        sub    al, byte ptr [72dff2ch]
        add    byte ptr ss:[eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        add    byte ptr [ecx + eax + 54h], al
        or     eax, 700351h
        push   eax
        add    byte ptr [ebp], dl
        db  07Fh, 036h
        insd
        add    ebp, dword ptr [esi + 1]
        outsd
        ja     SHORT loc_4_0BC1
        adc    al, 50h
        add    dword ptr [eax], edx
        adc    ah, cl
        adc    dword ptr [eax], eax
        adc    bl, byte ptr [edi - 60ebffedh]
        adc    eax, 17cc1600h
        add    byte ptr [eax], bl
        mov    al, 19h
        add    byte ptr [edx], bl
        mov    esi, 401c001bh
        push   ds
        add    byte ptr [eax], ah
        db  0E0h, 021h
        add    esp, dword ptr [edx]
        mov    edi, 0bf240323h
        and    eax, 27e02603h
        add    ebp, dword ptr [eax]
        shr    dword ptr [ecx], 3
        sub    al, byte ptr [72dff2ch]
        add    byte ptr ss:[eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        add    byte ptr [ecx + eax + 54h], al
        or     eax, 700351h
        push   eax
        add    byte ptr [ebp], dl
        jg     SHORT loc_4_0BD3
        insd
        add    ebp, dword ptr [esi + 1]
        outsd
        ja     SHORT loc_4_0C1B
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        sub    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0BC1:
        add    byte ptr [eax], cl
        add    byte ptr [eax], dl
        add    byte ptr [eax], bl
        add    byte ptr [eax], ah
        add    byte ptr [eax], ch
        add    byte ptr [eax], dh
loc_4_0BD3:
        add    byte ptr [eax], bh
        add    byte ptr [eax + 880000h], al
        add    byte ptr [eax + 980000h], dl
        add    byte ptr [eax + 0a80000h], ah
        add    byte ptr [eax + 0b80000h], dh
        add    al, al
        add    byte ptr [eax], al
        enter  0, -30h
        add    byte ptr [eax], al
        fadd   dword ptr [eax]
        add    al, ah
        add    byte ptr [eax], al
        db  0E8h, 000h, 000h, 0F0h, 000h
        add    al, bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        xor    byte ptr [eax], al
loc_4_0C1B:
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], al
        push   eax
        add    byte ptr [eax], al
        pop    eax
        add    byte ptr [eax], al
        pushad
        add    byte ptr [eax], al
        push   700000h
        add    byte ptr [eax], bh
        add    byte ptr [eax + 880000h], al
        add    byte ptr [eax + 980000h], dl
        add    byte ptr [eax + 0a80000h], ah
        add    byte ptr [eax + 0b80000h], dh
        add    al, al
        add    byte ptr [eax], al
        enter  0, -30h
        add    byte ptr [eax], al
        fadd   dword ptr [eax]
        add    al, ah
        add    byte ptr [eax], al
        db  0E8h, 000h, 000h, 0F0h, 000h
        add    al, bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        xor    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], al
        push   eax
        add    byte ptr [eax], al
        pop    eax
        add    byte ptr [eax], al
        pushad
        add    byte ptr [eax], al
        push   700000h
        add    byte ptr [eax], bh
        add    byte ptr [eax + 880000h], al
        add    byte ptr [eax + 980000h], dl
        add    byte ptr [eax + 0a80000h], ah
        add    byte ptr [eax + 0b80000h], dh
        add    al, al
        add    byte ptr [eax], al
        enter  0, -30h
        add    byte ptr [eax], al
        fadd   dword ptr [eax]
        add    al, ah
        add    byte ptr [eax], al
        db  0E8h, 000h, 000h, 0F0h, 000h
        add    al, bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        xor    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], al
        push   eax
        add    byte ptr [eax], al
        pop    eax
        add    byte ptr [eax], al
        pushad
        add    byte ptr [eax], al
        push   700000h
        add    byte ptr [eax], bh
        add    byte ptr [eax + 880000h], al
        add    byte ptr [eax + 980000h], dl
        add    byte ptr [eax + 0a80000h], ah
        add    byte ptr [eax + 0b80000h], dh
        add    al, al
        add    byte ptr [eax], al
        enter  0, -30h
        add    byte ptr [eax], al
        fadd   dword ptr [eax]
        add    al, ah
        add    byte ptr [eax], al
        db  0E8h, 000h, 000h, 0F0h, 000h
        add    al, bh
loc_4_0D25:
        pushad
        cmp    al, 13h
        je     SHORT loc_4_0D41
        cmp    al, 0ah
        ja     SHORT loc_4_0D3A
        movzx  eax, al
        db  0FFh, 014h, 085h, 0ADh, 000h, 000h, 000h
        jmp    SHORT loc_4_0D40
loc_4_0D3A:
        mov    word ptr [ebp + 1ch], 100h
loc_4_0D40:
        stc
loc_4_0D41:
        popad
        ret
loc_4_0D43:
        mov    ah, 38h
        mov    al, 0
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     NEAR PTR loc_4_0E36
        cld
        mov    cl, 1
        mov    edx, dword ptr [eax]
        cmp    dl, 56h
        je     SHORT loc_4_0D65
        cmp    dl, 76h
        jne    SHORT loc_4_0D83
loc_4_0D65:
        cmp    dh, 42h
        je     SHORT loc_4_0D6F
        cmp    dh, 62h
        jne    SHORT loc_4_0D83
loc_4_0D6F:
        shr    edx, 10h
        cmp    dl, 45h
        je     SHORT loc_4_0D7C
        cmp    dl, 65h
        jne    SHORT loc_4_0D83
loc_4_0D7C:
        cmp    dh, 32h
        jne    SHORT loc_4_0D83
        mov    cl, 2
loc_4_0D83:
        mov    dword ptr [eax], 41534556h
        mov    word ptr [eax + 4], 200h
        mov    si, word ptr [ebp + 38h]
        shl    esi, 10h
        lea    edx, [eax + 14h]
        mov    si, 14h
        cmp    cl, 1
        je     SHORT loc_4_0DAC
        lea    edx, [eax + 100h]
        mov    si, 100h
loc_4_0DAC:
        mov    dword ptr [eax + 6], esi
        mov    dword ptr [edx], 414758h
        mov    byte ptr [edx + 3], 0
        mov    dword ptr [eax + 0ah], 2
        push   ecx
        mov    si, word ptr [ebp + 38h]
        shl    esi, 10h
        lea    edi, [eax + 18h]
        mov    si, 18h
        cmp    cl, 1
        je     SHORT loc_4_0DDB
        lea    edi, [eax + 22h]
        mov    si, 22h
loc_4_0DDB:
        mov    dword ptr [eax + 0eh], esi
        db  0BEh, 0D9h, 000h, 000h, 000h
        mov    ecx, 0eh
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        jne    SHORT loc_4_0DFE
        db  0BEh, 0E7h, 000h, 000h, 000h
        mov    ecx, 16h
loc_4_0DFE:
        rep movsb
        pop    ecx
        db  08Bh, 015h, 0F4h, 000h, 000h, 000h
        shr    edx, 10h
        mov    word ptr [eax + 12h], dx
        cmp    cl, 1
        je     SHORT loc_4_0E2E
        mov    word ptr [eax + 14h], 200h
        mov    dword ptr [eax + 16h], 0
        mov    dword ptr [eax + 1ah], 0
        mov    dword ptr [eax + 1eh], 0
loc_4_0E2E:
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_0E3C
loc_4_0E36:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_0E3C:
        ret
loc_4_0E3D:
        mov    ah, 38h
        mov    al, 0
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     NEAR PTR loc_4_0FB3
        cld
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 40h
        rep stosd
        mov    ax, word ptr [ebp + 18h]
        and    ax, 3f7fh
        mov    ecx, 6
        db  0BFh, 0D9h, 000h, 000h, 000h
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        jne    SHORT loc_4_0E85
        mov    ecx, 0ah
        db  0BFh, 0E7h, 000h, 000h, 000h
loc_4_0E85:
        repne scasw
        movzx  ecx, ax
        pop    eax
        jne    NEAR PTR loc_4_0FB3
        or     word ptr [eax], 3bh
        cmp    cl, 3
        jne    SHORT loc_4_0EAC
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0DBh, 08Fh, 000h, 000h
        jne    SHORT loc_4_0EAC
loc_4_0EA7:
        and    byte ptr [eax], 0feh
        jmp    SHORT loc_4_0EE9
loc_4_0EAC:
        cmp    cl, 4
        jbe    SHORT loc_4_0EE9
        db  081h, 03Dh, 0F4h, 000h, 000h, 000h, 000h, 000h, 010h, 000h
        jb     SHORT loc_4_0EA7
        cmp    cl, 7
        je     SHORT loc_4_0ECC
        cmp    cl, 14h
        je     SHORT loc_4_0ECC
        cmp    cl, 17h
        jne    SHORT loc_4_0EE9
loc_4_0ECC:
        db  081h, 03Dh, 0F4h, 000h, 000h, 000h, 000h, 000h, 020h, 000h
        jb     SHORT loc_4_0EA7
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        je     SHORT loc_4_0EE9
        cmp    cl, 11h
        ja     SHORT loc_4_0EA7
loc_4_0EE9:
        mov    byte ptr [eax + 2], 7
        mov    byte ptr [eax + 3], 0
        mov    word ptr [eax + 4], 40h
        mov    word ptr [eax + 6], 40h
        mov    word ptr [eax + 8], 0a000h
        mov    word ptr [eax + 0ah], 0
        db  08Bh, 035h, 0F8h, 000h, 000h, 000h
        mov    dword ptr [eax + 0ch], esi
        movzx  esi, cl
        shl    esi, 4
        db  0BFh, 0FDh, 000h, 000h, 000h
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        jne    SHORT loc_4_0F2E
        db  0BFh, 0FBh, 006h, 000h, 000h
loc_4_0F2E:
        add    edi, esi
        mov    dx, word ptr [edi + 6]
        mov    word ptr [eax + 12h], dx
        mov    dx, word ptr [edi + 8]
        mov    word ptr [eax + 10h], dx
        mov    dx, word ptr [edi + 0ah]
        mov    word ptr [eax + 14h], dx
        mov    dl, byte ptr [edi + 5]
        mov    byte ptr [eax + 19h], dl
        mov    byte ptr [eax + 16h], 0
        mov    byte ptr [eax + 17h], 0
        mov    byte ptr [eax + 18h], 1
        mov    byte ptr [eax + 1bh], 4
        mov    byte ptr [eax + 1ah], 1
        mov    byte ptr [eax + 1ch], 0
        mov    byte ptr [eax + 1dh], 0
        mov    byte ptr [eax + 1eh], 1
        cmp    cl, 11h
        jne    SHORT loc_4_0F8B
        mov    byte ptr [eax + 1fh], 5
        mov    byte ptr [eax + 20h], 0bh
        mov    byte ptr [eax + 21h], 6
        mov    byte ptr [eax + 22h], 5
        mov    byte ptr [eax + 23h], 5
        mov    byte ptr [eax + 24h], 0
loc_4_0F8B:
        push   eax
        movzx  edx, word ptr [eax + 10h]
        movzx  eax, word ptr [eax + 14h]
        mul    edx
        mov    edx, eax
        pop    eax
        mov    dword ptr [eax + 2ch], edx
        db  08Bh, 00Dh, 0F4h, 000h, 000h, 000h
        sub    ecx, edx
        shr    ecx, 0ah
        mov    word ptr [eax + 30h], cx
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_0FB9
loc_4_0FB3:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_0FB9:
        ret
loc_4_0FBA:
        cld
        movzx  eax, word ptr [ebp + 10h]
        test   ah, 4
        jne    NEAR PTR loc_4_12A3
        and    ax, 3f7fh
        cmp    ax, 13h
        jbe    NEAR PTR loc_4_128A
        mov    ecx, 6
        db  0BFh, 0D9h, 000h, 000h, 000h
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        jne    SHORT loc_4_0FF6
        mov    ecx, 0ah
        db  0BFh, 0E7h, 000h, 000h, 000h
loc_4_0FF6:
        repne scasw
        jne    NEAR PTR loc_4_12A3
        cmp    al, 3
        jne    SHORT loc_4_1013
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0DBh, 08Fh, 000h, 000h
        je     NEAR PTR loc_4_12A3
loc_4_1013:
        cmp    al, 4
        jbe    SHORT loc_4_1057
        db  081h, 03Dh, 0F4h, 000h, 000h, 000h, 000h, 000h, 010h, 000h
        jb     NEAR PTR loc_4_12A3
        cmp    al, 7
        je     SHORT loc_4_1033
        cmp    al, 14h
        je     SHORT loc_4_1033
        cmp    al, 17h
        jne    SHORT loc_4_1057
loc_4_1033:
        db  081h, 03Dh, 0F4h, 000h, 000h, 000h, 000h, 000h, 020h, 000h
        jb     NEAR PTR loc_4_12A3
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        je     SHORT loc_4_1057
        cmp    al, 11h
        ja     NEAR PTR loc_4_12A3
loc_4_1057:
        sub    eax, 100h
        shl    eax, 4
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        jne    SHORT loc_4_108A
        mov    cl, byte ptr [ebp + 10h]
        cmp    cl, 4
        je     SHORT loc_4_108A
        cmp    cl, 5
        je     SHORT loc_4_108A
        db  08Bh, 0B0h, 0FBh, 006h, 000h, 000h
        db  00Fh, 0B6h, 088h, 0FFh, 006h, 000h, 000h
        jmp    NEAR PTR loc_4_111D
loc_4_108A:
        db  08Bh, 0B0h, 0FDh, 000h, 000h, 000h
        db  00Fh, 0B6h, 088h, 001h, 001h, 000h, 000h
        db  03Bh, 01Dh, 0E8h, 000h, 000h, 000h
        jne    SHORT loc_4_111D
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0DBh, 08Fh, 000h, 000h
        je     SHORT loc_4_111D
        db  083h, 03Dh, 014h, 001h, 000h, 000h, 03Ch
        jb     SHORT loc_4_111D
        movzx  eax, byte ptr [ebp + 10h]
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        jne    SHORT loc_4_10DF
        db  083h, 03Dh, 014h, 001h, 000h, 000h, 048h
        jb     SHORT loc_4_111D
        shl    eax, 4
        db  08Bh, 0B0h, 0FBh, 006h, 000h, 000h
        db  00Fh, 0B6h, 088h, 0FFh, 006h, 000h, 000h
        jmp    SHORT loc_4_111D
loc_4_10DF:
        cmp    al, 3
        jne    SHORT loc_4_10F3
        db  083h, 03Dh, 014h, 001h, 000h, 000h, 048h
        jb     SHORT loc_4_111D
        db  0BEh, 0D5h, 004h, 000h, 000h
        jmp    SHORT loc_4_111D
loc_4_10F3:
        cmp    al, 5
        jne    SHORT loc_4_111D
        db  0BEh, 043h, 005h, 000h, 000h
        mov    ecx, 37h
        db  083h, 03Dh, 014h, 001h, 000h, 000h, 048h
        jb     SHORT loc_4_111D
        db  0BEh, 0B1h, 005h, 000h, 000h
        db  083h, 03Dh, 014h, 001h, 000h, 000h, 04Bh
        jb     SHORT loc_4_111D
        db  0BEh, 01Fh, 006h, 000h, 000h
loc_4_111D:
        mov    eax, 12h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 4]
        add    eax, 0a0000h
        mov    dl, 10h
loc_4_1134:
        mov    dword ptr [eax], 0f0f0f0fh
        add    eax, 1000h
        dec    dl
        jne    SHORT loc_4_1134
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        je     SHORT loc_4_1157
        mov    edx, 3c3h
        xor    al, al
        out    dx, al
loc_4_1157:
        db  08Bh, 015h, 0FCh, 000h, 000h, 000h
        add    dl, 4
        xor    al, al
        out    dx, al
        inc    dl
        mov    al, 0ffh
        out    dx, al
        sub    dl, 5
        mov    al, 4
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        jne    SHORT loc_4_117B
        mov    al, 5
loc_4_117B:
        out    dx, al
        inc    dl
        mov    al, 1
        out    dx, al
        add    dl, 5
        xor    al, al
        out    dx, al
        add    dl, 2
        out    dx, al
        inc    dl
        mov    al, 3
        cmp    byte ptr [ebp + 10h], 11h
        je     SHORT loc_4_119F
        mov    al, 0ah
        cmp    byte ptr [ebp + 10h], 4
        je     SHORT loc_4_119F
        mov    al, 3
loc_4_119F:
        out    dx, al
        inc    dl
        rep outsw
        cmp    byte ptr [ebp + 10h], 11h
        jb     SHORT loc_4_1211
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        jne    SHORT loc_4_11D5
        mov    edx, 3c6h
        mov    al, 0ffh
        out    dx, al
        mov    edx, 216ah
        mov    al, 51h
        out    dx, al
        mov    edx, 3c6h
        in     al, dx
        in     al, dx
        in     al, dx
        in     al, dx
        mov    al, 0e0h
        out    dx, al
        jmp    SHORT loc_4_1211
loc_4_11D5:
        db  0BEh, 0A5h, 00Bh, 000h, 000h
        mov    cl, 80h
        mov    al, 55h
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        test   al, 80h
        mov    ch, 0
        jne    SHORT loc_4_11EC
        mov    ch, 80h
loc_4_11EC:
        mov    ax, 66h
        out    dx, ax
loc_4_11F2:
        mov    al, 60h
        mov    ah, ch
        out    dx, ax
        mov    al, 65h
        mov    ah, byte ptr [esi]
        out    dx, ax
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        add    esi, 3
        inc    ch
        dec    cl
        jne    SHORT loc_4_11F2
loc_4_1211:
        mov    ax, word ptr [ebp + 10h]
        mov    edi, ebx
        db  003h, 03Dh, 0F0h, 000h, 000h, 000h
        mov    word ptr [edi], ax
        db  03Bh, 01Dh, 0E8h, 000h, 000h, 000h
        je     SHORT loc_4_1255
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        je     SHORT loc_4_1255
        mov    edx, 3c7h
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        mov    edx, 3c8h
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        mov    edx, 3c9h
        db  0CDh, 020h, 098h, 000h, 001h, 000h
loc_4_1255:
        test   byte ptr [edi + 1], 80h
        jne    SHORT loc_4_129B
        db  08Bh, 035h, 0F4h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 001h, 000h, 000h
        xor    eax, eax
        out    dx, al
loc_4_126A:
        push   eax
        db  08Bh, 03Dh, 0ECh, 000h, 000h, 000h
        mov    ecx, 4000h
        xor    eax, eax
        rep stosd
        pop    eax
        inc    eax
        out    dx, al
        sub    esi, 10000h
        jg     SHORT loc_4_126A
        xor    eax, eax
        out    dx, al
        jmp    SHORT loc_4_129B
loc_4_128A:
        call   7
        movzx  eax, byte ptr [ebp + 10h]
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_4_129B:
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_12A9
loc_4_12A3:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_12A9:
        ret
loc_4_12AA:
        mov    edi, ebx
        db  003h, 03Dh, 0F0h, 000h, 000h, 000h
        mov    ax, word ptr [edi]
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebp + 1ch], 4fh
        ret
loc_4_12C0:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_4_12CC
        mov    word ptr [ebp + 10h], 0
loc_4_12CC:
        mov    word ptr [ebp + 1ch], 4fh
        ret
loc_4_12D3:
        db  08Bh, 015h, 000h, 001h, 000h, 000h
        cmp    byte ptr [ebp + 11h], 0
        je     SHORT loc_4_12E8
        xor    ah, ah
        in     al, dx
        mov    word ptr [ebp + 14h], ax
        jmp    SHORT loc_4_12EC
loc_4_12E8:
        mov    al, byte ptr [ebp + 14h]
        out    dx, al
loc_4_12EC:
        mov    word ptr [ebp + 1ch], 4fh
        ret
loc_4_12F3:
        movzx  ecx, byte ptr [ebp + 10h]
        cmp    cl, 4
        jae    SHORT loc_4_137A
        mov    edi, ebx
        db  003h, 03Dh, 0F0h, 000h, 000h, 000h
        movzx  edi, word ptr [edi]
        cmp    edi, 117h
        ja     SHORT loc_4_137A
        sub    edi, 100h
        jl     SHORT loc_4_137A
        shl    edi, 4
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        jne    SHORT loc_4_1336
        db  00Fh, 0B7h, 0B7h, 001h, 007h, 000h, 000h
        db  00Fh, 0B7h, 0BFh, 003h, 007h, 000h, 000h
        jmp    SHORT loc_4_1344
loc_4_1336:
        db  00Fh, 0B7h, 0B7h, 003h, 001h, 000h, 000h
        db  00Fh, 0B7h, 0BFh, 005h, 001h, 000h, 000h
loc_4_1344:
        or     esi, esi
        je     SHORT loc_4_137A
        db  0A1h, 0F4h, 000h, 000h, 000h
        xor    edx, edx
        div    edi
        movzx  edx, word ptr [ebp + 18h]
        mov    word ptr [ebp + 10h], di
        mov    word ptr [ebp + 18h], si
        mov    word ptr [ebp + 14h], ax
        db  0FFh, 024h, 08Dh, 081h, 013h, 000h, 000h
loc_4_1368:
        cmp    edx, esi
        jne    SHORT loc_4_137A
        jmp    SHORT loc_4_1372
loc_4_136E:
        cmp    edx, edi
        jne    SHORT loc_4_137A
loc_4_1372:
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_1380
loc_4_137A:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_1380:
        ret
loc_4_1381:
        db  068h, 013h, 000h, 000h, 072h
        adc    eax, dword ptr [eax]
        db  000h, 06Eh, 013h
        add    byte ptr [eax], al
        db  072h, 013h
        add    byte ptr [eax], al
loc_4_1391:
        cmp    byte ptr [ebp + 10h], 1
        jne    SHORT loc_4_13AF
        mov    byte ptr [ebp + 11h], 0
        mov    word ptr [ebp + 18h], 0
        mov    word ptr [ebp + 14h], 0
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_13B5
loc_4_13AF:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_13B5:
        ret
loc_4_13B6:
        cmp    byte ptr [ebp + 10h], 1
        jne    SHORT loc_4_13C8
        mov    byte ptr [ebp + 11h], 6
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_13CE
loc_4_13C8:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_13CE:
        ret
loc_4_13CF:
        mov    cl, byte ptr [ebp + 10h]
        and    cl, 7fh
        or     cl, cl
        je     NEAR PTR loc_4_1466
        cmp    cl, 1
        jne    NEAR PTR loc_4_14E7
        mov    ah, 38h
        mov    al, 0
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     NEAR PTR loc_4_14DF
        mov    edi, eax
        movzx  ecx, word ptr [ebp + 14h]
        movzx  esi, word ptr [ebp + 18h]
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        je     SHORT loc_4_1443
        db  08Bh, 015h, 004h, 001h, 000h, 000h
        mov    eax, 66h
        out    dx, ax
loc_4_141C:
        mov    al, 60h
        mov    ah, cl
        out    dx, ax
        mov    al, 65h
        out    dx, al
        mov    byte ptr [edi], cl
        inc    dl
        in     al, dx
        mov    byte ptr [edi + 1], al
        in     al, dx
        mov    byte ptr [edi + 2], al
        in     al, dx
        mov    byte ptr [edi + 3], al
        dec    dl
        add    edi, 4
        inc    ecx
        dec    esi
        jne    SHORT loc_4_141C
        jmp    NEAR PTR loc_4_14D7
loc_4_1443:
        mov    edx, 3c7h
loc_4_1448:
        mov    al, cl
        out    dx, al
        mov    dl, 0c9h
        mov    byte ptr [edi], cl
        in     al, dx
        mov    byte ptr [edi + 1], al
        in     al, dx
        mov    byte ptr [edi + 2], al
        in     al, dx
        mov    byte ptr [edi + 3], al
        mov    dl, 0c7h
        add    edi, 4
        inc    ecx
        dec    esi
        jne    SHORT loc_4_1448
        jmp    SHORT loc_4_14D7
loc_4_1466:
        mov    ah, 38h
        mov    al, 0
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     SHORT loc_4_14DF
        mov    esi, eax
        movzx  ecx, word ptr [ebp + 14h]
        movzx  edi, word ptr [ebp + 18h]
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        je     SHORT loc_4_14B8
        db  08Bh, 015h, 004h, 001h, 000h, 000h
        mov    eax, 66h
        out    dx, ax
loc_4_1498:
        mov    al, 60h
        mov    ah, cl
        out    dx, ax
        mov    al, 65h
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        mov    ah, byte ptr [esi + 3]
        out    dx, ax
        add    esi, 4
        inc    ecx
        dec    edi
        jne    SHORT loc_4_1498
        jmp    SHORT loc_4_14D7
loc_4_14B8:
        mov    edx, 3c8h
loc_4_14BD:
        mov    al, cl
        out    dx, al
        inc    dl
        mov    al, byte ptr [esi + 1]
        out    dx, al
        mov    al, byte ptr [esi + 2]
        out    dx, al
        mov    al, byte ptr [esi + 3]
        out    dx, al
        dec    dl
        add    esi, 4
        inc    ecx
        dec    edi
        jne    SHORT loc_4_14BD
loc_4_14D7:
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_14ED
loc_4_14DF:
        mov    word ptr [ebp + 1ch], 14fh
        jmp    SHORT loc_4_14ED
loc_4_14E7:
        mov    word ptr [ebp + 1ch], 24fh
loc_4_14ED:
        ret
loc_4_14EE:
        mov    word ptr [ebp + 1ch], 14fh
        ret
loc_4_14F5:
        db  08Bh, 00Dh, 0F4h, 000h, 000h, 000h
        stc
        ret
loc_4_14FD:
        push   esi
        push   edi
        db  03Bh, 01Dh, 0E8h, 000h, 000h, 000h
        je     NEAR PTR loc_4_15A9
        cmp    dl, 0c6h
        je     NEAR PTR loc_4_15A9
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        jne    NEAR PTR loc_4_15A9
        mov    edi, ebx
        db  003h, 03Dh, 0F0h, 000h, 000h, 000h
        movzx  esi, word ptr [edi]
        and    esi, 0ffff3f7fh
        cmp    esi, 100h
        jb     SHORT loc_4_15A9
        cmp    dl, 0c7h
        jne    SHORT loc_4_155D
        db  08Bh, 015h, 004h, 001h, 000h, 000h
        mov    cl, al
        mov    ax, 66h
        out    dx, ax
        mov    ah, cl
        mov    al, 62h
        out    dx, ax
        mov    byte ptr [edi + 3], ah
        mov    byte ptr [edi + 5], 0
        jmp    SHORT loc_4_15AC
loc_4_155D:
        cmp    dl, 0c8h
        jne    SHORT loc_4_157F
        db  08Bh, 015h, 004h, 001h, 000h, 000h
        mov    cl, al
        mov    ax, 66h
        out    dx, ax
        mov    ah, cl
        mov    al, 60h
        out    dx, ax
        mov    byte ptr [edi + 2], ah
        mov    byte ptr [edi + 4], 0
        jmp    SHORT loc_4_15AC
loc_4_157F:
        db  08Bh, 015h, 004h, 001h, 000h, 000h
        mov    cl, al
        cmp    byte ptr [edi + 4], 3
        jb     SHORT loc_4_159B
        mov    byte ptr [edi + 4], 0
        inc    byte ptr [edi + 2]
        mov    al, 60h
        mov    ah, byte ptr [edi + 2]
        out    dx, ax
loc_4_159B:
        mov    ah, cl
        shl    ah, 2
        mov    al, 65h
        out    dx, ax
        inc    byte ptr [edi + 4]
        jmp    SHORT loc_4_15AC
loc_4_15A9:
        clc
        jmp    SHORT loc_4_15AD
loc_4_15AC:
        stc
loc_4_15AD:
        pop    edi
        pop    esi
        ret
loc_4_15B0:
        push   esi
        push   edi
        db  03Bh, 01Dh, 0E8h, 000h, 000h, 000h
        je     SHORT loc_4_161D
        cmp    dl, 0c6h
        je     SHORT loc_4_161D
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        jne    SHORT loc_4_161D
        mov    edi, ebx
        db  003h, 03Dh, 0F0h, 000h, 000h, 000h
        movzx  esi, word ptr [edi]
        and    esi, 0ffff3f7fh
        cmp    esi, 100h
        jb     SHORT loc_4_161D
        cmp    dl, 0c7h
        jne    SHORT loc_4_15EB
        xor    al, al
        jmp    SHORT loc_4_1620
loc_4_15EB:
        cmp    dl, 0c8h
        jne    SHORT loc_4_15F5
        mov    al, byte ptr [edi + 2]
        jmp    SHORT loc_4_1620
loc_4_15F5:
        db  08Bh, 015h, 004h, 001h, 000h, 000h
        cmp    byte ptr [edi + 5], 3
        jb     SHORT loc_4_160F
        mov    byte ptr [edi + 5], 0
        inc    byte ptr [edi + 3]
        mov    al, 62h
        mov    ah, byte ptr [edi + 3]
        out    dx, ax
loc_4_160F:
        mov    al, 65h
        out    dx, al
        inc    dl
        in     al, dx
        shr    al, 2
        inc    byte ptr [edi + 5]
        jmp    SHORT loc_4_1620
loc_4_161D:
        clc
        jmp    SHORT loc_4_1621
loc_4_1620:
        stc
loc_4_1621:
        pop    edi
        pop    esi
        ret
loc_4_1624:
        db  080h, 03Dh, 064h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_1646
        test   cl, 4
        je     SHORT loc_4_1646
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        je     SHORT loc_4_1646
        db  03Bh, 03Dh, 0E8h, 000h, 000h, 000h
        jne    SHORT loc_4_1646
        or     al, 5
loc_4_1646:
        db  0CDh, 020h, 015h, 080h, 00Ah, 000h
loc_4_164C:
        test   cl, 4
        je     SHORT loc_4_1663
        cmp    dl, 6ah
        jne    SHORT loc_4_1687
        mov    edi, ebx
        db  003h, 03Dh, 0F0h, 000h, 000h, 000h
        mov    byte ptr [edi + 6], al
        jmp    SHORT loc_4_1687
loc_4_1663:
        mov    edi, ebx
        db  003h, 03Dh, 0F0h, 000h, 000h, 000h
        cmp    byte ptr [edi + 6], 6fh
        jne    SHORT loc_4_169A
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        je     SHORT loc_4_169A
        mov    dl, 6ah
        mov    al, 6fh
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0fbh
        jmp    SHORT loc_4_16A0
loc_4_1687:
        db  080h, 03Dh, 064h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_169A
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        jne    SHORT loc_4_16A0
loc_4_169A:
        db  0CDh, 020h, 015h, 080h, 00Ah, 000h
loc_4_16A0:
        ret
_PTEXT ends
        end
