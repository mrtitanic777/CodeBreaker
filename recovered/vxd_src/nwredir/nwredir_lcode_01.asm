; nwredir_lcode_01.asm -- NWREDIR object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

NWREDIR_Control:
        cmp    eax, 25h
        jae    SHORT loc_1_000C
        db  0FFh, 024h, 085h, 00Eh, 000h, 000h, 000h
loc_1_000C:
        clc
        ret
        db  000h, 000h, 000h, 000h, 053h, 000h, 000h, 000h, 0F1h, 005h, 000h, 000h, 0D1h, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 0F0h, 000h, 000h, 000h, 0FAh, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 004h, 001h, 000h, 000h, 00Eh, 001h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  0B0h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 0C6h, 000h, 000h, 000h, 08Ah, 000h, 000h, 000h
        db  0A9h, 000h, 000h, 000h
loc_1_00A2:
        cmp    word ptr [ebp + 1ch], 4a19h
        db  00Fh, 084h, 066h, 0FFh, 0FEh, 0FFh
loc_1_00AE:
        stc
        ret
NWREDIR_DEBUG_QUERY:
        lea    edx, [esi - 1]
loc_1_00B3:
        inc    edx
        cmp    byte ptr fs:[edx], 20h
        je     SHORT loc_1_00B3
        push   dword ptr fs:[edx]
        db  0E8h, 0FEh, 004h, 0FFh, 0FFh
        add    esp, 4
        ret
NWREDIR_PNP_NEW_DEVNODE:
        push   edx
        push   ebx
        db  0E8h, 0B3h, 0CAh, 0FFh, 0FFh
        add    esp, 8
        cmp    eax, 1
        cmc
        ret
loc_1_00D5:
        db  0E8h, 0CFh, 0FFh, 0FEh, 0FFh
        ret
        db  000h, 000h, 000h, 000h, 000h
sub_1_00E0:
        sub    esp, 4
        push   ebx
        mov    ecx, dword ptr [esp + 10h]
        mov    dx, word ptr [ecx + 20h]
        add    ecx, 20h
        db  0A1h, 0B8h, 005h, 000h, 000h
        cmp    word ptr [eax + 4ah], dx
        je     SHORT loc_1_0101
        sub    eax, eax
        pop    ebx
        add    esp, 4
        ret
loc_1_0101:
        cmp    dx, 400h
        jne    SHORT loc_1_0140
        db  0B8h, 016h, 004h, 000h, 000h
        lea    edx, [ecx + 2]
loc_1_0110:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [edx]
        jne    SHORT loc_1_0130
        or     bl, bl
        je     SHORT loc_1_012C
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [edx + 1]
        jne    SHORT loc_1_0130
        add    eax, 2
        add    edx, 2
        or     bl, bl
        jne    SHORT loc_1_0110
loc_1_012C:
        sub    eax, eax
        jmp    SHORT loc_1_0135
loc_1_0130:
        sbb    eax, eax
        sbb    eax, -1
loc_1_0135:
        cmp    eax, 1
        sbb    al, al
        neg    al
        jmp    SHORT loc_1_0144
        db  08Bh, 0FFh
loc_1_0140:
        mov    al, byte ptr [esp + 7]
loc_1_0144:
        or     al, al
        jne    SHORT loc_1_0180
        db  066h, 083h, 03Dh, 050h, 004h, 000h, 000h, 000h
        je     SHORT loc_1_0180
        db  066h, 083h, 03Dh, 05Ch, 004h, 000h, 000h, 000h
        jne    SHORT loc_1_0199
        db  0BAh, 052h, 004h, 000h, 000h
        mov    eax, dword ptr [ecx + 32h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 36h]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 3ah]
        add    ecx, 32h
        mov    dword ptr [edx + 8], eax
        sub    eax, eax
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_0180:
        db  0BAh, 046h, 004h, 000h, 000h
        mov    eax, dword ptr [ecx + 32h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 36h]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 3ah]
        add    ecx, 32h
        mov    dword ptr [edx + 8], eax
loc_1_0199:
        sub    eax, eax
        pop    ebx
        add    esp, 4
        ret
sub_1_01A0:
        cmp    dword ptr [esi + 1ch], 0
        je     SHORT loc_1_01DE
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 20h]
        mov    dword ptr [eax + 20h], edx
        mov    dword ptr [edx + 1ch], eax
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 1ch], 0
        mov    edx, dword ptr [esi + 28h]
loc_1_01BF:
        cmp    edx, esi
        je     SHORT loc_1_01CF
        mov    eax, edx
        mov    edx, dword ptr [edx + 28h]
        db  068h, 0BFh, 001h, 000h, 000h
        jmp    SHORT loc_1_01F6
loc_1_01CF:
        cmp    dword ptr [ecx + 50h], 0
        jne    SHORT loc_1_01DE
        push   ecx
        call   sub_1_0270
        add    esp, 4
loc_1_01DE:
        db  039h, 035h, 03Ch, 01Ah, 000h, 000h
        je     SHORT loc_1_01E7
        ret
loc_1_01E7:
        db  0C7h, 005h, 03Ch, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_1_01F2:
        mov    eax, dword ptr [esp + 4]
loc_1_01F6:
        db  0FFh, 00Dh, 054h, 01Ah, 000h, 000h
        db  083h, 03Dh, 050h, 01Ah, 000h, 000h, 028h
        jb     SHORT loc_1_0213
        db  0FFh, 035h, 070h, 01Ah, 000h, 000h
        pop    dword ptr [eax]
        db  0A3h, 070h, 01Ah, 000h, 000h
loc_1_0212:
        ret
loc_1_0213:
        db  0FFh, 035h, 060h, 01Ah, 000h, 000h
        pop    dword ptr [eax]
        db  0A3h, 060h, 01Ah, 000h, 000h
        db  0FFh, 005h, 050h, 01Ah, 000h, 000h
        jmp    SHORT loc_1_0212
sub_1_0228:
        db  08Ah, 025h, 016h, 00Eh, 000h, 000h
        db  0CDh, 020h, 002h, 080h, 08Bh, 004h
sub_1_0234:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax]
loc_1_023A:
        bts    dword ptr [eax], 0fh
        jb     SHORT loc_1_0243
        dec    dword ptr [eax]
        ret
loc_1_0243:
        pushad
        push   eax
        db  0CDh, 020h, 012h, 000h, 040h, 000h
        pop    eax
        popad
        jmp    SHORT loc_1_023A
sub_1_024F:
        mov    eax, dword ptr [esp + 4]
        and    word ptr [eax], 7fffh
        jne    SHORT loc_1_025B
loc_1_025A:
        ret
loc_1_025B:
        pushad
        push   eax
        db  0CDh, 020h, 013h, 000h, 040h, 000h
        add    esp, 4
        popad
        jmp    SHORT loc_1_025A
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0270:
        mov    ecx, dword ptr [esp + 4]
        cmp    dword ptr [ecx + 1ch], ecx
        jne    SHORT loc_1_02B5
        mov    eax, ecx
        sub    eax, dword ptr [ecx + 0ch]
        cmp    eax, 10h
        jne    SHORT loc_1_02B5
        cmp    dword ptr [ecx + 4ch], 0
        jne    SHORT loc_1_02B5
        cmp    dword ptr [ecx + 58h], 0
        jne    SHORT loc_1_02B5
        cmp    dword ptr [ecx + 2ch], 0
        jne    SHORT loc_1_02B5
        mov    eax, dword ptr [ecx + 8]
        or     dword ptr [ecx + 54h], 82h
        cmp    word ptr [eax + 26h], 0
        je     SHORT loc_1_02B0
        or     word ptr [eax + 24h], 1
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_02B0:
        call   sub_1_02C0
loc_1_02B5:
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_02C0:
        push   esi
        mov    esi, ecx
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 78h]
        or     eax, eax
        je     SHORT loc_1_02E2
        push   eax
        db  0E8h, 0FFh, 007h, 0FFh, 0FFh
        add    esp, 4
