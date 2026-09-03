; ifsmgr_lcode_01_01.asm -- IFSMGR object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

        db  080h, 003h, 000h, 000h, 053h, 004h, 000h, 000h, 0E6h, 008h, 000h, 000h, 022h, 001h, 000h, 000h
        db  086h, 001h, 000h, 000h, 02Fh, 001h, 000h, 000h, 02Fh, 001h, 000h, 000h, 009h, 001h, 000h, 000h
        db  0E3h, 000h, 000h, 000h, 017h, 001h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0FCh, 001h, 000h, 000h
        db  0FCh, 001h, 000h, 000h, 024h, 001h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h
        db  0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h
        db  0E5h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0D5h, 001h, 000h, 000h
        db  0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h
        db  0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h
        db  0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0F6h, 000h, 000h, 000h
        db  0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h
        db  0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h
        db  0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h
        db  0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h, 0E3h, 000h, 000h, 000h
        db  0E3h, 000h, 000h, 000h
IFSMGR_Control:
        cmp    eax, 35h
        jae    SHORT loc_1_00E4
        db  02Eh, 0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_00E3:
        clc
loc_1_00E4:
        ret
IFSMGR_DEBUG_QUERY:
        db  0F7h, 005h, 0EBh, 000h, 000h, 000h, 000h, 000h, 040h, 000h
        db  00Fh, 085h, 05Ch, 002h, 0FFh, 0FFh
        ret
IFSMGR_W32_DEVICEIOCONTROL:
        cmp    ecx, 0
        jne    SHORT loc_1_00FF
        xor    eax, eax
        jmp    SHORT loc_1_0108
loc_1_00FF:
        push   esi
        db  0E8h, 0CAh, 0F4h, 0FFh, 0FFh
        add    esp, 4
loc_1_0108:
        ret
IFSMGR_CREATE_VM:
        push   ebx
        db  0E8h, 068h, 03Ah, 0FFh, 0FFh
        pop    ecx
        test   eax, eax
        jne    SHORT loc_1_0115
        ret
loc_1_0115:
        stc
        ret
IFSMGR_VM_INIT:
        push   ebx
        db  0E8h, 08Dh, 001h, 0FFh, 0FFh
        add    esp, 4
        clc
        ret
IFSMGR_SYS_VM_INIT:
        clc
        ret
IFSMGR_VM_SUSPEND:
        db  0A1h, 0B4h, 055h, 000h, 000h
        or     byte ptr [ebx + eax + 8], 8
        ret
loc_1_012F:
        db  00Fh, 0BAh, 02Dh, 074h, 059h, 000h, 000h, 001h
        jb     SHORT loc_1_0184
        call   IFSMGR_SYS_VM_TERMINATE
        db  0A1h, 0D7h, 061h, 000h, 000h
        mov    dword ptr [eax + 19h], 0ffffffffh
        push   ebx
        sub    esp, 104h
        mov    edi, esp
        push   edi
        db  0E8h, 0F8h, 005h, 0FFh, 0FFh
        add    esp, 4
        push   edi
        db  0E8h, 0E5h, 0BFh, 0FFh, 0FFh
        pop    ecx
        push   edi
        db  0E8h, 07Eh, 0D4h, 0FFh, 0FFh
        pop    ecx
        add    esp, 104h
        db  083h, 03Dh, 03Ch, 059h, 000h, 000h, 000h
        je     SHORT loc_1_0183
        push   2
        db  0E8h, 042h, 0FFh, 0FEh, 0FFh
        add    esp, 4
loc_1_0183:
        pop    ebx
loc_1_0184:
        clc
        ret
IFSMGR_SYS_VM_TERMINATE:
        db  00Fh, 0BAh, 02Dh, 074h, 059h, 000h, 000h, 000h
        jb     SHORT loc_1_01D3
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        push   ebx
        sub    esp, 104h
        mov    edi, esp
        push   edi
        db  0E8h, 0ACh, 005h, 0FFh, 0FFh
        add    esp, 4
        mov    byte ptr [edi + 4], 4
        mov    dword ptr [edi + 84h], 0ffffffffh
        push   edi
        db  0E8h, 051h, 0B0h, 0FFh, 0FFh
        pop    ecx
        add    esp, 104h
        db  0E8h, 0E7h, 063h, 000h, 000h
        push   0
        db  0E8h, 05Dh, 00Bh, 0FFh, 0FFh
        add    esp, 4
        pop    ebx
loc_1_01D3:
        clc
        ret
IFSMGR_DEVICE_REBOOT_NOTIFY:
        sub    esp, 104h
        mov    edi, esp
        push   edi
        db  0E8h, 06Eh, 005h, 0FFh, 0FFh
        push   0
        push   0
        db  0E8h, 0B2h, 01Ah, 000h, 000h
        add    esp, 8
        db  0E8h, 08Eh, 086h, 0FFh, 0FFh
        add    esp, 108h
        clc
        ret
loc_1_01FC:
        push   0
        push   dword ptr [ebx + 0ch]
        db  0E8h, 098h, 01Ah, 000h, 000h
        add    esp, 8
        db  068h, 018h, 002h, 000h, 000h
        push   ebx
        db  0E8h, 00Bh, 03Dh, 0FFh, 0FFh
        pop    ecx
        pop    ecx
        clc
        ret
loc_1_0218:
        int3
        ret
loc_1_021A:
        push   eax
        test   dword ptr [ebx], 10000h
        jne    SHORT loc_1_0235
        mov    eax, 2710h
        mov    ecx, 1
        db  0CDh, 020h, 0ECh, 000h, 001h, 000h
        pop    eax
        ret
loc_1_0235:
        db  0A1h, 0B4h, 055h, 000h, 000h
        xor    ecx, ecx
        xchg   dword ptr [ebx + eax + 10h], ecx
        jecxz  loc_1_024C
        push   ebx
        push   ecx
        db  0E8h, 023h, 03Fh, 0FFh, 0FFh
        add    esp, 8
loc_1_024C:
        pop    eax
        db  0FFh, 025h, 0ACh, 061h, 000h, 000h
loc_1_0253:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0A1h, 0B4h, 055h, 000h, 000h
        xor    ecx, ecx
        xchg   dword ptr [ebx + eax + 10h], ecx
        jecxz  loc_1_0270
        push   ebx
        push   ecx
        db  0E8h, 0FFh, 03Eh, 0FFh, 0FFh
        add    esp, 8
loc_1_0270:
        db  0FFh, 025h, 0B4h, 061h, 000h, 000h
loc_1_0276:
        push   eax
        db  0A1h, 0B4h, 055h, 000h, 000h
        cmp    dword ptr [ebx + eax + 10h], 0
        je     SHORT loc_1_0296
        bts    dword ptr [ebx + eax + 8], 2
        jb     SHORT loc_1_0296
        mov    eax, 100000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_1_0296:
        pop    eax
        db  0FFh, 025h, 0B0h, 061h, 000h, 000h
loc_1_029D:
        push   eax
loc_1_029E:
        db  0A1h, 0B4h, 055h, 000h, 000h
        cmp    dword ptr [ebx + eax + 10h], 0
        jne    SHORT loc_1_02FA
        db  0A1h, 044h, 059h, 000h, 000h
        cmp    word ptr [eax + 130h], 0
        jne    SHORT loc_1_02C0
        pop    eax
loc_1_02BA:
        db  0FFh, 025h, 0A0h, 061h, 000h, 000h
loc_1_02C0:
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    ecx, 0d1h
        sub    esi, esi
        call   sub_1_1584
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        jmp    SHORT loc_1_029E
loc_1_02FA:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_02BA
        push   ecx
        push   ebx
        db  0CDh, 020h, 0F1h, 000h, 001h, 000h
        mov    ecx, ebx
        pop    ebx
        jae    SHORT loc_1_0314
        cmp    ebx, ecx
        stc
        je     SHORT loc_1_032C
loc_1_0314:
        inc    byte ptr [ebx + eax + 9]
        bts    dword ptr [ebx + eax + 8], 1
        jb     SHORT loc_1_032B
        mov    eax, 10h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_1_032B:
        clc
loc_1_032C:
        pop    ecx
        pop    eax
        ret
loc_1_032F:
        db  0FFh, 035h, 0A8h, 061h, 000h, 000h
        jmp    SHORT loc_1_033D
loc_1_0337:
        db  0FFh, 035h, 0A4h, 061h, 000h, 000h
loc_1_033D:
        push   eax
        db  0A1h, 0B4h, 055h, 000h, 000h
        cmp    byte ptr [ebx + eax + 9], 0
        jne    SHORT loc_1_034C
        pop    eax
        ret
loc_1_034C:
        dec    byte ptr [ebx + eax + 9]
        pop    eax
        add    esp, 4
        ret
IFSMgr_Get_Version:
        mov    eax, 22h
        clc
        ret
sub_1_035C:
        push   eax
        push   ecx
        push   edx
        push   5
        db  0FFh, 035h, 0F0h, 055h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_1_0374:
        push   eax
        push   ecx
        push   edx
        db  0FFh, 035h, 0F0h, 055h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        pop    edx
        pop    ecx
        pop    eax
        ret
IFSMgr_RegisterMount:
        cmp    dword ptr [esp + 8], 22h
        jne    NEAR PTR loc_1_0419
        mov    ecx, dword ptr [esp + 4]
        db  08Dh, 015h, 0F4h, 055h, 000h, 000h
        cmp    dword ptr [esp + 0ch], 1
        jne    SHORT loc_1_03BC
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_0419
        call   sub_1_035C
        mov    dword ptr [edx], ecx
        mov    eax, 0
        mov    dword ptr [edx + 4], eax
        jmp    SHORT loc_1_0413
loc_1_03BC:
        db  080h, 03Dh, 028h, 059h, 000h, 000h, 009h
        jae    SHORT loc_1_0419
        call   sub_1_035C
        db  0FEh, 005h, 028h, 059h, 000h, 000h
        db  00Fh, 0B6h, 005h, 028h, 059h, 000h, 000h
        db  08Ah, 025h, 024h, 059h, 000h, 000h
        mul    ah
        db  08Dh, 090h, 0F4h, 055h, 000h, 000h
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 8], 40h
        mov    dword ptr [edx + 0ch], 0
        push   edx
        push   0
        push   0
        db  0E8h, 001h, 0FCh, 0FEh, 0FFh
        add    esp, 8
        pop    edx
        mov    dword ptr [edx + 4], eax
        push   eax
        push   0
        call   sub_1_0571
        lea    esp, [esp + 4]
        pop    eax
loc_1_0413:
        call   sub_1_0374
        ret
loc_1_0419:
        mov    eax, 0ffffffffh
        ret
IFSMgr_RegisterNet:
        cmp    dword ptr [esp + 8], 22h
        jne    SHORT loc_1_048A
        db  00Fh, 0B6h, 005h, 029h, 059h, 000h, 000h
        cmp    eax, 8
        jae    SHORT loc_1_048A
        call   sub_1_035C
        db  0FEh, 005h, 029h, 059h, 000h, 000h
        db  08Ah, 00Dh, 025h, 059h, 000h, 000h
        mul    cl
        db  08Dh, 090h, 094h, 056h, 000h, 000h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 8], 40h
        mov    dword ptr [edx + 0ch], 0
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [edx + 10h], ecx
        push   edx
        push   0ah
        push   1
        db  0E8h, 090h, 0FBh, 0FEh, 0FFh
        add    esp, 8
        pop    edx
        mov    dword ptr [edx + 4], eax
        push   eax
        push   1
        call   sub_1_0571
        lea    esp, [esp + 4]
        pop    eax
        call   sub_1_0374
        ret
loc_1_048A:
        mov    eax, 0ffffffffh
        ret
IFSMgr_RegisterMailSlot:
        cmp    dword ptr [esp + 8], 22h
        jne    SHORT loc_1_04F9
        db  00Fh, 0B6h, 005h, 02Ah, 059h, 000h, 000h
        db  080h, 03Dh, 02Ah, 059h, 000h, 000h, 004h
        jae    SHORT loc_1_04F9
        call   sub_1_035C
        db  0FEh, 005h, 02Ah, 059h, 000h, 000h
        db  08Ah, 00Dh, 026h, 059h, 000h, 000h
        mul    cl
        db  08Dh, 090h, 034h, 057h, 000h, 000h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 8], 40h
        mov    dword ptr [edx + 0ch], 0
        push   edx
        push   0
        push   2
        db  0E8h, 022h, 0FBh, 0FEh, 0FFh
        lea    esp, [esp + 8]
        pop    edx
        mov    dword ptr [edx + 4], eax
        push   eax
        push   2
        call   sub_1_0571
        lea    esp, [esp + 4]
        pop    eax
        call   sub_1_0374
        ret
loc_1_04F9:
        mov    eax, 0ffffffffh
        ret
IFSMgr_RegisterCFSD:
        cmp    dword ptr [esp + 8], 22h
        jne    SHORT loc_1_056B
        db  00Fh, 0B6h, 005h, 02Bh, 059h, 000h, 000h
        cmp    eax, 8
        jae    SHORT loc_1_056B
        call   sub_1_035C
        db  0FEh, 005h, 02Bh, 059h, 000h, 000h
        db  08Ah, 00Dh, 027h, 059h, 000h, 000h
        mul    cl
        db  08Dh, 090h, 074h, 057h, 000h, 000h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 8], 40h
        mov    dword ptr [edx + 0ch], 0
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [edx + 10h], ecx
        push   edx
        push   0
        push   3
        db  0E8h, 0B0h, 0FAh, 0FEh, 0FFh
        lea    esp, [esp + 8]
        pop    edx
        mov    dword ptr [edx + 4], eax
        push   eax
        push   3
        call   sub_1_0571
        lea    esp, [esp + 4]
        pop    eax
        call   sub_1_0374
        ret
loc_1_056B:
        mov    eax, 0ffffffffh
        ret
sub_1_0571:
        enter  4, 0
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        db  00Fh, 0B6h, 090h, 024h, 059h, 000h, 000h
        db  08Bh, 034h, 085h, 014h, 059h, 000h, 000h
        db  00Fh, 0B6h, 088h, 028h, 059h, 000h, 000h
        mov    bl, 1
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_1_059C
        add    esi, edx
        mov    bl, 0
loc_1_059C:
        dec    ecx
        jecxz  loc_1_05DE
        cld
loc_1_05A0:
        push   ecx
        push   esi
        mov    dword ptr [ebp - 4], 0
loc_1_05A9:
        mov    eax, dword ptr [esi + 8]
        cmp    eax, dword ptr [edx + esi + 8]
        je     SHORT loc_1_05D2
        sbb    bh, bh
        cmp    bh, bl
        jne    SHORT loc_1_05D2
        push   ecx
        lea    edi, [edx + esi]
        mov    ecx, edx
        shr    ecx, 2
loc_1_05C1:
        lodsd
        xchg   dword ptr [edi], eax
        mov    dword ptr [esi - 4], eax
        add    edi, 4
        loop   loc_1_05C1
        pop    ecx
        sub    esi, edx
        inc    dword ptr [ebp - 4]
loc_1_05D2:
        add    esi, edx
        loop   loc_1_05A9
        cmp    dword ptr [ebp - 4], 0
        pop    esi
        pop    ecx
        jne    SHORT loc_1_05A0
loc_1_05DE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_05E3:
        mov    al, byte ptr [ebx]
        add    byte ptr [eax], al
        pop    ds
        add    al, 0
        db  000h, 090h, 004h, 000h, 000h, 0FFh
        add    al, 0
        db  000h
IFSMGR_Service_118:
        enter  4, 0
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        cmp    dword ptr [ebp + 1ch], 22h
        jne    SHORT loc_1_0678
        cmp    dword ptr [ebp + 8], 3
        ja     SHORT loc_1_0678
        call   sub_1_035C
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        db  0FFh, 014h, 085h, 0E3h, 005h, 000h, 000h
        lea    esp, [esp + 0ch]
        cmp    eax, -1
        je     SHORT loc_1_0673
        mov    dword ptr [ebp - 4], eax
        push   eax
        push   dword ptr [ebp + 8]
        db  0E8h, 010h, 0FAh, 0FEh, 0FFh
        lea    esp, [esp + 8]
        mov    ecx, dword ptr [ebp + 8]
        db  08Bh, 034h, 08Dh, 014h, 059h, 000h, 000h
        db  08Ah, 089h, 024h, 059h, 000h, 000h
        mul    cl
        add    esi, eax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        jne    SHORT loc_1_0661
        mov    eax, 40h
loc_1_0661:
        mov    dword ptr [esi + 8], eax
        push   dword ptr [ebp + 8]
        call   sub_1_0571
        lea    esp, [esp + 4]
        mov    eax, dword ptr [ebp - 4]
loc_1_0673:
        call   sub_1_0374
loc_1_0678:
        pop    edi
        pop    esi
        leave
        ret
IFSMGR_Service_117:
        enter  4, 0
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        cmp    dword ptr [ebp + 14h], 22h
        jne    NEAR PTR loc_1_0745
        cmp    dword ptr [ebp + 8], 3
        ja     NEAR PTR loc_1_0745
        mov    eax, 5
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_1_06B0
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_0745
loc_1_06B0:
        call   sub_1_035C
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 087h, 0F9h, 0FEh, 0FFh
        lea    esp, [esp + 8]
        cmp    eax, -1
        je     SHORT loc_1_0740
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 8]
        db  08Bh, 034h, 095h, 014h, 059h, 000h, 000h
        db  00Fh, 0B6h, 08Ah, 024h, 059h, 000h, 000h
        mul    cl
        add    esi, eax
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   edx
        db  0E8h, 092h, 0B8h, 0FFh, 0FFh
        pop    edx
        lea    esp, [esp + 8]
        or     eax, eax
        jne    SHORT loc_1_0740
        db  00Fh, 0B6h, 08Ah, 02Ch, 059h, 000h, 000h
        sub    ecx, dword ptr [ebp - 4]
        dec    ecx
        db  00Fh, 0B6h, 092h, 024h, 059h, 000h, 000h
        mov    edi, esi
        add    edi, edx
        jecxz  loc_1_0716
loc_1_070E:
        cmp    dword ptr [edi], eax
        je     SHORT loc_1_0716
        add    edi, edx
        loop   loc_1_070E
loc_1_0716:
        mov    ecx, edi
        sub    ecx, esi
        sub    ecx, edx
        mov    edi, esi
        jecxz  loc_1_0728
        add    esi, edx
        shr    ecx, 2
        cld
        rep movsd
loc_1_0728:
        mov    ecx, dword ptr [ebp + 8]
        db  0FEh, 089h, 028h, 059h, 000h, 000h
        db  00Fh, 0B6h, 089h, 024h, 059h, 000h, 000h
        shr    ecx, 2
        sub    eax, eax
        cld
        rep stosd
loc_1_0740:
        call   sub_1_0374
loc_1_0745:
        pop    edi
        pop    esi
        leave
        ret
IFSMgr_Attach:
        mov    eax, 1
        stc
        ret
IFSMgr_DerefConnection:
        push   edi
        mov    edi, dword ptr [esp + 8]
        push   edi
        call   sub_1_28FE
        pop    ecx
        mov    dword ptr [edi + 7ch], 0
        pop    edi
        ret
        db  02Eh, 08Bh, 0C0h
IFSMgr_CheckHeap:
        sub    eax, eax
        ret
IFSMgr_DeleteMailSlot:
        db  0E9h, 03Ah, 0DAh, 0FFh, 0FFh
sub_1_0770:
        db  0CDh, 020h, 024h, 080h, 040h, 000h
sub_1_0776:
        db  0CDh, 020h, 06Ch, 080h, 040h, 000h
IFSMgr_Get_Drive_Info:
        ret
Query_PhysLock:
        push   ebx
        push   edi
        push   eax
        db  08Bh, 00Dh, 0A4h, 065h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        db  0A1h, 09Ch, 065h, 000h, 000h
        mov    cx, word ptr [eax + 10h]
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, dword ptr [edi + 14h]
        add    cx, word ptr [ebx + 0ch]
        pop    eax
        db  08Bh, 03Dh, 084h, 06Eh, 000h, 000h
loc_1_07A5:
        or     edi, edi
        je     SHORT loc_1_07C2
        test   byte ptr [edi + 1bh], 8
        jne    SHORT loc_1_07B4
        cmp    byte ptr [edi + 31h], al
        je     SHORT loc_1_07B9
loc_1_07B4:
        mov    edi, dword ptr [edi + 4]
        jmp    SHORT loc_1_07A5
loc_1_07B9:
        cmp    dword ptr [edi + 28h], ecx
        jne    SHORT loc_1_07C2
        clc
loc_1_07BF:
        pop    edi
        pop    ebx
        ret
loc_1_07C2:
        stc
        jmp    SHORT loc_1_07BF
loc_1_07C5:
        db  0FFh, 035h, 0E4h, 055h, 000h, 000h
        db  0E9h, 020h, 05Ch, 000h, 000h
        db  0EBh, 006h, 0FFh, 025h, 0E8h, 055h, 000h, 000h
loc_1_07D8:
        db  0C6h, 005h, 032h, 059h, 000h, 000h, 001h
        db  0FFh, 025h, 0E8h, 055h, 000h, 000h
loc_1_07E5:
        db  0C6h, 005h, 032h, 059h, 000h, 000h, 000h
        db  0FFh, 025h, 0ECh, 055h, 000h, 000h
IFSMgr_RunScheduledEvents:
        db  080h, 03Dh, 032h, 059h, 000h, 000h, 001h
        jae    SHORT loc_1_07FC
        ret
loc_1_07FC:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        clc
        ret
loc_1_0810:
        db  08Bh, 03Dh, 090h, 065h, 000h, 000h
        cmp    word ptr [edi], -1
        je     NEAR PTR sub_1_08E1
        push   edx
        push   ecx
        push   eax
        push   ebx
        db  0E8h, 081h, 0FAh, 0FEh, 0FFh
        db  0A3h, 0DCh, 066h, 000h, 000h
        pop    ecx
        pop    eax
        pop    ecx
        pop    edx
        db  08Bh, 03Dh, 0B4h, 055h, 000h, 000h
        db  0C7h, 044h, 01Fh, 00Ch, 0E1h, 008h, 000h, 000h
        jmp    NEAR PTR sub_1_08E1
loc_1_0845:
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        movzx  ecx, word ptr [ebp + 10h]
        mov    word ptr [ebp + 10h], ax
        cmp    cl, 0bdh
        db  00Fh, 084h, 0AAh, 00Eh, 0FFh, 0FFh
        cmp    cl, 74h
        jae    SHORT loc_1_087F
        mov    edx, 2
        mov    esi, 0ffffffffh
        mov    eax, 21h
        db  0FFh, 014h, 08Dh, 09Ch, 012h, 000h, 000h
        jb     SHORT loc_1_08A5
loc_1_087F:
        db  0A1h, 09Ch, 065h, 000h, 000h
        movzx  edx, word ptr [eax + 0eh]
        movzx  eax, word ptr [eax + 0ch]
        shl    edx, 4
        add    edx, eax
        add    edx, dword ptr [ebx + 4]
        mov    esi, 0ffffffffh
        db  0A1h, 0B4h, 055h, 000h, 000h
        call   dword ptr [ebx + eax + 0ch]
        jb     SHORT loc_1_08A5
        ret
loc_1_08A5:
        db  08Bh, 015h, 044h, 059h, 000h, 000h
        mov    cx, word ptr [edx + 1ceh]
        movzx  edx, word ptr [edx + 1cch]
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        ret
loc_1_08C0:
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_096A
loc_1_08CB:
        push   ecx
        push   esi
loc_1_08CD:
        mov    ecx, 0ch
        mov    esi, ebp
        cld
        rep movsd
        sub    edi, 30h
        pop    esi
        pop    ecx
        jmp    NEAR PTR loc_1_096A
