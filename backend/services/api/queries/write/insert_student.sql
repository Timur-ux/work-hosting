INSERT INTO students (user_id, gv_name, group_number, in_group_order, first_name, last_name, father_name, initials, email)
VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)
ON CONFLICT DO NOTHING
