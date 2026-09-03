; VPD -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 1047 instructions recovered by recursive descent

Declare_Virtual_Device VPD, 3, 0, VPD_Control, 0000Fh, Undefined_Init_Order
;   DDK version 0400h   init_order 03C000014h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xa30  R-X
VPD_Control:
        cmp    eax, 1
        je     loc_1_0034
        cmp    eax, 2
        je     loc_1_0203
        cmp    eax, 0xf
        je     loc_1_0226
        cmp    eax, 0xb
        je     loc_1_023F
        cmp    eax, 0x30
        je     loc_3_015A
        cmp    eax, 5
        je     loc_1_027C
        clc    
        ret    
loc_1_0034:
        xor    esi, esi
        mov    edi, 0x58                     ; -> obj2:0x0058
        mov    eax, dword ptr [0x9be]        ; -> loc_1_09BE
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        mov    dword ptr [0x9be], eax        ; -> loc_1_09BE
        mov    edi, 0x34                     ; -> loc_2_0034
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        jb     loc_1_0075
        test   eax, eax
        je     loc_1_0075
        cmp    eax, 3
        ja     loc_1_0075
        dec    eax
        imul   eax, eax, 0x40
        add    eax, 0x8c0                    ; -> loc_1_08C0
        mov    dword ptr [0x9c4], eax        ; -> obj1:0x09C4
        mov    dword ptr [0x9c8], ebx        ; -> loc_1_09C8
loc_1_0075:
        xor    edi, edi
        xor    ecx, ecx
        mov    esi, 0x8c0                    ; -> loc_1_08C0
loc_1_007E:
        push   ecx
        push   esi
        push   edi
        add    cl, 0x31
        mov    byte ptr [0x6f], cl           ; -> loc_2_006F
        mov    byte ptr [0x7c], cl           ; -> loc_2_007C
        mov    byte ptr [0x4f], cl           ; -> loc_2_004F
        mov    byte ptr [0x40], cl           ; -> loc_2_0040
        xor    esi, esi
        mov    edi, 0x4c                     ; -> obj2:0x004C
        xor    eax, eax
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        test   eax, eax
        pop    edi
        pop    esi
        pop    ecx
        jne    loc_1_0188
        push   ecx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, 0x3d                     ; -> loc_2_003D
        mov    eax, 0xea60
        mov    ecx, 3
        VMMCall  VMM.Get_Profile_Fixed_Point     ; cd20ad000100
        cmp    eax, 0xfffffc18
        jne    loc_1_00DA
        or     eax, 0xffffffff
loc_1_00DA:
        pop    edi
        pop    esi
        pop    ecx
        mov    dword ptr [esi + 4], eax
        movzx  edx, word ptr [ecx*2 + 0x408]
        test   edx, edx
        je     loc_1_0188
        mov    dword ptr [esi + 0x14], edx
        inc    edi
        test   dh, dh
        je     loc_1_0188
        cmp    esi, dword ptr [0x9c4]        ; -> obj1:0x09C4
        je     loc_1_0188
        cmp    esi, 0x8c0                    ; -> loc_1_08C0
        je     loc_1_0131
        cmp    dword ptr [0x8d4], edx        ; -> obj1:0x08D4
        je     loc_1_0128
        cmp    esi, 0x900                    ; -> obj1:0x0900
        je     loc_1_0131
        cmp    dword ptr [0x914], edx        ; -> obj1:0x0914
        jne    loc_1_0131
loc_1_0128:
        mov    dword ptr [esi + 0x14], 1
        jmp    loc_1_0188
loc_1_0131:
        pushal 
        xor    esi, esi
        mov    edi, 0x69                     ; -> obj2:0x0069
        xor    eax, eax
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        cmp    eax, 0
        popal  
        je     loc_1_0161
        pushal 
        xor    esi, esi
        mov    edi, 0x71                     ; -> loc_2_0071
        xor    eax, eax
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        cmp    eax, 0
        popal  
        je     loc_1_0161
        or     dword ptr [esi + 0x20], 0x10
loc_1_0161:
        push   ecx
        push   esi
        mov    esi, dword ptr [esi + 8]
        mov    ecx, 3
