package org.gradle.wrapper;

import java.io.File;
import java.io.FileInputStream;
import java.util.Properties;

public final class WrapperExecutor {
	public final Properties properties;
	public final File propertiesFile;
	public final WrapperConfiguration config;

	public WrapperExecutor(File param1, Properties param2) {
		// $FF: Couldn't be decompiled
	}

	public static void loadProperties(File var0, Properties var1) {
		// $FF: Couldn't be decompiled
	}

	public final String readDistroUrl() {
		if(this.properties.getProperty("distributionUrl") != null) {
			return this.getProperty("distributionUrl", (String)null, true);
		} else {
			File var2 = this.propertiesFile;
			Object[] var1;
			Object[] var10002 = var1 = new Object[2];
			var10002[0] = "distributionUrl";
			var10002[1] = var2;
			throw new RuntimeException(String.format("No value with key \'%s\' specified in wrapper properties file \'%s\'.", var1));
		}
	}

	public final String getProperty(String var1, String var2) {
		return this.getProperty(var1, var2, true);
	}

	public final int getProperty(int var1) {
		WrapperExecutor var10000 = this;
		String var2 = "networkTimeout";
		return Integer.parseInt(var10000.getProperty(var2, String.valueOf(var1), true));
	}

	public final boolean getProperty(boolean var1) {
		WrapperExecutor var10000 = this;
		String var2 = "validateDistributionUrl";
		return Boolean.parseBoolean(var10000.getProperty(var2, String.valueOf(var1), true));
	}

	public final String getProperty(String var1, String var2, boolean var3) {
		String var4;
		if((var4 = this.properties.getProperty(var1)) != null) {
			return var4;
		} else if(var2 != null) {
			return var2;
		} else if(!var3) {
			return null;
		} else {
			File var5 = this.propertiesFile;
			Object[] var6;
			Object[] var10002 = var6 = new Object[2];
			var10002[0] = var1;
			var10002[1] = var5;
			throw new RuntimeException(String.format("No value with key \'%s\' specified in wrapper properties file \'%s\'.", var6));
		}
	}
}
