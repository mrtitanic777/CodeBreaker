package org.gradle.internal.file;

import java.util.Locale;

public abstract class PathTraversalChecker {
	public static String safePathName(String var0) {
		if(var0.isEmpty() || var0.startsWith("/") || var0.startsWith("\\") || var0.contains("..") && (var0.endsWith("\\..") || var0.contains("..\\") || var0.endsWith("/..") || var0.contains("../")) || var0.contains(":") && System.getProperty("os.name").toLowerCase(Locale.US).contains("windows")) {
			Object[] var1;
			(var1 = new Object[1])[0] = var0;
			throw new IllegalArgumentException(String.format("\'%s\' is not a safe archive entry or path name.", var1));
		} else {
			return var0;
		}
	}
}
