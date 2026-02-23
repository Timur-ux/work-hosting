-- log registrations
CREATE OR REPLACE FUNCTION log_action()
RETURNS TRIGGER AS $$
BEGIN
	INSERT INTO students_actions(user_id, action)
	VALUES (NEW.user_id, TG_ARGV[0]::action_type);
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER log_register
AFTER INSERT ON students
FOR EACH ROW
EXECUTE FUNCTION log_action('register');

-- create mark placeholders when new student created
CREATE OR REPLACE FUNCTION insert_marks_on_student_added()
RETURNS TRIGGER AS $$
DECLARE
	work RECORD;
BEGIN
	FOR work IN
		SELECT id FROM works
	LOOP
		INSERT INTO marks(student_id, work_id)
		VALUES (NEW.id, work.id);
	END LOOP;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER insert_marks_for_new_student
AFTER INSERT ON students
FOR EACH ROW
EXECUTE FUNCTION insert_marks_on_student_added();

-- create mark placeholders when new work added
CREATE OR REPLACE FUNCTION insert_marks_on_work_added()
RETURNS TRIGGER AS $$
DECLARE
	student RECORD;
BEGIN
	FOR student IN
		SELECT id FROM students
	LOOP
		INSERT INTO marks(student_id, work_id)
		VALUES (student.id, NEW.id);
	END LOOP;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER insert_marks_for_new_work
AFTER INSERT ON works
FOR EACH ROW
EXECUTE FUNCTION insert_marks_on_work_added();


CREATE OR REPLACE FUNCTION update_mark_status()
RETURNS TRIGGER AS $$
BEGIN
	UPDATE marks
	SET status=TG_ARGV[0]::work_pass_status
	WHERE work_id=NEW.work_id;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- set mark status to 'checking' when corresponding work added to checking queue
CREATE OR REPLACE TRIGGER set_mark_to_checking_on_adding_to_queue
AFTER INSERT ON checking_queue
FOR EACH ROW
EXECUTE FUNCTION update_mark_status('checking');

-- set mark status to 'accepted' when corresponding work added to accepted_works
CREATE OR REPLACE TRIGGER set_mark_to_accepted
AFTER INSERT ON accepted_works
FOR EACH ROW
EXECUTE FUNCTION update_mark_status('accepted');

-- set mark status to 'rejected' when corresponding work added to rejected_works
CREATE OR REPLACE TRIGGER set_mark_to_rejected
AFTER INSERT ON rejected_works
FOR EACH ROW
EXECUTE FUNCTION update_mark_status('rejected');
