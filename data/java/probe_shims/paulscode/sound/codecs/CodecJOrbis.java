// A compile-and-load stand-in for the judge (probe.py), put ahead of the sound library on the classpath.
// Minecraft shipped its own build of this codec, inside minecraft.jar: the game's CodecMus extends it and
// reads `url` and `urlConnection` and overrides openInputStream(), none of which the public
// codecjorbis-20101023 has (its fields are private). No music is played while judging, so this only has
// the shape the game compiles against.
package paulscode.sound.codecs;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;
import javax.sound.sampled.AudioFormat;
import paulscode.sound.ICodec;
import paulscode.sound.SoundBuffer;

public class CodecJOrbis implements ICodec {
	protected URL url;
	protected URLConnection urlConnection;

	protected InputStream openInputStream() throws IOException {
		return this.urlConnection.getInputStream();
	}

	public void reverseByteOrder(boolean b) {
	}

	public boolean initialize(URL u) {
		this.url = u;
		return false;
	}

	public boolean initialized() {
		return false;
	}

	public SoundBuffer read() {
		return null;
	}

	public SoundBuffer readAll() {
		return null;
	}

	public boolean endOfStream() {
		return true;
	}

	public void cleanup() {
	}

	public AudioFormat getAudioFormat() {
		return null;
	}
}
