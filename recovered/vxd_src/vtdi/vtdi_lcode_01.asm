; vtdi_lcode_01.asm -- VTDI object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VTDI_Service_00:
        mov    ax, 100h
        clc
        ret
VTDI_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F1h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 0E8h, 0FFh, 0FEh, 0FFh
        clc
        ret
        db  000h, 000h
VTDI_Service_10:
        db  0A1h, 080h, 004h, 000h, 000h
        db  083h, 005h, 080h, 004h, 000h, 000h, 001h
        db  083h, 015h, 080h, 004h, 000h, 000h, 000h
        clc
        ret
sub_1_0031:
        mov    ecx, 8
loc_1_0036:
        cmp    dword ptr [edx], eax
        je     SHORT loc_1_0041
        add    edx, 8
        loop   loc_1_0036
        stc
        ret
loc_1_0041:
        clc
        ret
loc_1_0043:
        push   edx
        call   sub_1_0031
        pop    ecx
        jb     SHORT loc_1_005C
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edx + 4], eax
        test   eax, eax
        jne    SHORT loc_1_0059
        mov    dword ptr [edx], eax
loc_1_0059:
        sub    eax, eax
        ret
loc_1_005C:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_0059
        mov    edx, ecx
        push   eax
        sub    eax, eax
        call   sub_1_0031
        pop    eax
        jb     SHORT loc_1_007B
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edx + 4], eax
        jmp    SHORT loc_1_0059
loc_1_007B:
        mov    al, 1
        ret
VTDI_Service_11:
        db  0BAh, 0C4h, 004h, 000h, 000h
        jmp    SHORT loc_1_0043
VTDI_Service_12:
        db  0BAh, 084h, 004h, 000h, 000h
        jmp    SHORT loc_1_0043
sub_1_008C:
        mov    ecx, dword ptr [eax]
loc_1_008E:
        jecxz  loc_1_009A
        cmp    ecx, edx
        jne    SHORT loc_1_0096
        jmp    SHORT loc_1_00A0
loc_1_0096:
        mov    ecx, dword ptr [ecx]
        jmp    SHORT loc_1_008E
loc_1_009A:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax], edx
loc_1_00A0:
        ret
sub_1_00A1:
        mov    ecx, dword ptr [eax]
        jecxz  loc_1_00B9
        cmp    ecx, edx
        jne    SHORT loc_1_00B5
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], 0
        jmp    SHORT loc_1_00B9
loc_1_00B5:
        mov    eax, ecx
        jmp    SHORT sub_1_00A1
loc_1_00B9:
        ret
loc_1_00BA:
        db  0B8h, 07Ch, 004h, 000h, 000h
        call   sub_1_00A1
        mov    dword ptr [edx + 0ch], 0
        jmp    SHORT loc_1_00E5
loc_1_00CD:
        mov    dword ptr [edx + 0ch], 0
        db  0CDh, 020h, 0F1h, 000h, 001h, 000h
        jb     SHORT loc_1_00F1
        mov    ecx, dword ptr [ebx + 8]
        test   byte ptr [ecx + 2dh], 2
        je     SHORT loc_1_00F1
loc_1_00E5:
        push   dword ptr [edx + 8]
        push   edx
        cld
        call   dword ptr [edx + 4]
        add    esp, 8
        ret
loc_1_00F1:
        db  0BEh, 0BAh, 000h, 000h, 000h
        sub    ebx, ebx
        mov    ecx, 801h
        sub    eax, eax
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        test   esi, esi
        je     SHORT loc_1_0116
        mov    dword ptr [edx + 0ch], esi
        db  0B8h, 07Ch, 004h, 000h, 000h
        call   sub_1_008C
loc_1_0116:
        ret
VTDI_Service_01:
        mov    edx, dword ptr [esp + 4]
        db  0B8h, 078h, 004h, 000h, 000h
        call   sub_1_008C
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [esp + 8]
        push   esi
        db  0BEh, 085h, 001h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edx + 14h], esi
        mov    eax, esi
        pop    esi
        cld
        ret
