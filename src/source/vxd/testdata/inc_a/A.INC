; comment
Begin_Service_Table VMM, VMM
VMM_Service  Get_VMM_Version, LOCAL     ; ordinal 0
VMM_Service  Get_Cur_VM_Handle          ; ordinal 1
   vmm_service  lower_case_service
VMM_Service Dup, 1
VMM_ServiceX NotAService
VXD_Service Wrong_Dev
End_Service_Table VMM, VMM
Begin_Service_Table VMM
VMM_Service Second_Table_Ignored
End_Service_Table