loc_1_016B:
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_1_017D
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        inc    edx
        loop   loc_1_016B
        clc    
loc_1_017D:
        pop    esi
        pop    ecx
        jae    loc_1_0188
        mov    dword ptr [esi + 0x14], 0
loc_1_0188:
        add    esi, 0x40
        inc    ecx
        cmp    ecx, 3
        jb     loc_1_007E
        test   edi, edi
        jne    loc_1_019B
        stc    
        ret    
loc_1_019B:
        cmp    dword ptr [0x9c4], 0          ; -> obj1:0x09C4
        je     loc_1_01BD
        mov    edi, 0                        ; -> loc_2_0000
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jae    loc_1_01BD
        mov    dword ptr [0x9c4], 0          ; -> obj1:0x09C4
        jmp    loc_1_01BD
loc_1_01BD:
        push   0
        push   4
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    loc_1_01D3
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_1_01D3:
        mov    dword ptr [0x8bc], eax        ; -> obj1:0x08BC
        push   0
        push   0x20                          ; -> obj2:0x0020
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    loc_1_01F1
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_1_01F1:
        mov    eax, 0x17
        mov    esi, 0x5c4                    ; -> loc_1_05C4
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        clc    
        ret    
loc_1_0203:
        mov    ecx, 3
        lea    esi, [0x8cc]                  ; -> loc_1_08CC
loc_1_020E:
        VxDCall  DOSMGR.DOSMGR_Instance_Device   ; cd2007001500
        add    esi, 0x40
        loop   loc_1_020E
        mov    esi, 0x7e                     ; -> loc_2_007E
        VxDCall  DOSMGR.DOSMGR_Instance_Device   ; cd2007001500
        clc    
        ret    
loc_1_0226:
        cmp    dword ptr [0x9c4], 0          ; -> obj1:0x09C4
        je     loc_1_023E
        test   edx, edx
        je     loc_1_0238
        cmp    edx, 0xa
        jne    loc_1_023E
loc_1_0238:
        mov    dword ptr [0x9c8], ebx        ; -> loc_1_09C8
loc_1_023E:
        ret    
loc_1_023F:
        mov    ecx, 3
        mov    esi, 0x8c0                    ; -> loc_1_08C0
loc_1_0249:
        cmp    ebx, dword ptr [esi + 0x1c]
        jne    loc_1_0276
        call   sub_1_063F
        cmp    dword ptr [esi + 0x24], 0
        je     loc_1_0276
        push   0
        push   dword ptr [esi + 0x30]
        push   dword ptr [esi + 0x2c]
        push   esi
        push   1
        call   sub_3_0077
        add    esp, 0x14
        push   1
        push   dword ptr [esi + 0x28]
        call   dword ptr [esi + 0x24]
        pop    ecx
        pop    ecx
loc_1_0276:
        add    esi, 0x40
        loop   loc_1_0249
        ret    
loc_1_027C:
        mov    ecx, 3
        mov    esi, 0x8c0                    ; -> loc_1_08C0
loc_1_0286:
        bt     dword ptr [esi + 0x20], 3
        jae    loc_1_0292
        call   sub_1_0736
loc_1_0292:
        add    esi, 0x40
        loop   loc_1_0286
        ret    
VPD_PM_API:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_02BB
        movzx  eax, word ptr [ebp + 0x14]
        cmp    eax, 0x12
        ja     loc_1_02BB
        mov    ecx, dword ptr [eax*4 + 0x9cc] ; -> obj1:0x09CC
        jecxz  loc_1_02BB
        jmp    dword ptr [eax*4 + 0x9cc]     ; -> obj1:0x09CC
loc_1_02BB:
        or     dword ptr [ebp + 0x2c], 1
        ret    
loc_1_02C0:
        and    dword ptr [ebp + 0x2c], 0xfffffffe
        ret    
loc_1_02C5:
        mov    word ptr [ebp + 0x1c], 0x30b
        mov    word ptr [ebp + 0x14], 0xcb01
        jmp    loc_1_02C0
