; msopl -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 1040 instructions recovered by recursive descent

Declare_Virtual_Device msopl, 4, 0, msopl_Control, 0044Dh, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x188  R-X
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0044Dh                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'msopl   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  msopl_Control            ; DDB_Control_Proc
        dd  msopl_PM_API             ; DDB_V86_API_Proc
        dd  msopl_PM_API             ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0058:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_005C:
        or     al, 0
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h
msopl_Control:
        cmp    eax, 8
        je     loc_5_02AD
loc_1_0075:
        cmp    eax, 0xb
        je     loc_5_025D
loc_1_007E:
        cmp    eax, 3
        je     loc_5_02AD
loc_1_0087:
        cmp    eax, 0x1b
        je     loc_7_0000
loc_1_0090:
        cmp    eax, 0x1c
        je     loc_3_0000
loc_1_0099:
        cmp    eax, 0x22
        je     loc_4_03D2
loc_1_00A2:
        clc    
        ret    
sub_1_00A4:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   esi
        pushfd 
        mov    eax, dword ptr [ebp + 0xc]
        shl    eax, 3
        mov    edx, dword ptr [eax]          ; -> loc_1_005C
        mov    eax, dword ptr [eax + 4]      ; -> loc_1_0060
        mov    dword ptr [ebp + 0xc], eax
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_1_00FB
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_00FB
loc_1_00D7:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0xc], 1
        jne    loc_1_00EB
        cmp    dword ptr [edx + edi], ebx
        jne    loc_1_00F1
loc_1_00E7:
        popfd  
        clc    
        jmp    loc_1_00FF
loc_1_00EB:
        cmp    word ptr [edx + edi], bx
        je     loc_1_00E7
loc_1_00F1:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_00FB
        jmp    loc_1_00D7
loc_1_00FB:
        popfd  
        xor    edi, edi
        stc    
loc_1_00FF:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_1_0105:
        push   eax
        push   ebx
        pushfd 
        add    ebx, dword ptr [0]            ; -> obj1:0x0050
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     loc_1_0132
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_0132
loc_1_011D:
        cmp    dword ptr [eax + 4], edi
        jne    loc_1_0128
        mov    esi, eax
        popfd  
        clc    
        jmp    loc_1_0136
loc_1_0128:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_0132
        jmp    loc_1_011D
loc_1_0132:
        popfd  
        xor    esi, esi
        stc    
loc_1_0136:
        pop    ebx
        pop    eax
        ret    
        db  02Eh, 08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h
        dd  loc_1_0058               ; fixup
loc_1_0144:
        pushal 
        pushfd 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_0180
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_1_0180
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_0180
loc_1_0165:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x10], ebx
        jne    loc_1_0171
        call   sub_5_030B
loc_1_0171:
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_0180
        jmp    loc_1_0165
loc_1_0180:
        popfd  
        popal  
        jmp    loc_1_0058
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x59  R-X
        db  001h, 000h, 000h, 000h
        dd  loc_2_0010               ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h, 049h, 000h
loc_2_0010:
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x88
        and    byte ptr [ecx + 0x63], ah
        arpl   word ptr [ebp + 0x73], sp
        jae    loc_2_0041
        inc    esi
        dec    ebp
        and    byte ptr [ebx + 0x79], dh
        outsb  dx, byte ptr [esi]
        je     0x91
        jae    0x95
        jae    0x4e
        jbe    0x9b
        arpl   word ptr [ebp + 0x20], sp
        sub    eax, 0x20746920
        imul   ebp, dword ptr [esi + 0x20], 0x20657375
loc_2_0041:
        bound  edi, qword ptr [ecx + 0x20]
        popal  
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xb1
        jb     0x6c
        popal  
        jo     0xbf
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        db  02Eh, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x4c  R-X
loc_3_0000:
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_0144
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        xor    esi, esi
        xchg   dword ptr [0], esi            ; -> obj1:0x0054
        or     esi, esi
        je     loc_3_004A
        VMMCall  VMM.List_Destroy                ; cd209c000100
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
        push   ebx
loc_3_002F:
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     loc_3_003E
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_3_003E:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_3_002F
        pop    ebx
