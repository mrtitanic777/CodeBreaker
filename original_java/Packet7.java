package net.minecraft.src;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class Packet7 extends Packet {
	public int playerEntityId;
	public int targetEntityId;
	public int isLeftClick;

	public Packet7() {
	}

	public Packet7(int var1, int var2, int var3) {
		this.playerEntityId = var1;
		this.targetEntityId = var2;
		this.isLeftClick = var3;
	}

	public void readPacketData(DataInputStream var1) throws IOException {
		this.playerEntityId = var1.readInt();
		this.targetEntityId = var1.readInt();
		this.isLeftClick = var1.readByte();
	}

	public void writePacketData(DataOutputStream var1) throws IOException {
		var1.writeInt(this.playerEntityId);
		var1.writeInt(this.targetEntityId);
		var1.writeByte(this.isLeftClick);
	}

	public void processPacket(NetHandler var1) {
		var1.handleUseEntity(this);
	}

	public int getPacketSize() {
		return 9;
	}
}
