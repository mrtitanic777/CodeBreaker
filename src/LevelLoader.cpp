#include "stdafx.h"
// Auto-generated from LevelLoader.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "LevelLoader.h"
#include "Common.h"
#include "Entity.h"
#include "IProgressUpdate.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "EntityCreeper.h"
#include "EntityGiantZombie.h"
#include "EntityItem.h"
#include "EntityPig.h"
#include "EntitySheep.h"
#include "EntitySkeleton.h"
#include "EntitySpider.h"
#include "EntityZombie.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NBTTagShort.h"
#include "TileEntity.h"
#include "TileEntityChest.h"
#include "BlockGrass.h"
#include "EntityMap.h"

LevelLoader::LevelLoader()
	: m_guiLoading(NULL)
{
}

LevelLoader::LevelLoader(IProgressUpdate* var1)
	: m_guiLoading(NULL)
{
		m_guiLoading = var1;
}

LevelLoader::~LevelLoader() {
	m_guiLoading = NULL;
}

World* LevelLoader::load(InputStream* var1) {
		if(m_guiLoading != NULL) {
			m_guiLoading->displayProgressMessage("Loading level");
		}

		if(m_guiLoading != NULL) {
			m_guiLoading->displayLoadingString("Reading..");
		}

		NBTTagCompound* var13 = LoadingScreenRenderer::writeLevelTags(var1);
		NBTTagCompound* var2 = var13->getCompoundTag("About");
		NBTTagCompound* var3 = var13->getCompoundTag("Map");
		NBTTagCompound* var4 = var13->getCompoundTag("Environment");
		NBTTagList* var5 = var13->getTagList("Entities");
		short var6 = var3->getShort("Width");
		short var7 = var3->getShort("Length");
		short var8 = var3->getShort("Height");
		World* var9 = new World();
		if(m_guiLoading != NULL) {
			m_guiLoading->displayLoadingString("Preparing level..");
		}

		NBTTagList* var10 = var3->getTagList("Spawn");
		var9->m_xSpawn = static_cast<NBTTagShort*>(var10->tagAt(0))->m_shortValue;
		var9->m_ySpawn = static_cast<NBTTagShort*>(var10->tagAt(1))->m_shortValue;
		var9->m_zSpawn = static_cast<NBTTagShort*>(var10->tagAt(2))->m_shortValue;
		var9->m_authorName = var2->getString("Author");
		var9->m_name = var2->getString("Name");
		var9->m_createTime = var2->getLong("CreatedOn");
		var9->m_cloudColor = var4->getInteger("CloudColor");
		var9->m_skyColor = var4->getInteger("SkyColor");
		var9->m_fogColor = var4->getInteger("FogColor");
		var9->m_skyBrightness = var4->getByte("SkyBrightness");
		if(var9->m_skyBrightness < 0) {
			var9->m_skyBrightness = 0;
		}

		if(var9->m_skyBrightness > 15) {
			var9->m_skyBrightness = var9->m_skyBrightness * 15 / 100;
		}

		var9->m_cloudHeight = var4->getShort("CloudHeight");
		var9->m_groundLevel = var4->getShort("SurroundingGroundHeight");
		var9->m_waterLevel = var4->getShort("SurroundingWaterHeight");
		var9->m_defaultFluid = var4->getByte("SurroundingWaterType");
		var9->m_worldTime = var4->getShort("TimeOfDay");
		var9->m_skylightSubtracted = var9->getSkyBrightness();
		var9->a(var6, var8, var7, var3->getByteArray("Blocks"), var3->getByteArray("Data"));
		if(m_guiLoading != NULL) {
			m_guiLoading->displayLoadingString("Preparing entities..");
		}

		for(int var16 = 0; var16 < var5->tagCount(); ++var16) {
   { // was try
				var3 = (NBTTagCompound*)var5->tagAt(var16);
				std::string var19 = var3->getString("id");
				Entity var22 = loadEntity(var9, var19);
				if(var22 != NULL) {
					var22.readFromNBT(var3);
					var9->spawnEntityInWorld(var22);
				} else {
					DbgPrintF("%s\n", "Skipping unknown entity id \"" + var19 + "\"");
				}
			} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var12)
				DbgPrintF("%s\n", "Error reading entity");
    // Removed Java stream/exception call: var12.printStackTrace();
			}
		}

		NBTTagList* var17 = var13->getTagList("TileEntities");

		for(int var18 = 0; var18 < var17->tagCount(); ++var18) {
   { // was try
				var4 = (NBTTagCompound*)var17->tagAt(var18);
				int var23 = var4->getInteger("Pos");
				std::string var14 = var4->getString("id");
				TileEntityChest* var21 = var14 == "Chest" ? new TileEntityChest() : NULL;
				if(var21 != NULL) {
					var21->readFromNBT(var4);
					int var15 = var23 % 1024;
					int var20 = (var23 >> 10) % 1024;
					var23 = (var23 >> 20) % 1024;
					var9->setBlockTileEntity(var15, var20, var23, var21);
				} else {
					DbgPrintF("%s\n", "Skipping unknown tile entity id \"" + var14 + "\"");
				}
			} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var11)
				DbgPrintF("%s\n", "Error reading tileentity");
    // Removed Java stream/exception call: var11.printStackTrace();
			}
		}

		return var9;
}

