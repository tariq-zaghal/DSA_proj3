#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX_SIZE 2048


typedef struct node* ptrNode;
typedef ptrNode Stack;

typedef struct nodeInt* ptrNodeInt;
typedef ptrNodeInt StackInt;



struct node{
    char element;
    ptrNode next;
};



struct nodeInt{
    int element;
    ptrNodeInt next;
};



//CHAR STACK>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Stack initializeStack();
int isEmpty(Stack stack);
void push(char x, Stack stack);
char pop(Stack stack);
char top(Stack stack);
void makeEmpty(Stack stack);
void disposeStack(Stack stack);

//INT STACK>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StackInt initializeStackInt();
int isEmptyInt(StackInt stack);
void pushInt(int x, StackInt stack);
int popInt(StackInt stack);
int topInt(StackInt stack);
void makeEmptyInt(StackInt stack);
void disposeStackInt(StackInt stack);



//THE FUNCTIONS>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int statementCheck(char * statement);
char* inToPre(char* statement);
void reverse(char* str, int len);
int evaluate(char* statement);

int main(){

    char* s = "3-443+542*[(333-215)]";

    printf("%s\n",s);
    char* statement = inToPre(s);

    printf("%s \n",statement);

    //int result = evaluate(statement);

    //printf("%d\n",result);

    return 0;
}





int statementCheck(char * statement){ //checks the overall placement of operators, brackets, and parantheses in the equation

    int n = strlen(statement);    

    char operator[] = {'+', '-', '*', '/'};
    char paran[] = {'(', ')', '[', ']'};

    Stack stack = initializeStack();


    
    if(n == 0)//empty statement
        return 0;
    
    if(n == 2){
        if(((statement[0]>=48 && statement[0]<=57) || statement[0] == operator[1]) && (statement[1]>=48 && statement[1]<=57))
            return 1;
        else
            return 0;
    }


//check if the first or last element in statement is an inverse bracket/parantheses (invalid in this case)
    if(statement[0] == paran[1] || statement[0] == paran[3])
            return 0;

    if(statement[n-1] == paran[0] || statement[n-1] == paran[2])
            return 0;



    // check if first character is + * / which makes statement invalid
    if(statement[0] == operator[0]||statement[0] == operator[2] || statement[0] == operator[3])
        return 0;

    
    //check if last element is an operand(invalid case)
     for(int i = 0; i< 4; i++){
        if(statement[n-1] == operator[i])
            return 0;
    }

    if(statement[0]== paran[0] || statement[0]== paran[2]) //stack to handle parantheses
        push(statement[0],stack);

    for(int i = 1; i<n-1;i++){

        printf("%c \n", statement[i]);

        if(statement[i] == operator[0] || statement[i] == operator[1] || statement[i] == operator[2] || statement[i] == operator[3]){ // the case where the element is an operator

            if(!(statement[i-1] == paran[1] || statement[i-1] == paran[3] ||  (statement[i-1]>=48 && statement[i-1]<=57))) //if the previous element is not a digit or an opening bracket\parantheses 
                return 0;
                
            if(!(statement[i+1] == paran[0] || statement[i+1] == paran[2] ||(statement[i+1]>=48 && statement[i+1]<=57))) //if the front element is not a digit or a closing bracket\parantheses 
                return 0;
        }

        if(statement[i] == paran[0] || statement[i] == paran[2]){ //case of opening bracket\paranthesis
            push(statement[i],stack);

            if(!(statement[i-1] == operator[0] || statement[i-1] == operator[1] ||statement[i-1] == operator[2] ||statement[i-1] == operator[3] || statement[i-1] == paran[0] || statement[i-1] == paran[2])) //should be preceded either by an operation or another opening pracket
                return 0;
            if(!(statement[i+1] == paran[0] || statement[i+1] == paran[2] ||(statement[i+1]>=48 && statement[i+1]<=57))) //should be proceeded by either a digit or another bracket
                return 0;
        }


        if(statement[i] == paran[1] || statement[i] == paran[3]){ //case of closing bracket

            if(stack == NULL)
                return 0;
            else{
                char temp = pop(stack);
                if(statement[i] == paran[1]){
                    if(temp!=paran[0])
                        return 0;
                }

                if(statement[i] == paran[3]){
                    if(temp!=paran[2])
                        return 0;
                }

            }
            

            if(!(statement[i-1] == paran[1] || statement[i-1] == paran[3] ||  (statement[i-1]>=48 && statement[i-1]<=57))) //should be preceeded by a digit or another closing bracket
                return 0;  

            if(!(statement[i+1] == paran[1] || statement[i+1] == paran[3] || statement[i+1] == operator[0] || statement[i+1] == operator[1] ||statement[i+1] == operator[2] ||statement[i+1] == operator[3] )) //should be proceeded by another closing bracket or operator
                return 0;    
                    
          }


    }


    if(statement[n-1] == paran[1] || statement[n-1] == paran[3]){//handles the case when last element is a bracket
        if(isEmpty(stack))
            return 0;
        else{
            char temp = pop(stack);
            if(statement[n-1] == paran[1]){
                if(temp!=paran[0])
                    return 0;
            }

            if(statement[n-1] == paran[3]){
                if(temp!=paran[2])
                    return 0;
            }

        }
    }

    if(!isEmpty(stack))
        return 0;

    return 1;


}



