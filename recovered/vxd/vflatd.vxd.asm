; VFLATD -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 587 instructions recovered by recursive descent

Declare_Virtual_Device VFLATD, 2, 1, VFLATD_Control, 0011Fh, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h
;   service table: obj1:0x2a0, 2 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x330  R-X
VFLATD_Control:
        clc    
        ret    
sub_1_0002:
        cmp    word ptr [0x302], 0           ; -> obj1:0x0302
        je     loc_1_007B
        mov    eax, dword ptr [0x2f8]        ; -> obj1:0x02F8
        mov    cl, byte ptr [0x14a]          ; -> obj1:0x014A
        shr    eax, cl
        mov    dword ptr [0x14c], eax        ; -> obj1:0x014C
        shl    eax, cl
        movzx  ecx, word ptr [0x302]         ; -> obj1:0x0302
        lsl    edx, ecx
        shr    edx, 0xc
        push   0
        push   0x80
        push   0x92
        push   edx
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        movzx  ecx, word ptr [0x302]         ; -> obj1:0x0302
        push   0
        push   eax
        push   edx
        push   ebx
        push   ecx
        VMMCall  VMM._SetDescriptor              ; cd207c000100
        add    esp, 0x14
        cmp    dword ptr [0x324], 0          ; -> loc_1_0324
        je     loc_1_007B
        mov    edx, dword ptr [0x1ff]        ; -> loc_1_01FF
        mov    ecx, dword ptr [0x304]        ; -> obj1:0x0304
        shr    ecx, 0xc
loc_1_0072:
        mov    byte ptr [edx], 3
        add    edx, 4
        dec    ecx
        jne    loc_1_0072
loc_1_007B:
        ret    
VFLATD_Service_01:
        mov    dword ptr [0x320], esi        ; -> loc_1_0320
sub_1_0082:
        cmp    word ptr [0x302], 0           ; -> obj1:0x0302
        je     loc_1_00BE
        cmp    dword ptr [0x31c], 0          ; -> loc_1_031C
        jne    loc_1_00BE
        pushal 
        call   2
        mov    esi, 0xdc                     ; -> loc_1_00DC
        VMMCall  VMM.Hook_Invalid_Page_Fault     ; cd20e9000100
        mov    esi, 0x15a                    ; -> loc_1_015A
        mov    eax, 0x130                    ; -> loc_1_0130
        sub    eax, esi
        mov    ah, al
        mov    al, 0xeb
        xchg   dword ptr [esi], eax
        mov    dword ptr [0x31c], eax        ; -> loc_1_031C
        popal  
loc_1_00BE:
        ret    
loc_1_00BF:
        cmp    edx, 0x3c4
        je     loc_1_00CD
loc_1_00C7:
        jmp    loc_1_0318
loc_1_00CD:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_00C7
        call   sub_1_0082
        jmp    loc_1_00C7
loc_1_00DC:
        mov    eax, dword ptr [edi]
        cmp    eax, dword ptr [0x133]        ; -> obj1:0x0133
        jne    loc_1_0130
        xor    eax, eax
        xchg   dword ptr [0x31c], eax        ; -> loc_1_031C
        mov    dword ptr [0x15a], eax        ; -> loc_1_015A
        mov    esi, 0xdc                     ; -> loc_1_00DC
        VMMCall  VMM.Unhook_Invalid_Page_Fault   ; cd20ea000100
        mov    esi, dword ptr [0x320]        ; -> loc_1_0320
        or     esi, esi
        je     loc_1_0113
        call   esi
        xor    esi, esi
        mov    dword ptr [0x320], esi        ; -> loc_1_0320
        ret    
loc_1_0113:
        mov    edx, 0x3c4
        mov    ecx, 0
        VMMCall  VMM.Simulate_VM_IO              ; cd2030010100
        mov    ecx, 4
        VMMCall  VMM.Simulate_VM_IO              ; cd2030010100
        clc    
        ret    