loc_3_004A:
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x45a  R-X
sub_4_0000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_00A4
loc_4_000E:
        add    esp, 8
        or     edi, edi
        je     loc_4_001B
        cmp    dword ptr [edi + 0x10], 0
        jne    loc_4_0022
loc_4_001B:
        mov    eax, 0
        jmp    loc_4_0027
loc_4_0022:
        mov    eax, 0xffffffff
loc_4_0027:
        pop    edi
        pop    ebp
        ret    
sub_4_002A:
        push   eax
        push   ecx
        push   esi
        push   edi
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_4_0065
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_0065
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 8
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        pop    eax
        mov    edi, dword ptr [esp]
        mov    dword ptr [eax + 4], edi
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        clc    
loc_4_0060:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
loc_4_0065:
        stc    
        jmp    loc_4_0060
sub_4_0068:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd 
        mov    esi, dword ptr [0]            ; -> obj1:0x0050
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     loc_4_00A0
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_00A0
loc_4_0083:
        cmp    dword ptr [eax + 4], edi
        je     loc_4_0092
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_00A0
        jmp    loc_4_0083
loc_4_0092:
        push   eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_4_00A0:
        popfd  
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
sub_4_00A6:
        jmp    loc_4_00B2
loc_4_00A8:
        mov    esi, 0
        jmp    loc_4_016B
loc_4_00B2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        pushal 
        mov    dword ptr [ebp - 0xc], edx
        movzx  ecx, word ptr [edi]
        mov    eax, ecx
        shl    eax, 2
        add    eax, ecx
        shl    eax, 1
        mov    dword ptr [ebp - 4], eax
        push   0x101
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_0165
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_00ED:
        push   ecx
        mov    ecx, 0xa
        lea    esi, [0]                      ; -> loc_4_00A8
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        loop   loc_4_00ED
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_0109:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0xa
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_4_013C
        add    edi, 6
        add    esi, 0xa
        loop   loc_4_0109
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 0x1c], eax
        jmp    loc_4_016B
loc_4_013C:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_0157
        add    edi, 2
loc_4_0149:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_0149
loc_4_0157:
        push   0
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_0165:
        xor    eax, eax
        mov    dword ptr [esp + 0x1c], eax
loc_4_016B:
        popal  
        add    esp, 0xc
        pop    ebp
        ret    
sub_4_0171:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_017B:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_017B
        pop    eax
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret    
sub_4_019A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_4_0273
        movzx  edx, word ptr [ebp + 0xc]
        push   1
        push   edx
        call   sub_1_00A4
loc_4_01C2:
        add    esp, 8
        or     edi, edi
        jne    loc_4_0273
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_0273
        mov    dword ptr [ebp - 4], eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    dword ptr [eax], 0
        push   1
        push   0x1c
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_0273
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0xc], eax
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [edi + 4], ax
        call   sub_5_0361
loc_4_021B:
        mov    word ptr [edi + 8], ax
        mov    esi, 0                        ; -> obj6:0x0020
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0xc]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0231:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0231
        mov    edx, dword ptr [esp]
        call   sub_4_00A6
        pop    edi
        mov    dword ptr [edi + 0x18], eax
        or     eax, eax
        je     loc_4_0273
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_0251:
        call   sub_4_002A
        jb     loc_4_0272
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_0251
        pop    ebx
        mov    dword ptr [esp + 0x1c], 0xffffffff
loc_4_026C:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_0272:
        pop    ebx
loc_4_0273:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     loc_4_027F
        call   sub_4_0289
loc_4_027F:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_026C
sub_4_0289:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_4_02D3
        mov    eax, dword ptr [edi + 0x18]
        or     eax, eax
        je     loc_4_02C7
        mov    esi, 0                        ; -> obj6:0x0020
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_02B2:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_02B2
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x18]
        call   sub_4_0171
        pop    edi
loc_4_02C7:
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_02D3:
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        mov    eax, dword ptr [ebp - 4]
        push   eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        popal  
        add    esp, 4
        pop    ebp
        ret    
sub_4_02F0:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        movsx  edx, word ptr [ebp + 0xc]
        call   sub_4_03E8
        jb     loc_4_0325
        push   eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_019A
        add    esp, 0xc
        or     eax, eax
        je     loc_4_0322
        mov    dword ptr [esp + 0x1c], 0
