; ndis2sup_icode_02.asm -- NDIS2SUP object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  02Bh, 0C0h, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0010:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    word ptr [ebp - 0ch], 9
        mov    word ptr [ebp - 0ah], 9
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 045h, 0F8h, 00Ch, 038h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        db  0A3h, 014h, 037h, 000h, 000h
        push   0
        push   0
        push   0
        db  068h, 028h, 037h, 000h, 000h
        db  0E8h, 0FAh, 034h, 0FEh, 0FFh
        push   0
        push   3fh
        db  0E8h, 0C7h, 033h, 0FEh, 0FFh
        add    esp, 8
        db  0A3h, 044h, 037h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_01B2
        mov    byte ptr [eax], 3
        db  0A1h, 044h, 037h, 000h, 000h
        mov    byte ptr [eax + 1], 0
        db  0A1h, 044h, 037h, 000h, 000h
        db  0C7h, 040h, 006h, 010h, 006h, 000h, 000h
        db  0A1h, 044h, 037h, 000h, 000h
        db  0C7h, 040h, 00Ah, 0A0h, 007h, 000h, 000h
        db  0A1h, 044h, 037h, 000h, 000h
        db  0C7h, 040h, 00Eh, 0A0h, 00Eh, 000h, 000h
        db  0A1h, 044h, 037h, 000h, 000h
        db  0C7h, 040h, 012h, 040h, 011h, 000h, 000h
        db  0A1h, 044h, 037h, 000h, 000h
        db  0C7h, 040h, 016h, 060h, 014h, 000h, 000h
        db  0A1h, 044h, 037h, 000h, 000h
        db  0C7h, 040h, 01Ah, 090h, 021h, 000h, 000h
        db  0A1h, 044h, 037h, 000h, 000h
        db  0C7h, 040h, 01Eh, 090h, 021h, 000h, 000h
        db  0A1h, 044h, 037h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        db  0C7h, 040h, 022h, 0D0h, 021h, 000h, 000h
        db  0A1h, 044h, 037h, 000h, 000h
        db  0C7h, 040h, 026h, 070h, 02Ah, 000h, 000h
        db  0A1h, 044h, 037h, 000h, 000h
        push   3fh
        db  0C7h, 040h, 02Ah, 010h, 022h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        db  08Bh, 015h, 044h, 037h, 000h, 000h
        mov    dword ptr [edx + 2eh], eax
        mov    dword ptr [edx + 32h], ecx
        add    edx, 2eh
        db  0A1h, 044h, 037h, 000h, 000h
        push   eax
        push   0
        db  0A1h, 028h, 037h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        db  068h, 048h, 037h, 000h, 000h
        push   eax
        db  0E8h, 036h, 034h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_01A2
        db  0A1h, 077h, 03Ah, 000h, 000h
        db  0A3h, 02Ch, 037h, 000h, 000h
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        add    ecx, eax
        db  003h, 00Dh, 014h, 037h, 000h, 000h
        db  089h, 00Dh, 030h, 037h, 000h, 000h
        db  0C7h, 005h, 050h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 054h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 068h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 060h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 05Ch, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 038h, 037h, 000h, 000h, 000h
        db  0C7h, 005h, 040h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 03Ch, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_2_01C0
        or     eax, eax
        jne    SHORT loc_2_01A2
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
loc_2_01A2:
        push   0
        db  0A1h, 044h, 037h, 000h, 000h
        push   eax
        db  0E8h, 0A7h, 032h, 0FEh, 0FFh
        add    esp, 8
loc_2_01B2:
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_2_01C0:
        push   esi
        db  080h, 03Dh, 076h, 03Ah, 000h, 000h, 000h
        jbe    SHORT loc_2_022C
        db  0BAh, 036h, 03Ah, 000h, 000h
        xor    ecx, ecx
        db  08Ah, 00Dh, 076h, 03Ah, 000h, 000h
loc_2_01D7:
        mov    eax, dword ptr [edx]
        mov    esi, eax
        and    esi, 0ffff0fffh
        shr    esi, 0ch
        and    eax, 0ffffh
        add    esi, eax
        db  003h, 035h, 014h, 037h, 000h, 000h
        mov    eax, dword ptr [esi + 28h]
        mov    esi, eax
        and    esi, 0ffff0fffh
        shr    esi, 0ch
        and    eax, 0ffffh
        add    esi, eax
        db  0A1h, 014h, 037h, 000h, 000h
        mov    ax, word ptr [esi + eax + 44h]
        and    eax, 0ffffh
        db  08Bh, 035h, 090h, 03Ah, 000h, 000h
        cmp    eax, esi
        ja     SHORT loc_2_0221
        mov    eax, esi
loc_2_0221:
        db  0A3h, 090h, 03Ah, 000h, 000h
        add    edx, 4
        dec    ecx
        jne    SHORT loc_2_01D7
