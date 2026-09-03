; WSIPX -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 3125 instructions recovered by recursive descent

Declare_Virtual_Device WSIPX, 0, 11, WSIPX_Control, 0003Fh, Undefined_Init_Order
;   DDK version 0400h   init_order 054006002h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x26dc  R-X
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  edx, word ptr [esi + 0xa4]
        push   edx
        xor    eax, eax
        mov    ebx, 0
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    ecx
        or     al, al
        jne    loc_1_0026
        jecxz  loc_1_0031
loc_1_0026:
        cmp    al, 0xff
        jne    loc_1_002C
        sub    al, al
loc_1_002C:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
loc_1_0031:
        mov    word ptr [esi + 0xa4], dx
        jmp    loc_1_0026
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  edx, word ptr [esi + 0xa4]
        xor    eax, eax
        mov    ebx, 1
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
sub_1_005D:
        push   ebx
        mov    ebx, 0x16
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 0xa]
        mov    dx, word ptr [eax + 0xcc]
        jmp    loc_1_0089
sub_1_0073:
        push   ebx
        mov    ebx, 3
        jmp    loc_1_0089
sub_1_007B:
        push   ebx
        mov    ebx, 0x17
        jmp    loc_1_0089
sub_1_0083:
        push   ebx
        mov    ebx, 4
loc_1_0089:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        add    esi, 0xe
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        cmp    ebx, 4
        je     loc_1_00A2
        xor    al, al
loc_1_00A2:
        movzx  eax, al
        pop    esi
        pop    edi
        pop    ebp
        pop    ebx
        ret    
sub_1_00AA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0xe
        mov    ebx, 6
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
sub_1_00C7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    ebx, 0x11
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0xe
        xor    al, al
        mov    ah, 1
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        or     al, al
        jne    loc_1_00F4
        sub    esi, 0xe
        mov    esi, dword ptr [esi + 0xa]
        mov    word ptr [esi + 0xcc], dx
loc_1_00F4:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
sub_1_00F9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    ebx, 0x12
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0xe
        xor    al, al
        mov    ah, 1
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
sub_1_011A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    ebx, 0x13
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xa]
        movzx  edx, word ptr [edi + 0xcc]
        add    esi, 0xe
        xor    eax, eax
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
sub_1_0143:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        xor    eax, eax
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ebx, 0x15
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        movzx  eax, al
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
sub_1_0165:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        xor    eax, eax
        mov    ebx, 0x10
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        cmp    al, 0xff
        je     loc_1_0185
        jmp    loc_1_019D
sub_1_017F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
loc_1_0185:
        sub    eax, eax
        sub    edx, edx
        mov    esi, 0x269c                   ; -> loc_1_269C
        add    esi, 2
        mov    ebx, 9
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
loc_1_019D:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
sub_1_01A2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        sub    eax, eax
        sub    edx, edx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, 2
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        movzx  eax, al
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
        db  000h, 000h
sub_1_01C8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        cmp    byte ptr [0x47c], 0           ; -> obj1:0x047C
        jne    loc_1_01E4
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        jmp    loc_1_01EA
loc_1_01E4:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d004000
loc_1_01EA:
        add    esp, 8
        or     eax, eax
        je     loc_1_01F6
loc_1_01F1:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
loc_1_01F6:
        jmp    loc_1_01F1
sub_1_01F8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        cmp    byte ptr [0x47c], 0           ; -> obj1:0x047C
        jne    loc_1_0214
        VMMCall  VMM._HeapFree                   ; cd2051000100
        jmp    loc_1_021A
loc_1_0214:
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e004000
loc_1_021A:
        add    esp, 8
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
sub_1_0222:
        cmp    byte ptr [0x47c], 0           ; -> obj1:0x047C
        je     loc_1_023B
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011004000
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
loc_1_023B:
        ret    
sub_1_023C:
        sub    eax, eax
        cmp    byte ptr [0x47c], 0           ; -> obj1:0x047C
        je     loc_1_025A
        cmp    byte ptr [0x47d], 2           ; -> obj1:0x047D
        jne    loc_1_025A
        push   ebp
        mov    ebp, esp
        VMMCall  VMM._AtEventTime                ; cd2091010100
        pop    ebp
loc_1_025A:
        ret    
loc_1_025B:
        mov    word ptr [edx + 0x8a], 0
        ret    
sub_1_0265:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    eax, 0x1388
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 0x25b                    ; -> loc_1_025B
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        or     esi, esi
        je     loc_1_02D5
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_1_02A1:
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cmp    word ptr [ebx + 0x8a], 0
        je     loc_1_02B9
        VMMCall  VMM.Release_Time_Slice          ; cd2038000100
        jmp    loc_1_02A1
loc_1_02B9:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
loc_1_02D5:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
loc_1_02DA:
        push   edx
        call   sub_1_2147
        add    esp, 4
        ret    
sub_1_02E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    esi, 0x2da                    ; -> loc_1_02DA
        mov    edx, dword ptr [ebp + 8]
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
loc_1_02FD:
        xor    eax, eax
        mov    ebx, 1
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        cld    
        ret    
sub_1_030C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    esi, 0x2fd                    ; -> loc_1_02FD
        mov    edx, dword ptr [ebp + 8]
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
        db  000h, 000h, 000h
WSIPX_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 2
        je     loc_2_008A
        cmp    eax, 0x1b
        je     loc_2_0000
        clc    
        ret    
        db  000h, 000h, 000h
sub_1_0348:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_0387
        or     ebx, ebx
        je     loc_1_0387
        movzx  ecx, ax
        and    ch, 0xf
        mov    esi, ecx
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_038B
        add    eax, esi
loc_1_0387:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_038B:
        xor    eax, eax
        jmp    loc_1_0387
sub_1_038F:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_03CA
        or     ebx, ebx
        je     loc_1_03CA
        movzx  ecx, ax
        and    ch, 0xf
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_03D3
loc_1_03CA:
        mov    eax, 1
loc_1_03CF:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_03D3:
        xor    eax, eax
        jmp    loc_1_03CF
        db  053h, 057h, 056h, 08Bh, 044h, 024h, 010h, 08Bh, 05Ch, 024h, 014h, 00Bh, 0C0h, 074h, 02Bh, 00Bh
        db  0DBh, 074h, 027h, 00Fh, 0B7h, 0C8h, 080h, 0E5h, 00Fh, 003h, 0D9h, 081h, 0C3h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EBh, 00Ch, 0C1h, 0E8h, 00Ch, 08Bh, 0CBh, 06Ah, 000h, 053h, 050h, 0CDh, 020h, 067h
        db  000h, 001h, 000h, 083h, 0C4h, 00Ch, 03Bh, 0C1h, 075h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Eh
        db  05Fh, 05Bh, 0C3h, 033h, 0C0h, 0EBh, 0F8h, 000h, 000h
sub_1_0420:
        VxDCall  Dev_003E.Service_8003h          ; cd2003803e00
        VxDCall  Dev_003E.Service_8004h          ; cd2004803e00
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0003Fh                      ; DDB_Req_Device_Number
        db  0, 11                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'WSIPX   '                  ; DDB_Name
        dd  054006002h                  ; DDB_Init_Order
        dd  WSIPX_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
sub_1_0480:
        mov    eax, dword ptr [0x26c4]       ; -> loc_1_26C4
        or     eax, eax
        jne    loc_1_04C3
        cmp    dword ptr [esp + 4], 0
        jne    loc_1_0494
        sub    eax, eax
        jmp    loc_1_04FF
loc_1_0494:
        mov    dword ptr [0x2640], 1         ; -> obj1:0x2640
        push   dword ptr [0x26c0]            ; -> loc_1_26C0
        call   sub_1_01C8
        add    esp, 4
        or     eax, eax
        je     loc_1_04BF
        mov    ecx, dword ptr [0x26d4]       ; -> loc_1_26D4
        inc    ecx
        mov    dword ptr [0x26d4], ecx       ; -> loc_1_26D4
        jmp    loc_1_04CB
loc_1_04BF:
        sub    eax, eax
        jmp    loc_1_04FF
loc_1_04C3:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x26c4], ecx       ; -> loc_1_26C4
loc_1_04CB:
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    word ptr [eax + 8], 1
        mov    word ptr [eax + 0x30], 0
        mov    edx, dword ptr [0x2650]       ; -> obj1:0x2650
        inc    edx
        mov    dword ptr [0x2650], edx       ; -> obj1:0x2650
        cmp    edx, dword ptr [0x2648]       ; -> obj1:0x2648
        jle    loc_1_04FF
        mov    dword ptr [0x2648], edx       ; -> obj1:0x2648
loc_1_04FF:
        ret    
sub_1_0500:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        test   byte ptr [ebx + 8], 4
        je     loc_1_051A
        push   dword ptr [ebx + 0x32]
        call   sub_1_0617
        add    esp, 4
        jmp    loc_1_0537
loc_1_051A:
        mov    si, word ptr [ebx + 0x30]
        lea    edi, [ebx + 0x32]
        or     si, si
        je     loc_1_0537
loc_1_0526:
        push   dword ptr [edi]
        call   sub_1_0617
        add    esp, 4
        add    edi, 6
        dec    si
        jne    loc_1_0526
loc_1_0537:
        mov    word ptr [ebx + 0x30], 0
        mov    word ptr [ebx + 8], 0
        mov    eax, dword ptr [0x26c4]       ; -> loc_1_26C4
        mov    dword ptr [ebx], eax
        mov    dword ptr [0x26c4], ebx       ; -> loc_1_26C4
        mov    eax, dword ptr [0x2650]       ; -> obj1:0x2650
        or     eax, eax
        je     loc_1_055F
        dec    eax
        mov    dword ptr [0x2650], eax       ; -> obj1:0x2650
loc_1_055F:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0563:
        mov    eax, dword ptr [0x26c8]       ; -> loc_1_26C8
        or     eax, eax
        jne    loc_1_05A5
        cmp    dword ptr [esp + 4], 0
        jne    loc_1_0577
        sub    eax, eax
        jmp    loc_1_05C8
loc_1_0577:
        mov    dword ptr [0x2640], 1         ; -> obj1:0x2640
        push   0x100
        call   sub_1_01C8
        add    esp, 4
        or     eax, eax
        je     loc_1_05A1
        mov    ecx, dword ptr [0x26b8]       ; -> loc_1_26B8
        inc    ecx
        mov    dword ptr [0x26b8], ecx       ; -> loc_1_26B8
        jmp    loc_1_05AD
loc_1_05A1:
        sub    eax, eax
        jmp    loc_1_05C8
loc_1_05A5:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x26c8], ecx       ; -> loc_1_26C8
loc_1_05AD:
        mov    edx, dword ptr [0x264c]       ; -> obj1:0x264C
        inc    edx
        mov    dword ptr [0x264c], edx       ; -> obj1:0x264C
        cmp    edx, dword ptr [0x2644]       ; -> obj1:0x2644
        jle    loc_1_05C8
        mov    dword ptr [0x2644], edx       ; -> obj1:0x2644
loc_1_05C8:
        ret    
sub_1_05C9:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [0x26ac]       ; -> loc_1_26AC
        lea    ebx, [esi + 0x32]
        mov    edi, dword ptr [esp + 0x18]
        cmp    eax, edi
        jge    loc_1_05E3
        mov    edi, eax
loc_1_05E3:
        sub    ebp, ebp
        cmp    edi, ebp
        jle    loc_1_0607
loc_1_05E9:
        push   1
        call   sub_1_0563
        add    esp, 4
        mov    dword ptr [ebx], eax
        mov    word ptr [ebx + 4], 0x100
        or     eax, eax
        je     loc_1_0607
        inc    ebp
        add    ebx, 6
        cmp    ebp, edi
        jl     loc_1_05E9
