file(GLOB_RECURSE handlers_src LIST_DIRECTORIES FALSE ./src/handlers/*.cpp)
file(GLOB_RECURSE components_src LIST_DIRECTORIES FALSE ./src/components/*.cpp)

add_library(service_lib STATIC ${handlers_src} ${components_src})