loc_1_02E2:
        push   esi
        db  0E8h, 0F6h, 007h, 0FFh, 0FFh
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_02F0:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    8
        db  000h, 000h, 000h
sub_1_0308:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_1_0324:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_032C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h, 008h, 002h, 000h, 000h, 00Fh, 002h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 08Eh, 004h, 001h, 000h, 000h, 000h, 04Eh, 057h, 052h, 045h
        db  044h, 049h, 052h, 020h, 000h, 080h, 001h, 0A0h, 000h, 000h, 000h, 000h, 0FBh, 001h, 000h, 000h
        db  0FBh, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  03Ch, 003h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_03B4:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_03B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03C4:
        add    byte ptr [eax], al
loc_1_03C6:
        add    byte ptr [eax], al
loc_1_03C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03EC:
        add    byte ptr [eax], al
loc_1_03EE:
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
loc_1_0404:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0408:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_040C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0410:
        add    byte ptr [eax], al
loc_1_0412:
        add    byte ptr [eax], al
loc_1_0414:
        add    byte ptr [eax], al
loc_1_0416:
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
loc_1_0446:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0450:
        add    byte ptr [eax], al
loc_1_0452:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_045C:
        add    byte ptr [eax], al
loc_1_045E:
        add    byte ptr [eax], al
loc_1_0460:
        add    byte ptr [eax], al
        db  0E8h, 003h, 000h, 000h, 000h
loc_1_0467:
        add    byte ptr [eax], al
loc_1_0469:
        add    byte ptr [eax], al
loc_1_046B:
        add    byte ptr [eax], al
loc_1_046D:
        add    byte ptr [eax], al
loc_1_046F:
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
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
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
        db  000h
loc_1_0DDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DE0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DE8:
        add    byte ptr [eax], al
loc_1_0DEA:
        add    byte ptr [eax], al
loc_1_0DEC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DF4:
        add    byte ptr [eax], al
loc_1_0DF6:
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
loc_1_0E12:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E16:
        add    byte ptr [ecx], dl
        add    byte ptr [edi - 4dh], bl
        mov    ah, 0b5h
        mov    dh, 0b8h
        mov    edx, 0bebdbcbbh
        ret    0c4c3h
        db  0C5h, 0C6h, 0C7h, 0CBh, 0CDh, 0CFh, 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h, 0DAh
loc_1_0E36:
        fcmovnu st(0), st(4)
        fstp   st(6)
loc_1_0E3A:
        fnstsw ax
        db  0E1h, 0E2h
        db  0E3h, 0E7h
        db  0E9h, 0EAh, 0EBh, 0ECh, 0EDh
        db  0EEh, 0EFh, 0F0h, 0F1h, 0F2h, 0F3h, 0B0h, 0B1h, 0B2h, 0B7h, 0B9h, 0BFh, 0C0h, 0C1h, 0C8h, 0C9h
        db  0CAh, 0CCh, 0CEh, 0D8h, 0D9h, 0E4h, 0E5h
sub_1_0E5C:
        out    0e8h, al
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        db  000h, 0F0h
        jae    SHORT loc_1_0E67
loc_1_0E67:
        db  000h, 020h
        daa
        add    byte ptr [eax], al
        db  070h, 078h
        add    byte ptr [eax], al
        cmp    byte ptr [eax], 0
        pushad
        jns    SHORT loc_1_0E77
loc_1_0E77:
        db  000h, 0C0h
        jns    SHORT loc_1_0E7B
loc_1_0E7B:
        db  000h, 050h, 07Bh
        add    byte ptr [eax], al
        nop
        jg     SHORT loc_1_0E83
loc_1_0E83:
        db  000h, 0B0h, 07Fh, 000h, 000h, 0E0h
        jg     SHORT loc_1_0E8B
loc_1_0E8B:
        db  000h, 070h, 080h
        add    byte ptr [eax], al
        jo     SHORT loc_1_0E12
        add    byte ptr [eax], al
        lock add byte ptr [eax], 0
        push   eax
        db  081h, 000h, 000h, 050h, 081h, 000h
        db  000h, 0B0h, 081h, 000h, 000h, 0E0h
        add    byte ptr [eax], 0
        db  010h, 083h, 000h, 000h, 060h, 084h
        add    byte ptr [eax], al
        loopne loc_1_0E36
        add    byte ptr [eax], al
        loopne loc_1_0E3A
        add    byte ptr [eax], al
        db  030h, 085h, 000h, 000h, 0D0h, 085h
        add    byte ptr [eax], al
        db  080h, 086h, 000h, 000h, 0F0h, 086h, 000h
        db  000h, 080h, 086h, 000h, 000h, 0F0h
        xchg   byte ptr [eax], al
        db  000h, 060h, 087h
        add    byte ptr [eax], al
        db  070h, 087h
        add    byte ptr [eax], al
        db  000h, 088h, 000h, 000h, 060h, 088h
        add    byte ptr [eax], al
        db  0A0h, 088h, 000h, 000h, 000h
        mov    dword ptr [eax], eax
        db  000h, 010h
        mov    dword ptr [eax], eax
        db  000h, 040h, 089h
        add    byte ptr [eax], al
        db  030h, 08Bh, 000h, 000h, 030h, 08Bh
        add    byte ptr [eax], al
        db  030h, 08Bh, 000h, 000h, 030h, 08Bh
        add    byte ptr [eax], al
        db  080h, 091h, 000h, 000h, 000h, 092h, 000h
        db  000h, 0A0h, 097h, 000h, 000h, 0D0h
        cdq
        add    byte ptr [eax], al
        db  0D0h, 09Ah, 000h, 000h, 0D0h, 09Ah
        add    byte ptr [eax], al
        nop
        wait
        add    byte ptr [eax], al
        db  0E0h, 09Bh
        add    byte ptr [eax], al
        nop
        pushfd
        add    byte ptr [eax], al
        db  020h, 09Eh, 000h, 000h, 020h, 0A0h
        add    byte ptr [eax], al
        push   eax
        db  0A3h, 000h, 000h, 0F0h, 0A1h
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
        push   es
        pop    es
        add    byte ptr [eax], al
