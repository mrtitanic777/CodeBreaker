; VCD -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 2848 instructions recovered by recursive descent

Declare_Virtual_Device VCD, 3, 10, VCD_Control, 0000Eh, Undefined_Init_Order
;   DDK version 0400h   init_order 030000000h   flags 00000h
;   service table: obj1:0x34c, 13 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x448  R-X
loc_1_0000:
        or     ebx, ebx
        je     loc_1_0013
        test   byte ptr [0x420], 1           ; -> obj1:0x0420
        je     loc_1_0013
        VxDCall  VMM_STRING.Service_0002h        ; cd2002001800
loc_1_0013:
        ret    
VCD_Control:
        cmp    eax, 0
        je     sub_3_0027
        cmp    eax, 1
        je     loc_3_014E
        cmp    eax, 2
        je     loc_3_0282
        cmp    eax, 7
        je     sub_4_05FC
        cmp    eax, 0xc
        je     loc_4_0A21
        cmp    eax, 0xf
        je     loc_4_04AF
        cmp    eax, 0xd
        je     loc_4_0510
        cmp    eax, 0x30
        je     loc_4_0B43
        clc    
        ret    
sub_1_005E:
        pop    esi
        add    esi, 0x17
        cmp    ecx, 4
        jbe    loc_1_0070
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0070:
        mov    edi, dword ptr [esi]
        add    edi, ebx
        or     byte ptr [edi + 6], 0x80
        cmp    ebx, dword ptr [esi + 0x1c]
        jne    loc_1_00BE
        test   byte ptr [esi + 6], 0x80
        jne    loc_1_00A2
loc_1_0083:
        jecxz  loc_1_0087
        out    dx, al
        ret    
loc_1_0087:
        mov    al, dl
        and    al, 7
        cmp    al, 2
        je     loc_1_0091
        in     al, dx
        ret    
loc_1_0091:
        mov    al, 1
        xchg   byte ptr [esi + 0x3b], al
        mov    ah, al
        and    ah, 7
        cmp    ah, 2
        je     loc_1_00A1
        in     al, dx
loc_1_00A1:
        ret    
loc_1_00A2:
        test   word ptr [esi + 6], 0x141
        jne    loc_1_0083
        push   edi
        mov    edi, edx
        and    edi, 7
        shl    edi, 3
        add    edi, dword ptr [esi + 0x3c]
        mov    edi, dword ptr [ecx + edi + 0x18]
        xchg   dword ptr [esp], edi
        ret    
loc_1_00BE:
        push   edi
        mov    edi, edx
        and    edi, 7
        mov    edi, dword ptr cs:[ecx + edi*8 + 0xd0] ; -> loc_1_00D0
        xchg   dword ptr [esp], edi
        ret    
loc_1_00D0:
        or     byte ptr [ecx], al
        add    byte ptr [eax], al
        or     byte ptr [ecx], al
        add    byte ptr [eax], al
        push   ss
        add    dword ptr [eax], eax
        add    byte ptr [esi], dl            ; -> loc_1_0116
        add    dword ptr [eax], eax
        add    byte ptr [ecx + eax], ch      ; -> loc_1_012C
        add    byte ptr [eax], al
        das    
        add    dword ptr [eax], eax
        add    byte ptr [ecx + eax], dh      ; -> loc_1_0134
        add    byte ptr [eax], al
        cmp    byte ptr [ecx], al
        add    byte ptr [eax], al
        inc    eax
        add    dword ptr [eax], eax
        add    byte ptr [ecx + eax], al      ; -> loc_1_0144
        add    byte ptr [ecx + eax], cl      ; -> loc_1_014C
        add    byte ptr [ebp + 0x5a000001], cl ; -> loc_1_018D
        add    dword ptr [eax], eax
        add    byte ptr [ebp - 0x9ffffff], cl ; -> loc_1_018D
        inc    edi
        add    eax, dword ptr [eax + 0x3e35a74]
        mov    byte ptr [edi], al
        ret    
        db  08Ah, 007h, 0C3h
loc_1_0116:
        test   byte ptr [edi + 3], 0x80
        je     loc_1_0121
        lea    edi, [edi + 1]
        jmp    loc_1_0124
loc_1_0121:
        lea    edi, [edi + 2]
loc_1_0124:
        jecxz  loc_1_0129
        mov    byte ptr [edi], al
        ret    
loc_1_0129:
        mov    al, byte ptr [edi]
        ret    
loc_1_012C:
        mov    al, 1
        ret    
loc_1_012F:
        or     al, al
        jne    loc_1_0168
        ret    
loc_1_0134:
        mov    al, byte ptr [edi + 3]
        ret    
loc_1_0138:
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 3], al
        ret    
loc_1_0140:
        mov    al, byte ptr [edi + 4]
        ret    
loc_1_0144:
        test   al, 0x18
        jne    loc_1_0168
        mov    byte ptr [edi + 4], al
        ret    
loc_1_014C:
        test   byte ptr [edi + 5], 1
        jne    loc_1_0168
        or     byte ptr [edi + 5], 1
        mov    al, byte ptr [esi + 0x39]
        ret    
loc_1_015A:
        test   byte ptr [edi + 5], 2
        jne    loc_1_0168
        or     byte ptr [edi + 5], 2
        mov    al, byte ptr [esi + 0x3a]
        ret    
loc_1_0168:
        pushal 
        or     byte ptr [esi + 7], 4
        push   1
        push   1
        push   dword ptr [esi + 0x64]
        VxDCall  CONFIGMG.Service_0064h          ; cd2064003300
        add    esp, 0xc
        popal  
        push   ecx
        call   sub_1_0190
        test   ecx, ecx
        pop    ecx
        je     loc_1_0070
loc_1_018D:
        mov    al, 0xff
        ret    
sub_1_0190:
        call   sub_4_0A85
        cmp    ecx, 3
        jne    loc_1_01DD
        pushal 
        mov    edi, dword ptr [esi]
        bts    word ptr [edi + ebx + 6], 1
        jb     loc_1_01DC
        mov    ecx, 0x27                     ; -> obj5:0x0027
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [ecx + 4], eax
        push   0x27                          ; -> obj5:0x0027
        push   0xc0004
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        xchg   ecx, eax
        mov    eax, 0xc0000030
        xor    edi, edi
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        add    esp, dword ptr [esp]
loc_1_01DC:
        popal  
loc_1_01DD:
        ret    
sub_1_01DE:
        pop    esi
        movzx  edx, dl
        or     edx, edx
        je     loc_1_01EA
        jmp    dword ptr [esi + edx + 0x40]
loc_1_01EA:
        push   esi
        call   dword ptr [esi + edx + 0x40]
        pop    esi
        jb     loc_1_01FA
        test   byte ptr [esi + 7], 4
        je     loc_1_0214
        stc    
        ret    
loc_1_01FA:
        test   byte ptr [esi + 7], 2
        stc    
        je     loc_1_0214
        cmp    dword ptr [esi + 0x1c], 1
        jb     loc_1_0214
        mov    eax, dword ptr [esi + 0x20]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        bt     ecx, 3
loc_1_0214:
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_0218:
        mov    ecx, eax
        mov    edx, dword ptr [esi + 0x18]
        add    edx, 2
        in     al, dx
        mov    byte ptr [esi + 0x3b], al
        test   al, 1
        mov    eax, ecx
        stc    
        jne    loc_1_0257
loc_1_022B:
        test   byte ptr [esi + 6], 0x40
        jne    loc_1_029E
        mov    ebx, dword ptr [esi + 0x1c]
        test   ebx, ebx
        je     loc_1_0265
        inc    ebx
        je     loc_1_0258
        dec    ebx
loc_1_023C:
        test   byte ptr [ebx], 8
        jne    loc_1_0258
        mov    ecx, eax
        mov    edx, dword ptr [esi + 0x18]
        add    dl, 4
        in     al, dx
        test   al, 8
        mov    eax, ecx
        je     loc_1_0258
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_0256:
        clc    
loc_1_0257:
        ret    
loc_1_0258:
        call   sub_1_032C
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        jmp    loc_1_0256
loc_1_0265:
        mov    edx, dword ptr [esi + 0x18]
        add    dl, 1
        in     al, dx
        or     al, al
        stc    
        je     loc_1_0257
        jmp    loc_1_0273
loc_1_0273:
        add    dl, 3
        in     al, dx
        test   al, 8
        stc    
        jne    loc_1_0257
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        bts    word ptr [esi + 6], 5
        jb     loc_1_023C
        mov    dword ptr [esi + 0x1c], ebx
        mov    edx, esi
        mov    esi, 0x89c                    ; -> loc_4_089C
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    esi, edx
        jmp    loc_1_023C
loc_1_029E:
        test   byte ptr [esi + 6], 0x10
        je     loc_1_02BF
        push   esi
        call   dword ptr [0x41c]             ; -> obj1:0x041C
        pop    esi
        mov    eax, dword ptr [esi + 0x20]
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [esi + 0x24], eax
        stc    
loc_1_02BF:
        cmc    
        ret    
loc_1_02C1:
        push   es
        push   ds
        push   eax
        mov    eax, ss
        mov    ds, eax
        mov    es, eax
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [esi + 0x24], eax
        pop    eax
        pop    ds
        pop    es
        retf   
        db  090h
loc_1_02D8:
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   cl, 0x80
        je     loc_1_0322
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        mov    edx, dword ptr [esi + 0x18]
        add    edx, 2
        in     al, dx
        mov    byte ptr [esi + 0x3b], al
        test   al, 1
        je     loc_1_0308
        mov    eax, dword ptr [esi + 0x20]
        jmp    loc_1_0302
loc_1_02FC:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_1_0302:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
loc_1_0308:
        mov    eax, dword ptr [0x3f0]        ; -> obj1:0x03F0
        VMMCall  VMM.Adjust_Execution_Time       ; cd2037000100
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [esi + 0x24], eax
        call   dword ptr [0x3d0]             ; -> obj1:0x03D0
loc_1_0322:
        ret    
loc_1_0323:
        jecxz  loc_1_0326
        ret    
loc_1_0326:
        call   sub_1_0190
        ret    
sub_1_032C:
        push   eax
loc_1_032D:
        mov    edx, dword ptr [esi + 0x18]
        add    dl, 2
        in     al, dx
        test   al, 1
        jne    loc_1_0349
        movzx  eax, al
        shr    eax, 1
        sub    dl, 2
        add    dl, byte ptr [eax + 0x444]    ; -> obj1:0x0444
        in     al, dx
        jmp    loc_1_032D
loc_1_0349:
        pop    eax
        ret    
        db  000h
        dd  VCD_Get_Version          ; fixup
        dd  VCD_Set_Port_Global      ; fixup
        dd  VCD_Get_Focus            ; fixup
        dd  VCD_Virtualize_Port      ; fixup
        dd  VCD_Acquire_Port         ; fixup
        dd  VCD_Free_Port            ; fixup
        dd  VCD_Acquire_Port_Windows_Style ; fixup
        dd  VCD_Free_Port_Windows_Style ; fixup
        dd  VCD_Steal_Port_Windows_Style ; fixup
        dd  VCD_Find_COM_Index       ; fixup
        dd  VCD_Set_Port_Global_Special ; fixup
        dd  VCD_Virtualize_Port_Dynamic ; fixup
        dd  VCD_Unvirtualize_Port_Dynamic ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0000Eh                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VCD     '                  ; DDB_Name
        dd  030000000h                  ; DDB_Init_Order
        dd  VCD_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  VCD_PM_API               ; DDB_PM_API_Proc
        dd  obj1:0x034C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        dd  loc_1_0013               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_03F4:
        movsb  byte ptr es:[edi], byte ptr [esi]
        add    al, 0
        add    byte ptr [ebx], ch            ; -> loc_6_002B
        add    byte ptr [eax], al
        add    byte ptr [edx], bh            ; -> loc_6_007A
        add    byte ptr [eax], al
        nop    
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x11000000], ah ; -> loc_6_00A6
        add    dword ptr [eax], eax
        add    byte ptr [ecx], ah            ; -> loc_6_0121
        add    dword ptr [eax], eax
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
loc_1_0414:
        sub    eax, dword ptr [edx]
        add    byte ptr [eax], al
loc_1_0418:
        cld    
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp - 0xbfffff6], al ; -> sub_4_0A85
        add    al, 0
        add    byte ptr [ebx], bh            ; -> loc_3_053B
        add    eax, 0x8130000                ; -> loc_3_0813
        add    byte ptr [eax], al
