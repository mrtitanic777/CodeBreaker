; ELPC3 -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 7866 instructions recovered by recursive descent

Declare_Virtual_Device ELPC3, 3, 10, ELPC3_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x599c  R-X
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
        db  0FFh, 0FFh, 0FFh, 0FFh
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'ELPC3   '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  ELPC3_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 0CCh, 0CCh, 0CCh, 045h, 04Ch, 050h, 043h
        db  033h, 000h, 000h, 000h
loc_1_01F0:
        inc    ebp
        dec    esp
        push   eax
        inc    ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        dec    ebp
        popal  
        js     0x265
        insd   dword ptr es:[edi], dx
        jne    loc_1_026C
        dec    ebp
        jne    loc_1_026E
        je     0x26d
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_0255
        imul   esi, dword ptr [ebx + 0x74], 0
        dec    ebp
        popal  
        js     0x27d
        insd   dword ptr es:[edi], dx
        jne    loc_1_0284
        dec    ebp
        jne    0x286
        je     0x285
        arpl   word ptr [ecx + 0x73], sp
        je     0x26d
        imul   esi, dword ptr [ebx + 0x74], 0
        inc    esp
        push   esp
        inc    ecx
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    loc_1_02A1
        arpl   word ptr [ebp + 0x43], sp
        outsd  dx, dword ptr [esi]
        jne    loc_1_02A3
        je     0x237
        db  000h
loc_1_0238:
        inc    esp
        push   esp
        inc    ecx
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    loc_1_02B1
        arpl   word ptr [ebp + 0x43], sp
        outsd  dx, dword ptr [esi]
        jne    loc_1_02B3
        je     loc_1_0247
loc_1_0247:
        add    byte ptr [esi + 0x45], cl
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
loc_1_0255:
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        inc    edx
        jne    0x2d6
        push   esp
        jns    0x2d6
        add    byte ptr gs:[edx + 0x75], al
        jae    0x2c0
loc_1_026C:
        jns    0x2de
loc_1_026E:
        add    byte ptr gs:[ecx + 0x64], al
        popal  
        jo     loc_1_02E9
        jb     0x2cc
        jns    0x2ea
        add    byte ptr gs:[ecx + 0x64], al
        popal  
        jo     loc_1_02F5
        jb     0x2d8
loc_1_0284:
        jns    0x2f6
        db  065h, 000h
loc_1_0288:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
loc_1_02A1:
        add    dword ptr [ecx], eax
loc_1_02A3:
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
loc_1_02B1:
        add    dword ptr [ecx], eax
loc_1_02B3:
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0xd000201
loc_1_02E9:
        add    al, byte ptr [edx]
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
loc_1_02F5:
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, 0x6010202
        add    al, byte ptr [edx]
        add    dword ptr [edi], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx + eax], eax
        add    al, byte ptr [ecx]
loc_1_0324:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_0380:
        inc    ebp
        dec    esp
        dec    esi
        dec    ebx
        xor    esp, dword ptr [eax]
        inc    ebp
        je     0x3f1
        jb     0x3fa
        je     loc_1_03AF
        inc    ecx
        popal  
        jo     loc_1_0408
        jb     loc_1_0397
loc_1_0397:
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 049h, 04Fh, 041h, 044h, 044h, 052h, 045h, 053h, 053h, 000h, 000h, 000h
loc_1_03A8:
        dec    ecx
        dec    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
loc_1_03AF:
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4c], dl
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03BC:
        push   ebx
        dec    esp
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        inc    esp
        push   eax
        dec    edi
        push   edx
        push   esp
        add    byte ptr [eax], al
loc_1_03CC:
        dec    ecx
        inc    esp
        push   eax
        dec    edi
        push   edx
        push   esp
        add    byte ptr [eax], al
        dec    ecx
        push   edx
        push   ecx
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_03E0:
        mov    al, byte ptr [0]              ; -> obj1:0x01E4
        cmp    al, 0
        jne    loc_1_0405
        inc    byte ptr [0]                  ; -> obj1:0x01E4
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0407
        push   0
        push   0
        call   sub_1_0490
        cmp    eax, 0
        jne    loc_1_0407
loc_1_0405:
        clc    
        ret    
loc_1_0407:
        stc    
loc_1_0408:
        ret    
ELPC3_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_0418
        jmp    dword ptr [eax*4]             ; -> obj1:0x041A
loc_1_0418:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_03E0
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_0418
        dd  loc_1_03E0
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_0490:
        push   ebp
        mov    eax, 6
        mov    ebp, esp
        sub    esp, 0xc
        mov    word ptr [ebp - 0xc], ax
        push   esi
        mov    word ptr [ebp - 0xa], ax
        push   edi
        mov    edi, 0                        ; -> obj1:0x0000
        xor    eax, eax
        mov    dword ptr [ebp - 8], 0        ; -> loc_1_01F0
        mov    ecx, 0x55
        rep stosd dword ptr es:[edi], eax
        mov    word ptr [0], 8               ; -> obj1:0x0160
        mov    word ptr [2], 4               ; -> obj1:0x0162
        xor    esi, esi
        mov    dword ptr [4], esi            ; -> obj1:0x0164
        mov    dword ptr [8], esi            ; -> obj1:0x0168
        mov    dword ptr [0xc], esi          ; -> obj1:0x016C
        mov    dword ptr [0x10], esi         ; -> obj1:0x0170
        mov    dword ptr [0x14], esi         ; -> obj1:0x0174
        mov    dword ptr [0x18], esi         ; -> obj1:0x0178
        push   esi
        mov    dword ptr [0x1c], esi         ; -> obj1:0x017C
        push   esi
        mov    dword ptr [0x20], esi         ; -> obj1:0x0180
        mov    word ptr [0x24], 0x62f7       ; -> obj1:0x0184
        push   0                             ; -> obj1:0x0160
        push   4                             ; -> obj1:0x0004
        call   sub_1_547A
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [0x3e], edx         ; -> obj1:0x003E
        mov    dword ptr [0x42], ecx         ; -> obj1:0x0042
        mov    dword ptr [0x46], esi         ; -> obj1:0x0046
        mov    byte ptr [8], 3               ; -> obj1:0x0008
        mov    byte ptr [9], 0xa             ; -> obj1:0x0009
        mov    dword ptr [0xa], esi          ; -> obj1:0x000A
        mov    dword ptr [0xe], 0            ; -> obj1:0x000E
        mov    dword ptr [0x12], 0           ; -> obj1:0x0012
        mov    dword ptr [0x16], 0           ; -> obj1:0x0016
        mov    dword ptr [0x1a], 0           ; -> obj1:0x001A
        mov    dword ptr [0x1e], 0           ; -> obj1:0x001E
        mov    dword ptr [0x22], 0           ; -> obj1:0x0022
        mov    dword ptr [0x26], 0           ; -> obj1:0x0026
        mov    dword ptr [0x2a], 0           ; -> obj1:0x002A
        mov    dword ptr [0x2e], 0           ; -> obj1:0x002E
        mov    dword ptr [0x32], 0           ; -> obj1:0x0032
        mov    eax, dword ptr [ebp + 8]
        push   0x36
        push   8                             ; -> obj1:0x0008
        mov    dword ptr [0x36], edx         ; -> obj1:0x0036
        mov    edx, dword ptr [ebp + 0xc]
        push   0                             ; -> obj1:0x0000
        mov    dword ptr [0x3a], ecx         ; -> obj1:0x003A
        mov    dword ptr [0x52], eax         ; -> obj1:0x0052
        mov    dword ptr [0x56], edx         ; -> obj1:0x0056
        mov    ecx, dword ptr [4]            ; -> obj1:0x0004
        push   ecx
        push   0                             ; -> obj1:0x0000
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_5486
        cmp    dword ptr [ebp - 4], esi
        je     loc_1_0615
        cmp    dword ptr [0x46], esi         ; -> obj1:0x0046
        jbe    loc_1_05FD
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_05FD:
        push   0
        mov    eax, dword ptr [4]            ; -> obj1:0x0004
        push   eax
        call   sub_1_5480
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0615:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_1_0620:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x0398
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    ecx, [ebp - 4]
        push   esi
        push   edi
        push   eax
        push   0
        push   0x462
        push   ecx
        call   sub_1_5566
        test   eax, eax
        je     loc_1_0651
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0651:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0x118
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        add    esi, 0xb0
        push   eax
        call   sub_1_0BA0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2090
        mov    ebx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   eax
        call   sub_1_0770
        test   eax, eax
        je     loc_1_06AD
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x462
        push   eax
        call   sub_1_556C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_06AD:
        cmp    dword ptr [esi + 0x78], 3
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jne    loc_1_06F4
        call   sub_1_2DC0
        test   eax, eax
        je     loc_1_0731
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000004
        mov    ecx, dword ptr [eax + 0x3a]
        push   ecx
        call   sub_1_5638
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 4]
        push   0
        push   0x462
        push   ecx
        call   sub_1_556C
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_06F4:
        call   sub_1_2E30
        test   eax, eax
        je     loc_1_0731
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000004
        mov    ecx, dword ptr [eax + 0x3a]
        push   ecx
        call   sub_1_5638
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 4]
        push   0
        push   0x462
        push   ecx
        call   sub_1_556C
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0731:
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   ebx
        push   eax
        call   sub_1_08F0
        mov    esi, eax
        test   esi, esi
        je     loc_1_075D
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x462
        push   eax
        call   sub_1_556C
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_075D:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0770:
        push   ebp
        mov    eax, 0xf
        mov    ebp, esp
        sub    esp, 0x2c
        mov    word ptr [ebp - 0x2c], ax
        push   ebx
        mov    word ptr [ebp - 0x2a], ax
        push   esi
        mov    dword ptr [ebp - 0x28], 0     ; -> loc_1_0238
        mov    dword ptr [ebp - 0x18], 0     ; -> obj1:0x0268
        mov    dword ptr [ebp - 0x20], 0     ; -> obj1:0x027C
        mov    eax, 8
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ebp - 0x1c], ax
        push   ecx
        mov    word ptr [ebp - 0x1a], ax
        lea    edx, [ebp - 8]
        mov    eax, 0xc
        push   edx
        mov    word ptr [ebp - 0x24], ax
        mov    word ptr [ebp - 0x22], ax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_55E4
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_07D8
        mov    eax, 0xc0000001
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_07D8:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        mov    dword ptr [esi + 8], eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_54EC
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0811
        lea    edx, [esi + 0x3e]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    eax, ecx
        mov    ebx, dword ptr [ecx]
        mov    dword ptr [edx], ebx
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], cl
loc_1_0811:
        push   1
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_55EA
        cmp    dword ptr [ebp - 4], 0
        mov    eax, 8
        jne    loc_1_0839
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
loc_1_0839:
        cmp    eax, 0xff
        ja     loc_1_084D
        and    ax, 0xff
        mov    word ptr [esi + 0x460], ax
        jmp    loc_1_0856
loc_1_084D:
        mov    word ptr [esi + 0x460], 8
loc_1_0856:
        push   0
        lea    eax, [ebp - 0x24]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        mov    bl, 0
        call   sub_1_55EA
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_087B
        mov    eax, dword ptr [ebp - 0xc]
        mov    bl, byte ptr [eax + 4]
loc_1_087B:
        push   1
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_55EA
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_08BF
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 4]
        cmp    al, 1
        jne    loc_1_08A6
        xor    ecx, ecx
        jmp    loc_1_08C4
loc_1_08A6:
        cmp    al, 2
        jne    loc_1_08B1
        mov    ecx, 1
        jmp    loc_1_08C4
loc_1_08B1:
        sub    al, 3
        cmp    al, 1
        sbb    ecx, ecx
        and    ecx, 0xfffffffe
        add    ecx, 4
        jmp    loc_1_08C4
loc_1_08BF:
        mov    ecx, 4
loc_1_08C4:
        cmp    bl, 1
        jne    loc_1_08CE
        mov    ecx, 3
loc_1_08CE:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   ecx
        push   esi
        call   sub_1_2C16
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_55F0
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_1_08F0:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x0398
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    ecx, [ebp - 4]
        push   esi
        push   edi
        push   eax
        push   0
        push   0x32
        push   ecx
        call   sub_1_5566
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_1_0921
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0921:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x128]
        test   eax, eax
        je     loc_1_095E
        cmp    eax, 1
        jne    loc_1_094D
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xa], 2
        jmp    loc_1_0968
loc_1_094D:
        cmp    eax, 2
        jne    loc_1_095E
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xa], 3
        jmp    loc_1_0968
loc_1_095E:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xa], 1
loc_1_0968:
        mov    eax, dword ptr [ebp - 4]
        xor    edx, edx
        lea    ecx, [esi + 0x158]
        mov    edi, 0x10
        mov    dword ptr [eax + 0x16], 1
loc_1_097F:
        mov    eax, dword ptr [esi + 0x154]
        mov    ebx, dword ptr [ebp - 4]
        add    edx, 0xc
        cmp    edx, 0xc
        mov    dword ptr [ebx + edx + 0xe], eax
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + edx + 0x12], edi
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + edx + 0x16], ecx
        jb     loc_1_097F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   ecx
        mov    eax, dword ptr [0]            ; -> obj1:0x0000
        push   edx
        lea    ecx, [esi + 0x3a]
        push   esi
        push   eax
        push   ecx
        call   sub_1_5492
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_1_0A17
        push   esi
        call   sub_1_35B0
        test   eax, eax
        je     loc_1_09EE
        push   0
        mov    eax, dword ptr [esi + 0x3a]
        push   0xaa000002
        push   eax
        call   sub_1_5638
        mov    dword ptr [ebp - 8], 0xc000009a
        add    esp, 0xc
        jmp    loc_1_0B46
loc_1_09EE:
        push   esi
        call   sub_1_21F0
        test   eax, eax
        je     loc_1_0A74
        cmp    eax, 2
        jne    loc_1_0A30
        push   0
        mov    eax, dword ptr [esi + 0x3a]
        push   0xaa000007
        push   eax
        call   sub_1_5638
        mov    dword ptr [ebp - 8], 0xc0010004
        add    esp, 0xc
loc_1_0A17:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x10
        push   eax
        call   sub_1_556C
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0A30:
        cmp    eax, 3
        jne    loc_1_0A58
        mov    eax, dword ptr [esi + 0x154]
        mov    ecx, dword ptr [esi + 0x3a]
        push   eax
        push   1
        push   0xaa000001
        push   ecx
        call   sub_1_5638
        mov    dword ptr [ebp - 8], 0xc001001e
        add    esp, 0x10
        jmp    loc_1_0A17
loc_1_0A58:
        push   0
        mov    eax, dword ptr [esi + 0x3a]
        push   0xaa000006
        push   eax
        call   sub_1_5638
        mov    dword ptr [ebp - 8], 0xc0000001
        add    esp, 0xc
        jmp    loc_1_0A17
loc_1_0A74:
        lea    ebx, [esi + 0x22]
        xor    eax, eax
        mov    edi, ebx
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esi + 0x12c]
        dec    eax
        mov    ecx, dword ptr [esi + 0x130]
        cmp    eax, 1
        mov    eax, 2
        adc    eax, -1
        push   eax
        mov    al, byte ptr [esi + 0x124]
        push   eax
        push   ecx
        push   ecx
        mov    eax, dword ptr [esi + 0x3a]
        push   0                             ; -> loc_1_5310
        lea    edx, [ebp - 8]
        push   esi
        push   0                             ; -> loc_1_52F0
        push   eax
        push   ebx
        push   edx
        call   sub_1_554E
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_0ADF
        mov    eax, dword ptr [esi + 0x130]
        mov    ecx, dword ptr [esi + 0x3a]
        push   eax
        push   1
        push   0xaa000005
        push   ecx
        call   sub_1_5638
        add    esp, 0x10
        jmp    loc_1_0B46
loc_1_0ADF:
        push   esi
        call   sub_1_2AD0
        test   eax, eax
        je     loc_1_0B13
        mov    eax, dword ptr [esi + 0x130]
        mov    ecx, dword ptr [esi + 0x154]
        push   eax
        mov    edx, dword ptr [esi + 0x3a]
        push   ecx
        push   2
        push   0xaa000001
        push   edx
        call   sub_1_5638
        mov    dword ptr [ebp - 8], 0xc001001e
        add    esp, 0x14
        jmp    loc_1_0B40
loc_1_0B13:
        lea    eax, [esi + 0x5e]
        lea    ecx, [esi + 0xd]
        push   eax
        lea    edx, [esi + 0x3e]
        push   ecx
        push   edx
        push   0                             ; -> loc_1_1DB0
        push   0                             ; -> loc_1_1B40
        push   0                             ; -> loc_1_1B10
        push   0x10
        call   sub_2_0000
loc_1_0B35:
        test   al, al
        jne    loc_1_0B5A
        mov    dword ptr [ebp - 8], 0xc0000001
loc_1_0B40:
        push   ebx
        call   sub_1_5554
loc_1_0B46:
        push   esi
        call   sub_1_3650
        mov    eax, dword ptr [esi + 0x3a]
        push   eax
        call   sub_1_5498
        jmp    loc_1_0A17
loc_1_0B5A:
        xor    ecx, ecx
        mov    dword ptr [esi + 0x56], ecx
        mov    dword ptr [esi + 0x52], ecx
        mov    dword ptr [esi + 0x5a], ecx
        mov    eax, dword ptr [0x4a]         ; -> obj1:0x004A
        mov    dword ptr [esi + 0x4e], eax
        push   ecx
        push   0x32
        mov    dword ptr [0x4a], esi         ; -> obj1:0x004A
        mov    edx, dword ptr [esi + 0x4a]
        mov    dword ptr [esi + 0x62], ecx
        mov    dword ptr [esi + 0x66], 0     ; -> loc_1_1AF0
        mov    dword ptr [esi + 0x6a], esi
        inc    dword ptr [edx + 0x46]
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_556C
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
sub_1_0BA0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x19], ecx
        mov    dword ptr [eax + 0x1d], ecx
        mov    dword ptr [eax + 0x11], ecx
        mov    dword ptr [eax + 0x15], ecx
        mov    dword ptr [eax + 0x72], ecx
        mov    dword ptr [eax + 0x76], ecx
        mov    dword ptr [eax + 0x7a], ecx
        mov    dword ptr [eax + 0x7e], ecx
        pop    ebp
        mov    dword ptr [eax], 0x4a
        mov    dword ptr [eax + 0x4a], 0     ; -> obj1:0x0000
        mov    dword ptr [eax + 4], 0x64
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 050h, 080h
        db  048h, 021h, 040h, 0E8h, 02Fh, 046h, 000h, 000h, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C00:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x0000
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_548C
        push   0
        mov    ecx, dword ptr [4]            ; -> obj1:0x0004
        push   ecx
        call   sub_1_5480
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [esi + 0x62]
        push   ecx
        call   sub_1_5548
        cmp    esi, dword ptr [0x4a]         ; -> obj1:0x004A
        jne    loc_1_0C59
        mov    eax, dword ptr [esi + 0x4e]
        mov    dword ptr [0x4a], eax         ; -> obj1:0x004A
        jmp    loc_1_0C75
loc_1_0C59:
        mov    ecx, dword ptr [0x4a]         ; -> obj1:0x004A
        cmp    dword ptr [ecx + 0x4e], esi
        je     loc_1_0C6C
loc_1_0C64:
        mov    ecx, dword ptr [ecx + 0x4e]
        cmp    dword ptr [ecx + 0x4e], esi
        jne    loc_1_0C64
loc_1_0C6C:
        mov    eax, dword ptr [ecx + 0x4e]
        mov    edx, dword ptr [eax + 0x4e]
        mov    dword ptr [ecx + 0x4e], edx
