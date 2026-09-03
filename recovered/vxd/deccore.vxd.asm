; DECCORE -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 2125 instructions recovered by recursive descent

VxD__LTEXT SEGMENT   ; object 1  vsize=0x4c4  R-X
loc_1_0000:
        call   sub_1_0322
        clc    
        ret    
loc_1_0007:
        mov    dword ptr [0x20fdc], ebx      ; -> loc_2_0FDC
        call   sub_1_0058
        call   sub_1_01E8
        mov    eax, 0x37
        call   sub_2_01E1
        or     dword ptr [0x20fd8], 1        ; -> loc_2_0FD8
        bt     dword ptr [0x20fd8], 1        ; -> loc_2_0FD8
        jb     loc_1_003E
        call   sub_1_0348
        call   sub_1_0150
        clc    
        ret    
loc_1_003E:
        stc    
        ret    
loc_1_0040:
        mov    byte ptr [0x21228], 0         ; -> loc_2_1228
        xor    dword ptr [0x20fd8], 1        ; -> loc_2_0FD8
        xor    dword ptr [0x210b4], 8        ; -> loc_2_10B4
        clc    
        ret    
        db  000h
sub_1_0058:
        mov    word ptr [0x210c8], 1         ; -> loc_2_10C8
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        mov    word ptr [0x210b2], ax        ; -> loc_2_10B2
        jecxz  loc_1_0076
        or     dword ptr [0x210b4], 4        ; -> loc_2_10B4
loc_1_0076:
        VMMCall  VMM.Test_Debug_Installed        ; cd20c1000100
        je     loc_1_0085
        or     dword ptr [0x210b4], 1        ; -> loc_2_10B4
loc_1_0085:
        mov    dword ptr [0x210c0], 0x37     ; -> loc_2_10C0
        or     dword ptr [0x210b4], 8        ; -> loc_2_10B4
        mov    eax, 0x24
        mov    dword ptr [0x210bc], 0x10     ; -> loc_2_10BC
        mul    dword ptr [0x210bc]           ; -> loc_2_10BC
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [0x210b8], eax      ; -> loc_2_10B8
        or     eax, eax
        jne    loc_1_00C7
        or     dword ptr [0x20fd8], 2        ; -> loc_2_0FD8
loc_1_00C7:
        VMMCall  VMM.Get_Config_Directory        ; cd20b7000100
        mov    esi, edx
        cld    
        mov    edi, 0x210d0                  ; -> loc_2_10D0
        mov    dword ptr [0x210c4], edi      ; -> loc_2_10C4
        mov    ecx, 0x40
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   0
        VMMCall  VMM._PageGetAllocInfo           ; cd2059000100
        add    esp, 4
        mov    dword ptr [0x210cc], edx      ; -> loc_2_10CC
        push   edx
        push   ebx
        VMMCall  VMM.Get_Machine_Info            ; cd20bb000100
        bt     ebx, 0x10
        jae    loc_1_014D
        or     dword ptr [0x210b4], 0x200    ; -> loc_2_10B4
        pushfd 
        cli    
        pushfd 
        pop    eax
        mov    ebx, eax
        xor    eax, 0x200000
        push   eax
        popfd  
        pushfd 
        pop    eax
        popfd  
        xor    eax, ebx
        je     loc_1_014D
        mov    eax, 1
        cpuid  
        and    eax, 0xf00
        cmp    eax, 0x500
        jb     loc_1_014D
        or     dword ptr [0x210b4], 0x400    ; -> loc_2_10B4
        cmp    eax, 0x600
        jb     loc_1_014D
        or     dword ptr [0x210b4], 0x800    ; -> loc_2_10B4
loc_1_014D:
        pop    ebx
        pop    edx
        ret    
sub_1_0150:
        push   0x10400                       ; -> loc_1_0400
        push   0x200
        call   sub_2_0218
        add    esp, 8
        mov    byte ptr [0x10424], 1         ; -> obj1:0x0424
        mov    byte ptr [0x10425], 0         ; -> loc_1_0425
        mov    word ptr [0x10426], 0x19      ; -> obj1:0x0426
        push   0x10414                       ; -> loc_1_0414
        push   0x100
        call   sub_2_0218
        add    esp, 8
        ret    
loc_1_018C:
        mov    esi, dword ptr [ebp + 0xc]
        xor    eax, eax
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    ecx, 3
        ja     loc_1_01DD
        shl    ecx, 2
        jmp    loc_1_03C0
loc_1_01A5:
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        jmp    loc_1_01BB
loc_1_01AD:
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        jmp    loc_1_01BB
loc_1_01B5:
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
loc_1_01BB:
        jb     loc_1_01D5
        je     loc_1_01D5
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx], eax
        ret    
loc_1_01C5:
        xor    edx, edx
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
        jb     loc_1_01D5
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx], edx
        ret    
loc_1_01D5:
        mov    dword ptr [ebp - 4], 0xffffffff
        ret    
loc_1_01DD:
        mov    dword ptr [ebp - 4], 0xffffff9a
        ret    
        db  000h, 000h, 000h
sub_1_01E8:
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x10448                  ; -> obj1:0x0448
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        or     eax, eax
        je     loc_1_0202
        mov    byte ptr [0x21228], 0xff      ; -> loc_2_1228
loc_1_0202:
        mov    eax, 0x2000
        mov    edi, 0x10438                  ; -> obj1:0x0438
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        cmp    eax, 0x400
        jl     loc_1_0249
        cmp    eax, 0x32000
        jg     loc_1_0249
loc_1_0220:
        mov    dword ptr [0x21214], eax      ; -> loc_2_1214
        sub    dword ptr [0x21214], 5        ; -> loc_2_1214
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [0x2120c], eax      ; -> loc_2_120C
        or     eax, eax
        jne    loc_1_0248
        or     dword ptr [0x20fd8], 2        ; -> loc_2_0FD8
loc_1_0248:
        ret    
loc_1_0249:
        mov    eax, 0x2000
        jmp    loc_1_0220
sub_1_0250:
        cld    
        mov    byte ptr [0x213a7], 0         ; -> obj2:0x13A7
        dec    ecx
        mov    dword ptr [0x21394], ecx      ; -> loc_2_1394
        mov    dword ptr [0x21390], edi      ; -> loc_2_1390
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebp
        sub    ebp, 0x18
        mov    dword ptr [ebp + 8], 0x10000600
        test   byte ptr [0x213a8], 0xff      ; -> loc_2_13A8
        je     loc_1_028D
        mov    byte ptr [0x213a8], 0         ; -> loc_2_13A8
        mov    dword ptr [ebp + 8], 0x10000605
loc_1_028D:
        mov    dword ptr [ebp + 0xc], 0x21398 ; -> loc_2_1398
        mov    dword ptr [ebp + 0x10], 0x213a3 ; -> obj2:0x13A3
        mov    dword ptr [ebp + 0x14], 0
        call   sub_2_0A80
        or     eax, eax
        jne    loc_1_0319
        mov    dword ptr [ebp + 8], 0x10000602
        mov    eax, dword ptr [0x213a3]      ; -> obj2:0x13A3
        mov    dword ptr [ebp + 0xc], eax
        mov    dword ptr [ebp + 0x10], 0
        mov    dword ptr [ebp + 0x14], 2
        call   sub_2_0A80
        or     eax, eax
        js     loc_1_0319
        mov    dword ptr [ebp + 8], 0x10000603
        mov    eax, dword ptr [0x213a3]      ; -> obj2:0x13A3
        mov    dword ptr [ebp + 0xc], eax
        mov    eax, dword ptr [0x21394]      ; -> loc_2_1394
        mov    dword ptr [ebp + 0x10], eax
        mov    eax, dword ptr [0x21390]      ; -> loc_2_1390
        mov    dword ptr [ebp + 0x14], eax
        call   sub_2_0A80
        or     eax, eax
        js     loc_1_0319
        mov    dword ptr [ebp + 8], 0x10000604
        mov    eax, dword ptr [0x213a3]      ; -> obj2:0x13A3
        mov    dword ptr [ebp + 0xc], eax
        call   sub_2_0A80
loc_1_030D:
        mov    byte ptr [0x213a7], 0xff      ; -> obj2:0x13A7
        pop    ebp
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0319:
        mov    byte ptr [0x21228], 0         ; -> loc_2_1228
        jmp    loc_1_030D
sub_1_0322:
        push   0x1000
        push   0x3f0
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_1_033F
        mov    dword ptr [0x21378], eax      ; -> loc_2_1378
        ret    
