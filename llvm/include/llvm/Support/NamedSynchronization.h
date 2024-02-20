//===-- NamedSynchronization.h - System wide sync objects -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// Provides system wide synchronization objects such as mutexes.
///
/// For each synchronization primitive defined here, all objects created with
/// the same name on the same system refer to the same entity.
///
/// Each class is modeled after the associated standard interface.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_NAMEDSYNCHRONIZATION_H
#define LLVM_SUPPORT_NAMEDSYNCHRONIZATION_H

#include "llvm/Support/Error.h"

namespace llvm {

class NamedSynchronizationError : public ErrorInfo<NamedSynchronizationError> {
public:
  NamedSynchronizationError(std::string Name, std::error_code EC) : Name(Name), EC(EC) {}
  std::error_code convertToErrorCode() const override { return EC; }
  void log(raw_ostream &OS) const override;
  StringRef getName() const { return Name; }

  static char ID;
protected:
  std::string Name;
  std::error_code EC;
};

/// Provides a system wide mutex identified by name.
///
/// This is used to synchronize access to global system resources both between
/// processes and within a single process. For example a file on disk.
///
/// It is resilient to process and system crashes.
class SystemNamedMutex {
public:
  static llvm::Expected<SystemNamedMutex> create_locked(StringRef Name);
  SystemNamedMutex(const SystemNamedMutex &) = delete;
  SystemNamedMutex(SystemNamedMutex &&);
  ~SystemNamedMutex();

  llvm::Error lock();
  llvm::Error try_lock();
  llvm::Error try_lock_for(std::chrono::steady_clock::duration Length);
  llvm::Error try_lock_until(std::chrono::steady_clock::time_point When);
  llvm::Error unlock();

  llvm::Error lock_shared();
  llvm::Error try_lock_shared();
  llvm::Error try_lock_shared_for(std::chrono::steady_clock::duration Length);
  llvm::Error try_lock_shared_until(std::chrono::steady_clock::time_point When);
  llvm::Error unlock_shared();

private:
  SystemNamedMutex();

  struct ImplState;
  std::unique_ptr<ImplState> State;
};

} // end namespace llvm

#endif // LLVM_SUPPORT_NAMEDSYNCHRONIZATION_H