loc_1_0607:
        mov    word ptr [esi + 0x30], bp
        mov    ax, bp
        shl    ax, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0617:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0x26c8]       ; -> loc_1_26C8
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x26c8], eax       ; -> loc_1_26C8
        mov    eax, dword ptr [0x264c]       ; -> obj1:0x264C
        or     eax, eax
        je     loc_1_0637
        dec    eax
        mov    dword ptr [0x264c], eax       ; -> obj1:0x264C
loc_1_0637:
        ret    
sub_1_0638:
        push   esi
        push   edi
        mov    edi, dword ptr [0x26b4]       ; -> loc_1_26B4
        imul   edi, dword ptr [0x2698]       ; -> loc_1_2698
        cmp    dword ptr [0x2698], 0         ; -> loc_1_2698
        mov    esi, dword ptr [esp + 0xc]
        je     loc_1_0673
        mov    ax, word ptr [esi + 0x86]
        cmp    ax, 0x1e
        je     loc_1_0673
        mov    edi, dword ptr [0x26b8]       ; -> loc_1_26B8
        movsx  ecx, ax
        imul   ecx, dword ptr [0x26ac]       ; -> loc_1_26AC
        sub    edi, ecx
loc_1_0673:
        cmp    dword ptr [0x26b8], edi       ; -> loc_1_26B8
        jle    loc_1_06B6
loc_1_067B:
        push   0
        call   sub_1_0563
        add    esp, 4
        or     eax, eax
        je     loc_1_06B6
        push   eax
        call   sub_1_01F8
        add    esp, 4
        mov    ecx, dword ptr [0x26b8]       ; -> loc_1_26B8
        or     ecx, ecx
        mov    eax, dword ptr [0x264c]       ; -> obj1:0x264C
        je     loc_1_06A2
        dec    ecx
loc_1_06A2:
        or     eax, eax
        je     loc_1_06AC
        dec    eax
        mov    dword ptr [0x264c], eax       ; -> obj1:0x264C
loc_1_06AC:
        mov    dword ptr [0x26b8], ecx       ; -> loc_1_26B8
        cmp    ecx, edi
        jg     loc_1_067B
loc_1_06B6:
        mov    eax, dword ptr [0x2698]       ; -> loc_1_2698
        lea    edi, [eax*4]
        or     eax, eax
        je     loc_1_06DE
        mov    ax, word ptr [esi + 0x86]
        cmp    ax, 0x1e
        je     loc_1_06DE
        mov    edi, dword ptr [0x26d4]       ; -> loc_1_26D4
        movsx  eax, ax
        sub    edi, eax
loc_1_06DE:
        cmp    dword ptr [0x26d4], edi       ; -> loc_1_26D4
        jle    loc_1_0721
loc_1_06E6:
        push   0
        call   sub_1_0480
        add    esp, 4
        or     eax, eax
        je     loc_1_0721
        push   eax
        call   sub_1_01F8
        add    esp, 4
        mov    ecx, dword ptr [0x26d4]       ; -> loc_1_26D4
        or     ecx, ecx
        mov    eax, dword ptr [0x2650]       ; -> obj1:0x2650
        je     loc_1_070D
        dec    ecx
loc_1_070D:
        or     eax, eax
        je     loc_1_0717
        dec    eax
        mov    dword ptr [0x2650], eax       ; -> obj1:0x2650
loc_1_0717:
        mov    dword ptr [0x26d4], ecx       ; -> loc_1_26D4
        cmp    ecx, edi
        jg     loc_1_06E6
loc_1_0721:
        pop    edi
        pop    esi
        ret    
sub_1_0724:
        cmp    dword ptr [0x2640], 0         ; -> obj1:0x2640
        je     loc_1_0745
        call   sub_1_023C
        or     eax, eax
        jne    loc_1_0745
        mov    dword ptr [0x2640], 0         ; -> obj1:0x2640
        call   sub_1_0222
loc_1_0745:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0750:
        push   esi
        push   edi
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x10]
        cmp    ecx, 0x3e8
        jne    loc_1_08CC
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x14]
        cmp    dword ptr [esi + 0x2c], 2
        jne    loc_1_082B
        cmp    ecx, 0x4000
        je     loc_1_07A0
        cmp    ecx, 0x4001
        je     loc_1_07BB
        cmp    ecx, 0x4006
        je     loc_1_07EA
        cmp    ecx, 0x4007
        je     loc_1_0805
        mov    eax, 0x273a
        jmp    loc_1_08F6
loc_1_07A0:
        mov    edi, dword ptr [esp + 0x18]
        mov    cl, byte ptr [esi + 0xa6]
        mov    byte ptr [edi], cl
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], 1
        jmp    loc_1_08F6
loc_1_07BB:
        test   byte ptr [esi + 0xce], 1
        je     loc_1_07E0
        mov    edi, dword ptr [esp + 0x18]
        movzx  ecx, byte ptr [esi + 0xcf]
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], 1
        jmp    loc_1_08F6
loc_1_07E0:
        mov    eax, 0x273a
        jmp    loc_1_08F6
loc_1_07EA:
        mov    ecx, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esp + 0x18]
        mov    dword ptr [ecx], 4
        mov    ecx, dword ptr [0x26b0]       ; -> loc_1_26B0
        mov    dword ptr [edi], ecx
        jmp    loc_1_08F6
loc_1_0805:
        mov    ecx, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esp + 0x18]
        mov    dword ptr [ecx], 0xe
        mov    dword ptr [edi], 0
        add    esi, 0x9a
        add    edi, 4
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        jmp    loc_1_08F6
loc_1_082B:
        cmp    ecx, 0x4002
        je     loc_1_0855
        cmp    ecx, 0x4005
        je     loc_1_0870
        cmp    ecx, 0x4006
        je     loc_1_0891
        cmp    ecx, 0x4007
        je     loc_1_08A9
        mov    eax, 0x273a
        jmp    loc_1_08F6
loc_1_0855:
        mov    edi, dword ptr [esp + 0x18]
        mov    cl, byte ptr [esi + 0xd0]
        mov    byte ptr [edi], cl
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], 1
        jmp    loc_1_08F6
loc_1_0870:
        mov    ecx, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esp + 0x18]
        mov    dword ptr [ecx], 4
        mov    cl, byte ptr [esi + 0xce]
        and    cl, 2
        cmp    cl, 1
        sbb    ecx, ecx
        inc    ecx
        mov    dword ptr [edi], ecx
        jmp    loc_1_08F6
loc_1_0891:
        mov    ecx, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esp + 0x18]
        mov    dword ptr [ecx], 4
        mov    ecx, dword ptr [0x26bc]       ; -> loc_1_26BC
        mov    dword ptr [edi], ecx
        jmp    loc_1_08F6
loc_1_08A9:
        mov    ecx, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esp + 0x18]
        mov    dword ptr [ecx], 0xe
        mov    dword ptr [edi], 0
        add    esi, 0x9a
        add    edi, 4
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        jmp    loc_1_08F6
loc_1_08CC:
        cmp    ecx, 0xffff
        jne    loc_1_08F1
        mov    ecx, dword ptr [esp + 0x14]
        cmp    ecx, 8
        je     loc_1_08F6
        cmp    ecx, 0x10
        je     loc_1_08F6
        cmp    ecx, 0x100
        je     loc_1_08F6
        mov    eax, 0x273a
        jmp    loc_1_08F6
loc_1_08F1:
        mov    eax, 0x2726
loc_1_08F6:
        pop    edi
        pop    esi
        ret    
loc_1_08F9:
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [esp + 0xc]
        cmp    eax, 0x3e8
        jne    loc_1_09B9
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [ecx + 0x2c], 2
        jne    loc_1_098D
        cmp    eax, 0x4000
        je     loc_1_0946
        cmp    eax, 0x4001
        je     loc_1_0957
        cmp    eax, 0x4003
        je     loc_1_096F
        cmp    eax, 0x4004
        je     loc_1_097B
        cmp    eax, 0x4005
        je     loc_1_0984
        mov    esi, 0x273a
        jmp    loc_1_09FE
loc_1_0946:
        mov    eax, dword ptr [esp + 0x14]
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx + 0xa6], al
        jmp    loc_1_09FE
loc_1_0957:
        mov    eax, dword ptr [esp + 0x14]
        or     byte ptr [ecx + 0xce], 1
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx + 0xcf], al
        jmp    loc_1_09FE
loc_1_096F:
        and    byte ptr [ecx + 0xce], 0xfe
        jmp    loc_1_09FE
loc_1_097B:
        or     byte ptr [ecx + 0xce], 4
        jmp    loc_1_09FE
loc_1_0984:
        or     byte ptr [ecx + 0xce], 2
        jmp    loc_1_09FE
loc_1_098D:
        cmp    eax, 0x4002
        je     loc_1_09A2
        cmp    eax, 0x4005
        je     loc_1_09B0
        mov    esi, 0x273a
        jmp    loc_1_09FE
loc_1_09A2:
        mov    eax, dword ptr [esp + 0x14]
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx + 0xd0], al
        jmp    loc_1_09FE
loc_1_09B0:
        or     byte ptr [ecx + 0xce], 2
        jmp    loc_1_09FE
loc_1_09B9:
        cmp    eax, 0xffff
        jne    loc_1_09F9
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, 0x1001
        jne    loc_1_09DA
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 0x34], eax
        jmp    loc_1_09FE
loc_1_09DA:
        cmp    eax, 0x1002
        jne    loc_1_09FE
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 0x38], eax
        push   ecx
        call   sub_1_0A65
        add    esp, 4
        jmp    loc_1_09FE
loc_1_09F9:
        mov    esi, 0x2726
loc_1_09FE:
        mov    eax, esi
        pop    esi
        ret    
loc_1_0A02:
        push   esi
        push   edi
        cmp    dword ptr [esp + 0x10], 0x4004667f
        jne    loc_1_0A5A
        sub    edi, edi
        mov    ecx, dword ptr [esp + 0xc]
        sub    esi, esi
        mov    edx, dword ptr [ecx + 0x7c]
        mov    eax, ecx
        sub    eax, edx
        cmp    eax, -0x7c
        je     loc_1_0A37
loc_1_0A22:
        mov    eax, dword ptr [edx + 0x32]
        inc    edi
        movzx  eax, word ptr [eax + 2]
        add    esi, eax
        mov    edx, dword ptr [edx]
        mov    eax, ecx
        sub    eax, edx
        cmp    eax, -0x7c
        jne    loc_1_0A22
loc_1_0A37:
        test   byte ptr [ecx + 0xce], 2
        jne    loc_1_0A50
        cmp    dword ptr [ecx + 0x2c], 2
        jne    loc_1_0A4B
        imul   edi, edi, -0x1e
        jmp    loc_1_0A4E
loc_1_0A4B:
        imul   edi, edi, -0x2a
loc_1_0A4E:
        add    esi, edi
loc_1_0A50:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax], esi
        sub    eax, eax
        jmp    loc_1_0A5F
loc_1_0A5A:
        mov    eax, 0x2726
loc_1_0A5F:
        pop    edi
        pop    esi
        ret    
loc_1_0A62:
        sub    eax, eax
        ret    
sub_1_0A65:
        push   ebx
        push   esi
        push   edi
        sub    edx, edx
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [0x26cc]       ; -> loc_1_26CC
        mov    eax, dword ptr [edi + 0x38]
        add    eax, ecx
        div    ecx
        mov    ebx, eax
        cmp    ebx, 0x1f4
        jle    loc_1_0A8A
        mov    ebx, 0x1f4
loc_1_0A8A:
        mov    word ptr [edi + 0x86], bx
        sub    esi, esi
        cmp    ebx, esi
        je     loc_1_0ABF