loc_1_033F:
        or     dword ptr [0x20fd8], 2        ; -> loc_2_0FD8
        ret    
        db  000h
sub_1_0348:
        xor    ebx, ebx
        VxDCall  VNETBIOS.VNETBIOS_Get_Version   ; cd2000001400
        jb     loc_1_03BC
        cmp    eax, 0x30a
        jb     loc_1_03BC
        test   ebx, 1
        je     loc_1_03BC
        or     dword ptr [0x20fd8], 0x200    ; -> loc_2_0FD8
        or     dword ptr [0x210b4], 0x20     ; -> loc_2_10B4
        mov    byte ptr [0x1048c], ah        ; -> loc_1_048C
        mov    byte ptr [0x1048d], al        ; -> obj1:0x048D
        mov    byte ptr [0x104b0], ah        ; -> loc_1_04B0
        mov    byte ptr [0x104b1], al        ; -> obj1:0x04B1
        push   0x1047c                       ; -> obj1:0x047C
        push   0x100
        call   sub_2_0218
        add    esp, 8
        push   0x104a0                       ; -> obj1:0x04A0
        push   0x100
        call   sub_2_0218
        add    esp, 8
        VxDCall  Dev_0481.Service_0000h          ; cd2000008104
        jb     loc_1_03BD
        or     dword ptr [0x210b4], 0x10     ; -> loc_2_10B4
        ret    
loc_1_03BC:
        ret    
loc_1_03BD:
        ret    
        db  000h, 000h
loc_1_03C0:
        movsd  dword ptr es:[edi], dword ptr [esi]
        add    dword ptr [ecx], eax
        add    byte ptr [ebp - 0x4afffeff], ch ; -> loc_1_01AD
        add    dword ptr [ecx], eax
        add    ch, al                        ; -> loc_1_01C5
        add    dword ptr [ecx], eax
        add    byte ptr [ebp + eax*2 + 0x43], al
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        and    byte ptr [esi + 0x31], dl
        xor    byte ptr cs:[esi], ch
        xor    byte ptr [edx], dh
        xor    eax, 0x616f4c20
        and    byte ptr fs:[ebx + 0x75], dl
        arpl   word ptr [ebx + 0x65], sp
        jae    loc_1_0462
        jne    loc_1_045E
        insb   byte ptr es:[edi], dx
        jns    loc_1_0423
        or     eax, 0x4f43000a
        push   edx
        inc    ebp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_0400:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        clc    
        add    eax, dword ptr [ecx]
        add    byte ptr [ebx + 0x20], dl
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        rol    byte ptr [ebx], 1
        add    dword ptr [eax], eax
loc_1_0414:
        inc    esp
        inc    ebp
        inc    ebx
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        xor    byte ptr [ebp + eax*2 + 0x43], al
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
loc_1_0423:
        and    byte ptr [eax], al
loc_1_0425:
        add    byte ptr [eax], al
        add    byte ptr [eax], bl            ; -> sub_2_0218
        add    al, byte ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4f], al
        push   edx
        inc    ebp
        push   ebp
        jae    loc_1_04A4
        jb     loc_1_0483
        jne    0x4a9
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x524f4300
        inc    ebp
        push   edi
        jb     0x4b8
        je     0x4b6
        push   ebp
        jae    0x4b9
        jb     0x498
        jne    0x4be
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebx
        dec    edi
loc_1_045E:
        push   edx
        inc    ebp
        inc    ecx
        insb   byte ptr es:[edi], dx
loc_1_0462:
        insb   byte ptr es:[edi], dx
        dec    ebp
        jae    0x4da
        popal  
        jae    loc_1_046C
loc_1_046C:
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        inc    esp
        jne    0x4e3
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6574
        add    byte ptr [esi + 0x45], cl
        push   esp
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
loc_1_0483:
        xor    byte ptr [esi + 0x4e], dl
        inc    ebp
        push   esp
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
loc_1_048C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        insb   byte ptr es:[edi], dx
        or     al, 2
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x45], cl
        push   esp
        inc    edx
loc_1_04A4:
        pop    edi
        inc    ebx
        inc    edx
        xor    byte ptr [esi + 0x4e], dl
        inc    ebp
        push   esp
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
loc_1_04B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        lcall  0x4000, 0x20c
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x13f8  R-X
loc_2_0000:
        cmp    eax, 0
        jne    loc_2_000A
        jmp    loc_1_0000
loc_2_000A:
        cmp    eax, 1
        jne    loc_2_0014
        jmp    loc_1_0007
loc_2_0014:
        cmp    eax, 2
        jne    loc_2_001E
        jmp    loc_1_0040
loc_2_001E:
        cmp    eax, 0x23
        jne    loc_2_0028
        jmp    loc_2_0257
loc_2_0028:
        cmp    eax, 4
        jne    loc_2_002F
        jmp    loc_2_0038
loc_2_002F:
        cmp    eax, 0xb
        jne    loc_2_0036
        jmp    loc_2_0038
loc_2_0036:
        clc    
        ret    
loc_2_0038:
        cmp    ebx, dword ptr [0x20fdc]      ; -> loc_2_0FDC
        jne    loc_2_004A
        or     dword ptr [0x20fd8], 0x100    ; -> loc_2_0FD8
loc_2_004A:
        ret    
loc_2_004B:
        xor    ecx, ecx
        mov    eax, 0x1019
        bt     dword ptr [0x20fd8], 1        ; -> loc_2_0FD8
        jae    loc_2_005D
        ret    
loc_2_005D:
        mov    ecx, 0x20218                  ; -> sub_2_0218
        ret    
loc_2_0063:
        movzx  eax, word ptr [ebp + 0x1c]
        test   eax, eax
        jne    loc_2_0096
        mov    word ptr [ebp + 0x1c], ax
        and    word ptr [ebp + 0x2c], 0xfffe
        mov    word ptr [ebp + 0x10], 0x1019
        mov    edx, dword ptr [0x21218]      ; -> loc_2_1218
        mov    word ptr [ebp + 0x18], dx
        mov    edx, dword ptr [0x210b4]      ; -> loc_2_10B4
        mov    word ptr [ebp + 4], dx
        shr    edx, 0x10
        mov    word ptr [ebp], dx
        ret    
loc_2_0096:
        cmp    eax, 6
        ja     loc_2_00A0
        jmp    loc_2_0657
loc_2_00A0:
        cmp    eax, 7
        ja     loc_2_00AA
        jmp    loc_2_0DFC
loc_2_00AA:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_2_00B0:
        cmp    cl, 1
        jb     loc_2_00CC
        cmp    cl, 2
        je     loc_2_01EF
        ja     loc_2_00E1
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        ret    
loc_2_00CC:
        mov    ecx, 0xa
        mov    esi, 0x21004                  ; -> obj2:0x1004
loc_2_00D6:
        cmp    dword ptr [esi + 8], 0
        je     loc_2_00E9
        add    esi, 0xc
        loop   loc_2_00D6
loc_2_00E1:
        mov    dword ptr [ebp - 4], 0xffffff90
        ret    
loc_2_00E9:
        mov    ecx, 0x37
        mov    eax, dword ptr [ebp + 0x10]
        xor    edx, edx
        div    ecx
        cmp    edx, 0x1b
        jl     loc_2_00FB
        inc    eax
loc_2_00FB:
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi], eax
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 8], edx
        ret    
        db  090h, 090h, 090h
loc_2_010A:
        mov    eax, 0x37
        add    ecx, dword ptr [0x20ffc]      ; -> loc_2_0FFC
        sub    ecx, 0x37
        neg    ecx
        add    eax, ecx
        mov    dword ptr [0x20ffc], eax      ; -> loc_2_0FFC
        cmp    eax, 0x14
        jl     loc_2_0190
loc_2_0126:
        call   sub_2_01E1
        bts    dword ptr [0x20fd8], 2        ; -> loc_2_0FD8
        jb     loc_2_0183
loc_2_0135:
        mov    esi, 0x21004                  ; -> obj2:0x1004
        mov    ecx, 0xa
loc_2_013F:
        cmp    dword ptr [esi + 8], 0
        je     loc_2_0162
        dec    dword ptr [esi]
        jne    loc_2_0162
        mov    edx, dword ptr [esi + 4]
        cmp    eax, 0x4f
        jl     loc_2_0157
        cmp    edx, 4
        jl     loc_2_0157
        dec    edx
loc_2_0157:
        mov    dword ptr [esi], edx
        pushal 
        xor    ebx, ebx
        mov    ebp, esp
        call   dword ptr [esi + 8]
        popal  
