; apix_lcode_01.asm -- APIX object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0014:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_001C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
APIX_Control:
        sub    eax, 6
        cmp    eax, 1eh
        jae    SHORT loc_1_0043
        db  0FFh, 024h, 085h, 045h, 000h, 000h, 000h
loc_1_0043:
        clc
        ret
        db  05Ah, 001h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h
        db  043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h
        db  043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h
        db  043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h
        db  043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h
        db  043h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h
        db  043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h, 043h, 000h, 000h, 000h
        db  043h, 000h, 000h, 000h, 0BDh, 000h, 000h, 000h
APIX_W32_DEVICEIOCONTROL:
        cmp    ecx, 3
        jne    SHORT loc_1_010F
        mov    ecx, dword ptr [esi + 10h]
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ecx + 20h], eax
        push   esi
        push   8
        push   ecx
        db  0E8h, 06Ch, 00Fh, 0FFh, 0FFh
        add    esp, 8
        pop    esi
        or     dword ptr [esi + 24h], 0
        je     SHORT loc_1_0152
        cmp    eax, -3
        je     SHORT loc_1_00E8
        cmp    eax, -1
        jne    SHORT loc_1_014C
loc_1_00E8:
        mov    ebx, dword ptr [esi + 24h]
        mov    ebx, dword ptr [ebx]
        push   ebx
        mov    ebx, dword ptr [esi + 10h]
        mov    dword ptr [ebx + 1ch], 0
        mov    dword ptr [ebx + 20h], 0
        mov    dword ptr [ebx + 2ch], 0
        pop    ebx
        db  0CDh, 020h, 001h, 000h, 02Ah, 000h
        jmp    SHORT loc_1_014C
loc_1_010F:
        cmp    ecx, 1
        jne    SHORT loc_1_0140
        mov    ebx, dword ptr [esi + 18h]
        mov    dword ptr [ebx], 0
        mov    ecx, 8
        db  0A0h, 0BFh, 008h, 000h, 000h
loc_1_0127:
        shr    al, 1
        adc    dword ptr [ebx], 0
        loop   loc_1_0127
        mov    dword ptr [esi + 1ch], 4
        mov    ebx, dword ptr [esi + 20h]
        mov    dword ptr [ebx], 4
        jmp    SHORT loc_1_0152
loc_1_0140:
        cmp    ecx, 0
        je     SHORT loc_1_0152
        cmp    ecx, 0
        je     SHORT loc_1_0152
        jmp    SHORT loc_1_0155
loc_1_014C:
        mov    eax, 0ffffffffh
        ret
loc_1_0152:
        xor    eax, eax
        ret
loc_1_0155:
        mov    ax, 1
        ret
APIX_SYS_CRITICAL_EXIT:
        db  0E8h, 021h, 01Dh, 0FFh, 0FFh
        jmp    SHORT loc_1_0163
        db  0F9h, 0C3h
loc_1_0163:
        clc
        ret
sub_1_0165:
        push   edi
        push   esi
        push   ebx
        push   eax
        push   ecx
        mov    edx, dword ptr [esp + 18h]
        mov    edi, dword ptr [edx + 1ch]
        mov    ebx, dword ptr [edi + 14h]
        xor    eax, eax
        mov    ecx, 21h
        db  0BEh, 0A9h, 004h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    eax
        pop    ebx
        pop    esi
        pop    edi
        ret
sub_1_018C:
        push   edi
        push   esi
        push   ebx
        push   eax
        push   ecx
        mov    edx, dword ptr [esp + 18h]
        mov    ebx, dword ptr [edx + 1ch]
        xor    eax, eax
        mov    ecx, 121h
        db  0BEh, 0A9h, 004h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    eax
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_1_01B0:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        movzx  eax, word ptr [ebx]
        cmp    ax, 3
        je     SHORT loc_1_022D
        cmp    ax, 4
        je     NEAR PTR loc_1_02AD
        cmp    ax, 2
        je     SHORT loc_1_01DA
        cmp    ax, 1
        je     SHORT APIX_SYS_CRITICAL_EXIT
        ret
loc_1_01DA:
        db  066h, 00Fh, 0BAh, 02Dh, 08Dh, 000h, 000h, 000h, 000h
        jb     SHORT loc_1_0225
        db  08Bh, 00Dh, 089h, 000h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_0226
        xor    eax, eax
        db  00Bh, 005h, 0C0h, 008h, 000h, 000h
        jne    SHORT loc_1_0225
        sub    edx, edx
        db  0BEh, 0B4h, 000h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        jb     SHORT loc_1_0225
        db  0A3h, 000h, 009h, 000h, 000h
        db  068h, 0A3h, 000h, 000h, 000h
        push   22h
        db  068h, 050h, 021h, 000h, 000h
        db  0CDh, 020h, 062h, 000h, 040h, 000h
        add    esp, 0ch
        inc    eax
        jne    SHORT loc_1_0225
loc_1_0225:
        ret
loc_1_0226:
        mov    word ptr [ebx + 2], 0ffffh
        ret