loc_1_0130:
        stc    
        ret    
        db  0B8h, 000h, 010h, 000h, 000h, 00Fh, 022h, 0D0h, 05Ah
loc_1_013B:
        pop    eax
loc_1_013C:
        ljmp   0:0
        add    byte ptr [eax + 0xf], dl
        and    al, dl
        shr    eax, 0x10
        sub    eax, 0x12345678
        jl     loc_1_013B
        cmp    eax, 0x12345678
        jge    loc_1_013B
        push   edx
loc_1_015A:
        mov    edx, 0x12345678
        sub    edx, eax
loc_1_0161:
        mov    dword ptr ss:[0x14c], edx     ; -> obj1:0x014C
loc_1_0168:
        shl    edx, 8
        mov    byte ptr ss:[0x12345678], dl
        shr    edx, 8
loc_1_0175:
        mov    byte ptr ss:[0x12345678], dl
        db  036h, 088h, 035h
loc_1_017F:
        js     loc_1_01D7
        xor    al, 0x12
        mov    edx, es
        mov    es, edx
        mov    edx, ds
        mov    ds, edx
loc_1_018B:
        pop    edx
        pop    eax
        add    esp, 4
loc_1_0190:
        iretd  
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_01D7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_01F3:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        mov    edx, ss
        mov    ds, dx
        db  0BAh
loc_1_01FF:
        js     0x257
        xor    al, 0x12
        mov    byte ptr [edx], 3
        mov    byte ptr [edx + 4], 3
        mov    byte ptr [edx + 8], 3
        mov    byte ptr [edx + 0xc], 3
        mov    byte ptr [edx + 0x10], 3
        mov    byte ptr [edx + 0x14], 3
        mov    byte ptr [edx + 0x18], 3
        mov    byte ptr [edx + 0x1c], 3
        mov    byte ptr [edx + 0x20], 3
        mov    byte ptr [edx + 0x24], 3
        mov    byte ptr [edx + 0x28], 3
        mov    byte ptr [edx + 0x2c], 3
        mov    byte ptr [edx + 0x30], 3
        mov    byte ptr [edx + 0x34], 3
        mov    byte ptr [edx + 0x38], 3
        mov    byte ptr [edx + 0x3c], 3
        mov    edx, eax
        shl    edx, 6
        add    edx, 0x12345678
        mov    byte ptr [edx], 7
        mov    byte ptr [edx + 4], 7
        mov    byte ptr [edx + 8], 7
        mov    byte ptr [edx + 0xc], 7
        mov    byte ptr [edx + 0x10], 7
        mov    byte ptr [edx + 0x14], 7
        mov    byte ptr [edx + 0x18], 7
        mov    byte ptr [edx + 0x1c], 7
        mov    byte ptr [edx + 0x20], 7
        mov    byte ptr [edx + 0x24], 7
        mov    byte ptr [edx + 0x28], 7
        mov    byte ptr [edx + 0x2c], 7
        mov    byte ptr [edx + 0x30], 7
        mov    byte ptr [edx + 0x34], 7
        mov    byte ptr [edx + 0x38], 7
        mov    byte ptr [edx + 0x3c], 7
        mov    dword ptr [0x1ff], edx        ; -> loc_1_01FF
        mov    edx, 0x12345678
        mov    cr3, edx
        pop    ds
        jmp    loc_1_018B
        dd  VFLATD_Service_00        ; fixup
        dd  VFLATD_Service_01        ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0011Fh                      ; DDB_Req_Device_Number
        db  2, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VFLATD  '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  VFLATD_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  VFLATD_PM_API            ; DDB_PM_API_Proc
        dd  obj1:0x02A0              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_1_0314:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0318:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_031C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0320:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0324:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0328:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x607  R-X
