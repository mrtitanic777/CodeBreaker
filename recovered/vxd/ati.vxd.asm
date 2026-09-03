; ATI -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 1897 instructions recovered by recursive descent

Declare_Virtual_Device ATI, 3, 1, ATI_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1a8  R-X
ATI_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_2_0000
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_6_0000
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_4_0000
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_5_0000
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_2_0000
loc_1_007D:
        push   edi
        cmp    byte ptr [0x1a3], 0           ; -> obj1:0x01A3
        jne    loc_1_00B2
        mov    edi, ebx
        add    edi, dword ptr [0x188]        ; -> obj1:0x0188
        test   ecx, 4
        je     loc_1_00B2
        cmp    ecx, 0xc
        jne    loc_1_00A2
        mov    word ptr [edi + 0x43], ax
        jmp    loc_1_00B2
loc_1_00A2:
        cmp    ecx, 4
        jne    loc_1_00B2
        add    edi, 0x43
        cmp    dl, 0xee
        je     loc_1_00B0
        inc    edi
loc_1_00B0:
        mov    byte ptr [edi], al
loc_1_00B2:
        pop    edi
        VxDCall  VDD.VDD_Do_Physical_IO          ; cd2015800a00
loc_1_00B9:
        push   edi
        cmp    byte ptr [0x1a3], 0           ; -> obj1:0x01A3
        jne    loc_1_00EE
        mov    edi, ebx
        add    edi, dword ptr [0x188]        ; -> obj1:0x0188
        test   ecx, 4
        je     loc_1_00EE
        cmp    ecx, 0xc
        jne    loc_1_00DE
        mov    word ptr [edi + 0x41], ax
        jmp    loc_1_00EE
loc_1_00DE:
        cmp    ecx, 4
        jne    loc_1_00EE
        add    edi, 0x41
        cmp    dl, 0xe8
        je     loc_1_00EC
        inc    edi
loc_1_00EC:
        mov    byte ptr [edi], al
loc_1_00EE:
        pop    edi
        VxDCall  VDD.VDD_Do_Physical_IO          ; cd2015800a00
loc_1_00F5:
        or     eax, eax
        jne    loc_1_0100
        call   sub_1_010A
        jb     loc_1_0108
loc_1_0100:
        mov    eax, 0xffffffff
        clc    
        jmp    loc_1_0109
loc_1_0108:
        stc    
loc_1_0109:
        ret    
sub_1_010A:
        push   edi
        mov    edi, ebx
        add    edi, dword ptr [0x188]        ; -> obj1:0x0188
        mov    al, byte ptr [edi + 0x43]
        or     al, byte ptr [edi + 0x41]
        test   al, 1
        clc    
        je     loc_1_011F
        stc    
loc_1_011F:
        pop    edi
        ret    
        db  000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'ATI     '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  ATI_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 020h, 037h, 036h, 031h, 032h, 039h, 035h, 035h
        db  032h, 030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_019C:
        inc    dword ptr [eax]
loc_1_019E:
        add    byte ptr [eax], al
loc_1_01A0:
        add    byte ptr [eax], al
loc_1_01A2:
        add    byte ptr [eax], al
loc_1_01A4:
        add    byte ptr [eax], al
loc_1_01A6:
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xab8  R-X
loc_2_0000:
        cld    
        mov    dword ptr [0x180], ebx        ; -> obj1:0x0180
        mov    word ptr [0x1a4], 0           ; -> loc_1_01A4
        mov    ax, 0xa012
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    ah, 0
        jne    loc_2_0179
        cmp    dx, 0x2ec
        je     loc_2_003C
        cmp    dx, 0x1c8
        je     loc_2_003C
        or     cx, cx
        je     loc_2_0179
loc_2_003C:
        sub    dx, 0x2ec
        mov    word ptr [0x1a4], dx          ; -> loc_1_01A4
        mov    word ptr [0x1a6], cx          ; -> loc_1_01A6
        xor    edx, edx
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_006A
        add    dx, 0x42ec
        jmp    loc_2_006F
loc_2_006A:
        add    dx, 0x36c
loc_2_006F:
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        mov    ecx, eax
        sub    dx, 3
        mov    eax, 0xa55aa55a
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        sub    dx, 3
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        sub    dx, 3
        xchg   ecx, eax
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        cmp    ecx, 0xa55aa55a
        jne    loc_2_0179
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_00FA
        add    dx, 0x46ec
        jmp    loc_2_00FF
loc_2_00FA:
        add    dx, 0x370
loc_2_00FF:
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        mov    ecx, eax
        sub    dx, 3
        mov    eax, 0x12345678
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        sub    dx, 3
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        sub    dx, 3
        xchg   ecx, eax
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        cmp    ecx, 0x12345678
        jne    loc_2_0179
        mov    byte ptr [0x1a2], 0x64        ; -> loc_1_01A2
        jmp    loc_2_026F
