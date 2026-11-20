#include "stdafx.h"
#include "Item.h"

void Item::init()
{
	Item::s_book = (new Item(47))->setIconCoord(11, 3)->setName("book");
	Item::s_diamond = (new Item(264))->setIconIndex(55)->setName("diamond");
	Item::s_fishCooked = (new Item(350))->setIconCoord(10, 5)->setName("fishCooked");
	Item::s_stick = (new Item(280))->setName("stick");
}
