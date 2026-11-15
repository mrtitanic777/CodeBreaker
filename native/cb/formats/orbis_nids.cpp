// cb-unity: alone
// cb/formats/orbis_nids.cpp -- the names an Orbis ELF's imports are given, and the NID and system-call arithmetic behind them.
//
// An Orbis ELF imports a function by its NID: the first eight bytes of SHA-1(name + salt), reversed, in base 64 (`+` and `-` for the 62nd and 63rd digit,
// no padding). Nothing in the file says the name, so a NID is named by hashing the names of the functions programs are known to call: the C library's, the
// POSIX and BSD system interface's, and the Sony library functions OpenOrbis's headers and the PS5 payload SDK's programs name. The list is of names (facts about
// the interface), hashed here; a function not in it keeps its NID. The salt is the system's.
#include <array>
#include <string>
#include <unordered_map>

#include "cb/core/crypto.hpp"
#include "cb/formats/orbis.hpp"

namespace cb::formats::orbis {

namespace {

constexpr std::uint8_t kSalt[16] = {0x51, 0x8D, 0x64, 0xA6, 0x35, 0xDE, 0xD8, 0xC1, 0xE6, 0xB0, 0x39, 0xB1, 0xC3, 0xE5, 0x52, 0x30};

constexpr const char* kNames[] = {
    "FT_Get_Char_Index", "FT_Init_FreeType", "FT_Load_Glyph", "FT_New_Face", "FT_Render_Glyph", "FT_Set_Pixel_Sizes", "_Unwind_RaiseException",
    "_Unwind_Resume", "_ZSt17__throw_bad_allocv", "_ZSt9terminatev", "_ZdaPv", "_ZdlPv", "_ZdlPvm", "_Znam", "_Znwm", "_ZnwmRKSt9nothrow_t",
    "__cxa_allocate_exception", "__cxa_atexit", "__cxa_begin_catch", "__cxa_end_catch", "__cxa_finalize", "__cxa_guard_acquire",
    "__cxa_guard_release", "__cxa_pure_virtual", "__cxa_throw", "__error", "__gxx_personality_v0", "__inet_ntop", "__isthreaded", "__stack_chk_fail",
    "__stack_chk_guard", "__stderrp", "__sys_exit", "__sys_socketex", "_close", "_exit", "_fcntl", "_fstat", "_ioctl", "_longjmp", "_nanosleep",
    "_open", "_read", "_readv", "_setjmp", "_write", "_writev", "abort", "abs", "accept", "access", "acos", "acosf", "aligned_alloc", "asctime",
    "asin", "asinf", "atan", "atan2", "atan2f", "atanf", "atexit", "atoi", "atol", "bind", "bsearch", "calloc", "cbrt", "ceil", "ceilf", "chdir",
    "chmod", "clock", "clock_getres", "clock_gettime", "close", "closedir", "connect", "cos", "cosf", "cosh", "cpuset_getaffinity", "ctime",
    "difftime", "div", "dup", "dup2", "eglBindAPI", "eglChooseConfig", "eglCreateContext", "eglCreateWindowSurface", "eglDestroyContext",
    "eglDestroySurface", "eglGetDisplay", "eglGetError", "eglInitialize", "eglMakeCurrent", "eglSwapBuffers", "eglSwapInterval", "eglTerminate",
    "execve", "exit", "exp", "exp2", "expf", "fabs", "fabsf", "fclose", "fcntl", "fdopen", "feof", "ferror", "fflush", "fgetc", "fgets", "fileno",
    "floor", "floorf", "fmod", "fmodf", "fopen", "fprintf", "fputc", "fputs", "fread", "free", "freeaddrinfo", "freeifaddrs", "frexp", "fscanf",
    "fseek", "fstat", "fsync", "ftell", "ftruncate", "fwrite", "gai_strerror", "getaddrinfo", "getc", "getchar", "getcwd", "getegid", "getenv",
    "geteuid", "getgid", "gethostbyname", "gethostname", "getifaddrs", "getopt", "getpagesize", "getpeername", "getpid", "getppid", "getrlimit",
    "getrusage", "getsockname", "getsockopt", "gettimeofday", "getuid", "glActiveTexture", "glAttachShader", "glBindTexture", "glBlendFunc",
    "glClear", "glClearColor", "glCreateProgram", "glCreateShader", "glDeleteProgram", "glDeleteShader", "glDeleteTextures", "glDrawArrays",
    "glEnable", "glEnableVertexAttribArray", "glGenTextures", "glGetActiveAttrib", "glGetActiveUniform", "glGetAttribLocation", "glGetError",
    "glGetProgramiv", "glGetString", "glGetUniformLocation", "glLinkProgram", "glPixelStorei", "glShaderBinary", "glTexImage2D", "glTexParameteri",
    "glUniform1f", "glUniform1i", "glUniformMatrix4fv", "glUseProgram", "glVertexAttribPointer", "glViewport", "gmtime", "gmtime_r", "htonl", "htons",
    "hypot", "hypotf", "inet_addr", "inet_ntoa", "inet_ntop", "inet_pton", "ioctl", "isalnum", "isalpha", "iscntrl", "isdigit", "isgraph", "islower",
    "isprint", "ispunct", "isspace", "isupper", "isxdigit", "kevent", "kill", "kqueue", "labs", "ldexp", "ldiv", "link", "listen", "llabs",
    "localtime", "localtime_r", "log", "log10", "log2", "log2f", "logf", "longjmp", "lrint", "lround", "lseek", "lstat", "madvise", "malloc",
    "malloc_usable_size", "memalign", "memchr", "memcmp", "memcpy", "memmove", "memset", "mkdir", "mktime", "mlock", "mmap", "modf", "mprotect",
    "msync", "munlock", "munmap", "nanosleep", "ntohl", "ntohs", "open", "opendir", "optarg", "perror", "pipe", "poll", "posix_memalign", "pow",
    "powf", "pread", "printf", "pthread_attr_destroy", "pthread_attr_init", "pthread_attr_setdetachstate", "pthread_attr_setstacksize",
    "pthread_cond_broadcast", "pthread_cond_destroy", "pthread_cond_init", "pthread_cond_signal", "pthread_cond_timedwait", "pthread_cond_wait",
    "pthread_create", "pthread_detach", "pthread_equal", "pthread_exit", "pthread_getschedparam", "pthread_getspecific", "pthread_join",
    "pthread_key_create", "pthread_mutex_destroy", "pthread_mutex_init", "pthread_mutex_lock", "pthread_mutex_trylock", "pthread_mutex_unlock",
    "pthread_mutexattr_destroy", "pthread_mutexattr_init", "pthread_mutexattr_settype", "pthread_once", "pthread_rwlock_destroy",
    "pthread_rwlock_init", "pthread_rwlock_rdlock", "pthread_rwlock_unlock", "pthread_rwlock_wrlock", "pthread_self", "pthread_setcanceltype",
    "pthread_setname_np", "pthread_setschedparam", "pthread_setspecific", "pthread_sigmask", "putc", "putchar", "putenv", "puts", "pwrite", "qsort",
    "raise", "rand", "read", "readdir", "readlink", "realloc", "reallocf", "recv", "recvfrom", "remove", "rename", "rewind", "rfork_thread", "rmdir",
    "round", "roundf", "scanf", "sceAppContentGetAddcontInfoList", "sceAppContentInitialize", "sceAppContentTerminate", "sceAudioOutClose",
    "sceAudioOutGetPortState", "sceAudioOutInit", "sceAudioOutOpen", "sceAudioOutOutput", "sceAudioOutOutputs", "sceAudioOutSetVolume",
    "sceCommonDialogInitialize", "sceCommonDialogIsUsed", "sceCommonDialogUpdateStatus", "sceFiosFHCloseSync", "sceFiosFHOpenSync",
    "sceFiosFHReadSync", "sceFiosFileGetSizeSync", "sceFiosFileReadSync", "sceFiosFileWriteSync", "sceFiosInitialize", "sceFiosTerminate",
    "sceGnmDispatchDirect", "sceGnmDrawIndex", "sceGnmDrawIndexAuto", "sceGnmFlushGarlic", "sceGnmInsertWaitFlipDone", "sceGnmSetCsShader",
    "sceGnmSubmitCommandBuffers", "sceGnmSubmitDone", "sceHttpCreateConnectionWithURL", "sceHttpCreateRequestWithURL", "sceHttpCreateTemplate",
    "sceHttpDeleteConnection", "sceHttpDeleteRequest", "sceHttpDeleteTemplate", "sceHttpGetResponseContentLength", "sceHttpGetStatusCode",
    "sceHttpInit", "sceHttpReadData", "sceHttpSendRequest", "sceHttpTerm", "sceHttpsSetSslCallback", "sceImeDialogGetResult", "sceImeDialogGetStatus",
    "sceImeDialogInit", "sceImeDialogTerm", "sceJpegDecCreate", "sceJpegDecDecode", "sceJpegDecDelete", "sceJpegDecMapMemory",
    "sceJpegDecParseHeader", "sceJpegDecQueryMemorySize", "sceJpegDecUnmapMemory", "sceKernelAddUserEvent", "sceKernelAllocateDirectMemory",
    "sceKernelAllocateMainDirectMemory", "sceKernelCheckReachability", "sceKernelClose", "sceKernelCreateEqueue", "sceKernelCreateEventFlag",
    "sceKernelCreateSema", "sceKernelDebugOutText", "sceKernelDeleteEqueue", "sceKernelDeleteEventFlag", "sceKernelDeleteSema", "sceKernelDlsym",
    "sceKernelExitProcess", "sceKernelFstat", "sceKernelGetCpuTemperature", "sceKernelGetCpumode", "sceKernelGetCurrentCpu",
    "sceKernelGetDirectMemorySize", "sceKernelGetFsSandboxRandomWord", "sceKernelGetModuleInfo", "sceKernelGetModuleList", "sceKernelGetOpenPsId",
    "sceKernelGetProcParam", "sceKernelGetProcessTime", "sceKernelGetProcessTimeCounter", "sceKernelGetProcessTimeCounterFrequency",
    "sceKernelGetSocPowerConsumption", "sceKernelGetTscFrequency", "sceKernelGetdents", "sceKernelIsNeoMode", "sceKernelLoadStartModule",
    "sceKernelLseek", "sceKernelMapDirectMemory", "sceKernelMapNamedDirectMemory", "sceKernelMkdir", "sceKernelMmap", "sceKernelMunmap",
    "sceKernelOpen", "sceKernelRead", "sceKernelReadTsc", "sceKernelReleaseDirectMemory", "sceKernelRename", "sceKernelRmdir",
    "sceKernelSendNotificationRequest", "sceKernelSetEventFlag", "sceKernelSetProcessName", "sceKernelSignalSema", "sceKernelSleep", "sceKernelStat",
    "sceKernelStopUnloadModule", "sceKernelUnlink", "sceKernelUsleep", "sceKernelVirtualQuery", "sceKernelWaitEqueue", "sceKernelWaitEventFlag",
    "sceKernelWaitSema", "sceKernelWrite", "sceKeyboardGetKey2Char", "sceKeyboardInit", "sceKeyboardOpen", "sceKeyboardReadState",
    "sceMsgDialogClose", "sceMsgDialogGetResult", "sceMsgDialogInitialize", "sceMsgDialogOpen", "sceMsgDialogTerminate", "sceMsgDialogUpdateStatus",
    "sceNetAccept", "sceNetBind", "sceNetConnect", "sceNetCtlGetInfo", "sceNetCtlGetState", "sceNetCtlInit", "sceNetCtlTerm", "sceNetEpollCreate",
    "sceNetErrnoLoc", "sceNetGetsockname", "sceNetGetsockopt", "sceNetHtonl", "sceNetHtons", "sceNetInetNtop", "sceNetInetPton", "sceNetInit",
    "sceNetListen", "sceNetNtohl", "sceNetNtohs", "sceNetPoolCreate", "sceNetPoolDestroy", "sceNetRecv", "sceNetRecvfrom", "sceNetResolverCreate",
    "sceNetResolverDestroy", "sceNetResolverStartNtoa", "sceNetSend", "sceNetSendto", "sceNetSetsockopt", "sceNetShutdown", "sceNetSocket",
    "sceNetSocketClose", "sceNetTerm", "sceNpInit", "sceNpTerm", "sceNpTrophyCreateContext", "sceNpTrophyCreateHandle", "sceNpTrophyDestroyContext",
    "sceNpTrophyDestroyHandle", "sceNpTrophyRegisterContext", "sceNpTrophyUnlockTrophy", "scePadClose", "scePadGetControllerInformation",
    "scePadGetHandle", "scePadInit", "scePadOpen", "scePadRead", "scePadReadHistory", "scePadReadState", "scePadResetLightBar", "scePadSetLightBar",
    "scePadSetMotionSensorState", "scePadSetVibration", "scePigletSetConfigurationVSH", "scePrecompiledShaderEntries", "scePthreadAttrDestroy",
    "scePthreadAttrInit", "scePthreadAttrSetdetachstate", "scePthreadAttrSetstacksize", "scePthreadCondBroadcast", "scePthreadCondDestroy",
    "scePthreadCondInit", "scePthreadCondSignal", "scePthreadCondTimedwait", "scePthreadCondWait", "scePthreadCreate", "scePthreadDetach",
    "scePthreadExit", "scePthreadGetaffinity", "scePthreadGetprio", "scePthreadGetspecific", "scePthreadJoin", "scePthreadKeyCreate",
    "scePthreadMutexDestroy", "scePthreadMutexInit", "scePthreadMutexLock", "scePthreadMutexTrylock", "scePthreadMutexUnlock",
    "scePthreadMutexattrDestroy", "scePthreadMutexattrInit", "scePthreadMutexattrSettype", "scePthreadOnce", "scePthreadRename", "scePthreadSelf",
    "scePthreadSetaffinity", "scePthreadSetprio", "scePthreadSetspecific", "scePthreadYield", "sceRtcGetCurrentClock",
    "sceRtcGetCurrentClockLocalTime", "sceRtcGetCurrentTick", "sceRtcGetTick", "sceRtcGetTime_t", "sceRtcSetTime_t", "sceRtcTickAddMinutes",
    "sceSaveDataInitialize3", "sceSaveDataMount2", "sceSaveDataSetParam", "sceSaveDataTerminate", "sceSaveDataUmount", "sceSslCreateSslConnection",
    "sceSslCreateSslContext", "sceSslDeleteSslConnection", "sceSslDeleteSslContext", "sceSslInit", "sceSslTerm", "sceSysmoduleIsLoaded",
    "sceSysmoduleLoadModule", "sceSysmoduleLoadModuleInternal", "sceSysmoduleLoadModuleInternalWithArg", "sceSysmoduleUnloadModule",
    "sceSystemServiceDisableSuspendNotification", "sceSystemServiceGetAppIdOfRunningBigApp", "sceSystemServiceGetAppVersion",
    "sceSystemServiceGetDisplaySafeAreaInfo", "sceSystemServiceGetStatus", "sceSystemServiceHideSplashScreen", "sceSystemServiceKillApp",
    "sceSystemServiceLaunchApp", "sceSystemServiceLoadExec", "sceSystemServiceParamGetInt", "sceSystemServiceParamGetString",
    "sceSystemServiceReceiveEvent", "sceUserServiceGetEvent", "sceUserServiceGetInitialUser", "sceUserServiceGetLoginUserIdList",
    "sceUserServiceGetUserName", "sceUserServiceInitialize", "sceUserServiceTerminate", "sceVideoOutAddFlipEvent", "sceVideoOutAddVblankEvent",
    "sceVideoOutClose", "sceVideoOutGetFlipStatus", "sceVideoOutGetResolutionStatus", "sceVideoOutIsFlipPending", "sceVideoOutOpen",
    "sceVideoOutRegisterBuffers", "sceVideoOutSetBufferAttribute", "sceVideoOutSetFlipRate", "sceVideoOutSetWindowModeMargins",
    "sceVideoOutSubmitFlip", "sceVideoOutUnregisterBuffers", "sceVideoOutWaitVblank", "sched_get_priority_max", "sched_get_priority_min",
    "sched_yield", "select", "sem_destroy", "sem_getvalue", "sem_init", "sem_post", "sem_trywait", "sem_wait", "send", "sendfile", "sendto", "setenv",
    "setjmp", "setsockopt", "setvbuf", "shutdown", "sigaction", "sigaddset", "sigdelset", "sigemptyset", "sigfillset", "signal", "sigprocmask",
    "sigsuspend", "sigwait", "sin", "sinf", "sinh", "sleep", "snprintf", "socket", "sprintf", "sqrt", "sqrtf", "srand", "sscanf", "stat",
    "strcasecmp", "strcat", "strchr", "strcmp", "strcpy", "strcspn", "strdup", "strerror", "strerror_r", "strftime", "strlen", "strncasecmp",
    "strncat", "strncmp", "strncpy", "strndup", "strnlen", "strpbrk", "strrchr", "strspn", "strstr", "strtod", "strtof", "strtok", "strtok_r",
    "strtol", "strtoll", "strtoul", "strtoull", "symlink", "sysconf", "sysctl", "system", "tan", "tanf", "tanh", "time", "tmpfile", "tolower",
    "toupper", "trunc", "truncate", "truncf", "ungetc", "unlink", "unsetenv", "usleep", "utimes", "vfprintf", "vprintf", "vsnprintf", "vsprintf",
    "vsscanf", "waitpid", "write",
};

const std::unordered_map<std::string, std::string>& by_nid() {
    static const std::unordered_map<std::string, std::string> table = [] {
        std::unordered_map<std::string, std::string> t;
        for (const char* name : kNames) t.emplace(nid_of(name), name);
        return t;
    }();
    return table;
}

}  // namespace

std::string nid_of(const std::string& name) {
    Bytes input(name.begin(), name.end());
    input.insert(input.end(), std::begin(kSalt), std::end(kSalt));
    const crypto::Sha1Digest digest = crypto::sha1(input);
    static constexpr char kAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
    // the first eight bytes, reversed, as base 64: eleven digits (the last holds four bits)
    std::array<std::uint8_t, 8> reversed{};
    for (std::size_t k = 0; k < 8; ++k) reversed[k] = digest[7 - k];
    std::string out;
    for (std::size_t k = 0; k < 8; k += 3) {
        const std::uint32_t b0 = reversed[k], b1 = k + 1 < 8 ? reversed[k + 1] : 0, b2 = k + 2 < 8 ? reversed[k + 2] : 0;
        const std::uint32_t triple = b0 << 16 | b1 << 8 | b2;
        const std::size_t digits = k + 2 < 8 ? 4 : 3;   // the last group has two bytes: three digits
        for (std::size_t d = 0; d < digits; ++d) out += kAlphabet[(triple >> (18 - 6 * d)) & 63];
    }
    return out;
}

std::string function_name(const std::string& nid) {
    const auto& t = by_nid();
    const auto it = t.find(nid);
    return it == t.end() ? std::string() : it->second;
}

std::string syscall_name(std::uint32_t number) {
    static const std::unordered_map<std::uint32_t, const char*> table = {
        {1, "exit"}, {2, "fork"}, {3, "read"}, {4, "write"}, {5, "open"}, {6, "close"}, {7, "wait4"}, {9, "link"}, {10, "unlink"}, {12, "chdir"}, {15, "chmod"},
        {16, "chown"}, {20, "getpid"}, {23, "setuid"}, {24, "getuid"}, {33, "access"}, {37, "kill"}, {39, "getppid"}, {41, "dup"}, {42, "pipe"}, {46, "sigaction"},
        {47, "getgid"}, {54, "ioctl"}, {58, "readlink"}, {59, "execve"}, {73, "munmap"}, {74, "mprotect"}, {75, "madvise"}, {78, "mincore"}, {83, "setitimer"},
        {90, "dup2"}, {92, "fcntl"}, {93, "select"}, {95, "fsync"}, {97, "socket"}, {98, "connect"}, {99, "accept"}, {104, "bind"}, {105, "setsockopt"},
        {106, "listen"}, {116, "gettimeofday"}, {117, "getrusage"}, {118, "getsockopt"}, {120, "readv"}, {121, "writev"}, {128, "rename"}, {133, "sendto"},
        {136, "mkdir"}, {137, "rmdir"}, {165, "sysarch"}, {188, "stat"}, {189, "fstat"}, {190, "lstat"}, {194, "getrlimit"}, {195, "setrlimit"}, {196, "getdirentries"},
        {199, "lseek"}, {202, "__sysctl"}, {203, "mlock"}, {204, "munlock"}, {232, "clock_gettime"}, {233, "clock_settime"}, {234, "clock_getres"}, {240, "nanosleep"},
        {362, "kqueue"}, {363, "kevent"}, {431, "thr_exit"}, {432, "thr_self"}, {433, "thr_kill"}, {454, "_umtx_op"}, {455, "thr_new"}, {475, "pread"}, {476, "pwrite"},
        {477, "mmap"}, {478, "lseek"}, {479, "truncate"}, {480, "ftruncate"}, {481, "thr_kill2"}, {482, "shm_open"}, {483, "shm_unlink"}, {499, "rtprio_thread"}};
    const auto it = table.find(number);
    return it == table.end() ? std::string() : std::string(it->second);
}

}  // namespace cb::formats::orbis
