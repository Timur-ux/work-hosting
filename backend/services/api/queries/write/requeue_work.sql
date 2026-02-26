INSERT INTO checking_queue(student_id, work_id) SELECT student_id, work_id FROM checking_queue WHERE id=$1
