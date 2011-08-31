#include <stdio.h>
#include <stdint.h>
#include <intrinsics.h>

#include <os/interrupt.h>
#include <os/driver.h>
#include <device/sbdspcore_sb3500.h>
#include <device/sbdspsoc_sb3500.h>

uintptr_t nmpt;
uintptr_t rfintr;

volatile uint32_t* counter = (uint32_t*)0x80000000;

/* Multi-purpose timer ISR */
void timer_isr(OsMachRegs* r, int vec) {

	puts("in timer_isr");
    /* increment the counter variable */
    ++*counter;

    /* clear the internal interrupt request */
    osIntrClear(vec);

    /* clear the external RF3 interrupt request */
    MMIO_WRITE32(rfintr, SBX_RFINTR3_STATUS, 1);
}

int main(int argc, char** argv) {

    /* assume OS core index 0 corresponds to HW core ID 1 */
    int core_index = 0;
    int core_id = 1;

    /* compute base address of the DSP node multi-purpose timer peripheral device */
    nmpt = SBX_MMIO_ADDR(core_id, 0x3000);
    rfintr = SBX_MMIO_ADDR(core_id, 0x2400);

    /* reset counter variable to 0 */
    *counter = 0;

    /* install MPT interrupt handler */
    osIntrSetHandler(core_index, SBX_IVEC_RF3, timer_isr);

    /* enable external RF3 interrupts */
    MMIO_WRITE32(rfintr, SBX_RFINTR3_ENABLE, 1);

    /* configure multi-purpose timer to generate periodic interrupts */
    MMIO_WRITE32(nmpt, SBX_MPT_PRESCALE, 16|(7 << 16)|(1 << 2));
    MMIO_WRITE32(nmpt, SBX_MPT0_CTL, 3|(6 << 11)|(1 << 6)|(3 << 4)|(3 << 2));
    MMIO_WRITE32(nmpt, SBX_MPT0_START, 0);
    MMIO_WRITE32(nmpt, SBX_MPT_ADJUST, (1 << 22));

    /* spin in an infinite loop */
    for (;;) {
        osFastPause(1000000);
    }
}