sub_1_08E1:
        db  03Bh, 01Dh, 0B0h, 055h, 000h, 000h
        je     SHORT loc_1_0947
        test   ecx, 10000000h
        jne    SHORT loc_1_0947
        test   dword ptr [ebx], 20h
        jne    SHORT loc_1_0947
        test   byte ptr [ebx + eax + 8], 20h
        je     SHORT loc_1_0947
        push   eax
        db  0A1h, 044h, 059h, 000h, 000h
        test   byte ptr [eax + 12eh], 2
        je     SHORT loc_1_0946
        and    byte ptr [eax + 12eh], 0fdh
        db  0A1h, 09Ch, 065h, 000h, 000h
        cmp    byte ptr [eax + 1], 0
        jne    SHORT loc_1_0946
        push   ecx
        db  0A1h, 044h, 059h, 000h, 000h
        mov    cx, word ptr [eax + 1d2h]
        movzx  edx, word ptr [eax + 1d0h]
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        pop    ecx
        mov    eax, ecx
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        pop    eax
        ret
loc_1_0946:
        pop    eax
loc_1_0947:
        sub    esp, 6ch
        mov    edi, esp
        test   ecx, 10000000h
        jne    NEAR PTR loc_1_08CB
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        test   word ptr [ebp + 2ch], 200h
        je     NEAR PTR loc_1_08C0
loc_1_096A:
        lea    edi, [ebx + eax]
        push   edi
        push   ebx
        db  0F6h, 005h, 098h, 065h, 000h, 000h, 001h
        je     SHORT loc_1_0985
        push   ecx
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        pop    ecx
loc_1_0985:
        db  0A1h, 09Ch, 065h, 000h, 000h
        inc    byte ptr [eax + 1]
        db  0A1h, 0B0h, 065h, 000h, 000h
        inc    byte ptr [eax + 12b8h]
        db  0A1h, 044h, 059h, 000h, 000h
        inc    byte ptr [eax + 12fh]
loc_1_09A3:
        push   0fffffbbbh
        db  0A1h, 0B4h, 055h, 000h, 000h
        mov    ch, byte ptr [ebx + eax + 0ah]
        inc    ch
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        db  003h, 005h, 0BCh, 055h, 000h, 000h
        inc    word ptr [eax]
        test   ecx, 80000000h
        je     SHORT loc_1_09D5
        test   ecx, 4000000h
        jne    SHORT loc_1_09F8
loc_1_09D5:
        mov    edi, dword ptr [edi]
        test   edi, 40000h
        je     SHORT loc_1_09E5
        or     ecx, 20000000h
loc_1_09E5:
        test   ecx, 10000000h
        jne    SHORT loc_1_09F8
        and    edi, 30000h
        shl    edi, 8
        or     ecx, edi
loc_1_09F8:
        pop    edi
        push   ecx
        test   ecx, 10000000h
        jne    SHORT loc_1_0A46
        test   ecx, ecx
        db  08Bh, 00Dh, 0A4h, 065h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        jns    SHORT loc_1_0A52
        push   -1
        test   byte ptr [esp + 7], 4
        jne    SHORT loc_1_0A31
        db  0A1h, 09Ch, 065h, 000h, 000h
        movzx  eax, word ptr [eax + 10h]
        shl    eax, 4
        push   eax
        movzx  ecx, word ptr [esi + 12h]
        shl    ecx, 10h
        mov    cx, word ptr [esi + 14h]
        jmp    SHORT loc_1_0A6D
loc_1_0A31:
        mov    cx, word ptr [esi + 1ch]
        shl    ecx, 10h
        mov    cx, word ptr [esi + 1eh]
        push   0fffffbdbh
        mov    esi, dword ptr [esi + 0ch]
        jmp    SHORT loc_1_0A85
loc_1_0A46:
        push   esi
        mov    ecx, 0ffffffffh
        push   ecx
        mov    esi, dword ptr [ebp + 4]
        jmp    SHORT loc_1_0A85
loc_1_0A52:
        push   esi
        db  0A1h, 09Ch, 065h, 000h, 000h
        movzx  eax, word ptr [eax + 10h]
        shl    eax, 4
        push   eax
        db  0A1h, 09Ch, 065h, 000h, 000h
        mov    cx, word ptr [eax + 10h]
        add    cx, word ptr [ebx + 0ch]
loc_1_0A6D:
        sub    esi, esi
        mov    ax, word ptr [ebp + 3ch]
        or     ax, word ptr [ebp + 14h]
        je     SHORT loc_1_0A85
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    esi, eax
loc_1_0A85:
        sub    esp, 68h
        push   esi
        push   edx
        push   0fffffbbbh
        push   ecx
        mov    eax, 0ff0000h
        db  08Ah, 025h, 0ECh, 06Ah, 000h, 000h
        mov    al, byte ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        cmp    ecx, -1
        je     SHORT loc_1_0ABA
        test   dword ptr [ebx], 20h
        je     SHORT loc_1_0AB7
        test   dword ptr [ebx], 180h
        jne    SHORT loc_1_0ABA
loc_1_0AB7:
        movzx  eax, ax
loc_1_0ABA:
        push   eax
        mov    eax, esp
        test   byte ptr [eax + 8bh], 10h
        jne    SHORT loc_1_0AC9
        mov    dword ptr [edi + 10h], esp
loc_1_0AC9:
        lea    edi, [esp + 18h]
        mov    ecx, 1ah
        sub    eax, eax
        cld
        rep stosd
        mov    edi, esp
        push   edi
        movzx  ecx, byte ptr [edi + 88h]
        db  0FFh, 014h, 08Dh, 0DCh, 061h, 000h, 000h
        add    esp, 4
        cmp    dword ptr [edi + 7ch], 0
        je     SHORT loc_1_0AF8
        push   edi
        call   sub_1_28FE
        pop    edi
loc_1_0AF8:
        cmp    dword ptr [edi + 8ch], 0fffffbbbh
        je     SHORT loc_1_0B0B
        push   edi
        call   sub_1_18C5
        pop    edi
loc_1_0B0B:
        mov    ecx, dword ptr [edi + 94h]
        test   byte ptr [edi + 8bh], 10h
        jne    SHORT loc_1_0B21
        mov    dword ptr [ecx + 10h], 0
loc_1_0B21:
        cmp    byte ptr [ecx + 9], 0
        je     SHORT loc_1_0B4B
        mov    ebx, dword ptr [edi + 90h]
        db  0A1h, 044h, 059h, 000h, 000h
        cmp    word ptr [eax + 130h], 0
        jne    NEAR PTR loc_1_0C6C
        dec    byte ptr [ecx + 9]
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
        jmp    SHORT loc_1_0B21
loc_1_0B4B:
        test   byte ptr [ecx + 8], 6
        je     SHORT loc_1_0B6F
        xor    eax, eax
        btr    dword ptr [ecx + 8], 1
        jae    SHORT loc_1_0B5D
        sub    eax, 10h
loc_1_0B5D:
        btr    dword ptr [ecx + 8], 2
        jae    SHORT loc_1_0B69
        sub    eax, 100000h
loc_1_0B69:
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_1_0B6F:
        movsx  edx, word ptr [edi + 1ah]
        cmp    edx, -40h
        je     SHORT loc_1_0B86
        cmp    edx, -41h
        je     SHORT loc_1_0BA1
        test   edx, edx
        je     SHORT loc_1_0B9A
        jmp    NEAR PTR loc_1_0C99
loc_1_0B86:
        test   byte ptr [edi + 8bh], 10h
        je     SHORT loc_1_0BA1
        mov    word ptr [edi + 1ah], 3
        jmp    NEAR PTR loc_1_0C99
loc_1_0B9A:
        and    byte ptr [edi + 0c4h], 0feh
loc_1_0BA1:
        mov    ebx, dword ptr [edi + 90h]
        lea    esi, [edi + 98h]
        db  0F6h, 005h, 098h, 065h, 000h, 000h, 001h
        je     SHORT loc_1_0BBC
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_1_0BBC:
        mov    cl, byte ptr [edi + 8bh]
        test   cl, 10h
        je     SHORT loc_1_0BD4
        mov    edi, ebp
        mov    ecx, 0ch
        rep movsd
        mov    cl, 10h
        jmp    SHORT loc_1_0BDA
loc_1_0BD4:
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
loc_1_0BDA:
        add    esp, 104h
        mov    ebp, dword ptr [ebx + 8]
        db  0A1h, 09Ch, 065h, 000h, 000h
        sub    byte ptr [eax + 1], 1
        adc    byte ptr [eax + 1], 0
        db  0A1h, 0B0h, 065h, 000h, 000h
        sub    byte ptr [eax + 12b8h], 1
        adc    byte ptr [eax + 12b8h], 0
        db  0A1h, 044h, 059h, 000h, 000h
        dec    byte ptr [eax + 12fh]
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 0BCh, 055h, 000h, 000h
        xor    eax, eax
        dec    dword ptr [edi]
        js     SHORT loc_1_0C42
        cmp    word ptr [edi], ax
        sete   al
        dec    eax
        and    dword ptr [edi], eax
loc_1_0C29:
        cmp    edx, -41h
        je     SHORT loc_1_0C32
loc_1_0C2E:
        add    edx, -1
        ret
loc_1_0C32:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0CDh, 020h, 018h, 000h, 02Ah, 000h
        xor    edx, edx
        jmp    SHORT loc_1_0C2E
loc_1_0C42:
        cmp    word ptr [edi], 0
        sete   al
        dec    eax
        and    dword ptr [edi], eax
        jne    SHORT loc_1_0C29
        db  0FFh, 00Dh, 0A0h, 072h, 000h, 000h
        db  083h, 03Dh, 0A0h, 072h, 000h, 000h, 001h
        jne    SHORT loc_1_0C29
        push   edx
        db  068h, 0A0h, 072h, 000h, 000h
        call   IFSMgr_Wakeup
        pop    edx
        pop    edx
        jmp    SHORT loc_1_0C29
loc_1_0C6C:
        mov    ecx, 0d1h
loc_1_0C71:
        dec    word ptr [edx]
        mov    ebx, dword ptr [edi + 90h]
        mov    edx, dword ptr [edi + 14h]
        mov    esi, dword ptr [edi + 84h]
        mov    ebp, dword ptr [ebx + 8]
        mov    eax, dword ptr [edi + 94h]
        lea    esp, [edi + 90h]
        mov    edi, eax
        jmp    NEAR PTR loc_1_09A3
loc_1_0C99:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 0BCh, 055h, 000h, 000h
        mov    ecx, dword ptr [edi]
        dec    cx
        jne    SHORT loc_1_0CD1
        and    word ptr [edi + 2], cx
        or     ecx, ecx
        jns    SHORT loc_1_0CD1
        db  0FFh, 00Dh, 0A0h, 072h, 000h, 000h
        db  083h, 03Dh, 0A0h, 072h, 000h, 000h, 001h
        jne    SHORT loc_1_0CD1
        db  068h, 0A0h, 072h, 000h, 000h
        call   IFSMgr_Wakeup
        lea    esp, [esp + 4]
loc_1_0CD1:
        call   sub_1_3000
        mov    edx, edi
        pop    edi
        mov    ecx, dword ptr [edi + 88h]
        test   eax, eax
        jne    SHORT loc_1_0C71
        cmp    cl, 32h
        je     SHORT loc_1_0CEF
        or     byte ptr [edi + 0c4h], 1
loc_1_0CEF:
        xor    edx, edx
        jmp    NEAR PTR loc_1_0BA1
loc_1_0CF6:
        mov    ecx, dword ptr [edi + 88h]
        mov    eax, dword ptr [edi + 0b4h]
        movzx  edx, word ptr [edi + 0c0h]
        mov    ebx, dword ptr [edi + 0bch]
        int3
        mov    word ptr [edi + 1ah], 1
loc_1_0D16:
        ret
sub_1_0D17:
        push   edi
        mov    edi, dword ptr [esp + 8]
        push   esi
        mov    edx, dword ptr [edi + 90h]
        lea    esi, [edi + 98h]
        cmp    byte ptr [edi + 88h], 32h
        je     SHORT loc_1_0D37
        or     word ptr [esi + 2ch], 1
loc_1_0D37:
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        test   dword ptr [edx], 20h
        je     SHORT loc_1_0D57
        mov    eax, 24h
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        setne  al
        inc    al
        jmp    SHORT loc_1_0D8D
loc_1_0D57:
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        movzx  edx, word ptr [ecx + 24h]
        mov    cx, word ptr [ecx + 26h]
        test   byte ptr [edi + 8bh], 40h
        je     SHORT loc_1_0D7E
        cmp    byte ptr [esi + 1ch], 0a0h
        jae    SHORT loc_1_0D7E
        mov    al, byte ptr [esi + 1dh]
        mov    byte ptr [esi + 1ch], al
        mov    byte ptr [esi + 1dh], 71h
loc_1_0D7E:
        call   sub_1_0D90
        mov    edi, esi
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        sub    eax, eax
loc_1_0D8D:
        pop    esi
        pop    edi
        ret