loc_1_0438:
        pop    es
        pop    es
        add    byte ptr [eax], al
loc_1_043C:
        jmp    loc_1_0443
        db  000h, 000h
        dd  loc_3_05D0               ; fixup
loc_1_0443:
        add    byte ptr [esi], al
        add    al, byte ptr [eax]
        db  005h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x29b  R-X
loc_2_0000:
        add    al, 0
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        retf   
        db  000h, 075h, 001h, 012h, 002h, 083h, 002h, 000h, 000h
loc_2_0018:
        push   esp
        push   0x65722065
        jno    0x95
        jae    0x97
        and    byte ptr fs:[ebx + 0x4f], al
        dec    ebp
        and    byte ptr [eax + 0x6f], dh
        jb     loc_2_00A2
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [esi + 0x6f], ch
        je     loc_2_0056
        popal  
        jbe    0x9a
        imul   ebp, dword ptr [ecx + 0x62], 0x202c656c
        bound  esp, qword ptr [ebp + 0x63]
        popal  
        jne    0xba
        and    byte ptr gs:[ecx + 0x20], ah
        push   0x77647261
        popal  
        jb     0xb8
        and    byte ptr [ecx + 0x6e], ch
loc_2_0056:
        je     0xbd
        jb     0xcc
        jne    0xcc
        je     0x7e
        arpl   word ptr [edi + 0x6e], bp
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x74], 0x73616820
        and    byte ptr [edx + 0x65], ah
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [ebp + 0x74], ah
        arpl   word ptr gs:[ebp + 0x64], si
        and    byte ptr cs:[eax], ah
        pop    ecx
        outsd  dx, dword ptr [esi]
        jne    0x9f
        insd   dword ptr es:[edi], dx
        jne    0xf5
        je     loc_2_00A4
        arpl   word ptr [eax + 0x61], bp
        outsb  dx, byte ptr [esi]
        and    byte ptr gs:[bx + di + 0x6f], bh
        jne    0x101
        and    byte ptr [eax + 0x61], ch
        jb     loc_2_00F8
        ja     0xf7
        jb     0xfd
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x75], 0x6172
loc_2_00A2:
        je     0x10d
loc_2_00A4:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [edi + ebp*2 + 0x20], dh
        bound  esp, qword ptr [ebp + 0x20]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     loc_2_0123
        and    byte ptr [ebp + 0x73], dh
        and    byte ptr gs:[eax + ebp*2 + 0x69], dh
        jae    0xde
        inc    ebx
        dec    edi
        dec    ebp
        and    byte ptr [eax + 0x6f], dh
        jb     loc_2_013A
        and    byte ptr [esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x146
        and    byte ptr [ecx + 0x70], ah
        jo     0x144
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        jae    loc_2_010F
        add    byte ptr [eax], al
        pop    ecx
        outsd  dx, dword ptr [esi]
        jne    loc_2_0107
        push   0x20657661
        imul   ebp, dword ptr [esi + 0x73], 0x6c6c6174
        and    byte ptr fs:[ecx + 0x20], ah
loc_2_00F8:
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x2073776f
        xor    ebp, dword ptr [esi]
        xor    byte ptr [eax], ah
        arpl   word ptr [edi + 0x6d], bp
loc_2_0107:
        jo     0x16a
        je     0x174
        bound  ebp, qword ptr [ebp + 0x20]
loc_2_010F:
        arpl   word ptr [edi + 0x6d], bp
        insd   dword ptr es:[edi], dx
        jne    loc_2_0183
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        jae    loc_2_013E
        jb     loc_2_018A
        jbe    loc_2_0188
loc_2_0123:
        jb     0x153
        and    byte ptr [eax], ah
        push   esp
        outsd  dx, dword ptr [esi]
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        jo     0x194
        jb     0x19d
        jns    0x153
        jae    0x1aa
        jo     0x1a7
        outsd  dx, dword ptr [esi]
        jb     0x1ae
loc_2_013A:
        and    byte ptr [eax + ebp*2 + 0x69], dh
loc_2_013E:
        jae    0x160
        jb     0x1ac
        jbe    0x1aa
        jb     0x167
        jns    loc_2_01B8
        jne    0x16b
        insd   dword ptr es:[edi], dx
        jne    loc_2_01C1
        je     0x170
        jae    0x1b7
        je     0x174
        and    al, byte ptr [ebx + 0x4f]
        dec    ebp
        dec    ebp
        inc    esp
        push   edx
        push   esi
        xor    esi, dword ptr [eax]
        cmp    eax, 0x45555254
        and    ah, byte ptr [eax]
        imul   ebp, dword ptr [esi + 0x20], 0x20656874
        pop    ebx
        xor    edi, dword ptr [eax]
        inc    ebp
        outsb  dx, byte ptr [esi]
        push   0x6573205d
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [ebx + 0x59], dl
        push   ebx
loc_2_0183:
        push   esp
        inc    ebp
        dec    ebp
        dec    ecx
loc_2_0188:
        dec    esi
        dec    ecx
loc_2_018A:
        add    byte ptr cs:[eax], al
        push   esp
        push   0x76207369
        imul   esi, dword ptr [edx + 0x74], 0x206c6175
        jbe    loc_2_0207
        arpl   word ptr [ebp + 0x20], sp
        imul   esi, dword ptr [ebx + 0x20], 0x69797274
        outsb  dx, byte ptr [esi]
        and    byte ptr [si + 0x6f], dh
        and    byte ptr [edi + 0x65], ah
        je     0x1d2
        popal  
        arpl   word ptr [ebx + 0x65], sp
        jae    0x22b
loc_2_01B8:
        and    byte ptr [edi + ebp*2 + 0x20], dh
        and    eax, 0x68772073
loc_2_01C1:
        imul   esp, dword ptr [ebx + 0x68], 0x20736920
        popal  
        insb   byte ptr es:[edi], dx
        jb     0x231
        popal  
        jns    0x1f0
        imul   ebp, dword ptr [esi + 0x20], 0x20657375
        bound  edi, qword ptr [ecx + 0x20]
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x70], ah
        jo     loc_2_024D
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        or     eax, 0x6f440a0a
        and    byte ptr [ecx + 0x6f], bh
        jne    0x213
        ja     loc_2_0256
        outsb  dx, byte ptr [esi]
        je     loc_2_0218
        je     0x262
        and    byte ptr gs:[esi + 0x69], dh
        jb     0x274
        jne    0x263
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x76], ah
loc_2_0207:
        imul   esp, dword ptr [ebx + 0x65], 0x206f7420
        je     loc_2_0271
        imul   esp, dword ptr [ebp + 0x20], 0x63
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_2_028A
loc_2_0218:
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [eax + 0x6f], dh
        jb     0x29a
        aas    
        and    byte ptr [eax], al
        add    byte ptr [eax + ebp*2 + 0x69], dl
        jae    0x24f
        popal  
        jo     0x2a2
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        daa    
        jae    loc_2_025D
        popal  
        je     0x2b4
        insd   dword ptr es:[edi], dx
        jo     0x2b8
        and    byte ptr [edi + ebp*2 + 0x20], dh
        je     loc_2_026C
        popal  
loc_2_024D:
        arpl   word ptr [ebx + 0x65], sp
        jae    0x2c5
        and    byte ptr [edi + ebp*2 + 0x20], dh
loc_2_0256:
        and    eax, 0x61682073
        jae    loc_2_027D
loc_2_025D:
        bound  esp, qword ptr [ebp + 0x65]
        outsb  dx, byte ptr [esi]
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x6562202c
loc_2_026C:
        arpl   word ptr [ecx + 0x75], sp
        jae    0x2d6
loc_2_0271:
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [eax + 0x6f], dh
        jb     0x2ee
        and    byte ptr [ecx + 0x73], ch
loc_2_027D:
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ebp + 0x73], dh
        and    byte ptr gs:[edx + 0x79], ah
        and    byte ptr [ecx + 0x20], ah
loc_2_028A:
        jbe    0x2f5
        jb     0x302
        jne    0x2f1
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x76], ah
        db  069h, 063h, 065h, 02Eh, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x948  R-X
        db  050h, 052h, 0FCh, 043h, 08Bh, 0C3h, 083h, 0F8h, 009h, 076h, 009h, 0B2h, 00Ah, 0F6h, 0F2h, 004h
        db  030h, 0AAh, 08Ah, 0C4h, 004h, 030h, 0AAh, 04Bh, 05Ah, 058h, 0C3h
sub_3_001B:
        pushal 
        mov    esi, edx
        cld    
loc_3_001F:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        jne    loc_3_001F
        popal  
        ret    
sub_3_0027:
        cmp    dword ptr [0x3d8], 0          ; -> obj1:0x03D8
        jne    loc_3_014A
        mov    ecx, 0x18
        xor    eax, eax
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_3_014C
        mov    dword ptr [0x3d8], esi        ; -> obj1:0x03D8
        call   sub_3_0468
        mov    eax, dword ptr [0x8e8]        ; -> loc_3_08E8
        mov    dword ptr [0x41c], eax        ; -> obj1:0x041C
        VMMCall  VMM.Get_Machine_Info            ; cd20bb000100
        mov    dword ptr [9], ebx            ; -> obj5:0x0009
        test   ebx, 0x40000
        je     loc_3_0077
        or     byte ptr [8], 2               ; -> obj5:0x0008
loc_3_0077:
        xor    edi, edi
        test   ebx, 0xc0000
        je     loc_3_0084
        or     edi, 0xffffffff
loc_3_0084:
        mov    esi, 0x8d8                    ; -> obj3:0x08D8
        push   esi
        push   0
        push   0x8d4                         ; -> obj3:0x08D4
        push   2
        VMMCall  VMM._GetRegistryKey             ; cd206f010100
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_00D6
        mov    dword ptr [0xd], 1            ; -> obj5:0x000D
        push   0xd                           ; -> obj5:0x000D
        push   0x8dc                         ; -> obj3:0x08DC
        push   0xd3                          ; -> obj5:0x00D3
        push   0
        push   0x91c                         ; -> obj3:0x091C
        push   dword ptr [esi]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_00D6
        movzx  edi, byte ptr [0x8dc]         ; -> obj3:0x08DC
loc_3_00D6:
        or     edi, edi
        je     loc_3_0102
        mov    dword ptr [0x410], 0x7b       ; -> obj1:0x0410
        mov    dword ptr [0x414], 0x218      ; -> loc_1_0414
        mov    dword ptr [0x418], 0x2d8      ; -> loc_1_0418
        mov    eax, dword ptr [0x8e4]        ; -> loc_3_08E4
        mov    dword ptr [0x41c], eax        ; -> obj1:0x041C
loc_3_0102:
        mov    eax, 2
        cmp    dword ptr [esi], 0
        je     loc_3_0145
        mov    dword ptr [0xd], 4            ; -> obj5:0x000D
        push   0xd                           ; -> obj5:0x000D
        push   0x8dd                         ; -> loc_3_08DD
        push   0xd3                          ; -> obj5:0x00D3
        push   0
        push   0x8ec                         ; -> loc_3_08EC
        push   dword ptr [esi]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        test   eax, eax
        mov    eax, 2
        jne    loc_3_0145
        mov    eax, dword ptr [0x8dd]        ; -> loc_3_08DD
loc_3_0145:
        mov    dword ptr [0x3f0], eax        ; -> obj1:0x03F0
loc_3_014A:
        clc    
        ret    
loc_3_014C:
        stc    
        ret    
loc_3_014E:
        or     byte ptr [8], 1               ; -> obj5:0x0008
        mov    esi, dword ptr [0x3d8]        ; -> obj1:0x03D8
        test   esi, esi
        je     loc_3_0281
        VMMCall  VMM.List_Get_First              ; cd20a3000100
loc_3_0169:
        je     loc_3_01BE
        inc    dword ptr [0x30]              ; -> obj5:0x0030
        mov    edi, eax
        cmp    word ptr [edi], 0
        je     loc_3_019D
        cmp    dword ptr [edi + 0xc], 0
        jne    loc_3_019D
        pushal 
        movzx  eax, word ptr [edi]
        movzx  ebx, byte ptr [edi + 2]
        mov    ecx, dword ptr [edi + 0x10]
        call   sub_3_02B8
        jae    loc_3_019C
        dec    dword ptr [0x30]              ; -> obj5:0x0030
        mov    word ptr [edi], 0
loc_3_019C:
        popal  
loc_3_019D:
        mov    eax, edi
        mov    ecx, dword ptr [edi + 0x10]
        cmp    ecx, dword ptr [0x34]         ; -> obj5:0x0034
        jb     loc_3_01B0
        mov    dword ptr [0x34], ecx         ; -> obj5:0x0034
