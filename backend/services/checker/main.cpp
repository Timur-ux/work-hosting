#include "CheckerRequest.hpp"
#include "logger.hpp"
#include "work.hpp"
#include "zmq.hpp"
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <sys/wait.h>
#include <unistd.h>
#include "nlohmann/json.hpp"
#include "Serializers.hpp"

using json = nlohmann::json;

void startJob(CheckerRequest r) {
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
  printf("%s\n", r.work.typeAsString());
  printf("%d\n", r.work.number());
  printf("%s\n", r.work.gvName().c_str());
	printf("%s\n", r.first_name.c_str());
	printf("%s\n", r.last_name.c_str());
	printf("%s\n", r.father_name.c_str());
	printf("%d\n", r.group_number);
	printf("%d\n", r.in_group_order);
  fflush(stdout);
  close(pipefd[1]);
  dup2(oldstdout, 1);
}

int main(int argc, const char *argw[]) {
  if (!getenv("CHECKING_QUEUE_UPDATER_HOST"))
    throw std::runtime_error("No CHECKING_QUEUE_UPDATER_HOST env specified");
  if (!getenv("CHECKING_QUEUE_UPDATER_PORT"))
    throw std::runtime_error("No CHECKING_QUEUE_UPDATER_PORT env specified");

  const char *logPath = getenv("LOG_PATH");
  if (!logPath)
    throw std::runtime_error("No LOG_PATH env specified");

  zmq::context_t context{};
  zmq::socket_t socket{context, zmq::socket_type::pull};
	socket.set(zmq::sockopt::rcvtimeo, 3000);

  const char *checkerAddr = getenv("CHECKER_ADDR");
  if (!checkerAddr)
    throw std::runtime_error("No CHECKER_ADDR env specified");

	TimedLog{} << "Checker started";
  socket.bind(checkerAddr);
	TimedLog{} << "Checker binded to " << checkerAddr << " and waiting for connections";

  zmq::message_t msg;
  while (true) {
    auto recvRetVal = socket.recv(msg);
		if(!recvRetVal)
			continue;
    TimedLog{} << "Recieved: " << msg.to_string();
		CheckerRequest request = nlohmann::json::parse(msg.to_string());

    startJob(std::move(request));
		std::cout << "Job created"<< '\n';
  }

	TimedLog{} << "Wait for jobs ending...";
  wait(NULL);
	TimedLog{} << "All started jobs ends, shutting down...";

  return 0;
}
