
#include "get_next_line.h"

int main(void)
{
    char *result;
    int fd;

    fd = open("text.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while((result = get_next_line(fd)))
    {
        printf("%s", result);
        free(result);
    }
    close(fd);
    return (0);
}