loc_3_01B0:
        mov    esi, dword ptr [0x3d8]        ; -> obj1:0x03D8
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_3_0169
loc_3_01BE:
        mov    eax, dword ptr [0x34]         ; -> obj5:0x0034
        cmp    eax, dword ptr [0x30]         ; -> obj5:0x0030
        jae    loc_3_01D5
        mov    eax, dword ptr [0x30]         ; -> obj5:0x0030
        mov    dword ptr [0x34], eax         ; -> obj5:0x0034
loc_3_01D5:
        shl    eax, 2
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        test   eax, eax
        jne    loc_3_01EE
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_3_01EE:
        mov    dword ptr [0x3dc], eax        ; -> obj1:0x03DC
        mov    edi, eax
        mov    esi, dword ptr [0x3d8]        ; -> obj1:0x03D8
loc_3_01FB:
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_3_0227
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        cmp    word ptr [eax], 0
        je     loc_3_021F
        mov    ecx, dword ptr [eax + 0x10]
        mov    ebx, dword ptr [eax + 0xc]
        mov    edx, dword ptr [eax + 0x14]
        mov    dword ptr [ebx + 0x64], edx
        mov    dword ptr [edi + ecx*4 - 4], ebx
loc_3_021F:
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        jmp    loc_3_01FB
loc_3_0227:
        VMMCall  VMM.List_Destroy                ; cd209c000100
        xor    eax, eax
        mov    dword ptr [0x3d8], eax        ; -> obj1:0x03D8
        mov    al, 0x14
        mov    esi, 0xb1a                    ; -> loc_4_0B1A
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        push   0
        push   0x87c                         ; -> obj3:0x087C
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        add    esp, 8
        push   0
        push   0x890                         ; -> obj3:0x0890
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        add    esp, 8
        VxDCall  VMM_STRING.Service_0000h        ; cd2000001800
        or     eax, eax
        je     loc_3_0275
        mov    dword ptr [0x3d0], 0          ; -> obj1:0x03D0
loc_3_0275:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        call   sub_4_05FC
        clc    
loc_3_0281:
        ret    
loc_3_0282:
        or     dword ptr [0x420], 1          ; -> obj1:0x0420
        mov    esi, 0x8a4                    ; -> loc_3_08A4
        VxDCall  DOSMGR.DOSMGR_Instance_Device   ; cd2007001500
        mov    ecx, dword ptr [0x34]         ; -> obj5:0x0034
        jecxz  loc_3_02B6
        mov    edi, dword ptr [0x3dc]        ; -> obj1:0x03DC
loc_3_02A2:
        mov    esi, dword ptr [edi]
        or     esi, esi
        je     loc_3_02B1
        lea    esi, [esi + 0x10]
        VxDCall  DOSMGR.DOSMGR_Instance_Device   ; cd2007001500
loc_3_02B1:
        add    edi, 4
        loop   loc_3_02A2
loc_3_02B6:
        clc    
        ret    
sub_3_02B8:
        pushal 
        mov    eax, 0x90
        add    eax, dword ptr [edi + 4]
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        test   eax, eax
        jne    loc_3_02D7
        popal  
        jmp    loc_3_03CF
loc_3_02D7:
        mov    esi, eax
        add    esi, 0x1c
        mov    dword ptr [edi + 0xc], esi
        push   edi
        push   esi
        mov    edi, eax
        mov    esi, 0                        ; -> loc_4_0000
        mov    ecx, 7
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        mov    edi, 0x1de                    ; -> sub_1_01DE
        sub    edi, esi
        mov    dword ptr [esi - 4], edi
        mov    edi, 0x59                     ; -> obj1:0x0059
        sub    edi, eax
        mov    dword ptr [esi - 0x1b], edi
        pop    edi
        mov    eax, 0x18
        add    eax, dword ptr [edi + 8]
        push   0
        push   eax
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        test   eax, eax
        jne    loc_3_0327
        popal  
        jmp    loc_3_03CF
loc_3_0327:
        mov    dword ptr [esi], eax
        mov    dword ptr [esp + 4], esi
        popal  
        mov    byte ptr [esi + 4], cl
        mov    dword ptr [esi + 0x18], eax
        mov    byte ptr [esi + 5], bl
        push   eax
        call   sub_4_001C
        pop    eax
        push   esi
        lea    esi, [esi - 0x1c]
        push   ecx
        mov    ecx, 7
        mov    edx, eax
loc_3_034A:
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_3_0355
        inc    edx
        loop   loc_3_034A
loc_3_0355:
        pop    ecx
        pop    esi
        jb     loc_3_03CE
        cmp    bl, 0xff
        jne    loc_3_0362
        or     byte ptr [esi + 6], 4
loc_3_0362:
        push   edi
        mov    eax, dword ptr [0x93f]        ; -> loc_3_093F
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [0x943]        ; -> obj3:0x0943
        mov    dword ptr [esi + 0x14], eax
        movzx  edi, byte ptr [0x947]         ; -> obj3:0x0947
        lea    edi, [esi + edi + 0x10]
        dec    ecx
        mov    ebx, ecx
        call   0
        push   ecx
        push   esi
        mov    edi, 0x90f                    ; -> loc_3_090F
        call   0
        mov    edx, 0x901                    ; -> obj3:0x0901
        call   sub_3_001B
        mov    edi, 0x90c                    ; -> obj3:0x090C
        mov    eax, 0xfffffffe
        mov    ecx, 3
        xor    esi, esi
        VMMCall  VMM.Get_Profile_Fixed_Point     ; cd20ad000100
        pop    esi
        pop    ecx
        cmp    eax, 0xfffffc18
        jne    loc_3_03BF
        or     eax, 0xffffffff
loc_3_03BF:
        mov    dword ptr [esi + 0xc], eax
        xor    eax, eax
        mov    dword ptr [esi + 8], eax
        pop    edi
        call   sub_4_0540
        clc    
loc_3_03CE:
        ret    
loc_3_03CF:
        stc    
        ret    
VCD_Virtualize_Port:
        pushal 
        test   byte ptr [0x420], 1           ; -> obj1:0x0420
        jne    loc_3_0460
        mov    esi, dword ptr [0x3d8]        ; -> obj1:0x03D8
        test   esi, esi
        je     loc_3_0460
        mov    ecx, eax
        VMMCall  VMM.List_Get_First              ; cd20a3000100
loc_3_03F1:
        je     loc_3_0460
        cmp    dword ptr [eax + 0x10], ecx
        je     loc_3_0400
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jmp    loc_3_03F1
loc_3_0400:
        mov    esi, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 0x18]
        cmp    dword ptr [esi + 4], 0
        je     loc_3_0460
        cmp    byte ptr [eax + 3], 0
        jne    loc_3_0458
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], edx
        mov    edi, eax
        mov    ecx, dword ptr [esp + 0x1c]
        movzx  ebx, byte ptr [edi + 2]
        movzx  eax, word ptr [edi]
        call   sub_3_02B8
        jae    loc_3_0435
        mov    word ptr [edi], 0
        jb     loc_3_0460
loc_3_0435:
        inc    byte ptr [edi + 3]
        mov    edi, dword ptr [edi + 0xc]
        test   byte ptr [esp + 0x10], 1
        jne    loc_3_0446
        or     byte ptr [edi + 6], 2
loc_3_0446:
        mov    esi, dword ptr [esp + 4]
        mov    dword ptr [edi + 0x3c], esi
        or     byte ptr [edi + 6], 0x80
        mov    dword ptr [esp + 0x1c], edi
        popal  
        clc    
        ret    
loc_3_0458:
        popal  
        mov    eax, 1
        jmp    loc_3_0463
loc_3_0460:
        popal  
        xor    eax, eax
loc_3_0463:
        stc    
        ret    
        db  000h, 000h, 000h
sub_3_0468:
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x92a                    ; -> obj3:0x092A
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        or     eax, eax
        je     loc_3_04DF
        mov    eax, 0x87a                    ; -> obj3:0x087A
        sub    eax, 0x4f4                    ; -> loc_3_04F4
        add    eax, 3
        and    al, 0xfc
        push   eax
        push   0
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        pop    ecx
        or     eax, eax
        je     loc_3_04DF
        shr    ecx, 2
        mov    edi, eax
        mov    esi, 0x4f4                    ; -> loc_3_04F4
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        sub    eax, 0x4f4                    ; -> loc_3_04F4
        add    dword ptr [0x42c], eax        ; -> obj1:0x042C
        add    dword ptr [0x430], eax        ; -> obj1:0x0430
        add    dword ptr [0x8e4], eax        ; -> loc_3_08E4
        add    dword ptr [0x8e8], eax        ; -> loc_3_08E8
        add    dword ptr [0x434], eax        ; -> obj1:0x0434
        add    dword ptr [0x438], eax        ; -> loc_1_0438
        add    dword ptr [0x43c], eax        ; -> loc_1_043C
        add    dword ptr [0x440], eax        ; -> obj1:0x0440
        ret    
loc_3_04DF:
        xor    eax, eax
        mov    dword ptr [0x42c], eax        ; -> obj1:0x042C
        mov    dword ptr [0x8e4], eax        ; -> loc_3_08E4
        mov    dword ptr [0x8e8], eax        ; -> loc_3_08E8
        ret    
        db  02Eh, 08Bh, 0C0h
loc_3_04F4:
        xchg   edi, eax
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        mov    dword ptr [esi + 0x28], edi
        movzx  eax, word ptr [edi + 0x28]
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        movzx  ecx, word ptr [edi + 0x26]
        add    eax, ecx
        mov    dword ptr [esi + 0x2c], eax
        movzx  eax, word ptr [edi + 0x2e]
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        movzx  ecx, word ptr [edi + 0x2c]
        add    eax, ecx
        mov    dword ptr [esi + 0x30], eax
        mov    dword ptr [edi + 0x4e], esi
        ret    
loc_3_053B:
        xchg   edi, eax
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        mov    esi, dword ptr [edi + 0x4e]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        cmp    dword ptr [esi + 0x1c], ebx
        je     loc_3_0565
        call   dword ptr [0x428]             ; -> obj1:0x0428
        or     ecx, ecx
        je     loc_3_0565
        or     byte ptr [edi + 0x22], 4
        jmp    loc_3_057C
loc_3_0565:
        mov    edx, dword ptr [esi + 0x18]
        add    dl, 5
        in     al, dx
        test   al, 0x20
        je     loc_3_057C
        pushfd 
        cli    
        call   sub_3_0707
        mov    byte ptr [edi + 0x4a], 0
        popfd  
loc_3_057C:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        ret    
sub_3_0583:
        mov    edx, dword ptr [esi + 0x18]
        add    dl, 5
        in     al, dx
        and    al, 0x20
        je     loc_3_05A1
        sub    dl, 4
        in     al, dx
        test   al, 2
        jne    loc_3_059C
        or     al, 2
        out    dx, al
        jmp    loc_3_059B
loc_3_059B:
        out    dx, al
loc_3_059C:
        jmp    sub_3_0707
loc_3_05A1:
        ret    
        db  08Bh, 0C0h
loc_3_05A4:
        mov    al, byte ptr [esi + 0x3b]
        jmp    loc_3_05B3
        db  02Eh, 08Bh, 0C0h
loc_3_05AC:
        mov    edx, dword ptr [esi + 0x18]
        add    dl, 2
        in     al, dx
loc_3_05B3:
        mov    edi, dword ptr [esi + 0x28]
        test   al, 1
        jne    loc_3_05C7
        movzx  eax, al
        shl    eax, 1
        call   dword ptr [eax + 0x434]       ; -> obj1:0x0434
        jmp    loc_3_05AC
loc_3_05C7:
        mov    ax, word ptr [edi + 0x24]
        and    word ptr [edi + 0x22], ax
        ret    
loc_3_05D0:
        or     byte ptr [edi + 0x22], 0x80
        mov    edx, dword ptr [esi + 0x18]
        add    dl, 5
        in     al, dx
        test   al, 0xe
        je     loc_3_05E2
        mov    byte ptr [edi + 0x4b], al
loc_3_05E2:
        test   al, 0x10
        je     loc_3_05EA
        or     byte ptr [edi + 0x22], 0x40
loc_3_05EA:
        ret    