VFLATD_Service_00:
        mov    eax, 0x201
        xor    ecx, ecx
        ret    
        ; ---- dispatch table (7 entries) ----
        dd  loc_2_0040
        dd  loc_2_006D
        dd  loc_2_047C
        dd  loc_2_00FD
        dd  loc_2_0098
        dd  loc_2_0486
        dd  loc_2_0522
VFLATD_PM_API:
        movzx  eax, byte ptr [ebp + 0x14]
        cmp    eax, 6
        ja     loc_2_0036
        jmp    dword ptr [eax*4 + 8]         ; -> obj2:0x0008
loc_2_0036:
        or     dword ptr [ebp + 0x2c], 1
        ret    
loc_2_003B:
        and    dword ptr [ebp + 0x2c], 0xfffffffe
        ret    
loc_2_0040:
        mov    dword ptr [ebp + 0x1c], 0x201
        movzx  eax, word ptr [0x302]         ; -> obj1:0x0302
        mov    dword ptr [ebp + 0x10], eax
        xor    eax, eax
        cmp    dword ptr [0x324], eax        ; -> loc_1_0324
        je     loc_2_0060
        mov    eax, dword ptr [0x2f8]        ; -> obj1:0x02F8
loc_2_0060:
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [0x2fc]        ; -> obj1:0x02FC
        mov    dword ptr [ebp + 0x18], eax
        jmp    loc_2_003B
loc_2_006D:
        mov    edx, 0
        mov    ebx, 0x10000
        mov    esi, 0xa0000
        movzx  eax, word ptr [ebp + 0x1c]
        movzx  ecx, word ptr [ebp + 0x18]
        xchg   edi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        shl    eax, 0xc
        jmp    loc_2_011A
loc_2_0098:
        movzx  edx, byte ptr [ebp + 0x15]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp + 0x18]
        add    ecx, 0xfff
        and    ecx, 0xfffff000
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        mov    dword ptr [ebp + 0x14], eax
        mov    ecx, dword ptr [ebp + 0x18]
        add    ecx, 0xfff
        shr    ecx, 0xc
        dec    ecx
        push   0
        push   0x80
        push   0x92
        push   ecx
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        mov    dword ptr [ebp + 0x1c], eax
        mov    word ptr [0x302], ax          ; -> obj1:0x0302
        jmp    loc_2_003B
loc_2_00FD:
        movzx  edx, byte ptr [ebp + 0x15]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ebx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 4]
        movzx  ecx, word ptr [ebp + 0x18]
        xchg   edi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
loc_2_011A:
        call   sub_2_012A
        mov    dword ptr [ebp + 0x1c], eax
        mov    dword ptr [ebp + 0x14], edx
        jmp    loc_2_003B
sub_2_012A:
        push   eax
        push   ebx
        push   esi
        push   ecx
        push   edi
        push   edx
        cmp    dword ptr [esp + 8], 0x64
        ja     loc_2_03D4
        cmp    word ptr [0x302], 0           ; -> obj1:0x0302
        jne    loc_2_036A
        mov    eax, dword ptr [esp]
        test   eax, eax
        jne    loc_2_03D4
        mov    word ptr [0x300], ax          ; -> obj1:0x0300
        mov    eax, cr3
        mov    dword ptr [0x293], eax        ; -> obj1:0x0293
        add    dword ptr [esp + 0x14], 0xfff
        shr    dword ptr [esp + 0x14], 0xc
        mov    eax, dword ptr [esp + 0x10]
        test   eax, 0xfff
        jne    loc_2_03D4
        mov    dword ptr [0x304], eax        ; -> obj1:0x0304
        bsf    ecx, eax
        shr    eax, 0xc
        mov    dword ptr [esp + 0x10], eax
        mov    byte ptr [0x14a], cl          ; -> obj1:0x014A
        sub    cl, 8
        mov    byte ptr [0x16a], cl          ; -> obj1:0x016A
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [0x308], eax        ; -> obj1:0x0308
        mov    eax, dword ptr [esp + 0x14]
        add    eax, 0xf
        and    al, 0xf0
        cmp    eax, 0x400
        jae    loc_2_01BA
        mov    eax, 0x400
