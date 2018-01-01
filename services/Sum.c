#include <stdio.h>
#include <string.h>

int sum(int n);

int main(int argc, char **argv)
{
    int n = 0;
    switch (argc) {
        case 1:
            n = 10;
            break;
        case 2:
            n = atoi(argv[1]);
            break;
        default:
            break;
    }
    printf("%d\n", sum(n));
}

int sum(int n)
{
    if (n == 1) return 1;
    else return n * n + sum(n - 1);
}