loc_3_05EB:
        mov    edx, dword ptr [esi + 0x18]
        add    dl, 0
        in     al, dx
        mov    ah, byte ptr [edi + 0x4b]
        mov    bh, ah
        or     ah, ah
        je     loc_3_0605
        and    ah, byte ptr [edi + 0x3f]
        or     byte ptr [edi + 0x1a], ah
        mov    byte ptr [edi + 0x4b], 0
loc_3_0605:
        or     byte ptr [edi + 0x22], 1
        movzx  ecx, word ptr [edi + 0x32]
        cmp    cx, word ptr [edi + 0x2a]
        jge    loc_3_068D
        test   byte ptr [edi + 0x3e], 0x20
        jne    loc_3_068D
        test   bh, 4
        jne    loc_3_0674
loc_3_061E:
        and    al, byte ptr [edi + 0x40]
        je     loc_3_067F
loc_3_0623:
        test   byte ptr [edi + 0xc], 1
        jne    loc_3_062E
        cmp    al, byte ptr [edi + 0x15]
        je     loc_3_0687
loc_3_062E:
        test   byte ptr [edi + 0xd], 1
        je     loc_3_0646
        cmp    al, byte ptr [edi + 0xf]
        je     loc_3_06D1
        cmp    al, byte ptr [edi + 0xe]
        je     loc_3_06EF
loc_3_0646:
        cmp    al, byte ptr [edi + 0x16]
        je     loc_3_06C8
loc_3_064B:
        test   byte ptr [edi + 0x42], 0x80
        jne    loc_3_0673
        push   esi
        mov    esi, dword ptr [esi + 0x2c]
        movzx  ebx, word ptr [edi + 0x36]
        mov    byte ptr [esi + ebx], al
        pop    esi
        inc    ebx
        cmp    bx, word ptr [edi + 0x2a]
        jae    loc_3_0693
loc_3_0664:
        mov    word ptr [edi + 0x36], bx
        inc    ecx
        mov    word ptr [edi + 0x32], cx
        cmp    cx, word ptr [edi + 0x20]
        jae    loc_3_0697
loc_3_0673:
        ret    
loc_3_0674:
        test   byte ptr [edi + 0xd], 4
        je     loc_3_061E
        mov    al, byte ptr [edi + 0x14]
        jmp    loc_3_0646
loc_3_067F:
        test   byte ptr [edi + 0xd], 8
        jne    loc_3_0673
        jmp    loc_3_0623
loc_3_0687:
        or     byte ptr [edi + 0x3e], 0x20
        jmp    loc_3_0673
loc_3_068D:
        or     byte ptr [edi + 0x1a], 1
        jmp    loc_3_0673
loc_3_0693:
        xor    ebx, ebx
        jmp    loc_3_0664
loc_3_0697:
        mov    cl, byte ptr [edi + 0x42]
        test   cl, 0x13
        jne    loc_3_0673
        mov    ah, byte ptr [edi + 0x43]
        or     ah, ah
        je     loc_3_06B5
        add    dl, 4
        in     al, dx
        not    ah
        and    al, ah
        or     cl, 2
        mov    byte ptr [edi + 0x42], cl
        out    dx, al
loc_3_06B5:
        test   byte ptr [edi + 0xd], 2
        je     loc_3_0673
        or     cl, 1
        mov    byte ptr [edi + 0x42], cl
        call   sub_3_0583
        jmp    loc_3_0673
loc_3_06C8:
        or     byte ptr [edi + 0x22], 2
        jmp    loc_3_064B
loc_3_06D1:
        or     byte ptr [edi + 0x42], 8
        test   byte ptr [edi + 0xc], 0x60
        je     loc_3_0673
        movzx  ecx, word ptr [edi + 0x32]
        cmp    cx, word ptr [edi + 0x10]
        ja     loc_3_0673
        and    byte ptr [edi + 0x42], 0xf7
        or     byte ptr [edi + 0x42], 4
        jmp    loc_3_06FD
loc_3_06EF:
        and    byte ptr [edi + 0x42], 0xf7
        test   byte ptr [edi + 0xc], 0x60
        je     loc_3_06FD
        and    byte ptr [edi + 0x42], 0xf7
loc_3_06FD:
        call   sub_3_0583
        jmp    loc_3_0673
sub_3_0707:
        mov    edx, dword ptr [esi + 0x18]
        add    dl, 5
        in     al, dx
        test   al, 0x20
        je     loc_3_0805
        sub    dl, 5
        mov    ah, byte ptr [edi + 0x42]
        test   ah, 0x60
        jne    loc_3_0809
        test   byte ptr [edi + 0xc], 0x60
        jne    loc_3_0734
        test   ah, 5
        jne    loc_3_07D3
loc_3_0734:
        test   ah, 0x7f
        jne    loc_3_0809
        test   byte ptr [edi + 0x3e], 0x40
        jne    loc_3_07CA
        movzx  ecx, word ptr [edi + 0x38]
        jecxz  loc_3_077B
        test   byte ptr [edi + 0xc], 0x40
        jne    loc_3_0780
        test   byte ptr [edi + 0xc], 0x20
        jne    loc_3_07A0
loc_3_0759:
        push   esi
        mov    esi, dword ptr [esi + 0x30]
        movzx  ebx, word ptr [edi + 0x3a]
        mov    al, byte ptr [esi + ebx]
        pop    esi
        inc    ebx
        cmp    bx, word ptr [edi + 0x30]
        jae    loc_3_07C6
loc_3_076C:
        mov    word ptr [edi + 0x3a], bx
        movzx  ecx, word ptr [edi + 0x38]
        dec    ecx
        mov    word ptr [edi + 0x38], cx
loc_3_0779:
        out    dx, al
        ret    
loc_3_077B:
        jmp    loc_3_0805
loc_3_0780:
        movzx  ecx, word ptr [edi + 0x4c]
        cmp    cx, word ptr [edi + 0x10]
        jne    loc_3_0799
        mov    word ptr [edi + 0x4c], 0
        or     byte ptr [edi + 0x42], 8
        mov    al, byte ptr [edi + 0xf]
        jmp    loc_3_0779
loc_3_0799:
        inc    ecx
        mov    word ptr [edi + 0x4c], cx
        jmp    loc_3_0759
loc_3_07A0:
        movzx  ecx, word ptr [edi + 0x4c]
        or     ecx, ecx
        jne    loc_3_07B7
        mov    word ptr [edi + 0x4c], 1
        or     byte ptr [edi + 0x42], 8
        mov    al, byte ptr [edi + 0xf]
        jmp    loc_3_0779
loc_3_07B7:
        inc    ecx
        cmp    cx, word ptr [edi + 0x10]
        jne    loc_3_07C0
        xor    ecx, ecx
loc_3_07C0:
        mov    word ptr [edi + 0x4c], cx
        jmp    loc_3_0759
loc_3_07C6:
        xor    ebx, ebx
        jmp    loc_3_076C
loc_3_07CA:
        and    byte ptr [edi + 0x3e], 0xbf
        mov    al, byte ptr [edi + 0x41]
        jmp    loc_3_0779
loc_3_07D3:
        test   ah, 1
        je     loc_3_07E3
        and    ah, 0xfe
        or     ah, 0x10
        mov    al, byte ptr [edi + 0xf]
        jmp    loc_3_07E9
loc_3_07E3:
        and    ah, 0xeb
        mov    al, byte ptr [edi + 0xe]
loc_3_07E9:
        mov    byte ptr [edi + 0x42], ah
        jmp    loc_3_0779
        db  0F6h, 0C4h, 004h, 00Fh, 084h, 050h, 0FFh, 0FFh, 0FFh, 080h, 0E4h, 0FBh, 08Ah, 047h, 00Eh, 088h
        db  067h, 042h, 0E9h, 074h, 0FFh, 0FFh, 0FFh
loc_3_0805:
        or     byte ptr [edi + 0x22], 4
loc_3_0809:
        inc    dl
        in     al, dx
        and    al, 0xfd
        jmp    loc_3_0779
loc_3_0813:
        mov    edx, dword ptr [esi + 0x18]
        add    dl, 6
        in     al, dx
        mov    byte ptr [edi + 0x45], al
        mov    ch, al
        movzx  eax, al
        mov    ah, al
        shr    eax, 2
        shr    ah, 1
        shr    eax, 3
        and    ax, 0x138
        or     word ptr [edi + 0x22], ax
        mov    ah, ch
        shr    ah, 2
        and    ax, 0xc00
        or     word ptr [edi + 0x22], ax
        mov    ah, ch
        shr    ah, 3
        and    ax, 0x20
        or     word ptr [edi + 0x22], ax
        mov    ah, ch
        shl    ah, 3
        and    ax, 0x2000
        or     word ptr [edi + 0x22], ax
        mov    al, byte ptr [edi + 0x44]
        or     al, al
        jne    loc_3_0863
loc_3_0862:
        ret    
loc_3_0863:
        and    ch, al
        cmp    ch, al
        je     loc_3_086F
        or     byte ptr [edi + 0x42], 0x20
        jmp    loc_3_0862
loc_3_086F:
        and    byte ptr [edi + 0x42], 0xdf
        call   sub_3_0583
        jmp    loc_3_0862
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 008h, 000h
        db  000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Ch, 004h
        db  000h, 000h, 004h, 000h, 000h, 000h, 000h, 002h, 000h, 000h
loc_3_08A4:
        inc    ecx
        push   ebp
        pop    eax
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 0x4f], al
        dec    ebp
        xor    byte ptr [eax], ah
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
        add    byte ptr [esi + 0x43], dl
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_08DD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_3_08E4:
        movsb  byte ptr es:[edi], byte ptr [esi]
        db  005h, 000h, 000h
loc_3_08E8:
        lodsb  al, byte ptr [esi]
        db  005h, 000h, 000h
loc_3_08EC:
        inc    ebx
        dec    edi
        dec    ebp
        inc    edx
        dec    edi
        dec    edi
        push   ebx
        push   esp
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        add    byte ptr [ecx + 0x55], al
        push   esp
        dec    edi
        inc    ecx
        push   ebx
        push   ebx
        dec    ecx
        inc    edi
        dec    esi
        add    byte ptr [ebx + 0x4f], al
        dec    ebp
loc_3_090F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4f], al
        dec    ebp
        dec    ecx
        push   edx
        push   ecx
        push   ebx
        dec    eax
        inc    ecx
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [ebx + 0x4f], al
        dec    ebp
        dec    ebp
        inc    esp
        push   edx
        push   esi
        xor    esi, dword ptr [eax]
        add    byte ptr [ebp + 0x41], cl
        pop    eax
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        dec    edi
        push   edx
        push   esp
        db  000h
loc_3_093F:
        inc    ebx
        dec    edi
        dec    ebp
        xor    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xd10  R-X
loc_4_0000:
        call   sub_1_005E
        mov    dl, 0x10
        jmp    loc_4_0017
        db  0B2h, 00Ch, 0EBh, 00Ah, 0B2h, 008h, 0EBh, 006h, 0B2h, 004h, 0EBh, 002h, 0B2h, 000h
loc_4_0017:
        call   sub_1_01DE
sub_4_001C:
        mov    edx, dword ptr [esi + 0x18]
        add    edx, 3
        in     al, dx
        mov    byte ptr [esi + 0x37], al
        or     al, 0x80
        jmp    loc_4_002A
loc_4_002A:
        out    dx, al
        add    edx, -3
        jmp    loc_4_0030
loc_4_0030:
        in     al, dx
        mov    byte ptr [esi + 0x34], al
        inc    edx
        jmp    loc_4_0037
loc_4_0037:
        in     al, dx
        mov    byte ptr [esi + 0x35], al
        add    edx, 2
        mov    al, byte ptr [esi + 0x37]
        jmp    loc_4_0043
loc_4_0043:
        out    dx, al
        add    edx, -2
        jmp    loc_4_0049
loc_4_0049:
        in     al, dx
        mov    byte ptr [esi + 0x36], al
        add    edx, 3
        jmp    loc_4_0052
loc_4_0052:
        in     al, dx
        mov    byte ptr [esi + 0x38], al
        inc    edx
        jmp    loc_4_0059
loc_4_0059:
        in     al, dx
        or     al, 0x60
        mov    byte ptr [esi + 0x39], al
        inc    edx
        jmp    loc_4_0062
loc_4_0062:
        in     al, dx
        mov    byte ptr [esi + 0x3a], al
        mov    byte ptr [esi + 0x3b], 1
        ret    