loc_2_0179:
        mov    dx, 0x42e8
        mov    ax, 0x900f
        out    dx, ax
        call   sub_6_0FCB
        mov    ax, 0x400f
        out    dx, ax
        mov    dx, 0x92e8
        mov    ax, 0x5555
        out    dx, ax
        call   sub_6_0FCB
        in     ax, dx
        cmp    ax, 0x5555
        jne    loc_2_0202
        mov    ax, 0xaaaa
        out    dx, ax
        call   sub_6_0FCB
        in     ax, dx
        cmp    ax, 0xaaaa
        jne    loc_2_0202
        mov    dx, 0x52ee
        in     ax, dx
        ror    eax, 0x10
        mov    ax, 0x5555
        out    dx, ax
        call   sub_6_0FCB
        in     ax, dx
        ror    eax, 0x10
        out    dx, ax
        ror    eax, 0x10
        cmp    ax, 0x5555
        jne    loc_2_0202
        mov    byte ptr [0x1a2], 8           ; -> loc_1_01A2
        mov    dx, 0x8ee8
        mov    ax, 0xaaaa
        out    dx, ax
        mov    dx, 0xdaee
        in     ax, dx
        cmp    ax, 0x2aa
        jne    loc_2_026F
        mov    byte ptr [0x1a2], 0x32        ; -> loc_1_01A2
        jmp    loc_2_026F
loc_2_0202:
        push   0
        push   0x100
        push   0xc0000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_0A71
        cld    
        mov    edi, eax
        add    edi, 0x30
        mov    esi, 0x174                    ; -> obj1:0x0174
        mov    ecx, 0xa
        repe cmpsb byte ptr es:[esi], byte ptr es:[edi]
        jne    loc_2_0A71
        cmp    byte ptr [eax + 0x40], 0x33
        jne    loc_2_0A71
        push   ebp
        push   ebx
        mov    eax, 0x1203
        mov    ebx, 0x5506
        mov    ebp, 0xffff
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    bp, -1
        pop    ebx
        pop    ebp
        je     loc_2_0A71
        mov    byte ptr [0x1a2], 0           ; -> loc_1_01A2
loc_2_026F:
        push   0
        push   0x47
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0x188], eax        ; -> obj1:0x0188
        or     eax, eax
        je     loc_2_0A71
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_0A71
        cmp    byte ptr [0x1a2], 0           ; -> loc_1_01A2
        je     loc_2_035E
        mov    dword ptr [edi + 4], 0xbe     ; -> loc_3_00BE
        mov    dword ptr [edi + 0xc0], 0xfd7 ; -> loc_6_0FD7
        cmp    byte ptr [0x1a2], 0x32        ; -> loc_1_01A2
        jb     loc_2_035E
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 0x94], 0x1019 ; -> loc_6_1019
        mov    dword ptr [edi + 0x90], 0x1011 ; -> loc_6_1011
        ja     loc_2_034A
        mov    dword ptr [edi + 8], 7        ; -> loc_6_0007
        mov    dword ptr [edi + 0xc], 0x6a   ; -> loc_6_006A
        mov    dword ptr [edi + 0x58], 0xc8  ; -> loc_6_00C8
        mov    dword ptr [edi + 0x5c], 0x10d ; -> loc_6_010D
        mov    dword ptr [edi + 0x3c], 0xf86 ; -> loc_6_0F86
        mov    dword ptr [edi + 0x88], 0x1071 ; -> loc_6_1071
        mov    dword ptr [edi + 0x80], 0x10b7 ; -> loc_6_10B7
        mov    dword ptr [edi + 0x84], 0x10e6 ; -> loc_6_10E6
        mov    dword ptr [edi + 0x9c], 0x1022 ; -> loc_6_1022
        mov    dword ptr [edi + 0xa0], 0x104d ; -> loc_6_104D
        mov    dword ptr [edi + 0xac], 0xf5  ; -> loc_1_00F5
        mov    dword ptr [edi + 0x8c], 0x10a ; -> sub_1_010A
        jmp    loc_2_035E
loc_2_034A:
        mov    dword ptr [edi + 0x80], 0x10b7 ; -> loc_6_10B7
        mov    dword ptr [edi + 0x84], 0x10e6 ; -> loc_6_10E6
loc_2_035E:
        mov    dword ptr [edi + 0x10], 0x13c ; -> loc_6_013C
        mov    dword ptr [edi + 0x14], 0x16d ; -> loc_6_016D
        mov    dword ptr [edi + 0x18], 0x199 ; -> loc_6_0199
        mov    dword ptr [edi + 0x1c], 0x1bd ; -> loc_6_01BD
        mov    dword ptr [edi + 0x34], 0x244 ; -> loc_6_0244
        mov    dword ptr [edi + 0x38], 0x517 ; -> loc_6_0517
        mov    dword ptr [edi + 0xa8], 0xab0 ; -> loc_2_0AB0
        cmp    byte ptr [0x1a2], 0x32        ; -> loc_1_01A2
        jne    loc_2_059F
        mov    edx, 0x76ee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x82e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x82ee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x86e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x8ae8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x8ee8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x8eee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x92e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x9aee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xa2e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xa2ee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xa6e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xa6ee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xaaee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xaeee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xb2ee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xbaee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xbee8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xbeee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xc2ee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xcaee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xceee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xd2ee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xd6ee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xdaee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xdeee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xe2e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xe2ee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xe6ee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xfeee
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    esi, 0x7d                     ; -> loc_1_007D
        mov    edx, 0x4aee
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0A71
        mov    edx, 0x4aee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x4aef
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0A71
        mov    edx, 0x4aef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    esi, 0xb9                     ; -> loc_1_00B9
        mov    edx, 0x4ae8
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0A71
        mov    edx, 0x4ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0A71
        mov    edx, 0x4ae9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