loc_1_0F7C:
        dec    esi
        push   edi
        push   edx
        inc    ebp
        inc    esp
        dec    ecx
        push   edx
        add    byte ptr [ebp + 41h], cl
        pop    eax
        inc    ebx
        dec    edi
        dec    esi
        dec    esi
        inc    ebp
        inc    ebx
        push   esp
        dec    ecx
        dec    edi
        dec    esi
        push   ebx
        add    byte ptr [eax + 52h], dl
        inc    ebp
        inc    esi
        inc    ebp
        push   edx
        push   edx
        inc    ebp
        inc    esp
        push   ebx
        inc    ebp
        push   edx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [ecx + 50h], al
        dec    ecx
        push   ebx
        push   ebp
        push   eax
        push   eax
        dec    edi
        push   edx
        push   esp
        add    byte ptr [ebp + 41h], cl
        pop    eax
        push   edx
        inc    ebp
        push   esp
        push   edx
        pop    ecx
        add    byte ptr [ebp + 49h], cl
        dec    esi
        push   edx
        inc    ebp
        push   esp
        push   edx
        pop    ecx
        add    byte ptr [ebp + 41h], cl
        pop    eax
        dec    esp
        dec    ecx
        push   eax
        add    byte ptr [ebx + 55h], dl
        push   eax
        push   eax
        dec    edi
        push   edx
        push   esp
        dec    esp
        inc    esi
        dec    esi
        add    byte ptr [eax + 52h], dl
        inc    ebp
        push   ebx
        inc    ebp
        push   edx
        push   esi
        inc    ebp
        inc    ebx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [edi + 50h], cl
        push   esp
        dec    ecx
        dec    ebp
        dec    ecx
        pop    edx
        inc    ebp
        push   ebx
        inc    esi
        dec    esi
        add    byte ptr [ebx + 55h], dl
        push   eax
        push   eax
        dec    edi
        push   edx
        push   esp
        push   esp
        push   ebp
        dec    esi
        dec    esi
        inc    ebp
        dec    esp
        dec    esp
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [ebx + 55h], dl
        push   eax
        push   eax
        dec    edi
        push   edx
        push   esp
        inc    edx
        push   ebp
        push   edx
        push   ebx
        push   esp
        add    byte ptr [esi + 55h], al
        dec    esp
        dec    esp
        push   esp
        push   edx
        inc    ecx
        inc    ebx
        inc    ebp
        add    byte ptr [edx + 45h], dl
        inc    ecx
        inc    esp
        inc    ebx
        inc    ecx
        inc    ebx
        dec    eax
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [ebx + 48h], dl
        dec    edi
        push   edi
        inc    esp
        dec    edi
        push   esp
        push   ebx
        add    byte ptr [edi + 41h], al
        push   eax
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [esi + 49h], al
        push   edx
        push   ebx
        push   esp
        dec    esi
        inc    ebp
        push   esp
        push   edi
        dec    edi
        push   edx
        dec    ebx
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        add    byte ptr [ebx + 45h], dl
        inc    ecx
        push   edx
        inc    ebx
        dec    eax
        dec    ebp
        dec    edi
        inc    esp
        inc    ebp
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
        add    byte ptr [ebx + 53000000h], al
        pop    ecx
        push   ebx
        push   esp
        inc    ebp
        dec    ebp
        pop    esp
        inc    ebx
        db  075h, 072h
        jb     SHORT loc_1_10E1
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_1_10E5
        push   ebx
        jb     NEAR PTR loc_1_1103
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        dec    esi
        push   edi
        push   edx
        inc    ebp
        inc    esp
        dec    ecx
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dl
        add    al, 0
        adc    byte ptr [eax], al
        dec    ebp
        add    byte ptr [ebp], al
        push   ebx
        add    byte ptr [ebx], dl
        dec    esi
        add    byte ptr [edi], al
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 53h], cl
        inc    esp
        dec    edi
        push   ebx
        add    byte ptr [esi + 37h], dl
        xor    byte ptr cs:[eax], dh
        add    byte ptr [ecx + 42h], cl
        dec    ebp
        pop    edi
        push   eax
        inc    ebx
        add    byte ptr [ecx + 42h], cl
        dec    ebp
        add    byte ptr [eax], al
loc_1_10D4:
        push   ebx
        db  079h, 073h
        je     SHORT loc_1_113E
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        jb     SHORT loc_1_1145
        outsb
loc_1_10E1:
        je     SHORT loc_1_1126
        outsd
        outsb
loc_1_10E5:
        je     SHORT loc_1_1159
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        dec    esi
        push   edi
        push   edx
        inc    ebp
        inc    esp
        dec    ecx
        push   edx
        add    byte ptr [eax], al
loc_1_1103:
        add    byte ptr [ebx + 68h], dl
        insb
        insb
        push   esi
        db  065h, 072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 0
loc_1_1114:
        dec    ebp
        popad
        js     SHORT loc_1_115B
        outsd
        outsb
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        jae    SHORT loc_1_1123
loc_1_1123:
        add    byte ptr [ebp + 61h], cl
loc_1_1126:
        js     SHORT loc_1_117A
        db  065h, 074h, 072h
        jns    SHORT loc_1_112D
loc_1_112D:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 69h], cl
        outsb
        push   edx
        je     NEAR PTR loc_1_11A9
        jns    SHORT loc_1_1139
loc_1_1139:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 61h], cl
loc_1_113E:
        db  078h, 04Ch
        dec    ecx
        push   eax
        add    byte ptr [eax], al
loc_1_1144:
        inc    edi
loc_1_1145:
        popad
        db  070h, 054h
        imul   ebp, dword ptr [ebp + 65h], 72694600h
        jae    SHORT loc_1_11C5
        dec    esi
        db  065h, 074h, 077h
        outsd
        db  072h, 06Bh
        inc    esp
loc_1_1159:
        jb     SHORT loc_1_11C4
loc_1_115B:
        jbe    SHORT loc_1_11C2
        add    byte ptr [eax], al
        add    byte ptr [ebx + 65h], dl
        popad
        jb     SHORT loc_1_11C8
        push   65646f4dh
        add    byte ptr [eax], al
loc_1_116C:
        push   ebx
        db  075h, 070h
        jo     SHORT loc_1_11E0
        db  072h, 074h
        inc    edx
        db  075h, 072h
        jae    SHORT loc_1_11EC
        add    byte ptr [eax], al
loc_1_117A:
        add    byte ptr [eax], al
loc_1_117C:
        push   ebx
        db  075h, 070h
        jo     SHORT loc_1_11F0
        db  072h, 074h
        dec    esp
        inc    esi
        dec    esi
        add    byte ptr [eax], al
loc_1_1188:
        push   eax
        jb     SHORT loc_1_11F0
        db  073h, 065h
        jb     SHORT loc_1_1205
        inc    ebx
        popad
        db  073h, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1198:
        dec    edi
        db  070h, 074h
        imul   ebp, dword ptr [ebp + 69h], 4653657ah
        dec    esi
        add    byte ptr [ebx + 75h], dl
        db  070h, 070h
        outsd
loc_1_11A9:
        db  072h, 074h
        push   esp
        jne    SHORT loc_1_121C
        outsb
        insb
        insb
        imul   ebp, dword ptr [esi + 67h], 53000000h
        push   6f44776fh
        je     SHORT loc_1_1233
        add    byte ptr [eax], al
loc_1_11C2:
        add    byte ptr [eax], al
loc_1_11C4:
        inc    esi
loc_1_11C5:
        jne    SHORT loc_1_1233
        insb
loc_1_11C8:
        push   esp
        db  072h, 061h
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
loc_1_11D0:
        push   edx
        popad
        inc    ebx
        popad
        arpl   word ptr [eax + 69h], bp
        outsb
        add    byte ptr [si + 61h], cl
        db  072h, 067h
loc_1_11E0:
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 657a6953h
        inc    ebx
        outsd
        insd
loc_1_11EC:
        jo     SHORT loc_1_124F
        je     SHORT loc_1_1259
loc_1_11F0:
        bound  ebp, qword ptr [ecx + 6ch]
        imul   esi, dword ptr [ecx + edi*2], 73795300h
        db  074h, 065h
        insd
        pop    esp
        inc    ebx
        jne    SHORT loc_1_1274
        jb     SHORT loc_1_1269
        outsb
loc_1_1205:
        je     SHORT loc_1_124A
        outsd
        outsb
        je     SHORT loc_1_127D
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        jb     NEAR PTR loc_1_128B
        imul   esp, dword ptr [ebx + 65h], 574e5c73h
loc_1_121C:
        dec    esi
        push   eax
        xor    esi, dword ptr [edx]
        pop    esp
        dec    esi
        db  065h, 074h, 077h
        outsd
        jb     SHORT loc_1_1293
        push   eax
        db  072h, 06Fh
        jbe    SHORT loc_1_1296
        jb     NEAR PTR loc_1_1231
loc_1_1231:
        add    byte ptr [eax], al
loc_1_1233:
        add    byte ptr [ecx + 75h], al
        je     SHORT loc_1_12A0
        outsb
        je     SHORT loc_1_12A5
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [esi + 67h], 6e656741h
        je     SHORT loc_1_1248
loc_1_1248:
        dec    esp
        push   ebx
loc_1_124A:
        push   esp
        cmp    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_124F:
        add    byte ptr [esp + ebx*2], bl
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
loc_1_1259:
        add    byte ptr [eax], al
        add    byte ptr [esi + 44h], al
        push   edi
        sub    al, 44h
        cmp    eax, 64645e77h
loc_1_1269:
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_1274:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_127D:
        inc    dword ptr [edx + edx*2]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1284:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1288:
        push   ebx
        db  062h, 062h
loc_1_128B:
        jo     SHORT loc_1_12B9
        bound  ebp, qword ptr [62215300h]
loc_1_1293:
        add    byte ptr [ebx + 68h], dl
loc_1_1296:
        bound  esi, qword ptr [eax + 2ch]
        bound  ebp, qword ptr [402c5300h]
        pop    edi
