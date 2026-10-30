// XUR scene reader (see xur.h). The class property lists follow the XDK 21256 XUI runtime (the XML/binary pair of
// its default skin) and XAM 17559's registered classes (Nexia360's tables, BSD-3, were read for the latter; checked by
// reading every scene of the dashboard packages, CB_XUI_TEST=1).

#include "xui/xur.h"

#include <algorithm>
#include <bit>
#include <cstring>
#include <format>

namespace xui
{
    namespace
    {
        using T = Type;
        constexpr uint32_t I = kPropIndexed;

#define TABLE(name, ...)                                                    \
        const PropDef name##_defs[] = { __VA_ARGS__ };                     \
        const PropTable name = { name##_defs, uint32_t(std::size(name##_defs)) };

        TABLE(kElement,
            { "Id", T::kString }, { "Width", T::kFloat }, { "Height", T::kFloat }, { "Position", T::kVector },
            { "Scale", T::kVector }, { "Rotation", T::kQuaternion }, { "Opacity", T::kFloat }, { "Anchor", T::kUnsigned },
            { "Pivot", T::kVector }, { "Show", T::kBool }, { "BlendMode", T::kUnsigned },
            { "DisableTimelineRecursion", T::kBool }, { "DesignTime", T::kBool }, { "ColorWriteFlags", T::kUnsigned },
            { "ClipChildren", T::kBool }, { "EnableEffects", T::kBool }, { "DisableFocusRecursion", T::kBool },
            { "GripTarget", T::kBool }, { "Hittable", T::kBool }, { "LayoutLineBreak", T::kBool },
            { "LayoutFloat", T::kBool }, { "Column", T::kUnsigned }, { "Row", T::kUnsigned },
            { "ColumnSpan", T::kUnsigned }, { "RowSpan", T::kUnsigned }, { "ColorFactor", T::kColor },
            { "CenterPivot", T::kBool })
        TABLE(kControl,
            { "ClassOverride", T::kString }, { "Visual", T::kString }, { "Enabled", T::kBool },
            { "UnfocusedInput", T::kBool }, { "NavLeft", T::kString }, { "NavRight", T::kString },
            { "NavUp", T::kString }, { "NavDown", T::kString }, { "NavTabForward", T::kString },
            { "NavTabBackward", T::kString }, { "Text", T::kString }, { "PointSize", T::kFloat },
            { "ImagePath", T::kString }, { "HasContextMenu", T::kBool }, { "SizeToText", T::kBool },
            { "UseNuiAsMouse", T::kBool }, { "AutoId", T::kString }, { "HoverSelectTimer", T::kFloat },
            { "QuickInput", T::kBool })
        // Version 5 scenes' XuiControl (the Blades and NXE dashboards' and system UI's, XDK 21256's xui2bin): no
        // NavTabForward / NavTabBackward, which came with version 8; its bit 8 holds the Text (hud.xex 6683's buttons:
        // "Personal Settings"), bit 10 the ImagePath (vk.xex 2241's: "RT.png").
        TABLE(kControlV5,
            { "ClassOverride", T::kString }, { "Visual", T::kString }, { "Enabled", T::kBool },
            { "UnfocusedInput", T::kBool }, { "NavLeft", T::kString }, { "NavRight", T::kString },
            { "NavUp", T::kString }, { "NavDown", T::kString }, { "Text", T::kString }, { "PointSize", T::kFloat },
            { "ImagePath", T::kString }, { "HasContextMenu", T::kBool }, { "SizeToText", T::kBool },
            { "UseNuiAsMouse", T::kBool }, { "AutoId", T::kString }, { "HoverSelectTimer", T::kFloat },
            { "QuickInput", T::kBool })
        TABLE(kButton,
            { "PressKey", T::kUnsigned }, { "PressAnimObject", T::kString }, { "PressAnimStartFrame", T::kString },
            { "PressAnimEndFrame", T::kString }, { "FocusAnimObject", T::kString },
            { "FocusAnimStartFrame", T::kString }, { "FocusAnimEndFrame", T::kString })
        TABLE(kPressKey, { "PressKey", T::kUnsigned })
        TABLE(kStroke, { "StrokeWidth", T::kFloat }, { "StrokeColor", T::kColor })
        TABLE(kGradient,
            { "Radial", T::kBool }, { "NumStops", T::kInteger }, { "StopColor", T::kColor, I },
            { "StopPos", T::kFloat, I })
        TABLE(kFill,
            { "FillType", T::kUnsigned }, { "FillColor", T::kColor }, { "TextureFileName", T::kString },
            { "Gradient", T::kObject, 0, &kGradient }, { "Translation", T::kVector }, { "Scale", T::kVector },
            { "Rotation", T::kFloat }, { "WrapX", T::kUnsigned }, { "WrapY", T::kUnsigned },
            { "BrushFlags", T::kUnsigned }, { "TransformVersion", T::kUnsigned })
        TABLE(kFigure,
            { "Stroke", T::kObject, 0, &kStroke }, { "Fill", T::kObject, 0, &kFill }, { "Closed", T::kBool },
            { "Points", T::kCustom })
        TABLE(kText,
            { "Text", T::kString }, { "TextColor", T::kColor }, { "DropShadowColor", T::kColor },
            { "PointSize", T::kFloat }, { "Font", T::kString }, { "TextStyle", T::kUnsigned },
            { "LineSpacingAdjust", T::kInteger }, { "TextScale", T::kFloat })
        TABLE(kTextPresenter,
            { "TextColor", T::kColor }, { "DropShadowColor", T::kColor }, { "PointSize", T::kFloat },
            { "Font", T::kString }, { "TextStyle", T::kUnsigned }, { "LineSpacingAdjust", T::kInteger },
            { "DataAssociation", T::kUnsigned }, { "TextScale", T::kFloat })
        TABLE(kImage,
            { "SizeMode", T::kUnsigned }, { "ImagePath", T::kString }, { "BrushFlags", T::kUnsigned },
            { "TextureSurfaceElement", T::kString }, { "LoadType", T::kUnsigned })
        TABLE(kImagePresenter,
            { "SizeMode", T::kUnsigned }, { "DataAssociation", T::kUnsigned }, { "BrushFlags", T::kUnsigned },
            { "LoadType", T::kUnsigned })
        TABLE(kNineGrid,
            { "TextureFileName", T::kString }, { "LeftOffset", T::kUnsigned }, { "TopOffset", T::kUnsigned },
            { "RightOffset", T::kUnsigned }, { "BottomOffset", T::kUnsigned }, { "NoCenter", T::kBool })
        TABLE(kScene,
            { "DefaultFocus", T::kString }, { "TransFrom", T::kString }, { "TransTo", T::kString },
            { "TransBackFrom", T::kString }, { "TransBackTo", T::kString }, { "InterruptTransitions", T::kUnsigned },
            { "IgnorePresses", T::kBool }, { "RecurseTransitions", T::kBool })
        TABLE(kList, { "Wrap", T::kBool }, { "WrapBump", T::kBool })
        TABLE(kListItem,
            { "Layout", T::kUnsigned }, { "Checkable", T::kBool }, { "SelectedSize", T::kVector },
            { "KeepSizeUnfocused", T::kBool }, { "InterItemSpacing", T::kVector }, { "SmoothScroll", T::kBool },
            { "SmoothScrollBaseSpeed", T::kFloat }, { "SmoothScrollMaxSpeed", T::kFloat },
            { "SmoothScrollAcceleration", T::kFloat })
        TABLE(kCommonList, { "ItemsText", T::kString }, { "ItemsImage", T::kString }, { "ItemsNavPath", T::kString })
        TABLE(kEdit,
            { "TextLimit", T::kUnsigned }, { "AllowedChars", T::kString }, { "PasswordChar", T::kString },
            { "ReadOnly", T::kBool }, { "Multiline", T::kBool }, { "SmoothScroll", T::kBool })
        TABLE(kLabel, { "MaxFlowLines", T::kUnsigned })
        TABLE(kNavButton,
            { "PressPath", T::kString }, { "StayVisible", T::kBool }, { "SrcTransIndex", T::kUnsigned },
            { "DestTransIndex", T::kUnsigned })
        TABLE(kScrollBar, { "Direction", T::kUnsigned }, { "MinThumbSize", T::kUnsigned })
        TABLE(kDirection, { "Direction", T::kUnsigned })
        TABLE(kSlider,
            { "RangeMin", T::kInteger }, { "RangeMax", T::kInteger }, { "Value", T::kInteger }, { "Step", T::kInteger },
            { "Vertical", T::kBool }, { "AccelInc", T::kInteger }, { "AccelTime", T::kUnsigned })
        TABLE(kProgressBar, { "RangeMin", T::kInteger }, { "RangeMax", T::kInteger }, { "Value", T::kInteger })
        TABLE(kTabScene,
            { "TabCount", T::kUnsigned }, { "Wrap", T::kBool }, { "UserInterrupt", T::kBool },
            { "VerticalTabs", T::kBool }, { "NoAutoHide", T::kBool }, { "DefaultTab", T::kUnsigned })
        TABLE(kSound, { "State", T::kUnsigned }, { "Loop", T::kBool }, { "Finish", T::kBool }, { "Volume", T::kFloat })
        TABLE(kSoundXact,
            { "Cue", T::kString }, { "SoundBank", T::kString }, { "WaveBank", T::kString },
            { "DopplerScale", T::kFloat }, { "AudioPosition", T::kVector })
        TABLE(kSoundXAudio, { "File", T::kString })
        TABLE(kGridPanel,
            { "Columns", T::kString }, { "Rows", T::kString }, { "CellSpacing", T::kFloat }, { "Param0", T::kFloat },
            { "Param1", T::kFloat }, { "Param2", T::kFloat }, { "Param3", T::kFloat })
        TABLE(kGamerCard, { "Format", T::kString }, { "ShowExtendedPanel", T::kBool })
        TABLE(kVariable,
            { "Id", T::kString }, { "VectorVariable", T::kVector }, { "FloatVariable", T::kFloat },
            { "IntegerVariable", T::kInteger })
        TABLE(kShader,
            { "Id", T::kString }, { "ShaderFile", T::kString }, { "TextureFileName", T::kString },
            { "TextureSurfaceElement", T::kString }, { "WrapX", T::kUnsigned }, { "WrapY", T::kUnsigned },
            { "BrushFlags", T::kUnsigned }, { "CompositeEdge", T::kFloat }, { "ForceComposite", T::kBool },
            { "EffectParams1", T::kVector }, { "EffectParams2", T::kVector }, { "EffectParams3", T::kVector },
            { "EffectParams4", T::kVector }, { "EffectParams5", T::kVector })
        TABLE(kTextureSurface,
            { "Offscreen", T::kBool }, { "DepthStencil", T::kBool }, { "PreRender", T::kBool },
            { "Buffered", T::kBool })
        TABLE(kPerspectiveScene,
            { "ProjectionScale", T::kFloat }, { "ProjectionCenterU", T::kFloat }, { "ProjectionCenterV", T::kFloat })
        TABLE(kHtmlControl, { "LineHeight", T::kInteger }, { "TeletypeCount", T::kInteger })
        TABLE(kDataAssociation, { "DataAssociation", T::kUnsigned })
        TABLE(kAvatar,
            { "UseActiveUserAvatar", T::kBool }, { "EnableUserControl", T::kBool }, { "UseCustomView", T::kBool },
            { "RenderView", T::kUnsigned }, { "CustomViewEyePosition", T::kVector },
            { "CustomViewFocusPosition", T::kVector }, { "CustomViewUpDirection", T::kVector },
            { "ShowSelfShadow", T::kBool }, { "ShowShadow", T::kBool }, { "ShowCarryable", T::kBool },
            { "ShowHat", T::kBool }, { "FreezeAvatar", T::kBool }, { "UseAvateering", T::kBool },
            { "FullBodyAvateering", T::kBool }, { "AvateeringTrackingID", T::kUnsigned })
        TABLE(kTouch,
            { "PositionOffset", T::kVector }, { "HorizontalScrub", T::kBool }, { "HorizontalFling", T::kBool },
            { "HorizontalRubberBand", T::kBool }, { "HorizontalClamp", T::kBool }, { "VerticalScrub", T::kBool },
            { "VerticalFling", T::kBool }, { "VerticalRubberBand", T::kBool }, { "VerticalClamp", T::kBool },
            { "CenterSmallCanvas", T::kBool }, { "StretchToFit", T::kBool }, { "EnableBandBreak", T::kBool },
            { "Hollow", T::kBool })
        // XAM's own scene classes with properties.
        TABLE(kHudScene,
            { "OpenType", T::kUnsigned }, { "LegendA", T::kString }, { "LegendB", T::kString },
            { "LegendX", T::kString }, { "LegendY", T::kString }, { "ShowGamerInfo", T::kBool })
        TABLE(kDashScene, { "PanelSettings", T::kString }, { "PanelStrings", T::kString }, { "PanelScenePaths", T::kString })
        TABLE(kHubScene, { "PositionRay", T::kQuaternion }, { "ScreenOffset", T::kQuaternion })
        TABLE(kAuraControl,
            { "ThemeImageIndex", T::kUnsigned }, { "BackgroundImage", T::kString }, { "SurfaceSphere", T::kBool },
            { "BannerImage", T::kString }, { "BackgroundImagePanScale", T::kFloat }, { "SurfaceVignette", T::kBool })
        TABLE(kHandedness, { "Handedness", T::kUnsigned })
        TABLE(kNuiHover,
            { "Handedness", T::kUnsigned }, { "Magnetism", T::kBool }, { "NuiFastAttachRadius", T::kFloat },
            { "NuiSlowAttachRadius", T::kFloat }, { "NuiSlowDetachRadius", T::kFloat },
            { "NuiApproachRadius", T::kFloat }, { "MagnetOffset", T::kVector })
        TABLE(kNuiSideNav, { "Handedness", T::kUnsigned }, { "Magnetism", T::kBool }, { "NuiFastAttachRadius", T::kFloat })
        // hud.xex's Guide buttons (17559; property names from the module's strings).
        TABLE(kGuideMainSceneNav, { "MainScenePressPath", T::kString })
        TABLE(kGuideDashCommandNav, { "DashCommand", T::kUnsigned })
        // NXE hud.xex's (7258 ... 8955: its Marketplace blade's buttons; 9199: StorePickerNavButton), names from the
        // module's strings, each class's after its name.
        TABLE(kScriptActionNav, { "SceneName", T::kString }, { "TabToFocus", T::kUnsigned })
        TABLE(kStorePickerNav, { "TabToFocus", T::kUnsigned })
        // 17559's control pack MultiImage (dashuisk's skin.xur; its registration is XAM's): only the second property,
        // a string, is set in the skin, and the names are not known: placeholders.
        TABLE(kMultiImage, { "MultiImage0", T::kUnsigned }, { "MultiImage1", T::kString })
        // The Blades dashboards' banner classes ("banner of the day": the Xbox Live blade's ad, Marketplace and
        // Arcade banners). 1888's registrations (0x9218D010 / 0x9218D0A8) and property tables (0x9218CB30 /
        // 0x9218CD50), both based on XuiScene.
        TABLE(kBotdScene,
            { "Link", T::kUnsigned }, { "TitleId", T::kString }, { "OfferId", T::kString }, { "Flags", T::kUnsigned },
            { "ButtonVisual", T::kString })
        TABLE(kBotdContainer, { "Area", T::kUnsigned }, { "DefaultBanner", T::kString })
        constexpr PropTable kNone = { nullptr, 0 };
#undef TABLE

        const ClassDef kClasses[] = {
            { "XuiElement", nullptr, kElement },
            { "XuiCanvas", "XuiElement", kNone },
            { "XuiGroup", "XuiElement", kNone },
            { "XuiVisual", "XuiElement", kNone },
            { "XuiFigure", "XuiElement", kFigure },
            { "XuiText", "XuiElement", kText },
            { "XuiTextPresenter", "XuiElement", kTextPresenter },
            { "XuiImage", "XuiElement", kImage },
            { "XuiImagePresenter", "XuiElement", kImagePresenter },
            { "XuiHtmlPresenter", "XuiElement", kDataAssociation },
            { "XuiNineGrid", "XuiElement", kNineGrid },
            { "XuiSound", "XuiElement", kSound },
            { "XuiSoundXACT", "XuiSound", kSoundXact },
            { "XuiSoundXAudio", "XuiSound", kSoundXAudio },
            { "XuiGridPanel", "XuiElement", kGridPanel },
            { "XuiVariable", "XuiElement", kVariable },
            { "XuiShader", "XuiElement", kShader },
            { "XuiTransition", "XuiElement", kNone },
            { "XuiTextureSurface", "XuiGroup", kTextureSurface },
            { "XuiControl", "XuiElement", kControl },
            { "XuiButton", "XuiControl", kButton },
            { "XuiBackButton", "XuiButton", kNone },
            { "XuiNavButton", "XuiButton", kNavButton },
            { "XuiMessageBoxButton", "XuiButton", kNone },
            { "XuiCheckbox", "XuiControl", kPressKey },
            { "XuiRadioButton", "XuiControl", kPressKey },
            { "XuiRadioGroup", "XuiControl", kNone },
            { "XuiListItem", "XuiCheckbox", kListItem },
            { "XuiList", "XuiControl", kList },
            { "XuiCommonList", "XuiList", kCommonList },
            { "XuiEdit", "XuiControl", kEdit },
            { "XuiLabel", "XuiControl", kLabel },
            { "XuiCaret", "XuiControl", kNone },
            { "XuiScrollBar", "XuiControl", kScrollBar },
            { "XuiScrollEnd", "XuiControl", kDirection },
            { "XuiSlider", "XuiControl", kSlider },
            { "XuiProgressBar", "XuiControl", kProgressBar },
            { "XuiGamerCard", "XuiControl", kGamerCard },
            { "XuiHtmlControl", "XuiControl", kHtmlControl },
            { "XuiTouch", "XuiControl", kTouch },
            { "XuiAvatar", "XuiControl", kAvatar },
            { "XuiCompVideo", "XuiControl", kNone },
            { "XuiScene", "XuiControl", kScene },
            { "XuiTabScene", "XuiScene", kTabScene },
            { "XuiPerspectiveScene", "XuiScene", kPerspectiveScene },
            { "XuiMessageBox", "XuiScene", kNone },
            // XAM 17559 classes.
            { "HUDScene", "XuiScene", kHudScene },
            { "DashScene", "XuiScene", kDashScene },
            { "ControlPackHubScene", "XuiScene", kHubScene },
            { "ControlPackAuraControl", "XuiElement", kAuraControl },
            { "XuiBOTDScene", "XuiScene", kBotdScene },
            { "XuiBOTDContainer", "XuiScene", kBotdContainer },
            { "ControlPackNuiBack", "XuiControl", kHandedness },
            { "ControlPackNuiButton", "XuiButton", kNone },
            { "ControlPackNuiCheckbox", "XuiCheckbox", kNone },
            { "ControlPackNuiHoverBack", "XuiButton", kNuiHover },
            { "ControlPackNuiHoverButton", "XuiButton", kNuiHover },
            { "ControlPackNuiHoverCheckbox", "XuiCheckbox", kNuiHover },
            { "ControlPackNuiHoverChrome", "XuiButton", kNuiHover },
            { "ControlPackNuiRadioButton", "XuiRadioButton", kNone },
            { "ControlPackNuiSideNav", "XuiControl", kNuiSideNav },
            { "ControlPackNuiSwipeNav", "XuiControl", kHandedness },
            { "ControlPackNuiTipsScene", "XuiScene", kNone },
            { "ControlPackNuiVScroll", "XuiElement", kNone },
            { "ControlPackSimpleCursorScene", "XuiScene", kNone },
            { "ControlPackSystemGesture", "XuiElement", kNone },
            { "ControlPackLegacyControl", "XuiControl", kNone },
            { "ControlPackScourScene", "XuiPerspectiveScene", kNone },
            { "ControlPackVariablesScene", "XuiScene", kNone },
            { "ControlPackVuiBling", "XuiControl", kNone },
        };

        // XAM's scene classes without properties of their own, by base.
        const char* const kHudScenes[] = {
            "AchievementDetailsScene", "AchievementsGameListScene", "AddXBoxLiveScene", "AvatarAwardGamesMeScene",
            "CAchievementsGridScene", "CAvatarAwardsGridScene", "CSignInPreferencesScene", "ChangeGamerTileScene",
            "ChangePersonalTileScene", "CongratulationsScene", "EditProfileScene", "GameScene", "GameShowcaseMeScene",
            "GameShowcaseYouScene", "GamerCardUIScene", "GamerPreferencesScene", "GamerTagScene",
            "LoadAchievementsScene", "LoadAvatarAwardsScene", "LoadNuiAchievementsScene", "NuiAchievementDetailsScene",
            "NuiAchievementsGridScene", "NuiSignIn", "PreferenceCategoryScene", "PreferenceSettingScene",
            "RecoveryIntroScene", "SelectGamerZoneScene", "SignIn", "YouAchievementsScene",
        };
        const char* const kXuiScenes[] = {
            "BaseScene", "BrushImageControl", "ExtendedScene", "GamerCardScene", "InsPadScene", "JoinScene",
            "KeyboardControl", "MetaPanelScene", "NotifyPopupScene", "NuiBack", "NuiNotch", "NuiSideNav",
            "NuiUserListScene", "NuiVScroll", "PasscodeGridScene", "PasscodeScene", "ReputationScene",
            "SingleHandleVisual", "StatusScene", "UserListScene",
        };
        const char* const kXuiLists[] = {
            "AvatarAwardGameList", "CAchievementsGridList", "CAvatarAwardsGridList", "ClauseList", "GamerPreferencesList",
            "GamerTileList", "MeGameList", "PreferenceValueList", "UserList", "YouAchievementsList", "YouGameList",
        };
        const char* const kXuiControls[] = {
            "MeGamercardWorker", "SelectedProfile", "YouAsyncWorker", "YouGamercardWorker",
        };

        struct Registry
        {
            std::map<std::string, ClassDef, std::less<>> classes;
            Registry()
            {
                for (const ClassDef& c : kClasses)
                    classes.emplace(c.name, c);
                auto add = [this](const char* const* names, size_t count, const char* base) {
                    for (size_t i = 0; i < count; i++)
                        classes.emplace(names[i], ClassDef{ names[i], base, kNone });
                };
                add(kHudScenes, std::size(kHudScenes), "HUDScene");
                add(kXuiScenes, std::size(kXuiScenes), "XuiScene");
                add(kXuiLists, std::size(kXuiLists), "XuiList");
                add(kXuiControls, std::size(kXuiControls), "XuiControl");
                classes.emplace("AsyncTaskManager0", ClassDef{ "AsyncTaskManager0", "XuiElement", kNone });
                // The Blades and NXE dashboards' own classes without properties of their own (their version 5 scenes
                // read to the exact end with these bases: NXE's LegacyControl hosts the Blades screens it kept).
                classes.emplace("LegacyControl", ClassDef{ "LegacyControl", "XuiControl", kNone });
                for (const char* name : { "ScriptScene", "RomeRootScene", "MobyRootScene", "MediaScene" })
                    classes.emplace(name, ClassDef{ name, "XuiScene", kNone });
                classes.emplace("ScriptData", ClassDef{ "ScriptData", "XuiElement", kNone });
                // 17559's MultiImage: a control (its Visual names the skin's MultiImage visuals) with a level of its own.
                classes.emplace("MultiImage", ClassDef{ "MultiImage", "XuiControl", kMultiImage });
                // NXE's theme background (homepage.xur): the properties of 17559's ControlPackAuraControl.
                classes.emplace("AuraControl", ClassDef{ "AuraControl", "XuiElement", kAuraControl });
                classes.emplace("CXuiEditAutoScroll", ClassDef{ "CXuiEditAutoScroll", "XuiEdit", kNone });
                classes.emplace("GamerPicButton", ClassDef{ "GamerPicButton", "XuiButton", kNone });
                classes.emplace("VKXuiButton", ClassDef{ "VKXuiButton", "XuiButton", kNone });
                // hud.xex's Guide buttons without properties of their own (most scenes name them as a ClassOverride
                // of an XuiNavButton).
                // Derived from the dash-command button (SettingsTabSignedIn.xur: its DashCommand level precedes its own).
                classes.emplace("AccountManagementNavButton", ClassDef{ "AccountManagementNavButton", "GuideDashCommandNavButton", kNone });
                for (const char* name : { "AchievementsNavButton", "ActiveDownloadsNavButton",
                         "ActivityFeedNavButton", "AvatarAwardsNavButton", "BeaconsNavButton", "CommunitySessionsNavButton",
                         "ConnectToLiveNavButton", "CreateNewProfileNavButton", "FriendsNavButton", "LiveRequiredNavButton",
                         "MessagesNavButton", "NatalTroubleshooterNavButton", "PartyNavButton", "ProfileNavButton",
                         "QuickChatNavButton", "RecoverGamerTagNavButton", "RedeemCodeNavButton", "ShutdownNavButton",
                         "SignInNavButton", "TrayNavButton" })
                    classes.emplace(name, ClassDef{ name, "XuiNavButton", kNone });
                classes.emplace("GuideMainSceneNavButton", ClassDef{ "GuideMainSceneNavButton", "XuiNavButton", kGuideMainSceneNav });
                classes.emplace("GuideDashCommandNavButton", ClassDef{ "GuideDashCommandNavButton", "XuiNavButton", kGuideDashCommandNav });
                classes.emplace("ScriptActionNavButton", ClassDef{ "ScriptActionNavButton", "XuiNavButton", kScriptActionNav });
                classes.emplace("StorePickerNavButton", ClassDef{ "StorePickerNavButton", "XuiNavButton", kStorePickerNav });
            }
        };
        const Registry& Classes()
        {
            static const Registry registry;
            return registry;
        }

        uint32_t Be32(const uint8_t* p) { return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]; }
        uint16_t Be16(const uint8_t* p) { return uint16_t(p[0] << 8 | p[1]); }
        float BeFloat(const uint8_t* p) { return std::bit_cast<float>(Be32(p)); }
    }

