; CONFIGMG -- reconstructed 32-bit disassembly (VxD / LE format)
; 8 objects, 22390 instructions recovered by recursive descent

Declare_Virtual_Device CONFIGMG, 4, 0, CONFIGMG_Control, 00033h, Undefined_Init_Order
;   DDK version 0400h   init_order 01680FFFFh   flags 00000h
;   service table: obj1:0x0, 109 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x6e4  R-X
        dd  _CONFIGMG_Get_Version    ; fixup
        dd  _CONFIGMG_Initialize     ; fixup
        dd  _CONFIGMG_Locate_DevNode ; fixup
        dd  _CONFIGMG_Get_Parent     ; fixup
        dd  _CONFIGMG_Get_Child      ; fixup
        dd  _CONFIGMG_Get_Sibling    ; fixup
        dd  _CONFIGMG_Get_Device_ID_Size ; fixup
        dd  _CONFIGMG_Get_Device_ID  ; fixup
        dd  _CONFIGMG_Get_Depth      ; fixup
        dd  _CONFIGMG_Get_Private_DWord ; fixup
        dd  _CONFIGMG_Set_Private_DWord ; fixup
        dd  _CONFIGMG_Create_DevNode ; fixup
        dd  _CONFIGMG_Query_Remove_SubTree ; fixup
        dd  _CONFIGMG_Remove_SubTree ; fixup
        dd  _CONFIGMG_Register_Device_Driver ; fixup
        dd  _CONFIGMG_Register_Enumerator ; fixup
        dd  _CONFIGMG_Register_Arbitrator ; fixup
        dd  _CONFIGMG_Deregister_Arbitrator ; fixup
        dd  _CONFIGMG_Query_Arbitrator_Free_Size ; fixup
        dd  _CONFIGMG_Query_Arbitrator_Free_Data ; fixup
        dd  _CONFIGMG_Sort_NodeList  ; fixup
        dd  _CONFIGMG_Yield          ; fixup
        dd  _CONFIGMG_Lock           ; fixup
        dd  _CONFIGMG_Unlock         ; fixup
        dd  _CONFIGMG_Add_Empty_Log_Conf ; fixup
        dd  _CONFIGMG_Free_Log_Conf  ; fixup
        dd  _CONFIGMG_Get_First_Log_Conf ; fixup
        dd  _CONFIGMG_Get_Next_Log_Conf ; fixup
        dd  _CONFIGMG_Add_Res_Des    ; fixup
        dd  _CONFIGMG_Modify_Res_Des ; fixup
        dd  _CONFIGMG_Free_Res_Des   ; fixup
        dd  _CONFIGMG_Get_Next_Res_Des ; fixup
        dd  _CONFIGMG_Get_Performance_Info ; fixup
        dd  _CONFIGMG_Get_Res_Des_Data_Size ; fixup
        dd  _CONFIGMG_Get_Res_Des_Data ; fixup
        dd  _CONFIGMG_Process_Events_Now ; fixup
        dd  _CONFIGMG_Create_Range_List ; fixup
        dd  _CONFIGMG_Add_Range      ; fixup
        dd  _CONFIGMG_Delete_Range   ; fixup
        dd  _CONFIGMG_Test_Range_Available ; fixup
        dd  _CONFIGMG_Dup_Range_List ; fixup
        dd  _CONFIGMG_Free_Range_List ; fixup
        dd  _CONFIGMG_Invert_Range_List ; fixup
        dd  _CONFIGMG_Intersect_Range_List ; fixup
        dd  _CONFIGMG_First_Range    ; fixup
        dd  _CONFIGMG_Next_Range     ; fixup
        dd  _CONFIGMG_Dump_Range_List ; fixup
        dd  _CONFIGMG_Load_DLVxDs    ; fixup
        dd  _CONFIGMG_Get_DDBs       ; fixup
        dd  _CONFIGMG_Get_CRC_CheckSum ; fixup
        dd  _CONFIGMG_Register_DevLoader ; fixup
        dd  _CONFIGMG_Reenumerate_DevNode ; fixup
        dd  _CONFIGMG_Setup_DevNode  ; fixup
        dd  _CONFIGMG_Reset_Children_Marks ; fixup
        dd  _CONFIGMG_Get_DevNode_Status ; fixup
        dd  _CONFIGMG_Remove_Unmarked_Children ; fixup
        dd  _CONFIGMG_ISAPNP_To_CM   ; fixup
        dd  _CONFIGMG_CallBack_Device_Driver ; fixup
        dd  _CONFIGMG_CallBack_Enumerator ; fixup
        dd  _CONFIGMG_Get_Alloc_Log_Conf ; fixup
        dd  _CONFIGMG_Get_DevNode_Key_Size ; fixup
        dd  _CONFIGMG_Get_DevNode_Key ; fixup
        dd  _CONFIGMG_Read_Registry_Value ; fixup
        dd  _CONFIGMG_Write_Registry_Value ; fixup
        dd  _CONFIGMG_Disable_DevNode ; fixup
        dd  _CONFIGMG_Enable_DevNode ; fixup
        dd  _CONFIGMG_Move_DevNode   ; fixup
        dd  _CONFIGMG_Set_Bus_Info   ; fixup
        dd  _CONFIGMG_Get_Bus_Info   ; fixup
        dd  _CONFIGMG_Set_HW_Prof    ; fixup
        dd  _CONFIGMG_Recompute_HW_Prof ; fixup
        dd  _CONFIGMG_Query_Change_HW_Prof ; fixup
        dd  _CONFIGMG_Get_Device_Driver_Private_DWord ; fixup
        dd  _CONFIGMG_Set_Device_Driver_Private_DWord ; fixup
        dd  _CONFIGMG_Get_HW_Prof_Flags ; fixup
        dd  _CONFIGMG_Set_HW_Prof_Flags ; fixup
        dd  _CONFIGMG_Read_Registry_Log_Confs ; fixup
        dd  _CONFIGMG_Run_Detection  ; fixup
        dd  _CONFIGMG_Call_At_Appy_Time ; fixup
        dd  _CONFIGMG_Fail_Change_HW_Prof ; fixup
        dd  _CONFIGMG_Set_Private_Problem ; fixup
        dd  _CONFIGMG_Debug_DevNode  ; fixup
        dd  _CONFIGMG_Get_Hardware_Profile_Info ; fixup
        dd  _CONFIGMG_Register_Enumerator_Function ; fixup
        dd  _CONFIGMG_Call_Enumerator_Function ; fixup
        dd  _CONFIGMG_Add_ID         ; fixup
        dd  _CONFIGMG_Find_Range     ; fixup
        dd  _CONFIGMG_Get_Global_State ; fixup
        dd  _CONFIGMG_Broadcast_Device_Change_Message ; fixup
        dd  _CONFIGMG_Call_DevNode_Handler ; fixup
        dd  _CONFIGMG_Remove_Reinsert_All ; fixup
        dd  CONFIGMG_Service_91      ; fixup
        dd  CONFIGMG_Service_92      ; fixup
        dd  CONFIGMG_Service_93      ; fixup
        dd  CONFIGMG_Service_94      ; fixup
        dd  CONFIGMG_Service_95      ; fixup
        dd  CONFIGMG_Service_96      ; fixup
        dd  CONFIGMG_Service_97      ; fixup
        dd  CONFIGMG_Service_98      ; fixup
        dd  CONFIGMG_Service_99      ; fixup
        dd  CONFIGMG_Service_100     ; fixup
        dd  CONFIGMG_Service_101     ; fixup
        dd  CONFIGMG_Service_102     ; fixup
        dd  CONFIGMG_Service_103     ; fixup
        dd  CONFIGMG_Service_104     ; fixup
        dd  CONFIGMG_Service_105     ; fixup
        dd  CONFIGMG_Service_106     ; fixup
        dd  CONFIGMG_Service_107     ; fixup
        dd  CONFIGMG_Service_108     ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00033h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'CONFIGMG'                  ; DDB_Name
        dd  01680FFFFh                  ; DDB_Init_Order
        dd  CONFIGMG_Control         ; DDB_Control_Proc
        dd  CONFIGMG_PM_API          ; DDB_V86_API_Proc
        dd  CONFIGMG_PM_API          ; DDB_PM_API_Proc
        dd  obj1:0x0000              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
CONFIGMG_Control:
        cmp    eax, 0x23
        je     loc_5_00FB
loc_1_04B5:
        cmp    eax, 0x22
        jne    loc_1_04C3
        push   edx
        push   ebx
        call   sub_5_C70F
loc_1_04C1:
        stc    
        ret    
loc_1_04C3:
        cmp    eax, 2
        jne    loc_1_04D1
        call   sub_8_0282
loc_1_04CD:
        cmp    eax, 1
        ret    
loc_1_04D1:
        cmp    eax, 1
        jne    loc_1_04DF
        call   sub_8_0316
loc_1_04DB:
        cmp    eax, 1
        ret    
loc_1_04DF:
        cmp    eax, 0x17
        jne    loc_1_04ED
        call   sub_3_0113
loc_1_04E9:
        cmp    eax, 1
        ret    
loc_1_04ED:
        cmp    eax, 4
        jne    loc_1_04FB
        call   sub_3_00F5
loc_1_04F7:
        cmp    eax, 1
        ret    
loc_1_04FB:
        clc    
        ret    
sub_1_04FD:
        pushfd 
        cli    
        mov    eax, dword ptr [0]            ; -> obj1:0x020C
        push   eax
        call   sub_1_06B8
        popfd  
        ret    
sub_1_050C:
        pushfd 
        cli    
        push   ecx
        mov    eax, dword ptr [0]            ; -> obj1:0x020C
        push   eax
        call   sub_1_06CC
        popfd  
        ret    
sub_1_051C:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    esi, ecx
        call   sub_1_04FD
        mov    edi, eax
        test   edi, edi
        je     loc_1_0561
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        push   1
        mov    dword ptr [edi + 0x10], esi
        push   edi
        mov    dword ptr [edi], ebx
        push   0                             ; -> loc_5_01BE
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], ecx
        call   sub_1_06DC
        add    esp, 0xc
        test   eax, eax
        mov    eax, 1
        jne    loc_1_0563
        mov    ecx, edi
        call   sub_1_050C
loc_1_0561:
        xor    eax, eax
loc_1_0563:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_1_0569:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 1
        je     loc_1_057B
        cmp    eax, 4
        je     loc_1_057B
        xor    eax, eax
        jmp    loc_1_0597
loc_1_057B:
        push   0
        sub    eax, 4
        push   0
        mov    ecx, 0                        ; -> loc_5_6DFA
        push   dword ptr [esp + 0x10]
        cmp    eax, 1
        sbb    edx, edx
        neg    edx
        call   sub_5_02D3
loc_1_0597:
        ret    
sub_1_0598:
        push   0
        mov    edx, ecx
        push   0
        mov    ecx, 0                        ; -> loc_5_7B17
        inc    dword ptr [0]                 ; -> obj1:0x04A8
        call   sub_1_051C
        ret    
_CONFIGMG_Lock:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_05BD
        mov    eax, 4
        jmp    loc_1_05D4
loc_1_05BD:
        xor    eax, eax
        cmp    dword ptr [0], eax            ; -> obj1:0x0204
        jne    loc_1_05CC
        mov    dword ptr [0], eax            ; -> obj1:0x0208
loc_1_05CC:
        xor    eax, eax
        inc    dword ptr [0]                 ; -> obj1:0x0204
loc_1_05D4:
        ret    
_CONFIGMG_Unlock:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_05E3
        mov    eax, 4
        jmp    loc_1_0606
loc_1_05E3:
        cmp    dword ptr [0], 0              ; -> obj1:0x0204
        je     loc_1_0604
        dec    dword ptr [0]                 ; -> obj1:0x0204
        jne    loc_1_0604
        cmp    dword ptr [0], 0              ; -> obj1:0x0208
        je     loc_1_0604
        xor    ecx, ecx
        call   sub_1_0598
loc_1_0604:
        xor    eax, eax
loc_1_0606:
        ret    
_CONFIGMG_Reenumerate_DevNode:
        push   ebp
        mov    ebp, esp
        test   dword ptr [esp + 0xc], 0xfffffffe
        je     loc_1_061B
        mov    eax, 4
        jmp    loc_1_0632
loc_1_061B:
        test   byte ptr [ebp + 0xc], 1
        mov    ecx, dword ptr [ebp + 8]
        je     loc_1_062B
        call   sub_5_9463
loc_1_0629:
        jmp    loc_1_0632
loc_1_062B:
        call   sub_1_0598
        xor    eax, eax
loc_1_0632:
        pop    ebp
        ret    
_CONFIGMG_Call_At_Appy_Time:
        cmp    dword ptr [esp + 0xc], 0
        mov    eax, 4
        jne    loc_1_0659
        mov    edx, dword ptr [esp + 8]
        push   0
        mov    ecx, dword ptr [esp + 8]
        push   0
        call   sub_1_051C
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x13
loc_1_0659:
        ret    
_CONFIGMG_Yield:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushfd 
        pop    dword ptr [ebp - 4]
        test   dword ptr [ebp + 0xc], 0xfffffffe
        je     loc_1_0674
        mov    eax, 4
        jmp    loc_1_06B2
loc_1_0674:
        test   byte ptr [ebp - 3], 2
        jne    loc_1_0689
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_06B0
loc_1_0680:
        in     al, 0x84
        dec    dword ptr [ebp + 8]
        jne    loc_1_0680
        jmp    loc_1_06B0
loc_1_0689:
        cmp    dword ptr [ebp + 0xc], 1
        jne    loc_1_06A6
        cmp    dword ptr [ebp + 8], 0xc8
        jbe    loc_1_06A6
        call   sub_5_D0B8
loc_1_069D:
        mov    edx, 1
        test   al, 2
        jne    loc_1_06A8
loc_1_06A6:
        xor    edx, edx
loc_1_06A8:
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_C80C
loc_1_06B0:
        xor    eax, eax
loc_1_06B2:
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh
sub_1_06B8:
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
sub_1_06CC:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_1_06DC:
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e801700
        int3   
        int3   
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xaca  R-X
        db  016h, 000h, 000h, 000h
        dd  loc_2_003C               ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h, 054h, 000h, 060h, 000h, 069h, 000h, 072h, 000h, 089h, 000h
        db  0DAh, 001h, 006h, 002h, 0A7h, 002h, 0CAh, 002h, 071h, 003h, 09Ch, 003h, 030h, 004h, 054h, 004h
        db  05Eh, 004h, 011h, 005h, 026h, 005h, 0DFh, 005h, 002h, 006h, 077h, 006h, 09Ah, 007h, 0BDh, 009h
        db  08Eh, 00Ah, 0CCh, 0CCh
loc_2_003C:
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x2073776f
        imul   esi, dword ptr [ebx + 0x20], 0x62616e75
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[edi + ebp*2 + 0x20], dh
        je     loc_2_00BA
        jb     loc_2_00C4
        imul   ebp, dword ptr [esi + 0x65], 0x756f7920
        jb     0x80
        push   0x77647261
        popal  
        jb     0xcd
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        imul   bp, word ptr [ebp + 0x2e], 0x2020
        dec    ecx
        jae    loc_2_0096
        and    ah, byte ptr [0x74202273]
        push   0x75632065
        jb     0xf5
        outsb  dx, byte ptr gs:[esi]
        je     0xa7
        jo     0xfb
        outsd  dx, dword ptr [esi]
        imul   bp, word ptr [ebp + 0x3f], 0x4400
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x20], bp
        inc    ebx
loc_2_0096:
        push   0x65676e61
        add    byte ptr [ebp + 0x6e], dl
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x65], bp
        add    byte ptr fs:[edi + ebp*2 + 0x63], al
        imul   esp, dword ptr [eax], 0x25
        insb   byte ptr es:[edi], dx
        add    byte ptr fs:[edi + 0x72], cl
        imul   esp, dword ptr [edi + 0x69], 0x206c616e
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_2_00BA:
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     loc_2_012B
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_2_00C4:
        add    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x13f
        and    byte ptr [edi + ebp*2 + 0x65], ah
        jae    loc_2_00F2
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xf6
        push   0x20657661
        popal  
        and    byte ptr [eax + 0x61], ch
        jb     0x145
        ja     loc_2_0144
        jb     0x14a
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        imul   bp, word ptr [ebp + 0x20], 0x7263
        popal  
loc_2_00F2:
        je     0x159
        and    byte ptr fs:[esi + 0x6f], ah
        jb     0x11a
        jns    0x16b
        jne    loc_2_0170
        and    byte ptr [ebp + 0x6e], dh
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x65], bp
        and    byte ptr fs:[ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     loc_2_017C
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr cs:[ecx + 0x66], cl
        and    byte ptr [ecx + 0x6f], bh
        jne    loc_2_013E
        jne    loc_2_018E
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x20], bp
        bound  esp, qword ptr [ebp + 0x66]
        outsd  dx, dword ptr [esi]
        jb     loc_2_0190
loc_2_012B:
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x1a6
        and    byte ptr [ebp + 0x74], ah
        arpl   word ptr gs:[ebx + esi*2 + 0x20], si
        je     0x1a6
loc_2_013E:
        and    byte ptr gs:[eax + 0x61], ch
        jb     loc_2_01A8
loc_2_0144:
        ja     loc_2_01A7
        jb     0x1ad
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x6e], dh
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x65], bp
        and    byte ptr fs:[ebx + 0x74], dh
        popal  
        je     loc_2_01C3
        sub    al, 0x20
        jns    0x1d1
        jne    0x1d6
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     0x1df
        je     0x1d1
        jb     loc_2_018E
        insd   dword ptr es:[edi], dx
        popal  
loc_2_0170:
        jns    loc_2_0192
        jae    0x1e8
        outsd  dx, dword ptr [esi]
        jo     loc_2_0197
        jb     0x1de
        jae    0x1eb
        outsd  dx, dword ptr [esi]
loc_2_017C:
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr fs:[esi + 0x67], 0x6f54202e
        and    byte ptr [eax + 0x61], ch
        jbe    0x1ef
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
loc_2_018E:
        outsd  dx, dword ptr fs:[esi]
loc_2_0190:
        ja     loc_2_0205
loc_2_0192:
        and    byte ptr [ebx + 0x72], ah
        popal  
loc_2_0197:
        je     loc_2_01FE
        and    byte ptr [ecx + 0x20], ah
        push   0x77647261
        popal  
        jb     0x209
        and    byte ptr [eax + 0x72], dh
loc_2_01A7:
        outsd  dx, dword ptr [esi]
loc_2_01A8:
        imul   bp, word ptr [ebp + 0x2c], 0x7320
        push   0x64207475
        outsd  dx, dword ptr [esi]
        ja     0x225
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr fs:[edx + 0x65], dh
        jae    0x234
        popal  
        jb     loc_2_0237
loc_2_01C3:
        and    byte ptr [ecx + 0x6f], bh
        jne    0x23a
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     0x243
        je     0x235
        jb     loc_2_01F2
        imul   ebp, dword ptr [esi + 0x20], 0x20656874
        jne    loc_2_0249
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x65], bp
        and    byte ptr fs:[ebx + 0x74], dh
        popal  
        je     loc_2_024C
        and    byte ptr cs:[edi + ebp*2 + 0x20], al
        jns    0x25d
        jne    loc_2_0210
        ja     0x253
loc_2_01F2:
        outsb  dx, byte ptr [esi]
        je     0x215
        je     loc_2_0266
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     loc_2_0266
        outsb  dx, byte ptr [esi]
loc_2_01FE:
        jne    0x265
        and    byte ptr [edi + 0x69], dh
        je     0x26d
loc_2_0205:
        outsd  dx, dword ptr [esi]
        jne    0x27c
        and    byte ptr [edx + 0x65], dh
        jae    loc_2_0281
        popal  
        jb     0x284
loc_2_0210:
        imul   ebp, dword ptr [esi + 0x67], 0x5700203f
        imul   ebp, dword ptr [esi + 0x64], 0x2073776f
        dec    eax
        popal  
        jae    0x242
        dec    esi
        outsd  dx, dword ptr [esi]
        je     0x246
        push   ebx
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [ebp + 0x6e], dl
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x65], bp
        and    byte ptr fs:[ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
loc_2_0237:
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     loc_2_02A8
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     loc_2_02BC
loc_2_0249:
        and    byte ptr [ebx + 0x61], ah
loc_2_024C:
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x271
        je     0x2ba
        arpl   word ptr [eax + 0x61], si
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[ecx + 0x6e], ch
        jae    loc_2_02D4
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x6f], bh
loc_2_0266:
        jne    loc_2_02DA
        and    byte ptr [ebp + 0x76], ah
        imul   esp, dword ptr [ebx + 0x65], 0x68772073
        imul   ebp, dword ptr [ebp + 0x20], 0x69207469
        jae    0x29d
        jb     0x2f4
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
loc_2_0281:
        imul   ebp, dword ptr [esi + 0x67], 0x206e6920
        jae    loc_2_02EB
        and    byte ptr gs:[ebp + 0x6f], ch
        and    byte ptr cs:[edi + ebp*2 + 0x20], dl
        je     0x2ff
        arpl   word ptr [eax + 0x61], si
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[ecx + 0x6e], ch
        jae    0x319
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
loc_2_02A8:
        and    byte ptr [ecx + 0x6f], bh
        jne    loc_2_031F
        and    byte ptr [ebp + 0x76], ah
        imul   esp, dword ptr [ebx + 0x65], 0x72202c73
        jae    loc_2_032F
        popal  
loc_2_02BC:
        jb     loc_2_0332
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     loc_2_0339
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr fs:[edi + ebp*2 + 0x6e], ah
        daa    
        je     0x2f1
        arpl   word ptr [eax + 0x6f], bp
loc_2_02D4:
        outsd  dx, dword ptr [esi]
        jae    0x33c
        and    byte ptr [ebx + 0x61], dh
loc_2_02DA:
        and    byte ptr gs:[ebp + 0x6f], ch
        add    byte ptr cs:[ebx + 0x61], al
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x30a
        inc    esp
loc_2_02EB:
        je     0x353
        arpl   word ptr [eax + 0x44], si
        jbe    loc_2_035E
        arpl   word ptr [ebp + 0x73], sp
        and    byte ptr [ecx + 0x6e], cl
        and    byte ptr [ebx + 0x61], dl
        and    byte ptr gs:[ebp + 0x6f], cl
        add    byte ptr gs:[edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     loc_2_0380
        and    byte ptr [ebx + 0x6f], ah
        jne    0x37e
        and    byte ptr fs:[esi + 0x6f], ch
        je     0x338
        imul   ebp, dword ptr [esi + 0x66], 0x206d726f
loc_2_031F:
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [edi + 0x70], ch
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        jb     0x38f
        insd   dword ptr es:[edi], dx
loc_2_032F:
        jae    0x351
        popal  
loc_2_0332:
        bound  ebp, qword ptr [edi + 0x75]
        je     0x357
        je     0x3a1
loc_2_0339:
        imul   esi, dword ptr [ebx + 0x20], 0x666e6f63
        imul   esp, dword ptr [edi + 0x75], 0x69746172
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x68], ah
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr cs:[bp + si + 0x65], al
        outsw  dx, word ptr [esi]
        jb     0x3bd
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     0x3c7
loc_2_035E:
        outsb  dx, byte ptr [esi]
        jne    0x3ca
        outsb  dx, byte ptr [esi]
        sub    al, 0x20
        jae    loc_2_03C8
        jbe    0x3ce
        and    byte ptr [ecx + 0x6e], ah
        jns    0x38e
        jne    0x3de
        jae    0x3d3
        jbe    loc_2_03D9
        and    byte ptr fs:[ecx + 0x6e], ch
        outsw  dx, word ptr [esi]
        jb     0x3e9
        popal  
        je     0x3e8
        outsd  dx, dword ptr [esi]
loc_2_0380:
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ecx + 0x6f], bh
        jne    loc_2_03FB
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        jb     loc_2_03F1
        insd   dword ptr es:[edi], dx
        jae    loc_2_03C1
        and    byte ptr [edi + ebp*2 + 0x20], al
        jns    loc_2_0408
        jne    0x3bb
        ja     0x3fe
        outsb  dx, byte ptr [esi]
        je     0x3c0
        je     0x411
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     0x411
        outsb  dx, byte ptr [esi]
        jne    loc_2_0410
        aas    
        add    byte ptr [ebx + 0x68], al
        popal  
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr [bp + 0x67], 0x6e6f4320
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     0x42a
loc_2_03C1:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x61], cl
        jns    0x3e8
loc_2_03C8:
        inc    ecx
        arpl   word ptr gs:[eax + 0x50], si
        jb     0x441
        jb     loc_2_0436
        insd   dword ptr es:[edi], dx
        jae    loc_2_03D8
loc_2_03D8:
        dec    ecx
loc_2_03D9:
        je     loc_2_03FB
        imul   esi, dword ptr [ebx + 0x20], 0x6f636572
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr fs:[eax + ebp*2 + 0x61], dh
        je     loc_2_040F
        jns    0x460
loc_2_03F1:
        jne    0x413
        arpl   word ptr [edi + ebp*2 + 0x73], bp
        and    byte ptr gs:[eax + 0x72], dh
loc_2_03FB:
        outsd  dx, dword ptr [esi]
        jb     0x460
        insd   dword ptr es:[edi], dx
        jae    0x422
        bound  esp, qword ptr [ebp + 0x66]
        outsd  dx, dword ptr [esi]
        jb     0x46d
loc_2_0408:
        and    byte ptr [edx + 0x65], dh
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jbe    0x478
loc_2_040F:
        outsb  dx, byte ptr [esi]
loc_2_0410:
        and    byte ptr [bx + 0x72], ch
        and    byte ptr [ecx + ebp*2 + 0x73], ah
        popal  
        bound  ebp, qword ptr [ecx + ebp*2 + 0x6e]
        and    byte ptr [si + 0x68], dh
        imul   esi, dword ptr [ebx + 0x20], 0x69766564
        arpl   word ptr [ebp + 0x20], sp
        sub    byte ptr [0x202e2973], ah
        inc    esp
        outsd  dx, dword ptr [esi]
        and    byte ptr [ecx + 0x6f], bh
loc_2_0436:
        jne    loc_2_0458
        ja     0x49b
        outsb  dx, byte ptr [esi]
        je     loc_2_045D
        je     0x4ae
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     0x4ae
        outsb  dx, byte ptr [esi]
        jne    0x4ad
        and    byte ptr [edx + 0x65], dh
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jbe    0x4b8
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + 0x72], ch
        and    byte ptr [ecx + ebp*2 + 0x73], ah
loc_2_0458:
        popal  
        bound  ebp, qword ptr [ecx + ebp*2 + 0x6e]
loc_2_045D:
        and    byte ptr [si + 0x68], dh
        imul   esi, dword ptr [ebx + 0x20], 0x69766564
        arpl   word ptr [ebp + 0x3f], sp
        add    byte ptr [edx + 0x65], dl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jbe    0x4db
        outsb  dx, byte ptr [esi]
        and    byte ptr [si + 0x65], al
        jbe    loc_2_04E2
        arpl   word ptr [ebp + 0x20], sp
        dec    ebp
        popal  
        jns    0x4a0
        inc    ecx
        arpl   word ptr gs:[eax + 0x50], si
        jb     0x4f9
        jb     0x4ee
        insd   dword ptr es:[edi], dx
        jae    loc_2_0490
loc_2_0490:
        daa    
        jne    0x501
        imul   ebp, dword ptr [esi + 0x6f], 0x77
        outsb  dx, byte ptr [esi]
        daa    
        add    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     loc_2_0514
        and    byte ptr [ebx + 0x6f], ah
        jne    0x512
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_2_04CC
        arpl   word ptr [eax + 0x61], bp
        outsb  dx, byte ptr [esi]
        and    byte ptr gs:[si + 0x68], dh
        and    byte ptr gs:[ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     0x52b
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [edx + 0x65], ah
        arpl   word ptr [ecx + 0x75], sp
        jae    0x531
loc_2_04CC:
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x547
        and    byte ptr [ebx + 0x6f], ah
        jne    loc_2_0545
        and    byte ptr fs:[esi + 0x6f], ch
        je     0x4ff
        jb     loc_2_0546
        insd   dword ptr es:[edi], dx
loc_2_04E2:
        outsd  dx, dword ptr [esi]
        jbe    loc_2_054A
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ecx + ebp*2 + 0x73], ah
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        popal  
        and    byte ptr [ebp + 0x76], ah
        imul   esp, dword ptr [ebx + 0x65], 0x6f54202e
        and    byte ptr [ebp + 0x6e], dh
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x20], bp
        jns    0x576
        jne    0x57b
        and    byte ptr [ecx + 0x70], ch
        je     0x57e
        jo     0x531
        arpl   word ptr [edi + 0x6d], bp
loc_2_0514:
        jo     loc_2_058B
        je     loc_2_057D
        jb     loc_2_0546
        and    byte ptr [ebx + 0x68], dh
        jne    0x593
        and    byte ptr [edi + ebp*2 + 0x77], ah
        outsb  dx, byte ptr [esi]
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x59f
        sub    al, 0x20
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x6e], dh
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x20], bp
        jns    loc_2_05AF
        jne    loc_2_05B4
        and    byte ptr [ebx + 0x6f], ah
loc_2_0545:
        insd   dword ptr es:[edi], dx
loc_2_0546:
        jo     0x5bd
        je     loc_2_05AF
loc_2_054A:
        jb     0x57a
        add    byte ptr [edi + ebp*2 + 0x63], al
        imul   esp, dword ptr [eax], 0x2f
        and    byte ptr [ebp + 0x6e], dl
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x20], bp
        inc    esi
        popal  
        imul   ebp, dword ptr [ebp + 0x64], 0x6e695700
        outsd  dx, dword ptr fs:[esi]
        ja     0x5dc
        and    byte ptr [ebx + 0x6f], ah
        jne    0x5da
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_2_0594
        arpl   word ptr [eax + 0x61], bp
        outsb  dx, byte ptr [esi]
        and    byte ptr gs:[si + 0x68], dh
loc_2_057D:
        and    byte ptr gs:[ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     0x5f3
        outsd  dx, dword ptr [esi]
loc_2_058B:
        outsb  dx, byte ptr [esi]
        and    byte ptr [edx + 0x65], ah
        arpl   word ptr [ecx + 0x75], sp
        jae    0x5f9
loc_2_0594:
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [eax + 0x6f], dh
        jb     0x611
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        outsd  dx, dword ptr [esi]
        jb     0x5c5
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x69], bp
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + di + 0x74], dh
loc_2_05AF:
        popal  
        je     0x61b
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_2_05B4:
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x65687420
        and    byte ptr [edx + 0x65], dh
        jno    0x639
        jae    0x63b
        and    byte ptr cs:[edi + ebp*2 + 0x20], dl
        jne    loc_2_063C
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x20], bp
        jns    0x644
        jne    0x649
        and    byte ptr [ecx + 0x70], ch
        je     loc_2_064C
        jo     0x5ff
        arpl   word ptr [edi + 0x6d], bp
        jo     loc_2_0659
        je     loc_2_064B
        jb     loc_2_0614
        and    byte ptr [ebx + 0x68], dh
        jne    0x661
        and    byte ptr [edi + ebp*2 + 0x77], ah
        outsb  dx, byte ptr [esi]
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     loc_2_066D
        sub    al, 0x20
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x6e], dh
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x20], bp
        jns    0x67d
        jne    loc_2_0682
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
loc_2_0614:
        jo     0x68b
        je     0x67d
        jb     loc_2_0648
        add    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x695
        and    byte ptr [eax + 0x61], cl
        jae    0x647
        dec    esi
        outsd  dx, dword ptr [esi]
        je     loc_2_064B
        inc    esi
        imul   ebp, dword ptr [esi + 0x69], 0x64656873
        and    byte ptr [ebp + 0x74], al
        arpl   word ptr gs:[ecx + ebp*2 + 0x6f], si
loc_2_063C:
        outsb  dx, byte ptr [esi]
        add    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x6b8
        and    byte ptr [eax + 0x61], ch
loc_2_0648:
        jae    loc_2_066A
        outsb  dx, byte ptr [esi]
loc_2_064B:
        outsd  dx, dword ptr [esi]
loc_2_064C:
        je     0x66e
        jae    loc_2_06C5
        arpl   word ptr [ebx + 0x65], sp
        jae    loc_2_06C8
        jne    0x6c4
        insb   byte ptr es:[edi], dx
loc_2_0659:
        jns    0x67b
        jb     loc_2_06C2
        jb     0x6d4
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x74], ah
        arpl   word ptr gs:[ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
loc_2_066A:
        and    byte ptr [ecx + 0x6e], ch
loc_2_066D:
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x6f], ah
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        ja     0x6e2
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + si + 0x61], ch
        jb     0x6e4
        ja     loc_2_06E3
loc_2_0682:
        jb     0x6e9
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        imul   bp, word ptr [ebp + 0x73], 0x7320
        imul   ebp, dword ptr [esi + 0x63], 0x68742065
        and    byte ptr gs:[ecx + 0x73], ch
        je     loc_2_06BD
        je     0x708
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[ecx + 0x6f], bh
        jne    loc_2_06C6
        jb     0x709
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x65], dl
        je     loc_2_0723
        jo     loc_2_06EA
        or     cl, byte ptr [edx]
        add    byte ptr [edx], cl
        dec    ecx
        and    byte ptr [ecx + 0x6f], bh
        jne    loc_2_06DB
        jne    0x72b
loc_2_06BD:
        outsd  dx, dword ptr fs:[esi]
        arpl   word ptr [ebx + 0x20], bp
loc_2_06C2:
        bound  esp, qword ptr [ebp + 0x66]
loc_2_06C5:
        outsd  dx, dword ptr [esi]
loc_2_06C6:
        jb     0x72d
loc_2_06C8:
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x743
        and    byte ptr [edx + 0x65], dh
        je     loc_2_073C
        arpl   word ptr [ebx + esi*2 + 0x20], si
loc_2_06DB:
        je     0x745
        and    byte ptr gs:[eax + 0x61], ch
        jb     0x747
loc_2_06E3:
        ja     0x746
        jb     0x74c
        and    byte ptr [ecx + 0x6e], ch
loc_2_06EA:
        and    byte ptr [ecx + 0x6c], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [ecx + 0x6f], bh
        jne    0x768
        and    byte ptr [eax + 0x61], ch
        jb     loc_2_075F
        ja     0x75e
        jb     loc_2_0764
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        imul   bp, word ptr [ebp + 0x73], 0x202c
        jns    loc_2_077B
        jne    loc_2_0780
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     loc_2_0789
        je     loc_2_077B
        jb     loc_2_0738
        insd   dword ptr es:[edi], dx
        popal  
        jns    loc_2_073C
        jae    0x792
        outsd  dx, dword ptr [esi]
        jo     0x741
        jb     loc_2_0788
loc_2_0723:
        jae    0x795
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr fs:[esi + 0x67], 0x6f54202e
        and    byte ptr [eax + 0x61], ch
        jbe    loc_2_0799
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
loc_2_0738:
        outsd  dx, dword ptr fs:[esi]
        ja     0x7af
loc_2_073C:
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x74], ah
        arpl   word ptr gs:[ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ecx + 0x20], ah
        jae    0x7c2
        arpl   word ptr gs:[ecx + 0x66], bp
        imul   esp, dword ptr [ebx + 0x20], 0x64726168
        ja     loc_2_07C0
loc_2_075F:
        jb     loc_2_07C6
        and    byte ptr [eax + 0x72], dh
loc_2_0764:
        outsd  dx, dword ptr [esi]
        imul   bp, word ptr [ebp + 0x2c], 0x7320
        push   0x64207475
        outsd  dx, dword ptr [esi]
        ja     0x7e2
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr fs:[edx + 0x65], dh
loc_2_077B:
        jae    0x7f1
        popal  
        jb     loc_2_07F4
loc_2_0780:
        and    byte ptr [ecx + 0x6f], bh
        jne    0x7f7
        and    byte ptr [ebx + 0x6f], ah
loc_2_0788:
        insd   dword ptr es:[edi], dx
loc_2_0789:
        jo     0x800
        je     0x7f2
        jb     0x7af
        imul   ebp, dword ptr [esi + 0x20], 0x74616874
        and    byte ptr [eax + 0x61], ch
loc_2_0799:
        jb     0x7ff
        ja     0x7fe
        jb     0x804
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        imul   bp, word ptr [ebp + 0x2e], 0x4420
        outsd  dx, dword ptr [esi]
        and    byte ptr [ecx + 0x6f], bh
        jne    loc_2_07D0
        ja     0x813
        outsb  dx, byte ptr [esi]
        je     0x7d5
        je     0x826
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     0x826
        outsb  dx, byte ptr [esi]
        jne    0x825
loc_2_07C0:
        and    byte ptr [edi + 0x69], dh
        je     loc_2_082D
        outsd  dx, dword ptr [esi]
loc_2_07C6:
        jne    0x83c
        and    byte ptr [edx + 0x65], dh
        jae    0x841
        popal  
        jb     0x844
loc_2_07D0:
        imul   ebp, dword ptr [esi + 0x67], 0x5900203f
        outsd  dx, dword ptr [esi]
        jne    0x84c
        and    byte ptr [ebp + 0x75], ch
        insb   byte ptr es:[edi], dx
        je     loc_2_0849
        sub    eax, 0x636e7566
        je     0x850
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x76], ah
        imul   esp, dword ptr [ebx + 0x65], 0x73252820
loc_2_07F4:
        sub    dword ptr [eax], esp
        push   0x73207361
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[ebx + 0x68], ah
        imul   ebp, dword ptr [esp + 0x20], 0x69766564
        arpl   word ptr [ebp + 0x73], sp
        and    byte ptr [ebp + 0x73], dh
        imul   ebp, dword ptr [esi + 0x67], 0x2d323320
        bound  ebp, qword ptr [ecx + 0x74]
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x884
        jb     0x894
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr fs:[edi + 0x74], ch
        push   0x20737265
loc_2_082D:
        jne    0x8a2
        imul   ebp, dword ptr [esi + 0x67], 0x6d6f6320
        jo     0x899
        je     loc_2_08A3
        bound  ebp, qword ptr [ecx + 0x6c]
        imul   esi, dword ptr [ecx + edi*2 + 0x2d], 0x65646f6d
        and    byte ptr [edx + esi*2 + 0x69], ah
loc_2_0849:
        jbe    0x8b0
        jb     loc_2_08C0
        and    byte ptr cs:[eax + ebp*2 + 0x69], dl
        jae    loc_2_0874
        arpl   word ptr [edi + 0x6e], bp
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     loc_2_08C8
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [esi + 0x6f], ch
        je     0x889
        jae    0x8e0
        jo     0x8dd
        outsd  dx, dword ptr [esi]
        jb     loc_2_08E4
        sub    al, 0x20
loc_2_0874:
        jae    0x8e5
        and    byte ptr [ecx + 0x6f], bh
        jne    0x8ed
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     0x8f6
        je     loc_2_08E8
        jb     loc_2_08A5
        push   0x62207361
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [eax + 0x61], ch
        insb   byte ptr es:[edi], dx
        je     0x8f8
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        jo     0x90c
        jbe    loc_2_0902
        outsb  dx, byte ptr [esi]
        je     loc_2_08C0
        arpl   word ptr [edi + 0x72], bp
loc_2_08A3:
        jb     0x91a
loc_2_08A5:
        jo     0x91b
        imul   ebp, dword ptr [edi + 0x6e], 0xd0a0d2e
        or     al, byte ptr [ecx + 0x66]
        je     0x918
        jb     loc_2_08D5
        jns    0x926
        jne    0x8d9
        jb     loc_2_0920
        jae    loc_2_0931
        popal  
        jb     0x934
loc_2_08C0:
        and    byte ptr [ecx + 0x6f], bh
        jne    0x937
        and    byte ptr [ebx + 0x6f], ah
loc_2_08C8:
        insd   dword ptr es:[edi], dx
        jo     0x940
        je     0x932
        jb     loc_2_08FB
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
loc_2_08D5:
        ja     0x94a
        and    byte ptr [edi + 0x69], dh
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x73], dh
        and    byte ptr gs:[ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
loc_2_08E4:
        jo     0x947
        je     0x951
loc_2_08E8:
        bound  ebp, qword ptr [ecx + 0x6c]
        imul   esi, dword ptr [ecx + edi*2 + 0x2d], 0x65646f6d
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x95e
        jb     0x96e
loc_2_08FB:
        and    byte ptr [esi + 0x6f], ah
        jb     loc_2_0920
        popal  
loc_2_0902:
        arpl   word ptr [eax + 0x20], bp
        arpl   word ptr [eax + 0x69], bp
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[ebp + 0x76], ah
        imul   esp, dword ptr [ebx + 0x65], 0x74746120
        popal  
        arpl   word ptr [eax + 0x65], bp
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        je     0x988
loc_2_0920:
        imul   esi, dword ptr [ebx + 0x20], 0x746c756d
        imul   ebp, dword ptr [0x636e7566], 0x6e6f6974
loc_2_0931:
        and    byte ptr [ebp + 0x76], ah
        imul   esp, dword ptr [ebx + 0x65], 0x6649202e
        and    byte ptr [ecx + 0x6f], bh
        jne    loc_2_0961
        ja     0x9a4
        outsb  dx, byte ptr [esi]
        je     0x966
        je     0x9b7
        and    byte ptr [ebp + 0x73], dh
        and    byte ptr gs:[ebx], dh
        xor    ch, byte ptr [0x20746962]
        jb     loc_2_09C0
        jbe    0x9be
        jb     loc_2_09CE
        sub    al, 0x20
        jns    loc_2_09CE
        jne    0x981
loc_2_0961:
        insd   dword ptr es:[edi], dx
        popal  
        jns    loc_2_0985
        bound  esp, qword ptr [ebp + 0x20]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x9de
        and    byte ptr [edi + 0x62], ch
        je     0x9d5
        imul   ebp, dword ptr [esi + 0x20], 0x75206e61
        jo     loc_2_09E1
        popal  
        je     0x9e5
        and    byte ptr fs:[edx + esi*2 + 0x69], ah
loc_2_0985:
        jbe    0x9ec
        jb     0x9a9
        outsw  dx, word ptr [esi]
        jb     0x9ad
        je     0x9f7
        and    byte ptr gs:[ebp + 0x76], ah
        imul   esp, dword ptr [ebx + 0x65], 0x61687420
        je     0x9bd
        arpl   word ptr [ecx + 0x75], sp
        jae    0xa07
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        bound  ebp, qword ptr [ebp + 0x6d]
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     loc_2_0A19
        arpl   word ptr [ecx + ebp*2 + 0x6e], si
        and    byte ptr [bx + di + 0x6f], bh
loc_2_09C0:
        jne    0xa34
        and    byte ptr [eax + 0x61], ch
        jb     0xa2b
        ja     0xa2a
        jb     loc_2_0A30
        and    byte ptr [ebp + 0x61], ch
loc_2_09CE:
        outsb  dx, byte ptr [esi]
        jne    0xa37
        popal  
        arpl   word ptr [ebp + esi*2 + 0x72], si
        jb     loc_2_0A05
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ecx + 0x6f], bh
        jne    0xa01
loc_2_09E1:
        arpl   word ptr [ecx + 0x6e], sp
        and    byte ptr [ecx + ebp*2 + 0x73], ah
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     loc_2_0A57
        and    byte ptr gs:[ebp + 0x76], ah
        imul   esp, dword ptr [ebx + 0x65], 0x6f43002e
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     loc_2_0A6D
        outsd  dx, dword ptr [esi]
loc_2_0A05:
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x61], cl
        outsb  dx, byte ptr [esi]
        popal  
        jb     0xa2f
        arpl   word ptr [ecx + 0x6e], sp
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_2_0A36
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
loc_2_0A19:
        and    byte ptr fs:[edx + 0x65], ah
        arpl   word ptr [ecx + 0x75], sp
        jae    loc_2_0A87
        and    byte ptr [edi + 0x6e], ch
        and    byte ptr gs:[edi + 0x66], ch
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x6f], ah
loc_2_0A30:
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        ja     0xa9e
        outsb  dx, byte ptr [esi]
loc_2_0A36:
        and    byte ptr [di], cl
        or     ah, byte ptr [esi + 0x69]
        insb   byte ptr es:[edi], dx
        jae    0xa60
        imul   esi, dword ptr [ebx + 0x20], 0x68746965
        jb     0xa6a
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_2_0A6E
        jo     loc_2_0AC2
        jae    loc_2_0AB8
        outsb  dx, byte ptr [esi]
        je     loc_2_0A76
        outsd  dx, dword ptr [esi]
loc_2_0A57:
        jb     0xa79
        push   0x61207361
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x6e], ch
        jbe    loc_2_0AC5
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x76], 0x69737265
loc_2_0A6D:
        outsd  dx, dword ptr [esi]
loc_2_0A6E:
        outsb  dx, byte ptr [esi]
        and    byte ptr [esi + 0x75], ch
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
loc_2_0A76:
        cmp    ah, byte ptr [eax]
        or     eax, 0x4d4d560a
        push   esi
        pop    eax
        inc    esp
        sub    al, 0x20
        push   ebx
        dec    eax
        inc    ebp
        dec    esp
loc_2_0A87:
        dec    esp
        push   esi
        pop    eax
        inc    esp
        sub    al, 0x20
        push   esi
        push   esp
        inc    esp
        push   esi
        pop    eax
        inc    esp
        sub    al, 0x20
        push   esi
        pop    eax
        inc    esp
        dec    esp
        inc    esp
        push   edx
        push   esi
        pop    eax
        inc    esp
        sub    al, 0x20
        push   esi
        push   eax
        dec    ecx
        inc    ebx
        inc    esp
        push   esi
        pop    eax
        inc    esp
        or     eax, 0x540a0d0a
        jb     0xb2d
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
loc_2_0AB8:
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr [esi + 0x67], 0x74655320
        jne    0xb32
loc_2_0AC2:
        and    byte ptr [ecx + 0x67], ah
loc_2_0AC5:
        popal  
        db  069h, 06Eh, 02Eh, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x148  R-X
        db  0CDh, 020h, 000h, 000h, 040h, 000h, 072h, 016h, 053h, 033h, 0DBh, 06Ah, 000h, 053h, 0CDh, 020h
        db  04Ch, 000h, 040h, 000h, 083h, 0C4h, 008h, 043h, 083h, 0FBh, 01Ah, 072h, 0EEh, 05Bh, 0C3h
loc_3_001F:
        push   esi
        xor    edx, edx
        mov    esi, dword ptr [esp + 8]
        push   0
        push   esi
        mov    ecx, 0xd
        push   esi
        call   sub_5_1E4B
loc_3_0034:
        push   1
        xor    edx, edx
        push   esi
        mov    ecx, 0xd
        push   esi
        call   sub_5_1E4B
loc_3_0044:
        mov    eax, 1
        pop    esi
        ret    
loc_3_004B:
        push   esi
        xor    edx, edx
        mov    esi, dword ptr [esp + 8]
        push   0
        push   esi
        mov    ecx, 0x12
        push   esi
        call   sub_5_1E4B
loc_3_0060:
        push   1
        xor    edx, edx
        push   esi
        mov    ecx, 0x12
        push   esi
        call   sub_5_1E4B
loc_3_0070:
        mov    eax, 1
        pop    esi
        ret    
loc_3_0077:
        mov    dword ptr [0], 0x80000000     ; -> obj7:0x008C
        test   byte ptr [0], 0x10            ; -> obj7:0x00AC
        je     loc_3_0097
        push   0                             ; -> loc_1_0569
        call   sub_5_D23C
loc_3_0094:
        add    esp, 4
loc_3_0097:
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_3_00AC
        mov    edx, 0                        ; -> loc_3_004B
        xor    ecx, ecx
        call   sub_5_CC30
loc_3_00AC:
        call   0
        mov    edx, 0                        ; -> loc_5_C4B0
        xor    ecx, ecx
        call   sub_5_CC30
loc_3_00BD:
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_3_00ED
        mov    edx, 0                        ; -> loc_5_C554
        xor    ecx, ecx
        call   sub_5_CC30
loc_3_00D2:
        mov    edx, 0                        ; -> loc_5_C575
        mov    ecx, 1
        call   sub_5_CC30
loc_3_00E1:
        mov    edx, 0                        ; -> loc_3_001F
        xor    ecx, ecx
        call   sub_5_CC30
loc_3_00ED:
        or     byte ptr [0], 0x40            ; -> obj7:0x00AC
        ret    
sub_3_00F5:
        xor    eax, eax
        mov    ecx, 0                        ; -> loc_3_0077
        push   eax
        xor    edx, edx
        or     byte ptr [0], 8               ; -> obj7:0x00AC
        push   eax
        push   eax
        call   sub_5_02D3
loc_3_010D:
        mov    eax, 1
        ret    
sub_3_0113:
        or     dword ptr [0], 0x408          ; -> obj7:0x00AC
        xor    eax, eax
        mov    ecx, 0                        ; -> loc_3_0077
        push   eax
        xor    edx, edx
        mov    dword ptr [0], 0xffffffff     ; -> obj7:0x0090
        mov    dword ptr [0], 0x80000000     ; -> obj7:0x0094
        push   eax
        push   eax
        call   sub_5_02D3
loc_3_0142:
        mov    eax, 1
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x6a7  R-X
        db  066h, 08Bh, 002h, 053h, 056h, 057h, 066h, 0BEh, 001h, 000h, 066h, 033h, 0FFh, 066h, 085h, 0C0h
        db  074h, 014h, 00Fh, 0B7h, 0DFh, 06Bh, 0DBh, 00Ch, 066h, 039h, 074h, 013h, 00Ch, 074h, 007h, 066h
        db  047h, 066h, 03Bh, 0F8h, 072h, 0ECh, 066h, 03Bh, 0F8h, 074h, 011h, 066h, 046h, 066h, 083h, 0FEh
        db  009h, 072h, 0D7h, 066h, 0C7h, 001h, 008h, 000h, 05Fh, 05Eh, 05Bh, 0C3h, 066h, 089h, 031h, 0EBh
        db  0F7h
sub_4_0041:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    bx, bx
        push   edi
        cmp    word ptr [eax], bx
        je     loc_4_0089
loc_4_0056:
        movzx  esi, bx
        imul   esi, esi, 0xc
        cmp    dword ptr [esi + eax + 2], ecx
        lea    edi, [esi + eax]
        jne    loc_4_0082
        cmp    dword ptr [edi + 6], edx
        jne    loc_4_0082
        mov    si, word ptr [ebp + 8]
        cmp    word ptr [edi + 0xa], si
        jne    loc_4_0082
        mov    si, word ptr [ebp + 0xc]
        cmp    word ptr [edi + 0xc], si
        je     loc_4_011A
loc_4_0082:
        inc    bx
        cmp    bx, word ptr [eax]
        jb     loc_4_0056
loc_4_0089:
        mov    word ptr [ebp - 2], 0
        xor    bx, bx
        cmp    word ptr [eax], bx
        je     loc_4_00D6
loc_4_0097:
        cmp    word ptr [ebp - 2], bx
        je     loc_4_00B5
        movzx  esi, word ptr [ebp - 2]
        movzx  edi, bx
        imul   esi, esi, 0xc
        imul   edi, edi, 0xc
        lea    esi, [esi + eax + 2]
        lea    edi, [edi + eax + 2]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_4_00B5:
        movzx  edi, word ptr [ebp - 2]
        imul   edi, edi, 0xc
        mov    si, word ptr [ebp + 0xc]
        cmp    word ptr [edi + eax + 0xc], si
        je     loc_4_00C9
        inc    bx
loc_4_00C9:
        inc    word ptr [ebp - 2]
        mov    si, word ptr [ebp - 2]
        cmp    word ptr [eax], si
        ja     loc_4_0097
loc_4_00D6:
        cmp    bx, 9
        jne    loc_4_00DE
        dec    bx
loc_4_00DE:
        movzx  esi, bx
        imul   esi, esi, 0xc
        mov    dword ptr [esi + eax + 2], ecx
        lea    edi, [esi + eax]
        mov    cx, word ptr [ebp + 8]
        mov    dword ptr [edi + 6], edx
        mov    si, word ptr [ebp + 0xc]
        push   0x6e
        inc    bx
        push   eax
        mov    word ptr [edi + 0xa], cx
        push   3
        mov    word ptr [edi + 0xc], si
        push   0
        mov    word ptr [eax], bx
        push   0                             ; -> loc_7_07A0
        push   dword ptr [ebp + 0x14]
        call   sub_5_D0EC
loc_4_0117:
        add    esp, 0x18
loc_4_011A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_4_0123:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x4c
        push   ebx
        mov    dword ptr [ebp - 4], edx
        push   esi
        push   edi
        mov    esi, 0                        ; -> loc_7_07A4
        lea    edi, [ebp - 0x4c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        lea    edx, [ebp - 0xc]
        lea    edi, [ebp - 0xc]
        call   sub_5_30A3
loc_4_0146:
        mov    edx, 0xffffffff
        sub    eax, eax
        mov    ecx, edx
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    ebx, ecx
        mov    esi, edi
        mov    ecx, edx
        lea    edi, [ebp - 0x4c]
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        add    edi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, edx
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        lea    edx, [ebp - 0x4c]
        push   dword ptr [ebp - 4]
        push   1
        push   0
        push   edx
        push   dword ptr [ebp + 8]
        call   sub_5_D0EC
loc_4_0191:
        add    esp, 0x18
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_4_019D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x50
        push   ebx
        push   esi
        xor    si, si
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    bx, word ptr [edi]
        cmp    bx, si
        je     loc_4_0222
loc_4_01B4:
        movzx  eax, si
        imul   eax, eax, 0xc
        mov    edx, dword ptr [ebp + 0x14]
        lea    ecx, [eax + edi]
        cmp    dword ptr [eax + edi + 2], edx
        jne    loc_4_021B
        mov    eax, dword ptr [ebp + 0x18]
        cmp    dword ptr [ecx + 6], eax
        jne    loc_4_021B
        mov    ax, word ptr [ebp + 0x1c]
        cmp    word ptr [ecx + 0xa], ax
        jne    loc_4_021B
        push   dword ptr [ebp + 0x20]
        mov    cx, word ptr [ecx + 0xc]
        lea    edx, [ebp - 0x50]
        push   edx
        push   ecx
        call   sub_5_30B6
loc_4_01E9:
        add    esp, 0xc
        lea    eax, [ebp - 0x50]
        mov    edx, dword ptr [ebp + 0x10]
loc_4_01F2:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    loc_4_0212
        or     cl, cl
        je     loc_4_020E
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    loc_4_0212
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    loc_4_01F2
loc_4_020E:
        xor    eax, eax
        jmp    loc_4_0217
loc_4_0212:
        sbb    eax, eax
        sbb    eax, -1
loc_4_0217:
        test   eax, eax
        je     loc_4_022B
loc_4_021B:
        inc    si
        cmp    si, bx
        jb     loc_4_01B4
loc_4_0222:
        xor    eax, eax
loc_4_0224:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_4_022B:
        movzx  eax, si
        imul   eax, eax, 0xc
        mov    cx, word ptr [eax + edi + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx], cx
        mov    eax, 1
        jmp    loc_4_0224
sub_4_0243:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        mov    ebx, dword ptr [ebp + 0x18]
        push   ebx
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   edi
        push   esi
        call   sub_4_019D
        add    esp, 0x1c
        test   eax, eax
        jne    loc_4_02A6
        mov    edx, edi
        mov    ecx, esi
        call   0
        mov    si, word ptr [esi]
        push   ebx
        push   edi
        mov    edx, dword ptr [ebp + 0x10]
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x14]
        call   sub_4_0041
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_4_0123
        cmp    word ptr [edi], 1
        jbe    loc_4_02A6
        movzx  ecx, si
        mov    edx, 5
        call   sub_5_4572
loc_4_02A6:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
sub_4_02AD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10c
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        lea    edx, [ebp - 0xc]
        call   sub_5_30A3
loc_4_02C3:
        push   5
        lea    eax, [ebp - 0xc]
        push   eax
        push   1
        push   0
        lea    edi, [ebp - 0x10c]
        push   0                             ; -> loc_7_07B4
        push   esi
        mov    esi, 0                        ; -> loc_7_07C4
        call   sub_5_D0EC
loc_4_02E3:
        add    esp, 0x18
        mov    edx, 0xffffffff
        mov    ecx, edx
        sub    eax, eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    edi, [ebp - 0xc]
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    ebx, ecx
        mov    esi, edi
        mov    ecx, edx
        lea    edi, [ebp - 0x10c]
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        add    edi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x10c]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_D164
loc_4_032F:
        add    esp, 0xc
        push   0x80000005
        push   dword ptr [ebp - 4]
        call   sub_5_D15C
loc_4_033F:
        add    esp, 8
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
loc_4_034A:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_0354:
        push   ebp
        mov    edx, dword ptr [0]            ; -> obj7:0x00AC
        mov    ebp, esp
        mov    ecx, edx
        sub    esp, 0x160
        and    ecx, 4
        mov    eax, 1
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    ecx, eax
        sbb    ecx, ecx
        and    edx, 0x1000
        neg    ecx
        shr    edx, 0xc
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [0]            ; -> obj7:0x01D0
        mov    dword ptr [ebp - 0xc], edx
        cmp    ecx, eax
        sbb    ebx, ebx
        add    ebx, eax
        cmp    dword ptr [ebp - 8], esi
        je     loc_4_03A4
        test   ebx, ebx
        jne    loc_4_03A4
        cmp    edx, esi
        jne    loc_4_06A0
loc_4_03A4:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_03BE
        test   ebx, ebx
        je     loc_4_03B8
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_4_069E
loc_4_03B8:
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_0414
loc_4_03BE:
        test   ebx, ebx
        je     loc_4_0414
        mov    eax, dword ptr [ecx + 0xc]
        shr    eax, 0xc
        mov    dword ptr [0], eax            ; -> obj7:0x04A0
        mov    eax, dword ptr [ecx + 8]
        add    eax, 0xfff
        shr    eax, 0xc
        cmp    dword ptr [ebp - 0xc], 0
        mov    dword ptr [0], eax            ; -> obj7:0x04A4
        je     loc_4_0414
        push   1
        push   0x22c
        call   sub_5_D134
loc_4_03EF:
        add    esp, 8
        test   eax, eax
        je     loc_4_0405
        mov    edi, eax
        mov    esi, dword ptr [0]            ; -> obj7:0x01D0
        mov    ecx, 0x8b
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_4_0405:
        mov    dword ptr [0], eax            ; -> obj7:0x01D0
        mov    eax, 1
        jmp    loc_4_06A0
loc_4_0414:
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj7:0x07CC
        push   0x80000002
        call   sub_5_D164
loc_4_0427:
        add    esp, 0xc
        test   eax, eax
        jne    loc_4_069E
        lea    eax, [ebp - 0x14]
        lea    ecx, [ebp - 0x160]
        mov    dword ptr [ebp - 0x14], 0x6e
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> loc_7_07F8
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
loc_4_0455:
        add    esp, 0x18
        test   eax, eax
        jne    loc_4_0462
        cmp    dword ptr [ebp - 0x14], 0x6e
        je     loc_4_0473
loc_4_0462:
        lea    edi, [ebp - 0x160]
        xor    eax, eax
        mov    ecx, 0x1b
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
loc_4_0473:
        test   ebx, ebx
        je     loc_4_0575
        mov    esi, dword ptr [0]            ; -> obj7:0x01D0
        lea    edi, [ebp - 0xf0]
        add    esi, 0x10
        mov    ecx, 0x17
        mov    dword ptr [ebp - 0x18], 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movzx  eax, word ptr [ebp - 0xe6]
        cmp    word ptr [ebp - 0xe6], 1
        sbb    esi, esi
        neg    esi
        test   eax, eax
        je     loc_4_04E2
        cmp    eax, 0xffff
        je     loc_4_0514
        lea    ecx, [ebp - 0x160]
        mov    edx, dword ptr [ebp - 0xec]
        push   dword ptr [ebp - 4]
        push   ecx
        push   dword ptr [ebp - 0xe6]
        push   dword ptr [ebp - 0xe8]
        mov    ecx, dword ptr [ebp - 0xf0]
        call   sub_4_0041
        jmp    loc_4_0611
loc_4_04E2:
        lea    eax, [ebp - 0xe4]
        lea    edx, [ebp - 0x160]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 0xe8]
        push   dword ptr [ebp - 0xec]
        push   dword ptr [ebp - 0xf0]
        push   eax
        lea    ecx, [ebp - 0xe6]
        call   sub_4_0243
        jmp    loc_4_0611
loc_4_0514:
        lea    eax, [ebp - 0x18]
        lea    ecx, [ebp - 0x24]
        push   eax
        xor    edi, edi
        push   ecx
        push   edi
        push   edi
        push   0                             ; -> loc_7_07FC
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
loc_4_052D:
        add    esp, 0x18
        test   eax, eax
        jne    loc_4_0693
        mov    word ptr [ebp - 0xe6], di
        lea    edx, [ebp - 0x24]
        mov    eax, 4
loc_4_0547:
        shl    word ptr [ebp - 0xe6], 4
        mov    cl, byte ptr [edx]
        cmp    cl, 0x39
        movsx  di, cl
        mov    cx, 0x37
        jg     loc_4_0562
        mov    cx, 0x30
loc_4_0562:
        sub    di, cx
        inc    edx
        add    word ptr [ebp - 0xe6], di
        dec    eax
        jne    loc_4_0547
        jmp    loc_4_0611
loc_4_0575:
        xor    eax, eax
        cmp    word ptr [ebp - 0x160], ax
        jne    loc_4_05A4
        push   dword ptr [ebp - 4]
        push   eax
        push   eax
        push   eax
        push   5
        call   sub_5_6710
loc_4_058D:
        add    esp, 4
        lea    edx, [ebp - 0x160]
        lea    ecx, [ebp - 0xe6]
        push   eax
        call   sub_4_0243
        jmp    loc_4_0611
loc_4_05A4:
        cmp    word ptr [ebp - 0x160], 1
        jne    loc_4_05B7
        mov    ax, word ptr [ebp - 0x154]
        jmp    loc_4_060A
loc_4_05B7:
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_05D2
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
loc_4_05C5:
        add    esp, 4
        mov    eax, 1
        jmp    loc_4_06A0
loc_4_05D2:
        xor    eax, eax
        lea    ecx, [ebp - 0x1c]
        push   ecx
        mov    edx, 0                        ; -> loc_7_080C
        push   8
        mov    ecx, 0                        ; -> obj7:0x081C
        mov    dword ptr [ebp - 0x18], 0xffffffff
        push   eax
        mov    dword ptr [ebp - 0x1c], eax
        call   sub_5_1240
loc_4_05F4:
        cmp    eax, 0x21
        jb     loc_4_0693
        cmp    eax, 0x29
        ja     loc_4_0693
        sub    ax, 0x20
loc_4_060A:
        mov    word ptr [ebp - 0xe6], ax
loc_4_0611:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xe6]
        call   sub_4_02AD
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
loc_4_0627:
        add    esp, 4
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_066C
        test   ebx, ebx
        je     loc_4_066C
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_4_066C
        lea    edx, [ebp - 0x10]
        xor    ecx, ecx
        call   sub_5_44B6
loc_4_0644:
        test   byte ptr [ebp - 0x10], 4
        je     loc_4_065C
        test   esi, esi
        jne    loc_4_065C
        and    byte ptr [ebp - 0x10], 0xfb
        mov    edx, dword ptr [ebp - 0x10]
        xor    ecx, ecx
        call   sub_5_4572
loc_4_065C:
        test   dword ptr [ebp - 0x10], 0x80000003
        je     loc_4_066C
        or     byte ptr [0], 0x20            ; -> obj7:0x00AC
loc_4_066C:
        xor    esi, esi
        cmp    dword ptr [ebp - 8], esi
        jne    loc_4_069E
        test   ebx, ebx
        je     loc_4_069E
        cmp    dword ptr [ebp - 0xc], esi
        je     loc_4_069E
        push   esi
        mov    eax, dword ptr [0]            ; -> obj7:0x01D0
        push   eax
        call   sub_5_D0C8
loc_4_0688:
        add    esp, 8
        mov    dword ptr [0], esi            ; -> obj7:0x01D0
        jmp    loc_4_069E
loc_4_0693:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
loc_4_069B:
        add    esp, 4
loc_4_069E:
        xor    eax, eax
loc_4_06A0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0xd242  R-X
CONFIGMG_PM_API:
        mov    ax, 0x3408
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    esi, eax
        movzx  ebx, word ptr [ebp + 0x1c]
        mov    eax, 0x20
        cmp    ebx, 0x6d
        jae    loc_5_007B
        movzx  edx, word ptr [ebx*2]         ; -> loc_7_0C98
        cmp    dx, -1
        je     loc_5_007B
        mov    edi, edx
        shr    edx, 4
        and    edi, 0xf
        mov    ecx, edi
        je     loc_5_0072
        shl    edi, 2
        add    esi, edi
        ror    edx, cl
loc_5_003B:
        mov    eax, dword ptr [esi]
        sub    esi, 4
        rol    edx, 1
        test   edx, 1
        je     loc_5_006F
        or     eax, eax
        je     loc_5_006F
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x3c]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        pop    dword ptr [ebp + 0x3c]
        pop    dword ptr [ebp + 0x14]
loc_5_006F:
        push   eax
        loop   loc_5_003B
loc_5_0072:
        call   dword ptr [ebx*4]             ; -> obj1:0x0000
        add    esp, edi
loc_5_007B:
        mov    edx, dword ptr [ebp + 0x1c]
        and    edx, 0xffff
        mov    dx, ax
        sub    ax, 1
        cmc    
        lahf   
        mov    dword ptr [ebp + 0x1c], edx
        mov    byte ptr [ebp + 0x2c], ah
        ret    
sub_5_0094:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 4]
        and    ebx, 0xffff
        push   eax
        mov    esi, dword ptr [eax]
        mov    eax, 0x20
        cmp    ebx, 0x6d
        jae    loc_5_00DA
        movzx  ecx, word ptr [ebx*2]         ; -> loc_7_0C98
        cmp    cx, -1
        je     loc_5_00DA
        and    ecx, 0xf
        mov    edi, ecx
        shl    edi, 2
        mov    eax, edi
        sub    esp, edi
        mov    edi, esp
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, eax
        call   dword ptr [ebx*4]             ; -> obj1:0x0000
        add    esp, edi
loc_5_00DA:
        pop    ebx
        mov    dword ptr [ebx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_00E2:
        lea    eax, [esp + 4]
        push   eax
        push   0x14
        push   dword ptr [0]                 ; -> obj7:0x0084
        push   0
        VMMCall  VMM._CallRing3                  ; cd2053010100
        add    esp, 0x10
        ret    
loc_5_00FB:
        mov    ebx, dword ptr [esi + 0xc]
        sub    ebx, 0x80000000
        jb     loc_5_019D
        cmp    ebx, 0x6d
        jae    loc_5_019D
        mov    eax, 0x20
        push   esi
        cmp    dword ptr [esi + 0x1c], 4
        jne    loc_5_018B
        cmp    dword ptr [esi + 0x14], 4
        jne    loc_5_018B
        movzx  edi, word ptr [ebx*2]         ; -> loc_7_0C98
        cmp    di, -1
        je     loc_5_018B
        and    edi, 0xf
        shl    edi, 2
        mov    esi, dword ptr [esi + 0x10]
        push   0
        push   0
        push   0
        push   4
        push   esi
        VMMCall  VMM._Assert_Range               ; cd2073010100
        add    esp, 0x14
        or     eax, eax
        mov    eax, 3
        je     loc_5_018B
        mov    esi, dword ptr [esi]
        push   0
        push   0
        push   0
        push   edi
        push   esi
        VMMCall  VMM._Assert_Range               ; cd2073010100
        add    esp, 0x14
        or     eax, eax
        mov    eax, 3
        je     loc_5_018B
        mov    ecx, edi
        mov    eax, edi
        shr    ecx, 2
        sub    esp, edi
        mov    edi, esp
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, eax
        call   dword ptr [ebx*4]             ; -> obj1:0x0000
        add    esp, esi
loc_5_018B:
        pop    esi
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi + 0x20]
        xor    eax, eax
        mov    dword ptr [ecx], 4
        ret    
loc_5_019D:
        add    ebx, 0x80000000
        cmp    ebx, 0
        je     loc_5_01B6
        cmp    ebx, -1
        je     loc_5_01B3
        mov    eax, 0xffffffff
        ret    
loc_5_01B3:
        xor    eax, eax
        ret    
loc_5_01B6:
        xor    eax, eax
        mov    ecx, 0x400
        ret    
loc_5_01BE:
        cmp    dword ptr [0], 0              ; -> obj7:0x0094
        je     loc_5_01D8
        push   dword ptr [esp + 4]
        push   dword ptr [0]                 ; -> obj7:0x01CC
        call   sub_5_D024
        jmp    loc_5_01EA
loc_5_01D8:
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_020F
        mov    ecx, dword ptr [esp + 4]
        jmp    sub_1_050C
loc_5_01EA:
        ret    
sub_5_01EB:
        push   esi
        inc    dword ptr [0]                 ; -> obj7:0x0094
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   dword ptr [esi + 0x10]
        add    esp, 0xc
        mov    dword ptr [esi + 0xc], eax
        dec    dword ptr [0]                 ; -> obj7:0x0094
        pop    esi
        ret    
sub_5_020F:
        push   esi
        push   edi
        mov    edi, ecx
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> obj7:0x0094
        jne    loc_5_02B4
        cmp    dword ptr [0], esi            ; -> obj7:0x0090
        jne    loc_5_0245
        push   9
        push   esi
        push   esi
        push   esi
        push   esi
        push   esi
        push   1
        push   4
        call   sub_5_D068
        add    esp, 0x20
        mov    dword ptr [0], eax            ; -> obj7:0x0090
        cmp    eax, esi
        je     loc_5_02BD
loc_5_0245:
        push   0x4000
        mov    eax, dword ptr [0]            ; -> obj7:0x0090
        add    eax, 0x4000
        push   eax
        push   edi
        push   0                             ; -> sub_5_01EB
        call   sub_5_D060
        add    esp, 0x10
loc_5_0263:
        mov    eax, dword ptr [0]            ; -> obj7:0x01CC
        push   eax
        call   sub_5_D050
        mov    esi, eax
        test   esi, esi
        je     loc_5_0292
        push   1
        push   esi
        push   0                             ; -> loc_5_01BE
        call   sub_1_06DC
loc_5_0281:
        add    esp, 0xc
        mov    eax, dword ptr [0]            ; -> obj7:0x01CC
        push   esi
        push   eax
        call   sub_5_D03C
        jmp    loc_5_0263
loc_5_0292:
        test   byte ptr [0], 4               ; -> obj7:0x00AC
        je     loc_5_02BD
        mov    eax, dword ptr [0]            ; -> obj7:0x0090
        xor    ecx, ecx
        push   ecx
        mov    dword ptr [0], ecx            ; -> obj7:0x0090
        push   eax
        call   sub_5_D034
        add    esp, 8
        jmp    loc_5_02BD
loc_5_02B4:
        push   edi
        call   sub_5_01EB
        add    esp, 4
loc_5_02BD:
        pop    edi
        pop    esi
        ret    
loc_5_02C0:
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_020F
        push   dword ptr [esp + 8]
        call   sub_5_D070
        ret    
sub_5_02D3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    edi, ecx
        call   sub_1_04FD
loc_5_02E5:
        mov    esi, eax
        test   esi, esi
        jne    loc_5_02F3
        mov    eax, dword ptr [ebp + 0x10]
        jmp    loc_5_041F
loc_5_02F3:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x10], edi
        mov    dword ptr [esi], ebx
        lea    edi, [esi + 0xc]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [edi], eax
        call   sub_5_D0B8
        test   al, 1
        je     loc_5_040D
        test   byte ptr [0], 8               ; -> obj7:0x00AC
        jne    loc_5_040D
        test   al, 2
        jne    loc_5_040D
        call   sub_5_D0AC
        cmp    eax, dword ptr [0]            ; -> obj7:0x009C
        je     loc_5_0390
        xor    ebx, ebx
        xor    ecx, ecx
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        mov    dword ptr [ebp - 0xc], ecx
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_0359
        mov    dword ptr [edi], 0x16
        jmp    loc_5_0414
loc_5_0359:
        push   0
        call   sub_5_D09C
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_0414
        push   0
        mov    ecx, 0                        ; -> loc_5_02C0
        push   ebx
        mov    edx, esi
        call   sub_1_051C
loc_5_0379:
        test   eax, eax
        je     loc_5_0385
        push   1
        push   ebx
        call   sub_5_D090
loc_5_0385:
        push   ebx
        call   sub_5_D084
        jmp    loc_5_0414
loc_5_0390:
        mov    eax, dword ptr [0]            ; -> obj7:0x009C
        test   byte ptr [eax], 0x20
        jne    loc_5_03A2
        mov    dword ptr [edi], 0x16
        jmp    loc_5_0414
loc_5_03A2:
        mov    eax, dword ptr [eax + 8]
        mov    ax, word ptr [eax + 0x28]
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ebp - 2]
        lar    eax, eax
        mov    dword ptr [ebp - 0xc], eax
        test   byte ptr [ebp - 0xa], 0x40
        jne    loc_5_03E2
        push   esi
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 8], 1
        push   0x7fff
        push   0x219
        push   eax
        push   0x80000068
        call   sub_5_D07C
        add    esp, 0x14
        jmp    loc_5_0414
loc_5_03E2:
        cmp    dword ptr [0], 0              ; -> obj7:0x0084
        je     loc_5_0414
        push   esi
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 8], 1
        push   0x7fff
        push   0x219
        push   eax
        push   0x68
        call   sub_5_00E2
        add    esp, 0x14
        jmp    loc_5_0414
loc_5_040D:
        mov    ecx, esi
        call   sub_5_020F
loc_5_0414:
        mov    edi, dword ptr [edi]
        mov    ecx, esi
        call   sub_1_050C
loc_5_041D:
        mov    eax, edi
loc_5_041F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_0428:
        xor    eax, eax
        ret    
sub_5_042B:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0]            ; -> obj7:0x00B8
        push   ebp
        xor    edi, edi
        dec    ebx
        mov    eax, dword ptr [0]            ; -> obj7:0x00B4
        cmp    dword ptr [eax - 0x24], ecx
        jae    loc_5_0446
        mov    dword ptr [edx], edi
        jmp    loc_5_048F
loc_5_0446:
        mov    esi, dword ptr [0]            ; -> obj7:0x00B0
        test   ebx, ebx
        je     loc_5_046B
loc_5_0450:
        lea    eax, [edi + ebx]
        shr    eax, 1
        mov    ebp, eax
        imul   ebp, ebp, 0x24
        cmp    dword ptr [ebp + esi], ecx
        jb     loc_5_0464
        mov    ebx, eax
        jmp    loc_5_0467
loc_5_0464:
        lea    edi, [eax + 1]
loc_5_0467:
        cmp    ebx, edi
        jne    loc_5_0450
loc_5_046B:
        imul   edi, edi, 0x24
        lea    eax, [edi + esi]
        mov    esi, dword ptr [eax]
        sub    esi, ecx
        cmp    esi, 1
        sbb    esi, esi
        neg    esi
        mov    dword ptr [edx], esi
        test   esi, esi
        je     loc_5_048F
        cmp    dword ptr [eax - 0x24], ecx
        jne    loc_5_048F
loc_5_0487:
        sub    eax, 0x24
        cmp    dword ptr [eax - 0x24], ecx
        je     loc_5_0487
loc_5_048F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_0494:
        push   ebx
        push   esi
        mov    eax, dword ptr [edx + 0xc]
        push   edi
        push   ebp
        mov    ebx, edx
        mov    edx, dword ptr [ecx + 0x1c]
        mov    edi, ecx
        mov    ebp, dword ptr [eax + 4]
        mov    ecx, ebp
        call   sub_5_1D59
        test   eax, eax
        jne    loc_5_04B4
        xor    eax, eax
        jmp    loc_5_0511
loc_5_04B4:
        mov    esi, dword ptr [edi + 8]
        test   esi, 8
        jne    loc_5_04C6
        mov    eax, 1
        jmp    loc_5_0511
loc_5_04C6:
        cmp    dword ptr [ebx + 0x14], -1
        jne    loc_5_04E8
        mov    ecx, dword ptr [ebp + 0x38]
        test   ecx, ecx
        je     loc_5_04E1
        mov    edx, dword ptr [ebx + 4]
        call   sub_5_184D
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_04E8
loc_5_04E1:
        mov    eax, 1
        jmp    loc_5_0511
loc_5_04E8:
        test   esi, 0x10
        je     loc_5_04F5
        add    ebx, 0x1c
        jmp    loc_5_04FB
loc_5_04F5:
        mov    eax, dword ptr [ebx + 0xc]
        mov    ebx, dword ptr [eax + 4]
loc_5_04FB:
        push   ebx
        mov    edx, 7
        mov    ecx, edi
        call   sub_5_0B16
        sub    eax, 0x13
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_5_0511:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_0516:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    eax, dword ptr [ecx + 0xc]
        push   ebp
        mov    edi, ecx
        mov    ecx, dword ptr [eax + 4]
        call   sub_5_1D31
        mov    ebp, eax
        lea    edx, [esp + 0x10]
        mov    ecx, dword ptr [edi + 4]
        call   sub_5_042B
        mov    esi, eax
        cmp    dword ptr [esp + 0x10], ebx
        je     loc_5_0580
loc_5_0542:
        mov    eax, dword ptr [esi + 0x10]
        test   eax, eax
        je     loc_5_057E
        cmp    eax, ebp
        jae    loc_5_0555
        test   byte ptr [esi + 8], 2
        je     loc_5_055D
        cmp    eax, ebp
loc_5_0555:
        jne    loc_5_056A
        test   byte ptr [esi + 8], 2
        je     loc_5_056A
loc_5_055D:
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_0494
        test   eax, eax
        jne    loc_5_057E
loc_5_056A:
        add    esi, 0x24
        cmp    esi, dword ptr [0]            ; -> obj7:0x00B4
        je     loc_5_0580
        mov    eax, dword ptr [esi]
        cmp    dword ptr [edi + 4], eax
        je     loc_5_0542
        jmp    loc_5_0580
loc_5_057E:
        mov    ebx, esi
loc_5_0580:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_5_058A:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        test   edi, edi
        je     loc_5_05BA
loc_5_0595:
        mov    ebx, dword ptr [edi + 0x10]
        test   ebx, ebx
        je     loc_5_05B4
loc_5_059C:
        test   byte ptr [ebx + 5], 0x80
        jne    loc_5_05AD
        mov    ecx, ebx
        call   sub_5_0516
        test   eax, eax
        je     loc_5_05C3
loc_5_05AD:
        mov    ebx, dword ptr [ebx + 0x10]
        test   ebx, ebx
        jne    loc_5_059C
loc_5_05B4:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_5_0595
loc_5_05BA:
        mov    eax, 1
loc_5_05BF:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_5_05C3:
        mov    edx, 5
        mov    ecx, esi
        call   sub_5_731F
        xor    eax, eax
        jmp    loc_5_05BF
sub_5_05D3:
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, dword ptr [0]            ; -> obj7:0x00B0
        add    edi, 0x24
        cmp    edi, dword ptr [0]            ; -> obj7:0x00B4
        je     loc_5_0625
        mov    ebx, 0x100000
loc_5_05F1:
        push   ebx
        mov    ecx, dword ptr [edi + 0x1c]
        push   0
        push   ebx
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_061A
        lea    ecx, [esp + 0xc]
        push   esi
        mov    edx, dword ptr [edi]
        call   sub_5_08C9
        mov    ecx, eax
        call   sub_5_0516
        cmp    eax, edi
        je     loc_5_0631
loc_5_061A:
        add    edi, 0x24
        cmp    edi, dword ptr [0]            ; -> obj7:0x00B4
        jne    loc_5_05F1
loc_5_0625:
        mov    eax, 1
loc_5_062A:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_5_0631:
        xor    eax, eax
        jmp    loc_5_062A
sub_5_0635:
        mov    eax, dword ptr [0]            ; -> obj7:0x00B0
        mov    edx, dword ptr [0]            ; -> obj7:0x00B4
        add    eax, 0x24
        cmp    edx, eax
        je     loc_5_0653
loc_5_0647:
        cmp    dword ptr [eax + 4], ecx
        je     loc_5_0655
        add    eax, 0x24
        cmp    edx, eax
        jne    loc_5_0647
loc_5_0653:
        xor    eax, eax
loc_5_0655:
        ret    
sub_5_0656:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        mov    dword ptr [ebp - 4], edx
        push   edi
        mov    dword ptr [ebp - 0xc], ecx
        test   byte ptr [ebp + 0x14], 1
        je     loc_5_0676
        xor    edi, edi
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        mov    dword ptr [ebp + 0x10], eax
        jmp    loc_5_0680
loc_5_0676:
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_5_1D31
        mov    edi, eax
loc_5_0680:
        call   sub_5_675F
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_5_06A9
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_5_069F
        mov    edx, 3
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_5_731F
loc_5_069F:
        mov    eax, 2
        jmp    loc_5_07D2
loc_5_06A9:
        cmp    dword ptr [0], 0              ; -> obj7:0x00BC
        je     loc_5_06BA
        dec    dword ptr [0]                 ; -> obj7:0x00BC
        jmp    loc_5_070D
loc_5_06BA:
        mov    eax, dword ptr [0]            ; -> obj7:0x00B8
        push   0x201
        inc    eax
        imul   eax, eax, 0x24
        push   eax
        push   dword ptr [0]                 ; -> obj7:0x00B0
        call   sub_5_D0C0
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_06F8
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_5_06EE
        mov    edx, 3
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_5_731F
loc_5_06EE:
        mov    eax, 2
        jmp    loc_5_07D2
loc_5_06F8:
        mov    ecx, dword ptr [0]            ; -> obj7:0x00B8
        mov    dword ptr [0], eax            ; -> obj7:0x00B0
        imul   ecx, ecx, 0x24
        add    eax, ecx
        mov    dword ptr [0], eax            ; -> obj7:0x00B4
loc_5_070D:
        lea    edx, [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_042B
        cmp    dword ptr [ebp - 0x10], 0
        mov    esi, eax
        je     loc_5_0737
loc_5_0720:
        cmp    dword ptr [esi + 0x10], edi
        jbe    loc_5_0790
        add    esi, 0x24
        cmp    esi, dword ptr [0]            ; -> obj7:0x00B4
        je     loc_5_0737
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi], eax
        je     loc_5_0720
loc_5_0737:
        mov    eax, dword ptr [0]            ; -> obj7:0x00B4
        mov    ecx, 0x24
        sub    eax, esi
        cdq    
        idiv   ecx
        imul   eax, ecx
        push   eax
        lea    ecx, [esi + 0x24]
        mov    edx, esi
        call   sub_5_5B5D
        add    dword ptr [0], 0x24           ; -> obj7:0x00B4
        mov    dword ptr [esi + 4], 0
        inc    dword ptr [0]                 ; -> obj7:0x00B8
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_07AA
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C0
loc_5_0774:
        test   ecx, ecx
        je     loc_5_078D
        mov    dword ptr [0], ecx            ; -> obj7:0x00C0
        call   sub_5_0635
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C0
        test   eax, eax
        je     loc_5_079B
loc_5_078D:
        inc    ecx
        jmp    loc_5_0774
loc_5_0790:
        test   edi, edi
        jne    loc_5_0737
        mov    eax, 8
        jmp    loc_5_07D2
loc_5_079B:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [eax], ecx
        inc    ecx
        mov    dword ptr [0], ecx            ; -> obj7:0x00C0
loc_5_07AA:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 8], ecx
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x10], edi
        mov    dword ptr [esi + 0x14], edx
        mov    dword ptr [esi + 0x18], eax
        mov    edx, dword ptr [ebp - 8]
        xor    eax, eax
        mov    dword ptr [esi + 0x1c], ecx
        mov    dword ptr [esi + 0x20], edx
loc_5_07D2:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_5_07DA:
        push   esi
        mov    edx, 5
        push   edi
        push   0
        mov    edi, ecx
        call   sub_5_0B16
        mov    esi, dword ptr [edi + 0x20]
        mov    ecx, esi
        call   sub_5_684A
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_0808
        push   0
        push   esi
        call   sub_5_D0C8
        add    esp, 8
loc_5_0808:
        mov    eax, dword ptr [0]            ; -> obj7:0x00B4
        mov    ecx, 0x24
        sub    eax, edi
        cdq    
        idiv   ecx
        imul   eax, ecx
        sub    eax, 0x24
        lea    edx, [edi + 0x24]
        push   eax
        mov    ecx, edi
        call   sub_5_5B5D
        pop    edi
        dec    dword ptr [0]                 ; -> obj7:0x00B8
        pop    esi
        inc    dword ptr [0]                 ; -> obj7:0x00BC
        sub    dword ptr [0], 0x24           ; -> obj7:0x00B4
        ret    
sub_5_083E:
        call   sub_5_0635
        test   eax, eax
        jne    loc_5_084B
        xor    eax, eax
        jmp    loc_5_0857
loc_5_084B:
        mov    ecx, eax
        call   sub_5_07DA
        mov    eax, 1
loc_5_0857:
        ret    
sub_5_0858:
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, dword ptr [0]            ; -> obj7:0x00B0
        add    edi, 0x24
        cmp    edi, dword ptr [0]            ; -> obj7:0x00B4
        je     loc_5_0887
loc_5_086D:
        cmp    dword ptr [edi + 0x1c], esi
        jne    loc_5_087C
        mov    ecx, edi
        sub    edi, 0x24
        call   sub_5_07DA
loc_5_087C:
        add    edi, 0x24
        cmp    edi, dword ptr [0]            ; -> obj7:0x00B4
        jne    loc_5_086D
loc_5_0887:
        pop    edi
        pop    esi
        ret    
sub_5_088A:
        push   esi
        mov    esi, dword ptr [0]            ; -> obj7:0x00B0
        add    esi, 0x24
        cmp    dword ptr [0], esi            ; -> obj7:0x00B4
        je     loc_5_08C7
loc_5_089C:
        mov    eax, dword ptr [esi + 8]
        test   eax, 0x20000000
        je     loc_5_08BC
        and    eax, 0xdfffffff
        push   0
        mov    edx, 9
        mov    ecx, esi
        mov    dword ptr [esi + 8], eax
        call   sub_5_0B16
loc_5_08BC:
        add    esi, 0x24
        cmp    dword ptr [0], esi            ; -> obj7:0x00B4
        jne    loc_5_089C
loc_5_08C7:
        pop    esi
        ret    
sub_5_08C9:
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, ecx
        mov    edi, esi
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        mov    ecx, dword ptr [esp + 0xc]
        lea    eax, [esi + 0x1c]
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi + 0xc], eax
        mov    eax, esi
        mov    dword ptr [esi + 0x20], ecx
        pop    edi
        mov    dword ptr [esi + 0x2c], esi
        mov    dword ptr [esi + 0x14], 0xffffffff
        mov    dword ptr [esi + 0x18], 0x4c52574d
        pop    esi
        ret    4
sub_5_0900:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        test   eax, eax
        push   edi
        mov    esi, edx
        push   ebp
        je     loc_5_0959
        mov    edi, dword ptr [eax + 0x10]
        test   edi, edi
        je     loc_5_0959
loc_5_0915:
        test   byte ptr [edi + 5], 0x80
        jne    loc_5_0952
        mov    ecx, edi
        call   sub_5_0516
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_0952
        mov    ebp, dword ptr [ebx + 0x20]
        mov    eax, dword ptr [ebp]
        test   eax, eax
        je     loc_5_0937
        cmp    dword ptr [eax + 8], esi
        je     loc_5_0952
loc_5_0937:
        mov    edx, esi
        mov    ecx, ebp
        call   sub_5_6791
        test   eax, eax
        je     loc_5_0965
        mov    ecx, dword ptr [ebp]
        mov    eax, dword ptr [esp + 0x18]
        or     byte ptr [ebx + 0xb], 0x40
        mov    dword ptr [ecx + 0xc], eax
loc_5_0952:
        mov    edi, dword ptr [edi + 0x10]
        test   edi, edi
        jne    loc_5_0915
loc_5_0959:
        mov    eax, 1
loc_5_095E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_5_0965:
        xor    eax, eax
        jmp    loc_5_095E
sub_5_0969:
        push   esi
        push   edi
        push   edx
        mov    edi, edx
        mov    edx, ecx
        mov    esi, ecx
        xor    ecx, ecx
        push   dword ptr [esi + 0x38]
        call   sub_5_0900
        test   eax, eax
        mov    eax, 0
        je     loc_5_0999
        push   edi
        mov    edx, esi
        push   edi
        mov    ecx, 1
        call   sub_5_0900
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_5_0999:
        pop    edi
        pop    esi
        ret    
sub_5_099C:
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, ecx
        mov    edi, dword ptr [0]            ; -> obj7:0x00B0
        add    edi, 0x24
        cmp    dword ptr [0], edi            ; -> obj7:0x00B4
        je     loc_5_09D8
loc_5_09B3:
        test   byte ptr [edi + 0xb], 0x40
        je     loc_5_09CD
        mov    edx, esi
        mov    ecx, edi
        push   dword ptr [edi + 0x20]
        call   sub_5_0B16
        test   eax, eax
        jne    loc_5_09D8
        or     byte ptr [edi + 0xb], 0x80
loc_5_09CD:
        add    edi, 0x24
        cmp    dword ptr [0], edi            ; -> obj7:0x00B4
        jne    loc_5_09B3
loc_5_09D8:
        pop    edi
        pop    esi
        ret    
sub_5_09DB:
        push   esi
        mov    esi, dword ptr [0]            ; -> obj7:0x00B0
        add    esi, 0x24
        cmp    esi, dword ptr [0]            ; -> obj7:0x00B4
        je     loc_5_0A30
loc_5_09ED:
        lea    ecx, [esi + 8]
        mov    eax, dword ptr [ecx]
        test   eax, 0x40000000
        je     loc_5_0A25
        and    eax, 0xbfffffff
        mov    dword ptr [ecx], eax
        test   eax, 0x80000000
        je     loc_5_0A1D
        and    eax, 0x7fffffff
        mov    edx, 3
        push   dword ptr [esi + 0x20]
        mov    dword ptr [ecx], eax
        mov    ecx, esi
        call   sub_5_0B16
loc_5_0A1D:
        mov    ecx, dword ptr [esi + 0x20]
        call   sub_5_684A
loc_5_0A25:
        add    esi, 0x24
        cmp    esi, dword ptr [0]            ; -> obj7:0x00B4
        jne    loc_5_09ED
loc_5_0A30:
        pop    esi
        ret    
sub_5_0A32:
        push   esi
        mov    esi, dword ptr [0]            ; -> obj7:0x00B4
        sub    esi, 0x24
        cmp    dword ptr [0], esi            ; -> obj7:0x00B0
        je     loc_5_0A87
loc_5_0A44:
        lea    ecx, [esi + 8]
        mov    eax, dword ptr [ecx]
        test   eax, 0x40000000
        je     loc_5_0A7C
        and    eax, 0xbfffffff
        mov    edx, 2
        push   dword ptr [esi + 0x20]
        mov    dword ptr [ecx], eax
        and    eax, 0x7fffffff
        mov    dword ptr [ecx], eax
        or     eax, 0x20000000
        mov    dword ptr [ecx], eax
        mov    ecx, esi
        call   sub_5_0B16
        mov    ecx, dword ptr [esi + 0x20]
        call   sub_5_684A
loc_5_0A7C:
        sub    esi, 0x24
        cmp    dword ptr [0], esi            ; -> obj7:0x00B0
        jne    loc_5_0A44
loc_5_0A87:
        pop    esi
        ret    
sub_5_0A89:
        sub    esp, 0xc
        mov    eax, dword ptr [ecx + 0x20]
        mov    dword ptr [esp], ecx
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, edx
        mov    edi, dword ptr [eax]
        test   edi, edi
        je     loc_5_0B0E
loc_5_0A9F:
        mov    eax, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 0xc]
        lea    ebx, [esp + 0x14]
        mov    dword ptr [esp + 0x18], edx
        mov    ecx, dword ptr [eax + 0x38]
        mov    dword ptr [esp + 0x14], ecx
loc_5_0AB4:
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     loc_5_0AFD
        mov    ebp, dword ptr [eax + 0x10]
        test   ebp, ebp
        je     loc_5_0AFD
loc_5_0AC1:
        mov    ecx, ebp
        call   sub_5_0516
        cmp    eax, dword ptr [esp + 0x10]
        je     loc_5_0AF6
        test   esi, esi
        je     loc_5_0AE6
        mov    eax, dword ptr [ebp + 4]
        test   ah, 0x80
        jne    loc_5_0AE6
        or     ah, 0x80
        or     byte ptr [ebp], 1
        mov    dword ptr [ebp + 4], eax
        jmp    loc_5_0AF6
loc_5_0AE6:
        mov    eax, dword ptr [ebp]
        test   al, 1
        je     loc_5_0AF6
        and    byte ptr [ebp + 5], 0x7f
        and    al, 0xfe
        mov    dword ptr [ebp], eax
loc_5_0AF6:
        mov    ebp, dword ptr [ebp + 0x10]
        test   ebp, ebp
        jne    loc_5_0AC1
loc_5_0AFD:
        add    ebx, 4
        lea    eax, [esp + 0x1c]
        cmp    ebx, eax
        jb     loc_5_0AB4
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_5_0A9F
loc_5_0B0E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_5_0B16:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        test   byte ptr [esi + 8], 8
        jne    loc_5_0B39
        cmp    edi, 1
        jbe    loc_5_0B2D
        cmp    edi, 6
        jne    loc_5_0B39
loc_5_0B2D:
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_0A89
loc_5_0B39:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esi + 0x1c]
        push   dword ptr [esi + 0x18]
        push   edi
        call   dword ptr [esi + 0x14]
        add    esp, 0x10
        mov    ebx, eax
        test   byte ptr [esi + 8], 8
        jne    loc_5_0B65
        cmp    edi, 1
        jbe    loc_5_0B5C
        cmp    edi, 6
        jne    loc_5_0B65
loc_5_0B5C:
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_0A89
loc_5_0B65:
        cmp    ebx, 1
        jne    loc_5_0B91
        cmp    edi, 3
        jbe    loc_5_0B88
        cmp    edi, 4
        je     loc_5_0B8C
        cmp    edi, 5
        jb     loc_5_0B91
        cmp    edi, 7
        jbe    loc_5_0B88
        cmp    edi, 8
        je     loc_5_0B8C
        cmp    edi, 9
        jne    loc_5_0B91
loc_5_0B88:
        xor    ebx, ebx
        jmp    loc_5_0B91
loc_5_0B8C:
        mov    ebx, 0x13
loc_5_0B91:
        test   ebx, ebx
        je     loc_5_0BA4
        cmp    ebx, 2
        je     loc_5_0BA4
        cmp    ebx, 0x13
        je     loc_5_0BA4
        mov    ebx, 0x13
loc_5_0BA4:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_5_0BAC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x314
        mov    eax, 0x20000
        push   ebx
        mov    byte ptr [ebp - 1], 1
        push   esi
        push   edi
        mov    esi, ecx
        push   0
        push   eax
        push   0
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_0D02
        mov    edi, dword ptr [esi + 0x20]
        lea    eax, [ebp - 0x14]
        push   eax
        xor    edx, edx
        mov    ecx, edi
        call   sub_5_8D6E
        test   byte ptr [ebp - 0x14], 0x80
        je     loc_5_0D02
        xor    ebx, ebx
        mov    esi, dword ptr [edi + 0x24]
        mov    dword ptr [ebp - 0xc], 0
        cmp    esi, ebx
        je     loc_5_0C2C
loc_5_0C00:
        push   0
        mov    ecx, esi
        push   8
        push   0
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_0C16
        inc    ebx
        jmp    loc_5_0C25
loc_5_0C16:
        mov    eax, dword ptr [esi + 0xc]
        test   eax, eax
        je     loc_5_0C25
        cmp    eax, 0x1d
        je     loc_5_0C25
        inc    dword ptr [ebp - 0xc]
loc_5_0C25:
        mov    esi, dword ptr [esi + 0x28]
        test   esi, esi
        jne    loc_5_0C00
loc_5_0C2C:
        xor    esi, esi
        cmp    dword ptr [ebp - 0xc], esi
        je     loc_5_0D02
        test   ebx, ebx
        je     loc_5_0D02
        push   0xe
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 0x114]
        push   eax
        push   ecx
        call   sub_5_D0FC
        add    esp, 8
        lea    ecx, [ebp - 0x10]
        mov    dword ptr [ebp - 0x10], 0x100
        push   esi
        lea    edx, [ebp - 0x114]
        push   ecx
        push   edx
        push   1
        xor    edx, edx
        push   0                             ; -> obj7:0x0004
        mov    ecx, edi
        call   sub_5_87D6
        lea    ecx, [ebp - 0x114]
        push   ecx
        push   0x15
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 0x314]
        push   eax
        push   ecx
        call   sub_5_D0FC
        add    esp, 0xc
        lea    ecx, [ebp - 8]
        push   ecx
        push   1
        push   0                             ; -> obj7:0x0010
        push   2
        call   sub_5_D0F4
        add    esp, 0x10
        test   eax, eax
        jne    loc_5_0CE7
        push   1
        lea    eax, [ebp - 1]
        push   eax
        push   3
        push   esi
        push   0                             ; -> obj7:0x0014
        push   dword ptr [ebp - 8]
        call   sub_5_D0EC
        add    esp, 0x18
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        push   0x5350574d
        call   sub_5_D0DC
        add    esp, 4
loc_5_0CE7:
        push   1
        lea    edx, [ebp - 0x314]
        xor    ecx, ecx
        call   sub_5_5EA8
        push   0
        push   0                             ; -> obj7:0x001C
        call   sub_5_D0D0
loc_5_0D02:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0D09:
        push   1
        push   edx
        push   9
        push   edx
        push   3
        mov    edx, ecx
        mov    ecx, 1
        call   sub_5_CEA8
        ret    
sub_5_0D1E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    eax, dword ptr [ecx + 0x3c]
        mov    esi, ecx
        test   eax, eax
        je     loc_5_0DB8
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        je     loc_5_0DB8
loc_5_0D39:
        cmp    dword ptr [eax + 4], 1
        jne    loc_5_0D45
        test   byte ptr [eax + 0x28], 1
        je     loc_5_0D4E
loc_5_0D45:
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        jne    loc_5_0D39
        jmp    loc_5_0DB8
loc_5_0D4E:
        lea    eax, [ebp - 4]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [ebp - 3], 1
        jne    loc_5_0DB8
        cmp    dword ptr [0], 0              ; -> obj7:0x008C
        je     loc_5_0D71
        mov    eax, 1
        jmp    loc_5_0DBA
loc_5_0D71:
        lea    eax, [ebp - 8]
        mov    edx, 0                        ; -> obj7:0x0020
        push   eax
        mov    ecx, 0                        ; -> obj7:0x0030
        push   4
        mov    dword ptr [ebp - 8], esi
        push   0
        call   sub_5_1240
        cmp    eax, 0x20
        jne    loc_5_0D97
        mov    eax, 1
        jmp    loc_5_0DBA
loc_5_0D97:
        cmp    eax, 0x22
        jne    loc_5_0DB8
        push   0
        lea    eax, [ebp - 4]
        push   4
        xor    edx, edx
        push   eax
        mov    ecx, esi
        or     byte ptr [ebp - 3], 1
        push   3
        push   0                             ; -> obj7:0x0040
        call   sub_5_886D
loc_5_0DB8:
        xor    eax, eax
loc_5_0DBA:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_0DBF:
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0xc]
        push   esi
        mov    edx, edi
        push   0                             ; -> loc_5_0DBF
        mov    ecx, 2
        call   sub_5_1A48
        push   0x8000000
        mov    ecx, edi
        push   esi
        call   sub_5_1923
        push   esi
        mov    ecx, edi
        push   8
        push   esi
        push   8
        call   sub_5_18E3
        test   eax, eax
        mov    eax, 1
        je     loc_5_0E25
        xor    esi, esi
        mov    edx, 0x8003
        push   esi
        mov    ecx, edi
        call   sub_5_6048
        push   esi
        mov    eax, dword ptr [0]            ; -> obj7:0x0000
        push   esi
        mov    edx, edi
        push   -1
        xor    ecx, ecx
        push   eax
        push   0xa
        call   sub_5_CEA8
        mov    eax, 1
loc_5_0E25:
        pop    edi
        pop    esi
        ret    
sub_5_0E28:
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0xc]
        push   esi
        mov    edx, edi
        push   0                             ; -> sub_5_0E28
        mov    ecx, 3
        call   sub_5_1A48
        push   0x8000000
        mov    ecx, edi
        push   esi
        call   sub_5_1923
        push   esi
        mov    ecx, edi
        push   8
        push   esi
        push   8
        call   sub_5_18E3
        test   eax, eax
        mov    eax, 1
        je     loc_5_0E8E
        xor    esi, esi
        mov    edx, 0x8002
        push   esi
        mov    ecx, edi
        call   sub_5_6048
        push   esi
        mov    eax, dword ptr [0]            ; -> obj7:0x0000
        push   esi
        mov    edx, edi
        push   -1
        xor    ecx, ecx
        push   eax
        push   9
        call   sub_5_CEA8
        mov    eax, 1
loc_5_0E8E:
        pop    edi
        pop    esi
        ret    
sub_5_0E91:
        push   esi
        push   ecx
        mov    esi, dword ptr [0]            ; -> obj7:0x0000
        mov    dword ptr [0], 0              ; -> obj7:0x0000
        call   sub_5_0E28
        add    esp, 4
        mov    dword ptr [0], esi            ; -> obj7:0x0000
        pop    esi
        ret    
loc_5_0EB3:
        push   esi
        mov    eax, 0x8000000
        push   edi
        push   eax
        mov    edi, dword ptr [esp + 0x10]
        push   0
        push   eax
        mov    ecx, edi
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_0ED9
        mov    eax, 1
        jmp    loc_5_0F8F
loc_5_0ED9:
        push   0                             ; -> sub_5_0E28
        mov    edx, edi
        push   0                             ; -> loc_5_0EB3
        mov    ecx, 2
        call   sub_5_1A48
        test   eax, eax
        je     loc_5_0F8D
        push   0
        mov    ecx, edi
        push   8
        push   0
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_0F3A
        mov    ecx, edi
        inc    dword ptr [0]                 ; -> obj7:0x008C
        call   sub_5_0D1E
        cmp    eax, 1
        sbb    esi, esi
        dec    dword ptr [0]                 ; -> obj7:0x008C
        neg    esi
        test   esi, esi
        je     loc_5_0F36
        push   0x8000000
        mov    ecx, edi
        push   0
        call   sub_5_1906
loc_5_0F36:
        mov    eax, esi
        jmp    loc_5_0F8F
loc_5_0F3A:
        push   1
        mov    edx, 0x8001
        mov    ecx, edi
        call   sub_5_6048
        test   eax, eax
        je     loc_5_0F7F
        mov    edx, dword ptr [0]            ; -> obj7:0x0000
        mov    ecx, edi
        call   sub_5_0D09
        test   eax, eax
        je     loc_5_0F7F
        mov    ecx, edi
        call   sub_5_0D1E
        test   eax, eax
        je     loc_5_0F92
        push   0
        mov    eax, dword ptr [0]            ; -> obj7:0x0000
        push   0
        mov    edx, edi
        push   -1
        xor    ecx, ecx
        push   eax
        push   9
        call   sub_5_CEA8
loc_5_0F7F:
        push   0
        mov    edx, 0x8002
        mov    ecx, edi
        call   sub_5_6048
loc_5_0F8D:
        xor    eax, eax
loc_5_0F8F:
        pop    edi
        pop    esi
        ret    
loc_5_0F92:
        push   0x8000000
        mov    ecx, edi
        push   0
        call   sub_5_1906
        mov    eax, 1
        jmp    loc_5_0F8F
sub_5_0FA7:
        push   esi
        push   0                             ; -> sub_5_0E28
        mov    esi, dword ptr [0]            ; -> obj7:0x0000
        push   0                             ; -> loc_5_0EB3
        mov    dword ptr [0], edx            ; -> obj7:0x0000
        mov    edx, ecx
        mov    ecx, 2
        call   sub_5_CBF2
        mov    dword ptr [0], esi            ; -> obj7:0x0000
        pop    esi
        ret    
sub_5_0FD2:
        push   esi
        push   0                             ; -> loc_5_0DBF
        mov    esi, dword ptr [0]            ; -> obj7:0x0000
        mov    dword ptr [0], edx            ; -> obj7:0x0000
        mov    edx, ecx
        mov    ecx, 2
        call   sub_5_CBE2
        mov    dword ptr [0], esi            ; -> obj7:0x0000
        pop    esi
        ret    
        db  056h, 068h
        dd  sub_5_0E28               ; fixup
        db  08Bh, 035h
        dd  obj7:0x0000              ; fixup
        db  089h, 015h
        dd  obj7:0x0000              ; fixup
        db  08Bh, 0D1h, 0B9h, 003h, 000h, 000h, 000h, 0E8h, 0CCh, 0BBh, 000h, 000h, 089h, 035h
        dd  obj7:0x0000              ; fixup
        db  05Eh, 0C3h
loc_5_101E:
        mov    ecx, dword ptr [esp + 4]
        cmp    dword ptr [ecx + 0xc], 0x1f
        jne    loc_5_102D
        call   sub_5_7024
loc_5_102D:
        mov    eax, 1
        ret    
sub_5_1033:
        sub    esp, 4
        mov    edx, 1
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        push   0                             ; -> obj7:0x01D8
        mov    ecx, esi
        call   sub_5_1B18
        test   eax, eax
        jne    loc_5_1055
        xor    edi, edi
        jmp    loc_5_109B
loc_5_1055:
        push   0
        mov    ecx, esi
        push   0x40
        push   0
        push   0x40
        call   sub_5_18E3
        test   eax, eax
        mov    eax, 1
        je     loc_5_1078
        mov    eax, dword ptr [0]            ; -> obj7:0x00AC
        and    eax, 8
        shr    eax, 2
loc_5_1078:
        mov    ecx, 1
        mov    edx, esi
        push   ecx
        push   0
        push   -1
        push   eax
        push   ecx
        call   sub_5_CEA8
        mov    edi, eax
        push   0                             ; -> obj7:0x01D8
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_1B18
loc_5_109B:
        test   edi, edi
        je     loc_5_1106
        push   0
        mov    ecx, esi
        push   0x48
        call   sub_5_1906
        lea    eax, [esp + 8]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [esp + 9], 2
        je     loc_5_10CD
        push   0x400000
        mov    ecx, esi
        push   0
        call   sub_5_1906
loc_5_10CD:
        mov    ecx, esi
        call   sub_5_0BAC
        push   0
        mov    edx, 0x8000
        mov    ecx, esi
        call   sub_5_6048
        push   0
        mov    edx, esi
        push   0                             ; -> loc_5_101E
        mov    ecx, 2
        call   sub_5_1A48
        call   sub_5_7B5F
        call   sub_5_73CA
        mov    eax, 1
        jmp    loc_5_1136
loc_5_1106:
        mov    ecx, esi
        call   sub_5_0BAC
        push   0
        mov    ecx, 0x40000
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        mov    eax, 0
        jne    loc_5_1136
        mov    edx, 0xa
        mov    ecx, esi
        call   sub_5_731F
        xor    eax, eax
loc_5_1136:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_5_113C:
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    edx, esi
        push   0                             ; -> loc_5_113C
        mov    ecx, 2
        call   sub_5_1A48
        push   edi
        mov    ecx, esi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_11D6
        xor    edi, edi
        mov    edx, 0x8004
        push   edi
        mov    ecx, esi
        call   sub_5_6048
        push   edi
        mov    eax, 0x400
        push   edi
        mov    ecx, esi
        push   -1
        push   edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        mov    edx, esi
        cmp    eax, 1
        sbb    eax, eax
        xor    ecx, ecx
        inc    eax
        push   eax
        push   2
        call   sub_5_CEA8
        push   edi
        mov    ecx, esi
        push   8
        call   sub_5_1923
        push   edi
        mov    eax, 0x100000
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_11CA
        test   byte ptr [1], 0x40            ; -> obj7:0x00AD
        jne    loc_5_11CA
        mov    ecx, esi
        call   sub_5_1CB6
loc_5_11CA:
        mov    ecx, esi
        call   sub_5_0BAC
        call   sub_5_73CA
loc_5_11D6:
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
sub_5_11DE:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        mov    esi, 1
        mov    edx, esi
        call   sub_5_0FA7
        test   eax, eax
        mov    eax, 0
        je     loc_5_1205
        mov    edx, esi
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_0FD2
        mov    eax, esi
loc_5_1205:
        pop    esi
        ret    
loc_5_1207:
        xor    edx, edx
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_0FA7
        test   eax, eax
        mov    eax, 0
        je     loc_5_122B
        xor    edx, edx
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_0FD2
        mov    eax, 1
loc_5_122B:
        ret    
sub_5_122C:
        mov    edx, dword ptr [esp + 4]
        push   0                             ; -> loc_5_113C
        mov    ecx, 2
        call   sub_5_CBE2
        ret    
sub_5_1240:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x194
        push   ebx
        mov    dword ptr [ebp - 8], edx
        push   esi
        mov    ebx, ecx
        push   edi
        mov    esi, 0                        ; -> obj7:0x0050
        lea    edi, [ebp - 0x194]
        xor    eax, eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    edi, [ebp - 0x18a]
        mov    ecx, 0x1d
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, 0                        ; -> obj7:0x005C
        lea    edi, [ebp - 0x14]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0                        ; -> obj7:0x0068
        lea    edi, [ebp - 0x94]
        movsb  byte ptr es:[edi], byte ptr [esi]
        lea    edi, [ebp - 0x93]
        mov    edx, 0x1f
        mov    ecx, edx
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, 0                        ; -> obj7:0x006C
        mov    ecx, edx
        stosb  byte ptr es:[edi], al
        lea    edi, [ebp - 0x114]
        movsb  byte ptr es:[edi], byte ptr [esi]
        lea    edi, [ebp - 0x113]
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        xor    edi, edi
        call   sub_5_C73C
        mov    word ptr [ebp - 2], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, edi
        je     loc_5_12CE
        cmp    dword ptr [esi + 4], edi
        jne    loc_5_13CD
loc_5_12CE:
        lea    ecx, [ebp - 0x94]
        mov    edx, dword ptr [ebp - 8]
        call   sub_5_C486
        test   ebx, ebx
        jne    loc_5_12E8
        lea    ecx, [ebp - 0x14]
        jmp    loc_5_138A
loc_5_12E8:
        cmp    dword ptr [0], 0              ; -> obj7:0x004C
        jne    loc_5_131A
        push   0                             ; -> obj7:0x004C
        push   1
        push   0                             ; -> obj7:0x0070
        push   2
        call   sub_5_D0F4
        add    esp, 0x10
        test   eax, eax
        je     loc_5_131A
        mov    ecx, dword ptr [ebp - 2]
        call   sub_5_C77D
        xor    eax, eax
        jmp    loc_5_1400
loc_5_131A:
        mov    edi, 0x80
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0x194]
        push   ecx
        mov    eax, dword ptr [0]            ; -> obj7:0x004C
        push   0
        mov    dword ptr [ebp - 8], edi
        push   0
        push   ebx
        push   eax
        call   sub_5_D11C
        add    esp, 0x18
        lea    ecx, [ebp - 0x114]
        mov    edx, ebx
        mov    dword ptr [ebp - 8], edi
        lea    edi, [ebp - 8]
        call   sub_5_C486
        mov    edx, 0                        ; -> obj7:0x007C
        lea    ecx, [ebp - 0x114]
        call   sub_5_C44E
        push   edi
        lea    eax, [ebp - 0x94]
        push   eax
        lea    ecx, [ebp - 0x114]
        push   0
        push   0
        push   ecx
        push   dword ptr [0]                 ; -> obj7:0x004C
        call   sub_5_D11C
        add    esp, 0x18
        lea    ecx, [ebp - 0x194]
loc_5_138A:
        test   esi, esi
        jne    loc_5_13A8
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        lea    ebx, [ebp - 0x94]
        push   ebx
        push   ecx
        call   sub_5_D104
        add    esp, 0x10
        mov    edi, eax
        jmp    loc_5_13CD
loc_5_13A8:
        push   ecx
        call   sub_5_D114
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0x20
        jb     loc_5_13CD
        lea    eax, [ebp - 0x94]
        mov    dword ptr [esi], edi
        push   eax
        push   edi
        call   sub_5_D10C
        add    esp, 8
        mov    dword ptr [esi + 4], eax
loc_5_13CD:
        test   esi, esi
        je     loc_5_13F6
        mov    ecx, dword ptr [esi + 4]
        test   ecx, ecx
        je     loc_5_13F6
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   ecx
        add    esi, 8
        push   0
        mov    ebx, dword ptr [esi]
        inc    ebx
        mov    dword ptr [esi], ebx
        call   sub_5_D104
        add    esp, 0x10
        mov    edi, eax
        dec    ebx
        mov    dword ptr [esi], ebx
loc_5_13F6:
        mov    ecx, dword ptr [ebp - 2]
        call   sub_5_C77D
        mov    eax, edi
loc_5_1400:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_5_1409:
        push   edi
        mov    eax, dword ptr [ecx]
        mov    edi, ecx
        test   eax, eax
        je     loc_5_142D
        cmp    dword ptr [edi + 8], 0
        jne    loc_5_1428
        push   eax
        call   sub_5_D124
        add    esp, 4
        xor    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        jmp    loc_5_142D
loc_5_1428:
        call   sub_5_7B5F
loc_5_142D:
        pop    edi
        ret    
loc_5_142F:
        push   esi
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> obj7:0x004C
        je     loc_5_144E
        mov    eax, dword ptr [0]            ; -> obj7:0x004C
        push   eax
        call   sub_5_D0E4
        add    esp, 4
        mov    dword ptr [0], esi            ; -> obj7:0x004C
loc_5_144E:
        pop    esi
        ret    
sub_5_1450:
        cmp    edx, 1
        sbb    eax, eax
        sub    ecx, 4
        and    eax, 0x80000000
        push   eax
        push   0
        push   0x574d5950
        push   0x18
        push   ecx
        call   sub_5_D12C
        add    esp, 0x14
        ret    
sub_5_1471:
        push   0x80000000
        push   0x18
        push   0x4c52574d
        push   4
        push   ecx
        call   sub_5_D12C
        add    esp, 0x14
        ret    
sub_5_1489:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        xor    edx, edx
        mov    ecx, dword ptr [esi]
        sub    ecx, 0x1c
        call   sub_5_1471
        test   eax, eax
        je     loc_5_14A2
        sub    dword ptr [esi], 0x1c
loc_5_14A2:
        pop    edi
        pop    esi
        ret    
sub_5_14A5:
        push   0x201
        push   0x18
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        je     loc_5_14C1
        mov    dword ptr [eax], 0x574d5950
        add    eax, 4
loc_5_14C1:
        ret    
sub_5_14C2:
        push   esi
        push   0x201
        lea    eax, [ecx + 0x1c]
        mov    esi, ecx
        push   eax
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        je     loc_5_14E4
        mov    dword ptr [eax + 0x18], 0x4c52574d
        mov    dword ptr [eax + 0x14], esi
loc_5_14E4:
        pop    esi
        ret    
sub_5_14E6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    edi, ecx
        xor    esi, esi
        call   sub_5_14A5
        mov    dword ptr [ebp - 4], eax
        cmp    eax, esi
        jne    loc_5_1508
        xor    eax, eax
        jmp    loc_5_158F
loc_5_1508:
        push   0x14
        mov    edx, edi
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_5B5D
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [edi + 0x10]
        test   edi, edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x10], 0
        je     loc_5_158C
loc_5_1534:
        mov    eax, dword ptr [edi + 0x14]
        push   0x201
        add    eax, 0x1c
        push   eax
        mov    dword ptr [ebp - 8], eax
        call   sub_5_D134
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_1584
        mov    edx, edi
        mov    ecx, ebx
        push   dword ptr [ebp - 8]
        call   sub_5_5B5D
        mov    eax, dword ptr [ebp - 4]
        test   esi, esi
        mov    dword ptr [ebx + 0x10], 0
        mov    dword ptr [ebx + 0xc], eax
        je     loc_5_1573
        mov    dword ptr [esi + 0x10], ebx
        jmp    loc_5_1579
loc_5_1573:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x10], ebx
loc_5_1579:
        mov    esi, ebx
        mov    edi, dword ptr [edi + 0x10]
        test   edi, edi
        jne    loc_5_1534
        jmp    loc_5_158C
loc_5_1584:
        lea    ecx, [ebp - 4]
        call   sub_5_1596
loc_5_158C:
        mov    eax, dword ptr [ebp - 4]
loc_5_158F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_1596:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [ecx]
        mov    edi, ecx
        test   ebx, ebx
        je     loc_5_15E8
        mov    esi, dword ptr [ebx + 0x10]
        test   esi, esi
        je     loc_5_15CA
        xor    ebp, ebp
loc_5_15AB:
        mov    eax, esi
        mov    dword ptr [esi + 0x18], ebp
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        mov    esi, dword ptr [esi + 0x10]
        jne    loc_5_15C6
        push   ebp
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_15C6:
        test   esi, esi
        jne    loc_5_15AB
loc_5_15CA:
        lea    eax, [ebx - 4]
        xor    edx, edx
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax], edx
        mov    dword ptr [edi], ecx
        jne    loc_5_15E8
        push   edx
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_15E8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_15ED:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ecx]
        mov    edi, ecx
        test   esi, esi
        je     loc_5_160D
loc_5_15FA:
        lea    ecx, [esp + 8]
        mov    dword ptr [esp + 8], esi
        mov    esi, dword ptr [esi]
        call   sub_5_1596
        test   esi, esi
        jne    loc_5_15FA
loc_5_160D:
        mov    dword ptr [edi], 0
        pop    edi
        pop    esi
        add    esp, 4
        ret    
sub_5_1619:
        push   esi
        mov    esi, dword ptr [ecx]
        cmp    edx, esi
        jne    loc_5_1624
        mov    eax, ecx
        jmp    loc_5_1643
loc_5_1624:
        cmp    dword ptr [esi], 0
        je     loc_5_1636
loc_5_1629:
        mov    eax, dword ptr [esi]
        cmp    edx, eax
        je     loc_5_1636
        cmp    dword ptr [eax], 0
        mov    esi, eax
        jne    loc_5_1629
loc_5_1636:
        mov    eax, 0
        cmp    dword ptr [esi], 1
        adc    eax, -1
        and    eax, esi
loc_5_1643:
        pop    esi
        ret    
sub_5_1645:
        lea    eax, [ecx + 0x10]
        mov    ecx, dword ptr [eax]
        cmp    ecx, edx
        je     loc_5_1673
        cmp    dword ptr [ecx + 0x10], 0
        je     loc_5_1663
loc_5_1654:
        mov    eax, dword ptr [ecx + 0x10]
        cmp    eax, edx
        je     loc_5_1663
        cmp    dword ptr [eax + 0x10], 0
        mov    ecx, eax
        jne    loc_5_1654
loc_5_1663:
        add    ecx, 0x10
        mov    eax, 0
        cmp    dword ptr [ecx], 1
        adc    eax, -1
        and    eax, ecx
loc_5_1673:
        ret    
sub_5_1674:
        mov    eax, dword ptr [esp + 8]
        push   ebx
        push   edi
        mov    dword ptr [edx + 4], ecx
        mov    edi, dword ptr [esp + 0xc]
        lea    ebx, [ecx + eax*4 + 0x30]
        mov    dword ptr [edx + 0xc], edi
        mov    dword ptr [edx + 8], eax
        cmp    dword ptr [ebx], 0
        je     loc_5_16A9
loc_5_1690:
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ecx + 0xc]
        cmp    edi, eax
        jb     loc_5_16A9
        jne    loc_5_16A2
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_5_16A9
loc_5_16A2:
        cmp    dword ptr [ecx], 0
        mov    ebx, ecx
        jne    loc_5_1690
loc_5_16A9:
        mov    eax, dword ptr [ebx]
        pop    edi
        mov    dword ptr [edx], eax
        mov    dword ptr [ebx], edx
        pop    ebx
        ret    0xc
sub_5_16B4:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        call   sub_5_14A5
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_16DA
        mov    edx, ebx
        mov    ecx, esi
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   edi
        call   sub_5_1674
        jmp    loc_5_16E6
loc_5_16DA:
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
loc_5_16E6:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_5_16EE:
        push   esi
        mov    edx, dword ptr [ecx]
        mov    esi, ecx
        mov    eax, dword ptr [edx + 8]
        mov    ecx, dword ptr [edx + 4]
        lea    ecx, [ecx + eax*4 + 0x30]
        call   sub_5_1619
        mov    ecx, eax
        call   sub_5_1596
        mov    dword ptr [esi], 0
        pop    esi
        ret    
sub_5_1711:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    edi, ecx
        mov    ecx, dword ptr [esp + 0x18]
        call   sub_5_14C2
        mov    esi, eax
        test   esi, esi
        jne    loc_5_1738
        mov    edx, 3
        mov    ecx, dword ptr [ebx + 4]
        call   sub_5_731F
        xor    eax, eax
        jmp    loc_5_175B
loc_5_1738:
        mov    eax, dword ptr [esp + 0x10]
        lea    ecx, [esi + 0x1c]
        mov    edx, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 4], eax
        push   dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0xc], ebx
        call   sub_5_5B5D
        mov    ecx, dword ptr [edi]
        mov    eax, esi
        mov    dword ptr [esi + 0x10], ecx
        mov    dword ptr [edi], esi
loc_5_175B:
        pop    edi
        pop    esi
        pop    ebx
        ret    0xc
sub_5_1761:
        push   esi
        mov    esi, ecx
        lea    ecx, [ecx + 0x10]
        cmp    dword ptr [ecx], 0
        je     loc_5_1776
loc_5_176C:
        mov    ecx, dword ptr [ecx]
        add    ecx, 0x10
        cmp    dword ptr [ecx], 0
        jne    loc_5_176C
loc_5_1776:
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        push   edx
        mov    edx, esi
        call   sub_5_1711
        pop    esi
        ret    8
sub_5_178A:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        push   ebp
        mov    esi, ecx
        mov    ebp, dword ptr [esp + 0x18]
        cmp    dword ptr [esi + 0x14], ebp
        jne    loc_5_17EA
        mov    edi, esi
loc_5_179D:
        push   ebp
        lea    ecx, [edi + 0x1c]
        mov    edx, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 4], ebx
        call   sub_5_5B5D
        cmp    edi, esi
        je     loc_5_17E1
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [esi + 0x10]
        mov    edx, esi
        mov    dword ptr [edi + 0xc], eax
        mov    dword ptr [edi + 0x10], ecx
        mov    ecx, dword ptr [esi + 0xc]
        call   sub_5_1645
        mov    dword ptr [eax], edi
        xor    eax, eax
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        mov    dword ptr [esi + 0x18], eax
        jne    loc_5_17E1
        push   eax
        push   esi
        call   sub_5_D0C8
        add    esp, 8
loc_5_17E1:
        mov    eax, edi
loc_5_17E3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_5_17EA:
        mov    ecx, ebp
        call   sub_5_14C2
        mov    edi, eax
        test   edi, edi
        jne    loc_5_179D
        xor    eax, eax
        jmp    loc_5_17E3
sub_5_17FB:
        push   esi
        push   edi
        mov    edi, dword ptr [ecx + 0xc]
        mov    edx, ecx
        mov    esi, ecx
        mov    ecx, edi
        call   sub_5_1645
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax], ecx
        mov    ecx, edi
        sub    ecx, eax
        cmp    ecx, -0x10
        je     loc_5_181C
        lea    edi, [eax + 0xc]
loc_5_181C:
        xor    eax, eax
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        mov    dword ptr [esi + 0x18], eax
        jne    loc_5_1834
        push   eax
        push   esi
        call   sub_5_D0C8
        add    esp, 8
loc_5_1834:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_5_1839:
        test   edx, edx
        je     loc_5_1847
        mov    eax, 0
        cmp    dword ptr [ecx + 4], edx
        jne    loc_5_184C
loc_5_1847:
        mov    eax, 1
loc_5_184C:
        ret    
sub_5_184D:
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, dword ptr [ecx + 0x10]
        test   edi, edi
        jne    loc_5_185C
        xor    eax, eax
        jmp    loc_5_1874
loc_5_185C:
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_1839
        test   eax, eax
        mov    eax, edi
        jne    loc_5_1874
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_1877
loc_5_1874:
        pop    edi
        pop    esi
        ret    
sub_5_1877:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
loc_5_187D:
        mov    esi, dword ptr [esi + 0x10]
        test   esi, esi
        je     loc_5_1896
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_1839
        test   eax, eax
        je     loc_5_187D
        mov    eax, esi
loc_5_1893:
        pop    edi
        pop    esi
        ret    
loc_5_1896:
        xor    eax, eax
        jmp    loc_5_1893
sub_5_189A:
        test   ecx, ecx
        jne    loc_5_18A2
        xor    eax, eax
        jmp    loc_5_18B5
loc_5_18A2:
        mov    eax, dword ptr [ecx + 0x10]
        test   eax, eax
        je     loc_5_18B5
loc_5_18A9:
        test   edx, edx
        je     loc_5_18B5
        mov    eax, dword ptr [eax + 0x10]
        dec    edx
        test   eax, eax
        jne    loc_5_18A9
loc_5_18B5:
        ret    
sub_5_18B6:
        push   ebp
        mov    eax, dword ptr [ecx + 8]
        mov    ebp, esp
        mov    edx, dword ptr [ecx + 0x88]
        sub    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_18CF:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0x88], edx
        ret    8
sub_5_18E3:
        push   ebp
        mov    ebp, esp
        call   sub_5_18B6
        and    eax, dword ptr [ebp + 8]
        and    edx, dword ptr [ebp + 0xc]
        cmp    edx, dword ptr [ebp + 0x14]
        jne    loc_5_1900
        cmp    eax, dword ptr [ebp + 0x10]
        mov    eax, 1
        je     loc_5_1902
loc_5_1900:
        xor    eax, eax
loc_5_1902:
        pop    ebp
        ret    0x10
sub_5_1906:
        push   esi
        mov    esi, ecx
        call   sub_5_18B6
        or     eax, dword ptr [esp + 8]
        or     edx, dword ptr [esp + 0xc]
        push   edx
        mov    ecx, esi
        push   eax
        call   sub_5_18CF
        pop    esi
        ret    8
sub_5_1923:
        push   esi
        push   edi
        mov    edi, ecx
        call   sub_5_18B6
        mov    ecx, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x10]
        not    ecx
        not    esi
        and    eax, ecx
        and    edx, esi
        push   edx
        mov    ecx, edi
        push   eax
        call   sub_5_18CF
        pop    edi
        pop    esi
        ret    8
sub_5_194A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    ecx, 0xc0000000
        mov    dword ptr [ebp - 4], ecx
        jb     loc_5_1A24
        cmp    ecx, 0xffbfffff
        ja     loc_5_1A24
        test   cl, 3
        jne    loc_5_1A24
        xor    edx, edx
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_1989
        mov    eax, 1
        jmp    loc_5_1A26
loc_5_1989:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_1450
        test   eax, eax
        je     loc_5_19A1
        mov    eax, 2
        jmp    loc_5_1A26
loc_5_19A1:
        lea    ecx, [ebp - 4]
        xor    edx, edx
        call   sub_5_1489
        test   eax, eax
        je     loc_5_19B6
        mov    eax, 3
        jmp    loc_5_1A26
loc_5_19B6:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_8317
        test   eax, eax
        je     loc_5_19CB
        mov    eax, 4
        jmp    loc_5_1A26
loc_5_19CB:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_8335
        test   eax, eax
        je     loc_5_19E0
        mov    eax, 5
        jmp    loc_5_1A26
loc_5_19E0:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_6723
        test   eax, eax
        je     loc_5_19F5
        mov    eax, 6
        jmp    loc_5_1A26
loc_5_19F5:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_6741
        test   eax, eax
        je     loc_5_1A0A
        mov    eax, 7
        jmp    loc_5_1A26
loc_5_1A0A:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_1471
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 8
        jmp    loc_5_1A26
loc_5_1A24:
        xor    eax, eax
loc_5_1A26:
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_1A2A:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0]            ; -> loc_7_01FC
        cmp    dword ptr [ecx + 0x74], eax
        mov    eax, 1
        jne    loc_5_1A47
        push   ecx
        call   sub_7_01F8
        add    esp, 4
loc_5_1A47:
        ret    
sub_5_1A48:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    ebx, dword ptr [0]            ; -> loc_7_01FC
        push   edi
        mov    esi, ecx
        mov    edi, dword ptr [0]            ; -> sub_7_01F8
        push   0                             ; -> loc_5_1A2A
        mov    dword ptr [0], eax            ; -> sub_7_01F8
        mov    dword ptr [0], edx            ; -> loc_7_01FC
        lea    edx, [ebp - 8]
        push   edx
        push   0
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        call   sub_5_CB5A
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_5_1AAE
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        jne    loc_5_1AAE
        push   0                             ; -> loc_5_1A2A
        lea    ecx, [esi + 4]
        push   0
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        push   dword ptr [ebp - 8]
        mov    dword ptr [0], eax            ; -> sub_7_01F8
        call   sub_5_CB5A
loc_5_1AAE:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0], edi            ; -> sub_7_01F8
        pop    edi
        mov    dword ptr [0], ebx            ; -> loc_7_01FC
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_1AC6:
        cmp    edx, 1
        sbb    eax, eax
        and    eax, 0x80000000
        push   eax
        push   0
        push   0x59504c52
        push   0x8c
        push   ecx
        call   sub_5_D12C
        add    esp, 0x14
        ret    
sub_5_1AE7:
        push   ebx
        push   esi
        mov    esi, ecx
        push   0
        mov    ebx, 0x20000
        push   ebx
        push   0
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_1B13
loc_5_1AFF:
        mov    esi, dword ptr [esi + 0x20]
        push   0
        push   ebx
        mov    ecx, esi
        push   0
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_1AFF
loc_5_1B13:
        mov    eax, esi
        pop    esi
        pop    ebx
        ret    
sub_5_1B18:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        test   byte ptr [1], 0x10            ; -> obj7:0x00AD
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        je     loc_5_1B33
        mov    eax, 1
        jmp    loc_5_1B8A
loc_5_1B33:
        mov    ebx, dword ptr [ecx + 4]
        cmp    byte ptr [ebx], 0
        jne    loc_5_1B42
        mov    eax, 1
        jmp    loc_5_1B8A
loc_5_1B42:
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0x40a
        mov    eax, 1
        jb     loc_5_1B8A
        push   0
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    esi, 1
        sbb    eax, eax
        push   ecx
        push   ebx
        and    eax, 3
        push   dword ptr [ebp + 8]
        push   eax
        call   sub_5_D13C
        add    esp, 0x14
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_5_1B8A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_1B93:
        push   esi
        mov    ecx, 0x1000
        mov    esi, dword ptr [esp + 8]
        push   0
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_1BB5
        mov    eax, 1
        jmp    loc_5_1BDA
loc_5_1BB5:
        mov    eax, dword ptr [0]            ; -> loc_7_0208
        mov    edx, dword ptr [esi + 4]
        push   eax
        mov    ecx, dword ptr [0]            ; -> loc_7_0204
        call   sub_5_C3FC
        test   eax, eax
        mov    eax, 1
        je     loc_5_1BDA
        xor    eax, eax
        mov    dword ptr [0], esi            ; -> loc_7_0200
loc_5_1BDA:
        pop    esi
        ret    
sub_5_1BDC:
        push   esi
        mov    esi, ecx
        call   sub_5_C3C4
        test   eax, eax
        jne    loc_5_1BEC
        xor    eax, eax
        jmp    loc_5_1C1C
loc_5_1BEC:
        inc    eax
        push   0                             ; -> loc_5_1B93
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        mov    ecx, 2
        mov    dword ptr [0], eax            ; -> loc_7_0208
        mov    dword ptr [0], esi            ; -> loc_7_0204
        mov    dword ptr [0], 0              ; -> loc_7_0200
        call   sub_5_CBE2
        mov    eax, dword ptr [0]            ; -> loc_7_0200
loc_5_1C1C:
        pop    esi
        ret    
sub_5_1C1E:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    ecx, edx
        push   ebp
        mov    edi, edx
        add    esi, 0x8c
        call   sub_5_C3C4
        mov    ebp, eax
        push   0x201
        lea    eax, [esi + ebp]
        inc    eax
        push   eax
        call   sub_5_D134
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_1C6C
        lea    ecx, [esi + ebx]
        push   ebp
        mov    edx, edi
        mov    dword ptr [ebx + 4], ecx
        call   sub_5_5B5D
        mov    eax, 1
        mov    dword ptr [ebx], 0x59504c52
        mov    dword ptr [ebx + 0x78], eax
        mov    dword ptr [ebx + 0x7c], eax
loc_5_1C6C:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_1C73:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        mov    ecx, edi
        call   sub_5_1D31
        lea    ecx, [eax*4 + 4]
        mov    edx, esi
        call   sub_5_1C1E
        test   eax, eax
        je     loc_5_1CB3
        mov    ecx, dword ptr [edi + 0x24]
        mov    dword ptr [eax + 0x20], edi
        test   ecx, ecx
        je     loc_5_1CB0
        cmp    dword ptr [ecx + 0x28], 0
        je     loc_5_1CAB
loc_5_1CA2:
        mov    ecx, dword ptr [ecx + 0x28]
        cmp    dword ptr [ecx + 0x28], 0
        jne    loc_5_1CA2
loc_5_1CAB:
        mov    dword ptr [ecx + 0x28], eax
        jmp    loc_5_1CB3
loc_5_1CB0:
        mov    dword ptr [edi + 0x24], eax
loc_5_1CB3:
        pop    edi
        pop    esi
        ret    
sub_5_1CB6:
        push   edi
        push   0x80000000
        mov    edi, ecx
        push   0
        call   sub_5_1923
        push   0
        mov    eax, 0x20000
        push   eax
        mov    ecx, edi
        push   0
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_1D27
        mov    edx, dword ptr [edi + 0x38]
        test   edx, edx
        je     loc_5_1D16
        mov    ecx, edi
        call   sub_5_058A
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        mov    ecx, edi
        mov    edx, dword ptr [eax + 0x38]
        call   sub_5_0969
        test   eax, eax
        je     loc_5_1D11
        mov    ecx, 1
        call   sub_5_099C
        test   eax, eax
        jne    loc_5_1D11
        call   sub_5_0A32
        jmp    loc_5_1D16
loc_5_1D11:
        call   sub_5_09DB
loc_5_1D16:
        push   0x10000000
        mov    ecx, edi
        push   0x800
        call   sub_5_1923
loc_5_1D27:
        lea    ecx, [edi + 0x38]
        call   sub_5_15ED
        pop    edi
        ret    
sub_5_1D31:
        xor    eax, eax
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        cmp    edx, ecx
        je     loc_5_1D45
loc_5_1D3D:
        inc    eax
        mov    ecx, dword ptr [ecx + 0x20]
        cmp    edx, ecx
        jne    loc_5_1D3D
loc_5_1D45:
        ret    
sub_5_1D46:
        push   esi
        mov    esi, ecx
        mov    ecx, edx
        call   sub_5_1D31
        lea    eax, [esi + eax*4 + 0x88]
        pop    esi
        ret    
sub_5_1D59:
        test   ecx, ecx
        je     loc_5_1D68
loc_5_1D5D:
        cmp    edx, ecx
        je     loc_5_1D6B
        mov    ecx, dword ptr [ecx + 0x20]
        test   ecx, ecx
        jne    loc_5_1D5D
loc_5_1D68:
        xor    eax, eax
loc_5_1D6A:
        ret    
loc_5_1D6B:
        mov    eax, 1
        jmp    loc_5_1D6A
sub_5_1D72:
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        cmp    eax, edx
        push   esi
        push   edi
        mov    ebx, edx
        mov    edi, ecx
        je     loc_5_1DB2
        cmp    eax, edi
        je     loc_5_1DB2
        mov    esi, 1
        mov    edx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_1DB2
        mov    edx, esi
        mov    ecx, ebx
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_1DB2
        mov    edx, ebx
        mov    ecx, edi
        call   sub_5_1D59
        test   eax, eax
        mov    eax, esi
        jne    loc_5_1DB4
loc_5_1DB2:
        xor    eax, eax
loc_5_1DB4:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_5_1DB8:
        mov    eax, 1
        ret    
sub_5_1DBE:
        push   ebp
        mov    ebp, esp
        push   esi
        cmp    ecx, 0x13
        mov    esi, ecx
        ja     loc_5_1DDB
        je     loc_5_1E44
        test   esi, esi
        je     loc_5_1E44
        cmp    esi, 1
        je     loc_5_1DF6
        cmp    esi, 2
        je     loc_5_1DFA
        jmp    loc_5_1E3F
loc_5_1DDB:
        cmp    esi, 0x17
        je     loc_5_1E3F
        cmp    esi, 0x22
        je     loc_5_1E09
        cmp    esi, 0x24
        je     loc_5_1E10
        cmp    esi, 0x29
        je     loc_5_1E23
        cmp    esi, 0x2b
        je     loc_5_1E32
        jmp    loc_5_1E3F
loc_5_1DF6:
        mov    esi, edx
        jmp    loc_5_1E44
loc_5_1DFA:
        mov    edx, 3
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_731F
        jmp    loc_5_1E44
loc_5_1E09:
        mov    edx, 0xe
        jmp    loc_5_1E37
loc_5_1E10:
        push   0
        mov    ecx, dword ptr [ebp + 8]
        push   8
        call   sub_5_1923
        mov    edx, 0x18
        jmp    loc_5_1E37
loc_5_1E23:
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1CB6
        mov    edx, 0x18
        jmp    loc_5_1E37
loc_5_1E32:
        mov    edx, 0x1e
loc_5_1E37:
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_731F
loc_5_1E3F:
        mov    esi, 0x13
loc_5_1E44:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    4
sub_5_1E4B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    ecx, 3
        mov    edi, edx
        mov    esi, ecx
        je     loc_5_1E63
        cmp    esi, 8
        je     loc_5_1EAB
        xor    ebx, ebx
        jmp    loc_5_1EBE
loc_5_1E63:
        cmp    edi, 1
        je     loc_5_1E6C
        xor    ebx, ebx
        jmp    loc_5_1EBE
loc_5_1E6C:
        push   0
        mov    eax, 0x4000
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        push   eax
        call   sub_5_18E3
        push   0
        cmp    eax, 1
        sbb    ebx, ebx
        push   8
        push   0
        mov    ecx, dword ptr [ebp + 0xc]
        neg    ebx
        push   8
        call   sub_5_18E3
        and    ebx, eax
        and    ebx, dword ptr [ebp + 0x10]
        cmp    ebx, 1
        mov    ebx, 0
        adc    ebx, -1
        and    ebx, 0x13
        jmp    loc_5_1EBE
loc_5_1EAB:
        cmp    edi, 9
        jb     loc_5_1EB5
        cmp    edi, 0x10
        jbe    loc_5_1EB9
loc_5_1EB5:
        xor    ebx, ebx
        jmp    loc_5_1EBE
loc_5_1EB9:
        mov    ebx, 0x13
loc_5_1EBE:
        mov    ecx, dword ptr [ebp + 8]
        xor    edx, edx
        cmp    dword ptr [ecx], edx
        je     loc_5_1EFE
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax], edx
        je     loc_5_1EFE
        cmp    dword ptr [ebp + 0x10], edx
        jne    loc_5_1EE8
        mov    eax, dword ptr [ecx + 0x1c]
        test   eax, eax
        je     loc_5_1EFE
        push   edx
        push   dword ptr [ebp + 0xc]
        push   ecx
        push   edi
        push   esi
        call   eax
        add    esp, 0x14
        jmp    loc_5_1F03
loc_5_1EE8:
        mov    eax, dword ptr [ecx + 0x18]
        test   eax, eax
        je     loc_5_1EFE
        push   0
        push   dword ptr [ecx + 0x14]
        push   ecx
        push   edi
        push   esi
        call   eax
        add    esp, 0x14
        jmp    loc_5_1F03
loc_5_1EFE:
        mov    eax, 1
loc_5_1F03:
        mov    edx, ebx
        mov    ecx, eax
        push   dword ptr [ebp + 0xc]
        call   sub_5_1DBE
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
sub_5_1F16:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x14]
        test   edi, 1
        je     loc_5_1F39
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   0x2000
        mov    ecx, esi
        call   sub_5_1906
        jmp    loc_5_1F3C
loc_5_1F39:
        mov    esi, dword ptr [ebp + 8]
loc_5_1F3C:
        test   edi, 2
        je     loc_5_1F52
        push   0
        mov    ecx, esi
        push   0x4000
        call   sub_5_1906
loc_5_1F52:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        mov    dword ptr [esi + 0x18], eax
        jne    loc_5_1F63
        mov    dword ptr [esi + 0x18], 0     ; -> loc_5_1DB8
loc_5_1F63:
        mov    eax, dword ptr [ebp + 0x10]
        push   0
        push   0x22
        mov    ecx, esi
        mov    dword ptr [esi + 0x14], eax
        call   sub_5_1906
        push   0
        mov    ecx, esi
        push   8
        call   sub_5_1923
        test   edi, 4
        je     loc_5_1F8E
        call   sub_5_7B5F
        jmp    loc_5_1FAC
loc_5_1F8E:
        push   0x200000
        xor    edi, edi
        push   edi
        mov    ecx, esi
        call   sub_5_1906
        push   edi
        mov    ecx, 0                        ; -> sub_5_79CB
        push   edi
        mov    edx, esi
        push   edi
        call   sub_5_02D3
loc_5_1FAC:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_5_1FB0:
        mov    edx, 0x16
        mov    ecx, dword ptr [esp + 4]
        jmp    sub_5_731F
sub_5_1FBE:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_5_122C
        add    esp, 4
        mov    ecx, 0x20000
        push   0
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_1FFA
        push   0x90000000
        mov    ecx, esi
        push   0x800
        call   sub_5_1923
        mov    ecx, esi
        call   sub_5_1CB6
loc_5_1FFA:
        mov    ecx, esi
        call   sub_5_7024
        cmp    dword ptr [esp + 0xc], 0
        je     loc_5_2013
        push   esi
        call   sub_5_79CB
        add    esp, 4
        jmp    loc_5_2018
loc_5_2013:
        call   sub_5_73CA
loc_5_2018:
        pop    esi
        ret    
loc_5_201A:
        push   esi
        mov    eax, 0x400
        mov    esi, dword ptr [esp + 8]
        push   edi
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_206E
        mov    eax, 0x1000000
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_206E
        push   edi
        push   esi
        call   sub_5_1FBE
        add    esp, 8
        xor    edx, edx
        mov    ecx, 0xf
        push   edi
        push   esi
        push   dword ptr [esi + 0x20]
        call   sub_5_1E4B
        push   esi
        call   sub_5_79CB
        add    esp, 4
loc_5_206E:
        pop    edi
        pop    esi
        ret    
loc_5_2071:
        sub    esp, 0xcc
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xd8]
        push   edi
        push   ebp
        mov    ecx, esi
        xor    ebx, ebx
        call   sub_5_8CCD
        push   ebx
        xor    edx, edx
        push   esi
        mov    ecx, 6
        push   dword ptr [esi + 0x20]
        call   sub_5_1E4B
        push   ebx
        mov    eax, 0x80
        push   eax
        mov    ecx, dword ptr [esi + 0x20]
        push   ebx
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_210A
        mov    ebp, dword ptr [esi + 4]
        mov    ecx, ebp
        call   sub_5_C3C4
        lea    ecx, [esp + 0x10]
        push   eax
        mov    edx, ebp
        call   sub_5_5B5D
        mov    edx, 0x5c
        mov    ecx, ebp
        call   sub_5_C3E0
        lea    ecx, [ebp + eax + 1]
        mov    edi, eax
        mov    edx, 0x5c
        call   sub_5_C3E0
        lea    eax, [edi + eax + 1]
        push   1
        lea    edx, [esp + 0x14]
        mov    ecx, esi
        mov    byte ptr [esp + eax + 0x14], bl
        call   sub_5_21CF
        lea    edx, [esp + edi + 0x11]
        cmp    byte ptr [edx], 0x2a
        jne    loc_5_210A
        push   1
        mov    ecx, esi
        call   sub_5_21CF
loc_5_210A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xcc
        ret    
loc_5_2115:
        push   esi
        xor    edx, edx
        mov    esi, dword ptr [esp + 8]
        push   1
        push   esi
        mov    ecx, 0x10
        push   esi
        call   sub_5_1E4B
        push   0
        xor    edx, edx
        push   esi
        mov    ecx, 0x10
        push   esi
        call   sub_5_1E4B
        pop    esi
        ret    
loc_5_213C:
        push   ebp
        mov    ecx, 0x11
        mov    ebp, esp
        push   dword ptr [esp + 0x10]
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        call   sub_5_1E4B
        pop    ebp
        ret    
sub_5_2158:
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, ecx
loc_5_215F:
        mov    edx, 0x2c
        mov    ecx, edi
        call   sub_5_C3E0
        cmp    eax, -1
        je     loc_5_218F
        lea    ebx, [eax + edi]
        mov    edx, esi
        push   dword ptr [esp + 0x10]
        mov    ecx, edi
        mov    byte ptr [ebx], 0
        call   sub_5_C3FC
        mov    byte ptr [ebx], 0x2c
        test   eax, eax
        jne    loc_5_219E
        lea    edi, [ebx + 1]
        jmp    loc_5_215F
loc_5_218F:
        mov    edx, esi
        mov    ecx, edi
        push   dword ptr [esp + 0x10]
        call   sub_5_C3FC
        jmp    loc_5_21A3
loc_5_219E:
        mov    eax, 1
loc_5_21A3:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_5_21A9:
        push   esi
        mov    esi, ecx
        call   sub_5_C430
        cmp    byte ptr [esi], 0
        je     loc_5_21CD
loc_5_21B6:
        mov    al, byte ptr [esi]
        cmp    al, 0x20
        jle    loc_5_21C4
        cmp    al, 0x7f
        jg     loc_5_21C4
        cmp    al, 0x2c
        jne    loc_5_21C7
loc_5_21C4:
        mov    byte ptr [esi], 0x5f
loc_5_21C7:
        inc    esi
        cmp    byte ptr [esi], 0
        jne    loc_5_21B6
loc_5_21CD:
        pop    esi
        ret    
sub_5_21CF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x404
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    ecx, edx
        mov    edi, edx
        call   sub_5_C3C4
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_22A6
        mov    eax, 0x3fe
        push   0
        sub    eax, ebx
        lea    ecx, [ebp - 0x404]
        cmp    dword ptr [ebp + 8], 0
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        push   1
        mov    eax, 0                        ; -> obj7:0x022C
        jne    loc_5_2219
        mov    eax, 0                        ; -> loc_7_0238
loc_5_2219:
        push   eax
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_87D6
        test   eax, eax
        je     loc_5_222E
        cmp    eax, 0x25
        je     loc_5_2253
        jmp    loc_5_22A6
loc_5_222E:
        inc    ebx
        lea    ecx, [ebp - 0x404]
        push   ebx
        mov    edx, edi
        call   sub_5_2158
        test   eax, eax
        jne    loc_5_22A6
        mov    edx, 0                        ; -> loc_7_0248
        lea    ecx, [ebp - 0x404]
        call   sub_5_C44E
        jmp    loc_5_225A
loc_5_2253:
        mov    byte ptr [ebp - 0x404], 0
loc_5_225A:
        lea    ecx, [ebp - 0x404]
        call   sub_5_C3C4
        mov    ebx, eax
        lea    ecx, [ebp - 0x404]
        mov    edx, edi
        call   sub_5_C44E
        lea    ecx, [ebp + ebx - 0x404]
        call   sub_5_21A9
        xor    eax, eax
        lea    ecx, [ebp - 0x404]
        push   eax
        cmp    dword ptr [ebp + 8], eax
        push   eax
        push   ecx
        push   1
        mov    eax, 0                        ; -> loc_7_024C
        jne    loc_5_229C
        mov    eax, 0                        ; -> loc_7_0258
loc_5_229C:
        push   eax
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_886D
loc_5_22A6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_22AF:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    edx, [ebp - 4]
        xor    esi, esi
        push   esi
        push   eax
        push   dword ptr [0]                 ; -> loc_7_020C
        push   edx
        call   _CONFIGMG_Create_DevNode
        add    esp, 0x10
        test   eax, eax
        jne    loc_5_22FE
        push   esi
        push   esi
        push   dword ptr [ebp - 4]
        call   _CONFIGMG_Register_Enumerator
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 4]
        push   esi
        push   0x800
        call   sub_5_1906
        push   esi
        push   esi
        push   esi
        push   dword ptr [ebp - 4]
        call   sub_5_1F16
        add    esp, 0x10
loc_5_22FE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_2303:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0]            ; -> loc_7_0214
        cmp    dword ptr [ecx + 0xc], eax
        jne    loc_5_2316
        call   sub_5_7024
loc_5_2316:
        mov    eax, 1
        ret    
sub_5_231C:
        mov    edx, 0                        ; -> loc_5_2303
        mov    dword ptr [0], ecx            ; -> loc_7_0214
        xor    ecx, ecx
        jmp    sub_5_CC30
loc_5_232E:
        push   esi
        mov    eax, 0x40000
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0xc]
        push   esi
        push   eax
        mov    ecx, edi
        push   esi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_23D7
        cmp    dword ptr [edi + 0x40], esi
        jne    loc_5_23D7
        push   esi
        mov    eax, 0x4000
        push   eax
        mov    ecx, edi
        push   esi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_23D7
        push   esi
        mov    eax, 0x1000
        push   eax
        mov    ecx, edi
        push   esi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_23D7
        push   esi
        mov    eax, 0x20000
        push   eax
        mov    ecx, edi
        push   esi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_23D7
        cmp    dword ptr [edi + 0x38], esi
        je     loc_5_23D7
        mov    eax, dword ptr [edi + 0x34]
        test   eax, eax
        je     loc_5_23D7
        cmp    dword ptr [eax + 0xc], 0x7fff
        ja     loc_5_23D7
        cmp    dword ptr [edi + 0x3c], esi
        je     loc_5_23C1
        mov    eax, 0x800000
        mov    ecx, edi
        push   eax
        push   esi
        push   eax
        push   esi
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_23D7
loc_5_23C1:
        push   edi
        call   sub_5_11DE
        add    esp, 4
        test   eax, eax
        je     loc_5_23D7
        push   edi
        call   sub_5_9014
        add    esp, 4
loc_5_23D7:
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
sub_5_23DF:
        cmp    dword ptr [esp + 4], 0
        jne    loc_5_2412
        cmp    dword ptr [0], 0              ; -> loc_7_0210
        jne    loc_5_240A
        mov    edx, 0                        ; -> loc_5_232E
        mov    ecx, 1
        inc    dword ptr [0]                 ; -> obj7:0x008C
        call   sub_5_CC30
        dec    dword ptr [0]                 ; -> obj7:0x008C
loc_5_240A:
        inc    dword ptr [0]                 ; -> loc_7_0210
        jmp    loc_5_2469
loc_5_2412:
        mov    eax, dword ptr [0]            ; -> loc_7_0210
        test   eax, eax
        je     loc_5_2469
        cmp    eax, 1
        jne    loc_5_2463
        push   0
        mov    ecx, dword ptr [0]            ; -> obj7:0x00A8
        and    byte ptr [1], 0xfd            ; -> obj7:0x00AD
        push   0x20
        call   sub_5_1906
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        push   eax
        call   sub_5_79CB
        add    esp, 4
        mov    ecx, 0x1a
        or     byte ptr [1], 2               ; -> obj7:0x00AD
        call   sub_5_231C
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        push   eax
        call   sub_5_79CB
        add    esp, 4
loc_5_2463:
        dec    dword ptr [0]                 ; -> loc_7_0210
loc_5_2469:
        ret    
sub_5_246A:
        mov    eax, dword ptr [ecx + 0x10]
        test   eax, eax
        je     loc_5_2487
loc_5_2471:
        mov    ecx, dword ptr [eax + 4]
        and    ch, 0x7f
        cmp    ecx, 4
        jne    loc_5_2480
        or     byte ptr [eax + 0x1c], 1
loc_5_2480:
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        jne    loc_5_2471
loc_5_2487:
        ret    
sub_5_2488:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, ecx
        mov    ecx, dword ptr [ecx + 0x38]
        test   ecx, ecx
        je     loc_5_24E1
        mov    edx, 2
        call   sub_5_14E6
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_5_250C
        mov    ecx, eax
        call   sub_5_246A
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, edi
        call   sub_5_0969
        test   eax, eax
        je     loc_5_250C
        mov    ecx, 6
        call   sub_5_099C
        test   eax, eax
        je     loc_5_24D4
        call   sub_5_09DB
        jmp    loc_5_250C
loc_5_24D4:
        call   sub_5_0A32
        lea    ecx, [ebp - 4]
        call   sub_5_1596
loc_5_24E1:
        add    edi, 0x30
        mov    esi, 7
loc_5_24E9:
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_5_2506
loc_5_24F2:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_246A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 4], ecx
        test   ecx, ecx
        jne    loc_5_24F2
loc_5_2506:
        add    edi, 4
        dec    esi
        jne    loc_5_24E9
loc_5_250C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_2512:
        mov    ecx, dword ptr [0]            ; -> loc_7_026C
        test   ecx, ecx
        jne    loc_5_2529
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [0]            ; -> loc_7_0268
        mov    dword ptr [eax], edx
        jmp    loc_5_2549
loc_5_2529:
        mov    eax, dword ptr [0]            ; -> loc_7_0268
        mov    edx, dword ptr [esp + 8]
        cmp    dword ptr [eax], edx
        jae    loc_5_253D
        mov    eax, 0xea
        jmp    loc_5_254B
loc_5_253D:
        push   edx
        mov    dword ptr [eax], edx
        mov    edx, dword ptr [esp + 8]
        call   sub_5_5B5D
loc_5_2549:
        xor    eax, eax
loc_5_254B:
        ret    
loc_5_254C:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 4]
        mov    ecx, esi
        call   sub_5_C3C4
        inc    eax
        push   eax
        push   esi
        call   sub_5_2512
        add    esp, 8
        pop    esi
        ret    
loc_5_2568:
        mov    eax, dword ptr [esp + 4]
        push   4
        add    eax, 0xc
        push   eax
        call   sub_5_2512
        add    esp, 8
        ret    
loc_5_257B:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        mov    ecx, dword ptr [eax + 8]
        push   4
        mov    dword ptr [esp + 4], ecx
        lea    ecx, [esp + 4]
        push   ecx
        call   sub_5_2512
        add    esp, 0xc
        ret    
loc_5_2599:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        lea    edx, [esp]
        push   esi
        push   edi
        mov    ecx, dword ptr [eax + 0x38]
        call   sub_5_8BD1
        mov    edi, eax
        test   edi, edi
        jne    loc_5_25BB
        mov    eax, 0xe
        jmp    loc_5_25E0
loc_5_25BB:
        push   dword ptr [esp + 8]
        push   edi
        call   sub_5_2512
        add    esp, 8
        mov    esi, eax
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_25DE
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_25DE:
        mov    eax, esi
loc_5_25E0:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_5_25E6:
        mov    eax, dword ptr [esp + 4]
        push   4
        add    eax, 0x24
        push   eax
        call   sub_5_2512
        add    esp, 8
        ret    
loc_5_25F9:
        mov    eax, dword ptr [esp + 4]
        push   4
        add    eax, 0x28
        push   eax
        call   sub_5_2512
        add    esp, 8
        ret    
loc_5_260C:
        mov    eax, dword ptr [esp + 4]
        push   4
        add    eax, 0x20
        push   eax
        call   sub_5_2512
        add    esp, 8
        ret    
loc_5_261F:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 0x3fa
        je     loc_5_2650
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [0], eax            ; -> loc_7_0268
        mov    dword ptr [0], ecx            ; -> loc_7_026C
        mov    eax, dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   dword ptr [eax + 4]
loc_5_2650:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_2654:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x14
        push   esi
        mov    dword ptr [ebp - 0x10], eax
        push   edi
        mov    esi, ecx
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0x14], 0     ; -> loc_5_261F
        mov    dword ptr [ebp - 4], 1
        test   byte ptr [1], 1               ; -> obj7:0x00AD
        je     loc_5_26CE
        mov    edx, 0                        ; -> obj7:0x0320
        mov    edi, 0                        ; -> obj7:0x00CC
        mov    ecx, edi
        call   sub_5_C486
        mov    edx, 0                        ; -> obj7:0x0334
        mov    ecx, edi
        call   sub_5_C44E
        push   esi
        mov    ecx, edi
        push   0                             ; -> obj7:0x0338
        call   sub_5_C3C4
        add    eax, edi
        push   eax
        call   sub_5_D0FC
        add    esp, 0xc
        lea    eax, [esi + 0x70]
        lea    ecx, [ebp - 0x14]
        push   eax
        push   7
        push   0                             ; -> loc_7_0270
        push   ecx
        push   esi
        push   edi
        call   sub_5_D144
        add    esp, 0x18
loc_5_26CE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_26D4:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj7:0x00AC
        test   ah, 1
        mov    esi, ecx
        je     loc_5_2705
        test   al, 8
        jne    loc_5_2705
        mov    eax, dword ptr [esi + 0x70]
        test   eax, eax
        je     loc_5_2705
        push   0                             ; -> obj7:0x033C
        push   eax
        call   sub_5_D14C
        add    esp, 8
        push   dword ptr [esi + 0x70]
        call   sub_5_D0E4
        add    esp, 4
loc_5_2705:
        pop    esi
        ret    
sub_5_2707:
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        push   ebp
        mov    edi, ecx
        call   sub_5_09DB
        mov    edi, dword ptr [edi]
        test   edi, edi
        je     loc_5_273B
        xor    ebx, ebx
loc_5_271C:
        mov    ecx, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 0xc]
        test   edx, edx
        mov    dword ptr [ecx + 0x48], ebx
        je     loc_5_2735
        mov    dword ptr [ecx + 0x48], edx
        call   sub_5_0969
        test   eax, eax
        je     loc_5_2790
loc_5_2735:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_5_271C
loc_5_273B:
        xor    edx, edx
        xor    edi, edi
        mov    ecx, dword ptr [0]            ; -> obj7:0x00B0
        add    ecx, 0x24
        cmp    ecx, dword ptr [0]            ; -> obj7:0x00B4
        je     loc_5_2774
loc_5_2750:
        mov    eax, dword ptr [ecx + 0x20]
        cmp    dword ptr [eax], 0
        je     loc_5_2769
        test   byte ptr [ecx + 0xb], 0x80
        jne    loc_5_2769
        mov    eax, dword ptr [ecx + 0xc]
        cmp    edx, eax
        ja     loc_5_2769
        mov    edi, ecx
        mov    edx, eax
loc_5_2769:
        add    ecx, 0x24
        cmp    ecx, dword ptr [0]            ; -> obj7:0x00B4
        jne    loc_5_2750
loc_5_2774:
        test   edi, edi
        je     loc_5_2797
        mov    edx, esi
        mov    ecx, edi
        push   dword ptr [edi + 0x20]
        call   sub_5_0B16
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_279B
        or     byte ptr [edi + 0xb], 0x80
        jmp    loc_5_273B
loc_5_2790:
        mov    eax, 2
        jmp    loc_5_27C2
loc_5_2797:
        xor    eax, eax
        jmp    loc_5_27C2
loc_5_279B:
        mov    ebp, dword ptr [edi]
        mov    esi, dword ptr [edi + 0x1c]
loc_5_27A0:
        mov    edx, dword ptr [edi + 0x1c]
        mov    ecx, esi
        call   sub_5_1D59
        test   eax, eax
        je     loc_5_27B1
        inc    dword ptr [edi + 0xc]
loc_5_27B1:
        add    edi, 0x24
        cmp    dword ptr [0], edi            ; -> obj7:0x00B4
        je     loc_5_27C0
        cmp    dword ptr [edi], ebp
        je     loc_5_27A0
loc_5_27C0:
        mov    eax, ebx
loc_5_27C2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_27C7:
        push   ebx
        push   esi
        mov    ebx, dword ptr [ecx + 8]
        push   edi
        push   ebp
        mov    esi, ecx
        lea    edi, [ebx + 0x38]
        mov    ecx, edi
        call   sub_5_1596
        mov    eax, dword ptr [esi + 0xc]
        push   0x90000000
        push   0x800
        mov    ecx, ebx
        mov    dword ptr [edi], eax
        call   sub_5_1906
        mov    esi, dword ptr [esi + 0xc]
        mov    dword ptr [esi + 8], 2
        add    esi, 0x10
        cmp    dword ptr [esi], 0
        je     loc_5_2848
loc_5_2802:
        mov    eax, dword ptr [esi]
        mov    ebx, dword ptr [eax + 8]
        test   ebx, ebx
        je     loc_5_283E
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        mov    ebp, dword ptr [eax + 0x10]
        jne    loc_5_2822
        push   0
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_2822:
        lea    ecx, [ebx + 0x1c]
        mov    edx, dword ptr [edi]
        push   dword ptr [ebx + 0x14]
        push   ecx
        mov    ecx, esi
        mov    dword ptr [esi], ebp
        push   dword ptr [ebx + 4]
        call   sub_5_1711
        test   eax, eax
        je     loc_5_2852
        mov    dword ptr [eax + 8], ebx
loc_5_283E:
        mov    esi, dword ptr [esi]
        add    esi, 0x10
        cmp    dword ptr [esi], 0
        jne    loc_5_2802
loc_5_2848:
        mov    eax, 1
loc_5_284D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_5_2852:
        xor    eax, eax
        jmp    loc_5_284D
sub_5_2856:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     loc_5_2867
loc_5_285C:
        cmp    dword ptr [eax + 8], ecx
        je     loc_5_286A
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_5_285C
loc_5_2867:
        xor    eax, eax
loc_5_2869:
        ret    
loc_5_286A:
        mov    eax, 1
        jmp    loc_5_2869
loc_5_2871:
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    ecx, esi
        push   0x200000
        call   sub_5_1923
        mov    eax, 0x8000000
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_28CE
        cmp    dword ptr [0], edi            ; -> obj7:0x0360
        jne    loc_5_28BC
        mov    edx, dword ptr [0]            ; -> obj7:0x00C4
        mov    ecx, esi
        call   sub_5_2856
        test   eax, eax
        jne    loc_5_28BC
        mov    ecx, esi
        call   sub_5_0E91
        jmp    loc_5_28CE
loc_5_28BC:
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_0FD2
        push   esi
        call   sub_5_122C
        add    esp, 4
loc_5_28CE:
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
sub_5_28D6:
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj7:0x00C8
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [eax]
        test   esi, esi
        je     loc_5_2929
        xor    edi, edi
        mov    ebx, 0x800000
loc_5_28EC:
        mov    ebp, dword ptr [esi + 8]
        push   ebx
        push   edi
        mov    ecx, ebp
        push   ebx
        mov    dword ptr [ebp + 0x48], edi
        push   edi
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_2923
        mov    eax, dword ptr [ebp + 0x38]
        test   eax, eax
        je     loc_5_2923
        mov    ecx, dword ptr [eax + 0x10]
        test   ecx, ecx
        je     loc_5_2923
loc_5_290F:
        mov    eax, dword ptr [ecx + 8]
        test   eax, eax
        je     loc_5_291C
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], eax
loc_5_291C:
        mov    ecx, dword ptr [ecx + 0x10]
        test   ecx, ecx
        jne    loc_5_290F
loc_5_2923:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_5_28EC
loc_5_2929:
        mov    edx, 0                        ; -> loc_5_2871
        mov    ecx, 1
        call   sub_5_CC30
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_293D:
        push   esi
        call   sub_5_0A32
        call   sub_5_28D6
        mov    eax, dword ptr [0]            ; -> obj7:0x00C4
        mov    esi, dword ptr [eax]
        test   esi, esi
        je     loc_5_2960
loc_5_2953:
        mov    ecx, esi
        call   sub_5_27C7
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_5_2953
loc_5_2960:
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C4
        call   sub_5_684A
        pop    esi
        ret    
sub_5_296D:
        push   esi
        push   edi
        mov    esi, ecx
        test   esi, esi
        je     loc_5_2990
        mov    edi, dword ptr [esi]
        test   edi, edi
        je     loc_5_2989
loc_5_297B:
        lea    ecx, [edi + 0xc]
        call   sub_5_1596
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_5_297B
loc_5_2989:
        mov    ecx, esi
        call   sub_5_684A
loc_5_2990:
        pop    edi
        pop    esi
        ret    
sub_5_2993:
        cmp    dword ptr [0], 0              ; -> obj7:0x0348
        push   ebx
        push   esi
        push   edi
        push   ebp
        je     loc_5_29B3
        cmp    dword ptr [0], 0              ; -> obj7:0x034C
        jne    loc_5_29B3
        mov    eax, 0x13
        jmp    loc_5_2A62
loc_5_29B3:
        xor    edx, edx
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C8
        call   sub_5_2707
        test   eax, eax
        jne    loc_5_2A62
        call   sub_5_675F
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_29DD
        mov    eax, 2
        jmp    loc_5_2A62
loc_5_29DD:
        mov    eax, dword ptr [0]            ; -> obj7:0x00C8
        mov    ebp, dword ptr [eax]
        test   ebp, ebp
        je     loc_5_2A19
loc_5_29E8:
        mov    edi, dword ptr [ebp + 0xc]
        test   edi, edi
        je     loc_5_2A12
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_5_6791
        test   eax, eax
        je     loc_5_2A67
        mov    esi, dword ptr [ebx]
        mov    edx, 0xbeefdead
        mov    ecx, edi
        call   sub_5_14E6
        mov    dword ptr [esi + 0xc], eax
        test   eax, eax
        je     loc_5_2A67
loc_5_2A12:
        mov    ebp, dword ptr [ebp]
        test   ebp, ebp
        jne    loc_5_29E8
loc_5_2A19:
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C4
        call   sub_5_296D
        mov    ecx, dword ptr [0]            ; -> obj7:0x0350
        xor    eax, eax
        mov    edx, 1
        mov    dword ptr [0], ebx            ; -> obj7:0x00C4
        mov    esi, edx
        mov    dword ptr [0], ecx            ; -> obj7:0x0354
        cmp    eax, dword ptr [0]            ; -> obj7:0x0348
        lea    eax, [ecx - 1]
        adc    esi, -1
        cmp    eax, edx
        sbb    eax, eax
        neg    eax
        and    esi, eax
        cmp    ecx, edx
        sbb    eax, eax
        neg    eax
        or     esi, eax
        cmp    esi, edx
        sbb    eax, eax
        and    eax, 0x13
loc_5_2A62:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_5_2A67:
        mov    ecx, ebx
        call   sub_5_296D
        mov    eax, 2
        jmp    loc_5_2A62
sub_5_2A75:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        test   esi, esi
        jne    loc_5_2A8C
        call   sub_5_2993
        jmp    loc_5_2B9F
loc_5_2A8C:
        mov    edi, dword ptr [esi + 8]
        xor    ebx, ebx
        push   ebx
        mov    eax, 0x200000
        push   eax
        mov    ecx, edi
        push   ebx
        mov    dword ptr [esi + 0xc], ebx
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_2ACB
        mov    ecx, edi
        call   sub_5_05D3
        test   eax, eax
        je     loc_5_2ACB
        mov    ecx, dword ptr [esi]
        call   sub_5_2A75
        test   eax, eax
        je     loc_5_2B9F
        cmp    eax, 2
        je     loc_5_2B9F
loc_5_2ACB:
        mov    ebx, dword ptr [edi + 0x34]
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esp + 0x14], eax
loc_5_2AD5:
        mov    ecx, dword ptr [0]            ; -> obj7:0x035C
        call   sub_5_C7F6
        test   eax, eax
        jne    loc_5_2B86
        mov    ebp, dword ptr [ebx + 0xc]
        cmp    dword ptr [0], ebp            ; -> obj7:0x0344
        jb     loc_5_2B8D
        mov    eax, dword ptr [0]            ; -> obj7:0x0350
        sub    eax, dword ptr [esp + 0x14]
        add    eax, ebp
        cmp    dword ptr [0], eax            ; -> obj7:0x0354
        jbe    loc_5_2B94
        push   0x100000
        cmp    ebp, dword ptr [0]            ; -> obj7:0x0348
        sbb    ecx, ecx
        push   0
        inc    ecx
        mov    dword ptr [0], eax            ; -> obj7:0x0350
        add    dword ptr [0], ecx            ; -> obj7:0x034C
        mov    dword ptr [esi + 0xc], ebx
        mov    ecx, edi
        call   sub_5_1906
        mov    ecx, dword ptr [esi]
        call   sub_5_2A75
        push   0x100000
        mov    ecx, edi
        mov    dword ptr [esp + 0x14], eax
        push   0
        call   sub_5_1923
        cmp    ebp, dword ptr [0]            ; -> obj7:0x0348
        sbb    eax, eax
        inc    eax
        sub    dword ptr [0], eax            ; -> obj7:0x034C
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, ebp
        add    dword ptr [0], eax            ; -> obj7:0x0350
        cmp    dword ptr [esp + 0x10], 0
        je     loc_5_2B9B
        cmp    dword ptr [esp + 0x10], 2
        je     loc_5_2B9B
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    loc_5_2AD5
        mov    eax, 0x13
        jmp    loc_5_2B9F
loc_5_2B86:
        mov    eax, 0x13
        jmp    loc_5_2B9F
loc_5_2B8D:
        mov    eax, 0x13
        jmp    loc_5_2B9F
loc_5_2B94:
        mov    eax, 0x13
        jmp    loc_5_2B9F
loc_5_2B9B:
        mov    eax, dword ptr [esp + 0x10]
loc_5_2B9F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_5_2BA7:
        push   edi
        mov    eax, 0x800000
        mov    edi, ecx
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_2BE0
        mov    eax, dword ptr [edi + 0x38]
        test   eax, eax
        je     loc_5_2BE0
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        je     loc_5_2BE0
loc_5_2BCC:
        cmp    dword ptr [eax + 8], 0
        je     loc_5_2BD9
        mov    dword ptr [eax + 4], 0x8000
loc_5_2BD9:
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        jne    loc_5_2BCC
loc_5_2BE0:
        pop    edi
        ret    
sub_5_2BE2:
        push   esi
        push   edi
        mov    edi, dword ptr [ecx + 8]
        mov    esi, ecx
        cmp    dword ptr [0], edi            ; -> obj7:0x0340
        je     loc_5_2C03
        push   0
        mov    eax, 0x200000
        push   eax
        mov    ecx, edi
        push   0
        push   eax
        call   sub_5_18E3
loc_5_2C03:
        mov    ecx, edi
        call   sub_5_1D31
        pop    edi
        mov    dword ptr [esi + 0x10], eax
        pop    esi
        ret    
loc_5_2C10:
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    ecx, esi
        push   0x2008
        push   edi
        push   0x23408
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_2C99
        mov    eax, dword ptr [esi + 0x34]
        test   eax, eax
        je     loc_5_2C99
        cmp    dword ptr [eax + 0xc], 0x7fff
        ja     loc_5_2C99
        cmp    dword ptr [esi + 0x38], edi
        je     loc_5_2C99
        cmp    dword ptr [esi + 0x40], edi
        jne    loc_5_2C99
        cmp    dword ptr [esi + 0x3c], edi
        je     loc_5_2C62
        mov    eax, 0x800000
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_2C99
loc_5_2C62:
        mov    eax, 0x8000000
        mov    ecx, esi
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_2C85
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_0FA7
        test   eax, eax
        je     loc_5_2C99
loc_5_2C85:
        mov    ecx, esi
        call   sub_5_2BA7
        mov    eax, 1
        inc    dword ptr [0]                 ; -> obj7:0x0358
        jmp    loc_5_2C9B
loc_5_2C99:
        xor    eax, eax
loc_5_2C9B:
        pop    edi
        pop    esi
        ret    
sub_5_2C9E:
        test   ecx, ecx
        je     loc_5_2CBF
        mov    edx, ecx
        mov    ecx, dword ptr [0]            ; -> obj7:0x0340
        call   sub_5_731F
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C4
        call   sub_5_296D
        call   sub_5_28D6
loc_5_2CBF:
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C8
        call   sub_5_684A
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C4
        call   sub_5_296D
        call   sub_5_09DB
        and    byte ptr [1], 0xbf            ; -> obj7:0x00AD
        dec    dword ptr [0]                 ; -> obj7:0x008C
        ret    
sub_5_2CE8:
        xor    eax, eax
        mov    dword ptr [0], ecx            ; -> obj7:0x0348
        mov    dword ptr [0], edx            ; -> obj7:0x0344
        mov    dword ptr [0], eax            ; -> obj7:0x0350
        mov    dword ptr [0], 0xffffffff     ; -> obj7:0x0354
        mov    dword ptr [0], eax            ; -> obj7:0x034C
        xor    edx, edx
        VxDCall  VTD.VTD_Get_Real_Time           ; cd2007000500
        mov    ecx, 0                        ; -> obj7:0x035C
        mov    edx, 0x4c4b40
        mov    dword ptr [0], eax            ; -> obj7:0x035C
        call   sub_5_C7CC
        mov    eax, dword ptr [0]            ; -> obj7:0x00C8
        mov    ecx, dword ptr [eax]
        call   sub_5_2A75
        test   eax, eax
        jne    loc_5_2D3F
        call   sub_5_293D
        xor    ecx, ecx
        jmp    loc_5_2D74
loc_5_2D3F:
        cmp    dword ptr [0], -1             ; -> obj7:0x0354
        jae    loc_5_2D65
        mov    edx, 1
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C4
        call   sub_5_2707
        test   eax, eax
        jne    loc_5_2D65
        call   sub_5_293D
        xor    ecx, ecx
        jmp    loc_5_2D74
loc_5_2D65:
        cmp    eax, 2
        mov    eax, 0
        jne    loc_5_2D7E
        mov    ecx, 3
loc_5_2D74:
        call   sub_5_2C9E
        mov    eax, 1
loc_5_2D7E:
        ret    
sub_5_2D7F:
        push   esi
        mov    edx, 0x20000
        push   edi
        mov    esi, ecx
        push   0
        push   edx
        push   0
        push   edx
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_2DA3
        mov    ecx, esi
        call   sub_5_6238
        jmp    loc_5_2F35
loc_5_2DA3:
        xor    edi, edi
        mov    eax, 0x800000
        push   eax
        mov    ecx, esi
        push   edi
        mov    dword ptr [0], edi            ; -> obj7:0x0360
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_2DD6
        cmp    dword ptr [esi + 0x38], edi
        je     loc_5_2DD6
        push   0x80000000
        mov    ecx, esi
        push   edi
        call   sub_5_1906
        jmp    loc_5_2F35
loc_5_2DD6:
        mov    eax, dword ptr [esi + 0x34]
        test   eax, eax
        jne    loc_5_2DF0
        push   0x80000000
        mov    ecx, esi
        push   0
        call   sub_5_1906
        jmp    loc_5_2F35
loc_5_2DF0:
        test   byte ptr [1], 0x40            ; -> obj7:0x00AD
        je     loc_5_2E03
        call   sub_5_7B5F
        jmp    loc_5_2F35
loc_5_2E03:
        inc    dword ptr [0]                 ; -> obj7:0x008C
        mov    dword ptr [0], esi            ; -> obj7:0x0340
        or     byte ptr [1], 0x40            ; -> obj7:0x00AD
        cmp    dword ptr [eax + 0xc], 0x7fff
        jbe    loc_5_2E29
        mov    ecx, 0x1b
        jmp    loc_5_2F30
loc_5_2E29:
        mov    edx, esi
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C8
        call   sub_5_6791
        test   eax, eax
        jne    loc_5_2E44
        mov    ecx, 3
        jmp    loc_5_2F30
loc_5_2E44:
        mov    ecx, dword ptr [0]            ; -> obj7:0x00B0
loc_5_2E4A:
        add    ecx, 0x24
        cmp    ecx, dword ptr [0]            ; -> obj7:0x00B4
        je     loc_5_2E6F
        test   byte ptr [ecx + 8], 4
        je     loc_5_2E62
        mov    eax, 0x40000000
        jmp    loc_5_2E67
loc_5_2E62:
        mov    eax, 0x80000000
loc_5_2E67:
        sub    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [ecx + 0xc], eax
        jmp    loc_5_2E4A
loc_5_2E6F:
        mov    ecx, esi
        call   sub_5_2BA7
        push   0
        mov    ecx, esi
        push   0x200000
        call   sub_5_1906
        mov    edx, 0x3fff
        xor    ecx, ecx
        call   sub_5_2CE8
        test   eax, eax
        jne    loc_5_2F35
        mov    eax, dword ptr [0]            ; -> obj7:0x00C8
        mov    ecx, dword ptr [eax]
        call   sub_5_2BE2
        mov    edx, 0                        ; -> loc_5_2C10
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C8
        mov    dword ptr [0], 0              ; -> obj7:0x0358
        call   sub_5_6812
        test   eax, eax
        jne    loc_5_2EC9
        mov    ecx, 3
        jmp    loc_5_2F30
loc_5_2EC9:
        cmp    dword ptr [0], 0              ; -> obj7:0x0358
        je     loc_5_2F0C
        mov    eax, dword ptr [0]            ; -> obj7:0x00C8
        mov    esi, dword ptr [eax]
        test   esi, esi
        je     loc_5_2EEA
loc_5_2EDD:
        mov    ecx, esi
        call   sub_5_2BE2
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_5_2EDD
loc_5_2EEA:
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C8
        call   sub_5_692A
        mov    edx, 0x3fff
        xor    ecx, ecx
        call   sub_5_2CE8
        test   eax, eax
        je     loc_5_2F0C
        mov    eax, 1
        jmp    loc_5_2F37
loc_5_2F0C:
        mov    edx, 0x7fff
        mov    ecx, 0x4000
        call   sub_5_2CE8
        test   eax, eax
        je     loc_5_2F2B
        cmp    dword ptr [0], 1              ; -> obj7:0x0358
        sbb    eax, eax
        inc    eax
        jmp    loc_5_2F37
loc_5_2F2B:
        mov    ecx, 0xc
loc_5_2F30:
        call   sub_5_2C9E
loc_5_2F35:
        xor    eax, eax
loc_5_2F37:
        pop    edi
        pop    esi
        ret    
sub_5_2F3A:
        mov    dword ptr [0], 0              ; -> obj7:0x0360
        test   byte ptr [1], 0x40            ; -> obj7:0x00AD
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        je     loc_5_2F5E
        call   sub_5_7B5F
        jmp    loc_5_309A
loc_5_2F5E:
        mov    edx, esi
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C8
        inc    dword ptr [0]                 ; -> obj7:0x008C
        mov    dword ptr [0], esi            ; -> obj7:0x0340
        or     byte ptr [1], 0x40            ; -> obj7:0x00AD
        call   sub_5_6791
        test   eax, eax
        jne    loc_5_2F93
        mov    ecx, 3
        call   sub_5_2C9E
        xor    eax, eax
        jmp    loc_5_309F
loc_5_2F93:
        mov    ecx, dword ptr [0]            ; -> obj7:0x00B0
loc_5_2F99:
        add    ecx, 0x24
        cmp    dword ptr [0], ecx            ; -> obj7:0x00B4
        je     loc_5_2FBE
        test   byte ptr [ecx + 8], 4
        je     loc_5_2FB1
        mov    eax, 0x40000000
        jmp    loc_5_2FB6
loc_5_2FB1:
        mov    eax, 0x80000000
loc_5_2FB6:
        sub    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [ecx + 0xc], eax
        jmp    loc_5_2F99
loc_5_2FBE:
        mov    edx, 0                        ; -> loc_5_2C10
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C8
        mov    dword ptr [0], 0              ; -> obj7:0x0358
        call   sub_5_6812
        test   eax, eax
        jne    loc_5_2FED
        mov    ecx, 3
        call   sub_5_2C9E
        xor    eax, eax
        jmp    loc_5_309F
loc_5_2FED:
        cmp    dword ptr [0], 0              ; -> obj7:0x0358
        jne    loc_5_3004
        xor    ecx, ecx
        call   sub_5_2C9E
        xor    eax, eax
        jmp    loc_5_309F
loc_5_3004:
        mov    ecx, dword ptr [0]            ; -> obj7:0x00C8
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     loc_5_302C
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
loc_5_3016:
        cmp    dword ptr [eax + 8], esi
        jne    loc_5_3020
        mov    dword ptr [eax + 0xc], edi
        jmp    loc_5_3026
loc_5_3020:
        mov    ebx, dword ptr [edx + 0x38]
        mov    dword ptr [eax + 0xc], ebx
loc_5_3026:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_5_3016
loc_5_302C:
        mov    edx, 1
        call   sub_5_2707
        test   eax, eax
        je     loc_5_304A
        call   sub_5_28D6
        xor    ecx, ecx
        call   sub_5_2C9E
        xor    eax, eax
        jmp    loc_5_309F
loc_5_304A:
        mov    eax, dword ptr [0]            ; -> obj7:0x00C8
        mov    dword ptr [0], 1              ; -> obj7:0x0360
        mov    edi, dword ptr [eax]
        test   edi, edi
        je     loc_5_3089
loc_5_305F:
        mov    esi, dword ptr [edi + 8]
        cmp    dword ptr [0], esi            ; -> obj7:0x0340
        je     loc_5_3083
        push   0x90000000
        mov    ecx, esi
        push   0x800
        call   sub_5_1923
        lea    ecx, [esi + 0x38]
        call   sub_5_15ED
loc_5_3083:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_5_305F
loc_5_3089:
        call   sub_5_0A32
        call   sub_5_28D6
        xor    ecx, ecx
        call   sub_5_2C9E
loc_5_309A:
        mov    eax, 1
loc_5_309F:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_30A3:
        movzx  eax, cx
        push   eax
        push   0                             ; -> obj7:0x04A8
        push   edx
        call   sub_5_D0FC
        add    esp, 0xc
        ret    
sub_5_30B6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x9c
        mov    dword ptr [ebp - 4], 0x4f
        push   ebx
        push   esi
        push   edi
        mov    esi, 0                        ; -> obj7:0x04AC
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edi, ebx
        movsb  byte ptr es:[edi], byte ptr [esi]
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_5_3175
        mov    esi, 0                        ; -> obj7:0x04B0
        lea    edi, [ebp - 0x9c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        lea    edx, [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 8]
        lea    edi, [ebp - 0xc]
        call   sub_5_30A3
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0xffffffff
        lea    edi, [ebp - 0x9c]
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x5c]
        push   eax
        lea    edx, [ebp - 0x9c]
        push   ecx
        push   0
        push   0
        push   edx
        push   dword ptr [ebp + 0x10]
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_3175
        xor    edi, edi
        lea    esi, [ebx + 1]
        cmp    byte ptr [ebp - 0x5c], 0
        mov    al, byte ptr [ebp - 0x5c]
        mov    byte ptr [ebx], al
        lea    eax, [ebp - 0x5b]
        je     loc_5_3171
loc_5_315F:
        cmp    edi, 0x50
        jge    loc_5_3171
        inc    edi
        mov    ecx, esi
        inc    esi
        mov    dl, byte ptr [eax]
        inc    eax
        mov    byte ptr [ecx], dl
        test   dl, dl
        jne    loc_5_315F
loc_5_3171:
        mov    byte ptr [ebx + 0x4f], 0
loc_5_3175:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_317C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [0]            ; -> obj7:0x048C
        xor    edi, edi
        add    edx, 0                        ; -> obj7:0x038C
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   edi
        mov    ecx, esi
        mov    dword ptr [ebp - 4], edi
        call   sub_5_8613
        test   eax, eax
        je     loc_5_31EC
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj7:0x038C
        push   0x80000002
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_31EC
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0xc], 4
        push   eax
        push   ecx
        push   edi
        push   edi
        push   0                             ; -> loc_7_04C0
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 0x18
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
loc_5_31EC:
        test   byte ptr [ebp - 4], 2
        je     loc_5_31FD
        push   esi
        call   sub_5_9014
        add    esp, 4
        jmp    loc_5_325E
loc_5_31FD:
        mov    edi, 0x40000000
        xor    ebx, ebx
        push   edi
        mov    ecx, esi
        push   ebx
        push   edi
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_322E
        test   byte ptr [ebp - 4], 1
        jne    loc_5_322E
        push   ebx
        push   esi
        call   _CONFIGMG_Enable_DevNode
        add    esp, 8
        mov    ecx, esi
        push   edi
        push   ebx
        call   sub_5_1923
        jmp    loc_5_325E
loc_5_322E:
        mov    edi, 0x40000000
        xor    ebx, ebx
        push   edi
        mov    ecx, esi
        push   ebx
        push   edi
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_325E
        test   byte ptr [ebp - 4], 1
        je     loc_5_325E
        push   1
        push   esi
        call   _CONFIGMG_Disable_DevNode
        add    esp, 8
        mov    ecx, esi
        push   edi
        push   ebx
        call   sub_5_1906
loc_5_325E:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_326A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x118
        push   ebx
        lea    edx, [ebp - 0xc]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], 5
        call   sub_5_30A3
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> loc_7_04D0
        push   0x80000002
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_32E2
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0x18]
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> loc_7_04FC
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_32E2
        push   5
        lea    eax, [ebp - 0xc]
        push   eax
        push   1
        push   0
        push   0                             ; -> loc_7_050C
        push   dword ptr [ebp - 4]
        call   sub_5_D0EC
        add    esp, 0x18
        test   eax, eax
        je     loc_5_32E7
loc_5_32E2:
        mov    ebx, 0x1d
loc_5_32E7:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    edx, 0                        ; -> loc_7_051C
        lea    ecx, [ebp - 0x118]
        call   sub_5_C486
        lea    edx, [ebp - 0xc]
        lea    ecx, [ebp - 0x118]
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x118]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_D164
        add    esp, 0xc
        push   0x80000005
        lea    edi, [ebp - 0xc]
        push   dword ptr [ebp - 4]
        call   sub_5_D15C
        add    esp, 8
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    edx, 0xffffffff
        mov    ecx, edx
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, 7                        ; -> obj7:0x0393
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0                        ; -> obj7:0x0524
        mov    edi, 0                        ; -> obj7:0x038C
        mov    ecx, edx
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        add    edi, edx
        mov    ecx, edx
        movsw  word ptr es:[edi], word ptr [esi]
        mov    edi, 0                        ; -> obj7:0x038C
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, edx
        mov    edx, 0                        ; -> loc_5_317C
        mov    dword ptr [0], ecx            ; -> obj7:0x048C
        xor    ecx, ecx
        call   sub_5_CC30
        xor    eax, eax
        lea    ecx, [ebp - 0x18]
        mov    esi, 4
loc_5_33A9:
        shl    eax, 4
        mov    dl, byte ptr [ecx]
        movsx  edi, dl
        cmp    dl, 0x39
        mov    edx, 0x37
        jg     loc_5_33C0
        mov    edx, 0x30
loc_5_33C0:
        sub    eax, edx
        inc    ecx
        add    eax, edi
        dec    esi
        jne    loc_5_33A9
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> obj1:0x0204
        je     loc_5_341D
        push   esi
        call   _CONFIGMG_Lock
loc_5_33D8:
        add    esp, 4
        cmp    dword ptr [0], esi            ; -> obj7:0x0494
        je     loc_5_33FA
        push   esi
        mov    ecx, 0                        ; -> sub_5_23DF
        push   esi
        mov    edx, 1
        mov    dword ptr [0], esi            ; -> obj7:0x0494
        call   sub_1_051C
loc_5_33FA:
        xor    esi, esi
        mov    ecx, 0                        ; -> sub_5_5FFE
        push   esi
        mov    edx, 0x18
        push   esi
        call   sub_1_051C
loc_5_340D:
        push   esi
        mov    ecx, 0                        ; -> _CONFIGMG_Unlock
        push   esi
        xor    edx, edx
        call   sub_1_051C
loc_5_341B:
        jmp    loc_5_3442
loc_5_341D:
        xor    eax, eax
        cmp    dword ptr [0], eax            ; -> obj7:0x0494
        je     loc_5_3436
        push   1
        mov    dword ptr [0], eax            ; -> obj7:0x0494
        call   sub_5_23DF
        add    esp, 4
loc_5_3436:
        push   0
        push   0x18
        call   sub_5_5FFE
        add    esp, 8
loc_5_3442:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_344B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        push   esi
        push   edi
        mov    esi, edx
        xor    edi, edi
        cmp    dword ptr [ebp + 8], edi
        jne    loc_5_34F3
        mov    ecx, esi
        call   sub_5_1BDC
        test   eax, eax
        jne    loc_5_35B6
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], edi
        mov    dword ptr [ebp - 0xc], 4
        push   ecx
        push   edi
        push   edi
        push   0                             ; -> obj7:0x0528
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        je     loc_5_34B9
        cmp    eax, 2
        je     loc_5_35B6
        cmp    eax, 0x3f4
        je     loc_5_35B6
        mov    edi, 0x1d
        jmp    loc_5_35B6
loc_5_34B9:
        test   byte ptr [ebp - 3], 0x10
        jne    loc_5_35B6
        push   4
        lea    eax, [ebp - 4]
        or     byte ptr [ebp - 3], 4
        push   eax
        push   3
        push   0
        push   0                             ; -> loc_7_0534
        push   dword ptr [ebp - 8]
        call   sub_5_D0EC
        add    esp, 0x18
        test   eax, eax
        je     loc_5_35B6
        mov    edi, 0x1d
        jmp    loc_5_35B6
loc_5_34F3:
        cmp    byte ptr [esi], 0
        je     loc_5_3504
        mov    edx, 0                        ; -> loc_7_0540
        mov    ecx, esi
        call   sub_5_C44E
loc_5_3504:
        mov    ecx, esi
        call   sub_5_C3C4
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x14], eax
loc_5_3515:
        test   edi, edi
        jne    loc_5_35B6
        mov    eax, dword ptr [ebp - 0x14]
        push   0x100
        add    eax, esi
        push   eax
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        call   sub_5_D16C
        add    esp, 0x10
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_359E
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_C430
        cmp    dword ptr [ebp + 8], 3
        jne    loc_5_3568
        push   0
        mov    eax, dword ptr [0]            ; -> obj7:0x0490
        push   0
        push   0
        push   0
        push   esi
        push   eax
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_35AB
loc_5_3568:
        lea    eax, [ebp - 0x10]
        push   eax
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        call   sub_5_D154
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_35A6
        mov    eax, dword ptr [ebp + 8]
        mov    edx, esi
        dec    eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        call   sub_5_344B
        push   dword ptr [ebp - 0x10]
        mov    edi, eax
        call   sub_5_D0E4
        add    esp, 4
        jmp    loc_5_35AB
loc_5_359E:
        cmp    ebx, 0x103
        je     loc_5_35AB
loc_5_35A6:
        mov    edi, 0x1d
loc_5_35AB:
        inc    dword ptr [ebp - 0xc]
        test   ebx, ebx
        je     loc_5_3515
loc_5_35B6:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_35C1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x204
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        push   0                             ; -> obj7:0x0490
        mov    edi, 0x80000002
        push   0                             ; -> obj7:0x0544
        push   edi
        call   sub_5_D154
        add    esp, 0xc
        mov    ebx, eax
        test   eax, eax
        jne    loc_5_3637
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj7:0x0580
        push   edi
        call   sub_5_D154
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_3629
        push   3
        lea    edx, [ebp - 0x204]
        mov    byte ptr [ebp - 0x204], 0
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_344B
        push   dword ptr [ebp - 4]
        mov    esi, eax
        call   sub_5_D0E4
        add    esp, 4
loc_5_3629:
        mov    eax, dword ptr [0]            ; -> obj7:0x0490
        push   eax
        call   sub_5_D0E4
        add    esp, 4
loc_5_3637:
        test   ebx, ebx
        je     loc_5_3640
        mov    esi, 0x1d
loc_5_3640:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_3649:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x158
        push   ebx
        mov    dword ptr [ebp - 4], edx
        push   esi
        lea    eax, [ebp - 8]
        push   edi
        mov    esi, ecx
        xor    edi, edi
        push   eax
        push   8
        mov    edx, 0                        ; -> loc_7_0588
        push   edi
        mov    ecx, 0                        ; -> obj7:0x0598
        mov    dword ptr [ebp - 8], 0xf
        call   sub_5_1240
        cmp    eax, 0x21
        jb     loc_5_36A6
        cmp    eax, 0x29
        ja     loc_5_36A6
        lea    edi, [eax - 0x20]
        xor    ecx, ecx
        mov    eax, dword ptr [esi]
        cmp    eax, ecx
        jle    loc_5_3703
        add    esi, 4
loc_5_3691:
        movzx  edx, word ptr [esi]
        cmp    edx, edi
        je     loc_5_36A2
        add    esi, 2
        inc    ecx
        cmp    ecx, eax
        jl     loc_5_3691
        jmp    loc_5_3703
loc_5_36A2:
        mov    edi, ecx
        jmp    loc_5_3703
loc_5_36A6:
        push   dword ptr [ebp + 8]
        mov    dx, word ptr [esi + edi*2 + 4]
        lea    ecx, [ebp - 0x58]
        push   ecx
        lea    ebx, [ebp - 0x158]
        push   edx
        call   sub_5_30B6
        add    esp, 0xc
        lea    ecx, [ebp - 0x58]
        push   ecx
        push   1
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 0x158]
        push   eax
        push   ecx
        call   sub_5_D0FC
        add    esp, 0xc
        push   1
        push   2
        call   sub_5_6710
        add    esp, 4
        mov    edx, ebx
        mov    ecx, eax
        call   sub_5_5ECA
        test   eax, eax
        jne    loc_5_3703
        inc    edi
        cmp    dword ptr [esi], edi
        jg     loc_5_36FF
        xor    edi, edi
loc_5_36FF:
        test   eax, eax
        je     loc_5_36A6
loc_5_3703:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_370E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xa0
        push   esi
        lea    eax, [ebp - 0xa0]
        push   edi
        mov    esi, edx
        mov    di, 1
        mov    edx, ecx
        mov    ecx, eax
        call   sub_5_C486
        lea    ecx, [ebp - 0xa0]
        call   sub_5_C430
loc_5_3739:
        push   esi
        lea    eax, [ebp - 0x50]
        push   eax
        push   edi
        call   sub_5_30B6
        add    esp, 0xc
        lea    ecx, [ebp - 0x50]
        call   sub_5_C430
        lea    eax, [ebp - 0xa0]
        lea    edx, [ebp - 0x50]
loc_5_3758:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    loc_5_3778
        or     cl, cl
        je     loc_5_3774
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    loc_5_3778
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    loc_5_3758
loc_5_3774:
        xor    eax, eax
        jmp    loc_5_377D
loc_5_3778:
        sbb    eax, eax
        sbb    eax, -1
loc_5_377D:
        test   eax, eax
        je     loc_5_3791
        inc    di
        cmp    di, 9
        jb     loc_5_3739
        xor    eax, eax
loc_5_378B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_3791:
        mov    eax, 1
        jmp    loc_5_378B
sub_5_3798:
        sub    esp, 0x54
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        push   ebp
        xor    ebp, ebp
        test   edx, edx
        jne    loc_5_3843
        push   3
        call   sub_5_6710
        add    esp, 4
        mov    edi, eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        mov    edx, dword ptr [esp + 0x68]
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [esp + 0x10], ecx
        mov    ecx, ebx
        call   sub_5_370E
        test   eax, eax
        je     loc_5_386A
loc_5_37F7:
        inc    ebp
        lea    eax, [esp + 0x14]
        push   ebp
        push   0                             ; -> obj7:0x05A8
        push   eax
        call   sub_5_D0FC
        lea    edi, [esp + 0x20]
        add    esp, 0xc
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [esp + 0x10]
        add    edi, ebx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [esp + 0x68]
        mov    ecx, ebx
        call   sub_5_370E
        test   eax, eax
        jne    loc_5_37F7
        jmp    loc_5_386A
loc_5_3843:
        push   4
        call   sub_5_6710
        add    esp, 4
        mov    esi, eax
loc_5_384F:
        inc    ebp
        push   ebp
        push   esi
        push   ebx
        call   sub_5_D0FC
        mov    edx, dword ptr [esp + 0x74]
        add    esp, 0xc
        mov    ecx, ebx
        call   sub_5_370E
        test   eax, eax
        jne    loc_5_384F
loc_5_386A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x54
        ret    4
sub_5_3874:
        sub    esp, 4
        test   ecx, ecx
        je     loc_5_38B6
        cmp    ecx, 0x8000
        je     loc_5_38CA
        cmp    ecx, 0x8001
        je     loc_5_38D6
loc_5_388B:
        cmp    ecx, 1
        mov    dword ptr [esp], ecx
        sbb    eax, eax
        lea    ecx, [esp]
        inc    eax
        push   ecx
        push   4
        mov    edx, 0                        ; -> loc_7_05B0
        push   0
        mov    ecx, 0                        ; -> loc_7_05C0
        mov    dword ptr [0], eax            ; -> obj7:0x049C
        call   sub_5_1240
loc_5_38B2:
        add    esp, 4
        ret    
loc_5_38B6:
        cmp    dword ptr [0], 1              ; -> obj7:0x0498
        je     loc_5_38B2
        cmp    dword ptr [0], 0              ; -> obj7:0x049C
        je     loc_5_38B2
        jmp    loc_5_388B
loc_5_38CA:
        mov    dword ptr [0], 1              ; -> obj7:0x0498
        jmp    loc_5_38B2
loc_5_38D6:
        mov    dword ptr [0], 0              ; -> obj7:0x0498
        jmp    loc_5_38B2
loc_5_38E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x12c
        mov    dword ptr [ebp - 0xc], 0x6e
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    dword ptr [ebp + 0x10], esi
        jne    loc_5_3907
        mov    dword ptr [ebp + 0xc], 1
        xor    edi, edi
        jmp    loc_5_3913
loc_5_3907:
        mov    edi, dword ptr [ebp + 8]
        test   edi, edi
        jne    loc_5_3913
        mov    edi, 0xfffffff0
loc_5_3913:
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> loc_7_05D0
        push   0x80000002
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_3C02
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x9c]
        push   eax
        xor    ebx, ebx
        push   ecx
        push   ebx
        push   ebx
        push   0                             ; -> loc_7_05FC
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_3BF7
        cmp    dword ptr [ebp - 0xc], 0x6e
        jne    loc_5_3BF7
        mov    ecx, 0x8000
        call   sub_5_3874
        mov    ecx, 3
        call   sub_5_3874
        cmp    word ptr [ebp - 0x9c], bx
        mov    dword ptr [ebp - 0x2c], ebx
        jbe    loc_5_39C6
loc_5_3982:
        movzx  eax, bx
        imul   eax, eax, 0xc
        cmp    dword ptr [ebp + eax - 0x9a], edi
        jne    loc_5_39BB
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp + eax - 0x96], ecx
        jne    loc_5_39BB
        mov    ax, word ptr [ebp + eax - 0x90]
        mov    ecx, dword ptr [ebp - 0x2c]
        mov    word ptr [ebp + ecx*2 - 0x28], ax
        mov    eax, 1
        mov    cl, bl
        inc    dword ptr [ebp - 0x2c]
        shl    eax, cl
        or     esi, eax
loc_5_39BB:
        inc    bx
        cmp    bx, word ptr [ebp - 0x9c]
        jb     loc_5_3982
loc_5_39C6:
        mov    eax, dword ptr [ebp - 0x2c]
        test   eax, eax
        je     loc_5_3A18
        cmp    eax, 1
        je     loc_5_3B94
        lea    ecx, [ebp - 0x2c]
        mov    edx, esi
        push   dword ptr [ebp - 8]
        call   sub_5_3649
        movzx  esi, word ptr [ebp + eax*2 - 0x28]
        lea    edx, [ebp - 4]
        mov    ecx, esi
        call   sub_5_44B6
        test   byte ptr [ebp - 4], 4
        je     loc_5_3A06
        and    byte ptr [ebp - 4], 0xfb
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        call   sub_5_4572
loc_5_3A06:
        test   dword ptr [ebp - 4], 0x80000003
        je     loc_5_3BC6
        jmp    loc_5_3BBF
loc_5_3A18:
        mov    si, 1
loc_5_3A1C:
        xor    ax, ax
        cmp    word ptr [ebp - 0x9c], ax
        jbe    loc_5_3A4A
loc_5_3A28:
        movzx  ecx, ax
        imul   ecx, ecx, 0xc
        cmp    word ptr [ebp + ecx - 0x90], si
        je     loc_5_3A43
        inc    ax
        cmp    word ptr [ebp - 0x9c], ax
        ja     loc_5_3A28
loc_5_3A43:
        cmp    word ptr [ebp - 0x9c], ax
loc_5_3A4A:
        je     loc_5_3A54
        inc    si
        cmp    si, 9
        jb     loc_5_3A1C
loc_5_3A54:
        cmp    word ptr [ebp - 0x9c], 9
        jb     loc_5_3A6D
        mov    word ptr [ebp - 0x9c], 9
        mov    ax, 8
        jmp    loc_5_3A7B
loc_5_3A6D:
        mov    ax, word ptr [ebp - 0x9c]
        inc    word ptr [ebp - 0x9c]
loc_5_3A7B:
        movzx  eax, ax
        imul   eax, eax, 0xc
        mov    ecx, dword ptr [ebp + 0xc]
        push   0x6e
        mov    dword ptr [ebp + eax - 0x9a], edi
        mov    dword ptr [ebp + eax - 0x96], ecx
        xor    ecx, ecx
        mov    word ptr [ebp + eax - 0x92], cx
        mov    word ptr [ebp + eax - 0x90], si
        lea    eax, [ebp - 0x9c]
        push   eax
        push   3
        push   ecx
        push   0                             ; -> loc_7_0600
        push   dword ptr [ebp - 8]
        call   sub_5_D0EC
        add    esp, 0x18
        test   eax, eax
        je     loc_5_3AE5
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        mov    ecx, 0x8001
        call   sub_5_3874
        xor    ecx, ecx
        call   sub_5_3874
        jmp    loc_5_3C02
loc_5_3AE5:
        mov    ax, si
        xor    di, di
loc_5_3AEB:
        mov    cx, ax
        movzx  ebx, di
        and    cx, 0xf
        cmp    cx, 0xa
        sbb    dl, dl
        shr    ax, 4
        and    dl, 0xf9
        inc    di
        add    dl, 0x37
        add    dl, cl
        lea    ecx, [ebp - 0x11]
        sub    ecx, ebx
        cmp    di, 4
        mov    byte ptr [ecx], dl
        jb     loc_5_3AEB
        xor    ebx, ebx
        mov    edx, 0                        ; -> loc_7_0604
        lea    ecx, [ebp - 0xdc]
        mov    byte ptr [ebp - 0x10], bl
        call   sub_5_C486
        lea    edx, [ebp - 0x14]
        lea    ecx, [ebp - 0xdc]
        call   sub_5_C44E
        push   dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 0x10], 1
        sbb    edx, edx
        lea    ecx, [ebp - 0x12c]
        inc    edx
        call   sub_5_3798
        lea    ecx, [ebp - 0x12c]
        call   sub_5_C3C4
        push   eax
        lea    ecx, [ebp - 0x12c]
        push   ecx
        lea    edx, [ebp - 0xdc]
        movzx  esi, si
        push   1
        push   ebx
        push   edx
        push   dword ptr [ebp - 8]
        call   sub_5_D0EC
        add    esp, 0x18
        mov    edx, 5
        mov    ecx, esi
        call   sub_5_4572
        or     byte ptr [0], 0x20            ; -> obj7:0x00AC
        call   sub_5_7B5F
        push   ebx
        jmp    loc_5_3BC8
loc_5_3B94:
        movzx  esi, word ptr [ebp - 0x28]
        lea    edx, [ebp - 4]
        mov    ecx, esi
        call   sub_5_44B6
        test   byte ptr [ebp - 4], 4
        je     loc_5_3BB6
        and    byte ptr [ebp - 4], 0xfb
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        call   sub_5_4572
loc_5_3BB6:
        test   dword ptr [ebp - 4], 0x80000003
        je     loc_5_3BC6
loc_5_3BBF:
        or     byte ptr [0], 0x20            ; -> obj7:0x00AC
loc_5_3BC6:
        push   0
loc_5_3BC8:
        push   esi
        call   _CONFIGMG_Set_HW_Prof
        add    esp, 8
        mov    esi, eax
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        mov    ecx, dword ptr [0]            ; -> obj7:0x00A8
        call   sub_1_0598
loc_5_3BE9:
        mov    ecx, 0x8001
        call   sub_5_3874
        mov    eax, esi
        jmp    loc_5_3C07
loc_5_3BF7:
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
loc_5_3C02:
        mov    eax, 0x1d
loc_5_3C07:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_3C0E:
        sub    esp, 0x100
        test   ecx, ecx
        push   esi
        je     loc_5_3C1D
        push   0xf
        jmp    loc_5_3C1F
loc_5_3C1D:
        push   0x11
loc_5_3C1F:
        call   sub_5_6710
        lea    ecx, [esp + 8]
        add    esp, 4
        push   eax
        push   ecx
        call   sub_5_D0FC
        lea    esi, [esp + 0xc]
        add    esp, 8
        push   0
        push   0x10
        call   sub_5_6710
        add    esp, 4
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_5EA8
        pop    esi
        add    esp, 0x100
        ret    
loc_5_3C56:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     loc_5_3D92
        lea    eax, [ebp - 4]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   eax, eax
        je     loc_5_3C8F
        test   byte ptr [ebp - 3], 4
        je     loc_5_3C8F
        mov    edi, 2
        jmp    loc_5_3D1F
loc_5_3C8F:
        mov    edx, 0                        ; -> loc_7_0614
        mov    ecx, 0                        ; -> obj7:0x00CC
        xor    edi, edi
        call   sub_5_C486
        mov    ecx, 0                        ; -> obj7:0x00CC
        mov    edx, dword ptr [esi + 4]
        xor    ebx, ebx
        call   sub_5_C44E
        cmp    dword ptr [0x24], ebx         ; -> obj7:0x0388
        jbe    loc_5_3D1F
        mov    dword ptr [ebp - 4], 0        ; -> obj7:0x0364
loc_5_3CBE:
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   0                             ; -> obj7:0x00CC
        push   dword ptr [ecx]
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_3D12
        lea    eax, [ebp - 0x14]
        lea    ecx, [ebp - 0xc]
        mov    dword ptr [ebp - 0x14], 4
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj7:0x061C
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 0x18
        mov    dword ptr [ebp - 0x10], eax
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_5_3D12
        or     edi, dword ptr [ebp - 0xc]
loc_5_3D12:
        add    dword ptr [ebp - 4], 4
        inc    ebx
        cmp    dword ptr [0x24], ebx         ; -> obj7:0x0388
        ja     loc_5_3CBE
loc_5_3D1F:
        test   edi, 2
        je     loc_5_3D4A
        push   esi
        call   sub_5_11DE
        add    esp, 4
        test   eax, eax
        jne    loc_5_3D3F
        mov    ecx, esi
        call   sub_5_3C0E
        xor    eax, eax
        jmp    loc_5_3D97
loc_5_3D3F:
        push   esi
        call   sub_5_9014
        add    esp, 4
        jmp    loc_5_3D92
loc_5_3D4A:
        test   edi, 1
        je     loc_5_3D92
        mov    eax, 0x40000000
        mov    ecx, esi
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_3D92
        push   0
        push   esi
        call   _CONFIGMG_Disable_DevNode
        add    esp, 8
        test   eax, eax
        jne    loc_5_3D87
        push   0x40000000
        mov    ecx, esi
        push   0
        call   sub_5_1906
        jmp    loc_5_3D92
loc_5_3D87:
        mov    ecx, esi
        call   sub_5_3C0E
        xor    eax, eax
        jmp    loc_5_3D97
loc_5_3D92:
        mov    eax, 1
loc_5_3D97:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_3D9E:
        sub    esp, 4
        mov    eax, 0x40000000
        push   esi
        push   eax
        mov    esi, dword ptr [esp + 0x10]
        push   0
        push   eax
        mov    ecx, esi
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_3DF8
        lea    ecx, [esp + 4]
        push   0
        push   ecx
        push   0
        push   dword ptr [esi + 4]
        call   _CONFIGMG_Get_HW_Prof_Flags
        add    esp, 0x10
        test   eax, eax
        jne    loc_5_3DF8
        test   byte ptr [esp + 4], 1
        jne    loc_5_3DF8
        push   0
        push   esi
        call   _CONFIGMG_Enable_DevNode
        add    esp, 8
        test   eax, eax
        jne    loc_5_3DF8
        push   0x40000000
        mov    ecx, esi
        push   0
        call   sub_5_1923
loc_5_3DF8:
        mov    eax, 1
        pop    esi
        add    esp, 4
        ret    
loc_5_3E02:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x4d8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], ebx
        mov    byte ptr [ebp - 0x4d8], bl
        cmp    dword ptr [ebp + 0x10], ebx
        je     loc_5_3E38
        push   ebx
        call   sub_5_23DF
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [0], 1              ; -> obj7:0x0494
        jmp    loc_5_409A
loc_5_3E38:
        push   1
        push   0x17
        call   sub_5_5FFE
        add    esp, 8
        test   eax, eax
        jne    loc_5_3E4D
        mov    ebx, 0x13
loc_5_3E4D:
        test   ebx, ebx
        jne    loc_5_4072
        lea    eax, [ebp - 4]
        mov    esi, 0x80000002
        push   eax
        push   0                             ; -> loc_7_062C
        push   esi
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_3EB0
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0xd8]
        mov    dword ptr [ebp - 0x10], 0x6e
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> loc_7_0658
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_3EB0
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> loc_7_065C
        push   esi
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        je     loc_5_3EB5
loc_5_3EB0:
        mov    ebx, 0x1d
loc_5_3EB5:
        test   ebx, ebx
        jne    loc_5_4072
        mov    dword ptr [0x24], 0           ; -> obj7:0x0388
        xor    esi, esi
        cmp    word ptr [ebp - 0xd8], si
        jbe    loc_5_3F8D
        lea    edi, [ebp - 0xcc]
loc_5_3EDC:
        mov    cx, word ptr [edi]
        lea    edx, [ebp - 0x18]
        call   sub_5_30A3
        mov    eax, dword ptr [0x24]         ; -> obj7:0x0388
        lea    ecx, [ebp - 0x18]
        lea    eax, [eax*4]                  ; -> obj7:0x0364
        push   eax
        push   ecx
        push   dword ptr [ebp - 8]
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_3F7A
        lea    edx, [ebp - 0xc]
        inc    dword ptr [0x24]              ; -> obj7:0x0388
        movzx  ecx, word ptr [edi]
        call   sub_5_44B6
        test   eax, eax
        jne    loc_5_3F7A
        test   dword ptr [ebp - 0xc], 0x80000003
        je     loc_5_3F7A
        cmp    byte ptr [ebp - 0x4d8], 0
        jne    loc_5_3F45
        push   0x13
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 0x4d8]
        mov    edx, eax
        call   sub_5_C486
loc_5_3F45:
        push   dword ptr [ebp - 4]
        mov    dx, word ptr [edi]
        lea    ecx, [ebp - 0x68]
        mov    byte ptr [ebp - 0x68], 0
        push   ecx
        push   edx
        call   sub_5_30B6
        add    esp, 0xc
        lea    edx, [ebp - 0x68]
        lea    ecx, [ebp - 0x4d8]
        call   sub_5_C44E
        mov    edx, 0                        ; -> obj7:0x0664
        lea    ecx, [ebp - 0x4d8]
        call   sub_5_C44E
loc_5_3F7A:
        add    edi, 0xc
        inc    esi
        movzx  eax, word ptr [ebp - 0xd8]
        cmp    eax, esi
        ja     loc_5_3EDC
loc_5_3F8D:
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        cmp    byte ptr [ebp - 0x4d8], 0
        je     loc_5_3FB8
        push   0x14
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 0x4d8]
        mov    edx, eax
        call   sub_5_C44E
loc_5_3FB8:
        cmp    dword ptr [0x24], 0           ; -> obj7:0x0388
        jb     loc_5_4072
        cmp    dword ptr [0x24], 1           ; -> obj7:0x0388
        jne    loc_5_3FF3
        push   0
        push   6
        call   sub_5_6710
        add    esp, 4
        mov    esi, eax
        push   7
        call   sub_5_6710
        add    esp, 4
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_5ECA
        test   eax, eax
        je     loc_5_4048
loc_5_3FF3:
        cmp    byte ptr [ebp - 0x4d8], 0
        je     loc_5_401B
        push   0
        lea    esi, [ebp - 0x4d8]
        push   0x12
        call   sub_5_6710
        add    esp, 4
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_5ECA
        test   eax, eax
        je     loc_5_4048
loc_5_401B:
        mov    ecx, 0x8000
        call   sub_5_3874
        mov    ecx, 1
        call   sub_5_3874
        push   0                             ; -> loc_5_3C56
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        mov    ecx, 3
        call   sub_5_CBE2
        test   eax, eax
        jne    loc_5_404D
loc_5_4048:
        mov    ebx, 0x13
loc_5_404D:
        xor    esi, esi
        cmp    dword ptr [0x24], esi         ; -> obj7:0x0388
        jbe    loc_5_4072
        mov    edi, 0                        ; -> obj7:0x0364
loc_5_405C:
        push   dword ptr [edi]
        call   sub_5_D0E4
        add    esp, 4
        add    edi, 4
        inc    esi
        cmp    dword ptr [0x24], esi         ; -> obj7:0x0388
        ja     loc_5_405C
loc_5_4072:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        cmp    ebx, 0x13
        jne    loc_5_408E
        push   -1
        call   sub_5_40A1
        add    esp, 4
        jmp    loc_5_4098
loc_5_408E:
        mov    ecx, 2
        call   sub_5_3874
loc_5_4098:
        mov    eax, ebx
loc_5_409A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_40A1:
        mov    ecx, 0x8000
        call   sub_5_3874
        mov    ecx, 3
        call   sub_5_3874
        mov    edx, 0                        ; -> loc_5_3D9E
        xor    ecx, ecx
        call   sub_5_CC30
        xor    eax, eax
        cmp    dword ptr [0], eax            ; -> obj7:0x0494
        je     loc_5_40DA
        push   1
        mov    dword ptr [0], eax            ; -> obj7:0x0494
        call   sub_5_23DF
        add    esp, 4
loc_5_40DA:
        push   0
        push   0x19
        call   sub_5_5FFE
        add    esp, 8
        cmp    dword ptr [esp + 4], -1
        je     loc_5_40F6
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_3C0E
loc_5_40F6:
        mov    ecx, dword ptr [0]            ; -> obj7:0x00A8
        call   sub_1_0598
loc_5_4101:
        mov    ecx, 0x8001
        call   sub_5_3874
        xor    eax, eax
        ret    
sub_5_410E:
        push   ebp
        mov    eax, edx
        mov    ebp, esp
        sub    esp, 0x110
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        je     loc_5_4188
        lea    edx, [ebp - 0xc]
        mov    ecx, eax
        call   sub_5_30A3
loc_5_412C:
        mov    edx, 0                        ; -> loc_7_06A4
        lea    ecx, [ebp - 0x110]
        call   sub_5_C486
        lea    edx, [ebp - 0xc]
        lea    ecx, [ebp - 0x110]
        call   sub_5_C44E
        mov    edx, 0                        ; -> obj7:0x06AC
        lea    ecx, [ebp - 0x110]
        call   sub_5_C44E
        lea    ecx, [ebp - 0x110]
        mov    edx, esi
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x110]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        je     loc_5_41EB
        jmp    loc_5_423B
loc_5_4188:
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj7:0x0668
        push   0x80000002
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        je     loc_5_41AC
        mov    eax, 0x1d
        jmp    loc_5_4246
loc_5_41AC:
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], 5
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> loc_7_0694
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 0x18
        mov    ebx, eax
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        test   ebx, ebx
        je     loc_5_412C
        mov    eax, 0x1d
        jmp    loc_5_4246
loc_5_41EB:
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0xb]
        mov    dword ptr [ebp - 0x10], 4
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> loc_7_06B4
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 0x18
        mov    edi, eax
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        test   edi, edi
        jne    loc_5_4229
        mov    eax, dword ptr [ebp - 0xb]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        jmp    loc_5_4244
loc_5_4229:
        cmp    edi, 0x3f4
        je     loc_5_423B
        mov    eax, 0x1d
        cmp    edi, 2
        jne    loc_5_4246
loc_5_423B:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
loc_5_4244:
        xor    eax, eax
loc_5_4246:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_424F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x120
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   eax
        push   0                             ; -> loc_7_06C4
        push   0x80000002
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        je     loc_5_427F
        mov    eax, 0x1d
        jmp    loc_5_44AF
loc_5_427F:
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0x20]
        mov    dword ptr [ebp - 0x10], 5
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> loc_7_06F0
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 0x18
        mov    edi, eax
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        test   edi, edi
        je     loc_5_42BD
        mov    eax, 0x1d
        jmp    loc_5_44AF
loc_5_42BD:
        xor    esi, esi
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    ecx, esi
        je     loc_5_42FF
        lea    edx, [ebp - 0x18]
        call   sub_5_30A3
        lea    eax, [ebp - 0x18]
        lea    ecx, [ebp - 0x20]
loc_5_42D4:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    loc_5_42F4
        or     dl, dl
        je     loc_5_42F0
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    loc_5_42F4
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    loc_5_42D4
loc_5_42F0:
        xor    eax, eax
        jmp    loc_5_42F9
loc_5_42F4:
        sbb    eax, eax
        sbb    eax, -1
loc_5_42F9:
        test   eax, eax
        jne    loc_5_430F
        jmp    loc_5_430A
loc_5_42FF:
        lea    edx, [ebp - 0x20]
        lea    ecx, [ebp - 0x18]
        call   sub_5_C486
loc_5_430A:
        mov    esi, 1
loc_5_430F:
        mov    edx, 0                        ; -> loc_7_0700
        lea    ecx, [ebp - 0x120]
        call   sub_5_C486
        lea    edx, [ebp - 0x18]
        lea    ecx, [ebp - 0x120]
        call   sub_5_C44E
        mov    edx, 0                        ; -> obj7:0x0708
        lea    ecx, [ebp - 0x120]
        call   sub_5_C44E
        lea    ecx, [ebp - 0x120]
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x120]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_D164
        add    esp, 0xc
        test   eax, eax
        je     loc_5_4371
        mov    eax, 0x1d
        jmp    loc_5_44AF
loc_5_4371:
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], 4
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> loc_7_0710
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 0x18
        cmp    eax, 0x3f4
        je     loc_5_43B9
        cmp    eax, 2
        je     loc_5_43B9
        test   eax, eax
        je     loc_5_43C0
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, 0x1d
        jmp    loc_5_44AF
loc_5_43B9:
        mov    dword ptr [ebp - 0xc], 0
loc_5_43C0:
        mov    ebx, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [ebp + 0x10]
        xor    ebx, edi
        je     loc_5_4400
        push   4
        lea    eax, [ebp - 0xc]
        push   eax
        mov    dword ptr [ebp - 0xc], edi
        push   3
        push   0
        push   0                             ; -> loc_7_0720
        push   dword ptr [ebp - 4]
        call   sub_5_D0EC
        add    esp, 0x18
        test   eax, eax
        je     loc_5_4400
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, 0x1d
        jmp    loc_5_44AF
loc_5_4400:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        test   esi, esi
        je     loc_5_44AD
        test   ebx, ebx
        je     loc_5_44AD
        test   bl, 1
        je     loc_5_445D
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 8]
        push   ecx
        call   _CONFIGMG_Locate_DevNode
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_445D
        test   edi, 1
        push   0
        push   dword ptr [ebp - 8]
        je     loc_5_4455
        call   _CONFIGMG_Disable_DevNode
        add    esp, 8
        test   eax, eax
        je     loc_5_445D
        mov    eax, 0x22
        jmp    loc_5_44AF
loc_5_4455:
        call   _CONFIGMG_Enable_DevNode
        add    esp, 8
loc_5_445D:
        test   bl, 2
        je     loc_5_44AD
        test   edi, 2
        je     loc_5_44A2
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 8]
        push   ecx
        call   _CONFIGMG_Locate_DevNode
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_44AD
        push   dword ptr [ebp - 8]
        call   sub_5_11DE
        add    esp, 4
        test   eax, eax
        je     loc_5_449B
        push   dword ptr [ebp - 8]
        call   sub_5_9014
        add    esp, 4
        jmp    loc_5_44AD
loc_5_449B:
        mov    eax, 0x22
        jmp    loc_5_44AF
loc_5_44A2:
        mov    ecx, dword ptr [0]            ; -> obj7:0x00A8
        call   sub_1_0598
loc_5_44AD:
        xor    eax, eax
loc_5_44AF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_44B6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10c
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    esi, ecx
        test   esi, esi
        je     loc_5_4515
        mov    edx, 0                        ; -> loc_7_0730
        lea    ecx, [ebp - 0x10c]
        call   sub_5_C486
        lea    edx, [ebp - 0xc]
        mov    ecx, esi
        call   sub_5_30A3
        lea    edx, [ebp - 0xc]
        lea    ecx, [ebp - 0x10c]
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x10c]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_D164
        add    esp, 0xc
        test   eax, eax
        je     loc_5_451C
        mov    eax, 0x1d
        jmp    loc_5_456B
loc_5_4515:
        mov    dword ptr [ebp - 4], 0x80000005
loc_5_451C:
        mov    dword ptr [ebp - 0xb], 4
        lea    eax, [ebp - 0xb]
        push   eax
        push   ebx
        push   0
        push   0
        push   0                             ; -> obj7:0x0738
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 0x18
        mov    edi, eax
        cmp    eax, 0x3f4
        je     loc_5_454A
        cmp    eax, 2
        jne    loc_5_454E
loc_5_454A:
        xor    edi, edi
        mov    dword ptr [ebx], edi
loc_5_454E:
        test   esi, esi
        je     loc_5_455D
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
loc_5_455D:
        mov    eax, 0
        cmp    edi, 1
        adc    eax, -1
        and    eax, 0x1d
loc_5_456B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_4572:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x110
        push   esi
        mov    dword ptr [ebp - 8], edx
        push   edi
        mov    esi, ecx
        test   esi, esi
        je     loc_5_45D1
        mov    edx, 0                        ; -> obj7:0x0748
        lea    ecx, [ebp - 0x110]
        call   sub_5_C486
        lea    edx, [ebp - 0x10]
        mov    ecx, esi
        call   sub_5_30A3
        lea    edx, [ebp - 0x10]
        lea    ecx, [ebp - 0x110]
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x110]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_D164
        add    esp, 0xc
        test   eax, eax
        je     loc_5_45D8
        mov    eax, 0x1d
        jmp    loc_5_4611
loc_5_45D1:
        mov    dword ptr [ebp - 4], 0x80000005
loc_5_45D8:
        push   4
        lea    eax, [ebp - 8]
        push   eax
        push   3
        push   0
        push   0                             ; -> obj7:0x0750
        push   dword ptr [ebp - 4]
        call   sub_5_D0EC
        add    esp, 0x18
        mov    edi, eax
        test   esi, esi
        je     loc_5_4603
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
loc_5_4603:
        mov    eax, 0
        cmp    edi, 1
        adc    eax, -1
        and    eax, 0x1d
loc_5_4611:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_4617:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x84
        push   ebx
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 8], 0x6e
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, ecx
        push   eax
        push   0                             ; -> obj7:0x0760
        push   0x80000002
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_4782
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x84]
        push   eax
        xor    ebx, ebx
        push   ecx
        push   ebx
        push   ebx
        push   0                             ; -> loc_7_078C
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_4777
        cmp    dword ptr [ebp - 8], 0x6e
        jne    loc_5_4777
        cmp    edi, -1
        jne    loc_5_4700
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x14]
        mov    dword ptr [ebp - 8], 5
        push   eax
        push   ecx
        push   ebx
        push   ebx
        push   0                             ; -> loc_7_0790
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_4777
        xor    dx, dx
        lea    ecx, [ebp - 0x14]
        mov    eax, 4
loc_5_46B6:
        mov    word ptr [ebp - 0xc], 0x37
        movsx  bx, byte ptr [ecx]
        shl    dx, 4
        cmp    byte ptr [ecx], 0x39
        jg     loc_5_46CF
        mov    word ptr [ebp - 0xc], 0x30
loc_5_46CF:
        sub    dx, word ptr [ebp - 0xc]
        inc    ecx
        add    dx, bx
        dec    eax
        jne    loc_5_46B6
        xor    ecx, ecx
        cmp    word ptr [ebp - 0x84], cx
        jbe    loc_5_4700
        lea    eax, [ebp - 0x78]
loc_5_46E8:
        cmp    word ptr [eax], dx
        je     loc_5_46FE
        add    eax, 0xc
        inc    ecx
        movzx  ebx, word ptr [ebp - 0x84]
        cmp    ebx, ecx
        ja     loc_5_46E8
        jmp    loc_5_4700
loc_5_46FE:
        mov    edi, ecx
loc_5_4700:
        movzx  eax, word ptr [ebp - 0x84]
        cmp    eax, edi
        ja     loc_5_471D
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, 0x23
        jmp    loc_5_4787
loc_5_471D:
        imul   edi, edi, 0xc
        mov    cx, word ptr [ebp + edi - 0x78]
        lea    edi, [ebp + edi - 0x82]
        push   dword ptr [ebp - 4]
        movzx  eax, cx
        lea    ebx, [esi + 4]
        mov    dword ptr [esi], eax
        push   ebx
        push   ecx
        call   sub_5_30B6
        add    esp, 0xc
        xor    ecx, ecx
        cmp    dword ptr [edi], ecx
        jne    loc_5_4761
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        jne    loc_5_4753
        mov    dword ptr [esi + 0x54], ecx
        jmp    loc_5_4768
loc_5_4753:
        cmp    eax, 1
        jne    loc_5_4761
        mov    dword ptr [esi + 0x54], 1
        jmp    loc_5_4768
loc_5_4761:
        mov    dword ptr [esi + 0x54], 2
loc_5_4768:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        xor    eax, eax
        jmp    loc_5_4787
loc_5_4777:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
loc_5_4782:
        mov    eax, 0x1d
loc_5_4787:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_478E:
        mov    eax, dword ptr [0]            ; -> obj7:0x082C
        mov    cl, byte ptr [eax]
        test   cl, 0x80
        je     loc_5_47A3
        movzx  eax, word ptr [eax + 1]
        add    eax, 3
        jmp    loc_5_47AB
loc_5_47A3:
        xor    eax, eax
        mov    al, cl
        and    eax, 7
        inc    eax
loc_5_47AB:
        ret    
sub_5_47AC:
        and    ecx, 0x1f
        mov    eax, 0x1a
        cmp    eax, ecx
        sbb    eax, eax
        inc    eax
        cmp    ecx, 1
        sbb    ecx, ecx
        inc    ecx
        and    eax, ecx
        ret    
sub_5_47C2:
        push   esi
        push   edi
        mov    eax, ecx
        mov    esi, ecx
        shr    ax, 0xd
        and    ecx, 3
        movzx  edx, ax
        lea    ecx, [edx + ecx*8]
        call   sub_5_47AC
        mov    edi, eax
        mov    ecx, esi
        shr    ecx, 8
        call   sub_5_47AC
        and    edi, eax
        mov    ecx, esi
        shr    ecx, 2
        and    esi, 0x80
        call   sub_5_47AC
        and    edi, eax
        cmp    esi, 1
        sbb    eax, eax
        neg    eax
        and    edi, eax
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_5_4808:
        push   ebx
        mov    byte ptr [ecx], 0x2a
        mov    al, dl
        inc    ecx
        shr    al, 2
        mov    bl, dl
        add    al, 0x40
        and    bl, 3
        mov    byte ptr [ecx], al
        inc    ecx
        mov    ax, dx
        shr    ax, 0xd
        lea    ebx, [eax + ebx*8]
        mov    ax, dx
        add    bl, 0x40
        shr    ax, 8
        mov    byte ptr [ecx], bl
        shr    edx, 0x10
        inc    ecx
        and    al, 0x1f
        add    al, 0x40
        mov    byte ptr [ecx], al
        inc    ecx
        movzx  ebx, dl
        movzx  eax, dh
        push   eax
        push   ebx
        push   0                             ; -> obj7:0x08D0
        push   ecx
        call   sub_5_D0FC
        add    esp, 0x10
        pop    ebx
        ret    
sub_5_4855:
        cmp    dword ptr [0], 0              ; -> obj7:0x0844
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        jne    loc_5_48CA
        test   esi, 0x8000
        jne    loc_5_4873
        inc    dword ptr [0]                 ; -> obj7:0x0854
loc_5_4873:
        mov    ecx, dword ptr [0]            ; -> obj7:0x0830
        test   ecx, ecx
        je     loc_5_4893
        push   dword ptr [esp + 0x10]
        push   edi
        mov    edx, esi
        call   sub_5_1761
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
        jmp    loc_5_48CC
loc_5_4893:
        mov    ecx, 0
        mov    eax, dword ptr [0]            ; -> obj7:0x0838
        cmp    dword ptr [0], 1              ; -> obj7:0x0848
        adc    ecx, -1
        and    ecx, 3
        mov    ebx, dword ptr [eax + ecx*4 + 0x30]
        test   ebx, ebx
        je     loc_5_48CA
loc_5_48B2:
        mov    edx, esi
        mov    ecx, ebx
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_5_1761
        test   eax, eax
        je     loc_5_48D2
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    loc_5_48B2
loc_5_48CA:
        xor    eax, eax
loc_5_48CC:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_5_48D2:
        mov    eax, 2
        jmp    loc_5_48CC
loc_5_48D9:
        sub    esp, 8
        cmp    dword ptr [0], 0              ; -> obj7:0x0844
        push   esi
        mov    esi, dword ptr [0]            ; -> obj7:0x082C
        je     loc_5_4932
        movzx  eax, byte ptr [esi + 2]
        mov    cl, byte ptr [esi + 1]
        push   eax
        xor    eax, eax
        mov    al, cl
        lea    edx, [esp + 8]
        shr    cl, 4
        and    eax, 0xf
        push   eax
        movzx  eax, cl
        push   eax
        push   0                             ; -> loc_7_08D8
        push   edx
        call   sub_5_D0FC
        lea    ecx, [esp + 0x18]
        add    esp, 0x14
        xor    edx, edx
        push   0
        push   0
        push   ecx
        push   1
        mov    ecx, dword ptr [0]            ; -> obj7:0x0838
        push   0                             ; -> obj7:0x08E8
        call   sub_5_886D
loc_5_4932:
        cmp    byte ptr [esi + 1], 0x10
        sbb    eax, eax
        pop    esi
        and    eax, 0x1f
        add    esp, 8
        ret    
loc_5_4940:
        sub    esp, 0xc
        cmp    dword ptr [0], 0              ; -> obj7:0x0844
        push   esi
        je     loc_5_4981
        mov    eax, dword ptr [0]            ; -> obj7:0x082C
        mov    esi, dword ptr [eax + 1]
        mov    ecx, esi
        call   sub_5_47C2
        test   eax, eax
        mov    eax, 0
        je     loc_5_4983
        lea    ecx, [esp + 4]
        mov    edx, esi
        call   sub_5_4808
        lea    edx, [esp + 4]
        push   1
        mov    ecx, dword ptr [0]            ; -> obj7:0x0838
        call   sub_5_21CF
loc_5_4981:
        xor    eax, eax
loc_5_4983:
        pop    esi
        add    esp, 0xc
        ret    
loc_5_4988:
        sub    esp, 0xc
        cmp    dword ptr [0], 0              ; -> obj7:0x0844
        push   esi
        je     loc_5_49C9
        mov    eax, dword ptr [0]            ; -> obj7:0x082C
        mov    esi, dword ptr [eax + 1]
        mov    ecx, esi
        call   sub_5_47C2
        test   eax, eax
        mov    eax, 0
        je     loc_5_49CB
        lea    ecx, [esp + 4]
        mov    edx, esi
        call   sub_5_4808
        lea    edx, [esp + 4]
        push   0
        mov    ecx, dword ptr [0]            ; -> obj7:0x0838
        call   sub_5_21CF
loc_5_49C9:
        xor    eax, eax
loc_5_49CB:
        pop    esi
        add    esp, 0xc
        ret    
loc_5_49D0:
        sub    esp, 0x104
        cmp    dword ptr [0], 0              ; -> obj7:0x0844
        push   ebx
        push   esi
        push   edi
        je     loc_5_4A90
        lea    eax, [esp + 0xc]
        push   0
        lea    ecx, [esp + 0x14]
        push   eax
        push   ecx
        mov    eax, 0                        ; -> loc_7_08F4
        mov    dword ptr [esp + 0x18], 0x100
        cmp    dword ptr [0], -1             ; -> obj7:0x0850
        push   1
        je     loc_5_4A0F
        mov    eax, 0                        ; -> loc_7_0900
loc_5_4A0F:
        push   eax
        xor    edx, edx
        mov    ecx, dword ptr [0]            ; -> obj7:0x0838
        call   sub_5_87D6
        test   eax, eax
        je     loc_5_4A90
        mov    ebx, dword ptr [0]            ; -> obj7:0x082C
        push   0x201
        movzx  esi, word ptr [ebx + 1]
        lea    eax, [esi + 1]
        push   eax
        call   sub_5_D134
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_5_4A49
        mov    eax, 2
        jmp    loc_5_4A92
loc_5_4A49:
        push   esi
        lea    edx, [ebx + 3]
        mov    ecx, edi
        call   sub_5_5B5D
        push   0
        mov    eax, 0                        ; -> loc_7_0910
        cmp    dword ptr [0], -1             ; -> obj7:0x0850
        push   0
        push   edi
        push   1
        je     loc_5_4A6E
        mov    eax, 0                        ; -> loc_7_091C
loc_5_4A6E:
        push   eax
        xor    edx, edx
        mov    ecx, dword ptr [0]            ; -> obj7:0x0838
        call   sub_5_886D
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_4A90
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_4A90:
        xor    eax, eax
loc_5_4A92:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x104
        ret    
loc_5_4A9C:
        push   ebp
        mov    ecx, dword ptr [0]            ; -> obj7:0x082C
        mov    ebp, esp
        inc    ecx
        sub    esp, 8
        cmp    word ptr [ecx], 0
        push   edi
        jne    loc_5_4ABB
        push   0
        xor    edx, edx
        mov    ecx, 0x8004
        jmp    loc_5_4AF0
loc_5_4ABB:
        lea    edi, [ebp - 8]
        xor    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        cmp    dword ptr [0], eax            ; -> obj7:0x0848
        mov    ax, word ptr [ecx]
        je     loc_5_4AE2
        cmp    ax, 1
        je     loc_5_4AE6
loc_5_4AD3:
        shr    ax, 1
        inc    word ptr [ebp - 6]
        cmp    ax, 1
        jne    loc_5_4AD3
        jmp    loc_5_4AE6
loc_5_4AE2:
        mov    word ptr [ebp - 4], ax
loc_5_4AE6:
        push   8
        lea    edx, [ebp - 8]
        mov    ecx, 4
loc_5_4AF0:
        call   sub_5_4855
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_4AFA:
        push   ebp
        mov    ecx, dword ptr [0]            ; -> obj7:0x082C
        mov    ebp, esp
        inc    ecx
        sub    esp, 4
        cmp    byte ptr [ecx], 0
        push   edi
        jne    loc_5_4B18
        push   0
        xor    edx, edx
        mov    ecx, 0x8003
        jmp    loc_5_4B44
loc_5_4B18:
        lea    edi, [ebp - 4]
        xor    eax, eax
        stosd  dword ptr es:[edi], eax
        cmp    dword ptr [0], eax            ; -> obj7:0x0848
        mov    al, byte ptr [ecx]
        je     loc_5_4B37
        cmp    al, 1
        je     loc_5_4B3A
loc_5_4B2C:
        shr    al, 1
        inc    byte ptr [ebp - 3]
        cmp    al, 1
        jne    loc_5_4B2C
        jmp    loc_5_4B3A
loc_5_4B37:
        mov    byte ptr [ebp - 2], al
loc_5_4B3A:
        push   4
        lea    edx, [ebp - 4]
        mov    ecx, 3
loc_5_4B44:
        call   sub_5_4855
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_4B4E:
        push   esi
        cmp    edx, ecx
        je     loc_5_4B93
        mov    esi, dword ptr [esp + 8]
        test   esi, esi
        je     loc_5_4B93
        lea    eax, [esi - 1]
        test   eax, esi
        jne    loc_5_4B6A
        test   eax, ecx
        jne    loc_5_4B6A
        test   eax, edx
        je     loc_5_4B93
loc_5_4B6A:
        cmp    edx, ecx
        jb     loc_5_4B83
        mov    eax, edx
        sub    edx, edx
        sub    eax, ecx
        div    esi
        inc    eax
        cmp    eax, 0x100
        mov    eax, 0
        jb     loc_5_4B98
loc_5_4B83:
        mov    edx, 9
        mov    ecx, dword ptr [0]            ; -> obj7:0x0838
        call   sub_5_731F
loc_5_4B93:
        mov    eax, 1
loc_5_4B98:
        pop    esi
        ret    4
sub_5_4B9C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    word ptr [ebp - 2], dx
        push   ebx
        mov    word ptr [ebp - 4], cx
        push   esi
        and    word ptr [ebp - 2], 0x3ff
        mov    dx, word ptr [ebp - 2]
        push   edi
        and    word ptr [ebp - 4], 0x3ff
        cmp    word ptr [ebp - 4], dx
        ja     loc_5_4CEF
        cmp    dx, 0x100
        jb     loc_5_4CEF
        mov    ecx, 0
        mov    eax, dword ptr [0]            ; -> obj7:0x0838
        cmp    dword ptr [0], 1              ; -> obj7:0x0848
        adc    ecx, -1
        and    ecx, 3
        mov    esi, dword ptr [eax + ecx*4 + 0x30]
        test   esi, esi
        je     loc_5_4CEF
loc_5_4BF5:
        mov    edx, 2
        mov    ecx, esi
        call   sub_5_184D
        test   eax, eax
        je     loc_5_4CE5
loc_5_4C09:
        mov    di, word ptr [eax + 0x22]
        lea    edx, [eax + 0x1c]
        mov    cx, word ptr [ebp - 2]
        cmp    di, cx
        jb     loc_5_4C1C
        mov    di, cx
loc_5_4C1C:
        mov    cx, word ptr [edx + 4]
        mov    bx, word ptr [ebp - 4]
        cmp    cx, bx
        ja     loc_5_4C2C
        mov    cx, bx
loc_5_4C2C:
        mov    word ptr [ebp - 6], 0
        cmp    di, cx
        sbb    ecx, ecx
        inc    ecx
        cmp    word ptr [edx], 0
        mov    dword ptr [ebp - 0x10], ecx
        je     loc_5_4CA4
        mov    cx, word ptr [edx]
        mov    word ptr [ebp - 0x14], cx
loc_5_4C48:
        movzx  ecx, word ptr [ebp - 6]
        mov    bx, word ptr [ebp - 4]
        imul   ecx, ecx, 0xc
        add    ecx, edx
        mov    di, word ptr [ecx + 0x10]
        cmp    bx, di
        ja     loc_5_4C61
        mov    bx, di
loc_5_4C61:
        mov    di, word ptr [ecx + 0x12]
        mov    word ptr [ebp - 0xc], di
        mov    di, word ptr [ebp - 2]
        cmp    word ptr [ebp - 0xc], di
        jb     loc_5_4C77
        mov    word ptr [ebp - 0xc], di
loc_5_4C77:
        cmp    bx, word ptr [ebp - 0xc]
        ja     loc_5_4C96
        movzx  edi, word ptr [ecx + 0x12]
        movzx  ebx, word ptr [ecx + 0xe]
        movzx  ecx, word ptr [ecx + 0x10]
        sub    edi, ebx
        sub    edi, ecx
        cmp    edi, -1
        jne    loc_5_4C96
        or     byte ptr [ebp - 0x10], 1
loc_5_4C96:
        mov    cx, word ptr [ebp - 0x14]
        inc    word ptr [ebp - 6]
        cmp    word ptr [ebp - 6], cx
        jb     loc_5_4C48
loc_5_4CA4:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_5_4CD1
        mov    byte ptr [edx + 0xa], 0
        mov    byte ptr [edx + 0xb], 0
        xor    di, di
        cmp    word ptr [edx], di
        je     loc_5_4CD1
loc_5_4CBA:
        movzx  ebx, di
        imul   ebx, ebx, 0xc
        add    ebx, edx
        inc    di
        mov    byte ptr [ebx + 0x16], 0
        mov    byte ptr [ebx + 0x17], 0
        cmp    word ptr [edx], di
        ja     loc_5_4CBA
loc_5_4CD1:
        mov    edx, 2
        mov    ecx, eax
        call   sub_5_1877
        test   eax, eax
        jne    loc_5_4C09
loc_5_4CE5:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_5_4BF5
loc_5_4CEF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_4CF6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0]            ; -> obj7:0x082C
        lea    esi, [ebx + 7]
        cmp    byte ptr [esi], 0
        jne    loc_5_4D20
        push   0
        xor    edx, edx
        mov    ecx, 0x8002
        call   sub_5_4855
        jmp    loc_5_4EE2
loc_5_4D20:
        lea    edi, [ebp - 0x24]
        xor    eax, eax
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        mov    word ptr [ebp - 0x22], 0xc
        mov    ax, 1
        sub    ax, word ptr [0]              ; -> obj7:0x0848
        mov    word ptr [ebp - 0x24], ax
        cmp    dword ptr [0], 0              ; -> obj7:0x0848
        je     loc_5_4D74
        lea    ecx, [ebx + 2]
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 0x20], ax
        movzx  ax, byte ptr [esi]
        add    ax, word ptr [ecx]
        dec    ax
        mov    word ptr [ebp - 0x1e], ax
        test   byte ptr [ebx + 1], 1
        jne    loc_5_4D6F
        mov    byte ptr [ebp - 0x1a], 4
        mov    byte ptr [ebp - 0x19], 3
loc_5_4D6F:
        mov    cx, word ptr [ecx]
        jmp    loc_5_4DF1
loc_5_4D74:
        movzx  eax, byte ptr [ebx + 6]
        mov    dx, word ptr [ebx + 2]
        push   eax
        movzx  ecx, word ptr [ebx + 4]
        mov    word ptr [ebp - 0xc], dx
        mov    dword ptr [ebp - 4], eax
        movzx  edi, dx
        mov    edx, ecx
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, edi
        call   sub_5_4B4E
        test   eax, eax
        je     loc_5_4E24
        cmp    byte ptr [ebx + 6], 0
        je     loc_5_4DB5
        movzx  ax, byte ptr [ebx + 6]
        dec    ax
        not    ax
        mov    word ptr [ebp - 0x18], ax
        jmp    loc_5_4DBB
loc_5_4DB5:
        mov    word ptr [ebp - 0x18], 0xffff
loc_5_4DBB:
        movzx  ax, byte ptr [esi]
        mov    word ptr [ebp - 0x16], ax
        mov    cx, word ptr [ebx + 2]
        mov    word ptr [ebp - 0x14], cx
        movzx  ax, byte ptr [esi]
        add    ax, word ptr [ebx + 4]
        dec    ax
        mov    word ptr [ebp - 0x12], ax
        test   byte ptr [ebx + 1], 1
        jne    loc_5_4DE7
        mov    byte ptr [ebp - 0xe], 4
        mov    byte ptr [ebp - 0xd], 3
loc_5_4DE7:
        mov    cx, word ptr [ebx + 2]
        cmp    word ptr [ebx + 4], cx
        jne    loc_5_4E06
loc_5_4DF1:
        cmp    cx, 0x400
        jbe    loc_5_4E06
        movzx  dx, byte ptr [esi]
        add    dx, cx
        dec    dx
        call   sub_5_4B9C
loc_5_4E06:
        mov    ecx, 2
        lea    edx, [ebp - 0x24]
        mov    eax, ecx
        sub    eax, dword ptr [0]            ; -> obj7:0x0848
        imul   eax, eax, 0xc
        push   eax
        call   sub_5_4855
        jmp    loc_5_4EE2
loc_5_4E24:
        mov    eax, dword ptr [ebp - 8]
        sub    eax, edi
        cdq    
        idiv   dword ptr [ebp - 4]
        inc    eax
        push   0x201
        mov    dword ptr [ebp - 4], eax
        inc    eax
        imul   eax, eax, 0xc
        push   eax
        mov    dword ptr [ebp - 8], eax
        call   sub_5_D134
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_5_4E63
        mov    edx, 3
        mov    ecx, dword ptr [0]            ; -> obj7:0x0838
        call   sub_5_731F
        mov    eax, 2
        jmp    loc_5_4EE2
loc_5_4E63:
        mov    word ptr [edi + 2], 0xc
        cmp    dword ptr [ebp - 4], 0
        mov    ax, word ptr [ebp - 4]
        mov    dx, word ptr [ebp - 0xc]
        mov    word ptr [edi], ax
        je     loc_5_4EBB
        lea    ecx, [edi + 0xc]
        mov    eax, dword ptr [ebp - 4]
        movzx  si, byte ptr [esi]
        mov    dword ptr [ebp - 4], eax
loc_5_4E87:
        mov    word ptr [ecx], 0xffff
        lea    eax, [esi + edx]
        dec    ax
        mov    word ptr [ecx + 2], si
        test   byte ptr [ebx + 1], 1
        mov    word ptr [ecx + 4], dx
        mov    word ptr [ecx + 6], ax
        jne    loc_5_4EAB
        mov    byte ptr [ecx + 0xa], 4
        mov    byte ptr [ecx + 0xb], 3
loc_5_4EAB:
        movzx  ax, byte ptr [ebx + 6]
        add    dx, ax
        add    ecx, 0xc
        dec    dword ptr [ebp - 4]
        jne    loc_5_4E87
loc_5_4EBB:
        mov    edx, edi
        mov    ecx, 2
        push   dword ptr [ebp - 8]
        call   sub_5_4855
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        mov    esi, eax
        jne    loc_5_4EE0
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_4EE0:
        mov    eax, esi
loc_5_4EE2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_4EE9:
        push   ebp
        mov    edx, dword ptr [0]            ; -> obj7:0x082C
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        lea    esi, [edx + 3]
        cmp    byte ptr [esi], 0
        jne    loc_5_4F0E
        push   0
        xor    edx, edx
        mov    ecx, 0x8002
        jmp    loc_5_4FBF
loc_5_4F0E:
        lea    edi, [ebp - 0x1c]
        xor    eax, eax
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        mov    di, word ptr [edx + 1]
        cmp    di, 0x100
        jae    loc_5_4F2C
        xor    bl, bl
        mov    byte ptr [ebp - 1], al
        jmp    loc_5_4F50
loc_5_4F2C:
        cmp    di, 0x400
        jae    loc_5_4F3B
        mov    byte ptr [ebp - 1], 3
        mov    bl, 4
        jmp    loc_5_4F50
loc_5_4F3B:
        xor    bl, bl
        mov    ecx, edi
        movzx  dx, byte ptr [esi]
        add    dx, di
        mov    byte ptr [ebp - 1], bl
        dec    dx
        call   sub_5_4B9C
loc_5_4F50:
        mov    ax, 1
        mov    edx, dword ptr [0]            ; -> obj7:0x0848
        mov    word ptr [ebp - 0x1a], 0xc
        sub    ax, dx
        test   edx, edx
        mov    word ptr [ebp - 0x1c], ax
        je     loc_5_4F87
        mov    word ptr [ebp - 0x18], di
        mov    cl, byte ptr [ebp - 1]
        movzx  ax, byte ptr [esi]
        add    ax, di
        mov    byte ptr [ebp - 0x12], bl
        dec    ax
        mov    byte ptr [ebp - 0x11], cl
        mov    word ptr [ebp - 0x16], ax
        jmp    loc_5_4FAF
loc_5_4F87:
        mov    word ptr [ebp - 0x10], 0xffff
        movzx  ax, byte ptr [esi]
        mov    word ptr [ebp - 0xe], ax
        mov    cl, byte ptr [ebp - 1]
        mov    word ptr [ebp - 0xc], di
        movzx  ax, byte ptr [esi]
        add    ax, di
        mov    byte ptr [ebp - 6], bl
        dec    ax
        mov    byte ptr [ebp - 5], cl
        mov    word ptr [ebp - 0xa], ax
loc_5_4FAF:
        mov    ecx, 2
        mov    eax, ecx
        sub    eax, edx
        lea    edx, [ebp - 0x1c]
        imul   eax, eax, 0xc
        push   eax
loc_5_4FBF:
        call   sub_5_4855
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_4FCB:
        cmp    dword ptr [0], 0              ; -> obj7:0x0844
        push   esi
        push   edi
        jne    loc_5_50B0
        cmp    dword ptr [0], 0              ; -> obj7:0x0848
        je     loc_5_4FED
        mov    eax, 0x1f
        jmp    loc_5_50B2
loc_5_4FED:
        call   sub_5_478E
        cmp    eax, 1
        jbe    loc_5_500D
        mov    eax, dword ptr [0]            ; -> obj7:0x082C
        mov    al, byte ptr [eax + 1]
        cmp    al, 3
        jb     loc_5_500F
        mov    eax, 0x1f
        jmp    loc_5_50B2
loc_5_500D:
        mov    al, 1
loc_5_500F:
        movzx  eax, al
        mov    ecx, dword ptr [0]            ; -> obj7:0x0858
        imul   ecx, ecx, 3
        add    eax, ecx
        cmp    dword ptr [0], 0              ; -> obj7:0x0830
        mov    esi, dword ptr [eax*4]        ; -> obj7:0x085C
        je     loc_5_5064
        mov    edx, 0xbeefdead
        mov    ecx, dword ptr [0]            ; -> obj7:0x0834
        call   sub_5_14E6
        mov    edi, eax
        test   edi, edi
        jne    loc_5_504A
        mov    eax, 2
        jmp    loc_5_50B2
loc_5_504A:
        push   0
        mov    edx, edi
        push   0
        mov    ecx, dword ptr [0]            ; -> obj7:0x0838
        push   esi
        call   sub_5_1674
        mov    dword ptr [0], edi            ; -> obj7:0x0830
        jmp    loc_5_50B0
loc_5_5064:
        cmp    dword ptr [0], 0              ; -> obj7:0x083C
        je     loc_5_5074
        mov    eax, 0x1f
        jmp    loc_5_50B2
loc_5_5074:
        mov    edx, 0xbeefdead
        mov    eax, dword ptr [0]            ; -> obj7:0x0838
        mov    dword ptr [0], 1              ; -> obj7:0x083C
        mov    ecx, dword ptr [eax + 0x30]
        call   sub_5_14E6
        mov    dword ptr [0], eax            ; -> obj7:0x0834
        test   eax, eax
        jne    loc_5_50A0
        mov    eax, 2
        jmp    loc_5_50B2
loc_5_50A0:
        mov    eax, dword ptr [0]            ; -> obj7:0x0838
        mov    eax, dword ptr [eax + 0x30]
        mov    dword ptr [0], eax            ; -> obj7:0x0830
        mov    dword ptr [eax + 0xc], esi
loc_5_50B0:
        xor    eax, eax
loc_5_50B2:
        pop    edi
        pop    esi
        ret    
loc_5_50B5:
        cmp    dword ptr [0], 0              ; -> obj7:0x0844
        jne    loc_5_50E6
        cmp    dword ptr [0], 0              ; -> obj7:0x0848
        je     loc_5_50CE
        mov    eax, 0x1f
        jmp    loc_5_50E8
loc_5_50CE:
        cmp    dword ptr [0], 0              ; -> obj7:0x083C
        mov    eax, 0x1f
        je     loc_5_50E8
        mov    dword ptr [0], 0              ; -> obj7:0x0830
loc_5_50E6:
        xor    eax, eax
loc_5_50E8:
        ret    
sub_5_50E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        cmp    dword ptr [ebp + 0xc], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        mov    ebx, ecx
        jne    loc_5_510F
        push   0
        xor    edx, edx
        mov    ecx, 0x8001
        call   sub_5_4855
        jmp    loc_5_524F
loc_5_510F:
        lea    edi, [ebp - 0x24]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        mov    word ptr [ebp - 0x22], 0x14
        mov    ax, 1
        mov    edx, dword ptr [0]            ; -> obj7:0x0848
        sub    ax, dx
        test   edx, edx
        mov    word ptr [ebp - 0x24], ax
        je     loc_5_514C
        mov    cx, word ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [ebp - 0x18], cx
        add    eax, esi
        dec    eax
        mov    dword ptr [ebp - 0x20], ebx
        mov    dword ptr [ebp - 0x1c], eax
        jmp    loc_5_5192
loc_5_514C:
        mov    edx, esi
        mov    ecx, ebx
        push   dword ptr [ebp + 8]
        call   sub_5_4B4E
        mov    edx, dword ptr [0]            ; -> obj7:0x0848
        test   eax, eax
        je     loc_5_51AB
        cmp    dword ptr [ebp + 8], 0
        je     loc_5_5173
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        not    eax
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_5_517A
loc_5_5173:
        mov    dword ptr [ebp - 0x14], 0xffffffff
loc_5_517A:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], ebx
        mov    dword ptr [ebp - 0x10], ecx
        lea    eax, [esi + ecx]
        mov    cx, word ptr [ebp + 0x10]
        dec    eax
        mov    word ptr [ebp - 4], cx
        mov    dword ptr [ebp - 8], eax
loc_5_5192:
        imul   edx, edx, -0x14
        add    edx, 0x24
        mov    ecx, 1
        push   edx
        lea    edx, [ebp - 0x24]
        call   sub_5_4855
        jmp    loc_5_524F
loc_5_51AB:
        sub    esi, ebx
        sub    edx, edx
        mov    eax, esi
        push   0x201
        div    dword ptr [ebp + 8]
        lea    edi, [eax + 1]
        mov    eax, edi
        imul   eax, eax, 0x14
        add    eax, 0x10
        push   eax
        call   sub_5_D134
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_5_51EA
        mov    edx, 3
        mov    ecx, dword ptr [0]            ; -> obj7:0x0838
        call   sub_5_731F
        mov    eax, 2
        jmp    loc_5_524F
loc_5_51EA:
        mov    word ptr [esi], di
        test   edi, edi
        mov    word ptr [esi + 2], 0xc
        je     loc_5_5222
        lea    eax, [esi + 0x10]
        mov    edx, edi
loc_5_51FC:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], 0xffffffff
        mov    dword ptr [eax + 4], ecx
        add    eax, 0x14
        add    ecx, ebx
        mov    dword ptr [eax - 0xc], ebx
        dec    ecx
        add    ebx, dword ptr [ebp + 8]
        dec    edx
        mov    dword ptr [eax - 8], ecx
        mov    cx, word ptr [ebp + 0x10]
        mov    word ptr [eax - 4], cx
        jne    loc_5_51FC
loc_5_5222:
        mov    eax, edi
        mov    edx, esi
        imul   eax, eax, 0x14
        add    eax, 0x10
        mov    ecx, 1
        push   eax
        call   sub_5_4855
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        mov    edi, eax
        jne    loc_5_524D
        push   0
        push   esi
        call   sub_5_D0C8
        add    esp, 8
loc_5_524D:
        mov    eax, edi
loc_5_524F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_5258:
        push   esi
        mov    esi, dword ptr [0]            ; -> obj7:0x082C
        mov    ax, word ptr [esi + 8]
        test   ax, ax
        je     loc_5_526D
        movzx  eax, ax
        jmp    loc_5_5272
loc_5_526D:
        mov    eax, 0x10000
loc_5_5272:
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 3]
        movzx  edx, word ptr [esi + 6]
        shl    edx, 8
        and    ecx, 1
        push   ecx
        movzx  ecx, word ptr [esi + 0xa]
        shl    ecx, 8
        push   ecx
        push   eax
        movzx  ecx, word ptr [esi + 4]
        shl    ecx, 8
        call   sub_5_50E9
        pop    esi
        ret    
loc_5_5299:
        mov    ecx, dword ptr [0]            ; -> obj7:0x082C
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        mov    edx, dword ptr [ecx + 8]
        and    eax, 1
        or     al, 2
        push   eax
        push   dword ptr [ecx + 0x10]
        push   dword ptr [ecx + 0xc]
        mov    ecx, dword ptr [ecx + 4]
        call   sub_5_50E9
        ret    
loc_5_52BC:
        mov    eax, dword ptr [0]            ; -> obj7:0x082C
        xor    ecx, ecx
        mov    edx, dword ptr [eax + 4]
        mov    cl, byte ptr [eax + 3]
        and    ecx, 1
        or     cl, 2
        push   ecx
        mov    ecx, edx
        push   dword ptr [eax + 8]
        push   0
        call   sub_5_50E9
        ret    
loc_5_52DD:
        mov    dword ptr [0], 1              ; -> obj7:0x0840
        call   sub_5_478E
        cmp    eax, 1
        ja     loc_5_52F5
        xor    eax, eax
        jmp    loc_5_5317
loc_5_52F5:
        cmp    byte ptr [0], 0               ; -> obj7:0x084C
        mov    eax, 0
        je     loc_5_5317
        mov    eax, dword ptr [0]            ; -> obj7:0x082C
        cmp    byte ptr [eax + 1], 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x1f
loc_5_5317:
        ret    
sub_5_5318:
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj7:0x0838
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [eax + 0x30]
        test   eax, eax
        je     loc_5_537C
        mov    edi, dword ptr [eax]
        test   edi, edi
        je     loc_5_537C
        xor    ebx, ebx
        mov    ebp, dword ptr [eax + 0x10]
        cmp    ebp, ebx
        je     loc_5_5365
loc_5_5337:
        mov    esi, edi
        test   edi, edi
        je     loc_5_535D
loc_5_533D:
        mov    edx, ebx
        mov    ecx, esi
        call   sub_5_189A
        test   eax, eax
        je     loc_5_5383
        mov    eax, dword ptr [eax + 4]
        xor    eax, dword ptr [ebp + 4]
        test   eax, 0xffff7fff
        jne    loc_5_5383
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_5_533D
loc_5_535D:
        inc    ebx
        mov    ebp, dword ptr [ebp + 0x10]
        test   ebp, ebp
        jne    loc_5_5337
loc_5_5365:
        test   edi, edi
        je     loc_5_537C
loc_5_5369:
        mov    edx, ebx
        mov    ecx, edi
        call   sub_5_189A
        test   eax, eax
        jne    loc_5_538A
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_5_5369
loc_5_537C:
        xor    eax, eax
loc_5_537E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_5_5383:
        mov    eax, 1
        jmp    loc_5_537E
loc_5_538A:
        mov    eax, 1
        jmp    loc_5_537E
sub_5_5391:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, 1
        lea    eax, [ecx - 3]
        lea    ebx, [ecx - 2]
        cmp    eax, edx
        sbb    eax, eax
        neg    eax
        mov    dword ptr [0], eax            ; -> obj7:0x0858
        cmp    ebx, edx
        sbb    eax, eax
        dec    ecx
        neg    eax
        mov    dword ptr [0], eax            ; -> obj7:0x0848
        cmp    ecx, edx
        sbb    eax, eax
        neg    eax
        mov    dword ptr [0], eax            ; -> obj7:0x0844
        test   eax, eax
        jne    loc_5_5413
        cmp    dword ptr [0], 0              ; -> obj7:0x0848
        jne    loc_5_53E9
        mov    eax, dword ptr [0]            ; -> obj7:0x0858
        imul   eax, eax, 0xc
        mov    edx, dword ptr [eax]          ; -> obj7:0x085C
loc_5_53E9:
        push   0
        mov    eax, 0
        cmp    dword ptr [0], 1              ; -> obj7:0x0848
        adc    eax, -1
        mov    ecx, edi
        and    eax, 3
        push   eax
        call   sub_5_16B4
        test   eax, eax
        jne    loc_5_5413
        mov    eax, 2
        jmp    loc_5_56F8
loc_5_5413:
        xor    ebx, ebx
        mov    dword ptr [0], esi            ; -> obj7:0x082C
        xor    esi, esi
        mov    dword ptr [0], ebx            ; -> obj7:0x0830
        mov    dword ptr [0], ebx            ; -> obj7:0x0834
        mov    dword ptr [0], edi            ; -> obj7:0x0838
        mov    dword ptr [0], ebx            ; -> obj7:0x0840
        mov    dword ptr [0], ebx            ; -> obj7:0x083C
        mov    dword ptr [ebp - 8], ebx
        mov    byte ptr [0], bl              ; -> obj7:0x084C
        mov    dword ptr [0], 0xffffffff     ; -> obj7:0x0850
        mov    dword ptr [0], ebx            ; -> obj7:0x0854
loc_5_5454:
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 0x100
        ja     loc_5_556E
        push   0
        mov    eax, dword ptr [0]            ; -> obj7:0x082C
        push   0
        push   0
        push   1
        push   eax
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        mov    eax, dword ptr [0]            ; -> obj7:0x082C
        je     loc_5_556E
        test   byte ptr [eax], 0x80
        je     loc_5_54A5
        push   0
        push   0
        push   0
        push   3
        push   eax
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        je     loc_5_556E
loc_5_54A5:
        call   sub_5_478E
        push   0
        mov    dword ptr [ebp - 4], eax
        push   0
        push   0
        push   eax
        push   dword ptr [0]                 ; -> obj7:0x082C
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        mov    al, byte ptr [0]              ; -> obj7:0x084C
        je     loc_5_556E
        xor    edx, edx
        cmp    dword ptr [ebp - 4], edx
        je     loc_5_54EA
loc_5_54D6:
        mov    ecx, dword ptr [0]            ; -> obj7:0x082C
        add    al, byte ptr [ecx + edx]
        inc    edx
        mov    byte ptr [0], al              ; -> obj7:0x084C
        cmp    edx, dword ptr [ebp - 4]
        jb     loc_5_54D6
loc_5_54EA:
        mov    ecx, dword ptr [0]            ; -> obj7:0x082C
        mov    al, byte ptr [ecx]
        test   al, 0x80
        jne    loc_5_54F8
        and    al, 0xf8
loc_5_54F8:
        test   al, al
        je     loc_5_556E
        cmp    al, 0xff
        je     loc_5_556E
        cmp    al, 0x10
        jne    loc_5_550A
        inc    dword ptr [0]                 ; -> obj7:0x0850
loc_5_550A:
        cmp    dword ptr [0], -1             ; -> obj7:0x0850
        je     loc_5_5522
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, dword ptr [0]            ; -> obj7:0x0850
        je     loc_5_5522
        cmp    al, 0x78
        jne    loc_5_5552
loc_5_5522:
        cmp    byte ptr [4], 0               ; -> loc_7_0878
        mov    ecx, 0                        ; -> loc_7_0874
        je     loc_5_5552
loc_5_5530:
        cmp    byte ptr [ecx + 4], al
        je     loc_5_5540
        add    ecx, 6
        cmp    byte ptr [ecx + 4], 0
        jne    loc_5_5530
        jmp    loc_5_5552
loc_5_5540:
        movzx  eax, byte ptr [ecx + 5]
        mov    esi, 0x1f
        cmp    eax, dword ptr [ebp - 4]
        ja     loc_5_5552
        call   dword ptr [ecx]
        mov    esi, eax
loc_5_5552:
        test   esi, esi
        jne    loc_5_5573
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [0], eax            ; -> obj7:0x082C
        cmp    dword ptr [0], 0              ; -> obj7:0x0840
        je     loc_5_5454
        jmp    loc_5_5573
loc_5_556E:
        mov    esi, 0x1f
loc_5_5573:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, 1
        mov    eax, dword ptr [0]            ; -> obj7:0x0850
        cmp    dword ptr [0], ecx            ; -> obj7:0x0850
        mov    ecx, edx
        adc    ecx, -1
        add    eax, edx
        cmp    eax, edx
        sbb    eax, eax
        add    eax, edx
        and    ecx, eax
        mov    eax, dword ptr [0]            ; -> obj7:0x0850
        cmp    dword ptr [ebp + 8], edx
        sbb    ebx, ebx
        add    eax, edx
        add    ebx, edx
        cmp    eax, edx
        sbb    eax, eax
        neg    eax
        and    ebx, eax
        or     ecx, ebx
        je     loc_5_55B4
        mov    esi, 0x14
loc_5_55B4:
        cmp    esi, 2
        je     loc_5_55C0
        cmp    esi, 0x1f
        je     loc_5_55C7
        jmp    loc_5_55D3
loc_5_55C0:
        mov    edx, 3
        jmp    loc_5_55CC
loc_5_55C7:
        mov    edx, 9
loc_5_55CC:
        mov    ecx, edi
        call   sub_5_731F
loc_5_55D3:
        cmp    dword ptr [0], 0              ; -> obj7:0x0844
        jne    loc_5_56F6
        mov    ecx, 0                        ; -> obj7:0x0834
        call   sub_5_1596
        test   esi, esi
        jne    loc_5_55F7
        cmp    dword ptr [0], 0              ; -> obj7:0x0854
        jne    loc_5_5612
loc_5_55F7:
        cmp    dword ptr [0], 1              ; -> obj7:0x0848
        mov    eax, 0
        adc    eax, -1
        and    eax, 3
        lea    ecx, [edi + eax*4 + 0x30]
        call   sub_5_15ED
loc_5_5612:
        cmp    dword ptr [0], 0              ; -> obj7:0x0848
        jne    loc_5_5643
        call   sub_5_5318
        test   eax, eax
        je     loc_5_5636
        push   0
        mov    ecx, dword ptr [0]            ; -> obj7:0x0838
        push   0x400000
        call   sub_5_1906
loc_5_5636:
        cmp    dword ptr [0], 0              ; -> obj7:0x0848
        je     loc_5_56F6
loc_5_5643:
        mov    edi, dword ptr [edi + 0x3c]
        test   edi, edi
        je     loc_5_56F6
        mov    edx, 3
        mov    ecx, edi
        call   sub_5_184D
        test   eax, eax
        jne    loc_5_56F6
        mov    edx, 1
        mov    ecx, edi
        call   sub_5_184D
        test   eax, eax
        jne    loc_5_56F6
        mov    edx, 2
        mov    ecx, edi
        call   sub_5_184D
        mov    ecx, eax
        test   ecx, ecx
        je     loc_5_56F6
        mov    ax, word ptr [ecx + 0x20]
        cmp    ax, 0x3f8
        je     loc_5_56A4
        cmp    ax, 0x2f8
        je     loc_5_56A4
        cmp    ax, 0x3e8
        je     loc_5_56A4
        cmp    ax, 0x2e8
        jne    loc_5_56F6
loc_5_56A4:
        movzx  edx, word ptr [ecx + 0x22]
        movzx  eax, ax
        sub    edx, eax
        cmp    edx, 7
        jne    loc_5_56F6
        mov    edx, 2
        call   sub_5_1877
        test   eax, eax
        jne    loc_5_56F6
        mov    edx, 4
        mov    ecx, edi
        call   sub_5_184D
        mov    edi, eax
        test   edi, edi
        je     loc_5_56F6
        mov    ax, word ptr [edi + 0x1e]
        cmp    ax, 4
        je     loc_5_56E2
        cmp    ax, 3
        jne    loc_5_56F6
loc_5_56E2:
        mov    edx, 4
        mov    ecx, edi
        call   sub_5_1877
        test   eax, eax
        jne    loc_5_56F6
        or     byte ptr [edi + 0x1c], 1
loc_5_56F6:
        mov    eax, esi
loc_5_56F8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_5701:
        push   esi
        mov    esi, ecx
        test   byte ptr [ecx + 0xb], 0x80
        jne    loc_5_570E
        xor    eax, eax
        jmp    loc_5_5722
loc_5_570E:
        call   sub_5_D17C
        test   eax, eax
        je     loc_5_5722
loc_5_5717:
        cmp    dword ptr [eax + 5], esi
        je     loc_5_5722
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_5_5717
loc_5_5722:
        pop    esi
        ret    
sub_5_5724:
        push   esi
        push   0
        mov    esi, ecx
        push   0
        push   dword ptr [esp + 0x10]
        push   esi
        push   0x22
        push   edx
        call   sub_5_D184
        cmp    dword ptr [esp + 8], 1
        mov    ecx, eax
        jne    loc_5_5767
        cmp    ecx, 0x21
        jne    loc_5_5767
        push   0x4000000
        mov    ecx, esi
        push   0
        call   sub_5_1906
        mov    edx, 0x17
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 0x13
        jmp    loc_5_5772
loc_5_5767:
        push   esi
        mov    edx, 0x13
        call   sub_5_1DBE
loc_5_5772:
        pop    esi
        ret    4
sub_5_5776:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        push   ebp
        mov    esi, 0                        ; -> loc_7_092C
        cmp    edx, 8
        mov    ebp, edx
        lea    edi, [esp + 0x10]
        mov    eax, 0
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        ja     loc_5_57B5
        mov    ecx, ebx
        call   sub_5_C430
        lea    ecx, [esp + 0x10]
        push   ebp
        mov    edx, ebx
        call   sub_5_5B5D
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        call   sub_5_D1B0
loc_5_57B5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_5_57BD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        call   sub_5_C3C4
        test   eax, eax
        jne    loc_5_57D6
        xor    eax, eax
        jmp    loc_5_584E
loc_5_57D6:
        cmp    byte ptr [esi], 0x2a
        jne    loc_5_57E8
        lea    edx, [eax - 1]
        lea    ecx, [esi + 1]
        call   sub_5_5776
        jmp    loc_5_584E
loc_5_57E8:
        push   1
        lea    eax, [ebp - 8]
        push   esi
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_5_D1C8
        test   eax, eax
        je     loc_5_584B
        cmp    eax, 3
        je     loc_5_581B
        cmp    eax, 6
        je     loc_5_581F
        cmp    eax, 7
        je     loc_5_581B
        mov    edx, 0x14
        mov    ecx, edi
        call   sub_5_731F
        xor    eax, eax
        jmp    loc_5_584E
loc_5_581B:
        xor    eax, eax
        jmp    loc_5_584E
loc_5_581F:
        mov    edx, 0x2e
        mov    ecx, esi
        call   sub_5_C3E0
        cmp    eax, -1
        mov    edx, eax
        jne    loc_5_583D
        mov    ecx, esi
        call   sub_5_C3C4
        mov    edx, eax
        jmp    loc_5_5841
loc_5_583D:
        mov    byte ptr [esi + edx], 0
loc_5_5841:
        mov    ecx, esi
        call   sub_5_5776
        mov    dword ptr [ebp - 4], eax
loc_5_584B:
        mov    eax, dword ptr [ebp - 4]
loc_5_584E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_5854:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        mov    edi, esi
        call   sub_5_C3C4
        add    eax, esi
        mov    dword ptr [ebp - 8], eax
loc_5_586E:
        mov    edx, 0x20
        mov    ecx, edi
        call   sub_5_C3E0
        cmp    eax, -1
        je     loc_5_5889
        mov    byte ptr [edi + eax], 0
        lea    edi, [edi + eax + 1]
        jmp    loc_5_586E
loc_5_5889:
        mov    edi, esi
loc_5_588B:
        mov    edx, 0x2c
        mov    ecx, edi
        call   sub_5_C3E0
        cmp    eax, -1
        je     loc_5_58E0
        mov    byte ptr [edi + eax], 0
        lea    edi, [edi + eax + 1]
        jmp    loc_5_588B
loc_5_58A6:
        push   4
        call   sub_5_D134
        add    esp, 8
loc_5_58B0:
        test   eax, eax
        je     loc_5_5946
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, edi
        push   dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x4c], eax
        mov    dword ptr [eax + ebx*4 - 4], edi
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_5724
        test   eax, eax
        jne    loc_5_5958
        mov    ecx, esi
        call   sub_5_C3C4
        add    esi, eax
loc_5_58E0:
        cmp    byte ptr [esi], 0
        jne    loc_5_58F0
loc_5_58E5:
        cmp    dword ptr [ebp - 8], esi
        jbe    loc_5_593B
        inc    esi
        cmp    byte ptr [esi], 0
        je     loc_5_58E5
loc_5_58F0:
        cmp    dword ptr [ebp - 8], esi
        jbe    loc_5_593B
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, esi
        call   sub_5_57BD
        mov    edi, eax
        test   edi, edi
        je     loc_5_593F
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0xc]
        push   0x201
        mov    ebx, dword ptr [eax + ecx*4 + 0x58]
        lea    edx, [eax + ecx*4]
        inc    ebx
        mov    dword ptr [ebp - 4], edx
        cmp    ebx, 1
        mov    dword ptr [edx + 0x58], ebx
        je     loc_5_58A6
        lea    eax, [ebx*4]
        push   eax
        push   dword ptr [edx + 0x4c]
        call   sub_5_D0C0
        add    esp, 0xc
        jmp    loc_5_58B0
loc_5_593B:
        xor    eax, eax
        jmp    loc_5_5958
loc_5_593F:
        mov    eax, 0xc
        jmp    loc_5_5958
loc_5_5946:
        mov    edx, 3
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_731F
        mov    eax, 2
loc_5_5958:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_595F:
        push   ebx
        push   esi
        push   edi
        mov    esi, 3
        push   ebp
        lea    edi, [ecx + 0x58]
loc_5_596B:
        mov    ebx, dword ptr [edi]
        test   ebx, ebx
        je     loc_5_59AE
        mov    eax, dword ptr [edi - 0xc]
        lea    ebp, [eax + ebx*4 - 4]
loc_5_5978:
        push   0x80000000
        push   0
        push   0
        push   0x50
        push   dword ptr [ebp]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        je     loc_5_59A8
        mov    ecx, dword ptr [ebp]
        call   sub_5_5701
        test   eax, eax
        je     loc_5_59A8
        push   dword ptr [eax + 0xb]
        push   0
        call   sub_5_D1EC
loc_5_59A8:
        sub    ebp, 4
        dec    ebx
        jne    loc_5_5978
loc_5_59AE:
        add    edi, 4
        dec    esi
        jne    loc_5_596B
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_59B9:
        mov    eax, 0                        ; -> loc_5_1DB8
        push   0
        push   0x4006
        mov    dword ptr [ecx + 0x18], eax
        mov    dword ptr [ecx + 0x1c], eax
        call   sub_5_1906
        ret    
sub_5_59D1:
        push   ebp
        mov    eax, 0x4000000
        mov    ebp, esp
        sub    esp, 0x108
        push   esi
        push   edi
        mov    esi, ecx
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_5A05
        push   1
        mov    eax, dword ptr [esi + 0x50]
        mov    ecx, esi
        mov    edx, dword ptr [eax]
        call   sub_5_5724
        jmp    loc_5_5AF5
loc_5_5A05:
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     loc_5_5B50
        lea    eax, [ebp - 8]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   eax, eax
        jne    loc_5_5A33
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_5B57
loc_5_5A33:
        test   byte ptr [ebp - 8], 4
        je     loc_5_5A44
        push   0
        mov    ecx, esi
        push   0x10
        call   sub_5_1906
loc_5_5A44:
        xor    edi, edi
        lea    eax, [ebp - 4]
        push   edi
        xor    edx, edx
        push   eax
        mov    ecx, esi
        push   edi
        mov    dword ptr [ebp - 4], edi
        push   1
        push   0                             ; -> loc_7_0938
        call   sub_5_87D6
        test   eax, eax
        je     loc_5_5A71
        cmp    eax, 0x25
        je     loc_5_5B50
        jmp    loc_5_5B57
loc_5_5A71:
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0x108]
        push   ecx
        xor    edx, edx
        push   1
        mov    ecx, esi
        mov    dword ptr [ebp - 4], 0x100
        push   0                             ; -> obj7:0x0940
        call   sub_5_87D6
        test   eax, eax
        je     loc_5_5AB9
        cmp    eax, 0x1d
        je     loc_5_5B3B
        cmp    eax, 0x25
        je     loc_5_5B50
        cmp    eax, 0x26
        je     loc_5_5B42
        jmp    loc_5_5B57
loc_5_5AB9:
        lea    ecx, [ebp - 0x108]
        mov    edx, 0x20
        call   sub_5_C3E0
        cmp    eax, -1
        jne    loc_5_5B2D
        lea    ecx, [ebp - 0x108]
        mov    edx, 0x2c
        call   sub_5_C3E0
        cmp    eax, -1
        jne    loc_5_5B2D
        push   1
        lea    eax, [ebp - 0x108]
        push   esi
        push   eax
        call   sub_5_5854
        add    esp, 0xc
loc_5_5AF5:
        cmp    eax, 0xc
        jne    loc_5_5B08
        mov    edx, 8
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_5B57
loc_5_5B08:
        test   eax, eax
        jne    loc_5_5B1F
        push   0
        mov    ecx, esi
        push   2
        push   0
        push   2
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_5B57
loc_5_5B1F:
        mov    edx, 2
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_5B57
loc_5_5B2D:
        mov    edx, 8
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_5B57
loc_5_5B3B:
        mov    edx, 0x13
        jmp    loc_5_5B47
loc_5_5B42:
        mov    edx, 4
loc_5_5B47:
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_5B57
loc_5_5B50:
        mov    ecx, esi
        call   sub_5_59B9
loc_5_5B57:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_5B5D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    dword ptr [ebp - 8], edx
        push   edi
        mov    dword ptr [ebp - 4], ecx
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    edi, esi
        jbe    loc_5_5B8C
        mov    eax, esi
        add    eax, ecx
        cmp    edi, eax
        jae    loc_5_5B8C
        add    esi, ecx
        add    edi, ecx
        dec    edi
        dec    esi
        std    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cld    
        jmp    loc_5_5B9B
loc_5_5B8C:
        mov    eax, ecx
        shr    ecx, 2
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_5_5B9B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_5BA3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 4], 4
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0                             ; -> loc_7_094C
        push   ecx
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_5BDE
        cmp    dword ptr [ebp - 8], 3
        jne    loc_5_5BDE
        cmp    dword ptr [ebp - 4], 4
        mov    eax, 1
        je     loc_5_5BE0
loc_5_5BDE:
        xor    eax, eax
loc_5_5BE0:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_5BE4:
        sub    esp, 4
        push   0                             ; -> obj7:0x0088
        push   0                             ; -> obj7:0x0958
        push   0x80000002
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        je     loc_5_5C0E
        mov    dword ptr [0], 0              ; -> obj7:0x0088
        jmp    loc_5_5C2F
loc_5_5C0E:
        lea    edx, [esp]
        mov    ecx, dword ptr [0]            ; -> obj7:0x0088
        call   sub_5_5BA3
        test   eax, eax
        je     loc_5_5C2F
        test   byte ptr [esp], 1
        je     loc_5_5C2F
        or     byte ptr [0], 0x80            ; -> obj7:0x00AC
loc_5_5C2F:
        add    esp, 4
        ret    
loc_5_5C33:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   esi
        lea    eax, [ebp - 0x20]
        push   edi
        xor    esi, esi
        mov    word ptr [ebp - 0x1c], si
        push   eax
        push   6
        mov    edx, 0                        ; -> obj7:0x0984
        push   esi
        xor    ecx, ecx
        mov    dword ptr [ebp - 0x20], 0x7f02
        mov    edi, 0x10040
        call   sub_5_1240
        mov    word ptr [0], ax              ; -> obj7:0x0098
        push   0                             ; -> obj7:0x0990
        push   esi
        push   edi
        call   sub_5_D208
        add    esp, 0xc
        mov    dword ptr [ebp - 0x10], eax
        cmp    eax, esi
        je     loc_5_5D07
        lea    eax, [ebp - 0x18]
        mov    dword ptr [ebp - 0x14], esi
        push   eax
        mov    dword ptr [ebp - 0x18], esi
        push   0xc
        push   0x201
        push   0                             ; -> loc_7_099C
        call   sub_5_D104
        add    esp, 0x10
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 0x20
        jb     loc_5_5CFC
        push   0                             ; -> obj7:0x09A8
        push   esi
        push   edi
        call   sub_5_D208
        add    esp, 0xc
        mov    dword ptr [ebp - 8], eax
        cmp    eax, esi
        je     loc_5_5CE4
        lea    eax, [ebp - 8]
        push   eax
        push   8
        push   0x203
        push   0                             ; -> loc_7_09C0
        call   sub_5_D104
        add    esp, 0x10
        mov    dword ptr [0], eax            ; -> obj7:0x0084
        push   dword ptr [ebp - 8]
        call   sub_5_D200
        add    esp, 4
loc_5_5CE4:
        lea    eax, [ebp - 4]
        push   eax
        push   4
        push   0x202
        push   0                             ; -> obj7:0x09CC
        call   sub_5_D104
        add    esp, 0x10
loc_5_5CFC:
        push   dword ptr [ebp - 0x10]
        call   sub_5_D200
        add    esp, 4
loc_5_5D07:
        cmp    dword ptr [0], 0              ; -> obj7:0x0088
        jne    loc_5_5D15
        call   sub_5_5BE4
loc_5_5D15:
        test   byte ptr [0], 0x80            ; -> obj7:0x00AC
        je     loc_5_5D5B
        test   byte ptr [1], 0x18            ; -> obj7:0x00AD
        je     loc_5_5D51
        and    byte ptr [0], 0x7f            ; -> obj7:0x00AC
        push   0
        push   8
        call   sub_5_6710
        add    esp, 4
        mov    edi, eax
        push   9
        call   sub_5_6710
        add    esp, 4
        mov    edx, edi
        mov    ecx, eax
        call   sub_5_5EA8
        jmp    loc_5_5D5B
loc_5_5D51:
        push   1
        call   sub_5_7092
        add    esp, 4
loc_5_5D5B:
        or     byte ptr [0], 4               ; -> obj7:0x00AC
        test   byte ptr [1], 0x20            ; -> obj7:0x00AD
        je     loc_5_5D77
        call   sub_4_0354
loc_5_5D70:
        and    byte ptr [1], 0xdf            ; -> obj7:0x00AD
loc_5_5D77:
        test   byte ptr [1], 0x10            ; -> obj7:0x00AD
        je     loc_5_5D85
        call   sub_5_22AF
loc_5_5D85:
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        inc    dword ptr [0]                 ; -> obj7:0x008C
        push   eax
        call   sub_5_79CB
        add    esp, 4
        test   byte ptr [0], 0x80            ; -> obj7:0x00AC
        je     loc_5_5E11
        and    byte ptr [0], 0x7f            ; -> obj7:0x00AC
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        push   eax
        call   sub_5_79CB
        add    esp, 4
        mov    ecx, 0x20
        call   sub_5_231C
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        push   eax
        call   sub_5_79CB
        add    esp, 4
        push   0
        call   sub_5_7092
        add    esp, 4
        lea    edx, [ebp - 0xc]
        mov    ecx, dword ptr [0]            ; -> obj7:0x0088
        call   sub_5_5BA3
        test   eax, eax
        je     loc_5_5E0C
        push   4
        lea    eax, [ebp - 0xc]
        and    byte ptr [ebp - 0xc], 0xfe
        push   eax
        push   3
        push   0
        push   0                             ; -> obj7:0x09D8
        push   dword ptr [0]                 ; -> obj7:0x0088
        call   sub_5_D0EC
        add    esp, 0x18
loc_5_5E0C:
        mov    esi, 1
loc_5_5E11:
        cmp    dword ptr [0], 0              ; -> obj7:0x0088
        je     loc_5_5E28
        mov    eax, dword ptr [0]            ; -> obj7:0x0088
        push   eax
        call   sub_5_D0E4
        add    esp, 4
loc_5_5E28:
        dec    dword ptr [0]                 ; -> obj7:0x008C
        call   sub_5_73CA
        test   esi, esi
        je     loc_5_5E44
        push   0x80000002
        call   sub_5_D0DC
        add    esp, 4
loc_5_5E44:
        or     byte ptr [1], 0x80            ; -> obj7:0x00AD
        call   sub_5_73F9
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_5E56:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 8], 0x219
        jne    loc_5_5EA1
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0x20
        je     loc_5_5E79
        cmp    eax, 0x22
        je     loc_5_5E8C
        cmp    eax, 0x7fff
        je     loc_5_5E99
        jmp    loc_5_5EA1
loc_5_5E79:
        xor    eax, eax
        mov    ecx, 0                        ; -> loc_5_5C33
        push   eax
        xor    edx, edx
        push   eax
        push   eax
        call   sub_5_02D3
        jmp    loc_5_5EA1
loc_5_5E8C:
        push   dword ptr [ebp + 0x10]
        call   sub_5_0094
        add    esp, 4
        jmp    loc_5_5EA1
loc_5_5E99:
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_5_020F
loc_5_5EA1:
        mov    eax, 1
        pop    ebp
        ret    
sub_5_5EA8:
        cmp    dword ptr [esp + 4], 1
        sbb    eax, eax
        push   ecx
        push   edx
        and    eax, 0x4ffff030
        sub    eax, 0x3ffff000
        push   eax
        push   dword ptr [0]                 ; -> obj7:0x009C
        call   sub_5_D210
        ret    4
sub_5_5ECA:
        cmp    dword ptr [esp + 4], 1
        sbb    eax, eax
        push   ecx
        push   edx
        and    eax, 0x4ffff030
        sub    eax, 0x3fffeffc
        push   eax
        push   dword ptr [0]                 ; -> obj7:0x009C
        call   sub_5_D210
        sub    eax, 6
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        ret    4
sub_5_5EF6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x304
        push   esi
        push   edi
        mov    edi, ecx
        push   0xe
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 0x104]
        push   eax
        push   ecx
        call   sub_5_D0FC
        add    esp, 8
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 4], 0x100
        push   0
        lea    edx, [ebp - 0x104]
        push   ecx
        push   edx
        push   1
        xor    edx, edx
        push   0                             ; -> obj7:0x09E4
        mov    ecx, edi
        call   sub_5_87D6
        lea    ecx, [ebp - 0x104]
        push   ecx
        push   0xc
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 0x304]
        lea    esi, [ebp - 0x304]
        push   eax
        push   ecx
        call   sub_5_D0FC
        add    esp, 0xc
        push   0
        push   0xd
        call   sub_5_6710
        add    esp, 4
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_5ECA
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_5F83:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj7:0x00AC
        mov    ebp, esp
        sub    esp, 4
        test   al, 2
        push   ebx
        push   esi
        push   edi
        je     loc_5_5FF5
        test   al, 8
        jne    loc_5_5FF5
        cmp    dword ptr [0], 1              ; -> obj7:0x008C
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 4], eax
        call   sub_5_C73C
        push   dword ptr [ebp + 0xc]
        mov    si, ax
        lea    edx, [ebp - 4]
        push   dword ptr [ebp + 8]
        push   0x219
        push   edx
        mov    edi, dword ptr [ebp + 0x10]
        cmp    edi, 1
        sbb    eax, eax
        add    eax, 0x69
        push   eax
        call   sub_5_D07C
        add    esp, 0x14
        mov    ebx, eax
        mov    ecx, esi
        call   sub_5_C77D
        test   edi, edi
        je     loc_5_5FF5
        test   ebx, ebx
        je     loc_5_5FEE
        cmp    ebx, 1
        je     loc_5_5FF5
        mov    eax, 0x13
        jmp    loc_5_5FF7
loc_5_5FEE:
        mov    eax, 0x2a
        jmp    loc_5_5FF7
loc_5_5FF5:
        xor    eax, eax
loc_5_5FF7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_5FFE:
        push   esi
        push   dword ptr [esp + 0xc]
        push   0
        push   dword ptr [esp + 0x10]
        call   sub_5_5F83
        add    esp, 0xc
        test   eax, eax
        je     loc_5_603D
        cmp    eax, 0x2a
        je     loc_5_6044
        push   0
        push   0xa
        call   sub_5_6710
        add    esp, 4
        mov    esi, eax
        push   0xb
        call   sub_5_6710
        add    esp, 4
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_5ECA
        jmp    loc_5_6046
loc_5_603D:
        mov    eax, 1
        jmp    loc_5_6046
loc_5_6044:
        xor    eax, eax
loc_5_6046:
        pop    esi
        ret    
sub_5_6048:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        mov    dword ptr [ebp - 0x10], 0x10
        mov    dword ptr [ebp - 0xc], 1
        mov    dword ptr [ebp - 8], 0
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], ecx
        mov    esi, ecx
        push   dword ptr [ebp + 8]
        lea    ecx, [ebp - 0x10]
        push   ecx
        push   edx
        call   sub_5_5F83
        add    esp, 0xc
        test   eax, eax
        je     loc_5_608C
        cmp    eax, 0x2a
        je     loc_5_6093
        mov    ecx, esi
        call   sub_5_5EF6
        jmp    loc_5_6095
loc_5_608C:
        mov    eax, 1
        jmp    loc_5_6095
loc_5_6093:
        xor    eax, eax
loc_5_6095:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_609D:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        mov    esi, 0                        ; -> loc_7_09F4
        lea    edi, [esp + 0xc]
        lea    eax, [esp + 0xc]
        push   edx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        push   0                             ; -> loc_7_0A00
        push   0                             ; -> loc_7_0A08
        push   eax
        call   sub_5_D0FC
        lea    edx, [esp + 0x1c]
        add    esp, 0x10
        mov    ecx, ebx
        push   0
        push   0
        call   sub_5_86BE
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_5_60E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    eax, 1
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ecx + 0x38]
        mov    byte ptr [ebp - 0x10], bl
        mov    dword ptr [ebp - 0xf], ebx
        mov    dword ptr [ebp - 0xb], ebx
        mov    dword ptr [ebp - 7], eax
        test   esi, esi
        je     loc_5_61C7
        lea    eax, [ebp - 0x14]
        lea    ecx, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], 0xd
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> loc_7_0A10
        push   edx
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_61C2
        movzx  edx, byte ptr [ebp - 0x10]
        mov    ecx, esi
        call   sub_5_189A
        test   eax, eax
        jne    loc_5_6145
        xor    eax, eax
        jmp    loc_5_61C7
loc_5_6145:
        mov    ecx, dword ptr [eax + 4]
        cmp    ecx, 1
        je     loc_5_6156
        cmp    ecx, 2
        je     loc_5_618B
        xor    eax, eax
        jmp    loc_5_61C7
loc_5_6156:
        mov    ecx, dword ptr [eax + 0x20]
        add    eax, 0x1c
        mov    edx, dword ptr [ebp - 0xf]
        mov    eax, dword ptr [eax + 8]
        add    edx, ecx
        lea    esi, [eax + 1]
        cmp    edx, esi
        jae    loc_5_6187
        cmp    dword ptr [ebp - 0xb], 0
        je     loc_5_6187
        sub    eax, ecx
        sub    edx, edx
        sub    eax, dword ptr [ebp - 0xf]
        mov    ecx, dword ptr [ebp - 7]
        inc    eax
        div    dword ptr [ebp - 0xb]
        cmp    eax, ecx
        jb     loc_5_61C7
        mov    eax, ecx
        jmp    loc_5_61C7
loc_5_6187:
        xor    eax, eax
        jmp    loc_5_61C7
loc_5_618B:
        add    eax, 0x1c
        mov    edx, dword ptr [ebp - 0xf]
        movzx  ecx, word ptr [eax + 4]
        movzx  eax, word ptr [eax + 6]
        add    edx, ecx
        lea    esi, [eax + 1]
        cmp    edx, esi
        jae    loc_5_61BE
        cmp    dword ptr [ebp - 0xb], 0
        je     loc_5_61BE
        sub    eax, ecx
        sub    edx, edx
        sub    eax, dword ptr [ebp - 0xf]
        mov    ecx, dword ptr [ebp - 7]
        inc    eax
        div    dword ptr [ebp - 0xb]
        cmp    eax, ecx
        jb     loc_5_61C7
        mov    eax, ecx
        jmp    loc_5_61C7
loc_5_61BE:
        xor    eax, eax
        jmp    loc_5_61C7
loc_5_61C2:
        mov    eax, 1
loc_5_61C7:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_61CD:
        push   esi
        push   edi
        mov    edi, edx
        lea    esi, [ecx + 0x38]
        mov    eax, dword ptr [esi]
        test   eax, eax
        jne    loc_5_61F4
        mov    eax, dword ptr [ecx + 0x20]
        push   0
        push   2
        mov    eax, dword ptr [eax + 0x38]
        mov    edx, dword ptr [eax + 0xc]
        call   sub_5_16B4
        test   eax, eax
        jne    loc_5_61F4
        xor    eax, eax
        jmp    loc_5_6233
loc_5_61F4:
        mov    edx, edi
        mov    ecx, eax
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        call   sub_5_1761
        test   eax, eax
        jne    loc_5_6214
        mov    ecx, esi
        call   sub_5_1596
        xor    eax, eax
        jmp    loc_5_6233
loc_5_6214:
        cmp    dword ptr [eax + 4], 4
        jne    loc_5_621E
        or     byte ptr [eax + 0x1c], 1
loc_5_621E:
        cmp    edi, 1
        jb     loc_5_622E
        cmp    edi, 4
        ja     loc_5_622E
        inc    dword ptr [0]                 ; -> loc_7_09F0
loc_5_622E:
        mov    eax, 1
loc_5_6233:
        pop    edi
        pop    esi
        ret    8
sub_5_6238:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x148
        push   ebx
        push   esi
        mov    edx, dword ptr [ecx + 0x20]
        push   edi
        mov    esi, ecx
        call   sub_5_1D46
        mov    ecx, dword ptr [eax]
        movzx  edi, cx
        mov    eax, ecx
        lea    ecx, [esi + 0x38]
        shr    eax, 0x10
        xor    ebx, ebx
        movzx  edx, ax
        mov    dword ptr [ebp - 0x10], edx
        call   sub_5_15ED
        mov    ecx, dword ptr [esi + 0x20]
        mov    eax, dword ptr [ecx + 0x38]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, ebx
        je     loc_5_6505
        mov    edx, edi
        call   sub_5_609D
        cmp    eax, ebx
        mov    edi, eax
        je     loc_5_6505
        lea    eax, [ebp - 0x38]
        lea    ecx, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        mov    dword ptr [ebp - 0x38], 4
        push   ecx
        push   ebx
        push   ebx
        push   0                             ; -> obj7:0x0A24
        push   edi
        call   sub_5_D11C
        add    esp, 0x18
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0x100
        push   ecx
        lea    eax, [ebp - 0x148]
        push   eax
        push   ebx
        push   ebx
        push   0                             ; -> loc_7_0A2C
        push   edi
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        je     loc_5_62D5
        mov    dword ptr [ebp - 8], ebx
loc_5_62D5:
        lea    eax, [ebp - 0x34]
        lea    ecx, [ebp - 0x20]
        push   eax
        xor    ebx, ebx
        mov    dword ptr [ebp - 0x34], 0xd
        push   ecx
        push   ebx
        push   ebx
        push   0                             ; -> loc_7_0A38
        push   edi
        call   sub_5_D11C
        add    esp, 0x18
        cmp    eax, 1
        sbb    eax, eax
        push   edi
        neg    eax
        mov    dword ptr [ebp - 0x30], eax
        call   sub_5_D0E4
        add    esp, 4
        mov    dword ptr [0], ebx            ; -> loc_7_09F0
        cmp    dword ptr [ebp - 8], ebx
        jbe    loc_5_6366
loc_5_6314:
        movzx  edx, byte ptr [ebp + ebx - 0x148]
        mov    ecx, dword ptr [ebp - 0xc]
        call   sub_5_189A
        test   eax, eax
        jne    loc_5_6347
        test   byte ptr [ebp - 4], 4
        je     loc_5_6394
        push   0
        mov    edx, 0x8000
        push   0
        mov    ecx, esi
        call   sub_5_61CD
        test   eax, eax
        jne    loc_5_6360
        jmp    loc_5_6505
loc_5_6347:
        lea    edx, [eax + 0x1c]
        mov    ecx, esi
        push   dword ptr [eax + 0x14]
        push   edx
        mov    edx, dword ptr [eax + 4]
        call   sub_5_61CD
        test   eax, eax
        je     loc_5_6505
loc_5_6360:
        inc    ebx
        cmp    dword ptr [ebp - 8], ebx
        ja     loc_5_6314
loc_5_6366:
        cmp    dword ptr [ebp - 0x30], 0
        je     loc_5_64BF
        mov    ecx, dword ptr [ebp - 0xc]
        inc    dword ptr [ebp - 8]
        movzx  edx, byte ptr [ebp - 0x20]
        call   sub_5_189A
        test   eax, eax
        jne    loc_5_63AD
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_6505
loc_5_6394:
        lea    ecx, [esi + 0x38]
        call   sub_5_1596
        mov    edx, 0x11
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_6505
loc_5_63AD:
        mov    ecx, dword ptr [eax + 4]
        cmp    ecx, 1
        je     loc_5_63CF
        cmp    ecx, 2
        je     loc_5_6443
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_6505
loc_5_63CF:
        mov    ecx, dword ptr [eax + 0x20]
        add    eax, 0x1c
        mov    edx, dword ptr [ebp - 0x1b]
        imul   edx, dword ptr [ebp - 0x10]
        add    edx, dword ptr [ebp - 0x1f]
        add    edx, ecx
        cmp    edx, ecx
        jb     loc_5_6432
        mov    ecx, dword ptr [ebp - 0x1b]
        lea    ecx, [ecx + edx - 1]
        cmp    dword ptr [eax + 8], ecx
        jb     loc_5_6432
        mov    dword ptr [ebp - 0x44], edx
        mov    dword ptr [ebp - 0x40], ecx
        mov    word ptr [ebp - 0x48], 0
        push   0x10
        lea    edx, [ebp - 0x48]
        mov    word ptr [ebp - 0x46], 0x14
        mov    cx, word ptr [eax + 0xc]
        push   edx
        mov    word ptr [ebp - 0x3c], cx
        mov    edx, 1
        mov    ax, word ptr [eax + 0xe]
        mov    ecx, esi
        mov    word ptr [ebp - 0x3a], ax
        call   sub_5_61CD
        test   eax, eax
        jne    loc_5_64BF
        jmp    loc_5_6505
loc_5_6432:
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_6505
loc_5_6443:
        add    eax, 0x1c
        mov    ecx, dword ptr [ebp - 0x1b]
        imul   ecx, dword ptr [ebp - 0x10]
        add    ecx, dword ptr [ebp - 0x1f]
        movzx  edi, word ptr [eax + 4]
        lea    edx, [ecx + edi]
        cmp    edx, edi
        jb     loc_5_64B1
        movzx  edi, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp - 0x1b]
        add    ecx, edx
        dec    ecx
        cmp    ecx, edi
        ja     loc_5_64B1
        mov    word ptr [ebp - 0x28], dx
        mov    ecx, dword ptr [ebp - 0x1b]
        mov    word ptr [ebp - 0x2c], 0
        mov    word ptr [ebp - 0x2a], 0xc
        add    cx, dx
        dec    cx
        mov    word ptr [ebp - 0x26], cx
        push   0xc
        mov    dx, word ptr [eax + 8]
        lea    ecx, [ebp - 0x2c]
        mov    word ptr [ebp - 0x24], dx
        push   ecx
        mov    bl, byte ptr [eax + 0xa]
        mov    edx, 2
        mov    ecx, esi
        mov    byte ptr [ebp - 0x22], bl
        mov    al, byte ptr [eax + 0xb]
        mov    byte ptr [ebp - 0x21], al
        call   sub_5_61CD
        test   eax, eax
        jne    loc_5_64BF
        jmp    loc_5_6505
loc_5_64B1:
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_6505
loc_5_64BF:
        test   byte ptr [ebp - 4], 1
        jne    loc_5_64E2
        cmp    dword ptr [0], 0              ; -> loc_7_09F0
        jne    loc_5_64F7
        test   byte ptr [ebp - 4], 2
        jne    loc_5_64EE
        mov    edx, 0x1d
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_64F7
loc_5_64E2:
        cmp    dword ptr [esi + 0x38], 0
        jne    loc_5_64F7
        test   byte ptr [ebp - 4], 2
        je     loc_5_64F7
loc_5_64EE:
        push   esi
        call   sub_5_9014
        add    esp, 4
loc_5_64F7:
        push   0x80000000
        mov    ecx, esi
        push   0
        call   sub_5_1906
loc_5_6505:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_650C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xd8
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
loc_5_651A:
        mov    edx, esi
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_609D
        mov    edi, eax
        test   edi, edi
        je     loc_5_65F9
        mov    edx, edi
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_60E0
        push   edi
        mov    dword ptr [ebp - 8], eax
        xor    edi, edi
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 1
        ja     loc_5_6553
        mov    eax, 1
loc_5_6553:
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_5_65F3
        mov    ebx, esi
loc_5_6560:
        cmp    dword ptr [ebp - 8], 1
        ja     loc_5_657D
        push   esi
        lea    eax, [ebp - 0xd8]
        push   0                             ; -> obj7:0x0A4C
        push   eax
        call   sub_5_D0FC
        add    esp, 0xc
        jmp    loc_5_6593
loc_5_657D:
        push   edi
        lea    eax, [ebp - 0xd8]
        push   esi
        push   0                             ; -> loc_7_0A5C
        push   eax
        call   sub_5_D0FC
        add    esp, 0x10
loc_5_6593:
        push   0
        lea    ecx, [ebp - 0xd8]
        push   dword ptr [ebp + 8]
        push   ecx
        lea    edx, [ebp - 4]
        push   edx
        call   _CONFIGMG_Create_DevNode
        add    esp, 0x10
        test   eax, eax
        je     loc_5_65B4
        cmp    eax, 0x10
        jne    loc_5_65E3
loc_5_65B4:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_1D46
        push   0
        mov    dword ptr [eax], ebx
        push   0x20000
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_1906
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_65E3
        mov    edx, 9
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_731F
loc_5_65E3:
        add    ebx, 0x10000
        inc    edi
        cmp    edi, dword ptr [ebp - 0xc]
        jb     loc_5_6560
loc_5_65F3:
        inc    esi
        jmp    loc_5_651A
loc_5_65F9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_6600:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x404
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        mov    edx, ecx
        mov    edi, ecx
        mov    ecx, esi
        xor    ebx, ebx
        call   sub_5_1D46
        movzx  edx, word ptr [eax]
        mov    ecx, edi
        call   sub_5_609D
        mov    edi, eax
        test   edi, edi
        je     loc_5_66BC
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x404]
        mov    dword ptr [ebp - 4], 0x400
        push   eax
        push   ecx
        push   ebx
        push   ebx
        push   0                             ; -> obj7:0x0A78
        push   edi
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_6670
        push   ebx
        lea    ecx, [ebp - 0x404]
        push   dword ptr [ebp - 4]
        push   ecx
        push   1
        xor    edx, edx
        push   0                             ; -> loc_7_0A84
        mov    ecx, esi
        call   sub_5_886D
loc_5_6670:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x404]
        push   eax
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], 0x400
        push   ecx
        push   ebx
        push   ebx
        push   0                             ; -> loc_7_0A90
        push   edi
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_66B3
        push   ebx
        lea    ecx, [ebp - 0x404]
        push   dword ptr [ebp - 4]
        push   ecx
        push   1
        xor    edx, edx
        push   0                             ; -> loc_7_0AA0
        mov    ecx, esi
        call   sub_5_886D
loc_5_66B3:
        push   edi
        call   sub_5_D0E4
        add    esp, 4
loc_5_66BC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_66C3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    eax, 5
        je     loc_5_66DB
        cmp    eax, 6
        je     loc_5_6700
        mov    eax, 1
        jmp    loc_5_670D
loc_5_66DB:
        push   0
        push   dword ptr [ebp + 0x10]
        call   _CONFIGMG_Reset_Children_Marks
        add    esp, 8
        push   dword ptr [ebp + 0x10]
        call   sub_5_650C
        add    esp, 4
        push   dword ptr [ebp + 0x10]
        call   sub_5_909C
        add    esp, 4
        jmp    loc_5_670B
loc_5_6700:
        mov    edx, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_5_6600
loc_5_670B:
        xor    eax, eax
loc_5_670D:
        pop    ebp
        ret    
        db  0CCh
sub_5_6710:
        mov    ecx, dword ptr [esp + 4]
        movzx  eax, word ptr [ecx*2 + 0xa]   ; -> obj2:0x000A
        add    eax, dword ptr [4]            ; -> obj2:0x0004
        ret    
sub_5_6723:
        cmp    edx, 1
        sbb    eax, eax
        and    eax, 0x80000000
        push   eax
        push   8
        push   0x484e5950
        push   0xc
        push   ecx
        call   sub_5_D12C
        add    esp, 0x14
        ret    
sub_5_6741:
        cmp    edx, 1
        sbb    eax, eax
        and    eax, 0x80000000
        push   eax
        push   0x14
        push   0x454e5950
        push   0x18
        push   ecx
        call   sub_5_D12C
        add    esp, 0x14
        ret    
sub_5_675F:
        push   0x201
        push   0xc
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        je     loc_5_6779
        mov    dword ptr [eax + 8], 0x484e5950
loc_5_6779:
        ret    
sub_5_677A:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        test   eax, eax
        je     loc_5_6785
        mov    dword ptr [eax + 4], edx
loc_5_6785:
        cmp    dword ptr [ecx + 4], 0
        jne    loc_5_678E
        mov    dword ptr [ecx + 4], edx
loc_5_678E:
        mov    dword ptr [ecx], edx
        ret    
sub_5_6791:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    esi, ecx
        push   0x201
        push   0x18
        call   sub_5_D134
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     loc_5_67C0
        mov    edx, edi
        mov    ecx, esi
        mov    dword ptr [edi + 0x14], 0x454e5950
        mov    dword ptr [edi + 8], ebx
        call   sub_5_677A
loc_5_67C0:
        cmp    edi, 1
        pop    edi
        sbb    eax, eax
        pop    esi
        inc    eax
        pop    ebx
        ret    
loc_5_67CA:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_7_0AB4
        add    esp, 4
        test   eax, eax
        je     loc_5_6807
        push   0x201
        push   0x18
        call   sub_5_D134
        add    esp, 8
        mov    edx, eax
        test   edx, edx
        je     loc_5_680E
        mov    ecx, dword ptr [0]            ; -> loc_7_0AB0
        mov    dword ptr [edx + 8], esi
        mov    dword ptr [edx + 0x14], 0x454e5950
        call   sub_5_677A
loc_5_6807:
        mov    eax, 1
loc_5_680C:
        pop    esi
        ret    
loc_5_680E:
        xor    eax, eax
        jmp    loc_5_680C
sub_5_6812:
        push   0                             ; -> loc_5_67CA
        mov    dword ptr [0], ecx            ; -> loc_7_0AB0
        mov    ecx, 3
        mov    dword ptr [0], edx            ; -> sub_7_0AB4
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        call   sub_5_CBE2
        test   eax, eax
        mov    eax, 1
        jne    loc_5_6849
        mov    ecx, dword ptr [0]            ; -> loc_7_0AB0
        call   sub_5_684A
        xor    eax, eax
loc_5_6849:
        ret    
sub_5_684A:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ecx]
        mov    edi, ecx
        test   eax, eax
        je     loc_5_6875
        xor    ebx, ebx
loc_5_6857:
        mov    esi, dword ptr [eax]
        mov    dword ptr [eax + 0x14], ebx
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_686F
        push   ebx
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_686F:
        mov    eax, esi
        test   esi, esi
        jne    loc_5_6857
loc_5_6875:
        xor    eax, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_6880:
        sub    esp, 0x18
        push   esi
        push   edi
        mov    esi, edx
        lea    edi, [ecx + 8]
        lea    ecx, [esp + 0x10]
        push   0x10
        mov    edx, edi
        add    esi, 8
        call   sub_5_5B5D
        push   0x10
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_5B5D
        lea    edx, [esp + 0x10]
        push   0x10
        mov    ecx, esi
        call   sub_5_5B5D
        pop    edi
        pop    esi
        add    esp, 0x18
        ret    
sub_5_68B8:
        push   ebx
        push   esi
        mov    ebx, ecx
        push   edi
        cmp    edx, ecx
        push   ebp
        mov    ebp, edx
        mov    edi, edx
        mov    esi, ecx
        je     loc_5_6925
loc_5_68C8:
        mov    eax, dword ptr [ebx + 0x10]
        cmp    dword ptr [ebp + 0x10], eax
        jbe    loc_5_68D8
loc_5_68D0:
        mov    ebp, dword ptr [ebp + 4]
        cmp    dword ptr [ebp + 0x10], eax
        ja     loc_5_68D0
loc_5_68D8:
        cmp    ebx, ebp
        je     loc_5_6908
        mov    edx, ebp
        mov    ecx, ebx
        call   sub_5_6880
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebx + 0x10], eax
        jae    loc_5_68F6
loc_5_68EF:
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebx + 0x10], eax
        jb     loc_5_68EF
loc_5_68F6:
        cmp    ebx, ebp
        je     loc_5_6908
        mov    edx, ebp
        mov    ecx, ebx
        call   sub_5_6880
        mov    ebp, dword ptr [ebp + 4]
        jmp    loc_5_68C8
loc_5_6908:
        cmp    ebx, esi
        je     loc_5_6916
        mov    edx, dword ptr [ebx + 4]
        mov    ecx, esi
        call   sub_5_68B8
loc_5_6916:
        cmp    edi, ebp
        je     loc_5_6925
        mov    esi, dword ptr [ebp]
        mov    ebp, edi
        mov    ebx, esi
        cmp    edi, esi
        jne    loc_5_68C8
loc_5_6925:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_692A:
        mov    edx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ecx]
        jmp    sub_5_68B8
sub_5_6934:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        push   0
        push   8
        lea    edi, [esi + 0x1c]
        call   sub_5_1906
        mov    ebx, dword ptr [esi + 0x18]
        mov    ebp, dword ptr [edi]
        mov    eax, 0                        ; -> loc_5_1DB8
        push   esi
        mov    dword ptr [esi + 0x18], eax
        mov    dword ptr [edi], eax
        call   sub_5_1FB0
        add    esp, 4
        mov    ecx, esi
        inc    dword ptr [esi + 0x10]
        push   0
        push   8
        call   sub_5_1923
        mov    dword ptr [esi + 0x18], ebx
        mov    dword ptr [edi], ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_6976:
        push   esi
        mov    esi, dword ptr [0]            ; -> obj7:0x00B0
        add    esi, 0x24
        cmp    dword ptr [0], esi            ; -> obj7:0x00B4
        je     loc_5_69B0
loc_5_6988:
        test   byte ptr [esi + 0xb], 0x80
        je     loc_5_69A5
        mov    edx, 8
        mov    ecx, esi
        push   dword ptr [esi + 0x20]
        call   sub_5_0B16
        test   eax, eax
        jne    loc_5_69A5
        or     byte ptr [esi + 0xb], 0x20
loc_5_69A5:
        add    esi, 0x24
        cmp    dword ptr [0], esi            ; -> obj7:0x00B4
        jne    loc_5_6988
loc_5_69B0:
        call   sub_5_0A32
        pop    esi
        ret    
sub_5_69B7:
        push   esi
        mov    esi, ecx
        mov    ecx, edx
        call   sub_5_099C
        test   eax, eax
        je     loc_5_69D1
        cmp    eax, 2
        je     loc_5_69DD
        cmp    eax, 0x13
        je     loc_5_69F5
        jmp    loc_5_69FC
loc_5_69D1:
        call   sub_5_6976
        mov    eax, 1
        jmp    loc_5_69FC
loc_5_69DD:
        call   sub_5_09DB
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 1
        jmp    loc_5_69FC
loc_5_69F5:
        call   sub_5_09DB
        xor    eax, eax
loc_5_69FC:
        pop    esi
        ret    
sub_5_69FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    dword ptr [ebp - 0xc], edx
        mov    dword ptr [ebp - 8], 0
        push   esi
        push   edi
        cmp    dword ptr [ecx + 0x40], 1
        sbb    ebx, ebx
        mov    edi, ecx
        inc    ebx
        mov    edx, 2
        cmp    ebx, 1
        sbb    eax, eax
        mov    esi, dword ptr [edi + eax*4 + 0x40]
        mov    ecx, esi
        call   sub_5_14E6
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_5_6A49
        mov    edx, 3
        mov    ecx, edi
        call   sub_5_731F
        xor    eax, eax
        jmp    loc_5_6AE2
loc_5_6A49:
        test   byte ptr [ebp - 0xc], 0x20
        je     loc_5_6A5B
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        cmp    dword ptr [eax + 0x24], edi
        je     loc_5_6A5B
        xor    ebx, ebx
loc_5_6A5B:
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_058A
        test   eax, eax
        jne    loc_5_6A74
        lea    ecx, [ebp - 4]
        call   sub_5_1596
        xor    eax, eax
        jmp    loc_5_6AE2
loc_5_6A74:
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_0969
        test   eax, eax
        jne    loc_5_6A91
        mov    edx, 3
        mov    ecx, edi
        call   sub_5_731F
        xor    eax, eax
        jmp    loc_5_6AE2
loc_5_6A91:
        mov    edx, 1
        mov    ecx, edi
        call   sub_5_69B7
        test   eax, eax
        jne    loc_5_6ABE
        test   byte ptr [0], 4               ; -> obj7:0x00AC
        je     loc_5_6AE9
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_2F3A
        test   eax, eax
        je     loc_5_6AE9
        mov    dword ptr [ebp - 8], 1
loc_5_6ABE:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [edi + 0x38], eax
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        je     loc_5_6ADF
loc_5_6ACE:
        test   ecx, ecx
        je     loc_5_6ADF
        mov    dword ptr [eax + 8], ecx
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ecx + 0x10]
        test   eax, eax
        jne    loc_5_6ACE
loc_5_6ADF:
        mov    eax, dword ptr [ebp - 8]
loc_5_6AE2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_6AE9:
        test   ebx, ebx
        je     loc_5_6AFD
        mov    edx, 6
        mov    ecx, edi
        call   sub_5_69B7
        test   eax, eax
        jne    loc_5_6ABE
loc_5_6AFD:
        mov    eax, dword ptr [edi + 4]
        cmp    byte ptr [eax], 0
        je     loc_5_6B16
        mov    ecx, edi
        call   sub_5_1D31
        mov    edx, 0xf
        cmp    eax, 2
        jae    loc_5_6B1B
loc_5_6B16:
        mov    edx, 6
loc_5_6B1B:
        mov    ecx, edi
        call   sub_5_731F
        lea    ecx, [ebp - 4]
        call   sub_5_1596
        xor    eax, eax
        jmp    loc_5_6AE2
sub_5_6B2E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        xor    edi, edi
        mov    eax, 0x20000
        push   edi
        mov    dword ptr [ebp - 8], edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_6B5D
        push   0x10000000
        mov    ecx, esi
        push   edi
        call   sub_5_1906
loc_5_6B5D:
        lea    ecx, [esi + 0x40]
        lea    ebx, [esi + 0x34]
        call   sub_5_15ED
        push   4
        mov    edx, 0                        ; -> loc_7_0AB8
        mov    ecx, esi
        call   sub_5_8AD6
        mov    ecx, ebx
        call   sub_5_15ED
        push   1
        mov    edx, 0                        ; -> obj7:0x0AC8
        push   1
        mov    ecx, esi
        call   sub_5_8B0B
        test   eax, eax
        je     loc_5_6B98
        xor    eax, eax
        jmp    loc_5_6DAA
loc_5_6B98:
        cmp    dword ptr [ebx], 0
        je     loc_5_6BB0
        lea    ecx, [esi + 0x30]
        call   sub_5_15ED
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0x30], eax
        mov    dword ptr [ebx], 0
loc_5_6BB0:
        xor    ebx, ebx
        mov    ecx, esi
        push   ebx
        push   1
        push   ebx
        push   1
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_6C0E
        push   ebx
        lea    eax, [ebp - 0x10]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        xor    edx, edx
        push   3
        mov    ecx, esi
        mov    dword ptr [ebp - 0x10], 4
        push   0                             ; -> obj7:0x0AD4
        call   sub_5_87D6
        test   eax, eax
        jne    loc_5_6C0E
        mov    eax, 0x2000000
        mov    ecx, esi
        push   eax
        push   ebx
        push   eax
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_6C0E
        mov    edx, 0xd
        mov    ecx, esi
        call   sub_5_731F
        xor    eax, eax
        jmp    loc_5_6DAA
loc_5_6C0E:
        lea    eax, [ebp - 4]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [ebp - 4], 8
        je     loc_5_6C29
        lea    ecx, [esi + 0x3c]
        call   sub_5_15ED
loc_5_6C29:
        lea    eax, [ebp - 0xc]
        mov    edx, 1
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [ebp - 0xc], 4
        je     loc_5_6C52
        mov    edx, 0x16
        mov    ecx, esi
        call   sub_5_731F
        xor    eax, eax
        jmp    loc_5_6DAA
loc_5_6C52:
        cmp    dword ptr [esi + 0x40], 0
        jne    loc_5_6C5E
        cmp    dword ptr [esi + 0x3c], 0
        je     loc_5_6CAC
loc_5_6C5E:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        call   sub_5_69FE
        mov    edi, eax
        push   0
        mov    ecx, 0x400
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_6C87
        xor    eax, eax
        jmp    loc_5_6DAA
loc_5_6C87:
        test   byte ptr [ebp - 8], 0x10
        je     loc_5_6CAC
        test   byte ptr [ebp - 8], 0xf
        je     loc_5_6CAC
        cmp    dword ptr [esi + 0x40], 0
        jne    loc_5_6CAC
        mov    edx, 0x10
        mov    ecx, esi
        call   sub_5_731F
        xor    eax, eax
        jmp    loc_5_6DAA
loc_5_6CAC:
        push   0x10000000
        mov    ecx, esi
        push   0
        call   sub_5_1906
        test   byte ptr [ebp - 4], 1
        je     loc_5_6CC7
        mov    ecx, esi
        call   sub_5_6934
loc_5_6CC7:
        test   byte ptr [ebp - 0xc], 1
        je     loc_5_6CE2
        mov    ecx, esi
        call   sub_5_6934
        push   0x40000000
        mov    ecx, esi
        push   0
        call   sub_5_1906
loc_5_6CE2:
        test   byte ptr [ebp - 4], 0x20
        je     loc_5_6D18
        mov    eax, 0x1000000
        mov    ecx, esi
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_6D18
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        cmp    esi, eax
        je     loc_5_6D18
        cmp    dword ptr [eax + 0x24], esi
        je     loc_5_6D18
        mov    edx, 0x12
        mov    ecx, esi
        call   sub_5_731F
loc_5_6D18:
        test   byte ptr [ebp - 4], 0x40
        je     loc_5_6D40
        mov    eax, 0x1000000
        mov    ecx, esi
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_6D40
        mov    edx, 0x1c
        mov    ecx, esi
        call   sub_5_731F
loc_5_6D40:
        push   0
        mov    ecx, esi
        push   0
        push   0x200000
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_6DA8
        cmp    dword ptr [esi + 0x40], 0
        jne    loc_5_6DA8
        mov    eax, dword ptr [esi + 0x30]
        test   eax, eax
        je     loc_5_6DA8
        cmp    dword ptr [eax + 0xc], 0x7fff
        ja     loc_5_6DA8
        test   byte ptr [1], 2               ; -> obj7:0x00AD
        jne    loc_5_6DA8
        mov    ebx, dword ptr [eax + 0x10]
        test   ebx, ebx
        je     loc_5_6DA8
loc_5_6D7C:
        test   byte ptr [ebx + 5], 0x80
        jne    loc_5_6D93
        mov    ecx, ebx
        call   sub_5_0516
        test   eax, eax
        je     loc_5_6D9C
        test   byte ptr [eax + 8], 4
        je     loc_5_6D9C
loc_5_6D93:
        mov    ebx, dword ptr [ebx + 0x10]
        test   ebx, ebx
        jne    loc_5_6D7C
        jmp    loc_5_6DA8
loc_5_6D9C:
        mov    edx, 0x1a
        mov    ecx, esi
        call   sub_5_731F
loc_5_6DA8:
        mov    eax, edi
loc_5_6DAA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_6DB1:
        push   1
        mov    eax, dword ptr [0]            ; -> obj7:0x0AE4
        mov    edx, dword ptr [esp + 8]
        push   eax
        push   8
        mov    ecx, 1
        push   dword ptr [0]                 ; -> obj7:0x0AE0
        push   8
        call   sub_5_CEA8
        ret    
loc_5_6DD2:
        push   0
        mov    eax, dword ptr [0]            ; -> obj7:0x0AE0
        cmp    dword ptr [0], 1              ; -> obj7:0x0AE8
        sbb    ecx, ecx
        push   0
        mov    edx, dword ptr [esp + 0xc]
        push   -1
        neg    ecx
        push   eax
        push   8
        call   sub_5_CEA8
        mov    eax, 1
        ret    
loc_5_6DFA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        inc    dword ptr [0]                 ; -> obj7:0x008C
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], edi
        cmp    esi, edi
        jne    loc_5_6E9F
        mov    ebx, dword ptr [ebp + 0xc]
        and    ebx, 0x10
        cmp    ebx, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [0], eax            ; -> obj7:0x0AE0
        cmp    ebx, 1
        mov    eax, 3
        adc    eax, -1
        test   byte ptr [ebp + 0xc], 1
        mov    dword ptr [0], eax            ; -> obj7:0x0AE4
        je     loc_5_6E52
        mov    eax, 0x80000000
        or     dword ptr [0], eax            ; -> obj7:0x0AE0
        or     dword ptr [0], eax            ; -> obj7:0x0AE4
loc_5_6E52:
        push   0                             ; -> loc_5_6DB1
        lea    eax, [ebp - 4]
        push   eax
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        push   0
        mov    ecx, 2
        call   sub_5_CB5A
        test   eax, eax
        je     loc_5_6E8D
        mov    edx, 5
        mov    ecx, 2
        mov    dword ptr [0], 1              ; -> obj7:0x0AE8
        cmp    ebx, 1
        adc    edx, -1
        jmp    loc_5_6ECA
loc_5_6E8D:
        mov    edi, 0x80000002
        mov    edx, dword ptr [0]            ; -> obj7:0x0AE4
        mov    ecx, 2
        jmp    loc_5_6EC0
loc_5_6E9F:
        test   byte ptr [ebp + 0xc], 0x20
        mov    edx, 8
        jne    loc_5_6EBB
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, 7
        and    eax, 0x10
        cmp    eax, 1
        adc    edx, -1
loc_5_6EBB:
        mov    ecx, 3
loc_5_6EC0:
        mov    dword ptr [0], 0              ; -> obj7:0x0AE8
loc_5_6ECA:
        test   byte ptr [ebp + 0xc], 1
        mov    dword ptr [0], edx            ; -> obj7:0x0AE0
        je     loc_5_6EE2
        or     edx, 0x80000000
        mov    dword ptr [0], edx            ; -> obj7:0x0AE0
loc_5_6EE2:
        push   0                             ; -> loc_5_6DD2
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        push   0
        push   dword ptr [ebp - 4]
        call   sub_5_CB5A
        test   esi, esi
        je     loc_5_6F14
        cmp    dword ptr [0], 0              ; -> obj1:0x0204
        jne    loc_5_6F14
        push   1
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        push   eax
        call   _CONFIGMG_Reenumerate_DevNode
loc_5_6F11:
        add    esp, 8
loc_5_6F14:
        dec    dword ptr [0]                 ; -> obj7:0x008C
        test   esi, esi
        je     loc_5_6F2C
        cmp    dword ptr [0], 0              ; -> obj1:0x0204
        jne    loc_5_6F2C
        call   sub_5_73CA
loc_5_6F2C:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_6F35:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x78], edi
        je     loc_5_6FB5
        cmp    edi, 1
        je     loc_5_6F5D
        cmp    edi, 2
        je     loc_5_6F6D
        cmp    edi, 4
        je     loc_5_6F7D
        cmp    edi, 8
        je     loc_5_6F8D
        jmp    loc_5_6F9B
loc_5_6F5D:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 0xa
        jmp    loc_5_6F9B
loc_5_6F6D:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 0xc
        jmp    loc_5_6F9B
loc_5_6F7D:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 0xe
        jmp    loc_5_6F9B
loc_5_6F8D:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 0x10
loc_5_6F9B:
        push   eax
        mov    edx, esi
        push   8
        xor    ecx, ecx
        call   sub_5_CFA3
        test   eax, eax
        mov    eax, 0x13
        je     loc_5_6FB5
        xor    eax, eax
        mov    dword ptr [esi + 0x78], edi
loc_5_6FB5:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_5_6FB9:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 0x78]
        xor    eax, eax
        mov    dword ptr [edx], ecx
        ret    
sub_5_6FC9:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        or     dword ptr [eax + 0x7c], ecx
        xor    eax, eax
        ret    
sub_5_6FD7:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 0x7c]
        xor    eax, eax
        mov    dword ptr [edx], ecx
        ret    
sub_5_6FE7:
        sub    esp, 4
        mov    eax, dword ptr [ecx + 0xc]
        cmp    eax, 1
        je     loc_5_701B
        cmp    eax, 6
        je     loc_5_701B
        cmp    eax, 0xc
        jb     loc_5_7006
        cmp    eax, 0xf
        jbe    loc_5_701B
        cmp    eax, 0x12
        je     loc_5_701B
loc_5_7006:
        lea    eax, [esp]
        xor    edx, edx
        push   eax
        call   sub_5_8D6E
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        jmp    loc_5_7020
loc_5_701B:
        mov    eax, 1
loc_5_7020:
        add    esp, 4
        ret    
sub_5_7024:
        push   esi
        mov    eax, 0x400
        push   edi
        mov    esi, ecx
        xor    edi, edi
        push   edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_7056
        mov    eax, 0x20000000
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_7056
        dec    dword ptr [0]                 ; -> obj7:0x00A4
loc_5_7056:
        push   0x20000000
        mov    ecx, esi
        push   0x400
        call   sub_5_1923
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        cmp    dword ptr [esi + 0x10], eax
        je     loc_5_707F
        mov    edx, 0x16
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_708F
loc_5_707F:
        push   0
        mov    ecx, esi
        push   0x20
        call   sub_5_1906
        call   sub_5_7B5F
loc_5_708F:
        pop    edi
        pop    esi
        ret    
sub_5_7092:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [0], eax            ; -> obj7:0x0B04
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 0                        ; -> obj7:0x0B0C
        mov    ecx, 0                        ; -> loc_7_0B1C
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0xc
        push   0                             ; -> obj7:0x0AEC
        call   sub_5_1240
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_70C9:
        push   ebp
        mov    ecx, 0x40000
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_7271
        xor    edi, edi
        cmp    dword ptr [0], edi            ; -> obj7:0x0B08
        je     loc_5_7126
        cmp    dword ptr [esi + 0x40], edi
        jne    loc_5_7126
        mov    eax, 0x1000000
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_7126
        mov    eax, dword ptr [esi + 0x30]
        test   eax, eax
        je     loc_5_7126
        cmp    dword ptr [eax + 0xc], 0x7fff
        jbe    loc_5_7271
loc_5_7126:
        mov    edi, 0x1000000
        xor    ebx, ebx
        push   edi
        mov    ecx, esi
        push   ebx
        push   edi
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_7193
        push   ebx
        mov    ecx, esi
        push   0x400
        dec    dword ptr [0]                 ; -> obj7:0x00A4
        call   sub_5_1906
        mov    eax, 1
        push   esi
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0xc], eax
        call   sub_5_8780
        add    esp, 4
        lea    ecx, [ebp - 0xc]
        mov    edx, 0                        ; -> obj7:0x0B24
        push   ecx
        push   0xc
        mov    ecx, 0                        ; -> loc_7_0B34
        push   0                             ; -> obj7:0x0AEC
        call   sub_5_1240
        push   edi
        mov    ecx, esi
        push   0x400
        call   sub_5_1923
        mov    dword ptr [esi + 0xc], ebx
loc_5_7193:
        xor    edi, edi
        mov    eax, 0x400
        push   edi
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_7220
        mov    eax, 0x20000000
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_7220
        test   byte ptr [0], 0x80            ; -> obj7:0x00AC
        je     loc_5_71CC
        cmp    dword ptr [esi + 0xc], 0xc
        je     loc_5_7220
loc_5_71CC:
        push   0x20000000
        mov    ecx, esi
        push   0
        dec    dword ptr [0]                 ; -> obj7:0x00A4
        call   sub_5_1906
        push   esi
        call   sub_5_8780
        add    esp, 4
        mov    ecx, esi
        call   sub_5_6FE7
        test   eax, eax
        je     loc_5_7220
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [0]            ; -> obj7:0x0B04
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], eax
        mov    edx, 0                        ; -> obj7:0x0B3C
        mov    dword ptr [ebp - 0xc], ecx
        lea    ecx, [ebp - 0xc]
        push   ecx
        push   0xc
        push   0                             ; -> obj7:0x0AEC
        mov    ecx, 0                        ; -> loc_7_0B4C
        call   sub_5_1240
loc_5_7220:
        xor    edi, edi
        mov    ebx, 0x8000
        push   edi
        mov    ecx, esi
        push   ebx
        push   edi
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_7271
        push   edi
        mov    ecx, esi
        push   ebx
        dec    dword ptr [0]                 ; -> obj7:0x00A4
        call   sub_5_1923
        mov    eax, dword ptr [esi + 0x6c]
        push   esi
        mov    dword ptr [ebp - 0x10], esi
        mov    dword ptr [ebp - 0x14], eax
        call   sub_5_8780
        add    esp, 4
        lea    ecx, [ebp - 0x14]
        mov    edx, 0                        ; -> obj7:0x0B54
        push   ecx
        push   8
        mov    ecx, 0                        ; -> loc_7_0B68
        push   0                             ; -> obj7:0x0AF8
        call   sub_5_1240
loc_5_7271:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_727D:
        push   esi
        mov    edx, 0                        ; -> loc_5_70C9
        mov    esi, 1
        mov    ecx, esi
        mov    dword ptr [0], esi            ; -> obj7:0x0B08
        call   sub_5_CC30
        mov    edx, 0                        ; -> loc_5_70C9
        mov    ecx, esi
        mov    dword ptr [0], 0              ; -> obj7:0x0B08
        call   sub_5_CC30
        pop    esi
        ret    
sub_5_72AD:
        mov    ecx, 0                        ; -> obj7:0x0AEC
        call   sub_5_1409
        mov    ecx, 0                        ; -> obj7:0x0AF8
        jmp    sub_5_1409
sub_5_72C1:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        and    byte ptr [0], 0xdf            ; -> obj7:0x00AC
        lea    edx, [ebp - 4]
        call   sub_5_44B6
        mov    edx, dword ptr [ebp - 4]
        xor    ecx, ecx
        or     dl, 2
        call   sub_5_4572
        mov    eax, dword ptr [ebp - 4]
        mov    edx, 0                        ; -> obj7:0x0B70
        and    eax, 3
        mov    ecx, 0                        ; -> loc_7_0B7C
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        push   eax
        push   4
        push   0
        call   sub_5_1240
        and    dword ptr [ebp - 4], 0x7ffffffc
        mov    edx, dword ptr [ebp - 4]
        xor    ecx, ecx
        call   sub_5_4572
        call   sub_5_35C1
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_731F:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        mov    edi, edx
        xor    ebx, ebx
        mov    ebp, 0x400
        push   ebx
        push   ebp
        push   ebx
        push   ebp
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_738E
        push   ebx
        mov    ecx, esi
        push   ebp
        call   sub_5_1906
        push   ebx
        mov    ecx, esi
        push   8
        mov    dword ptr [esi + 0xc], edi
        push   ebx
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_737C
        call   sub_5_D0B8
        test   al, 2
        je     loc_5_736C
        push   esi
        call   sub_5_122C
        add    esp, 4
        jmp    loc_5_737C
loc_5_736C:
        push   0
        mov    ecx, 0                        ; -> sub_5_122C
        push   0
        mov    edx, esi
        call   sub_1_051C
loc_5_737C:
        inc    dword ptr [0]                 ; -> obj7:0x00A4
        call   sub_5_7B5F
        mov    ecx, esi
        call   sub_5_0BAC
loc_5_738E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_7393:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        push   0
        mov    ebx, 0x8000
        push   ebx
        push   0
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_73C6
        push   0
        mov    ecx, esi
        push   ebx
        call   sub_5_1906
        mov    dword ptr [esi + 0x6c], edi
        inc    dword ptr [0]                 ; -> obj7:0x00A4
        call   sub_5_7B5F
loc_5_73C6:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_73CA:
        xor    eax, eax
        cmp    dword ptr [0], eax            ; -> loc_7_0B8C
        jne    loc_5_73EE
        cmp    dword ptr [0], eax            ; -> obj1:0x04A8
        jne    loc_5_73EE
        push   eax
        mov    dword ptr [0], eax            ; -> loc_7_0B84
        push   7
        call   sub_5_5FFE
        add    esp, 8
        jmp    loc_5_73F8
loc_5_73EE:
        mov    dword ptr [0], 1              ; -> loc_7_0B84
loc_5_73F8:
        ret    
sub_5_73F9:
        call   sub_5_8438
        call   sub_5_088A
        call   sub_5_87CA
        call   sub_5_72AD
        call   sub_5_C7C2
        jmp    loc_5_142F
sub_5_7417:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        xor    esi, esi
        lea    edi, [ebx + 0x34]
        mov    ecx, edi
        mov    dword ptr [ebp - 4], esi
        call   sub_5_15ED
        mov    ebx, dword ptr [ebx + 0x30]
        cmp    ebx, esi
        jne    loc_5_743F
        mov    eax, 1
        jmp    loc_5_747B
loc_5_743F:
        test   ebx, ebx
        je     loc_5_7470
loc_5_7443:
        mov    edx, 1
        mov    ecx, ebx
        call   sub_5_14E6
        test   eax, eax
        je     loc_5_7468
        test   esi, esi
        je     loc_5_745B
        mov    dword ptr [esi], eax
        jmp    loc_5_745E
loc_5_745B:
        mov    dword ptr [ebp - 4], eax
loc_5_745E:
        mov    esi, eax
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    loc_5_7443
        jmp    loc_5_7470
loc_5_7468:
        lea    ecx, [ebp - 4]
        call   sub_5_15ED
loc_5_7470:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_5_747B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_7482:
        mov    eax, dword ptr [ecx + 0x10]
        test   eax, eax
        je     loc_5_7495
        xor    ecx, ecx
loc_5_748B:
        mov    dword ptr [eax + 8], ecx
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        jne    loc_5_748B
loc_5_7495:
        ret    
sub_5_7496:
        mov    eax, dword ptr [edx + 0x10]
        test   eax, eax
        je     loc_5_74BD
loc_5_749D:
        cmp    dword ptr [eax + 8], 0
        jne    loc_5_74B1
        mov    edx, dword ptr [eax + 4]
        xor    edx, dword ptr [ecx + 4]
        test   edx, 0xffff7fff
        je     loc_5_74BA
loc_5_74B1:
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        jne    loc_5_749D
        jmp    loc_5_74BD
loc_5_74BA:
        mov    dword ptr [eax + 8], ecx
loc_5_74BD:
        ret    
sub_5_74BE:
        mov    edx, dword ptr [ecx]
        push   0
        push   1
        push   0xf000
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edx + 4]
        call   sub_5_1674
        ret    
sub_5_74D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        mov    dword ptr [ebp - 4], ecx
        push   esi
        push   edi
        push   0x800000
        push   0
        call   sub_5_1923
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x3c]
        mov    dword ptr [ebp - 8], ecx
        test   ecx, ecx
        je     loc_5_7607
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x34
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x10], eax
        mov    esi, dword ptr [eax]
        test   esi, esi
        je     loc_5_75C5
loc_5_7515:
        cmp    dword ptr [esi + 0xc], 0x7fff
        ja     loc_5_75C5
        mov    ecx, esi
        call   sub_5_7482
        mov    ecx, dword ptr [ebp - 8]
        call   sub_5_7482
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [esi + 0x10]
        mov    dword ptr [ebp - 0x14], eax
        test   edi, edi
        je     loc_5_7584
        mov    ebx, 0x8000
loc_5_7542:
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, edi
        call   sub_5_7496
        test   eax, eax
        je     loc_5_756E
        test   dword ptr [eax + 4], ebx
        je     loc_5_7562
        test   dword ptr [edi + 4], ebx
        je     loc_5_756E
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 4], ebx
        jmp    loc_5_757D
loc_5_7562:
        lea    ecx, [edi + 4]
        mov    dword ptr [edi + 8], eax
        test   dword ptr [ecx], ebx
        mov    dword ptr [ecx], ebx
        jmp    loc_5_757D
loc_5_756E:
        push   0x800000
        mov    ecx, dword ptr [ebp - 4]
        push   0
        call   sub_5_1906
loc_5_757D:
        mov    edi, dword ptr [edi + 0x10]
        test   edi, edi
        jne    loc_5_7542
loc_5_7584:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        je     loc_5_75B7
loc_5_758E:
        cmp    dword ptr [eax + 8], 0
        je     loc_5_759D
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        jne    loc_5_758E
        jmp    loc_5_75B7
loc_5_759D:
        push   0x800000
        mov    ecx, dword ptr [ebp - 4]
        push   0
        call   sub_5_1906
        mov    ecx, dword ptr [ebp - 0x10]
        call   sub_5_74BE
        mov    esi, dword ptr [ebp - 0x10]
loc_5_75B7:
        mov    dword ptr [ebp - 0x10], esi
        mov    esi, dword ptr [ebp - 0x14]
        test   esi, esi
        jne    loc_5_7515
loc_5_75C5:
        mov    ecx, dword ptr [ebp - 8]
        call   sub_5_7482
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [eax]
        mov    eax, 0x800000
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_7607
        test   esi, esi
        je     loc_5_75F6
        cmp    dword ptr [esi + 0xc], 0x7fff
        jbe    loc_5_7607
loc_5_75F6:
        mov    edx, 0x1b
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_731F
        xor    eax, eax
        jmp    loc_5_760C
loc_5_7607:
        mov    eax, 1
loc_5_760C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_7613:
        push   esi
        mov    eax, 0x20000
        mov    esi, dword ptr [esp + 8]
        push   edi
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_76F8
        cmp    dword ptr [esi + 0x40], edi
        jne    loc_5_76F8
        mov    ecx, esi
        call   sub_5_7417
        test   eax, eax
        jne    loc_5_7658
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_76F8
loc_5_7658:
        push   1
        xor    eax, eax
        push   eax
        mov    edx, esi
        push   -1
        xor    ecx, ecx
        push   eax
        push   eax
        call   sub_5_CEA8
        test   eax, eax
        je     loc_5_76A3
        mov    ecx, esi
        call   sub_5_74D6
        test   eax, eax
        je     loc_5_76F8
        cmp    dword ptr [esi + 0x34], 0
        jne    loc_5_7693
        cmp    dword ptr [esi + 0x30], 0
        je     loc_5_7693
        mov    edx, 7
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_76F8
loc_5_7693:
        push   0
        mov    ecx, esi
        push   0x800
        call   sub_5_1906
        jmp    loc_5_76AF
loc_5_76A3:
        mov    edx, 7
        mov    ecx, esi
        call   sub_5_731F
loc_5_76AF:
        test   byte ptr [0], 0x80            ; -> obj7:0x00AC
        je     loc_5_76F8
        mov    eax, 0x1000000
        xor    edi, edi
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_76F8
        mov    eax, 0x800000
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_76EC
        cmp    dword ptr [esi + 0x34], edi
        je     loc_5_76F8
        cmp    dword ptr [esi + 0x3c], edi
        jne    loc_5_76F8
loc_5_76EC:
        mov    edx, 0x20
        mov    ecx, esi
        call   sub_5_731F
loc_5_76F8:
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
sub_5_7700:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        xor    edi, edi
        push   edi
        push   4
        push   edi
        push   4
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_7764
        mov    ebx, 0                        ; -> obj7:0x01E8
        mov    edx, 1
        push   ebx
        mov    ecx, esi
        call   sub_5_1B18
        test   eax, eax
        je     loc_5_7764
        push   edi
        mov    ebp, 0x80000
        push   esi
        mov    ecx, esi
        push   esi
        push   edi
        push   ebp
        push   edi
        push   ebp
        call   sub_5_18E3
        mov    ecx, 5
        cmp    eax, 1
        sbb    edx, edx
        neg    edx
        call   sub_5_1E4B
        push   edi
        mov    ecx, esi
        push   ebp
        call   sub_5_1906
        push   ebx
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_1B18
loc_5_7764:
        push   0
        mov    ecx, esi
        push   0x20
        call   sub_5_1923
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_5_7774:
        push   esi
        mov    eax, 0x400
        mov    esi, dword ptr [esp + 8]
        push   edi
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_79C3
        push   edi
        mov    ecx, esi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_77CB
        push   edi
        mov    ecx, esi
        push   2
        push   edi
        push   2
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_77CB
        push   edi
        mov    ecx, esi
        push   0x20
        push   edi
        push   0x20
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_79C3
loc_5_77CB:
        mov    ecx, dword ptr [esi + 0x20]
        test   ecx, ecx
        je     loc_5_77E7
        push   0
        push   8
        push   0
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_79C3
loc_5_77E7:
        mov    ecx, dword ptr [esi + 0x74]
        test   ecx, ecx
        je     loc_5_780B
        push   0
        push   8
        push   0
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_780B
        mov    edx, 0x1f
        mov    ecx, esi
        call   sub_5_731F
loc_5_780B:
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_7862
        mov    eax, 0x10000000
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_7862
        push   edi
        mov    eax, 0x400
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_7862
        mov    ecx, esi
        call   sub_5_6B2E
        test   eax, eax
        je     loc_5_7858
        xor    eax, eax
        jmp    loc_5_79C8
loc_5_7858:
        call   sub_5_7B5F
        jmp    loc_5_79C3
loc_5_7862:
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   2
        push   edi
        push   2
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_7890
        push   edi
        mov    eax, 0x400
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_7890
        mov    ecx, esi
        call   sub_5_59D1
loc_5_7890:
        xor    edi, edi
        mov    eax, 0x800
        push   edi
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_78D4
        push   edi
        mov    ecx, esi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_78D4
        push   edi
        mov    eax, 0x400
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_78D4
        push   esi
        call   sub_5_7613
        add    esp, 4
loc_5_78D4:
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_792F
        push   edi
        mov    eax, 0x400
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_792F
        mov    eax, 0x80000000
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_792F
        mov    edx, dword ptr [esi + 0x34]
        mov    ecx, esi
        call   sub_5_058A
        test   eax, eax
        je     loc_5_792F
        mov    ecx, esi
        call   sub_5_2D7F
        test   eax, eax
        je     loc_5_792F
        xor    eax, eax
        jmp    loc_5_79C8
loc_5_792F:
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_7973
        push   edi
        mov    eax, 0x400
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_7973
        mov    eax, 0x80000000
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_7973
        push   esi
        call   sub_5_1033
        add    esp, 4
loc_5_7973:
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   0x20
        push   edi
        push   0x20
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_79C3
        push   edi
        mov    ecx, esi
        push   2
        push   edi
        push   2
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_79C3
        push   edi
        mov    ecx, esi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_79C3
        push   edi
        mov    eax, 0x400
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_79C3
        mov    ecx, esi
        call   sub_5_7700
loc_5_79C3:
        mov    eax, 1
loc_5_79C8:
        pop    edi
        pop    esi
        ret    
sub_5_79CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    word ptr [ebp - 2], 0
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> loc_7_0B8C
        jne    loc_5_79F3
        mov    esi, dword ptr [0]            ; -> loc_7_0B84
        call   sub_5_C73C
        mov    word ptr [ebp - 2], ax
loc_5_79F3:
        xor    edi, edi
        mov    ebx, 1
        mov    dword ptr [ebp - 8], 0xb
        inc    dword ptr [0]                 ; -> loc_7_0B8C
loc_5_7A07:
        push   0                             ; -> loc_5_7774
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, 3
        mov    dword ptr [0], edi            ; -> loc_7_0B84
        call   sub_5_CBE2
        test   eax, eax
        jne    loc_5_7A31
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        mov    dword ptr [0], ebx            ; -> loc_7_0B84
        mov    dword ptr [ebp + 8], eax
loc_5_7A31:
        or     esi, dword ptr [0]            ; -> loc_7_0B84
        test   byte ptr [0], 4               ; -> obj7:0x00AC
        je     loc_5_7A96
        inc    dword ptr [0]                 ; -> obj1:0x0204
        cmp    dword ptr [0], edi            ; -> obj7:0x00A4
        je     loc_5_7A67
        cmp    dword ptr [0], edi            ; -> loc_7_0B94
        jne    loc_5_7A67
        mov    dword ptr [0], ebx            ; -> loc_7_0B94
        call   sub_5_727D
        mov    dword ptr [0], edi            ; -> loc_7_0B94
loc_5_7A67:
        test   byte ptr [0], 0x20            ; -> obj7:0x00AC
        je     loc_5_7A90
        cmp    dword ptr [0], edi            ; -> loc_7_0B94
        jne    loc_5_7A8B
        mov    dword ptr [0], ebx            ; -> loc_7_0B94
        call   sub_5_72C1
        mov    dword ptr [0], edi            ; -> loc_7_0B94
        jmp    loc_5_7A90
loc_5_7A8B:
        call   sub_5_7B5F
loc_5_7A90:
        dec    dword ptr [0]                 ; -> obj1:0x0204
loc_5_7A96:
        cmp    dword ptr [0], edi            ; -> loc_7_0B84
        je     loc_5_7AA7
        dec    dword ptr [ebp - 8]
        jne    loc_5_7A07
loc_5_7AA7:
        dec    dword ptr [0]                 ; -> loc_7_0B8C
        jne    loc_5_7ADE
        test   esi, esi
        je     loc_5_7AB8
        call   sub_5_73CA
loc_5_7AB8:
        mov    ecx, dword ptr [ebp - 2]
        call   sub_5_C77D
        cmp    dword ptr [0], 0              ; -> obj1:0x04A8
        jne    loc_5_7AD0
        xor    ecx, ecx
        call   sub_5_3874
loc_5_7AD0:
        test   byte ptr [1], 0x80            ; -> obj7:0x00AD
        je     loc_5_7ADE
        call   sub_5_73F9
loc_5_7ADE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_7AE5:
        xor    eax, eax
        mov    dword ptr [0], eax            ; -> loc_7_0B88
        cmp    dword ptr [0], eax            ; -> obj1:0x04A8
        jne    loc_5_7B16
        cmp    dword ptr [0], eax            ; -> obj1:0x0204
        je     loc_5_7B08
        mov    dword ptr [0], 1              ; -> obj1:0x0208
        jmp    loc_5_7B16
loc_5_7B08:
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        push   eax
        call   sub_5_79CB
        add    esp, 4
loc_5_7B16:
        ret    
loc_5_7B17:
        push   esi
        dec    dword ptr [0]                 ; -> obj1:0x04A8
        mov    esi, dword ptr [esp + 8]
        test   esi, esi
        je     loc_5_7B4F
        push   0x80000000
        push   0
        push   0x59504c52
        push   0x8c
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        je     loc_5_7B4F
        push   0
        mov    ecx, esi
        push   0x20
        call   sub_5_1906
loc_5_7B4F:
        cmp    dword ptr [0], 0              ; -> obj1:0x04A8
        jne    loc_5_7B5D
        call   sub_5_7B5F
loc_5_7B5D:
        pop    esi
        ret    
sub_5_7B5F:
        cmp    dword ptr [0], 0              ; -> loc_7_0B8C
        je     loc_5_7B74
        mov    dword ptr [0], 1              ; -> loc_7_0B84
        jmp    loc_5_7BC1
loc_5_7B74:
        xor    ecx, ecx
        cmp    dword ptr [0], ecx            ; -> loc_7_0B88
        jne    loc_5_7BC1
        cmp    dword ptr [0], ecx            ; -> obj1:0x04A8
        jne    loc_5_7BC1
        mov    eax, dword ptr [0]            ; -> obj7:0x00AC
        test   al, 2
        je     loc_5_7BC1
        test   al, 8
        jne    loc_5_7BC1
        cmp    dword ptr [0], ecx            ; -> obj1:0x0204
        je     loc_5_7BA7
        mov    dword ptr [0], 1              ; -> obj1:0x0208
        jmp    loc_5_7BC1
loc_5_7BA7:
        push   0
        mov    ecx, 0                        ; -> loc_5_7AE5
        push   0
        xor    edx, edx
        mov    dword ptr [0], 1              ; -> loc_7_0B88
        call   sub_1_051C
loc_5_7BC1:
        ret    
sub_5_7BC2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        mov    ebx, ecx
        push   edi
        lea    esi, [ebp - 8]
        mov    edi, edx
        push   eax
        push   esi
        lea    eax, [ebp - 0xc]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_5_8473
        test   eax, eax
        je     loc_5_7C17
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_7C24
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, esi
        call   sub_5_84FB
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_5_7C09
        mov    eax, 2
        jmp    loc_5_7C92
loc_5_7C09:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], edi
        jmp    loc_5_7C90
loc_5_7C17:
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_5_7C24
        mov    eax, 0x13
        jmp    loc_5_7C92
loc_5_7C24:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        mov    ecx, dword ptr [eax]
        cmp    ecx, ebx
        jb     loc_5_7C32
        mov    ecx, ebx
loc_5_7C32:
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        mov    ecx, dword ptr [eax]
        cmp    ecx, edi
        ja     loc_5_7C42
        mov    ecx, edi
loc_5_7C42:
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], ecx
        test   ecx, ecx
        je     loc_5_7C90
loc_5_7C54:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xc], edi
        jae    loc_5_7C6F
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, esi
        call   sub_5_8530
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_5_7C54
        jmp    loc_5_7C90
loc_5_7C6F:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        dec    eax
        cmp    eax, edi
        ja     loc_5_7C90
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [ecx + 0xc], edx
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, esi
        call   sub_5_8530
loc_5_7C90:
        xor    eax, eax
loc_5_7C92:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_7C9B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 4]
        test   eax, eax
        je     loc_5_7D85
        cmp    dword ptr [eax + 8], esi
        jb     loc_5_7CC7
        mov    eax, dword ptr [ebx + 8]
        cmp    dword ptr [eax + 0xc], edi
        jbe    loc_5_7D85
loc_5_7CC7:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ecx
        push   edx
        push   ebx
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_8473
        test   eax, eax
        jne    loc_5_7D8C
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        add    ecx, 8
        cmp    dword ptr [ecx], esi
        jb     loc_5_7D0E
        cmp    dword ptr [eax + 0xc], edi
        jbe    loc_5_7CFF
        inc    edi
        mov    dword ptr [ecx], edi
        jmp    loc_5_7D8C
loc_5_7CFF:
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, ebx
        call   sub_5_8530
        mov    dword ptr [ebp - 4], eax
        jmp    loc_5_7D55
loc_5_7D0E:
        add    eax, 0xc
        cmp    dword ptr [eax], edi
        jbe    loc_5_7D46
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, ebx
        call   sub_5_84FB
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_5_7D2A
        xor    eax, eax
        jmp    loc_5_7D91
loc_5_7D2A:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [ecx + 0xc], edx
        mov    ecx, dword ptr [ebp - 8]
        inc    edi
        dec    esi
        mov    dword ptr [ecx + 8], edi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], esi
        jmp    loc_5_7D8C
loc_5_7D46:
        dec    esi
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], ecx
loc_5_7D55:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_7D8C
loc_5_7D5B:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xc], edi
        ja     loc_5_7D76
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, ebx
        call   sub_5_8530
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_5_7D5B
        jmp    loc_5_7D8C
loc_5_7D76:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        cmp    dword ptr [eax], edi
        ja     loc_5_7D8C
        inc    edi
        mov    dword ptr [eax], edi
        jmp    loc_5_7D8C
loc_5_7D85:
        mov    ecx, ebx
        call   sub_5_85F7
loc_5_7D8C:
        mov    eax, 1
loc_5_7D91:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_7D9A:
        sub    esp, 8
        push   esi
        mov    esi, ecx
        lea    ecx, [esp + 4]
        call   sub_5_856D
        lea    ecx, [esp + 4]
        mov    edx, dword ptr [esi + 4]
        call   sub_5_85B7
        pop    esi
        add    esp, 8
        ret    
sub_5_7DBA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, ecx
        lea    ecx, [ebp - 8]
        call   sub_5_856D
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    loc_5_7DE1
        lea    ecx, [ebp - 8]
        xor    edx, edx
        push   dword ptr [ebp + 8]
        call   sub_5_857D
        jmp    loc_5_7E39
loc_5_7DE1:
        cmp    dword ptr [esi + 8], 0
        mov    edx, 0
        jne    loc_5_7DF3
        mov    edx, dword ptr [esi + 0xc]
        mov    esi, dword ptr [esi + 4]
        inc    edx
loc_5_7DF3:
        test   esi, esi
        je     loc_5_7E18
loc_5_7DF7:
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [ebp + 8], eax
        jb     loc_5_7E18
        dec    eax
        lea    ecx, [ebp - 8]
        push   eax
        call   sub_5_857D
        test   eax, eax
        je     loc_5_7E30
        mov    edx, dword ptr [esi + 0xc]
        mov    esi, dword ptr [esi + 4]
        inc    edx
        test   esi, esi
        jne    loc_5_7DF7
loc_5_7E18:
        cmp    edx, dword ptr [ebp + 8]
        ja     loc_5_7E34
        push   dword ptr [ebp + 8]
        lea    ecx, [ebp - 8]
        call   sub_5_857D
        test   eax, eax
        jne    loc_5_7E34
        xor    eax, eax
        jmp    loc_5_7E39
loc_5_7E30:
        xor    eax, eax
        jmp    loc_5_7E39
loc_5_7E34:
        mov    eax, 1
loc_5_7E39:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_7E40:
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        lea    ecx, [esp + 8]
        mov    edx, dword ptr [esp + 0x14]
        call   sub_5_856D
        mov    esi, dword ptr [esi + 4]
        mov    edi, dword ptr [edi + 4]
        test   esi, esi
        je     loc_5_7EA2
loc_5_7E60:
        test   edi, edi
        je     loc_5_7EA2
        mov    edx, dword ptr [edi + 8]
        mov    eax, dword ptr [esi + 8]
        cmp    edx, eax
        ja     loc_5_7E70
        mov    edx, eax
loc_5_7E70:
        mov    eax, dword ptr [edi + 0xc]
        mov    ecx, dword ptr [esi + 0xc]
        cmp    eax, ecx
        jb     loc_5_7E7C
        mov    eax, ecx
loc_5_7E7C:
        cmp    eax, edx
        jb     loc_5_7E8E
        lea    ecx, [esp + 8]
        push   eax
        call   sub_5_857D
        test   eax, eax
        je     loc_5_7EAF
loc_5_7E8E:
        mov    eax, dword ptr [esi + 0xc]
        cmp    dword ptr [edi + 0xc], eax
        jbe    loc_5_7E9B
        mov    esi, dword ptr [esi + 4]
        jmp    loc_5_7E9E
loc_5_7E9B:
        mov    edi, dword ptr [edi + 4]
loc_5_7E9E:
        test   esi, esi
        jne    loc_5_7E60
loc_5_7EA2:
        mov    eax, 1
loc_5_7EA7:
        pop    edi
        pop    esi
        add    esp, 8
        ret    4
loc_5_7EAF:
        xor    eax, eax
        jmp    loc_5_7EA7
sub_5_7EB3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        not    esi
        mov    edi, ecx
        mov    eax, 1
        lea    ecx, [edx + esi]
        lea    ebx, [esi + 1]
        and    ebx, esi
        cmp    ebx, eax
        sbb    ebx, ebx
        add    ebx, eax
        cmp    ecx, edx
        mov    edx, eax
        adc    edx, -1
        or     ebx, edx
        lea    edx, [esi + 1]
        cmp    edx, eax
        sbb    edx, edx
        neg    edx
        or     ebx, edx
        cmp    dword ptr [ebp + 8], eax
        sbb    eax, eax
        neg    eax
        or     ebx, eax
        je     loc_5_7EFC
        xor    eax, eax
        jmp    loc_5_7F9A
loc_5_7EFC:
        mov    ebx, esi
        mov    eax, dword ptr [ebp + 8]
        not    ebx
        and    ebx, ecx
        lea    edx, [ebx + eax - 1]
        mov    eax, 1
        mov    ecx, eax
        cmp    dword ptr [ebp + 0x10], edx
        adc    ecx, -1
        cmp    edx, ebx
        adc    eax, -1
        or     ecx, eax
        je     loc_5_7F23
        xor    eax, eax
        jmp    loc_5_7F9A
loc_5_7F23:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        lea    eax, [ebp - 0xc]
        mov    ecx, ebx
        push   eax
        push   dword ptr [ebp + 0x14]
        call   sub_5_8473
        test   eax, eax
        jne    loc_5_7F93
loc_5_7F3D:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc]
        inc    eax
        je     loc_5_7FA3
        lea    ecx, [eax + esi]
        cmp    eax, ecx
        ja     loc_5_7FA7
        mov    ebx, esi
        mov    eax, dword ptr [ebp + 8]
        not    ebx
        and    ebx, ecx
        mov    edx, 1
        lea    ecx, [ebx + eax - 1]
        mov    eax, 1
        cmp    dword ptr [ebp + 0x10], ecx
        adc    edx, -1
        cmp    ecx, ebx
        adc    eax, -1
        or     edx, eax
        jne    loc_5_7FAB
loc_5_7F73:
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], edx
        test   edx, edx
        je     loc_5_7F85
        cmp    dword ptr [edx + 0xc], ebx
        jb     loc_5_7F73
loc_5_7F85:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_7F93
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 8], ecx
        jbe    loc_5_7F3D
loc_5_7F93:
        mov    eax, 1
        mov    dword ptr [edi], ebx
loc_5_7F9A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_5_7FA3:
        xor    eax, eax
        jmp    loc_5_7F9A
loc_5_7FA7:
        xor    eax, eax
        jmp    loc_5_7F9A
loc_5_7FAB:
        xor    eax, eax
        jmp    loc_5_7F9A
sub_5_7FAF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 4]
        test   esi, esi
        jne    loc_5_7FD0
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_85F7
        mov    eax, 1
        jmp    loc_5_8087
loc_5_7FD0:
        mov    edi, dword ptr [edx + 4]
        test   edi, edi
        jne    loc_5_7FE4
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_7D9A
        jmp    loc_5_8087
loc_5_7FE4:
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_856D
loc_5_7FEF:
        mov    edx, dword ptr [esi + 8]
loc_5_7FF2:
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [edi + 8]
        cmp    eax, ecx
        jae    loc_5_8012
        push   eax
        lea    ecx, [ebp - 8]
        call   sub_5_857D
        test   eax, eax
        je     loc_5_8060
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        je     loc_5_8064
        jmp    loc_5_7FEF
loc_5_8012:
        cmp    ecx, edx
        jbe    loc_5_8024
        dec    ecx
        push   ecx
        lea    ecx, [ebp - 8]
        call   sub_5_857D
        test   eax, eax
        je     loc_5_806B
loc_5_8024:
        mov    eax, dword ptr [edi + 0xc]
        inc    eax
        je     loc_5_806F
loc_5_802A:
        cmp    dword ptr [esi + 0xc], eax
        jae    loc_5_803D
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    loc_5_802A
        mov    eax, 1
        jmp    loc_5_8087
loc_5_803D:
        mov    edx, dword ptr [esi + 8]
        cmp    edx, eax
        ja     loc_5_8046
        mov    edx, eax
loc_5_8046:
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    loc_5_7FF2
        push   dword ptr [esi + 0xc]
        lea    ecx, [ebp - 8]
        call   sub_5_857D
        test   eax, eax
        jne    loc_5_8076
        xor    eax, eax
        jmp    loc_5_8087
loc_5_8060:
        xor    eax, eax
        jmp    loc_5_8087
loc_5_8064:
        mov    eax, 1
        jmp    loc_5_8087
loc_5_806B:
        xor    eax, eax
        jmp    loc_5_8087
loc_5_806F:
        mov    eax, 1
        jmp    loc_5_8087
loc_5_8076:
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [esi + 4]
        call   sub_5_85B7
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_5_8087:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_808F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_856D
        mov    esi, dword ptr [esi + 4]
        mov    edi, dword ptr [edi + 4]
loc_5_80AC:
        test   esi, esi
        je     loc_5_80FF
        test   edi, edi
        je     loc_5_80FF
        mov    edx, dword ptr [esi + 8]
        mov    ecx, dword ptr [edi + 8]
        cmp    edx, ecx
        jbe    loc_5_80C6
        mov    eax, esi
        mov    edx, ecx
        mov    esi, edi
        mov    edi, eax
loc_5_80C6:
        mov    eax, dword ptr [esi + 0xc]
        test   edi, edi
        mov    esi, dword ptr [esi + 4]
        je     loc_5_80F0
loc_5_80D0:
        cmp    dword ptr [edi + 0xc], eax
        ja     loc_5_80DC
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    loc_5_80D0
loc_5_80DC:
        test   edi, edi
        je     loc_5_80F0
        lea    ecx, [eax + 1]
        cmp    dword ptr [edi + 8], ecx
        ja     loc_5_80F0
        mov    eax, esi
        mov    esi, edi
        mov    edi, eax
        jmp    loc_5_80C6
loc_5_80F0:
        push   eax
        lea    ecx, [ebp - 8]
        call   sub_5_857D
        test   eax, eax
        jne    loc_5_80AC
        jmp    loc_5_8120
loc_5_80FF:
        lea    ecx, [ebp - 8]
        mov    edx, esi
        call   sub_5_85B7
        test   eax, eax
        je     loc_5_8120
        lea    ecx, [ebp - 8]
        mov    edx, edi
        call   sub_5_85B7
        test   eax, eax
        mov    eax, 1
        jne    loc_5_8122
loc_5_8120:
        xor    eax, eax
loc_5_8122:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_812A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        push   edi
        mov    esi, edx
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        je     loc_5_8155
        mov    eax, 0x25
        jmp    loc_5_824E
loc_5_8155:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        push   0
        push   ecx
        push   0
        push   0                             ; -> loc_7_0B98
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        je     loc_5_817F
        mov    esi, 0x25
        jmp    loc_5_8241
loc_5_817F:
        cmp    dword ptr [ebp - 0xc], 3
        jne    loc_5_823C
        test   byte ptr [ebp - 4], 7
        jne    loc_5_823C
        xor    edi, edi
        cmp    dword ptr [ebp - 4], edi
        je     loc_5_81BA
        push   0x201
        push   dword ptr [ebp - 4]
        call   sub_5_D134
        add    esp, 8
        mov    edi, eax
        test   eax, eax
        jne    loc_5_81BA
        mov    esi, 2
        jmp    loc_5_8241
loc_5_81BA:
        lea    ecx, [ebp - 0x14]
        mov    edx, esi
        call   sub_5_856D
        cmp    dword ptr [ebp - 4], 0
        jne    loc_5_81CE
        xor    esi, esi
        jmp    loc_5_8241
loc_5_81CE:
        mov    ebx, edi
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0xc]
        push   edi
        push   ecx
        push   0
        push   0                             ; -> obj7:0x0BAC
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 0x18
        test   eax, eax
        je     loc_5_81F6
        mov    esi, 0x25
        jmp    loc_5_8226
loc_5_81F6:
        xor    esi, esi
        cmp    dword ptr [ebp - 4], 8
        jb     loc_5_8226
loc_5_81FE:
        mov    edx, dword ptr [edi]
        lea    ecx, [ebp - 0x14]
        add    edi, 4
        mov    eax, dword ptr [edi]
        add    edi, 4
        push   eax
        call   sub_5_857D
        test   eax, eax
        je     loc_5_8221
        sub    dword ptr [ebp - 4], 8
        cmp    dword ptr [ebp - 4], 8
        jae    loc_5_81FE
        jmp    loc_5_8226
loc_5_8221:
        mov    esi, 2
loc_5_8226:
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_8241
        push   0
        push   ebx
        call   sub_5_D0C8
        add    esp, 8
        jmp    loc_5_8241
loc_5_823C:
        mov    esi, 0x1f
loc_5_8241:
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, esi
loc_5_824E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_8255:
        sub    esp, 4
        xor    eax, eax
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        push   ebp
        mov    edi, dword ptr [edx + 0xc]
        shl    edi, 3
        mov    ebp, edx
        test   edi, edi
        je     loc_5_8288
        push   0x201
        push   edi
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        jne    loc_5_8288
        mov    eax, 2
        jmp    loc_5_830F
loc_5_8288:
        mov    esi, eax
        mov    edx, dword ptr [ebp + 4]
        test   edx, edx
        je     loc_5_82A8
loc_5_8291:
        mov    ecx, dword ptr [edx + 8]
        mov    ebp, dword ptr [edx + 0xc]
        mov    edx, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        add    eax, 4
        mov    dword ptr [eax], ebp
        add    eax, 4
        test   edx, edx
        jne    loc_5_8291
loc_5_82A8:
        lea    eax, [esp + 0x10]
        push   eax
        push   ebx
        push   0x80000002
        mov    ebx, 0x1d
        call   sub_5_D164
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_82F5
        push   edi
        push   esi
        push   3
        push   0
        push   0                             ; -> obj7:0x0BC0
        mov    ebx, 0
        push   dword ptr [esp + 0x24]
        call   sub_5_D0EC
        add    esp, 0x18
        cmp    eax, 1
        adc    ebx, -1
        push   dword ptr [esp + 0x10]
        call   sub_5_D0E4
        and    ebx, 0x1d
        add    esp, 4
loc_5_82F5:
        test   edi, edi
        je     loc_5_830D
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_830D
        push   0
        push   esi
        call   sub_5_D0C8
        add    esp, 8
loc_5_830D:
        mov    eax, ebx
loc_5_830F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_5_8317:
        cmp    edx, 1
        sbb    eax, eax
        and    eax, 0x80000000
        push   eax
        push   0
        push   0x48525950
        push   0x10
        push   ecx
        call   sub_5_D12C
        add    esp, 0x14
        ret    
sub_5_8335:
        push   esi
        cmp    edx, 1
        sbb    eax, eax
        push   edi
        mov    edi, ecx
        and    eax, 0x80000000
        push   eax
        xor    esi, esi
        push   esi
        push   0x45525950
        push   0x10
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        je     loc_5_838A
        cmp    dword ptr [0], esi            ; -> obj7:0x0BD8
        je     loc_5_8383
        mov    eax, dword ptr [0]            ; -> obj7:0x0BD4
        cmp    eax, esi
        je     loc_5_8379
loc_5_836C:
        cmp    eax, edi
        je     loc_5_838A
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    loc_5_836C
loc_5_8379:
        mov    dword ptr [0], 0              ; -> obj7:0x0BD8
loc_5_8383:
        mov    eax, 1
        jmp    loc_5_838C
loc_5_838A:
        xor    eax, eax
loc_5_838C:
        pop    edi
        pop    esi
        ret    
sub_5_838F:
        push   0x201
        push   0x10
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        je     loc_5_83A8
        mov    dword ptr [eax], 0x48525950
loc_5_83A8:
        ret    
sub_5_83A9:
        xor    eax, eax
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        mov    dword ptr [ecx], eax
        jne    loc_5_83C0
        push   eax
        push   ecx
        call   sub_5_D0C8
        add    esp, 8
loc_5_83C0:
        ret    
sub_5_83C1:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj7:0x0BD4
        xor    esi, esi
        test   eax, eax
        je     loc_5_83E2
        lea    edx, [eax + 4]
        mov    dword ptr [eax], 0x45525950
        mov    ecx, dword ptr [edx]
        mov    dword ptr [0], ecx            ; -> obj7:0x0BD4
        mov    dword ptr [edx], esi
        jmp    loc_5_8405
loc_5_83E2:
        mov    dword ptr [0], 0              ; -> obj7:0x0BD8
        push   0x201
        push   0x10
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        je     loc_5_8405
        mov    dword ptr [eax], 0x45525950
loc_5_8405:
        pop    esi
        ret    
sub_5_8407:
        mov    edx, dword ptr [0]            ; -> obj7:0x0BD4
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [0], ecx            ; -> obj7:0x0BD4
        ret    
sub_5_841D:
        mov    dword ptr [0], 1              ; -> obj7:0x0BD8
        mov    eax, dword ptr [0]            ; -> obj7:0x0BD4
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [0], eax            ; -> obj7:0x0BD4
        ret    
sub_5_8438:
        mov    ecx, dword ptr [0]            ; -> obj7:0x0BD4
        test   ecx, ecx
        je     loc_5_8468
loc_5_8442:
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [0], eax            ; -> obj7:0x0BD4
        jne    loc_5_845E
        push   0
        push   ecx
        call   sub_5_D0C8
        add    esp, 8
loc_5_845E:
        mov    ecx, dword ptr [0]            ; -> obj7:0x0BD4
        test   ecx, ecx
        jne    loc_5_8442
loc_5_8468:
        mov    dword ptr [0], 0              ; -> obj7:0x0BD8
        ret    
sub_5_8473:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        xor    edi, edi
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0x14]
        mov    dword ptr [esi], edi
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebx], eax
        test   eax, eax
        je     loc_5_84A8
loc_5_848F:
        mov    edi, dword ptr [eax + 0xc]
        cmp    ecx, edi
        jbe    loc_5_84D6
        sub    edi, ecx
        cmp    edi, -1
        je     loc_5_84BD
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebx], eax
        test   eax, eax
        jne    loc_5_848F
loc_5_84A8:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], 0
loc_5_84B1:
        mov    eax, 1
loc_5_84B6:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_5_84BD:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        mov    dword ptr [ecx], 1
        je     loc_5_84B1
        cmp    dword ptr [eax + 8], edx
        ja     loc_5_84B1
        xor    eax, eax
        jmp    loc_5_84B6
loc_5_84D6:
        mov    eax, dword ptr [eax + 8]
        cmp    edx, eax
        jae    loc_5_84EE
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 0xc]
        dec    eax
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ecx], eax
        jmp    loc_5_84B1
loc_5_84EE:
        mov    ecx, dword ptr [ebp + 0xc]
        xor    eax, eax
        mov    dword ptr [ecx], 1
        jmp    loc_5_84B6
sub_5_84FB:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        call   sub_5_83C1
        test   eax, eax
        jne    loc_5_850E
        xor    eax, eax
        jmp    loc_5_852D
loc_5_850E:
        inc    dword ptr [esi + 0xc]
        test   edi, edi
        jne    loc_5_851D
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [esi + 4], eax
        jmp    loc_5_8523
loc_5_851D:
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [edi + 4], eax
loc_5_8523:
        mov    dword ptr [eax + 4], ecx
        test   ecx, ecx
        jne    loc_5_852D
        mov    dword ptr [esi + 8], eax
loc_5_852D:
        pop    edi
        pop    esi
        ret    
sub_5_8530:
        push   esi
        push   edi
        lea    esi, [ecx + 0xc]
        mov    edi, ecx
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_5_8540
        dec    eax
        mov    dword ptr [esi], eax
loc_5_8540:
        test   edx, edx
        jne    loc_5_854F
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edi + 4], eax
        jmp    loc_5_8558
loc_5_854F:
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
loc_5_8558:
        cmp    dword ptr [edi + 8], ecx
        jne    loc_5_8560
        mov    dword ptr [edi + 8], edx
loc_5_8560:
        mov    esi, dword ptr [ecx + 4]
        call   sub_5_8407
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
sub_5_856D:
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx], edx
        mov    ecx, edx
        jmp    sub_5_85F7
sub_5_857D:
        push   ebx
        push   esi
        mov    esi, dword ptr [ecx]
        push   edi
        mov    edi, edx
        mov    ebx, ecx
        mov    ecx, esi
        mov    edx, dword ptr [ebx + 4]
        call   sub_5_84FB
        test   eax, eax
        jne    loc_5_859F
        mov    ecx, esi
        call   sub_5_85F7
        xor    eax, eax
        jmp    loc_5_85B1
loc_5_859F:
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 8], edi
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [ebx + 4], eax
        mov    eax, 1
loc_5_85B1:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_5_85B7:
        push   esi
        push   edi
        mov    esi, edx
        test   esi, esi
        je     loc_5_85E4
        mov    edi, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
loc_5_85C4:
        mov    edx, eax
        mov    ecx, edi
        call   sub_5_84FB
        test   eax, eax
        je     loc_5_85EC
        mov    ecx, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 0xc]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0xc], edx
        test   esi, esi
        jne    loc_5_85C4
loc_5_85E4:
        mov    eax, 1
loc_5_85E9:
        pop    edi
        pop    esi
        ret    
loc_5_85EC:
        mov    ecx, edi
        call   sub_5_85F7
        xor    eax, eax
        jmp    loc_5_85E9
sub_5_85F7:
        push   esi
        push   edi
        mov    esi, ecx
        xor    edi, edi
        cmp    dword ptr [esi + 0xc], edi
        je     loc_5_8610
        call   sub_5_841D
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 0xc], edi
loc_5_8610:
        pop    edi
        pop    esi
        ret    
sub_5_8613:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [ecx + 4]
        mov    edi, edx
        mov    esi, ecx
        cmp    byte ptr [eax], 0
        jne    loc_5_862E
        xor    eax, eax
        jmp    loc_5_86B6
loc_5_862E:
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        je     loc_5_863B
        xor    eax, eax
        jmp    loc_5_86B6
loc_5_863B:
        test   byte ptr [ebp + 0xc], 1
        je     loc_5_867A
        push   0
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [edi + 0x28]
        push   ecx
        xor    edx, edx
        push   1
        mov    ecx, esi
        mov    dword ptr [ebp - 4], 0xd8
        push   0                             ; -> obj7:0x0BDC
        call   sub_5_87D6
        test   eax, eax
        je     loc_5_866A
        xor    eax, eax
        jmp    loc_5_86B6
loc_5_866A:
        push   0x28
        mov    edx, 0                        ; -> obj7:0x0BE4
        mov    ecx, edi
        call   sub_5_5B5D
        jmp    loc_5_8690
loc_5_867A:
        mov    edx, 0                        ; -> loc_7_0C10
        mov    ecx, edi
        call   sub_5_C486
        mov    edx, dword ptr [esi + 4]
        mov    ecx, edi
        call   sub_5_C44E
loc_5_8690:
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     loc_5_86B1
        cmp    byte ptr [esi], 0
        je     loc_5_86B1
        mov    edx, 0                        ; -> obj7:0x0C18
        mov    ecx, edi
        call   sub_5_C44E
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_C44E
loc_5_86B1:
        mov    eax, 1
loc_5_86B6:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_86BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        xor    ebx, ebx
        test   byte ptr [ebp + 9], 1
        je     loc_5_86D8
        mov    edi, 0x80000001
        jmp    loc_5_86EF
loc_5_86D8:
        test   byte ptr [ebp + 9], 2
        mov    edi, 0x80000005
        jne    loc_5_86EF
        mov    edi, 0x80000002
        cmp    edx, 1
        sbb    ebx, ebx
        neg    ebx
loc_5_86EF:
        test   ebx, ebx
        je     loc_5_870B
        mov    eax, dword ptr [ebp + 8]
        and    eax, 1
        mov    ecx, dword ptr [esi + eax*4 + 0x80]
        mov    dword ptr [ebp - 4], ecx
        test   ecx, ecx
        je     loc_5_870B
        mov    eax, ecx
        jmp    loc_5_875D
loc_5_870B:
        push   dword ptr [ebp + 8]
        push   edx
        mov    edx, 0                        ; -> obj7:0x00CC
        mov    ecx, esi
        call   sub_5_8613
        test   eax, eax
        je     loc_5_875B
        cmp    dword ptr [ebp + 0xc], 0
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj7:0x00CC
        push   edi
        je     loc_5_8736
        call   sub_5_D164
        jmp    loc_5_873B
loc_5_8736:
        call   sub_5_D154
loc_5_873B:
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_875B
        test   ebx, ebx
        je     loc_5_8756
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 1
        mov    dword ptr [esi + ecx*4 + 0x80], eax
loc_5_8756:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_5_875D
loc_5_875B:
        xor    eax, eax
loc_5_875D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_8766:
        cmp    dword ptr [ecx + 0x80], edx
        je     loc_5_877F
        cmp    dword ptr [ecx + 0x84], edx
        je     loc_5_877F
        push   edx
        call   sub_5_D0E4
        add    esp, 4
loc_5_877F:
        ret    
sub_5_8780:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     loc_5_87C1
        lea    edi, [esi + 0x80]
        xor    ebx, ebx
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     loc_5_87A8
        push   eax
        call   sub_5_D0E4
        add    esp, 4
        mov    dword ptr [edi], ebx
loc_5_87A8:
        add    esi, 0x84
        xor    edi, edi
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_5_87C1
        push   eax
        call   sub_5_D0E4
        add    esp, 4
        mov    dword ptr [esi], edi
loc_5_87C1:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_87CA:
        mov    edx, 0                        ; -> sub_5_8780
        xor    ecx, ecx
        jmp    sub_5_CC30
sub_5_87D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   0
        push   dword ptr [ebp + 0x18]
        call   sub_5_86BE
        mov    edi, eax
        test   edi, edi
        jne    loc_5_87F8
        mov    eax, 0x25
        jmp    loc_5_8864
loc_5_87F8:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        lea    edx, [ebp - 4]
        push   edx
        push   0
        push   dword ptr [ebp + 8]
        push   edi
        call   sub_5_D11C
        add    esp, 0x18
        mov    ebx, eax
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_8766
        test   ebx, ebx
        je     loc_5_883B
        cmp    ebx, 2
        je     loc_5_8858
        cmp    ebx, 0xea
        je     loc_5_885F
        cmp    ebx, 0x3f4
        je     loc_5_8858
        mov    eax, 0x1d
        jmp    loc_5_8864
loc_5_883B:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_8854
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_8854
        cmp    eax, dword ptr [ebp - 4]
        je     loc_5_8854
        mov    eax, 0x26
        jmp    loc_5_8864
loc_5_8854:
        xor    eax, eax
        jmp    loc_5_8864
loc_5_8858:
        mov    eax, 0x25
        jmp    loc_5_8864
loc_5_885F:
        mov    eax, 0x1a
loc_5_8864:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
sub_5_886D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   1
        push   dword ptr [ebp + 0x18]
        call   sub_5_86BE
        mov    edi, eax
        mov    eax, 0x1d
        test   edi, edi
        je     loc_5_88BA
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   0
        push   dword ptr [ebp + 8]
        push   edi
        call   sub_5_D0EC
        add    esp, 0x18
        mov    ebx, eax
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_8766
        mov    eax, 0
        cmp    ebx, 1
        adc    eax, -1
        and    eax, 0x1d
loc_5_88BA:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
sub_5_88C1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        mov    dword ptr [ebp - 0x10], edx
        push   edi
        mov    esi, ecx
        push   eax
        lea    ecx, [ebp - 0xc]
        push   0
        push   ecx
        push   0
        push   dword ptr [ebp + 8]
        push   edx
        call   sub_5_D11C
        add    esp, 0x18
        mov    ebx, eax
        cmp    ebx, 0x3f4
        je     loc_5_8ACB
        cmp    ebx, 2
        je     loc_5_8ACB
        test   ebx, ebx
        je     loc_5_8918
        mov    edx, 0x13
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 0x1d
        jmp    loc_5_8ACD
loc_5_8918:
        push   0x201
        push   dword ptr [ebp - 8]
        call   sub_5_D134
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_5_8944
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 2
        jmp    loc_5_8ACD
loc_5_8944:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        push   edi
        push   ecx
        push   0
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 0x10]
        call   sub_5_D11C
        add    esp, 0x18
        test   ebx, ebx
        je     loc_5_898B
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_8975
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_8975:
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 0x1d
        jmp    loc_5_8ACD
loc_5_898B:
        cmp    dword ptr [ebp - 0xc], 3
        je     loc_5_89BB
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_89A5
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_89A5:
        mov    edx, 4
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 0x1d
        jmp    loc_5_8ACD
loc_5_89BB:
        mov    eax, 0x400
        cmp    dword ptr [edi], eax
        jbe    loc_5_89ED
        test   dword ptr [0], eax            ; -> obj7:0x00AC
        jne    loc_5_89D7
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_89D7:
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 0x1d
        jmp    loc_5_8ACD
loc_5_89ED:
        xor    ebx, ebx
        mov    edx, dword ptr [edi + 4]
        push   ebx
        mov    ecx, esi
        push   dword ptr [ebp + 0xc]
        call   sub_5_16B4
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    loc_5_8A21
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_8A17
        push   ebx
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_8A17:
        mov    eax, 2
        jmp    loc_5_8ACD
loc_5_8A21:
        mov    ecx, 8
        mov    eax, dword ptr [edi + 8]
        test   eax, eax
        je     loc_5_8A57
loc_5_8A2D:
        lea    ebx, [ecx + eax]
        cmp    dword ptr [ebp - 8], ebx
        jbe    loc_5_8A57
        sub    eax, 8
        lea    edx, [ecx + edi + 8]
        push   eax
        push   edx
        mov    edx, dword ptr [ecx + edi + 4]
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_1761
        test   eax, eax
        je     loc_5_8A79
        mov    ecx, ebx
        mov    eax, dword ptr [ebx + edi]
        test   eax, eax
        jne    loc_5_8A2D
loc_5_8A57:
        sub    ecx, dword ptr [ebp - 8]
        cmp    ecx, -4
        jne    loc_5_8A9C
        test   eax, eax
        jne    loc_5_8A9C
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_8ACB
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
        jmp    loc_5_8ACB
loc_5_8A79:
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_8A8D
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_8A8D:
        lea    ecx, [ebp - 4]
        call   sub_5_16EE
        mov    eax, 2
        jmp    loc_5_8ACD
loc_5_8A9C:
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_8AB0
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_8AB0:
        lea    ecx, [ebp - 4]
        call   sub_5_16EE
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 0x1f
        jmp    loc_5_8ACD
loc_5_8ACB:
        xor    eax, eax
loc_5_8ACD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_8AD6:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    edi, ecx
        push   0
        push   0
        xor    edx, edx
        call   sub_5_86BE
        mov    esi, eax
        test   esi, esi
        je     loc_5_8B05
        mov    edx, esi
        mov    ecx, edi
        push   dword ptr [esp + 0x10]
        push   ebx
        call   sub_5_88C1
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_8766
loc_5_8B05:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_5_8B0B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   0
        push   dword ptr [ebp + 0xc]
        call   sub_5_86BE
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_5_8B2E
        xor    eax, eax
        jmp    loc_5_8BC8
loc_5_8B2E:
        xor    ebx, ebx
loc_5_8B30:
        push   0
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0xc], 0x100
        push   0
        push   0
        push   0
        push   eax
        push   0                             ; -> obj7:0x00CC
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_5_D234
        add    esp, 0x20
        mov    edi, eax
        test   edi, edi
        jne    loc_5_8B73
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        push   dword ptr [ebp + 8]
        push   0                             ; -> obj7:0x00CC
        call   sub_5_88C1
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_5_8B8F
loc_5_8B73:
        inc    ebx
        test   edi, edi
        je     loc_5_8B30
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        cmp    edi, 0x103
        jne    loc_5_8BAB
        xor    eax, eax
        jmp    loc_5_8BC8
loc_5_8B8F:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [esi + eax*4 + 0x30]
        call   sub_5_15ED
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_5_8BC8
loc_5_8BAB:
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [esi + eax*4 + 0x30]
        call   sub_5_15ED
        mov    edx, 0x13
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 0x1d
loc_5_8BC8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_8BD1:
        sub    esp, 4
        mov    dword ptr [esp], edx
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, ecx
        mov    esi, 0xc
        test   edi, edi
        je     loc_5_8BFD
        mov    eax, dword ptr [edi + 0x10]
        test   eax, eax
        je     loc_5_8BFD
loc_5_8BEE:
        mov    ecx, dword ptr [eax + 0x14]
        mov    eax, dword ptr [eax + 0x10]
        add    ecx, 8
        add    esi, ecx
        test   eax, eax
        jne    loc_5_8BEE
loc_5_8BFD:
        push   0x201
        push   esi
        call   sub_5_D134
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_8C15
        xor    eax, eax
        jmp    loc_5_8C75
loc_5_8C15:
        mov    dword ptr [ebx], 0x400
        test   edi, edi
        je     loc_5_8C25
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 4], eax
loc_5_8C25:
        mov    ebp, 8
        test   edi, edi
        je     loc_5_8C6D
        mov    edi, dword ptr [edi + 0x10]
        test   edi, edi
        je     loc_5_8C6D
loc_5_8C35:
        lea    ecx, [ebx + ebp]
        mov    eax, dword ptr [edi + 0x14]
        lea    edx, [eax + 8]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [edi + 8]
        test   edx, edx
        je     loc_5_8C4C
        mov    edx, dword ptr [edx + 4]
        jmp    loc_5_8C4F
loc_5_8C4C:
        mov    edx, dword ptr [edi + 4]
loc_5_8C4F:
        mov    dword ptr [ecx + 4], edx
        push   eax
        lea    edx, [edi + 0x1c]
        add    ecx, 8
        call   sub_5_5B5D
        mov    eax, dword ptr [edi + 0x14]
        mov    edi, dword ptr [edi + 0x10]
        add    eax, 8
        add    ebp, eax
        test   edi, edi
        jne    loc_5_8C35
loc_5_8C6D:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], esi
        mov    eax, ebx
loc_5_8C75:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_5_8C7D:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        mov    ecx, dword ptr [esp + 0x14]
        lea    edx, [esp + 0xc]
        call   sub_5_8BD1
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_8CC4
        push   0
        mov    edx, 0                        ; -> obj7:0x0C1C
        push   dword ptr [esp + 0x10]
        push   ebx
        push   3
        mov    ecx, esi
        push   edi
        call   sub_5_886D
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_8CC4
        push   0
        push   ebx
        call   sub_5_D0C8
        add    esp, 8
loc_5_8CC4:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
sub_5_8CCD:
        sub    esp, 8
        push   ebx
        push   esi
        mov    eax, dword ptr [ecx + 4]
        push   edi
        push   ebp
        mov    esi, ecx
        xor    ebx, ebx
        cmp    byte ptr [eax], bl
        je     loc_5_8D66
        push   ebx
        mov    edi, 1
        push   edi
        push   ebx
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_8D66
        push   ebx
        xor    edx, edx
        push   ebx
        mov    ecx, esi
        call   sub_5_86BE
        cmp    eax, ebx
        mov    ebp, eax
        je     loc_5_8D66
        push   0                             ; -> loc_7_0C28
        push   ebp
        call   sub_5_D14C
        add    esp, 8
        mov    edx, ebp
        mov    ecx, esi
        call   sub_5_8766
        cmp    dword ptr [esi + 0x34], ebx
        je     loc_5_8D36
        push   ebx
        mov    eax, 0x800
        push   eax
        mov    ecx, esi
        push   ebx
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_8D38
loc_5_8D36:
        xor    edi, edi
loc_5_8D38:
        mov    edi, dword ptr [esi + edi*4 + 0x30]
        test   edi, edi
        je     loc_5_8D66
loc_5_8D40:
        lea    eax, [esp + 0x10]
        push   ebx
        push   0                             ; -> loc_7_0C34
        inc    ebx
        push   eax
        call   sub_5_D0FC
        lea    edx, [esp + 0x1c]
        add    esp, 0xc
        mov    ecx, esi
        push   edi
        call   sub_5_8C7D
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_5_8D40
loc_5_8D66:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_5_8D6E:
        sub    esp, 4
        mov    eax, 0
        cmp    edx, 1
        push   esi
        adc    eax, -1
        lea    esi, [esp + 4]
        mov    dword ptr [esp + 4], 4
        and    eax, 0x200
        test   edx, edx
        push   eax
        push   esi
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, 0                        ; -> obj7:0x0C3C
        push   esi
        push   3
        jne    loc_5_8DA5
        mov    eax, 0                        ; -> loc_7_0C4C
loc_5_8DA5:
        push   eax
        xor    edx, edx
        call   sub_5_87D6
        test   eax, eax
        mov    eax, 1
        je     loc_5_8DBE
        xor    eax, eax
        mov    dword ptr [esi], 0
loc_5_8DBE:
        pop    esi
        add    esp, 4
        ret    4
loc_5_8DC5:
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_8DDC
        push   0
        push   dword ptr [esp + 8]
        call   sub_5_D0C8
        add    esp, 8
loc_5_8DDC:
        ret    
sub_5_8DDD:
        push   esi
        mov    edx, dword ptr [ecx + 0x20]
        mov    esi, dword ptr [edx + 0x24]
        cmp    esi, ecx
        jne    loc_5_8DF0
        mov    eax, dword ptr [ecx + 0x28]
        mov    dword ptr [edx + 0x24], eax
        jmp    loc_5_8E0B
loc_5_8DF0:
        test   esi, esi
        je     loc_5_8E0B
loc_5_8DF4:
        mov    eax, dword ptr [esi + 0x28]
        cmp    eax, ecx
        je     loc_5_8E01
        mov    esi, eax
        test   esi, esi
        jne    loc_5_8DF4
loc_5_8E01:
        test   esi, esi
        je     loc_5_8E0B
        mov    eax, dword ptr [ecx + 0x28]
        mov    dword ptr [esi + 0x28], eax
loc_5_8E0B:
        pop    esi
        ret    
sub_5_8E0D:
        push   ebx
        push   esi
        lea    ebx, [ecx + 0x4c]
        push   edi
        mov    esi, ecx
        mov    edi, 3
        call   sub_5_595F
        mov    ecx, esi
        call   sub_5_26D4
loc_5_8E26:
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     loc_5_8E40
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_8E40
        push   0
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_8E40:
        add    ebx, 4
        dec    edi
        jne    loc_5_8E26
        mov    ecx, esi
        call   sub_5_8DDD
        push   0
        mov    ecx, 0                        ; -> loc_5_8DC5
        push   0
        mov    edx, esi
        call   sub_1_051C
loc_5_8E5D:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_5_8E61:
        push   ebx
        mov    ecx, 0x40000
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        push   0
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_8FD5
        push   0
        mov    ecx, esi
        push   0x40000
        mov    edi, 0
        call   sub_5_1906
        mov    eax, dword ptr [0]            ; -> obj7:0x00AC
        test   al, 8
        je     loc_5_8EAB
        and    eax, 0x400
        cmp    eax, 1
        sbb    edi, edi
        add    edi, 2
loc_5_8EAB:
        xor    ebx, ebx
        mov    edx, 0x8004
        push   ebx
        mov    ecx, esi
        call   sub_5_6048
        push   ebx
        mov    edx, esi
        push   ebx
        xor    ecx, ecx
        push   -1
        push   edi
        push   4
        call   sub_5_CEA8
        test   byte ptr [0], 8               ; -> obj7:0x00AC
        je     loc_5_8EE1
        mov    ecx, esi
        mov    dword ptr [esi], ebx
        call   sub_5_8DDD
        jmp    loc_5_8FD5
loc_5_8EE1:
        mov    ecx, esi
        xor    edi, edi
        call   sub_5_1CB6
        mov    ecx, esi
        call   sub_5_0858
        push   edi
        mov    ecx, esi
        push   8
        call   sub_5_1923
        mov    edx, 0x15
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 0x20000000
        mov    dword ptr [esi], edi
        push   eax
        mov    ecx, esi
        push   edi
        mov    dword ptr [esi + 0x18], edi
        push   eax
        mov    dword ptr [esi + 0x1c], edi
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_8F29
        dec    dword ptr [0]                 ; -> obj7:0x00A4
loc_5_8F29:
        mov    eax, 0x1000000
        mov    ecx, esi
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_8F45
        dec    dword ptr [0]                 ; -> obj7:0x00A4
loc_5_8F45:
        push   0
        mov    ecx, 0x8000
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_8F61
        dec    dword ptr [0]                 ; -> obj7:0x00A4
loc_5_8F61:
        xor    edi, edi
        mov    eax, 0x1000
        push   edi
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_8F8C
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_8F8C
        push   edi
        push   dword ptr [esi + 4]
        call   sub_5_D0C8
        add    esp, 8
loc_5_8F8C:
        lea    ebx, [esi + 0x30]
        mov    edi, 7
        mov    dword ptr [esi + 4], 0
loc_5_8F9B:
        mov    ecx, ebx
        add    ebx, 4
        call   sub_5_15ED
        dec    edi
        jne    loc_5_8F9B
        mov    eax, dword ptr [esi + 0x68]
        test   eax, eax
        je     loc_5_8FC3
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_8FC3
        push   0
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_8FC3:
        mov    ecx, esi
        inc    dword ptr [0]                 ; -> obj7:0x00A0
        call   sub_5_8E0D
        call   sub_5_73CA
loc_5_8FD5:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_5_8FDE:
        mov    eax, dword ptr [0]            ; -> obj7:0x00AC
        mov    edx, 0
        test   al, 8
        je     loc_5_8FF9
        and    eax, 0x400
        cmp    eax, 1
        sbb    edx, edx
        add    edx, 2
loc_5_8FF9:
        push   1
        mov    ecx, dword ptr [0]            ; -> loc_7_0C58
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        call   sub_5_1E4B
        mov    eax, 1
        ret    
sub_5_9014:
        push   ebx
        xor    edx, edx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [0]            ; -> loc_7_0C58
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 1
        je     loc_5_9097
        push   0
        mov    edx, esi
        push   0                             ; -> sub_5_9014
        mov    ebx, 2
        mov    ecx, ebx
        mov    ebp, 0                        ; -> loc_5_8FDE
        call   sub_5_1A48
        push   ebp
        mov    edx, esi
        mov    dword ptr [0], 0xc            ; -> loc_7_0C58
        mov    ecx, ebx
        call   sub_5_CBE2
        push   ebp
        mov    edx, esi
        mov    dword ptr [0], 0xe            ; -> loc_7_0C58
        mov    ecx, 3
        call   sub_5_CBE2
        push   0                             ; -> loc_5_8E61
        mov    edx, esi
        mov    ecx, ebx
        call   sub_5_CBE2
        call   sub_5_7B5F
        mov    eax, 1
        mov    dword ptr [0], edi            ; -> loc_7_0C58
loc_5_9097:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_909C:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [eax + 0x24]
        test   esi, esi
        je     loc_5_9105
        xor    ebx, ebx
        mov    ebp, 0x200
loc_5_90B2:
        mov    ecx, esi
        mov    edi, esi
        mov    esi, dword ptr [esi + 0x28]
        push   ebx
        push   ebp
        push   ebx
        push   ebp
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_9101
        push   ebx
        mov    ecx, edi
        push   0x1000
        push   ebx
        push   0x1000
        call   sub_5_18E3
        test   eax, eax
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        jne    loc_5_9101
        cmp    dword ptr [edi + 0x20], eax
        jne    loc_5_90F8
        push   ebx
        mov    ecx, edi
        push   1
        push   ebx
        push   1
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_9101
loc_5_90F8:
        push   edi
        call   sub_5_9014
        add    esp, 4
loc_5_9101:
        test   esi, esi
        jne    loc_5_90B2
loc_5_9105:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_910A:
        push   esi
        push   0
        mov    esi, ecx
        push   1
        call   sub_5_1906
        push   3
        mov    edx, 0                        ; -> loc_7_0C5C
        mov    ecx, esi
        call   sub_5_8AD6
        push   0
        mov    edx, 0                        ; -> obj7:0x0C68
        push   0
        mov    ecx, esi
        call   sub_5_8B0B
        pop    esi
        ret    
sub_5_9136:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        mov    ebp, esp
        sub    esp, 4
        push   0
        lea    edx, [ebp - 4]
        push   eax
        push   ecx
        push   edx
        call   _CONFIGMG_Create_DevNode
        add    esp, 0x10
        cmp    eax, 0x10
        jne    loc_5_917E
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   8
        push   0
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_917E
        push   1
        xor    edx, edx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 4]
        mov    ecx, 0xb
        call   sub_5_1E4B
loc_5_917E:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_9182:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x12c
        test   byte ptr [1], 0x10            ; -> obj7:0x00AD
        push   ebx
        push   esi
        push   edi
        je     loc_5_919E
        xor    eax, eax
        jmp    loc_5_932A
loc_5_919E:
        mov    edx, 0                        ; -> loc_7_0C74
        lea    ecx, [ebp - 0x12c]
        call   sub_5_C486
        lea    ecx, [ebp - 0x12c]
        call   sub_5_C3C4
        mov    edx, 0                        ; -> obj7:0x0C7C
        lea    ecx, [ebp - 0x12c]
        mov    dword ptr [ebp - 0x1c], eax
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x12c]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_D154
        add    esp, 0xc
        test   eax, eax
        je     loc_5_91F2
        mov    eax, 0x1d
        jmp    loc_5_932A
loc_5_91F2:
        mov    edx, 0                        ; -> obj7:0x0C84
        lea    ecx, [ebp - 0x12c]
        xor    esi, esi
        call   sub_5_C44E
        lea    ecx, [ebp - 0x12c]
        call   sub_5_C3C4
        mov    edi, eax
        mov    dword ptr [ebp - 0xc], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, 0x100
        sub    eax, edi
        mov    dword ptr [ebp - 0x2c], eax
loc_5_9224:
        push   dword ptr [ebp - 0x2c]
        lea    eax, [ebp + edi - 0x12c]
        push   eax
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 0x28]
        call   sub_5_D16C
        add    esp, 0x10
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_5_92FD
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0x12c]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_D154
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_9318
        mov    edx, 0                        ; -> obj7:0x0C88
        lea    ecx, [ebp - 0x12c]
        call   sub_5_C44E
        lea    ecx, [ebp - 0x12c]
        call   sub_5_C3C4
        mov    dword ptr [ebp - 0x18], eax
        test   ebx, ebx
        mov    dword ptr [ebp - 0x14], 0
        jne    loc_5_92F0
        mov    eax, 0x100
        mov    ecx, dword ptr [ebp - 0x10]
        sub    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x20], ecx
        mov    dword ptr [ebp - 0x24], eax
loc_5_92A4:
        mov    ecx, dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x24]
        lea    eax, [ebp + ecx - 0x12c]
        push   eax
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x20]
        call   sub_5_D16C
        add    esp, 0x10
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_92D7
        mov    eax, dword ptr [ebp - 0x1c]
        lea    ecx, [ebp + eax - 0x12c]
        call   sub_5_9136
        jmp    loc_5_92DF
loc_5_92D7:
        cmp    ebx, 0x103
        jne    loc_5_92E8
loc_5_92DF:
        inc    dword ptr [ebp - 0x14]
        test   ebx, ebx
        je     loc_5_92A4
        jmp    loc_5_92F0
loc_5_92E8:
        mov    esi, 0x1d
        mov    dword ptr [ebp - 8], ebx
loc_5_92F0:
        push   dword ptr [ebp - 0x10]
        call   sub_5_D0E4
        add    esp, 4
        jmp    loc_5_9309
loc_5_92FD:
        cmp    eax, 0x103
        je     loc_5_9309
        mov    esi, 0x1d
loc_5_9309:
        inc    dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_9224
        jmp    loc_5_931D
loc_5_9318:
        mov    esi, 0x1d
loc_5_931D:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, esi
loc_5_932A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_9331:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc8
        cmp    eax, 5
        push   esi
        push   edi
        je     loc_5_9354
        cmp    eax, 6
        je     loc_5_937D
        mov    eax, 1
        jmp    loc_5_93DB
loc_5_9354:
        push   0
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        push   eax
        call   _CONFIGMG_Reset_Children_Marks
        add    esp, 8
        call   sub_5_9182
        test   eax, eax
        jne    loc_5_93DB
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        push   eax
        call   sub_5_909C
        add    esp, 4
        jmp    loc_5_93D9
loc_5_937D:
        lea    ecx, [ebp - 0xc8]
        mov    eax, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [eax + 4]
        call   sub_5_C486
        lea    ecx, [ebp - 0xc8]
        mov    edx, 0x5c
        call   sub_5_C3E0
        lea    esi, [ebp + eax - 0xc7]
        mov    edi, eax
        mov    edx, 0x5c
        mov    ecx, esi
        call   sub_5_C3E0
        add    eax, edi
        mov    ecx, esi
        mov    byte ptr [ebp + eax - 0xc7], 0
        call   sub_5_C3C4
        push   0
        xor    edx, edx
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   esi
        push   1
        push   0                             ; -> obj7:0x0C8C
        call   sub_5_886D
loc_5_93D9:
        xor    eax, eax
loc_5_93DB:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_93E1:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0]            ; -> obj7:0x0D74
        mov    edx, dword ptr [ecx + 0x50]
        cmp    dword ptr [edx], eax
        jne    loc_5_93FC
        cmp    dword ptr [ecx + 0xc], 0x17
        jne    loc_5_93FC
        call   sub_5_7024
loc_5_93FC:
        mov    eax, 1
        ret    
loc_5_9402:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0]            ; -> obj7:0x0D78
        cmp    dword ptr [ecx + 0x18], eax
        jne    loc_5_9420
        push   1
        xor    edx, edx
        push   ecx
        push   ecx
        mov    ecx, 7
        call   sub_5_1E4B
loc_5_9420:
        mov    eax, 1
        ret    
loc_5_9426:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0]            ; -> obj7:0x0D7C
        cmp    dword ptr [ecx + 0x1c], eax
        jne    loc_5_9444
        push   0
        xor    edx, edx
        push   ecx
        push   ecx
        mov    ecx, 7
        call   sub_5_1E4B
loc_5_9444:
        mov    eax, 1
        ret    
sub_5_944A:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, ecx
        inc    eax
        push   eax
        call   sub_5_5B5D
        mov    ecx, esi
        call   sub_5_21A9
        pop    esi
        ret    4
sub_5_9463:
        push   esi
        mov    edx, 1
        push   edi
        mov    esi, ecx
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     loc_5_949B
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   0x20
        call   sub_5_1906
        push   edi
        mov    ecx, 0                        ; -> sub_5_79CB
        push   edi
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        push   edi
        call   sub_5_02D3
        xor    eax, eax
loc_5_949B:
        pop    edi
        pop    esi
        ret    
_CONFIGMG_Get_Version:
        clc    
        mov    eax, 0x400
        ret    
_CONFIGMG_Initialize:
        push   ebp
        mov    ebp, esp
        test   dword ptr [esp + 8], 0xfffffffe
        je     loc_5_94B9
        mov    eax, 4
        jmp    loc_5_9513
loc_5_94B9:
        cmp    dword ptr [ebp + 8], 1
        jne    loc_5_94EE
        test   byte ptr [0], 2               ; -> obj7:0x00AC
        je     loc_5_94CC
        xor    eax, eax
        jmp    loc_5_9513
loc_5_94CC:
        call   sub_8_0185
loc_5_94D1:
        test   eax, eax
        jne    loc_5_94DB
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_5_94DB:
        xor    eax, eax
        mov    ecx, 0                        ; -> loc_8_01F6
        push   eax
        xor    edx, edx
        push   eax
        push   eax
        call   sub_5_02D3
        jmp    loc_5_9511
loc_5_94EE:
        test   byte ptr [0], 1               ; -> obj7:0x00AC
        je     loc_5_94FB
        xor    eax, eax
        jmp    loc_5_9513
loc_5_94FB:
        or     byte ptr [0], 1               ; -> obj7:0x00AC
        call   sub_8_004A
loc_5_9507:
        test   eax, eax
        jne    loc_5_9511
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_5_9511:
        xor    eax, eax
loc_5_9513:
        pop    ebp
        ret    
_CONFIGMG_Locate_DevNode:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xcc
        push   esi
        push   edi
        push   1
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_9542
        mov    eax, 3
        jmp    loc_5_95C2
loc_5_9542:
        push   1
        mov    edi, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   1
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_9561
        mov    eax, 3
        jmp    loc_5_95C2
loc_5_9561:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_5_956E
        mov    eax, 4
        jmp    loc_5_95C2
loc_5_956E:
        mov    ecx, edi
        call   sub_5_C3C4
        test   eax, eax
        jne    loc_5_9588
        test   esi, esi
        je     loc_5_9584
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        mov    dword ptr [esi], eax
loc_5_9584:
        xor    eax, eax
        jmp    loc_5_95C2
loc_5_9588:
        cmp    eax, 0xc8
        jbe    loc_5_9596
        mov    eax, 0x1e
        jmp    loc_5_95C2
loc_5_9596:
        push   eax
        lea    ecx, [ebp - 0xcc]
        mov    edx, edi
        call   sub_5_944A
        lea    ecx, [ebp - 0xcc]
        call   sub_5_1BDC
        test   eax, eax
        je     loc_5_95BD
        test   esi, esi
        je     loc_5_95B9
        mov    dword ptr [esi], eax
loc_5_95B9:
        xor    eax, eax
        jmp    loc_5_95C2
loc_5_95BD:
        mov    eax, 0xd
loc_5_95C2:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
_CONFIGMG_Get_Parent:
        push   esi
        xor    eax, eax
        push   edi
        push   eax
        mov    edi, dword ptr [esp + 0x10]
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_95E9
        mov    eax, 3
        jmp    loc_5_9624
loc_5_95E9:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_5_95F7
        mov    eax, 4
        jmp    loc_5_9624
loc_5_95F7:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_9612
        mov    eax, 5
        jmp    loc_5_9624
loc_5_9612:
        mov    eax, dword ptr [esi + 0x20]
        test   eax, eax
        je     loc_5_961F
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    loc_5_9624
loc_5_961F:
        mov    eax, 0xd
loc_5_9624:
        pop    edi
        pop    esi
        ret    
_CONFIGMG_Get_Child:
        push   esi
        xor    eax, eax
        push   edi
        push   eax
        mov    edi, dword ptr [esp + 0x10]
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_9648
        mov    eax, 3
        jmp    loc_5_9683
loc_5_9648:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_5_9656
        mov    eax, 4
        jmp    loc_5_9683
loc_5_9656:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_9671
        mov    eax, 5
        jmp    loc_5_9683
loc_5_9671:
        mov    eax, dword ptr [esi + 0x24]
        test   eax, eax
        je     loc_5_967E
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    loc_5_9683
loc_5_967E:
        mov    eax, 0xd
loc_5_9683:
        pop    edi
        pop    esi
        ret    
_CONFIGMG_Get_Sibling:
        push   esi
        xor    eax, eax
        push   edi
        push   eax
        mov    edi, dword ptr [esp + 0x10]
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_96A7
        mov    eax, 3
        jmp    loc_5_96E2
loc_5_96A7:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_5_96B5
        mov    eax, 4
        jmp    loc_5_96E2
loc_5_96B5:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_96D0
        mov    eax, 5
        jmp    loc_5_96E2
loc_5_96D0:
        mov    eax, dword ptr [esi + 0x28]
        test   eax, eax
        je     loc_5_96DD
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    loc_5_96E2
loc_5_96DD:
        mov    eax, 0xd
loc_5_96E2:
        pop    edi
        pop    esi
        ret    
_CONFIGMG_Get_Device_ID_Size:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [esp + 8]
        push   edi
        push   eax
        push   eax
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_9706
        mov    eax, 3
        jmp    loc_5_9739
loc_5_9706:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_5_9714
        mov    eax, 4
        jmp    loc_5_9739
loc_5_9714:
        mov    edx, 1
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     loc_5_9739
        mov    ecx, dword ptr [edi + 4]
        call   sub_5_C3C4
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_9739:
        pop    edi
        pop    esi
        ret    
_CONFIGMG_Get_Device_ID:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   eax
        mov    edi, dword ptr [ebp + 0x10]
        push   eax
        push   eax
        push   edi
        push   dword ptr [ebp + 0xc]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_9761
        mov    eax, 3
        jmp    loc_5_97AF
loc_5_9761:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_5_976E
        mov    eax, 4
        jmp    loc_5_97AF
loc_5_976E:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_9788
        mov    eax, 5
        jmp    loc_5_97AF
loc_5_9788:
        mov    esi, dword ptr [esi + 4]
        mov    ecx, esi
        call   sub_5_C3C4
        mov    ebx, eax
        lea    eax, [eax + 1]
        cmp    eax, edi
        jb     loc_5_979D
        mov    eax, edi
loc_5_979D:
        push   eax
        mov    edx, esi
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_5B5D
        cmp    edi, ebx
        sbb    eax, eax
        and    eax, 0x1a
loc_5_97AF:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
_CONFIGMG_Get_Depth:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_97D6
        mov    eax, 3
        jmp    loc_5_9805
loc_5_97D6:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_5_97E3
        mov    eax, 4
        jmp    loc_5_9805
loc_5_97E3:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     loc_5_9805
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_1D31
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_9805:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Get_Private_DWord:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_982A
        mov    eax, 3
        jmp    loc_5_985C
loc_5_982A:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_5_9837
        mov    eax, 4
        jmp    loc_5_985C
loc_5_9837:
        mov    edx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_1D72
        test   eax, eax
        mov    eax, 5
        je     loc_5_985C
        mov    edx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_1D46
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_985C:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Set_Private_DWord:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0x14], 0
        je     loc_5_9870
        mov    eax, 4
        jmp    loc_5_9896
loc_5_9870:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1D72
        test   eax, eax
        mov    eax, 5
        je     loc_5_9896
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1D46
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_5_9896:
        pop    ebp
        ret    
_CONFIGMG_Create_DevNode:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xdc
        push   ebx
        push   esi
        push   edi
        push   1
        push   0
        push   0
        push   4
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_98C5
        mov    eax, 3
        jmp    loc_5_9C40
loc_5_98C5:
        push   1
        mov    edi, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   1
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_98E7
        mov    eax, 3
        jmp    loc_5_9C40
loc_5_98E7:
        test   dword ptr [ebp + 0x14], 0xfffffffc
        je     loc_5_98FA
        mov    eax, 4
        jmp    loc_5_9C40
loc_5_98FA:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_9915
        mov    eax, 5
        jmp    loc_5_9C40
loc_5_9915:
        test   byte ptr [1], 0x10            ; -> obj7:0x00AD
        je     loc_5_9933
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        cmp    dword ptr [eax + 0x24], 0
        je     loc_5_9933
        mov    eax, 0x15
        jmp    loc_5_9C40
loc_5_9933:
        mov    ecx, edi
        call   sub_5_C3C4
        cmp    eax, 0xc8
        mov    esi, eax
        jbe    loc_5_994D
        mov    eax, 0x1e
        jmp    loc_5_9C40
loc_5_994D:
        test   esi, esi
        je     loc_5_9A90
        push   esi
        lea    ecx, [ebp - 0xdc]
        mov    edx, edi
        lea    ebx, [esi - 1]
        call   sub_5_944A
        lea    ecx, [ebp - 0xdc]
        mov    edx, 0x5c
        call   sub_5_C3E0
        cmp    eax, ebx
        mov    edi, eax
        sbb    ecx, ecx
        lea    eax, [edi + 1]
        inc    ecx
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        or     ecx, eax
        cmp    edi, 1
        sbb    eax, eax
        neg    eax
        or     ecx, eax
        je     loc_5_999E
        mov    eax, 0x1e
        jmp    loc_5_9C40
loc_5_999E:
        lea    ecx, [ebp + edi - 0xdb]
        mov    edx, 0x5c
        call   sub_5_C3E0
        cmp    eax, -1
        jne    loc_5_99E8
        mov    edx, 0                        ; -> obj7:0x0DC0
        lea    ecx, [ebp - 0xdc]
        inc    esi
        call   sub_5_C44E
        or     byte ptr [ebp + 0x14], 2
loc_5_99C9:
        test   byte ptr [ebp + 0x14], 2
        je     loc_5_9A97
        mov    eax, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [eax + 4]
        cmp    byte ptr [edi], 0
        jne    loc_5_9A2B
        mov    eax, 0x1e
        jmp    loc_5_9C40
loc_5_99E8:
        lea    eax, [edi + eax + 1]
        mov    ecx, eax
        sub    ecx, edi
        dec    ecx
        cmp    ecx, 1
        sbb    edx, edx
        neg    edx
        cmp    eax, ebx
        sbb    ecx, ecx
        inc    ecx
        or     edx, ecx
        je     loc_5_9A0B
        mov    eax, 0x1e
        jmp    loc_5_9C40
loc_5_9A0B:
        lea    ecx, [ebp + eax - 0xdb]
        mov    edx, 0x5c
        call   sub_5_C3E0
        cmp    eax, -1
        je     loc_5_99C9
        mov    eax, 0x1e
        jmp    loc_5_9C40
loc_5_9A2B:
        mov    ecx, edi
        call   sub_5_C3C4
        add    eax, esi
        inc    eax
        cmp    eax, 0xc8
        jbe    loc_5_9A46
        mov    eax, 0x1e
        jmp    loc_5_9C40
loc_5_9A46:
        lea    ecx, [ebp - 0xdc]
        mov    edx, edi
        call   sub_5_C44E
        lea    ecx, [ebp + esi - 0xdc]
        mov    edx, 0x5c
        call   sub_5_C3E0
        lea    edi, [esi + eax]
        mov    ebx, eax
        mov    edx, 0x5c
        lea    ecx, [ebp + edi - 0xdb]
        call   sub_5_C3E0
        add    eax, ebx
        add    eax, esi
        mov    byte ptr [ebp + edi - 0xdc], 0x26
        mov    byte ptr [ebp + eax - 0xdb], 0x26
        jmp    loc_5_9A97
loc_5_9A90:
        mov    byte ptr [ebp - 0xdc], 0
loc_5_9A97:
        lea    ecx, [ebp - 0xdc]
        call   sub_5_1BDC
        mov    esi, eax
        test   esi, esi
        je     loc_5_9ACB
        push   0
        mov    ecx, esi
        push   0x220
        call   sub_5_1906
        cmp    dword ptr [ebp + 8], 0
        je     loc_5_9AC1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], esi
loc_5_9AC1:
        mov    eax, 0x10
        jmp    loc_5_9C40
loc_5_9ACB:
        lea    ecx, [ebp - 0xdc]
        mov    edx, dword ptr [ebp + 0x10]
        call   sub_5_1C73
        mov    esi, eax
        test   esi, esi
        je     loc_5_9C3B
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     loc_5_9BEA
        push   1
        xor    edx, edx
        push   0
        mov    ecx, esi
        call   sub_5_86BE
        lea    eax, [ebp - 4]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [ebp + 0x14], 1
        je     loc_5_9B2D
        test   eax, eax
        je     loc_5_9B19
        test   byte ptr [ebp - 4], 0x20
        je     loc_5_9B2D
loc_5_9B19:
        push   0x1000000
        mov    ecx, esi
        push   0
        call   sub_5_1906
        inc    dword ptr [0]                 ; -> obj7:0x00A4
loc_5_9B2D:
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        cmp    dword ptr [eax + 0x24], esi
        je     loc_5_9BEA
        test   byte ptr [1], 8               ; -> obj7:0x00AD
        jne    loc_5_9B48
        or     byte ptr [ebp - 4], 0x10
loc_5_9B48:
        mov    edi, 4
        cmp    dword ptr [ebp + 0x10], eax
        mov    dword ptr [ebp - 0xc], edi
        jne    loc_5_9BBA
        push   5
        mov    edx, 0                        ; -> obj7:0x0DC4
        lea    ecx, [ebp - 0xdc]
        call   sub_5_C3FC
        test   eax, eax
        je     loc_5_9BBA
        xor    ebx, ebx
        lea    eax, [ebp - 0xc]
        push   ebx
        lea    ecx, [ebp - 8]
        push   eax
        xor    edx, edx
        push   ecx
        push   3
        push   0                             ; -> obj7:0x0DCC
        mov    ecx, esi
        call   sub_5_87D6
        test   eax, eax
        jne    loc_5_9BBA
        test   byte ptr [ebp - 5], 0x80
        je     loc_5_9BBA
        push   0x2000000
        mov    ecx, esi
        push   ebx
        call   sub_5_1906
        push   ebx
        lea    eax, [ebp - 8]
        push   edi
        xor    edx, edx
        push   eax
        mov    ecx, esi
        push   3
        mov    dword ptr [ebp - 0xc], edi
        and    byte ptr [ebp - 5], 0x7f
        push   0                             ; -> obj7:0x0DD8
        call   sub_5_886D
loc_5_9BBA:
        lea    eax, [ebp - 0x10]
        mov    edx, 1
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [ebp - 0x10], 2
        jne    loc_5_9BDC
        test   byte ptr [ebp - 4], 2
        jne    loc_5_9BDC
        test   byte ptr [ebp - 4], 0x10
        jne    loc_5_9BEA
loc_5_9BDC:
        mov    ecx, esi
        call   sub_5_8E0D
        mov    eax, 0x15
        jmp    loc_5_9C40
loc_5_9BEA:
        mov    ecx, esi
        call   sub_5_2654
        push   0
        mov    ecx, esi
        push   0x220
        call   sub_5_1906
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, dword ptr [0]            ; -> obj7:0x00A8
        jne    loc_5_9C27
        push   5
        mov    edx, 0                        ; -> obj7:0x0DE4
        lea    ecx, [ebp - 0xdc]
        call   sub_5_C3FC
        test   eax, eax
        je     loc_5_9C27
        mov    ecx, esi
        call   sub_5_910A
loc_5_9C27:
        call   sub_5_7B5F
        cmp    dword ptr [ebp + 8], 0
        je     loc_5_9C37
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], esi
loc_5_9C37:
        xor    eax, eax
        jmp    loc_5_9C40
loc_5_9C3B:
        mov    eax, 2
loc_5_9C40:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
_CONFIGMG_Query_Remove_SubTree:
        test   dword ptr [esp + 8], 0xfffffffe
        push   esi
        push   edi
        je     loc_5_9C5A
        mov    eax, 4
        jmp    loc_5_9CB0
loc_5_9C5A:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_9CAB
        cmp    esi, dword ptr [0]            ; -> obj7:0x00A8
        je     loc_5_9CAB
        mov    edi, dword ptr [esp + 0x10]
        and    edi, 1
        je     loc_5_9C85
        inc    dword ptr [0]                 ; -> obj7:0x008C
loc_5_9C85:
        push   0x13
        mov    ecx, 0                        ; -> sub_5_11DE
        push   0
        mov    edx, esi
        push   0
        call   sub_5_02D3
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x17
        test   edi, edi
        je     loc_5_9CB0
        dec    dword ptr [0]                 ; -> obj7:0x008C
        jmp    loc_5_9CB0
loc_5_9CAB:
        mov    eax, 5
loc_5_9CB0:
        pop    edi
        pop    esi
        ret    
_CONFIGMG_Remove_SubTree:
        test   dword ptr [esp + 8], 0xfffffffe
        push   esi
        push   edi
        je     loc_5_9CC6
        mov    eax, 4
        jmp    loc_5_9D16
loc_5_9CC6:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_9D11
        cmp    dword ptr [0], esi            ; -> obj7:0x00A8
        je     loc_5_9D11
        mov    edi, dword ptr [esp + 0x10]
        and    edi, 1
        je     loc_5_9CF1
        inc    dword ptr [0]                 ; -> obj7:0x008C
loc_5_9CF1:
        push   0
        mov    ecx, 0                        ; -> sub_5_9014
        push   0
        mov    edx, esi
        push   0
        call   sub_5_02D3
        test   edi, edi
        je     loc_5_9D0D
        dec    dword ptr [0]                 ; -> obj7:0x008C
loc_5_9D0D:
        xor    eax, eax
        jmp    loc_5_9D16
loc_5_9D11:
        mov    eax, 5
loc_5_9D16:
        pop    edi
        pop    esi
        ret    
_CONFIGMG_Register_Device_Driver:
        push   ebp
        mov    ebp, esp
        test   dword ptr [esp + 0x14], 0xfffffff8
        je     loc_5_9D2D
        mov    eax, 4
        jmp    loc_5_9D59
loc_5_9D2D:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     loc_5_9D59
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_1F16
        add    esp, 0x10
        xor    eax, eax
loc_5_9D59:
        pop    ebp
        ret    
_CONFIGMG_Register_Enumerator:
        push   ebp
        mov    ebp, esp
        push   esi
        test   dword ptr [ebp + 0x10], 0xfffffffe
        je     loc_5_9D6F
        mov    eax, 4
        jmp    loc_5_9DBF
loc_5_9D6F:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_9D89
        mov    eax, 5
        jmp    loc_5_9DBF
loc_5_9D89:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, esi
        and    eax, 1
        shl    eax, 7
        cdq    
        push   edx
        push   eax
        call   sub_5_1906
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        mov    dword ptr [esi + 0x1c], eax
        jne    loc_5_9DAD
        mov    dword ptr [esi + 0x1c], 0     ; -> loc_5_1DB8
loc_5_9DAD:
        push   0
        mov    ecx, esi
        push   0x24
        call   sub_5_1906
        call   sub_5_7B5F
        xor    eax, eax
loc_5_9DBF:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Register_Arbitrator:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x1c]
        test   ebx, 0xffffffe0
        je     loc_5_9DDD
        mov    eax, 4
        jmp    loc_5_9E7D
loc_5_9DDD:
        mov    edi, dword ptr [ebp + 0x18]
        test   edi, edi
        je     loc_5_9E02
        mov    edx, 1
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_9E78
        cmp    edi, dword ptr [0]            ; -> obj7:0x00A8
        jne    loc_5_9E0C
        jmp    loc_5_9E78
loc_5_9E02:
        test   bl, 1
        je     loc_5_9E78
        test   bl, 8
        jne    loc_5_9E78
loc_5_9E0C:
        mov    esi, dword ptr [ebp + 0xc]
        test   esi, esi
        jne    loc_5_9E1A
        mov    eax, 0xb
        jmp    loc_5_9E7D
loc_5_9E1A:
        test   esi, 0x8000
        je     loc_5_9E29
        mov    eax, 0xb
        jmp    loc_5_9E7D
loc_5_9E29:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 0x10]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_9E46
        mov    eax, 3
        jmp    loc_5_9E7D
loc_5_9E46:
        push   1
        push   0
        push   0
        push   4
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_9E64
        mov    eax, 3
        jmp    loc_5_9E7D
loc_5_9E64:
        push   ebx
        mov    edx, esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_5_0656
        jmp    loc_5_9E7D
loc_5_9E78:
        mov    eax, 5
loc_5_9E7D:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
_CONFIGMG_Deregister_Arbitrator:
        cmp    dword ptr [esp + 8], 0
        mov    eax, 4
        jne    loc_5_9E9F
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_083E
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 8
loc_5_9E9F:
        ret    
_CONFIGMG_Query_Arbitrator_Free_Size:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x38
        cmp    dword ptr [ebp + 0x14], 0
        push   ebx
        push   esi
        push   edi
        je     loc_5_9EB9
        mov    eax, 4
        jmp    loc_5_9F41
loc_5_9EB9:
        mov    esi, dword ptr [ebp + 0xc]
        test   esi, esi
        jne    loc_5_9ED4
        mov    esi, dword ptr [0]            ; -> obj7:0x00A8
loc_5_9EC6:
        mov    ebx, dword ptr [ebp + 0x10]
        test   ebx, ebx
        jne    loc_5_9EEB
        mov    eax, 0xb
        jmp    loc_5_9F41
loc_5_9ED4:
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_9EC6
        mov    eax, 5
        jmp    loc_5_9F41
loc_5_9EEB:
        xor    eax, eax
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_9F09
        mov    eax, 3
        jmp    loc_5_9F41
loc_5_9F09:
        push   esi
        lea    ecx, [ebp - 0x38]
        mov    edx, ebx
        call   sub_5_08C9
        mov    ecx, eax
        call   sub_5_0516
        mov    ecx, eax
        test   ecx, ecx
        jne    loc_5_9F28
        mov    eax, 0x1b
        jmp    loc_5_9F41
loc_5_9F28:
        lea    eax, [ebp - 8]
        mov    edx, 4
        push   eax
        call   sub_5_0B16
        test   eax, eax
        jne    loc_5_9F41
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi], eax
        xor    eax, eax
loc_5_9F41:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
_CONFIGMG_Query_Arbitrator_Free_Data:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x38
        cmp    dword ptr [ebp + 0x18], 0
        push   esi
        push   edi
        je     loc_5_9F60
        mov    eax, 4
        jmp    loc_5_A004
loc_5_9F60:
        mov    esi, dword ptr [ebp + 0x10]
        test   esi, esi
        jne    loc_5_9F7E
        mov    esi, dword ptr [0]            ; -> obj7:0x00A8
loc_5_9F6D:
        mov    edi, dword ptr [ebp + 0x14]
        test   edi, edi
        jne    loc_5_9F95
        mov    eax, 0xb
        jmp    loc_5_A004
loc_5_9F7E:
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_9F6D
        mov    eax, 5
        jmp    loc_5_A004
loc_5_9F95:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_9FB3
        mov    eax, 3
        jmp    loc_5_A004
loc_5_9FB3:
        push   esi
        lea    ecx, [ebp - 0x38]
        mov    edx, edi
        call   sub_5_08C9
        mov    ecx, eax
        call   sub_5_0516
        test   eax, eax
        jne    loc_5_9FD0
        mov    eax, 0x1b
        jmp    loc_5_A004
loc_5_9FD0:
        lea    ecx, [ebp - 8]
        mov    edx, 4
        push   ecx
        mov    ecx, eax
        call   sub_5_0B16
        test   eax, eax
        jne    loc_5_A004
        mov    esi, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    esi, eax
        jb     loc_5_9FF0
        mov    esi, eax
loc_5_9FF0:
        push   esi
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_5B5D
        cmp    esi, dword ptr [ebp - 4]
        sbb    eax, eax
        and    eax, 0x1a
loc_5_A004:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
_CONFIGMG_Sort_NodeList:
        cmp    dword ptr [esp + 8], 0
        je     loc_5_A018
        mov    eax, 4
        jmp    loc_5_A03A
loc_5_A018:
        mov    edx, 1
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_6723
        test   eax, eax
        mov    eax, 9
        je     loc_5_A03A
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_692A
        xor    eax, eax
loc_5_A03A:
        ret    
_CONFIGMG_Add_Empty_Log_Conf:
        push   ebx
        xor    eax, eax
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        push   ebp
        push   eax
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A05E
        mov    eax, 3
        jmp    loc_5_A0D8
loc_5_A05E:
        mov    ebp, dword ptr [esp + 0x20]
        test   ebp, 0xfffffff0
        jne    loc_5_A0D3
        mov    esi, ebp
        and    esi, 7
        cmp    esi, 7
        jae    loc_5_A0D3
        mov    edx, 1
        mov    ebx, dword ptr [esp + 0x18]
        mov    ecx, ebx
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_A0CC
        cmp    ebx, dword ptr [0]            ; -> obj7:0x00A8
        je     loc_5_A0CC
        cmp    dword ptr [esp + 0x1c], 0xffff
        jbe    loc_5_A0A1
        mov    eax, 0x27
        jmp    loc_5_A0D8
loc_5_A0A1:
        and    ebp, 8
        mov    edx, dword ptr [esp + 0x1c]
        sub    ebp, 8
        mov    ecx, ebx
        cmp    ebp, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   esi
        call   sub_5_16B4
        test   eax, eax
        je     loc_5_A0C5
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    loc_5_A0D8
loc_5_A0C5:
        mov    eax, 2
        jmp    loc_5_A0D8
loc_5_A0CC:
        mov    eax, 5
        jmp    loc_5_A0D8
loc_5_A0D3:
        mov    eax, 4
loc_5_A0D8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
_CONFIGMG_Free_Log_Conf:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0xc], 0
        je     loc_5_A0EE
        mov    eax, 4
        jmp    loc_5_A138
loc_5_A0EE:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1450
        test   eax, eax
        jne    loc_5_A106
        mov    eax, 7
        jmp    loc_5_A138
loc_5_A106:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, 1
        mov    ecx, dword ptr [eax + 4]
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_A133
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        cmp    dword ptr [ecx + 4], eax
        je     loc_5_A133
        lea    ecx, [ebp + 8]
        call   sub_5_16EE
        xor    eax, eax
        jmp    loc_5_A138
loc_5_A133:
        mov    eax, 5
loc_5_A138:
        pop    ebp
        ret    
_CONFIGMG_Get_First_Log_Conf:
        push   ebx
        push   esi
        push   edi
        push   1
        mov    edi, dword ptr [esp + 0x14]
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A15D
        mov    eax, 3
        jmp    loc_5_A1AA
loc_5_A15D:
        mov    ebx, dword ptr [esp + 0x18]
        test   ebx, 0xfffffff8
        jne    loc_5_A1A5
        mov    eax, ebx
        and    al, 7
        cmp    al, 7
        jae    loc_5_A1A5
        mov    edx, 1
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_A18C
        mov    eax, 5
        jmp    loc_5_A1AA
loc_5_A18C:
        mov    eax, dword ptr [esi + ebx*4 + 0x30]
        test   eax, eax
        je     loc_5_A19E
        test   edi, edi
        je     loc_5_A19A
        mov    dword ptr [edi], eax
loc_5_A19A:
        xor    eax, eax
        jmp    loc_5_A1AA
loc_5_A19E:
        mov    eax, 0xe
        jmp    loc_5_A1AA
loc_5_A1A5:
        mov    eax, 4
loc_5_A1AA:
        pop    edi
        pop    esi
        pop    ebx
        ret    
_CONFIGMG_Get_Next_Log_Conf:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        push   1
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A1D0
        mov    eax, 3
        jmp    loc_5_A20E
loc_5_A1D0:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_5_A1DE
        mov    eax, 4
        jmp    loc_5_A20E
loc_5_A1DE:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, esi
        call   sub_5_1450
        test   eax, eax
        jne    loc_5_A1F9
        mov    eax, 7
        jmp    loc_5_A20E
loc_5_A1F9:
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_5_A209
        test   edi, edi
        je     loc_5_A205
        mov    dword ptr [edi], eax
loc_5_A205:
        xor    eax, eax
        jmp    loc_5_A20E
loc_5_A209:
        mov    eax, 0xe
loc_5_A20E:
        pop    edi
        pop    esi
        ret    
_CONFIGMG_Add_Res_Des:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   1
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A239
        mov    eax, 3
        jmp    loc_5_A2D1
loc_5_A239:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A257
        mov    eax, 3
        jmp    loc_5_A2D1
loc_5_A257:
        cmp    dword ptr [ebp + 0x1c], 0
        je     loc_5_A264
        mov    eax, 4
        jmp    loc_5_A2D1
loc_5_A264:
        mov    edx, 1
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, edi
        call   sub_5_1450
        test   eax, eax
        jne    loc_5_A27E
        mov    eax, 7
        jmp    loc_5_A2D1
loc_5_A27E:
        mov    ebx, dword ptr [edi + 4]
        mov    edx, 1
        mov    ecx, ebx
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_A2CC
        cmp    dword ptr [0], ebx            ; -> obj7:0x00A8
        je     loc_5_A2CC
        mov    edx, dword ptr [ebp + 0x10]
        test   edx, edx
        jne    loc_5_A2A7
        mov    eax, 0xb
        jmp    loc_5_A2D1
loc_5_A2A7:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        mov    ecx, edi
        call   sub_5_1761
        test   eax, eax
        je     loc_5_A2C5
        test   esi, esi
        je     loc_5_A2C1
        add    eax, 0x1c
        mov    dword ptr [esi], eax
loc_5_A2C1:
        xor    eax, eax
        jmp    loc_5_A2D1
loc_5_A2C5:
        mov    eax, 2
        jmp    loc_5_A2D1
loc_5_A2CC:
        mov    eax, 5
loc_5_A2D1:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
_CONFIGMG_Modify_Res_Des:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A2FB
        mov    eax, 3
        jmp    loc_5_A3A5
loc_5_A2FB:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A31C
        mov    eax, 3
        jmp    loc_5_A3A5
loc_5_A31C:
        cmp    dword ptr [ebp + 0x1c], 0
        je     loc_5_A329
        mov    eax, 4
        jmp    loc_5_A3A5
loc_5_A329:
        lea    ecx, [ebp + 0xc]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        jne    loc_5_A341
        mov    eax, 6
        jmp    loc_5_A3A5
loc_5_A341:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, 1
        mov    eax, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [eax + 4]
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_A3A0
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        mov    ecx, dword ptr [eax + 0xc]
        cmp    dword ptr [ecx + 4], edx
        je     loc_5_A3A0
        mov    edx, dword ptr [ebp + 0x10]
        mov    ecx, eax
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        call   sub_5_178A
        test   eax, eax
        je     loc_5_A386
        add    eax, 0x1c
        mov    dword ptr [esi], eax
        xor    eax, eax
        jmp    loc_5_A3A5
loc_5_A386:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, 3
        mov    esi, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [esi + 4]
        call   sub_5_731F
        mov    eax, 2
        jmp    loc_5_A3A5
loc_5_A3A0:
        mov    eax, 5
loc_5_A3A5:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Free_Res_Des:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A3CA
        mov    eax, 3
        jmp    loc_5_A434
loc_5_A3CA:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_5_A3D7
        mov    eax, 4
        jmp    loc_5_A434
loc_5_A3D7:
        lea    ecx, [ebp + 0xc]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        jne    loc_5_A3EF
        mov    eax, 6
        jmp    loc_5_A434
loc_5_A3EF:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, 1
        mov    eax, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [eax + 4]
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_A42F
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        mov    ecx, dword ptr [eax + 0xc]
        cmp    dword ptr [ecx + 4], edx
        je     loc_5_A42F
        mov    ecx, eax
        call   sub_5_17FB
        test   eax, eax
        je     loc_5_A428
        mov    dword ptr [esi], eax
        xor    eax, eax
        jmp    loc_5_A434
loc_5_A428:
        mov    eax, 0xf
        jmp    loc_5_A434
loc_5_A42F:
        mov    eax, 5
loc_5_A434:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Get_Next_Res_Des:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   1
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A45E
        mov    eax, 3
        jmp    loc_5_A542
loc_5_A45E:
        push   1
        mov    edi, dword ptr [ebp + 0x14]
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A480
        mov    eax, 3
        jmp    loc_5_A542
loc_5_A480:
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_5_A490
        mov    eax, 4
        jmp    loc_5_A542
loc_5_A490:
        mov    eax, dword ptr [ebp + 0xc]
        xor    ecx, ecx
        sub    eax, 4
        push   ecx
        push   ecx
        push   ecx
        push   4
        push   eax
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A4B4
        mov    eax, 3
        jmp    loc_5_A542
loc_5_A4B4:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax - 4]
        cmp    eax, 0x4c52574d
        je     loc_5_A4CF
        cmp    eax, 0x574d5950
        je     loc_5_A4F4
        mov    eax, 6
        jmp    loc_5_A542
loc_5_A4CF:
        lea    ecx, [ebp + 0xc]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        jne    loc_5_A4E7
        mov    eax, 6
        jmp    loc_5_A542
loc_5_A4E7:
        mov    edx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_1877
        jmp    loc_5_A517
loc_5_A4F4:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_1450
        test   eax, eax
        jne    loc_5_A50C
        mov    eax, 7
        jmp    loc_5_A542
loc_5_A50C:
        mov    edx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_184D
loc_5_A517:
        test   eax, eax
        je     loc_5_A53D
        test   edi, edi
        je     loc_5_A530
        mov    ecx, dword ptr [eax + 8]
        test   ecx, ecx
        je     loc_5_A52B
        mov    ecx, dword ptr [ecx + 4]
        jmp    loc_5_A52E
loc_5_A52B:
        mov    ecx, dword ptr [eax + 4]
loc_5_A52E:
        mov    dword ptr [edi], ecx
loc_5_A530:
        test   esi, esi
        je     loc_5_A539
        add    eax, 0x1c
        mov    dword ptr [esi], eax
loc_5_A539:
        xor    eax, eax
        jmp    loc_5_A542
loc_5_A53D:
        mov    eax, 0xf
loc_5_A542:
        pop    edi
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Get_Performance_Info:
        mov    eax, 0x13
        ret    
_CONFIGMG_Get_Res_Des_Data_Size:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A56E
        mov    eax, 3
        jmp    loc_5_A59B
loc_5_A56E:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_5_A57B
        mov    eax, 4
        jmp    loc_5_A59B
loc_5_A57B:
        lea    ecx, [ebp + 0xc]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        mov    eax, 6
        je     loc_5_A59B
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x14]
        xor    eax, eax
        mov    dword ptr [esi], ecx
loc_5_A59B:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Get_Res_Des_Data:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        push   dword ptr [ebp + 0xc]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A5C2
        mov    eax, 3
        jmp    loc_5_A60B
loc_5_A5C2:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_5_A5CF
        mov    eax, 4
        jmp    loc_5_A60B
loc_5_A5CF:
        lea    ecx, [ebp + 8]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        jne    loc_5_A5E7
        mov    eax, 6
        jmp    loc_5_A60B
loc_5_A5E7:
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 0x14]
        cmp    edi, esi
        mov    eax, edi
        jb     loc_5_A5F5
        mov    eax, esi
loc_5_A5F5:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        add    edx, 0x1c
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_5B5D
        cmp    esi, edi
        sbb    eax, eax
        and    eax, 0x1a
loc_5_A60B:
        pop    edi
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Process_Events_Now:
        cmp    dword ptr [esp + 4], 0
        mov    eax, 4
        jne    loc_5_A632
        xor    eax, eax
        mov    ecx, 0                        ; -> sub_5_79CB
        push   eax
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        push   eax
        push   eax
        call   sub_5_02D3
        xor    eax, eax
loc_5_A632:
        ret    
_CONFIGMG_Create_Range_List:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     loc_5_A642
        mov    eax, 4
        jmp    loc_5_A675
loc_5_A642:
        xor    eax, eax
        mov    esi, dword ptr [esp + 8]
        push   eax
        push   eax
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A661
        mov    eax, 3
        jmp    loc_5_A675
loc_5_A661:
        call   sub_5_838F
        test   eax, eax
        jne    loc_5_A671
        mov    eax, 2
        jmp    loc_5_A675
loc_5_A671:
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_A675:
        pop    esi
        ret    
_CONFIGMG_Add_Range:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0x10]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A693
        mov    eax, 0x11
        jmp    loc_5_A6C7
loc_5_A693:
        test   dword ptr [ebp + 0x14], 0xfffffffe
        je     loc_5_A6A3
        mov    eax, 4
        jmp    loc_5_A6C7
loc_5_A6A3:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], eax
        mov    eax, 0x12
        jb     loc_5_A6C7
        mov    eax, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp + 0xc]
        not    eax
        and    eax, 1
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_5_7BC2
loc_5_A6C7:
        pop    ebp
        ret    
_CONFIGMG_Delete_Range:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0x10]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A6E5
        mov    eax, 0x11
        jmp    loc_5_A715
loc_5_A6E5:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_5_A6F2
        mov    eax, 4
        jmp    loc_5_A715
loc_5_A6F2:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, dword ptr [ebp + 8]
        mov    eax, 0x12
        jb     loc_5_A715
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        call   sub_5_7C9B
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_A715:
        pop    ebp
        ret    
_CONFIGMG_Test_Range_Available:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        sub    esp, 0xc
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A735
        mov    eax, 0x11
        jmp    loc_5_A771
loc_5_A735:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_5_A742
        mov    eax, 4
        jmp    loc_5_A771
loc_5_A742:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], eax
        mov    eax, 0x12
        jb     loc_5_A771
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        call   sub_5_8473
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x13
loc_5_A771:
        mov    esp, ebp
        pop    ebp
        ret    
_CONFIGMG_Dup_Range_List:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A791
        mov    eax, 0x11
        jmp    loc_5_A7C7
loc_5_A791:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A7A9
        mov    eax, 0x11
        jmp    loc_5_A7C7
loc_5_A7A9:
        cmp    dword ptr [ebp + 0x10], 0
        mov    eax, 4
        jne    loc_5_A7C7
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_7D9A
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_A7C7:
        pop    ebp
        ret    
_CONFIGMG_Free_Range_List:
        push   esi
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A7E5
        mov    eax, 0x11
        jmp    loc_5_A801
loc_5_A7E5:
        cmp    dword ptr [esp + 0xc], 0
        mov    eax, 4
        jne    loc_5_A801
        mov    ecx, esi
        call   sub_5_85F7
        mov    ecx, esi
        call   sub_5_83A9
        xor    eax, eax
loc_5_A801:
        pop    esi
        ret    
_CONFIGMG_Invert_Range_List:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A81F
        mov    eax, 0x11
        jmp    loc_5_A858
loc_5_A81F:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A837
        mov    eax, 0x11
        jmp    loc_5_A858
loc_5_A837:
        cmp    dword ptr [ebp + 0x14], 0
        mov    eax, 4
        jne    loc_5_A858
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        call   sub_5_7DBA
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_A858:
        pop    ebp
        ret    
_CONFIGMG_Intersect_Range_List:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A876
        mov    eax, 0x11
        jmp    loc_5_A8C7
loc_5_A876:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A88E
        mov    eax, 0x11
        jmp    loc_5_A8C7
loc_5_A88E:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A8A6
        mov    eax, 0x11
        jmp    loc_5_A8C7
loc_5_A8A6:
        cmp    dword ptr [ebp + 0x14], 0
        mov    eax, 4
        jne    loc_5_A8C7
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        call   sub_5_7E40
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_A8C7:
        pop    ebp
        ret    
_CONFIGMG_First_Range:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_A8EA
        mov    eax, 0x11
        jmp    loc_5_A97F
loc_5_A8EA:
        push   1
        mov    esi, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A909
        mov    eax, 3
        jmp    loc_5_A97F
loc_5_A909:
        push   1
        mov    edi, dword ptr [ebp + 0x10]
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A928
        mov    eax, 3
        jmp    loc_5_A97F
loc_5_A928:
        push   1
        mov    ebx, dword ptr [ebp + 0x14]
        push   0
        push   0
        push   4
        push   ebx
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A947
        mov    eax, 3
        jmp    loc_5_A97F
loc_5_A947:
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_5_A954
        mov    eax, 4
        jmp    loc_5_A97F
loc_5_A954:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 4]
        test   ecx, ecx
        jne    loc_5_A965
        mov    eax, 0x13
        jmp    loc_5_A97F
loc_5_A965:
        test   ebx, ebx
        je     loc_5_A96B
        mov    dword ptr [ebx], ecx
loc_5_A96B:
        test   esi, esi
        je     loc_5_A974
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esi], eax
loc_5_A974:
        test   edi, edi
        je     loc_5_A97D
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edi], eax
loc_5_A97D:
        xor    eax, eax
loc_5_A97F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
_CONFIGMG_Next_Range:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        push   1
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A9AB
        mov    eax, 3
        jmp    loc_5_AA35
loc_5_A9AB:
        mov    edi, dword ptr [esp + 0x1c]
        push   1
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A9CB
        mov    eax, 3
        jmp    loc_5_AA35
loc_5_A9CB:
        xor    eax, eax
        mov    ebx, dword ptr [esp + 0x14]
        push   eax
        push   eax
        push   eax
        push   4
        push   ebx
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_A9EA
        mov    eax, 3
        jmp    loc_5_AA35
loc_5_A9EA:
        mov    ebp, dword ptr [ebx]
        mov    edx, 1
        mov    ecx, ebp
        call   sub_5_8335
        test   eax, eax
        jne    loc_5_AA03
        mov    eax, 0x12
        jmp    loc_5_AA35
loc_5_AA03:
        cmp    dword ptr [esp + 0x20], 0
        je     loc_5_AA11
        mov    eax, 4
        jmp    loc_5_AA35
loc_5_AA11:
        mov    eax, dword ptr [ebp + 4]
        test   eax, eax
        jne    loc_5_AA1F
        mov    eax, 0x13
        jmp    loc_5_AA35
loc_5_AA1F:
        mov    dword ptr [ebx], eax
        test   esi, esi
        je     loc_5_AA2A
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [esi], ecx
loc_5_AA2A:
        test   edi, edi
        je     loc_5_AA33
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [edi], eax
loc_5_AA33:
        xor    eax, eax
loc_5_AA35:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
_CONFIGMG_Dump_Range_List:
        mov    edx, 1
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_8317
        test   eax, eax
        mov    eax, 0x11
        je     loc_5_AA62
        mov    eax, dword ptr [esp + 8]
        and    al, 0xfe
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0xf
        add    eax, 4
loc_5_AA62:
        ret    
_CONFIGMG_Load_DLVxDs:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0x14], 0
        je     loc_5_AA74
        mov    eax, 4
        jmp    loc_5_AAC9
loc_5_AA74:
        cmp    dword ptr [ebp + 0x10], 3
        jb     loc_5_AA81
        mov    eax, 0x19
        jmp    loc_5_AAC9
loc_5_AA81:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_AA99
        mov    eax, 5
        jmp    loc_5_AAC9
loc_5_AA99:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 0xc]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        mov    eax, 3
        je     loc_5_AAC9
        push   0x13
        mov    ecx, 0                        ; -> sub_5_5854
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        call   sub_5_02D3
loc_5_AAC9:
        pop    ebp
        ret    
_CONFIGMG_Get_DDBs:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_5_AADE
        mov    eax, 4
        jmp    loc_5_AB51
loc_5_AADE:
        mov    esi, dword ptr [ebp + 0x10]
        cmp    esi, 3
        jb     loc_5_AAED
        mov    eax, 0x19
        jmp    loc_5_AB51
loc_5_AAED:
        mov    edx, 1
        mov    edi, dword ptr [ebp + 0x14]
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_AB07
        mov    eax, 5
        jmp    loc_5_AB51
loc_5_AB07:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   4
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_AB24
        mov    eax, 3
        jmp    loc_5_AB51
loc_5_AB24:
        xor    eax, eax
        mov    ebx, dword ptr [ebp + 0xc]
        push   eax
        push   eax
        push   eax
        push   4
        push   ebx
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        mov    eax, 3
        je     loc_5_AB51
        mov    eax, dword ptr [edi + esi*4 + 0x58]
        mov    ecx, dword ptr [edi + esi*4 + 0x4c]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        xor    eax, eax
        mov    dword ptr [edx], ecx
loc_5_AB51:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
_CONFIGMG_Get_CRC_CheckSum:
        cmp    dword ptr [esp + 0x10], 0
        push   ebx
        push   esi
        push   edi
        push   ebp
        je     loc_5_AB6B
        mov    eax, 4
        jmp    loc_5_ABEC
loc_5_AB6B:
        xor    eax, eax
        mov    edi, dword ptr [esp + 0x1c]
        push   eax
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_AB8A
        mov    eax, 3
        jmp    loc_5_ABEC
loc_5_AB8A:
        xor    eax, eax
        mov    esi, dword ptr [esp + 0x18]
        push   eax
        push   eax
        push   eax
        push   esi
        push   dword ptr [esp + 0x24]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_ABAB
        mov    eax, 3
        jmp    loc_5_ABEC
loc_5_ABAB:
        mov    ebx, dword ptr [esp + 0x14]
        test   esi, esi
        je     loc_5_ABEA
loc_5_ABB3:
        mov    edx, dword ptr [edi]
        mov    al, byte ptr [ebx]
        xor    al, dl
        dec    esi
        shr    edx, 8
        mov    cl, al
        shr    cl, 3
        inc    ebx
        movzx  ecx, cl
        and    cl, 0xfe
        mov    bp, word ptr [ecx]            ; -> obj7:0x0DA0
        xor    ecx, ecx
        mov    cl, al
        and    ecx, 0xf
        xor    bp, word ptr [ecx*2]          ; -> obj7:0x0D80
        movzx  eax, bp
        xor    eax, edx
        test   esi, esi
        mov    dword ptr [edi], eax
        jne    loc_5_ABB3
loc_5_ABEA:
        xor    eax, eax
loc_5_ABEC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
_CONFIGMG_Register_DevLoader:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     loc_5_AC00
        mov    eax, 4
        jmp    loc_5_AC27
loc_5_AC00:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     loc_5_AC25
        mov    esi, dword ptr [0]            ; -> obj7:0x0D74
        mov    edx, 0                        ; -> loc_5_93E1
        xor    ecx, ecx
        mov    dword ptr [0], eax            ; -> obj7:0x0D74
        call   sub_5_CC30
        mov    dword ptr [0], esi            ; -> obj7:0x0D74
loc_5_AC25:
        xor    eax, eax
loc_5_AC27:
        pop    esi
        ret    
_CONFIGMG_Setup_DevNode:
        test   dword ptr [esp + 8], 0xfffffffc
        push   esi
        je     loc_5_AC3E
        mov    eax, 4
        jmp    loc_5_ACBF
loc_5_AC3E:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_AC59
        mov    eax, 5
        jmp    loc_5_ACBF
loc_5_AC59:
        mov    eax, dword ptr [esp + 0xc]
        test   eax, eax
        je     loc_5_AC72
        cmp    eax, 1
        je     loc_5_AC87
        cmp    eax, 2
        je     loc_5_ACA3
        cmp    eax, 3
        je     loc_5_ACAC
        jmp    loc_5_ACBD
loc_5_AC72:
        push   esi
        call   sub_5_8780
        add    esp, 4
        xor    eax, eax
        mov    ecx, 0                        ; -> loc_5_201A
        push   eax
        push   eax
        push   eax
        jmp    loc_5_ACB6
loc_5_AC87:
        cmp    dword ptr [0], esi            ; -> obj7:0x00A8
        je     loc_5_ACBD
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     loc_5_ACBD
        xor    eax, eax
        mov    ecx, 0                        ; -> loc_5_2071
        push   eax
        push   eax
        push   eax
        jmp    loc_5_ACB6
loc_5_ACA3:
        mov    ecx, esi
        call   sub_5_8CCD
        jmp    loc_5_ACBD
loc_5_ACAC:
        xor    eax, eax
        mov    ecx, 0                        ; -> loc_5_2115
        push   eax
        push   eax
        push   eax
loc_5_ACB6:
        mov    edx, esi
        call   sub_5_02D3
loc_5_ACBD:
        xor    eax, eax
loc_5_ACBF:
        pop    esi
        ret    
_CONFIGMG_Reset_Children_Marks:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     loc_5_ACD0
        mov    eax, 4
        jmp    loc_5_AD09
loc_5_ACD0:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_ACEB
        mov    eax, 5
        jmp    loc_5_AD09
loc_5_ACEB:
        mov    esi, dword ptr [esi + 0x24]
        test   esi, esi
        je     loc_5_AD07
loc_5_ACF2:
        push   0
        mov    ecx, esi
        push   0x200
        call   sub_5_1923
        mov    esi, dword ptr [esi + 0x28]
        test   esi, esi
        jne    loc_5_ACF2
loc_5_AD07:
        xor    eax, eax
loc_5_AD09:
        pop    esi
        ret    
_CONFIGMG_Get_DevNode_Status:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_5_AD1E
        mov    eax, 4
        jmp    loc_5_AD8A
loc_5_AD1E:
        mov    edx, 1
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_AD38
        mov    eax, 5
        jmp    loc_5_AD8A
loc_5_AD38:
        push   1
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   0
        push   4
        push   ebx
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_AD57
        mov    eax, 3
        jmp    loc_5_AD8A
loc_5_AD57:
        push   1
        mov    esi, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_AD76
        mov    eax, 3
        jmp    loc_5_AD8A
loc_5_AD76:
        test   ebx, ebx
        je     loc_5_AD7F
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx], eax
loc_5_AD7F:
        test   esi, esi
        je     loc_5_AD88
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [esi], eax
loc_5_AD88:
        xor    eax, eax
loc_5_AD8A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
_CONFIGMG_Remove_Unmarked_Children:
        cmp    dword ptr [esp + 8], 0
        je     loc_5_AD9D
        mov    eax, 4
        jmp    loc_5_ADC9
loc_5_AD9D:
        mov    edx, 1
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     loc_5_ADC9
        xor    eax, eax
        mov    ecx, 0                        ; -> sub_5_909C
        mov    edx, dword ptr [esp + 4]
        push   eax
        push   eax
        push   eax
        call   sub_5_02D3
        xor    eax, eax
loc_5_ADC9:
        ret    
_CONFIGMG_ISAPNP_To_CM:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x14]
        test   edi, 0xfffffffc
        je     loc_5_ADE1
        mov    eax, 4
        jmp    loc_5_AE42
loc_5_ADE1:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_ADFB
        mov    eax, 5
        jmp    loc_5_AE42
loc_5_ADFB:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_AE18
        mov    eax, 3
        jmp    loc_5_AE42
loc_5_AE18:
        cmp    dword ptr [esi + 0x30], 0
        je     loc_5_AE22
        test   edi, edi
        je     loc_5_AE2D
loc_5_AE22:
        cmp    dword ptr [esi + 0x3c], 0
        je     loc_5_AE34
        cmp    edi, 2
        jne    loc_5_AE34
loc_5_AE2D:
        mov    eax, 0xa
        jmp    loc_5_AE42
loc_5_AE34:
        push   edi
        mov    edx, esi
        push   dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_5391
loc_5_AE42:
        pop    edi
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_CallBack_Device_Driver:
        push   ebp
        mov    eax, 4
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [0]            ; -> obj7:0x0D78
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_5_AE77
        mov    eax, dword ptr [ebp + 8]
        mov    edx, 0                        ; -> loc_5_9402
        xor    ecx, ecx
        mov    dword ptr [0], eax            ; -> obj7:0x0D78
        call   sub_5_CC30
        xor    eax, eax
        mov    dword ptr [0], esi            ; -> obj7:0x0D78
loc_5_AE77:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_CallBack_Enumerator:
        push   ebp
        mov    eax, 4
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [0]            ; -> obj7:0x0D7C
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_5_AEAB
        mov    eax, dword ptr [ebp + 8]
        mov    edx, 0                        ; -> loc_5_9426
        xor    ecx, ecx
        mov    dword ptr [0], eax            ; -> obj7:0x0D7C
        call   sub_5_CC30
        xor    eax, eax
        mov    dword ptr [0], esi            ; -> obj7:0x0D7C
loc_5_AEAB:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Get_Alloc_Log_Conf:
        test   dword ptr [esp + 0xc], 0xfffffffe
        push   ebx
        push   esi
        push   edi
        push   ebp
        je     loc_5_AEC6
        mov    eax, 4
        jmp    loc_5_B075
loc_5_AEC6:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0x18]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_AEE4
        mov    eax, 5
        jmp    loc_5_B075
loc_5_AEE4:
        xor    eax, eax
        mov    ebx, dword ptr [esp + 0x14]
        push   eax
        push   eax
        push   eax
        push   0xd8
        push   ebx
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_AF09
        mov    eax, 3
        jmp    loc_5_B075
loc_5_AF09:
        mov    edi, ebx
        xor    eax, eax
        mov    ecx, 0x36
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esp + 0x1c]
        and    eax, 1
        dec    eax
        cmp    eax, 1
        mov    eax, 3
        adc    eax, -1
        mov    esi, dword ptr [esi + eax*4 + 0x30]
        test   esi, esi
        jne    loc_5_AF39
        mov    eax, 0xe
        jmp    loc_5_B075
loc_5_AF39:
        xor    di, di
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_184D
        test   eax, eax
        je     loc_5_AF85
loc_5_AF4C:
        cmp    di, 9
        jae    loc_5_AF85
        mov    ecx, dword ptr [eax + 0x20]
        lea    ebp, [eax + 0x1c]
        movzx  edx, di
        inc    di
        mov    dword ptr [ebx + edx*4 + 2], ecx
        mov    ecx, dword ptr [ebp + 8]
        sub    ecx, dword ptr [ebp + 4]
        mov    bp, word ptr [ebp + 0xc]
        inc    ecx
        mov    dword ptr [ebx + edx*4 + 0x26], ecx
        mov    ecx, eax
        mov    word ptr [ebx + edx*2 + 0x4a], bp
        mov    edx, 1
        call   sub_5_1877
        test   eax, eax
        jne    loc_5_AF4C
loc_5_AF85:
        mov    word ptr [ebx], di
        mov    edx, 2
        xor    di, di
        mov    ecx, esi
        call   sub_5_184D
        test   eax, eax
        je     loc_5_AFD4
loc_5_AF9B:
        cmp    di, 0x14
        jae    loc_5_AFD4
        movzx  ecx, di
        inc    di
        lea    edx, [eax + 0x1c]
        lea    ebp, [ebx + ecx*2]
        mov    cx, word ptr [edx + 4]
        mov    dx, word ptr [edx + 6]
        sub    dx, cx
        inc    dx
        mov    word ptr [ebp + 0x5e], cx
        mov    ecx, eax
        mov    word ptr [ebp + 0x86], dx
        mov    edx, 2
        call   sub_5_1877
        test   eax, eax
        jne    loc_5_AF9B
loc_5_AFD4:
        mov    word ptr [ebx + 0x5c], di
        mov    edx, 4
        xor    di, di
        mov    ecx, esi
        call   sub_5_184D
        test   eax, eax
        je     loc_5_B01E
loc_5_AFEB:
        cmp    di, 7
        jae    loc_5_B01E
        movzx  ecx, di
        lea    edx, [ecx + ebx]
        mov    cl, byte ptr [eax + 0x1e]
        inc    di
        lea    ebp, [eax + 0x1c]
        mov    byte ptr [edx + 0xb0], cl
        mov    cl, byte ptr [ebp]
        mov    byte ptr [edx + 0xb7], cl
        mov    edx, 4
        mov    ecx, eax
        call   sub_5_1877
        test   eax, eax
        jne    loc_5_AFEB
loc_5_B01E:
        mov    word ptr [ebx + 0xae], di
        mov    edx, 3
        mov    ecx, esi
        xor    si, si
        call   sub_5_184D
        test   eax, eax
        je     loc_5_B06C
loc_5_B038:
        cmp    si, 7
        jae    loc_5_B06C
        mov    cl, byte ptr [eax + 0x1d]
        lea    edx, [eax + 0x1c]
        movzx  edi, si
        inc    si
        mov    byte ptr [edi + ebx + 0xc0], cl
        movzx  dx, byte ptr [edx]
        mov    word ptr [ebx + edi*2 + 0xc7], dx
        mov    ecx, eax
        mov    edx, 3
        call   sub_5_1877
        test   eax, eax
        jne    loc_5_B038
loc_5_B06C:
        mov    word ptr [ebx + 0xbe], si
        xor    eax, eax
loc_5_B075:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
_CONFIGMG_Get_DevNode_Key_Size:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B0A0
        mov    eax, 3
        jmp    loc_5_B121
loc_5_B0A0:
        mov    edi, dword ptr [ebp + 0x14]
        test   edi, 0xfffffcfe
        je     loc_5_B0B2
        mov    eax, 4
        jmp    loc_5_B121
loc_5_B0B2:
        mov    eax, edi
        and    ah, 3
        cmp    ah, 3
        jne    loc_5_B0C3
        mov    eax, 4
        jmp    loc_5_B121
loc_5_B0C3:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_B0DB
        mov    eax, 5
        jmp    loc_5_B121
loc_5_B0DB:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 0x10]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B0F9
        mov    eax, 3
        jmp    loc_5_B121
loc_5_B0F9:
        push   edi
        mov    edx, 0                        ; -> obj7:0x00CC
        push   dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_8613
        test   eax, eax
        mov    eax, 0x1c
        je     loc_5_B121
        mov    ecx, 0                        ; -> obj7:0x00CC
        call   sub_5_C3C4
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_B121:
        pop    edi
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Get_DevNode_Key:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 0x14]
        push   eax
        push   esi
        push   dword ptr [ebp + 0x10]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B14C
        mov    eax, 3
        jmp    loc_5_B1EE
loc_5_B14C:
        mov    edi, dword ptr [ebp + 0x18]
        test   edi, 0xfffffcfe
        je     loc_5_B161
        mov    eax, 4
        jmp    loc_5_B1EE
loc_5_B161:
        mov    eax, edi
        and    ah, 3
        cmp    ah, 3
        jne    loc_5_B172
        mov    eax, 4
        jmp    loc_5_B1EE
loc_5_B172:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_B18A
        mov    eax, 5
        jmp    loc_5_B1EE
loc_5_B18A:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 0xc]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B1A8
        mov    eax, 3
        jmp    loc_5_B1EE
loc_5_B1A8:
        push   edi
        mov    edx, 0                        ; -> obj7:0x00CC
        push   dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_8613
        test   eax, eax
        jne    loc_5_B1C4
        mov    eax, 0x1c
        jmp    loc_5_B1EE
loc_5_B1C4:
        mov    ecx, 0                        ; -> obj7:0x00CC
        call   sub_5_C3C4
        mov    edi, eax
        lea    eax, [eax + 1]
        cmp    eax, esi
        jb     loc_5_B1D9
        mov    eax, esi
loc_5_B1D9:
        push   eax
        mov    edx, 0                        ; -> obj7:0x00CC
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_5_5B5D
        cmp    esi, edi
        sbb    eax, eax
        and    eax, 0x1a
loc_5_B1EE:
        pop    edi
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Read_Registry_Value:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_B211
        mov    eax, 5
        jmp    loc_5_B2C4
loc_5_B211:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 0xc]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B232
        mov    eax, 3
        jmp    loc_5_B2C4
loc_5_B232:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 0x10]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B250
        mov    eax, 3
        jmp    loc_5_B2C4
loc_5_B250:
        xor    eax, eax
        mov    esi, dword ptr [ebp + 0x1c]
        push   eax
        push   eax
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B26E
        mov    eax, 3
        jmp    loc_5_B2C4
loc_5_B26E:
        push   1
        push   0
        push   0
        push   dword ptr [esi]
        push   dword ptr [ebp + 0x18]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B28C
        mov    eax, 3
        jmp    loc_5_B2C4
loc_5_B28C:
        mov    eax, dword ptr [ebp + 0x20]
        test   eax, 0xfffffcfe
        je     loc_5_B29D
        mov    eax, 4
        jmp    loc_5_B2C4
loc_5_B29D:
        mov    ecx, eax
        and    ch, 3
        cmp    ch, 3
        jne    loc_5_B2AE
        mov    eax, 4
        jmp    loc_5_B2C4
loc_5_B2AE:
        push   eax
        mov    edx, dword ptr [ebp + 0xc]
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_5_87D6
loc_5_B2C4:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Write_Registry_Value:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_B382
        mov    eax, dword ptr [esi + 4]
        xor    ecx, ecx
        cmp    byte ptr [eax], cl
        je     loc_5_B382
        push   1
        push   ecx
        push   ecx
        push   1
        push   dword ptr [ebp + 0xc]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B30B
        mov    eax, 3
        jmp    loc_5_B387
loc_5_B30B:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 0x10]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B329
        mov    eax, 3
        jmp    loc_5_B387
loc_5_B329:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B347
        mov    eax, 3
        jmp    loc_5_B387
loc_5_B347:
        mov    eax, dword ptr [ebp + 0x20]
        test   eax, 0xfffffcfe
        je     loc_5_B358
        mov    eax, 4
        jmp    loc_5_B387
loc_5_B358:
        mov    ecx, eax
        and    ch, 3
        cmp    ch, 3
        jne    loc_5_B369
        mov    eax, 4
        jmp    loc_5_B387
loc_5_B369:
        push   eax
        mov    edx, dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        mov    ecx, esi
        call   sub_5_886D
        jmp    loc_5_B387
loc_5_B382:
        mov    eax, 5
loc_5_B387:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Disable_DevNode:
        push   ebx
        mov    edx, 1
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_B44C
        cmp    dword ptr [0], esi            ; -> obj7:0x00A8
        je     loc_5_B44C
        mov    edi, dword ptr [esp + 0x14]
        test   edi, 0xfffffffc
        je     loc_5_B3C7
        mov    eax, 4
        jmp    loc_5_B451
loc_5_B3C7:
        test   edi, 2
        je     loc_5_B3DD
        mov    edx, 0x1d
        mov    ecx, esi
        call   sub_5_731F
        jmp    loc_5_B448
loc_5_B3DD:
        xor    ebx, ebx
        cmp    dword ptr [esi + 0x10], ebx
        jne    loc_5_B445
        push   ebx
        mov    ecx, esi
        push   8
        push   ebx
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_B434
        test   edi, 1
        jne    loc_5_B434
        push   ebx
        mov    eax, 0x2000
        push   eax
        mov    ecx, esi
        push   ebx
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_B418
        mov    eax, 0x28
        jmp    loc_5_B451
loc_5_B418:
        xor    eax, eax
        mov    ecx, 0                        ; -> loc_5_1207
        push   eax
        mov    edx, esi
        push   eax
        push   eax
        call   sub_5_02D3
        test   eax, eax
        jne    loc_5_B434
        mov    eax, 0x28
        jmp    loc_5_B451
loc_5_B434:
        xor    eax, eax
        mov    ecx, 0                        ; -> sub_5_1FB0
        push   eax
        mov    edx, esi
        push   eax
        push   eax
        call   sub_5_02D3
loc_5_B445:
        inc    dword ptr [esi + 0x10]
loc_5_B448:
        xor    eax, eax
        jmp    loc_5_B451
loc_5_B44C:
        mov    eax, 5
loc_5_B451:
        pop    edi
        pop    esi
        pop    ebx
        ret    
_CONFIGMG_Enable_DevNode:
        push   esi
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_B49E
        cmp    esi, dword ptr [0]            ; -> obj7:0x00A8
        je     loc_5_B49E
        cmp    dword ptr [esp + 0xc], 0
        je     loc_5_B480
        mov    eax, 4
        jmp    loc_5_B4A3
loc_5_B480:
        mov    eax, dword ptr [esi + 0x10]
        test   eax, eax
        je     loc_5_B49A
        dec    eax
        mov    dword ptr [esi + 0x10], eax
        jne    loc_5_B49A
        cmp    dword ptr [esi + 0xc], 0x16
        jne    loc_5_B49A
        mov    ecx, esi
        call   sub_5_7024
loc_5_B49A:
        xor    eax, eax
        jmp    loc_5_B4A3
loc_5_B49E:
        mov    eax, 5
loc_5_B4A3:
        pop    esi
        ret    
_CONFIGMG_Move_DevNode:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   ebp
        test   esi, esi
        je     loc_5_B4D1
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_B5BE
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     loc_5_B5BE
loc_5_B4D1:
        mov    edx, 1
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_B5BE
        mov    eax, dword ptr [edi + 4]
        cmp    byte ptr [eax], 0
        je     loc_5_B5BE
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_5_B506
        mov    eax, 4
        jmp    loc_5_B5C3
loc_5_B506:
        test   esi, esi
        je     loc_5_B57F
        mov    ecx, dword ptr [edi + 4]
        call   sub_5_C3C4
        mov    ebx, eax
        push   0x201
        lea    eax, [ebx + 1]
        push   eax
        call   sub_5_D134
        add    esp, 8
        mov    ebp, eax
        test   eax, eax
        jne    loc_5_B541
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 2
        jmp    loc_5_B5C3
loc_5_B541:
        push   esi
        call   sub_5_8780
        add    esp, 4
        mov    edx, dword ptr [edi + 4]
        mov    ecx, ebp
        push   ebx
        xor    ebx, ebx
        call   sub_5_5B5D
        push   ebx
        mov    ecx, esi
        push   0x1000
        mov    dword ptr [esi + 4], ebp
        call   sub_5_1906
        mov    ecx, edi
        mov    dword ptr [edi + 0x10], ebx
        call   sub_5_7024
        mov    edx, 0x19
        mov    ecx, edi
        call   sub_5_731F
        jmp    loc_5_B5BA
loc_5_B57F:
        xor    ebx, ebx
        mov    ecx, 0                        ; -> sub_5_650C
        push   ebx
        mov    edx, edi
        push   ebx
        push   ebx
        call   sub_5_02D3
        mov    esi, dword ptr [edi + 0x24]
        cmp    esi, ebx
        je     loc_5_B5BA
loc_5_B597:
        mov    ecx, esi
        call   sub_5_7024
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_6600
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_731F
        mov    esi, dword ptr [esi + 0x28]
        test   esi, esi
        jne    loc_5_B597
loc_5_B5BA:
        xor    eax, eax
        jmp    loc_5_B5C3
loc_5_B5BE:
        mov    eax, 5
loc_5_B5C3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
_CONFIGMG_Set_Bus_Info:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_5_B5DE
        mov    eax, 4
        jmp    loc_5_B68B
loc_5_B5DE:
        xor    eax, eax
        mov    edi, dword ptr [ebp + 0x10]
        push   eax
        push   eax
        push   eax
        push   edi
        push   dword ptr [ebp + 0x14]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B600
        mov    eax, 3
        jmp    loc_5_B68B
loc_5_B600:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_B618
        mov    eax, 5
        jmp    loc_5_B68B
loc_5_B618:
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AE7
        mov    esi, eax
        mov    eax, dword ptr [eax + 0x68]
        test   eax, eax
        je     loc_5_B644
        test   byte ptr [1], 4               ; -> obj7:0x00AD
        jne    loc_5_B63D
        push   0
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_B63D:
        mov    dword ptr [esi + 0x68], 0
loc_5_B644:
        test   edi, edi
        je     loc_5_B683
        push   0x201
        lea    eax, [edi + 4]
        push   eax
        call   sub_5_D134
        mov    dword ptr [esi + 0x68], eax
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_B675
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 2
        jmp    loc_5_B68B
loc_5_B675:
        push   edi
        lea    ecx, [ebx + 4]
        mov    edx, dword ptr [ebp + 0x14]
        call   sub_5_5B5D
        mov    dword ptr [ebx], edi
loc_5_B683:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x64], eax
        xor    eax, eax
loc_5_B68B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
_CONFIGMG_Get_Bus_Info:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_5_B6A6
        mov    eax, 4
        jmp    loc_5_B78C
loc_5_B6A6:
        push   1
        mov    esi, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B6C8
        mov    eax, 3
        jmp    loc_5_B78C
loc_5_B6C8:
        mov    edi, dword ptr [ebp + 0x10]
        xor    eax, eax
        test   edi, edi
        je     loc_5_B6F1
        push   eax
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B6ED
        mov    eax, 3
        jmp    loc_5_B78C
loc_5_B6ED:
        mov    ebx, dword ptr [edi]
        jmp    loc_5_B6F3
loc_5_B6F1:
        xor    ebx, ebx
loc_5_B6F3:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   ebx
        push   dword ptr [ebp + 0x14]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B70F
        mov    eax, 3
        jmp    loc_5_B78C
loc_5_B70F:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_B727
        mov    eax, 5
        jmp    loc_5_B78C
loc_5_B727:
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AE7
        test   esi, esi
        je     loc_5_B738
        mov    ecx, dword ptr [eax + 0x64]
        mov    dword ptr [esi], ecx
loc_5_B738:
        mov    edx, dword ptr [eax + 0x68]
        mov    esi, 0
        test   edx, edx
        je     loc_5_B746
        mov    esi, dword ptr [edx]
loc_5_B746:
        test   ebx, ebx
        jne    loc_5_B754
        test   edi, edi
        je     loc_5_B750
        mov    dword ptr [edi], esi
loc_5_B750:
        xor    eax, eax
        jmp    loc_5_B78C
loc_5_B754:
        cmp    ebx, esi
        jb     loc_5_B75A
        mov    ebx, esi
loc_5_B75A:
        test   edi, edi
        je     loc_5_B760
        mov    dword ptr [edi], ebx
loc_5_B760:
        push   ebx
        add    edx, 4
        mov    ecx, dword ptr [ebp + 0x14]
        call   sub_5_5B5D
        mov    edx, 1
        cmp    ebx, esi
        adc    edx, -1
        cmp    ebx, 1
        sbb    eax, eax
        inc    eax
        and    edx, eax
        mov    eax, 0
        cmp    edx, 1
        adc    eax, -1
        and    eax, 0x1a
loc_5_B78C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
_CONFIGMG_Set_HW_Prof:
        cmp    dword ptr [esp + 8], 0
        mov    eax, 4
        jne    loc_5_B7B1
        push   0x13
        mov    ecx, 0                        ; -> loc_5_326A
        mov    edx, dword ptr [esp + 8]
        push   0
        push   0
        call   sub_5_02D3
loc_5_B7B1:
        ret    
_CONFIGMG_Recompute_HW_Prof:
        push   ebp
        mov    eax, 4
        mov    ebp, esp
        test   dword ptr [esp + 0x10], 0xfffffffe
        jne    loc_5_B7D9
        push   0x13
        mov    ecx, 0                        ; -> loc_5_38E2
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_02D3
loc_5_B7D9:
        pop    ebp
        ret    
_CONFIGMG_Query_Change_HW_Prof:
        push   ebp
        mov    eax, 4
        mov    ebp, esp
        test   dword ptr [esp + 0x10], 0xfffffffc
        jne    loc_5_B802
        push   0x13
        mov    ecx, 0                        ; -> loc_5_3E02
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_02D3
loc_5_B802:
        pop    ebp
        ret    
_CONFIGMG_Get_Device_Driver_Private_DWord:
        cmp    dword ptr [esp + 0xc], 0
        push   esi
        push   edi
        je     loc_5_B814
        mov    eax, 4
        jmp    loc_5_B853
loc_5_B814:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_B82F
        mov    eax, 5
        jmp    loc_5_B853
loc_5_B82F:
        xor    eax, eax
        mov    edi, dword ptr [esp + 0x10]
        push   eax
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        mov    eax, 3
        je     loc_5_B853
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [edi], eax
        xor    eax, eax
loc_5_B853:
        pop    edi
        pop    esi
        ret    
_CONFIGMG_Set_Device_Driver_Private_DWord:
        cmp    dword ptr [esp + 0xc], 0
        push   esi
        je     loc_5_B865
        mov    eax, 4
        jmp    loc_5_B887
loc_5_B865:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     loc_5_B887
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 0x14], eax
        xor    eax, eax
loc_5_B887:
        pop    esi
        ret    
_CONFIGMG_Get_HW_Prof_Flags:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B8AB
        mov    eax, 3
        jmp    loc_5_B8F2
loc_5_B8AB:
        mov    ecx, esi
        call   sub_5_C3C4
        test   eax, eax
        jne    loc_5_B8BD
        mov    eax, 3
        jmp    loc_5_B8F2
loc_5_B8BD:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   4
        push   dword ptr [ebp + 0x10]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B8DA
        mov    eax, 3
        jmp    loc_5_B8F2
loc_5_B8DA:
        cmp    dword ptr [ebp + 0x14], 0
        mov    eax, 4
        jne    loc_5_B8F2
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, esi
        push   dword ptr [ebp + 0x10]
        call   sub_5_410E
loc_5_B8F2:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Set_HW_Prof_Flags:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_B917
        mov    eax, 3
        jmp    loc_5_B948
loc_5_B917:
        mov    ecx, esi
        call   sub_5_C3C4
        test   eax, eax
        jne    loc_5_B929
        mov    eax, 3
        jmp    loc_5_B948
loc_5_B929:
        cmp    dword ptr [ebp + 0x14], 0
        mov    eax, 4
        jne    loc_5_B948
        push   0x13
        mov    ecx, 0                        ; -> loc_5_424F
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        mov    edx, esi
        call   sub_5_02D3
loc_5_B948:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Read_Registry_Log_Confs:
        cmp    dword ptr [esp + 8], 0
        je     loc_5_B959
        mov    eax, 4
        jmp    loc_5_B982
loc_5_B959:
        mov    edx, 1
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     loc_5_B982
        push   1
        mov    edx, 0                        ; -> obj7:0x0DEC
        mov    ecx, dword ptr [esp + 8]
        push   0
        call   sub_5_8B0B
loc_5_B982:
        ret    
_CONFIGMG_Run_Detection:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [ebp + 8], 0
        je     loc_5_B996
        mov    eax, 4
        jmp    loc_5_B9BC
loc_5_B996:
        lea    edx, [ebp - 4]
        xor    ecx, ecx
        or     byte ptr [0], 0x20            ; -> obj7:0x00AC
        call   sub_5_44B6
        or     byte ptr [ebp - 1], 0x80
        mov    edx, dword ptr [ebp - 4]
        xor    ecx, ecx
        call   sub_5_4572
        call   sub_5_7B5F
        xor    eax, eax
loc_5_B9BC:
        mov    esp, ebp
        pop    ebp
        ret    
_CONFIGMG_Fail_Change_HW_Prof:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     loc_5_B9CF
        mov    eax, 4
        jmp    loc_5_BA03
loc_5_B9CF:
        mov    esi, dword ptr [esp + 8]
        test   esi, esi
        je     loc_5_B9F1
        cmp    esi, -1
        je     loc_5_B9F1
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     loc_5_BA03
loc_5_B9F1:
        push   0x13
        mov    ecx, 0                        ; -> sub_5_40A1
        push   0
        mov    edx, esi
        push   0
        call   sub_5_02D3
loc_5_BA03:
        pop    esi
        ret    
_CONFIGMG_Set_Private_Problem:
        cmp    dword ptr [esp + 0xc], 0
        push   esi
        je     loc_5_BA14
        mov    eax, 4
        jmp    loc_5_BA44
loc_5_BA14:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_BA3F
        cmp    esi, dword ptr [0]            ; -> obj7:0x00A8
        je     loc_5_BA3F
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, esi
        call   sub_5_7393
        xor    eax, eax
        jmp    loc_5_BA44
loc_5_BA3F:
        mov    eax, 5
loc_5_BA44:
        pop    esi
        ret    
_CONFIGMG_Debug_DevNode:
        cmp    dword ptr [esp + 8], 0
        mov    eax, 4
        jne    loc_5_BA6A
        mov    edx, 1
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_1AC6
        cmp    eax, 1
        sbb    eax, eax
        and    al, 0xf2
        add    eax, 0x13
loc_5_BA6A:
        ret    
_CONFIGMG_Get_Hardware_Profile_Info:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   eax
        push   eax
        push   eax
        push   0x58
        push   dword ptr [ebp + 0xc]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_BA8B
        mov    eax, 3
        jmp    loc_5_BAA1
loc_5_BA8B:
        cmp    dword ptr [ebp + 0x10], 0
        mov    eax, 4
        jne    loc_5_BAA1
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_4617
loc_5_BAA1:
        pop    ebp
        ret    
_CONFIGMG_Register_Enumerator_Function:
        cmp    dword ptr [esp + 0xc], 0
        push   esi
        je     loc_5_BAB2
        mov    eax, 4
        jmp    loc_5_BADE
loc_5_BAB2:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_BAD9
        cmp    esi, dword ptr [0]            ; -> obj7:0x00A8
        je     loc_5_BAD9
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 0x2c], eax
        xor    eax, eax
        jmp    loc_5_BADE
loc_5_BAD9:
        mov    eax, 5
loc_5_BADE:
        pop    esi
        ret    
_CONFIGMG_Call_Enumerator_Function:
        push   ebp
        mov    ebp, esp
        push   esi
        cmp    dword ptr [ebp + 0x1c], 0
        je     loc_5_BAF4
        mov    eax, 4
        jmp    loc_5_BB78
loc_5_BAF4:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_BB73
        cmp    dword ptr [0], esi            ; -> obj7:0x00A8
        je     loc_5_BB73
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_BB2D
        mov    eax, 3
        jmp    loc_5_BB78
loc_5_BB2D:
        push   0
        mov    eax, 0x20000
        push   eax
        mov    ecx, esi
        push   0
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_BB4E
        mov    eax, dword ptr [esi + 0x20]
        cmp    dword ptr [eax + 0x2c], 0
        jne    loc_5_BB4E
        mov    esi, eax
loc_5_BB4E:
        mov    eax, dword ptr [esi + 0x20]
        mov    ecx, dword ptr [eax + 0x2c]
        test   ecx, ecx
        jne    loc_5_BB5F
        mov    eax, 0x20
        jmp    loc_5_BB78
loc_5_BB5F:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   esi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   ecx
        add    esp, 0x14
        jmp    loc_5_BB78
loc_5_BB73:
        mov    eax, 5
loc_5_BB78:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Add_ID:
        push   ebp
        mov    ebp, esp
        push   esi
        test   dword ptr [ebp + 0x10], 0xfffffffe
        je     loc_5_BB8F
        mov    eax, 4
        jmp    loc_5_BBE3
loc_5_BB8F:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_BBDE
        cmp    dword ptr [0], esi            ; -> obj7:0x00A8
        je     loc_5_BBDE
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 0xc]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_BBC7
        mov    eax, 3
        jmp    loc_5_BBE3
loc_5_BBC7:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, esi
        cmp    dword ptr [ebp + 0x10], 1
        sbb    eax, eax
        neg    eax
        push   eax
        call   sub_5_21CF
        xor    eax, eax
        jmp    loc_5_BBE3
loc_5_BBDE:
        mov    eax, 5
loc_5_BBE3:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Find_Range:
        push   ebp
        mov    ebp, esp
        push   1
        push   0
        push   0
        push   4
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_BC07
        mov    eax, 3
        jmp    loc_5_BC49
loc_5_BC07:
        cmp    dword ptr [ebp + 0x20], 0
        je     loc_5_BC14
        mov    eax, 4
        jmp    loc_5_BC49
loc_5_BC14:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0x1c]
        call   sub_5_8317
        test   eax, eax
        mov    eax, 0x11
        je     loc_5_BC49
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_5_7EB3
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x13
loc_5_BC49:
        pop    ebp
        ret    
_CONFIGMG_Get_Global_State:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_BC70
        mov    eax, 3
        jmp    loc_5_BCC4
loc_5_BC70:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_5_BC7D
        mov    eax, 4
        jmp    loc_5_BCC4
loc_5_BC7D:
        xor    ecx, ecx
        cmp    dword ptr [0], ecx            ; -> obj7:0x008C
        jne    loc_5_BC8C
        mov    ecx, 1
loc_5_BC8C:
        mov    dword ptr [ebp - 4], esp
        mov    eax, dword ptr [ebp - 4]
        sub    eax, dword ptr [0]            ; -> obj7:0x0090
        cmp    eax, 0x4000
        jae    loc_5_BCA2
        or     cl, 2
loc_5_BCA2:
        mov    eax, dword ptr [0]            ; -> obj7:0x00AC
        test   al, 0x40
        jne    loc_5_BCB2
        test   al, 1
        je     loc_5_BCB2
        or     cl, 4
loc_5_BCB2:
        test   al, 8
        je     loc_5_BCB9
        or     cl, 8
loc_5_BCB9:
        test   al, 0x20
        je     loc_5_BCC0
        or     cl, 0x10
loc_5_BCC0:
        xor    eax, eax
        mov    dword ptr [esi], ecx
loc_5_BCC4:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
_CONFIGMG_Broadcast_Device_Change_Message:
        push   ebp
        mov    ebp, esp
        test   dword ptr [esp + 0x10], 0xfffffffe
        je     loc_5_BCDD
        mov    eax, 4
        jmp    loc_5_BD06
loc_5_BCDD:
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 0x10
        test   ax, ax
        mov    eax, 0x13
        jne    loc_5_BD06
        push   0x13
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 1
        mov    ecx, 0                        ; -> sub_5_5F83
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        call   sub_5_02D3
loc_5_BD06:
        pop    ebp
        ret    
_CONFIGMG_Call_DevNode_Handler:
        push   ebp
        mov    ebp, esp
        push   esi
        test   dword ptr [ebp + 0x10], 0xfffffffe
        je     loc_5_BD1C
        mov    eax, 4
        jmp    loc_5_BD56
loc_5_BD1C:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_BD51
        cmp    esi, dword ptr [0]            ; -> obj7:0x00A8
        je     loc_5_BD51
        push   0x13
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 1
        mov    ecx, 0                        ; -> loc_5_213C
        push   eax
        mov    edx, esi
        push   dword ptr [ebp + 0xc]
        call   sub_5_02D3
        jmp    loc_5_BD56
loc_5_BD51:
        mov    eax, 5
loc_5_BD56:
        pop    esi
        pop    ebp
        ret    
_CONFIGMG_Remove_Reinsert_All:
        test   dword ptr [esp + 4], 0xfffffffe
        mov    eax, 4
        jne    loc_5_BD80
        xor    eax, eax
        mov    edx, dword ptr [esp + 4]
        push   eax
        and    edx, 1
        push   eax
        mov    ecx, 0                        ; -> sub_5_23DF
        push   eax
        call   sub_5_02D3
        xor    eax, eax
loc_5_BD80:
        ret    
CONFIGMG_Service_91:
        push   ebp
        mov    ebp, esp
        test   dword ptr [esp + 0x10], 0xfffffffe
        je     loc_5_BD95
        mov    eax, 4
        jmp    loc_5_BDC7
loc_5_BD95:
        mov    ecx, dword ptr [ebp + 0xc]
        test   ecx, 0xfe449d3f
        je     loc_5_BDA7
        mov    eax, 0x1f
        jmp    loc_5_BDC7
loc_5_BDA7:
        mov    eax, dword ptr [ebp + 0x10]
        test   eax, eax
        je     loc_5_BDB5
        cmp    eax, 1
        je     loc_5_BDBF
        jmp    loc_5_BDC5
loc_5_BDB5:
        mov    eax, dword ptr [ebp + 8]
        not    ecx
        and    dword ptr [eax + 8], ecx
        jmp    loc_5_BDC5
loc_5_BDBF:
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 8], ecx
loc_5_BDC5:
        xor    eax, eax
loc_5_BDC7:
        pop    ebp
        ret    
CONFIGMG_Service_92:
        test   dword ptr [esp + 8], 0xfffffffe
        mov    eax, 4
        jne    loc_5_BDF4
        mov    eax, dword ptr [esp + 8]
        push   0
        push   0
        and    eax, 1
        push   eax
        mov    ecx, 0                        ; -> sub_5_1FBE
        mov    edx, dword ptr [esp + 0x10]
        call   sub_5_02D3
        xor    eax, eax
loc_5_BDF4:
        ret    
CONFIGMG_Service_93:
        cmp    dword ptr [esp + 0xc], 0
        push   esi
        je     loc_5_BE04
        mov    eax, 4
        jmp    loc_5_BE2E
loc_5_BE04:
        xor    eax, eax
        mov    esi, dword ptr [esp + 8]
        push   eax
        push   eax
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        mov    eax, 3
        je     loc_5_BE2E
        mov    ecx, dword ptr [esp + 0xc]
        call   sub_5_194A
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_BE2E:
        pop    esi
        ret    
CONFIGMG_Service_94:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     loc_5_BE3F
        mov    eax, 4
        jmp    loc_5_BE88
loc_5_BE3F:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_BE83
        cmp    esi, dword ptr [0]            ; -> obj7:0x00A8
        je     loc_5_BE83
        xor    eax, eax
        cmp    dword ptr [esi + 0x44], eax
        je     loc_5_BE67
        mov    dword ptr [esi + 0x3c], eax
        jmp    loc_5_BE88
loc_5_BE67:
        mov    eax, dword ptr [esi + 0x3c]
        push   0x800000
        mov    ecx, esi
        mov    dword ptr [esi + 0x44], eax
        xor    eax, eax
        push   eax
        mov    dword ptr [esi + 0x3c], eax
        call   sub_5_1923
        xor    eax, eax
        jmp    loc_5_BE88
loc_5_BE83:
        mov    eax, 5
loc_5_BE88:
        pop    esi
        ret    
CONFIGMG_Service_95:
        cmp    dword ptr [esp + 0xc], 0
        push   esi
        push   edi
        je     loc_5_BE9A
        mov    eax, 4
        jmp    loc_5_BF02
loc_5_BE9A:
        mov    edx, 1
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_BEFD
        mov    esi, dword ptr [esp + 0xc]
        test   esi, esi
        je     loc_5_BEC6
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_BEFD
loc_5_BEC6:
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_1D59
        test   eax, eax
        jne    loc_5_BEFD
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_1D59
        test   eax, eax
        jne    loc_5_BEFD
        mov    dword ptr [edi + 0x74], esi
        test   esi, esi
        jne    loc_5_BEF4
        cmp    dword ptr [edi + 0xc], 0x1f
        jne    loc_5_BEF4
        mov    ecx, edi
        call   sub_5_7024
loc_5_BEF4:
        call   sub_5_7B5F
        xor    eax, eax
        jmp    loc_5_BF02
loc_5_BEFD:
        mov    eax, 5
loc_5_BF02:
        pop    edi
        pop    esi
        ret    
CONFIGMG_Service_96:
        cmp    dword ptr [esp + 0xc], 0
        push   esi
        push   edi
        je     loc_5_BF15
        mov    eax, 4
        jmp    loc_5_BF6E
loc_5_BF15:
        mov    edi, dword ptr [esp + 0xc]
        push   1
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_BF35
        mov    eax, 3
        jmp    loc_5_BF6E
loc_5_BF35:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_BF69
        cmp    esi, dword ptr [0]            ; -> obj7:0x00A8
        je     loc_5_BF69
        mov    eax, dword ptr [esi + 0x74]
        test   eax, eax
        je     loc_5_BF62
        test   edi, edi
        je     loc_5_BF5E
        mov    dword ptr [edi], eax
loc_5_BF5E:
        xor    eax, eax
        jmp    loc_5_BF6E
loc_5_BF62:
        mov    eax, 0x2c
        jmp    loc_5_BF6E
loc_5_BF69:
        mov    eax, 5
loc_5_BF6E:
        pop    edi
        pop    esi
        ret    
CONFIGMG_Service_97:
        push   esi
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_BFB2
        cmp    esi, dword ptr [0]            ; -> obj7:0x00A8
        je     loc_5_BFB2
        cmp    dword ptr [esp + 0xc], 0
        je     loc_5_BF9C
        mov    eax, 4
        jmp    loc_5_BFB7
loc_5_BF9C:
        push   0x13
        mov    ecx, 0                        ; -> sub_5_7613
        push   0
        mov    edx, esi
        push   0
        call   sub_5_02D3
        xor    eax, eax
        jmp    loc_5_BFB7
loc_5_BFB2:
        mov    eax, 5
loc_5_BFB7:
        pop    esi
        ret    
CONFIGMG_Service_99:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_BFD5
        mov    eax, 0x11
        jmp    loc_5_C026
loc_5_BFD5:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_BFED
        mov    eax, 0x11
        jmp    loc_5_C026
loc_5_BFED:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_C005
        mov    eax, 0x11
        jmp    loc_5_C026
loc_5_C005:
        cmp    dword ptr [ebp + 0x14], 0
        mov    eax, 4
        jne    loc_5_C026
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        call   sub_5_7FAF
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_C026:
        pop    ebp
        ret    
CONFIGMG_Service_98:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_C044
        mov    eax, 0x11
        jmp    loc_5_C095
loc_5_C044:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_C05C
        mov    eax, 0x11
        jmp    loc_5_C095
loc_5_C05C:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_C074
        mov    eax, 0x11
        jmp    loc_5_C095
loc_5_C074:
        cmp    dword ptr [ebp + 0x14], 0
        mov    eax, 4
        jne    loc_5_C095
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        call   sub_5_808F
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_C095:
        pop    ebp
        ret    
CONFIGMG_Service_100:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_C0B3
        mov    eax, 5
        jmp    loc_5_C0F4
loc_5_C0B3:
        mov    ecx, dword ptr [ebp + 0xc]
        test   ecx, 0xfffffff0
        je     loc_5_C0C5
        mov    eax, 4
        jmp    loc_5_C0F4
loc_5_C0C5:
        lea    eax, [ecx - 1]
        test   ecx, eax
        je     loc_5_C0D3
        mov    eax, 0x1f
        jmp    loc_5_C0F4
loc_5_C0D3:
        test   dword ptr [ebp + 0x10], 0xfffffffe
        mov    eax, 4
        jne    loc_5_C0F4
        push   0x13
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        push   ecx
        mov    ecx, 0                        ; -> loc_5_6F35
        call   sub_5_02D3
loc_5_C0F4:
        pop    ebp
        ret    
CONFIGMG_Service_101:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_C112
        mov    eax, 5
        jmp    loc_5_C145
loc_5_C112:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   4
        push   dword ptr [ebp + 0xc]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_C12F
        mov    eax, 3
        jmp    loc_5_C145
loc_5_C12F:
        cmp    dword ptr [ebp + 0x10], 0
        mov    eax, 4
        jne    loc_5_C145
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_6FB9
loc_5_C145:
        mov    esp, ebp
        pop    ebp
        ret    
CONFIGMG_Service_102:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_C165
        mov    eax, 5
        jmp    loc_5_C18B
loc_5_C165:
        test   dword ptr [ebp + 0xc], 0xffffff80
        je     loc_5_C175
        mov    eax, 4
        jmp    loc_5_C18B
loc_5_C175:
        cmp    dword ptr [ebp + 0x10], 0
        mov    eax, 4
        jne    loc_5_C18B
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_6FC9
loc_5_C18B:
        mov    esp, ebp
        pop    ebp
        ret    
CONFIGMG_Service_103:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    loc_5_C1AB
        mov    eax, 5
        jmp    loc_5_C1DE
loc_5_C1AB:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   4
        push   dword ptr [ebp + 0xc]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_C1C8
        mov    eax, 3
        jmp    loc_5_C1DE
loc_5_C1C8:
        cmp    dword ptr [ebp + 0x10], 0
        mov    eax, 4
        jne    loc_5_C1DE
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_6FD7
loc_5_C1DE:
        mov    esp, ebp
        pop    ebp
        ret    
CONFIGMG_Service_104:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_C202
        mov    eax, 3
        jmp    loc_5_C230
loc_5_C202:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_C21A
        mov    eax, 0x11
        jmp    loc_5_C230
loc_5_C21A:
        cmp    dword ptr [ebp + 0x10], 0
        mov    eax, 4
        jne    loc_5_C230
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_812A
loc_5_C230:
        pop    ebp
        ret    
CONFIGMG_Service_105:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_C252
        mov    eax, 3
        jmp    loc_5_C280
loc_5_C252:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_8317
        test   eax, eax
        jne    loc_5_C26A
        mov    eax, 0x11
        jmp    loc_5_C280
loc_5_C26A:
        cmp    dword ptr [ebp + 0x10], 0
        mov    eax, 4
        jne    loc_5_C280
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_8255
loc_5_C280:
        pop    ebp
        ret    
CONFIGMG_Service_106:
        cmp    dword ptr [esp + 0xc], 0
        push   esi
        push   edi
        je     loc_5_C292
        mov    eax, 4
        jmp    loc_5_C2D1
loc_5_C292:
        mov    edx, 1
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, edi
        call   sub_5_1450
        test   eax, eax
        jne    loc_5_C2AD
        mov    eax, 7
        jmp    loc_5_C2D1
loc_5_C2AD:
        xor    eax, eax
        mov    esi, dword ptr [esp + 0xc]
        push   eax
        push   eax
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        mov    eax, 3
        je     loc_5_C2D1
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_C2D1:
        pop    edi
        pop    esi
        ret    
CONFIGMG_Service_107:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     loc_5_C2E3
        mov    eax, 4
        jmp    loc_5_C30F
loc_5_C2E3:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_C30A
        cmp    esi, dword ptr [0]            ; -> obj7:0x00A8
        je     loc_5_C30A
        mov    ecx, esi
        call   sub_5_2488
        xor    eax, eax
        jmp    loc_5_C30F
loc_5_C30A:
        mov    eax, 5
loc_5_C30F:
        pop    esi
        ret    
CONFIGMG_Service_108:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_C336
        mov    eax, 3
        jmp    loc_5_C3C1
loc_5_C336:
        mov    eax, dword ptr [ebp + 0xc]
        xor    ecx, ecx
        sub    eax, 4
        push   ecx
        push   ecx
        push   ecx
        push   4
        push   eax
        call   sub_5_D12C
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_C357
        mov    eax, 3
        jmp    loc_5_C3C1
loc_5_C357:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_5_C364
        mov    eax, 4
        jmp    loc_5_C3C1
loc_5_C364:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax - 4]
        cmp    eax, 0x4c52574d
        je     loc_5_C37F
        cmp    eax, 0x574d5950
        je     loc_5_C39F
        mov    eax, 6
        jmp    loc_5_C3C1
loc_5_C37F:
        lea    ecx, [ebp + 0xc]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        jne    loc_5_C397
        mov    eax, 6
        jmp    loc_5_C3C1
loc_5_C397:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0xc]
        jmp    loc_5_C3BD
loc_5_C39F:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_5_1450
        test   eax, eax
        jne    loc_5_C3B7
        mov    eax, 7
        jmp    loc_5_C3C1
loc_5_C3B7:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 4]
loc_5_C3BD:
        xor    eax, eax
        mov    dword ptr [esi], ecx
loc_5_C3C1:
        pop    esi
        pop    ebp
        ret    
sub_5_C3C4:
        push   edi
        mov    eax, 0
        test   ecx, ecx
        je     loc_5_C3DE
        mov    edi, ecx
        sub    eax, eax
        mov    ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [ecx - 1]
loc_5_C3DE:
        pop    edi
        ret    
sub_5_C3E0:
        xor    eax, eax
        test   ecx, ecx
        je     loc_5_C3F6
        cmp    byte ptr [ecx], 0
        je     loc_5_C3F6
loc_5_C3EB:
        cmp    byte ptr [ecx], dl
        je     loc_5_C3FB
        inc    ecx
        inc    eax
        cmp    byte ptr [ecx], 0
        jne    loc_5_C3EB
loc_5_C3F6:
        mov    eax, 0xffffffff
loc_5_C3FB:
        ret    
sub_5_C3FC:
        push   esi
        push   edi
        mov    esi, ecx
        test   esi, esi
        je     loc_5_C429
        test   edx, edx
        je     loc_5_C429
        mov    eax, dword ptr [esp + 0xc]
        mov    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_5_C41E
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
loc_5_C41E:
        mov    eax, 1
        je     loc_5_C42B
        xor    eax, eax
        jmp    loc_5_C42B
loc_5_C429:
        xor    eax, eax
loc_5_C42B:
        pop    edi
        pop    esi
        ret    4
sub_5_C430:
        test   ecx, ecx
        je     loc_5_C44D
        cmp    byte ptr [ecx], 0
        je     loc_5_C44D
loc_5_C439:
        mov    al, byte ptr [ecx]
        cmp    al, 0x61
        jl     loc_5_C447
        cmp    al, 0x7a
        jg     loc_5_C447
        sub    al, 0x20
        mov    byte ptr [ecx], al
loc_5_C447:
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    loc_5_C439
loc_5_C44D:
        ret    
sub_5_C44E:
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        push   ebp
        mov    edi, edx
        mov    edx, 0xffffffff
        sub    eax, eax
        mov    ecx, edx
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    ebp, ecx
        mov    esi, edi
        mov    ecx, edx
        mov    edi, ebx
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        add    edi, edx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        pop    ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_C486:
        push   ebx
        sub    eax, eax
        push   esi
        mov    ebx, ecx
        push   edi
        mov    ecx, 0xffffffff
        mov    edi, edx
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_5_C4B0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        xor    edi, edi
        cmp    dword ptr [esi + 0x3c], edi
        jne    loc_5_C54C
        cmp    dword ptr [esi + 0x44], edi
        jne    loc_5_C54C
        push   edi
        mov    ecx, esi
        push   0x10
        push   edi
        push   0x10
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_C54C
        push   edi
        mov    eax, 0x1000
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_C54C
        push   edi
        mov    eax, 0x10000
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_C508
        cmp    dword ptr [esi + 0x24], edi
        jne    loc_5_C54C
loc_5_C508:
        xor    edi, edi
        mov    eax, 0x20000
        push   edi
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_C54C
        push   edi
        mov    ecx, esi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     loc_5_C54C
        mov    eax, 0x400000
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    loc_5_C54C
        push   esi
        call   sub_5_9014
        add    esp, 4
loc_5_C54C:
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
loc_5_C554:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, dword ptr [0]            ; -> obj7:0x00A8
        je     loc_5_C56F
        cmp    dword ptr [eax + 0x44], 0
        je     loc_5_C56F
        push   eax
        call   sub_5_122C
        add    esp, 4
loc_5_C56F:
        mov    eax, 1
        ret    
loc_5_C575:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [0], eax            ; -> obj7:0x00A8
        je     loc_5_C599
        mov    ecx, dword ptr [eax + 0x44]
        xor    edx, edx
        test   ecx, ecx
        je     loc_5_C599
        push   eax
        mov    dword ptr [eax + 0x38], ecx
        mov    dword ptr [eax + 0x44], edx
        call   sub_5_1033
        add    esp, 4
loc_5_C599:
        mov    eax, 1
        ret    
sub_5_C59F:
        push   edi
        push   0
        mov    edi, ecx
        push   0x10000
        call   sub_5_1906
        push   0
        mov    eax, 0x80
        push   eax
        mov    ecx, dword ptr [edi + 0x20]
        push   0
        push   eax
        call   sub_5_18E3
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        push   eax
        push   0                             ; -> loc_5_66C3
        push   edi
        call   _CONFIGMG_Register_Enumerator
        add    esp, 0xc
        pop    edi
        ret    
sub_5_C5D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x104
        mov    dword ptr [ebp - 4], 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        xor    ebx, ebx
        xor    edi, edi
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    edx, [ebp - 8]
        push   edx
        push   3
        push   0                             ; -> obj7:0x0DF8
        xor    edx, edx
        call   sub_5_87D6
        test   eax, eax
        jne    loc_5_C61F
        push   dword ptr [ebp - 8]
        push   ebx
        push   esi
        mov    edi, 1
        call   _CONFIGMG_Register_Enumerator
        add    esp, 0xc
loc_5_C61F:
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        xor    edx, edx
        push   3
        mov    ecx, esi
        mov    dword ptr [ebp - 4], 4
        push   0                             ; -> obj7:0x0E04
        call   sub_5_87D6
        test   eax, eax
        jne    loc_5_C65C
        mov    eax, dword ptr [ebp - 8]
        or     al, 4
        mov    ebx, 1
        push   eax
        push   0
        push   0
        push   esi
        call   sub_5_1F16
        add    esp, 0x10
loc_5_C65C:
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0x104]
        push   ecx
        xor    edx, edx
        push   1
        mov    ecx, esi
        mov    dword ptr [ebp - 4], 0x100
        push   0                             ; -> obj7:0x0E10
        call   sub_5_87D6
        test   eax, eax
        jne    loc_5_C69F
        mov    ebx, 1
        push   2
        push   esi
        lea    eax, [ebp - 0x104]
        push   eax
        call   sub_5_5854
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_C708
loc_5_C69F:
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0x104]
        push   ecx
        xor    edx, edx
        push   1
        mov    ecx, esi
        mov    dword ptr [ebp - 4], 0x100
        push   0                             ; -> obj7:0x0E20
        call   sub_5_87D6
        test   eax, eax
        jne    loc_5_C6E2
        mov    edi, 1
        push   0
        push   esi
        lea    eax, [ebp - 0x104]
        push   eax
        call   sub_5_5854
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_C708
loc_5_C6E2:
        test   edi, edi
        jne    loc_5_C6F3
        push   0
        push   0
        push   esi
        call   _CONFIGMG_Register_Enumerator
        add    esp, 0xc
loc_5_C6F3:
        test   ebx, ebx
        jne    loc_5_C706
        push   4
        push   0
        push   0
        push   esi
        call   sub_5_1F16
        add    esp, 0x10
loc_5_C706:
        xor    eax, eax
loc_5_C708:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_C70F:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     loc_5_C723
        cmp    eax, 1
        je     loc_5_C730
        mov    eax, 1
        jmp    loc_5_C739
loc_5_C723:
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_C59F
        xor    eax, eax
        jmp    loc_5_C739
loc_5_C730:
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_C5D8
loc_5_C739:
        ret    8
sub_5_C73C:
        xor    ax, ax
        push   esi
        xor    esi, esi
        cmp    word ptr [0], si              ; -> obj7:0x0098
        je     loc_5_C77B
        cmp    dword ptr [0], esi            ; -> obj7:0x0E38
        jne    loc_5_C77B
        push   0                             ; -> obj7:0x0098
        mov    edx, 0                        ; -> obj7:0x0E3C
        push   2
        xor    ecx, ecx
        mov    dword ptr [0], 1              ; -> obj7:0x0E38
        push   0                             ; -> obj7:0x0E2C
        call   sub_5_1240
        mov    dword ptr [0], esi            ; -> obj7:0x0E38
loc_5_C77B:
        pop    esi
        ret    
sub_5_C77D:
        sub    esp, 4
        mov    word ptr [esp + 2], cx
        push   esi
        xor    esi, esi
        cmp    cx, si
        je     loc_5_C7BD
        cmp    dword ptr [0], esi            ; -> obj7:0x0E38
        jne    loc_5_C7BD
        lea    eax, [esp + 6]
        mov    edx, 0                        ; -> obj7:0x0E48
        push   eax
        xor    ecx, ecx
        mov    dword ptr [0], 1              ; -> obj7:0x0E38
        push   2
        push   0                             ; -> obj7:0x0E2C
        call   sub_5_1240
        mov    dword ptr [0], esi            ; -> obj7:0x0E38
loc_5_C7BD:
        pop    esi
        add    esp, 4
        ret    
sub_5_C7C2:
        mov    ecx, 0                        ; -> obj7:0x0E2C
        jmp    sub_5_1409
sub_5_C7CC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 0x122dd4
        mul    ecx
        mov    ecx, 0xf4240
        div    ecx
        mov    ecx, dword ptr [ebp - 4]
        add    dword ptr [ecx], eax
        adc    dword ptr [ecx + 4], 0
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_C7F6:
        xor    edx, edx
        VxDCall  VTD.VTD_Get_Real_Time           ; cd2007000500
        sub    eax, ecx
        test   eax, eax
        mov    eax, 1
        jg     loc_5_C80B
        xor    eax, eax
loc_5_C80B:
        ret    
sub_5_C80C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        xor    edx, edx
        VxDCall  VTD.VTD_Get_Real_Time           ; cd2007000500
        lea    ecx, [ebp - 4]
        mov    edx, esi
        mov    dword ptr [ebp - 4], eax
        call   sub_5_C7CC
        test   edi, edi
        je     loc_5_C837
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
loc_5_C837:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_C7F6
        test   eax, eax
        jne    loc_5_C859
loc_5_C843:
        test   edi, edi
        je     loc_5_C84D
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
loc_5_C84D:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_C7F6
        test   eax, eax
        je     loc_5_C843
loc_5_C859:
        test   edi, edi
        je     loc_5_C863
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
loc_5_C863:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_C869:
        mov    eax, dword ptr [ecx + 0x24]
        test   eax, eax
        jne    loc_5_C874
        xor    eax, eax
        jmp    loc_5_C883
loc_5_C874:
        cmp    dword ptr [eax + 0x28], 0
        je     loc_5_C883
loc_5_C87A:
        mov    eax, dword ptr [eax + 0x28]
        cmp    dword ptr [eax + 0x28], 0
        jne    loc_5_C87A
loc_5_C883:
        ret    
sub_5_C884:
        mov    eax, dword ptr [ecx + 0x20]
        test   eax, eax
        jne    loc_5_C88F
        xor    eax, eax
        jmp    loc_5_C8A7
loc_5_C88F:
        mov    eax, dword ptr [eax + 0x24]
        cmp    eax, ecx
        jne    loc_5_C89A
        xor    eax, eax
        jmp    loc_5_C8A7
loc_5_C89A:
        cmp    dword ptr [eax + 0x28], ecx
        je     loc_5_C8A7
loc_5_C89F:
        mov    eax, dword ptr [eax + 0x28]
        cmp    dword ptr [eax + 0x28], ecx
        jne    loc_5_C89F
loc_5_C8A7:
        ret    
sub_5_C8A8:
        push   esi
        cmp    ecx, dword ptr [0]            ; -> loc_7_0E60
        mov    esi, ecx
        jne    loc_5_C8B7
        xor    eax, eax
        jmp    loc_5_C8CB
loc_5_C8B7:
        push   esi
        call   sub_7_0E54
        add    esp, 4
        test   eax, eax
        jne    loc_5_C8CB
        mov    dword ptr [0], esi            ; -> loc_7_0E58
loc_5_C8CB:
        pop    esi
        ret    
loc_5_C8CD:
        push   esi
        mov    esi, dword ptr [esp + 8]
loc_5_C8D2:
        test   esi, esi
        je     loc_5_C8E8
        mov    ecx, esi
        mov    esi, dword ptr [esi + 0x20]
        call   sub_5_C8A8
        test   eax, eax
        jne    loc_5_C8D2
        xor    eax, eax
        jmp    loc_5_C8ED
loc_5_C8E8:
        mov    eax, 1
loc_5_C8ED:
        pop    esi
        ret    
sub_5_C8EF:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   esi, esi
        jne    loc_5_C8FF
        mov    eax, 1
        jmp    loc_5_C91A
loc_5_C8FF:
        push   dword ptr [esi + 0x20]
        call   sub_5_C8EF
        add    esp, 4
        test   eax, eax
        mov    eax, 0
        je     loc_5_C91A
        mov    ecx, esi
        call   sub_5_C8A8
loc_5_C91A:
        pop    esi
        ret    
sub_5_C91C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        test   esi, esi
        je     loc_5_C97A
        push   dword ptr [esi + 0x24]
        call   sub_5_C91C
        add    esp, 4
        test   eax, eax
        jne    loc_5_C939
        xor    eax, eax
        jmp    loc_5_C97F
loc_5_C939:
        mov    edi, dword ptr [esi + 0x28]
        mov    ecx, esi
        call   sub_5_C8A8
        test   eax, eax
        jne    loc_5_C94B
        xor    eax, eax
        jmp    loc_5_C97F
loc_5_C94B:
        cmp    dword ptr [0], esi            ; -> loc_7_0E5C
        je     loc_5_C97A
        test   edi, edi
        je     loc_5_C97A
loc_5_C957:
        push   dword ptr [edi + 0x24]
        mov    esi, dword ptr [edi + 0x28]
        call   sub_5_C91C
        add    esp, 4
        test   eax, eax
        je     loc_5_C982
        mov    ecx, edi
        call   sub_5_C8A8
        test   eax, eax
        je     loc_5_C986
        mov    edi, esi
        test   esi, esi
        jne    loc_5_C957
loc_5_C97A:
        mov    eax, 1
loc_5_C97F:
        pop    edi
        pop    esi
        ret    
loc_5_C982:
        xor    eax, eax
        jmp    loc_5_C97F
loc_5_C986:
        xor    eax, eax
        jmp    loc_5_C97F
sub_5_C98A:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        test   esi, esi
        je     loc_5_CA08
        cmp    esi, dword ptr [0]            ; -> loc_7_0E5C
        jne    loc_5_C9C0
        mov    ecx, esi
        call   sub_5_C8A8
        test   eax, eax
        jne    loc_5_C9AC
        xor    eax, eax
        jmp    loc_5_CA0D
loc_5_C9AC:
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     loc_5_CA08
        mov    esi, dword ptr [esi + 0x24]
        test   esi, esi
        je     loc_5_CA08
loc_5_C9C0:
        mov    ebx, dword ptr [esi + 0x20]
        mov    edi, dword ptr [esi + 0x28]
        mov    ecx, esi
        call   sub_5_C8A8
        test   eax, eax
        jne    loc_5_C9D5
        xor    eax, eax
        jmp    loc_5_CA0D
loc_5_C9D5:
        test   edi, edi
        je     loc_5_C9EB
        mov    ecx, edi
        mov    edi, dword ptr [edi + 0x28]
        call   sub_5_C8A8
        test   eax, eax
        jne    loc_5_C9D5
        xor    eax, eax
        jmp    loc_5_CA0D
loc_5_C9EB:
        mov    esi, dword ptr [ebx + 0x24]
        test   esi, esi
        je     loc_5_CA08
loc_5_C9F2:
        push   dword ptr [esi + 0x24]
        call   sub_5_C98A
        add    esp, 4
        test   eax, eax
        je     loc_5_CA11
        mov    esi, dword ptr [esi + 0x28]
        test   esi, esi
        jne    loc_5_C9F2
loc_5_CA08:
        mov    eax, 1
loc_5_CA0D:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_5_CA11:
        xor    eax, eax
        jmp    loc_5_CA0D
sub_5_CA15:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    esi, dword ptr [0]            ; -> loc_7_0E60
        jne    loc_5_CA29
        mov    eax, 1
        jmp    loc_5_CA44
loc_5_CA29:
        push   dword ptr [esi + 0x20]
        call   sub_5_CA15
        add    esp, 4
        test   eax, eax
        mov    eax, 0
        je     loc_5_CA44
        mov    ecx, esi
        call   sub_5_C8A8
loc_5_CA44:
        pop    esi
        ret    
loc_5_CA46:
        push   esi
        mov    eax, dword ptr [0]            ; -> loc_7_0E60
        mov    esi, dword ptr [eax + 0x20]
loc_5_CA4F:
        test   esi, esi
        je     loc_5_CA65
        mov    ecx, esi
        mov    esi, dword ptr [esi + 0x20]
        call   sub_5_C8A8
        test   eax, eax
        jne    loc_5_CA4F
        xor    eax, eax
        jmp    loc_5_CA6A
loc_5_CA65:
        mov    eax, 1
loc_5_CA6A:
        pop    esi
        ret    
sub_5_CA6C:
        push   esi
        mov    esi, ecx
        test   esi, esi
        je     loc_5_CAAF
        cmp    dword ptr [0], esi            ; -> loc_7_0E60
        je     loc_5_CA8A
        mov    ecx, esi
        call   sub_5_C8A8
        test   eax, eax
        jne    loc_5_CA8A
        xor    eax, eax
        jmp    loc_5_CAB4
loc_5_CA8A:
        mov    ecx, esi
        call   sub_5_C869
        mov    esi, eax
        test   esi, esi
        je     loc_5_CAAF
loc_5_CA97:
        mov    ecx, esi
        call   sub_5_CA6C
        test   eax, eax
        je     loc_5_CAB6
        mov    ecx, esi
        call   sub_5_C884
        mov    esi, eax
        test   esi, esi
        jne    loc_5_CA97
loc_5_CAAF:
        mov    eax, 1
loc_5_CAB4:
        pop    esi
        ret    
loc_5_CAB6:
        xor    eax, eax
        jmp    loc_5_CAB4
loc_5_CABA:
        push   esi
        push   edi
        mov    esi, dword ptr [0]            ; -> loc_7_0E60
        mov    ecx, esi
        call   sub_5_CA6C
        test   eax, eax
        jne    loc_5_CAD1
        xor    eax, eax
        jmp    loc_5_CB03
loc_5_CAD1:
        cmp    esi, dword ptr [esp + 0xc]
        je     loc_5_CAFE
loc_5_CAD7:
        mov    edi, dword ptr [esi + 0x20]
loc_5_CADA:
        mov    ecx, esi
        call   sub_5_C884
        mov    esi, eax
        test   esi, esi
        je     loc_5_CAF6
        mov    ecx, esi
        call   sub_5_CA6C
        test   eax, eax
        jne    loc_5_CADA
        xor    eax, eax
        jmp    loc_5_CB03
loc_5_CAF6:
        mov    esi, edi
        cmp    edi, dword ptr [esp + 0xc]
        jne    loc_5_CAD7
loc_5_CAFE:
        mov    eax, 1
loc_5_CB03:
        pop    edi
        pop    esi
        ret    
loc_5_CB06:
        push   esi
        push   edi
        mov    esi, dword ptr [0]            ; -> loc_7_0E60
loc_5_CB0E:
        mov    edi, dword ptr [esi + 0x20]
        test   esi, esi
        je     loc_5_CB3B
loc_5_CB15:
        cmp    dword ptr [0], esi            ; -> loc_7_0E60
        je     loc_5_CB28
        mov    ecx, esi
        call   sub_5_C8A8
        test   eax, eax
        je     loc_5_CB4E
loc_5_CB28:
        cmp    esi, dword ptr [esp + 0xc]
        je     loc_5_CB52
        mov    ecx, esi
        call   sub_5_C884
        mov    esi, eax
        test   esi, esi
        jne    loc_5_CB15
loc_5_CB3B:
        cmp    dword ptr [edi + 0x28], 0
        mov    esi, edi
        je     loc_5_CB0E
loc_5_CB43:
        mov    esi, dword ptr [esi + 0x28]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_5_CB43
        jmp    loc_5_CB0E
loc_5_CB4E:
        xor    eax, eax
        jmp    loc_5_CB57
loc_5_CB52:
        mov    eax, 1
loc_5_CB57:
        pop    edi
        pop    esi
        ret    
sub_5_CB5A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0]            ; -> sub_7_0E54
        mov    edi, dword ptr [0]            ; -> loc_7_0E58
        mov    ebx, dword ptr [0]            ; -> loc_7_0E60
        mov    eax, dword ptr [0]            ; -> loc_7_0E5C
        mov    dword ptr [ebp - 8], ebx
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        push   edx
        mov    dword ptr [0], ebx            ; -> sub_7_0E54
        mov    dword ptr [0], edx            ; -> loc_7_0E5C
        mov    dword ptr [0], 0              ; -> loc_7_0E58
        mov    dword ptr [0], eax            ; -> loc_7_0E60
        call   sub_7_0E64
        add    esp, 4
        mov    edx, dword ptr [ebp + 0xc]
        test   edx, edx
        je     loc_5_CBBB
        mov    ecx, dword ptr [0]            ; -> loc_7_0E58
        mov    dword ptr [edx], ecx
loc_5_CBBB:
        mov    ebx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [0], esi            ; -> sub_7_0E54
        mov    dword ptr [0], edi            ; -> loc_7_0E58
        pop    edi
        mov    dword ptr [0], ebx            ; -> loc_7_0E60
        pop    esi
        mov    dword ptr [0], ecx            ; -> loc_7_0E5C
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_5_CBE2:
        push   dword ptr [esp + 4]
        push   0
        push   0
        call   sub_5_CB5A
        ret    4
sub_5_CBF2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        lea    ebx, [ebp - 4]
        push   dword ptr [ebp + 8]
        push   ebx
        push   0
        call   sub_5_CB5A
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_CC25
        lea    ecx, [esi + 4]
        mov    edx, edi
        push   dword ptr [ebp + 0xc]
        push   0
        push   dword ptr [ebp - 4]
        call   sub_5_CB5A
loc_5_CC25:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_CC30:
        push   edx
        cmp    ecx, 1
        sbb    ecx, ecx
        mov    edx, dword ptr [0]            ; -> obj7:0x00A8
        add    ecx, 3
        call   sub_5_CBE2
        ret    
loc_5_CC45:
        push   esi
        mov    ecx, dword ptr [0]            ; -> loc_7_0E94
        mov    esi, dword ptr [esp + 8]
        cmp    ecx, esi
        je     loc_5_CC75
        push   0
        mov    edx, dword ptr [0]            ; -> loc_7_0E88
        push   ecx
        push   esi
        mov    ecx, dword ptr [0]            ; -> loc_7_0E84
        call   sub_5_1E4B
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        jmp    loc_5_CD45
loc_5_CC75:
        mov    eax, dword ptr [0]            ; -> loc_7_0E98
        cmp    eax, 1
        je     loc_5_CC89
        cmp    eax, 4
        mov    eax, 0
        jne    loc_5_CC8E
loc_5_CC89:
        mov    eax, 1
loc_5_CC8E:
        push   eax
        mov    edx, dword ptr [0]            ; -> loc_7_0E88
        push   ecx
        push   esi
        mov    ecx, dword ptr [0]            ; -> loc_7_0E84
        call   sub_5_1E4B
        test   eax, eax
        jne    loc_5_CD43
        mov    eax, dword ptr [0]            ; -> loc_7_0E98
        test   eax, eax
        je     loc_5_CCBD
        cmp    eax, 5
        mov    eax, 0
        jne    loc_5_CCC2
loc_5_CCBD:
        mov    eax, 1
loc_5_CCC2:
        push   eax
        mov    edx, dword ptr [0]            ; -> loc_7_0E88
        push   dword ptr [0]                 ; -> loc_7_0E94
        push   esi
        mov    ecx, dword ptr [0]            ; -> loc_7_0E84
        call   sub_5_1E4B
        test   eax, eax
        jne    loc_5_CCE6
        mov    eax, 1
        jmp    loc_5_CD45
loc_5_CCE6:
        mov    ecx, dword ptr [0]            ; -> loc_7_0E8C
        cmp    ecx, -1
        je     loc_5_CD43
        mov    eax, dword ptr [0]            ; -> loc_7_0E98
        cmp    eax, 1
        je     loc_5_CD05
        cmp    eax, 4
        mov    eax, 0
        jne    loc_5_CD0A
loc_5_CD05:
        mov    eax, 1
loc_5_CD0A:
        push   eax
        mov    edx, dword ptr [0]            ; -> loc_7_0E90
        push   dword ptr [0]                 ; -> loc_7_0E94
        push   esi
        call   sub_5_1E4B
        test   eax, eax
        je     loc_5_CD43
        mov    edx, 0xb
        mov    ecx, dword ptr [0]            ; -> loc_7_0E94
        call   sub_5_731F
        push   0
        mov    ecx, dword ptr [0]            ; -> loc_7_0E94
        push   0x100
        call   sub_5_1906
loc_5_CD43:
        xor    eax, eax
loc_5_CD45:
        pop    esi
        ret    
loc_5_CD47:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0]            ; -> loc_7_0E94
        mov    edi, dword ptr [esp + 0x10]
        push   ebp
        cmp    esi, edi
        je     loc_5_CD77
        push   0
        mov    edx, dword ptr [0]            ; -> loc_7_0E88
        push   esi
        mov    ecx, dword ptr [0]            ; -> loc_7_0E84
        push   edi
        call   sub_5_1E4B
        test   eax, eax
        jne    loc_5_CDE2
        jmp    loc_5_CDFC
loc_5_CD77:
        mov    eax, dword ptr [0]            ; -> loc_7_0E98
        cmp    eax, 1
        je     loc_5_CD8B
        mov    ecx, 0
        cmp    eax, 4
        jne    loc_5_CD90
loc_5_CD8B:
        mov    ecx, 1
loc_5_CD90:
        test   eax, eax
        je     loc_5_CD9E
        mov    ebp, 0
        cmp    eax, 5
        jne    loc_5_CDA3
loc_5_CD9E:
        mov    ebp, 1
loc_5_CDA3:
        push   ecx
        mov    edx, dword ptr [0]            ; -> loc_7_0E88
        push   esi
        mov    ecx, dword ptr [0]            ; -> loc_7_0E84
        push   edi
        call   sub_5_1E4B
        push   ebp
        mov    edx, dword ptr [0]            ; -> loc_7_0E88
        push   esi
        mov    ecx, dword ptr [0]            ; -> loc_7_0E84
        push   edi
        cmp    eax, 1
        sbb    ebx, ebx
        call   sub_5_1E4B
        neg    ebx
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        test   eax, ebx
        mov    eax, 1
        jne    loc_5_CE01
loc_5_CDE2:
        mov    edx, 0xb
        mov    ecx, edi
        call   sub_5_731F
        push   0
        mov    ecx, edi
        push   0x100
        call   sub_5_1906
loc_5_CDFC:
        mov    eax, 1
loc_5_CE01:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_5_CE06:
        push   esi
        mov    ecx, dword ptr [0]            ; -> loc_7_0EA4
        mov    esi, dword ptr [esp + 8]
        cmp    esi, ecx
        je     loc_5_CE32
        push   0
        mov    edx, dword ptr [0]            ; -> loc_7_0EA0
        push   ecx
        push   esi
        mov    ecx, dword ptr [0]            ; -> loc_7_0E9C
        call   sub_5_1E4B
        test   eax, eax
        jne    loc_5_CEA1
        xor    eax, eax
        jmp    loc_5_CEA6
loc_5_CE32:
        mov    eax, dword ptr [0]            ; -> loc_7_0EA8
        cmp    eax, 1
        je     loc_5_CE46
        cmp    eax, 4
        mov    eax, 0
        jne    loc_5_CE4B
loc_5_CE46:
        mov    eax, 1
loc_5_CE4B:
        push   eax
        mov    edx, dword ptr [0]            ; -> loc_7_0EA0
        push   ecx
        push   esi
        mov    ecx, dword ptr [0]            ; -> loc_7_0E9C
        call   sub_5_1E4B
        test   eax, eax
        jne    loc_5_CE67
        xor    eax, eax
        jmp    loc_5_CEA6
loc_5_CE67:
        mov    eax, dword ptr [0]            ; -> loc_7_0EA8
        test   eax, eax
        je     loc_5_CE7A
        cmp    eax, 5
        mov    eax, 0
        jne    loc_5_CE7F
loc_5_CE7A:
        mov    eax, 1
loc_5_CE7F:
        push   eax
        mov    edx, dword ptr [0]            ; -> loc_7_0EA0
        push   dword ptr [0]                 ; -> loc_7_0EA4
        push   esi
        mov    ecx, dword ptr [0]            ; -> loc_7_0E9C
        call   sub_5_1E4B
        test   eax, eax
        mov    eax, 0
        je     loc_5_CEA6
loc_5_CEA1:
        mov    eax, 1
loc_5_CEA6:
        pop    esi
        ret    
sub_5_CEA8:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_7_0E84
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    dword ptr [ebp - 4], edx
        push   esi
        mov    ebx, dword ptr [0]            ; -> loc_7_0E8C
        push   edi
        mov    esi, dword ptr [0]            ; -> loc_7_0E88
        mov    edi, dword ptr [0]            ; -> loc_7_0E94
        mov    edx, dword ptr [0]            ; -> loc_7_0E98
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0xc], edx
        mov    eax, dword ptr [0]            ; -> loc_7_0E90
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [0], edx            ; -> loc_7_0E88
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [0], eax            ; -> loc_7_0E84
        mov    dword ptr [0], edx            ; -> loc_7_0E90
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [0], ecx            ; -> loc_7_0E98
        cmp    dword ptr [ebp + 0x18], 0
        mov    dword ptr [0], eax            ; -> loc_7_0E8C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0], eax            ; -> loc_7_0E94
        mov    eax, 0                        ; -> loc_5_CC45
        jne    loc_5_CF1F
        mov    eax, 0                        ; -> loc_5_CD47
loc_5_CF1F:
        push   eax
        mov    edx, dword ptr [ebp - 4]
        lea    eax, [ebp - 0x18]
        push   eax
        push   0
        call   sub_5_CB5A
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_5_CF6B
        cmp    dword ptr [ebp + 0x10], -1
        je     loc_5_CF6B
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        push   0                             ; -> loc_5_CD47
        mov    edx, dword ptr [ebp - 4]
        push   0
        mov    dword ptr [0], eax            ; -> loc_7_0E84
        push   dword ptr [ebp - 0x18]
        add    dword ptr [0], 4              ; -> loc_7_0E98
        mov    dword ptr [0], ecx            ; -> loc_7_0E88
        mov    ecx, dword ptr [0]            ; -> loc_7_0E98
        call   sub_5_CB5A
loc_5_CF6B:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [0], eax            ; -> loc_7_0E84
        mov    dword ptr [0], esi            ; -> loc_7_0E88
        mov    edx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [0], ebx            ; -> loc_7_0E8C
        mov    dword ptr [0], ecx            ; -> loc_7_0E90
        mov    dword ptr [0], edi            ; -> loc_7_0E94
        mov    dword ptr [0], edx            ; -> loc_7_0E98
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
sub_5_CFA3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0]            ; -> loc_7_0E9C
        mov    edi, dword ptr [0]            ; -> loc_7_0EA0
        mov    ebx, dword ptr [0]            ; -> loc_7_0EA4
        mov    eax, dword ptr [0]            ; -> loc_7_0EA8
        mov    dword ptr [ebp - 8], ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   0                             ; -> loc_5_CE06
        push   0
        mov    dword ptr [0], ebx            ; -> loc_7_0E9C
        push   0
        mov    dword ptr [0], eax            ; -> loc_7_0EA0
        mov    dword ptr [0], edx            ; -> loc_7_0EA4
        mov    dword ptr [0], ecx            ; -> loc_7_0EA8
        call   sub_5_CB5A
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [0], esi            ; -> loc_7_0E9C
        mov    dword ptr [0], edi            ; -> loc_7_0EA0
        pop    edi
        mov    dword ptr [0], ecx            ; -> loc_7_0EA4
        pop    esi
        mov    dword ptr [0], edx            ; -> loc_7_0EA8
        pop    ebx
        cmp    eax, 1
        sbb    eax, eax
        mov    esp, ebp
        neg    eax
        pop    ebp
        ret    8
        db  0CCh, 0CCh
sub_5_D024:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_D034:
        VMMCall  VMM._PageFree                   ; cd2055800100
        int3   
        int3   
sub_5_D03C:
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
sub_5_D050:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_D060:
        VMMCall  VMM._Call_On_My_Stack           ; cd208c810100
        int3   
        int3   
sub_5_D068:
        VMMCall  VMM._PageAllocate               ; cd2053800100
        int3   
        int3   
sub_5_D070:
        pop    edx
        pop    eax
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        jmp    edx
        db  0CCh, 0CCh
sub_5_D07C:
        VxDCall  SHELL._SHELL_BroadcastSystemMessage ; cd2010801700
        int3   
        int3   
sub_5_D084:
        pop    edx
        pop    eax
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        jmp    edx
        db  0CCh, 0CCh
sub_5_D090:
        pop    edx
        pop    eax
        pop    ecx
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        jmp    edx
        db  0CCh
sub_5_D09C:
        pop    edx
        pop    ecx
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_5_D0AC:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  0CCh
sub_5_D0B8:
        VxDCall  SHELL._SHELL_QueryAppyTimeAvailable ; cd200d801700
        int3   
        int3   
sub_5_D0C0:
        VMMCall  VMM._HeapReAllocate             ; cd2050800100
        int3   
        int3   
sub_5_D0C8:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_5_D0D0:
        pop    edx
        pop    esi
        pop    eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be800100
        int3   
        int3   
        int3   
sub_5_D0DC:
        VMMCall  VMM._RegFlushKey                ; cd2055810100
        int3   
        int3   
sub_5_D0E4:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_5_D0EC:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        int3   
        int3   
sub_5_D0F4:
        VMMCall  VMM._GetRegistryKey             ; cd206f810100
        int3   
        int3   
sub_5_D0FC:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_5_D104:
        VxDCall  SHELL._SHELL_CallDll            ; cd2018801700
        int3   
        int3   
sub_5_D10C:
        VxDCall  SHELL._SHELL_GetProcAddress     ; cd2017801700
        int3   
        int3   
sub_5_D114:
        VxDCall  SHELL._SHELL_LoadLibrary        ; cd2015801700
        int3   
        int3   
sub_5_D11C:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_5_D124:
        VxDCall  SHELL._SHELL_FreeLibrary        ; cd2016801700
        int3   
        int3   
sub_5_D12C:
        VMMCall  VMM._Assert_Range               ; cd2073810100
        int3   
        int3   
sub_5_D134:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_5_D13C:
        VMMCall  VMM.Service_8196h               ; cd2096810100
        int3   
        int3   
sub_5_D144:
        VMMCall  VMM._RegCreateDynKey            ; cd2076810100
        int3   
        int3   
sub_5_D14C:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        int3   
        int3   
sub_5_D154:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        int3   
        int3   
sub_5_D15C:
        VMMCall  VMM._RegRemapPreDefKey          ; cd2071810100
        int3   
        int3   
sub_5_D164:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        int3   
        int3   
sub_5_D16C:
        VMMCall  VMM._RegEnumKey                 ; cd204c810100
        int3   
        int3   
sub_5_D174:
        VMMCall  VMM.Get_Boot_Flags              ; cd2079810100
        int3   
        int3   
sub_5_D17C:
        VxDCall  APM.Service_8005h               ; cd2005802700
        int3   
        int3   
sub_5_D184:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x24]
        mov    esi, dword ptr [esp + 0x20]
        mov    edx, dword ptr [esp + 0x1c]
        mov    ebx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x10]
        VMMCall  VMM.Directed_Sys_Control        ; cd2047010100
        sbb    ebx, ebx
        and    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    0x18
        db  0CCh
sub_5_D1B0:
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        movzx  eax, word ptr [esp + 8]
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        mov    eax, ecx
        pop    edi
        ret    8
        db  0CCh, 0CCh
sub_5_D1C8:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_5_D1E6
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_D1E6:
        ret    0x10
        db  0CCh, 0CCh, 0CCh
sub_5_D1EC:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        VxDCall  APM.Service_0002h               ; cd2002002700
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_5_D200:
        VxDCall  SHELL._SHELL_LocalFree          ; cd2014801700
        int3   
        int3   
sub_5_D208:
        VxDCall  SHELL._SHELL_LocalAllocEx       ; cd2013801700
        int3   
        int3   
sub_5_D210:
        push   ebx
        push   edi
        mov    ebx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 0x10]
        or     eax, 0x1000
        mov    ecx, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x18]
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        pop    edi
        pop    ebx
        ret    0x10
        db  0CCh, 0CCh
sub_5_D234:
        VMMCall  VMM._RegEnumValue               ; cd2050810100
        int3   
        int3   
sub_5_D23C:
        VxDCall  VPEND.Service_800Bh             ; cd200b802600
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x37d  R-X
        db  000h, 000h, 000h, 000h, 043h, 04Fh, 04Eh, 046h, 049h, 047h, 024h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0F7h, 0C3h, 001h, 000h, 075h, 018h, 03Dh, 000h, 004h, 072h, 013h
        db  066h, 089h, 00Eh, 000h, 000h, 0E8h, 010h, 000h, 066h, 08Bh, 0D0h, 0B8h, 000h, 000h, 033h, 0DBh
        db  033h, 0F6h, 0C3h, 0B8h, 001h, 080h, 0EBh, 0F6h, 08Dh, 016h, 004h, 000h, 0B8h, 000h, 03Dh, 0CDh
        db  021h, 00Fh, 082h, 0F4h, 000h, 0A3h, 00Ch, 000h, 08Bh, 0D8h, 0B9h, 005h, 000h, 08Dh, 016h, 00Eh
        db  000h, 0B8h, 002h, 044h, 0CDh, 021h, 00Fh, 082h, 0DFh, 000h, 083h, 0F8h, 005h, 00Fh, 085h, 0D8h
        db  000h, 08Bh, 01Eh, 00Ch, 000h, 0B4h, 03Eh, 0CDh, 021h, 00Fh, 082h, 0CCh, 000h, 0A0h, 012h, 000h
        db  0A8h, 001h, 00Fh, 084h, 0C3h, 000h, 0A8h, 002h, 075h, 01Ah, 01Eh, 01Eh, 007h, 0BFh, 00Eh, 000h
        db  0C5h, 036h, 00Eh, 000h, 0B9h, 02Ch, 002h, 0FCh, 0F3h, 0A4h, 0C7h, 084h, 0EEh, 0FDh, 0FFh, 0FFh
        db  01Fh, 0E9h, 083h, 000h, 0B8h, 010h, 043h, 0CDh, 02Fh, 08Ch, 0C0h, 00Bh, 0C3h, 00Fh, 084h, 098h
        db  000h, 089h, 01Eh, 04Fh, 002h, 08Ch, 006h, 051h, 002h, 066h, 0C7h, 006h, 03Fh, 002h, 02Ch, 002h
        db  000h, 000h, 0A1h, 010h, 000h, 0A3h, 053h, 002h, 0A3h, 043h, 002h, 033h, 0C0h, 0A3h, 045h, 002h
        db  0A3h, 047h, 002h, 0A3h, 049h, 002h, 0C7h, 006h, 04Bh, 002h, 00Eh, 000h, 08Ch, 01Eh, 04Dh, 002h
        db  0B4h, 00Bh, 08Dh, 036h, 03Fh, 002h, 0FFh, 01Eh, 04Fh, 002h, 00Bh, 0C0h, 074h, 05Bh, 0FFh, 036h
        db  028h, 000h, 0C7h, 006h, 028h, 000h, 0FFh, 0FFh, 033h, 0C0h, 0A3h, 043h, 002h, 0C7h, 006h, 045h
        db  002h, 00Eh, 000h, 08Ch, 01Eh, 047h, 002h, 08Bh, 01Eh, 053h, 002h, 089h, 01Eh, 049h, 002h, 0A3h
        db  04Bh, 002h, 0A3h, 04Dh, 002h, 0B4h, 00Bh, 08Dh, 036h, 03Fh, 002h, 0FFh, 01Eh, 04Fh, 002h, 08Fh
        db  006h, 028h, 000h, 00Bh, 0C0h, 074h, 022h, 081h, 03Eh, 00Eh, 000h, 044h, 057h, 075h, 01Ah, 081h
        db  03Eh, 010h, 000h, 043h, 04Dh, 075h, 012h, 0B8h, 006h, 000h, 0B9h, 02Ch, 001h, 0BAh, 001h, 000h
        db  0BEh, 00Eh, 000h, 0FFh, 01Eh, 000h, 000h, 073h, 003h, 066h, 033h, 0C0h, 0C3h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 7  vsize=0xeac  RW-
        db  000h, 000h, 000h, 000h, 044h, 065h, 076h, 069h, 063h, 065h, 044h, 065h, 073h, 063h, 000h, 000h
        db  049h, 04Fh, 053h, 000h, 04Eh, 06Fh, 049h, 044h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  044h, 04Dh, 052h, 065h, 06Dh, 06Fh, 076h, 065h, 052h, 06Fh, 06Dh, 04Fh, 06Bh, 061h, 079h, 000h
        db  052h, 065h, 06Dh, 06Fh, 076h, 065h, 052h, 06Fh, 06Dh, 04Fh, 06Bh, 061h, 079h, 000h, 000h, 000h
        db  043h, 06Fh, 06Eh, 066h, 069h, 067h, 046h, 06Ch, 061h, 067h, 073h, 000h, 000h, 000h, 000h, 000h
        db  053h, 059h, 053h, 044h, 04Dh, 02Eh, 043h, 050h, 04Ch, 000h, 000h, 000h, 055h, 053h, 045h, 052h
        db  02Eh, 045h, 058h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  043h, 04Fh, 04Eh, 046h, 049h, 047h, 04Dh, 047h, 000h, 000h, 000h, 000h, 046h, 075h, 06Eh, 063h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 024h, 025h, 0CFh, 0AEh, 029h, 0CFh, 011h
        db  097h, 0EAh, 000h, 0AAh, 000h, 034h, 031h, 09Dh, 060h, 077h, 04Eh, 09Bh, 096h, 031h, 0CFh, 011h
        db  097h, 0EAh, 000h, 0AAh, 000h, 034h, 031h, 09Dh
sub_7_01F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_01FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0200:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0204:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0208:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_020C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0210:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0214:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0218:
        dec    eax
        push   esp
        push   edx
        inc    ebp
        inc    ebp
        pop    esp
        push   edx
        inc    ebp
        push   ebx
        inc    ebp
        push   edx
        push   esi
        inc    ebp
        inc    esp
        pop    esp
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x61], cl
        jb     loc_7_0294
        ja     0x293
        jb     0x299
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
loc_7_0238:
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     loc_7_029E
        je     loc_7_02A8
        bound  ebp, qword ptr [ebp + 0x49]
        inc    esp
        jae    loc_7_0246
loc_7_0246:
        add    byte ptr [eax], al
loc_7_0248:
        sub    al, 0
        add    byte ptr [eax], al
loc_7_024C:
        dec    eax
        popal  
        jb     loc_7_02B4
        ja     0x2b3
        jb     0x2b9
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
loc_7_0258:
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     loc_7_02BE
        je     loc_7_02C8
        bound  ebp, qword ptr [ebp + 0x49]
        inc    esp
        jae    loc_7_0266
loc_7_0266:
        add    byte ptr [eax], al
loc_7_0268:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_026C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0270:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0294:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
loc_7_029E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_02A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_02B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
loc_7_02BE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_02C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_7_02E0:
        dec    eax
        popal  
        jb     0x348
        push   edi
        popal  
        jb     loc_7_034D
        dec    ebx
        jns    loc_7_02EC
loc_7_02EC:
        push   eax
        jb     0x35e
        bound  ebp, qword ptr [ebp + 0x6d]
        add    byte ptr [ebx + 0x74], dl
        popal  
        je     0x36e
        jae    loc_7_02FB
loc_7_02FB:
        add    byte ptr [ecx + 0x6c], al
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x74], sp
        imul   ebp, dword ptr [edi + 0x6e], 0x68430000
        imul   ebp, dword ptr [esp], 0x69530000
        bound  ebp, qword ptr [ecx + ebp*2 + 0x6e]
        add    byte ptr [bx + si + 0x61], dl
        jb     loc_7_0381
        outsb  dx, byte ptr [esi]
        je     loc_7_031F
loc_7_031F:
        add    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x20], 0x614d
        outsb  dx, byte ptr [esi]
        popal  
        jb     loc_7_038B
        inc    ebp
        outsb  dx, byte ptr [esi]
        jne    0x3a0
        add    byte ptr [eax + eax], bl
        add    byte ptr [0x5838], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_034D:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0381:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_038B:
        add    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x5c], 0
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
        add    byte ptr [0x5834], ah
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x72], al
        imul   esp, dword ptr [ebp + 0x6e], 0x4e796c64
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_7_04C0:
        inc    ebx
        push   ebx
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x46], 0x616c
        jae    loc_7_04CE
loc_7_04CE:
        add    byte ptr [eax], al
loc_7_04D0:
        push   ebx
        jns    loc_7_0546
        je     0x53a
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    loc_7_054C
        jb     loc_7_0541
        outsb  dx, byte ptr [esi]
        je     0x522
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_7_0555
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x545
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x560
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        dec    ecx
        inc    esp
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x44], 0x42
loc_7_04FC:
        inc    ebx
        jne    0x571
        jb     0x566
        outsb  dx, byte ptr [esi]
        je     0x547
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
loc_7_050C:
        inc    ebx
        jne    0x581
        jb     loc_7_0576
        outsb  dx, byte ptr [esi]
        je     loc_7_0557
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
loc_7_051C:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x5c], 0x5c00
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x46], 0x616c
        jae    loc_7_0534
loc_7_0534:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x46], 0x616c
        jae    loc_7_0540
loc_7_0540:
        pop    esp
loc_7_0541:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x79], dl
loc_7_0546:
        jae    loc_7_05BC
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
loc_7_054C:
        jne    loc_7_05C0
        jb     0x5b5
        outsb  dx, byte ptr [esi]
        je     0x596
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_7_0555:
        je     0x5c9
loc_7_0557:
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x5b9
        push   ebx
        jb     loc_7_05D7
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
        pop    esp
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x4d], 0x5c67
        push   ebx
        jo     loc_7_05D7
loc_7_0576:
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x42]
        jne    loc_7_05F2
        add    byte ptr [ebp + 0x6e], al
        jne    loc_7_05F1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0588:
        inc    esp
        dec    ebp
        inc    ecx
        jae    0x5f8
        inc    esi
        outsd  dx, dword ptr [esi]
        jb     0x5d4
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0x4100
        jae    0x606
        inc    esi
        outsd  dx, dword ptr [esi]
        jb     0x5e2
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
        add    byte ptr [eax], ah
        and    eax, 0x646c
        add    byte ptr [eax], al
loc_7_05B0:
        inc    esp
        dec    ebp
        push   edi
        popal  
        imul   esi, dword ptr [esi + eax*2 + 0x6f], 0x646e5572
loc_7_05BC:
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx], bp
loc_7_05C0:
        push   edi
        popal  
        imul   esi, dword ptr [esi + eax*2 + 0x6f], 0x646e5572
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx], bp
        add    byte ptr [eax], al
loc_7_05D0:
        push   ebx
        jns    loc_7_0646
        je     0x63a
        insd   dword ptr es:[edi], dx
        pop    esp
loc_7_05D7:
        inc    ebx
        jne    loc_7_064C
        jb     loc_7_0641
        outsb  dx, byte ptr [esi]
        je     0x622
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x655
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_7_0645
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x660
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
loc_7_05F1:
        dec    ecx
loc_7_05F2:
        inc    esp
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x44], 0x42
loc_7_05FC:
        dec    ebp
        popal  
        jo     loc_7_0600
loc_7_0600:
        dec    ebp
        popal  
        jo     loc_7_0604
loc_7_0604:
        inc    esi
        jb     loc_7_0670
        outsb  dx, byte ptr gs:[esi]
        insb   byte ptr es:[edi], dx
        jns    0x65b
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_7_0614:
        inc    ebp
        outsb  dx, byte ptr [esi]
        jne    0x685
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x53], al
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x46], 0x616c
        jae    loc_7_062A
loc_7_062A:
        add    byte ptr [eax], al
loc_7_062C:
        push   ebx
        jns    0x6a2
        je     0x696
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x6a8
        jb     loc_7_069D
        outsb  dx, byte ptr [esi]
        je     loc_7_067E
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_7_06B1
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_7_0641:
        push   ebx
        je     0x6a1
loc_7_0645:
        inc    ebx
loc_7_0646:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x6bc
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_7_064C:
        pop    esp
        dec    ecx
        inc    esp
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x44], 0x42
loc_7_0658:
        dec    ebp
        popal  
        jo     loc_7_065C
loc_7_065C:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0xa00
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x79], dl
        jae    loc_7_06E0
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
loc_7_0670:
        jne    loc_7_06E4
        jb     loc_7_06D9
        outsb  dx, byte ptr [esi]
        je     0x6ba
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x6ed
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
loc_7_067E:
        je     loc_7_06DD
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_7_06F8
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        dec    ecx
        inc    esp
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x44], 0x42
loc_7_0694:
        inc    ebx
        jne    loc_7_0709
        jb     0x6fe
        outsb  dx, byte ptr [esi]
        je     loc_7_06DF
        outsd  dx, dword ptr [esi]
loc_7_069D:
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
loc_7_06A4:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x5c], 0x5c00
        inc    ebp
        outsb  dx, byte ptr [esi]
        jne    loc_7_071E
loc_7_06B1:
        pop    esp
        add    byte ptr [eax], al
loc_7_06B4:
        inc    ebx
        push   ebx
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x46], 0x616c
        jae    loc_7_06C2
loc_7_06C2:
        add    byte ptr [eax], al
loc_7_06C4:
        push   ebx
        jns    0x73a
        je     loc_7_072E
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x740
        jb     0x735
        outsb  dx, byte ptr [esi]
        je     0x716
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x749
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_7_06D9:
        push   ebx
        je     loc_7_0739
loc_7_06DD:
        inc    ebx
        outsd  dx, dword ptr [esi]
loc_7_06DF:
        outsb  dx, byte ptr [esi]
loc_7_06E0:
        je     0x754
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_7_06E4:
        pop    esp
        dec    ecx
        inc    esp
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x44], 0x42
loc_7_06F0:
        inc    ebx
        jne    0x765
        jb     loc_7_075A
        outsb  dx, byte ptr [esi]
        je     loc_7_073B
loc_7_06F8:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
loc_7_0700:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x5c], 0x5c00
loc_7_0709:
        inc    ebp
        outsb  dx, byte ptr [esi]
        jne    loc_7_077A
        pop    esp
        add    byte ptr [eax], al
loc_7_0710:
        inc    ebx
        push   ebx
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x46], 0x616c
        jae    loc_7_071E
loc_7_071E:
        add    byte ptr [eax], al
loc_7_0720:
        inc    ebx
        push   ebx
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x46], 0x616c
        jae    loc_7_072E
loc_7_072E:
        add    byte ptr [eax], al
loc_7_0730:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x5c], 0x5000
loc_7_0739:
        jb     0x7aa
loc_7_073B:
        imul   bp, word ptr [ebp + 0x46], 0x616c
        jae    loc_7_0745
loc_7_0745:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x5c], 0x5000
        jb     0x7c2
        imul   bp, word ptr [ebp + 0x46], 0x616c
loc_7_075A:
        jae    loc_7_075D
loc_7_075D:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x79], dl
        jae    loc_7_07D8
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    loc_7_07DC
        jb     loc_7_07D1
        outsb  dx, byte ptr [esi]
        je     loc_7_07B2
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_7_07E5
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x7d5
        inc    ebx
loc_7_077A:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_7_07F0
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        dec    ecx
        inc    esp
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x44], 0x42
loc_7_078C:
        dec    ebp
        popal  
        jo     loc_7_0790
loc_7_0790:
        inc    ebx
        jne    loc_7_0805
        jb     loc_7_07FA
        outsb  dx, byte ptr [esi]
        je     loc_7_07DB
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
loc_7_07A0:
        dec    ebp
        popal  
        jo     loc_7_07A4
loc_7_07A4:
        inc    esi
        jb     0x810
        outsb  dx, byte ptr gs:[esi]
        insb   byte ptr es:[edi], dx
        jns    0x7fb
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
loc_7_07B2:
        add    byte ptr [eax], al
loc_7_07B4:
        inc    ebx
        jne    0x829
        jb     0x81e
        outsb  dx, byte ptr [esi]
        je     loc_7_07FF
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
loc_7_07C4:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x5c], 0x5300
        jns    0x842
        je     0x836
loc_7_07D1:
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x848
        jb     loc_7_083D
loc_7_07D8:
        outsb  dx, byte ptr [esi]
        je     0x81e
loc_7_07DB:
        outsd  dx, dword ptr [esi]
loc_7_07DC:
        outsb  dx, byte ptr [esi]
        je     loc_7_0851
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_7_0841
loc_7_07E5:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x85c
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        dec    ecx
        inc    esp
        inc    ebx
loc_7_07F0:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x44], 0x42
loc_7_07F8:
        dec    ebp
        popal  
loc_7_07FA:
        jo     loc_7_07FC
loc_7_07FC:
        inc    ebx
        jne    0x871
loc_7_07FF:
        jb     loc_7_0866
        outsb  dx, byte ptr [esi]
        je     loc_7_0847
        outsd  dx, dword ptr [esi]
loc_7_0805:
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
loc_7_080C:
        inc    esp
        dec    ebp
        inc    ecx
        jae    loc_7_087C
        inc    esi
        outsd  dx, dword ptr [esi]
        jb     0x858
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0x4100
        jae    loc_7_088A
        inc    esi
        outsd  dx, dword ptr [esi]
        jb     loc_7_0866
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_083D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0841:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0847:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0851:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
loc_7_0866:
        add    byte ptr [eax], al
        add    al, ah
        add    byte ptr [eax], al
        add    eax, esp
        add    byte ptr [eax], al
        add    ah, al
        add    byte ptr [eax], al
loc_7_0874:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0878:
        or     byte ptr [ebx], al
        add    byte ptr [eax], al
loc_7_087C:
        add    byte ptr [eax], al
        adc    byte ptr [0], al              ; -> loc_5_4988
        sbb    byte ptr [0], al              ; -> loc_5_49D0
loc_7_088A:
        add    byte ptr [ebx], 0             ; -> loc_5_4A9C
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    eax, dword ptr [eax]          ; -> loc_5_4AFA
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    eax, dword ptr [eax]          ; -> loc_5_4CF6
        add    byte ptr [eax], al
        add    byte ptr [eax + 8], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    eax
        add    al, 0                         ; -> loc_5_4FCB
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    dword ptr [eax], eax          ; -> loc_5_50B5
        add    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    dword ptr [eax], eax          ; -> loc_5_52DD
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     dword ptr [eax + eax], 0x85000000 ; -> loc_5_5299
        adc    al, 0                         ; -> loc_5_52BC
        add    byte ptr [eax], al
        add    byte ptr [esi + 0xc], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [0x32255832], ah
        pop    eax
        add    byte ptr [eax], al
loc_7_08D8:
        and    eax, 0x252e5831
        xor    dword ptr [eax + 0x2e], ebx
        and    eax, 0x583230
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x57], cl
        push   edx
        jbe    loc_7_0957
        jae    0x959
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_7_08F4:
        inc    esp
        jbe    loc_7_0961
        arpl   word ptr [ebp + 0x44], sp
        jae    loc_7_0961
        add    byte ptr [eax], al
loc_7_0900:
        inc    esi
        jne    0x971
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        inc    esp
        jae    0x96f
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0910:
        inc    esp
        jbe    0x97d
        arpl   word ptr [ebp + 0x44], sp
        jae    0x97d
        add    byte ptr [eax], al
loc_7_091C:
        inc    esi
        jne    loc_7_098D
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        inc    esp
        jae    loc_7_098B
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_092C:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0938:
        inc    esp
        jb     0x9a4
        jbe    loc_7_09A2
        jb     loc_7_093F
loc_7_093F:
        add    byte ptr [ebp + 0x76], al
        dec    esp
        outsd  dx, dword ptr [esi]
        popal  
        jb     loc_7_094A
loc_7_094A:
        add    byte ptr [eax], al
loc_7_094C:
        push   ebx
        je     0x9c5
        jo     loc_7_0998
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_7_0957
loc_7_0957:
        add    byte ptr [ebx + 0x6f], dl
        je     0x9d4
        popal  
        jb     0x9c5
        pop    esp
loc_7_0961:
        dec    ebp
        imul   esp, dword ptr [ebx + 0x72], 0x666f736f
        je     0x9c7
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x5c73776f
        inc    ebx
        jne    0x9e8
        jb     0x9dd
        outsb  dx, byte ptr [esi]
        je     0x9d1
        jb     0x9f1
        imul   ebp, dword ptr [edi + 0x6e], 0x4c000000
        outsd  dx, dword ptr [esi]
        popal  
        inc    ebx
        jne    0x9fd
loc_7_098B:
        jae    loc_7_09FC
loc_7_098D:
        jb     loc_7_098F
loc_7_098F:
        add    byte ptr [ebp + 0x53], dl
        inc    ebp
        push   edx
        xor    esi, dword ptr [edx]
        inc    esp
loc_7_0998:
        dec    esp
        dec    esp
        add    byte ptr [eax], al
loc_7_099C:
        dec    ebx
        push   edx
        dec    esi
        dec    esp
        xor    edi, dword ptr [eax]
loc_7_09A2:
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [edx + 0x72], al
        outsd  dx, dword ptr [esi]
        popal  
        arpl   word ptr fs:[ecx + 0x73], sp
        je     0xa05
        jns    loc_7_0A27
        je     loc_7_0A1B
        insd   dword ptr es:[edi], dx
        dec    ebp
        jae    0xa2e
        popal  
        add    byte ptr gs:[bx + si], al
loc_7_09C0:
        dec    ebx
        push   edx
        dec    esi
        dec    esp
        xor    edi, dword ptr [eax]
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [ebx + 0x52], cl
        dec    esi
        dec    esp
        xor    edi, dword ptr [eax]
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [ebx + 0x65], dl
        je     0xa51
        jo     0xa24
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_7_09E3
loc_7_09E3:
        add    byte ptr [ebp + 0x76], al
        imul   esp, dword ptr [ebx + 0x65], 0x63736544
        add    byte ptr [eax], al
loc_7_09F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_09F4:
        inc    ebx
        push   0x30646c69
        xor    byte ptr [eax], dh
loc_7_09FC:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0A00:
        inc    ebx
        push   0x646c69
        add    byte ptr [eax], al
loc_7_0A08:
        and    eax, 0x34302573
        pop    eax
        add    byte ptr [eax], al
loc_7_0A10:
        push   esi
        popal  
        jb     loc_7_0A8D
        imul   ebp, dword ptr [esi + 0x67], 0x6f736552
loc_7_0A1B:
        jne    0xa8f
        arpl   word ptr [ebp + 0x4d], sp
        popal  
        jo     loc_7_0A23
loc_7_0A23:
        add    byte ptr [ebp + 0x46], cl
        inc    esi
loc_7_0A27:
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_7_0A2C
loc_7_0A2C:
        push   edx
        jae    0xa9f
        jne    0xaa4
        arpl   word ptr [ebp + 0x4d], sp
        popal  
        jo     loc_7_0A38
loc_7_0A38:
        push   esi
        popal  
        jb     0xab5
        imul   ebp, dword ptr [esi + 0x67], 0x6f736552
        jne    0xab7
        arpl   word ptr [ebp + 0x4d], sp
        popal  
        jo     loc_7_0A4B
loc_7_0A4B:
        add    byte ptr [ebp + 0x46], cl
        pop    esp
        inc    ebx
        push   0x25646c69
        xor    byte ptr [eax + ebx*2], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0A5C:
        dec    ebp
        inc    esi
        pop    esp
        inc    ebx
        push   0x25646c69
        xor    byte ptr [eax + ebx*2], dh
        push   ebx
        jne    0xace
        inc    ebx
        push   0x25646c69
        xor    byte ptr [eax + ebx*2], dh
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x61], cl
        jb     0xae0
        ja     loc_7_0ADF
        jb     loc_7_0AE5
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
loc_7_0A84:
        dec    eax
        popal  
        jb     0xaec
        ja     loc_7_0AEB
        jb     loc_7_0AF1
        dec    ecx
loc_7_0A8D:
        inc    esp
        add    byte ptr [eax], al
loc_7_0A90:
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     0xaf6
        je     0xb00
        bound  ebp, qword ptr [ebp + 0x49]
        inc    esp
        jae    loc_7_0A9E
loc_7_0A9E:
        add    byte ptr [eax], al
loc_7_0AA0:
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     0xb06
        je     0xb10
        bound  ebp, qword ptr [ebp + 0x49]
        inc    esp
        jae    loc_7_0AAE
loc_7_0AAE:
        add    byte ptr [eax], al
loc_7_0AB0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_7_0AB4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0AB8:
        inc    esi
        outsd  dx, dword ptr [esi]
        jb     loc_7_0B1F
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
        add    byte ptr [edi + 0x76], cl
        jb     loc_7_0B3F
        imul   esp, dword ptr [ebp], 0x44000000
        je     0xb1e
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_7_0ADD
loc_7_0ADD:
        add    byte ptr [eax], al
loc_7_0ADF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0AE5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0AEB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0AF1:
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
        add    byte ptr [ebp + ecx*2 + 0x53], al
        je     0xb87
        jo     0xb58
        jbe    0xb65
        outsd  dx, dword ptr [esi]
        add    byte ptr gs:[eax], al
loc_7_0B1C:
        push   ebx
        jns    loc_7_0B92
loc_7_0B1F:
        inc    esp
        dec    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 0x53], al
        je     0xb9f
        jo     0xb70
        jbe    loc_7_0B7D
        outsd  dx, dword ptr [esi]
        add    byte ptr gs:[eax], al
loc_7_0B34:
        push   ebx
        jns    0xbaa
        inc    esp
        dec    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 0x53], al
loc_7_0B3F:
        je     loc_7_0BB7
        jo     loc_7_0B88
        jbe    0xb95
        outsd  dx, dword ptr [esi]
        add    byte ptr gs:[eax], al
loc_7_0B4C:
        push   ebx
        jns    0xbc2
        inc    esp
        dec    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 0x50], al
        jb     0xbc2
        jbe    0xbbc
        je     0xbc2
        push   eax
        jb     0xbcf
        bound  ebp, qword ptr [ebp + 0x6d]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0B68:
        push   eax
        jb     0xbd4
        jbe    loc_7_0BCE
        je     0xbd4
        add    byte ptr [ebp + ecx*2 + 0x52], al
        je     0xbdd
        arpl   word ptr [eax + eax], si
loc_7_0B7C:
        inc    esp
loc_7_0B7D:
        je     0xbe5
        arpl   word ptr [eax + eax], si
loc_7_0B84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0B88:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0B8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0B92:
        add    byte ptr [eax], al
loc_7_0B94:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0B98:
        push   edx
        jae    loc_7_0C01
        jb     loc_7_0C14
        push   edx
        popal  
        outsb  dx, byte ptr [esi]
        dec    esp
        imul   esi, dword ptr [ebx + 0x74], 0x52000000
        jae    loc_7_0C15
        jb     loc_7_0C28
        push   edx
        popal  
        outsb  dx, byte ptr [esi]
loc_7_0BB7:
        dec    esp
        imul   esi, dword ptr [ebx + 0x74], 0x52000000
        jae    loc_7_0C29
        jb     0xc3c
        push   edx
        popal  
        outsb  dx, byte ptr [esi]
        dec    esp
loc_7_0BCE:
        imul   esi, dword ptr [ebx + 0x74], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + esi*2 + 0x69], al
        jbe    0xc46
        jb     loc_7_0BE3
loc_7_0BE3:
        add    byte ptr [ebx + 0x79], dl
        jae    loc_7_0C5C
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0xc60
        jb     loc_7_0C55
        outsb  dx, byte ptr [esi]
        je     0xc36
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_7_0C69
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0xc59
        push   ebx
        jb     0xc77
loc_7_0C01:
        imul   esp, dword ptr [ebx + 0x65], 0x6c435c73
        popal  
        jae    loc_7_0C7E
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0C10:
        inc    ebp
        outsb  dx, byte ptr [esi]
        jne    loc_7_0C81
loc_7_0C14:
        pop    esp
loc_7_0C15:
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [edi + ebp*2 + 0x67], cl
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
loc_7_0C28:
        dec    esp
loc_7_0C29:
        outsd  dx, dword ptr [esi]
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
loc_7_0C34:
        and    eax, 0x583430
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x53], al
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x46], 0x616c
        jae    loc_7_0C4A
loc_7_0C4A:
        add    byte ptr [eax], al
loc_7_0C4C:
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x46], 0x616c
loc_7_0C55:
        jae    loc_7_0C58
loc_7_0C58:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0C5C:
        inc    edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_7_0CA4
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0x4c00
loc_7_0C69:
        outsd  dx, dword ptr [esi]
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi], 0
loc_7_0C74:
        inc    ebp
        outsb  dx, byte ptr [esi]
        jne    loc_7_0CE5
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x6f], dl
loc_7_0C7E:
        outsd  dx, dword ptr [esi]
        je     loc_7_0C81
loc_7_0C81:
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax + 0x61], cl
        jb     0xcf4
        ja     loc_7_0CF3
        jb     loc_7_0CF9
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
loc_7_0C98:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 033h, 000h, 013h, 000h, 013h, 000h, 013h, 000h
loc_7_0CA4:
        adc    eax, dword ptr [eax]
        and    al, 0
        adc    eax, dword ptr [eax]
        adc    al, 0
        add    al, 0
        xor    al, 0
        add    al, byte ptr [eax]
        add    al, byte ptr [eax]
        add    al, 0
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 014h, 000h, 015h, 000h, 0FFh, 0FFh, 002h, 000h, 001h, 000h
        db  001h, 000h, 014h, 000h, 002h, 000h, 013h, 000h, 013h, 000h, 096h, 000h, 096h, 000h, 013h, 000h
        db  095h, 000h, 012h, 000h, 013h, 000h, 024h, 000h, 001h, 000h, 012h, 000h, 004h, 000h, 004h
loc_7_0CE5:
        add    byte ptr [eax + eax], al
        add    eax, dword ptr [eax]
        add    al, byte ptr [eax]
        add    al, 0
        add    al, 0
        in     eax, 0
        db  074h
loc_7_0CF3:
        add    byte ptr [edx], al
        add    byte ptr [eax + eax], ah
        db  0FFh
loc_7_0CF9:
        call   dword ptr [eax + eax - 1]
        inc    dword ptr [edx]
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [eax + eax], dh
        add    al, byte ptr [eax]
        adc    al, 0
        db  0FFh, 0FFh, 0FFh, 0FFh, 013h, 000h, 054h, 000h, 065h, 000h, 067h, 003h, 067h, 001h, 002h, 000h
        db  002h, 000h, 003h, 000h, 085h, 000h, 0E5h, 000h, 002h, 000h, 003h, 000h, 003h, 000h, 023h, 000h
        db  003h, 000h, 054h, 000h, 014h, 000h, 002h, 000h, 001h, 000h, 0FFh, 0FFh, 002h, 000h, 003h, 000h
        db  002h, 000h, 023h, 000h, 0FFh, 0FFh, 086h, 000h, 023h, 000h, 017h, 000h, 012h, 000h, 023h, 000h
        db  003h, 000h, 001h, 000h, 003h, 000h, 002h, 000h, 013h, 000h, 002h, 000h, 003h, 000h, 003h, 000h
        db  002h, 000h, 004h, 000h, 004h, 000h, 003h, 000h, 023h, 000h, 003h, 000h, 023h, 000h, 013h, 000h
        db  013h, 000h, 013h, 000h, 002h, 000h, 013h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C1h, 0C0h, 081h, 0C1h, 040h, 001h, 001h, 0C3h
        db  0C0h, 003h, 080h, 002h, 041h, 0C2h, 001h, 0C6h, 0C0h, 006h, 080h, 007h, 041h, 0C7h, 000h, 005h
        db  0C1h, 0C5h, 081h, 0C4h, 040h, 004h, 000h, 000h, 001h, 0CCh, 001h, 0D8h, 000h, 014h, 001h, 0F0h
        db  000h, 03Ch, 000h, 028h, 001h, 0E4h, 001h, 0A0h, 000h, 06Ch, 000h, 078h, 001h, 0B4h, 000h, 050h
        db  001h, 09Ch, 001h, 088h, 000h, 046h, 05Ch, 000h, 000h, 000h, 052h, 04Fh, 04Fh, 054h, 05Ch, 000h
        db  000h, 000h, 044h, 065h, 074h, 046h, 06Ch, 061h, 067h, 073h, 000h, 000h, 000h, 000h, 044h, 065h
        db  074h, 046h, 06Ch, 061h, 067h, 073h, 000h, 000h, 000h, 000h, 052h, 04Fh, 04Fh, 054h, 05Ch, 000h
        db  000h, 000h, 04Ch, 06Fh, 067h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 000h, 000h, 000h, 043h, 04Dh
        db  045h, 06Eh, 075h, 06Dh, 046h, 06Ch, 061h, 067h, 073h, 000h, 043h, 04Dh, 044h, 072h, 069h, 076h
        db  046h, 06Ch, 061h, 067h, 073h, 000h, 044h, 065h, 076h, 069h, 063h, 065h, 044h, 072h, 069h, 076h
        db  065h, 072h, 000h, 000h, 000h, 000h, 045h, 06Eh, 075h, 06Dh, 065h, 072h, 061h, 074h, 06Fh, 072h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 053h, 065h, 074h, 043h, 075h, 072h, 073h, 06Fh, 072h, 000h, 000h, 000h, 053h, 065h
        db  074h, 043h, 075h, 072h, 073h, 06Fh, 072h, 000h, 000h, 000h
sub_7_0E54:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0E58:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0E5C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0E60:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_7_0E64:
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
loc_7_0E84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0E88:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0E8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0E90:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0E94:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0E98:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_7_0E9C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0EA0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0EA4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0EA8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0x366  R-X
loc_8_0000:
        inc    ebx
        dec    edi
        dec    esi
        inc    esi
        dec    ecx
        inc    edi
        dec    ebp
        inc    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_000C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0010:
        dec    eax
        push   esp
        push   edx
        inc    ebp
        inc    ebp
        pop    esp
        push   edx
        dec    edi
        dec    edi
        push   esp
        pop    esp
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
sub_8_0020:
        push   0                             ; -> obj7:0x004C
        push   1
        push   0                             ; -> loc_8_0000
        push   2
        call   sub_5_D0F4
loc_8_0033:
        add    esp, 0x10
        ret    
sub_8_0037:
        push   0
        push   0x16
        call   sub_5_6710
loc_8_0040:
        add    esp, 4
        push   eax
        call   sub_5_D0D0
loc_8_0049:
        ret    
sub_8_004A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        mov    word ptr [ebp - 0xa], ax
        cmp    word ptr [ebp - 0xa], 0x400
        jae    loc_8_006C
        call   sub_8_0037
loc_8_006C:
        VxDCall  APM.Service_0000h               ; cd2000002700
        mov    word ptr [ebp - 8], ax
        cmp    word ptr [ebp - 8], 0x100
        jae    loc_8_0083
        call   sub_8_0037
loc_8_0083:
        VxDCall  SHELL.SHELL_Get_Version         ; cd2000001700
        mov    word ptr [ebp - 6], ax
        cmp    word ptr [ebp - 6], 0x400
        jae    loc_8_009A
        call   sub_8_0037
loc_8_009A:
        VxDCall  VTD.VTD_Get_Version             ; cd2000000500
        xchg   word ptr [ebp - 4], ax
        xchg   dword ptr [ebp - 0x10], ebx
        xchg   dword ptr [ebp - 0x10], ecx
        cmp    word ptr [ebp - 4], 0x310
        jae    loc_8_00B7
        call   sub_8_0037
loc_8_00B7:
        VxDCall  VPICD.VPICD_Get_Version         ; cd2000000300
        xchg   word ptr [ebp - 2], ax
        xchg   dword ptr [ebp - 0x10], ebx
        xchg   dword ptr [ebp - 0x10], ecx
        cmp    word ptr [ebp - 2], 0x400
        jae    loc_8_00D4
        call   sub_8_0037
loc_8_00D4:
        mov    esi, dword ptr [0]            ; -> loc_8_000C
        mov    ecx, esi
        call   sub_5_C3C4
loc_8_00E1:
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_1C1E
loc_8_00EA:
        mov    dword ptr [0], eax            ; -> obj7:0x00A8
        test   eax, eax
        jne    loc_8_00FA
        xor    eax, eax
        jmp    loc_8_0180
loc_8_00FA:
        xor    esi, esi
loc_8_00FC:
        push   0
        xor    edx, edx
        push   esi
        mov    ecx, dword ptr [0]            ; -> obj7:0x00A8
        call   sub_5_16B4
loc_8_010C:
        test   eax, eax
        je     loc_8_0132
        inc    esi
        cmp    esi, 7
        jb     loc_8_00FC
        push   0x201
        push   0x24
        call   sub_5_D134
loc_8_0122:
        add    esp, 8
        mov    dword ptr [0], eax            ; -> obj7:0x00B0
        test   eax, eax
        jne    loc_8_0136
        xor    eax, eax
        jmp    loc_8_0180
loc_8_0132:
        xor    eax, eax
        jmp    loc_8_0180
loc_8_0136:
        mov    ecx, dword ptr [0]            ; -> obj7:0x00A8
        push   0x14
        mov    dword ptr [eax + 0x1c], ecx
        push   5
        mov    dword ptr [eax + 8], 1
        mov    dword ptr [eax + 0x14], 0     ; -> loc_5_0428
        add    eax, 0x24
        mov    dword ptr [0], eax            ; -> obj7:0x00B4
        call   sub_8_0330
        mov    dword ptr [0], eax            ; -> obj1:0x020C
        test   eax, eax
        mov    eax, 0
        je     loc_8_0180
        push   0x14
        push   4
        call   sub_8_0330
        mov    dword ptr [0], eax            ; -> obj7:0x01CC
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_8_0180:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_8_0185:
        call   sub_8_0344
        mov    dword ptr [0], eax            ; -> obj7:0x009C
        call   sub_5_D174
loc_8_0194:
        test   al, 4
        je     loc_8_01A1
        or     byte ptr [1], 8               ; -> obj7:0x00AD
        jmp    loc_8_01AC
loc_8_01A1:
        test   al, 1
        je     loc_8_01AC
        or     byte ptr [1], 0x10            ; -> obj7:0x00AD
loc_8_01AC:
        mov    eax, dword ptr [0x2c]         ; -> obj1:0x01E0
        mov    dword ptr [0], eax            ; -> obj7:0x01D0
        call   sub_4_0354
loc_8_01BB:
        test   eax, eax
        je     loc_8_01C6
        or     byte ptr [1], 0x20            ; -> obj7:0x00AD
loc_8_01C6:
        call   sub_5_675F
loc_8_01CB:
        mov    dword ptr [0], eax            ; -> obj7:0x00C4
        test   eax, eax
        jne    loc_8_01D8
        xor    eax, eax
        jmp    loc_8_01F5
loc_8_01D8:
        call   sub_5_675F
loc_8_01DD:
        mov    dword ptr [0], eax            ; -> obj7:0x00C8
        test   eax, eax
        mov    eax, 0
        je     loc_8_01F5
        call   sub_8_0020
        mov    eax, 1
loc_8_01F5:
        ret    
loc_8_01F6:
        or     byte ptr [0], 2               ; -> obj7:0x00AC
        push   esi
        call   sub_5_5BE4
loc_8_0203:
        test   byte ptr [1], 0x10            ; -> obj7:0x00AD
        jne    loc_8_0211
        call   sub_5_22AF
loc_8_0211:
        xor    esi, esi
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        push   esi
        push   0                             ; -> loc_5_9331
        push   eax
        call   _CONFIGMG_Register_Enumerator
loc_8_0224:
        add    esp, 0xc
        mov    ecx, dword ptr [0]            ; -> obj7:0x00A8
        push   0x90000000
        push   0x808
        call   sub_5_1906
loc_8_023C:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj7:0x00A8
        push   esi
        push   esi
        push   eax
        call   sub_5_1F16
loc_8_024A:
        add    esp, 0x10
        pop    esi
        ret    
loc_8_024F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_2654
loc_8_025B:
        push   0
        xor    edx, edx
        push   esi
        mov    ecx, 0x13
        push   esi
        call   sub_5_1E4B
loc_8_026B:
        push   1
        xor    edx, edx
        push   esi
        mov    ecx, 0x13
        push   esi
        call   sub_5_1E4B
loc_8_027B:
        mov    eax, 1
        pop    esi
        ret    
sub_8_0282:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj7:0x00AC
        mov    ebp, esp
        and    al, 3
        sub    esp, 4
        cmp    al, 3
        je     loc_8_0297
        xor    eax, eax
        jmp    loc_8_0312
loc_8_0297:
        mov    eax, dword ptr [0]            ; -> obj7:0x04A0
        test   eax, eax
        je     loc_8_02B3
        push   0
        push   1
        push   dword ptr [0]                 ; -> obj7:0x04A4
        push   eax
        call   sub_8_0360
        add    esp, 0x10
loc_8_02B3:
        mov    edx, 0                        ; -> loc_8_024F
        mov    ecx, 2
        or     byte ptr [1], 1               ; -> obj7:0x00AD
        call   sub_5_CC30
loc_8_02C9:
        VxDCall  VPEND.Service_0000h             ; cd2000002600
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0x400
        jb     loc_8_02F8
        push   0xc0000000
        push   0                             ; -> loc_1_0569
        call   sub_8_0358
        add    esp, 8
        test   eax, eax
        jne    loc_8_02F8
        or     byte ptr [0], 0x10            ; -> obj7:0x00AC
loc_8_02F8:
        push   0x40000000
        push   0
        push   0                             ; -> loc_5_5E56
        call   sub_8_0350
        add    esp, 0xc
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_8_0312:
        mov    esp, ebp
        pop    ebp
        ret    
sub_8_0316:
        or     byte ptr [1], 2               ; -> obj7:0x00AD
        mov    ecx, 0x1a
        call   sub_5_231C
loc_8_0327:
        mov    eax, 1
        ret    
        db  0CCh, 0CCh, 0CCh
sub_8_0330:
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
sub_8_0344:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  0CCh
sub_8_0350:
        VxDCall  SHELL._SHELL_HookSystemBroadcast ; cd2011801700
        int3   
        int3   
sub_8_0358:
        VxDCall  VPEND.Service_800Ah             ; cd200a802600
        int3   
        int3   
sub_8_0360:
        VMMCall  VMM._AddFreePhysPage            ; cd20d2800100
VxD__LTEXT ENDS
