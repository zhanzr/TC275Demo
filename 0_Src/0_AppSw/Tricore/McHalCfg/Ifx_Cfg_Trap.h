#ifndef __IFX_CFG_TRAP_H__
#define	__IFX_CFG_TRAP_H__

#include "user_hook.h"

#define IFX_CFG_CPU_TRAP_TSR_HOOK(trapInfo)  cTrapHook(trapInfo)
#define IFX_CFG_CPU_TRAP_SYSCALL_CPU0_HOOK(trapInfo)  c0SVCHook(trapInfo)
#define IFX_CFG_CPU_TRAP_SYSCALL_CPU1_HOOK(trapInfo)  c1SVCHook(trapInfo)
#define IFX_CFG_CPU_TRAP_SYSCALL_CPU2_HOOK(trapInfo)  c2SVCHook(trapInfo)

#endif