loc_1_0C75:
        lea    eax, [esi + 0x22]
        push   eax
        call   sub_1_5554
        mov    eax, dword ptr [esi + 0x5e]
        push   eax
        call   sub_2_0120
loc_1_0C87:
        push   esi
        call   sub_1_3650
        mov    eax, dword ptr [esi + 0x3a]
        push   eax
        call   sub_1_5644
        mov    eax, dword ptr [esi + 0x3a]
        push   eax
        call   sub_1_5498
        push   0
        push   0x462
        push   esi
        call   sub_1_556C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0CC0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi]
        inc    dword ptr [esi + 0x1c]
        cmp    byte ptr [esi + 0x20], 0
        je     loc_1_0CDA
        mov    edi, 0xc0010002
        jmp    loc_1_0D16
loc_1_0CDA:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x10]
        test   eax, eax
        jne    loc_1_0CF0
        push   ecx
        push   esi
        push   edx
        call   sub_1_1280
        mov    edi, eax
        jmp    loc_1_0D16
loc_1_0CF0:
        cmp    eax, 1
        jne    loc_1_0D11
        cmp    byte ptr [edx + 0x83], 0
        je     loc_1_0D05
        mov    edi, 0xc001000d
        jmp    loc_1_0D16
loc_1_0D05:
        push   ecx
        push   esi
        push   edx
        call   sub_1_12D0
        mov    edi, eax
        jmp    loc_1_0D16
loc_1_0D11:
        mov    edi, 0x10001
loc_1_0D16:
        push   esi
        call   sub_1_1DD0
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    ecx, dword ptr [esi + 0x134]
        mov    word ptr [ebp - 6], bx
        mov    dword ptr [ebp - 0x20], ecx
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], ebx
        cmp    edx, 0x1010101
        mov    eax, edx
        mov    byte ptr [ebp - 4], bl
        mov    dword ptr [ebp - 0x10], ebx
        mov    dword ptr [ebp - 0x1c], ebx
        ja     loc_1_0D7B
        je     loc_1_10C1
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_0D9C
        jmp    dword ptr [eax*4]             ; -> obj1:0x1204
loc_1_0D7B:
        cmp    eax, 0x1010102
        je     loc_1_10DD
        cmp    eax, 0x1010103
        je     loc_1_10F9
        cmp    eax, 0x1010104
        je     loc_1_11A7
loc_1_0D9C:
        mov    eax, edx
        and    eax, 0xff000000
        cmp    eax, 0xff000000
        je     loc_1_0DB6
        mov    dword ptr [ebp - 0x10], 0xc00000bb
        jmp    loc_1_11B8
loc_1_0DB6:
        add    edx, 0xfefeff
        cmp    edx, 8
        ja     loc_1_0DC8
        jmp    dword ptr [edx*4]             ; -> obj1:0x1250
loc_1_0DC8:
        mov    dword ptr [ebp - 0x10], 0xc00000bb
        jmp    loc_1_11B8
loc_1_0DD4:
        mov    al, byte ptr [esi + 0x456]
        mov    esi, 1
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 4]
        jmp    loc_1_11BE
loc_1_0DEA:
        mov    ax, word ptr [esi + 0x457]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_11BE
loc_1_0E02:
        mov    ax, word ptr [esi + 0x459]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_11BE
loc_1_0E1A:
        mov    ax, word ptr [esi + 0x45b]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_11BE
loc_1_0E32:
        mov    al, byte ptr [esi + 0x45d]
        mov    esi, 1
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 4]
        jmp    loc_1_11BE
loc_1_0E48:
        mov    ax, word ptr [esi + 0x460]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_11BE
loc_1_0E60:
        mov    ax, word ptr [esi + 0x45e]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_11BE
loc_1_0E78:
        xor    eax, eax
        mov    ax, word ptr [esi + 0x460]
        lea    ecx, [eax + eax*4]
        lea    edi, [ecx*2]
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_1_0EA1
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], edi
        jmp    loc_1_11B8
loc_1_0EA1:
        push   edi
        mov    eax, dword ptr [esi + 0x452]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   ecx
        mov    bl, 1
        call   sub_1_5940
        mov    ecx, dword ptr [ebp + 0x24]
        mov    dword ptr [ecx], edi
        jmp    loc_1_11B8
loc_1_0EBE:
        cmp    dword ptr [ebp + 0x1c], 0x1e
        jae    loc_1_0ED9
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], 0x1e
        jmp    loc_1_11B8
loc_1_0ED9:
        push   0x1e
        mov    eax, dword ptr [esi + 0x452]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   ecx
        mov    bl, 1
        call   sub_1_5940
        mov    ecx, dword ptr [ebp + 0x24]
        mov    dword ptr [ecx], 0x1e
        jmp    loc_1_11B8
loc_1_0EFB:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_0F10
        mov    eax, 0                        ; -> loc_1_0324
        mov    esi, 0x5c
        jmp    loc_1_11BE
loc_1_0F10:
        mov    eax, 0                        ; -> loc_1_0288
        mov    esi, 0x9c
        jmp    loc_1_11BE
loc_1_0F1F:
        cmp    byte ptr [esi + 0x83], 0
        mov    dword ptr [ebp - 0x1c], 2
        jne    loc_1_0F36
        mov    dword ptr [ebp - 0x1c], 0
loc_1_0F36:
        lea    eax, [ebp - 0x1c]
        mov    esi, 4
        jmp    loc_1_11BE
loc_1_0F43:
        lea    eax, [ebp - 0x20]
        mov    esi, 4
        jmp    loc_1_11BE
loc_1_0F50:
        mov    eax, dword ptr [esi + 0x138]
        mov    esi, 4
        sub    eax, 0xe
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_11BE
loc_1_0F69:
        mov    eax, dword ptr [esi + 0x13c]
        mov    esi, 4
        sub    eax, 0xe
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_11BE
loc_1_0F82:
        mov    ecx, 0x64
        mov    eax, dword ptr [esi + 0x140]
        sub    edx, edx
        mov    esi, 4
        div    ecx
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_11BE
loc_1_0FA1:
        mov    eax, dword ptr [esi + 0x144]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_11BE
loc_1_0FB7:
        mov    eax, dword ptr [esi + 0x148]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_11BE
loc_1_0FCD:
        mov    eax, dword ptr [esi + 0x14c]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_11BE
loc_1_0FE3:
        mov    eax, dword ptr [esi + 0x150]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_11BE
loc_1_0FF9:
        push   3
        add    esi, 0x44
        push   esi
        lea    eax, [ebp - 0xc]
        push   eax
        mov    esi, 4
        call   sub_1_5940
        and    dword ptr [ebp - 0xc], 0xffffff00
        lea    eax, [ebp - 0xc]
        jmp    loc_1_11BE
loc_1_101C:
        mov    eax, 0                        ; -> loc_1_0380
        mov    esi, 0x18
        jmp    loc_1_11BE
loc_1_102B:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_1042
        test   ecx, ecx
        jne    loc_1_103D
        mov    eax, dword ptr [esi + 0x5e]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_1051
loc_1_103D:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1051
loc_1_1042:
        xor    eax, eax
        test   ecx, ecx
        jne    loc_1_1051
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 0x14]
        mov    eax, dword ptr [eax + 8]
loc_1_1051:
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_11BE
loc_1_1061:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_106E
        mov    eax, dword ptr [esi]
        sub    eax, 0xe
        jmp    loc_1_1074
loc_1_106E:
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 0x18]
loc_1_1074:
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_11BE
loc_1_1084:
        lea    eax, [ebp - 6]
        mov    esi, 2
        mov    word ptr [ebp - 6], 0x30a
        jmp    loc_1_11BE
loc_1_1097:
        mov    eax, dword ptr [esi + 0x13c]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_11BE
loc_1_10AD:
        lea    eax, [ebp - 0xc]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], 7
        jmp    loc_1_11BE
loc_1_10C1:
        push   6
        add    esi, 0x44
        push   esi
        lea    eax, [ebp - 0x28]
        push   eax
        mov    esi, 6
        call   sub_1_5940
        lea    eax, [ebp - 0x28]
        jmp    loc_1_11BE
loc_1_10DD:
        push   6
        add    esi, 0x3e
        push   esi
        lea    eax, [ebp - 0x28]
        push   eax
        mov    esi, 6
        call   sub_1_5940
        lea    eax, [ebp - 0x28]
        jmp    loc_1_11BE
loc_1_10F9:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_114E
        mov    ecx, dword ptr [esi + 0x5e]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x18], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_1_1123
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_1_11B8
loc_1_1123:
        mov    eax, dword ptr [ebp + 0x18]
        lea    edx, [ebp - 0x18]
        push   eax
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        lea    eax, [ebp - 0x10]
        push   ebx
        push   ecx
        push   eax
        call   sub_2_0760
loc_1_1139:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 0x24]
        cmp    dword ptr [ebp - 0x10], 0
        lea    eax, [eax + eax*2]
        mov    bl, 1
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_11B8
loc_1_114E:
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x5e]
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        push   ecx
        call   sub_2_0660
loc_1_115E:
        mov    dword ptr [ebp - 0x18], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_1_1179
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_1_11B8
loc_1_1179:
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [edi + 0x14]
        push   edx
        mov    ecx, dword ptr [esi + 0x5e]
        push   eax
        lea    edx, [ebp - 0x10]
        push   ecx
        push   edx
        mov    bl, 1
        call   sub_2_06A0
loc_1_1198:
        mov    ecx, dword ptr [ebp - 0x18]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_11B8
loc_1_11A7:
        lea    eax, [ebp - 0xc]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], 0x10
        jmp    loc_1_11BE
loc_1_11B8:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0x14]
loc_1_11BE:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_11F5
        test   bl, bl
        jne    loc_1_11F5
        cmp    dword ptr [ebp + 0x1c], esi
        jae    loc_1_11E5
        mov    ecx, dword ptr [ebp + 0x20]
        pop    edi
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx], esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_11E5:
        push   esi
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_5940
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], esi
loc_1_11F5:
        mov    eax, dword ptr [ebp - 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 049h, 000h
        ; ---- dispatch table (28 entries) ----
        dd  loc_1_0EFB
        dd  loc_1_0F1F
        dd  loc_1_0F43
        dd  loc_1_0F43
        dd  loc_1_0F50
        dd  loc_1_0F69
        dd  loc_1_0F82
        dd  loc_1_0FA1
        dd  loc_1_0FB7
        dd  loc_1_0FCD
        dd  loc_1_0FE3
        dd  loc_1_0FF9
        dd  loc_1_101C
        dd  loc_1_102B
        dd  loc_1_1061
        dd  loc_1_1084
        dd  loc_1_1097
        dd  loc_1_0D9C
        dd  loc_1_10AD
        dd  loc_1_0DD4
        dd  loc_1_0DEA
        dd  loc_1_0E02
        dd  loc_1_0E1A
        dd  loc_1_0E32
        dd  loc_1_0E48
        dd  loc_1_0E60
        dd  loc_1_0E78
        dd  loc_1_0EBE
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1280:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esi + 0x18]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x14]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_1_0D30
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010103
        ja     loc_1_131A
        je     loc_1_14EF
        cmp    eax, 0x1010e
        je     loc_1_145D
        cmp    eax, 0x1010f
        je     loc_1_1499
        cmp    eax, 0x10112
        je     loc_1_14E6
loc_1_131A:
        mov    edx, eax
        and    edx, 0xff000000
        cmp    edx, 0xff000000
        je     loc_1_133E
        mov    dword ptr [ebp - 4], 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_152D
loc_1_133E:
        add    eax, 0xfefefe
        cmp    eax, 3
        ja     loc_1_134F
        jmp    dword ptr [eax*4]             ; -> obj1:0x154C
loc_1_134F:
        mov    dword ptr [ebp - 4], 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_152D
loc_1_1363:
        cmp    ebx, 2
        je     loc_1_137C
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_152D
loc_1_137C:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 0x457
        push   edi
        call   sub_1_5940
        jmp    loc_1_152D
loc_1_1393:
        cmp    ebx, 2
        je     loc_1_13AC
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_152D
loc_1_13AC:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 0x457
        push   edi
        call   sub_1_5940
        jmp    loc_1_152D
loc_1_13C3:
        cmp    ebx, 2
        je     loc_1_13DC
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_152D
loc_1_13DC:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 0x457
        push   edi
        call   sub_1_5940
        jmp    loc_1_152D
loc_1_13F3:
        cmp    ebx, 1
        je     loc_1_140C
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_152D
loc_1_140C:
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx]
        cmp    byte ptr [edi + 0x45d], al
        je     loc_1_152D
        mov    byte ptr [edi + 0x45d], al
        test   al, al
        jne    loc_1_152D
        xor    edx, edx
        xor    eax, eax
        mov    word ptr [edi + 0x45e], 0
        mov    dx, word ptr [edi + 0x460]
        mov    edi, dword ptr [edi + 0x452]
        lea    edx, [edx + edx*4]
        add    edx, edx
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        jmp    loc_1_152D
loc_1_145D:
        cmp    ebx, 4
        je     loc_1_1476
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_152D
loc_1_1476:
        push   4
        lea    eax, [ebp - 0xc]
        push   ecx
        push   eax
        call   sub_1_5940
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edi
        call   sub_1_1840
        jmp    loc_1_152A
loc_1_1499:
        cmp    ebx, 4
        je     loc_1_14AF
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_152D
loc_1_14AF:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_5940
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x138]
        sub    eax, 0xe
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_14DD
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        push   edi
        mov    dword ptr [eax + 0x18], ecx
        call   sub_1_18B0
        jmp    loc_1_152D
loc_1_14DD:
        mov    dword ptr [ebp - 4], 0xc0010014
        jmp    loc_1_152D
loc_1_14E6:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_152D
loc_1_14EF:
        mov    edi, 6
        mov    eax, ebx
        sub    edx, edx
        div    edi
        test   edx, edx
        je     loc_1_150F
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_152D
loc_1_150F:
        push   ecx
        mov    eax, ebx
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edi
        call   sub_1_17D0
loc_1_152A:
        mov    dword ptr [ebp - 4], eax
loc_1_152D:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_153D
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], ebx
loc_1_153D:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
        ; ---- dispatch table (4 entries) ----
        dd  loc_1_1363
        dd  loc_1_1393
        dd  loc_1_13C3
        dd  loc_1_13F3
        db  08Dh, 064h, 024h, 000h
sub_1_1560:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 8], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 0xc]
        mov    eax, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        push   edx
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x10], ecx
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0x10]
        push   ecx
        mov    eax, dword ptr [esi + 0x14]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_0D30
        cmp    eax, 0xc00000bb
        mov    edi, eax
        jne    loc_1_1706
        xor    edi, edi
        push   ebx
        call   sub_1_5270
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x2020d
        ja     loc_1_15E4
        je     loc_1_167F
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_1_1612
        jmp    dword ptr [eax*4]             ; -> obj1:0x1720
loc_1_15E4:
        cmp    eax, 0x1020201
        ja     loc_1_161C
        je     loc_1_169F
        cmp    eax, 0x1020101
        je     loc_1_1687
        cmp    eax, 0x1020102
        je     loc_1_168F
        cmp    eax, 0x1020103
        je     loc_1_1697
loc_1_1612:
        mov    edi, 0xc0010017
        jmp    loc_1_16D8
loc_1_161C:
        sub    eax, 0x1020202
        cmp    eax, 5
        ja     loc_1_1612
        jmp    dword ptr [eax*4]             ; -> obj1:0x1734
loc_1_162D:
        mov    eax, dword ptr [ebx + 0x1b8]
        jmp    loc_1_16D5
loc_1_1638:
        mov    eax, dword ptr [ebx + 0x1bc]
        jmp    loc_1_16D5
loc_1_1643:
        mov    eax, dword ptr [ebx + 0x1ec]
        add    eax, dword ptr [ebx + 0x1dc]
        add    eax, dword ptr [ebx + 0x1cc]
        add    eax, dword ptr [ebx + 0x1f0]
        jmp    loc_1_16D5
loc_1_165D:
        mov    eax, dword ptr [ebx + 0x1f4]
        add    eax, dword ptr [ebx + 0x1f8]
        add    eax, dword ptr [ebx + 0x1fc]
        add    eax, dword ptr [ebx + 0x200]
        jmp    loc_1_16D5
loc_1_1677:
        mov    eax, dword ptr [ebx + 0x200]
        jmp    loc_1_16D5
loc_1_167F:
        mov    eax, dword ptr [ebx + 0x1f8]
        jmp    loc_1_16D5
loc_1_1687:
        mov    eax, dword ptr [ebx + 0x1f4]
        jmp    loc_1_16D5
loc_1_168F:
        mov    eax, dword ptr [ebx + 0x1d4]
        jmp    loc_1_16D5
loc_1_1697:
        mov    eax, dword ptr [ebx + 0x1d8]
        jmp    loc_1_16D5
loc_1_169F:
        mov    eax, dword ptr [ebx + 0x1e8]
        jmp    loc_1_16D5
loc_1_16A7:
        mov    eax, dword ptr [ebx + 0x1dc]
        jmp    loc_1_16D5
loc_1_16AF:
        mov    eax, dword ptr [ebx + 0x200]
        jmp    loc_1_16D5
loc_1_16B7:
        mov    eax, dword ptr [ebx + 0x1cc]
        jmp    loc_1_16D5
loc_1_16BF:
        mov    eax, dword ptr [ebx + 0x1d0]
        jmp    loc_1_16D5
loc_1_16C7:
        mov    eax, dword ptr [ebx + 0x1e0]
        jmp    loc_1_16D5
loc_1_16CF:
        mov    eax, dword ptr [ebx + 0x1e4]
loc_1_16D5:
        mov    dword ptr [ebp - 4], eax
loc_1_16D8:
        push   ebx
        call   sub_1_5290
        test   edi, edi
        jne    loc_1_1706
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_1_16FD
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_5940
        add    dword ptr [ebp - 0xc], 4
        jmp    loc_1_1706
loc_1_16FD:
        add    dword ptr [ebp - 8], 4
        mov    edi, 0xc0010014
loc_1_1706:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
        ; ---- dispatch table (11 entries) ----
        dd  loc_1_162D
        dd  loc_1_1638
        dd  loc_1_1643
        dd  loc_1_165D
        dd  loc_1_1677
        dd  loc_1_16A7
        dd  loc_1_16AF
        dd  loc_1_16B7
        dd  loc_1_16BF
        dd  loc_1_16C7
        dd  loc_1_16CF
        db  08Dh, 064h, 024h, 000h
loc_1_1750:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_177F
        cmp    eax, 0x20101
        jae    loc_1_17C2
        cmp    eax, 0x10101
        jb     loc_1_17B9
        cmp    eax, 0x10111
        jbe    loc_1_17C2
        mov    eax, 0xc0010017
        pop    ebp
        ret    8
loc_1_177F:
        cmp    eax, 0x1010104
        ja     loc_1_179D
        cmp    eax, 0x1010101
        jae    loc_1_17C2
        cmp    eax, 0x2020d
        je     loc_1_17C2
        mov    eax, 0xc0010017
        pop    ebp
        ret    8
loc_1_179D:
        cmp    eax, 0x1020207
        ja     loc_1_17B9
        cmp    eax, 0x1020201
        jae    loc_1_17C2
        cmp    eax, 0x1020101
        jb     loc_1_17B9
        cmp    eax, 0x1020103
        jbe    loc_1_17C2
loc_1_17B9:
        mov    eax, 0xc0010017
        pop    ebp
        ret    8
loc_1_17C2:
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1560
        pop    ebp
        ret    8
sub_1_17D0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 0x83], 0
        push   edi
        jne    loc_1_1826
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_1819
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x14]
        push   edx
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [ecx + 0x5e]
        push   ecx
        call   sub_2_0300
