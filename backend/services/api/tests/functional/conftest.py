import pytest

from testsuite.databases.pgsql import discover

pytest_plugins = ['pytest_userver.plugins.postgresql']


@pytest.fixture(scope='session')
def pgsql_local(service_source_dir, pgsql_local_create):
    databases = discover.find_schemas(
        'api',
        [service_source_dir.joinpath('../../../init/pg_tests')],
    )
    return pgsql_local_create(list(databases.values()))

# static data


@pytest.fixture()
def first_name():
    return "biba"


@pytest.fixture()
def last_name():
    return "boba"


@pytest.fixture()
def father_name():
    return "pipa"


@pytest.fixture()
def email():
    return "lala.baba@pipa.com"


@pytest.fixture()
def gvname():
    return "some_gv_name"


@pytest.fixture()
def password():
    return "some_password"


@pytest.fixture()
def register_data(gvname, password, first_name, last_name, father_name, email):
    return {
        "gv-name": gvname,
        "first-name": first_name,
        "last-name": last_name,
        "father-name": father_name,
        "email": email,
        "password": password}


# actions
async def register(service_client, register_data):
    response = await service_client.post("/register", json=register_data)

    body = response.json()

    assert response.status // 100 == 2
    assert 'status' in body and body['status'] == 'success'
    assert 'message' in body and body[
        'message'] == f'Registered user with username: {register_data["gv-name"]}'
    return response


async def login(service_client, gvname, password):
    response = await service_client.post("/login", json={
        "username": gvname,
        "password": password
    })
    assert response.status // 100 == 2
    body = response.json()
    assert 'status' in body and body['status'] == "success"
    assert 'bearer-token' in body
    return body['bearer-token']
