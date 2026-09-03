; mssndsys_lcode_01.asm -- MSSNDSYS object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  083h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  085h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  084h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  086h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
loc_1_0040:
        sal    byte ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0050:
        sal    dword ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 05Fh, 004h, 004h, 000h, 000h, 000h, 06Dh, 073h, 073h, 06Eh
        db  064h, 073h, 079h, 073h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00DC:
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0115:
        add    byte ptr [eax], al
        add    byte ptr [edi + 3f9f9f9fh], bl
        aas
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cld
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h
loc_1_0136:
        add    dword ptr [edi], ecx
        add    byte ptr [esi], cl
        add    eax, dword ptr [edx]
        add    eax, 0d060407h
        or     dword ptr [ebx], ecx
        db  00Ch
loc_1_0144:
        mov    cl, 17h
        add    byte ptr [eax], al
        mov    ds, word ptr [eax + eax]
        add    byte ptr [eax + 22h], ah
        add    byte ptr [eax], al
        dec    eax
        sub    byte ptr [eax], al
        add    al, cl
        xor    al, 0
        add    byte ptr [edx], ch
        inc    esp
        add    byte ptr [eax], al
        sti
        dec    edi
        add    byte ptr [eax], al
        lahf
        pushad
        add    byte ptr [eax], al
        push   cs
        je     SHORT loc_1_0167
loc_1_0167:
        add    byte ptr [ecx], bl
        jg     SHORT loc_1_016B
loc_1_016B:
        add    byte ptr [ebp - 76h], ch
        add    byte ptr [eax], al
        neg    byte ptr [edi - 4c1e0000h]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_017C:
        xchg   byte ptr [19dc0000h], dl
        add    byte ptr [eax], al
        inc    eax
        pop    ds
        add    byte ptr [eax], al
        and    byte ptr [2b110000h], 0
        add    byte ptr [eax - 2bffffc2h], al
        dec    ecx
        add    byte ptr [eax], al
        and    dl, byte ptr [esi]
        add    byte ptr [ebx + ebp*2], bl
        add    byte ptr [eax], al
        add    byte ptr [ebp], bh
        add    byte ptr [ebx], dh
        add    dword ptr [eax], 93a800h
        db  000h
loc_1_01AC:
        inc    esp
        lodsb
        add    byte ptr [eax], al
sub_1_01B0:
        cmp    byte ptr [ebx + 140000h], 0
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    dword ptr [eax], 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    dword ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        fadd   dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        aam    0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        fiadd  dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loop   loc_1_0206
loc_1_0206:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        in     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0219:
        add    byte ptr [eax], al
        add    al, ah
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    cl, ah
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    dl, dh
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h
mssndsys_Control:
        cmp    eax, 8
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_023D:
        cmp    eax, 0bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0246:
        cmp    eax, 3
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_024F:
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0258:
        cmp    eax, 1ch
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0261:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_026A:
        clc
        ret
sub_1_026C:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   esi
        pushfd
        mov    eax, dword ptr [ebp + 0ch]
        shl    eax, 3
        db  08Bh, 090h, 000h, 000h, 000h, 000h
        db  08Bh, 080h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp + 0ch], eax
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_02C3
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_02C3
loc_1_029F:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0ch], 1
        jne    SHORT loc_1_02B3
        cmp    dword ptr [edx + edi], ebx
        jne    SHORT loc_1_02B9
loc_1_02AF:
        popfd
        clc
        jmp    SHORT loc_1_02C7
loc_1_02B3:
        cmp    word ptr [edx + edi], bx
        je     SHORT loc_1_02AF
loc_1_02B9:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_02C3
        jmp    SHORT loc_1_029F
loc_1_02C3:
        popfd
        xor    edi, edi
        stc
loc_1_02C7:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
        db  0B8h, 000h, 004h, 000h, 000h, 0C3h
sub_1_02D3:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_026C
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     SHORT loc_1_02F0
        mov    eax, dword ptr [edi + 0b4h]
loc_1_02F0:
        pop    edi
        pop    ebp
        ret
sub_1_02F3:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_026C
        add    esp, 8
        or     edi, edi
        je     SHORT loc_1_0311
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 0b4h], eax
loc_1_0311:
        pop    edi
        pop    ebp
        ret
sub_1_0314:
        push   eax
        push   ebx
        pushfd
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     SHORT loc_1_0341
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_0341
loc_1_032C:
        cmp    dword ptr [eax + 4], edi
        jne    SHORT loc_1_0337
        mov    esi, eax
        popfd
        clc
        jmp    SHORT loc_1_0345
loc_1_0337:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_0341
        jmp    SHORT loc_1_032C
loc_1_0341:
        popfd
        xor    esi, esi
        stc
loc_1_0345:
        pop    ebx
        pop    eax
        ret
        db  0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_0350:
        pushad
        pushfd
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_039F
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_039F
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_039F
loc_1_0371:
        xor    ecx, ecx
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 24h], ebx
        jne    SHORT loc_1_037D
        or     ecx, 1
loc_1_037D:
        cmp    dword ptr [edi + 30h], ebx
        jne    SHORT loc_1_0385
        or     ecx, 2
loc_1_0385:
        jecxz  loc_1_0390
        push   eax
        mov    eax, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_038F:
        pop    eax
loc_1_0390:
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_039F
        jmp    SHORT loc_1_0371
loc_1_039F:
        popfd
        popad
        db  0FFh, 025h, 000h, 000h, 000h, 000h
        db  090h
loc_1_03A8:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_03EC
        db  0CDh, 020h, 004h, 000h, 004h, 000h
        and    dl, 3fh
        push   edi
        push   2
        push   eax
        call   sub_1_026C
        add    esp, 8
        or     edi, edi
        jne    SHORT loc_1_03D4
        push   3
        push   eax
        call   sub_1_026C
        add    esp, 8
loc_1_03D4:
        or     edi, edi
        je     SHORT loc_1_03E3
        test   word ptr [edi + 4], 4
        je     SHORT loc_1_03E3
        or     dl, 40h
loc_1_03E3:
        pop    edi
        xor    dh, dh
        db  0CDh, 020h, 005h, 000h, 004h, 000h
