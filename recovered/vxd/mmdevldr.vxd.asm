; mmdevldr -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 2024 instructions recovered by recursive descent

Declare_Virtual_Device mmdevldr, 1, 0, mmdevldr_Control, 0044Ah, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h
;   service table: obj1:0x0, 6 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x90  R-X
        dd  MMDEVLDR_Register_Device_Driver ; fixup
        dd  _MMDEVLDR_SetDevicePresence ; fixup
        dd  _MMDEVLDR_SetEnvironmentString ; fixup
        dd  _MMDEVLDR_GetEnvironmentString ; fixup
        dd  _MMDEVLDR_RemoveEnvironmentString ; fixup
        dd  _MMDEVLDR_AddEnvironmentString ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0044Ah                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'mmdevldr'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  mmdevldr_Control         ; DDB_Control_Proc
        dd  mmdevldr_PM_API          ; DDB_V86_API_Proc
        dd  mmdevldr_PM_API          ; DDB_PM_API_Proc
        dd  obj1:0x0000              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
mmdevldr_Control:
        db  083h
        db  0F8h, 023h, 00Fh, 084h
        dd  loc_3_015A               ; fixup
        db  083h, 0F8h, 01Bh, 00Fh, 084h
        dd  loc_5_0000               ; fixup
        db  083h, 0F8h, 01Ch, 00Fh, 084h
        dd  loc_3_0029               ; fixup
        db  083h, 0F8h, 022h, 00Fh, 084h
        dd  loc_3_0032               ; fixup
        db  0F8h, 0C3h, 0CCh, 0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xea  R-X
sub_2_0000:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        VMMCall  VMM.List_Create                 ; cd209b000100
        mov    eax, esi
        pop    esi
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_2_0014:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_0024:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_0034:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_0044:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_2_004C:
        VxDCall  CONFIGMG._CONFIGMG_Load_DLVxDs  ; cd202f803300
        int3   
        int3   
sub_2_0054:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_2_005C:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        int3   
        int3   
sub_2_0064:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        int3   
        int3   
sub_2_006C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_007C:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Allocate               ; cd209d000100
        mov    esi, ecx
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_2_0090:
        VxDCall  CONFIGMG._CONFIGMG_Get_DDBs     ; cd2030803300
        int3   
        int3   
sub_2_0098:
        VMMCall  VMM._RegEnumKey                 ; cd204c810100
        int3   
        int3   
sub_2_00A0:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        int3   
        int3   
sub_2_00A8:
        VMMCall  VMM._RegEnumValue               ; cd2050810100
        int3   
        int3   
sub_2_00B0:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        int3   
        int3   
sub_2_00B8:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        int3   
        int3   
sub_2_00C0:
        VMMCall  VMM._RegFlushKey                ; cd2055810100
        int3   
        int3   
sub_2_00C8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_00D8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x1581  R-X
loc_3_0000:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_3_1059
        add    esp, 0x14
        leave  
        ret    
MMDEVLDR_Register_Device_Driver:
        push   ecx
        push   ebx
        push   eax
        call   sub_3_0799
        add    esp, 0xc
        clc    
        ret    
loc_3_0029:
        call   sub_3_058F
        xor    eax, eax
        clc    
        ret    
loc_3_0032:
        xor    eax, eax
        mov    ecx, 0x1000
        push   eax
        push   eax
        push   ebx
        push   eax
        push   ecx
        call   sub_3_1059
        add    esp, 0x14
        cmp    eax, 1
        ja     loc_3_0063
        push   3
        push   edx
        push   0                             ; -> loc_3_0000
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e003300
        add    esp, 0x10
        inc    dword ptr [0]                 ; -> obj4:0x003C
loc_3_0063:
        stc    
        ret    
sub_3_0065:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        lea    eax, [esp]
        push   eax
        push   8
        push   0                             ; -> obj4:0x0000
        push   0                             ; -> obj4:0x0010
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        add    esp, 8
        movzx  eax, ax
        pop    ebx
        leave  
        ret    
loc_3_0091:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_3_0821
        add    esp, 4
        leave  
        ret    
sub_3_00A1:
        push   ebx
        VxDCall  SHELL._SHELL_QueryAppyTimeAvailable ; cd200d001700
        or     eax, eax
        je     loc_3_00AE
        jmp    loc_3_00B4
loc_3_00AE:
        clc    
        jmp    loc_3_0158
loc_3_00B4:
        mov    eax, 0                        ; -> obj4:0x0010
        push   0
        push   0
        push   0x92
        push   0x20
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [0], ebx            ; -> obj4:0x0034
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        VMMCall  VMM._Allocate_LDT_Selector      ; cd2078000100
        add    esp, 0x14
        or     edx, eax
        je     loc_3_00F0
        jmp    loc_3_00F0
loc_3_00F0:
        mov    dword ptr [0], eax            ; -> obj4:0x0038
        shl    eax, 0x10
        push   eax
        lea    eax, [esp]
        push   eax
        push   4
        push   0                             ; -> obj4:0x0024
        push   0                             ; -> obj4:0x001D
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        add    esp, 4
        movzx  eax, ax
        or     eax, eax
        je     loc_3_0141
        xor    eax, eax
        mov    ebx, 3
        push   eax
        push   ebx
        lea    eax, [esp]
        push   eax
        push   8
        push   0                             ; -> obj4:0x0000
        push   0                             ; -> obj4:0x0010
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        add    esp, 8
loc_3_0141:
        push   0
        push   dword ptr [0]                 ; -> obj4:0x0038
        push   dword ptr [0]                 ; -> obj4:0x0034
        VMMCall  VMM._Free_LDT_Selector          ; cd2079000100
        add    esp, 0xc
loc_3_0158:
        pop    ebx
        ret    
loc_3_015A:
        jecxz  loc_3_019D
        dec    ecx
        je     loc_3_01C0
        dec    ecx
        je     loc_3_01F9
        dec    ecx
        je     loc_3_0227
        dec    ecx
        je     loc_3_0270
        dec    ecx
        je     loc_3_02A9
        dec    ecx
        je     loc_3_0246
        dec    ecx
        je     loc_3_02CE
        dec    ecx
        je     loc_3_0326
        dec    ecx
        je     loc_3_035B
        mov    eax, 1
        ret    
