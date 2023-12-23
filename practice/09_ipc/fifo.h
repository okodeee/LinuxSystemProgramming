#define	MAX_FIFO_NAME	32

#define	SERV_FIFO	"./.fifo"

typedef struct  {
	char	returnFifo[MAX_FIFO_NAME];
	//client가 많기 때문에 returnfifo 이름을 저장

	char	data[128];
	//메세지 내용
}
	MsgType;