loc_1_1809:
        mov    edi, eax
        push   esi
        call   sub_1_1DD0
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x14
loc_1_1819:
        mov    edi, 0xc0010002
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x14
loc_1_1826:
        mov    edi, 0xc001000d
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1840:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        cmp    byte ptr [ecx + 0x83], 0
        jne    loc_1_189C
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_188F
        inc    dword ptr [esi + 0x1c]
        cmp    dword ptr [ecx + 0x134], edi
        jne    loc_1_1881
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    edi, dword ptr [esi + 0x14]
        push   edi
        mov    eax, dword ptr [ecx + 0x5e]
        push   eax
        call   sub_2_05E0
loc_1_187F:
        mov    edi, eax
loc_1_1881:
        push   esi
        call   sub_1_1DD0
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
loc_1_188F:
        mov    edi, 0xc0010002
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
loc_1_189C:
        mov    edi, 0xc001000d
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_18B0:
        push   ebp
        mov    edx, 0x3c
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0x52]
        test   esi, esi
        je     loc_1_18D3
loc_1_18C3:
        mov    eax, dword ptr [esi + 0x18]
        cmp    eax, edx
        jbe    loc_1_18CC
        mov    edx, eax
loc_1_18CC:
        mov    esi, dword ptr [esi + 0xc]
        test   esi, esi
        jne    loc_1_18C3
loc_1_18D3:
        add    edx, 0xe
        pop    esi
        pop    ebp
        mov    dword ptr [ecx], edx
        ret    4
        db  08Dh, 049h, 000h
loc_1_18E0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [eax]
        mov    esi, dword ptr [ebp + 0x18]
        push   esi
        mov    dword ptr [esi + 0x28], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    word ptr [esi + 0x2e], ax
        mov    eax, dword ptr [ebp + 0x10]
        add    ax, 0xe
        mov    word ptr [esi + 0x2c], ax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        call   dword ptr [eax + 0xc]
        test   eax, eax
        je     loc_1_191E
        cmp    eax, 1
        je     loc_1_192D
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    0x18
loc_1_191E:
        mov    ecx, dword ptr [esi + 0x30]
        mov    eax, dword ptr [ebp + 0x1c]
        pop    esi
        pop    ebp
        mov    dword ptr [eax], ecx
        xor    eax, eax
        ret    0x18
loc_1_192D:
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1940:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi]
        mov    edx, dword ptr [edi + 0x160]
        in     ax, dx
        cmp    ax, 0xffff
        jne    loc_1_196A
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_196A:
        lea    eax, [ebp - 4]
        mov    ebx, dword ptr [ebp + 0xc]
        push   eax
        push   0
        push   0
        push   0
        push   ebx
        call   sub_1_5590
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0x28], esi
        mov    word ptr [ebx + 0x2c], cx
        cmp    ecx, 0xe
        mov    byte ptr [ebx + 0x2e], 0
        jb     loc_1_1A7E
        cmp    dword ptr [edi + 0x13c], ecx
        jb     loc_1_1A7E
        cmp    byte ptr [esi + 0x20], 0
        je     loc_1_19B4
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_19B4:
        cmp    byte ptr [edi + 0x83], 0
        je     loc_1_19CB
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_19CB:
        inc    dword ptr [esi + 0x1c]
        test   byte ptr [edi + 0x21], 0xa
        mov    dword ptr [ebx + 0x24], 0
        je     loc_1_19FD
        cmp    dword ptr [edi + 0x15], 0
        jne    loc_1_19E6
        mov    dword ptr [edi + 0x15], ebx
        jmp    loc_1_19EC
loc_1_19E6:
        mov    eax, dword ptr [edi + 0x11]
        mov    dword ptr [eax + 0x24], ebx
loc_1_19EC:
        mov    eax, 0x103
        mov    dword ptr [edi + 0x11], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_19FD:
        push   ebx
        mov    dword ptr [edi + 0x15], ebx
        push   edi
        mov    dword ptr [edi + 0x11], ebx
        or     byte ptr [edi + 0x21], 8
        call   sub_1_43F0
        test   eax, eax
        je     loc_1_1A20
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1A20:
        mov    eax, dword ptr [edi + 0x15]
        mov    ebx, dword ptr [eax + 0x24]
        mov    dword ptr [edi + 0x15], ebx
        test   ebx, ebx
        je     loc_1_1A46
loc_1_1A2D:
        push   ebx
        push   edi
        call   sub_1_43F0
        test   eax, eax
        jne    loc_1_1A64
        push   ebx
        push   edi
        call   sub_1_1A90
        mov    ebx, dword ptr [edi + 0x15]
        test   ebx, ebx
        jne    loc_1_1A2D
loc_1_1A46:
        and    byte ptr [edi + 0x21], 0xf7
        mov    eax, dword ptr [esi + 0x1c]
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        jne    loc_1_1A59
        push   esi
        call   sub_1_1DF0
loc_1_1A59:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1A64:
        mov    eax, dword ptr [esi + 0x1c]
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        jne    loc_1_1A73
        push   esi
        call   sub_1_1DF0
loc_1_1A73:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1A7E:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_1A90:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        push   edi
        mov    al, byte ptr [edx + 0x2e]
        test   al, 2
        jne    loc_1_1AA4
        xor    edi, edi
        jmp    loc_1_1AB6
loc_1_1AA4:
        and    al, 8
        cmp    al, 1
        sbb    edi, edi
        and    edi, 0xfffefff5
        sub    edi, 0x3ffefff4
loc_1_1AB6:
        mov    ecx, dword ptr [ebp + 8]
        push   edi
        push   edx
        mov    eax, dword ptr [ecx + 0x15]
        mov    esi, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x15], esi
        mov    esi, dword ptr [edx + 0x28]
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_54AA
        mov    eax, dword ptr [esi + 0x1c]
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        jne    loc_1_1ADF
        push   esi
        call   sub_1_1DF0
loc_1_1ADF:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1AF0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_4FC0
        mov    eax, dword ptr [esi + 4]
        add    esi, 0x62
        push   eax
        push   esi
        call   sub_1_5542
        pop    esi
        pop    ebp
        ret    0x10
        db  090h
loc_1_1B10:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        mov    eax, 0
        pop    ebp
        cmp    byte ptr [ecx + 0x83], 1
        adc    eax, -1
        and    eax, 0xc001000d
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1B40:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        mov    edx, dword ptr [eax]
        xor    eax, eax
        cmp    byte ptr [edx + 0x83], al
        je     loc_1_1B5C
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_1_1B5C:
        mov    ecx, dword ptr [ebp + 0xc]
        test   ecx, 0xf010
        je     loc_1_1B70
        mov    eax, 0xc00000bb
        pop    ebp
        ret    0x14
loc_1_1B70:
        test   cl, 1
        je     loc_1_1B7A
        mov    eax, 1
loc_1_1B7A:
        test   cl, 2
        je     loc_1_1B82
        or     eax, 2
loc_1_1B82:
        test   cl, 4
        je     loc_1_1B8A
        or     eax, 2
loc_1_1B8A:
        test   cl, 8
        je     loc_1_1B92
        or     eax, 4
loc_1_1B92:
        mov    byte ptr [edx + 0x82], 0
        test   cl, 0x20
        je     loc_1_1BA8
        mov    byte ptr [edx + 0x82], 1
        or     eax, 8
loc_1_1BA8:
        add    ax, 0x8000
        pop    ebp
        mov    word ptr [edx + 0xf0], ax
        mov    edx, dword ptr [edx + 0x15c]
        out    dx, ax
        xor    eax, eax
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1BD0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 0x10]
        push   edi
        mov    edx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0xffffffff
        lea    eax, [eax + edx*4]
loc_1_1BF5:
        test   edx, edx
        je     loc_1_1C04
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_1_1BF5
        mov    dword ptr [ecx], edx
loc_1_1C04:
        cmp    dword ptr [ecx], -1
        jne    loc_1_1C17
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_1C17:
        mov    eax, dword ptr [0]            ; -> obj1:0x0398
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x49
        push   ecx
        call   sub_1_5566
        test   eax, eax
        je     loc_1_1C3C
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_1C3C:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        add    eax, 0x14
        mov    esi, dword ptr [ebp + 0x20]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x5e]
        push   eax
        push   ecx
        call   sub_2_0170
loc_1_1C57:
        test   al, al
        jne    loc_1_1C76
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x49
        push   eax
        call   sub_1_556C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_1C76:
        cmp    byte ptr [esi + 0x83], 0
        je     loc_1_1C95
        mov    eax, dword ptr [esi + 0x5a]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, 0x103
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x5a], ecx
        jmp    loc_1_1CA6
loc_1_1C95:
        mov    eax, dword ptr [esi + 0x52]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x52], ecx
loc_1_1CA6:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x4a]
        mov    dword ptr [eax + 0x10], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [eax + 4], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], edx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x2a], cl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 1
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx + 0x18], eax
        cmp    dword ptr [esi + 0x56], ecx
        jne    loc_1_1CFA
        mov    eax, dword ptr [esi + 4]
        lea    ecx, [esi + 0x62]
        push   eax
        push   ecx
        call   sub_1_5542
loc_1_1CFA:
        push   esi
        inc    dword ptr [esi + 0x56]
        call   sub_1_51D0
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Bh, 0FFh
loc_1_1D10:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x20], 0
        mov    edi, dword ptr [esi]
        je     loc_1_1D2B
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_1D2B:
        mov    byte ptr [esi + 0x20], 1
        inc    dword ptr [esi + 0x1c]
        mov    eax, dword ptr [edi + 0x52]
        cmp    esi, eax
        jne    loc_1_1D41
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edi + 0x52], eax
        jmp    loc_1_1D54
loc_1_1D41:
        cmp    dword ptr [eax + 0xc], esi
        je     loc_1_1D4E
loc_1_1D46:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], esi
        jne    loc_1_1D46
loc_1_1D4E:
        mov    ecx, dword ptr [esi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_1D54:
        lea    eax, [esi + 0x21]
        mov    ecx, dword ptr [esi + 0x14]
        push   eax
        mov    edx, dword ptr [edi + 0x5e]
        push   ecx
        push   edx
        call   sub_2_0240
loc_1_1D65:
        cmp    dword ptr [edi + 0x128], 3
        jne    loc_1_1D82
        push   esi
        call   sub_1_1DD0
        push   esi
        call   sub_1_1DD0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_1D82:
        test   eax, eax
        je     loc_1_1D8D
        cmp    eax, 0x103
        jne    loc_1_1D93
loc_1_1D8D:
        push   esi
        call   sub_1_1DD0
loc_1_1D93:
        push   esi
        call   sub_1_1DD0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1DB0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_1DD0
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1DD0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x1c]
        dec    ecx
        mov    dword ptr [eax + 0x1c], ecx
        jne    loc_1_1DE6
        push   eax
        call   sub_1_1DF0
loc_1_1DE6:
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1DF0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi]
        push   edi
        call   sub_1_18B0
        mov    eax, dword ptr [edi + 0x56]
        mov    ecx, 1
        dec    eax
        push   edi
        mov    dword ptr [edi + 0x56], eax
        cmp    ecx, eax
        mov    al, cl
        adc    al, 0xff
        mov    byte ptr [edi + 0xc], al
        call   sub_1_5200
        push   0
        push   0x49
        push   esi
        call   sub_1_556C
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1E30:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [eax + 0x20], 0
        mov    esi, dword ptr [eax]
        je     loc_1_1E4A
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    4
loc_1_1E4A:
        mov    cl, byte ptr [esi + 0x21]
        test   cl, 2
        je     loc_1_1E5C
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    4
loc_1_1E5C:
        or     cl, 2
        push   0x40010004
        push   esi
        mov    byte ptr [esi + 0x21], cl
        inc    dword ptr [eax + 0x1c]
        mov    dword ptr [esi + 0x6e], eax
        call   sub_1_1F60
        mov    byte ptr [esi + 0x83], 1
        push   esi
        call   sub_1_52A0
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1E90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0x11]
loc_1_1E9E:
        push   edi
        call   sub_1_1F40
        test   eax, eax
        je     loc_1_1EC7
        or     byte ptr [eax + 0x2e], 0xa
        mov    dword ptr [eax + 0x24], ebx
        cmp    dword ptr [esi + 0x1d], ebx
        jne    loc_1_1EBC
        mov    dword ptr [esi + 0x1d], eax
        mov    dword ptr [esi + 0x19], eax
        jmp    loc_1_1E9E
loc_1_1EBC:
        mov    ecx, dword ptr [esi + 0x19]
        mov    dword ptr [ecx + 0x24], eax
        mov    dword ptr [esi + 0x19], eax
        jmp    loc_1_1E9E
loc_1_1EC7:
        lea    edi, [esi + 0x19]
loc_1_1ECA:
        push   edi
        call   sub_1_1F40
        test   eax, eax
        je     loc_1_1EDD
        push   eax
        push   esi
        call   sub_1_1A90
        jmp    loc_1_1ECA
loc_1_1EDD:
        mov    edi, dword ptr [esi + 0x6e]
        push   0x40010005
        and    byte ptr [esi + 0x21], 0xfd
        mov    byte ptr [esi + 0x83], 0
        push   esi
        call   sub_1_1F60
        push   0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_54B6
        push   edi
        call   sub_1_1DD0
        mov    ecx, dword ptr [esi + 0x5a]
        test   ecx, ecx
        je     loc_1_1F34
loc_1_1F0E:
        mov    eax, dword ptr [esi + 0x5a]
        mov    ebx, dword ptr [esi + 0x52]
        push   0
        push   0
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x5a], edx
        mov    dword ptr [ecx + 0xc], ebx
        mov    dword ptr [esi + 0x52], ecx
        mov    eax, dword ptr [ecx + 4]
        push   eax
        call   sub_1_549E
        mov    ecx, dword ptr [esi + 0x5a]
        test   ecx, ecx
        jne    loc_1_1F0E
loc_1_1F34:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_1F40:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 4]
        test   eax, eax
        je     loc_1_1F54
        mov    edx, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 4], edx
loc_1_1F54:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1F60:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 0x52]
        test   edi, edi
        je     loc_1_1FA9
        mov    esi, dword ptr [ebp + 0xc]
loc_1_1F77:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi + 0x1c]
        mov    ebx, dword ptr [edi + 0xc]
        push   esi
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        call   sub_1_54CE
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        call   sub_1_54D4
        push   edi
        call   sub_1_1DD0
        mov    edi, ebx
        test   ebx, ebx
        jne    loc_1_1F77
loc_1_1FA9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1FC0:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x5e]
        push   edx
        call   sub_1_58D0
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 055h, 033h, 0D2h, 08Bh, 0ECh, 08Bh, 04Ch, 024h
        db  008h, 08Dh, 081h, 02Bh, 001h, 000h, 000h, 088h, 051h, 05Ah, 083h, 0C1h, 05Ah, 089h, 010h, 089h
        db  050h, 004h, 089h, 050h, 008h, 089h, 050h, 00Ch, 08Dh, 081h, 0E1h, 000h, 000h, 000h, 089h, 010h
        db  089h, 050h, 004h, 088h, 050h, 008h, 088h, 091h, 0EAh, 000h, 000h, 000h, 0C6h, 081h, 0EBh, 000h
        db  000h, 000h, 040h, 0C6h, 081h, 0ECh, 000h, 000h, 000h, 05Bh, 0C6h, 081h, 0EDh, 000h, 000h, 000h
        db  06Fh, 0C6h, 081h, 0EEh, 000h, 000h, 000h, 080h, 0C6h, 081h, 0EFh, 000h, 000h, 000h, 08Fh, 0C6h
        db  081h, 0F0h, 000h, 000h, 000h, 09Dh, 0C6h, 081h, 0F1h, 000h, 000h, 000h, 0A9h, 0C6h, 081h, 0F2h
        db  000h, 000h, 000h, 0B5h, 0C6h, 081h, 0F3h, 000h, 000h, 000h, 0C0h, 0C6h, 081h, 0F4h, 000h, 000h
        db  000h, 0CAh, 033h, 0C0h, 05Dh, 0C6h, 081h, 0F5h, 000h, 000h, 000h, 0D4h, 0C6h, 081h, 0F6h, 000h
        db  000h, 000h, 0DEh, 0C6h, 081h, 0F7h, 000h, 000h, 000h, 0E7h, 0C6h, 081h, 0F8h, 000h, 000h, 000h
        db  0EFh, 0C6h, 081h, 0F9h, 000h, 000h, 000h, 0F8h, 0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h
        db  000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2090:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0xb0]
        mov    byte ptr [edi], dl
        mov    byte ptr [edi + 1], dl
        mov    byte ptr [edi + 2], dl
        mov    byte ptr [edi + 3], dl
        mov    byte ptr [edi + 4], dl
        mov    dword ptr [edi + 0xc], 0x3c
        mov    dword ptr [edi + 0x14], 0x78fe
        mov    dword ptr [edi + 0x28], edx
        mov    dword ptr [edi + 0x24], 0x8800
        mov    dword ptr [edi + 0x34], edx
        mov    dword ptr [edi + 0x38], edx
        mov    dword ptr [edi + 0x3c], edx
        mov    eax, 0x5ea
        mov    word ptr [edi + 0x40], 0x8000
        mov    dword ptr [edi + 0x68], edx
        mov    dword ptr [edi + 0x6c], edx
        mov    dword ptr [edi + 0xa4], edx
        mov    dword ptr [edi + 0x70], edx
        mov    dword ptr [edi + 0x84], edx
        mov    dword ptr [edi + 0x88], eax
        mov    ecx, 0x20
        mov    dword ptr [edi + 0x8c], eax
        mov    eax, 4
        mov    dword ptr [edi + 0x90], 0x989680
        mov    dword ptr [edi + 0x9c], eax
        mov    dword ptr [edi + 0xa0], eax
        mov    word ptr [edi + 0xd8], 1
        mov    word ptr [edi + 0xda], cx
        mov    dword ptr [edi + 0xdc], 0x400
        mov    dword ptr [edi + 0xe0], ecx
        mov    dword ptr [edi + 0xe4], 0x1388
        mov    dword ptr [edi + 0xe8], 0x1d4c
        mov    dword ptr [edi + 0xec], 0x9810
        mov    dword ptr [edi + 0xf0], eax
        mov    word ptr [edi + 0xf4], dx
        mov    word ptr [edi + 0xf6], dx
        mov    word ptr [edi + 0xf8], dx
        mov    dword ptr [edi + 0xfa], 0x9900
        mov    dword ptr [edi + 0xfe], 0x100
        mov    word ptr [edi + 0x102], dx
        mov    word ptr [edi + 0x104], dx
        mov    word ptr [edi + 0x106], dx
        mov    dword ptr [edi + 0x160], 0    ; -> loc_1_4010
        mov    dword ptr [edi + 0x164], edx
        push   esi
        mov    dword ptr [edi + 0x158], edx
        mov    dword ptr [edi + 0x15c], edx
        mov    dword ptr [edi + 0x174], 0    ; -> loc_1_41D0
        mov    dword ptr [edi + 0x178], edx
        mov    dword ptr [edi + 0x16c], edx
        mov    dword ptr [edi + 0x170], edx
        call   sub_1_3810
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_21F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        add    ebx, 0xb0
        mov    eax, dword ptr [ecx + 0x22c]
        mov    esi, dword ptr [ecx + 0x184]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xe
        mov    dword ptr [ebx + 0xac], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xe
        mov    dword ptr [ebx + 0xb0], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xb
        mov    dword ptr [ebx + 0xb4], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xa
        mov    dword ptr [ebx + 0xb8], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xc
        mov    dword ptr [ebx + 0xbc], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 8
        mov    dword ptr [ebx + 0xc0], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 0xa
        mov    dword ptr [ebx + 0xc4], eax
        mov    eax, dword ptr [ebx + 0xa8]
        add    eax, 8
        mov    dword ptr [ebx + 0xc8], eax
        mov    eax, dword ptr [ebx + 0xa8]
        mov    dword ptr [ebx + 0xcc], eax
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebx + 0x78], 0
        jne    loc_1_22CE
        cmp    byte ptr [ebx + 0xa], 0
        jne    loc_1_22CE
        cli    
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_3530
        mov    ecx, dword ptr [ebx + 0x68]
        mov    edx, esi
        sub    cl, 0x28
        mov    al, cl
        out    dx, al
        mov    eax, dword ptr [ebx + 0xa4]
        sub    eax, 0x200
        shr    eax, 4
        sub    al, 0x20
        out    dx, al
        sti    
