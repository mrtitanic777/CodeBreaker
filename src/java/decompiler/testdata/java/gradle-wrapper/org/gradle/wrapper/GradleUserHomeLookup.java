package org.gradle.wrapper;

public abstract class GradleUserHomeLookup {
	public static final String DEFAULT_GRADLE_USER_HOME = System.getProperty("user.home") + "/.gradle";
}
