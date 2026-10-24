// XUI element runtime (see element.h). Semantics from the XDK 21256 XUI runtime as linked into TU46 (decompiled:
// XuiElement::UpdateXForm, XUISubtimeline_GotoTime, _ComputeEaseCurve, FillProperty::UpdateXFormV1/GetFillXForm,
// XUIDC::FillGradientTexture, XuiNineGridElement::BuildVerts, XUIFONT::DrawText/GetLine/CalculateLineSpacing/GetGlyph).

#include "xui/element.h"

#include "xui/assets.h"
#include "xui/host.h"
#include "log.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdlib>
#include <mutex>
#include <numbers>
#include <optional>
#include <set>

namespace xui
{
    namespace
    {
        std::atomic<host::SoundPlayer> g_soundPlayer = nullptr;

        bool IsControlClass(const std::string& className)
        {
            for (const ClassDef* c : ClassChain(className))
                if (std::string_view(c->name) == "XuiControl")
                    return true;
            return false;
        }

        std::u16string Utf16(const std::string& s)
        {
            std::u16string out;
            for (size_t i = 0; i < s.size();)
            {
                uint32_t c = uint8_t(s[i]);
                int extra = c >= 0xF0 ? 3 : c >= 0xE0 ? 2 : c >= 0xC0 ? 1 : 0;
                c &= extra == 3 ? 0x07 : extra == 2 ? 0x0F : extra == 1 ? 0x1F : 0x7F;
                i++;
                for (int k = 0; k < extra && i < s.size(); k++, i++)
                    c = c << 6 | (uint8_t(s[i]) & 0x3F);
                if (c >= 0x10000)
                {
                    c -= 0x10000;
                    out += char16_t(0xD800 + (c >> 10));
                    out += char16_t(0xDC00 + (c & 0x3FF));
                }
                else
                    out += char16_t(c);
            }
            return out;
        }

        uint32_t MulColor(uint32_t a, uint32_t b)
        {
            uint32_t out = 0;
            for (int shift = 0; shift < 32; shift += 8)
                out |= ((((a >> shift) & 0xFF) * ((b >> shift) & 0xFF) + 127) / 255) << shift;
            return out;
        }

        // Linear interpolation of two key values (InterpolateValues): numbers and vectors per component, colours per
        // channel; other types do not interpolate.
        Value Interpolate(const Value& a, const Value& b, float t)
        {
            Value v = a;
            switch (a.type)
            {
            case Type::kFloat: v.f = a.f + (b.f - a.f) * t; break;
            case Type::kInteger:
            case Type::kUnsigned: v.i = int64_t(std::lround(double(a.i) + double(b.i - a.i) * t)); break;
            case Type::kVector:
                v.v = { a.v.x + (b.v.x - a.v.x) * t, a.v.y + (b.v.y - a.v.y) * t, a.v.z + (b.v.z - a.v.z) * t };
                break;
            case Type::kColor:
            {
                uint32_t out = 0;
                for (int shift = 0; shift < 32; shift += 8)
                {
                    const float ca = float((a.color >> shift) & 0xFF), cb = float((b.color >> shift) & 0xFF);
                    out |= uint32_t(std::clamp(int(ca + (cb - ca) * t + 0.5f), 0, 255)) << shift;
                }
                v.color = out;
                break;
            }
            case Type::kQuaternion:
            {
                // Spherical interpolation along the shorter arc (XMQuaternionSlerp): constant angular speed, and keys
                // of opposite sign take the short way. The skin's Loading_Small turns 90 degrees a key and two of its
                // key pairs have opposite signs (270 -> 360 as (0,0,-.707,.707) -> (0,0,0,-1)); a plain linear blend
                // turned those quarters the long way round, backwards (owner, 2026-10-21).
                Quat bq = b.q;
                float cosOmega = a.q.x * bq.x + a.q.y * bq.y + a.q.z * bq.z + a.q.w * bq.w;
                if (cosOmega < 0)
                {
                    cosOmega = -cosOmega;
                    bq = { -bq.x, -bq.y, -bq.z, -bq.w };
                }
                float wa = 1.0f - t, wb = t;
                if (cosOmega < 0.9999f)
                {
                    const float omega = std::acos(std::min(cosOmega, 1.0f));
                    const float sinOmega = std::sin(omega);
                    wa = std::sin((1.0f - t) * omega) / sinOmega;
                    wb = std::sin(t * omega) / sinOmega;
                }
                Quat q{ a.q.x * wa + bq.x * wb, a.q.y * wa + bq.y * wb, a.q.z * wa + bq.z * wb, a.q.w * wa + bq.w * wb };
                const float n = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
                if (n > 0)
                    q = { q.x / n, q.y / n, q.z / n, q.w / n };
                v.q = q;
                break;
            }
            default: break;
            }
            return v;
        }

        // A mutable copy of a property object with one value replaced along a path ("Fill.Gradient.StopPos").
        std::shared_ptr<const Node> WithValue(const std::shared_ptr<const Node>& object, const PropTable& table,
                                              const std::vector<std::string_view>& path, size_t depth, bool indexed,
                                              uint32_t element, const Value& value)
        {
            auto copy = std::make_shared<Node>(object ? *object : Node{});
            const std::string_view name = path[depth];
            const PropDef* def = nullptr;
            for (uint32_t i = 0; i < table.count; i++)
                if (name == table.defs[i].name)
                    def = &table.defs[i];
            if (!def)
                return copy;
            auto it = std::find_if(copy->props.begin(), copy->props.end(),
                                   [&](const Property& p) { return p.def == def; });
            if (it == copy->props.end())
            {
                copy->props.push_back(Property{ def, {} });
                it = copy->props.end() - 1;
            }
            if (depth + 1 < path.size())
            {
                if (it->values.empty())
                    it->values.push_back(Value{ Type::kObject });
                it->values[0].object = WithValue(it->values[0].object, *def->sub, path, depth + 1, indexed, element, value);
            }
            else if (indexed)
            {
                if (it->values.size() <= element)
                    it->values.resize(element + 1, Value{ def->type });
                it->values[element] = value;
            }
            else
            {
                it->values.assign(1, value);
            }
            return copy;
        }

        const PropTable* ObjectTable(std::string_view className, std::string_view property)
        {
            for (const ClassDef* c : ClassChain(className))
                for (uint32_t i = 0; i < c->props.count; i++)
                    if (property == c->props.defs[i].name && c->props.defs[i].sub)
                        return c->props.defs[i].sub;
            return nullptr;
        }
    }

    void EaseControls(int easeIn, int easeOut, int easeScale, float& p1, float& p2)
    {
        // Control points: (s, 0) and (-s, 0) rotated by (1 - e/100) * 45 degrees, the second offset by (1, 1); the
        // curve uses their heights.
        const float s = float(easeScale) * 0.01f;
        const float aIn = (1.0f - float(easeIn) * 0.01f) * std::numbers::pi_v<float> / 4;
        const float aOut = (float(easeOut) * 0.01f - 1.0f) * -std::numbers::pi_v<float> / 4;
        p1 = s * std::sin(aIn);
        p2 = 1.0f - s * std::sin(aOut);
    }

