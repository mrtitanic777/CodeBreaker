#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet5PlayerInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet5PlayerInventory.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "Item.h"
#include "ItemStack.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NetHandler.h"
#include "Packet.h"
#include "Random.h"

Packet5PlayerInventory::Packet5PlayerInventory()
	: type(0)
{
}

Packet5PlayerInventory::Packet5PlayerInventory(int var1, JArray<ItemStack* > var2)
	: type(0)
{
	this->type = var1;
	this->stacks = JArray<ItemStack* >(var2.length);
	for(int var3 = 0; var3 < this->stacks.length; ++var3) {
	this->stacks[var3] = var2[var3] == NULL ? NULL : var2[var3]->copy();
	}
}

Packet5PlayerInventory::~Packet5PlayerInventory()
{
}

void Packet5PlayerInventory::readPacketData(DataInputStream* var1)
{
	this->type = var1->readInt();
	short var2 = var1->readShort();
	this->stacks = JArray<ItemStack* >(var2);
	for(int var3 = 0; var3 < var2; ++var3) {
	short var4 = var1->readShort();
	if(var4 >= 0) {
	unsigned char var5 = var1->readByte();
	short var6 = var1->readShort();
	this->stacks[var3] = new ItemStack(var4, var5, var6);
	}
	}
}

void Packet5PlayerInventory::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->type);
	var1->writeShort(this->stacks.length);
	for(int var2 = 0; var2 < this->stacks.length; ++var2) {
	if(this->stacks[var2] == NULL) {
	var1->writeShort(-1);
	} else {
	var1->writeShort((short)this->stacks[var2]->itemID);
	var1->writeByte((unsigned char)this->stacks[var2]->stackSize);
	var1->writeShort((short)this->stacks[var2]->itemDamage);
	}
	}
}

void Packet5PlayerInventory::processPacket(NetHandler* var1)
{
	var1->handlePlayerInventory(this);
}

int Packet5PlayerInventory::getPacketSize()
{
	return 6 + this->stacks.length * 5;
}

void Packet5PlayerInventory::initStatics()
{
}
