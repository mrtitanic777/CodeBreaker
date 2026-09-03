; scsi1hlp -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 2227 instructions recovered by recursive descent

Declare_Virtual_Device scsi1hlp, 4, 0, scsi1hlp_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x200  R-X
scsi1hlp_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        clc    
        ret    
        db  000h
loc_1_000C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 3
        je     0xffff024c                    ; -> obj3:0x024C
        cmp    si, 4
        je     loc_3_0467
        cmp    si, 2
        je     loc_3_0489
        mov    word ptr [ebx + 2], 0xffff
        ret    
        db  000h, 000h
loc_1_0040:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, 0x49c                    ; -> loc_3_049C
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
        ret    
loc_1_0052:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [edx + 8]
        mov    ecx, dword ptr [ecx + 8]
loc_1_005D:
        cmp    dword ptr [ecx], 0xc4         ; -> loc_1_00C4
        je     loc_1_006C
        mov    ecx, dword ptr [ecx + 0xc]
        or     ecx, ecx
        jne    loc_1_005D
loc_1_006C:
        mov    eax, dword ptr [edx + 0x14]
        mov    dword ptr [eax + 4], ecx
        movsx  eax, word ptr [ecx + 0x10]
        add    eax, edx
        or     byte ptr [eax + 0xd], 1
        mov    esi, 0x49c                    ; -> loc_3_049C
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
        ret    
        db  000h, 000h, 000h
sub_1_008C:
        ja     0x8f
        add    byte ptr [eax], al
        ja     0x93
        add    byte ptr [eax], al
        ja     0x97
        add    byte ptr [eax], al
        ja     0x9b
        add    byte ptr [eax], al
        ja     0x9f
        add    byte ptr [eax], al
        ja     0xa3
        add    byte ptr [eax], al
        ja     0xa7
        add    byte ptr [eax], al
        ja     0xab
        add    byte ptr [eax], al
        ja     0xaf
        add    byte ptr [eax], al
        ja     0xb3
        add    byte ptr [eax], al
        ja     0xb7
        add    byte ptr [eax], al
        ja     0xbb
        add    byte ptr [eax], al
        ja     0xbf
        add    byte ptr [eax], al
        ja     0xc3
        add    byte ptr [eax], al
loc_1_00C4:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 4]
        cmp    word ptr [ebx + 0x68], 0xf
        je     loc_1_00E0
        test   byte ptr [ebx + 0x6d], 0x80
        jne    loc_1_0168
loc_1_00E0:
        mov    al, 4
        movzx  ecx, word ptr [ebx + 0x68]
        cmp    ecx, 0xe
        jae    loc_1_00F5
        call   sub_1_008C
loc_1_00F5:
        mov    edx, dword ptr [ebx + 0x10]
        or     al, al
        je     loc_1_010D
        dec    al
        jne    loc_1_0130
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        and    byte ptr [eax + 0xd], 0xfe
loc_1_010D:
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax], 0x40         ; -> loc_1_0040
        mov    dword ptr [eax + 4], edx
        add    dword ptr [ebx + 0x14], 8
loc_1_011D:
        mov    edx, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [ebx + 0x10], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
loc_1_012C:
        pop    esi
        pop    edi
        pop    ebp
        ret    
loc_1_0130:
        cmp    al, 3
        je     loc_1_011D
        dec    al
        jne    loc_1_0149
        mov    eax, dword ptr [ebx + 0x14]
        sub    eax, 8
        mov    dword ptr [ebx + 0x14], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        jmp    loc_1_012C
loc_1_0149:
        dec    al
        je     loc_1_012C
        mov    al, 1
        mov    word ptr [ebx + 0x6a], 0x16
        cmp    word ptr [ebx + 0x68], 0xd
        jne    loc_1_0130
        mov    dword ptr [ebx + 0xa0], 0x1f
        jmp    loc_1_0130
loc_1_0168:
        mov    esi, 0x708                    ; -> loc_3_0708
        mov    edx, ebx
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        jmp    loc_1_012C
loc_1_0177:
        mov    al, 4
        ret    
        db  000h, 000h
sub_1_017C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0188:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0190:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'scsi1hlp'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  scsi1hlp_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x5c  R-X
loc_2_0000:
        push   0x24                          ; -> loc_2_0024
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        cmp    word ptr [0x50], 1            ; -> loc_2_0050
        je     loc_2_0023
        cmp    word ptr [0x50], 2            ; -> loc_2_0050
        je     loc_2_0023
        stc    
loc_2_0023:
        ret    
loc_2_0024:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax + 0xc0000], al  ; -> loc_1_000C
        add    byte ptr [eax], al
        jl     0x37
        add    byte ptr [eax], al
        push   ebx
        inc    ebx
        push   ebx
        dec    ecx
        xor    dword ptr [eax], esp
        inc    ebx
        inc    esp
        dec    eax
        inc    ebp
        dec    esp
        push   eax
        and    byte ptr [esi + 0x53], dl
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0050:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x174e  R-X
loc_3_0000:
        dec    esi
        inc    ebp
        inc    ebx
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [edi + ecx*2 + 0x53], dl
        dec    eax
        dec    ecx
        inc    edx
        inc    ecx
        and    byte ptr [ecx], al
        add    byte ptr [ebx + 0x48], al
        dec    ecx
        dec    esi
        dec    edi
        dec    esi
        and    byte ptr [eax], ah
        add    al, byte ptr [eax]
        push   ebx
        dec    edi
        dec    esi
        pop    ecx
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    eax, dword ptr [eax]
loc_3_0028:
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        insb   byte ptr es:[edi], dx
        add    dword ptr [eax], eax
        add    ah, ch                        ; -> obj3:0x01EC
        add    dword ptr [eax], eax
        add    byte ptr [edx], dl            ; -> obj3:0x0212
        add    al, byte ptr [eax]
        db  000h
loc_3_0038:
        adc    dword ptr [eax], eax
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    bh, byte ptr [eax]
        xor    esp, dword ptr [eax]
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [ebx]
        cmp    byte ptr [eax], ah
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    bh, byte ptr [eax]
        xor    al, 0x31
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    bh, byte ptr [eax]
        xor    al, 0x20
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [edx]
        xor    dword ptr [eax], esi
        add    dword ptr [eax], eax
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    ebx
        inc    esp
        push   edx
        sub    eax, 0x50303132
        and    byte ptr [ecx], al
        add    byte ptr [ebx + 0x44], al
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [eax + esi]
        xor    byte ptr [eax], al
        add    byte ptr [ebx + 0x44], al
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [0x3030]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [0x3031]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [0x3131]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [0x3231]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [eax + esi]
        xor    dword ptr [eax], eax
        add    byte ptr [ebx + 0x44], al
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [0x3130]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [esi]
        xor    byte ptr [ecx], dh
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [0x3230]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    dh, byte ptr [esi]
        xor    byte ptr [edx], dh
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        cmp    bh, byte ptr [ecx]
        xor    byte ptr [eax], dh
        add    byte ptr [eax], al
loc_3_016C:
        pop    es
        add    byte ptr [ebx + 0x44], al
        sub    eax, 0x204d4f52
        pop    eax
        dec    ebp
        sub    eax, 0x31303433
        push   esp
        inc    ecx
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        pop    eax
        dec    ebp
        sub    eax, 0x31303533
        push   esp
        inc    ecx
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        pop    eax
        dec    ebp
        sub    eax, 0x31303633
        push   esp
        inc    ecx
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        pop    eax
        dec    ebp
        sub    eax, 0x31303335
        push   esp
        inc    ecx
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        pop    eax
        dec    ebp
        sub    eax, 0x31303134
        inc    ecx
        and    byte ptr [edx], al
        add    byte ptr [ebx + 0x44], al
        sub    eax, 0x204d4f52
        pop    eax
        dec    ebp
        sub    eax, 0x31303134
        push   esp
        inc    ecx
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        pop    eax
        dec    ebp
        sub    eax, 0x31303134
        inc    edx
        and    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [ebx + 0x44], al
        sub    eax, 0x204d4f52
        inc    ebx
        inc    esp
        push   ebx
        sub    eax, 0x20353335
        and    byte ptr [ebx], al
        add    byte ptr [ebx + 0x44], al
        sub    eax, 0x204d4f52
        inc    ebx
        inc    esp
        push   ebx
        sub    eax, 0x20353334
        and    byte ptr [ebx], al
        add    byte ptr [edx], al
        add    byte ptr [ebx + 0x44], al
        sub    eax, 0x204d4f52
        inc    ebx
        inc    esp
        push   ebp
        xor    esi, dword ptr [ebx]
        inc    ecx
        and    byte ptr [eax], ah
        and    byte ptr [eax + eax], al
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    ebx
        inc    esp
        push   ebp
        xor    esi, dword ptr [ecx]
        inc    ecx
        and    byte ptr [eax], ah
        and    byte ptr [eax + eax], al
loc_3_0238:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        test   byte ptr [eax], al
        add    byte ptr [eax], al
        je     0x243
        add    byte ptr [eax], al
        aam    1
        add    byte ptr [eax], al
        pop    esp
        add    dword ptr [eax], eax
        add    byte ptr [ebp + 0x56], dl
        mov    esi, dword ptr [ebx + 0xc]
        test   dword ptr [esi + 0x20], 0x6000
        jne    loc_3_034C
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_034C
        cmp    byte ptr [esi + 0x40], 5
        jne    loc_3_034C
        push   esi
        lea    edi, [0x21d]                  ; -> obj4:0x021D
        mov    ecx, 5
        lea    esi, [esi + 0x8a]