loc_2_0162:
        add    esi, 0xc
        loop   loc_2_013F
        btr    dword ptr [0x20fd8], 3        ; -> loc_2_0FD8
        jb     loc_2_0135
        btr    dword ptr [0x20fd8], 6        ; -> loc_2_0FD8
        jb     loc_2_0135
        and    dword ptr [0x20fd8], 0xfffffffb ; -> loc_2_0FD8
        ret    
loc_2_0183:
        mov    edx, dword ptr [0x20ffc]      ; -> loc_2_0FFC
        add    dword ptr [0x21000], edx      ; -> obj2:0x1000
        ret    
loc_2_0190:
        add    eax, 0x37
        or     dword ptr [0x20fd8], 8        ; -> loc_2_0FD8
        mov    dword ptr [0x20ffc], 0x37     ; -> loc_2_0FFC
        cmp    eax, 0x14
        jge    loc_2_0126
        add    eax, 0x37
        or     dword ptr [0x20fd8], 0x40     ; -> loc_2_0FD8
        cmp    eax, 0x14
        jge    loc_2_0126
        mov    edx, 0x14
        sub    edx, eax
        je     loc_2_01CD
        jns    loc_2_01D0
        neg    edx
loc_2_01CD:
        add    edx, 0x14
loc_2_01D0:
        add    dword ptr [0x21000], edx      ; -> obj2:0x1000
        mov    eax, 0x37
        jmp    loc_2_0126
        db  090h
sub_2_01E1:
        xor    edx, edx
        mov    esi, 0x2010a                  ; -> loc_2_010A
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        ret    
loc_2_01EF:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, 0xa
        mov    esi, 0x21004                  ; -> obj2:0x1004
loc_2_01FC:
        cmp    dword ptr [esi + 8], eax
        je     loc_2_020B
        add    esi, 0xc
        loop   loc_2_01FC
        jmp    loc_2_00E1
loc_2_020B:
        xor    ecx, ecx
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi], ecx
        ret    
        db  000h, 000h
sub_2_0218:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   ebx
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0xfffffdab
        cmp    byte ptr [ebp + 9], 2
        je     loc_2_023B
        bt     dword ptr [0x20fd8], 1        ; -> loc_2_0FD8
        jb     loc_2_0250
loc_2_023B:
        movzx  eax, byte ptr [ebp + 9]
        cld    
        mov    dword ptr [ebp - 4], 0
        shl    eax, 2
        call   sub_2_1088
loc_2_0250:
        cld    
        pop    edi
        pop    ebx
        pop    esi
        pop    eax
        pop    ebp
        ret    
loc_2_0257:
        mov    ebx, dword ptr [0x20fdc]      ; -> loc_2_0FDC
        mov    ebp, dword ptr [ebx + 8]
        mov    ecx, dword ptr [esi + 0xc]
        jecxz  loc_2_02A1
        cmp    ecx, -1
        je     loc_2_02A6
        cmp    ecx, 0x201
        je     loc_2_027F
        mov    eax, 0x16
        cmp    ecx, 0x200
        jne    loc_2_02A8
loc_2_027F:
        mov    eax, 0x57
        mov    edx, dword ptr [esi + 0x10]
        cmp    edx, 0x400000
        jb     loc_2_02A8
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_2_02A8
        push   edx
        push   ecx
        call   sub_2_0218
        add    esp, 8
        clc    
        ret    
loc_2_02A1:
        mov    ecx, 0x1019
loc_2_02A6:
        xor    eax, eax
loc_2_02A8:
        clc    
        ret    
        db  000h, 000h
loc_2_02AC:
        mov    edx, dword ptr [ebp + 0xc]
        cmp    cl, 1
        jb     loc_2_02BE
        je     loc_2_02DC
loc_2_02B6:
        mov    dword ptr [ebp - 4], 0xffffff95
        ret    
loc_2_02BE:
        mov    edi, edx
        push   1
        push   edx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_2_02B6
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx], eax
        add    dword ptr [0x21110], edi      ; -> loc_2_1110
        ret    
loc_2_02DC:
        mov    ecx, dword ptr [edx]
        push   ecx
        push   0
        push   ecx
        VMMCall  VMM._HeapGetSize                ; cd2052000100
        add    esp, 8
        pop    ecx
        je     loc_2_02B6
        sub    dword ptr [0x21110], eax      ; -> loc_2_1110
        push   0
        push   ecx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        or     eax, eax
        je     loc_2_02B6
        ret    
loc_2_0305:
        cmp    cl, 1
        ja     loc_2_0D5F
        je     loc_1_018C
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx], 0x210b0      ; -> obj2:0x10B0
        mov    eax, dword ptr [ebp + 0x10]
        xor    edx, edx
        mov    ecx, 0xa
        div    ecx
        cmp    eax, 1
        je     loc_2_0350
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0x80000030
        or     eax, 0x40001000
        mov    ecx, 0x2112b                  ; -> loc_2_112B
        mov    edi, 0x21114                  ; -> obj2:0x1114
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        jmp    loc_2_0351
loc_2_0350:
        ret    
loc_2_0351:
        mov    dword ptr [ebp - 4], 0xffffff9b
        ret    
        db  000h, 000h, 000h
loc_2_035C:
        mov    edx, ecx
        mov    eax, 0x210b0                  ; -> obj2:0x10B0
        mov    ecx, dword ptr [eax + 0xc]
        dec    ecx
        mov    edi, dword ptr [eax + 8]
        xor    eax, eax
        cmp    dl, 1
        jae    loc_2_03C9
loc_2_0371:
        cmp    dword ptr [edi], eax
        je     loc_2_037C
        add    edi, 0x24
        loop   loc_2_0371
        jmp    loc_2_03C1
loc_2_037C:
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, 0x24
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x14]
        mov    eax, dword ptr [esi + 0x18]
        cmp    eax, 0x10100
        jne    loc_2_039C
        mov    dword ptr [0x211a4], ecx      ; -> loc_2_11A4
loc_2_039C:
        cmp    eax, 0x40100
        jne    loc_2_03AB
        mov    edi, esi
        stc    
        call   sub_2_046F
loc_2_03AB:
        mov    edx, eax
        shr    edx, 8
        cmp    edx, 0x402
        jne    loc_2_03C0
        mov    edi, dword ptr [esi + 0x1c]
        call   sub_2_0CB3
loc_2_03C0:
        ret    
loc_2_03C1:
        mov    dword ptr [ebp - 4], 0xffffff99
        ret    
loc_2_03C9:
        cmp    dl, 2
        ja     loc_2_042B
        cld    
loc_2_03CF:
        mov    esi, dword ptr [ebp + 0xc]
        push   ecx
        push   edi
        mov    ecx, 8
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    edi
        pop    ecx
        je     loc_2_03E6
        add    edi, 0x24
        loop   loc_2_03CF
        jmp    loc_2_03F8
loc_2_03E6:
        mov    ecx, 0x24
        cmp    dl, 2
        je     loc_2_0400
        mov    esi, dword ptr [ebp + 0xc]
        xchg   edi, esi
        rep movsb byte ptr es:[edi], byte ptr [esi]
        ret    
loc_2_03F8:
        mov    dword ptr [ebp - 4], 0xffffffff
        ret    
loc_2_0400:
        mov    edx, dword ptr [edi + 0x18]
        cmp    edx, 0x40100
        jne    loc_2_0411
        clc    
        call   sub_2_046F
loc_2_0411:
        mov    esi, dword ptr [edi + 0x14]
        xor    al, al
        rep stosb byte ptr es:[edi], al
        mov    eax, edx
        shr    edx, 8
        cmp    edx, 0x402
        jne    loc_2_042A
        call   sub_2_0D1F
loc_2_042A:
        ret    
loc_2_042B:
        cmp    dl, 3
        ja     loc_2_03C1
loc_2_0430:
        mov    eax, dword ptr [edi + 0x18]
        shr    eax, 0x10
        cmp    al, 4
        jne    loc_2_0469
        push   edi
        push   ecx
        cld    
        lea    esi, [edi + 8]
        mov    edi, 0x211a8                  ; -> loc_2_11A8
        mov    ecx, 8
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_044C:
        dec    edi
        cmp    byte ptr [edi], 0x20
        jne    loc_2_0457
        mov    byte ptr [edi], 0
        jmp    loc_2_044C
loc_2_0457:
        mov    ebx, 0xffffffff
        mov    edx, 0x211a8                  ; -> loc_2_11A8
        VxDCall  APM.Service_0002h               ; cd2002002700
        pop    ecx
        pop    edi
loc_2_0469:
        add    edi, 0x24
        loop   loc_2_0430
        ret    
