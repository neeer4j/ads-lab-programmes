
#include<stdio.h>
int main()
{
    int n, i;
    unsigned long long fact = 1;
    printf("Enter a number: ");
    if(scanf("%d", &n) != 1 || n < 0 || n > 20)
        printf("Enter an integer from 0 to 20.\n");
    else
    {
        for(i = 1; i <= n; i++)
        {
            fact *= i;
        }
        printf("Factorial of %d = %llu\n", n, fact);
    }
    return 0;
}