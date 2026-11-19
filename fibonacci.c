
#include<stdio.h>
int main()
{
    int n, i;
    unsigned long long first = 0, second = 1, next;
    printf("Enter the number of terms: ");
    if(scanf("%d", &n) != 1 || n < 0 || n > 94)
    {
        printf("Enter an integer from 0 to 94.\n");
        return 1;
    }
    printf("Fibonacci Series: ");
    for(i = 0; i < n; i++)
    {
        if(i == 0)
            next = first;
        else if(i == 1)
            next = second;
        else
        {
            next = first + second;
            first = second;
            second = next;
        }
        printf("%llu ", next);
    }
   
    return 0;
}   