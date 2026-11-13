// Input.cs -- the Xbox 360 controller a game reads, on a PC: the real one when there is one, the keyboard as the
// first player's controller, and presses a run is scripted with. The port step makes the game's GamePad.GetState
// calls XnaPort.Input.GetState.
//
//   keyboard   Enter / Space = A, Backspace = B, X = X, Y = Y, Tab = Start, Escape = Back, arrows = the D-pad,
//              W A S D = the left stick, I J K L = the right stick, Q / E = the shoulders, F / R = the triggers,
//              C / V = the stick clicks
//   XNAPORT_PRESS=<seconds>:<button>[:<held seconds>],...   presses from the start of the run, for checking a
//              game past its menus without a person at it, e.g. "8:A,10:A,14:Start" (a press is held 0.2 s)
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace XnaPort
{
    public static class Input
    {
        static readonly Stopwatch clock = Stopwatch.StartNew();
        static readonly List<(double at, double until, Buttons button)> script = Script();

        static List<(double, double, Buttons)> Script()
        {
            var list = new List<(double, double, Buttons)>();
            string text = Environment.GetEnvironmentVariable("XNAPORT_PRESS");
            if (string.IsNullOrWhiteSpace(text))
                return list;
            foreach (string item in text.Split(','))
            {
                string[] parts = item.Trim().Split(':');
                if (parts.Length < 2 || !double.TryParse(parts[0], NumberStyles.Float, CultureInfo.InvariantCulture, out double at) ||
                    !Enum.TryParse(parts[1], true, out Buttons button))
                {
                    Runtime.Log("XnaPort: XNAPORT_PRESS: cannot read '" + item + "'");
                    continue;
                }
                double held = 0.2;
                if (parts.Length > 2)
                    double.TryParse(parts[2], NumberStyles.Float, CultureInfo.InvariantCulture, out held);
                list.Add((at, at + held, button));
            }
            return list;
        }

        public static GamePadState GetState(PlayerIndex player) => GetState(player, GamePadDeadZone.IndependentAxes);

        public static GamePadState GetState(PlayerIndex player, GamePadDeadZone deadZone)
        {
            GamePadState pad = GamePad.GetState(player, deadZone);
            if (player != PlayerIndex.One)
                return pad;
            var buttons = new List<Buttons>();
            foreach (Buttons b in Enum.GetValues(typeof(Buttons)))
                if (b != 0 && IsSingle(b) && pad.IsButtonDown(b) && !IsStickDirection(b))
                    buttons.Add(b);
            Vector2 left = pad.ThumbSticks.Left, right = pad.ThumbSticks.Right;
            float leftTrigger = pad.Triggers.Left, rightTrigger = pad.Triggers.Right;

            KeyboardState k = Keyboard.GetState();
            void key(Keys key, Buttons b) { if (k.IsKeyDown(key) && !buttons.Contains(b)) buttons.Add(b); }
            key(Keys.Enter, Buttons.A); key(Keys.Space, Buttons.A); key(Keys.Back, Buttons.B); key(Keys.X, Buttons.X);
            key(Keys.Y, Buttons.Y); key(Keys.Tab, Buttons.Start); key(Keys.Escape, Buttons.Back);
            key(Keys.Up, Buttons.DPadUp); key(Keys.Down, Buttons.DPadDown); key(Keys.Left, Buttons.DPadLeft);
            key(Keys.Right, Buttons.DPadRight); key(Keys.Q, Buttons.LeftShoulder); key(Keys.E, Buttons.RightShoulder);
            key(Keys.C, Buttons.LeftStick); key(Keys.V, Buttons.RightStick);
            Vector2 stick(Keys up, Keys down, Keys l, Keys r) =>
                new Vector2((k.IsKeyDown(r) ? 1 : 0) - (k.IsKeyDown(l) ? 1 : 0), (k.IsKeyDown(up) ? 1 : 0) - (k.IsKeyDown(down) ? 1 : 0));
            Vector2 keysLeft = stick(Keys.W, Keys.S, Keys.A, Keys.D), keysRight = stick(Keys.I, Keys.K, Keys.J, Keys.L);
            if (keysLeft != Vector2.Zero) left = keysLeft;
            if (keysRight != Vector2.Zero) right = keysRight;
            if (k.IsKeyDown(Keys.F)) leftTrigger = 1;
            if (k.IsKeyDown(Keys.R)) rightTrigger = 1;

            double now = clock.Elapsed.TotalSeconds;
            foreach (var (at, until, button) in script)
                if (now >= at && now < until && !buttons.Contains(button))
                    buttons.Add(button);
            return new GamePadState(left, right, leftTrigger, rightTrigger, buttons.ToArray());
        }

        static bool IsSingle(Buttons b) => ((int)b & ((int)b - 1)) == 0;

        // the sticks' directions are buttons too; GamePadState works them out from the sticks
        static bool IsStickDirection(Buttons b) => b.ToString().Contains("Thumbstick");
    }
}
