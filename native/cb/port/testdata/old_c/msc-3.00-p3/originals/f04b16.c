extern f048bd(), f040fb(), f0418c(), strlen();

/* a COMENT record of the class cls, the text s */
f04b16(cls, s)
char cls;
char *s;
{
    f048bd(0x88, 0);
    f040fb(0);
    f040fb((unsigned char)cls);
    f0418c(s, strlen(s));
}
