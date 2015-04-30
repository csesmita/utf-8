#include <stdio.h>
#include <unistd.h>

#define UTF_8_MASK 0x80
#define MAX_UTF_8_BYTES 4

int main ()
{
    unsigned char c;
    unsigned char non_ascii_char_line = 0;
    char skip_bytes = 0;

    while (read(STDIN_FILENO, &c, 1) > 0 ) {
        if (c == '\n') {
            /* Dump the output on this line */
            printf("%u\n", non_ascii_char_line);
            non_ascii_char_line = 0;
            continue;
        }
        if (c & UTF_8_MASK) {
            non_ascii_char_line++;
            skip_bytes = 1;
            /* How many bytes to skip = Number of 1s set in the first nibble
             * Upto a max 4 bytes
             */
            while ((c << skip_bytes) & UTF_8_MASK) {
                if (++skip_bytes > MAX_UTF_8_BYTES) {
                    break;
                }
            }
            /* Trash the next skip_bytes */
            while (--skip_bytes > 0) {
                read(STDIN_FILENO, &c, 1);
            }
        }
    }

    return 0;
}
