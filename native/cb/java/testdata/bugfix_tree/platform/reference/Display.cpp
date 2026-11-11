void f() {
    // FIXED BUG display-flip
    // The flip must happen before Present() | else tearing
    g();   // J2X-BUGFIX with-pipe | pipe in the middle | and more
}
// J2X-BUGFIX marker is a meta reference and is skipped
// FIXED BUG markers neither
// J2X-BUGFIX
// FIXED BUG    spaced-out-slug   ***/
