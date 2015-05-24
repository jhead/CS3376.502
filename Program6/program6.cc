/**
 * CS3376.502 - Program 6
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

#include <iostream>
#include <vector>
#include <signal.h>
#include "cdk.h"
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include "program6.h"

#define NUM_THREADS 25
#define MATRIX_WIDTH 5
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 18
#define MATRIX_NAME_STRING "Worker Matrix"

using namespace std;

WINDOW *window;
CDKSCREEN	*cdkscreen;
CDKMATRIX *matrix;

boost::barrier barrier(NUM_THREADS + 1);
boost::thread_group threads;
boost::mutex screenLock;

vector<int> cWork(NUM_THREADS + 1);
vector<char*> cStatus(NUM_THREADS + 1);
vector<boost::interprocess::interprocess_semaphore*> cLocks(NUM_THREADS + 1);

bool running = false, stopping = false;

int main() {
  // Seed the random number generator (badly)
  srand(time(0));

  // Register signal handlers
  signal(SIGINT, handleSignal);
  signal(SIGTERM, handleSignal);

  // Inititialize CDK matrix
  cout << "Initializing CDK matrix" << endl;
  initMatrix();

  // Initialize consumer threads
  initThreads();

  // Wait and release threads
  sleep(5);
  running = true;
  barrier.wait();
  sleep(5);

  // Wait for completion
  while(running && !stopping) {
    int cid = (rand() % NUM_THREADS) + 1;
    int work = (rand() % 11) + 10;

    cWork[cid] += work;

    for (int i = 0; i < cWork[cid]; i++) {
      cLocks[cid]->post();
    }

    sleep(1);
  }

  threads.join_all();

  try {
    endCDK();
  } catch (...) { }

  cout << "Exiting..." << endl;
}

/**
 * Initializes CDK matrix
 */
void initMatrix() {
  const char *rowTitles    [MATRIX_HEIGHT + 1] = { NULL, "a", "b", "c", "d", "e" };
  const char *columnTitles [MATRIX_WIDTH + 1]  = { NULL, "a", "b", "c", "d", "e" };
  int	        boxWidths    [MATRIX_WIDTH + 1]  = { NULL, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH };
  int	        boxTypes     [MATRIX_WIDTH + 1]  = { NULL, vMIXED, vMIXED, vMIXED, vMIXED, vMIXED };

  // Initialize CDK engine
  window = initscr();
  cdkscreen = initCDKScreen(window);
  initCDKColor();

  // Create CDK matrix
  matrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT,
			  MATRIX_NAME_STRING, (char **) columnTitles, (char **) rowTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (matrix == NULL) {
    printf("Error creating Matrix\n");
    exit(1);
  }

  // Initial draw
  drawCDKMatrix(matrix, true);
}

/**
 * Creates consumer and producer threads, adding each to the thread group
 */
void initThreads() {
  // Consumer threads
  boost::thread *thread;
  for(int i = 1; i <= NUM_THREADS; i++) {
    cLocks[i] = new boost::interprocess::interprocess_semaphore(0);

    thread = new boost::thread(consumerFunc, i);
    threads.add_thread(thread);
  }
}

/**
 * Handles SIGTERM and SIGINT signals (i.e. CTRL+C)
 *
 * @param {int}  signum  Signal number/code
 */
void handleSignal(int signum) {
  if (signum == SIGTERM || signum == SIGINT) {
    exitProgram(0);
  }
}

/**
 * Consumer logic
 *
 * @param {int}  id  Thread ID
 */
void consumerFunc(int id) {
  // Wait for all threads
  cStatus[id] = "B";
  updateMatrix(id);
  barrier.wait();

  if (running) {
    // Initial waiting state
    cStatus[id] = "W";
    updateMatrix(id);
    cLocks[id]->wait();

    // Consumption cycle
    while (cWork[id] > 0) {
      cWork[id]--;
      cStatus[id] = "C";
      updateMatrix(id);

      sleep(1);

      cStatus[id] = "W";
      updateMatrix(id);

      cLocks[id]->wait();
    }
  }

  // Exit state
  cStatus[id] = "E";
  updateMatrix(id);
}

/**
 * Calls updateCell on matrix cell/bin based on thread ID
 *
 * @param {int}  tid  Thread ID
 */
void updateMatrix(int tid) {
  int x = (tid % MATRIX_WIDTH != 0 ? tid - (MATRIX_WIDTH * floor(tid / MATRIX_WIDTH)) : MATRIX_WIDTH);

  int y = (tid  / MATRIX_HEIGHT);
  y += (tid % MATRIX_HEIGHT != 0 ? 1 : 0);

  updateCell(x, y, tid, cStatus[tid], cWork[tid]);
}

/**
 * Updates matrix cell/bin with appropriately formatted string based on arguments
 *
 * @param {int}    x       Horizontal cell number
 * @param {int}    y       Vertical cell number
 * @param {int}    tid     Thread ID
 * @param {char*}  status  Status character
 * @param {int}    qty     Amount/quantity of work left
 */
void updateCell(int x, int y, int tid, char* status, int qty) {
  screenLock.lock();

  string text = " tid: " +
                boost::lexical_cast<std::string>(tid) +
                " S:" +
                status +
                " Q:" +
                boost::lexical_cast<std::string>(qty);

  setCDKMatrixCell(matrix, y, x, text.c_str());
  drawCDKMatrix(matrix, true);

  screenLock.unlock();
}

/**
 * Exit program with specified code.
 * * @param {int}  code  Exit code. If zero, a proper shutdown is attempted.
 */
void exitProgram(int code) {
  if (code == 0) {
    stopping = true;

    bool complete[NUM_THREADS];
    bool status = false;

    if (!running) {
      barrier.wait();
    }

    running = false;

    while (!status) {
      status = true;

      for (int i = 1; i <= NUM_THREADS; i++) {
        complete[i - 1] = (string(cStatus[i]).compare("E") == 0);

        if (!complete[i - 1] && string(cStatus[i]).compare("W") == 0 && cWork[i] <= 0) {
          cLocks[i]->post();
          complete[i - 1] = true;
        }

        status = status && complete[i - 1];
      }

      sleep(1);
    }

    threads.join_all();
  }

  try {
    endCDK();
  } catch (...) { }

  exit(code);
}
