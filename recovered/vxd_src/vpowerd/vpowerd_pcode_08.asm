; vpowerd_pcode_08.asm -- VPOWERD object 8 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VPOWERD_PM_API:
        movzx  ebx, word ptr [ebp + 1ch]
        mov    eax, 0ffh
        cmp    ebx, 19h
        jae    SHORT loc_8_006C
        db  00Fh, 0B6h, 00Ch, 05Dh, 000h, 000h, 000h, 000h
        lea    edi, [ecx*4]
        jecxz  loc_8_0063
        mov    eax, 8000000ch
        cmp    cl, 0ffh
        je     SHORT loc_8_006C
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        lea    esi, [edi + eax]
        db  00Fh, 0B6h, 014h, 05Dh, 001h, 000h, 000h, 000h
loc_8_003E:
        sub    esi, 4
        mov    eax, dword ptr [esi]
        rcr    edx, 1
        jae    SHORT loc_8_0060
        or     eax, eax
        je     SHORT loc_8_0060
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        mov    ax, 141ch
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
loc_8_0060:
        push   eax
        loop   loc_8_003E
loc_8_0063:
        db  0FFh, 014h, 09Dh, 070h, 005h, 000h, 000h
        add    esp, edi
loc_8_006C:
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        ret
sub_8_0078:
        xor    esi, esi
        db  087h, 035h, 034h, 006h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        db  0C6h, 005h, 02Ch, 006h, 000h, 000h, 000h
        mov    ax, 5304h
        xor    ebx, ebx
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  08Bh, 03Dh, 030h, 006h, 000h, 000h
        btr    dword ptr [edi + 3], 7
        jae    SHORT loc_8_00B9
        mov    ax, 5401h
        mov    bl, byte ptr [edi + 2]
        mov    bh, 1
        push   2fh
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_8_00B9:
        clc
        ret
        db  000h
loc_8_00BC:
        enter  18h, 0
        push   1
        push   0ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_8_01E0
        push   eax
        push   1
        push   40h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_8_01E0
        mov    dword ptr [ebp - 14h], eax
        push   1
        push   100h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_8_01E0
        mov    dword ptr [ebp - 18h], eax
        db  00Fh, 0B7h, 005h, 00Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        mov    ebx, dword ptr [ebp - 14h]
        push   ebx
        push   eax
        db  0E8h, 0F4h, 000h, 0F8h, 0FFh
        add    esp, 8
        mov    ecx, 200h
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_8_01B5
        mov    word ptr [ebp - 10h], cx
        mov    dword ptr [ebp - 8], edx
        db  00Fh, 0B7h, 005h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        cmp    dword ptr [ebp + 8], 80000002h
        je     SHORT loc_8_015E
        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_8_015E:
        mov    ebx, dword ptr [ebp - 18h]
        push   ebx
        push   eax
        db  0E8h, 0ACh, 000h, 0F8h, 0FFh
        add    esp, 8
        mov    ecx, 200h
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        jb     SHORT loc_8_01B5
        pop    eax
        mov    word ptr [ebp - 0ch], cx
        mov    dword ptr [ebp - 4], edx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], cx
        mov    cx, word ptr [ebp - 10h]
        mov    word ptr [eax + 4], cx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [eax + 6], cx
        mov    cx, word ptr [ebp - 0ch]
        mov    word ptr [eax + 8], cx
        push   eax
        push   eax
        push   0ch
        db  068h, 039h, 000h, 000h, 000h
        db  068h, 034h, 000h, 000h, 000h
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
loc_8_01B5:
        pop    eax
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    eax, dword ptr [ebp - 14h]
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    eax, dword ptr [ebp - 18h]
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_8_01E0:
        leave
        ret
loc_8_01E2:
        push   0
        push   2
        push   6
        db  0E8h, 0ABh, 000h, 0F8h, 0FFh
        add    esp, 0ch
        ret
loc_8_01F1:
        call   sub_8_029B
        push   0
        push   2
        push   8
        db  0E8h, 097h, 000h, 0F8h, 0FFh
        add    esp, 0ch
        ret
loc_8_0205:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, 200h
        jb     SHORT loc_8_0228
        cmp    ecx, 2ffh
        ja     SHORT loc_8_0228
        push   0
        push   2
        push   ecx
        db  0E8h, 073h, 000h, 0F8h, 0FFh
        add    esp, 0ch
