; vshare_icode_02.asm -- VSHARE object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  053h, 056h, 057h, 055h, 0A1h, 074h, 003h, 000h, 000h, 08Bh, 070h, 004h, 0A1h, 0BDh, 003h, 000h
        db  000h, 08Bh, 044h, 006h, 004h, 050h, 0E8h, 065h, 015h, 0FEh, 0FFh, 003h, 0C6h, 08Bh, 035h, 074h
        db  003h, 000h, 000h, 033h, 0C9h, 066h, 08Bh, 048h, 004h, 001h, 00Dh, 020h, 018h, 000h, 000h, 08Bh
        db  000h, 050h, 0E8h, 049h, 015h, 0FEh, 0FFh, 08Bh, 035h, 074h, 003h, 000h, 000h, 003h, 046h, 004h
        db  066h, 081h, 038h, 0FFh, 0FFh, 075h, 0DCh, 08Bh, 076h, 004h, 0A1h, 0BDh, 003h, 000h, 000h, 08Bh
        db  044h, 006h, 01Ah, 050h, 0E8h, 027h, 015h, 0FEh, 0FFh, 033h, 0C9h, 066h, 08Bh, 04Ch, 006h, 004h
        db  0A1h, 020h, 018h, 000h, 000h, 003h, 0C1h, 0A3h, 020h, 018h, 000h, 000h, 0C1h, 0E0h, 002h, 050h
        db  0E8h, 02Fh, 000h, 0FEh, 0FFh, 083h, 0C4h, 004h, 0A3h, 024h, 018h, 000h, 000h, 00Bh, 0C0h, 00Fh
        db  084h, 0BCh, 000h, 000h, 000h, 08Bh, 00Dh, 020h, 018h, 000h, 000h, 0C1h, 0E1h, 002h, 051h, 050h
        db  0E8h, 0BEh, 000h, 0FEh, 0FFh, 0A1h, 074h, 003h, 000h, 000h, 08Bh, 070h, 004h, 0A1h, 0BDh, 003h
        db  000h, 000h, 08Bh, 044h, 006h, 004h, 050h, 0E8h, 0D4h, 014h, 0FEh, 0FFh, 08Dh, 014h, 006h, 08Bh
        db  035h, 024h, 018h, 000h, 000h, 0A1h, 074h, 003h, 000h, 000h, 066h, 08Bh, 04Ah, 004h, 08Bh, 078h
        db  004h, 08Dh, 05Ah, 006h, 02Bh, 0DFh, 066h, 08Bh, 0C1h, 066h, 049h, 066h, 00Bh, 0C0h, 074h, 016h
        db  0A1h, 0D4h, 003h, 000h, 000h, 089h, 01Eh, 083h, 0C6h, 004h, 003h, 0D8h, 066h, 08Bh, 0E9h, 066h
        db  049h, 066h, 00Bh, 0EDh, 075h, 0EFh, 08Bh, 002h, 050h, 0E8h, 092h, 014h, 0FEh, 0FFh, 08Dh, 014h
        db  007h, 0A1h, 074h, 003h, 000h, 000h, 066h, 081h, 03Ah, 0FFh, 0FFh, 075h, 0BDh, 08Bh, 078h, 004h
        db  0A1h, 0BDh, 003h, 000h, 000h, 08Bh, 044h, 007h, 01Ah, 050h, 0E8h, 071h, 014h, 0FEh, 0FFh, 003h
        db  0F8h, 0A1h, 074h, 003h, 000h, 000h, 066h, 08Bh, 057h, 004h, 083h, 0C7h, 006h, 02Bh, 078h, 004h
        db  066h, 08Bh, 0C2h, 066h, 04Ah, 066h, 00Bh, 0C0h, 074h, 017h, 0A1h, 0D4h, 003h, 000h, 000h, 090h
        db  089h, 03Eh, 083h, 0C6h, 004h, 003h, 0F8h, 066h, 08Bh, 0CAh, 066h, 04Ah, 066h, 00Bh, 0C9h, 075h
        db  0EFh, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0150:
        push   esi
        push   edi
        test   edx, 10000h
        jne    SHORT loc_2_0184
        db  0FEh, 00Dh, 084h, 003h, 000h, 000h
        db  0C7h, 005h, 0D4h, 003h, 000h, 000h, 035h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 0D8h, 003h, 000h, 000h, 0D1h, 006h
        db  0BFh, 085h, 003h, 000h, 000h
        db  0BEh, 02Ch, 003h, 000h, 000h
        mov    ecx, 13h
        rep movsd
