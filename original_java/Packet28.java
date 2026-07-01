package net.minecraft.src;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class Packet28 extends Packet {
	public int entityId;
	public int velocityX;
	public int velocityY;
	public int velocityZ;

	public Packet28() {
	}

	public Packet28(Entity var1) {
		this(var1.entityId, var1.motionX, var1.motionY, var1.motionZ);
	}

	public Packet28(int var1, double var2, double var4, double var6) {
		this.entityId = var1;
		double var8 = 3.9D;
		if(var2 < -var8) {
			var2 = -var8;
		}

		if(var4 < -var8) {
			var4 = -var8;
		}

		if(var6 < -var8) {
			var6 = -var8;
		}

		if(var2 > var8) {
			var2 = var8;
		}

		if(var4 > var8) {
			var4 = var8;
		}

		if(var6 > var8) {
			var6 = var8;
		}

		this.velocityX = (int)(var2 * 8000.0D);
		this.velocityY = (int)(var4 * 8000.0D);
		this.velocityZ = (int)(var6 * 8000.0D);
	}

	public void readPacketData(DataInputStream var1) throws IOException {
		this.entityId = var1.readInt();
		this.velocityX = var1.readShort();
		this.velocityY = var1.readShort();
		this.velocityZ = var1.readShort();
	}

	public void writePacketData(DataOutputStream var1) throws IOException {
		var1.writeInt(this.entityId);
		var1.writeShort(this.velocityX);
		var1.writeShort(this.velocityY);
		var1.writeShort(this.velocityZ);
	}

	public void processPacket(NetHandler var1) {
		var1.handleEntityVelocity(this);
	}

	public int getPacketSize() {
		return 10;
	}
}
