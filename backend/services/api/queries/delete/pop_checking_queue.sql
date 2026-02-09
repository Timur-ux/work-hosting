DELETE FROM checking_queue
WHERE id = (
	SELECT id 
	FROM checking_queue
	ORDER BY created_at ASC
	LIMIT 1
);
