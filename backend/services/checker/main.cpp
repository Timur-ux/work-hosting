#include "logger.hpp"
#include "work.hpp"
#include "zmq.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <sys/wait.h>
#include <unistd.h>


void startJob(Work work) {
  int pipefd[2];
  if (pipe(pipefd) == -1)
    throw std::runtime_error("Can't create a pipe");

  int pid = fork();
  if (pid == -1)
    throw std::runtime_error("Can't do fork");

  if (pid == 0) { // child process
    close(pipefd[1]);
    dup2(pipefd[0], 0);
    execlp("/scripts/checkWork.sh", "/scripts/checkWork.sh", NULL);
    perror("Failed to execlp!");
    return;
  }

  int oldstdout = dup(1);
  close(pipefd[0]);
  dup2(pipefd[1], 1);
  printf("%s\n", work.typeAsString());
  printf("%d\n", work.number());
  printf("%s\n", work.gvName().c_str());
  fflush(stdout);
  close(pipefd[1]);
  dup2(oldstdout, 1);
}

int main(int argc, const char *argw[]) {
  const char *logPath = getenv("LOG_PATH");
  if (!logPath)
    throw std::runtime_error("No LOG_PATH env specified");

  zmq::context_t context{};
  zmq::socket_t socket{context, zmq::socket_type::pull};

  const char *checkerAddr = getenv("CHECKER_ADDR");
  if (!checkerAddr)
    throw std::runtime_error("No CHECKER_ADDR env specified");

	TimedLog{} << "Checker started";
  socket.bind(checkerAddr);
	TimedLog{} << "Checker binded to " << checkerAddr << " and waiting for connections";

  zmq::message_t msg;
  while (true) {
    TimedLog{} << "Wait for message";

    auto recvRetVal = socket.recv(msg);
    TimedLog{} << "Recieved: " << msg.to_string();

    Work work = Work::from_string(msg.to_string());
    TimedLog{} << "Created work: " << work.to_string();

    startJob(std::move(work));
  }

	TimedLog{} << "Wait for jobs ending...";
  wait(NULL);
	TimedLog{} << "All started jobs ends, shutting down...";

  return 0;
}