VTDI_Service_02:
        mov    edx, dword ptr [esp + 4]
        db  0B8h, 078h, 004h, 000h, 000h
        call   sub_1_00A1
        db  0B8h, 07Ch, 004h, 000h, 000h
        call   sub_1_00A1
        push   esi
        mov    eax, dword ptr [edx + 14h]
        mov    esi, eax
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    esi, dword ptr [edx + 0ch]
        or     eax, esi
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        sub    esi, esi
        mov    dword ptr [edx + 14h], esi
        mov    dword ptr [edx + 0ch], esi
        pop    esi
        cld
        ret
loc_1_0185:
        mov    dword ptr [edx + 14h], 0
        db  0B8h, 078h, 004h, 000h, 000h
        call   sub_1_00A1
        jmp    NEAR PTR loc_1_00CD
VTDI_Service_03:
        mov    edx, dword ptr [esp + 4]
        cmp    dword ptr [edx + 0ch], 0
        jne    SHORT loc_1_01D8
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 8], ecx
        push   esi
        push   ebx
        db  0BEh, 0BAh, 000h, 000h, 000h
        sub    eax, eax
        mov    ecx, 809h
        sub    ebx, ebx
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        test   esi, esi
        je     SHORT loc_1_01D3
        mov    dword ptr [edx + 0ch], esi
        db  0B8h, 07Ch, 004h, 000h, 000h
        call   sub_1_008C
loc_1_01D3:
        mov    eax, esi
        pop    ebx
        pop    esi
        ret
loc_1_01D8:
        mov    eax, edx
        ret
VTDI_Service_04:
        mov    edx, dword ptr [esp + 4]
        db  0B8h, 07Ch, 004h, 000h, 000h
        call   sub_1_00A1
        sub    eax, eax
        xchg   dword ptr [edx + 0ch], eax
        push   esi
        mov    esi, eax
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        pop    esi
        cld
        ret
VTDI_Service_05:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    dword ptr [edx + 8], ebx
        pop    ebx
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_1_020F:
        cmp    dword ptr [edx + 4], 0
        jne    SHORT loc_1_0225
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        sub    eax, eax
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        jmp    SHORT loc_1_020F
loc_1_0225:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    eax, dword ptr [edx]
        cld
        ret
VTDI_Service_13:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    dword ptr [edx + 8], ebx
        pop    ebx
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_1_0244:
        cmp    dword ptr [edx + 4], 0
        jne    SHORT loc_1_025D
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        mov    eax, 3e8h
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        jmp    SHORT loc_1_0244
loc_1_025D:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    eax, dword ptr [edx]
        cld
        ret
VTDI_Service_06:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], eax
        mov    ecx, dword ptr [eax + 8]
        jecxz  loc_1_0283
        xchg   ebx, ecx
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        mov    ebx, ecx
loc_1_0283:
        cld
        ret
VTDI_Service_09:
        push   esi
        push   edi
        push   ebx
        db  0BAh, 078h, 004h, 000h, 000h
        sub    edx, 0
loc_1_0290:
        mov    ecx, dword ptr [edx]
        jecxz  loc_1_02AE
        cmp    dword ptr [ecx + 10h], eax
        jne    SHORT loc_1_02AA
        sub    esi, esi
        xchg   dword ptr [ecx + 14h], esi
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        jmp    SHORT loc_1_0290
loc_1_02AA:
        mov    edx, ecx
        jmp    SHORT loc_1_0290
loc_1_02AE:
        db  0BAh, 07Ch, 004h, 000h, 000h
        sub    edx, 0
loc_1_02B6:
        mov    ecx, dword ptr [edx]
        jecxz  loc_1_02D4
        cmp    dword ptr [ecx + 10h], eax
        jne    SHORT loc_1_02D0
        sub    esi, esi
        xchg   dword ptr [ecx + 0ch], esi
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        jmp    SHORT loc_1_02B6
loc_1_02D0:
        mov    edx, ecx
        jmp    SHORT loc_1_02B6
loc_1_02D4:
        sub    ebx, ebx
        sub    ecx, ecx
        db  0BEh, 084h, 004h, 000h, 000h
        db  0BFh, 0C4h, 004h, 000h, 000h
loc_1_02E2:
        cmp    dword ptr [ebx + esi], eax
        jne    SHORT loc_1_02EE
        mov    dword ptr [ebx + esi], ecx
        mov    dword ptr [ebx + esi + 4], ecx