loc_8_0228:
        leave
        ret
loc_8_022A:
        enter  8, 0
        push   esi
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_8_024E
        lea    esi, [ebp - 8]
        db  0C7h, 005h, 049h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        push   esi
        push   1
        db  0CDh, 020h, 007h, 000h, 026h, 000h
        add    esp, 8
loc_8_024E:
        push   0
        push   2
        push   7
        db  0E8h, 03Fh, 000h, 0F8h, 0FFh
        add    esp, 0ch
        pop    esi
        leave
        ret
loc_8_025F:
        enter  8, 0
        mov    ax, 5310h
        xor    bx, bx
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jb     SHORT loc_8_028C
        db  0A1h, 030h, 006h, 000h, 000h
        mov    word ptr [eax + 4], cx
        mov    byte ptr [eax + 6], bl
        push   0
        push   2
        push   9
        db  0E8h, 00Fh, 000h, 0F8h, 0FFh
        add    esp, 0ch
loc_8_028C:
        lea    ecx, [ebp - 8]
        push   ecx
        push   1
        db  0E8h, 05Dh, 000h, 0F8h, 0FFh
        pop    ecx
        pop    ecx
        leave
        ret
sub_8_029B:
        push   ebx
        push   esi
        push   edi
        clc
        mov    ah, 2
        push   1ah
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     NEAR PTR loc_8_0362
        mov    al, cl
        call   sub_8_0366
        mov    esi, eax
        mov    al, dh
        call   sub_8_0366
        mov    edi, eax
        mov    al, ch
        call   sub_8_0366
        xor    edx, edx
        mov    ecx, 3ch
        mul    ecx
        add    eax, esi
        mul    ecx
        add    eax, edi
        mov    ecx, 1234dch
        mul    ecx
        mov    ecx, edx
        shr    eax, 10h
        mov    edx, eax
        mov    ah, 1
        push   1ah
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_8_02EF:
        cmp    cx, 18h
        jb     SHORT loc_8_0308
        cmp    dx, 9ch
        jb     SHORT loc_8_0308
        xor    eax, eax
        push   1ah
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jmp    SHORT loc_8_02EF
loc_8_0308:
        clc
        mov    ah, 4
        push   1ah
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_8_0362
        mov    al, dh
        call   sub_8_0366
        mov    dh, al
        mov    al, dl
        call   sub_8_0366
        mov    dl, al
        mov    al, cl
        call   sub_8_0366
        mov    esi, eax
        mov    cl, al
        mov    al, ch
        call   sub_8_0366
        imul   eax, eax, 64h
        add    esi, eax
        mov    edi, edx
        mov    ah, 2ah
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    cx, si
        jne    SHORT loc_8_0354
        cmp    dx, di
        je     SHORT loc_8_0362
loc_8_0354:
        mov    ecx, esi
        mov    edx, edi
        mov    ah, 2bh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_8_0362:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_0366:
        mov    ah, al
        and    al, 0fh
        shr    ah, 4
        aad    0ah
        cwde
        ret
sub_8_0371:
        aam    0ah
        shl    ah, 4
        or     al, ah
        movzx  eax, al
        ret
_VPOWERD_Register_Power_Handler:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  08Dh, 005h, 088h, 006h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, eax
        mov    ecx, eax
loc_8_0392:
        mov    eax, dword ptr [eax]
        cmp    eax, ecx
        je     SHORT loc_8_03A6
        cmp    dword ptr [eax + 8], esi
        je     SHORT loc_8_03D0
        cmp    dword ptr [eax + 0ch], edi
        ja     SHORT loc_8_0392
        mov    ebx, eax
        jmp    SHORT loc_8_0392
loc_8_03A6:
        push   0
        push   10h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_8_03D7
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0ch], edi
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebx], eax
        xor    eax, eax
loc_8_03CB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_8_03D0:
        mov    eax, 8000000ah
        jmp    SHORT loc_8_03CB
loc_8_03D7:
        mov    eax, 80000009h
        jmp    SHORT loc_8_03CB
_VPOWERD_Deregister_Power_Handler:
        push   ebp
        mov    ebp, esp
        db  08Dh, 005h, 088h, 006h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, eax
