#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int Data;
    struct Node *Link;
};

struct Node *start = NULL;




void create()
{
    int n, i;
    struct Node *newnode, *temp;

    printf("Enter the number of nodes : ");
    if (scanf("%d", &n) != 1 || n < 0)
    {
        printf("Invalid number of nodes.\n");
        return;
    }

    for(i = 1; i <= n; i++)
    {
 newnode = (struct Node *)malloc(sizeof(struct Node));
 if(newnode == NULL)
 {
     printf("Unable to allocate memory.\n");
     return;
 }

 printf("Enter the data : ");
 if(scanf("%d", &newnode->Data) != 1)
 {
     printf("Invalid data.\n");
     free(newnode);
     return;
 }

  newnode->Link = NULL;

  if(start == NULL)
     start = newnode;
        else
        { temp = start;
     while(temp->Link != NULL)
     temp = temp->Link;

 temp->Link = newnode;
        }
    }
}




void insert_at_beg()
{
 struct Node *newnode;

newnode = (struct Node *)malloc(sizeof(struct Node));
 if(newnode == NULL)
 {
     printf("Unable to allocate memory.\n");
     return;
 }

  printf("Enter the data : ");
   if(scanf("%d", &newnode->Data) != 1)
   {
       printf("Invalid data.\n");
       free(newnode);
       return;
   }

 newnode->Link = NULL;

    if(start == NULL)
        start = newnode;
    else
    {
    newnode->Link = start;
     start = newnode;
    }
}



void insert_at_end()
{
 struct Node *newnode, *trav;

newnode = (struct Node *)malloc(sizeof(struct Node));
 if(newnode == NULL)
 {
     printf("Unable to allocate memory.\n");
     return;
 }

printf("Enter the data : ");
    if(scanf("%d", &newnode->Data) != 1)
    {
        printf("Invalid data.\n");
        free(newnode);
        return;
    }

 newnode->Link = NULL;

    if(start == NULL)
        start = newnode;
    else
    {
     trav = start;

     while(trav->Link != NULL)
        trav = trav->Link;

     trav->Link = newnode;
    }
}




void insert_mid()
{
 int pos, i;
 struct Node *new_node, *temp, *temp1;

 new_node = (struct Node *)malloc(sizeof(struct Node));

printf("Enter the data : ");
    scanf("%d", &new_node->Data);

    new_node->Link = NULL;

 printf("Enter the position : ");
    if(scanf("%d", &pos) != 1 || pos < 1)
    {
            printf("Invalid position\n");
            free(new_node);
            return;
    }

    if(pos == 1)
    {
        new_node->Link = start;
        start = new_node;
        return;
    }

    temp = start;

for(i = 1; i < pos - 1 && temp != NULL; i++)
        temp = temp->Link;

    if(temp == NULL)
    {
     printf("Invalid position\n");
     free(new_node);
        return;
    }

 temp1 = temp->Link;

 new_node->Link = temp1;
 temp->Link = new_node;
}




void del_beg()
{
    struct Node *temp;

    if(start == NULL)
 {
        printf("List is empty.\n");
        return;
 }

 temp = start;

 start = start->Link;

 free(temp);

    printf("The Element deleted Successfully\n");
}



void delete_at_end()
{
 struct Node *temp, *cur = start;

    if(start == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    if(start->Link == NULL)
    {
     free(start);
     start = NULL;

     printf("The Element deleted Successfully\n");
        return;
    }

    while(cur->Link->Link != NULL)
        cur = cur->Link;

    temp = cur->Link;

    cur->Link = NULL;

    free(temp);

    printf("The Element deleted Successfully\n");
}

void delete_Specific_Node()
{
    int pos, i = 1;
    struct Node *dnode, *temp = start;

    if(start == NULL)
    {
        printf("List is empty.\n");
  return;
    }

 printf("Enter the position to delete : ");
    if(scanf("%d", &pos) != 1 || pos < 1)
    {
        printf("Invalid position\n");
        return;
    }

    if(pos == 1)
    {
        del_beg();
        return;
    }

  while(i < pos - 1 && temp->Link != NULL)
    {
        temp = temp->Link;
        i++;
    }

    if(temp->Link == NULL)
    {
        printf("Invalid position\n");
        return;
    }

    dnode = temp->Link;
    temp->Link = dnode->Link;
    dnode->Link = NULL;
    free(dnode);
    printf("The Element deleted Successfully\n");
}

void search()
{
    int item, pos = 1;
    struct Node *temp = start;

    printf("Enter the element to search : ");
    scanf("%d", &item);
    while(temp != NULL)
    {
        if(temp->Data == item)
        {
            printf("Element found at position %d\n", pos);
            return;
        }
        temp = temp->Link;
        pos++;
    }

    printf("Element not found\n");
}

void Display_List()
{
    struct Node *temp = start;

    if(temp == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    while(temp != NULL)
    {
        printf("%d ", temp->Data);
        temp = temp->Link;
    }

    printf("\n");
}

void count()
{
    int cnt = 0;
    struct Node *temp = start;

    while(temp != NULL)
    {
        cnt++;
        temp = temp->Link;
    }

    printf("Number of nodes = %d\n", cnt);
}

void reverse()
{
    struct Node *prev = NULL;
    struct Node *cur = start;
    struct Node *next = NULL;

    while(cur != NULL)
    {
        next = cur->Link;
        cur->Link = prev;
        prev = cur;
        cur = next;
    }

    start = prev;

    printf("List reversed successfully\n");
}

void sort()
{
    struct Node *temp, *temp1;
    int t;

    temp = start;

    while(temp != NULL)
    {
        temp1 = temp->Link;

        while(temp1 != NULL)
        {
            if(temp->Data > temp1->Data)
            {
                t = temp->Data;
               temp->Data = temp1->Data;
                temp1->Data = t;
            }

            temp1 = temp1->Link;
        }

        temp = temp->Link;
    }

    printf("List sorted successfully\n");
}


int main()
{
    int choice;

    while(1)
    {

        printf("1. Create Linked List\n");
        printf("2. Insert at Beginning\n");
        printf("3. Insert at End\n");
        printf("4. Insert at Specific Position\n");
        printf("5. Delete from Beginning\n");
        printf("6. Delete from End\n");
        printf("7. Delete from Specific Position\n");
        printf("8. Search for an Element\n");
        printf("9. Display List\n");
        printf("10. Count Number of Nodes\n");
        printf("11. Reverse Linked List\n");
        printf("12. Sort Linked List\n");
        printf("13. Exit\n");

        printf("\nEnter your choice : ");
        scanf("%d", &choice);

 switch(choice)
        {
 case 1:
          create();
                break;

 case 2:
           insert_at_beg();
                break;

 case 3:
         insert_at_end();
                break;

   case 4:
             insert_mid();
                break;

  case 5:
           del_beg();
                break;

  case 6:
           delete_at_end();
                break;

 case 7:
         delete_Specific_Node();
                break;

 case 8:
        search();
                break;

 case 9:
           Display_List();
                break;

    case 10:
         count();
                break;

 case 11:
          reverse();
                break;

 case 12:
         sort();
                break;

  case 13:
          exit(0);

      default:
           printf("Invalid choice\n");
        }
    }

    return 0;
}