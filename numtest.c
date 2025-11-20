#include "types.h"
#include "user.h"
#include "stat.h"

int main(void){
	int vp;
	printf(1, "Testing numvp() and numpp() system calls\n");
	
	vp = numvp();
	int pp = numpp();
	int tpsize = getptsize();

	printf(1, "Virtual Pages : %d\n", vp);
	printf(1, "Physical pages : %d\n", pp);
	printf(1, "Page Table Size : %d\n", tpsize);
	exit();

}

