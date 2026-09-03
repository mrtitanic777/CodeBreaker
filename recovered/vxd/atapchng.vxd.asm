; ATAPCHNG -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 955 instructions recovered by recursive descent

Declare_Virtual_Device ATAPCHNG, 4, 0, ATAPCHNG_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x5b0  R-X
ATAPCHNG_Control:
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
        je     0xffff0001                    ; -> obj3:0x0001
        cmp    si, 4
        je     loc_3_023A
        cmp    si, 2
        je     loc_3_025C
        mov    word ptr [ebx + 2], 0xffff
        ret    
        db  000h, 000h
loc_1_0040:
        push   ebp
        push   edi
        mov    ebx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        mov    eax, dword ptr [ebx + 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebx + 0x10], eax
        mov    edi, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebx + 0x20]
        mov    eax, dword ptr [ecx + 0x1c]
        cmp    byte ptr [ecx + 3], 1
        je     loc_1_0077
        test   byte ptr [ecx + 3], 0x80
        je     loc_1_0081
        cmp    byte ptr [eax + 2], 2
        je     loc_1_0081
        cmp    byte ptr [eax + 2], 6
        je     loc_1_0081
loc_1_0077:
        call   sub_1_00C0
        jmp    loc_1_010F
loc_1_0081:
        mov    dword ptr [edi + 0x10], 0xff
        jmp    loc_1_0077
loc_1_008A:
        push   ebp
        push   edi
        mov    ebx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        mov    edi, dword ptr [ebx + 0x14]
        push   dword ptr [edi + 4]
        sub    edi, 8
        mov    dword ptr [ebx + 0x14], edi
        push   dword ptr [ebx + 0x68]
        push   ebx
        call   dword ptr [edi]
        add    esp, 4
        pop    eax
        cmp    ax, 0x23
        pop    eax
        je     loc_1_00B8
        pop    esi
        pop    edi
        pop    ebp
        ret    
loc_1_00B8:
        mov    edi, dword ptr [eax + 8]
        jmp    loc_1_010F
sub_1_00C0:
        push   edi
        btr    dword ptr [edi + 4], 0
        mov    edi, dword ptr [ebx + 0x14]
        sub    edi, 8
        mov    dword ptr [ebx + 0x14], edi
        push   ebx
        call   dword ptr [edi]
        add    esp, 4
        pop    edi
        ret    
        db  000h
loc_1_00D8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [edx + 8]
        test   byte ptr [ebx + 0x6d], 0x80
        je     loc_1_04AF
        mov    ecx, dword ptr [ebx + 0x20]
        and    byte ptr [ecx + 0x31], 0x1f
        test   byte ptr [ebx + 0x6d], 8
        je     loc_1_010A
        cmp    byte ptr [ecx + 0x30], 3
        je     loc_1_04AF
loc_1_010A:
        call   sub_1_0510
loc_1_010F:
        cli    
        bts    dword ptr [edi + 4], 0
        jb     loc_1_01F5
        call   sub_1_04E8
        jb     loc_1_01FA
        mov    edx, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [edx + 8]
        sti    
        mov    esi, dword ptr [ebx + 8]
        movzx  eax, byte ptr [esi + 0x7d]
        mov    ecx, dword ptr [ebx + 0x20]
        cmp    byte ptr [ecx + 0x30], 0
        je     loc_1_02AA
        cmp    byte ptr [ecx + 0x30], 0x12
        je     loc_1_01A7
        cmp    dword ptr [edi + 0x10], eax
        je     loc_1_01A7
        cmp    byte ptr [ecx + 0x30], 0x42
        je     loc_1_0455
loc_1_0156:
        mov    edx, ebx
        mov    esi, 0x168                    ; -> loc_1_0168
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        jmp    loc_1_01F5
loc_1_0168:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 0x10]
        mov    ecx, 1
        call   sub_3_0447
        mov    dword ptr [edi + 0x30], 0xa6
        mov    dword ptr [edi + 0x34], 3
        movzx  eax, byte ptr [esi + 0x7d]
        mov    dword ptr [edi + 0x38], eax
        mov    dword ptr [ebx + 0x70], 0x201 ; -> loc_1_0201
        mov    edx, 0xd8                     ; -> loc_1_00D8
        call   sub_3_056C
        jmp    loc_1_01F5
