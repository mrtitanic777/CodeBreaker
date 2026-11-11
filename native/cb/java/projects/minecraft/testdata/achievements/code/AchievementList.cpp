#include "stdafx.h"
#include "AchievementList.h"

void AchievementList::init()
{
	AchievementList::s_openInventory = (new Achievement(0, "openInventory", 0, 0, Item::s_book, NULL))->setIndependent()->registerAchievement();
	AchievementList::s_mineWood = (new Achievement(1, "mineWood", 2, 1, Block::s_wood, openInventory))->registerAchievement();
	AchievementList::s_buildWorkBench = (new Achievement(2, "buildWorkBench", 4, -1, Block::s_workbench, mineWood))->registerAchievement();
	AchievementList::s_diamonds = (new Achievement(3, "diamonds", -5, 5, Item::s_diamond, buildWorkBench))->setSpecial()->registerAchievement();
	AchievementList::s_onARail = (new Achievement(4, "onARail", 2, 3, Item::s_missing, NULL))->registerAchievement();
	AchievementList::s_cookFish = (new Achievement(5, "cookFish", 1, 1, Item::s_fishCooked, null))->registerAchievement();
	AchievementList::s_notThis = something("new Achievement is only a word here");
}