loc_1_0A97:
        push   1
        call   sub_1_0480
        add    esp, 4
        or     eax, eax
        jne    loc_1_0AB8
        call   sub_1_0724
        push   1
        call   sub_1_0480
        add    esp, 4
        or     eax, eax
        je     loc_1_0ABC
loc_1_0AB8:
        mov    dword ptr [eax], esi
        mov    esi, eax
loc_1_0ABC:
        dec    ebx
        jne    loc_1_0A97
loc_1_0ABF:
        or     esi, esi
        je     loc_1_0AD4
loc_1_0AC3:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_1_0500
        add    esp, 4
        or     esi, esi
        jne    loc_1_0AC3
loc_1_0AD4:
        movsx  eax, word ptr [edi + 0x86]
        lea    edi, [eax*2]
        or     edi, edi
        je     loc_1_0B0E
loc_1_0AE6:
        push   1
        call   sub_1_0563
        add    esp, 4
        or     eax, eax
        jne    loc_1_0B07
        call   sub_1_0724
        push   1
        call   sub_1_0563
        add    esp, 4
        or     eax, eax
        je     loc_1_0B0B
loc_1_0B07:
        mov    dword ptr [eax], esi
        mov    esi, eax
loc_1_0B0B:
        dec    edi
        jne    loc_1_0AE6
loc_1_0B0E:
        or     esi, esi
        je     loc_1_0B23
loc_1_0B12:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_1_0617
        add    esp, 4
        or     esi, esi
        jne    loc_1_0B12
loc_1_0B23:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0B30:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 0x12], 0
        mov    ecx, dword ptr [esp + 0x2c]
        mov    eax, ecx
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [esp + 0x14], eax
        mov    ebx, dword ptr [esp + 0x24]
        or     ebx, ebx
        je     loc_1_0D7E
        mov    esi, dword ptr [esp + 0x20]
        or     esi, esi
        je     loc_1_0D7E
        cmp    dword ptr [esp + 0x38], 0
        je     loc_1_0D7E
        mov    eax, dword ptr [esi + 0x2c]
        cmp    eax, 2
        mov    ebp, dword ptr [esp + 0x30]
        jne    loc_1_0B91
        or     ebp, ebp
        je     loc_1_0B91
        cmp    dword ptr [esp + 0x34], 0
        jne    loc_1_0B91
        jmp    loc_1_0D7E
loc_1_0B91:
        mov    edx, dword ptr [esp + 0x38]
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [edx], 0
        mov    edi, dword ptr [esi + 0x7c]
        mov    edx, esi
        sub    edx, edi
        cmp    edx, -0x7c
        jne    loc_1_0BE0
        mov    eax, dword ptr [esi + 0x3c]
        test   al, 2
        je     loc_1_0BD3
        or     eax, 1
        mov    dword ptr [esi + 0x3c], eax
        push   0x20
        push   esi
        push   0
        call   sub_1_24F0
        add    esp, 0xc
        mov    eax, 0x2746
        jmp    loc_1_0D83
loc_1_0BD3:
        mov    dword ptr [esp + 0x18], 0x2733
        jmp    loc_1_0CF5
loc_1_0BE0:
        cmp    eax, 5
        jne    loc_1_0C1F
        cmp    byte ptr [esi + 0x89], 0
        jne    loc_1_0C1F
        mov    eax, dword ptr [esi + 0x3c]
        test   al, 2
        je     loc_1_0C12
        or     eax, 1
        mov    dword ptr [esi + 0x3c], eax
        push   0x20
        push   esi
        push   0
        call   sub_1_24F0
        add    esp, 0xc
        mov    eax, 0x2746
        jmp    loc_1_0D83
loc_1_0C12:
        mov    dword ptr [esp + 0x18], 0x2733
        jmp    loc_1_0CF5
loc_1_0C1F:
        cmp    eax, 2
        jne    loc_1_0C74
        and    cl, 0xb
        lea    eax, [esp + 0x11]
        push   ecx
        push   eax
        push   dword ptr [esp + 0x3c]
        push   ebp
        mov    ebp, dword ptr [esp + 0x38]
        push   ebp
        push   ebx
        push   edi
        call   sub_1_116F
        mov    word ptr [esp + 0x2e], ax
        add    esp, 0x1c
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_0CF5
        mov    eax, dword ptr [esi + 0x7c]
        push   edi
        mov    eax, dword ptr [eax]
        lea    ecx, [esi + 0x7c]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [esi + 0x7c], eax
        dec    word ptr [esi + 0x84]
        call   sub_1_0500
        add    esp, 4
        jmp    loc_1_0CF5
loc_1_0C74:
        mov    ebp, dword ptr [esp + 0x28]
loc_1_0C78:
        or     ebp, ebp
        je     loc_1_0CF5
        push   ebp
        push   ebx
        push   edi
        call   sub_1_1270
        add    word ptr [esp + 0x1e], ax
        add    esp, 0xc
        movzx  ecx, ax
        add    ebx, ecx
        sub    ebp, ecx
        cmp    dword ptr [esi + 0x2c], 5
        jne    loc_1_0CB0
        mov    ecx, dword ptr [edi + 0x32]
        test   byte ptr [ecx + 0x1e], 0x10
        je     loc_1_0CB0
        sub    ebp, ebp
        dec    byte ptr [esi + 0x89]
        mov    ax, word ptr [0x26cc]         ; -> loc_1_26CC
loc_1_0CB0:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_0CE9
        or     ebp, ebp
        jne    loc_1_0CC9
        push   eax
        push   edi
        call   sub_1_1127
        add    esp, 8
        or     eax, eax
        jne    loc_1_0CF5
loc_1_0CC9:
        mov    eax, dword ptr [esi + 0x7c]
        mov    edx, eax
        mov    eax, dword ptr [eax]
        lea    ecx, [esi + 0x7c]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [esi + 0x7c], eax
        dec    word ptr [esi + 0x84]
        push   edx
        call   sub_1_0500
        add    esp, 4
loc_1_0CE9:
        mov    edi, dword ptr [esi + 0x7c]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x7c
        jne    loc_1_0C78
loc_1_0CF5:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_0D08
        mov    edx, dword ptr [esp + 0x38]
        mov    dword ptr [edx], 0
        jmp    loc_1_0D4A
loc_1_0D08:
        mov    edx, dword ptr [esp + 0x38]
        movzx  eax, word ptr [esp + 0x12]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi + 0x2c], 5
        jne    loc_1_0D22
        movzx  eax, byte ptr [esi + 0x89]
        jmp    loc_1_0D29
loc_1_0D22:
        movsx  eax, word ptr [esi + 0x84]
loc_1_0D29:
        or     eax, eax
        je     loc_1_0D46
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_0D4A
        and    dword ptr [esi + 0x50], 0xfffffffe
        push   1
        push   esi
        push   0
        call   sub_1_24F0
        add    esp, 0xc
        jmp    loc_1_0D4A
loc_1_0D46:
        and    dword ptr [esi + 0x50], 0xfffffffe
loc_1_0D4A:
        push   esi
        call   sub_1_1027
        add    esp, 4
        push   esi
        call   sub_1_1027
        add    esp, 4
        test   byte ptr [esi + 0xce], 0x40
        je     loc_1_0D73
        call   sub_1_0724
        push   esi
        call   sub_1_1027
        add    esp, 4
loc_1_0D73:
        call   sub_1_0724
        mov    eax, dword ptr [esp + 0x18]
        jmp    loc_1_0D83
loc_1_0D7E:
        mov    eax, 0x271e
loc_1_0D83:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_0D8B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 1
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi - 4]
        sub    edi, 0xe
        mov    dword ptr [ebp - 8], ecx
        mov    esi, ecx
        mov    eax, dword ptr [edi + 0x32]
        mov    dword ptr [ebp - 0xc], eax
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp - 8]
        dec    byte ptr [ecx + 0x88]
        mov    al, byte ptr [edi + 0x17]
        cmp    al, 0xed
        jne    loc_1_0E1F
        mov    ax, word ptr [edi + 0x1a]
        cmp    word ptr [ecx + 0xcc], ax
        je     loc_1_0DE2
        push   eax
        push   ecx
        call   sub_1_1393
        add    esp, 8
        mov    esi, eax
loc_1_0DE2:
        mov    ax, word ptr [edi + 0x1a]
        cmp    word ptr [esi + 0xcc], ax
        jne    loc_1_0FD8
        cmp    dword ptr [esi + 0x4c], 6
        jne    loc_1_0E04
        mov    dword ptr [esi + 0x4c], 7
        or     dword ptr [esi + 0x3c], 1
loc_1_0E04:
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_0E0E
        and    dword ptr [esi + 0x50], 0xfffffffe
loc_1_0E0E:
        push   1
        push   esi
        push   edi
        call   sub_1_24F0
        add    esp, 0xc
        jmp    loc_1_0FD8
loc_1_0E1F:
        or     al, al
        je     loc_1_0E4C
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 0x58], 0
        je     loc_1_0E30
        and    dword ptr [ecx + 0x50], 0xfffffffe
loc_1_0E30:
        push   1
        push   dword ptr [ebp - 8]
        push   edi
        call   sub_1_24F0
        add    esp, 0xc
        push   edi
        call   sub_1_0500
        add    esp, 4
        jmp    loc_1_1020
loc_1_0E4C:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ebp - 2], ax
        push   eax
        mov    ax, word ptr [ebp - 2]
        xchg   ah, al
        mov    word ptr [ebp - 2], ax
        pop    eax
        mov    cx, word ptr [ebp - 2]
        mov    eax, dword ptr [ebp - 0xc]
        mov    word ptr [eax + 2], cx
        movzx  eax, word ptr [ebp - 2]
        add    eax, 0xff
        cdq    
        and    edx, 0xff
        add    eax, edx
        sar    eax, 8
        mov    word ptr [ebp - 2], ax
        movzx  eax, ax
        mov    ecx, dword ptr [0x26ac]       ; -> loc_1_26AC
        cmp    ecx, eax
        jle    loc_1_0EC8
        imul   eax, eax, 6
        lea    ebx, [edi + eax + 0x32]
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [edi + 0x30], ax
        movzx  eax, word ptr [ebp - 2]
        cmp    eax, ecx
        jge    loc_1_0EC8
loc_1_0EAB:
        push   dword ptr [ebx]
        call   sub_1_0617
        add    esp, 4
        inc    word ptr [ebp - 2]
        add    ebx, 6
        movzx  ecx, word ptr [ebp - 2]
        cmp    ecx, dword ptr [0x26ac]       ; -> loc_1_26AC
        jl     loc_1_0EAB
loc_1_0EC8:
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 0x2c], 2
        jne    loc_1_0EE0
        push   edi
        push   ecx
        call   sub_1_1402
        add    esp, 8
        jmp    loc_1_0FE1
loc_1_0EE0:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0x22]
        cmp    word ptr [ecx + 0xcc], ax
        je     loc_1_0EFF
        push   eax
        push   ecx
        call   sub_1_1393
        add    esp, 8
        mov    esi, eax
loc_1_0EFF:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax + 0x22]
        cmp    word ptr [esi + 0xcc], ax
        jne    loc_1_0FD8
        mov    eax, dword ptr [ebp - 0xc]
        cmp    byte ptr [eax + 0x1f], 0xfe
        jne    loc_1_0F73
        mov    eax, dword ptr [esi + 0x3c]
        or     eax, 2
        mov    dword ptr [esi + 0x3c], eax
        cmp    dword ptr [esi + 0x68], -1
        je     loc_1_0F6B
        mov    ecx, dword ptr [esi + 0x7c]
        sub    ecx, esi
        cmp    ecx, 0x7c
        jne    loc_1_0F53
        or     eax, 1
        mov    dword ptr [esi + 0x3c], eax
        mov    dword ptr [esi + 0x4c], 7
        push   0x20
        push   esi
        push   0
        call   sub_1_24F0
        add    esp, 0xc
        mov    byte ptr [edi + 0x17], 0xed
