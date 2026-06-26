#include "stdafx.h"
// Auto-generated from TexturePackList.java
// Target: Xbox 360 / C++03 / DirectX 9
//
// PORT: On a single-player Xbox 360 port there are NO external (.zip) texture
// packs and no java.io.File directory scanning. The original references inner
// classes TexturePackDefault / TexturePackCustom and java.io.File, none of which
// exist in this codebase. TexturePackList therefore only ever holds the built-in
// default pack (a plain TexturePackBase); availableTexturePacks() returns that
// single entry. See GuiTexturePacks.cpp for the matching UI assumption.

#include "TexturePackList.h"
#include "Common.h"
#include "Map.h"
#include "Minecraft.h"
#include "GameSettings.h"
#include "TexturePackBase.h"
#include <string>

TexturePackList::TexturePackList()
	: m_selectedTexturePack(NULL)
	, m_availableTexturePacks()
	, m_defaultTexturePack(new TexturePackBase())
	, m_field_6538_d(new std::map<std::string, void*>())
	, mc(NULL)
	, m_texturePackDir(NULL)
	, m_field_6535_g("")
{
}

TexturePackList::TexturePackList(Minecraft* var1, std::string /* PORT: was java.io.File */* var2)
	: m_selectedTexturePack(NULL)
	, m_availableTexturePacks()
	, m_defaultTexturePack(new TexturePackBase())
	, m_field_6538_d(new std::map<std::string, void*>())
	, mc(NULL)
	, m_texturePackDir(NULL)
	, m_field_6535_g("")
{
		mc = var1;
		// PORT: no texturepacks directory on 360 (java.io.File unsupported)
		if(var1->m_gameSettings != NULL) {
			m_field_6535_g = var1->m_gameSettings->m_skin;
		}
		func_6532_a();
		if(m_selectedTexturePack != NULL) {
			m_selectedTexturePack->func_6482_a();
		}
}

TexturePackList::~TexturePackList() {
	m_defaultTexturePack = NULL;
	m_selectedTexturePack = NULL;
	m_field_6538_d = NULL;
	mc = NULL;
	m_texturePackDir = NULL;
}

bool TexturePackList::setTexturePack(TexturePackBase* var1) {
		if(var1 == m_selectedTexturePack) {
			return false;
		} else {
			if(m_selectedTexturePack != NULL) {
				m_selectedTexturePack->closeTexturePackFile();
			}
			m_field_6535_g = var1->m_texturePackFileName;
			m_selectedTexturePack = var1;
			if(mc != NULL && mc->m_gameSettings != NULL) {
				mc->m_gameSettings->m_skin = m_field_6535_g;
				mc->m_gameSettings->saveOptions();
			}
			m_selectedTexturePack->func_6482_a();
			return true;
		}
}

void TexturePackList::func_6532_a() {
		// PORT: only the built-in default pack exists on 360 (no .zip scanning).
		std::vector<void*> var1;
		m_selectedTexturePack = NULL;
		var1.push_back((void*)m_defaultTexturePack);

		if(m_selectedTexturePack == NULL) {
			m_selectedTexturePack = m_defaultTexturePack;
		}

		m_availableTexturePacks = var1;
}

std::vector<void*> /* PORT: determine element type */ TexturePackList::availableTexturePacks() {
		return m_availableTexturePacks;
}