loc_1_03EC:
        db  0CDh, 020h, 012h, 000h, 004h, 000h
        ret
        db  090h
loc_1_03F4:
        push   eax
        push   edi
        push   edx
        mov    edi, edx
        mov    eax, dword ptr [edi + 24h]
        or     eax, eax
        je     NEAR PTR loc_1_0487
        cmp    word ptr [edi + 0b8h], 0
        jne    SHORT loc_1_0487
        mov    ebx, eax
        test   word ptr [edi + 52h], 8
        je     SHORT loc_1_0465
        movzx  edx, word ptr [edi + 22h]
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0465
        push   ebx
        mov    ebx, dword ptr [edi + 30h]
        or     ebx, ebx
        je     SHORT loc_1_044E
        test   word ptr [edi + 4], 2000h
        jne    SHORT loc_1_044E
        mov    byte ptr [edi + 0ach], al
        or     word ptr [edi + 4], 2000h
        push   edx
        push   esi
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edx, edi
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        pop    edx
loc_1_044E:
        pop    ebx
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_04B3
        push   ebx
        mov    ebx, 4
        mov    al, 60h
        call   sub_1_0937
        pop    ebx
loc_1_0465:
        test   word ptr [edi + 4], 200h
        je     SHORT loc_1_047C
        test   word ptr [edi + 4], 4000h
        je     SHORT loc_1_047C
        call   sub_1_0954
        jb     SHORT loc_1_04A6
loc_1_047C:
        pop    edx
        pop    edi
        pop    eax
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        clc
        ret
loc_1_0487:
        test   word ptr [edi + 52h], 8
        je     SHORT loc_1_04A6
        movzx  edx, word ptr [edi + 22h]
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_04A6
        push   ebx
        mov    ebx, 4
        mov    al, 60h
        call   sub_1_0937
        pop    ebx
loc_1_04A6:
        movzx  edx, word ptr [edi + 42h]
        xor    eax, eax
        add    edx, 2
        out    dx, al
        sub    edx, 2
loc_1_04B3:
        pop    edx
        pop    edi
        pop    eax
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
        db  08Bh, 0C0h
loc_1_04C0:
        push   eax
        push   edi
        push   edx
        mov    edi, edx
        test   word ptr [edi + 4], 2
        jne    SHORT loc_1_04E1
        push   esi
        or     word ptr [edi + 4], 2
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edx, edi
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
loc_1_04E1:
        movzx  edx, word ptr [edi + 42h]
        add    edx, 0ah
        in     al, dx
        test   al, 40h
        je     SHORT loc_1_051B
        mov    eax, dword ptr [edi + 24h]
        or     eax, eax
        je     SHORT loc_1_050E
        mov    ebx, eax
        test   word ptr [edi + 4], 4000h
        je     SHORT loc_1_0505
        call   sub_1_0954
        jb     SHORT loc_1_050E
loc_1_0505:
        mov    eax, dword ptr [edi + 2ch]
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_050E:
        movzx  edx, word ptr [edi + 42h]
        xor    eax, eax
        add    edx, 2
        out    dx, al
        sub    edx, 2
loc_1_051B:
        movzx  edx, word ptr [edi + 48h]
        in     al, dx
        test   al, 20h
        je     SHORT loc_1_0529
        pop    edx
        pop    edi
        pop    eax
        stc
        ret
loc_1_0529:
        pop    edx
        pop    edi
        pop    eax
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
loc_1_0534:
        pushfd
        cli
        mov    edi, edx
        and    word ptr [edi + 4], 0fffdh
        mov    eax, dword ptr [edi + 2ch]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 0a1h
        jne    SHORT loc_1_055A
        movzx  edx, word ptr [edi + 42h]
        add    edx, 0ah
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0563
loc_1_055A:
        movzx  edx, word ptr [edi + 48h]
        in     al, dx
        test   al, 20h
        je     SHORT loc_1_0570
loc_1_0563:
        movzx  edx, word ptr [edi + 42h]
        add    edx, 0ah
        mov    al, 8
        out    dx, al
        xor    al, al
        out    dx, al
loc_1_0570:
        popfd
        ret
        db  08Bh, 0C0h
loc_1_0574:
        push   ecx
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 8
        je     SHORT loc_1_058F
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_1_058F:
        pop    ecx
        clc
        ret
        db  08Bh, 0C0h
loc_1_0594:
        push   edi
        push   1
        push   eax
        call   sub_1_026C
        add    esp, 8
        or     edi, edi
        je     SHORT loc_1_05C3
        test   word ptr [edi + 4], 8
        jne    SHORT loc_1_05CD
        cmp    dword ptr [edi + 24h], ebx
        jne    SHORT loc_1_05C3
        jecxz  loc_1_05BB
loc_1_05B3:
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_1_05CD
loc_1_05BB:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_1_05CD
loc_1_05C3:
        test   word ptr [edi + 4], 8000h
        jne    SHORT loc_1_05BB
        je     SHORT loc_1_05B3
loc_1_05CD:
        pop    edi
        clc
        ret
sub_1_05D0:
        push   eax
        push   ecx
        push   edx
        add    dx, 0
        mov    ecx, 20000h
        in     al, dx
loc_1_05DD:
        in     al, dx
        or     al, al
        jns    SHORT loc_1_05E5
        loop   loc_1_05DD
        stc
loc_1_05E5:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_1_05E9:
        call   sub_1_05D0
        jb     SHORT loc_1_0606
        pushfd
        cli
        push   edx
        add    dx, 0
        mov    al, ah
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        out    dx, al
        inc    dx
        in     al, dx
        pop    edx
        popfd
        clc
loc_1_0606:
        ret
sub_1_0607:
        call   sub_1_05D0
        jb     SHORT loc_1_0628
        pushfd
        cli
        push   eax
        push   edx
        add    dx, 0
        xchg   al, ah
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        out    dx, al
        inc    dx
        mov    al, ah
        out    dx, al
        pop    edx
        pop    eax
        popfd
        clc
loc_1_0628:
        ret
sub_1_0629:
        push   edx
        push   ecx
        push   eax
        pushfd
        cli
        cmp    word ptr [edi + 4ch], 0
        jne    SHORT loc_1_0680
        mov    ecx, 6
        push   esi
        xor    esi, esi
