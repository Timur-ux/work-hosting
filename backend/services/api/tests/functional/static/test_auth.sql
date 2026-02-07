INSERT INTO auth_schema.tokens(token, user_id, scopes, username) VALUES 
('wrong-scoped-token', 1, '{"wrong-scope"}', 'Bad User'),
('THE_USER_TOKEN', 2, '{"student", "hello"}', 'Good User');
