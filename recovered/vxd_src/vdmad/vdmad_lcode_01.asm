; vdmad_lcode_01.asm -- VDMAD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VDMAD_Query:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_0011
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0011:
        je     SHORT loc_1_002C
        xor    al, al
        ret
loc_1_0016:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_0027
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0027:
        je     SHORT loc_1_002C
        xor    al, al
loc_1_002B:
        ret
loc_1_002C:
        mov    cl, al
        and    cl, 0ch
        je     SHORT loc_1_002B
        db  0CDh, 020h, 02Fh, 000h, 001h, 000h
loc_1_0039:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_004A
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_004A:
        je     SHORT loc_1_004E
        in     al, dx
        ret
loc_1_004E:
        out    dx, al
        db  0BEh, 00Ch, 020h, 000h, 000h
        cmp    dx, 40bh
        db  0BAh, 078h, 01Dh, 000h, 000h
        je     SHORT loc_1_0066
        add    esi, 8
        add    edx, 4
loc_1_0066:
        movzx  ecx, al
        and    cl, 3
        mov    byte ptr [edx + ecx], al
        and    al, 0ch
        btr    dword ptr [esi], ecx
        cmp    al, 4
        jne    SHORT loc_1_007B
        bts    dword ptr [esi], ecx
loc_1_007B:
        ret
loc_1_007C:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        sub    edx, 400h
        cmp    ecx, 4
        jbe    SHORT loc_1_0093
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0093:
        je     SHORT loc_1_00A4
        call   sub_1_1519
        call   sub_1_02DA
        movzx  eax, word ptr [eax + 23h]
        ret
loc_1_00A4:
        call   sub_1_1519
        mov    ah, 3
        call   sub_1_175F
        xor    edx, edx
        jmp    NEAR PTR sub_1_1932
loc_1_00B7:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        db  0BEh, 00Ch, 020h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_00CD
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_00CD:
        jb     SHORT loc_1_00E8
        call   sub_1_00F6
        mov    edx, eax
        call   sub_1_02DA
        mov    byte ptr [eax + 2ah], dl
        mov    byte ptr [eax + 2eh], dl
        xor    edx, edx
        jmp    NEAR PTR sub_1_1932
loc_1_00E8:
        call   sub_1_00F6
        call   sub_1_02DA
        mov    al, byte ptr [eax + 2ah]
        ret
sub_1_00F6:
        movzx  ecx, dl
        and    cl, 7
        cmp    dx, 4c6h
        jb     SHORT loc_1_0108
        add    esi, 8
        shr    cl, 1
loc_1_0108:
        dec    cl
        shr    cl, 1
        ret
loc_1_010D:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_011E
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_011E:
        je     SHORT loc_1_0123
        xor    al, al
loc_1_0122:
        ret
loc_1_0123:
        or     al, al
        je     SHORT loc_1_0122
        db  0CDh, 020h, 02Fh, 000h, 001h, 000h
loc_1_012D:
        pushad
        mov    edi, eax
        mov    ebx, edx
        movzx  eax, word ptr [edi + 18h]
        mov    edx, dword ptr [edi]
        db  00Fh, 0B6h, 092h, 034h, 021h, 000h, 000h
        call   sub_1_06C6
        movzx  eax, word ptr [edi + 16h]
        mov    edx, dword ptr [edi]
        db  00Fh, 0B6h, 092h, 02Ch, 021h, 000h, 000h
        out    dx, al
        add    dx, 400h
        mov    al, ah
        out    dx, al
        mov    eax, dword ptr [edi + 10h]
        dec    eax
        mov    edx, dword ptr [edi]
        db  08Ah, 08Ah, 078h, 01Dh, 000h, 000h
        and    cl, 0ch
        cmp    cl, 4
        jne    SHORT loc_1_0170
        shr    eax, 1
loc_1_0170:
        db  00Fh, 0B6h, 092h, 03Ch, 021h, 000h, 000h
        call   sub_1_06C6
        add    dx, 400h
        shr    eax, 10h
        out    dx, al
        mov    eax, dword ptr [edi]
        mov    edx, eax
        and    al, 3
        and    bl, 0fch
        or     al, bl
        and    edx, 4
        shl    edx, 2
        db  00Fh, 0B6h, 092h, 020h, 020h, 000h, 000h
        out    dx, al
        mov    eax, dword ptr [edi]
        mov    edx, eax
        and    al, 3
        db  00Ah, 082h, 078h, 01Dh, 000h, 000h
        and    bh, 0f0h
        or     al, bh
        and    edx, 0fffffffch
        mov    dx, 40bh
        je     SHORT loc_1_01BC
        mov    dx, 4d6h
loc_1_01BC:
        out    dx, al
        popad
        ret
        db  000h
VDMAD_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0D3h, 0FEh, 0FEh, 0FFh
        cmp    eax, 7
        je     SHORT loc_1_0205
        cmp    eax, 0bh
        je     SHORT loc_1_024F
        cmp    eax, 6
        je     NEAR PTR loc_1_1058
        cmp    eax, 22h
        db  00Fh, 084h, 01Bh, 0FEh, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 02Eh, 001h, 0FFh, 0FFh
        db  0F7h, 005h, 0F4h, 001h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_1_0203
        cmp    eax, 14h
        db  00Fh, 084h, 0FDh, 0FDh, 0FEh, 0FFh
loc_1_0203:
        clc
        ret
loc_1_0205:
        mov    esi, ebx
        db  003h, 035h, 068h, 01Eh, 000h, 000h
        xor    eax, eax
        mov    word ptr [esi + 8], ax
        clc
        ret
sub_1_0215:
        push   eax
        push   esi
        db  0BEh, 00Ch, 020h, 000h, 000h
        mov    eax, dword ptr [eax]
        btr    eax, 2
        jae    SHORT loc_1_0229
        db  0BEh, 014h, 020h, 000h, 000h
loc_1_0229:
        bts    dword ptr [esi + 5], eax
        pop    esi
        pop    eax
        db  0CDh, 020h, 007h, 000h, 004h, 000h
        cmp    byte ptr [eax + 15h], 0
        je     SHORT loc_1_0242
        call   sub_1_0FCC
        jmp    SHORT loc_1_024E
loc_1_0242:
        cmp    byte ptr [eax + 14h], 0
        je     SHORT loc_1_024E
        stc
        call   sub_1_0F64
loc_1_024E:
        ret
loc_1_024F:
        db  0B8h, 06Ch, 01Eh, 000h, 000h
        mov    ecx, 8
loc_1_0259:
        cmp    dword ptr [eax + 8], ebx
        jne    SHORT loc_1_029F
        call   sub_1_02EF
        jae    SHORT loc_1_029A
        mov    edx, ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xchg   ebx, edx
        mov    dword ptr [eax + 8], edx
        cmp    byte ptr [eax + 15h], 0
        je     SHORT loc_1_0284
        push   eax
        movzx  eax, byte ptr [eax + 15h]
        db  089h, 090h, 04Bh, 020h, 000h, 000h
        pop    eax
loc_1_0284:
        cmp    dword ptr [eax + 1ch], 0
        jne    SHORT loc_1_029F
        push   esi
        mov    esi, dword ptr [eax + 0ch]
        sub    esi, dword ptr [ebx + 4]
        add    esi, dword ptr [edx + 4]
        mov    dword ptr [eax + 0ch], esi
        pop    esi
        jmp    SHORT loc_1_029F
loc_1_029A:
        call   sub_1_0215
loc_1_029F:
        add    eax, 34h
        loop   loc_1_0259
        db  0B8h, 03Ch, 020h, 000h, 000h
loc_1_02A9:
        cmp    dword ptr [eax + 10h], ebx
        jne    SHORT loc_1_02BE
        push   ebx
        mov    ebx, eax
        db  081h, 0EBh, 03Bh, 020h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 004h, 000h
        pop    ebx
loc_1_02BE:
        add    eax, 18h
        db  03Dh, 0FCh, 020h, 000h, 000h
        jb     SHORT loc_1_02A9
        clc
        ret
sub_1_02CA:
        cmp    eax, 8
        jae    SHORT loc_1_02D8
        imul   eax, eax, 34h
        db  005h, 06Ch, 01Eh, 000h, 000h
        stc
loc_1_02D8:
        cmc
        ret
sub_1_02DA:
        mov    eax, ecx
        db  081h, 0FEh, 00Ch, 020h, 000h, 000h
        jbe    SHORT loc_1_02E6
        add    al, 4
loc_1_02E6:
        imul   eax, eax, 34h
        db  005h, 06Ch, 01Eh, 000h, 000h
        ret
sub_1_02EF:
        push   eax
        push   ecx
        push   edx
        cmp    dword ptr [eax + 24h], 0a0000h
        jae    SHORT loc_1_031F
        mov    edx, dword ptr [eax + 2ch]
        test   byte ptr [eax + 31h], 40h
        je     SHORT loc_1_0307
        shl    edx, 1
        inc    edx
loc_1_0307:
        inc    edx
        push   0
        push   edx
        push   dword ptr [eax + 24h]
        db  0CDh, 020h, 05Fh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 1
        je     SHORT loc_1_031F
        clc
        jmp    SHORT loc_1_0320
loc_1_031F:
        stc
loc_1_0320:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_1_0324:
        push   ecx
        in     al, 8
        db  008h, 005h, 018h, 021h, 000h, 000h
        db  08Ah, 00Dh, 010h, 020h, 000h, 000h
        and    cl, 0fh
        or     cl, al
        db  088h, 00Dh, 010h, 020h, 000h, 000h
        in     al, 0d0h
        db  008h, 005h, 019h, 021h, 000h, 000h
        db  08Ah, 00Dh, 018h, 020h, 000h, 000h
        and    cl, 0fh
        or     cl, al
        db  088h, 00Dh, 018h, 020h, 000h, 000h
        db  00Fh, 0B7h, 005h, 018h, 021h, 000h, 000h
        pop    ecx
        ret
VDMAD_Get_Version:
        mov    eax, 400h
        clc
        ret
VDMAD_Get_Max_Phys_Page:
        db  0A1h, 000h, 021h, 000h, 000h
        ret
VDMAD_Virtualize_Channel:
        call   sub_1_02CA
        db  081h, 078h, 004h, 02Dh, 00Eh, 000h, 000h
        je     SHORT loc_1_037E
        stc
        jmp    SHORT loc_1_038B
loc_1_037E:
        push   eax
        mov    eax, dword ptr [eax]
        db  0E8h, 050h, 0FEh, 0FEh, 0FFh
        pop    eax
        mov    dword ptr [eax + 4], esi
        clc
loc_1_038B:
        ret
VDMAD_Unvirtualize_Channel:
        mov    edx, eax
        mov    eax, dword ptr [edx]
        call   sub_1_02CA
        jb     SHORT loc_1_039B
        cmp    eax, edx
        je     SHORT loc_1_039D
loc_1_039B:
        stc
        ret
loc_1_039D:
        mov    eax, dword ptr [edx]
        db  0E8h, 048h, 0FEh, 0FEh, 0FFh
        db  0C7h, 042h, 004h, 02Dh, 00Eh, 000h, 000h
        mov    word ptr [edx + 1ah], 0
        clc
        ret
VDMAD_Set_Channel_Callbacks:
        and    word ptr [eax + 1ah], bx
        or     word ptr [eax + 1ah], cx
        ret
VDMAD_Disable_Translation:
        push   ecx
        mov    ecx, dword ptr [eax]
        db  00Fh, 0ABh, 00Dh, 010h, 021h, 000h, 000h
        mov    ecx, dword ptr [eax + 1ch]
        add    ecx, 1
        jb     SHORT loc_1_03D1
        mov    dword ptr [eax + 1ch], ecx
loc_1_03D1:
        pop    ecx
        ret
VDMAD_Enable_Translation:
        push   ecx
        mov    ecx, dword ptr [eax + 1ch]
        sub    ecx, 1
        jb     SHORT loc_1_03EB
        mov    dword ptr [eax + 1ch], ecx
        jne    SHORT loc_1_03EB
        mov    ecx, dword ptr [eax]
        db  00Fh, 0B3h, 00Dh, 010h, 021h, 000h, 000h
        clc