sub_2_046F:
        pushal 
        mov    ecx, dword ptr [0x211a4]      ; -> loc_2_11A4
        jecxz  loc_2_047C
        call   ecx
loc_2_047A:
        popal  
        ret    
loc_2_047C:
        jae    loc_2_047A
        mov    word ptr [0x20ff0], 0x2045    ; -> loc_2_0FF0
        mov    dword ptr [0x20ff4], 2        ; -> loc_2_0FF4
        mov    dword ptr [0x20ff8], 0x211b4  ; -> loc_2_0FF8
        push   0x20fe8                       ; -> loc_2_0FE8
        push   0x201
        call   sub_2_0218
        add    esp, 8
        jmp    loc_2_047A
        db  000h
loc_2_04B0:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        cmp    cl, 1
        jb     sub_2_04EF
        je     sub_2_061D
        bt     dword ptr [0x210b4], 2        ; -> loc_2_10B4
        jae    loc_2_04D1
        call   sub_2_08D0
        ret    
loc_2_04D1:
        mov    esi, dword ptr [ebp + 0xc]
        or     esi, esi
        jne    loc_2_04DF
        mov    dword ptr [ebp + 0xc], 0x2123b ; -> obj2:0x123B
loc_2_04DF:
        call   sub_2_061D
        mov    esi, dword ptr [ebp + 0xc]
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
sub_2_04EF:
        cld    
        push   ebx
        xor    ecx, ecx
        cmp    dword ptr [0x2120c], ecx      ; -> loc_2_120C
        je     loc_2_0612
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebx], 1
        jne    loc_2_0612
        cmp    dword ptr [ebx + 4], ecx
        je     loc_2_0612
        cmp    dword ptr [ebx + 0xc], 0x270f
        jg     loc_2_0612
        mov    edi, dword ptr [ebx + 0x10]
        test   edi, edi
        je     loc_2_0552
        xor    al, al
        mov    ecx, 0x200
        repne scasb al, byte ptr es:[edi]
        jne    loc_2_0612
        sub    edi, dword ptr [ebx + 0x10]
        mov    ecx, edi
        mov    eax, dword ptr [0x21214]      ; -> loc_2_1214
        sub    eax, dword ptr [0x21210]      ; -> loc_2_1210
        sub    eax, 0xe
        cmp    eax, ecx
        jl     loc_2_060C
loc_2_0552:
        mov    edi, dword ptr [0x2120c]      ; -> loc_2_120C
        add    edi, dword ptr [0x21210]      ; -> loc_2_1210
        mov    dx, word ptr [ebx + 8]
        cmp    dh, 0x20
        je     loc_2_0570
        cmp    dh, 0x2a
        jne    loc_2_0612
loc_2_0570:
        push   edi
        mov    word ptr [edi], dx
        add    dword ptr [0x21210], 2        ; -> loc_2_1210
        add    edi, 2
        push   ecx
        mov    esi, dword ptr [ebx + 4]
        mov    ecx, 4
loc_2_0587:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x20
        jae    loc_2_058E
        mov    al, 0x20
loc_2_058E:
        stosb  byte ptr es:[edi], al
        loop   loc_2_0587
        mov    al, 0x20
        stosb  byte ptr es:[edi], al
        add    dword ptr [0x21210], 0xa      ; -> loc_2_1210
        mov    ecx, 4
        mov    al, 0x30
        rep stosb byte ptr es:[edi], al
        mov    al, 0x20
        stosb  byte ptr es:[edi], al
        push   edi
        sub    edi, 2
        mov    eax, dword ptr [ebx + 0xc]
        mov    ecx, 0xa
loc_2_05B3:
        xor    edx, edx
        or     eax, eax
        je     loc_2_05C3
        div    ecx
        add    dl, 0x30
        mov    byte ptr [edi], dl
        dec    edi
        jmp    loc_2_05B3
loc_2_05C3:
        pop    edi
        pop    ecx
        jecxz  loc_2_05E4
        mov    esi, dword ptr [ebx + 0x10]
        add    dword ptr [0x21210], ecx      ; -> loc_2_1210
        push   ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [edi], ecx
        pop    ecx
        add    ecx, 0xc
        sub    edi, 3
        cmp    byte ptr [edi], 0xd
        je     loc_2_05F4
        add    edi, 2
loc_2_05E4:
        add    ecx, 2
        mov    dword ptr [edi], 0xa0d
        add    dword ptr [0x21210], 2        ; -> loc_2_1210
loc_2_05F4:
        pop    edi
        pop    ebx
        push   edi
        test   byte ptr [0x21228], 0xff      ; -> loc_2_1228
        je     loc_2_0605
        call   sub_1_0250
loc_2_0605:
        call   sub_2_077C
        pop    edi
        ret    
loc_2_060C:
        inc    dword ptr [0x21218]           ; -> loc_2_1218
loc_2_0612:
        pop    ebx
        xor    edi, edi
        mov    dword ptr [ebp - 4], 0xffffff97
        ret    
sub_2_061D:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 9], 0x2a
        call   sub_2_04EF
        mov    ecx, edi
        jecxz  loc_2_064F
        cmp    dword ptr [0x2121c], 0        ; -> loc_2_121C
        jne    loc_2_064E
        add    ecx, 2
        mov    eax, 0x80000030
        or     eax, 0x40001000
        mov    edi, 0x21229                  ; -> obj2:0x1229
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
loc_2_064E:
        ret    
loc_2_064F:
        mov    dword ptr [ebp - 4], 0xffffff8c
        ret    
loc_2_0657:
        cmp    ax, 2
        ja     loc_2_06C0
        push   eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    edi, eax
        pop    eax
        cmp    edi, -1
        je     loc_2_06B2
        mov    esi, dword ptr [0x2120c]      ; -> loc_2_120C
        mov    ecx, dword ptr [0x21210]      ; -> loc_2_1210
        jecxz  loc_2_06AC
        movzx  edx, word ptr [ebp + 0x18]
        cmp    edx, ecx
        jbe    loc_2_06B2
        mov    word ptr [ebp + 0x18], cx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    ax, 2
        je     loc_2_06A0
        mov    dword ptr [0x21210], ecx      ; -> loc_2_1210
        mov    esi, dword ptr [0x2120c]      ; -> loc_2_120C
        mov    dword ptr [esi], ecx
loc_2_06A0:
        mov    edx, dword ptr [0x21218]      ; -> loc_2_1218
        mov    word ptr [ebp + 0x10], dx
        jmp    loc_2_0712
loc_2_06AC:
        mov    word ptr [ebp + 0x18], cx
        jmp    loc_2_06A0
loc_2_06B2:
        mov    word ptr [ebp + 0x18], cx
        mov    word ptr [ebp + 0x10], cx
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_2_06C0:
        cmp    ax, 5
        jge    loc_2_07CA
        test   dword ptr [ebx], 0x20
        je     loc_2_06B2
        cmp    ebx, dword ptr [0x20fdc]      ; -> loc_2_0FDC
        jne    loc_2_06B2
        mov    di, word ptr [ebp + 0x38]
        shl    edi, 0x10
        mov    di, word ptr [ebp]
        mov    ecx, dword ptr [0x2121c]      ; -> loc_2_121C
        cmp    ax, 3
        je     loc_2_06FD
        xor    ecx, edi
        jne    loc_2_06B2
        mov    dword ptr [0x2121c], ecx      ; -> loc_2_121C
        jmp    loc_2_0712
loc_2_06FD:
        mov    dword ptr [0x2121c], edi      ; -> loc_2_121C
        movzx  ecx, word ptr [ebp + 0x10]
        mov    dword ptr [0x21224], ecx      ; -> loc_2_1224
        call   sub_2_077C
loc_2_0712:
        xor    eax, eax
        mov    word ptr [ebp + 0x1c], ax
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_2_071E:
        mov    dword ptr [0x21220], edx      ; -> loc_2_1220
        mov    edi, dword ptr [0x2121c]      ; -> loc_2_121C
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        btr    dword ptr [0x21224], 0x1f     ; -> loc_2_1224
        mov    eax, dword ptr [0x21224]      ; -> loc_2_1224
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ecx, edi
        shr    ecx, 0x10
        movzx  edx, di
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
sub_2_077C:
        cmp    dword ptr [0x21210], 2        ; -> loc_2_1210
        jb     loc_2_07C9
        mov    ecx, dword ptr [0x2121c]      ; -> loc_2_121C
        jecxz  loc_2_07C9
        bt     dword ptr [0x20fd8], 8        ; -> loc_2_0FD8
        jb     loc_2_07C9
        bts    dword ptr [0x21224], 0x1f     ; -> loc_2_1224
        jb     loc_2_07C9
        mov    eax, 0x10
        mov    ebx, dword ptr [0x20fdc]      ; -> loc_2_0FDC
        mov    ecx, 0xb
        xor    edx, edx
        mov    esi, 0x2071e                  ; -> loc_2_071E
        mov    edi, 0x7fffffff
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [0x21220], esi      ; -> loc_2_1220
loc_2_07C9:
        ret    