loc_1_0F53:
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_0F5D
        and    dword ptr [esi + 0x50], 0xfffffffe
loc_1_0F5D:
        push   1
        push   esi
        push   edi
        call   sub_1_24F0
        add    esp, 0xc
        jmp    loc_1_0FD8
loc_1_0F6B:
        or     eax, 1
        mov    dword ptr [esi + 0x3c], eax
        jmp    loc_1_0FD8
loc_1_0F73:
        mov    word ptr [edi + 0x1a], 0
        mov    word ptr [edi + 0x1c], 0x2a
        lea    eax, [esi + 0x7c]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0x80]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 0x80], edi
        inc    word ptr [esi + 0x84]
        cmp    dword ptr [esi + 0x2c], 5
        jne    loc_1_0FBA
        mov    eax, dword ptr [ebp - 0xc]
        test   byte ptr [eax + 0x1e], 0x10
        je     loc_1_0FB3
        inc    byte ptr [esi + 0x89]
        jmp    loc_1_0FBA
loc_1_0FB3:
        mov    dword ptr [ebp - 0x10], 0
loc_1_0FBA:
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_0FC4
        and    dword ptr [esi + 0x50], 0xfffffffe
loc_1_0FC4:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_0FE1
        push   1
        push   esi
        push   edi
        call   sub_1_24F0
        add    esp, 0xc
        jmp    loc_1_0FE1
loc_1_0FD8:
        push   edi
        call   sub_1_0500
        add    esp, 4
loc_1_0FE1:
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ecx + 0x86]
        cmp    word ptr [ecx + 0x84], ax
        jge    loc_1_1016
        push   esi
        call   sub_1_1027
        add    esp, 4
        test   byte ptr [esi + 0xce], 0x40
        je     loc_1_1020
        call   sub_1_0724
        push   esi
        call   sub_1_1027
        add    esp, 4
        jmp    loc_1_1020
loc_1_1016:
        mov    ecx, dword ptr [ebp - 8]
        or     byte ptr [ecx + 0xce], 0x40
loc_1_1020:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_1027:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        cmp    byte ptr [esi + 0x88], 3
        jb     loc_1_103B
        jmp    loc_1_1122
loc_1_103B:
        mov    ax, word ptr [esi + 0x86]
        cmp    word ptr [esi + 0x84], ax
        jl     loc_1_1050
        jmp    loc_1_1122
loc_1_1050:
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_106A
        mov    eax, 0x2747
        jmp    loc_1_1124
loc_1_106A:
        push   dword ptr [0x26ac]            ; -> loc_1_26AC
        push   edi
        call   sub_1_05C9
        add    esp, 8
        movzx  ecx, ax
        cmp    ecx, dword ptr [0x26cc]       ; -> loc_1_26CC
        jge    loc_1_109E
        or     byte ptr [esi + 0xce], 0x40
        push   edi
        call   sub_1_0500
        add    esp, 4
        mov    eax, 0x2747
        jmp    loc_1_1124
loc_1_109E:
        mov    dword ptr [edi + 0xa], esi
        mov    ax, word ptr [esi + 0xa4]
        mov    word ptr [edi + 0x18], ax
        mov    dword ptr [edi + 0x12], 0xd8b ; -> loc_1_0D8B
        lea    eax, [esi + 0x74]
        inc    byte ptr [esi + 0x88]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0x78]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 0x78], edi
        cmp    dword ptr [esi + 0x2c], 2
        push   edi
        jne    loc_1_1113
        call   sub_1_0083
        add    esp, 4
        or     al, al
        je     loc_1_111B
        mov    byte ptr [edi + 0x17], 0xff
        push   1
        push   esi
        push   edi
        call   sub_1_24F0
        add    esp, 0xc
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        dec    byte ptr [esi + 0x88]
        or     byte ptr [esi + 0xce], 0x40
        push   edi
        call   sub_1_0500
        add    esp, 4
        mov    eax, 0x2742
        jmp    loc_1_1124
loc_1_1113:
        call   sub_1_007B
        add    esp, 4
loc_1_111B:
        and    byte ptr [esi + 0xce], 0xbf
loc_1_1122:
        sub    eax, eax
loc_1_1124:
        pop    edi
        pop    esi
        ret    
sub_1_1127:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [esp + 0xc]
        add    cx, word ptr [eax + 0x1c]
        mov    dx, cx
        shr    dx, 8
        add    dx, word ptr [eax + 0x1a]
        mov    word ptr [eax + 0x1a], dx
        and    cx, 0xff
        mov    word ptr [eax + 0x1c], cx
        mov    esi, dword ptr [eax + 0x32]
        movzx  edx, dx
        shl    edx, 8
        movzx  eax, cx
        add    edx, eax
        movzx  eax, word ptr [esi + 2]
        cmp    edx, eax
        mov    eax, 0
        jge    loc_1_116D
        mov    eax, 1
loc_1_116D:
        pop    esi
        ret    
sub_1_116F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 0x32
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 0x10], eax
        mov    esi, dword ptr [ecx + 0x32]
        cmp    byte ptr [esp + 0x30], 0
        mov    bx, word ptr [esi + 2]
        mov    ax, bx
        jne    loc_1_11E0
        mov    bp, word ptr [esp + 0x20]
        cmp    bp, bx
        jb     loc_1_11A3
        mov    bp, bx
loc_1_11A3:
        cmp    bp, 0xe2
        jbe    loc_1_11AE
        mov    bp, 0xe2
loc_1_11AE:
        movzx  edx, bp
        add    esi, 0x1e
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [ebx - 0x1e]
        add    dword ptr [esp + 0x1c], edx
        sub    word ptr [esp + 0x20], bp
        sub    bx, bp
        mov    ecx, dword ptr [esp + 0x18]
        add    ecx, 0x38
        mov    dword ptr [esp + 0x10], ecx
loc_1_11E0:
        cmp    word ptr [esp + 0x20], 0
        je     loc_1_1239
loc_1_11E8:
        or     bx, bx
        je     loc_1_1239
        mov    dx, word ptr [esp + 0x20]
        cmp    dx, bx
        jb     loc_1_11FA
        mov    dx, bx
loc_1_11FA:
        cmp    dx, 0x100
        jbe    loc_1_1205
        mov    dx, 0x100
loc_1_1205:
        movzx  ebp, dx
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [ecx]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [esp + 0x1c], ebp
        sub    word ptr [esp + 0x20], dx
        sub    bx, dx
        add    dword ptr [esp + 0x10], 6
        cmp    word ptr [esp + 0x20], 0
        jne    loc_1_11E8
loc_1_1239:
        mov    edi, dword ptr [esp + 0x24]
        or     edi, edi
        je     loc_1_1268
        mov    ecx, dword ptr [esp + 0x18]
        mov    edx, dword ptr [ecx + 0x32]
        mov    word ptr [edi], 6
        lea    esi, [edx + 0x12]
        add    edi, 2
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [esp + 0x28]
        mov    word ptr [ecx], 0xe
        mov    ecx, dword ptr [esp + 0x2c]
        mov    dl, byte ptr [edx + 5]
        mov    byte ptr [ecx], dl
loc_1_1268:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_1270:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        sub    ax, ax
        mov    bx, word ptr [esi + 0x1a]
        movzx  edi, bx
        mov    ecx, edi
        imul   ecx, ecx, 6
        lea    ecx, [esi + ecx + 0x32]
        mov    dword ptr [esp + 0x14], ecx
        mov    ecx, dword ptr [esi + 0x32]
        mov    bp, word ptr [esi + 0x1c]
        mov    dx, word ptr [ecx + 2]
        movzx  esi, bp
        shl    edi, 8
        add    edi, esi
        movzx  ecx, dx
        cmp    edi, ecx
        jl     loc_1_12B1
        jmp    loc_1_138B
loc_1_12B1:
        mov    cx, bx
        shl    cx, 8
        add    cx, bp
        sub    dx, cx
        mov    word ptr [esp + 0x12], bx
        or     bp, bp
        je     loc_1_131B
        mov    ax, word ptr [esp + 0x24]
        cmp    ax, dx
        jb     loc_1_12D5
        mov    ax, dx
loc_1_12D5:
        mov    ecx, 0x100
        sub    ecx, esi
        movzx  edi, ax
        cmp    ecx, edi
        jge    loc_1_12EA
        mov    ax, 0x100
        sub    ax, bp
loc_1_12EA:
        movzx  ebx, ax
        mov    ecx, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x20]
        add    esi, dword ptr [ecx]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [esp + 0x20], ebx
        sub    word ptr [esp + 0x24], ax
        sub    dx, ax
        add    dword ptr [esp + 0x14], 6
        inc    word ptr [esp + 0x12]
loc_1_131B:
        cmp    word ptr [esp + 0x24], 0
        je     loc_1_138B
loc_1_1323:
        or     dx, dx
        je     loc_1_138B
        mov    esi, dword ptr [esp + 0x1c]
        mov    cx, word ptr [esp + 0x12]
        cmp    word ptr [esi + 0x30], cx
        jbe    loc_1_138B
        mov    bx, word ptr [esp + 0x24]
        cmp    bx, dx
        jb     loc_1_1344
        mov    bx, dx
loc_1_1344:
        cmp    bx, 0x100
        jbe    loc_1_134F
        mov    bx, 0x100
loc_1_134F:
        movzx  ebp, bx
        mov    ecx, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x20]
        mov    esi, dword ptr [ecx]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [esp + 0x20], ebp
        add    ax, bx
        add    dword ptr [esp + 0x14], 6
        sub    dx, bx
        sub    word ptr [esp + 0x24], bx
        inc    word ptr [esp + 0x12]
        cmp    word ptr [esp + 0x24], 0
        jne    loc_1_1323
loc_1_138B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_1393:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xbc]
        sub    eax, 0xbc
        cmp    eax, ecx
        je     loc_1_13C3
        mov    dx, word ptr [esp + 8]
loc_1_13AB:
        cmp    word ptr [eax + 0xcc], dx
        je     loc_1_13C3
        mov    eax, dword ptr [eax + 0xbc]
        sub    eax, 0xbc
        cmp    eax, ecx
        jne    loc_1_13AB
loc_1_13C3:
        ret    
sub_1_13C4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    edx, dword ptr [esp + 0x14]
        add    edx, 0x32
        movzx  ebx, word ptr [eax + 0x30]
        add    eax, 0x32
        or     ebx, ebx
        je     loc_1_13FD
loc_1_13DE:
        movzx  ebp, word ptr [eax + 4]
        mov    edi, dword ptr [edx]
        mov    esi, dword ptr [eax]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    edx, 6
        add    eax, 6
        dec    ebx
        jne    loc_1_13DE
loc_1_13FD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1402:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 0x14], 1
        mov    ecx, dword ptr [edi + 0x32]
        mov    dword ptr [esp + 0x18], 0
        cmp    dword ptr [ecx + 0xa], -1
        lea    eax, [ecx + 4]
        jne    loc_1_1439
        cmp    word ptr [eax + 0xa], 0xffff
        jne    loc_1_1439
        mov    dword ptr [esp + 0x18], 1
loc_1_1439:
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [esp + 0x13], al
        mov    esi, edi
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 0xa], 0
        mov    ebx, dword ptr [esp + 0x20]
loc_1_1453:
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_1_1460
        test   byte ptr [ebx + 0x40], 1
        je     loc_1_14D2