loc_3_0284:
        push   edi
        push   esi
        push   ecx
        mov    ecx, 0x18
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_3_02AE
        lodsd  eax, dword ptr [esi]
        xchg   ah, al
        ror    eax, 0x10
        xchg   ah, al
        cmp    eax, dword ptr [edi]
        jb     loc_3_02AE
        cmp    eax, dword ptr [edi + 4]
        ja     loc_3_02AE
        add    esp, 0xc
        pop    esi
        or     byte ptr [esi + 0x482], 0x20
        jmp    loc_3_02B7
loc_3_02AE:
        pop    ecx
        pop    esi
        pop    edi
        add    edi, 0x20
        loop   loc_3_0284
        pop    esi
loc_3_02B7:
        push   esi
        mov    edi, 0                        ; -> loc_3_0000
        mov    ecx, 4
        lea    esi, [esi + 0x8a]
loc_3_02C8:
        push   esi
        push   edi
        push   ecx
        mov    cl, 8
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    ecx
        pop    edi
        pop    esi
        je     loc_3_03CC
        lea    edi, [edi + 0xa]
        loop   loc_3_02C8
loc_3_02DD:
        pop    esi
        mov    ebp, 0x114                    ; -> obj4:0x0114
        test   byte ptr [esi + 0x24], 2
        jne    loc_3_037C
        mov    al, byte ptr [esi + 0x84]
        and    al, 7
        cmp    al, 2
        jae    loc_3_034C
        mov    ebp, 0x108                    ; -> obj4:0x0108
loc_3_02FE:
        inc    byte ptr [0x21c]              ; -> obj4:0x021C
        mov    eax, dword ptr [esi + 0x1c]
        sub    esp, 0x18
        mov    edi, esp
        mov    word ptr [edi], 5
        mov    dword ptr [edi + 0x12], eax
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [edi + 8], 0xc4     ; -> loc_1_00C4
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [edi + 0x16], al
        push   1
        push   dword ptr [ebp + 4]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        mov    dword ptr [edi + 0xc], eax
        mov    dword ptr [eax], ebp
        mov    ax, word ptr [ebp + 2]
        mov    word ptr [edi + 0x10], ax
        push   edi
        call   sub_1_017C
        add    esp, 0x1c
loc_3_034C:
        pop    esi
        pop    ebp
        ret    
loc_3_034F:
        push   esp
        dec    edi
        push   edx
        imul   edx, dword ptr [ebx + 0x41], 0x4443204e
        sub    eax, 0x204d4f52
        inc    ebx
        inc    esp
        push   edx
        db  02Dh, 053h, 031h
loc_3_0364:
        dec    ebp
        inc    ecx
        push   esp
        push   ebx
        dec    eax
        dec    ecx
        push   esp
        inc    ecx
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    ebx
        push   edx
        sub    eax, 0x20313735
        and    byte ptr [eax], ah
loc_3_037C:
        push   esi
        push   edi
        lea    esi, [esi + 0x8a]
        lea    edi, [0x34f]                  ; -> loc_3_034F
        mov    ecx, 0x15
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    edi
        pop    esi
        je     loc_3_03C0
        cmp    ecx, 1
        jne    loc_3_03A3
        cmp    byte ptr [esi + 0x9e], 0x33
        je     loc_3_03C0
loc_3_03A3:
        push   esi
        push   edi
        lea    esi, [esi + 0x8a]
        lea    edi, [0x364]                  ; -> loc_3_0364
        mov    ecx, 6
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        pop    edi
        pop    esi
        jne    loc_3_02FE
loc_3_03C0:
        or     byte ptr [esi + 0x482], 0x40
        jmp    loc_3_02FE
loc_3_03CC:
        movzx  ecx, word ptr [edi + 8]
        mov    esi, dword ptr [esp]
        cmp    ecx, 0
        jne    loc_3_0423
        cmp    dword ptr [esi + 0x92], 0x522d4443
        jne    loc_3_0423
        cmp    dword ptr [esi + 0x96], 0x44204d4f
        jne    loc_3_0423
        cmp    dword ptr [esi + 0x9a], 0x45564952
        jne    loc_3_0423
        cmp    dword ptr [esi + 0x9e], 0x3230353a
        jne    loc_3_0423
        mov    eax, dword ptr [esi + 0xa2]
        xchg   ah, al
        ror    eax, 0x10
        xchg   ah, al
        cmp    eax, 0x322e3079
        jae    loc_3_02DD
loc_3_0423:
        mov    edi, dword ptr [ecx*4 + 0x28] ; -> loc_3_0028
        movzx  ecx, word ptr [edi]
        lea    edi, [edi + 2]
        lea    esi, [esi + 0x92]
loc_3_0436:
        push   ecx
        push   esi
        push   edi
        mov    ecx, 4
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        pop    edi
        pop    esi
        pop    ecx
        je     loc_3_044F
        lea    edi, [edi + 0x12]
        loop   loc_3_0436
        jmp    loc_3_02DD
loc_3_044F:
        movzx  ebp, word ptr [edi + 0x10]
        mov    ebp, dword ptr [ebp*4 + 0x238] ; -> loc_3_0238
        pop    esi
        or     byte ptr [esi + 0x482], 8
        jmp    loc_3_02FE
loc_3_0467:
        mov    esi, dword ptr [ebx + 0xc]
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_0488
        cmp    byte ptr [esi + 0x40], 5
        jne    loc_3_0488
        mov    al, byte ptr [esi + 0x84]
        and    al, 7
        cmp    al, 2
        jae    loc_3_0488
        dec    byte ptr [0x21c]              ; -> obj4:0x021C
loc_3_0488:
        ret    
loc_3_0489:
        cmp    byte ptr [0x21c], 0           ; -> obj4:0x021C
        jne    loc_3_0498
        mov    word ptr [ebx + 2], 0xffff
loc_3_0498:
        ret    
        db  000h, 000h, 000h
loc_3_049C:
        mov    esi, edx
        mov    ebx, dword ptr [esi + 8]
        test   byte ptr [esi + 0x6d], 0x80
        je     loc_3_0502
        mov    edi, dword ptr [esi + 0x20]
        mov    eax, dword ptr [esi + 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 0x10], eax
        movsx  eax, word ptr [eax + 0x10]
        add    eax, esi
        test   byte ptr [eax + 0xd], 1
        je     loc_3_04C5
        cmp    dword ptr [eax + 0x1f], 0
        jne    loc_3_0508
loc_3_04C5:
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_050F
loc_3_04CB:
        mov    ebx, dword ptr [esi + 0x10]
        movsx  ebx, word ptr [ebx + 0x10]
        add    ebx, esi
        test   byte ptr [ebx + 0xd], 1
        je     loc_3_0502
        push   edi
        push   esi
        mov    cl, byte ptr [ebx]
        mov    byte ptr [edi + 0xa], cl
        mov    ecx, dword ptr [ebx + 0x17]
        mov    dword ptr [edi + 0x10], ecx
        mov    ecx, dword ptr [ebx + 0x1b]
        mov    dword ptr [edi + 0xc], ecx
        mov    ecx, dword ptr [ebx + 0x13]
        mov    dword ptr [esi + 0x7c], ecx
        lea    esi, [ebx + 1]
        lea    edi, [edi + 0x30]
        mov    ecx, 3
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        pop    edi
loc_3_0502:
        call   sub_3_0578
loc_3_0507:
        ret    
loc_3_0508:
        call   dword ptr [eax + 0x1f]
        cmp    al, 3
        jne    loc_3_0522
loc_3_050F:
        xor    eax, eax
        mov    edx, dword ptr [esi + 0x10]
        mov    edx, dword ptr [edx + 8]
        mov    edx, dword ptr [edx]
        cmp    dword ptr [edx + 8], 0
        je     loc_3_0522
        call   dword ptr [edx + 8]
loc_3_0522:
        or     al, al
        je     loc_3_04CB
        cmp    al, 2
        je     loc_3_0507
        mov    eax, dword ptr [esi + 0x10]
        add    dword ptr [esi + 0x14], 8
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x10], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        jmp    loc_3_0507
loc_3_053F:
        cmp    byte ptr [edi + 0x30], 0x43
        jne    loc_3_0575
        mov    byte ptr [edi + 3], 1
        mov    eax, dword ptr [esi + 0x80]
        mov    dword ptr [eax], 0x1011200
        add    eax, 4
        mov    dword ptr [eax], 0x10000
        mov    dword ptr [eax + 4], 0x4e20
        add    eax, 8
        mov    dword ptr [eax], 0xaa0000
        mov    dword ptr [eax + 4], 0x1770
loc_3_0575:
        xor    al, al
        ret    
sub_3_0578:
        mov    edi, dword ptr [esi + 0x14]
        sub    edi, 8
        mov    dword ptr [esi + 0x14], edi
        push   esi
        call   dword ptr [edi]
        add    esp, 4
        ret    
sub_3_0588:
        push   ebx
        push   dword ptr [ebx + 0x10]
        call   sub_3_05E0
        or     edi, edi
        je     loc_3_05BF
        mov    dword ptr [edi + 0xa4], edi
        pop    dword ptr [edi + 0x10]
        mov    word ptr [edi + 0x68], 0x12
        mov    dword ptr [edi + 8], esi
        mov    ebx, edi
        call   sub_3_0664
        pop    dword ptr [ebx + 0x90]
        mov    dl, byte ptr [esi + 0x7d]
        shl    dl, 5
        mov    byte ptr [edi + 0x31], dl
        clc    
        ret    
loc_3_05BF:
        pop    ebx
        pop    ebx
        stc    
        ret    
sub_3_05C3:
        push   esi
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        call   sub_1_017C
        add    esp, 0xc
        pop    esi
        ret    