loc_3_019D:
        push   edi
        push   esi
        cmp    dword ptr [esi + 0x1c], 4
        jb     loc_3_01BB
        mov    edi, dword ptr [esi + 0x20]
        mov    esi, dword ptr [esi + 0x18]
        mov    dword ptr [esi], 0x100
        or     edi, edi
        je     loc_3_01BB
        mov    dword ptr [edi], 4
loc_3_01BB:
        pop    esi
        pop    edi
        xor    eax, eax
        ret    
loc_3_01C0:
        push   edi
        push   esi
        cmp    dword ptr [esi + 0x14], 0xc
        jb     loc_3_01F4
        cmp    dword ptr [esi + 0x1c], 4
        jb     loc_3_01F4
        mov    edi, dword ptr [esi + 0x20]
        mov    eax, dword ptr [esi + 0x10]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        mov    esi, dword ptr [esi + 0x18]
        mov    dword ptr [esi], eax
        or     edi, edi
        je     loc_3_01F4
        mov    dword ptr [edi], 4
loc_3_01F4:
        pop    esi
        pop    edi
        xor    eax, eax
        ret    
loc_3_01F9:
        push   edi
        push   esi
        cmp    dword ptr [esi + 0x14], 0xc
        jb     loc_3_0222
        mov    edi, dword ptr [esi + 0x20]
        mov    eax, dword ptr [esi + 0x10]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        or     edi, edi
        je     loc_3_0222
        mov    dword ptr [edi], 0
loc_3_0222:
        pop    esi
        pop    edi
        xor    eax, eax
        ret    
loc_3_0227:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, edi
        mov    edi, dword ptr [esi + 0x18]
        mov    dword ptr [edi], eax
        mov    edi, dword ptr [esi + 0x20]
        or     edi, edi
        je     loc_3_0242
        mov    dword ptr [edi], 4
loc_3_0242:
        pop    edi
        xor    eax, eax
        ret    
loc_3_0246:
        push   edi
        cmp    dword ptr [esi + 0x14], 4
        jb     loc_3_026C
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_3_026C
        VxDCall  NDIS.Service_0014h              ; cd2014002a00
        mov    edi, dword ptr [esi + 0x18]
        mov    dword ptr [edi], eax
        mov    edi, dword ptr [esi + 0x20]
        or     edi, edi
        je     loc_3_0242
        mov    dword ptr [edi], 4
loc_3_026C:
        pop    edi
        xor    eax, eax
        ret    
loc_3_0270:
        push   edi
        push   esi
        cmp    dword ptr [esi + 0x14], 0xc
        jb     loc_3_02A4
        cmp    dword ptr [esi + 0x1c], 4
        jb     loc_3_02A4
        mov    edi, dword ptr [esi + 0x20]
        mov    eax, dword ptr [esi + 0x10]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        mov    esi, dword ptr [esi + 0x18]
        mov    dword ptr [esi], eax
        or     edi, edi
        je     loc_3_02A4
        mov    dword ptr [edi], 4
loc_3_02A4:
        pop    esi
        pop    edi
        xor    eax, eax
        ret    
loc_3_02A9:
        push   edi
        push   esi
        cmp    dword ptr [esi + 0x1c], 4
        jb     loc_3_02C9
        mov    edi, dword ptr [esi + 0x20]
        mov    esi, dword ptr [esi + 0x18]
        lea    eax, [0]                      ; -> obj4:0x003C
        mov    dword ptr [esi], eax
        or     edi, edi
        je     loc_3_02C9
        mov    dword ptr [edi], 4
loc_3_02C9:
        pop    esi
        pop    edi
        xor    eax, eax
        ret    
loc_3_02CE:
        push   edi
        push   esi
        cmp    dword ptr [esi + 0x14], 8
        jb     loc_3_0321
        cmp    dword ptr [esi + 0x1c], 0x10
        jb     loc_3_0321
        mov    edi, dword ptr [esi + 0x20]
        mov    eax, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        or     eax, 2
        sub    esp, 4
        mov    ebx, esp
        push   eax
        push   ebx
        push   0xfffff
        push   0
        push   0xf
        push   0
        push   1
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        pop    ebx
        mov    esi, dword ptr [esi + 0x18]
        mov    dword ptr [esi], edx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        or     edi, edi
        je     loc_3_0321
        mov    dword ptr [edi], 4
loc_3_0321:
        pop    esi
        pop    edi
        xor    eax, eax
        ret    
loc_3_0326:
        push   edi
        push   esi
        cmp    dword ptr [esi + 0x14], 4
        jb     loc_3_0356
        cmp    dword ptr [esi + 0x1c], 4
        jb     loc_3_0356
        mov    edi, dword ptr [esi + 0x20]
        mov    eax, dword ptr [esi + 0x10]
        push   0
        push   dword ptr [eax]
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        mov    esi, dword ptr [esi + 0x18]
        mov    dword ptr [esi], eax
        or     edi, edi
        je     loc_3_0356
        mov    dword ptr [edi], 4
loc_3_0356:
        pop    esi
        pop    edi
        xor    eax, eax
        ret    
loc_3_035B:
        push   edi
        push   esi
        cmp    dword ptr [esi + 0x1c], 4
        jb     loc_3_03D1
        mov    edi, dword ptr [esi + 0x20]
        mov    dword ptr [edi], 4
        mov    eax, dword ptr [esi + 0x10]
        lea    ebx, [ebp - 4]
        push   0
        push   eax
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Locate_DevNode ; cd2002003300
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_3_03C7
        lea    eax, [ebp - 8]
        lea    ebx, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        push   0
        push   ecx
        push   ebx
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Status ; cd2036003300
        add    esp, 0x10
        cmp    eax, 0
        jne    loc_3_03C7
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ebp - 0xc]
        test   eax, 8
        jne    loc_3_03B9
        cmp    ebx, 0x16
        je     loc_3_03C0
        mov    eax, 2
        jmp    loc_3_03CC
loc_3_03B9:
        mov    eax, 1
        jmp    loc_3_03CC
loc_3_03C0:
        mov    eax, 3
        jmp    loc_3_03CC
loc_3_03C7:
        mov    eax, 0
loc_3_03CC:
        mov    esi, dword ptr [esi + 0x18]
        mov    dword ptr [esi], eax
loc_3_03D1:
        pop    esi
        pop    edi
        xor    eax, eax
        ret    
sub_3_03D6:
        push   ebp
        mov    ebp, esp
        push   -1
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        leave  
        ret    