loc_2_059F:
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    edx, 0x2e8
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    edx, 0x1ce
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0A71
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    edx, 0x1cf
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0A71
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    dword ptr [0x18c], 0          ; -> obj1:0x018C
        cmp    byte ptr [0x1a2], 0x64        ; -> loc_1_01A2
        jne    loc_2_0A4A
        xor    edx, edx
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_0624
        add    dx, 0x4eec
        jmp    loc_2_0629
loc_2_0624:
        add    dx, 0x38c
loc_2_0629:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_0652
        add    dx, 0x52ec
        jmp    loc_2_0657
loc_2_0652:
        add    dx, 0x39c
loc_2_0657:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_0680
        add    dx, 0x56ec
        jmp    loc_2_0685
loc_2_0680:
        add    dx, 0x3a0
loc_2_0685:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_06AE
        add    dx, 0x5aec
        jmp    loc_2_06B3
loc_2_06AE:
        add    dx, 0x3a4
loc_2_06B3:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_06DC
        add    dx, 0x66ec
        jmp    loc_2_06E1
loc_2_06DC:
        add    dx, 0x3bc
loc_2_06E1:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_070A
        add    dx, 0x6aec
        jmp    loc_2_070F
loc_2_070A:
        add    dx, 0x3c8
loc_2_070F:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_0738
        add    dx, 0x6ec
        jmp    loc_2_073D
loc_2_0738:
        add    dx, 0x2f0
loc_2_073D:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_0766
        add    dx, 0xaec
        jmp    loc_2_076B
loc_2_0766:
        add    dx, 0x2f4
loc_2_076B:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_0794
        add    dx, 0xeec
        jmp    loc_2_0799
loc_2_0794:
        add    dx, 0x2f8
loc_2_0799:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_07C2
        add    dx, 0x12ec
        jmp    loc_2_07C7
loc_2_07C2:
        add    dx, 0x2fc
loc_2_07C7:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_07F0
        add    dx, 0x16ec
        jmp    loc_2_07F5
loc_2_07F0:
        add    dx, 0x300
loc_2_07F5:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_081E
        add    dx, 0x1aec
        jmp    loc_2_0823
loc_2_081E:
        add    dx, 0x304
loc_2_0823:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_084C
        add    dx, 0x1eec
        jmp    loc_2_0851
loc_2_084C:
        add    dx, 0x308
loc_2_0851:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_087A
        add    dx, 0x22ec
        jmp    loc_2_087F
loc_2_087A:
        add    dx, 0x32c
loc_2_087F:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_08A8
        add    dx, 0x26ec
        jmp    loc_2_08AD
loc_2_08A8:
        add    dx, 0x330
loc_2_08AD:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_08D6
        add    dx, 0x2aec
        jmp    loc_2_08DB
loc_2_08D6:
        add    dx, 0x334
loc_2_08DB:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_0904
        add    dx, 0x2eec
        jmp    loc_2_0909
loc_2_0904:
        add    dx, 0x34c
loc_2_0909:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_0932
        add    dx, 0x32ec
        jmp    loc_2_0937
loc_2_0932:
        add    dx, 0x350
loc_2_0937:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_0960
        add    dx, 0x36ec
        jmp    loc_2_0965
loc_2_0960:
        add    dx, 0x354
loc_2_0965:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_098E
        add    dx, 0x3aec
        jmp    loc_2_0993
loc_2_098E:
        add    dx, 0x358
loc_2_0993:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_09BC
        add    dx, 0x3eec
        jmp    loc_2_09C1
loc_2_09BC:
        add    dx, 0x35c
loc_2_09C1:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_09EA
        add    dx, 0x4aec
        jmp    loc_2_09EF
loc_2_09EA:
        add    dx, 0x37c
loc_2_09EF:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_0A14
        add    dx, 0x5eec
        jmp    loc_2_0A19
loc_2_0A14:
        add    dx, 0x3ac
loc_2_0A19:
        call   sub_2_0A73
        jb     loc_2_0A72
        mov    esi, 0xeab                    ; -> loc_6_0EAB
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_2_0A3E
        add    dx, 0x62ec
        jmp    loc_2_0A43
loc_2_0A3E:
        add    dx, 0x3b0
loc_2_0A43:
        call   sub_2_0A73
        jb     loc_2_0A72
loc_2_0A4A:
        cmp    byte ptr [0x1a2], 0x32        ; -> loc_1_01A2
        jne    loc_2_0A6E
        mov    edx, 0x52ee
        in     al, dx
        and    eax, 0x7f
        mov    edx, 0x80
        mul    edx
        add    eax, 0xc000
        mov    word ptr [0x198], ax          ; -> obj1:0x0198
loc_2_0A6E:
        clc    
        jmp    loc_2_0A72
loc_2_0A71:
        stc    
loc_2_0A72:
        ret    
sub_2_0A73:
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0AAF
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        inc    edx
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0AAF
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        inc    edx
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0AAF
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        inc    edx
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0AAF
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        clc    
loc_2_0AAF:
        ret    