    namespace
    {
        // XAM FUN_817a9fa0: the curve rising from 0 to 1 over t in [0, 1] (float and double steps as XAM's code).
        float Curve(Interpolation type, float p1, float p2, float t)
        {
            constexpr double kE = 2.7182817459106445; // XAM's constant (float e widened)
            constexpr float kHalfPi = 1.5707963705062866f;
            switch (type)
            {
            case Interpolation::kLinear: return t;
            case Interpolation::kEase: // the y of the cubic Bezier (0,0) (., p1) (., p2) (1,1) at parameter t
            {
                const float u = 1.0f - t;
                return ((u * p1 + p2 * t) * u * t) * 3.0f + t * t * t;
            }
            case Interpolation::kQuadratic: return t * t;
            case Interpolation::kCubic: return t * t * t;
            case Interpolation::kQuartic: return t * t * t * t;
            case Interpolation::kQuintic: return t * t * t * t * t;
            case Interpolation::kPower: return float(std::pow(double(t), double(p1)));
            case Interpolation::kSine: return 1.0f - float(std::sin(double((1.0f - t) * kHalfPi)));
            case Interpolation::kCircular: return (std::sqrt(1.0f - t * t) - 1.0f) * -1.0f;
            case Interpolation::kExponential:
                return float((double(float(std::pow(kE, double(p1 * t)))) - 1.0) /
                             (double(float(std::pow(kE, double(p1)))) - 1.0));
            case Interpolation::kElastic:
            {
                float amplitude = 0.0f; // XAM leaves it 0 for an exponent near 0
                if (p1 < -0.0001f || p1 > 0.0001f)
                    amplitude = float((double(float(std::pow(kE, double(p1 * t)))) - 1.0) /
                                      (double(float(std::pow(kE, double(p1)))) - 1.0));
                const double w = double(p2) * 6.2831854820251465 + 1.5707963705062866;
                return float(std::sin(w * double(t)) * double(amplitude));
            }
            case Interpolation::kBounce:
            {
                // Parabolic arcs; arc i (0..n) starts where 1 - x(1 - b) = b^i and is (1/b)^(n-i) high, the last one
                // peaking at t = 1.
                const float n = p1 >= 0.0f ? p1 : 0.0f;
                const float b = p2 > 1.0001f ? p2 : 1.01f;
                const float bn = float(std::pow(double(b), double(n)));
                const float oneMinusB = 1.0f - b;
                const float total = bn * 0.5f + (1.0f - bn) / oneMinusB;
                const float logB = float(std::log(double(b)));
                const double x = 1.0 - double(total * t) * (1.0 - double(b));
                const float i = float(std::floor(double(float(std::log(x) / double(logB)))));
                const float w = total * oneMinusB;
                const float start = float((1.0 - double(float(std::pow(double(b), double(i))))) / double(w));
                const float next = float((1.0 - double(float(std::pow(double(b), double(i + 1.0f))))) / double(w));
                const float mid = (next + start) * 0.5f;
                const float u = t - mid;
                const float half = mid - start;
                const float height = float(std::pow(1.0 / double(b), double(n - i)));
                return height * (-1.0f / (half * half)) * (u - half) * (half + u);
            }
            default: return 0.0f;
            }
        }
    }

    float CurveFraction(const KeyFrame& k, float t)
    {
        // XAM FUN_817aa2c0: hold jumps at the end; otherwise the curve as it is (in), mirrored (out) or both halves.
        t = std::clamp(t, 0.0f, 1.0f);
        if (k.interpolation == Interpolation::kNone)
            return t >= 0.9999f ? 1.0f : 0.0f;
        switch (k.direction)
        {
        case EaseDirection::kIn: return Curve(k.interpolation, k.p1, k.p2, t);
        case EaseDirection::kOut: return 1.0f - Curve(k.interpolation, k.p1, k.p2, 1.0f - t);
        case EaseDirection::kInOut:
            if (t >= 0.5f)
                return -(Curve(k.interpolation, k.p1, k.p2, -(t * 2.0f - 2.0f)) * 0.5f - 1.0f);
            return Curve(k.interpolation, k.p1, k.p2, t * 2.0f) * 0.5f;
        default: return 0.0f;
        }
    }

    float EaseFraction(int easeIn, int easeOut, int easeScale, float t)
    {
        KeyFrame k;
        k.interpolation = Interpolation::kEase;
        EaseControls(easeIn, easeOut, easeScale, k.p1, k.p2);
        return CurveFraction(k, t);
    }

    std::unique_ptr<Element> Element::Create(Assets& assets, const Scene& scene, const Node& node,
                                             const std::string& basePackage, Element* parent)
    {
        std::unique_ptr<Element> e(new Element());
        e->Build(assets, scene, node, basePackage, parent);
        return e;
    }

    std::unique_ptr<Element> Element::CreateControl(Assets& assets, std::string_view className, std::string_view visual,
                                                    float width, float height)
    {
        std::unique_ptr<Element> e(new Element());
        e->className_ = className;
        e->basePackage_ = Assets::kSkinBase;
        e->imageBase = Assets::kSkinBase;
        e->SetFloat("Width", width);
        e->SetFloat("Height", height);
        if (visual.empty()) // an element drawn by its class (XuiImage)
            return e;
        e->SetString("Visual", std::string(visual));
        const Node* v = nullptr;
        const Scene* skin = nullptr;
        std::string skinBase;
        if (assets.FindVisual(visual, v, skin, skinBase))
        {
            e->visual_.reset(new Element());
            e->visual_->owner_ = e.get();
            e->visual_->Build(assets, *skin, *v, skinBase, nullptr);
            if (width <= 0 || height <= 0) // the visual's own size
            {
                e->SetFloat("Width", e->visual_->DesignWidth());
                e->SetFloat("Height", e->visual_->DesignHeight());
            }
            else
                e->visual_->SetSize(width, height);
        }
        else
            Log("System UI: skin visual {} is missing", visual);
        return e;
    }

    Element* Element::Adopt(std::unique_ptr<Element> child)
    {
        child->parent_ = this;
        children_.push_back(std::move(child));
        return children_.back().get();
    }

    std::unique_ptr<Element> Element::Release(Element* child)
    {
        for (auto it = children_.begin(); it != children_.end(); ++it)
            if (it->get() == child)
            {
                std::unique_ptr<Element> out = std::move(*it);
                children_.erase(it);
                out->parent_ = nullptr;
                return out;
            }
        return nullptr;
    }

