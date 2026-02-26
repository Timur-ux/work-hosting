import pytest

from conftest import login, register


@pytest.mark.pgsql('workHosting', files=['test_register.sql'])
async def test_login_incorrect(service_client):
    response = await service_client.post("/login", json={
        "username": "wrong-username",
        "password": "wrong-password"
    })

    assert response.status == 404
    assert response.text == "Login or password incorrect"


@pytest.mark.pgsql('workHosting', files=['test_register.sql'])
async def test_login(service_client, register_data):
    await register(service_client, register_data)
    await login(service_client, register_data["gv-name"], register_data["password"])
