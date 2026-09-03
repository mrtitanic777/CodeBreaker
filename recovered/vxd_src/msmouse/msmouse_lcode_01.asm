; msmouse_lcode_01.asm -- MSMOUSE object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

MSMINI_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 0E9h, 000h, 0FFh, 0FFh
        cmp    eax, 9
        db  00Fh, 084h, 0E0h, 000h, 0FFh, 0FFh
        cmp    eax, 0fh
        db  00Fh, 084h, 0C7h, 000h, 0FFh, 0FFh
        cmp    eax, 23h
        db  00Fh, 084h, 0A3h, 004h, 0FFh, 0FFh
        clc
        ret
        db  090h
loc_1_0030:
        push   edx
        push   ebp
        db  066h, 089h, 035h, 047h, 005h, 000h, 000h
        db  066h, 089h, 03Dh, 049h, 005h, 000h, 000h
        db  08Bh, 01Dh, 0C4h, 004h, 000h, 000h
        mov    edx, ebx
        db  003h, 015h, 0D0h, 004h, 000h, 000h
        mov    byte ptr [edx + 12h], al
        push   ebx
        call   sub_1_0074
        call   sub_1_00CC
        call   sub_1_0155
        call   sub_1_029A
        pop    ebx
        or     al, 80h
        pop    ebp
        pop    edx
        db  0FFh, 025h, 0D8h, 004h, 000h, 000h
        db  02Eh, 08Bh, 0C0h
sub_1_0074:
        cmp    word ptr [edx + 6], 0
        je     SHORT loc_1_00CA
        push   eax
        push   edx
        mov    ebp, 0ffh
        movsx  ebx, word ptr [edx + 8]
        movsx  ecx, word ptr [edx + 0ah]
        push   edi
        mov    eax, esi
        imul   eax, ecx
        xchg   edi, eax
        imul   eax, ebx
        sub    edi, eax
        db  00Fh, 0BFh, 005h, 0EAh, 004h, 000h, 000h
        add    eax, edi
        cdq
        idiv   ebp
        db  066h, 089h, 015h, 0EAh, 004h, 000h, 000h
        xchg   esi, eax
        imul   eax, ebx
        xchg   edi, eax
        pop    eax
        imul   eax, ecx
        add    edi, eax
        db  00Fh, 0BFh, 005h, 0ECh, 004h, 000h, 000h
        add    eax, edi
        cdq
        idiv   ebp
        db  066h, 089h, 015h, 0ECh, 004h, 000h, 000h
        xchg   edi, eax
        pop    edx
        pop    eax
loc_1_00CA:
        ret
        db  090h
sub_1_00CC:
        db  080h, 03Dh, 0F4h, 004h, 000h, 000h, 000h
        je     SHORT loc_1_0118
        push   eax
        push   edx
        call   sub_1_011C
        xchg   esi, eax
        imul   eax, ecx
        db  00Fh, 0BFh, 01Dh, 0EEh, 004h, 000h, 000h
        add    eax, ebx
        cdq
        mov    ebx, 10h
        idiv   ebx
        db  066h, 089h, 015h, 0EEh, 004h, 000h, 000h
        xchg   esi, eax
        xchg   edi, eax
        imul   eax, ecx
        db  00Fh, 0BFh, 01Dh, 0F0h, 004h, 000h, 000h
        add    eax, ebx
        cdq
        mov    ebx, 10h
        idiv   ebx
        db  066h, 089h, 015h, 0F0h, 004h, 000h, 000h
        xchg   edi, eax
        pop    edx
        pop    eax
loc_1_0118:
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_011C:
        push   edi
        mov    eax, esi
        cdq
        xor    eax, edx
        sub    eax, edx
        xchg   edi, eax
        cdq
        xor    eax, edx
        sub    eax, edx
        cmp    eax, edi
        jae    SHORT loc_1_012F
        xchg   edi, eax
loc_1_012F:
        db  0BFh, 0F5h, 004h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 0F4h, 004h, 000h, 000h
        cld
        inc    edi
        dec    ecx
        je     SHORT loc_1_0145
loc_1_0140:
        scasb
        jb     SHORT loc_1_0145
        loop   loc_1_0140
loc_1_0145:
        db  02Ah, 00Dh, 0F4h, 004h, 000h, 000h
        not    cl
        db  08Ah, 089h, 015h, 005h, 000h, 000h
        pop    edi
        ret
sub_1_0155:
        cmp    byte ptr [edx + 0dh], 0
        jne    SHORT loc_1_015C
        ret
