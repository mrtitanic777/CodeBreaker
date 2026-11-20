void SoundManager::playSound(JavaString name, float var2, float var3, float var4, float var5, float var6)
{
	if(this->loaded && this->options->sound) {
	SoundPoolEntry* var7 = NULL;   // J2X sfx-bypass: console OGG pool is empty; drop the null-guard and route the sound name to the XACT bridge (newSource identifier == cue). [generator: sound_fix]
	if(var5 > 0.0F) {
	s_sndSystem->newSource(false, (void*)NULL, name, false, var2, var3, var4, 2, 16.0F);
	s_sndSystem->setVolume(name, var5 * this->options->soundVolume);
	s_sndSystem->play(name);
	}
	}
}

void SoundManager::playSoundFX(JavaString var1, float var2, float var3)
{
	if(this->loaded && this->options->sound) {
	SoundPoolEntry* var4 = NULL;   // J2X sfx-bypass: console OGG pool is empty; drop the null-guard and route the sound name to the XACT bridge (newSource identifier == cue). [generator: sound_fix]
	if(true) {
	s_sndSystem->newSource(false, (void*)NULL, var1, false, 0.0F, 0.0F, 0.0F, 0, 0.0F);
	s_sndSystem->play(var1);
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