sub_3_05E0:
        push   esi
        mov    esi, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        add    ecx, 0x64
        sub    esp, 0x10
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    cx, 0x106
        add    cx, word ptr [esi + 0x80]
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0xe], 0
        mov    word ptr [edi], 2
        push   edi
        call   sub_1_017C
        mov    edi, dword ptr [edi + 0xa]
        add    esp, 0x14
        mov    eax, edi
        or     edi, edi
        je     loc_3_0662
        add    edi, dword ptr [esi + 4]
        xor    eax, eax
        mov    word ptr [edi + 0xc], 0xf
        mov    word ptr [edi + 0xe], 0xf
        mov    dword ptr [edi + 0x64], eax
        mov    dword ptr [edi + 0xa8], eax
        mov    dword ptr [edi + 0x7c], eax
        mov    dword ptr [edi + 0x6c], 0x8c09
        pop    esi
        mov    al, byte ptr [esi + 0x14]
        mov    byte ptr [edi + 0xb1], al
        mov    eax, edi
        add    eax, 0xbc
        mov    dword ptr [edi + 0xac], eax
        lea    ecx, [edi + 0xcc]
        mov    dword ptr [edi + 0x20], ecx
        ret    
loc_3_0662:
        pop    esi
        ret    
sub_3_0664:
        push   edx
        mov    edi, ecx
        mov    dword ptr [edi + 0x40], ebx
        mov    dword ptr [edi], 0x40
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 3], 0
        xor    eax, eax
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [edi + 0x44], eax
        mov    dword ptr [edi + 0x20], eax
        mov    byte ptr [edi + 0x4c], al
        mov    byte ptr [edi + 8], al
        mov    byte ptr [edi + 9], al
        mov    dword ptr [edi + 0x18], eax
        mov    dword ptr [edi + 0x14], 0xf
        mov    al, byte ptr [esi + 0x75]
        mov    byte ptr [edi + 5], al
        mov    al, byte ptr [esi + 0x7c]
        mov    byte ptr [edi + 6], al
        mov    al, byte ptr [esi + 0x7d]
        mov    byte ptr [edi + 7], al
        mov    edx, edi
        add    edx, 0x50
        mov    dword ptr [edi + 0x28], edx
        movzx  eax, word ptr [esi + 0x80]
        add    edx, eax
        mov    dword ptr [edi + 0x1c], edx
        mov    byte ptr [edi + 0xb], 0xe
        pop    edx
        ret    
sub_3_06C2:
        push   ebx
        call   sub_1_0190
        add    esp, 4
        or     al, al
        je     loc_3_06D4
        or     dword ptr [ebx + 0x6c], 0x40
loc_3_06D4:
        mov    eax, dword ptr [ebx + 0x7c]
        test   byte ptr [ebx + 0x6d], 2
        jne    loc_3_06E3
        mul    dword ptr [esi + 0xdb]
loc_3_06E3:
        mov    dword ptr [edi + 0x10], eax
        mov    edx, 0xc4                     ; -> loc_1_00C4
        mov    byte ptr [edi + 3], 0
        mov    cx, word ptr [ebx + 0xe]
        mov    word ptr [ebx + 0xc], cx
        xchg   esi, edi
        xchg   edi, ebx
        mov    dword ptr [edi + 0xa4], edi
        call   sub_1_0188
        ret    
loc_3_0708:
        push   ebp
        push   edi
        push   esi
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [ebx + 0x20]
        mov    eax, dword ptr [ebx + 0x10]
        mov    ecx, dword ptr [ebx + 0x14]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], 0x40         ; -> loc_1_0040
        mov    edx, dword ptr [eax + 8]
        mov    edx, dword ptr [edx]
        mov    al, byte ptr [edi + 0x30]
        movzx  ecx, word ptr [edx]
        lea    edx, [edx + 0xc]
        jecxz  loc_3_073B
loc_3_0732:
        cmp    byte ptr [edx], al
        je     loc_3_0742
        lea    edx, [edx + 0xc]
        loop   loc_3_0732
loc_3_073B:
        mov    al, 1
        jmp    loc_1_00F5
loc_3_0742:
        push   edi
        push   esi
        lea    esi, [edi + 0x30]
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        mov    cl, byte ptr [edi + 0xa]
        mov    byte ptr [eax], cl
        or     byte ptr [eax + 0xd], 1
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x17], ecx
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0x1b], ecx
        mov    ecx, dword ptr [ebx + 0x7c]
        mov    dword ptr [eax + 0x13], ecx
        lea    edi, [eax + 1]
        mov    ecx, 3
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        pop    edi
        mov    ecx, dword ptr [edx + 8]
        mov    dword ptr [eax + 0x1f], ecx
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [edi + 0x30], al
        xor    eax, eax
        cmp    dword ptr [edx + 4], eax
        je     loc_1_00F5
        call   dword ptr [edx + 4]
        jmp    loc_1_00F5
sub_3_0796:
        push   edx
        mov    eax, 0x1194
        movzx  cx, ch
        mul    cx
        shl    edx, 0x10
        add    eax, edx
        ror    ecx, 0x10
        movzx  edx, ch
        add    edx, eax
        xchg   ecx, edx
        mov    eax, 0x4b
        mul    dl
        add    ecx, eax
        cmp    ecx, 0x96
        jb     loc_3_07C9
        sub    ecx, 0x96
loc_3_07C9:
        pop    edx
        ret    
sub_3_07CB:
        push   edx
        push   ebx
        add    ecx, 0x96
        mov    ebx, 0x1194
        mov    eax, ecx
        xor    edx, edx
        div    ebx
        movzx  ecx, al
        mov    eax, edx
        mov    ebx, 0x4b
        xor    edx, edx
        div    bx
        mov    ch, al
        shl    ecx, 8
        shl    edx, 0x18
        or     ecx, edx
        pop    ebx
        pop    edx
        ret    
sub_3_07FA:
        mov    ecx, 3
        mov    eax, ebx
loc_3_0801:
        rol    eax, 8
        aam    0xa
        shl    ah, 4
        or     ah, al
        mov    bl, ah
        shl    ebx, 8
        loop   loc_3_0801
        ret    
sub_3_0813:
        xor    al, al
        push   ecx
        mov    cl, 3
        movzx  ecx, cl
loc_3_081B:
        rol    eax, 8
        shl    ax, 4
        shr    al, 4
        aad    0xa
        shl    ax, 8
        loop   loc_3_081B
        pop    ecx
        ret    
        db  000h
loc_3_0830:
        call   sub_3_1101
        jb     loc_3_0844
        test   dl, 4
        jne    loc_3_0844
        mov    byte ptr [edi + 0x30], 0xd8
        mov    byte ptr [edi + 0xa], 0xa
loc_3_0844:
        xor    al, al
        ret    
sub_3_0847:
        mov    cl, byte ptr [esi + 0x84]
        and    cl, 7
        cmp    cl, 2
        jae    loc_3_0DC2
        mov    ecx, dword ptr [edi + 0x32]
        call   sub_3_0796
        mov    ebp, ecx
        xchg   cl, ch
        ror    ecx, 0x10
        xchg   cl, ch
        mov    dword ptr [edi + 0x32], ecx
        mov    ecx, dword ptr [edi + 0x35]
        call   sub_3_0796
        jmp    loc_3_08A9
sub_3_0877:
        movzx  ecx, word ptr [edi + 0x37]
        shl    ecx, 0x10
        jecxz  loc_3_08C3
        mov    dword ptr [edi + 0x36], ecx
sub_3_0883:
        mov    al, byte ptr [esi + 0x84]
        and    al, 7
        cmp    al, 2
        jae    loc_3_0DC2
        mov    ecx, dword ptr [edi + 0x36]
        xchg   cl, ch
        ror    ecx, 0x10
        xchg   cl, ch
        mov    eax, dword ptr [edi + 0x32]
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
        add    ecx, eax
loc_3_08A9:
        mov    byte ptr [edi + 0x30], 0xd8
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        mov    dword ptr [eax + 0xf], ecx
        xor    eax, eax
        mov    dword ptr [edi + 0x36], eax
        mov    byte ptr [edi + 0xa], 0xa
        ret    
loc_3_08C3:
        mov    byte ptr [edi + 3], 1
        mov    al, 2
        ret    
sub_3_08CA:
        mov    ecx, dword ptr [eax + 0xf]
        xchg   ch, cl
        ror    ecx, 0x10
        xchg   ch, cl
        cmp    ecx, dword ptr [edi + 0x32]
        je     loc_3_08FF
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_10A3
        mov    dword ptr [eax + 0x1f], 0
        mov    dword ptr [edi + 0x32], ecx
        mov    ebx, dword ptr [esi + 0x10]
        mov    ebx, dword ptr [ebx + 8]
        mov    ah, byte ptr [ebx + 4]
        mov    al, 0xd9
        mov    word ptr [edi + 0x30], ax
        mov    al, 1
        ret    
loc_3_08FF:
        mov    byte ptr [edi + 3], 1
        xor    al, al
        ret    
loc_3_0906:
        call   sub_3_0847
        jmp    loc_3_0919
loc_3_090D:
        call   sub_3_0883
        jmp    loc_3_0919
loc_3_0914:
        call   sub_3_0877
loc_3_0919:
        cmp    al, 2
        je     loc_3_0941
loc_3_091D:
        mov    ecx, dword ptr [edi + 0x32]
        xchg   cl, ch
        ror    ecx, 0x10
        xchg   cl, ch
        push   ebx
        push   eax
        call   sub_3_07CB
        mov    ebx, ecx
        call   sub_3_07FA
        pop    eax
        ror    ebx, 8
        mov    dword ptr [edi + 0x32], ebx
        pop    ebx
        mov    byte ptr [edi + 0x39], 0x40
loc_3_0941:
        ret    
