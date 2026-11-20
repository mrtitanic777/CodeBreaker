package org.gradle.wrapper;

import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import org.gradle.internal.file.locking.ExclusiveFileAccessManager;

public final class Install {
	public final Logger logger;
	public final Download download;
	public final ExclusiveFileAccessManager exclusiveFileAccessManager;

	public Install(Logger var1, Download var2, PathAssembler var3) {
		// $FF: Couldn't be decompiled
	}

	public static Install$InstallCheck access$000(Install var0, File var1, String var2) {
		// $FF: Couldn't be decompiled
	}

	public static void verifyDownloadChecksum(String param0, File param1, String param2) {
		// $FF: Couldn't be decompiled
	}

	public static boolean deleteDir(File var0) {
		String[] var1;
		if(var0.isDirectory() && (var1 = var0.list()) != null) {
			for(int var2 = 0; var2 < var1.length; ++var2) {
				if(!deleteDir(new File(var0, var1[var2]))) {
					return false;
				}
			}
		}

		return var0.delete();
	}

	public static void unzip(File param0, File param1) {
		// $FF: Couldn't be decompiled
	}

	public final void unzipLocal(File var1, File var2) {
		try {
			unzip(var1, var2);
		} catch (IOException var4) {
			this.logger.log("Could not unzip " + var1.getAbsolutePath() + " to " + var2.getAbsolutePath() + ".");
			this.logger.log("Reason: " + var4.getMessage());
			throw var4;
		}
	}

	public final void forceFetch(File param1, URI param2) {
		// $FF: Couldn't be decompiled
	}
}