loc_1_12A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12A4:
        pop    ecx
loc_1_12A5:
        dec    edi
        push   ebp
        add    byte ptr [ebx + 2ch], dl
        inc    eax
        pop    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12B0:
        push   ebx
        sub    al, 72h
        bound  esp, qword ptr [edx]
        add    byte ptr [eax], al
loc_1_12B8:
        inc    esi
loc_1_12B9:
        ja     SHORT loc_1_12E8
        sub    al, 77h
        ja     SHORT loc_1_12BF
loc_1_12BF:
        add    byte ptr [ebp + 21h], al
        sub    al, 66h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 21h], al
        sub    al, 66h
        add    byte ptr [eax], al
        add    byte ptr [esi + 77h], al
        sub    al, 77h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12D8:
        dec    ebp
        push   ebx
        pop    edi
        pop    eax
        dec    esi
        inc    ebx
        push   eax
        pop    edi
        push   eax
        inc    ecx
        push   edx
        inc    ecx
        dec    ebp
        push   ebx
        add    byte ptr [eax], al
loc_1_12E8:
        dec    ebp
        push   ebx
        pop    edi
        inc    ebp
        pop    eax
        push   esp
        inc    ebp
        dec    esi
        inc    esp
        inc    ebp
        inc    esp
        pop    edi
        dec    esi
        inc    ebx
        push   eax
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12FC:
        push   ebx
        push   edi
        jo     SHORT loc_1_1362
        jo     SHORT loc_1_132E
        jb     SHORT loc_1_1304
loc_1_1304:
        pop    eax
        push   edi
        sub    al, 44h
        inc    esp
        inc    esp
        add    byte ptr [eax], al
loc_1_130C:
        inc    esi
        inc    esp
        push   edi
        sub    al, 44h
        cmp    eax, 64645e77h
        add    byte ptr [eax], al
        add    byte ptr [ebx + 2dh], al
        add    byte ptr [eax], al
loc_1_1320:
        inc    esp
        and    dword ptr [2d214400h], ebp
        add    byte ptr [esi + 45h], cl
        push   esp
        pop    edi
        inc    ecx
        inc    esp
loc_1_132E:
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [ebx + 77h], dl
        jo     SHORT loc_1_139A
        db  070h, 02Ch
        jb     SHORT loc_1_133C
loc_1_133C:
        push   ebx
        bound  ebp, qword ptr [edx + esi*2]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1344:
        push   ebx
        sub    al, 72h
        add    byte ptr [ebx + 77h], dl
        jo     SHORT loc_1_1378
        jb     SHORT loc_1_134E
loc_1_134E:
        add    byte ptr [eax], al
loc_1_1350:
        push   ebx
        jb     SHORT loc_1_13CA
        jo     SHORT loc_1_1355
loc_1_1355:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 70h], dl
        jo     SHORT loc_1_135C
loc_1_135C:
        inc    ebx
        sub    eax, 21440000h
loc_1_1362:
        sub    eax, 62625300h
        bound  ebp, qword ptr gs:[ebx]
        sub    al, 62h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1370:
        push   ebx
        bound  esp, qword ptr [edx + 65h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1378:
        push   ebx
        sub    al, 62h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1380:
        dec    esp
        bound  esp, qword ptr [edx + 77h]
        bound  edi, qword ptr [2c45623dh]
        bound  eax, qword ptr [eax]
loc_1_138C:
        push   ebx
        push   622c7062h
        sub    eax, 2c664600h
        add    byte ptr [eax], al
loc_1_139A:
        add    byte ptr [eax], al
loc_1_139C:
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [esi + 45h], cl
        push   esp
        pop    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [ebx + 77h], dl
        bound  esi, qword ptr [edx + 62h]
        jo     SHORT loc_1_13DF
        jb     SHORT loc_1_13B5
loc_1_13B5:
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
loc_1_13BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 004h, 052h, 000h, 000h
loc_1_13CA:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_13D4:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        nop
        inc    edx
        add    byte ptr [eax], al
        mov    al, 43h
        db  000h
loc_1_13DF:
        db  000h, 000h
        dec    ecx
        add    byte ptr [eax], al
        db  0F0h, 04Ah, 000h, 000h, 090h, 04Ch, 000h, 000h, 0C0h, 035h, 000h, 000h, 010h, 04Eh, 000h, 000h
        db  040h, 051h, 000h, 000h, 060h, 044h, 000h, 000h, 090h, 045h, 000h, 000h, 060h, 06Dh, 000h, 000h
        db  060h, 06Dh, 000h, 000h, 060h, 06Dh, 000h, 000h, 0C0h, 056h, 000h, 000h, 060h, 06Dh, 000h, 000h
loc_1_1414:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        db  070h, 06Dh
        add    byte ptr [eax], al
        db  070h, 06Dh
        add    byte ptr [eax], al
        db  070h, 06Dh
        add    byte ptr [eax], al
        jo     SHORT loc_1_1493
        add    byte ptr [eax], al
        db  070h, 06Dh
        add    byte ptr [eax], al
        db  0C0h, 035h, 000h, 000h, 070h, 06Dh, 000h
        db  000h, 070h, 06Dh
        add    byte ptr [eax], al
        pushad
        inc    esp
        add    byte ptr [eax], al
        nop
        inc    ebp
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        db  070h, 06Dh
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
loc_1_1454:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_145C:
        add    al, 0
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1464:
        push   cs
        add    byte ptr [eax + eax], al
        or     al, byte ptr [eax]
        dec    esi
        add    byte ptr [ebp], al
        push   esp
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1478:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        adc    byte ptr [ecx], al
loc_1_1493:
        db  000h, 060h, 06Dh
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        db  0E0h, 02Fh
        add    byte ptr [eax], al
        xor    byte ptr [eax], dh
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
loc_1_14B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_14BC:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        adc    byte ptr [esi], ah
        db  000h, 0C0h
        add    byte ptr [bx + si], al
        sub    byte ptr [ecx], 0
        shr    byte ptr [ebx], 1
        db  000h, 0C0h
        db  069h, 000h, 000h, 060h, 06Dh, 000h
        db  000h, 060h, 06Dh
        add    byte ptr [eax], al
        jo     SHORT loc_1_154A
        add    byte ptr [eax], al
loc_1_14E0:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        pushad
        insd
        add    byte ptr [eax], al
        db  070h, 05Fh
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
        pushad
        insd
        add    byte ptr [eax], al
loc_1_1504:
        add    dword ptr [edx], eax
        add    eax, dword ptr [ecx]
loc_1_1508:
        adc    byte ptr [eax + ecx], cl
        add    al, 0
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], al
        add    dword ptr [ecx], eax
        or     byte ptr [ecx], al
        or     eax, dword ptr [eax]
        add    byte ptr [esi + 77h], al
        push   2c6562h
        db  000h
loc_1_1520:
        push   ebx
        sub    al, 62h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1528:
        dec    esp
        bound  esp, qword ptr [edx + 77h]
        bound  edi, qword ptr [2c45623dh]
        bound  eax, qword ptr [eax]
loc_1_1534:
        dec    esp
        bound  esp, qword ptr [edx + 77h]
        bound  edi, qword ptr [2c45623dh]
        bound  eax, qword ptr [eax]
loc_1_1540:
        push   ebx
        bound  esp, qword ptr [edx + 65h]
        bound  ebp, qword ptr [ebx]
        sub    al, 62h
        add    byte ptr [eax], al
loc_1_154A:
        add    byte ptr [eax], al
loc_1_154C:
        push   ebx
        bound  esp, qword ptr [edx + 65h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1554:
        push   ebx
        bound  eax, qword ptr [eax]
        add    byte ptr [ebx + 64h], dl
        ja     SHORT loc_1_15C1
        sub    al, 64h
        add    byte ptr [eax], al
loc_1_1560:
        dec    esi
        push   edi
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        inc    ecx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_156C:
        push   ebx
        sub    al, 5fh
        cmp    eax, 4c000072h
        push   eax
        push   esp
        aas
        and    byte ptr [ebx + 61h], al
        je     SHORT loc_1_15E0
        push   574e2820h
        push   edx
        inc    ebp
        inc    esp
        dec    ecx
        push   edx
        sub    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ebx + 64h], dl
        pop    edi
        ja     NEAR PTR loc_1_15F2
        db  077h, 062h
        bound  ebx, qword ptr [edi + 66h]
        sub    al, 5fh
        db  077h, 05Fh
        jb     SHORT loc_1_159E