sub_1_0D90:
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        mov    ax, word ptr [esi + 38h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [esi + 3ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [esi + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [esi]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [esi + 4]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [esi + 14h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [esi + 18h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [esi + 10h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [esi + 1ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        ret
sub_1_0DF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 03Dh, 09Ch, 065h, 000h, 000h
        mov    edx, dword ptr [ebx + 90h]
        mov    esi, dword ptr [edx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    edx, dword ptr [esi + 24h]
        mov    cx, word ptr [esi + 28h]
        call   sub_1_0D90
        mov    ax, word ptr [ebp + 14h]
        mov    word ptr [esi + 1ch], ax
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [esi + 14h], ax
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [esi], ax
        db  00Fh, 0B7h, 00Dh, 04Ah, 059h, 000h, 000h
        mov    dword ptr [esi + 4], ecx
        db  00Fh, 0B7h, 005h, 048h, 059h, 000h, 000h
        shl    eax, 4
        add    ecx, 12h
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        mov    word ptr [esi + 8], cx
        mov    eax, 3
        cmp    byte ptr [edi], 0
        jne    SHORT loc_1_0EA6
        inc    byte ptr [edi]
        db  0F6h, 005h, 098h, 065h, 000h, 000h, 001h
        je     SHORT loc_1_0E7C
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_1_0E7C:
        mov    eax, 24h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0F6h, 005h, 098h, 065h, 000h, 000h, 001h
        je     SHORT loc_1_0E9B
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
loc_1_0E9B:
        mov    byte ptr [edi], 0
        add    dword ptr [esi + 30h], 18h
        movzx  eax, byte ptr [esi + 1ch]
loc_1_0EA6:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        cmp    al, 1
        je     SHORT loc_1_0ED5
        mov    word ptr [edi + 4], 53h
        lea    esi, [ebx + 98h]
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [esi + 1ch], ax
        sub    eax, eax
loc_1_0ED5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
IFSMgr_Block:
        push   ebp
        mov    ebp, esp
        mov    eax, 5
        push   eax
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        cld
        leave
        ret
IFSMgr_Wakeup:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        leave
        ret
IFSMgr_BlockNoEvents:
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   eax
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        cld
        leave
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_0F14:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  090h
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        jne    SHORT loc_1_0F3D
        shr    ecx, 2
        rep stosd
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 3
loc_1_0F3D:
        rep stosb
        mov    eax, dword ptr [ebp + 8]
        pop    esi
        pop    edi
        leave
        ret
        db  08Bh, 0C0h
IFSMgr_SchedEvent:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        and    byte ptr [edx + 14h], 0fdh
        test   eax, eax
        je     SHORT loc_1_0F6B
        db  0BEh, 074h, 00Fh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edx + 4], esi
        pop    esi
        leave
        ret
loc_1_0F6B:
        call   sub_1_0F74
        pop    esi
        leave
        ret
        db  090h
sub_1_0F74:
        push   ebx
        pushfd
        cli
        mov    dword ptr [edx + 4], 0
        test   byte ptr [edx + 14h], 8
        jne    SHORT loc_1_0FCF
        test   byte ptr [edx + 14h], 40h
        jne    SHORT loc_1_0FC4
        test   byte ptr [edx + 14h], 20h
        jne    SHORT loc_1_0FB5
        xor    ebx, ebx
        mov    eax, 400000h
        mov    ecx, 809h
loc_1_0F9C:
        db  0BEh, 0ECh, 00Fh, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
loc_1_0FA7:
        test   esi, esi
        je     SHORT loc_1_0FB2
        mov    dword ptr [edx + 4], esi
        or     byte ptr [edx + 14h], 2
loc_1_0FB2:
        popfd
        pop    ebx
        ret
loc_1_0FB5:
        mov    eax, 1000h
        mov    ebx, dword ptr [edx + 8]
        mov    ecx, 9
        jmp    SHORT loc_1_0F9C
loc_1_0FC4:
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 0bh
        jmp    SHORT loc_1_0F9C
loc_1_0FCF:
        sub    eax, eax
        mov    ecx, 4bh
        test   byte ptr [edx + 14h], 20h
        jne    SHORT loc_1_0FDE
        sub    ebx, ebx
loc_1_0FDE:
        db  0BEh, 0ECh, 00Fh, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        jmp    SHORT loc_1_0FA7
        db  090h
loc_1_0FEC:
        mov    dword ptr [edx + 4], 0
        xor    eax, eax
        test   byte ptr [edx + 14h], 10h
        je     SHORT loc_1_102B
        push   edx
        push   0
        call   sub_1_1785
        add    esp, 4
        pop    edx
        test   eax, eax
        je     SHORT loc_1_1035
        mov    dword ptr [eax + 30h], edx
        mov    dword ptr [eax + 90h], ebx
        db  08Ah, 00Dh, 0ECh, 06Ah, 000h, 000h
        mov    byte ptr [eax + 5], cl
        db  08Bh, 00Dh, 0B4h, 055h, 000h, 000h
        add    ecx, ebx
        mov    dword ptr [eax + 94h], ecx
loc_1_102B:
        cld
        push   eax
        push   edx
        call   dword ptr [edx + 0ch]
        add    esp, 8
        ret
loc_1_1035:
        mov    eax, 2
        db  0BEh, 074h, 00Fh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        and    byte ptr [edx + 14h], 0fdh
        mov    dword ptr [edx + 4], esi
        ret
        db  02Eh, 08Bh, 0C0h
IFSMgr_QueueEvent:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        call   sub_1_0F74
        pop    esi
        leave
        ret
IFSMgr_KillEvent:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        xor    esi, esi
        xchg   dword ptr [edx + 4], esi
        test   byte ptr [edx + 14h], 2
        je     SHORT loc_1_1089
        test   byte ptr [edx + 14h], 8
        jne    SHORT loc_1_1080
        db  0CDh, 020h, 015h, 000h, 001h, 000h
        pop    esi
        leave
        ret
loc_1_1080:
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        pop    esi
        leave
        ret
loc_1_1089:
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        leave
        ret
sub_1_1092:
        push   1
        push   800h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_10C4
        db  083h, 03Ch, 09Dh, 0E0h, 066h, 000h, 000h, 000h
        jne    SHORT loc_1_10CB
        db  089h, 004h, 09Dh, 0E0h, 066h, 000h, 000h
        db  081h, 005h, 0E8h, 06Ah, 000h, 000h, 000h, 001h, 000h, 000h
loc_1_10C1:
        xor    eax, eax
loc_1_10C3:
        ret
loc_1_10C4:
        mov    eax, 8
        jmp    SHORT loc_1_10C3
loc_1_10CB:
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_1_10C1
sub_1_10D9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cld
        xor    eax, eax
        db  08Bh, 00Dh, 0E0h, 06Ah, 000h, 000h
loc_1_10EB:
        mov    ebx, ecx
        shr    ebx, 8
        db  08Bh, 03Ch, 09Dh, 0E0h, 066h, 000h, 000h
        xor    ch, ch
        lea    edi, [edi + ecx*8]
        not    cl
        inc    ecx
        lea    ecx, [ecx*2]
loc_1_1106:
        or     edi, edi
        je     SHORT loc_1_111D
        repne scasd
        je     SHORT loc_1_114A
        inc    ebx
        db  08Bh, 03Ch, 09Dh, 0E0h, 066h, 000h, 000h
        mov    ecx, 200h
        jmp    SHORT loc_1_1106
loc_1_111D:
        db  083h, 03Dh, 0E8h, 06Ah, 000h, 000h, 000h
        ja     SHORT loc_1_1142
        cmp    ebx, 0feh
        jae    SHORT loc_1_1193
        call   sub_1_1092
        or     eax, eax
        jne    SHORT loc_1_1193
        db  08Bh, 03Ch, 09Dh, 0E0h, 066h, 000h, 000h
        xor    ecx, ecx
        jmp    SHORT loc_1_1154
loc_1_1142:
        db  08Bh, 00Dh, 0E4h, 06Ah, 000h, 000h
        jmp    SHORT loc_1_10EB
loc_1_114A:
        sub    edi, 4
        inc    ecx
        shr    ecx, 1
        not    cl
        inc    cl
loc_1_1154:
        mov    ch, bl
        inc    ecx
        db  0A1h, 0E0h, 06Ah, 000h, 000h
        db  03Bh, 005h, 0E4h, 06Ah, 000h, 000h
        jne    SHORT loc_1_116A
        db  089h, 00Dh, 0E4h, 06Ah, 000h, 000h
loc_1_116A:
        db  089h, 00Dh, 0E0h, 06Ah, 000h, 000h
        dec    ecx
        add    ecx, 200h
        mov    word ptr [esi + 6], cx
        mov    eax, dword ptr [esi + 74h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 4], eax
        db  0FFh, 00Dh, 0E8h, 06Ah, 000h, 000h
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1193:
        mov    eax, 4
        mov    word ptr [esi + 1ah], ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_11A1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 6]
        sub    eax, 200h
        db  03Bh, 005h, 0E4h, 06Ah, 000h, 000h
        jae    SHORT loc_1_11BF
        db  0A3h, 0E4h, 06Ah, 000h, 000h
loc_1_11BF:
        mov    edi, eax
        shr    edi, 8
        db  08Bh, 03Ch, 0BDh, 0E0h, 066h, 000h, 000h
        xor    ah, ah
        lea    edi, [edi + eax*8]
        cld
        xor    eax, eax
        stosd
        stosd
        db  0FFh, 005h, 0E8h, 06Ah, 000h, 000h
        pop    edi
        pop    esi
        leave
        ret
        db  000h
loc_1_11E0:
        call   dword ptr [ecx]
        add    bh, bh
        inc    dword ptr [0bfffh]
        add    byte ptr [eax], al
        dec    eax
        adc    dword ptr [eax], eax
        db  000h, 019h
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 074h, 011h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h
loc_1_1208:
        movzx  ecx, byte ptr [ebp + 1dh]
        mov    edx, ecx
        and    dl, 7
        db  038h, 08Ah, 0E0h, 011h, 000h, 000h
        je     SHORT loc_1_121B
loc_1_1219:
        stc
        ret
loc_1_121B:
        mov    esi, 0ffffffffh
        db  0FFh, 024h, 095h, 0E8h, 011h, 000h, 000h
        db  08Bh, 045h, 01Ch, 00Fh, 0B7h, 055h, 028h, 08Bh, 04Dh, 024h, 0CCh, 0F9h, 0C3h
loc_1_1234:
        db  08Bh, 015h, 044h, 059h, 000h, 000h
        add    edx, dword ptr [ebx + 4]
        cmp    byte ptr [edx + 13h], 0
        jne    SHORT loc_1_1245
        stc
        ret
loc_1_1245:
        mov    byte ptr [edx + 13h], 0
        mov    ecx, 0c9h
        jmp    NEAR PTR sub_1_1584
        db  000h
loc_1_1254:
        or     al, 0
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        adc    al, 1ah
        add    byte ptr [eax], al
        adc    al, 1ah
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        adc    al, 1ah
        add    byte ptr [eax], al
        adc    al, 1ah
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        movsb
        db  015h, 000h, 000h, 00Ch, 000h
        add    byte ptr [eax], al
        adc    al, 1ah
        add    byte ptr [eax], al
        adc    al, 1ah
        add    byte ptr [eax], al
        adc    al, 1ah
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        adc    al, 1ah
        add    byte ptr [eax], al
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A8h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A0h, 01Eh, 000h, 000h, 036h, 019h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 00Ch, 019h, 000h, 000h, 09Eh, 01Ah, 000h, 000h
        db  0B8h, 01Ah, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 09Eh, 01Ah, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A2h, 01Bh, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 01Fh, 01Ch, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0B2h, 01Ah, 000h, 000h, 0D9h, 01Eh, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0B8h, 01Ah, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 08Fh, 01Ch, 000h, 000h, 08Fh, 01Ch, 000h, 000h, 08Fh, 01Ch, 000h, 000h
        db  08Fh, 01Ch, 000h, 000h, 08Fh, 01Ch, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 08Fh, 01Ch, 000h, 000h, 00Ch, 000h, 000h, 000h, 08Fh, 01Ch, 000h, 000h
        db  0FBh, 019h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0B8h, 01Ah, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 070h, 018h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 040h, 018h, 000h, 000h, 08Fh, 01Ch, 000h, 000h, 0F5h, 01Ah, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 08Fh, 01Ch, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 08Fh, 01Ch, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 010h, 01Eh, 000h, 000h, 0FCh, 018h, 000h, 000h, 084h, 018h, 000h, 000h
        db  088h, 01Ch, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 08Eh, 01Ah, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  088h, 01Ch, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h, 0A4h, 015h, 000h, 000h
        db  0A4h, 015h, 000h, 000h, 068h, 01Fh, 000h, 000h, 0A4h, 015h, 000h, 000h, 0EBh, 01Fh, 000h, 000h
loc_1_146C:
        movzx  ecx, byte ptr [ebp + 1dh]
        cmp    cl, 74h
        jae    SHORT loc_1_14C6
        xor    edx, edx
        db  0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 003h
        je     SHORT loc_1_14A1
        db  0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 002h
        je     SHORT loc_1_14A0
        push   ebx
        db  003h, 01Dh, 0B4h, 055h, 000h, 000h
        test   byte ptr [ebx + 8], 10h
        pop    ebx
        je     SHORT loc_1_14A1
        db  0E8h, 05Bh, 00Ah, 0FFh, 0FFh
        jae    SHORT loc_1_14A1
        jmp    SHORT loc_1_14B4
loc_1_14A0:
        inc    edx
loc_1_14A1:
        mov    esi, 0ffffffffh
        db  0FFh, 014h, 08Dh, 09Ch, 012h, 000h, 000h
        jae    SHORT loc_1_14E8
        cmp    ecx, -1
        je     SHORT loc_1_14FE
loc_1_14B4:
        db  08Bh, 00Dh, 0FCh, 06Ah, 000h, 000h
        db  08Bh, 015h, 0F8h, 06Ah, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_1_14C6:
        db  083h, 03Dh, 000h, 06Bh, 000h, 000h, 000h
        je     SHORT loc_1_14B4
        db  08Bh, 015h, 000h, 06Bh, 000h, 000h
        mov    esi, 0ffffffffh
        call   dword ptr [edx + ecx*4 - 1d0h]
        jb     SHORT loc_1_14B4
        mov    ecx, 0d4h
loc_1_14E8:
        db  0CDh, 020h, 011h, 000h, 040h, 000h
        db  0A1h, 0B4h, 055h, 000h, 000h
        mov    edx, 0ffffffffh
        call   dword ptr [ebx + eax + 0ch]
        jb     SHORT loc_1_14B4
loc_1_14FE:
        mov    ax, word ptr [ebp + 2ch]
        and    ax, 1
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        and    word ptr [ebp + 2ch], 0fffeh
        or     word ptr [ebp + 2ch], ax
        ret
loc_1_1516:
        db  0CDh, 020h, 011h, 000h, 040h, 000h
        movzx  ecx, byte ptr [ebp + 1dh]
        mov    esi, 0ffffffffh
        cmp    cl, 74h
        jae    SHORT loc_1_1567
        xor    edx, edx
        db  0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 003h
        je     SHORT loc_1_1556
        db  0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 002h
        je     SHORT loc_1_1555
        push   ebx
        db  003h, 01Dh, 0B4h, 055h, 000h, 000h
        test   byte ptr [ebx + 8], 10h
        pop    ebx
        je     SHORT loc_1_1556
        db  0E8h, 0A6h, 009h, 0FFh, 0FFh
        jae    SHORT loc_1_1556
        jmp    SHORT loc_1_15A4
loc_1_1555:
        inc    edx
loc_1_1556:
        db  0FFh, 014h, 08Dh, 09Ch, 012h, 000h, 000h
        jae    SHORT sub_1_1584
        cmp    ecx, -1
        je     SHORT loc_1_1565
        stc
loc_1_1565:
        ret
        db  0C3h
loc_1_1567:
        db  083h, 03Dh, 000h, 06Bh, 000h, 000h, 000h
        je     SHORT loc_1_15A4
        db  08Bh, 015h, 000h, 06Bh, 000h, 000h
        call   dword ptr [edx + ecx*4 - 1d0h]
        jb     SHORT loc_1_15A4
        mov    ecx, 0d4h
sub_1_1584:
        db  0A1h, 09Ch, 065h, 000h, 000h
        movzx  edx, word ptr [eax + 0eh]
        movzx  eax, word ptr [eax + 0ch]
        shl    edx, 4
        add    edx, eax
        add    edx, dword ptr [ebx + 4]
        db  0A1h, 0B4h, 055h, 000h, 000h
        jmp    dword ptr [ebx + eax + 0ch]
        db  08Bh, 0C0h
loc_1_15A4:
        stc
        ret
        db  08Bh, 0C0h
loc_1_15A8:
        test   dword ptr [ebx], 20h
        jne    SHORT loc_1_15F2
        test   edx, 1
        jne    SHORT loc_1_15F2
        db  0A1h, 09Ch, 065h, 000h, 000h
        movzx  eax, word ptr [eax + 10h]
        shl    eax, 4
        movzx  edx, word ptr [eax + 36h]
        shl    edx, 4
        movzx  eax, word ptr [eax + 34h]
        lea    edx, [eax + edx]
        movzx  edx, byte ptr [edx]
        db  08Bh, 03Dh, 044h, 059h, 000h, 000h
        add    edi, dword ptr [ebx + 4]
        test   byte ptr [edx + edi + 2eh], 4
        je     SHORT loc_1_15F2
        sub    eax, eax
        db  0BEh, 0F4h, 015h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
loc_1_15F2:
        stc
        ret
loc_1_15F4:
        db  08Bh, 03Dh, 0B4h, 055h, 000h, 000h
        add    edi, ebx
        mov    eax, dword ptr [edi + 4ch]
        mov    eax, dword ptr [eax + edx*4]
        mov    eax, dword ptr [eax + 15h]
        mov    edi, dword ptr [edi + edx*4 + 0d4h]
        mov    dword ptr [edi + 18h], eax
        ret
IFSMgr_GetLockState:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        cmp    ebx, 20h
        jae    SHORT loc_1_167C
        db  0F6h, 083h, 08Ch, 000h, 000h, 000h, 001h
        je     SHORT loc_1_167C
        db  08Bh, 034h, 09Dh, 0C0h, 065h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_167C
        mov    esi, dword ptr [esi]
        or     esi, esi
        je     SHORT loc_1_167C
        test   byte ptr [esi + 1bh], 10h
        je     SHORT loc_1_1683
        mov    ecx, dword ptr [ebp + 14h]
        jecxz  loc_1_1648
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [ecx], eax
loc_1_1648:
        mov    ecx, dword ptr [ebp + 0ch]
        jecxz  loc_1_1654
        movzx  eax, byte ptr [esi + 30h]
        dec    eax
        mov    dword ptr [ecx], eax
loc_1_1654:
        mov    ecx, dword ptr [ebp + 10h]
        jecxz  loc_1_1675
        xor    eax, eax
        bt     word ptr [esi + 32h], 8
        rcl    eax, 1
        bt     word ptr [esi + 32h], 3
        rcl    eax, 1
        bt     word ptr [esi + 32h], 5
        rcl    eax, 1
        mov    dword ptr [ecx], eax
loc_1_1675:
        xor    eax, eax
loc_1_1677:
        pop    esi
        pop    edi
        pop    ebx
        leave
        ret
loc_1_167C:
        mov    eax, 0fh
        jmp    SHORT loc_1_1677
loc_1_1683:
        mov    eax, 1
        jmp    SHORT loc_1_1677
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_1690:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 94h]
        mov    edx, dword ptr [eax + 4ch]
        sub    eax, eax
loc_1_16A2:
        mov    ecx, dword ptr [edx]
        add    edx, 4
        or     ecx, ecx
        je     SHORT loc_1_16B9
        cmp    word ptr [ecx], 0
        je     SHORT loc_1_16C1
        inc    eax
        cmp    eax, 0ffh
        jl     SHORT loc_1_16A2
loc_1_16B9:
        mov    eax, 1
loc_1_16BE:
        pop    esi
        pop    ebp
        ret
loc_1_16C1:
        mov    word ptr [ecx], 0ffffh
        mov    dword ptr [esi + 78h], ecx
        mov    word ptr [esi + 6], ax
        sub    eax, eax
        jmp    SHORT loc_1_16BE
sub_1_16D1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        or     ebx, ebx
        je     NEAR PTR loc_1_1780
        lea    esi, [ebx*4]
        mov    ecx, dword ptr [ebp + 10h]
        db  08Bh, 086h, 044h, 066h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_170D
        db  08Bh, 015h, 044h, 059h, 000h, 000h
        or     byte ptr [ebx + edx + 2eh], 1
        add    eax, 3
        mov    dword ptr [ecx + 4], eax
        mov    dx, 1
        jmp    SHORT loc_1_171B
loc_1_170D:
        db  0A1h, 044h, 059h, 000h, 000h
        and    byte ptr [ebx + eax + 2eh], 0feh
        mov    dx, 2
loc_1_171B:
        db  089h, 08Eh, 0BCh, 065h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        or     eax, eax
        je     SHORT loc_1_1734
        or     ecx, ecx
        je     SHORT loc_1_1731
        inc    byte ptr [eax + 19h]
        jmp    SHORT loc_1_1734
loc_1_1731:
        dec    byte ptr [eax + 19h]
loc_1_1734:
        mov    esi, dword ptr [ebp + 8]
        push   edx
        mov    byte ptr [esi + 4], bl
        mov    edi, dword ptr [esi + 10h]
        mov    eax, dword ptr [esi + 84h]
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 84h], 0ffffffffh
        push   esi
        db  0E8h, 021h, 012h, 0FFh, 0FFh
        add    esp, 8
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [esi + 84h], eax
        mov    dword ptr [esi + 10h], edi
        mov    byte ptr [esi + 89h], bl
        mov    al, byte ptr [ebp + 18h]
        mov    byte ptr [esi + 4], al
        db  068h, 0C7h, 029h, 000h, 000h
        push   esi
        db  0E8h, 0CAh, 0EDh, 0FEh, 0FFh
        add    esp, 8
loc_1_1780:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_1785:
        cli
        db  0A1h, 0ACh, 083h, 000h, 000h
        cmp    eax, 1
        sbb    ecx, ecx
        neg    ecx
        or     ecx, ecx
        jne    SHORT loc_1_17B3
        mov    ecx, dword ptr [eax + 10h]
        db  089h, 00Dh, 0ACh, 083h, 000h, 000h
        mov    word ptr [eax + 1ah], 0
        mov    dword ptr [eax + 14h], 0fffff000h
        mov    dword ptr [eax + 30h], 0fffff000h
loc_1_17B3:
        sti
        ret
IFSMgr_FreeIOReq:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        movsx  eax, word ptr [ecx + 1ah]
        cli
        db  08Bh, 015h, 0ACh, 083h, 000h, 000h
        mov    dword ptr [ecx + 10h], edx
        db  089h, 00Dh, 0ACh, 083h, 000h, 000h
        sti
        ret
sub_1_17D2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8ch], 0fffffbbbh
        je     SHORT loc_1_17EC
        jmp    NEAR PTR loc_1_18BE
loc_1_17EC:
        cli
        db  0A1h, 0CCh, 06Ch, 000h, 000h
        or     eax, eax
        jne    NEAR PTR loc_1_18A4
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_1_1811
        sti
        mov    word ptr [esi + 1ah], 8
        mov    eax, 8
        jmp    NEAR PTR loc_1_18C0
loc_1_1811:
        sti
        db  083h, 03Dh, 0D4h, 06Ch, 000h, 000h, 008h
        jge    SHORT loc_1_1836
        push   1
        push   71ch
        call   sub_1_5348
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_1836
        db  0FFh, 005h, 0D4h, 06Ch, 000h, 000h
        jmp    SHORT loc_1_18AC
loc_1_1836:
        call   sub_1_5370
        db  08Bh, 03Dh, 0BCh, 055h, 000h, 000h
        add    edi, eax
        mov    ebx, 1
loc_1_1848:
        cli
        db  0A1h, 0CCh, 06Ch, 000h, 000h
        cmp    eax, ebx
        sbb    ecx, ecx
        neg    ecx
        or     ecx, ecx
        je     SHORT loc_1_18A4
        mov    eax, dword ptr [edi]
        test   eax, 80000000h
        je     SHORT loc_1_1883
        and    eax, 7fffffffh
        mov    dword ptr [edi], eax
        db  0FFh, 00Dh, 0A0h, 072h, 000h, 000h
        db  039h, 01Dh, 0A0h, 072h, 000h, 000h
        jne    SHORT loc_1_1883
        db  068h, 0A0h, 072h, 000h, 000h
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_1883:
        or     dword ptr [edi], 40000000h
        db  0FFh, 005h, 0D0h, 06Ch, 000h, 000h
        db  068h, 0CCh, 06Ch, 000h, 000h
        call   IFSMgr_Block
        add    esp, 4
        and    dword ptr [edi], 0bfffffffh
        jmp    SHORT loc_1_1848
loc_1_18A4:
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 0CCh, 06Ch, 000h, 000h
loc_1_18AC:
        mov    dword ptr [esi + 8ch], eax
        mov    dword ptr [esi + 0ch], eax
        add    eax, 410h
        mov    dword ptr [esi + 2ch], eax
        sti
loc_1_18BE:
        sub    eax, eax
loc_1_18C0:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_18C5:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 8ch]
        cmp    ecx, 0fffffbbbh
        je     SHORT loc_1_1910
        mov    dword ptr [eax + 8ch], 0fffffbbbh
        db  0A1h, 0CCh, 06Ch, 000h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 0CCh, 06Ch, 000h, 000h
        db  083h, 03Dh, 0D0h, 06Ch, 000h, 000h, 000h
        je     SHORT loc_1_1910
        db  0C7h, 005h, 0D0h, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 0CCh, 06Ch, 000h, 000h
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_1910:
        pop    ebp
        ret
sub_1_1912:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8ch], 0fffffbbbh
        je     SHORT loc_1_1929
        sub    eax, eax
        jmp    SHORT loc_1_1975
loc_1_1929:
        db  083h, 03Dh, 0CCh, 06Ch, 000h, 000h, 000h
        jne    SHORT loc_1_196C
        db  083h, 03Dh, 0D4h, 06Ch, 000h, 000h, 008h
        jl     SHORT loc_1_196C
        push   1
        push   71ch
        call   sub_1_5348
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_1965
        mov    dword ptr [esi + 8ch], eax
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 8ch], 0fffffbbbh
        sub    eax, eax
        jmp    SHORT loc_1_1975
loc_1_1965:
        mov    eax, 8
        jmp    SHORT loc_1_1975
loc_1_196C:
        push   esi
        call   sub_1_17D2
        add    esp, 4
loc_1_1975:
        pop    esi
        pop    ebp
        ret
sub_1_1978:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8ch], 0fffffbbbh
        jne    SHORT loc_1_19A1
        push   0
        push   dword ptr [esi + 0ch]
        call   sub_1_5350
        add    esp, 8
        mov    dword ptr [esi + 0ch], 0fffffbbbh
        jmp    SHORT loc_1_19AA
loc_1_19A1:
        push   esi
        call   sub_1_18C5
        add    esp, 4
loc_1_19AA:
        pop    esi
        pop    ebp
        ret
loc_1_19AD:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 1ch]
        call   dword ptr [ebp + 8]
        mov    esp, ebp
        pop    ebp
        ret
IFSMgr_InstallFileSystemApiHook:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  0A1h, 0E4h, 06Ch, 000h, 000h
        db  0FFh, 005h, 0E4h, 06Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_19EA
loc_1_19CE:
        db  068h, 0E4h, 06Ch, 000h, 000h
        call   IFSMgr_Block
        add    esp, 4
        db  0A1h, 0E4h, 06Ch, 000h, 000h
        db  0FFh, 005h, 0E4h, 06Ch, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_19CE
loc_1_19EA:
        push   0ch
        call   IFSMgr_GetHeap
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_1A1D
        db  0FFh, 00Dh, 0E4h, 06Ch, 000h, 000h
        je     SHORT loc_1_1A19
        db  0C7h, 005h, 0E4h, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 0E4h, 06Ch, 000h, 000h
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_1A19:
        sub    eax, eax
        jmp    SHORT loc_1_1A9C
loc_1_1A1D:
        db  083h, 03Dh, 020h, 06Eh, 000h, 000h, 000h
        je     SHORT loc_1_1A43
        sub    edi, edi
loc_1_1A28:
        db  0FFh, 005h, 0E8h, 06Ch, 000h, 000h
        db  068h, 0E8h, 06Ch, 000h, 000h
        call   IFSMgr_Block
        add    esp, 4
        db  039h, 03Dh, 020h, 06Eh, 000h, 000h
        jne    SHORT loc_1_1A28
loc_1_1A43:
        cli
        db  083h, 03Dh, 0F8h, 06Ch, 000h, 000h, 000h
        jne    SHORT loc_1_1A57
        db  0C7h, 005h, 0F8h, 06Ch, 000h, 000h, 0ECh, 06Ch, 000h, 000h
loc_1_1A57:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        db  0A1h, 0F8h, 06Ch, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 4], eax
        db  0A1h, 0F8h, 06Ch, 000h, 000h
        mov    dword ptr [esi + 8], eax
        db  089h, 035h, 0F8h, 06Ch, 000h, 000h
        sti
        db  0FFh, 00Dh, 0E4h, 06Ch, 000h, 000h
        je     SHORT loc_1_1A94
        db  0C7h, 005h, 0E4h, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 0E4h, 06Ch, 000h, 000h
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_1A94:
        db  0A1h, 0F8h, 06Ch, 000h, 000h
        add    eax, 4
loc_1_1A9C:
        pop    edi
        pop    esi
        pop    ebp
        ret
IFSMgr_RemoveFileSystemApiHook:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        db  08Bh, 035h, 0F8h, 06Ch, 000h, 000h
        db  0A1h, 0E4h, 06Ch, 000h, 000h
        db  0FFh, 005h, 0E4h, 06Ch, 000h, 000h
        cmp    eax, edi
        je     SHORT loc_1_1AD9
loc_1_1ABD:
        db  068h, 0E4h, 06Ch, 000h, 000h
        call   IFSMgr_Block
        add    esp, 4
        db  0A1h, 0E4h, 06Ch, 000h, 000h
        db  0FFh, 005h, 0E4h, 06Ch, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_1ABD
loc_1_1AD9:
        db  081h, 0FEh, 0ECh, 06Ch, 000h, 000h
        je     SHORT loc_1_1AFD
        mov    eax, dword ptr [ebp + 8]
loc_1_1AE4:
        cmp    dword ptr [esi], eax
        je     SHORT loc_1_1AF5
        mov    edi, esi
        mov    esi, dword ptr [esi + 8]
        db  081h, 0FEh, 0ECh, 06Ch, 000h, 000h
        jne    SHORT loc_1_1AE4
loc_1_1AF5:
        db  081h, 0FEh, 0ECh, 06Ch, 000h, 000h
        jne    SHORT loc_1_1B04
loc_1_1AFD:
        mov    esi, 0ffffffffh
        jmp    SHORT loc_1_1B66
loc_1_1B04:
        db  083h, 03Dh, 020h, 06Eh, 000h, 000h, 000h
        je     SHORT loc_1_1B2A
        sub    ebx, ebx
loc_1_1B0F:
        db  0FFh, 005h, 0E8h, 06Ch, 000h, 000h
        db  068h, 0E8h, 06Ch, 000h, 000h
        call   IFSMgr_Block
        add    esp, 4
        db  039h, 01Dh, 020h, 06Eh, 000h, 000h
        jne    SHORT loc_1_1B0F
loc_1_1B2A:
        cli
        or     edi, edi
        mov    eax, dword ptr [esi + 8]
        je     SHORT loc_1_1B3F
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 8], eax
        jmp    SHORT loc_1_1B44
loc_1_1B3F:
        db  0A3h, 0F8h, 06Ch, 000h, 000h
loc_1_1B44:
        db  081h, 03Dh, 0F8h, 06Ch, 000h, 000h, 0ECh, 06Ch, 000h, 000h
        jne    SHORT loc_1_1B5A
        db  0C7h, 005h, 0F8h, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1B5A:
        sti
        push   esi
        call   IFSMgr_RetHeap
        add    esp, 4
        sub    esi, esi
loc_1_1B66:
        db  0FFh, 00Dh, 0E4h, 06Ch, 000h, 000h
        je     SHORT loc_1_1B85
        db  0C7h, 005h, 0E4h, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 0E4h, 06Ch, 000h, 000h
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_1B85:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h
sub_1_1B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 10h]
        or     ecx, ecx
        mov    eax, dword ptr [eax + 7ch]
        jne    SHORT loc_1_1BB3
        cmp    byte ptr [eax + 30h], 0
        jne    SHORT loc_1_1BBB
        mov    ecx, 1
        jmp    SHORT loc_1_1BBB
loc_1_1BB3:
        cmp    byte ptr [eax + 30h], 0
        jne    SHORT loc_1_1BBB
        sub    ecx, ecx
loc_1_1BBB:
        db  083h, 03Dh, 0F8h, 06Ch, 000h, 000h, 000h
        je     NEAR PTR loc_1_1CE9
        or     ecx, ecx
        je     NEAR PTR loc_1_1CE9
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp + 0ch], 0bh
        mov    eax, dword ptr [eax + 7ch]
        mov    dword ptr [ebp - 8], eax
        je     SHORT loc_1_1BE5
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_1C24
loc_1_1BE5:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [eax + 74h]
        add    eax, 74h
        test   byte ptr [ecx + 1fh], 0ch
        je     SHORT loc_1_1C24
        cmp    dword ptr [ebp + 0ch], 0bh
        mov    dword ptr [ebp + 0ch], 12h
        je     SHORT loc_1_1C08
        mov    dword ptr [ebp + 0ch], 2
loc_1_1C08:
        mov    eax, dword ptr [eax]
        test   byte ptr [eax + 1fh], 8
        je     SHORT loc_1_1C24
        cmp    dword ptr [ebp + 0ch], 0bh
        mov    dword ptr [ebp + 0ch], 13h
        je     SHORT loc_1_1C24
        mov    dword ptr [ebp + 0ch], 3
loc_1_1C24:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax + 1bh], 8
        je     SHORT loc_1_1C48
        cmp    dword ptr [ebp + 0ch], 11h
        jg     SHORT loc_1_1C3C
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_1_1C53
loc_1_1C3C:
        mov    eax, dword ptr [ebp + 10h]
        movzx  eax, byte ptr [eax + 89h]
        jmp    SHORT loc_1_1C50
loc_1_1C48:
        mov    eax, dword ptr [ebp - 8]
        movzx  eax, word ptr [eax + 14h]
        inc    eax
loc_1_1C50:
        mov    dword ptr [ebp - 4], eax
