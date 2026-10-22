// rex/logging.h - SHIM for the vendored DXBC translator.
//
// The real logging layer is built on spdlog + fmt sinks. The shader translator
// only emits diagnostic log lines through the REXLOG_* macros; for the vendored
// standalone build we compile them out (no spdlog dependency). Every macro
// discards its arguments and evaluates to a no-op.
#pragma once
#include <cstdint>

// category definition/declaration -> nothing
#define REXLOG_DEFINE_CATEGORY(...)
#define REXLOG_DECLARE_CATEGORY(...)

// plain level macros
#define REXLOG_TRACE(...)     ((void)0)
#define REXLOG_DEBUG(...)     ((void)0)
#define REXLOG_INFO(...)      ((void)0)
#define REXLOG_WARN(...)      ((void)0)
#define REXLOG_ERROR(...)     ((void)0)
#define REXLOG_CRITICAL(...)  ((void)0)

// category-scoped
#define REXLOG_CAT_TRACE(...)     ((void)0)
#define REXLOG_CAT_DEBUG(...)     ((void)0)
#define REXLOG_CAT_INFO(...)      ((void)0)
#define REXLOG_CAT_WARN(...)      ((void)0)
#define REXLOG_CAT_ERROR(...)     ((void)0)
#define REXLOG_CAT_CRITICAL(...)  ((void)0)

// category + thread-id
#define REXLOG_CAT_TID_TRACE(...)     ((void)0)
#define REXLOG_CAT_TID_DEBUG(...)     ((void)0)
#define REXLOG_CAT_TID_INFO(...)      ((void)0)
#define REXLOG_CAT_TID_WARN(...)      ((void)0)
#define REXLOG_CAT_TID_ERROR(...)     ((void)0)
#define REXLOG_CAT_TID_CRITICAL(...)  ((void)0)

// category + function-name
#define REXLOG_CAT_FN_TRACE(...)     ((void)0)
#define REXLOG_CAT_FN_DEBUG(...)     ((void)0)
#define REXLOG_CAT_FN_INFO(...)      ((void)0)
#define REXLOG_CAT_FN_WARN(...)      ((void)0)
#define REXLOG_CAT_FN_ERROR(...)     ((void)0)
#define REXLOG_CAT_FN_CRITICAL(...)  ((void)0)

// per-subsystem category macros (REXGPU_*, REXCPU_*, ... - all compiled out)
#define REX_LOG_CATEGORY_NOOP(PREFIX)         \
  /* PREFIX##_LEVEL(fmt, ...) */
#define REXAPU_TRACE(...)       ((void)0)
#define REXAPU_DEBUG(...)       ((void)0)
#define REXAPU_INFO(...)        ((void)0)
#define REXAPU_WARN(...)        ((void)0)
#define REXAPU_ERROR(...)       ((void)0)
#define REXAPU_CRITICAL(...)    ((void)0)
#define REXCPU_TRACE(...)       ((void)0)
#define REXCPU_DEBUG(...)       ((void)0)
#define REXCPU_INFO(...)        ((void)0)
#define REXCPU_WARN(...)        ((void)0)
#define REXCPU_ERROR(...)       ((void)0)
#define REXCPU_CRITICAL(...)    ((void)0)
#define REXGPU_TRACE(...)       ((void)0)
#define REXGPU_DEBUG(...)       ((void)0)
#define REXGPU_INFO(...)        ((void)0)
#define REXGPU_WARN(...)        ((void)0)
#define REXGPU_ERROR(...)       ((void)0)
#define REXGPU_CRITICAL(...)    ((void)0)
#define REXFS_TRACE(...)        ((void)0)
#define REXFS_DEBUG(...)        ((void)0)
#define REXFS_INFO(...)         ((void)0)
#define REXFS_WARN(...)         ((void)0)
#define REXFS_ERROR(...)        ((void)0)
#define REXFS_CRITICAL(...)     ((void)0)
#define REXKRNL_TRACE(...)      ((void)0)
#define REXKRNL_DEBUG(...)      ((void)0)
#define REXKRNL_INFO(...)       ((void)0)
#define REXKRNL_WARN(...)       ((void)0)
#define REXKRNL_ERROR(...)      ((void)0)
#define REXKRNL_CRITICAL(...)   ((void)0)
#define REXKRNLFN_TRACE(...)    ((void)0)
#define REXKRNLFN_DEBUG(...)    ((void)0)
#define REXKRNLFN_INFO(...)     ((void)0)
#define REXKRNLFN_WARN(...)     ((void)0)
#define REXKRNLFN_ERROR(...)    ((void)0)
#define REXKRNLFN_CRITICAL(...) ((void)0)
#define REXLOGFN_TRACE(...)     ((void)0)
#define REXLOGFN_DEBUG(...)     ((void)0)
#define REXLOGFN_INFO(...)      ((void)0)
#define REXLOGFN_WARN(...)      ((void)0)
#define REXLOGFN_ERROR(...)     ((void)0)
#define REXLOGFN_CRITICAL(...)  ((void)0)
#define REXSYS_TRACE(...)       ((void)0)
#define REXSYS_DEBUG(...)       ((void)0)
#define REXSYS_INFO(...)        ((void)0)
#define REXSYS_WARN(...)        ((void)0)
#define REXSYS_ERROR(...)       ((void)0)
#define REXSYS_CRITICAL(...)    ((void)0)
#define REXCODEGEN_TRACE(...)   ((void)0)
#define REXCODEGEN_DEBUG(...)   ((void)0)
#define REXCODEGEN_INFO(...)    ((void)0)
#define REXCODEGEN_WARN(...)    ((void)0)
#define REXCODEGEN_ERROR(...)   ((void)0)
#define REXCODEGEN_CRITICAL(...) ((void)0)
