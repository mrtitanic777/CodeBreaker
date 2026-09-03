; vfbackup_lcode_01.asm -- VFBACKUP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h
VFBACKUP_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0F3h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 0F3h, 000h, 0FFh, 0FFh
        cmp    eax, 0ch
        je     SHORT loc_1_0022
        cmp    eax, 21h
        je     SHORT loc_1_004E
        clc
        ret
loc_1_0022:
        db  08Bh, 00Dh, 0F4h, 003h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        cmp    ebx, ecx
        je     SHORT loc_1_0030
        clc
        ret
loc_1_0030:
        db  0E8h, 0F0h, 001h, 0FFh, 0FFh
        pushfd
        db  00Fh, 0B7h, 015h, 0E0h, 003h, 000h, 000h
        mov    al, 0ch
        out    dx, al
        db  08Bh, 015h, 0F8h, 003h, 000h, 000h
        mov    dword ptr [edx], 0
        popfd
        ret
loc_1_004E:
        mov    ebx, edi
        jmp    SHORT loc_1_0022
VFBACKUP_V86_API:
        ret
VFBACKUP_PM_API:
        ret
VFBACKUP_Get_Version:
        mov    eax, 400h
        clc
        ret
loc_1_005B:
        cmp    eax, 1
        db  00Fh, 084h, 00Bh, 003h, 0FFh, 0FFh
        clc
        ret
        db  000h, 000h
sub_1_0068:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_009C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        cmp    word ptr [ebx], 8
        je     SHORT loc_1_00B9
        cmp    word ptr [ebx], 2
        je     SHORT loc_1_00B8
        mov    word ptr [ebx + 2], 0ffffh
loc_1_00B8:
        ret
loc_1_00B9:
        db  08Bh, 01Dh, 0F4h, 003h, 000h, 000h
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_00C5
loc_1_00C4:
        ret
loc_1_00C5:
        db  08Bh, 00Dh, 0FCh, 003h, 000h, 000h
        test   byte ptr [ecx], 1
        je     SHORT loc_1_00C4
        db  0FFh, 00Dh, 09Ch, 004h, 000h, 000h
        jne    SHORT loc_1_00C4
        db  0BEh, 0E6h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        ret
loc_1_00E6:
        mov    ebx, edx
        db  0E8h, 038h, 001h, 0FFh, 0FFh
        ret
        db  000h, 000h
loc_1_00F0:
        test   byte ptr [ebp + 14h], 80h
        jne    SHORT loc_1_010F
        call   sub_1_019F
        db  080h, 00Dh, 0F0h, 003h, 000h, 000h, 040h
        xor    eax, eax
        db  0BEh, 011h, 001h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
loc_1_010F:
        stc
        ret
loc_1_0111:
        call   sub_1_0185
        db  080h, 025h, 0F0h, 003h, 000h, 000h, 0BFh
        db  0A1h, 0F8h, 003h, 000h, 000h
        mov    dword ptr [eax], 0
        ret
loc_1_0129:
        push   ebx
        db  0CDh, 020h, 0F1h, 000h, 001h, 000h
        db  0F6h, 005h, 0F0h, 003h, 000h, 000h, 040h
        jne    SHORT loc_1_016E
        db  08Bh, 00Dh, 0F8h, 003h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_015C
        mov    ebx, ecx
        db  0C7h, 005h, 09Ch, 004h, 000h, 000h, 01Eh, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 004h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_015C
        pop    ebx
        jmp    ecx
loc_1_015C:
        push   eax
        db  0A1h, 0FCh, 003h, 000h, 000h
        test   dword ptr [eax], 100h
        pop    eax
        je     SHORT loc_1_016E
        mov    ebx, dword ptr [ebx + 14h]
loc_1_016E:
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        pop    ebx
        clc
        ret
loc_1_0177:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
sub_1_0185:
        mov    ecx, 6
        db  0BFh, 0DCh, 003h, 000h, 000h