loc_2_07CA:
        movzx  ecx, word ptr [ebp + 0x38]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x10]
        mov    dword ptr [0x212a4], ecx      ; -> loc_2_12A4
        mov    dword ptr [0x212ac], 0x14     ; -> loc_2_12AC
        mov    esi, 0x212a4                  ; -> loc_2_12A4
        call   sub_2_0EFE
        jb     loc_2_08A3
        mov    edi, 0x21290                  ; -> loc_2_1290
        mov    esi, dword ptr [0x212a8]      ; -> loc_2_12A8
        mov    ecx, 0x14
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [0x21294]      ; -> loc_2_1294
        mov    dword ptr [0x212b8], ecx      ; -> loc_2_12B8
        mov    dword ptr [0x212c0], 9        ; -> loc_2_12C0
        mov    esi, 0x212b8                  ; -> loc_2_12B8
        call   sub_2_0EFE
        jb     loc_2_0899
        mov    ecx, dword ptr [0x212bc]      ; -> loc_2_12BC
        mov    dword ptr [0x21294], ecx      ; -> loc_2_1294
        mov    ecx, dword ptr [0x212a0]      ; -> loc_2_12A0
        mov    dword ptr [0x212cc], ecx      ; -> loc_2_12CC
        mov    dword ptr [0x212d4], 0x200    ; -> loc_2_12D4
        mov    esi, 0x212cc                  ; -> loc_2_12CC
        call   sub_2_0EFE
        jb     loc_2_088F
        mov    ecx, dword ptr [0x212d0]      ; -> loc_2_12D0
        mov    dword ptr [0x212a0], ecx      ; -> loc_2_12A0
        push   0x21290                       ; -> loc_2_1290
        mov    ecx, 0x200
        cmp    eax, 6
        jne    loc_2_0877
        mov    ecx, 0x201
loc_2_0877:
        push   ecx
        call   sub_2_0218
        add    esp, 8
        mov    dword ptr [0x212e0], eax      ; -> obj2:0x12E0
        mov    esi, 0x212cc                  ; -> loc_2_12CC
        call   sub_2_0F7B
loc_2_088F:
        mov    esi, 0x212b8                  ; -> loc_2_12B8
        call   sub_2_0F7B
loc_2_0899:
        mov    esi, 0x212a4                  ; -> loc_2_12A4
        call   sub_2_0F7B
loc_2_08A3:
        mov    eax, dword ptr [0x212e0]      ; -> obj2:0x12E0
        mov    word ptr [ebp + 0x1c], ax
        ret    
        db  000h, 000h, 000h
loc_2_08B0:
        cmp    cl, 2
        jae    loc_2_08C8
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        movzx  eax, byte ptr [ebp + 8]
        shl    eax, 2
        jmp    dword ptr [eax + 0x212e4]     ; -> obj2:0x12E4
loc_2_08C8:
        mov    dword ptr [ebp - 4], 0xffffff8f
        ret    
sub_2_08D0:
        pushfd 
        pushal 
        mov    esi, dword ptr [ebp + 0xc]
        or     esi, esi
        jne    loc_2_08DE
        mov    esi, 0x212ec                  ; -> loc_2_12EC
loc_2_08DE:
        VMMCall  VMM.Out_Debug_String            ; cd20c2000100
        popal  
        popfd  
        int    1
        ret    
loc_2_08E9:
        pushfd 
        pushal 
        mov    esi, dword ptr [ebp + 0xc]
        VMMCall  VMM.Out_Debug_String            ; cd20c2000100
        popal  
        popfd  
        ret    
loc_2_08F7:
        push   0
        push   0
        mov    esi, dword ptr [ebp + 0xc]
        VMMCall  VMM.Queue_Debug_String          ; cd20ca000100
        ret    
        db  000h, 000h, 000h
loc_2_0908:
        movzx  eax, cl
        shl    eax, 2
        jmp    dword ptr [eax + 0x21324]     ; -> obj2:0x1324
loc_2_0914:
        inc    dword ptr [0x21318]           ; -> loc_2_1318
        mov    eax, dword ptr [0x21318]      ; -> loc_2_1318
        mov    dword ptr [ebp - 4], eax
        ret    
        db  090h
loc_2_0924:
        mov    ecx, dword ptr [ebp + 0xc]
        dec    dword ptr [0x21318]           ; -> loc_2_1318
        ret    
        db  090h, 090h
loc_2_0930:
        cmp    dword ptr [0x21310], 0        ; -> obj2:0x1310
        jg     loc_2_0949
        pushfd 
        pop    eax
        test   eax, 0x200
        jne    loc_2_0949
        or     dword ptr [0x20fd8], 0x20     ; -> loc_2_0FD8
loc_2_0949:
        cli    
        inc    dword ptr [0x21310]           ; -> obj2:0x1310
        ret    
        db  090h, 090h, 090h
loc_2_0954:
        dec    dword ptr [0x21310]           ; -> obj2:0x1310
        jne    loc_2_0967
        btr    dword ptr [0x20fd8], 5        ; -> loc_2_0FD8
        jb     loc_2_0967
        sti    
loc_2_0967:
        ret    
loc_2_0968:
        bsf    eax, dword ptr [0x21320]      ; -> obj2:0x1320
        je     loc_2_097B
        btr    dword ptr [0x21320], eax      ; -> obj2:0x1320
        mov    byte ptr [ebp - 4], al
loc_2_097B:
        ret    
loc_2_097C:
        mov    eax, dword ptr [ebp + 0xc]
        ret    
        db  090h, 090h
loc_2_0982:
        mov    ecx, dword ptr [ebp + 0xc]
        bts    dword ptr [0x2131c], ecx      ; -> loc_2_131C
        setb   byte ptr [ebp - 4]
        ret    
        db  090h
loc_2_0992:
        mov    ecx, dword ptr [ebp + 0xc]
        btr    dword ptr [0x2131c], ecx      ; -> loc_2_131C
        setb   byte ptr [ebp - 4]
        ret    
loc_2_09A1:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Get_VMM_Reenter_Count       ; cd2006000100
        mov    ecx, dword ptr [ebp + 0xc]
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
loc_2_09B6:
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.Release_Time_Slice          ; cd2038000100
        jecxz  loc_2_09CD
        bt     dword ptr [0x2131c], ecx      ; -> loc_2_131C
        jb     loc_2_09B6
loc_2_09CD:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        ret    
loc_2_09D4:
        VMMCall  VMM.Get_VMM_Reenter_Count       ; cd2006000100
        bt     dword ptr [0x20fd8], 2        ; -> loc_2_0FD8
        jae    loc_2_09E5
        inc    ecx
loc_2_09E5:
        mov    dword ptr [ebp - 4], ecx
        ret    
        db  090h
loc_2_09EA:
        cmp    cl, 1
        je     loc_2_09D4
        mov    esi, 0x2134c                  ; -> loc_2_134C
        mov    edx, dword ptr [ebp + 0xc]
        pushfd 
        cli    
        cmp    dword ptr [0x21348], 0        ; -> loc_2_1348
        je     loc_2_0A27
        mov    ecx, 0xa
loc_2_0A07:
        cmp    dword ptr [esi], edx
        je     loc_2_0A49
        add    esi, 4
        loop   loc_2_0A07
        mov    ecx, 0xa
        mov    esi, 0x2134c                  ; -> loc_2_134C
loc_2_0A1A:
        cmp    dword ptr [esi], 0
        je     loc_2_0A27
        add    esi, 4
        loop   loc_2_0A1A
        popfd  
        jmp    loc_2_0A4B
loc_2_0A27:
        inc    dword ptr [0x21348]           ; -> loc_2_1348
        mov    dword ptr [esi], edx
        mov    esi, 0x20a55                  ; -> loc_2_0A55
        popfd  
        cmp    dword ptr [ebp + 0x10], 1
        je     loc_2_0A42
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        ret    
loc_2_0A42:
        VMMCall  VMM.Call_Global_Event           ; cd2010000100
        ret    
loc_2_0A49:
        popfd  
        ret    
loc_2_0A4B:
        mov    dword ptr [ebp - 4], 0xffffff8e
        ret    
        db  090h, 090h