loc_1_02D3:
        mov    esi, 0x8c0                    ; -> loc_1_08C0
        xor    eax, eax
        mov    ecx, eax
loc_1_02DC:
        cmp    dword ptr [esi + 0x14], 0
        je     loc_1_02E5
        bts    eax, ecx
loc_1_02E5:
        add    esi, 0x40
        inc    ecx
        cmp    ecx, 3
        jb     loc_1_02DC
        mov    word ptr [ebp + 0x1c], ax
        jmp    loc_1_02C0
loc_1_02F4:
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    ecx, 3
        jae    loc_1_02BB
        imul   ecx, ecx, 0x40
        add    ecx, 0x8c0                    ; -> loc_1_08C0
        mov    eax, dword ptr [ecx + 4]
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x10], ax
        jmp    loc_1_02C0
loc_1_0316:
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    ecx, 3
        jae    loc_1_02BB
        imul   ecx, ecx, 0x40
        add    ecx, 0x8c0                    ; -> loc_1_08C0
        mov    ax, word ptr [ebp + 0x10]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_02C0
sub_1_0338:
        jne    loc_1_0351
        mov    eax, dword ptr [0x8bc]        ; -> obj1:0x08BC
        add    eax, ebx
        mov    ecx, dword ptr [esi]
        push   dword ptr [eax]
        bts    dword ptr [eax], ecx
        call   sub_1_06C0
        pop    dword ptr [eax]
        jb     loc_1_035A
loc_1_0351:
        xor    ecx, ecx
        dec    ecx
        call   sub_1_0669
        clc    
loc_1_035A:
        ret    
sub_1_035B:
        mov    edx, dword ptr [esi + 0x14]
        cmp    edx, 2
        jb     loc_1_0379
        mov    ecx, 3
loc_1_0368:
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        inc    edx
        loop   loc_1_0368
        or     dword ptr [esi + 0x20], 0x80
        clc    
loc_1_0379:
        ret    
sub_1_037A:
        mov    edx, dword ptr [esi + 0x14]
        cmp    edx, 2
        jb     loc_1_0398
        mov    ecx, 3
loc_1_0387:
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        inc    edx
        loop   loc_1_0387
        and    dword ptr [esi + 0x20], 0xffffff7f
        clc    
loc_1_0398:
        ret    
loc_1_0399:
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    ecx, 3
        jae    loc_1_02BB
        imul   ecx, ecx, 0x40
        lea    esi, [ecx + 0x8c0]            ; -> loc_1_08C0
        test   byte ptr [esi + 0x20], 8
        je     loc_1_03C4
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        cmp    dword ptr [esi + 0x1c], ebx
        je     loc_1_02C0
loc_1_03C4:
        test   byte ptr [ebp + 0x1c], 1
        call   sub_1_0338
        jae    loc_1_02C0
        jb     loc_1_02BB
loc_1_03D9:
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    ecx, 3
        jae    loc_1_02BB
        imul   ecx, ecx, 0x40
        lea    esi, [ecx + 0x8c0]            ; -> loc_1_08C0
        cmp    dword ptr [esi + 0x1c], ebx
        jne    loc_1_02BB
        call   sub_1_063F
        jmp    loc_1_02C0
loc_1_0402:
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    ecx, 3
        jae    loc_1_02BB
        imul   ecx, ecx, 0x40
        lea    esi, [ecx + 0x8c0]            ; -> loc_1_08C0
        movzx  ecx, word ptr [ebp + 0x1c]
        cmp    ecx, dword ptr [esi + 0x38]
        jne    loc_1_042B
        or     ecx, ecx
        je     loc_1_02C0
        jmp    loc_1_046F
loc_1_042B:
        mov    eax, dword ptr [esi + 0x34]
        or     eax, eax
        je     loc_1_0444
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        mov    dword ptr [esi + 0x34], 0
loc_1_0444:
        mov    dword ptr [esi + 0x38], ecx
        or     ecx, ecx
        je     loc_1_02C0
        push   ecx
        push   1
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    loc_1_046C
        mov    dword ptr [esi + 0x38], 0
        jmp    loc_1_02BB
loc_1_046C:
        mov    dword ptr [esi + 0x34], eax