loc_1_03EB:
        pop    ecx
        ret
VDMAD_Get_Region_Info:
        movzx  ebx, byte ptr [eax + 15h]
        mov    bh, byte ptr [eax + 14h]
        mov    esi, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        ret
VDMAD_Set_Region_Info:
        mov    byte ptr [eax + 15h], bl
        mov    byte ptr [eax + 14h], bh
        mov    dword ptr [eax + 0ch], esi
        mov    dword ptr [eax + 10h], ecx
        push   edx
        db  080h, 03Dh, 01Ah, 021h, 000h, 000h, 001h
        je     SHORT loc_1_0422
        test   byte ptr [eax + 31h], 40h
        je     SHORT loc_1_0422
        shr    edx, 1
        ror    edx, 10h
        shl    dx, 1
        ror    edx, 10h
loc_1_0422:
        mov    word ptr [eax + 18h], dx
        shr    edx, 10h
        mov    word ptr [eax + 16h], dx
        pop    edx
        ret
VDMAD_Get_Virt_State:
        db  0BEh, 00Ch, 020h, 000h, 000h
        mov    ecx, dword ptr [eax]
        push   eax
        cmp    cl, 4
        jb     SHORT loc_1_0442
        sub    cl, 4
        add    esi, 8
loc_1_0442:
        movzx  edx, byte ptr [eax + 30h]
        and    dl, 0fch
        mov    dh, byte ptr [eax + 31h]
        movzx  eax, byte ptr [esi + 5]
        bt     eax, ecx
        jae    SHORT loc_1_0458
        or     dl, 1
loc_1_0458:
        movzx  eax, byte ptr [esi + 6]
        bt     eax, ecx
        jae    SHORT loc_1_0464
        or     dl, 2
loc_1_0464:
        pop    eax
        mov    esi, dword ptr [eax + 20h]
        mov    ecx, dword ptr [eax + 28h]
        cmp    dword ptr [eax + 1ch], 0
        jne    SHORT loc_1_0474
        add    esi, dword ptr [ebx + 4]
loc_1_0474:
        test   dh, 40h
        je     SHORT loc_1_047C
        shl    ecx, 1
        inc    ecx
loc_1_047C:
        inc    ecx
        ret
VDMAD_Set_Virt_State:
        pushad
        db  0BFh, 00Ch, 020h, 000h, 000h
        mov    ecx, dword ptr [eax]
        cmp    cl, 4
        jb     SHORT loc_1_0491
        sub    cl, 4
        add    edi, 8
loc_1_0491:
        movzx  eax, byte ptr [edi + 5]
        test   dl, 1
        je     SHORT loc_1_049F
        bts    eax, ecx
        jmp    SHORT loc_1_04A2
loc_1_049F:
        btr    eax, ecx
loc_1_04A2:
        mov    byte ptr [edi + 5], al
        movzx  eax, byte ptr [edi + 6]
        test   dl, 2
        je     SHORT loc_1_04B3
        bts    eax, ecx
        jmp    SHORT loc_1_04B6
loc_1_04B3:
        btr    eax, ecx
loc_1_04B6:
        mov    byte ptr [edi + 6], al
        mov    edi, dword ptr [esp + 1ch]
        and    dl, 0fch
        mov    byte ptr [edi + 30h], dl
        mov    byte ptr [edi + 31h], dh
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [edi + 1ch], 0
        jne    SHORT loc_1_04D3
        sub    eax, dword ptr [ebx + 4]
loc_1_04D3:
        mov    dword ptr [edi + 20h], eax
        mov    eax, dword ptr [esp + 18h]
        dec    eax
        test   dh, 40h
        je     SHORT loc_1_04E2
        shr    eax, 1
loc_1_04E2:
        mov    dword ptr [edi + 28h], eax
        mov    dword ptr [edi + 2ch], eax
        popad
        ret
VDMAD_Get_Virt_Count:
        mov    ecx, dword ptr [eax + 28h]
        test   byte ptr [eax + 31h], 40h
        je     SHORT loc_1_04F6
        shl    ecx, 1
        inc    ecx
loc_1_04F6:
        inc    ecx
        ret
VDMAD_Set_Virt_Count:
        push   ecx
        jecxz  loc_1_0504
        dec    ecx
        test   byte ptr [eax + 31h], 40h
        je     SHORT loc_1_0504
        shr    ecx, 1
loc_1_0504:
        mov    dword ptr [eax + 28h], ecx
        pop    ecx
        ret
VDMAD_Set_Phys_State:
        pushad
        mov    edi, eax
        mov    ebx, edx
        movzx  eax, word ptr [edi + 18h]
        mov    edx, dword ptr [edi]
        db  00Fh, 0B6h, 092h, 034h, 021h, 000h, 000h
        call   sub_1_06C6
        movzx  eax, word ptr [edi + 16h]
        mov    edx, dword ptr [edi]
        db  00Fh, 0B6h, 092h, 02Ch, 021h, 000h, 000h
        out    dx, al
        mov    eax, dword ptr [edi + 10h]
        dec    eax
        mov    edx, dword ptr [edi]
        cmp    edx, 4
        jb     SHORT loc_1_053B
        shr    eax, 1
loc_1_053B:
        db  00Fh, 0B6h, 092h, 03Ch, 021h, 000h, 000h
        call   sub_1_06C6
        mov    eax, dword ptr [edi]
        mov    edx, eax
        and    al, 3
        and    bl, 0fch
        or     al, bl
        and    edx, 4
        shl    edx, 2
        db  00Fh, 0B6h, 092h, 020h, 020h, 000h, 000h
        out    dx, al
        popad
        ret
VDMAD_Get_Phys_Count:
        push   edx
        push   edi
        mov    edi, eax
        mov    eax, dword ptr [edi]
        db  00Fh, 0B6h, 090h, 03Ch, 021h, 000h, 000h
        call   sub_1_067C
        mov    ecx, eax
        mov    eax, edi
        pop    edi
        pop    edx
        ret
VDMAD_Get_Phys_Status:
        pushfd
        cli
        call   sub_1_0324
        popfd
        ret
VDMAD_Mask_Channel:
        pushad
        pushfd
        cli
        db  00Fh, 0B6h, 00Dh, 013h, 020h, 000h, 000h
        db  08Ah, 02Dh, 01Bh, 020h, 000h, 000h
        jecxz  loc_1_05DB
        push   ebx
        xor    ebx, ebx
        mov    edx, dword ptr [eax]
        bts    ebx, edx
        shl    ebx, 4
        shr    bl, 4
        not    ebx
        and    ecx, ebx
        pop    ebx
        db  088h, 00Dh, 013h, 020h, 000h, 000h
        db  088h, 02Dh, 01Bh, 020h, 000h, 000h
        jne    SHORT loc_1_05DB
        push   eax
        db  0A1h, 008h, 021h, 000h, 000h
        db  0A3h, 00Ch, 021h, 000h, 000h
        pop    eax
        db  0C7h, 005h, 004h, 021h, 000h, 000h, 052h, 019h, 000h, 000h
        xor    esi, esi
        db  087h, 035h, 014h, 021h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 001h, 000h
loc_1_05DB:
        popfd
loc_1_05DC:
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [eax]
        mov    edx, eax
        and    edx, 4
        shl    edx, 2
        and    al, 3
        or     al, 4
        db  00Fh, 0B6h, 092h, 021h, 020h, 000h, 000h
        out    dx, al
        jmp    SHORT loc_1_05FB
loc_1_05FB:
        popad
        ret
VDMAD_Phys_Mask_Channel:
        pushad
        jmp    SHORT loc_1_05DC
VDMAD_UnMask_Channel:
        pushad
        mov    edx, eax
        pushfd
        cli
        call   sub_1_0324
        xor    ecx, ecx
        mov    eax, dword ptr [edx]
        bts    ecx, eax
        shl    ecx, 4
        shr    cl, 4
        not    ecx
        db  020h, 00Dh, 013h, 020h, 000h, 000h
        db  020h, 02Dh, 01Bh, 020h, 000h, 000h
        not    ecx
        test   byte ptr [edx + 30h], 10h
        jne    SHORT loc_1_0658
        db  008h, 00Dh, 013h, 020h, 000h, 000h
        db  008h, 02Dh, 01Bh, 020h, 000h, 000h
        not    ecx
        db  066h, 021h, 00Dh, 018h, 021h, 000h, 000h
        db  0BEh, 07Eh, 00Dh, 000h, 000h
        db  089h, 035h, 00Ch, 021h, 000h, 000h
        db  0BEh, 05Dh, 019h, 000h, 000h
        db  089h, 035h, 004h, 021h, 000h, 000h
loc_1_0658:
        popfd
loc_1_0659:
        mov    dword ptr [edx + 8], ebx
        mov    eax, dword ptr [edx]
        call   sub_1_1D68
        mov    edx, eax
        and    edx, 4
        shl    edx, 2
        and    al, 3
        db  00Fh, 0B6h, 092h, 021h, 020h, 000h, 000h
        out    dx, al
        jmp    SHORT loc_1_0677
loc_1_0677:
        popad
        ret
VDMAD_Phys_Unmask_Channel:
        pushad
        jmp    SHORT loc_1_0659
sub_1_067C:
        pushfd
        cli
        push   edx
        mov    eax, dword ptr [edi]
        and    eax, 4
        shl    eax, 2
        db  00Fh, 0B6h, 090h, 023h, 020h, 000h, 000h
        out    dx, al
        jmp    SHORT loc_1_0691
loc_1_0691:
        pop    edx
        push   ecx
        call   sub_1_06A6
loc_1_0698:
        mov    ecx, eax
        call   sub_1_06A6
        cmp    ah, ch
        jne    SHORT loc_1_0698
        pop    ecx
        popfd
        ret
sub_1_06A6:
        push   edx
        in     al, dx
        jmp    SHORT loc_1_06AA
loc_1_06AA:
        xchg   ah, al
        in     al, dx
        xchg   ah, al
        db  080h, 03Dh, 01Ah, 021h, 000h, 000h, 002h
        jne    SHORT loc_1_06C4
        add    dx, 400h
        shl    eax, 10h
        in     al, dx
        rol    eax, 10h
loc_1_06C4:
        pop    edx
        ret
sub_1_06C6:
        pushfd
        cli
        push   eax
        push   edx
        mov    eax, dword ptr [edi]
        and    eax, 4
        shl    eax, 2
        db  00Fh, 0B6h, 090h, 023h, 020h, 000h, 000h
        out    dx, al
        jmp    SHORT loc_1_06DC
loc_1_06DC:
        pop    edx
        pop    eax
        out    dx, al
        jmp    SHORT loc_1_06E1
loc_1_06E1:
        xchg   ah, al
        out    dx, al
        xchg   ah, al
        popfd
        ret
sub_1_06E8:
        or     al, 0ffh
        push   esi
        push   ecx
        shl    esi, 0ch
loc_1_06EF:
        and    byte ptr [esi], al
        add    esi, 1000h
        loop   loc_1_06EF
        pop    ecx
        pop    esi
        ret
VDMAD_Lock_DMA_Region:
        pushfd
        pushad
        mov    ebp, esp
        sub    esp, 18h
        mov    edi, esp
        mov    dword ptr [edi], ecx
        mov    dword ptr [edi + 4], esi
        xor    eax, eax
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 0ch], eax
        mov    byte ptr [edi + 0ch], 1
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 00Bh, 000h, 004h, 000h
        jb     SHORT loc_1_078E
        mov    edx, dword ptr [edi + 10h]
        mov    ecx, dword ptr [edi + 14h]
        pushfd
        test   byte ptr [ebp + 14h], 3
        je     SHORT loc_1_0748
        mov    esi, 10000h
        test   byte ptr [ebp + 14h], 2
        je     SHORT loc_1_073E
        shl    esi, 1
loc_1_073E:
        movzx  eax, dx
        add    eax, ecx
        dec    eax
        cmp    eax, esi
        jae    SHORT loc_1_0781