loc_8_03EC:
        mov    eax, dword ptr [eax]
        cmp    eax, ecx
        je     SHORT loc_8_0411
        cmp    dword ptr [eax + 8], edx
        jne    SHORT loc_8_03EC
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        xor    eax, eax
loc_8_040F:
        leave
        ret
loc_8_0411:
        mov    eax, 8000000bh
        jmp    SHORT loc_8_040F
_VPOWERD_Get_Version:
        mov    eax, 400h
        clc
        ret
_VPOWERD_Get_APM_BIOS_Version:
        db  0A1h, 030h, 006h, 000h, 000h
        movzx  eax, word ptr [eax]
        ret
_VPOWERD_Get_Power_Management_Level:
        db  00Fh, 0B6h, 005h, 090h, 006h, 000h, 000h
        ret
_VPOWERD_Set_Power_Management_Level:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ah, 3
        db  080h, 03Dh, 02Ch, 006h, 000h, 000h, 000h
        je     NEAR PTR loc_8_04ED
        mov    eax, 80000005h
        cmp    dword ptr [ebp + 8], 2
        ja     NEAR PTR loc_8_04F0
        db  080h, 03Dh, 02Ah, 006h, 000h, 000h, 000h
        jne    NEAR PTR loc_8_04F5
        db  080h, 03Dh, 090h, 006h, 000h, 000h, 000h
        je     SHORT loc_8_047C
        mov    ax, 5303h
        xor    ebx, ebx
        push   ebp
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_8_04ED
loc_8_047C:
        cmp    dword ptr [ebp + 8], 2
        je     SHORT loc_8_0488
        mov    cx, 1
        jmp    SHORT loc_8_048A
loc_8_0488:
        xor    ecx, ecx
loc_8_048A:
        mov    ax, 5308h
        mov    bx, 0ffffh
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jae    SHORT loc_8_04A5
        cmp    dword ptr [ebp + 8], 2
        jne    SHORT loc_8_04ED
        cmp    ah, 1
        jne    SHORT loc_8_04ED
loc_8_04A5:
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_8_04C9
        mov    ax, 5304h
        xor    ebx, ebx
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jae    SHORT loc_8_04C9
        mov    ax, 5304h
        xor    ebx, ebx
        push   ebp
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
loc_8_04C9:
        mov    eax, dword ptr [ebp + 8]
        db  0A2h, 090h, 006h, 000h, 000h
        cmp    eax, 0
        jne    SHORT loc_8_04DD
        db  0E8h, 0F3h, 0FFh, 0F7h, 0FFh
        jmp    SHORT loc_8_04EB
loc_8_04DD:
        xor    esi, esi
        db  087h, 035h, 034h, 006h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_8_04EB:
        xor    eax, eax
loc_8_04ED:
        movzx  eax, ah
loc_8_04F0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_8_04F5:
        mov    cx, 1
        call   sub_8_053B
        mov    cx, 1
        call   sub_8_0541
        jb     SHORT loc_8_04ED
        mov    cx, 1
        call   sub_8_053B
        jb     SHORT loc_8_04ED
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0
        je     SHORT loc_8_04C9
        cmp    eax, 1
        jne    SHORT loc_8_052E
        mov    cx, 0
        call   sub_8_0541
        jae    SHORT loc_8_04C9
        jmp    SHORT loc_8_04ED
loc_8_052E:
        mov    cx, 0
        call   sub_8_053B
        jae    SHORT loc_8_04C9
        jmp    SHORT loc_8_04ED
sub_8_053B:
        mov    ax, 5308h
        jmp    SHORT loc_8_0545
sub_8_0541:
        mov    ax, 530fh
loc_8_0545:
        mov    bx, 1
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        ret
_VPOWERD_Set_Device_Power_State:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 80000006h
        cmp    ebx, 1
        je     SHORT loc_8_0577
        mov    ax, 5307h
        mov    ecx, dword ptr [ebp + 0ch]
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jb     SHORT loc_8_0574
        xor    eax, eax
loc_8_0574:
        movzx  eax, ah
loc_8_0577:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_VPOWERD_Get_Power_State:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ax, 530ch
        mov    ebx, dword ptr [ebp + 8]
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jb     SHORT loc_8_05A4
        mov    eax, 80000003h
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        je     SHORT loc_8_05A7
        movzx  ecx, cx
        mov    dword ptr [esi], ecx
        xor    eax, eax