loc_1_1C53:
        cmp    dword ptr [ebp + 0ch], 1eh
        jne    SHORT loc_1_1C70
        db  083h, 03Dh, 0A8h, 06Eh, 000h, 000h, 000h
        jbe    SHORT loc_1_1C70
        mov    eax, dword ptr [ebp + 10h]
        movzx  eax, byte ptr [eax + 89h]
        inc    eax
        mov    dword ptr [ebp - 4], eax
loc_1_1C70:
        db  0FFh, 005h, 020h, 06Eh, 000h, 000h
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        lea    eax, [eax]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 8bh]
        and    eax, 3
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1bh]
        and    eax, 99h
        push   eax
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0A1h, 0F8h, 06Ch, 000h, 000h
        mov    eax, dword ptr [eax]
        call   eax
        add    esp, 18h
        mov    dword ptr [ebp - 0ch], eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        db  0FFh, 00Dh, 020h, 06Eh, 000h, 000h
        db  083h, 03Dh, 0E8h, 06Ch, 000h, 000h, 000h
        je     SHORT loc_1_1CE4
        db  083h, 03Dh, 020h, 06Eh, 000h, 000h, 000h
        jne    SHORT loc_1_1CE4
        db  0C7h, 005h, 0E8h, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 0E8h, 06Ch, 000h, 000h
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_1CE4:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_1CF2
loc_1_1CE9:
        push   dword ptr [ebp + 10h]
        call   dword ptr [ebp + 8]
        add    esp, 4
loc_1_1CF2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1CF9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 74h]
        sub    edi, edi
        test   byte ptr [esi + 3ch], 1
        je     SHORT loc_1_1D12
        mov    edi, 1
loc_1_1D12:
        mov    ax, word ptr [esi + 3eh]
loc_1_1D16:
        lea    ebx, [esi + 3eh]
        lea    ecx, [eax + 1]
        mov    word ptr [ebx], cx
        or     ax, ax
        je     SHORT loc_1_1D3B
loc_1_1D24:
        push   ebx
        call   IFSMgr_Block
        add    esp, 4
        mov    ax, word ptr [ebx]
        or     ax, ax
        lea    ecx, [eax + 1]
        mov    word ptr [ebx], cx
        jne    SHORT loc_1_1D24
loc_1_1D3B:
        or     edi, edi
        jne    SHORT loc_1_1D4A
        cmp    dword ptr [ebp + 0ch], 2
        jne    SHORT loc_1_1D4A
        or     word ptr [esi + 3ch], 1
loc_1_1D4A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_1D51:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 74h]
        mov    ax, word ptr [ecx + 3eh]
        add    ecx, 3eh
        dec    ax
        mov    word ptr [ecx], ax
        je     SHORT loc_1_1D76
        mov    word ptr [ecx], 0
        push   ecx
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_1D76:
        pop    ebp
        ret
sub_1_1D78:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0
        mov    edi, dword ptr [esi + 74h]
        push   esi
        call   sub_1_1CF9
        add    esp, 8
        push   esi
        call   sub_1_1D51
        add    esp, 4
        mov    eax, dword ptr [edi + 10h]
        cmp    dword ptr [esi + 7ch], eax
        je     SHORT loc_1_1DB7
        push   esi
        call   sub_1_28FE
        add    esp, 4
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 7ch], eax
        push   esi
        call   sub_1_2D7B
        add    esp, 4
loc_1_1DB7:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_1_1DBD:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_242B
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1E24
        mov    eax, dword ptr [esi + 7ch]
        mov    byte ptr [esi + 4], 2
        test   byte ptr [eax + 3eh], 1
        je     SHORT loc_1_1DE4
        mov    word ptr [esi + 18h], 1
loc_1_1DE4:
        push   2
        push   esi
        call   sub_1_1CF9
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1E24
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0bh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 8]
        call   sub_1_1B90
        add    esp, 10h
        push   esi
        call   sub_1_28FE
        add    esp, 4
        push   esi
        db  0E8h, 0ADh, 033h, 0FFh, 0FFh
        add    esp, 4
        push   esi
        call   sub_1_1D51
        add    esp, 4
loc_1_1E24:
        pop    esi
        pop    ebp
        ret
sub_1_1E27:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        db  0A1h, 084h, 06Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_1E6D
        mov    esi, 0c0h
        mov    ebx, 3
loc_1_1E3F:
        test   word ptr [eax + 32h], si
        je     SHORT loc_1_1E55
        push   eax
        call   sub_1_2D0D
        add    esp, 4
        db  0A1h, 084h, 06Eh, 000h, 000h
        jmp    SHORT loc_1_1E5D
loc_1_1E55:
        cmp    byte ptr [eax + 30h], bl
        jae    SHORT loc_1_1E63
        mov    eax, dword ptr [eax + 4]
loc_1_1E5D:
        or     eax, eax
        jne    SHORT loc_1_1E3F
        jmp    SHORT loc_1_1E6D
loc_1_1E63:
        call   sub_1_462C
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax]
loc_1_1E6D:
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_1E71:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        ret
sub_1_1E78:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        push   1
        mov    eax, dword ptr [edx + 24h]
        movzx  ebx, word ptr [eax + 14h]
        add    ebx, 16h
        push   ebx
        db  0E8h, 065h, 047h, 000h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1EA1
        mov    eax, 8
        jmp    SHORT loc_1_1ECD
loc_1_1EA1:
        mov    edx, dword ptr [ebp + 8]
        mov    edi, eax
        mov    esi, dword ptr [edx + 24h]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [ebp + 8]
        push   0
        mov    ecx, dword ptr [edx + 24h]
        mov    dword ptr [edx + 24h], eax
        push   ecx
        db  0E8h, 038h, 047h, 000h, 000h
        add    esp, 8
        sub    eax, eax
loc_1_1ECD:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1EE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        sub    ecx, ecx
        movzx  eax, byte ptr [ebx + 89h]
        db  08Bh, 034h, 085h, 0C0h, 065h, 000h, 000h
        mov    edi, dword ptr [ebx + 7ch]
        mov    edx, dword ptr [ebp + 8]
        cmp    edx, ecx
        je     SHORT loc_1_1F33
        db  0B8h, 0C0h, 065h, 000h, 000h
loc_1_1F08:
        mov    ebx, dword ptr [eax]
        or     ebx, ebx
        je     SHORT loc_1_1F19
        cmp    esi, ebx
        je     SHORT loc_1_1F19
        cmp    dword ptr [ebx], edi
        jne    SHORT loc_1_1F19
        mov    dword ptr [ebx], edx
        inc    ecx
loc_1_1F19:
        add    eax, 4
        db  03Dh, 03Ch, 066h, 000h, 000h
        jbe    SHORT loc_1_1F08
        add    byte ptr [edx + 19h], cl
        mov    eax, ecx
        add    dword ptr [edx + 10h], eax
        sub    byte ptr [edi + 19h], cl
        sub    dword ptr [edi + 10h], eax
        jmp    SHORT loc_1_1F45
loc_1_1F33:
        cmp    dword ptr [esi], edi
        je     SHORT loc_1_1F45
        push   ebx
        call   sub_1_2D7B
        add    esp, 4
        mov    dword ptr [esi], edi
        inc    byte ptr [edi + 19h]
loc_1_1F45:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_1F50:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [ebp + 0ch], 2
        mov    ecx, dword ptr [esi + 78h]
        mov    dword ptr [ebp - 4], ecx
        jne    NEAR PTR loc_1_217D
        mov    dword ptr [ebp - 2ch], 1
        mov    dword ptr [ebp - 28h], 40h
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    ebx, dword ptr [ebp - 0ch]
        or     ebx, ebx
        jne    SHORT loc_1_1FA9
        mov    word ptr [esi + 1ah], 4
        mov    eax, 4
        jmp    NEAR PTR loc_1_2290
loc_1_1FA9:
        test   byte ptr [ebp + 0ch], 8
        je     SHORT loc_1_1FC8
        test   byte ptr [esi + 8bh], 20h
        jne    SHORT loc_1_1FC4
        test   byte ptr [ebp + 0ch], 10h
        jne    SHORT loc_1_1FC4
        and    dword ptr [ebp + 0ch], 0fffffff7h
        jmp    SHORT loc_1_1FC8
loc_1_1FC4:
        or     dword ptr [ebp + 0ch], 4
loc_1_1FC8:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 4
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    NEAR PTR loc_1_20B0
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_20B0
        push   esi
        call   sub_1_1690
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2022
        mov    dword ptr [ebp - 24h], 0
        mov    dword ptr [ebp - 20h], ebx
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 30h], eax
        mov    word ptr [esi + 1ah], 4
        mov    eax, 4
        jmp    NEAR PTR loc_1_2290
loc_1_2022:
        test   byte ptr [ebp + 0ch], 20h
        je     SHORT loc_1_206E
        movzx  eax, word ptr [esi + 6]
        db  03Bh, 005h, 0D0h, 066h, 000h, 000h
        jl     SHORT loc_1_206E
        mov    eax, dword ptr [esi + 78h]
        mov    word ptr [eax], 0
        mov    dword ptr [ebp - 1ch], 0
        mov    dword ptr [ebp - 18h], ebx
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 34h], eax
        mov    word ptr [esi + 1ah], 4
        mov    eax, 4
        jmp    NEAR PTR loc_1_2290
loc_1_206E:
        mov    ecx, dword ptr [esi + 78h]
        mov    dword ptr [ebp - 4], ecx
        mov    edx, dword ptr [esi + 8]
        mov    eax, edx
        shr    eax, 10h
        sub    dx, ax
        mov    word ptr [ecx + 31h], dx
        mov    eax, dword ptr [esi + 8]
        shr    eax, 10h
        mov    word ptr [ecx + 2fh], ax
        mov    word ptr [ecx + 33h], 0
        mov    word ptr [ecx], 1
        mov    edi, ecx
        movzx  ax, byte ptr [esi + 4]
        mov    word ptr [ecx + 2], ax
        add    edi, 20h
        mov    eax, 20202020h
        stosd
        stosd
        stosw
        stosb
loc_1_20B0:
        mov    dword ptr [esi + 74h], ebx
        mov    edi, dword ptr [ebp + 0ch]
        and    edi, 8
        je     SHORT loc_1_2101
        push   esi
        call   sub_1_10D9
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2101
        mov    dword ptr [esi + 74h], 0
        mov    dword ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 10h], ebx
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 38h], eax
        mov    word ptr [esi + 1ah], 4
        mov    eax, 4
        jmp    NEAR PTR loc_1_2290
loc_1_2101:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 14h], ecx
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_2120
        test   byte ptr [ecx + 3], 80h
        jne    SHORT loc_1_2120
        test   byte ptr [esi + 4], 70h
        je     SHORT loc_1_2120
        or     byte ptr [esi + 8ah], 1
loc_1_2120:
        mov    al, byte ptr [esi + 8ah]
        and    al, 0dfh
        mov    byte ptr [ebx + 1eh], al
        movzx  ax, byte ptr [esi + 89h]
        dec    ax
        and    ax, 3fh
        add    ax, 40h
        mov    word ptr [ebx + 1ch], ax
        or     edi, edi
        lea    edi, [ebx + 1fh]
        je     SHORT loc_1_214D
        mov    byte ptr [edi], 2
        jmp    SHORT loc_1_2150
loc_1_214D:
        mov    byte ptr [edi], 1
loc_1_2150:
        call   sub_1_367B
        test   byte ptr [eax + 2], 8
        je     SHORT loc_1_215E
        or     byte ptr [edi], 40h
loc_1_215E:
        movzx  ax, byte ptr [esi + 4]
        mov    word ptr [ebx + 22h], ax
        mov    word ptr [ebx + 20h], 0ffffh
        mov    ax, word ptr [esi + 6]
        mov    word ptr [ebx + 30h], ax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebx + 34h], eax
        jmp    SHORT loc_1_2180
loc_1_217D:
        mov    ebx, dword ptr [esi + 74h]
loc_1_2180:
        test   byte ptr [ebp + 0ch], 4
        jne    NEAR PTR loc_1_2250
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 15h], 0
        mov    dword ptr [ecx + 7], 10001h
        movzx  ax, byte ptr [esi + 89h]
        dec    ax
        and    ax, 3fh
        add    ax, 40h
        mov    word ptr [ebx + 1ch], ax
        add    ax, 8000h
        mov    word ptr [ecx + 5], ax
        test   byte ptr [esi + 18h], 80h
        je     SHORT loc_1_21C7
        or     ax, 1000h
        mov    word ptr [ecx + 5], ax
loc_1_21C7:
        db  0A1h, 044h, 059h, 000h, 000h
        mov    eax, dword ptr [eax + 1c8h]
        or     eax, eax
        je     SHORT loc_1_21DD
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 39h], ax
loc_1_21DD:
        mov    ecx, dword ptr [ebp - 4]
        lea    edi, [esi + 6]
        test   byte ptr [ecx + 3], 80h
        jne    SHORT loc_1_21F9
        movzx  eax, word ptr [edi]
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        or     byte ptr [eax + ecx + 2eh], 4
        jmp    SHORT loc_1_2236
loc_1_21F9:
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 058h, 0E0h, 0FEh, 0FFh
        add    esp, 8
        mov    word ptr [edi], ax
        db  0A1h, 09Ch, 065h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [ecx + 31h], ax
        db  0A1h, 0A4h, 065h, 000h, 000h
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ecx + 2fh], ax
        db  066h, 0A1h, 0A0h, 083h, 000h, 000h
        mov    word ptr [ecx + 0bh], ax
        db  066h, 0FFh, 005h, 0A0h, 083h, 000h, 000h
loc_1_2236:
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [edi]
        mov    word ptr [ecx + 1bh], ax
        mov    ecx, dword ptr [esi + 94h]
        movzx  eax, word ptr [edi]
        mov    dword ptr [ecx + eax*4 + 0d4h], ebx
loc_1_2250:
        mov    eax, dword ptr [esi + 7ch]
        mov    dword ptr [ebx + 10h], eax
        cmp    dword ptr [ebx + 0ch], ebx
        je     SHORT loc_1_228B
        mov    eax, dword ptr [esi + 7ch]
        push   esi
        inc    word ptr [eax + 16h]
        call   sub_1_2D7B
        add    esp, 4
        mov    eax, dword ptr [esi + 7ch]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebx + 2ch], eax
        mov    eax, dword ptr [esi + 7ch]
        mov    dword ptr [eax + 24h], ebx
        mov    dword ptr [ebx + 28h], 0
        mov    eax, dword ptr [ebx + 2ch]
        or     eax, eax
        je     SHORT loc_1_228B
        mov    dword ptr [eax + 28h], ebx
loc_1_228B:
        mov    dword ptr [esi + 10h], ebx
        sub    eax, eax
loc_1_2290:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2297:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 74h]
        mov    ax, word ptr [esi + 6]
        cmp    ax, 0ffh
        ja     SHORT loc_1_22BD
        movzx  ecx, ax
        db  08Bh, 015h, 044h, 059h, 000h, 000h
        and    byte ptr [ecx + edx + 2eh], 0fbh
loc_1_22BD:
        cmp    ax, 200h
        jae    SHORT loc_1_22D7
        mov    ecx, dword ptr [esi + 94h]
        movzx  eax, ax
        mov    dword ptr [ecx + eax*4 + 0d4h], 0
loc_1_22D7:
        mov    eax, dword ptr [esi + 78h]
        or     eax, eax
        je     SHORT loc_1_22F9
        and    word ptr [eax + 5], 77ffh
        test   byte ptr [esi + 8ah], 20h
        jne    SHORT loc_1_22F2
        mov    word ptr [eax], 0
loc_1_22F2:
        mov    dword ptr [eax + 7], 0
loc_1_22F9:
        cmp    dword ptr [edi + 0ch], edi
        je     NEAR PTR loc_1_2383
        cmp    word ptr [edi + 3eh], 0
        je     SHORT loc_1_2312
        push   esi
        call   sub_1_1D51
        add    esp, 4
loc_1_2312:
        mov    eax, dword ptr [edi + 10h]
        push   esi
        dec    word ptr [eax + 16h]
        call   sub_1_28FE
        add    esp, 4
        mov    eax, dword ptr [edi + 28h]
        or     eax, eax
        lea    edx, [edi + 2ch]
        je     SHORT loc_1_2333
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax + 2ch], ecx
        jmp    SHORT loc_1_233B
loc_1_2333:
        mov    ecx, dword ptr [edi + 10h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 24h], eax
loc_1_233B:
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_1_2347
        mov    ecx, dword ptr [edi + 28h]
        mov    dword ptr [eax + 28h], ecx
loc_1_2347:
        mov    dword ptr [esi + 74h], 0
        test   byte ptr [edi + 1fh], 10h
        je     SHORT loc_1_235F
        push   0
        push   esi
        db  0E8h, 054h, 003h, 0FFh, 0FFh
        add    esp, 8
loc_1_235F:
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], edi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        jmp    SHORT loc_1_238C
loc_1_2383:
        push   esi
        db  0E8h, 058h, 016h, 0FFh, 0FFh
        add    esp, 4
loc_1_238C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2392:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 6]
        cmp    dx, 200h
        jb     SHORT loc_1_23AA
        movzx  eax, dx
        jmp    SHORT loc_1_23E5
loc_1_23AA:
        mov    ecx, dword ptr [esi + 80h]
        mov    eax, dword ptr [ecx + 34h]
        mov    edi, eax
        and    edi, 0ffff0fffh
        shr    edi, 0ch
        and    eax, 0ffffh
        add    edi, eax
        movzx  ecx, word ptr [ecx + 32h]
        sub    eax, eax
        cmp    ecx, eax
        jle    SHORT loc_1_23DA
loc_1_23CF:
        cmp    byte ptr [edi + eax], 0ffh
        je     SHORT loc_1_23E9
        inc    eax
        cmp    eax, ecx
        jl     SHORT loc_1_23CF
loc_1_23DA:
        mov    word ptr [esi + 1ah], 4
        mov    eax, 0ffffffffh
loc_1_23E5:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_23E9:
        mov    byte ptr [edi + eax], dl
        jmp    SHORT loc_1_23E5
sub_1_23EE:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 6], 200h
        jb     SHORT loc_1_2407
        push   eax
        call   sub_1_11A1
        add    esp, 4
        jmp    SHORT loc_1_2429
loc_1_2407:
        mov    eax, dword ptr [eax + 80h]
        mov    eax, dword ptr [eax + 34h]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        add    ecx, eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + eax], 0ffh
loc_1_2429:
        pop    ebp
        ret
sub_1_242B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 74h]
        or     edi, edi
        je     SHORT loc_1_24A1
        cmp    word ptr [edi + 20h], 0ffffh
        je     SHORT loc_1_24A1
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 7ch], eax
        push   esi
        call   sub_1_2D7B
        add    esp, 4
        mov    eax, dword ptr [edi + 10h]
        cmp    dword ptr [esi + 7ch], eax
        je     SHORT loc_1_2471
        push   esi
        call   sub_1_28FE
        add    esp, 4
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 7ch], eax
        push   esi
        call   sub_1_2D7B
        add    esp, 4
loc_1_2471:
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    al, byte ptr [edi + 1eh]
        and    al, 0f9h
        mov    byte ptr [esi + 8ah], al
        mov    eax, dword ptr [esi + 7ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [edi + 10h]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [esi + 84h], eax
        sub    eax, eax
        jmp    SHORT loc_1_24AC
loc_1_24A1:
        mov    word ptr [esi + 1ah], 6
        mov    eax, 6
loc_1_24AC:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_1_24B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   18h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_1F50
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_24DE
        mov    eax, dword ptr [esi + 74h]
        movzx  ecx, word ptr [esi + 6]
        mov    dword ptr [eax + 18h], ecx
        mov    byte ptr [eax + 1fh], 4
        sub    eax, eax
        mov    word ptr [esi + 1ah], 0
        jmp    SHORT loc_1_24E9
loc_1_24DE:
        mov    word ptr [esi + 1ah], 4
        mov    eax, 4
loc_1_24E9:
        pop    esi
        pop    ebp
        ret
sub_1_24EC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_11A1
        add    esp, 4
        push   esi
        call   sub_1_2297
        add    esp, 4
        pop    esi
        pop    ebp
        ret
sub_1_2508:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [esi + 6], ax
        push   esi
        db  0E8h, 094h, 0E2h, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_252B
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_1_2595
loc_1_252B:
        mov    edi, dword ptr [esi + 74h]
        mov    al, byte ptr [edi + 1fh]
        test   al, 0ch
        jne    SHORT loc_1_2542
        mov    word ptr [esi + 1ah], 6
        mov    eax, 6
        jmp    SHORT loc_1_2595
loc_1_2542:
        test   al, 8
        je     SHORT loc_1_254F
        push   edi
        call   sub_1_4575
        add    esp, 4
loc_1_254F:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 7ch], eax
        push   esi
        call   sub_1_2D7B
        add    esp, 4
        mov    eax, dword ptr [edi + 10h]
        cmp    dword ptr [esi + 7ch], eax
        je     SHORT loc_1_257E
        push   esi
        call   sub_1_28FE
        add    esp, 4
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 7ch], eax
        push   esi
        call   sub_1_2D7B
        add    esp, 4
loc_1_257E:
        mov    eax, dword ptr [esi + 7ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 20h], eax
        sub    eax, eax
        mov    word ptr [esi + 1ah], 0
loc_1_2595:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_25A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0E8h, 095h, 040h, 000h, 000h
        db  08Bh, 03Dh, 0BCh, 055h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        add    edi, eax
        mov    eax, dword ptr [esi]
        or     eax, eax
        lea    ecx, [eax + 1]
        mov    dword ptr [esi], ecx
        je     SHORT loc_1_2619
        mov    ebx, 1
loc_1_25C6:
        mov    eax, dword ptr [edi]
        test   eax, 80000000h
        je     SHORT loc_1_25F9
        db  039h, 01Dh, 0A0h, 072h, 000h, 000h
        jle    SHORT loc_1_25F9
        and    eax, 7fffffffh
        mov    dword ptr [edi], eax
        db  0FFh, 00Dh, 0A0h, 072h, 000h, 000h
        db  039h, 01Dh, 0A0h, 072h, 000h, 000h
        jne    SHORT loc_1_25F9
        db  068h, 0A0h, 072h, 000h, 000h
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_25F9:
        or     dword ptr [edi], 40000000h
        push   esi
        call   IFSMgr_Block
        add    esp, 4
        and    dword ptr [edi], 0bfffffffh
        mov    eax, dword ptr [esi]
        or     eax, eax
        lea    ecx, [eax + 1]
        mov    dword ptr [esi], ecx
        jne    SHORT loc_1_25C6
loc_1_2619:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_261E:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax]
        dec    ecx
        mov    dword ptr [eax], ecx
        je     SHORT loc_1_263A
        mov    dword ptr [eax], 0
        push   eax
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_263A:
        pop    ebp
        ret
sub_1_263C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  068h, 0A4h, 06Eh, 000h, 000h
        call   sub_1_25A0
        mov    esi, dword ptr [ebp + 8]
        add    esp, 4
        cmp    dword ptr [ebp + 0ch], 0
        mov    word ptr [esi + 18h], 0
        jne    NEAR PTR loc_1_270A
        mov    ecx, dword ptr [esi + 84h]
        cmp    ecx, -1
        je     SHORT loc_1_2687
        mov    eax, dword ptr [esi + 20h]
        or     eax, eax
        je     SHORT loc_1_2687
        cmp    dword ptr [eax + 1ch], ecx
        je     SHORT loc_1_2687
        mov    word ptr [esi + 1ah], 8cah
        jmp    NEAR PTR loc_1_28E3
