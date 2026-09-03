; NSCL -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 4470 instructions recovered by recursive descent

Declare_Virtual_Device NSCL, 1, 0, NSCL_Control, 00494h, Undefined_Init_Order
;   DDK version 0400h   init_order 05400606Eh   flags 00000h
;   service table: obj1:0x63c, 3 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3a4a  R-X
NSCL_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 0x23
        je     loc_1_0568
        cmp    eax, 4
        je     loc_1_0024
        cmp    eax, 0xb
        je     loc_1_006C
        clc    
        ret    
NSCL_Service_00:
        mov    ax, 0x100
        clc    
        ret    
loc_1_0024:
        test   dword ptr [0x3920], 1         ; -> obj1:0x3920
        je     loc_1_0035
        call   sub_1_2EDC
loc_1_0035:
        ret    
NSCL_Service_02:
        test   dword ptr [0x3920], 1         ; -> obj1:0x3920
        je     loc_1_006B
        and    dword ptr [0x3920], 0xffffffdf ; -> obj1:0x3920
        call   sub_1_016D
        call   sub_1_0351
        and    dword ptr [0x3920], 0xffffdbdd ; -> obj1:0x3920
        cmp    dword ptr [0x396c], 0         ; -> obj1:0x396C
        je     loc_1_006B
        call   sub_1_0F51
loc_1_006B:
        ret    
loc_1_006C:
        push   ebx
        call   sub_1_2E45
        pop    ebx
        ret    
sub_1_0074:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        leave  
        ret    
sub_1_008C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        leave  
        ret    
sub_1_009C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    eax, 4
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, 0xa
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    eax, esi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_00BF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    eax, esi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_00D6:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
        pop    esi
        leave  
        ret    
sub_1_00E6:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        leave  
        ret    
sub_1_00FA:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        leave  
        ret    
sub_1_010E:
        push   ebx
        mov    ebx, 0xa
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        pop    ebx
        ret    
sub_1_011C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ebx, 0xe
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi], dx
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_013F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 3
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0156:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 6
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_016D:
        push   ebx
        push   esi
        and    dword ptr [0x3920], 0xffffffbf ; -> obj1:0x3920
        mov    esi, 0x396c                   ; -> obj1:0x396C
        mov    ebx, 9
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cmp    ecx, 4
        jb     loc_1_0192
        or     dword ptr [0x3920], 0x40      ; -> obj1:0x3920
loc_1_0192:
        imul   ecx, ecx, 0x6e
        add    ecx, 0xfa
        mov    dword ptr [0x3978], ecx       ; -> obj1:0x3978
        pop    esi
        pop    ebx
        ret    
sub_1_01A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, 2
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        or     eax, eax
        jne    loc_1_01C7
        mov    esi, dword ptr [ebp + 0x10]
        movzx  ecx, cx
        mov    dword ptr [esi], ecx
loc_1_01C7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_01CC:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        ret    
sub_1_01D3:
        VMMCall  VMM._BlockOnID                  ; cd20fa800100
sub_1_01D9:
        VMMCall  VMM._SignalID                   ; cd2027810100
sub_1_01DF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [ebp + 0xc]
        push   1
        push   ebx
        push   esi
        push   edi
        VxDCall  IFSMgr.BCSToUni                 ; cd2042004000
        add    esp, 0x10
        lea    eax, [ebx*2]
        push   eax
        push   edi
        push   edi
        VxDCall  IFSMgr.UniToUpper               ; cd2043004000
        add    esp, 0xc
        push   1
        push   ebx
        add    ebx, ebx
        push   ebx
        push   edi
        push   esi
        VxDCall  IFSMgr.UniToBCS                 ; cd2040004000
        add    esp, 0x14
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
        ; ---- dispatch table (11 entries) ----
        dd  loc_1_0295
        dd  loc_1_02A3
        dd  loc_1_02B4
        dd  loc_1_02A5
        dd  loc_1_02F5
        dd  loc_1_02FE
        dd  loc_1_0317
        dd  loc_1_0337
        dd  loc_1_02C1
        dd  loc_1_02DA
        dd  loc_1_02E9
NSCL_Service_01:
        push   0
        call   sub_1_025F
        add    esp, 4
        ret    
sub_1_025F:
        test   dword ptr [0x3920], 1         ; -> obj1:0x3920
        je     loc_1_027F
loc_1_026B:
        movzx  eax, al
        cmp    eax, 0xb
        jae    loc_1_028E
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        jmp    dword ptr [eax*4 + 0x228]     ; -> obj1:0x0228
loc_1_027F:
        push   eax
        push   ecx
        push   edx
        call   sub_1_06F6
        or     eax, eax
        pop    edx
        pop    ecx
        pop    eax
        je     loc_1_026B
loc_1_028E:
        stc    
        mov    eax, 0xffffffff
        ret    
loc_1_0295:
        push   ebp
        push   edi
        push   ecx
        push   esi
        call   sub_1_08B7
        add    esp, 0x10
        pop    ebp
        ret    
loc_1_02A3:
        xor    ebx, ebx
loc_1_02A5:
        push   ebp
        push   ebx
        push   edx
        push   ecx
        push   esi
        call   sub_1_09B1
        add    esp, 0x14
        pop    ebp
        ret    
loc_1_02B4:
        push   ebp
        push   ecx
        push   esi
        call   sub_1_0BAB
        add    esp, 0xc
        pop    ebp
        ret    
loc_1_02C1:
        push   ebp
        push   ecx
        push   edi
        mov    eax, dword ptr [esi + 2]
        push   eax
        mov    ax, word ptr [esi]
        push   eax
        add    esi, 6
        push   esi
        call   sub_1_0C7E
        add    esp, 0x18
        pop    ebp
        ret    
loc_1_02DA:
        push   ebp
        push   ecx
        push   edi
        push   dword ptr [esi]
        call   sub_1_0EC2
        add    esp, 0x10
        pop    ebp
        ret    
loc_1_02E9:
        push   ebp
        push   esi
        call   sub_1_0D9A
        add    esp, 8
        pop    ebp
        ret    
loc_1_02F5:
        push   ebx
        call   sub_1_1076
        pop    ebx
        pop    ebp
        ret    
loc_1_02FE:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esi + 0x24]
        movzx  ecx, word ptr [edi + 2]
        xchg   ch, cl
        lea    esi, [esi + 0x1c]
        call   sub_1_03AB
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_0317:
        and    dword ptr [0x3920], 0xfffffdff ; -> obj1:0x3920
        mov    ebx, 0xe
        mov    dx, 0x5505
        mov    esi, 0x3ab                    ; -> sub_1_03AB
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        pop    ebp
        ret    
loc_1_0337:
        push   ecx
        VxDCall  Dev_0492.Service_0000h          ; cd2000009204
        pop    ecx
        cmc    
        sbb    eax, eax
        and    ecx, eax
        mov    dword ptr [0x3924], ecx       ; -> obj1:0x3924
        call   sub_1_0351
        pop    ebp
        ret    
sub_1_0351:
        and    dword ptr [0x3920], 0xfffffeff ; -> obj1:0x3920
        xor    eax, eax
        cmp    dword ptr [0x3924], eax       ; -> obj1:0x3924
        je     loc_1_037C
        test   dword ptr [0x3920], 0x40      ; -> obj1:0x3920
        jne    loc_1_037C
        or     dword ptr [0x3920], 0x100     ; -> obj1:0x3920
        ret    
loc_1_037C:
        and    dword ptr [0x3920], 0xfffffdff ; -> obj1:0x3920
        ret    
loc_1_0387:
        cmp    byte ptr [eax + 9], 0
        jne    loc_1_03A0
        mov    edi, dword ptr [eax + 0x24]
        movzx  ecx, word ptr [edi + 2]
        xchg   ch, cl
        lea    esi, [eax + 0x1c]
        push   eax
        call   sub_1_03AB
        pop    eax
loc_1_03A0:
        mov    edx, eax
        xchg   dword ptr [0x399c], edx       ; -> obj1:0x399C
        mov    dword ptr [eax], edx
        ret    
sub_1_03AB:
        lea    ebx, [edi + 0x1e]
        cmp    byte ptr [edi + 5], 0x14
        jne    loc_1_03B7
        add    ebx, 0x20
loc_1_03B7:
        cmp    word ptr [edi + 0x1c], 0x5505
        jne    loc_1_03DF
        cmp    dword ptr [ebx], 0x53575242
        jne    loc_1_03DF
        movzx  eax, word ptr [edi + 2]
        xchg   ah, al
        cmp    ecx, eax
        jb     loc_1_0401
        push   esi
        push   ecx
        push   ebx
        push   edi
        call   sub_1_1F80
        add    esp, 0x10
        jmp    loc_1_0455
loc_1_03DF:
        cmp    word ptr [edi + 0x1c], 0x5204
        jne    loc_1_0433
        cmp    word ptr [edi + 0x1e], 0x400
        je     loc_1_03F7
        cmp    word ptr [edi + 0x1e], 0x200
        jne    loc_1_0433
loc_1_03F7:
        movzx  eax, word ptr [edi + 2]
        xchg   ah, al
        cmp    ecx, eax
        jae    loc_1_041E
loc_1_0401:
        mov    esi, dword ptr [0x399c]       ; -> obj1:0x399C
        or     esi, esi
        je     loc_1_0458
        mov    edx, dword ptr [esi]
        mov    dword ptr [0x399c], edx       ; -> obj1:0x399C
        xor    ebx, ebx
        dec    ebx
        mov    dword ptr [esi + 4], 0x387    ; -> loc_1_0387
        ret    
loc_1_041E:
        add    edi, 0x20
        sub    ecx, 0x20
        shr    ecx, 6
        push   ecx
        push   edi
        call   sub_1_26A0
        add    esp, 8
        jmp    loc_1_0455
loc_1_0433:
        cmp    dword ptr [ebx], 0x53575242
        je     loc_1_0455
        movzx  eax, word ptr [edi + 2]
        xchg   ah, al
        cmp    eax, 0x28
        jb     loc_1_0455
        cmp    ecx, eax
        jb     loc_1_0401
        push   ecx
        push   ebx
        push   edi
        call   sub_1_37E7
        add    esp, 0xc
loc_1_0455:
        xor    esi, esi
        ret    
loc_1_0458:
        xor    esi, esi
        ret    
sub_1_045B:
        mov    eax, dword ptr [0x399c]       ; -> obj1:0x399C
        or     eax, eax
        je     loc_1_046D
        mov    edx, dword ptr [eax]
        mov    dword ptr [0x399c], edx       ; -> obj1:0x399C
        ret    
loc_1_046D:
        stc    
        ret    
sub_1_046F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x28], 0x440
        mov    edx, eax
        xchg   dword ptr [0x399c], edx       ; -> obj1:0x399C
        mov    dword ptr [eax], edx
        leave  
        ret    
loc_1_0487:
        mov    dword ptr [edx + 0x10], 0
        push   edx
        call   sub_1_114B
        pop    edx
        ret    
loc_1_0496:
        mov    dword ptr [edx + 0x14], 0
        push   edx
        call   sub_1_114B
        pop    edx
        ret    
loc_1_04A5:
        xor    eax, eax
        mov    dword ptr [edx + 0x10], eax
        push   eax
        push   eax
        push   edx
        call   sub_1_0FBB
        add    esp, 0xc
        ret    
loc_1_04B6:
        mov    dword ptr [edx + 0x14], 0
        push   edx
        mov    byte ptr [edx + 8], 0
        call   dword ptr [edx + 0x24]
        pop    edx
        ret    
loc_1_04C7:
        or     dword ptr [0x3920], 0x200     ; -> obj1:0x3920
        VxDCall  Dev_0492.Service_0002h          ; cd2002009204
        ret    
NSCL_V86_API:
        push   ebx
        movzx  esi, word ptr [ebp + 4]
        movzx  edi, word ptr [ebp]
        movzx  eax, word ptr [ebp + 0x38]
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        add    esi, eax
        add    edi, eax
        movzx  eax, word ptr [ebp + 0x1c]
        movzx  ebx, word ptr [ebp + 0x10]
        movzx  ecx, word ptr [ebp + 0x18]
        movzx  edx, word ptr [ebp + 0x14]
        cmp    al, 0xb
        jae    loc_1_050E
        call   sub_1_025F
        pop    ebx
loc_1_0509:
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_1_050E:
        mov    ax, 0xffff
        jmp    loc_1_0509
NSCL_PM_API:
        push   ebx
        movzx  eax, word ptr [ebp + 0x3c]
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        movzx  esi, word ptr [ebp + 4]
        add    esi, eax
        movzx  eax, word ptr [ebp + 0x38]
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        movzx  edi, word ptr [ebp]
        add    edi, eax
        movzx  eax, word ptr [ebp + 0x1c]
        movzx  ebx, word ptr [ebp + 0x10]
        movzx  ecx, word ptr [ebp + 0x18]
        movzx  edx, word ptr [ebp + 0x14]
        cmp    al, 4
        jae    loc_1_0562
        call   sub_1_025F
        pop    ebx
loc_1_055D:
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_1_0562:
        mov    ax, 0xffff
        jmp    loc_1_055D
loc_1_0568:
        test   dword ptr [0x3920], 1         ; -> obj1:0x3920
        jne    loc_1_057D
        call   sub_1_06F6
        or     eax, eax
        jne    loc_1_05A9
loc_1_057D:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_1_05A6
        mov    eax, dword ptr [esi + 0x18]
        mov    ebx, dword ptr [esi + 0x1c]
        call   sub_1_05AF
        jb     loc_1_05A9
        push   eax
        push   eax
        push   esi
        call   sub_1_0746
        add    esp, 8
        mov    ebx, eax
        pop    eax
        call   sub_1_0601
        mov    eax, ebx
        ret    
loc_1_05A6:
        xor    eax, eax
        ret    
loc_1_05A9:
        mov    eax, 0xffffffff
        ret    
sub_1_05AF:
        or     eax, eax
        je     loc_1_05F7
        push   ebx
        push   edi
        push   esi
        push   eax
        push   ecx
        mov    ecx, eax
        and    ecx, 0xfff
        add    ebx, ecx
        shr    eax, 0xc
        add    ebx, 0xfff
        shr    ebx, 0xc
        pop    ecx
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_05FD
        cmp    eax, 0xfffffbad
        je     loc_1_05F8
        pop    ebx
        and    ebx, 0xfff
        add    eax, ebx
        clc    
loc_1_05F4:
        pop    esi
        pop    edi
        pop    ebx
loc_1_05F7:
        ret    
loc_1_05F8:
        int3   
        stc    
        pop    ebx
        jmp    loc_1_05F4
loc_1_05FD:
        stc    
        pop    ebx
        jmp    loc_1_05F4
sub_1_0601:
        or     eax, eax
        je     loc_1_0638
        push   ebx
        push   edi
        push   esi
        push   ecx
        mov    ecx, eax
        and    ecx, 0xfff
        add    ebx, ecx
        shr    eax, 0xc
        add    ebx, 0xfff
        shr    ebx, 0xc
        pop    ecx
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0639
        clc    
loc_1_0635:
        pop    esi
        pop    edi
        pop    ebx
loc_1_0638:
        ret    
loc_1_0639:
        jmp    loc_1_0635
        db  000h
        dd  NSCL_Service_00          ; fixup
        dd  NSCL_Service_01          ; fixup
        dd  NSCL_Service_02          ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00494h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'NSCL    '                  ; DDB_Name
        dd  05400606Eh                  ; DDB_Init_Order
        dd  NSCL_Control             ; DDB_Control_Proc
        dd  NSCL_V86_API             ; DDB_V86_API_Proc
        dd  NSCL_PM_API              ; DDB_PM_API_Proc
        dd  obj1:0x063C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_06C0:
        push   esi
        mov    eax, dword ptr [0x39b8]       ; -> obj1:0x39B8
        imul   eax, eax, 0x41c64e6d
        add    eax, 0x3039
        mov    dword ptr [0x39b8], eax       ; -> obj1:0x39B8
        jne    loc_1_06DD
        mov    eax, 1
loc_1_06DD:
        mov    dword ptr [0x39b8], eax       ; -> obj1:0x39B8
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        sub    esi, ecx
        inc    esi
        sub    edx, edx
        div    esi
        lea    eax, [ecx + edx]
        pop    esi
        ret    
sub_1_06F6:
        sub    esp, 4
        mov    word ptr [esp + 2], 0x5505
        call   sub_1_01CC
        mov    dword ptr [0x39b8], eax       ; -> obj1:0x39B8
        push   0
        push   0
        lea    eax, [esp + 0xa]
        push   0x3ab                         ; -> sub_1_03AB
        push   eax
        call   sub_1_011C
        add    esp, 0x10
        or     eax, eax
        mov    eax, 0xffffffff
        jne    loc_1_0742
        call   sub_1_016D
        mov    eax, dword ptr [0x3920]       ; -> obj1:0x3920
        or     eax, 1
        mov    dword ptr [0x3920], eax       ; -> obj1:0x3920
        call   sub_1_0F51
        sub    eax, eax