loc_1_046F:
        mov    edi, dword ptr [esi + 0x34]
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        jne    loc_1_04A2
        mov    eax, dword ptr [esi + 0x34]
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        mov    dword ptr [esi + 0x34], 0
        mov    dword ptr [esi + 0x38], 0
        jmp    loc_1_02BB
loc_1_04A2:
        mov    esi, eax
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_02C0
loc_1_04AC:
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    ecx, 3
        jae    loc_1_02BB
        imul   ecx, ecx, 0x40
        lea    esi, [ecx + 0x8c0]            ; -> loc_1_08C0
        test   byte ptr [esi + 0x20], 0x20
        jne    loc_1_02BB
        test   byte ptr [esi + 0x20], 1
        jne    loc_1_02BB
        cmp    dword ptr [esi + 0x1c], 0
        je     loc_1_04F4
        cmp    dword ptr [esi + 4], -1
        je     loc_1_04F4
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, dword ptr [esi + 0x18]
        cmp    eax, dword ptr [esi + 4]
        jb     loc_1_02BB
loc_1_04F4:
        xor    al, al
        or     dword ptr [esi + 0x20], 0x40
        call   sub_1_0338
        jae    loc_1_050A
        and    dword ptr [esi + 0x20], 0xffffffbf
        jmp    loc_1_02BB
loc_1_050A:
        and    dword ptr [esi + 0x20], 0xffffffbf
        or     dword ptr [esi + 0x20], 8
        jmp    loc_1_02C0
loc_1_0517:
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    ecx, 3
        jae    loc_1_02BB
        imul   ecx, ecx, 0x40
        lea    esi, [ecx + 0x8c0]            ; -> loc_1_08C0
        test   byte ptr [esi + 0x20], 8
        je     loc_1_02BB
        call   sub_1_063F
        jmp    loc_1_02C0
loc_1_0541:
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    ecx, 3
        jae    loc_1_02BB
        imul   ecx, ecx, 0x40
        lea    esi, [ecx + 0x8c0]            ; -> loc_1_08C0
        mov    edx, dword ptr [esi + 0x14]
        test   dh, dh
        jne    loc_1_0563
        jmp    loc_1_02BB
loc_1_0563:
        inc    dx
        in     al, dx
        xor    ah, ah
        mov    word ptr [ebp + 0x1c], ax
        jmp    loc_1_02C0
        db  02Eh, 08Bh, 0C0h
loc_1_0574:
        mov    esi, 0x8c0                    ; -> loc_1_08C0
        jmp    loc_1_058C
        db  090h
loc_1_057C:
        mov    esi, 0x900                    ; -> obj1:0x0900
        jmp    loc_1_058C
        db  090h
loc_1_0584:
        mov    esi, 0x940                    ; -> obj1:0x0940
        jmp    loc_1_058C
        db  090h
loc_1_058C:
        cmp    ecx, 4
        jbe    loc_1_0597
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0597:
        mov    edi, edx
        and    edi, 3
        jmp    loc_1_0A18
loc_1_05A3:
        in     al, dx
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_05A8:
        call   sub_1_06C0
        jb     loc_1_05B0
        out    dx, al
loc_1_05B0:
        ret    
loc_1_05B1:
        in     al, dx
        cmp    dword ptr [esi + 0x1c], 0
        je     loc_1_05C1
        call   sub_1_06C0
        jae    loc_1_05C1
        mov    al, 0xb8
loc_1_05C1:
        ret    
        db  08Bh, 0C0h
loc_1_05C4:
        cmp    byte ptr [ebp + 0x1d], 2
        jae    loc_1_0625
        movzx  esi, word ptr [ebp + 0x14]
        cmp    esi, 2
        ja     loc_1_0625
        imul   esi, esi, 0x40
        add    esi, 0x8c0                    ; -> loc_1_08C0
        test   byte ptr [esi + 0x20], 0x80
        je     loc_1_0625
        call   sub_1_06C0
        jae    loc_1_05EF
loc_1_05E9:
        mov    byte ptr [ebp + 0x1d], 0x21
        clc    
        ret    
