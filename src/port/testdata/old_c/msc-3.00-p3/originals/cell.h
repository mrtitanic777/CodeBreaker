/* a cell of the object's lists: the next, and two words of what the list holds (for the communal variables'
   sizes: the size and its index) */
struct cell {
    struct cell *next;
    int size;
    int idx;
};
