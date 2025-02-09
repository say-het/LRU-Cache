/*
NAME OF THE PROJECT : FrequentlyContacted

ALGORITHM USED : LRU (Least Recently Used) Cache

CACHE : -> Stores data to speed up future requests
        -> May be a previous computaion or redundant copy of the data
        
LRU CACHE : -> Most recent element is brought to the top
            -> Element that exceeds the limit is dropped out

DATA STRUCTURES : -> Doubly Linked List
                  -> Binary Search Tree

TIME COMPLEXITY : -> Searching element in BST - O(log n)
                  -> Inserting new element in linked list - O(1)
                  -> Deleting last element in linked list - O(1)

SPACE COMPLEXITY : O(n)

APPLICATIONS : -> WhatsApp Chat history
               -> Recently used apps on smartphone
               -> Notification Panel
*/

#include <stdio.h>
#include <stdlib.h>
#include<string.h>

// MAX LENGTH OF CACHE = 5

# define max 5
int length;

// DOUBLY LINKED LIST

struct listnode
{
	char name[20];
	struct listnode* prev;
	struct listnode* next;
};

struct listnode* head;
struct listnode* tail;

void insert_list(char name[])
{
	struct listnode* temp=(struct listnode*)malloc(sizeof(struct listnode));
    strcpy(temp->name,name);
    temp->prev=NULL;
    temp->next=NULL;

    if(head==NULL)
    {
        head=temp;
        tail=temp;
    }
    else
    {
        temp->next=head;
        head->prev=temp;
        head=temp;
    }
}

void delete_last_list()
{
    struct listnode* del = tail;
    tail=tail->prev;
    tail->next=NULL;
    free(del);
}

void print(struct listnode* p)
{
	while(p!=NULL)
	{
		printf("%s\n",p->name);
		p=p->next;
	}
}

// BINARY SEARCH TREE

struct treenode
{
	char name[20];
	struct listnode* address;
	struct treenode* lchild;
	struct treenode* rchild;
};

struct treenode* root;

struct treenode* insert_bst(struct treenode* t,char name[],struct listnode* address)  
{
    if(t==NULL)
    {
        struct treenode* temp=(struct treenode*)malloc(sizeof(struct treenode));
        strcpy(temp->name,name);
        temp->address=address;
        temp->lchild=NULL;
        temp->rchild=NULL;
        t=temp;
    }
    if(strcmp(name,t->name)<0)   
    {
        t->lchild=insert_bst(t->lchild,name,address);
    }
    else if(strcmp(name,t->name)>0)  
    {
        t->rchild=insert_bst(t->rchild,name,address);
    }
    return t;
}

int height(struct treenode *t)
{
    if(t==NULL)
    return 0;
    int x, y;
    x = height(t->lchild);
    y = height(t->rchild);
    if (x > y)
        return x + 1;
    else
        return y + 1;
}

struct treenode *pred(struct treenode *t) 
{
    while (t != NULL && t->rchild != NULL)
        t = t->rchild;
    return t;
}

struct treenode *succ(struct treenode *t) 
{
    while (t != NULL && t->lchild != NULL)
        t = t->lchild;
    return t;
}

struct treenode* delete_bst(struct treenode* t,char name[]) 
{
    if (t == NULL) 
        return t;

    if (strcmp(name,t->name)>0)
        t->rchild = delete_bst(t->rchild, name);

    else if (strcmp(name,t->name)<0)
        t->lchild = delete_bst(t->lchild, name);

    else 
    {
        if (t->lchild == NULL && t->rchild == NULL)
        {
            free(t);
            t = NULL;
        }
        else if (t->lchild != NULL && t->rchild != NULL)
        {
            if (height(t->lchild) > height(t->rchild))
            {
                struct treenode* temp = pred(t->lchild); 
                strcpy(t->name,temp->name);
                t->address=temp->address;
                t->lchild = delete_bst(t->lchild, temp->name);
            }
            else
            {
                struct treenode* temp = succ(t->rchild);  
                strcpy(t->name,temp->name);
                t->address=temp->address;
                t->rchild = delete_bst(t->rchild, temp->name);
            }
        }
        else
        {
            if (t->lchild == NULL)
            {
                struct treenode *temp = t;
                t = t->rchild;
                free(temp);
            }
            else
            {
                struct treenode *temp = t;
                t = t->lchild;
                free(temp);
            }
        }
    }
    return t;
}

struct treenode* search_bst(struct treenode* t,char name[])
{
    if(t==NULL)
    return NULL;
    
    if(strcmp(t->name,name)==0)
    return t;
    else if(strcmp(t->name,name)>0)
    return search_bst(t->lchild,name);
    else 
    return search_bst(t->rchild,name);
}



int main()
{
	printf(" * * * CHAT LIST * * * \n");

	// MENU
	while(1)
	{
		int choice;
		printf("Choose an option -\n");
		printf("1. Add new contact\n");
		printf("2. Display Chat List\n");
		printf("3. Exit\n");
		printf("--> ");
        
		scanf("%d",&choice);
        printf("------------------------------------------------------\n");

		if(choice==1)
		{
			printf("Enter new contact: ");
			char name[20];
			scanf("%s",name);

			struct treenode* check=search_bst(root,name);

			if(check==NULL)
			{
				// Element is not present in the BST
				insert_list(name);
                root=insert_bst(root,name,head);
				length++;

				if(length>max)
				{
					// Delete last element
                    root=delete_bst(root,tail->name);
                    delete_last_list();
                    length--;
				}
			}

			else
			{
				// Element is already present in the BST
                struct listnode* add_node=check->address;

                if(add_node==head)
                continue;

                else if(add_node==tail)
                {
                    delete_last_list();
                    insert_list(name);
                    check->address=head;
                }

                else
                {
                    add_node->prev->next=add_node->next;
                    add_node->next->prev=add_node->prev;
                    free(add_node);
                    insert_list(name);
                    check->address=head;
                }
			}
		}

		else if(choice==2)
		{
			printf("Chat History - \n");
			print(head);
		}

		else
		break;

        printf("------------------------------------------------------\n");
	}

	return 0;
}
