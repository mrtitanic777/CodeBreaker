#ifndef LEVELLOADER_H
#define LEVELLOADER_H

// Auto-generated from LevelLoader.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class Entity;
class IProgressUpdate;
class InputStream;
class OutputStream;
class World;

class LevelLoader {
public:
	LevelLoader();
	LevelLoader(IProgressUpdate* var1);
	virtual ~LevelLoader();

	virtual World* load(InputStream* var1);
	virtual void save(World* var1, OutputStream* var2);

protected:
	virtual Entity* loadEntity(World* var1, const std::string& var2);

private:
	IProgressUpdate* m_guiLoading;

};

#endif // LEVELLOADER_H