    void Element::Build(Assets& assets, const Scene& scene, const Node& node, const std::string& basePackage,
                        Element* parent)
    {
        className_ = node.className;
        id_ = node.Id();
        basePackage_ = basePackage;
        assets_ = &assets;
        scene_ = &scene;
        node_ = &node;
        parent_ = parent;
        if (parent)
            owner_ = parent->owner_; // a visual's elements present data for the visual's control
        for (const Property& p : node.props)
            if (!p.values.empty())
                props_[p.def->name] = p.values.front();
        designWidth_ = Width();
        designHeight_ = Height();
        // Indexed properties (gradient stops) live inside property objects; element properties are never indexed.
        for (const Node& child : node.children)
            children_.push_back(Create(assets, scene, child, basePackage, this));
        if (IsControlClass(className_))
        {
            text = Utf16(GetString("Text"));
            imagePath = GetString("ImagePath");
            imageBase = basePackage;
            std::string visualName = GetString("Visual");
            if (visualName.empty())
                visualName = className_;
            const Node* v = nullptr;
            const Scene* skin = nullptr;
            std::string skinBase;
            if (assets.FindVisual(visualName, v, skin, skinBase))
            {
                visual_.reset(new Element());
                visual_->owner_ = this;
                visual_->Build(assets, *skin, *v, skinBase, nullptr);
                visual_->SetSize(Width(), Height());
            }
        }
        // The first frame of every timeline applies at creation (XUI samples frame 0). Sounds stay silent while a tree
        // loads (XUI flags the object being created, 0x921E6828, and _SetFile does not play then).
        if (!node.timelines.empty())
            Sample(false);
    }

    Element* Element::Control()
    {
        return owner_;
    }

    Element* Element::Find(std::string_view id)
    {
        if (id_ == id)
            return this;
        for (auto& c : children_)
            if (Element* f = c->Find(id))
                return f;
        return nullptr;
    }

    Element* Element::FindInVisual(std::string_view id)
    {
        return visual_ ? visual_->Find(id) : nullptr;
    }

    const Value* Element::Get(std::string_view name) const
    {
        const auto it = props_.find(name);
        return it == props_.end() ? nullptr : &it->second;
    }

    void Element::Set(std::string_view name, const Value& value)
    {
        props_[std::string(name)] = value;
    }

    float Element::GetFloat(std::string_view name, float fallback) const
    {
        const Value* v = Get(name);
        return v && v->type == Type::kFloat ? v->f : fallback;
    }

    int64_t Element::GetInt(std::string_view name, int64_t fallback) const
    {
        const Value* v = Get(name);
        return v && (v->type == Type::kInteger || v->type == Type::kUnsigned) ? v->i : fallback;
    }

    bool Element::GetBool(std::string_view name, bool fallback) const
    {
        const Value* v = Get(name);
        return v && v->type == Type::kBool ? v->b : fallback;
    }

    uint32_t Element::GetColor(std::string_view name, uint32_t fallback) const
    {
        const Value* v = Get(name);
        return v && v->type == Type::kColor ? v->color : fallback;
    }

    std::string Element::GetString(std::string_view name) const
    {
        const Value* v = Get(name);
        return v && v->type == Type::kString ? v->s : std::string();
    }

    Vec3 Element::GetVector(std::string_view name, Vec3 fallback) const
    {
        const Value* v = Get(name);
        return v && v->type == Type::kVector ? v->v : fallback;
    }

    void Element::SetFloat(std::string_view name, float v)
    {
        Value value{ Type::kFloat };
        value.f = v;
        Set(name, value);
    }

    void Element::SetBool(std::string_view name, bool v)
    {
        Value value{ Type::kBool };
        value.b = v;
        Set(name, value);
    }

    void Element::SetVector(std::string_view name, Vec3 v)
    {
        Value value{ Type::kVector };
        value.v = v;
        Set(name, value);
    }

    void Element::SetString(std::string_view name, std::string v)
    {
        Value value{ Type::kString };
        value.s = std::move(v);
        Set(name, value);
    }

    void Element::SetSize(float width, float height)
    {
        const float oldWidth = Width(), oldHeight = Height();
        SetFloat("Width", width);
        SetFloat("Height", height);
        if (oldWidth != width || oldHeight != height)
            ApplyAnchors(oldWidth, oldHeight, width, height);
        if (visual_)
            visual_->SetSize(width, height);
    }

    void Element::ApplyAnchors(float oldWidth, float oldHeight, float newWidth, float newHeight)
    {
        const float dw = newWidth - oldWidth, dh = newHeight - oldHeight;
        for (auto& c : children_)
        {
            const uint32_t a = uint32_t(c->GetInt("Anchor", 0));
            Vec3 p = c->GetVector("Position", {});
            float w = c->Width(), h = c->Height();
            if ((a & 1) && (a & 4))
                w += dw;
            else if (a & 4)
                p.x += dw;
            else if (a & 0x10)
                p.x += dw / 2;
            else if ((a & 0x40) && oldWidth != 0)
            {
                p.x *= newWidth / oldWidth;
                w *= newWidth / oldWidth;
            }
            if ((a & 2) && (a & 8))
                h += dh;
            else if (a & 8)
                p.y += dh;
            else if (a & 0x20)
                p.y += dh / 2;
            else if ((a & 0x80) && oldHeight != 0)
            {
                p.y *= newHeight / oldHeight;
                h *= newHeight / oldHeight;
            }
            c->SetVector("Position", p);
            c->SetSize(std::max(0.0f, w), std::max(0.0f, h));
        }
    }

    // ---- Visual states ----

    void Element::PlayVisualState(std::string_view state)
    {
        if (!visual_)
            return;
        // XUI's visual state table (XuiControlPlayOptionalVisual, TU46 0x820AED84): each state's range and the state
        // played instead when the visual lacks it. KillFocus without its own range plays KillFocus..EndNormal, then
        // Normal.
        struct VisualState { std::string_view from, to; int fallback; };
        static constexpr VisualState kStates[] = {
            { "Normal", "EndNormal", -1 }, { "Focus", "EndFocus", 0 }, { "KillFocus", "EndNormal", 0 },
            { "NormalDisable", "EndNormalDisable", 0 }, { "FocusDisable", "EndFocusDisable", 1 },
            { "Press", "EndPress", 1 }, { "NormalPress", "EndNormalPress", 5 }, { "NormalCheck", "EndNormalCheck", 0 },
            { "FocusCheck", "EndFocusCheck", 7 }, { "NormalCheckDisable", "EndNormalCheckDisable", 7 },
            { "FocusCheckDisable", "EndFocusCheckDisable", 8 }, { "NormalSel", "EndNormalSel", 0 },
            { "NormalSelCheck", "EndNormalSelCheck", 11 }, { "NormalSelDisable", "EndNormalSelDisable", 11 },
            { "NormalSelCheckDisable", "EndNormalSelCheckDisable", 13 }, { "ScrollMore", "EndScrollMore", 0 },
            { "Scrolling", "EndScrolling", 15 }, { "ScrollMoreDisable", "EndScrollMoreDisable", 15 },
            { "InitFocus", "EndInitFocus", 1 }, { "InitFocusDisable", "EndInitFocusDisable", 4 },
            { "InitFocusCheck", "EndInitFocusCheck", 8 }, { "InitFocusCheckDisable", "EndInitFocusCheckDisable", 10 },
            { "PressDisable", "EndPressDisable", 6 }, { "KillFocus", "EndKillFocus", 2 } };
        // A checked item's states (KillFocus has no Check form: it ends on NormalCheck).
        static constexpr std::pair<std::string_view, std::string_view> kChecked[] = {
            { "Normal", "NormalCheck" }, { "Focus", "FocusCheck" }, { "KillFocus", "NormalCheck" },
            { "NormalDisable", "NormalCheckDisable" }, { "FocusDisable", "FocusCheckDisable" },
            { "InitFocus", "InitFocusCheck" }, { "InitFocusDisable", "InitFocusCheckDisable" } };
        if (checked)
            for (const auto& [plain, check] : kChecked)
                if (state == plain)
                {
                    state = check;
                    break;
                }
        int index = -1;
        for (int i = 0; i < int(std::size(kStates)); i++)
            if (kStates[i].from == state && kStates[i].to.substr(3) == state)
                index = i;
        for (int guard = 0; index >= 0 && guard < 8; guard++)
        {
            const VisualState& v = kStates[index];
            if (visual_->FindNamedFrame(v.from) >= 0 && visual_->FindNamedFrame(v.to) >= 0)
            {
                visual_->PlayNamed(v.from, v.to, true);
                return;
            }
            index = v.fallback;
        }
    }