loc_1_015C:
        push   ebx
        mov    bh, al
        and    bh, 3ch
        mov    bl, 3ch
        xor    bl, byte ptr [edx + 11h]
        xor    bl, byte ptr [edx + 10h]
        db  080h, 03Dh, 0F2h, 004h, 000h, 000h, 000h
        je     SHORT loc_1_01B9
        db  080h, 03Dh, 0F2h, 004h, 000h, 000h, 001h
        je     SHORT loc_1_01E0
        db  080h, 03Dh, 0F2h, 004h, 000h, 000h, 002h
        je     SHORT loc_1_01FA
        db  080h, 03Dh, 0F2h, 004h, 000h, 000h, 004h
        je     NEAR PTR loc_1_0235
        db  080h, 03Dh, 0F2h, 004h, 000h, 000h, 084h
        je     NEAR PTR loc_1_0246
        db  080h, 03Dh, 0F2h, 004h, 000h, 000h, 008h
        je     NEAR PTR loc_1_0257
        db  080h, 03Dh, 0F2h, 004h, 000h, 000h, 010h
        je     NEAR PTR loc_1_026B
loc_1_01B9:
        test   bh, 3ch
        jne    SHORT loc_1_01C3
        jmp    NEAR PTR loc_1_0293
loc_1_01C3:
        test   bl, bh
        jne    SHORT loc_1_01CE
        mov    al, bh
        jmp    NEAR PTR loc_1_0298
loc_1_01CE:
        db  0C6h, 005h, 0F2h, 004h, 000h, 000h, 001h
        db  088h, 03Dh, 0F3h, 004h, 000h, 000h
        jmp    NEAR PTR loc_1_0293
loc_1_01E0:
        db  08Ah, 01Dh, 0F3h, 004h, 000h, 000h
        test   bl, bh
        jne    NEAR PTR loc_1_0293
        db  0C6h, 005h, 0F2h, 004h, 000h, 000h, 002h
        jmp    NEAR PTR loc_1_0293
loc_1_01FA:
        db  080h, 03Dh, 0F2h, 004h, 000h, 000h, 002h
        jne    SHORT loc_1_0235
        test   bl, bh
        jne    SHORT loc_1_0221
        test   bh, 3ch
        je     NEAR PTR loc_1_0293
        and    bh, bl
        db  088h, 03Dh, 0F3h, 004h, 000h, 000h
        db  0C6h, 005h, 0F2h, 004h, 000h, 000h, 004h
        jmp    SHORT loc_1_0293
loc_1_0221:
        test   bh, 3ch
        je     SHORT loc_1_0293
        db  088h, 03Dh, 0F3h, 004h, 000h, 000h
        db  0C6h, 005h, 0F2h, 004h, 000h, 000h, 084h
        jmp    SHORT loc_1_0293
loc_1_0235:
        db  022h, 03Dh, 0F3h, 004h, 000h, 000h
        jne    SHORT loc_1_0293
        db  0C6h, 005h, 0F2h, 004h, 000h, 000h, 008h
        jmp    SHORT loc_1_027C
loc_1_0246:
        db  022h, 03Dh, 0F3h, 004h, 000h, 000h
        jne    SHORT loc_1_027C
        db  0C6h, 005h, 0F2h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_027C
loc_1_0257:
        and    bh, 3ch
        je     SHORT loc_1_027C
        db  088h, 03Dh, 0F3h, 004h, 000h, 000h
        db  0C6h, 005h, 0F2h, 004h, 000h, 000h, 010h
        jmp    SHORT loc_1_027C
loc_1_026B:
        db  022h, 03Dh, 0F3h, 004h, 000h, 000h
        jne    SHORT loc_1_027C
        db  0C6h, 005h, 0F2h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_0293
loc_1_027C:
        or     bh, byte ptr [edx + 11h]
        push   cx
        mov    ch, byte ptr [edx + 10h]
        not    ch
        and    bh, ch
        pop    cx
        and    bl, 0c3h
        or     bl, bh
        mov    al, bl
        jmp    SHORT loc_1_0298
loc_1_0293:
        and    bl, 0c3h
        mov    al, bl
loc_1_0298:
        pop    ebx
        ret
sub_1_029A:
        mov    ah, al
        xor    al, al
        test   byte ptr [edx + 11h], ah
        je     SHORT loc_1_02A5
        or     al, 20h
loc_1_02A5:
        test   byte ptr [edx + 10h], ah
        je     SHORT loc_1_02AC
        or     al, 10h
loc_1_02AC:
        ret
sub_1_02AD:
        push   esi
        push   eax
        push   ecx
        movzx  eax, word ptr [ebp + 10h]
        cmp    eax, 0
        jne    SHORT loc_1_02D9
        db  066h, 0A3h, 041h, 005h, 000h, 000h
        db  0BEh, 05Bh, 005h, 000h, 000h
loc_1_02C4:
        mov    byte ptr [esi], al
        add    esi, 4
        db  081h, 0FEh, 06Bh, 005h, 000h, 000h
        jne    SHORT loc_1_02C4
        dec    eax
        db  0A2h, 045h, 005h, 000h, 000h
        jmp    SHORT loc_1_031E
