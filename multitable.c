
#include<stdio.h>
int main()
{
    int n, i, m;
    long long product;
    printf("Enter a number: ");
    if(scanf("%d", &n) != 1)
    {
        printf("Invalid number.\n");
        return 1;
    }
    printf("upto: ");
    if(scanf("%d", &m) != 1 || m < 0)
    {
        printf("Enter a non-negative limit.\n");
        return 1;
    }
    printf("Multiplication table of %d:\n", n);
    for(i = 1; i <= m; i++)
    {
        product = (long long)n * i;
        printf("%d x %d = %lld\n", n, i, product);
    }
    return 0;
}