loc_2_0AB0:
        movzx  eax, byte ptr [0x1a2]         ; -> loc_1_01A2
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16e  R-X
loc_3_0000:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [0x184], eax        ; -> obj1:0x0184
        mov    edx, dword ptr [ebp + 0x14]
        or     edx, edx
        je     loc_3_0064
        cmp    dword ptr [0x190], 0          ; -> obj1:0x0190
        jne    loc_3_0061
        push   0
        push   0x800000
        push   edx
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        push   0
        push   0x80
        push   0x92
        push   0x800
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        VMMCall  VMM._Allocate_LDT_Selector      ; cd2078000100
        add    esp, 0x14
        mov    dword ptr [0x190], eax        ; -> obj1:0x0190
        push   eax
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        pop    eax
loc_3_0061:
        mov    dword ptr [ebp + 0x14], eax
loc_3_0064:
        mov    edx, dword ptr [ebp + 4]
        or     edx, edx
        je     loc_3_00BD
        cmp    dword ptr [0x194], 0          ; -> obj1:0x0194
        jne    loc_3_00BA
        push   0
        push   0x2000
        push   edx
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        push   0
        push   0x80
        push   0x92
        push   2
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        VMMCall  VMM._Allocate_LDT_Selector      ; cd2078000100
        add    esp, 0x14
        mov    dword ptr [0x194], eax        ; -> obj1:0x0194
        push   eax
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        pop    eax
loc_3_00BA:
        mov    dword ptr [ebp + 4], eax
loc_3_00BD:
        ret    
loc_3_00BE:
        mov    byte ptr [0x1a0], 0xff        ; -> loc_1_01A0
        cmp    byte ptr [0x1a2], 0x32        ; -> loc_1_01A2
        je     loc_3_00E9
        cmp    byte ptr [0x1a2], 0x64        ; -> loc_1_01A2
        je     loc_3_016A
        xor    ecx, ecx
        mov    edx, 0x10000
        xor    ah, ah
        jmp    loc_3_016D
loc_3_00E9:
        mov    eax, ecx
        shr    eax, 0x10
        or     cx, cx
        je     loc_3_010D
        mov    edx, dword ptr [0x184]        ; -> obj1:0x0184
        shr    edx, 0x10
        dec    dl
        cmp    al, dl
        jae    loc_3_016A
        inc    al
        xor    cx, cx
        add    ecx, 0x10000
loc_3_010D:
        mov    edx, dword ptr [0x184]        ; -> obj1:0x0184
        sub    edx, ecx
        cmp    edx, 0x10000
        mov    edx, 0x10000
        ja     loc_3_0127
        mov    edx, 0x8000
loc_3_0127:
        push   edx
        push   ebx
        mov    byte ptr [0x19c], 0xff        ; -> loc_1_019C
        mov    ah, al
        shr    al, 2
        and    ah, 3
        shl    ah, 2
        mov    byte ptr [0x1a1], ah          ; -> obj1:0x01A1
        mov    bl, al
        mov    bh, al
        and    bl, 0xf
        shl    bl, 1
        mov    byte ptr [0x19a], bl          ; -> obj1:0x019A
        shr    bh, 4
        mov    edx, 0x1ce
        mov    al, 0xae
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf0
        or     al, bh
        mov    byte ptr [0x19b], al          ; -> obj1:0x019B
        pop    ebx
        pop    edx
        jmp    loc_3_016D
loc_3_016A:
        mov    edx, dword ptr [ebp + 0x18]
loc_3_016D:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x1e  R-X
loc_4_0000:
        mov    edx, 0x1ce
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x1cf
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        inc    dword ptr [0x18c]             ; -> obj1:0x018C
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x14  R-X
loc_5_0000:
        dec    dword ptr [0x18c]             ; -> obj1:0x018C
        jge    loc_5_0012
        mov    dword ptr [0x18c], 0          ; -> obj1:0x018C
loc_5_0012:
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x1115  R-X
loc_6_0000:
        call   sub_6_01C5
        clc    
        ret    
loc_6_0007:
        push   eax
        push   edx
        mov    edx, 0x42ee
        xor    eax, eax
        out    dx, ax
        pop    edx
        pop    eax
        cmp    byte ptr [0x19c], 0xff        ; -> loc_1_019C
        jne    loc_6_0061
        cmp    byte ptr [0x1a0], 0           ; -> loc_1_01A0
        je     loc_6_0069
        push   eax
        push   edx
        mov    edx, 0x1ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb2
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x19c], al          ; -> loc_1_019C
        dec    edx
        mov    ah, byte ptr [0x19a]          ; -> obj1:0x019A
        mov    al, 0xb2
        out    dx, ax
        mov    al, 0xae
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x19d], al          ; -> obj1:0x019D
        dec    edx
        mov    ah, byte ptr [0x19b]          ; -> obj1:0x019B
        mov    al, 0xae
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_6_0061:
        and    al, 0xf
        add    al, byte ptr [0x1a1]          ; -> obj1:0x01A1
loc_6_0069:
        ret    
loc_6_006A:
        cmp    byte ptr [0x19c], 0xff        ; -> loc_1_019C
        je     loc_6_00C7
        push   eax
        push   edx
        mov    edx, 0x1ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xae
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        dec    edx
        mov    al, 0xb2
        out    dx, al
        inc    edx
        in     al, dx
        mov    dl, byte ptr [0x19a]          ; -> obj1:0x019A
        mov    dh, byte ptr [0x19b]          ; -> obj1:0x019B
        cmp    ax, dx
        jne    loc_6_00B5
        mov    edx, 0x1ce
        mov    al, 0xb2
        mov    ah, byte ptr [0x19c]          ; -> loc_1_019C
        out    dx, ax
        mov    al, 0xae
        mov    ah, byte ptr [0x19d]          ; -> obj1:0x019D
        out    dx, ax