loc_1_2687:
        push   0
        push   10h
        push   esi
        db  0E8h, 08Fh, 091h, 0FFh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_28EA
        mov    ebx, dword ptr [esi + 20h]
        cmp    byte ptr [ebx + 18h], 0
        jne    SHORT loc_1_26BC
        cmp    byte ptr [ebx + 19h], 0
        je     SHORT loc_1_26BC
        test   byte ptr [ebx + 1bh], 1
        jne    SHORT loc_1_26BC
        mov    word ptr [esi + 1ah], 3
        jmp    NEAR PTR loc_1_28DA
loc_1_26BC:
        and    byte ptr [ebx + 1bh], 0feh
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        push   esi
        db  0E8h, 0DDh, 090h, 0FFh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_26D9
        mov    dword ptr [esi + 7ch], ebx
        jmp    NEAR PTR loc_1_28DA
loc_1_26D9:
        mov    al, byte ptr [ebx + 18h]
        or     al, al
        je     NEAR PTR loc_1_28D3
        or     edi, edi
        jne    SHORT loc_1_26FB
        dec    al
        mov    byte ptr [ebx + 18h], al
        push   esi
        call   sub_1_28FE
        add    esp, 4
        jmp    NEAR PTR loc_1_28D3
loc_1_26FB:
        movzx  eax, al
        sub    dword ptr [ebx + 10h], eax
        mov    byte ptr [ebx + 18h], 0
        jmp    NEAR PTR loc_1_28D3
loc_1_270A:
        mov    eax, dword ptr [esi + 84h]
        mov    dword ptr [ebp - 0ch], eax
        mov    ebx, dword ptr [esi + 7ch]
        or     ebx, ebx
        jne    SHORT loc_1_2725
        mov    word ptr [esi + 1ah], 3
        jmp    NEAR PTR loc_1_28EA
loc_1_2725:
        cmp    eax, -1
        je     SHORT loc_1_273F
        cmp    dword ptr [ebx + 1ch], eax
        je     SHORT loc_1_273A
        mov    word ptr [esi + 1ah], 8cah
        jmp    NEAR PTR loc_1_28E3
loc_1_273A:
        cmp    eax, -1
        jne    SHORT loc_1_2748
loc_1_273F:
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esi + 84h], eax
loc_1_2748:
        cmp    dword ptr [ebp + 0ch], 20h
        mov    eax, dword ptr [ebp + 0ch]
        jg     NEAR PTR loc_1_2855
        lea    ecx, [eax*4]
        db  08Bh, 081h, 0BCh, 065h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_1_2782
        test   byte ptr [eax + 0bh], 2
        mov    edi, dword ptr [ebp + 10h]
        je     SHORT loc_1_2785
        cmp    edi, 4
        jge    SHORT loc_1_2785
        mov    word ptr [esi + 1ah], 5
        jmp    NEAR PTR loc_1_28E3
loc_1_2782:
        mov    edi, dword ptr [ebp + 10h]
loc_1_2785:
        db  08Bh, 081h, 044h, 066h, 000h, 000h
        mov    ax, word ptr [eax + 43h]
        and    ah, 0c0h
        cmp    ah, 0c0h
        je     SHORT loc_1_27A2
        mov    word ptr [esi + 1ah], 3
        jmp    NEAR PTR loc_1_28E3
loc_1_27A2:
        cmp    edi, 3
        jge    SHORT loc_1_27C4
        mov    eax, dword ptr [ebp + 0ch]
        dec    eax
        push   eax
        push   esi
        db  0E8h, 06Ah, 008h, 0FFh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_27C4
        mov    word ptr [esi + 1ah], 964h
        jmp    NEAR PTR loc_1_28E3
loc_1_27C4:
        push   edi
        push   esi
        db  0E8h, 0DCh, 08Fh, 0FFh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_1_28EA
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esi + 84h], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 1ch], eax
        push   ebx
        push   0
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_16D1
        mov    eax, dword ptr [ebp + 0ch]
        add    esp, 14h
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 001h
        je     SHORT loc_1_2832
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        stosd
        mov    dword ptr [esi + 84h], 0ffffffffh
        push   eax
        push   eax
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_16D1
        add    esp, 14h
        jmp    SHORT loc_1_283F
loc_1_2832:
        push   0
        push   dword ptr [ebp - 4]
        db  0E8h, 0C4h, 03Dh, 000h, 000h
        add    esp, 8
loc_1_283F:
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        dec    eax
        push   eax
        db  068h, 039h, 06Bh, 000h, 000h
        db  0E8h, 098h, 03Dh, 000h, 000h
        add    esp, 0ch
        jmp    SHORT loc_1_28CA
loc_1_2855:
        sub    al, 21h
        mov    byte ptr [ebp - 8], al
        mov    byte ptr [esi + 89h], al
        push   dword ptr [ebp + 10h]
        push   esi
        db  0E8h, 03Eh, 08Fh, 0FFh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_28EA
        mov    edi, dword ptr [ebp + 0ch]
        sub    edi, 21h
        push   edi
        push   4
        db  0E8h, 044h, 0D8h, 0FEh, 0FFh
        add    esp, 8
        mov    word ptr [esi + 1ah], ax
        or     ax, ax
        je     SHORT loc_1_288C
        jmp    SHORT loc_1_28E3
loc_1_288C:
        push   edi
        push   esi
        db  0E8h, 092h, 00Ch, 0FFh, 0FFh
        add    esp, 8
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [esi + 4], al
        mov    dword ptr [esi + 7ch], ebx
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esi + 84h], eax
        push   5
        push   esi
        db  0E8h, 0C2h, 000h, 0FFh, 0FFh
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        db  0C7h, 004h, 085h, 0B0h, 06Dh, 000h, 000h, 000h, 000h, 000h, 000h
        dec    byte ptr [ebx + 19h]
loc_1_28CA:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 84h], eax
loc_1_28D3:
        mov    dword ptr [esi + 7ch], ebx
        or     ebx, ebx
        je     SHORT loc_1_28EA
loc_1_28DA:
        push   esi
        call   sub_1_28FE
        add    esp, 4
loc_1_28E3:
        mov    dword ptr [esi + 7ch], 0
loc_1_28EA:
        db  068h, 0A4h, 06Eh, 000h, 000h
        call   sub_1_261E
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_28FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 7ch]
        mov    edx, dword ptr [esi + 10h]
        dec    edx
        mov    dword ptr [esi + 10h], edx
        je     SHORT loc_1_295E
        lea    ecx, [esi + 38h]
        cmp    word ptr [ecx], 0
        jbe    NEAR PTR loc_1_2BD6
        test   byte ptr [esi + 32h], 40h
        je     NEAR PTR loc_1_2BD6
        movzx  edi, byte ptr [esi + 19h]
        movzx  eax, word ptr [esi + 16h]
        add    edi, eax
        movzx  eax, word ptr [esi + 3ah]
        add    edi, eax
        movzx  eax, word ptr [esi + 3ch]
        add    edi, eax
        cmp    edi, edx
        jne    NEAR PTR loc_1_2BD6
        mov    word ptr [ecx], 0
        push   ecx
        call   IFSMgr_Wakeup
        add    esp, 4
        jmp    NEAR PTR loc_1_2BD6
loc_1_295E:
        mov    al, byte ptr [esi + 1bh]
        test   al, 1
        je     SHORT loc_1_2975
        cmp    byte ptr [esi + 1ah], 4
        je     SHORT loc_1_2975
        and    al, 0fdh
        mov    byte ptr [esi + 1bh], al
        jmp    NEAR PTR loc_1_2BD6
loc_1_2975:
        db  0A1h, 044h, 059h, 000h, 000h
        inc    byte ptr [eax + 12fh]
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebp - 1], al
        mov    byte ptr [edi + 4], 0
        test   byte ptr [esi + 3eh], 1
        je     SHORT loc_1_2994
        mov    byte ptr [edi + 4], 1
loc_1_2994:
        test   byte ptr [esi + 3eh], 2
        je     SHORT loc_1_299E
        mov    byte ptr [edi + 4], 2
loc_1_299E:
        movsx  eax, word ptr [edi + 1ah]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    word ptr [edi + 1ah], 0
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 1ch], eax
        mov    word ptr [edi + 18h], 0
        mov    dword ptr [edi + 0ch], 0
        mov    al, byte ptr [esi + 1bh]
        test   al, 10h
        je     NEAR PTR loc_1_2A5A
        cmp    byte ptr [edi + 4], 2
        jne    SHORT loc_1_2A0B
        db  0B9h, 084h, 06Eh, 000h, 000h
        db  083h, 03Dh, 084h, 06Eh, 000h, 000h, 000h
        je     NEAR PTR loc_1_2AA6
loc_1_29E7:
        mov    eax, dword ptr [ecx]
        cmp    eax, esi
        je     SHORT loc_1_29FA
        lea    ecx, [eax + 4]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_29E7
        jmp    NEAR PTR loc_1_2AA6
loc_1_29FA:
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 4], 0
        jmp    NEAR PTR loc_1_2AA6
loc_1_2A0B:
        mov    ebx, dword ptr [esi + 20h]
        db  08Bh, 035h, 084h, 06Eh, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_2A2F
        sub    eax, eax
loc_1_2A1A:
        cmp    dword ptr [esi + 20h], ebx
        jne    SHORT loc_1_2A28
        cmp    dword ptr [esi + 10h], eax
        jne    NEAR PTR loc_1_2BB1
loc_1_2A28:
        mov    esi, dword ptr [esi + 4]
        cmp    esi, eax
        jne    SHORT loc_1_2A1A
loc_1_2A2F:
        db  0B9h, 084h, 06Eh, 000h, 000h
        db  083h, 03Dh, 084h, 06Eh, 000h, 000h, 000h
        je     SHORT loc_1_2AA9
loc_1_2A3D:
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [eax + 20h], ebx
        jne    SHORT loc_1_2A50
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [eax + 4], esi
        mov    esi, eax
        jmp    SHORT loc_1_2A53
loc_1_2A50:
        lea    ecx, [eax + 4]
loc_1_2A53:
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_2A3D
        jmp    SHORT loc_1_2AA9
loc_1_2A5A:
        test   al, 80h
        je     SHORT loc_1_2A65
        db  0B9h, 080h, 06Eh, 000h, 000h
        jmp    SHORT loc_1_2A85
loc_1_2A65:
        push   0
        push   edi
        mov    eax, dword ptr [esi + 0ch]
        push   27h
        push   dword ptr [eax + 24h]
        call   sub_1_1B90
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_1_2BB1
        db  0B9h, 088h, 06Eh, 000h, 000h
loc_1_2A85:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_2AA6
loc_1_2A8A:
        mov    eax, dword ptr [ecx]
        cmp    eax, esi
        je     SHORT loc_1_2A9A
        lea    ecx, [eax + 4]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_2A8A
        jmp    SHORT loc_1_2AA6
loc_1_2A9A:
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 4], 0
loc_1_2AA6:
        mov    ebx, dword ptr [ebp - 8]
loc_1_2AA9:
        mov    eax, dword ptr [edi + 7ch]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [edi + 7ch], esi
        push   0
        push   edi
        mov    eax, dword ptr [esi + 0ch]
        push   28h
        push   dword ptr [eax + 28h]
        call   sub_1_1B90
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_2B0F
        cmp    byte ptr [edi + 4], 1
        je     SHORT loc_1_2B0F
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [edi + 7ch], eax
        db  0B9h, 088h, 06Eh, 000h, 000h
        mov    al, byte ptr [esi + 1bh]
        test   al, 10h
        je     SHORT loc_1_2AE8
        db  0B9h, 084h, 06Eh, 000h, 000h
        jmp    SHORT loc_1_2AF1
loc_1_2AE8:
        test   al, 80h
        je     SHORT loc_1_2AF1
        db  0B9h, 080h, 06Eh, 000h, 000h
loc_1_2AF1:
        mov    edx, esi
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_1_2B03
        sub    eax, eax
loc_1_2AFB:
        mov    edx, dword ptr [edx + 4]
        cmp    dword ptr [edx + 4], eax
        jne    SHORT loc_1_2AFB
loc_1_2B03:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [ecx], esi
        jmp    NEAR PTR loc_1_2BB1
loc_1_2B0F:
        mov    al, byte ptr [esi + 1bh]
        test   al, 10h
        je     SHORT loc_1_2B32
        cmp    byte ptr [edi + 4], 2
        je     SHORT loc_1_2B32
        mov    dword ptr [edi + 10h], ebx
        mov    dword ptr [edi + 1ch], 0
        cmp    dword ptr [esi + 1ch], 1
        sbb    eax, eax
        and    eax, 2
        push   eax
        jmp    SHORT loc_1_2B49
loc_1_2B32:
        test   al, 80h
        je     SHORT loc_1_2B52
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [edi + 10h], eax
        or     eax, eax
        je     SHORT loc_1_2B52
        mov    dword ptr [edi + 1ch], 0
        push   1
loc_1_2B49:
        push   edi
        db  0E8h, 085h, 0F5h, 0FEh, 0FFh
        add    esp, 8
loc_1_2B52:
        or     esi, esi
        je     SHORT loc_1_2BAA
loc_1_2B56:
        test   byte ptr [esi + 1bh], 80h
        movzx  eax, byte ptr [esi + 3]
        je     SHORT loc_1_2B7C
        db  0C7h, 004h, 0C5h, 014h, 058h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        movzx  eax, byte ptr [esi + 3]
        db  0C7h, 004h, 0C5h, 018h, 058h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        jmp    SHORT loc_1_2B87
loc_1_2B7C:
        db  0C7h, 004h, 085h, 0B0h, 083h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2B87:
        mov    ebx, dword ptr [esi + 4]
        cmp    dword ptr [esi + 40h], 0
        je     SHORT loc_1_2B99
        push   esi
        db  0E8h, 034h, 02Ah, 000h, 000h
        add    esp, 4
loc_1_2B99:
        push   0
        push   esi
        db  0E8h, 05Fh, 03Ah, 000h, 000h
        add    esp, 8
        mov    esi, ebx
        or     ebx, ebx
        jne    SHORT loc_1_2B56
loc_1_2BAA:
        mov    dword ptr [edi + 7ch], 0
loc_1_2BB1:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [edi + 0ch], eax
        cmp    byte ptr [edi + 4], 1
        je     SHORT loc_1_2BC5
        mov    ax, word ptr [ebp - 0ch]
        mov    word ptr [edi + 1ah], ax
loc_1_2BC5:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [edi + 4], al
        db  0A1h, 044h, 059h, 000h, 000h
        dec    byte ptr [eax + 12fh]
loc_1_2BD6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_2BDD:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   0
        mov    dword ptr [esi + 30h], 0
        push   esi
        call   sub_1_2C33
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_2C15
        db  0A1h, 088h, 06Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_2C15
        sub    ecx, ecx
loc_1_2C05:
        cmp    dword ptr [eax + 10h], ecx
        jne    SHORT loc_1_2C0E
        or     byte ptr [eax + 1bh], 2
loc_1_2C0E:
        mov    eax, dword ptr [eax + 4]
        cmp    eax, ecx
        jne    SHORT loc_1_2C05
loc_1_2C15:
        push   esi
        call   IFSMgr_FreeIOReq
        add    esp, 4
        push   1d4c0h
        db  068h, 08Ch, 06Eh, 000h, 000h
        call   IFSMgr_SchedEvent
        add    esp, 8
        pop    esi
        pop    ebp
        ret
sub_1_2C33:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    eax, eax
        sub    edi, edi
        db  08Bh, 01Dh, 088h, 06Eh, 000h, 000h
        cmp    ebx, eax
        mov    esi, dword ptr [ebp + 8]
        je     SHORT loc_1_2CA7
loc_1_2C4A:
        cmp    dword ptr [ebx + 10h], 0
        jne    SHORT loc_1_2C5D
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_2C66
        test   byte ptr [ebx + 1bh], 2
        jne    SHORT loc_1_2C66
        inc    eax
loc_1_2C5D:
        mov    ebx, dword ptr [ebx + 4]
loc_1_2C60:
        or     ebx, ebx
        jne    SHORT loc_1_2C4A
        jmp    SHORT loc_1_2CA7
loc_1_2C66:
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        cmp    byte ptr [ecx + 12fh], 0
        jne    SHORT loc_1_2C5D
        db  080h, 03Dh, 098h, 065h, 000h, 000h, 000h
        je     SHORT loc_1_2C8A
        or     edi, edi
        jne    SHORT loc_1_2C8A
        push   5
        call   sub_1_5358
        inc    edi
loc_1_2C8A:
        mov    dword ptr [esi + 7ch], ebx
        and    byte ptr [ebx + 1bh], 0feh
        inc    dword ptr [ebx + 10h]
        push   esi
        call   sub_1_28FE
        add    esp, 4
        sub    eax, eax
        db  08Bh, 01Dh, 088h, 06Eh, 000h, 000h
        jmp    SHORT loc_1_2C60
loc_1_2CA7:
        or     edi, edi
        je     SHORT loc_1_2CB1
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_1_2CB1:
        mov    dword ptr [esi + 7ch], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  068h, 08Ch, 06Eh, 000h, 000h, 0E8h, 098h, 0E3h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0C3h
sub_1_2CCB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  08Bh, 035h, 084h, 06Eh, 000h, 000h
        sub    edi, edi
        cmp    esi, edi
        je     SHORT loc_1_2CFE
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
loc_1_2CE6:
        cmp    esi, eax
        jne    SHORT loc_1_2CEF
        cmp    dword ptr [esi + 20h], edx
        je     SHORT loc_1_2D09
loc_1_2CEF:
        cmp    word ptr [esi + 14h], cx
        jne    SHORT loc_1_2CF7
        mov    edi, esi
loc_1_2CF7:
        mov    esi, dword ptr [esi + 4]
        or     esi, esi
        jne    SHORT loc_1_2CE6
loc_1_2CFE:
        or     edi, edi
        db  0B8h, 0DCh, 06Dh, 000h, 000h
        je     SHORT loc_1_2D09
        mov    eax, edi
loc_1_2D09:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_1_2D0D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_5370
        db  08Bh, 01Dh, 0BCh, 055h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        add    ebx, eax
        test   byte ptr [esi + 32h], 0c0h
        je     SHORT loc_1_2D76
        lea    edi, [esi + 38h]
loc_1_2D2C:
        mov    eax, dword ptr [ebx]
        test   eax, 80000000h
        je     SHORT loc_1_2D58
        and    eax, 7fffffffh
        mov    dword ptr [ebx], eax
        db  0FFh, 00Dh, 0A0h, 072h, 000h, 000h
        db  083h, 03Dh, 0A0h, 072h, 000h, 000h, 001h
        jne    SHORT loc_1_2D58
        db  068h, 0A0h, 072h, 000h, 000h
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_2D58:
        inc    word ptr [edi]
        or     dword ptr [ebx], 40000000h
        push   edi
        call   IFSMgr_Block
        add    esp, 4
        and    dword ptr [ebx], 0bfffffffh
        test   byte ptr [esi + 32h], 0c0h
        jne    SHORT loc_1_2D2C
loc_1_2D76:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_2D7B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 7ch]
        call   sub_1_5370
        db  08Bh, 00Dh, 0BCh, 055h, 000h, 000h
        add    ecx, eax
        test   byte ptr [edi + 19h], 90h
        jne    SHORT loc_1_2DF3
        mov    eax, dword ptr [edi + 74h]
        or     eax, eax
        je     SHORT loc_1_2DAA
        test   byte ptr [eax + 1fh], 40h
        jne    SHORT loc_1_2DF3
loc_1_2DAA:
        mov    ax, word ptr [esi + 32h]
        test   al, 0c0h
        je     SHORT loc_1_2DF3
        mov    edx, dword ptr [esi + 34h]
        cmp    dword ptr [edi + 8], edx
        je     SHORT loc_1_2DF3
        cmp    dword ptr [ecx], 0
        jg     SHORT loc_1_2DC3
        test   al, 80h
        je     SHORT loc_1_2DF3
loc_1_2DC3:
        mov    ebx, dword ptr [esi + 20h]
        mov    ax, word ptr [esi + 14h]
        mov    word ptr [ebp - 2], ax
        push   esi
        call   sub_1_2D0D
        add    esp, 4
        push   ebx
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_2CCB
        add    esp, 0ch
        mov    dword ptr [edi + 7ch], eax
        mov    esi, eax
        db  081h, 0FEh, 0DCh, 06Dh, 000h, 000h
        jne    SHORT loc_1_2E11
        jmp    SHORT loc_1_2E14
loc_1_2DF3:
        test   byte ptr [esi + 1bh], 8
        jne    SHORT loc_1_2E01
        db  081h, 0FEh, 0F8h, 06Eh, 000h, 000h
        jne    SHORT loc_1_2E11
loc_1_2E01:
        mov    eax, dword ptr [ecx]
        test   eax, 0ffffh
        je     SHORT loc_1_2E11
        or     eax, 20000000h
        mov    dword ptr [ecx], eax
loc_1_2E11:
        inc    dword ptr [esi + 10h]
loc_1_2E14:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2E1B:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        push   edx
        movzx  eax, byte ptr [edx + 89h]
        db  08Bh, 00Ch, 085h, 0C0h, 065h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ecx], 0
        dec    byte ptr [eax + 19h]
        call   sub_1_28FE
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2E50:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
loc_1_2E57:
        cli
        mov    eax, 0
        cmp    word ptr [esi + 1ah], 0
        setl   al
        or     eax, eax
        je     SHORT loc_1_2E74
        push   esi
        call   IFSMgr_Block
        add    esp, 4
        jmp    SHORT loc_1_2E57
loc_1_2E74:
        sti
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h
sub_1_2E80:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        push   edi
        db  0A1h, 044h, 059h, 000h, 000h
        inc    byte ptr [eax + 12fh]
        db  080h, 03Dh, 098h, 065h, 000h, 000h, 000h
        je     SHORT loc_1_2EA6
        push   5
        call   sub_1_5358
loc_1_2EA6:
        lea    edi, [ebp - 104h]
        sub    eax, eax
        mov    ecx, 41h
        rep stosd
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 090h, 0D8h, 0FEh, 0FFh
        mov    esi, dword ptr [ebp + 8]
        add    esp, 4
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 80h], eax
        mov    dword ptr [ebp - 78h], 0fffffbbbh
        mov    dword ptr [ebp - 0f8h], 0fffffbbbh
        mov    byte ptr [ebp - 79h], 5
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 104h], eax
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebp - 0f0h], eax
        mov    byte ptr [ebp - 0ffh], 1
        mov    word ptr [ebp - 0feh], 0ffh
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 50h], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 5ch], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 54h], eax
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebp - 58h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 6ch], eax
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 68h], eax
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 64h], eax
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_1_17D2
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_1_2FF9
        cmp    byte ptr [ebp - 4fh], 5fh
        jne    SHORT loc_1_2F5B
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 01Fh, 0C3h, 0FFh, 0FFh
        add    esp, 4
        jmp    SHORT loc_1_2F78
loc_1_2F5B:
        mov    al, byte ptr [ebp - 79h]
        and    eax, 4
        push   eax
        push   dword ptr [ebp - 80h]
        lea    eax, [ebp - 6ch]
        push   eax
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_1_0770
        add    esp, 10h
loc_1_2F78:
        mov    eax, dword ptr [ebp - 50h]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebp - 5ch]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp - 54h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [ebp - 6ch]
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [ebp - 68h]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [ebp - 64h]
        mov    dword ptr [esi + 1ch], eax
        cmp    dword ptr [ebp - 88h], 0
        je     SHORT loc_1_2FBA
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_1_28FE
        add    esp, 4
loc_1_2FBA:
        mov    ax, word ptr [ebp - 0eah]
        mov    word ptr [esi + 20h], ax
        or     ax, ax
        je     SHORT loc_1_2FD0
        movzx  eax, ax
        mov    dword ptr [esi + 4], eax
