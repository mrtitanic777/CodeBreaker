#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet.h"
#include "Class.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "Exception.h"
#include "IOException.h"
#include "IllegalArgumentException.h"
#include "NetHandler.h"
#include <typeinfo>

// ---- Headers of EVERY registered packet class (one per addIdClassMapping below).
//      Needed so typeid(PacketX) sees a complete type and new PacketX() compiles.
#include "Packet0KeepAlive.h"
#include "Packet1Login.h"
#include "Packet2Handshake.h"
#include "Packet3Chat.h"
#include "Packet4UpdateTime.h"
#include "Packet5PlayerInventory.h"
#include "Packet6SpawnPosition.h"
#include "Packet7.h"
#include "Packet8.h"
#include "Packet9.h"
#include "Packet10Flying.h"
#include "Packet11PlayerPosition.h"
#include "Packet12PlayerLook.h"
#include "Packet13PlayerLookMove.h"
#include "Packet14BlockDig.h"
#include "Packet15Place.h"
#include "Packet16BlockItemSwitch.h"
#include "Packet17AddToInventory.h"
#include "Packet18ArmAnimation.h"
#include "Packet20NamedEntitySpawn.h"
#include "Packet21PickupSpawn.h"
#include "Packet22Collect.h"
#include "Packet23VehicleSpawn.h"
#include "Packet24MobSpawn.h"
#include "Packet28.h"
#include "Packet29DestroyEntity.h"
#include "Packet30Entity.h"
#include "Packet31RelEntityMove.h"
#include "Packet32EntityLook.h"
#include "Packet33RelEntityMoveLook.h"
#include "Packet34EntityTeleport.h"
#include "Packet38.h"
#include "Packet39.h"
#include "Packet50PreChunk.h"
#include "Packet51MapChunk.h"
#include "Packet52MultiBlockChange.h"
#include "Packet53BlockChange.h"
#include "Packet59ComplexEntity.h"
#include "Packet60.h"
#include "Packet255KickDisconnect.h"

// ---- Template factory: one per ctor signature. Packets use the NO-ARG ctor
//      (Java did `class.newInstance()`), so a single zero-arg factory covers all.
typedef void* (*PacketFactory)();
template<class T> static void* j2x_new0() { return new T(); }

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // The maps below are keyed by java.lang.Class objects. The C++ port can't use
 *  // Class identity as a map key, so it keys the class-side map on the RTTI type
 *  // name string (typeid(T).name()) -- a stable, unique-per-class std::string that
 *  // matches typeid(*instance).name() at runtime, mirroring Java's Class identity.
 *  private static Map packetIdToClassMap = new HashMap();
 *  private static Map packetClassToIdMap = new HashMap();
 *  ---- end original Java ---- */
// id -> class : we store the construction factory (cast to void*) so this public
//               map is the live id->"class" table (non-empty, drives getNewPacket).
std::map<int, void*> Packet::s_packetIdToClassMap;
// class -> id : keyed by RTTI identity string (typeid name), value = packet id.
std::map<std::string, int> Packet::s_packetClassToIdMap;

Packet::Packet()
	: isChunkDataPacket(0)
{
}

Packet::~Packet()
{
}

// Real registration. Keys the class-side map by the RTTI identity name and stores
// the no-arg factory (as void*) in the id-side map so packets can be created by id.
static void _registerPacket(int id, const char* classKey, PacketFactory factory)
{
	if(Packet::s_packetIdToClassMap.count(id)) {
		throw new IllegalArgumentException(JavaString(std::string(JCat() + "Duplicate packet id:" + id)));
	} else if(Packet::s_packetClassToIdMap.count(std::string(classKey))) {
		throw new IllegalArgumentException(JavaString(std::string(JCat() + "Duplicate packet class:" + classKey)));
	} else {
		Packet::s_packetIdToClassMap[id] = (void*)factory;
		Packet::s_packetClassToIdMap[std::string(classKey)] = id;
	}
}

// Public API kept stable (other code may call it). Routes a Class* registration
// through the RTTI-keyed table using the Class stub's name.
void Packet::addIdClassMapping(int var0, Class* var1)
{
	const char* key = var1 ? var1->getName().c_str() : "";
	if(s_packetIdToClassMap.count(var0)) {
		throw new IllegalArgumentException(JavaString(std::string(JCat() + "Duplicate packet id:" + var0)));
	} else if(s_packetClassToIdMap.count(std::string(key))) {
		throw new IllegalArgumentException(JavaString(std::string(JCat() + "Duplicate packet class:" + key)));
	} else {
		s_packetIdToClassMap[var0] = (void*)var1;
		s_packetClassToIdMap[std::string(key)] = var0;
	}
}