loc_1_22CE:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 0x800
        mov    edx, dword ptr [ecx + 0x15c]
        mov    esi, dword ptr [ecx + 0x158]
        out    dx, ax
        mov    edx, esi
        in     ax, dx
        cmp    ax, 0x6d50
        je     loc_1_22FC
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_22FC:
        lea    edx, [esi + 0xc]
        mov    eax, 0x5555
        out    dx, ax
        in     ax, dx
        cmp    ax, 0x5555
        je     loc_1_231C
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_231C:
        mov    eax, 0xaaaa
        out    dx, ax
        in     ax, dx
        cmp    ax, 0xaaaa
        je     loc_1_2339
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2339:
        cmp    dword ptr [ebx + 0x80], 0
        jne    loc_1_2376
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x128]
        cmp    eax, 2
        je     loc_1_235F
        cmp    eax, 3
        je     loc_1_235F
        push   9
        push   ecx
        call   sub_1_36A0
        jmp    loc_1_2364
loc_1_235F:
        lea    edx, [esi + 8]
        in     ax, dx
loc_1_2364:
        shr    ax, 0xc
        mov    ecx, eax
        and    ecx, 0xffff
        mov    dword ptr [ebx + 0x80], ecx
loc_1_2376:
        cmp    dword ptr [ebx + 0x78], 2
        jne    loc_1_2383
        mov    eax, 1
        jmp    loc_1_2385
loc_1_2383:
        xor    eax, eax
loc_1_2385:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0x7c], eax
        mov    byte ptr [ebx + 0x74], al
        mov    edx, dword ptr [ecx + 0x158]
        add    edx, 4
        in     ax, dx
        test   ah, 2
        je     loc_1_23A2
        mov    byte ptr [ebx + 5], 1
loc_1_23A2:
        test   ah, 0x10
        je     loc_1_23AB
        mov    byte ptr [ebx + 6], 1
loc_1_23AB:
        test   ah, 0x20
        je     loc_1_23B4
        mov    byte ptr [ebx + 7], 1
loc_1_23B4:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x158]
        add    edx, 6
        cmp    dword ptr [ebx + 0x44], 0
        in     ax, dx
        jne    loc_1_2406
        mov    cx, ax
        and    ecx, 0xc000
        je     loc_1_23E5
        cmp    ecx, 0x4000
        je     loc_1_23EE
        cmp    ecx, 0xc000
        je     loc_1_23F7
        jmp    loc_1_23FE
loc_1_23E5:
        mov    dword ptr [ebx + 0x44], 1
        jmp    loc_1_23FE
loc_1_23EE:
        mov    dword ptr [ebx + 0x44], 3
        jmp    loc_1_23FE
loc_1_23F7:
        mov    dword ptr [ebx + 0x44], 2
loc_1_23FE:
        test   al, 0x80
        je     loc_1_2406
        mov    byte ptr [ebx + 4], 1
loc_1_2406:
        push   0xd
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_36A0
        cmp    dword ptr [ebx + 0x18], 0
        jne    loc_1_2435
        mov    cx, ax
        and    cx, 0x3f00
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0xffff
        lea    esi, [edx + edx*4 + 5]
        lea    ecx, [esi + esi*4]
        mov    dword ptr [ebx + 0x18], ecx
loc_1_2435:
        test   ah, 0x40
        je     loc_1_243E
        mov    byte ptr [ebx + 3], 1
loc_1_243E:
        push   0xe
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_36A0
        test   ah, ah
        je     loc_1_245B
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_245B:
        push   0x14
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_36A0
        and    eax, 0xf
        push   0xa
        inc    eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        mov    dword ptr [ebx + 0x10], eax
        call   sub_1_36A0
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    esi, 0x44
        push   0xb
        push   ecx
        mov    byte ptr [ecx + 0x45], al
        mov    byte ptr [esi], ah
        call   sub_1_36A0
        mov    ecx, dword ptr [ebp + 8]
        push   0xc
        push   ecx
        mov    byte ptr [ecx + 0x46], ah
        mov    byte ptr [ecx + 0x47], al
        call   sub_1_36A0
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [ecx + 0x3e]
        mov    byte ptr [ecx + 0x48], ah
        mov    byte ptr [ecx + 0x49], al
        mov    al, byte ptr [ecx + 0x3f]
        or     al, byte ptr [ecx + 0x40]
        or     al, byte ptr [ecx + 0x41]
        or     al, byte ptr [ecx + 0x42]
        or     al, byte ptr [ecx + 0x43]
        or     al, byte ptr [edx]
        jne    loc_1_24CA
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        mov    cx, word ptr [esi + 4]
        mov    word ptr [edx + 4], cx
loc_1_24CA:
        cmp    dword ptr [ebx + 0x78], 3
        je     loc_1_24F4
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x158]
        mov    ecx, dword ptr [ecx + 0x130]
        shl    cx, 0xc
        add    edx, 8
        in     ax, dx
        and    ax, 0xfff
        or     cx, ax
        mov    ax, cx
        out    dx, ax
loc_1_24F4:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 1
        mov    esi, dword ptr [ecx + 0x158]
        mov    edi, dword ptr [ecx + 0x15c]
        lea    edx, [esi + 4]
        out    dx, ax
        mov    edx, edi
        mov    eax, 0x5800
        out    dx, ax
        mov    eax, dword ptr [ecx + 0x160]
        mov    edx, eax
        mov    dword ptr [ebp - 0x14], eax
loc_1_2521:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2521
        mov    edx, edi
        mov    eax, 0x2800
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x14]
loc_1_2534:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2534
        mov    edx, edi
        mov    eax, 0x68ff
        out    dx, ax
        mov    eax, 0x806
        out    dx, ax
        mov    eax, 0xb000
        out    dx, ax
        mov    edx, esi
        in     al, dx
        lea    edx, [esi + 4]
        in     al, dx
        lea    edx, [esi + 8]
        in     al, dx
        lea    edx, [esi + 1]
        in     al, dx
        lea    edx, [esi + 2]
        in     al, dx
        lea    edx, [esi + 3]
        in     al, dx
        lea    edx, [esi + 5]
        in     al, dx
        lea    edx, [esi + 6]
        in     al, dx
        lea    edx, [esi + 7]
        in     al, dx
        lea    edx, [esi + 0xc]
        in     ax, dx
        lea    edx, [esi + 0xa]
        mov    esi, dword ptr [ebp + 8]
        in     ax, dx
        mov    edx, edi
        mov    eax, 0x801
        out    dx, ax
        mov    edx, dword ptr [esi + 0x16c]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        cmp    ax, 0xbd4
        mov    dword ptr [ebx + 0x30], ecx
        ja     loc_1_25A8
        mov    ax, 0xbd4
loc_1_25A8:
        and    eax, 0xffff
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0x94], eax
        mov    eax, 0x803
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
        mov    edx, dword ptr [esi + 0x174]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        cmp    ax, 0xbd4
        mov    dword ptr [ebx + 0x2c], ecx
        ja     loc_1_25E0
        mov    ax, 0xbd4
loc_1_25E0:
        and    eax, 0xffff
        mov    dword ptr [ebx + 0x98], eax
        cmp    dword ptr [ebx + 0x78], 3
        jne    loc_1_2605
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 2
        mov    edx, dword ptr [ecx + 0x158]
        out    dx, ax
        add    edx, eax
        out    dx, ax
loc_1_2605:
        mov    ax, word ptr [ebx + 0x14]
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 0x18]
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
        mov    eax, 0x70fe
        lea    edi, [edi + edi*4]
        out    dx, ax
        add    edi, edi
        shr    edi, 5
        cmp    edi, 0xff
        jb     loc_1_2633
        mov    edi, 0xff
loc_1_2633:
        mov    eax, 0x801
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        push   esi
        mov    esi, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0xc]
        mov    eax, 0x200
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        shl    edi, 9
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x168]
        xor    ecx, ecx
        in     al, dx
        mov    cl, al
        sub    edx, edx
        mov    eax, edi
        div    ecx
        and    eax, 0xfffffffc
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0x1c], eax
        mov    eax, 0x5800
        mov    esi, dword ptr [ecx + 0x15c]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x160]
loc_1_2691:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2691
        mov    edx, esi
        mov    eax, 0x68ff
        mov    dword ptr [ebp - 0x18], 0x200
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x18]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x168]
        in     al, dx
        and    eax, 0xff
        shr    eax, 1
        mov    dword ptr [ebx + 0x20], eax
        mov    eax, 0x2800
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
        mov    dword ptr [ebp - 8], edx
        mov    edx, dword ptr [ecx + 0x160]
loc_1_26EC:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_26EC
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 0x68ff
        xor    edi, edi
        xor    esi, esi
        out    dx, ax
        mov    eax, 0x802
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    eax, dword ptr [ecx + 0x158]
        mov    dword ptr [ebp - 4], eax
loc_1_2714:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, edi
        inc    di
        add    edx, esi
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        inc    esi
        mov    al, byte ptr [eax + ecx + 0x3e]
        out    dx, al
        cmp    di, 6
        jb     loc_1_2714
        cmp    byte ptr [ebx + 4], 0
        je     loc_1_2796
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 0xb000
        cmp    byte ptr [ebx + 5], 0
        out    dx, ax
        je     loc_1_275A
        push   ecx
        call   sub_1_2800
        test   al, al
        je     loc_1_275A
        mov    dword ptr [ebx + 0x44], 1
        jmp    loc_1_27A3
loc_1_275A:
        cmp    byte ptr [ebx + 6], 0
        je     loc_1_2778
        push   2
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_2890
        test   al, al
        je     loc_1_2778
        mov    dword ptr [ebx + 0x44], 2
        jmp    loc_1_27A3
loc_1_2778:
        cmp    byte ptr [ebx + 7], 0
        je     loc_1_2796
        push   3
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_2890
        test   al, al
        je     loc_1_2796
        mov    dword ptr [ebx + 0x44], 3
        jmp    loc_1_27A3
loc_1_2796:
        mov    eax, dword ptr [ebx + 0x44]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_2A30
loc_1_27A3:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 0xa800
        mov    esi, dword ptr [ecx + 0x15c]
        mov    ecx, dword ptr [ecx + 0x160]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0x5800
        out    dx, ax
        mov    edx, ecx
loc_1_27C4:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_27C4
        mov    edx, esi
        mov    eax, 0x2800
        out    dx, ax
        mov    edx, ecx
loc_1_27D6:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_27D6
        mov    ax, word ptr [ebx + 0x24]
        mov    edx, esi
        out    dx, ax
        mov    ax, word ptr [ebx + 0xec]
        pop    edi
        out    dx, ax
        mov    eax, 0x801
        pop    esi
        out    dx, ax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_2800:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   1
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_2A30
        mov    edx, dword ptr [edi + 0x15c]
        mov    eax, 0x4800
        out    dx, ax
        mov    eax, 0x2000
        push   0x30d40
        out    dx, ax
        call   sub_1_5560
        mov    esi, dword ptr [edi + 0x15c]
        mov    eax, 0x804
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [edi + 0x158]
        add    edx, 0xa
        in     ax, dx
        and    ax, 0x800
        mov    edx, esi
        cmp    ax, 1
        mov    ecx, dword ptr [edi + 0x160]
        sbb    ebx, ebx
        mov    eax, 0x5800
        out    dx, ax
        inc    ebx
        mov    edx, ecx
loc_1_2865:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2865
        mov    edx, esi
        mov    eax, 0x2800
        out    dx, ax
        mov    edx, ecx
loc_1_2877:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2877
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2890:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        push   esi
        call   sub_1_2A30
loc_1_28A5:
        push   esi
        call   sub_1_28D0
        test   al, al
        jne    loc_1_28BD
        inc    edi
        cmp    edi, 3
        jb     loc_1_28A5
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_28BD:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_28D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x17c]
        mov    ecx, dword ptr [edi + 0x15c]
        mov    edx, ecx
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0x804
        out    dx, ax
        mov    edx, dword ptr [edi + 0x158]
        add    edx, 6
        in     ax, dx
        and    ax, 0xfff
        or     ah, 0x80
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0x4800
        out    dx, ax
        mov    eax, 0x2000
        out    dx, ax
        mov    eax, 0x8001
        lea    ecx, [edi + 0x3e]
        out    dx, ax
        mov    eax, 0x801
        mov    dword ptr [ebp - 8], ecx
        out    dx, ax
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 0x800e
        out    dx, eax
        push   esi
        mov    esi, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 6
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        push   esi
        mov    esi, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 6
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    edx, dword ptr [ebp - 4]
        xor    eax, eax
        out    dx, eax
        mov    esi, 0x3e8
loc_1_2978:
        push   esi
        call   sub_1_5560
        mov    edx, dword ptr [edi + 0x164]
        in     al, dx
        test   al, 0x80
        jne    loc_1_2997
        add    esi, 0x3e8
        cmp    esi, 0x59d8
        jb     loc_1_2978
loc_1_2997:
        mov    edx, dword ptr [edi + 0x15c]
        mov    eax, 0x5800
        out    dx, ax
        mov    edx, dword ptr [edi + 0x160]
loc_1_29AA:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_29AA
        xor    esi, esi
loc_1_29B3:
        mov    edx, dword ptr [edi + 0x170]
        in     ax, dx
        test   ah, 0x80
        je     loc_1_29CD
        push   0x64
        inc    esi
        call   sub_1_5560
        cmp    esi, 0xf
        jb     loc_1_29B3
loc_1_29CD:
        mov    edx, dword ptr [edi + 0x170]
        in     ax, dx
        test   ah, 0xc0
        jne    loc_1_29DC
        mov    bl, 1
loc_1_29DC:
        mov    ecx, dword ptr [edi + 0x15c]
        mov    eax, 0x2800
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [edi + 0x160]
loc_1_29F1:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_29F1
        mov    edx, ecx
        mov    eax, 0x68ff
        out    dx, ax
        mov    eax, 0x804
        out    dx, ax
        mov    edx, dword ptr [edi + 0x158]
        pop    edi
        add    edx, 6
        pop    esi
        in     ax, dx
        and    eax, 0xfff
        out    dx, ax
        mov    al, bl
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2A30:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, 0x800
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0x15c]
        mov    ecx, dword ptr [edi + 0x158]
        mov    edx, esi
        mov    ebx, dword ptr [ebp + 0xc]
        out    dx, ax
        lea    edx, [ecx + 6]
        in     ax, dx
        and    ax, 0x3fff
        cmp    ebx, 1
        je     loc_1_2A71
        cmp    ebx, 2
        jne    loc_1_2A69
        or     ah, 0xc0
        jmp    loc_1_2A71
loc_1_2A69:
        cmp    ebx, 3
        jne    loc_1_2A71
        or     ah, 0x40
loc_1_2A71:
        out    dx, ax
        mov    edx, esi
        mov    eax, 0x804
        out    dx, ax
        lea    edx, [ecx + 0xa]
        in     ax, dx
        and    ax, 0xff37
        cmp    ebx, 3
        jne    loc_1_2A8C
        or     al, 8
loc_1_2A8C:
        cmp    ebx, 1
        jne    loc_1_2A9E
        or     al, 0x40
        cmp    byte ptr [edi + 0xb3], 0
        jne    loc_1_2A9E
        or     al, 0x80
loc_1_2A9E:
        out    dx, ax
        mov    edx, esi
        cmp    ebx, 2
        jne    loc_1_2AAE
        mov    eax, 0x1000
        jmp    loc_1_2AB3
loc_1_2AAE:
        mov    eax, 0xb800
loc_1_2AB3:
        out    dx, ax
        push   0xbb8
        call   sub_1_5560
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xb8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x15c]
        mov    eax, 0x6000
        out    dx, ax
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_2B02
loc_1_2AFF:
        inc    dword ptr [ebp - 8]
loc_1_2B02:
        cmp    dword ptr [ebp - 8], 0x1388
        jae    loc_1_2B55
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb8]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_2B2A
        jmp    loc_1_2B55
loc_1_2B2A:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x168]
        in     al, dx
        and    eax, 0xff
        cmp    eax, 0xff
        jne    loc_1_2B49
        jmp    loc_1_2B55
loc_1_2B49:
        push   1
        call   sub_1_5560
        jmp    loc_1_2AFF
loc_1_2B55:
        cmp    dword ptr [ebp - 8], 0x320
        jae    loc_1_2B69
        mov    dword ptr [ebp - 8], 0x320
loc_1_2B69:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_2B78
loc_1_2B75:
        inc    dword ptr [ebp - 4]
loc_1_2B78:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 2
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_1_2BE8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb8]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_2BAB
        jmp    loc_1_2BE8
loc_1_2BAB:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x160]
        in     ax, dx
        and    eax, 0xffff
        test   al, 1
        jne    loc_1_2BD2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xb8], 1
        jmp    loc_1_2BE8
loc_1_2BD2:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x168]
        in     al, dx
        push   1
        call   sub_1_5560
        jmp    loc_1_2B75
loc_1_2BE8:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb8]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_2C08
        mov    eax, 1
        jmp    loc_1_2C0F
loc_1_2C08:
        xor    eax, eax
        jmp    loc_1_2C0F
loc_1_2C0F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_2C16:
        push   ebp
        mov    eax, 0xa
        mov    ebp, esp
        sub    esp, 0x28
        mov    word ptr [ebp - 0x28], ax
        push   esi
        mov    word ptr [ebp - 0x26], ax
        push   edi
        mov    dword ptr [ebp - 0x24], 0     ; -> loc_1_03A8
        mov    dword ptr [ebp - 0x14], 0     ; -> loc_1_03BC
        mov    dword ptr [ebp - 0xc], 0      ; -> loc_1_03CC
        mov    eax, 5
        mov    word ptr [ebp - 0x18], ax
        mov    word ptr [ebp - 0x16], ax
        mov    eax, 7
        mov    word ptr [ebp - 0x10], ax
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0xe], ax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, 4
        push   1
        mov    word ptr [ebp - 0x20], ax
        mov    edi, dword ptr [ebp + 0x10]
        mov    word ptr [ebp - 0x1e], ax
        mov    dword ptr [esi + 0x128], ecx
        lea    eax, [ebp - 0x28]
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0x1c], 0     ; -> obj1:0x03D8
        push   eax
        push   edi
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_55EA
        mov    dword ptr [esi + 0x154], 0
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_2CC8
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x128], 3
        mov    eax, dword ptr [eax + 4]
        je     loc_1_2CC2
        cmp    eax, 0x200
        jb     loc_1_2CC8
        cmp    eax, 0x3e0
        ja     loc_1_2CC8
        test   al, 0xf
        jne    loc_1_2CC8
loc_1_2CC2:
        mov    dword ptr [esi + 0x154], eax
