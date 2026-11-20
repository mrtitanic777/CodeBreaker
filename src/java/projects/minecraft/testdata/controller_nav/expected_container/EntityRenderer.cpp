#include "stdafx.h"
#include "EntityRenderer.h"

void EntityRenderer::updateCameraAndRender(float var1)
{
	if (this->mc->theWorld != NULL) {
		this->renderWorld(var1);
	}
	if (this->mc->currentScreen != NULL) {
		d3d9::glClear(d3d9::GL_DEPTH_BUFFER_BIT);
		this->mc->currentScreen->drawScreen(var2, var3, var1);
		// CONSOLE (controller): draw the virtual cursor (Pointer.png) ONLY in container GUIs. CodeBreaker-injected.
		{ extern bool g_guiCursorMode;
		if (g_guiCursorMode && this->mc->currentScreen != NULL) {
			int _gw = this->mc->currentScreen->width, _gh = this->mc->currentScreen->height;
			int _mx = _gw > 0 ? Mouse::getX() * _gw / this->mc->displayWidth : 0;
			int _my = _gh > 0 ? _gh - Mouse::getY() * _gh / this->mc->displayHeight - 1 : 0;
			d3d9::glBindTexture(d3d9::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/gui/Pointer.png"));
			d3d9::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
			d3d9::glEnable(d3d9::GL_BLEND);
			d3d9::glBlendFunc(d3d9::GL_SRC_ALPHA, d3d9::GL_ONE_MINUS_SRC_ALPHA);
			d3d9::glDisable(d3d9::GL_DEPTH_TEST);
			Tessellator* _t = Tessellator::s_instance;
			int _cs = 16, _px = _mx - _cs / 2, _py = _my - _cs / 2;
			_t->startDrawingQuads();
			_t->addVertexWithUV((float)_px, (float)(_py + _cs), 0.0F, 0.0F, 1.0F);
			_t->addVertexWithUV((float)(_px + _cs), (float)(_py + _cs), 0.0F, 1.0F, 1.0F);
			_t->addVertexWithUV((float)(_px + _cs), (float)_py, 0.0F, 1.0F, 0.0F);
			_t->addVertexWithUV((float)_px, (float)_py, 0.0F, 0.0F, 0.0F);
			_t->draw();
			d3d9::glEnable(d3d9::GL_DEPTH_TEST);
		}
		}
	}
}