loc_2_0184:
        movzx  edx, dx
        shl    edx, 4
        db  089h, 015h, 080h, 003h, 000h, 000h
        mov    ecx, 13h
        db  0BFh, 085h, 003h, 000h, 000h
loc_2_019A:
        add    dword ptr [edi], edx
        add    edi, 4
        loop   loc_2_019A
        pop    edi
        pop    esi
        ret
VSHARE_DEVICE_INIT:
        push   edi
        push   esi
        db  089h, 01Dh, 074h, 003h, 000h, 000h
        db  089h, 01Dh, 078h, 003h, 000h, 000h
        db  089h, 02Dh, 07Ch, 003h, 000h, 000h
        call   sub_2_0150
        db  0A1h, 0A1h, 003h, 000h, 000h
        mov    esi, eax
        db  0BFh, 030h, 004h, 000h, 000h
        mov    ecx, 0fh
        cld
        rep movsd
        db  0A1h, 099h, 003h, 000h, 000h
        mov    al, byte ptr [eax]
        db  0A2h, 070h, 004h, 000h, 000h
        db  0A1h, 0BDh, 003h, 000h, 000h
        mov    eax, dword ptr [eax + 1ah]
        mov    esi, eax
        push   eax
        db  0E8h, 093h, 013h, 0FEh, 0FFh
        cmp    word ptr [eax + 4], 4
        jne    SHORT loc_2_0253
        db  0A1h, 0A5h, 003h, 000h, 000h
        mov    ax, word ptr [eax]
        or     ax, ax
        jne    SHORT loc_2_0253
        push   204h
        mov    eax, 3c0h
        db  080h, 03Dh, 084h, 003h, 000h, 000h, 001h
        je     SHORT loc_2_0219
        mov    eax, 360h
loc_2_0219:
        push   eax
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_0253
        mov    dword ptr [eax], 0ffffffffh
        mov    word ptr [eax + 4], 10h
        db  089h, 035h, 06Ch, 004h, 000h, 000h
        push   eax
        db  0E8h, 061h, 013h, 0FEh, 0FFh
        mov    edx, eax
        db  0A1h, 0BDh, 003h, 000h, 000h
        mov    dword ptr [eax + 1ah], edx
        db  0A1h, 0A5h, 003h, 000h, 000h
        mov    word ptr [eax], 8
loc_2_0253:
        db  0BEh, 0B4h, 003h, 000h, 000h
        db  0BFh, 0BBh, 003h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        jb     SHORT loc_2_026C
        inc    al
        db  0A2h, 00Ch, 018h, 000h, 000h
loc_2_026C:
        db  0BEh, 0B4h, 003h, 000h, 000h
        db  0BFh, 0D3h, 003h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        jb     SHORT loc_2_0283
        db  0A2h, 014h, 018h, 000h, 000h
loc_2_0283:
        db  0BEh, 0B4h, 003h, 000h, 000h
        db  0BFh, 0ECh, 003h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        jb     SHORT loc_2_029A
        db  0A2h, 010h, 018h, 000h, 000h
loc_2_029A:
        call   0
        db  0BFh, 078h, 003h, 000h, 000h
        mov    ecx, 0fh
        db  0BEh, 000h, 000h, 000h, 000h
