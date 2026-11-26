// dotnet/decompiler/util/cancellation.hpp -- a cancellation token (System.Threading.CancellationToken, as ILSpy uses it): a flag another
// thread may raise; the work checks it between steps and stops with OperationCanceledError.
#pragma once

#include <atomic>

#include "core/error.hpp"

namespace cb::dotnet::decompiler::util {

class OperationCanceledError : public Error {
public:
    OperationCanceledError() : Error("the operation was canceled") {}
    CB_ERROR_BOILERPLATE(OperationCanceledError)
};

class CancellationToken {
public:
    CancellationToken() = default;   // never canceled (CancellationToken.None)
    explicit CancellationToken(const std::atomic<bool>* flag) : flag_(flag) {}
    bool is_cancellation_requested() const { return flag_ != nullptr && flag_->load(std::memory_order_relaxed); }
    void throw_if_cancellation_requested() const {
        if (is_cancellation_requested()) throw OperationCanceledError();
    }

private:
    const std::atomic<bool>* flag_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::util
