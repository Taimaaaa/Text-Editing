//Taymaa Nasser  1222640  Section 5.
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

//type defining pointers of my structs for easier use
typedef struct Stacks* SPtr;
typedef SPtr MyStack;
typedef struct Queue* QPtr;
typedef QPtr MyQueue;
typedef struct OriginalStr* ptr;
typedef ptr OgString;


typedef struct Stacks //struct of undo and redo stacks
{
    char str[300];
    int index;
    int operation; //1 for insert and 0 for remove
    SPtr next;
} Stacks;


typedef struct Queue //struct of queue
{
    char str[300];
    QPtr next;
    QPtr last; //pointer that points to the last element in queue
} Queue;

typedef struct OriginalStr //struct for doubly linked list of the string we enter and modify
{
    char word [300];
    int index;
    ptr next;
    ptr last; //pointer that points to the last word in the string.
} OriginalStr;

//declaring all functions used in project
void displayMenu();
void loadStringFile(ptr head,int op);
void saveDataFile(ptr head);
int isExist(ptr head, char key[]);
ptr createNode_String();
void Insert_StringAtLast(ptr head, char NewWord[],int indexed);
ptr SearchKey(ptr head, char key[]);
int indexing(ptr head, char indexed_word[]);
void Insert(QPtr front, SPtr undo,ptr head,char newStr[], int place);
void Delete(SPtr undo,ptr head,char keyStr[]);
void Undoing(SPtr undo,SPtr redo,ptr head);
void Redoing(SPtr redo,SPtr undo,ptr head);
SPtr createNode_Stack();
QPtr createNode_Queue();
void push (SPtr head, char newStr[],int op,int indexed);
void pop(SPtr head);
void DisplayStack(SPtr head);
void enqueue(QPtr head, char newStr []);
QPtr dequeue(QPtr head);

int main()
{
    MyStack Undo=createNode_Stack(); //creating head node for undo stack linked list
    MyStack Redo=createNode_Stack(); //creating head node for redo stack linked list
    MyQueue InQueue=createNode_Queue(); //creating head node for queue linked list
    InQueue->last=InQueue; //initializing the last pointer in the queue LL to first point at the head itself
    OgString OS=createNode_String(); //creating head node for strings of words linked list.

    char newStr[300],key[300],deleteStr[30]; //declaring variables to store user-entered data in menu
    int place; //option for user to select where insertion of string takes place.
    int loadFlag=0; //initializing flag to prevent loading of txt file more than once.
    int func=0; //initializing flag to enter and end menu.(when func==9, exit menu).

    while(func!=9)
    {
        displayMenu();
        scanf("%d",&func);
        getchar(); //to deal with special characters entered by user.
        switch(func)
        {
        case 0:
            DisplayString(OS);
            break;
        case 1:
            if(loadFlag==0)
            {
                loadStringFile(OS,1);
                loadFlag=1;
            }
            else
                printf("You can not load the file more than once. Please select another operation. \n");
            break;
        case 2:
            if(loadFlag!=1) //forcing user to load file before printing it using the loadFlag.
            {
                printf("You must load file before printing it. Please select operation 1.\n");
            }
            else
                loadStringFile(OS,2);
            break;
        case 3:
            printf("Please provide the new string to insert:\n");
            gets(newStr);
            printf("Specify where do you want the insertion to take place in the original string: \n");
            printf(" 1-At the beginning.\n 2-In the middle.\n 3-At the end.\n");
            scanf("%d",&place);
            Insert(InQueue,Undo,OS,newStr,place);
            break;
        case 4:
            printf("Please insert word you want to delete from string:\n");
            gets(deleteStr);
            printf("Deleting %s...\n",deleteStr);
            Delete(Undo,OS,deleteStr);
            break;
        case 5:
            printf("Undoing last modification...\n");
            Undoing(Undo,Redo,OS);
            printf("Undo successful!\n");
            break;
        case 6:
            printf("Redo last modification...\n");
            Redoing(Redo,Undo,OS);
            printf("Redo successful!\n");
            break;
        case 7:
            printf("Printing Undo stack...\n");
            DisplayStack(Undo);
            printf("\n 1Printing Redo stack...\n");
            DisplayStack(Redo);
            break;
        case 8:
            saveDataFile(OS);
            break;
        case 9:
            printf("Thank you for using my text editor with differential undo/redo system! Goodbye.\n"); //exiting system
            break;
        default:
            printf("No such operation! Please try again.\n"); //deals with user-entered data that is not a number from 1-9
        }
    }
    return 0;
}


