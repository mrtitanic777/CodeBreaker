#include "stdafx.h"
#include "EnumCreatureType.h"

EnumCreatureType* EnumCreatureType::s_monster = NULL;
EnumCreatureType* EnumCreatureType::s_creature = NULL;
EnumCreatureType* EnumCreatureType::values_[2] = {0};
const int EnumCreatureType::values_count = 2;

EnumCreatureType::EnumCreatureType() : ordinal(0) {}

EnumCreatureType::EnumCreatureType(void* var3, int var4) : ordinal(0)
{
	field_4278_c = var3;
	maxNumberOfEntityType = var4;
}

void EnumCreatureType::initStatics()
{
	// PORT-BLOCKED: s_monster = new EnumCreatureType(NULL /* PORT: IMobs.class */, 100);
	s_monster->ordinal = 0; s_monster->nameStr = "monster"; values_[0] = s_monster;
	// PORT-BLOCKED: s_creature = new EnumCreatureType(NULL /* PORT: EntityAnimals.class */, 20);
	s_creature->ordinal = 1; s_creature->nameStr = "creature"; values_[1] = s_creature;
}
