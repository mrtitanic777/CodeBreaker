; NWNBLINK -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 9681 instructions recovered by recursive descent

Declare_Virtual_Device NWNBLINK, 3, 0, NWNBLINK_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054006001h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x7998  R-X
NWNBLINK_Control:
        cmp    eax, 0
        je     loc_2_0000
        cmp    eax, 1
        je     loc_2_0008
        cmp    eax, 2
        je     loc_2_002C
        cmp    eax, 4
        je     loc_1_0027
        cmp    eax, 0x22
        je     loc_1_0032
        clc    
        ret    
loc_1_0027:
        clc    
        ret    
loc_1_0029:
        cld    
        push   ebx
        call   sub_1_2CE0
        pop    ebx
        ret    
loc_1_0032:
        push   edx
        push   ebx
        call   sub_1_7530
        add    esp, 8
        or     eax, eax
        je     loc_1_0041
        stc    
loc_1_0041:
        ret    
sub_1_0042:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [ebx]
        mov    ebx, 1
        mov    edx, 0x29                     ; -> loc_1_0029
        mov    ecx, dword ptr [ebp + 0xc]
        VxDCall  VNETBIOS.VNETBIOS_Register2     ; cd2005001400
        jae    loc_1_0068
        mov    eax, 0xffffffff
        jmp    loc_1_006F
loc_1_0068:
        mov    ebx, dword ptr [ebp + 8]
        mov    byte ptr [ebx], al
        xor    eax, eax
loc_1_006F:
        pop    ebx
        leave  
        ret    
sub_1_0072:
        push   ebp
        mov    ebp, esp
        movzx  eax, byte ptr [ebp + 8]
        VxDCall  VNETBIOS.VNETBIOS_Deregister    ; cd2004001400
        leave  
        ret    
sub_1_0081:
        mov    eax, 1
        inc    dword ptr [0x7714]            ; -> obj1:0x7714
        jne    loc_1_008F
loc_1_008E:
        ret    
loc_1_008F:
        dec    dword ptr [0x7714]            ; -> obj1:0x7714
        xor    eax, eax
        jmp    loc_1_008E
sub_1_0099:
        mov    eax, 1
        inc    dword ptr [0x7718]            ; -> obj1:0x7718
        jne    loc_1_00A7
loc_1_00A6:
        ret    
loc_1_00A7:
        dec    dword ptr [0x7718]            ; -> obj1:0x7718
        xor    eax, eax
        jmp    loc_1_00A6
sub_1_00B1:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Release_Time_Slice          ; cd2038000100
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        cld    
        ret    
sub_1_00E9:
        push   esi
        movzx  eax, word ptr [0x7991]        ; -> loc_1_7991
        mov    edx, 0
        mov    esi, 0x124                    ; -> loc_1_0124
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x35c], esi        ; -> obj1:0x035C
        cld    
        pop    esi
        ret    
sub_1_010A:
        push   esi
        mov    esi, dword ptr [0x35c]        ; -> obj1:0x035C
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    dword ptr [0x35c], 0          ; -> obj1:0x035C
        cld    
        pop    esi
        ret    
loc_1_0124:
        cmp    dword ptr [0x35c], 0          ; -> obj1:0x035C
        je     loc_1_0150
        movzx  eax, word ptr [0x7991]        ; -> loc_1_7991
        mov    edx, 0
        mov    esi, 0x124                    ; -> loc_1_0124
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x35c], esi        ; -> obj1:0x035C
        cld    
        call   sub_1_3C50
loc_1_0150:
        ret    
sub_1_0151:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebx + 0x31], al
        xor    eax, eax
        cmp    eax, dword ptr [ebx + 0x2c]
        je     loc_1_0168
        call   dword ptr [ebx + 0x2c]
loc_1_0168:
        popal  
        leave  
        ret    
sub_1_016B:
        push   ebp
        mov    ebp, esp
        mov    eax, 0
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        leave  
        ret    
sub_1_0187:
        push   ebp
        mov    ebp, esp
        mov    ax, word ptr [ebp + 8]
        xchg   al, ah
        leave  
        ret    
sub_1_0192:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    al, byte ptr [ebp + 0x10]
        mov    ah, al
        mov    cx, ax
        shl    eax, 0x10
        mov    ax, cx
        mov    ecx, dword ptr [ebp + 0xc]
        shr    ecx, 2
        mov    edi, dword ptr [ebp + 8]
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        pop    edi
        leave  
        ret    
sub_1_01BB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        movzx  ecx, word ptr [ebp + 0x10]
        mov    eax, ecx
        cld    
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_01DD:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
        push   ebp
        mov    ebp, esp
        mov    eax, 2
        push   eax
        push   dword ptr [ebp + 8]
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        leave  
        ret    
sub_1_01FA:
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
        ret    
sub_1_0201:
        push   ebp
        mov    ebp, esp
        push   ebx
        xor    ebx, ebx
        mov    edx, 0x5504
        mov    al, byte ptr [ebp + 8]
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    ebx
        leave  
        ret    
loc_1_0219:
        sub    esp, 4
        mov    eax, esp
        push   eax
        push   esi
        push   edi
        push   ecx
        call   sub_1_5FA0
        add    esp, 0x10
        pop    ebx
        mov    esi, eax
        ret    
sub_1_022E:
        push   ebx
        push   esi
        mov    ebx, 0xe
        mov    edx, 0x5504
        mov    esi, 0x219                    ; -> loc_1_0219
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    esi
        pop    ebx
        ret    
        db  053h, 0BBh, 00Ah, 000h, 000h, 000h, 0CDh, 020h, 001h, 000h, 087h, 004h, 0FCh, 05Bh, 0C3h
sub_1_0258:
        push   ebx
        mov    edx, 0x5504
        mov    ebx, 1
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    ebx
        ret    
sub_1_026C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, 2
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        or     eax, eax
        je     loc_1_028A
        xor    ecx, ecx
loc_1_028A:
        movzx  eax, cx
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0292:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, 3
        mov    esi, dword ptr [ebp + 8]
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        inc    dword ptr [0x7840]            ; -> obj1:0x7840
        cld    
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_02B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, 4
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_02CA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 9
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_02E2:
        push   ebx
        mov    ebx, 0x1a
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    ebx
        ret    
sub_1_02F1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, 6
        mov    esi, dword ptr [ebp + 8]
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'NWNBLINK'                  ; DDB_Name
        dd  054006001h                  ; DDB_Init_Order
        dd  NWNBLINK_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 04Eh, 057h, 04Eh, 042h
        db  04Ch, 049h, 04Eh, 04Bh, 000h, 04Eh, 057h, 04Eh, 042h, 04Ch, 049h, 04Eh, 04Bh, 03Ah, 020h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0380:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax + 0xa], 0x2a
        jne    loc_1_0390
        mov    al, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_0390:
        mov    dword ptr [eax + 0x3c], 0x920 ; -> loc_1_0920
        push   eax
        call   sub_1_27D0
        add    esp, 4
        or     eax, eax
        jne    loc_1_03A7
        mov    al, 9
        ret    
loc_1_03A7:
        mov    al, 0xff
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_03B0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        lea    eax, [esi + 0x1a]
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     loc_1_040B
        test   byte ptr [eax + 0x14], 4
        je     loc_1_040B
        cmp    byte ptr [esi + 0xa], 0x2a
        jne    loc_1_03F0
        lea    edx, [esi + 0x34]
        add    eax, 0x30
        mov    dword ptr [edx], eax
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [ecx], edx
        mov    dword ptr [eax + 4], edx
        mov    al, 0xff
        mov    byte ptr [esi + 0x32], 3
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_03F0:
        lea    ecx, [esi + 0x34]
        add    eax, 0x28
        mov    dword ptr [ecx], eax
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 4], ecx
        mov    al, 0xff
        mov    byte ptr [esi + 0x32], 2
        pop    esi
        ret    
loc_1_040B:
        mov    al, 0x15
        pop    esi
        ret    
        db  090h
sub_1_0410:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        sub    bl, bl
        mov    al, byte ptr [esi + 0x32]
        cmp    al, 2
        je     loc_1_042A
        cmp    al, 3
        je     loc_1_042A
        mov    bl, 0x26
        mov    al, bl
        pop    esi
        pop    ebx
        ret    
loc_1_042A:
        lea    eax, [esi + 0x34]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 0x32], 0
        mov    byte ptr [esi + 1], 0xb
        mov    al, bl
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0450:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        sub    bl, bl
        cmp    byte ptr [esi + 0x32], 8
        jne    loc_1_0476
        lea    eax, [esi + 0x34]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 0x32], bl
        mov    byte ptr [esi + 1], 0xb
        mov    al, bl
        pop    esi
        pop    ebx
        ret    
loc_1_0476:
        mov    bl, 0x26
        mov    al, bl
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_0480:
        push   esi
        push   edi
        sub    al, al
        mov    edi, dword ptr [esp + 0xc]
        xor    edx, edx
        mov    ecx, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    dl, byte ptr [edi + 2]
        mov    esi, dword ptr [ecx + edx*4]
        or     esi, esi
        jne    loc_1_049C
        mov    al, 8
loc_1_049C:
        or     al, al
        jne    loc_1_0507
        mov    cl, byte ptr [esi + 6]
        cmp    cl, 1
        je     loc_1_04AD
        cmp    cl, 6
        jne    loc_1_04AF
loc_1_04AD:
        mov    al, 8
loc_1_04AF:
        or     al, al
        jne    loc_1_0507
        mov    dword ptr [esi + 0x70], edi
        mov    byte ptr [edi + 0x32], 0xa
        cmp    byte ptr [esi + 7], 0
        jne    loc_1_0500
        mov    byte ptr [esi + 6], 6
        mov    eax, dword ptr [0x7994]       ; -> obj1:0x7994
        mov    word ptr [esi + 0x28], ax
        mov    eax, dword ptr [esi + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_04F3
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   0
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0xc
        mov    al, 0xff
        pop    edi
        pop    esi
        ret    
loc_1_04F3:
        or     word ptr [esi + 4], 0x1000
        mov    al, 0xff
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_0500:
        or     word ptr [esi + 4], 0x10
        mov    al, 0xff
loc_1_0507:
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0510:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        push   0xa0
        call   sub_1_01DD
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_1_0738
        mov    dl, byte ptr [0x7950]         ; -> obj1:0x7950
        mov    ecx, dword ptr [0x797c]       ; -> obj1:0x797C
        lea    esp, [esp]
loc_1_0540:
        inc    dl
        cmp    dl, 0xff
        jne    loc_1_0549
        mov    dl, 1
loc_1_0549:
        xor    eax, eax
        mov    al, dl
        cmp    dword ptr [ecx + eax*4], 0
        jne    loc_1_0540
        mov    bl, dl
        and    ebx, 0xffff00ff
        mov    word ptr [esp + 0x10], bx
        mov    eax, ebx
        and    eax, 0xffff
        lea    edi, [eax*4]
        mov    dword ptr [ecx + edi], esi
        push   0
        push   0xa0
        push   esi
        call   sub_1_0192
        add    esp, 0xc
        mov    word ptr [esi + 4], 1
        mov    ax, word ptr [0x7950]         ; -> obj1:0x7950
        and    ax, 0xff00
        or     ax, word ptr [esp + 0x10]
        mov    word ptr [0x7950], ax         ; -> obj1:0x7950
        mov    word ptr [esi + 0x10], ax
        mov    eax, dword ptr [0x7968]       ; -> obj1:0x7968
        mov    ecx, 0xa
        mov    word ptr [esi + 0x30], ax
        xor    eax, eax
        mov    al, byte ptr [esp + 0x28]
        add    eax, 9
        cdq    
        idiv   ecx
        mov    byte ptr [esi + 0x2d], al
        xor    eax, eax
        mov    al, byte ptr [esp + 0x2c]
        add    eax, 9
        cdq    
        idiv   ecx
        mov    byte ptr [esi + 0x2e], al
        push   0
        push   0xa4
        call   sub_1_01DD
        add    esp, 8
        mov    dword ptr [esi + 0x68], eax
        or     eax, eax
        jne    loc_1_0603
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    dword ptr [edi + eax], 0
        push   0
        push   esi
        call   sub_1_01FA
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0603:
        mov    word ptr [eax + 0xa], 0x5504
        mov    eax, dword ptr [esi + 0x68]
        mov    byte ptr [eax + 8], 0
        mov    eax, dword ptr [esi + 0x68]
        mov    word ptr [eax + 0x3c], 2
        mov    eax, dword ptr [esi + 0x68]
        lea    ecx, [eax + 0x4c]
        mov    dword ptr [eax + 0x24], ecx
        mov    eax, dword ptr [esi + 0x68]
        mov    ecx, dword ptr [esp + 0x18]
        mov    word ptr [eax + 0x5c], 0x5504
        lea    edx, [esi + 0x7a]
        mov    byte ptr [esi + 7], 0
        mov    word ptr [esi + 0x20], 1
        mov    word ptr [esi + 0x34], 1
        mov    word ptr [esi + 0x22], 1
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0xc], eax
        mov    edi, dword ptr [esp + 0x1c]
        lea    edx, [esi + 0x8a]
        mov    eax, dword ptr [edi]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [edx + 0xc], eax
        mov    ebx, dword ptr [esi + 0x68]
        mov    edx, dword ptr [esp + 0x20]
        lea    edi, [ebx + 0x52]
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [edi + 4], eax
        add    ebx, 0x4c
        mov    ax, word ptr [edx + 8]
        mov    word ptr [edi + 8], ax
        mov    edi, dword ptr [esp + 0x24]
        mov    eax, dword ptr [esi + 0x68]
        mov    ebp, dword ptr [edi]
        mov    dword ptr [eax + 0x1c], ebp
        mov    bp, word ptr [edi + 4]
        mov    word ptr [eax + 0x20], bp
        add    eax, 0x1c
        mov    byte ptr [ebx + 5], 4
        mov    eax, dword ptr [esi + 0x68]
        mov    word ptr [eax + 0x28], 0x30
        mov    eax, dword ptr [esi + 0x68]
        or     word ptr [eax + 0x3c], 0x80
        mov    eax, dword ptr [esi + 0x68]
        mov    bx, word ptr [esp + 0x10]
        mov    word ptr [eax + 0x3e], bx
        lea    ebx, [esi + 0x3c]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebx + 4], eax
        mov    ax, word ptr [edx + 8]
        lea    edx, [esi + 0x36]
        mov    word ptr [ebx + 8], ax
        mov    eax, dword ptr [edi]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [edi + 4]
        mov    word ptr [edx + 4], ax
        sub    edx, edx
        mov    eax, dword ptr [ecx + 0x38]
        mov    dword ptr [esi], eax
        mov    dword ptr [ecx + 0x38], esi
        mov    dword ptr [esi + 0x48], ecx
        mov    ecx, 0x32
        mov    eax, dword ptr [0x78e8]       ; -> obj1:0x78E8
        div    ecx
        cmp    eax, 1
        jb     loc_1_071D
        mov    eax, 1
loc_1_071D:
        mov    byte ptr [esi + 0x2c], al
        lea    eax, [esi + 0x54]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x58], eax
        lea    eax, [esi + 0x5c]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x60], eax
        lea    eax, [esi + 0x4c]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x50], eax
loc_1_0738:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0750:
        push   esi
        mov    esi, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cl, byte ptr [esi + 0x10]
        mov    dword ptr [eax + ecx*4], 0
        mov    eax, dword ptr [esi + 0x48]
        or     eax, eax
        je     loc_1_077D
        test   byte ptr [eax + 0x14], 0x40
        je     loc_1_077D
        push   esi
        push   eax
        call   sub_1_36E0
        add    esp, 8
loc_1_077D:
        mov    eax, dword ptr [esi + 0x68]
        or     eax, eax
        je     loc_1_078D
        push   eax
        call   sub_1_3820
        add    esp, 4
loc_1_078D:
        mov    word ptr [esi + 4], 0
        push   0
        push   esi
        call   sub_1_01FA
        add    esp, 8
        pop    esi
        ret    
sub_1_07A0:
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [esp + 0x14]
        mov    edi, ebx
        and    edi, 0xff
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        lea    eax, [esi + 0x54]
        push   eax
        call   sub_1_7080
        add    esp, 8
        push   edi
        lea    eax, [esi + 0x5c]
        push   eax
        call   sub_1_7080
        add    esp, 8
        push   edi
        lea    eax, [esi + 0x4c]
        push   eax
        call   sub_1_7080
        add    esp, 8
        mov    eax, dword ptr [esi + 0x68]
        or     eax, eax
        je     loc_1_080D
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_0800
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_3820
        add    esp, 4
        jmp    loc_1_0806
        db  08Bh, 0FFh
loc_1_0800:
        mov    word ptr [eax + 0x3e], 0xff
loc_1_0806:
        mov    dword ptr [esi + 0x68], 0
loc_1_080D:
        mov    eax, dword ptr [esi + 0x6c]
        or     eax, eax
        je     loc_1_0836
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_0836
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x6c]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 0x6c], 0
loc_1_0836:
        test   byte ptr [esi + 4], 0x40
        je     loc_1_086F
        mov    eax, dword ptr [0x78d4]       ; -> obj1:0x78D4
        cmp    eax, 0x78d4                   ; -> obj1:0x78D4
        je     loc_1_0869
loc_1_0848:
        mov    ecx, esi
        sub    ecx, eax
        cmp    ecx, -8
        je     loc_1_0860
        mov    eax, dword ptr [eax]
        cmp    eax, 0x78d4                   ; -> obj1:0x78D4
        jne    loc_1_0848
        jmp    loc_1_0869
        db  08Dh, 064h, 024h, 000h
loc_1_0860:
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_0869:
        and    word ptr [esi + 4], 0xffbf
loc_1_086F:
        mov    eax, dword ptr [esi + 0x48]
        mov    byte ptr [esi + 7], 0
        mov    edx, dword ptr [eax + 0x18]
        mov    ecx, edx
        sub    ecx, eax
        cmp    ecx, 0x18
        mov    edi, 0
        je     loc_1_0893
        lea    edi, [edx - 0x34]
        push   edx
        call   sub_1_3B50
        add    esp, 4
loc_1_0893:
        or     edi, edi
        je     loc_1_08C4
        mov    al, byte ptr [esi + 0x10]
        mov    byte ptr [edi + 2], al
        mov    byte ptr [edi + 1], bl
        test   byte ptr [edi], 0x80
        je     loc_1_08C4
        mov    dword ptr [edi + 0x34], 0x7890 ; -> obj1:0x7890
        add    edi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], edi
        mov    dword ptr [0x7894], edi       ; -> obj1:0x7894
loc_1_08C4:
        mov    ecx, dword ptr [esi + 0x70]
        mov    dword ptr [esi + 0x70], 0
        or     ecx, ecx
        je     loc_1_090E
        mov    al, byte ptr [ecx]
        and    eax, 0x7f
        cmp    eax, 0x12
        jne    loc_1_090E
        sub    bl, 0xa
        cmp    bl, 1
        cmc    
        sbb    al, al
        and    al, 0x18
        mov    byte ptr [ecx + 1], al
        test   byte ptr [ecx], 0x80
        je     loc_1_090E
        mov    dword ptr [ecx + 0x34], 0x7890 ; -> obj1:0x7890
        add    ecx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x7894], ecx       ; -> obj1:0x7894
loc_1_090E:
        mov    byte ptr [esi + 6], 0
        push   esi
        call   sub_1_0750
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_0920:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        lea    eax, [esi + 0x1a]
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     loc_1_09D1
        test   byte ptr [eax + 0x14], 4
        je     loc_1_09D1
        mov    edx, dword ptr [esp + 0x10]
        or     edx, edx
        jne    loc_1_0953
        mov    al, 0x14
        jmp    loc_1_09D3
loc_1_0953:
        mov    cl, byte ptr [edx + 0x28]
        and    cl, 0x86
        cmp    cl, 4
        je     loc_1_0962
        mov    al, 0x14
        jmp    loc_1_09D3
loc_1_0962:
        mov    cl, byte ptr [esi + 0x2b]
        push   ecx
        mov    cl, byte ptr [esi + 0x2a]
        push   ecx
        lea    ecx, [edx + 0x1a]
        push   ecx
        lea    ecx, [edx + 0x10]
        push   ecx
        push   edx
        push   eax
        call   sub_1_0510
        add    esp, 0x18
        mov    edi, eax
        or     edi, edi
        jne    loc_1_0986
        mov    al, 0x11
        jmp    loc_1_09D3
loc_1_0986:
        mov    dword ptr [edi + 0x70], esi
        mov    byte ptr [edi + 6], 1
        call   sub_1_74F0
        mov    word ptr [edi + 0x9a], ax
        mov    word ptr [edi + 0x9c], 0x25
        mov    word ptr [edi + 0x9e], 0xd
        mov    eax, dword ptr [0x7994]       ; -> obj1:0x7994
        mov    word ptr [edi + 0x28], ax
        mov    al, byte ptr [edi + 0x10]
        mov    byte ptr [esi + 2], al
        mov    byte ptr [esi + 0x32], 0xa
        push   0
        mov    eax, dword ptr [edi + 0x68]
        push   eax
        push   edi
        call   sub_1_0A10
        add    esp, 0xc
        mov    al, 0xff
        jmp    loc_1_09D3
loc_1_09D1:
        mov    al, 0x15
loc_1_09D3:
        cmp    al, 0xff
        je     loc_1_09FE
        mov    byte ptr [esi + 1], al
        test   byte ptr [esi], 0x80
        je     loc_1_09FE
        mov    dword ptr [esi + 0x34], 0x7890 ; -> obj1:0x7890
        add    esi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], esi
        mov    dword ptr [0x7894], esi       ; -> obj1:0x7894
loc_1_09FE:
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A10:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        lea    edx, [ecx + 0x4c]
        lea    esi, [ecx + 0x6a]
        mov    byte ptr [edx + 5], 4
        mov    word ptr [ecx + 0x28], 0x30
        lea    eax, [edi + 0x7a]
        mov    dword ptr [ecx + 0x2a], eax
        mov    word ptr [ecx + 0x2e], 0x26
        mov    word ptr [ecx + 0x22], 2
        mov    al, byte ptr [edi + 0x10]
        and    eax, 0xffff00ff
        mov    word ptr [ecx + 0x3e], ax
        add    edx, 6
        mov    eax, dword ptr [edi + 0x3c]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edi + 0x40]
        mov    dword ptr [edx + 4], eax
        mov    ax, word ptr [edi + 0x44]
        lea    ebx, [edi + 0x3c]
        mov    word ptr [edx + 8], ax
        lea    eax, [edi + 0x36]
        lea    ebx, [ecx + 0x1c]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebx], edx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebx + 4], ax
        mov    edx, 0x1040                   ; -> sub_1_1040
        mov    ax, word ptr [edi + 0x10]
        mov    word ptr [esi + 2], ax
        mov    al, byte ptr [esp + 0x18]
        or     al, al
        jne    loc_1_0A8E
        mov    edx, 0xce0                    ; -> loc_1_0CE0
loc_1_0A8E:
        mov    dword ptr [ecx + 4], edx
        or     al, al
        je     loc_1_0AA0
        mov    dx, word ptr [edi + 0x12]
        mov    word ptr [esi + 4], dx
        jmp    loc_1_0AA6
        db  090h
loc_1_0AA0:
        mov    word ptr [esi + 4], 0xffff
loc_1_0AA6:
        mov    byte ptr [esi], 0
        cmp    byte ptr [0x788e], 0          ; -> obj1:0x788E
        je     loc_1_0ABF
        test   byte ptr [edi + 5], 0x80
        jne    loc_1_0ABC
        or     al, al
        jne    loc_1_0ABF
loc_1_0ABC:
        mov    byte ptr [esi], 1
loc_1_0ABF:
        cmp    al, 1
        sbb    dl, dl
        and    dl, 0xc0
        add    dl, 0x80
        or     byte ptr [esi], dl
        cmp    al, 1
        mov    word ptr [esi + 6], 0
        sbb    eax, eax
        inc    eax
        mov    word ptr [esi + 0xe], ax
        mov    byte ptr [esi + 1], 6
        mov    word ptr [esi + 0xc], 0x26
        mov    word ptr [esi + 8], 0x26
        add    ecx, 0x44
        mov    word ptr [esi + 0xa], 0
        mov    word ptr [esi + 0x10], 0
        mov    dword ptr [ecx], 0x78a0       ; -> obj1:0x78A0
        mov    eax, dword ptr [0x78a4]       ; -> obj1:0x78A4
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x78a4]       ; -> obj1:0x78A4
        pop    edi
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x78a4], ecx       ; -> obj1:0x78A4
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0B20:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        mov    cl, byte ptr [esp + 0x14]
        lea    esi, [edx + 0x6a]
        cmp    cl, 1
        sbb    al, al
        add    al, 8
        mov    edi, dword ptr [esp + 0xc]
        mov    byte ptr [esi + 1], al
        mov    ax, word ptr [edi + 0x10]
        mov    word ptr [esi + 2], ax
        cmp    cl, 1
        mov    ax, word ptr [edi + 0x12]
        mov    word ptr [esi + 4], ax
        mov    ax, word ptr [edi + 0x14]
        mov    word ptr [esi + 6], ax
        mov    ax, word ptr [edi + 0x1c]
        mov    word ptr [esi + 0xe], ax
        sbb    al, al
        and    al, 0xc0
        add    al, 0x80
        mov    byte ptr [esi], al
        mov    word ptr [esi + 0xc], 0
        mov    word ptr [esi + 0xa], 0
        mov    word ptr [esi + 8], 0
        mov    eax, 0xbc0                    ; -> loc_1_0BC0
        or     cl, cl
        jne    loc_1_0B86
        mov    eax, 0xc60                    ; -> loc_1_0C60
loc_1_0B86:
        mov    dword ptr [edx + 4], eax
        mov    word ptr [edx + 0x22], 1
        mov    dword ptr [edx + 0x44], 0x78a0 ; -> obj1:0x78A0
        add    edx, 0x44
        mov    eax, dword ptr [0x78a4]       ; -> obj1:0x78A4
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x78a4]       ; -> obj1:0x78A4
        pop    edi
        mov    dword ptr [eax], edx
        mov    dword ptr [0x78a4], edx       ; -> obj1:0x78A4
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0BC0:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        xor    edx, edx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    dx, word ptr [ecx + 0x3e]
        mov    esi, dword ptr [eax + edx*4]
        or     esi, esi
        je     loc_1_0BF2
        push   ecx
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 0x68], 0
        push   0xa
        push   esi
        call   sub_1_07A0
        add    esp, 8
loc_1_0BF2:
        call   sub_1_2E80
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C00:
        push   esi
        mov    edx, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cx, word ptr [edx + 0x3e]
        mov    esi, dword ptr [eax + ecx*4]
        or     esi, esi
        je     loc_1_0C50
        mov    ax, word ptr [esi + 0x28]
        dec    ax
        mov    word ptr [esi + 0x28], ax
        je     loc_1_0C31
        push   0
        push   edx
        push   esi
        call   sub_1_0B20
        add    esp, 0xc
        pop    esi
        ret    
loc_1_0C31:
        mov    dword ptr [esi + 0x68], 0
        push   edx
        call   sub_1_3820
        add    esp, 4
        push   0x18
        push   esi
        call   sub_1_07A0
        add    esp, 8
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_0C50:
        push   edx
        call   sub_1_3820
        add    esp, 4
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_0C60:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        xor    edx, edx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    dx, word ptr [ecx + 0x3e]
        mov    esi, dword ptr [eax + edx*4]
        or     esi, esi
        je     loc_1_0CC4
        cmp    byte ptr [ecx + 9], 0
        je     loc_1_0CA0
        mov    dword ptr [esi + 0x68], 0
        push   ecx
        call   sub_1_3820
        add    esp, 4
        push   0x18
        push   esi
        call   sub_1_07A0
        add    esp, 8
        call   sub_1_2E80
        pop    esi
        ret    
        db  090h
loc_1_0CA0:
        mov    eax, dword ptr [esi + 0x68]
        mov    dword ptr [eax + 4], 0xc00    ; -> loc_1_0C00
        xor    eax, eax
        mov    ax, word ptr [esi + 0x30]
        push   eax
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_3BE0
        add    esp, 8
        call   sub_1_2E80
        pop    esi
        ret    
loc_1_0CC4:
        push   ecx
        call   sub_1_3820
        add    esp, 4
        call   sub_1_2E80
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0CE0:
        mov    edx, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cx, word ptr [edx + 0x3e]
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        je     loc_1_0D40
        cmp    byte ptr [eax + 6], 1
        jne    loc_1_0D20
        mov    dword ptr [edx + 4], 0xd50    ; -> loc_1_0D50
        mov    ax, word ptr [eax + 0x30]
        and    eax, 0xffff
        push   eax
        push   edx
        call   sub_1_3BE0
        add    esp, 8
        jmp    sub_1_2E80
        db  005h, 000h, 000h, 000h, 000h
loc_1_0D20:
        mov    word ptr [edx + 0x22], 1
        mov    word ptr [edx + 0x28], 0x30
        mov    ecx, dword ptr [eax + 0x68]
        mov    ax, word ptr [eax + 0x12]
        mov    word ptr [ecx + 0x6e], ax
        push   edx
        call   sub_1_1040
        add    esp, 4
loc_1_0D40:
        jmp    sub_1_2E80
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D50:
        mov    edx, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cx, word ptr [edx + 0x3e]
        mov    ecx, dword ptr [eax + ecx*4]
        or     ecx, ecx
        je     loc_1_0DF6
        cmp    byte ptr [ecx + 6], 1
        jne    loc_1_0DD6
        mov    ax, word ptr [ecx + 0x28]
        or     ax, ax
        je     loc_1_0D90
        dec    ax
        mov    word ptr [ecx + 0x28], ax
        push   0
        push   edx
        push   ecx
        call   sub_1_0A10
        add    esp, 0xc
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_0D90:
        mov    eax, dword ptr [ecx + 0x70]
        mov    byte ptr [eax + 1], 0x12
        mov    eax, dword ptr [ecx + 0x70]
        mov    byte ptr [eax + 0x32], 0
        mov    edx, dword ptr [ecx + 0x70]
        test   byte ptr [edx], 0x80
        je     loc_1_0DC5
        mov    dword ptr [edx + 0x34], 0x7890 ; -> obj1:0x7890
        add    edx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], edx
        mov    dword ptr [0x7894], edx       ; -> obj1:0x7894
loc_1_0DC5:
        mov    dword ptr [ecx + 0x70], 0
        push   ecx
        call   sub_1_0750
        add    esp, 4
        ret    
loc_1_0DD6:
        mov    word ptr [edx + 0x22], 1
        mov    word ptr [edx + 0x28], 0x30
        mov    eax, dword ptr [ecx + 0x68]
        mov    cx, word ptr [ecx + 0x12]
        mov    word ptr [eax + 0x6e], cx
        push   edx
        call   sub_1_1040
        add    esp, 4
loc_1_0DF6:
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E00:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0xc]
        lea    esi, [ecx + 0x6a]
        mov    ax, word ptr [edx + 0x14]
        mov    word ptr [esi + 6], ax
        mov    ax, word ptr [edx + 0x1c]
        mov    word ptr [esi + 0xe], ax
        mov    byte ptr [esi], 0x80
        mov    ax, word ptr [edx + 4]
        test   ah, 8
        je     loc_1_0E33
        and    ax, 0xf7ff
        mov    word ptr [edx + 4], ax
        mov    byte ptr [esi], 0x88
loc_1_0E33:
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        cmp    dword ptr [edx + 0x74], eax
        jae    loc_1_0E40
        or     byte ptr [esi], 0x40
loc_1_0E40:
        test   byte ptr [edx + 5], 0x80
        je     loc_1_0E52
        mov    ax, word ptr [edx + 0x22]
        add    ax, word ptr [edx + 0x1c]
        dec    ax
        jmp    loc_1_0E56
loc_1_0E52:
        mov    ax, word ptr [edx + 0x1e]
loc_1_0E56:
        mov    word ptr [esi + 0x10], ax
        mov    word ptr [esi + 0xc], 0
        mov    word ptr [esi + 0xa], 0
        mov    word ptr [esi + 8], 0
        add    ecx, 0x44
        and    word ptr [edx + 4], 0xdfff
        mov    word ptr [edx + 0x2a], 0
        mov    dword ptr [ecx - 0x40], 0x1040 ; -> sub_1_1040
        mov    word ptr [ecx - 0x22], 1
        mov    dword ptr [ecx], 0x78a0       ; -> obj1:0x78A0
        mov    eax, dword ptr [0x78a4]       ; -> obj1:0x78A4
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x78a4]       ; -> obj1:0x78A4
        pop    esi
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x78a4], ecx       ; -> obj1:0x78A4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0EB0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 0x74]
        add    ecx, 0x4b0
        cmp    ecx, dword ptr [0x7864]       ; -> obj1:0x7864
        jae    loc_1_0EEB
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 0x68], 0
        mov    byte ptr [esi + 6], 6
        push   0x18
        push   esi
        call   sub_1_07A0
        add    esp, 8
        pop    esi
        ret    
loc_1_0EEB:
        mov    eax, dword ptr [esp + 0xc]
        push   esi
        push   eax
        call   sub_1_0E00
        add    esp, 8
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_0F00:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cx, word ptr [esi + 0x3e]
        mov    edi, dword ptr [eax + ecx*4]
        or     edi, edi
        jne    loc_1_0F26
        push   esi
        call   sub_1_3820
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0F26:
        cmp    byte ptr [edi + 6], 3
        jne    loc_1_0FA6
        cmp    byte ptr [edi + 0x2d], 0
        je     loc_1_0FA6
        mov    al, byte ptr [edi + 0x2f]
        or     al, al
        je     loc_1_0FA6
        dec    al
        mov    byte ptr [edi + 0x2f], al
        jne    loc_1_0FA6
        mov    ecx, dword ptr [edi + 0x54]
        mov    eax, ecx
        sub    eax, edi
        cmp    eax, 0x54
        je     loc_1_0FA6
        lea    ebx, [ecx - 0x34]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [ebx + 0x32], 0
        mov    byte ptr [ebx + 1], 5
        test   byte ptr [ebx], 0x80
        je     loc_1_0F84
        mov    dword ptr [ebx + 0x34], 0x7890 ; -> obj1:0x7890
        add    ebx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], ebx
        mov    dword ptr [0x7894], ebx       ; -> obj1:0x7894
loc_1_0F84:
        cmp    word ptr [edi + 0x78], 0
        je     loc_1_0FA0
        mov    byte ptr [edi + 6], 6
        push   0
        push   esi
        push   edi
        call   sub_1_0B20
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_0FA0:
        mov    al, byte ptr [edi + 0x2d]
        mov    byte ptr [edi + 0x2f], al
loc_1_0FA6:
        mov    al, byte ptr [edi + 0x2c]
        dec    al
        mov    byte ptr [edi + 0x2c], al
        jne    loc_1_0FE9
        test   byte ptr [edi + 5], 0x80
        je     loc_1_0FC0
        mov    byte ptr [edi + 0x2c], 6
        jmp    loc_1_0FDB
        db  08Dh, 064h, 024h, 000h
loc_1_0FC0:
        mov    ecx, 0x32
        mov    eax, dword ptr [0x78e8]       ; -> obj1:0x78E8
        sub    edx, edx
        div    ecx
        cmp    eax, 1
        jb     loc_1_0FD8
        mov    eax, 1