loc_1_159E:
        add    byte ptr [eax], al
loc_1_15A0:
        dec    esp
        push   eax
        push   esp
        and    byte ptr [ebx + 61h], al
        db  074h, 063h
        push   574e2820h
        push   edx
        inc    ebp
        inc    esp
        dec    ecx
        push   edx
        sub    dword ptr [eax], eax
loc_1_15B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_15B8:
        dec    esp
        push   ebx
        push   esp
        cmp    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_15C1:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 64h], dl
        pop    edi
        db  064h, 064h, 077h, 05Fh
        db  077h, 062h
        bound  ebx, qword ptr [edi + 66h]
        sub    al, 5fh
        ja     SHORT loc_1_1633
        jb     SHORT loc_1_15D6
loc_1_15D6:
        add    byte ptr [eax], al
loc_1_15D8:
        dec    esp
        bound  esp, qword ptr [edx + 57h]
        inc    esp
        push   edi
        push   edi
        push   edi
loc_1_15E0:
        sub    eax, 45455f68h
        sub    al, 72h
        bound  ebp, qword ptr [445f575fh]
        pop    edi
        push   edi
        push   edi
        pop    edi
        push   edi
loc_1_15F2:
        add    byte ptr [eax], al
loc_1_15F4:
        inc    esi
        push   722c6562h
        cmp    eax, 642d625fh
        cmp    eax, 777777h
loc_1_1604:
        inc    esi
        push   2c656262h
        db  072h, 03Dh
        pop    edi
        bound  ebp, qword ptr [77773d64h]
        ja     SHORT loc_1_1615
loc_1_1615:
        add    byte ptr [eax], al
        add    byte ptr [esi + 62h], al
        bound  esp, qword ptr [edx + 65h]
        sub    al, 72h
        cmp    eax, 642d625fh
        cmp    eax, 777777h
        add    byte ptr [eax], al
        add    byte ptr [esi + 68h], al
        bound  esp, qword ptr [ebp + 2ch]
        jb     SHORT loc_1_1670
loc_1_1633:
        pop    edi
        bound  ebp, qword ptr [77773d64h]
        ja     SHORT loc_1_163C
loc_1_163C:
        inc    esi
        sub    eax, 776472h
        add    byte ptr [eax], al
loc_1_1644:
        inc    esi
        sub    eax, 2d460072h
        jb     SHORT loc_1_164C
loc_1_164C:
        push   ebx
        ja     NEAR PTR loc_1_1650
loc_1_1650:
        inc    esi
        sub    eax, 624c0072h
        bound  edx, qword ptr [edi + 2dh]
        push   455fh
        add    byte ptr [eax], al
loc_1_1660:
        inc    esi
        push   6962h
        add    byte ptr [eax], al
loc_1_1668:
        dec    esp
        bound  esp, qword ptr [edx + 57h]
        inc    esp
        bound  ebp, qword ptr [eax + 5fh]
loc_1_1670:
        inc    ebp
        sub    al, 5fh
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1678:
        inc    esi
        db  077h, 068h
        bound  esp, qword ptr [ebp]
        add    byte ptr [eax], al
loc_1_1680:
        push   ebx
        bound  ebp, qword ptr [eax + 65h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1688:
        dec    esp
        bound  esp, qword ptr [edx + 57h]
        inc    esp
        push   edi
        push   edi
        push   edi
        sub    eax, 45455f68h
        add    byte ptr [eax], al
        add    byte ptr [ebx + 68h], dl
        bound  esp, qword ptr [ebp]
        add    byte ptr [eax], al
        add    byte ptr [edx + 62h], cl
        push   edi
        sub    eax, 455f68h
        add    byte ptr [eax], al
        add    byte ptr [ebx + 68h], dl
        sub    eax, 65h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 68h], dl
        bound  esi, qword ptr [eax + 2ch]
        bound  ebp, qword ptr [62624c00h]
        push   edi
        inc    esp
        bound  esp, qword ptr [edx + 5fh]
        inc    ebp
        sub    al, 5fh
        jne    SHORT loc_1_16CD
loc_1_16CD:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 62h], dl
        add    byte ptr [eax], al
loc_1_16D4:
        dec    esp
        bound  esp, qword ptr [edx + 57h]
        inc    esp
        bound  ebp, qword ptr [eax + 5fh]
        inc    ebp
        sub    al, 5fh
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_16E4:
        inc    esi
        ja     SHORT loc_1_174F
        bound  esp, qword ptr [ebp + 2ch]
        cmp    eax, 625f3dh
        add    byte ptr [edx + 62h], cl
        push   edi
        inc    esp
        push   edi
        pop    edi
        bound  ebp, qword ptr [eax + 5fh]
        inc    ebp
        add    byte ptr [esi + 62h], al
        push   6562h
        add    byte ptr [ebx + 68h], dl
        sub    al, 77h
        db  077h, 077h
        add    byte ptr [eax], al
loc_1_170C:
        push   ebx
        bound  ebp, qword ptr [esp + eax*2]
        inc    esp
        inc    esp
        pop    edi
        bound  eax, qword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 44h], dl
        sub    al, 44h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1720:
        push   ebx
        bound  eax, qword ptr [esp + ebp + 44h]
        inc    esp
        add    byte ptr [eax], al
loc_1_1728:
        dec    esp
        bound  esp, qword ptr [edx + 57h]
        bound  ebp, qword ptr [eax + 5fh]
        sub    eax, 2d5f6862h
        inc    ebp
        inc    ebp
        inc    ebp
        add    byte ptr [esi + 68h], al
        bound  ebp, qword ptr [ecx + 68h]
        imul   eax, dword ptr [eax], 69685300h
        imul   eax, dword ptr [eax], 4c000000h
        bound  esp, qword ptr [edx + 57h]
        bound  ebp, qword ptr [eax + 5fh]
loc_1_174F:
        inc    ebp
        sub    al, 62h
        add    byte ptr [eax], al
loc_1_1754:
        inc    esi
        bound  ebp, qword ptr [7777622ch]
        sub    eax, 0
loc_1_1760:
        push   ebx
        bound  eax, qword ptr [eax]
        add    byte ptr [esi + 68h], al
        sub    al, 62h
        ja     SHORT loc_1_17E2
        sub    eax, 0
loc_1_1770:
        push   ebx
        push   3d3d3d2ch
        cmp    eax, 723dh
        add    byte ptr [esi + 62h], al
        ja     SHORT loc_1_17F7
        bound  esi, qword ptr [eax + 2ch]
        db  077h, 03Dh
        jb     SHORT loc_1_17B4
        bound  ebp, qword ptr [46007777h]
        bound  esi, qword ptr [edi + 77h]
        bound  esi, qword ptr [eax + 2ch]
        ja     SHORT loc_1_17D2
        jb     SHORT loc_1_17C4
        bound  ebp, qword ptr [773d3d64h]
        ja     SHORT loc_1_179F
loc_1_179F:
        add    byte ptr [esi + 68h], al
        sub    al, 62h
        db  077h, 077h
        add    byte ptr [edx + 62h], cl
        sub    eax, 2c455f68h
        bound  eax, qword ptr [esp + eax*2]
loc_1_17B4:
        dec    esp
        bound  esp, qword ptr [edx + 57h]
        bound  ebp, qword ptr [eax + 5fh]
        inc    ebp
        sub    al, 62h
        add    byte ptr [eax], al
loc_1_17C0:
        push   ebx
        bound  esp, qword ptr [edx + 2dh]