loc_1_2FD0:
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_1_18C5
        db  0A1h, 044h, 059h, 000h, 000h
        add    esp, 4
        dec    byte ptr [eax + 12fh]
        db  080h, 03Dh, 098h, 065h, 000h, 000h, 000h
        je     SHORT loc_1_2FF9
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_1_2FF9:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  000h
sub_1_3000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 1ah]
        movsx  ebx, dx
        mov    al, byte ptr [esi + 88h]
        cmp    al, 0ddh
        je     NEAR PTR loc_1_3392
        cmp    al, 0deh
        je     NEAR PTR loc_1_3392
        mov    al, byte ptr [esi + 8bh]
        mov    cl, al
        and    cl, 84h
        cmp    cl, 84h
        je     NEAR PTR loc_1_3392
        cmp    ebx, 0ffh
        jle    SHORT loc_1_304C
        db  0BFh, 008h, 071h, 000h, 000h
        jmp    SHORT loc_1_3065
loc_1_304C:
        db  0BFh, 02Ch, 070h, 000h, 000h
        db  038h, 01Dh, 02Ch, 070h, 000h, 000h
        je     SHORT loc_1_3065
loc_1_3059:
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_3065
        add    edi, 4
        cmp    byte ptr [edi], bl
        jne    SHORT loc_1_3059
loc_1_3065:
        test   al, 40h
        je     SHORT loc_1_307C
        cmp    dx, 7100h
        jne    SHORT loc_1_307C
        mov    word ptr [esi + 0b4h], dx
        jmp    NEAR PTR loc_1_3399
loc_1_307C:
        test   al, 10h
        jne    NEAR PTR loc_1_3107
        call   sub_1_367B
        mov    ecx, dword ptr [esi + 7ch]
        or     ecx, ecx
        jne    SHORT loc_1_309C
        db  08Bh, 00Dh, 09Ch, 065h, 000h, 000h
        mov    byte ptr [ecx + 3], 1
        jmp    SHORT loc_1_30B3
loc_1_309C:
        mov    cl, byte ptr [ecx + 1bh]
        and    cl, 8
        cmp    cl, 1
        sbb    cl, cl
        add    cl, 3
        db  08Bh, 015h, 09Ch, 065h, 000h, 000h
        mov    byte ptr [edx + 3], cl
loc_1_30B3:
        db  08Bh, 00Dh, 09Ch, 065h, 000h, 000h
        mov    cl, byte ptr [ecx + 3]
        mov    byte ptr [eax + 33h], cl
        db  08Bh, 00Dh, 09Ch, 065h, 000h, 000h
        mov    word ptr [ecx + 4], bx
        db  08Bh, 00Dh, 09Ch, 065h, 000h, 000h
        mov    cx, word ptr [ecx + 4]
        mov    word ptr [eax + 34h], cx
        db  08Bh, 015h, 09Ch, 065h, 000h, 000h
        mov    cl, byte ptr [edi + 2]
        mov    byte ptr [edx + 7], cl
        db  08Bh, 00Dh, 09Ch, 065h, 000h, 000h
        mov    cl, byte ptr [ecx + 7]
        mov    byte ptr [eax + 37h], cl
        db  08Bh, 015h, 09Ch, 065h, 000h, 000h
        mov    cl, byte ptr [edi + 3]
        mov    byte ptr [edx + 6], cl
        db  08Bh, 00Dh, 09Ch, 065h, 000h, 000h
        mov    cl, byte ptr [ecx + 6]
        mov    byte ptr [eax + 36h], cl
loc_1_3107:
        push   ebx
        movzx  eax, byte ptr [esi + 88h]
        push   eax
        call   _GetMappedErr
        add    esp, 8
        cmp    byte ptr [edi], 13h
        mov    word ptr [ebp - 2], ax
        jne    SHORT loc_1_3136
        cmp    byte ptr [esi + 88h], 40h
        jne    SHORT loc_1_3136
        mov    word ptr [esi + 0b4h], ax
        jmp    NEAR PTR loc_1_3399
loc_1_3136:
        cmp    byte ptr [edi + 1], 0ffh
        je     NEAR PTR loc_1_3385
        db  0A1h, 09Ch, 065h, 000h, 000h
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_1_3385
        mov    al, byte ptr [esi + 88h]
        cmp    al, 5fh
        je     NEAR PTR loc_1_3378
        cmp    al, 94h
        je     NEAR PTR loc_1_3378
        cmp    al, 44h
        je     NEAR PTR loc_1_336B
        cmp    al, 69h
        je     NEAR PTR loc_1_336B
        mov    al, byte ptr [esi + 8bh]
        test   al, 80h
        je     SHORT loc_1_318E
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0b4h], ax
        jmp    NEAR PTR loc_1_3399
loc_1_318E:
        test   al, 10h
        je     SHORT loc_1_31A2
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0b4h], ax
        jmp    NEAR PTR loc_1_3399
loc_1_31A2:
        mov    al, byte ptr [esi + 8ah]
        test   al, 40h
        je     SHORT loc_1_31BC
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0b4h], ax
        jmp    NEAR PTR loc_1_3399
loc_1_31BC:
        cmp    ebx, 20h
        je     SHORT loc_1_31C6
        cmp    ebx, 21h
        jne    SHORT loc_1_31DB
loc_1_31C6:
        cmp    byte ptr [esi + 0b5h], 5ch
        je     NEAR PTR loc_1_335E
        test   al, 1
        jne    NEAR PTR loc_1_335E
loc_1_31DB:
        mov    eax, dword ptr [esi + 90h]
        test   byte ptr [eax + 1], 1
        je     SHORT loc_1_31F7
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0b4h], ax
        jmp    NEAR PTR loc_1_3399
loc_1_31F7:
        push   esi
        call   sub_1_0D17
        add    esp, 4
        cmp    eax, 1
        jne    SHORT loc_1_3215
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0b4h], ax
        jmp    NEAR PTR loc_1_3399
loc_1_3215:
        mov    edx, dword ptr [esi + 7ch]
        or     edx, edx
        jne    SHORT loc_1_3234
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        mov    dx, 0feh
        and    word ptr [ecx + 1b6h], 7f00h
        jmp    NEAR PTR loc_1_32C9
loc_1_3234:
        mov    cl, byte ptr [edx + 1ah]
        cmp    cl, 2
        je     SHORT loc_1_32A1
        cmp    cl, 3
        je     SHORT loc_1_32A1
        cmp    byte ptr [esi + 89h], 0ffh
        jne    SHORT loc_1_324E
        mov    cl, 0ffh
        jmp    SHORT loc_1_3274
loc_1_324E:
        test   byte ptr [edx + 1bh], 10h
        je     SHORT loc_1_3259
        mov    cl, byte ptr [edx + 14h]
        jmp    SHORT loc_1_3274
loc_1_3259:
        sub    cl, cl
loc_1_325B:
        movzx  ebx, cl
        db  08Bh, 01Ch, 09Dh, 0C0h, 065h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_326D
        cmp    dword ptr [ebx], edx
        je     SHORT loc_1_3274
loc_1_326D:
        inc    cl
        cmp    cl, 20h
        jb     SHORT loc_1_325B
loc_1_3274:
        db  08Bh, 015h, 044h, 059h, 000h, 000h
        and    word ptr [edx + 1b6h], 7f00h
        db  08Bh, 015h, 044h, 059h, 000h, 000h
        movzx  bx, cl
        or     word ptr [edx + 1b6h], bx
        cmp    cl, 20h
        jb     SHORT loc_1_329B
        mov    cl, 0feh
loc_1_329B:
        movzx  dx, cl
        jmp    SHORT loc_1_32C9
loc_1_32A1:
        mov    cl, byte ptr [esi + 89h]
        db  08Bh, 01Dh, 044h, 059h, 000h, 000h
        add    cl, 10h
        mov    byte ptr [ebx + 1bfh], cl
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        mov    dx, 87h
        or     word ptr [ecx + 1b6h], 8000h
loc_1_32C9:
        mov    cl, byte ptr [esi + 88h]
        cmp    cl, 3eh
        je     SHORT loc_1_32E0
        cmp    cl, 7ch
        je     SHORT loc_1_32E0
        or     dx, 1800h
        jmp    SHORT loc_1_32E5
loc_1_32E0:
        or     dx, 800h
loc_1_32E5:
        or     eax, eax
        je     SHORT loc_1_3301
        push   edx
        movzx  ax, byte ptr [edi + 1]
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_0DF0
        add    esp, 10h
        jmp    NEAR PTR loc_1_339B
loc_1_3301:
        mov    word ptr [esi + 0b4h], dx
        test   byte ptr [esi + 8bh], 40h
        je     SHORT loc_1_331C
        mov    word ptr [esi + 0b0h], 71h
        jmp    SHORT loc_1_332B
loc_1_331C:
        movzx  ax, byte ptr [esi + 88h]
        mov    word ptr [esi + 0b0h], ax
loc_1_332B:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0ach], ax
        movzx  ax, byte ptr [edi + 1]
        mov    word ptr [esi + 98h], ax
        db  066h, 0A1h, 04Ah, 059h, 000h, 000h
        mov    word ptr [esi + 9ch], ax
        db  066h, 0A1h, 048h, 059h, 000h, 000h
        mov    word ptr [esi + 0a0h], ax
        jmp    SHORT loc_1_3399
loc_1_335E:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0b4h], ax
        jmp    SHORT loc_1_3399
loc_1_336B:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0b4h], ax
        jmp    SHORT loc_1_3399
loc_1_3378:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0b4h], ax
        jmp    SHORT loc_1_3399
loc_1_3385:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 0b4h], ax
        jmp    SHORT loc_1_3399
loc_1_3392:
        mov    word ptr [esi + 0b4h], bx
loc_1_3399:
        sub    eax, eax
loc_1_339B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
_GetMappedErr:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        cmp    ebx, 36h
        jae    SHORT loc_1_33BC
        movzx  eax, bl
        shl    eax, 8
        add    eax, 0ffh
        jmp    SHORT loc_1_3401
loc_1_33BC:
        jne    SHORT loc_1_33C5
        mov    eax, 0ffffh
        jmp    SHORT loc_1_3401
loc_1_33C5:
        mov    eax, dword ptr [ebp + 0ch]
        db  00Fh, 0B6h, 00Dh, 03Fh, 001h, 000h, 000h
        lea    edx, [ebx - 36h]
        cmp    ecx, edx
        jbe    SHORT loc_1_3401
        db  00Fh, 0B6h, 08Bh, 0D2h, 000h, 000h, 000h
        cmp    ecx, 0ffh
        je     SHORT loc_1_3401
        db  08Dh, 091h, 0B0h, 000h, 000h, 000h
        movzx  ecx, byte ptr [edx]
        or     ecx, ecx
        je     SHORT loc_1_33FE
loc_1_33F2:
        movzx  esi, byte ptr [edx + 1]
        inc    edx
        cmp    esi, eax
        je     SHORT loc_1_33FE
        dec    ecx
        jne    SHORT loc_1_33F2
loc_1_33FE:
        movzx  eax, byte ptr [edx]
loc_1_3401:
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h
IFSMgr_Get_NetTime:
        db  0CDh, 020h, 008h, 000h, 005h, 000h
        add    edx, 0e44h
        db  00Fh, 0AFh, 015h, 044h, 001h, 000h, 000h
        push   edx
        mov    ecx, 3e8h
        xor    edx, edx
        div    ecx
        push   edx
        push   eax
        db  0E8h, 03Fh, 030h, 000h, 000h
        shl    eax, 2
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        pop    ecx
        add    eax, ecx
        pop    edx
        pop    ecx
        add    eax, ecx
        ret
        db  000h, 000h, 000h
IFSMgr_printf:
        db  0F7h, 005h, 046h, 034h, 000h, 000h, 000h, 000h, 040h, 000h
        db  00Fh, 085h, 0D0h, 0CCh, 0FEh, 0FFh
        ret
sub_1_3451:
        mov    eax, dword ptr [esp + 4]
        push   8
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    ecx, eax
        or     ecx, edx
        je     SHORT loc_1_3477
        mov    dword ptr [edx], eax
        mov    eax, edx
loc_1_3477:
        ret
sub_1_3478:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        ret
sub_1_3489:
        push   edi
        mov    edi, dword ptr [esp + 8]
        mov    ecx, dword ptr [edi]
        add    edi, 4
        sub    ecx, 4
        shr    ecx, 2
        sub    eax, eax
        rep stosd
        pop    edi
        ret
sub_1_349F:
        push   1
        call   sub_1_3451
        pop    ecx
        db  08Bh, 00Dh, 010h, 071h, 000h, 000h
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0ch], 1000h
        db  0A3h, 010h, 071h, 000h, 000h
        db  0C7h, 005h, 014h, 071h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
        db  090h
IFSMgr_FillHeapSpare:
        db  083h, 03Dh, 014h, 071h, 000h, 000h, 000h
        jne    SHORT loc_1_34DA
        db  083h, 03Dh, 010h, 071h, 000h, 000h, 000h
        je     SHORT loc_1_34DB
loc_1_34DA:
        ret
loc_1_34DB:
        pushad
        db  0C7h, 005h, 014h, 071h, 000h, 000h, 001h, 000h, 000h, 000h
        call   sub_1_349F
        popad
        ret
loc_1_34ED:
        mov    edx, dword ptr [esp]
        jmp    SHORT loc_1_34FC
loc_1_34F2:
        xchg   edi, edx
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        xchg   edi, edx
loc_1_34FC:
        mov    ecx, dword ptr [esp + 4]
loc_1_3500:
        test   byte ptr [esp + 0ch], 1
        je     NEAR PTR loc_1_35C3
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_3535
        test   byte ptr [esp + 0ch], 2
        jne    SHORT loc_1_351D
        cmp    byte ptr [ecx + 4], 0
        jne    SHORT loc_1_3535
loc_1_351D:
        inc    byte ptr [ecx + 6]
        jmp    NEAR PTR loc_1_35F0
loc_1_3525:
        test   byte ptr [esp + 0ch], 8
        jne    NEAR PTR loc_1_35FA
        inc    byte ptr [ecx + 4]
        jmp    SHORT loc_1_3543
loc_1_3535:
        test   byte ptr [esp + 0ch], 8
        jne    NEAR PTR loc_1_35FA
        inc    byte ptr [ecx + 5]
loc_1_3543:
        or     byte ptr [esp + 0ch], 4
        sub    eax, eax
        push   ecx
        push   edx
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 0BCh, 055h, 000h, 000h
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_1_3590
        db  08Bh, 015h, 0A0h, 072h, 000h, 000h
        and    byte ptr [edi + 3], 7fh
        dec    edx
        db  089h, 015h, 0A0h, 072h, 000h, 000h
        mov    eax, dword ptr [esp + 1ch]
        or     eax, eax
        je     SHORT loc_1_357B
        dec    dword ptr [eax + 10h]
loc_1_357B:
        dec    edx
        jne    SHORT loc_1_3590
        push   eax
        db  08Dh, 005h, 0A0h, 072h, 000h, 000h
        push   eax
        call   IFSMgr_Wakeup
        lea    esp, [esp + 4]
        pop    eax
loc_1_3590:
        push   eax
        or     byte ptr [edi + 3], 40h
        push   ecx
        call   IFSMgr_Block
        lea    esp, [esp + 4]
        pop    ecx
        jecxz  loc_1_35A5
        inc    dword ptr [ecx + 10h]
loc_1_35A5:
        and    byte ptr [edi + 3], 0bfh
        pop    edi
        pop    edx
        pop    ecx
        test   byte ptr [esp + 0ch], 1
        je     SHORT loc_1_35BB
        dec    byte ptr [ecx + 5]
        jmp    NEAR PTR loc_1_3500
loc_1_35BB:
        dec    byte ptr [ecx + 4]
        jmp    NEAR PTR loc_1_3500
loc_1_35C3:
        cmp    dword ptr [ecx], edx
        je     SHORT loc_1_35F3
        cmp    dword ptr [ecx], 0
        jne    NEAR PTR loc_1_3525
        cmp    byte ptr [ecx + 6], 0
        jne    NEAR PTR loc_1_3525
        test   byte ptr [esp + 0ch], 4
        jne    SHORT loc_1_35EB
        cmp    byte ptr [ecx + 4], 0
        jne    NEAR PTR loc_1_3525
loc_1_35EB:
        mov    dword ptr [ecx], edx
loc_1_35ED:
        inc    byte ptr [ecx + 7]
loc_1_35F0:
        xor    eax, eax
loc_1_35F2:
        ret
loc_1_35F3:
        test   byte ptr [esp + 0ch], 10h
        je     SHORT loc_1_35ED
loc_1_35FA:
        mov    eax, 5
        jmp    SHORT loc_1_35F2
loc_1_3601:
        mov    ecx, dword ptr [esp + 4]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_3620
        dec    byte ptr [ecx + 7]
        jne    SHORT loc_1_3634
        mov    dword ptr [ecx], 0
loc_1_3615:
        movzx  eax, byte ptr [ecx + 4]
        or     al, byte ptr [ecx + 5]
        je     SHORT loc_1_3639
        jmp    SHORT loc_1_3627
loc_1_3620:
        dec    byte ptr [ecx + 6]
        jne    SHORT loc_1_3634
        jmp    SHORT loc_1_3615
loc_1_3627:
        push   ecx
        call   IFSMgr_Wakeup
        lea    esp, [esp + 4]
        xor    eax, eax
        ret
loc_1_3634:
        mov    eax, 1
loc_1_3639:
        ret
sub_1_363A:
        push   edi
        db  08Bh, 00Dh, 0BCh, 055h, 000h, 000h
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    edx, edi
loc_1_3649:
        cmp    word ptr [ecx + edi], 0
        je     SHORT loc_1_366F
        test   dword ptr [ecx + edi], 60000000h
        jne    SHORT loc_1_366F
        test   dword ptr [ecx + edi], 80000000h
        jne    SHORT loc_1_366F
        or     dword ptr [ecx + edi], 80000000h
        db  0FFh, 005h, 0A0h, 072h, 000h, 000h
loc_1_366F:
        db  0CDh, 020h, 013h, 001h, 001h, 000h
        cmp    edi, edx
        jne    SHORT loc_1_3649
        pop    edi
        ret
sub_1_367B:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        pop    edi
        ret
sub_1_3686:
        xor    eax, eax
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 74h]
        cmp    word ptr [edx + 3eh], ax
        jne    SHORT loc_1_3696
        ret
loc_1_3696:
        push   ecx
        call   sub_1_1D78
        add    esp, 4
        ret
loc_1_36A0:
        sub    eax, eax
        ret
IFSMgr_RegisterHeap:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        je     SHORT loc_1_36CC
        push   8
        call   IFSMgr_GetHeap
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_36CC
        db  08Bh, 00Dh, 018h, 071h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 018h, 071h, 000h, 000h
        mov    dword ptr [eax + 4], esi
loc_1_36CC:
        pop    esi
        pop    ebp
        ret
sub_1_36CF:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, edx
        mov    ecx, dword ptr [edx]
        or     ecx, ecx
        jle    SHORT loc_1_36E7
loc_1_36DD:
        add    eax, ecx
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        jg     SHORT loc_1_36DD
        or     ecx, ecx
loc_1_36E7:
        jne    SHORT loc_1_3705
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 1ch
        cmp    eax, edx
        je     SHORT loc_1_3709
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        jle    SHORT loc_1_3705
loc_1_36F9:
        cmp    eax, edx
        jae    SHORT loc_1_3707
        add    eax, ecx
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        jg     SHORT loc_1_36F9
loc_1_3705:
        cmp    eax, edx
loc_1_3707:
        jne    SHORT loc_1_370B
loc_1_3709:
        sub    eax, eax
loc_1_370B:
        pop    ebp
        ret
IFSMgr_GetHeap:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 01Ch, 071h, 000h, 000h
        db  0FFh, 005h, 01Ch, 071h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_3726
        int3
loc_1_3726:
        mov    esi, dword ptr [ebp + 8]
        add    esi, 7
        and    esi, 0fffffffch
        db  08Bh, 03Dh, 00Ch, 071h, 000h, 000h
        or     edi, edi
        je     NEAR PTR loc_1_37C4
loc_1_373D:
        lea    eax, [edi + 18h]
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [eax]
        cmp    eax, esi
        jb     SHORT loc_1_37B9
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_37B9
        mov    dword ptr [ebp - 4], eax
loc_1_3754:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ebx, eax
        neg    ebx
loc_1_3760:
        sub    ecx, eax
        mov    eax, dword ptr [ecx]
        or     eax, eax
        jl     SHORT loc_1_3760
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        sub    edx, ecx
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [edi + 14h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_1_3784
        cmp    ecx, eax
        jbe    SHORT loc_1_3784
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 14h], eax
loc_1_3784:
        lea    ecx, [esi + edx]
        or     ecx, ecx
        jle    SHORT loc_1_37FF
        mov    edx, esi
        sub    edx, ecx
        push   edi
        mov    eax, dword ptr [ebp - 4]
        add    eax, edx
        push   eax
        call   sub_1_36CF
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [edi + 14h], eax
        jne    SHORT loc_1_37AD
        mov    dword ptr [ebp - 4], 0
loc_1_37AD:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_3754
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], bx
loc_1_37B9:
        mov    edi, dword ptr [edi + 8]
        or     edi, edi
        jne    NEAR PTR loc_1_373D
loc_1_37C4:
        push   esi
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_3965
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_3880
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edi, ecx
        mov    eax, dword ptr [edi + 14h]
        add    edi, 14h
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [eax]
        add    edx, esi
        or     edx, edx
        jg     NEAR PTR loc_1_3875
        mov    dword ptr [eax], esi
        je     SHORT loc_1_3848
        add    eax, esi
        mov    dword ptr [edi], eax
        mov    dword ptr [eax], edx
        jmp    SHORT loc_1_3854
loc_1_37FF:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], esi
        add    eax, esi
        or     ecx, ecx
        je     SHORT loc_1_380E
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_1_3818
loc_1_380E:
        push   edi
        push   eax
        call   sub_1_36CF
        add    esp, 8
loc_1_3818:
        mov    dword ptr [edi + 14h], eax
        or     eax, eax
        jne    SHORT loc_1_3827
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
loc_1_3827:
        inc    word ptr [edi + 1ah]
        push   dword ptr [ebp - 4]
        call   sub_1_3489
        add    esp, 4
        db  083h, 02Dh, 01Ch, 071h, 000h, 000h, 001h
        je     SHORT loc_1_3840
        int3
loc_1_3840:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        jmp    SHORT loc_1_388C
loc_1_3848:
        mov    dword ptr [edi], 0
        mov    word ptr [ecx + 18h], 0
loc_1_3854:
        inc    word ptr [ecx + 1ah]
        push   dword ptr [ebp - 4]
        call   sub_1_3489
        add    esp, 4
        db  083h, 02Dh, 01Ch, 071h, 000h, 000h, 001h
        je     SHORT loc_1_386D
        int3
loc_1_386D:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        jmp    SHORT loc_1_388C
loc_1_3875:
        db  083h, 02Dh, 01Ch, 071h, 000h, 000h, 001h
        je     SHORT loc_1_388A
        jmp    SHORT loc_1_3889
loc_1_3880:
        db  083h, 02Dh, 01Ch, 071h, 000h, 000h, 001h
        je     SHORT loc_1_388A
loc_1_3889:
        int3
loc_1_388A:
        sub    eax, eax
loc_1_388C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
IFSMgr_RetHeap:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_1_38EA
        db  0A1h, 01Ch, 071h, 000h, 000h
        db  0FFh, 005h, 01Ch, 071h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_38B2
        int3
loc_1_38B2:
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 4
        mov    dword ptr [ebp - 4], eax
        db  08Bh, 00Dh, 00Ch, 071h, 000h, 000h
        sub    edx, edx
        cmp    ecx, edx
        je     SHORT loc_1_38E0
loc_1_38C7:
        lea    eax, [ecx + 1ch]
        cmp    eax, dword ptr [ebp - 4]
        ja     SHORT loc_1_38D7
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 10h], eax
        ja     SHORT loc_1_38F1