loc_1_01A7:
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax], 0x40         ; -> loc_1_0040
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [edx + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 4], ecx
        add    dword ptr [ebx + 0x14], 8
        mov    ecx, dword ptr [ebx + 0x20]
        cmp    byte ptr [ecx + 0x30], 0
        je     loc_1_02AA
        cmp    byte ptr [ecx + 0x30], 0x55
        jne    loc_1_01D7
        mov    byte ptr [ecx + 0x37], 0
loc_1_01D7:
        cli    
        mov    esi, dword ptr [ebx + 4]
        cmp    byte ptr [esi + 0x76], 0
        jne    loc_1_0477
loc_1_01E5:
        sti    
        mov    edx, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [ebx + 0x10], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
loc_1_01F5:
        sti    
        pop    esi
        pop    edi
        pop    ebp
        ret    
loc_1_01FA:
        btr    dword ptr [edi + 4], 0
        jmp    loc_1_01F5
loc_1_0201:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [ebx + 0x20]
        push   esi
        mov    esi, dword ptr [ebx + 0x20]
        movzx  eax, byte ptr [esi + 3]
        mov    edx, dword ptr [ebx + 0x90]
        mov    edi, dword ptr [edx + 0x20]
        mov    byte ptr [edi + 3], al
        movzx  ecx, byte ptr [edi + 0xb]
        mov    esi, dword ptr [esi + 0x1c]
        mov    edi, dword ptr [edi + 0x1c]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        call   sub_1_04CD
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 0x20]
        mov    edx, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [edx + 8]
        cmp    byte ptr [eax + 3], 1
        je     loc_1_0284
        mov    eax, dword ptr [eax + 0x1c]
        cmp    byte ptr [eax + 0xc], 0x3a
        jne    loc_1_0274
loc_1_0252:
        movzx  ecx, byte ptr [esi + 0x7d]
        mov    byte ptr [ecx + edi + 0x18], 0x3a
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 0x20]
        mov    byte ptr [edi + 3], 0x84
        movzx  ecx, byte ptr [edi + 0xb]
        mov    edi, dword ptr [edi + 0x1c]
        mov    esi, 0x424                    ; -> loc_1_0424
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    edi
loc_1_0274:
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        call   sub_1_00C0
        jmp    loc_1_010F
loc_1_0284:
        movzx  ecx, byte ptr [esi + 0x7d]
        mov    byte ptr [ecx + edi + 0x18], 0
        mov    dword ptr [edi + 0x10], ecx
        mov    ecx, dword ptr [ebx + 0x20]
        cmp    byte ptr [ecx + 0x30], 0
        je     loc_1_03F4
        movzx  eax, word ptr [ebx + 0xe]
        mov    word ptr [ebx + 0xc], ax
        jmp    loc_1_01A7
loc_1_02AA:
        mov    edx, ebx
        mov    esi, 0x2bc                    ; -> loc_1_02BC
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        jmp    loc_1_01F5
loc_1_02BC:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 8]
        mov    edx, dword ptr [ebx + 0x10]
        mov    ecx, 1
        call   sub_3_0447
        mov    edx, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [edx + 8]
        mov    ecx, dword ptr [eax + 0x14]
        mov    dword ptr [ebx + 0x80], ecx
        mov    dword ptr [edi + 0x30], 0xbd
        mov    dword ptr [edi + 0x34], 0
        mov    ecx, dword ptr [eax + 0xc]
        lea    ecx, [ecx*4 + 8]
        mov    dword ptr [ebx + 0x70], 0x32f ; -> loc_1_032F
        mov    dword ptr [ebx + 0x7c], ecx
        mov    dword ptr [edi + 0x10], ecx
        ror    cx, 8
        mov    dword ptr [edi + 0x38], ecx
        push   ebx
        call   sub_1_0540
        add    esp, 4
        or     eax, eax
        je     loc_1_0320
        or     dword ptr [ebx + 0x6c], 0x40