loc_3_0942:
        call   sub_3_08CA
        cmp    al, 1
        jne    loc_3_0941
        jmp    loc_3_091D
loc_3_094D:
        cmp    byte ptr [edi + 0x39], 0x80
        je     loc_3_0A25
        call   sub_3_0588
        jb     loc_3_09F8
        sub    esp, 0x10
        mov    edx, esp
        mov    word ptr [edx + 4], 0x41c
        mov    word ptr [edx + 0xa], 9
        mov    word ptr [edx], 3
        mov    byte ptr [edx + 0xc], 8
        push   edx
        call   sub_1_017C
        mov    eax, dword ptr [edx + 6]
        add    esp, 0x14
        or     eax, eax
        je     loc_3_0A09
        mov    dword ptr [ebx + 0x80], eax
        mov    dword ptr [ebx + 0x7c], 0x41c
        and    dword ptr [ebx + 0x6c], 0xfffffffd
        or     dword ptr [ebx + 0x6c], 0x200
        mov    dword ptr [edi + 0xc], 0x248
        mov    byte ptr [edi + 0xa], 0xa
        mov    word ptr [edi + 0x30], 0x3de
        xor    eax, eax
        mov    dword ptr [edi + 0x32], eax
        mov    dword ptr [edi + 0x36], eax
        mov    byte ptr [ebx + 0xb0], al
        mov    dword ptr [ebx + 0x70], 0x52  ; -> loc_1_0052
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        mov    dword ptr [eax + 0x1f], 0xa54 ; -> loc_3_0A54
        mov    eax, dword ptr [ebx + 0x90]
        mov    eax, dword ptr [eax + 0x20]
        test   byte ptr [eax + 0x39], 0x40
        jne    loc_3_0A1F
loc_3_09EA:
        push   ebx
        push   edi
        push   esi
        call   sub_3_06C2
        pop    esi
        pop    edi
        pop    ebx
        mov    al, 3
        ret    
loc_3_09F8:
        mov    word ptr [ebx + 0x6a], 0x1d
        mov    byte ptr [edi + 3], 4
        mov    byte ptr [edi + 4], 8
        mov    al, 2
        ret    
loc_3_0A09:
        push   dword ptr [ebx + 0x90]
        mov    edi, ebx
        mov    ebx, dword ptr [edi + 8]
        call   sub_3_05C3
        pop    ebx
        mov    edi, dword ptr [ebx + 0x20]
        jmp    loc_3_09F8
loc_3_0A1F:
        mov    byte ptr [edi + 0x32], 0xb0
        jmp    loc_3_09EA
loc_3_0A25:
        mov    word ptr [ebx + 0x6a], 0x16
        mov    byte ptr [edi + 3], 0x86
        mov    byte ptr [edi + 4], 2
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [eax], 0x50070
        mov    dword ptr [eax + 4], 0xb000000
        xor    ecx, ecx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0xe], ecx
        mov    word ptr [eax + 0xc], 0x24
        mov    al, 2
        ret    
loc_3_0A54:
        cmp    byte ptr [edi + 3], 1
        je     loc_3_0A64
        cmp    byte ptr [edi + 3], 0x12
        jne    loc_3_0BF8
loc_3_0A64:
        mov    ebp, dword ptr [esi + 0x80]
        mov    ecx, dword ptr [esi + 0x90]
        mov    edx, dword ptr [ecx + 0x80]
        cmp    byte ptr [edi + 0x32], 0xb0
        je     loc_3_0C4F
        movzx  eax, byte ptr [ebp + 0x13]
        shl    ax, 4
        shr    al, 4
        aad    0xa
        mov    bl, al
        movzx  eax, byte ptr [ebp + 9]
        shl    ax, 4
        shr    al, 4
        aad    0xa
        mov    ah, bl
        mov    ebp, dword ptr [ecx + 0x20]
        mov    word ptr [edx + 2], ax
        movzx  ecx, byte ptr [ebp + 0x36]
        or     ecx, ecx
        je     loc_3_0BF1
        cmp    cl, al
        jb     loc_3_0C2A
        cmp    cl, ah
        ja     loc_3_0C2A
loc_3_0AC1:
        sub    ah, cl
        sub    cl, al
        xchg   cl, ah
        movzx  ebx, ah
        inc    ecx
        lea    ebx, [ebx + ebx*4]
        lea    eax, [ecx*8 + 0xa]
        xchg   al, ah
        mov    word ptr [edx], ax
        push   edi
        push   esi
        lea    edi, [edx + 4]
        mov    esi, dword ptr [esi + 0x80]
        lea    esi, [esi + ebx*2 + 0x20]
loc_3_0AE9:
        xor    eax, eax
        lodsw  ax, word ptr [esi]
        ror    al, 4
        shl    eax, 0x18
        lodsb  al, byte ptr [esi]
        shl    ax, 4
        shr    al, 4
        aad    0xa
        ror    eax, 0x10
        stosd  dword ptr es:[edi], eax
        lea    esi, [esi + 3]
        lodsd  eax, dword ptr [esi]
        call   sub_3_0813
        test   byte ptr [ebp + 0x31], 2
        jne    loc_3_0B22
        push   ecx
        mov    ecx, eax
        call   sub_3_0796
        mov    eax, ecx
        pop    ecx
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
loc_3_0B22:
        stosd  dword ptr es:[edi], eax
        loop   loc_3_0AE9
        mov    eax, 0xaa0000
        stosd  dword ptr es:[edi], eax
        pop    esi
        mov    ebx, dword ptr [esi + 0x80]
        mov    eax, dword ptr [ebx + 0x1c]
        call   sub_3_0813
        test   byte ptr [ebp + 0x31], 2
        jne    loc_3_0B50
        mov    ecx, eax
        call   sub_3_0796
        mov    eax, ecx
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
loc_3_0B50:
        stosd  dword ptr es:[edi], eax
        pop    edi
        mov    byte ptr [ebp + 3], 1
        mov    ebx, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [ecx + 8]
        mov    byte ptr [ecx + 5], 0
        mov    eax, dword ptr [esi + 0x80]
        cmp    byte ptr [eax + 0xa], 0x20
        jne    loc_3_0BC1
        mov    byte ptr [ecx + 5], 0xc0
        mov    dword ptr [esi + 0x7c], 0xa
        mov    dword ptr [edi + 0x30], 0x15
        mov    word ptr [edi + 0x34], 0xa
        mov    byte ptr [edi + 0xa], 6
        mov    dword ptr [edi + 0xc], 0x288
        xor    edx, edx
        mov    dword ptr [eax], edx
        mov    byte ptr [eax + 4], 0xc0
        mov    dword ptr [eax + 5], edx
        mov    byte ptr [eax + 9], 0xf
        mov    eax, dword ptr [esi + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, esi
        mov    dword ptr [eax + 0x1f], 0xbc1 ; -> loc_3_0BC1
        push   ebx
        push   edi
        push   esi
        xchg   esi, ebx
        call   sub_3_06C2
        pop    esi
        pop    edi
        pop    ebx
        mov    al, 2
        ret    
loc_3_0BC1:
        mov    ebx, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 0x80]
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], edx
        mov    word ptr [eax], 4
        push   eax
        call   sub_1_017C
        add    esp, 0xc
        mov    edi, esi
        mov    esi, dword ptr [edi + 0x90]
        call   sub_3_05C3
        xor    al, al
        ret    
loc_3_0BF1:
        mov    cl, al
        jmp    loc_3_0AC1
loc_3_0BF8:
        mov    eax, dword ptr [esi + 0x90]
        mov    eax, dword ptr [eax + 0x20]
        movzx  ecx, word ptr [edi + 3]
        mov    word ptr [eax + 3], cx
        push   edi
        push   esi
        movzx  ecx, byte ptr [eax + 0xb]
        mov    esi, dword ptr [edi + 0x1c]
        mov    edi, dword ptr [eax + 0x1c]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    edi
        jmp    loc_3_0BC1
loc_3_0C1B:
        jo     loc_3_0C1D
loc_3_0C1D:
        add    eax, 0x60000
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        db  000h
loc_3_0C2A:
        mov    eax, dword ptr [esi + 0x90]
        mov    eax, dword ptr [eax + 0x20]
        mov    word ptr [eax + 3], 0x284
        mov    edi, dword ptr [eax + 0x1c]
        push   esi
        mov    esi, 0xc1b                    ; -> loc_3_0C1B
        mov    ecx, 0xf
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        jmp    loc_3_0BC1
loc_3_0C4F:
        mov    ebx, dword ptr [ecx + 0x20]
        mov    word ptr [edx], 0xa00
        cmp    byte ptr [ebp + 0xe], 0xb0
        je     loc_3_0C83
        mov    word ptr [edx + 2], 0x101
        mov    eax, 0x20000
        test   byte ptr [ebx + 0x31], 2
        jne    loc_3_0C70
        xor    eax, eax
loc_3_0C70:
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 4], 0x100
        mov    byte ptr [ebx + 3], 1
        jmp    loc_3_0BC1
loc_3_0C83:
        mov    word ptr [edx + 2], 0x201
        mov    eax, dword ptr [ebp + 0xe]
        call   sub_3_0813
        ror    eax, 8
        add    ah, 2
        cmp    ah, 0x3c
        jb     loc_3_0CA1
        sub    ah, 0x3c
        inc    al
loc_3_0CA1:
        rol    eax, 8
        test   byte ptr [ebx + 0x31], 2
        jne    loc_3_0C70
        mov    ecx, eax
        call   sub_3_0796
        xchg   ch, cl
        ror    ecx, 0x10
        xchg   ch, cl
        mov    eax, ecx
        jmp    loc_3_0C70