loc_1_018F:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        add    edi, 2
        loop   loc_1_018F
        clc
        ret
sub_1_019F:
        mov    ecx, 6
        db  0BFh, 0DCh, 003h, 000h, 000h
loc_1_01A9:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        add    edi, 2
        loop   loc_1_01A9
        clc
        ret
loc_1_01B9:
        db  08Bh, 03Dh, 0FCh, 003h, 000h, 000h
        test   byte ptr [edi], 8
        jne    NEAR PTR loc_1_02A1
        cmp    word ptr [ebp + 28h], 0f000h
        je     NEAR PTR loc_1_0275
        cmp    edx, 3f2h
        je     NEAR PTR loc_1_0263
loc_1_01E0:
        db  08Bh, 03Dh, 0F4h, 003h, 000h, 000h
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     SHORT loc_1_01FE
        push   eax
        db  0A1h, 0FCh, 003h, 000h, 000h
        test   dword ptr [eax], 100h
        pop    eax
        je     SHORT loc_1_01FE
        mov    edi, dword ptr [edi + 14h]
loc_1_01FE:
        cmp    edi, ebx
        mov    edi, 1
        jne    SHORT loc_1_0217
        db  08Bh, 03Dh, 0FCh, 003h, 000h, 000h
        mov    edi, dword ptr [edi]
        db  08Bh, 01Dh, 0F4h, 003h, 000h, 000h
        mov    ebx, dword ptr [ebx]
loc_1_0217:
        push   eax
        xor    eax, eax
        test   edi, 1
        je     SHORT loc_1_0223
        inc    eax
loc_1_0223:
        push   ecx
        xor    ecx, ecx
        db  0E8h, 0D5h, 0FDh, 0FEh, 0FFh
        pop    ecx
        pop    eax
        pushfd
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        popfd
        jb     SHORT loc_1_025D
        db  08Bh, 035h, 0FCh, 003h, 000h, 000h
        or     dword ptr [esi], edi
        db  0C7h, 005h, 09Ch, 004h, 000h, 000h, 01Eh, 000h, 000h, 000h
loc_1_0249:
        test   ecx, 58h
        jne    SHORT loc_1_025A
        test   ecx, 4
        jne    SHORT loc_1_025B
        in     al, dx
loc_1_025A:
        ret
loc_1_025B:
        out    dx, al
        ret
loc_1_025D:
        db  0CDh, 020h, 02Fh, 000h, 001h, 000h
loc_1_0263:
        movzx  edi, al
        and    di, 0f8h
        cmp    di, 8
        jne    NEAR PTR loc_1_01E0
loc_1_0275:
        db  08Bh, 03Dh, 0F8h, 003h, 000h, 000h
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_029F
        db  08Bh, 03Dh, 0FCh, 003h, 000h, 000h
        test   byte ptr [edi], 4
        jne    SHORT loc_1_029F
        db  08Bh, 03Dh, 0F4h, 003h, 000h, 000h
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_029F
        db  0F6h, 005h, 0F0h, 003h, 000h, 000h, 010h
        jne    SHORT loc_1_025A
loc_1_029F:
        jmp    SHORT loc_1_0249
loc_1_02A1:
        test   ecx, 58h
        jne    SHORT loc_1_025A
        test   ecx, 4
        jne    SHORT loc_1_025A
        mov    al, 0ffh
        ret
loc_1_02B4:
        cmp    eax, 1
        db  00Fh, 084h, 063h, 001h, 0FFh, 0FFh
        clc
        ret
loc_1_02BF:
        db  00Fh, 0BAh, 02Dh, 0ACh, 004h, 000h, 000h, 001h
        jb     SHORT loc_1_02E5
        db  087h, 025h, 0A4h, 004h, 000h, 000h
        db  0FFh, 015h, 0B0h, 004h, 000h, 000h
        db  087h, 025h, 0A4h, 004h, 000h, 000h
        pushfd
        db  080h, 025h, 0ACh, 004h, 000h, 000h, 0FDh
        popfd
        ret
