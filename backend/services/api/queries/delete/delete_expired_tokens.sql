DELETE FROM auth_schema.tokens WHERE updated + '$1 milliseconds'::interval < NOW()
