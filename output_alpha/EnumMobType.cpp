#include "stdafx.h"
#include "EnumMobType.h"

EnumMobType* EnumMobType::s_everything = NULL;
EnumMobType* EnumMobType::s_mobs = NULL;
EnumMobType* EnumMobType::s_players = NULL;
EnumMobType* EnumMobType::values_[3] = {0};
const int EnumMobType::values_count = 3;

EnumMobType::EnumMobType() : ordinal(0) {}

void EnumMobType::initStatics()
{
	s_everything = new EnumMobType();
	s_everything->ordinal = 0; s_everything->nameStr = "everything"; values_[0] = s_everything;
	s_mobs = new EnumMobType();
	s_mobs->ordinal = 1; s_mobs->nameStr = "mobs"; values_[1] = s_mobs;
	s_players = new EnumMobType();
	s_players->ordinal = 2; s_players->nameStr = "players"; values_[2] = s_players;
}