loc_1_0742:
        add    esp, 4
        ret    
sub_1_0746:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        dec    eax
        cmp    eax, 5
        ja     loc_1_075A
        jmp    dword ptr [eax*4 + 0x7ff]     ; -> obj1:0x07FF
loc_1_075A:
        mov    eax, 0xffffffff
        jmp    loc_1_07FE
loc_1_0764:
        mov    ecx, dword ptr [ecx + 0x10]
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        mov    ax, word ptr [ecx]
        add    ecx, 2
        push   eax
        push   ecx
        call   sub_1_08B7
        add    esp, 0x10
        jmp    loc_1_07FE
loc_1_0780:
        mov    edx, dword ptr [ecx + 0x10]
        push   0
        mov    eax, dword ptr [esp + 0xc]
        push   dword ptr [ecx + 0x1c]
        add    edx, 6
        push   eax
        push   dword ptr [edx - 4]
        mov    ax, word ptr [edx - 6]
        push   eax
        push   edx
        call   sub_1_0C7E
        add    esp, 0x18
        jmp    loc_1_07FE
loc_1_07A3:
        push   0
        push   dword ptr [ecx + 0x1c]
        mov    eax, dword ptr [ecx + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [eax]
        call   sub_1_0EC2
        add    esp, 0x10
        jmp    loc_1_07FE
loc_1_07BB:
        mov    ecx, dword ptr [ecx + 0x10]
        push   0
        push   0
        mov    ax, word ptr [ecx]
        add    ecx, 4
        push   eax
        mov    ax, word ptr [ecx - 2]
        push   eax
        push   ecx
        call   sub_1_09B1
        add    esp, 0x14
        jmp    loc_1_07FE
loc_1_07D9:
        mov    eax, dword ptr [ecx + 0x10]
        push   0
        mov    cx, word ptr [eax + 2]
        push   ecx
        add    eax, 4
        push   eax
        call   sub_1_0BAB
        add    esp, 0xc
        jmp    loc_1_07FE
loc_1_07F1:
        push   0
        push   dword ptr [ecx + 0x10]
        call   sub_1_0D9A
        add    esp, 8
loc_1_07FE:
        ret    
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_0764
        dd  loc_1_0780
        dd  loc_1_07A3
        dd  loc_1_07BB
        dd  loc_1_07D9
        dd  loc_1_07F1
sub_1_0817:
        cmp    word ptr [0x3998], 0          ; -> obj1:0x3998
        jne    loc_1_084A
        push   0
        push   0
        push   0x3ab                         ; -> sub_1_03AB
        push   0x3998                        ; -> obj1:0x3998
        call   sub_1_011C
        add    esp, 0x10
        cmp    eax, -1
        jne    loc_1_084A
        mov    edx, dword ptr [esp + 4]
        mov    byte ptr [edx + 8], 0
        mov    byte ptr [edx + 0xb], 5
        jmp    loc_1_08B6
loc_1_084A:
        mov    edx, dword ptr [esp + 4]
        mov    al, byte ptr [esp + 0x10]
        mov    byte ptr [edx + 8], 0xff
        mov    byte ptr [edx + 9], al
        movzx  eax, al
        mov    byte ptr [edx + 0xa], 0
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 0x20], ecx
        mov    dword ptr [edx + 0x10], 0
        mov    dword ptr [edx + 0x14], 0
        mov    eax, dword ptr [eax*4 + 0x39b8] ; -> obj1:0x39B8
        mov    dword ptr [edx + 0x18], eax
        mov    byte ptr [ecx + 8], 0
        mov    word ptr [ecx + 0x22], 1
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [0x3980]       ; -> loc_1_3980
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], 0x3980   ; -> loc_1_3980
        mov    dword ptr [0x3980], edx       ; -> loc_1_3980
        mov    eax, dword ptr [edx]
        push   edx
        mov    dword ptr [eax + 4], edx
        inc    dword ptr [0x397c]            ; -> obj1:0x397C
        call   dword ptr [edx + 0x18]
        add    esp, 4
loc_1_08B6:
        ret    
sub_1_08B7:
        sub    esp, 0x160
        push   ebx
        push   esi
        push   edi
        mov    ax, word ptr [esp + 0x174]
        or     ax, ax
        jne    loc_1_08D7
        mov    eax, 8
        jmp    loc_1_09A7
loc_1_08D7:
        mov    dword ptr [esp + 0x5c], 0
        mov    dword ptr [esp + 0x70], 0
        mov    word ptr [esp + 0x74], ax
        mov    dword ptr [esp + 0x68], 0
        mov    ecx, 0xffffffff
        mov    eax, dword ptr [esp + 0x17c]
        mov    dword ptr [esp + 0x44], eax
        mov    edx, dword ptr [esp + 0x170]
        mov    edi, edx
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 0x2f
        jbe    loc_1_0923
        mov    eax, 4
        jmp    loc_1_09A7
loc_1_0923:
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 0x76]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0xc4]
        push   2
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x44]
        push   eax
        call   sub_1_0817
        add    esp, 0x10
        cmp    byte ptr [esp + 0x40], 0xff
        jne    loc_1_0983
        mov    ebx, 0xff
loc_1_096E:
        lea    eax, [esp + 0x38]
        push   1
        push   eax
        call   sub_1_01D3
        add    esp, 8
        cmp    byte ptr [esp + 0x40], bl
        je     loc_1_096E
loc_1_0983:
        cmp    byte ptr [esp + 0x43], 0
        jne    loc_1_09A2
        mov    edi, dword ptr [esp + 0x178]
        or     edi, edi
        je     loc_1_09A2
        lea    esi, [esp + 0x74]
        mov    ecx, 0x13
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
loc_1_09A2:
        movzx  eax, byte ptr [esp + 0x43]
loc_1_09A7:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x160
        ret    
sub_1_09B1:
        sub    esp, 0x168
        push   ebx
        push   esi
        push   edi
        push   ebp
        test   byte ptr [0x3920], 0x10       ; -> obj1:0x3920
        je     loc_1_09CE
        mov    eax, 6
        jmp    loc_1_0B3D
loc_1_09CE:
        push   0x44
        call   sub_1_00E6
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        or     eax, eax
        jne    loc_1_09EA
        mov    eax, 5
        jmp    loc_1_0B3D
loc_1_09EA:
        mov    ebp, dword ptr [esp + 0x188]
        or     ebp, ebp
        jne    loc_1_0A12
        lea    ebx, [esp + 0x44]
        lea    edx, [esp + 0x18]
        lea    eax, [esp + 0xd0]
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esp + 0x68], 0
        jmp    loc_1_0A63
loc_1_0A12:
        push   0x15a
        call   sub_1_00E6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_0A3B
        push   dword ptr [esp + 0x10]
        call   sub_1_00FA
        add    esp, 4
        mov    eax, 5
        jmp    loc_1_0B3D
loc_1_0A3B:
        lea    eax, [ebx + 0xb4]
        lea    edx, [ebx + 0x8a]
        mov    dword ptr [esp + 0x14], eax
        mov    byte ptr [ebp + 8], 0xff
        mov    dword ptr [ebp], ebx
        mov    dword ptr [ebx + 0x24], 0xb48 ; -> loc_1_0B48
        mov    dword ptr [ebx + 0x28], ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebp + 0xc], eax
loc_1_0A63:
        mov    ax, word ptr [esp + 0x180]
        mov    word ptr [ebx + 0x3c], ax
        mov    edi, dword ptr [esp + 0x17c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 0x30
        jbe    loc_1_0A91
        mov    eax, 4
        jmp    loc_1_0B3D
loc_1_0A91:
        mov    edi, dword ptr [esp + 0x17c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 0x3e]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x396c                   ; -> obj1:0x396C
        lea    edi, [ebx + 0x6e]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    si, word ptr [esp + 0x184]
        mov    word ptr [ebx + 0x78], si
        mov    eax, dword ptr [esp + 0x18c]
        push   3
        mov    dword ptr [ebx + 0xc], eax
        push   dword ptr [esp + 0x18]
        push   edx
        push   ebx
        call   sub_1_0817
        add    esp, 0x10
        or     ebp, ebp
        je     loc_1_0AF2
        sub    eax, eax
        jmp    loc_1_0B3D
loc_1_0AF2:
        cmp    byte ptr [ebx + 8], 0xff
        jne    loc_1_0B09
loc_1_0AF8:
        push   1
        push   ebx
        call   sub_1_01D3
        add    esp, 8
        cmp    byte ptr [ebx + 8], 0xff
        je     loc_1_0AF8
loc_1_0B09:
        cmp    byte ptr [ebx + 0xb], 0
        push   dword ptr [esp + 0x10]
        jne    loc_1_0B31
        movzx  eax, si
        push   eax
        movzx  eax, word ptr [esp + 0x188]
        push   eax
        push   dword ptr [esp + 0x188]
        call   sub_1_282B
        add    esp, 0x10
        jmp    loc_1_0B39
loc_1_0B31:
        call   sub_1_00FA
        add    esp, 4
loc_1_0B39:
        movzx  eax, byte ptr [ebx + 0xb]
loc_1_0B3D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x168
        ret    
loc_1_0B48:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edi + 0x28]
        cmp    byte ptr [edi + 0xb], 0
        push   dword ptr [esi + 0xc]
        jne    loc_1_0B72
        movzx  eax, word ptr [edi + 0x78]
        push   eax
        movzx  eax, word ptr [edi + 0x3c]
        push   eax
        lea    eax, [edi + 0x3e]
        push   eax
        call   sub_1_282B
        add    esp, 0x10
        jmp    loc_1_0B7A
loc_1_0B72:
        call   sub_1_00FA
        add    esp, 4
loc_1_0B7A:
        or     esi, esi
        je     loc_1_0B8E
        mov    al, byte ptr [edi + 0xb]
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 8], 0
        mov    dword ptr [esi], 0
loc_1_0B8E:
        push   edi
        call   sub_1_00FA
        add    esp, 4
        or     esi, esi
        je     loc_1_0BA8
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_0BA8
        push   esi
        call   eax
        add    esp, 4
loc_1_0BA8:
        pop    edi
        pop    esi
        ret    
sub_1_0BAB:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        call   sub_1_27D0
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_0BD2
        mov    eax, 1
        jmp    loc_1_0C76
loc_1_0BD2:
        push   0x15a
        call   sub_1_00E6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_0BF3
        push   ebp
        call   sub_1_28ED
        add    esp, 4
        jmp    loc_1_0C74
loc_1_0BF3:
        lea    eax, [ebx + 0x8a]
        mov    dword ptr [esp + 0x10], eax
        add    eax, 0x2a
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [ebx + 0x24], 0xfae ; -> loc_1_0FAE
        mov    dword ptr [ebx + 0x28], 0
        mov    eax, dword ptr [esp + 0x24]
        lea    edi, [ebp + 0x14]
        mov    dword ptr [ebx + 0xc], eax
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [ebx + 0x3c], ax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 0x3e]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x396c                   ; -> obj1:0x396C
        lea    edi, [ebx + 0x6e]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    ax, word ptr [ebp + 0xe]
        mov    word ptr [ebx + 0x78], ax
        push   ebp
        call   sub_1_28ED
        add    esp, 4
        push   4
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   ebx
        call   sub_1_0817
        add    esp, 0x10
loc_1_0C74:
        sub    eax, eax
loc_1_0C76:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_0C7E:
        sub    esp, 0x154
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0x7c], 0
        mov    ax, word ptr [esp + 0x168]
        mov    word ptr [esp + 0x94], ax
        mov    ecx, dword ptr [esp + 0x170]
        mov    dword ptr [esp + 0x8c], ecx
        mov    eax, dword ptr [esp + 0x174]
        mov    dword ptr [esp + 0x90], eax
        mov    eax, dword ptr [esp + 0x16c]
        mov    dword ptr [esp + 0x88], eax
        mov    eax, dword ptr [esp + 0x178]
        mov    dword ptr [esp + 0x64], eax
        sub    eax, eax
        mov    word ptr [ecx + 4], 0
        mov    dword ptr [ecx], 0
        mov    edx, dword ptr [esp + 0x164]
        mov    edi, edx
        mov    ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 0xf
        jbe    loc_1_0D04
        mov    eax, 4
        jmp    loc_1_0D90
loc_1_0D04:
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 0x96]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0xc]
        push   eax
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        lea    eax, [esp + 0x9a]
        push   ecx
        push   eax
        call   sub_1_01DF
        add    esp, 0xc
        lea    eax, [esp + 0xb8]
        push   5
        push   eax
        lea    eax, [esp + 0x34]
        push   eax
        lea    eax, [esp + 0x64]
        push   eax
        call   sub_1_0817
        add    esp, 0x10
        cmp    byte ptr [esp + 0x60], 0xff
        jne    loc_1_0D8B
        mov    ebx, 0xff
loc_1_0D76:
        lea    eax, [esp + 0x58]
        push   1
        push   eax
        call   sub_1_01D3
        add    esp, 8
        cmp    byte ptr [esp + 0x60], bl
        je     loc_1_0D76
loc_1_0D8B:
        movzx  eax, byte ptr [esp + 0x63]
loc_1_0D90:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x154
        ret    
sub_1_0D9A:
        sub    esp, 0x154
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x164]
        mov    eax, edx
        mov    ecx, 0x6a8                    ; -> obj1:0x06A8
loc_1_0DB1:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [ecx]
        jne    loc_1_0DD1
        or     bl, bl
        je     loc_1_0DCD
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [ecx + 1]
        jne    loc_1_0DD1
        add    eax, 2
        add    ecx, 2
        or     bl, bl
        jne    loc_1_0DB1
loc_1_0DCD:
        sub    eax, eax
        jmp    loc_1_0DD6
loc_1_0DD1:
        sbb    eax, eax
        sbb    eax, -1
loc_1_0DD6:
        or     eax, eax
        jne    loc_1_0DE1
        sub    eax, eax
        jmp    loc_1_0EB8
loc_1_0DE1:
        mov    dword ptr [esp + 0x7c], 0
        mov    word ptr [esp + 0x94], 0
        mov    dword ptr [esp + 0x8c], 0
        mov    dword ptr [esp + 0x90], 1
        mov    edi, edx
        mov    dword ptr [esp + 0x88], 0
        mov    eax, dword ptr [esp + 0x168]
        mov    dword ptr [esp + 0x64], eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 0xf
        jbe    loc_1_0E39
        mov    eax, 4
        jmp    loc_1_0EB8
loc_1_0E39:
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 0x96]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x9a]
        push   4
        push   eax
        call   sub_1_01DF
        add    esp, 0xc
        lea    eax, [esp + 0xb8]
        push   7
        push   eax
        lea    eax, [esp + 0x34]
        push   eax
        lea    eax, [esp + 0x64]
        push   eax
        call   sub_1_0817
        add    esp, 0x10
        cmp    byte ptr [esp + 0x60], 0xff
        jne    loc_1_0EB3
        mov    ebx, 0xff
loc_1_0E9E:
        lea    eax, [esp + 0x58]
        push   1
        push   eax
        call   sub_1_01D3
        add    esp, 8
        cmp    byte ptr [esp + 0x60], bl
        je     loc_1_0E9E
loc_1_0EB3:
        movzx  eax, byte ptr [esp + 0x63]
loc_1_0EB8:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x154
        ret    
sub_1_0EC2:
        sub    esp, 0x134
        push   ebx
        mov    dword ptr [esp + 0x54], 0
        mov    ecx, dword ptr [esp + 0x140]
        mov    dword ptr [esp + 0x64], ecx
        mov    eax, dword ptr [esp + 0x144]
        mov    dword ptr [esp + 0x68], eax
        mov    eax, dword ptr [esp + 0x13c]
        mov    dword ptr [esp + 0x60], eax
        mov    eax, dword ptr [esp + 0x148]
        mov    dword ptr [esp + 0x3c], eax
        lea    eax, [esp + 0x90]
        push   6
        push   eax
        mov    dword ptr [ecx], 0
        lea    eax, [esp + 0xc]
        push   eax
        mov    byte ptr [ecx + 4], 0
        lea    eax, [esp + 0x3c]
        push   eax
        call   sub_1_0817
        add    esp, 0x10
        cmp    byte ptr [esp + 0x38], 0xff
        jne    loc_1_0F44
        mov    ebx, 0xff
loc_1_0F2F:
        lea    eax, [esp + 0x30]
        push   1
        push   eax
        call   sub_1_01D3
        add    esp, 8
        cmp    byte ptr [esp + 0x38], bl
        je     loc_1_0F2F
loc_1_0F44:
        movzx  eax, byte ptr [esp + 0x3b]
        pop    ebx
        add    esp, 0x134
        ret    
sub_1_0F51:
        test   dword ptr [0x3920], 0x2082    ; -> obj1:0x3920
        jne    loc_1_0F87
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        lea    ecx, [eax + 0x68]
        add    eax, 0x3e
        mov    edx, dword ptr [0x3988]       ; -> obj1:0x3988
        push   1
        mov    dword ptr [edx + 0x24], 0
        push   ecx
        push   eax
        push   dword ptr [0x3988]            ; -> obj1:0x3988
        call   sub_1_0817
        add    esp, 0x10