loc_1_38D7:
        mov    edx, ecx
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        jne    SHORT loc_1_38C7
loc_1_38E0:
        db  083h, 02Dh, 01Ch, 071h, 000h, 000h, 001h
        je     SHORT loc_1_38EA
        int3
loc_1_38EA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_38F1:
        mov    esi, dword ptr [ebp - 4]
        mov    eax, esi
        mov    ebx, dword ptr [eax]
        mov    eax, ebx
        neg    eax
loc_1_38FC:
        sub    esi, eax
        mov    eax, dword ptr [esi]
        or     eax, eax
        jl     SHORT loc_1_38FC
        mov    edi, esi
        sub    edi, dword ptr [ebp - 4]
        movzx  eax, word ptr [ecx + 18h]
        cmp    eax, edi
        jge    SHORT loc_1_3915
        mov    word ptr [ecx + 18h], bx
loc_1_3915:
        mov    eax, dword ptr [ecx + 14h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_1_3923
        cmp    esi, eax
        jbe    SHORT loc_1_3923
        jmp    SHORT loc_1_3927
loc_1_3923:
        or     eax, eax
        jne    SHORT loc_1_392D
loc_1_3927:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
loc_1_392D:
        mov    eax, dword ptr [ebp - 4]
        neg    edi
        mov    dword ptr [eax], edi
        mov    ax, word ptr [ecx + 1ah]
        dec    ax
        mov    word ptr [ecx + 1ah], ax
        jne    SHORT loc_1_38E0
        mov    eax, dword ptr [ecx + 8]
        db  08Bh, 035h, 010h, 071h, 000h, 000h
        mov    dword ptr [ecx + 8], esi
        db  089h, 00Dh, 010h, 071h, 000h, 000h
        or     edx, edx
        jne    SHORT loc_1_395D
        db  0A3h, 00Ch, 071h, 000h, 000h
        jmp    SHORT loc_1_38E0
loc_1_395D:
        mov    dword ptr [edx + 8], eax
        jmp    NEAR PTR loc_1_38E0
sub_1_3965:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        db  08Bh, 03Dh, 018h, 071h, 000h, 000h
        cmp    edi, esi
        je     SHORT loc_1_3985
loc_1_397A:
        call   dword ptr [edi + 4]
        or     esi, eax
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_397A
loc_1_3985:
        or     esi, esi
        je     SHORT loc_1_3990
        mov    eax, esi
        jmp    NEAR PTR loc_1_3A95
loc_1_3990:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 20h
        mov    dword ptr [ebp - 4], ecx
        db  0A1h, 010h, 071h, 000h, 000h
        sub    edx, edx
        cmp    eax, edx
        je     SHORT loc_1_39CF
loc_1_39A4:
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0ch], ecx
        jae    SHORT loc_1_39B7
        mov    edx, eax
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    SHORT loc_1_39A4
        jmp    SHORT loc_1_39CF
loc_1_39B7:
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], ecx
        or     edx, edx
        mov    ecx, dword ptr [eax + 8]
        jne    SHORT loc_1_39CC
        db  089h, 00Dh, 010h, 071h, 000h, 000h
        jmp    SHORT loc_1_39CF
loc_1_39CC:
        mov    dword ptr [edx + 8], ecx
loc_1_39CF:
        or     eax, eax
        jne    SHORT loc_1_3A29
        cmp    dword ptr [ebp - 4], 1000h
        ja     SHORT loc_1_39E3
        sub    eax, eax
        jmp    NEAR PTR loc_1_3A95
loc_1_39E3:
        db  083h, 02Dh, 01Ch, 071h, 000h, 000h, 001h
        je     SHORT loc_1_39ED
        int3
loc_1_39ED:
        mov    esi, dword ptr [ebp - 4]
        add    esi, 0fffh
        mov    eax, esi
        shr    eax, 0ch
        push   eax
        call   sub_1_3451
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_3A15
        db  0FFh, 005h, 01Ch, 071h, 000h, 000h
        sub    eax, eax
        jmp    NEAR PTR loc_1_3A95
loc_1_3A15:
        db  0FFh, 005h, 01Ch, 071h, 000h, 000h
        and    esi, 0fffff000h
        mov    dword ptr [ebp - 4], esi
        mov    ecx, esi
        mov    dword ptr [eax + 0ch], ecx
loc_1_3A29:
        sub    dword ptr [ebp - 4], 20h
        mov    dword ptr [eax + 4], 48534649h
        lea    edx, [eax + 1ch]
        mov    ecx, dword ptr [ebp - 4]
        neg    ecx
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebp - 4]
        lea    ecx, [eax + ecx + 1ch]
        mov    dword ptr [eax + 10h], ecx
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax + 14h], edx
        mov    cx, word ptr [ebp - 4]
        sub    esi, esi
        mov    word ptr [eax + 18h], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + 1ah], 0
        mov    dword ptr [ecx], eax
        db  08Bh, 015h, 00Ch, 071h, 000h, 000h
        cmp    edx, esi
        je     SHORT loc_1_3A79
loc_1_3A70:
        mov    esi, edx
        mov    edx, dword ptr [edx + 8]
        or     edx, edx
        jne    SHORT loc_1_3A70
loc_1_3A79:
        or     esi, esi
        je     SHORT loc_1_3A82
        mov    dword ptr [esi + 8], eax
        jmp    SHORT loc_1_3A87
loc_1_3A82:
        db  0A3h, 00Ch, 071h, 000h, 000h
loc_1_3A87:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 8], 0
        mov    eax, 1
loc_1_3A95:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_3A9C:
        push   esi
        db  083h, 03Dh, 010h, 071h, 000h, 000h, 000h
        je     SHORT loc_1_3ACF
        db  08Bh, 00Dh, 010h, 071h, 000h, 000h
        mov    eax, dword ptr [ecx + 8]
        add    ecx, 8
        or     eax, eax
        mov    dword ptr [ecx], 0
        je     SHORT loc_1_3ACF
loc_1_3ABC:
        mov    esi, dword ptr [eax + 8]
        push   dword ptr [eax]
        call   sub_1_3478
        add    esp, 4
        mov    eax, esi
        or     esi, esi
        jne    SHORT loc_1_3ABC
loc_1_3ACF:
        push   7530h
        db  068h, 020h, 071h, 000h, 000h
        call   IFSMgr_SchedEvent
        add    esp, 8
        pop    esi
        ret
        db  000h
loc_1_3AE4:
        aad    0d6h
        xlatb
        fcomp  st(5)
        db  0DEh, 0DFh, 0DCh, 04Eh, 043h, 056h, 041h, 05Ch, 04Fh
sub_1_3AF3:
        db  0B8h, 01Ah, 000h, 000h, 02Bh
        cmp    eax, dword ptr [eax]
        db  000h, 045h, 03Bh
        add    byte ptr [eax], al
        inc    ebp
        cmp    eax, dword ptr [eax]
        db  000h, 045h, 03Bh
        add    byte ptr [eax], al
        inc    edi
        cmp    eax, dword ptr [eax]
        db  000h, 047h, 03Bh
        add    byte ptr [eax], al
        inc    ebp
        cmp    eax, dword ptr [eax]
        db  000h, 045h, 03Bh
        add    byte ptr [eax], al
        sub    edi, dword ptr [ebx]
        add    byte ptr [eax], al
        sub    edi, dword ptr [ebx]
        add    byte ptr [eax], al
        sub    edi, dword ptr [ebx]
        add    byte ptr [eax], al
        sub    edi, dword ptr [ebx]
        add    byte ptr [eax], al
        inc    ebp
        cmp    eax, dword ptr [eax]
        add    byte ptr [eax - 75h], dl
        inc    ebp
        sbb    al, 25h
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        or     ecx, eax
        pop    eax
        db  0E8h, 053h, 0E1h, 0FEh, 0FFh
        jae    SHORT loc_1_3B44
        mov    word ptr [ebp + 1ch], 3
loc_1_3B44:
        ret
loc_1_3B45:
        clc
        ret
loc_1_3B47:
        movzx  edx, byte ptr [ebp + 1ch]
        db  0E8h, 097h, 0DAh, 0FEh, 0FFh
        ret
IFSMgr_Ring0_FileIO:
        push   ebp
        push   edi
        push   esi
        sub    esp, 30h
        mov    edi, esp
        push   ecx
        push   eax
        mov    ecx, 0ch
        xor    eax, eax
        cld
        rep stosd
        sub    edi, 30h
        pop    eax
        pop    ecx
        mov    ebp, edi
        mov    edi, dword ptr [ebp + 34h]
        mov    dword ptr [ebp], edi
        mov    dword ptr [ebp + 1ch], eax
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp + 18h], ecx
        mov    dword ptr [ebp + 14h], edx
        mov    dword ptr [ebp + 4], esi
        pushfd
        pop    dword ptr [ebp + 2ch]
        db  0BFh, 0E4h, 03Ah, 000h, 000h
        xchg   al, ah
        mov    ecx, 0fh
        repne scasb
        jne    SHORT loc_1_3BF3
        mov    edi, ecx
        sub    edi, 0fh
        neg    edi
        dec    edi
        movzx  ecx, al
        or     ecx, 50000000h
        db  08Bh, 01Dh, 0B0h, 055h, 000h, 000h
        mov    eax, 21h
        cmp    cl, 4fh
        je     SHORT loc_1_3BC4
        xor    edx, edx
        db  0FFh, 014h, 0BDh, 0F3h, 03Ah, 000h, 000h
        jb     SHORT loc_1_3BF9
loc_1_3BC4:
        mov    esi, 0ffffffffh
        db  0A1h, 0B4h, 055h, 000h, 000h
        mov    edx, 0ffffffffh
        push   ebp
        call   sub_1_08E1
        pop    ebp
loc_1_3BDA:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ebx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        push   dword ptr [ebp + 2ch]
        pop    esi
        add    esp, 30h
        push   esi
        popfd
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_1_3BF3:
        mov    word ptr [ebp + 1ch], 1
loc_1_3BF9:
        or     dword ptr [ebp + 2ch], 1
        jmp    SHORT loc_1_3BDA
IFSMgr_Ring0GetDriveInfo:
        sub    eax, eax
        cmp    edx, 20h
        jae    SHORT loc_1_3C44
        db  0F6h, 082h, 08Ch, 000h, 000h, 000h, 001h
        stc
        je     SHORT loc_1_3C2A
        db  0F6h, 082h, 08Ch, 000h, 000h, 000h, 004h
        je     SHORT loc_1_3C43
        or     eax, 1
        db  0F6h, 082h, 08Ch, 000h, 000h, 000h, 020h
        je     SHORT loc_1_3C43
        or     eax, 2
        jmp    SHORT loc_1_3C43
loc_1_3C2A:
        mov    ecx, edx
        inc    ecx
        db  08Bh, 00Ch, 08Dh, 0BCh, 065h, 000h, 000h
        jecxz  loc_1_3C44
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_3C44
        test   byte ptr [ecx + 1bh], 8
        je     SHORT loc_1_3C44
        or     eax, 1
loc_1_3C43:
        ret
loc_1_3C44:
        stc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_3C50:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3CCE
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_3C7C
        push   0
        push   esi
        db  0E8h, 034h, 035h, 0FFh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_3C7C
        push   esi
        call   sub_1_3D53
        add    esp, 4
loc_1_3C7C:
        pop    esi
        pop    ebp
        ret
sub_1_3C7F:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        call   sub_1_5364
        mov    dword ptr [esi + 90h], eax
        db  003h, 005h, 0B4h, 055h, 000h, 000h
        mov    dword ptr [esi + 94h], eax
        mov    eax, dword ptr [esi + 90h]
        mov    ecx, dword ptr [eax + 0ch]
        db  0A1h, 09Ch, 065h, 000h, 000h
        movzx  eax, word ptr [eax + 10h]
        add    eax, ecx
        shl    ecx, 10h
        or     eax, ecx
        mov    dword ptr [esi + 8], eax
        db  0A1h, 09Ch, 065h, 000h, 000h
        movzx  eax, word ptr [eax + 10h]
        shl    eax, 4
        mov    dword ptr [esi + 80h], eax
        pop    esi
        pop    ebp
        ret
sub_1_3CCE:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 24h], 0
        mov    al, byte ptr [esi + 0a8h]
        mov    byte ptr [esi + 4], al
        or     byte ptr [esi + 8ah], 40h
        mov    ax, word ptr [esi + 0ach]
        cmp    word ptr [esi + 0b4h], 0d501h
        mov    word ptr [esi + 18h], ax
        jne    SHORT loc_1_3D0B
        push   esi
        call   sub_1_3C7F
        add    esp, 4
loc_1_3D0B:
        mov    ax, word ptr [esi + 18h]
        test   al, 0ech
        jne    SHORT loc_1_3D45
        mov    cx, ax
        and    cl, 3
        cmp    cl, 2
        ja     SHORT loc_1_3D45
        or     ax, ax
        je     SHORT loc_1_3D45
        mov    al, byte ptr [esi + 4]
        mov    cl, al
        and    cl, 70h
        cmp    cl, 40h
        ja     SHORT loc_1_3D38
        test   al, 8
        jne    SHORT loc_1_3D38
        sub    eax, eax
        jmp    SHORT loc_1_3D50
loc_1_3D38:
        mov    word ptr [esi + 1ah], 0ch
        mov    eax, 0ch
        jmp    SHORT loc_1_3D50
loc_1_3D45:
        mov    word ptr [esi + 1ah], 1
        mov    eax, 1
loc_1_3D50:
        pop    esi
        pop    ebp
        ret
sub_1_3D53:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 18h], 10h
        je     SHORT loc_1_3D84
        mov    ecx, dword ptr [esi + 7ch]
        mov    eax, 0d0h
        test   byte ptr [ecx + 1bh], 8
        je     SHORT loc_1_3D75
        mov    eax, 58h
loc_1_3D75:
        test   dword ptr [esi], eax
        je     SHORT loc_1_3D84
        mov    word ptr [esi + 1ah], 5
        jmp    NEAR PTR loc_1_3E1E
loc_1_3D84:
        push   18h
        push   esi
        call   sub_1_1F50
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_1_3E1E
        mov    ecx, dword ptr [esi + 74h]
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [ecx + 22h], ax
        mov    bx, word ptr [esi + 18h]
        and    bx, 0ff7fh
        mov    word ptr [esi + 18h], bx
        mov    edi, dword ptr [esi + 14h]
        mov    dword ptr [esi + 14h], 0
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   24h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 18h]
        call   sub_1_1B90
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_3DE7
        push   esi
        call   sub_1_11A1
        add    esp, 4
        push   esi
        call   sub_1_2297
        add    esp, 4
        jmp    SHORT loc_1_3E1B
loc_1_3DE7:
        mov    eax, dword ptr [esi + 74h]
        mov    word ptr [eax + 20h], 1
        mov    ecx, dword ptr [esi + 74h]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ecx + 0ch], eax
        mov    ax, word ptr [esi + 18h]
        and    ax, 0ffh
        mov    word ptr [esi + 0b0h], ax
        mov    eax, dword ptr [esi + 74h]
        mov    dword ptr [esi + 0b4h], eax
        test   bh, 10h
        je     SHORT loc_1_3E1B
        db  0A3h, 00Ch, 072h, 000h, 000h
loc_1_3E1B:
        mov    dword ptr [esi + 14h], edi
loc_1_3E1E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_3E23:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0b4h]
        shr    ecx, 10h
        mov    ebx, dword ptr [esi + 0a8h]
        mov    dword ptr [esi + 74h], ebx
        mov    ax, word ptr [ebx + 22h]
        and    ax, 0ff00h
        cmp    word ptr [esi + 0b4h], 0d610h
        mov    word ptr [esi + 18h], ax
        jb     SHORT loc_1_3E6E
        mov    eax, dword ptr [esi + 98h]
        mov    dword ptr [esi + 10h], eax
        sub    byte ptr [esi + 0b4h], 10h
        or     word ptr [esi + 18h], 2000h
loc_1_3E6E:
        test   ch, 80h
        je     SHORT loc_1_3E7F
        or     word ptr [esi + 18h], 8000h
        mov    ax, word ptr [ebx + 32h]
        jmp    SHORT loc_1_3E83
loc_1_3E7F:
        mov    ax, word ptr [ebx + 30h]
loc_1_3E83:
        mov    word ptr [esi + 6], ax
        push   esi
        call   sub_1_242B
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_1_3FF7
        mov    eax, dword ptr [esi + 0ach]
        mov    dword ptr [esi + 24h], eax
        cmp    word ptr [esi + 0b4h], 0d601h
        jbe    SHORT loc_1_3EBC
        push   esi
        call   sub_1_3C7F
        add    esp, 4
        sub    byte ptr [esi + 0b4h], 2
loc_1_3EBC:
        test   byte ptr [esi + 19h], 10h
        jne    SHORT loc_1_3ECB
        push   esi
        call   sub_1_3686
        add    esp, 4
loc_1_3ECB:
        cmp    byte ptr [esi + 0b4h], 0
        jne    SHORT loc_1_3EF6
        cmp    dword ptr [esi], 0
        jbe    SHORT loc_1_3EEF
        push   0
        push   esi
        push   0
        push   dword ptr [ebx]
        call   sub_1_1B90
        add    esp, 10h
        mov    edi, eax
        jmp    NEAR PTR loc_1_3F89
loc_1_3EEF:
        sub    edi, edi
        jmp    NEAR PTR loc_1_3F89
loc_1_3EF6:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_3F6A
        test   byte ptr [ebx + 23h], 10h
        je     SHORT loc_1_3F6A
        mov    eax, dword ptr [esi + 7ch]
        mov    cl, byte ptr [eax + 30h]
        or     cl, cl
        je     SHORT loc_1_3F6A
        mov    dword ptr [ebp - 4], eax
        test   byte ptr [eax + 32h], 20h
        je     SHORT loc_1_3F5F
        cmp    cl, 4
        je     SHORT loc_1_3F5F
        test   byte ptr [ebx + 1eh], 4
        je     SHORT loc_1_3F32
        push   1
        push   esi
        call   sub_1_433B
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_1_3FF7
loc_1_3F32:
        push   0
        push   esi
        push   1
        push   dword ptr [ebx + 4]
        call   sub_1_1B90
        add    esp, 10h
        mov    edi, eax
        or     dword ptr [ebx + 38h], 0c0000000h
        and    word ptr [ebx + 1ch], 0ffbfh
        or     edi, edi
        jne    SHORT loc_1_3F8B
        mov    eax, dword ptr [ebp - 4]
        or     word ptr [eax + 32h], 10h
        jmp    SHORT loc_1_3F89
loc_1_3F5F:
        mov    word ptr [esi + 1ah], 28h
        jmp    NEAR PTR loc_1_3FF7
loc_1_3F6A:
        push   0
        push   esi
        push   1
        push   dword ptr [ebx + 4]
        call   sub_1_1B90
        add    esp, 10h
        mov    edi, eax
        or     dword ptr [ebx + 38h], 0c0000000h
        and    word ptr [ebx + 1ch], 0ffbfh
loc_1_3F89:
        or     edi, edi
loc_1_3F8B:
        jge    SHORT loc_1_3F98
        push   esi
        call   sub_1_2E50
        add    esp, 4
        mov    edi, eax
loc_1_3F98:
        test   byte ptr [esi + 19h], 90h
        jne    SHORT loc_1_3FC9
        cmp    edi, 27h
        jne    SHORT loc_1_3FBB
        mov    eax, dword ptr [esi + 7ch]
        test   byte ptr [eax + 1bh], 10h
        je     SHORT loc_1_3FC3
        movzx  eax, word ptr [eax + 14h]
        push   eax
        db  0E8h, 012h, 013h, 0FFh, 0FFh
        add    esp, 4
        jmp    SHORT loc_1_3FC3
loc_1_3FBB:
        cmp    edi, 0eah
        jne    SHORT loc_1_3FC9
loc_1_3FC3:
        mov    word ptr [esi + 1ah], 0
loc_1_3FC9:
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_1_3FF7
        test   byte ptr [esi + 19h], 90h
        jne    SHORT loc_1_3FDC
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebx + 18h], eax
loc_1_3FDC:
        mov    eax, dword ptr [esi]
        mov    dword ptr [esi + 0b4h], eax
        cmp    edi, 0eah
        jne    SHORT loc_1_3FF7
        or     eax, 80000000h
        mov    dword ptr [esi + 0b4h], eax
loc_1_3FF7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_3FFE:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0a8h]
        mov    dword ptr [esi + 74h], eax
        mov    cx, word ptr [eax + 22h]
        and    cx, 0ff00h
        mov    word ptr [esi + 18h], cx
        mov    ax, word ptr [eax + 30h]
        mov    word ptr [esi + 6], ax
        push   esi
        call   sub_1_242B
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_4068
        mov    byte ptr [esi + 4], 2
        mov    dword ptr [esi + 24h], 0
        mov    edi, dword ptr [esi + 74h]
        push   esi
        call   sub_1_3686
        add    esp, 4
        push   0
        push   esi
        mov    eax, dword ptr [edi + 8]
        push   0ah
        push   dword ptr [eax + 4]
        call   sub_1_1B90
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_4068
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [esi + 0b4h], eax
loc_1_4068:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_406C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0a8h]
        mov    dword ptr [esi + 74h], eax
        mov    ax, word ptr [eax + 22h]
        and    ax, 0ff00h
        mov    word ptr [esi + 18h], ax
        mov    eax, dword ptr [esi + 74h]
        mov    ax, word ptr [eax + 30h]
        mov    word ptr [esi + 6], ax
        mov    eax, dword ptr [esi + 74h]
        mov    ax, word ptr [eax + 32h]
        or     ax, ax
        je     SHORT loc_1_40A4
        mov    word ptr [esi + 6], ax
loc_1_40A4:
        push   esi
        call   sub_1_242B
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_1_415D
        push   esi
        call   sub_1_11A1
        add    esp, 4
        mov    ebx, 2
        mov    ecx, dword ptr [esi + 74h]
        mov    ax, word ptr [ecx + 20h]
        add    ecx, 20h
        cmp    ax, 1
        je     SHORT loc_1_40DD
        dec    ax
        mov    word ptr [ecx], ax
        mov    ebx, 1
loc_1_40DD:
        mov    byte ptr [esi + 4], bl
        push   ebx
        push   esi
        call   sub_1_1CF9
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_4104
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0bh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 8]
        call   sub_1_1B90
        add    esp, 10h
loc_1_4104:
        push   esi
        call   sub_1_1D51
        add    esp, 4
        or     ebx, ebx
        je     SHORT loc_1_4120
        push   1
        push   0bh
        push   -1
        push   esi
        call   sub_1_4B67
        add    esp, 10h
loc_1_4120:
        mov    eax, dword ptr [esi + 74h]
        add    eax, 32h
        cmp    word ptr [eax], 0
        je     SHORT loc_1_413F
        mov    word ptr [eax], 0
        mov    eax, dword ptr [esi + 74h]
        and    byte ptr [eax + 1fh], 0dfh
        mov    eax, dword ptr [esi + 74h]
        and    byte ptr [eax + 1eh], 0fbh
loc_1_413F:
        cmp    ebx, 2
        jne    SHORT loc_1_415D
        push   esi
        call   sub_1_2297
        add    esp, 4
        test   byte ptr [esi + 19h], 10h
        je     SHORT loc_1_415D
        db  0C7h, 005h, 00Ch, 072h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_415D:
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_4161:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    edi, dword ptr [esi + 0a8h]
        mov    dword ptr [esi + 74h], edi
        call   sub_1_242B
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_41CB
        push   esi
        call   sub_1_3C7F
        add    esp, 4
        cmp    byte ptr [esi + 0b4h], 12h
        mov    word ptr [esi + 1ah], 5
        jne    SHORT loc_1_41CB
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 7ch], eax
        mov    eax, dword ptr [edi + 10h]
        push   0
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 20h], eax
        lea    eax, [esi + 98h]
        mov    dword ptr [esi + 28h], eax
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   2ah
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 30h]
        call   sub_1_1B90
        add    esp, 10h