    const ClassDef* FindClass(std::string_view name)
    {
        const auto& classes = Classes().classes;
        const auto it = classes.find(name);
        return it == classes.end() ? nullptr : &it->second;
    }

    std::vector<const ClassDef*> ClassChain(std::string_view name)
    {
        std::vector<const ClassDef*> chain;
        const ClassDef* c = FindClass(name);
        while (c)
        {
            chain.push_back(c);
            if (!c->base)
            {
                std::reverse(chain.begin(), chain.end());
                return chain;
            }
            c = FindClass(c->base);
        }
        return {};
    }

    const Value* Node::Find(std::string_view name) const
    {
        for (const Property& p : props)
            if (name == p.def->name && !p.values.empty())
                return &p.values.front();
        return nullptr;
    }

    std::string Node::Id() const
    {
        const Value* v = Find("Id");
        return v ? v->s : std::string();
    }

    struct Scene::Reader
    {
        const uint8_t* p;
        size_t size, at = 0;
        bool failed = false;

        bool Eof() const { return at >= size; }
        uint8_t Byte()
        {
            if (at >= size)
            {
                failed = true;
                return 0;
            }
            return p[at++];
        }
        uint32_t Packed()
        {
            const uint8_t lead = Byte();
            if (lead == 0xFF)
            {
                uint32_t v = 0;
                for (int i = 0; i < 4; i++)
                    v = v << 8 | Byte();
                return v;
            }
            if (lead >= 0xF0)
                return uint32_t(lead & 0x0F) << 8 | Byte();
            return lead;
        }
    };