Entity* LevelLoader::loadEntity(World* var1, const std::string& var2) {
		return (Entity*)(var2 == "Pig" ? new EntityPig(var1) : (var2 == "Sheep" ? new EntitySheep(var1) : (var2 == "Creeper" ? new EntityCreeper(var1) : (var2 == "Skeleton" ? new EntitySkeleton(var1) : (var2 == "Spider" ? new EntitySpider(var1) : (var2 == "Zombie" ? new EntityZombie(var1) : (var2 == "Giant" ? new EntityGiantZombie(var1) : (var2 == "Item" ? new EntityItem(var1) : NULL))))))));
}

void LevelLoader::save(World* var1, OutputStream* var2) {
		if(m_guiLoading != NULL) {
			m_guiLoading->displayProgressMessage("Saving level");
		}

		if(m_guiLoading != NULL) {
			m_guiLoading->displayLoadingString("Preparing level..");
		}

		NBTTagCompound* var3 = new NBTTagCompound();
		var3->setInteger("CloudColor", var1->m_cloudColor);
		var3->setInteger("SkyColor", var1->m_skyColor);
		var3->setInteger("FogColor", var1->m_fogColor);
		var3->setByte("SkyBrightness", (unsigned char)var1->m_skyBrightness);
		var3->setShort("CloudHeight", (short)var1->m_cloudHeight);
		var3->setShort("SurroundingGroundHeight", (short)var1->m_groundLevel);
		var3->setShort("SurroundingWaterHeight", (short)var1->m_waterLevel);
		var3->setByte("SurroundingGroundType", (unsigned char)Block::s_grass->blockID);
		var3->setByte("SurroundingWaterType", (unsigned char)var1->m_defaultFluid);
		var3->setShort("TimeOfDay", (short)var1->m_worldTime);
		NBTTagCompound* var4 = new NBTTagCompound();
		var4->setShort("Width", (short)var1->width);
		var4->setShort("Length", (short)var1 /* PORT: array length - use tracked size */);
		var4->setShort("Height", (short)var1->height);
		var4->setByteArray("Blocks", var1->m_d);
		var4->setByteArray("Data", var1->m_e);
		NBTTagList* var5 = new NBTTagList();
		var5->setTag(new NBTTagShort((short)var1->m_xSpawn));
		var5->setTag(new NBTTagShort((short)var1->m_ySpawn));
		var5->setTag(new NBTTagShort((short)var1->m_zSpawn));
		var4->setTag("Spawn", var5);
		NBTTagCompound* var15 = new NBTTagCompound();
		var15->setString("Author", var1->m_authorName);
		var15->setString("Name", var1->m_name);
		var15->setLong("CreatedOn", var1->m_createTime);
		if(m_guiLoading != NULL) {
			m_guiLoading->displayLoadingString("Preparing entities..");
		}

		NBTTagList* var6 = new NBTTagList();
		for (size_t _i7 = 0; _i7 < var1->m_entityMap.m_entities.size(); ++_i7) {
			Entity var8 = (Entity*)var7.next();
			NBTTagCompound* var9 = new NBTTagCompound();
			var8.writeToNBT(var9);
			if(!var9->emptyNBTMap()) {
				var6->setTag(var9);
			}
		}

		NBTTagList* var16 = new NBTTagList();
		for (size_t _i17 = 0; _i17 < var1->m_map.keySet().size(); ++_i17) {
			int var19 = ((int)var17.next()).intValue();
			NBTTagCompound* var10 = new NBTTagCompound();
			var10->setInteger("Pos", var19);
			TileEntity* var20 = (TileEntity*)var1->m_map.get(var19);
			var20->writeToNBT(var10);
			var16->setTag(var10);
		}

		NBTTagCompound* var18 = new NBTTagCompound();
		var18->setKey("MinecraftLevel");
		var18->setCompoundTag("About", var15);
		var18->setCompoundTag("Map", var4);
		var18->setCompoundTag("Environment", var3);
		var18->setTag("Entities", var6);
		var18->setTag("TileEntities", var16);
		if(m_guiLoading != NULL) {
			m_guiLoading->displayLoadingString("Writing..");
		}

		NBTTagCompound var13 = var18;
  // PORT: DataOutputStream -> Xbox 360 file I/O  // was: DataOutputStream* var14 = new DataOutputStream(new GZIPOutputStream(var2));

  { // was try
			NBTBase::writeTag(var13, var14);
  } { // was finally � cleanup code follows
   // Removed Java stream/exception call: var14.close();
		}

}
