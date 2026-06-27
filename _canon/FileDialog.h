#ifndef FILEDIALOG_H
#define FILEDIALOG_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.awt.FileDialog.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class FileDialog {
public:
	FileDialog(...) {}
	virtual ~FileDialog() {}
	virtual void setVisible(...) {}
	virtual void setMode(...) {}
	virtual void setDirectory(...) {}
	virtual void setFile(...) {}
	virtual void setFilenameFilter(...) {}
	virtual void setLocationRelativeTo(...) {}
	virtual std::string getFile(...) { return std::string(); }
	virtual std::string getDirectory(...) { return std::string(); }
};
#endif // FILEDIALOG_H