loc_2_01BA:
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [0x2fc], eax        ; -> obj1:0x02FC
        shl    dword ptr [0x2fc], 0xc        ; -> obj1:0x02FC
        lea    eax, [eax + eax]
        push   eax
        push   9
        push   eax
        push   0x80080000
        VMMCall  VMM._PageReserve                ; cd201d010100
        add    esp, 0xc
        pop    ecx
        or     eax, eax
        je     loc_2_03D4
        mov    esi, eax
        mov    dword ptr [0x2f8], eax        ; -> obj1:0x02F8
        call   sub_2_03DC
        je     loc_2_03D4
        mov    esi, dword ptr [0x2f8]        ; -> obj1:0x02F8
        add    esi, dword ptr [0x2fc]        ; -> obj1:0x02FC
        add    dword ptr [0x133], esi        ; -> obj1:0x0133
        mov    ebx, dword ptr [0x304]        ; -> obj1:0x0304
        shr    esi, 0xc
        shr    ebx, 0xc
        push   0
        push   ebx
        push   esi
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_2_03D4
        mov    eax, dword ptr [0x2f8]        ; -> obj1:0x02F8
        mov    ecx, dword ptr [esp + 0x14]
        dec    ecx
        push   0
        push   0x80
        push   0x92
        push   ecx
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        or     eax, eax
        je     loc_2_03D4
        mov    word ptr [0x302], ax          ; -> obj1:0x0302
        mov    edx, dword ptr [0x2f8]        ; -> obj1:0x02F8
        mov    cl, byte ptr [0x14a]          ; -> obj1:0x014A
        shr    edx, cl
        mov    dword ptr [0x14c], edx        ; -> obj1:0x014C
        mov    esi, dword ptr [esp + 0x14]
        shl    esi, 0xc
        shr    esi, cl
        mov    dword ptr [0x153], esi        ; -> obj1:0x0153
        add    edx, esi
        mov    dword ptr [0x15b], edx        ; -> obj1:0x015B
        sgdt   [0x30e]                       ; -> obj1:0x030E
        mov    edx, dword ptr [0x310]        ; -> obj1:0x0310
        and    eax, 0xfff8
        add    edx, eax
        add    edx, 3
        mov    dword ptr [0x16e], edx        ; -> obj1:0x016E
        add    edx, 1
        mov    dword ptr [0x178], edx        ; -> obj1:0x0178
        add    edx, 3
        mov    dword ptr [0x17f], edx        ; -> loc_1_017F
        pushfd 
        cli    
        sub    esp, 8
        sidt   [esp + 2]
        pop    eax
        pop    eax
        add    eax, 0x70
        mov    bx, cs
        mov    esi, 0x144                    ; -> obj1:0x0144
        xchg   word ptr [eax + 2], bx
        xchg   word ptr [eax], si
        ror    esi, 0x10
        xchg   word ptr [eax + 6], si
        ror    esi, 0x10
        mov    ax, cs
        cmp    bx, ax
        jne    loc_2_0300
        mov    byte ptr [0x13c], 0xe9        ; -> loc_1_013C
        sub    esi, 0x141                    ; -> obj1:0x0141
loc_2_0300:
        mov    dword ptr [0x13d], esi        ; -> obj1:0x013D
        mov    word ptr [0x141], bx          ; -> obj1:0x0141
        popfd  
        mov    eax, 0x10076
        mov    esi, 0x59b                    ; -> loc_2_059B
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    dword ptr [0x314], esi        ; -> sub_1_0314
        mov    eax, 0x10098
        mov    esi, 0xbf                     ; -> loc_1_00BF
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    dword ptr [0x318], esi        ; -> loc_1_0318
        mov    word ptr [0x30e], 0           ; -> obj1:0x030E
        cmp    dword ptr [0x304], 0x10000    ; -> obj1:0x0304
        jne    loc_2_036A
        mov    edi, 0x168                    ; -> loc_1_0168
        mov    esi, 0x175                    ; -> loc_1_0175
        mov    ecx, 7
        rep movsd dword ptr es:[edi], dword ptr [esi]
        sub    dword ptr [0x29c], 0xd        ; -> obj1:0x029C
