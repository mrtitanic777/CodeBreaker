void Minecraft::setLevel(World* var1)
{
	if(this->theWorld != NULL) {
	this->theWorld->setLevel();
	}
	this->theWorld = var1;   // J2X applet-ping-bypass: console has no applet -- getDocumentBase()->openStream() is a NULL-returning stub and readLine() on it FAULTS (an access violation, NOT caught by try/catch). Drop the ping and set the level directly (matches the OG Indev hand-port). [generator: applet_ping]
	if(var1 != NULL) {
	var1->load();
	}
}

void MinecraftApplet::init()
{
	this->mc->minecraftUri = this->getDocumentBase()->getHost();
}