loc_1_17C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_17C8:
        dec    esp
        bound  esp, qword ptr [edx + 2dh]
        bound  ebx, qword ptr [edi + 2dh]
        sub    al, 62h
        inc    esp
loc_1_17D2:
        inc    esp
        add    byte ptr [ebx + 62h], dl
        add    byte ptr [eax], al
loc_1_17D8:
        sub    al, byte ptr [eax]
        add    byte ptr cs:[edx], ch
        add    byte ptr [eax], al
        add    byte ptr [ebx + 68h], dl
loc_1_17E2:
        sub    al, 3dh
        cmp    eax, 7a3d3d3dh
        add    byte ptr [eax], al
        add    byte ptr [edx + 62h], cl
        push   edi
        inc    esp
        bound  eax, qword ptr [esp + eax*2 + 70h]
        sub    al, 2dh
loc_1_17F7:
        inc    esp
        inc    esp
        sub    eax, 445f575fh
        pop    edi
        inc    esp
        pop    edi
        inc    esp
        pop    edi
        push   edi
        pop    edi
        inc    esp
        pop    edi
        jne    SHORT loc_1_1809
loc_1_1809:
        add    byte ptr [eax], al
        add    byte ptr [esi + 62h], al
        db  077h, 077h
        bound  esi, qword ptr [eax + 2ch]
        db  077h, 03Dh
        db  072h, 02Dh
        bound  ebp, qword ptr [46007777h]
        bound  esi, qword ptr [edi + 77h]
        bound  esi, qword ptr [eax + 2ch]
        ja     SHORT loc_1_1862
        db  072h, 02Dh
        bound  ebp, qword ptr [77777764h]
        ja     SHORT loc_1_182F
loc_1_182F:
        add    byte ptr [ebx + 62h], dl
        inc    esp
        bound  ebp, qword ptr [edi + ebx*2]
        jne    SHORT loc_1_1838
loc_1_1838:
        push   ebx
        bound  eax, qword ptr [edx + 2ch]
        pop    edi
        jne    SHORT loc_1_1840
loc_1_1840:
        inc    esi
        sub    eax, 2c776472h
        ja     SHORT loc_1_18A7
        add    byte ptr [eax], al
        add    byte ptr [edx + 44h], al
        db  072h, 064h
        sub    al, 64h
        pop    edi
        db  077h, 05Fh
        add    byte ptr [eax], al
        add    byte ptr [esi + 2dh], al
        jb     SHORT loc_1_18C4
        ja     SHORT loc_1_18C8
loc_1_1862:
        add    byte ptr [eax], al
loc_1_1864:
        inc    edx
        inc    esp
        jb     SHORT loc_1_18CC
        sub    al, 64h
        add    byte ptr [eax], al
loc_1_1870:
        inc    esi
        sub    eax, 642c72h
        add    byte ptr [eax], al
loc_1_1878:
        push   ebx
        ja     NEAR PTR loc_1_187C
loc_1_187C:
        push   ebx
        ja     NEAR PTR loc_1_1880
loc_1_1880:
        inc    esi
        sub    eax, 2d460072h
        jb     SHORT loc_1_1888
loc_1_1888:
        inc    esi
        sub    eax, 624c0072h
        bound  edx, qword ptr [edi + 44h]
        bound  ebp, qword ptr [eax + 5fh]
        inc    ebp
        sub    al, 5fh
        push   edi
        push   edi
        pop    edi
        push   edi
        add    byte ptr [esi + 77h], al
        push   5f2c6562h
        ja     SHORT loc_1_191C
        add    byte ptr [eax], al
loc_1_18A7:
        add    byte ptr [esi + 62h], al
        jb     SHORT loc_1_1910
        ja     NEAR PTR loc_1_18AF
loc_1_18AF:
        add    byte ptr [eax], dh
        xor    dword ptr [edx], esi
        xor    esi, dword ptr [esi + 39383736h]
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
loc_1_18C0:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_18C4:
        sbb    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_18C8:
        add    al, 0
        add    byte ptr [eax], al
loc_1_18CC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_18D0:
        pop    esp
        pop    esp
        add    byte ptr [eax], al
loc_1_18D4:
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 64h], dl
        ja     SHORT loc_1_18E0
loc_1_18E0:
        push   ebx
        ja     NEAR PTR loc_1_18E4
loc_1_18E4:
        inc    esi
        sub    eax, 72h
        add    byte ptr [eax], al
loc_1_18EC:
        push   ebx
        push   622c7062h
        sub    eax, 625300h
        add    byte ptr [ebx + edx*2 + 54h], cl
        cmp    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 4eh], dl
        dec    ebx
        dec    esi
        dec    edi
        push   edi
        dec    esi
        add    byte ptr [ecx + 48h], al
        cmp    ah, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1910:
        inc    esi
        db  077h, 062h
        bound  esp, qword ptr [ebp + 2bh]
        sub    al, 72h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_191C:
        inc    esi
        db  077h, 062h
        bound  esp, qword ptr [ebp + 2ch]
        jb     SHORT loc_1_1924
loc_1_1924:
        inc    esi
        bound  esp, qword ptr [edx + 62h]
        sub    eax, dword ptr gs:[eax]
        add    byte ptr [esi + 62h], al
        bound  esp, qword ptr [edx + 65h]
        add    byte ptr [eax], al
        add    byte ptr [esi + 62h], al
        db  072h, 077h
        db  077h, 077h
        db  077h, 077h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1940:
        inc    esi
        sub    eax, 77777772h
        ja     SHORT loc_1_1948
loc_1_1948:
        inc    esi
        bound  esi, qword ptr [edi]
loc_1_194C:
        inc    esi
        sub    eax, 62460000h
        bound  esi, qword ptr [edx + 2ch]
        bound  eax, qword ptr fs:[eax]
loc_1_1958:
        inc    esi
        bound  esp, qword ptr [esp + ebp + 62h]
        bound  eax, qword ptr [eax]
        add    byte ptr [esi + 62h], al
        ja     NEAR PTR loc_1_1965
loc_1_1965:
        add    byte ptr [eax], al
        add    byte ptr [esi + 62h], al
        add    byte ptr fs:[esi + 2dh], al
        add    byte ptr [eax], al
loc_1_1970:
        inc    esi
        bound  eax, qword ptr [eax]
        add    byte ptr [esi + 62h], al
        sub    al, 57h
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [esi + 62h], al
        push   edi
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1984:
        inc    esi
        bound  ebp, qword ptr [edi + esi*2]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_198C:
        inc    esi
        bound  esi, qword ptr [edi]
loc_1_1990:
        inc    esi
        ja     SHORT loc_1_1993
loc_1_1993:
        add    byte ptr [esi + 2dh], al
        add    byte ptr [eax], al