loc_1_1460:
        test   byte ptr [ebx + 0xce], 1
        je     loc_1_1475
        mov    al, byte ptr [esp + 0x13]
        cmp    byte ptr [ebx + 0xcf], al
        jne    loc_1_14D2
loc_1_1475:
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_14C7
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_14D2
        movzx  eax, word ptr [edi + 0x30]
        push   eax
        push   ebp
        call   sub_1_05C9
        add    esp, 8
        movzx  ecx, word ptr [edi + 0x30]
        shl    ecx, 8
        movzx  eax, ax
        cmp    ecx, eax
        jle    loc_1_14B3
        push   ebp
        call   sub_1_0500
        add    esp, 4
        jmp    loc_1_14D2
loc_1_14B3:
        push   edi
        push   ebp
        call   sub_1_13C4
        add    esp, 8
        mov    dword ptr [ebp], esi
        mov    esi, ebp
        mov    dword ptr [ebp + 0xa], ebx
        jmp    loc_1_14D2
loc_1_14C7:
        mov    dword ptr [edi + 0xa], ebx
        mov    dword ptr [esp + 0x14], 0
loc_1_14D2:
        mov    ebx, dword ptr [ebx + 0xbc]
        sub    ebx, 0xbc
        cmp    dword ptr [esp + 0x20], ebx
        jne    loc_1_1453
        or     esi, esi
        je     loc_1_153A
        sub    edi, edi
loc_1_14EE:
        mov    edx, esi
        mov    esi, dword ptr [esi]
        mov    ecx, dword ptr [edx + 0xa]
        cmp    ecx, edi
        je     loc_1_152D
        lea    eax, [ecx + 0x7c]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 0x80]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x80], edx
        inc    word ptr [ecx + 0x84]
        cmp    dword ptr [ecx + 0x58], edi
        je     loc_1_151F
        and    dword ptr [ecx + 0x50], 0xfffffffe
loc_1_151F:
        push   1
        push   ecx
        push   edx
        call   sub_1_24F0
        add    esp, 0xc
        jmp    loc_1_1536
loc_1_152D:
        push   edx
        call   sub_1_0500
        add    esp, 4
loc_1_1536:
        cmp    esi, edi
        jne    loc_1_14EE
loc_1_153A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1550:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x24], 0
        cmp    dword ptr [esp + 0x38], 0
        je     loc_1_191C
        mov    ebx, dword ptr [esp + 0x34]
        or     ebx, ebx
        je     loc_1_191C
        cmp    dword ptr [esp + 0x4c], 0
        je     loc_1_191C
        mov    esi, dword ptr [esp + 0x3c]
        mov    ecx, dword ptr [esp + 0x4c]
        or     esi, esi
        mov    dword ptr [ecx], 0
        jne    loc_1_159A
        sub    eax, eax
        jmp    loc_1_1921
loc_1_159A:
        cmp    dword ptr [ebx + 0x2c], 2
        jne    loc_1_1603
        cmp    dword ptr [0x26b0], esi       ; -> loc_1_26B0
        jae    loc_1_15B2
        mov    eax, 0x2738
        jmp    loc_1_1921
loc_1_15B2:
        cmp    dword ptr [esp + 0x44], 0
        je     loc_1_1603
        cmp    dword ptr [esp + 0x48], 0xe
        jae    loc_1_15C5
        jmp    loc_1_191C
loc_1_15C5:
        mov    ecx, dword ptr [esp + 0x44]
        cmp    word ptr [ecx], 6
        je     loc_1_15D9
        mov    eax, 0x273f
        jmp    loc_1_1921
loc_1_15D9:
        mov    ecx, dword ptr [esp + 0x44]
        cmp    dword ptr [ecx + 6], -1
        jne    loc_1_1603
        cmp    word ptr [ecx + 0xa], 0xffff
        jne    loc_1_1603
        test   byte ptr [ebx + 0x40], 1
        jne    loc_1_15FB
        mov    eax, 0x271d
        jmp    loc_1_1921
loc_1_15FB:
        mov    dword ptr [esp + 0x24], 1
loc_1_1603:
        test   byte ptr [ebx + 0x41], 1
        je     loc_1_161D
        cmp    word ptr [ebx + 0x8a], 0
        je     loc_1_1637
        mov    eax, 0x2747
        jmp    loc_1_1921
loc_1_161D:
        lea    eax, [ebx + 0x8a]
        cmp    word ptr [ebx + 0x8a], 0xa
        jl     loc_1_1637
        mov    eax, 0x2733
        jmp    loc_1_1921
loc_1_1637:
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_1903
        mov    eax, dword ptr [ebx + 0x2c]
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0xfffffff4
        test   byte ptr [ebx + 0x41], 1
        lea    ecx, [eax + 0x2a]
        mov    dword ptr [esp + 0x14], ecx
        je     loc_1_16F3
        push   1
        call   sub_1_0563
        add    esp, 4
        mov    dword ptr [ebp + 0x32], eax
        or     eax, eax
        je     loc_1_18F6
        push   esi
        mov    word ptr [ebp + 0x30], 1
        push   dword ptr [esp + 0x3c]
        call   sub_1_0348
        add    esp, 8
        or     eax, eax
        je     loc_1_18F6
        mov    dword ptr [ebx + 0x8c], eax
        mov    dword ptr [ebx + 0x90], esi
        mov    dword ptr [ebx + 0x94], 0
        mov    word ptr [ebp + 0x30], 2
        or     word ptr [ebp + 8], 4
        mov    cx, word ptr [esp + 0x14]
        mov    word ptr [ebp + 0x36], cx
        mov    ecx, esi
        mov    dword ptr [ebp + 0x38], eax
        mov    eax, dword ptr [0x26cc]       ; -> loc_1_26CC
        sub    eax, dword ptr [esp + 0x14]
        cmp    ecx, eax
        jb     loc_1_16D8
        mov    ecx, eax
loc_1_16D8:
        mov    word ptr [ebp + 0x3c], cx
        cmp    esi, eax
        mov    eax, 0
        setbe  al
        mov    dword ptr [esp + 0x18], esi
        mov    dword ptr [esp + 0x1c], eax
        jmp    loc_1_17C5
loc_1_16F3:
        mov    ecx, dword ptr [0x26cc]       ; -> loc_1_26CC
        sub    ecx, dword ptr [esp + 0x14]
        cmp    esi, ecx
        mov    eax, 0
        setbe  al
        mov    dword ptr [esp + 0x1c], eax
        jb     loc_1_170F
        mov    esi, ecx
loc_1_170F:
        mov    dword ptr [esp + 0x10], esi
        mov    esi, dword ptr [esp + 0x14]
        add    esi, dword ptr [esp + 0x10]
        lea    eax, [esi + 0xff]
        shr    eax, 8
        push   eax
        push   ebp
        call   sub_1_05C9
        add    esp, 8
        movzx  eax, ax
        cmp    eax, esi
        jb     loc_1_18F6
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0x18], eax
        mov    edx, 0x100
        sub    edx, dword ptr [esp + 0x14]
        cmp    edx, eax
        jb     loc_1_1750
        mov    edx, eax
loc_1_1750:
        lea    eax, [ebp + 0x32]
        mov    edi, dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x38]
        mov    ecx, edx
        shr    ecx, 2
        add    edi, dword ptr [ebp + 0x32]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    dword ptr [esp + 0x10], edx
        mov    ax, word ptr [esp + 0x14]
        add    dword ptr [esp + 0x38], edx
        add    ax, dx
        cmp    dword ptr [esp + 0x10], 0
        mov    word ptr [ebp + 0x36], ax
        lea    eax, [ebp + 0x38]
        je     loc_1_17C5
loc_1_178A:
        mov    edx, dword ptr [esp + 0x10]
        cmp    edx, 0x100
        jb     loc_1_179B
        mov    edx, 0x100
loc_1_179B:
        mov    edi, dword ptr [eax]
        mov    esi, dword ptr [esp + 0x38]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    dword ptr [esp + 0x10], edx
        mov    word ptr [eax + 4], dx
        add    dword ptr [esp + 0x38], edx
        add    eax, 6
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_178A
loc_1_17C5:
        mov    dword ptr [ebp + 0x12], 0x1929 ; -> loc_1_1929
        mov    dword ptr [ebp + 0xa], ebx
        inc    word ptr [ebx + 0x8a]
        lea    eax, [ebx + 0x74]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [ebx + 0x78]
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [ebx + 0x78], ebp
        and    dword ptr [ebx + 0x50], 0xfffffffd
        cmp    dword ptr [ebx + 0x2c], 2
        jne    loc_1_18A2
        mov    ax, word ptr [ebx + 0xa4]
        cmp    dword ptr [esp + 0x44], 0
        mov    word ptr [ebp + 0x18], ax
        mov    eax, dword ptr [ebp + 0x32]
        jne    loc_1_182D
        lea    edi, [eax + 6]
        lea    esi, [ebx + 0xa9]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    cl, byte ptr [ebx + 0xa6]
        lea    esi, [ebx + 0xb6]
        mov    byte ptr [eax + 5], cl
        lea    edi, [ebp + 0x2a]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        jmp    loc_1_189A
loc_1_182D:
        lea    edx, [eax + 6]
        mov    ecx, dword ptr [esp + 0x44]
        add    ecx, 2
        mov    dword ptr [esp + 0x20], ecx
        mov    edi, edx
        mov    esi, ecx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        test   byte ptr [ebx + 0xce], 4
        je     loc_1_1854
        mov    ecx, dword ptr [esp + 0x44]
        mov    cl, byte ptr [ecx + 0xe]
        jmp    loc_1_185A
loc_1_1854:
        mov    cl, byte ptr [ebx + 0xa6]
loc_1_185A:
        mov    byte ptr [eax + 5], cl
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_1871
        lea    edi, [ebp + 0x2a]
        mov    eax, 0xffffffff
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        jmp    loc_1_189A
loc_1_1871:
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [0x269e]       ; -> loc_1_269E
        cmp    dword ptr [ecx], eax
        jne    loc_1_188D
        mov    esi, dword ptr [esp + 0x44]
        add    esi, 6
        lea    edi, [ebp + 0x2a]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        jmp    loc_1_189A
loc_1_188D:
        lea    eax, [ebp + 0x2a]
        push   eax
        push   edx
        call   sub_1_01A2
        add    esp, 8
loc_1_189A:
        push   ebp
        call   sub_1_0073
        jmp    loc_1_18D3
loc_1_18A2:
        mov    eax, dword ptr [ebp + 0x32]
        mov    cx, word ptr [ebx + 0xcc]
        mov    word ptr [eax + 0x20], cx
        mov    byte ptr [eax + 0x1e], 0
        mov    cl, byte ptr [ebx + 0xd0]
        mov    byte ptr [eax + 0x1f], cl
        or     word ptr [ebp + 8], 2
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_18CD
        mov    byte ptr [eax + 0x1e], 0x10
loc_1_18CD:
        push   ebp
        call   sub_1_005D
loc_1_18D3:
        mov    ecx, dword ptr [esp + 0x50]
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 4
        mov    dword ptr [ecx], eax
        call   sub_1_0724
        test   byte ptr [ebx + 0x41], 1
        je     loc_1_18F2
        mov    eax, 0x2733
        jmp    loc_1_1921
loc_1_18F2:
        sub    eax, eax
        jmp    loc_1_1921
loc_1_18F6:
        or     ebp, ebp
        je     loc_1_1903
        push   ebp
        call   sub_1_0500
        add    esp, 4
loc_1_1903:
        call   sub_1_0724
        test   byte ptr [ebx + 0x41], 1
        je     loc_1_1915
        mov    eax, 0x2747
        jmp    loc_1_1921
loc_1_1915:
        mov    eax, 0x2733
        jmp    loc_1_1921