loc_1_0320:
        mov    edx, 0xd8                     ; -> loc_1_00D8
        call   sub_3_056C
        jmp    loc_1_01F5
loc_1_032F:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        push   esi
        mov    esi, dword ptr [ebx + 0x20]
        movzx  eax, byte ptr [esi + 3]
        mov    edx, dword ptr [ebx + 0x90]
        mov    edi, dword ptr [edx + 0x20]
        mov    byte ptr [edi + 3], al
        movzx  ecx, byte ptr [edi + 0xb]
        mov    esi, dword ptr [esi + 0x1c]
        mov    edi, dword ptr [edi + 0x1c]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        call   sub_1_04CD
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [ebx + 0x20]
        mov    edx, dword ptr [ebx + 0x10]
        mov    edx, dword ptr [edx + 8]
        cmp    byte ptr [edi + 3], 1
        mov    edi, edx
        jne    loc_1_0436
        mov    ebp, dword ptr [edx + 0x14]
        mov    ecx, dword ptr [edx + 0xc]
loc_1_0380:
        test   dword ptr [ebp + ecx*4 + 4], 1
        je     loc_1_0396
        mov    byte ptr [ecx + edx + 0x17], 0xff
        mov    dword ptr [edx + 0x10], 0xff
loc_1_0396:
        loop   loc_1_0380
        movzx  ecx, byte ptr [esi + 0x7d]
        mov    ecx, dword ptr [ebp + ecx*4 + 8]
        test   byte ptr [edx + 4], 4
        je     loc_1_03C9
        test   ecx, 0x80
        je     loc_1_0252
        movzx  eax, byte ptr [esi + 0x7d]
        cmp    byte ptr [eax + edx + 0x18], 0x3a
        jne    loc_1_03C9
        test   ecx, 1
        je     loc_1_0156
loc_1_03C9:
        test   ecx, 1
        jne    loc_1_0156
        movzx  ecx, byte ptr [esi + 0x7d]
        cmp    byte ptr [ecx + edx + 0x18], 0xff
        je     loc_1_0156
        cmp    byte ptr [ecx + edx + 0x18], 0x3a
        je     loc_1_0252
        jmp    loc_1_0274
loc_1_03F4:
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 0x20]
        mov    byte ptr [edi + 3], 0x84
        movzx  ecx, byte ptr [edi + 0xb]
        mov    edi, dword ptr [edi + 0x1c]
        mov    esi, 0x412                    ; -> loc_1_0412
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    edi
        jmp    loc_1_0274
loc_1_0412:
        jo     loc_1_0414
loc_1_0414:
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0424:
        jo     loc_1_0426
loc_1_0426:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0436:
        mov    eax, dword ptr [ebx + 0x20]
        mov    eax, dword ptr [eax + 0x1c]
        cmp    byte ptr [eax + 2], 6
        jne    loc_1_0274
        cmp    byte ptr [eax + 0xc], 0x28
        jne    loc_1_0274
        jmp    loc_1_02AA
loc_1_0455:
        push   esi
        push   edi
        push   ecx
        mov    edi, dword ptr [ebx + 0x80]
        mov    ecx, 4
        lea    esi, [0x49f]                  ; -> obj1:0x049F
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    ecx
        pop    edi
        pop    esi
        mov    byte ptr [ecx + 3], 1
        jmp    loc_1_0274
loc_1_0477:
        test   byte ptr [ebx + 0x6d], 8
        jne    loc_1_01E5
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        mov    edi, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [edi + 8]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 0x64], eax
        mov    dword ptr [edi], ebx
        btr    dword ptr [edi + 4], 0
        sti    
        jmp    loc_1_010F
        db  000h, 015h, 000h, 00Ch, 001h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_04AF:
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax], 0x8a         ; -> loc_1_008A
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [edx + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 4], ecx
        add    dword ptr [ebx + 0x14], 8
        jmp    loc_1_01D7
sub_1_04CD:
        sub    ebx, dword ptr [esi + 4]
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], ebx
        mov    word ptr [eax], 4
        push   eax
        call   sub_1_052C
        add    esp, 0xc
        ret    