loc_1_2CC8:
        push   1
        lea    eax, [ebp - 0x20]
        push   eax
        lea    ecx, [ebp - 4]
        push   edi
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_55EA
        mov    dword ptr [esi + 0x130], 0
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_2D29
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x128], 3
        mov    eax, dword ptr [eax + 4]
        je     loc_1_2D23
        cmp    eax, 3
        je     loc_1_2D23
        cmp    eax, 5
        je     loc_1_2D23
        cmp    eax, 7
        je     loc_1_2D23
        cmp    eax, 9
        je     loc_1_2D23
        cmp    eax, 0xa
        je     loc_1_2D23
        cmp    eax, 0xb
        je     loc_1_2D23
        cmp    eax, 0xc
        je     loc_1_2D23
        cmp    eax, 0xf
        jne    loc_1_2D29
loc_1_2D23:
        mov    dword ptr [esi + 0x130], eax
loc_1_2D29:
        cmp    dword ptr [esi + 0x128], 3
        je     loc_1_2DAE
        push   1
        lea    eax, [ebp - 0x18]
        push   eax
        lea    ecx, [ebp - 4]
        push   edi
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_55EA
        mov    dword ptr [esi + 0x120], 0
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_2D6C
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 1
        jb     loc_1_2D6C
        cmp    eax, 0xf
        ja     loc_1_2D6C
        mov    dword ptr [esi + 0x120], eax
loc_1_2D6C:
        push   1
        lea    eax, [ebp - 0x10]
        push   eax
        lea    ecx, [ebp - 4]
        push   edi
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_55EA
        mov    dword ptr [esi + 0x180], 0x110
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_2DAE
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 0x100
        jb     loc_1_2DAE
        cmp    eax, 0x1f0
        ja     loc_1_2DAE
        test   al, 0xf
        jne    loc_1_2DAE
        mov    dword ptr [esi + 0x180], eax
loc_1_2DAE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2DC0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x128], 3
        je     loc_1_2DDA
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_2DDA:
        mov    edx, dword ptr [esi + 0x154]
        in     ax, dx
        cmp    ax, 0x6d50
        mov    dword ptr [esi + 0x158], edx
        je     loc_1_2DF8
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_2DF8:
        push   8
        push   esi
        call   sub_1_36A0
        and    ax, 0xffe0
        mov    edx, dword ptr [esi + 0x158]
        add    edx, 6
        push   9
        out    dx, ax
        push   esi
        call   sub_1_36A0
        and    ax, 0xfff
        mov    edx, dword ptr [esi + 0x158]
        or     ah, 0x30
        add    edx, 8
        out    dx, ax
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
sub_1_2E30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x128]
        cmp    eax, 1
        jne    loc_1_2E80
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_3050
        test   eax, eax
        jne    loc_1_2E57
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_2E57:
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_3120
        test   eax, eax
        jne    loc_1_2E76
        mov    dword ptr [esi + 0x128], 0
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_2E76:
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_2E80:
        cmp    eax, 2
        jne    loc_1_2E94
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_2F00
        pop    esi
        pop    ebp
        ret    4
loc_1_2E94:
        test   eax, eax
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        jne    loc_1_2EC2
        call   sub_1_3120
        test   eax, eax
        jne    loc_1_2EAD
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_2EAD:
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_3050
        pop    esi
        cmp    eax, 1
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
loc_1_2EC2:
        call   sub_1_3120
        test   eax, eax
        jne    loc_1_2ED2
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_2ED2:
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_3050
        test   eax, eax
        jne    loc_1_2EE7
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_2EE7:
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_2F00
        pop    esi
        cmp    eax, 1
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_2F00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + 0x120]
        test   ebx, ebx
        je     loc_1_2F42
        cmp    byte ptr [edi + ebx + 0x13b], 0
        jne    loc_1_2F42
        push   ebx
        push   esi
        call   sub_1_2F90
        test   al, al
        je     loc_1_2F42
        xor    eax, eax
        mov    dword ptr [esi + 0x11c], ebx
        mov    byte ptr [edi + ebx + 0x13b], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2F42:
        mov    ebx, 1
loc_1_2F47:
        cmp    byte ptr [edi + ebx + 0x13b], 0
        jne    loc_1_2F5C
        push   ebx
        push   esi
        call   sub_1_2F90
        test   al, al
        jne    loc_1_2F6E
loc_1_2F5C:
        inc    ebx
        cmp    ebx, 8
        jbe    loc_1_2F47
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2F6E:
        xor    eax, eax
        mov    dword ptr [esi + 0x11c], ebx
        mov    byte ptr [edi + ebx + 0x13b], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2F90:
        push   ebp
        mov    al, 0xff
        mov    ebp, esp
        xor    edx, edx
        out    0x94, al
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    ecx, edx
        je     loc_1_2FAB
loc_1_2FA0:
        mov    eax, edx
        inc    edx
        or     al, 8
        cmp    ecx, edx
        out    0x96, al
        ja     loc_1_2FA0
loc_1_2FAB:
        mov    edx, 0x100
        in     al, dx
        mov    cl, al
        mov    edx, 0x101
        in     al, dx
        and    ax, 0xff
        and    ecx, 0xffff00ff
        shl    cx, 8
        add    cx, ax
        mov    al, cl
        mov    cl, ch
        and    eax, 0xffff00ff
        and    ecx, 0xffff00ff
        shl    ax, 8
        add    ax, cx
        cmp    ax, 0x627c
        je     loc_1_3006
        cmp    ax, 0x627d
        je     loc_1_3006
        cmp    ax, 0x61db
        je     loc_1_3006
        cmp    ax, 0x62f6
        je     loc_1_3006
        cmp    ax, 0x62f7
        je     loc_1_3006
        xor    al, al
        pop    ebp
        out    0x96, al
        ret    8
loc_1_3006:
        mov    edx, 0x102
        in     al, dx
        test   al, 1
        jne    loc_1_3018
        xor    al, al
        pop    ebp
        out    0x96, al
        ret    8
loc_1_3018:
        mov    edx, 0x104
        in     al, dx
        and    ax, 0xfc
        mov    edx, dword ptr [ebp + 8]
        add    ax, 2
        shl    ax, 8
        pop    ebp
        mov    ecx, eax
        xor    al, al
        out    0x96, al
        and    ecx, 0xffff
        mov    al, 1
        mov    dword ptr [edx + 0x154], ecx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3050:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + 0x120]
        test   ebx, ebx
        je     loc_1_309D
        cmp    byte ptr [edi + ebx + 0x12b], 0
        jne    loc_1_309D
        push   ebx
        push   esi
        call   sub_1_30F0
        test   al, al
        je     loc_1_309D
        mov    eax, ebx
        mov    dword ptr [esi + 0x11c], ebx
        shl    eax, 0xc
        mov    dword ptr [esi + 0x154], eax
        xor    eax, eax
        mov    byte ptr [edi + ebx + 0x12b], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_309D:
        mov    ebx, 1
loc_1_30A2:
        cmp    byte ptr [edi + ebx + 0x12b], 0
        jne    loc_1_30B7
        push   ebx
        push   esi
        call   sub_1_30F0
        test   al, al
        jne    loc_1_30C9
loc_1_30B7:
        inc    ebx
        cmp    ebx, 0xf
        jbe    loc_1_30A2
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_30C9:
        mov    eax, ebx
        mov    dword ptr [esi + 0x11c], ebx
        shl    eax, 0xc
        mov    dword ptr [esi + 0x154], eax
        xor    eax, eax
        mov    byte ptr [edi + ebx + 0x12b], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_30F0:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        shl    edx, 0xc
        mov    ebp, esp
        add    edx, 0xc80
        in     ax, dx
        cmp    ax, 0x6d50
        je     loc_1_310E
        xor    al, al
        pop    ebp
        ret    8
loc_1_310E:
        add    edx, 2
        pop    ebp
        in     ax, dx
        mov    cl, al
        not    cl
        and    cl, 1
        mov    al, cl
        ret    8
sub_1_3120:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 0x180]
        mov    dword ptr [edi + 0x184], eax
        cmp    byte ptr [esi + 0x5a], 0
        jne    loc_1_3149
        mov    byte ptr [esi + 0x5a], 1
        push   esi
        push   edi
        call   sub_1_3210
loc_1_3149:
        mov    ebx, dword ptr [edi + 0x154]
        test   ebx, ebx
        je     loc_1_31DE
        push   ebx
        push   esi
        push   edi
        call   sub_1_3330
        cmp    eax, 2
        jne    loc_1_3207
        push   0
        push   ebx
        push   esi
        push   edi
        call   sub_1_34B0
        test   al, al
        je     loc_1_317F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_317F:
        push   1
        push   ebx
        push   esi
        push   edi
        call   sub_1_34B0
        test   al, al
        je     loc_1_3196
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3196:
        push   0
        push   0
        push   esi
        push   edi
        call   sub_1_34B0
        test   al, al
        je     loc_1_31B4
        xor    eax, eax
        mov    dword ptr [edi + 0x154], ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_31B4:
        push   1
        push   0
        push   esi
        push   edi
        call   sub_1_34B0
        test   al, al
        je     loc_1_31D2
        xor    eax, eax
        mov    dword ptr [edi + 0x154], ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_31D2:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_31DE:
        push   0
        push   0
        push   esi
        push   edi
        call   sub_1_34B0
        test   al, al
        je     loc_1_31F6
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_31F6:
        push   1
        push   0
        push   esi
        push   edi
        call   sub_1_34B0
        cmp    al, 1
        sbb    eax, eax
        neg    eax
loc_1_3207:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_3210:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    byte ptr [ebp - 1], 0
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x184]
        mov    dword ptr [ebp - 8], eax
        cli    
        push   edi
        mov    ebx, 1
        call   sub_1_3530
        mov    edx, dword ptr [ebp - 8]
        mov    al, 0xd0
        out    dx, al
        mov    esi, dword ptr [ebp + 0xc]
        add    esi, 0x69
loc_1_3241:
        push   7
        push   edi
        call   sub_1_3570
        cmp    ax, 0x6d50
        jne    loc_1_330C
        push   0xa
        push   edi
        call   sub_1_3570
        push   0xb
        mov    byte ptr [esi + 6], ah
        push   edi
        mov    byte ptr [esi + 7], al
        call   sub_1_3570
        push   0xc
        mov    byte ptr [esi + 8], ah
        push   edi
        mov    byte ptr [esi + 9], al
        call   sub_1_3570
        push   3
        mov    byte ptr [esi + 0xa], ah
        push   edi
        mov    byte ptr [esi + 0xb], al
        mov    byte ptr [esi - 1], 1
        mov    byte ptr [esi], 0
        call   sub_1_3570
        test   al, 1
        je     loc_1_3295
        mov    byte ptr [esi], 1
        jmp    loc_1_32EF
loc_1_3295:
        push   8
        push   edi
        call   sub_1_3570
        and    ax, 0x1f
        mov    ecx, eax
        and    ecx, 0xffff
        add    ecx, 0x20
        shl    ecx, 4
        cmp    ax, 0x1f
        mov    dword ptr [esi + 2], ecx
        jne    loc_1_32BD
        mov    byte ptr [esi], 1
        jmp    loc_1_32EF
loc_1_32BD:
        push   0x10
        push   edi
        call   sub_1_3570
        test   al, 1
        je     loc_1_32EF
        push   0x13
        push   edi
        call   sub_1_3570
        test   al, 8
        jne    loc_1_32D9
        test   al, 0xc
        jne    loc_1_32EF
loc_1_32D9:
        mov    byte ptr [esi + 1], 1
        push   0x14
        push   edi
        call   sub_1_3570
        and    al, 0xf
        cmp    al, 2
        jae    loc_1_32EF
        mov    byte ptr [ebp - 1], 1
loc_1_32EF:
        lea    eax, [ebx - 0x30]
        mov    edx, dword ptr [ebp - 8]
        push   edi
        add    esi, 0xd
        out    dx, al
        xor    al, al
        inc    ebx
        out    dx, al
        call   sub_1_3530
        cmp    ebx, 8
        jb     loc_1_3241
loc_1_330C:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        sti    
        cmp    byte ptr [ebp - 1], al
        je     loc_1_3326
        mov    edx, 0x279
        mov    al, 2
        out    dx, al
        mov    edx, 0xa79
        out    dx, al
loc_1_3326:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_3330:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, 0x800
        lea    edx, [esi + 0xe]
        mov    dword ptr [edi + 0x154], esi
        out    dx, ax
        mov    edx, esi
        mov    dword ptr [edi + 0x158], esi
        in     ax, dx
        cmp    ax, 0x6d50
        je     loc_1_336D
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_336D:
        lea    edx, [esi + 2]
        in     ax, dx
        test   al, 1
        je     loc_1_3384
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3384:
        lea    edx, [esi + 0xc]
        mov    eax, 0x5555
        out    dx, ax
        in     ax, dx
        cmp    ax, 0x5555
        je     loc_1_33A4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_33A4:
        mov    eax, 0xaaaa
        out    dx, ax
        in     ax, dx
        cmp    ax, 0xaaaa
        je     loc_1_33C1
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_33C1:
        mov    ebx, 1
        push   0xa
        push   edi
        mov    byte ptr [edi + 0xba], bl
        call   sub_1_36A0
        push   0xb
        mov    byte ptr [ebp - 8], ah
        push   edi
        mov    byte ptr [ebp - 7], al
        call   sub_1_36A0
        push   0xc
        mov    byte ptr [ebp - 6], ah
        push   edi
        mov    byte ptr [ebp - 5], al
        call   sub_1_36A0
        mov    edi, dword ptr [ebp + 0xc]
        mov    byte ptr [ebp - 4], ah
        lea    edx, [edi + 0x68]
loc_1_33F9:
        cmp    byte ptr [edx], 0
        je     loc_1_3434
        mov    cl, byte ptr [ebp - 8]
        cmp    byte ptr [edx + 7], cl
        jne    loc_1_342B
        mov    cl, byte ptr [ebp - 7]
        cmp    byte ptr [edx + 8], cl
        jne    loc_1_342B
        mov    cl, byte ptr [ebp - 6]
        cmp    byte ptr [edx + 9], cl
        jne    loc_1_342B
        mov    cl, byte ptr [ebp - 5]
        cmp    byte ptr [edx + 0xa], cl
        jne    loc_1_342B
        mov    cl, byte ptr [ebp - 4]
        cmp    byte ptr [edx + 0xb], cl
        jne    loc_1_342B
        cmp    byte ptr [edx + 0xc], al
        je     loc_1_3478
loc_1_342B:
        add    edx, 0xd
        inc    ebx
        cmp    ebx, 0x10
        jb     loc_1_33F9
loc_1_3434:
        lea    ecx, [ebx + ebx*2]
        lea    ebx, [ebx + ecx*4]
        mov    ecx, 1
        mov    byte ptr [ebx + edi + 0x5b], cl
        lea    edx, [ebx + edi]
        mov    bl, byte ptr [ebp - 8]
        mov    byte ptr [edx + 0x5c], cl
        mov    cl, byte ptr [ebp - 7]
        mov    dword ptr [edx + 0x5e], esi
        mov    byte ptr [edx + 0x62], bl
        mov    bl, byte ptr [ebp - 6]
        mov    byte ptr [edx + 0x63], cl
        mov    cl, byte ptr [ebp - 5]
        pop    edi
        mov    byte ptr [edx + 0x64], bl
        mov    bl, byte ptr [ebp - 4]
        pop    esi
        mov    byte ptr [edx + 0x65], cl
        mov    byte ptr [edx + 0x66], bl
        pop    ebx
        mov    byte ptr [edx + 0x67], al
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3478:
        lea    eax, [ebx + ebx*2]
        lea    eax, [ebx + eax*4]
        add    eax, edi
        cmp    byte ptr [eax + 0x5c], 0
        je     loc_1_3494
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3494:
        pop    edi
        mov    dword ptr [eax + 0x5e], esi
        mov    byte ptr [eax + 0x5c], 1
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_34B0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    edx, 1
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x10]
        mov    bl, byte ptr [ebp + 0x14]
        lea    eax, [esi + 0x68]
loc_1_34C9:
        cmp    byte ptr [eax], 0
        je     loc_1_34EF
        cmp    byte ptr [eax + 1], 0
        jne    loc_1_34E2
        cmp    byte ptr [eax + 2], bl
        jne    loc_1_34E2
        test   edi, edi
        je     loc_1_34ED
        cmp    dword ptr [eax + 3], edi
        je     loc_1_34ED
loc_1_34E2:
        add    eax, 0xd
        inc    edx
        cmp    edx, 0x10
        jl     loc_1_34C9
        jmp    loc_1_34EF
loc_1_34ED:
        mov    cl, 1
loc_1_34EF:
        cmp    cl, 1
        jne    loc_1_351B
        lea    eax, [edx + edx*2]
        lea    eax, [edx + eax*4]
        add    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax + 0x5c], 1
        mov    ecx, dword ptr [eax + 0x5e]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 0x154], ecx
        mov    dword ptr [eax + 0x118], edx
        mov    al, 1
        ret    0x10
loc_1_351B:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3530:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [eax + 0x184]
        mov    ecx, 0xfe
        xor    al, al
        out    dx, al
        out    dx, al
        mov    ax, 0xff
loc_1_354B:
        out    dx, al
        add    ax, ax
        test   ah, 1
        je     loc_1_3556
        xor    al, 0xcf
loc_1_3556:
        mov    esi, ecx
        dec    ecx
        test   esi, esi
        jne    loc_1_354B
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3570:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        push   0x190
        xor    si, si
        mov    edi, dword ptr [eax + 0x184]
        mov    al, byte ptr [ebp + 0xc]
        mov    edx, edi
        add    al, 0x80
        out    dx, al
        call   sub_1_5560
        mov    ecx, 0x10
loc_1_3599:
        mov    edx, edi
        in     al, dx
        and    ax, 1
        dec    ecx
        lea    esi, [eax + esi*2]
        jne    loc_1_3599
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  090h
sub_1_35B0:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x0398
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        add    esi, 0xb0
        push   0
        push   0x800
        lea    ecx, [esi + 0x17c]
        push   ecx
        call   sub_1_5566
        test   eax, eax
        je     loc_1_35E8
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_35E8:
        mov    edi, dword ptr [esi + 0x17c]
        xor    eax, eax
        mov    ecx, 0x200
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0398
        lea    ebx, [esi + 0x180]
        push   eax
        push   0
        push   0x5ec
        push   ebx
        call   sub_1_5566
        test   eax, eax
        je     loc_1_3620
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_3620:
        mov    edi, dword ptr [ebx]
        xor    eax, eax
        mov    ecx, 0x17b
        rep stosd dword ptr es:[edi], eax
        pop    edi
        mov    eax, dword ptr [esi + 0x17c]
        mov    dword ptr [esi + 0x154], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [esi + 0x168], ecx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3650:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x22c]
        add    esi, 0xb0
        test   eax, eax
        je     loc_1_3674
        push   0
        push   0x800
        push   eax
        call   sub_1_556C
loc_1_3674:
        mov    eax, dword ptr [esi + 0x180]
        test   eax, eax
        je     loc_1_368B
        push   0
        push   0x5ec
        push   eax
        call   sub_1_556C
loc_1_368B:
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_36A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    si, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x158]
        mov    eax, 0x800
        and    esi, 0xffff00ff
        add    si, 0x80
        lea    edx, [ecx + 0xe]
        out    dx, ax
        mov    ax, si
        lea    edx, [ecx + 0xa]
        out    dx, ax
loc_1_36CF:
        in     ax, dx
        test   ah, 0x80
        jne    loc_1_36CF
        lea    edx, [ecx + 0xc]
        pop    esi
        in     ax, dx
        pop    ebp
        ret    8
sub_1_36E0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [ecx + 0x160]
        in     ax, dx
        cmp    ax, 0xffff
        jne    loc_1_36FB
        xor    al, al
        pop    ebp
        ret    4
loc_1_36FB:
        in     al, dx
        test   al, 1
        je     loc_1_371A
        mov    edx, dword ptr [ecx + 0x15c]
        mov    eax, 0x7000
        out    dx, ax
        mov    eax, 0x6801
        pop    ebp
        out    dx, ax
        mov    al, 1
        ret    4