loc_1_05EF:
        cmp    byte ptr [ebp + 0x1d], 0
        jne    loc_1_0625
        test   byte ptr [esi + 0x20], 2
        jne    loc_1_0625
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    byte ptr [ebp + 0x1d], 2
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        xchg   dword ptr [ebp + 0x1c], ecx
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        test   ch, 0x80
        je     loc_1_05E9
        test   ch, 0x20
        jne    loc_1_05E9
        or     dword ptr [esi + 0x20], 2
loc_1_0625:
        stc    
        ret    
loc_1_0627:
        mov    ebx, dword ptr [0x9c8]        ; -> loc_1_09C8
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002800300
loc_1_0633:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004800300
sub_1_063F:
        pushal 
        and    dword ptr [esi + 0x20], 0xfffffffe
        mov    ebx, dword ptr [esi + 0x1c]
        test   ebx, ebx
        je     loc_1_0667
        mov    edx, dword ptr [esi + 0x14]
        test   dh, dh
        je     loc_1_0667
        mov    ecx, 3
loc_1_0657:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    edx
        loop   loc_1_0657
        mov    dword ptr [esi + 0x1c], 0
loc_1_0667:
        popal  
        ret    
sub_1_0669:
        pushal 
        mov    edx, dword ptr [esi + 0x14]
        test   dh, dh
        je     loc_1_06BB
        call   sub_1_063F
        mov    dword ptr [esi + 0x1c], ebx
        test   byte ptr [esi + 0x20], 8
        je     loc_1_0693
        test   byte ptr [esi + 0x20], 0x40
        jne    loc_1_0693
        and    dword ptr [esi + 0x20], 0xfffffff7
        call   sub_1_0736
        call   sub_1_075D
loc_1_0693:
        jecxz  loc_1_0699
        or     dword ptr [esi + 0x20], 1
loc_1_0699:
        mov    eax, dword ptr [0x8bc]        ; -> obj1:0x08BC
        add    eax, ebx
        mov    edi, dword ptr [esi]
        btr    dword ptr [eax], edi
        jecxz  loc_1_06AD
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
loc_1_06AD:
        inc    edx
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    edx
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
loc_1_06BB:
        popal  
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_06C0:
        push   eax
        cmp    dword ptr [esi + 0x1c], ebx
        je     loc_1_06ED
        cmp    dword ptr [esi + 0x1c], 0
        je     loc_1_06E6
        test   byte ptr [esi + 0x20], 1
        jne    loc_1_06F9
        cmp    dword ptr [esi + 4], -1
        je     loc_1_06F9
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, dword ptr [esi + 0x18]
        cmp    eax, dword ptr [esi + 4]
        jb     loc_1_06F9
loc_1_06E6:
        xor    ecx, ecx
        call   sub_1_0669
loc_1_06ED:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [esi + 0x18], eax
        pop    eax
        clc    
        ret    
loc_1_06F9:
        pushal 
        mov    eax, dword ptr [0x8bc]        ; -> obj1:0x08BC
        add    eax, ebx
        mov    ecx, dword ptr [esi]
        bts    dword ptr [eax], ecx
        jb     loc_1_0732
        mov    eax, dword ptr [esi + 0x1c]
        push   esi
        lea    esi, [esi + 0xc]
        VxDCall  SHELL.SHELL_Resolve_Contention  ; cd2001001700
        pop    esi
        mov    eax, dword ptr [esi + 0x1c]
        cmp    ebx, eax
        je     loc_1_0732
        mov    ecx, dword ptr [esi]
        add    eax, dword ptr [0x8bc]        ; -> obj1:0x08BC
        bts    dword ptr [eax], ecx
        xor    ecx, ecx
        call   sub_1_0669
        popal  
        jmp    loc_1_06ED
loc_1_0732:
        popal  
        pop    eax
        stc    
        ret    
sub_1_0736:
        pushal 
        mov    edi, esi
        bt     dword ptr [edi + 0x20], 4
        jae    loc_1_074B
        mov    esi, dword ptr [edi + 0x34]
        mov    ecx, dword ptr [edi + 0x38]
        jcxz   loc_1_075B
        jmp    loc_1_0756