sub_1_04E8:
        mov    ebx, dword ptr [edi]
        lea    eax, [edi - 0x64]
        mov    esi, dword ptr [edi + 8]
loc_1_04F0:
        or     ebx, ebx
        stc    
        je     loc_1_050F
        cmp    byte ptr [esi + 0x76], 0
        je     loc_1_0508
        test   byte ptr [ebx + 0x6d], 8
        jne    loc_1_0508
        mov    eax, ebx
        mov    ebx, dword ptr [ebx + 0x64]
        jmp    loc_1_04F0
loc_1_0508:
        mov    ecx, dword ptr [ebx + 0x64]
        mov    dword ptr [eax + 0x64], ecx
        clc    
loc_1_050F:
        ret    
sub_1_0510:
        cli    
        lea    eax, [edi - 0x64]
loc_1_0514:
        cmp    dword ptr [eax + 0x64], 0
        je     loc_1_051F
        mov    eax, dword ptr [eax + 0x64]
        jmp    loc_1_0514
loc_1_051F:
        mov    dword ptr [eax + 0x64], ebx
        mov    dword ptr [ebx + 0x64], 0
        sti    
        ret    
        db  000h
sub_1_052C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0538:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0540:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
        db  'ATAPCHNG'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  ATAPCHNG_Control         ; DDB_Control_Proc
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
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        or     al, 0
        add    byte ptr [eax], al
        sub    al, 5
        add    byte ptr [eax], al
        inc    ecx
        push   esp
        inc    ecx
        push   eax
        dec    ecx
        and    byte ptr [ebx + 0x44], al
        and    byte ptr [ebx + 0x48], al
        inc    ecx
        dec    esi
        inc    edi
        inc    ebp
        push   edx
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

VxD__LTEXT SEGMENT   ; object 3  vsize=0x5fc  R-X
loc_3_0000:
        add    eax, 0xa205b0                 ; -> loc_3_0000
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x56], dl
        push   ebx
        mov    esi, dword ptr [ebx + 0xc]
        test   dword ptr [esi + 0x20], 0x6000
        jne    loc_3_01F7
        cmp    dword ptr [esi + 0xa6], 0x61656469
        jne    loc_3_0033
        cmp    dword ptr [esi + 0xaa], 0x69706174
        je     loc_3_003D
loc_3_0033:
        test   byte ptr [esi + 0x24], 2
        je     loc_3_01F7
loc_3_003D:
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_01F7
        cmp    byte ptr [esi + 0x40], 5
        jne    loc_3_01F7
        cmp    byte ptr [esi + 0x7d], 0
        jne    loc_3_01F7
        xor    ecx, ecx
        call   sub_3_0447
        or     byte ptr [ebx + 0x6d], 1
        mov    dword ptr [edi + 0x30], 0x2a005a
        mov    dword ptr [edi + 0x34], 0
        mov    ecx, 0x1c
        mov    dword ptr [edi + 0x38], ecx
        mov    dword ptr [edi + 0xc], 0x248
        call   sub_3_05A6
        jb     loc_3_0202
