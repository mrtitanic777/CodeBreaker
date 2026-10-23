// GamerServices.cs -- the parts of XNA's GamerServices a game needs to do something (the rest are stubs generated
// from XNA's own API, GamerServices.stubs.cs): one local player signed in (not to Xbox LIVE), the full game (not a
// trial), and a Guide whose message boxes and keyboard answer at once -- the default button, the default text.
using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;

namespace Microsoft.Xna.Framework.GamerServices
{
    public abstract partial class Gamer
    {
        string gamertag = Environment.UserName;
        protected Gamer() { }
        public string Gamertag { get { return gamertag; } internal set { gamertag = value; } }
        public string DisplayName { get { return gamertag; } }
        public object Tag { get; set; }
        public bool IsDisposed { get { return false; } }
        public static SignedInGamerCollection SignedInGamers { get { return SignedInGamerCollection.Local; } }
        public LeaderboardWriter LeaderboardWriter { get { return null; } }
        public override string ToString() { return gamertag; }
        public GamerProfile GetProfile() { return null; }
        public IAsyncResult BeginGetProfile(AsyncCallback callback, object asyncState) { return XnaPort.Completed.Call(callback, asyncState); }
        public GamerProfile EndGetProfile(IAsyncResult result) { return null; }
        public static IAsyncResult BeginGetFromGamertag(string gamertag, AsyncCallback callback, object asyncState) { return XnaPort.Completed.Call(callback, asyncState); }
        public static Gamer EndGetFromGamertag(IAsyncResult result) { return null; }
        public static Gamer GetFromGamertag(string gamertag) { return null; }
        public static IAsyncResult BeginGetPartnerToken(string audienceUri, AsyncCallback callback, object asyncState) { return XnaPort.Completed.Call(callback, asyncState); }
        public static string EndGetPartnerToken(IAsyncResult result) { return null; }
        public static string GetPartnerToken(string audienceUri) { return null; }
    }

    public partial class SignedInGamer : Gamer
    {
        internal SignedInGamer(PlayerIndex index) { PlayerIndex = index; }
        protected SignedInGamer() { }
        public PlayerIndex PlayerIndex { get; private set; }
        public bool IsSignedInToLive { get { return false; } }
        public bool IsGuest { get { return false; } }
        public GameDefaults GameDefaults { get { return null; } }
        public GamerPrivileges Privileges { get { return null; } }
        public int PartySize { get { return 1; } }
        public GamerPresence Presence { get { return null; } }
#pragma warning disable 67
        public static event EventHandler<SignedInEventArgs> SignedIn;
        public static event EventHandler<SignedOutEventArgs> SignedOut;
#pragma warning restore 67
        public bool IsFriend(Gamer gamer) { return false; }
        public FriendCollection GetFriends() { return null; }
        public bool IsHeadset(Microsoft.Xna.Framework.Audio.Microphone microphone) { return false; }
        public IAsyncResult BeginAwardAchievement(string achievementKey, AsyncCallback callback, object state) { XnaPort.Runtime.Log("achievement: " + achievementKey); return XnaPort.Completed.Call(callback, state); }
        public void EndAwardAchievement(IAsyncResult result) { }
        public void AwardAchievement(string achievementKey) { XnaPort.Runtime.Log("achievement: " + achievementKey); }
        public IAsyncResult BeginGetAchievements(AsyncCallback callback, object asyncState) { return XnaPort.Completed.Call(callback, asyncState); }
        public AchievementCollection EndGetAchievements(IAsyncResult result) { return null; }
        public AchievementCollection GetAchievements() { return null; }
    }

    public partial class SignedInGamerCollection : GamerCollection<SignedInGamer>
    {
        internal static readonly SignedInGamerCollection Local = new SignedInGamerCollection(new SignedInGamer(PlayerIndex.One));
        readonly SignedInGamer one;
        SignedInGamerCollection(SignedInGamer g) : base() { one = g; Items.Add(g); }
        protected SignedInGamerCollection() { }
        public SignedInGamer this[PlayerIndex i0] { get { return i0 == PlayerIndex.One ? one : null; } }
    }

