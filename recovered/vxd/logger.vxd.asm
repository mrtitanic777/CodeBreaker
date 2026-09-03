; LOGGER -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 608 instructions recovered by recursive descent

Declare_Virtual_Device LOGGER, 1, 0, LOGGER_Control, 0049Dh, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h
;   service table: obj1:0x18, 5 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0xb0  R-X
sub_1_0000:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_1_0008:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_1_0010:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        add    byte ptr [eax], al
        dd  LOGGER_Service_00        ; fixup
        dd  LOGGER_Service_01        ; fixup
        dd  LOGGER_Service_02        ; fixup
        dd  LOGGER_Service_03        ; fixup
        dd  LOGGER_Service_04        ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0049Dh                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'LOGGER  '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  LOGGER_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x0018              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
loc_1_0080:
        pop    ds
        add    byte ptr [eax], al
        db  000h
loc_1_0084:
        sbb    al, 0
        add    byte ptr [eax], al
        pop    ds
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x3a  R-X
        db  001h, 000h, 000h, 000h
loc_2_0004:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
        sub    al, byte ptr [eax]
loc_2_0010:
        and    eax, 0x2d643230
        and    eax, 0x2d643230
        and    eax, 0x20643230
        and    eax, 0x3a643230
        and    eax, 0x3a643230
        and    eax, 0x322e3230
        and    eax, 0x322e3230
        db  064h, 020h, 02Dh, 020h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x754  R-X
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, 0xd500
        mov    bx, 2
        mov    cx, 0
        mov    dl, 0x11
        mov    dh, 0
        mov    esi, dword ptr [ebp - 8]
        call   sub_3_070A
        jae    loc_3_0033
        xor    eax, eax
loc_3_0033:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_3_005B
        mov    eax, 0xd800
        mov    ebx, dword ptr [ebp - 4]
        call   sub_3_070A
        jae    loc_3_0058
        mov    eax, 0xffffffff
loc_3_0058:
        mov    dword ptr [ebp - 0xc], eax
loc_3_005B:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x104], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_3_0077
loc_3_0077:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
LOGGER_Service_03:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_3_0096
        mov    dword ptr [ebp + 0xc], 0x1000
loc_3_0096:
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x119
        push   eax
        call   sub_1_0000
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_3_00BD
        sub    eax, eax
        jmp    loc_3_019F
loc_3_00BD:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x10c], eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 1]
        cmp    eax, 0x3a
        je     loc_3_0114
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax]
        cmp    eax, 0x5c
        je     loc_3_0114
        call   sub_3_0740
        mov    edi, eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    edx, edi
        mov    edi, dword ptr [ebp - 4]
        add    edi, 4
        mov    esi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_0114:
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    edx, edi
        mov    ebx, ecx
        mov    edi, dword ptr [ebp - 4]
        add    edi, 4
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    esi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        add    esp, 4
        or     eax, eax
        je     loc_3_0184
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x104], 0
        jae    loc_3_017F
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_0440
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
loc_3_017F:
        jmp    loc_3_0197
loc_3_0184:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_0440
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
loc_3_0197:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_3_019F
loc_3_019F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_3_01A4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0x114], 4
        je     loc_3_01C5
        jmp    loc_3_0303
loc_3_01C5:
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 0x114], 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x118
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x10], 0
loc_3_01E9:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x104]
        add    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x108]
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, 0xd601
        mov    ebx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x1c]
        mov    esi, dword ptr [ebp - 0x14]
        call   sub_3_070A
        jae    loc_3_0228
        mov    eax, 0xffffffff
loc_3_0228:
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        jb     loc_3_0241
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x104], eax
loc_3_0241:
        mov    eax, dword ptr [ebp - 0xc]
        add    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x108], ecx
        jne    loc_3_01E9
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x108], 0
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x110]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x110], 0
        jne    loc_3_02EF
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0x114], 2
        je     loc_3_02CC
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0x114], 1
        jne    loc_3_02CC
        cmp    dword ptr [ebp - 8], 0
        je     loc_3_02B6
        mov    eax, 0xd700
        mov    ebx, dword ptr [ebp - 8]
        call   sub_3_070A
loc_3_02B6:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        and    dword ptr [eax + 0x114], 0xfffffffd
loc_3_02CC:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0x114], 1
        je     loc_3_02EF
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_0440
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
loc_3_02EF:
        cmp    dword ptr [ebp - 4], 0
        je     loc_3_0303
        mov    eax, dword ptr [ebp - 4]
        and    dword ptr [eax + 0x114], 0xfffffffb
loc_3_0303:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
LOGGER_Service_01:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_3_043B
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        je     loc_3_043B
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x104], 0x40000
        jbe    loc_3_033F
        jmp    loc_3_043B
loc_3_033F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10c]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x108]
        cmp    eax, 0x100
        jae    loc_3_0361
        jmp    loc_3_043B
loc_3_0361:
        xor    eax, eax
        mov    ax, word ptr [0xc]            ; -> loc_2_000C
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x108]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x118
        push   eax
        call   sub_3_053D
        add    esp, 8
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x108], eax
        lea    eax, [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x108]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x118
        push   eax
        call   sub_1_0010
        add    esp, 0x30
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x108], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10c]
        shr    eax, 1
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x108]
        ja     loc_3_043B
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x110]
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [0x1a4]                  ; -> loc_3_01A4
        mov    ecx, 0x40
        call   sub_3_06F4