loc_3_008E:
        xor    edx, edx
        mov    dword ptr [ebx + 0x10], 0
        call   sub_3_056C
        mov    edx, dword ptr [ebx + 0x90]
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_0209
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [eax + 8], 0x2a
        jne    loc_3_01FB
        movzx  ecx, byte ptr [eax + 0xe]
        mov    ch, cl
        and    cl, 0xc0
        cmp    cl, 0x80
        jne    loc_3_01FB
        and    ch, 0x20
        mov    cl, byte ptr [eax + 0xf]
        or     cl, ch
        mov    dword ptr [edi + 0x30], 0xbd
        mov    word ptr [edi + 0x38], 0x800
        mov    dword ptr [ebx + 0x7c], 8
        mov    dword ptr [ebx + 0x10], 0
        mov    dword ptr [edi + 0x10], 8
        push   ebx
        call   sub_1_0540
        add    esp, 4
        xor    edx, edx
        call   sub_3_056C
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_01FB
        mov    edx, dword ptr [ebx + 0x80]
        mov    ch, byte ptr [edx + 5]
        call   sub_3_0217
        inc    byte ptr [0]                  ; -> obj4:0x0000
        mov    eax, dword ptr [esi + 0x1c]
        mov    ebx, dword ptr [esp]
        sub    esp, 0x18
        mov    edi, esp
        mov    word ptr [edi], 5
        mov    dword ptr [edi + 0x12], eax
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [edi + 8], 0xd8     ; -> loc_1_00D8
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [edi + 0x16], al
        push   ecx
        movzx  ecx, ch
        lea    eax, [ecx + ecx*4 + 0x21]
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        movzx  ecx, byte ptr [esp + 1]
        or     eax, eax
        lea    ebx, [ecx + eax + 0x19]
        mov    dword ptr [eax + 0x14], ebx
        mov    dword ptr [eax + 0x10], 0xff
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [eax + 8], esi
        pop    ecx
        movzx  edx, cl
        and    edx, 4
        movzx  ecx, ch
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [edi + 0x10], 0
        push   ecx
        push   edi
        lea    edi, [eax + 0x18]
        mov    al, 0xff
        rep stosb byte ptr es:[edi], al
        call   sub_1_052C
        pop    ecx
        pop    ecx
        add    esp, 0x18
        mov    eax, ecx
        dec    ecx
loc_3_01A7:
        push   eax
        sub    esp, 0xc
        mov    ebx, esp
        mov    word ptr [ebx], 1
        mov    word ptr [ebx + 4], 0x4a8
        push   ebx
        call   sub_1_052C
        mov    ebx, dword ptr [ebx + 6]
        add    esp, 0x10
        push   ecx
        mov    ecx, 0x12a
        cli    
        push   esi
        push   dword ptr [ebx + 0xc]
        push   dword ptr [ebx + 0x70]
        push   edi
        mov    edi, ebx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    dword ptr [ebx + 0x70]
        pop    dword ptr [ebx + 0xc]
        pop    esi
        mov    dword ptr [esi + 0x6c], ebx
        sti    
        pop    ecx
        mov    eax, dword ptr [esp]
        sub    eax, ecx
        mov    byte ptr [ebx + 0x7d], al
        mov    dword ptr [ebx], ebx
        call   0x28c
        pop    eax
        loop   loc_3_01A7
loc_3_01F7:
        pop    ebx
        pop    esi
        pop    ebp
        ret    
loc_3_01FB:
        call   sub_3_0217
        jmp    loc_3_01F7
loc_3_0202:
        call   sub_3_0234
        jmp    loc_3_01F7
loc_3_0209:
        dec    byte ptr [0]                  ; -> loc_3_0000
        jne    loc_3_008E
        jmp    loc_3_01FB
sub_3_0217:
        mov    edx, dword ptr [ebx + 0x80]
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], edx
        mov    word ptr [eax], 4
        push   eax
        call   sub_1_052C
        add    esp, 0xc
sub_3_0234:
        call   sub_1_04CD
        ret    
loc_3_023A:
        mov    esi, dword ptr [ebx + 0xc]
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_025B
        cmp    byte ptr [esi + 0x40], 5
        jne    loc_3_025B
        mov    al, byte ptr [esi + 0x84]
        and    al, 7
        cmp    al, 2
        jae    loc_3_025B
        dec    byte ptr [0]                  ; -> obj4:0x0000
loc_3_025B:
        ret    
loc_3_025C:
        cmp    byte ptr [0], 0               ; -> obj4:0x0000
        jne    loc_3_026B
        mov    word ptr [ebx + 2], 0xffff
loc_3_026B:
        ret    
loc_3_026C:
        push   ebx
        inc    ebx
        push   ebx
        dec    ecx
        pop    esp
        db  000h
