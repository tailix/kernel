#include <kernelmq/stdlib.h>

#include <assert.h>

#include <string.h>

int main()
{
    char buffer[7];

    kmemset(buffer, 0, 1);
    assert(strlen(buffer) == 0);

    kmemset(buffer,     1, 3);
    kmemset(&buffer[3], 0, 4);
    assert(strlen(buffer) == 3);

    strcpy(buffer, "qwerty");
    kmemset(&buffer[2], 'a', 2);
    assert(!strcmp(buffer, "qwaaty"));

    return 0;
}
