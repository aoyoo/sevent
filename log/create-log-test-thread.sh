 echo "g++ log-test-thread.cpp src/CTime.cpp src/CommonOperation.cpp /src/CLevel.cpp /src/CLogger.cpp /src/CLogRecord.cpp -I./include  -lpthread -lrt -g -o log-test-thread"
 g++ log-test-thread.cpp src/CTime.cpp src/CommonOperation.cpp src/CLevel.cpp src/CLogger.cpp src/CLogRecord.cpp -I./include -lpthread -lrt -g -o log-test-thread