loc_3_0272:
        push   ebp
        jae    0x2da
        jb     0x2bb
        jb     loc_3_02E2
        jbe    loc_3_02E0
        dec    esp
        je     loc_3_02F3
        jb     0x2c3
        jae    0x2f7
        imul   esp, dword ptr [edi + 0x6e], 0x746e656d
        add    byte ptr [ebx - 0x729ff314], al
        inc    ebx
        jo     0x2fe
        add    bh, bh
        jbe    loc_3_0308
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003003300
        add    esp, 0xc
        lea    eax, [esp + 0x28]
        push   0
        push   dword ptr [ebx + 0x70]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID_Size ; cd2006003300
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_0353
        mov    eax, dword ptr [esp + 0x28]
        add    eax, 0x22
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_3_0353
        push   eax
        lea    esi, [0x26c]                  ; -> loc_3_026C
        mov    ecx, 5
loc_3_02E0:
        mov    edi, eax
loc_3_02E2:
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    esi, [ebx + 0x8a]
        mov    ecx, 0x19
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    al, 0x5c
loc_3_02F3:
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [esp + 0x2c]
        push   0
        push   eax
        push   edi
        push   dword ptr [ebx + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007003300
        add    esp, 0x10
loc_3_0308:
        pop    esi
        xor    ecx, ecx
        or     eax, eax
        jne    loc_3_0353
        lea    edi, [esi + 0x1f]
        mov    al, 0x5c
        mov    ecx, dword ptr [esp + 0x28]
        mov    edx, ecx
loc_3_031A:
        repne scasb al, byte ptr es:[edi]
        jecxz  loc_3_0324
        mov    byte ptr [edi - 1], 0x26
        jmp    loc_3_031A
loc_3_0324:
        lea    eax, [esi + 0x1f]
        add    eax, edx
        mov    cx, word ptr [ebx + 0x7c]
        add    cx, 0x3030
        mov    word ptr [eax], cx
        lea    edi, [esp + 0x24]
        push   0
        push   dword ptr [ebx + 0x70]
        push   esi
        push   edi
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b003300
        add    esp, 0x10
        xor    ecx, ecx
        or     eax, eax
        jne    loc_3_0353
        mov    ecx, dword ptr [esp + 0x24]
loc_3_0353:
        mov    dword ptr [ebx + 0x70], ecx
        mov    word ptr [ebx + 0x48], 0xffff
        lea    ecx, [esp + 0x20]
        sub    esp, 0x18
        mov    eax, esp
        mov    word ptr [eax], 0x11
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 0xc], 1
        mov    dword ptr [eax + 0x10], 3
        mov    dword ptr [eax + 0x14], ecx
        mov    dword ptr [eax + 8], 0x272    ; -> loc_3_0272
        push   eax
        call   sub_1_052C
        movzx  eax, word ptr [eax + 2]
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_03A4
        mov    ax, word ptr [esp + 0x20]
        sub    ax, 0x4141
        mov    word ptr [ebx + 0x48], ax
loc_3_03A4:
        popal  
        call   0x3d3
        jae    loc_3_03B8
        call   sub_3_0412
        mov    esi, ebx
        call   sub_3_0412
loc_3_03B8:
        add    esp, 0xc
        ret    
loc_3_03BC:
        inc    ecx
        jne    loc_3_0433
        outsd  dx, dword ptr [esi]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jae    loc_3_0429
        jb     0x43a
        dec    esi
        outsd  dx, dword ptr [esi]
        je     loc_3_0433
        imul   sp, word ptr [ebx + 0x61], 0x6974
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [ecx - 0x7d], dl
        in     al, dx
        sbb    al, 0x8b
        int3   
        mov    word ptr [ecx], 0x11
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 0xc], 3
        mov    dword ptr [ecx + 0x10], 1
        lea    edx, [ecx + 0x18]
        mov    dword ptr [ecx + 0x14], edx
        mov    dword ptr [ecx + 8], 0x3bc    ; -> loc_3_03BC
        push   ecx
        call   sub_1_052C
        movzx  ecx, word ptr [ecx + 2]
        movzx  edx, byte ptr [edx]
        add    esp, 0x20
        jecxz  loc_3_0410
        stc    
loc_3_0410:
        pop    ecx
        ret    
sub_3_0412:
        enter  0xc, 0
        pushal 
        mov    dword ptr [ebp - 0xc], 1
        mov    edx, dword ptr [esi + 0x70]
        lea    eax, [ebp - 8]
        mov    byte ptr [eax], 1
        push   0