loc_2_0A55:
        mov    ecx, 0xa
        mov    esi, 0x2134c                  ; -> loc_2_134C
loc_2_0A5F:
        cmp    dword ptr [esi], edx
        je     loc_2_0A68
        add    esi, 4
        loop   loc_2_0A5F
loc_2_0A68:
        mov    dword ptr [esi], 0
        dec    dword ptr [0x21348]           ; -> loc_2_1348
        push   ebp
        mov    ebp, esp
        xor    ebx, ebx
        call   edx
        pop    ebp
        ret    
        db  000h, 000h, 000h
sub_2_0A80:
        mov    edi, 0x2137c                  ; -> loc_2_137C
        mov    esi, ebp
        add    esi, 8
        mov    ecx, 4
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x2138c], ecx      ; -> loc_2_138C
        push   ebp
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        call   sub_2_0C2F
        jb     loc_2_0B21
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    eax, dword ptr [0x21378]      ; -> loc_2_1378
        mov    edx, eax
        shr    edx, 4
        mov    word ptr [ebp + 0x3c], dx
        shl    edx, 4
        sub    eax, edx
        mov    word ptr [ebp + 0x14], ax
        mov    eax, dword ptr [0x21380]      ; -> loc_2_1380
        mov    word ptr [ebp + 0x10], ax
        mov    ecx, dword ptr [0x2137c]      ; -> loc_2_137C
        movzx  eax, cl
        shl    eax, 2
        call   sub_2_13AC
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        and    dword ptr [0x20fd8], 0xffffefff ; -> loc_2_0FD8
        pop    ebp
        mov    eax, dword ptr [0x2138c]      ; -> loc_2_138C
        test   byte ptr [0x213a7], 0xff      ; -> obj2:0x13A7
        je     loc_2_0B20
        mov    dword ptr [ebp - 4], eax
loc_2_0B20:
        ret    
loc_2_0B21:
        pop    ebp
        mov    eax, 0xffffff38
        sub    eax, edi
        test   byte ptr [0x213a7], 0xff      ; -> obj2:0x13A7
        je     loc_2_0B20
        mov    dword ptr [ebp - 4], eax
        ret    
loc_2_0B36:
        mov    word ptr [ebp + 0x1c], 0x3d02
loc_2_0B3C:
        mov    esi, dword ptr [0x21380]      ; -> loc_2_1380
        mov    edi, dword ptr [0x21378]      ; -> loc_2_1378
        mov    ecx, 0x200
        rep movsb byte ptr es:[edi], byte ptr [esi]
        call   sub_2_0C0E
        mov    esi, dword ptr [0x21384]      ; -> loc_2_1384
        mov    dword ptr [esi], eax
        ret    
loc_2_0B5D:
        mov    word ptr [ebp + 0x18], 0
        mov    word ptr [ebp + 0x1c], 0x3c00
        jmp    loc_2_0B3C
loc_2_0B6B:
        mov    ecx, dword ptr [0x21384]      ; -> loc_2_1384
        mov    word ptr [ebp + 0x18], cx
        mov    word ptr [ebp + 0x1c], 0x3f00
        call   sub_2_0C0E
        mov    dword ptr [0x2138c], eax      ; -> loc_2_138C
        mov    esi, dword ptr [0x21378]      ; -> loc_2_1378
        mov    edi, dword ptr [0x21388]      ; -> loc_2_1388
        mov    ecx, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        ret    
        db  0C7h, 005h
        dd  loc_2_138C               ; fixup
        db  08Dh, 0FFh, 0FFh, 0FFh, 0C3h
loc_2_0BA1:
        mov    ecx, dword ptr [0x21384]      ; -> loc_2_1384
        mov    word ptr [ebp + 0x14], cx
        shr    ecx, 0x10
        mov    word ptr [ebp + 0x18], cx
        mov    eax, dword ptr [0x21388]      ; -> loc_2_1388
        mov    ah, 0x42
        mov    word ptr [ebp + 0x1c], ax
        call   sub_2_0C0E
        mov    ax, word ptr [ebp + 0x14]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x1c]
        mov    dword ptr [0x2138c], eax      ; -> loc_2_138C
        ret    
loc_2_0BD3:
        mov    ecx, dword ptr [0x21384]      ; -> loc_2_1384
        mov    edi, dword ptr [0x21378]      ; -> loc_2_1378
        mov    esi, dword ptr [0x21388]      ; -> loc_2_1388
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [0x21384]      ; -> loc_2_1384
        mov    word ptr [ebp + 0x18], cx
        mov    word ptr [ebp + 0x1c], 0x4000
        call   sub_2_0C0E
        mov    dword ptr [0x2138c], eax      ; -> loc_2_138C
        ret    
loc_2_0C02:
        mov    word ptr [ebp + 0x1c], 0x3e00
        call   sub_2_0C0E
        ret    
sub_2_0C0E:
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x1c]
        test   word ptr [ebp + 0x2c], 1
        jne    loc_2_0C26
        ret    
loc_2_0C26:
        neg    eax
        mov    dword ptr [0x2138c], eax      ; -> loc_2_138C
        pop    eax
        ret    
sub_2_0C2F:
        mov    edi, 9
        bts    dword ptr [0x20fd8], 0xc      ; -> loc_2_0FD8
        jb     loc_2_0C60
        dec    edi
        bt     dword ptr [0x20fd8], 0        ; -> loc_2_0FD8
        jb     loc_2_0C5E
        VxDCall  DOSMGR.DOSMGR_Get_DOS_Crit_Status ; cd2008001500
        jne    loc_2_0C60
        dec    edi
        VxDCall  DOSMGR.DOSMGR_Get_IndosPtr      ; cd2004001500
        cmp    word ptr [eax], 0
        jne    loc_2_0C60
loc_2_0C5E:
        clc    
        ret    
loc_2_0C60:
        and    dword ptr [0x20fd8], 0xffffefff ; -> loc_2_0FD8
        stc    
        ret    
loc_2_0C6C:
        mov    ebx, dword ptr [esp + 4]
        VxDCall  VNETBIOS.VNETBIOS_Submit        ; cd2002001400
        cld    
        ret    
loc_2_0C78:
        mov    eax, 4
        jmp    loc_2_0C81
loc_2_0C7F:
        xor    eax, eax
loc_2_0C81:
        mov    ecx, dword ptr [eax + 0x213c8] ; -> loc_2_13C8
        jecxz  loc_2_0C91
        cld    
        push   ebx
        call   ecx
        add    esp, 4
        ret    
loc_2_0C91:
        mov    al, 0x23
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 0x31], al
        ret    
loc_2_0C9A:
        mov    edx, ebx
        mov    ecx, dword ptr [esp + 4]
        mov    ebx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        push   ebp
        push   edx
        push   esi
        push   edi
        call   ecx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_2_0CB3:
        test   dword ptr [0x20fd8], 0x200    ; -> loc_2_0FD8
        je     loc_2_0D08
        xor    ebx, ebx
        mov    edx, 0x20c7f                  ; -> loc_2_0C7F
        mov    esi, 0x213c8                  ; -> loc_2_13C8
        cmp    dword ptr [esi], ebx
        je     loc_2_0CDB
        add    esi, 4
        cmp    dword ptr [esi], ebx
        jne    loc_2_0D09
        mov    edx, 0x20c78                  ; -> loc_2_0C78
loc_2_0CDB:
        mov    dword ptr [esi], ecx
        movzx  eax, al
        bts    dword ptr [0x213d0], eax      ; -> obj2:0x13D0
        jae    loc_2_0D08
        push   esi
        cmp    word ptr [0x210b2], 0x400     ; -> loc_2_10B2
        jb     loc_2_0CFF
        mov    ecx, edi
        VxDCall  VNETBIOS.VNETBIOS_Register2     ; cd2005001400
        jmp    loc_2_0D05
loc_2_0CFF:
        VxDCall  VNETBIOS.VNETBIOS_Register      ; cd2001001400
loc_2_0D05:
        pop    esi
        jb     loc_2_0D11
loc_2_0D08:
        ret    
loc_2_0D09:
        mov    dword ptr [ebp - 4], 0xffffff98
        ret    
loc_2_0D11:
        mov    dword ptr [esi], 0
        mov    dword ptr [ebp - 4], 0xffffff8d
        ret    
sub_2_0D1F:
        test   dword ptr [0x20fd8], 0x200    ; -> loc_2_0FD8
        je     loc_2_0D3B
        mov    edi, 0x213c8                  ; -> loc_2_13C8
        cmp    esi, dword ptr [edi]
        je     loc_2_0D3C
        add    edi, 4
        cmp    esi, dword ptr [edi]
        je     loc_2_0D3C