VCD_Virtualize_Port_Dynamic:
        pushal 
        add    eax, 0x90
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        test   eax, eax
        je     loc_4_013E
        mov    esi, eax
        add    esi, 0x1c
        mov    ecx, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    edx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + edx*4 - 4], esi
        mov    byte ptr [esi + 4], dl
        mov    edx, dword ptr [esp + 4]
        mov    byte ptr [esi + 5], dl
        shr    edx, 0x10
        mov    dword ptr [esi + 0x18], edx
        push   esi
        mov    edi, eax
        mov    esi, 0                        ; -> loc_4_0000
        mov    ecx, 7
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        mov    edi, 0x1de                    ; -> sub_1_01DE
        sub    edi, esi
        mov    dword ptr [esi - 4], edi
        mov    edi, 0x59                     ; -> obj1:0x0059
        sub    edi, eax
        mov    dword ptr [esi - 0x1b], edi
        mov    eax, 0x18
        add    eax, dword ptr [esp + 0x18]
        push   0
        push   eax
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        test   eax, eax
        je     loc_4_013E
        mov    dword ptr [esi], eax
        call   sub_4_001C
        mov    edx, dword ptr [esp + 4]
        shr    edx, 0x10
        push   esi
        lea    esi, [esi - 0x1c]
        push   ecx
        mov    ecx, 7
loc_4_0101:
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_4_010C
        inc    edx
        loop   loc_4_0101
loc_4_010C:
        pop    ecx
        pop    esi
        jb     loc_4_013E
        test   byte ptr [esp + 0x10], 1
        jne    loc_4_011B
        or     byte ptr [esi + 6], 2
loc_4_011B:
        mov    dword ptr [esi + 0xc], 0xfffffffe
        xor    eax, eax
        mov    dword ptr [esi + 8], eax
        call   sub_4_0540
        mov    dword ptr [esp + 4], esi
        popal  
        mov    dword ptr [esi + 0x3c], edi
        test   edi, edi
        je     loc_4_013C
        or     byte ptr [esi + 6], 0x80
loc_4_013C:
        clc    
        ret    
loc_4_013E:
        popal  
        stc    
        ret    
VCD_Unvirtualize_Port_Dynamic:
        pushal 
        test   byte ptr [esi + 6], 8
        je     loc_4_0151
        mov    eax, dword ptr [esi + 0x20]
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_0151:
        mov    edx, dword ptr [esi + 0x18]
        mov    ecx, 7
loc_4_0159:
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        inc    edx
        loop   loc_4_0159
        push   0
        push   dword ptr [esi]
        VMMCall  VMM._Deallocate_Device_CB_Area  ; cd2015010100
        add    esp, 8
        sub    esi, 0x1c
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        popal  
        ret    
VCD_Get_Version:
        mov    eax, 0x400
        clc    
        ret    
VCD_Set_Port_Global:
        push   eax
        test   byte ptr [8], 1               ; -> obj5:0x0008
        je     loc_4_01E5
        cmp    eax, dword ptr [0x34]         ; -> obj5:0x0034
        ja     loc_4_01E5
        sub    eax, 1
        jb     loc_4_01E6
        shl    eax, 2
        add    eax, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_4_01E5
        or     edx, edx
        jne    loc_4_01C1
        or     byte ptr [eax + 6], 1
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        jmp    loc_4_01D1
loc_4_01C1:
        and    byte ptr [eax + 6], 0xfe
        push   ebx
        mov    ebx, dword ptr [eax + 0x1c]
        or     ebx, ebx
        je     loc_4_01E3
        inc    ebx
        je     loc_4_01E3
        dec    ebx
loc_4_01D1:
        push   esi
        mov    esi, eax
        push   ebx
        xor    ebx, ebx
        call   sub_4_08AD
        pop    ebx
        call   sub_4_08AD
        pop    esi
loc_4_01E3:
        pop    ebx
        stc    
loc_4_01E5:
        cmc    
loc_4_01E6:
        pop    eax
        ret    
VCD_Set_Port_Global_Special:
        or     byte ptr [0x23], 1            ; -> obj5:0x0023
        VxDCall  VCD.VCD_Set_Port_Global         ; cd2001800e00
VCD_Get_Focus:
        push   eax
        cmp    eax, dword ptr [0x34]         ; -> obj5:0x0034
        ja     loc_4_0218
        sub    eax, 1
        jb     loc_4_0219
        mov    ebx, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    ebx, dword ptr [ebx + eax*4]
        or     ebx, ebx
        je     loc_4_0218
        mov    ebx, dword ptr [ebx + 0x1c]
        inc    ebx
        je     loc_4_0217
        dec    ebx
loc_4_0217:
        stc    
loc_4_0218:
        cmc    
loc_4_0219:
        pop    eax
        ret    
VCD_Acquire_Port:
        pushal 
        test   cl, 2
        je     loc_4_0226
        call   sub_4_02E7
loc_4_0226:
        sub    eax, 1
        jb     loc_4_02BC
        cmp    eax, dword ptr [0x34]         ; -> obj5:0x0034
        jae    loc_4_02C1
        shl    eax, 2
        add    eax, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     loc_4_02C1
        cmp    dword ptr [esi + 0x18], 0
        je     loc_4_02C1
        mov    ecx, dword ptr [esi + 0x1c]
        jecxz  loc_4_02A8
        inc    ecx
        je     loc_4_02BC
        cmp    dword ptr [esi + 0xc], 0
        je     loc_4_02A8
        jl     loc_4_026E
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, dword ptr [esi + 0x24]
        cmp    eax, dword ptr [esi + 0xc]
        ja     loc_4_02A8
loc_4_026E:
        mov    ebx, 0x27                     ; -> obj5:0x0027
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [ebx + 4], eax
        mov    ebx, ecx
        push   0x27                          ; -> obj5:0x0027
        push   0xc0003
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        xchg   ecx, eax
        mov    eax, 0xc0001014
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        add    esp, dword ptr [esp]
        cmp    al, 6
        jne    loc_4_02BC
loc_4_02A8:
        push   ebx
        xor    ebx, ebx
        call   sub_4_08AD
        dec    ebx
        mov    dword ptr [esi + 0x1c], ebx
        pop    ebx
        mov    dword ptr [esp + 0x1c], esi
        popal  
        clc    
        ret    
loc_4_02BC:
        xor    eax, eax
        popal  
        stc    
        ret    
loc_4_02C1:
        test   esp, esp
        popal  
        stc    
        ret    
VCD_Free_Port:
        cmp    dword ptr [eax + 0x1c], -1
        jne    loc_4_02D3
        mov    dword ptr [eax + 0x1c], 0
loc_4_02D3:
        pushal 
        mov    edx, dword ptr [eax + 0x18]
        mov    ecx, 7
loc_4_02DC:
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        inc    edx
        loop   loc_4_02DC
        popal  
        ret    
sub_4_02E7:
        push   ebx
        push   ecx
        mov    edx, dword ptr [0x3dc]        ; -> obj1:0x03DC
        xor    ecx, ecx
loc_4_02F1:
        cmp    ecx, dword ptr [0x34]         ; -> obj5:0x0034
        jae    loc_4_030A
        mov    ebx, dword ptr [edx]
        or     ebx, ebx
        je     loc_4_0304
        cmp    dword ptr [ebx + 0x18], eax
        je     loc_4_030A
loc_4_0304:
        inc    ecx
        add    edx, 4
        jmp    loc_4_02F1
loc_4_030A:
        mov    eax, ecx
        inc    eax
        pop    ecx
        pop    ebx
        ret    
VCD_Find_COM_Index:
        push   edx
        call   sub_4_02E7
        pop    edx
        cmp    dword ptr [0x34], eax         ; -> obj5:0x0034
        ret    
VCD_Acquire_Port_Windows_Style:
        pushal 
        btr    dword ptr [0x23], 1           ; -> obj5:0x0023
        jb     loc_4_0330
        or     byte ptr [0x23], 1            ; -> obj5:0x0023
loc_4_0330:
        test   cl, 2
        je     loc_4_033A
        call   sub_4_02E7
loc_4_033A:
        sub    eax, 1
        jb     loc_4_043C
        cmp    eax, dword ptr [0x34]         ; -> obj5:0x0034
        jae    loc_4_043C
        mov    esi, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    esi, dword ptr [esi + eax*4]
        or     esi, esi
        je     loc_4_043C
        cmp    dword ptr [esi + 0x18], 0
        je     loc_4_043C
        cmp    byte ptr [esi + 5], 0xff
        je     loc_4_043C
        cmp    al, 2
        jae    loc_4_038D
        test   byte ptr [esi + 6], 1
        je     loc_4_038D
        cmp    word ptr [eax*2 + 0x400], 0
        je     loc_4_043C
loc_4_038D:
        mov    ecx, dword ptr [esi + 0x1c]
        jecxz  loc_4_03CA
        inc    ecx
        je     loc_4_043E
        dec    ecx
        test   byte ptr [esp + 0x18], 1
        je     loc_4_03A9
        add    ecx, dword ptr [esi]
        or     byte ptr [ecx + 6], 1
        jmp    loc_4_03CA
loc_4_03A9:
        cmp    dword ptr [esi + 0x1c], ebx
        je     loc_4_03CA
        test   byte ptr [esi + 6], 1
        jne    loc_4_03CA
        cmp    dword ptr [esi + 0xc], 0
        je     loc_4_03CA
        jl     loc_4_0438
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, dword ptr [esi + 0x24]
        cmp    eax, dword ptr [esi + 0xc]
        jb     loc_4_0438
loc_4_03CA:
        cmp    ebx, dword ptr [esi + 0x1c]
        jne    loc_4_03D8
        push   ebx
        xor    ebx, ebx
        call   sub_4_08AD
        pop    ebx
loc_4_03D8:
        mov    edi, ebx
        add    edi, dword ptr [esi]
        mov    byte ptr [edi + 2], 0
        or     byte ptr [edi + 6], 0x40
        test   byte ptr [esp + 0x18], 4
        je     loc_4_03F1
        xor    word ptr [edi + 6], 0x140
loc_4_03F1:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp]
        test   byte ptr [esp + 0x18], 8
        jne    loc_4_041B
        cmp    dword ptr [esi + 0x54], 0
        je     loc_4_0411
        pushal 
        push   0
        push   dword ptr [esi + 0x58]
        call   dword ptr [esi + 0x54]
        pop    ecx
        pop    ecx
        popal  
loc_4_0411:
        mov    dword ptr [esi + 0x28], eax
        add    eax, ecx
        mov    dword ptr [esi + 0x2c], eax
        jmp    loc_4_0421
loc_4_041B:
        mov    dword ptr [esi + 0x54], eax
        mov    dword ptr [esi + 0x58], ecx
loc_4_0421:
        mov    ebx, dword ptr [esp + 0x10]
        call   sub_4_08AD
        mov    dword ptr [esp + 0x1c], esi
        popal  
        and    byte ptr [0x23], 0xfe         ; -> obj5:0x0023
        clc    
        ret    
loc_4_0438:
        xor    eax, eax
        jmp    loc_4_043E
loc_4_043C:
        or     ebx, ebx
loc_4_043E:
        popal  
        pushfd 
        and    byte ptr [0x23], 0xfe         ; -> obj5:0x0023
        popfd  
        stc    
        ret    
sub_4_044A:
        push   edi
        push   ecx
        mov    edi, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    ecx, dword ptr [0x34]         ; -> obj5:0x0034
        test   esp, esp
        jecxz  loc_4_045F
        cld    
        repne scasd eax, dword ptr es:[edi]
loc_4_045F:
        pop    ecx
        pop    edi
        ret    
VCD_Free_Port_Windows_Style:
        pushal 
        call   sub_4_044A
        jne    loc_4_048A
        mov    esi, eax
        mov    edi, ebx
        add    edi, dword ptr [esi]
        test   word ptr [edi + 6], 0x140
        je     loc_4_048A
        and    word ptr [edi + 6], 0xfeaf
        cmp    dword ptr [esi + 0x1c], ebx
        jne    loc_4_048A
        xor    ebx, ebx
        call   sub_4_08AD
loc_4_048A:
        popal  
        ret    
VCD_Steal_Port_Windows_Style:
        mov    ecx, esp
        mov    eax, esi
        call   sub_4_044A
        jne    loc_4_049C
        call   sub_4_0A85
loc_4_049C:
        xor    eax, eax
        or     ecx, ecx
        jne    loc_4_04A3
        inc    eax
loc_4_04A3:
        ret    