loc_1_0F87:
        ret    
loc_1_0F88:
        mov    dword ptr [0x3934], 0         ; -> obj1:0x3934
        test   dword ptr [0x3920], 0x2082    ; -> obj1:0x3920
        jne    loc_1_0FAD
        push   0
        push   0xf51                         ; -> sub_1_0F51
        call   sub_1_00BF
        add    esp, 8
loc_1_0FAD:
        ret    
loc_1_0FAE:
        push   dword ptr [esp + 4]
        call   sub_1_00FA
        add    esp, 4
        ret    
sub_1_0FBB:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        cmp    byte ptr [esi + 0xa], 0xff
        je     loc_1_0FCE
        mov    al, byte ptr [esp + 0x10]
        mov    byte ptr [esi + 0xb], al
loc_1_0FCE:
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_0FE5
        push   eax
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [esi + 0x10], 0
loc_1_0FE5:
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_1_0FFC
        push   eax
        call   sub_1_00D6
        add    esp, 4
        mov    dword ptr [esi + 0x14], 0
loc_1_0FFC:
        mov    eax, dword ptr [esi + 0x20]
        mov    edi, dword ptr [esp + 0x14]
        cmp    byte ptr [eax + 8], 0
        je     loc_1_1034
        or     edi, edi
        je     loc_1_1018
        push   eax
        call   sub_1_0156
        add    esp, 4
        jmp    loc_1_1034
loc_1_1018:
        mov    byte ptr [esi + 0xa], 0xff
        push   esi
        push   0x4a5                         ; -> loc_1_04A5
        push   0x1f4
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
        jmp    loc_1_1073
loc_1_1034:
        or     edi, edi
        jne    loc_1_105E
        cmp    dword ptr [esi + 0x24], 0
        je     loc_1_1051
        push   esi
        push   0x4b6                         ; -> loc_1_04B6
        call   sub_1_009C
        add    esp, 8
        mov    dword ptr [esi + 0x14], eax
        jmp    loc_1_1055
loc_1_1051:
        mov    byte ptr [esi + 8], 0
loc_1_1055:
        push   esi
        call   sub_1_01D9
        add    esp, 4
loc_1_105E:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        dec    dword ptr [0x397c]            ; -> obj1:0x397C
loc_1_1073:
        pop    edi
        pop    esi
        ret    
sub_1_1076:
        mov    ecx, dword ptr [0x3980]       ; -> loc_1_3980
        cmp    ecx, 0x3980                   ; -> loc_1_3980
        je     loc_1_1098
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
loc_1_108A:
        cmp    ecx, eax
        je     loc_1_109E
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0x3980                   ; -> loc_1_3980
        jne    loc_1_108A
loc_1_1098:
        mov    eax, 0xffffffff
loc_1_109D:
        ret    
loc_1_109E:
        mov    dword ptr [ecx + 0x28], 0
        push   0
        push   7
        push   ecx
        call   sub_1_0FBB
        add    esp, 0xc
        sub    eax, eax
        jmp    loc_1_109D
sub_1_10B6:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0x3980]       ; -> loc_1_3980
        cmp    esi, 0x3980                   ; -> loc_1_3980
        je     loc_1_111C
        mov    bl, byte ptr [esp + 0x10]
loc_1_10CB:
        mov    edi, dword ptr [esi]
        movzx  eax, byte ptr [esi + 9]
        cmp    eax, 1
        je     loc_1_10E7
        cmp    eax, 3
        je     loc_1_10F5
        cmp    eax, 5
        jl     loc_1_1112
        cmp    eax, 7
        jle    loc_1_10F5
        jmp    loc_1_1112
loc_1_10E7:
        push   0
        push   ebx
        push   esi
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_1112
loc_1_10F5:
        cmp    byte ptr [esi + 0xa], 1
        jne    loc_1_1112
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_1_1112
        push   esi
        push   0x496                         ; -> loc_1_0496
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [esi + 0x14], eax
loc_1_1112:
        mov    esi, edi
        cmp    edi, 0x3980                   ; -> loc_1_3980
        jne    loc_1_10CB
loc_1_111C:
        and    dword ptr [0x3920], 0xfffff77f ; -> obj1:0x3920
        mov    word ptr [0x3a38], 0x440      ; -> loc_1_3A38
        cmp    dword ptr [0x398c], 0         ; -> obj1:0x398C
        je     loc_1_1147
        push   0
        push   0x2ae7                        ; -> sub_1_2AE7
        call   sub_1_009C
        add    esp, 8
loc_1_1147:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_114B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 8], 0xff
        jne    loc_1_11AB
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_116D
        push   eax
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [esi + 0x10], 0
loc_1_116D:
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_1_1184
        push   eax
        call   sub_1_00D6
        add    esp, 4
        mov    dword ptr [esi + 0x14], 0
loc_1_1184:
        mov    eax, dword ptr [esi + 0x20]
        push   esi
        cmp    byte ptr [eax + 8], 0
        je     loc_1_11A5
        push   0x487                         ; -> loc_1_0487
        push   0x96
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
        jmp    loc_1_11AB
loc_1_11A5:
        call   dword ptr [esi + 0x18]
        add    esp, 4
loc_1_11AB:
        pop    esi
        ret    
loc_1_11AD:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        movzx  eax, byte ptr [esi + 0xa]
        or     eax, eax
        je     loc_1_11F2
        cmp    eax, 1
        je     loc_1_1223
        cmp    eax, 2
        je     loc_1_131F
        mov    edi, dword ptr [esp + 0xc]
loc_1_11D0:
        push   esi
        push   0x487                         ; -> loc_1_0487
        push   edi
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
        push   dword ptr [esi + 0x20]
        call   sub_1_013F
        add    esp, 4
        jmp    loc_1_1363
