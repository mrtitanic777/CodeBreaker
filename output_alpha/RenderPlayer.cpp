#include "stdafx.h"
// Hand-written PORT of RenderPlayer.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Real render code (armor passes, name-tag billboard, held-item / first-person
// hand). Ported from the converter output (output_alpha/RenderPlayer.cpp),
// fixed up against Alpha's RenderPlayer.h and the working indev RenderPlayer.
// Key fixes vs the raw conversion:
//   * 's_armorFilenamePrefix' is a real string array; armor path built without
//     Java string+int concat.
//   * RenderLiving base field is 'm_e' (not the stray 'e' the converter left).
//   * Virtual dispatch helpers call the real overloads (a()/b()), not the
//     non-existent m_a()/m_b().
//   * Tesselator::getInstance() returns a REFERENCE and MUST NOT be copied by
//     value (the vertex buffer is ~6 MB -> stack overflow). Bound by reference.
//   * Tesselator vertex/UV args cast to float (Alpha Tesselator is float-only).
//   * '->' vs '.' and (Type)x -> (Type*)x corrected.

#include "RenderPlayer.h"
#include "Common.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "ModelBase.h"
#include "ModelBiped.h"
#include "ModelRenderer.h"
#include "RenderManager.h"
#include "RenderEngine.h"
#include "Block.h"
#include "FontRenderer.h"
#include "Item.h"
#include "ItemArmor.h"
#include "ItemStack.h"
#include "ItemRenderer.h"
#include "RenderBlocks.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "RenderLiving.h"
#include <string>
#include <sstream>

// Java: private static final String[] armorFilenamePrefix =
//       {"cloth", "chain", "iron", "diamond", "gold"};
static std::string s_armorFilenamePrefixData[5] = {
	"cloth", "chain", "iron", "diamond", "gold"
};
std::string* RenderPlayer::s_armorFilenamePrefix = s_armorFilenamePrefixData;

// Build "/armor/<prefix>_<n>.png" without Java's String+int operator.
static std::string buildArmorTexture(const std::string& prefix, int part) {
	std::ostringstream ss;
	ss << "/armor/" << prefix << "_" << part << ".png";
	return ss.str();
}

RenderPlayer::RenderPlayer()
	: RenderLiving(new ModelBiped(0.0f), 0.5f)
	, m_field_209_f((ModelBiped*)m_e)
	, m_field_208_g(new ModelBiped(1.0f))
	, m_field_207_h(new ModelBiped(0.5f))
{
}

RenderPlayer::~RenderPlayer() {
	m_field_209_f = NULL;
	m_field_208_g = NULL;
	m_field_207_h = NULL;
}

// Armor render pass: returns true if the slot had armor (so RenderLiving runs the pass).
bool RenderPlayer::a(EntityPlayer* var1, int var2) {
	ItemStack* var3 = var1->m_inventory->armorItemInSlot(3 - var2);
	if(var3 != NULL) {
		Item* var4 = var3->getItem();
		if(dynamic_cast<ItemArmor*>(var4) != NULL) {
			ItemArmor* var5 = (ItemArmor*)var4;
			loadTexture(buildArmorTexture(s_armorFilenamePrefix[var5->m_renderIndex], (var2 == 2 ? 2 : 1)));
			ModelBiped* var6 = var2 == 2 ? m_field_207_h : m_field_208_g;
			var6->m_bipedHead->m_field_1403_h = var2 == 0;
			var6->m_field_1285_b->m_field_1403_h = var2 == 0;
			var6->m_field_1284_c->m_field_1403_h = var2 == 1 || var2 == 2;
			var6->m_bipedRightArm->m_field_1403_h = var2 == 1;
			var6->m_bipedLeftArm->m_field_1403_h = var2 == 1;
			var6->m_bipedRightLeg->m_field_1403_h = var2 == 2 || var2 == 3;
			var6->m_bipedLeftLeg->m_field_1403_h = var2 == 2 || var2 == 3;
			func_4013_a(var6);
			return true;
		}
	}

	return false;
}

// doRender(EntityPlayer) + name-tag billboard.
void RenderPlayer::a(EntityPlayer* var1, double var2, double var4, double var6, float var8, float var9) {
	ItemStack* var10 = var1->m_inventory->getCurrentItem();
	m_field_208_g->m_field_1278_i = m_field_207_h->m_field_1278_i = m_field_209_f->m_field_1278_i = var10 != NULL;
	m_field_208_g->m_field_1277_j = m_field_207_h->m_field_1277_j = m_field_209_f->m_field_1277_j = var1->func_381_o();
	double var11 = var4 - (double)var1->yOffset;
	if(var1->m_field_12240_bw) {
		var11 -= 0.125f;
	}

	RenderLiving::a((EntityLiving*)var1, var2, var11, var6, var8, var9);
	m_field_208_g->m_field_1277_j = m_field_207_h->m_field_1277_j = m_field_209_f->m_field_1277_j = false;
	m_field_208_g->m_field_1278_i = m_field_207_h->m_field_1278_i = m_field_209_f->m_field_1278_i = false;
	float var13 = 1.6F;
	float var14 = (float)(1.0f / 60.0f) * var13;
	float var15 = var1->getDistanceToEntity(renderManager->m_field_1226_h);
	float var16 = var1->func_381_o() ? 32.0F : 64.0F;
	if(var15 < var16) {
		var14 = (float)((double)var14 * (sqrtf((double)var15) / 2.0f));
		FontRenderer* var17 = getFontRendererFromRenderManager();
		D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2 + 0.0F, (float)var4 + 2.3F, (float)var6);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		Tesselator::getInstance().normal(0.0F, 1.0F, 0.0F);
		{ D3DXVECTOR3 _ax(0.0F,1.0F,0.0F); D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_ax, (-this->renderManager->m_field_1225_i)*DEG_TO_RAD);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		{ D3DXVECTOR3 _ax(1.0F,0.0F,0.0F); D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_ax, (this->renderManager->m_field_1224_j)*DEG_TO_RAD);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, -var14, -var14, var14);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		std::string var18 = var1->m_field_771_i;
		// Lighting is shader-based on Xbox 360 (disabled)
		Tesselator& var19 = Tesselator::getInstance();
		int var20;
		if(!var1->func_381_o()) {
			g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			g_pd3dDevice->SetTexture(0, NULL);
			var19.init();
			var20 = var17->getStringWidth(var18) / 2;
			var19.color4f(0.0F, 0.0F, 0.0F, 0.25F);
			var19.vertex((float)(-var20 - 1), -1.0f, 0.0f);
			var19.vertex((float)(-var20 - 1), 8.0f, 0.0f);
			var19.vertex((float)(var20 + 1), 8.0f, 0.0f);
			var19.vertex((float)(var20 + 1), -1.0f, 0.0f);
			var19.flush();
			g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: re-bind font texture handled by drawString */);
			var17->drawString(var18, -var17->getStringWidth(var18) / 2, 0, 553648127);
			g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			var17->drawString(var18, -var17->getStringWidth(var18) / 2, 0, -1);
			// Lighting is shader-based on Xbox 360 (enabled)
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		} else {
			{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.25F / var14, 0.0F);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			g_pd3dDevice->SetTexture(0, NULL);
			var19.init();
			var20 = var17->getStringWidth(var18) / 2;
			var19.color4f(0.0F, 0.0F, 0.0F, 0.25F);
			var19.vertex((float)(-var20 - 1), -1.0f, 0.0f);
			var19.vertex((float)(-var20 - 1), 8.0f, 0.0f);
			var19.vertex((float)(var20 + 1), 8.0f, 0.0f);
			var19.vertex((float)(var20 + 1), -1.0f, 0.0f);
			var19.flush();
			g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: re-bind font texture handled by drawString */);
			g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			var17->drawString(var18, -var17->getStringWidth(var18) / 2, 0, 553648127);
			// Lighting is shader-based on Xbox 360 (enabled)
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		}
	}

}