loc_4_04A4:
        VxDCall  VCD.VCD_Get_Version             ; cd2000000e00
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_4_04AF:
        pushal 
        cmp    edx, 0xe
        jne    loc_4_050D
        cmp    esi, dword ptr [0x34]         ; -> obj5:0x0034
        ja     loc_4_050D
        sub    esi, 1
        jb     loc_4_050D
        shl    esi, 2
        add    esi, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    esi, dword ptr [esi]
        or     esi, esi
        je     loc_4_050D
        mov    edx, ebx
        test   byte ptr [esi + 6], 1
        je     loc_4_0506
        mov    ebx, dword ptr [esi + 0x1c]
        or     ebx, ebx
        je     loc_4_0506
        inc    ebx
        je     loc_4_050D
        dec    ebx
        test   byte ptr [esi + 6], 8
        je     loc_4_0506
        mov    eax, dword ptr [esi + 0x20]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   cl, 0x20
        je     loc_4_0506
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        mov    ebx, edx
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_4_0506:
        mov    ebx, edx
        call   sub_4_08AD
loc_4_050D:
        clc    
        popal  
        ret    
loc_4_0510:
        mov    edi, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    ecx, dword ptr [0x34]         ; -> obj5:0x0034
        jecxz  loc_4_053E
loc_4_051E:
        push   ecx
        mov    esi, dword ptr [edi]
        or     esi, esi
        je     loc_4_0538
        cmp    dword ptr [esi + 0x1c], ebx
        jne    loc_4_0538
        test   byte ptr [esi + 6], 8
        je     loc_4_0538
        mov    eax, dword ptr [esi + 0x20]
        call   sub_4_068D
loc_4_0538:
        add    edi, 4
        pop    ecx
        loop   loc_4_051E
loc_4_053E:
        clc    
        ret    
sub_4_0540:
        push   eax
        mov    eax, dword ptr [0x414]        ; -> loc_1_0414
        mov    dword ptr [esi + 0x40], eax
        mov    dword ptr [esi + 0x44], 0x5a5 ; -> loc_4_05A5
        mov    eax, dword ptr [0x418]        ; -> loc_1_0418
        mov    dword ptr [esi + 0x48], eax
        mov    dword ptr [esi + 0x4c], 0x323 ; -> loc_1_0323
        mov    dword ptr [esi + 0x50], 0x5a5 ; -> loc_4_05A5
        pop    eax
        test   byte ptr [esi + 6], 0x80
        je     loc_4_05A5
        test   word ptr [esi + 6], 0x141
        jne    loc_4_05A5
        pushal 
        mov    ecx, 5
        mov    edi, dword ptr [esi + 0x3c]
        lea    edi, [edi + 4]
        lea    ebx, [esi + 0x40]
loc_4_0584:
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     loc_4_058C
        mov    dword ptr [ebx], eax
loc_4_058C:
        add    edi, 4
        add    ebx, 4
        loop   loc_4_0584
        mov    edi, dword ptr [esi + 0x3c]
        cmp    dword ptr [edi + 0xc], 0
        jne    loc_4_05A4
        mov    dword ptr [esi + 0x48], 0x5a5 ; -> loc_4_05A5
loc_4_05A4:
        popal  
loc_4_05A5:
        ret    
sub_4_05A6:
        test   byte ptr [esi + 6], 0x80
        je     loc_4_05BE
        test   byte ptr [esi + 7], 1
        jne    loc_4_05BE
        pushal 
        mov    eax, 0
        mov    edi, dword ptr [esi + 0x3c]
        call   dword ptr [edi]
        popal  
loc_4_05BE:
        ret    
sub_4_05BF:
        pushal 
        mov    edx, dword ptr [esi + 0x18]
        mov    ecx, 7
loc_4_05C8:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    edx
        loop   loc_4_05C8
        popal  
        ret    
sub_4_05D3:
        pushal 
        mov    edx, dword ptr [esi + 0x18]
        mov    ecx, 7
        mov    ebp, dword ptr [0x410]        ; -> obj1:0x0410
        test   word ptr [esi + 6], 0x140
        je     loc_4_05ED
        or     ebp, 0xffffffff
loc_4_05ED:
        shr    ebp, 1
        jae    loc_4_05F7
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
loc_4_05F7:
        inc    edx
        loop   loc_4_05ED
        popal  
        ret    
sub_4_05FC:
        mov    ecx, dword ptr [0x34]         ; -> obj5:0x0034
        jecxz  loc_4_0618
        mov    esi, dword ptr [0x3dc]        ; -> obj1:0x03DC
loc_4_060A:
        push   esi
        mov    esi, dword ptr [esi]
        call   sub_4_061A
        pop    esi
        add    esi, 4
        loop   loc_4_060A
loc_4_0618:
        clc    
        ret    
sub_4_061A:
        test   esi, esi
        je     loc_4_0665
        call   sub_4_0666
        mov    edi, dword ptr [esi]
        lea    edi, [ebx + edi]
        test   byte ptr [esi + 6], 1
        je     loc_4_065C
        push   ebx
        mov    ebx, dword ptr [esi + 0x1c]
        inc    ebx
        je     loc_4_064F
        dec    ebx
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_4_064F
        push   edi
        lea    edi, [esi + 0x34]
        push   eax
        push   edx
        call   sub_4_0767
        pop    edx
        pop    eax
        pop    edi
        pop    ebx
        jmp    loc_4_065C
loc_4_064F:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        add    ebx, dword ptr [esi]
        lea    esi, [ebx]
        pop    ebx
        jmp    loc_4_065F
loc_4_065C:
        lea    esi, [esi + 0x34]
loc_4_065F:
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [edi], 0
loc_4_0665:
        ret    
sub_4_0666:
        test   byte ptr [esi + 7], 0x10
        je     loc_4_068C
        push   eax
        movzx  eax, byte ptr [esi + 4]
        cmp    al, 4
        ja     loc_4_068B
        mov    edi, dword ptr [ebx + 4]
        lea    edi, [edi + eax*2 + 0x3fe]
        cmp    word ptr [edi], 0
        jne    loc_4_068B
        mov    eax, dword ptr [esi + 0x18]
        mov    word ptr [edi], ax
loc_4_068B:
        pop    eax
loc_4_068C:
        ret    
sub_4_068D:
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   cl, 0x80
        je     loc_4_06A9
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        call   sub_1_032C
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_4_06A9:
        ret    
sub_4_06AA:
        pushal 
        test   byte ptr [esi + 6], 0x80
        je     loc_4_06C0
        test   byte ptr [esi + 6], 1
        jne    loc_4_06C0
        test   word ptr [ecx + ebx + 6], 0x140
        je     loc_4_06F0
loc_4_06C0:
        call   sub_4_05D3
        test   byte ptr [ecx + ebx + 6], 0x40
        je     loc_4_06F0
        test   byte ptr [ecx + ebx + 6], 0x10
        jne    loc_4_06F0
        push   ecx
        mov    ecx, dword ptr [esi + 0x54]
        jecxz  loc_4_06E5
        push   1
        push   dword ptr [esi + 0x58]
        call   ecx
        pop    ecx
        pop    ecx
        pop    ecx
        jmp    loc_4_06F0
loc_4_06E5:
        pop    ecx
        mov    eax, dword ptr [esi + 0x2c]
        or     eax, eax
        je     loc_4_06F0
        and    byte ptr [eax], 0xfe
loc_4_06F0:
        test   byte ptr [esi + 6], 8
        je     loc_4_06FF
        mov    eax, dword ptr [esi + 0x20]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
loc_4_06FF:
        mov    dword ptr [esi + 0x1c], ebx
        add    ecx, ebx
        mov    edx, dword ptr [esi + 0x18]
        add    edx, 3
        mov    al, 0x80
        out    dx, al
        jmp    loc_4_070F
loc_4_070F:
        add    edx, -3
        mov    al, byte ptr [ecx]
        out    dx, al
        jmp    loc_4_0717
loc_4_0717:
        inc    edx
        mov    al, byte ptr [ecx + 1]
        out    dx, al
        jmp    loc_4_071E
loc_4_071E:
        add    edx, 2
        mov    al, byte ptr [ecx + 3]
        out    dx, al
        jmp    loc_4_0727
loc_4_0727:
        inc    edx
        mov    al, byte ptr [ecx + 4]
        out    dx, al
        jmp    loc_4_072E
loc_4_072E:
        movzx  eax, word ptr [ecx + 6]
        and    eax, 0x150
        or     word ptr [esi + 6], ax
        add    edx, -3
        mov    al, byte ptr [ecx + 2]
        out    dx, al
        jmp    loc_4_0744
loc_4_0744:
        call   sub_4_074B
        popal  
        ret    
sub_4_074B:
        test   byte ptr [esi + 6], 8
        je     loc_4_0766
        mov    eax, dword ptr [esi + 0x20]
        cmp    dword ptr [esi + 0x54], 0
        je     loc_4_0760
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009800300
loc_4_0760:
        VxDCall  VPICD.VPICD_Set_Auto_Masking    ; cd200a800300
loc_4_0766:
        ret    
sub_4_0767:
        mov    edx, dword ptr [esi + 0x18]
        add    edx, 3
        in     al, dx
        mov    byte ptr [edi + 3], al
        or     al, 0x80
        out    dx, al
        jmp    loc_4_0776
loc_4_0776:
        add    dl, 0xfd
        in     al, dx
        mov    byte ptr [edi], al
        jmp    loc_4_077E
loc_4_077E:
        inc    edx
        in     al, dx
        jmp    loc_4_0782
loc_4_0782:
        mov    byte ptr [edi + 1], al
        add    dl, 2
        mov    al, byte ptr [edi + 3]
        out    dx, al
        sub    dl, 2
        in     al, dx
        jmp    loc_4_0792
loc_4_0792:
        mov    byte ptr [edi + 2], al
        add    dl, 3
        in     al, dx
        mov    byte ptr [edi + 4], al
        ret    
sub_4_079D:
        cmp    dword ptr [esi + 0x1c], 0
        je     loc_4_07F8
        cmp    dword ptr [esi + 0x1c], -1
        je     loc_4_07F8
        pushal 
        mov    ebx, dword ptr [esi + 0x1c]
        mov    edi, ebx
        add    edi, ecx
        test   byte ptr [esi + 6], 0x40
        je     loc_4_07D7
        test   byte ptr [esi + 6], 0x10
        jne    loc_4_07D7
        mov    ecx, dword ptr [esi + 0x54]
        jecxz  loc_4_07CD
        push   0
        push   dword ptr [esi + 0x58]
        call   ecx
        pop    ecx
        pop    ecx
        jmp    loc_4_07D7
loc_4_07CD:
        mov    eax, dword ptr [esi + 0x2c]
        or     eax, eax
        je     loc_4_07D7
        or     byte ptr [eax], 1
loc_4_07D7:
        call   sub_4_05BF
        call   sub_4_0767
        mov    edx, dword ptr [esi + 0x18]
        inc    edx
        xor    al, al
        out    dx, al
        jmp    loc_4_07EA
loc_4_07EA:
        inc    edx
        out    dx, al
        xor    ebx, ebx
        xchg   dword ptr [esi + 0x1c], ebx
        and    word ptr [esi + 6], 0xfeaf
        popal  
loc_4_07F8:
        ret    
sub_4_07F9:
        test   byte ptr [esi + 6], 4
        jne    loc_4_089B
        push   ecx
        sub    esp, 0x20
        mov    edi, esp
        movzx  eax, byte ptr [esi + 5]
        mov    word ptr [edi], ax
        xor    ecx, ecx
        test   byte ptr [esi + 6], 2
        jne    loc_4_081B
        or     cl, 2
loc_4_081B:
        mov    word ptr [edi + 2], cx
        lea    eax, [esi - 7]
        lea    edi, [edi + 4]
        cld    
        mov    ecx, 5
loc_4_082B:
        stosd  dword ptr es:[edi], eax
        sub    eax, 4
        loop   loc_4_082B
        mov    edi, esp
        mov    dword ptr [edi + 0x18], 0x1f4
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x3c]
        jecxz  loc_4_0853
        cmp    dword ptr [ecx + 8], eax
        jne    loc_4_084B
        mov    dword ptr [edi + 8], eax
loc_4_084B:
        cmp    dword ptr [ecx + 0x14], eax
        jne    loc_4_0853
        mov    dword ptr [edi + 0x14], eax
loc_4_0853:
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_4_0893
        mov    dword ptr [esi + 0x20], eax
        or     byte ptr [esi + 6], 8
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   cl, 8
        je     loc_4_0873
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_4_0873:
        test   byte ptr [esi + 6], 2
        jne    loc_4_0882
        test   cl, 4
        jne    loc_4_0882
        or     byte ptr [esi + 7], 4