loc_1_0748:
        popfd
        je     SHORT loc_1_079B
        mov    dword ptr [ebp + 18h], ecx
        mov    al, 1
        cmp    word ptr [edi + 0eh], 1
        jg     SHORT loc_1_0759
        mov    al, 3
loc_1_0759:
        movzx  eax, al
        mov    dword ptr [ebp + 1ch], eax
        mov    esi, dword ptr [edi + 4]
        lea    eax, [ecx + esi + 0fffh]
        shr    esi, 0ch
        shr    eax, 0ch
        sub    eax, esi
        push   0
        push   eax
        push   esi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        stc
        jmp    SHORT loc_1_079F
loc_1_0781:
        popfd
        sub    esi, eax
        dec    esi
        add    esi, ecx
        mov    dword ptr [ebp + 18h], esi
        mov    al, 2
        jmp    SHORT loc_1_0759
loc_1_078E:
        xor    eax, eax
        mov    dword ptr [ebp + 18h], eax
        mov    al, 3
        mov    dword ptr [ebp + 1ch], eax
        stc
        jmp    SHORT loc_1_079F
loc_1_079B:
        mov    dword ptr [ebp + 14h], edx
        clc
loc_1_079F:
        pushfd
        pop    eax
        mov    dword ptr [ebp + 20h], eax
        mov    esp, ebp
        popad
        popfd
        ret
VDMAD_Unlock_DMA_Region:
        clc
        jecxz  loc_1_07D3
        pushad
loc_1_07AD:
        pushfd
        dec    ecx
        add    ecx, esi
        shr    esi, 0ch
        shr    ecx, 0ch
        sub    ecx, esi
        inc    ecx
        popfd
        jb     SHORT loc_1_07C2
        call   sub_1_06E8
loc_1_07C2:
        push   0
        push   ecx
        push   esi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        sub    eax, 1
        popad
loc_1_07D3:
        ret
VDMAD_Unlock_DMA_Region_No_Dirty:
        pushad
        stc
        jmp    SHORT loc_1_07AD
loc_1_07D8:
        push   0
        push   1
        push   esi
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0804
        push   0
        push   ebp
        push   1
        push   esi
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        mov    eax, dword ptr [ebp]
        and    eax, 0fffff000h
        or     al, 1
        ret
loc_1_0804:
        xor    eax, eax
        stc
        ret
loc_1_0808:
        push   0
        push   ebp
        push   1
        push   esi
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        mov    eax, dword ptr [ebp]
        test   al, 1
        je     SHORT loc_1_084A
        push   0
        push   1
        push   esi
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_084D
        push   0
        push   ebp
        push   1
        push   esi
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        mov    eax, dword ptr [ebp]
        and    eax, 0fffff000h
        or     al, 1
        ret
loc_1_084A:
        xor    eax, eax
        ret
loc_1_084D:
        xor    eax, eax
        stc
        ret
VDMAD_Scatter_Lock:
        pushad
        movzx  eax, word ptr [edi + 8]
        or     eax, eax
        je     SHORT loc_1_0870
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_1_0A0B
loc_1_0870:
        mov    esi, dword ptr [edi + 4]
        add    esi, eax
        mov    edx, esi
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        jne    SHORT loc_1_0882
        jmp    NEAR PTR loc_1_0A0D
loc_1_0882:
        dec    ecx
        add    ecx, esi
        jb     NEAR PTR loc_1_0A0F
        shr    esi, 0ch
        shr    ecx, 0ch
        sub    ecx, esi
        inc    ecx
        test   byte ptr [esp + 1ch], 1
        je     NEAR PTR loc_1_092B
        mov    eax, edx
        and    eax, 0fffh
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp + 14h], ecx
        mov    word ptr [edi + 0eh], 0
        movzx  ebx, word ptr [edi + 0ch]
        or     ebx, ebx
        je     NEAR PTR loc_1_0A11
        db  0C7h, 005h, 01Ch, 021h, 000h, 000h, 0D8h, 007h, 000h, 000h
        test   byte ptr [esp + 1ch], 2
        jne    SHORT loc_1_08DB
        db  0C7h, 005h, 01Ch, 021h, 000h, 000h, 008h, 008h, 000h, 000h
loc_1_08DB:
        push   edi
        push   ecx
        push   ebp
        sub    esp, 4
        mov    ebp, esp
        add    edi, 10h
loc_1_08E6:
        push   ecx
        db  0FFh, 015h, 01Ch, 021h, 000h, 000h
        jb     SHORT loc_1_0928
        dec    ebx
        js     SHORT loc_1_091A
        cld
        stosd
        pop    ecx
        inc    esi
        loop   loc_1_08E6
loc_1_08F8:
        add    esp, 4
        pop    ebp
        pop    edx
        pop    edi
        jecxz  loc_1_0915
        sub    edx, ecx
        jbe    NEAR PTR loc_1_0A11
        shl    edx, 0ch
        mov    eax, edx
        mov    edx, dword ptr [esp + 4]
        sub    eax, edx
        mov    dword ptr [edi], eax
loc_1_0915:
        jmp    NEAR PTR loc_1_09E8
loc_1_091A:
        push   0
        push   1
        push   esi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_1_0928:
        pop    ecx
        jmp    SHORT loc_1_08F8
loc_1_092B:
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp + 14h], ecx
        mov    word ptr [edi + 0eh], 0
        movzx  ebx, word ptr [edi + 0ch]
        or     ebx, ebx
        je     NEAR PTR loc_1_0A11
        push   edi
        push   ecx
        push   ebp
        push   dword ptr [edi]
        sub    esp, 0ch
        mov    ebp, esp
        add    edi, 0ch
        mov    eax, edx
        and    eax, 0fffh
        neg    eax
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp + 4], 0ffffffffh
loc_1_096C:
        push   ecx
        push   0
        push   1
        push   esi
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_1_0A08
        push   0
        push   ebp
        push   1
        push   esi
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        mov    eax, dword ptr [ebp]
        and    eax, 0fffff000h
        cmp    eax, dword ptr [ebp + 4]
        je     SHORT loc_1_09BC
        add    edi, 4
        dec    ebx
        js     SHORT loc_1_09FA
        cld
        stosd
        cmp    dword ptr [ebp + 4], -1
        je     SHORT loc_1_09B5
        mov    dword ptr [edi], 0
        jmp    SHORT loc_1_09BC
loc_1_09B5:
        mov    edx, dword ptr [edi]
        neg    edx
        add    dword ptr [edi - 4], edx
loc_1_09BC:
        mov    edx, 1000h
        add    dword ptr [edi], edx
        add    eax, edx
        add    dword ptr [ebp + 8], edx
        mov    dword ptr [ebp + 4], eax
        pop    ecx
        inc    esi
        loop   loc_1_096C
        mov    edx, dword ptr [ebp + 8]
        sub    edx, dword ptr [ebp + 0ch]
        sub    dword ptr [edi], edx
loc_1_09D7:
        mov    eax, dword ptr [ebp + 8]
        add    esp, 10h
        pop    ebp
        pop    edx
        pop    edi
        jecxz  loc_1_09E8
        sub    edx, ecx
        jbe    SHORT loc_1_0A11
        mov    dword ptr [edi], eax
loc_1_09E8:
        movzx  eax, word ptr [edi + 0ch]
        sub    eax, ebx
        mov    word ptr [edi + 0eh], ax
        mov    dword ptr [esp + 14h], eax
        or     ecx, ecx
        jmp    SHORT loc_1_0A18
loc_1_09FA:
        push   0
        push   1
        push   esi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_1_0A08:
        pop    ecx
        jmp    SHORT loc_1_09D7
loc_1_0A0B:
        jmp    SHORT loc_1_0A11
loc_1_0A0D:
        jmp    SHORT loc_1_0A11
loc_1_0A0F:
        jmp    SHORT loc_1_0A11
loc_1_0A11:
        mov    dword ptr [edi], 0
        stc
loc_1_0A18:
        popad
        ret
VDMAD_Scatter_Unlock:
        pushad
        movzx  eax, word ptr [edi + 8]
        or     eax, eax
        je     SHORT loc_1_0A30
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
loc_1_0A30:
        mov    esi, dword ptr [edi + 4]
        add    esi, eax
        mov    eax, dword ptr [esp + 1ch]
        and    al, 3
        cmp    al, 3
        je     SHORT loc_1_0A6B
        mov    ecx, dword ptr [edi]
        dec    ecx
        add    ecx, esi
        shr    esi, 0ch
        shr    ecx, 0ch
        sub    ecx, esi
        inc    ecx
        test   byte ptr [esp + 1ch], 4
        jne    SHORT loc_1_0A59
        call   sub_1_06E8
loc_1_0A59:
        push   0
        push   ecx
        push   esi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        sub    eax, 1
loc_1_0A69:
        popad
        ret
loc_1_0A6B:
        mov    ebp, esi
        and    ebp, 0fffff000h
        shr    esi, 0ch
        movzx  ecx, word ptr [edi + 0eh]
        lea    edi, [edi + 10h]
loc_1_0A7D:
        test   byte ptr [edi], 1
        je     SHORT loc_1_0A9D
        test   byte ptr [esp + 1ch], 4
        jne    SHORT loc_1_0A8D
        and    byte ptr [ebp], 0ffh
loc_1_0A8D:
        push   ecx
        push   0
        push   1
        push   esi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
loc_1_0A9D:
        inc    esi
        add    edi, 4
        add    ebp, 1000h
        loop   loc_1_0A7D
        clc
        jmp    SHORT loc_1_0A69
VDMAD_Request_Buffer:
        push   ecx
        push   esi
        add    ecx, 0fffh
        shr    ecx, 0ch
        mov    dl, 5
        xor    esi, esi
        db  0B8h, 03Ch, 020h, 000h, 000h
loc_1_0AC0:
        mov    ebx, dword ptr [eax]
        cmp    ecx, ebx
        ja     SHORT loc_1_0AD0
        test   byte ptr [eax + 0ch], 1
        je     SHORT loc_1_0AF4
        mov    dl, 6
        jmp    SHORT loc_1_0AE0
loc_1_0AD0:
        test   byte ptr [eax + 0ch], 1
        jne    SHORT loc_1_0AE0
        test   esi, esi
        je     SHORT loc_1_0ADE
        cmp    ebx, dword ptr [esi]
        jbe    SHORT loc_1_0AE0
loc_1_0ADE:
        mov    esi, eax
loc_1_0AE0:
        add    eax, 18h
        db  03Dh, 0FCh, 020h, 000h, 000h
        jb     SHORT loc_1_0AC0
        test   esi, esi
        jne    SHORT loc_1_0B37
        mov    al, 6
loc_1_0AF0:
        pop    esi
        pop    ecx
        stc
        ret
loc_1_0AF4:
        or     byte ptr [eax + 0ch], 1
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    dword ptr [eax + 10h], ebx
        mov    edx, dword ptr [eax + 4]
        mov    ebx, eax
        db  081h, 0EBh, 03Bh, 020h, 000h, 000h
        cmp    ebx, 1
        jne    SHORT loc_1_0B15
loc_1_0B11:
        pop    esi
        pop    ecx
        clc
        ret
loc_1_0B15:
        xor    esi, esi
        db  039h, 035h, 0FCh, 020h, 000h, 000h
        jne    SHORT loc_1_0B11
        mov    eax, 0ea60h
        db  0BEh, 0A5h, 00Bh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 0FCh, 020h, 000h, 000h
        jmp    SHORT loc_1_0B11
loc_1_0B37:
        pushad
        mov    eax, 4
        cmp    ecx, eax
        ja     SHORT loc_1_0B43
        mov    ecx, eax
loc_1_0B43:
        mov    edi, ecx
        mov    eax, ecx
        dec    eax
        bsr    cx, ax
        inc    cl
        mov    eax, 1
        shl    eax, cl
        dec    eax
        and    eax, 1fh
        push   edx
        mov    edx, esp
        push   0eh
        push   edx
        db  0FFh, 035h, 000h, 021h, 000h, 000h
        push   0
        push   eax
        push   0
        push   1
        push   edi
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_1_0B9C
        pop    dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [esi + 8], eax
        test   edx, edx
        je     SHORT loc_1_0B94
        push   0
        push   edx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_1_0B94:
        popad
        mov    eax, esi
        jmp    NEAR PTR loc_1_0AF4
