#include "stdafx.h"
// Auto-generated from TileEntitySign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntitySign.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include <string>
#include <sstream>
#include "Entity.h"
#include "TileEntity.h"

TileEntitySign::TileEntitySign()
	: m_signText(NULL)
	, m_lineBeingEdited(-1)
{
	// PORT(array-literal field init): C++03 has no init-list array fill; alloc + fill here.
	m_signText = new std::string[4];
	m_signText_count = 4;
	m_signText[0] = "";
	m_signText[1] = "";
	m_signText[2] = "";
	m_signText[3] = "";
}

TileEntitySign::~TileEntitySign() {
	delete[] m_signText;
	m_signText = NULL;
}

void TileEntitySign::writeToNBT(NBTTagCompound* var1) {
		// PORT: base TileEntity::writeToNBT now needs an id (no reflection on 360);
		// write it explicitly then persist coords + sign text.
		var1->setString("id", "Sign");
		var1->setInteger("x", xCoord);
		var1->setInteger("y", yCoord);
		var1->setInteger("z", zCoord);
		var1->setString("Text1", m_signText[0]);
		var1->setString("Text2", m_signText[1]);
		var1->setString("Text3", m_signText[2]);
		var1->setString("Text4", m_signText[3]);
}

void TileEntitySign::readFromNBT(NBTTagCompound* var1) {
		TileEntity::readFromNBT(var1);

		for(int var2 = 0; var2 < 4; ++var2) {
			// PORT: Java was "Text" + (var2 + 1) (string concat). C++ needs explicit
			// number-to-string; the converter emitted pointer arithmetic by mistake.
			std::stringstream var3;
			var3 << "Text" << (var2 + 1);
			m_signText[var2] = var1->getString(var3.str());
			if(m_signText[var2].length() > 15) {
				m_signText[var2] = m_signText[var2].substr(0, 15);
			}
		}

}