loc_4_0322:
        popal  
        pop    ebp
        ret    
loc_4_0325:
        mov    dword ptr [esp + 0x1c], 0x24
        jmp    loc_4_0322
sub_4_032F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd 
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     loc_4_0362
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_00A4
loc_4_034F:
        add    esp, 8
        or     edi, edi
        je     loc_4_0362
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_4_0362
        call   sub_5_030B
loc_4_0362:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_4_03AF
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_03AF
loc_4_0378:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0xc], ebx
        je     loc_4_0389
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_03AF
        jmp    loc_4_0378
loc_4_0389:
        popfd  
        pushfd 
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_0392:
        call   sub_4_0068
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_0392
        pop    ebx
        mov    edi, eax
        call   sub_4_0289
        mov    eax, 0xffffffff
loc_4_03AF:
        popfd  
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_4_03B5:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_0000
loc_4_03CC:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_4_03D2:
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_03B5
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
        stc    
        ret    
        db  0CCh, 0CCh
sub_4_03E8:
        push   ebx
        push   ecx
        pushfd 
        cli    
        mov    ebx, 4
        mov    al, 0x60
        call   sub_5_00C5
loc_4_03F8:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_5_00C5
loc_4_0404:
        mov    ebx, 2
        mov    al, 0x80
        call   sub_5_00C5
loc_4_0410:
        mov    ebx, 4
        mov    al, 1
        call   sub_5_00C5
loc_4_041C:
        mov    cx, 0x4000
loc_4_0420:
        in     al, dx
        test   al, 0x40
        jne    loc_4_0427
        loop   loc_4_0420
loc_4_0427:
        push   eax
        mov    ebx, 4
        mov    al, 0x60
        call   sub_5_00C5
loc_4_0434:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_5_00C5
loc_4_0440:
        pop    eax
        popfd  
        test   al, 0x40
        je     loc_4_0456
        test   al, 6
        jne    loc_4_0451
        mov    eax, 1
        jmp    loc_4_0453
loc_4_0451:
        xor    eax, eax
loc_4_0453:
        clc    
        jmp    loc_4_0457
loc_4_0456:
        stc    
loc_4_0457:
        pop    ecx
        pop    ebx
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x496  R-X
sub_5_0000:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xd8
        test   eax, eax
        je     loc_5_002F
        cmp    eax, 1
        je     loc_5_0036
        cmp    eax, 2
        je     loc_5_007D
        cmp    eax, 3
        je     loc_5_008C
        cmp    eax, 4
        je     loc_5_007D
        mov    eax, 1
        jmp    loc_5_00B8
loc_5_002F:
        xor    eax, eax
        jmp    loc_5_00B8
loc_5_0036:
        push   0
        lea    ecx, [ebp - 0xd8]
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_5_0490
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_00B8
        cmp    word ptr [ebp - 0x7c], 1
        je     loc_5_005C
        mov    eax, 0x13
        jmp    loc_5_00B8
loc_5_005C:
        mov    ax, word ptr [ebp - 0x7a]
        movzx  ecx, ax
        cmp    ecx, -1
        jne    loc_5_006F
        mov    eax, 0x13
        jmp    loc_5_00B8
loc_5_006F:
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_4_02F0
loc_5_0078:
        add    esp, 8
        jmp    loc_5_00B8
loc_5_007D:
        push   dword ptr [ebp + 0x10]
        call   sub_4_032F
loc_5_0085:
        add    esp, 4
        xor    eax, eax
        jmp    loc_5_00B8
loc_5_008C:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_009F
        cmp    eax, 1
        je     loc_5_009F
        mov    eax, 1
        jmp    loc_5_00B8
loc_5_009F:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_00A7:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
loc_5_00B8:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_00BC:
        push   eax
        in     al, dx
        jmp    loc_5_00C0
loc_5_00C0:
        in     al, dx
        jmp    loc_5_00C3
loc_5_00C3:
        pop    eax
        ret    
sub_5_00C5:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_5_00BC
        pop    eax
        inc    dl
        out    dx, al
        call   sub_5_00BC
        pop    edx
        pop    ebx
        ret    