loc_1_063D:
        mov    eax, esi
        mov    ah, al
        call   sub_1_05E9
        db  088h, 086h, 000h, 000h, 000h, 000h
        or     al, 80h
        call   sub_1_0607
        inc    esi
        loop   loc_1_063D
        mov    ecx, 2
loc_1_065B:
        mov    eax, esi
        mov    ah, al
        call   sub_1_05E9
        db  088h, 086h, 000h, 000h, 000h, 000h
        push   eax
        and    al, 0fh
        cmp    al, 0dh
        pop    eax
        jbe    SHORT loc_1_0676
        mov    al, 0dh
        jmp    SHORT loc_1_0678
loc_1_0676:
        and    al, 0f0h
loc_1_0678:
        call   sub_1_0607
        loop   loc_1_065B
        pop    esi
loc_1_0680:
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 40h
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_05D0
        jb     SHORT loc_1_069C
        add    dx, 0
        out    dx, al
        popfd
        clc
        jmp    SHORT loc_1_069E
loc_1_069C:
        popfd
        stc
loc_1_069E:
        pop    eax
        pop    ecx
        pop    edx
        ret
sub_1_06A2:
        push   edx
        push   ecx
        push   eax
        pushfd
        cli
        db  0A0h, 000h, 000h, 000h, 000h
        and    al, 0bfh
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_05D0
        jb     SHORT loc_1_070A
        or     al, 9
        add    dx, 0
        out    dx, al
        inc    dx
        in     al, dx
        test   al, 8
        je     SHORT loc_1_06E0
        mov    ecx, 1388h
loc_1_06CD:
        in     al, dx
        test   al, 20h
        jne    SHORT loc_1_06D4
        loop   loc_1_06CD
loc_1_06D4:
        mov    ecx, 1388h
loc_1_06D9:
        in     al, dx
        test   al, 20h
        je     SHORT loc_1_06E0
        loop   loc_1_06D9
loc_1_06E0:
        sub    dx, 1
        cmp    word ptr [edi + 4ch], 0
        jne    SHORT loc_1_0706
        mov    ecx, 8
        push   esi
        xor    esi, esi
loc_1_06F3:
        mov    eax, esi
        mov    ah, al
        db  08Ah, 086h, 000h, 000h, 000h, 000h
        call   sub_1_0607
        inc    esi
        loop   loc_1_06F3
        pop    esi
loc_1_0706:
        popfd
        clc
        jmp    SHORT loc_1_070C
loc_1_070A:
        popfd
        stc
loc_1_070C:
        pop    eax
        pop    ecx
        pop    edx
        ret
sub_1_0710:
        push   edx
        push   ecx
        push   eax
        mov    cx, ax
        mov    ah, 0ah
        call   sub_1_05E9
        and    al, 3fh
        db  08Ah, 025h, 000h, 000h, 000h, 000h
        or     cx, cx
        je     SHORT loc_1_072F
        or     ah, 40h
        jmp    SHORT loc_1_0742
loc_1_072F:
        and    ah, 0bfh
        push   eax
        mov    ecx, 5000h
        mov    ah, 0ah
loc_1_073A:
        call   sub_1_05E9
        loop   loc_1_073A
        pop    eax
loc_1_0742:
        db  088h, 025h, 000h, 000h, 000h, 000h
        or     al, ah
        mov    ah, 0ah
        call   sub_1_0607
        pop    eax
        pop    ecx
        pop    edx
        clc
        ret
sub_1_0756:
        push   edx
        push   ecx
        push   eax
        pushfd
        cli
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 20h
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_05D0
        jb     SHORT loc_1_0777
        add    dx, 0
        out    dx, al
        popfd
        clc
        jmp    SHORT loc_1_0779
loc_1_0777:
        popfd
        stc
loc_1_0779:
        pop    eax
        pop    ecx
        pop    edx
        ret
sub_1_077D:
        push   edx
        push   ecx
        push   eax
        pushfd
        cli
        db  0A0h, 000h, 000h, 000h, 000h
        and    al, 0dfh
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_05D0
        jb     SHORT loc_1_0777
        add    dx, 0
        out    dx, al
        popfd
        clc
        jmp    SHORT loc_1_07A0
        db  09Dh, 0F9h
loc_1_07A0:
        pop    eax
        pop    ecx
        pop    edx
        ret
sub_1_07A4:
        pushad
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 42h]
        push   edi
        push   esi
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        call   sub_1_0314
        jb     SHORT loc_1_07C4
        mov    edi, dword ptr [esp]
        mov    eax, dword ptr [esi + 0ah]
        mov    dword ptr [edi + 0ah], eax
loc_1_07C4:
        pop    esi
        pop    edi
        pushfd
        cli
        cmp    word ptr [edi + 4ch], 0
        jne    SHORT loc_1_07D9
        mov    eax, 1
        call   sub_1_0710
loc_1_07D9:
        call   sub_1_0629
        mov    ecx, 8
        mov    ah, 0fh
loc_1_07E5:
        mov    al, byte ptr [ecx + esi + 0fh]
        cmp    ah, 0ah
        jne    SHORT loc_1_07F6
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0806
loc_1_07F6:
        cmp    ah, 9
        jne    SHORT loc_1_07FF
        and    al, 0ch
        jmp    SHORT loc_1_0806
loc_1_07FF:
        cmp    ah, 0ah
        jne    SHORT loc_1_0806
        and    al, 0fdh
loc_1_0806:
        call   sub_1_0607
        dec    ah
        loop   loc_1_07E5
        call   sub_1_06A2
        mov    ecx, 7
        mov    ah, 7
loc_1_081B:
        mov    al, byte ptr [ecx + esi + 8]
        call   sub_1_0607
        dec    ah
        loop   loc_1_081B
        cmp    word ptr [edi + 4ch], 0
        jne    SHORT loc_1_0836
        xor    eax, eax
        call   sub_1_0710
loc_1_0836:
        test   word ptr [edi + 4], 200h
        je     SHORT loc_1_0870
        add    edx, 0ah
        push   edx
        mov    eax, 6
        out    dx, al
        movzx  edx, word ptr [edi + 0aeh]
        mov    ecx, 4000h
        mov    eax, 80h
