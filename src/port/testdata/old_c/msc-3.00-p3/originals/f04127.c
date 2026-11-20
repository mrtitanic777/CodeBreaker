extern f040fb();

/* the word w put in the record */
f04127(w)
register w;
{
    f040fb((char)w);
    f040fb((char)(w >> 8));
}
