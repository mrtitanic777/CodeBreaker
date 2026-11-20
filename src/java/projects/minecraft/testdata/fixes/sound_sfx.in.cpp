void SoundManager::playSound(JavaString name, float var2, float var3, float var4, float var5, float var6)
{
	if(this->loaded && this->options->sound) {
	SoundPoolEntry* var7 = this->soundPoolSounds->getRandomSoundFromSoundPool(name);
	if(var7 != NULL && var5 > 0.0F) {
	s_sndSystem->newSource(false, var7->soundUrl, var7->soundName, false, var2, var3, var4, 2, 16.0F);
	s_sndSystem->setVolume(var7->soundName, var5 * this->options->soundVolume);
	s_sndSystem->play(var7->soundName);
	}
	}
}

void SoundManager::playSoundFX(JavaString var1, float var2, float var3)
{
	if(this->loaded && this->options->sound) {
	SoundPoolEntry* var4 = this->soundPoolSounds->getRandomSoundFromSoundPool(var1);
	if(var4 != NULL) {
	s_sndSystem->newSource(false, var4->soundUrl, var4->soundName, false, 0.0F, 0.0F, 0.0F, 0, 0.0F);
	s_sndSystem->play(var4->soundName);
	}
	}
}

void SoundManager::playStreaming(JavaString var1, float var2, float var3, float var4)
{
	SoundPoolEntry* var5 = this->soundPoolStreaming->getRandomSoundFromSoundPool(var1);
	if(var5 != NULL) {
	s_sndSystem->newStreamingSource(var5->soundName);
	}
}