    bool Scene::Load(const uint8_t* data, size_t size, std::string& error, bool version8Control)
    {
        version8Control_ = version8Control;
        data_.assign(data, data + size);
        const uint8_t* d = data_.data();
        if (size < 0x14 || std::memcmp(d, "XUIB", 4) != 0)
        {
            error = "not an XUIB scene";
            return false;
        }
        const uint32_t version = Be32(d + 4), total = Be32(d + 0x0E);
        const uint16_t sections = Be16(d + 0x12);
        if (version == 5)
            return LoadV5(error);
        if (version != 8)
        {
            error = std::format("scene version {} (only 5 and 8 are known)", version);
            return false;
        }
        if (total != size)
        {
            error = std::format("scene size {} does not match the file ({})", total, size);
            return false;
        }
        Reader header{ d + 0x14, size - 0x14 };
        for (int i = 0; i < 12; i++)
            header.Packed(); // pool sizes (allocation hints)
        size_t at = 0x14 + header.at;
        for (uint16_t i = 0; i < sections; i++)
        {
            if (at + 12 > size)
            {
                error = "section table runs past the end";
                return false;
            }
            const std::string tag(reinterpret_cast<const char*>(d + at), 4);
            const uint32_t offset = Be32(d + at + 4), length = Be32(d + at + 8);
            at += 12;
            if (uint64_t(offset) + length > size)
            {
                error = std::format("section {} runs past the end", tag);
                return false;
            }
            sections_[tag] = { d + offset, length };
        }
        if (!ReadPools(error))
            return false;
        const auto it = sections_.find("DATA");
        if (it == sections_.end())
        {
            error = "no DATA section";
            return false;
        }
        Reader r{ it->second.data, it->second.size };
        if (!ReadObject(r, root_, error, 0))
            return false;
        if (r.failed || r.at != r.size)
        {
            error = std::format("object stream: {} of {} bytes read{}", r.at, r.size, r.failed ? " (overrun)" : "");
            return false;
        }
        return true;
    }