loc_1_1998:
        inc    esi
        bound  esi, qword ptr [edi + 72h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19A0:
        inc    esi
        bound  esi, qword ptr [edi]
loc_1_19A4:
        inc    esi
        jb     SHORT loc_1_19A7
loc_1_19A7:
        add    byte ptr [esi + 2dh], al
        add    byte ptr [eax], al
loc_1_19AC:
        inc    esi
        bound  ebp, qword ptr [edx + esi*2]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19B4:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_19B8:
        push   ebx
        bound  ebp, qword ptr [eax + esi*2]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19C0:
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [esi + 62h], al
        bound  esi, qword ptr [edx + 2ch]
        db  072h, 05Fh
        add    byte ptr [ebx + 66h], dl
        sub    al, 66h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19D4:
        inc    esi
        sub    al, 72h
        add    byte ptr [ebx + 2dh], dl
        sub    eax, 622c2b65h
        sub    eax, 0
loc_1_19E4:
        push   ebx
        sub    eax, 2b62652dh
        sub    al, 62h
        sub    eax, 53000000h
        bound  ebp, qword ptr [2d622c65h]
        add    byte ptr [ebx + 62h], dl
        add    byte ptr [eax], al
loc_1_19FC:
        inc    esi
        bound  esp, qword ptr [edx + 77h]
        sub    eax, dword ptr gs:[eax]
        add    byte ptr [esi + 62h], al
        bound  esi, qword ptr [edi + 65h]
        add    byte ptr [eax], al
        add    byte ptr [esi + 62h], al
        sub    eax, dword ptr gs:[eax]
        add    byte ptr [eax], al
        add    byte ptr [esi + 62h], al
        add    byte ptr gs:[esi], al
        add    byte ptr [eax], al
loc_1_1A1C:
        inc    esi
        sub    al, 66h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A24:
        inc    esi
        add    byte ptr [eax], al
loc_1_1A28:
        pop    eax
        sub    al, 66h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A30:
        inc    esi
        sub    eax, 64647272h
        sub    al, 64h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A41:
        add    dword ptr [eax], eax
        add    byte ptr [ecx], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    dword ptr [ecx], eax
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
        add    byte ptr [esi + 2dh], al
        jb     SHORT loc_1_1AE0
        ja     SHORT loc_1_1AAA
        inc    eax
        ja     SHORT loc_1_1AE0
        add    byte ptr [eax], al
loc_1_1A84:
        inc    edx
        inc    esp
        jb     SHORT loc_1_1AEC
        sub    al, 40h
        bound  ebx, qword ptr [edi + 77h]
        pop    edi
        add    byte ptr [bx + si], al
loc_1_1A94:
        inc    esi
        sub    eax, 66776472h
        sub    al, 40h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1AA0:
        inc    edx
        inc    esp
        db  072h, 064h
        sub    al, 40h
loc_1_1AAA:
        bound  ebx, qword ptr [edi]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 68h], dl
        sub    al, 40h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1AB8:
        inc    esi
        sub    eax, 402ch
        add    byte ptr [eax], al
loc_1_1AC0:
        inc    esi
        sub    eax, 3d777772h
        sub    al, 40h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1ACC:
        push   ebx
        push   7777402ch
        ja     SHORT loc_1_1AD4
loc_1_1AD4:
        push   ebx
        bound  ebp, qword ptr [eax + eax*2]
        jo     SHORT loc_1_1ADA
loc_1_1ADA:
        add    byte ptr [eax], al
loc_1_1ADC:
        dec    esp
        bound  esp, qword ptr [edx + 57h]
loc_1_1AE0:
        inc    esp
        bound  ebp, qword ptr [eax + 5fh]
        inc    ebp
        sub    al, 40h
        pop    edi
        push   edi
        pop    edi
        inc    esp
        pop    edi
loc_1_1AEC:
        push   edi
        push   edi
        add    byte ptr [eax], al
loc_1_1AF0:
        inc    esi
        ja     SHORT loc_1_1B5B
        bound  esp, qword ptr [ebp + 2ch]
        inc    eax
        cmp    eax, 2d625f3dh
        pop    edi
        db  077h, 077h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B04:
        inc    esi
        sub    eax, 402c72h
        add    byte ptr [eax], al
loc_1_1B0C:
        or     cl, byte ptr [edx]
        add    byte ptr [eax], al
loc_1_1B10:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_1B14:
        or     cl, byte ptr [edx]
        add    byte ptr [eax], al
loc_1_1B18:
        aas
        aas
        aas
        add    byte ptr [ebx + 65h], dl
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B24:
        push   eax
        popad
        db  072h, 065h
        outsb
        je     SHORT loc_1_1B2B
loc_1_1B2B:
        add    byte ptr [edi + 72h], al
        popad
        outsb
        db  064h, 070h, 061h
        db  072h, 065h
        outsb
        je     SHORT loc_1_1B38
loc_1_1B38:
        dec    esi
        je     NEAR PTR loc_1_1B3C
loc_1_1B3C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B40:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B44:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B48:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B50:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B54:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B58:
        inc    ebx
        dec    edi
        dec    ebp
loc_1_1B5B:
        push   ebx
        push   eax
        inc    ebp
        inc    ebx
        add    byte ptr [eax + 41h], dl
        push   esp
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B68:
        aas
        cmp    al, byte ptr [eax]
        db  000h, 03Dh, 000h, 000h, 000h
sub_1_1B70:
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        lea    eax, [esp + 2ch]
        push   eax
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        mov    ecx, esi
        call   sub_1_1C00
        lea    eax, [esp + 0ch]
        push   eax
        lea    edx, [esp + 30h]
        mov    esi, dword ptr [esp + 54h]
        push   10h
        mov    ecx, esi
        call   sub_1_1C00
        lea    eax, [esp + 1ch]
        push   eax
        lea    edx, [esp + 30h]
        push   10h
        lea    ecx, [esi + 4]
        call   sub_1_1C00
        sub    eax, eax
        mov    edi, edi
loc_1_1BC0:
        lea    ecx, [esp + 2bh]
        sub    ecx, eax
        inc    eax
        mov    cl, byte ptr [ecx]
        xor    byte ptr [esp + eax + 0bh], cl
        cmp    eax, 10h
        jl     SHORT loc_1_1BC0
        sub    ecx, ecx
        mov    eax, dword ptr [esp + 54h]
loc_1_1BD8:
        lea    edx, [esp + 1bh]
        sub    edx, ecx
        mov    bl, byte ptr [esp + ecx + 0ch]
        xor    bl, byte ptr [edx]
        mov    byte ptr [eax + ecx], bl
        inc    ecx
        cmp    ecx, 8
        jl     SHORT loc_1_1BD8
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1C00:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, ecx
        mov    ebp, dword ptr [esp + 34h]
        or     ebp, ebp
        jle    SHORT loc_1_1C1E
        sub    eax, eax
loc_1_1C13:
        cmp    byte ptr [edx + ebp - 1], al
        jne    SHORT loc_1_1C1E
        dec    ebp
        cmp    ebp, eax
        jg     SHORT loc_1_1C13
loc_1_1C1E:
        cmp    ebp, 20h
        lea    edi, [esp + 10h]
        jle    SHORT loc_1_1C70
        mov    esi, edx
        mov    ecx, 8
        rep movsd
        add    edx, 20h
        sub    ebp, 20h
        cmp    ebp, 20h
        jl     SHORT loc_1_1C79
        mov    esi, ebp
        shr    esi, 5
        mov    eax, esi
        shl    eax, 5
        sub    ebp, eax
loc_1_1C47:
        mov    ecx, edx
        lea    eax, [esp + 10h]
loc_1_1C50:
        mov    edi, dword ptr [ecx]
        xor    dword ptr [eax], edi
        add    ecx, 4
        add    eax, 4
        lea    edi, [esp + 2ch]
        cmp    eax, edi
        jbe    SHORT loc_1_1C50
        add    edx, 20h
        dec    esi
        jne    SHORT loc_1_1C47
        jmp    SHORT loc_1_1C79
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1C70:
        sub    eax, eax
        mov    ecx, 8
        rep stosd
loc_1_1C79:
        or     ebp, ebp
        jle    SHORT loc_1_1CA1
        sub    ecx, ecx
        sub    esi, esi
loc_1_1C81:
        cmp    ecx, ebp
        jne    SHORT loc_1_1C93
        sub    ecx, ecx
        db  08Ah, 086h, 050h, 01Eh, 000h, 000h
        xor    byte ptr [esp + esi + 10h], al
        jmp    SHORT loc_1_1C9B
loc_1_1C93:
        mov    al, byte ptr [edx + ecx]
        xor    byte ptr [esp + esi + 10h], al
        inc    ecx
loc_1_1C9B:
        inc    esi
        cmp    esi, 20h
        jl     SHORT loc_1_1C81
loc_1_1CA1:
        sub    eax, eax
loc_1_1CA3:
        mov    ecx, eax
        and    ecx, 3
        mov    cl, byte ptr [ebx + ecx]
        xor    byte ptr [esp + eax + 10h], cl
        inc    eax
        cmp    eax, 20h
        jl     SHORT loc_1_1CA3
        lea    esi, [esp + 10h]
        mov    edx, esi
        sub    ecx, ecx
        call   sub_1_1D10
        mov    edx, esi
        mov    ecx, eax
        call   sub_1_1D10
        sub    edx, edx
        lea    esi, [esp + 11h]
        mov    ecx, dword ptr [esp + 38h]
