#include "stdafx.h"
#include "EnumArt.h"

EnumArt* EnumArt::s_Kebab = NULL;
EnumArt* EnumArt::s_Aztec = NULL;
EnumArt* EnumArt::s_Alban = NULL;
EnumArt* EnumArt::s_Aztec2 = NULL;
EnumArt* EnumArt::s_Bomb = NULL;
EnumArt* EnumArt::s_Plant = NULL;
EnumArt* EnumArt::s_Wasteland = NULL;
EnumArt* EnumArt::s_Pool = NULL;
EnumArt* EnumArt::s_Courbet = NULL;
EnumArt* EnumArt::s_Sea = NULL;
EnumArt* EnumArt::s_Sunset = NULL;
EnumArt* EnumArt::s_Creebet = NULL;
EnumArt* EnumArt::s_Wanderer = NULL;
EnumArt* EnumArt::s_Graham = NULL;
EnumArt* EnumArt::s_Match = NULL;
EnumArt* EnumArt::s_Bust = NULL;
EnumArt* EnumArt::s_Stage = NULL;
EnumArt* EnumArt::s_Void = NULL;
EnumArt* EnumArt::s_SkullAndRoses = NULL;
EnumArt* EnumArt::s_Fighters = NULL;
EnumArt* EnumArt::s_Pointer = NULL;
EnumArt* EnumArt::s_Pigscene = NULL;
EnumArt* EnumArt::s_Skeleton = NULL;
EnumArt* EnumArt::s_DonkeyKong = NULL;
EnumArt* EnumArt::values_[24] = {0};
const int EnumArt::values_count = 24;

EnumArt::EnumArt() : ordinal(0) {}

EnumArt::EnumArt(const std::string& var3, int var4, int var5, int var6, int var7) : ordinal(0)
{
	field_1624_y = var3;
	field_1623_z = var4;
	field_1636_A = var5;
	field_1634_B = var6;
	field_1632_C = var7;
}

void EnumArt::initStatics()
{
	s_Kebab = new EnumArt("Kebab", 16, 16, 0, 0);
	s_Kebab->ordinal = 0; s_Kebab->nameStr = "Kebab"; values_[0] = s_Kebab;
	s_Aztec = new EnumArt("Aztec", 16, 16, 16, 0);
	s_Aztec->ordinal = 1; s_Aztec->nameStr = "Aztec"; values_[1] = s_Aztec;
	s_Alban = new EnumArt("Alban", 16, 16, 32, 0);
	s_Alban->ordinal = 2; s_Alban->nameStr = "Alban"; values_[2] = s_Alban;
	s_Aztec2 = new EnumArt("Aztec2", 16, 16, 48, 0);
	s_Aztec2->ordinal = 3; s_Aztec2->nameStr = "Aztec2"; values_[3] = s_Aztec2;
	s_Bomb = new EnumArt("Bomb", 16, 16, 64, 0);
	s_Bomb->ordinal = 4; s_Bomb->nameStr = "Bomb"; values_[4] = s_Bomb;
	s_Plant = new EnumArt("Plant", 16, 16, 80, 0);
	s_Plant->ordinal = 5; s_Plant->nameStr = "Plant"; values_[5] = s_Plant;
	s_Wasteland = new EnumArt("Wasteland", 16, 16, 96, 0);
	s_Wasteland->ordinal = 6; s_Wasteland->nameStr = "Wasteland"; values_[6] = s_Wasteland;
	s_Pool = new EnumArt("Pool", 32, 16, 0, 32);
	s_Pool->ordinal = 7; s_Pool->nameStr = "Pool"; values_[7] = s_Pool;
	s_Courbet = new EnumArt("Courbet", 32, 16, 32, 32);
	s_Courbet->ordinal = 8; s_Courbet->nameStr = "Courbet"; values_[8] = s_Courbet;
	s_Sea = new EnumArt("Sea", 32, 16, 64, 32);
	s_Sea->ordinal = 9; s_Sea->nameStr = "Sea"; values_[9] = s_Sea;
	s_Sunset = new EnumArt("Sunset", 32, 16, 96, 32);
	s_Sunset->ordinal = 10; s_Sunset->nameStr = "Sunset"; values_[10] = s_Sunset;
	s_Creebet = new EnumArt("Creebet", 32, 16, 128, 32);
	s_Creebet->ordinal = 11; s_Creebet->nameStr = "Creebet"; values_[11] = s_Creebet;
	s_Wanderer = new EnumArt("Wanderer", 16, 32, 0, 64);
	s_Wanderer->ordinal = 12; s_Wanderer->nameStr = "Wanderer"; values_[12] = s_Wanderer;
	s_Graham = new EnumArt("Graham", 16, 32, 16, 64);
	s_Graham->ordinal = 13; s_Graham->nameStr = "Graham"; values_[13] = s_Graham;
	s_Match = new EnumArt("Match", 32, 32, 0, 128);
	s_Match->ordinal = 14; s_Match->nameStr = "Match"; values_[14] = s_Match;
	s_Bust = new EnumArt("Bust", 32, 32, 32, 128);
	s_Bust->ordinal = 15; s_Bust->nameStr = "Bust"; values_[15] = s_Bust;
	s_Stage = new EnumArt("Stage", 32, 32, 64, 128);
	s_Stage->ordinal = 16; s_Stage->nameStr = "Stage"; values_[16] = s_Stage;
	s_Void = new EnumArt("Void", 32, 32, 96, 128);
	s_Void->ordinal = 17; s_Void->nameStr = "Void"; values_[17] = s_Void;
	s_SkullAndRoses = new EnumArt("SkullAndRoses", 32, 32, 128, 128);
	s_SkullAndRoses->ordinal = 18; s_SkullAndRoses->nameStr = "SkullAndRoses"; values_[18] = s_SkullAndRoses;
	s_Fighters = new EnumArt("Fighters", 64, 32, 0, 96);
	s_Fighters->ordinal = 19; s_Fighters->nameStr = "Fighters"; values_[19] = s_Fighters;
	s_Pointer = new EnumArt("Pointer", 64, 64, 0, 192);
	s_Pointer->ordinal = 20; s_Pointer->nameStr = "Pointer"; values_[20] = s_Pointer;
	s_Pigscene = new EnumArt("Pigscene", 64, 64, 64, 192);
	s_Pigscene->ordinal = 21; s_Pigscene->nameStr = "Pigscene"; values_[21] = s_Pigscene;
	s_Skeleton = new EnumArt("Skeleton", 64, 48, 192, 64);
	s_Skeleton->ordinal = 22; s_Skeleton->nameStr = "Skeleton"; values_[22] = s_Skeleton;
	s_DonkeyKong = new EnumArt("DonkeyKong", 64, 48, 192, 112);
	s_DonkeyKong->ordinal = 23; s_DonkeyKong->nameStr = "DonkeyKong"; values_[23] = s_DonkeyKong;
}
