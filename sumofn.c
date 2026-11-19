
#include<stdio.h>
int main()
{
    int n;
    long long sum = 0;
    printf("Enter the number of natural numbers to be summed: ");
    if(scanf("%d", &n) != 1 || n < 0 || n > 3037000499LL)
    {
        printf("Enter a non-negative integer.\n");
        return 1;
    }
    for(int i = 1; i <= n; i++)
    {
        sum =sum+i;
    }
    printf("The sum is: %lld\n", sum);
    return 0;
}