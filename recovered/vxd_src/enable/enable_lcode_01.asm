; enable_lcode_01.asm -- ENABLE object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        xor    eax, eax
        xor    ebx, ebx
        lea    ecx, [ebx + 40h]
        db  0BEh, 0B2h, 005h, 000h, 000h
        xor    edi, edi
        db  0CDh, 020h, 05Ah, 081h, 001h, 000h
VMINI_ShutDownItem:
        db  0BEh, 03Ch, 002h, 000h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
loc_1_0021:
        ret
VMINI_Status:
        db  0A1h, 0CCh, 003h, 000h, 000h
        ret
VMINI_Siren:
        pushad
        db  0BEh, 027h, 002h, 000h, 000h
        mov    dh, al
        mov    dl, cl
        shl    edx, 10h
        mov    dx, bx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 2000h
        mov    ecx, 8
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        popad
        ret
VMINI_Update:
        db  0BEh, 0E1h, 003h, 000h, 000h
        mov    edx, eax
        db  0CDh, 020h, 00Eh, 080h, 001h, 000h
VMINI_SetTimeStamp:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  0A3h, 0C8h, 003h, 000h, 000h
        ret
VMINI_DisplayError:
        db  0BEh, 050h, 002h, 000h, 000h
        mov    edx, eax
        db  0CDh, 020h, 00Eh, 080h, 001h, 000h
ENABLE_Control:
        cmp    eax, 2
        db  00Fh, 084h, 081h, 0FFh, 0FEh, 0FFh
        cmp    eax, 23h
        db  00Fh, 084h, 063h, 012h, 0FFh, 0FFh
        cmp    eax, 22h
        jne    SHORT loc_1_0096
        push   edx
        push   ebx
        db  0E8h, 068h, 017h, 0FFh, 0FFh
        stc
        ret
loc_1_0096:
        clc
        ret
loc_1_0098:
        db  080h, 03Dh, 0C6h, 003h, 000h, 000h, 000h
        jne    SHORT loc_1_00BF
        pushad
        or     al, al
        jne    SHORT loc_1_00AA
        or     esi, edi
        je     SHORT loc_1_00BE
loc_1_00AA:
        db  0BEh, 0BEh, 001h, 000h, 000h
        mov    edx, eax
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        db  0C6h, 005h, 0C6h, 003h, 000h, 000h, 0FFh
loc_1_00BE:
        popad
loc_1_00BF:
        db  0FFh, 025h, 0D4h, 003h, 000h, 000h
loc_1_00C5:
        db  080h, 03Dh, 0D8h, 003h, 000h, 000h, 001h
        je     SHORT loc_1_00DD
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        call   sub_1_013E
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
loc_1_00DD:
        jb     SHORT loc_1_00E5
        db  0FFh, 025h, 0D0h, 003h, 000h, 000h
loc_1_00E5:
        ret
sub_1_00E6:
        pushad
        db  0F6h, 005h, 0CDh, 003h, 000h, 000h, 040h
        je     SHORT loc_1_0113
        db  066h, 0F7h, 005h, 0CCh, 003h, 000h, 000h, 07Bh, 007h
        je     SHORT loc_1_0113
        mov    cx, word ptr [ebp + 10h]
        mov    ah, cl
        mov    al, 0e0h
        db  0FFh, 015h, 0B4h, 003h, 000h, 000h
        jb     SHORT loc_1_0113
        mov    al, 0e0h
        db  0CDh, 020h, 013h, 000h, 00Dh, 000h
loc_1_0113:
        popad
        ret
sub_1_0115:
        cmp    al, 45h
        je     SHORT loc_1_013A
        cmp    al, 0c5h
        jne    SHORT loc_1_013C
        db  0C6h, 005h, 0C2h, 003h, 000h, 000h, 000h
        db  0BEh, 075h, 004h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        test   dh, 40h
        je     SHORT loc_1_013A
        db  0FFh, 015h, 0B8h, 003h, 000h, 000h
loc_1_013A:
        clc
        ret
loc_1_013C:
        stc
        ret
sub_1_013E:
        push   ebp
        db  0BDh, 0B4h, 003h, 000h, 000h
        push   eax
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [ebp + 14h], eax
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0CDh, 020h, 011h, 000h, 00Dh, 000h
        and    al, 0afh
        mov    word ptr [ebp + 10h], ax
        pop    eax
        push   eax
        cmp    byte ptr [ebp + 0eh], 1
        jne    SHORT loc_1_016F
        call   sub_1_0115
        jae    SHORT loc_1_01B6
loc_1_016F:
        cmp    byte ptr [ebp + 29h], 1
        jne    SHORT loc_1_017B
        pop    eax
        jmp    NEAR PTR loc_1_031D
loc_1_017B:
        test   al, 80h
        je     SHORT loc_1_0186
        mov    dword ptr [ebp + 8], 0
loc_1_0186:
        cmp    word ptr [ebp + 10h], 10bh
        je     SHORT loc_1_0196
        cmp    word ptr [ebp + 10h], 12bh
        jne    SHORT loc_1_01E1
loc_1_0196:
        cmp    byte ptr [ebp + 0dh], 1dh
        je     SHORT loc_1_01BC
        cmp    byte ptr [ebp + 0dh], 9dh
        je     SHORT loc_1_01BC
        cmp    al, 45h
        jne    SHORT loc_1_01BC
        mov    edx, 40h
        call   0
        mov    byte ptr [ebp + 0eh], 1
        jmp    SHORT loc_1_01D2
loc_1_01B6:
        pop    eax
        jmp    NEAR PTR loc_1_0330
loc_1_01BC:
        cmp    al, 0e0h
        je     SHORT loc_1_01DB
        cmp    al, 54h
        je     SHORT loc_1_01C8
        cmp    al, 37h
        jne    SHORT loc_1_01E1
