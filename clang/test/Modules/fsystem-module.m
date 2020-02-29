// RUN: rm -rf %t
// RUN: rm -rf %t-saved
// RUN: mkdir %t-saved

// -fsystem-module requires -emit-module
// RUN: not %clang_cc1 -fsyntax-only -fsystem-module %s 2>&1 | grep "-emit-module"

// RUN: not %clang_cc1 -fmodules -I %S/Inputs \
// RUN:   -emit-module -fmodule-name=warning -pedantic -Werror \
// RUN:   %S/Inputs/module.map -o %t-saved/warning.pcm

// RUN: %clang_cc1 -fmodules -I %S/Inputs \
// RUN:   -emit-module -fmodule-name=warning -pedantic -Werror \
// RUN:   %S/Inputs/module.map -o %t-saved/warning-system.pcm -fsystem-module

// RUN: not %clang_cc1 -fmodules -I %S/Inputs \
// RUN:   -emit-module -fmodule-name=warning -pedantic -Werror \
// RUN:   %S/Inputs/module.map -o %t-saved/warning-system.pcm -fsystem-module \
// RUN:   -Wsystem-headers