loc_1_0B9C:
        pop    ecx
        popad
        mov    al, dl
        jmp    NEAR PTR loc_1_0AF0
loc_1_0BA5:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        xor    ecx, ecx
        xor    esi, esi
        db  0BBh, 054h, 020h, 000h, 000h
loc_1_0BB4:
        cmp    dword ptr [ebx], ecx
        je     SHORT loc_1_0BE9
        test   byte ptr [ebx + 0ch], 1
        jne    SHORT loc_1_0BE8
        mov    edx, eax
        sub    edx, dword ptr [ebx + 14h]
        cmp    edx, 1d4c0h
        jb     SHORT loc_1_0BE8
        pushad
        mov    edi, ebx
        push   0
        push   dword ptr [ebx + 8]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        mov    ecx, 6
        xor    eax, eax
        cld
        rep stosd
        popad
        jmp    SHORT loc_1_0BE9
loc_1_0BE8:
        inc    esi
loc_1_0BE9:
        add    ebx, 18h
        db  081h, 0FBh, 0FCh, 020h, 000h, 000h
        jb     SHORT loc_1_0BB4
        test   esi, esi
        je     SHORT loc_1_0C08
        mov    eax, 0ea60h
        db  0BEh, 0A5h, 00Bh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
loc_1_0C08:
        db  089h, 035h, 0FCh, 020h, 000h, 000h
        ret
VDMAD_Release_Buffer:
        push   eax
        push   ebx
        movzx  ebx, bl
        dec    ebx
        test   ebx, 7
        jne    SHORT loc_1_0C3E
        db  081h, 0C3h, 03Ch, 020h, 000h, 000h
        xor    eax, eax
        test   byte ptr [ebx + 0ch], 1
        je     SHORT loc_1_0C3E
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [ebx + 10h], eax
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [ebx + 14h], eax
        clc
loc_1_0C3B:
        pop    ebx
        pop    eax
        ret
loc_1_0C3E:
        stc
        jmp    SHORT loc_1_0C3B
sub_1_0C41:
        bt     dword ptr [esi + 6], ecx
        jb     SHORT loc_1_0C4D
        bt     dword ptr [esi + 5], ecx
        jb     SHORT loc_1_0CBE
loc_1_0C4D:
        call   sub_1_02DA
        mov    edi, eax
        call   sub_1_067C
        test   byte ptr [edi + 30h], 10h
        jne    SHORT loc_1_0CA9
sub_1_0C5F:
        mov    edx, dword ptr [edi + 28h]
        push   eax
        inc    edx
        inc    eax
        mov    ebx, 0ffffffh
        db  080h, 03Dh, 01Ah, 021h, 000h, 000h, 002h
        je     SHORT loc_1_0C76
        shr    ebx, 8
loc_1_0C76:
        and    eax, ebx
        and    edx, ebx
        pop    ebx
        cmp    eax, edx
        jae    SHORT loc_1_0CBE
        mov    dword ptr [edi + 28h], ebx
        test   byte ptr [edi + 31h], 40h
        je     SHORT loc_1_0C8C
        shl    eax, 1
        shl    edx, 1
loc_1_0C8C:
        add    dword ptr [edi + 20h], edx
        sub    dword ptr [edi + 20h], eax
        cmp    byte ptr [edi + 15h], 0
        je     SHORT loc_1_0CBE
        test   byte ptr [edi + 30h], 4
        je     SHORT loc_1_0CBE
        push   ecx
        push   esi
        call   sub_1_0CBF
        pop    esi
        pop    ecx
        jmp    SHORT loc_1_0CBE
loc_1_0CA9:
        mov    dword ptr [edi + 28h], eax
        sub    eax, dword ptr [edi + 2ch]
        neg    eax
        bt     dword ptr [esi], ecx
        jae    SHORT loc_1_0CB8
        shl    eax, 1
loc_1_0CB8:
        add    eax, dword ptr [edi + 24h]
        mov    dword ptr [edi + 20h], eax
loc_1_0CBE:
        ret
sub_1_0CBF:
        movzx  ebx, byte ptr [edi + 15h]
        mov    esi, dword ptr [edi + 0ch]
        mov    edi, dword ptr [edi + 10h]
        sub    edi, edx
        mov    ecx, edx
        sub    ecx, eax
        db  0CDh, 020h, 011h, 000h, 004h, 000h
        jae    SHORT loc_1_0CD7
loc_1_0CD7:
        ret
sub_1_0CD8:
        movzx  eax, bl
        dec    eax
        test   eax, 7
        jne    SHORT loc_1_0CFD
        db  005h, 03Ch, 020h, 000h, 000h
        push   edi
        add    edi, ecx
        jb     SHORT loc_1_0CF8
        dec    edi
        shr    edi, 0ch
        cmp    edi, dword ptr [eax]
        jae    SHORT loc_1_0CF8
        pop    edi
        clc
        ret
loc_1_0CF8:
        pop    edi
        mov    al, 0bh
        stc
        ret
loc_1_0CFD:
        mov    al, 0ah
        stc
        ret
VDMAD_Copy_To_Buffer:
        call   sub_1_0CD8
        jb     SHORT loc_1_0D2B
        pushad
        add    edi, dword ptr [eax + 8]
        jmp    SHORT loc_1_0D1B
VDMAD_Copy_From_Buffer:
        call   sub_1_0CD8
        jb     SHORT loc_1_0D2B
        pushad
        xchg   edi, esi
        add    esi, dword ptr [eax + 8]
loc_1_0D1B:
        cld
        mov    eax, ecx
        shr    ecx, 2
loc_1_0D21:
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_0D2A:
        popad
loc_1_0D2B:
        ret
VDMAD_Get_EISA_Adr_Mode:
        push   eax
        cmp    eax, 7
        jbe    SHORT loc_1_0D34
        mov    eax, dword ptr [eax]
loc_1_0D34:
        db  08Ah, 088h, 078h, 01Dh, 000h, 000h
        and    cl, 0ch
        shr    cl, 2
        pop    eax
        ret
VDMAD_Set_EISA_Adr_Mode:
        db  080h, 03Dh, 01Ah, 021h, 000h, 000h, 002h
        jne    SHORT loc_1_0D77
        push   eax
        push   ecx
        cmp    eax, 7
        jbe    SHORT loc_1_0D54
        mov    eax, dword ptr [eax]
loc_1_0D54:
        shl    cl, 2
        and    cl, 0ch
        db  088h, 088h, 078h, 01Dh, 000h, 000h
        imul   eax, eax, 34h
        db  005h, 06Ch, 01Eh, 000h, 000h
        mov    byte ptr [eax + 31h], 0
        cmp    cl, 4
        jne    SHORT loc_1_0D75
        mov    byte ptr [eax + 31h], 40h
loc_1_0D75:
        pop    ecx
        pop    eax
loc_1_0D77:
        ret
loc_1_0D78:
        db  0FFh, 025h, 00Ch, 021h, 000h, 000h
loc_1_0D7E:
        push   eax
        pushfd
        cli
        call   sub_1_0324
        db  022h, 005h, 013h, 020h, 000h, 000h
        db  022h, 025h, 01Bh, 020h, 000h, 000h
        or     al, ah
        je     SHORT loc_1_0DB6
        db  083h, 03Dh, 014h, 021h, 000h, 000h, 000h
        jne    SHORT loc_1_0DB6
        push   esi
        push   edx
        db  0BEh, 0BEh, 00Dh, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        db  089h, 035h, 014h, 021h, 000h, 000h
        pop    edx
        pop    esi
loc_1_0DB6:
        popfd
        pop    eax
        db  0FFh, 025h, 008h, 021h, 000h, 000h
loc_1_0DBE:
        pushfd
        cli
        xor    eax, eax
        db  0A3h, 014h, 021h, 000h, 000h
        db  086h, 005h, 018h, 021h, 000h, 000h
        db  086h, 025h, 019h, 021h, 000h, 000h
        popfd
        push   eax
        db  0BEh, 00Ch, 020h, 000h, 000h
        call   sub_1_0DE9
        pop    eax
        mov    al, ah
        db  0BEh, 014h, 020h, 000h, 000h
        jmp    SHORT sub_1_0DE9
sub_1_0DE9:
        and    al, byte ptr [esi + 7]
        je     SHORT loc_1_0E2C
        movzx  eax, al
loc_1_0DF1:
        bsf    ecx, eax
        je     SHORT loc_1_0E2C
        btr    eax, ecx
        push   eax
        call   sub_1_02DA
        bts    dword ptr [esi + 5], ecx
        cmp    ebx, dword ptr [eax + 8]
        jne    SHORT loc_1_0E10
        call   sub_1_0FE9
        pop    eax
        jmp    SHORT loc_1_0DF1
loc_1_0E10:
        pushad
        mov    edx, eax
        mov    eax, 400000h
        mov    ebx, dword ptr [edx + 8]
        xor    ecx, ecx
        db  0BEh, 0E7h, 00Fh, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        popad
        pop    eax
        jmp    SHORT loc_1_0DF1
loc_1_0E2C:
        ret
loc_1_0E2D:
        db  0CDh, 020h, 012h, 080h, 004h, 000h
VDMAD_Default_Handler:
        mov    ecx, dword ptr [eax]
        db  0CDh, 020h, 004h, 000h, 004h, 000h
        test   dl, 2
        jne    SHORT loc_1_0E49
        test   dl, 1
        jne    NEAR PTR loc_1_0F0C
loc_1_0E49:
        cmp    byte ptr [eax + 14h], 0
        jne    SHORT loc_1_0E55
        cmp    byte ptr [eax + 15h], 0
        je     SHORT loc_1_0E7C
loc_1_0E55:
        cmp    esi, dword ptr [eax + 0ch]
        jne    SHORT loc_1_0E63
        cmp    ecx, dword ptr [eax + 10h]
        je     NEAR PTR loc_1_0EF9
loc_1_0E63:
        cmp    byte ptr [eax + 15h], 0
        jne    SHORT loc_1_0E77
        test   byte ptr [eax + 30h], 8
        je     SHORT loc_1_0E70
        stc
loc_1_0E70:
        call   sub_1_0F64
        jmp    SHORT loc_1_0E7C
loc_1_0E77:
        call   sub_1_0FCC
loc_1_0E7C:
        mov    edi, dword ptr [eax]
        db  00Fh, 0A3h, 03Dh, 010h, 021h, 000h, 000h
        jb     SHORT loc_1_0EC9
        mov    edi, esi
        sub    edi, dword ptr [ebx + 4]
        cmp    edi, 0a0000h
        jae    SHORT loc_1_0EC9
        push   eax
        lea    edi, [ecx + edi]
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        shl    eax, 0ch
        cmp    edi, eax
        pop    eax
        jbe    SHORT loc_1_0EC9
        test   byte ptr [eax + 30h], 10h
        je     SHORT loc_1_0ED0
        call   sub_1_0F12
        jae    SHORT loc_1_0EF9
        db  0E8h, 048h, 0F1h, 0FEh, 0FFh
        jb     SHORT loc_1_0EC1
        call   sub_1_0F12
        jae    SHORT loc_1_0EF9
loc_1_0EC1:
        mov    ecx, dword ptr [eax]
        db  0CDh, 020h, 02Fh, 000h, 001h, 000h
loc_1_0EC9:
        call   sub_1_0F12
        jae    SHORT loc_1_0EF9
loc_1_0ED0:
        call   sub_1_0F9A
        test   dl, 8
        je     SHORT loc_1_0EEC
        pushad
        xor    edi, edi
        db  0CDh, 020h, 002h, 000h, 004h, 000h
        db  0CDh, 020h, 010h, 000h, 004h, 000h
        popad
        jmp    SHORT loc_1_0EF9
loc_1_0EEC:
        push   eax
        movzx  eax, byte ptr [eax + 15h]
        db  083h, 088h, 047h, 020h, 000h, 000h, 002h
        pop    eax