    bool Scene::ReadPools(std::string& error)
    {
        auto section = [this](const char* tag) -> const Section* {
            const auto it = sections_.find(tag);
            return it == sections_.end() ? nullptr : &it->second;
        };
        if (const Section* s = section("STRN"))
        {
            if (s->size < 6)
            {
                error = "STRN too small";
                return false;
            }
            const uint16_t count = Be16(s->data + 4);
            size_t at = 6;
            while (strings_.size() < count)
            {
                const uint8_t* end = static_cast<const uint8_t*>(std::memchr(s->data + at, 0, s->size - at));
                if (!end)
                {
                    error = "STRN: unterminated string";
                    return false;
                }
                strings_.emplace_back(reinterpret_cast<const char*>(s->data + at), end - (s->data + at));
                at = size_t(end - s->data) + 1;
            }
        }
        if (const Section* s = section("FLOT"))
            for (size_t at = 0; at + 4 <= s->size; at += 4)
                floats_.push_back(BeFloat(s->data + at));
        if (const Section* s = section("VECT"))
            for (size_t at = 0; at + 12 <= s->size; at += 12)
                vectors_.push_back({ BeFloat(s->data + at), BeFloat(s->data + at + 4), BeFloat(s->data + at + 8) });
        if (const Section* s = section("QUAT"))
            for (size_t at = 0; at + 16 <= s->size; at += 16)
                quats_.push_back({ BeFloat(s->data + at), BeFloat(s->data + at + 4), BeFloat(s->data + at + 8),
                                   BeFloat(s->data + at + 12) });
        if (const Section* s = section("COLR"))
            for (size_t at = 0; at + 4 <= s->size; at += 4)
                colors_.push_back(Be32(s->data + at));
        if (const Section* s = section("CUST"))
        {
            size_t at = 0;
            while (at < s->size)
            {
                if (at + 16 > s->size)
                {
                    error = "CUST: truncated record";
                    return false;
                }
                const uint32_t length = Be32(s->data + at);
                const uint8_t* body = s->data + at + 4;
                FigurePath path;
                path.width = BeFloat(body);
                path.height = BeFloat(body + 4);
                const uint32_t count = Be32(body + 8);
                if (length != 12 + count * 24 || at + 4 + length > s->size)
                {
                    error = std::format("CUST: record at {} has size {} for {} points", at, length, count);
                    return false;
                }
                for (uint32_t i = 0; i < count; i++)
                {
                    const uint8_t* q = body + 12 + i * 24;
                    path.points.push_back({ BeFloat(q), BeFloat(q + 4), BeFloat(q + 8), BeFloat(q + 12),
                                            BeFloat(q + 16), BeFloat(q + 20) });
                }
                figures_.emplace(uint32_t(at), std::move(path));
                at += 4 + length;
            }
        }
        if (const Section* s = section("KEYP"))
        {
            Reader r{ s->data, s->size };
            while (!r.Eof())
                keyValues_.push_back(r.Packed());
            if (r.failed)
            {
                error = "KEYP: truncated value";
                return false;
            }
        }
        if (const Section* s = section("KEYD"))
        {
            Reader r{ s->data, s->size };
            while (!r.Eof())
            {
                KeyRecord k{};
                k.frame = r.Packed();
                k.mode = r.Byte();
                // As XAM's loader (FUN_817e0570): ease carries 3 bytes, the parametric curves a VECT index.
                switch (Interpolation(k.mode & 0x3F))
                {
                case Interpolation::kEase:
                    for (int i = 0; i < 3; i++)
                        k.ease[i] = int8_t(r.Byte());
                    break;
                case Interpolation::kPower:
                case Interpolation::kExponential:
                case Interpolation::kElastic:
                case Interpolation::kBounce:
                    k.vector = r.Packed();
                    if (k.vector >= vectors_.size())
                    {
                        error = std::format("KEYD: key vector {} outside VECT ({})", k.vector, vectors_.size());
                        return false;
                    }
                    break;
                case Interpolation::kLinear:
                case Interpolation::kNone:
                case Interpolation::kQuadratic:
                case Interpolation::kCubic:
                case Interpolation::kQuartic:
                case Interpolation::kQuintic:
                case Interpolation::kSine:
                case Interpolation::kCircular:
                    break;
                default:
                    error = std::format("KEYD: key frame interpolation {} (mode {:02X}) is not known", k.mode & 0x3F,
                                        k.mode);
                    return false;
                }
                k.valueBase = r.Packed();
                if (r.failed)
                {
                    error = "KEYD: truncated key frame";
                    return false;
                }
                keyRecords_.push_back(k);
            }
        }
        if (const Section* s = section("NAME"))
        {
            Reader r{ s->data, s->size };
            while (!r.Eof())
            {
                NamedFrame n;
                const uint32_t name = r.Packed();
                n.name = name && name <= strings_.size() ? strings_[name - 1] : std::string();
                n.frame = r.Packed();
                const uint8_t command = r.Byte();
                if (command > uint8_t(Command::kGotoAndStop))
                {
                    error = std::format("NAME: command {} is not known", command);
                    return false;
                }
                n.command = Command(command);
                if (n.command >= Command::kGoto)
                {
                    const uint32_t target = r.Packed();
                    n.target = target && target <= strings_.size() ? strings_[target - 1] : std::string();
                }
                if (r.failed)
                {
                    error = "NAME: truncated record";
                    return false;
                }
                nameRecords_.push_back(std::move(n));
            }
        }
        return true;
    }

