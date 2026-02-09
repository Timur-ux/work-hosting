import json
import pytest

async def test_auth(service_client, redis_store):
    redis_store.set("THE_USER_TOKEN", json.dumps({"username" : "Good User", "user_id": 1, "scopes": ["hello"]}))
    response = await service_client.get("/testing/hello")
    assert response.status == 401
    assert response.text == "Empty 'Authorization' header"

    response = await service_client.get(
        "/testing/hello",
        headers={'Authorization': "Bearer THE_USER_TOKEN"})

    assert response.status == 200
    assert 'text/plain' in response.headers["Content-Type"]
    assert response.text == "Hello: Good User"