loc_2_022C:
        db  0A1h, 090h, 03Ah, 000h, 000h
        add    eax, 4
        db  039h, 005h, 08Ch, 03Ah, 000h, 000h
        jae    SHORT loc_2_0257
        mov    ecx, 8000h
loc_2_0241:
        db  039h, 00Dh, 08Ch, 03Ah, 000h, 000h
        jae    SHORT loc_2_0257
        db  0D1h, 025h, 08Ch, 03Ah, 000h, 000h
        db  039h, 005h, 08Ch, 03Ah, 000h, 000h
        jb     SHORT loc_2_0241
loc_2_0257:
        db  081h, 03Dh, 08Ch, 03Ah, 000h, 000h, 000h, 010h, 000h, 000h
        jae    SHORT loc_2_026D
        db  0C7h, 005h, 08Ch, 03Ah, 000h, 000h, 000h, 010h, 000h, 000h
loc_2_026D:
        push   4208h
        db  0A1h, 08Ch, 03Ah, 000h, 000h
        push   eax
        db  0E8h, 091h, 031h, 0FEh, 0FFh
        add    esp, 8
        db  0A3h, 064h, 037h, 000h, 000h
        db  0A3h, 07Ch, 03Ah, 000h, 000h
        or     eax, eax
        jne    SHORT loc_2_0295
        mov    eax, 0c000009ah
        pop    esi
        ret
loc_2_0295:
        db  0A1h, 064h, 037h, 000h, 000h
        push   eax
        call   sub_2_0672
        add    esp, 4
        db  0C7h, 005h, 074h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 08Ch, 03Ah, 000h, 000h
        sub    eax, 4
        db  0A3h, 070h, 037h, 000h, 000h
        db  0A1h, 08Ch, 03Ah, 000h, 000h
        dec    eax
        db  0A3h, 07Ch, 037h, 000h, 000h
        db  0E8h, 02Ch, 0FDh, 0FDh, 0FFh
        cmp    eax, 0c0000001h
        jne    SHORT loc_2_02E2
        mov    eax, 0c0000001h
        pop    esi
        ret
loc_2_02E2:
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_02F0:
        sub    eax, eax
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0300:
        stc
        ret
NDIS2SUP_SYS_CRITICAL_INIT:
        clc
        ret
sub_2_0304:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        push   0
        push   18h
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_2_03C5
        mov    edi, eax
        mov    dword ptr [edi], 5349444eh
        mov    dword ptr [edi + 4], 24504c48h
        mov    byte ptr [edi + 8], 0
        shr    eax, 4
        mov    word ptr [ebp + 1ch], 3d40h
        mov    word ptr [ebp + 3ch], ax
        mov    word ptr [ebp + 14h], 0
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  ecx, word ptr [ebp + 1ch]
        test   word ptr [ebp + 2ch], 1
        jne    SHORT loc_2_03BF
        mov    word ptr [ebp + 10h], cx
        mov    dword ptr [edi], 3733e96fh
        mov    word ptr [ebp + 1ch], 4402h
        mov    word ptr [ebp + 18h], 8
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   word ptr [ebp + 2ch], 1
        jne    SHORT loc_2_03B0
        cmp    word ptr [ebp + 1ch], 8
        jne    SHORT loc_2_03B0
        cmp    dword ptr [edi], 0ef6f3733h
        jne    SHORT loc_2_03B0
        mov    eax, dword ptr [edi + 4]
        db  0A3h, 07Ch, 038h, 000h, 000h
