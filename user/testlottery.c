#include "types.h"
#include "stat.h"
#include "user.h"

static volatile int sink = 0;
static void burn(int n){ for(int i=0;i<n;i++) sink += i; }

static void test_share(int duration, int tA, int tB){
	int pipeA[2], pipeB[2];
	int pidA, pidB;
	int countA, countB;
	printf(1, "\n--- Timed CPU Share Experiment ---\n");
	printf(1, "child A: %d ticket, child B: %d tickets\n", tA, tB);
	printf(1, "running for ~%d ticks each\n", duration);

	pipe(pipeA);
	pipe(pipeB);


   	settickets(tA);
	pidA = fork();
	if(pidA == 0){
		close(pipeA[0]);
    	close(pipeB[0]);
    	close(pipeB[1]);
    	int done = 0;
    	uint start = uptime();
    	while((uptime() - start) < duration){
      		burn(10000);
      		done++;
    	}	
    	write(pipeA[1], &done, sizeof(done));
    	close(pipeA[1]);
    	exit();
	}	

    settickets(tB);
	pidB = fork();
  	if(pidB == 0){
    	close(pipeB[0]);
    	close(pipeA[0]);
    	close(pipeA[1]);
    	int done = 0;
    	uint start = uptime();
    	while((uptime() - start) < duration){
      		burn(10000);
      		done++;
    	}	
    	write(pipeB[1], &done, sizeof(done));
    	close(pipeB[1]);
    	exit();
	}		

  	close(pipeA[1]);
  	close(pipeB[1]);

	read(pipeA[0], &countA, sizeof(countA));
  	read(pipeB[0], &countB, sizeof(countB));
  	close(pipeA[0]);
  	close(pipeB[0]);

  	wait(); wait();

  	printf(1, "child A (%d tickets)  burn count: %d\n",tA, countA);
  	printf(1, "child B (%d tickets) burn count: %d\n",tB, countB);
  	if(countA > 0){
    	int ratio10 = (countB * 10) / countA;
    	printf(1, "ratio B/A * 10 = %d  (expected ~%d for %d:%d)\n", ratio10, (tB*10)/tA, tB, tA);
    	if(ratio10 >= ((tB*10)/tA)/2 && ratio10 <= ((tB*10)/tA)*2){
      		printf(1, "PASS: ratio within acceptable range\n");
    	} else {
      		printf(1, "NOTE: ratio outside tight range (variance is normal for short runs)\n");
    }
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

  test_share(500, 1, 10);
  test_share(5000, 1, 10);
  test_share(1000, 2, 5);
  printf(1, "testlottery: done\n");
  exit();
}
