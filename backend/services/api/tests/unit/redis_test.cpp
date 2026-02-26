#include <userver/utest/using_namespace_userver.hpp>

/// [Unit test]
#include <userver/storages/redis/utest/redis_fixture.hpp>

using RedisTest = storages::redis::utest::RedisTest;

UTEST_F(RedisTest, Sample) {
		std::cout << 1<< '\n';
    auto client = GetClient();
		std::cout << 2<< '\n';

    client->Rpush("sample_list", "a", {}).Get();
		std::cout << 3<< '\n';
    client->Rpush("sample_list", "b", {}).Get();
		std::cout << 4<< '\n';

    const auto length = client->Llen("sample_list", {}).Get();
		std::cout << 5<< '\n';
    EXPECT_EQ(length, 2);
}
/// [Unit test]