loc_6_00B5:
        mov    byte ptr [0x19c], 0xff        ; -> loc_1_019C
        mov    edx, 0x1ce
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_6_00C7:
        ret    
loc_6_00C8:
        push   eax
        push   edx
        cmp    byte ptr [0x1a0], 0           ; -> loc_1_01A0
        je     loc_6_010A
        mov    edx, 0x1ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb2
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x19f], al          ; -> obj1:0x019F
        dec    edx
        mov    al, 0xb2
        mov    ah, byte ptr [0x19a]          ; -> obj1:0x019A
        out    dx, ax
        mov    al, 0xae
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x19e], al          ; -> loc_1_019E
        dec    edx
        mov    al, 0xae
        mov    ah, byte ptr [0x19b]          ; -> obj1:0x019B
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_6_010A:
        pop    edx
        pop    eax
        ret    
loc_6_010D:
        push   eax
        push   edx
        cmp    byte ptr [0x1a0], 0           ; -> loc_1_01A0
        je     loc_6_0139
        mov    edx, 0x1ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb2
        mov    ah, byte ptr [0x19f]          ; -> obj1:0x019F
        out    dx, ax
        mov    al, 0xae
        mov    ah, byte ptr [0x19e]          ; -> loc_1_019E
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_6_0139:
        pop    edx
        pop    eax
        ret    
loc_6_013C:
        mov    byte ptr [0x1a3], 0xff        ; -> obj1:0x01A3
        mov    edx, 0x1ce
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x1cf
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        cmp    byte ptr [0x1a2], 0x64        ; -> loc_1_01A2
        jne    loc_6_0167
        call   sub_6_0B28
loc_6_0167:
        call   sub_6_01C5
        ret    
loc_6_016D:
        mov    edx, 0x1ce
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x1cf
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        cmp    byte ptr [0x1a2], 0x64        ; -> loc_1_01A2
        jne    loc_6_0191
        call   sub_6_07DD
loc_6_0191:
        mov    byte ptr [0x1a3], 0           ; -> obj1:0x01A3
        ret    
loc_6_0199:
        mov    byte ptr [0x1a3], 0xff        ; -> obj1:0x01A3
        cmp    byte ptr [0x1a0], 0           ; -> loc_1_01A0
        je     loc_6_01BC
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xe04
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_6_01BC:
        ret    
loc_6_01BD:
        mov    byte ptr [0x1a3], 0           ; -> obj1:0x01A3
        ret    
sub_6_01C5:
        cmp    byte ptr [0x1a0], 0           ; -> loc_1_01A0
        je     loc_6_0243
        cmp    byte ptr [0x1a2], 8           ; -> loc_1_01A2
        je     loc_6_0238
        cmp    byte ptr [0x1a2], 0x32        ; -> loc_1_01A2
        jne    loc_6_0243
        push   ebx
        push   ebp
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    edx, 0x68
        mov    cx, word ptr [0x198]          ; -> obj1:0x0198
        mov    dword ptr [ebp + 0x1c], 0
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebp
        pop    ebx
        jmp    loc_6_0243
loc_6_0238:
        mov    edx, 0x4aee
        in     ax, dx
        and    al, 0xfe
        out    dx, ax
loc_6_0243:
        ret    
loc_6_0244:
        mov    edx, 0x1ce
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x1cf
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        cmp    byte ptr [0x1a2], 0x64        ; -> loc_1_01A2
        jne    loc_6_0268
        call   sub_6_07DD
loc_6_0268:
        cmp    byte ptr [0x1a2], 0x32        ; -> loc_1_01A2
        jne    loc_6_0516
        cmp    byte ptr [0x1a0], 0           ; -> loc_1_01A0
        je     loc_6_0516
        mov    edx, 0x76ee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x76ef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x82e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x82e9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x82ee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x82ef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x86e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x86e9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x8ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x8ae9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x8ee8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x8ee9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x8eee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x8eef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x92e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x92e9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x9aee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x9aef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa2e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa2e9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa2ee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa2ef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa6e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa6e9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa6ee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa6ef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xaaee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xaaef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xaeee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xaeef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xb2ee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xb2ef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xbaee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xbaef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xbee8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xbee9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xbeee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xbeef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xc2ee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xc2ef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xcaee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xcaef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xceee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xceef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xd2ee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xd2ef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xd6ee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xd6ef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xdaee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xdaef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xdeee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xdeef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xe2e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xe2e9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xe2ee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xe2ef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xe6ee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xe6ef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xfeee
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xfeef
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
loc_6_0516:
        ret    
loc_6_0517:
        cmp    dword ptr [0x18c], 0          ; -> obj1:0x018C
        jg     loc_6_052E
        cmp    byte ptr [0x1a2], 0x64        ; -> loc_1_01A2
        jne    loc_6_052E
        call   sub_6_0B28
