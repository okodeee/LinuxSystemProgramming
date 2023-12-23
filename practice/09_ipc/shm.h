#define	MY_ID		40  // shm_key가 겹치면 permission denied 발생
#define	SHM_KEY		(0x9000 + MY_ID)
#define	SHM_SIZE	1024
#define	SHM_MODE	(SHM_R | SHM_W | IPC_CREAT)
