package org.gradle.wrapper;

import java.net.Authenticator;
import java.net.URI;
import java.net.URLConnection;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

public final class Download {
	public final Logger logger;
	public final Download$DefaultDownloadProgressListener progressListener;
	public final Map systemProperties;
	public final int networkTimeout;

	public static HashMap convertSystemProperties(Properties var0) {
		// $FF: Couldn't be decompiled
	}

	public Download(Logger var1, HashMap var2, int var3) {
		// $FF: Couldn't be decompiled
	}

	public static URI safeUri(URI param0) {
		// $FF: Couldn't be decompiled
	}

	public final void configureProxyAuthentication() {
		if(this.systemProperties.get("http.proxyUser") != null || this.systemProperties.get("https.proxyUser") != null) {
			Authenticator.setDefault(new Download$ProxyAuthenticator(this.systemProperties));
		}

	}

	public final void addBasicAuthentication(URI param1, URLConnection param2) {
		// $FF: Couldn't be decompiled
	}

	public final String calculateUserAgent() {
		Download var10000 = this;
		Download var10001 = this;
		Download var10002 = this;
		Download var10003 = this;
		Download var10004 = this;
		String var7 = (String)this.systemProperties.get("java.vendor");
		String var1 = (String)var10004.systemProperties.get("java.version");
		String var2 = (String)var10003.systemProperties.get("java.vm.version");
		String var3 = (String)var10002.systemProperties.get("os.name");
		String var4 = (String)var10001.systemProperties.get("os.version");
		String var5 = (String)var10000.systemProperties.get("os.arch");
		Object[] var6;
		Object[] var8 = var6 = new Object[8];
		var8[0] = "gradlew";
		var8[1] = "0";
		var8[2] = var3;
		var8[3] = var4;
		var8[4] = var5;
		var8[5] = var7;
		var8[6] = var1;
		var8[7] = var2;
		return String.format("%s/%s (%s;%s;%s) (%s;%s;%s)", var6);
	}
}
