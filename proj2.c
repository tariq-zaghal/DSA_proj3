#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX_SIZE 2048
#define MAX_LINES 1024


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


struct expression{
    int valid;
    char* equation;
    char* prefix;
    int value;
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

int isDigit(char c);
int isOperator(char c);
int isOpen(char c);
int isClosed(char c);
int isValidElement(char c);

int main(){

        // char* s = "2+(14-9)+4*[6+(7+5)-1]";

        // // printf("%s\n",s);
        // // char* statement = inToPre(s);
        // // printf("%s\n", statement);

        // // int result = evaluate(statement);

        // //  printf("%d\n",result);
        // int check = statementCheck(s);
        // printf("%d\n",check);


    FILE * file;

    int lineCount = 0;
    struct expression* equations = malloc(sizeof(struct expression)*MAX_LINES);
    char fileName[MAX_SIZE];
    int choice;


    //the output file
    FILE* output;

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> M E N U >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    printf("Welcome to CalcEQ.com!!!\n");
    printf("This calculator is meant to perform basic operations (+,-,*,/) on basic functions presented in a file.\n");
    printf("Please, enter the path of a file containing mathematical expressions you want to read from your computer into\n");
    printf("The program will show you which expressions are valid and which are not.\n\n\n");


    while (1)
    {

    printf("MENU: \n");


    printf("1) Read file\n");
    printf("2) Check validity of equations\n");
    printf("3) Print prefix expressions of valid equations\n");
    printf("4) Evaluate Expression\n");
    printf("5) Print invalid equations\n");
    printf("6) Write to file\n");
    printf("7) Exit\n");

    scanf("%d", &choice);


    //function resposnible for reading the file and checking it
    if(choice == 1){
        lineCount = 0;
        printf("Enter the name of the file you want to read: \n");
        scanf("%s",fileName);

        file = fopen(fileName,"r");


    //if file was not read successfully exit the program
        if(file == NULL){
            printf("Error opening file, program will exit");
            return 1;
        }

    //reads file content line by line
        while(!feof(file) && !ferror(file)){

            equations[lineCount].equation = malloc(sizeof(char)* MAX_SIZE);//initializes equation

            if(fgets(equations[lineCount].equation,MAX_LINES,file)!=NULL) {
                int n = strlen((equations[lineCount].equation));
                if(equations[lineCount].equation[n-1] == '\n'){
                    equations[lineCount].equation[n-1] = '\0';
                }
                lineCount++;

            }   

        }

        printf("%s has been read successfully!\n",fileName);

        fclose(file);

        for(int i = 0; i<lineCount;i++){//performs necessary operations on file content

            equations[i].valid = statementCheck(equations[i].equation); //check validity

            if(equations[i].valid){//when valid:
                equations[i].prefix = inToPre(equations[i].equation);//get prefix
                equations[i].value = evaluate(equations[i].prefix); // calculate value
            }
        }


    }else if(choice == 2){ //prints whether an operation is valid or not

        if(lineCount == 0){//check if read correctly
            printf("YOU HAVE TO READ A FILE FIRST!!!\n");
        }else{
            for(int i = 0;i<lineCount;i++){
                if(equations[i].valid)
                    printf("Equation No.%d: %s -> valid\n",i+1,equations[i].equation);
                else
                    printf("Equation No.%d: %s -> invalid\n",i+1,equations[i].equation);
            }
        }
    }else if(choice==3){//prints the prefix of valid equations
        if(lineCount == 0){
            printf("YOU HAVE TO READ A FILE FIRST!!!\n");
        }else{
            for(int i = 0; i<lineCount;i++){
                if(equations[i].valid)
                    printf("Equation No.%d -> %s\n", i+1,equations[i].prefix);
            }
        }
    }else if(choice==4){//prints the result of valid equations
        if(lineCount == 0){
            printf("YOU HAVE TO READ A FILE FIRST!!!\n");
        }else{
            for(int i = 0; i<lineCount;i++){
                if(equations[i].valid)
                    printf("Equation No.%d -> %d\n", i+1,equations[i].value);
            }
        }
    }else if(choice==5){//prints invalid equations
        if(lineCount == 0){
            printf("YOU HAVE TO READ A FILE FIRST!!!\n");
        }else{
            for(int i = 0; i<lineCount;i++){
                if(!equations[i].valid)
                    printf("Equation No.%d -> %s\n", i+1,equations[i].equation);
            }
        }
    }else if(choice == 6){
        if(lineCount == 0){
            printf("YOU HAVE TO READ A FILE FIRST!!!\n");
        }else{
        output = fopen("output.txt", "w");

        for(int i = 0; i< lineCount; i++){

            if(equations[i].valid){
                fprintf(output,"Equation No.%d: %s -> valid\n", i+1,equations[i].equation);
                fprintf(output,"%s\n",equations[i].prefix);
                fprintf(output,"%d\n",equations[i].value);
            }else{
                 fprintf(output,"Equation No.%d: %s -> invalid\n", i+1,equations[i].equation);
            }
        }

        fclose(output);
        }
    }else if(choice == 7){
        printf("BYE :)\n");
        return 0;
    }else{
        printf("Oops! you have choosen a command that is not in our menu :( Please choose a valid one from 1-7\n");
    }
    }

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
        if((isDigit(statement[0]) || statement[0] == '-') && isDigit(statement[1]))
            return 1;
        else
            return 0;
    }

    if(!isValidElement(statement[0]))//check if the first character is a valid character
        return 0;
    if(!isValidElement(statement[n-1]))//check if the last character is a valid character
        return 0;

//check if the first or last element in statement is an inverse bracket/parantheses (invalid in this case)
    if(isClosed(statement[0]))
            return 0;