loc_1_0859:
        out    dx, al
        loop   loc_1_0859
        xchg   dword ptr [esp], edx
        mov    al, 7
        out    dx, al
        pop    edx
        mov    ecx, 4000h
        mov    eax, 80h
loc_1_086D:
        out    dx, al
        loop   loc_1_086D
loc_1_0870:
        popfd
        clc
        popad
        ret
sub_1_0874:
        pushad
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 42h]
        pushfd
        cli
        mov    ecx, 0fh
        mov    ah, 0fh
loc_1_0885:
        call   sub_1_05E9
        mov    byte ptr [ecx + esi + 8], al
        dec    ah
        loop   loc_1_0885
        popfd
        clc
        popad
        ret
sub_1_0896:
        push   eax
        push   ecx
        push   edx
        push   esi
        xor    ecx, ecx
        movzx  eax, ax
loc_1_089F:
        db  03Bh, 081h, 000h, 000h, 000h, 000h
        jbe    SHORT loc_1_08B6
        add    ecx, 4
        db  083h, 0B9h, 000h, 000h, 000h, 000h, 0FFh
        jne    SHORT loc_1_089F
        sub    ecx, 4
loc_1_08B6:
        db  08Bh, 081h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 90h], eax
        shr    ecx, 2
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        cmp    al, byte ptr [esi + 8ch]
        je     SHORT loc_1_0929
        mov    byte ptr [esi + 8ch], al
        movzx  edx, word ptr [esi + 42h]
        cmp    word ptr [esi + 4ch], 0
        jne    SHORT loc_1_08EE
        mov    eax, 1
        call   sub_1_0710
loc_1_08EE:
        call   sub_1_0629
        jb     SHORT loc_1_0929
        mov    ah, 8
        mov    al, byte ptr [esi + 8ch]
        call   sub_1_0607
        call   sub_1_06A2
        cmp    byte ptr [esi + 0bh], 8ah
        je     SHORT loc_1_0913
        cmp    byte ptr [esi + 0bh], 81h
        jne    SHORT loc_1_091B
loc_1_0913:
        mov    ecx, 0c8h
loc_1_0918:
        in     al, dx
        loop   loc_1_0918
loc_1_091B:
        cmp    word ptr [esi + 4ch], 0
        jne    SHORT loc_1_0929
        xor    eax, eax
        call   sub_1_0710
loc_1_0929:
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_1_092E:
        push   eax
        in     al, dx
        jmp    SHORT loc_1_0932
loc_1_0932:
        in     al, dx
        jmp    SHORT loc_1_0935
loc_1_0935:
        pop    eax
        ret
sub_1_0937:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_092E
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_092E
        pop    edx
        pop    ebx
        ret
sub_1_0954:
        test   word ptr [edi + 94h], 1
        je     SHORT loc_1_09D9
        pushad
        mov    ax, word ptr [edi + 98h]
        sub    word ptr [edi + 0a2h], ax
        cmc
        pushfd
        jb     SHORT loc_1_0980
        mov    ax, word ptr [edi + 0a0h]
        mov    word ptr [edi + 0a2h], ax
loc_1_0980:
        movzx  eax, word ptr [edi + 9ah]
        add    ax, word ptr [edi + 98h]
        cmp    word ptr [edi + 96h], ax
        ja     SHORT loc_1_0999
        xor    eax, eax
loc_1_0999:
        mov    word ptr [edi + 9ah], ax
        mov    esi, dword ptr [edi + 9ch]
        add    esi, eax
        movzx  ecx, word ptr [edi + 96h]
        sub    ecx, eax
        mov    word ptr [edi + 0a4h], cx
        movzx  edx, word ptr [edi + 42h]
        add    edx, 0ah
        mov    al, 4
        out    dx, al
        movzx  ecx, word ptr [edi + 98h]
        inc    ecx
        shr    ecx, 1
        movzx  edx, word ptr [edi + 0aeh]
        cld
        rep outsw
        popfd
        popad
loc_1_09D9:
        ret
        db  08Bh, 0C0h
loc_1_09DC:
        pushad
        push   6
        push   eax
        call   sub_1_026C
        add    esp, 8
        or     edi, edi
        je     NEAR PTR loc_1_0A8D
        db  0CDh, 020h, 004h, 000h, 004h, 000h
        test   dl, 1
        je     SHORT loc_1_0A26
        test   word ptr [edi + 94h], 2
        jne    NEAR PTR loc_1_0A8D
        or     word ptr [edi + 94h], 2
        mov    word ptr [edi + 96h], 0
        mov    word ptr [edi + 0a4h], 0ffffh
        jmp    SHORT loc_1_0A86
loc_1_0A26:
        test   word ptr [edi + 94h], 2
        jne    SHORT loc_1_0A4D
        cmp    word ptr [edi + 0a4h], -1
        je     SHORT loc_1_0A4D
        pushfd
        cli
        mov    cx, word ptr [edi + 0a4h]
        db  0CDh, 020h, 021h, 000h, 004h, 000h
        popfd
        jmp    SHORT loc_1_0A8D
loc_1_0A4D:
        mov    word ptr [edi + 9ah], 0
        mov    word ptr [edi + 96h], cx
        mov    word ptr [edi + 98h], cx
        mov    word ptr [edi + 0a4h], cx
        mov    dword ptr [edi + 9ch], esi
        and    word ptr [edi + 94h], 0fffch
        test   dl, 10h
        je     SHORT loc_1_0A86
        or     word ptr [edi + 94h], 1
loc_1_0A86:
        popad
        db  0CDh, 020h, 012h, 080h, 004h, 000h
loc_1_0A8D:
        popad
        ret
        db  090h
sub_1_0A90:
        push   eax
        mov    eax, dword ptr [esi + 24h]
        or     eax, eax
        je     SHORT loc_1_0AA0
        cmp    eax, ebx
        jne    NEAR PTR loc_1_0B39
loc_1_0AA0:
        test   word ptr [esi + 4], 4000h
        jne    NEAR PTR loc_1_0B48
        cmp    dword ptr [esi + 24h], ebx
        je     SHORT loc_1_0AEB
        push   ebx
        mov    ebx, dword ptr [esi + 28h]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_0AC4
        mov    dword ptr [esi + 28h], 0ffffffffh