loc_1_11F2:
        mov    dword ptr [esi + 0x1c], 2
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [esi + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
        test   byte ptr [0x3920], 0x10       ; -> obj1:0x3920
        jne    loc_1_125F
        test   dword ptr [0x3920], 0x2002    ; -> obj1:0x3920
        je     loc_1_125F
        mov    byte ptr [esi + 0xa], 1
loc_1_1223:
        test   byte ptr [0x3921], 4          ; -> obj1:0x3921
        jne    loc_1_12BD
        test   byte ptr [0x3920], 2          ; -> obj1:0x3920
        je     loc_1_1276
        cmp    dword ptr [0x3938], 0         ; -> obj1:0x3938
        jne    loc_1_130A
        push   0
        push   0x2654                        ; -> loc_1_2654
        call   sub_1_009C
        add    esp, 8
        mov    dword ptr [0x3938], eax       ; -> obj1:0x3938
        jmp    loc_1_130A
loc_1_125F:
        test   byte ptr [0x3920], 0x20       ; -> obj1:0x3920
        jne    loc_1_126D
        call   sub_1_0F51
loc_1_126D:
        mov    byte ptr [esi + 0xa], 2
        jmp    loc_1_131F
loc_1_1276:
        lea    eax, [esp + 8]
        push   eax
        push   0x3a44                        ; -> obj1:0x3A44
        push   0x3944                        ; -> obj1:0x3944
        call   sub_1_01A4
        add    esp, 0xc
        or     eax, eax
        je     loc_1_12A2
        and    dword ptr [0x3920], 0xffffdfff ; -> obj1:0x3920
        call   sub_1_0F51
        jmp    loc_1_130A
loc_1_12A2:
        or     dword ptr [0x3920], 0x400     ; -> obj1:0x3920
        mov    eax, dword ptr [esp + 8]
        imul   eax, eax, 0x37
        add    eax, 0x1f4
        mov    dword ptr [0x392c], eax       ; -> obj1:0x392C
loc_1_12BD:
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, eax
        je     loc_1_12EB
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        mov    edi, dword ptr [0x392c]       ; -> obj1:0x392C
        push   1
        push   0
        push   esi
        push   0
        mov    ax, word ptr [esi + 0x3c]
        push   eax
        lea    eax, [esi + 0x3e]
        push   eax
        call   sub_1_3233
        add    esp, 0x18
        jmp    loc_1_11D0
loc_1_12EB:
        test   byte ptr [0x3921], 0x20       ; -> obj1:0x3921
        je     loc_1_1303
        and    dword ptr [0x3920], 0xffffdbff ; -> obj1:0x3920
        call   sub_1_0F51
loc_1_1303:
        mov    dword ptr [esi + 0x1c], 2
loc_1_130A:
        mov    byte ptr [esi + 0xa], 2
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [esi + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
loc_1_131F:
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, eax
        je     loc_1_1356
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        mov    edi, dword ptr [0x3978]       ; -> obj1:0x3978
        push   2
        push   0x3950                        ; -> obj1:0x3950
        push   esi
        push   0
        add    edi, 0xfa
        mov    ax, word ptr [esi + 0x3c]
        push   eax
        lea    eax, [esi + 0x3e]
        push   eax
        call   sub_1_3233
        add    esp, 0x18
        jmp    loc_1_11D0
loc_1_1356:
        push   0
        push   1
        push   esi
        call   sub_1_0FBB
        add    esp, 0xc
loc_1_1363:
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_1_1369:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        movzx  eax, byte ptr [esi + 0xa]
        or     eax, eax
        je     loc_1_13B6
        cmp    eax, 1
        je     loc_1_1428
        cmp    eax, 2
        je     loc_1_13F1
        cmp    eax, 3
        je     loc_1_1510
        mov    eax, dword ptr [esp + 8]
loc_1_1394:
        push   esi
        push   0x487                         ; -> loc_1_0487
        push   eax
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
        push   dword ptr [esi + 0x20]
        call   sub_1_013F
        add    esp, 4
loc_1_13B1:
        pop    esi
        add    esp, 8
        ret    
loc_1_13B6:
        test   dword ptr [0x3920], 0x2002    ; -> obj1:0x3920
        jne    loc_1_13D1
        mov    byte ptr [esi + 0xb], 1
        mov    byte ptr [esi + 0xa], 1
        call   sub_1_0F51
        jmp    loc_1_13B1
loc_1_13D1:
        mov    byte ptr [esi + 0xb], 0
loc_1_13D5:
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [esi + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
        mov    byte ptr [esi + 0xa], 2
        mov    dword ptr [esi + 0x1c], 3
loc_1_13F1:
        test   byte ptr [0x3921], 4          ; -> obj1:0x3921
        jne    loc_1_1494
        lea    eax, [esp + 4]
        push   eax
        push   0x3a44                        ; -> obj1:0x3A44
        push   0x3944                        ; -> obj1:0x3944
        call   sub_1_01A4
        add    esp, 0xc
        or     eax, eax
        je     loc_1_1479
        mov    byte ptr [esi + 0xb], 1
        mov    byte ptr [esi + 0xa], 1
        call   sub_1_0F51
        jmp    loc_1_13B1
loc_1_1428:
        test   dword ptr [0x3920], 0x2002    ; -> obj1:0x3920
        jne    loc_1_13D5
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0xc
        cmp    dword ptr [0x3934], 0         ; -> obj1:0x3934
        jne    loc_1_13B1
        test   byte ptr [0x3920], 0x40       ; -> obj1:0x3920
        je     loc_1_13B1
        push   0
        push   0xf88                         ; -> loc_1_0F88
        push   0x927c0
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [0x3934], eax       ; -> obj1:0x3934
        jmp    loc_1_13B1
loc_1_1479:
        or     dword ptr [0x3920], 0x400     ; -> obj1:0x3920
        mov    eax, dword ptr [esp + 4]
        imul   eax, eax, 0x37
        add    eax, 0x1f4
        mov    dword ptr [0x392c], eax       ; -> obj1:0x392C
loc_1_1494:
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, eax
        je     loc_1_14F4
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        test   byte ptr [0x3921], 0x20       ; -> obj1:0x3921
        je     loc_1_14CA
        push   1
        push   0
        push   esi
        push   0
        mov    ax, word ptr [esi + 0x3c]
        push   eax
        lea    eax, [esi + 0x3e]
        push   eax
        call   sub_1_3233
        add    esp, 0x18
        mov    eax, dword ptr [0x392c]       ; -> obj1:0x392C
        jmp    loc_1_1394
loc_1_14CA:
        push   2
        push   0x3950                        ; -> obj1:0x3950
        push   esi
        push   0
        mov    ax, word ptr [esi + 0x3c]
        push   eax
        lea    eax, [esi + 0x3e]
        push   eax
        call   sub_1_3233
        add    esp, 0x18
        mov    eax, dword ptr [0x3978]       ; -> obj1:0x3978
        add    eax, 0xfa
        jmp    loc_1_1394
loc_1_14F4:
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [esi + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
        mov    byte ptr [esi + 0xa], 3
        mov    dword ptr [esi + 0x1c], 3
loc_1_1510:
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, eax
        je     loc_1_153E
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        push   esi
        mov    ax, word ptr [esi + 0x78]
        push   eax
        mov    ax, word ptr [esi + 0x3c]
        push   eax
        lea    eax, [esi + 0x3e]
        push   eax
        push   0x10
        call   sub_1_3196
        add    esp, 0x14
        mov    eax, dword ptr [0x392c]       ; -> obj1:0x392C
        jmp    loc_1_1394
loc_1_153E:
        and    dword ptr [0x3920], 0xffffdbfd ; -> obj1:0x3920
        cmp    byte ptr [esi + 0xb], 0
        jne    loc_1_1560
        mov    byte ptr [esi + 0xa], 1
        mov    byte ptr [esi + 0xb], 1
        call   sub_1_0F51
        jmp    loc_1_13B1
loc_1_1560:
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_13B1
loc_1_1572:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x18]
        movzx  eax, byte ptr [ebx + 0xa]
        or     eax, eax
        je     loc_1_15CE
        cmp    eax, 1
        je     loc_1_1637
        cmp    eax, 2
        je     loc_1_17B4
        cmp    eax, 3
        je     loc_1_1807
        cmp    eax, 5
        je     loc_1_16BB
        mov    edi, dword ptr [esp + 0x10]
loc_1_15AC:
        push   ebx
        push   0x487                         ; -> loc_1_0487
        push   edi
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [ebx + 0x10], eax
        push   dword ptr [ebx + 0x20]
        call   sub_1_013F
        add    esp, 4
        jmp    loc_1_1841
loc_1_15CE:
        test   dword ptr [0x3920], 0x2002    ; -> obj1:0x3920
        jne    loc_1_1637
        test   byte ptr [0x3920], 0x20       ; -> obj1:0x3920
        jne    loc_1_15EC
        mov    byte ptr [ebx + 0xa], 1
        call   sub_1_0F51
loc_1_15EC:
        mov    eax, 0x6a8                    ; -> obj1:0x06A8
        lea    ecx, [ebx + 0x3e]
loc_1_15F4:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    loc_1_1614
        or     dl, dl
        je     loc_1_1610
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    loc_1_1614
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    loc_1_15F4
loc_1_1610:
        sub    eax, eax
        jmp    loc_1_1619
loc_1_1614:
        sbb    eax, eax
        sbb    eax, -1
loc_1_1619:
        or     eax, eax
        jne    loc_1_1637
        test   byte ptr [0x3920], 0x20       ; -> obj1:0x3920
        je     loc_1_1841
        push   0
        push   0
        push   ebx
        call   sub_1_0FBB
        add    esp, 0xc
loc_1_1637:
        test   byte ptr [0x3921], 0x20       ; -> obj1:0x3921
        je     loc_1_1642
        jmp    loc_1_169F
loc_1_1642:
        mov    eax, 0x6a8                    ; -> obj1:0x06A8
        lea    ecx, [ebx + 0x3e]
loc_1_164A:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    loc_1_166A
        or     dl, dl
        je     loc_1_1666
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    loc_1_166A
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    loc_1_164A
loc_1_1666:
        sub    eax, eax
        jmp    loc_1_166F
loc_1_166A:
        sbb    eax, eax
        sbb    eax, -1
loc_1_166F:
        or     eax, eax
        jne    loc_1_17A9
        test   byte ptr [0x3920], 2          ; -> obj1:0x3920
        jne    loc_1_1689
        push   0
        push   0
        jmp    loc_1_1838
loc_1_1689:
        lea    edi, [ebx + 0x4e]
        mov    esi, 0x3a44                   ; -> obj1:0x3A44
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, 0x3944                   ; -> obj1:0x3944
        lea    edi, [ebx + 0x54]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_1_169F:
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [ebx + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
        mov    byte ptr [ebx + 0xa], 5
        mov    dword ptr [ebx + 0x1c], 3
loc_1_16BB:
        cmp    dword ptr [ebx + 0x1c], 0
        jbe    loc_1_175A
        test   byte ptr [0x3921], 0x20       ; -> obj1:0x3921
        je     loc_1_16EE
        push   1
        push   0
        push   ebx
        lea    eax, [ebx + 0x3e]
        push   eax
        mov    ax, word ptr [ebx + 0x3c]
        push   eax
        push   0
        call   sub_1_3233
        add    esp, 0x18
        mov    edi, dword ptr [0x392c]       ; -> obj1:0x392C
        jmp    loc_1_1752
loc_1_16EE:
        lea    esi, [ebx + 0x4e]
        cmp    dword ptr [esi], 0
        jne    loc_1_1724
        lea    eax, [esp + 0xc]
        push   eax
        push   esi
        lea    eax, [ebx + 0x54]
        push   eax
        call   sub_1_01A4
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_171B
        mov    edi, dword ptr [esp + 0xc]
        imul   edi, edi, 0x37
        add    edi, 0x1f4
        jmp    loc_1_173B
loc_1_171B:
        push   0
        push   0
        jmp    loc_1_1838
loc_1_1724:
        cmp    dword ptr [0x392c], 0         ; -> obj1:0x392C
        je     loc_1_1735
        mov    edi, dword ptr [0x392c]       ; -> obj1:0x392C
        jmp    loc_1_173B
loc_1_1735:
        mov    edi, dword ptr [0x3978]       ; -> obj1:0x3978
loc_1_173B:
        push   1
        push   esi
        push   ebx
        lea    eax, [ebx + 0x3e]
        push   eax
        mov    ax, word ptr [ebx + 0x3c]
        push   eax
        push   0
        call   sub_1_3233
        add    esp, 0x18
loc_1_1752:
        dec    dword ptr [ebx + 0x1c]
        jmp    loc_1_15AC
loc_1_175A:
        push   0
        push   0xff
        push   ebx
        call   sub_1_0FBB
        add    esp, 0xc
        test   byte ptr [0x3921], 0x20       ; -> obj1:0x3921
        jne    loc_1_1795
        test   byte ptr [0x3920], 2          ; -> obj1:0x3920
        je     loc_1_1841
        lea    esi, [ebx + 0x54]
        mov    edi, 0x3944                   ; -> obj1:0x3944
        mov    ecx, 3
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_1841
loc_1_1795:
        and    dword ptr [0x3920], 0xffffdbfd ; -> obj1:0x3920
        call   sub_1_0F51
        jmp    loc_1_1841
loc_1_17A9:
        mov    byte ptr [ebx + 0xa], 2
        mov    dword ptr [ebx + 0x1c], 3
loc_1_17B4:
        test   byte ptr [0x3920], 2          ; -> obj1:0x3920
        je     loc_1_17FC
        cmp    dword ptr [ebx + 0x1c], 0
        jbe    loc_1_17E4
        lea    eax, [ebx + 0x3e]
        push   eax
        push   5
        push   1
        push   dword ptr [ebx + 0x20]
        call   sub_1_1CFD
        add    esp, 0x10
        mov    edi, dword ptr [0x392c]       ; -> obj1:0x392C
        dec    dword ptr [ebx + 0x1c]
        jmp    loc_1_15AC
loc_1_17E4:
        and    dword ptr [0x3920], 0xfffffbfd ; -> obj1:0x3920
        test   byte ptr [0x3920], 0x20       ; -> obj1:0x3920
        jne    loc_1_17FC
        call   sub_1_0F51
loc_1_17FC:
        mov    byte ptr [ebx + 0xa], 3
        mov    dword ptr [ebx + 0x1c], 3
loc_1_1807:
        cmp    dword ptr [ebx + 0x1c], 0
        jbe    loc_1_1834
        lea    eax, [ebx + 0x3e]
        push   eax
        push   0xd
        push   3
        push   dword ptr [ebx + 0x20]
        call   sub_1_1CFD
        add    esp, 0x10
        mov    edi, dword ptr [0x3978]       ; -> obj1:0x3978
        add    edi, 0x1f4
        dec    dword ptr [ebx + 0x1c]
        jmp    loc_1_15AC
loc_1_1834:
        push   0
        push   3
loc_1_1838:
        push   ebx
        call   sub_1_0FBB
        add    esp, 0xc
loc_1_1841:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_1848:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        movzx  eax, byte ptr [esi + 0xa]
        or     eax, eax
        je     loc_1_18A0
        cmp    eax, 1
        je     loc_1_1933
        cmp    eax, 2
        je     loc_1_1968
        cmp    eax, 4
        je     loc_1_18BB
        cmp    eax, 5
        je     loc_1_1910
        mov    edi, dword ptr [esp + 0xc]
loc_1_187D:
        push   esi
        push   0x487                         ; -> loc_1_0487
        push   edi
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
        push   dword ptr [esi + 0x20]
        call   sub_1_013F
        add    esp, 4
loc_1_189A:
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_1_18A0:
        test   dword ptr [0x3920], 0x2022    ; -> obj1:0x3920
        jne    loc_1_1933
        mov    byte ptr [esi + 0xa], 1
        call   sub_1_0F51
        jmp    loc_1_189A
loc_1_18BB:
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esi + 0x4e]
        push   eax
        lea    eax, [esi + 0x54]
        push   eax
        call   sub_1_01A4
        add    esp, 0xc
        or     eax, eax
        je     loc_1_18E3
        push   0
        push   3
        push   esi
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_189A
loc_1_18E3:
        mov    eax, dword ptr [esp + 8]
        imul   eax, eax, 0x37
        add    eax, 0x1f4
        mov    dword ptr [0x392c], eax       ; -> obj1:0x392C
        mov    byte ptr [esi + 0xa], 5
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [esi + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
        mov    dword ptr [esi + 0x1c], 3
loc_1_1910:
        cmp    dword ptr [esi + 0x1c], 0
        jbe    loc_1_19F8
        test   dword ptr [0x3920], 0x2002    ; -> obj1:0x3920
        push   1
        je     loc_1_19C6
        push   0
        jmp    loc_1_19CA
loc_1_1933:
        test   dword ptr [0x3920], 0x2002    ; -> obj1:0x3920
        je     loc_1_195D
        mov    byte ptr [esi + 0xa], 5
        mov    dword ptr [esi + 0x1c], 3
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [esi + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
        jmp    loc_1_1910
loc_1_195D:
        mov    byte ptr [esi + 0xa], 2
        mov    dword ptr [esi + 0x1c], 4
loc_1_1968:
        mov    edi, dword ptr [0x3978]       ; -> obj1:0x3978
        mov    eax, dword ptr [esi + 0x1c]
        cmp    eax, 2
        jbe    loc_1_1993
        push   0x300
        push   0x7b06
        push   dword ptr [esi + 0x20]
        call   sub_1_1E19
        add    esp, 0xc
        dec    dword ptr [esi + 0x1c]
        jmp    loc_1_187D
loc_1_1993:
        or     eax, eax
        je     loc_1_19B4
        push   0x100
        push   0x7b06
        push   dword ptr [esi + 0x20]
        call   sub_1_1E19
        add    esp, 0xc
        dec    dword ptr [esi + 0x1c]
        jmp    loc_1_187D
loc_1_19B4:
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_189A
loc_1_19C6:
        lea    eax, [esi + 0x4e]
        push   eax
loc_1_19CA:
        push   esi
        push   0
        push   0
        push   0
        call   sub_1_3233
        add    esp, 0x18
        cmp    dword ptr [0x392c], 0         ; -> obj1:0x392C
        je     loc_1_19EA
        mov    edi, dword ptr [0x392c]       ; -> obj1:0x392C
        jmp    loc_1_19F0
loc_1_19EA:
        mov    edi, dword ptr [0x3978]       ; -> obj1:0x3978
loc_1_19F0:
        dec    dword ptr [esi + 0x1c]
        jmp    loc_1_187D
loc_1_19F8:
        test   dword ptr [0x3920], 0x2002    ; -> obj1:0x3920
        je     loc_1_1A1C
        and    dword ptr [0x3920], 0xffffdbfd ; -> obj1:0x3920
        test   byte ptr [0x3920], 0x20       ; -> obj1:0x3920
        jne    loc_1_1A1C
        call   sub_1_0F51
loc_1_1A1C:
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_189A
loc_1_1A2E:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  eax, byte ptr [esi + 0xa]
        or     eax, eax
        je     loc_1_1A42
        cmp    eax, 1
        je     loc_1_1A6C
        jmp    loc_1_1A90
loc_1_1A42:
        test   dword ptr [0x3920], 0x2002    ; -> obj1:0x3920
        jne    loc_1_1A50
        jmp    loc_1_1AB4
loc_1_1A50:
        mov    byte ptr [esi + 0xa], 1
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [esi + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
        mov    dword ptr [esi + 0x1c], 3
loc_1_1A6C:
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, eax
        je     loc_1_1AB4
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        push   esi
        mov    ax, word ptr [esi + 0x78]
        push   eax
        mov    ax, word ptr [esi + 0x3c]
        push   eax
        lea    eax, [esi + 0x3e]
        push   eax
        push   0x13
        call   sub_1_3196
        add    esp, 0x14
loc_1_1A90:
        push   esi
        push   0x487                         ; -> loc_1_0487
        push   dword ptr [0x392c]            ; -> obj1:0x392C
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
        push   dword ptr [esi + 0x20]
        call   sub_1_013F
        add    esp, 4
        jmp    loc_1_1AC1
loc_1_1AB4:
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0xc
loc_1_1AC1:
        pop    esi
        ret    
loc_1_1AC3:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        movzx  eax, byte ptr [esi + 0xa]
        cmp    eax, 5
        ja     loc_1_1B1D
        jmp    dword ptr [eax*4 + 0x1ce5]    ; -> obj1:0x1CE5
loc_1_1ADB:
        test   byte ptr [0x3920], 0x80       ; -> obj1:0x3920
        jne    loc_1_1B3E
        or     dword ptr [0x3920], 0x80      ; -> obj1:0x3920
        mov    byte ptr [esi + 0xa], 1
        mov    dword ptr [esi + 0x1c], 4
loc_1_1AF9:
        mov    eax, dword ptr [esi + 0x1c]
        cmp    eax, 2
        jbe    loc_1_1BB9
        push   0x300
loc_1_1B0A:
        push   0x5006
        push   dword ptr [esi + 0x20]
        call   sub_1_1E19
        add    esp, 0xc
loc_1_1B1A:
        dec    dword ptr [esi + 0x1c]
loc_1_1B1D:
        push   esi
        push   0x487                         ; -> loc_1_0487
        push   0x2ee
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
        push   dword ptr [esi + 0x20]
        call   sub_1_013F
loc_1_1B3B:
        add    esp, 4
loc_1_1B3E:
        pop    esi
        add    esp, 4
        ret    
loc_1_1B43:
        cmp    dword ptr [esi + 0x1c], 0
        jbe    loc_1_1BA3
        lea    eax, [esp + 4]
        push   eax
        push   0x3a44                        ; -> obj1:0x3A44
        push   0x3944                        ; -> obj1:0x3944
        call   sub_1_01A4
        add    esp, 0xc
        or     eax, eax
        je     loc_1_1B7F
        dec    dword ptr [esi + 0x1c]
        push   esi
        push   0x487                         ; -> loc_1_0487
        push   0x12c
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
        jmp    loc_1_1B3E
loc_1_1B7F:
        mov    eax, dword ptr [esp + 4]
        imul   eax, eax, 0x37
        add    eax, 0x1f4
        mov    dword ptr [0x392c], eax       ; -> obj1:0x392C
        or     dword ptr [0x3920], 0x400     ; -> obj1:0x3920
        push   0
        call   sub_1_10B6
        jmp    loc_1_1B3B
loc_1_1BA3:
        and    dword ptr [0x3920], 0xffffdffd ; -> obj1:0x3920
        push   3
        call   sub_1_10B6
        add    esp, 4
        jmp    loc_1_1C35
loc_1_1BB9:
        or     eax, eax
        je     loc_1_1BC7
        push   0x100
        jmp    loc_1_1B0A
loc_1_1BC7:
        test   byte ptr [0x3920], 0x10       ; -> obj1:0x3920
        je     loc_1_1BDC
        push   6
        call   sub_1_10B6
        jmp    loc_1_1B3B
loc_1_1BDC:
        mov    byte ptr [esi + 0xa], 2
        mov    dword ptr [esi + 0x1c], 2
loc_1_1BE7:
        cmp    dword ptr [esi + 0x1c], 0
        jbe    loc_1_1C03
        push   0
        push   0xd
        push   2
        push   dword ptr [esi + 0x20]
        call   sub_1_1CFD
        add    esp, 0x10
        jmp    loc_1_1B1A
loc_1_1C03:
        mov    byte ptr [esi + 0xa], 3
        mov    dword ptr [esi + 0x1c], 2
loc_1_1C0E:
        cmp    dword ptr [esi + 0x1c], 0
        jbe    loc_1_1C2A
        push   0
        push   0xd
        push   3
        push   dword ptr [esi + 0x20]
        call   sub_1_1CFD
        add    esp, 0x10
        jmp    loc_1_1B1A
loc_1_1C2A:
        mov    byte ptr [esi + 0xa], 5
        mov    dword ptr [esi + 0x1c], 1
loc_1_1C35:
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, eax
        je     loc_1_1CC5
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        test   byte ptr [0x3921], 0x40       ; -> obj1:0x3921
        jne    loc_1_1CAA
        cmp    dword ptr [0x39b0], 0         ; -> obj1:0x39B0
        je     loc_1_1C6E
        push   dword ptr [0x39b0]            ; -> obj1:0x39B0
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [0x39b0], 0         ; -> obj1:0x39B0
loc_1_1C6E:
        call   sub_1_01CC
        mov    dword ptr [0x39a0], eax       ; -> obj1:0x39A0
        mov    dword ptr [0x39a8], 0x1d      ; -> obj1:0x39A8
        mov    dword ptr [0x39a4], 4         ; -> obj1:0x39A4
        call   sub_1_01CC
        add    eax, 0x3e8
        mov    dword ptr [0x39b4], eax       ; -> obj1:0x39B4
        or     dword ptr [0x3920], 0x4000    ; -> obj1:0x3920
        call   sub_1_1E95
loc_1_1CAA:
        push   esi
        push   0x487                         ; -> loc_1_0487
        push   0x4e20
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
        jmp    loc_1_1B3E
loc_1_1CC5:
        push   3
        call   sub_1_10B6
        add    esp, 4
        or     dword ptr [0x3920], 0x20      ; -> obj1:0x3920
        mov    dword ptr [0x393c], 0x1e      ; -> obj1:0x393C
        jmp    loc_1_1B3E
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_1ADB
        dd  loc_1_1AF9
        dd  loc_1_1BE7
        dd  loc_1_1C0E
        dd  loc_1_1B43
        dd  loc_1_1C35
sub_1_1CFD:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + 4], 0
        mov    ax, word ptr [0x3998]         ; -> obj1:0x3998
        mov    word ptr [ecx + 0xa], ax
        mov    ebp, dword ptr [ecx + 0x24]
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, 1
        je     loc_1_1D4E
        cmp    eax, 2
        jb     loc_1_1D36
        cmp    eax, 3
        jbe    loc_1_1D66
        cmp    eax, 4
        je     loc_1_1D84
        cmp    eax, 5
        je     loc_1_1D9C
loc_1_1D36:
        mov    ebx, dword ptr [esp + 0x20]
loc_1_1D3A:
        cmp    eax, 3
        jne    loc_1_1DB3
        lea    edx, [ebp + 0x3e]
        mov    byte ptr [ebp + 5], 0x14
        mov    word ptr [ecx + 0x28], 0x66
        jmp    loc_1_1DC0
loc_1_1D4E:
        mov    esi, 0x3a44                   ; -> obj1:0x3A44
        lea    edi, [ecx + 0x1c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, 0x3944                   ; -> obj1:0x3944
        lea    edi, [ebp + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_1D36
loc_1_1D66:
        mov    esi, 0x3964                   ; -> obj1:0x3964
        lea    edi, [ecx + 0x1c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    dword ptr [ebp + 6], 0
        mov    esi, 0x3964                   ; -> obj1:0x3964
        lea    edi, [ebp + 0xa]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_1D36
loc_1_1D84:
        mov    esi, 0x3970                   ; -> obj1:0x3970
        lea    edi, [ecx + 0x1c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, 0x396c                   ; -> obj1:0x396C
        lea    edi, [ebp + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_1D36
loc_1_1D9C:
        lea    edi, [ecx + 0x1c]
        mov    ebx, dword ptr [esp + 0x20]
        mov    esi, ebx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    esi, [ebx + 6]
        lea    edi, [ebp + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_1D3A
loc_1_1DB3:
        lea    edx, [ebp + 0x1e]
        mov    byte ptr [ebp + 5], 4
        mov    word ptr [ecx + 0x28], 0x46
loc_1_1DC0:
        mov    dword ptr [edx], 0x53575242
        mov    dword ptr [edx + 8], 0x10006
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edx + 0x14], eax
        cmp    eax, 0xd
        mov    ecx, dword ptr [0x3924]       ; -> obj1:0x3924
        mov    dword ptr [edx + 0xc], ecx
        je     loc_1_1DE7
        cmp    eax, 5
        jne    loc_1_1DEF
loc_1_1DE7:
        or     ebx, ebx
        je     loc_1_1DEF
        mov    edi, ebx
        jmp    loc_1_1DF4
loc_1_1DEF:
        mov    edi, 0x6a8                    ; -> obj1:0x06A8
loc_1_1DF4:
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 0x18]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1E19:
        push   edi
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 4], 0
        mov    ax, word ptr [0x3998]         ; -> obj1:0x3998
        lea    edi, [ecx + 0x1c]
        mov    word ptr [ecx + 0xa], ax
        mov    eax, 0xffffffff
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    word ptr [ecx + 0x28], 0x22
        mov    ecx, dword ptr [ecx + 0x24]
        mov    byte ptr [ecx + 5], 4
        lea    edi, [ecx + 0xa]
        mov    dword ptr [ecx + 6], 0
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    word ptr [ecx + 0x10], 0x5204
        mov    ax, word ptr [esp + 0x10]
        mov    word ptr [ecx + 0x1e], ax
        mov    ax, word ptr [esp + 0xc]
        mov    word ptr [ecx + 0x20], ax
        pop    edi
        ret    
loc_1_1E6E:
        mov    dword ptr [0x3940], 0         ; -> obj1:0x3940
        mov    eax, dword ptr [0x3920]       ; -> obj1:0x3920
        and    eax, 0xfffffffb
        mov    dword ptr [0x3920], eax       ; -> obj1:0x3920
        push   0
        push   0x2ae7                        ; -> sub_1_2AE7
        call   sub_1_009C
        add    esp, 8
        ret    
sub_1_1E95:
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [0x39b0], 0         ; -> obj1:0x39B0
        cmp    dword ptr [0x39a4], 0         ; -> obj1:0x39A4
        jbe    loc_1_1F33
        call   sub_1_01CC
        mov    edi, eax
        call   sub_1_045B
        mov    esi, eax
        or     esi, esi
        jne    loc_1_1EC8
        mov    eax, 0x1f4
        jmp    loc_1_1F1C
loc_1_1EC8:
        mov    ebx, dword ptr [esi + 0x24]
        push   0
        push   1
        push   3
        push   esi
        call   sub_1_1CFD
        add    esp, 0x10
        mov    dword ptr [esi + 4], 0x46f    ; -> sub_1_046F
        mov    word ptr [esi + 0xa], 0x5505
        mov    eax, dword ptr [0x39a0]       ; -> obj1:0x39A0
        mov    dword ptr [ebx + 0x4e], eax
        dec    dword ptr [0x39a4]            ; -> obj1:0x39A4
        push   esi
        call   sub_1_013F
        add    esp, 4
        add    dword ptr [0x39b4], 0x3e8     ; -> obj1:0x39B4
        cmp    dword ptr [0x39b4], edi       ; -> obj1:0x39B4
        mov    eax, 1
        jbe    loc_1_1F1C
        mov    eax, dword ptr [0x39b4]       ; -> obj1:0x39B4
        sub    eax, edi
loc_1_1F1C:
        push   0
        push   0x1e95                        ; -> sub_1_1E95
        push   eax
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [0x39b0], eax       ; -> obj1:0x39B0
        jmp    loc_1_1F7C
loc_1_1F33:
        and    dword ptr [0x3920], 0xffffbfff ; -> obj1:0x3920
        test   byte ptr [0x3921], 1          ; -> obj1:0x3921
        je     loc_1_1F57
        push   0
        push   0x4c7                         ; -> loc_1_04C7
        call   sub_1_00BF
        add    esp, 8
        jmp    loc_1_1F7C
loc_1_1F57:
        and    dword ptr [0x3920], 0xffffdffd ; -> obj1:0x3920
        or     dword ptr [0x3920], 0x20      ; -> obj1:0x3920
        mov    dword ptr [0x393c], 0x1e      ; -> obj1:0x393C
        push   3
        call   sub_1_10B6
        add    esp, 4
loc_1_1F7C:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1F80:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        cmp    dword ptr [ebx + 0x14], 0x15
        je     loc_1_1FC6
        lea    eax, [ebx + 0x18]
        mov    ecx, 0x6a8                    ; -> obj1:0x06A8
loc_1_1F99:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    loc_1_1FB9
        or     dl, dl
        je     loc_1_1FB5
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    loc_1_1FB9
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    loc_1_1F99
loc_1_1FB5:
        sub    eax, eax
        jmp    loc_1_1FBE
loc_1_1FB9:
        sbb    eax, eax
        sbb    eax, -1
loc_1_1FBE:
        or     eax, eax
        jne    loc_1_264C
loc_1_1FC6:
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 1
        je     loc_1_2025
        cmp    eax, 2
        je     loc_1_22A7
        cmp    eax, 0x12
        je     loc_1_221F
        cmp    eax, 0x15
        je     loc_1_2272
loc_1_1FE9:
        mov    ebp, dword ptr [0x3980]       ; -> loc_1_3980
        cmp    ebp, 0x3980                   ; -> loc_1_3980
        je     loc_1_264C
loc_1_1FFB:
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x10
        je     loc_1_245A
        cmp    eax, 0x11
        je     loc_1_24FD
        cmp    eax, 0x15
        je     loc_1_2558
        jmp    loc_1_263C
loc_1_2025:
        mov    esi, dword ptr [esp + 0x18]
        mov    edi, 0x396c                   ; -> obj1:0x396C
        lea    esi, [esi + 0x12]
        mov    ecx, 2
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_2040
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
loc_1_2040:
        je     loc_1_264C
        test   byte ptr [0x3921], 0x40       ; -> obj1:0x3921
        je     loc_1_2065
        cmp    dword ptr [ebx + 0xc], 0
        je     loc_1_264C
        push   0x384
        push   0xc8
        jmp    loc_1_2094
loc_1_2065:
        call   sub_1_01CC
        mov    dword ptr [0x39a0], eax       ; -> obj1:0x39A0
        mov    dword ptr [0x39a8], 0x1e      ; -> obj1:0x39A8
        cmp    dword ptr [0x3924], 0x50000   ; -> obj1:0x3924
        mov    esi, 0x64
        jae    loc_1_209E
        push   0xbb8
        push   0x320
loc_1_2094:
        call   sub_1_06C0
        add    esp, 8
        mov    esi, eax
loc_1_209E:
        test   byte ptr [0x3921], 1          ; -> obj1:0x3921
        jne    loc_1_20AE
        sub    ecx, ecx
        jmp    loc_1_2143
loc_1_20AE:
        cmp    dword ptr [ebx + 0xc], 0
        jne    loc_1_20BE
        mov    ecx, 1
        jmp    loc_1_2143
loc_1_20BE:
        dec    dword ptr [0x39a8]            ; -> obj1:0x39A8
        jne    loc_1_20CA
        sub    ecx, ecx
        jmp    loc_1_2143
loc_1_20CA:
        mov    eax, dword ptr [ebx + 8]
        cmp    eax, 0x10006
        je     loc_1_20E3
        cmp    eax, 0x10006
        mov    ecx, 0
        setb   cl
        jmp    loc_1_2143
loc_1_20E3:
        mov    eax, dword ptr [ebx + 0xc]
        cmp    dword ptr [0x3924], eax       ; -> obj1:0x3924
        je     loc_1_20F6
        cmp    dword ptr [0x3924], eax       ; -> obj1:0x3924
        jmp    loc_1_213B
loc_1_20F6:
        mov    eax, dword ptr [ebx + 0x10]
        cmp    eax, dword ptr [0x39a0]       ; -> obj1:0x39A0
        je     loc_1_2111
        mov    ecx, 0
        cmp    eax, dword ptr [0x39a0]       ; -> obj1:0x39A0
        setb   cl
        jmp    loc_1_2143
loc_1_2111:
        sub    ecx, ecx
        sub    edi, edi
loc_1_2115:
        mov    dl, byte ptr [edi + 0x3970]   ; -> obj1:0x3970
        mov    eax, dword ptr [esp + 0x18]
        cmp    byte ptr [edi + eax + 0x16], dl
        jne    loc_1_212D
        inc    edi
        cmp    edi, 6
        jb     loc_1_2115
        jmp    loc_1_2143
loc_1_212D:
        mov    cl, byte ptr [edi + 0x3970]   ; -> obj1:0x3970
        mov    eax, dword ptr [esp + 0x18]
        cmp    byte ptr [edi + eax + 0x16], cl
loc_1_213B:
        mov    ecx, 0
        seta   cl
loc_1_2143:
        or     ecx, ecx
        jne    loc_1_2185
        call   sub_1_01CC
        mov    dword ptr [0x39ac], eax       ; -> obj1:0x39AC
        and    dword ptr [0x3920], 0xffffbfff ; -> obj1:0x3920
        cmp    dword ptr [0x39b0], 0         ; -> obj1:0x39B0
        je     loc_1_264C
        push   dword ptr [0x39b0]            ; -> obj1:0x39B0
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [0x39b0], 0         ; -> obj1:0x39B0
        jmp    loc_1_264C
loc_1_2185:
        test   byte ptr [0x3921], 0x40       ; -> obj1:0x3921
        jne    loc_1_21C1
        cmp    dword ptr [0x39ac], 0         ; -> obj1:0x39AC
        je     loc_1_21AD
        mov    eax, dword ptr [0x39a0]       ; -> obj1:0x39A0
        sub    eax, dword ptr [0x39ac]       ; -> obj1:0x39AC
        cmp    eax, 0x36b0
        jb     loc_1_264C
loc_1_21AD:
        mov    dword ptr [0x39b4], 0         ; -> obj1:0x39B4
        or     dword ptr [0x3920], 0x4000    ; -> obj1:0x3920
loc_1_21C1:
        mov    dword ptr [0x39a4], 4         ; -> obj1:0x39A4
        call   sub_1_01CC
        lea    edi, [esi + eax]
        cmp    dword ptr [0x39b4], 0         ; -> obj1:0x39B4
        je     loc_1_21E8
        cmp    edi, dword ptr [0x39b4]       ; -> obj1:0x39B4
        jae    loc_1_264C
loc_1_21E8:
        cmp    dword ptr [0x39b0], 0         ; -> obj1:0x39B0
        je     loc_1_21FF
        push   dword ptr [0x39b0]            ; -> obj1:0x39B0
        call   sub_1_008C
        add    esp, 4
loc_1_21FF:
        mov    dword ptr [0x39b4], edi       ; -> obj1:0x39B4
        push   0
        push   0x1e95                        ; -> sub_1_1E95
        push   esi
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [0x39b0], eax       ; -> obj1:0x39B0
        jmp    loc_1_264C
loc_1_221F:
        test   byte ptr [0x3920], 2          ; -> obj1:0x3920
        je     loc_1_1FE9
        and    dword ptr [0x3920], 0xfffffbfd ; -> obj1:0x3920
        push   0x1f40
        push   0x1f4
        call   sub_1_06C0
        add    esp, 8
        mov    esi, eax
        push   dword ptr [0x3934]            ; -> obj1:0x3934
        call   sub_1_008C
        add    esp, 4
        push   0
        push   0xf88                         ; -> loc_1_0F88
        push   esi
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [0x3934], eax       ; -> obj1:0x3934
        jmp    loc_1_1FE9
loc_1_2272:
        lea    eax, [ebx + 0x18]
        mov    ecx, 0x6a8                    ; -> obj1:0x06A8
loc_1_227A:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    loc_1_229A
        or     dl, dl
        je     loc_1_2296
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    loc_1_229A
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    loc_1_227A
loc_1_2296:
        sub    eax, eax
        jmp    loc_1_229F
loc_1_229A:
        sbb    eax, eax
        sbb    eax, -1
loc_1_229F:
        or     eax, eax
        jne    loc_1_1FE9
loc_1_22A7:
        mov    eax, dword ptr [ebx + 8]
        cmp    eax, 0x10003
        jb     loc_1_264C
        test   byte ptr [0x3921], 0x20       ; -> obj1:0x3921
        je     loc_1_22D3
        cmp    eax, 0x10006
        jb     loc_1_264C
        and    dword ptr [0x3920], 0xffffdbff ; -> obj1:0x3920
loc_1_22D3:
        mov    dword ptr [0x3928], 6         ; -> obj1:0x3928
        and    dword ptr [0x3920], 0xffffbfdf ; -> obj1:0x3920
        mov    dword ptr [0x393c], 0         ; -> obj1:0x393C
        cmp    dword ptr [0x39b0], 0         ; -> obj1:0x39B0
        je     loc_1_2312
        push   dword ptr [0x39b0]            ; -> obj1:0x39B0
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [0x39b0], 0         ; -> obj1:0x39B0
loc_1_2312:
        test   byte ptr [0x3920], 2          ; -> obj1:0x3920
        je     loc_1_2334
        mov    edi, dword ptr [esp + 0x18]
        mov    esi, 0x3944                   ; -> obj1:0x3944
        lea    edi, [edi + 0x12]
        mov    ecx, 3
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        je     loc_1_23E1
loc_1_2334:
        test   byte ptr [0x3921], 0x20       ; -> obj1:0x3921
        jne    loc_1_23E1
        mov    esi, dword ptr [esp + 0x18]
        add    esi, 0x12
        mov    edi, 0x3944                   ; -> obj1:0x3944
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    edi, 0x3a44                   ; -> obj1:0x3A44
        mov    esi, dword ptr [esp + 0x24]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        or     dword ptr [0x3920], 2         ; -> obj1:0x3920
        and    dword ptr [0x3920], 0xfffffbff ; -> obj1:0x3920
        test   byte ptr [0x3920], 0x80       ; -> obj1:0x3920
        jne    loc_1_2393
        cmp    dword ptr [0x3938], 0         ; -> obj1:0x3938
        jne    loc_1_2393
        push   0
        push   0x2654                        ; -> loc_1_2654
        call   sub_1_009C
        add    esp, 8
        mov    dword ptr [0x3938], eax       ; -> obj1:0x3938
loc_1_2393:
        test   byte ptr [0x3920], 8          ; -> obj1:0x3920
        je     loc_1_23E1
        test   byte ptr [0x3920], 4          ; -> obj1:0x3920
        je     loc_1_23B3
        push   dword ptr [0x3940]            ; -> obj1:0x3940
        call   sub_1_008C
        add    esp, 4
loc_1_23B3:
        push   0x1388
        push   0x3e8
        call   sub_1_06C0
        add    esp, 8
        push   0
        push   0x1e6e                        ; -> loc_1_1E6E
        push   eax
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [0x3940], eax       ; -> obj1:0x3940
        or     dword ptr [0x3920], 4         ; -> obj1:0x3920
loc_1_23E1:
        test   byte ptr [0x3920], 0x80       ; -> obj1:0x3920
        je     loc_1_264C
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_240F
        push   eax
        call   sub_1_008C
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        add    esp, 4
        mov    dword ptr [eax + 0x10], 0
loc_1_240F:
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        mov    byte ptr [eax + 0xa], 4
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        mov    dword ptr [eax + 0x1c], 3
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        mov    eax, dword ptr [eax + 0x14]
        or     eax, eax
        je     loc_1_2439
        push   eax
        call   sub_1_00D6
        add    esp, 4
loc_1_2439:
        push   dword ptr [0x3988]            ; -> obj1:0x3988
        push   0x496                         ; -> loc_1_0496
        call   sub_1_00BF
        mov    ecx, dword ptr [0x3988]       ; -> obj1:0x3988
        add    esp, 8
        mov    dword ptr [ecx + 0x14], eax
        jmp    loc_1_264C
loc_1_245A:
        mov    al, byte ptr [ebp + 9]
        cmp    al, 7
        jne    loc_1_24A6
        lea    eax, [ebx + 0x2a]
        lea    edx, [ebp + 0x3e]
loc_1_2467:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    loc_1_2487
        or     cl, cl
        je     loc_1_2483
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    loc_1_2487
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    loc_1_2467
loc_1_2483:
        sub    eax, eax
        jmp    loc_1_248C
loc_1_2487:
        sbb    eax, eax
        sbb    eax, -1
loc_1_248C:
        or     eax, eax
        jne    loc_1_263C
        push   0
        push   0
        push   ebp
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_263C
loc_1_24A6:
        cmp    al, 5
        jne    loc_1_263C
        lea    eax, [ebx + 0x2a]
        lea    edx, [ebp + 0x3e]
loc_1_24B4:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    loc_1_24D4
        or     cl, cl
        je     loc_1_24D0
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    loc_1_24D4
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    loc_1_24B4
loc_1_24D0:
        sub    eax, eax
        jmp    loc_1_24D9
loc_1_24D4:
        sbb    eax, eax
        sbb    eax, -1
loc_1_24D9:
        or     eax, eax
        jne    loc_1_263C
        lea    edi, [ebp + 0x4e]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    esi, [ebx + 0x5a]
        lea    edi, [ebp + 0x54]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    word ptr [ebp + 0x5e], 0x5505
        jmp    loc_1_25F2
loc_1_24FD:
        cmp    byte ptr [ebp + 9], 5
        jne    loc_1_253C
        mov    byte ptr [ebp + 0xa], 3
        mov    dword ptr [ebp + 0x1c], 2
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        je     loc_1_2525
        push   eax
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [ebp + 0x10], 0
loc_1_2525:
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_253C
        push   ebp
        push   0x496                         ; -> loc_1_0496
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [ebp + 0x14], eax
loc_1_253C:
        cmp    byte ptr [ebp + 9], 7
        jne    loc_1_263C
        push   0
        push   3
        push   ebp
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_263C
loc_1_2558:
        mov    al, byte ptr [ebp + 9]
        cmp    al, 7
        jne    loc_1_25A4
        lea    eax, [ebx + 0x18]
        lea    edx, [ebp + 0x3e]
loc_1_2565:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    loc_1_2585
        or     cl, cl
        je     loc_1_2581
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    loc_1_2585
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    loc_1_2565
loc_1_2581:
        sub    eax, eax
        jmp    loc_1_258A
loc_1_2585:
        sbb    eax, eax
        sbb    eax, -1
loc_1_258A:
        or     eax, eax
        jne    loc_1_263C
        push   0
        push   0
        push   ebp
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_263C
loc_1_25A4:
        cmp    al, 5
        jne    loc_1_263C
        lea    eax, [ebx + 0x18]
        lea    edx, [ebp + 0x3e]
loc_1_25B2:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    loc_1_25D2
        or     cl, cl
        je     loc_1_25CE
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    loc_1_25D2
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    loc_1_25B2
loc_1_25CE:
        sub    eax, eax
        jmp    loc_1_25D7
loc_1_25D2:
        sbb    eax, eax
        sbb    eax, -1
loc_1_25D7:
        or     eax, eax
        jne    loc_1_263C
        lea    edi, [ebp + 0x4e]
        mov    esi, dword ptr [esp + 0x24]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, dword ptr [esp + 0x18]
        add    esi, 0x12
        lea    edi, [ebp + 0x54]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_1_25F2:
        mov    byte ptr [ebp + 0xa], 5
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [ebp + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
        mov    dword ptr [ebp + 0x1c], 3
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        je     loc_1_2625
        push   eax
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [ebp + 0x10], 0
loc_1_2625:
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_263C
        push   ebp
        push   0x496                         ; -> loc_1_0496
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [ebp + 0x14], eax
loc_1_263C:
        mov    ebp, dword ptr [esp + 0x10]
        cmp    ebp, 0x3980                   ; -> loc_1_3980
        jne    loc_1_1FFB
loc_1_264C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_2654:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        push   0x3a44                        ; -> obj1:0x3A44
        push   0x3944                        ; -> obj1:0x3944
        call   sub_1_01A4
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_2692
        mov    eax, dword ptr [esp]
        imul   eax, eax, 0x37
        add    eax, 0x1f4
        mov    dword ptr [0x392c], eax       ; -> obj1:0x392C
        mov    eax, dword ptr [0x3920]       ; -> obj1:0x3920
        or     eax, 0x400
        mov    dword ptr [0x3920], eax       ; -> obj1:0x3920
loc_1_2692:
        mov    dword ptr [0x3938], 0         ; -> obj1:0x3938
        add    esp, 4
        ret    
sub_1_26A0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        test   byte ptr [0x3920], 0x80       ; -> obj1:0x3920
        je     loc_1_274D
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        mov    ebx, dword ptr [esp + 0x14]
        cmp    byte ptr [eax + 0xa], 1
        jne    loc_1_2751
        cmp    word ptr [ebx], 0x5006
        jne    loc_1_2751
        or     dword ptr [0x3920], 0x2000    ; -> obj1:0x3920
        lea    esi, [ebx + 0x32]
        mov    edi, 0x3944                   ; -> obj1:0x3944
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_2705
        push   eax
        call   sub_1_008C
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        add    esp, 4
        mov    dword ptr [eax + 0x10], 0
loc_1_2705:
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        mov    byte ptr [eax + 0xa], 4
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        mov    dword ptr [eax + 0x1c], 3
        mov    eax, dword ptr [0x3988]       ; -> obj1:0x3988
        mov    eax, dword ptr [eax + 0x14]
        or     eax, eax
        je     loc_1_272F
        push   eax
        call   sub_1_00D6
        add    esp, 4
loc_1_272F:
        push   dword ptr [0x3988]            ; -> obj1:0x3988
        push   0x496                         ; -> loc_1_0496
        call   sub_1_00BF
        mov    ecx, dword ptr [0x3988]       ; -> obj1:0x3988
        add    esp, 8
        mov    dword ptr [ecx + 0x14], eax
        jmp    loc_1_27BF
loc_1_274D:
        mov    ebx, dword ptr [esp + 0x14]
loc_1_2751:
        cmp    word ptr [ebx], 0x7b06
        jne    loc_1_27BF
        mov    ebp, dword ptr [0x3980]       ; -> loc_1_3980
        cmp    ebp, 0x3980                   ; -> loc_1_3980
        je     loc_1_27BF
loc_1_2766:
        cmp    byte ptr [ebp + 9], 6
        jne    loc_1_27B4
        lea    esi, [ebx + 0x32]
        lea    edi, [ebp + 0x54]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    word ptr [ebp + 0x5e], 0x5505
        mov    byte ptr [ebp + 0xa], 4
        mov    dword ptr [ebp + 0x1c], 3
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        je     loc_1_279D
        push   eax
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [ebp + 0x10], 0
loc_1_279D:
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_27B4
        push   ebp
        push   0x496                         ; -> loc_1_0496
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [ebp + 0x14], eax
loc_1_27B4:
        mov    ebp, dword ptr [ebp]
        cmp    ebp, 0x3980                   ; -> loc_1_3980
        jne    loc_1_2766
loc_1_27BF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_27D0:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x3990]       ; -> loc_1_3990
        cmp    eax, 0x3990                   ; -> loc_1_3990
        je     loc_1_2825
        mov    edx, dword ptr [esp + 0x10]
        mov    si, word ptr [esp + 0x14]
loc_1_27E8:
        cmp    word ptr [eax + 0xc], si
        jne    loc_1_281C
        mov    ecx, edx
        lea    edi, [eax + 0x14]
loc_1_27F3:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [edi]
        jne    loc_1_2813
        or     bl, bl
        je     loc_1_280F
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    loc_1_2813
        add    ecx, 2
        add    edi, 2
        or     bl, bl
        jne    loc_1_27F3
loc_1_280F:
        sub    ecx, ecx
        jmp    loc_1_2818
loc_1_2813:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_1_2818:
        or     ecx, ecx
        je     loc_1_2827
loc_1_281C:
        mov    eax, dword ptr [eax]
        cmp    eax, 0x3990                   ; -> loc_1_3990
        jne    loc_1_27E8
loc_1_2825:
        sub    eax, eax
loc_1_2827:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_282B:
        push   esi
        push   edi
        mov    ax, word ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0x18]
        mov    ecx, 0xffffffff
        mov    word ptr [edx + 0xc], ax
        mov    ax, word ptr [esp + 0x14]
        mov    word ptr [edx + 0xe], ax
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, esi
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [edx + 0x10], ecx
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 0x14]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [edx + 8], 1
        mov    eax, dword ptr [0x3990]       ; -> loc_1_3990
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], 0x3990   ; -> loc_1_3990
        mov    eax, dword ptr [0x3990]       ; -> loc_1_3990
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [0x3990], edx       ; -> loc_1_3990
        inc    dword ptr [0x398c]            ; -> obj1:0x398C
        test   byte ptr [0x3920], 8          ; -> obj1:0x3920
        jne    loc_1_28D6
        or     dword ptr [0x3920], 8         ; -> obj1:0x3920
        mov    eax, dword ptr [0x39e8]       ; -> loc_1_39E8
        mov    dword ptr [0x39e4], eax       ; -> obj1:0x39E4
        push   0
        push   0x2928                        ; -> loc_1_2928
        push   0xea60
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [0x3a3c], eax       ; -> obj1:0x3A3C
loc_1_28D6:
        mov    eax, dword ptr [0x39e8]       ; -> loc_1_39E8
        cmp    dword ptr [0x39e4], eax       ; -> obj1:0x39E4
        jbe    loc_1_28E8
        mov    dword ptr [0x39e4], eax       ; -> obj1:0x39E4
loc_1_28E8:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
sub_1_28ED:
        mov    edx, dword ptr [esp + 4]
        cmp    edx, dword ptr [0x39e0]       ; -> obj1:0x39E0
        jne    loc_1_2900
        mov    eax, dword ptr [edx]
        mov    dword ptr [0x39e0], eax       ; -> obj1:0x39E0
loc_1_2900:
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        push   edx
        call   sub_1_00FA
        add    esp, 4
        dec    dword ptr [0x398c]            ; -> obj1:0x398C
        jne    loc_1_2927
        and    dword ptr [0x3920], 0xfffffff7 ; -> obj1:0x3920
loc_1_2927:
        ret    
loc_1_2928:
        mov    dword ptr [0x3a3c], 0         ; -> obj1:0x3A3C
        mov    eax, dword ptr [0x393c]       ; -> obj1:0x393C
        dec    eax
        mov    dword ptr [0x393c], eax       ; -> obj1:0x393C
        mov    edx, dword ptr [0x3920]       ; -> obj1:0x3920
        jne    loc_1_2948
        and    edx, 0xffffffdf
loc_1_2948:
        mov    dword ptr [0x3920], edx       ; -> obj1:0x3920
        test   dl, 8
        je     loc_1_29F7
        mov    ecx, dword ptr [0x3928]       ; -> obj1:0x3928
        or     ecx, ecx
        je     loc_1_2962
        dec    ecx
loc_1_2962:
        mov    eax, dword ptr [0x39e4]       ; -> obj1:0x39E4
        dec    eax
        mov    dword ptr [0x39e4], eax       ; -> obj1:0x39E4
        jne    loc_1_29B3
        mov    dword ptr [0x3928], ecx       ; -> obj1:0x3928
        mov    dword ptr [0x3920], edx       ; -> obj1:0x3920
        mov    eax, dword ptr [0x39e8]       ; -> loc_1_39E8
        mov    dword ptr [0x39e4], eax       ; -> obj1:0x39E4
        cmp    dword ptr [0x39e0], 0         ; -> obj1:0x39E0
        jne    loc_1_29B3
        mov    eax, dword ptr [0x3990]       ; -> loc_1_3990
        mov    dword ptr [0x39e0], eax       ; -> obj1:0x39E0
        push   0
        push   0x2bcc                        ; -> loc_1_2BCC
        call   sub_1_009C
        add    esp, 8
        mov    ecx, dword ptr [0x3928]       ; -> obj1:0x3928
        mov    edx, dword ptr [0x3920]       ; -> obj1:0x3920
loc_1_29B3:
        mov    dword ptr [0x3928], ecx       ; -> obj1:0x3928
        mov    dword ptr [0x3920], edx       ; -> obj1:0x3920
        or     ecx, ecx
        jne    loc_1_29DE
        test   dl, 2
        je     loc_1_29DE
        test   dl, 0x40
        jne    loc_1_29DE
        and    edx, 0xfffffbfd
        mov    dword ptr [0x3920], edx       ; -> obj1:0x3920
        call   sub_1_0F51
loc_1_29DE:
        push   0
        push   0x2928                        ; -> loc_1_2928
        push   0xea60
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [0x3a3c], eax       ; -> obj1:0x3A3C
loc_1_29F7:
        ret    
        db  0C3h
loc_1_29F9:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 9], 0
        je     loc_1_2A20
        mov    ecx, dword ptr [esi + 0x10]
        push   ecx
        mov    ax, word ptr [ecx + 0xe]
        push   eax
        mov    ax, word ptr [ecx + 0xc]
        add    ecx, 0x14
        push   eax
        push   ecx
        call   sub_1_282B
        add    esp, 0x10
        jmp    loc_1_2A2B
loc_1_2A20:
        push   dword ptr [esi + 0x10]
        call   sub_1_00FA
        add    esp, 4
loc_1_2A2B:
        test   byte ptr [0x3920], 4          ; -> obj1:0x3920
        je     loc_1_2A81
        mov    edx, dword ptr [0x39ec]       ; -> loc_1_39EC
        cmp    edx, 0x39ec                   ; -> loc_1_39EC
        je     loc_1_2A81
loc_1_2A42:
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0x3990]       ; -> loc_1_3990
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], 0x3990   ; -> loc_1_3990
        mov    eax, dword ptr [0x3990]       ; -> loc_1_3990
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [0x3990], edx       ; -> loc_1_3990
        inc    dword ptr [0x398c]            ; -> obj1:0x398C
        mov    edx, dword ptr [0x39ec]       ; -> loc_1_39EC
        cmp    edx, 0x39ec                   ; -> loc_1_39EC
        jne    loc_1_2A42
loc_1_2A81:
        mov    eax, dword ptr [0x39ec]       ; -> loc_1_39EC
        cmp    eax, 0x39ec                   ; -> loc_1_39EC
        je     loc_1_2A99
        push   esi
        push   eax
        call   sub_1_2AAE
        add    esp, 8
        jmp    loc_1_2AAC
loc_1_2A99:
        and    dword ptr [0x3920], 0xffff7fff ; -> obj1:0x3920
        push   esi
        call   sub_1_00FA
        add    esp, 4
loc_1_2AAC:
        pop    esi
        ret    
sub_1_2AAE:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    dword ptr [eax + 0x10], edx
        add    edx, 0x14
        mov    dword ptr [eax + 4], 0x29f9   ; -> loc_1_29F9
        movzx  eax, word ptr [edx - 6]
        push   eax
        movzx  eax, word ptr [edx - 8]
        push   eax
        push   edx
        call   sub_1_09B1
        add    esp, 0x10
        ret    
sub_1_2AE7:
        push   esi
        test   dword ptr [0x3920], 0x8004    ; -> obj1:0x3920
        jne    loc_1_2B87
        cmp    dword ptr [0x398c], 0         ; -> obj1:0x398C
        je     loc_1_2B87
        or     dword ptr [0x3920], 0x8000    ; -> obj1:0x3920
        push   0x18
        call   sub_1_00E6
        add    esp, 4
        or     eax, eax
        jne    loc_1_2B29
        and    dword ptr [0x3920], 0xffff7fff ; -> obj1:0x3920
        jmp    loc_1_2B87
loc_1_2B29:
        mov    esi, dword ptr [0x3990]       ; -> loc_1_3990
        cmp    esi, 0x3990                   ; -> loc_1_3990
        je     loc_1_2B78
loc_1_2B37:
        mov    edx, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edx + 4], ecx
        mov    edx, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [0x39ec]       ; -> loc_1_39EC
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], 0x39ec   ; -> loc_1_39EC
        mov    ecx, dword ptr [0x39ec]       ; -> loc_1_39EC
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [0x39ec], esi       ; -> loc_1_39EC
        dec    dword ptr [0x398c]            ; -> obj1:0x398C
        mov    esi, dword ptr [0x3990]       ; -> loc_1_3990
        cmp    esi, 0x3990                   ; -> loc_1_3990
        jne    loc_1_2B37
