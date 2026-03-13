#include <rtthread.h>
#include <fh_def.h>
#include <fh_arch.h>
#include "fh_hwspinlock.h"

/* hwspinlock registers definition */
#define HWSPINLOCK_COUNTER      0x0
#define HWSPINLOCK_STATUS(_X_)  (0x00 + 0x4 + 0x4 * (_X_))

/* try to lock the hardware spinlock */
void fh_hwspinlock_lock(unsigned int channel)
{
    UINT32 counter;
    UINT32 status;
    UINT32 addr;

    addr = HWSPINLOCK_REG_BASE + HWSPINLOCK_STATUS(channel);

    while (1)
    {
        while (1)
        {
            status = GET_REG(addr);
            if (status == 0)
                break;
        }

        counter = GET_REG(HWSPINLOCK_REG_BASE + HWSPINLOCK_COUNTER);
        SET_REG(addr, counter);
        status = GET_REG(addr);
        if (status == counter)
            break;
    }

}

/* unlock the hardware spinlock */
void fh_hwspinlock_unlock(unsigned int channel)
{
    SET_REG(HWSPINLOCK_REG_BASE + HWSPINLOCK_STATUS(channel), 0);
}