loc_1_0AC4:
        pop    ebx
        mov    eax, 1
        test   word ptr [esi + 52h], 8
        je     SHORT loc_1_0AD5
        or     eax, 2
loc_1_0AD5:
        mov    edi, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0ADC:
        jb     SHORT loc_1_0B39
        test   word ptr [esi + 52h], 8
        je     SHORT loc_1_0AEB
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0AEB:
        cmp    word ptr [esi + 4ch], 0
        je     SHORT loc_1_0AFD
        push   edx
        movzx  edx, word ptr [esi + 42h]
        call   sub_1_0756
        pop    edx
loc_1_0AFD:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 00Dh, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 00Dh, 000h
        mov    byte ptr [esi + 8ch], 0ffh
        mov    byte ptr [esi + 8dh], 5
        or     word ptr [esi + 4], 4000h
        or     word ptr [edi + 94h], 2
        and    word ptr [edi + 94h], 0fffeh
        jmp    SHORT loc_1_0B48
loc_1_0B39:
        mov    edi, esi
        mov    eax, 3
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0B45:
        stc
        jmp    SHORT loc_1_0B49
loc_1_0B48:
        clc
loc_1_0B49:
        pop    eax
        ret
        db  090h
loc_1_0B4C:
        xor    eax, eax
        dec    eax
        ret
sub_1_0B50:
        push   eax
        xor    eax, eax
        mov    dword ptr [esi + 70h], eax
        mov    word ptr [esi + 88h], ax
        mov    word ptr [esi + 0a0h], ax
        mov    word ptr [esi + 0a2h], ax
        and    word ptr [esi + 8ah], 2
        mov    dword ptr [esi + 68h], eax
        mov    dword ptr [esi + 6ch], eax
        mov    byte ptr [esi + 85h], 7fh
        mov    byte ptr [esi + 84h], 7fh
        mov    byte ptr [esi + 87h], 0
        mov    eax, 0aah
        call   sub_1_0C5E
        pop    eax
        ret
        db  08Bh, 0C0h
loc_1_0B9C:
        call   sub_1_0A90
        jb     SHORT loc_1_0BD9
        cmp    ecx, 4
        jbe    SHORT loc_1_0BAE
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0BAE:
        jb     SHORT loc_1_0BD9
        test   al, 1
        je     SHORT loc_1_0BD2
        test   word ptr [esi + 8ah], 2
        je     SHORT loc_1_0BC4
        call   sub_1_1148
loc_1_0BC4:
        call   sub_1_0B50
        or     byte ptr [esi + 85h], 80h
        jmp    SHORT loc_1_0BD9
loc_1_0BD2:
        and    byte ptr [esi + 85h], 7fh
loc_1_0BD9:
        ret
        db  08Bh, 0C0h
loc_1_0BDC:
        call   sub_1_0A90
        jb     SHORT loc_1_0C00
        movzx  edx, word ptr [esi + 22h]
        cmp    ecx, 4
        jbe    SHORT loc_1_0BF2
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0BF2:
        jb     SHORT loc_1_0BFF
        cmp    ah, 2
        jne    SHORT loc_1_0BFC
        add    edx, 2
loc_1_0BFC:
        out    dx, al
        jmp    SHORT loc_1_0C00
loc_1_0BFF:
        in     al, dx
loc_1_0C00:
        clc
        ret
        db  08Bh, 0C0h
loc_1_0C04:
        call   sub_1_0A90
        jb     SHORT loc_1_0C23
        cmp    ecx, 4
        jbe    SHORT loc_1_0C16
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0C16:
        jb     SHORT loc_1_0C20
        movzx  edx, word ptr [esi + 22h]
        inc    edx
        out    dx, al
        clc
        ret
loc_1_0C20:
        xor    eax, eax
        dec    eax
loc_1_0C23:
        ret
loc_1_0C24:
        call   sub_1_0A90
        jb     SHORT loc_1_0C5D
        cmp    ecx, 4
        jbe    SHORT loc_1_0C36
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0C36:
        je     SHORT loc_1_0C5D
        push   ecx
        mov    ecx, dword ptr [esi + 68h]
        cmp    ecx, dword ptr [esi + 6ch]
        je     SHORT loc_1_0C4E
        inc    ecx
        cmp    ecx, 10h
        jb     SHORT loc_1_0C49
        xor    ecx, ecx
loc_1_0C49:
        cmp    ecx, dword ptr [esi + 6ch]
        jne    SHORT loc_1_0C55
loc_1_0C4E:
        and    byte ptr [esi + 84h], 7fh
loc_1_0C55:
        mov    al, byte ptr [ecx + esi + 74h]
        mov    dword ptr [esi + 68h], ecx
        pop    ecx
loc_1_0C5D:
        ret
sub_1_0C5E:
        push   ecx
        mov    ecx, dword ptr [esi + 6ch]
        inc    ecx
        cmp    ecx, 10h
        jb     SHORT loc_1_0C6A
        xor    ecx, ecx
loc_1_0C6A:
        mov    byte ptr [ecx + esi + 74h], al
        mov    dword ptr [esi + 6ch], ecx
        or     byte ptr [esi + 84h], 80h
        pop    ecx
        ret
        db  08Bh, 0C0h
loc_1_0C7C:
        call   sub_1_0A90
        jb     SHORT loc_1_0CCD
        cmp    ecx, 4
        jbe    SHORT loc_1_0C8E
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0C8E:
        jb     SHORT loc_1_0CBE
        push   eax
        push   ecx
        mov    ecx, dword ptr [esi + 70h]
        jecxz  loc_1_0C9C
        call   ecx
        clc
        jmp    SHORT loc_1_0CBB
loc_1_0C9C:
        movzx  eax, al
        mov    ecx, 10h
loc_1_0CA4:
        db  03Bh, 004h, 0CDh, 0F8h, 0FFh, 0FFh, 0FFh
        je     SHORT loc_1_0CB2
        loop   loc_1_0CA4
        stc
        jmp    SHORT loc_1_0CBB
loc_1_0CB2:
        db  02Eh, 0FFh, 014h, 0CDh, 0FCh, 0FFh, 0FFh, 0FFh
        clc