loc_8_05A4:
        movzx  eax, ah
loc_8_05A7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_VPOWERD_Restore_Power_On_Defaults:
        mov    ax, 5309h
        mov    bx, 0ffffh
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        movzx  eax, ah
        jb     SHORT loc_8_05C1
        xor    eax, eax
loc_8_05C1:
        ret
_VPOWERD_OEM_APM_Function:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pushad
        mov    eax, 80000003h
        jecxz  loc_8_0617
        mov    edi, dword ptr [ecx]
        mov    esi, dword ptr [ecx + 4]
        mov    ebp, dword ptr [ecx + 8]
        mov    ebx, dword ptr [ecx + 0ch]
        mov    edx, dword ptr [ecx + 10h]
        mov    ah, byte ptr [ecx + 1ch]
        sahf
        mov    eax, dword ptr [ecx + 18h]
        mov    ecx, dword ptr [ecx + 14h]
        mov    ax, 5380h
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        xchg   dword ptr [esp + 18h], ecx
        mov    dword ptr [ecx + 18h], eax
        lahf
        mov    byte ptr [ecx + 1ch], ah
        mov    dword ptr [ecx + 10h], edx
        mov    dword ptr [ecx + 0ch], ebx
        mov    dword ptr [ecx + 8], ebp
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [ecx], edi
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, 0
loc_8_0617:
        mov    dword ptr [esp + 1ch], eax
        popad
        leave
        ret
VPOWERD_Service_14:
        push   ebp
        mov    ebp, esp
        mov    eax, 0ch
        db  080h, 03Dh, 02Bh, 006h, 000h, 000h, 001h
        jne    SHORT loc_8_0648
        db  08Bh, 00Dh, 030h, 006h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 80000003h
        or     edx, edx
        je     SHORT loc_8_0648
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx], eax
        xor    eax, eax
loc_8_0648:
        leave
        ret
VPOWERD_Service_16:
        xor    ecx, ecx
        jmp    SHORT sub_8_0653
VPOWERD_Service_15:
        mov    ecx, 1
sub_8_0653:
        push   ebx
        mov    eax, 0ch
        db  080h, 03Dh, 02Bh, 006h, 000h, 000h, 001h
        jne    SHORT loc_8_0676
        mov    eax, 5312h
        xor    ebx, ebx
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jb     SHORT loc_8_0673
        xor    eax, eax
loc_8_0673:
        movzx  eax, ah
loc_8_0676:
        pop    ebx
        ret
VPOWERD_Service_24:
        push   ebp
        mov    ebp, esp
        mov    eax, 80000003h
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_8_0698
        mov    ecx, 2
        call   sub_8_0653
        movzx  ecx, cx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx], ecx
loc_8_0698:
        leave
        ret
VPOWERD_Service_19:
        push   ebx
        mov    eax, 0ch
        db  080h, 03Dh, 02Bh, 006h, 000h, 000h, 001h
        jne    SHORT loc_8_06C7
        mov    ax, 5311h
        xor    ebx, ebx
        xor    ecx, ecx
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jb     SHORT loc_8_06BD
        xor    eax, eax
        jmp    SHORT loc_8_06C7
loc_8_06BD:
        cmp    ah, 0dh
        jne    SHORT loc_8_06C4
        xor    eax, eax
loc_8_06C4:
        movzx  eax, ah
loc_8_06C7:
        pop    ebx
        ret
VPOWERD_Service_20:
        mov    ecx, 1
        jmp    SHORT sub_8_06D2
VPOWERD_Service_21:
        xor    ecx, ecx
sub_8_06D2:
        push   ebx
        mov    eax, 0ch
        db  080h, 03Dh, 02Bh, 006h, 000h, 000h, 001h
        jne    SHORT loc_8_06F4
        mov    ax, 5313h
        xor    ebx, ebx
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jb     SHORT loc_8_06F1
        xor    eax, eax
loc_8_06F1:
        movzx  eax, ah
loc_8_06F4:
        pop    ebx
        ret
VPOWERD_Service_23:
        push   ebp
        mov    ebp, esp
        mov    eax, 80000003h
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_8_0716
        mov    ecx, 2
        call   sub_8_06D2
        movzx  ecx, cx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx], ecx
loc_8_0716:
        leave
        ret
_PTEXT ends
        end
