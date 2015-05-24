/**
 * CS3376.502 - Program 6
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

void initMatrix();
void initThreads();

void handleSignal(int);

void consumerFunc(int);
void producerFunc();

void updateCell(int, int, int, char*, int);
void updateMatrix(int);

void exitProgram(int);

boost::interprocess::named_semaphore* getSemaphore(int);
