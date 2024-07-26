getPerson='SELECT json_agg("sub/1") AS person FROM person, LATERAL (SELECT person.name) AS "sub/1" WHERE ((person.id) = %d);';

createPerson='INSERT INTO Person(id, name) VALUES(%d, %s);';