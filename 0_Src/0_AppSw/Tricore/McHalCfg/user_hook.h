#ifndef __USER_HOOK_H__
#define	__USER_HOOK_H__

IFX_INLINE void cTrapHook(IfxCpu_Trap trapInfo)
{
	switch (trapInfo.tClass)
	{
		case IfxCpu_Trap_Class_memoryManagement:
		{
			//user code: Function calls allowed.
			break;
		}
		case IfxCpu_Trap_Class_internalProtection:
		{
			//user code: Function calls allowed.
			break;
		}
		case IfxCpu_Trap_Class_instructionErrors:
		{
			//user code: Function calls allowed.
			break;
		}
		case IfxCpu_Trap_Class_contextManagement:
		{
			//user code: Function calls NOT allowed.
			break;
		}
		case IfxCpu_Trap_Class_bus:
		{
			//user code: Function calls allowed.
			break;
		}
		case IfxCpu_Trap_Class_assertion:
		{
			//user code: Function calls allowed.
			break;
		}
		case IfxCpu_Trap_Class_nonMaskableInterrupt:
		{
			//user code: Function calls allowed.
			break;
		}
		default:
		{
			break;
		}
	}

	printf("%s:%i\n", __func__, trapInfo.tClass);
}

IFX_INLINE void c0SVCHook(IfxCpu_Trap trapInfo)
{
  switch (trapInfo.tId)
  {
    case 0:
    {
      //user code: Function calls allowed.
      break;
    }
    case 1:
    {
      //user code: Function calls allowed.
      break;
    }
    // and so on..
    default:
    {
      break;
    }
  }

  printf("%s:%i\n", __func__, trapInfo.tId);
}


IFX_INLINE void c1SVCHook(IfxCpu_Trap trapInfo)
{
  switch (trapInfo.tId)
  {
    case 0:
    {
      //user code: Function calls allowed.
      break;
    }
    case 1:
    {
      //user code: Function calls allowed.
      break;
    }
    // and so on..
    default:
    {
      break;
    }
  }

  printf("%s:%i\n", __func__, trapInfo.tId);
}


IFX_INLINE void c2SVCHook(IfxCpu_Trap trapInfo)
{
  switch (trapInfo.tId)
  {
    case 0:
    {
      //user code: Function calls allowed.
      break;
    }
    case 1:
    {
      //user code: Function calls allowed.
      break;
    }
    // and so on..
    default:
    {
      break;
    }
  }

  printf("%s:%i\n", __func__, trapInfo.tId);
}

#endif