loc_2_036A:
        xor    edx, edx
        cmp    dword ptr [0x304], 0x10000    ; -> obj1:0x0304
        jne    loc_2_037D
        mov    edx, 0xfffffff3
loc_2_037D:
        mov    esi, 0x190                    ; -> loc_1_0190
        add    esi, edx
        mov    edi, dword ptr [esp + 8]
        movzx  ecx, word ptr [0x30e]         ; -> obj1:0x030E
        mov    word ptr [0x30e], di          ; -> obj1:0x030E
        add    esi, ecx
        sub    edi, ecx
        je     loc_2_03A9
        jl     loc_2_03C9
        add    edi, esi
        std    
loc_2_03A1:
        mov    ecx, 6
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cld    
loc_2_03A9:
        mov    esi, dword ptr [esp + 4]
        mov    edi, 0x18b                    ; -> loc_1_018B
        add    edi, edx
        mov    ecx, dword ptr [esp + 8]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  eax, word ptr [0x302]         ; -> obj1:0x0302
        mov    edx, dword ptr [0x2f8]        ; -> obj1:0x02F8
        jmp    loc_2_03D8
loc_2_03C9:
        sub    esi, 5
        add    edi, esi
        cld    
        jmp    loc_2_03A1
loc_2_03D4:
        xor    eax, eax
        mov    edx, eax
loc_2_03D8:
        add    esp, 0x18
        ret    
sub_2_03DC:
        pushal 
        mov    esi, dword ptr [0x2f8]        ; -> obj1:0x02F8
        mov    edi, esi
        add    esi, dword ptr [0x2fc]        ; -> obj1:0x02FC
        mov    ebx, dword ptr [0x304]        ; -> obj1:0x0304
        shr    esi, 0xc
        shr    edi, 0xc
        shr    ebx, 0xc
loc_2_03FA:
        mov    eax, dword ptr [0x308]        ; -> obj1:0x0308
        shr    eax, 0xc
        push   0x40020000
        push   eax
        push   ebx
        push   edi
        VMMCall  VMM._PageCommitPhys             ; cd2028010100
        add    esp, 0x10
        or     eax, eax
        je     loc_2_0438
        add    edi, ebx
        cmp    edi, esi
        jb     loc_2_03FA
        mov    eax, dword ptr [0x308]        ; -> obj1:0x0308
        shr    eax, 0xc
        push   0x40060000
        push   eax
        push   ebx
        push   esi
        VMMCall  VMM._PageCommitPhys             ; cd2028010100
        add    esp, 0x10
        or     eax, eax
loc_2_0438:
        popal  
        ret    
sub_2_043A:
        mov    eax, cr3
        push   0
        push   0x1000
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        mov    edi, eax
        mov    eax, esi
        shr    eax, 0xa
        mov    edi, dword ptr [edi + eax*4]
        and    edi, 0xfffff000
        push   0
        push   0x1000
        push   edi
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        mov    edi, eax
        mov    eax, esi
        and    eax, 0x3ff
        lea    edi, [edi + eax*4]
        ret    
loc_2_047C:
        call   sub_1_0002
        jmp    loc_2_003B
