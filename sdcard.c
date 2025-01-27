/*
 * sdcard.c
 *
 *  Created on: Mar 21, 2023
 *      Author: Kian
 */

#include "sdcard.h"


int SdInitMount(){
	FRESULT fRes;				// Result of file operations --> 0 : OK , any other value: some error
	TCHAR *path = "0:/";		// TCHAR : char
	fRes = f_mount(&fatFs , path , 0);
	if(fRes){
		printf("ERROR: SD MOUNT ERROR CODE %d\n\r",fRes);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

int SdEjectUnmount(){
	FRESULT fRes;				// Result of file operations --> 0 : OK , any other value: some error
	TCHAR *path = "0:/";		// TCHAR : char
	fRes = f_mount(0 , path , 0);
	if(fRes){
		printf("ERROR: SD EJECT ERROR CODE %d\n\r",fRes);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

int ReadBinFile(char *fileName , u32 bufferAddress){
	FIL file;
	FRESULT fRes;
	UINT br;
	u32 fileSize;

	fRes = f_open(&file , fileName , FA_READ);
	if(fRes){
		printf("ERROR READ BIN FILE: f_open() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	fRes = f_lseek(&file , 0);
	if(fRes){
		printf("ERROR READ BIN FILE: f_lseek() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	fileSize = file.obj.objsize;
	fRes = f_read(&file , (void *)bufferAddress , fileSize , &br);
	if(fRes){
		printf("ERROR READ BIN FILE: f_read() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	fRes = f_close(&file);
	if(fRes){
		printf("ERROR READ BIN FILE: f_close() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	Xil_DCacheFlush();
	return XST_SUCCESS;
}

int WriteBinFile(char *fileName, u32 size , u32 bufferAddress){
	UINT btw;
	FIL file;
	FRESULT fRes;

	fRes = f_open(&file , fileName , FA_OPEN_ALWAYS | FA_WRITE);
	if(fRes){
		printf("ERROR WRITE BIN FILE: f_open() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	fRes = f_write(&file , (void *)bufferAddress , size , &btw);
	if(fRes){
		printf("ERROR WRITE BIN FILE: f_write() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	fRes = f_close(&file);
	if(fRes){
		printf("ERROR WRITE BIN FILE: f_close() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}


int ReadFile(char *fileName , u32 bufferAddress){
	FIL file;
	FRESULT fRes;
	UINT br;
	u32 fileSize;
	fRes = f_open(&file , fileName , FA_READ);
	if(fRes){
		xil_printf("ERROR : f_open() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	fRes = f_lseek(&file , 0);
	if(fRes){
		xil_printf("ERROR : f_lseek() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	fileSize = file.obj.objsize;
	fRes = f_read(&file , (void *)bufferAddress , fileSize , &br);
	if(fRes){
		xil_printf("ERROR : f_read() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	fRes = f_close(&file);
	if(fRes){
		xil_printf("ERROR : f_close() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	Xil_DCacheFlush();
	return XST_SUCCESS;
}

int WriteTextFile(char *fileName){
	FIL file;
	FRESULT fRes;

	fRes = f_open(&file , fileName , FA_OPEN_ALWAYS | FA_WRITE);
	if(fRes){
		xil_printf("ERROR : f_open() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

/*
	int i ,j;
	for(i=0 ; i<512 ; i++){
		for(j=0 ; j<512 ; j++){
			f_printf(&file, "%d\n", edge[i][j]);
		}
	}
*/

	fRes = f_close(&file);
	if(fRes){
		xil_printf("ERROR : f_close() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

int WriteAppendCsvFile(char *fileName , u8 *samples , int sampleSize){
//	UINT btw;
	FIL file;
	FRESULT fRes;
	u32 fileSize;

	fRes = f_open(&file , fileName , FA_OPEN_ALWAYS | FA_WRITE);
	if(fRes){
		printf("ERROR WRITE BIN FILE: f_open() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	fileSize = file.obj.objsize;
	f_lseek(&file, fileSize);

	int i;
	for(i=0 ; i<sampleSize ; i++){
		f_printf(&file, "%d,", samples[i]);
	}
	f_printf(&file, "\n");

	fRes = f_close(&file);
	if(fRes){
		printf("ERROR WRITE BIN FILE: f_close() ERROR CODE %d\n\r", fRes);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

