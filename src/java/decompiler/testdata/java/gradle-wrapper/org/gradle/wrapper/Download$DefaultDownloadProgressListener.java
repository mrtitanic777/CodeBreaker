package org.gradle.wrapper;

public final class Download$DefaultDownloadProgressListener {
	public final Logger logger;
	public final Download$DefaultDownloadProgressListener delegate;
	public int previousDownloadPercent;

	public Download$DefaultDownloadProgressListener(Logger var1) {
		this.logger = var1;
		this.delegate = null;
		this.previousDownloadPercent = 0;
	}

	public final void downloadStatusChanged(long param1, long param3) {
		// $FF: Couldn't be decompiled
	}
}
