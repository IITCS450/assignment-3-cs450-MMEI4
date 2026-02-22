#include "types.h"
#include "stat.h"
#include "user.h"

static volatile int sink = 0;
static void burn(int n){ for(int i=0;i<n;i++) sink += i; }

#define BURN 20000
static void test_share(int duration, int tA, int tB){
	int pipeA[2], pipeB[2];
	int pidA, pidB, pidD;
	int countA, countB;
	printf(1, "\n--- Timed CPU Share Experiment ---\n");
	printf(1, "child A: %d ticket, child B: %d tickets\n", tA, tB);
	printf(1, "running for ~%d ticks each\n", duration);

	pipe(pipeA);
	pipe(pipeB);


	pidA = fork();
   	settickets(tA);
	if(pidA == 0){
		close(pipeA[0]);
    	close(pipeB[0]);
    	close(pipeB[1]);
    	int done = 0;
    	uint start = uptime();
    	while((uptime() - start) < duration){
      		burn(BURN);
      		done++;
    	}	
    	write(pipeA[1], &done, sizeof(done));
    	close(pipeA[1]);
    	exit();
	}	

	pidB = fork();
    settickets(tB);
  	if(pidB == 0){
    	close(pipeB[0]);
    	close(pipeA[0]);
    	close(pipeA[1]);
    	int done = 0;
    	uint start = uptime();
    	while((uptime() - start) < duration){
      		burn(BURN);
      		done++;
    	}	
    	write(pipeB[1], &done, sizeof(done));
    	close(pipeB[1]);
    	exit();
	}		

	pidD = fork();
	if(pidD == 0){
        uint start;
        close(pipeA[0]); close(pipeA[1]);
        close(pipeB[0]); close(pipeB[1]);
        settickets(1);
        start = uptime();
        while((uptime() - start) < (uint)duration){
            burn(BURN);
        }
        exit();
    }

  	close(pipeA[1]);
  	close(pipeB[1]);

	read(pipeA[0], &countA, sizeof(countA));
  	read(pipeB[0], &countB, sizeof(countB));
  	close(pipeA[0]);
  	close(pipeB[0]);

  	wait(); wait(); wait();

  	printf(1, "child A (%d tickets)  burn count: %d\n",tA, countA);
  	printf(1, "child B (%d tickets) burn count: %d\n",tB, countB);
  	if(countA > 0){
    	int ratio10 = (countB * 10) / countA;
    	printf(1, "ratio B/A * 10 = %d  (expected ~%d for %d:%d)\n", ratio10, (tB*10)/tA, tB, tA);
    
  	}	
}

int main(int argc, char **argv){
  int t = 10;
  if(argc == 2) t = atoi(argv[1]);

  if(settickets(0) == 0){
    printf(1, "FAIL: settickets(0) should fail\n");
    exit();
  }
  if(t >= 1 && settickets(t) != 0){
    printf(1, "FAIL: settickets(%d) should succeed\n", t);
    exit();
  }
  printf(1, "PASS: settickets validation\n");

  for(int k=0;k<200;k++) burn(200000);

  test_share(1, 2, 4);
  test_share(5, 2, 4);
  test_share(10, 2, 4);
  test_share(50, 2, 4);
  test_share(100, 2, 4);
  test_share(500, 2, 4);
  test_share(1, 1, 8);
  test_share(10, 1, 8);
  test_share(100, 1, 8);
  test_share(1000, 1, 8);

  
  printf(1, "testlottery: done\n");
  exit();
}
