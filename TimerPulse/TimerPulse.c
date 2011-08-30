#include <stdio.h>
#include <stdint.h>
#include <intrinsics.h>

#include <os/interrupt.h>
#include <os/driver.h>
#include <device/sbdspcore_sb3500.h>
#include <device/sbdspsoc_sb3500.h>

uintptr_t nmpt;

int main(int argc, char** argv)
{

	/* assume OS core index 0 corresponds to HW core ID 1 */
	int core_index = 0;
	int core_id;

	#define DELTA 0xA

	//DCNT_O_2 on Node2, Timer0
	core_id = 2; //For Node2,
	nmpt = SBX_MMIO_ADDR(core_id, 0x3000);
	MMIO_WRITE32(nmpt, SBX_MPT0_START, 0x1000000-DELTA);
	MMIO_WRITE32(nmpt, SBX_MPT0_COMPARE, 0x1000000-DELTA+(DELTA>>2));
	MMIO_WRITE32(nmpt, SBX_MPT0_CTL, 0x3A3F);
	MMIO_WRITE32(nmpt, SBX_MPT_PRESCALE, 0);
	MMIO_WRITE32(nmpt, SBX_MPT_ADJUST, (0x1<<22)); //Timer0
	
	//DCNT_O_4 and DCNT_O_5 on Node3
	core_id = 3; //For Node3,
	nmpt = SBX_MMIO_ADDR(core_id, 0x3000);
	MMIO_WRITE32(nmpt, SBX_MPT0_START, 0x1000000-DELTA);
	MMIO_WRITE32(nmpt, SBX_MPT0_COMPARE, 0x1000000-DELTA+(DELTA>>2));
	MMIO_WRITE32(nmpt, SBX_MPT0_CTL, 0x3A3F);
	MMIO_WRITE32(nmpt, SBX_MPT_PRESCALE, 0);

	MMIO_WRITE32(nmpt, SBX_MPT2_START, 0x1000000-DELTA);
	MMIO_WRITE32(nmpt, SBX_MPT2_COMPARE, 0x1000000-DELTA+(DELTA>>2));
	MMIO_WRITE32(nmpt, SBX_MPT2_CTL, 0x3A3F);
	MMIO_WRITE32(nmpt, SBX_MPT_PRESCALE, 0);
	MMIO_WRITE32(nmpt, SBX_MPT_ADJUST, (0x1<<24) | (0x1<<22)); //Timer0 and Timer2

	puts("Timer Pulse on Node3's Timer0 and Timer 2, Node2's Timer0");

	/* spin in an infinite loop */
	for (;;)
	{
		osFastPause(1000000);
		//puts("timer_isr in loop");
	}
}
