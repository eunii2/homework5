#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum {
    lparen = 0, /* ( 왼쪽 괄호 */
    rparen = 9, /* ) 오른쪽 괄호*/
    times = 7, /* * 곱셈 */
    divide = 6, /* / 나눗셈 */
    plus = 5, /* + 덧셈 */
    minus = 4, /* - 뺄셈 */
    operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];     // infix 표기법으로 표현된 수식을 저장하는 배열
char postfixExp[MAX_EXPRESSION_SIZE];   // postfix 표기법으로 변환된 수식을 저장하는 배열
char postfixStack[MAX_STACK_SIZE];      // postfix 표기법으로 변환할 때 필요한 스택을 저장하는 배열
int evalStack[MAX_STACK_SIZE];          // 계산을 위해 필요한 스택을 저장하는 배열
int postfixStackTop = -1;               // postfix 표기법으로 변환할 때 필요한 스택의 탑 인덱스
int evalStackTop = -1;                  // 계산을 위해 필요한 스택의 탑 인덱스
int evalResult = 0;                     // 계산 결과값을 저장하는 변수

void postfixPush(char x);               // postfix 표기법으로 변환할 때 필요한 스택에 데이터를 push하는 함수
char postfixPop();                      // postfix 표기법으로 변환할 때 필요한 스택에서 데이터를 pop하는 함수
void evalPush(int x);                   // 계산을 위해 필요한 스택에 데이터를 push하는 함수
int evalPop();                          // 계산을 위해 필요한 스택에서 데이터를 pop하는 함수
void getInfix();                        // 사용자로부터 infix 표기법으로 표현된 수식을 입력받는 함수
precedence getToken(char symbol);       // 입력된 문자의 우선순위를 반환하는 함수
precedence getPriority(char x);         // 입력된 연산자의 우선순위를 반환하는 함수
void charCat(char* c);                  // postfix 표기법으로 변환할 때 필요한 문자열 연결 함수
void toPostfix();                       // infix 표기법을 postfix 표기법으로 변환하는 함수
void debug();                           // 디버깅을 위한 함수
void reset();                           // 초기화 함수
void evaluation();                      // postfix 표기법으로 변환된 수식을 계산하는 함수

int main() {
    printf("[----- [최가은] [2022078054] -----]");

    char command;  // 사용자 입력을 받을 변수 선언
    do {  // do-while 루프: 사용자가 q를 입력하기 전까지 반복
        printf("---------------------------------------------------------------\n");
        printf(" Infix to Postfix, then Evaluation\n");  // 프로그램의 기능 설명
        printf("---------------------------------------------------------------\n");
        printf(" Infix=i, Postfix=p, Eval=e, Debug=d, Reset=r, Quit=q\n");  // 사용자 입력 안내
        printf("---------------------------------------------------------------\n");
        printf("Command = ");
        scanf(" %c", &command);  // 사용자 입력 받음
        switch (command) {  // 사용자가 입력한 명령어에 따라 분기
        case 'i':
        case 'I':  // Infix 입력 명령
            getInfix();  // 중위식 입력 함수 호출
            break;
        case 'p':
        case 'P':  // Postfix 변환 명령
            toPostfix();  // 후위식 변환 함수 호출
            break;
        case 'e':
        case 'E':  // 평가 명령
            evaluation();  // 평가 함수 호출
            break;
        case 'd':
        case 'D':  // 디버그 명령
            debug();  // 디버그 함수 호출
            break;
        case 'r':
        case 'R':  // 초기화 명령
            reset();  // 초기화 함수 호출
            break;
        case 'q':
        case 'Q':  // 종료 명령
            break;
        default:  // 잘못된 명령 입력 시
            printf("\n >>>>> Concentration!! <<<<<\n");
            break;
        }
    } while (command != 'q' && command != 'Q');  // 사용자가 q를 입력하지 않은 동안 반복

    return 1;  // 프로그램 종료
}

void postfixPush(char x) {
    postfixStack[++postfixStackTop] = x;  // 후위식 스택에 x 값을 추가
}

char postfixPop() {
    char x;
    if (postfixStackTop == -1)  // 후위식 스택이 비어있는 경우
        return '\0';  // null 문자 반환
    else {
        x = postfixStack[postfixStackTop--];  // 후위식 스택에서 값을 꺼내 x에 저장
    }
    return x;  // x 값 반환
}

void evalPush(int x) { //Evaluation stack에 정수형 값을 추가하는 함수
    evalStack[++evalStackTop] = x;
}

