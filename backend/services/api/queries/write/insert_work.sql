INSERT INTO works(work_type, old_work_number, work_number, theme) VALUES ($1, $2, (SELECT count(*) FROM works), $3)
