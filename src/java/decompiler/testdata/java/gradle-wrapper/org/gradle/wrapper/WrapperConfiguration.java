package org.gradle.wrapper;

import java.net.URI;

public final class WrapperConfiguration {
	public URI distribution;
	public String distributionBase = "GRADLE_USER_HOME";
	public String distributionPath = "wrapper/dists";
	public String distributionSha256Sum;
	public String zipBase = "GRADLE_USER_HOME";
	public String zipPath = "wrapper/dists";
	public int networkTimeout = 10000;
	public boolean validateDistributionUrl = true;

	public final void setDistribution(URI var1) {
		this.distribution = var1;
	}

	public final String getDistributionBase() {
		return this.distributionBase;
	}

	public final void setDistributionBase(String var1) {
		this.distributionBase = var1;
	}

	public final String getDistributionPath() {
		return this.distributionPath;
	}

	public final void setDistributionPath(String var1) {
		this.distributionPath = var1;
	}

	public final String getDistributionSha256Sum() {
		return this.distributionSha256Sum;
	}

	public final void setDistributionSha256Sum(String var1) {
		this.distributionSha256Sum = var1;
	}

	public final String getZipBase() {
		return this.zipBase;
	}

	public final void setZipBase(String var1) {
		this.zipBase = var1;
	}

	public final String getZipPath() {
		return this.zipPath;
	}

	public final void setZipPath(String var1) {
		this.zipPath = var1;
	}

	public final int getNetworkTimeout() {
		return this.networkTimeout;
	}

	public final void setNetworkTimeout(int var1) {
		this.networkTimeout = var1;
	}

	public final boolean getValidateDistributionUrl() {
		return this.validateDistributionUrl;
	}

	public final void setValidateDistributionUrl(boolean var1) {
		this.validateDistributionUrl = var1;
	}
}