sub_5_00E2:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 6]
        mov    ebx, 0x105
        mov    al, 1
        call   sub_5_00C5
        mov    ebx, 4
        mov    al, 0x60
        call   sub_5_00C5
        mov    ebx, 0x104
        mov    al, 0x3f
        call   sub_5_00C5
        mov    ebx, 8
        xor    al, al
        call   sub_5_00C5
        mov    ebx, 0xbd
        mov    al, 0xc0
        call   sub_5_00C5
        mov    ecx, 0x15
        mov    ebx, 0x40
        mov    al, 0x3f
loc_5_0132:
        xor    bh, bh
        call   sub_5_00C5
        mov    bh, 1
        call   sub_5_00C5
        inc    bl
        loop   loc_5_0132
        mov    ecx, 8
        mov    ebx, 0xb0
        xor    eax, eax
loc_5_0150:
        xor    bh, bh
        call   sub_5_00C5
        mov    bh, 1
        call   sub_5_00C5
        inc    bl
        loop   loc_5_0150
        mov    ebx, 0x104
        xor    eax, eax
        call   sub_5_00C5
        mov    ebx, 0x105
        call   sub_5_00C5
        call   sub_5_00BC
        call   sub_5_00BC
        call   sub_5_00BC
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret    
loc_5_018C:
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        cmp    eax, ebx
        je     loc_5_01A7
        or     eax, eax
        jne    loc_5_01BA
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        call   sub_5_032E
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_01BA
loc_5_01A7:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_01B3
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_01B3:
        je     loc_5_01B8
        in     al, dx
        jmp    loc_5_01B9
loc_5_01B8:
        out    dx, al
loc_5_01B9:
        ret    
loc_5_01BA:
        mov    edi, esi
        call   sub_5_0226
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_5_01C6:
        push   esi
        push   eax
        push   ecx
        mov    si, 1
        test   word ptr [edi + 4], 1
        je     loc_5_01D9
        mov    si, 3
loc_5_01D9:
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_01DF:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_01F0
        inc    cx
        jmp    loc_5_01DF
loc_5_01F0:
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_01F5:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 1
        test   word ptr [edi + 4], 1
        je     loc_5_0209
        mov    si, 3
loc_5_0209:
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_020F:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_0220
        inc    cx
        jmp    loc_5_020F
loc_5_0220:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0226:
        test   word ptr [edi + 0xa], 1
        jne    loc_5_025C
        call   sub_1_0105
loc_5_0233:
        jb     loc_5_025C
        test   dword ptr [esi], 1
        jne    loc_5_025C
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        or     dword ptr [esi], 1
        mov    eax, 0x30
        xor    esi, esi
        xor    edi, edi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
loc_5_025C:
        ret    
loc_5_025D:
        pushfd 
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_5_0295
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_0295
loc_5_0271:
        popfd  
        pushfd 
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 0x10], ebx
        jne    loc_5_0289
        mov    dword ptr [edi + 0x14], 0xffffffff
        call   sub_5_030B
loc_5_0289:
        pop    eax
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_0295
        jmp    loc_5_0271
loc_5_0295:
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     loc_5_02AA
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_02AA:
        popfd  
        clc    
        ret    
loc_5_02AD:
        pushal 
        pushfd 
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
        mov    eax, 5
        mov    ecx, 8
        cli    
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_5_0303
        mov    dword ptr [edi + ebx], esi
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_5_0307
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_0307
loc_5_02DD:
        mov    edi, dword ptr [eax]
        call   sub_4_002A
loc_5_02E4:
        jb     loc_5_02F0
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_0307
        jmp    loc_5_02DD
loc_5_02F0:
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_5_0303
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_0303:
        popfd  
        stc    
        jmp    loc_5_0309
loc_5_0307:
        popfd  
        clc    
loc_5_0309:
        popal  
        ret    
sub_5_030B:
        push   eax
        push   edx
        push   esi
        call   sub_1_0105
loc_5_0313:
        jb     loc_5_0329
        cmp    dword ptr [edi + 0x10], ebx
        jne    loc_5_0329
        call   sub_5_01C6
        mov    dword ptr [edi + 0x10], 0
        and    dword ptr [esi], 0xfffffffe