loc_1_0CBB:
        pop    ecx
        pop    eax
        ret
loc_1_0CBE:
        mov    al, byte ptr [esi + 85h]
        and    byte ptr [esi + 85h], 7fh
        clc
        ret
loc_1_0CCD:
        stc
        ret
        db  090h, 066h, 0F7h, 086h, 08Ah, 000h, 000h, 000h, 001h, 000h, 075h, 053h, 050h, 052h, 00Fh, 0B7h
        db  056h, 042h, 0E8h, 043h, 0F9h, 0FFh, 0FFh, 0B4h, 00Fh, 032h, 0C0h, 0E8h, 018h, 0F9h, 0FFh, 0FFh
        db  0B4h, 00Eh, 0E8h, 011h, 0F9h, 0FFh, 0FFh, 0B4h, 009h, 0B0h, 041h, 0E8h, 008h, 0F9h, 0FFh, 0FFh
        db  0E8h, 09Eh, 0F9h, 0FFh, 0FFh, 0B4h, 006h, 08Ah, 086h, 08Dh, 000h, 000h, 000h, 0E8h, 0F6h, 0F8h
        db  0FFh, 0FFh, 0B4h, 007h, 0E8h, 0EFh, 0F8h, 0FFh, 0FFh, 0B4h, 00Ah, 0A0h, 000h, 000h, 000h, 000h
        db  0E8h, 0E3h, 0F8h, 0FFh, 0FFh, 066h, 083h, 08Eh, 08Ah, 000h, 000h, 000h, 001h, 05Ah, 058h, 0C6h
        db  086h, 086h, 000h, 000h, 000h, 000h, 0C7h, 046h, 070h, 000h, 000h, 000h, 000h, 0C3h, 02Eh, 08Bh
        db  0C0h
loc_1_0D40:
        push   edx
        movzx  edx, word ptr [esi + 42h]
        add    dx, 3
        out    dx, al
        mov    dword ptr [esi + 70h], 0
        pop    edx
        ret
loc_1_0D53:
        push   edx
        movzx  edx, word ptr [esi + 42h]
        test   word ptr [esi + 8ah], 1
        je     SHORT loc_1_0D9D
        cmp    word ptr [esi + 4ch], 0
        jne    SHORT loc_1_0D74
        mov    eax, 1
        call   sub_1_0710
loc_1_0D74:
        call   sub_1_0629
        mov    ah, 9
        mov    al, 4
        call   sub_1_0607
        call   sub_1_06A2
        cmp    word ptr [esi + 4ch], 0
        jne    SHORT loc_1_0D95
        xor    eax, eax
        call   sub_1_0710
loc_1_0D95:
        and    word ptr [esi + 8ah], 0fffeh
loc_1_0D9D:
        push   ecx
        mov    eax, dword ptr [esi + 2ch]
        db  0CDh, 020h, 006h, 000h, 003h, 000h
        test   ecx, 2
        pop    ecx
        je     SHORT loc_1_0DB8
        or     word ptr [esi + 8ah], 4
loc_1_0DB8:
        pushfd
        cli
        mov    ah, 0ah
        db  0A0h, 000h, 000h, 000h, 000h
        call   sub_1_0607
        mov    ah, 9
        mov    al, 4
        call   sub_1_0607
        xor    al, al
        mov    dx, word ptr [esi + 42h]
        add    dx, 2
        out    dx, al
        sub    dx, 2
        mov    word ptr [esi + 0a0h], 0
        mov    word ptr [esi + 0a2h], 0
        mov    byte ptr [esi + 86h], 0
        db  0C7h, 046h, 070h, 000h, 000h, 000h, 000h
        and    word ptr [esi + 8ah], 0ffefh
        popfd
        pop    edx
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_0E0C:
        pushfd
        push   edx
        push   ecx
        push   eax
        cli
        or     byte ptr [esi + 85h], 80h
        or     word ptr [esi + 8ah], 2
        movzx  ecx, word ptr [esi + 0a0h]
        movzx  edx, word ptr [esi + 42h]
        jmp    SHORT loc_1_0E78
        db  02Eh, 08Bh, 0C0h
loc_1_0E30:
        pushfd
        push   edx
        push   ecx
        push   eax
        mov    cx, word ptr [esi + 0a0h]
        mov    cl, ch
        mov    ch, al
        mov    word ptr [esi + 0a0h], cx
        mov    cl, byte ptr [esi + 86h]
        inc    cl
        cmp    cl, 2
        jb     NEAR PTR loc_1_0F08
        mov    dword ptr [esi + 70h], 0
        mov    byte ptr [esi + 86h], 0
        cli
        movzx  edx, word ptr [esi + 42h]
        movzx  ecx, word ptr [esi + 0a0h]
        mov    word ptr [esi + 0a2h], cx
loc_1_0E78:
        test   word ptr [esi + 4], 200h
        je     SHORT loc_1_0EAE
        movzx  ecx, word ptr [esi + 98h]
        dec    ecx
        cld
        push   ecx
        inc    ecx
        add    edx, 0ah
        push   edx
        mov    al, 6
        out    dx, al
        push   esi
        shr    ecx, 1
        movzx  edx, word ptr [esi + 0aeh]
        mov    esi, dword ptr [esi + 9ch]
        rep outsw
        pop    esi
        pop    edx
        mov    al, 7
        out    dx, al
        sub    edx, 0ah
        pop    ecx
loc_1_0EAE:
        mov    ah, 0fh
        mov    al, cl
        call   sub_1_0607
        mov    ah, 0eh
        mov    al, ch
        call   sub_1_0607
        mov    ah, 6
        mov    al, byte ptr [esi + 8dh]
        cmp    byte ptr [esi + 87h], 0ffh
        je     SHORT loc_1_0ED3
        or     al, 80h
loc_1_0ED3:
        call   sub_1_0607
        mov    ah, 7
        call   sub_1_0607
        mov    ah, 9
        mov    al, 1
        call   sub_1_0607
        mov    ah, 0ah
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 2
        call   sub_1_0607
        or     byte ptr [esi + 85h], 80h
        or     word ptr [esi + 8ah], 2
        xor    cx, cx
