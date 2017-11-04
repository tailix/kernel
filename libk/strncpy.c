char *kstrncpy(char *const dest, const char *const src, const unsigned long length)
{
    for (unsigned long i = 0; i < length; ++i) {
        dest[i] = src[i];
    }

    dest[length] = 0;

    return dest;
}