Packet* Packet::getNewPacket(int var0)
{
	try {
		std::map<int, void*>::iterator it = s_packetIdToClassMap.find(var0);
		if(it == s_packetIdToClassMap.end() || it->second == NULL) {
			return NULL;
		}
		PacketFactory factory = (PacketFactory)it->second;
		return (Packet*)factory();
	} catch (Exception var2) {
		var2.printStackTrace();
		System::out.println((JCat() + "Skipping packet with id " + var0));
		return NULL;
	}
}

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // Java looks up the packet id by this instance's runtime Class object.
 *  // The C++ port keys the map on the RTTI type name (typeid(*this).name()),
 *  // which DOES match the typeid(T).name() used at registration time -- so the
 *  // lookup now resolves correctly, exactly like Java's getClass()-keyed map.
 *  public final int getPacketId() {
 *      return ((Integer)packetClassToIdMap.get(this.getClass())).intValue();
 *  }
 *  ---- end original Java ---- */
int Packet::getPacketId()
{
	return s_packetClassToIdMap[std::string(typeid(*this).name())];
}

Packet* Packet::readPacket(DataInputStream* var0)
{
	int var1 = var0->read();
	if(var1 == -1) {
		return NULL;
	} else {
		Packet* var2 = getNewPacket(var1);
		if(var2 == NULL) {
			throw new IOException(JavaString(std::string(JCat() + "Bad packet id " + var1)));
		} else {
			var2->readPacketData(var0);
			return var2;
		}
	}
}

void Packet::writePacket(Packet* var0, DataOutputStream* var1)
{
	var1->write(var0->getPacketId());
	var0->writePacketData(var1);
}

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  static {
 *      addIdClassMapping(0, Packet0KeepAlive.class);
 *      addIdClassMapping(1, Packet1Login.class);
 *      addIdClassMapping(2, Packet2Handshake.class);
 *      addIdClassMapping(3, Packet3Chat.class);
 *      addIdClassMapping(4, Packet4UpdateTime.class);
 *      addIdClassMapping(5, Packet5PlayerInventory.class);
 *      addIdClassMapping(6, Packet6SpawnPosition.class);
 *      addIdClassMapping(7, Packet7.class);
 *      addIdClassMapping(8, Packet8.class);
 *      addIdClassMapping(9, Packet9.class);
 *      addIdClassMapping(10, Packet10Flying.class);
 *      addIdClassMapping(11, Packet11PlayerPosition.class);
 *      addIdClassMapping(12, Packet12PlayerLook.class);
 *      addIdClassMapping(13, Packet13PlayerLookMove.class);
 *      addIdClassMapping(14, Packet14BlockDig.class);
 *      addIdClassMapping(15, Packet15Place.class);
 *      addIdClassMapping(16, Packet16BlockItemSwitch.class);
 *      addIdClassMapping(17, Packet17AddToInventory.class);
 *      addIdClassMapping(18, Packet18ArmAnimation.class);
 *      addIdClassMapping(20, Packet20NamedEntitySpawn.class);
 *      addIdClassMapping(21, Packet21PickupSpawn.class);
 *      addIdClassMapping(22, Packet22Collect.class);
 *      addIdClassMapping(23, Packet23VehicleSpawn.class);
 *      addIdClassMapping(24, Packet24MobSpawn.class);
 *      addIdClassMapping(28, Packet28.class);
 *      addIdClassMapping(29, Packet29DestroyEntity.class);
 *      addIdClassMapping(30, Packet30Entity.class);
 *      addIdClassMapping(31, Packet31RelEntityMove.class);
 *      addIdClassMapping(32, Packet32EntityLook.class);
 *      addIdClassMapping(33, Packet33RelEntityMoveLook.class);
 *      addIdClassMapping(34, Packet34EntityTeleport.class);
 *      addIdClassMapping(38, Packet38.class);
 *      addIdClassMapping(39, Packet39.class);
 *      addIdClassMapping(50, Packet50PreChunk.class);
 *      addIdClassMapping(51, Packet51MapChunk.class);
 *      addIdClassMapping(52, Packet52MultiBlockChange.class);
 *      addIdClassMapping(53, Packet53BlockChange.class);
 *      addIdClassMapping(59, Packet59ComplexEntity.class);
 *      addIdClassMapping(60, Packet60.class);
 *      addIdClassMapping(255, Packet255KickDisconnect.class);
 *  }
 *  ---- end original Java ---- */