loc_1_2B78:
        push   eax
        push   dword ptr [0x39ec]            ; -> loc_1_39EC
        call   sub_1_2AAE
        add    esp, 8
loc_1_2B87:
        pop    esi
        ret    
loc_1_2B89:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 0xb], 0
        je     loc_1_2BAC
        test   dword ptr [0x3920], 0x2002    ; -> obj1:0x3920
        je     loc_1_2BA7
        call   sub_1_2AE7
        jmp    loc_1_2BAC
loc_1_2BA7:
        call   sub_1_0F51
loc_1_2BAC:
        push   dword ptr [esi + 0x20]
        call   sub_1_046F
        add    esp, 4
        push   esi
        call   sub_1_00FA
        add    esp, 4
        mov    dword ptr [0x39e0], 0         ; -> obj1:0x39E0
        pop    esi
        ret    
loc_1_2BCC:
        sub    esp, 4
        push   esi
        test   byte ptr [0x3921], 4          ; -> obj1:0x3921
        jne    loc_1_2C0F
        lea    eax, [esp + 4]
        push   eax
        push   0x3a44                        ; -> obj1:0x3A44
        push   0x3944                        ; -> obj1:0x3944
        call   sub_1_01A4
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_2C40
        mov    eax, dword ptr [esp + 4]
        imul   eax, eax, 0x37
        add    eax, 0x1f4
        mov    dword ptr [0x392c], eax       ; -> obj1:0x392C
        or     dword ptr [0x3920], 0x400     ; -> obj1:0x3920
