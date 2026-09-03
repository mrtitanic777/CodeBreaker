; TORISAN3 -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 846 instructions recovered by recursive descent

Declare_Virtual_Device TORISAN3, 4, 0, TORISAN3_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x4ec  R-X
TORISAN3_Control:
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
        je     loc_3_0032
        cmp    si, 4
        je     loc_3_01B1
        cmp    si, 2
        je     loc_3_01D3
        mov    word ptr [ebx + 2], 0xffff
        ret    
        db  000h, 000h
loc_1_0040:
        push   ebp
        mov    ebx, dword ptr [esp + 8]
        push   edi
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
        je     loc_1_00C8
        cmp    byte ptr [eax + 2], 2
        je     loc_1_00C8
        cmp    byte ptr [eax + 2], 6
        je     loc_1_00C8
loc_1_0077:
        cmp    byte ptr [ecx + 0x30], 0
        jne    loc_1_00AF
        movzx  edx, byte ptr [esi + 0x7d]
        movzx  edx, byte ptr [edx + edi + 0x10]
        or     edx, edx
        je     loc_1_00BF
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 0x20]
        mov    byte ptr [edi + 3], 0x84
        movzx  ecx, byte ptr [edi + 0xb]
        mov    edi, dword ptr [edi + 0x1c]
        mov    esi, 0xd8                     ; -> loc_1_00D8
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    edi
        mov    byte ptr [eax + 0xc], dl
        cmp    dl, 0x28
        je     loc_1_00AF
        mov    byte ptr [eax + 2], 2
loc_1_00AF:
        call   sub_1_0120
        jmp    loc_1_016F
        db  0C6h, 041h, 030h, 02Bh, 0EBh, 09Dh
loc_1_00BF:
        mov    eax, dword ptr [ebx + 0x20]
        mov    byte ptr [eax + 3], 1
        jmp    loc_1_00AF
loc_1_00C8:
        mov    dword ptr [edi + 0xc], 0xff
        mov    dword ptr [edi + 0x10], 0x28282828
        jmp    loc_1_00AF
loc_1_00D8:
        jo     loc_1_00DA
loc_1_00DA:
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_00EA:
        push   ebp
        mov    ebx, dword ptr [esp + 8]
        push   edi
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
        je     loc_1_0118
        pop    esi
        pop    edi
        pop    ebp
        ret    
loc_1_0118:
        mov    edi, dword ptr [eax + 8]
        jmp    loc_1_016F
sub_1_0120:
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
loc_1_0138:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [edx + 8]
        test   byte ptr [ebx + 0x6d], 0x80
        je     loc_1_03E1
        mov    ecx, dword ptr [ebx + 0x20]
        and    byte ptr [ecx + 0x31], 0x1f
        test   byte ptr [ebx + 0x6d], 8
        je     loc_1_016A
        cmp    byte ptr [ecx + 0x30], 3
        je     loc_1_03E1
loc_1_016A:
        call   sub_1_0430
loc_1_016F:
        cli    
        bts    dword ptr [edi + 4], 0
        jb     loc_1_0274
        call   sub_1_0408
        jb     loc_1_0279
        mov    edx, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [edx + 8]
        sti    
        mov    esi, dword ptr [ebx + 8]
        mov    ecx, dword ptr [ebx + 0x20]
        cmp    byte ptr [ecx + 0x30], 0x12
        je     loc_1_021D
        movzx  eax, byte ptr [esi + 0x7d]
        cmp    dword ptr [edi + 0xc], eax
        je     loc_1_021D
        cmp    dword ptr [edi + 0xc], 0xff
        je     loc_1_01C6
        cmp    byte ptr [ecx + 0x30], 0x42
        je     loc_1_0387
        cmp    byte ptr [ecx + 0x30], 0
        jne    loc_1_01C6
        cmp    byte ptr [eax + edi + 0x10], 0
        je     loc_1_021D
loc_1_01C6:
        or     byte ptr [edi + 4], 2
        mov    edx, ebx
        mov    esi, 0x1dc                    ; -> loc_1_01DC
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        jmp    loc_1_0274
loc_1_01DC:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 0x10]
        call   sub_3_03B4
        mov    dword ptr [edi + 0x30], 0
        mov    dword ptr [edi + 0x38], 0
        movzx  eax, byte ptr [esi + 0x7d]
        cmp    eax, 0xff
        ror    eax, 8
        mov    dword ptr [edi + 0x34], eax
        mov    byte ptr [edi + 0xa], 0xc
        mov    dword ptr [ebx + 0x70], 0x280 ; -> loc_1_0280
        call   sub_3_04D8
        jmp    loc_1_0274
