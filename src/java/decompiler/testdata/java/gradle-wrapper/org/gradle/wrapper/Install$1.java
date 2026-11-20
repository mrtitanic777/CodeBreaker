package org.gradle.wrapper;

import java.io.File;
import java.net.URI;
import java.util.concurrent.Callable;

public final class Install$1 implements Callable {
	public final File val$localZipFile;
	public final File val$distDir;
	public final URI val$distributionUrl;
	public final WrapperConfiguration val$configuration;
	public final Install this$0;

	public Install$1(File var1, File var2, URI var3, Install var4, WrapperConfiguration var5) {
		this.this$0 = var4;
		this.val$localZipFile = var1;
		this.val$distDir = var2;
		this.val$distributionUrl = var3;
		this.val$configuration = var5;
	}

	public final Object call() {
		// $FF: Couldn't be decompiled
	}
}
