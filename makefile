master: main.cpp com.cpp moter.cpp serial.cpp main_proc.cpp imgProcess.cpp objectPosition.cpp
	g++ -o _master.out \
	main.cpp main_proc.cpp com.cpp camera.cpp robot.cpp moter.cpp serial.cpp imgProcess.cpp objectPosition.cpp \
	-I /usr/include/opencv4 -lwiringPi \
	-lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_imgproc

serial: test_serial.cpp com.cpp serial.cpp
	g++ -o _serial.out test_serial.cpp com.cpp serial.cpp -lwiringPi

moter: test_moter.cpp com.cpp serial.cpp moter.cpp
	g++ -o _moter.out test_moter.cpp com.cpp serial.cpp moter.cpp -lwiringPi

camera: test_camera.cpp camera.cpp
	g++ -o _camera.out test_camera.cpp camera.cpp com.cpp \
	-I /usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio