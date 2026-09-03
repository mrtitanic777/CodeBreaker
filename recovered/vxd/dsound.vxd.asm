; DSOUND -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 1323 instructions recovered by recursive descent

Declare_Virtual_Device DSOUND, 4, 2, DSOUND_Control, 0357Eh, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h
;   service table: obj1:0x58, 3 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1ec  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 046h, 0D9h, 0D4h, 047h, 0E8h, 062h, 0CFh, 011h
        db  093h, 0BCh, 044h, 045h, 053h, 054h, 000h, 000h, 083h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h
        db  0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 085h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h
        db  0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 0C0h, 033h, 042h, 08Ch, 0CCh, 0B4h, 0CEh, 011h
        db  092h, 094h, 044h, 045h, 053h, 054h, 000h, 000h, 0C1h, 033h, 042h, 08Ch, 0CCh, 0B4h, 0CEh, 011h
        db  092h, 094h, 044h, 045h, 053h, 054h, 000h, 000h
        dd  DSOUND_Service_00        ; fixup
        dd  DSOUND_Service_01        ; fixup
        dd  DSOUND_Service_02        ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0357Eh                      ; DDB_Req_Device_Number
        db  4, 2                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'DSOUND  '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  DSOUND_Control           ; DDB_Control_Proc
        dd  DSOUND_PM_API            ; DDB_V86_API_Proc
        dd  DSOUND_PM_API            ; DDB_PM_API_Proc
        dd  obj1:0x0058              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        dd  loc_1_0190               ; fixup
        dd  loc_3_0000               ; fixup
        dd  loc_3_0045               ; fixup
        dd  loc_3_00AE               ; fixup
        dd  loc_3_00DF               ; fixup
        dd  loc_3_007D               ; fixup
        dd  loc_3_010A               ; fixup
        dd  loc_3_0135               ; fixup
        dd  loc_3_015D               ; fixup
        dd  loc_3_0188               ; fixup
        dd  loc_3_01F1               ; fixup
        dd  loc_3_0222               ; fixup
        dd  loc_3_0248               ; fixup
        dd  loc_3_02BC               ; fixup
        dd  loc_3_02FB               ; fixup
        dd  loc_3_0324               ; fixup
        dd  loc_3_034D               ; fixup
        dd  loc_3_0376               ; fixup
        dd  loc_3_039F               ; fixup
        dd  loc_3_03CD               ; fixup
        dd  loc_3_0403               ; fixup
        dd  loc_3_0429               ; fixup
        dd  loc_3_0454               ; fixup
        dd  loc_3_0496               ; fixup
        dd  loc_3_04FB               ; fixup
        dd  loc_3_05E6               ; fixup
        dd  loc_3_0591               ; fixup
        dd  loc_3_06CC               ; fixup
        dd  loc_3_06FC               ; fixup
        dd  loc_3_0750               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
DSOUND_Control:
        cmp    eax, 0x1b
        je     loc_1_0147
        cmp    eax, 0x1c
        je     loc_1_0150
        cmp    eax, 0x23
        je     loc_1_0160
        clc    
        ret    
DSOUND_PM_API:
        clc    
        ret    
loc_1_0147:
        call   sub_1_01E0
        sub    eax, 1
        ret    
loc_1_0150:
        call   sub_1_01E5
        sub    eax, 1
        ret    
DSOUND_Service_00:
        mov    eax, 0x402
        clc    
        ret    
loc_1_0160:
        cmp    ecx, 0x1e
        jb     loc_1_0178
        xor    eax, eax
        cmp    ecx, -1
        jne    loc_1_0170
        ret    
loc_1_0170:
        mov    eax, ecx
        mov    eax, 0xffffffff
        ret    
loc_1_0178:
        push   edi
        push   esi
        push   ebx
        mov    dword ptr [0], esi            ; -> obj1:0x012C
        push   esi
        call   dword ptr [ecx*4]             ; -> obj1:0x00B4
        add    esp, 4
        pop    ebx
        pop    esi
        pop    edi
        ret    
loc_1_0190:
        xor    eax, eax
        mov    ecx, 0x402
        ret    
loc_1_0198:
        mov    eax, edx
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
        ret    
loc_1_01A1:
        push   ebx
        push   esi
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, 0                        ; -> loc_1_0198
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
        pop    esi
        pop    ebx
        ret    
sub_1_01B7:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        jne    loc_1_01CD
        mov    eax, dword ptr [ebp + 8]
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
        jmp    loc_1_01DD
loc_1_01CD:
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 0                        ; -> loc_1_01A1
        VMMCall  VMM.Set_Async_Time_Out          ; cd20fe000100
        mov    eax, esi
loc_1_01DD:
        pop    esi
        leave  
        ret    
sub_1_01E0:
        jmp    loc_3_0A8C
sub_1_01E5:
        jmp    loc_3_0AF1
        db  0CCh, 0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xd6  R-X
sub_2_0000:
        VMMCall  VMM._PageAllocate               ; cd2053800100
        int3   
        int3   
sub_2_0008:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  0CCh
sub_2_0014:
        VMMCall  VMM._PageFree                   ; cd2055800100
        int3   
        int3   
sub_2_001C:
        VMMCall  VMM._PageReserve                ; cd201d810100
        int3   
        int3   
sub_2_0024:
        VMMCall  VMM._PageDecommit               ; cd201f810100
        int3   
        int3   
sub_2_002C:
        VMMCall  VMM._LinPageLock                ; cd2063800100
        int3   
        int3   
sub_2_0034:
        VMMCall  VMM._LinPageUnLock              ; cd2064800100
        int3   
        int3   
sub_2_003C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_004C:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_005C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_006C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_2_0080:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_0090:
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
sub_2_00A4:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_00B4:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_2_00BC:
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
sub_2_00D0:
        VMMCall  VMM._HeapFree                   ; cd2051800100
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xb2b  R-X
loc_3_0000:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0xc]
        push   edi
        push   8
        xor    esi, esi
        mov    edi, dword ptr [ebx + 0x18]
        push   esi
        push   esi
        push   esi
        push   esi
        call   sub_2_0008
loc_3_0017:
        push   eax
        push   esi
        push   1
        call   sub_2_0000
loc_3_0020:
        add    esp, 0x20
        mov    dword ptr [0], eax            ; -> obj1:0x0130
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x8007000e
        mov    dword ptr [edi], eax
        add    edi, 4
        sub    edi, dword ptr [ebx + 0x18]
        mov    eax, dword ptr [ebx + 0x20]
        mov    dword ptr [eax], edi
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_0045:
        cmp    dword ptr [0], 0              ; -> obj1:0x0130
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edi + 0x18]
        je     loc_3_0067
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x0130
        push   eax
        call   sub_2_0014
loc_3_0064:
        add    esp, 8
loc_3_0067:
        mov    dword ptr [esi], 0
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_007D:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 8]
        mov    edx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   edx
        call   sub_3_08C1
        add    esp, 0xc
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_00AE:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 8]
        mov    edx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   edx
        call   sub_3_08D5
        add    esp, 0xc
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_00DF:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_3_092A
        add    esp, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_010A:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_3_0960
        add    esp, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_0135:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x10]
        push   dword ptr [eax]
        call   sub_3_099D
        add    esp, 4
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_015D:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_3_09C0
        add    esp, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_0188:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x20]
        mov    ebp, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        mov    ecx, dword ptr [ebp]
        add    ebp, 4
        mov    edx, dword ptr [ebp]
        add    ebp, 4
        push   dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [ebp]
        add    ebp, 4
        mov    eax, dword ptr [ebp]
        add    ebp, 4
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [ebp]
        add    ebp, 4
        mov    dword ptr [esp + 0x18], eax
        mov    ebp, dword ptr [ebp]
        push   ebp
        push   dword ptr [esp + 0x1c]
        mov    ebp, dword ptr [esp + 0x24]
        push   ebp
        push   ebx
        push   edx
        push   ecx
        call   sub_3_09D1
        add    esp, 0x1c
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    ebp
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_3_01F1:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 8]
        mov    edx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   edx
        call   sub_3_09F4
        add    esp, 0xc
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_0222:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x10]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_0248:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x24]
        mov    ebp, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        mov    ecx, dword ptr [ebp]
        add    ebp, 4
        mov    edx, dword ptr [ebp]
        add    ebp, 4
        mov    ebx, dword ptr [ebp]
        add    ebp, 4
        mov    eax, dword ptr [ebp]
        add    ebp, 4
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [ebp]
        add    ebp, 4
        mov    dword ptr [esp + 0x18], eax
        push   dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp]
        add    ebp, 4
        mov    dword ptr [esp + 0x18], eax
        mov    ebp, dword ptr [ebp]
        push   ebp
        push   dword ptr [esp + 0x1c]
        mov    ebp, dword ptr [esp + 0x24]
        mov    eax, dword ptr [ecx]
        push   ebp
        push   dword ptr [esp + 0x2c]
        push   ebx
        push   edx
        push   ecx
        call   dword ptr [eax + 0xc]
        pop    ebp
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_3_02BC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        mov    ebx, dword ptr [eax]
        mov    ebp, dword ptr [eax + 0x10]
        add    eax, 4
        push   ebp
        mov    ecx, dword ptr [eax]
        add    eax, 4
        mov    edx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   edx
        push   ecx
        mov    eax, dword ptr [ebx]
        push   ebx
        call   dword ptr [eax + 0x10]
        pop    ebp
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        ret    
loc_3_02FB:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 0x14]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_0324:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 0x18]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_034D:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 0x1c]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_0376:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 0x20]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_039F:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 8]
        mov    edx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   edx
        mov    eax, dword ptr [edx]
        call   dword ptr [eax + 0x24]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_03CD:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 0xc]
        mov    edx, dword ptr [eax]
        add    eax, 4
        mov    ebx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   ebx
        push   edx
        mov    eax, dword ptr [edx]
        call   dword ptr [eax + 0x28]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        ret    
loc_3_0403:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x10]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0x2c]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_0429:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_1_01B7
loc_3_043F:
        add    esp, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_0454:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x10]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 8], ecx
        xor    ecx, ecx
        xor    edx, edx
        mov    eax, dword ptr [ebp - 8]
        VxDCall  NDIS.Service_0014h              ; cd2014002a00
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_0496:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0xc]
        push   edi
        push   ebp
        mov    eax, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [ebx + 0x18]
        mov    esi, dword ptr [eax]
        mov    ebp, esi
        mov    eax, dword ptr [eax + 4]
        and    ebp, 0xfffff000
        push   8
        sub    eax, ebp
        lea    eax, [eax + esi + 0xfff]
        cdq    
        and    edx, 0xfff
        add    eax, edx
        sar    eax, 0xc
        push   eax
        push   0x80060000
        call   sub_2_001C
loc_3_04D3:
        add    esp, 0xc
        cmp    eax, -1
        je     loc_3_04E5
        test   eax, eax
        je     loc_3_04E5
        sub    ebp, esi
        add    eax, ebp
        jmp    loc_3_04E7
loc_3_04E5:
        xor    eax, eax
loc_3_04E7:
        pop    ebp
        mov    dword ptr [edi], eax
        add    edi, 4
        mov    eax, dword ptr [ebx + 0x20]
        sub    edi, dword ptr [ebx + 0x18]
        mov    dword ptr [eax], edi
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_04FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        mov    ebx, dword ptr [edx]
        add    edx, 4
        mov    eax, dword ptr [edx]
        mov    edx, dword ptr [edx + 4]
        mov    ecx, eax
        and    ecx, 0xfffff000
        sub    edx, ecx
        shr    ecx, 0xc
        lea    eax, [edx + eax + 0xfff]
        cdq    
        shr    ebx, 0xc
        and    edx, 0xfff
        add    eax, edx
        sar    eax, 0xc
        mov    edx, eax
        xor    eax, eax
        VMMCall  VMM._GetCurrentContext          ; cd2057010100
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x14], edx
        mov    dword ptr [ebp - 0x10], ebx
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], ecx
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        VMMCall  VMM._PageAttach                 ; cd2025010100
        add    esp, 0x10
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_0591:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, edx
        and    ecx, 0xfffff000
        sub    eax, ecx
        push   0
        shr    ecx, 0xc
        lea    eax, [eax + edx + 0xfff]
        cdq    
        and    edx, 0xfff
        add    eax, edx
        sar    eax, 0xc
        push   eax
        push   ecx
        call   sub_2_0024
loc_3_05CB:
        add    esp, 0xc
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_05E6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, edx
        and    ecx, 0xfffff000
        sub    eax, ecx
        shr    ecx, 0xc
        lea    eax, [eax + edx + 0xfff]
        cdq    
        and    edx, 0xfff
        mov    dword ptr [ebp - 4], ecx
        add    eax, edx
        sar    eax, 0xc
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0130
        shr    eax, 0xc
        mov    dword ptr [ebp - 0x20], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._EnterMustComplete          ; cd2035010100
        push   0
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        call   sub_2_0024
loc_3_0647:
        add    esp, 0xc
        cmp    eax, 1
        sbb    ebx, ebx
        inc    ebx
        je     loc_3_06AA
loc_3_0652:
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [ebp - 8]
        test   eax, eax
        je     loc_3_06AA
        mov    ecx, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 4]
        xor    eax, eax
        VMMCall  VMM._GetCurrentContext          ; cd2057010100
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    dword ptr [ebp - 0x18], ecx
        mov    ecx, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x1c], 1
        mov    dword ptr [ebp - 0x10], ecx
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        VMMCall  VMM._PageAttach                 ; cd2025010100
        add    esp, 0x10
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 1
        sbb    ebx, ebx
        inc    ebx
        jne    loc_3_0652
loc_3_06AA:
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._LeaveMustComplete          ; cd2036010100
        mov    dword ptr [esi], ebx
        add    esi, 4
        sub    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_06CC:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        push   0
        mov    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x10]
        mov    eax, dword ptr [eax]
        and    eax, 0xfffff000
        push   eax
        call   sub_2_0014
loc_3_06E7:
        add    esp, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret    
loc_3_06FC:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [ecx]
        add    ecx, 4
        mov    ebx, eax
        mov    edx, dword ptr [ecx]
        and    ebx, 0xfffff000
        sub    edx, ebx
        push   dword ptr [ecx + 4]
        shr    ebx, 0xc
        lea    eax, [edx + eax + 0xfff]
        cdq    
        and    edx, 0xfff
        add    eax, edx
        sar    eax, 0xc
        push   eax
        push   ebx
        call   sub_2_002C
loc_3_073A:
        add    esp, 0xc
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        ret    
loc_3_0750:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        mov    eax, dword ptr [ecx]
        add    ecx, 4
        mov    ebx, eax
        mov    edx, dword ptr [ecx]
        and    ebx, 0xfffff000
        sub    edx, ebx
        push   dword ptr [ecx + 4]
        shr    ebx, 0xc
        lea    eax, [edx + eax + 0xfff]
        cdq    
        and    edx, 0xfff
        add    eax, edx
        sar    eax, 0xc
        push   eax
        push   ebx
        call   sub_2_0034
loc_3_078E:
        add    esp, 0xc
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 0x20]
        sub    esi, dword ptr [edi + 0x18]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        ret    
sub_3_07A4:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        xor    edi, edi
        xor    esi, esi
loc_3_07AD:
        mov    ecx, dword ptr [0]            ; -> obj1:0x0000
        add    ecx, esi
        cmp    dword ptr [ecx + 4], 0
        jne    loc_3_07CC
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, ebx
        lea    edx, [eax + 1]
        mov    dword ptr [ecx + 8], edx
        jb     loc_3_07CC
        mov    edi, ecx
        mov    ebx, edx
loc_3_07CC:
        add    esi, 0xc
        cmp    esi, 0x78
        jl     loc_3_07AD
        mov    eax, 0
        test   edi, edi
        je     loc_3_07E6
        mov    dword ptr [edi + 4], 1
        mov    eax, dword ptr [edi]
loc_3_07E6:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_07EA:
        push   ebx
        mov    edx, 0
        push   esi
        mov    ebx, 0
        push   edi
        push   ebp
        mov    ebp, 0
loc_3_07FD:
        mov    eax, dword ptr [0]            ; -> obj1:0x0000
        mov    ecx, 0x10
        mov    edi, dword ptr [esp + 0x14]
        mov    esi, dword ptr [eax + ebp]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_3_081D
        add    ebp, 0xc
        inc    edx
        cmp    ebp, 0x78
        jl     loc_3_07FD
        jmp    loc_3_0828
loc_3_081D:
        imul   edx, edx, 0xc
        add    edx, dword ptr [0]            ; -> obj1:0x0000
        mov    ebx, edx
loc_3_0828:
        xor    eax, eax
        test   ebx, ebx
        je     loc_3_0834
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 8], eax
loc_3_0834:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_0839:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        call   sub_2_004C
loc_3_0845:
        test   eax, eax
        je     loc_3_0861
        mov    esi, dword ptr [esp + 8]
loc_3_084D:
        cmp    dword ptr [eax], esi
        je     loc_3_0861
        push   eax
        push   dword ptr [0]                 ; -> obj4:0x0000
        call   sub_2_003C
loc_3_085D:
        test   eax, eax
        jne    loc_3_084D
loc_3_0861:
        pop    esi
        ret    
sub_3_0863:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   edi
        push   eax
        call   sub_2_004C
loc_3_0870:
        test   eax, eax
        je     loc_3_0894
loc_3_0874:
        mov    ecx, 0x10
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [eax + 4]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_3_0894
        push   eax
        push   dword ptr [0]                 ; -> obj4:0x0000
        call   sub_2_003C
loc_3_0890:
        test   eax, eax
        jne    loc_3_0874
loc_3_0894:
        pop    edi
        pop    esi
        ret    
sub_3_0897:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        mov    esi, dword ptr [esp + 8]
        push   esi
        push   eax
        call   sub_2_006C
loc_3_08A8:
        push   dword ptr [esi + 4]
        call   sub_3_07EA
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   esi
        push   eax
        call   sub_2_005C
loc_3_08BF:
        pop    esi
        ret    
sub_3_08C1:
        push   dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax]
        ret    
sub_3_08D5:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        test   eax, eax
        jne    loc_3_08EC
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        call   sub_2_004C
loc_3_08EA:
        jmp    loc_3_0905
loc_3_08EC:
        push   eax
        call   sub_3_0863
        add    esp, 4
        test   eax, eax
        je     loc_3_0909
        push   eax
        push   dword ptr [0]                 ; -> obj4:0x0000
        call   sub_2_003C
loc_3_0905:
        test   eax, eax
        jne    loc_3_0910
loc_3_0909:
        mov    eax, 0x88780078
        jmp    loc_3_0927
loc_3_0910:
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax + 4]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        push   dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0xc]
loc_3_0927:
        pop    edi
        pop    esi
        ret    
sub_3_092A:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        jne    loc_3_093F
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        call   sub_2_004C
loc_3_093D:
        jmp    loc_3_0948
loc_3_093F:
        push   eax
        call   sub_3_0863
        add    esp, 4
loc_3_0948:
        test   eax, eax
        jne    loc_3_0953
        mov    eax, 0x88780078
        jmp    loc_3_095F
loc_3_0953:
        push   dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0xc]
loc_3_095F:
        ret    
sub_3_0960:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   dword ptr [esp + 0x10]
        mov    dword ptr [esi], 0
        call   sub_3_0863
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_3_0986
        mov    eax, 0x88780078
        jmp    loc_3_0999
loc_3_0986:
        mov    eax, dword ptr [edi]
        push   eax
        mov    ebx, dword ptr [eax]
        call   dword ptr [ebx + 0x10]
        test   eax, eax
        jne    loc_3_0999
        xor    eax, eax
        inc    dword ptr [edi + 8]
        mov    dword ptr [esi], edi
loc_3_0999:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_099D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_3_09B0
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0x14]
        jmp    loc_3_09BB
loc_3_09B0:
        push   esi
        call   sub_3_0897
        add    esp, 4
        xor    eax, eax
loc_3_09BB:
        dec    dword ptr [esi + 8]
        pop    esi
        ret    
sub_3_09C0:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0x18]
        ret    
sub_3_09D1:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   dword ptr [eax + 0x1c]
        pop    ebp
        ret    
sub_3_09F4:
        push   dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 0x20]
        ret    
DSOUND_Service_01:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   edi
        push   eax
        call   sub_2_0090
loc_3_0A16:
        mov    esi, eax
        test   esi, esi
        jne    loc_3_0A23
        mov    eax, 0x8007000e
        jmp    loc_3_0A5D
loc_3_0A23:
        mov    edi, esi
        xor    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        call   sub_3_07A4
        mov    dword ptr [esi + 4], eax
        test   eax, eax
        jne    loc_3_0A49
        push   esi
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        call   sub_2_005C
loc_3_0A42:
        mov    eax, 0x80004005
        jmp    loc_3_0A5D
loc_3_0A49:
        mov    eax, dword ptr [esp + 0xc]
        push   esi
        mov    dword ptr [esi], eax
        push   dword ptr [0]                 ; -> obj4:0x0000
        call   sub_2_0080
loc_3_0A5B:
        xor    eax, eax
loc_3_0A5D:
        pop    edi
        pop    esi
        ret    
DSOUND_Service_02:
        push   dword ptr [esp + 4]
        call   sub_3_0839
        add    esp, 4
        test   eax, eax
        jne    loc_3_0A77
        mov    eax, 0x80070057
        jmp    loc_3_0A8B
loc_3_0A77:
        xor    ecx, ecx
        mov    dword ptr [eax], ecx
        cmp    dword ptr [eax + 8], ecx
        jne    loc_3_0A89
        push   eax
        call   sub_3_0897
        add    esp, 4
loc_3_0A89:
        xor    eax, eax
loc_3_0A8B:
        ret    
loc_3_0A8C:
        push   esi
        push   0xc
        push   4
        call   sub_2_00BC
loc_3_0A96:
        mov    dword ptr [0], eax            ; -> obj4:0x0000
        test   eax, eax
        jne    loc_3_0AA3
        xor    eax, eax
        jmp    loc_3_0AEF
loc_3_0AA3:
        push   1
        xor    esi, esi
        push   0x78
        call   sub_2_00B4
loc_3_0AAE:
        add    esp, 8
        mov    dword ptr [0], eax            ; -> obj1:0x0000
        cmp    eax, esi
        jne    loc_3_0ACF
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        call   sub_2_00A4
loc_3_0AC5:
        xor    eax, eax
        mov    dword ptr [0], esi            ; -> obj4:0x0000
        jmp    loc_3_0AEF
loc_3_0ACF:
        mov    ecx, 0                        ; -> obj4:0x0008
        xor    eax, eax
loc_3_0AD6:
        mov    edx, dword ptr [0]            ; -> obj1:0x0000
        mov    dword ptr [edx + eax], ecx
        add    ecx, 0x10
        add    eax, 0xc
        cmp    eax, 0x78
        jl     loc_3_0AD6
        mov    eax, 1
loc_3_0AEF:
        pop    esi
        ret    
loc_3_0AF1:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj1:0x0000
        xor    esi, esi
        test   eax, eax
        je     loc_3_0B0D
        push   esi
        push   eax
        call   sub_2_00D0
loc_3_0B04:
        add    esp, 8
        mov    dword ptr [0], esi            ; -> obj1:0x0000
loc_3_0B0D:
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        xor    esi, esi
        test   eax, eax
        je     loc_3_0B24
        push   eax
        call   sub_2_00A4
loc_3_0B1E:
        mov    dword ptr [0], esi            ; -> obj4:0x0000
loc_3_0B24:
        mov    eax, 1
        pop    esi
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0xa8  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 092h, 00Bh, 03Dh, 0FCh, 0ABh, 0CEh, 011h
        db  0A3h, 0B3h, 000h, 0AAh, 000h, 04Ah, 09Fh, 00Ch, 0C1h, 092h, 00Bh, 03Dh, 0FCh, 0ABh, 0CEh, 011h
        db  0A3h, 0B3h, 000h, 0AAh, 000h, 04Ah, 09Fh, 00Ch, 0C2h, 092h, 00Bh, 03Dh, 0FCh, 0ABh, 0CEh, 011h
        db  0A3h, 0B3h, 000h, 0AAh, 000h, 04Ah, 09Fh, 00Ch, 0C3h, 092h, 00Bh, 03Dh, 0FCh, 0ABh, 0CEh, 011h
        db  0A3h, 0B3h, 000h, 0AAh, 000h, 04Ah, 09Fh, 00Ch, 0C4h, 092h, 00Bh, 03Dh, 0FCh, 0ABh, 0CEh, 011h
        db  0A3h, 0B3h, 000h, 0AAh, 000h, 04Ah, 09Fh, 00Ch, 0C5h, 092h, 00Bh, 03Dh, 0FCh, 0ABh, 0CEh, 011h
        db  0A3h, 0B3h, 000h, 0AAh, 000h, 04Ah, 09Fh, 00Ch, 0C6h, 092h, 00Bh, 03Dh, 0FCh, 0ABh, 0CEh, 011h
        db  0A3h, 0B3h, 000h, 0AAh, 000h, 04Ah, 09Fh, 00Ch, 0C7h, 092h, 00Bh, 03Dh, 0FCh, 0ABh, 0CEh, 011h
        db  0A3h, 0B3h, 000h, 0AAh, 000h, 04Ah, 09Fh, 00Ch, 0C8h, 092h, 00Bh, 03Dh, 0FCh, 0ABh, 0CEh, 011h
        db  0A3h, 0B3h, 000h, 0AAh, 000h, 04Ah, 09Fh, 00Ch, 0C9h, 092h, 00Bh, 03Dh, 0FCh, 0ABh, 0CEh, 011h
        db  0A3h, 0B3h, 000h, 0AAh, 000h, 04Ah, 09Fh, 00Ch
VxD__LDATA ENDS