    // ---- Timelines ----

    int Element::FindNamedFrame(std::string_view name) const
    {
        if (!node_)
            return -1;
        for (const NamedFrame& f : node_->namedFrames)
            if (f.name == name)
                return int(f.frame);
        return -1;
    }

    bool Element::PlayNamed(std::string_view from, std::string_view to, bool recurse)
    {
        const int start = FindNamedFrame(from);
        const int end = to.empty() ? -1 : FindNamedFrame(to);
        bool any = false;
        if (start >= 0)
        {
            frame_ = float(start);
            stopFrame_ = end;
            playing_ = true;
            // A state frame (a named frame with a stop command, gamercard.xur's Online / Offline) played on its own
            // is shown and stays: its stop holds the timeline there.
            if (from == to)
                for (const NamedFrame& f : node_->namedFrames)
                    if (f.name == from && f.command == Command::kStop)
                        playing_ = false;
            Sample();
            any = true;
        }
        // XuiElement::PlayTimeline recurses into children that are not controls (controls play their own visuals).
        if (recurse && !GetBool("DisableTimelineRecursion", false))
            for (auto& c : children_)
                if (!IsControlClass(c->className_))
                    any |= c->PlayNamed(from, to, true);
        return any;
    }

    void Element::ShowFrame(int frame)
    {
        frame_ = float(frame);
        stopFrame_ = -1;
        playing_ = false;
        Sample();
    }

    int Element::LastKeyFrame() const
    {
        int last = 0;
        if (node_)
            for (const Timeline& t : node_->timelines)
                for (const KeyFrame& k : t.keys)
                    last = std::max(last, int(k.frame));
        return last;
    }

    void Element::SetProgressValue(int value)
    {
        const int lo = int(GetInt("RangeMin", 0)), hi = int(GetInt("RangeMax", 100));
        value = std::clamp(value, std::min(lo, hi), std::max(lo, hi));
        Value v{ Type::kInteger };
        v.i = value;
        Set("Value", v);
        Element* body = visual_ ? visual_->Find("ProgressBody") : nullptr;
        if (!body || hi == lo)
            return;
        body->ShowFrame(int(int64_t(value - lo) * body->LastKeyFrame() / (hi - lo)));
    }

    bool Element::Playing() const
    {
        if (playing_)
            return true;
        for (const auto& c : children_)
            if (c->Playing())
                return true;
        return visual_ && visual_->Playing();
    }

    void Element::Advance(float seconds)
    {
        if (playing_ && node_)
        {
            float next = frame_ + seconds * 60.0f;
            // Commands of the named frames passed (in order); the end of the requested range stops.
            for (int guard = 0; guard < 8 && playing_; guard++)
            {
                const NamedFrame* hit = nullptr;
                for (const NamedFrame& f : node_->namedFrames)
                    if (float(f.frame) > frame_ && float(f.frame) <= next && (!hit || f.frame < hit->frame))
                        if (f.command != Command::kPlay || int(f.frame) == stopFrame_)
                            hit = &f;
                if (!hit)
                    break;
                if (hit->command == Command::kStop || (hit->command == Command::kPlay && int(hit->frame) == stopFrame_))
                {
                    next = float(hit->frame);
                    playing_ = false;
                    break;
                }
                const int target = FindNamedFrame(hit->target);
                if (target < 0)
                {
                    next = float(hit->frame);
                    playing_ = false;
                    break;
                }
                const float overshoot = next - float(hit->frame);
                frame_ = float(target);
                next = frame_ + overshoot;
                if (hit->command == Command::kGotoAndStop || hit->command == Command::kGoto)
                {
                    next = frame_;
                    playing_ = hit->command == Command::kGoto ? playing_ : false;
                    if (hit->command == Command::kGotoAndStop)
                        break;
                }
            }
            if (stopFrame_ >= 0 && playing_ && next >= float(stopFrame_) && frame_ <= float(stopFrame_))
            {
                next = float(stopFrame_);
                playing_ = false;
            }
            frame_ = next;
            Sample();
        }
        for (auto& c : children_)
            c->Advance(seconds);
        if (visual_)
            visual_->Advance(seconds);
    }

    void Element::Sample(bool sounds)
    {
        if (!node_)
            return;
        std::vector<Element*> dirty; // sounds whose File changed to a path
        for (const Timeline& t : node_->timelines)
        {
            if (t.keys.empty())
                continue;
            Element* target = nullptr;
            for (auto& c : children_)
                if (c->id_ == t.target)
                {
                    target = c.get();
                    break;
                }
            if (!target)
                continue;
            size_t k = 0;
            while (k + 1 < t.keys.size() && float(t.keys[k + 1].frame) <= frame_)
                k++;
            const KeyFrame& a = t.keys[k];
            const bool between = k + 1 < t.keys.size() && float(a.frame) <= frame_;
            for (size_t i = 0; i < t.tracks.size(); i++)
            {
                const Track& track = t.tracks[i];
                if (!track.def || i >= a.values.size())
                    continue;
                if (track.name == "File")
                {
                    // A string holds its key's value (no interpolation). The sound plays after the key's other
                    // tracks (its Volume) are applied, as XUI's timer plays it later.
                    if (SetSoundFile(*target, a.values[i]) && sounds)
                        dirty.push_back(target);
                    continue;
                }
                Value v = a.values[i];
                if (between && a.interpolation != Interpolation::kNone)
                {
                    const KeyFrame& b = t.keys[k + 1];
                    float f = std::clamp((frame_ - float(a.frame)) / float(b.frame - a.frame), 0.0f, 1.0f);
                    v = Interpolate(a.values[i], b.values[i], CurveFraction(a, f));
                }
                ApplyTrack(*target, track, v);
            }
        }
        for (Element* e : dirty)
            e->StartSound();
    }

    void host::SetSoundPlayer(SoundPlayer player)
    {
        g_soundPlayer = player;
    }

