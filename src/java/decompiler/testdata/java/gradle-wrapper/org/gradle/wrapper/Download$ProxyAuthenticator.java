package org.gradle.wrapper;

import java.net.Authenticator;
import java.net.PasswordAuthentication;
import java.net.Authenticator.RequestorType;
import java.util.Map;

public final class Download$ProxyAuthenticator extends Authenticator {
	public final Map systemProperties;

	public Download$ProxyAuthenticator(Map var1) {
		this.systemProperties = var1;
	}

	public final PasswordAuthentication getPasswordAuthentication() {
		if(this.getRequestorType() == RequestorType.PROXY) {
			String var1 = this.getRequestingURL().getProtocol();
			String var2;
			if((var2 = (String)this.systemProperties.get(var1 + ".proxyUser")) != null) {
				String var3;
				if((var3 = (String)this.systemProperties.get(var1 + ".proxyPassword")) == null) {
					var3 = "";
				}

				return new PasswordAuthentication(var2, var3.toCharArray());
			}
		}

		return super.getPasswordAuthentication();
	}
}