    if(isOpen(statement[n-1]))
            return 0;



    // check if first character is + * / which makes statement invalid
    if(statement[0] == '+'||statement[0] == '*' || statement[0] == '/')
        return 0;


    //check if last element is an operator(invalid case)
    if(isOperator(statement[n-1]))
        return 0;

    if(isOpen(statement[0])) //stack to handle parantheses
        push(statement[0],stack);

    for(int i = 1; i<n-1;i++){

        //printf("%c \n", statement[i]);
        if(!isValidElement(statement[i]))
            return 0;

        if(statement[i] == '-'){

            if(!( isOpen(statement[i-1]) || isClosed(statement[i-1]) || isDigit(statement[i-1]) || isOperator(statement[i-1]))) //if the previous element is not a digit or an opening bracket\parantheses
                return 0;

            if(!(isOpen(statement[i+1]) || statement[i+1] == '-' ||isDigit(statement[i+1]))) //if the front element is not a digit or a closing bracket\parantheses
                return 0;

            if(isOperator(statement[i-1]) && statement[i+1] == '-' )
                return 0;

        }

        if(statement[i] == '+' || statement[i] == '*' || statement[i] == '/'){ // the case where the element is an operator

            if(statement[i]=='/' && (statement[i+1]=='0' || (i<(n-2) && statement[i+1]=='-' && statement[i+2]=='0'))){
                return 0;
            }

            if(!(isClosed(statement[i-1]) || isDigit(statement[i-1]))) //if the previous element is not a digit or an opening bracket\parantheses
                return 0;

            if(!(isOpen(statement[i+1]) || statement[i+1] == '-' || isDigit(statement[i+1]))) //if the front element is not a digit or a closing bracket\parantheses
                return 0;
        }

        if(isOpen(statement[i])){ //case of opening bracket\paranthesis
            push(statement[i],stack);

            if(!(isOperator(statement[i-1]) || isOpen(statement[i-1]))) //should be preceded either by an operation or another opening pracket
                return 0;
            if(!(isOpen(statement[i+1]) || statement[i+1] == '-' || isDigit(statement[i+1]))) //should be proceeded by either a digit or another bracket
                return 0;
        }


        if(isClosed(statement[i])){ //case of closing bracket

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


            if(!(isClosed(statement[i-1]) ||  isDigit(statement[i-1]))) //should be preceeded by a digit or another closing bracket
                return 0;

            if(!(isClosed(statement[i+1]) || isOperator(statement[i+1]))) //should be proceeded by another closing bracket or operator
                return 0;

          }


    }


    if(isClosed(statement[n-1])){//handles the case when last element is a bracket
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
    for(int i = n-1;i>=0;i--){//walks on the string in reverse

        if(isDigit(statement[i])){
             resStr[j] = statement[i];
             j++;


             if(i>=2 && !(statement[i-1] =='-' && (isOperator(statement[i-2]) || isOpen(statement[i-2]))) && !isDigit(statement[i-1]) ){
                resStr[j] = ' ';
                j++;
             }else if(i>0 && !isDigit(statement[i-1])&& !(statement[i-1] =='-')){
                resStr[j] = ' ';
                j++;
             }

        }

        if(statement[i] == '/' || statement[i] == '*')
            push(statement[i],stack);



        if(statement[i]=='-'){
            if(i>0 && isDigit(statement[i+1]) && (isOperator(statement[i-1]) || isOpen(statement[i-1]))){
                resStr[j] = '-';
                j++;
                resStr[j] = ' ';
                j++;

            }else if(i==0 && isOpen(statement[1]) ){
                push('*',stack);
                resStr[j] = '1';
                j++;
                resStr[j] = '-';
                j++;
                resStr[j] = ' ';
                j++;



            }else if(i>0 && isOpen(statement[i+1]) && (isOpen(statement[i-1]) || isOperator(statement[i-1])) ){

                push('*',stack);
                resStr[j] = '1';
                j++;
                resStr[j] = '-';
                j++;
                resStr[j] = ' ';
                j++;
            }else if(i == 0 && isDigit(statement[i+1])){
                resStr[j] = '-';
                j++;
            }else{

                if(top(stack) == '*' || top(stack) == '/'){
                    resStr[j] = pop(stack);
                    j++;

                    resStr[j] = ' ';
                    j++;
                }

                push(statement[i],stack);

            }


        }








        if(statement[i] == '+'){
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

   // printf("%s\n", resStr);

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

            if(statement[i]=='-'){
                num*=-1;
                i--;
            }

            pushInt(num,stack);
           // printf("%d\n",num);
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



int isOperator(char c){
   return  (c == '-' || c == '+' || c == '*' || c == '/');
}

int isOpen(char c){
    return (c == '(' || c == '[');
}


int isClosed(char c){
    return (c == ')' || c == ']');
}

int isDigit(char c){
    return (c>=48 && c<=57);
}


int isValidElement(char c){
    return (isOperator(c)||isDigit(c)||isOpen(c)||isClosed(c));
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
      //  printf("stack is empty!\n");
        return '\0';
    }
}

char top(Stack stack){
    if(!isEmpty(stack))
        return stack->next->element;
    else{
      //  printf("stack is empty!\n");
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
       // printf("stack is empty!\n");
        return -1;
    }
}

int topInt(StackInt stack){
    if(!isEmptyInt(stack))
        return stack->next->element;
    else{
       // printf("stack is empty!\n");
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




