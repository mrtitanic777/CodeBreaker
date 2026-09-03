; deccore_icode_01.asm -- DECCORE object 1 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_1_0000:
        call   sub_1_0322
        clc
        ret
loc_1_0007:
        db  089h, 01Dh, 0DCh, 00Fh, 002h, 000h
        call   sub_1_0058
        call   sub_1_01E8
        mov    eax, 37h
        db  0E8h, 0C0h, 001h, 001h, 000h
        db  083h, 00Dh, 0D8h, 00Fh, 002h, 000h, 001h
        db  00Fh, 0BAh, 025h, 0D8h, 00Fh, 002h, 000h, 001h
        jb     SHORT loc_1_003E
        call   sub_1_0348
        call   sub_1_0150
        clc
        ret
loc_1_003E:
        stc
        ret
loc_1_0040:
        db  0C6h, 005h, 028h, 012h, 002h, 000h, 000h
        db  083h, 035h, 0D8h, 00Fh, 002h, 000h, 001h
        db  083h, 035h, 0B4h, 010h, 002h, 000h, 008h
        clc
        ret
        db  000h
sub_1_0058:
        db  066h, 0C7h, 005h, 0C8h, 010h, 002h, 000h, 001h, 000h
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        db  066h, 0A3h, 0B2h, 010h, 002h, 000h
        jecxz  loc_1_0076
        db  083h, 00Dh, 0B4h, 010h, 002h, 000h, 004h
loc_1_0076:
        db  0CDh, 020h, 0C1h, 000h, 001h, 000h
        je     SHORT loc_1_0085
        db  083h, 00Dh, 0B4h, 010h, 002h, 000h, 001h
loc_1_0085:
        db  0C7h, 005h, 0C0h, 010h, 002h, 000h, 037h, 000h, 000h, 000h
        db  083h, 00Dh, 0B4h, 010h, 002h, 000h, 008h
        mov    eax, 24h
        db  0C7h, 005h, 0BCh, 010h, 002h, 000h, 010h, 000h, 000h, 000h
        db  0F7h, 025h, 0BCh, 010h, 002h, 000h
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 0B8h, 010h, 002h, 000h
        or     eax, eax
        jne    SHORT loc_1_00C7
        db  083h, 00Dh, 0D8h, 00Fh, 002h, 000h, 002h
loc_1_00C7:
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    esi, edx
        cld
        db  0BFh, 0D0h, 010h, 002h, 000h
        db  089h, 03Dh, 0C4h, 010h, 002h, 000h
        mov    ecx, 40h
        rep movsb
        push   0
        db  0CDh, 020h, 059h, 000h, 001h, 000h
        add    esp, 4
        db  089h, 015h, 0CCh, 010h, 002h, 000h
        push   edx
        push   ebx
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        bt     ebx, 10h
        jae    SHORT loc_1_014D
        db  081h, 00Dh, 0B4h, 010h, 002h, 000h, 000h, 002h, 000h, 000h
        pushfd
        cli
        pushfd
        pop    eax
        mov    ebx, eax
        xor    eax, 200000h
        push   eax
        popfd
        pushfd
        pop    eax
        popfd
        xor    eax, ebx
        je     SHORT loc_1_014D
        mov    eax, 1
        cpuid
        and    eax, 0f00h
        cmp    eax, 500h
        jb     SHORT loc_1_014D
        db  081h, 00Dh, 0B4h, 010h, 002h, 000h, 000h, 004h, 000h, 000h
        cmp    eax, 600h
        jb     SHORT loc_1_014D
        db  081h, 00Dh, 0B4h, 010h, 002h, 000h, 000h, 008h, 000h, 000h
loc_1_014D:
        pop    ebx
        pop    edx
        ret
sub_1_0150:
        db  068h, 000h, 004h, 001h, 000h
        push   200h
        db  0E8h, 0B9h, 000h, 001h, 000h
        add    esp, 8
        db  0C6h, 005h, 024h, 004h, 001h, 000h, 001h
        db  0C6h, 005h, 025h, 004h, 001h, 000h, 000h
        db  066h, 0C7h, 005h, 026h, 004h, 001h, 000h, 019h, 000h
        db  068h, 014h, 004h, 001h, 000h
        push   100h
        db  0E8h, 090h, 000h, 001h, 000h
        add    esp, 8
        ret