loc_1_0FD8:
        mov    byte ptr [edi + 0x2c], al
loc_1_0FDB:
        push   esi
        push   edi
        call   sub_1_0EB0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0FE9:
        push   esi
        call   sub_1_1040
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1000:
        mov    edx, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cx, word ptr [edx + 0x3e]
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        jne    loc_1_1020
        push   edx
        call   sub_1_3820
        add    esp, 4
        ret    
loc_1_1020:
        and    word ptr [eax + 4], 0xdfff
        push   eax
        push   edx
        call   sub_1_0E00
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1040:
        mov    edx, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cx, word ptr [edx + 0x3e]
        mov    ecx, dword ptr [eax + ecx*4]
        or     ecx, ecx
        je     loc_1_1109
        mov    ax, word ptr [ecx + 4]
        test   al, 2
        je     loc_1_1079
        and    ax, 0xfffd
        mov    word ptr [ecx + 4], ax
        push   ecx
        push   edx
        call   sub_1_0E00
        add    esp, 8
        jmp    sub_1_2E80
loc_1_1079:
        test   ah, 0x10
        je     loc_1_1097
        and    ax, 0xefff
        mov    word ptr [ecx + 4], ax
        push   0
        push   edx
        push   ecx
        call   sub_1_0B20
        add    esp, 0xc
        jmp    sub_1_2E80
loc_1_1097:
        test   ah, 1
        je     loc_1_10B5
        and    ax, 0xfeff
        mov    word ptr [ecx + 4], ax
        push   1
        push   edx
        push   ecx
        call   sub_1_0A10
        add    esp, 0xc
        jmp    sub_1_2E80
loc_1_10B5:
        test   al, 8
        je     loc_1_10D2
        and    ax, 0xfff7
        mov    word ptr [ecx + 4], ax
        push   1
        push   edx
        push   ecx
        call   sub_1_0B20
        add    esp, 0xc
        jmp    sub_1_2E80
loc_1_10D2:
        test   ah, 0x20
        je     loc_1_10F2
        mov    dword ptr [edx + 4], 0x1000   ; -> loc_1_1000
        mov    eax, dword ptr [0x793c]       ; -> obj1:0x793C
        push   eax
        push   edx
        call   sub_1_3BE0
        add    esp, 8
        jmp    sub_1_2E80
loc_1_10F2:
        mov    dword ptr [edx + 4], 0xf00    ; -> loc_1_0F00
        push   0x32
        push   edx
        call   sub_1_3BE0
        add    esp, 8
        jmp    sub_1_2E80
loc_1_1109:
        push   edx
        call   sub_1_3820
        add    esp, 4
        jmp    sub_1_2E80
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1120:
        sub    esp, 8
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [esp + 0x14]
        lea    edi, [eax*4]
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    esi, dword ptr [edi + eax]
        or     esi, esi
        jne    loc_1_1149
        mov    eax, 0xff
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_1_1149:
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esi + 0x3c]
        push   eax
        call   sub_1_026C
        add    esp, 8
        mov    ecx, dword ptr [0x797c]       ; -> obj1:0x797C
        cmp    dword ptr [edi + ecx], esi
        je     loc_1_1170
        mov    eax, 0xff
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_1_1170:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x32]
        cmp    ecx, eax
        jae    loc_1_1180
        inc    ecx
        sub    eax, ecx
        jmp    loc_1_1182
        db  090h
loc_1_1180:
        sub    eax, eax
loc_1_1182:
        cmp    eax, 0x1e
        jbe    loc_1_118C
        mov    eax, 0x1e
loc_1_118C:
        add    word ptr [esi + 0x30], ax
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_11A0:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x24]
        add    ecx, 0x1e
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 0x18], ecx
        mov    ax, word ptr [eax + 2]
        and    eax, 0xffff
        push   eax
        call   sub_1_0187
        mov    ecx, dword ptr [esp + 0x1c]
        add    esp, 4
        mov    dx, word ptr [ecx + 8]
        and    eax, 0xffff
        mov    ecx, edx
        and    ecx, 0xffff
        sub    eax, ecx
        cmp    eax, 0x30
        jne    loc_1_14AA
        cmp    dx, 0x26
        jb     loc_1_14AA
        mov    eax, dword ptr [esp + 0x24]
        add    eax, 0x30
        mov    dword ptr [esp + 0x10], eax
        add    eax, 0x10
        push   eax
        call   sub_1_3240
        mov    dword ptr [esp + 0x18], eax
        add    esp, 4
        or     eax, eax
        je     loc_1_14AA
        test   byte ptr [eax + 0x14], 4
        je     loc_1_14AA
        mov    ebx, dword ptr [eax + 0x38]
        or     ebx, ebx
        je     loc_1_1257
        mov    ecx, dword ptr [esp + 0x18]
        mov    ax, word ptr [ecx + 2]
        mov    edi, edi
loc_1_1230:
        cmp    word ptr [ebx + 0x12], ax
        jne    loc_1_1249
        lea    esi, [ebx + 0x8a]
        mov    ecx, 4
        mov    edi, dword ptr [esp + 0x10]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        je     loc_1_124F
loc_1_1249:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_1_1230
loc_1_124F:
        or     ebx, ebx
        jne    loc_1_1483
loc_1_1257:
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, dword ptr [eax + 0x28]
        sub    eax, ebp
        cmp    eax, -0x28
        je     loc_1_1293
loc_1_1265:
        lea    esi, [ebp - 0x2a]
        lea    ebx, [ebp - 0x34]
        mov    ecx, 4
        mov    edi, dword ptr [esp + 0x10]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        je     loc_1_1288
        mov    ebp, dword ptr [ebp]
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, ebp
        cmp    eax, -0x28
        jne    loc_1_1265
        jmp    loc_1_1297
loc_1_1288:
        push   ebp
        call   sub_1_3B50
        add    esp, 4
        jmp    loc_1_1297
loc_1_1293:
        mov    ebx, dword ptr [esp + 0x1c]
loc_1_1297:
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, ebp
        cmp    eax, -0x28
        jne    loc_1_12C5
        mov    eax, dword ptr [esp + 0x14]
        mov    ebx, 0
        mov    ecx, dword ptr [eax + 0x30]
        mov    eax, ecx
        sub    eax, dword ptr [esp + 0x14]
        cmp    eax, 0x30
        je     loc_1_12C5
        lea    ebx, [ecx - 0x34]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_12C5:
        or     ebx, ebx
        je     loc_1_14AA
        mov    al, byte ptr [ebx + 0x2b]
        push   eax
        mov    al, byte ptr [ebx + 0x2a]
        push   eax
        mov    byte ptr [ebx + 0x32], 0
        mov    eax, dword ptr [esp + 0x30]
        push   eax
        mov    eax, dword ptr [esp + 0x30]
        add    eax, 0x12
        push   eax
        mov    eax, dword ptr [esp + 0x20]
        push   eax
        mov    eax, dword ptr [esp + 0x28]
        push   eax
        call   sub_1_0510
        add    esp, 0x18
        mov    esi, eax
        or     esi, esi
        je     loc_1_1452
        mov    ecx, dword ptr [esp + 0x18]
        test   byte ptr [ecx], 1
        je     loc_1_1330
        cmp    byte ptr [0x788e], 0          ; -> obj1:0x788E
        je     loc_1_1330
        or     word ptr [esi + 4], 0x8000
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        add    eax, 0x384
        mov    dword ptr [esi + 0x74], eax
        jmp    loc_1_1341
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1330:
        mov    eax, dword ptr [0x78e8]       ; -> obj1:0x78E8
        lea    ecx, [eax + eax*2]
        add    ecx, dword ptr [0x7864]       ; -> obj1:0x7864
        mov    dword ptr [esi + 0x74], ecx
loc_1_1341:
        mov    eax, dword ptr [esp + 0x24]
        mov    al, byte ptr [eax + 4]
        and    ax, 0xf
        mov    word ptr [esi + 0x32], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 0x10]
        push   eax
        push   0x1120                        ; -> loc_1_1120
        call   sub_1_016B
        add    esp, 8
        mov    al, byte ptr [esi + 0x10]
        mov    byte ptr [ebx + 2], al
        mov    edx, dword ptr [esp + 0x10]
        lea    ecx, [ebx + 0xa]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [esp + 0x18]
        mov    ax, word ptr [ecx + 2]
        mov    word ptr [esi + 0x12], ax
        mov    word ptr [esi + 0x1c], 1
        mov    byte ptr [esi + 6], 3
        call   sub_1_74F0
        mov    eax, dword ptr [esp + 0x10]
        mov    cx, word ptr [0x78e4]         ; -> obj1:0x78E4
        mov    ax, word ptr [eax + 0x20]
        cmp    ax, cx
        jb     loc_1_13B9
        mov    ax, cx
loc_1_13B9:
        mov    word ptr [esi + 0x9a], ax
        cmp    byte ptr [0x7814], 0          ; -> obj1:0x7814
        je     loc_1_13DF
        cmp    word ptr [esi + 0x32], 0
        je     loc_1_13DF
        cmp    ax, 0x400
        jbe    loc_1_13DF
        mov    word ptr [esi + 0x9a], 0x400
loc_1_13DF:
        mov    ax, word ptr [esi + 0x32]
        lea    eax, [eax*2 + 4]
        mov    word ptr [esi + 0x22], ax
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_140C
        mov    ecx, eax
        and    ecx, 0xffff
        mov    eax, dword ptr [0x794c]       ; -> obj1:0x794C
        cmp    ecx, eax
        jbe    loc_1_140C
        mov    word ptr [esi + 0x22], ax
loc_1_140C:
        xor    eax, eax
        mov    ax, word ptr [esi + 0x22]
        mov    al, byte ptr [eax + 0x75b0]   ; -> loc_1_75B0
        and    eax, 0xffff00ff
        mov    word ptr [esi + 0x34], ax
        mov    eax, dword ptr [esp + 0x10]
        push   1
        mov    ax, word ptr [eax + 0x22]
        mov    word ptr [esi + 0x9c], ax
        mov    eax, dword ptr [esp + 0x14]
        mov    ax, word ptr [eax + 0x24]
        mov    word ptr [esi + 0x9e], ax
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        push   esi
        call   sub_1_0A10
        add    esp, 0xc
        sub    al, al
        jmp    loc_1_1454
loc_1_1452:
        mov    al, 0x11
loc_1_1454:
        mov    byte ptr [ebx + 1], al
        test   byte ptr [ebx], 0x80
        je     loc_1_14AA
        mov    dword ptr [ebx + 0x34], 0x7890 ; -> obj1:0x7890
        add    ebx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        pop    ebp
        mov    dword ptr [eax], ebx
        mov    dword ptr [0x7894], ebx       ; -> obj1:0x7894
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_1483:
        cmp    byte ptr [ebx + 6], 3
        jne    loc_1_14AA
        mov    eax, dword ptr [ebx + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_14AA
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   1
        mov    eax, dword ptr [ebx + 0x68]
        push   eax
        push   ebx
        call   sub_1_0A10
        add    esp, 0xc
loc_1_14AA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_14C0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x14]
        xor    eax, eax
        lea    edi, [ebx + 0x1e]
        mov    ax, word ptr [ebx + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        cmp    ax, 0x56
        jb     loc_1_1500
        lea    eax, [ebx + 0x30]
        mov    esi, dword ptr [esp + 0x10]
        mov    ax, word ptr [eax + 0x20]
        cmp    word ptr [esi + 0x9a], ax
        jbe    loc_1_1504
        mov    word ptr [esi + 0x9a], ax
        jmp    loc_1_1504
        db  08Dh, 064h, 024h, 000h
loc_1_1500:
        mov    esi, dword ptr [esp + 0x10]
loc_1_1504:
        cmp    word ptr [edi + 6], 0
        jne    loc_1_1642
        cmp    word ptr [edi + 0xe], 1
        jne    loc_1_1642
        test   byte ptr [edi], 1
        je     loc_1_1534
        or     word ptr [esi + 4], 0x8000
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        add    eax, 0x384
        mov    dword ptr [esi + 0x74], eax
        jmp    loc_1_1545
loc_1_1534:
        mov    eax, dword ptr [0x78e8]       ; -> obj1:0x78E8
        lea    ecx, [eax + eax*2]
        add    ecx, dword ptr [0x7864]       ; -> obj1:0x7864
        mov    dword ptr [esi + 0x74], ecx
loc_1_1545:
        mov    al, byte ptr [ebx + 4]
        and    ax, 0xf
        mov    word ptr [esi + 0x32], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 0x10]
        push   eax
        push   0x1120                        ; -> loc_1_1120
        call   sub_1_016B
        add    esp, 8
        mov    ax, word ptr [edi + 2]
        mov    word ptr [esi + 0x12], ax
        mov    word ptr [esi + 0x24], 1
        mov    word ptr [esi + 0x14], 1
        mov    al, byte ptr [ebx + 4]
        and    ax, 0xf
        lea    eax, [eax*2 + 4]
        mov    word ptr [esi + 0x22], ax
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_15A8
        mov    ecx, eax
        and    ecx, 0xffff
        mov    eax, dword ptr [0x794c]       ; -> obj1:0x794C
        cmp    ecx, eax
        jbe    loc_1_15A8
        mov    word ptr [esi + 0x22], ax
loc_1_15A8:
        mov    ax, word ptr [esi + 0x22]
        mov    word ptr [esi + 0x20], ax
        and    eax, 0xffff
        mov    al, byte ptr [eax + 0x75b0]   ; -> loc_1_75B0
        and    eax, 0xffff00ff
        mov    word ptr [esi + 0x34], ax
        mov    eax, dword ptr [esi + 0x68]
        mov    byte ptr [esi + 6], 3
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_160C
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x68]
        mov    word ptr [eax + 0x22], 1
        mov    eax, dword ptr [esi + 0x68]
        push   0x32
        mov    word ptr [eax + 0x28], 0x30
        mov    ecx, dword ptr [esi + 0x68]
        mov    ax, word ptr [edi + 2]
        mov    word ptr [ecx + 0x6e], ax
        mov    dword ptr [ecx + 4], 0xf00    ; -> loc_1_0F00
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_3BE0
        add    esp, 8
loc_1_160C:
        mov    ecx, dword ptr [esi + 0x70]
        mov    dword ptr [esi + 0x70], 0
        mov    byte ptr [ecx + 0x32], 0
        mov    byte ptr [ecx + 1], 0
        test   byte ptr [ecx], 0x80
        je     loc_1_1642
        mov    dword ptr [ecx + 0x34], 0x7890 ; -> obj1:0x7890
        add    ecx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x7894], ecx       ; -> obj1:0x7894
loc_1_1642:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1650:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        mov    esi, dword ptr [esp + 0x14]
        add    eax, 0x384
        mov    dword ptr [esi + 0x74], eax
        cmp    byte ptr [esi + 7], 0
        mov    edi, dword ptr [esp + 0x18]
        jne    loc_1_1690
        mov    ax, word ptr [edi + 0xe]
        cmp    word ptr [esi + 0x14], ax
        jne    loc_1_1915
        mov    cx, word ptr [edi + 0x10]
        sub    cx, ax
        inc    cx
        mov    word ptr [esi + 0x20], cx
        jmp    loc_1_1915
        db  090h
loc_1_1690:
        mov    cx, word ptr [edi + 0xe]
        mov    dx, cx
        sub    dx, word ptr [esi + 0x24]
        je     loc_1_1915
        cmp    dx, 8
        ja     loc_1_1915
        mov    ax, dx
        dec    dx
        or     ax, ax
        je     loc_1_16ED
loc_1_16B5:
        mov    ax, word ptr [esi + 0x26]
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0x16]
        mov    ebp, eax
        and    ebp, 0xffff
        sub    ebx, ebp
        xor    ebp, ebp
        mov    bp, word ptr [esi + 0x9a]
        cmp    ebx, ebp
        jl     loc_1_16D8
        mov    ebx, ebp
loc_1_16D8:
        add    bx, ax
        inc    word ptr [esi + 0x24]
        mov    word ptr [esi + 0x26], bx
        mov    ax, dx
        dec    dx
        or     ax, ax
        jne    loc_1_16B5
loc_1_16ED:
        mov    ax, word ptr [esi + 0x24]
        mov    dx, word ptr [esi + 0x14]
        sub    dx, ax
        cmp    dx, 8
        jbe    loc_1_170A
        mov    word ptr [esi + 0x14], ax
        mov    ax, word ptr [esi + 0x26]
        mov    word ptr [esi + 0x18], ax
loc_1_170A:
        mov    ax, word ptr [edi + 0x10]
        sub    ax, cx
        inc    ax
        cmp    ax, 8
        jb     loc_1_171D
        mov    ax, 8
loc_1_171D:
        mov    bx, word ptr [esi + 0x20]
        cmp    ax, bx
        jbe    loc_1_173E
        and    eax, 0xffff
        mov    ecx, ebx
        and    ecx, 0xffff
        sub    eax, ecx
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        add    ax, bx
loc_1_173E:
        mov    word ptr [esi + 0x20], ax
        mov    eax, dword ptr [0x7994]       ; -> obj1:0x7994
        mov    word ptr [esi + 0x28], ax
        mov    ax, word ptr [esi + 0x14]
        cmp    word ptr [esi + 0x24], ax
        jne    loc_1_1778
        mov    eax, dword ptr [esi + 0x6c]
        or     eax, eax
        je     loc_1_1778
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x6c]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 0x6c], 0
loc_1_1778:
        mov    ax, word ptr [esi + 0x16]
        cmp    word ptr [esi + 0x26], ax
        jb     loc_1_18CC
        mov    word ptr [esi + 0x26], 0
        mov    ecx, dword ptr [esi + 0x5c]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0x5c
        mov    ebx, 0
        je     loc_1_17A9
        lea    ebx, [ecx - 0x34]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_17A9:
        mov    byte ptr [ebx + 0x32], 0
        mov    al, byte ptr [ebx]
        and    eax, 0x7f
        cmp    eax, 0x48
        jne    loc_1_17F0
        mov    ax, word ptr [ebx + 0xa]
        mov    word ptr [ebx + 8], ax
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 0x3c], 0x4710 ; -> loc_1_4710
        mov    dword ptr [ebx + 0x34], 0x7884 ; -> obj1:0x7884
        add    ebx, 0x34
        mov    eax, dword ptr [0x7888]       ; -> obj1:0x7888
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [0x7888]       ; -> obj1:0x7888
        mov    dword ptr [eax], ebx
        mov    dword ptr [0x7888], ebx       ; -> obj1:0x7888
        jmp    loc_1_1818
        db  08Dh, 049h, 000h
loc_1_17F0:
        mov    byte ptr [ebx + 1], 0
        test   byte ptr [ebx], 0x80
        je     loc_1_1818
        mov    dword ptr [ebx + 0x34], 0x7890 ; -> obj1:0x7890
        add    ebx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], ebx
        mov    dword ptr [0x7894], ebx       ; -> obj1:0x7894
loc_1_1818:
        test   byte ptr [esi + 4], 0x10
        je     loc_1_188B
        mov    byte ptr [esi + 6], 6
        mov    eax, dword ptr [esi + 0x6c]
        or     eax, eax
        je     loc_1_184B
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_184B
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x6c]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 0x6c], 0
loc_1_184B:
        mov    eax, dword ptr [esi + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_1880
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [0x7994]       ; -> obj1:0x7994
        mov    word ptr [esi + 0x28], ax
        push   0
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1880:
        or     word ptr [esi + 4], 0x1000
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_188B:
        mov    edx, dword ptr [esi + 0x5c]
        mov    eax, edx
        sub    eax, esi
        cmp    eax, 0x5c
        je     loc_1_18C8
        mov    cx, word ptr [edx - 0x2c]
        sub    edx, 0x34
        mov    word ptr [esi + 0x16], cx
        mov    al, byte ptr [edx]
        and    eax, 0x7f
        cmp    eax, 0x17
        je     loc_1_18B1
        cmp    eax, 0x72
        jne    loc_1_18BC
loc_1_18B1:
        mov    ax, word ptr [edx + 0xa]
        add    ax, cx
        mov    word ptr [esi + 0x16], ax
loc_1_18BC:
        mov    word ptr [esi + 0x18], 0
        mov    byte ptr [esi + 7], 1
        jmp    loc_1_18CC
loc_1_18C8:
        mov    byte ptr [esi + 7], 0
loc_1_18CC:
        cmp    byte ptr [esi + 7], 8
        jne    loc_1_18E0
        mov    ax, word ptr [esi + 0x16]
        cmp    word ptr [esi + 0x18], ax
        jae    loc_1_18E0
        mov    byte ptr [esi + 7], 1
loc_1_18E0:
        cmp    byte ptr [esi + 7], 1
        jne    loc_1_1915
        mov    ax, word ptr [esi + 4]
        test   al, 0x40
        jne    loc_1_1915
        or     ax, 0x40
        mov    word ptr [esi + 4], ax
        mov    dword ptr [esi + 8], 0x78d4   ; -> obj1:0x78D4
        lea    ecx, [esi + 8]
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x78d8], ecx       ; -> obj1:0x78D8
loc_1_1915:
        test   byte ptr [edi], 0x40
        je     loc_1_1945
        mov    eax, dword ptr [esi + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_1940
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   esi
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_0E00
        add    esp, 8
        jmp    loc_1_1945
        db  005h, 000h, 000h, 000h, 000h
loc_1_1940:
        or     word ptr [esi + 4], 2
loc_1_1945:
        test   byte ptr [edi], 8
        je     loc_1_197C
        mov    eax, dword ptr [esi + 0x6c]
        or     eax, eax
        je     loc_1_1973
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_1973
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x6c]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 0x6c], 0
loc_1_1973:
        push   esi
        call   sub_1_43D0
        add    esp, 4
loc_1_197C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1990:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x78e8]       ; -> obj1:0x78E8
        mov    esi, dword ptr [esp + 0x10]
        lea    ecx, [eax + eax*2]
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        add    ecx, eax
        mov    dword ptr [esi + 0x74], ecx
        mov    al, byte ptr [esi + 7]
        or     al, al
        jne    loc_1_19B5
        jmp    loc_1_1BF0
loc_1_19B5:
        cmp    al, 8
        jne    loc_1_1BF0
        mov    edi, dword ptr [esp + 0x14]
        mov    cx, word ptr [esi + 0x14]
        mov    ax, word ptr [edi + 0xe]
        mov    dx, ax
        sub    dx, cx
        cmp    dx, 1
        je     loc_1_19EC
        cmp    cx, ax
        jne    loc_1_1BF4
        mov    ax, word ptr [esi + 0x18]
        cmp    word ptr [edi + 0x10], ax
        jbe    loc_1_1BF4
loc_1_19EC:
        mov    byte ptr [esi + 7], 1
        mov    eax, dword ptr [esi + 0x6c]
        or     eax, eax
        je     loc_1_1A19
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_1A19
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x6c]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 0x6c], 0
loc_1_1A19:
        mov    ax, word ptr [edi + 0x10]
        mov    word ptr [esi + 0x18], ax
        mov    ax, word ptr [edi + 0xe]
        mov    word ptr [esi + 0x14], ax
        mov    eax, dword ptr [0x7994]       ; -> obj1:0x7994
        mov    word ptr [esi + 0x28], ax
        mov    ax, word ptr [esi + 0x18]
        or     ax, ax
        je     loc_1_1A87
        cmp    word ptr [esi + 0x16], ax
        je     loc_1_1A87
        mov    ax, word ptr [esi + 4]
        test   al, 0x40
        jne    loc_1_1BF4
        mov    ecx, dword ptr [esi + 0x5c]
        sub    ecx, esi
        cmp    ecx, 0x5c
        je     loc_1_1BF4
        or     ax, 0x40
        mov    word ptr [esi + 4], ax
        mov    dword ptr [esi + 8], 0x78d4   ; -> obj1:0x78D4
        lea    ecx, [esi + 8]
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x78d8], ecx       ; -> obj1:0x78D8
        jmp    loc_1_1BF4
loc_1_1A87:
        mov    ecx, dword ptr [esi + 0x5c]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0x5c
        mov    ebx, 0
        je     loc_1_1AA4
        lea    ebx, [ecx - 0x34]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_1AA4:
        test   byte ptr [esi + 4], 0x10
        je     loc_1_1AEB
        mov    eax, dword ptr [esi + 0x68]
        mov    byte ptr [esi + 6], 6
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_1AE0
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [0x7994]       ; -> obj1:0x7994
        mov    word ptr [esi + 0x28], ax
        push   0
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0xc
        jmp    loc_1_1B7B
        db  08Dh, 049h, 000h
loc_1_1AE0:
        or     word ptr [esi + 4], 0x1000
        jmp    loc_1_1B7B
loc_1_1AEB:
        mov    edx, dword ptr [esi + 0x5c]
        mov    eax, edx
        sub    eax, esi
        cmp    eax, 0x5c
        jne    loc_1_1B21
        mov    byte ptr [esi + 7], 0
        mov    byte ptr [esi + 6], 3
        mov    ax, word ptr [esi + 4]
        test   al, 0x40
        je     loc_1_1B7B
        test   ax, 0x1f9e
        jne    loc_1_1B7B
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        and    word ptr [esi + 4], 0xffbf
        jmp    loc_1_1B7B
loc_1_1B21:
        sub    edx, 0x34
        mov    word ptr [esi + 0x18], 0
        mov    cx, word ptr [edx + 8]
        mov    word ptr [esi + 0x16], cx
        mov    al, byte ptr [edx]
        and    eax, 0x7f
        cmp    eax, 0x17
        je     loc_1_1B41
        cmp    eax, 0x72
        jne    loc_1_1B4C
loc_1_1B41:
        mov    ax, word ptr [edx + 0xa]
        add    ax, cx
        mov    word ptr [esi + 0x16], ax
loc_1_1B4C:
        mov    ax, word ptr [esi + 4]
        test   al, 0x40
        jne    loc_1_1B7B
        or     ax, 0x40
        mov    word ptr [esi + 4], ax
        mov    dword ptr [esi + 8], 0x78d4   ; -> obj1:0x78D4
        lea    ecx, [esi + 8]
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x78d8], ecx       ; -> obj1:0x78D8
loc_1_1B7B:
        mov    al, byte ptr [ebx]
        and    eax, 0x7f
        cmp    eax, 0x48
        jne    loc_1_1BC0
        mov    ax, word ptr [ebx + 0xa]
        mov    word ptr [ebx + 8], ax
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 0x3c], 0x4710 ; -> loc_1_4710
        mov    dword ptr [ebx + 0x34], 0x7884 ; -> obj1:0x7884
        add    ebx, 0x34
        mov    eax, dword ptr [0x7888]       ; -> obj1:0x7888
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [0x7888]       ; -> obj1:0x7888
        mov    dword ptr [eax], ebx
        mov    dword ptr [0x7888], ebx       ; -> obj1:0x7888
        jmp    loc_1_1BF4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1BC0:
        mov    byte ptr [ebx + 1], 0
        test   byte ptr [ebx], 0x80
        je     loc_1_1BF4
        mov    dword ptr [ebx + 0x34], 0x7890 ; -> obj1:0x7890
        add    ebx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], ebx
        mov    dword ptr [0x7894], ebx       ; -> obj1:0x7894
        jmp    loc_1_1BF4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1BF0:
        mov    edi, dword ptr [esp + 0x14]
loc_1_1BF4:
        test   byte ptr [edi], 0x40
        je     loc_1_1C25
        mov    eax, dword ptr [esi + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_1C20
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   esi
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_0E00
        add    esp, 8
        jmp    loc_1_1C25
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1C20:
        or     word ptr [esi + 4], 2
loc_1_1C25:
        test   byte ptr [edi], 8
        je     loc_1_1C4C
        mov    eax, dword ptr [esi + 0x6c]
        or     eax, eax
        je     loc_1_1C4C
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_1C4C
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x6c]
        push   eax
        call   sub_1_4230
        add    esp, 4
loc_1_1C4C:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1C50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x18]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x4e]
        push   eax
        call   sub_1_0187
        mov    edi, dword ptr [esp + 0x20]
        add    esp, 4
        mov    dx, word ptr [edi + 6]
        mov    esi, dword ptr [esp + 0x14]
        mov    cx, word ptr [esi + 0x1c]
        cmp    dx, cx
        jne    loc_1_1CC0
        xor    ebp, ebp
        mov    bp, word ptr [edi + 0xc]
        and    eax, 0xffff
        sub    eax, 0x30
        cmp    ebp, eax
        ja     loc_1_1CC0
        mov    ax, word ptr [esi + 4]
        and    ax, 0xff7f
        mov    word ptr [esi + 4], ax
        test   ah, 0x80
        push   edi
        push   ebx
        push   esi
        je     loc_1_1CB0
        call   sub_1_4B30
        add    esp, 0xc
        jmp    loc_1_1CDC
        db  08Dh, 064h, 024h, 000h
loc_1_1CB0:
        call   sub_1_4960
        add    esp, 0xc
        jmp    loc_1_1CDC
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1CC0:
        sub    dx, cx
        cmp    dx, 8
        ja     loc_1_1CDC
        mov    ax, word ptr [esi + 4]
        test   al, 0x80
        jne    loc_1_1CDC
        or     ax, 0x880
        mov    word ptr [esi + 4], ax
        or     byte ptr [edi], 0x40