void displayMenu() //function to display main menu
{
    printf("Please select one functionality to run (1-9):\n\n");
    printf("1. Load the input file.\n");
    printf("2. Print the loaded text.\n");
    printf("3. Insert strings to the text.\n");
    printf("4. Remove strings from the text.\n");
    printf("5. Perform Undo operation.\n");
    printf("6. Perform Redo operation.\n");
    printf("7. Print the Undo Stack and the Redo stack.\n");
    printf("8. Save the updated text to the output file.\n");
    printf("9. Exit System.\n");
}
void loadStringFile(ptr head,int op) //function that loads input file Original_String.txt and prints loaded info
{
    FILE *fp; //declaring a file pointer fp
    fp = fopen("Original_String.txt","r"); //using fp to open Original_String.txt file in read mode
    if (fp == NULL)
    {
        printf("Input file failed to load!\n");
        return;
    }
    char FullString[300]; //declaring a variable to contain the tokenized string read from file
    char* token;
    while(!feof(fp)) //read lines until end of file
    {
        fgets(FullString,300,fp); //read until end of line.
        char * token = strtok(FullString, " "); //tokenise the full string after every space " " char
        while(token != NULL )
        {
            strcpy(FullString,token); //save that token in variable FullString.
            token = strtok(NULL, " ");//tokenise the leftover string after space char.
            int E=isExist(head,FullString); //calling function to check if current word exits to prevent duplicate stringsf.
            if(E==0) //prevent from loading input sentence again.
            {

                Insert_StringAtLast(head,FullString,0);
                 int indexed=indexing(head,FullString); //giving each word inserted an index based on first char position
                 head->last->index = indexed;
            } //inserting the current word into the LL

            if(op==2) //printing loaded information if op entered was 2
            {
                printf("%s ",FullString);
            }

        }
        printf("\n");
    }
    if(op!=2)
    {
        printf("Loading data input file ...\n");
        printf("Data file Loaded!\n\n");
    }
    fclose(fp); //closing the file
}
void saveDataFile(ptr head) //function that saves all changes to output file Final_String.txt
{
    printf("Updating output data file ...\n");
    FILE *fp; //declaring a file pointer fp.
    fp = fopen("Final_String.txt","w"); //using fp to open Final_String.txt file in write mode
    if (fp == NULL)
    {
        printf("Output file failed to load! \n");
    }

    ptr temp = head->next; //traversing all word nodes in LL
    while (temp != NULL)
    {
        fprintf(fp,"%s ",temp->word);
        temp = temp->next; //moving to the next word after printing the current one into the output file.
    }
    fprintf(fp,"\n");
    fclose(fp); //closing the file.

    printf("Data file saved!\n\n");
}
ptr createNode_String()  //creates new node of a word for insertion
{
    ptr head=(ptr)malloc(sizeof(struct OriginalStr));
    head->next= NULL;
    head->last=head; //initializing the last pointer to point at the head at creation
    head->index=0;
    if(head == NULL)
    {
        printf("Memory can not be allocated.\n");
        return NULL; //return NULL pointer in case of memory allocation failure
    }
    return head;
}
int isExist(ptr head, char key[]) //checks if specific word node exits in LL.
{
    ptr p;
    int flag=0;
    p= head->next;
    while(p!=NULL)
    {
        if(strcmp(p->word,key)==0)
        {
            flag=1;
        }
        p=p->next;
    }
    return flag; //returns 0 if it does not exist.
}

void Insert_StringAtLast(ptr head, char NewWord[],int indexed)  //inserts word at end of String LL.
{
    ptr temp; //declaring a word pointer (temp points to new node to be created)
    temp = createNode_String(); //creating and allocating memory for the new word node
    strcpy(temp->word,NewWord);
    head->index=indexed;
    head->last->next = temp;//updating the next of the current last node to the new node
    head->last = temp; //updating the last of the previous last node to the new node inserted
    temp->next = NULL; //updating the next of the final node to null
}

