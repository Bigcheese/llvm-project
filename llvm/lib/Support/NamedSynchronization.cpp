//===-- NamedSynchronization.cpp - System wide synchronization objects ----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/NamedSynchronization.h"

#if defined(_WIN32)
#include "llvm/Support/Windows/WindowsSupport.h"
#elif LLVM_ON_UNIX
# include <fcntl.h>
# if defined(F_OFD_SETLKWTIMEOUT)
#   define HAS_OFD_LOCKS
# endif
#endif

#if !defined(_WIN32) && !defined(HAS_OFD_LOCKS)
#error No named synchronization primitives for this platform
#endif

using namespace llvm;

void NamedSynchronizationError::log(llvm::raw_ostream &OS) const {
  OS << "Named synchronization error with: " << Name << ": " << EC.message();
}

char NamedSynchronizationError::ID;

struct SystemNamedMutex::ImplState {
  std::string Name;
#if defined(_WIN32)
#elif defined(HAS_OFD_LOCKS)
  std::string UniqueLockName;
  int FD;
#endif
};

SystemNamedMutex::SystemNamedMutex() : State(std::make_unique<ImplState>()) {}

llvm::Expected<SystemNamedMutex> SystemNamedMutex::create(llvm::StringRef Name) {
#if defined(_WIN32)
#elif defined(HAS_OFD_LOCKS)
#endif
}