int evalPop() { //Evaluation stack에서 정수형 값을 pop하는 함수
    if (evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix() { //사용자로부터 infix expression을 입력받아 infixExp에 저장하는 함수
    printf("Type the expression >>> ");
    scanf("%s", infixExp);
}

precedence getToken(char symbol) {
    switch (symbol) {
    case '(':
        return lparen;
    case ')':
        return rparen;
    case '+':
        return plus;
    case '-':
        return minus;
    case '/':
        return divide;
    case '*':
        return times;
    default :
        return operand;
    }
}

precedence getPriority(char x) {
    return getToken(x);
}

/* 문자 하나를 전달받아, postfixExp에 추가 */
void charCat(char* c) {
    if (postfixExp == '\0')
        strncpy(postfixExp, c, 1); // postfixExp에 문자를 복사
    else
        strncat(postfixExp, c, 1); // postfixExp에 문자를 추가
}

/* infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postfixExp에 저장된다.
 */
void toPostfix() {
    /* infixExp의 문자 하나씩을 읽기위한 포인터 */
    char *exp = infixExp;
    char x; /* 문자하나를 임시로 저장하기 위한 변수 */
    /* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
    while(*exp != '\0') {
        if(getPriority(*exp) == operand) {
            x = *exp;
            charCat(&x); // 피연산자일 경우, postfixExp에 추가
        }
        else if(getPriority(*exp) == lparen) {
            postfixPush(*exp); // '('일 경우, stack에 push
        }
        else if(getPriority(*exp) == rparen) {
            while((x = postfixPop()) != '(') { // ')'일 경우, '('가 나올 때까지 pop
                charCat(&x); // pop한 연산자를 postfixExp에 추가
            }
        }
        else {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) {
                x = postfixPop();
                charCat(&x); // 스택에 있는 연산자 우선순위가 높은 경우, pop 후 postfixExp에 추가
            }
            postfixPush(*exp); // 현재 연산자를 stack에 push
        }
        exp++; // 다음 문자로 이동
    }
    while(postfixStackTop != -1) {
        x = postfixPop();
        charCat(&x); // 남은 연산자 모두 pop 후 postfixExp에 추가
    }
}

void debug() { //현재 상태를 출력하는 디버그 함수
    printf("\n---DEBUG\n");
    printf("infixExp = %s\n", infixExp);
    printf("postExp = %s\n", postfixExp);
    printf("eval result = %d\n", evalResult);
    printf("postfixStack : ");
    for(int i = 0; i < MAX_STACK_SIZE; i++)
        printf("%c ", postfixStack[i]);
    printf("\n");
}

void reset() { //계산기 초기화 함수
    infixExp[0] = '\0'; //infixExp 초기화
    postfixExp[0] = '\0'; //postfixExp 초기화
    for(int i = 0; i < MAX_STACK_SIZE; i++)
        postfixStack[i] = '\0'; //postfixStack 초기화

    postfixStackTop = -1; // postfixStackTop 초기화
    evalStackTop = -1; // evalStackTop 초기화
    evalResult = 0; // evalResult 초기화
}

void evaluation() {                           // 후위 표기식 계산 함수
    int opr1, opr2, i;                        // 계산에 사용될 피연산자 opr1, opr2와 반복문에서 사용될 i 선언
    int length = strlen(postfixExp);          // 후위 표기식 문자열의 길이 계산
    char symbol;                              // 후위 표기식에서 가져온 연산자나 피연산자 저장 변수
    evalStackTop = -1;                        // 계산 스택 초기화
    for(i = 0; i < length; i++) {             // 후위 표기식 문자열을 반복하며 계산 수행
        symbol = postfixExp[i];               // 문자열에서 한 글자씩 가져옴
        if(getToken(symbol) == operand) {     // 가져온 문자가 피연산자인 경우
            evalPush(symbol - '0');           // 스택에 추가 (문자를 정수로 변환한 후 스택에 추가)
        }
        else {                                // 가져온 문자가 연산자인 경우
            opr2 = evalPop();                 // 스택에서 두 번째 피연산자를 팝해서 opr2에 저장
            opr1 = evalPop();                 // 스택에서 첫 번째 피연산자를 팝해서 opr1에 저장
            switch(getToken(symbol)) {        // 연산자에 따른 계산 수행
                case plus: evalPush(opr1 + opr2); break;     // 덧셈
                case minus: evalPush(opr1 - opr2); break;    // 뺄셈
                case times: evalPush(opr1 * opr2); break;    // 곱셈
                case divide: evalPush(opr1 / opr2); break;   // 나눗셈
                default: break;                // 다른 연산자인 경우 아무 작업도 하지 않음
            }
        }
    }
    evalResult = evalPop();                    // 계산 결과 팝해서 evalResult에 저장
}
