#pragma once
#ifndef REGPROG_H
#define REGPROG_H

#include "Globals.h"
#include "voxelxuLinux.h"
//extern int printTest(void);

class RegProgrammer
{
public:
	int CMD_REGWRITE;
	int CMD_REGREAD;
	int CMD_REGWRITE_CONT;
	int CMD_REGREAD_CONT;
	int CMD_TRANSACT;

	virtual void writeReg(int regAddr, int data) = 0;
	virtual int readReg(int regAddr) = 0;
	virtual void setReg(int regAddr, int data, int msb, int lsb) = 0;
	virtual void selectDevice(int devnum, int devCount = 1) = 0;
};

class VoxelI2CRegProgrammer : public RegProgrammer
{
public:
	VoxelI2CRegProgrammer(int devnum = 0, int devCount = 1);
	void selectDevice(int devnum = 0, int devCount = 1);
	void writeReg(int regAddr, int data);
	int readReg(int regAddr);
	void setReg(int regAddr, int data, int msb, int lsb);
};

class TPS659122RegProgrammer : public RegProgrammer
{
public:
	TPS659122RegProgrammer();
	void selectDevice(int devnum = 0, int devCount = 1);
	void writeReg(int regAddr, int data);
	int readReg(int regAddr);
	void setReg(int regAddr, int data, int msb, int lsb);
};

#endif // REGPROG_H