sub_3_03EC:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        VMMCall  VMM._lstrlen                    ; cd207c010100
        add    esp, 4
        leave  
        ret    
sub_3_03FD:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        VMMCall  VMM._lstrlen                    ; cd207c010100
        add    esp, 4
        push   -1
        push   dword ptr [ebp + 0xc]
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        leave  
        ret    
mmdevldr_PM_API:
        movzx  ebx, word ptr [ebp + 0x14]
        mov    eax, 0xb
        cmp    ebx, 7
        jae    loc_3_04DD
        cmp    ebx, 3
        je     loc_3_04EE
        cmp    ebx, 4
        je     loc_3_0569
        cmp    ebx, 0
        je     loc_3_04BD
        cmp    ebx, 1
        je     loc_3_04BD
        cmp    ebx, 5
        je     loc_3_050B
        cmp    ebx, 6
        je     loc_3_054D
        cmp    ebx, 2
        jne    loc_3_04DD
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    esi, eax
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x3c]
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    ecx, eax
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   ecx
        push   eax
        push   dword ptr [esi]
        call   sub_3_0869
        add    esp, 0xc
        pop    dword ptr [ebp + 0x3c]
        pop    dword ptr [ebp + 0x14]
        jmp    loc_3_04DD
loc_3_04BD:
        push   0
        push   0
        push   0                             ; -> loc_3_0091
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_3_04DB
        mov    eax, 0xb
        jmp    loc_3_04DD
loc_3_04DB:
        xor    eax, eax
loc_3_04DD:
        mov    dx, ax
        sub    ax, 1
        cmc    
        lahf   
        mov    byte ptr [ebp + 0x2c], ah
        mov    word ptr [ebp + 0x1c], dx
        ret    
loc_3_04EE:
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        jmp    loc_3_04DD
loc_3_050B:
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        sub    esp, 4
        mov    ebx, esp
        push   dword ptr [eax]
        push   ebx
        push   dword ptr [eax + 4]
        push   0
        push   0xf
        push   0
        push   1
        push   dword ptr [eax + 8]
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        pop    ebx
        mov    ecx, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], edx
        xor    eax, eax
        jmp    loc_3_04DD
loc_3_054D:
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   0
        push   dword ptr [eax]
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        jmp    loc_3_04DD
loc_3_0569:
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    eax, dword ptr [eax]
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
        jmp    loc_3_04DD
sub_3_0580:
        push   0xc
        push   4
        call   sub_2_0000
loc_3_0589:
        mov    dword ptr [0], eax            ; -> obj4:0x0040
        ret    
sub_3_058F:
        cmp    dword ptr [0], 0              ; -> obj4:0x0040
        push   esi
        je     loc_3_05DE
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_0034
loc_3_05A4:
        mov    esi, eax
        test   esi, esi
        je     loc_3_05C9
loc_3_05AA:
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     loc_3_05B7
        push   eax
        call   sub_2_0014
loc_3_05B7:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_0024
loc_3_05C3:
        mov    esi, eax
        test   esi, esi
        jne    loc_3_05AA
loc_3_05C9:
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_0014
loc_3_05D4:
        mov    dword ptr [0], 0              ; -> obj4:0x0040
loc_3_05DE:
        pop    esi
        ret    
sub_3_05E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x208
        push   esi
        lea    eax, [ebp - 0x208]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   0x100
        push   eax
        push   0
        push   dword ptr [esi]
        call   sub_2_0064
loc_3_0605:
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_06DC
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x208]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_2_005C
loc_3_0625:
        add    esp, 0xc
        test   eax, eax
        je     loc_3_0636
        mov    eax, 0xd
        jmp    loc_3_06DC
loc_3_0636:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x108]
        mov    dword ptr [ebp - 4], 0x100
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj4:0x0088
        push   dword ptr [ebp - 8]
        call   sub_2_0054
loc_3_0659:
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_06CA
        cmp    dword ptr [ebp - 4], 0
        je     loc_3_06CA
        push   0
        lea    eax, [ebp - 0x108]
        push   2
        push   eax
        push   dword ptr [esi]
        call   sub_2_004C
loc_3_0678:
        add    esp, 0x10
        mov    edi, eax
        test   edi, edi
        jne    loc_3_06CF
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x108]
        mov    dword ptr [ebp - 4], 0x100
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj4:0x0090
        push   dword ptr [ebp - 8]
        call   sub_2_0054
loc_3_06A4:
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_06CF
        cmp    dword ptr [ebp - 4], 0
        je     loc_3_06CF
        push   0
        lea    eax, [ebp - 0x108]
        push   0
        push   eax
        push   dword ptr [esi]
        call   sub_2_004C
loc_3_06C3:
        add    esp, 0x10
        mov    edi, eax
        jmp    loc_3_06CF
loc_3_06CA:
        mov    edi, 0xc
loc_3_06CF:
        push   dword ptr [ebp - 8]
        call   sub_2_0044
loc_3_06D7:
        add    esp, 4
        mov    eax, edi
loc_3_06DC:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_06E2:
        cmp    dword ptr [0], 0              ; -> obj4:0x0044
        je     loc_3_06FB
        push   1
        push   dword ptr [esp + 8]
        call   sub_3_0065
        add    esp, 8
        jmp    loc_3_0713
loc_3_06FB:
        cmp    dword ptr [0], 0              ; -> obj4:0x0048
        jne    loc_3_0713
        call   sub_3_00A1
        mov    dword ptr [0], 1              ; -> obj4:0x0048
loc_3_0713:
        ret    
sub_3_0714:
        cmp    dword ptr [0], 0              ; -> obj4:0x0044
        je     loc_3_072B
        push   0
        push   dword ptr [esp + 8]
        call   sub_3_0065
        add    esp, 8
loc_3_072B:
        ret    
sub_3_072C:
        mov    eax, dword ptr [esp + 0xc]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, 1
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        je     loc_3_0792
        push   eax
        call   sub_2_0034
loc_3_0746:
        mov    ebp, eax
        test   ebp, ebp
        je     loc_3_0792
        mov    esi, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [esp + 0x18]
