getPerson='SELECT json_agg(person) AS person FROM (SELECT person.id, person.name FROM graphql_proxy.Person WHERE person.id = $1) AS person;';

createPerson='INSERT INTO graphql_proxy.Person(id, name) VALUES($1, $2);';

updatePerson='UPDATE graphql_proxy.Person SET name = $1 WHERE id = $2;';

deletePerson='DELETE FROM graphql_proxy.Person WHERE id = $1;';
