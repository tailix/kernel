void memset(void *const buffer, const unsigned char value, const unsigned int size)
{
    const unsigned char *end = buffer + size;

    for (unsigned char *p = buffer; p < end; ++p) {
        *p = value;
    }
}
