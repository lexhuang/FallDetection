

#include "regProg.h"

#include <stdio.h>

VoxelI2CRegProgrammer::VoxelI2CRegProgrammer(int devnum, int devCount)
{
	logMessage("Initializing Voxel I2C Reg Programmer\n");
	Voxelxu::enumerateDevices();
	this->selectDevice(devnum, devCount);
}

void VoxelI2CRegProgrammer::selectDevice(int devnum, int devCount)
{
	if (devCount == 0){
		Voxelxu::selectDevice(-1);
	}
	else{
		Voxelxu::selectDevice(devnum);
    }
}

void VoxelI2CRegProgrammer::writeReg(int regAddr, int data)
{
	int data1, data2, data3;
	unsigned char arr[5];
	data1 = data & 0xFF;
	data2 = (data >> 8) & 0xFF;
	data3 = (data >> 16) & 0xFF;
	if (regAddr >= 256) {
		arr[0] = 0x58;
	}
	else {
		arr[0] = 0x5C;
	}
	arr[1] = regAddr & 0xFF;
	arr[2] = data1;
	arr[3] = data2;
	arr[4] = data3;

	Voxelxu::setCur(1, 5, arr);
	logMessage("Write Reg: %x %x\n", regAddr, data);
}

int VoxelI2CRegProgrammer::readReg(int regAddr)
{
	unsigned char arr[2], readArr[3];
	if (regAddr >= 256) {
		arr[0] = 0x58;
	}
	else {
		arr[0] = 0x5C;
	}
	arr[1] = regAddr & 0xFF;
	Voxelxu::setCur(2, 2, arr);

	readArr[0] = 0xAA;
	readArr[1] = 0x55;
	readArr[2] = 0xCC;
	Voxelxu::getCur(3, 3, readArr);
	logMessage("Read Reg: %x\n", regAddr);
	logMessage("%x %x %x\n", readArr[0], readArr[1], readArr[2]);
	logMessage("%x\n", readArr[0] + (readArr[1] << 8) + (readArr[2] << 16));
	return readArr[0] + (readArr[1] << 8) + (readArr[2] << 16);
}

void VoxelI2CRegProgrammer::setReg(int regAddr, int data, int msb, int lsb)
{
	int mask, i, reg, modifiedReg;
	int regLen = 24;
	mask = (1 << regLen) - 1;
	for (i = lsb; i <= msb; i++) {
		mask -= 1 << i;
	}
	reg = this->readReg(regAddr);
	//reg = 51;
	modifiedReg = (reg & mask) | (data << lsb);
	this->writeReg(regAddr, modifiedReg);
	logMessage("Set Reg Value: %x\n", modifiedReg);
}

TPS659122RegProgrammer::TPS659122RegProgrammer()
{

}

void TPS659122RegProgrammer::selectDevice(int devnum, int devCount)
{
}

void TPS659122RegProgrammer::writeReg(int regAddr, int data)
{
	unsigned char arr[3];
	arr[0] = 0x2D;
	arr[1] = regAddr;
	arr[2] = data;

	Voxelxu::setCur(4, 3, arr);
	logMessage("Write Reg: %x %x\n", regAddr, data);
}

int TPS659122RegProgrammer::readReg(int regAddr)
{
	unsigned char arr[2], readArr[1];
	arr[0] = 0x2D;
	arr[1] = regAddr;
	Voxelxu::setCur(2, 2, arr);

	readArr[0] = 0xAA;
	Voxelxu::getCur(5, 1, readArr);
	logMessage("Read Reg and Data: %x %x\n", regAddr, readArr[0]);
	return readArr[0];
}

void TPS659122RegProgrammer::setReg(int regAddr, int data, int msb, int lsb)
{
	int mask, i, reg, modifiedReg;
	int regLen = 8;
	mask = (1 << regLen) - 1;
	for (i = lsb; i <= msb; i++) {
		mask -= 1 << i;
	}
	reg = this->readReg(regAddr);
	//reg = 51;
	modifiedReg = (reg & mask) | (data << lsb);
	this->writeReg(regAddr, modifiedReg);
	logMessage("Set Reg Value: %x\n", modifiedReg);
}