loc_1_0F08:
        mov    byte ptr [esi + 86h], cl
        pop    eax
        pop    ecx
        pop    edx
        popfd
        ret
        db  090h
loc_1_0F14:
        pushfd
        push   eax
        push   ecx
        push   edx
        cli
        movzx  edx, word ptr [esi + 42h]
        cmp    word ptr [esi + 4ch], 0
        je     SHORT loc_1_0F29
        call   sub_1_077D
loc_1_0F29:
        test   word ptr [esi + 8ah], 1
        je     SHORT loc_1_0F6E
        cmp    word ptr [esi + 4ch], 0
        jne    SHORT loc_1_0F45
        mov    eax, 1
        call   sub_1_0710
loc_1_0F45:
        call   sub_1_0629
        mov    ah, 9
        mov    al, 4
        call   sub_1_0607
        call   sub_1_06A2
        cmp    word ptr [esi + 4ch], 0
        jne    SHORT loc_1_0F66
        xor    eax, eax
        call   sub_1_0710
loc_1_0F66:
        and    word ptr [esi + 8ah], 0fffeh
loc_1_0F6E:
        movzx  ecx, word ptr [esi + 0a0h]
        mov    word ptr [esi + 0a2h], cx
        or     word ptr [esi + 8ah], 12h
        test   word ptr [esi + 4], 200h
        je     SHORT loc_1_1009
        push   ebx
        push   edx
        mov    ecx, 4
        movzx  eax, word ptr [esi + 96h]
        cmp    ax, word ptr [esi + 0a0h]
        jbe    SHORT loc_1_0FAB
        movzx  eax, word ptr [esi + 0a0h]
        inc    eax
loc_1_0FAB:
        push   eax
loc_1_0FAC:
        mov    eax, dword ptr [esp]
        xor    ebx, ebx
        bts    ebx, ecx
        dec    ebx
        mov    edx, eax
        shr    edx, cl
        cmp    edx, 40h
        jbe    SHORT loc_1_0FC2
        and    eax, ebx
        je     SHORT loc_1_0FC7
loc_1_0FC2:
        dec    ecx
        jecxz  loc_1_0FC7
        jmp    SHORT loc_1_0FAC
loc_1_0FC7:
        pop    eax
        pop    edx
        pop    ebx
        shr    eax, cl
        xchg   ecx, eax
        mov    word ptr [esi + 98h], cx
        add    edx, 0ah
        mov    al, 6
        out    dx, al
        movzx  ecx, word ptr [esi + 98h]
        cld
        push   esi
        push   ecx
        push   edx
        movzx  edx, word ptr [edi + 0aeh]
        mov    esi, dword ptr [esi + 9ch]
        shr    ecx, 1
        rep outsw
        xchg   dword ptr [esp], edx
        mov    al, 7
        out    dx, al
        pop    edx
        mov    ecx, dword ptr [esp]
        shr    ecx, 1
        rep outsw
        pop    ecx
        pop    esi
        dec    ecx
loc_1_1009:
        movzx  edx, word ptr [esi + 42h]
        mov    ah, 0fh
        mov    al, cl
        call   sub_1_0607
        mov    ah, 0eh
        mov    al, ch
        call   sub_1_0607
        mov    ah, 6
        mov    al, byte ptr [esi + 8dh]
        call   sub_1_0607
        mov    ah, 7
        call   sub_1_0607
        mov    ah, 0ah
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 2
        call   sub_1_0607
        mov    ah, 9
        mov    al, 1
        call   sub_1_0607
        pop    edx
        pop    ecx
        pop    eax
        popfd
        ret
        db  090h
loc_1_1050:
        mov    word ptr [esi + 88h], 0
        mov    byte ptr [esi + 86h], 0
        db  0C7h, 046h, 070h, 000h, 000h, 000h, 000h
        ret
loc_1_1068:
        push   eax
        push   ebx
        push   edx
        neg    al
        movzx  ebx, al
        or     ebx, ebx
        jne    SHORT loc_1_1079
        mov    ebx, 100h
loc_1_1079:
        mov    edx, 0fh
        mov    eax, 4240h
        div    bx
        mov    word ptr [esi + 88h], ax
        mov    dword ptr [esi + 70h], 0
        call   sub_1_0896
        pop    edx
        pop    ebx
        pop    eax
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_10A0:
        mov    word ptr [esi + 0a0h], 0
        mov    word ptr [esi + 0a2h], 0
        mov    byte ptr [esi + 86h], 0
        db  0C7h, 046h, 070h, 000h, 000h, 000h, 000h
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_10C4:
        push   edx
        movzx  edx, word ptr [esi + 0a0h]
        mov    dl, dh
        mov    dh, al
        mov    word ptr [esi + 0a0h], dx
        mov    dl, byte ptr [esi + 86h]
        inc    dl
        cmp    dl, 2
        jb     SHORT loc_1_1101
        cmp    edx, 10h
        jae    SHORT loc_1_10F5
        mov    edx, 10h
        mov    word ptr [esi + 0a0h], dx
loc_1_10F5:
        mov    word ptr [esi + 0a2h], dx
        xor    edx, edx
        mov    dword ptr [esi + 70h], edx
loc_1_1101:
        mov    byte ptr [esi + 86h], dl
        pop    edx
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_110C:
        mov    byte ptr [esi + 87h], 0ffh
        ret
loc_1_1114:
        mov    byte ptr [esi + 87h], 0
        ret
loc_1_111C:
        push   eax
        mov    al, byte ptr [esi + 87h]
        call   sub_1_0C5E
        pop    eax
        ret
loc_1_112A:
        cmp    word ptr [esi + 4ch], 0
        je     SHORT loc_1_113C
        push   edx
        movzx  edx, word ptr [esi + 42h]
        call   sub_1_0756
        pop    edx
loc_1_113C:
        and    word ptr [esi + 8ah], 0ffefh
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_1148:
        push   eax
        push   edx
        movzx  edx, word ptr [esi + 42h]
        mov    ah, 6
        mov    al, 3fh
        call   sub_1_0607
        inc    ah
        call   sub_1_0607
        mov    al, 4
        mov    ah, 9
        call   sub_1_0607
        mov    ah, 0ah
        db  0A0h, 000h, 000h, 000h, 000h
        call   sub_1_0607
        xor    al, al
        add    dx, 2
        out    dx, al
        sub    dx, 2
        and    word ptr [esi + 8ah], 0fffdh
        pop    edx
        pop    eax
        ret
