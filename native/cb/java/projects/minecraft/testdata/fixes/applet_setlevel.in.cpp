void Minecraft::setLevel(World* var1)
{
	if(this->theWorld != NULL) {
	this->theWorld->setLevel();
	}
	try {
	BufferedReader* var2 = new BufferedReader(new InputStreamReader((new URL((JCat() + this->mcApplet->getDocumentBase() + "?n=" + this->session->username + "&i=" + this->session->sessionId)))->openStream()));
	Integer::parseInt(var2->readLine());
	var2->close();
	if(this->mcApplet->getDocumentBase()->toString().startsWith("http://www.minecraft.net/") || this->mcApplet->getDocumentBase()->toString().startsWith("http://minecraft.net/")) {
	this->theWorld = var1;
	}
	} catch (Throwable*) {
	}
	if(var1 != NULL) {
	var1->load();
	}
}

void MinecraftApplet::init()
{
	this->mc->minecraftUri = this->getDocumentBase()->getHost();
}