ptr SearchKey(ptr head, char key[]) //searches for word node and returns it
{
    ptr temp = head->next; //declaring pointer temp that points to the first word in string LL
    while (temp != NULL) //traverse LL using temp until key is found
    {
        if (strcmp(temp->word,key)==0) //if the word was located in LL
        {
            return temp; //return node that carries key
        }
        temp = temp->next;

    }
    return NULL; //return null if key was not found
}
int indexing(ptr head, char indexed_word[]) //function that gives every word in the list an index based on its first character position in string
{
    int index=0; //initializing variable to zero
    ptr temp=head->next; //declaring pointer temp that points to the first word in string LL
    while(temp!=NULL)  //traverse LL using temp until key is found
    {
        if(strcmp(temp->word,indexed_word)==0) //if the word was located in LL
        {
            return index; //return the index of the first letter in the word
        }
        index+=strlen(temp->word)+1; //counts length of characters in every word before the indexed word and counting for the spaces between each node
        temp=temp->next;
    }
    return -2; //word is not located in string

}
void Insert(QPtr front, SPtr undo,ptr head,char newStr[], int place) //function to insert new string from user anywhere in the original text
{
    //declaring variables
    char dequeuedWord[30];  //latest word dequeued from the queue
    char key [300]; //word in original string that user wants to insert AFTER it
    int index; //indexing every new word inserted
    char *token=strtok(newStr," "); //for tokenising the user-entered string into words to enqueue them separately and in order
    while (token != NULL)
    {
        enqueue(front, token); //enqueue current tokenised word
        token = strtok(NULL, " "); // Get next word from user string
    }

    if(place==1)   //if user wanted insertion to take place before original string
    {
        OgString NewString=createNode_String(); //creating a head for a new linked list for the new string (acts as temp)
        while(front != NULL && front->next!=NULL) //as long as the queue is not empty
        {
            strcpy(dequeuedWord,dequeue(front)->str); //fill dequeuedWord with the str from the current front of queue
            index=indexing(NewString,dequeuedWord); //calculating index of current word
            Insert_StringAtLast (NewString,dequeuedWord,index); //make and insert new node for the dequeued word into the newstring LL
            push(undo,dequeuedWord,1,index); //pushing the word and its operation and index to the undo stack
            free(dequeuedWord); //freeing space allocated for the current dequeued word
        }
        NewString->last->next = head->next; //assigning the next pointer of last word of the temp string LL to point at the first word in the original string
        head->next = NewString->next; //assigning the next of the head pointer to point at the first word in the temp string LL
        free (NewString); //freeing space allocated for temp NewString LL
    }

    else if (place==2)  //if user wanted insertion to take place somewhere specific inside original string
    {
        printf("After what word in the original sentence:\n");
        scanf("%s",key);
        ptr Searched=SearchKey(head,key); //assigning pointer to the key word in the original linked list
        if(Searched!=NULL) //if the key was found
        {
            OgString NewString=createNode_String(); //creating a head for a new linked list for the new string (acts as temp)
            int insertionIndex = indexing(head, Searched->word); //calculating index of searched word (length of string before the searched word)
            int shift = strlen(Searched->word) + 1; //calculates length of searched word and the space after it
            while(front != NULL && front->next!=NULL) //as long as queue is not empty
            {
                strcpy(dequeuedWord,dequeue(front)->str);
                index = insertionIndex + shift; //updates index to sum of entire string up to and including the searched word
                shift += strlen(dequeuedWord) + 1; //updates with every new word inserted and the space following it
                Insert_StringAtLast (NewString,dequeuedWord,index);
                push(undo,dequeuedWord,1,index);  //pushing the word and its operation and index to the undo stack
                free(dequeuedWord);
            }
            NewString->last->next = Searched->next;  //assigning the next pointer of last word of the temp string LL to point at the node after the one that has the key word
            Searched->next = NewString->next; //assigning the next of the key word node pointer to point at the first word in the temp string LL
            free (NewString);

        }
        else
        {
            printf("%s is not in the string.\n",key);
        }
    }


    else if(place==3) //if user wanted insertion to take place after original string
    {
        while(front != NULL && front->next!=NULL) //as long as queue is not empty
        {
            strcpy(dequeuedWord,dequeue(front)->str);
            Insert_StringAtLast(head,dequeuedWord,-1); //simply insert dequeued word at the end of the original LL
            push(undo,dequeuedWord,1,-1); //pushing the word and its operation and index to the undo stack (index==-1 as it was inserted at end of string)
            free(dequeuedWord);
        }
    }
    else
    {
        printf("invalid option\n"); //user entered a number other than 1-3
    }

}