// Equipped-item pass: helmet block on head + held item in right hand.
void RenderPlayer::a(EntityPlayer* var1, float var2) {
	ItemStack* var3 = var1->m_inventory->armorItemInSlot(3);
	if(var3 != NULL && var3->getItem()->shiftedIndex < 256) {
		D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		m_field_209_f->m_bipedHead->func_926_b(1.0F / 16.0F);
		if(RenderBlocks::func_1219_a(Block::blocksList[var3->itemID]->getRenderType())) {
			float var4 = 10.0F / 16.0F;
			{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, -0.25F, 0.0F);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXVECTOR3 _ax(0.0F,1.0F,0.0F); D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_ax, (180.0F)*DEG_TO_RAD);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, var4, -var4, var4);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		renderManager->m_field_4236_f->renderItem(var3);
		Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
	}

	ItemStack* var6 = var1->m_inventory->getCurrentItem();
	if(var6 != NULL) {
		D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		m_field_209_f->m_bipedRightArm->func_926_b(1.0F / 16.0F);
		{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -(1.0F / 16.0F), 7.0F / 16.0F, 1.0F / 16.0F);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		if(var1->m_fishEntity != NULL) {
			var6 = new ItemStack(Item::s_stick->shiftedIndex);
		}

		float var5;
		if(var6->itemID < 256 && RenderBlocks::func_1219_a(Block::blocksList[var6->itemID]->getRenderType())) {
			var5 = 0.5F;
			{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 3.0F / 16.0F, -(5.0F / 16.0F));
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var5 *= 12.0F / 16.0F;
			{ D3DXVECTOR3 _ax(1.0F,0.0F,0.0F); D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_ax, (20.0F)*DEG_TO_RAD);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXVECTOR3 _ax(0.0F,1.0F,0.0F); D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_ax, (45.0F)*DEG_TO_RAD);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, var5, -var5, var5);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		} else if(Item::itemsList[var6->itemID]->isFull3D()) {
			var5 = 10.0F / 16.0F;
			if(Item::itemsList[var6->itemID]->shouldRotateAroundWhenRendering()) {
				{ D3DXVECTOR3 _ax(0.0F,0.0F,1.0F); D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_ax, (180.0F)*DEG_TO_RAD);
				  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
				  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, -(2.0F / 16.0F), 0.0F);
				  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
				  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			}

			{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 3.0F / 16.0F, 0.0F);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, var5, -var5, var5);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXVECTOR3 _ax(1.0F,0.0F,0.0F); D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_ax, (-100.0F)*DEG_TO_RAD);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXVECTOR3 _ax(0.0F,1.0F,0.0F); D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_ax, (45.0F)*DEG_TO_RAD);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		} else {
			var5 = 6.0F / 16.0F;
			{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.25F, 3.0F / 16.0F, -(3.0F / 16.0F));
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, var5, var5, var5);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXVECTOR3 _ax(0.0F,0.0F,1.0F); D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_ax, (60.0F)*DEG_TO_RAD);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXVECTOR3 _ax(1.0F,0.0F,0.0F); D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_ax, (-90.0F)*DEG_TO_RAD);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXVECTOR3 _ax(0.0F,0.0F,1.0F); D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_ax, (20.0F)*DEG_TO_RAD);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		renderManager->m_field_4236_f->renderItem(var6);
		Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
	}

}

// Scale-down pass (b) applied before rendering the player model.
void RenderPlayer::b(EntityPlayer* var1, float var2) {
	float var3 = 15.0F / 16.0F;
	{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, var3, var3, var3);
	  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
	  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
}

void RenderPlayer::drawFirstPersonHand() {
	m_field_209_f->m_field_1244_k = 0.0F;
	m_field_209_f->setRotationAngles(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F / 16.0F);
	m_field_209_f->m_bipedRightArm->render(1.0F / 16.0F);
}

// ---- Virtual dispatch shims from the RenderLiving base (EntityLiving*) ----
void RenderPlayer::func_6330_a(EntityLiving* var1, float var2) {
	this->b((EntityPlayer*)var1, var2);
}

bool RenderPlayer::func_166_a(EntityLiving* var1, int var2) {
	return this->a((EntityPlayer*)var1, var2);
}

void RenderPlayer::func_6331_b(EntityLiving* var1, float var2) {
	this->a((EntityPlayer*)var1, var2);
}

void RenderPlayer::a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9) {
	this->a((EntityPlayer*)var1, var2, var4, var6, var8, var9);
}

void RenderPlayer::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
	this->a((EntityPlayer*)var1, var2, var4, var6, var8, var9);
}
