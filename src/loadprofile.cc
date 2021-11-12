#include <cstdlib>
#include <cstdio>
#include <iostream>

//using namespace std;


void loadp(std::string fullFileName, std::string& feedBack){

	const int N = 300;

	std::string fileName;	//name without path
	fileName = fullFileName.substr(fullFileName.find_last_of("/") + 1);


	std::string scmd = "echo \"123\" | sudo mv " + fullFileName + " /etc/apparmor.d";
	//const char* cmd1 = scmd.c_str();
	//system(cmd1);
	system(scmd.c_str());


	char line[N];
	FILE *fp;

	scmd = "echo \"123\" | sudo apparmor_parser -r /etc/apparmor.d/" + fileName;
	//const char* cmd2 = scmd.c_str();
	//fp = popen(cmd2, "r");
	fp = popen(scmd.c_str(), "r");


	//error message
	std::string fb = "";
	std::string sl;
	while(fgets(line, sizeof(line)-1, fp) !=NULL){
		sl = line;
		fb += sl;
	}


	feedBack = fb;


	pclose(fp);

}

int main(){

	std::string FEEDBACK;

	loadp("/home/fer/Desktop/appNov1/usr.bin.certspotter", FEEDBACK);
	//loadp("/home/fer/Desktop/appNov1/usr.bin.evolution", FEEDBACK);
	std::cout << FEEDBACK;

	return 0;
}