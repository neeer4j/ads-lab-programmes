#include <stdio.h>
int main(void)
{
    int a[50], b[50], c[100], i, j, k, n1, n2, temp;
    printf("Enter the number of elements for array 1: ");
  if (scanf("%d", &n1) != 1 || n1 < 0 || n1 > 50) {
    printf("Array size must be between 0 and 50.\n");
    return 1;
  }
    printf("Enter the elements for array 1: ");
    for(i=0;i<n1;i++)
    {
    if (scanf("%d", &a[i]) != 1) {
      printf("Invalid array element.\n");
      return 1;
    }
    }
    printf("Enter the number of elements for array 2: ");
  if (scanf("%d", &n2) != 1 || n2 < 0 || n2 > 50) {
    printf("Array size must be between 0 and 50.\n");
    return 1;
  }
    printf("Enter the elements of array 2: ");
    for(i=0;i<n2;i++)
    {
    if (scanf("%d", &b[i]) != 1) {
      printf("Invalid array element.\n");
      return 1;
    }
    }
    for(i=0; i<n1-1; i++)
    {
     for(j=0; j<n1-i-1; j++)
        {
            if(a[j] > a[j+1])
            {
     temp = a[j];
      a[j] = a[j+1];
          a[j+1] = temp;
     }
     }
    }

    for(i=0; i<n2-1; i++)
    {
        for(j=0; j<n2-i-1; j++)
  {
     if(b[j] > b[j+1])
            {
     temp = b[j];
     b[j] = b[j+1];
      b[j+1] = temp;
     }  }
    }

    i = 0;
    j = 0;
    k = 0;
    while(i < n1 && j < n2)
    {
  if(a[i] <= b[j])
        {
     c[k++] = a[i++];
        }
  else
        {
      c[k++] = b[j++];
        }
    }
 while(i < n1)
    {
        c[k++] = a[i++];
    }

  while(j < n2)
    {
      c[k++] = b[j++];
    }

 printf("Merged sorted array: ");
    for(i=0; i<k; i++)
    {
        printf("%d ", c[i]);
    }
      printf("\n");
    return 0;
}