loc_5_0329:
        pop    esi
        pop    edx
        pop    eax
        clc    
        ret    
sub_5_032E:
        push   eax
        push   ecx
        push   edx
        cmp    dword ptr [edi + 0x10], 0
        je     loc_5_033F
        cmp    dword ptr [edi + 0x10], ebx
        je     loc_5_035C
        stc    
        jb     loc_5_035D
loc_5_033F:
        mov    dword ptr [edi + 0x10], ebx
        cmp    dword ptr [edi + 0x14], ebx
        je     loc_5_0354
        test   word ptr [edi + 4], 1
        je     loc_5_0354
        call   sub_5_00E2
loc_5_0354:
        mov    dword ptr [edi + 0x14], ebx
        call   sub_5_01F5
loc_5_035C:
        clc    
loc_5_035D:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_5_0361:
        mov    eax, 0x400
        clc    
        ret    
msopl_PM_API:
        movzx  eax, word ptr [ebp + 0x14]
        cmp    ah, 2
        jae    loc_5_0392
        movzx  ecx, ah
        shl    ecx, 3
        lea    ecx, [ecx]                    ; -> obj6:0x0010
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    loc_5_0392
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     loc_5_0392
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_5_0392:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_5_0398:
        call   sub_5_0361
        mov    word ptr [ebp + 0x1c], ax
        clc    
        ret    
loc_5_03A3:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_03D3
        push   1
        push   edx
        call   sub_1_00A4
loc_5_03B3:
        add    esp, 8
        or     edi, edi
        je     loc_5_03D3
        mov    ecx, dword ptr [edi + 0x14]
        mov    word ptr [ebp + 0x1c], 0
        call   sub_5_032E
        jb     loc_5_03CB
        ret    
loc_5_03CB:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_03D9
loc_5_03D3:
        mov    word ptr [ebp + 0x1c], 1
loc_5_03D9:
        stc    
        ret    
loc_5_03DB:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_040E
        push   1
        push   edx
        call   sub_1_00A4
loc_5_03EB:
        add    esp, 8
        or     edi, edi
        je     loc_5_040E
        mov    ecx, dword ptr [edi + 0x10]
        cmp    ecx, ebx
        je     loc_5_0401
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_5_0414
loc_5_0401:
        call   sub_5_030B
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_040E:
        mov    word ptr [ebp + 0x1c], 1
loc_5_0414:
        stc    
        ret    
loc_5_0416:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        jne    loc_5_0487
        mov    ecx, dword ptr [ebp + 0x18]
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_5_0487
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_0487
loc_5_0437:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0xc], ecx
        je     loc_5_0448
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_0487
        jmp    loc_5_0437
loc_5_0448:
        popfd  
        pushfd 
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_0487
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_0487
        cmp    ecx, 0x1c
        ja     loc_5_0487
        mov    ax, word ptr [esi + 4]
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [esi + 6]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_0487:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
sub_5_0490:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x3a  RW-
        dd  loc_5_0398               ; fixup
        dd  loc_5_0416               ; fixup
        dd  loc_5_03A3               ; fixup
        dd  loc_5_03DB               ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj6:0x0000              ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj6:0x0000              ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_5_018C               ; fixup
        db  001h, 000h
        dd  loc_5_018C               ; fixup
        db  002h, 000h
        dd  loc_5_018C               ; fixup
        db  003h, 000h
        dd  loc_5_018C               ; fixup
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x73  R-X
loc_7_0000:
        push   0
        push   4
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        or     eax, eax
        jne    loc_7_0013
loc_7_0011:
        stc    
        ret    
loc_7_0013:
        mov    dword ptr [0], eax            ; -> obj1:0x0050
        mov    eax, 5
        mov    ecx, 4
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_7_0011
        mov    dword ptr [0], esi            ; -> obj1:0x0054
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
        push   ebx
loc_7_003D:
        mov    eax, 5
        mov    ecx, 8
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_7_0070
        mov    dword ptr [edi + ebx], esi
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_7_003D
        pop    ebx
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_0144
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        clc    
        ret    
loc_7_0070:
        pop    ebx
        jmp    loc_7_0011
VxD__LTEXT ENDS