loc_6_052E:
        cmp    byte ptr [0x1a2], 0x32        ; -> loc_1_01A2
        jne    loc_6_07DC
        cmp    byte ptr [0x1a0], 0           ; -> loc_1_01A0
        je     loc_6_07DC
        mov    edx, 0x76ee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x76ef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x82e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x82e9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x82ee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x82ef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x86e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x86e9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x8ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x8ae9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x8ee8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x8ee9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x8eee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x8eef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x92e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x92e9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x9aee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x9aef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa2e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa2e9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa2ee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa2ef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa6e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa6e9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa6ee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa6ef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xaaee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xaaef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xaeee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xaeef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xb2ee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xb2ef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xbaee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xbaef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xbee8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xbee9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xbeee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xbeef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xc2ee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xc2ef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xcaee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xcaef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xceee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xceef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xd2ee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xd2ef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xd6ee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xd6ef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xdaee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xdaef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xdeee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xdeef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xe2e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xe2e9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xe2ee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xe2ef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xe6ee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xe6ef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xfeee
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xfeef
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
loc_6_07DC:
        ret    
sub_6_07DD:
        xor    edx, edx
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_07F8
        add    dx, 0x4eec
        jmp    loc_6_07FD
loc_6_07F8:
        add    dx, 0x38c
loc_6_07FD:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_081B
        add    dx, 0x52ec
        jmp    loc_6_0820
loc_6_081B:
        add    dx, 0x39c
loc_6_0820:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_083E
        add    dx, 0x56ec
        jmp    loc_6_0843
loc_6_083E:
        add    dx, 0x3a0
loc_6_0843:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0861
        add    dx, 0x5aec
        jmp    loc_6_0866
loc_6_0861:
        add    dx, 0x3a4
loc_6_0866:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0884
        add    dx, 0x66ec
        jmp    loc_6_0889
loc_6_0884:
        add    dx, 0x3bc
loc_6_0889:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_08A7
        add    dx, 0x6aec
        jmp    loc_6_08AC
loc_6_08A7:
        add    dx, 0x3c8
loc_6_08AC:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_08CA
        add    dx, 0x6ec
        jmp    loc_6_08CF
loc_6_08CA:
        add    dx, 0x2f0
loc_6_08CF:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_08ED
        add    dx, 0xaec
        jmp    loc_6_08F2
loc_6_08ED:
        add    dx, 0x2f4
loc_6_08F2:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0910
        add    dx, 0xeec
        jmp    loc_6_0915
loc_6_0910:
        add    dx, 0x2f8
loc_6_0915:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0933
        add    dx, 0x12ec
        jmp    loc_6_0938
loc_6_0933:
        add    dx, 0x2fc
loc_6_0938:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0956
        add    dx, 0x16ec
        jmp    loc_6_095B
loc_6_0956:
        add    dx, 0x300
loc_6_095B:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0979
        add    dx, 0x1aec
        jmp    loc_6_097E
loc_6_0979:
        add    dx, 0x304
loc_6_097E:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_099C
        add    dx, 0x1eec
        jmp    loc_6_09A1
loc_6_099C:
        add    dx, 0x308
loc_6_09A1:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_09BF
        add    dx, 0x22ec
        jmp    loc_6_09C4
loc_6_09BF:
        add    dx, 0x32c
loc_6_09C4:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_09E2
        add    dx, 0x26ec
        jmp    loc_6_09E7
loc_6_09E2:
        add    dx, 0x330
loc_6_09E7:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0A05
        add    dx, 0x2aec
        jmp    loc_6_0A0A
loc_6_0A05:
        add    dx, 0x334
loc_6_0A0A:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0A28
        add    dx, 0x2eec
        jmp    loc_6_0A2D
loc_6_0A28:
        add    dx, 0x34c
loc_6_0A2D:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0A4B
        add    dx, 0x32ec
        jmp    loc_6_0A50
loc_6_0A4B:
        add    dx, 0x350
loc_6_0A50:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0A6E
        add    dx, 0x36ec
        jmp    loc_6_0A73
loc_6_0A6E:
        add    dx, 0x354
loc_6_0A73:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0A91
        add    dx, 0x3aec
        jmp    loc_6_0A96
loc_6_0A91:
        add    dx, 0x358
loc_6_0A96:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0AB4
        add    dx, 0x3eec
        jmp    loc_6_0AB9
loc_6_0AB4:
        add    dx, 0x35c
loc_6_0AB9:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0AD7
        add    dx, 0x4aec
        jmp    loc_6_0ADC
loc_6_0AD7:
        add    dx, 0x37c
loc_6_0ADC:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0AFA
        add    dx, 0x5eec
        jmp    loc_6_0AFF
loc_6_0AFA:
        add    dx, 0x3ac
loc_6_0AFF:
        call   sub_6_0E73
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0B1D
        add    dx, 0x62ec
        jmp    loc_6_0B22
loc_6_0B1D:
        add    dx, 0x3b0
loc_6_0B22:
        call   sub_6_0E73
        ret    
sub_6_0B28:
        xor    edx, edx
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0B43
        add    dx, 0x4eec
        jmp    loc_6_0B48
loc_6_0B43:
        add    dx, 0x38c
loc_6_0B48:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0B66
        add    dx, 0x52ec
        jmp    loc_6_0B6B
loc_6_0B66:
        add    dx, 0x39c
loc_6_0B6B:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0B89
        add    dx, 0x56ec
        jmp    loc_6_0B8E
loc_6_0B89:
        add    dx, 0x3a0