loc_1_02EE:
        cmp    dword ptr [ebx + edi], eax
        jne    SHORT loc_1_02FA
        mov    dword ptr [ebx + edi], ecx
        mov    dword ptr [ebx + edi + 4], ecx
loc_1_02FA:
        add    ebx, 8
        cmp    ebx, 40h
        jb     SHORT loc_1_02E2
        mov    edi, dword ptr [esp + 10h]
        db  0BEh, 0C4h, 004h, 000h, 000h
        mov    ebx, 8
loc_1_0310:
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     SHORT loc_1_031D
        push   edi
        call   eax
        add    esp, 4
loc_1_031D:
        add    esi, 8
        dec    ebx
        jne    SHORT loc_1_0310
        pop    ebx
        pop    edi
        pop    esi
        ret
sub_1_0327:
        push   esi
        push   edx
        push   eax
        mov    esi, eax
loc_1_032C:
        lodsb
        cmp    al, byte ptr [edx]
        jne    SHORT loc_1_0338
        test   al, al
        je     SHORT loc_1_0338
        inc    edx
        jmp    SHORT loc_1_032C
loc_1_0338:
        pop    eax
        pop    edx
        pop    esi
        ret
VTDI_Service_08:
        cld
        mov    edx, dword ptr [esp + 4]
sub_1_0341:
        push   esi
        db  08Bh, 00Dh, 004h, 005h, 000h, 000h
        jecxz  loc_1_035D
        db  0BEh, 008h, 005h, 000h, 000h
loc_1_034F:
        mov    eax, dword ptr [esi]
        call   sub_1_0327
        je     SHORT loc_1_0361
        add    esi, 8
        loop   loc_1_034F
loc_1_035D:
        sub    eax, eax
        pop    esi
        ret
loc_1_0361:
        mov    eax, dword ptr [esi + 4]
        mov    edx, esi
        pop    esi
        ret
VTDI_Service_07:
        cld
        mov    edx, dword ptr [esp + 4]
        call   sub_1_0341
        test   eax, eax
        jne    SHORT loc_1_03C4
        db  08Bh, 015h, 004h, 005h, 000h, 000h
        imul   edx, edx, 8
        db  081h, 0C2h, 008h, 005h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 4], eax
        db  0FFh, 005h, 004h, 005h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        push   ebx
        db  0BEh, 084h, 004h, 000h, 000h
        mov    ebx, 8
        mov    edi, eax
loc_1_03AB:
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     SHORT loc_1_03B8
        push   edi
        call   eax
        add    esp, 4
loc_1_03B8:
        add    esi, 8
        dec    ebx
        jne    SHORT loc_1_03AB
        pop    ebx
        pop    edi
        pop    esi
        mov    eax, esp
        ret
loc_1_03C4:
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_1_03E4
        push   esi
        push   edi
        mov    edi, edx
        lea    esi, [edx + 8]
        db  0B9h, 010h, 005h, 000h, 000h
        sub    ecx, esi
        rep movsb
        pop    edi
        pop    esi
        db  0FFh, 00Dh, 004h, 005h, 000h, 000h
        ret
loc_1_03E4:
        sub    eax, eax
        ret
        db  000h, 000h, 000h, 000h, 000h, 017h, 001h, 000h, 000h, 04Bh, 001h, 000h, 000h, 09Bh, 001h, 000h
        db  000h, 0DBh, 001h, 000h, 000h, 0FAh, 001h, 000h, 000h, 067h, 002h, 000h, 000h, 068h, 003h, 000h
        db  000h, 03Ch, 003h, 000h, 000h, 085h, 002h, 000h, 000h, 01Ch, 000h, 000h, 000h, 07Eh, 000h, 000h
        db  000h, 085h, 000h, 000h, 000h, 02Fh, 002h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 088h, 004h, 003h, 000h, 000h, 000h, 056h, 054h, 044h, 049h
        db  020h, 020h, 020h, 020h, 000h, 060h, 000h, 054h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 003h, 000h, 000h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
loc_1_0474:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
loc_1_0478:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_047C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0480:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_0484:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0504:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0508:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
