/**
 * @file        runtime/runtime.cpp
 * @brief       Runtime subsystem implementation
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#include <fstream>

#include <rex/chrono/clock.h>
#include <rex/filesystem/devices/host_path_device.h>
#include <rex/filesystem/devices/null_device.h>
#include <rex/filesystem/vfs.h>
#include <rex/logging.h>
#include <rex/perf/counter.h>
#include <rex/ppc/context.h>     // PPCFuncMapping
#include <rex/ppc/exceptions.h>  // SEH exception support
#include <rex/kernel/crt/heap.h>
#include <rex/runtime.h>
#include <rex/system/export_resolver.h>
#include <rex/system/kernel_state.h>
#include <rex/system/function_dispatcher.h>
#include <rex/system/user_module.h>
#include <rex/system/xex_module.h>
#include <rex/system/xmemory.h>
#include <rex/system/xthread.h>
#include <rex/thread.h>
#include <rex/types.h>  // rex::be

namespace rex {

// Static instance for global access
Runtime* Runtime::instance_ = nullptr;

Runtime::Runtime(const std::filesystem::path& game_data_root,
                 const std::filesystem::path& user_data_root,
                 const std::filesystem::path& update_data_root)
    : game_data_root_(game_data_root),
      user_data_root_(user_data_root.empty() ? game_data_root : user_data_root),
      update_data_root_(update_data_root) {}

Runtime::~Runtime() {
  Shutdown();
}

X_STATUS Runtime::Setup(RuntimeConfig config) {
  // Start profiler (Tracy network threads, counter init)
  rex::perf::Profiler::Startup();

  // Initialize SEH exception support for hardware exception handling
  rex::initialize_seh();

  // Initialize clock
  chrono::Clock::set_guest_tick_frequency(50000000);
  chrono::Clock::set_guest_system_time_base(chrono::Clock::QueryHostSystemTime());
  chrono::Clock::set_guest_time_scalar(1.0);

  // Enable threading affinity configuration
  thread::EnableAffinityConfiguration();

  // Guard against reinitialization
  if (memory_) {
    REXSYS_ERROR("Runtime::Setup() called but already initialized");
    return X_STATUS_UNSUCCESSFUL;
  }

  tool_mode_ = config.tool_mode;

  // Create memory system first
  memory_ = std::make_unique<memory::Memory>();
  if (!memory_->Initialize()) {
    REXSYS_ERROR("Failed to initialize memory system");
    memory_.reset();
    return X_STATUS_UNSUCCESSFUL;
  }

  export_resolver_ = std::make_unique<runtime::ExportResolver>();

  function_dispatcher_ =
      std::make_unique<runtime::FunctionDispatcher>(memory_.get(), export_resolver_.get());
  REXSYS_INFO("FunctionDispatcher initialized");

  // Create virtual file system
  file_system_ = std::make_unique<rex::filesystem::VirtualFileSystem>();

  // Create kernel state - this sets the global singleton
  kernel_state_ = std::make_unique<system::KernelState>(this);

  // Initialize input from injected config
  if (config.input_factory) {
    input_system_ = config.input_factory(tool_mode_);
    if (input_system_) {
      X_STATUS input_status = input_system_->Setup();
      if (XFAILED(input_status)) {
        REXSYS_WARN("Failed to initialize input system (status {:08X}) - input disabled",
                    input_status);
        input_system_.reset();
      } else {
        REXSYS_INFO("Input system initialized");
      }
    }
  }

  // HLE kernel modules and apps.
  if (config.kernel_init) {
    config.kernel_init(this, kernel_state_.get());
  }

  // Initialize the APU (Audio Processing Unit) from injected config
  if (config.audio_factory) {
    audio_system_ = config.audio_factory(function_dispatcher_.get());
    if (audio_system_) {
      X_STATUS audio_status = audio_system_->Setup(kernel_state_.get());
      if (XFAILED(audio_status)) {
        REXSYS_WARN("Failed to initialize audio system (status {:08X}) - audio disabled",
                    audio_status);
        audio_system_.reset();
      } else {
        REXSYS_INFO("Audio system initialized");
      }
    }
  }

  // Set up VFS: game_data_root as game:/d:, update_data_root as update:
  if (!SetupVfs()) {
    REXSYS_ERROR("Failed to set up VFS");
    return X_STATUS_UNSUCCESSFUL;
  }

  // Skip GPU initialization in tool mode (for analysis tools like codegen)
  if (tool_mode_) {
    REXSYS_INFO("Runtime initialized in tool mode (no GPU)");
    return X_STATUS_SUCCESS;
  }

  // Initialize GPU from injected config
  if (config.graphics) {
    graphics_system_ = std::move(config.graphics);
    bool with_presentation = (app_context_ != nullptr);
    X_STATUS gpu_status = graphics_system_->Setup(function_dispatcher_.get(), kernel_state_.get(),
                                                  app_context_, with_presentation);
    if (XFAILED(gpu_status)) {
      REXSYS_ERROR("Failed to initialize GPU - required for runtime");
      graphics_system_.reset();
      return gpu_status;
    }
    REXSYS_INFO("GPU system initialized (presentation={})", with_presentation);
  }

  REXSYS_INFO("Runtime initialized successfully");
  return X_STATUS_SUCCESS;
}

X_STATUS Runtime::Setup(uint32_t code_base, uint32_t code_size, uint32_t image_base,
                        uint32_t image_size, const PPCFuncMapping* func_mappings,
                        RuntimeConfig config) {
  // Guard against multiple singleton instances
  if (instance_ != nullptr) {
    REXSYS_ERROR("Runtime::Setup() called but global instance already exists");
    return X_STATUS_UNSUCCESSFUL;
  }

  // First perform the basic setup with injected config
  X_STATUS status = Setup(std::move(config));
  if (status != X_STATUS_SUCCESS) {
    return status;
  }

  // Initialize function table in FunctionDispatcher for recompiled code dispatch
  if (!function_dispatcher_->InitializeFunctionTable(code_base, code_size, image_base,
                                                     image_size)) {
    REXSYS_ERROR("Failed to initialize function table");
    return X_STATUS_UNSUCCESSFUL;
  }

  // Register all recompiled functions from the mapping table
  if (func_mappings) {
    int count = 0;
    for (int i = 0; func_mappings[i].guest != 0; ++i) {
      if (func_mappings[i].host != nullptr) {
        function_dispatcher_->SetFunction(static_cast<uint32_t>(func_mappings[i].guest),
                                          func_mappings[i].host);
        ++count;
      }
    }
    REXSYS_DEBUG("Registered {} recompiled functions", count);
  }

  // Set the global instance for recompiled code access
  instance_ = this;

  REXSYS_DEBUG("Runtime setup for recompiled code complete (code: {:08X}-{:08X})", code_base,
               code_base + code_size);
  return X_STATUS_SUCCESS;
}

void Runtime::Shutdown() {
  // Clear global instance
  if (instance_ == this) {
    instance_ = nullptr;
  }

  // Destroy in reverse order
  if (graphics_system_) {
    graphics_system_->Shutdown();
    graphics_system_.reset();
  }
  if (audio_system_) {
    audio_system_->Shutdown();
    audio_system_.reset();
  }
  if (input_system_) {
    input_system_->Shutdown();
    input_system_.reset();
  }
  kernel_state_.reset();
  function_dispatcher_.reset();
  export_resolver_.reset();
  file_system_.reset();
  memory_.reset();

  rex::perf::Profiler::Shutdown();
}

uint8_t* Runtime::virtual_membase() const {
  return memory_ ? memory_->virtual_membase() : nullptr;
}

bool Runtime::SetupVfs() {
  if (game_data_root_.empty()) {
    REXSYS_WARN("Runtime::SetupVfs: No game_data_root specified, skipping VFS setup");
    return true;
  }

  auto abs_game_root = std::filesystem::absolute(game_data_root_);
  if (!std::filesystem::exists(abs_game_root)) {
    REXSYS_ERROR("Runtime::SetupVfs: game_data_root does not exist: {}", abs_game_root.string());
    return false;
  }

  // Mount game_data_root as \Device\Harddisk0\Partition1
  auto mount_path = "\\Device\\Harddisk0\\Partition1";
  auto device = std::make_unique<rex::filesystem::HostPathDevice>(
      mount_path, abs_game_root, !REXCVAR_GET(allow_game_relative_writes));
  if (!device->Initialize()) {
    REXSYS_ERROR("Runtime::SetupVfs: Failed to initialize host path device");
    return false;
  }
  if (!file_system_->RegisterDevice(std::move(device))) {
    REXSYS_ERROR("Runtime::SetupVfs: Failed to register host path device");
    return false;
  }
  REXSYS_INFO("  Mounted {} at {}", abs_game_root.string(), mount_path);

  // Register symbolic links for game: and D:
  file_system_->RegisterSymbolicLink("game:", mount_path);
  file_system_->RegisterSymbolicLink("d:", mount_path);
  REXSYS_DEBUG("  Registered symbolic links: game:, d:");

  // Mount update_data_root as update:\ if provided
  if (!update_data_root_.empty()) {
    auto abs_update_root = std::filesystem::absolute(update_data_root_);
    if (std::filesystem::exists(abs_update_root)) {
      auto update_mount = "\\Device\\Harddisk0\\PartitionUpdate";
      auto update_device =
          std::make_unique<rex::filesystem::HostPathDevice>(update_mount, abs_update_root, true);
      if (update_device->Initialize() && file_system_->RegisterDevice(std::move(update_device))) {
        file_system_->RegisterSymbolicLink("update:", update_mount);
        REXSYS_INFO("  Mounted {} at update:", abs_update_root.string());
      }
    }
  }

  // Setup NullDevice for raw HDD partition accesses
  // Cache/STFC code baked into games tries reading/writing to these
  // Using a NullDevice returns success to all IO requests, allowing games
  // to believe cache/raw disk was accessed successfully.
  // NOTE: Must be registered AFTER Partition1 so Partition1 requests don't
  // go to NullDevice (VFS resolves devices in registration order)
  auto null_paths = {std::string("\\Partition0"), std::string("\\Cache0"), std::string("\\Cache1")};
  auto null_device =
      std::make_unique<rex::filesystem::NullDevice>("\\Device\\Harddisk0", null_paths);
  if (null_device->Initialize()) {
    file_system_->RegisterDevice(std::move(null_device));
    REXSYS_DEBUG("  Registered NullDevice for \\Device\\Harddisk0\\{{Partition0,Cache0,Cache1}}");
  }

  // NOTE: Do NOT register a device for cache: paths
  // Games handle "device not found" gracefully but don't handle actual device
  // errors (like NAME_COLLISION) well. Let cache: fail cleanly.

  return true;
}

X_STATUS Runtime::LoadXexImage(const std::string_view module_path) {
  REXSYS_INFO("Loading XEX image: {}", std::string(module_path));

  auto module = system::object_ref<system::UserModule>(new system::UserModule(kernel_state_.get()));
  X_STATUS status = module->LoadFromFile(module_path);
  if (XFAILED(status)) {
    REXSYS_ERROR("Runtime::LoadXexImage: Failed to load module, status {:08X}", status);
    return status;
  }

  kernel_state_->SetExecutableModule(module);
  REXSYS_DEBUG("  XEX image loaded successfully");
  return X_STATUS_SUCCESS;
}

X_STATUS Runtime::LoadXexImageFromMemory(const void* data, size_t size) {
  REXSYS_INFO("Loading XEX image from embedded memory ({} bytes)", size);

  auto module = system::object_ref<system::UserModule>(new system::UserModule(kernel_state_.get()));
  X_STATUS status = module->LoadFromMemoryImage(data, size, "default.xex");
  if (XFAILED(status)) {
    REXSYS_ERROR("Runtime::LoadXexImageFromMemory: Failed to load module, status {:08X}", status);
    return status;
  }

  kernel_state_->SetExecutableModule(module);
  REXSYS_DEBUG("  XEX image loaded successfully (from embedded memory)");
  return X_STATUS_SUCCESS;
}

X_STATUS Runtime::LoadNativeImage(const rex::runtime::NativeImageDesc& desc) {
  REXSYS_INFO(
      "Loading native image: base {:08X}, size {:08X}, entry {:08X} ({} image bytes, "
      "{} variable imports, {} protection ranges)",
      desc.base, desc.image_size, desc.entry_point, desc.image_bytes, desc.import_count,
      desc.protection_count);

  // 1) Allocate the image region and copy the pre-decrypted bytes in.
  //    (Mirrors XexModule::ReadImageUncompressed.)
  auto* heap = memory_->LookupHeap(desc.base);
  bool alloc_result = heap->AllocFixed(
      desc.base, desc.image_size, 4096,
      rex::memory::kMemoryAllocationReserve | rex::memory::kMemoryAllocationCommit,
      rex::memory::kMemoryProtectRead | rex::memory::kMemoryProtectWrite);
  if (!alloc_result) {
    REXSYS_ERROR("Runtime::LoadNativeImage: Unable to allocate image memory at {:08X}-{:08X}",
                 desc.base, desc.base + desc.image_size);
    return X_STATUS_UNSUCCESSFUL;
  }
  uint8_t* buffer = memory_->TranslateVirtual(desc.base);
  std::memset(buffer, 0, desc.image_size);
  std::memcpy(buffer, desc.image, desc.image_bytes);

  // 2) Patch variable imports in guest memory. (Mirrors
  //    XexModule::SetupLibraryImports's variable branch.)
  auto* export_resolver = export_resolver_.get();
  uint32_t patched = 0;
  if (export_resolver) {
    for (uint32_t i = 0; i < desc.import_count; ++i) {
      const auto& imp = desc.imports[i];
      auto* exp = export_resolver->GetExportByOrdinal(imp.lib, imp.ordinal);
      if (exp && exp->type == rex::runtime::Export::Type::kVariable) {
        auto* p = memory_->TranslateVirtual<rex::be<uint32_t>*>(imp.slot);
        if (exp->is_implemented() && exp->variable_ptr) {
          *p = exp->variable_ptr;
        } else {
          *p = 0xD000BEEF | ((exp->ordinal & 0xFFF) << 16);
        }
        ++patched;
      }
      // Non-kVariable exports: leave the slot untouched.
    }
  }

  // 3) Apply page protections. (Mirrors XexModule::LoadContinue's protection
  //    loop.)
  for (uint32_t i = 0; i < desc.protection_count; ++i) {
    const auto& prot = desc.protections[i];
    heap->Protect(prot.addr, prot.size,
                  prot.writable
                      ? (rex::memory::kMemoryProtectRead | rex::memory::kMemoryProtectWrite)
                      : rex::memory::kMemoryProtectRead);
  }

  // 4) Wrap the image in a UserModule (backed by a NativeImageModule) and set it
  //    as the executable module, mirroring the XEX/ELF paths.
  auto module = system::object_ref<system::UserModule>(new system::UserModule(kernel_state_.get()));
  X_STATUS status = module->LoadNative(desc.base, desc.image_size, desc.entry_point, desc.xex_header,
                                       desc.xex_header_size, desc.full_image_size);
  if (XFAILED(status)) {
    REXSYS_ERROR("Runtime::LoadNativeImage: Failed to load module, status {:08X}", status);
    return status;
  }

  kernel_state_->SetExecutableModule(module);
  REXSYS_INFO("  Native image loaded successfully ({} variable imports patched)", patched);
  return X_STATUS_SUCCESS;
}

X_STATUS Runtime::DumpNativeImagePackage(const std::filesystem::path& out_dir) {
  auto exe = kernel_state_->GetExecutableModule();
  if (!exe) {
    REXSYS_ERROR("DumpNativeImagePackage: no executable module loaded");
    return X_STATUS_UNSUCCESSFUL;
  }
  auto* xex = exe->xex_module();
  if (!xex) {
    REXSYS_ERROR("DumpNativeImagePackage: executable module is not a XEX");
    return X_STATUS_UNSUCCESSFUL;
  }

  std::error_code ec;
  std::filesystem::create_directories(out_dir, ec);

  const uint32_t base = xex->base_address();
  const uint32_t size = xex->image_size();
  const uint32_t entry = xex->entry_point();
  const uint8_t* img = memory_->TranslateVirtual(base);

  // 1) Raw decompressed image. The variable-import slots inside it are re-patched
  //    by the native loader at boot, so their stale values here don't matter.
  {
    std::ofstream f(out_dir / "image.bin", std::ios::binary);
    f.write(reinterpret_cast<const char*>(img), size);
  }

  // 1b) The plaintext XEX header (a few KB). The guest's CRT reads this at
  //     startup via RtlImageXexHeaderField, so the native loader must map it
  //     into guest memory and point ldr_data->xex_header_base at it.
  const auto* xex_hdr = xex->xex_header();
  const uint32_t header_size = xex_hdr->header_size;
  const uint32_t full_image_size = xex->xex_security_info()->image_size;
  {
    std::ofstream f(out_dir / "xex_header.bin", std::ios::binary);
    f.write(reinterpret_cast<const char*>(xex_hdr), header_size);
  }

  // 2) native_image_data.h -- the metadata the native loader replays.
  std::ofstream h(out_dir / "native_image_data.h");
  h << std::hex;
  h << "// Auto-generated by UP_BAKE_NATIVE -- the data the native loader replays\n";
  h << "// instead of parsing a XEX. image.bin (next to this file) is the image.\n";
  h << "#pragma once\n#include <cstdint>\n\nnamespace up_native {\n\n";
  h << "inline constexpr uint32_t kImageBase     = 0x" << base << ";\n";
  h << "inline constexpr uint32_t kImageSize     = 0x" << size << ";\n";
  h << "inline constexpr uint32_t kEntryPoint    = 0x" << entry << ";\n";
  h << "inline constexpr uint32_t kXexHeaderSize = 0x" << header_size << ";\n";
  h << "inline constexpr uint32_t kFullImageSize = 0x" << full_image_size << ";\n\n";

  // Variable imports: data pointers the recompiled code reads from guest memory.
  h << "struct VarImport { uint32_t slot; const char* lib; uint16_t ordinal; };\n";
  h << "inline constexpr VarImport kVarImports[] = {\n";
  uint32_t var_count = 0;
  for (const auto& lib : *xex->import_libraries()) {
    for (const auto& imp : lib.imports) {
      if (imp.value_address) {
        h << "    { 0x" << imp.value_address << ", \"" << lib.name << "\", 0x" << imp.ordinal
          << " },\n";
        ++var_count;
      }
    }
  }
  h << "};\n";

  // Memory protections, from the XEX page descriptors.
  h << "struct ProtRange { uint32_t addr; uint32_t size; uint8_t writable; };\n";
  h << "inline constexpr ProtRange kProtRanges[] = {\n";
  const auto* si = xex->xex_security_info();
  auto* heap = memory_->LookupHeap(base);
  const uint32_t page_size = heap->page_size();
  uint32_t prot_count = 0;
  for (uint32_t i = 0, page = 0; i < si->page_descriptor_count; ++i) {
    xex2_page_descriptor desc;
    desc.value = rex::byte_swap(si->page_descriptors[i].value);
    const uint32_t addr = base + page * page_size;
    const uint32_t sz = desc.page_count * page_size;
    int writable = -1;
    if (desc.info == XEX_SECTION_CODE || desc.info == XEX_SECTION_READONLY_DATA) {
      writable = 0;
    } else if (desc.info == XEX_SECTION_DATA) {
      writable = 1;
    }
    if (writable >= 0) {
      h << "    { 0x" << addr << ", 0x" << sz << ", " << writable << " },\n";
      ++prot_count;
    }
    page += desc.page_count;
  }
  h << "};\n";
  h << std::dec;
  h << "inline constexpr uint32_t kVarImportCount  = " << var_count << ";\n";
  h << "inline constexpr uint32_t kProtRangeCount  = " << prot_count << ";\n\n";
  h << "}  // namespace up_native\n";

  REXSYS_INFO(
      "Baked native image: {} bytes, {} variable imports, {} protection ranges, "
      "entry {:08X} -> {}",
      size, var_count, prot_count, entry, out_dir.string());
  return X_STATUS_SUCCESS;
}

system::object_ref<system::XThread> Runtime::LaunchModule() {
  auto executable = kernel_state_->GetExecutableModule();
  if (!executable) {
    REXSYS_ERROR("Runtime::LaunchModule: No executable module loaded");
    return nullptr;
  }

  auto thread = kernel_state_->LaunchModule(executable);
  if (!thread) {
    REXSYS_ERROR("Runtime::LaunchModule: Failed to launch module");
    return nullptr;
  }

  REXSYS_DEBUG("  Module launched on thread '{}'", thread->name());
  return thread;
}

}  // namespace rex
