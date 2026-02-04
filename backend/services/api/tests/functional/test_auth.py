import pytest

@pytest.mark.pgsql('auth', files=['test_data.sql'])
async def test_auth(service_client):
    response = await service_client.get("/testing/hello")
    assert response.status == 401
    assert response.text == "Empty 'Authorization' header"

    response = await service_client.get(
        "/testing/hello",
        headers={'Authorization': "Bearer THE_USER_TOKEN"})

    assert response.status == 200
    assert 'text/plain' in response.headers["Content-Type"]
    assert response.text == "Hello: Good User"