loc_1_371A:
        xor    al, al
        pop    ebp
        ret    4
sub_1_3720:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x160]
        in     ax, dx
        cmp    ax, 0xffff
        je     loc_1_3808
        mov    byte ptr [esi + 0xbb], 1
        mov    edx, dword ptr [esi + 0x168]
        in     al, dx
        mov    di, ax
        and    edi, 0xffff00ff
loc_1_3751:
        mov    edx, dword ptr [esi + 0x160]
        in     ax, dx
        and    eax, 0xfe
        je     loc_1_376D
        push   esi
        and    eax, 0xfffffffe
        call   dword ptr [esi + eax*2 + 0x234]
        jmp    loc_1_3751
loc_1_376D:
        mov    dx, word ptr [esi + 0x18a]
        mov    ecx, edi
        add    dx, di
        and    ecx, 0xffff
        imul   ecx, ecx
        mov    ax, word ptr [esi + 0x188]
        add    ecx, dword ptr [esi + 0x18c]
        inc    ax
        mov    dword ptr [esi + 0x18c], ecx
        mov    word ptr [esi + 0x18a], dx
        mov    word ptr [esi + 0x188], ax
        cmp    ax, 0x100
        jb     loc_1_37C8
        mov    word ptr [esi + 0x188], 0x80
        shr    ecx, 1
        shr    dx, 1
        mov    dword ptr [esi + 0x18c], ecx
        mov    word ptr [esi + 0x18a], dx
loc_1_37C8:
        mov    byte ptr [esi + 0xbb], 0
        cmp    byte ptr [esi + 0xb1], 0
        je     loc_1_37FB
        mov    edx, dword ptr [esi + 0x15c]
        mov    eax, 0x70fe
        mov    byte ptr [esi + 0xb1], 0
        out    dx, ax
        mov    eax, dword ptr [esi + 0x5e]
        push   eax
        call   sub_1_5830
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_37FB:
        mov    edx, dword ptr [esi + 0x15c]
        mov    eax, 0x70fe
        out    dx, ax
loc_1_3808:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_3810:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0xb0], 0
        je     loc_1_382D
        mov    byte ptr [esi + 0xb0], 0
        push   esi
        call   sub_1_42C0
loc_1_382D:
        lea    ecx, [esi + 0x234]
        xor    eax, eax
loc_1_3835:
        test   al, 2
        je     loc_1_3841
        mov    dword ptr [ecx], 0            ; -> loc_1_3AA0
        jmp    loc_1_388D
loc_1_3841:
        test   al, 0x80
        je     loc_1_384D
        mov    dword ptr [ecx], 0            ; -> sub_1_38B0
        jmp    loc_1_388D
loc_1_384D:
        test   al, 0x20
        je     loc_1_3859
        mov    dword ptr [ecx], 0            ; -> loc_1_3B80
        jmp    loc_1_388D
loc_1_3859:
        test   al, 0x10
        je     loc_1_3865
        mov    dword ptr [ecx], 0            ; -> loc_1_3EB0
        jmp    loc_1_388D
loc_1_3865:
        test   al, 4
        je     loc_1_3871
        mov    dword ptr [ecx], 0            ; -> loc_1_4C90
        jmp    loc_1_388D
loc_1_3871:
        test   al, 8
        je     loc_1_387D
        mov    dword ptr [ecx], 0            ; -> loc_1_4DE0
        jmp    loc_1_388D
loc_1_387D:
        test   al, 0x40
        mov    dword ptr [ecx], 0            ; -> loc_1_3A40
        jne    loc_1_388D
        mov    dword ptr [ecx], 0            ; -> loc_1_3B70
loc_1_388D:
        add    ecx, 4
        add    eax, 2
        cmp    eax, 0x100
        jl     loc_1_3835
        mov    dword ptr [esi + 0x210], 0    ; -> loc_1_4010
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_38B0:
        push   ebp
        mov    eax, 0x806
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0x15c]
        mov    edi, dword ptr [ecx + 0x158]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0xb000
        out    dx, ax
        lea    edx, [edi + 1]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1d0], edx
        lea    edx, [edi + 2]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1d8], edx
        lea    edx, [edi + 3]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1d4], edx
        lea    edx, [edi + 5]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x200], edx
        lea    edx, [edi + 7]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1bc], edx
        lea    edx, [edi + 0xc]
        in     ax, dx
        mov    edx, eax
        and    edx, 0xffff
        add    dword ptr [ecx + 0x1c4], edx
        lea    edx, [edi + 0xa]
        in     ax, dx
        mov    edx, eax
        and    edx, 0xffff
        add    dword ptr [ecx + 0x1c8], edx
        lea    edx, [edi + 6]
        in     al, dx
        and    eax, 0xff
        add    dword ptr [ecx + 0x1b8], eax
        cmp    byte ptr [ecx + 0xb2], 0
        je     loc_1_395E
        add    dword ptr [ecx + 0x1d0], eax
loc_1_395E:
        mov    edx, esi
        mov    eax, 0x801
        cmp    dword ptr [ecx + 0xc0], 1
        out    dx, ax
        jne    loc_1_398C
        mov    edx, dword ptr [ecx + 0x164]
        in     al, dx
        test   al, 0x80
        je     loc_1_398C
        mov    eax, dword ptr [ecx + 0xc4]
        mov    edx, esi
        and    eax, 0xff7f
        out    dx, ax
        jmp    loc_1_39BF
loc_1_398C:
        mov    edx, esi
        mov    eax, 0x806
        out    dx, ax
        mov    edx, edi
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1e0], edx
        lea    edx, [edi + 4]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 0x1e4], edx
        lea    edx, [edi + 8]
        in     al, dx
        and    eax, 0xff
        add    dword ptr [ecx + 0x1e8], eax
loc_1_39BF:
        mov    edx, esi
        mov    eax, 0xa800
        out    dx, ax
        mov    eax, 0x801
        pop    edi
        out    dx, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_39E0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    dword ptr [ecx + 0xc0], 1
        push   edi
        ja     loc_1_3A32
        mov    edi, dword ptr [ecx + 0x15c]
        mov    eax, 0x806
        mov    edx, edi
        mov    esi, dword ptr [ecx + 0x158]
        out    dx, ax
        mov    eax, 0xb000
        out    dx, ax
        mov    edx, esi
        in     al, dx
        lea    edx, [esi + 4]
        in     al, dx
        lea    edx, [esi + 8]
        in     al, dx
        mov    edx, edi
        mov    eax, 0xa800
        out    dx, ax
        mov    eax, 0x801
        out    dx, ax
        mov    ax, word ptr [ecx + 0xc4]
        out    dx, ax
loc_1_3A32:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3A40:
        push   ebp
        mov    eax, 0x6840
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0xb8], 1
        mov    edx, dword ptr [esi + 0x15c]
        cmp    byte ptr [esi + 0xb9], 0
        out    dx, ax
        je     loc_1_3A8D
        push   esi
        call   sub_1_43A0
        mov    byte ptr [esi + 0xb0], 0
        mov    byte ptr [esi + 0xb9], 0
        push   esi
        call   sub_1_1E90
        mov    ax, word ptr [esi + 0xc4]
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
loc_1_3A8D:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3AA0:
        push   ebp
        mov    eax, 0x804
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0x15c]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [edi + 0x158]
        add    edx, 4
        in     ax, dx
        mov    cx, ax
        mov    edx, esi
        test   ch, 4
        mov    eax, 0x801
        out    dx, ax
        je     loc_1_3B10
        push   edi
        call   sub_1_4F80
        mov    ax, word ptr [edi + 0x19c]
        mov    edx, dword ptr [edi + 0x15c]
        cmp    dword ptr [edi + 0xf8], 0
        out    dx, ax
        je     loc_1_3B64
        mov    byte ptr [edi + 0x110], 0
        mov    eax, dword ptr [edi + 0xfc]
        mov    dword ptr [edi + 0x100], eax
        pop    edi
        sub    ax, 0x7000
        pop    esi
        out    dx, ax
        pop    ebp
        ret    4
loc_1_3B10:
        test   ch, 0x20
        je     loc_1_3B3A
        cmp    dword ptr [edi + 0x128], 2
        je     loc_1_3B3A
        cmp    dword ptr [edi + 0xc0], 1
        jne    loc_1_3B3A
        push   edi
        call   sub_1_43A0
        mov    byte ptr [edi + 0xb0], 0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3B3A:
        mov    edx, esi
        mov    eax, 0x2800
        out    dx, ax
        mov    ecx, dword ptr [edi + 0x160]
        mov    edx, ecx
loc_1_3B4B:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_3B4B
        mov    edx, esi
        mov    eax, 0x5800
        out    dx, ax
        mov    edx, ecx
loc_1_3B5D:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_3B5D
loc_1_3B64:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3B70:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3B80:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x17c]
        mov    ecx, dword ptr [esi + 0x15c]
        mov    edx, ecx
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, 0x6820
        mov    dword ptr [ebp - 0x2c], ecx
        cmp    byte ptr [esi + 0xb0], 0
        out    dx, ax
        je     loc_1_3CB3
        mov    edx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 0xd4]
        mov    ecx, dword ptr [esi + 0x208]
        mov    byte ptr [esi + 0xb0], 0
        out    dx, ax
        mov    edx, dword ptr [esi + 0x170]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [esi + 0x22c]
        mov    dword ptr [ebp - 0x24], edx
        in     ax, dx
        add    ecx, dword ptr [ebp - 8]
        and    eax, 0x7fc
        mov    ebx, eax
        mov    dword ptr [ebp - 4], ecx
loc_1_3BEF:
        add    dword ptr [ebp - 8], ebx
        mov    dword ptr [ebp - 0x1c], ebx
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x1c]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    edx, dword ptr [ebp - 0x24]
        add    dword ptr [ebp - 4], ebx
        in     ax, dx
        mov    ebx, eax
        and    ebx, 0xffff
        test   bh, 0x80
        je     loc_1_3C24
        and    ebx, 0x7fc
        jmp    loc_1_3BEF
loc_1_3C24:
        test   bh, 0x40
        je     loc_1_3C45
        and    ebx, 0x7ff
        mov    esi, dword ptr [ebp + 8]
        add    ebx, dword ptr [ebp - 8]
        push   ebx
        push   esi
        call   sub_1_4200
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3C45:
        and    ebx, 0x7ff
        lea    eax, [ebx + 3]
        mov    dword ptr [ebp - 0x34], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x34]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    esi, dword ptr [ebp + 8]
        add    dword ptr [ebp - 8], ebx
        mov    edi, dword ptr [ebp - 8]
        lea    edx, [esi + 0x204]
        mov    dword ptr [esi + 0x20c], edi
        mov    dword ptr [esi + 0x208], edi
        lea    eax, [edi - 0xe]
        mov    ecx, dword ptr [edx]
        push   eax
        sub    edi, 0xe
        push   edi
        lea    ebx, [ecx + 0xe]
        push   ebx
        mov    eax, dword ptr [esi + 0x5e]
        mov    byte ptr [esi + 0xb1], 1
        push   0xe
        push   ecx
        push   ecx
        push   edx
        push   eax
        call   sub_1_5650
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_42C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3CB3:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xbc]
        mov    ecx, dword ptr [esi + 0x22c]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [esi + 0x170]
        mov    dword ptr [ebp - 0x28], ecx
        mov    edx, eax
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [ebp - 0x20], eax
        in     ax, dx
        and    eax, 0x7fc
        cmp    dword ptr [ebp - 8], 0
        mov    ebx, eax
        je     loc_1_3D30
loc_1_3CE8:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, ebx
        jl     loc_1_3CF1
        mov    eax, ebx
loc_1_3CF1:
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x18]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x20]
        add    dword ptr [ebp - 4], eax
        sub    dword ptr [ebp - 0x10], eax
        in     ax, dx
        mov    ebx, eax
        and    ebx, 0xffff
        test   bh, 0x80
        je     loc_1_3D67
        and    ebx, 0x7fc
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_3CE8
loc_1_3D30:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x28]
        mov    dword ptr [esi + 0x208], eax
        mov    cx, word ptr [edx + 0xc]
        xor    eax, eax
        mov    al, cl
        mov    cl, ch
        shl    eax, 8
        and    ecx, 0xff
        add    eax, ecx
        cmp    eax, 0x600
        jge    loc_1_3E02
        add    eax, 0xe
        jmp    loc_1_3E4C
loc_1_3D67:
        test   bh, 0x40
        je     loc_1_3D8B
        and    ebx, 0x7ff
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, dword ptr [ebp - 0x10]
        add    ebx, dword ptr [ebp - 8]
        push   ebx
        push   esi
        call   sub_1_4200
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3D8B:
        and    ebx, 0x7ff
        lea    eax, [ebx + 3]
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x30]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    esi, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 8], eax
        lea    edx, [esi + 0x204]
        mov    byte ptr [esi + 0xb1], 1
        add    dword ptr [ebp - 8], ebx
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x208], ecx
        mov    dword ptr [esi + 0x20c], ecx
        lea    eax, [ecx - 0xe]
        mov    esi, dword ptr [edx]
        push   eax
        sub    ecx, 0xe
        push   ecx
        lea    ebx, [esi + 0xe]
        push   ebx
        push   0xe
        push   esi
        push   esi
        push   edx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x5e]
        push   eax
        call   sub_1_5650
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_42C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3E02:
        jne    loc_1_3E1F
        mov    eax, dword ptr [ebp - 0x28]
        xor    ecx, ecx
        mov    ax, word ptr [eax + 0x10]
        mov    cl, al
        mov    al, ah
        shl    ecx, 8
        and    eax, 0xff
        lea    eax, [ecx + eax + 0xe]
        jmp    loc_1_3E4C
loc_1_3E1F:
        cmp    eax, 0x800
        je     loc_1_3E32
        cmp    eax, 0x8137
        mov    eax, 0x5ea
        jne    loc_1_3E4C
loc_1_3E32:
        mov    eax, dword ptr [ebp - 0x28]
        mov    cx, word ptr [eax + 0x10]
        xor    eax, eax
        mov    al, cl
        mov    cl, ch
        shl    eax, 8
        and    ecx, 0xff
        lea    eax, [eax + ecx + 0xe]
loc_1_3E4C:
        cmp    eax, 0x5ea
        jg     loc_1_3E58
        cmp    dword ptr [ebp - 8], eax
        jle    loc_1_3E5D
loc_1_3E58:
        mov    eax, 0x5ea
loc_1_3E5D:
        mov    ecx, eax
        mov    esi, dword ptr [ebp + 8]
        sub    ecx, dword ptr [ebp - 8]
        sar    ecx, 1
        imul   ecx, dword ptr [esi + 0xd0]
        sar    ecx, 5
        sub    eax, ecx
        sub    eax, dword ptr [esi + 0x190]
        jns    loc_1_3E7E
        xor    eax, eax
loc_1_3E7E:
        sub    ax, 0x7800
        mov    esi, dword ptr [ebp + 8]
        and    eax, 0xffff
        mov    edx, dword ptr [ebp - 0x2c]
        mov    byte ptr [esi + 0xb0], 1
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3EB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x17c]
        mov    edx, dword ptr [ebx + 0x170]
        mov    dword ptr [ebp - 0x10], eax
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        test   ch, 0x40
        mov    dword ptr [ebp - 4], ecx
        je     loc_1_3F1F
        and    dword ptr [ebp - 4], 0x7ff
        cmp    byte ptr [ebx + 0xb0], 0
        je     loc_1_3EF6
        mov    eax, dword ptr [ebx + 0x208]
        add    dword ptr [ebp - 4], eax
loc_1_3EF6:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ebx
        call   sub_1_4200
        mov    ax, word ptr [ebx + 0xd4]
        mov    edx, dword ptr [ebx + 0x15c]
        mov    byte ptr [ebx + 0xb0], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
loc_1_3F1F:
        and    dword ptr [ebp - 4], 0x7ff
        cmp    byte ptr [ebx + 0xb0], 0
        je     loc_1_3FA2
        mov    ax, word ptr [ebx + 0xd4]
        mov    edx, dword ptr [ebx + 0x15c]
        mov    byte ptr [ebx + 0xb0], 0
        out    dx, ax
        mov    eax, dword ptr [ebx + 0x208]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebx + 0x22c]
        mov    byte ptr [ebx + 0xb1], 1
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        add    dword ptr [ebp - 8], ecx
        mov    edx, dword ptr [ebp - 8]
        lea    eax, [ecx + 3]
        mov    dword ptr [ebx + 0x20c], edx
        mov    dword ptr [ebx + 0x208], edx
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x18]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        lea    edx, [ebx + 0x204]
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0xe
        mov    ecx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_3FEB
loc_1_3FA2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebx + 0x22c]
        mov    dword ptr [ebx + 0x20c], eax
        mov    dword ptr [ebx + 0x208], eax
        add    eax, 3
        mov    dword ptr [ebp - 0xc], ecx
        mov    byte ptr [ebx + 0xb1], 1
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x14]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        lea    edx, [ebx + 0x204]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xe
        mov    ecx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [ebp - 4]
loc_1_3FEB:
        sub    eax, 0xe
        push   eax
        lea    eax, [ecx + 0xe]
        push   eax
        push   0xe
        push   ecx
        push   ecx
        push   edx
        mov    ecx, dword ptr [ebx + 0x5e]
        push   ecx
        call   sub_1_5650
        push   ebx
        call   sub_1_42C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4010:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [eax + 0x22c]
        mov    cx, word ptr [esi + 0x2c]
        mov    eax, dword ptr [ebp + 0xc]
        add    edi, ecx
        mov    ebx, dword ptr [eax + 8]
        sub    ebx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 0x2e]
        cmp    ebx, eax
        jbe    loc_1_4041
        mov    ebx, eax
loc_1_4041:
        mov    eax, dword ptr [esi + 8]
        lea    ecx, [ebp - 4]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        mov    dword ptr [esi + 0x30], ebx
        push   eax
        mov    dword ptr [ebp - 8], eax
        call   sub_1_55B4
        test   ebx, ebx
        je     loc_1_40C1
loc_1_405C:
        cmp    dword ptr [ebp - 4], ebx
        jae    loc_1_40A8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   edi
        push   ecx
        call   sub_1_5940
        sub    ebx, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_55D8
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_40BE
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_55B4
        test   ebx, ebx
        jne    loc_1_405C
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_40A8:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   edi
        push   eax
        call   sub_1_5940
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_40BE:
        sub    dword ptr [esi + 0x30], ebx
loc_1_40C1:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_1_40D0:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [eax + 0x2c]
        mov    eax, dword ptr [esi]
        add    eax, 0xf
        mov    dword ptr [esi + 0x220], edi
        and    eax, 0xfffffff0
        cmp    eax, edi
        jae    loc_1_40FA
        mov    edi, eax
loc_1_40FA:
        mov    eax, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 4]
        push   edx
        mov    dword ptr [esi + 0x21c], edi
        mov    ecx, dword ptr [eax + 8]
        lea    eax, [ebp - 0xc]
        push   eax
        mov    dword ptr [ebp - 8], ecx
        push   ecx
        call   sub_1_55B4
        mov    ebx, dword ptr [esi + 0x230]
loc_1_411D:
        cmp    edi, dword ptr [ebp - 4]
        jb     loc_1_415C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        push   ebx
        call   sub_1_5940
        sub    edi, dword ptr [ebp - 4]
        add    ebx, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_55D8
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_4167
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_55B4
        jmp    loc_1_411D
loc_1_415C:
        push   edi
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   ebx
        call   sub_1_5940
