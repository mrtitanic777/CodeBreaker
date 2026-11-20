package org.gradle.wrapper;

import java.io.File;
import java.io.FilenameFilter;

public final class BootstrapMainStarter$1 implements FilenameFilter {
	public final boolean accept(File var1, String var2) {
		return var2.matches("gradle-launcher-.*\\.jar");
	}
}