char* inToPre(char* statement){//converts from infix to prefix

    Stack stack = initializeStack();
    int n = strlen(statement);
    char* resStr = malloc(sizeof(char)*MAX_SIZE);
    int j = 0; 
    for(int i = n-1;i>=0;i--){
      //  printf("%c\n",statement[i]);
        if(statement[i]>=48 && statement[i]<=57){
             resStr[j] = statement[i];
             j++;

             if(i>=1 && !(statement[i-1]>=48 && statement[i-1]<=57)){
                resStr[j] = ' ';
                j++;
             }
        }

        if(statement[i] == '/' || statement[i] == '*')
            push(statement[i],stack);
        
        if(statement[i] == '-' || statement[i] == '+'){
            if(top(stack) == '*' || top(stack) == '/'){
                resStr[j] = pop(stack);
                j++;

                resStr[j] = ' ';
                j++;
            }

            push(statement[i],stack);

        }

        if(statement[i] == ')' || statement[i] == ']')
            push(statement[i],stack);

        if(statement[i] == '('){
            char temp = pop(stack);
            while(temp!= ')'){
                resStr[j] = temp;
                j++;
                resStr[j] = ' ';
                j++;
                temp = pop(stack);
            }
        }

        if(statement[i] == '['){
            char temp = pop(stack);
            while(temp!= ']'){
                resStr[j] = temp;
                j++;

                resStr[j] = ' ';
                j++;
                temp = pop(stack);
            }
        }

    }

    while(!isEmpty(stack)){
        resStr[j] = ' ';
        j++;

        resStr[j] = pop(stack);
        j++;
    }


    resStr[j] = '\0';

    int m = strlen(resStr);
    reverse(resStr, m);

    printf("%s\n", resStr);

    return resStr;

}


void reverse(char* str, int len){
     char temp;
    for(int i = 0;i<len/2;i++){
        temp = str[i];  
        str[i] = str[len - i - 1];  
        str[len - i - 1] = temp;  
    }
}


int evaluate(char* statement){
    int res = 0;
    int n = strlen(statement);
    StackInt stack = initializeStackInt();
    
    // for(int i = n-1; i>=0; i--){

    // }

    int i = n-1;

    int num = 0;
    int a;
    int b;
    
    while(i>=0){

        if(statement[i]>=48 && statement[i]<=57){
            int ten = 1;
            
            while(statement[i]>=48 && statement[i]<=57){
                num += (int)(statement[i] -'0')*ten;
                ten = ten*10;
                i--;
            }

            pushInt(num,stack);
            num = 0;
        }


        if(statement[i] == '+'){
            a = popInt(stack);
            b = popInt(stack);
            a = a+b;

            pushInt(a,stack);
            
        }

        
        if(statement[i] == '-'){
            a = popInt(stack);
            b = popInt(stack);
            a = a-b;
            
            pushInt(a,stack);
            
        }

        
        if(statement[i] == '*'){
            a = popInt(stack);
            b = popInt(stack);
            a = a*b;
            
            pushInt(a,stack);
            
        }

        
        if(statement[i] == '/'){
            a = popInt(stack);
            b = popInt(stack);
            a = a/b;
            
            pushInt(a,stack);
            
        }




        i-- ;

    }

    res = popInt(stack);

    return res;

}










//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>STACK FUNCTIONS>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Stack initializeStack(){
    Stack stack;
    stack = malloc(sizeof(struct node));
    if( stack == NULL )
        printf( "Out of space!\n" );
    stack->next = NULL;

    return stack;
    
}


int isEmpty(Stack stack){
    return stack->next == NULL;
}

void push(char x, Stack stack){
    ptrNode newNode = malloc(sizeof(struct node));

    if( newNode == NULL)
    printf( "Out of space!" );


    newNode->element = x;
    newNode->next = stack->next;
    stack->next = newNode;
    

}



char pop(Stack stack){
    if(!isEmpty(stack)){
            ptrNode temp = stack->next;
            stack->next = temp->next;
            char val = temp->element;

            free(temp);
            return val;
    }else{
        printf("stack is empty!\n");
        return '\0';
    }
}

char top(Stack stack){
    if(!isEmpty(stack))
        return stack->next->element;
    else{
        printf("stack is empty!\n");
        return '\0';
    }
        
}

void makeEmpty(Stack stack){
    while(!isEmpty(stack)){
        pop(stack);
    }
}


void disposeStack(Stack stack){
    makeEmpty(stack);
    free(stack);
}






//>>>>>>>>>>>>>>>>>>INT STACK>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


StackInt initializeStackInt(){
    StackInt stack;
    stack = malloc(sizeof(struct nodeInt));
    if( stack == NULL )
        printf( "Out of space!\n" );
    stack->next = NULL;
    return stack;
    
}



void pushInt(int x, StackInt stack){
    ptrNodeInt newNode = malloc(sizeof(struct nodeInt));

    if(newNode == NULL)
    printf( "Out of space!" );


    newNode->element = x;
    newNode->next = stack->next;
    stack->next = newNode;
    

}

int isEmptyInt(StackInt stack){
    return stack->next == NULL;
}

int popInt(StackInt stack){
    if(!isEmptyInt(stack)){
            ptrNodeInt temp = stack->next;
            stack->next = temp->next;
            int val = temp->element;

            free(temp);
            return val;
    }else{
        printf("stack is empty!\n");
        return -1;
    }
}

int topInt(StackInt stack){
    if(!isEmptyInt(stack))
        return stack->next->element;
    else{
        printf("stack is empty!\n");
        return -1;
    }
        
}

void makeEmptyInt(StackInt stack){
    while(!isEmptyInt(stack)){
        popInt(stack);
    }
}


void disposeStackInt(StackInt stack){
    makeEmptyInt(stack);
    free(stack);
}










