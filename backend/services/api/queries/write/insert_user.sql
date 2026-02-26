INSERT INTO users(username, password_hash, role)
VALUES ($1,$2,$3)
ON CONFLICT DO NOTHING