loc_3_0CBC:
        mov    ebp, dword ptr [ebx + 0x80]
        movzx  eax, byte ptr [ebp + 3]
        cmp    byte ptr [eax + ebp + 4], 1
        je     loc_3_0D40
        lea    ebp, [eax + ebp + 4]
        cmp    byte ptr [ebp], 0xe
        jne    loc_3_0D31
        mov    cl, byte ptr [esi + 0x84]
        and    cl, 7
        cmp    cl, 2
        jae    loc_3_0DC2
        xor    edx, edx
        movzx  eax, byte ptr [ebp + 0xb]
        sub    eax, 1
        cmc    
        rcl    dh, 1
        movzx  eax, byte ptr [ebp + 9]
        sub    eax, 1
        cmc    
        rcl    dh, 1
        mov    eax, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    byte ptr [eax + 4], dh
        test   byte ptr [esi + 0x47e], 1
        mov    byte ptr [edi + 0xa], 0xa
        mov    dl, 0xd9
        mov    dword ptr [edi + 0x30], edx
        xor    eax, eax
        mov    dword ptr [ebx + 0x7c], eax
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0xc], 0
        mov    dword ptr [edi + 0x34], eax
        mov    ah, 0xc0
        mov    word ptr [edi + 0x38], ax
        ret    
loc_3_0D31:
        mov    al, 1
        ret    
loc_3_0D34:
        or     byte ptr [eax], al
        or     dword ptr [eax], esp
        or     dword ptr [ecx + ecx], esp
        db  030h
loc_3_0D3C:
        pop    ss
        pop    es
        add    al, byte ptr [eax]
loc_3_0D40:
        movzx  eax, word ptr [ebp + 0xa]
        push   edi
        mov    edi, 0xd34                    ; -> loc_3_0D34
        mov    ecx, 4
        repne scasw ax, word ptr es:[edi]
        pop    edi
        jne    loc_3_0A25
        mov    edx, dword ptr [ebx + 0x10]
        mov    edx, dword ptr [edx + 8]
        movzx  eax, byte ptr [edx + 5]
        and    al, 0xc0
        or     al, byte ptr [ecx + 0xd3c]    ; -> loc_3_0D3C
        mov    byte ptr [edx + 5], al
        mov    cl, byte ptr [esi + 0x84]
        and    cl, 7
        cmp    cl, 2
        jae    loc_3_0DB8
        and    byte ptr [edi + 0x31], 0xef
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [ebx + 0x5c], ecx
        mov    cl, 0xa
        movzx  ecx, cl
        mov    dword ptr [ebx + 0x7c], ecx
        mov    dword ptr [edi + 0x10], ecx
        mov    byte ptr [edi + 0x34], cl
        and    al, 0xef
        xor    ecx, ecx
        mov    dword ptr [ebp], ecx
        mov    dword ptr [ebp + 4], eax
        mov    byte ptr [ebp + 8], cl
        mov    byte ptr [ebp + 9], 0xa
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        mov    dword ptr [eax + 0x1f], 0xdd5 ; -> loc_3_0DD5
        xor    al, al
        ret    
loc_3_0DB8:
        test   al, 0x10
        je     loc_3_0DC2
        mov    word ptr [ebp + 0xa], 0x2409
loc_3_0DC2:
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        mov    dword ptr [eax + 0x1f], 0
        xor    al, al
        ret    
loc_3_0DD5:
        mov    ecx, dword ptr [esi + 0x80]
        mov    eax, dword ptr [esi + 0x5c]
        mov    dword ptr [ecx], eax
        xor    al, al
        mov    byte ptr [ecx + 4], al
        ret    
loc_3_0DE6:
        test   byte ptr [edi + 3], 0x80
        je     loc_3_0DF9
        mov    eax, dword ptr [edi + 0x1c]
        cmp    byte ptr [eax + 0xc], 0x2c
        jne    loc_3_0DF9
        mov    byte ptr [edi + 3], 1
loc_3_0DF9:
        xor    al, al
        ret    
loc_3_0DFC:
        sbb    eax, dword ptr [ecx]
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        push   cs
        push   cs
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0E18:
        mov    al, byte ptr [edi + 0x32]
        and    al, 0x3f
        cmp    al, 1
        je     loc_3_0E93
        cmp    al, 0xe
        jne    loc_3_0D31
        test   byte ptr [edi + 0x32], 0x40
        jne    loc_3_0E3D
        mov    cl, byte ptr [esi + 0x84]
        and    cl, 7
        cmp    cl, 2
        jae    loc_3_0DC2
loc_3_0E3D:
        cmp    dword ptr [edi + 0x10], 0x1b
        jb     loc_3_0D31
        push   ebp
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 0x80]
        mov    ebp, edi
        lea    esi, [0xdfc]                  ; -> loc_3_0DFC
        mov    ecx, 0x1b
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    edi
        mov    eax, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax + 4]
        rcr    al, 1
        mov    dl, 0xff
        test   byte ptr [edi + 0x32], 0x40
        jne    loc_3_0E79
        sbb    dl, dl
loc_3_0E79:
        mov    byte ptr [ebp + 0x15], dl
        rcr    al, 1
        mov    dl, 0xff
        test   byte ptr [edi + 0x32], 0x40
        jne    loc_3_0E88
        sbb    dl, dl
loc_3_0E88:
        mov    byte ptr [ebp + 0x17], dl
        mov    byte ptr [edi + 3], 1
        pop    ebp
        mov    al, 2
        ret    
loc_3_0E93:
        cmp    dword ptr [ebx + 0x7c], 0x14
        test   byte ptr [edi + 0x32], 0x40
        jne    loc_3_0EEA
        mov    eax, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [eax + 8]
        movzx  eax, byte ptr [eax + 5]
        and    al, 0x3f
        push   edi
        mov    ecx, 4
        mov    edi, 0xd3c                    ; -> loc_3_0D3C
        repne scasb al, byte ptr es:[edi]
        pop    edi
        movzx  eax, word ptr [ecx*2 + 0xd34] ; -> loc_3_0D34
loc_3_0EBF:
        mov    ecx, dword ptr [ebx + 0x80]
        mov    dword ptr [ecx], 0x8000013
        shl    eax, 0x10
        mov    dword ptr [ecx + 8], eax
        xor    eax, eax
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 0xc], 0x80001
        mov    dword ptr [ecx + 0x10], eax
        mov    byte ptr [edi + 3], 1
        mov    eax, 2
        ret    
loc_3_0EEA:
        mov    eax, 0xff0f
        jmp    loc_3_0EBF
loc_3_0EF1:
        cmp    byte ptr [edi + 0x33], 3
        ja     loc_3_0F14
        mov    dword ptr [ebx + 0x7c], 0xa
        mov    dword ptr [edi + 0x10], 0xa
        mov    word ptr [edi + 0x30], 0xadd
        xor    eax, eax
        mov    dword ptr [edi + 0x32], eax
        mov    dword ptr [edi + 0x36], eax
        ret    
loc_3_0F14:
        mov    al, 1
        ret    
loc_3_0F17:
        adc    dword ptr [eax], eax
        adc    dl, byte ptr [ebx]
loc_3_0F1B:
        sub    ecx, dword ptr [ebx]
        adc    edx, dword ptr [ebx]
loc_3_0F1F:
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_0FF0
        mov    ebx, dword ptr [esi + 0x80]
        movzx  eax, byte ptr [ebx]
        movzx  eax, byte ptr [eax + 0xf17]   ; -> loc_3_0F17
        mov    ebp, dword ptr [esi + 0x10]
        movsx  ebp, word ptr [ebp + 0x10]
        add    ebp, esi
        movzx  ecx, byte ptr [ebp + 4]
        cmp    cl, 3
        ja     loc_3_0FF0
        mov    edx, dword ptr [ebx + 1]
        push   edi
        lea    edi, [ebx + 5]
        mov    byte ptr [ebx + 4], cl
        cmp    cl, 1
        pushfd 
        movzx  ecx, byte ptr [ecx + 0xf1b]   ; -> loc_3_0F1B
        shl    ecx, 0x18
        mov    dword ptr [ebx], ecx
        shr    ecx, 0x18
        mov    byte ptr [ebx + 1], al
        popfd  
        ja     loc_3_0FEA
        mov    al, dl
        push   word ptr [ebx + 9]
        push   dword ptr [ebx + 5]
        shl    al, 4
        stosb  byte ptr es:[edi], al
        movzx  eax, dh
        shl    eax, 4
        shr    al, 4
        aad    0xa
        shr    edx, 0x10
        mov    ah, dh
        xor    dh, dh
        shl    dx, 4
        shr    dl, 4
        xchg   dx, ax
        aad    0xa
        mov    ah, al
        mov    al, dl
        xor    dl, dl
        stosw  word ptr es:[edi], ax
        shrd   eax, edx, 0x10
        pop    ax
        ror    eax, 0x10
        call   sub_3_0813
        pop    edx
        shl    edx, 8
        xchg   edx, eax
        call   sub_3_0813
        test   byte ptr [ebp + 2], 2
        jne    loc_3_0FE3
        push   ecx
        mov    ecx, eax
        call   sub_3_0796
        xchg   cl, ch
        ror    ecx, 0x10
        xchg   cl, ch
        xchg   edx, ecx
        call   sub_3_0796
        xchg   cl, ch
        ror    ecx, 0x10
        xchg   cl, ch
        mov    eax, ecx
        pop    ecx
        xchg   edx, eax
loc_3_0FE3:
        stosd  dword ptr es:[edi], eax
        mov    eax, edx
        stosd  dword ptr es:[edi], eax
        sub    ecx, 0xb
loc_3_0FEA:
        xor    al, al
        rep stosb byte ptr es:[edi], al
        pop    edi
        ret    