// C++ equivalent of the Java static{} block: fills both maps with RTTI identity + no-arg factories.
// Run in dependency order by j2x_initAll() (was a file-local auto-trigger -- undefined cross-TU order).
void Packet::initStatics()
{
			_registerPacket(0,   typeid(Packet0KeepAlive).name(),       &j2x_new0<Packet0KeepAlive>);
			_registerPacket(1,   typeid(Packet1Login).name(),          &j2x_new0<Packet1Login>);
			_registerPacket(2,   typeid(Packet2Handshake).name(),      &j2x_new0<Packet2Handshake>);
			_registerPacket(3,   typeid(Packet3Chat).name(),           &j2x_new0<Packet3Chat>);
			_registerPacket(4,   typeid(Packet4UpdateTime).name(),     &j2x_new0<Packet4UpdateTime>);
			_registerPacket(5,   typeid(Packet5PlayerInventory).name(),&j2x_new0<Packet5PlayerInventory>);
			_registerPacket(6,   typeid(Packet6SpawnPosition).name(),  &j2x_new0<Packet6SpawnPosition>);
			_registerPacket(7,   typeid(Packet7).name(),               &j2x_new0<Packet7>);
			_registerPacket(8,   typeid(Packet8).name(),               &j2x_new0<Packet8>);
			_registerPacket(9,   typeid(Packet9).name(),               &j2x_new0<Packet9>);
			_registerPacket(10,  typeid(Packet10Flying).name(),        &j2x_new0<Packet10Flying>);
			_registerPacket(11,  typeid(Packet11PlayerPosition).name(),&j2x_new0<Packet11PlayerPosition>);
			_registerPacket(12,  typeid(Packet12PlayerLook).name(),    &j2x_new0<Packet12PlayerLook>);
			_registerPacket(13,  typeid(Packet13PlayerLookMove).name(),&j2x_new0<Packet13PlayerLookMove>);
			_registerPacket(14,  typeid(Packet14BlockDig).name(),      &j2x_new0<Packet14BlockDig>);
			_registerPacket(15,  typeid(Packet15Place).name(),         &j2x_new0<Packet15Place>);
			_registerPacket(16,  typeid(Packet16BlockItemSwitch).name(),&j2x_new0<Packet16BlockItemSwitch>);
			_registerPacket(17,  typeid(Packet17AddToInventory).name(),&j2x_new0<Packet17AddToInventory>);
			_registerPacket(18,  typeid(Packet18ArmAnimation).name(),  &j2x_new0<Packet18ArmAnimation>);
			_registerPacket(20,  typeid(Packet20NamedEntitySpawn).name(),&j2x_new0<Packet20NamedEntitySpawn>);
			_registerPacket(21,  typeid(Packet21PickupSpawn).name(),   &j2x_new0<Packet21PickupSpawn>);
			_registerPacket(22,  typeid(Packet22Collect).name(),       &j2x_new0<Packet22Collect>);
			_registerPacket(23,  typeid(Packet23VehicleSpawn).name(),  &j2x_new0<Packet23VehicleSpawn>);
			_registerPacket(24,  typeid(Packet24MobSpawn).name(),      &j2x_new0<Packet24MobSpawn>);
			_registerPacket(28,  typeid(Packet28).name(),              &j2x_new0<Packet28>);
			_registerPacket(29,  typeid(Packet29DestroyEntity).name(), &j2x_new0<Packet29DestroyEntity>);
			_registerPacket(30,  typeid(Packet30Entity).name(),        &j2x_new0<Packet30Entity>);
			_registerPacket(31,  typeid(Packet31RelEntityMove).name(), &j2x_new0<Packet31RelEntityMove>);
			_registerPacket(32,  typeid(Packet32EntityLook).name(),    &j2x_new0<Packet32EntityLook>);
			_registerPacket(33,  typeid(Packet33RelEntityMoveLook).name(),&j2x_new0<Packet33RelEntityMoveLook>);
			_registerPacket(34,  typeid(Packet34EntityTeleport).name(),&j2x_new0<Packet34EntityTeleport>);
			_registerPacket(38,  typeid(Packet38).name(),              &j2x_new0<Packet38>);
			_registerPacket(39,  typeid(Packet39).name(),              &j2x_new0<Packet39>);
			_registerPacket(50,  typeid(Packet50PreChunk).name(),      &j2x_new0<Packet50PreChunk>);
			_registerPacket(51,  typeid(Packet51MapChunk).name(),      &j2x_new0<Packet51MapChunk>);
			_registerPacket(52,  typeid(Packet52MultiBlockChange).name(),&j2x_new0<Packet52MultiBlockChange>);
			_registerPacket(53,  typeid(Packet53BlockChange).name(),   &j2x_new0<Packet53BlockChange>);
			_registerPacket(59,  typeid(Packet59ComplexEntity).name(), &j2x_new0<Packet59ComplexEntity>);
			_registerPacket(60,  typeid(Packet60).name(),              &j2x_new0<Packet60>);
			_registerPacket(255, typeid(Packet255KickDisconnect).name(),&j2x_new0<Packet255KickDisconnect>);
}