loc_1_1CDC:
        test   byte ptr [esi + 5], 0x80
        je     loc_1_1D46
        push   edi
        push   esi
        call   sub_1_1650
        add    esp, 8
        cmp    byte ptr [0x7990], 0          ; -> obj1:0x7990
        je     loc_1_1D85
        test   byte ptr [edi], 0x40
        jne    loc_1_1D85
        mov    ax, word ptr [esi + 4]
        test   ah, 0x20
        jne    loc_1_1D85
        or     ax, 0x2000
        mov    word ptr [esi + 4], ax
        mov    eax, dword ptr [esi + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_1D85
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x68]
        mov    dword ptr [eax + 4], 0x1000   ; -> loc_1_1000
        mov    eax, dword ptr [0x793c]       ; -> obj1:0x793C
        push   eax
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_3BE0
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_1D46:
        mov    eax, dword ptr [0x78e8]       ; -> obj1:0x78E8
        lea    ecx, [eax + eax*2]
        add    ecx, dword ptr [0x7864]       ; -> obj1:0x7864
        mov    dword ptr [esi + 0x74], ecx
        test   byte ptr [edi], 0x40
        je     loc_1_1D85
        mov    eax, dword ptr [esi + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_1D80
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   esi
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_0E00
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_1D80:
        or     word ptr [esi + 4], 2
loc_1_1D85:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1D90:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_3900
        add    esp, 4
        jmp    sub_1_2E80
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1DB0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_3860
        mov    edx, dword ptr [esp + 0x1c]
        mov    dword ptr [esp + 0x10], eax
        mov    ax, word ptr [edx + 0x22]
        lea    ebp, [edx + 0x1e]
        mov    si, ax
        and    esi, 0xffff00ff
        mov    word ptr [esp + 0x14], si
        mov    ecx, dword ptr [0x797c]       ; -> obj1:0x797C
        and    esi, 0xffff
        mov    ebx, dword ptr [ecx + esi*4]
        or     ebx, ebx
        je     loc_1_1FD0
        cmp    word ptr [ebx + 0x10], ax
        jne    loc_1_1FD0
        lea    edi, [ebx + 0x3c]
        lea    esi, [edx + 0x12]
        mov    ecx, 2
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_1E0F
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
loc_1_1E0F:
        je     loc_1_1E30
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_1E25
        mov    ecx, dword ptr [esp + 0x10]
        push   ecx
        call   sub_1_3900
        add    esp, 4
loc_1_1E25:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_1E30:
        mov    al, byte ptr [ebx + 6]
        cmp    al, 1
        jne    loc_1_1E50
        test   byte ptr [ebp], 0x80
        je     loc_1_1E50
        push   edx
        push   ebx
        call   sub_1_14C0
        jmp    loc_1_1FE8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1E50:
        mov    cx, word ptr [ebx + 0x12]
        cmp    word ptr [ebp + 2], cx
        jne    loc_1_1FEB
        cmp    al, 3
        jne    loc_1_1FEB
        test   byte ptr [ebp], 0x80
        je     loc_1_1E8A
        test   byte ptr [ebx + 5], 0x80
        push   ebp
        push   ebx
        je     loc_1_1E80
        call   sub_1_1650
        jmp    loc_1_1FE8
        db  08Bh, 0FFh
loc_1_1E80:
        call   sub_1_1990
        jmp    loc_1_1FE8
loc_1_1E8A:
        xor    eax, eax
        mov    ax, word ptr [edx + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        mov    dx, word ptr [ebp + 6]
        mov    cx, word ptr [ebx + 0x1c]
        cmp    dx, cx
        jne    loc_1_1F08
        xor    esi, esi
        mov    si, word ptr [ebp + 0xc]
        and    eax, 0xffff
        sub    eax, 0x30
        cmp    esi, eax
        ja     loc_1_1F08
        mov    ax, word ptr [ebx + 4]
        and    ax, 0xff7f
        cmp    dword ptr [esp + 0x10], 0
        mov    word ptr [ebx + 4], ax
        je     loc_1_1F25
        mov    ecx, dword ptr [esp + 0x10]
        mov    si, word ptr [esp + 0x14]
        mov    word ptr [ecx + 0x3e], si
        test   ah, 0x80
        mov    word ptr [ecx + 0x28], 0x30
        mov    dword ptr [ecx + 4], 0x5070   ; -> loc_1_5070
        je     loc_1_1EF7
        push   ecx
        push   ebp
        push   ebx
        call   sub_1_4EC0
        add    esp, 0xc
        jmp    loc_1_1F25
loc_1_1EF7:
        mov    ecx, dword ptr [esp + 0x10]
        push   ecx
        push   ebp
        push   ebx
        call   sub_1_4D40
        add    esp, 0xc
        jmp    loc_1_1F25
loc_1_1F08:
        sub    dx, cx
        cmp    dx, 8
        ja     loc_1_1F25
        mov    ax, word ptr [ebx + 4]
        test   al, 0x80
        jne    loc_1_1F25
        or     ax, 0x880
        mov    word ptr [ebx + 4], ax
        or     byte ptr [ebp], 0x40
loc_1_1F25:
        test   byte ptr [ebx + 5], 0x80
        je     loc_1_1F92
        push   ebp
        push   ebx
        call   sub_1_1650
        add    esp, 8
        cmp    byte ptr [0x7990], 0          ; -> obj1:0x7990
        je     loc_1_1FEB
        test   byte ptr [ebp], 0x40
        jne    loc_1_1FEB
        mov    ax, word ptr [ebx + 4]
        test   ah, 0x20
        jne    loc_1_1FEB
        or     ax, 0x2000
        mov    word ptr [ebx + 4], ax
        mov    eax, dword ptr [ebx + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_1FEB
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [ebx + 0x68]
        mov    dword ptr [eax + 4], 0x1000   ; -> loc_1_1000
        mov    eax, dword ptr [0x793c]       ; -> obj1:0x793C
        push   eax
        mov    eax, dword ptr [ebx + 0x68]
        push   eax
        call   sub_1_3BE0
        jmp    loc_1_1FE8
loc_1_1F92:
        mov    eax, dword ptr [0x78e8]       ; -> obj1:0x78E8
        lea    ecx, [eax + eax*2]
        add    ecx, dword ptr [0x7864]       ; -> obj1:0x7864
        mov    dword ptr [ebx + 0x74], ecx
        test   byte ptr [ebp], 0x40
        je     loc_1_1FEB
        mov    eax, dword ptr [ebx + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_1FC7
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   ebx
        mov    eax, dword ptr [ebx + 0x68]
        push   eax
        call   sub_1_0E00
        jmp    loc_1_1FE8
loc_1_1FC7:
        or     word ptr [ebx + 4], 2
        jmp    loc_1_1FEB
        db  08Bh, 0FFh
loc_1_1FD0:
        cmp    ax, 0xffff
        jne    loc_1_1FEB
        cmp    word ptr [ebp + 6], 0
        jne    loc_1_1FEB
        mov    eax, dword ptr [esp + 0x20]
        push   eax
        push   edx
        call   sub_1_11A0
loc_1_1FE8:
        add    esp, 8
loc_1_1FEB:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_200A
        mov    ecx, dword ptr [esp + 0x10]
        cmp    word ptr [ecx + 0x22], 1
        jne    loc_1_200A
        mov    word ptr [ecx + 0x22], 0
        mov    dword ptr [ecx + 4], 0x1d90   ; -> loc_1_1D90
loc_1_200A:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2020:
        push   ebx
        push   esi
        push   edi
        mov    edx, 0
        mov    ebx, 0x7820                   ; -> obj1:0x7820
        mov    eax, dword ptr [esp + 0x10]
loc_1_2031:
        mov    ecx, 4
        mov    esi, eax
        mov    edi, ebx
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        je     loc_1_2050
        add    ebx, 0x10
        inc    edx
        cmp    ebx, 0x7840                   ; -> obj1:0x7840
        jb     loc_1_2031
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_2050:
        mov    eax, dword ptr [edx*4 + 0x7600] ; -> obj1:0x7600
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_2060:
        push   esi
        mov    edx, dword ptr [0x7600]       ; -> obj1:0x7600
        sub    ecx, ecx
        mov    esi, 1
        mov    edi, edi
loc_1_2070:
        mov    eax, dword ptr [esi*4 + 0x7600] ; -> obj1:0x7600
        cmp    edx, eax
        jbe    loc_1_207F
        mov    ecx, esi
        mov    edx, eax
loc_1_207F:
        inc    esi
        cmp    esi, 2
        jl     loc_1_2070
        mov    edx, dword ptr [esp + 8]
        mov    esi, ecx
        shl    esi, 4
        add    esi, 0x7820                   ; -> obj1:0x7820
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        mov    dword ptr [ecx*4 + 0x7600], eax ; -> obj1:0x7600
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_20C0:
        push   esi
        push   edi
        mov    edx, 0
        mov    eax, 0x7820                   ; -> obj1:0x7820
        lea    esp, [esp]
loc_1_20D0:
        mov    esi, 0x75ca                   ; -> loc_1_75CA
        mov    ecx, 4
        mov    edi, eax
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        je     loc_1_20EB
        add    eax, 0x10
        inc    edx
        cmp    eax, 0x7840                   ; -> obj1:0x7840
        jb     loc_1_20D0
loc_1_20EB:
        mov    byte ptr [0x75f1], 0          ; -> loc_1_75F1
        cmp    edx, 2
        jge    loc_1_212C
        cmp    byte ptr [0x75c1], 0          ; -> loc_1_75C1
        jne    loc_1_2120
        mov    eax, edx
        shl    eax, 4
        mov    dword ptr [edx*4 + 0x7600], 0 ; -> obj1:0x7600
        mov    dword ptr [eax + 0x7820], 0x2a2a2a2a ; -> obj1:0x7820
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_2120:
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        mov    dword ptr [edx*4 + 0x7600], eax ; -> obj1:0x7600
loc_1_212C:
        pop    edi
        pop    esi
        ret    
        db  090h
sub_1_2130:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 3]
        mov    eax, dword ptr [0x7850]       ; -> obj1:0x7850
        mov    al, byte ptr [ecx + eax]
        and    eax, 0xff
        cmp    dword ptr [0x7984], eax       ; -> obj1:0x7984
        jbe    loc_1_2250
        mov    ecx, eax
        shl    ecx, 4
        add    ecx, eax
        mov    eax, dword ptr [0x78f4]       ; -> obj1:0x78F4
        test   byte ptr [eax + ecx*4 + 0x14], 4
        je     loc_1_2250
        mov    ax, word ptr [esi + 8]
        mov    cx, word ptr [0x78e4]         ; -> obj1:0x78E4
        cmp    cx, 0x200
        jb     loc_1_2182
        mov    cx, 0x200
loc_1_2182:
        cmp    ax, cx
        jbe    loc_1_2190
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_2190:
        sub    edi, edi
        cmp    byte ptr [0x7814], 0          ; -> obj1:0x7814
        je     loc_1_221A
        lea    ebx, [esi + 0xa]
        push   ebx
        call   sub_1_2020
        add    esp, 4
        cmp    eax, edi
        je     loc_1_2208
        cmp    byte ptr [0x75f1], 0          ; -> loc_1_75F1
        jne    loc_1_221A
        mov    ecx, dword ptr [0x7864]       ; -> obj1:0x7864
        sub    ecx, eax
        cmp    ecx, 0x7d0
        jbe    loc_1_221A
        mov    ecx, 0x75ca                   ; -> loc_1_75CA
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        mov    byte ptr [0x75f1], 0xff       ; -> loc_1_75F1
        push   0x75c0                        ; -> loc_1_75C0
        call   sub_1_2750
        add    esp, 4
        and    eax, 0xff
        cmp    eax, 0xff
        je     loc_1_221A
        mov    byte ptr [0x75f1], 0          ; -> loc_1_75F1
        jmp    loc_1_221A
loc_1_2208:
        mov    dword ptr [esi + 0x3c], 0x25e0 ; -> loc_1_25E0
        push   esi
        call   sub_1_27D0
        add    esp, 4
        mov    edi, eax
loc_1_221A:
        or     edi, edi
        jne    loc_1_2248
        mov    dword ptr [esi + 0x3c], 0
        mov    dword ptr [esi + 0x34], 0x7848 ; -> obj1:0x7848
        lea    ecx, [esi + 0x34]
        mov    eax, dword ptr [0x784c]       ; -> obj1:0x784C
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x784c]       ; -> obj1:0x784C
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x784c], ecx       ; -> obj1:0x784C
        mov    byte ptr [esi + 0x32], 0xc
loc_1_2248:
        mov    al, 0xff
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_2250:
        mov    al, 0x13
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2260:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    al, byte ptr [ecx + 3]
        cmp    al, 0xff
        jne    loc_1_2293
        mov    dword ptr [ecx + 0x34], 0x796c ; -> obj1:0x796C
        lea    edx, [ecx + 0x34]
        mov    eax, dword ptr [0x7970]       ; -> obj1:0x7970
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x7970]       ; -> obj1:0x7970
        pop    esi
        mov    dword ptr [eax], edx
        mov    al, 0xff
        mov    dword ptr [0x7970], edx       ; -> obj1:0x7970
        mov    byte ptr [ecx + 0x32], 4
        ret    
loc_1_2293:
        xor    edx, edx
        mov    dl, al
        mov    eax, dword ptr [0x7850]       ; -> obj1:0x7850
        mov    al, byte ptr [edx + eax]
        and    eax, 0xff
        cmp    dword ptr [0x7984], eax       ; -> obj1:0x7984
        jbe    loc_1_22DC
        mov    edx, eax
        shl    edx, 4
        add    edx, eax
        mov    eax, dword ptr [0x78f4]       ; -> obj1:0x78F4
        lea    edx, [eax + edx*4]
        test   byte ptr [edx + 0x14], 4
        je     loc_1_22DC
        lea    eax, [ecx + 0x34]
        add    edx, 0x20
        mov    dword ptr [eax], edx
        mov    esi, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [esi], eax
        mov    dword ptr [edx + 4], eax
        mov    al, 0xff
        mov    byte ptr [ecx + 0x32], 4
        pop    esi
        ret    
loc_1_22DC:
        mov    al, 0x13
        pop    esi
        ret    
loc_1_22E0:
        sub    esp, 0x10
        push   esi
        mov    ecx, dword ptr [esp + 0x18]
        lea    edx, [esp + 4]
        mov    eax, dword ptr [ecx + 0xa]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 0xe]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 0x12]
        lea    esi, [ecx + 0xa]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [ecx + 0x16], 0
        mov    dword ptr [ecx + 0x12], 0
        mov    dword ptr [ecx + 0xe], 0
        mov    dword ptr [esi], 0
        mov    byte ptr [esi], 0x2a
        push   ecx
        call   sub_1_2130
        lea    edx, [esp + 8]
        add    esp, 4
        mov    ecx, dword ptr [edx]
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], ecx
        mov    ecx, dword ptr [edx + 8]
        mov    dword ptr [esi + 8], ecx
        mov    ecx, dword ptr [edx + 0xc]
        mov    dword ptr [esi + 0xc], ecx
        pop    esi
        add    esp, 0x10
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_2350:
        mov    edx, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 3]
        mov    eax, dword ptr [0x7850]       ; -> obj1:0x7850
        mov    al, byte ptr [ecx + eax]
        and    eax, 0xff
        cmp    dword ptr [0x7984], eax       ; -> obj1:0x7984
        jbe    loc_1_23A7
        mov    ecx, eax
        shl    ecx, 4
        add    ecx, eax
        mov    eax, dword ptr [0x78f4]       ; -> obj1:0x78F4
        test   byte ptr [eax + ecx*4 + 0x14], 4
        je     loc_1_23A7
        mov    dword ptr [edx + 0x34], 0x7918 ; -> obj1:0x7918
        lea    ecx, [edx + 0x34]
        mov    eax, dword ptr [0x791c]       ; -> obj1:0x791C
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x791c]       ; -> obj1:0x791C
        mov    dword ptr [eax], ecx
        mov    al, 0xff
        mov    dword ptr [0x791c], ecx       ; -> obj1:0x791C
        mov    byte ptr [edx + 0x32], 4
        ret    
loc_1_23A7:
        mov    al, 0x13
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_23B0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 0x32], 4
        jne    loc_1_23D0
        lea    eax, [esi + 0x34]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 1], 0xb
        sub    al, al
        pop    esi
        ret    
        db  090h
loc_1_23D0:
        mov    al, 0x26
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_23E0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 0x3c], 0x20
        je     loc_1_2425
        mov    ecx, dword ptr [esi + 0x40]
        or     ecx, ecx
        je     loc_1_2425
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_2425
        mov    byte ptr [ecx + 1], 0
        test   byte ptr [ecx], 0x80
        je     loc_1_2420
        mov    dword ptr [ecx + 0x34], 0x7890 ; -> obj1:0x7890
        add    ecx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x7894], ecx       ; -> obj1:0x7894
loc_1_2420:
        call   sub_1_2E80
loc_1_2425:
        push   esi
        call   sub_1_3820
        add    esp, 4
        pop    esi
        ret    
sub_1_2430:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 0x18]
        lea    esi, [ecx + 0x1c]
        mov    word ptr [ecx + 0x28], 0x40
        mov    edx, dword ptr [eax]
        mov    dword ptr [esi], edx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [esi + 4], ax
        mov    dword ptr [ecx + 4], 0x23e0   ; -> loc_1_23E0
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x40], edx
        or     word ptr [ecx + 0x3c], 0x20
        mov    word ptr [ecx + 0x22], 2
        add    ecx, 0x4c
        mov    ax, word ptr [edx + 8]
        mov    word ptr [ecx - 0x1e], ax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx - 0x22], eax
        mov    byte ptr [ecx + 5], 4
        mov    esi, dword ptr [esp + 0x14]
        lea    edi, [ecx + 6]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 4], eax
        add    ecx, 0x1e
        mov    ax, word ptr [esi + 8]
        mov    word ptr [edi + 8], ax
        movzx  esi, byte ptr [edx + 3]
        mov    eax, dword ptr [0x7850]       ; -> obj1:0x7850
        lea    edi, [ecx + 2]
        mov    al, byte ptr [esi + eax]
        and    eax, 0xff
        cmp    dword ptr [0x7984], eax       ; -> obj1:0x7984
        jbe    loc_1_24C4
        mov    esi, eax
        shl    esi, 4
        add    esi, eax
        mov    eax, dword ptr [0x78f4]       ; -> obj1:0x78F4
        lea    edi, [eax + esi*4]
loc_1_24C4:
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 6], eax
        lea    esi, [ecx + 2]
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [esi + 0xc], eax
        mov    al, byte ptr [edx]
        and    eax, 0x7f
        cmp    eax, 0x20
        jne    loc_1_2510
        lea    esi, [ecx + 0x12]
        mov    eax, dword ptr [edx + 0xa]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 0xe]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edx + 0x12]
        add    edx, 0xa
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [esi + 0xc], eax
        mov    byte ptr [ecx + 1], 0xb
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_2510:
        mov    byte ptr [ecx + 1], 0xc
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2520:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0x40]
        or     edi, edi
        je     loc_1_25CD
        cmp    byte ptr [edi + 1], 0xff
        jne    loc_1_25CD
        mov    edx, dword ptr [edi + 0x3c]
        or     edx, edx
        je     loc_1_2587
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 0x52], eax
        lea    eax, [edx + 8]
        lea    ebx, [esi + 0x1c]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebx + 4], ax
        add    esi, 0x44
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi + 0x3c], eax
        mov    dword ptr [esi], 0x78a0       ; -> obj1:0x78A0
        mov    eax, dword ptr [0x78a4]       ; -> obj1:0x78A4
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x78a4]       ; -> obj1:0x78A4
        mov    dword ptr [eax], esi
        mov    dword ptr [0x78a4], esi       ; -> obj1:0x78A4
        call   sub_1_2E80
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_2587:
        lea    eax, [edi + 0xa]
        push   eax
        push   0x78c0                        ; -> obj1:0x78C0
        call   sub_1_2780
        add    esp, 8
        or     eax, eax
        je     loc_1_25A0
        dec    word ptr [eax + 0x2a]
loc_1_25A0:
        mov    byte ptr [edi + 1], 0
        test   byte ptr [edi], 0x80
        je     loc_1_25C8
        mov    dword ptr [edi + 0x34], 0x7890 ; -> obj1:0x7890
        add    edi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], edi
        mov    dword ptr [0x7894], edi       ; -> obj1:0x7894
loc_1_25C8:
        call   sub_1_2E80
loc_1_25CD:
        push   esi
        call   sub_1_3820
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_25E0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        or     eax, eax
        mov    dword ptr [esi + 0x3c], eax
        je     loc_1_25F6
        inc    word ptr [eax + 0x2a]
        jmp    loc_1_2602
loc_1_25F6:
        lea    eax, [esi + 0xa]
        push   eax
        call   sub_1_2060
        add    esp, 4
loc_1_2602:
        mov    dword ptr [esi + 0x34], 0x7848 ; -> obj1:0x7848
        lea    ecx, [esi + 0x34]
        mov    eax, dword ptr [0x784c]       ; -> obj1:0x784C
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x784c]       ; -> obj1:0x784C
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x784c], ecx       ; -> obj1:0x784C
        mov    byte ptr [esi + 0x32], 0xc
        pop    esi
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2630:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [0x7848], 0x7848    ; -> obj1:0x7848
        je     loc_1_273B
        mov    ebx, 0x7848                   ; -> obj1:0x7848
        lea    esp, [esp]
loc_1_2650:
        call   sub_1_37F0
        mov    esi, eax
        or     esi, esi
        je     loc_1_273B
        cmp    dword ptr [0x7848], ebx       ; -> obj1:0x7848
        mov    edi, 0
        je     loc_1_2683
        mov    edi, dword ptr [0x7848]       ; -> obj1:0x7848
        sub    edi, 0x34
        mov    eax, dword ptr [0x7848]       ; -> obj1:0x7848
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_2683:
        mov    byte ptr [edi + 0x32], 0
        mov    ebp, dword ptr [edi + 0x3c]
        or     ebp, ebp
        jne    loc_1_26A7
        push   0x78ac                        ; -> obj1:0x78AC
        push   0x78a8                        ; -> obj1:0x78A8
        push   edi
        push   esi
        call   sub_1_2430
        add    esp, 0x10
        jmp    loc_1_2726
loc_1_26A7:
        test   byte ptr [ebp + 0x28], 0x80
        je     loc_1_2710
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        je     loc_1_26C5
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi + 0x3c], eax
        jmp    loc_1_26D5
loc_1_26C5:
        mov    eax, dword ptr [0x7920]       ; -> obj1:0x7920
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [edi + 0x3c], 0
loc_1_26D5:
        mov    dword ptr [esp + 0x14], 0xffffffff
        mov    word ptr [esp + 0x18], 0xffff
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        je     loc_1_26F0
        add    eax, 8
        jmp    loc_1_26F5
loc_1_26F0:
        mov    eax, 0x78ac                   ; -> obj1:0x78AC
loc_1_26F5:
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        push   edi
        push   esi
        call   sub_1_2430
        add    esp, 0x10
        mov    dword ptr [esi + 4], 0x2520   ; -> loc_1_2520
        jmp    loc_1_2726
        db  08Bh, 0FFh
loc_1_2710:
        lea    eax, [ebp + 0x1a]
        push   eax
        lea    eax, [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_1_2430
        add    esp, 0x10
        dec    word ptr [ebp + 0x2a]
loc_1_2726:
        push   esi
        call   sub_1_0292
        add    esp, 4
        cmp    dword ptr [0x7848], ebx       ; -> obj1:0x7848
        jne    loc_1_2650
loc_1_273B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2750:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 8], 4
        jae    loc_1_2760
        mov    al, 1
        ret    
        db  08Bh, 0FFh
loc_1_2760:
        mov    dword ptr [eax + 0x3c], 0x2a50 ; -> loc_1_2A50
        push   eax
        call   sub_1_27D0
        add    esp, 4
        or     eax, eax
        jne    loc_1_2777
        mov    al, 9
        ret    
loc_1_2777:
        mov    al, 0xff
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2780:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [edx + 4]
        cmp    ebp, edx
        je     loc_1_27B0
        mov    ebx, dword ptr [esp + 0x1c]
loc_1_2796:
        lea    eax, [ebp - 0x20]
        mov    ecx, 4
        mov    esi, ebx
        mov    edi, eax
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        je     loc_1_27B4
        mov    ebp, dword ptr [ebp + 4]
        cmp    ebp, edx
        jne    loc_1_2796
        jmp    loc_1_27B4
        db  090h
loc_1_27B0:
        mov    eax, dword ptr [esp + 0x10]
loc_1_27B4:
        cmp    ebp, edx
        jne    loc_1_27BA
        sub    eax, eax
loc_1_27BA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_27D0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        lea    ebx, [edi + 0xa]
        push   ebx
        push   0x78c0                        ; -> obj1:0x78C0
        call   sub_1_2780
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_1_2800
        push   esi
        push   edi
        call   dword ptr [edi + 0x3c]
        add    esp, 8
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_2800:
        push   ebx
        push   0x78ec                        ; -> obj1:0x78EC
        call   sub_1_2780
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_1_2831
        lea    ecx, [edi + 0x34]
        lea    edx, [esi + 0x34]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        mov    eax, esi
        mov    dword ptr [edx + 4], ecx
        mov    byte ptr [edi + 0x32], 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_2831:
        cmp    dword ptr [0x78cc], 0x78cc    ; -> obj1:0x78CC
        mov    esi, 0
        je     loc_1_2859
        mov    esi, dword ptr [0x78cc]       ; -> obj1:0x78CC
        sub    esi, 0x20
        mov    eax, dword ptr [0x78cc]       ; -> obj1:0x78CC
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_2859:
        or     esi, esi
        jne    loc_1_28B9
        mov    eax, 0x78c0                   ; -> obj1:0x78C0
        sub    ecx, ecx
loc_1_2864:
        mov    eax, dword ptr [eax]
        cmp    eax, 0x78c0                   ; -> obj1:0x78C0
        je     loc_1_288D
        cmp    word ptr [eax + 0xa], cx
        lea    esi, [eax - 0x20]
        jne    loc_1_2864
        mov    eax, dword ptr [0x78c0]       ; -> obj1:0x78C0
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   sub_1_2BC0
        add    esp, 4
loc_1_288D:
        cmp    dword ptr [0x78cc], 0x78cc    ; -> obj1:0x78CC
        mov    esi, 0
        je     loc_1_28B5
        mov    esi, dword ptr [0x78cc]       ; -> obj1:0x78CC
        sub    esi, 0x20
        mov    eax, dword ptr [0x78cc]       ; -> obj1:0x78CC
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_28B5:
        or     esi, esi
        je     loc_1_2926
loc_1_28B9:
        mov    word ptr [esi + 0x2a], 1
        mov    dword ptr [esi + 0x10], 0
        mov    byte ptr [esi + 0x28], 0x40
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi], eax
        lea    ecx, [esi + 0x34]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 8], eax
        lea    edx, [edi + 0x34]
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [ecx], ecx
        mov    dword ptr [esi + 0x38], ecx
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        lea    ecx, [esi + 0x20]
        mov    byte ptr [edi + 0x32], 8
        mov    dword ptr [ecx], 0x78ec       ; -> obj1:0x78EC
        mov    eax, dword ptr [0x78f0]       ; -> obj1:0x78F0
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x78f0]       ; -> obj1:0x78F0
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x78f0], ecx       ; -> obj1:0x78F0
        mov    al, byte ptr [0x7854]         ; -> obj1:0x7854
        mov    byte ptr [esi + 0x29], al
        or     byte ptr [esi + 0x28], 8
loc_1_2926:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_2930:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0xc]
        and    byte ptr [esi + 0x28], 0xf7
        lea    ecx, [edx + 0x4c]
        mov    dword ptr [edx + 4], 0x2ba0   ; -> loc_1_2BA0
        mov    dword ptr [edx + 0x40], esi
        mov    dword ptr [edx + 0x1c], 0xffffffff
        mov    word ptr [edx + 0x20], 0xffff
        mov    byte ptr [ecx + 5], 4
        mov    dword ptr [ecx + 0xa], 0xffffffff
        mov    word ptr [ecx + 0xe], 0xffff
        add    ecx, 0x1e
        mov    eax, dword ptr [0x7920]       ; -> obj1:0x7920
        mov    dword ptr [ecx - 0x18], eax
        mov    byte ptr [ecx + 0x21], 1
        mov    byte ptr [ecx + 1], 1
        mov    byte ptr [ecx + 0x20], 0
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx + 0x22], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 0x26], eax
        add    ecx, 0x22
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        mov    word ptr [edx + 0x28], 0x50
        push   edx
        call   sub_1_0292
        add    esp, 4
        dec    byte ptr [esi + 0x29]
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_29B0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    al, byte ptr [esi + 0x28]
        and    al, 0x80
        cmp    al, 1
        sbb    ecx, ecx
        and    ecx, 0xffff92a0
        add    ecx, 0x7530
        add    ecx, dword ptr [0x7864]       ; -> obj1:0x7864
        mov    dword ptr [esi + 0x2c], ecx
        or     byte ptr [esi + 0x28], 0x20
        mov    eax, dword ptr [0x78c4]       ; -> obj1:0x78C4
        mov    ecx, eax
        sub    ecx, 0x20
        cmp    eax, 0x78c0                   ; -> obj1:0x78C0
        je     loc_1_2A02
        mov    edx, dword ptr [esi + 0x2c]
loc_1_29F0:
        cmp    dword ptr [ecx + 0x2c], edx
        jbe    loc_1_2A02
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 0x78c0                   ; -> obj1:0x78C0
        lea    ecx, [eax - 0x20]
        jne    loc_1_29F0
loc_1_2A02:
        push   eax
        lea    eax, [esi + 0x20]
        push   eax
        call   sub_1_3B30
        add    esp, 8
        sub    ebx, ebx
loc_1_2A11:
        mov    eax, dword ptr [esi + 0x34]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, 0x34
        mov    edi, ebx
        je     loc_1_2A2B
        lea    edi, [eax - 0x34]
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_2A2B:
        cmp    edi, ebx
        je     loc_1_2A40
        mov    byte ptr [edi + 0x32], bl
        push   esi
        push   edi
        call   dword ptr [edi + 0x3c]
        add    esp, 8
        jmp    loc_1_2A11
        db  08Dh, 064h, 024h, 000h
loc_1_2A40:
        dec    word ptr [esi + 0x2a]
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2A50:
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        mov    edx, dword ptr [esp + 4]
        je     loc_1_2A92
        mov    ecx, dword ptr [edx + 4]
        mov    word ptr [ecx], 1
        test   byte ptr [eax + 0x28], 0x80
        je     loc_1_2A82
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_1_2A7C
loc_1_2A73:
        inc    word ptr [ecx]
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_2A73
loc_1_2A7C:
        mov    byte ptr [ecx + 3], 1
        jmp    loc_1_2A86
loc_1_2A82:
        mov    byte ptr [ecx + 3], 0
loc_1_2A86:
        mov    word ptr [edx + 8], 4
        mov    byte ptr [edx + 1], 0
        jmp    loc_1_2A96
loc_1_2A92:
        mov    byte ptr [edx + 1], 5
loc_1_2A96:
        test   byte ptr [edx], 0x80
        je     loc_1_2ABA
        mov    dword ptr [edx + 0x34], 0x7890 ; -> obj1:0x7890
        add    edx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], edx
        mov    dword ptr [0x7894], edx       ; -> obj1:0x7894
loc_1_2ABA:
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_2AC0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        sub    ebx, ebx
loc_1_2AC9:
        mov    eax, dword ptr [esi + 0x34]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, 0x34
        mov    edi, ebx
        je     loc_1_2AE3
        lea    edi, [eax - 0x34]
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_2AE3:
        cmp    edi, ebx
        je     loc_1_2AF4
        mov    byte ptr [edi + 0x32], bl
        push   ebx
        push   edi
        call   dword ptr [edi + 0x3c]
        add    esp, 8
        jmp    loc_1_2AC9
loc_1_2AF4:
        dec    word ptr [esi + 0x2a]
        push   esi
        call   sub_1_2BC0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2B10:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0x40]
        or     esi, esi
        je     loc_1_2B94
        mov    cl, byte ptr [esi + 0x28]
        test   cl, 0x40
        je     loc_1_2B94
        test   cl, 0x20
        jne    loc_1_2B94
        mov    cl, byte ptr [esi + 0x29]
        or     cl, cl
        jne    loc_1_2B70
        push   eax
        call   sub_1_3820
        add    esp, 4
        lea    eax, [esi + 0x20]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    al, byte ptr [esi + 0x28]
        and    al, 0xc7
        cmp    al, 0xc4
        push   esi
        jne    loc_1_2B60
        call   sub_1_29B0
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2B60:
        call   sub_1_2AC0
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2B70:
        mov    dword ptr [eax + 4], 0x2ba0   ; -> loc_1_2BA0
        cmp    byte ptr [0x7814], 0          ; -> obj1:0x7814
        je     loc_1_2B84
        mov    byte ptr [eax + 0x51], 0x14
loc_1_2B84:
        dec    cl
        mov    byte ptr [esi + 0x29], cl
        push   eax
        call   sub_1_0292
        add    esp, 4
        pop    esi
        ret    
loc_1_2B94:
        push   eax
        call   sub_1_3820
        add    esp, 4
        pop    esi
        ret    
        db  090h
loc_1_2BA0:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], 0x2b10   ; -> loc_1_2B10
        mov    ecx, dword ptr [0x7948]       ; -> obj1:0x7948
        push   ecx
        push   eax
        call   sub_1_3BE0
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_2BC0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 0x28], 0x80
        je     loc_1_2BED
        mov    edx, dword ptr [esi + 0x10]
        or     edx, edx
        je     loc_1_2BED
        mov    ecx, dword ptr [0x7964]       ; -> obj1:0x7964
loc_1_2BD8:
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 0x10], eax
        mov    dword ptr [edx], ecx
        mov    ecx, edx
        mov    dword ptr [0x7964], ecx       ; -> obj1:0x7964
        mov    edx, eax
        or     edx, edx
        jne    loc_1_2BD8
loc_1_2BED:
        mov    byte ptr [esi + 0x28], 0
        mov    dword ptr [esi + 0x20], 0x78cc ; -> obj1:0x78CC
        add    esi, 0x20
        mov    eax, dword ptr [0x78d0]       ; -> obj1:0x78D0
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x78d0]       ; -> obj1:0x78D0
        mov    dword ptr [eax], esi
        mov    dword ptr [0x78d0], esi       ; -> obj1:0x78D0
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2C20:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0x78c0], 0x78c0    ; -> obj1:0x78C0
        je     loc_1_2C78
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        sub    edi, edi
        mov    ebx, 0x78c0                   ; -> obj1:0x78C0
        add    eax, 0
loc_1_2C40:
        mov    esi, dword ptr [0x78c0]       ; -> obj1:0x78C0
        sub    esi, 0x20
        cmp    dword ptr [esi + 0x2c], eax
        ja     loc_1_2C78
        cmp    word ptr [esi + 0x2a], di
        jne    loc_1_2C78
        mov    eax, dword ptr [0x78c0]       ; -> obj1:0x78C0
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   sub_1_2BC0
        add    esp, 4
        cmp    dword ptr [0x78c0], ebx       ; -> obj1:0x78C0
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        jne    loc_1_2C40
loc_1_2C78:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_2C80:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        mov    esi, dword ptr [edx + 0x10]
        mov    eax, esi
        or     eax, eax
        je     loc_1_2C9F
loc_1_2C94:
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_2C9F
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_2C94
loc_1_2C9F:
        or     eax, eax
        jne    loc_1_2CD3
        mov    edi, dword ptr [0x7964]       ; -> obj1:0x7964
        or     edi, edi
        je     loc_1_2CD3
        mov    eax, dword ptr [edi]
        mov    dword ptr [0x7964], eax       ; -> obj1:0x7964
        or     edi, edi
        je     loc_1_2CD3
        mov    dword ptr [edi + 4], ecx
        mov    eax, dword ptr [esp + 0x18]
        lea    ebx, [edi + 8]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebx + 4], ax
        mov    dword ptr [edi], esi
        mov    dword ptr [edx + 0x10], edi
loc_1_2CD3:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2CE0:
        sub    esp, 4
        push   ebx
        push   esi
        mov    bl, 0xff
        test   byte ptr [0x78b5], 0x80       ; -> loc_1_78B5
        jne    loc_1_2CF2
        mov    bl, 0xfe
loc_1_2CF2:
        mov    esi, dword ptr [esp + 0x10]
        mov    al, byte ptr [esi]
        mov    byte ptr [esp + 0xb], al
        and    eax, 0xff
        mov    ecx, eax
        and    ecx, 0xffffff7f
        cmp    ecx, 0x37
        jb     loc_1_2DA0
        and    eax, 0xffffff7f
        cmp    eax, 0x72
        jg     loc_1_2D32
        je     loc_1_2D70
        cmp    eax, 0x48
        je     loc_1_2D40
        cmp    eax, 0x70
        je     loc_1_2D50
        cmp    eax, 0x71
        je     loc_1_2D60
        jmp    loc_1_2DB3
loc_1_2D32:
        cmp    eax, 0x78
        je     loc_1_2D80
        cmp    eax, 0x79
        je     loc_1_2D90
        jmp    loc_1_2DB3
        db  08Bh, 0FFh
loc_1_2D40:
        mov    dword ptr [esi + 0x3c], 0x3d50 ; -> sub_1_3D50
        jmp    loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D50:
        mov    dword ptr [esi + 0x3c], 0x3230 ; -> loc_1_3230
        jmp    loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D60:
        mov    dword ptr [esi + 0x3c], 0x3e30 ; -> loc_1_3E30
        jmp    loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D70:
        mov    dword ptr [esi + 0x3c], 0x3e40 ; -> loc_1_3E40
        jmp    loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D80:
        mov    dword ptr [esi + 0x3c], 0x2750 ; -> sub_1_2750
        jmp    loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D90:
        mov    dword ptr [esi + 0x3c], 0x3220 ; -> obj1:0x3220
        jmp    loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2DA0:
        and    eax, 0xffffff7f
        mov    eax, dword ptr [eax*4 + 0x7608] ; -> obj1:0x7608
        mov    dword ptr [esi + 0x3c], eax
        or     eax, eax
        jne    loc_1_2DB5
loc_1_2DB3:
        mov    bl, 3
loc_1_2DB5:
        mov    byte ptr [esi + 0x32], 0
        mov    byte ptr [esi + 0x33], 0
        mov    byte ptr [esi + 1], bl
        mov    byte ptr [esi + 0x31], bl
        cmp    bl, 0xff
        jne    loc_1_2E71
        call   sub_1_0081
        or     eax, eax
        je     loc_1_2DF8
        push   esi
        call   dword ptr [esi + 0x3c]
        add    esp, 4
        mov    bl, al
        mov    eax, dword ptr [0x7714]       ; -> obj1:0x7714
        dec    eax
        mov    dword ptr [0x7714], eax       ; -> obj1:0x7714
        cmp    bl, 0xff
        je     loc_1_2DF1
        mov    byte ptr [esi + 1], bl
loc_1_2DF1:
        call   sub_1_2E80
        jmp    loc_1_2E1B
loc_1_2DF8:
        mov    dword ptr [esi + 0x34], 0x7884 ; -> obj1:0x7884
        lea    ecx, [esi + 0x34]
        mov    eax, dword ptr [0x7888]       ; -> obj1:0x7888
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x7888]       ; -> obj1:0x7888
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x7888], ecx       ; -> obj1:0x7888
        mov    byte ptr [esi + 0x32], 1
loc_1_2E1B:
        test   byte ptr [esp + 0xb], 0x80
        je     loc_1_2E55
        cmp    bl, 0xff
        je     loc_1_2E4B
        mov    dword ptr [esi + 0x34], 0x7890 ; -> obj1:0x7890
        add    esi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], esi
        mov    dword ptr [0x7894], esi       ; -> obj1:0x7894
        call   sub_1_2E80
loc_1_2E4B:
        sub    bl, bl
        mov    al, bl
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_2E55:
        test   byte ptr [esi + 1], 0x80
        je     loc_1_2E6B
        add    eax, 0
loc_1_2E60:
        call   sub_1_00B1
        test   byte ptr [esi + 1], 0x80
        jne    loc_1_2E60
loc_1_2E6B:
        mov    bl, byte ptr [esi + 1]
        mov    byte ptr [esi + 0x31], bl
loc_1_2E71:
        mov    al, bl
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2E80:
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_0081
        or     eax, eax
        je     loc_1_302B
        mov    edi, 0x7858                   ; -> obj1:0x7858
        mov    ebx, 0x7884                   ; -> obj1:0x7884
        mov    ebp, 0x78a0                   ; -> obj1:0x78A0
        cmp    dword ptr [0x7858], edi       ; -> obj1:0x7858
        je     loc_1_2EEB
loc_1_2EA8:
        mov    esi, dword ptr [0x7858]       ; -> obj1:0x7858
        sub    esi, 0x44
        mov    eax, dword ptr [0x7858]       ; -> obj1:0x7858
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   dword ptr [esi + 4]
        add    esp, 4
        test   byte ptr [esi + 0x3c], 1
        je     loc_1_2EE3
        push   esi
        call   sub_1_3930
        add    esp, 4
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_2EE3
        call   sub_1_39A0
loc_1_2EE3:
        cmp    dword ptr [0x7858], edi       ; -> obj1:0x7858
        jne    loc_1_2EA8