loc_1_0EF9:
        db  0CDh, 020h, 006h, 000h, 004h, 000h
        test   dl, 1
        jne    SHORT loc_1_0F11
        db  0CDh, 020h, 008h, 000h, 004h, 000h
        jmp    SHORT loc_1_0F11
loc_1_0F0C:
        call   sub_1_0FE9
loc_1_0F11:
        ret
sub_1_0F12:
        pushad
        mov    edx, dword ptr [eax]
        db  00Fh, 0A3h, 015h, 010h, 021h, 000h, 000h
        mov    edx, esi
        jb     SHORT loc_1_0F56
        xor    edx, edx
        db  0F6h, 005h, 01Ah, 021h, 000h, 000h, 003h
        jne    SHORT loc_1_0F36
        mov    edx, dword ptr [eax]
        cmp    dl, 4
        mov    dl, 1
        jb     SHORT loc_1_0F36
        shl    edx, 1
loc_1_0F36:
        db  0CDh, 020h, 009h, 000h, 004h, 000h
        jb     SHORT loc_1_0F62
        push   ecx
        add    ecx, edx
        shr    ecx, 0ch
        db  03Bh, 00Dh, 000h, 021h, 000h, 000h
        pop    ecx
        jb     SHORT loc_1_0F56
        db  0CDh, 020h, 00Ah, 000h, 004h, 000h
        stc
        jmp    SHORT loc_1_0F62
loc_1_0F56:
        mov    ebx, 0ff00h
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        clc
loc_1_0F62:
        popad
        ret
sub_1_0F64:
        pushad
        pushfd
        mov    edx, dword ptr [eax]
        db  00Fh, 0A3h, 015h, 010h, 021h, 000h, 000h
        jb     SHORT loc_1_0F81
        db  0CDh, 020h, 002h, 000h, 004h, 000h
        popfd
        pushfd
        jb     SHORT loc_1_0F92
        db  0CDh, 020h, 00Ah, 000h, 004h, 000h
loc_1_0F81:
        popfd
        xor    ebx, ebx
        xor    esi, esi
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        popad
        ret
loc_1_0F92:
        db  0CDh, 020h, 017h, 000h, 004h, 000h
        jmp    SHORT loc_1_0F81
sub_1_0F9A:
        pushad
loc_1_0F9B:
        db  0CDh, 020h, 00Eh, 000h, 004h, 000h
        jae    SHORT loc_1_0FC0
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        cmp    al, 5
        db  00Fh, 084h, 0E0h, 0F0h, 0FEh, 0FFh
        push   eax
        mov    eax, 64h
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        pop    eax
        jmp    SHORT loc_1_0F9B
loc_1_0FC0:
        mov    eax, dword ptr [esp + 1ch]
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        popad
        ret
sub_1_0FCC:
        pushad
        movzx  ebx, byte ptr [eax + 15h]
        db  0CDh, 020h, 00Fh, 000h, 004h, 000h
        xor    ebx, ebx
        xor    esi, esi
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        popad
        ret
loc_1_0FE7:
        mov    eax, edx
sub_1_0FE9:
        cmp    dword ptr [eax + 8], 0
        je     SHORT loc_1_1055
        push   edx
        mov    edx, 0ffffffh
        db  080h, 03Dh, 01Ah, 021h, 000h, 000h, 002h
        je     SHORT loc_1_1001
        shr    edx, 8
loc_1_1001:
        cmp    byte ptr [eax + 15h], 0
        je     SHORT loc_1_1039
        pushad
        movzx  ecx, byte ptr [eax + 15h]
        db  0F6h, 081h, 047h, 020h, 000h, 000h, 002h
        je     SHORT loc_1_102E
        mov    ecx, dword ptr [eax]
        db  0BEh, 00Ch, 020h, 000h, 000h
        btr    ecx, 2
        jae    SHORT loc_1_1025
        add    esi, 8
loc_1_1025:
        mov    edi, eax
        mov    eax, edx
        call   sub_1_0C5F
loc_1_102E:
        popad
        mov    dword ptr [eax + 28h], edx
        call   sub_1_0FCC
        jmp    SHORT loc_1_104E
loc_1_1039:
        mov    dword ptr [eax + 28h], edx
        cmp    byte ptr [eax + 14h], 0
        je     SHORT loc_1_104E
        test   byte ptr [eax + 30h], 8
        je     SHORT loc_1_1049
        stc
loc_1_1049:
        call   sub_1_0F64
loc_1_104E:
        db  0CDh, 020h, 007h, 000h, 004h, 000h
        pop    edx
loc_1_1055:
        ret
        db  000h, 000h
loc_1_1058:
        db  0A0h, 026h, 021h, 000h, 000h
        mov    byte ptr [47bh], al
        db  0A1h, 028h, 021h, 000h, 000h
        inc    eax
        je     SHORT loc_1_1070
        dec    eax
        mov    dword ptr [12ch], eax
loc_1_1070:
        ret
loc_1_1071:
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ah, 81h
        jne    SHORT loc_1_1081
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        jmp    SHORT loc_1_10D4
loc_1_1081:
        db  0F6h, 005h, 027h, 021h, 000h, 000h, 008h
        jne    SHORT loc_1_1090
        db  0CDh, 020h, 047h, 080h, 001h, 000h
loc_1_1090:
        db  00Fh, 0B7h, 00Dh, 024h, 021h, 000h, 000h
        db  08Bh, 015h, 020h, 021h, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_1_10A3:
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ah, 81h
        jne    SHORT loc_1_10B3
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        jmp    SHORT loc_1_10D4
loc_1_10B3:
        db  0F6h, 005h, 027h, 021h, 000h, 000h, 008h
        jne    SHORT loc_1_10C2
        db  0CDh, 020h, 047h, 080h, 001h, 000h
loc_1_10C2:
        db  08Bh, 00Dh, 028h, 021h, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_1_10D4:
        cmp    al, 0ch
        ja     SHORT loc_1_10EE
        sub    al, 2
        jb     SHORT loc_1_10EE
        movzx  eax, al
        shl    eax, 2
        and    word ptr [ebp + 2ch], 0fffeh
        db  02Eh, 0FFh, 0A0h, 0F8h, 010h, 000h, 000h
loc_1_10EE:
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1ch], 0fh
        ret
loc_1_10F8:
        xor    al, 11h
        add    byte ptr [eax], al
        lea    edx, [ecx]
        add    byte ptr [eax], al
        pushad
        adc    al, byte ptr [eax]
        db  000h, 0B2h, 012h, 000h, 000h, 025h
        adc    eax, dword ptr [eax]
        db  000h, 068h, 013h
        add    byte ptr [eax], al
        lodsb
        adc    eax, dword ptr [eax]
        db  000h, 0EEh
        adc    eax, dword ptr [eax]
        db  000h, 028h
        adc    al, 0
        db  000h, 062h, 014h
        add    byte ptr [eax], al
        xchg   edx, eax
        adc    al, 0
        add    dh, al
        inc    ebp
        sbb    al, 10h
loc_1_1128:
        or     word ptr [ebp + 2ch], 1
        ret
loc_1_112E:
        mov    byte ptr [ebp + 1ch], 7
        jmp    SHORT loc_1_1128
loc_1_1134:
        test   word ptr [ebp + 14h], 0ffffh
        db  075h, 0E8h
        mov    word ptr [ebp + 1ch], 100h
        mov    word ptr [ebp + 10h], 3
        mov    word ptr [ebp + 18h], 200h
        db  08Bh, 015h, 03Ch, 020h, 000h, 000h
        mov    eax, 1fe00h
        shl    edx, 0ch
        cmp    eax, edx
        ja     SHORT loc_1_1162
        mov    eax, edx
loc_1_1162:
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 4], ax
        xor    eax, eax
        db  080h, 03Dh, 01Ah, 021h, 000h, 000h, 004h
        jne    SHORT loc_1_1179
        inc    eax
loc_1_1179:
        db  081h, 03Dh, 000h, 021h, 000h, 000h, 000h, 001h, 000h, 000h
        ja     SHORT loc_1_1187
        or     al, 2
loc_1_1187:
        mov    word ptr [ebp + 14h], ax
        clc
        ret
loc_1_118D:
        test   word ptr [ebp + 14h], 0ffc1h
        db  075h, 08Fh
        call   sub_1_14C2
        jb     SHORT loc_1_112E
        mov    ecx, dword ptr [edi]
        jecxz  loc_1_11FF
        test   dword ptr [ebx], 20h
        jne    SHORT loc_1_11DC
        movzx  eax, word ptr [edi + 8]
        shl    eax, 4
        add    eax, dword ptr [edi + 4]
        mov    edx, eax
        shr    edx, 0ch
        cmp    dx, 0a0h
        jae    SHORT loc_1_11DC
        push   eax
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        cmp    edx, eax
        pop    eax
        jbe    SHORT loc_1_11DC
        mov    edx, eax
        add    eax, ecx
        cmp    eax, 0a0000h
        jbe    SHORT loc_1_11DC
        mov    ecx, 0a0000h
        sub    ecx, edx
loc_1_11DC:
        mov    word ptr [edi + 0ah], 0
        mov    dword ptr [edi + 0ch], 0
        mov    dl, byte ptr [ebp + 14h]
        and    dl, 30h
        shr    dl, 4
        db  0CDh, 020h, 009h, 000h, 004h, 000h
        jb     SHORT loc_1_122D
loc_1_11FA:
        mov    dword ptr [edi + 0ch], edx
        jmp    SHORT loc_1_125E
loc_1_11FF:
        push   eax
        mov    edx, esp
        mov    ecx, esi
        shr    ecx, 0ch
        push   0
        push   edx
        push   1
        push   ecx
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        pop    edx
        xor    ecx, ecx
        test   dl, 1
        je     SHORT loc_1_122D
        and    dx, 0f000h
        and    esi, 0fffh
        or     edx, esi
        jmp    SHORT loc_1_11FA
loc_1_122D:
        xchg   dword ptr [edi], ecx
        test   byte ptr [ebp + 14h], 4
        jne    SHORT loc_1_1256
        db  0CDh, 020h, 00Eh, 000h, 004h, 000h
        jb     SHORT loc_1_1256
        mov    dword ptr [edi], ecx
        mov    word ptr [edi + 0ah], bx
        mov    dword ptr [edi + 0ch], edx
        test   byte ptr [ebp + 14h], 2
        je     SHORT loc_1_125E
        xor    edi, edi
        db  0CDh, 020h, 010h, 000h, 004h, 000h
        jae    SHORT loc_1_125E
loc_1_1256:
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1ch], al
loc_1_125E:
        clc
        ret
loc_1_1260:
        test   word ptr [ebp + 14h], 0fffdh
        db  00Fh, 085h, 0B8h, 0FEh, 0FFh, 0FFh
        call   sub_1_14C2
        jb     NEAR PTR loc_1_112E
        mov    ecx, dword ptr [edi]
        movzx  ebx, word ptr [edi + 0ah]
        or     ebx, ebx
        je     SHORT loc_1_129F
        test   byte ptr [ebp + 14h], 2
        je     SHORT loc_1_1291
        xor    edi, edi
        db  0CDh, 020h, 011h, 000h, 004h, 000h
        jb     SHORT loc_1_1299
loc_1_1291:
        db  0CDh, 020h, 00Fh, 000h, 004h, 000h
        jae    SHORT loc_1_12B0
loc_1_1299:
        mov    byte ptr [ebp + 1ch], 0ah
        jmp    SHORT loc_1_12AB
loc_1_129F:
        db  0CDh, 020h, 00Ah, 000h, 004h, 000h
        jae    SHORT loc_1_12B0
        mov    byte ptr [ebp + 1ch], 8
loc_1_12AB:
        or     word ptr [ebp + 2ch], 1
loc_1_12B0:
        clc
        ret