loc_3_0754:
        mov    ecx, dword ptr [ebp]
        mov    edi, 0
        test   ecx, ecx
        je     loc_3_0776
        push   dword ptr [esp + 0x20]
        mov    edi, dword ptr [esp + 0x20]
        push   dword ptr [ebp + 4]
        push   dword ptr [edi]
        push   ebx
        push   esi
        call   ecx
        add    esp, 0x14
        mov    edi, eax
loc_3_0776:
        test   edi, edi
        je     loc_3_077F
        cmp    edi, 1
        jne    loc_3_0792
loc_3_077F:
        mov    eax, dword ptr [esp + 0x1c]
        push   ebp
        push   dword ptr [eax + 8]
        call   sub_2_0024
loc_3_078C:
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_3_0754
loc_3_0792:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_0799:
        cmp    dword ptr [0], 0              ; -> obj4:0x0040
        push   esi
        push   edi
        je     loc_3_07CF
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_0034
loc_3_07AF:
        mov    esi, eax
        test   esi, esi
        je     loc_3_07CF
        mov    edi, dword ptr [esp + 0xc]
loc_3_07B9:
        cmp    dword ptr [esi], edi
        je     loc_3_07D7
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_0024
loc_3_07C9:
        mov    esi, eax
        test   esi, esi
        jne    loc_3_07B9
loc_3_07CF:
        mov    eax, 0xd
loc_3_07D4:
        pop    edi
        pop    esi
        ret    
loc_3_07D7:
        cmp    dword ptr [esi + 8], 0
        jne    loc_3_07F4
        push   8
        push   4
        call   sub_2_0000
loc_3_07E6:
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        jne    loc_3_07F4
        mov    eax, 2
        jmp    loc_3_07D4
loc_3_07F4:
        push   dword ptr [esi + 8]
        call   sub_2_007C
loc_3_07FC:
        test   eax, eax
        jne    loc_3_0807
        mov    eax, 2
        jmp    loc_3_07D4
loc_3_0807:
        mov    ecx, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0x14]
        push   eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], edx
        push   dword ptr [esi + 8]
        call   sub_2_006C
loc_3_081D:
        xor    eax, eax
        jmp    loc_3_07D4
sub_3_0821:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        mov    dword ptr [0], 1              ; -> obj4:0x0044
        push   edi
        push   eax
        call   sub_2_0034
loc_3_0838:
        mov    esi, eax
        test   esi, esi
        je     loc_3_0866
        mov    edi, 2
loc_3_0843:
        cmp    dword ptr [esi + 4], edi
        jne    loc_3_0854
        push   1
        push   dword ptr [esi]
        call   sub_3_0065
        add    esp, 8
loc_3_0854:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_0024
loc_3_0860:
        mov    esi, eax
        test   esi, esi
        jne    loc_3_0843
loc_3_0866:
        pop    edi
        pop    esi
        ret    
sub_3_0869:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        lea    edx, [ebp - 8]
        push   0
        push   dword ptr [ebp + 8]
        push   2
        push   ecx
        push   edx
        call   sub_2_0090
loc_3_0885:
        add    esp, 0x14
        test   eax, eax
        je     loc_3_0893
        mov    eax, 0xb
        jmp    loc_3_08BF
loc_3_0893:
        xor    edi, edi
        mov    edx, dword ptr [ebp + 0x10]
        cmp    dword ptr [edx], edi
        jbe    loc_3_08BB
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
loc_3_08A2:
        cmp    edi, dword ptr [ebp - 4]
        jae    loc_3_08BB
        mov    eax, dword ptr [esi]
        inc    edi
        mov    ax, word ptr [eax + 6]
        add    esi, 4
        mov    word ptr [ecx], ax
        add    ecx, 2
        cmp    dword ptr [edx], edi
        ja     loc_3_08A2
loc_3_08BB:
        xor    eax, eax
        mov    dword ptr [edx], edi
loc_3_08BF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_08C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x218
        push   ebx
        lea    eax, [ebp - 0xc]
        push   esi
        lea    ecx, [ebp - 0x218]
        push   edi
        xor    esi, esi
        push   eax
        lea    edx, [ebp - 0x18]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        lea    ecx, [ebp - 0x118]
        push   esi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0x100
        mov    dword ptr [ebp - 0xc], 0x100
        push   eax
        push   ecx
        push   esi
        push   edi
        call   sub_2_00A8
loc_3_0906:
        add    esp, 0x20
        mov    ebx, dword ptr [ebp + 0xc]
        test   eax, eax
        jne    loc_3_0968
loc_3_0910:
        lea    ecx, [ebp - 0x218]
        lea    eax, [ebp - 0x118]
        push   dword ptr [ebp - 0xc]
        push   ecx
        push   dword ptr [ebp - 0x18]
        push   0
        push   eax
        inc    esi
        push   ebx
        call   sub_2_00B0
loc_3_092D:
        add    esp, 0x18
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x218]
        lea    edx, [ebp - 0x18]
        mov    dword ptr [ebp - 8], 0x100
        mov    dword ptr [ebp - 0xc], 0x100
        push   eax
        push   ecx
        push   edx
        lea    eax, [ebp - 8]
        push   0
        lea    ecx, [ebp - 0x118]
        push   eax
        push   ecx
        push   esi
        push   edi
        call   sub_2_00A8
loc_3_0961:
        add    esp, 0x20
        test   eax, eax
        je     loc_3_0910
loc_3_0968:
        xor    esi, esi
        push   0x100
        mov    dword ptr [ebp - 8], 0x100
        lea    eax, [ebp - 0x118]
        push   eax
        push   esi
        push   edi
        call   sub_2_0098
loc_3_0984:
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_0A04
loc_3_098B:
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0x118]
        push   eax
        push   ecx
        push   edi
        call   sub_2_005C
loc_3_099C:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_0A06
        lea    eax, [ebp - 0x14]
        lea    ecx, [ebp - 0x118]
        push   eax
        push   ecx
        push   ebx
        call   sub_2_00A0
loc_3_09B4:
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_3_0A0D
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        call   sub_3_08C5
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp - 0x10]
        call   sub_2_0044
loc_3_09D7:
        add    esp, 4
        push   dword ptr [ebp - 0x14]
        call   sub_2_0044
loc_3_09E2:
        add    esp, 4
        cmp    dword ptr [ebp - 4], 0
        jne    loc_3_0A1D
        inc    esi
        lea    ecx, [ebp - 0x118]
        push   dword ptr [ebp - 8]
        push   ecx
        push   esi
        push   edi
        call   sub_2_0098