loc_1_2EEB:
        and    dword ptr [0x771c], 0xfffffffe ; -> obj1:0x771C
        call   sub_1_42F0
        cmp    dword ptr [0x7884], ebx       ; -> obj1:0x7884
        je     loc_1_2F55
        nop    
loc_1_2F00:
        mov    esi, dword ptr [0x7884]       ; -> obj1:0x7884
        mov    byte ptr [esi - 2], 0
        sub    esi, 0x34
        mov    eax, dword ptr [0x7884]       ; -> obj1:0x7884
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   dword ptr [esi + 0x3c]
        add    esp, 4
        cmp    al, 0xff
        je     loc_1_2F4D
        mov    byte ptr [esi + 1], al
        test   byte ptr [esi], 0x80
        je     loc_1_2F4D
        mov    dword ptr [esi + 0x34], 0x7890 ; -> obj1:0x7890
        add    esi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], esi
        mov    dword ptr [0x7894], esi       ; -> obj1:0x7894
loc_1_2F4D:
        cmp    dword ptr [0x7884], ebx       ; -> obj1:0x7884
        jne    loc_1_2F00
loc_1_2F55:
        call   sub_1_2630
        cmp    dword ptr [0x78a0], ebp       ; -> obj1:0x78A0
        je     loc_1_2F8A
loc_1_2F62:
        mov    esi, dword ptr [0x78a0]       ; -> obj1:0x78A0
        sub    esi, 0x44
        mov    eax, dword ptr [0x78a0]       ; -> obj1:0x78A0
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   sub_1_0292
        add    esp, 4
        cmp    dword ptr [0x78a0], ebp       ; -> obj1:0x78A0
        jne    loc_1_2F62
loc_1_2F8A:
        cmp    dword ptr [0x7858], edi       ; -> obj1:0x7858
        jne    loc_1_2EA8
        dec    dword ptr [0x7714]            ; -> obj1:0x7714
        call   sub_1_0099
        or     eax, eax
        je     loc_1_302B
        cmp    dword ptr [0x78f8], 0x78f8    ; -> obj1:0x78F8
        je     loc_1_2FBA
        call   sub_1_3030
loc_1_2FBA:
        mov    edi, 0x7890                   ; -> obj1:0x7890
        sub    ebx, ebx
loc_1_2FC1:
        cmp    dword ptr [0x7890], edi       ; -> obj1:0x7890
        mov    esi, ebx
        je     loc_1_2FE2
        mov    esi, dword ptr [0x7890]       ; -> obj1:0x7890
        sub    esi, 0x34
        mov    eax, dword ptr [0x7890]       ; -> obj1:0x7890
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_2FE2:
        cmp    esi, ebx
        je     loc_1_3020
        cmp    byte ptr [esi + 0x33], bl
        je     loc_1_3010
        mov    dword ptr [esi + 0x34], 0x78f8 ; -> obj1:0x78F8
        add    esi, 0x34
        mov    eax, dword ptr [0x78fc]       ; -> obj1:0x78FC
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x78fc]       ; -> obj1:0x78FC
        mov    dword ptr [eax], esi
        mov    dword ptr [0x78fc], esi       ; -> obj1:0x78FC
        jmp    loc_1_2FC1
        db  08Dh, 064h, 024h, 000h
loc_1_3010:
        push   esi
        call   sub_1_0151
        add    esp, 4
        jmp    loc_1_2FC1
        db  005h, 000h, 000h, 000h, 000h
loc_1_3020:
        mov    eax, dword ptr [0x7718]       ; -> obj1:0x7718
        dec    eax
        mov    dword ptr [0x7718], eax       ; -> obj1:0x7718
loc_1_302B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_3030:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x78f8]       ; -> obj1:0x78F8
        cmp    eax, 0x78f8                   ; -> obj1:0x78F8
        je     loc_1_3067
        sub    ebx, ebx
loc_1_3041:
        mov    edi, dword ptr [eax]
        cmp    byte ptr [eax - 1], bl
        lea    esi, [eax - 0x34]
        jne    loc_1_305D
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   sub_1_0151
        add    esp, 4
loc_1_305D:
        mov    eax, edi
        cmp    edi, 0x78f8                   ; -> obj1:0x78F8
        jne    loc_1_3041
loc_1_3067:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_3070:
        call   sub_1_70E0
        mov    eax, dword ptr [0x7890]       ; -> obj1:0x7890
        cmp    eax, 0x7890                   ; -> obj1:0x7890
        je     loc_1_3093
loc_1_3081:
        mov    dl, byte ptr [eax - 0x33]
        mov    byte ptr [eax - 3], dl
        lea    ecx, [eax - 0x34]
        mov    eax, dword ptr [eax]
        cmp    eax, 0x7890                   ; -> obj1:0x7890
        jne    loc_1_3081
loc_1_3093:
        mov    dword ptr [0x7890], 0x7890    ; -> obj1:0x7890
        mov    dword ptr [0x7894], 0x7890    ; -> obj1:0x7894
        call   sub_1_73F0
        sub    al, al
        ret    
        db  090h
sub_1_30B0:
        mov    al, 0x24
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_30C0:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax + 4]
        cmp    byte ptr [esi + 1], 0xff
        je     loc_1_30D6
        mov    al, 0x24
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_30D6:
        mov    al, byte ptr [esi]
        and    eax, 0x7f
        cmp    eax, 0x33
        jg     loc_1_30F3
        je     loc_1_3131
        sub    eax, 0x10
        cmp    eax, 0x13
        ja     loc_1_30FD
        shl    eax, 2
        jmp    loc_1_31CC
loc_1_30F3:
        cmp    eax, 0x71
        jl     loc_1_30FD
        cmp    eax, 0x72
        jle    loc_1_3119
loc_1_30FD:
        mov    bl, 0x26
        jmp    loc_1_313C
loc_1_3101:
        push   esi
        call   sub_1_0450
        jmp    loc_1_3137
loc_1_3109:
        push   esi
        call   sub_1_0410
        jmp    loc_1_3137
loc_1_3111:
        push   esi
        call   sub_1_30B0
        jmp    loc_1_3137
loc_1_3119:
        push   esi
        call   sub_1_3E50
        jmp    loc_1_3137
loc_1_3121:
        push   esi
        call   sub_1_4830
        jmp    loc_1_3137
loc_1_3129:
        push   esi
        call   sub_1_23B0
        jmp    loc_1_3137
loc_1_3131:
        push   esi
        call   sub_1_6660
loc_1_3137:
        add    esp, 4
        mov    bl, al
loc_1_313C:
        cmp    bl, 0x26
        jne    loc_1_315D
        cmp    byte ptr [esi + 0x32], 1
        jne    loc_1_315D
        mov    byte ptr [esi + 0x32], 0
        lea    eax, [esi + 0x34]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 1], 0xb
        sub    bl, bl
loc_1_315D:
        or     bl, bl
        jne    loc_1_31C3
        test   byte ptr [esi], 0x80
        je     loc_1_3185
        mov    dword ptr [esi + 0x34], 0x7890 ; -> obj1:0x7890
        add    esi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], esi
        mov    dword ptr [0x7894], esi       ; -> obj1:0x7894
loc_1_3185:
        mov    edi, 0x7890                   ; -> obj1:0x7890
        lea    ebx, [ebx]
loc_1_3190:
        cmp    dword ptr [0x7890], edi       ; -> obj1:0x7890
        mov    esi, 0
        je     loc_1_31B4
        mov    esi, dword ptr [0x7890]       ; -> obj1:0x7890
        sub    esi, 0x34
        mov    eax, dword ptr [0x7890]       ; -> obj1:0x7890
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_31B4:
        or     esi, esi
        je     loc_1_31C3
        push   esi
        call   sub_1_0151
        add    esp, 4
        jmp    loc_1_3190
loc_1_31C3:
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_31CC:
        add    dword ptr [ecx], esi
        add    byte ptr [eax], al
        or     dword ptr [ecx], esi
        add    byte ptr [eax], al
        adc    dword ptr [ecx], esi
        add    byte ptr [eax], al
        std    
        xor    byte ptr [eax], al
        add    byte ptr [ecx], bl            ; -> loc_1_3119
        xor    dword ptr [eax], eax
        add    byte ptr [ecx], ah            ; -> loc_1_3121
        xor    dword ptr [eax], eax
        add    byte ptr [ecx], ah            ; -> loc_1_3121
        xor    dword ptr [eax], eax
        add    byte ptr [ecx], bl            ; -> loc_1_3119
        xor    dword ptr [eax], eax
        add    ch, bh                        ; -> loc_1_30FD
        xor    byte ptr [eax], al
        add    ch, bh                        ; -> loc_1_30FD
        xor    byte ptr [eax], al
        add    ch, bh                        ; -> loc_1_30FD
        xor    byte ptr [eax], al
        add    ch, bh                        ; -> loc_1_30FD
        xor    byte ptr [eax], al
        add    ch, bh                        ; -> loc_1_30FD
        xor    byte ptr [eax], al
        add    ch, bh                        ; -> loc_1_30FD
        xor    byte ptr [eax], al
        add    ch, bh                        ; -> loc_1_30FD
        xor    byte ptr [eax], al
        add    ch, bh                        ; -> loc_1_30FD
        xor    byte ptr [eax], al
        add    ch, bh                        ; -> loc_1_30FD
        xor    byte ptr [eax], al
        add    byte ptr [ecx], ch            ; -> loc_1_3129
        xor    dword ptr [eax], eax
        add    ch, bh                        ; -> loc_1_30FD
        xor    byte ptr [eax], al
        add    byte ptr [ecx], ch            ; -> loc_1_3129
        xor    dword ptr [eax], eax
        add    byte ptr [ebp - 0x4fffdb9c], cl
        add    eax, ebx
        lea    ebx, [ebx]
        lea    esp, [esp]
loc_1_3230:
        sub    al, al
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3240:
        push   esi
        mov    eax, dword ptr [0x7880]       ; -> obj1:0x7880
        or     eax, eax
        je     loc_1_3273
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
loc_1_3250:
        cmp    dword ptr [eax], esi
        jne    loc_1_326C
        mov    ecx, dword ptr [edx + 4]
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_326C
        mov    ecx, dword ptr [edx + 8]
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_326C
        mov    ecx, dword ptr [edx + 0xc]
        cmp    dword ptr [eax + 0xc], ecx
        je     loc_1_3273
loc_1_326C:
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_3250
loc_1_3273:
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3280:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    bl, byte ptr [0x7974]         ; -> obj1:0x7974
        mov    cl, byte ptr [esi + 0x1a]
        lea    eax, [esi + 0x1a]
        cmp    cl, 0x2a
        je     loc_1_3332
        or     cl, cl
        je     loc_1_3332
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     loc_1_32B7
        mov    al, 0xd
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_32B7:
        sub    ecx, ecx
        mov    edi, dword ptr [0x7984]       ; -> obj1:0x7984
        cmp    edi, ecx
        mov    edx, dword ptr [0x78f4]       ; -> obj1:0x78F4
        je     loc_1_32DD
        lea    eax, [edx + 0x14]
        lea    esp, [esp]
loc_1_32D0:
        test   byte ptr [eax], 0x40
        je     loc_1_32DD
        add    eax, 0x44
        inc    ecx
        cmp    edi, ecx
        ja     loc_1_32D0
loc_1_32DD:
        cmp    edi, ecx
        ja     loc_1_32E7
        mov    al, 0xe
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_32E7:
        mov    edi, dword ptr [0x7850]       ; -> obj1:0x7850
        lea    ecx, [ecx]
loc_1_32F0:
        inc    bl
        cmp    bl, 0xff
        jne    loc_1_32F9
        mov    bl, 1
loc_1_32F9:
        xor    eax, eax
        mov    al, bl
        cmp    byte ptr [edi + eax], 0xff
        jne    loc_1_32F0
        xor    eax, eax
        mov    al, bl
        mov    byte ptr [edi + eax], cl
        mov    eax, ecx
        shl    eax, 4
        add    eax, ecx
        lea    eax, [edx + eax*4]
        mov    byte ptr [esi + 3], bl
        or     byte ptr [eax + 0x14], 0x40
        mov    byte ptr [eax + 0x15], bl
        mov    byte ptr [0x7974], bl         ; -> obj1:0x7974
        push   eax
        push   esi
        call   sub_1_3420
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_3332:
        mov    al, 0x15
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3340:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        add    esi, 0x1a
        push   esi
        call   sub_1_3240
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_1_3412
        test   byte ptr [edi + 0x14], 4
        jne    loc_1_3370
        mov    bl, 0x15
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_3370:
        push   edi
        call   sub_1_3760
        add    esp, 4
        mov    bl, al
        xor    ecx, ecx
        mov    cl, byte ptr [edi + 0x15]
        mov    eax, dword ptr [0x7850]       ; -> obj1:0x7850
        mov    byte ptr [ecx + eax], 0xff
        call   sub_1_37F0
        or     eax, eax
        je     loc_1_3404
        mov    word ptr [eax + 0x28], 0x50
        mov    dword ptr [eax + 0x1c], 0xffffffff
        mov    word ptr [eax + 0x20], 0xffff
        lea    edx, [eax + 0x4c]
        cmp    byte ptr [0x7814], 1          ; -> obj1:0x7814
        sbb    cl, cl
        and    cl, 0xf0
        add    cl, 0x14
        mov    byte ptr [edx + 5], cl
        mov    ecx, dword ptr [0x7920]       ; -> obj1:0x7920
        mov    dword ptr [edx + 6], ecx
        mov    dword ptr [edx + 0xa], 0xffffffff
        mov    word ptr [edx + 0xe], 0xffff
        mov    byte ptr [edx + 0x3f], 5
        mov    byte ptr [edx + 0x1f], 5
        add    edx, 0x1e
        mov    cl, byte ptr [edi + 0x14]
        mov    byte ptr [edx + 0x20], cl
        mov    ecx, dword ptr [esi]
        mov    dword ptr [edx + 0x22], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edx + 0x26], ecx
        add    edx, 0x22
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [esi + 0xc]
        mov    dword ptr [edx + 0xc], ecx
        push   eax
        call   sub_1_0292
        add    esp, 4
loc_1_3404:
        or     bl, bl
        jne    loc_1_3414
        mov    byte ptr [edi + 0x14], 0
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_3412:
        mov    bl, 0x15
loc_1_3414:
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3420:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esi + 0x1a]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0x1e]
        lea    ebx, [esi + 0x1a]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [edi + 0xc], eax
        mov    al, byte ptr [esi]
        and    eax, 0x7f
        cmp    eax, 0x36
        jne    loc_1_3453
        or     byte ptr [edi + 0x14], 0x80
loc_1_3453:
        lea    eax, [edi + 0x18]
        mov    dword ptr [eax], eax
        mov    dword ptr [edi + 0x1c], eax
        lea    eax, [edi + 0x20]
        mov    dword ptr [eax], eax
        mov    dword ptr [edi + 0x24], eax
        lea    eax, [edi + 0x28]
        mov    dword ptr [eax], eax
        mov    dword ptr [edi + 0x2c], eax
        lea    eax, [edi + 0x30]
        mov    dword ptr [eax], eax
        mov    dword ptr [edi + 0x34], eax
        mov    dword ptr [edi + 0x38], 0
        mov    al, byte ptr [0x7854]         ; -> obj1:0x7854
        mov    byte ptr [edi + 0x17], al
        mov    dword ptr [edi + 0x3c], esi
        call   sub_1_37F0
        or     eax, eax
        jne    loc_1_34B0
        xor    ecx, ecx
        mov    cl, byte ptr [edi + 0x15]
        mov    eax, dword ptr [0x7850]       ; -> obj1:0x7850
        mov    byte ptr [ecx + eax], 0xff
        mov    byte ptr [edi + 0x14], 0
        mov    al, 9
        mov    byte ptr [esi + 1], 9
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_34B0:
        mov    ecx, dword ptr [0x7880]       ; -> obj1:0x7880
        mov    dword ptr [edi + 0x10], ecx
        mov    dword ptr [0x7880], edi       ; -> obj1:0x7880
        mov    dword ptr [eax + 4], 0x35e0   ; -> loc_1_35E0
        lea    edx, [eax + 0x4c]
        mov    dword ptr [eax + 0x1c], 0xffffffff
        mov    word ptr [eax + 0x20], 0xffff
        mov    cl, byte ptr [edi + 0x15]
        and    ecx, 0xffff00ff
        or     word ptr [eax + 0x3c], 0x100
        mov    word ptr [eax + 0x3e], cx
        cmp    byte ptr [0x7814], 1          ; -> obj1:0x7814
        sbb    cl, cl
        and    cl, 0xf0
        add    cl, 0x14
        mov    byte ptr [edx + 5], cl
        mov    ecx, dword ptr [0x7920]       ; -> obj1:0x7920
        mov    dword ptr [edx + 6], ecx
        mov    dword ptr [edx + 0xa], 0xffffffff
        mov    word ptr [edx + 0xe], 0xffff
        mov    byte ptr [edx + 0x3f], 3
        mov    byte ptr [edx + 0x1f], 3
        add    edx, 0x1e
        mov    cl, byte ptr [edi + 0x14]
        mov    byte ptr [edx + 0x20], cl
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [edx + 0x22], ecx
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [edx + 0x26], ecx
        add    edx, 0x22
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [edx + 0xc], ecx
        mov    word ptr [eax + 0x28], 0x50
        push   eax
        call   sub_1_0292
        add    esp, 4
        mov    al, 0xff
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3560:
        push   esi
        mov    eax, dword ptr [0x7850]       ; -> obj1:0x7850
        mov    esi, dword ptr [0x78f4]       ; -> obj1:0x78F4
        mov    byte ptr [eax + 1], 0
        mov    byte ptr [esi + 0x15], 1
        mov    byte ptr [esi + 0x14], 0x44
        push   0
        push   0xa
        push   esi
        call   sub_1_0192
        add    esp, 0xc
        mov    eax, 0x7924                   ; -> obj1:0x7924
        lea    edx, [esi + 0xa]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        lea    eax, [esi + 0x18]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x1c], eax
        lea    eax, [esi + 0x20]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x24], eax
        lea    eax, [esi + 0x28]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x2c], eax
        lea    eax, [esi + 0x30]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x34], eax
        mov    dword ptr [esi + 0x38], 0
        mov    eax, dword ptr [0x7880]       ; -> obj1:0x7880
        mov    dword ptr [esi + 0x10], eax
        mov    dword ptr [0x7880], esi       ; -> obj1:0x7880
        mov    dword ptr [esi + 0x3c], 0
        pop    esi
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_35E0:
        push   esi
        mov    edx, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 0x3e]
        mov    eax, dword ptr [0x7850]       ; -> obj1:0x7850
        mov    cl, byte ptr [ecx + eax]
        and    ecx, 0xff
        cmp    dword ptr [0x7984], ecx       ; -> obj1:0x7984
        jbe    loc_1_3629
        mov    esi, ecx
        shl    esi, 4
        mov    eax, dword ptr [0x78f4]       ; -> obj1:0x78F4
        add    esi, ecx
        dec    byte ptr [eax + esi*4 + 0x17]
        mov    dword ptr [edx + 4], 0x3640   ; -> loc_1_3640
        mov    eax, dword ptr [0x7948]       ; -> obj1:0x7948
        push   eax
        push   edx
        call   sub_1_3BE0
        add    esp, 8
        pop    esi
        ret    
loc_1_3629:
        push   edx
        call   sub_1_3820
        add    esp, 4
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3640:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0xc]
        xor    ecx, ecx
        lea    esi, [edx + 0x6a]
        mov    cx, word ptr [edx + 0x3e]
        mov    eax, dword ptr [0x7850]       ; -> obj1:0x7850
        mov    al, byte ptr [ecx + eax]
        and    eax, 0xff
        cmp    dword ptr [0x7984], eax       ; -> obj1:0x7984
        jbe    loc_1_36D4
        mov    ecx, eax
        shl    ecx, 4
        add    ecx, eax
        mov    eax, dword ptr [0x78f4]       ; -> obj1:0x78F4
        lea    edi, [eax + ecx*4]
        mov    al, byte ptr [edi + 0x14]
        mov    cl, al
        and    cl, 0xc7
        cmp    byte ptr [esi + 0x20], cl
        jne    loc_1_36D4
        cmp    byte ptr [edi + 0x17], 0
        je     loc_1_3699
        mov    dword ptr [edx + 4], 0x35e0   ; -> loc_1_35E0
        push   edx
        call   sub_1_0292
        add    esp, 4
        pop    edi
        pop    esi
        ret    
loc_1_3699:
        or     al, 4
        mov    byte ptr [edi + 0x14], al
        mov    ecx, dword ptr [edi + 0x3c]
        or     ecx, ecx
        je     loc_1_36D4
        mov    dword ptr [edi + 0x3c], 0
        mov    byte ptr [ecx + 1], 0
        test   byte ptr [ecx], 0x80
        je     loc_1_36D4
        mov    dword ptr [ecx + 0x34], 0x7890 ; -> obj1:0x7890
        add    ecx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x7894], ecx       ; -> obj1:0x7894
loc_1_36D4:
        push   edx
        call   sub_1_3820
        add    esp, 4
        pop    edi
        pop    esi
        ret    
sub_1_36E0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esi + 0x38]
        cmp    ecx, edx
        jne    loc_1_36F7
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 0x38], eax
        jmp    loc_1_370F
loc_1_36F7:
        or     edx, edx
        je     loc_1_370F
loc_1_36FB:
        mov    eax, dword ptr [edx]
        cmp    ecx, eax
        je     loc_1_3707
        mov    edx, eax
        or     edx, edx
        jne    loc_1_36FB
loc_1_3707:
        or     edx, edx
        je     loc_1_370F
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
loc_1_370F:
        cmp    dword ptr [esi + 0x38], 0
        jne    loc_1_371F
        cmp    byte ptr [esi + 0x14], 1
        jne    loc_1_371F
        mov    byte ptr [esi + 0x14], 0
loc_1_371F:
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C3h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C3h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C3h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3760:
        push   ebx
        push   esi
        mov    eax, dword ptr [0x7880]       ; -> obj1:0x7880
        mov    edx, eax
        or     eax, eax
        mov    esi, dword ptr [esp + 0xc]
        je     loc_1_377E
loc_1_3771:
        mov    ecx, dword ptr [edx + 0x10]
        cmp    esi, ecx
        je     loc_1_377E
        mov    edx, ecx
        or     edx, edx
        jne    loc_1_3771
loc_1_377E:
        cmp    esi, eax
        jne    loc_1_378C
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [0x7880], eax       ; -> obj1:0x7880
        jmp    loc_1_3796
loc_1_378C:
        or     edx, edx
        je     loc_1_3796
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [edx + 0x10], eax
loc_1_3796:
        mov    al, byte ptr [esi + 0x14]
        and    al, 0xfb
        mov    byte ptr [esi + 0x14], al
        or     al, 1
        mov    byte ptr [esi + 0x14], al
        and    al, 2
        cmp    al, 1
        sbb    bl, bl
        and    bl, 0xfe
        add    bl, 0x19
        push   ebx
        lea    eax, [esi + 0x28]
        push   eax
        call   sub_1_7080
        add    esp, 8
        push   ebx
        lea    eax, [esi + 0x30]
        push   eax
        call   sub_1_7080
        add    esp, 8
        push   ebx
        lea    eax, [esi + 0x18]
        push   eax
        call   sub_1_7080
        add    esp, 8
        push   ebx
        lea    eax, [esi + 0x20]
        push   eax
        call   sub_1_7080
        add    esp, 8
        sub    al, al
        cmp    dword ptr [esi + 0x38], 0
        je     loc_1_37ED
        mov    al, 0xf
loc_1_37ED:
        pop    esi
        pop    ebx
        ret    
sub_1_37F0:
        mov    eax, dword ptr [0x7954]       ; -> obj1:0x7954
        or     eax, eax
        je     loc_1_3814
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x7954], ecx       ; -> obj1:0x7954
        mov    word ptr [eax + 0x3c], 2
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0x3b20   ; -> loc_1_3B20
loc_1_3814:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3820:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 0x3c], 0x80
        je     loc_1_3836
        push   0
        push   eax
        call   sub_1_01FA
        add    esp, 8
        ret    
loc_1_3836:
        mov    byte ptr [eax + 8], 0
        mov    word ptr [eax + 0x3c], 0
        mov    word ptr [eax + 0x22], 1
        mov    ecx, dword ptr [0x7954]       ; -> obj1:0x7954
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x7954], eax       ; -> obj1:0x7954
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3860:
        mov    eax, dword ptr [0x7940]       ; -> obj1:0x7940
        or     eax, eax
        je     loc_1_387D
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x7940], ecx       ; -> obj1:0x7940
        mov    word ptr [eax + 0x3c], 1
        mov    dword ptr [eax], 0
loc_1_387D:
        ret    
        db  08Bh, 0FFh
sub_1_3880:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x7940]       ; -> obj1:0x7940
        or     eax, eax
        je     loc_1_38F0
        mov    ebx, dword ptr [0x78c8]       ; -> obj1:0x78C8
        cmp    ebx, 3
        jb     loc_1_38F0
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x7940], ecx       ; -> obj1:0x7940
        mov    word ptr [eax + 0x3c], 1
        mov    dword ptr [eax + 4], 0x3a10   ; -> loc_1_3A10
        lea    esi, [eax + 0x2a]
        mov    edi, dword ptr [0x7934]       ; -> obj1:0x7934
        mov    dx, word ptr [0x788c]         ; -> obj1:0x788C
        sub    dx, 8
        mov    ecx, 3
loc_1_38C6:
        lea    ebp, [edi + 8]
        mov    dword ptr [esi], ebp
        mov    word ptr [esi + 4], dx
        inc    word ptr [eax + 0x22]
        mov    edi, dword ptr [edi]
        add    esi, 6
        dec    ecx
        jne    loc_1_38C6
        mov    dword ptr [0x7934], edi       ; -> obj1:0x7934
        sub    ebx, 3
        mov    dword ptr [0x78c8], ebx       ; -> obj1:0x78C8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_38F0:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3900:
        mov    ecx, dword ptr [esp + 4]
        mov    word ptr [ecx + 0x3c], 0
        mov    byte ptr [ecx + 8], 0
        mov    word ptr [ecx + 0x22], 1
        mov    word ptr [ecx + 0x28], 0x58
        mov    eax, dword ptr [0x7940]       ; -> obj1:0x7940
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x7940], ecx       ; -> obj1:0x7940
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3930:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    word ptr [edx + 0x3c], 0
        mov    eax, dword ptr [0x7940]       ; -> obj1:0x7940
        mov    dword ptr [edx], eax
        mov    dword ptr [0x7940], edx       ; -> obj1:0x7940
        mov    ax, word ptr [edx + 0x22]
        cmp    ax, 1
        jbe    loc_1_399C
        dec    ax
        mov    esi, dword ptr [0x7934]       ; -> obj1:0x7934
        mov    ecx, eax
        and    ecx, 0xffff
        mov    eax, dword ptr [0x78c8]       ; -> obj1:0x78C8
        add    eax, ecx
        mov    dword ptr [0x78c8], eax       ; -> obj1:0x78C8
        mov    edi, edi
loc_1_3970:
        mov    cx, word ptr [edx + 0x22]
        mov    eax, ecx
        and    eax, 0xffff
        dec    cx
        lea    eax, [eax + eax*2]
        mov    eax, dword ptr [edx + eax*2 + 0x1e]
        mov    dword ptr [eax - 8], esi
        sub    eax, 8
        mov    esi, eax
        mov    dword ptr [0x7934], esi       ; -> obj1:0x7934
        mov    word ptr [edx + 0x22], cx
        cmp    cx, 1
        ja     loc_1_3970
loc_1_399C:
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_39A0:
        push   esi
        call   sub_1_3880
        mov    esi, eax
        or     esi, esi
        jne    loc_1_39B0
        sub    eax, eax
        pop    esi
        ret    
loc_1_39B0:
        push   esi
        call   sub_1_02B0
        add    esp, 4
        or     eax, eax
        je     loc_1_39CA
        push   esi
        call   sub_1_3930
        add    esp, 4
        sub    eax, eax
        pop    esi
        ret    
loc_1_39CA:
        mov    eax, dword ptr [0x794c]       ; -> obj1:0x794C
        inc    eax
        mov    dword ptr [0x794c], eax       ; -> obj1:0x794C
        mov    eax, 1
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_39E0:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 0x3c]
        test   cl, 2
        je     loc_1_39F7
        push   eax
        call   sub_1_3820
        add    esp, 4
        ret    
loc_1_39F7:
        test   cl, 1
        je     loc_1_3A05
        push   eax
        call   sub_1_3930
        add    esp, 4
loc_1_3A05:
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3A10:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    al, byte ptr [esi + 9]
        or     al, al
        jne    loc_1_3AF8
        mov    eax, dword ptr [0x771c]       ; -> obj1:0x771C
        or     eax, 1
        mov    dword ptr [0x771c], eax       ; -> obj1:0x771C
        mov    eax, dword ptr [0x7978]       ; -> obj1:0x7978
        inc    eax
        mov    dword ptr [0x7978], eax       ; -> obj1:0x7978
        cmp    word ptr [esi + 0x68], 0x5504
        lea    edi, [esi + 0x4c]
        je     loc_1_3A68
        inc    dword ptr [0x7844]            ; -> obj1:0x7844
        push   esi
        call   sub_1_3930
        add    esp, 4
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_3B1D
        call   sub_1_39A0
        pop    edi
        pop    esi
        ret    
loc_1_3A68:
        mov    ax, word ptr [edi + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_3A89
        mov    eax, dword ptr [0x794c]       ; -> obj1:0x794C
        dec    eax
        mov    dword ptr [0x794c], eax       ; -> obj1:0x794C
loc_1_3A89:
        cmp    byte ptr [edi + 5], 0x14
        jne    loc_1_3A94
        mov    al, byte ptr [edi + 0x3f]
        jmp    loc_1_3A97
loc_1_3A94:
        mov    al, byte ptr [edi + 0x1f]
loc_1_3A97:
        dec    al
        cmp    al, 0xc
        jae    loc_1_3AD3
        and    eax, 0xff
        mov    eax, dword ptr [eax*4 + 0x76e4] ; -> obj1:0x76E4
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0x44], 0x7858 ; -> obj1:0x7858
        add    esi, 0x44
        mov    eax, dword ptr [0x785c]       ; -> obj1:0x785C
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x785c]       ; -> obj1:0x785C
        mov    dword ptr [eax], esi
        mov    dword ptr [0x785c], esi       ; -> obj1:0x785C
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret    
loc_1_3AD3:
        inc    dword ptr [0x7844]            ; -> obj1:0x7844
        push   esi
        call   sub_1_3930
        add    esp, 4
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_3AF0
        call   sub_1_39A0
loc_1_3AF0:
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret    
loc_1_3AF8:
        cmp    al, 0xfc
        je     loc_1_3B12
        cmp    al, 0xff
        je     loc_1_3B12
        inc    dword ptr [0x7844]            ; -> obj1:0x7844
        push   esi
        call   sub_1_3930
        add    esp, 4
        pop    edi
        pop    esi
        ret    
loc_1_3B12:
        mov    eax, dword ptr [0x794c]       ; -> obj1:0x794C
        dec    eax
        mov    dword ptr [0x794c], eax       ; -> obj1:0x794C
loc_1_3B1D:
        pop    edi
        pop    esi
        ret    
loc_1_3B20:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_3820
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_3B30:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [ecx], eax
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3B50:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        ret    
        db  090h
sub_1_3B60:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x18]
        and    ecx, 0xffff
        mov    esi, dword ptr [esp + 0x14]
        xor    ebx, ebx
        mov    bx, word ptr [esi + 4]
        mov    di, word ptr [esp + 0x20]
        sub    ebx, ecx
        mov    eax, edi
        and    eax, 0xffff
        cmp    ebx, eax
        jl     loc_1_3B8C
        mov    ebx, eax
loc_1_3B8C:
        or     bx, bx
        je     loc_1_3BA6
        mov    ebp, dword ptr [esp + 0x1c]
        push   ebx
        push   ebp
        mov    eax, dword ptr [esi]
        add    eax, ecx
        push   eax
        call   sub_1_01BB
        add    esp, 0xc
        jmp    loc_1_3BAA
loc_1_3BA6:
        mov    ebp, dword ptr [esp + 0x1c]
loc_1_3BAA:
        sub    di, bx
        je     loc_1_3BDA
        nop    
loc_1_3BB0:
        add    esi, 6
        mov    eax, ebx
        and    eax, 0xffff
        add    ebp, eax
        mov    bx, word ptr [esi + 4]
        cmp    bx, di
        jb     loc_1_3BC8
        mov    bx, di
loc_1_3BC8:
        push   ebx
        push   ebp
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_01BB
        add    esp, 0xc
        sub    di, bx
        jne    loc_1_3BB0
loc_1_3BDA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_3BE0:
        push   esi
        mov    esi, dword ptr [0x7864]       ; -> obj1:0x7864
        mov    eax, dword ptr [esp + 8]
        add    esi, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x10], esi
        or     word ptr [eax + 0x3c], 8
        mov    edx, dword ptr [0x7930]       ; -> obj1:0x7930
        mov    ecx, edx
        sub    ecx, 0x44
        cmp    edx, 0x792c                   ; -> obj1:0x792C
        je     loc_1_3C1D
loc_1_3C0A:
        cmp    dword ptr [ecx + 0x10], esi
        jbe    loc_1_3C1D
        mov    edx, dword ptr [edx + 4]
        cmp    edx, 0x792c                   ; -> obj1:0x792C
        lea    ecx, [edx - 0x44]
        jne    loc_1_3C0A
loc_1_3C1D:
        push   edx
        add    eax, 0x44
        push   eax
        call   sub_1_3B30
        add    esp, 8
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_3C30:
        push   esi
        mov    esi, dword ptr [esp + 8]
        lea    eax, [esi + 0x44]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        and    word ptr [esi + 0x3c], 0xfff7
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3C50:
        push   ebx
        push   esi
        push   edi
        inc    dword ptr [0x7864]            ; -> obj1:0x7864
        call   sub_1_0081
        or     eax, eax
        jne    loc_1_3CD2
        mov    ecx, dword ptr [0x792c]       ; -> obj1:0x792C
        mov    esi, ecx
        sub    esi, 0x44
        cmp    ecx, 0x792c                   ; -> obj1:0x792C
        je     loc_1_3D45
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        mov    edi, 0x792c                   ; -> obj1:0x792C
loc_1_3C83:
        cmp    dword ptr [esi + 0x10], eax
        ja     loc_1_3D45
        and    word ptr [esi + 0x3c], 0xfff7
        push   ecx
        call   sub_1_3B50
        add    esp, 4
        mov    dword ptr [esi + 0x44], 0x7858 ; -> obj1:0x7858
        add    esi, 0x44
        mov    eax, dword ptr [0x785c]       ; -> obj1:0x785C
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x785c]       ; -> obj1:0x785C
        mov    dword ptr [eax], esi
        mov    dword ptr [0x785c], esi       ; -> obj1:0x785C
        mov    ecx, dword ptr [0x792c]       ; -> obj1:0x792C
        mov    esi, ecx
        sub    esi, 0x44
        cmp    ecx, edi
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        jne    loc_1_3C83
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_3CD2:
        sub    edi, edi
        mov    ecx, dword ptr [0x792c]       ; -> obj1:0x792C
        mov    esi, ecx
        sub    esi, 0x44
        cmp    ecx, 0x792c                   ; -> obj1:0x792C
        je     loc_1_3D2C
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        mov    ebx, 0x792c                   ; -> obj1:0x792C
loc_1_3CF1:
        cmp    dword ptr [esi + 0x10], eax
        ja     loc_1_3D2C
        push   ecx
        call   sub_1_3B50
        add    esp, 4
        and    word ptr [esi + 0x3c], 0xfff7
        mov    ecx, dword ptr [esi + 4]
        or     ecx, ecx
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        je     loc_1_3D1C
        push   esi
        call   ecx
        add    esp, 4
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
loc_1_3D1C:
        inc    edi
        mov    ecx, dword ptr [0x792c]       ; -> obj1:0x792C
        mov    esi, ecx
        sub    esi, 0x44
        cmp    ecx, ebx
        jne    loc_1_3CF1
