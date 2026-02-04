INSERT INTO auth_schema.tokens(token, user_id, scopes, username) VALUES
($1, $2, $3, $4)
ON CONFLICT DO NOTHING;