void Delete(SPtr undo,ptr head,char keyStr[]) //function to delete specific word from string
{
    ptr temp,d; //assinging ptr temp to hold the word we want to delete and d to traverse the LL
    int index;
    int flag=0; //flag used to indicate key word was found or not
    if(head!=NULL && head->next!=NULL) //as long as the LL is not empty
    {
        index=indexing(head,keyStr); //indexing the key word to store it in undo stack
        d = head;
        while(d->next!= NULL) //traversing LL
        {
            if(strcmp(d->next->word,keyStr)==0) //if the key word was found
            {
                temp = d->next; //assign temp to point to the key word node that we want to delete
                d->next = d->next->next; //skip over the deleted node and connect the one before it with the one after it
                push(undo,temp->word,0,index); //push to undo stack
                free(temp); //free the node that had the word
                flag=1; //raise flag to indicate word was found and deleted
                printf("%s has been deleted!\n",keyStr);
                break; //leave loop
            }
            else
            {
                d = d->next;
            }

        }
        if(flag==0)
        {
            printf("word %s is not in linked list \n",keyStr);
        }
    }
}
void Undoing(SPtr undo,SPtr redo,ptr head)
{
    if (undo == NULL || undo->next == NULL) //checks if there exists any words in stack.
    {
        printf("Undo stack is empty.\n");
        return;
    }
    else
    {
        //declaring variables to hold data
        char cpyString[30];
        int op,indexed;
        strcpy(cpyString,undo->next->str);
        op=undo->next->operation;
        indexed=undo->next->index;
        if(op==1)  //if the top of the undo stack was a word inserted anywhere in the string
        {
            ptr temp=head; //let temp start at head
            ptr deleted=SearchKey(head,cpyString); //assign pointer to node that was last inserted
            while(temp!=NULL) //traverse
            {
                if(temp==deleted || temp->next==deleted)
                {
                    temp->next=deleted->next; //set the next pointer of the node before the last inserted node to the one after it
                    free(deleted); //free deleted node
                    break; //leave loop
                }
                temp=temp->next;
            }


        }
        else  //if the top of the stack was a word removed from string
        {
            ptr inserted=createNode_String(); //creates new node for reinserting the removed word
            strcpy(inserted->word,cpyString);
            ptr temp=head->next; //assigning pointer to the first word in string
            while(temp!=NULL) //traverse
            {
                if(temp->index <= undo->index && temp->next->index >= undo->index) //condition to check for index of removed word and where it should fit in the string LL
                {   ptr temp2 = createNode_String();
                    strcpy(temp2->word, undo->next->str);
                    temp2->index = undo->next->index;
                    temp2->next=temp->next; //setting next of inserted node to point to the node after temp
                    temp->next=temp2; //setting next of temp to point to inserted node
                    break;
                }
                temp=temp->next;
            }

        }
        push(redo,cpyString,op,indexed); //push top element to redo stack
        pop(undo); //pop top element from undo stack

    }
}
void Redoing(SPtr redo,SPtr undo,ptr head)
{
    if (redo == NULL || redo->next == NULL) //checks if there exists any words in stack.
    {
        printf("Undo stack is empty.\n");
        return;
    }
    else
    {
        //declaring variables to hold data
        char cpyString[30];
        int op,indexed;
        strcpy(cpyString,redo->next->str);
        op=redo->next->operation;
        indexed=redo->next->index;
        if(op==0)  //if the top of the redo stack was a word deleted anywhere in the string
        {
            ptr temp=head; //let temp start at head
            ptr deleted=SearchKey(head,cpyString); //assign pointer to node that was last inserted
            while(temp!=NULL) //traverse
            {
                if(temp==deleted || temp->next==deleted)
                {
                    temp->next=deleted->next; //set the next pointer of the node before the last inserted node to the one after it
                    free(deleted); //free deleted node
                    break; //leave loop
                }
                temp=temp->next;
            }


        }
        else  //if the top of the redo stack was a word inserted to the string
        {
             ptr inserted=createNode_String(); //creates new node for reinserting the removed word
            strcpy(inserted->word,cpyString);
            ptr temp=head->next; //assigning pointer to the first word in string
            while(temp!=NULL) //traverse
            {
                if(temp->index <= undo->index && temp->next->index >= undo->index) //condition to check for index of removed word and where it should fit in the string LL
                {    ptr temp2 = createNode_String();
                     strcpy(temp2->word, undo->next->str);
                     temp2->index = undo->next->index;
                    temp2->next=temp->next; //setting next of inserted node to point to the node after temp
                    temp->next=temp2; //setting next of temp to point to inserted node
                    break;
                }
                temp=temp->next;
            }

        }
        push(undo,cpyString,op,indexed); //push top element to undo stack
        pop(redo);//pop top element of redo stack

    }


}