loc_1_41CB:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h
sub_1_41D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 32h], 80h
        jne    SHORT loc_1_41EE
        lea    eax, [esi + 38h]
        push   eax
        mov    word ptr [eax], 0
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_41EE:
        inc    word ptr [esi + 38h]
        add    esi, 38h
        push   esi
        call   IFSMgr_Block
        add    esp, 4
        pop    esi
        pop    ebp
        ret
sub_1_4201:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 7ch]
        call   sub_1_5370
        mov    ebx, eax
        test   byte ptr [esi + 32h], 0c0h
        je     SHORT loc_1_4229
loc_1_421A:
        push   esi
        call   sub_1_41D0
        add    esp, 4
        test   byte ptr [esi + 32h], 0c0h
        jne    SHORT loc_1_421A
loc_1_4229:
        mov    dword ptr [esi + 34h], ebx
        or     word ptr [esi + 32h], 40h
        call   sub_1_363A
        db  083h, 03Dh, 0A0h, 072h, 000h, 000h, 001h
        jle    SHORT loc_1_4259
        mov    ebx, 1
loc_1_4244:
        db  068h, 0A0h, 072h, 000h, 000h
        call   IFSMgr_Block
        add    esp, 4
        db  039h, 01Dh, 0A0h, 072h, 000h, 000h
        jg     SHORT loc_1_4244
loc_1_4259:
        movzx  ecx, byte ptr [esi + 19h]
        movzx  eax, word ptr [esi + 16h]
        add    ecx, eax
        movzx  eax, word ptr [esi + 3ah]
        add    ecx, eax
        movzx  eax, word ptr [esi + 3ch]
        add    ecx, eax
        cmp    ecx, dword ptr [esi + 10h]
        jae    SHORT loc_1_429E
        lea    ebx, [esi + 38h]
loc_1_4277:
        inc    word ptr [ebx]
        push   ebx
        call   IFSMgr_Block
        add    esp, 4
        movzx  eax, word ptr [esi + 16h]
        movzx  ecx, byte ptr [esi + 19h]
        add    ecx, eax
        movzx  eax, word ptr [esi + 3ah]
        add    ecx, eax
        movzx  eax, word ptr [esi + 3ch]
        add    ecx, eax
        cmp    ecx, dword ptr [esi + 10h]
        jb     SHORT loc_1_4277
loc_1_429E:
        and    word ptr [esi + 32h], 0ffbfh
        push   edi
        call   sub_1_2D7B
        add    esp, 4
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 34h], eax
        or     word ptr [esi + 32h], 80h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_42BE:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    esi, dword ptr [eax + 7ch]
        and    word ptr [esi + 32h], 0ff7fh
        mov    dword ptr [esi + 34h], 0
        call   sub_1_28FE
        add    esi, 38h
        add    esp, 4
        cmp    word ptr [esi], 0
        je     SHORT loc_1_42F5
        mov    word ptr [esi], 0
        push   esi
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_42F5:
        pop    esi
        pop    ebp
        ret
sub_1_42F8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cli
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        jne    SHORT loc_1_4326
        mov    eax, dword ptr [ebp + 8]
        mov    esi, eax
        mov    edi, dword ptr [eax + 24h]
        movsd
        movsd
        movsd
        db  0C7h, 000h, 062h, 049h, 000h, 000h
        db  0C7h, 040h, 004h, 0A4h, 049h, 000h, 000h
        db  0C7h, 040h, 008h, 074h, 072h, 000h, 000h
        jmp    SHORT loc_1_4336
loc_1_4326:
        cmp    eax, 1
        jne    SHORT loc_1_4336
        mov    eax, dword ptr [ebp + 8]
        mov    edi, eax
        mov    esi, dword ptr [eax + 24h]
        movsd
        movsd
        movsd
loc_1_4336:
        sti
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_1_433B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 74h]
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebp - 1], al
        mov    esi, dword ptr [edi + 0ch]
        mov    ax, word ptr [edi + 18h]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebx + 24h]
        add    eax, 14h
        mov    dword ptr [edi + 0ch], eax
        mov    byte ptr [edi + 4], 4
        mov    dword ptr [edi], 0
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_4399
        test   byte ptr [ebx + 23h], 10h
        je     SHORT loc_1_4387
        mov    word ptr [edi + 18h], 1000h
        jmp    SHORT loc_1_4399
loc_1_4387:
        test   byte ptr [ebx + 1fh], 20h
        mov    word ptr [edi + 18h], 8000h
        jne    SHORT loc_1_4399
        mov    word ptr [edi + 18h], 0
loc_1_4399:
        cmp    dword ptr [ebx + 24h], 0
        je     SHORT loc_1_43C1
loc_1_439F:
        mov    eax, dword ptr [ebx + 24h]
        test   byte ptr [eax + 10h], 4
        je     SHORT loc_1_43C1
        inc    word ptr [eax + 12h]
        mov    eax, dword ptr [ebx + 24h]
        add    eax, 12h
        push   eax
        call   IFSMgr_Block
        add    esp, 4
        cmp    dword ptr [ebx + 24h], 0
        jne    SHORT loc_1_439F
loc_1_43C1:
        mov    eax, dword ptr [ebx + 24h]
        or     eax, eax
        je     NEAR PTR loc_1_444E
        or     word ptr [eax + 10h], 4
        test   byte ptr [ebx + 23h], 10h
        je     SHORT loc_1_43FA
        mov    eax, dword ptr [ebx + 24h]
        test   byte ptr [eax + 10h], 8
        je     SHORT loc_1_43E3
        or     dword ptr [edi], 1
loc_1_43E3:
        mov    ecx, dword ptr [ebx + 24h]
        mov    ax, word ptr [ecx + 10h]
        add    ecx, 10h
        test   al, 10h
        je     SHORT loc_1_43FD
        and    ax, 0ffe7h
        mov    word ptr [ecx], ax
        jmp    SHORT loc_1_43FD
loc_1_43FA:
        or     dword ptr [edi], 1
loc_1_43FD:
        push   7
        push   edi
        call   sub_1_478C
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_4425
        test   byte ptr [ebx + 23h], 10h
        jne    SHORT loc_1_4421
        test   byte ptr [ebx + 1fh], 60h
        je     SHORT loc_1_4425
        mov    eax, dword ptr [edi + 7ch]
        cmp    byte ptr [eax + 30h], 4
        jae    SHORT loc_1_4425
loc_1_4421:
        and    byte ptr [ebx + 1eh], 0fbh
loc_1_4425:
        mov    eax, dword ptr [ebx + 24h]
        and    word ptr [eax + 10h], 0fffbh
        mov    eax, dword ptr [ebx + 24h]
        add    eax, 12h
        cmp    word ptr [eax], 0
        je     SHORT loc_1_444E
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebx + 24h]
        add    eax, 12h
        push   eax
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_444E:
        mov    dword ptr [edi + 0ch], esi
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [edi + 4], al
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [edi + 18h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edi], eax
        movsx  eax, word ptr [edi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_446F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 7ch]
        add    esi, 28h
        mov    eax, dword ptr [edi + 8]
        cmp    dword ptr [esi], eax
        jne    SHORT loc_1_448F
        mov    eax, 1
        jmp    NEAR PTR loc_1_4515
loc_1_448F:
        test   byte ptr [edi + 8bh], 10h
        je     SHORT loc_1_449F
        cmp    eax, -1
        jne    SHORT loc_1_449F
        jmp    SHORT loc_1_4513
loc_1_449F:
        mov    ecx, dword ptr [edi + 90h]
        test   byte ptr [ecx], 20h
        je     SHORT loc_1_44AC
        jmp    SHORT loc_1_4513
loc_1_44AC:
        shr    eax, 10h
        movzx  ebx, ax
        push   ebx
        db  0E8h, 0ABh, 0C7h, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_44C1
        jmp    SHORT loc_1_4513
loc_1_44C1:
        db  08Bh, 00Dh, 0B0h, 055h, 000h, 000h
        cmp    dword ptr [ecx + 0ch], ebx
        jne    SHORT loc_1_44CE
        jmp    SHORT loc_1_4513
loc_1_44CE:
        mov    edx, dword ptr [esi]
        mov    ecx, edx
        shr    ecx, 10h
        cmp    ecx, ebx
        je     SHORT loc_1_44DB
        jmp    SHORT loc_1_4513
loc_1_44DB:
        and    edx, 0ffffh
        shl    edx, 4
        shl    ecx, 4
        sub    edx, ecx
        mov    ecx, dword ptr [edi + 80h]
        add    ecx, eax
        add    edx, eax
        movzx  esi, word ptr [ecx + 16h]
        shl    esi, 4
        add    esi, eax
        cmp    esi, ecx
        je     SHORT loc_1_4513
loc_1_4500:
        cmp    edx, esi
        je     SHORT loc_1_451A
        mov    ecx, esi
        movzx  esi, word ptr [esi + 16h]
        shl    esi, 4
        add    esi, eax
        cmp    esi, ecx
        jne    SHORT loc_1_4500
loc_1_4513:
        sub    eax, eax
loc_1_4515:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_451A:
        mov    eax, 1
        jmp    SHORT loc_1_4515
sub_1_4521:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        je     SHORT loc_1_4570
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, 4
loc_1_4536:
        or     edi, edi
        mov    eax, dword ptr [esi + 24h]
        je     SHORT loc_1_4543
        or     word ptr [eax + 10h], bx
        jmp    SHORT loc_1_4569
loc_1_4543:
        and    word ptr [eax + 10h], 0fffbh
        mov    eax, dword ptr [esi + 24h]
        add    eax, 12h
        cmp    word ptr [eax], 0
        je     SHORT loc_1_4569
        mov    word ptr [eax], 0
        mov    eax, dword ptr [esi + 24h]
        add    eax, 12h
        push   eax
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_4569:
        mov    esi, dword ptr [esi + 2ch]
        or     esi, esi
        jne    SHORT loc_1_4536
loc_1_4570:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_4575:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_1_45F3
        mov    edi, 4
loc_1_4589:
        mov    eax, dword ptr [esi + 24h]
        test   word ptr [eax + 10h], di
        je     SHORT loc_1_45F3
        db  0E8h, 0A9h, 020h, 000h, 000h
        db  08Bh, 01Dh, 0BCh, 055h, 000h, 000h
        add    ebx, eax
        mov    eax, dword ptr [ebx]
        test   eax, 80000000h
        je     SHORT loc_1_45CB
        and    eax, 7fffffffh
        mov    dword ptr [ebx], eax
        db  0FFh, 00Dh, 0A0h, 072h, 000h, 000h
        db  083h, 03Dh, 0A0h, 072h, 000h, 000h, 001h
        jne    SHORT loc_1_45CB
        db  068h, 0A0h, 072h, 000h, 000h
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_45CB:
        mov    eax, dword ptr [esi + 24h]
        inc    word ptr [eax + 12h]
        or     dword ptr [ebx], 40000000h
        mov    eax, dword ptr [esi + 24h]
        add    eax, 12h
        push   eax
        call   IFSMgr_Block
        add    esp, 4
        and    dword ptr [ebx], 0bfffffffh
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_1_4589
loc_1_45F3:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_45F8:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_1_25A0
        add    esp, 4
        call   sub_1_5370
        db  0A3h, 09Ch, 072h, 000h, 000h
        pop    ebp
        ret
sub_1_4612:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_1_261E
        add    esp, 4
        db  0C7h, 005h, 09Ch, 072h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    ebp
        ret
sub_1_462C:
        push   esi
        call   sub_1_5370
        db  08Bh, 035h, 0BCh, 055h, 000h, 000h
loc_1_4638:
        add    esi, eax
        mov    eax, dword ptr [esi]
        test   eax, 80000000h
        je     SHORT loc_1_4666
        and    eax, 7fffffffh
        mov    dword ptr [esi], eax
        db  0FFh, 00Dh, 0A0h, 072h, 000h, 000h
        db  083h, 03Dh, 0A0h, 072h, 000h, 000h, 001h
        jne    SHORT loc_1_4666
        db  068h, 0A0h, 072h, 000h, 000h
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_4666:
        or     dword ptr [esi], 40000000h
        push   5
        call   sub_1_5358
        and    dword ptr [esi], 0bfffffffh
        pop    esi
        ret
sub_1_467B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_5370
        db  08Bh, 00Dh, 0BCh, 055h, 000h, 000h
        add    ecx, eax
        mov    dword ptr [ebp - 4], ecx
        db  00Fh, 0B6h, 005h, 098h, 065h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_1_46A8
        db  0E8h, 06Eh, 0D6h, 0FFh, 0FFh
loc_1_46A8:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 3], 80h
        je     NEAR PTR loc_1_46E4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        and    eax, 7fffffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        db  0FFh, 00Dh, 0A0h, 072h, 000h, 000h
        db  083h, 03Dh, 0A0h, 072h, 000h, 000h, 001h
        jne    NEAR PTR loc_1_46E4
        db  068h, 0A0h, 072h, 000h, 000h
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_46E4:
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax], 40000000h
        push   dword ptr [ebp + 0ch]
        call   IFSMgr_Block
        add    esp, 4
        db  00Fh, 0B6h, 005h, 098h, 065h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_1_477E
        mov    eax, dword ptr [ebp + 0ch]
        inc    word ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 38h]
        or     eax, eax
        jle    NEAR PTR loc_1_4771
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 32h]
        test   al, 40h
        je     NEAR PTR loc_1_4771
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 19h]
        mov    ecx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx + 16h]
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx + 3ah]
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx + 3ch]
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 10h]
        jne    NEAR PTR loc_1_4771
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 38h], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 38h
        push   eax
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_4771:
        push   5
        call   sub_1_5358
        mov    eax, dword ptr [ebp + 0ch]
        dec    word ptr [eax]
loc_1_477E:
        mov    eax, dword ptr [ebp - 4]
        and    dword ptr [eax], 0bfffffffh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_478C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 74h]
        mov    ecx, dword ptr [edx + 24h]
        or     ecx, ecx
        je     SHORT loc_1_479E
        mov    edx, ecx
loc_1_479E:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, -1
        jne    SHORT loc_1_47AF
        push   1
        push   eax
        push   0
        push   dword ptr [edx]
        jmp    SHORT loc_1_47C9
loc_1_47AF:
        cmp    ecx, -2
        push   1
        push   eax
        jne    SHORT loc_1_47BE
        push   1
        push   dword ptr [edx + 4]
        jmp    SHORT loc_1_47C9
loc_1_47BE:
        lea    eax, [ecx + 0ah]
        push   eax
        mov    eax, dword ptr [edx + 8]
        push   dword ptr [eax + ecx*4 + 4]
loc_1_47C9:
        call   sub_1_1B90
        mov    esp, ebp
        pop    ebp
        ret
sub_1_47D2:
        push   ebp
        mov    ebp, esp
        push   ebx
loc_1_47D6:
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    bl, bl
        mov    edi, dword ptr [esi + 7ch]
        push   esi
        call   sub_1_446F
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_1_48B3
        mov    ecx, dword ptr [esi + 74h]
        mov    al, byte ptr [ecx + 1fh]
        test   al, 20h
        je     SHORT loc_1_4809
        mov    cx, word ptr [ecx + 32h]
        cmp    word ptr [esi + 6], cx
        je     NEAR PTR loc_1_48B3
loc_1_4809:
        test   al, 40h
        jne    NEAR PTR loc_1_48B3
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1
        jne    SHORT loc_1_4881
        test   byte ptr [edi + 32h], 20h
        jne    SHORT loc_1_482F
        mov    word ptr [esi + 1ah], 13h
        mov    eax, 13h
        jmp    NEAR PTR loc_1_4928
loc_1_482F:
        cmp    eax, 1
        jne    SHORT loc_1_4881
        mov    al, byte ptr [edi + 30h]
        cmp    al, 3
        jb     SHORT loc_1_487D
        cmp    al, 4
        je     SHORT loc_1_4855
        cmp    al, 3
        jne    SHORT loc_1_484C
        db  0E8h, 0B9h, 0B7h, 0FFh, 0FFh
        or     eax, eax
        je     SHORT loc_1_4855
loc_1_484C:
        db  080h, 03Dh, 098h, 065h, 000h, 000h, 000h
        je     SHORT loc_1_487D
loc_1_4855:
        inc    word ptr [edi + 3ch]
        lea    ebx, [edi + 3ch]
        lea    eax, [edi + 38h]
        cmp    word ptr [eax], 0
        jbe    SHORT loc_1_4873
        mov    word ptr [eax], 0
        push   eax
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_4873:
        push   ebx
        push   edi
        call   sub_1_467B
        add    esp, 8
loc_1_487D:
        mov    bl, 10h
        jmp    SHORT loc_1_48B3
loc_1_4881:
        or     eax, eax
        jne    SHORT loc_1_48B3
        cmp    byte ptr [edi + 30h], 4
        jne    SHORT loc_1_48B3
        lea    eax, [edi + 38h]
        inc    word ptr [edi + 3ah]
        cmp    word ptr [eax], 0
        jbe    SHORT loc_1_48A6
        mov    word ptr [eax], 0
        push   eax
        call   IFSMgr_Wakeup
        add    esp, 4
loc_1_48A6:
        lea    eax, [edi + 3ah]
        push   eax
        push   edi
        call   sub_1_467B
        add    esp, 8
loc_1_48B3:
        mov    ecx, dword ptr [esi + 74h]
        mov    eax, dword ptr [ecx + 24h]
        or     eax, eax
        je     SHORT loc_1_490A
        mov    ax, word ptr [eax + 10h]
        test   al, 1
        je     SHORT loc_1_48E1
        push   3
        push   esi
        db  0E8h, 000h, 0C0h, 0FFh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_48D6
        jmp    SHORT loc_1_4924
loc_1_48D6:
        mov    eax, dword ptr [esi + 74h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 20h], eax
        jmp    SHORT loc_1_490A
loc_1_48E1:
        test   al, 2
        je     SHORT loc_1_48F9
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [esi + 74h]
        mov    eax, dword ptr [eax + 24h]
        and    word ptr [eax + 10h], 0fffdh
        jmp    SHORT loc_1_490A
loc_1_48F9:
        test   byte ptr [ecx + 1eh], 4
        je     SHORT loc_1_490A
        push   1
        push   esi
        call   sub_1_433B
        add    esp, 8
loc_1_490A:
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_478C
        add    esp, 8
        cmp    byte ptr [edi + 30h], 0
        jbe    SHORT loc_1_4924
        movzx  ax, bl
        or     word ptr [edi + 32h], ax
loc_1_4924:
        movsx  eax, word ptr [esi + 1ah]
loc_1_4928:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_492D:
        push   ebp
        mov    ebp, esp
        push   0
        push   -1
        push   dword ptr [ebp + 8]
        call   sub_1_47D2
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4940:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 7ch]
        test   byte ptr [ecx + 32h], 20h
        jne    SHORT loc_1_4954
        or     word ptr [eax + 18h], 1
loc_1_4954:
        push   1
        push   -2
        push   eax
        call   sub_1_47D2
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4962:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 19h], 10h
        je     SHORT loc_1_4996
        mov    eax, dword ptr [esi + 74h]
        test   byte ptr [eax + 1eh], 4
        je     SHORT loc_1_4996
        mov    eax, dword ptr [eax + 24h]
        test   byte ptr [eax + 10h], 8
        je     SHORT loc_1_4996
        push   1
        push   esi
        call   sub_1_433B
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_4996
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_1_49A1
loc_1_4996:
        push   -1
        push   esi
        call   sub_1_478C
        add    esp, 8
loc_1_49A1:
        pop    esi
        pop    ebp
        ret
loc_1_49A4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 19h], 10h
        je     SHORT loc_1_49D8
        mov    eax, dword ptr [esi + 74h]
        test   byte ptr [eax + 1eh], 4
        je     SHORT loc_1_49D8
        mov    eax, dword ptr [eax + 24h]
        test   byte ptr [eax + 10h], 8
        je     SHORT loc_1_49D8
        push   1
        push   esi
        call   sub_1_433B
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_49D8
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_1_49E3
loc_1_49D8:
        push   -2
        push   esi
        call   sub_1_478C
        add    esp, 8
loc_1_49E3:
        pop    esi
        pop    ebp
        ret
loc_1_49E6:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_478C
        mov    esp, ebp
        pop    ebp
        ret
loc_1_49F7:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 19h], 10h
        je     SHORT loc_1_4A2B
        mov    eax, dword ptr [esi + 74h]
        test   byte ptr [eax + 1eh], 4
        je     SHORT loc_1_4A2B
        mov    eax, dword ptr [eax + 24h]
        test   byte ptr [eax + 10h], 8
        je     SHORT loc_1_4A2B
        push   1
        push   esi
        call   sub_1_433B
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_4A2B
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_1_4A36
loc_1_4A2B:
        push   7
        push   esi
        call   sub_1_478C
        add    esp, 8
loc_1_4A36:
        pop    esi
        pop    ebp
        ret
loc_1_4A39:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 1ah], 28h
        mov    eax, 28h
        ret
sub_1_4A4C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 74h]
        mov    eax, dword ptr [esi + 24h]
        test   byte ptr [eax + 10h], 4
        je     SHORT loc_1_4A85
        mov    ebx, 4
loc_1_4A66:
        mov    eax, dword ptr [esi + 24h]
        inc    word ptr [eax + 12h]
        mov    eax, dword ptr [esi + 24h]
        add    eax, 12h
        push   eax
        call   IFSMgr_Block
        add    esp, 4
        mov    eax, dword ptr [esi + 24h]
        test   word ptr [eax + 10h], bx
        jne    SHORT loc_1_4A66
loc_1_4A85:
        mov    esi, dword ptr [esi + 24h]
        movzx  eax, word ptr [esi + 14h]
        add    esi, 14h
        add    eax, 2
        mov    edi, dword ptr [edi + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_4AA8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  0A1h, 00Ch, 072h, 000h, 000h
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [edi + 7ch], eax
        push   edi
        call   sub_1_2D7B
        db  0A1h, 00Ch, 072h, 000h, 000h
        add    esp, 4
        mov    eax, dword ptr [eax + 10h]
        cmp    byte ptr [eax + 30h], 4
        je     SHORT loc_1_4AD6
        jmp    SHORT loc_1_4B2F
loc_1_4AD6:
        push   edi
        call   sub_1_446F
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_4AEA
        mov    esi, 5
        jmp    SHORT loc_1_4B34
loc_1_4AEA:
        db  0A1h, 00Ch, 072h, 000h, 000h
        mov    ecx, dword ptr [eax + 24h]
        mov    ax, word ptr [edi + 0a8h]
        test   al, 1
        je     SHORT loc_1_4B17
        mov    ax, word ptr [ecx + 10h]
        or     ax, 8
        mov    word ptr [ecx + 10h], ax
        test   al, 10h
        je     SHORT loc_1_4B34
        and    ax, 0ffefh
        mov    word ptr [ecx + 10h], ax
        jmp    SHORT loc_1_4B34
loc_1_4B17:
        test   al, 2
        je     SHORT loc_1_4B2F
        mov    ax, word ptr [ecx + 10h]
        test   al, 8
        jne    SHORT loc_1_4B25
        jmp    SHORT loc_1_4B2F
loc_1_4B25:
        or     ax, 10h
        mov    word ptr [ecx + 10h], ax
        jmp    SHORT loc_1_4B34
loc_1_4B2F:
        mov    esi, 1
loc_1_4B34:
        mov    word ptr [edi + 1ah], si
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h
sub_1_4B40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_28FE
        add    esp, 4
        push   dword ptr [esi + 7ch]
        call   sub_1_2D0D
        add    esp, 4
        push   esi
        call   sub_1_2D7B
        add    esp, 4
        pop    esi
        pop    ebp
        ret
_LTEXT ends
        end