    Value Scene::PoolValue(Type type, uint32_t index) const
    {
        Value v;
        v.type = type;
        switch (type)
        {
        case Type::kBool: v.b = index != 0; break;
        case Type::kInteger: v.i = int32_t(index); break;
        case Type::kUnsigned: v.i = index; break;
        case Type::kFloat: v.f = index < floats_.size() ? floats_[index] : 0.0f; break;
        case Type::kString: v.s = index && index <= strings_.size() ? strings_[index - 1] : std::string(); break;
        case Type::kColor: v.color = index < colors_.size() ? colors_[index] : 0; break;
        case Type::kVector: v.v = index < vectors_.size() ? vectors_[index] : Vec3{}; break;
        case Type::kQuaternion: v.q = index < quats_.size() ? quats_[index] : Quat{}; break;
        case Type::kCustom: v.custom = index; break;
        default: break;
        }
        return v;
    }

    bool Scene::ReadValue(Reader& r, const PropDef& def, Value& out, std::string& error)
    {
        if (def.type == Type::kBool)
        {
            out.type = Type::kBool;
            out.b = r.Byte() != 0;
            return true;
        }
        if (def.type != Type::kObject)
        {
            const uint32_t index = r.Packed();
            const size_t limit = def.type == Type::kFloat ? floats_.size() : def.type == Type::kString ? strings_.size() + 1 :
                def.type == Type::kColor ? colors_.size() : def.type == Type::kVector ? vectors_.size() :
                def.type == Type::kQuaternion ? quats_.size() : SIZE_MAX;
            if (index >= limit)
            {
                error = std::format("{} index {} is outside its pool ({})", def.name, index, limit);
                return false;
            }
            if (def.type == Type::kCustom && !figures_.contains(index))
            {
                error = std::format("{}: no figure path at CUST offset {}", def.name, index);
                return false;
            }
            out = PoolValue(def.type, index);
            return true;
        }
        // A property object is written in full where it is first used, then referred to by its index.
        out.type = Type::kObject;
        const uint32_t index = r.Packed();
        if (const auto it = objects_.find(index); it != objects_.end())
        {
            out.object = it->second;
            return true;
        }
        auto object = std::make_shared<Node>();
        object->className = def.name;
        const uint32_t count = r.Packed();
        if (!ReadValues(r, *def.sub, object->props, error))
            return false;
        size_t read = 0;
        for (const Property& p : object->props)
            read += p.values.size();
        if (read != count)
        {
            error = std::format("{} object: {} values announced, {} read", def.name, count, read);
            return false;
        }
        objects_.emplace(index, object);
        out.object = object;
        return true;
    }

    bool Scene::ReadValues(Reader& r, const PropTable& table, std::vector<Property>& out, std::string& error)
    {
        const uint32_t mask = r.Packed();
        if (table.count < 32 && (mask >> table.count) != 0)
        {
            error = std::format("property mask {:X} has bits past the class's {} properties", mask, table.count);
            return false;
        }
        for (uint32_t i = 0; i < table.count; i++)
        {
            if (!(mask & (1u << i)))
                continue;
            const PropDef& def = table.defs[i];
            Property p{ &def };
            const uint32_t count = (def.flags & kPropIndexed) ? r.Packed() : 1;
            p.values.resize(count);
            for (Value& v : p.values)
                if (!ReadValue(r, def, v, error))
                    return false;
            out.push_back(std::move(p));
        }
        return !r.failed || (error = "property values run past the end", false);
    }

    bool Scene::ReadObject(Reader& r, Node& node, std::string& error, int depth)
    {
        if (depth > 64)
        {
            error = "objects nested too deeply";
            return false;
        }
        const uint32_t classIndex = r.Packed();
        const uint8_t flags = r.Byte();
        node.className = classIndex && classIndex <= strings_.size() ? strings_[classIndex - 1] : std::string();
        if ((flags & ~0x0Fu) || (flags & 9) == 9)
        {
            error = std::format("{}: object flags {:02X}", node.className, flags);
            return false;
        }
        if (flags & 8)
        {
            const uint32_t index = r.Packed();
            if (index >= propertyBlocks_.size() || propertyBlocks_[index]->className != node.className)
            {
                error = std::format("{}: shared properties {} ({} blocks so far{})", node.className, index,
                                    propertyBlocks_.size(), index < propertyBlocks_.size() ?
                                        ", of class " + propertyBlocks_[index]->className : std::string());
                return false;
            }
            node.props = propertyBlocks_[index]->props;
        }
        if (flags & 1)
        {
            const auto chain = ClassChain(node.className);
            if (chain.empty())
            {
                error = std::format("class {} is not known", node.className);
                return false;
            }
            const uint32_t count = r.Packed();
            for (const ClassDef* c : chain)
                if (!ReadValues(r, c->props, node.props, error))
                    return (error = std::format("{} ({} level): {}", node.className, c->name, error)), false;
            size_t read = 0;
            for (const Property& p : node.props)
                read += p.values.size();
            if (read != count)
            {
                error = std::format("{}: {} values announced, {} read", node.className, count, read);
                return false;
            }
            propertyBlocks_.push_back(&node);
        }
        if (flags & 2)
        {
            const uint32_t count = r.Packed();
            node.children.resize(count);
            for (Node& child : node.children)
                if (!ReadObject(r, child, error, depth + 1))
                    return false;
        }
        if (flags & 4)
            return ReadTimelines(r, node, error);
        return !r.failed || (error = "object stream runs past the end", false);
    }