loc_1_02D9:
        mov    eax, 1
        movzx  ecx, word ptr [ebp]
        cmp    ecx, 4
        jae    SHORT loc_1_0320
        mov    eax, 4
        mul    cx
        db  005h, 05Bh, 005h, 000h, 000h
        xchg   esi, eax
        cli
        push   eax
        push   eax
        mov    byte ptr [esi + 3], cl
        movzx  eax, word ptr [ebp + 14h]
        mov    byte ptr [esi], al
        movzx  eax, word ptr [ebp + 4]
        mov    word ptr [esi + 1], ax
        movzx  eax, word ptr [ebp + 18h]
        db  066h, 0A3h, 043h, 005h, 000h, 000h
        movzx  eax, word ptr [ebp + 10h]
        db  066h, 0A3h, 041h, 005h, 000h, 000h
        sti
loc_1_031E:
        xor    eax, eax
loc_1_0320:
        mov    word ptr [ebp + 1ch], ax
        pop    ecx
        pop    eax
        pop    esi
        ret
sub_1_0328:
        cli
        movzx  ecx, word ptr [ebp + 10h]
        db  066h, 089h, 00Dh, 03Bh, 005h, 000h, 000h
        movzx  ecx, word ptr [ebp + 4]
        db  066h, 089h, 00Dh, 039h, 005h, 000h, 000h
        movzx  ecx, word ptr [ebp + 18h]
        db  066h, 089h, 00Dh, 03Fh, 005h, 000h, 000h
        movzx  ecx, word ptr [ebp]
        db  066h, 089h, 00Dh, 03Dh, 005h, 000h, 000h
        sti
        mov    eax, ecx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  066h, 08Bh, 00Dh, 0E8h, 004h, 000h, 000h
        db  08Bh, 015h, 0E4h, 004h, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
sub_1_039C:
        movzx  eax, word ptr [ebp + 4]
        db  0A3h, 0E4h, 004h, 000h, 000h
        mov    ax, word ptr [ebp + 38h]
        db  066h, 0A3h, 0E8h, 004h, 000h, 000h
        ret
        db  055h, 053h, 045h, 052h, 000h, 047h, 065h, 074h, 041h, 073h, 079h, 06Eh, 063h, 04Bh, 065h, 079h
        db  053h, 074h, 061h, 074h, 065h, 000h, 050h, 06Fh, 073h, 074h, 04Dh, 065h, 073h, 073h, 061h, 067h
        db  065h, 000h, 0A1h, 039h, 005h, 000h, 000h, 00Bh, 0C0h, 074h, 007h, 0A1h, 03Dh, 005h, 000h, 000h
        db  00Bh, 0C0h, 0C3h, 066h, 083h, 03Dh, 041h, 005h, 000h, 000h, 000h, 00Fh, 084h, 081h, 000h, 000h
        db  000h, 080h, 03Dh, 045h, 005h, 000h, 000h, 0FFh, 075h, 063h, 053h, 051h, 056h, 0BEh, 05Bh, 005h
        db  000h, 000h, 084h, 006h, 074h, 031h, 08Bh, 01Dh, 0C4h, 004h, 000h, 000h, 0CDh, 020h, 011h, 000h
        db  00Dh, 000h, 00Fh, 0B7h, 04Eh, 001h, 083h, 0F9h, 010h, 075h, 006h, 066h, 0A9h, 001h, 000h, 075h
        db  026h, 083h, 0F9h, 011h, 075h, 006h, 066h, 0A9h, 080h, 000h, 075h, 01Bh, 083h, 0F9h, 012h, 075h
        db  006h, 066h, 0A9h, 000h, 001h, 075h, 010h, 083h, 0C6h, 004h, 081h, 0FEh, 06Bh, 005h, 000h, 000h
        db  075h, 0C0h, 05Eh, 059h, 05Bh, 0EBh, 02Bh, 00Fh, 0B6h, 006h, 0A2h, 046h, 005h, 000h, 000h, 0F6h
        db  0D0h, 0A2h, 045h, 005h, 000h, 000h, 00Fh, 0B6h, 046h, 003h, 05Eh, 059h, 05Bh, 084h, 005h, 046h
        db  005h, 000h, 000h, 075h, 007h, 0C6h, 005h, 045h, 005h, 000h, 000h, 0FFh, 022h, 005h, 045h, 005h
        db  000h, 000h, 0C3h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 04Dh, 053h, 04Dh, 049h
        db  04Eh, 049h, 020h, 020h, 001h, 000h, 000h, 034h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_04D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04E8:
        add    byte ptr [eax], al
loc_1_04EA:
        add    byte ptr [eax], al
loc_1_04EC:
        add    byte ptr [eax], al
loc_1_04EE:
        add    byte ptr [eax], al
loc_1_04F0:
        add    byte ptr [eax], al
loc_1_04F2:
        add    byte ptr [eax], al
loc_1_04F4:
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
        add    bh, bh
loc_1_0546:
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
_LTEXT ends
        end