loc_3_09FD:
        add    esp, 0x10
        test   eax, eax
        je     loc_3_098B
loc_3_0A04:
        xor    eax, eax
loc_3_0A06:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_0A0D:
        push   dword ptr [ebp - 0x10]
        call   sub_2_0044
loc_3_0A15:
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_3_0A06
loc_3_0A1D:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_3_0A06
_MMDEVLDR_SetDevicePresence:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x104
        push   esi
        lea    eax, [ebp - 0x104]
        push   1
        push   0x100
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_0064
loc_3_0A44:
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_0B23
        push   0                             ; -> obj4:0x0070
        lea    eax, [ebp - 0x104]
        push   eax
        call   sub_3_03FD
        add    esp, 8
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x104]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_2_005C
loc_3_0A78:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_0B23
        push   2
        push   0                             ; -> obj4:0x007C
        push   1
        push   0
        push   0                             ; -> obj4:0x0050
        push   dword ptr [ebp - 4]
        call   sub_2_00B0
loc_3_0A9B:
        add    esp, 0x18
        push   dword ptr [ebp - 4]
        call   sub_2_0044
loc_3_0AA6:
        add    esp, 4
        mov    esi, dword ptr [ebp + 0xc]
        test   esi, esi
        je     loc_3_0B23
        push   0                             ; -> obj4:0x00FC
        lea    eax, [ebp - 0x104]
        push   eax
        call   sub_3_03FD
        add    esp, 8
        lea    eax, [ebp - 0x104]
        push   esi
        push   eax
        call   sub_3_03FD
        add    esp, 8
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x104]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_2_005C
loc_3_0AE9:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_0B23
        cmp    dword ptr [ebp + 0x10], 0
        push   2
        je     loc_3_0AFF
        push   0                             ; -> obj4:0x007C
        jmp    loc_3_0B04
loc_3_0AFF:
        push   0                             ; -> obj4:0x0080
loc_3_0B04:
        push   1
        push   0
        push   0                             ; -> obj4:0x0060
        push   dword ptr [ebp - 4]
        call   sub_2_00B0
loc_3_0B15:
        add    esp, 0x18
        push   dword ptr [ebp - 4]
        call   sub_2_0044
loc_3_0B20:
        add    esp, 4
loc_3_0B23:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0B28:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x57c
        push   ebx
        lea    eax, [ebp - 0x47c]
        mov    dword ptr [ebp - 0x1c], 0
        push   esi
        push   edi
        push   1
        push   0x100
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_0064
loc_3_0B53:
        add    esp, 0x14
        lea    ecx, [ebp - 0x47c]
        lea    edx, [ebp - 0x37c]
        push   ecx
        push   edx
        call   sub_3_03D6
        add    esp, 8
        lea    ecx, [ebp - 0x37c]
        push   0                             ; -> obj4:0x0070
        push   ecx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 0x20]
        lea    edx, [ebp - 0x37c]
        push   ecx
        push   edx
        push   0x80000002
        call   sub_2_005C
loc_3_0B95:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_100A
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x2c]
        mov    dword ptr [ebp - 0xc], 8
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj4:0x0050
        push   dword ptr [ebp - 0x20]
        call   sub_2_0054
loc_3_0BC0:
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_0C07
        cmp    byte ptr [ebp - 0x2c], 0x31
        jne    loc_3_0C07
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0x1c], 1
        mov    dword ptr [ebp - 0xc], 4
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj4:0x00E8
        push   dword ptr [ebp - 0x20]
        call   sub_2_0054
loc_3_0BF4:
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_0C07
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 8], eax
        je     loc_3_0FFF
loc_3_0C07:
        push   4
        lea    eax, [ebp + 8]
        push   eax
        push   3
        push   0
        push   0                             ; -> obj4:0x00E8
        push   dword ptr [ebp - 0x20]
        call   sub_2_00B0
loc_3_0C1E:
        add    esp, 0x18
        lea    ecx, [ebp - 0x3c]
        push   0                             ; -> obj4:0x0104
        push   ecx
        call   sub_3_03D6
        add    esp, 8
        lea    ecx, [ebp - 0x47c]
        push   ecx
        call   sub_3_03EC
        add    esp, 4
        cmp    byte ptr [ebp + eax - 0x47c], 0x5c
        je     loc_3_0C5A
        mov    ecx, 0x5c
loc_3_0C50:
        dec    eax
        cmp    byte ptr [ebp + eax - 0x47c], cl
        jne    loc_3_0C50
loc_3_0C5A:
        lea    eax, [ebp + eax - 0x47b]
        lea    ecx, [ebp - 0x3c]
        push   eax
        push   ecx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 0x3c]
        push   0                             ; -> obj4:0x0100
        push   ecx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 0x57c]
        push   0                             ; -> obj4:0x00A0
        push   ecx
        call   sub_3_03D6
        add    esp, 8
        lea    ecx, [ebp - 0x7c]
        mov    dword ptr [ebp - 0xc], 0x40
        mov    dword ptr [ebp - 0x24], 0
        push   0x40
        push   ecx
        push   0
        push   dword ptr [ebp - 0x20]
        call   sub_2_0098
loc_3_0CB1:
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_0FE4
loc_3_0CBC:
        lea    eax, [ebp - 0x14]
        lea    ecx, [ebp - 0x7c]
        mov    dword ptr [ebp - 0x18], 1
        push   eax
        push   ecx
        push   dword ptr [ebp - 0x20]
        call   sub_2_005C
loc_3_0CD3:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_0FFF
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x2c]
        mov    dword ptr [ebp - 0xc], 8
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj4:0x0060
        push   dword ptr [ebp - 0x14]
        call   sub_2_0054
loc_3_0CFE:
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_0D12
        cmp    byte ptr [ebp - 0x2c], 0x31
        jne    loc_3_0D12
        mov    dword ptr [ebp - 0x18], 0
loc_3_0D12:
        xor    esi, esi
        push   0x100
        mov    dword ptr [ebp - 0xc], 0x100
        lea    eax, [ebp - 0x37c]
        push   eax
        push   esi
        push   dword ptr [ebp - 0x14]
        call   sub_2_0098
loc_3_0D30:
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_0FB3
        mov    ebx, 0x5c