loc_1_191C:
        mov    eax, 0x271e
loc_1_1921:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_1929:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    dx, word ptr [esi - 6]
        sub    esi, 0xe
        test   dl, 8
        je     loc_1_1942
        jmp    loc_1_1A18
loc_1_1942:
        mov    edi, dword ptr [esi + 0xa]
        mov    ebp, dword ptr [edi + 0x40]
        and    ebp, 0x100
        je     loc_1_19C1
        cmp    dword ptr [edi + 0x2c], 2
        je     loc_1_19C1
        cmp    byte ptr [esi + 0x17], 0
        jne    loc_1_19C1
        movzx  ebx, word ptr [esi + 0x3c]
        add    ebx, dword ptr [edi + 0x94]
        mov    dword ptr [edi + 0x94], ebx
        mov    ecx, dword ptr [edi + 0x90]
        cmp    ecx, ebx
        jbe    loc_1_19C1
        mov    eax, dword ptr [edi + 0x8c]
        add    eax, ebx
        mov    dword ptr [esi + 0x38], eax
        mov    eax, ecx
        sub    ax, bx
        mov    word ptr [esi + 0x3c], ax
        movzx  edi, ax
        mov    eax, dword ptr [0x26bc]       ; -> loc_1_26BC
        cmp    edi, eax
        jle    loc_1_199A
        mov    word ptr [esi + 0x3c], ax
loc_1_199A:
        movzx  eax, word ptr [esi + 0x3c]
        add    eax, ebx
        cmp    eax, ecx
        jb     loc_1_19AB
        mov    eax, dword ptr [esi + 0x32]
        or     byte ptr [eax + 0x1e], 0x10
loc_1_19AB:
        mov    word ptr [esi + 0x30], 2
        or     dx, 2
        mov    word ptr [esi + 8], dx
        push   esi
        call   sub_1_005D
        jmp    loc_1_1A1E
loc_1_19C1:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        dec    word ptr [edi + 0x8a]
        or     ebp, ebp
        je     loc_1_1A0C
        push   dword ptr [edi + 0x90]
        push   dword ptr [edi + 0x8c]
        call   sub_1_038F
        add    esp, 8
        mov    dword ptr [edi + 0x8c], 0
        mov    dword ptr [edi + 0x90], 0
        mov    dword ptr [edi + 0x94], 0
        and    dword ptr [edi + 0x50], 0xfffffffd
loc_1_1A0C:
        push   2
        push   edi
        push   esi
        call   sub_1_24F0
        add    esp, 0xc
loc_1_1A18:
        push   esi
        call   sub_1_0500
loc_1_1A1E:
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1A30:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_1A43
        mov    eax, 0x273d
        jmp    loc_1_1A7C
loc_1_1A43:
        call   sub_1_0724
        mov    esi, dword ptr [edi + 0x4c]
        mov    dword ptr [edi + 0x4c], 3
        movzx  eax, byte ptr [esp + 0x10]
        mov    dword ptr [edi + 0x44], eax
        mov    dword ptr [edi + 0x48], 0
        mov    dword ptr [edi + 0x54], 0x37
        push   edi
        call   sub_1_1F38
        add    esp, 4
        or     eax, eax
        je     loc_1_1A7A
        mov    dword ptr [edi + 0x4c], esi
        jmp    loc_1_1A7C
loc_1_1A7A:
        sub    eax, eax
loc_1_1A7C:
        pop    edi
        pop    esi
        ret    
loc_1_1A7F:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_0724
        mov    ebx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [ebx + 0x4c]
        cmp    eax, 5
        je     loc_1_1C89
        cmp    eax, 6
        je     loc_1_1C89
        cmp    dword ptr [esp + 0x2c], 0xe
        jae    loc_1_1AB5
        mov    eax, 0x271e
        jmp    loc_1_1C8E
loc_1_1AB5:
        mov    ecx, dword ptr [esp + 0x28]
        cmp    word ptr [ecx], 6
        je     loc_1_1AC9
        mov    eax, 0x273f
        jmp    loc_1_1C8E
loc_1_1AC9:
        cmp    dword ptr [ecx + 0xc], -1
        lea    eax, [ecx + 6]
        jne    loc_1_1AE5
        movzx  eax, word ptr [eax + 0xa]
        cmp    eax, -1
        jne    loc_1_1AE5
        mov    eax, 0x273f
        jmp    loc_1_1C8E
loc_1_1AE5:
        lea    eax, [ebx + 0xa7]
        mov    edi, eax
        mov    esi, ecx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    dword ptr [ebx + 0x18], eax
        mov    dword ptr [ebx + 0x24], 0xe
        test   byte ptr [ebx + 0xce], 4
        je     loc_1_1B17
        mov    al, byte ptr [ecx + 0xe]
        mov    byte ptr [ebx + 0xb5], al
        mov    dword ptr [ebx + 0x24], 0xf
loc_1_1B17:
        cmp    dword ptr [ebx + 0x2c], 2
        jne    loc_1_1B41
        lea    eax, [ebx + 0xb6]
        push   eax
        lea    eax, [ebx + 0xa9]
        push   eax
        call   sub_1_01A2
        add    esp, 8
        or     al, al
        je     loc_1_1B41
        mov    eax, 0x2751
        jmp    loc_1_1C8E
loc_1_1B41:
        cmp    dword ptr [ebx + 0x4c], 1
        jne    loc_1_1B8B
        lea    edi, [esp + 0x10]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    word ptr [esp + 0x10], 6
        lea    eax, [esp + 0x10]
        push   0xe
        push   eax
        push   ebx
        call   sub_1_232C
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1C8E
        cmp    dword ptr [ebx + 0x2c], 2
        jne    loc_1_1BA3
        mov    dword ptr [ebx + 0x54], 0
        push   0
        push   2
        push   ebx
        call   sub_1_0420
        add    esp, 0xc
loc_1_1B8B:
        cmp    dword ptr [ebx + 0x2c], 2
        jne    loc_1_1BA3
        mov    dword ptr [ebx + 0x4c], 6
        and    dword ptr [ebx + 0x50], 0xffffffef
        sub    eax, eax
        jmp    loc_1_1C8E
loc_1_1BA3:
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_1BBD
        mov    eax, 0x2747
        jmp    loc_1_1C8E
loc_1_1BBD:
        push   1
        call   sub_1_0563
        add    esp, 4
        mov    dword ptr [ebp + 0x32], eax
        mov    ecx, eax
        or     ecx, ecx
        jne    loc_1_1BE3
        push   ebp
        call   sub_1_0500
        add    esp, 4
        mov    eax, 0x2747
        jmp    loc_1_1C8E
loc_1_1BE3:
        and    dword ptr [ebx + 0x54], 0xffffffef
        mov    word ptr [ebp + 0x30], 1
        mov    word ptr [ebp + 0x36], 0x2a
        mov    ax, word ptr [ebx + 0xa4]
        lea    esi, [ebx + 0xa9]
        mov    word ptr [ebp + 0x18], ax
        mov    dword ptr [ebp + 0xa], ebx
        mov    dword ptr [ebp + 0x12], 0x1ef0 ; -> loc_1_1EF0
        lea    edi, [ecx + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    byte ptr [ecx + 5], 5
        lea    eax, [ebx + 0x74]
        mov    esi, dword ptr [ebx + 0x4c]
        mov    dword ptr [ebx + 0x4c], 5
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [ebx + 0x78]
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [ebx + 0x78], ebp
        or     word ptr [ebp + 8], 2
        and    dword ptr [ebx + 0x50], 0xffffffef
        push   ebp
        call   sub_1_00C7
        add    esp, 4
        or     al, al
        je     loc_1_1C67
        mov    eax, dword ptr [ebp + 4]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   ebp
        call   sub_1_0500
        add    esp, 4
        mov    dword ptr [ebx + 0x4c], esi
        mov    eax, 0x2742
        jmp    loc_1_1C8E
loc_1_1C67:
        push   ebx
        call   sub_1_1027
        add    esp, 4
        or     eax, eax
        je     loc_1_1C82
        call   sub_1_0724
        push   ebx
        call   sub_1_1027
        add    esp, 4
loc_1_1C82:
        mov    eax, 0x2733
        jmp    loc_1_1C8E
loc_1_1C89:
        mov    eax, 0x2748
loc_1_1C8E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_1C96:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x14]
        and    dword ptr [esi + 0x50], 0xfffffff7
        or     edi, edi
        jne    loc_1_1CB3
        mov    eax, 0x2726
        jmp    loc_1_1D97
loc_1_1CB3:
        cmp    dword ptr [esi + 0x48], 0
        jne    loc_1_1CBE
        jmp    loc_1_1D92
loc_1_1CBE:
        call   sub_1_0724
        mov    dword ptr [edi], 0
        sub    ebx, ebx
        mov    eax, dword ptr [esi + 0xbc]
        sub    eax, 0xbc
        cmp    esi, eax
        je     loc_1_1D0C
        mov    ecx, 4
loc_1_1CDF:
        cmp    dword ptr [eax + 0x4c], ecx
        jne    loc_1_1CEC
        cmp    dword ptr [eax + 0x94], esi
        je     loc_1_1CFD
loc_1_1CEC:
        mov    eax, dword ptr [eax + 0xbc]
        sub    eax, 0xbc
        cmp    esi, eax
        jne    loc_1_1CDF
        jmp    loc_1_1D0C
loc_1_1CFD:
        mov    ebx, eax
        test   byte ptr [eax + 0x3c], 1
        je     loc_1_1D0C
        mov    dword ptr [eax + 0x4c], 7
loc_1_1D0C:
        or     ebx, ebx
        je     loc_1_1D8B
        dec    dword ptr [esi + 0x48]
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ebx + 0x68], eax
        cmp    dword ptr [ebx + 0x4c], 4
        jne    loc_1_1D2E
        mov    dword ptr [ebx + 0x4c], 6
        mov    dword ptr [ebx + 0x54], 8
loc_1_1D2E:
        test   byte ptr [ebx + 0x41], 1
        jne    loc_1_1D60
        and    dword ptr [ebx + 0x50], 0xfffffffd
        push   2
        push   ebx
        push   0
        call   sub_1_24F0
        add    esp, 0xc
        cmp    word ptr [ebx + 0x84], 0
        je     loc_1_1D60
        and    dword ptr [ebx + 0x50], 0xfffffffe
        push   1
        push   ebx
        push   0
        call   sub_1_24F0
        add    esp, 0xc
loc_1_1D60:
        mov    dword ptr [edi], ebx
        cmp    dword ptr [esi + 0x48], 0
        je     loc_1_1D79
        and    dword ptr [esi + 0x50], 0xfffffff7
        push   8
        push   esi
        push   0
        call   sub_1_24F0
        add    esp, 0xc
loc_1_1D79:
        push   esi
        call   sub_1_1F38
        add    esp, 4
        call   sub_1_0724
        sub    eax, eax
        jmp    loc_1_1D97
loc_1_1D8B:
        mov    dword ptr [esi + 0x48], 0
loc_1_1D92:
        mov    eax, 0x2733
loc_1_1D97:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_1D9B:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        sub    eax, 0xe
        mov    dword ptr [esp + 0x10], eax
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [eax + 0xa]
        and    byte ptr [ebx + 0xce], 0x7f
        cmp    byte ptr [eax + 0x17], 0
        je     loc_1_1DEC
        push   8
        push   ebx
        and    dword ptr [ebx + 0x50], 0xfffffff7
        push   dword ptr [esp + 0x18]
        call   sub_1_24F0
        add    esp, 0xc
        push   dword ptr [esp + 0x10]
        call   sub_1_0500
        jmp    loc_1_1EE5
