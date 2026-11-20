package org.gradle.internal.file.locking;

import java.io.Closeable;

public final class ExclusiveFileAccessManager {
	public static void maybeCloseQuietly(Closeable var0) {
		if(var0 != null) {
			try {
				var0.close();
			} catch (Exception var1) {
			}
		}

	}
}