loc_1_018C:
        mov    esi, dword ptr [ebp + 0ch]
        xor    eax, eax
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        cmp    ecx, 3
        ja     SHORT loc_1_01DD
        shl    ecx, 2
        db  0FFh, 0A1h, 0C0h, 003h, 001h, 000h
loc_1_01A5:
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        jmp    SHORT loc_1_01BB
loc_1_01AD:
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        jmp    SHORT loc_1_01BB
loc_1_01B5:
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
loc_1_01BB:
        jb     SHORT loc_1_01D5
        je     SHORT loc_1_01D5
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        ret
loc_1_01C5:
        xor    edx, edx
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        jb     SHORT loc_1_01D5
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], edx
        ret
loc_1_01D5:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ret
loc_1_01DD:
        mov    dword ptr [ebp - 4], 0ffffff9ah
        ret
        db  000h, 000h, 000h
sub_1_01E8:
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 048h, 004h, 001h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_0202
        db  0C6h, 005h, 028h, 012h, 002h, 000h, 0FFh
loc_1_0202:
        mov    eax, 2000h
        db  0BFh, 038h, 004h, 001h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        cmp    eax, 400h
        jl     SHORT loc_1_0249
        cmp    eax, 32000h
        jg     SHORT loc_1_0249
loc_1_0220:
        db  0A3h, 014h, 012h, 002h, 000h
        db  083h, 02Dh, 014h, 012h, 002h, 000h, 005h
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 00Ch, 012h, 002h, 000h
        or     eax, eax
        jne    SHORT loc_1_0248
        db  083h, 00Dh, 0D8h, 00Fh, 002h, 000h, 002h
loc_1_0248:
        ret
loc_1_0249:
        mov    eax, 2000h
        jmp    SHORT loc_1_0220
sub_1_0250:
        cld
        db  0C6h, 005h, 0A7h, 013h, 002h, 000h, 000h
        dec    ecx
        db  089h, 00Dh, 094h, 013h, 002h, 000h
        db  089h, 03Dh, 090h, 013h, 002h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebp
        sub    ebp, 18h
        mov    dword ptr [ebp + 8], 10000600h
        db  0F6h, 005h, 0A8h, 013h, 002h, 000h, 0FFh
        je     SHORT loc_1_028D
        db  0C6h, 005h, 0A8h, 013h, 002h, 000h, 000h
        mov    dword ptr [ebp + 8], 10000605h
loc_1_028D:
        db  0C7h, 045h, 00Ch, 098h, 013h, 002h, 000h
        db  0C7h, 045h, 010h, 0A3h, 013h, 002h, 000h
        mov    dword ptr [ebp + 14h], 0
        db  0E8h, 0D9h, 007h, 001h, 000h
        or     eax, eax
        jne    SHORT loc_1_0319
        mov    dword ptr [ebp + 8], 10000602h
        db  0A1h, 0A3h, 013h, 002h, 000h
        mov    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp + 10h], 0
        mov    dword ptr [ebp + 14h], 2
        db  0E8h, 0B3h, 007h, 001h, 000h
        or     eax, eax
        js     SHORT loc_1_0319
        mov    dword ptr [ebp + 8], 10000603h
        db  0A1h, 0A3h, 013h, 002h, 000h
        mov    dword ptr [ebp + 0ch], eax
        db  0A1h, 094h, 013h, 002h, 000h
        mov    dword ptr [ebp + 10h], eax
        db  0A1h, 090h, 013h, 002h, 000h
        mov    dword ptr [ebp + 14h], eax
        db  0E8h, 08Bh, 007h, 001h, 000h
        or     eax, eax
        js     SHORT loc_1_0319
        mov    dword ptr [ebp + 8], 10000604h
        db  0A1h, 0A3h, 013h, 002h, 000h
        mov    dword ptr [ebp + 0ch], eax
        db  0E8h, 073h, 007h, 001h, 000h
loc_1_030D:
        db  0C6h, 005h, 0A7h, 013h, 002h, 000h, 0FFh
        pop    ebp
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0319:
        db  0C6h, 005h, 028h, 012h, 002h, 000h, 000h
        jmp    SHORT loc_1_030D
sub_1_0322:
        push   1000h
        push   3f0h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_033F
        db  0A3h, 078h, 013h, 002h, 000h
        ret