loc_1_074B:
        lea    esi, [0x980]                  ; -> obj1:0x0980
        mov    ecx, 0x3e
loc_1_0756:
        call   sub_1_07ED
loc_1_075B:
        popal  
        ret    
sub_1_075D:
        pushal 
        mov    edi, esi
        cld    
        or     dword ptr [edi + 0x20], 0x20
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        add    eax, 0xfa0
        mov    esi, eax
loc_1_0778:
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        cmp    eax, esi
        jb     loc_1_0778
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        add    eax, 0xdac0
        mov    esi, eax
        mov    edx, dword ptr [edi + 0x14]
        inc    dx
loc_1_079A:
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        in     al, dx
        and    al, 0xb8
        cmp    al, 0x98
        je     loc_1_07B1
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        cmp    eax, esi
        jb     loc_1_079A
loc_1_07B1:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        and    dword ptr [edi + 0x20], 0xffffffdf
        bt     dword ptr [edi + 0x20], 4
        jb     loc_1_07EB
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        add    eax, dword ptr [0x9be]        ; -> loc_1_09BE
        mov    dword ptr [edi + 0x3c], eax
        mov    edx, dword ptr [edi + 0x14]
        in     al, dx
        mov    ah, al
        mov    ecx, 0x80
loc_1_07DC:
        xor    eax, eax
        call   sub_1_0814
        jb     loc_1_07E8
        loop   loc_1_07DC
        clc    
loc_1_07E8:
        mov    al, ah
        out    dx, al
loc_1_07EB:
        popal  
        ret    
sub_1_07ED:
        pushal 
        cld    
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        add    eax, dword ptr [0x9be]        ; -> loc_1_09BE
        mov    dword ptr [edi + 0x3c], eax
        mov    edx, dword ptr [edi + 0x14]
        in     al, dx
        mov    ah, al
loc_1_0804:
        lodsb  al, byte ptr [esi]
        call   sub_1_0814
        jb     loc_1_080F
        loop   loc_1_0804
        clc    
loc_1_080F:
        mov    al, ah
        out    dx, al
        popal  
        ret    
sub_1_0814:
        pushal 
        push   eax
        mov    edx, dword ptr [edi + 0x14]
        inc    dx
loc_1_081B:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        cmp    dword ptr [edi + 0x3c], eax
        jbe    loc_1_0868
        in     al, dx
        test   al, 0x80
        jne    loc_1_083F
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        jmp    loc_1_081B
loc_1_083F:
        dec    dx
        pop    eax
        out    dx, al
        jmp    loc_1_0845
loc_1_0845:
        jmp    loc_1_0847
loc_1_0847:
        inc    dx
        inc    dx
        in     al, dx
        and    al, 0x1f
        or     al, 1
        out    dx, al
        jmp    loc_1_0853
loc_1_0853:
        jmp    loc_1_0855
loc_1_0855:
        push   ecx
        mov    ecx, 0x100
loc_1_085B:
        loop   loc_1_085B
        pop    ecx
        and    al, 0xfe
        out    dx, al
        jmp    loc_1_0863
loc_1_0863:
        jmp    loc_1_0865
loc_1_0865:
        popal  
        clc    
        ret    
loc_1_0868:
        pop    eax
        popal  
        stc    
        ret    
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0000Fh                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VPD     '                  ; DDB_Name
        dd  03C000014h                  ; DDB_Init_Order
        dd  VPD_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  VPD_PM_API               ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
loc_1_08C0:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        pushal 
        ljmp   0:0x5740000                   ; -> loc_1_0574
loc_1_08CC:
        dec    esp
        push   eax
        push   esp
        xor    dword ptr [eax], esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
        add    byte ptr [eax - 0x16], ah
        add    byte ptr [eax], al
        jl     loc_1_090F
        add    byte ptr [eax], al
        dec    esp
        push   eax
        push   esp