loc_3_0FF0:
        xor    al, al
        ret    
loc_3_0FF3:
        mov    word ptr [ebx + 0xe], 0x3c
        mov    word ptr [ebx + 0xc], 0x3c
        call   sub_3_1101
        jb     loc_3_100B
        test   dl, 4
        je     loc_3_1063
loc_3_100B:
        mov    edx, dword ptr [ebx + 0x10]
        mov    edx, dword ptr [edx + 8]
        test   byte ptr [edx + 5], 0x10
        je     loc_3_1060
        cmp    dword ptr [ebx + 0x7c], 0xc
        jb     loc_3_1060
        mov    eax, dword ptr [ebx + 0x80]
        mov    dword ptr [eax], 0xfffffff
        mov    dword ptr [eax + 4], 0xffffffff
        mov    dword ptr [eax + 8], 0xfffffff0
        add    dword ptr [ebx + 0x80], 0xc
        sub    dword ptr [ebx + 0x7c], 0xc
        sub    dword ptr [edi + 0x10], 0xc
        push   ebx
        call   sub_1_0190
        add    esp, 4
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        mov    dword ptr [eax + 0x1f], 0x108d ; -> loc_3_108D
loc_3_1060:
        xor    al, al
        ret    
loc_3_1063:
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        mov    dword ptr [eax + 0x1f], 0x10e1 ; -> loc_3_10E1
        xor    eax, eax
        mov    dword ptr [edi + 0x30], eax
        mov    dword ptr [edi + 0x34], eax
        mov    dword ptr [ebx + 0x7c], eax
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0xc], 0
        mov    byte ptr [edi + 0xa], 6
        ret    
loc_3_108D:
        add    dword ptr [esi + 0x7c], 0xc
        sub    dword ptr [esi + 0x80], 0xc
        push   esi
        call   sub_1_0190
        lea    esp, [esp + 4]
loc_3_10A3:
        cmp    byte ptr [edi + 3], 1
        je     loc_3_10D8
        test   byte ptr [edi + 3], 0x80
        je     loc_3_10D8
        mov    eax, dword ptr [edi + 0x1c]
        mov    cl, byte ptr [eax + 0xc]
        cmp    cl, 0x1d
        je     loc_3_10C9
        cmp    cl, 0x21
        je     loc_3_10C9
        cmp    cl, 0xd
        je     loc_3_10DB
        cmp    cl, 0x1c
        jne    loc_3_10D8
loc_3_10C9:
        mov    eax, dword ptr [edi + 0x1c]
        mov    byte ptr [edi + 3], 0x84
        mov    byte ptr [eax + 0xc], 0x64
        mov    byte ptr [eax + 2], 5
loc_3_10D8:
        xor    al, al
        ret    
loc_3_10DB:
        mov    byte ptr [eax + 0xc], 0x3a
        jmp    loc_3_10D8
loc_3_10E1:
        cmp    byte ptr [edi + 3], 1
        je     loc_3_10C9
        xor    al, al
        ret    
loc_3_10EA:
        xor    eax, eax
        cmp    byte ptr [edi + 0x34], 2
        jne    loc_3_1100
        mov    byte ptr [edi + 0x30], 0xdc
        mov    byte ptr [edi + 0xa], 0xa
        mov    dword ptr [edi + 0x32], eax
        mov    dword ptr [edi + 0x36], eax
loc_3_1100:
        ret    
sub_3_1101:
        push   ebx
        mov    esi, dword ptr [ebx + 8]
        movzx  ecx, word ptr [esi + 0x134]
        xchg   cl, ch
        movzx  eax, byte ptr [esi + 0x137]
        sub    al, byte ptr [esi + 0x136]
        inc    eax
        mov    edx, eax
        imul   eax, eax, 8
        cmp    eax, ecx
        jg     loc_3_115D
        cmp    ecx, 0x324
        jg     loc_3_115D
        lea    ebp, [esi + 0x138]
        mov    ecx, edx
        movzx  edx, byte ptr [ebp + 1]
        mov    ebx, dword ptr [edi + 0x32]
        xchg   bl, bh
        rol    ebx, 0x10
        xchg   bl, bh
        inc    ecx
loc_3_1144:
        push   ecx
        mov    ecx, dword ptr [ebp + 4]
        call   sub_3_0796
        mov    eax, ecx
        pop    ecx
        cmp    ebx, eax
        jb     loc_3_115D
        movzx  edx, byte ptr [ebp + 1]
        lea    ebp, [ebp + 8]
        loop   loc_3_1144
loc_3_115D:
        pop    ebx
        cmc    
        ret    
loc_3_1160:
        cmp    byte ptr [edi + 0x39], 0x80
        je     loc_3_0A25
        cmp    byte ptr [edi + 0x39], 0x40
        jne    loc_3_11A1
        mov    byte ptr [edi + 0x30], 0xc7
        mov    byte ptr [edi + 0x31], 3
        xor    ecx, ecx
        mov    dword ptr [edi + 0x32], ecx
        mov    dword ptr [edi + 0x36], ecx
        mov    dword ptr [edi + 0x10], 4
        mov    dword ptr [ebx + 0x7c], 4
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        mov    dword ptr [eax + 0x1f], 0x11a4 ; -> loc_3_11A4
        xor    al, al
        ret    
loc_3_11A1:
        mov    al, 1
        ret    
loc_3_11A4:
        cmp    byte ptr [edi + 3], 1
        je     loc_3_11B0
        cmp    byte ptr [edi + 3], 0x12
        jne    loc_3_120E
loc_3_11B0:
        mov    edx, dword ptr [esi + 0x80]
        mov    eax, dword ptr [edx]
        mov    ebp, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        or     al, al
        je     loc_3_11C4
        mov    al, 0x83
loc_3_11C4:
        mov    byte ptr [ecx + 5], al
        call   sub_3_0813
        movsx  ebp, word ptr [ebp + 0x10]
        add    ebp, esi
        test   byte ptr [ebp + 2], 2
        jne    loc_3_11E8
        mov    ecx, eax
        call   sub_3_0796
        mov    eax, ecx
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
loc_3_11E8:
        mov    word ptr [edx], 0xa00
        or     eax, eax
        jne    loc_3_1211
        mov    word ptr [edx + 2], 0x101
        mov    eax, 0x20000
        test   byte ptr [ebp + 2], 2
        jne    loc_3_1204
        xor    eax, eax
loc_3_1204:
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 4], 0x100
loc_3_120E:
        xor    al, al
        ret    
loc_3_1211:
        mov    ecx, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [ecx + 8]
        mov    byte ptr [ecx + 5], 0x83
        mov    word ptr [edx + 2], 0x201
        jmp    loc_3_1204
loc_3_1223:
        mov    eax, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [eax + 8]
        cmp    byte ptr [eax + 4], 0x82
        je     loc_3_12BB
        mov    dl, 0x82
loc_3_1235:
        push   edx
        call   sub_3_0588
        pop    edx
        jb     loc_3_09F8
        lea    ecx, [esi + 0x45c]
        mov    dword ptr [ebx + 0x80], ecx
        mov    byte ptr [ecx], 0
        mov    byte ptr [ecx + 3], 8
        mov    byte ptr [ecx + 4], dl
        mov    eax, 0x930
        cmp    dl, 0x82
        je     loc_3_1268
        mov    eax, dword ptr [esi + 0x490]
loc_3_1268:
        xchg   al, ah
        mov    word ptr [ecx + 0xa], ax
        mov    byte ptr [edi + 0x30], 0x15
        mov    byte ptr [edi + 0xa], 6
        xor    ecx, ecx
        mov    word ptr [edi + 0x32], cx
        mov    byte ptr [edi + 0x35], cl
        mov    cl, 0x14
        mov    dword ptr [ebx + 0x7c], ecx
        mov    byte ptr [edi + 0x34], cl
        or     byte ptr [edi + 0x31], 0x10
        or     byte ptr [ebx + 0x6d], 2
        mov    dword ptr [edi + 0xc], 0x288
        mov    dword ptr [ebx + 0x70], 0x52  ; -> loc_1_0052
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        mov    dword ptr [eax + 0x1f], 0x12be ; -> loc_3_12BE
        push   ebx
        push   edi
        push   esi
        call   sub_3_06C2
        pop    esi
        pop    edi
        pop    ebx
        mov    al, 3
        ret    
loc_3_12BB:
        mov    al, 1
        ret    
