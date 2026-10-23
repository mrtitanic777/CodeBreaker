#pragma once

// A live XUI element tree built from XUR scenes and skin visuals, with XUI's runtime semantics (the XDK 21256 XUI
// runtime that TU46 links was decompiled for these; NOTES.md "System UI"):
// - Transform: local = T(-pivot) S(scale) R(rotation) T(pivot) T(position); world = local x parent (row vectors).
// - Opacity multiplies down the tree; Show=false hides a subtree.
// - Anchors: when an element is resized from its authored size, children keep their distance to the edges they are
//   anchored to (left/right/top/bottom, centre, or stretch when anchored to both).
// - Controls (XuiButton, XuiEdit, XuiListItem, ...) draw through a visual: a copy of the skin's XuiVisual named by
//   the Visual property (or the class name), sized to the control; its presenters show the control's text and image.
// - Timelines: an element's timelines animate its children (by Id) at 60 frames per second. A key's interpolation
//   applies to the segment after it: linear, none (hold), ease (a cubic Bezier of the time fraction, control points
//   from the key's ease-in/out/scale) or one of XAM's curves (quadratic ... quintic, power, sine, circular,
//   exponential, elastic, bounce; in, out or in-out), see CurveFraction in xur.h. Named frames carry commands (stop,
//   goto, gotoandplay, gotoandstop).