loc_1_3D2C:
        call   sub_1_2C20
        mov    eax, dword ptr [0x7714]       ; -> obj1:0x7714
        dec    eax
        mov    dword ptr [0x7714], eax       ; -> obj1:0x7714
        or     edi, edi
        je     loc_1_3D45
        call   sub_1_2E80
loc_1_3D45:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3D50:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cl, byte ptr [esi + 2]
        mov    ecx, dword ptr [eax + ecx*4]
        or     ecx, ecx
        jne    loc_1_3D70
        mov    al, 8
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_3D70:
        cmp    byte ptr [ecx + 6], 3
        je     loc_1_3D80
        mov    al, 8
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_3D80:
        lea    eax, [esi + 0x34]
        lea    edx, [ecx + 0x5c]
        mov    dword ptr [eax], edx
        mov    edi, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi], eax
        mov    dword ptr [edx + 4], eax
        mov    byte ptr [esi + 0x32], 7
        mov    al, byte ptr [esi]
        and    eax, 0x7f
        cmp    eax, 0x17
        jne    loc_1_3DC0
        xor    edx, edx
        mov    dx, word ptr [esi + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0xa]
        add    eax, edx
        cmp    eax, 0xffff
        jbe    loc_1_3DC6
        mov    al, 1
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_3DC0:
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
loc_1_3DC6:
        cmp    byte ptr [ecx + 7], 0
        jne    loc_1_3DE4
        mov    byte ptr [ecx + 7], 1
        mov    edx, dword ptr [0x7994]       ; -> obj1:0x7994
        mov    word ptr [ecx + 0x28], dx
        mov    word ptr [ecx + 0x16], ax
        mov    word ptr [ecx + 0x18], 0
loc_1_3DE4:
        mov    ax, word ptr [ecx + 4]
        test   al, 0x40
        jne    loc_1_3E13
        or     ax, 0x40
        mov    word ptr [ecx + 4], ax
        mov    dword ptr [ecx + 8], 0x78d4   ; -> obj1:0x78D4
        add    ecx, 8
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x78d8], ecx       ; -> obj1:0x78D8
loc_1_3E13:
        mov    al, 0xff
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3E20:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_3D50
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_3E30:
        mov    al, 3
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3E40:
        mov    al, 3
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3E50:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 0x32], 7
        jne    loc_1_3E8B
        lea    eax, [esi + 0x34]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 0x32], 0
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cl, byte ptr [esi + 2]
        mov    eax, dword ptr [eax + ecx*4]
        mov    byte ptr [esi + 1], 0xb
        push   0x18
        push   eax
        call   sub_1_07A0
        add    esp, 8
        sub    al, al
        pop    esi
        ret    
loc_1_3E8B:
        mov    al, 0x26
        pop    esi
        ret    
        db  090h
sub_1_3E90:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_37F0
        or     eax, eax
        jne    loc_1_3EB0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3EB0:
        mov    edx, dword ptr [esp + 0x20]
        lea    ecx, [eax + 0x4c]
        mov    edi, dword ptr [edx + 0x5c]
        sub    edi, 0x34
        mov    dword ptr [esp + 0x14], ecx
        lea    ebx, [ecx + 0x1e]
        mov    dword ptr [esp + 0x10], ebx
        mov    dword ptr [eax + 0x40], edi
        inc    byte ptr [edi + 0x33]
        mov    cl, byte ptr [edx + 0x10]
        and    ecx, 0xffff00ff
        mov    word ptr [eax + 0x3e], cx
        mov    word ptr [eax + 0x28], 0x30
        lea    ebx, [eax + 0x1c]
        mov    ecx, dword ptr [edx + 0x36]
        mov    dword ptr [ebx], ecx
        mov    cx, word ptr [edx + 0x3a]
        mov    word ptr [ebx + 4], cx
        lea    esi, [edx + 0x36]
        mov    cl, byte ptr [edi]
        and    ecx, 0x7f
        cmp    ecx, 0x17
        jne    loc_1_3FB0
        mov    si, word ptr [edi + 8]
        mov    cx, word ptr [edx + 0x18]
        cmp    si, cx
        jbe    loc_1_3F76
        mov    word ptr [eax + 0x22], 2
        and    ecx, 0xffff
        xor    ebx, ebx
        mov    bx, word ptr [edx + 0x9a]
        and    esi, 0xffff
        sub    esi, ecx
        cmp    esi, ebx
        jl     loc_1_3F32
        mov    esi, ebx
loc_1_3F32:
        mov    word ptr [eax + 0x2e], si
        mov    ebp, dword ptr [edi + 4]
        add    ebp, ecx
        mov    dword ptr [eax + 0x2a], ebp
        cmp    si, word ptr [edx + 0x9a]
        jae    loc_1_3FE2
        mov    word ptr [eax + 0x22], 3
        xor    ecx, ecx
        mov    cx, word ptr [edi + 0xa]
        mov    ebp, esi
        and    ebp, 0xffff
        sub    ebx, ebp
        cmp    ecx, ebx
        jl     loc_1_3F67
        mov    ecx, ebx
loc_1_3F67:
        mov    word ptr [eax + 0x34], cx
        add    si, cx
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0x30], ecx
        jmp    loc_1_3FE2
loc_1_3F76:
        mov    ebp, ecx
        and    ebp, 0xffff
        mov    ebx, ebp
        mov    ecx, esi
        and    ecx, 0xffff
        sub    ebx, ecx
        mov    word ptr [eax + 0x22], 2
        xor    esi, esi
        mov    si, word ptr [edx + 0x16]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 0x9a]
        sub    esi, ebp
        cmp    esi, ecx
        jl     loc_1_3FA7
        mov    esi, ecx
loc_1_3FA7:
        mov    word ptr [eax + 0x2e], si
        mov    ecx, dword ptr [edi + 0xc]
        jmp    loc_1_3FDD
loc_1_3FB0:
        mov    word ptr [eax + 0x22], 2
        lea    ecx, [edx + 0x18]
        xor    ebx, ebx
        mov    bx, word ptr [edx + 0x18]
        xor    esi, esi
        mov    si, word ptr [edx + 0x16]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 0x9a]
        sub    esi, ebx
        cmp    esi, ecx
        jl     loc_1_3FD6
        mov    esi, ecx
loc_1_3FD6:
        mov    word ptr [eax + 0x2e], si
        mov    ecx, dword ptr [edi + 4]
loc_1_3FDD:
        add    ecx, ebx
        mov    dword ptr [eax + 0x2a], ecx
loc_1_3FE2:
        mov    ecx, dword ptr [esp + 0x14]
        lea    ebx, [edx + 0x3c]
        mov    byte ptr [ecx + 5], 4
        mov    edi, ecx
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [edi + 6], ecx
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [edi + 0xa], ecx
        add    edi, 6
        mov    cx, word ptr [ebx + 8]
        mov    ebx, dword ptr [esp + 0x10]
        mov    word ptr [edi + 8], cx
        mov    byte ptr [ebx + 1], 6
        mov    cx, word ptr [edx + 0x10]
        mov    word ptr [ebx + 2], cx
        mov    cx, word ptr [edx + 0x12]
        mov    word ptr [ebx + 4], cx
        mov    cx, word ptr [edx + 0x14]
        mov    word ptr [ebx + 6], cx
        mov    cx, word ptr [edx + 0x16]
        mov    word ptr [ebx + 8], cx
        mov    cx, word ptr [edx + 0x18]
        mov    word ptr [ebx + 0xa], cx
        mov    word ptr [edx + 0x1a], si
        mov    word ptr [ebx + 0xc], si
        mov    cx, word ptr [edx + 0x1c]
        mov    word ptr [ebx + 0xe], cx
        test   byte ptr [edx + 5], 0x80
        je     loc_1_4090
        mov    di, word ptr [edx + 0x22]
        add    di, cx
        dec    di
        mov    word ptr [ebx + 0x10], di
        mov    word ptr [edx + 0x2a], 0
        cmp    byte ptr [0x7990], 1          ; -> obj1:0x7990
        mov    di, word ptr [edx + 4]
        sbb    ecx, ecx
        neg    ecx
        mov    bx, di
        and    ebx, 4
        or     ecx, ebx
        cmp    ecx, 1
        cmc    
        sbb    cl, cl
        and    cl, 0x40
        mov    ebx, dword ptr [esp + 0x10]
        and    di, 0xfffb
        mov    byte ptr [ebx], cl
        mov    word ptr [edx + 4], di
        jmp    loc_1_409F
        db  090h
loc_1_4090:
        mov    ebx, dword ptr [esp + 0x10]
        mov    cx, word ptr [edx + 0x1e]
        mov    word ptr [ebx + 0x10], cx
        mov    byte ptr [ebx], 0x40
loc_1_409F:
        xor    edi, edi
        mov    di, word ptr [edx + 0x18]
        mov    ecx, esi
        and    ecx, 0xffff
        add    edi, ecx
        xor    ecx, ecx
        mov    cx, word ptr [edx + 0x16]
        mov    ebx, dword ptr [esp + 0x10]
        sub    edi, ecx
        cmp    edi, 1
        sbb    cl, cl
        and    cl, 0x10
        or     byte ptr [ebx], cl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
sub_1_40D0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x64], ecx
        mov    dword ptr [ecx + 4], 0x40f0   ; -> loc_1_40F0
        mov    byte ptr [eax + 7], 8
        push   ecx
        call   sub_1_0292
        add    esp, 4
        ret    
loc_1_40F0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cx, word ptr [edi + 0x3e]
        mov    esi, dword ptr [eax + ecx*4]
        mov    eax, dword ptr [edi + 0x40]
        dec    byte ptr [eax + 0x33]
        or     esi, esi
        je     loc_1_4213
        cmp    byte ptr [esi + 7], 8
        jne    loc_1_4213
        mov    ax, word ptr [esi + 0x28]
        or     ax, ax
        je     loc_1_4176
        dec    ax
        mov    word ptr [esi + 0x28], ax
        mov    eax, dword ptr [esi + 0x6c]
        or     eax, eax
        je     loc_1_414D
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_414D
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x6c]
        push   eax
        call   sub_1_3820
        add    esp, 4
loc_1_414D:
        mov    dword ptr [esi + 0x6c], edi
        mov    dword ptr [edi + 4], 0x4230   ; -> sub_1_4230
        xor    eax, eax
        mov    ax, word ptr [esi + 0x30]
        push   eax
        push   edi
        call   sub_1_3BE0
        add    esp, 8
        mov    dword ptr [esi + 0x64], 0
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret    
loc_1_4176:
        push   edi
        call   sub_1_3820
        add    esp, 4
        mov    ecx, dword ptr [esi + 0x5c]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0x5c
        mov    edi, 0
        je     loc_1_419C
        lea    edi, [ecx - 0x34]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_419C:
        mov    byte ptr [esi + 7], 1
        mov    byte ptr [edi + 1], 5
        test   byte ptr [edi], 0x80
        je     loc_1_41C8
        mov    dword ptr [edi + 0x34], 0x7890 ; -> obj1:0x7890
        add    edi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], edi
        mov    dword ptr [0x7894], edi       ; -> obj1:0x7894
loc_1_41C8:
        mov    dword ptr [esi + 0x64], 0
        mov    byte ptr [esi + 6], 6
        mov    eax, dword ptr [0x7994]       ; -> obj1:0x7994
        mov    word ptr [esi + 0x28], ax
        mov    eax, dword ptr [esi + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_4205
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   0
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0xc
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret    
loc_1_4205:
        or     word ptr [esi + 4], 0x1000
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret    
loc_1_4213:
        push   edi
        call   sub_1_3820
        add    esp, 4
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4230:
        push   esi
        mov    edx, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cx, word ptr [edx + 0x3e]
        mov    esi, dword ptr [eax + ecx*4]
        push   edx
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 0x6c], 0
        cmp    byte ptr [esi + 7], 8
        jne    loc_1_4293
        mov    eax, dword ptr [0x795c]       ; -> obj1:0x795C
        inc    eax
        mov    dword ptr [0x795c], eax       ; -> obj1:0x795C
        mov    ax, word ptr [esi + 4]
        test   al, 0x40
        jne    loc_1_4293
        or     ax, 0x40
        mov    word ptr [esi + 4], ax
        mov    dword ptr [esi + 8], 0x78d4   ; -> obj1:0x78D4
        add    esi, 8
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [eax], esi
        mov    dword ptr [0x78d8], esi       ; -> obj1:0x78D8
loc_1_4293:
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_42A0:
        sub    esp, 4
        push   esi
        cmp    dword ptr [0x78ec], 0x78ec    ; -> obj1:0x78EC
        je     loc_1_42E9
        mov    eax, dword ptr [0x78ec]       ; -> obj1:0x78EC
        mov    ecx, 8
loc_1_42BA:
        test   byte ptr [eax + 8], cl
        lea    esi, [eax - 0x20]
        jne    loc_1_42CB
        mov    eax, dword ptr [eax]
        cmp    eax, 0x78ec                   ; -> obj1:0x78EC
        jne    loc_1_42BA
loc_1_42CB:
        cmp    eax, 0x78ec                   ; -> obj1:0x78EC
        je     loc_1_42E9
        call   sub_1_37F0
        or     eax, eax
        je     loc_1_42E9
        and    byte ptr [esi + 0x28], 0xf7
        push   eax
        push   esi
        call   sub_1_2930
        add    esp, 8
loc_1_42E9:
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_42F0:
        push   esi
        push   edi
        call   sub_1_42A0
        cmp    dword ptr [0x78d4], 0x78d4    ; -> obj1:0x78D4
        je     loc_1_43C8
        mov    edi, 0x78d4                   ; -> obj1:0x78D4
        cmp    dword ptr [0x78d4], edi       ; -> obj1:0x78D4
        mov    esi, 0
        je     loc_1_4330
loc_1_4319:
        mov    esi, dword ptr [0x78d4]       ; -> obj1:0x78D4
        sub    esi, 8
        mov    eax, dword ptr [0x78d4]       ; -> obj1:0x78D4
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_4330:
        test   byte ptr [esi + 5], 0x80
        push   esi
        je     loc_1_4350
        call   sub_1_4590
        add    esp, 4
        or     eax, eax
        je     loc_1_4390
        and    word ptr [esi + 4], 0xffbf
        jmp    loc_1_4380
        db  005h, 000h, 000h, 000h, 000h
loc_1_4350:
        call   sub_1_3E90
        add    esp, 4
        or     eax, eax
        mov    ecx, dword ptr [0x7994]       ; -> obj1:0x7994
        je     loc_1_43B0
        cmp    byte ptr [esi + 7], 1
        jne    loc_1_4370
        mov    word ptr [esi + 0x28], cx
        mov    byte ptr [esi + 7], 4
loc_1_4370:
        and    word ptr [esi + 4], 0xffbf
        push   eax
        push   esi
        call   sub_1_40D0
        add    esp, 8
loc_1_4380:
        cmp    dword ptr [0x78d4], edi       ; -> obj1:0x78D4
        jne    loc_1_4319
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_4390:
        add    esi, 8
        mov    ecx, dword ptr [0x78d4]       ; -> obj1:0x78D4
        mov    eax, ecx
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 4], esi
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_43B0:
        add    esi, 8
        mov    ecx, dword ptr [0x78d4]       ; -> obj1:0x78D4
        mov    eax, ecx
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 4], esi
loc_1_43C8:
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_43D0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    ax, word ptr [esi + 0x28]
        dec    ax
        mov    word ptr [esi + 0x28], ax
        jne    loc_1_4471
        mov    byte ptr [esi + 7], 8
        mov    byte ptr [esi + 6], 6
        mov    eax, dword ptr [0x7994]       ; -> obj1:0x7994
        mov    word ptr [esi + 0x28], ax
        mov    edi, 0
        mov    ecx, dword ptr [esi + 0x5c]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0x5c
        je     loc_1_4414
        lea    edi, [ecx - 0x34]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_4414:
        or     edi, edi
        je     loc_1_4444
        mov    byte ptr [edi + 0x32], 0
        mov    byte ptr [edi + 1], 5
        test   byte ptr [edi], 0x80
        je     loc_1_4444
        mov    dword ptr [edi + 0x34], 0x7890 ; -> obj1:0x7890
        add    edi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], edi
        mov    dword ptr [0x7894], edi       ; -> obj1:0x7894
loc_1_4444:
        mov    eax, dword ptr [esi + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_4468
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   0
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0xc
        pop    edi
        pop    esi
        ret    
loc_1_4468:
        or     word ptr [esi + 4], 0x1000
        pop    edi
        pop    esi
        ret    
loc_1_4471:
        mov    eax, dword ptr [0x795c]       ; -> obj1:0x795C
        inc    eax
        mov    dword ptr [0x795c], eax       ; -> obj1:0x795C
        mov    word ptr [esi + 0x20], 1
        mov    ax, word ptr [esi + 0x24]
        mov    word ptr [esi + 0x14], ax
        mov    ax, word ptr [esi + 0x26]
        mov    word ptr [esi + 0x18], ax
        mov    byte ptr [esi + 7], 1
        mov    ax, word ptr [esi + 4]
        or     ax, 4
        mov    word ptr [esi + 4], ax
        test   al, 0x40
        jne    loc_1_44CD
        or     ax, 0x40
        mov    word ptr [esi + 4], ax
        mov    dword ptr [esi + 8], 0x78d4   ; -> obj1:0x78D4
        add    esi, 8
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x78d8]       ; -> obj1:0x78D8
        mov    dword ptr [eax], esi
        mov    dword ptr [0x78d8], esi       ; -> obj1:0x78D8
loc_1_44CD:
        pop    edi
        pop    esi
        ret    
loc_1_44D0:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        xor    edx, edx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    dx, word ptr [ecx + 0x3e]
        mov    esi, dword ptr [eax + edx*4]
        push   ecx
        mov    dword ptr [esi + 0x6c], 0
        call   sub_1_3820
        add    esp, 4
        cmp    byte ptr [esi + 6], 3
        jne    loc_1_4502
        push   esi
        call   sub_1_43D0
        add    esp, 4
loc_1_4502:
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4510:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cx, word ptr [esi + 0x3e]
        mov    edi, dword ptr [eax + ecx*4]
        mov    eax, dword ptr [esi + 0x40]
        dec    byte ptr [eax + 0x33]
        or     edi, edi
        jne    loc_1_4540
        push   esi
        call   sub_1_3820
        add    esp, 4
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_4540:
        mov    dword ptr [edi + 0x64], 0
        mov    eax, dword ptr [edi + 0x6c]
        or     eax, eax
        je     loc_1_4563
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [edi + 0x6c]
        push   eax
        call   sub_1_3820
        add    esp, 4
loc_1_4563:
        mov    dword ptr [edi + 0x6c], esi
        mov    dword ptr [esi + 4], 0x44d0   ; -> loc_1_44D0
        xor    eax, eax
        mov    ax, word ptr [edi + 0x30]
        push   eax
        push   esi
        call   sub_1_3BE0
        add    esp, 8
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4590:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        cmp    byte ptr [esi + 7], 1
        jne    loc_1_466B
        mov    edi, 0x4510                   ; -> loc_1_4510
        mov    ebx, 8
        lea    esp, [esp]
loc_1_45B0:
        mov    eax, dword ptr [esi + 0x5c]
        sub    eax, esi
        cmp    eax, 0x5c
        je     loc_1_4667
        push   esi
        call   sub_1_3E90
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_4660
        mov    ax, word ptr [esi + 0x1a]
        inc    word ptr [esi + 0x14]
        add    ax, word ptr [esi + 0x18]
        mov    word ptr [esi + 0x18], ax
        cmp    word ptr [esi + 0x16], ax
        jne    loc_1_45F0
        mov    byte ptr [esi + 7], bl
        jmp    loc_1_4605
        db  08Dh, 064h, 024h, 000h
loc_1_45F0:
        mov    ax, word ptr [esi + 0x14]
        sub    ax, word ptr [esi + 0x24]
        cmp    ax, word ptr [esi + 0x20]
        jb     loc_1_4605
        mov    byte ptr [esi + 7], bl
        or     byte ptr [ebp + 0x6a], 0x40
loc_1_4605:
        mov    dword ptr [esi + 0x64], ebp
        mov    dword ptr [ebp + 4], edi
        test   byte ptr [esi + 5], 0x20
        je     loc_1_4641
        mov    eax, dword ptr [esi + 0x68]
        test   word ptr [eax + 0x3c], bx
        je     loc_1_463B
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x68]
        push   0x32
        mov    dword ptr [eax + 4], 0xf00    ; -> loc_1_0F00
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_3BE0
        add    esp, 8
loc_1_463B:
        and    word ptr [esi + 4], 0xdfff
loc_1_4641:
        push   ebp
        call   sub_1_0292
        add    esp, 4
        cmp    byte ptr [esi + 7], 1
        je     loc_1_45B0
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_4660:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_4667:
        mov    byte ptr [esi + 7], 0
loc_1_466B:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4680:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cl, byte ptr [edi + 2]
        mov    esi, dword ptr [eax + ecx*4]
        or     esi, esi
        je     loc_1_4700
        cmp    byte ptr [esi + 6], 3
        jne    loc_1_46FB
        cmp    byte ptr [esi + 0x2f], 0
        jne    loc_1_46A9
        mov    al, byte ptr [esi + 0x2d]
        mov    byte ptr [esi + 0x2f], al
loc_1_46A9:
        lea    ecx, [edi + 0x34]
        lea    edx, [esi + 0x54]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx + 4], ecx
        mov    byte ptr [edi + 0x32], 5
        mov    ax, word ptr [esi + 4]
        test   ah, 4
        je     loc_1_46F6
        and    ax, 0xfbff
        mov    word ptr [esi + 4], ax
        mov    eax, dword ptr [esi + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_46F6
        push   eax
        call   sub_1_3C30
        add    esp, 4
        or     word ptr [esi + 4], 0x800
        push   esi
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_0E00
        add    esp, 8
loc_1_46F6:
        mov    al, 0xff
        pop    edi
        pop    esi
        ret    
loc_1_46FB:
        mov    al, 8
        pop    edi
        pop    esi
        ret    
loc_1_4700:
        mov    al, 8
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4710:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    al, byte ptr [ecx + 3]
        cmp    al, 0xff
        jne    loc_1_4747
        mov    dword ptr [ecx + 0x34], 0x7874 ; -> obj1:0x7874
        lea    edx, [ecx + 0x34]
        mov    eax, dword ptr [0x7878]       ; -> obj1:0x7878
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x7878]       ; -> obj1:0x7878
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 0xff
        mov    dword ptr [0x7878], edx       ; -> obj1:0x7878
        mov    byte ptr [ecx + 0x32], 6
        pop    esi
        pop    ebx
        ret    
loc_1_4747:
        xor    edx, edx
        mov    dl, al
        mov    eax, dword ptr [0x7850]       ; -> obj1:0x7850
        mov    al, byte ptr [edx + eax]
        and    eax, 0xff
        cmp    dword ptr [0x7984], eax       ; -> obj1:0x7984
        jbe    loc_1_482A
        mov    edx, eax
        shl    edx, 4
        add    edx, eax
        mov    eax, dword ptr [0x78f4]       ; -> obj1:0x78F4
        lea    edi, [eax + edx*4]
        test   byte ptr [edi + 0x14], 4
        je     loc_1_482A
        lea    edx, [ecx + 0x34]
        lea    ebx, [edi + 0x18]
        mov    dword ptr [edx], ebx
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ebx + 4], edx
        mov    byte ptr [ecx + 0x32], 6
        mov    esi, dword ptr [edi + 0x38]
        or     esi, esi
        je     loc_1_4824
        mov    eax, 3
        mov    ecx, 0x400
loc_1_47A9:
        cmp    byte ptr [esi + 6], al
        jne    loc_1_47B4
        test   word ptr [esi + 4], cx
        jne    loc_1_47C0
loc_1_47B4:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_47A9
        mov    al, 0xff
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_47C0:
        mov    eax, dword ptr [ebx]
        mov    ecx, eax
        sub    ecx, edi
        cmp    ecx, 0x18
        mov    edi, 0
        je     loc_1_47DC
        lea    edi, [eax - 0x34]
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_47DC:
        mov    al, byte ptr [esi + 0x10]
        mov    byte ptr [edi + 2], al
        lea    eax, [edi + 0x34]
        lea    ecx, [esi + 0x54]
        mov    dword ptr [eax], ecx
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edx], eax
        mov    dword ptr [ecx + 4], eax
        mov    byte ptr [edi + 0x32], 5
        mov    eax, dword ptr [esi + 0x68]
        and    word ptr [esi + 4], 0xfbff
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_4824
        push   eax
        call   sub_1_3C30
        add    esp, 4
        or     word ptr [esi + 4], 0x800
        push   esi
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_0E00
        add    esp, 8
loc_1_4824:
        mov    al, 0xff
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_482A:
        mov    al, 0x13
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_4830:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        sub    bl, bl
        mov    al, byte ptr [esi + 0x32]
        cmp    al, 5
        jne    loc_1_489A
        lea    eax, [esi + 0x34]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 0x32], bl
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cl, byte ptr [esi + 2]
        mov    edi, dword ptr [eax + ecx*4]
        mov    byte ptr [esi + 1], 0xb
        mov    eax, dword ptr [edi + 0x68]
        or     eax, eax
        je     loc_1_4889
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_4889
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    byte ptr [edi + 6], 6
        push   0
        mov    eax, dword ptr [edi + 0x68]
        push   eax
        push   edi
        call   sub_1_0B20
        add    esp, 0xc
loc_1_4889:
        push   0x18
        push   edi
        call   sub_1_07A0
        add    esp, 8
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_489A:
        cmp    al, 6
        jne    loc_1_48B8
        lea    eax, [esi + 0x34]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 0x32], 0
        mov    byte ptr [esi + 1], 0xb
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_48B8:
        mov    bl, 0x26
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_48C0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        sub    edi, edi
        mov    eax, dword ptr [ebx + 0x54]
        lea    esi, [ebx + 0x54]
        mov    ecx, eax
        sub    ecx, ebx
        cmp    ecx, 0x54
        je     loc_1_48E0
        sub    eax, 0x34
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_48E0:
        mov    edx, dword ptr [ebx + 0x48]
        test   byte ptr [edx + 0x14], 4
        je     loc_1_4906
        mov    ecx, dword ptr [edx + 0x18]
        mov    eax, ecx
        sub    eax, edx
        cmp    eax, 0x18
        mov    edi, 0
        je     loc_1_4906
        lea    edi, [ecx - 0x34]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_4906:
        or     edi, edi
        jne    loc_1_4936
        cmp    dword ptr [0x7874], 0x7874    ; -> obj1:0x7874
        mov    edi, 0
        je     loc_1_4932
        mov    edi, dword ptr [0x7874]       ; -> obj1:0x7874
        sub    edi, 0x34
        mov    eax, dword ptr [0x7874]       ; -> obj1:0x7874
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_4932:
        or     edi, edi
        je     loc_1_495A
loc_1_4936:
        mov    al, byte ptr [ebx + 0x10]
        mov    byte ptr [edi + 2], al
        mov    eax, dword ptr [ebx + 0x48]
        mov    al, byte ptr [eax + 0x15]
        mov    byte ptr [edi + 3], al
        mov    dword ptr [edi + 0x34], esi
        lea    eax, [edi + 0x34]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 4], eax
        mov    byte ptr [edi + 0x32], 5
loc_1_495A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_4960:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x20]
        mov    ax, word ptr [esi + 0x1e]
        cmp    word ptr [ecx + 0xa], ax
        jne    loc_1_4B1A
        mov    ebx, dword ptr [esp + 0x1c]
        add    ebx, 0x24
        mov    word ptr [esp + 0x10], 0x30
        mov    ax, word ptr [ecx + 0xc]
        mov    word ptr [esp + 0x12], ax
loc_1_4994:
        push   esi
        call   sub_1_48C0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_1_4B14
        mov    bp, word ptr [edi + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x78]
        mov    eax, ebp
        and    eax, 0xffff
        xor    edx, edx
        mov    dx, word ptr [esp + 0x12]
        sub    eax, ecx
        cmp    eax, edx
        jge    loc_1_49D0
        sub    bp, word ptr [esi + 0x78]
        jmp    loc_1_49D5
        db  005h, 000h, 000h, 000h, 000h
loc_1_49D0:
        mov    bp, word ptr [esp + 0x12]
loc_1_49D5:
        push   ebp
        mov    eax, dword ptr [edi + 4]
        add    eax, ecx
        push   eax
        mov    ax, word ptr [esp + 0x18]
        push   eax
        push   ebx
        call   sub_1_3B60
        add    esp, 0x10
        mov    dx, word ptr [esi + 0x1e]
        add    word ptr [esi + 0x78], bp
        add    dx, bp
        sub    word ptr [esp + 0x12], bp
        mov    word ptr [esi + 0x1e], dx
        xor    eax, eax
        mov    ax, word ptr [ebx + 4]
        mov    ecx, dword ptr [esp + 0x10]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    ecx, ebp
        and    ecx, 0xffff
        cmp    eax, ecx
        jg     loc_1_4A39
loc_1_4A20:
        sub    bp, word ptr [ebx + 4]
        add    bp, word ptr [esp + 0x10]
        add    ebx, 6
        mov    word ptr [esp + 0x10], 0
        cmp    word ptr [ebx + 4], bp
        jbe    loc_1_4A20
loc_1_4A39:
        mov    ecx, dword ptr [esp + 0x20]
        add    word ptr [esp + 0x10], bp
        cmp    word ptr [ecx + 8], dx
        je     loc_1_4AB0
        mov    ax, word ptr [esi + 0x78]
        cmp    word ptr [edi + 8], ax
        jne    loc_1_4A9C
        mov    word ptr [esi + 0x78], 0
        mov    byte ptr [edi + 1], 6
        mov    byte ptr [edi + 0x32], 0
        mov    al, byte ptr [esi + 0x2d]
        or     al, al
        je     loc_1_4A74
        mov    ecx, dword ptr [esi + 0x54]
        sub    ecx, esi
        cmp    ecx, 0x54
        je     loc_1_4A74
        mov    byte ptr [esi + 0x2f], al
loc_1_4A74:
        lea    ebp, [edi + 0x34]
        push   ebp
        call   sub_1_3B50
        add    esp, 4
        test   byte ptr [edi], 0x80
        je     loc_1_4A9C
        mov    dword ptr [ebp], 0x7890       ; -> obj1:0x7890
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [0x7894], ebp       ; -> obj1:0x7894
loc_1_4A9C:
        cmp    word ptr [esp + 0x12], 0
        jne    loc_1_4994
        jmp    loc_1_4B10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_4AB0:
        mov    ax, word ptr [esi + 0x78]
        inc    word ptr [esi + 0x1c]
        mov    word ptr [edi + 8], ax
        mov    word ptr [esi + 0x78], 0
        mov    word ptr [esi + 0x1e], 0
        mov    byte ptr [edi + 1], 0
        mov    byte ptr [edi + 0x32], 0
        mov    cl, byte ptr [esi + 0x2d]
        or     cl, cl
        je     loc_1_4AE4
        mov    eax, dword ptr [esi + 0x54]
        sub    eax, esi
        cmp    eax, 0x54
        je     loc_1_4AE4
        mov    byte ptr [esi + 0x2f], cl
loc_1_4AE4:
        lea    ebx, [edi + 0x34]
        push   ebx
        call   sub_1_3B50
        add    esp, 4
        test   byte ptr [edi], 0x80
        je     loc_1_4B10
        mov    dword ptr [ebx], 0x7890       ; -> obj1:0x7890
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], ebx
        mov    dword ptr [0x7894], ebx       ; -> obj1:0x7894
loc_1_4B10:
        or     edi, edi
        jne    loc_1_4B1A
loc_1_4B14:
        or     word ptr [esi + 4], 0x400
loc_1_4B1A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4B30:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x20]
        mov    word ptr [esp + 0x10], 0x30
        mov    ax, word ptr [eax + 0xc]
        add    edi, 0x24
        mov    esi, dword ptr [esp + 0x18]
        mov    word ptr [esp + 0x12], ax
        mov    ax, word ptr [esi + 0x1e]
        or     ax, ax
        je     loc_1_4BA2
        cmp    word ptr [esp + 0x12], ax
        jbe    loc_1_4D32
        xor    ecx, ecx
        sub    word ptr [esp + 0x12], ax
        mov    cx, word ptr [edi + 4]
        sub    ecx, 0x30
        mov    edx, eax
        and    edx, 0xffff
        cmp    ecx, edx
        jg     loc_1_4B9D
loc_1_4B84:
        sub    ax, word ptr [edi + 4]
        add    ax, word ptr [esp + 0x10]
        add    edi, 6
        mov    word ptr [esp + 0x10], 0
        cmp    word ptr [edi + 4], ax
        jbe    loc_1_4B84
loc_1_4B9D:
        add    word ptr [esp + 0x10], ax
loc_1_4BA2:
        push   esi
        call   sub_1_48C0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_4CF6
        mov    bp, word ptr [ebx + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x78]
        mov    eax, ebp
        and    eax, 0xffff
        xor    edx, edx
        mov    dx, word ptr [esp + 0x12]
        sub    eax, ecx
        cmp    eax, edx
        jge    loc_1_4BD9
        sub    bp, word ptr [esi + 0x78]
        jmp    loc_1_4BDE
loc_1_4BD9:
        mov    bp, word ptr [esp + 0x12]
loc_1_4BDE:
        push   ebp
        mov    eax, dword ptr [ebx + 4]
        add    eax, ecx
        push   eax
        mov    ax, word ptr [esp + 0x18]
        push   eax
        push   edi
        call   sub_1_3B60
        add    esp, 0x10
        add    word ptr [esi + 0x1e], bp
        add    word ptr [esi + 0x78], bp
        xor    ecx, ecx
        sub    word ptr [esp + 0x12], bp
        mov    cx, word ptr [edi + 4]
        mov    eax, dword ptr [esp + 0x10]
        and    eax, 0xffff
        sub    ecx, eax
        mov    eax, ebp
        and    eax, 0xffff
        cmp    ecx, eax
        jg     loc_1_4C39
loc_1_4C20:
        sub    bp, word ptr [edi + 4]
        add    bp, word ptr [esp + 0x10]
        add    edi, 6
        mov    word ptr [esp + 0x10], 0
        cmp    word ptr [edi + 4], bp
        jbe    loc_1_4C20
