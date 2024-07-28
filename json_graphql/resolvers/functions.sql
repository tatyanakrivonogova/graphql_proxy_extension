getPerson='SELECT json_agg("sub/1") AS Person FROM graphql_proxy.Person, LATERAL (SELECT person.id, person.name) AS "sub/1" WHERE ((person.id) = %d);';

createPerson='INSERT INTO graphql_proxy.Person(id, name) VALUES(%d, %s);';