    bool Scene::ResolveTrack(const std::string& className, Track& track) const
    {
        // The level counts up the class chain from the target's own class.
        const auto chain = ClassChain(className);
        if (track.level >= chain.size())
            return false;
        const ClassDef* level = chain[chain.size() - 1 - track.level];
        if (track.index >= level->props.count)
            return false;
        const PropDef* def = &level->props.defs[track.index];
        std::string name = def->name;
        for (uint8_t step : track.path)
        {
            if (def->type != Type::kObject || !def->sub || step >= def->sub->count)
                return false;
            def = &def->sub->defs[step];
            name += std::string(".") + def->name;
        }
        track.def = def;
        track.name = std::move(name);
        return true;
    }

    bool Scene::ReadTimelines(Reader& r, Node& node, std::string& error)
    {
        const uint32_t named = r.Packed();
        if (named)
        {
            const uint32_t base = r.Packed();
            if (uint64_t(base) + named > nameRecords_.size())
            {
                error = std::format("{}: named frames {}+{} outside NAME ({})", node.className, base, named,
                                    nameRecords_.size());
                return false;
            }
            node.namedFrames.assign(nameRecords_.begin() + base, nameRecords_.begin() + base + named);
        }
        if (node.children.empty())
            return !r.failed || (error = "timelines run past the end", false);
        const uint32_t count = r.Packed();
        for (uint32_t t = 0; t < count; t++)
        {
            Timeline timeline;
            const uint32_t target = r.Packed();
            timeline.target = target && target <= strings_.size() ? strings_[target - 1] : std::string();
            const Node* targetNode = nullptr;
            for (const Node& child : node.children)
                if (child.Id() == timeline.target)
                {
                    targetNode = &child;
                    break;
                }
            const uint32_t tracks = r.Packed();
            for (uint32_t i = 0; i < tracks; i++)
            {
                Track track;
                const uint8_t head = r.Byte();
                const uint8_t depth = head & 0x7F;
                if (depth == 0)
                {
                    error = std::format("{}: timeline {} has a track without a property", node.className, timeline.target);
                    return false;
                }
                track.level = r.Byte();
                track.index = r.Byte();
                for (uint8_t j = 1; j < depth; j++)
                    track.path.push_back(r.Byte());
                if (head & 0x80)
                {
                    track.hasIndex = true;
                    track.element = r.Packed();
                }
                if (!targetNode || !ResolveTrack(targetNode->className, track))
                {
                    error = std::format("{}: timeline track {}/{} of {} does not resolve", node.className, track.level,
                                        track.index, timeline.target);
                    return false;
                }
                timeline.tracks.push_back(std::move(track));
            }
            const uint32_t keys = r.Packed();
            const uint32_t keyBase = r.Packed();
            if (uint64_t(keyBase) + keys > keyRecords_.size())
            {
                error = std::format("{}: key frames {}+{} outside KEYD ({})", node.className, keyBase, keys,
                                    keyRecords_.size());
                return false;
            }
            for (uint32_t k = 0; k < keys; k++)
            {
                const KeyRecord& rec = keyRecords_[keyBase + k];
                KeyFrame key;
                key.frame = rec.frame;
                key.interpolation = Interpolation(rec.mode & 0x3F);
                key.easeIn = rec.ease[0];
                key.easeOut = rec.ease[1];
                key.easeScale = rec.ease[2];
                key.direction = EaseDirection(rec.mode >> 6);
                if (key.interpolation == Interpolation::kEase)
                    EaseControls(key.easeIn, key.easeOut, key.easeScale, key.p1, key.p2);
                else if (key.interpolation == Interpolation::kPower || key.interpolation == Interpolation::kExponential ||
                         key.interpolation == Interpolation::kElastic || key.interpolation == Interpolation::kBounce)
                {
                    key.p1 = vectors_[rec.vector].x;
                    key.p2 = vectors_[rec.vector].y;
                }
                for (size_t i = 0; i < timeline.tracks.size(); i++)
                {
                    const size_t at = size_t(rec.valueBase) + i;
                    if (at >= keyValues_.size())
                    {
                        error = std::format("{}: key value {} outside KEYP ({})", node.className, at, keyValues_.size());
                        return false;
                    }
                    key.values.push_back(PoolValue(timeline.tracks[i].def->type, keyValues_[at]));
                }
                timeline.keys.push_back(std::move(key));
            }
            node.timelines.push_back(std::move(timeline));
        }
        return !r.failed || (error = "timelines run past the end", false);
    }

    // ---- Version 5 ----
    //
    // The XDK's format (its xui2bin, XuiTool) and the one the Blades and NXE dashboards' scenes use (2.0.1888 ...
    // 9199). Worked out from the XDK 21256 xui2bin's output for scenes written to isolate each field, checked against
    // its XML samples and every scene of the dashboards' packages:
    //   header as version 8's without the pool sizes (the section table starts at 0x14);
    //   STRN: { u16 length; UTF-16BE characters } per string (index 1 = first, 0 = empty); VECT, QUAT, CUST as in 8;
    //   no FLOT/COLR/KEYP/KEYD/NAME: floats, colours, integers, key frames and named frames are inline.
    //   Object: u16 class (string index), u8 flags (1 properties, 2 children, 4 timelines), [u16 value count, then per
    //   class level (base first) a byte X = (properties - 1) << 3 | mask bytes (0: no properties at that level), the
    //   level's mask (big-endian, X & 7 bytes; the same bits as version 8's tables, whose earlier properties these
    //   are) and the values], [u32 child count, children],
    //   [timelines].
    //   Values: bool u8; integer, unsigned, float, colour u32; string u16 index; vector, quaternion u32 pool index;
    //   figure u32 CUST offset; an indexed property a u8 count then its values; an object u16 value count, then its
    //   one level (X, mask, values). Default values are not written.
    //   Timelines: u32 named frames { u16 name, u32 frame, u8 command, u16 target (0 but for goto commands) }; with children,
    //   u32 timelines { u16 target, u32 tracks { u8 depth (| 0x80 indexed), u8 level (from the target's own class),
    //   u8 index, depth - 1 bytes of object path, [u32 element] }, u32 keys { u32 frame, u8 interpolation, i8 ease
    //   in, ease out, ease scale, then one value per track as above } }.
    struct Scene::V5Reader
    {
        Scene& scene;
        const uint8_t* p;
        size_t size, at = 0;
        bool failed = false;

        uint8_t U8()
        {
            if (at + 1 > size)
                return failed = true, 0;
            return p[at++];
        }
        uint16_t U16()
        {
            if (at + 2 > size)
                return failed = true, 0;
            at += 2;
            return Be16(p + at - 2);
        }
        uint32_t U32()
        {
            if (at + 4 > size)
                return failed = true, 0;
            at += 4;
            return Be32(p + at - 4);
        }
        std::string String(uint16_t index) const
        {
            return index && index <= scene.strings_.size() ? scene.strings_[index - 1] : std::string();
        }