loc_1_021D:
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax], 0x40         ; -> loc_1_0040
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [edx + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 4], ecx
        add    dword ptr [ebx + 0x14], 8
        mov    ecx, dword ptr [ebx + 0x20]
        cmp    byte ptr [ecx + 0x30], 0
        je     loc_1_032D
        cmp    byte ptr [ecx + 0x30], 0x55
        jne    loc_1_024D
        mov    byte ptr [ecx + 0x37], 0
loc_1_024D:
        cli    
        mov    esi, dword ptr [ebx + 4]
        cmp    byte ptr [esi + 0x76], 0
        jne    loc_1_03A9
loc_1_025B:
        sti    
        mov    edx, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [edx + 8]
        mov    eax, dword ptr [eax + 8]
        cmp    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [ebx + 0x10], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
loc_1_0274:
        sti    
        pop    esi
        pop    edi
        pop    ebp
        ret    
loc_1_0279:
        btr    dword ptr [edi + 4], 0
        jmp    loc_1_0274
loc_1_0280:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 4]
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
        call   sub_1_044B
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx + 0x20]
        mov    edx, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [edx + 8]
        cmp    byte ptr [eax + 3], 1
        je     loc_1_030B
        mov    eax, dword ptr [eax + 0x1c]
        cmp    byte ptr [eax + 0xc], 0x3a
        je     loc_1_02F2
        btr    dword ptr [edi + 4], 1
        jae    loc_1_02DE
        cmp    byte ptr [eax + 0xc], 0x28
        je     loc_1_034A
loc_1_02DE:
        mov    edx, ebx
        mov    esi, 0x1dc                    ; -> loc_1_01DC
        mov    eax, 0x3e8
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        jmp    loc_1_0274
loc_1_02F2:
        movzx  ecx, byte ptr [esi + 0x7d]
        mov    byte ptr [ecx + edi + 0x10], 0x3a
loc_1_02FB:
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        call   sub_1_0120
        jmp    loc_1_016F
loc_1_030B:
        movzx  ecx, byte ptr [esi + 0x7d]
        mov    byte ptr [ecx + edi + 0x10], 0
        mov    dword ptr [edi + 0xc], ecx
        mov    ecx, dword ptr [ebx + 0x20]
        cmp    byte ptr [ecx + 0x30], 0
        je     loc_1_035A
        movzx  eax, word ptr [ebx + 0xe]
        mov    word ptr [ebx + 0xc], ax
        jmp    loc_1_021D
loc_1_032D:
        mov    ecx, dword ptr [edi + 0xc]
        mov    eax, dword ptr [ebx + 0x20]
        shl    ecx, 8
        mov    word ptr [eax + 0x36], cx
        mov    dword ptr [eax + 0x38], 0
        mov    byte ptr [eax + 0xa], 0xc
        jmp    loc_1_024D
loc_1_034A:
        mov    dword ptr [edi + 0xc], 0xff
        mov    dword ptr [edi + 0x10], 0x28282828
        jmp    loc_1_02FB
loc_1_035A:
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 0x20]
        mov    byte ptr [edi + 3], 0x84
        movzx  ecx, byte ptr [edi + 0xb]
        mov    edi, dword ptr [edi + 0x1c]
        mov    esi, 0x375                    ; -> loc_1_0375
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    edi
        jmp    loc_1_02FB
loc_1_0375:
        jo     loc_1_0377
loc_1_0377:
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0387:
        push   esi
        push   edi
        push   ecx
        mov    edi, dword ptr [ebx + 0x80]
        mov    ecx, 4
        lea    esi, [0x3d1]                  ; -> obj1:0x03D1
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    ecx
        pop    edi
        pop    esi
        mov    byte ptr [ecx + 3], 1
        jmp    loc_1_02FB
loc_1_03A9:
        test   byte ptr [ebx + 0x6d], 8
        jne    loc_1_025B
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        mov    edi, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [edi + 8]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 0x64], eax
        mov    dword ptr [edi], ebx
        btr    dword ptr [edi + 4], 0
        sti    
        jmp    loc_1_016F
        db  000h, 015h, 000h, 00Ch, 001h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_03E1:
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax], 0xea         ; -> loc_1_00EA
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [edx + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 4], ecx
        add    dword ptr [ebx + 0x14], 8
        jmp    loc_1_024D
        db  0C6h, 041h, 030h, 0BEh, 0E9h, 045h, 0FEh, 0FFh, 0FFh