SPtr createNode_Stack()  //creates new Stack node for insertion
{
    SPtr head=(SPtr)malloc(sizeof(struct Stacks));
    if(head == NULL)
    {
        printf("Memory can not be allocated.\n");
        return NULL; //return NULL pointer in case of memory allocation failure
    }
    head->next= NULL;
    head->operation=0;
    head->index=0;
    return head;
}
QPtr createNode_Queue()  //creates new Stack node for insertion.
{
    QPtr head=(QPtr)malloc(sizeof(struct Queue));
    if(head == NULL)
    {
        printf("Memory can not be allocated.\n");
        return NULL; //return NULL pointer in case of memory allocation failure.
    }
    head->next= NULL;
    return head;
}
void push (SPtr head, char newStr[],int op,int indexed) //function to push on top of stacks
{
    if (head!=NULL) //if the head node for the stack was created
    {
        SPtr temp=createNode_Stack(); //creating new node to push into stack
        if (temp != NULL)
        {
            strcpy(temp->str,newStr);
            temp->operation=op;
            temp->index=indexed;
            temp->next=head->next; //inserting new node immediately after the head pointer and assinging its next ptr to whatever the head was pointing to
            head->next=temp; //making next of head point to the new pushed node
        }
        else
        {
            printf("Memory allocation failed. Cannot push.\n");
        }
    }
    else
    {
        printf("LinkedList of this stack does not exist.");
    }

}

void pop(SPtr head) //function to pop the top of stack
{
    if(head!=NULL && head->next!=NULL) //if head node was created for stack and there is at least one element in stack to pop
    {
        SPtr temp=head->next; //pointing to element on top of stack
        head->next=temp->next; //making the second to top element of stack the new top
        free(temp); //freeing the top of stack
    }
    else
    {
        printf("Stack is empty. There is nothing to pop.");
    }
}

void DisplayStack(SPtr head) //function that prints all the contents of a stack by pushing top of stack to temp stack then popping it
{

    if (head == NULL && head->next == NULL) //checks if there exists any words in stack.
    {
        printf("Stack is empty.\n");
        return;
    }
    else
    {
        //declaring variables to store data to and from the temp stack
        char cpyString[30];
        int op,indexed;
        MyStack temp=createNode_Stack(); //creating head of new temp stack
        while(head->next!=NULL) //as long as the stack is not empty
        {
            strcpy(cpyString,head->next->str); //assign word of top of undo stack to cpyString
            op=head->next->operation;
            indexed=head->next->index;
            push(temp,cpyString,op,indexed); //push data of top of undo stack to temp stack
            printf("%s: ",cpyString);
            if(op==1) //converting the operation type into strings
            {
                printf("Insert: ");
            }
            else
            {
                printf("Remove: ");
            }
            printf("%d \n",indexed);
            pop(head); //finally pop the top of the undo stack and start over
        }
        while(temp->next!=NULL) //putting the elements back into the undo stack from the temp stack in order
        {
            strcpy(cpyString,temp->next->str);
            op=temp->next->operation;
            indexed=temp->next->index;
            push(head,cpyString,op,indexed); //push into undo stack
            pop(temp); //pop from temp stack

        }
        free(temp); //free space allocated for temp stack
    }

}

void enqueue(QPtr head, char newStr []) //function to enqueue elements into queue
{
    if (head!=NULL) //checks if the head node of the queue LL was created
    {
        QPtr temp=createNode_Queue(); //creates new node for enqueueing
        if (temp != NULL)
        {
            if (head->last == NULL) //if there are no elements yet in the queue
            {
                strcpy(temp->str,newStr);
                head->next = temp;
                head->last = temp;
            }
            else //if there is already other elements in queue
            {
                strcpy(temp->str,newStr);
                temp->next=head->last->next; //make next of new node point to whatever the last element was pointing to
                head->last->next=temp; //make the previously last element point to the now last element temp
                head->last=temp;//update the last pointer to be the latest node enqueued
            }
        }
        else
        {
            printf("Memory allocation failed. Cannot push.\n");
        }
    }

    else
    {
        printf("LinkedList of this queue does not exist.\n");
    }


}

QPtr dequeue(QPtr head) //function to dequeue and return the front of the queue
{
    char dequeuedWord[30]; //variable to hold word of front element
    if(head!=NULL && head->next!=NULL) //as long as the queue is not empty and has at least 1 element
    {
        QPtr temp=head->next; //assigning temp pointer to front of queue
        head->next=temp->next; //assigning next of head to be the second element in queue
        head->last=NULL; //updating last pointer to null
        return temp; //returning temp node
    }
    else
    {
        printf("Queue is empty. There is nothing to dequeue.\n");
        return NULL;
    }
}

void DisplayString(ptr head)  //prints out string LL
{

    if (head == NULL || head->next == NULL) //checks if there exists any words in the string
    {
        printf("List of words is empty.\n");
        return;
    }
    else
    {
        ptr temp=head;
        temp = temp->next;
        while(temp!= NULL)
        {
            printf("%s ", temp->word);
            temp = temp->next;
        }
        printf("\n");
    }
}
