INSERT INTO students (gv_name, group_number, first_name, last_name, father_name, initials, email, password_hash)
VALUES ($1, $2, $3, $4, $5, $6, $7, $8)
ON CONFLICT DO NOTHING