loc_1_2C0F:
        call   sub_1_045B
        mov    esi, eax
        or     esi, esi
        je     loc_1_2C5B
        push   0x3c
        call   sub_1_00E6
        add    esp, 4
        or     eax, eax
        je     loc_1_2C5B
        mov    dword ptr [eax + 0x24], 0x2b89 ; -> loc_1_2B89
        push   8
        push   dword ptr [esi + 0x24]
        push   esi
        push   eax
        call   sub_1_0817
        add    esp, 0x10
        jmp    loc_1_2C99
loc_1_2C40:
        and    dword ptr [0x3920], 0xffffdffd ; -> obj1:0x3920
        mov    dword ptr [0x39e0], 0         ; -> obj1:0x39E0
        call   sub_1_0F51
        jmp    loc_1_2C99
loc_1_2C5B:
        push   dword ptr [0x3a3c]            ; -> obj1:0x3A3C
        call   sub_1_008C
        add    esp, 4
        push   0
        push   0x2928                        ; -> loc_1_2928
        push   0x1f4
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [0x3a3c], eax       ; -> obj1:0x3A3C
        or     esi, esi
        je     loc_1_2C8F
        push   esi
        call   sub_1_046F
        add    esp, 4
loc_1_2C8F:
        mov    dword ptr [0x39e4], 1         ; -> obj1:0x39E4
