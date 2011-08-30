#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <assert.h>
#include <device/sbdspsoc.h>
#include <machine/machdefs.h>

#include <device/sbdspsoc_sb3500.h>

#define SBX_CLK	500

#define TXCOUNT 14	//AD7682 CFG Register's width
#define RXCOUNT 30

//20.83M, 0x5
#define SPI_CLK_DIV 0x5

#define SBXSPI_CSA 0

#define SPI_IS_IDLE(spibase_) \
    ((MMIO_READ32(spibase_, SBX_SPI_STATUS) & 0x41) == 0x40)

void test(void)
{
	uintptr_t mmio;
	uint32_t cfg;
	uint32_t sdo_val, cfg_readback;
	uint32_t rslt;

	mmio = SBX_SPI_BASE(1); //Core 1

	//Just send "CFG" data, No Recieve
	MMIO_WRITE32(mmio, SBX_SPI_CTL, TXCOUNT); //40 clock cycle = 1.92us, here we use 30, so < 2.2us
	//clock divider
	MMIO_WRITE32(mmio, SBX_SPI_PRESCALE, SPI_CLK_DIV);
	//CNV=CSa LOW When Write and Read
	MMIO_WRITE32(mmio, SBX_SPI_CONFIG, 0);//(0x1<<7) | (0x1<<3));

	//In0, In1, In2, In3 enabled.
	cfg = (1 << 13) | (7 << 10) | (0 << 7) | (1 << 6) | (0 << 3);

	printf("mmio:%x, cfg vale to send:%x\n", mmio, cfg);

	// 1. Send CFG Data
	MMIO_WRITE32(mmio, SBX_SPI_CS(SBXSPI_CSA), (cfg << 18));
	osFastPause(100);

	//Just Recieve, No Send
	MMIO_WRITE32(mmio, SBX_SPI_CTL, (1 << 12) | (RXCOUNT << 6));

	do
	{
		//CNV High, because there is no Write and Read.

		//CNV Low because of Write And Read
		MMIO_WRITE32(mmio, SBX_SPI_CS(SBXSPI_CSA), 0); //Just to bring CNV up
		MMIO_READ32(mmio, SBX_SPI_CS(SBXSPI_CSA)); //Read back data in RXCount Clock Cycles
		while (!SPI_IS_IDLE(mmio))
		{ //Wait for in-data ready
			osFastPause(10);
		}
		rslt = MMIO_READ32(mmio, SBX_SPI_READ); //Read ready data from Read-Register

		//CNV High Again
		sdo_val = rslt >> 14;
		cfg_readback = rslt & 0x3FFF;
		printf("sdo_val:%x, cfg_readback:%x\n", sdo_val, cfg_readback);

	} while (1);
}

int main(int argc, char *argv[])
{
	puts("start...");
	test();
}
