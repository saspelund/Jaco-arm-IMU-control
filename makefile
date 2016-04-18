Kinova_IMU_Control :
	g++ -std=c++11 -pthread Main.cpp -ldl -o Kinova_Imu_control -lboost_iostreams -lboost_system -lboost_filesystem

clean:
	\rm *.o *~ Kinova_IMU_Control