loc_1_4C39:
        add    word ptr [esp + 0x10], bp
        cmp    word ptr [esp + 0x12], 0
        jne    loc_1_4C92
        mov    eax, dword ptr [esp + 0x20]
        test   byte ptr [eax], 0x10
        je     loc_1_4C92
        mov    ax, word ptr [esi + 0x78]
        mov    word ptr [ebx + 8], ax
        mov    word ptr [esi + 0x78], 0
        mov    word ptr [esi + 0x1e], 0
        mov    byte ptr [ebx + 1], 0
        mov    byte ptr [ebx + 0x32], 0
        mov    al, byte ptr [esi + 0x2d]
        or     al, al
        je     loc_1_4C7F
        mov    ecx, dword ptr [esi + 0x54]
        sub    ecx, esi
        cmp    ecx, 0x54
        je     loc_1_4C7F
        mov    byte ptr [esi + 0x2f], al
loc_1_4C7F:
        lea    ebp, [ebx + 0x34]
        push   ebp
        call   sub_1_3B50
        add    esp, 4
        test   byte ptr [ebx], 0x80
        je     loc_1_4CE6
        jmp    loc_1_4CCF
loc_1_4C92:
        mov    ax, word ptr [esi + 0x78]
        cmp    word ptr [ebx + 8], ax
        jne    loc_1_4CE6
        mov    word ptr [esi + 0x78], 0
        mov    byte ptr [ebx + 1], 6
        mov    byte ptr [ebx + 0x32], 0
        mov    al, byte ptr [esi + 0x2d]
        or     al, al
        je     loc_1_4CBE
        mov    ecx, dword ptr [esi + 0x54]
        sub    ecx, esi
        cmp    ecx, 0x54
        je     loc_1_4CBE
        mov    byte ptr [esi + 0x2f], al
loc_1_4CBE:
        lea    ebp, [ebx + 0x34]
        push   ebp
        call   sub_1_3B50
        add    esp, 4
        test   byte ptr [ebx], 0x80
        je     loc_1_4CE6
loc_1_4CCF:
        mov    dword ptr [ebp], 0x7890       ; -> obj1:0x7890
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [0x7894], ebp       ; -> obj1:0x7894
loc_1_4CE6:
        cmp    word ptr [esp + 0x12], 0
        jne    loc_1_4BA2
        or     ebx, ebx
        jne    loc_1_4D04
loc_1_4CF6:
        or     word ptr [esi + 4], 0x480
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_4D04:
        inc    word ptr [esi + 0x1c]
        mov    eax, dword ptr [esp + 0x20]
        mov    word ptr [esi + 0x1e], 0
        mov    cl, byte ptr [eax]
        test   cl, 0x10
        jne    loc_1_4D32
        mov    ax, word ptr [esi + 0x2a]
        inc    ax
        cmp    word ptr [esi + 0x34], ax
        mov    word ptr [esi + 0x2a], ax
        ja     loc_1_4D32
        mov    eax, dword ptr [esp + 0x20]
        or     cl, 0x40
        mov    byte ptr [eax], cl
loc_1_4D32:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_4D40:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x1c]
        mov    ax, word ptr [esi + 0x1e]
        cmp    word ptr [ecx + 0xa], ax
        jne    loc_1_4EB3
        mov    ax, word ptr [ecx + 0xc]
        mov    edi, dword ptr [esp + 0x20]
        mov    word ptr [esp + 0x12], ax
        lea    ebp, [edi + 0x24]
loc_1_4D70:
        add    ebp, 6
        mov    ax, word ptr [edi + 0x22]
        inc    ax
        mov    word ptr [edi + 0x22], ax
        cmp    ax, 4
        ja     loc_1_4E68
        cmp    word ptr [esi + 0x78], 0
        je     loc_1_4D96
        mov    ebx, dword ptr [esi + 0x50]
        sub    ebx, 0x34
        jmp    loc_1_4DCD
loc_1_4D96:
        push   esi
        call   sub_1_48C0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_4EA5
        lea    edx, [ebx + 0x34]
        push   edx
        call   sub_1_3B50
        add    esp, 4
        lea    eax, [esi + 0x4c]
        mov    dword ptr [ebx + 0x34], eax
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 0x38], ecx
        lea    edx, [ebx + 0x34]
        mov    dword ptr [ecx], edx
        mov    dword ptr [eax + 4], edx
        mov    byte ptr [ebx + 0x32], 0xd
loc_1_4DCD:
        xor    edx, edx
        mov    dx, word ptr [esi + 0x78]
        xor    eax, eax
        mov    ax, word ptr [ebx + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esp + 0x12]
        sub    eax, edx
        cmp    eax, ecx
        jge    loc_1_4DF0
        mov    cx, word ptr [ebx + 8]
        sub    cx, word ptr [esi + 0x78]
        jmp    loc_1_4DF5
loc_1_4DF0:
        mov    cx, word ptr [esp + 0x12]
loc_1_4DF5:
        mov    eax, dword ptr [ebx + 4]
        add    eax, edx
        mov    dword ptr [ebp], eax
        mov    word ptr [ebp + 4], cx
        sub    word ptr [esp + 0x12], cx
        mov    ax, word ptr [esi + 0x78]
        add    ax, cx
        mov    word ptr [esi + 0x78], ax
        mov    ax, word ptr [esi + 0x1e]
        add    ax, cx
        mov    ecx, dword ptr [esp + 0x1c]
        mov    word ptr [esi + 0x1e], ax
        cmp    word ptr [ecx + 8], ax
        je     loc_1_4E70
        mov    ax, word ptr [ebx + 8]
        cmp    word ptr [esi + 0x78], ax
        jne    loc_1_4E53
        mov    word ptr [esi + 0x78], 0
        mov    byte ptr [ebx + 1], 0x86
        mov    al, byte ptr [esi + 0x2d]
        or     al, al
        je     loc_1_4E4E
        mov    ecx, dword ptr [esi + 0x54]
        sub    ecx, esi
        cmp    ecx, 0x54
        je     loc_1_4E4E
        mov    byte ptr [esi + 0x2f], al
loc_1_4E4E:
        mov    dword ptr [ebx + 0x3c], edi
        jmp    loc_1_4E5A
loc_1_4E53:
        mov    dword ptr [ebx + 0x3c], 0
loc_1_4E5A:
        cmp    word ptr [esp + 0x12], 0
        jne    loc_1_4D70
        jmp    loc_1_4EA9
loc_1_4E68:
        dec    word ptr [edi + 0x22]
        sub    ebx, ebx
        jmp    loc_1_4EA9
loc_1_4E70:
        mov    ax, word ptr [esi + 0x78]
        inc    word ptr [esi + 0x1c]
        mov    word ptr [ebx + 8], ax
        mov    word ptr [esi + 0x78], 0
        mov    word ptr [esi + 0x1e], 0
        mov    byte ptr [ebx + 1], 0x80
        mov    al, byte ptr [esi + 0x2d]
        or     al, al
        je     loc_1_4EA0
        mov    ecx, dword ptr [esi + 0x54]
        sub    ecx, esi
        cmp    ecx, 0x54
        je     loc_1_4EA0
        mov    byte ptr [esi + 0x2f], al
loc_1_4EA0:
        mov    dword ptr [ebx + 0x3c], edi
        jmp    loc_1_4EA9
loc_1_4EA5:
        dec    word ptr [edi + 0x22]
loc_1_4EA9:
        or     ebx, ebx
        jne    loc_1_4EB3
        or     word ptr [esi + 4], 0x400
loc_1_4EB3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_4EC0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x20]
        mov    esi, dword ptr [esp + 0x1c]
        mov    ax, word ptr [eax + 0xc]
        sub    ax, word ptr [esi + 0x1e]
        mov    edi, dword ptr [esp + 0x24]
        mov    word ptr [esp + 0x12], ax
        lea    ebp, [edi + 0x24]
loc_1_4EE3:
        add    ebp, 6
        mov    ax, word ptr [edi + 0x22]
        inc    ax
        mov    word ptr [edi + 0x22], ax
        cmp    ax, 4
        ja     loc_1_5015
        cmp    word ptr [esi + 0x78], 0
        je     loc_1_4F09
        mov    ebx, dword ptr [esi + 0x50]
        sub    ebx, 0x34
        jmp    loc_1_4F40
loc_1_4F09:
        push   esi
        call   sub_1_48C0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_5020
        lea    ecx, [ebx + 0x34]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
        lea    edx, [esi + 0x4c]
        mov    dword ptr [ebx + 0x34], edx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebx + 0x38], eax
        lea    ecx, [ebx + 0x34]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx + 4], ecx
        mov    byte ptr [ebx + 0x32], 0xd
loc_1_4F40:
        xor    eax, eax
        mov    ax, word ptr [esi + 0x78]
        mov    cx, word ptr [ebx + 8]
        mov    dword ptr [esp + 0x14], eax
        mov    edx, ecx
        and    edx, 0xffff
        sub    edx, eax
        xor    eax, eax
        mov    ax, word ptr [esp + 0x12]
        cmp    edx, eax
        jge    loc_1_4F70
        mov    ax, cx
        sub    ax, word ptr [esi + 0x78]
        jmp    loc_1_4F75
        db  08Dh, 064h, 024h, 000h
loc_1_4F70:
        mov    ax, word ptr [esp + 0x12]
loc_1_4F75:
        mov    edx, dword ptr [ebx + 4]
        add    edx, dword ptr [esp + 0x14]
        mov    dword ptr [ebp], edx
        mov    word ptr [ebp + 4], ax
        add    word ptr [esi + 0x1e], ax
        mov    dx, word ptr [esi + 0x78]
        add    dx, ax
        sub    word ptr [esp + 0x12], ax
        mov    word ptr [esi + 0x78], dx
        jne    loc_1_4FD2
        mov    eax, dword ptr [esp + 0x20]
        test   byte ptr [eax], 0x10
        je     loc_1_4FD2
        mov    ax, dx
        mov    word ptr [ebx + 8], ax
        mov    word ptr [esi + 0x78], 0
        mov    word ptr [esi + 0x1e], 0
        mov    al, byte ptr [esi + 0x2d]
        or     al, al
        je     loc_1_4FC9
        mov    ecx, dword ptr [esi + 0x54]
        sub    ecx, esi
        cmp    ecx, 0x54
        je     loc_1_4FC9
        mov    byte ptr [esi + 0x2f], al
loc_1_4FC9:
        mov    byte ptr [ebx + 1], 0x80
        mov    dword ptr [ebx + 0x3c], edi
        jmp    loc_1_5007
loc_1_4FD2:
        cmp    word ptr [esi + 0x78], cx
        jne    loc_1_5000
        mov    word ptr [esi + 0x78], 0
        mov    byte ptr [ebx + 1], 0x86
        mov    al, byte ptr [esi + 0x2d]
        or     al, al
        je     loc_1_4FF6
        mov    ecx, dword ptr [esi + 0x54]
        sub    ecx, esi
        cmp    ecx, 0x54
        je     loc_1_4FF6
        mov    byte ptr [esi + 0x2f], al
loc_1_4FF6:
        mov    dword ptr [ebx + 0x3c], edi
        jmp    loc_1_5007
        db  005h, 000h, 000h, 000h, 000h
loc_1_5000:
        mov    dword ptr [ebx + 0x3c], 0
loc_1_5007:
        cmp    word ptr [esp + 0x12], 0
        jne    loc_1_4EE3
        jmp    loc_1_5024
loc_1_5015:
        dec    word ptr [edi + 0x22]
        sub    ebx, ebx
        jmp    loc_1_5024
        db  08Dh, 049h, 000h
loc_1_5020:
        dec    word ptr [edi + 0x22]
loc_1_5024:
        or     ebx, ebx
        jne    loc_1_5036
        or     word ptr [esi + 4], 0x480
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_5036:
        inc    word ptr [esi + 0x1c]
        mov    eax, dword ptr [esp + 0x20]
        mov    word ptr [esi + 0x1e], 0
        mov    cl, byte ptr [eax]
        test   cl, 0x10
        jne    loc_1_5064
        mov    ax, word ptr [esi + 0x2a]
        inc    ax
        cmp    word ptr [esi + 0x34], ax
        mov    word ptr [esi + 0x2a], ax
        ja     loc_1_5064
        mov    eax, dword ptr [esp + 0x20]
        or     cl, 0x40
        mov    byte ptr [eax], cl
loc_1_5064:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_5070:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        xor    ecx, ecx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    cx, word ptr [esi + 0x3e]
        mov    edi, dword ptr [eax + ecx*4]
        or     edi, edi
        je     loc_1_50E2
        mov    ecx, dword ptr [edi + 0x4c]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -0x4c
        je     loc_1_50DC
loc_1_5096:
        mov    ebx, dword ptr [ecx]
        cmp    dword ptr [ecx + 8], esi
        lea    ebp, [ecx - 0x34]
        jne    loc_1_50D1
        push   ecx
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [ebp + 0x32], 0
        and    byte ptr [ebp + 1], 0x7f
        test   byte ptr [ebp], 0x80
        je     loc_1_50D1
        mov    dword ptr [ebp + 0x34], 0x7890 ; -> obj1:0x7890
        add    ebp, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [0x7894], ebp       ; -> obj1:0x7894
loc_1_50D1:
        mov    ecx, ebx
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -0x4c
        jne    loc_1_5096
loc_1_50DC:
        mov    word ptr [esi + 0x22], 1
loc_1_50E2:
        push   esi
        call   sub_1_3900
        add    esp, 4
        call   sub_1_2E80
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5100:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        sub    bl, bl
        lea    esi, [ebp + 0x8c]
        push   esi
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     loc_1_5125
        mov    bl, byte ptr [eax + 0x14]
        and    bl, 0xc7
        jmp    loc_1_5138
loc_1_5125:
        cmp    byte ptr [esi], 0x2a
        jne    loc_1_5138
        cmp    dword ptr [0x7918], 0x7918    ; -> obj1:0x7918
        je     loc_1_5138
        mov    bl, 0xc4
loc_1_5138:
        or     bl, bl
        je     loc_1_51B9
        lea    edi, [ebp + 0x4c]
        call   sub_1_37F0
        or     eax, eax
        je     loc_1_51B9
        mov    dword ptr [eax + 4], 0x3b20   ; -> loc_1_3B20
        lea    edx, [eax + 0x1c]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [edx], ecx
        mov    cx, word ptr [ebp + 0x20]
        mov    word ptr [edx + 4], cx
        add    ebp, 0x1c
        mov    word ptr [eax + 0x28], 0x50
        mov    byte ptr [eax + 0x51], 4
        lea    edx, [eax + 0x4c]
        add    edi, 0x12
        lea    ebp, [edx + 6]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ebp], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebp + 4], ecx
        add    edx, 0x1e
        mov    cx, word ptr [edi + 8]
        mov    word ptr [ebp + 8], cx
        mov    byte ptr [edx + 0x21], 2
        mov    byte ptr [edx + 1], 2
        mov    byte ptr [edx + 0x20], bl
        mov    ecx, dword ptr [esi]
        mov    dword ptr [edx + 0x22], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edx + 0x26], ecx
        add    edx, 0x22
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [esi + 0xc]
        mov    dword ptr [edx + 0xc], ecx
        push   eax
        call   sub_1_0292
        add    esp, 4
loc_1_51B9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_51C0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x18]
        lea    eax, [ebp + 0x8c]
        add    ebp, 0x4c
        mov    edx, dword ptr [0x78ec]       ; -> obj1:0x78EC
        cmp    edx, 0x78ec                   ; -> obj1:0x78EC
        je     loc_1_5200
loc_1_51E2:
        lea    ebx, [edx - 0x20]
        mov    ecx, 4
        mov    esi, eax
        mov    edi, ebx
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        je     loc_1_5204
        mov    edx, dword ptr [edx]
        cmp    edx, 0x78ec                   ; -> obj1:0x78EC
        jne    loc_1_51E2
        jmp    loc_1_5204
        db  08Bh, 0FFh
loc_1_5200:
        mov    ebx, dword ptr [esp + 0x10]
loc_1_5204:
        cmp    edx, 0x78ec                   ; -> obj1:0x78EC
        je     loc_1_52C2
        mov    cl, byte ptr [ebp + 0x3e]
        mov    al, cl
        and    al, 0xc7
        cmp    al, 0x44
        jne    loc_1_5297
        mov    cl, byte ptr [ebx + 0x28]
        test   cl, 0x80
        jne    loc_1_5274
        or     cl, al
        mov    byte ptr [ebx + 0x28], cl
        lea    eax, [ebx + 0x20]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        lea    ecx, [ebx + 0x10]
        mov    eax, dword ptr [ebp + 0x12]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x16]
        mov    dword ptr [ecx + 4], eax
        mov    ax, word ptr [ebp + 0x1a]
        add    ebp, 0x12
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 0x1c
        lea    edx, [ebx + 0x1a]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        push   ebx
        call   sub_1_29B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_5274:
        or     cl, 2
        mov    byte ptr [ebx + 0x28], cl
        lea    eax, [ebx + 0x20]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   ebx
        call   sub_1_2AC0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_5297:
        and    cl, 0x84
        cmp    cl, 0x84
        jne    loc_1_52C2
        or     byte ptr [ebx + 0x28], al
        test   byte ptr [ebp + 4], 0xf
        mov    eax, 0x78ac                   ; -> obj1:0x78AC
        je     loc_1_52B4
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 0x1c
loc_1_52B4:
        push   eax
        mov    eax, dword ptr [ebp + 0x12]
        push   eax
        push   ebx
        call   sub_1_2C80
        add    esp, 0xc
loc_1_52C2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_52D0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     loc_1_5396
        mov    cl, byte ptr [eax + 0x14]
        test   cl, 4
        je     loc_1_5313
        or     cl, 2
        mov    byte ptr [eax + 0x14], cl
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x15]
        mov    edx, dword ptr [0x7850]       ; -> obj1:0x7850
        mov    byte ptr [ecx + edx], 0xff
        push   eax
        call   sub_1_3760
        add    esp, 4
        pop    esi
        ret    
loc_1_5313:
        mov    ecx, dword ptr [eax + 0x3c]
        or     ecx, ecx
        je     loc_1_534D
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_534D
        mov    byte ptr [ecx + 1], 0x16
        mov    edx, dword ptr [eax + 0x3c]
        test   byte ptr [edx], 0x80
        je     loc_1_534D
        mov    dword ptr [edx + 0x34], 0x7890 ; -> obj1:0x7890
        add    edx, 0x34
        mov    ecx, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ecx], edx
        mov    dword ptr [0x7894], edx       ; -> obj1:0x7894
loc_1_534D:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x15]
        mov    edx, dword ptr [0x7850]       ; -> obj1:0x7850
        mov    byte ptr [ecx + edx], 0xff
        mov    ecx, dword ptr [0x7880]       ; -> obj1:0x7880
        mov    esi, ecx
        or     ecx, ecx
        je     loc_1_5375
loc_1_5368:
        mov    edx, dword ptr [esi + 0x10]
        cmp    edx, eax
        je     loc_1_5375
        mov    esi, edx
        or     esi, esi
        jne    loc_1_5368
loc_1_5375:
        cmp    ecx, eax
        jne    loc_1_5388
        mov    ecx, dword ptr [ecx + 0x10]
        mov    dword ptr [0x7880], ecx       ; -> obj1:0x7880
        mov    byte ptr [eax + 0x14], 0
        pop    esi
        ret    
loc_1_5388:
        or     esi, esi
        je     loc_1_5392
        mov    ecx, dword ptr [eax + 0x10]
        mov    dword ptr [esi + 0x10], ecx
loc_1_5392:
        mov    byte ptr [eax + 0x14], 0
loc_1_5396:
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_53A0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [0x7924]       ; -> obj1:0x7924
        lea    edi, [esi + 0x6a]
        cmp    dword ptr [esi + 0x62], eax
        lea    ebx, [esi + 0x4c]
        jne    loc_1_53D1
        mov    ax, word ptr [0x7928]         ; -> obj1:0x7928
        cmp    word ptr [ebx + 0x1a], ax
        jne    loc_1_53D1
        test   byte ptr [ebx + 4], 0xf
        je     loc_1_54B9
loc_1_53D1:
        lea    eax, [edi + 0x22]
        mov    dword ptr [esp + 0x10], eax
        push   eax
        call   sub_1_3240
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_5480
        mov    al, byte ptr [edi + 0x20]
        and    al, byte ptr [ebp + 0x14]
        test   al, 0x80
        jne    loc_1_5480
        push   esi
        call   sub_1_37F0
        add    esp, 4
        or     eax, eax
        je     loc_1_54B9
        lea    edx, [eax + 0x1c]
        mov    ecx, dword ptr [esi + 0x1c]
        mov    dword ptr [edx], ecx
        mov    cx, word ptr [esi + 0x20]
        mov    word ptr [edx + 4], cx
        add    esi, 0x1c
        mov    word ptr [eax + 0x28], 0x50
        mov    byte ptr [eax + 0x51], 4
        lea    edx, [eax + 0x4c]
        add    ebx, 0x12
        lea    esi, [edx + 6]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], ecx
        add    edx, 0x1e
        mov    cx, word ptr [ebx + 8]
        mov    word ptr [esi + 8], cx
        mov    byte ptr [edx + 0x21], 4
        mov    byte ptr [edx + 1], 4
        add    edx, 0x22
        mov    cl, byte ptr [ebp + 0x14]
        mov    byte ptr [edx - 2], cl
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebp + 4]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xc], ecx
        push   eax
        call   sub_1_0292
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_5480:
        test   byte ptr [edi + 0x20], 0x80
        je     loc_1_54B9
        test   byte ptr [ebx + 4], 0xf
        je     loc_1_54B9
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        push   0x78ec                        ; -> obj1:0x78EC
        call   sub_1_2780
        add    esp, 8
        or     eax, eax
        je     loc_1_54B9
        test   byte ptr [eax + 0x28], 0x80
        je     loc_1_54B9
        add    esi, 0x1c
        push   esi
        mov    ecx, dword ptr [ebx + 0x12]
        push   ecx
        push   eax
        call   sub_1_2C80
        add    esp, 0xc
loc_1_54B9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_54D0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 0x8c
        mov    ebp, dword ptr [0x78c0]       ; -> obj1:0x78C0
        cmp    ebp, 0x78c0                   ; -> obj1:0x78C0
        je     loc_1_5510
        mov    edi, edi
loc_1_54F0:
        lea    ebx, [ebp - 0x20]
        mov    ecx, 4
        mov    esi, eax
        mov    edi, ebx
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        je     loc_1_5514
        mov    ebp, dword ptr [ebp]
        cmp    ebp, 0x78c0                   ; -> obj1:0x78C0
        jne    loc_1_54F0
        jmp    loc_1_5514
        db  08Dh, 049h, 000h
loc_1_5510:
        mov    ebx, dword ptr [esp + 0x10]
loc_1_5514:
        cmp    ebp, 0x78c0                   ; -> obj1:0x78C0
        je     loc_1_5559
        test   byte ptr [ebx + 0x28], 0x80
        jne    loc_1_5559
        push   ebp
        call   sub_1_3B50
        add    esp, 4
        cmp    word ptr [ebx + 0x2a], 0
        je     loc_1_5550
        mov    dword ptr [ebx + 0x2c], 0
        push   ebp
        push   0x78c0                        ; -> obj1:0x78C0
        call   sub_1_3B30
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_5550:
        push   ebx
        call   sub_1_2BC0
        add    esp, 4
loc_1_5559:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5570:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x14]
        mov    esi, dword ptr [0x797c]       ; -> obj1:0x797C
        lea    ebp, [edx + 0x6a]
        lea    eax, [edx + 0x4c]
        mov    cx, word ptr [ebp + 4]
        mov    edi, ecx
        and    edi, 0xff
        mov    ebx, dword ptr [esi + edi*4]
        or     ebx, ebx
        je     loc_1_5630
        cmp    word ptr [ebx + 0x10], cx
        jne    loc_1_5630
        lea    edi, [ebx + 0x3c]
        lea    esi, [edx + 0x5e]
        mov    ecx, 2
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_564B
        mov    cx, word ptr [esi]
        cmp    cx, word ptr [edi]
        jne    loc_1_564B
        mov    cl, byte ptr [ebx + 6]
        cmp    cl, 1
        jne    loc_1_55E1
        test   byte ptr [ebp], 0x80
        je     loc_1_55E1
        push   eax
        push   ebx
        call   sub_1_14C0
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_55E1:
        mov    ax, word ptr [ebx + 0x12]
        cmp    word ptr [ebp + 2], ax
        jne    loc_1_564B
        cmp    cl, 3
        jne    loc_1_564B
        test   byte ptr [ebp], 0x80
        je     loc_1_561D
        test   byte ptr [ebx + 5], 0x80
        push   ebp
        push   ebx
        je     loc_1_5610
        call   sub_1_1650
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_5610:
        call   sub_1_1990
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_561D:
        push   ebp
        push   edx
        push   ebx
        call   sub_1_1C50
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_5630:
        cmp    cx, 0xffff
        jne    loc_1_564B
        cmp    word ptr [ebp + 6], 0
        jne    loc_1_564B
        add    edx, 0x1c
        push   edx
        push   eax
        call   sub_1_11A0
        add    esp, 8
loc_1_564B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_5650:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        add    ecx, 0x6a
        xor    edx, edx
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    dl, byte ptr [ecx + 4]
        mov    esi, dword ptr [eax + edx*4]
        or     esi, esi
        je     loc_1_56A5
        mov    byte ptr [esi + 6], 6
        test   byte ptr [esi + 5], 0x80
        je     loc_1_567D
        push   ecx
        push   esi
        call   sub_1_1650
        add    esp, 8
loc_1_567D:
        mov    eax, dword ptr [esi + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_56A0
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   1
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0xc
        pop    esi
        ret    
loc_1_56A0:
        or     word ptr [esi + 4], 8
loc_1_56A5:
        pop    esi
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_56B0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x6e]
        mov    eax, dword ptr [0x797c]       ; -> obj1:0x797C
        mov    esi, dword ptr [eax + ecx*4]
        or     esi, esi
        je     loc_1_56FE
        mov    eax, dword ptr [esi + 0x68]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_56E6
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_3820
        add    esp, 4
        jmp    loc_1_56EC
loc_1_56E6:
        mov    word ptr [eax + 0x3e], 0xff
loc_1_56EC:
        mov    dword ptr [esi + 0x68], 0
        push   0xa
        push   esi
        call   sub_1_07A0
        add    esp, 8
loc_1_56FE:
        pop    esi
        ret    
loc_1_5700:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   0
        mov    eax, dword ptr [esi + 0x2a]
        push   eax
        call   sub_1_01FA
        add    esp, 8
        push   esi
        call   sub_1_3820
        add    esp, 4
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_5720:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        call   sub_1_37F0
        lea    edi, [ebx + 0x4c]
        mov    esi, eax
        or     esi, esi
        je     loc_1_57CD
        mov    dword ptr [esi + 4], 0x5700   ; -> loc_1_5700
        lea    ecx, [esi + 0x1c]
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [ebx + 0x20]
        mov    word ptr [ecx + 4], ax
        add    ebx, 0x1c
        mov    word ptr [esi + 0x28], 0x20
        mov    byte ptr [esi + 0x51], 4
        lea    ecx, [esi + 0x4c]
        add    edi, 0x12
        lea    edx, [ecx + 6]
        mov    eax, dword ptr [edi]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [edx + 4], eax
        mov    ax, word ptr [edi + 8]
        mov    word ptr [edx + 8], ax
        mov    byte ptr [ecx + 0x1f], 0xa
        mov    eax, dword ptr [0x7984]       ; -> obj1:0x7984
        imul   ax, ax, 0x12
        add    ax, 0x3c
        push   0
        mov    edi, eax
        and    edi, 0xffff
        push   edi
        call   sub_1_01DD
        add    esp, 8
        mov    dword ptr [esi + 0x2a], eax
        or     eax, eax
        je     loc_1_57C4
        push   edi
        push   eax
        call   sub_1_6230
        add    esp, 8
        mov    word ptr [esi + 0x2e], ax
        mov    word ptr [esi + 0x22], 2
        push   esi
        call   sub_1_0292
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_57C4:
        push   esi
        call   sub_1_3820
        add    esp, 4
loc_1_57CD:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_57E0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [0x7980]       ; -> obj1:0x7980
        or     ebp, ebp
        je     loc_1_58B8
        mov    edx, dword ptr [ebp + 0x3c]
        lea    ebx, [ebp + 0x3c]
        or     edx, edx
        je     loc_1_58B8
        mov    eax, dword ptr [esp + 0x18]
        lea    esi, [edx + 0x52]
        lea    eax, [eax + 0x4c]
        mov    edi, eax
        mov    dword ptr [esp + 0x10], eax
        lea    edi, [edi + 0x12]
        mov    ecx, 2
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_58B8
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    loc_1_58B8
        mov    dword ptr [0x7980], 0         ; -> obj1:0x7980
        test   byte ptr [edx + 0x3c], 8
        je     loc_1_5853
        push   edx
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_3820
        add    esp, 4
loc_1_5853:
        mov    eax, dword ptr [esp + 0x10]
        mov    ax, word ptr [eax + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        sub    ax, 0x20
        mov    cx, word ptr [ebp + 8]
        cmp    ax, cx
        jbe    loc_1_5878
        mov    ax, cx
        mov    bl, 6
        jmp    loc_1_587E
loc_1_5878:
        mov    word ptr [ebp + 8], ax
        sub    bl, bl
loc_1_587E:
        push   eax
        mov    eax, dword ptr [ebp + 4]
        push   eax
        mov    eax, dword ptr [esp + 0x20]
        push   0x20
        add    eax, 0x24
        push   eax
        call   sub_1_3B60
        add    esp, 0x10
        mov    byte ptr [ebp + 1], bl
        test   byte ptr [ebp], 0x80
        je     loc_1_58B8
        mov    dword ptr [ebp + 0x34], 0x7890 ; -> obj1:0x7890
        add    ebp, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [0x7894], ebp       ; -> obj1:0x7894
loc_1_58B8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_58C0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x18]
        lea    ebx, [edi + 0x6a]
        mov    ax, word ptr [edi + 0x4e]
        push   eax
        call   sub_1_0187
        add    esp, 4
        lea    ebp, [eax - 0x40]
        lea    eax, [ebx + 0x12]
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     loc_1_59C4
        test   byte ptr [eax + 0x14], 4
        je     loc_1_59C4
        mov    edx, dword ptr [eax + 0x20]
        mov    ecx, edx
        sub    ecx, eax
        cmp    ecx, 0x20
        mov    esi, 0
        je     loc_1_5919
        lea    esi, [edx - 0x34]
        push   edx
        call   sub_1_3B50
        add    esp, 4
loc_1_5919:
        or     esi, esi
        jne    loc_1_5949
        cmp    dword ptr [0x796c], 0x796c    ; -> obj1:0x796C
        mov    esi, 0
        je     loc_1_5945
        mov    esi, dword ptr [0x796c]       ; -> obj1:0x796C
        sub    esi, 0x34
        mov    eax, dword ptr [0x796c]       ; -> obj1:0x796C
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_5945:
        or     esi, esi
        je     loc_1_59C4
loc_1_5949:
        mov    byte ptr [esi + 0x32], 0
        mov    ax, word ptr [esi + 8]
        cmp    bp, ax
        jbe    loc_1_5960
        mov    bp, ax
        mov    byte ptr [esp + 0x13], 6
        jmp    loc_1_5969
loc_1_5960:
        mov    byte ptr [esp + 0x13], 0
        mov    word ptr [esi + 8], bp
loc_1_5969:
        lea    ecx, [esi + 0xa]
        mov    eax, dword ptr [ebx + 2]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 6]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebx + 0xa]
        add    ebx, 2
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        push   ebp
        add    edi, 0x24
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   0x40
        push   edi
        call   sub_1_3B60
        mov    al, byte ptr [esp + 0x23]
        add    esp, 0x10
        mov    byte ptr [esi + 1], al
        test   byte ptr [esi], 0x80
        je     loc_1_59C4
        mov    dword ptr [esi + 0x34], 0x7890 ; -> obj1:0x7890
        add    esi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], esi
        mov    dword ptr [0x7894], esi       ; -> obj1:0x7894
loc_1_59C4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_59D0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esp + 0x14]
        add    edi, 0x6a
        mov    ax, word ptr [eax + 0x4e]
        push   eax
        call   sub_1_0187
        add    esp, 4
        lea    ebp, [eax - 0x40]
        cmp    dword ptr [0x7918], 0x7918    ; -> obj1:0x7918
        mov    esi, 0
        je     loc_1_5A17
        mov    esi, dword ptr [0x7918]       ; -> obj1:0x7918
        sub    esi, 0x34
        mov    eax, dword ptr [0x7918]       ; -> obj1:0x7918
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_5A17:
        or     esi, esi
        je     loc_1_5ABC
        add    edi, 2
loc_1_5A22:
        cmp    word ptr [esi + 8], bp
        mov    bl, 6
        jb     loc_1_5A30
        sub    bl, bl
        mov    word ptr [esi + 8], bp
loc_1_5A30:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x1a], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 0x1e], eax
        lea    ecx, [esi + 0x1a]
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        mov    ax, word ptr [esi + 8]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   0x40
        add    eax, 0x24
        push   eax
        call   sub_1_3B60
        add    esp, 0x10
        mov    byte ptr [esi + 1], bl
        test   byte ptr [esi], 0x80
        je     loc_1_5A8C
        mov    dword ptr [esi + 0x34], 0x7890 ; -> obj1:0x7890
        add    esi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], esi
        mov    dword ptr [0x7894], esi       ; -> obj1:0x7894
loc_1_5A8C:
        cmp    dword ptr [0x7918], 0x7918    ; -> obj1:0x7918
        mov    esi, 0
        je     loc_1_5AB4
        mov    esi, dword ptr [0x7918]       ; -> obj1:0x7918
        sub    esi, 0x34
        mov    eax, dword ptr [0x7918]       ; -> obj1:0x7918
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_5AB4:
        or     esi, esi
        jne    loc_1_5A22
loc_1_5ABC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5AD0:
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [esp + 8]
        add    eax, 0x40
        push   eax
        call   sub_1_3240
        add    esp, 4
        cmp    eax, esi
        je     loc_1_5B00
        call   sub_1_3860
        mov    esi, eax
        or     esi, esi
        je     loc_1_5B00
        mov    dword ptr [esi + 4], 0x3a10   ; -> loc_1_3A10
        mov    dword ptr [esi + 0x40], 0x5100 ; -> loc_1_5100
loc_1_5B00:
        mov    eax, esi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5B10:
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [esp + 8]
        add    eax, 0x40
        push   eax
        push   0x78c0                        ; -> obj1:0x78C0
        call   sub_1_2780
        add    esp, 8
        cmp    eax, esi
        je     loc_1_5B45
        call   sub_1_3860
        mov    esi, eax
        or     esi, esi
        je     loc_1_5B45
        mov    dword ptr [esi + 4], 0x3a10   ; -> loc_1_3A10
        mov    dword ptr [esi + 0x40], 0x54d0 ; -> loc_1_54D0
