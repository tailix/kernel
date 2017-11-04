void kmemset(void *const buffer, const unsigned char value, const unsigned int size)
{
    unsigned char *const s = buffer;

    for (unsigned int i = 0; i < size; ++i) {
        s[i] = value;
    }
}
