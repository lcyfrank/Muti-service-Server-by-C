#include <stdio.h>
#include <string.h>

const char *reverse(const char *str);

int main(int argc, char **argv)
{
    char *str;
    switch (argc) {
        case 1:
            str = "Hello";
            break;
        case 2:
            str = argv[1];
            break;
        default:
            break;
    }
    
    printf("%s\n", reverse(str));
    return 0;
}

const char *reverse(const char *str)
{
    int length = strlen(str);
    char *result = (char *)malloc(sizeof(char) * (length + 1));
    int i;
    int j;
    for (i = length - 1, j = 0; i >= 0; i--, j++) {
        result[j] = str[i];
    }
    result[length] = '\0';
    return result;
}