    public class GamerServicesComponent : GameComponent
    {
        public GamerServicesComponent(Game game) : base(game) { }
    }

    public static class GamerServicesDispatcher
    {
        public static bool IsInitialized { get { return true; } }
        public static IntPtr WindowHandle { get; set; }
#pragma warning disable 67
        public static event EventHandler<EventArgs> InstallingTitleUpdate;
#pragma warning restore 67
        public static void Initialize(IServiceProvider serviceProvider) { }
        public static void Update() { }
    }

    public static class Guide
    {
        sealed class Answer : XnaPort.Completed.WithValue { }

        public static bool IsScreenSaverEnabled { get; set; }
        public static bool IsVisible { get { return false; } }
        public static NotificationPosition NotificationPosition { get; set; }
        public static bool IsTrialMode { get { return false; } }
        public static bool SimulateTrialMode { get; set; }

        public static IAsyncResult BeginShowMessageBox(PlayerIndex player, string title, string text, IEnumerable<string> buttons,
                                                       int focusButton, MessageBoxIcon icon, AsyncCallback callback, object state)
        {
            XnaPort.Runtime.Log("Guide message box: " + title + ": " + text + " -> button " + focusButton);
            return XnaPort.Completed.Call(callback, state, (int?)focusButton);
        }
        public static IAsyncResult BeginShowMessageBox(string title, string text, IEnumerable<string> buttons, int focusButton,
                                                       MessageBoxIcon icon, AsyncCallback callback, object state)
            => BeginShowMessageBox(PlayerIndex.One, title, text, buttons, focusButton, icon, callback, state);
        public static int? EndShowMessageBox(IAsyncResult result) { return XnaPort.Completed.Value<int?>(result); }
        public static void ShowSignIn(int paneCount, bool onlineOnly) { }
        public static IAsyncResult BeginShowKeyboardInput(PlayerIndex player, string title, string description, string defaultText,
                                                          AsyncCallback callback, object state)
            => XnaPort.Completed.Call(callback, state, defaultText);
        public static IAsyncResult BeginShowKeyboardInput(PlayerIndex player, string title, string description, string defaultText,
                                                          AsyncCallback callback, object state, bool usePasswordMode)
            => XnaPort.Completed.Call(callback, state, defaultText);
        public static string EndShowKeyboardInput(IAsyncResult result) { return XnaPort.Completed.Value<string>(result); }
        public static void ShowMessages(PlayerIndex player) { }
        public static void ShowFriends(PlayerIndex player) { }
        public static void ShowPlayers(PlayerIndex player) { }
        public static void ShowFriendRequest(PlayerIndex player, Gamer gamer) { }
        public static void ShowPlayerReview(PlayerIndex player, Gamer gamer) { }
        public static void ShowGamerCard(PlayerIndex player, Gamer gamer) { }
        public static void ShowParty(PlayerIndex player) { }
        public static void ShowPartySessions(PlayerIndex player) { }
        public static void ShowComposeMessage(PlayerIndex player, string text, IEnumerable<Gamer> recipients) { }
        public static void DelayNotifications(TimeSpan delay) { }
        public static void ShowGameInvite(PlayerIndex player, IEnumerable<Gamer> recipients) { }
        public static void ShowGameInvite(string sessionId) { }
        public static void ShowMarketplace(PlayerIndex player) { }
    }
}

namespace XnaPort
{
    // An IAsyncResult already finished: the callback runs at once, and End* reads the value it carries.
    public class Completed : IAsyncResult
    {
        public class WithValue : Completed { }
        object state, value;
        public object AsyncState => state;
        public System.Threading.WaitHandle AsyncWaitHandle => new System.Threading.ManualResetEvent(true);
        public bool CompletedSynchronously => true;
        public bool IsCompleted => true;

        public static IAsyncResult Call(AsyncCallback callback, object state, object value = null)
        {
            var r = new Completed { state = state, value = value };
            callback?.Invoke(r);
            return r;
        }

        public static T Value<T>(IAsyncResult r) => r is Completed c && c.value is T v ? v : default(T);
    }
}