loc_3_0D40:
        mov    edi, 1
        lea    eax, [ebp - 0x10]
        push   eax
        lea    ecx, [ebp - 0x37c]
        push   ecx
        push   dword ptr [ebp - 0x14]
        call   sub_2_005C
loc_3_0D58:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_1011
        lea    eax, [ebp - 0x3c]
        lea    ecx, [ebp - 0x37c]
        push   eax
        push   ecx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 0x57c]
        lea    edx, [ebp - 0x27c]
        push   ecx
        push   edx
        call   sub_3_03D6
        add    esp, 8
        lea    ecx, [ebp - 0x7c]
        lea    edx, [ebp - 0x27c]
        push   ecx
        push   edx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 0x27c]
        push   0                             ; -> obj4:0x00FC
        push   ecx
        call   sub_3_03FD
        add    esp, 8
        lea    ecx, [ebp - 0x37c]
        lea    edx, [ebp - 0x27c]
        push   ecx
        push   edx
        call   sub_3_03FD
        add    esp, 8
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_3_0E15
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x27c]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_2_005C
loc_3_0DE4:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_0E15
        push   4
        lea    eax, [ebp + 8]
        push   eax
        push   3
        push   0
        push   0                             ; -> obj4:0x00E8
        push   dword ptr [ebp - 8]
        call   sub_2_00B0
loc_3_0E02:
        add    esp, 0x18
        push   dword ptr [ebp - 8]
        call   sub_2_0044
loc_3_0E0D:
        add    esp, 4
        jmp    loc_3_0F82
loc_3_0E15:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x2c]
        mov    dword ptr [ebp - 0xc], 8
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj4:0x0060
        push   dword ptr [ebp - 0x10]
        call   sub_2_0054
loc_3_0E35:
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_0E44
        cmp    byte ptr [ebp - 0x2c], 0x31
        jne    loc_3_0E44
        xor    edi, edi
loc_3_0E44:
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_3_0F6E
        test   edi, edi
        je     loc_3_0F6E
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x27c]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_2_00A0
loc_3_0E6B:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_101E
        push   0
        lea    eax, [ebp - 0x17c]
        push   0x100
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_0064
loc_3_0E8E:
        mov    dword ptr [ebp - 4], 0
        add    esp, 0x14
        cmp    byte ptr [ebp - 0x17c], bl
        je     loc_3_0EAF
loc_3_0EA0:
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [ebp + eax - 0x17c], bl
        jne    loc_3_0EA0
loc_3_0EAF:
        lea    eax, [ebp - 0x47c]
        push   eax
        call   sub_3_03EC
        add    esp, 4
        inc    eax
        lea    ecx, [ebp - 0x47c]
        push   eax
        push   ecx
        push   1
        push   0
        push   0                             ; -> obj4:0x00D8
        push   dword ptr [ebp - 8]
        call   sub_2_00B0
loc_3_0ED8:
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_1036
        lea    eax, [ebp - 0x47c]
        push   eax
        call   sub_3_03EC
        add    esp, 4
        inc    eax
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        lea    edx, [ebp + ecx - 0x17b]
        push   edx
        push   1
        push   0
        push   0                             ; -> obj4:0x00F0
        push   dword ptr [ebp - 8]
        call   sub_2_00B0
loc_3_0F10:
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_1036
        push   4
        lea    eax, [ebp + 8]
        push   eax
        push   3
        push   0
        push   0                             ; -> obj4:0x00E8
        push   dword ptr [ebp - 8]
        call   sub_2_00B0
loc_3_0F32:
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_1036
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 0x10]
        call   sub_3_08C5
        add    esp, 8
        test   eax, eax
        jne    loc_3_1036
        push   dword ptr [ebp - 8]
        call   sub_2_00C0
loc_3_0F5B:
        add    esp, 4
        push   dword ptr [ebp - 0x10]
        call   sub_2_0044
loc_3_0F66:
        add    esp, 4
        push   dword ptr [ebp - 8]
        jmp    loc_3_0F85
loc_3_0F6E:
        lea    eax, [ebp - 0x27c]
        push   eax
        push   0x80000002
        call   sub_2_00B8
loc_3_0F7F:
        add    esp, 8
loc_3_0F82:
        push   dword ptr [ebp - 0x10]
loc_3_0F85:
        inc    esi
        call   sub_2_0044
loc_3_0F8B:
        add    esp, 4
        lea    ecx, [ebp - 0x37c]
        mov    dword ptr [ebp - 0xc], 0x100
        push   dword ptr [ebp - 0xc]
        push   ecx
        push   esi
        push   dword ptr [ebp - 0x14]
        call   sub_2_0098
loc_3_0FA8:
        add    esp, 0x10
        test   eax, eax
        je     loc_3_0D40
loc_3_0FB3:
        push   dword ptr [ebp - 0x14]
        call   sub_2_0044
loc_3_0FBB:
        add    esp, 4
        lea    eax, [ebp - 0x7c]
        mov    dword ptr [ebp - 0xc], 0x40
        push   0x40
        inc    dword ptr [ebp - 0x24]
        push   eax
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        call   sub_2_0098
loc_3_0FD9:
        add    esp, 0x10
        test   eax, eax
        je     loc_3_0CBC
loc_3_0FE4:
        push   2
        push   0                             ; -> obj4:0x0080
        push   1
        push   0
        push   0                             ; -> obj4:0x0050
        push   dword ptr [ebp - 0x20]
        call   sub_2_00B0
loc_3_0FFC:
        add    esp, 0x18
loc_3_0FFF:
        push   dword ptr [ebp - 0x20]
        call   sub_2_0044
loc_3_1007:
        add    esp, 4
loc_3_100A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1011:
        push   dword ptr [ebp - 0x14]
        call   sub_2_0044
loc_3_1019:
        add    esp, 4
        jmp    loc_3_0FFF
loc_3_101E:
        push   dword ptr [ebp - 0x10]
        call   sub_2_0044
loc_3_1026:
        add    esp, 4
        push   dword ptr [ebp - 0x14]
        call   sub_2_0044
loc_3_1031:
        add    esp, 4
        jmp    loc_3_0FFF
loc_3_1036:
        push   dword ptr [ebp - 0x10]
        call   sub_2_0044
loc_3_103E:
        add    esp, 4
        push   dword ptr [ebp - 8]
        call   sub_2_0044
loc_3_1049:
        add    esp, 4
        push   dword ptr [ebp - 0x14]
        call   sub_2_0044
