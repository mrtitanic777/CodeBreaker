#pragma once

// XUR: the binary scene format of XUI (the Xbox 360's UI library), version 8 as XAM 17559's packages have it, and
// version 5 (the XDK's xui2bin and the Blades / NXE dashboards: inline values, xur.cpp "Version 5"). A scene is a tree of objects (an XUI class name, property values, children, timelines); values are
// stored as indices into shared pools (strings, floats, vectors, quaternions, colours, figure paths) and every object
// writes one property mask per class of its class chain (base class first), so a reader needs each class's property
// list in registration order. The lists here are those of the XDK 21256 XUI runtime and XAM 17559 (checked: the XDK's
// skin_default.xzp against its XML source skin_default.xui, and every scene of the dashboard 17559 packages reads to
// the exact end of its object stream; CB_XUI_TEST=1).
//
// Layout (big-endian; "packed" = 1 byte < 0xF0, 0xFn + 1 byte = 12 bits, 0xFF + 4 bytes):
//   header: "XUIB", u32 version 8, u32 flags, u16 0x0E, u32 total size, u16 section count, 12 packed pool sizes
//   section table: { char tag[4]; u32 offset (from the file start); u32 size } per section
//   STRN: u32 size, u16 count, NUL-terminated UTF-8 strings (index 1 = first; 0 = empty)
//   FLOT/VECT/QUAT/COLR: arrays of floats, float[3], float[4], ARGB u32
//   CUST: figure paths (u32 size, float width, height, u32 points, points x {point, control 1, control 2} as float[2])
//   KEYP: packed key values; KEYD: key frames; NAME: named frames; DATA: the object tree.
// Object: packed class (string index), u8 flags (1 properties, 2 children, 4 timelines), [packed value count, one mask
// per class level + values], [packed child count, children], [timelines].

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace xui
{
    enum class Type : uint8_t // XUI_ELEMENT_PROPERTY_TYPE
    {
        kEmpty, kBool, kInteger, kUnsigned, kFloat, kString, kColor, kVector, kQuaternion, kObject, kCustom
    };

    constexpr uint32_t kPropIndexed = 0x1; // XUI_ELEMENT_PROPF_INDEXED: an array, written with its count

    struct PropTable;
    struct PropDef
    {
        const char* name;
        Type type;
        uint32_t flags;
        const PropTable* sub; // kObject: the object's own properties
    };
    struct PropTable
    {
        const PropDef* defs;
        uint32_t count;
    };
    struct ClassDef
    {
        const char* name;
        const char* base;
        PropTable props;
    };

    const ClassDef* FindClass(std::string_view name);
    // Base class first; empty if the class or one of its bases is unknown.
    std::vector<const ClassDef*> ClassChain(std::string_view name);

    struct Vec3 { float x = 0, y = 0, z = 0; };
    struct Quat { float x = 0, y = 0, z = 0, w = 1; };

    struct Node;
    struct Value
    {
        Type type = Type::kEmpty;
        bool b = false;
        int64_t i = 0;          // kInteger (signed), kUnsigned
        float f = 0;
        std::string s;          // UTF-8
        uint32_t color = 0;     // ARGB
        Vec3 v;
        Quat q;
        uint32_t custom = 0;    // kCustom: byte offset of the figure path in CUST
        std::shared_ptr<const Node> object; // kObject: a property object (brushes, gradients)
    };

    struct Property
    {
        const PropDef* def = nullptr;
        std::vector<Value> values; // one unless indexed
    };

    // One animated property of a timeline: the class level and index of the property in the target's class chain,
    // then the index inside each nested object property (Fill -> FillColor).
    struct Track
    {
        uint8_t level = 0, index = 0;
        std::vector<uint8_t> path;
        bool hasIndex = false;    // an indexed property: which element
        uint32_t element = 0;
        const PropDef* def = nullptr; // resolved against the target's class (null if it cannot be)
        std::string name;             // "Fill.FillColor"
    };

    // XUI_INTERPOLATE (0-2, the XDK's) and the curves XAM's newer XUI adds (FUN_817a9fa0 in 17559's xam.xex).
    enum class Interpolation : uint8_t
    {
        kLinear = 0, kNone = 1, kEase = 2, kQuadratic = 3, kCubic = 4, kQuartic = 5, kQuintic = 6,
        kPower = 7,       // t^p1
        kSine = 8, kCircular = 9,
        kExponential = 10, // (e^(p1 t) - 1) / (e^p1 - 1)
        kElastic = 11,     // the exponential (p1) times sin((2 pi p2 + pi/2) t): p2 oscillations
        kBounce = 12,      // p1 bounces, each 1/p2 as high as the next
    };

    // Which end of a segment a curve shapes (key mode bits 6-7): the curve itself, mirrored, or both halves.
    enum class EaseDirection : uint8_t { kIn = 0, kOut = 1, kInOut = 2 };

    struct KeyFrame
    {
        uint32_t frame = 0;
        Interpolation interpolation = Interpolation::kLinear;
        EaseDirection direction = EaseDirection::kIn;
        int8_t easeIn = 0, easeOut = 0, easeScale = 0; // kEase: -100..100, -100..100, 0..100
        // The curve's two parameters: kEase's Bezier control heights (from easeIn/Out/Scale), or the x and y of the
        // key's vector (kPower, kExponential, kElastic, kBounce); 0 otherwise.
        float p1 = 0, p2 = 0;
        std::vector<Value> values; // one per track
    };

    // kEase's Bezier control heights from the key's ease bytes (XAM FUN_817bf208, the XDK's _ComputeEaseCurve).
    void EaseControls(int easeIn, int easeOut, int easeScale, float& p1, float& p2);
    // The value fraction of the segment after key k at time fraction t (XAM FUN_817aa2c0 + FUN_817a9fa0).
    float CurveFraction(const KeyFrame& k, float t);

    struct Timeline
    {
        std::string target; // Id of the child it animates
        std::vector<Track> tracks;
        std::vector<KeyFrame> keys;
    };

    enum class Command : uint8_t { kPlay = 0, kStop = 1, kGoto = 2, kGotoAndPlay = 3, kGotoAndStop = 4 }; // XUI_COMMAND

    struct NamedFrame
    {
        std::string name;
        uint32_t frame = 0;
        Command command = Command::kPlay;
        std::string target; // kGoto*: the named frame to go to
    };

    struct Node
    {
        std::string className;
        std::vector<Property> props; // in class-chain order
        std::vector<Node> children;
        std::vector<NamedFrame> namedFrames;
        std::vector<Timeline> timelines;

        const Value* Find(std::string_view name) const;
        std::string Id() const;
    };

    struct FigurePoint { float x, y, c1x, c1y, c2x, c2y; };
    struct FigurePath
    {
        float width = 0, height = 0;
        std::vector<FigurePoint> points;
    };

    class Scene
    {
    public:
        // Returns false with a reason in `error`. `version8Control`: a version 5 scene's XuiControl has version 8's
        // properties (NavTabForward / NavTabBackward included): the NXE dashboards' own scenes (6683 on) write it so;
        // XAM's and the Blades dashboards' version 5 scenes do not (kControlV5, xur.cpp).
        bool Load(const uint8_t* data, size_t size, std::string& error, bool version8Control = false);
        const Node& Root() const { return root_; }
        const FigurePath* Figure(uint32_t offset) const;

    private:
        struct Reader;
        bool ReadPools(std::string& error);
        bool ReadObject(Reader& r, Node& node, std::string& error, int depth);
        bool ReadValues(Reader& r, const PropTable& table, std::vector<Property>& out, std::string& error);
        bool ReadValue(Reader& r, const PropDef& def, Value& out, std::string& error);
        bool ReadTimelines(Reader& r, Node& node, std::string& error);
        bool ResolveTrack(const std::string& className, Track& track) const;
        Value PoolValue(Type type, uint32_t index) const;
        struct V5Reader;
        bool LoadV5(std::string& error); // version 5: the XDK's and the Blades / NXE dashboards' (xur.cpp)

        bool version8Control_ = false;
        std::vector<uint8_t> data_;
        struct Section { const uint8_t* data; size_t size; };
        std::map<std::string, Section> sections_;
        std::vector<std::string> strings_;
        std::vector<float> floats_;
        std::vector<Vec3> vectors_;
        std::vector<Quat> quats_;
        std::vector<uint32_t> colors_;
        std::map<uint32_t, FigurePath> figures_;
        std::vector<uint32_t> keyValues_;
        struct KeyRecord { uint32_t frame; uint8_t mode; int8_t ease[3]; uint32_t vector; uint32_t valueBase; };
        std::vector<KeyRecord> keyRecords_;
        // Every object written with its properties, in stream order: an object with flag 8 shares the properties
        // of one of them (by index).
        std::vector<const Node*> propertyBlocks_;
        std::vector<NamedFrame> nameRecords_;
        std::map<uint32_t, std::shared_ptr<Node>> objects_; // property objects by pool index
        Node root_;
    };

    // Text of a scene for diagnostics (CB_XUI_TEST dumps).
    std::string DumpNode(const Node& node, int depth = 0);
}