loc_1_5B45:
        mov    eax, esi
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5B50:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        sub    edi, edi
        lea    ecx, [ebx + 0x40]
        mov    al, byte ptr [ebx + 0x3e]
        lea    esi, [ebx + 0x1e]
        and    al, 0x84
        cmp    al, 0x84
        jne    loc_1_5BA6
        push   ecx
        push   0x78ec                        ; -> obj1:0x78EC
        call   sub_1_2780
        add    esp, 8
        cmp    eax, edi
        je     loc_1_5BBF
        mov    cl, byte ptr [esi + 0x20]
        and    cl, 0xc7
        or     byte ptr [eax + 0x28], cl
        test   byte ptr [ebx + 4], 0xf
        mov    ecx, 0x78ac                   ; -> obj1:0x78AC
        je     loc_1_5B92
        mov    ecx, dword ptr [esp + 0x14]
loc_1_5B92:
        push   ecx
        mov    ecx, dword ptr [ebx + 0x12]
        push   ecx
        push   eax
        call   sub_1_2C80
        add    esp, 0xc
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_5BA6:
        call   sub_1_3860
        mov    edi, eax
        or     edi, edi
        je     loc_1_5BBF
        mov    dword ptr [edi + 4], 0x3a10   ; -> loc_1_3A10
        mov    dword ptr [edi + 0x40], 0x51c0 ; -> loc_1_51C0
loc_1_5BBF:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5BD0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 0x40]
        cmp    byte ptr [ecx + 0x31], 0xff
        jne    loc_1_5C06
        and    byte ptr [ecx + 1], 0x7f
        test   byte ptr [ecx], 0x80
        je     loc_1_5C06
        mov    dword ptr [ecx + 0x34], 0x7890 ; -> obj1:0x7890
        add    ecx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x7894], ecx       ; -> obj1:0x7894
loc_1_5C06:
        push   esi
        call   sub_1_59D0
        add    esp, 4
        push   esi
        call   sub_1_3900
        add    esp, 4
        call   sub_1_2E80
        pop    esi
        ret    
        db  090h
loc_1_5C20:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0x40]
        cmp    byte ptr [edx + 0x31], 0xff
        jne    loc_1_5C55
        and    byte ptr [edx + 1], 0x7f
        test   byte ptr [edx], 0x80
        je     loc_1_5C55
        mov    dword ptr [edx + 0x34], 0x7890 ; -> obj1:0x7890
        add    edx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], edx
        mov    dword ptr [0x7894], edx       ; -> obj1:0x7894
loc_1_5C55:
        push   ecx
        call   sub_1_3900
        add    esp, 4
        jmp    sub_1_2E80
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5C70:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        sub    edi, edi
        lea    ebp, [eax + 0x1e]
        mov    ax, word ptr [eax + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        lea    ebx, [eax - 0x40]
        lea    eax, [ebp + 0x12]
        push   eax
        call   sub_1_3240
        add    esp, 4
        mov    esi, eax
        cmp    esi, edi
        je     loc_1_5D7C
        test   byte ptr [esi + 0x14], 4
        je     loc_1_5D7C
        call   sub_1_3860
        mov    edi, eax
        or     edi, edi
        je     loc_1_5D7C
        mov    ecx, dword ptr [esi + 0x20]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0x20
        mov    esi, 0
        je     loc_1_5CD9
        lea    esi, [ecx - 0x34]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_5CD9:
        or     esi, esi
        jne    loc_1_5D20
        cmp    dword ptr [0x796c], 0x796c    ; -> obj1:0x796C
        mov    esi, 0
        je     loc_1_5D05
        mov    esi, dword ptr [0x796c]       ; -> obj1:0x796C
        sub    esi, 0x34
        mov    eax, dword ptr [0x796c]       ; -> obj1:0x796C
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_5D05:
        or     esi, esi
        jne    loc_1_5D20
        push   edi
        call   sub_1_3900
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5D20:
        mov    byte ptr [esi + 0x32], 0
        mov    ax, word ptr [esi + 8]
        cmp    bx, ax
        jbe    loc_1_5D34
        mov    bx, ax
        mov    al, 6
        jmp    loc_1_5D3A
loc_1_5D34:
        sub    al, al
        mov    word ptr [esi + 8], bx
loc_1_5D3A:
        lea    edx, [esi + 0xa]
        mov    ecx, dword ptr [ebp + 2]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebp + 6]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [ebp + 0xa]
        add    ebp, 2
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xc], ecx
        mov    word ptr [edi + 0x28], 0x40
        or     al, 0x80
        mov    word ptr [edi + 0x2e], bx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edi + 0x2a], ecx
        mov    word ptr [edi + 0x22], 2
        mov    dword ptr [edi + 0x40], esi
        mov    dword ptr [edi + 4], 0x5c20   ; -> loc_1_5C20
        mov    byte ptr [esi + 1], al
loc_1_5D7C:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5D90:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        sub    edi, edi
        lea    ebp, [eax + 0x1e]
        mov    ax, word ptr [eax + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        lea    ebx, [eax - 0x40]
        cmp    dword ptr [0x7918], 0x7918    ; -> obj1:0x7918
        mov    esi, 0
        je     loc_1_5DD5
        mov    esi, dword ptr [0x7918]       ; -> obj1:0x7918
        sub    esi, 0x34
        mov    eax, dword ptr [0x7918]       ; -> obj1:0x7918
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_5DD5:
        or     esi, esi
        je     loc_1_5E64
        call   sub_1_3860
        mov    edi, eax
        or     edi, edi
        jne    loc_1_5E10
        mov    dword ptr [esi + 0x34], 0x7918 ; -> obj1:0x7918
        add    esi, 0x34
        mov    eax, dword ptr [0x791c]       ; -> obj1:0x791C
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x791c]       ; -> obj1:0x791C
        pop    ebp
        mov    dword ptr [eax], esi
        sub    eax, eax
        mov    dword ptr [0x791c], esi       ; -> obj1:0x791C
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_5E10:
        cmp    word ptr [esi + 8], bx
        mov    al, 6
        jb     loc_1_5E1E
        sub    al, al
        mov    word ptr [esi + 8], bx
loc_1_5E1E:
        lea    edx, [esi + 0x1a]
        mov    ecx, dword ptr [ebp + 2]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebp + 6]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [ebp + 0xa]
        add    ebp, 2
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xc], ecx
        mov    word ptr [edi + 0x28], 0x40
        mov    ecx, dword ptr [esi + 4]
        or     al, 0x80
        mov    dword ptr [edi + 0x2a], ecx
        mov    cx, word ptr [esi + 8]
        mov    word ptr [edi + 0x2e], cx
        mov    word ptr [edi + 0x22], 2
        mov    dword ptr [edi + 0x40], esi
        mov    dword ptr [edi + 4], 0x5bd0   ; -> loc_1_5BD0
        mov    byte ptr [esi + 1], al
loc_1_5E64:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_5E70:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0x40]
        cmp    byte ptr [edx + 0x31], 0xff
        jne    loc_1_5EA5
        and    byte ptr [edx + 1], 0x7f
        test   byte ptr [edx], 0x80
        je     loc_1_5EA5
        mov    dword ptr [edx + 0x34], 0x7890 ; -> obj1:0x7890
        add    edx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], edx
        mov    dword ptr [0x7894], edx       ; -> obj1:0x7894
loc_1_5EA5:
        push   ecx
        call   sub_1_3900
        add    esp, 4
        jmp    sub_1_2E80
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5EC0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [0x7980]       ; -> obj1:0x7980
        sub    ebp, ebp
        cmp    ebx, ebp
        je     loc_1_5F95
        lea    eax, [ebx + 0x3c]
        mov    dword ptr [esp + 0x10], eax
        mov    esi, dword ptr [eax]
        cmp    esi, ebp
        je     loc_1_5F95
        mov    edi, dword ptr [esp + 0x18]
        lea    esi, [esi + 0x52]
        lea    edi, [edi + 0x12]
        mov    ecx, 2
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_5F95
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    loc_1_5F95
        call   sub_1_3860
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_5F95
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0x7980], 0         ; -> obj1:0x7980
        mov    eax, dword ptr [eax]
        test   byte ptr [eax + 0x3c], 8
        je     loc_1_5F44
        push   eax
        call   sub_1_3C30
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 4
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_3820
        add    esp, 4
loc_1_5F44:
        mov    eax, dword ptr [esp + 0x18]
        mov    ax, word ptr [eax + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        sub    ax, 0x20
        mov    cx, word ptr [ebx + 8]
        cmp    ax, cx
        jbe    loc_1_5F69
        mov    ax, cx
        mov    cl, 6
        jmp    loc_1_5F6F
loc_1_5F69:
        mov    word ptr [ebx + 8], ax
        sub    cl, cl
loc_1_5F6F:
        mov    word ptr [ebp + 0x22], 2
        mov    word ptr [ebp + 0x28], 0x20
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [ebp + 0x2a], edx
        or     cl, 0x80
        mov    word ptr [ebp + 0x2e], ax
        mov    dword ptr [ebp + 4], 0x5e70   ; -> loc_1_5E70
        mov    dword ptr [ebp + 0x40], ebx
        mov    byte ptr [ebx + 1], cl
loc_1_5F95:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
sub_1_5FA0:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        sub    eax, eax
        cmp    word ptr [edx + 0x1c], 0x5504
        je     loc_1_5FC0
        mov    ecx, dword ptr [0x7844]       ; -> obj1:0x7844
        inc    ecx
        mov    dword ptr [0x7844], ecx       ; -> obj1:0x7844
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_5FC0:
        cmp    byte ptr [edx + 5], 0x14
        jne    loc_1_5FCB
        mov    cl, byte ptr [edx + 0x3f]
        jmp    loc_1_5FCE
loc_1_5FCB:
        mov    cl, byte ptr [edx + 0x1f]
loc_1_5FCE:
        dec    cl
        cmp    cl, 0xc
        jae    loc_1_6012
        xor    eax, eax
        mov    al, cl
        lea    esi, [eax*4]
        mov    ecx, dword ptr [esi + 0x7720] ; -> loc_1_7720
        or     ecx, ecx
        je     loc_1_5FF7
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        push   edx
        call   ecx
        add    esp, 8
        jmp    loc_1_601F
loc_1_5FF7:
        call   sub_1_3860
        or     eax, eax
        je     loc_1_601F
        mov    dword ptr [eax + 4], 0x3a10   ; -> loc_1_3A10
        mov    ecx, dword ptr [esi + 0x76e4] ; -> obj1:0x76E4
        mov    dword ptr [eax + 0x40], ecx
        jmp    loc_1_601F
loc_1_6012:
        mov    ecx, dword ptr [0x7844]       ; -> obj1:0x7844
        inc    ecx
        mov    dword ptr [0x7844], ecx       ; -> obj1:0x7844
loc_1_601F:
        mov    ecx, dword ptr [esp + 0x14]
        pop    esi
        mov    dword ptr [ecx], 0
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_6030:
        mov    edx, dword ptr [0x7980]       ; -> obj1:0x7980
        or     edx, edx
        mov    ecx, dword ptr [esp + 4]
        je     loc_1_6096
        cmp    dword ptr [edx + 0x3c], ecx
        jne    loc_1_6096
        mov    ax, word ptr [ecx + 0x3e]
        or     ax, ax
        je     loc_1_6063
        dec    ax
        mov    word ptr [ecx + 0x3e], ax
        mov    dword ptr [ecx + 4], 0x60a0   ; -> loc_1_60A0
        push   ecx
        call   sub_1_0292
        add    esp, 4
        ret    
loc_1_6063:
        mov    byte ptr [edx + 1], 5
        test   byte ptr [edx], 0x80
        je     loc_1_608B
        mov    dword ptr [edx + 0x34], 0x7890 ; -> obj1:0x7890
        add    edx, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], edx
        mov    dword ptr [0x7894], edx       ; -> obj1:0x7894
loc_1_608B:
        mov    dword ptr [0x7980], 0         ; -> obj1:0x7980
        ret    
loc_1_6096:
        push   ecx
        call   sub_1_3820
        add    esp, 4
        ret    
loc_1_60A0:
        cmp    dword ptr [0x7980], 0         ; -> obj1:0x7980
        mov    eax, dword ptr [esp + 4]
        je     loc_1_60C5
        mov    dword ptr [eax + 4], 0x6030   ; -> loc_1_6030
        mov    ecx, dword ptr [0x7948]       ; -> obj1:0x7948
        push   ecx
        push   eax
        call   sub_1_3BE0
        add    esp, 8
        ret    
loc_1_60C5:
        push   eax
        call   sub_1_3820
        add    esp, 4
        ret    
        db  090h
loc_1_60D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 4], 0
        je     loc_1_612D
        cmp    word ptr [esi + 8], 0x3c
        jb     loc_1_612D
        lea    eax, [esi + 0xa]
        cmp    byte ptr [eax], 0x2a
        je     loc_1_6114
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        jne    loc_1_6114
        mov    dword ptr [esi + 0x3c], 0x6140 ; -> loc_1_6140
        push   esi
        call   sub_1_27D0
        add    esp, 4
        or     eax, eax
        jne    loc_1_6110
        mov    al, 0x21
        pop    esi
        ret    
        db  090h
loc_1_6110:
        mov    al, 0xff
        pop    esi
        ret    
loc_1_6114:
        mov    ax, word ptr [esi + 8]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_6230
        add    esp, 8
        mov    word ptr [esi + 8], ax
        sub    al, al
        pop    esi
        ret    
loc_1_612D:
        mov    al, 1
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_6140:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        or     edi, edi
        jne    loc_1_6152
        mov    al, 5
        jmp    loc_1_61F2
loc_1_6152:
        cmp    dword ptr [0x7980], 0         ; -> obj1:0x7980
        jne    loc_1_61F0
        call   sub_1_37F0
        or     eax, eax
        je     loc_1_61F0
        test   byte ptr [edi + 0x28], 0x80
        jne    loc_1_61E0
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [0x7980], esi       ; -> obj1:0x7980
        mov    dword ptr [esi + 0x3c], eax
        mov    word ptr [eax + 0x28], 0x20
        lea    ecx, [edi + 0x1a]
        lea    ebx, [eax + 0x1c]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebx], edx
        mov    cx, word ptr [ecx + 4]
        mov    word ptr [ebx + 4], cx
        lea    edx, [eax + 0x4c]
        mov    dword ptr [eax + 4], 0x60a0   ; -> loc_1_60A0
        mov    ecx, dword ptr [0x7854]       ; -> obj1:0x7854
        mov    word ptr [eax + 0x3e], cx
        mov    byte ptr [edx + 5], 4
        add    edi, 0x10
        lea    ebx, [edx + 6]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ebx], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebx + 4], ecx
        add    edx, 0x1e
        mov    cx, word ptr [edi + 8]
        mov    word ptr [ebx + 8], cx
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 1], 9
        push   eax
        call   sub_1_0292
        add    esp, 4
        mov    al, 0xff
        jmp    loc_1_61F6
        db  08Bh, 0FFh
loc_1_61E0:
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    al, 5
        jmp    loc_1_61F2
        db  08Dh, 049h, 000h
loc_1_61F0:
        mov    al, 9
loc_1_61F2:
        mov    esi, dword ptr [esp + 0x10]
loc_1_61F6:
        cmp    al, 0xff
        je     loc_1_6221
        mov    byte ptr [esi + 1], al
        test   byte ptr [esi], 0x80
        je     loc_1_6221
        mov    dword ptr [esi + 0x34], 0x7890 ; -> obj1:0x7890
        add    esi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], esi
        mov    dword ptr [0x7894], esi       ; -> obj1:0x7894
loc_1_6221:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6230:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, 0x7924                   ; -> obj1:0x7924
        mov    ecx, dword ptr [esp + 0x14]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ecx + 4], ax
        mov    byte ptr [ecx + 6], 0x40
        mov    byte ptr [ecx + 7], 0
        mov    byte ptr [ecx + 8], 1
        mov    byte ptr [ecx + 9], 0
        mov    eax, dword ptr [0x7864]       ; -> obj1:0x7864
        shr    eax, 9
        mov    word ptr [ecx + 0xa], ax
        mov    eax, dword ptr [0x7844]       ; -> obj1:0x7844
        mov    word ptr [ecx + 0xc], ax
        mov    word ptr [ecx + 0x10], 0
        mov    word ptr [ecx + 0xe], 0
        mov    word ptr [ecx + 0x12], 0
        mov    eax, dword ptr [0x7840]       ; -> obj1:0x7840
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [0x7978]       ; -> obj1:0x7978
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [0x795c]       ; -> obj1:0x795C
        mov    word ptr [ecx + 0x1c], ax
        mov    word ptr [ecx + 0x1e], 0
        mov    dword ptr [ecx + 0x24], 0
        mov    dword ptr [ecx + 0x20], 0
        mov    ax, word ptr [0x7898]         ; -> obj1:0x7898
        mov    word ptr [ecx + 0x28], ax
        mov    word ptr [ecx + 0x2c], ax
        mov    word ptr [ecx + 0x2a], ax
        mov    word ptr [ecx + 0x2e], 0
        mov    dx, word ptr [0x78e4]         ; -> obj1:0x78E4
        mov    ax, dx
        cmp    ax, 0x200
        jb     loc_1_62D9
        mov    ax, 0x200
loc_1_62D9:
        mov    word ptr [ecx + 0x30], ax
        mov    word ptr [ecx + 0x32], 0
        mov    ax, word ptr [0x7944]         ; -> obj1:0x7944
        mov    word ptr [ecx + 0x36], ax
        mov    word ptr [ecx + 0x34], ax
        mov    word ptr [ecx + 0x38], dx
        mov    word ptr [ecx + 0x3a], 0
        cmp    dword ptr [0x7984], 1         ; -> obj1:0x7984
        mov    esi, dword ptr [0x78f4]       ; -> obj1:0x78F4
        jbe    loc_1_6334
        lea    edi, [esi + 0x58]
        mov    edx, dword ptr [0x7984]       ; -> obj1:0x7984
        dec    edx
loc_1_6314:
        test   byte ptr [edi], 0x40
        je     loc_1_632E
        inc    word ptr [ecx + 0x3a]
        mov    eax, dword ptr [edi + 0x24]
        or     eax, eax
        je     loc_1_632E
loc_1_6324:
        inc    word ptr [ecx + 0x32]
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_6324
loc_1_632E:
        add    edi, 0x44
        dec    edx
        jne    loc_1_6314
loc_1_6334:
        mov    eax, 0x4e
        mov    edi, 1
        add    ecx, 0x3c
        cmp    word ptr [esp + 0x18], ax
        jb     loc_1_63E5
        mov    ebx, dword ptr [0x7850]       ; -> obj1:0x7850
loc_1_6352:
        inc    edi
        cmp    edi, 0xff
        jge    loc_1_63E5
        xor    edx, edx
        mov    dl, byte ptr [edi + ebx]
        cmp    dword ptr [0x7984], edx       ; -> obj1:0x7984
        jbe    loc_1_63D3
        mov    ebp, edx
        shl    ebp, 4
        add    ebp, edx
        lea    ebp, [esi + ebp*4]
        mov    edx, dword ptr [ebp]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [ebp + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0xc], edx
        mov    dl, byte ptr [ebp + 0x15]
        mov    byte ptr [ecx + 0x10], dl
        mov    dl, byte ptr [ebp + 0x14]
        test   dl, 4
        je     loc_1_63A4
        lea    ebp, [ecx + 0x11]
        mov    byte ptr [ebp], 4
        jmp    loc_1_63C4
loc_1_63A4:
        test   dl, 1
        je     loc_1_63B2
        lea    ebp, [ecx + 0x11]
        mov    byte ptr [ebp], 5
        jmp    loc_1_63C4
loc_1_63B2:
        test   dl, 2
        lea    ebp, [ecx + 0x11]
        je     loc_1_63C0
        mov    byte ptr [ebp], 6
        jmp    loc_1_63C4
loc_1_63C0:
        mov    byte ptr [ebp], 0
loc_1_63C4:
        test   dl, 0x80
        je     loc_1_63CD
        or     byte ptr [ebp], 0x80
loc_1_63CD:
        add    eax, 0x12
        add    ecx, 0x12
loc_1_63D3:
        mov    edx, dword ptr [esp + 0x18]
        and    edx, 0xffff
        cmp    edx, eax
        jge    loc_1_6352
loc_1_63E5:
        sub    eax, 0x12
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_63F0:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 0x10]
        mov    al, byte ptr [ecx + 0x10]
        mov    byte ptr [edx], al
        mov    al, byte ptr [ecx + 6]
        cmp    al, 1
        jne    loc_1_640C
        mov    byte ptr [edx + 1], 2
        jmp    loc_1_641E
loc_1_640C:
        cmp    al, 6
        je     loc_1_641A
        test   byte ptr [ecx + 4], 0x10
        mov    byte ptr [edx + 1], 3
        je     loc_1_641E
loc_1_641A:
        mov    byte ptr [edx + 1], 4
loc_1_641E:
        lea    esi, [edx + 2]
        mov    eax, dword ptr [ecx + 0x7a]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 0x7e]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ecx + 0x82]
        lea    edi, [ecx + 0x7a]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [esi + 0xc], eax
        lea    esi, [edx + 0x12]
        mov    eax, dword ptr [ecx + 0x8a]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 0x8e]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ecx + 0x92]
        lea    edi, [ecx + 0x8a]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [esi + 0xc], eax
        mov    byte ptr [edx + 0x22], 0
        lea    esi, [ecx + 0x5c]
        mov    byte ptr [edx + 0x23], 0
        mov    eax, ecx
        sub    eax, dword ptr [esi]
        cmp    eax, -0x5c
        je     loc_1_6489
loc_1_647B:
        inc    byte ptr [edx + 0x23]
        mov    esi, dword ptr [esi]
        mov    eax, ecx
        sub    eax, dword ptr [esi]
        cmp    eax, -0x5c
        jne    loc_1_647B
loc_1_6489:
        mov    eax, dword ptr [ecx + 0x54]
        lea    esi, [ecx + 0x54]
        sub    eax, ecx
        cmp    eax, 0x54
        je     loc_1_64A4
loc_1_6496:
        inc    byte ptr [edx + 0x22]
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [esi]
        sub    eax, ecx
        cmp    eax, 0x54
        jne    loc_1_6496
loc_1_64A4:
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_64B0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [eax + 4]
        or     esi, esi
        je     loc_1_6650
        cmp    word ptr [eax + 8], 4
        jb     loc_1_6650
        mov    edi, 4
        lea    ebx, [esi + 4]
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 1], 0
        mov    byte ptr [esi + 3], 0
        add    eax, 0x1a
        mov    byte ptr [esi + 2], 0
        cmp    byte ptr [eax], 0x2a
        jne    loc_1_65A6
        mov    dword ptr [esp + 0x10], 0
        cmp    dword ptr [0x7984], 0         ; -> obj1:0x7984
        je     loc_1_6620
        mov    dword ptr [esp + 0x14], 0
loc_1_6511:
        mov    ecx, dword ptr [esp + 0x14]
        add    ecx, dword ptr [0x78f4]       ; -> obj1:0x78F4
        test   byte ptr [ecx + 0x14], 0x40
        je     loc_1_658B
        mov    eax, dword ptr [ecx + 0x18]
        lea    edx, [ecx + 0x18]
        sub    eax, ecx
        cmp    eax, 0x18
        je     loc_1_653E
        mov    edi, edi
loc_1_6530:
        inc    byte ptr [esi + 3]
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [edx]
        sub    eax, ecx
        cmp    eax, 0x18
        jne    loc_1_6530
loc_1_653E:
        mov    eax, dword ptr [ecx + 0x20]
        lea    edx, [ecx + 0x20]
        sub    eax, ecx
        cmp    eax, 0x20
        je     loc_1_6559
loc_1_654B:
        inc    byte ptr [esi + 2]
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [edx]
        sub    eax, ecx
        cmp    eax, 0x20
        jne    loc_1_654B
loc_1_6559:
        mov    ebp, dword ptr [ecx + 0x38]
        or     ebp, ebp
        je     loc_1_658B
loc_1_6560:
        mov    eax, dword ptr [esp + 0x1c]
        xor    ecx, ecx
        inc    byte ptr [esi + 1]
        mov    cx, word ptr [eax + 8]
        lea    eax, [edi + 0x24]
        cmp    ecx, eax
        jb     loc_1_6584
        push   ebx
        push   ebp
        call   sub_1_63F0
        add    esp, 8
        add    edi, 0x24
        add    ebx, 0x24
loc_1_6584:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    loc_1_6560
loc_1_658B:
        add    dword ptr [esp + 0x14], 0x44
        inc    dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, dword ptr [0x7984]       ; -> obj1:0x7984
        jb     loc_1_6511
        jmp    loc_1_6620
loc_1_65A6:
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     loc_1_6620
        mov    cl, byte ptr [eax + 0x15]
        mov    byte ptr [esi], cl
        mov    ecx, dword ptr [eax + 0x18]
        lea    edx, [eax + 0x18]
        sub    ecx, eax
        cmp    ecx, 0x18
        je     loc_1_65D3
loc_1_65C5:
        inc    byte ptr [esi + 3]
        mov    edx, dword ptr [edx]
        mov    ecx, dword ptr [edx]
        sub    ecx, eax
        cmp    ecx, 0x18
        jne    loc_1_65C5
loc_1_65D3:
        lea    edx, [eax + 0x20]
        mov    ecx, eax
        sub    ecx, dword ptr [edx]
        cmp    ecx, -0x20
        je     loc_1_65EE
loc_1_65E0:
        inc    byte ptr [esi + 2]
        mov    edx, dword ptr [edx]
        mov    ecx, eax
        sub    ecx, dword ptr [edx]
        cmp    ecx, -0x20
        jne    loc_1_65E0
loc_1_65EE:
        mov    ebp, dword ptr [eax + 0x38]
        or     ebp, ebp
        je     loc_1_6620
loc_1_65F5:
        mov    eax, dword ptr [esp + 0x1c]
        xor    ecx, ecx
        inc    byte ptr [esi + 1]
        mov    cx, word ptr [eax + 8]
        lea    eax, [edi + 0x24]
        cmp    ecx, eax
        jb     loc_1_6619
        push   ebx
        push   ebp
        call   sub_1_63F0
        add    esp, 8
        add    edi, 0x24
        add    ebx, 0x24
loc_1_6619:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    loc_1_65F5
loc_1_6620:
        mov    eax, dword ptr [esp + 0x1c]
        mov    word ptr [eax + 8], di
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        shl    eax, 2
        lea    eax, [eax + eax*8]
        sub    eax, edi
        cmp    eax, -4
        jne    loc_1_6644
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_6644:
        mov    al, 6
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_6650:
        mov    al, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_6660:
        mov    al, 0x24
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6670:
        mov    dword ptr [0x7870], 0         ; -> obj1:0x7870
        mov    dword ptr [0x78b4], 0         ; -> obj1:0x78B4
        mov    word ptr [0x7950], 0          ; -> obj1:0x7950
        mov    dword ptr [0x7980], 0         ; -> obj1:0x7980
        call   sub_1_6AA0
        mov    dword ptr [0x7870], eax       ; -> obj1:0x7870
        or     eax, eax
        jne    loc_1_66D4
        or     dword ptr [0x78b4], 2         ; -> obj1:0x78B4
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        je     loc_1_66CA
        call   sub_1_022E
        mov    dword ptr [0x7870], eax       ; -> obj1:0x7870
        or     eax, eax
        je     loc_1_66D4
        mov    byte ptr [0x7960], 0          ; -> obj1:0x7960
loc_1_66CA:
        push   0
        call   sub_1_0201
        add    esp, 4
loc_1_66D4:
        mov    dword ptr [0x7870], eax       ; -> obj1:0x7870
        or     eax, eax
        jne    loc_1_6744
        or     dword ptr [0x78b4], 1         ; -> obj1:0x78B4
        push   0x7920                        ; -> obj1:0x7920
        call   sub_1_02CA
        mov    word ptr [0x792a], 0x5504     ; -> obj1:0x792A
        mov    eax, dword ptr [0x7920]       ; -> obj1:0x7920
        mov    dword ptr [0x78a8], eax       ; -> obj1:0x78A8
        mov    dword ptr [0x78ac], 0xffffffff ; -> obj1:0x78AC
        add    esp, 4
        mov    word ptr [0x78b0], 0xffff     ; -> obj1:0x78B0
        call   sub_1_74F0
        mov    cx, 3
        mov    ax, word ptr [0x78e4]         ; -> obj1:0x78E4
        sub    dx, dx
        div    cx
        add    ax, 0xf
        and    ax, 0xfff0
        mov    word ptr [0x788c], ax         ; -> obj1:0x788C
        call   sub_1_6C90
        mov    dword ptr [0x7870], eax       ; -> obj1:0x7870
loc_1_6744:
        call   sub_1_0081
        mov    eax, dword ptr [0x7870]       ; -> obj1:0x7870
        or     eax, eax
        mov    ecx, dword ptr [0x78b4]       ; -> obj1:0x78B4
        jne    loc_1_6890
        or     ecx, 4
        mov    dword ptr [0x78b4], ecx       ; -> obj1:0x78B4
        mov    dword ptr [0x7858], 0x7858    ; -> obj1:0x7858
        mov    dword ptr [0x785c], 0x7858    ; -> obj1:0x785C
        mov    dword ptr [0x792c], 0x792c    ; -> obj1:0x792C
        mov    dword ptr [0x7930], 0x792c    ; -> obj1:0x7930
        mov    dword ptr [0x7884], 0x7884    ; -> obj1:0x7884
        mov    dword ptr [0x7888], 0x7884    ; -> obj1:0x7888
        mov    dword ptr [0x7890], 0x7890    ; -> obj1:0x7890
        mov    dword ptr [0x7894], 0x7890    ; -> obj1:0x7894
        mov    dword ptr [0x78f8], 0x78f8    ; -> obj1:0x78F8
        mov    dword ptr [0x78fc], 0x78f8    ; -> obj1:0x78FC
        mov    dword ptr [0x796c], 0x796c    ; -> obj1:0x796C
        mov    dword ptr [0x7970], 0x796c    ; -> obj1:0x7970
        mov    dword ptr [0x7874], 0x7874    ; -> obj1:0x7874
        mov    dword ptr [0x7878], 0x7874    ; -> obj1:0x7878
        mov    dword ptr [0x7918], 0x7918    ; -> obj1:0x7918
        mov    dword ptr [0x791c], 0x7918    ; -> obj1:0x791C
        mov    dword ptr [0x78d4], 0x78d4    ; -> obj1:0x78D4
        mov    dword ptr [0x78d8], 0x78d4    ; -> obj1:0x78D8
        mov    dword ptr [0x7848], 0x7848    ; -> obj1:0x7848
        mov    dword ptr [0x784c], 0x7848    ; -> obj1:0x784C
        mov    dword ptr [0x78a0], 0x78a0    ; -> obj1:0x78A0
        mov    dword ptr [0x78a4], 0x78a0    ; -> obj1:0x78A4
        mov    dword ptr [0x7844], 0         ; -> obj1:0x7844
        mov    dword ptr [0x7978], 0         ; -> obj1:0x7978
        mov    dword ptr [0x7840], 0         ; -> obj1:0x7840
        mov    dword ptr [0x786c], 0         ; -> obj1:0x786C
        mov    dword ptr [0x795c], 0         ; -> obj1:0x795C
        mov    dword ptr [0x7864], 0         ; -> obj1:0x7864
        call   sub_1_00E9
        mov    ecx, dword ptr [0x78b4]       ; -> obj1:0x78B4
        or     ecx, 8
        mov    eax, dword ptr [0x7870]       ; -> obj1:0x7870
loc_1_6890:
        or     eax, eax
        jne    loc_1_68D3
        mov    dword ptr [0x78b4], ecx       ; -> obj1:0x78B4
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_68D3
        mov    dword ptr [0x794c], 0         ; -> obj1:0x794C
        lea    ecx, [ecx]
loc_1_68B0:
        call   sub_1_39A0
        or     eax, eax
        mov    ecx, dword ptr [0x78b4]       ; -> obj1:0x78B4
        mov    eax, dword ptr [0x7870]       ; -> obj1:0x7870
        jne    loc_1_68B0
        cmp    dword ptr [0x794c], 0         ; -> obj1:0x794C
        je     loc_1_68D2
        or     ecx, 0x10
        jmp    loc_1_68D3
loc_1_68D2:
        inc    eax
loc_1_68D3:
        mov    dword ptr [0x78b4], ecx       ; -> obj1:0x78B4
        mov    dword ptr [0x7870], eax       ; -> obj1:0x7870
        or     eax, eax
        jne    loc_1_68FB
        mov    eax, dword ptr [0x787c]       ; -> obj1:0x787C
        push   eax
        push   0x7868                        ; -> obj1:0x7868
        call   sub_1_0042
        add    esp, 8
        mov    ecx, dword ptr [0x78b4]       ; -> obj1:0x78B4
loc_1_68FB:
        mov    dword ptr [0x7870], eax       ; -> obj1:0x7870
        or     eax, eax
        jne    loc_1_692A
        or     ecx, 0x20
        mov    dword ptr [0x78b4], ecx       ; -> obj1:0x78B4
        mov    byte ptr [0x7974], 1          ; -> obj1:0x7974
        call   sub_1_3560
        mov    ecx, dword ptr [0x78b4]       ; -> obj1:0x78B4
        or     ecx, 0x8000
        mov    eax, dword ptr [0x7870]       ; -> obj1:0x7870
loc_1_692A:
        mov    dword ptr [0x78b4], ecx       ; -> obj1:0x78B4
        mov    dword ptr [0x7870], eax       ; -> obj1:0x7870
        or     eax, eax
        je     loc_1_69B3
        test   cl, 0x20
        je     loc_1_6954
        xor    eax, eax
        mov    al, byte ptr [0x7868]         ; -> obj1:0x7868
        push   eax
        call   sub_1_0072
        add    esp, 4
        mov    ecx, dword ptr [0x78b4]       ; -> obj1:0x78B4
loc_1_6954:
        mov    dword ptr [0x78b4], ecx       ; -> obj1:0x78B4
        test   cl, 8
        je     loc_1_696A
        call   sub_1_010A
        mov    ecx, dword ptr [0x78b4]       ; -> obj1:0x78B4
loc_1_696A:
        mov    dword ptr [0x78b4], ecx       ; -> obj1:0x78B4
        test   cl, 4
        je     loc_1_6995
        push   0
        mov    eax, dword ptr [0x78e0]       ; -> obj1:0x78E0
        push   eax
        call   sub_1_01FA
        add    esp, 8
        mov    dword ptr [0x78e0], 0         ; -> obj1:0x78E0
        mov    ecx, dword ptr [0x78b4]       ; -> obj1:0x78B4
loc_1_6995:
        mov    dword ptr [0x78b4], ecx       ; -> obj1:0x78B4
        test   cl, 1
        je     loc_1_69A5
        call   sub_1_0258
loc_1_69A5:
        mov    eax, dword ptr [0x7714]       ; -> obj1:0x7714
        dec    eax
        mov    dword ptr [0x7714], eax       ; -> obj1:0x7714
        sub    eax, eax
        ret    