sub_1_0408:
        mov    ebx, dword ptr [edi]
        lea    eax, [edi - 0x64]
        mov    esi, dword ptr [edi + 8]
loc_1_0410:
        or     ebx, ebx
        stc    
        je     loc_1_042F
        cmp    byte ptr [esi + 0x76], 0
        je     loc_1_0428
        test   byte ptr [ebx + 0x6d], 8
        jne    loc_1_0428
        mov    eax, ebx
        mov    ebx, dword ptr [ebx + 0x64]
        jmp    loc_1_0410
loc_1_0428:
        mov    ecx, dword ptr [ebx + 0x64]
        mov    dword ptr [eax + 0x64], ecx
        clc    
loc_1_042F:
        ret    
sub_1_0430:
        cli    
        lea    eax, [edi - 0x64]
loc_1_0434:
        cmp    dword ptr [eax + 0x64], 0
        je     loc_1_043F
        mov    eax, dword ptr [eax + 0x64]
        jmp    loc_1_0434
loc_1_043F:
        mov    dword ptr [eax + 0x64], ebx
        mov    dword ptr [ebx + 0x64], 0
        sti    
        ret    
sub_1_044B:
        sub    ebx, dword ptr [esi + 4]
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], ebx
        mov    word ptr [eax], 4
        push   eax
        call   sub_1_0468
        add    esp, 0xc
        ret    
        db  000h, 000h
sub_1_0468:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0474:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
        db  'TORISAN3'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  TORISAN3_Control         ; DDB_Control_Proc
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
        cmp    word ptr [0x50], 1            ; -> obj2:0x0050
        je     loc_2_0023
        cmp    word ptr [0x50], 2            ; -> obj2:0x0050
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
        push   0x54000004
        dec    edi
        push   edx
        imul   edx, dword ptr [ebx + 0x4f], 0x4333204e
        inc    esp
        and    byte ptr [edx + edx*2 + 0x56], al
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
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x506  R-X
loc_3_0000:
        push   esp
        dec    edi
        push   edx
        imul   edx, dword ptr [ebx + 0x41], 0x204e
loc_3_000A:
        push   cs
        add    byte ptr [eax], al
        db  000h
loc_3_000E:
        add    al, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    ebx
        inc    esp
        push   edx
        sub    eax, 0x143343
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        inc    ebx
        inc    esp
        push   edx
        pop    edi
        inc    ebx
        xor    edx, dword ptr [eax + eax]
        add    byte ptr [eax], al
loc_3_0032:
        push   ebp
        push   esi
        mov    esi, dword ptr [ebx + 0xc]
        test   dword ptr [esi + 0x20], 0x6000
        jne    loc_3_01AE
        test   byte ptr [esi + 0x24], 2
        je     loc_3_01AE
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_01AE
        cmp    byte ptr [esi + 0x40], 5
        jne    loc_3_01AE
        cmp    byte ptr [esi + 0x7d], 0
        jne    loc_3_01AE
        push   esi
        mov    edi, 0                        ; -> loc_3_0000
        mov    ecx, 1
        lea    esi, [esi + 0x8a]
loc_3_007D:
        push   esi
        push   edi
        push   ecx
        mov    cl, 8
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    ecx
        pop    edi
        pop    esi
        je     loc_3_0094
        lea    edi, [edi + 0xa]
        loop   loc_3_007D
loc_3_008E:
        pop    esi
        jmp    loc_3_01AE
loc_3_0094:
        movzx  ecx, word ptr [edi + 8]
        mov    esi, dword ptr [esp]
        mov    edi, dword ptr [ecx*4 + 0xa]  ; -> loc_3_000A
        movzx  ecx, word ptr [edi]
        lea    edi, [edi + 2]
        lea    esi, [esi + 0x92]
loc_3_00AE:
        push   ecx
        push   esi
        push   edi
        mov    ecx, 0xd
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    edi
        pop    esi
        pop    ecx
        je     loc_3_00C4
        lea    edi, [edi + 0x11]
        loop   loc_3_00AE
        jmp    loc_3_008E
