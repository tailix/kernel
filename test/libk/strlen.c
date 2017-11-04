#include <kernelmq/stdlib.h>

#include <assert.h>

int main()
{
    assert(kstrlen("") == 0);
    assert(kstrlen("q") == 1);
    assert(kstrlen("qwe rty") == 7);
    assert(kstrlen("qwe\0rty") == 3);

    return 0;
}
