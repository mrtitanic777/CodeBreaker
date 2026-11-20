void SoundManager::playRandomMusic()
{
	if(this->loaded && this->options->music) {
	if(!s_sndSystem->playing("BgMusic")) {
	if(this->ticksBeforeMusic > 0) {
	--this->ticksBeforeMusic;
	} else {
	SoundPoolEntry* var1 = this->soundPoolMusic->getRandomSound();   // J2X music-bypass: console music pool is empty; run the play() path unconditionally so the XACT playlist starts, keeping only the SoundPoolEntry deref (backgroundMusic) guarded. [generator: sound_fix]
	if(true) {
	this->ticksBeforeMusic = this->rand.nextInt(12000) + 12000;
	if(var1 != NULL) s_sndSystem->backgroundMusic("BgMusic", var1->soundUrl, var1->soundName, false);
	s_sndSystem->setVolume("BgMusic", this->options->musicVolume);
	s_sndSystem->play("BgMusic");
	}
	}
	}
	}
}