    // XuiSoundXAudioElement::_SetFile (dash 0x921DBB40): a File different from the current one is stored; a non-empty
    // one marks the sound dirty, and its timer plays it (XuiSoundElement 0x92204B48, State's default 2 = play).
    bool Element::SetSoundFile(Element& target, const Value& value)
    {
        if (value.type != Type::kString)
            return false;
        const Value* current = target.Get("File");
        if (current && current->type == Type::kString && current->s == value.s)
            return false;
        target.Set("File", value);
        return !value.s.empty();
    }

    void Element::StartSound()
    {
        const host::SoundPlayer player = g_soundPlayer.load();
        if (!player || !assets_)
            return;
        const std::string file = GetString("File");
        const Package::Entry* e = assets_->FindEntry(file, basePackage_);
        if (!e)
        {
            static std::mutex mutex;
            static std::set<std::string> reported;
            std::lock_guard lock(mutex);
            if (reported.insert(basePackage_ + "|" + file).second)
                Log("System UI: sound {} (base {}) not found", file, basePackage_);
            return;
        }
        player({ e->data, e->size, file, GetFloat("Volume", 0.0f), this });
    }

    void Element::ApplyTrack(Element& target, const Track& track, const Value& value)
    {
        const size_t dot = track.name.find('.');
        if (dot == std::string::npos)
        {
            if (track.name == "Width" || track.name == "Height")
            {
                // Resizing re-anchors the children, as XuiElementSetBounds does.
                const float w = track.name == "Width" ? value.f : target.Width();
                const float h = track.name == "Height" ? value.f : target.Height();
                target.SetSize(w, h);
                return;
            }
            target.Set(track.name, value);
            return;
        }
        // A value inside a property object (Fill.FillColor, Fill.Gradient.StopPos[1]).
        std::vector<std::string_view> path;
        std::string_view rest = track.name;
        while (!rest.empty())
        {
            const size_t p = rest.find('.');
            path.push_back(rest.substr(0, p));
            rest = p == std::string_view::npos ? std::string_view() : rest.substr(p + 1);
        }
        const PropTable* table = ObjectTable(target.className_, path[0]);
        if (!table)
            return;
        Value object = target.Get(path[0]) ? *target.Get(path[0]) : Value{ Type::kObject };
        object.type = Type::kObject;
        object.object = WithValue(object.object, *table, path, 1, track.hasIndex, track.element, value);
        target.Set(path[0], object);
    }

    // ---- Rendering ----

    Affine Element::LocalTransform() const
    {
        const Vec3 position = GetVector("Position", {});
        const Vec3 scale = GetVector("Scale", { 1, 1, 1 });
        Vec3 pivot = GetVector("Pivot", {});
        if (GetBool("CenterPivot", false))
            pivot = { Width() / 2, Height() / 2, 0 };
        const Value* r = Get("Rotation");
        Affine rotation;
        if (r && r->type == Type::kQuaternion)
        {
            // Rotation matrix of the quaternion, the 2D part (row-vector convention like D3DX).
            const Quat q = r->q;
            rotation.a = 1 - 2 * (q.y * q.y + q.z * q.z);
            rotation.b = 2 * (q.x * q.y + q.z * q.w);
            rotation.c = 2 * (q.x * q.y - q.z * q.w);
            rotation.d = 1 - 2 * (q.x * q.x + q.z * q.z);
        }
        return Affine::Translate(-pivot.x, -pivot.y)
            .Then(Affine::Scale(scale.x, scale.y))
            .Then(rotation)
            .Then(Affine::Translate(pivot.x, pivot.y))
            .Then(Affine::Translate(position.x, position.y));
    }

    void Element::Render(Assets& assets, Canvas& canvas, const Affine& toDevice, float opacity, const ClipRect& clip,
                         uint32_t colorFactor)
    {
        if (!Shown())
            return;
        const Affine world = LocalTransform().Then(toDevice);
        const float op = opacity * GetFloat("Opacity", 1.0f);
        if (op <= 0.0f)
            return;
        ClipRect inner = clip;
        if (GetBool("ClipChildren", false))
        {
            const Vec2 p0 = world.Apply({ 0, 0 }), p1 = world.Apply({ Width(), Height() });
            inner.x0 = std::max(clip.x0, int(std::ceil(std::min(p0.x, p1.x) - 0.5f)));
            inner.y0 = std::max(clip.y0, int(std::ceil(std::min(p0.y, p1.y) - 0.5f)));
            inner.x1 = std::min(clip.x1, int(std::ceil(std::max(p0.x, p1.x) - 0.5f)));
            inner.y1 = std::min(clip.y1, int(std::ceil(std::max(p0.y, p1.y) - 0.5f)));
        }
        renderFactor_ = MulColor(colorFactor, GetColor("ColorFactor", 0xFFFFFFFF));
        // BlendMode (XUI_BLEND_MODE): this element's and, unless they set their own, its children's draws.
        const int64_t blend = GetInt("BlendMode", 0);
        std::optional<ScopedBlend> scopedBlend;
        if (blend >= 1 && blend <= 6)
            scopedBlend.emplace(BlendMode(blend));
        // ColorWriteFlags (XUI's D3DCOLORWRITEENABLE: 1 red, 2 green, 4 blue, 8 alpha): an element that writes no
        // colour only shapes the target's alpha (the NXE dashboards' tile corner masks, CornerMask.png with 8, over
        // tiles drawn with 7). The overlay composites colour as it is drawn, so such an element is not drawn (its
        // rounded corners are lost); its children still are.
        if (GetInt("ColorWriteFlags", 0xF) & 7)
            RenderSelf(assets, canvas, world, op, inner);
        if (visual_)
            visual_->Render(assets, canvas, world, op, inner, renderFactor_);
        for (auto& c : children_)
            c->Render(assets, canvas, world, op, inner, renderFactor_);
    }

    void Element::RenderSelf(Assets& assets, Canvas& canvas, const Affine& world, float opacity, const ClipRect& clip)
    {
        if (className_ == "XuiFigure")
            RenderFigure(assets, canvas, world, opacity, clip);
        else if (className_ == "XuiImage")
        {
            // A picture set by code (XuiImageElementSetImage with a texture: a gamer picture) replaces ImagePath.
            const std::string path = GetString("ImagePath");
            if (image)
                RenderImage(assets, canvas, world, opacity, clip, image, uint32_t(GetInt("SizeMode", 0)));
            else if (!path.empty())
                RenderImage(assets, canvas, world, opacity, clip, assets.LoadImage(path, basePackage_),
                            uint32_t(GetInt("SizeMode", 0)));
        }
        else if (className_ == "XuiImagePresenter")
        {
            Element* control = Control();
            const Image* image = nullptr;
            const int64_t association = GetInt("DataAssociation", 0);
            if (control && association > 0 && association < kDataSlots && control->dataVisual[association])
            {
                Element* v = control->dataVisual[association].get();
                if (v->Width() != Width() || v->Height() != Height())
                    v->SetSize(Width(), Height());
                v->Render(assets, canvas, world, opacity, clip, renderFactor_);
                return;
            }
            if (control && association > 0 && association < kDataSlots)
                image = control->dataImage[association];
            else if (control)
                image = control->image ? control->image :
                    control->imagePath.empty() ? nullptr : assets.LoadImage(control->imagePath, control->imageBase);
            if (!image)
                image = this->image; // a scene's own presenter given a picture by code (828's achievementImage)
            if (image)
                RenderImage(assets, canvas, world, opacity, clip, image, uint32_t(GetInt("SizeMode", 0)));
        }
        else if (className_ == "XuiNineGrid")
            RenderNineGrid(assets, canvas, world, opacity, clip);
        else if (className_ == "XuiText")
            RenderText(assets, canvas, world, opacity, clip, Utf16(GetString("Text")));
        else if (className_ == "XuiTextPresenter")
        {
            if (Element* control = Control())
            {
                const int64_t association = GetInt("DataAssociation", 0);
                RenderText(assets, canvas, world, opacity, clip,
                           association > 0 && association < kDataSlots ? control->dataText[association] : control->text);
            }
        }
    }

    void Element::RenderFigure(Assets& assets, Canvas& canvas, const Affine& world, float opacity,
                               const ClipRect& clip)
    {
        const Value* points = Get("Points");
        const Value* fill = Get("Fill");
        if (!points || !fill || !fill->object)
            return;
        const FigurePath* path = scene_->Figure(points->custom);
        if (!path || path->points.size() < 2)
            return;
        const float w = Width(), h = Height();
        const float sx = path->width > 0 ? w / path->width : 1.0f, sy = path->height > 0 ? h / path->height : 1.0f;
        // Flatten the cubic segments (point, its two controls, the next point) in element space, then to the device.
        std::vector<Vec2> polygon;
        const size_t n = path->points.size();
        const bool closed = GetBool("Closed", true);
        for (size_t i = 0; i < (closed ? n : n - 1); i++)
        {
            const FigurePoint& p = path->points[i];
            const FigurePoint& q = path->points[(i + 1) % n];
            const Vec2 p0{ p.x * sx, p.y * sy }, c1{ p.c1x * sx, p.c1y * sy }, c2{ p.c2x * sx, p.c2y * sy },
                p3{ q.x * sx, q.y * sy };
            const float len = std::hypot(c1.x - p0.x, c1.y - p0.y) + std::hypot(c2.x - c1.x, c2.y - c1.y) +
                std::hypot(p3.x - c2.x, p3.y - c2.y);
            const bool straight = (c1.x == p0.x && c1.y == p0.y && c2.x == p3.x && c2.y == p3.y) ||
                std::abs((p3.x - p0.x) * (c1.y - p0.y) - (p3.y - p0.y) * (c1.x - p0.x)) < 1e-3f &&
                std::abs((p3.x - p0.x) * (c2.y - p0.y) - (p3.y - p0.y) * (c2.x - p0.x)) < 1e-3f;
            const int steps = straight ? 1 : std::clamp(int(len * 2), 4, 64);
            for (int s = 0; s < steps; s++)
            {
                const float t = float(s) / float(steps), u = 1 - t;
                polygon.push_back(world.Apply({ u * u * u * p0.x + 3 * u * u * t * c1.x + 3 * u * t * t * c2.x +
                                                    t * t * t * p3.x,
                                                u * u * u * p0.y + 3 * u * u * t * c1.y + 3 * u * t * t * c2.y +
                                                    t * t * t * p3.y }));
            }
        }
        if (!closed)
        {
            const FigurePoint& last = path->points[n - 1];
            polygon.push_back(world.Apply({ last.x * sx, last.y * sy }));
        }

        const Node& brush = *fill->object;
        auto prop = [&](std::string_view name) { return brush.Find(name); };
        const Value* fillType = prop("FillType");
        const int type = fillType ? int(fillType->i) : 1;
        if (type == 0)
            return;
        const uint32_t factor = renderFactor_;
        const Value* fillColorValue = prop("FillColor");
        const uint32_t fillColor = fillColorValue ? fillColorValue->color : 0xFF000000;
        auto finish = [&](uint32_t argb) {
            Rgba c = ArgbToRgba(MulColor(argb, factor));
            c.a *= opacity;
            return c;
        };
        if (type == 1)
        {
            const Rgba c = finish(fillColor);
            canvas.FillPolygon(polygon, [&](float, float) { return c; }, clip);
            return;
        }
        // Brush space (FillProperty::GetFillXForm). Transform version 1 (UpdateXFormV1): uv = ((p - centre +
        // T * size) R) S/g + 0.5, g = min(w, h) for linear gradients, max(w, h) for radial ones, the texture size for
        // textures. Version 0 scales before it rotates and translates in brush units: gradients
        // (UpdateXFormLinearV0) uv = ((p / size - 0.5) S R + 0.5) + T, textures (UpdateXFormV0) uv = ((p - centre) S R +
        // centre) / texture size + T. (Row vectors; R = XuiMatrixRotationZ(Rotation degrees).)
        const Value* tr = prop("Translation");
        const Value* sc = prop("Scale");
        const Value* rot = prop("Rotation");
        const Vec3 T = tr ? tr->v : Vec3{};
        const Vec3 S = sc ? sc->v : Vec3{ 1, 1, 1 };
        const float theta = (rot ? rot->f : 0.0f) * std::numbers::pi_v<float> / 180.0f;
        const Value* version = prop("TransformVersion");
        const bool version1 = version && version->i == 1;
        const Image* texture = nullptr;
        float gw, gh;
        if (type == 4)
        {
            const Value* file = prop("TextureFileName");
            texture = file ? assets.LoadImage(file->s, basePackage_) : nullptr;
            if (!texture)
                return;
            gw = float(texture->width);
            gh = float(texture->height);
        }
        else
            gw = gh = type == 2 ? std::min(w, h) : std::max(w, h);
        const float cosT = std::cos(theta), sinT = std::sin(theta);
        const Affine inverse = world.Inverse();
        std::vector<std::pair<float, uint32_t>> stops;
        if (type == 2 || type == 3)
        {
            const Value* gradient = prop("Gradient");
            if (gradient && gradient->object)
            {
                const Node& g = *gradient->object;
                const Value* count = g.Find("NumStops");
                std::vector<Value> colors, positions;
                for (const Property& p : g.props)
                {
                    if (std::string_view(p.def->name) == "StopColor")
                        colors = p.values;
                    if (std::string_view(p.def->name) == "StopPos")
                        positions = p.values;
                }
                const size_t n2 = std::min({ size_t(count ? count->i : 0), colors.size(), positions.size() });
                for (size_t i = 0; i < n2; i++)
                    stops.push_back({ positions[i].f, colors[i].color });
            }
            if (stops.empty())
                stops.push_back({ 0.0f, fillColor });
        }
        auto gradientColor = [&](float t) {
            // XUIDC::FillGradientTexture: clamped to the first and last stops, linear in between.
            if (t <= stops.front().first)
                return stops.front().second;
            for (size_t i = 0; i + 1 < stops.size(); i++)
                if (t < stops[i + 1].first)
                {
                    const float span = stops[i + 1].first - stops[i].first;
                    if (span <= 0)
                        return stops[i + 1].second;
                    Value a{ Type::kColor }, b{ Type::kColor };
                    a.color = stops[i].second;
                    b.color = stops[i + 1].second;
                    return Interpolate(a, b, (t - stops[i].first) / span).color;
                }
            return stops.back().second;
        };
        canvas.FillPolygon(polygon, [&](float x, float y) {
            const Vec2 p = inverse.Apply({ x, y });
            float u, v;
            if (version1)
            {
                const float qx = p.x - w / 2 + T.x * w, qy = p.y - h / 2 + T.y * h;
                const float rx = qx * cosT - qy * sinT, ry = qx * sinT + qy * cosT;
                u = rx * S.x / gw + 0.5f;
                v = ry * S.y / gh + 0.5f;
            }
            else
            {
                // Gradients work in units of the figure's size, textures in element units.
                const float bw = type == 4 ? 1.0f : w, bh = type == 4 ? 1.0f : h;
                const float qx = (p.x - w / 2) / bw * S.x, qy = (p.y - h / 2) / bh * S.y;
                const float rx = qx * cosT - qy * sinT, ry = qx * sinT + qy * cosT;
                u = type == 4 ? (rx + w / 2) / gw + T.x : rx + 0.5f + T.x;
                v = type == 4 ? (ry + h / 2) / gh + T.y : ry + 0.5f + T.y;
            }
            if (type == 2)
                return finish(gradientColor(u));
            if (type == 3)
                return finish(gradientColor(2.0f * std::hypot(u - 0.5f, v - 0.5f)));
            Rgba s = SampleBilinear(*texture, u * gw, v * gh, Wrap::kWrap, Wrap::kWrap);
            const Rgba f = ArgbToRgba(factor);
            return Rgba{ s.r * f.r, s.g * f.g, s.b * f.b, s.a * f.a * opacity };
        }, clip);
    }

    void Element::RenderImage(Assets&, Canvas& canvas, const Affine& world, float opacity, const ClipRect& clip,
                              const Image* image, uint32_t sizeMode)
    {
        if (!image || image->width == 0)
            return;
        const float w = Width(), h = Height(), iw = float(image->width), ih = float(image->height);
        // Image placement in the element (XUI_IMAGE_PRESENTER_MODE / XuiImage SizeMode).
        float x = 0, y = 0, dw = iw, dh = ih;
        if (sizeMode & 4)
        {
            dw = w;
            dh = h;
        }
        else if (sizeMode & (8 | 16))
        {
            const float s = std::min(w / iw, h / ih);
            dw = iw * s;
            dh = ih * s;
            if (sizeMode & 16)
            {
                x = (w - dw) / 2;
                y = (h - dh) / 2;
            }
        }
        else if (sizeMode & 2)
        {
            x = (w - iw) / 2;
            y = (h - ih) / 2;
        }
        Affine toDevice = Affine::Scale(dw / iw, dh / ih).Then(Affine::Translate(x, y)).Then(world);
        // On the HUD's unit grid (Canvas::SetUnitGrid) an axis-aligned image's edges fall on whole units: the first
        // unit column / row whose centre it covers, ceil(edge - 0.5), up to the same for its far edge.
        if (const Affine* grid = canvas.UnitGrid(); grid && toDevice.b == 0 && toDevice.c == 0)
        {
            const Affine toUnits = grid->Inverse();
            const Vec2 p0 = toUnits.Apply(toDevice.Apply({ 0, 0 })), p1 = toUnits.Apply(toDevice.Apply({ iw, ih }));
            const float x0 = std::ceil(p0.x - 0.5f), x1 = std::ceil(p1.x - 0.5f);
            const float y0 = std::ceil(p0.y - 0.5f), y1 = std::ceil(p1.y - 0.5f);
            if (x0 == x1 || y0 == y1)
                return; // no unit's centre inside: the console's target draws nothing
            toDevice = Affine::Scale((x1 - x0) / iw, (y1 - y0) / ih).Then(Affine::Translate(x0, y0)).Then(*grid);
        }
        Rgba tint = ArgbToRgba(renderFactor_);
        tint.a *= opacity;
        canvas.DrawImage(*image, toDevice, tint, clip);
    }

    void Element::RenderNineGrid(Assets& assets, Canvas& canvas, const Affine& world, float opacity,
                                 const ClipRect& clip)
    {
        const Image* image = assets.LoadImage(GetString("TextureFileName"), basePackage_);
        if (!image)
            return;
        // XuiNineGridElement::BuildVerts: columns 0, L, w-R, w over texture u 0, L, tw-R, tw (rows alike); the
        // corners keep their texel size.
        const float w = Width(), h = Height(), tw = float(image->width), th = float(image->height);
        const float L = float(GetInt("LeftOffset", 0)), T = float(GetInt("TopOffset", 0));
        const float R = float(GetInt("RightOffset", 0)), B = float(GetInt("BottomOffset", 0));
        const float xs[4] = { 0, L, w - R, w }, us[4] = { 0, L, tw - R, tw };
        const float ys[4] = { 0, T, h - B, h }, vs[4] = { 0, T, th - B, th };
        const bool noCenter = GetBool("NoCenter", false);
        Rgba tint = ArgbToRgba(renderFactor_);
        tint.a *= opacity;
        for (int j = 0; j < 3; j++)
            for (int i = 0; i < 3; i++)
            {
                if (noCenter && i == 1 && j == 1)
                    continue;
                const float cw = xs[i + 1] - xs[i], ch = ys[j + 1] - ys[j];
                const float uw = us[i + 1] - us[i], vh = vs[j + 1] - vs[j];
                if (cw <= 0 || ch <= 0 || uw <= 0 || vh <= 0)
                    continue;
                // Texel (u, v) of this slice -> element space -> device.
                const Affine slice = Affine::Translate(-us[i], -vs[j])
                    .Then(Affine::Scale(cw / uw, ch / vh))
                    .Then(Affine::Translate(xs[i], ys[j]))
                    .Then(world);
                canvas.DrawImagePart(*image, us[i], vs[j], us[i + 1], vs[j + 1], slice, tint, clip);
            }
    }

    int Element::MeasureText(Assets& assets, const std::u16string& text) const
    {
        Font& font = assets.DefaultFont();
        const float pixelsPerEm = GetFloat("PointSize", 14.0f) * 96.0f / 72.0f * GetFloat("TextScale", 1.0f);
        const float unit = pixelsPerEm / float(font.UnitsPerEm());
        int widest = 0;
        for (size_t at = 0; at <= text.size();)
        {
            size_t end = at;
            while (end < text.size() && text[end] != u'\r' && text[end] != u'\n')
                end++;
            size_t last = end;
            while (last > at && (text[last - 1] == u' ' || text[last - 1] == 0x3000))
                last--;
            int w = 0;
            for (size_t i = at; i < last; i++)
                w += int(std::ceil(float(font.Advance(font.GlyphIndex(text[i]))) * unit));
            widest = std::max(widest, w);
            if (end < text.size() && text[end] == u'\r')
                end++;
            at = end + 1;
        }
        return widest;
    }