loc_1_12B2:
        test   word ptr [ebp + 14h], 0ff3fh
        db  00Fh, 085h, 066h, 0FEh, 0FFh, 0FFh
        call   sub_1_14C2
        jb     NEAR PTR loc_1_112E
        xchg   dword ptr [edi + 4], esi
        xor    ecx, ecx
        xchg   word ptr [edi + 8], cx
        mov    al, byte ptr [ebp + 14h]
        and    al, 0c0h
        shr    al, 6
        push   esi
        db  0CDh, 020h, 00Bh, 000h, 004h, 000h
        jbe    SHORT loc_1_12F0
        or     al, 4
        db  0CDh, 020h, 00Ch, 000h, 004h, 000h
        mov    word ptr [edi + 0eh], dx
        stc
loc_1_12F0:
        pop    dword ptr [edi + 4]
        mov    word ptr [edi + 8], cx
        jb     SHORT loc_1_130C
        test   word ptr [ebp + 14h], 40h
        je     SHORT loc_1_1323
        and    si, 0fffh
        mov    word ptr [ebp + 10h], si
        jmp    SHORT loc_1_1323
loc_1_130C:
        or     word ptr [ebp + 2ch], 1
        movzx  edx, word ptr [edi + 0eh]
        mov    byte ptr [ebp + 1ch], 3
        cmp    dx, word ptr [edi + 0ch]
        jbe    SHORT loc_1_1323
        mov    byte ptr [ebp + 1ch], 9
loc_1_1323:
        clc
        ret
loc_1_1325:
        test   word ptr [ebp + 14h], 0ff3fh
        db  00Fh, 085h, 0F3h, 0FDh, 0FFh, 0FFh
        call   sub_1_14C2
        jb     NEAR PTR loc_1_112E
        xchg   dword ptr [edi + 4], esi
        xor    cx, cx
        xchg   word ptr [edi + 8], cx
        mov    al, byte ptr [ebp + 14h]
        and    al, 0c0h
        shr    al, 6
        db  0CDh, 020h, 00Ch, 000h, 004h, 000h
        mov    word ptr [edi + 8], cx
        mov    dword ptr [edi + 4], esi
        jae    SHORT loc_1_1366
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1ch], 8
loc_1_1366:
        clc
        ret
loc_1_1368:
        test   word ptr [ebp + 14h], 0fffdh
        db  00Fh, 085h, 0B0h, 0FDh, 0FFh, 0FFh
        call   sub_1_14C2
        jb     NEAR PTR loc_1_112E
        mov    ecx, dword ptr [edi]
        db  0CDh, 020h, 00Eh, 000h, 004h, 000h
        jb     SHORT loc_1_13A2
        mov    word ptr [edi + 0ah], bx
        mov    dword ptr [edi + 0ch], edx
        test   word ptr [ebp + 14h], 2
        je     SHORT loc_1_13AA
        xor    edi, edi
        db  0CDh, 020h, 010h, 000h, 004h, 000h
        jae    SHORT loc_1_13AA
loc_1_13A2:
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1ch], al
loc_1_13AA:
        clc
        ret
loc_1_13AC:
        test   word ptr [ebp + 14h], 0fffdh
        db  00Fh, 085h, 06Ch, 0FDh, 0FFh, 0FFh
        call   sub_1_14C2
        jb     NEAR PTR loc_1_112E
        movzx  ebx, word ptr [edi + 0ah]
        test   word ptr [ebp + 14h], 2
        je     SHORT loc_1_13DB
        mov    ecx, dword ptr [edi]
        xor    edi, edi
        db  0CDh, 020h, 011h, 000h, 004h, 000h
        jb     SHORT loc_1_13E3
loc_1_13DB:
        db  0CDh, 020h, 00Fh, 000h, 004h, 000h
        jae    SHORT loc_1_13EC
loc_1_13E3:
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1ch], 0ah
loc_1_13EC:
        clc
        ret
loc_1_13EE:
        test   word ptr [ebp + 14h], 0ffffh
        db  00Fh, 085h, 02Ah, 0FDh, 0FFh, 0FFh
        call   sub_1_14C2
        jb     NEAR PTR loc_1_112E
        movzx  ebx, word ptr [edi + 0ah]
        mov    ecx, dword ptr [edi]
        mov    di, word ptr [ebp + 10h]
        shl    edi, 10h
        mov    di, word ptr [ebp + 18h]
        db  0CDh, 020h, 010h, 000h, 004h, 000h
        jae    SHORT loc_1_1426
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1ch], al
loc_1_1426:
        clc
        ret
loc_1_1428:
        test   word ptr [ebp + 14h], 0ffffh
        db  00Fh, 085h, 0F0h, 0FCh, 0FFh, 0FFh
        call   sub_1_14C2
        jb     NEAR PTR loc_1_112E
        movzx  ebx, word ptr [edi + 0ah]
        mov    ecx, dword ptr [edi]
        mov    di, word ptr [ebp + 10h]
        shl    edi, 10h
        mov    di, word ptr [ebp + 18h]
        db  0CDh, 020h, 011h, 000h, 004h, 000h
        jae    SHORT loc_1_1460
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1ch], al
loc_1_1460:
        clc
        ret
loc_1_1462:
        test   word ptr [ebp + 14h], 0ffffh
        db  00Fh, 085h, 0B6h, 0FCh, 0FFh, 0FFh
        movzx  eax, word ptr [ebp + 10h]
        call   sub_1_02CA
        jb     SHORT loc_1_1487
        db  0CDh, 020h, 013h, 000h, 004h, 000h
        jae    SHORT loc_1_1490
        mov    byte ptr [ebp + 1ch], 0dh
        jmp    SHORT loc_1_148B
loc_1_1487:
        mov    byte ptr [ebp + 1ch], 0ch
loc_1_148B:
        or     word ptr [ebp + 2ch], 1
loc_1_1490:
        clc
        ret
loc_1_1492:
        test   word ptr [ebp + 14h], 0ffffh
        db  00Fh, 085h, 086h, 0FCh, 0FFh, 0FFh
        movzx  eax, word ptr [ebp + 10h]
        call   sub_1_02CA
        jb     SHORT loc_1_14B7
        db  0CDh, 020h, 014h, 000h, 004h, 000h
        jae    SHORT loc_1_14C0
        mov    byte ptr [ebp + 1ch], 0eh
        jmp    SHORT loc_1_14BB
loc_1_14B7:
        mov    byte ptr [ebp + 1ch], 0ch
loc_1_14BB:
        or     word ptr [ebp + 2ch], 1
loc_1_14C0:
        clc
        ret
sub_1_14C2:
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        mov    esi, dword ptr [edi + 4]
        movzx  eax, word ptr [edi + 8]
        or     eax, eax
        je     SHORT loc_1_1506
        test   dword ptr [ebp + 2ch], 20000h
        jne    SHORT loc_1_14F0
        push   ecx
        push   esi
        add    esi, dword ptr [edi]
        dec    esi
        lsl    ecx, eax
        cmp    ecx, esi
        pop    esi
        pop    ecx
        jb     SHORT loc_1_1507
loc_1_14F0:
        push   dword ptr [ebp + 38h]
        mov    word ptr [ebp + 38h], ax
        mov    ax, 38ffh
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        add    esi, eax
        pop    dword ptr [ebp + 38h]
loc_1_1506:
        clc
loc_1_1507:
        ret
sub_1_1508:
        db  0BEh, 00Ch, 020h, 000h, 000h
        cmp    dx, 0c0h
        jb     SHORT loc_1_1517
        add    esi, 8
loc_1_1517:
        cmc
        ret
sub_1_1519:
        db  0BEh, 00Ch, 020h, 000h, 000h
        push   eax
        db  0BFh, 02Ch, 021h, 000h, 000h
        mov    ecx, 8
        mov    al, dl
        cld
        repne scasb
        pop    eax
        jne    SHORT loc_1_1545
        db  081h, 0EFh, 02Dh, 021h, 000h, 000h
        mov    ecx, edi
        cmp    cl, 4
        jb     SHORT loc_1_1544
        sub    cl, 4
        add    esi, 8
loc_1_1544:
        ret
loc_1_1545:
        ret
sub_1_1546:
        db  0BEh, 00Ch, 020h, 000h, 000h
        mov    cx, dx
        cmp    cx, 0fh
        jbe    SHORT loc_1_155F
        sub    cx, 0c0h
        shr    cx, 1
        add    esi, 8
loc_1_155F:
        shr    cl, 1
        cmp    cl, 4
        jb     SHORT loc_1_1568
        xor    ecx, ecx
loc_1_1568:
        ret
sub_1_1569:
        db  08Bh, 03Dh, 068h, 01Eh, 000h, 000h
        lea    edi, [ebx + edi + 8]
        db  081h, 0FEh, 00Ch, 020h, 000h, 000h
        je     SHORT loc_1_157C
        inc    edi
loc_1_157C:
        ret
loc_1_157D:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_158E
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_158E:
        je     SHORT loc_1_159B
        call   sub_1_1508
        xor    al, al
        xchg   byte ptr [esi + 4], al
        ret
loc_1_159B:
        or     al, al
        je     SHORT loc_1_15A5
        db  0CDh, 020h, 02Fh, 000h, 001h, 000h
loc_1_15A5:
        ret
loc_1_15A6:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_15B7
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_15B7:
        jb     NEAR PTR loc_1_1912
        call   sub_1_1508
        xor    al, al
        mov    byte ptr [esi + 4], al
        mov    byte ptr [esi + 6], al
        call   sub_1_1569
        mov    byte ptr [edi], al
        xor    ecx, ecx
        call   sub_1_02DA
        mov    cl, 4
loc_1_15DA:
        mov    byte ptr [eax + 30h], 40h
        and    byte ptr [eax + 31h], 40h
        add    eax, 34h
        loop   loc_1_15DA
        movzx  eax, byte ptr [esi + 5]
        mov    byte ptr [esi + 5], 0fh
        call   sub_1_1914
        ret
loc_1_15F5:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_1606
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_1606:
        jb     NEAR PTR loc_1_1912
        call   sub_1_1508
        xchg   byte ptr [esi + 5], al
        movzx  eax, al
        call   sub_1_1914
        ret
loc_1_161D:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_162E
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_162E:
        jb     NEAR PTR loc_1_1912
        call   sub_1_1508
        movzx  eax, byte ptr [esi + 5]
        mov    byte ptr [esi + 5], 0
        call   sub_1_1914
        ret
loc_1_1647:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_1658
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_1658:
        jb     NEAR PTR loc_1_1912
        mov    ah, al
        and    ah, 0c0h
        test   al, 20h
        je     SHORT loc_1_166D
        db  0CDh, 020h, 02Fh, 000h, 001h, 000h
loc_1_166D:
        xor    ah, ah
        call   sub_1_1508
        movzx  ecx, al
        and    ecx, 3
        xchg   edi, eax
        call   sub_1_02DA
        xchg   edi, eax
        and    al, 0fch
        mov    byte ptr [edi + 30h], al
        mov    ah, byte ptr [edi + 31h]
        bt     dword ptr [esi], ecx
        jae    SHORT loc_1_1693
        or     ah, 40h
        jmp    SHORT loc_1_1696
loc_1_1693:
        and    ah, 0bfh
loc_1_1696:
        and    ah, 0f2h
        test   al, 4
        je     SHORT loc_1_16A2
        or     ah, 0ch
        jmp    SHORT loc_1_16A9
loc_1_16A2:
        test   al, 8
        je     SHORT loc_1_16A9
        or     ah, 4
loc_1_16A9:
        mov    dl, al
        and    dl, 10h
        shr    dl, 3
        and    ah, 0fdh
        or     ah, dl
        mov    byte ptr [edi + 31h], ah
        xor    edx, edx
        jmp    NEAR PTR sub_1_1932
loc_1_16C0:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_16D1
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_16D1:
        jb     NEAR PTR loc_1_1912
        call   sub_1_1508
        movzx  ecx, al
        and    cl, 3
        shr    al, 2
        and    al, 1
        mov    ah, 0feh
        shl    al, cl
        rol    ah, cl
        and    ah, byte ptr [esi + 6]
        or     al, ah
        mov    byte ptr [esi + 6], al
        or     edx, 1
        jmp    NEAR PTR sub_1_1932