loc_3_0429:
        push   1
        push   eax
        push   3
        push   0x3bc                         ; -> loc_3_03BC
loc_3_0433:
        push   0
        push   edx
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        popal  
        leave  
        ret    
        db  000h, 000h
loc_3_0444:
        push   ecx
        jmp    loc_3_044D
sub_3_0447:
        push   ebx
        jecxz  loc_3_0444
        push   dword ptr [ebx + 0x10]
loc_3_044D:
        push   ecx
        call   sub_3_0482
        mov    eax, ecx
        pop    ecx
        or     edi, edi
        je     loc_3_047E
        mov    dword ptr [edi + 0xa4], edi
        pop    dword ptr [edi + 0x10]
        mov    word ptr [edi + 0x68], 0x12
        mov    dword ptr [edi + 8], esi
        mov    dword ptr [edi + 4], esi
        mov    ebx, edi
        call   sub_3_0506
        pop    dword ptr [ebx + 0x90]
        clc    
        ret    
loc_3_047E:
        pop    ebx
        pop    ebx
        stc    
        ret    
sub_3_0482:
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
        mov    byte ptr [edi + 0xe], 2
        mov    word ptr [edi], 2
        push   edi
        call   sub_1_052C
        mov    edi, dword ptr [edi + 0xa]
        add    esp, 0x14
        mov    eax, edi
        or     edi, edi
        je     loc_3_0504
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
loc_3_0504:
        pop    esi
        ret    
sub_3_0506:
        push   edx
        mov    edi, eax
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
        mov    dword ptr [edi + 0xc], 0
        mov    dword ptr [edi + 0x10], eax
        mov    byte ptr [edi + 0xa], 0xc
        mov    al, byte ptr [esi + 0x75]
        mov    byte ptr [edi + 5], al
        mov    al, byte ptr [esi + 0x7c]
        mov    byte ptr [edi + 6], al
        mov    edx, edi
        add    edx, 0x50
        mov    dword ptr [edi + 0x28], edx
        movzx  eax, word ptr [esi + 0x80]
        add    edx, eax
        mov    dword ptr [edi + 0x1c], edx
        mov    byte ptr [edi + 0xb], 0xe
        pop    edx
        ret    
sub_3_056C:
        push   esi
        push   edi
        push   ebx
        mov    ecx, dword ptr [ebx + 0x10]
        jecxz  loc_3_057D
        mov    esi, dword ptr [ecx + 8]
        mov    esi, dword ptr [esi + 8]
        mov    dword ptr [ebx + 4], esi
loc_3_057D:
        mov    byte ptr [edi + 3], 0
        mov    eax, dword ptr [ebx + 0x80]
        mov    dword ptr [edi + 0x18], eax
        mov    cx, word ptr [ebx + 0xe]
        mov    word ptr [ebx + 0xc], cx
        xchg   esi, edi
        xchg   edi, ebx
        mov    dword ptr [edi + 0xa4], edi
        call   sub_1_0538
        pop    ebx
        pop    edi
        pop    esi
        ret    
sub_3_05A6:
        sub    esp, 0x10
        mov    edx, esp
        mov    word ptr [edx + 4], cx
        mov    word ptr [edx + 0xa], 9
        mov    word ptr [edx], 3
        mov    byte ptr [edx + 0xc], 8
        push   edx
        call   sub_1_052C
        mov    eax, dword ptr [edx + 6]
        add    esp, 0x14
        or     eax, eax
        je     loc_3_05F8
        mov    dword ptr [ebx + 0x80], eax
        mov    dword ptr [ebx + 0x7c], ecx
        mov    dword ptr [edi + 0x10], ecx
        and    byte ptr [ebx + 0x6c], 0xfd
        or     byte ptr [ebx + 0x6d], 2
        push   eax
        push   ebx
        call   sub_1_0540
        add    esp, 4
        or     al, al
        pop    eax
        je     loc_3_05F7
        or     dword ptr [ebx + 0x6c], 0x40
loc_3_05F7:
        ret    
loc_3_05F8:
        stc    
        ret    
        db  026h, 041h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x4  RW-
VxD__LDATA ENDS
