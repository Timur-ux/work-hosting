import pytest
from conftest import register, login

@pytest.mark.pgsql('workHosting', files=['test_register.sql'])
async def test_get_student_profile(service_client, register_data):
    await register(service_client, register_data)
    token = await login(service_client, register_data["gv-name"], register_data["password"])

    response = await service_client.get("/student/profile",
                                        headers={"Authorization": f"Bearer {token}"})
    assert response.status == 200
    assert 'application/json' in response.headers["Content-Type"]
    body = response.json()
    assert 'gv-name' in body and body['gv-name'] == register_data['gv-name']
    assert 'first-name' in body and body['first-name'] == register_data['first-name']
    assert 'last-name' in body and body['last-name'] == register_data['last-name']
    assert 'father-name' in body and body['father-name'] == register_data['father-name']
    assert 'email' in body and body['email'] == register_data['email']
    assert 'group-number' in body and body['group-number'] == 109
    assert 'in-group-order' in body and body['in-group-order'] == 1