loc_3_00C4:
        mov    ebp, dword ptr [edi + 0xd]
        pop    esi
        inc    byte ptr [0]                  ; -> obj4:0x0000
        mov    eax, dword ptr [esi + 0x1c]
        sub    esp, 0x18
        mov    edi, esp
        mov    word ptr [edi], 5
        mov    dword ptr [edi + 0x12], eax
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [edi + 8], 0x138    ; -> loc_1_0138
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [edi + 0x16], al
        push   1
        push   ebp
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0xc], 0xff
        mov    dword ptr [eax + 0x10], 0x28282828
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [edi + 0x10], 0
        push   edi
        call   sub_1_0468
        add    esp, 0x1c
        sub    esp, 0xc
        mov    ebx, esp
        mov    word ptr [ebx], 1
        mov    word ptr [ebx + 4], 0x4a8
        push   ebx
        call   sub_1_0468
        mov    ebx, dword ptr [ebx + 6]
        add    esp, 0x10
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
        mov    esi, dword ptr [esp]
        mov    dword ptr [esi + 0x6c], ebx
        sti    
        mov    byte ptr [ebx + 0x7d], 1
        mov    dword ptr [ebx], ebx
        call   0x203
        sub    esp, 0xc
        mov    ebx, esp
        mov    word ptr [ebx], 1
        mov    word ptr [ebx + 4], 0x4a8
        push   ebx
        call   sub_1_0468
        mov    ebx, dword ptr [ebx + 6]
        add    esp, 0x10
        mov    ecx, 0x12a
        cli    
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
        mov    byte ptr [ebx + 0x7d], 2
        mov    dword ptr [ebx], ebx
        call   0x203
loc_3_01AE:
        pop    esi
        pop    ebp
        ret    
loc_3_01B1:
        mov    esi, dword ptr [ebx + 0xc]
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_01D2
        cmp    byte ptr [esi + 0x40], 5
        jne    loc_3_01D2
        mov    al, byte ptr [esi + 0x84]
        and    al, 7
        cmp    al, 2
        jae    loc_3_01D2
        dec    byte ptr [0]                  ; -> obj4:0x0000
loc_3_01D2:
        ret    
loc_3_01D3:
        cmp    byte ptr [0], 0               ; -> obj4:0x0000
        jne    loc_3_01E2
        mov    word ptr [ebx + 2], 0xffff
loc_3_01E2:
        ret    
loc_3_01E3:
        inc    ebp
        push   ebx
        inc    esp
        dec    ecx
        pop    esp
        db  000h
loc_3_01E9:
        push   ebp
        jae    0x251
        jb     0x232
        jb     0x259
        jbe    0x257
        dec    esp
        je     0x26a
        jb     0x23a
        jae    0x26e
        imul   esp, dword ptr [edi + 0x6e], 0x746e656d
        add    byte ptr [ebx - 0x729ff314], al
        inc    ebx
        jo     0x275
        add    bh, bh
        jbe    0x27f
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003003300
        add    esp, 0xc
        push   esi
        lea    eax, [esp + 0x2c]
        push   0
        push   dword ptr [ebx + 0x70]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID_Size ; cd2006003300
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_02CB
        mov    eax, dword ptr [esp + 0x2c]
        add    eax, 0x22
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_3_02CB
        push   eax
        lea    esi, [0x1e3]                  ; -> loc_3_01E3
        mov    ecx, 5
        mov    edi, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    esi, [ebx + 0x8a]
        mov    ecx, 0x19
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    al, 0x5c
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [esp + 0x30]
        push   0
        push   eax
        push   edi
        push   dword ptr [ebx + 0x70]
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007003300
        add    esp, 0x10
        pop    esi
        xor    ecx, ecx
        or     eax, eax
        jne    loc_3_02CB
        lea    edi, [esi + 0x1f]
        mov    al, 0x5c
        mov    ecx, dword ptr [esp + 0x2c]
        mov    edx, ecx
loc_3_0292:
        repne scasb al, byte ptr es:[edi]
        jecxz  loc_3_029C
        mov    byte ptr [edi - 1], 0x26
        jmp    loc_3_0292
loc_3_029C:
        lea    eax, [esi + 0x1f]
        add    eax, edx
        mov    cx, word ptr [ebx + 0x7c]
        add    cx, 0x3030
        mov    word ptr [eax], cx
        lea    edi, [esp + 0x28]
        push   0
        push   dword ptr [ebx + 0x70]
        push   esi
        push   edi
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b003300
        add    esp, 0x10
        xor    ecx, ecx
        or     eax, eax
        jne    loc_3_02CB
        mov    ecx, dword ptr [esp + 0x28]
