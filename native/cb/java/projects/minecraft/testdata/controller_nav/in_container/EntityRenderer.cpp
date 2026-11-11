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
	}
}