loc_1_090F:
        xor    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax - 0x16], ah
        add    byte ptr [eax], al
        test   byte ptr [0x504c0000], al
        push   esp
        xor    esp, dword ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], bl
        sub    ah, byte ptr [ebx + 0x31]
        xor    edi, dword ptr ss:[eax]
        xor    al, 0x44
        sbb    ebp, dword ptr [ecx]
        jae    loc_1_09C8
        cmp    dword ptr [edi + 0x1b], edx
        sbb    ebx, dword ptr [ebx]
        sbb    eax, dword ptr [edx + 0x1800ffff]
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 006h, 000h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 033h, 003h, 01Dh, 0E5h, 01Bh, 01Bh
loc_1_09BE:
        mov    al, byte ptr [0xf]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_09C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_02C5
        dd  loc_1_02D3
        dd  loc_1_02F4
        dd  loc_1_0316
        dd  loc_1_0399
        dd  loc_1_03D9
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
        dd  loc_1_0402               ; fixup
        dd  loc_1_04AC               ; fixup
        dd  loc_1_0517               ; fixup
        dd  loc_1_0541               ; fixup
loc_1_0A18:
        mov    dword ptr [0xa8000005], eax   ; -> loc_1_05A8
        add    eax, 0x5b10000                ; -> loc_1_05B1
        add    byte ptr [eax], al
        test   al, 5
        add    byte ptr [eax], al
        mov    dword ptr [0xa8000005], eax   ; -> loc_1_05A8
        db  005h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x86  R-X
loc_2_0000:
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [edi], ah            ; -> loc_1_0627
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    eax, dword ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        hlt    
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 4], bh
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
loc_2_0034:
        push   ebx
        inc    edi
        push   edx
        inc    ecx
        inc    edx
        dec    esp
        push   eax
        push   esp
        db  000h
loc_2_003D:
        dec    esp
        push   eax
        push   esp
loc_2_0040:
        js     loc_2_0083
        push   ebp
        push   esp
        dec    edi
        inc    ecx
        push   ebx
        push   ebx
        dec    ecx
        inc    edi
        dec    esi
        add    byte ptr [eax + edx*2 + 0x54], cl
loc_2_004F:
        js     0x95
        dec    ecx
        push   ebx
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        add    byte ptr [eax + edx*2 + 0x54], cl
        push   ebx
        ja     0xc7
        je     0xc3
        push   0x656d6954
        outsd  dx, dword ptr [esi]
        jne    0xdc
        add    byte ptr [edi + 0x50], dl
        push   ebx
        dec    esp
        push   eax
        push   esp
loc_2_006F:
        js     loc_2_0071
loc_2_0071:
        dec    esi
        dec    edi
        dec    esi
        dec    edx
        push   ebp
        dec    ebp
        inc    edx
        dec    edi
        dec    esp
        push   eax
        push   esp
loc_2_007C:
        js     loc_2_007E
loc_2_007E:
        push   eax
        push   edx
        dec    esi
        and    byte ptr [eax], ah
loc_2_0083:
        and    byte ptr [eax], ah
        db  020h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x174  R-X
loc_3_0000:
        push   ebp
        mov    ebp, esp
        push   3
        pop    ecx
        mov    edx, 0x8c0                    ; -> loc_1_08C0
        mov    eax, dword ptr [ebp + 0x14]
loc_3_000E:
        cmp    dword ptr [edx + 0x14], eax
        je     loc_3_001A
        lea    edx, [edx + 0x40]
        loop   loc_3_000E
        xor    edx, edx
loc_3_001A:
        mov    eax, edx
        leave  
        ret    
loc_3_001E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 0xc]
        test   esi, esi
        je     loc_3_006C
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [esi + 0x2c], eax
        jne    loc_3_005E
        cmp    ebx, dword ptr [esi + 0x1c]
        jne    loc_3_004F
        push   0
        push   dword ptr [esi + 0x28]
        call   dword ptr [esi + 0x24]
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_3_0072
        mov    eax, dword ptr [ebp + 0x10]
loc_3_004F:
        xchg   dword ptr [esi + 0x24], eax
        xchg   dword ptr [esi + 0x2c], eax
        mov    eax, dword ptr [esi + 0x28]
        xchg   dword ptr [esi + 0x30], eax
        mov    dword ptr [esi + 0x28], eax