loc_2_03B0:
        mov    byte ptr [ebp + 1dh], 3eh
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_2_03BF:
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
loc_2_03C5:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  083h, 03Dh, 07Ch, 038h, 000h, 000h, 001h
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
sub_2_03E7:
        push   eax
        push   ebx
        push   ecx
        push   edx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, 6
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  00Fh, 0B7h, 015h, 07Ch, 038h, 000h, 000h
        db  066h, 08Bh, 00Dh, 07Eh, 038h, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        add    word ptr [ebp + 30h], 2
        movzx  edx, word ptr [ebp + 14h]
        movzx  eax, word ptr [ebp + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        cmp    eax, 1
        jb     NEAR PTR loc_2_04DD
        cmp    edx, 1
        jb     NEAR PTR loc_2_04DD
        db  00Fh, 0B7h, 035h, 07Eh, 038h, 000h, 000h
        shl    esi, 4
        add    esi, eax
        db  0BFh, 0A0h, 038h, 000h, 000h
        cmp    word ptr [esi + 21h], 1
        jae    SHORT loc_2_0478
        cmp    byte ptr [esi + 2eh], 0
        stc
        jne    SHORT loc_2_04DD
loc_2_0478:
        movzx  ecx, byte ptr [esi + 2eh]
        mov    eax, ecx
        imul   ecx, ecx, 1ch
        add    ecx, 34h
        cld
        rep movsb
        mov    ecx, eax
        imul   ecx, ecx, 10h
        add    ecx, 2
        db  0BFh, 0B4h, 039h, 000h, 000h
        rep movsb
        db  0BFh, 036h, 03Ah, 000h, 000h
        db  0BEh, 0D4h, 038h, 000h, 000h
        mov    ecx, eax
loc_2_04A2:
        movsd
        add    esi, 18h
        loop   loc_2_04A2
        db  0A0h, 0CEh, 038h, 000h, 000h
        db  0A2h, 076h, 03Ah, 000h, 000h
        db  066h, 0A1h, 0BFh, 038h, 000h, 000h
        db  066h, 0A3h, 09Eh, 038h, 000h, 000h
        db  00Fh, 0B7h, 035h, 07Eh, 038h, 000h, 000h
        shl    esi, 4
        add    esi, edx
        db  089h, 035h, 098h, 038h, 000h, 000h
        db  0BFh, 080h, 038h, 000h, 000h
        mov    ecx, 6
        rep movsd
        clc
loc_2_04DD:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_2_04E2:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        db  080h, 03Dh, 0CEh, 038h, 000h, 000h, 000h
        je     SHORT loc_2_054F
        db  0BEh, 021h, 031h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        push   eax
        db  0BEh, 0DBh, 030h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        push   eax
        db  0BEh, 0A6h, 030h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        push   eax
        db  0BEh, 054h, 030h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        push   eax
        db  0BEh, 023h, 030h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        push   eax
        db  0BEh, 0EDh, 02Fh, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        push   eax
        mov    ecx, 6
        mov    esi, esp
        db  08Bh, 03Dh, 098h, 038h, 000h, 000h
        pushfd
        cli
        cld
        rep movsd
        popfd
        add    esp, 18h
loc_2_054F:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_2_0556:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        db  08Bh, 03Dh, 094h, 006h, 000h, 000h
        db  0BEh, 098h, 006h, 000h, 000h
        mov    ecx, 210h
        rep movsb
        db  08Bh, 035h, 094h, 006h, 000h, 000h
        mov    eax, esi
        db  0A1h, 094h, 006h, 000h, 000h
        add    eax, 0
        db  0A3h, 088h, 03Ah, 000h, 000h
        mov    eax, esi
        add    esi, 6
        shr    eax, 4
        mov    ecx, 10h
loc_2_0591:
        mov    word ptr [esi], ax
        add    esi, 20h
        loop   loc_2_0591
        db  066h, 0A3h, 079h, 03Ah, 000h, 000h
        db  066h, 0C7h, 005h, 077h, 03Ah, 000h, 000h, 000h, 002h
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_2_05AE:
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        db  0A0h, 094h, 03Ah, 000h, 000h
        cmp    al, 0
        jne    NEAR PTR loc_2_0647
        db  0FEh, 005h, 094h, 03Ah, 000h, 000h
        push   5008h
        push   1000h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_0300
        db  0A3h, 094h, 006h, 000h, 000h
        add    eax, 210h
        mov    edi, eax
        add    eax, 400h
        shl    edi, 0ch
        mov    di, 400h
        db  089h, 03Dh, 078h, 038h, 000h, 000h
        cld
        call   0
        cmp    eax, 0c0000001h
        je     NEAR PTR loc_2_0300
        call   sub_2_0304
        jb     NEAR PTR loc_2_0300
        call   sub_2_03E7
        jb     NEAR PTR loc_2_0300
        call   sub_2_0556
        push   ebx
        cld
        call   sub_2_0010
        add    esp, 4
        cmp    eax, 0c0000001h
        je     NEAR PTR loc_2_0300
        call   sub_2_04E2
        ret
loc_2_0647:
        clc
        ret
NDIS2SUP_INIT_COMPLETE:
        push   ebx
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        jb     SHORT loc_2_0658
        db  088h, 01Dh, 024h, 038h, 000h, 000h
loc_2_0658:
        pop    ebx
        cld
        call   sub_2_02F0
        cmp    eax, 0c0000001h
        je     NEAR PTR loc_2_0300
        db  0FFh, 005h, 020h, 038h, 000h, 000h
        clc
        ret
sub_2_0672:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 088h, 03Ah, 000h, 000h
        rol    eax, 0ch
        sub    ecx, 20h
loc_2_0682:
        add    ecx, 20h
        mov    dword ptr [ecx + 14h], eax
        mov    dword ptr [ecx + 1ch], eax
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_2_0682
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0698:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        sub    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        push   0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [88000000h], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    al, cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    al, ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        push   1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [88000000h], cl
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    al, cl
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0E8h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 054h
        db  054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h
_ITEXT ends
        end