loc_3_043B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_3_0440:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_3_0474
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        je     loc_3_0474
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0xd700
        mov    ebx, dword ptr [ebp - 4]
        call   sub_3_070A
loc_3_0474:
        cmp    dword ptr [ebp + 8], 0
        je     loc_3_048C
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0008
        add    esp, 8
loc_3_048C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
LOGGER_Service_02:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_3_04DC
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 0x114], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 0
        je     loc_3_04C0
        jmp    loc_3_04DC
loc_3_04C0:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x110]
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [0x1a4]                  ; -> loc_3_01A4
        mov    ecx, 0x40
        call   sub_3_06F4
loc_3_04DC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
LOGGER_Service_04:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_3_0538
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        je     loc_3_0538
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 0x114], 2
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x110], 0
        je     loc_3_051C
        jmp    loc_3_0538
loc_3_051C:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x110]
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [0x1a4]                  ; -> loc_3_01A4
        mov    ecx, 0x40
        call   sub_3_06F4
loc_3_0538:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_3_053D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_3_0710
        add    esp, 4
        mov    dword ptr [ebp - 0x2c], eax
        mov    ecx, 0x15180
        mov    eax, dword ptr [ebp - 0x2c]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 0x1c], eax
        mov    ecx, 0x15180
        mov    eax, dword ptr [ebp - 0x2c]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 0x2c], edx
        mov    ecx, 0x3c
        mov    eax, dword ptr [ebp - 0x2c]
        sub    edx, edx
        div    ecx
        mov    word ptr [ebp - 0x14], dx
        mov    ecx, 0x3c
        mov    eax, dword ptr [ebp - 0x2c]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 0x2c], eax
        mov    ecx, 0x3c
        mov    eax, dword ptr [ebp - 0x2c]
        sub    edx, edx
        div    ecx
        mov    word ptr [ebp - 4], dx
        mov    ecx, 0x3c
        mov    eax, dword ptr [ebp - 0x2c]
        sub    edx, edx
        div    ecx
        mov    word ptr [ebp - 0x30], ax
        mov    ecx, 0x5b5
        mov    eax, dword ptr [ebp - 0x1c]
        sub    edx, edx
        div    ecx
        mov    word ptr [ebp - 0x28], ax
        mov    ecx, 0x5b5
        mov    eax, dword ptr [ebp - 0x1c]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 0x20], edx
        mov    eax, dword ptr [ebp - 0x28]
        and    eax, 0xffff
        shl    eax, 2
        add    eax, 0x7bc
        mov    word ptr [ebp - 0x34], ax
        mov    dword ptr [0x84], 0x1d        ; -> loc_1_0084
        mov    word ptr [ebp - 0x18], 0x16e
        sub    eax, eax
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 0x20], eax
        js     loc_3_0640
        dec    word ptr [ebp - 0x18]
loc_3_0611:
        cmp    dword ptr [ebp - 0x20], 0
        jl     loc_3_0636
        inc    word ptr [ebp - 0x34]
        sub    eax, eax
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 0x20], eax
        jmp    loc_3_0611
loc_3_0636:
        mov    dword ptr [0x84], 0x1c        ; -> loc_1_0084
loc_3_0640:
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        add    dword ptr [ebp - 0x20], eax
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_3_065A
loc_3_0657:
        inc    dword ptr [ebp - 0xc]
loc_3_065A:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x20]
        cmp    dword ptr [eax*4 + 0x80], ecx ; -> loc_1_0080
        jg     loc_3_0683
        sub    eax, eax
        mov    ecx, dword ptr [ebp - 0xc]
        sub    eax, dword ptr [ecx*4 + 0x80] ; -> loc_1_0080
        neg    eax
        sub    dword ptr [ebp - 0x20], eax
        jmp    loc_3_0657
loc_3_0683:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        inc    eax
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp - 0x20]
        and    eax, 0xffff
        inc    eax
        mov    word ptr [ebp - 0x24], ax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp - 0x34]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0010
        add    esp, 0x24
        jmp    loc_3_06EC
loc_3_06EC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
sub_3_06F4:
        mov    eax, 0
        mov    ebx, 0
        VMMCall  VMM.Call_Restricted_Event       ; cd205a810100
        VMMCall  VMM._GetVxDName                 ; cd207e810100
sub_3_070A:
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032804000
sub_3_0710:
        push   ebp
        mov    ebp, esp
        VxDCall  VTD.VTD_Get_Date_And_Time       ; cd2008000500
        imul   edx, edx, 0x15180
        push   edx
        mov    ecx, 0x3e8
        xor    edx, edx
        div    ecx
        push   eax
        mov    eax, edx
        xor    edx, edx
        mov    ecx, 0xa
        div    ecx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx], eax
        pop    eax
        pop    edx
        add    eax, edx
        leave  
        ret    
sub_3_0740:
        VMMCall  VMM.Get_Config_Directory        ; cd20b7000100
        mov    eax, edx
LOGGER_Control:
        clc    
        ret    
LOGGER_Service_00:
        mov    eax, 1
        clc    
        ret    
        db  023h, 04Ch, 04Fh
VxD__LTEXT ENDS