loc_3_02CB:
        mov    dword ptr [ebx + 0x70], ecx
        mov    word ptr [ebx + 0x48], 0xffff
        lea    ecx, [esp + 0x24]
        sub    esp, 0x18
        mov    eax, esp
        mov    word ptr [eax], 0x11
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 0xc], 1
        mov    dword ptr [eax + 0x10], 3
        mov    dword ptr [eax + 0x14], ecx
        mov    dword ptr [eax + 8], 0x1e9    ; -> loc_3_01E9
        push   eax
        call   sub_1_0468
        movzx  eax, word ptr [eax + 2]
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_031C
        mov    ax, word ptr [esp + 0x24]
        sub    ax, 0x4141
        mov    word ptr [ebx + 0x48], ax
loc_3_031C:
        pop    esi
        call   0x345
        jae    loc_3_0329
        call   sub_3_0382
loc_3_0329:
        popal  
        add    esp, 0xc
        ret    
loc_3_032E:
        inc    ecx
        jne    loc_3_03A5
        outsd  dx, dword ptr [esi]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jae    loc_3_039B
        jb     loc_3_03AC
        dec    esi
        outsd  dx, dword ptr [esi]
        je     loc_3_03A5
        imul   sp, word ptr [ebx + 0x61], 0x6974
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [ebx - 0x3374e314], al
        mov    word ptr [ecx], 0x11
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 0xc], 3
        mov    dword ptr [ecx + 0x10], 1
        lea    edx, [ecx + 0x18]
        mov    dword ptr [ecx + 0x14], edx
        mov    dword ptr [ecx + 8], 0x32e    ; -> loc_3_032E
        push   ecx
        call   sub_1_0468
        movzx  ecx, word ptr [ecx + 2]
        movzx  edx, byte ptr [edx]
        add    esp, 0x20
        jecxz  loc_3_0381
        stc    
loc_3_0381:
        ret    
sub_3_0382:
        enter  0xc, 0
        pushal 
        mov    dword ptr [ebp - 0xc], 1
        mov    edx, dword ptr [esi + 0x70]
        lea    eax, [ebp - 8]
        mov    byte ptr [eax], 0
        push   0
        push   1
loc_3_039B:
        push   eax
        push   3
        push   0x32e                         ; -> loc_3_032E
        push   0
loc_3_03A5:
        push   edx
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
loc_3_03AC:
        add    esp, 0x1c
        popal  
        leave  
        ret    
        db  000h, 000h
sub_3_03B4:
        push   esi
        push   ebx
        push   dword ptr [ebx + 0x10]
        mov    esi, dword ptr [ebx + 0x10]
        mov    esi, dword ptr [esi + 8]
        mov    esi, dword ptr [esi + 8]
        call   sub_3_03F2
        or     edi, edi
        je     loc_3_03ED
        mov    dword ptr [edi + 0xa4], edi
        pop    dword ptr [edi + 0x10]
        mov    word ptr [edi + 0x68], 0x12
        mov    dword ptr [edi + 8], esi
        mov    ebx, edi
        call   sub_3_0476
        pop    dword ptr [ebx + 0x90]
        pop    esi
        clc    
        ret    
loc_3_03ED:
        pop    ebx
        pop    ebx
        pop    esi
        stc    
        ret    
sub_3_03F2:
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
        call   sub_1_0468
        mov    edi, dword ptr [edi + 0xa]
        add    esp, 0x14
        mov    eax, edi
        or     edi, edi
        je     loc_3_0474
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
loc_3_0474:
        pop    esi
        ret    
sub_3_0476:
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
        mov    dword ptr [edi + 0xc], 0
        mov    dword ptr [edi + 0x10], eax
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
sub_3_04D8:
        mov    esi, dword ptr [ebx + 0x10]
        mov    esi, dword ptr [esi + 8]
        mov    esi, dword ptr [esi + 8]
        mov    dword ptr [ebx + 4], esi
        mov    edx, 0x138                    ; -> loc_1_0138
        mov    byte ptr [edi + 3], 0
        mov    cx, word ptr [ebx + 0xe]
        mov    word ptr [ebx + 0xc], cx
        xchg   esi, edi
        xchg   edi, ebx
        mov    dword ptr [edi + 0xa4], edi
        call   sub_1_0474
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x4  RW-
VxD__LDATA ENDS