    void Element::RenderText(Assets& assets, Canvas& canvas, const Affine& world, float opacity, const ClipRect& clip,
                             const std::u16string& text)
    {
        if (text.empty())
            return;
        Font& font = assets.DefaultFont();
        uint32_t style = uint32_t(GetInt("TextStyle", 0));
        // A multiline XuiEdit breaks its text into lines itself (XuiEditElement::GetTextBreaks) at the width of its
        // text area, whatever its presenter's wrapping style.
        if (Element* control = Control(); control && control->className_ == "XuiEdit" &&
            control->GetBool("Multiline", false))
            style &= ~0x30u;
        const float pointSize = GetFloat("PointSize", 14.0f);
        const int lineAdjust = int(GetInt("LineSpacingAdjust", 0));
        const uint32_t factor = renderFactor_;
        Rgba color = ArgbToRgba(MulColor(GetColor("TextColor", 0xFFFFFFFF), factor));
        color.a *= opacity;
        Rgba shadow = ArgbToRgba(MulColor(GetColor("DropShadowColor", 0xFF000000), factor));
        shadow.a *= opacity;

        // XUI lays text out in the element's own units (XUIFONT::DrawText works on an integer rectangle, with the
        // font at its point size in those units: points at 96 dpi), then the render transform maps the glyph quads to
        // the target, so advances and line positions are whole units of the scene (1.5 pixels at 720p).
        const int width = int(Width()), bottom = int(Height());
        const float pixelsPerEm = pointSize * 96.0f / 72.0f * GetFloat("TextScale", 1.0f);
        const float unit = pixelsPerEm / float(font.UnitsPerEm());
        const float ascent = float(font.Ascent()) * unit;
        const int height = int(std::ceil(float(font.Ascent() - font.Descent()) * unit));
        const int spacing = lineAdjust < -height ? 0 : height + lineAdjust;
        if (width <= 0)
            return;
        auto advance = [&](char16_t c) {
            return int(std::ceil(float(font.Advance(font.GlyphIndex(c))) * unit));
        };
        auto isSpace = [](char16_t c) { return c == u' ' || c == 0x3000; };

        // Lines (XUIFONT::GetLine): fill up to the width; break at the last space (or after a hyphen), else mid-word;
        // NO_WORDWRAP cuts the line (truncated); trailing spaces do not count.
        struct Line { size_t start, count; int width; bool truncated; };
        std::vector<Line> lines;
        const bool noWrap = style & 0x10, singleLine = style & 0x20;
        size_t at = 0;
        while (at < text.size())
        {
            size_t i = at;
            int w = 0;
            while (i < text.size() && text[i] != u'\r' && text[i] != u'\n')
            {
                const int a = advance(text[i]);
                if (w + a > width)
                    break;
                w += a;
                i++;
            }
            Line line{ at, i - at, w, false };
            size_t next = i;
            if (i < text.size() && text[i] != u'\r' && text[i] != u'\n')
            {
                if (noWrap)
                {
                    line.truncated = true;
                    while (next < text.size() && text[next] != u'\r' && text[next] != u'\n')
                        next++;
                }
                else if (!isSpace(text[i]))
                {
                    size_t b = i;
                    int bw = w;
                    while (b > at && !isSpace(text[b - 1]) && text[b - 1] != u'-')
                    {
                        b--;
                        bw -= advance(text[b]);
                    }
                    if (b > at)
                    {
                        line.count = b - at;
                        line.width = bw;
                        next = b;
                    }
                }
                while (next < text.size() && isSpace(text[next]))
                    next++;
            }
            if (next < text.size() && text[next] == u'\r')
                next++;
            if (next < text.size() && text[next] == u'\n')
                next++;
            while (line.count > 0 && isSpace(text[line.start + line.count - 1]))
            {
                line.width -= advance(text[line.start + line.count - 1]);
                line.count--;
            }
            lines.push_back(line);
            if (singleLine || next == at)
                break;
            at = next;
        }
        // Blank lines at the end take no room: a text ending in a line break and spaces (PartySettings.xur's "Party
        // Status\n    ") is centred as its one line (owner's capture "party settings party chat.png").
        while (lines.size() > 1 && lines.back().count == 0)
            lines.pop_back();

        // Glyphs are rasterised at their size on the target (the glyph cache holds them at the device size) and
        // drawn as bilinear-filtered quads at the pen's position mapped to the target, which may fall between
        // pixels; the cache keeps an empty texel around each glyph. (Checked against the owner's captures: the
        // layout in scene units with glyphs at the device size matches them to within 0.6 pixel.)
        const float scale = std::sqrt(std::abs(world.a * world.d - world.b * world.c));
        auto drawGlyph = [&](char16_t c, int penX, int lineTop, Rgba tint, float offset) {
            const GlyphBitmap& g = font.Rasterize(font.GlyphIndex(c), unit * scale, 0.0f, 0.0f);
            if (g.width == 0)
                return;
            Image image;
            image.width = uint32_t(g.width + 2);
            image.height = uint32_t(g.height + 2);
            image.rgba.assign(size_t(image.width) * image.height * 4, 0);
            for (int y = 0; y < g.height; y++)
                for (int x = 0; x < g.width; x++)
                {
                    uint8_t* t = &image.rgba[(size_t(y + 1) * image.width + x + 1) * 4];
                    t[0] = t[1] = t[2] = 255;
                    t[3] = g.coverage[size_t(y) * g.width + x];
                }
            // The glyph's pixels follow the element's rotation (the Guide's blade labels are turned 90 degrees): its
            // upright device-size bitmap goes through the world transform's rotation (the scale is in the raster).
            const Vec2 p = world.Apply({ float(penX) + offset, float(lineTop) + ascent + offset });
            const Affine turn{ world.a / scale, world.b / scale, world.c / scale, world.d / scale, p.x, p.y };
            canvas.DrawImage(image, Affine::Translate(float(g.left - 1), float(g.top - 1)).Then(turn), tint, clip);
        };

        int y = 0;
        if (style & 0x1000) // vertical centre
            y = (bottom - int(lines.size()) * spacing + std::max(-height, lineAdjust)) / 2;
        for (const Line& line : lines)
        {
            if (y > bottom)
                break;
            std::u16string chars = text.substr(line.start, line.count);
            int lineWidth = line.width;
            if (line.truncated && (style & 0x4000)) // ellipsis
            {
                const int dots = advance(0x2026);
                while (!chars.empty() && lineWidth > width - dots)
                {
                    lineWidth -= advance(chars.back());
                    chars.pop_back();
                }
                chars += char16_t(0x2026);
                lineWidth += dots;
            }
            int x = 0;
            if (style & 0x400)
                x = (width - lineWidth) / 2;
            else if (style & 0x200)
                x = width - lineWidth;
            for (char16_t c : chars)
            {
                if (style & 1)
                    drawGlyph(c, x, y, shadow, 1.0f);
                drawGlyph(c, x, y, color, 0.0f);
                x += advance(c);
            }
            y += spacing;
        }
    }
}