loc_1_4167:
        mov    eax, dword ptr [ebp + 0xc]
        lea    edx, [esi + 0x218]
        mov    dword ptr [esi + 0x228], eax
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [esi + 0x220]
        lea    ebx, [ecx + 0xe]
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [esi + 0x21c]
        sub    eax, 0xe
        push   eax
        push   ebx
        push   0xe
        push   ecx
        push   ecx
        push   edx
        mov    ecx, dword ptr [esi + 0x5e]
        push   ecx
        call   sub_1_5650
        cmp    byte ptr [esi + 0xbb], 0
        je     loc_1_41B7
        mov    byte ptr [esi + 0xb1], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_41B7:
        mov    eax, dword ptr [esi + 0x5e]
        push   eax
        call   sub_1_5830
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_41D0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0x10]
        mov    dx, word ptr [ecx + 0x2c]
        lea    eax, [ecx + 0x30]
        push   eax
        push   edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x10]
        xor    eax, eax
        mov    ax, word ptr [ecx + 0x2e]
        push   edx
        push   eax
        push   0
        push   ecx
        call   sub_1_55DE
        xor    eax, eax
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_4200:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [ecx + 0x170]
        in     ax, dx
        and    eax, 0x3800
        cmp    eax, 0x1800
        ja     loc_1_4226
        je     loc_1_423E
        cmp    eax, 0x800
        je     loc_1_4236
        jmp    loc_1_4254
loc_1_4226:
        cmp    eax, 0x2000
        je     loc_1_4246
        cmp    eax, 0x2800
        je     loc_1_424E
        jmp    loc_1_4254
loc_1_4236:
        inc    dword ptr [ecx + 0x1fc]
        jmp    loc_1_4254
loc_1_423E:
        inc    dword ptr [ecx + 0xec]
        jmp    loc_1_4254
loc_1_4246:
        inc    dword ptr [ecx + 0x1f4]
        jmp    loc_1_4254
loc_1_424E:
        inc    dword ptr [ecx + 0x1f8]
loc_1_4254:
        test   eax, eax
        je     loc_1_426E
        cmp    eax, 0x1800
        je     loc_1_426E
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [ecx + 0x1bc]
        sub    dword ptr [ecx + 0x1c8], eax
loc_1_426E:
        push   ecx
        call   sub_1_42C0
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4280:
        push   ebp
        mov    cl, 0xf5
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 0xbc]
        sub    cl, byte ptr [edx + 0x190]
        shr    esi, 1
        mov    eax, dword ptr [edx + 0xd8]
        imul   esi, dword ptr [edx + 0xd0]
        shr    esi, cl
        cmp    esi, eax
        jge    loc_1_42AD
        mov    esi, eax
loc_1_42AD:
        add    esi, 0x8800
        mov    dword ptr [edx + 0xd4], esi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_42C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, 0x4000
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x15c]
        mov    edx, edi
        out    dx, ax
        mov    edx, dword ptr [esi + 0x160]
loc_1_42DE:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_42DE
        cmp    dword ptr [esi + 0xc0], 1
        ja     loc_1_438B
        mov    edx, dword ptr [esi + 0x170]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        test   ch, 0x80
        je     loc_1_438B
        mov    edx, edi
        mov    eax, 0x803
        out    dx, ax
        mov    edx, dword ptr [esi + 0x174]
        and    ecx, 0x7ff
        in     ax, dx
        mov    ebx, eax
        mov    edx, edi
        and    ebx, 0xffff
        mov    eax, 0x801
        out    dx, ax
        add    ecx, ebx
        add    ecx, 0x64
        cmp    ecx, dword ptr [esi + 0xdc]
        jae    loc_1_438B
        mov    edx, dword ptr [esi + 0x170]
        in     ax, dx
        mov    ecx, eax
        mov    edx, edi
        and    ecx, 0xffff
        mov    eax, 0x803
        out    dx, ax
        mov    edx, dword ptr [esi + 0x174]
        and    ecx, 0x7ff
        in     ax, dx
        mov    ebx, eax
        mov    edx, edi
        and    ebx, 0xffff
        mov    eax, 0x801
        out    dx, ax
        add    ecx, ebx
        add    ecx, 0x64
        cmp    ecx, dword ptr [esi + 0xdc]
        jae    loc_1_438B
        push   esi
        call   sub_1_43A0
loc_1_438B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_43A0:
        push   ebp
        mov    eax, 0x2800
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0x15c]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x160]
loc_1_43BC:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_43BC
        mov    ax, word ptr [ecx + 0xf0]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0x2000
        pop    esi
        out    dx, ax
        mov    ax, word ptr [ecx + 0xd4]
        pop    ebp
        out    dx, ax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_43F0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        mov    ecx, dword ptr [eax + 0x17c]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   edi
        mov    dword ptr [ebp - 0x18], ecx
        xor    edi, edi
        mov    eax, dword ptr [edx + 8]
        mov    di, word ptr [edx + 0x2c]
        lea    ecx, [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        mov    dword ptr [ebp - 0x14], eax
        push   eax
        call   sub_1_55B4
        cmp    dword ptr [ebp - 8], 6
        jl     loc_1_447B
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0x82], 0
        jne    loc_1_444F
        mov    eax, dword ptr [ebp - 0xc]
        test   byte ptr [eax], 1
        jne    loc_1_444F
        mov    ecx, eax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x40]
        mov    eax, 0
        cmp    dword ptr [ecx + 2], edx
        jne    loc_1_4461
loc_1_444F:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_1FC0
        and    eax, 0xff
loc_1_4461:
        test   eax, eax
        je     loc_1_447B
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        inc    dword ptr [eax + 0x1c0]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_40D0
loc_1_447B:
        mov    eax, dword ptr [ebp + 8]
        and    dword ptr [eax + 0xc4], 0xfffffffb
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0xc4]
        and    eax, 0xffff
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x16c]
        in     ax, dx
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x10], eax
        cmp    eax, 8
        jge    loc_1_4513
        mov    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xf8], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xfc], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x100], edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 0
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [edi - 0x7000]
        mov    edx, dword ptr [ecx + 0x15c]
        or     dword ptr [ecx + 0xc4], 4
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0xc4]
        mov    esp, ebp
        out    dx, ax
        mov    eax, 1
        pop    ebp
        ret    8
loc_1_4513:
        cli    
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, edi
        out    dx, eax
        mov    ebx, edi
        mov    eax, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 0x10], 4
        mov    eax, dword ptr [eax + 0xcc]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    eax, ecx
        jl     loc_1_4532
loc_1_4530:
        mov    eax, ecx
loc_1_4532:
        mov    dword ptr [ebp - 4], eax
loc_1_4535:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jl     loc_1_4581
        sub    ebx, eax
        sub    dword ptr [ebp - 4], eax
        push   esi
        mov    esi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        lea    eax, [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        push   ecx
        call   sub_1_55D8
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_45E0
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        push   edx
        call   sub_1_55B4
        jmp    loc_1_4535
loc_1_4581:
        push   esi
        mov    esi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 0xc], eax
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x16c]
        in     ax, dx
        and    eax, 0xffff
        sub    eax, 4
        mov    dword ptr [ebp - 0x10], eax
        cli    
        cmp    dword ptr [ebp - 0x10], 8
        jl     loc_1_46B7
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0xcc]
        cmp    eax, ecx
        jl     loc_1_4532
        jmp    loc_1_4530
loc_1_45E0:
        neg    edi
        and    edi, 3
        mov    dword ptr [ebp - 0x10], edi
        je     loc_1_45F9
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x10]
        rep outsb dx, byte ptr [esi]
        mov    esi, eax
loc_1_45F9:
        sti    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 0x804
        mov    edx, dword ptr [esi + 0x15c]
        add    esi, 0x15c
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x178]
        in     ax, dx
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, 0x801
        mov    edx, dword ptr [esi]
        test   byte ptr [ebp - 0xf], 0x20
        out    dx, ax
        je     loc_1_4690
        inc    dword ptr [ecx + 0x1cc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x164]
        mov    edx, ecx
loc_1_4643:
        in     al, dx
        test   al, 0x80
        je     loc_1_4643
        mov    edx, ecx
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4F80
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_39E0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x19c
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4E60
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   edx
        call   sub_1_4750
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0x19c]
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
loc_1_4690:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        or     dword ptr [eax + 0xc4], 4
        mov    eax, dword ptr [ebp + 8]
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0xc4]
        mov    esp, ebp
        out    dx, ax
        xor    eax, eax
        pop    ebp
        ret    8
loc_1_46B7:
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0x1aa]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xf8], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xfc], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x100], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x14]
        mov    dword ptr [eax + 0x104], edx
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x108], esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x10c], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 1
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ebx - 0x7000]
        mov    edx, dword ptr [ecx + 0x15c]
        or     dword ptr [ecx + 0xc4], 4
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0xc4]
        mov    esp, ebp
        out    dx, ax
        mov    eax, 1
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_4750:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        mov    ecx, dword ptr [eax + 0x17c]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   edi
        mov    dword ptr [ebp - 0x18], ecx
        xor    edi, edi
        mov    di, word ptr [edx + 0x2c]
        mov    edx, dword ptr [eax + 0x16c]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        mov    dword ptr [ebp - 0xc], ecx
        cmp    ecx, 8
        jge    loc_1_47CE
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xf8], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xfc], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x100], edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 0
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [edi - 0x7000]
        pop    edi
        pop    esi
        mov    edx, dword ptr [ecx + 0x15c]
        pop    ebx
        out    dx, ax
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_47CE:
        cli    
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, edi
        out    dx, eax
        mov    ebx, edi
        mov    eax, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 0xc], 4
        mov    eax, dword ptr [eax + 0xcc]
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    eax, ecx
        jl     loc_1_47ED
        mov    eax, ecx
loc_1_47ED:
        mov    ecx, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 8]
        push   edx
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ecx + 8]
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    dword ptr [ebp - 0x10], eax
        push   eax
loc_1_4802:
        call   sub_1_55B4
loc_1_4807:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jl     loc_1_484E
        sub    ebx, eax
        sub    dword ptr [ebp - 4], eax
        push   esi
        mov    esi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        lea    eax, [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        push   ecx
        call   sub_1_55D8
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_48AE
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0x10]
        push   ecx
        push   edx
        jmp    loc_1_4802
loc_1_484E:
        push   esi
        mov    esi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 0x14], eax
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x16c]
        in     ax, dx
        and    eax, 0xffff
        sub    eax, 4
        mov    dword ptr [ebp - 0xc], eax
        cli    
        cmp    dword ptr [ebp - 0xc], 8
        mov    eax, dword ptr [ebp + 8]
        jl     loc_1_4969
        mov    eax, dword ptr [eax + 0xcc]
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    eax, ecx
        jl     loc_1_48A6
        mov    eax, ecx
loc_1_48A6:
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_4807
loc_1_48AE:
        neg    edi
        and    edi, 3
        mov    dword ptr [ebp - 0xc], edi
        je     loc_1_48C7
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        rep outsb dx, byte ptr [esi]
        mov    esi, eax
loc_1_48C7:
        sti    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 0x804
        mov    edx, dword ptr [esi + 0x15c]
        add    esi, 0x15c
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x178]
        in     ax, dx
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, 0x801
        mov    edx, dword ptr [esi]
        test   byte ptr [ebp - 0xb], 0x20
        out    dx, ax
        je     loc_1_495E
        inc    dword ptr [ecx + 0x1cc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x164]
        mov    edx, ecx
loc_1_4911:
        in     al, dx
        test   al, 0x80
        je     loc_1_4911
        mov    edx, ecx
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4F80
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_39E0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x19c
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4E60
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   edx
        call   sub_1_4750
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0x19c]
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
loc_1_495E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4969:
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0x1aa]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xf8], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xfc], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x100], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x104], edx
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp - 0x14]
        mov    dword ptr [eax + 0x108], esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x10c], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 1
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ebx - 0x7000]
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
        sti    
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_49F0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        mov    ecx, dword ptr [eax + 0x17c]
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x18], ecx
        mov    ecx, dword ptr [eax + 0xf8]
        cli    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x16c]
        in     ax, dx
        mov    edx, eax
        mov    eax, dword ptr [ebp + 8]
        and    edx, 0xffff
        mov    dword ptr [ebp - 0x10], edx
        cmp    byte ptr [eax + 0x110], 0
        jne    loc_1_4A7C
        cmp    edx, 8
        jge    loc_1_4A55
        mov    edx, dword ptr [eax + 0x15c]
        mov    ecx, eax
        mov    eax, dword ptr [eax + 0xfc]
        sub    ax, 0x7000
        out    dx, ax
        sti    
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4A55:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0xfc]
        out    dx, eax
        lea    edx, [ebp - 8]
        lea    eax, [ebp - 0x14]
        sub    dword ptr [ebp - 0x10], 4
        mov    ecx, dword ptr [ecx + 8]
        push   edx
        push   eax
        mov    dword ptr [ebp - 0xc], ecx
        push   ecx
        call   sub_1_55B4
        jmp    loc_1_4A9A
loc_1_4A7C:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x104]
        mov    dword ptr [ebp - 0xc], ecx
        mov    edx, dword ptr [eax + 0x108]
        mov    dword ptr [ebp - 0x14], edx
        mov    ecx, dword ptr [eax + 0x10c]
        mov    dword ptr [ebp - 8], ecx
loc_1_4A9A:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edi, dword ptr [eax + 0x100]
        mov    eax, dword ptr [eax + 0xcc]
        cmp    eax, ecx
        jl     loc_1_4AB2
loc_1_4AB0:
        mov    eax, ecx
loc_1_4AB2:
        mov    dword ptr [ebp - 4], eax
loc_1_4AB5:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jl     loc_1_4B04
        push   esi
        mov    esi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [ebp - 0xc]
        sub    edi, eax
        push   ecx
        sub    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_55D8
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_4B63
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_55B4
        jmp    loc_1_4AB5
loc_1_4B04:
        push   esi
        mov    esi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 4]
        sub    edi, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 0x14], eax
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x16c]
        in     ax, dx
        and    eax, 0xffff
        sub    eax, 4
        mov    dword ptr [ebp - 0x10], eax
        cli    
        cmp    dword ptr [ebp - 0x10], 8
        mov    eax, dword ptr [ebp + 8]
        jl     loc_1_4C29
        mov    eax, dword ptr [eax + 0xcc]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    eax, ecx
        jl     loc_1_4AB2
        jmp    loc_1_4AB0
loc_1_4B63:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfc]
        neg    eax
        and    eax, 3
        mov    dword ptr [ebp - 0x10], eax
        je     loc_1_4B85
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x10]
        rep outsb dx, byte ptr [esi]
        mov    esi, eax
loc_1_4B85:
        sti    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 0x804
        mov    edx, dword ptr [esi + 0x15c]
        add    esi, 0x15c
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x178]
        in     ax, dx
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, 0x801
        mov    edx, dword ptr [esi]
        test   byte ptr [ebp - 0xf], 0x20
        out    dx, ax
        je     loc_1_4C1F
        inc    dword ptr [ecx + 0x1cc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x164]
        mov    edx, ecx
loc_1_4BCF:
        in     al, dx
        test   al, 0x80
        je     loc_1_4BCF
        mov    edx, ecx
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4F80
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_39E0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x19c
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4E60
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0xf8]
        push   edx
        push   ecx
        call   sub_1_4750
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0x19c]
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
loc_1_4C1F:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4C29:
        mov    edx, dword ptr [eax + 0x15c]
        mov    ax, word ptr [eax + 0x1aa]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x100], edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x104], edx
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp - 0x14]
        mov    dword ptr [eax + 0x108], esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x10c], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 1
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [edi - 0x7000]
        mov    edx, dword ptr [ecx + 0x15c]
        out    dx, ax
        sti    
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_4C90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x164]
        in     al, dx
        out    dx, al
        test   al, 0x10
        je     loc_1_4D10
        push   esi
        inc    dword ptr [esi + 0x1cc]
        call   sub_1_4F80
        push   esi
        call   sub_1_39E0
        cmp    dword ptr [esi + 0xf8], 0
        je     loc_1_4CF2
        lea    eax, [esi + 0x1aa]
        push   eax
        push   esi
        call   sub_1_4E60
        mov    eax, dword ptr [esi + 0xf8]
        push   eax
        push   esi
        call   sub_1_4750
        mov    eax, dword ptr [esi + 0xf8]
        push   eax
        push   esi
        call   sub_1_1A90
        mov    dword ptr [esi + 0xf8], 0
        jmp    loc_1_4CFF
loc_1_4CF2:
        lea    eax, [esi + 0x19c]
        push   eax
        push   esi
        call   sub_1_4E60
loc_1_4CFF:
        mov    ax, word ptr [esi + 0x19c]
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
        jmp    loc_1_4D61
loc_1_4D10:
        test   al, 0x20
        je     loc_1_4D98
        push   esi
        inc    dword ptr [esi + 0x1ec]
        call   sub_1_4F80
        mov    ax, word ptr [esi + 0x19c]
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
        push   esi
        call   sub_1_39E0
        mov    eax, dword ptr [esi + 0xf8]
        test   eax, eax
        je     loc_1_4D61
        push   eax
        push   esi
        call   sub_1_4750
        mov    eax, dword ptr [esi + 0xf8]
        push   eax
        push   esi
        call   sub_1_1A90
        mov    dword ptr [esi + 0xf8], 0
loc_1_4D61:
        mov    edi, dword ptr [esi + 0x15]
        test   edi, edi
        jne    loc_1_4D70
        and    byte ptr [esi + 0x21], 0xf7
        test   edi, edi
        je     loc_1_4DD4
loc_1_4D70:
        push   edi
        push   esi
        call   sub_1_4750
        cmp    eax, 1
        je     loc_1_4DD4
        push   edi
        push   esi
        call   sub_1_1A90
        mov    edi, dword ptr [esi + 0x15]
        test   edi, edi
        jne    loc_1_4D70
        and    byte ptr [esi + 0x21], 0xf7
        test   edi, edi
        jne    loc_1_4D70
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_4D98:
        test   al, 8
        je     loc_1_4DBB
        push   esi
        inc    dword ptr [esi + 0x1dc]
        call   sub_1_39E0
        mov    edx, dword ptr [esi + 0x15c]
        mov    eax, 0x4800
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_4DBB:
        push   esi
        inc    dword ptr [esi + 0x1f0]
        call   sub_1_39E0
        mov    edx, dword ptr [esi + 0x15c]
        mov    eax, 0x4800
        out    dx, ax
loc_1_4DD4:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_4DE0:
        push   ebp
        mov    eax, 0x6808
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        mov    edx, dword ptr [edi + 0x15c]
        out    dx, ax
        mov    ax, word ptr [edi + 0x19c]
        out    dx, ax
        call   sub_1_49F0
        cmp    eax, 1
        je     loc_1_4E51
        mov    eax, dword ptr [edi + 0xf8]
        push   eax
        push   edi
        call   sub_1_1A90
        mov    dword ptr [edi + 0xf8], 0
        mov    esi, dword ptr [edi + 0x15]
        test   esi, esi
        jne    loc_1_4E2F
        and    byte ptr [edi + 0x21], 0xf7
        test   esi, esi
        je     loc_1_4E51
loc_1_4E2F:
        push   esi
        push   edi
        call   sub_1_4750
        cmp    eax, 1
        je     loc_1_4E51
        push   esi
        push   edi
        call   sub_1_1A90
        mov    esi, dword ptr [edi + 0x15]
        test   esi, esi
        jne    loc_1_4E2F
        and    byte ptr [edi + 0x21], 0xf7
        test   esi, esi
        jne    loc_1_4E2F
loc_1_4E51:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4E60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4F30
        mov    dx, ax
        mov    ecx, dword ptr [ebp + 0xc]
        sub    dx, word ptr [ecx + 8]
        mov    word ptr [ecx + 8], ax
        mov    edi, edx
        mov    eax, dword ptr [esi + 0x198]
        and    edi, 0xffff
        cmp    edi, eax
        jbe    loc_1_4E92
        mov    dx, ax
