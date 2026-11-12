void RenderGlobal::updateRenderers(EntityPlayer* var1)
{
	Collections::sort(this->worldRenderersToUpdate, new RenderSorter(var1));
	int var2 = this->worldRenderersToUpdate.size() - 1;
	int var3 = this->worldRenderersToUpdate.size();
	for (int var4 = 0; var4 < var3; ++var4) {
	WorldRenderer* var5 = (WorldRenderer*)this->worldRenderersToUpdate[var2 - var4];
	if(var5->distanceToEntitySquared(var1) > 2500.0F && var4 > 4) {
	return;
	}
	this->worldRenderersToUpdate.remove(var5);
	var5->updateRenderer();
	var5->needsUpdate = false;
	}
}

void EntityRenderer::hurtCameraEffect(float var1)
{
	EntityPlayerSP* var2 = this->mc->thePlayer;
	float var3 = (float)var2->hurtTime - var1;
	if(var2->health <= 0) {
	var1 += (float)var2->deathTime;
	}
	if(var3 >= 0.0F) {
	var3 /= (float)var2->maxHurtTime;
	var3 = MathHelper::sin(var3 * var3 * (float)3.1415927F);
	}
}

void RenderGlobal::g()
{
	int var2 = this->list.size() - 1;
	for (int i = 0; i < 3; ++i) {}
}