loc_2_0D3B:
        ret    
loc_2_0D3C:
        mov    dword ptr [edi], 0
        cmp    word ptr [0x210b2], 0x400     ; -> loc_2_10B2
        jb     loc_2_0D54
        VxDCall  VNETBIOS.VNETBIOS_Deregister    ; cd2004001400
        ret    
loc_2_0D54:
        movzx  eax, al
        btr    dword ptr [0x213d0], eax      ; -> obj2:0x13D0
        ret    
loc_2_0D5F:
        cmp    cl, 2
        ja     loc_2_0D92
        test   dword ptr [0x20fd8], 0x200    ; -> loc_2_0FD8
        je     loc_2_0D8A
        VxDCall  VNETBIOS.VNETBIOS_Enum          ; cd2003001400
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], edx
        or     eax, edx
        and    eax, dword ptr [0x213d0]      ; -> obj2:0x13D0
        mov    dword ptr [ecx], eax
        ret    
loc_2_0D8A:
        mov    dword ptr [ebp - 4], 0xfffffffe
        ret    
loc_2_0D92:
        mov    dword ptr [ebp - 4], 0xffffff8b
        ret    
        db  000h, 000h
loc_2_0D9C:
        mov    eax, 0xffffff8b
        cmp    word ptr [0x210b2], 0x400     ; -> loc_2_10B2
        jb     loc_2_0DF5
        movzx  edx, cl
        shl    edx, 2
        jmp    loc_2_13D4
loc_2_0DB8:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        jmp    loc_2_0DF5
loc_2_0DCC:
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        jmp    loc_2_0DF5
loc_2_0DE9:
        push   dword ptr [ebp + 0xc]
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_2_0DF5:
        mov    dword ptr [ebp - 4], eax
        ret    
        db  000h, 000h, 000h
loc_2_0DFC:
        mov    ecx, 1
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        push   eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    edx, eax
        pop    eax
        cmp    edx, -1
        je     loc_2_0EE4
        dec    dword ptr [0x213e0]           ; -> loc_2_13E0
        cmp    dword ptr [edx], 1
        jne    loc_2_0EE4
        dec    dword ptr [0x213e0]           ; -> loc_2_13E0
        mov    eax, 0x210b0                  ; -> obj2:0x10B0
        mov    edi, dword ptr [eax + 8]
        mov    ecx, dword ptr [eax + 0xc]
        lea    esi, [edx + 4]
loc_2_0E41:
        push   ecx
        push   esi
        push   edi
        mov    ecx, 8
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    edi
        pop    esi
        pop    ecx
        je     loc_2_0E5A
        add    edi, 0x24
        loop   loc_2_0E41
        jmp    loc_2_0EE4
loc_2_0E5A:
        dec    dword ptr [0x213e0]           ; -> loc_2_13E0
        test   dword ptr [edi + 0x18], 0x200000
        je     loc_2_0EE4
        dec    dword ptr [0x213e0]           ; -> loc_2_13E0
        movzx  ecx, word ptr [ebp + 0x38]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x10]
        mov    dword ptr [0x213e4], ecx      ; -> loc_2_13E4
        mov    ecx, dword ptr [edx + 0xc]
        mov    dword ptr [0x213ec], ecx      ; -> loc_2_13EC
        dec    dword ptr [0x213e0]           ; -> loc_2_13E0
        mov    esi, 0x213e4                  ; -> loc_2_13E4
        call   sub_2_0EFE
        jb     loc_2_0EE4
        dec    dword ptr [0x213e0]           ; -> loc_2_13E0
        lea    esi, [edx + 0x28]
        call   sub_2_0EFE
        jb     loc_2_0EDA
        dec    dword ptr [0x213e0]           ; -> loc_2_13E0
        lea    esi, [edx + 0x3c]
        call   sub_2_0EFE
        jb     loc_2_0ED2
        dec    dword ptr [0x213e0]           ; -> loc_2_13E0
        pushal 
        push   edx
        call   dword ptr [edi + 0x14]
        add    esp, 4
        popal  
        lea    esi, [edx + 0x3c]
        call   sub_2_0F7B
loc_2_0ED2:
        lea    esi, [edx + 0x28]
        call   sub_2_0F7B
loc_2_0EDA:
        mov    esi, 0x213e4                  ; -> loc_2_13E4
        call   sub_2_0F7B
loc_2_0EE4:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        mov    eax, dword ptr [0x213e0]      ; -> loc_2_13E0
        mov    word ptr [ebp + 0x1c], ax
        mov    dword ptr [0x213e0], 8        ; -> loc_2_13E0
        ret    
sub_2_0EFE:
        pushal 
        xor    eax, eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0xc], eax
        mov    ecx, dword ptr [esi]
        jecxz  loc_2_0F75
        cmp    dword ptr [esi + 8], eax
        je     loc_2_0F75
        test   dword ptr [ebx], 0x20
        jne    loc_2_0F28
        movzx  edx, cx
        shr    ecx, 0x10
        shl    ecx, 4
        add    ecx, edx
        add    ecx, dword ptr [ebx + 4]
        jmp    loc_2_0F43
loc_2_0F28:
        movzx  edi, cx
        shr    ecx, 0x10
        push   0
        push   ecx
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_0F78
        lea    ecx, [edi + eax]
loc_2_0F43:
        mov    dword ptr [esi + 4], ecx
        mov    edi, ecx
        and    edi, 0xfff
        mov    ecx, dword ptr [esi + 8]
        add    ecx, edi
        dec    ecx
        shr    ecx, 0xc
        inc    ecx
        mov    dword ptr [esi + 0x10], ecx
        mov    edi, dword ptr [esi + 4]
        shr    edi, 0xc
        mov    dword ptr [esi + 0xc], edi
        push   0
        push   ecx
        push   edi
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_2_0F78
loc_2_0F75:
        popal  
        clc    
        ret    
loc_2_0F78:
        popal  
        stc    
        ret    
sub_2_0F7B:
        pushal 
        mov    edi, dword ptr [esi + 0xc]
        test   edi, edi
        je     loc_2_0F93
        mov    ecx, dword ptr [esi + 0x10]
        push   0
        push   ecx
        push   edi
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_2_0F93:
        popal  
        ret    
        db  000h, 000h, 000h
loc_2_0F98:
        dec    ebx
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    ebp, dword ptr [edx + 0xa0333]
        add    byte ptr [ebp + eax*2 + 0x43], al
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        and    bl, bh
        pop    ds
        add    byte ptr [eax + eax], dl      ; -> loc_2_0000
        add    al, byte ptr [eax]
        arpl   word ptr [eax], ax
        add    al, byte ptr [eax]
        arpl   word ptr [eax], ax
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cwde   
        lar    eax, word ptr [eax]
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FE0:
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0FE8:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        loopne loc_2_0FFD
        add    al, byte ptr [eax]
loc_2_0FF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FFC:
        aaa    
loc_2_0FFD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
sub_2_1088:
        add    eax, 0x5c000203               ; -> loc_2_035C
        add    eax, dword ptr [edx]
        add    byte ptr [eax - 0x4ffffdfc], dh ; -> loc_2_04B0
        or     byte ptr [edx], al
        add    byte ptr [edx + eax + 0x9080002], ch ; -> loc_2_02AC
        add    al, byte ptr [eax]
        or     byte ptr [edx], 2
        add    byte ptr [eax - 0x15fffe00], dh ; -> loc_2_00B0
        or     dword ptr [edx], eax
        add    byte ptr [ebp + ecx + 0x10190002], bl ; -> loc_2_0D9C
loc_2_10B2:
        add    byte ptr [eax], al
loc_2_10B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1110:
        lock and eax, dword ptr [eax]
        add    byte ptr [eax + 0x41], dl
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [esi + 0x65], dl
        jb     0x1195
        imul   ebp, dword ptr [edi + 0x6e], 0x656b5320
        ja     loc_2_112B
loc_2_112B:
        push   esp
        push   0x41502065
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [esi + 0x78], dl
        inc    esp
        daa    
        jae    0x115f
        outsd  dx, dword ptr fs:[esi]
        outsb  dx, byte ptr [esi]
        daa    
        je     loc_2_1165
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [eax + 0x61], ch
        jbe    loc_2_11B2
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx + 0x61], dh
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[esi + 0x65], dh
        jb     0x11ce
        imul   ebp, dword ptr [edi + 0x6e], 0x6d756e20
        bound  esp, qword ptr [ebp + 0x72]