loc_4_0882:
        movzx  eax, byte ptr [esi + 5]
        bts    dword ptr [0x424], eax        ; -> obj1:0x0424
        jb     loc_4_0893
        or     byte ptr [esi + 7], 2
loc_4_0893:
        or     byte ptr [esi + 6], 4
        add    esp, 0x20
        pop    ecx
loc_4_089B:
        ret    
loc_4_089C:
        mov    esi, edx
        xor    ebx, ebx
        xchg   dword ptr [esi + 0x1c], ebx
        call   sub_4_08AD
        and    byte ptr [esi + 6], 0xdf
        ret    
sub_4_08AD:
        pushal 
        test   esi, esi
        je     loc_4_099D
        mov    ecx, dword ptr [esi]
        cmp    ebx, dword ptr [esi + 0x1c]
        je     loc_4_098E
        push   dword ptr [esi + 0x1c]
        call   sub_4_079D
        or     ebx, ebx
        jne    loc_4_096F
        mov    edx, dword ptr [esi + 0x18]
        test   byte ptr [esp + 5], 2
        jne    loc_4_08EC
        inc    edx
        xor    al, al
        out    dx, al
        jmp    loc_4_08E1
loc_4_08E1:
        add    dl, 3
        in     al, dx
        jmp    loc_4_08E7
loc_4_08E7:
        and    al, 3
        out    dx, al
        jmp    loc_4_0967
loc_4_08EC:
        add    dl, 4
        in     al, dx
        test   al, 8
        je     loc_4_0967
        dec    ebx
        mov    dword ptr [esi + 0x1c], ebx
        inc    ebx
        test   byte ptr [esi + 6], 8
        je     loc_4_0908
        mov    eax, dword ptr [esi + 0x20]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
loc_4_0908:
        mov    eax, dword ptr [0x414]        ; -> loc_1_0414
        mov    dword ptr [esi + 0x40], eax
        mov    al, 1
        sub    dl, 3
        out    dx, al
        add    dl, 4
        jmp    loc_4_091B
loc_4_091B:
        in     al, dx
        jmp    loc_4_091E
loc_4_091E:
        and    al, 0x60
        cmp    al, 0x60
        jne    loc_4_091B
        dec    edx
        in     al, dx
        jmp    loc_4_0928
loc_4_0928:
        mov    ah, al
        or     al, 0x10
        out    dx, al
        jmp    loc_4_092F
loc_4_092F:
        sub    dl, 4
        xor    al, al
        out    dx, al
        jmp    loc_4_0937
loc_4_0937:
        add    dl, 5
loc_4_093A:
        in     al, dx
        jmp    loc_4_093D
loc_4_093D:
        and    al, 0x60
        cmp    al, 0x60
        jne    loc_4_093A
        dec    edx
        mov    al, ah
        and    al, 3
        out    dx, al
        test   byte ptr [esi + 6], 8
        je     loc_4_0958
        mov    eax, dword ptr [esi + 0x20]
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_4_0958:
        sti    
        sub    dl, 3
        xor    eax, eax
        out    dx, al
        mov    dword ptr [esi + 0x1c], eax
        call   sub_4_074B
loc_4_0967:
        pop    edx
        call   sub_4_05A6
        jmp    loc_4_099D
loc_4_096F:
        btr    dword ptr [0x23], 0           ; -> obj5:0x0023
        jb     loc_4_097E
        call   sub_4_07F9
loc_4_097E:
        call   sub_4_06AA
        call   sub_4_0540
        pop    edx
        call   sub_4_05A6
loc_4_098E:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [esi + 0x24], eax
        call   dword ptr [0x3d0]             ; -> obj1:0x03D0
loc_4_099D:
        popal  
        ret    
loc_4_099F:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 7], 4
        jne    loc_4_09B9
        push   0
        push   8
        push   dword ptr [eax + 0x64]
        VxDCall  CONFIGMG.Service_0064h          ; cd2064003300
        add    esp, 0xc
loc_4_09B9:
        ret    
sub_4_09BA:
        enter  0xc, 0
        push   eax
        push   ebx
        push   ecx
        push   edx
        xor    ebx, ebx
        mov    dword ptr [ebp - 0xc], 4
        lea    ecx, [ebp - 4]
        push   ecx
        push   0x38                          ; -> obj5:0x0038
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    ebx, eax
        jb     loc_4_0A1B
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   ecx
        push   eax
        push   0
        push   0
        push   0x64                          ; -> obj5:0x0064
        push   dword ptr [ebp - 4]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        cmp    ebx, eax
        jb     loc_4_0A0B
        cmp    dword ptr [ebp - 8], 1
loc_4_0A0B:
        pushfd 
        push   dword ptr [ebp - 4]
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        or     eax, eax
        popfd  
loc_4_0A1B:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        leave  
        ret    
loc_4_0A21:
        mov    ecx, dword ptr [0x34]         ; -> obj5:0x0034
        jecxz  loc_4_0A3D
        mov    esi, dword ptr [0x3dc]        ; -> obj1:0x03DC
loc_4_0A2F:
        push   esi
        mov    esi, dword ptr [esi]
        call   sub_4_0A3F
        pop    esi
        add    esi, 4
        loop   loc_4_0A2F
loc_4_0A3D:
        clc    
        ret    
sub_4_0A3F:
        test   esi, esi
        je     loc_4_0A64
        cmp    ebx, dword ptr [esi + 0x1c]
        jne    loc_4_0A64
        push   ebx
        xor    ebx, ebx
        call   sub_4_08AD
        cmp    dword ptr [esi + 0x54], 0
        je     loc_4_0A65
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ecx
        call   sub_4_0A85
        pop    ecx
        pop    ebx
loc_4_0A64:
        ret    
loc_4_0A65:
        pushal 
        call   sub_4_09BA
        jb     loc_4_0A82
        and    byte ptr [esi + 7], 0xfb
        push   0
        push   esi
        push   0x99f                         ; -> loc_4_099F
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e003300
        add    esp, 0xc
loc_4_0A82:
        popal  
        pop    ebx
        ret    
sub_4_0A85:
        pushal 
        cmp    dword ptr [esi + 0x1c], ebx
        je     loc_4_0ABE
        cmp    dword ptr [esi + 0x1c], 0
        je     loc_4_0AB9
        cmp    dword ptr [esi + 0x1c], -1
        je     loc_4_0B13
        test   byte ptr [esi + 6], 1
        jne    loc_4_0B0C
        cmp    dword ptr [esi + 0xc], -2
        je     loc_4_0B0C
        cmp    dword ptr [esi + 0xc], 0
        je     loc_4_0AB9
        jl     loc_4_0AC2
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, dword ptr [esi + 0x24]
        cmp    eax, dword ptr [esi + 0xc]
        jb     loc_4_0AC2
loc_4_0AB9:
        call   sub_4_08AD
loc_4_0ABE:
        popal  
        xor    ecx, ecx
        ret    
loc_4_0AC2:
        mov    eax, dword ptr [esi]
        add    eax, ebx
        bts    word ptr [eax + 6], 0
        jb     loc_4_0B0C
        mov    eax, dword ptr [esi + 0x1c]
        push   esi
        lea    esi, [esi + 0x10]
        VxDCall  SHELL.SHELL_Resolve_Contention  ; cd2001001700
        pop    esi
        jae    loc_4_0AE0
        mov    ebx, eax
loc_4_0AE0:
        cmp    ebx, eax
        je     loc_4_0AF6
        movzx  ecx, byte ptr [esi + 4]
        mov    edx, dword ptr [esi]
        or     byte ptr [eax + edx + 6], 1
        and    byte ptr [ebx + edx + 6], 0xfe
        jmp    loc_4_0AB9
loc_4_0AF6:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [esi + 0x24], eax
        call   dword ptr [0x3d0]             ; -> obj1:0x03D0
        popal  
        mov    ecx, 1
        ret    
loc_4_0B0C:
        popal  
        mov    ecx, 2
        ret    
loc_4_0B13:
        popal  
        mov    ecx, 3
        ret    
loc_4_0B1A:
        movzx  edx, word ptr [ebp + 0x14]
        cmp    edx, dword ptr [0x34]         ; -> obj5:0x0034
        jae    loc_4_0B41
        mov    esi, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    esi, dword ptr [esi + edx*4]
        test   esi, esi
        je     loc_4_0B41
        cmp    ebx, dword ptr [esi + 0x1c]
        jne    loc_4_0B41
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [esi + 0x24], eax
loc_4_0B41:
        stc    
        ret    
loc_4_0B43:
        mov    eax, 0xb4a                    ; -> loc_4_0B4A
        clc    
        ret    
loc_4_0B4A:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 5
        ja     loc_4_0B5A
        jmp    dword ptr [eax*4 + 0x7a]      ; -> obj5:0x007A
loc_4_0B5A:
        ret    
loc_4_0B5B:
        push   esi
        mov    edx, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    ecx, dword ptr [0x34]         ; -> obj5:0x0034
        jecxz  loc_4_0B7A
        mov    eax, dword ptr [esp + 0x14]
loc_4_0B6E:
        mov    esi, dword ptr [edx]
        cmp    eax, dword ptr [esi + 0x18]
        je     loc_4_0B7C
        lea    edx, [edx + 4]
        loop   loc_4_0B6E
loc_4_0B7A:
        xor    esi, esi
loc_4_0B7C:
        mov    eax, esi
        pop    esi
        ret    
sub_4_0B80:
        mov    edi, 0x11                     ; -> obj5:0x0011
        mov    eax, 0xd                      ; -> obj5:0x000D
        mov    dword ptr [eax], 9
        push   0
        push   eax
        push   edi
        push   1
        push   0x1a                          ; -> obj5:0x001A
        push   0
        push   ecx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        test   eax, eax
        jne    loc_4_0BE5
        mov    eax, dword ptr [edi]
        sub    eax, 0x304d4f43
        jb     loc_4_0BE5
        test   eax, 0xffffff
        jne    loc_4_0BE5
        rol    eax, 8
        cmp    al, 9
        ja     loc_4_0BE5
        cmp    al, 1
        jb     loc_4_0BE5
        movzx  ecx, byte ptr [edi + 4]
        test   ecx, ecx
        je     loc_4_0BE5
        cmp    byte ptr [edi + 5], 0
        jne    loc_4_0BE5
        imul   eax, eax, 0xa
        sub    cl, 0x30
        jb     loc_4_0BE5
        cmp    cl, 9
        ja     loc_4_0BE5
        add    eax, ecx
        xor    ecx, ecx
loc_4_0BE5:
        ret    
sub_4_0BE6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 0xc]
        test   esi, esi
        je     loc_4_0C41
        test   byte ptr [esi + 7], 9
        jne    loc_4_0C41
        mov    ecx, dword ptr [esi + 0x54]
        jecxz  loc_4_0C27
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_4_0C41
        push   0
        push   dword ptr [esi + 0x58]
        call   ecx
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_4_0C43
        cmp    dword ptr [esi + 0x54], 0
        je     loc_4_0C27
        mov    eax, dword ptr [esi + 0x54]
        mov    edx, dword ptr [esi + 0x58]
        mov    dword ptr [esi + 0x5c], eax
        mov    dword ptr [esi + 0x60], edx
        or     byte ptr [esi + 7], 8
loc_4_0C27:
        mov    eax, dword ptr [esi + 0x18]
        push   0xa
        pop    ecx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0x14]
        VxDCall  VCD.VCD_Acquire_Port_Windows_Style ; cd2006000e00
        jae    loc_4_0C43
loc_4_0C41:
        xor    eax, eax
loc_4_0C43:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_4_0C48:
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [esp + 0x14]
        call   sub_4_044A
        jne    loc_4_0CAC
        mov    esi, eax
        test   esi, esi
        je     loc_4_0CAC
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [esi + 0x5c], eax
        jne    loc_4_0C91
        cmp    ebx, dword ptr [esi + 0x1c]
        jne    loc_4_0C82
        push   0
        push   dword ptr [esi + 0x58]
        call   dword ptr [esi + 0x54]
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_4_0CB2
        mov    eax, dword ptr [esp + 0x18]
loc_4_0C82:
        xchg   dword ptr [esi + 0x54], eax
        xchg   dword ptr [esi + 0x5c], eax
        mov    eax, dword ptr [esi + 0x58]
        xchg   dword ptr [esi + 0x60], eax
        mov    dword ptr [esi + 0x58], eax