loc_6_0B8E:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0BAC
        add    dx, 0x5aec
        jmp    loc_6_0BB1
loc_6_0BAC:
        add    dx, 0x3a4
loc_6_0BB1:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0BCF
        add    dx, 0x66ec
        jmp    loc_6_0BD4
loc_6_0BCF:
        add    dx, 0x3bc
loc_6_0BD4:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0BF2
        add    dx, 0x6aec
        jmp    loc_6_0BF7
loc_6_0BF2:
        add    dx, 0x3c8
loc_6_0BF7:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0C15
        add    dx, 0x6ec
        jmp    loc_6_0C1A
loc_6_0C15:
        add    dx, 0x2f0
loc_6_0C1A:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0C38
        add    dx, 0xaec
        jmp    loc_6_0C3D
loc_6_0C38:
        add    dx, 0x2f4
loc_6_0C3D:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0C5B
        add    dx, 0xeec
        jmp    loc_6_0C60
loc_6_0C5B:
        add    dx, 0x2f8
loc_6_0C60:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0C7E
        add    dx, 0x12ec
        jmp    loc_6_0C83
loc_6_0C7E:
        add    dx, 0x2fc
loc_6_0C83:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0CA1
        add    dx, 0x16ec
        jmp    loc_6_0CA6
loc_6_0CA1:
        add    dx, 0x300
loc_6_0CA6:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0CC4
        add    dx, 0x1aec
        jmp    loc_6_0CC9
loc_6_0CC4:
        add    dx, 0x304
loc_6_0CC9:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0CE7
        add    dx, 0x1eec
        jmp    loc_6_0CEC
loc_6_0CE7:
        add    dx, 0x308
loc_6_0CEC:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0D0A
        add    dx, 0x22ec
        jmp    loc_6_0D0F
loc_6_0D0A:
        add    dx, 0x32c
loc_6_0D0F:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0D2D
        add    dx, 0x26ec
        jmp    loc_6_0D32
loc_6_0D2D:
        add    dx, 0x330
loc_6_0D32:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0D50
        add    dx, 0x2aec
        jmp    loc_6_0D55
loc_6_0D50:
        add    dx, 0x334
loc_6_0D55:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0D73
        add    dx, 0x2eec
        jmp    loc_6_0D78
loc_6_0D73:
        add    dx, 0x34c
loc_6_0D78:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0D96
        add    dx, 0x32ec
        jmp    loc_6_0D9B
loc_6_0D96:
        add    dx, 0x350
loc_6_0D9B:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0DB9
        add    dx, 0x36ec
        jmp    loc_6_0DBE
loc_6_0DB9:
        add    dx, 0x354
loc_6_0DBE:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0DDC
        add    dx, 0x3aec
        jmp    loc_6_0DE1
loc_6_0DDC:
        add    dx, 0x358
loc_6_0DE1:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0DFF
        add    dx, 0x3eec
        jmp    loc_6_0E04
loc_6_0DFF:
        add    dx, 0x35c
loc_6_0E04:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0E22
        add    dx, 0x4aec
        jmp    loc_6_0E27
loc_6_0E22:
        add    dx, 0x37c
loc_6_0E27:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0E45
        add    dx, 0x5eec
        jmp    loc_6_0E4A
loc_6_0E45:
        add    dx, 0x3ac
loc_6_0E4A:
        call   sub_6_0E8F
        mov    dx, word ptr [0x1a4]          ; -> loc_1_01A4
        test   word ptr [0x1a6], 1           ; -> loc_1_01A6
        jne    loc_6_0E68
        add    dx, 0x62ec
        jmp    loc_6_0E6D
loc_6_0E68:
        add    dx, 0x3b0
loc_6_0E6D:
        call   sub_6_0E8F
        ret    
sub_6_0E73:
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        inc    edx
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        inc    edx
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        inc    edx
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        ret    
sub_6_0E8F:
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        inc    edx
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        inc    edx
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        inc    edx
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        ret    
loc_6_0EAB:
        push   edi
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, dword ptr [0x180]        ; -> obj1:0x0180
        jne    loc_6_0EBF
        cmp    ebx, edi
        jne    loc_6_0EFE
loc_6_0EBF:
        cmp    ecx, 4
        je     loc_6_0EF8
        cmp    ecx, 0
        je     loc_6_0EE5
        cmp    ecx, 0xc
        je     loc_6_0EF1
        cmp    ecx, 8
        je     loc_6_0EDE
        cmp    ecx, 0x14
        je     loc_6_0EEB
        in     eax, dx
        jmp    loc_6_0F83
loc_6_0EDE:
        in     ax, dx
        jmp    loc_6_0F83
loc_6_0EE5:
        in     al, dx
        jmp    loc_6_0F83
loc_6_0EEB:
        out    dx, eax
        jmp    loc_6_0F83
loc_6_0EF1:
        out    dx, ax
        jmp    loc_6_0F83
loc_6_0EF8:
        out    dx, al
        jmp    loc_6_0F83
loc_6_0EFE:
        cmp    byte ptr [0x1a2], 0x64        ; -> loc_1_01A2
        jne    loc_6_0F15
        cmp    dx, 0x1ce
        je     loc_6_0F15
        cmp    dx, 0x1cf
        jne    loc_6_0F83
