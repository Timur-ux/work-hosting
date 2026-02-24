INSERT INTO accepted_works(student_id, work_id) VALUES (SELECT student_id, work_id FROM checking_queue WHERE id=$1)
