void RenderGlobal::updateRenderers(EntityPlayer* var1)
{
	Collections::sort(this->worldRenderersToUpdate, new RenderSorter(var1));
	// J2X render-budget: rebuild at most 8 chunks/frame (nearest first), not the whole dirty
	// set in one blocking frame -- a fresh world is 1000+ dirty chunks and would never present
	// (stuck on "Spawning"). The rest keep needsUpdate and rebuild over the next frames. [generator: render_fix]
	int var3 = 8;
	for (int var4 = 0; var4 < var3; ++var4) {
	int var2 = this->worldRenderersToUpdate.size();
	if(var2 == 0) {
	return;
	}
	WorldRenderer* var5 = (WorldRenderer*)this->worldRenderersToUpdate[var2 - 1];
	this->worldRenderersToUpdate.remove(var5);
	if(var5 != NULL) {
	var5->updateRenderer();
	var5->needsUpdate = false;
	}
	}
}

void EntityRenderer::hurtCameraEffect(float var1)
{
	EntityPlayerSP* var2 = this->mc->thePlayer;
	float var3 = (float)var2->hurtTime - var1;
	if(var2->health <= 0) {
	var1 += (float)var2->deathTime;
	}
	if(var3 >= 0.0F && var2->hurtTime > 0) {  // J2X hurt-NaN guard: on spawn hurtTime=0 & partialTick=0 -> var3=0 entered -> 0/0 NaN camera -> invisible world [render_fix]
	var3 /= (float)var2->maxHurtTime;
	var3 = MathHelper::sin(var3 * var3 * (float)3.1415927F);
	}
}

void RenderGlobal::g()
{
	int var2 = this->list.size() - 1;
	for (int i = 0; i < 3; ++i) {}
}
