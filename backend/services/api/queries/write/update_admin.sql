UPDATE users
SET password_hash = $1,
		role = 'admin'
WHERE id = $1
