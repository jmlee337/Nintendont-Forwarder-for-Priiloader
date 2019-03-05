 /****************************************************************************
 * Copyright 2009 The Lemon Man and thanks to luccax, Wiipower, Aurelio and crediar
 * Copyright 2010 Dimok
 * Copyright 2011 FIX94, oggzee
 *
 * Original forwarder source by
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/
#include <gccore.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ogc/machine/processor.h>
#include <wiiuse/wpad.h>

#include "devicemounter.h"

#define EXECUTE_ADDR	((u8 *) 0x92000000)
#define BOOTER_ADDR		((u8 *) 0x93000000)
#define ARGS_ADDR		((u8 *) 0x93200000)

typedef void (*entrypoint) (void);
extern void __exception_setreload(int t);
extern void __exception_closeall();
extern const u8 app_booter_bin[];
extern const u32 app_booter_bin_size;

static FILE *open_file(const char *dev, char *filepath)
{
	sprintf(filepath, "%s:/apps/usbloader/boot.dol", dev);
	FILE *exeFile = fopen(filepath ,"rb");

	if (exeFile == NULL)
	{
		sprintf(filepath, "%s:/apps/usbloader_cfg/boot.dol", dev);
		exeFile = fopen(filepath ,"rb");
	}
	if (exeFile == NULL)
	{
		sprintf(filepath, "%s:/apps/usbloader/boot.elf", dev);
		exeFile = fopen(filepath ,"rb");
	}
	if (exeFile == NULL)
	{
		sprintf(filepath, "%s:/apps/usbloader_cfg/boot.elf", dev);
		exeFile = fopen(filepath ,"rb");
	}

	return exeFile;
}

void SystemMenu()
{
	*(vu32*)0x8132FFFB = 0x50756e65;
	DCFlushRange((void *)(0x8132FFFB), 4);
	ICInvalidateRange((void *)(0x8132FFFB), 4);
	SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

int main(int argc, char *argv[])
{
	u32 cookie;
	FILE *exeFile = NULL;
	void *exeBuffer = (void *)EXECUTE_ADDR;
	u32 exeSize = 0;
	entrypoint exeEntryPoint;
	__exception_setreload(0);

	// initial video
	void *xfb = NULL;
	GXRModeObj *rmode = NULL;

	VIDEO_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(TRUE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) 
		VIDEO_WaitVSync();

	char filepath[200];

	//init wiimote
	WPAD_Init();

	// try SD Card First
	SDCard_Init();
	exeFile = open_file(DeviceName[SD], filepath);
	// if app not found on SD Card try USB
	if (exeFile == NULL)
	{
		USBDevice_Init();
		int dev;
		for(dev = USB1; dev < MAXDEVICES; ++dev)
		{
			if(!exeFile)
				exeFile = open_file(DeviceName[dev], filepath);
		}
	}

	// if nothing found exiting
	if (exeFile == NULL)
	{
		fclose(exeFile);
		SDCard_deInit();
		USBDevice_deInit();
		SystemMenu();
	}

	fseek(exeFile, 0, SEEK_END);
	exeSize = ftell(exeFile);
	rewind(exeFile);

	if(fread(exeBuffer, 1, exeSize, exeFile) != exeSize)
	{
		fclose(exeFile);
		SDCard_deInit();
		USBDevice_deInit();
		SystemMenu();
	}
	fclose(exeFile);

	memcpy(BOOTER_ADDR, app_booter_bin, app_booter_bin_size);
	DCFlushRange(BOOTER_ADDR, app_booter_bin_size);

	SDCard_deInit();
	USBDevice_deInit();

	exeEntryPoint = (entrypoint)BOOTER_ADDR;
	/* cleaning up and load dol */
	SYS_ResetSystem(SYS_SHUTDOWN, 0, 0);
	_CPU_ISR_Disable(cookie);
	__exception_closeall();
	exeEntryPoint();
	_CPU_ISR_Restore(cookie);
	return 0;
}
