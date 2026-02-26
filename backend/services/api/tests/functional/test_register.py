import pytest

from conftest import register


@pytest.mark.pgsql('workHosting', files=['test_register.sql'])
async def test_register(service_client, register_data):
    await register(service_client, register_data)
    

