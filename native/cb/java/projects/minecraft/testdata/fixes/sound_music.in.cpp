void SoundManager::playRandomMusic()
{
	if(this->loaded && this->options->music) {
	if(!s_sndSystem->playing("BgMusic")) {
	if(this->ticksBeforeMusic > 0) {
	--this->ticksBeforeMusic;
	} else {
	SoundPoolEntry* var1 = this->soundPoolMusic->getRandomSound();
	if(var1 != NULL) {
	this->ticksBeforeMusic = this->rand.nextInt(12000) + 12000;
	s_sndSystem->backgroundMusic("BgMusic", var1->soundUrl, var1->soundName, false);
	s_sndSystem->setVolume("BgMusic", this->options->musicVolume);
	s_sndSystem->play("BgMusic");
	}
	}
	}
	}
}