loc_3_12BE:
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_12D6
        mov    edx, dword ptr [esi + 0x80]
        mov    dl, byte ptr [edx + 4]
        mov    eax, dword ptr [esi + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    byte ptr [eax + 4], dl
loc_3_12D6:
        mov    edi, esi
        mov    esi, dword ptr [edi + 0x90]
        call   sub_3_05C3
        mov    al, 1
        ret    
loc_3_12E6:
        mov    eax, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dl, byte ptr [eax + 5]
        cmp    dl, byte ptr [eax + 4]
        jne    loc_3_1235
loc_3_12F8:
        mov    al, 1
        ret    
loc_3_12FB:
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [eax + 4], 0x83
        jne    loc_3_12F8
        cmp    word ptr [eax + 0xa], 0x3009
        jne    loc_3_12F8
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        mov    dword ptr [eax + 0x1f], 0x1326 ; -> loc_3_1326
        xor    al, al
        ret    
loc_3_1326:
        mov    eax, dword ptr [esi + 0x80]
        mov    byte ptr [eax + 4], 0x83
        xor    al, al
        ret    
loc_3_1333:
        mov    eax, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    byte ptr [eax + 4], 0xff
        mov    al, 1
        ret    
loc_3_1340:
        or     byte ptr [edi + 0x31], 2
        xor    al, al
        ret    
loc_3_1347:
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_138C
        mov    eax, dword ptr [esi + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, esi
        test   byte ptr [eax + 2], 2
        jne    loc_3_138C
        cmp    byte ptr [edi + 0x33], 1
        ja     loc_3_138C
        mov    edx, dword ptr [esi + 0x80]
        mov    ecx, dword ptr [edx + 8]
        call   sub_3_0796
        xchg   ch, cl
        ror    ecx, 0x10
        xchg   ch, cl
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [edx + 0xc]
        call   sub_3_0796
        xchg   ch, cl
        ror    ecx, 0x10
        xchg   ch, cl
        mov    dword ptr [edx + 0xc], ecx
loc_3_138C:
        xor    al, al
        ret    
loc_3_138F:
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [eax + 3], 8
        jne    loc_3_1414
        mov    edx, dword ptr [ebx + 0x10]
        mov    edx, dword ptr [edx + 8]
        movzx  ecx, word ptr [eax + 0xa]
        cmp    ecx, dword ptr [edx + 4]
        je     loc_3_13CB
        cmp    cx, 8
        je     loc_3_13B7
        cmp    cx, 0x3009
        jne    loc_3_1414
loc_3_13B7:
        mov    dword ptr [edx + 4], ecx
        cmp    byte ptr [eax + 0xc], 1
        jne    loc_3_13CB
        mov    byte ptr [edi + 3], 1
        mov    byte ptr [edi + 4], 0
        mov    al, 2
        ret    
loc_3_13CB:
        xor    ecx, ecx
        mov    byte ptr [edi + 0x30], 0x55
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebx + 0x7c]
        shl    ecx, 8
        sub    ch, 6
        sub    ch, 6
        shr    ecx, 8
        push   edi
        push   esi
        lea    edi, [eax + 8]
        lea    esi, [eax + 0xc]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    edi
        movzx  eax, byte ptr [eax + 8]
        mov    dword ptr [edi + 0x32], eax
loc_3_13F8:
        mov    eax, dword ptr [ebx + 0x7c]
        sub    al, 4
        mov    byte ptr [edi + 0x34], 0
        mov    dword ptr [edi + 0x38], eax
        mov    dword ptr [ebx + 0x7c], eax
        mov    dword ptr [edi + 0x10], eax
        xor    eax, eax
        mov    dword ptr [edi + 4], eax
        mov    byte ptr [edi + 0xa], 0xc
        ret    
loc_3_1414:
        jmp    loc_3_0A25
loc_3_1419:
        mov    eax, dword ptr [esi + 0x80]
        push   edi
        push   esi
        mov    ecx, dword ptr [esi + 0x7c]
        sub    cl, 8
        mov    edx, ecx
        sub    esp, ecx
        mov    edi, esp
        lea    esi, [eax + 8]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, edx
        mov    esi, esp
        lea    edi, [eax + 0xc]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    esp, edx
        pop    esi
        pop    edi
        movzx  ecx, byte ptr [eax + 1]
        add    ecx, 0x8000005
        mov    dword ptr [eax], ecx
        mov    edx, dword ptr [esi + 0x10]
        mov    edx, dword ptr [edx + 8]
        mov    ecx, 0x80000
        cmp    dword ptr [edx + 4], 0x3009
        jne    loc_3_1464
        mov    ecx, 0x30090000
loc_3_1464:
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 4], 0
        xor    al, al
        ret    
loc_3_1471:
        call   sub_3_1101
        jb     loc_3_1481
        test   dl, 4
        je     loc_3_1063
loc_3_1481:
        mov    edx, dword ptr [ebx + 0x10]
        mov    edx, dword ptr [edx + 8]
        cmp    dword ptr [edx + 4], 0x3009
        jne    loc_3_14A7
sub_3_1490:
        mov    word ptr [edi + 0x30], 0xbe
        mov    byte ptr [edi + 0x39], 0xf8
        mov    word ptr [edi + 0x3a], 0
        mov    byte ptr [edi + 0xa], 0xc
loc_3_14A4:
        xor    al, al
        ret    
loc_3_14A7:
        test   byte ptr [esi + 0x482], 0x40
        je     loc_3_14A4
        call   sub_3_1101
        jb     loc_3_14A4
        test   dl, 4
        je     loc_3_14A4
        call   sub_3_1490
        mov    byte ptr [edi + 0x39], 0x10
        jmp    loc_3_14A4
loc_3_14C7:
        cmp    byte ptr [edi + 3], 1
        je     loc_3_14F0
        test   byte ptr [edi + 3], 0x80
        je     loc_3_14F0
        mov    eax, dword ptr [edi + 0x1c]
        mov    cx, word ptr [eax + 0xc]
        cmp    cl, 0x21
        je     loc_3_14E6
        cmp    cx, 0x230
        jne    loc_3_14F0
loc_3_14E6:
        mov    word ptr [eax + 0xc], 0x64
        mov    byte ptr [eax + 2], 5
loc_3_14F0:
        xor    al, al
        ret    
loc_3_14F3:
        mov    byte ptr [edi + 0x31], 4
        mov    eax, dword ptr [edi + 0x36]
        shr    eax, 8
        mov    dword ptr [edi + 0x36], eax
        mov    byte ptr [edi + 0x39], 0xf0
        mov    word ptr [edi + 0x3a], 0
        xor    al, al
        ret    
        db  000h, 000h, 000h
loc_3_1510:
        xor    eax, eax
        cmp    word ptr [edi + 0x37], ax
        jne    loc_3_151E
        mov    byte ptr [edi + 3], 1
        mov    al, 2
loc_3_151E:
        ret    
loc_3_151F:
        mov    al, byte ptr [edi + 0x32]
        and    al, 0x3f
        cmp    al, 0xe
        jne    loc_3_15C9
        test   byte ptr [edi + 0x32], 0x40
        jne    loc_3_15F3
        call   sub_3_0588
        jb     loc_3_15CC
        sub    esp, 0x10
        mov    edx, esp
        mov    word ptr [edx + 4], 0x24
        mov    word ptr [edx + 0xa], 9
        mov    word ptr [edx], 3
        mov    byte ptr [edx + 0xc], 8
        push   edx
        call   sub_1_017C
        mov    eax, dword ptr [edx + 6]
        add    esp, 0x14
        or     eax, eax
        je     loc_3_15DD
        mov    dword ptr [ebx + 0x80], eax
        mov    dword ptr [ebx + 0x7c], 0x24
        and    dword ptr [ebx + 0x6c], 0xfffffffd
        or     dword ptr [ebx + 0x6c], 0x200
        mov    dword ptr [edi + 0xc], 0x248
        mov    byte ptr [edi + 0xa], 6
        mov    word ptr [edi + 0x30], 0x1a
        xor    eax, eax
        mov    dword ptr [edi + 0x32], eax
        mov    byte ptr [edi + 0x34], 0x24
        mov    byte ptr [ebx + 0xb0], al
        mov    dword ptr [ebx + 0x70], 0x52  ; -> loc_1_0052
        mov    eax, dword ptr [ebx + 0x10]
        movsx  eax, word ptr [eax + 0x10]
        add    eax, ebx
        mov    dword ptr [eax + 0x1f], 0x161d ; -> loc_3_161D
        push   ebx
        push   edi
        push   esi
        call   sub_3_06C2
        pop    esi
        pop    edi
        pop    ebx
        mov    al, 3
        ret    
loc_3_15C9:
        xor    al, al
        ret    
loc_3_15CC:
        mov    word ptr [ebx + 0x6a], 0x1d
        mov    byte ptr [edi + 3], 4
        mov    byte ptr [edi + 4], 8
        mov    al, 2
        ret    
loc_3_15DD:
        push   dword ptr [ebx + 0x90]
        mov    edi, ebx
        mov    ebx, dword ptr [edi + 8]
        call   sub_3_05C3
        pop    ebx
        mov    edi, dword ptr [ebx + 0x20]
        jmp    loc_3_15CC
loc_3_15F3:
        push   ebp
        push   edi
        push   esi
        lea    esi, [0xdfc]                  ; -> loc_3_0DFC
        lea    ecx, [0x1b]
        mov    edi, dword ptr [ebx + 0x80]
        mov    ebp, edi
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    edi
        mov    dword ptr [ebp + 0x14], 0xff0fff0f
        pop    ebp
        mov    byte ptr [edi + 3], 1
        mov    al, 2
        ret    
loc_3_161D:
        mov    ecx, dword ptr [esi + 0x90]
        mov    edx, dword ptr [ecx + 0x20]
        movzx  eax, word ptr [edi + 3]
        mov    word ptr [edx + 3], ax
        push   edi
        push   esi
        push   ebx
        push   ecx
        mov    ebx, esi
        mov    esi, dword ptr [edi + 0x1c]
        mov    edi, dword ptr [edx + 0x1c]
        mov    eax, dword ptr [ecx + 0x80]
        movzx  ecx, byte ptr [edx + 0xb]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, dword ptr [ebx + 0x80]
        mov    edi, eax
        mov    ecx, 0xc
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    esi, 8
        mov    ecx, 0x10
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        mov    byte ptr [eax], 0x1b
        jmp    loc_3_0BC1
loc_3_166B:
        cmp    byte ptr [edi + 0x39], 0x80
        je     loc_3_0A25
        xor    eax, eax
        cmp    byte ptr [edi + 0x39], al
        je     loc_3_16AA
        cmp    dword ptr [esi + 0x9d], 0x20353334
        je     loc_3_0A25
        mov    dword ptr [edi + 0x30], 0x26
        mov    dword ptr [edi + 0x34], eax
        mov    word ptr [edi + 0x38], ax
        mov    ecx, dword ptr [ebx + 0x10]
        movsx  ecx, word ptr [ecx + 0x10]
        add    ecx, ebx
        mov    dword ptr [ecx + 0x1f], 0x16ab ; -> loc_3_16AB
