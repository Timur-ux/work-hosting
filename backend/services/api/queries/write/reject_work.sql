INSERT INTO rejected_works(student_id, work_id) VALUES (SELECT student_id, work_id FROM checking_queue WHERE id=$1)

