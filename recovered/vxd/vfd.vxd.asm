; VFD -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 191 instructions recovered by recursive descent

Declare_Virtual_Device VFD, 2, 0, VFD_Control, 0001Bh, Undefined_Init_Order
;   DDK version 0400h   init_order 0A0060000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1f0  R-X
        db  000h, 000h, 000h, 000h
VFD_Control:
        cmp    dword ptr [0], 0              ; -> obj1:0x0000
        jne    loc_1_001F
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 0x1b
        je     loc_2_0000
loc_1_001F:
        cmp    eax, 6
        je     loc_1_0026
        clc    
        ret    
loc_1_0026:
        cmp    byte ptr [0x1c8], 0xff        ; -> obj1:0x01C8
        jne    loc_1_0041
        in     al, 0x86
        mov    ah, al
        and    ah, 0xc0
        cmp    ah, 0x40
        jne    loc_1_0041
        or     al, 0xc0
        jmp    loc_1_003F
loc_1_003F:
        out    0x86, al
loc_1_0041:
        clc    
        ret    
        db  0B8h, 000h, 004h, 000h, 000h, 0F8h, 0C3h, 08Bh, 0C0h
loc_1_004C:
        test   byte ptr [ebp + 0x14], 0x80
        jne    loc_1_0092
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        VxDCall  VTD.VTD_Disable_Trapping        ; cd2005000500
        movzx  eax, word ptr [ebp + 0x1c]
        mov    esi, dword ptr [0x1bc]        ; -> obj1:0x01BC
        mov    word ptr [ebx + esi], ax
        cmp    ah, 2
        jne    loc_1_0085
        cmp    dword ptr [0x1c0], 0          ; -> obj1:0x01C0
        jne    loc_1_0085
        mov    word ptr [ebx + esi], 0
loc_1_0085:
        xor    eax, eax
        mov    esi, 0x94                     ; -> loc_1_0094
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
loc_1_0092:
        stc    
        ret    
loc_1_0094:
        VxDCall  VTD.VTD_Enable_Trapping         ; cd2006000500
        mov    eax, dword ptr [0x1bc]        ; -> obj1:0x01BC
        mov    word ptr [ebx + eax], 0
        test   byte ptr [ebp + 0x2c], 1
        je     loc_1_00CA
        mov    eax, dword ptr [0x1c4]        ; -> obj1:0x01C4
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        test   dl, 1
        jne    loc_1_00CA
        or     dl, 1
        VxDCall  VDMAD.VDMAD_Set_Virt_State      ; cd2005000400
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012000400
loc_1_00CA:
        VMMCall  VMM.End_Critical_Section        ; cd2020800100
loc_1_00D0:
        pushal 
        mov    edi, dword ptr [0x1bc]        ; -> obj1:0x01BC
        add    edi, ebx
        movzx  ecx, word ptr [edi]
        jecxz  loc_1_012B
        movzx  edi, ch
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        cmp    edi, 5
        je     loc_1_010D
        cmp    edi, 4
        je     loc_1_0132
        cmp    edi, 2
        jne    loc_1_012B
        test   dl, 0xc
        jne    loc_1_012B
        mov    edi, esi
        sub    edi, dword ptr [ebx + 4]
        cmp    edi, 0xe0000
        jb     loc_1_012B
        or     dl, 4
        jmp    loc_1_0125
loc_1_010D:
        mov    edi, dword ptr [0x78]
        movzx  ecx, di
        shr    edi, 0x10
        shl    edi, 4
        add    edi, ecx
        movzx  ecx, byte ptr [edi + 4]
        shl    ecx, 2
loc_1_0125:
        VxDCall  VDMAD.VDMAD_Set_Virt_State      ; cd2005000400
loc_1_012B:
        popal  
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012800400
loc_1_0132:
        test   dl, 1
        jne    loc_1_0155
        push   ebx
        push   edx
        xor    bl, bl
        mov    bh, 0xff
        xor    edx, edx
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        pop    edx
        pop    ebx
        VxDCall  VDMAD.VDMAD_Set_Phys_State      ; cd2006000400
        VxDCall  VDMAD.VDMAD_UnMask_Channel      ; cd2008000400
        jmp    loc_1_0169
loc_1_0155:
        xor    ebx, ebx
        xor    esi, esi
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        VxDCall  VDMAD.VDMAD_Mask_Channel        ; cd2007000400
loc_1_0169:
        popal  
        ret    
        db  000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0001Bh                      ; DDB_Req_Device_Number
        db  2, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VFD     '                  ; DDB_Name
        dd  0A0060000h                  ; DDB_Init_Order
        dd  VFD_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_01CC:
        dec    eax
        dec    ecx
        inc    edi
        dec    eax
        inc    esi
        dec    esp
        dec    edi
        push   eax
        push   eax
        pop    ecx
        push   edx
        inc    ebp
        inc    ecx
        inc    esp
        push   ebx
        db  000h
loc_1_01DC:
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        inc    ecx
        push   ecx
        inc    esi
        inc    ecx
        push   ebx
        push   esp
        inc    esi
        dec    esp
        dec    edi
        push   eax
        push   eax
        pop    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xcc  R-X
loc_2_0000:
        inc    dword ptr [0]                 ; -> obj1:0x0000
        VxDCall  VFBACKUP.VFBACKUP_Get_Version   ; cd2000003600
        jb     loc_2_00CA
        VxDCall  VFBACKUP.VFBACKUP_Register_VFD  ; cd2004003600
        jb     loc_2_00CA
        mov    eax, 0x13
        mov    esi, 0x4c                     ; -> loc_1_004C
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 2
        mov    esi, 0xd0                     ; -> loc_1_00D0
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [0x1c4], eax        ; -> obj1:0x01C4
        push   ebx
        push   0
        push   2
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    loc_2_005A
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_005A:
        mov    dword ptr [0x1bc], eax        ; -> obj1:0x01BC
        pop    ebx
        or     eax, 0xffffffff
        xor    esi, esi
        mov    edi, 0x1cc                    ; -> loc_1_01CC
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        mov    dword ptr [0x1c0], eax        ; -> obj1:0x01C0
        mov    eax, 6
        xor    ebx, ebx
        VxDCall  VPICD.VPICD_Force_Default_Owner ; cd2010000300
        or     eax, 0xffffffff
        xor    esi, esi
        mov    edi, 0x1dc                    ; -> loc_1_01DC
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        test   eax, eax
        je     loc_2_00C9
        cmp    dword ptr [0xfffe8], 0x4f433330
        jne    loc_2_00C9
        cmp    dword ptr [0xfffec], 0x5141504d
        jne    loc_2_00C9
        in     al, 0x86
        mov    ah, al
        and    ah, 0xc0
        cmp    ah, 0xc0
        jne    loc_2_00C9
        and    al, 0x3f
        or     al, 0x40
        jmp    loc_2_00C0
loc_2_00C0:
        out    0x86, al
        mov    byte ptr [0x1c8], 0xff        ; -> obj1:0x01C8
loc_2_00C9:
        clc    
loc_2_00CA:
        ret    
        db  020h
VxD__LTEXT ENDS
