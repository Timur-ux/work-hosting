#include "components/authCache.hpp"
#include "components/authFactory.hpp"
#include "components/workHolder.hpp"
#include "generated/static_config.yaml.hpp"
#include "handlers/hello.hpp"
#include "handlers/login.hpp"
#include "handlers/register.hpp"
#include "handlers/sendWork.hpp"
#include "userver/clients/dns/component.hpp"
#include "userver/server/handlers/auth/auth_checker_factory.hpp"
#include "userver/storages/postgres/component.hpp"
#include <fmt/core.h>
#include <userver/components/component_list.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/utils/resources.hpp>

using namespace userver;
using namespace SERVICE_NAMESPACE;

int main(int argc, const char *argw[]) {
  server::handlers::auth::RegisterAuthCheckerFactory<AuthCheckerFactory>();
  auto componentsList = components::MinimalServerComponentList()
                            .Append<AuthCache>()
                            .Append<components::Postgres>("pg-database")
                            .Append<WorkHolder>()
                            .Append<SendWorkHandler>()
														.Append<RegisterHandler>()
														.Append<LoginHandler>()
														.Append<HelloHandler>()
														.Append<components::TestsuiteSupport>()
                            .Append<clients::dns::Component>();
  bool useInMemoryConfig = true;
  for (int i = 1; i < argc && useInMemoryConfig; ++i)
    if (strcmp(argw[i], "--config") == 0)
      useInMemoryConfig = false;

  auto config =
      components::InMemoryConfig{utils::FindResource("static_config_yaml")};

  if (useInMemoryConfig)
    return utils::DaemonMain(config, componentsList);
  else
    return utils::DaemonMain(argc, argw, componentsList);

  return 0;
}