loc_1_022D:
        mov    ebx, dword ptr [ebx + 0ch]
        test   byte ptr [ebx + 21h], 80h
        je     SHORT loc_1_02AC
        cmp    byte ptr [ebx + 74h], 1
        jne    SHORT loc_1_02AC
        db  0A1h, 01Ch, 000h, 000h, 000h
        push   dword ptr [eax + 50h]
        push   ebx
        db  0E8h, 0C6h, 01Ah, 0FFh, 0FFh
        add    esp, 8
        db  0A1h, 01Ch, 000h, 000h, 000h
        test   byte ptr [eax + 28h], 1
        jne    SHORT loc_1_02AC
        db  083h, 03Dh, 0B4h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0277
        db  0A1h, 0B4h, 000h, 000h, 000h
        cmp    dword ptr [eax], 49534353h
        jne    SHORT loc_1_0277
        cmp    dword ptr [eax + 4], 4d41435fh
        je     SHORT loc_1_02AC
loc_1_0277:
        push   0
        push   8
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_02AC
        db  0A3h, 0B4h, 000h, 000h, 000h
        mov    dword ptr [eax], 49534353h
        mov    dword ptr [eax + 4], 4d41435fh
        mov    eax, 4fh
        push   esi
        db  0BEh, 02Ch, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        pop    esi
loc_1_02AC:
        ret
loc_1_02AD:
        mov    ebx, dword ptr [ebx + 0ch]
        test   byte ptr [ebx + 21h], 80h
        je     SHORT loc_1_02CD
        cmp    byte ptr [ebx + 74h], 1
        jne    SHORT loc_1_02CD
        db  0A1h, 01Ch, 000h, 000h, 000h
        push   dword ptr [eax + 50h]
        push   ebx
        db  0E8h, 006h, 01Bh, 0FFh, 0FFh
        add    esp, 8
loc_1_02CD:
        ret
sub_1_02CE:
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 20h], 80000h
        test   dword ptr [eax + 20h], 4000h
        jne    SHORT loc_1_02E5
        mov    eax, dword ptr [eax + 10h]
loc_1_02E5:
        or     eax, eax
        je     SHORT loc_1_0316
        or     dword ptr [eax + 20h], 100000h
        db  08Bh, 015h, 01Ch, 000h, 000h, 000h
        mov    edx, dword ptr [edx + 40h]
        cli
loc_1_02FA:
        or     edx, edx
        je     SHORT loc_1_0310
        cmp    dword ptr [edx + 28h], eax
        jne    SHORT loc_1_030B
        or     dword ptr [edx + 4], 2
        or     dword ptr [edx + 4], 1
loc_1_030B:
        mov    edx, dword ptr [edx + 24h]
        jmp    SHORT loc_1_02FA
loc_1_0310:
        sti
        mov    eax, dword ptr [eax + 10h]
        jmp    SHORT loc_1_02E5
loc_1_0316:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0320:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        db  0A1h, 01Ch, 000h, 000h, 000h
        mov    edi, dword ptr [esi + 20h]
        cmp    dword ptr [eax + 54h], 0
        je     SHORT loc_1_0374
        mov    eax, dword ptr [esi + 0a0h]
        or     eax, eax
        je     SHORT loc_1_0374
        mov    cl, byte ptr [edi + 3]
        test   cl, 80h
        jne    SHORT loc_1_034B
        cmp    cl, 10h
        jne    SHORT loc_1_035E
loc_1_034B:
        xor    ecx, ecx
        mov    cl, byte ptr [edi + 0bh]
        push   ecx
        push   eax
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        db  0E8h, 042h, 0FFh, 0FEh, 0FFh
        add    esp, 0ch
loc_1_035E:
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        db  0E8h, 0EAh, 0FEh, 0FEh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 0a0h], 0
loc_1_0374:
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_1_039E
        mov    ecx, dword ptr [esi + 8]
        cmp    byte ptr [ecx + 40h], 0
        jne    SHORT loc_1_039E
        test   byte ptr [ecx + 20h], 4
        je     SHORT loc_1_039E
        mov    eax, dword ptr [edi + 1ch]
        mov    al, byte ptr [eax + 2]
        and    al, 0fh
        cmp    al, 6
        jne    SHORT loc_1_039E
        push   ecx
        call   sub_1_02CE
        add    esp, 4
loc_1_039E:
        db  0C7h, 046h, 070h, 060h, 007h, 000h, 000h
        test   byte ptr [esi + 94h], 8
        push   esi
        je     SHORT loc_1_03C0
        call   sub_1_018C
        add    esp, 4
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_03C0:
        call   sub_1_0165
        add    esp, 4
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_03D0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        db  0C7h, 040h, 070h, 0D0h, 011h, 000h, 000h
        call   sub_1_0165
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 022h, 000h, 003h, 00Ah, 000h, 000h, 041h, 050h, 049h, 058h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 034h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  093h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ch, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  00Ah, 003h, 010h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