loc_2_0486:
        cmp    word ptr [0x302], 0           ; -> obj1:0x0302
        je     loc_2_0036
        cmp    dword ptr [0x2fc], 0x400000   ; -> obj1:0x02FC
        ja     loc_2_0036
        cmp    dword ptr [0x304], 0x10000    ; -> obj1:0x0304
        jne    loc_2_0036
        inc    dword ptr [0x324]             ; -> loc_1_0324
        mov    eax, dword ptr [0x324]        ; -> loc_1_0324
        mov    dword ptr [ebp + 0x1c], eax
        cmp    eax, 1
        ja     loc_2_003B
        mov    esi, dword ptr [0x2f8]        ; -> obj1:0x02F8
        mov    eax, dword ptr [0x2fc]        ; -> obj1:0x02FC
        shr    esi, 0xc
        shr    eax, 0xc
        push   0
        push   eax
        push   esi
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        call   sub_2_043A
        mov    dword ptr [0x249], edi        ; -> obj1:0x0249
        mov    dword ptr [0x1ff], edi        ; -> loc_1_01FF
        call   sub_1_0002
        mov    esi, 0x161                    ; -> loc_1_0161
        mov    eax, 0x1f3                    ; -> loc_1_01F3
        sub    eax, esi
        mov    dl, 0xe9
        xchg   byte ptr [esi], dl
        xchg   dword ptr [esi + 1], eax
        mov    byte ptr [0x328], dl          ; -> loc_1_0328
        mov    dword ptr [0x329], eax        ; -> obj1:0x0329
        jmp    loc_2_003B
loc_2_0522:
        cmp    word ptr [0x302], 0           ; -> obj1:0x0302
        je     loc_2_0036
        cmp    dword ptr [0x324], 0          ; -> loc_1_0324
        je     loc_2_0036
        dec    dword ptr [0x324]             ; -> loc_1_0324
        mov    eax, dword ptr [0x324]        ; -> loc_1_0324
        mov    dword ptr [ebp + 0x1c], eax
        jne    loc_2_003B
        inc    dword ptr [0x324]             ; -> loc_1_0324
        call   sub_1_0002
        dec    dword ptr [0x324]             ; -> loc_1_0324
        mov    esi, dword ptr [0x2f8]        ; -> obj1:0x02F8
        mov    eax, dword ptr [0x2fc]        ; -> obj1:0x02FC
        shr    esi, 0xc
        shr    eax, 0xc
        push   0
        push   eax
        push   esi
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        mov    eax, dword ptr [0x329]        ; -> obj1:0x0329
        mov    dl, byte ptr [0x328]          ; -> loc_1_0328
        mov    byte ptr [0x161], dl          ; -> loc_1_0161
        mov    dword ptr [0x162], eax        ; -> obj1:0x0162
        jmp    loc_2_003B
loc_2_059B:
        pop    eax
        pop    ecx
        pop    edx
        xchg   dword ptr [esp], eax
        push   eax
        push   edx
        push   ecx
        call   sub_1_0314
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [esp], ecx
        or     eax, eax
        je     loc_2_0606
        push   eax
        sub    esp, 8
        sgdt   [esp + 2]
        pop    eax
        pop    eax
        cmp    eax, dword ptr [0x310]        ; -> obj1:0x0310
        je     loc_2_0605
        mov    dword ptr [0x310], eax        ; -> obj1:0x0310
        movzx  ecx, word ptr [0x302]         ; -> obj1:0x0302
        and    cl, 0xf8
        lea    eax, [ecx + eax + 3]
        mov    ecx, 0xfffffff3
        cmp    dword ptr [0x304], 0x10000    ; -> obj1:0x0304
        je     loc_2_05F3
        mov    dword ptr [0x16e], eax        ; -> obj1:0x016E
        xor    ecx, ecx
loc_2_05F3:
        add    eax, 1
        mov    dword ptr [ecx + 0x178], eax  ; -> obj1:0x0178
        add    eax, 3
        mov    dword ptr [ecx + 0x17f], eax  ; -> loc_1_017F
loc_2_0605:
        pop    eax
loc_2_0606:
        ret    
VxD__LTEXT ENDS
