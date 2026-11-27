// dotnet/decompiler/util/arithmetic_error.hpp -- the exceptions of arithmetic on constants: .NET's ArithmeticException, OverflowException
// and DivideByZeroException, as util::Decimal, csharp_primitive_cast and the C# operators throw them.
//
//     try { util::Decimal::divide(a, b); } catch (const util::ArithmeticError&) { /* no result: overflow or division by zero */ }
//
// Written for the ILSpy port from .NET's description of the three exception types (the messages are .NET's).
#pragma once

#include <stdexcept>

namespace cb::dotnet::decompiler::util {

// System.ArithmeticException: an operation on constants that has no result.
class ArithmeticError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};
// System.OverflowException: an operation overflowed (a checked one, or any decimal one).
class OverflowError : public ArithmeticError {
public:
    OverflowError() : ArithmeticError("Arithmetic operation resulted in an overflow.") {}
    explicit OverflowError(const char* message) : ArithmeticError(message) {}
};
// System.DivideByZeroException
class DivideByZeroError : public ArithmeticError {
public:
    DivideByZeroError() : ArithmeticError("Attempted to divide by zero.") {}
};

}  // namespace cb::dotnet::decompiler::util