loc_1_4E92:
        mov    di, word ptr [ecx + 0xa]
        inc    di
        mov    ax, word ptr [ecx + 0xc]
        add    ax, dx
        mov    word ptr [ecx + 0xa], di
        mov    word ptr [ecx + 0xc], ax
        cmp    di, 4
        jb     loc_1_4EBD
        shr    di, 1
        shr    ax, 1
        mov    word ptr [ecx + 0xa], di
        mov    word ptr [ecx + 0xc], ax
loc_1_4EBD:
        mov    ax, word ptr [ecx + 0xc]
        mov    edi, dword ptr [ecx]
        sub    dx, dx
        and    edi, 0x7ff
        div    word ptr [ecx + 0xa]
        mov    esi, dword ptr [esi + 0x194]
        and    eax, 0xffff
        mov    ebx, esi
        sub    ebx, eax
        imul   ebx, edi
        mov    eax, ebx
        cdq    
        idiv   esi
        mov    esi, eax
        test   esi, esi
        jl     loc_1_4F04
        cmp    edi, esi
        jge    loc_1_4EF3
        mov    esi, edi
loc_1_4EF3:
        add    edi, esi
        cmp    edi, 0x5ea
        jle    loc_1_4F1F
        mov    edi, 0x5ea
        jmp    loc_1_4F1F
loc_1_4F04:
        neg    esi
        mov    eax, edi
        cdq    
        sub    eax, edx
        sar    eax, 1
        cmp    eax, esi
        jge    loc_1_4F14
        mov    esi, eax
loc_1_4F14:
        sub    edi, esi
        mov    eax, dword ptr [ecx + 4]
        cmp    edi, eax
        jge    loc_1_4F1F
        mov    edi, eax
loc_1_4F1F:
        add    edi, 0x9800
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_4F30:
        push   ebp
        mov    eax, 0x806
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0x15c]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0xb000
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x158]
        add    edx, 6
        in     al, dx
        and    eax, 0xff
        mov    edx, esi
        pop    esi
        add    dword ptr [ecx + 0x1b8], eax
        mov    eax, 0xa800
        pop    ebp
        out    dx, ax
        mov    eax, 0x801
        out    dx, ax
        mov    eax, dword ptr [ecx + 0x1b8]
        ret    4
        db  08Dh, 049h, 000h
sub_1_4F80:
        push   ebp
        mov    eax, 0x5800
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x15c]
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 0x160]
loc_1_4F9C:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_4F9C
        mov    edx, ecx
        mov    eax, 0x4800
        out    dx, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        add    eax, dword ptr [esi + 0xe4]
        mov    dword ptr [esi + 0xe4], eax
        cmp    eax, 0x3e8
        jbe    loc_1_4FF2
        mov    dword ptr [esi + 0xe4], 0
        push   esi
        call   sub_1_38B0
loc_1_4FF2:
        cmp    byte ptr [esi + 0xb2], 0
        jne    loc_1_5044
        mov    eax, dword ptr [esi + 0x1d0]
        cmp    eax, 0x3e8
        jbe    loc_1_5044
        shl    eax, 2
        cmp    dword ptr [esi + 0x1b8], eax
        jae    loc_1_5044
        mov    ecx, dword ptr [esi + 0x15c]
        mov    eax, 0x804
        mov    byte ptr [esi + 0xb2], 1
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 0x158]
        add    edx, 0xa
        in     ax, dx
        and    eax, 0xfff7
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0x801
        out    dx, ax
loc_1_5044:
        mov    cx, word ptr [esi + 0x188]
        mov    ax, word ptr [esi + 0x18a]
        mov    ebx, ecx
        sub    dx, dx
        and    ebx, 0xffff
        div    cx
        mov    word ptr [ebp - 4], ax
        sub    edx, edx
        mov    cx, word ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x18c]
        div    ebx
        imul   cx, cx
        sub    ax, cx
        mov    edx, dword ptr [esi + 0x4a]
        mov    cx, ax
        shr    ax, 0xc
        mov    edi, eax
        and    edi, 0xffff
        mov    bl, byte ptr [edi + edx + 0x144]
        and    ebx, 0xffff00ff
        test   bx, bx
        jne    loc_1_50A8
        mov    bx, cx
        shr    bx, 5
        add    bx, 2
loc_1_50A8:
        mov    ax, cx
        sub    dx, dx
        div    bx
        mov    edi, eax
        mov    eax, ebx
        sub    dx, dx
        and    edi, 0xffff
        and    eax, 0xffff
        add    edi, eax
        sar    edi, 1
        mov    ax, cx
        div    di
        mov    ecx, eax
        mov    eax, edi
        and    ecx, 0xffff
        and    eax, 0xffff
        add    ecx, eax
        mov    eax, dword ptr [ebp - 4]
        sar    ecx, 1
        and    eax, 0xffff
        mov    edx, ecx
        and    edx, 0xffff
        lea    ecx, [eax + edx*2]
        mov    eax, dword ptr [esi + 4]
        shl    ecx, 2
        add    eax, dword ptr [esi + 0xe8]
        mov    dword ptr [esi + 0x190], ecx
        mov    dword ptr [esi + 0xe8], eax
        cmp    eax, 0x3e8
        jbe    loc_1_5138
        mov    dword ptr [esi + 0xe8], 0
        cmp    dword ptr [esi + 0xec], 0x64
        jb     loc_1_512E
        add    dword ptr [esi + 0xd8], 8
loc_1_512E:
        mov    dword ptr [esi + 0xec], 0
loc_1_5138:
        push   esi
        call   sub_1_4280
        cmp    byte ptr [esi + 0xb0], 0
        jne    loc_1_5156
        mov    ax, word ptr [esi + 0xd4]
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
loc_1_5156:
        push   esi
        xor    edi, edi
        call   sub_1_4F30
        and    eax, 0xffff
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
loc_1_5169:
        test   edi, edi
        je     loc_1_5174
        cmp    edi, 1
        je     loc_1_517C
        jmp    loc_1_5182
loc_1_5174:
        lea    ebx, [esi + 0x19c]
        jmp    loc_1_5182
loc_1_517C:
        lea    ebx, [esi + 0x1aa]
loc_1_5182:
        mov    eax, dword ptr [ebp - 4]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 8]
        sub    eax, ecx
        cmp    eax, dword ptr [esi + 0x198]
        jl     loc_1_519C
        push   ebx
        push   esi
        call   sub_1_4E60
loc_1_519C:
        inc    edi
        cmp    edi, 2
        jl     loc_1_5169
        cmp    dword ptr [esi + 0xf8], 0
        jne    loc_1_51BA
        mov    ax, word ptr [esi + 0x19c]
        mov    edx, dword ptr [esi + 0x15c]
        out    dx, ax
loc_1_51BA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_51D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0x56], 1
        jne    loc_1_51F1
        mov    edx, dword ptr [eax + 0x15c]
        mov    eax, 0x2000
        out    dx, ax
        mov    eax, 0x4800
        out    dx, ax
loc_1_51F1:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5200:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0x56], 0
        jne    loc_1_521A
        mov    edx, dword ptr [eax + 0x15c]
        mov    eax, 0x1800
        out    dx, ax
loc_1_521A:
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 055h, 0B8h, 000h, 058h, 000h, 000h, 08Bh, 0ECh, 056h, 08Bh, 04Dh, 008h, 08Bh, 0B1h
        db  05Ch, 001h, 000h, 000h, 08Bh, 089h, 060h, 001h, 000h, 000h, 08Bh, 0D6h, 066h, 0EFh, 08Bh, 0D1h
        db  066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 028h, 000h, 000h, 066h, 0EFh
        db  08Bh, 0D1h, 066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 078h, 000h, 000h
        db  066h, 0EFh, 0B8h, 0FFh, 068h, 000h, 000h, 05Eh, 066h, 0EFh, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h
        db  024h, 000h
sub_1_5270:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_38B0
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5290:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_52A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0xb9], 1
        call   sub_1_4F80
        mov    ax, word ptr [esi + 0x19c]
        mov    edx, dword ptr [esi + 0x15c]
        mov    dword ptr [esi + 0xf8], 0
        out    dx, ax
        mov    eax, 0x1800
        pop    esi
        out    dx, ax
        mov    eax, 0x7840
        pop    ebp
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_52F0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_36E0
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5310:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   eax
        call   sub_1_3720
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        add    eax, 0xac
        push   edi
        mov    esi, dword ptr [ebp + 0x14]
        push   esi
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5596
        test   esi, esi
        je     loc_1_537D
        mov    ebx, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0xc]
loc_1_5359:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        add    edi, 4
        mov    eax, dword ptr [edx + 0xac]
        lea    ecx, [ebp - 4]
        push   eax
        push   ebx
        push   ecx
        add    ebx, 4
        call   sub_1_55A2
        dec    esi
        jne    loc_1_5359
loc_1_537D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 0B9h, 000h, 000h, 000h, 000h
        db  08Bh, 0ECh, 056h, 08Bh, 075h, 00Ch, 08Bh, 046h, 00Ch, 085h, 0C0h, 074h, 010h, 08Bh, 056h, 004h
        db  08Dh, 00Ch, 010h, 0C1h, 0EAh, 00Ch, 049h, 0C1h, 0E9h, 00Ch, 02Bh, 0CAh, 041h, 08Bh, 045h, 018h
        db  039h, 008h, 072h, 010h, 050h, 08Bh, 045h, 014h, 050h, 056h, 0E8h, 0F5h, 001h, 000h, 000h, 05Eh
        db  05Dh, 0C2h, 014h, 000h, 05Eh, 089h, 008h, 05Dh, 0C2h, 014h, 000h, 08Dh, 064h, 024h, 000h, 08Dh
        db  064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 00Ch, 08Bh
        db  0ECh, 050h, 0E8h, 0C1h, 001h, 000h, 000h, 08Bh, 045h, 008h, 08Bh, 088h, 0ACh, 000h, 000h, 000h
        db  051h, 0E8h, 0A0h, 001h, 000h, 000h, 05Dh, 0C2h, 008h, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh
        db  074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh
        db  0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h
        db  0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch
        db  08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h
        db  08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_547A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_5480:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_5486:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_548C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_5492:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_5498:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_549E:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_54AA:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_54B6:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_54C2:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_54C8:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_54CE:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_54D4:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_54EC:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8057h           ; cd2057802800
        VxDCall  VPOWERD.Service_8058h           ; cd2058802800
        VxDCall  VPOWERD.Service_8059h           ; cd2059802800
        VxDCall  VPOWERD.Service_805Ah           ; cd205a802800
        VxDCall  VPOWERD.Service_805Bh           ; cd205b802800
        VxDCall  VPOWERD.Service_805Ch           ; cd205c802800
        VxDCall  VPOWERD.Service_805Dh           ; cd205d802800
        VxDCall  VPOWERD.Service_805Eh           ; cd205e802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        int3   
        int3   
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_5542:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_5548:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_554E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_5554:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_5560:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_5566:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_556C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_5590:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_5596:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_55A2:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_55B4:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_55D8:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_55DE:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_55E4:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_55EA:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_55F0:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_5638:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
sub_1_5644:
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
sub_1_5650:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5680
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_5680:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_5720
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_56C0
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_56B5
        cmp    al, 0xff
        jne    loc_1_56B5
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_56B5
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_5727
loc_1_56B5:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_5727
        db  08Bh, 0FFh
loc_1_56C0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_5710
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_56E6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_56ED
loc_1_56E6:
        mov    dword ptr [ebp - 0x10], 1
loc_1_56ED:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_5700
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_572A
        db  08Dh, 064h, 024h, 000h
loc_1_5700:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_572A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5710:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_572A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5720:
        mov    dword ptr [ebp - 4], 0x20
loc_1_5727:
        mov    esi, dword ptr [ebp + 8]
loc_1_572A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_5821
loc_1_5735:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_578D
        test   al, 2
        je     loc_1_5779
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_07C0
loc_1_575A:
        or     al, al
        je     loc_1_5779
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_578D
loc_1_5779:
        test   byte ptr [ebp - 4], 2
        je     loc_1_5784
        test   bl, 4
        jne    loc_1_578D
loc_1_5784:
        test   bl, 0x20
        je     loc_1_5816
loc_1_578D:
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_54C2
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_5816
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_57CF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_57CF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_57E0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_57E6
        db  08Bh, 0FFh
loc_1_57E0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_57E6:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_57F5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_54C8
loc_1_57F5:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_556C
        mov    edi, ebx
        jmp    loc_1_5819
loc_1_5816:
        mov    edi, dword ptr [edi + 0x10]
loc_1_5819:
        or     edi, edi
        jne    loc_1_5735
loc_1_5821:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_5830:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5840
        pop    ebp
        ret    4
sub_1_5840:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_58BE
loc_1_5850:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_58B7
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_54C8
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_58B7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_587C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_587C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_5890
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_5896
        db  005h, 000h, 000h, 000h, 000h
loc_1_5890:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_5896:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_556C
        mov    edi, ebx
        jmp    loc_1_58BA
loc_1_58B7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_58BA:
        or     edi, edi
        jne    loc_1_5850
loc_1_58BE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_58D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_58E5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_58E5:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_5921
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_5912
        cmp    bl, 0xff
        jne    loc_1_5912
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_5912
        test   al, 8
        je     loc_1_590B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_590B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_5912:
        test   al, 4
        jne    loc_1_591A
        test   al, 2
        je     loc_1_5939
loc_1_591A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_5921:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_5939
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_5939
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_5939:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_5940:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x14]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    0xc
sub_1_5960:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_5980
        or     ecx, ecx
        je     loc_1_5996
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_5994
loc_1_5980:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_5994
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_5994:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_5996:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0]            ; -> obj1:0x0190
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_5566
loc_2_001D:
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_0034
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0034:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_004E
        mov    esi, 2
loc_2_004E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_5566
loc_2_0061:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_0082
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0082:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_5566
loc_2_0099:
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_00B1
        push   eax
        call   sub_2_0120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_00B1:
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 0x2c], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 0x30], dl
        add    eax, 0x2c
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x14], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x18], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0120:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_0140
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_556C
loc_2_0140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_556C
loc_2_0156:
        push   0
        push   0x32
        push   esi
        call   sub_1_556C
loc_2_0160:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0170:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_0189
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0189:
        mov    eax, dword ptr [0]            ; -> obj1:0x0190
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_5566
loc_2_019C:
        or     eax, eax
        je     loc_2_01A9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_01A9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_01B0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_01C3
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_01B0
        jmp    loc_2_01D2
loc_2_01C3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_01D2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_01E8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_01E8:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 0x19], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_2_0240:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_05E0
        mov    edi, eax
        or     edi, edi
        je     loc_2_0269
        cmp    edi, 0x103
        jne    loc_2_0280
loc_2_0269:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        push   ebx
        call   sub_2_0300
        or     eax, eax
        je     loc_2_0280
        mov    edi, eax
loc_2_0280:
        or     edi, edi
        je     loc_2_028C
        cmp    edi, 0x103
        jne    loc_2_02EB
loc_2_028C:
        mov    eax, dword ptr [esi + 0xc]
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        jne    loc_2_02E6
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_02A2
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_02A2:
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_02B1
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_02B7
loc_2_02B1:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebx + 0x10], eax
loc_2_02B7:
        cmp    byte ptr [esi + 0x18], 0
        je     loc_2_02C6
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_54C8
loc_2_02C6:
        mov    eax, 1
        mov    cl, byte ptr [esi + 0x19]
        shl    eax, cl
        or     dword ptr [ebx + 0x28], eax
        push   0
        push   0x1a
        push   esi
        call   sub_1_556C
loc_2_02DD:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_02E6:
        mov    edi, 0xc001000e
loc_2_02EB:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0300:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x20]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 0x20], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [esi + 0x24]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_5960
loc_2_033B:
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_5960
loc_2_0353:
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_0384
        sub    eax, eax
        nop    
loc_2_0360:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 0x1c]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 0x24], edi
        ja     loc_2_0360
loc_2_0384:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_03EF
loc_2_0391:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_03E4
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        call   sub_1_5960
loc_2_03BE:
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        call   sub_1_5960
loc_2_03DF:
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_03E7
loc_2_03E4:
        inc    dword ptr [ebp - 4]
loc_2_03E7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_0391
loc_2_03EF:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_04DA
        mov    edi, dword ptr [ebp + 0x18]
loc_2_0403:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_07C0
        or     al, al
        je     loc_2_043A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_04C8
loc_2_043A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_0514
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        call   sub_1_5960
loc_2_0464:
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edi + 5]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0x24]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_5960
loc_2_049C:
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x14], edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 0x19]
        mov    edx, dword ptr [ebp - 0x14]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 0x24]
loc_2_04C8:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_2_0403
loc_2_04DA:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edx, edx
        cmp    dword ptr [ebp - 0xc], edx
        je     loc_2_0565
        mov    edi, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_04F0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_056B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_2_0552
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    loc_2_0552
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_0559
loc_2_0514:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_5960
loc_2_052A:
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_5960
loc_2_053E:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_0552:
        mov    dword ptr [ebp - 8], 1
loc_2_0559:
        add    edi, 6
        add    ecx, 6
        inc    edx
        cmp    dword ptr [ebp - 0xc], edx
        ja     loc_2_04F0
loc_2_0565:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_05D0
loc_2_056B:
        mov    al, byte ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   dword ptr [esi + 0x14]
        mov    edi, eax
        or     edi, edi
        je     loc_2_05D2
        cmp    edi, 0x103
        je     loc_2_05D2
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_5960
loc_2_05B5:
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_5960
loc_2_05C3:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_05D0:
        sub    edi, edi
loc_2_05D2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_05E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [edi + 0xc]
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        mov    dword ptr [edi + 0xc], 0
        je     loc_2_061D
        lea    esp, [esp]
loc_2_0610:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0610
loc_2_061D:
        mov    ecx, dword ptr [edi + 0xc]
        cmp    esi, ecx
        je     loc_2_0651
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 0x18]
        or     eax, eax
        je     loc_2_0653
        cmp    eax, 0x103
        je     loc_2_0653
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [edi + 0xc], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0651:
        sub    eax, eax
loc_2_0653:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_0660:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 0x19]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0xff
        mov    edi, dword ptr [edx + 0x24]
        cmp    edi, eax
        je     loc_2_0692
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_0687:
        test   dword ptr [esi], edx
        je     loc_2_068C
        inc    eax
loc_2_068C:
        add    esi, 4
        dec    edi
        jne    loc_2_0687
loc_2_0692:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_06A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x19]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 0x24], 0
        jbe    loc_2_071C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_06E0:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_070A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_0736
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_5960
loc_2_0704:
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_070A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_06E0
loc_2_071C:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_0736:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x18]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0760:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_0790
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0790:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edx + 0x24]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_5960
loc_2_07B4:
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_07C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        shr    edx, 1
        cmp    esi, ecx
        je     loc_2_0859
        dec    esi
        cmp    esi, edx
        jb     loc_2_0859
        nop    
loc_2_07E0:
        cmp    edx, ecx
        jb     loc_2_0859
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_0803
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0837
loc_2_0803:
        jbe    loc_2_0810
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_0837
        db  08Bh, 0FFh
loc_2_0810:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_0827
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0837
loc_2_0827:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_0837
        mov    dword ptr [ebp - 4], 0
loc_2_0837:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0869
        jle    loc_2_0848
        or     edx, edx
        je     loc_2_0859
        lea    esi, [edx - 1]
        jmp    loc_2_084B
loc_2_0848:
        lea    ecx, [edx + 1]
loc_2_084B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_07E0
loc_2_0859:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0869:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS
