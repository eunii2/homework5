#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 //큐의 최대 크기를 정의하는 매크로 함수

typedef char element; //큐에 저장할 자료형을 char로 정의
typedef struct {
	element queue[MAX_QUEUE_SIZE]; //큐를 구현하는 구조체
	int front, rear; //큐의 front와 rear 인덱스를 저장하는 변수
}QueueType;

QueueType *createQueue(); //큐를 생성하고 초기화하는 함수
int freeQueue(QueueType *cQ); //동적으로 할당한 큐 메모리를 해제하는 함수
int isEmpty(QueueType *cQ); //큐가 비어있는지 검사하는 함수
int isFull(QueueType *cQ); //큐가 가득 차있는지 검사하는 함수
void enQueue(QueueType *cQ, element item); //큐에 원소를 삽입하는 함수
void deQueue(QueueType *cQ, element* item); //큐에서 원소를 삭제하고 반환하는 함수
void printQ(QueueType *cQ); //큐의 원소들을 출력하는 함수
void debugQ(QueueType *cQ); //디버그용 함수, 큐의 내부 상태를 출력
element getElement(); //키보드에서 한 문자를 입력받아 반환하는 함수


int main(void)
{
	printf("[----- [최가은] [2022078054] -----]");

	QueueType *cQ = createQueue(); //큐를 생성하고 초기화
	element data; //큐에 삽입할 원소를 저장할 변수

	char command; //사용자 입력을 받을 변수

	do{
		//사용자 메뉴 출력
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I': //큐에 원소를 삽입하는 경우
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D': //큐에서 원소를 삭제하는 경우
			deQueue(cQ,&data);
			break;
		case 'p': case 'P': //큐의 원소를 출력하는 경우
			printQ(cQ);
			break;
		case 'b': case 'B': //큐의 내부 상태를 출력하는 경우
			debugQ(cQ);
			break;
		case 'q': case 'Q': //프로그램을 종료하는 경우
			break;
		default: //잘못된 입력이 들어온 경우
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	freeQueue(cQ); //동적 할당한 큐 메모리를 해제
	return 1;
}

QueueType *createQueue() //새로운 큐를 생성하고 그 주소를 반환하는 함수
{
	QueueType *cQ; //새로운 큐를 가리키는 포인터 변수
	cQ = (QueueType *)malloc(sizeof(QueueType)); //동적 메모리 할당
	cQ->front = 0; //시작 인덱스 초기화
	cQ->rear = 0; //끝 인덱스 초기화
	return cQ; //새로운 큐의 포인터 반환
}

int freeQueue(QueueType *cQ) //큐를 해제하는 함수
{
    if(cQ == NULL) return 1; //큐가 이미 해제되었거나 비어있는 경우
    free(cQ); //동적 메모리 해제
    return 1;
}

element getElement() //사용자로부터 입력을 받아 큐의 원소로 추가하는 함수
{
	element item; //새로운 원소를 저장할 변수
	printf("Input element = ");
	scanf(" %c", &item); //사용자로부터 문자 하나를 입력받음
	return item; //새로운 원소 반환
}

int isEmpty(QueueType *cQ) //큐가 비어있는지 확인하는 함수
{
	if (cQ->front == cQ->rear){ //시작 인덱스와 끝 인덱스가 같은 경우
		printf("Circular Queue is empty!"); //큐가 비어있다는 메시지 출력
		return 1; //참 반환
	}
	else return 0; //거짓 반환
}

int isFull(QueueType *cQ) //큐가 가득 찼는지 확인하는 함수
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { //끝 인덱스 다음 인덱스가 시작 인덱스인 경우
		printf(" Circular Queue is full!\n"); //큐가 가득 찼다는 메시지 출력
		return 1; //참 반환
	}
	else return 0; //거짓 반환
}

void enQueue(QueueType *cQ, element item) //새로운 요소를 큐의 맨 뒤에 추가하는 함수
{	//큐가 가득 찬 상태인 경우, 함수 종료
	if(isFull(cQ)) return;
	else {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
		// rear 위치에 새로운 요소 삽입
		cQ->queue[cQ->rear] = item;
	}
}

void deQueue(QueueType *cQ, element *item) //큐의 맨 앞에 있는 요소를 삭제하고, 삭제한 요소를 반환하는 함수
{
	//큐가 비어있는 상태인 경우, 함수 종료
	if(isEmpty(cQ)) return;
	else {
		//front를 다음 위치로 이동(환형 큐이므로, 다음 위치는 front+1이 아닌 (front+1) % MAX_QUEUE_SIZE)
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE;
		//front 위치에 있는 요소를 반환값으로 전달받은 포인터 item에 저장
		*item = cQ->queue[cQ->front];
		return;
	}
}


void printQ(QueueType *cQ) //큐의 내용을 출력하는 함수
{
	int i, first, last;

	//front와 rear의 다음 위치를 계산하여 first와 last에 저장
	first = (cQ->front + 1)%MAX_QUEUE_SIZE;
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;
	// 큐의 내용을 출력
	printf("Circular Queue : [");

	i = first;
	while(i != last){
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) //큐의 상태를 디버깅하는 함수
{
	//디버그 메시지 출력
	printf("\n---DEBUG\n");
	//배열의 모든 요소를 순회하며 디버그 정보 출력
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		//front 위치일 경우, front라는 메시지 출력
		if(i == cQ->front) {
			printf("  [%d] = front\n", i);
			continue;
		}
		//그 외의 경우, 해당 위치의 요소 출력
		printf("  [%d] = %c\n", i, cQ->queue[i]);

	}
	//printQ함수를 호출하여 큐의 내용 출력
	printQ(cQ);
	//front와 rear의 위치 출력
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}