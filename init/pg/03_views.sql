CREATE VIEW denormalized_checking_queue AS
SELECT t1.id, t1.gv_name, t2.work_type, t2.old_work_number
FROM checking_queue AS t0
INNER JOIN students AS t1 ON t0.student_id = t1.id
INNER JOIN works AS t2 ON t0.work_id = t2.id
ORDER BY t0.created_at;

CREATE VIEW denormalized_marks AS
SELECT t2.user_id, t0.mark, t0.status, t1.work_type, t1.old_work_number
FROM marks AS t0
INNER JOIN works AS t1 ON t0.work_id = t1.id
INNER JOIN students AS t2 ON t0.student_id = t2.id
ORDER BY t1.work_number;