        bool Value(const PropDef& def, xui::Value& out, std::string& error)
        {
            out.type = def.type;
            switch (def.type)
            {
            case Type::kBool: out.b = U8() != 0; return true;
            case Type::kInteger: out.i = int32_t(U32()); return true;
            case Type::kUnsigned: out.i = U32(); return true;
            case Type::kFloat: out.f = std::bit_cast<float>(U32()); return true;
            case Type::kColor: out.color = U32(); return true;
            case Type::kString:
            {
                const uint16_t index = U16();
                if (index > scene.strings_.size())
                    return (error = std::format("{}: string {} outside STRN ({})", def.name, index, scene.strings_.size())), false;
                out.s = String(index);
                return true;
            }
            case Type::kVector:
            {
                const uint32_t index = U32();
                if (index >= scene.vectors_.size())
                    return (error = std::format("{}: vector {} outside VECT ({})", def.name, index, scene.vectors_.size())), false;
                out.v = scene.vectors_[index];
                return true;
            }
            case Type::kQuaternion:
            {
                const uint32_t index = U32();
                if (index >= scene.quats_.size())
                    return (error = std::format("{}: quaternion {} outside QUAT ({})", def.name, index, scene.quats_.size())), false;
                out.q = scene.quats_[index];
                return true;
            }
            case Type::kCustom:
                out.custom = U32();
                if (!scene.figures_.contains(out.custom))
                    return (error = std::format("{}: no figure path at CUST offset {}", def.name, out.custom)), false;
                return true;
            case Type::kObject:
            {
                auto object = std::make_shared<Node>();
                object->className = def.name;
                const uint16_t count = U16();
                if (!Level(*def.sub, object->props, error))
                    return false;
                size_t read = 0;
                for (const Property& p : object->props)
                    read += p.values.size();
                if (read != count)
                    return (error = std::format("{} object: {} values announced, {} read", def.name, count, read)), false;
                out.object = std::move(object);
                return true;
            }
            default:
                return (error = std::format("{}: a property of type {}", def.name, int(def.type))), false;
            }
        }

        // One class level: X, the mask, the values.
        bool Level(const PropTable& table, std::vector<Property>& out, std::string& error)
        {
            const uint8_t x = U8();
            if (x == 0)
                return !failed || (error = "property levels run past the end", false);
            const uint32_t bytes = x & 7;
            if (bytes == 0 || bytes > 4)
                return (error = std::format("level byte {:02X}: a {}-byte mask", x, bytes)), false;
            uint32_t mask = 0;
            for (uint32_t i = 0; i < bytes; i++)
                mask = mask << 8 | U8();
            if (uint32_t(std::popcount(mask)) != (x >> 3) + 1u)
                return (error = std::format("level byte {:02X} at {:X} announces {} properties, mask {:X} has {}", x,
                                            at - 1 - bytes, (x >> 3) + 1, mask, std::popcount(mask))), false;
            if (table.count < 32 && (mask >> table.count) != 0)
                return (error = std::format("property mask {:X} has bits past the class's {} properties", mask,
                                            table.count)), false;
            for (uint32_t i = 0; i < table.count; i++)
            {
                if (!(mask & (1u << i)))
                    continue;
                const PropDef& def = table.defs[i];
                Property prop{ &def };
                const uint32_t count = (def.flags & kPropIndexed) ? U8() : 1;
                prop.values.resize(count);
                for (xui::Value& v : prop.values)
                    if (!Value(def, v, error))
                        return false;
                out.push_back(std::move(prop));
            }
            return !failed || (error = "property values run past the end", false);
        }

        bool Object(Node& node, std::string& error, int depth)
        {
            if (depth > 64)
                return (error = "objects nested too deeply"), false;
            node.className = String(U16());
            const uint8_t flags = U8();
            if (flags & ~0x07u)
            {
                std::string next;
                for (size_t i = at; i < at + 24 && i < size; i++)
                    next += std::format("{:02X}", p[i]);
                return (error = std::format("{}: object flags {:02X} at {:X} (then {})", node.className, flags, at - 1,
                                            next)), false;
            }
            if (flags & 1)
            {
                const auto chain = ClassChain(node.className);
                if (chain.empty())
                    return (error = std::format("class {} is not known", node.className)), false;
                const uint16_t count = U16();
                for (const ClassDef* c : chain)
                {
                    const bool control = c->props.defs == kControl.defs;
                    const size_t first = node.props.size();
                    if (!Level(control && !scene.version8Control_ ? kControlV5 : c->props, node.props, error))
                        return (error = std::format("{} ({} level): {}", node.className, c->name, error)), false;
                    // Its properties as the class's own (the writer and the version 8 table know them by those).
                    if (control)
                        for (size_t i = first; i < node.props.size(); i++)
                            node.props[i].def = &kControl.defs[std::find_if(kControl.defs, kControl.defs + kControl.count,
                                [&](const PropDef& d) { return std::string_view(d.name) == node.props[i].def->name; }) - kControl.defs];
                }
                size_t read = 0;
                for (const Property& p : node.props)
                    read += p.values.size();
                if (read != count)
                    return (error = std::format("{}: {} values announced, {} read", node.className, count, read)), false;
            }
            if (flags & 2)
            {
                const uint32_t count = U32();
                if (count > size)
                    return (error = std::format("{}: {} children", node.className, count)), false;
                node.children.resize(count);
                for (Node& child : node.children)
                    if (!Object(child, error, depth + 1))
                        return false;
            }
            if (flags & 4)
                return Timelines(node, error);
            return !failed || (error = "object stream runs past the end", false);
        }

        bool Timelines(Node& node, std::string& error)
        {
            const uint32_t named = U32();
            for (uint32_t i = 0; i < named && !failed; i++)
            {
                NamedFrame n;
                n.name = String(U16());
                n.frame = U32();
                const uint8_t command = U8();
                if (command > uint8_t(Command::kGotoAndStop))
                {
                    std::string around;
                    for (size_t j = at >= 16 ? at - 16 : 0; j < at + 16 && j < size; j++)
                        around += j + 1 == at ? std::format("[{:02X}]", p[j]) : std::format("{:02X}", p[j]);
                    return (error = std::format("{}: named frame {} of {}: command {} ({})", node.className, i, named,
                                                command, around)), false;
                }
                n.command = Command(command);
                n.target = String(U16()); // written for every command (0: none)
                node.namedFrames.push_back(std::move(n));
            }
            if (node.children.empty())
                return !failed || (error = "timelines run past the end", false);
            const uint32_t count = U32();
            for (uint32_t t = 0; t < count && !failed; t++)
            {
                Timeline timeline;
                timeline.target = String(U16());
                const Node* target = nullptr;
                for (const Node& child : node.children)
                    if (child.Id() == timeline.target)
                    {
                        target = &child;
                        break;
                    }
                const uint32_t tracks = U32();
                for (uint32_t i = 0; i < tracks && !failed; i++)
                {
                    Track track;
                    const uint8_t head = U8();
                    const uint8_t depth = head & 0x7F;
                    if (depth == 0)
                        return (error = std::format("{}: timeline {} has a track without a property", node.className,
                                                    timeline.target)), false;
                    track.level = U8();
                    track.index = U8();
                    for (uint8_t j = 1; j < depth; j++)
                        track.path.push_back(U8());
                    if (head & 0x80)
                    {
                        track.hasIndex = true;
                        track.element = U32();
                    }
                    bool resolved = target && scene.ResolveTrack(target->className, track);
                    // An XuiControl property: its index is the version 5 table's (kControlV5), the class's by name.
                    if (resolved && !scene.version8Control_ && track.path.empty() && track.def >= kControl.defs &&
                        track.def < kControl.defs + kControl.count)
                    {
                        if (track.index >= kControlV5.count)
                            resolved = false;
                        else
                        {
                            const std::string_view name = kControlV5.defs[track.index].name;
                            track.index = uint8_t(std::find_if(kControl.defs, kControl.defs + kControl.count,
                                [&](const PropDef& d) { return std::string_view(d.name) == name; }) - kControl.defs);
                            resolved = scene.ResolveTrack(target->className, track);
                        }
                    }
                    if (!resolved)
                        return (error = std::format("{}: timeline track {}/{} of {} does not resolve", node.className,
                                                    track.level, track.index, timeline.target)), false;
                    timeline.tracks.push_back(std::move(track));
                }
                const uint32_t keys = U32();
                for (uint32_t k = 0; k < keys && !failed; k++)
                {
                    KeyFrame key;
                    key.frame = U32();
                    const uint8_t mode = U8();
                    if ((mode & 0x3F) > uint8_t(Interpolation::kEase))
                        return (error = std::format("{}: key frame interpolation {:02X}", node.className, mode)), false;
                    key.interpolation = Interpolation(mode & 0x3F);
                    key.direction = EaseDirection(mode >> 6);
                    key.easeIn = int8_t(U8());
                    key.easeOut = int8_t(U8());
                    key.easeScale = int8_t(U8());
                    if (key.interpolation == Interpolation::kEase)
                        EaseControls(key.easeIn, key.easeOut, key.easeScale, key.p1, key.p2);
                    for (const Track& tr : timeline.tracks)
                    {
                        xui::Value v;
                        if (!Value(*tr.def, v, error))
                            return (error = std::format("{}: timeline {}: {}", node.className, timeline.target, error)), false;
                        key.values.push_back(std::move(v));
                    }
                    timeline.keys.push_back(std::move(key));
                }
                node.timelines.push_back(std::move(timeline));
            }
            return !failed || (error = "timelines run past the end", false);
        }
    };