loc_1_1189:
        db  0C7h, 046h, 070h, 000h, 000h, 000h, 000h
        ret
loc_1_1191:
        push   eax
        not    al
        call   sub_1_0C5E
        mov    dword ptr [esi + 70h], 0
        pop    eax
        ret
loc_1_11A2:
        db  0C7h, 046h, 070h, 000h, 000h, 000h, 000h
        ret
loc_1_11AA:
        push   esi
        push   edx
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 0a8h]
        or     eax, eax
        jne    SHORT loc_1_11BB
        mov    eax, dword ptr [esi + 38h]
loc_1_11BB:
        db  0CDh, 020h, 004h, 000h, 004h, 000h
        test   dl, 1
        jne    SHORT loc_1_122A
        mov    al, byte ptr [esp]
        mov    ah, byte ptr [esi + 8fh]
        push   ebx
        test   word ptr [esi + 8ah], 8
        jne    SHORT loc_1_1200
        mov    ah, al
        and    ah, 6
        shl    ah, 1
        mov    bl, 40h
        test   al, 10h
        je     SHORT loc_1_11EA
        mov    bl, 20h
loc_1_11EA:
        sub    bl, ah
        add    al, bl
        xor    ah, ah
        mov    byte ptr [esi + 8fh], al
        or     word ptr [esi + 8ah], 8
        jmp    SHORT loc_1_120A
loc_1_1200:
        xor    al, 0a5h
        add    al, ah
        mov    byte ptr [esi + 8fh], al
loc_1_120A:
        mov    byte ptr [esi], al
        inc    esi
        dec    ecx
        cmp    ecx, -1
        jne    SHORT loc_1_1216
        or     dl, 1
loc_1_1216:
        pop    ebx
        mov    eax, dword ptr [esi + 0a8h]
        or     eax, eax
        jne    SHORT loc_1_1224
        mov    eax, dword ptr [esi + 38h]
loc_1_1224:
        db  0CDh, 020h, 005h, 000h, 004h, 000h
loc_1_122A:
        mov    dword ptr [esi + 70h], 0
        pop    eax
        pop    ecx
        pop    edx
        pop    esi
        ret
loc_1_1236:
        db  0C7h, 046h, 070h, 000h, 000h, 000h, 000h
        ret
loc_1_123E:
        mov    byte ptr [esi + 8eh], al
        mov    dword ptr [esi + 70h], 0
        ret
sub_1_124C:
        push   eax
        mov    al, byte ptr [esi + 8eh]
        call   sub_1_0C5E
        pop    eax
        ret
        db  08Bh, 0C0h
loc_1_125C:
        push   eax
        mov    eax, 202h
        call   sub_1_0C5E
        shr    eax, 8
        call   sub_1_0C5E
        mov    eax, 5742h
        call   sub_1_0C5E
        shr    eax, 8
        call   sub_1_0C5E
        pop    eax
        ret
        db  090h
loc_1_1284:
        push   eax
        mov    eax, dword ptr [esi + 2ch]
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        pop    eax
        ret
loc_1_1290:
        call   sub_1_0A90
        jb     NEAR PTR loc_1_1339
        cmp    ecx, 4
        jbe    SHORT loc_1_12A6
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_12A6:
        je     NEAR PTR loc_1_1339
        push   eax
        push   ecx
        push   edx
        test   word ptr [esi + 8ah], 10h
        je     SHORT loc_1_12CA
        pushfd
        cli
        movzx  edx, word ptr [esi + 42h]
        add    edx, 2
        out    dx, al
        sub    edx, 2
        popfd
        jmp    SHORT loc_1_132E
loc_1_12CA:
        test   word ptr [esi + 8ah], 4
        je     SHORT loc_1_12DF
        and    word ptr [esi + 8ah], 0fffbh
        jmp    SHORT loc_1_132E
loc_1_12DF:
        mov    eax, dword ptr [esi + 2ch]
        db  0CDh, 020h, 006h, 000h, 003h, 000h
        test   ecx, 2
        je     SHORT loc_1_132E
        pushfd
        cli
        movzx  edx, word ptr [esi + 42h]
        mov    ah, 9
        mov    al, 4
        call   sub_1_0607
        mov    ah, 0ah
        db  0A0h, 000h, 000h, 000h, 000h
        call   sub_1_0607
        add    edx, 2
        out    dx, al
        sub    edx, 2
        popfd
        test   word ptr [esi + 8ah], 2
        je     SHORT loc_1_132E
        movzx  ecx, word ptr [esi + 0a0h]
        or     ecx, ecx
        jne    SHORT loc_1_132E
        call   sub_1_1148
loc_1_132E:
        pop    edx
        pop    ecx
        pop    eax
        mov    al, byte ptr [esi + 84h]
        clc
        ret
loc_1_1339:
        stc
        ret
sub_1_133B:
        push   eax
        cmp    word ptr [edi + 4ch], 0
        je     SHORT loc_1_134E
        push   edx
        movzx  edx, word ptr [edi + 42h]
        call   sub_1_077D
        pop    edx
loc_1_134E:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 00Dh, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 00Dh, 000h
        mov    ax, word ptr [edi + 8ah]
        test   ax, 2
        je     SHORT loc_1_1378
        mov    esi, edi
        call   sub_1_1148
loc_1_1378:
        and    word ptr [edi + 4], 0bfffh
        pop    eax
        ret
loc_1_1380:
        push   eax
        cmp    al, 49h
        jne    SHORT loc_1_1399
        mov    al, byte ptr [esi + 8dh]
        or     al, al
        je     SHORT loc_1_13A9
        dec    al
        mov    byte ptr [esi + 8dh], al
        jmp    SHORT loc_1_13A9
loc_1_1399:
        mov    al, byte ptr [esi + 8dh]
        inc    al
        and    al, 3fh
        mov    byte ptr [esi + 8dh], al
loc_1_13A9:
        pop    eax
        ret
        db  0CCh
_LTEXT ends
        end
