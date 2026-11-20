/* (a symbol's flags word as fields: its kind, class, frame) */
struct sflags {
    unsigned lo:4;
    unsigned cls:4;
    unsigned frame:4;
    unsigned hi:4;
};