loc_1_1CD5:
        xor    eax, eax
        mov    al, byte ptr [esi]
        add    esi, 2
        db  08Ah, 098h, 050h, 01Dh, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [esi - 3]
        shl    bl, 4
        db  00Ah, 098h, 050h, 01Dh, 000h, 000h
        mov    byte ptr [ecx + edx], bl
        inc    edx
        lea    eax, [esp + 31h]
        cmp    esi, eax
        jb     SHORT loc_1_1CD5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D10:
        push   ebx
        push   esi
        push   edi
        mov    eax, ecx
        sub    esi, esi
loc_1_1D17:
        lea    edi, [edx + esi]
        lea    ecx, [eax + esi]
        and    ecx, 1fh
        mov    cl, byte ptr [edx + ecx]
        db  02Ah, 08Eh, 050h, 01Eh, 000h, 000h
        mov    bl, byte ptr [edi]
        add    bl, al
        xor    cl, bl
        mov    byte ptr [edi], cl
        and    ecx, 0ffh
        add    eax, ecx
        inc    esi
        cmp    esi, 20h
        jl     SHORT loc_1_1D17
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1D50:
        pop    es
        or     byte ptr [eax], al
        or     byte ptr [esi], al
        add    al, 0eh
        add    al, 5
        or     al, 1
        pop    es
        or     ecx, dword ptr [edi]
        or     cl, byte ptr [eax]
        invd
        or     al, 0ch
        or     dword ptr [ecx + eax], eax
        push   cs
        add    al, 6
        add    al, byte ptr [eax + eax]
        or     cl, byte ptr [ebx]
        or     dword ptr [edx], eax
        ud2
        add    dword ptr [5090102h], ecx
        push   cs
        pop    es
        add    byte ptr [eax], al
        add    al, byte ptr [esi]
        push   es
        add    byte ptr [edi], al
        add    ecx, dword ptr [eax]
        add    cl, byte ptr [ecx]
        add    ecx, dword ptr [edi]
        pop    es
        db  00Fh, 00Ch, 00Fh, 006h, 004h, 00Ah, 000h, 002h, 003h, 00Ah, 00Bh, 00Dh, 008h, 003h, 00Ah, 001h
        db  007h, 00Ch, 00Fh, 001h, 008h, 009h, 00Dh, 009h, 001h, 009h, 004h, 00Eh, 004h, 00Ch, 005h, 005h
        db  00Ch, 008h, 00Bh, 002h, 003h, 009h, 00Eh, 007h, 007h, 006h, 009h, 00Eh, 00Fh, 00Ch, 008h, 00Dh
        db  001h, 00Ah, 006h, 00Eh, 00Dh, 000h, 007h, 007h, 00Ah, 000h, 001h, 00Fh, 005h, 004h, 00Bh, 007h
        db  00Bh, 00Eh, 00Ch, 009h, 005h, 00Dh, 001h, 00Bh, 00Dh, 001h, 003h, 005h, 00Dh, 00Eh, 006h, 003h
        db  000h, 00Bh, 00Bh, 00Fh, 003h, 006h, 004h, 009h, 00Dh, 00Ah, 003h, 001h, 004h, 009h, 004h, 008h
        db  003h, 00Bh, 00Eh, 005h, 000h, 005h, 002h, 00Ch, 00Bh, 00Dh, 005h, 00Dh, 005h, 00Dh, 002h, 00Dh
        db  009h, 00Ah, 00Ch, 00Ah, 000h, 00Bh, 003h, 005h, 003h, 006h, 009h, 005h, 001h, 00Eh, 00Eh, 000h
        db  00Eh, 008h, 002h, 00Dh, 002h, 002h, 000h, 004h, 00Fh, 008h, 005h, 009h, 006h, 008h, 006h, 00Bh
        db  00Ah, 00Bh, 00Fh, 000h, 007h, 002h, 008h, 00Ch, 007h, 003h, 00Ah, 001h, 004h, 002h, 005h, 00Fh
        db  007h, 00Ah, 00Ch, 00Eh, 005h, 009h, 003h, 00Eh, 007h, 001h, 002h, 00Eh, 001h, 00Fh, 004h, 00Ah
        db  006h, 00Ch, 006h, 00Fh, 004h, 003h, 000h, 00Ch, 000h, 003h, 006h, 00Fh, 008h, 007h, 00Bh, 002h
        db  00Dh, 00Ch, 006h, 00Ah, 00Ah, 008h, 00Dh
loc_1_1E50:
        dec    eax
        xchg   ebx, eax
        inc    esi
        cwde
        cmp    eax, 10b78de6h
        jp     SHORT loc_1_1E82
        pop    edx
        mov    ecx, 0f6b35b1h
        aad    70h
        scasb
        sti
        lodsd
        adc    esp, esi
        inc    edi
        fsub   qword ptr [edi - 3faf3014h]
loc_1_1E70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E82:
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
loc_1_1F08:
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
loc_1_1F3A:
        add    byte ptr [eax], al
loc_1_1F3C:
        add    byte ptr [eax], al
loc_1_1F3E:
        add    byte ptr [eax], al
loc_1_1F40:
        add    byte ptr [eax], al
loc_1_1F42:
        add    byte ptr [eax], al
loc_1_1F44:
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
loc_1_1F5A:
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
loc_1_1FD4:
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
        adc    al, 0
        add    byte ptr [eax], al
        cmp    byte ptr [eax], al
loc_1_2006:
        add    byte ptr [eax], al
loc_1_2008:
        add    byte ptr [ecx], al
loc_1_200A:
        add    byte ptr [eax], al
loc_1_200C:
        add    byte ptr [eax], al
loc_1_200E:
        sbb    eax, dword ptr [eax]
loc_1_2010:
        fadd   dword ptr [eax]
        out    0, al
        stc
        add    byte ptr [edi], dh
        add    dword ptr [ebx + 1], eax
        push   edx
        add    dword ptr [eax + 1], esp
        push   1
        db  075h, 001h
        add    byte ptr [ecx], 8dh
        db  001h
loc_1_2026:
        mov    eax, dword ptr [0c101b501h]
        add    edi, ecx
        add    edx, esp
        add    esi, esi
        add    dword ptr [edi], eax
loc_1_2033:
        add    dl, byte ptr [edx + eax]
        add    byte ptr [eax], al
        inc    ebx
        insb
        imul   esp, dword ptr [ebp + 6eh], 65532074h
loc_1_2041:
        jb     SHORT loc_1_20B9
        imul   esp, dword ptr [ebx + 65h], 726f6620h
        and    byte ptr [esi + 65h], cl
        db  074h, 077h
loc_1_204F:
        popad
        db  072h, 065h
        add    byte ptr [ebp + 6eh], dl
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 06Ch
        db  065h, 074h, 065h
        and    byte ptr [ecx + 20h], ah
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [edi + 72h], 6fh
        db  075h, 06Eh
        and    byte ptr fs:[edi + 70h], ch
        db  065h, 072h, 061h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [edi + 6eh], ch
        and    byte ptr [ebx + 6fh], ah
        outsb
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [edi + ebp*2 + 20h], dh
        and    eax, 20202e73h
        push   ebx
        insb
        arpl   word ptr gs:[eax + 52h], si
        db  065h, 074h, 072h
        db  079h, 020h
loc_1_20A0:
        db  074h, 06Fh
        and    byte ptr [edx + esi*2 + 79h], dh
        and    byte ptr [eax + ebp*2 + 65h], dh
        db  020h, 06Fh
loc_1_20AC:
        db  070h, 065h
loc_1_20AE:
        db  072h, 061h
loc_1_20B0:
        db  074h, 069h
        outsd
        outsb
        db  020h, 061h
loc_1_20B6:
        popad
        db  069h
loc_1_20B9:
        outsb
loc_1_20BA:
        sub    al, 20h
        outsd
        db  072h, 020h
        db  070h, 072h
        db  065h, 073h, 073h
        db  020h, 045h
_LTEXT ends
        end