loc_1_16FD:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_170E
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_170E:
        jb     NEAR PTR loc_1_1912
        call   sub_1_1508
        movzx  ecx, al
        and    cl, 3
        shr    al, 2
        and    al, 1
        mov    ah, 0feh
        shl    al, cl
        rol    ah, cl
        and    ah, byte ptr [esi + 5]
        or     al, ah
        mov    byte ptr [esi + 5], al
        or     edx, 1
        jmp    NEAR PTR sub_1_1932
loc_1_173A:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_174B
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_174B:
        jb     NEAR PTR loc_1_1912
        call   sub_1_1508
        call   sub_1_1569
        mov    byte ptr [edi], 0
        ret
sub_1_175F:
        pushad
        mov    bl, ah
        movzx  eax, al
        mov    edx, 0ffffff00h
        bt     dword ptr [esi], ecx
        jae    SHORT loc_1_1788
        cmp    bl, 2
        je     SHORT loc_1_1783
        ja     SHORT loc_1_1788
        shl    eax, 1
        rol    edx, 1
        or     bl, bl
        jne    SHORT loc_1_1788
        and    dl, 0feh
        jmp    SHORT loc_1_1788
loc_1_1783:
        and    al, 0feh
        or     dl, 1
loc_1_1788:
        push   eax
        call   sub_1_02DA
        lea    esi, [eax + 20h]
        pop    eax
        mov    cl, bl
        shl    cl, 3
        shl    eax, cl
        rol    edx, cl
        and    edx, dword ptr [esi]
        or     edx, eax
        cmp    bl, 3
        je     SHORT loc_1_17B3
        db  080h, 03Dh, 01Ah, 021h, 000h, 000h, 002h
        jne    SHORT loc_1_17B3
        and    edx, 0ffffffh
loc_1_17B3:
        mov    dword ptr [esi], edx
        mov    dword ptr [esi + 4], edx
        popad
        ret
loc_1_17BA:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_17CB
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_17CB:
        jb     SHORT loc_1_17EC
        call   sub_1_1546
        call   sub_1_1569
        mov    ah, byte ptr [edi]
        call   sub_1_175F
        xor    ah, 1
        mov    byte ptr [edi], ah
        jne    SHORT loc_1_1835
        xor    edx, edx
        jmp    NEAR PTR sub_1_1932
loc_1_17EC:
        call   sub_1_1546
        call   sub_1_1569
        inc    edx
        cmp    edx, 0fh
        jbe    SHORT loc_1_17FD
        inc    edx
loc_1_17FD:
        cmp    byte ptr [edi], 0
        jne    SHORT loc_1_180B
        push   ebx
        push   edi
        call   sub_1_0C41
        pop    edi
        pop    ebx
loc_1_180B:
        call   sub_1_02DA
        test   word ptr [eax + 1ah], 1
        je     SHORT loc_1_1822
        mov    edx, 3
        call   sub_1_1932
loc_1_1822:
        lea    edx, [eax + 20h]
        movzx  eax, byte ptr [edi]
        xor    byte ptr [edi], 1
        mov    eax, dword ptr [edx + eax]
        bt     dword ptr [esi], ecx
        jae    SHORT loc_1_1835
        shr    eax, 1
loc_1_1835:
        ret
loc_1_1836:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_1847
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_1847:
        jb     SHORT loc_1_1889
        call   sub_1_1546
        call   sub_1_1569
        push   eax
        call   sub_1_02DA
        lea    edx, [eax + 28h]
        pop    eax
        push   ecx
        movzx  ecx, byte ptr [edi]
        xor    byte ptr [edi], 1
        mov    byte ptr [ecx + edx], al
        mov    byte ptr [ecx + edx + 4], al
        db  080h, 03Dh, 01Ah, 021h, 000h, 000h, 002h
        jne    SHORT loc_1_187C
        mov    byte ptr [edx + 2], 0
        mov    byte ptr [edx + 6], 0
loc_1_187C:
        pop    ecx
        cmp    byte ptr [edi], 0
        jne    SHORT loc_1_18C4
        xor    edx, edx
        jmp    NEAR PTR sub_1_1932
loc_1_1889:
        call   sub_1_1546
        call   sub_1_1569
        cmp    byte ptr [edi], 0
        jne    SHORT loc_1_18A1
        push   ebx
        push   edi
        call   sub_1_0C41
        pop    edi
        pop    ebx
loc_1_18A1:
        call   sub_1_02DA
        test   word ptr [eax + 1ah], 1
        je     SHORT loc_1_18B8
        mov    edx, 1
        call   sub_1_1932
loc_1_18B8:
        lea    edx, [eax + 28h]
        movzx  ecx, byte ptr [edi]
        xor    byte ptr [edi], 1
        mov    al, byte ptr [edx + ecx]
loc_1_18C4:
        ret
loc_1_18C5:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_18D6
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_18D6:
        je     SHORT loc_1_1902
        call   sub_1_1519
        call   sub_1_02DA
        test   word ptr [eax + 1ah], 1
        je     SHORT loc_1_18F6
        push   edx
        mov    edx, 2
        call   sub_1_1932
        pop    edx
loc_1_18F6:
        movzx  eax, word ptr [eax + 22h]
        bt     dword ptr [esi], ecx
        jae    SHORT loc_1_1901
        and    al, 0feh
loc_1_1901:
        ret
loc_1_1902:
        call   sub_1_1519
        mov    ah, 2
        call   sub_1_175F
        xor    edx, edx
        jmp    SHORT sub_1_1932
loc_1_1912:
        in     al, dx
        ret
sub_1_1914:
        movzx  edx, byte ptr [esi + 5]
        xor    eax, edx
        mov    edx, 1
        mov    ecx, 3
loc_1_1924:
        bt     ax, cx
        jae    SHORT loc_1_192F
        call   sub_1_1932
loc_1_192F:
        loop   loc_1_1924
        ret
sub_1_1932:
        pushad
        call   sub_1_02DA
        cmp    dword ptr [eax + 4], 0
        je     SHORT loc_1_1950
        or     edx, edx
        jne    SHORT loc_1_194B
        movzx  edx, byte ptr [esi + 5]
        bt     edx, ecx
        jb     SHORT loc_1_1950
loc_1_194B:
        xor    ecx, ecx
        call   dword ptr [eax + 4]
loc_1_1950:
        popad
        ret
loc_1_1952:
        pushfd
        push   eax
        cli
        call   sub_1_0324
        pop    eax
        popfd
        ret
loc_1_195D:
        pushfd
        pushad
        pushfd
        cli
        call   sub_1_0324
        xor    eax, eax
        db  086h, 005h, 018h, 021h, 000h, 000h
        db  086h, 025h, 019h, 021h, 000h, 000h
        popfd
        push   eax
        db  0BEh, 00Ch, 020h, 000h, 000h
        call   sub_1_0DE9
        pop    eax
        mov    al, ah
        db  0BEh, 014h, 020h, 000h, 000h
        call   sub_1_0DE9
        popad
        popfd
        ret
sub_1_1990:
        or     al, 70h
        out    18h, al
        jmp    SHORT loc_1_1996
loc_1_1996:
        in     al, 1ah
        mov    byte ptr [esi + 31h], al
        push   ecx
        mov    cl, al
        and    cl, 2
        shl    cl, 3
        and    ah, 0efh
        or     ah, cl
        pop    ecx
        test   al, 4
        je     SHORT loc_1_19BA
        test   al, 8
        jne    SHORT loc_1_19B7
        or     ah, 8
        jmp    SHORT loc_1_19BA
loc_1_19B7:
        or     ah, 4
loc_1_19BA:
        mov    byte ptr [esi + 30h], ah
        ret
loc_1_19BE:
        mov    ecx, 8
        xor    edi, edi
        db  0BEh, 06Ch, 01Eh, 000h, 000h
loc_1_19CA:
        mov    eax, edi
        call   sub_1_1990
        mov    eax, edi
        or     al, 30h
        out    18h, al
        jmp    SHORT loc_1_19D9
loc_1_19D9:
        in     al, 1ah
        ror    eax, 8
        in     al, 1ah
        ror    eax, 8
        in     al, 1ah
        ror    eax, 10h
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        mov    eax, edi
        or     al, 50h
        out    18h, al
        jmp    SHORT loc_1_19F6
loc_1_19F6:
        in     al, 1ah
        shl    ax, 8
        in     al, 1ah
        xchg   al, ah
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [esi + 2ch], eax
        inc    edi
        add    esi, 34h
        loop   loc_1_19CA
        clc
        ret
loc_1_1A0E:
        pushad
        mov    ch, dl
        mov    cl, dh
        and    cl, 2
        shl    cl, 3
        and    ch, 0efh
        or     ch, cl
        and    ch, 0f3h
        test   dh, 4
        je     SHORT loc_1_1A36
        test   dh, 8
        jne    SHORT loc_1_1A30
        or     ch, 8
        jmp    SHORT loc_1_1A33
loc_1_1A30:
        or     ch, 4
loc_1_1A33:
        and    ch, 0ffh
loc_1_1A36:
        mov    dl, ch
        mov    edi, eax
        mov    ebx, edx
        mov    eax, dword ptr [edi]
        or     al, 20h
        out    18h, al
        jmp    SHORT loc_1_1A44
loc_1_1A44:
        movzx  eax, word ptr [edi + 18h]
        out    1ah, al
        jmp    SHORT loc_1_1A4C
loc_1_1A4C:
        mov    al, ah
        out    1ah, al
        jmp    SHORT loc_1_1A52
loc_1_1A52:
        mov    ax, word ptr [edi + 16h]
        out    1ah, al
        jmp    SHORT loc_1_1A5A
loc_1_1A5A:
        mov    eax, dword ptr [edi]
        mov    edx, eax
        and    al, 3
        and    bl, 0fch
        or     al, bl
        and    edx, 4
        shl    edx, 2
        db  00Fh, 0B6h, 092h, 020h, 020h, 000h, 000h
        out    dx, al
        jmp    SHORT loc_1_1A75
loc_1_1A75:
        mov    eax, dword ptr [edi]
        or     al, 70h
        out    18h, al
        jmp    SHORT loc_1_1A7D
loc_1_1A7D:
        mov    al, bh
        out    1ah, al
        mov    edx, dword ptr [edi + 10h]
        dec    edx
        test   bh, 40h
        je     SHORT loc_1_1A8C
        shr    edx, 1
loc_1_1A8C:
        mov    eax, dword ptr [edi]
        or     al, 40h
        out    18h, al
        jmp    SHORT loc_1_1A94
loc_1_1A94:
        mov    eax, edx
        out    1ah, al
        jmp    SHORT loc_1_1A9A
loc_1_1A9A:
        mov    al, ah
        out    1ah, al
        jmp    SHORT loc_1_1AA0
loc_1_1AA0:
        test   bh, 1
        je     SHORT loc_1_1ABD
        mov    eax, dword ptr [edi]
        movzx  ebx, word ptr [edi + 32h]
        or     al, 0
        out    18h, al
        jmp    SHORT loc_1_1AB1
loc_1_1AB1:
        mov    eax, ebx
        out    1ah, al
        jmp    SHORT loc_1_1AB7
loc_1_1AB7:
        mov    al, ah
        out    1ah, al
        jmp    SHORT loc_1_1ABD
loc_1_1ABD:
        popad
        ret
loc_1_1ABF:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_1AD0
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_1AD0:
        jb     NEAR PTR loc_1_1912
        db  0BEh, 00Ch, 020h, 000h, 000h
        movzx  edx, al
        and    eax, 7
        cmp    al, 4
        jb     SHORT loc_1_1AE8
        add    esi, 8
loc_1_1AE8:
        mov    ecx, edx
        and    ecx, 0f0h
        shr    ecx, 2
        db  02Eh, 0FFh, 0A1h, 0FAh, 01Ah, 000h, 000h