loc_1_01C8:
        mov    edx, 800h
        call   0
loc_1_01D2:
        pop    eax
        mov    al, 0ffh
        push   eax
        jmp    NEAR PTR loc_1_0305
loc_1_01DB:
        mov    byte ptr [ebp + 2ah], 1
        jmp    SHORT loc_1_01B6
loc_1_01E1:
        cmp    byte ptr [ebp + 2ah], 0
        je     SHORT loc_1_01F0
        call   sub_1_00E6
        mov    byte ptr [ebp + 2ah], 0
loc_1_01F0:
        test   byte ptr [ebp + 13h], 1
        jne    NEAR PTR loc_1_02AF
        db  0F6h, 005h, 0CCh, 003h, 000h, 000h, 001h
        jne    SHORT loc_1_020F
        test   word ptr [ebp + 10h], 0d8ch
        jne    NEAR PTR loc_1_02AF
loc_1_020F:
        mov    bl, al
        and    al, 7fh
        cmp    al, 2ah
        je     SHORT loc_1_021B
        cmp    al, 36h
        jne    SHORT loc_1_026D
loc_1_021B:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_0234
        mov    ah, byte ptr [ebp + 0dh]
        and    ah, 7fh
        cmp    ah, al
        je     SHORT loc_1_0234
        mov    byte ptr [ebp + 0ch], 0
        jmp    NEAR PTR loc_1_0305
loc_1_0234:
        test   bl, 80h
        je     SHORT loc_1_023C
        inc    byte ptr [ebp + 0ch]
loc_1_023C:
        mov    cl, byte ptr [ebp + 0dh]
        and    cl, 7fh
        cmp    cl, 2ah
        je     SHORT loc_1_024C
        cmp    cl, 36h
        jne    SHORT loc_1_0271
loc_1_024C:
        pop    eax
        mov    al, bl
        push   eax
        cmp    byte ptr [ebp + 0ch], 5
        jl     SHORT loc_1_0271
        mov    byte ptr [ebp + 0ch], 0
        mov    byte ptr [ebp + 0dh], 0
        mov    edx, 1
        call   0
        jmp    NEAR PTR loc_1_0305
loc_1_026D:
        mov    byte ptr [ebp + 0ch], 0
loc_1_0271:
        cmp    byte ptr [ebp + 0dh], al
        jne    NEAR PTR loc_1_0305
        cmp    al, 45h
        jne    SHORT loc_1_02AF
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_1_028A
        mov    dword ptr [ebp + 8], eax
loc_1_028A:
        sub    eax, dword ptr [ebp + 8]
        cmp    eax, 1388h
        jb     SHORT loc_1_0305
        xor    eax, eax
        mov    dword ptr [ebp + 8], eax
        mov    byte ptr [ebp + 0dh], al
        mov    edx, 200h
        call   0
        mov    byte ptr [ebp + 0eh], 1
        jmp    NEAR PTR loc_1_01D2
loc_1_02AF:
        cmp    byte ptr [ebp + 13h], 0ffh
        je     SHORT loc_1_02D5
        cmp    al, 36h
        jne    SHORT loc_1_02D5
        cmp    al, byte ptr [ebp + 0dh]
        jne    SHORT loc_1_0305
        cmp    byte ptr [ebp + 13h], 1
        je     SHORT loc_1_0305
        mov    byte ptr [ebp + 13h], 1
        db  0BEh, 024h, 004h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    SHORT loc_1_0305
loc_1_02D5:
        cmp    al, 0b6h
        je     SHORT loc_1_02DB
        jmp    SHORT loc_1_02ED
loc_1_02DB:
        db  0BEh, 08Bh, 000h, 000h, 000h
        mov    dl, byte ptr [ebp + 0fh]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    byte ptr [ebp + 13h], 0
loc_1_02ED:
        cmp    byte ptr [ebp + 13h], 0ffh
        je     SHORT loc_1_0305
        btr    dword ptr [ebp + 13h], 0
        jae    SHORT loc_1_0305
        db  0BEh, 04Bh, 004h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
loc_1_0305:
        pop    eax
        mov    byte ptr [ebp + 0dh], al
        db  0F6h, 005h, 0CDh, 003h, 000h, 000h, 040h
        je     SHORT loc_1_0330
        db  066h, 0F7h, 005h, 0CCh, 003h, 000h, 000h, 07Bh, 007h
        je     SHORT loc_1_0330
loc_1_031D:
        cmp    dword ptr [ebp], 0
        je     SHORT loc_1_0330
        mov    cx, word ptr [ebp + 10h]
        mov    ah, cl
        pop    ebp
        db  0FFh, 025h, 0B4h, 003h, 000h, 000h
loc_1_0330:
        mov    byte ptr [ebp + 0dh], al
        pop    ebp
        db  0FFh, 025h, 0D0h, 003h, 000h, 000h
        db  000h, 000h, 068h, 001h, 000h, 000h, 050h, 000h, 000h, 000h, 022h, 000h, 000h, 000h, 069h, 000h
        db  000h, 000h, 05Dh, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 06Fh, 001h, 000h, 000h, 09Bh, 003h
        db  000h, 000h, 014h, 000h, 000h, 000h, 0D1h, 001h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 037h, 000h, 004h, 000h, 000h, 000h, 045h, 04Eh, 041h, 042h
        db  04Ch, 045h, 020h, 020h, 000h, 000h, 000h, 038h, 076h, 000h, 000h, 000h, 01Bh, 003h, 000h, 000h
        db  08Dh, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  03Ch, 003h, 000h, 000h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
sub_1_03B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03C0:
        add    byte ptr [eax], al
loc_1_03C2:
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
loc_1_03DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
