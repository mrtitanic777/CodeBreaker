// cb-unity: alone (file-local names that other files in a batch also use)
// cb/formats/psp_nids.cpp -- the functions of the PSP's user-mode libraries by NID (psp.hpp).
//
// A NID is the first four bytes of the SHA-1 of a function's name, so the table is the names: pspsdk's published export lists (the `.exp` files of its
// libraries) name these functions, and each name's NID is computed when the table is first used. A name that is not here leaves its NID unnamed.
#include <string>
#include <unordered_map>

#include "cb/formats/psp.hpp"

namespace cb::formats::psp {

namespace {

constexpr std::string_view kNames[] = {
    // sceDisplay
    "sceDisplaySetMode", "sceDisplayGetMode", "sceDisplaySetFrameBuf", "sceDisplayGetFrameBuf", "sceDisplayIsForeground", "sceDisplayGetFramePerSec",
    "sceDisplayIsVblank", "sceDisplayWaitVblank", "sceDisplayWaitVblankCB", "sceDisplayWaitVblankStart", "sceDisplayWaitVblankStartCB", "sceDisplayGetVcount",
    "sceDisplayGetCurrentHcount", "sceDisplayGetAccumulatedHcount", "sceDisplayAdjustAccumulatedHcount", "sceDisplaySetHoldMode", "sceDisplayGetBrightness",
    // sceCtrl
    "sceCtrlSetSamplingCycle", "sceCtrlGetSamplingCycle", "sceCtrlSetSamplingMode", "sceCtrlGetSamplingMode", "sceCtrlPeekBufferPositive", "sceCtrlPeekBufferNegative",
    "sceCtrlReadBufferPositive", "sceCtrlReadBufferNegative", "sceCtrlPeekLatch", "sceCtrlReadLatch", "sceCtrlSetIdleCancelThreshold", "sceCtrlGetIdleCancelThreshold",
    // sceGe
    "sceGeEdramGetSize", "sceGeEdramGetAddr", "sceGeEdramSetAddrTranslation", "sceGeGetCmd", "sceGeGetMtx", "sceGeGetStack", "sceGeSaveContext", "sceGeRestoreContext",
    "sceGeListEnQueue", "sceGeListEnQueueHead", "sceGeListDeQueue", "sceGeListUpdateStallAddr", "sceGeListSync", "sceGeDrawSync", "sceGeBreak", "sceGeContinue",
    "sceGeSetCallback", "sceGeUnsetCallback",
    // LoadExecForUser, ModuleMgrForUser
    "sceKernelExitGame", "sceKernelExitGameWithStatus", "sceKernelRegisterExitCallback", "sceKernelLoadExec", "sceKernelLoadModule", "sceKernelLoadModuleByID",
    "sceKernelStartModule", "sceKernelStopModule", "sceKernelUnloadModule", "sceKernelSelfStopUnloadModule", "sceKernelStopUnloadSelfModule", "sceKernelGetModuleId",
    "sceKernelGetModuleIdByAddress", "sceKernelQueryModuleInfo",
    // ThreadManForUser
    "sceKernelCreateThread", "sceKernelDeleteThread", "sceKernelStartThread", "sceKernelExitThread", "sceKernelExitDeleteThread", "sceKernelTerminateThread",
    "sceKernelTerminateDeleteThread", "sceKernelSuspendDispatchThread", "sceKernelResumeDispatchThread", "sceKernelChangeCurrentThreadAttr", "sceKernelChangeThreadPriority",
    "sceKernelRotateThreadReadyQueue", "sceKernelReleaseWaitThread", "sceKernelGetThreadId", "sceKernelGetThreadCurrentPriority", "sceKernelGetThreadExitStatus",
    "sceKernelCheckThreadStack", "sceKernelGetThreadStackFreeSize", "sceKernelReferThreadStatus", "sceKernelReferThreadRunStatus", "sceKernelGetThreadmanIdList",
    "sceKernelSleepThread", "sceKernelSleepThreadCB", "sceKernelWakeupThread", "sceKernelCancelWakeupThread", "sceKernelSuspendThread", "sceKernelResumeThread",
    "sceKernelWaitThreadEnd", "sceKernelWaitThreadEndCB", "sceKernelDelayThread", "sceKernelDelayThreadCB", "sceKernelDelaySysClockThread", "sceKernelDelaySysClockThreadCB",
    "sceKernelCreateSema", "sceKernelDeleteSema", "sceKernelSignalSema", "sceKernelWaitSema", "sceKernelWaitSemaCB", "sceKernelPollSema", "sceKernelCancelSema",
    "sceKernelReferSemaStatus", "sceKernelCreateEventFlag", "sceKernelDeleteEventFlag", "sceKernelSetEventFlag", "sceKernelClearEventFlag", "sceKernelWaitEventFlag",
    "sceKernelWaitEventFlagCB", "sceKernelPollEventFlag", "sceKernelCancelEventFlag", "sceKernelReferEventFlagStatus", "sceKernelCreateMbx", "sceKernelDeleteMbx",
    "sceKernelSendMbx", "sceKernelReceiveMbx", "sceKernelReceiveMbxCB", "sceKernelPollMbx", "sceKernelCancelReceiveMbx", "sceKernelReferMbxStatus", "sceKernelCreateMsgPipe",
    "sceKernelDeleteMsgPipe", "sceKernelSendMsgPipe", "sceKernelReceiveMsgPipe", "sceKernelCreateVpl", "sceKernelDeleteVpl", "sceKernelAllocateVpl", "sceKernelFreeVpl",
    "sceKernelCreateFpl", "sceKernelDeleteFpl", "sceKernelAllocateFpl", "sceKernelFreeFpl", "sceKernelReferFplStatus", "sceKernelCreateCallback", "sceKernelDeleteCallback",
    "sceKernelNotifyCallback", "sceKernelCancelCallback", "sceKernelGetCallbackCount", "sceKernelCheckCallback", "sceKernelReferCallbackStatus", "sceKernelCreateLwMutex",
    "sceKernelDeleteLwMutex", "sceKernelLockLwMutex", "sceKernelUnlockLwMutex", "sceKernelTryLockLwMutex", "sceKernelCreateMutex", "sceKernelDeleteMutex",
    "sceKernelLockMutex", "sceKernelUnlockMutex", "sceKernelTryLockMutex", "sceKernelSetAlarm", "sceKernelSetSysClockAlarm", "sceKernelCancelAlarm", "sceKernelCreateVTimer",
    "sceKernelDeleteVTimer", "sceKernelGetVTimerBase", "sceKernelGetVTimerTime", "sceKernelStartVTimer", "sceKernelStopVTimer", "sceKernelSetVTimerHandler",
    "sceKernelSysClock2USec", "sceKernelUSec2SysClock", "sceKernelUSec2SysClockWide", "sceKernelSysClock2USecWide", "sceKernelGetSystemTime", "sceKernelGetSystemTimeLow",
    "sceKernelGetSystemTimeWide", "sceKernelRegisterThreadEventHandler", "sceKernelReleaseThreadEventHandler",
    // SysMemUserForUser, Kernel_Library, UtilsForUser, sceSuspendForUser, InterruptManager
    "sceKernelAllocPartitionMemory", "sceKernelFreePartitionMemory", "sceKernelGetBlockHeadAddr", "sceKernelTotalFreeMemSize", "sceKernelMaxFreeMemSize",
    "sceKernelDevkitVersion", "sceKernelPrintf", "sceKernelSetCompiledSdkVersion", "sceKernelGetCompiledSdkVersion", "sceKernelQueryMemoryPartitionInfo",
    "sceKernelCpuSuspendIntr", "sceKernelCpuResumeIntr", "sceKernelCpuResumeIntrWithSync", "sceKernelIsCpuIntrSuspended", "sceKernelIsCpuIntrEnable",
    "sceKernelRegisterSubIntrHandler", "sceKernelReleaseSubIntrHandler", "sceKernelEnableSubIntr", "sceKernelDisableSubIntr", "sceKernelDcacheWritebackAll",
    "sceKernelDcacheWritebackInvalidateAll", "sceKernelDcacheInvalidateAll", "sceKernelDcacheWritebackRange", "sceKernelDcacheWritebackInvalidateRange",
    "sceKernelDcacheInvalidateRange", "sceKernelIcacheInvalidateAll", "sceKernelIcacheInvalidateRange", "sceKernelDcacheProbe", "sceKernelUtilsMt19937Init",
    "sceKernelUtilsMt19937UInt", "sceKernelUtilsMd5Digest", "sceKernelUtilsMd5BlockInit", "sceKernelUtilsMd5BlockUpdate", "sceKernelUtilsMd5BlockResult",
    "sceKernelUtilsSha1Digest", "sceKernelUtilsSha1BlockInit", "sceKernelUtilsSha1BlockUpdate", "sceKernelUtilsSha1BlockResult", "sceKernelLibcClock", "sceKernelLibcTime",
    "sceKernelLibcGettimeofday", "sceKernelPowerLock", "sceKernelPowerUnlock", "sceKernelPowerTick", "sceKernelVolatileMemLock", "sceKernelVolatileMemTryLock",
    "sceKernelVolatileMemUnlock", "sceKernelStdin", "sceKernelStdout", "sceKernelStderr",
    // IoFileMgrForUser
    "sceIoOpen", "sceIoOpenAsync", "sceIoClose", "sceIoCloseAsync", "sceIoRead", "sceIoReadAsync", "sceIoWrite", "sceIoWriteAsync", "sceIoLseek", "sceIoLseekAsync",
    "sceIoLseek32", "sceIoLseek32Async", "sceIoRemove", "sceIoMkdir", "sceIoRmdir", "sceIoChdir", "sceIoRename", "sceIoDopen", "sceIoDread", "sceIoDclose", "sceIoGetstat",
    "sceIoChstat", "sceIoDevctl", "sceIoIoctl", "sceIoIoctlAsync", "sceIoSync", "sceIoWaitAsync", "sceIoWaitAsyncCB", "sceIoPollAsync", "sceIoGetAsyncStat",
    "sceIoCancel", "sceIoGetDevType", "sceIoAssign", "sceIoUnassign", "sceIoSetAsyncCallback",
    // sceAudio
    "sceAudioChReserve", "sceAudioChRelease", "sceAudioOutput", "sceAudioOutputBlocking", "sceAudioOutputPanned", "sceAudioOutputPannedBlocking", "sceAudioGetChannelRestLen",
    "sceAudioGetChannelRestLength", "sceAudioSetChannelDataLen", "sceAudioChangeChannelConfig", "sceAudioChangeChannelVolume", "sceAudioSRCChReserve", "sceAudioSRCChRelease",
    "sceAudioSRCOutputBlocking", "sceAudioInputBlocking", "sceAudioInput", "sceAudioInputInit", "sceAudioGetInputLength", "sceAudioWaitInputEnd", "sceAudioPollInputEnd",
    // scePower
    "scePowerRegisterCallback", "scePowerUnregisterCallback", "scePowerIsPowerOnline", "scePowerIsBatteryExist", "scePowerIsBatteryCharging", "scePowerGetBatteryChargingStatus",
    "scePowerIsLowBattery", "scePowerGetBatteryLifePercent", "scePowerGetBatteryLifeTime", "scePowerGetBatteryTemp", "scePowerGetBatteryVolt", "scePowerGetBatteryRemainCapacity",
    "scePowerGetBatteryFullCapacity", "scePowerGetCpuClockFrequency", "scePowerGetCpuClockFrequencyInt", "scePowerGetCpuClockFrequencyFloat", "scePowerGetBusClockFrequency",
    "scePowerGetBusClockFrequencyInt", "scePowerGetBusClockFrequencyFloat", "scePowerGetPllClockFrequencyInt", "scePowerGetPllClockFrequencyFloat", "scePowerSetClockFrequency",
    "scePowerSetCpuClockFrequency", "scePowerSetBusClockFrequency", "scePowerTick", "scePowerLock", "scePowerUnlock", "scePowerRequestStandby", "scePowerRequestSuspend",
    "scePowerCancelRequest", "scePowerIsRequest", "scePowerIsSuspendRequired",
    // sceRtc
    "sceRtcGetTickResolution", "sceRtcGetCurrentTick", "sceRtcGetCurrentClock", "sceRtcGetCurrentClockLocalTime", "sceRtcConvertUtcToLocalTime", "sceRtcConvertLocalTimeToUTC",
    "sceRtcIsLeapYear", "sceRtcGetDaysInMonth", "sceRtcGetDayOfWeek", "sceRtcCheckValid", "sceRtcSetTick", "sceRtcGetTick", "sceRtcCompareTick", "sceRtcTickAddTicks",
    "sceRtcTickAddMicroseconds", "sceRtcTickAddSeconds", "sceRtcTickAddMinutes", "sceRtcTickAddHours", "sceRtcTickAddDays", "sceRtcTickAddWeeks", "sceRtcTickAddMonths",
    "sceRtcTickAddYears", "sceRtcSetTime_t", "sceRtcGetTime_t", "sceRtcSetDosTime", "sceRtcGetDosTime", "sceRtcSetWin32FileTime", "sceRtcGetWin32FileTime",
    "sceRtcParseDateTime", "sceRtcFormatRFC2822", "sceRtcFormatRFC2822LocalTime", "sceRtcFormatRFC3339", "sceRtcFormatRFC3339LocalTime", "sceRtcParseRFC3339",
    // sceUtility
    "sceUtilitySavedataInitStart", "sceUtilitySavedataGetStatus", "sceUtilitySavedataShutdownStart", "sceUtilitySavedataUpdate", "sceUtilityMsgDialogInitStart",
    "sceUtilityMsgDialogShutdownStart", "sceUtilityMsgDialogGetStatus", "sceUtilityMsgDialogUpdate", "sceUtilityMsgDialogAbort", "sceUtilityOskInitStart",
    "sceUtilityOskShutdownStart", "sceUtilityOskUpdate", "sceUtilityOskGetStatus", "sceUtilityNetconfInitStart", "sceUtilityNetconfShutdownStart", "sceUtilityNetconfUpdate",
    "sceUtilityNetconfGetStatus", "sceUtilityCheckNetParam", "sceUtilityGetNetParam", "sceUtilityLoadModule", "sceUtilityUnloadModule", "sceUtilityLoadNetModule",
    "sceUtilityUnloadNetModule", "sceUtilityLoadAvModule", "sceUtilityUnloadAvModule", "sceUtilityGetSystemParamInt", "sceUtilityGetSystemParamString",
    "sceUtilityGameSharingInitStart", "sceUtilityGameSharingShutdownStart", "sceUtilityGameSharingUpdate", "sceUtilityGameSharingGetStatus",
    // sceImpose, sceUmd, sceHprm, sceUsb, sceWlan
    "sceImposeGetBatteryIconStatus", "sceImposeSetLanguageMode", "sceImposeGetLanguageMode", "sceImposeSetUMDPopup", "sceImposeGetUMDPopup", "sceImposeGetBacklightOffTime",
    "sceImposeSetBacklightOffTime", "sceUmdCheckMedium", "sceUmdActivate", "sceUmdDeactivate", "sceUmdWaitDriveStat", "sceUmdWaitDriveStatCB", "sceUmdWaitDriveStatWithTimer",
    "sceUmdGetDriveStat", "sceUmdGetErrorStat", "sceUmdRegisterUMDCallBack", "sceUmdUnRegisterUMDCallBack", "sceUmdGetDiscInfo", "sceUmdReplacePermit",
    "sceUmdReplaceProhibit", "sceHprmPeekCurrentKey", "sceHprmPeekLatch", "sceHprmReadLatch", "sceHprmIsRemoteExist", "sceHprmIsHeadphoneExist", "sceHprmIsMicrophoneExist",
    "sceHprmRegisterCallback", "sceHprmUnregisterCallback", "sceUsbStart", "sceUsbStop", "sceUsbActivate", "sceUsbDeactivate", "sceUsbGetState", "sceUsbGetDrvState",
    "sceWlanDevIsPowerOn", "sceWlanGetSwitchState", "sceWlanGetEtherAddr", "sceOpenPSIDGetOpenPSID",
    // media: sceAtrac3plus, sceMp3, sceMpeg, scePsmf, sceSasCore, sceVaudio, sceCcc
    "sceAtracGetAtracID", "sceAtracSetDataAndGetID", "sceAtracReleaseAtracID", "sceAtracDecodeData", "sceAtracGetRemainFrame", "sceAtracGetStreamDataInfo",
    "sceAtracAddStreamData", "sceAtracGetNextSample", "sceAtracGetSoundSample", "sceAtracSetLoopNum", "sceAtracGetBitrate", "sceAtracGetChannel", "sceAtracGetMaxSample",
    "sceMp3ReserveMp3Handle", "sceMp3ReleaseMp3Handle", "sceMp3InitResource", "sceMp3TermResource", "sceMp3Init", "sceMp3Decode", "sceMp3GetInfoToAddStreamData",
    "sceMp3NotifyAddStreamData", "sceMp3CheckStreamDataNeeded", "sceMp3SetLoopNum", "sceMp3GetLoopNum", "sceMp3GetSumDecodedSample", "sceMp3ResetPlayPosition",
    "sceMp3GetSamplingRate", "sceMp3GetBitRate", "sceMp3GetMaxOutputSample", "sceMp3GetMp3ChannelNum", "sceMpegInit", "sceMpegFinish", "sceMpegQueryMemSize", "sceMpegCreate",
    "sceMpegDelete", "sceMpegRingbufferQueryMemSize", "sceMpegRingbufferConstruct", "sceMpegRingbufferDestruct", "sceMpegRingbufferPut", "sceMpegRingbufferAvailableSize",
    "sceMpegAvcDecode", "sceMpegAvcDecodeStop", "sceMpegAtracDecode", "sceMpegGetAvcAu", "sceMpegGetAtracAu", "sceMpegFlushAllStream", "sceMpegRegistStream",
    "sceMpegUnRegistStream", "sceMpegQueryStreamOffset", "sceMpegQueryStreamSize", "scePsmfSetPsmf", "scePsmfGetNumberOfStreams", "scePsmfGetCurrentStreamType",
    "sceVaudioChReserve", "sceVaudioChRelease", "sceVaudioOutputBlocking", "sceSasInit", "sceSasCoreInit", "sceSasCore", "sceSasCoreWithMix", "sceSasSetOutputmode",
    "sceCccUTF8toUTF16", "sceCccUTF16toUTF8", "sceCccSJIStoUTF16", "sceCccUTF16toSJIS", "sceCccSetErrorCharUTF8", "sceCccSetErrorCharUTF16",
    // networking
    "sceNetInit", "sceNetTerm", "sceNetEtherNtostr", "sceNetEtherStrton", "sceNetGetLocalEtherAddr", "sceNetInetInit", "sceNetInetTerm", "sceNetInetSocket", "sceNetInetBind",
    "sceNetInetConnect", "sceNetInetListen", "sceNetInetAccept", "sceNetInetSend", "sceNetInetSendto", "sceNetInetRecv", "sceNetInetRecvfrom", "sceNetInetClose",
    "sceNetInetShutdown", "sceNetInetSetsockopt", "sceNetInetGetsockopt", "sceNetInetSelect", "sceNetInetInetAton", "sceNetInetInetNtop", "sceNetApctlInit", "sceNetApctlTerm",
    "sceNetApctlConnect", "sceNetApctlDisconnect", "sceNetApctlGetState", "sceNetApctlAddHandler", "sceNetApctlDelHandler", "sceNetApctlGetInfo", "sceNetResolverInit",
    "sceNetResolverTerm", "sceNetResolverCreate", "sceNetResolverDelete", "sceNetResolverStartNtoA", "sceNetResolverStartAtoN", "sceNetAdhocInit", "sceNetAdhocTerm",
    "sceNetAdhocctlInit", "sceNetAdhocctlTerm", "sceNetAdhocPdpCreate", "sceNetAdhocPdpDelete", "sceNetAdhocPdpSend", "sceNetAdhocPdpRecv", "sceHttpInit", "sceHttpEnd",
    "sceHttpCreateTemplate", "sceHttpDeleteTemplate", "sceHttpCreateConnectionWithURL", "sceHttpCreateRequestWithURL", "sceHttpSendRequest", "sceHttpGetStatusCode",
    "sceHttpReadData", "sceHttpDeleteRequest", "sceHttpDeleteConnection", "sceSslInit", "sceSslEnd",
    // fonts, jpeg, screen shots, power-saving
    "sceFontNewLib", "sceFontDoneLib", "sceFontOpen", "sceFontOpenUserMemory", "sceFontClose", "sceFontGetCharInfo", "sceFontGetCharGlyphImage", "sceFontFindOptimumFont",
    "sceFontGetNumFontList", "sceFontGetFontList", "sceFontSetResolution", "sceJpegInitMJpeg", "sceJpegFinishMJpeg", "sceJpegCreateMJpeg", "sceJpegDeleteMJpeg",
    "sceJpegDecodeMJpeg", "sceChnnlsv_Initialize",
};

const std::unordered_map<std::uint32_t, std::string_view>& table() {
    static const std::unordered_map<std::uint32_t, std::string_view> names = [] {
        std::unordered_map<std::uint32_t, std::string_view> t;
        for (const std::string_view name : kNames) t.emplace(nid_of(name), name);
        // the system exports every module has, by fixed NID
        t.emplace(0xD632ACDBu, "module_start");
        t.emplace(0xCEE8593Cu, "module_stop");
        t.emplace(0xF01D73A7u, "module_info");
        t.emplace(0x2F064FA6u, "module_reboot_before");
        t.emplace(0xD3744BE0u, "module_bootstart");
        t.emplace(0x0F7C276Cu, "module_reboot_phase");
        return t;
    }();
    return names;
}

}  // namespace

std::string_view nid_name(std::uint32_t nid) {
    const auto& t = table();
    const auto it = t.find(nid);
    return it == t.end() ? std::string_view() : it->second;
}

}  // namespace cb::formats::psp