    bool Scene::LoadV5(std::string& error)
    {
        const uint8_t* d = data_.data();
        const size_t size = data_.size();
        const uint32_t total = Be32(d + 0x0E);
        const uint16_t sections = Be16(d + 0x12);
        if (total != size)
        {
            error = std::format("scene size {} does not match the file ({})", total, size);
            return false;
        }
        // Header flag 1 (the dashboards' scenes, the XDK's skin): ten u32 allocation hints before the section table.
        const uint32_t headerFlags = Be32(d + 8);
        if (headerFlags & ~1u)
        {
            error = std::format("header flags {:X} (only 1 is known)", headerFlags);
            return false;
        }
        size_t at = (headerFlags & 1) ? 0x14 + 40 : 0x14;
        for (uint16_t i = 0; i < sections; i++)
        {
            if (at + 12 > size)
            {
                error = "section table runs past the end";
                return false;
            }
            const std::string tag(reinterpret_cast<const char*>(d + at), 4);
            const uint32_t offset = Be32(d + at + 4), length = Be32(d + at + 8);
            at += 12;
            if (uint64_t(offset) + length > size)
            {
                error = std::format("section {} runs past the end", tag);
                return false;
            }
            sections_[tag] = { d + offset, length };
        }
        if (const auto it = sections_.find("STRN"); it != sections_.end())
        {
            const Section& s = it->second;
            for (size_t q = 0; q < s.size;)
            {
                if (q + 2 > s.size || q + 2 + size_t(Be16(s.data + q)) * 2 > s.size)
                {
                    error = "STRN: truncated string";
                    return false;
                }
                const size_t length = Be16(s.data + q);
                std::string utf8;
                for (size_t c = 0; c < length; c++)
                {
                    const uint32_t ch = Be16(s.data + q + 2 + c * 2);
                    if (ch < 0x80)
                        utf8 += char(ch);
                    else if (ch < 0x800)
                    {
                        utf8 += char(0xC0 | ch >> 6);
                        utf8 += char(0x80 | (ch & 0x3F));
                    }
                    else
                    {
                        utf8 += char(0xE0 | ch >> 12);
                        utf8 += char(0x80 | (ch >> 6 & 0x3F));
                        utf8 += char(0x80 | (ch & 0x3F));
                    }
                }
                strings_.push_back(std::move(utf8));
                q += 2 + length * 2;
            }
        }
        // VECT, QUAT and CUST are read as version 8's (ReadPools; the other sections it reads are absent).
        for (const char* tag : { "FLOT", "COLR", "KEYP", "KEYD", "NAME" })
            if (sections_.contains(tag))
            {
                error = std::format("version 5 scene with a {} section", tag);
                return false;
            }
        // (ReadPools would read STRN as version 8's: it is set aside while the other pools are read.)
        const auto strn = sections_.find("STRN");
        const bool hasStrings = strn != sections_.end();
        const Section saved = hasStrings ? strn->second : Section{};
        if (hasStrings)
            sections_.erase(strn);
        const bool pools = ReadPools(error);
        if (hasStrings)
            sections_["STRN"] = saved;
        if (!pools)
            return false;
        const auto it = sections_.find("DATA");
        if (it == sections_.end())
        {
            error = "no DATA section";
            return false;
        }
        V5Reader r{ *this, it->second.data, it->second.size };
        if (!r.Object(root_, error, 0))
            return false;
        if (r.failed || r.at != r.size)
        {
            error = std::format("object stream: {} of {} bytes read{}", r.at, r.size, r.failed ? " (overrun)" : "");
            return false;
        }
        return true;
    }

    const FigurePath* Scene::Figure(uint32_t offset) const
    {
        const auto it = figures_.find(offset);
        return it == figures_.end() ? nullptr : &it->second;
    }

    namespace
    {
        std::string FormatValue(const Value& v)
        {
            switch (v.type)
            {
            case Type::kBool: return v.b ? "true" : "false";
            case Type::kInteger:
            case Type::kUnsigned: return std::to_string(v.i);
            case Type::kFloat: return std::format("{:.6f}", v.f);
            case Type::kString: return "\"" + v.s + "\"";
            case Type::kColor: return std::format("0x{:08x}", v.color);
            case Type::kVector: return std::format("{:.6f},{:.6f},{:.6f}", v.v.x, v.v.y, v.v.z);
            case Type::kQuaternion: return std::format("{:.6f},{:.6f},{:.6f},{:.6f}", v.q.x, v.q.y, v.q.z, v.q.w);
            case Type::kCustom: return std::format("figure@{}", v.custom);
            case Type::kObject:
            {
                std::string s = "{";
                if (v.object)
                    for (const Property& p : v.object->props)
                        for (size_t i = 0; i < p.values.size(); i++)
                            s += std::format(" {}{}={}", p.def->name, p.values.size() > 1 ? std::format("[{}]", i) : "",
                                             FormatValue(p.values[i]));
                return s + " }";
            }
            default: return "?";
            }
        }
    }

    std::string DumpNode(const Node& node, int depth)
    {
        const std::string pad(size_t(depth) * 2, ' ');
        std::string s = pad + node.className;
        for (const Property& p : node.props)
            for (size_t i = 0; i < p.values.size(); i++)
                s += std::format(" {}{}={}", p.def->name, p.values.size() > 1 ? std::format("[{}]", i) : "",
                                 FormatValue(p.values[i]));
        s += "\n";
        for (const Node& child : node.children)
            s += DumpNode(child, depth + 1);
        for (const NamedFrame& n : node.namedFrames)
            s += std::format("{}  @{} {} {}{}\n", pad, n.frame, n.name, int(n.command),
                             n.target.empty() ? "" : " -> " + n.target);
        for (const Timeline& t : node.timelines)
        {
            s += std::format("{}  timeline {}:", pad, t.target);
            for (const Track& tr : t.tracks)
                s += " " + tr.name + (tr.hasIndex ? std::format("[{}]", tr.element) : "");
            s += "\n";
            for (const KeyFrame& k : t.keys)
            {
                s += std::format("{}    {} i{}", pad, k.frame, int(k.interpolation));
                if (k.direction != EaseDirection::kIn)
                    s += std::format("d{}", int(k.direction));
                if (k.interpolation == Interpolation::kEase)
                    s += std::format("({},{},{})", k.easeIn, k.easeOut, k.easeScale);
                else if (k.p1 != 0 || k.p2 != 0)
                    s += std::format("({:g},{:g})", k.p1, k.p2);
                for (const Value& v : k.values)
                    s += " " + FormatValue(v);
                s += "\n";
            }
        }
        return s;
    }
}