loc_6_0F15:
        mov    edi, ebx
        add    edi, dword ptr [0x188]        ; -> obj1:0x0188
        cmp    ecx, 4
        je     loc_6_0F6B
        cmp    ecx, 0xc
        je     loc_6_0F5C
        cmp    dx, 0x1ce
        jne    loc_6_0F33
        mov    al, byte ptr [edi + 0x40]
        jmp    loc_6_0F83
loc_6_0F33:
        movzx  eax, byte ptr [edi + 0x40]
        cmp    al, 0xb6
        je     loc_6_0F4F
        cmp    al, 0xb0
        je     loc_6_0F4F
        cmp    al, 0xba
        je     loc_6_0F4F
        cmp    al, 0xbb
        je     loc_6_0F4F
        cmp    al, 0xa8
        je     loc_6_0F4F
        cmp    al, 0xa9
        jne    loc_6_0F55
loc_6_0F4F:
        dec    edx
        out    dx, al
        inc    edx
        in     al, dx
        jmp    loc_6_0F83
loc_6_0F55:
        sub    al, 0x80
        mov    al, byte ptr [eax + edi]
        jmp    loc_6_0F83
loc_6_0F5C:
        mov    byte ptr [edi + 0x40], al
        sub    al, 0x80
        mov    cl, ah
        movzx  eax, al
        mov    byte ptr [eax + edi], cl
        jmp    loc_6_0F83
loc_6_0F6B:
        cmp    edx, 0x1ce
        jne    loc_6_0F78
        mov    byte ptr [edi + 0x40], al
        jmp    loc_6_0F83
loc_6_0F78:
        mov    cl, al
        movzx  eax, byte ptr [edi + 0x40]
        sub    al, 0x80
        mov    byte ptr [eax + edi], cl
loc_6_0F83:
        pop    esi
        pop    edi
        ret    
loc_6_0F86:
        push   edx
        cmp    eax, dword ptr [0x180]        ; -> obj1:0x0180
        jne    loc_6_0FC9
        cmp    byte ptr [0x1a0], 0           ; -> loc_1_01A0
        je     loc_6_0FC9
        mov    ecx, 0x1000
loc_6_0F9D:
        mov    dx, 0x9aee
        in     ax, dx
        test   ax, 0xffff
        jne    loc_6_0FC1
        mov    dx, 0x62ee
        in     ax, dx
        test   ax, 0x2000
        jne    loc_6_0FC1
        mov    dx, 0x9ae8
        in     ax, dx
        test   ax, 0x200
        je     loc_6_0FC9
loc_6_0FC1:
        call   sub_6_0FCB
        dec    ecx
        jne    loc_6_0F9D
loc_6_0FC9:
        pop    edx
        ret    
sub_6_0FCB:
        push   ecx
        mov    ecx, 0x3e8
loc_6_0FD1:
        push   ecx
        pop    ecx
        loop   loc_6_0FD1
        pop    ecx
        ret    
loc_6_0FD7:
        push   edi
        push   esi
        mov    edi, ebx
        add    edi, dword ptr [0x188]        ; -> obj1:0x0188
        mov    word ptr [edi + 0x45], ax
        cmp    byte ptr [0x1a2], 8           ; -> loc_1_01A2
        je     loc_6_0FF7
        cmp    byte ptr [0x1a2], 0x32        ; -> loc_1_01A2
        jne    loc_6_100E
loc_6_0FF7:
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        jne    loc_6_100E
        and    al, 0x7f
        cmp    al, 0x13
        ja     loc_6_100E
        pushal 
        call   sub_6_01C5
        popal  
loc_6_100E:
        pop    esi
        pop    edi
        ret    
loc_6_1011:
        mov    ecx, dword ptr [0x184]        ; -> obj1:0x0184
        stc    
        ret    
loc_6_1019:
        mov    edx, 0x10000
        xor    eax, eax
        stc    
        ret    
loc_6_1022:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        add    esi, dword ptr [0x188]        ; -> obj1:0x0188
        mov    edx, 0x1ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xbe
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 0x3e], al
        and    al, 0xf7
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc    
        ret    
loc_6_104D:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        add    esi, dword ptr [0x188]        ; -> obj1:0x0188
        mov    edx, 0x1ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xbe
        mov    ah, byte ptr [esi + 0x3e]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc    
        ret    
loc_6_1071:
        push   edx
        push   eax
        push   ecx
        mov    cl, dl
        mov    ch, al
        mov    edx, 0x1ce
        in     al, dx
        ror    eax, 8
        mov    ax, cx
        and    ax, 0x70f
        shl    al, 1
        shl    ah, 5
        or     ah, al
        mov    al, 0xb2
        out    dx, ax
        mov    ax, cx
        and    ax, 0x1830
        shr    al, 4
        shr    ah, 1
        or     ah, al
        mov    al, 0xae
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0xf0
        or     al, ah
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
        pop    ecx
        pop    eax
        pop    edx
        stc    
        ret    
loc_6_10B7:
        mov    edx, 0x1ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb2
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
        mov    al, 0xae
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        and    ax, 0x1e03
        shr    ah, 1
        shl    al, 4
        or     al, ah
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc    
        ret    
loc_6_10E6:
        mov    edx, 0x1ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb2
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
        mov    al, 0xae
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        and    ax, 0xe00c
        shr    ah, 5
        shl    al, 1
        or     al, ah
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc    
        ret    
VxD__LTEXT ENDS
