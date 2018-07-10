#include "../Input/_Input.h"
#include "../Input/Standard_Ipt.h"
#include "string"
#include "sstream"
#include <stdlib.h>
#include <unistd.h>

using namespace nw;
using namespace std;

int main(int argc, char* argv[]){
/* argv[1]: Input file path (string)
 * argv[2]: output directory (string)
 * argv[3]: number of iterations (int)
 * */
  
	for (int i = 1; i <= atoi(argv[3]); i++){ // 3. Number of iterations
		_Input *ipt = new Standard_Ipt(argv[1],argv[2]); //1. Input File path 2. Output directory
		ipt->get_System()->go(i);
//		Release memory!
		if(ipt){delete ipt;ipt = NULL;}
//		Sleep for two seconds to get new seed for random generator
		sleep(2);
	}
}