loc_1_1DEC:
        push   ebx
        call   sub_1_2069
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_1EC0
        mov    dword ptr [ebp + 0x54], 0x10
        mov    dword ptr [ebp + 0x68], 0xffffffff
        mov    dword ptr [ebp + 0x50], 0
        mov    dword ptr [ebp + 0x3c], 0
        lea    edi, [ebp + 0xa9]
        mov    dword ptr [ebp + 0x4c], 4
        mov    dword ptr [ebp + 0x94], ebx
        mov    esi, dword ptr [esp + 0x10]
        add    esi, 0x1e
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    ecx, [ebp + 0xa7]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    word ptr [ecx], 6
        mov    ax, word ptr [ebx + 0xa4]
        mov    word ptr [ebp + 0xb3], ax
        lea    esi, [ebx + 0x98]
        mov    eax, dword ptr [esp + 0x10]
        mov    ax, word ptr [eax + 0x1a]
        mov    word ptr [ebp + 0xcc], ax
        lea    eax, [ebp + 0x98]
        mov    edi, eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    dword ptr [ebp + 0x14], eax
        mov    dword ptr [ebp + 0x18], ecx
        lea    ecx, [ebp + 0xbc]
        mov    dword ptr [ebp + 0x24], 0xe
        lea    eax, [ebx + 0xbc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 0xc0]
        mov    dword ptr [ebp + 0xc0], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebx + 0xc0], ecx
        inc    dword ptr [ebx + 0x48]
        push   ebp
        call   sub_1_1027
        add    esp, 4
        or     eax, eax
        je     loc_1_1EC0
        call   sub_1_0724
        push   ebp
        call   sub_1_1027
        add    esp, 4
loc_1_1EC0:
        push   8
        push   ebx
        and    dword ptr [ebx + 0x50], 0xfffffff7
        push   dword ptr [esp + 0x18]
        call   sub_1_24F0
        add    esp, 0xc
        push   dword ptr [esp + 0x10]
        call   sub_1_0500
        add    esp, 4
        push   ebx
        call   sub_1_1F38
loc_1_1EE5:
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_1EF0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        sub    esi, 0xe
        test   byte ptr [esi + 8], 8
        je     loc_1_1F00
        jmp    loc_1_1F2D
loc_1_1F00:
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esi + 0xa]
        cmp    byte ptr [esi + 0x17], 0
        jne    loc_1_1F21
        mov    dword ptr [eax + 0x4c], 6
        mov    dword ptr [eax + 0x54], 8
loc_1_1F21:
        push   0x10
        push   eax
        push   esi
        call   sub_1_24F0
        add    esp, 0xc
loc_1_1F2D:
        push   esi
        call   sub_1_0500
        add    esp, 4
        pop    esi
        ret    
sub_1_1F38:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        test   byte ptr [esi + 0xce], 0x80
        je     loc_1_1F49
        jmp    loc_1_1FB3
loc_1_1F49:
        mov    eax, dword ptr [esi + 0x44]
        cmp    dword ptr [esi + 0x48], eax
        jne    loc_1_1F58
        mov    eax, 0x2747
        jmp    loc_1_1FB5
loc_1_1F58:
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_1F6F
        mov    eax, 0x2747
        jmp    loc_1_1FB5
loc_1_1F6F:
        or     byte ptr [esi + 0xce], 0x80
        mov    word ptr [edi + 0x36], 0x2a
        mov    ax, word ptr [esi + 0xa4]
        mov    word ptr [edi + 0x18], ax
        lea    eax, [esi + 0x74]
        mov    dword ptr [edi + 0x12], 0x1d9b ; -> loc_1_1D9B
        mov    dword ptr [edi + 0xa], esi
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0x78]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 0x78], edi
        push   esi
        call   sub_1_1027
        add    esp, 4
        push   edi
        call   sub_1_00F9
        add    esp, 4
loc_1_1FB3:
        sub    eax, eax
loc_1_1FB5:
        pop    edi
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1FC0:
        mov    edx, dword ptr [esp + 8]
        cmp    edx, 1
        je     loc_1_1FDA
        cmp    edx, 2
        je     loc_1_1FDA
        cmp    edx, 5
        je     loc_1_1FDA
        mov    eax, 0x273c
        jmp    loc_1_204D
loc_1_1FDA:
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        je     loc_1_1FF2
        cmp    ecx, 6
        je     loc_1_1FEE
        mov    eax, 0x273f
        jmp    loc_1_204D
loc_1_1FEE:
        or     ecx, ecx
        jne    loc_1_2001
loc_1_1FF2:
        mov    eax, dword ptr [esp + 0xc]
        or     eax, eax
        jne    loc_1_2005
        mov    eax, 0x273b
        jmp    loc_1_204D
loc_1_2001:
        mov    eax, dword ptr [esp + 0xc]
loc_1_2005:
        cmp    eax, 6
        jne    loc_1_200C
        jmp    loc_1_204B
loc_1_200C:
        or     eax, eax
        jne    loc_1_2017
        cmp    ecx, 6
        jne    loc_1_2017
        jmp    loc_1_204B
loc_1_2017:
        cmp    edx, 1
        je     loc_1_2038
        cmp    edx, 5
        je     loc_1_2038
        cmp    eax, 0x4e7
        ja     loc_1_2031
        cmp    eax, 0x3e8
        jb     loc_1_2031
        jmp    loc_1_204B
loc_1_2031:
        mov    eax, 0x273b
        jmp    loc_1_204D
loc_1_2038:
        cmp    eax, 0x4e8
        je     loc_1_204B
        cmp    eax, 0x4e9
        mov    eax, 0x273b
        jne    loc_1_204D
loc_1_204B:
        sub    eax, eax
loc_1_204D:
        ret    
loc_1_204E:
        push   esi
        call   sub_1_0724
        push   0
        call   sub_1_2069
        add    esp, 4
        mov    esi, eax
        call   sub_1_0724
        mov    eax, esi
        pop    esi
        ret    
sub_1_2069:
        push   esi
        push   edi
        push   0xd1
        call   sub_1_01C8
        add    esp, 4
        or     eax, eax
        jne    loc_1_2083
        sub    eax, eax
        jmp    loc_1_2144
loc_1_2083:
        mov    word ptr [eax + 0x86], 0x1e
        mov    esi, dword ptr [esp + 0xc]
        or     esi, esi
        je     loc_1_209F
        mov    edi, eax
        mov    ecx, 0x1d
        rep movsd dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_20AD
loc_1_209F:
        mov    dword ptr [eax + 0x34], 0x800
        mov    dword ptr [eax + 0x38], 0x800
loc_1_20AD:
        mov    dword ptr [eax + 0xc4], 0x2690 ; -> loc_1_2690
        lea    edx, [eax + 0xc4]
        mov    ecx, dword ptr [0x2694]       ; -> loc_1_2694
        mov    dword ptr [eax + 0xc8], ecx
        mov    ecx, dword ptr [0x2694]       ; -> loc_1_2694
        mov    dword ptr [ecx], edx
        mov    dword ptr [0x2694], edx       ; -> loc_1_2694
        mov    ecx, dword ptr [0x2698]       ; -> loc_1_2698
        inc    ecx
        mov    dword ptr [0x2698], ecx       ; -> loc_1_2698
        lea    ecx, [eax + 0x7c]
        mov    dword ptr [eax + 0x80], ecx
        mov    dword ptr [ecx], ecx
        mov    word ptr [eax + 0x84], 0
        mov    byte ptr [eax + 0x89], 0
        lea    ecx, [eax + 0xbc]
        mov    byte ptr [eax + 0x88], 0
        mov    word ptr [eax + 0x8a], 0
        mov    word ptr [eax + 0xcc], 0
        mov    dword ptr [eax + 0xc0], ecx
        mov    dword ptr [ecx], ecx
        lea    ecx, [eax + 0x74]
        mov    dword ptr [eax + 0x78], ecx
        mov    dword ptr [ecx], ecx
        mov    byte ptr [eax + 0xce], 0
        mov    byte ptr [eax + 0xd0], 0
        lea    ecx, [eax + 8]
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [ecx], ecx
loc_1_2144:
        pop    edi
        pop    esi
        ret    
sub_1_2147:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x4c], 9
        mov    eax, dword ptr [esi + 0x7c]
        lea    edi, [esi + 0x7c]
        sub    eax, esi
        cmp    eax, 0x7c
        je     loc_1_217D
loc_1_2162:
        mov    ecx, dword ptr [edi]
        push   ecx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi], eax
        call   sub_1_0500
        add    esp, 4
        mov    eax, dword ptr [edi]
        sub    eax, esi
        cmp    eax, 0x7c
        jne    loc_1_2162
loc_1_217D:
        cmp    word ptr [esi + 0x8a], 0
        je     loc_1_2190
        push   esi
        call   sub_1_0265
        add    esp, 4
loc_1_2190:
        cmp    dword ptr [esi + 0x2c], 2
        je     loc_1_220A
        cmp    word ptr [esi + 0xcc], 0
        je     loc_1_220A
        test   byte ptr [esi + 0x3c], 1
        jne    loc_1_220A
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_1_220A
        push   1
        call   sub_1_0563
        add    esp, 4
        mov    dword ptr [edi + 0x32], eax
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_2201
        mov    word ptr [edi + 0x30], 1
        push   ebx
        movzx  eax, word ptr [esi + 0xcc]
        push   eax
        call   sub_1_0143
        add    esp, 8
        or     eax, eax
        jne    loc_1_2201
        cmp    byte ptr [ebx], 3
        jne    loc_1_2201
        mov    word ptr [edi + 0x36], 0x2a
        mov    dword ptr [edi + 0xa], esi
        mov    dword ptr [edi + 0x12], 0x2219 ; -> loc_1_2219
        push   edi
        call   sub_1_011A
        jmp    loc_1_2210
loc_1_2201:
        push   edi
        call   sub_1_0500
        add    esp, 4
loc_1_220A:
        push   esi
        call   sub_1_2238
loc_1_2210:
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_2219:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax - 4]
        sub    eax, 0xe
        push   eax
        call   sub_1_0500
        add    esp, 4
        push   esi
        call   sub_1_2238
        add    esp, 4
        pop    esi
        ret    
sub_1_2238:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 0x74]
        lea    edi, [esi + 0x74]
        sub    eax, esi
        cmp    eax, 0x74
        je     loc_1_2282
loc_1_224C:
        mov    ebx, dword ptr [edi]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi], eax
        mov    ax, word ptr [ebx + 8]
        test   al, 2
        je     loc_1_2267
        or     ax, 8
        mov    word ptr [ebx + 8], ax
        jmp    loc_1_2279
loc_1_2267:
        push   ebx
        call   sub_1_00AA
        add    esp, 4
        push   ebx
        call   sub_1_0500
        add    esp, 4
loc_1_2279:
        mov    eax, dword ptr [edi]
        sub    eax, esi
        cmp    eax, 0x74
        jne    loc_1_224C
loc_1_2282:
        mov    edi, dword ptr [esi + 0xbc]
        sub    edi, 0xbc
        cmp    edi, esi
        je     loc_1_22BF
        mov    ebx, 4
loc_1_2297:
        mov    eax, edi
        mov    edi, dword ptr [edi + 0xbc]
        sub    edi, 0xbc
        cmp    dword ptr [eax + 0x4c], ebx
        jne    loc_1_22BB
        cmp    dword ptr [eax + 0x94], esi
        jne    loc_1_22BB
        push   eax
        call   sub_1_02E4
        add    esp, 4
loc_1_22BB:
        cmp    edi, esi
        jne    loc_1_2297