loc_1_69B3:
        mov    eax, dword ptr [0x7714]       ; -> obj1:0x7714
        dec    eax
        mov    dword ptr [0x7714], eax       ; -> obj1:0x7714
        mov    eax, 1
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_69D0:
        sub    esp, 8
        mov    dword ptr [esp], 0
        mov    dword ptr [esp + 4], 4
        lea    eax, [esp + 4]
        push   1
        push   eax
        lea    eax, [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        push   3
        push   eax
        push   0
        mov    eax, dword ptr [0x787c]       ; -> obj1:0x787C
        push   eax
        call   sub_4_0008
        add    esp, 0x1c
        or     eax, eax
        jne    loc_1_6A3D
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [esp + 4], eax
        jb     loc_1_6A3D
        cmp    eax, 1
        jne    loc_1_6A27
        mov    ecx, dword ptr [esp + 0x10]
        mov    al, byte ptr [esp]
        mov    byte ptr [ecx], al
        add    esp, 8
        ret    
loc_1_6A27:
        cmp    eax, 2
        mov    eax, dword ptr [esp]
        mov    ecx, dword ptr [esp + 0x10]
        jne    loc_1_6A3B
        mov    word ptr [ecx], ax
        add    esp, 8
        ret    
loc_1_6A3B:
        mov    dword ptr [ecx], eax
loc_1_6A3D:
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6A50:
        sub    esp, 8
        mov    dword ptr [esp + 4], 4
        lea    eax, [esp + 4]
        push   1
        push   eax
        lea    eax, [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        push   3
        push   eax
        push   0
        mov    eax, dword ptr [0x787c]       ; -> obj1:0x787C
        push   eax
        call   sub_4_0008
        add    esp, 0x1c
        or     eax, eax
        jne    loc_1_6A91
        cmp    byte ptr [esp], 1
        sbb    cl, cl
        inc    cl
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax], cl
loc_1_6A91:
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6AA0:
        mov    byte ptr [0x7868], 0xff       ; -> obj1:0x7868
        push   1
        push   0x7868                        ; -> obj1:0x7868
        push   0x775c                        ; -> loc_1_775C
        call   sub_1_69D0
        add    esp, 0xc
        mov    dword ptr [0x7904], 8         ; -> obj1:0x7904
        push   4
        push   0x7904                        ; -> obj1:0x7904
        push   0x7768                        ; -> loc_1_7768
        call   sub_1_69D0
        add    esp, 0xc
        cmp    dword ptr [0x7904], 4         ; -> obj1:0x7904
        mov    eax, dword ptr [0x7904]       ; -> obj1:0x7904
        jbe    loc_1_6AF0
        add    eax, 8
        jmp    loc_1_6AF3
        db  08Dh, 064h, 024h, 000h
loc_1_6AF0:
        lea    eax, [eax + eax*2]
loc_1_6AF3:
        mov    dword ptr [0x7860], eax       ; -> obj1:0x7860
        mov    dword ptr [0x7984], 0x11      ; -> obj1:0x7984
        push   4
        push   0x7984                        ; -> obj1:0x7984
        push   0x7778                        ; -> loc_1_7778
        call   sub_1_69D0
        add    esp, 0xc
        mov    dword ptr [0x789c], 0x10      ; -> obj1:0x789C
        mov    dword ptr [0x7944], 0xfa      ; -> obj1:0x7944
        push   4
        push   0x7944                        ; -> obj1:0x7944
        push   0x7780                        ; -> obj1:0x7780
        call   sub_1_69D0
        add    esp, 0xc
        mov    dword ptr [0x7910], 0xa       ; -> obj1:0x7910
        push   4
        push   0x7910                        ; -> obj1:0x7910
        push   0x778c                        ; -> loc_1_778C
        call   sub_1_69D0
        add    esp, 0xc
        mov    byte ptr [0x7814], 1          ; -> obj1:0x7814
        push   0x7814                        ; -> obj1:0x7814
        push   0x7798                        ; -> obj1:0x7798
        call   sub_1_6A50
        add    esp, 8
        mov    byte ptr [0x7990], 1          ; -> obj1:0x7990
        push   0x7990                        ; -> obj1:0x7990
        push   0x77a4                        ; -> loc_1_77A4
        call   sub_1_6A50
        add    esp, 8
        mov    byte ptr [0x788e], 1          ; -> obj1:0x788E
        push   0x788e                        ; -> obj1:0x788E
        push   0x77b0                        ; -> obj1:0x77B0
        call   sub_1_6A50
        add    esp, 8
        cmp    byte ptr [0x788e], 0          ; -> obj1:0x788E
        jne    loc_1_6BB7
        mov    byte ptr [0x7990], 0          ; -> obj1:0x7990
loc_1_6BB7:
        mov    byte ptr [0x7960], 1          ; -> obj1:0x7960
        push   0x7960                        ; -> obj1:0x7960
        push   0x77c0                        ; -> loc_1_77C0
        call   sub_1_6A50
        add    esp, 8
        mov    dword ptr [0x7854], 4         ; -> obj1:0x7854
        push   4
        push   0x7854                        ; -> obj1:0x7854
        push   0x77cc                        ; -> obj1:0x77CC
        call   sub_1_69D0
        add    esp, 0xc
        mov    dword ptr [0x7948], 8         ; -> obj1:0x7948
        push   4
        push   0x7948                        ; -> obj1:0x7948
        push   0x77dc                        ; -> loc_1_77DC
        call   sub_1_69D0
        add    esp, 0xc
        mov    dword ptr [0x7994], 0xa       ; -> obj1:0x7994
        push   4
        push   0x7994                        ; -> obj1:0x7994
        push   0x77ec                        ; -> loc_1_77EC
        call   sub_1_69D0
        add    esp, 0xc
        mov    dword ptr [0x7968], 5         ; -> obj1:0x7968
        push   4
        push   0x7968                        ; -> obj1:0x7968
        push   0x77f8                        ; -> loc_1_77F8
        call   sub_1_69D0
        add    esp, 0xc
        mov    dword ptr [0x78e8], 0x32      ; -> obj1:0x78E8
        push   4
        push   0x78e8                        ; -> obj1:0x78E8
        push   0x7804                        ; -> loc_1_7804
        call   sub_1_69D0
        add    esp, 0xc
        mov    dword ptr [0x793c], 1         ; -> obj1:0x793C
        mov    dword ptr [0x7898], 0xc       ; -> obj1:0x7898
        mov    word ptr [0x7991], 0x64       ; -> loc_1_7991
        sub    eax, eax
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6C90:
        push   esi
        mov    edx, dword ptr [0x7984]       ; -> obj1:0x7984
        add    edx, 0x19
        mov    eax, edx
        shl    edx, 4
        add    edx, eax
        mov    eax, dword ptr [0x789c]       ; -> obj1:0x789C
        shl    eax, 2
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        lea    edx, [eax + edx*4]
        mov    eax, dword ptr [0x7910]       ; -> obj1:0x7910
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        lea    edx, [edx + eax*4]
        mov    eax, dword ptr [0x7904]       ; -> obj1:0x7904
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        lea    edx, [edx + eax*4]
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_6CED
        xor    ecx, ecx
        mov    cx, word ptr [0x788c]         ; -> obj1:0x788C
        imul   ecx, dword ptr [0x7860]       ; -> obj1:0x7860
        add    edx, ecx
loc_1_6CED:
        mov    dword ptr [0x781c], edx       ; -> obj1:0x781C
        push   0
        push   edx
        call   sub_1_01DD
        add    esp, 8
        mov    dword ptr [0x78e0], eax       ; -> obj1:0x78E0
        or     eax, eax
        jne    loc_1_6D10
        mov    eax, 0xff
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_6D10:
        mov    dword ptr [0x78e0], eax       ; -> obj1:0x78E0
        mov    esi, eax
        push   0xff
        push   0x100
        push   esi
        call   sub_1_0192
        add    esp, 0xc
        mov    dword ptr [0x7850], esi       ; -> obj1:0x7850
        add    esi, 0x100
        mov    dword ptr [0x797c], esi       ; -> obj1:0x797C
        push   0
        push   0x400
        push   esi
        call   sub_1_0192
        add    esp, 0xc
        add    esi, 0x400
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_6D73
        mov    dword ptr [0x7934], esi       ; -> obj1:0x7934
        xor    ecx, ecx
        mov    cx, word ptr [0x788c]         ; -> obj1:0x788C
        imul   ecx, dword ptr [0x7860]       ; -> obj1:0x7860
        add    esi, ecx
loc_1_6D73:
        mov    dword ptr [0x78f4], esi       ; -> obj1:0x78F4
        mov    eax, dword ptr [0x7984]       ; -> obj1:0x7984
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        lea    esi, [esi + eax*4]
        mov    eax, dword ptr [0x789c]       ; -> obj1:0x789C
        shl    eax, 2
        mov    dword ptr [0x7908], esi       ; -> obj1:0x7908
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    esi, eax
        mov    dword ptr [0x7954], esi       ; -> obj1:0x7954
        mov    eax, dword ptr [0x7910]       ; -> obj1:0x7910
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        lea    esi, [esi + eax*4]
        mov    dword ptr [0x7940], esi       ; -> obj1:0x7940
        mov    eax, dword ptr [0x7904]       ; -> obj1:0x7904
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        lea    eax, [esi + eax*4]
        mov    dword ptr [0x7964], eax       ; -> obj1:0x7964
        call   sub_1_6E00
        call   sub_1_6E60
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_6DE7
        call   sub_1_6F30
loc_1_6DE7:
        call   sub_1_6F00
        call   sub_1_6F90
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6E00:
        push   esi
        push   0
        mov    eax, dword ptr [0x7984]       ; -> obj1:0x7984
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [0x78f4]       ; -> obj1:0x78F4
        push   eax
        call   sub_1_0192
        add    esp, 0xc
        sub    esi, esi
        cmp    dword ptr [0x7984], esi       ; -> obj1:0x7984
        mov    edx, dword ptr [0x78f4]       ; -> obj1:0x78F4
        jbe    loc_1_6E45
        lea    eax, [edx + 0x10]
loc_1_6E34:
        lea    ecx, [eax + 0x34]
        mov    dword ptr [eax], ecx
        add    eax, 0x44
        inc    esi
        cmp    dword ptr [0x7984], esi       ; -> obj1:0x7984
        ja     loc_1_6E34
loc_1_6E45:
        mov    eax, esi
        shl    eax, 4
        add    eax, esi
        mov    dword ptr [edx + eax*4 - 0x34], 0
        mov    dword ptr [0x7880], 0         ; -> obj1:0x7880
        pop    esi
        ret    
sub_1_6E60:
        mov    eax, dword ptr [0x789c]       ; -> obj1:0x789C
        shl    eax, 2
        push   0
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        push   eax
        mov    eax, dword ptr [0x7908]       ; -> obj1:0x7908
        push   eax
        call   sub_1_0192
        mov    dword ptr [0x78c0], 0x78c0    ; -> obj1:0x78C0
        mov    dword ptr [0x78c4], 0x78c0    ; -> obj1:0x78C4
        mov    dword ptr [0x78ec], 0x78ec    ; -> obj1:0x78EC
        mov    dword ptr [0x78f0], 0x78ec    ; -> obj1:0x78F0
        add    esp, 0xc
        mov    dword ptr [0x78cc], 0x78cc    ; -> obj1:0x78CC
        mov    dword ptr [0x78d0], 0x78cc    ; -> obj1:0x78D0
        mov    ecx, dword ptr [0x789c]       ; -> obj1:0x789C
        or     ecx, ecx
        je     loc_1_6EF1
        mov    edx, dword ptr [0x7908]       ; -> obj1:0x7908
        add    edx, 0x20
        mov    edi, edi
loc_1_6ED0:
        mov    dword ptr [edx], 0x78cc       ; -> obj1:0x78CC
        mov    eax, dword ptr [0x78d0]       ; -> obj1:0x78D0
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x78d0]       ; -> obj1:0x78D0
        mov    dword ptr [eax], edx
        mov    dword ptr [0x78d0], edx       ; -> obj1:0x78D0
        add    edx, 0x3c
        dec    ecx
        jne    loc_1_6ED0
loc_1_6EF1:
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6F00:
        push   esi
        mov    eax, dword ptr [0x7964]       ; -> obj1:0x7964
        mov    ecx, eax
loc_1_6F08:
        lea    edx, [ecx + 0xe]
        mov    dword ptr [ecx], edx
        mov    ecx, edx
        mov    esi, eax
        sub    esi, edx
        cmp    esi, 0xfffffe5c
        jne    loc_1_6F08
        mov    dword ptr [ecx - 0xe], 0
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6F30:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [0x7934]       ; -> obj1:0x7934
        mov    esi, dword ptr [0x7860]       ; -> obj1:0x7860
        or     esi, esi
        je     loc_1_6F72
        xor    ebx, ebx
        mov    bx, word ptr [0x788c]         ; -> obj1:0x788C
        mov    dx, word ptr [0x788c]         ; -> obj1:0x788C
        sub    dx, 8
        mov    ecx, esi
        sub    ebp, ebp
        lea    esp, [esp]
loc_1_6F60:
        lea    eax, [edi + ebx]
        mov    dword ptr [edi], eax
        mov    word ptr [edi + 4], dx
        mov    word ptr [edi + 6], bp
        mov    edi, eax
        dec    ecx
        jne    loc_1_6F60
loc_1_6F72:
        xor    eax, eax
        mov    ax, word ptr [0x788c]         ; -> obj1:0x788C
        pop    ebp
        sub    edi, eax
        mov    dword ptr [edi], 0
        mov    dword ptr [0x78c8], esi       ; -> obj1:0x78C8
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_6F90:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [0x7954]       ; -> obj1:0x7954
        mov    dword ptr [0x798c], esi       ; -> obj1:0x798C
        mov    eax, dword ptr [0x7940]       ; -> obj1:0x7940
        mov    dword ptr [0x7988], eax       ; -> obj1:0x7988
        mov    edx, dword ptr [0x7910]       ; -> obj1:0x7910
        add    edx, dword ptr [0x7904]       ; -> obj1:0x7904
        or     edx, edx
        jle    loc_1_7001
        mov    edi, 1
        mov    ebp, 0x5504
        sub    ebx, ebx
loc_1_6FC6:
        mov    word ptr [esi + 0x22], di
        lea    ecx, [esi + 0x4c]
        mov    dword ptr [esi + 0x24], ecx
        mov    word ptr [esi + 0x5c], bp
        mov    word ptr [esi + 0x28], 0x58
        mov    word ptr [esi + 0xa], bp
        lea    ecx, [esi + 0xa4]
        mov    dword ptr [esi + 4], ebx
        mov    byte ptr [esi + 8], bl
        mov    word ptr [esi + 0x3c], bx
        mov    dword ptr [esi + 0x48], ebx
        cmp    eax, ecx
        mov    dword ptr [esi + 0x44], ebx
        mov    dword ptr [esi], ecx
        jne    loc_1_6FFC
        mov    dword ptr [esi], ebx
loc_1_6FFC:
        mov    esi, ecx
        dec    edx
        jne    loc_1_6FC6
loc_1_7001:
        mov    dword ptr [esi - 0xa4], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_7010:
        push   esi
        push   edi
        call   sub_1_70E0
        call   sub_1_0258
        cmp    dword ptr [0x7890], 0x7890    ; -> obj1:0x7890
        je     loc_1_7067
        mov    edi, 0x7890                   ; -> obj1:0x7890
        cmp    dword ptr [0x7890], edi       ; -> obj1:0x7890
        mov    esi, 0
        je     loc_1_7051
loc_1_703A:
        mov    esi, dword ptr [0x7890]       ; -> obj1:0x7890
        sub    esi, 0x34
        mov    eax, dword ptr [0x7890]       ; -> obj1:0x7890
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_7051:
        test   byte ptr [esi], 0x80
        je     loc_1_705F
        push   esi
        call   sub_1_0151
        add    esp, 4
loc_1_705F:
        cmp    dword ptr [0x7890], edi       ; -> obj1:0x7890
        jne    loc_1_703A
loc_1_7067:
        xor    eax, eax
        mov    al, byte ptr [0x7868]         ; -> obj1:0x7868
        push   eax
        call   sub_1_0072
        add    esp, 4
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_7080:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    bl, byte ptr [esp + 0x14]
loc_1_708B:
        mov    eax, dword ptr [esi]
        cmp    esi, eax
        mov    edi, 0
        je     loc_1_70A2
        lea    edi, [eax - 0x34]
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_70A2:
        or     edi, edi
        je     loc_1_70D3
        mov    byte ptr [edi + 1], bl
        mov    byte ptr [edi + 0x32], 0
        test   byte ptr [edi], 0x80
        je     loc_1_708B
        mov    dword ptr [edi + 0x34], 0x7890 ; -> obj1:0x7890
        add    edi, 0x34
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [0x7894]       ; -> obj1:0x7894
        mov    dword ptr [eax], edi
        mov    dword ptr [0x7894], edi       ; -> obj1:0x7894
        jmp    loc_1_708B
loc_1_70D3:
        cmp    edi, 1
        sbb    eax, eax
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_70E0:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x78b4]       ; -> obj1:0x78B4
        and    eax, 0xffff7fff
        mov    dword ptr [0x78b4], eax       ; -> obj1:0x78B4
        call   sub_1_010A
        and    dword ptr [0x78b4], 0xfffffff7 ; -> obj1:0x78B4
        sub    edi, edi
        mov    esi, dword ptr [0x7988]       ; -> obj1:0x7988
        cmp    dword ptr [0x7904], edi       ; -> obj1:0x7904
        jbe    loc_1_715D
        sub    ebx, ebx
loc_1_7110:
        cmp    byte ptr [esi + 8], bl
        je     loc_1_714E
        push   esi
        call   sub_1_02F1
        add    esp, 4
        cmp    eax, ebx
        mov    edx, dword ptr [0x794c]       ; -> obj1:0x794C
        jne    loc_1_7147
        mov    dword ptr [esi + 0x44], 0x7858 ; -> obj1:0x7858
        lea    ecx, [esi + 0x44]
        mov    eax, dword ptr [0x785c]       ; -> obj1:0x785C
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x785c]       ; -> obj1:0x785C
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x785c], ecx       ; -> obj1:0x785C
loc_1_7147:
        dec    edx
        mov    dword ptr [0x794c], edx       ; -> obj1:0x794C
loc_1_714E:
        add    esi, 0xa4
        inc    edi
        cmp    edi, dword ptr [0x7904]       ; -> obj1:0x7904
        jb     loc_1_7110
loc_1_715D:
        and    dword ptr [0x78b4], 0xffffffef ; -> obj1:0x78B4
        push   0xb
        push   0x796c                        ; -> obj1:0x796C
        call   sub_1_7080
        add    esp, 8
        push   0xb
        push   0x7918                        ; -> obj1:0x7918
        call   sub_1_7080
        add    esp, 8
        push   0xb
        push   0x7874                        ; -> obj1:0x7874
        call   sub_1_7080
        add    esp, 8
        push   0xb
        push   0x7848                        ; -> obj1:0x7848
        call   sub_1_7080
        add    esp, 8
        sub    ebx, ebx
        cmp    dword ptr [0x7984], ebx       ; -> obj1:0x7984
        jbe    loc_1_7224
        sub    edi, edi
        lea    esp, [esp]
loc_1_71B0:
        mov    esi, dword ptr [0x78f4]       ; -> obj1:0x78F4
        add    esi, edi
        test   byte ptr [esi + 0x14], 0x40
        je     loc_1_7218
        cmp    dword ptr [esi + 0x38], 0
        je     loc_1_71D8
loc_1_71C4:
        push   0x18
        mov    eax, dword ptr [esi + 0x38]
        push   eax
        call   sub_1_07A0
        add    esp, 8
        cmp    dword ptr [esi + 0x38], 0
        jne    loc_1_71C4
loc_1_71D8:
        test   byte ptr [esi + 0x14], 0x40
        je     loc_1_7218
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0x15]
        add    ecx, dword ptr [0x7850]       ; -> obj1:0x7850
        xor    eax, eax
        mov    al, byte ptr [ecx]
        cmp    dword ptr [0x7984], eax       ; -> obj1:0x7984
        jbe    loc_1_720B
        mov    edx, eax
        shl    edx, 4
        add    edx, eax
        mov    eax, dword ptr [0x78f4]       ; -> obj1:0x78F4
        lea    edx, [eax + edx*4]
        cmp    edx, esi
        jne    loc_1_720B
        mov    byte ptr [ecx], 0xff
loc_1_720B:
        push   esi
        call   sub_1_3760
        add    esp, 4
        mov    byte ptr [esi + 0x14], 0
loc_1_7218:
        add    edi, 0x44
        inc    ebx
        cmp    ebx, dword ptr [0x7984]       ; -> obj1:0x7984
        jb     loc_1_71B0
loc_1_7224:
        mov    edi, 0x78c0                   ; -> obj1:0x78C0
        sub    ebx, ebx
        add    eax, 0
loc_1_7230:
        cmp    dword ptr [0x78c0], edi       ; -> obj1:0x78C0
        mov    esi, ebx
        je     loc_1_7251
        mov    esi, dword ptr [0x78c0]       ; -> obj1:0x78C0
        sub    esi, 0x20
        mov    eax, dword ptr [0x78c0]       ; -> obj1:0x78C0
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_7251:
        cmp    esi, ebx
        je     loc_1_7287
        push   0xb
        lea    eax, [esi + 0x34]
        push   eax
        call   sub_1_7080
        add    esp, 8
        mov    byte ptr [esi + 0x28], bl
        mov    dword ptr [esi + 0x20], 0x78cc ; -> obj1:0x78CC
        add    esi, 0x20
        mov    eax, dword ptr [0x78d0]       ; -> obj1:0x78D0
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x78d0]       ; -> obj1:0x78D0
        mov    dword ptr [eax], esi
        mov    dword ptr [0x78d0], esi       ; -> obj1:0x78D0
        jmp    loc_1_7230
loc_1_7287:
        mov    edi, 0x78ec                   ; -> obj1:0x78EC
        sub    ebx, ebx
        mov    edi, edi
loc_1_7290:
        cmp    dword ptr [0x78ec], edi       ; -> obj1:0x78EC
        mov    esi, ebx
        je     loc_1_72B1
        mov    esi, dword ptr [0x78ec]       ; -> obj1:0x78EC
        sub    esi, 0x20
        mov    eax, dword ptr [0x78ec]       ; -> obj1:0x78EC
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_72B1:
        cmp    esi, ebx
        push   0xb
        je     loc_1_72E7
        lea    eax, [esi + 0x34]
        push   eax
        call   sub_1_7080
        add    esp, 8
        mov    byte ptr [esi + 0x28], bl
        mov    dword ptr [esi + 0x20], 0x78cc ; -> obj1:0x78CC
        add    esi, 0x20
        mov    eax, dword ptr [0x78d0]       ; -> obj1:0x78D0
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x78d0]       ; -> obj1:0x78D0
        mov    dword ptr [eax], esi
        mov    dword ptr [0x78d0], esi       ; -> obj1:0x78D0
        jmp    loc_1_7290
loc_1_72E7:
        push   0x7884                        ; -> obj1:0x7884
        call   sub_1_7080
        add    esp, 8
        mov    edi, 0x792c                   ; -> obj1:0x792C
        lea    esp, [esp]
loc_1_7300:
        cmp    dword ptr [0x792c], edi       ; -> obj1:0x792C
        mov    esi, 0
        je     loc_1_7324
        mov    esi, dword ptr [0x792c]       ; -> obj1:0x792C
        sub    esi, 0x44
        mov    eax, dword ptr [0x792c]       ; -> obj1:0x792C
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_7324:
        or     esi, esi
        je     loc_1_7333
        push   esi
        call   sub_1_39E0
        add    esp, 4
        jmp    loc_1_7300
loc_1_7333:
        mov    esi, 0x7858                   ; -> obj1:0x7858
loc_1_7338:
        cmp    dword ptr [0x7858], esi       ; -> obj1:0x7858
        mov    edi, 0
        je     loc_1_735C
        mov    edi, dword ptr [0x7858]       ; -> obj1:0x7858
        sub    edi, 0x44
        mov    eax, dword ptr [0x7858]       ; -> obj1:0x7858
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_735C:
        or     edi, edi
        je     loc_1_7370
        push   edi
        call   sub_1_39E0
        add    esp, 4
        jmp    loc_1_7338
        db  005h, 000h, 000h, 000h, 000h
loc_1_7370:
        mov    esi, 0x78a0                   ; -> obj1:0x78A0
loc_1_7375:
        cmp    dword ptr [0x78a0], esi       ; -> obj1:0x78A0
        mov    edi, 0
        je     loc_1_7399
        mov    edi, dword ptr [0x78a0]       ; -> obj1:0x78A0
        sub    edi, 0x44
        mov    eax, dword ptr [0x78a0]       ; -> obj1:0x78A0
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_7399:
        or     edi, edi
        je     loc_1_73A8
        push   edi
        call   sub_1_39E0
        add    esp, 4
        jmp    loc_1_7375
loc_1_73A8:
        sub    edi, edi
        mov    esi, dword ptr [0x798c]       ; -> obj1:0x798C
        cmp    dword ptr [0x7910], edi       ; -> obj1:0x7910
        jbe    loc_1_73E4
        sub    ebx, ebx
loc_1_73BA:
        cmp    byte ptr [esi + 8], bl
        je     loc_1_73D5
        push   esi
        call   sub_1_02F1
        add    esp, 4
        cmp    eax, ebx
        jne    loc_1_73D5
        push   esi
        call   sub_1_3820
        add    esp, 4
loc_1_73D5:
        add    esi, 0xa4
        inc    edi
        cmp    edi, dword ptr [0x7910]       ; -> obj1:0x7910
        jb     loc_1_73BA
loc_1_73E4:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_73F0:
        mov    dword ptr [0x7870], 0         ; -> obj1:0x7870
        mov    eax, dword ptr [0x78b4]       ; -> obj1:0x78B4
        test   al, 8
        jne    loc_1_7410
        call   sub_1_00E9
        mov    eax, dword ptr [0x78b4]       ; -> obj1:0x78B4
        or     eax, 8
loc_1_7410:
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_7430
        mov    dword ptr [0x78b4], eax       ; -> obj1:0x78B4
        test   al, 0x10
        jne    loc_1_7430
loc_1_7422:
        call   sub_1_39A0
        or     eax, eax
        mov    eax, dword ptr [0x78b4]       ; -> obj1:0x78B4
        jne    loc_1_7422
loc_1_7430:
        mov    dword ptr [0x78b4], eax       ; -> obj1:0x78B4
        cmp    dword ptr [0x7870], 0         ; -> obj1:0x7870
        jne    loc_1_7446
        or     eax, 0x10
        mov    dword ptr [0x78b4], eax       ; -> obj1:0x78B4
loc_1_7446:
        call   sub_1_3560
        cmp    dword ptr [0x7870], 0         ; -> obj1:0x7870
        jne    loc_1_7463
        mov    eax, dword ptr [0x78b4]       ; -> obj1:0x78B4
        or     eax, 0x8000
        mov    dword ptr [0x78b4], eax       ; -> obj1:0x78B4
loc_1_7463:
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7470:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 3
        je     loc_1_74E1
        cmp    eax, 9
        je     loc_1_74E1
        cmp    eax, 0xa
        je     loc_1_74E1
        cmp    eax, 0xd
        je     loc_1_7490
        cmp    eax, 0xe
        je     loc_1_74A1
        sub    eax, eax
        ret    
loc_1_7490:
        test   byte ptr [0x78b5], 0x80       ; -> loc_1_78B5
        je     loc_1_74E1
        call   sub_1_7010
        sub    eax, eax
        ret    
loc_1_74A1:
        mov    ecx, dword ptr [0x7758]       ; -> loc_1_7758
        cmp    ecx, 1
        jne    loc_1_74DA
        test   byte ptr [0x78b5], 0x80       ; -> loc_1_78B5
        je     loc_1_74C0
        call   sub_1_7010
        mov    ecx, dword ptr [0x7758]       ; -> loc_1_7758
loc_1_74C0:
        mov    eax, dword ptr [0x78e0]       ; -> obj1:0x78E0
        or     eax, eax
        je     loc_1_74DA
        push   0
        push   eax
        call   sub_1_01FA
        add    esp, 8
        mov    ecx, dword ptr [0x7758]       ; -> loc_1_7758
loc_1_74DA:
        dec    ecx
        mov    dword ptr [0x7758], ecx       ; -> loc_1_7758
loc_1_74E1:
        sub    eax, eax
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_74F0:
        call   sub_1_02E2
        or     ax, ax
        je     loc_1_751B
        sub    ax, 0x30
        and    ax, 0xfff0
        cmp    byte ptr [0x7960], 0          ; -> obj1:0x7960
        jne    loc_1_7515
        cmp    ax, 0xc00
        jbe    loc_1_7515
        mov    ax, 0xc00
loc_1_7515:
        mov    word ptr [0x78e4], ax         ; -> obj1:0x78E4
loc_1_751B:
        and    eax, 0xffff
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7530:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        sub    esi, esi
        VxDCall  CONFIGMG._CONFIGMG_Get_Version  ; cd2000003300
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], si
        je     loc_1_756C
        push   esi
        push   esi
        push   0x7470                        ; -> loc_1_7470
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_0000
        add    esp, 0x10
        mov    esi, eax
        or     esi, esi
        jne    loc_1_756C
        mov    dword ptr [0x7750], 1         ; -> loc_1_7750
loc_1_756C:
        mov    eax, dword ptr [0x7758]       ; -> loc_1_7758
        mov    ecx, eax
        inc    eax
        mov    dword ptr [0x7758], eax       ; -> loc_1_7758
        or     ecx, ecx
        jne    loc_1_7593
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0x787c], eax       ; -> obj1:0x787C
        cmp    dword ptr [0x7754], 0         ; -> loc_1_7754
        je     loc_1_7593
        call   sub_1_6670
loc_1_7593:
        push   0x7920                        ; -> obj1:0x7920
        call   sub_1_02CA
        add    esp, 4
        call   sub_1_74F0
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_75B0:
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    eax, dword ptr [ebx]
        add    al, 4
        add    eax, 0x6050505
        add    byte ptr [eax], al
        db  000h
loc_1_75C0:
        clc    
loc_1_75C1:
        add    byte ptr [eax], al
        add    byte ptr [ecx + edi*2], cl    ; -> obj1:0x790C
        add    byte ptr [eax], al
        add    al, 0
loc_1_75CA:
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
        shl    byte ptr [eax], 0
        add    byte ptr [eax], al
loc_1_75F1:
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
        add    byte ptr [eax - 0x4ffffffd], al ; -> loc_1_0380
        add    eax, dword ptr [eax]
        add    byte ptr [eax + 4], al        ; -> loc_1_0480
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x3d], dl     ; -> sub_1_3D50
        add    byte ptr [eax], al
        add    byte ptr [esi], 0
        adc    byte ptr [edi], al
        add    byte ptr [eax], ah            ; -> loc_1_3E20
        add    byte ptr ds:[eax], al
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
        xor    byte ptr [ecx], ah
        add    byte ptr [eax], al
        pushal 
        and    al, byte ptr [eax]
        add    al, ah                        ; -> loc_1_22E0
        and    al, byte ptr [eax]
        add    byte ptr [eax + 0x23], dl     ; -> loc_1_2350
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
        xor    byte ptr [edx], 0
        add    byte ptr [eax + 0x33], al     ; -> loc_1_3340
        add    byte ptr [eax], al
        jo     loc_1_7702
        add    byte ptr [eax], al
        shl    byte ptr [eax], 1
        add    byte ptr [eax - 0x3fffff9c], dh ; -> loc_1_64B0
        xor    byte ptr [eax], al
        add    byte ptr [eax + 0x32], al     ; -> loc_1_3280
        push   ecx
        add    byte ptr [eax], al
        rcl    byte ptr [ecx], 0
        mov    al, byte ptr [0xd0000053]     ; -> loc_1_52D0
        push   edx
        add    byte ptr [eax], al
        rcl    byte ptr [eax + eax]
        jo     0x774f
        add    byte ptr [eax], al
        push   eax
        push   esi
        add    byte ptr [eax], al
        mov    al, 0x56
loc_1_7702:
        add    byte ptr [eax], al
        and    byte ptr [edi], dl
        add    al, ah                        ; -> loc_1_57E0
        push   edi
        add    byte ptr [eax], al
        rcr    byte ptr [eax], 0
        rcr    byte ptr [ecx], 1
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_7720:
        rcr    byte ptr [edx], 1
        add    byte ptr [eax + 0x5b], dl     ; -> loc_1_5B50
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [ebx], bl
        add    byte ptr [eax + 0x1d], dh     ; -> loc_1_1DB0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al                        ; -> loc_1_5EC0
        pop    esi
        add    byte ptr [eax], al
        jo     loc_1_77A6
        add    byte ptr [eax], al
        nop    
        pop    ebp
        add    byte ptr [eax], al
loc_1_7750:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7754:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7758:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_775C:
        dec    esp
        inc    ecx
        dec    esi
        inc    ecx
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7768:
        push   edx
        inc    ebp
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        push   ebx
        add    byte ptr [eax], al
loc_1_7778:
        dec    esi
        inc    ecx
        dec    ebp
        inc    ebp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x45], dl
        push   ebx
        push   ebx
        dec    ecx
        dec    edi
        dec    esi
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_778C:
        push   ebx
        inc    ebp
        dec    esi
        inc    esp
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        push   ebx
        add    byte ptr [ecx + 0x4e], cl
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ebp
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_77A4:
        push   eax
        dec    ecx
loc_1_77A6:
        inc    edi
        inc    edi
        pop    ecx
        inc    edx
        inc    ecx
        inc    ebx
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x53], cl
        inc    ebp
        pop    eax
        push   esp
        inc    ebp
        dec    esi
        push   ebx
        dec    ecx
        dec    edi
        dec    esi
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_77C0:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        inc    ecx
        dec    eax
        inc    ebp
        inc    ecx
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x52], al
        dec    edi
        inc    ecx
        inc    esp
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
loc_1_77DC:
        inc    edx
        push   edx
        dec    edi
        inc    ecx
        inc    esp
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        inc    esp
        inc    ebp
        dec    esp
        inc    ecx
        pop    ecx
        add    byte ptr [eax], al
loc_1_77EC:
        push   edx
        inc    ebp
        push   esp
        push   edx
        pop    ecx
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
loc_1_77F8:
        push   edx
        inc    ebp
        push   esp
        push   edx
        pop    ecx
        inc    esp
        inc    ebp
        dec    esp
        inc    ecx
        pop    ecx
        add    byte ptr [eax], al
loc_1_7804:
        push   esi
        inc    ebp
        push   edx
        dec    ecx
        inc    esi
        pop    ecx
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        dec    edi
        push   ebp
        push   esp
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
loc_1_78B5:
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
loc_1_7991:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x30  R-X
loc_2_0000:
        mov    byte ptr [0x7938], dl         ; -> obj1:0x7938
        clc    
        ret    
loc_2_0008:
        mov    dword ptr [0x7754], 1         ; -> loc_1_7754
        VxDCall  Dev_0487.Service_0000h          ; cd2000008704
        jb     loc_2_002B
        cmp    dword ptr [0x7758], 0         ; -> loc_1_7758
        je     loc_2_002B
        call   sub_1_6670
        cmp    eax, 1
loc_2_002B:
        ret    
loc_2_002C:
        clc    
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16  R-X
        db  0B8h, 000h, 07Ah, 0CDh, 02Fh, 066h, 033h, 0D2h, 03Ch, 0FFh, 075h, 003h, 0BAh, 001h, 000h, 033h
        db  0C0h, 033h, 0DBh, 033h, 0F6h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xe  R-X
sub_4_0000:
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e803300
        add    byte ptr [eax], al
sub_4_0008:
        VxDCall  Dev_2533.Service_803Eh          ; cd203e803325
VxD__LTEXT ENDS