loc_2_02AE:
        mov    edx, dword ptr [edi]
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        stosd
        loop   loc_2_02AE
        db  0A1h, 0A1h, 003h, 000h, 000h
        mov    edi, eax
        db  0BEh, 078h, 003h, 000h, 000h
        mov    ecx, 0fh
        cld
        rep movsd
        mov    ecx, 0eh
        push   0
        push   ecx
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_030B
        mov    edi, eax
        mov    esi, eax
        and    esi, 0fh
        shr    eax, 4
        shl    eax, 10h
        or     esi, eax
        db  0A1h, 0A1h, 003h, 000h, 000h
        xchg   dword ptr [eax + 20h], esi
        mov    ecx, 0eh
        db  089h, 035h, 00Ah, 004h, 000h, 000h
        db  0BEh, 000h, 004h, 000h, 000h
        rep movsb
loc_2_030B:
        db  0A1h, 099h, 003h, 000h, 000h
        mov    byte ptr [eax], 0ffh
        clc
        pop    esi
        pop    edi
        ret
VSHARE_SYS_CRITICAL_INIT:
        push   esi
        mov    eax, 2fh
        db  0BEh, 09Eh, 002h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        clc
        pop    esi
        ret
        db  000h
loc_2_032C:
        and    al, 0
        add    byte ptr [eax], al
        pushad
        add    eax, 2de0000h
        add    byte ptr [eax], al
        rol    dword ptr [edx], 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    cl, 2
        add    byte ptr [eax], al
        inc    esp
        add    eax, 5c0000h
        add    byte ptr [eax], al
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax + 3], ah
        add    byte ptr [eax], al
        inc    esi
        add    eax, 2e80000h
        add    byte ptr [eax], al
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [esi], ah
        add    byte ptr [eax], al
        add    byte ptr [esi], dh
        add    eax, 2ea0000h
        add    byte ptr [eax], al
        out    2, al
        add    byte ptr [eax], al
        dec    edx
        db  005h, 000h, 000h
loc_2_0378:
        mov    al, 4
        add    byte ptr [eax], al
        rol    byte ptr [eax + eax], 0
        xor    byte ptr [esi], al
        add    byte ptr [eax], al
        push   eax
        push   es
        add    byte ptr [eax], al
        add    byte ptr [esi], 0
        db  000h, 0B0h, 006h, 000h, 000h, 050h
        pop    es
        add    byte ptr [eax], al
        rol    byte ptr [edi], 0
        db  000h, 0D0h
        or     byte ptr [eax], al
        db  000h, 060h, 009h
        add    byte ptr [eax], al
        inc    eax
        or     al, byte ptr [eax]
        db  000h, 080h, 00Ah, 000h, 000h, 0E0h
        or     al, byte ptr [eax]
        db  000h, 0E0h
        or     eax, dword ptr [eax]
        db  000h, 0A0h, 00Ch, 000h, 000h, 033h
        cmp    byte ptr [esi], dh
        inc    ebp
        dec    esi
        dec    eax
        db  000h
loc_2_03BB:
        dec    ecx
        inc    edi
        dec    esi
        dec    edi
        push   edx
        inc    ebp
        push   ebx
        dec    eax
        inc    ecx
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        push   esi
        dec    ecx
        dec    edi
        dec    esp
        inc    ecx
        push   esp
        dec    ecx
        dec    edi
        dec    esi
        push   ebx
        add    byte ptr [edx + edx*2 + 41h], dl
        inc    esp
        dec    ecx
        push   esp
        dec    ecx
        dec    edi
        dec    esi
        inc    ecx
        dec    esp
        inc    esp
        inc    ebp
        push   esi
        dec    ecx
        inc    ebx
        inc    ebp
        push   ebx
        dec    eax
        inc    ecx
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [ebp + 4eh], al
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        push   ebx
        dec    eax
        inc    ecx
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        push   eax
        dec    edi
        push   eax
        push   ebp
        push   eax
        push   ebx
        add    byte ptr [esi], ah
        cmp    byte ptr [ebp + 34h], 48h
        db  074h, 002h
        clc
        retf
        db  0EAh, 000h, 000h, 000h, 000h
_ITEXT ends
        end