loc_1_2C99:
        pop    esi
        add    esp, 4
        ret    
loc_1_2C9E:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [ebx + 0x20]
        mov    dword ptr [esp + 0x10], eax
        movzx  eax, byte ptr [ebx + 0xa]
        or     eax, eax
        je     loc_1_2CCB
        cmp    eax, 1
        je     loc_1_2CF2
        cmp    eax, 2
        je     loc_1_2E06
        jmp    loc_1_2DDB
loc_1_2CCB:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 4], 0
        mov    cx, word ptr [0x3998]         ; -> obj1:0x3998
        mov    word ptr [eax + 0xa], cx
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [ebx + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
loc_1_2CF2:
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, 0x3a44                   ; -> obj1:0x3A44
        mov    eax, dword ptr [eax + 0x24]
        mov    edi, dword ptr [esp + 0x10]
        lea    ebp, [eax + 0x1e]
        add    edi, 0x1c
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, 0x3944                   ; -> obj1:0x3944
        lea    edi, [eax + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    byte ptr [eax + 5], 4
        mov    al, byte ptr [ebx + 9]
        sub    al, 8
        cmp    al, 1
        sbb    al, al
        add    al, 0x13
        mov    byte ptr [ebp], al
        mov    byte ptr [ebp + 1], 0
        mov    ax, word ptr [ebx + 0x2c]
        mov    word ptr [ebp + 2], ax
        mov    esi, 0x28
        mov    word ptr [ebp + 4], 0x100
        mov    ax, word ptr [0x3a34]         ; -> obj1:0x3A34
        mov    word ptr [ebp + 6], ax
        mov    ah, byte ptr [0x3a38]         ; -> loc_1_3A38
        add    ebp, 0xa
        mov    al, byte ptr [0x3a39]         ; -> loc_1_3A39
        mov    word ptr [ebp - 2], ax
        movzx  edi, word ptr [0x3a38]        ; -> loc_1_3A38
        sub    edi, 0x28
        cmp    dword ptr [0x39e0], 0x3990    ; -> obj1:0x39E0
        je     loc_1_2DC8
loc_1_2D71:
        mov    eax, dword ptr [0x39e0]       ; -> obj1:0x39E0
        mov    ecx, dword ptr [eax + 0x10]
        mov    eax, dword ptr [0x6b8]        ; -> obj1:0x06B8
        lea    eax, [ecx + eax + 0x1a]
        cmp    eax, edi
        ja     loc_1_2DC8
        mov    eax, dword ptr [0x39e0]       ; -> obj1:0x39E0
        movzx  eax, word ptr [eax + 0xe]
        push   eax
        mov    eax, dword ptr [0x39e0]       ; -> obj1:0x39E0
        push   0x6a8                         ; -> obj1:0x06A8
        movzx  eax, word ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [0x39e0]       ; -> obj1:0x39E0
        add    eax, 0x14
        push   eax
        push   ebp
        call   sub_1_30C0
        add    esi, eax
        add    ebp, eax
        mov    eax, dword ptr [0x39e0]       ; -> obj1:0x39E0
        add    esp, 0x14
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x39e0], eax       ; -> obj1:0x39E0
        cmp    eax, 0x3990                   ; -> loc_1_3990
        jne    loc_1_2D71
loc_1_2DC8:
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0x28], si
        mov    byte ptr [ebx + 0xa], 2
        mov    dword ptr [ebx + 0x1c], 2
loc_1_2DDB:
        push   ebx
        push   0x487                         ; -> loc_1_0487
        push   dword ptr [0x392c]            ; -> obj1:0x392C
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [ebx + 0x10], eax
        push   dword ptr [esp + 0x10]
        call   sub_1_013F
        add    esp, 4
loc_1_2DFE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_2E06:
        mov    eax, dword ptr [ebx + 0x1c]
        or     eax, eax
        je     loc_1_2E13
        dec    eax
        mov    dword ptr [ebx + 0x1c], eax
        jmp    loc_1_2DDB
loc_1_2E13:
        cmp    word ptr [0x3a38], 0x240      ; -> loc_1_3A38
        jbe    loc_1_2E2C
        mov    word ptr [0x3a38], 0x240      ; -> loc_1_3A38
        jmp    loc_1_2CF2
loc_1_2E2C:
        and    dword ptr [0x3920], 0xffffdbfd ; -> obj1:0x3920
        push   0
        push   -1
        push   ebx
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_2DFE
sub_1_2E45:
        push   esi
        push   edi
        mov    eax, dword ptr [0x3980]       ; -> loc_1_3980
        cmp    eax, 0x3980                   ; -> loc_1_3980
        je     loc_1_2E75
        mov    esi, dword ptr [esp + 0xc]
loc_1_2E57:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [eax + 0xc], esi
        jne    loc_1_2E6B
        push   1
        push   0
        push   eax
        call   sub_1_0FBB
        add    esp, 0xc
loc_1_2E6B:
        mov    eax, edi
        cmp    edi, 0x3980                   ; -> loc_1_3980
        jne    loc_1_2E57
loc_1_2E75:
        pop    edi
        pop    esi
        ret    
sub_1_2E78:
        push   esi
        push   edi
        push   0x9a2
        call   sub_1_00E6
        add    esp, 4
        mov    dword ptr [0x3a40], eax       ; -> obj1:0x3A40
        or     eax, eax
        jne    loc_1_2E97
        mov    eax, 0xffffffff
        jmp    loc_1_2ED9
loc_1_2E97:
        mov    edx, dword ptr [0x3a40]       ; -> obj1:0x3A40
        mov    ecx, 2
        mov    esi, 1
        mov    edi, 0x440
loc_1_2EAC:
        mov    word ptr [edx + 0x22], si
        lea    eax, [edx + 0x2a]
        mov    dword ptr [edx + 0x24], eax
        mov    word ptr [edx + 0x28], di
        mov    eax, dword ptr [0x399c]       ; -> obj1:0x399C
        mov    dword ptr [edx], eax
        mov    dword ptr [0x399c], edx       ; -> obj1:0x399C
        mov    edx, dword ptr [edx + 0x24]
        add    edx, 0x440
        dec    ecx
        jne    loc_1_2EAC
        mov    dword ptr [0x3988], edx       ; -> obj1:0x3988
loc_1_2ED9:
        pop    edi
        pop    esi
        ret    
sub_1_2EDC:
        push   esi
        push   edi
        push   dword ptr [0x3a3c]            ; -> obj1:0x3A3C
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [0x3a3c], 0         ; -> obj1:0x3A3C
        test   byte ptr [0x3920], 8          ; -> obj1:0x3920
        je     loc_1_2F72
        test   dword ptr [0x3920], 0x2002    ; -> obj1:0x3920
        je     loc_1_2F72
        test   byte ptr [0x3921], 2          ; -> obj1:0x3921
        jne    loc_1_2F72
        call   sub_1_045B
        mov    edi, eax
        or     edi, edi
        je     loc_1_2F72
        push   0x3c
        call   sub_1_00E6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_2F69
        mov    dword ptr [esi + 0x24], 0
        mov    eax, dword ptr [0x3990]       ; -> loc_1_3990
        mov    dword ptr [0x39e0], eax       ; -> obj1:0x39E0
        push   9
        push   dword ptr [edi + 0x24]
        push   edi
        push   esi
        call   sub_1_0817
        add    esp, 0x10
        cmp    byte ptr [esi + 8], 0xff
        jne    loc_1_2F60
loc_1_2F55:
        call   sub_1_010E
        cmp    byte ptr [esi + 8], 0xff
        je     loc_1_2F55
loc_1_2F60:
        push   esi
        call   sub_1_00FA
        add    esp, 4
loc_1_2F69:
        push   edi
        call   sub_1_046F
        add    esp, 4
loc_1_2F72:
        mov    dword ptr [0x399c], 0         ; -> obj1:0x399C
        cmp    dword ptr [0x3a40], 0         ; -> obj1:0x3A40
        je     loc_1_2F93
        push   dword ptr [0x3a40]            ; -> obj1:0x3A40
        call   sub_1_00FA
        add    esp, 4
loc_1_2F93:
        mov    dword ptr [0x3a40], 0         ; -> obj1:0x3A40
        pop    edi
        pop    esi
        ret    
sub_1_2FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x18]
        lea    eax, [esi + 1]
        mov    dword ptr [ebp - 0x24], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x28], eax
        push   dword ptr [ebp - 0x30]
        push   dword ptr [ebp - 0x2c]
        push   dword ptr [ebp - 0x28]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        jne    loc_1_304A
        lea    eax, [ebp - 0x24]
        mov    dword ptr [ebp - 0x20], eax
        mov    ebx, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 0x1c], ebx
        lea    eax, [ebp - 0x34]
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 0x14], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        mov    edi, eax
        or     edi, edi
        jne    loc_1_3032
        mov    byte ptr [ebx + esi], 0
loc_1_3032:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_1_304A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3053:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    edi, 0x6a8                    ; -> obj1:0x06A8
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        push   0xf
        push   0x6a8                         ; -> obj1:0x06A8
        push   0x39f4                        ; -> loc_1_39F4
        push   0x3a00                        ; -> obj1:0x3A00
        push   0x80000002
        call   sub_1_2FA0
        add    esp, 0x14
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_30B4
        mov    edi, 0x6a8                    ; -> obj1:0x06A8
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [esp + 0xc]
        lea    esi, [ecx - 1]
        push   eax
        push   esi
        push   0x6a8                         ; -> obj1:0x06A8
        call   sub_1_01DF
        add    esp, 0xc
        mov    dword ptr [0x6b8], esi        ; -> obj1:0x06B8
loc_1_30B4:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  000h, 000h, 000h
sub_1_30C0:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        mov    ebx, edx
        mov    esi, dword ptr [esp + 0x14]
        or     esi, esi
        je     loc_1_3100
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    byte ptr [edx], cl
        mov    byte ptr [edx + 1], 1
        movzx  eax, byte ptr [edx]
        lea    edi, [edx + 2]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  eax, byte ptr [edx]
        lea    ebx, [edx + eax + 2]
loc_1_3100:
        mov    ax, word ptr [esp + 0x18]
        or     ax, ax
        je     loc_1_311D
        mov    byte ptr [ebx], 2
        mov    byte ptr [ebx + 1], 2
        mov    word ptr [ebx + 2], ax
        movzx  eax, byte ptr [ebx]
        add    eax, 2
        add    ebx, eax
loc_1_311D:
        mov    esi, dword ptr [esp + 0x1c]
        or     esi, esi
        je     loc_1_3155
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    byte ptr [ebx], cl
        mov    byte ptr [ebx + 1], 3
        movzx  eax, byte ptr [ebx]
        lea    edi, [ebx + 2]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  eax, byte ptr [ebx]
        add    eax, 2
        add    ebx, eax