loc_1_033F:
        db  083h, 00Dh, 0D8h, 00Fh, 002h, 000h, 002h
        ret
        db  000h
sub_1_0348:
        xor    ebx, ebx
        db  0CDh, 020h, 000h, 000h, 014h, 000h
        jb     SHORT loc_1_03BC
        cmp    eax, 30ah
        jb     SHORT loc_1_03BC
        test   ebx, 1
        je     SHORT loc_1_03BC
        db  081h, 00Dh, 0D8h, 00Fh, 002h, 000h, 000h, 002h, 000h, 000h
        db  083h, 00Dh, 0B4h, 010h, 002h, 000h, 020h
        db  088h, 025h, 08Ch, 004h, 001h, 000h
        db  0A2h, 08Dh, 004h, 001h, 000h
        db  088h, 025h, 0B0h, 004h, 001h, 000h
        db  0A2h, 0B1h, 004h, 001h, 000h
        db  068h, 07Ch, 004h, 001h, 000h
        push   100h
        db  0E8h, 081h, 0FEh, 000h, 000h
        add    esp, 8
        db  068h, 0A0h, 004h, 001h, 000h
        push   100h
        db  0E8h, 06Fh, 0FEh, 000h, 000h
        add    esp, 8
        db  0CDh, 020h, 000h, 000h, 081h, 004h
        jb     SHORT loc_1_03BD
        db  083h, 00Dh, 0B4h, 010h, 002h, 000h, 010h
        ret
loc_1_03BC:
        ret
loc_1_03BD:
        ret
        db  000h, 000h
loc_1_03C0:
        movsd
        add    dword ptr [ecx], eax
        db  000h, 0ADh, 001h, 001h, 000h, 0B5h
        add    dword ptr [ecx], eax
        db  000h, 0C5h
        add    dword ptr [ecx], eax
        add    byte ptr [ebp + eax*2 + 43h], al
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        and    byte ptr [esi + 31h], dl
        xor    byte ptr cs:[esi], ch
        xor    byte ptr [edx], dh
        xor    eax, 616f4c20h
        and    byte ptr fs:[ebx + 75h], dl
        arpl   word ptr [ebx + 65h], sp
        jae    SHORT loc_1_0462
        jne    NEAR PTR loc_1_045E
        insb
        jns    SHORT loc_1_0423
        or     eax, 4f43000ah
        push   edx
        inc    ebp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_0400:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        clc
        add    eax, dword ptr [ecx]
        add    byte ptr [ebx + 20h], dl
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        rol    byte ptr [ebx], 1
        add    dword ptr [eax], eax
loc_1_0414:
        inc    esp
        inc    ebp
        inc    ebx
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        xor    byte ptr [ebp + eax*2 + 43h], al
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
loc_1_0423:
        and    byte ptr [eax], al
loc_1_0425:
        add    byte ptr [eax], al
        db  000h, 018h
        add    al, byte ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 4fh], al
        push   edx
        inc    ebp
        push   ebp
        jae    SHORT loc_1_04A4
        jb     SHORT loc_1_0483
        db  075h, 066h
        push   ebx
        imul   edi, dword ptr [edx + 65h], 524f4300h
        inc    ebp
        push   edi
        db  072h, 069h
        db  074h, 065h
        push   ebp
        db  073h, 065h
        db  072h, 042h
        db  075h, 066h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebx
        dec    edi
loc_1_045E:
        push   edx
        inc    ebp
        inc    ecx
        insb
loc_1_0462:
        insb
        dec    ebp
        db  065h, 073h, 073h
        popad
        jae    NEAR PTR loc_1_046C
loc_1_046C:
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        inc    esp
        db  075h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6574h
        add    byte ptr [esi + 45h], cl
        push   esp
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
loc_1_0483:
        xor    byte ptr [esi + 4eh], dl
        inc    ebp
        push   esp
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
loc_1_048C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        insb
        or     al, 2
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 45h], cl
        push   esp
        inc    edx
loc_1_04A4:
        pop    edi
        inc    ebx
        inc    edx
        xor    byte ptr [esi + 4eh], dl
        inc    ebp
        push   esp
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
loc_1_04B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        lcall  4000h, 20ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_ITEXT ends
        end