#include "xui/canvas.h"
#include "xui/xur.h"

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace xui
{
    class Assets;

    class Element
    {
    public:
        // Builds the element tree of `node`; `basePackage` resolves its relative paths.
        static std::unique_ptr<Element> Create(Assets& assets, const Scene& scene, const Node& node,
                                               const std::string& basePackage, Element* parent);
        // A control created by code (as XAM creates its message boxes): `className` drawn through the skin visual
        // `visual`, sized w x h (no visual: an element drawn by its class, such as an XuiImage; 0 x 0: the visual's
        // own size).
        static std::unique_ptr<Element> CreateControl(Assets& assets, std::string_view className,
                                                      std::string_view visual, float width, float height);
        // Makes `child` the last child of this element / takes a child back out.
        Element* Adopt(std::unique_ptr<Element> child);
        std::unique_ptr<Element> Release(Element* child);

        // ---- Tree ----
        const std::string& ClassName() const { return className_; }
        const std::string& Id() const { return id_; }
        Element* Parent() const { return parent_; }
        const std::vector<std::unique_ptr<Element>>& Children() const { return children_; }
        Element* Find(std::string_view id); // depth-first, this element included; visuals not searched
        Element* FindInVisual(std::string_view id); // inside this control's visual
        Element* Visual() const { return visual_.get(); }

        // ---- Properties ----
        const Value* Get(std::string_view name) const;
        void Set(std::string_view name, const Value& value);
        float GetFloat(std::string_view name, float fallback) const;
        int64_t GetInt(std::string_view name, int64_t fallback) const;
        bool GetBool(std::string_view name, bool fallback) const;
        uint32_t GetColor(std::string_view name, uint32_t fallback) const;
        std::string GetString(std::string_view name) const;
        Vec3 GetVector(std::string_view name, Vec3 fallback) const;
        void SetFloat(std::string_view name, float v);
        void SetBool(std::string_view name, bool v);
        void SetVector(std::string_view name, Vec3 v);
        void SetString(std::string_view name, std::string v);

        // XuiElement's defaults (its constructor, TU46 0x82AA2C30): 60 x 30.
        float Width() const { return GetFloat("Width", 60); }
        float Height() const { return GetFloat("Height", 30); }
        bool Shown() const { return GetBool("Show", true); }
        // The size the scene authored (before anchors or code resized the element).
        float DesignWidth() const { return designWidth_; }
        float DesignHeight() const { return designHeight_; }
        // Resizes the element; anchored children (and the visual) follow.
        void SetSize(float width, float height);
        void SetPosition(float x, float y) { SetVector("Position", { x, y, 0 }); }

        // ---- Control data (what presenters show) ----
        std::u16string text;     // control text (DataAssociation 0)
        // DataAssociation 1-15 (list items' further lines, an edit's text, ...; the Guide's community rows use up to 12).
        static constexpr int kDataSlots = 16;
        std::u16string dataText[kDataSlots];
        std::string imagePath;   // image presenters (DataAssociation 0)
        std::string imageBase;   // package `imagePath` is relative to
        const Image* image = nullptr; // an image set by code (gamer pictures)
        const Image* dataImage[kDataSlots] = {};
        // A visual an image presenter shows in its slot instead of an image (XM_GET_SOURCE_IMAGE answering with an
        // XuiVisualCreateInstance handle: the Guide's recent players' Reputation stars), sized to the presenter.
        std::unique_ptr<Element> dataVisual[kDataSlots];
        bool focused = false, enabled = true;
        // A checked list item (XM_GET_ITEMCHECK: a read message) plays the Check form of each state (NormalCheck,
        // FocusCheck ...: MessageListItem's icons at half opacity).
        bool checked = false;
        // Plays the control's visual state (Normal, Focus, Press, ...), with XUI's fallbacks.
        void PlayVisualState(std::string_view state);

        // ---- Timelines ----
        // Plays this element's timeline from the named frame `from` until `to` (or until a stop command); with
        // `recurse`, children that have the same named frames play them too (as XuiElementPlayTimeline does).
        bool PlayNamed(std::string_view from, std::string_view to, bool recurse);
        void Advance(float seconds); // runs every playing timeline in the tree
        bool Playing() const;        // anything in the tree still animating
        bool TimelinePlaying() const { return playing_; } // this element's own timelines
        int FindNamedFrame(std::string_view name) const;
        // Shows frame `frame` of this element's timelines and stays there (XuiElementPlayTimeline(h, f, f, f)).
        void ShowFrame(int frame);
        // The last key frame of this element's own timelines (0 if none).
        int LastKeyFrame() const;
        // An XuiProgressBar's value (XuiProgressBarSetValue): clamped to RangeMin..RangeMax, shown as the dash's XUI
        // does (FUN_921F3090, SHOWS): its visual's ProgressBody shows frame (value - min) * frames / (max - min), where
        // frames is ProgressBody's timeline length (INFER: its last key frame).
        void SetProgressValue(int value);

        // The extent of `text` laid out by this text element / presenter without a width limit: the widest line, in
        // the element's units (what XAM measures to fit a control to its text).
        int MeasureText(Assets& assets, const std::u16string& text) const;

        // ---- Rendering ----
        // Draws the tree; `toDevice` maps this element's parent's space to device pixels. `colorFactor` is the
        // parents' ColorFactor: XUI multiplies an element's ColorFactor into its visual and children, as Opacity
        // (ProfileRoot.xur's tab labels: a dark factor on a white-text label visual, the owner's 7_47_09).
        void Render(Assets& assets, Canvas& canvas, const Affine& toDevice, float opacity, const ClipRect& clip,
                    uint32_t colorFactor = 0xFFFFFFFF);

    private:
        Element() = default;
        void Build(Assets& assets, const Scene& scene, const Node& node, const std::string& basePackage, Element* parent);
        void ApplyAnchors(float oldWidth, float oldHeight, float newWidth, float newHeight);
        void Sample(bool sounds = true); // applies the timelines at the current frame; `sounds`: File keys play
        // XuiSoundXAudio: stores a File key's value; true when it changed to a path (the sound is to play).
        static bool SetSoundFile(Element& target, const Value& value);
        void StartSound(); // hands this sound element's File to the runtime's player (host.h)
        void ApplyTrack(Element& target, const Track& track, const Value& value);
        Affine LocalTransform() const;
        Element* Control(); // the control a visual element presents data for

        void RenderSelf(Assets& assets, Canvas& canvas, const Affine& toDevice, float opacity, const ClipRect& clip);
        void RenderFigure(Assets& assets, Canvas& canvas, const Affine& toDevice, float opacity, const ClipRect& clip);
        void RenderImage(Assets& assets, Canvas& canvas, const Affine& toDevice, float opacity, const ClipRect& clip,
                         const Image* image, uint32_t sizeMode);
        void RenderNineGrid(Assets& assets, Canvas& canvas, const Affine& toDevice, float opacity, const ClipRect& clip);
        void RenderText(Assets& assets, Canvas& canvas, const Affine& toDevice, float opacity, const ClipRect& clip,
                        const std::u16string& text);

        std::string className_, id_, basePackage_;
        Assets* assets_ = nullptr; // set by Build (sounds resolve their File through it)
        const Scene* scene_ = nullptr;
        const Node* node_ = nullptr;
        Element* parent_ = nullptr;
        Element* owner_ = nullptr; // for a visual's elements: the control the visual belongs to
        std::vector<std::unique_ptr<Element>> children_;
        std::unique_ptr<Element> visual_;
        std::map<std::string, Value, std::less<>> props_;

        // Timeline playback of this element's own timelines.
        float designWidth_ = 60, designHeight_ = 30;
        float frame_ = 0;
        bool playing_ = false;
        int stopFrame_ = -1;
        bool sampled_ = false;
        uint32_t renderFactor_ = 0xFFFFFFFF; // this element's ColorFactor times its parents' (set by Render)
    };

    // XUI's ease curve (_ComputeEaseCurve + XUISubtimeline_GotoTime): the eased fraction for time fraction t (an
    // ease key's CurveFraction).
    float EaseFraction(int easeIn, int easeOut, int easeScale, float t);
}
