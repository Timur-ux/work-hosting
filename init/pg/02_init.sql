-- STUDENTS --
CREATE TABLE students (
	id						SERIAL PRIMARY KEY,
	gv_name				VARCHAR(60) NOT NULL UNIQUE,
	group_number	INTEGER NOT NULL,
	first_name		VARCHAR(30) NOT NULL,
	last_name			VARCHAR(30) NOT NULL,
	father_name		VARCHAR(30) NOT NULL,
	initials			VARCHAR(2)  NOT NULL,
	email					VARCHAR(128) NOT NULL,
	password_hash VARCHAR(64) NOT NULL UNIQUE,
	created_at		TIMESTAMP NOT NULL DEFAULT NOW(),
	updated_at		TIMESTAMP NOT NULL DEFAULT NOW()
);

CREATE TABLE students_update_history (
	id						SERIAL PRIMARY KEY,
	student_id		INTEGER NOT NULL, -- no reference in order for saving history even if student is deleted
	field					VARCHAR(20) NOT NULL,
	old_value			VARCHAR(128),
	new_value			VARCHAR(128) NOT NULL,
	update_time		TIMESTAMP NOT NULL DEFAULT NOW()
);

CREATE TYPE ACTION_TYPE AS ENUM('register', 'login', 'update_profile', 'send_work_to_check', 'send_message');

CREATE TABLE students_actions (
	id					SERIAL PRIMARY KEY,
	student_id	INTEGER NOT NULL,
	action			ACTION_TYPE NOT NULL
);

-- WORKS --

CREATE TYPE WORK_TYPE AS ENUM ('LR', 'KP');

CREATE TABLE works (
	id							SERIAL PRIMARY KEY,
	work_number			INTEGER NOT NULL,
	old_work_number INTEGER NOT NULL,
	work_type				WORK_TYPE NOT NULL,
	theme						TEXT NOT NULL,
	created_at			TIMESTAMP NOT NULL DEFAULT NOW()
);

-- MARKS --

CREATE TYPE MARK_TYPE AS ENUM ('0', '2', '3', '4', '5');
CREATE TYPE WORK_PASS_STATUS AS ENUM ('null', 'cheking', 'rejected', 'accepted');
CREATE TABLE marks (
	id SERIAL PRIMARY KEY,
	student_id	INTEGER NOT NULL REFERENCES students(id) ON DELETE CASCADE,
	work_id INTEGER NOT NULL REFERENCES works(id) ON DELETE CASCADE,
	mark MARK_TYPE NOT NULL DEFAULT '0',
	status WORK_PASS_STATUS NOT NULL DEFAULT 'null'
);


-- CHECK QUEUE --

CREATE TABLE checking_queue (
	id SERIAL PRIMARY KEY,
	student_id INTEGER NOT NULL REFERENCES students(id) ON DELETE CASCADE,
	work_id INTEGER NOT NULL REFERENCES works(id) ON DELETE CASCADE,
	commit_hash VARCHAR(20),
	created_at TIMESTAMP DEFAULT NOW() NOT NULL
);

-- MESSAGES --

CREATE TABLE messages (
	id SERIAL PRIMARY KEY,
	sender_id INTEGER NOT NULL REFERENCES students(id) ON DELETE CASCADE,
	reciever_id INTEGER NOT NULL REFERENCES students(id) ON DELETE CASCADE,
	message TEXT NOT NULL
);

CREATE TABLE file_quotes (
	id SERIAL PRIMARY KEY,
	reviewer_id INTEGER NOT NULL REFERENCES students(id) ON DELETE CASCADE,

	student_id INTEGER NOT NULL REFERENCES students(id) ON DELETE CASCADE,
	work_id INTEGER NOT NULL REFERENCES works(id) ON DELETE CASCADE,

	file_path TEXT NOT NULL,
	line_number INTEGER	NOT NULL,
	created_at TIMESTAMP DEFAULT NOW() NOT NULL
);

-- CHECK HISTORY --

CREATE TYPE CHECK_RESULT AS ENUM ('rejected', 'accepted');

CREATE TABLE check_history (
	id SERIAL PRIMARY KEY,
	student_id INTEGER NOT NULL REFERENCES students(id) ON DELETE CASCADE,
	work_id INTEGER NOT NULL REFERENCES works(id) ON DELETE CASCADE,
	time TIMESTAMP DEFAULT NOW() NOT NULL,
	text TEXT NOT NULL,
	check_status CHECK_RESULT NOT NULL
);

CREATE TABLE check_quotes (
	id SERIAL PRIMARY KEY,
	check_history_id INTEGER NOT NULL REFERENCES check_history(id) ON DELETE CASCADE,
	file_quote_id INTEGER	NOT NULL REFERENCES file_quotes(id) ON DELETE CASCADE
);

-- NOTIFICATIONS --

CREATE TABLE notifications (
	id SERIAL PRIMARY KEY,
	student_id INTEGER NOT NULL REFERENCES students(id) ON DELETE CASCADE,
	message TEXT NOT NULL,
	checked BOOLEAN DEFAULT FALSE NOT NULL,

	created_at TIMESTAMP DEFAULT NOW() NOT NULL,
	checked_at TIMESTAMP DEFAULT NULL
);
