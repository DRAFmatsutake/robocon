#include "camera.h"
#include "com.h"

int main(void){
	char c;
	Camera cam1;
	Camera cam2;
	if(cam1.Open(0)==-1)
		return -1;
	if(cam2.Open(2)==-1)
		return -1;
	do{
		cam1.Update();
		cam2.Update();
		//cam.Show();
		c=Func::KeyState();
		if(c=='w')
			cam1.Capture();
		if(c=='e')
			cam2.Capture();
	}while(c!='q');
	cam1.Close();
	cam2.Close();
	return 0;
}