loc_1_3155:
        mov    ax, word ptr [esp + 0x20]
        or     ax, ax
        je     loc_1_318E
        mov    byte ptr [ebx], 0x10
        mov    byte ptr [ebx + 1], 4
        mov    byte ptr [ebx + 2], 1
        mov    byte ptr [ebx + 3], 0x10
        mov    esi, 0x396c                   ; -> obj1:0x396C
        mov    byte ptr [ebx + 4], 6
        mov    byte ptr [ebx + 5], 0xc
        lea    edi, [ebx + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    word ptr [ebx + 0x10], ax
        movzx  eax, byte ptr [ebx]
        add    eax, 2
        add    ebx, eax
loc_1_318E:
        mov    eax, ebx
        sub    eax, edx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_3196:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x20]
        mov    esi, 0x3a44                   ; -> obj1:0x3A44
        mov    edi, dword ptr [ebx + 0x20]
        mov    ecx, dword ptr [edi + 0x24]
        add    edi, 0x1c
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, 0x3944                   ; -> obj1:0x3944
        lea    edi, [ecx + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    byte ptr [ecx + 5], 4
        mov    al, byte ptr [esp + 0x10]
        mov    byte ptr [ecx + 0x1e], al
        mov    byte ptr [ecx + 0x1f], 0
        mov    word ptr [ecx + 0x22], 0x100
        mov    ax, word ptr [ebx + 0x2c]
        push   dword ptr [esp + 0x1c]
        mov    word ptr [ecx + 0x20], ax
        mov    ax, word ptr [0x3a34]         ; -> obj1:0x3A34
        mov    word ptr [ecx + 0x24], ax
        add    ecx, 0x28
        mov    ah, byte ptr [0x3a38]         ; -> loc_1_3A38
        mov    al, byte ptr [0x3a39]         ; -> loc_1_3A39
        push   0x6a8                         ; -> obj1:0x06A8
        mov    word ptr [ecx - 2], ax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        push   ecx
        call   sub_1_30C0
        add    esp, 0x14
        mov    ecx, dword ptr [ebx + 0x20]
        add    ax, 0x28
        mov    dword ptr [ecx + 4], 0
        mov    edx, dword ptr [ebx + 0x20]
        mov    cx, word ptr [0x3998]         ; -> obj1:0x3998
        mov    word ptr [edx + 0xa], cx
        mov    ecx, dword ptr [ebx + 0x20]
        pop    edi
        mov    word ptr [ecx + 0x28], ax
        pop    esi
        pop    ebx
        ret    
sub_1_3233:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [esp + 0x2c]
        cmp    eax, 1
        mov    edi, dword ptr [ebx + 0x20]
        mov    ebp, dword ptr [edi + 0x24]
        jne    loc_1_325A
        lea    edx, [ebp + 0x1e]
        mov    dword ptr [esp + 0x10], 0x2e
        jmp    loc_1_3265
loc_1_325A:
        lea    edx, [ebp + 0x3e]
        mov    dword ptr [esp + 0x10], 0x4e
loc_1_3265:
        lea    ecx, [edx + 0xa]
        cmp    dword ptr [esp + 0x28], 0
        jne    loc_1_3281
        mov    esi, 0x3a44                   ; -> obj1:0x3A44
        add    edi, 0x1c
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, 0x3944                   ; -> obj1:0x3944
        jmp    loc_1_3292
loc_1_3281:
        add    edi, 0x1c
        mov    esi, dword ptr [esp + 0x28]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, dword ptr [esp + 0x28]
        add    esi, 6
loc_1_3292:
        lea    edi, [ebp + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        dec    eax
        cmp    eax, 1
        sbb    al, al
        and    al, 0xf0
        add    al, 0x14
        mov    byte ptr [ebp + 5], al
        mov    byte ptr [edx], 0x14
        mov    byte ptr [edx + 1], 0
        mov    ax, word ptr [ebx + 0x2c]
        mov    word ptr [edx + 2], ax
        mov    word ptr [edx + 4], 0x100
        mov    ax, word ptr [0x3a34]         ; -> obj1:0x3A34
        mov    word ptr [edx + 6], ax
        mov    esi, dword ptr [ebx + 0x38]
        or     esi, esi
        je     loc_1_32ED
        lea    eax, [esi + 0x28]
        movzx  edi, word ptr [0x3a38]        ; -> loc_1_3A38
        cmp    edi, eax
        jbe    loc_1_32ED
        add    si, 0x28
        shl    si, 8
        shr    eax, 8
        or     si, ax
        mov    word ptr [edx + 8], si
        jmp    loc_1_32FC
loc_1_32ED:
        mov    ah, byte ptr [0x3a38]         ; -> loc_1_3A38
        mov    al, byte ptr [0x3a39]         ; -> loc_1_3A39
        mov    word ptr [edx + 8], ax
loc_1_32FC:
        mov    edi, dword ptr [esp + 0x18]
        or     edi, edi
        mov    esi, dword ptr [esp + 0x20]
        jne    loc_1_331B
        or     esi, esi
        jne    loc_1_331B
        mov    word ptr [ecx], 0x2000
        mov    byte ptr [edx], 0x15
        mov    ax, word ptr [esp + 0x1c]
        jmp    loc_1_3338
loc_1_331B:
        or     edi, edi
        mov    word ptr [ecx], 0x3000
        jne    loc_1_3329
        mov    word ptr [ecx], 0x8000
loc_1_3329:
        mov    ax, word ptr [esp + 0x1c]
        or     ax, ax
        jne    loc_1_3338
        or     word ptr [ecx], 0x4000
loc_1_3338:
        mov    edx, dword ptr [ebx + 0x30]
        mov    dword ptr [ecx + 2], edx
        push   0
        push   esi
        add    ecx, 6
        push   eax
        push   edi
        push   ecx
        call   sub_1_30C0
        add    esp, 0x14
        mov    ecx, dword ptr [ebx + 0x20]
        mov    dword ptr [ecx + 4], 0
        mov    edx, dword ptr [ebx + 0x20]
        mov    cx, word ptr [0x3998]         ; -> obj1:0x3998
        mov    word ptr [edx + 0xa], cx
        mov    ecx, dword ptr [esp + 0x10]
        add    ecx, eax
        mov    eax, dword ptr [ebx + 0x20]
        pop    ebp
        mov    word ptr [eax + 0x28], cx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_337C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        movzx  eax, byte ptr [ebx + 0x24]
        or     eax, eax
        je     loc_1_33B9
        cmp    eax, 1
        je     loc_1_3419
        cmp    eax, 7
        je     loc_1_3419
        cmp    eax, 9
        je     loc_1_346B
        push   0
        mov    ebp, dword ptr [esp + 0x20]
        push   1
        push   ebp
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_3490
loc_1_33B9:
        cmp    word ptr [ebx + 0x2c], 0
        jne    loc_1_33D3
        push   0
        mov    ebp, dword ptr [esp + 0x20]
        push   -1
        push   ebp
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_33D7
loc_1_33D3:
        mov    ebp, dword ptr [esp + 0x1c]
loc_1_33D7:
        lea    edx, [ebp + 0x3c]
        movzx  eax, byte ptr [ebx + 0x2e]
        lea    esi, [ebx + 0x30]
        add    ebx, 0x2e
        lea    edi, [edx + 0x3e]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  eax, byte ptr [ebx]
        mov    byte ptr [edx + eax + 0x3e], 0
        movzx  eax, byte ptr [ebx]
        lea    esi, [ebx + eax + 8]
        lea    edi, [edx + 0x32]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        push   0
        push   0
        push   ebp
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_3490
loc_1_3419:
        test   byte ptr [0x3921], 0x20       ; -> obj1:0x3921
        mov    ebp, dword ptr [esp + 0x1c]
        jne    loc_1_345C
        cmp    byte ptr [ebp + 0xa], 1
        jne    loc_1_345C
        mov    byte ptr [ebp + 0xa], 2
        mov    dword ptr [ebp + 0x1c], 3
        push   dword ptr [ebp + 0x10]
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [ebp + 0x10], 0
        push   ebp
        push   0x496                         ; -> loc_1_0496
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [ebp + 0x14], eax
        jmp    loc_1_3490
loc_1_345C:
        push   0
        push   1
        push   ebp
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_3490
loc_1_346B:
        mov    ebp, dword ptr [esp + 0x1c]
        push   dword ptr [ebp + 0x10]
        call   sub_1_008C
        add    esp, 4
        push   ebp
        push   0x487                         ; -> loc_1_0487
        movzx  eax, word ptr [ebx + 0x26]
        push   eax
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [ebp + 0x10], eax
loc_1_3490:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_3495:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    al, byte ptr [esi + 0x1e]
        cmp    al, 0x94
        jne    loc_1_358E
        cmp    byte ptr [esi + 0x24], 0
        je     loc_1_34FE
        mov    edi, dword ptr [esp + 0x14]
        mov    byte ptr [edi + 0xa], 3
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [edi + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
        mov    dword ptr [edi + 0x1c], 3
        push   dword ptr [edi + 0x10]
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [edi + 0x10], 0
        cmp    dword ptr [edi + 0x14], 0
        jne    loc_1_35CF
        push   edi
        push   0x496                         ; -> loc_1_0496
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [edi + 0x14], eax
        jmp    loc_1_35CF
loc_1_34FE:
        movzx  eax, byte ptr [esi + 0x2e]
        add    esi, 0x2e
        lea    esi, [eax + esi + 2]
        mov    eax, 0x3970                   ; -> obj1:0x3970
        lea    edx, [esi + 0xa]
        mov    ecx, dword ptr [eax]
        cmp    ecx, dword ptr [edx]
        jne    loc_1_357C
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    loc_1_357C
        mov    eax, dword ptr [0x396c]       ; -> obj1:0x396C
        cmp    eax, dword ptr [esi + 6]
        je     loc_1_3531
        cmp    dword ptr [esi + 6], 0
        jne    loc_1_357C
loc_1_3531:
        mov    edi, dword ptr [esp + 0x14]
        mov    byte ptr [edi + 0xa], 3
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [edi + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
        mov    dword ptr [edi + 0x1c], 3
        push   dword ptr [edi + 0x10]
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [edi + 0x10], 0
        cmp    dword ptr [edi + 0x14], 0
        jne    loc_1_35CF
        push   edi
        push   0x496                         ; -> loc_1_0496
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [edi + 0x14], eax
        jmp    loc_1_35CF
loc_1_357C:
        push   0
        push   2
        push   dword ptr [esp + 0x1c]
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_35CF
loc_1_358E:
        cmp    al, 0x90
        jne    loc_1_35CF
        push   0
        cmp    byte ptr [esi + 0x24], 1
        cmc    
        sbb    eax, eax
        and    eax, 2
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_0FBB
        add    esp, 0xc
        mov    ax, word ptr [esi + 0x26]
        mov    ch, al
        mov    cl, ah
        movzx  eax, cx
        cmp    eax, 5
        jbe    loc_1_35C5
        sub    eax, 5
        mov    dword ptr [0x39e8], eax       ; -> loc_1_39E8
        jmp    loc_1_35CF
loc_1_35C5:
        mov    dword ptr [0x39e8], 1         ; -> loc_1_39E8
loc_1_35CF:
        pop    edi
        pop    esi
        ret    
sub_1_35D2:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x28]
        cmp    byte ptr [ecx + 0x1e], 0x94
        jne    loc_1_36DD
        cmp    byte ptr [ecx + 0x24], 0
        jne    loc_1_36CD
        mov    eax, dword ptr [esp + 0x30]
        lea    edx, [ecx + 0x2e]
        movzx  eax, word ptr [eax + 0x3c]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    bx, word ptr [ecx + 0x2c]
        mov    esi, dword ptr [eax + 0x34]
        mov    ah, bl
        mov    dword ptr [esp + 0x14], esi
        mov    al, bh
        cmp    dword ptr [esp + 0x18], 0
        mov    word ptr [esi + 4], ax
        mov    eax, dword ptr [ecx + 0x28]
        mov    dword ptr [esi], eax
        je     loc_1_362E
        mov    ebp, esi
        add    ebp, 6
        mov    ebx, dword ptr [esp + 0x1c]
        jmp    loc_1_3639
loc_1_362E:
        mov    ebx, dword ptr [esp + 0x14]
        add    ebx, 6
        mov    ebp, dword ptr [esp + 0x20]
loc_1_3639:
        mov    esi, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x10], 0
        cmp    word ptr [esi + 4], 0
        jbe    loc_1_36C7
loc_1_364C:
        cmp    dword ptr [esp + 0x18], 0
        mov    al, byte ptr [edx]
        jne    loc_1_3692
        mov    byte ptr [ebx + 2], al
        movzx  eax, byte ptr [edx]
        lea    esi, [edx + 2]
        lea    edi, [ebx + 3]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  eax, byte ptr [edx]
        add    eax, 2
        add    edx, eax
        mov    ax, word ptr [edx + 2]
        mov    word ptr [ebx], ax
        movzx  eax, byte ptr [edx]
        add    eax, 2
        add    edx, eax
        movzx  eax, byte ptr [ebx + 2]
        inc    eax
        imul   eax, eax, 3
        add    ebx, eax
        jmp    loc_1_36B5
loc_1_3692:
        mov    byte ptr [ebp], al
        inc    ebp
        movzx  eax, byte ptr [edx]
        lea    esi, [edx + 2]
        mov    edi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  eax, byte ptr [edx]
        add    ebp, eax
        lea    edx, [eax + edx + 2]
loc_1_36B5:
        mov    esi, dword ptr [esp + 0x14]
        inc    dword ptr [esp + 0x10]
        movzx  eax, word ptr [esi + 4]
        cmp    eax, dword ptr [esp + 0x10]
        ja     loc_1_364C
loc_1_36C7:
        push   0
        push   0
        jmp    loc_1_36D1
loc_1_36CD:
        push   0
        push   3
loc_1_36D1:
        push   dword ptr [esp + 0x38]
        call   sub_1_0FBB
        add    esp, 0xc
loc_1_36DD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
sub_1_36E5:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        cmp    byte ptr [esi + 0x1e], 0x95
        jne    loc_1_3771
        cmp    byte ptr [esi + 0x24], 0
        jne    loc_1_3761
        mov    ecx, dword ptr [esp + 0x20]
        lea    eax, [esi + 0x2e]
        mov    ebp, dword ptr [ecx + 0x34]
        mov    cx, word ptr [esi + 0x2c]
        mov    dx, cx
        shl    dl, 8
        or     dl, ch
        mov    byte ptr [ebp + 4], dl
        mov    ecx, dword ptr [esi + 0x28]
        mov    dword ptr [ebp], ecx
        lea    edx, [ebp + 5]
        sub    ebx, ebx
        cmp    byte ptr [ebp + 4], bl
        jbe    loc_1_375B
loc_1_3725:
        mov    cl, byte ptr [eax]
        mov    byte ptr [edx], cl
        movzx  ecx, byte ptr [eax]
        mov    dword ptr [esp + 0x10], ecx
        lea    esi, [eax + 2]
        lea    edi, [edx + 1]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [esp + 0x10]
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  ecx, byte ptr [eax]
        add    ecx, 2
        add    eax, ecx
        movzx  ecx, byte ptr [edx]
        inc    ecx
        add    edx, ecx
        inc    ebx
        movzx  ecx, byte ptr [ebp + 4]
        cmp    ecx, ebx
        ja     loc_1_3725
loc_1_375B:
        push   0
        push   0
        jmp    loc_1_3765
loc_1_3761:
        push   0
        push   3
loc_1_3765:
        push   dword ptr [esp + 0x28]
        call   sub_1_0FBB
        add    esp, 0xc
loc_1_3771:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_3779:
        push   esi
        mov    eax, dword ptr [esp + 8]
        cmp    byte ptr [eax + 6], 0
        jne    loc_1_37D5
        mov    esi, dword ptr [esp + 0xc]
        push   dword ptr [esi + 0x10]
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [esi + 0x10], 0
        cmp    dword ptr [0x39e0], 0x3990    ; -> obj1:0x39E0
        jne    loc_1_37AD
        push   0
        push   0
        push   esi
        jmp    loc_1_37DD
loc_1_37AD:
        mov    byte ptr [esi + 0xa], 1
        mov    ax, word ptr [0x3a30]         ; -> obj1:0x3A30
        mov    word ptr [esi + 0x2c], ax
        inc    word ptr [0x3a30]             ; -> obj1:0x3A30
        push   esi
        push   0x496                         ; -> loc_1_0496
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [esi + 0x14], eax
        jmp    loc_1_37E5
loc_1_37D5:
        push   0
        push   -1
        push   dword ptr [esp + 0x14]
loc_1_37DD:
        call   sub_1_0FBB
        add    esp, 0xc
loc_1_37E5:
        pop    esi
        ret    
sub_1_37E7:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        cmp    word ptr [edi + 4], 0x100
        jne    loc_1_38F2
        test   byte ptr [edi], 0x80
        je     loc_1_38F2
        mov    esi, dword ptr [0x3980]       ; -> loc_1_3980
        cmp    esi, 0x3980                   ; -> loc_1_3980
        je     loc_1_38F2
        mov    ax, word ptr [edi + 2]
loc_1_3818:
        cmp    word ptr [esi + 0x2c], ax
        je     loc_1_382D
        mov    esi, dword ptr [esi]
        cmp    esi, 0x3980                   ; -> loc_1_3980
        jne    loc_1_3818
        jmp    loc_1_38F2
loc_1_382D:
        movzx  eax, byte ptr [esi + 9]
        sub    eax, 2
        cmp    eax, 7
        ja     loc_1_38F2
        jmp    dword ptr [eax*4 + 0x38f5]    ; -> obj1:0x38F5
loc_1_3844:
        push   esi
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x14]
        call   sub_1_337C
        add    esp, 0xc
        jmp    loc_1_38F2
loc_1_385A:
        cmp    byte ptr [edi + 6], 9
        jne    loc_1_3888
        push   dword ptr [esi + 0x10]
        call   sub_1_008C
        add    esp, 4
        mov    ax, word ptr [edi + 8]
        push   esi
        mov    ch, al
        mov    cl, ah
        push   0x487                         ; -> loc_1_0487
        movzx  eax, cx
        push   eax
        call   sub_1_0074
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
loc_1_3888:
        push   esi
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x14]
        call   sub_1_3495
        add    esp, 0xc
        jmp    loc_1_38F2
loc_1_389B:
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_38F2
loc_1_38AA:
        push   esi
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x14]
        call   sub_1_35D2
        add    esp, 0xc
        jmp    loc_1_38F2
loc_1_38BD:
        push   esi
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x14]
        call   sub_1_36E5
        add    esp, 0xc
        jmp    loc_1_38F2
loc_1_38D0:
        push   0
        cmp    byte ptr [edi + 6], 1
        cmc    
        sbb    eax, eax
        and    eax, 3
        push   eax
        push   esi
        call   sub_1_0FBB
        add    esp, 0xc
        jmp    loc_1_38F2
loc_1_38E8:
        push   esi
        push   edi
        call   sub_1_3779
        add    esp, 8
loc_1_38F2:
        pop    edi
        pop    esi
        ret    
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_3844
        dd  loc_1_385A
        dd  loc_1_389B
        dd  loc_1_38AA
        dd  loc_1_38BD
        dd  loc_1_38D0
        dd  loc_1_38E8
        dd  loc_1_38E8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 055h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 005h, 055h, 000h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 005h, 055h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 005h, 055h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_3980:
        cmp    byte ptr [ecx], 0
        add    byte ptr [eax + 0x39], al     ; -> loc_1_3980
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_3990:
        nop    
        cmp    dword ptr [eax], eax
        add    byte ptr [eax + 0x39], dl     ; -> loc_1_3990
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
        add    bl, al                        ; -> loc_1_1AC3
        sbb    al, byte ptr [eax]
        add    byte ptr [ebp + 0x69000011], ch ; -> loc_1_11AD
        adc    eax, dword ptr [eax]
        add    byte ptr [esi], ch            ; -> loc_1_1A2E
        sbb    al, byte ptr [eax]
        add    byte ptr [edx + 0x15], dh     ; -> loc_1_1572
        add    byte ptr [eax], al
        dec    eax
        sbb    byte ptr [eax], al
        add    byte ptr [edx + 0x15], dh     ; -> loc_1_1572
        add    byte ptr [eax], al
        sahf   
        sub    al, 0
        add    byte ptr [esi + 0x2c], bl     ; -> loc_1_2C9E
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_39E8:
        and    ch, bh
        add    byte ptr [eax], al
loc_1_39EC:
        in     al, dx
        cmp    dword ptr [eax], eax
        add    ah, ch                        ; -> loc_1_39EC
        cmp    dword ptr [eax], eax
        db  000h
loc_1_39F4:
        push   edi
        outsd  dx, dword ptr [esi]
        jb     0x3a63
        jb     0x3a6a
        jne    0x3a6d
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x79], dl
        jae    0x3a78
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x3a7c
        jb     0x3a71
        outsb  dx, byte ptr [esi]
        je     0x3a52
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x3a85
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x3a75
        push   ebx
        jb     0x3a93
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
        pop    esp
        push   esi
        dec    esi
        inc    ebp
        push   esp
        push   ebx
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_3A38:
        inc    eax
loc_1_3A39:
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x70  R-X
loc_2_0000:
        VxDCall  Dev_0487.Service_0000h          ; cd2000008704
        jb     loc_2_0036
        xor    edx, edx
        call   sub_1_3053
        or     eax, eax
        jne    loc_2_002D
loc_2_0013:
        VxDCall  Dev_0487.Service_0002h          ; cd2002008704
        or     eax, eax
        je     loc_2_0027
        or     dword ptr [0x3920], 0x1000    ; -> obj1:0x3920
loc_2_0027:
        call   sub_1_2E78
        ret    
loc_2_002D:
        or     dword ptr [0x3920], 0x10      ; -> obj1:0x3920
        jmp    loc_2_0013
loc_2_0036:
        ret    
        db  000h, 04Eh, 065h, 074h, 077h, 06Fh, 072h, 06Bh, 000h, 057h, 06Fh, 072h, 06Bh, 067h, 072h, 06Fh
        db  075h, 070h, 000h, 044h, 069h, 072h, 065h, 063h, 074h, 065h, 064h, 042h, 072h, 06Fh, 061h, 064h
        db  063h, 061h, 073h, 074h, 000h, 04Dh, 041h, 053h, 054h, 045h, 052h, 05Fh, 043h, 052h, 045h, 044h
        db  045h, 04Eh, 054h, 049h, 041h, 04Ch, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 0B8h, 001h
        db  080h, 033h, 0DBh, 033h, 0F6h, 0C3h
VxD__LTEXT ENDS