loc_4_0C91:
        cmp    ebx, dword ptr [esi + 0x1c]
        je     loc_4_0CB0
        or     byte ptr [0x23], 1            ; -> obj5:0x0023
        VxDCall  VCD.VCD_Steal_Port_Windows_Style ; cd2008000e00
        and    byte ptr [0x23], 0xfe         ; -> obj5:0x0023
        jmp    loc_4_0CB2
loc_4_0CAC:
        xor    eax, eax
        jmp    loc_4_0CB2
loc_4_0CB0:
        or     al, 1
loc_4_0CB2:
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_4_0CB6:
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        call   sub_4_044A
        jne    loc_4_0D0C
        mov    esi, eax
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [esi + 0x5c], eax
        je     loc_4_0D08
        mov    eax, esi
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VCD.VCD_Free_Port_Windows_Style ; cd2007000e00
        mov    dword ptr [esi + 0x54], 0
        btr    word ptr [esi + 6], 0xb
        jae    loc_4_0D0C
        push   0
        push   dword ptr [esi + 0x60]
        push   dword ptr [esi + 0x5c]
        push   esi
        push   1
        call   sub_4_0BE6
        add    esp, 0x14
        push   1
        push   dword ptr [esi + 0x58]
        call   dword ptr [esi + 0x54]
        pop    ecx
        pop    ecx
loc_4_0D08:
        and    byte ptr [esi + 7], 0xf7
loc_4_0D0C:
        pop    esi
        pop    ebx
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0xe6  RW-
        db  0F8h, 003h, 0F8h, 002h, 0E8h, 003h, 0E8h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 050h, 06Fh, 072h, 074h, 04Eh, 061h
        db  06Dh, 065h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 053h, 079h, 073h, 074h, 065h, 06Dh, 05Ch, 043h
        db  075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h
        db  05Ch, 053h, 065h, 072h, 076h, 069h, 063h, 065h, 073h, 05Ch, 056h, 078h, 044h, 05Ch, 056h, 043h
        db  04Fh, 04Dh, 04Dh, 000h, 045h, 06Eh, 061h, 062h, 06Ch, 065h, 050h, 06Fh, 077h, 065h, 072h, 04Dh
        db  061h, 06Eh, 061h, 067h, 065h, 06Dh, 065h, 06Eh, 074h, 000h
        ; ---- dispatch table (6 entries) ----
        dd  loc_4_0B5B
        dd  sub_4_0BE6
        dd  loc_4_0C48
        dd  loc_4_0CB6
        dd  loc_6_012A
        dd  loc_6_030A
        db  000h, 000h, 053h, 06Fh, 066h, 074h, 077h, 061h, 072h, 065h, 05Ch, 04Dh, 069h, 063h, 072h, 06Fh
        db  073h, 06Fh, 066h, 074h, 05Ch, 057h, 069h, 06Eh, 064h, 06Fh, 077h, 073h, 05Ch, 043h, 075h, 072h
        db  072h, 065h, 06Eh, 074h, 056h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 05Ch, 052h, 075h, 06Eh, 04Fh
        db  06Eh, 063h, 065h, 000h, 045h, 06Eh, 061h, 062h, 06Ch, 065h, 050h, 043h, 04Dh, 043h, 049h, 041h
        db  000h, 003h, 000h, 000h, 000h, 000h, 043h, 04Fh, 04Dh, 056h, 045h, 052h, 049h, 046h, 059h, 042h
        db  041h, 053h, 045h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x384  R-X
VCD_PM_API:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_6_001F
        movzx  eax, word ptr [ebp + 0x14]
        cmp    eax, 7
        jae    loc_6_001F
        and    byte ptr [ebp + 0x2c], 0xfe
        call   sub_1_03F4
        ret    
loc_6_001F:
        mov    dword ptr [ebp + 0x1c], 0xffffffff
loc_6_0026:
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_6_002B:
        xor    eax, eax
        mov    esi, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    ecx, dword ptr [0x34]         ; -> obj5:0x0034
        jecxz  loc_6_0058
        cmp    ecx, 0x10
        jbe    loc_6_0045
        mov    ecx, 0x10
loc_6_0045:
        mov    edx, 1
loc_6_004A:
        cmp    dword ptr [esi], 0
        je     loc_6_0051
        or     eax, edx
loc_6_0051:
        shl    edx, 1
        add    esi, 4
        loop   loc_6_004A
loc_6_0058:
        mov    word ptr [ebp + 0x1c], ax
        ret    
sub_6_005D:
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    ecx, dword ptr [0x34]         ; -> obj5:0x0034
        jae    loc_6_0078
        shl    ecx, 2
        add    ecx, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    ecx, dword ptr [ecx]
        jecxz  loc_6_0078
        clc    
        ret    
loc_6_0078:
        stc    
        ret    
loc_6_007A:
        call   sub_6_005D
        jb     loc_6_0026
        mov    eax, dword ptr [ecx + 0xc]
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x10], ax
        ret    
loc_6_0090:
        call   sub_6_005D
        jb     loc_6_0026
        movzx  eax, word ptr [ebp + 0x10]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 0xc], eax
        ret    
loc_6_00A6:
        xchg   esi, eax
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        movzx  edi, word ptr [ebp]
        movzx  ecx, word ptr [ebp + 0x1c]
        btr    ecx, 1
        jae    loc_6_0026
        movzx  eax, word ptr [ebp + 0x18]
        inc    eax
        and    byte ptr [0x23], 0xfe         ; -> obj5:0x0023
        or     byte ptr [0x23], 2            ; -> obj5:0x0023
        VxDCall  VCD.VCD_Acquire_Port_Windows_Style ; cd2006000e00
        jb     loc_6_00FD
        mov    word ptr [ebp + 0x18], cs
        mov    dword ptr [ebp], 0x2c1        ; -> loc_1_02C1
        mov    dword ptr [ebp + 0x10], eax
        mov    ecx, dword ptr [eax + 0x18]
        movzx  edx, byte ptr [eax + 5]
        mov    word ptr [ebp + 0x1c], cx
        mov    word ptr [ebp + 0x14], dx
        ret    
loc_6_00FD:
        je     loc_6_0108
        and    byte ptr [ebp + 0x2c], 0xbf
        jmp    loc_6_0026
loc_6_0108:
        or     byte ptr [ebp + 0x2c], 0x40
        jmp    loc_6_0026
loc_6_0111:
        call   sub_6_005D
        jb     loc_6_0120
        mov    eax, ecx
        VxDCall  VCD.VCD_Free_Port_Windows_Style ; cd2007000e00
loc_6_0120:
        ret    
loc_6_0121:
        mov    byte ptr [ebp + 0x1c], 0
        jmp    loc_6_0026
loc_6_012A:
        sub    esp, 4
        push   esi
        push   edi
        push   ebx
        lea    esi, [esp + 0xc]
        push   esi
        push   0x94                          ; -> obj5:0x0094
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_6_0190
        mov    dword ptr [0xd], 1            ; -> obj5:0x000D
        push   0xd                           ; -> obj5:0x000D
        push   0xe                           ; -> obj5:0x000E
        push   0xd3                          ; -> obj5:0x00D3
        push   0
        push   0xc6                          ; -> obj5:0x00C6
        push   dword ptr [esi]
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        mov    edi, eax
        push   dword ptr [esi]
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        test   edi, edi
        je     loc_6_01BE
        cmp    edi, 0xea
        je     loc_6_01BE
loc_6_0190:
        mov    eax, 0xd                      ; -> obj5:0x000D
        push   4
        pop    ecx
        mov    dword ptr [eax], ecx
        push   0
        push   eax
        push   esi
        push   3
        push   0xd8                          ; -> obj5:0x00D8
        push   0
        push   dword ptr [esp + 0x30]
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        test   eax, eax
        jne    loc_6_01D0
        cmp    dword ptr [esp + 0xc], eax
        je     loc_6_01D0
loc_6_01BE:
        mov    edx, dword ptr [esp + 0x1c]
        add    dl, 2
        in     al, dx
        jmp    loc_6_01C8
loc_6_01C8:
        test   al, 0x30
        jne    loc_6_0301
loc_6_01D0:
        mov    ecx, dword ptr [esp + 0x18]
        call   sub_4_0B80
        je     loc_6_01F7
        mov    eax, dword ptr [esp + 0x1c]
        push   4
        pop    ecx
        mov    edi, 0                        ; -> obj5:0x0000
        cld    
        repne scasw ax, word ptr es:[edi]
        jne    loc_6_0301
        xor    eax, eax
        mov    al, 4
        sub    eax, ecx
loc_6_01F7:
        mov    dword ptr [esp + 0xc], eax
        test   byte ptr [8], 1               ; -> obj5:0x0008
        je     loc_6_02B9
        mov    ebx, eax
        mov    esi, 0x3dc                    ; -> obj1:0x03DC
        cmp    eax, dword ptr [0x34]         ; -> obj5:0x0034
        jbe    loc_6_0237
        mov    dword ptr [0x34], eax         ; -> obj5:0x0034
        shl    eax, 2
        push   1
        push   eax
        push   dword ptr [esi]
        VMMCall  VMM._HeapReAllocate             ; cd2050000100
        add    esp, 0xc
        test   eax, eax
        je     loc_6_0301
        mov    dword ptr [esi], eax
loc_6_0237:
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + ebx*4 - 4], 0
        jne    loc_6_02FD
        mov    ecx, dword ptr [esp + 0x20]
        mov    esi, dword ptr [esp + 0x1c]
        shl    esi, 0x10
        movzx  si, cl
        mov    edx, dword ptr [esp + 0xc]
        xor    eax, eax
        xor    ecx, ecx
        xor    edi, edi
        xor    ebx, ebx
        test   byte ptr [0xb], 0xc           ; -> obj5:0x000B
        je     loc_6_026B
        or     bl, 1
loc_6_026B:
        VxDCall  VCD.VCD_Virtualize_Port_Dynamic ; cd200b000e00
        jb     loc_6_0301
        push   esi
        lea    edi, [esi + 0x10]
        mov    esi, 0x11                     ; -> obj5:0x0011
        push   8
        pop    ecx
loc_6_0283:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        dec    ecx
        or     al, al
        jne    loc_6_0283
        dec    edi
        inc    ecx
        mov    al, 0x20
        rep stosb byte ptr es:[edi], al
        pop    esi
        mov    ebx, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x64], ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
loc_6_029E:
        push   esi
        or     byte ptr [esi + 7], 0x10
        call   sub_4_061A
        pop    esi
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_6_029E
        jmp    loc_6_02FD
loc_6_02B9:
        call   sub_3_0027
        jb     loc_6_0301
        mov    esi, dword ptr [0x3d8]        ; -> obj1:0x03D8
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_6_0301
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    edi, eax
        mov    ebx, edi
        xor    eax, eax
        mov    ecx, 0x18
        rep stosb byte ptr es:[edi], al
        mov    eax, dword ptr [esp + 0x20]
        mov    byte ptr [ebx + 2], al
        mov    eax, dword ptr [esp + 0x1c]
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ebx + 0x10], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ebx + 0x14], eax
loc_6_02FD:
        or     al, 1
        jmp    loc_6_0303
loc_6_0301:
        xor    eax, eax
loc_6_0303:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_6_030A:
        push   esi
        push   edi
        push   ebx
        test   dword ptr [0x420], 1          ; -> obj1:0x0420
        je     loc_6_037D
        mov    esi, dword ptr [0x3dc]        ; -> obj1:0x03DC
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [0x34]         ; -> obj5:0x0034
        jecxz  loc_6_037D
loc_6_032B:
        mov    edx, dword ptr [esi]
        test   edx, edx
        je     loc_6_0336
        cmp    dword ptr [edx + 0x18], eax
        je     loc_6_033D
loc_6_0336:
        add    esi, 4
        loop   loc_6_032B
        jmp    loc_6_037D
loc_6_033D:
        mov    dword ptr [esi], 0
        mov    esi, edx
        movzx  edx, byte ptr [esi + 4]
        mov    ecx, dword ptr [esi + 0x18]
        VxDCall  VCD.VCD_Unvirtualize_Port_Dynamic ; cd200c000e00
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
loc_6_0358:
        mov    eax, dword ptr [ebx + 4]
        cmp    word ptr [eax + edx*2 + 0x3fe], cx
        jne    loc_6_036F
        mov    word ptr [eax + edx*2 + 0x3fe], 0
loc_6_036F:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_6_0358
loc_6_037D:
        pop    ebx
        pop    edi
        pop    esi
        ret    
        db  021h, 056h, 043h
VxD__LTEXT ENDS
