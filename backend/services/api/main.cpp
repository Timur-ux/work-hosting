#include "components/workHolder.hpp"
#include "handlers/sendWork.hpp"
#include <fmt/core.h>
#include <userver/components/component_list.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/utils/resources.hpp>
#include "generated/static_config.yaml.hpp"

using namespace userver;
using namespace SERVICE_NAMESPACE;

int main(int argc, const char *argw[]) {
  auto componentsList = components::MinimalServerComponentList()
                            .Append<WorkHolder>()
                            .Append<SendWorkHandler>();
  bool useInMemoryConfig = true;
  for (int i = 1; i < argc && useInMemoryConfig; ++i)
    if (strcmp(argw[i], "--config") == 0)
      useInMemoryConfig = false;

  auto config = components::InMemoryConfig{utils::FindResource("static_config_yaml")};

  if (useInMemoryConfig)
    return utils::DaemonMain(config, componentsList);
  else
    return utils::DaemonMain(argc, argw, componentsList);

  return 0;
}