loc_1_1AFA:
        inc    esp
        sbb    al, 0
        db  000h, 03Ah
        sbb    eax, dword ptr [eax]
        db  000h, 058h, 01Ch
        add    byte ptr [eax], al
        pop    esp
        sbb    al, 0
        db  000h, 06Fh, 01Ch
        add    byte ptr [eax], al
        db  073h, 01Ch
        add    byte ptr [eax], al
        xchg   byte ptr [eax + eax], bl
        db  000h, 094h, 01Ch, 000h, 000h, 0A8h, 01Ch
        add    byte ptr [eax], al
        db  0B8h, 01Ch, 000h, 000h, 0CBh
        sbb    al, 0
        db  000h, 03Ah
        sbb    eax, dword ptr [eax]
        db  000h, 03Ah
        sbb    eax, dword ptr [eax]
        db  000h, 0DEh
        sbb    al, 0
        db  000h, 03Ah
        sbb    eax, dword ptr [eax]
        db  000h, 03Ah
        sbb    eax, dword ptr [eax]
        add    bl, al
loc_1_1B3B:
        db  0FFh, 015h, 004h, 021h, 000h, 000h
        mov    esi, ebx
        db  003h, 035h, 068h, 01Eh, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_1B54
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_1B54:
        je     SHORT loc_1_1B80
        movzx  edx, byte ptr [esi + 4]
        and    edx, 0f0h
        cmp    edx, 80h
        je     SHORT loc_1_1B74
        movzx  ebx, byte ptr [esi + 6]
        mov    al, byte ptr [esi + ebx]
        jmp    NEAR PTR loc_1_1C1F
loc_1_1B74:
        mov    al, byte ptr [esi + 4]
        out    18h, al
        in     al, 1ah
        jmp    NEAR PTR loc_1_1C2A
loc_1_1B80:
        cmp    byte ptr [esi + 5], 0ffh
        jne    NEAR PTR loc_1_1C2B
        movzx  edx, byte ptr [esi + 4]
        and    edx, 0f0h
        cmp    edx, 20h
        je     SHORT loc_1_1C0C
        cmp    edx, 40h
        je     SHORT loc_1_1BF7
        cmp    edx, 0
        je     SHORT loc_1_1BED
        cmp    edx, 70h
        je     SHORT loc_1_1BBD
        cmp    edx, 80h
        jne    SHORT loc_1_1C2B
        mov    ah, al
        mov    al, byte ptr [esi + 4]
        out    18h, al
        mov    al, ah
        out    1ah, al
        jmp    SHORT loc_1_1C2A
loc_1_1BBD:
        call   sub_1_1C2C
        mov    byte ptr [edi + 31h], al
        mov    ah, byte ptr [edi + 30h]
        mov    cl, al
        and    cl, 2
        shl    cl, 3
        and    ah, 0efh
        or     ah, cl
        and    ah, 0f3h
        test   al, 4
        je     SHORT loc_1_1BE8
        test   al, 8
        jne    SHORT loc_1_1BE5
        or     ah, 8
        jmp    SHORT loc_1_1BE8
loc_1_1BE5:
        or     ah, 4
loc_1_1BE8:
        mov    byte ptr [edi + 30h], ah
        jmp    SHORT loc_1_1C2A
loc_1_1BED:
        call   sub_1_1C2C
        lea    edi, [edi + 32h]
        jmp    SHORT loc_1_1BFF
loc_1_1BF7:
        call   sub_1_1C2C
        lea    edi, [edi + 28h]
loc_1_1BFF:
        movzx  ebx, byte ptr [esi + 6]
        mov    byte ptr [edi + ebx], al
        mov    byte ptr [edi + ebx + 4], al
        jmp    SHORT loc_1_1C1F
loc_1_1C0C:
        call   sub_1_1C2C
        movzx  ebx, byte ptr [esi + 6]
        lea    edi, [edi + 20h]
        mov    byte ptr [edi + ebx], al
        mov    byte ptr [edi + ebx + 4], al
loc_1_1C1F:
        inc    ebx
        cmp    bl, byte ptr [esi + 7]
        jb     SHORT loc_1_1C27
        xor    ebx, ebx
loc_1_1C27:
        mov    byte ptr [esi + 6], bl
loc_1_1C2A:
        ret
loc_1_1C2B:
        ret
sub_1_1C2C:
        push   eax
        mov    edi, ebx
        db  003h, 03Dh, 068h, 01Eh, 000h, 000h
        movzx  eax, byte ptr [edi + 4]
        and    al, 7
        call   sub_1_02CA
        mov    edi, eax
        pop    eax
        ret
loc_1_1C44:
        and    al, 3
        call   sub_1_02CA
        movzx  eax, word ptr [eax + 32h]
        mov    cl, 2
        mov    ch, 0ffh
        jmp    NEAR PTR loc_1_1D4E
loc_1_1C58:
        mov    ch, 0ffh
        jmp    SHORT loc_1_1C5E
loc_1_1C5C:
        mov    ch, 0
loc_1_1C5E:
        and    al, 3
        call   sub_1_02CA
        mov    eax, dword ptr [eax + 20h]
        mov    cl, 3
        jmp    NEAR PTR loc_1_1D4E
loc_1_1C6F:
        mov    ch, 0ffh
        jmp    SHORT loc_1_1C75
loc_1_1C73:
        mov    ch, 0
loc_1_1C75:
        and    al, 3
        call   sub_1_02CA
        mov    eax, dword ptr [eax + 28h]
        mov    cl, 2
        jmp    NEAR PTR loc_1_1D4E
loc_1_1C86:
        xor    al, al
        xchg   byte ptr [esi + 4], al
        mov    cl, 1
        mov    ch, 0
        jmp    NEAR PTR loc_1_1D4E
loc_1_1C94:
        and    al, 3
        call   sub_1_02CA
        movzx  eax, byte ptr [eax + 31h]
        mov    cl, 1
        mov    ch, 0ffh
        jmp    NEAR PTR loc_1_1D4E
loc_1_1CA8:
        mov    esi, ebx
        db  003h, 035h, 068h, 01Eh, 000h, 000h
        mov    byte ptr [esi + 4], dl
        mov    byte ptr [esi + 5], 0ffh
        ret
loc_1_1CB8:
        mov    ecx, eax
        and    cl, 3
        bts    dword ptr [esi + 5], ecx
        mov    edx, 1
        jmp    NEAR PTR sub_1_1932
loc_1_1CCB:
        mov    ecx, eax
        and    cl, 3
        btr    dword ptr [esi + 5], ecx
        mov    edx, 1
        jmp    NEAR PTR sub_1_1932
loc_1_1CDE:
        push   esi
        mov    esi, ebx
        db  003h, 035h, 068h, 01Eh, 000h, 000h
        xor    eax, eax
        mov    byte ptr [esi + 4], al
        mov    byte ptr [esi + 5], al
        mov    byte ptr [esi + 6], al
        mov    byte ptr [esi + 7], al
        mov    dword ptr [esi], eax
        mov    word ptr [esi + 8], ax
        pop    esi
        db  0A2h, 010h, 020h, 000h, 000h
        db  0A2h, 018h, 020h, 000h, 000h
        db  0A2h, 012h, 020h, 000h, 000h
        db  0A2h, 01Ah, 020h, 000h, 000h
        mov    ecx, 8
        db  0B8h, 06Ch, 01Eh, 000h, 000h
loc_1_1D1A:
        mov    byte ptr [eax + 30h], 40h
        mov    byte ptr [eax + 31h], 0
        cmp    cl, 4
        ja     SHORT loc_1_1D2B
        mov    byte ptr [eax + 31h], 40h
loc_1_1D2B:
        add    eax, 34h
        loop   loc_1_1D1A
        movzx  eax, byte ptr [esi + 5]
        mov    byte ptr [esi + 5], 0fh
        call   sub_1_1914
        add    esi, 8
        movzx  eax, byte ptr [esi + 5]
        mov    byte ptr [esi + 5], 0fh
        call   sub_1_1914
        ret
loc_1_1D4E:
        mov    esi, ebx
        db  003h, 035h, 068h, 01Eh, 000h, 000h
        mov    byte ptr [esi + 4], dl
        mov    byte ptr [esi + 5], ch
        mov    byte ptr [esi + 6], 0
        mov    byte ptr [esi + 7], cl
        mov    dword ptr [esi], eax
        ret
        db  000h, 000h
sub_1_1D68:
        cmp    eax, 8
        jae    SHORT loc_1_1D74
        db  00Fh, 0ABh, 005h, 01Bh, 021h, 000h, 000h
loc_1_1D74:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h, 060h, 003h, 000h, 000h, 06Dh
        db  003h, 000h, 000h, 0EDh, 003h, 000h, 000h, 0FBh, 003h, 000h, 000h, 02Fh, 004h, 000h, 000h, 07Eh
        db  004h, 000h, 000h, 009h, 005h, 000h, 000h, 084h, 005h, 000h, 000h, 000h, 006h, 000h, 000h, 0FCh
        db  006h, 000h, 000h, 0A9h, 007h, 000h, 000h, 051h, 008h, 000h, 000h, 01Ah, 00Ah, 000h, 000h, 0EBh
        db  002h, 000h, 000h, 0ACh, 00Ah, 000h, 000h, 00Fh, 00Ch, 000h, 000h, 001h, 00Dh, 000h, 000h, 00Eh
        db  00Dh, 000h, 000h, 033h, 00Eh, 000h, 000h, 0BCh, 003h, 000h, 000h, 0D3h, 003h, 000h, 000h, 02Ch
        db  00Dh, 000h, 000h, 042h, 00Dh, 000h, 000h, 0D4h, 007h, 000h, 000h, 0FDh, 005h, 000h, 000h, 079h
        db  006h, 000h, 000h, 08Ch, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 062h, 005h, 000h, 000h, 07Bh
        db  005h, 000h, 000h, 067h, 003h, 000h, 000h, 0B3h, 003h, 000h, 000h, 0EAh, 004h, 000h, 000h, 0F8h
        db  004h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 004h, 000h, 004h, 000h, 000h, 000h, 056h, 044h, 04Dh, 041h
        db  044h, 020h, 020h, 020h, 000h, 000h, 000h, 090h, 0C0h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  080h, 01Dh, 000h, 000h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 021h, 00Dh, 000h, 000h, 02Ah, 00Dh, 000h, 000h, 02Ah, 00Dh, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Dh, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 02Dh, 00Eh, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        db  02Dh, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  003h, 000h, 000h, 000h, 02Dh, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_1F3C:
        add    al, 0
        add    byte ptr [eax], al
loc_1_1F40:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  005h, 000h, 000h, 000h, 02Dh
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        db  000h, 02Dh, 00Eh, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
        db  000h, 02Dh, 00Eh, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2011:
        add    byte ptr [eax], al
loc_1_2013:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2019:
        add    byte ptr [eax], al
loc_1_201B:
        db  000h, 06Ch, 01Eh, 000h
        add    byte ptr [ebx], cl
loc_1_2021:
        or     cl, byte ptr [ecx]
loc_1_2023:
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 03Ch, 01Fh
        add    byte ptr [eax], al
        salc
        aam    0d2h
        fadd   dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_203B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2047:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_204B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        db  000h
sub_1_2104:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2108:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_210C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2110:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2114:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2118:
        add    byte ptr [eax], al
loc_1_211A:
        add    byte ptr [eax], al
sub_1_211C:
        fadd   dword ptr [edi]
        add    byte ptr [eax], al
loc_1_2120:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2124:
        add    byte ptr [eax], al
loc_1_2126:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_212C:
        xchg   dword ptr [ebx - 74707d7fh], eax
        mov    dword ptr [edx + 6040200h], ecx
        rol    ah, 0c8h
        int3
loc_1_213C:
        add    dword ptr [ebx], eax
        add    eax, 0cac6c207h
        into
loc_1_2144:
        xchg   dword ptr [ebx + eax*4], eax
        add    al, 81h
        add    al, 82h
        add    al, 0
        add    byte ptr [ebx - 75fb76fch], cl
        add    al, 1
        add    al, 3
        add    al, 5
        add    al, 7
        add    al, 0
        add    dh, al
        add    al, 0cah
        add    al, 0ceh
        db  004h
_LTEXT ends
        end