loc_1_02E5:
        db  0FFh, 025h, 0B0h, 004h, 000h, 000h
loc_1_02EB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jmp    SHORT loc_1_02F7
        db  0FFh, 025h, 0EBh, 002h, 000h, 000h
loc_1_02F7:
        cmp    word ptr [edi], 6
        jne    SHORT loc_1_032F
        db  0A1h, 0F4h, 003h, 000h, 000h
        cmp    dword ptr [eax], ebx
        jne    SHORT loc_1_030B
        db  0E8h, 01Ah, 0FFh, 0FEh, 0FFh
loc_1_030B:
        pushad
        mov    ecx, dword ptr [edi + 4]
        db  089h, 00Dh, 0B0h, 004h, 000h, 000h
        db  0B9h, 0BFh, 002h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, edi
        db  0E8h, 0D7h, 0FCh, 0FEh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
loc_1_032F:
        db  0FFh, 025h, 0EBh, 002h, 000h, 000h
loc_1_0335:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jmp    SHORT loc_1_0341
        db  0FFh, 025h, 035h, 003h, 000h, 000h
loc_1_0341:
        db  03Bh, 005h, 014h, 004h, 000h, 000h
        jne    SHORT loc_1_035B
        pushad
        xor    eax, eax
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, edi
        db  0E8h, 0CCh, 0FEh, 0FEh, 0FFh
        popad
        ret
loc_1_035B:
        db  0FFh, 025h, 035h, 003h, 000h, 000h
        db  000h, 000h, 000h
loc_1_0364:
        cmp    eax, 1
        db  00Fh, 084h, 0B3h, 001h, 0FFh, 0FFh
        clc
        ret
loc_1_036F:
        db  08Bh, 00Dh, 0F4h, 003h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_039C
        movzx  eax, byte ptr [ebp + 1ch]
        db  03Bh, 005h, 018h, 004h, 000h, 000h
        jae    SHORT loc_1_039C
        mov    ax, 804h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        test   word ptr [eax + 4], 8000h
        jne    SHORT loc_1_039C
        call   sub_1_039E
loc_1_039C:
        stc
        ret
sub_1_039E:
        push   ebx
        db  083h, 03Dh, 0BCh, 004h, 000h, 000h, 000h
        je     SHORT loc_1_03C2
        mov    eax, 10h
        mov    ebx, ecx
        xor    ecx, ecx
        db  0BEh, 0CCh, 006h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        db  089h, 035h, 0C0h, 004h, 000h, 000h
loc_1_03C2:
        pop    ebx
        ret
        db  054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 025h, 002h, 000h, 000h, 02Dh, 003h, 000h, 000h
        db  05Dh, 003h, 000h, 000h, 002h, 002h, 000h, 000h, 0F0h, 003h, 0F1h, 003h
loc_1_03E0:
        add    esi, esp
        add    esi, ebp
        add    esi, edi
        add    eax, dword ptr [eax]
        add    byte ptr [ecx + 20600h], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_03F4:
        add    al, 4
        add    byte ptr [eax], al
loc_1_03F8:
        or     byte ptr [eax + eax], al
        db  000h
loc_1_03FC:
        or     al, 4
        add    byte ptr [eax], al
loc_1_0400:
        adc    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
sub_1_043C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0440:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 036h, 000h, 004h, 000h, 000h, 000h, 056h, 046h, 042h, 041h
        db  043h, 04Bh, 055h, 050h, 000h, 000h, 081h, 0A0h, 004h, 000h, 000h, 000h, 052h, 000h, 000h, 000h
        db  053h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C4h, 003h, 000h, 000h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_04A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_04B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_04B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04BC:
        add    byte ptr [eax], al
loc_1_04BE:
        add    byte ptr [eax], al
loc_1_04C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_04CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