loc_3_16AA:
        ret    
loc_3_16AB:
        mov    ecx, dword ptr [esi + 0x80]
        mov    eax, dword ptr [ecx]
        mov    edx, 0x1010a00
        or     eax, eax
        je     loc_3_16C1
        mov    edx, 0x2010a00
loc_3_16C1:
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [esi + 0x10]
        movsx  edx, word ptr [edx + 0x10]
        add    edx, esi
        push   ecx
        test   byte ptr [edx + 2], 2
        je     loc_3_16E3
        mov    ecx, eax
        xchg   cl, ch
        ror    ecx, 0x10
        xchg   cl, ch
        call   sub_3_07CB
        mov    eax, ecx
loc_3_16E3:
        pop    edx
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 4], 0x100
        mov    byte ptr [edi + 3], 1
        xor    al, al
        ret    
loc_3_16F5:
        xor    eax, eax
        cmp    byte ptr [edi + 0x34], 2
        jne    loc_3_170B
        mov    byte ptr [edi + 0x30], 0xc0
        mov    byte ptr [edi + 0xa], 0xa
        mov    dword ptr [edi + 0x32], eax
        mov    dword ptr [edi + 0x36], eax
loc_3_170B:
        ret    
loc_3_170C:
        mov    ecx, dword ptr [esi + 0x80]
        movzx  eax, byte ptr [ecx + 6]
        shl    ax, 4
        shr    al, 4
        aad    0xa
        mov    byte ptr [ecx + 6], al
        movzx  eax, byte ptr [ecx + 7]
        shl    ax, 4
        shr    al, 4
        aad    0xa
        mov    byte ptr [ecx + 7], al
        mov    al, 0
        ret    
        db  000h, 000h, 000h
loc_3_1738:
        cmp    word ptr [ebx + 0xe], 0x3c
        ja     loc_3_174B
        mov    word ptr [ebx + 0xc], 0x3c
        mov    word ptr [ebx + 0xe], 0x3c
loc_3_174B:
        mov    al, 4
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x2c0  RW-
        db  00Ah, 000h, 023h, 000h, 006h, 000h, 000h, 000h
        dd  loc_3_10A3               ; fixup
        db  028h, 028h, 000h, 000h
        dd  loc_3_0FF3               ; fixup
        db  000h, 000h, 000h, 000h, 02Bh, 02Bh, 000h, 000h
        dd  loc_3_0830               ; fixup
        db  000h, 000h, 000h, 000h, 045h, 045h, 000h, 000h
        dd  sub_3_0877               ; fixup
        dd  sub_3_08CA               ; fixup
        db  047h, 047h, 000h, 000h
        dd  sub_3_0847               ; fixup
        dd  sub_3_08CA               ; fixup
        db  0A5h, 0A5h, 000h, 000h
        dd  sub_3_0883               ; fixup
        dd  sub_3_08CA               ; fixup
        db  043h, 043h, 000h, 000h
        dd  loc_3_094D               ; fixup
        dd  loc_3_0A54               ; fixup
        db  015h, 015h, 000h, 000h
        dd  loc_3_0CBC               ; fixup
        dd  loc_3_0DE6               ; fixup
        db  01Ah, 01Ah, 000h, 000h
        dd  loc_3_0E18               ; fixup
        db  000h, 000h, 000h, 000h, 042h, 042h, 000h, 000h
        dd  loc_3_0EF1               ; fixup
        dd  loc_3_0F1F               ; fixup
        db  01Bh, 01Bh, 000h, 000h
        dd  loc_3_10EA               ; fixup
        db  000h, 000h, 000h, 000h, 00Ah, 000h, 023h, 000h, 006h, 000h, 000h, 000h
        dd  loc_3_10A3               ; fixup
        db  028h, 028h, 000h, 000h
        dd  loc_3_0FF3               ; fixup
        db  000h, 000h, 000h, 000h, 02Bh, 02Bh, 000h, 000h
        dd  loc_3_0830               ; fixup
        db  000h, 000h, 000h, 000h, 045h, 045h, 000h, 000h
        dd  loc_3_0914               ; fixup
        dd  loc_3_0942               ; fixup
        db  047h, 047h, 000h, 000h
        dd  loc_3_0906               ; fixup
        dd  loc_3_0942               ; fixup
        db  0A5h, 0A5h, 000h, 000h
        dd  loc_3_090D               ; fixup
        dd  loc_3_0942               ; fixup
        db  043h, 043h, 000h, 000h
        dd  loc_3_094D               ; fixup
        dd  loc_3_0A54               ; fixup
        db  015h, 015h, 000h, 000h
        dd  loc_3_0CBC               ; fixup
        dd  loc_3_0DE6               ; fixup
        db  01Ah, 01Ah, 000h, 000h
        dd  loc_3_0E18               ; fixup
        db  000h, 000h, 000h, 000h, 042h, 042h, 000h, 000h
        dd  loc_3_0EF1               ; fixup
        dd  loc_3_0F1F               ; fixup
        db  01Bh, 01Bh, 000h, 000h
        dd  loc_3_10EA               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 023h, 000h, 004h, 000h, 000h, 000h
        dd  loc_3_053F               ; fixup
        db  005h, 000h, 023h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 028h, 028h, 000h, 000h
        dd  loc_3_1471               ; fixup
        dd  loc_3_14C7               ; fixup
        db  01Ah, 05Ah, 000h, 000h
        dd  loc_3_13F8               ; fixup
        dd  loc_3_1419               ; fixup
        db  015h, 015h, 000h, 000h
        dd  loc_3_138F               ; fixup
        dd  loc_3_1419               ; fixup
        db  042h, 042h, 000h, 000h
        dd  loc_3_1340               ; fixup
        dd  loc_3_1347               ; fixup
        db  0D8h, 0BEh, 000h, 000h
        dd  loc_3_14F3               ; fixup
        db  000h, 000h, 000h, 000h, 001h, 000h, 023h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  028h, 028h, 000h, 000h
        dd  loc_3_1738               ; fixup
        db  000h, 000h, 000h, 000h, 007h, 000h, 023h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  028h, 028h, 000h, 000h
        dd  loc_3_12E6               ; fixup
        db  000h, 000h, 000h, 000h, 043h, 043h, 000h, 000h
        dd  loc_3_1160               ; fixup
        db  000h, 000h, 000h, 000h, 015h, 015h, 000h, 000h
        dd  loc_3_12FB               ; fixup
        db  000h, 000h, 000h, 000h, 0D8h, 028h, 000h, 000h
        dd  loc_3_1223               ; fixup
        db  000h, 000h, 000h, 000h, 045h, 045h, 000h, 000h
        dd  loc_3_1333               ; fixup
        db  000h, 000h, 000h, 000h, 047h, 047h, 000h, 000h
        dd  loc_3_1333               ; fixup
        db  000h, 000h, 000h, 000h, 0A5h, 0A5h, 000h, 000h
        dd  loc_3_1333               ; fixup
        db  000h, 000h, 000h, 000h, 005h, 000h, 023h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  045h, 045h, 000h, 000h
        dd  loc_3_1510               ; fixup
        db  000h, 000h, 000h, 000h, 042h, 042h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_3_170C               ; fixup
        db  043h, 043h, 000h, 000h
        dd  loc_3_166B               ; fixup
        db  000h, 000h, 000h, 000h, 01Ah, 01Ah, 000h, 000h
        dd  loc_3_151F               ; fixup
        db  000h, 000h, 000h, 000h, 01Bh, 01Bh, 000h, 000h
        dd  loc_3_16F5               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 04Eh, 045h, 043h, 020h, 020h, 020h, 020h, 020h, 043h, 044h, 02Dh
        db  052h, 04Fh, 04Dh, 020h, 044h, 052h, 049h, 056h, 045h, 03Ah, 035h, 030h, 030h, 02Eh, 02Eh, 02Eh
        db  02Eh, 039h, 034h, 02Eh, 032h, 04Eh, 045h, 043h, 020h, 020h, 020h, 020h, 020h, 043h, 044h, 02Dh
        db  052h, 04Fh, 04Dh, 020h, 044h, 052h, 049h, 056h, 045h, 03Ah, 035h, 031h, 030h, 02Eh, 02Eh, 02Eh
        db  02Eh, 039h, 034h, 02Eh, 032h, 04Eh, 045h, 043h, 020h, 020h, 020h, 020h, 020h, 043h, 044h, 02Dh
        db  052h, 04Fh, 04Dh, 020h, 044h, 052h, 049h, 056h, 045h, 03Ah, 033h, 038h, 020h, 02Eh, 02Eh, 02Eh
        db  02Eh, 039h, 039h, 039h, 039h, 054h, 04Fh, 053h, 048h, 049h, 042h, 041h, 020h, 043h, 044h, 02Dh
        db  052h, 04Fh, 04Dh, 020h, 044h, 052h, 049h, 056h, 045h, 03Ah, 058h, 04Dh, 020h, 02Eh, 02Eh, 02Eh
        db  02Eh, 039h, 039h, 039h, 039h, 054h, 045h, 058h, 045h, 04Ch, 020h, 020h, 020h, 043h, 044h, 02Dh
        db  052h, 04Fh, 04Dh, 020h, 044h, 04Dh, 02Dh, 058h, 058h, 032h, 034h, 020h, 020h, 02Eh, 02Eh, 02Eh
        db  02Eh, 039h, 030h, 02Eh, 031h, 026h, 073h, 063h
VxD__LDATA ENDS
