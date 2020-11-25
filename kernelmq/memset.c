void kmemset(void *const buffer, const unsigned char value, const unsigned long size)
{
    unsigned char *const s = buffer;

    for (unsigned long i = 0; i < size; ++i) {
        s[i] = value;
    }
}