loc_2_1165:
        and    byte ptr cs:[eax], ah
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_2_11D6
        outsb  dx, byte ptr [esi]
        jne    0x11d5
        and    byte ptr fs:[edi + 0x70], ch
        jb     loc_2_11D8
        je     0x11e2
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        bound  ebp, qword ptr [ebp + 0x6d]
        popal  
        je     loc_2_11F2
        arpl   word ptr [esi], bp
        and    byte ptr [edx + 0x65], dl
        jae    0x1204
        popal  
        jb     0x1207
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     loc_2_120E
        and    byte ptr [esi + 0x6f], cl
        ja     0x11ce
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_11A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_11A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_11B2:
        add    byte ptr [eax], al
loc_2_11B4:
        inc    esp
        inc    ebp
        inc    ebx
        dec    ecx
        dec    edi
        inc    ebx
        inc    edx
        xor    edi, dword ptr cs:[eax]
        and    byte ptr ss:[ecx + 0x73], ch
        and    byte ptr [esi + 0x6f], ch
        je     0x11e7
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        or     eax, 0x7475420a
        and    byte ptr [ecx + 0x20], ah
loc_2_11D6:
        jo     loc_2_124A
loc_2_11D8:
        outsd  dx, dword ptr [esi]
        je     loc_2_124A
        arpl   word ptr [edi + 0x6c], bp
        and    byte ptr [esi + 0x65], ch
        jae    0x1205
        imul   esi, dword ptr [esi + ebp + 0xd], 0x206f530a
        jo     0x1261
        outsd  dx, dword ptr [esi]
        je     0x1261
loc_2_11F2:
        arpl   word ptr [edi + 0x6c], bp
        and    byte ptr [edi + 0x69], dh
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + 0x65], ah
        and    byte ptr [ecx + ebp*2 + 0x73], ah
        popal  
        bound  ebp, qword ptr [ebp + 0x64]
        or     eax, 0xa
loc_2_120C:
        add    byte ptr [eax], al
loc_2_120E:
        add    byte ptr [eax], al
loc_2_1210:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1214:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1218:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_121C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1220:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1224:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1228:
        add    byte ptr [eax + 0x41], dl
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [ebp + 0x65], cl
        jae    loc_2_12AA
        popal  
        add    byte ptr gs:[si + 0x65], al
        bound  esi, qword ptr [ebp + 0x67]
        and    byte ptr [edx + 0x72], al
        popal  
        imul   esi, dword ptr [eax + 0x6f], 0x69
        outsb  dx, byte ptr [esi]
loc_2_124A:
        je     loc_2_126C
        ja     0x12b7
        je     loc_2_12B8
        and    byte ptr [esi + 0x6f], cl
        outsb  dx, byte ptr [esi]
        sub    eax, 0x75626544
        and    byte ptr [bp + di + 0x4f], al
        push   edx
        inc    ebp
        or     eax, 0x656c500a
        popal  
        jae    loc_2_12CC
        and    byte ptr [esi + 0x6f], cl
        je     0x12d5
loc_2_126C:
        jns    0x128f
        inc    ebx
        jne    loc_2_12E5
        je     0x12e3
        insd   dword ptr es:[edi], dx
        jb     loc_2_1298
        push   ebx
        jne    0x12eb
        jo     loc_2_12EC
        jb     loc_2_12F3
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr fs:[edx + 0x65], dl
        sub    eax, 0x746f6f42
        add    byte ptr cs:[eax], al
        add    byte ptr [eax], al
loc_2_1290:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_2_1294:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1298:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12A8:
        add    byte ptr [eax], al
loc_2_12AA:
        add    byte ptr [eax], al
loc_2_12AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
        dd  loc_2_08E9               ; fixup
loc_2_12E5:
        or     byte ptr [edx], al
        add    bh, dh                        ; -> loc_2_08F7
        or     byte ptr [edx], al
        db  000h
loc_2_12EC:
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        cmp    ah, byte ptr [eax]
        inc    esp
loc_2_12F3:
        bound  esi, qword ptr gs:[ebp + 0x67]
        and    byte ptr [edx + 0x72], al
        popal  
        imul   esi, dword ptr [eax + 0x6f], 0x69
        outsb  dx, byte ptr [esi]
        je     loc_2_1323
        push   edx
        popal  
        arpl   word ptr [eax + 0x65], bp
        and    byte ptr cs:[eax], ah
        or     eax, 0xa
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1318:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_131C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FEh, 0FFh, 0FFh
loc_2_1323:
        push   dword ptr [eax]               ; -> loc_2_0930
        or     dword ptr [edx], eax
        add    byte ptr [ecx + ecx + 2], dl  ; -> loc_2_0954
        add    byte ptr [eax + 9], ch        ; -> loc_2_0968
        add    al, byte ptr [eax]
        or     byte ptr [ecx], 2
        add    byte ptr [edx - 0x5efffdf7], dl ; -> loc_2_0992
        or     dword ptr [edx], eax
        add    byte ptr [ecx + ecx + 2], bh  ; -> loc_2_097C
        add    byte ptr [ecx + ecx], dl      ; -> loc_2_0914
        add    al, byte ptr [eax]
        and    al, 9
        add    al, byte ptr [eax]
loc_2_1348:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_134C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1378:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_137C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1380:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1384:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1388:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_138C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1390:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1394:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1398:
        push   eax
        push   edi
        inc    edx
        dec    edi
        dec    edi
        push   esp
        dec    esp
        dec    edi
        inc    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
loc_2_13A8:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
sub_2_13AC:
        or     eax, dword ptr ss:[edx]
        add    byte ptr [ebx + 0xb], ch      ; -> loc_2_0B6B
        add    al, byte ptr [eax]
        mov    eax, dword ptr [0xd300020b]   ; -> loc_2_0BD3
        or     eax, dword ptr [edx]
        add    byte ptr [edx], al            ; -> loc_2_0C02
        or     al, 2
        add    byte ptr [ebp + 0xb], bl      ; -> loc_2_0B5D
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_13C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_2_13D4:
        mov    eax, 0xcc00020d               ; -> loc_2_0DCC
        or     eax, 0xde90002                ; -> loc_2_0DE9
        add    al, byte ptr [eax]
loc_2_13E0:
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_13E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_13EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xce  R-X
        inc    esp
        inc    ebp
        inc    ebx
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        xor    edi, dword ptr cs:[eax]
        and    byte ptr ss:[eax], ah
        push   esi
        xor    dword ptr [esi], ebp
        xor    byte ptr [esi], ch
        xor    dh, byte ptr [0xa0d2020]
        and    al, 0x43
        outsd  dx, dword ptr [esi]
        jo     0x96
        jb     loc_3_0088
        push   0x63282074
        sub    dword ptr [eax], esp
        inc    esp
        imul   esp, dword ptr [edi + 0x69], 0x206c6174
        inc    ebp
        jno    loc_3_00A7
        imul   esi, dword ptr [eax + 0x6d], 0x20746e65
        inc    ebx
        outsd  dx, dword ptr [esi]
        jb     0xad
        outsd  dx, dword ptr [esi]
        jb     0xa1
        je     0xab
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr cs:[ecx], dh
        cmp    dword ptr [ecx], edi
        xor    al, 0x2c
        and    byte ptr [ecx], dh
        cmp    dword ptr [ecx], edi
        xor    eax, 0x6c41202e
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + 0x69], dl
        push   0x52207374
        jae    loc_3_00C6
        jb     0xd9
        and    byte ptr cs:[0x5750240a], cl
        push   ebx
        imul   esp, dword ptr [edi + 0x6e], 0x6e6f
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    eax, 0x4672030a
        test   ebx, 0x40750003
        mov    eax, 0x89660003
loc_3_0088:
        push   cs
        je     loc_3_008B
loc_3_008B:
        xor    cx, cx
        xor    esi, esi
        mov    edi, 0x1eff006a
        je     loc_3_0097
loc_3_0097:
        jecxz  0xb8
        push   edx
        mov    ah, 0xf
        int    0x10
        cmp    al, 3
        je     loc_3_00A7
        mov    eax, 0x10cd0003
loc_3_00A7:
        mov    eax, 0xba0900
        add    ch, cl
        and    dword ptr [eax + 0x19ba0900], edi
        add    ch, cl
        and    dword ptr [edx + 0x33], ebx
        db  0DBh, 033h, 0F6h, 066h, 033h, 0D2h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh, 033h
loc_3_00C6:
        mul    byte ptr [esi + 0x33]
        db  0D2h, 0B8h, 001h, 080h, 0C3h
VxD__LTEXT ENDS