loc_3_005E:
        cmp    ebx, dword ptr [esi + 0x1c]
        je     loc_3_0070
        xor    al, al
        call   sub_1_0338
        jae    loc_3_0070
loc_3_006C:
        xor    eax, eax
        jmp    loc_3_0072
loc_3_0070:
        or     al, 1
loc_3_0072:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0077:
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [esp + 0x14]
        test   esi, esi
        je     loc_3_00F6
        test   byte ptr [esi + 0x20], 8
        je     loc_3_0097
        mov    eax, dword ptr [esp + 0x14]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        cmp    dword ptr [esi + 0x1c], ebx
        je     loc_3_00F8
loc_3_0097:
        test   byte ptr [esi + 0x20], 0x80
        jne    loc_3_00A2
        call   sub_1_035B
loc_3_00A2:
        test   byte ptr [esi + 0x20], 4
        jne    loc_3_00F6
        mov    ecx, dword ptr [esi + 0x24]
        jecxz  loc_3_00D3
        cmp    dword ptr [esp + 0x20], 0
        je     loc_3_00F6
        push   0
        push   dword ptr [esi + 0x28]
        call   ecx
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_3_00F8
        mov    ecx, dword ptr [esi + 0x24]
        jecxz  loc_3_00D3
        mov    edx, dword ptr [esi + 0x28]
        mov    dword ptr [esi + 0x2c], ecx
        mov    dword ptr [esi + 0x30], edx
        or     byte ptr [esi + 0x20], 4
loc_3_00D3:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    al, al
        call   sub_1_0338
        jb     loc_3_00F6
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 0x28], eax
        mov    eax, dword ptr [esp + 0x14]
        jmp    loc_3_00F8
loc_3_00F6:
        xor    eax, eax
loc_3_00F8:
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_3_00FC:
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [esi + 0x2c], eax
        je     loc_3_0147
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    dword ptr [esi + 0x1c], ebx
        jne    loc_3_011C
        call   sub_1_063F
loc_3_011C:
        mov    dword ptr [esi + 0x24], 0
        btr    dword ptr [esi + 0x20], 2
        jae    loc_3_014B
        push   0
        push   dword ptr [esi + 0x30]
        push   dword ptr [esi + 0x2c]
        push   esi
        push   1
        call   sub_3_0077
        add    esp, 0x14
        push   1
        push   dword ptr [esi + 0x28]
        call   dword ptr [esi + 0x24]
        pop    ecx
        pop    ecx
loc_3_0147:
        and    byte ptr [esi + 0x20], 0xfb
loc_3_014B:
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_0156
        call   sub_1_037A
loc_3_0156:
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_3_015A:
        mov    eax, 0x161                    ; -> loc_3_0161
        clc    
        ret    
loc_3_0161:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 4
        ja     loc_3_0171
        jmp    dword ptr [eax*4]             ; -> obj4:0x0000
loc_3_0171:
        xor    eax, eax
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x14  RW-
        ; ---- dispatch table (5 entries) ----
        dd  loc_3_0000
        dd  sub_3_0077
        dd  loc_3_001E
        dd  loc_3_00FC
        dd  loc_6_0000
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x19  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 00Bh, 066h, 033h, 0D2h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h
        db  0C3h, 033h, 0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x44  R-X
loc_6_0000:
        mov    edx, dword ptr [esp + 0xc]
        inc    edx
        in     al, dx
        mov    ch, al
        inc    edx
        in     al, dx
        mov    cl, al
        xor    eax, eax
        cmp    cl, 0xff
        je     loc_6_0043
        cmp    cl, ch
        je     loc_6_0043
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 0xc]
loc_6_001D:
        movzx  edx, word ptr [ecx*2 + 0x408]
        cmp    eax, edx
        je     loc_6_0043
        test   edx, edx
        je     loc_6_003B
        test   dh, 0xff
        je     loc_6_0043
        inc    cl
        cmp    cl, 2
        jbe    loc_6_001D
        jmp    loc_6_0043
loc_6_003B:
        mov    word ptr [ecx*2 + 0x408], ax
loc_6_0043:
        ret    
VxD__LTEXT ENDS
