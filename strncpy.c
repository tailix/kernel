char *kstrncpy(char *const dest, const char *const src, const unsigned long slen)
{
    for (unsigned long i = 0; i < slen; ++i) {
        dest[i] = src[i];
    }

    dest[slen] = 0;

    return dest;
}