loc_3_1054:
        add    esp, 4
        jmp    loc_3_0FFF
sub_3_1059:
        cmp    dword ptr [0], 0              ; -> obj4:0x0040
        push   ebx
        push   esi
        push   edi
        push   ebp
        jne    loc_3_1070
        mov    eax, 0x21
        jmp    loc_3_1216
loc_3_1070:
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_0034
loc_3_107B:
        mov    esi, eax
        mov    ebp, dword ptr [esp + 0x1c]
        test   esi, esi
        je     loc_3_1146
loc_3_1089:
        cmp    dword ptr [esi], ebp
        je     loc_3_10A4
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_0024
loc_3_1099:
        mov    esi, eax
        test   esi, esi
        jne    loc_3_1089
        jmp    loc_3_1146
loc_3_10A4:
        mov    edi, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [esp + 0x18]
        cmp    edi, 4
        jne    loc_3_10B5
        test   ebx, ebx
        je     loc_3_10BA
loc_3_10B5:
        cmp    edi, 2
        jne    loc_3_10C3
loc_3_10BA:
        push   ebp
        call   sub_3_0714
        add    esp, 4
loc_3_10C3:
        push   dword ptr [esp + 0x24]
        push   esi
        push   ebx
        push   edi
        call   sub_3_072C
        add    esp, 0x10
        mov    ebx, eax
        cmp    edi, 4
        jne    loc_3_1105
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     loc_3_10E6
        push   eax
        call   sub_2_0014
loc_3_10E6:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_00D8
loc_3_10F2:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_00C8
loc_3_10FE:
        mov    eax, ebx
        jmp    loc_3_1216
loc_3_1105:
        cmp    edi, 2
        jne    loc_3_1118
        mov    dword ptr [esi + 4], 3
        mov    eax, ebx
        jmp    loc_3_1216
loc_3_1118:
        cmp    edi, 1
        jne    loc_3_113F
        test   ebx, ebx
        je     loc_3_1126
        cmp    ebx, 1
        jne    loc_3_113F
loc_3_1126:
        mov    dword ptr [esi + 4], 2
        push   ebp
        call   sub_3_0B28
        add    esp, 4
        push   ebp
        call   sub_3_06E2
        add    esp, 4
loc_3_113F:
        mov    eax, ebx
        jmp    loc_3_1216
loc_3_1146:
        mov    edi, dword ptr [esp + 0x14]
        cmp    edi, 4
        je     loc_3_1211
        cmp    edi, 0xd
        je     loc_3_1211
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_007C
loc_3_1167:
        mov    esi, eax
        test   esi, esi
        jne    loc_3_1177
        mov    eax, 2
        jmp    loc_3_1216
loc_3_1177:
        push   ebp
        mov    dword ptr [esi], ebp
        mov    dword ptr [esi + 8], 0
        mov    dword ptr [esi + 4], 0
        call   sub_3_0B28
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   esi
        push   eax
        call   sub_2_006C
loc_3_119C:
        push   esi
        call   sub_3_05E0
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     loc_3_11D4
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     loc_3_11B8
        push   eax
        call   sub_2_0014
loc_3_11B8:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_00D8
loc_3_11C4:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0040
        push   eax
        call   sub_2_00C8
loc_3_11D0:
        mov    eax, ebx
        jmp    loc_3_1216
loc_3_11D4:
        push   dword ptr [esp + 0x24]
        push   esi
        push   dword ptr [esp + 0x20]
        push   edi
        mov    dword ptr [esi + 4], 1
        call   sub_3_072C
        add    esp, 0x10
        mov    ebx, eax
        cmp    edi, 1
        jne    loc_3_120D
        test   ebx, ebx
        je     loc_3_11FD
        cmp    ebx, 1
        jne    loc_3_120D
loc_3_11FD:
        mov    dword ptr [esi + 4], 2
        push   ebp
        call   sub_3_06E2
        add    esp, 4
loc_3_120D:
        mov    eax, ebx
        jmp    loc_3_1216
loc_3_1211:
        mov    eax, 1
loc_3_1216:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_121B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        push   esi
        push   esi
        mov    esi, dword ptr [ebp - 4]
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        mov    eax, edx
        pop    esi
        test   eax, eax
        jne    loc_3_123E
        xor    eax, eax
        jmp    loc_3_1258
loc_3_123E:
        movzx  ecx, word ptr [eax - 0xd]
        shl    ecx, 4
        test   ecx, ecx
        jne    loc_3_124D
        xor    eax, eax
        jmp    loc_3_1258
loc_3_124D:
        mov    edx, dword ptr [ebp + 8]
        test   edx, edx
        je     loc_3_1258
        add    ecx, eax
        mov    dword ptr [edx], ecx
loc_3_1258:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_125D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld    
        push   edi
        push   esi
        mov    ecx, dword ptr [ebp + 0x10]
        jecxz  loc_3_1291
        mov    edx, ecx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, edi
        xor    eax, eax
        repne scasb al, byte ptr es:[edi]
        neg    ecx
        add    ecx, edx
        mov    edi, esi
        mov    esi, dword ptr [ebp + 0xc]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    al, byte ptr [esi - 1]
        xor    ecx, ecx
        cmp    al, byte ptr [edi - 1]
        ja     loc_3_128F
        je     loc_3_1291
        dec    ecx
        dec    ecx
loc_3_128F:
        not    ecx
loc_3_1291:
        mov    eax, ecx
        pop    esi
        pop    edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_3_1299:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   esi
        push   edi
        cld    
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_3_12B2:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        cld    
        jecxz  loc_3_12CE
loc_3_12C6:
        lodsb  al, byte ptr [esi]
        and    al, al
        je     loc_3_12CE
        stosb  byte ptr es:[edi], al
        loop   loc_3_12C6
loc_3_12CE:
        mov    byte ptr [edi], 0
        pop    edi
        pop    esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_3_12D7:
        mov    edx, dword ptr [esp + 4]
        push   edi
        test   edx, edx
        jne    loc_3_12E4
        xor    eax, eax
        jmp    loc_3_1300
loc_3_12E4:
        cmp    byte ptr [edx], 0
        je     loc_3_12FD
loc_3_12E9:
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    edx, ecx
        cmp    byte ptr [edx], 0
        jne    loc_3_12E9
loc_3_12FD:
        lea    eax, [edx + 1]