loc_1_22BF:
        mov    ecx, dword ptr [esi + 0xc8]
        lea    eax, [esi + 0xc4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esi + 0xbc]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0xbc
        jne    loc_1_22F5
        movzx  eax, word ptr [esi + 0xa4]
        push   eax
        call   sub_1_030C
        add    esp, 4
        jmp    loc_1_2300
loc_1_22F5:
        mov    eax, dword ptr [esi + 0xc0]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
loc_1_2300:
        push   esi
        call   sub_1_0638
        add    esp, 4
        mov    eax, dword ptr [0x2698]       ; -> loc_1_2698
        dec    eax
        mov    dword ptr [0x2698], eax       ; -> loc_1_2698
        jne    loc_1_231F
        push   esi
        call   sub_1_0638
        add    esp, 4
loc_1_231F:
        push   esi
        call   sub_1_01F8
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_232C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 0x13], 0
        mov    edx, dword ptr [0x2690]       ; -> loc_1_2690
        sub    edx, 0xc4
        mov    ebx, dword ptr [esp + 0x1c]
        or     ebx, ebx
        je     loc_1_24E2
        cmp    dword ptr [esp + 0x20], 0xe
        jb     loc_1_24E2
        cmp    word ptr [ebx], 6
        je     loc_1_236B
        mov    eax, 0x273f
        jmp    loc_1_24E7
loc_1_236B:
        mov    cx, word ptr [ebx + 0xc]
        or     cx, cx
        je     loc_1_23F3
        sub    eax, eax
        mov    esi, dword ptr [0x2698]       ; -> loc_1_2698
        cmp    esi, eax
        mov    ebp, dword ptr [esp + 0x18]
        jle    loc_1_23BB
        mov    edi, 1
loc_1_2389:
        cmp    edx, 0x25cc                   ; -> obj1:0x25CC
        je     loc_1_23BB
        cmp    ebp, edx
        je     loc_1_23A3
        cmp    dword ptr [edx + 0x4c], edi
        je     loc_1_23A3
        cmp    word ptr [edx + 0xa4], cx
        je     loc_1_23B6
loc_1_23A3:
        inc    eax
        mov    edx, dword ptr [edx + 0xc4]
        sub    edx, 0xc4
        cmp    esi, eax
        jg     loc_1_2389
        jmp    loc_1_23BB
loc_1_23B6:
        mov    byte ptr [esp + 0x13], 1
loc_1_23BB:
        cmp    byte ptr [esp + 0x13], 0
        je     loc_1_23F7
        test   byte ptr [ebp + 0x40], 0x20
        jne    loc_1_23D2
        mov    eax, 0x2740
        jmp    loc_1_24E7
loc_1_23D2:
        lea    eax, [ebp + 0xbc]
        mov    ecx, dword ptr [edx + 0xbc]
        add    edx, 0xbc
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp + 0xc0], edx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx], eax
        jmp    loc_1_23F7
loc_1_23F3:
        mov    ebp, dword ptr [esp + 0x18]
loc_1_23F7:
        call   sub_1_017F
        lea    edx, [ebx + 6]
        cmp    dword ptr [edx], 0
        jne    loc_1_240B
        cmp    word ptr [ebx + 0xa], 0
        je     loc_1_2448
loc_1_240B:
        mov    eax, 0x26a2                   ; -> loc_1_26A2
        mov    ecx, dword ptr [eax]
        cmp    ecx, dword ptr [edx]
        jne    loc_1_241E
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
loc_1_241E:
        je     loc_1_2448
        cmp    byte ptr [esp + 0x13], 0
        je     loc_1_243E
        mov    ecx, dword ptr [ebp + 0xc0]
        mov    eax, dword ptr [ebp + 0xbc]
        add    ebp, 0xbc
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
loc_1_243E:
        mov    eax, 0x273f
        jmp    loc_1_24E7
loc_1_2448:
        lea    ecx, [ebp + 0x98]
        mov    esi, 0x269c                   ; -> loc_1_269C
        mov    edi, ecx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        test   byte ptr [ebp + 0xce], 4
        mov    ax, word ptr [ebx + 0xc]
        mov    word ptr [ebp + 0xa4], ax
        mov    dword ptr [ebp + 0x14], ecx
        mov    dword ptr [ebp + 0x20], 0xe
        je     loc_1_248A
        mov    al, byte ptr [ebx + 0xe]
        mov    byte ptr [ebp + 0xa6], al
        mov    dword ptr [ebp + 0x20], 0xf
        jmp    loc_1_249D
loc_1_248A:
        mov    eax, dword ptr [ebp + 0x2c]
        sub    eax, 2
        cmp    eax, 1
        sbb    al, al
        add    al, 5
        mov    byte ptr [ebp + 0xa6], al
loc_1_249D:
        cmp    byte ptr [esp + 0x13], 0
        jne    loc_1_24C1
        push   ebp
        call   0
        add    esp, 4
        or     al, al
        je     loc_1_24C1
        mov    word ptr [ebp + 0xa4], 0
        mov    eax, 0x2747
        jmp    loc_1_24E7
loc_1_24C1:
        mov    dword ptr [ebp + 0x4c], 2
        cmp    dword ptr [ebp + 0x2c], 2
        jne    loc_1_24D9
        push   ebp
        call   sub_1_1027
        add    esp, 4
        jmp    loc_1_24E7
loc_1_24D9:
        call   sub_1_0724
        sub    eax, eax
        jmp    loc_1_24E7
loc_1_24E2:
        mov    eax, 0x271e
loc_1_24E7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  000h
sub_1_24F0:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        cmp    ecx, eax
        je     loc_1_2510
        cmp    byte ptr [ecx + 0x17], al
        je     loc_1_2510
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        push   ecx
        call   sub_1_2524
        add    esp, 0xc
loc_1_2510:
        push   eax
        movzx  eax, byte ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x10]
        call   sub_1_0420
        add    esp, 0xc
        ret    
sub_1_2524:
        mov    eax, dword ptr [esp + 4]
        movzx  eax, byte ptr [eax + 0x17]
        or     eax, eax
        je     loc_1_253D
        cmp    eax, 0xed
        je     loc_1_2541
loc_1_2537:
        mov    eax, 0x2742
loc_1_253C:
        ret    
loc_1_253D:
        sub    eax, eax
        jmp    loc_1_253C
loc_1_2541:
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [ecx + 0x2c], 2
        je     loc_1_2537
        movzx  eax, byte ptr [esp + 0xc]
        cmp    eax, 1
        jl     loc_1_2537
        cmp    eax, 2
        jle    loc_1_2561
        cmp    eax, 0x10
        je     loc_1_2571
        jmp    loc_1_2537
loc_1_2561:
        mov    word ptr [ecx + 0xcc], 0
        mov    eax, 0x2746
        jmp    loc_1_253C
loc_1_2571:
        mov    eax, 0x2741
        jmp    loc_1_253C
sub_1_2578:
        push   esi
        mov    dword ptr [0x2694], 0x2690    ; -> loc_1_2694
        mov    dword ptr [0x2690], 0x2690    ; -> loc_1_2690
        mov    dword ptr [0x2698], 0         ; -> loc_1_2698
        mov    dword ptr [0x26b8], 0         ; -> loc_1_26B8
        mov    dword ptr [0x26d4], 0         ; -> loc_1_26D4
        mov    dword ptr [0x26d8], 0         ; -> loc_1_26D8
        mov    dword ptr [0x26d0], 0         ; -> loc_1_26D0
        mov    dword ptr [0x26c8], 0         ; -> loc_1_26C8
        mov    dword ptr [0x26c4], 0         ; -> loc_1_26C4
        mov    word ptr [0x269c], 6          ; -> loc_1_269C
        mov    word ptr [0x26a8], 0          ; -> loc_1_26A8
        mov    esi, dword ptr [0x26cc]       ; -> loc_1_26CC
        cmp    esi, 0x200
        jge    loc_1_25F8
        mov    esi, 0x200
loc_1_25F8:
        mov    dword ptr [0x26cc], esi       ; -> loc_1_26CC
        mov    eax, esi
        cdq    
        and    edx, 0xff
        add    eax, edx
        sar    eax, 8
        inc    eax
        mov    dword ptr [0x26ac], eax       ; -> loc_1_26AC
        mov    ecx, eax
        imul   ecx, ecx, 6
        add    ecx, 0x32
        mov    dword ptr [0x26c0], ecx       ; -> loc_1_26C0
        shl    eax, 2
        mov    dword ptr [0x26b4], eax       ; -> loc_1_26B4
        lea    eax, [esi - 0x2a]
        mov    dword ptr [0x26bc], eax       ; -> loc_1_26BC
        sub    esi, 0x1e
        mov    dword ptr [0x26b0], esi       ; -> loc_1_26B0
        sub    eax, eax
        pop    esi
        ret    
loc_1_263D:
        sub    eax, eax
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_2654:
        rcr    byte ptr [edi], 0
        add    byte ptr [esi + 0x20], cl     ; -> loc_1_204E
        add    byte ptr [eax], al
        inc    edi
        and    dword ptr [eax], eax
        add    byte ptr [ebx], ch            ; -> sub_1_232C
        add    byte ptr [eax], al
        xchg   esi, eax
        sbb    al, 0
        add    byte ptr [edi + 0x1a], bh     ; -> loc_1_1A7F
        add    byte ptr [eax], al
        xor    byte ptr [edx], bl
        add    byte ptr [eax], al
        xor    byte ptr [ebx], cl
        add    byte ptr [eax], al
        push   eax
        adc    eax, 0x263d0000               ; -> loc_1_263D
        add    byte ptr [eax], al
        push   eax
        pop    es
        add    byte ptr [eax], al
        stc    
        or     byte ptr [eax], al
        add    byte ptr [edx], al            ; -> loc_1_0A02
        or     al, byte ptr [eax]
        add    byte ptr [edx + 0xa], ah      ; -> loc_1_0A62
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2690:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2694:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2698:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_269C:
        add    byte ptr [eax], al
loc_1_269E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26A2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x98  R-X
loc_2_0000:
        mov    eax, 0x47e                    ; -> obj1:0x047E
        movzx  edx, byte ptr [eax]
        or     edx, edx
        jne    loc_2_0088
        inc    byte ptr [eax]
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        cmp    ax, 0x400
        jb     loc_2_0088
        VxDCall  SHELL.SHELL_Get_Version         ; cd2000001700
        jb     loc_2_0088
        VxDCall  Dev_0487.Service_0000h          ; cd2000008704
        jb     loc_2_0088
        VxDCall  Dev_003E.Service_0000h          ; cd2000003e00
        jb     loc_2_0088
        mov    eax, 0x3f
        mov    ebx, 1
        mov    ecx, 0x2654                   ; -> loc_1_2654
        VxDCall  Dev_003E.Service_0001h          ; cd2001003e00
        jb     loc_2_0088
        or     eax, eax
        je     loc_2_0088
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_2_006B
        inc    byte ptr [0x47c]              ; -> obj1:0x047C
        push   ebx
        xor    ebx, ebx
        VxDCall  PAGESWAP.PageSwap_Get_Version   ; cd2000000700
        mov    byte ptr [0x47d], bl          ; -> obj1:0x047D
        pop    ebx
loc_2_006B:
        mov    ebx, 0x1a
        VxDCall  Dev_0487.Service_0001h          ; cd2001008704
        jb     loc_2_0088
        mov    dword ptr [0x26cc], eax       ; -> loc_1_26CC
        call   sub_1_2578
        or     eax, eax
        jne    loc_2_0088
        clc    
        ret    
loc_2_0088:
        stc    
        ret    
loc_2_008A:
        call   sub_1_0165
        or     al, al
        jne    loc_2_0095
        clc    
        ret    
loc_2_0095:
        stc    
        ret    
        db  022h
VxD__LTEXT ENDS