loc_3_1300:
        pop    edi
        ret    
_MMDEVLDR_SetEnvironmentString:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   _MMDEVLDR_RemoveEnvironmentString
        mov    eax, dword ptr [esp + 0x10]
        add    esp, 4
        cmp    byte ptr [eax], 0
        je     loc_3_1323
        push   eax
        push   esi
        call   _MMDEVLDR_AddEnvironmentString
        add    esp, 8
loc_3_1323:
        pop    esi
        ret    
_MMDEVLDR_GetEnvironmentString:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        call   sub_3_121B
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     loc_3_1382
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, 0xffffffff
        mov    edi, esi
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [esp + 0x18]
        lea    ebp, [ecx - 1]
        mov    byte ptr [eax], 0
        cmp    byte ptr [ebx], 0
        je     loc_3_1382
loc_3_1359:
        push   ebp
        push   esi
        push   ebx
        call   sub_3_125D
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_136E
        cmp    byte ptr [ebx + ebp], 0x3d
        je     loc_3_1389
loc_3_136E:
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ebx, ecx
        cmp    byte ptr [ebx], 0
        jne    loc_3_1359
loc_3_1382:
        xor    eax, eax
loc_3_1384:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_1389:
        lea    ebx, [ebp + ebx + 1]
        mov    eax, dword ptr [esp + 0x1c]
        dec    eax
        push   eax
        push   ebx
        push   dword ptr [esp + 0x20]
        call   sub_3_12B2
        add    esp, 0xc
        mov    eax, 1
        jmp    loc_3_1384
_MMDEVLDR_RemoveEnvironmentString:
        sub    esp, 8
        lea    eax, [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   eax
        call   sub_3_121B
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        test   eax, eax
        je     loc_3_1456
        mov    esi, eax
        mov    ebx, dword ptr [esp + 0x1c]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [esp + 0x10]
        lea    ebp, [ecx - 1]
        cmp    byte ptr [eax], 0
        je     loc_3_1456
loc_3_13E6:
        push   ebp
        push   ebx
        push   esi
        call   sub_3_125D
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_13FB
        cmp    byte ptr [esi + ebp], 0x3d
        je     loc_3_1411
loc_3_13FB:
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    esi, ecx
        cmp    byte ptr [esi], 0
        jne    loc_3_13E6
        jmp    loc_3_1456
loc_3_1411:
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [ecx + esi]
        mov    ecx, dword ptr [esp + 0x14]
        sub    ecx, eax
        push   ecx
        push   eax
        push   esi
        call   sub_3_1299
        add    esp, 0xc
        push   dword ptr [esp + 0x10]
        call   sub_3_12D7
        mov    edx, dword ptr [esp + 0x18]
        add    esp, 4
        sub    edx, eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
loc_3_1456:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
_MMDEVLDR_AddEnvironmentString:
        sub    esp, 4
        lea    eax, [esp]
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   eax
        call   sub_3_121B
        add    esp, 4
        mov    edx, eax
        test   edx, edx
        je     loc_3_1579
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, 0xffffffff
        sub    eax, eax
        push   edx
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    esi, [ecx - 1]
        call   sub_3_12D7
        add    esp, 4
        mov    edx, eax
        test   edx, edx
        je     loc_3_1579
        mov    ebp, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x1c]
        sub    ebp, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    ebx, [ecx + esi + 1]
        cmp    ebp, ebx
        jae    loc_3_14E5
        add    esi, 3
        cmp    esi, ebp
        ja     loc_3_1579
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    eax, dword ptr [esp + 0x1c]
        sub    ecx, ebx
        add    ecx, ebp
        mov    byte ptr [ecx + eax], 0
loc_3_14E5:
        dec    edx
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, edx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, 0                        ; -> obj4:0x0108
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    ebx, ecx
        mov    esi, edi
        mov    ecx, 0xffffffff
        mov    edi, edx
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    ebx, ecx
        mov    esi, edi
        mov    ecx, 0xffffffff
        mov    edi, edx
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    byte ptr [edx + ecx], 0
loc_3_1579:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x10a  RW-
        db  052h, 049h, 04Eh, 047h, 033h, 05Fh, 044h, 045h, 056h, 04Ch, 04Fh, 041h, 044h, 045h, 052h, 000h
        db  04Dh, 04Dh, 053h, 059h, 053h, 054h, 045h, 04Dh, 02Eh, 044h, 04Ch, 04Ch, 000h, 04Bh, 045h, 052h
        db  04Eh, 045h, 04Ch, 000h, 047h, 045h, 054h, 04Dh, 04Fh, 044h, 055h, 04Ch, 045h, 048h, 041h, 04Eh
        db  044h, 04Ch, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  04Dh, 049h, 047h, 052h, 041h, 054h, 045h, 044h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  04Eh, 06Fh, 074h, 050h, 072h, 065h, 073h, 065h, 06Eh, 074h, 000h, 000h, 000h, 000h, 000h, 000h
        db  05Ch, 064h, 072h, 069h, 076h, 065h, 072h, 073h, 000h, 000h, 000h, 000h, 030h, 000h, 000h, 000h
        db  031h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 044h, 072h, 069h, 076h, 065h, 072h, 000h, 000h
        db  045h, 06Eh, 075h, 06Dh, 065h, 072h, 061h, 074h, 06Fh, 072h, 000h, 000h, 000h, 000h, 000h, 000h
        db  053h, 079h, 073h, 074h, 065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh
        db  06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h, 05Ch, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch
        db  05Ch, 04Dh, 065h, 064h, 069h, 061h, 052h, 065h, 073h, 06Fh, 075h, 072h, 063h, 065h, 073h, 05Ch
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 053h, 04Fh, 046h, 054h, 057h, 041h, 052h, 045h
        db  04Bh, 045h, 059h, 000h, 000h, 000h, 000h, 000h, 044h, 065h, 076h, 04Eh, 06Fh, 064h, 065h, 000h
        db  044h, 065h, 076h, 069h, 063h, 065h, 049h, 044h, 000h, 000h, 000h, 000h, 05Ch, 000h, 000h, 000h
        db  03Eh, 000h, 000h, 000h, 03Ch, 000h, 000h, 000h, 03Dh, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x9  R-X
loc_5_0000:
        call   sub_3_0580
        xor    eax, eax
        clc    
        ret    
VxD__LTEXT ENDS
